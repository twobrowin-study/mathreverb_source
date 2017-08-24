#include "mathreverb.h"
#include "mathreverbprocess.h"
#include "mathreverbparamids.h"
#include "mathreverbcids.h"	// for class ids

#include "pluginterfaces/base/ustring.h"	// for UString128
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/vstpresetkeys.h"	// for use of IStreamAttributes

#include <stdio.h>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverb Implementation
//------------------------------------------------------------------------
MathReverb::MathReverb () :fVuPPMOld (0.f), fGain (1.f) {
	// register its editor class (the same than used in mathreverbentry.cpp)
	setControllerClass (MathReverbControllerUID);
}

//------------------------------------------------------------------------
MathReverb::~MathReverb () {
	// nothing to do here yet..
}

//------------------------------------------------------------------------
tresult PLUGIN_API MathReverb::initialize (FUnknown* context) {
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk) {
		return result;
	}

	//---create Audio In/Out buses------
	// we want a stereo Input and a Stereo Output
	addAudioInput  (STR16 ("Stereo In"),  SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), SpeakerArr::kStereo);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API MathReverb::process (ProcessData& data) {
	// finally the process function
	// In this example there are 4 steps:
	// 1) Read inputs parameters coming from host (in order to adapt our model values)
	// 2) Read inputs events coming from host (we apply a gain reduction depending of the velocity of pressed key)
	// 3) Process the gain of the input buffer to the output buffer
	// 4) Write the new VUmeter value to the output Parameters queue

	//---1) Read inputs param changes------
	IParameterChanges* paramChanges = data.inputParameterChanges;
	if (paramChanges) {
		for (int32 i = 0; i < paramChanges->getParameterCount (); i++) {
			IParamValueQueue* paramQueue = paramChanges->getParameterData (i);
			if (paramQueue) {
				ParamValue value;
				int32 sampleOffset;
				int32 numPoints = paramQueue->getPointCount ();
				if ( (paramQueue->getParameterId () == kGainId) && (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue) )
					fGain = (float)value;
			}
		}
	}

	//-------------------------------------
	//---3) Process Audio------------------
	//-------------------------------------
	if (data.numInputs == 0 || data.numOutputs == 0) {
		// nothing to do
		return kResultOk;
	}

	// (simplification) we suppose in this example that we have the same input channel count than the output
	int32 numChannels = data.inputs[0].numChannels;

	//---get audio buffers----------------
	uint32 sampleFramesSize = getSampleFramesSizeInBytes (data.numSamples);
	void** in = getChannelBuffersPointer (data.inputs[0]);
	void** out = getChannelBuffersPointer (data.outputs[0]);

	//---check if silence---------------
	// normally we have to check each channel (simplification)
	if (data.inputs[0].silenceFlags != 0) {
		// mark output silence too
		data.outputs[0].silenceFlags = data.inputs[0].silenceFlags;

		// the Plug-in has to be sure that if it sets the flags silence that the output buffer are clear
		for (int32 i = 0; i < numChannels; i++) {
			// dont need to be cleared if the buffers are the same (in this case input buffer are already cleared by the host)
			if (in[i] != out[i])
				memset (out[i], 0, sampleFramesSize);
		}

		// nothing to do at this point
		return kResultOk;
	}

	// mark our outputs has not silent
	data.outputs[0].silenceFlags = 0;

	float fVuPPM = 0.f;

	if (data.symbolicSampleSize == kSample32)
		fVuPPM = processAudio<Sample32> ((Sample32**)in, (Sample32**)out, numChannels, data.numSamples);
	else
		fVuPPM = processAudio<Sample64> ((Sample64**)in, (Sample64**)out, numChannels, data.numSamples);

	//---4) Write outputs parameter changes-----------
	IParameterChanges* outParamChanges = data.outputParameterChanges;
	// a new value of VuMeter will be send to the host
	// (the host will send it back in sync to our controller for updating our editor)
	if (outParamChanges && fVuPPMOld != fVuPPM) {
		int32 index = 0;
		IParamValueQueue* paramQueue = outParamChanges->addParameterData (kVuPPMId, index);
		if (paramQueue) {
			int32 index2 = 0;
			paramQueue->addPoint (0, fVuPPM, index2);
		}
	}
	fVuPPMOld = fVuPPM;

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
