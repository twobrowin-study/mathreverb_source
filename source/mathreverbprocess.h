#pragma once

#include <algorithm>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
template <typename SampleType>
SampleType MathReverb::processAudio (SampleType** in, SampleType** out, int32 numChannels, int32 sampleFrames)
{
	SampleType vuPPM = 0, tmp;

	int32 delayInSamples = std::max<int32> (1, 1.f * processSetup.sampleRate);

	// in real Plug-in it would be better to do dezippering to avoid jump (click) in gain value
	for (int32 channel = 0; channel < numChannels; channel++)
	{
		SampleType* ptrIn = (SampleType*)in[channel];
		SampleType* ptrOut = (SampleType*)out[channel];
		int32 tempBufferPos = mBufferPos;
		for (int32 sample = 0; sample < sampleFrames; sample++)
		{
			tmp = ptrIn[sample] * fGain;
			ptrOut[sample] = mBuffer[channel][tempBufferPos];
			mBuffer[channel][tempBufferPos] = tmp;
			tempBufferPos++;
			if (tempBufferPos >= delayInSamples)
				tempBufferPos = 0;
			if (ptrOut[sample] > vuPPM)
				vuPPM = ptrOut[sample];
		}
	}
	mBufferPos += sampleFrames;
	while (delayInSamples && mBufferPos >= delayInSamples)
		mBufferPos -= delayInSamples;

	return vuPPM;
}

} // Vst
} // Steinberg
