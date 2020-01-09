#version 440 core

out vec4 FragColor;

in vec2 UV;
in vec3 FragPos;
in vec3 Eye;

#include "material.glsl"
#include "sdf.glsl"
#include "light.glsl"
#include "../noise/hash.glsl"

#define kPI 3.13415926

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

#define ISOTROPIC 0
#define RAYLEIGH 1
#define MIE 2
uniform int phaseMode;

#define MIE_HENYEY_GREENSTEIN 0
#define MIE_SCHLICK_APPROXIMATION 1 
uniform int mieMode;
uniform bool mieMultiLobe;
uniform float mieG1;
uniform float mieG2;
uniform float mieLobeMix;

#define MAX_SCATTERING_OCTAVES 8
struct ScatteringData
{
	vec3 extinction;
	vec3 scattering;
	float phaseScalar;
};
uniform ScatteringData scatteringData[MAX_SCATTERING_OCTAVES];
uniform int multiScatteringOctaves;

layout(binding = 0) uniform sampler3D velocitySampler;
layout(binding = 2) uniform sampler3D curlSampler;
layout(binding = 3) uniform sampler3D densitySampler;
layout(binding = 4) uniform sampler1D cloudColorCurve;
layout(binding = 5) uniform sampler3D shadowMapSampler;

uniform DirectionalLight dirLight;

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

/*
vec3 getShadowTransmittance(vec3 rayPos, vec3 boxMin, vec3 boxMax)
{
	float depth = 0.0;
	vec3 shadow = vec3(1.0);
	float shadowingLength = min(maxShadowingLength, intersectBox(rayPos, -dirLight.direction, boxMin, boxMax ).y);
	int numShadowSteps = int(shadowingLength / shadowStepSize) + 1;
	for (int j = 0; j < numShadowSteps; j++)
	{
		vec3 lightRayPos = rayPos + depth * -dirLight.direction;
		vec4 ld = sampleFluid(densitySampler, lightRayPos, boxMin, boxMax);
		shadow *= exp(-ld.r * extinction * shadowStepSize);
		depth += shadowStepSize;
	}
	return shadow;
}
*/

float miePhaseFunction(vec3 rayDir, vec3 lightDir, float g)
{
	float cosTheta = dot(rayDir, -lightDir);
	if (mieMode == MIE_HENYEY_GREENSTEIN)
		return (1 - g * g) / ( 4.0 * kPI * pow(1.0 + g * g - 2.0 * g * cosTheta, 1.5) );
	else if (mieMode == MIE_SCHLICK_APPROXIMATION)
	{
		float k = 1.55 * g - 0.55 * g * g * g;
		float a = 1 + k * cosTheta;
		return (1.0 - k * k) / (4.0 * kPI * a * a);
	}
}

float phaseFunction(vec3 rayDir, vec3 lightDir, float phaseScalar)
{
	if (phaseMode == ISOTROPIC)
	{
		return 1.0 / (4.0 * kPI);
	}
	else if (phaseMode == RAYLEIGH)
	{
		float cosTheta = dot(rayDir, -lightDir);
		return 3.0 / (16.0 * kPI) * (1 + cosTheta * cosTheta);
	}
	else if (phaseMode == MIE)
	{
		if (!mieMultiLobe)
			return miePhaseFunction(rayDir, lightDir, mieG1 * phaseScalar);
		return mix(miePhaseFunction(rayDir, lightDir, -mieG2 * phaseScalar), miePhaseFunction(rayDir, lightDir, mieG1 * phaseScalar), mieLobeMix);
	}
}



void marchStep(
		in float depth, 
		in float stepSize, 
		in vec3 rayDir, 
		inout vec3 scatteredLuminance, 
		inout vec3 transmittance[8],
		in vec3 boxMin,
		in vec3 boxMax)
{
	vec3 rayPos = Eye + depth * rayDir;
	float density = sampleFluid(densitySampler, rayPos, boxMin, boxMax).r;
	if (density > 0.0000000001)
	{
		vec3 shadow = sampleFluid(shadowMapSampler, rayPos, boxMin, boxMax).xyz; //getShadowTransmittance(rayPos, boxMin, boxMax);
		vec3 S0 = dirLight.color * shadow * density;
		for (int i = 0; i < multiScatteringOctaves; i++)
		{
			ScatteringData sd = scatteringData[i];
			vec3 S = S0 * sd.scattering * phaseFunction(rayDir, dirLight.direction, sd.phaseScalar);
			vec3 sampleExtinction = max(vec3(0.0000000001), density * sd.extinction);
			vec3 Sint = (S - S * exp(-sampleExtinction * stepSize)) / sampleExtinction;
			transmittance[i] *= exp(-sampleExtinction * stepSize);
			scatteredLuminance += transmittance[i] * Sint;
		}
	}
}


void main()
{
    vec3 rayDir = normalize(FragPos - Eye);
	vec3 boxMax = vec3( fluidSize) * 0.5;
	vec3 boxMin = vec3(-fluidSize) * 0.5;
	vec2 boxIntersections = intersectBox(Eye, rayDir, boxMin + 0.5, boxMax - 0.5);

	vec3 scatteredLuminance = vec3(0.0);
	vec3 transmittance[MAX_SCATTERING_OCTAVES] = vec3[MAX_SCATTERING_OCTAVES]
	(
		vec3(1), vec3(1), vec3(1), vec3(1),
		vec3(1), vec3(1), vec3(1), vec3(1)
	);

	if (boxIntersections.y > boxIntersections.x)
	{
		// Snap step size to view alligned planes
		float stepSize = rayStepSize / dot(-view[2].xyz, rayDir);

		boxIntersections = max(boxIntersections, 0);
		float firstPlaneDepth = ceil(boxIntersections.x / stepSize) * stepSize;
		int numPlanes = int((boxIntersections.y - boxIntersections.x) / stepSize);

		//float jitter = hash13(vec3(UV, time)) * 2 - 1;
		//firstPlane += jitter * stepSize;

		float firstStepSize = abs(firstPlaneDepth - boxIntersections.x);
		marchStep(boxIntersections.x, firstStepSize, rayDir, scatteredLuminance, transmittance, boxMin, boxMax);
		float depth = boxIntersections.x + firstStepSize;
		for (int i = 0; i < numPlanes; i++)
		{
			marchStep(depth, stepSize, rayDir, scatteredLuminance, transmittance, boxMin, boxMax);
			depth += stepSize;
		}
		float lastStepSize = boxIntersections.y - depth;
		marchStep(boxIntersections.y, lastStepSize, rayDir, scatteredLuminance, transmittance, boxMin, boxMax);
    }
	vec3 color = transmittance[0] * backgroundColor + scatteredLuminance;
	FragColor = vec4(pow(color, vec3(1/2.2)), 1.0);

	//vec3 color = texture(cloudColorCurve, clamp(directionalLightEnergy.r,0.0, 1.0)).xyz + ambientLightEnergy.xyz;
	//FragColor = vec4(mix(color, backgroundColor, clamp(transmittence.r, 0.0, 1.0)), 1.0);
    //FragColor = vec4(mix(color + backgroundColor, color, min(1.0, density.r  * 0.4) ), 1);
}
