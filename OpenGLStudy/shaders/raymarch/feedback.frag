#version 440 core

out vec4 FragColor;

in vec2 UV;
in vec3 FragPos;
in vec3 Eye;

#include "material.glsl"
#include "sdf.glsl"
#include "light.glsl"
#include "../noise/hash.glsl"

uniform ivec3 fluidSize;

uniform float time;
uniform mat4 view;

uniform float rayStepSize;
uniform float shadowStepSize;
uniform float maxShadowingLength;
uniform float maxDensity;

uniform float directionalLightExtinction;
uniform float ambientLightExtinction;

uniform vec3 backgroundColor;

layout(binding = 0) uniform sampler3D velocitySampler;
layout(binding = 1) uniform sampler3D pressureSampler;
layout(binding = 2) uniform sampler3D curlSampler;
layout(binding = 3) uniform sampler3D densitySampler;
layout(binding = 4) uniform sampler1D cloudColorCurve;

uniform DirectionalLight dirLight = DirectionalLight(vec3(-1), vec3(1));

uniform BlinnPhongMaterial material1 = BlinnPhongMaterial(vec3(0.26, 0.95, 0.39), 5.0);
uniform BlinnPhongMaterial material2 = BlinnPhongMaterial(vec3(0.4, 0.24, 0.16), 0.3);
uniform BlinnPhongMaterial material3 = BlinnPhongMaterial(vec3(0.01, 0.2, 0.5), 1.5);

vec2 intersectBox(vec3 rayOrigin, vec3 rayDir, vec3 boxMin, vec3 boxMax) 
{
  vec3 tMin = (boxMin - rayOrigin) / rayDir;
  vec3 tMax = (boxMax - rayOrigin) / rayDir;
  vec3 t1 = min(tMin, tMax);
  vec3 t2 = max(tMin, tMax);
  float tNear = max(max(t1.x, t1.y), t1.z);
  float tFar = min(min(t2.x, t2.y), t2.z);
  return vec2(tNear, tFar);
}

vec3 lighting(vec3 rayPos, vec3 rayDir, vec3 norm, BlinnPhongMaterial material, float steps, float depth)
{
	float ambient = 0.7 * pow(1.02, -steps);
	vec3 directional = blinnPhongDirectionalLighting(dirLight, norm, rayDir, material) * 0.4;
	vec3 light = (directional + ambient) * material.color;
	float fog = exp(-depth*0.0005);
	
	return mix(vec3(0), light, fog);
}

vec3 snap(vec3 ray, float stepSize)
{
	vec3 viewPlaneNormal = view[2].xyz;
	float planeDist = dot(ray, viewPlaneNormal);
	float snapDist = floor(planeDist / stepSize) * stepSize;
	return ray - (planeDist - snapDist) * viewPlaneNormal;
}

vec4 sampleFluid(sampler3D s, vec3 p, vec3 boxMin, vec3 boxMax)
{
	return texture(s, (p - boxMin) / (boxMax - boxMin));
}

uniform vec3 scattering = 25.0 * vec3(0.25,0.5,1.0);
uniform vec3 absorption = 0.0 * vec3(0.75,0.5,0.0);
#define extinction (absorption + scattering)

vec4 getShadowTransmittance(vec3 rayPos, vec3 boxMin, vec3 boxMax)
{
	float depth = 0.0;
	vec4 shadow = vec4(1.0);
	float shadowingLength = min(maxShadowingLength, intersectBox(rayPos, -dirLight.direction, boxMin, boxMax ).y);
	int numShadowSteps = int(shadowingLength / shadowStepSize);
	for (int j = 0; j < numShadowSteps; j++)
	{
		vec3 lightRayPos = rayPos + depth * -dirLight.direction;
		vec4 ld = sampleFluid(densitySampler, lightRayPos, boxMin, boxMax);
		shadow *= exp(-ld.r * extinction * shadowStepSize);
		depth += shadowStepSize;
	}
	return shadow;
}

void main()
{
    vec3 rayDir = normalize(FragPos - Eye);
	float raySnap = rayStepSize / dot(-view[2].xyz, rayDir);
	
	vec3 boxMax = vec3( fluidSize) * 0.5;
	vec3 boxMin = vec3(-fluidSize) * 0.5;
	vec2 boxIntersections = intersectBox(Eye, rayDir, boxMin + 0.5, boxMax - 0.5);
	boxIntersections = max(boxIntersections, 0);
	float depth = boxIntersections.x;
	float firstPlane = floor(depth / raySnap) * raySnap;
	int numSteps = int((boxIntersections.y - depth) / raySnap) + 2;

	float jitter = hash13(vec3(UV, time)) * 2 - 1;
	//firstPlane += jitter * raySnap;
	
	vec4 density = vec4(0);
	vec4 transmittence = vec4(1.0);
	vec4 directionalLightEnergy = vec4(0);
	vec4 ambientLightEnergy = vec4(0);
    for (int i = 0; i < numSteps; i++)
    {
        vec3 rayPos = Eye + depth * rayDir;

		vec4 d = sampleFluid(densitySampler, rayPos, boxMin, boxMax);
		density += d * raySnap;

		if (d.r > 0.00001)
		{
			float shadowDepth = 0.0;
			vec4 shadowDensity = vec4(0.0);
			float shadowingLength = min(maxShadowingLength, intersectBox(rayPos, -dirLight.direction, boxMin, boxMax ).y);
			int numShadowSteps = int(shadowingLength / shadowStepSize);
			for (int j = 0; j < numShadowSteps; j++)
			{
				vec3 lightRayPos = rayPos + shadowDepth * -dirLight.direction;
				vec4 ld = sampleFluid(densitySampler, lightRayPos, boxMin, boxMax);
				shadowDensity += ld * shadowStepSize;
				shadowDepth += shadowStepSize;
			}

			vec4 skyDensity = sampleFluid(densitySampler, rayPos + vec3(0, 0, 0.05), boxMin, boxMax);
			skyDensity += sampleFluid(densitySampler, rayPos + vec3(0, 0, 0.1), boxMin, boxMax);
			skyDensity += sampleFluid(densitySampler, rayPos + vec3(0, 0, 0.2), boxMin, boxMax);
			
			directionalLightEnergy += exp(-shadowDensity * directionalLightExtinction) * density * transmittence;
			ambientLightEnergy += exp(-skyDensity * ambientLightExtinction) * density * transmittence * vec4(backgroundColor, 1);
			transmittence *= 1.0 - clamp(density, 0.0, 1.0);
		}

		if (i == 0)
			depth = firstPlane;
		if (i == numSteps - 2)
			depth = boxIntersections.y;
		else
			depth += raySnap;
    }
	
	vec3 color = texture(cloudColorCurve, clamp(directionalLightEnergy.r,0.0, 1.0)).xyz + ambientLightEnergy.xyz;
	FragColor = vec4(mix(color, backgroundColor, clamp(transmittence.r, 0.0, 1.0)), 1.0);
    //FragColor = vec4(mix(color + backgroundColor, color, min(1.0, density.r  * 0.4) ), 1);
}
