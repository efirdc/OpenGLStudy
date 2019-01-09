#include "SpectrumAnalyzer.h"

SpectrumAnalyzer::SpectrumAnalyzer(int frameSize) 
{
	m_fftInSize = frameSize;
	m_fftOutSize = frameSize / 2 + 1;

	// Initialize fft buffers
	m_fftIn = new float[m_fftInSize]();
	m_fftOut = new float[m_fftOutSize * 2]();
	m_fftConfig = kiss_fftr_alloc(m_fftInSize, 0, NULL, NULL);

	// Iniitalize frequency spectrum
	m_frequencySpectrum = new FrequencySpectrum(m_fftOutSize);
}

SpectrumAnalyzer::~SpectrumAnalyzer()
{
	delete[] m_fftIn;
	delete[] m_fftOut;
	free(m_fftConfig);
	delete m_frequencySpectrum;
}

void SpectrumAnalyzer::processFrame()
{
	// Do the fft (real in, complex out) then get the magnitude of the complex output.
	kiss_fftr(m_fftConfig, (kiss_fft_scalar *)m_fftIn, (kiss_fft_cpx *)m_fftOut);
	for (int i = 0; i < m_fftOutSize; i++)
	{
		float rl = m_fftOut[i * 2];
		float im = m_fftOut[i * 2 + 1];
		m_frequencySpectrum->data[i] = sqrt(rl * rl + im * im) / (float)m_fftOutSize;
	}
}

void SpectrumAnalyzer::setFrameSize(int frameSize)
{
	// Calculate new sizes
	m_fftInSize = frameSize;
	m_fftOutSize = frameSize / 2 + 1;
	
	// Delete old buffers
	delete[] m_fftIn;
	delete[] m_fftOut;
	free(m_fftConfig);
	delete m_frequencySpectrum;

	// initialize new buffers
	m_fftIn = new float[m_fftInSize]();
	m_fftOut = new float[m_fftOutSize * 2]();
	m_fftConfig = kiss_fftr_alloc(m_fftInSize, 0, NULL, NULL);
	m_frequencySpectrum = new FrequencySpectrum(m_fftOutSize);
}

