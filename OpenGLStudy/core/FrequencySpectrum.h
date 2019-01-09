#ifndef FREQUENCYSPECTRUM_H
#define FREQUENCYSPECTRUM_H

class FrequencySpectrum
{
public:
	FrequencySpectrum(int size) : size(size), data(new float[size]()) {}
	~FrequencySpectrum() {delete[] data;}
	void resize(int newSize) { size = newSize; delete[] data; data = new float[size]; }
	float * data;
	int size;
};

#endif