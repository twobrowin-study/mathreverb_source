#pragma once

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
template <typename SampleType>
SampleType MathReverb::processAudio (SampleType** in, SampleType** out, int32 numChannels, int32 sampleFrames) {
	SampleType vuPPM = 0, tmp;

	// in real Plug-in it would be better to do dezippering to avoid jump (click) in gain value
	for (int32 i = 0; i < numChannels; i++) {
		int32 samples = sampleFrames;
		SampleType* ptrIn = (SampleType*)in[i];
		SampleType* ptrOut = (SampleType*)out[i];
		while (--samples >= 0) {
			tmp = (*ptrIn++) * fGain;
			(*ptrOut++) = tmp;
			if (tmp > vuPPM)
				vuPPM = tmp;
		}
	}
	return vuPPM;
}

} // Vst
} // Steinberg
