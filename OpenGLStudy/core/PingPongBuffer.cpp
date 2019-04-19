#include "PingPongBuffer.h"

PingPongBuffer::PingPongBuffer(int width, int height) :
	width(width),
	height(height),
	numBuffers(0)
{
	glGenFramebuffers(1, &FBO);
}

PingPongBuffer::~PingPongBuffer()
{
	// should probably implement this
}

void PingPongBuffer::addBuffer(const float * borderValues)
{
	TextureAttachment * newBuffer = new TextureAttachment();
	buffers[numBuffers] = newBuffer;
	glGenTextures(2, newBuffer->texture);
	newBuffer->attachment[0] = GL_COLOR_ATTACHMENT0 + numBuffers * 2;
	newBuffer->attachment[1] = GL_COLOR_ATTACHMENT1 + numBuffers * 2;
	newBuffer->sourceIndex = 0;
	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, newBuffer->texture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderValues);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, newBuffer->attachment[i], GL_TEXTURE_2D, newBuffer->texture[i], 0);
		glDrawBuffer(newBuffer->attachment[i]);
		glClearBufferfv(GL_COLOR, 0, fluidBorder);
	}
	numBuffers += 1;
}

void PingPongBuffer::bind()
{
	bindTextures();
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, width, height);
	setDrawBuffers();
}

void PingPongBuffer::bindTextures(int firstTextureUnit)
{
	for (int i = 0; i < numBuffers; i++) {
		glActiveTexture(firstTextureUnit + i);
		TextureAttachment * buffer = buffers[i];
		glBindTexture(GL_TEXTURE_2D, buffer->texture[buffer->sourceIndex]);
	}
}

void PingPongBuffer::setDrawBuffers() 
{
	for (int i = 0; i < numBuffers; i++) {
		TextureAttachment * buffer = buffers[i];
		drawBuffers[i] = buffer->attachment[1 - buffer->sourceIndex];
	}
	glDrawBuffers(numBuffers, drawBuffers);
}

void PingPongBuffer::swapBuffer(int bufferIndex)
{
	TextureAttachment * buffer = buffers[bufferIndex];
	buffer->sourceIndex = 1 - buffer->sourceIndex;
}
