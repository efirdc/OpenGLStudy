#include <loopback.h>

int loopback_initialized = 0;
IAudioCaptureClient * pAudioCaptureClient;
WAVEFORMATEX * pwfx;

int loopback_init()
{
	if (loopback_initialized)
		return 0;

	loopback_initialized = 1;

	HRESULT hr;
	hr = CoInitialize(NULL);

	// Activate a device enumerator
	IMMDeviceEnumerator *pMMDeviceEnumerator = NULL;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pMMDeviceEnumerator);

	// Get the IMMDevice
	IMMDevice *pMMDevice;
	hr = pMMDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pMMDevice);

	// activate an IAudioCLient with the IMMDevice
	IAudioClient *pAudioClient;
	hr = pMMDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&pAudioClient);

	// get the default device format
	hr = pAudioClient->GetMixFormat(&pwfx);

	// call IAudioClient::Initialize
	hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, 0, 0, pwfx, 0);

	// Get a pAudioCpatureClient
	hr = pAudioClient->GetService(__uuidof(IAudioCaptureClient), (void**)&pAudioCaptureClient);

	// call IAudioClient::Start
	hr = pAudioClient->Start();
	return 0;
}

int loopback_getSound(float * soundBuffer, int bufferSize)
{
	if (!loopback_initialized)
		return 0;

	HRESULT hr;
	UINT32 nextPacketSize;
	BYTE * pData = NULL;
	UINT32 numFramesToRead;
	DWORD dwFlags;
	int totalSamples = 0;

	// Reads new packets for as long as data is available
	for (hr = pAudioCaptureClient->GetNextPacketSize(&nextPacketSize); SUCCEEDED(hr) && nextPacketSize > 0; hr = pAudioCaptureClient->GetNextPacketSize(&nextPacketSize))
	{
		// Read the audio buffer
		hr = pAudioCaptureClient->GetBuffer(&pData, &numFramesToRead, &dwFlags, NULL, NULL);
		
		// Cast the packet buffer to floating point array and calculate the number of floats in the array
		float * packetBuffer = (float *)pData;
		int numBytes = numFramesToRead * pwfx->nBlockAlign;
		int numFloats =  (numBytes / sizeof(float)) / pwfx->nChannels;
		totalSamples += numFloats;

		// Shift contents of buffer to the front to make room for the new samples
		for (int i = 0; i < (bufferSize - numFloats); i++)
			soundBuffer[i] = soundBuffer[i + numFloats];

		// Copy in the new audio samples to the back of the audio buffer
		for (int i = 0; i < min(bufferSize, numFloats); i++)
		{
			float & sample = soundBuffer[bufferSize - i - 1];
			sample = 0.0;
			for (int ch = 0; ch < pwfx->nChannels; ch++)
				sample += packetBuffer[(numFloats - i - 1) * pwfx->nChannels + ch];
			sample /= pwfx->nChannels;
		}

		/*
		for (int i = 0; i < numFloats; i++)
		{
			float & sample = soundBuffer[bufferSize - numFloats + i];
			sample = 0.0;
			for (int c = 0; c < pwfx->nChannels; c++)
				sample += packetBuffer[i * pwfx->nChannels + c];
			sample /= pwfx->nChannels;
		}
		*/

		// Release the buffer
		hr = pAudioCaptureClient->ReleaseBuffer(numFramesToRead);
	}

	return totalSamples;
}

int loopback_samplesPerSec()
{
	return pwfx->nSamplesPerSec;
}
