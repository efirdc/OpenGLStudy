#include "FluidBuffer.h"

FluidBuffer::FluidBuffer(int width, int height) :
	width(width),
	height(height),
	fluidTextureIndex(0),
	densityTextureIndex(0)
{
	glGenTextures(2, fluidTexture);
	glGenTextures(2, densityTexture);
	glGenFramebuffers(1, &FBO);
	drawBuffers[0] = GL_COLOR_ATTACHMENT1;
	drawBuffers[1] = GL_COLOR_ATTACHMENT3;

	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, fluidTexture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, fluidBorder);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, fluidTexture[i], 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
		glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, densityTexture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, densityBorder);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2 + i, GL_TEXTURE_2D, densityTexture[i], 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT2 + i);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

FluidBuffer::~FluidBuffer()
{

}

void FluidBuffer::bind()
{
	bindTextures();
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, width, height);
	glDrawBuffers(2, drawBuffers);
}

void FluidBuffer::bindTextures()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fluidTexture[fluidTextureIndex]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, densityTexture[densityTextureIndex]);
}

void FluidBuffer::swapFluidBuffers()
{
	fluidTextureIndex = 1 - fluidTextureIndex;
	drawBuffers[0] = GL_COLOR_ATTACHMENT0 + 1 - fluidTextureIndex;
}

void FluidBuffer::swapDensityBuffers()
{
	densityTextureIndex = 1 - densityTextureIndex;
	drawBuffers[1] = GL_COLOR_ATTACHMENT2 + 1 - densityTextureIndex;
}
