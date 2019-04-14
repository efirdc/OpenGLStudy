#include "SpectrumFilter.h"

SpectrumFilter::SpectrumFilter() :
	m_outputSpectrum(new FrequencySpectrum(0))
{
}

const FrequencySpectrum * SpectrumFilter::getFrequencySpectrum()
{
	return m_outputSpectrum;
}

SpectrumFilter::~SpectrumFilter()
{
	delete m_outputSpectrum;
}

AmplitudeFilter::AmplitudeFilter(int amplitudeCurveSize) :
	SpectrumFilter(),
	m_amplitudeCurve(new float[amplitudeCurveSize]),
	m_amplitudeCurveSize(amplitudeCurveSize)
{
}

AmplitudeFilter::AmplitudeFilter(float * amplitudeCurve, int amplitudeCurveSize) :
	SpectrumFilter(),
	m_amplitudeCurve(new float[amplitudeCurveSize]()),
	m_amplitudeCurveSize(amplitudeCurveSize)
{
	memcpy(m_amplitudeCurve, amplitudeCurve, amplitudeCurveSize * sizeof(float));
}

AmplitudeFilter::~AmplitudeFilter()
{
	delete[] m_amplitudeCurve;
	//delete m_outputSpectrum;
}

const FrequencySpectrum * AmplitudeFilter::applyFilter(const FrequencySpectrum * inputSpectrum)
{
	if (m_outputSpectrum->size != inputSpectrum->size)
		m_outputSpectrum->resize(inputSpectrum->size);

	float * inputData = inputSpectrum->data;
	float * outputData = m_outputSpectrum->data;
	for (int i = 0; i < inputSpectrum->size; i++)
		outputData[i] = utl::getValue(m_amplitudeCurve, m_amplitudeCurveSize, utl::min(inputData[i], 1.0f));

	return m_outputSpectrum;
}

void AmplitudeFilter::setAmplitudeCurveSize(int amplitudeCurveSize)
{
	m_amplitudeCurveSize = amplitudeCurveSize;
	delete[] m_amplitudeCurve;
	m_amplitudeCurve = new float[m_amplitudeCurveSize]();
}

void AmplitudeFilter::setAmplitudeCurve(float * amplitudeCurve, int amplitudeCurveSize)
{
	m_amplitudeCurveSize = amplitudeCurveSize;
	m_amplitudeCurve = new float[m_amplitudeCurveSize]();
	memcpy(m_amplitudeCurve, amplitudeCurve, m_amplitudeCurveSize * sizeof(float));
	
}

int AmplitudeFilter::getAmplitudeCurveSize()
{
	return m_amplitudeCurveSize;
}

float * AmplitudeFilter::getAmplitudeCurve()
{
	return m_amplitudeCurve;
}

DomainShiftFilter::DomainShiftFilter(float domainShiftFactor, int numFrequencyBins) :
	SpectrumFilter(),
	m_domainShiftFactor(domainShiftFactor)
{
	m_outputSpectrum->resize(numFrequencyBins);
}

DomainShiftFilter::~DomainShiftFilter()
{
	//delete m_outputSpectrum;
}

const FrequencySpectrum * DomainShiftFilter::applyFilter(const FrequencySpectrum * inputSpectrum)
{
	// Resize and domain shift the frequency data
	float * inputData = inputSpectrum->data;
	float * outputData = m_outputSpectrum->data;
	int numFrequencyBins = m_outputSpectrum->size;
	for (int i = 0; i < numFrequencyBins; i++)
	{
		float t = (float)i / (float)(numFrequencyBins - 1);
		t = 1.0f - pow(1.0f - t, 1.0f / m_domainShiftFactor);
		outputData[i] = utl::getValueLerp(inputData, inputSpectrum->size, t);
	}
	return m_outputSpectrum;
}

void DomainShiftFilter::setDomainShiftFactor(float domainShiftFactor)
{
	m_domainShiftFactor = domainShiftFactor;
}

void DomainShiftFilter::setNumFrequencyBins(int numFrequencyBins)
{
	delete m_outputSpectrum;
	m_outputSpectrum = new FrequencySpectrum(numFrequencyBins);
}

float DomainShiftFilter::getDomainShiftFactor()
{
	return m_domainShiftFactor;
}

PeakFilter::PeakFilter(int peakCurveSize) :
	SpectrumFilter(),
	m_peakCurve(new float[peakCurveSize]()),
	m_peakCurveSize(peakCurveSize)
{
}

PeakFilter::PeakFilter(float * peakCurve, int peakCurveSize) :
	SpectrumFilter(),
	m_peakCurve(new float[peakCurveSize]()),
	m_peakCurveSize(peakCurveSize)
{
	memcpy(m_peakCurve, peakCurve, peakCurveSize * sizeof(float));
}

PeakFilter::~PeakFilter()
{
	delete[] m_peakCurve;
	//delete m_outputSpectrum;
}

const FrequencySpectrum * PeakFilter::applyFilter(const FrequencySpectrum * inputSpectrum)
{
	if (m_outputSpectrum->size != inputSpectrum->size)
		m_outputSpectrum->resize(inputSpectrum->size);

	float * inputData = inputSpectrum->data;
	float * outputData = m_outputSpectrum->data;
	int numFrequencyBins = m_outputSpectrum->size;

	// "Walk" to the right over the data
	// fall from the peaks in the shape of the peakcurve and stop falling after hitting the ground
	// store position in outputData
	float peak = 0.0f;
	float position = 0.0f;
	int curvePos = 0;
	for (int i = 0; i < numFrequencyBins; i++)
	{
		if (inputData[i] >= position || curvePos >= m_peakCurveSize)
		{
			peak = inputData[i];
			curvePos = 0;
		}
		else
			curvePos += 1;
		position = peak * utl::clamp(m_peakCurve[m_peakCurveSize - 1 - curvePos], 0.0f, 1.0f);
		outputData[i] = utl::max(inputData[i], position);
	}

	// Same thing, but walking to the left this time
	// Overwrite data with the max(position, tempSpectrumBuffer)
	peak = 0.0f;
	position = 0.0f;
	curvePos = 0;
	for (int i = 0; i < m_outputSpectrum->size; i++)
	{
		int i_rev = (numFrequencyBins - 1) - i;
		if (outputData[i_rev] >= position || curvePos >= m_peakCurveSize)
		{
			peak = inputData[i_rev];
			curvePos = 0;
		}
		else
			curvePos += 1;
		position = peak * utl::clamp(m_peakCurve[m_peakCurveSize - 1 - curvePos], 0.0f, 1.0f);
		outputData[i_rev] = utl::max(position, outputData[i_rev]);
	}
	return m_outputSpectrum;
}

void PeakFilter::setPeakCurveSize(int peakCurveSize)
{
	m_peakCurveSize = peakCurveSize;
	delete[] m_peakCurve;
	m_peakCurve = new float[m_peakCurveSize]();
}

void PeakFilter::setPeakCurve(float * peakCurve, int peakCurveSize)
{
	m_peakCurveSize = peakCurveSize;
	delete[] m_peakCurve;
	m_peakCurve = new float[m_peakCurveSize]();
	memcpy(m_peakCurve, peakCurve, m_peakCurveSize * sizeof(float));
}

int PeakFilter::getPeakCurveSize()
{
	return m_peakCurveSize;
}

float * PeakFilter::getPeakCurve()
{
	return m_peakCurve;
}

AverageFilter::AverageFilter(int numSpectrumsInAverage) :
	SpectrumFilter(),
	m_numSpectrums(numSpectrumsInAverage),
	m_spectrumID(0),
	m_spectrums(new FrequencySpectrum *[m_numSpectrums])
{
	for (int i = 0; i < m_numSpectrums; i++)
		m_spectrums[i] = new FrequencySpectrum(0);
}

AverageFilter::~AverageFilter()
{
	for (int i = 0; i < m_numSpectrums; i++)
		delete m_spectrums[i];
	delete[] m_spectrums;
}

const FrequencySpectrum * AverageFilter::applyFilter(const FrequencySpectrum * inputSpectrum)
{
	int numFreqBins = inputSpectrum->size;
	if (m_outputSpectrum->size != numFreqBins)
	{
		m_outputSpectrum->resize(numFreqBins);
		for (int i = 0; i < m_numSpectrums; i++)
			m_spectrums[i]->resize(numFreqBins);
	}
	
	// Store the input spectrum data in the spectrums array
	float * newData = m_spectrums[m_spectrumID]->data;
	m_spectrumID = (m_spectrumID + 1) % m_numSpectrums;
	memcpy(newData, inputSpectrum->data, numFreqBins * sizeof(float));

	// Reset output data
	float * outputData = m_outputSpectrum->data;
	for (int i = 0; i < numFreqBins; i++)
		outputData[i] = 0.0f;

	// Sum all the frequency spectrums
	for (int i = 0; i < m_numSpectrums; i++)
	{
		float * data = m_spectrums[i]->data;
		for (int j = 0; j < numFreqBins; j++)
			outputData[j] += data[j];
	}

	// Divide by number of spectrums to get the average
	for (int i = 0; i < numFreqBins; i++)
		outputData[i] /= (float)m_numSpectrums;

	return m_outputSpectrum;
}

void AverageFilter::setNumSpectrumsInAverage(int numSpectrumsInAverage)
{
	// Delete old spectrums
	for (int i = 0; i < m_numSpectrums; i++)
		delete m_spectrums[i];
	delete[] m_spectrums;

	// Create new spectrums
	m_numSpectrums = numSpectrumsInAverage;
	m_spectrums = new FrequencySpectrum *[m_numSpectrums];
	for (int i = 0; i < m_numSpectrums; i++)
		m_spectrums[i] = new FrequencySpectrum(m_outputSpectrum->size);
	m_spectrumID = 0;
}

int AverageFilter::getNumSpectrumsInAverage()
{
	return m_numSpectrums;
}