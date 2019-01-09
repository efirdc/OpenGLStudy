#ifndef SPECTRUMANALYZER_H
#define SPECTRUMANALYZER_H

#include "FrequencySpectrum.h"
#include "kissfft/kiss_fft.h"
#include "kissfft/kiss_fftr.h"

class SpectrumAnalyzer
{
public:
	SpectrumAnalyzer(int frameSize);
	~SpectrumAnalyzer();

	float * getFrameInputBuffer() { return m_fftIn; };
	void processFrame();
	FrequencySpectrum * getFrequencySpectrum() { return m_frequencySpectrum; }

	int getFrameSize() { return m_fftInSize; }
	void setFrameSize(int frameSize);

private:
	int m_fftInSize;
	int m_fftOutSize;

	float * m_fftIn;
	float * m_fftOut;
	kiss_fftr_cfg m_fftConfig;

	FrequencySpectrum * m_frequencySpectrum;
};

#endif