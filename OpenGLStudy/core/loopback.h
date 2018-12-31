#ifndef LOOPBACK_H
#define LOOPBACK_H

// https://msdn.microsoft.com/en-us/library/windows/desktop/dd316551(v=vs.85).aspx
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms686615(v=vs.85).aspx
// https://msdn.microsoft.com/en-us/library/windows/desktop/dd371399(v=vs.85).aspx < IMMDeviceEnumerator
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms686615(v=vs.85).aspx < CoCreateInstance

#include <iostream>
#include <Audioclient.h>
#include <audiopolicy.h>
#include <mmdeviceapi.h>

int loopback_init();

int loopback_getSound(float * soundBuffer, int bufferSize);

int loopback_samplesPerSec();

#endif