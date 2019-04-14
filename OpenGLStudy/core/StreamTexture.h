#ifndef STREAMTEXTURE_H
#define STREAMTEXTURE_H

/*
* A class that allows efficient streaming of pixel data to openGL textures every frame
* This uses the "streaming texture uploads" method described here: http://www.songho.ca/opengl/gl_pbo.html
*/

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class StreamTexture1D
{
public:
	unsigned int textureID;
	unsigned int pbo1;
	unsigned int pbo2;
	unsigned int internalFormat;
	unsigned int width;
	unsigned int format;
	unsigned int type;
	unsigned int numChannels;
	unsigned int bytesPerChannel;
	unsigned int dataSize;

	StreamTexture1D(
		unsigned int internalFormat,
		unsigned int width,
		unsigned int format,
		unsigned int type,
		unsigned int numChannels,
		unsigned int bytesPerChannel, 
		bool doubleBufferMode);
	/*
	* Constructor
	* Pre:
	*	internalFormat, width, format, and type are used for glTexImage1D calls
	*	see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage1D.xhtml for documentation
	*	numChannels must be the number of channels in the pixel data. For example, GL_RED has 1 channel, GL_RGB has 3 channels, etc
	*	bytesPerChannel must be the number of bytes in the pixel type used. For example, GL_SHORT would be 2 bytes, GL_FLOAT would be 4 bytes.
	*	set doubleBufferMode to true to use two pixel buffers instead of one. 
	*	This will result in slightly better performace, but causes a frame of delay.
	* Post:
	*	A new StreamTexture is created. 
	*	A texture, and two pixel buffer objects are generated with openGL.
	*	The byte size of the pixel buffer is saved in dataSize.
	*/

	~StreamTexture1D();

	void resize(int newWidth);
	/*
	* Pre:
	*	newWidth is the new desired width of the stream texture.
	*	The pixel buffer is currently unmapped.
	* Post:
	*	width and datasize fields are updated.
	*   The texture and pixel buffers are resized.
	*/

	char * getPixelBuffer();
	/*
	* Maps the pixel buffer into client memory. Buffer will have a size of dataSize.
	* Pre:
	*	unmapPixelBuffer() must be called once after the last getPixelBuffer() call
	* Post:
	*	returns a pointer to a new pixel buffer.
	*/

	void unmapPixelBuffer();
	/*
	* Release the pointer to the pixel buffer.
	* Pre:
	*	Call this once after getPixelBuffer() and filling the buffer with data.
	* Post:
	*	unmaps the pixel buffer
	*/

	int bufferLength();
	/*
	* Get the number of elements in the buffer (dataSize / bytesPerChannel)
	* Pre:
	*	None
	* Post:
	*	returns datasize / bytesPerChannel
	*/
};

#endif