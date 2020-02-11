//?#version 440

#define PARTICLES_PER_CELL 8

#define NULL_ID 0

layout(r32ui) uniform uimage3D particleMapImage;
uniform usampler3D particleMapSampler;

uniform int numParticles;
uniform ivec3 simulationSize;
uniform float timestep;
uniform bool resetParticles;

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

#define UNSORTED -1
#define OUT_OF_BOUNDS -2
#define FULL -3
int isSorted(vec3 particlePosition, uint particleID)
{
	ivec3 particleCell = ivec3(particlePosition);

	if ( any(lessThan(particleCell, ivec3(0) )) || any(greaterThan(particleCell, simulationSize)) )
		return OUT_OF_BOUNDS;

	particleCell *= ivec3(PARTICLES_PER_CELL, 1, 1);
	for (int i = 0; i < PARTICLES_PER_CELL; i++)
	{
		uint storedParticleID = texelFetch(particleMapSampler, particleCell + ivec3(i, 0, 0), 0).x;
		if (storedParticleID == NULL_ID)
			return UNSORTED;
		else if (storedParticleID == particleID)
			return i;
	}

	return FULL;
}



/*
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

void sampleParticleMapAdjacent(ivec3 particleCell, out uint particleIDs[3][3][3][PARTICLES_PER_CELL])
{
	for (int i = -1; i < 2; i++)
	for (int j = -1; j < 2; j++)
	for (int k = -1; k < 2; k++)
		sampleParticleMap(particleCell + ivec3(i, j, k), particleIDs[i + 1][j + 1][k + 1]);
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
*/