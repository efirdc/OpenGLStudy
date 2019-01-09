#ifndef SPECTRUMFILTER_H
#define SPECTRUMFILTER_H

#include "FrequencySpectrum.h"
#include "utilities.h"

class SpectrumFilter
{
public:
	virtual const FrequencySpectrum * applyFilter(const FrequencySpectrum * inputSpectrum) = 0;
	const FrequencySpectrum * getFrequencySpectrum();
	virtual ~SpectrumFilter();
protected:
	SpectrumFilter();
	FrequencySpectrum * m_outputSpectrum;
};

class AmplitudeFilter : public SpectrumFilter
{
public:
	AmplitudeFilter(int amplitudeCurveSize);
	AmplitudeFilter(float * amplitudeCurve, int amplitudeCurveSize);
	~AmplitudeFilter();

	virtual const FrequencySpectrum * applyFilter(const FrequencySpectrum * inputSpectrum);

	void setAmplitudeCurveSize(int amplitudeCurveSize);
	void setAmplitudeCurve(float * amplitudeCurve, int amplitudeCurveSize);
	int getAmplitudeCurveSize();
	float * getAmplitudeCurve();

private:
	float * m_amplitudeCurve;
	int m_amplitudeCurveSize;
};

class DomainShiftFilter : public SpectrumFilter
{
public:
	DomainShiftFilter(float domainShiftFactor, int numFrequencyBins);
	~DomainShiftFilter();

	const FrequencySpectrum * applyFilter(const FrequencySpectrum * inputSpectrum);

	void setDomainShiftFactor(float domainShiftFactor);
	void setNumFrequencyBins(int numFrequencyBins);
	float getDomainShiftFactor();
	

private:
	float m_domainShiftFactor;
};

class PeakFilter : public SpectrumFilter
{
public:
	PeakFilter(int peakCurveSize);
	PeakFilter(float * peakCurve, int peakCurveSize);
	~PeakFilter();

	const FrequencySpectrum * applyFilter(const FrequencySpectrum * inputSpectrum);

	void setPeakCurveSize(int peakCurveSize);
	void setPeakCurve(float * peakCurve, int peakCurveSize);
	int getPeakCurveSize();
	float * getPeakCurve();

private:
	float * m_peakCurve;
	int m_peakCurveSize;
};

class AverageFilter : public SpectrumFilter
{
public:
	AverageFilter(int numSpectrumsInAverage);
	~AverageFilter();

	const FrequencySpectrum * applyFilter(const FrequencySpectrum * inputSpectrum);

	void setNumSpectrumsInAverage(int numSpectrumsInAverage);
	int getNumSpectrumsInAverage();

private:
	int m_numSpectrums;
	FrequencySpectrum ** m_spectrums;
	int m_spectrumID;
};

#endif