//?#version 440

#define LOCAL_SIZE_X 8
#define LOCAL_SIZE_Y 8
#define LOCAL_SIZE_Z 8

#define IMAGE_QUALIFIERS

#ifndef FMT_FLUID 
#define FMT_FLUID rgba32f
#endif
#ifndef FMT_PRESSURE
#define FMT_PRESSURE rg32f
#endif
#ifndef FMT_CURL
#define FMT_CURL rgba32f
#endif
#ifndef FMT_DENSITY
#define FMT_DENSITY r32f
#endif
#ifndef FMT_SHADOWMAP
#define FMT_SHADOWMAP rgba32f
#endif

layout(binding = 0, FMT_FLUID) IMAGE_QUALIFIERS uniform image3D fluidImage;
layout(binding = 1, FMT_PRESSURE) IMAGE_QUALIFIERS uniform image3D pressureImage;
layout(binding = 2, FMT_CURL) IMAGE_QUALIFIERS uniform image3D curlImage;
layout(binding = 3, FMT_DENSITY) IMAGE_QUALIFIERS uniform image3D densityImage;
layout(binding = 5, FMT_SHADOWMAP) IMAGE_QUALIFIERS uniform image3D shadowMapImage;

layout(binding = 0) uniform sampler3D fluidSampler;
layout(binding = 1) uniform sampler3D pressureSampler;
layout(binding = 2) uniform sampler3D curlSampler;
layout(binding = 3) uniform sampler3D densitySampler;
layout(binding = 5) uniform sampler3D shadowMapSampler;

uniform float time;
uniform float deltaTime;
uniform float timeScale;
uniform mat4 view;

struct FluidSplat { 
	float radius, velocity, pressure, density;
};

uniform ivec3 fluidSize;
uniform int numPressureIterations;
uniform float velocityDissipation;
uniform float densityDissipation;
uniform float vorticityScalar;
uniform FluidSplat mouseSplat;
uniform FluidSplat externalSplat;

#define IMAGE_LOAD_ADJACENT(image, coords, left, right, top, bottom, front, back)\
{\
	left = imageLoad(image, icoords + ivec3(-1, 0, 0));\
	right = imageLoad(image, icoords + ivec3(1, 0, 0));\
	top = imageLoad(image, icoords + ivec3(0, 1, 0));\
	bottom = imageLoad(image, icoords + ivec3(0, -1, 0));\
	front = imageLoad(image, icoords + ivec3(0, 0, 1));\
	back = imageLoad(image, icoords + ivec3(0, 0, -1));\
}

void texelFetchAdjacent(sampler3D s, ivec3 icoords, 
	out vec4 left, 
	out vec4 right, 
	out vec4 top, 
	out vec4 bottom, 
	out vec4 front, 
	out vec4 back)
{
	left = texelFetchOffset(s, icoords, 0, ivec3(-1, 0, 0));
	right = texelFetchOffset(s, icoords, 0, ivec3(1, 0, 0));
	top = texelFetchOffset(s, icoords, 0, ivec3(0, 1, 0));
	bottom = texelFetchOffset(s, icoords, 0, ivec3(0, -1, 0));
	front = texelFetchOffset(s, icoords, 0, ivec3(0, 0, 1));
	back = texelFetchOffset(s, icoords, 0, ivec3(0, 0, -1));
}

vec4 texelFetchPressure(sampler3D s, ivec3 icoords)
{
	if (any(lessThan(icoords, vec3(0))) || any(greaterThan(icoords, fluidSize)))
		return vec4(0);
	return texelFetch(s, icoords, 0);
}


void texelFetchPressureAdjacent(sampler3D s, ivec3 icoords, 
	out vec4 left, 
	out vec4 right, 
	out vec4 top, 
	out vec4 bottom, 
	out vec4 front, 
	out vec4 back)
{
	left = texelFetchPressure(s, icoords + ivec3(-1, 0, 0));
	right = texelFetchPressure(s, icoords + ivec3(1, 0, 0));
	top = texelFetchPressure(s, icoords + ivec3(0, 1, 0));
	bottom = texelFetchPressure(s, icoords + ivec3(0, -1, 0));
	front = texelFetchPressure(s, icoords + ivec3(0, 0, 1));
	back = texelFetchPressure(s, icoords + ivec3(0, 0, -1));
}

