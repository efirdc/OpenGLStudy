#version 440
out vec4 FragColor;

in vec2 UV;
in vec3 FragPos;
in vec3 Eye;

layout(binding = 0) uniform sampler3D rdSampler;
layout(binding = 1) uniform sampler1D lightGradientSampler;

#include "../raymarch/material.glsl"
#include "../raymarch/light.glsl"

uniform float time;
uniform float deltaTime;
uniform float timeScale;
uniform mat4 view;

uniform vec3 color1;
uniform vec3 color2;

uniform float aspect;

uniform ivec2 screenSize;
uniform ivec3 simulationSize;
uniform vec2 screenMousePos;

uniform float rayStepSize;

uniform float levelSurface;

uniform DirectionalLight dirLight;

uniform PhysicalMaterial physicalMaterial;

void textureAdjacent(sampler3D s, out vec4 samples[3][3][3], in vec3 p, in vec3 texelSize, in vec3 boxMin, in vec3 boxMax)
{
	vec3 volumePos = (p - boxMin) / (boxMax - boxMin);
	for (int i = -1; i < 2; i++)
	for (int j = -1; j < 2; j++)
	for (int k = -1; k < 2; k++)
		samples[i+1][j+1][k+1] = texture(s, volumePos + vec3(i, j, k) * texelSize);
}

vec3 computeSobel(in vec4 texels[3][3][3], int channel)
{
	const float kernel[3][3][3] =
	{
		{{1.0, 2.0, 1.0},
		{2.0, 4.0, 2.0},
		{1.0, 2.0, 1.0}},

		{{0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0}},

		{{-1.0, -2.0, -1.0},
		{-2.0, -4.0, -2.0},
		{-1.0, -2.0, -1.0}}
	};
	vec3 result = vec3(0.0);
	for (int i = 0; i < 3; i++)
	for (int j = 0; j < 3; j++)
	for (int k = 0; k < 3; k++)
	{
		result.x -= texels[i][j][k][channel] * kernel[i][j][k];
		result.y -= texels[i][j][k][channel] * kernel[j][k][i];
		result.z -= texels[i][j][k][channel] * kernel[k][i][j];
	}
	return normalize(result);
}

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

vec4 sampleVolume(sampler3D s, vec3 p, vec3 boxMin, vec3 boxMax)
{
	return texture(s, (p - boxMin) / (boxMax - boxMin));
}

void marchStep(in float depth, in float stepSize, in vec3 rayDir, inout vec4 density, in vec3 boxMin, in vec3 boxMax)
{
	vec3 rayPos = Eye + depth * rayDir;
	density += sampleVolume(rdSampler, rayPos, boxMin, boxMax) * stepSize;
}


void main()
{
	//FragColor = vec4(texture(rdSampler, vec3(UV, 0.5)).rgb, 1.0);
	//return;

	vec3 rayDir = normalize(FragPos - Eye);
	vec3 boxMax = vec3( simulationSize) * 0.5;
	vec3 boxMin = vec3(-simulationSize) * 0.5;
	vec2 boxIntersections = intersectBox(Eye, rayDir, boxMin + 0.5, boxMax - 0.5);

	vec3 light = color1;
    if (boxIntersections.y > boxIntersections.x)
	{
		// Snap step size to view alligned planes
		float stepSize = rayStepSize / dot(-view[2].xyz, rayDir);

		boxIntersections = max(boxIntersections, 0);
		
		int numPlanes = int((boxIntersections.y - boxIntersections.x) / stepSize);
		int numSteps = numPlanes + 3;
		float firstPlaneDepth = floor(boxIntersections.x / stepSize) * stepSize;
		
		float depth = firstPlaneDepth.x;
		float prevDepth = depth;
		
		for (int i = 0; i < numSteps; i++)
		{
			float clampedDepth = clamp(depth, boxIntersections.x, boxIntersections.y);
			vec3 rayPos = Eye + clampedDepth * rayDir;
			vec4 density = sampleVolume(rdSampler, rayPos, boxMin, boxMax);
			if (density.y > levelSurface)
			{
				
				float start = clampedDepth;
				float end = max(prevDepth, boxIntersections.x);
				for (int j = 0; j < 16; j++)
				{
					float mid = (start + end) * 0.5;
					rayPos = Eye + mid * rayDir;
					density = sampleVolume(rdSampler, rayPos, boxMin, boxMax);
					if (density.y < levelSurface)
						end = mid;
					else
						start = mid;
					depth = mid;
				}
			
				BlinnPhongMaterial material = BlinnPhongMaterial(color1, 15.6);
				vec4 samples[3][3][3];
				textureAdjacent(rdSampler, samples, rayPos, 1.0 / simulationSize, boxMin, boxMax);
				vec3 norm = computeSobel(samples, 1);
				light = physicalDirectionalLighting(dirLight, norm, rayDir, physicalMaterial);
				vec3 ambient = vec3(0.03) * physicalMaterial.albedo * physicalMaterial.ao;
				light = light + ambient;
				light = light / (light + vec3(1.0));
				break;
			}

			prevDepth = clampedDepth;
			depth += stepSize;
		}
    }
	FragColor = vec4(pow(light, vec3(1.0/2.2)), 1.0);
}