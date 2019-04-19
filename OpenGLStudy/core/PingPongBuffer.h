#ifndef PINGPONGBUFFER_H
#define PINGPONGBUFFER_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define MAX_BUFFERS 4

class PingPongBuffer
{
public:
	unsigned int FBO;
	
	PingPongBuffer(int width, int height);
	~PingPongBuffer();

	void addBuffer(const float * borderValues);
	void bind();
	void bindTextures(int firstTextureUnit = GL_TEXTURE0);
	void setDrawBuffers();
	void swapBuffer(int bufferIndex);
private:
	struct TextureAttachment {
		unsigned int texture[2];
		unsigned int attachment[2];
		int sourceIndex;
		unsigned int textureUnit;
	};

	TextureAttachment * buffers[MAX_BUFFERS];
	unsigned int drawBuffers[MAX_BUFFERS];
	int numBuffers;
	int width;
	int height;

	const float fluidBorder[4] = { 0.5f, 0.5f, 0.5f, 0.0f };
	const float densityBorder[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
};

#endif