#include "PingPongBuffer.h"

PingPongBuffer::PingPongBuffer(int width, int height) :
	width(width),
	height(height),
	numChannels(0)
{
	glGenFramebuffers(1, &FBO);
}

PingPongBuffer::~PingPongBuffer()
{
	// should probably implement this
}

void PingPongBuffer::addTextureChannel(unsigned int textureUnit, const float * borderValues)
{
	TextureChannel * newChannel = new TextureChannel();
	channel[numChannels] = newChannel;
	glGenTextures(2, newChannel->texture);
	newChannel->fboAttachmentPoint[0] = GL_COLOR_ATTACHMENT0 + numChannels * 2;
	newChannel->fboAttachmentPoint[1] = GL_COLOR_ATTACHMENT1 + numChannels * 2;
	newChannel->sourceIndex = 0;
	newChannel->textureUnit = textureUnit;
	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, newChannel->texture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderValues);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, newChannel->fboAttachmentPoint[i], GL_TEXTURE_2D, newChannel->texture[i], 0);
		glDrawBuffer(newChannel->fboAttachmentPoint[i]);
		glClearBufferfv(GL_COLOR, 0, borderValues);
	}
	numChannels += 1;
}

void PingPongBuffer::bind()
{
	bindTextures();
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, width, height);
	setDrawBuffers();
}

void PingPongBuffer::bindTextures()
{
	for (int i = 0; i < numChannels; i++) {
		glActiveTexture(channel[i]->textureUnit);
		glBindTexture(GL_TEXTURE_2D, channel[i]->texture[channel[i]->sourceIndex]);
	}
}

void PingPongBuffer::setDrawBuffers() 
{
	for (int i = 0; i < numChannels; i++) {
		int destinationIndex = 1 - channel[i]->sourceIndex;
		drawBuffers[i] = channel[i]->fboAttachmentPoint[destinationIndex];
	}
	glDrawBuffers(numChannels, drawBuffers);
}

void PingPongBuffer::swapTextureChannel(int bufferIndex)
{
	channel[bufferIndex]->sourceIndex = 1 - channel[bufferIndex]->sourceIndex;
}
