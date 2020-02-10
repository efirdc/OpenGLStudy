//?#version 440

#define PARTICLES_PER_CELL 8

#define NULL_ID 999999999
#define UNSORTED -1
#define OUT_OF_BOUNDS -2

layout(binding = 0, rgba32ui) writeonly restrict uniform uimage3D particleMapImage1;
layout(binding = 0) uniform usampler3D particleMapSampler1;
layout(binding = 1, rgba32ui) writeonly restrict uniform uimage3D particleMapImage2;
layout(binding = 1) uniform usampler3D particleMapSampler2;

uniform int numParticles;
uniform ivec3 simulationSize;

struct Particle
{
	vec3 position;
	vec3 velocity;
};

layout(std430, binding = 0) buffer sourceParticleSSBO
{
	Particle sourceParticles[];
};

layout(std430, binding = 1) buffer destParticleSSBO
{
	Particle destParticles[];
};

void sampleParticleMap(ivec3 particleCell, out uint particleIDs[PARTICLES_PER_CELL])
{
	uvec4 fetch1 = texelFetch(particleMapSampler1, particleCell, 0);
	uvec4 fetch2 = texelFetch(particleMapSampler2, particleCell, 0);
	for (int i = 0; i < 4; i++)
	{
		particleIDs[i] = fetch1[i];
		particleIDs[i + 4] = fetch2[i];
	}
}

void writeParticleMap(ivec3 particleCell, uint particleIDs[PARTICLES_PER_CELL])
{
	uvec4 write1, write2;
	for (int i = 0; i < 4; i++)
	{
		write1[i] = particleIDs[i];
		write2[i] = particleIDs[i + 4];
	}
	imageStore(particleMapImage1, particleCell, write1);
	imageStore(particleMapImage2, particleCell, write1);
}


int isSorted(vec3 particlePosition, uint particleID)
{
	ivec3 particleCell = ivec3(particlePosition);

	if ( any(lessThan(particleCell, ivec3(0) )) || any(greaterThan(particleCell, simulationSize)) )
		return OUT_OF_BOUNDS;

	uint particleIDs[PARTICLES_PER_CELL];
	sampleParticleMap(particleCell, particleIDs);
	for (int i = 0; i < PARTICLES_PER_CELL; i++)
		if (particleIDs[i] == particleID)
			return i;

	return UNSORTED;
}