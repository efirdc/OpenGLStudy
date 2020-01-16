#include "utilities.h"

namespace utl
{
	glm::vec2 bezierValue(glm::vec2 controlPoints[2], float t)
	{
		float omt = 1.0f - t;
		return controlPoints[0] * (3.0f * omt * omt * t) + controlPoints[1] * (3 * omt * t * t) + glm::vec2(1.0f, 1.0f) * (t * t * t);
	}

	void bezierTable(glm::vec2 controlPoints[2], glm::vec2 * outBuffer, int outSize)
	{
		float width = (float)(outSize - 1);
		for (int i = 0; i < outSize; i++)
			outBuffer[i] = bezierValue(controlPoints, (float)i / width);
	}

	glm::vec2 bezierValue(glm::vec2 controlPoint1, glm::vec2 controlPoint2, float t)
	{
		float omt = 1.0f - t;
		return controlPoint1 * (3.0f * omt * omt * t) + controlPoint2 * (3 * omt * t * t) + glm::vec2(1.0f, 1.0f) * (t * t * t);
	}

	void bezierTable(glm::vec2& controlPoint1, glm::vec2& controlPoint2, glm::vec2* outBuffer, int outSize)
	{
		float width = (float)(outSize - 1);
		for (int i = 0; i < outSize; i++)
			outBuffer[i] = bezierValue(controlPoint1, controlPoint2, (float)i / width);
	}

	void curve2Dto1D(glm::vec2 * inBuffer, int inSize, float * outBuffer, int outSize)
	{
		int j = 0;
		for (int i = 0; i < outSize; i++)
		{
			// t from 0.0f to 1.0f at linear steps
			float t = (float)(i) / (float)outSize;

			// increment j until (points[j].x <= t <= points[j + 1].x)
			while (t > inBuffer[j + 1].x)
			{
				j++;
				// Failsafe if programmer provides inBuffer where x values are not always increasing
				if ((j + 1) >= inSize)
					return;
			}
			
			// get the two points
			glm::vec2 p1 = inBuffer[j];
			glm::vec2 p2 = inBuffer[j + 1];

			// find how far t is between p1.x and p2.x as a "percentage" on [0.0, 1.0]
			float pct = (t - p1.x) / (p2.x - p1.x);

			// lerp p1.y to p2.y by pct
			outBuffer[i] = mix(p1.y, p2.y, pct);
		}
	}

	void fft(float * inBuffer, float * outBuffer, unsigned int size)
	{
		// Declarations and initialize cfg
		static bool fftInit = false;
		static int lastSize = 0;
		static kiss_fft_cpx * fout;
		static kiss_fftr_cfg cfg;

		// outSize for a real only fft
		int outSize = size / 2 + 1;

		// initialize an outbuffer and the kissft config object the first time this function is called
		if (!fftInit)
		{
			fftInit = true;
			lastSize = size;
			fout = new kiss_fft_cpx[outSize];
			cfg = kiss_fftr_alloc(size, 0, NULL, NULL);
		}

		// reinitialize buffer and cfg if the size of the fft ever changes
		if (size != lastSize)
		{
			lastSize = size;
			delete[] fout;
			free(cfg);
			fout = new kiss_fft_cpx[outSize];
			cfg = kiss_fftr_alloc(size, 0, NULL, NULL);
		}

		kiss_fft_scalar * fin = (kiss_fft_scalar *)inBuffer;

		// Do the real only fft
		kiss_fftr(cfg, fin, fout);

		// copy magnitude of output into the outBuffer
		for (int i = 0; i < outSize; i++)
		{
			float rl = fout[i].r;
			float im = fout[i].i;
			outBuffer[i] = sqrt(rl * rl + im * im) / (float)outSize;
		}
	}

	void linearPeaks(float * data, int dataSize, float peakRadius)
	{
		int intRadius = (int)((float)dataSize * peakRadius);
		float grav = 1.0f / (float)intRadius;
		float pos = 0.0f;
		for (int i = 0; i < dataSize; i++)
		{

			pos = max(pos - grav, data[i]);
			data[i] = pos;
		}
		pos = 0.0f;
		for (int i = 0; i < dataSize; i++)
		{

			pos = max(pos - grav, data[dataSize - i]);
			data[dataSize - i] = pos;
		}
	}

	void curvePeaks(float * data, int dataSize, float * curve, int curveSize)
	{
		// forward
		float peak = 0.0f;
		int curvePos = 0;
		for (int i = 0; i < dataSize; i++)
		{
			if (data[i] >= peak || curvePos >= curveSize)
			{
				peak = data[i];
				curvePos = 0;
			}
			else
			{
				float fall = peak * clamp(curve[curveSize - 1 - curvePos], 0.0f, 1.0f);
				if (data[i] >= fall)
				{
					peak = data[i];
					curvePos = 0;
				}
				else
				{
					data[i] = fall;
					curvePos += 1;
				}
			}
		}

		// back
		peak = 0.0f;
		curvePos = 0;
		for (int i = 0; i < dataSize; i++)
		{
			int i_rev = (dataSize - 1) - i;
			if (data[i_rev] >= peak || curvePos >= curveSize)
			{
				peak = data[i_rev];
				curvePos = 0;
			}
			else
			{
				float fall = peak * clamp(curve[curveSize - 1 - curvePos], 0.0f, 1.0f);
				if (data[i_rev] >= fall)
				{
					peak = data[i_rev];
					curvePos = 0;
				}
				else
				{
					data[i_rev] = fall;
					curvePos += 1;
				}
			}
		}
	}

	void kernelBlur(float * data, int dataSize, float * kernel, int kernelSize)
	{
		float * temp = new float[dataSize];
		for (int i = 0; i < dataSize; i++)
		{
			float accum = 0.0f;
			for (int j = 0; j < kernelSize; j++)
			{
				float k = 1.0f / (float)kernelSize;// kernel[kernelSize - 1 - j] / (float)kernelSize;
				if (i - j >= 0)
					accum += data[i - j] * k;
				if (i + j <= dataSize - 1)
					accum += data[i + j] * k;
			}
			temp[i] = accum;
		}
		for (int i = 0; i < dataSize; i++)
			data[i] = temp[i];
		delete[] temp;
	}
}