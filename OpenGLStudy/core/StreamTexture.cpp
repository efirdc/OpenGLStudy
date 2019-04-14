#include "StreamTexture.h"

StreamTexture1D::StreamTexture1D(
	unsigned int internalFormat,
	unsigned int width,
	unsigned int format,
	unsigned int type,
	unsigned int numChannels,
	unsigned int bytesPerChannel,
	bool doubleBufferMode) :
	internalFormat(internalFormat),
	width(width),
	format(format),
	type(type),
	numChannels(numChannels),
	bytesPerChannel(bytesPerChannel)
{
	// Calculate the size of the pixel buffer
	dataSize = width * numChannels * bytesPerChannel;
	
	// Generate and initialize texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_1D, textureID);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	glTexImage1D(GL_TEXTURE_1D, 0, internalFormat, width, 0, format, type, 0);

	// Generate and initialize a pixel buffer object
	glGenBuffersARB(1, &pbo1);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo1);
	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, dataSize, 0, GL_STREAM_DRAW_ARB);
	
	// Generate another PBO if using double buffer mode
	if (doubleBufferMode)
	{
		glGenBuffersARB(1, &pbo2);
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo2);
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, dataSize, 0, GL_STREAM_DRAW_ARB);
	}

	// Otherwise just use a single PBO
	else
		pbo2 = pbo1;

	// Unbind the PBO and texture
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	glBindTexture(GL_TEXTURE_1D, 0);
}

StreamTexture1D::~StreamTexture1D()
{
	glDeleteBuffersARB(1, &pbo1);
	glDeleteBuffersARB(1, &pbo2);
	glDeleteTextures(1, &textureID);
}

void StreamTexture1D::resize(int newWidth)
{
	// Calculate the size of the pixel buffer
	width = newWidth;
	dataSize = width * numChannels * bytesPerChannel;

	// Bind and resize the texture
	glBindTexture(GL_TEXTURE_1D, textureID);
	glTexImage1D(GL_TEXTURE_1D, 0, internalFormat, width, 0, format, type, 0);

	// Bind and resize the pbos
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo1);
	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, dataSize, 0, GL_STREAM_DRAW_ARB);
	if (pbo1 != pbo2)
	{
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo2);
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, dataSize, 0, GL_STREAM_DRAW_ARB);
	}

	// Unbind the PBO and texture
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	glBindTexture(GL_TEXTURE_1D, 0);
}

char * StreamTexture1D::getPixelBuffer()
{
	// Every frame data is transfered from pbo1 to the texture object,
	// while new pixel data is tranfered from the char * data pointer to pbo2

	// Swap the pbos
	// This does nothing if dualBufferMode is false as pbo1 and pbo2 will be the same
	unsigned int temp = pbo1;
	pbo1 = pbo2;
	pbo2 = temp;

	// Bind the texture and pbo1
	glBindTexture(GL_TEXTURE_1D, textureID);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo1);

	// Since there is a texture and a buffer object bound, 
	// the "data" param of glTexSubImage is an offset into the pbo, instead of a pointer to CPU memory
	// 0 is used as an offset so that the entire buffer is copied into the texture
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, width, format, type, 0);

	// Bind pbo2
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo2);

	// It is important to call glBufferDataARB() with a 0 pointer before glMapBufferARB()
	// If we just call glMapBufferARB() the CPU will stall and wait if the GPU is still working with the buffer
	// If we call glBufferDataARB() first with a 0 pointer then this will allocate new memory for the buffer that we can use immediately
	// Then glMapBufferARB() does not stall, and the old buffer data gets discarded once the GPU is done with it.
	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, dataSize, 0, GL_STREAM_DRAW_ARB);
	char * pbo2Data = (char *)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);

	// Unbind the pbo
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	
	return pbo2Data;
}

void StreamTexture1D::unmapPixelBuffer()
{
	// Bind pbo2
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo2);

	// Release pointer to the mapping buffer
	glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB);

	// Unbind the PBO
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
}

int StreamTexture1D::bufferLength()
{
	return dataSize / bytesPerChannel;
}