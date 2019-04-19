#ifndef PINGPONGBUFFER_H
#define PINGPONGBUFFER_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

// Every texture channel uses 2 attachment points on the FBO
// OpenGL only guarantees that 8 attachment points are available, so for now there is a hardcoded max of 4
#define MAX_CHANNELS 4

class PingPongBuffer
{
public:
	PingPongBuffer(int width, int height);
	~PingPongBuffer();

	void addTextureChannel(unsigned int textureUnit, const float * borderValues);
	void swapTextureChannel(int channelIndex);
	void bind();
	void bindTextures();
	void setDrawBuffers();
private:
	unsigned int FBO;
	struct TextureChannel {
		unsigned int texture[2];
		unsigned int fboAttachmentPoint[2];
		int sourceIndex;
		unsigned int textureUnit;
	};

	TextureChannel * channel[MAX_CHANNELS];
	unsigned int drawBuffers[MAX_CHANNELS];
	int numChannels;
	int width;
	int height;

	const float fluidBorder[4] = { 0.5f, 0.5f, 0.5f, 0.0f };
	const float densityBorder[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
};

#endif