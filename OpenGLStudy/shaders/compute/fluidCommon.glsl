//?#version 440

#define LOCAL_SIZE_X 8
#define LOCAL_SIZE_Y 8
#define LOCAL_SIZE_Z 8

#define IMAGE_QUALIFIERS writeonly restrict

#define FLUID_FORMAT rgba32f
#define PRESSURE_FORMAT rg32f
#define CURL_FORMAT rgba32f
#define DENSITY_FORMAT r32f
#define SHADOWMAP_FORMAT rgba32f

#define FLUID_IMAGE_TYPE image3D
#define PRESSURE_IMAGE_TYPE image3D
#define CURL_IMAGE_TYPE image3D
#define DENSITY_IMAGE_TYPE image3D
#define SHADOWMAP_IMAGE_TYPE image3D

layout(binding = 0, FLUID_FORMAT) IMAGE_QUALIFIERS uniform FLUID_IMAGE_TYPE fluidImage;
layout(binding = 1, PRESSURE_FORMAT) IMAGE_QUALIFIERS uniform PRESSURE_IMAGE_TYPE pressureImage;
layout(binding = 2, CURL_FORMAT) IMAGE_QUALIFIERS uniform CURL_IMAGE_TYPE curlImage;
layout(binding = 5, SHADOWMAP_FORMAT) IMAGE_QUALIFIERS uniform SHADOWMAP_IMAGE_TYPE shadowMapImage;
layout(binding = 3, DENSITY_FORMAT) IMAGE_QUALIFIERS uniform DENSITY_IMAGE_TYPE densityImage;

#define FLUID_SAMPLER_TYPE sampler3D
#define PRESSURE_SAMPLER_TYPE sampler3D
#define CURL_SAMPLER_TYPE sampler3D
#define DENSITY_SAMPLER_TYPE sampler3D
#define SHADOWMAP_SAMPLER_TYPE sampler3D

layout(binding = 0) uniform FLUID_SAMPLER_TYPE fluidSampler;
layout(binding = 1) uniform PRESSURE_SAMPLER_TYPE pressureSampler;
layout(binding = 2) uniform CURL_SAMPLER_TYPE curlSampler;
layout(binding = 3) uniform DENSITY_SAMPLER_TYPE densitySampler;
layout(binding = 5) uniform SHADOWMAP_SAMPLER_TYPE shadowMapSampler;
layout(binding = 7) uniform sampler1D audioFrequencySampler;

#define FLUID_STORE_OP(data) data
#define PRESSURE_STORE_OP(data) data
#define CURL_STORE_OP(data) data
#define DENSITY_STORE_OP(data) data
#define SHADOWMAP_STORE_OP(data) (data)

#define VELOCITY_BOUNDARY_MODE 1

uniform float time;
uniform float deltaTime;
uniform float timeScale;
uniform mat4 view;

struct FluidSplat { 
	float radius, velocity, pressure, density;
	mat3 rotation;
};

uniform ivec3 fluidSize;
uniform int numPressureIterations;
uniform float velocityDissipation;
uniform float densityDissipation;
uniform float vorticityScalar;
uniform FluidSplat mouseSplat;
uniform FluidSplat externalSplat;
uniform float buoyancy;

uniform vec2 screenMousePos;
uniform vec2 prevScreenMousePos;

uniform vec3 mouseSplatPos;
uniform vec3 prevMouseSplatPos;
uniform bool mouseSplatActive;
uniform bool leftMouseDown;
uniform bool rightMouseDown;

void deleteError(inout vec4 value)
{
	if (any(isinf(value)) || any(isnan(value)))
		value = vec4(0);
}

vec4 sampleFluid(sampler3D s, vec3 p, vec3 boxMin, vec3 boxMax)
{
	return texture(s, (p - boxMin) / (boxMax - boxMin));
}

vec4 sampleFluid(isampler3D s, vec3 p, vec3 boxMin, vec3 boxMax)
{
	return vec4(texture(s, (p - boxMin) / (boxMax - boxMin))) / 255.0 + 128.0;
}

vec4 sampleFluid(usampler3D s, vec3 p, vec3 boxMin, vec3 boxMax)
{
	return vec4(texture(s, (p - boxMin) / (boxMax - boxMin))) / 255.0;
}

/*
void storeFluid(image3D img, ivec3 icoords, vec4 data)
{
	imageStore(img, icoords, data);
}

void storeFluid(uimage3D img, ivec3 icoords, vec4 data)
{
	imageStore(img, icoords, uvec4(data));
}

void storeFluid(iimage3D img, ivec3 icoords, vec4 data)
{
	imageStore(img, icoords, ivec4(data));
}
*/

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

vec4 texelFetchVelocity(ivec3 icoords)
{
	float flip = 1.0;
#if VELOCITY_BOUNDARY_MODE == 1
	if ( any(lessThan(icoords, ivec3(0.0))) || any(greaterThan(icoords, fluidSize)) )
		flip = -1.0;
#endif 
	return texelFetch(fluidSampler, icoords, 0) * flip;
}

void texelFetchAdjacentVelocity(ivec3 icoords, 
	out vec4 left, 
	out vec4 right, 
	out vec4 top, 
	out vec4 bottom, 
	out vec4 front, 
	out vec4 back)
{
	left = texelFetchVelocity(icoords + ivec3(-1, 0, 0));
	right = texelFetchVelocity(icoords + ivec3(1, 0, 0));
	top = texelFetchVelocity(icoords +  ivec3(0, 1, 0));
	bottom = texelFetchVelocity(icoords +  ivec3(0, -1, 0));
	front = texelFetchVelocity(icoords +  ivec3(0, 0, 1));
	back = texelFetchVelocity(icoords +  ivec3(0, 0, -1));
}

vec4 texelFetchClamped(sampler3D s, ivec3 icoords)
{
	return texelFetch(s, clamp(icoords, ivec3(0), fluidSize), 0);
}

void texelFetchAdjacentClamped(sampler3D s, ivec3 icoords, 
	out vec4 left, 
	out vec4 right, 
	out vec4 top, 
	out vec4 bottom, 
	out vec4 front, 
	out vec4 back)
{
	left = texelFetchClamped(s, icoords + ivec3(-1, 0, 0));
	right = texelFetchClamped(s, icoords + ivec3(1, 0, 0));
	top = texelFetchClamped(s, icoords +  ivec3(0, 1, 0));
	bottom = texelFetchClamped(s, icoords +  ivec3(0, -1, 0));
	front = texelFetchClamped(s, icoords +  ivec3(0, 0, 1));
	back = texelFetchClamped(s, icoords +  ivec3(0, 0, -1));
}

