#ifndef UTILITIES_H
#define UTILITIES_H

#include "glm/glm.hpp"
#include "kissfft/kiss_fft.h"
#include "kissfft/kiss_fftr.h"

namespace utl
{
	template <class T>
	inline T min(T x, T y);

	template <class T>
	inline T max(T x, T y);

	template <class T>
	inline T clamp(T x, T min, T max);

	template <class T>
	inline T mix(T vi, T vf, float t);

	template <class T>
	T getValue(T * data, int size, float t);

	template <class T>
	T getValueLerp(T * data, int size, float t);
	/*
	* Gets a value from a data array using a float t on the [0.0f, 1.0f] domain.
	* Linearly interpolates between the values in data
	* Pre:
	*	data is a pointer an array of values that support basic arithmetic operations
	*	size is the size of data
	*	t must be in the [0.0f, 1.0f] domain.
	* Post:
	*	returns value in data at position t
	*/

	template <class T>
	void expDomainShift(T * inBuffer, int inSize, T * outBuffer, int outSize, float exponent);
	/*
	* Performs an exponential domain shift on a set of data
	* Pre:
	*	inBuffer is the data to be domain shifted with size inSize
	*	outBuffer has size outSize
	*	exponent controls the domain shift. Corresponds to the variable 's': https://www.desmos.com/calculator/s9qukdx6zd
	* Post:
	*	outBuffer contains the domain shifted data
	*/

	glm::vec2 bezierValue(glm::vec2 controlPoints[2], float t);
	/*
	* Evaluate a bezier defined by 2 control points at position t
	* Normally a bezier is defined by 4 points, but for this function the first and last points are assumed to be (0,0) and (1,1)
	* See http://cubic-bezier.com to see how the control points change the function shape
	* Pre:
	*	controlPoints contains the positions of the bezier handles
	*	t is a position on [0.0, 1.0] domain
	* Post:
	*	returns a 2d point on the curve at position t
	*/

	void bezierTable(glm::vec2 controlPoints[2], glm::vec2 * outBuffer, int outSize);
	/*
	* Evaluates a table of 2d bezier points using linearly spaced values for t
	* Pre:
	*	controlPoints contains the positions of the bezier handles
	*	outBuffer has size outSize
	* Post:
	*	outBuffer is populated with values on the bezier curve
	*/

	glm::vec2 bezierValue(glm::vec2 controlPoint1, glm::vec2 controlPoint2, float t);

	void bezierTable(glm::vec2& controlPoint1, glm::vec2& controlPoint2, glm::vec2* outBuffer, int outSize);

	void curve2Dto1D(glm::vec2 * inBuffer, int inSize, float * outBuffer, int outSize);
	/*
	* Converts a curve defined by 2d points to a 1d array of floats
	* Useful to convert a bezierTable into a 1d data set that can be used with getValue() or as a 1d openGL texture
	* Pre:
	*	x values of points in inBuffer must always increase, else function will abort
	*	inBuffer has size inSize
	*	outBuffer has size outSize
	* Post:
	*	outBuffer contains results of conversion
	*/

	void fft(float * inBuffer, float * outBuffer, unsigned int size);
	/*
	* Performs a fast fourier transform on a signal
	* Pre:
	*	inBuffer is a real value time domain signal
	*	size is the size of inBuffer
	*	outBuffer is has a size of (size / 2) + 1
	* Post:
	*	outBuffer contains magnitude of the frequency domain of the signal
	*/

	void linearPeaks(float * data, int dataSize, float blurRadius);
	/*
	*/

	void curvePeaks(float * data, int dataSize, float * curve, int curveSize);
	/*
	*/

	void kernelBlur(float * data, int dataSize, float * kernel, int kernelSize);
	/*
	*/

	// TEMPLATE DEFINTIONS

	template <class T>
	inline T min(T x, T y)
	{
		return x < y ? x : y;
	}

	template <class T>
	inline T max(T x, T y)
	{
		return x > y ? x : y;
	}

	template <class T>
	inline T clamp(T x, T minVal, T maxVal)
	{
		return max(min(x, maxVal), minVal);
	}

	template <class T>
	inline T mix(T vi, T vf, float t)
	{
		return (1.0f - t) * vi + t * vf;
	}

	template <class T>
	T getValue(T * data, int size, float t)
	{
		return data[(int)((size - 1) * t)];
	}

	template <class T>
	T getValueLerp(T * data, int size, float t)
	{
		float width = (float)(size - 1);
		int j = (int)(width * t);
		float pct = width * t - (float)j;

		return (1.0f - pct) * data[j] + pct * data[j + 1];
	}

	template <class T>
	void expDomainShift(T * inBuffer, int inSize, T * outBuffer, int outSize, float exponent)
	{
		for (int i = 0; i < outSize; i++)
		{
			float t = (float)i / (float)(outSize - 1);
			t = 1.0f - pow(1.0f - t, exponent);
			outBuffer[i] = getValueLerp(inBuffer, inSize, t);
		}
	}
}

#endif