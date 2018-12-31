#include "SpectrumAnalyzer.h"

SpectrumAnalyzer::SpectrumAnalyzer(int frameSize, int numSpectrums, int numFreqBins, float domainShiftFactor) 
{
	m_fftInSize = frameSize;
	m_fftOutSize = frameSize / 2 + 1;
	m_numSpectrums = numSpectrums;
	m_numFreqBins = numFreqBins;
	m_domainShiftFactor = domainShiftFactor;
	m_spectrumID = 0;
	m_amplitudeCurveSize = 0;
	m_amplitudeCurve = nullptr;
	m_peakCurveSize = 0;
	m_peakCurve = nullptr;

	// Initialize fft buffers
	m_fftIn = new float[m_fftInSize]();
	m_fftOutComplex = new float[m_fftOutSize * 2]();
	m_fftOutMagnitude = new float[m_fftOutSize]();
	m_fftConfig = kiss_fftr_alloc(m_fftInSize, 0, NULL, NULL);

	// Iniitalize frequency spectrums
	m_spectrums = new FrequencySpectrum *[numSpectrums]();
	for (int i = 0; i < numSpectrums; i++)
	{
		m_spectrums[i] = new FrequencySpectrum;
		m_spectrums[i]->data = new float[numFreqBins]();
	}
	m_finalSpectrum = new FrequencySpectrum;
	m_finalSpectrum->data = new float[numFreqBins]();
	m_tempSpectrumBuffer = new float[numFreqBins]();

}

SpectrumAnalyzer::~SpectrumAnalyzer()
{
	delete[] m_fftIn;
	delete[] m_fftOutComplex;
	delete[] m_fftOutMagnitude;
	free(m_fftConfig);

	// delete old spectrums and finalSpectrum
	for (int i = 0; i < m_numSpectrums; i++)
	{
		delete[] m_spectrums[i]->data;
		delete m_spectrums[i];
	}
	delete[] m_spectrums;
	delete[] m_finalSpectrum->data;
	delete m_finalSpectrum;
	delete[] m_tempSpectrumBuffer;
}

void SpectrumAnalyzer::processFrame()
{
	// Do the fft (real in, complex out) then get the magnitude of the complex output.
	kiss_fftr(m_fftConfig, (kiss_fft_scalar *)m_fftIn, (kiss_fft_cpx *)m_fftOutComplex);
	for (int i = 0; i < m_fftOutSize; i++)
	{
		float rl = m_fftOutComplex[i * 2];
		float im = m_fftOutComplex[i * 2 + 1];
		m_fftOutMagnitude[i] = sqrt(rl * rl + im * im) / (float)m_fftOutSize;
	}

	// Get the spectrum for this frame
	FrequencySpectrum * spectrum = m_spectrums[m_spectrumID];
	m_spectrumID = (m_spectrumID + 1) % m_numSpectrums;

	// Apply the amplitude curve
	if(m_amplitudeCurve)
		for (int i = 0; i < m_fftOutSize; i++)
			m_fftOutMagnitude[i] = utl::getValue(m_amplitudeCurve, m_amplitudeCurveSize, utl::min(m_fftOutMagnitude[i], 1.0f));


	// Resize and domain shift the frequency data
	for (int i = 0; i < m_numFreqBins; i++)
	{
		float t = (float)i / (float)(m_numFreqBins - 1);
		t = 1.0f - pow(1.0f - t, 1.0f / m_domainShiftFactor);
		spectrum->data[i] = utl::getValueLerp(m_fftOutMagnitude, m_fftOutSize, t);
	}

	
	// Apply the peak curve
	if(m_peakCurve)
		applyPeakCurve(spectrum->data);
	

	// Get the average of all frquency spectrums
	for (int i = 0; i < m_numFreqBins; i++)
	{
		float avg = 0.0f;
		for (int j = 0; j < m_numSpectrums; j++)
			avg += m_spectrums[j]->data[i];
		m_finalSpectrum->data[i] = avg / (float)m_numSpectrums;
	}
}

void SpectrumAnalyzer::setFrameSize(int frameSize)
{
	// Calculate new sizes
	m_fftInSize = frameSize;
	m_fftOutSize = frameSize / 2 + 1;
	
	// Delete old buffers
	delete[] m_fftIn;
	delete[] m_fftOutComplex;
	delete[] m_fftOutMagnitude;
	free(m_fftConfig);

	// initialize new buffers
	m_fftIn = new float[m_fftInSize]();
	m_fftOutComplex = new float[m_fftOutSize * 2]();
	m_fftOutMagnitude = new float[m_fftOutSize]();
	m_fftConfig = kiss_fftr_alloc(m_fftInSize, 0, NULL, NULL);
}

void SpectrumAnalyzer::setNumSpectrums(int numSpectrums)
{
	// delete old spectrums
	for (int i = 0; i < m_numSpectrums; i++)
	{
		delete[] m_spectrums[i]->data;
		delete m_spectrums[i];
	}
	delete[] m_spectrums;

	// create new spectrums
	m_numSpectrums = numSpectrums;
	m_spectrumID = 0;
	m_spectrums = new FrequencySpectrum *[m_numSpectrums]();
	for (int i = 0; i < m_numSpectrums; i++)
	{
		m_spectrums[i] = new FrequencySpectrum;
		m_spectrums[i]->data = new float[m_numFreqBins]();
	}
}

void SpectrumAnalyzer::setNumFreqBins(int numFreqBins)
{
	// delete old spectrums and finalSpectrum
	for (int i = 0; i < m_numSpectrums; i++)
	{
		delete[] m_spectrums[i]->data;
		delete m_spectrums[i];
	}
	delete[] m_spectrums;
	delete[] m_finalSpectrum->data;
	delete m_finalSpectrum;
	delete[] m_tempSpectrumBuffer;

	// create new spectrums and finalSpectrum
	m_spectrumID = 0;
	m_numFreqBins = numFreqBins;
	m_spectrums = new FrequencySpectrum *[m_numSpectrums]();
	for (int i = 0; i < m_numSpectrums; i++)
	{
		m_spectrums[i] = new FrequencySpectrum;
		m_spectrums[i]->data = new float[m_numFreqBins]();
	}
	m_finalSpectrum = new FrequencySpectrum;
	m_finalSpectrum->data = new float[m_numFreqBins]();
	m_tempSpectrumBuffer = new float[m_numFreqBins]();
}

void SpectrumAnalyzer::setDomainShiftFactor(float domainShiftFactor)
{
	m_domainShiftFactor = domainShiftFactor;
}

void SpectrumAnalyzer::setAmplitudeCurve(float * amplitudeCurve, int amplitudeCurveSize)
{
	// delete old curve if it exists
	if (m_amplitudeCurve)
		delete[] m_amplitudeCurve;

	// create a new curve buffer and copy in the data
	m_amplitudeCurveSize = amplitudeCurveSize;
	m_amplitudeCurve = new float[m_amplitudeCurveSize];
	for (int i = 0; i < m_amplitudeCurveSize; i++)
		m_amplitudeCurve[i] = amplitudeCurve[i];
}

void SpectrumAnalyzer::setPeakCurve(float * peakCurve, int peakCurveSize)
{
	// delete old curve if it exists
	if (m_peakCurve)
		delete[] m_peakCurve;

	// create a new curve buffer and copy in the data
	m_peakCurveSize = peakCurveSize;
	m_peakCurve = new float[m_peakCurveSize];
	for (int i = 0; i < m_peakCurveSize; i++)
		m_peakCurve[i] = peakCurve[i];
}

void SpectrumAnalyzer::applyPeakCurve(float * data)
{
	// "Walk" to the right over the data
	// fall from the peaks in the shape of the peakcurve and stop falling after hitting the ground
	// store position in the temp spectrum buffer
	float peak = 0.0f;
	float position = 0.0f;
	int curvePos = 0;
	for (int i = 0; i < m_numFreqBins; i++)
	{
		if (data[i] >= position || curvePos >= m_peakCurveSize)
		{
			peak = data[i];
			curvePos = 0;
		}
		else
			curvePos += 1;
		position = peak * utl::clamp(m_peakCurve[m_peakCurveSize - 1 - curvePos], 0.0f, 1.0f);
		m_tempSpectrumBuffer[i] = utl::max(data[i], position);
	}

	// Same thing, but walking to the left this time
	// Overwrite data with the max(position, tempSpectrumBuffer)
	peak = 0.0f;
	position = 0.0f;
	curvePos = 0;
	for (int i = 0; i < m_numFreqBins; i++)
	{
		int i_rev = (m_numFreqBins - 1) - i;
		if (data[i_rev] >= position || curvePos >= m_peakCurveSize)
		{
			peak = data[i_rev];
			curvePos = 0;
		}
		else
			curvePos += 1;
		position = peak * utl::clamp(m_peakCurve[m_peakCurveSize - 1 - curvePos], 0.0f, 1.0f);
		data[i_rev] = utl::max(position, m_tempSpectrumBuffer[i_rev]);
	}
}