#ifndef SPECTRUMANALYZER_H
#define SPECTRUMANALYZER_H

#include "FrequencySpectrum.h"
#include "kissfft/kiss_fft.h"
#include "kissfft/kiss_fftr.h"
#include "utilities.h"

class SpectrumAnalyzer
{
public:
	SpectrumAnalyzer(int frameSize, int numSpectrums, int numFreqBins, float domainShiftFactor);
	~SpectrumAnalyzer();

	float * getFrameInputBuffer() { return m_fftIn; };
	void processFrame();
	const FrequencySpectrum * getFrequencySpectrum() { return m_finalSpectrum; }

	int getFrameSize() { return m_fftInSize; }
	int getNumSpectrums() { return m_numSpectrums; }
	int getNumFreqBins() { return m_numFreqBins; }
	float getDomainShiftFactor() { return m_domainShiftFactor; }
	int getAmplitudeCurveSize() { return m_amplitudeCurveSize; }
	float * getAmplitudeCurve() { return m_amplitudeCurve; }
	int getPeakCurveSize() { return m_peakCurveSize; }
	float * getPeakCurve() { return m_peakCurve; }

	void setFrameSize(int frameSize);
	void setNumSpectrums(int numSpectrums);
	void setNumFreqBins(int numFreqBins);
	void setDomainShiftFactor(float domainShiftFactor);
	void setAmplitudeCurve(float * amplitudeCurve, int amplitudeCurveSize);
	void setPeakCurve(float * peakCurve, int peakCurveSize);

private:
	int m_fftInSize;
	int m_fftOutSize;

	float * m_fftIn;
	float * m_fftOutComplex;
	float * m_fftOutMagnitude;
	kiss_fftr_cfg m_fftConfig;

	int m_numSpectrums;
	int m_spectrumID;
	FrequencySpectrum ** m_spectrums;
	FrequencySpectrum * m_finalSpectrum;
	float * m_tempSpectrumBuffer;

	int m_numFreqBins;
	float m_domainShiftFactor;

	int m_amplitudeCurveSize;
	float * m_amplitudeCurve;

	int m_peakCurveSize;
	float * m_peakCurve;

	void applyPeakCurve(float * data);
};

#endif