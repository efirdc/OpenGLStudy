#ifndef FLUIDBUFFER_H
#define FLUIDBUFFER_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class FluidBuffer
{
public:
	unsigned int FBO;
	unsigned int fluidTexture[2];
	unsigned int densityTexture[2];
	int fluidTextureIndex;
	int densityTextureIndex;
	int width;
	int height;

	FluidBuffer(int width, int height);
	~FluidBuffer();

	void bind();
	void bindTextures();
	void swapFluidBuffers();
	void swapDensityBuffers();
private:
	const float fluidBorder[4] = { 0.5f, 0.5f, 0.5f, 0.0f };
	const float densityBorder[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	unsigned int drawBuffers[2];
};

#endif