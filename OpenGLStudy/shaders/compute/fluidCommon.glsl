//?#version 440

#define LOCAL_SIZE_X 8
#define LOCAL_SIZE_Y 8
#define LOCAL_SIZE_Z 8

layout(binding = 0, rgba32f) writeonly uniform image3D fluidImage;
layout(binding = 1, rgba32f) writeonly uniform image3D vorticityImage;
layout(binding = 2, rgba32f) writeonly uniform image3D densityImage;

layout(binding = 0) uniform sampler3D velocitySampler;
layout(binding = 2) uniform sampler3D curlSampler;
layout(binding = 3) uniform sampler3D densitySampler;

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