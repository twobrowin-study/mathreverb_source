#pragma once

#include <algorithm>

namespace Steinberg {
namespace Vst {

// Input to output audio processing

//------------------------------------------------------------------------
template <typename SampleType>
SampleType MathReverb::processAudio (SampleType** in, SampleType** out, int32 numChannels, int32 sampleFrames)
{
	// Returning value
	SampleType vuPPM = 0;

	if (bBypass) // No process if Bypass
		for (int32 channel = 0; channel < numChannels; channel++)
			for (int32 sample = 0; sample < sampleFrames; sample++)
				out[channel][sample] = in[channel][sample];
	else
	{
		// Process
		SampleType processingIn = 0.f;
		SampleType processingOut = 0.f;
		for (int32 sample = 0; sample < sampleFrames; sample++)
			for (int32 channel = 0; channel < numChannels; channel++)
			{
				processingIn = in[channel][sample];
				processingOut = graph[channel].process (processingIn, fReflection) * fGain; // New sample process
				out[channel][sample] = processingOut;
			}
	}

	// VuMeter
	for (int32 sample = 0; sample < sampleFrames; sample++)
		if (out[0][sample] > vuPPM)
			vuPPM = out[0][sample];

	return vuPPM;
}

} // Vst
} // Steinberg
