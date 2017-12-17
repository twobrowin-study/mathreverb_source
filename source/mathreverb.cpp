#include "mathreverb.h"
#include "mathreverbprocess.h"
#include "mathreverbparams/paramids.h"
#include "mathreverbcids.h"	// class Id's

#include "public.sdk/source/vst/vstaudioprocessoralgo.h"

#include "pluginterfaces/base/ustring.h"	// For UString128
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/vstpresetkeys.h"	// For IStreamAttributes

#include <stdio.h>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverb Implementation
//------------------------------------------------------------------------
MathReverb::MathReverb ()
: fVuPPMOld (0.f)
, fGain (1.f)
, fWidth (1.f)
, fHeight (1.f)
, fLength (1.f)
, fReflection (1.f)
, fXPos (0.f)
, fYPos (0.f)
, fZPos (0.f)
, bBypass (false)
{
	setControllerClass (MathReverbControllerUID);
}

//------------------------------------------------------------------------
tresult PLUGIN_API MathReverb::setActive (TBool isActive)
{
	// Devices test
	SpeakerArrangement arr;
	if (getBusArrangement (kOutput, 0, arr) != kResultTrue)
		return kResultFalse;

	// Channels test
	int32 numChannels = SpeakerArr::getChannelCount (arr);
	if (numChannels == 0)
		return kResultFalse;

	if (isActive)
	{
		// Model init
		graph = (MathReverbGraph*)std::malloc ((size_t) (numChannels * sizeof (MathReverbGraph)));
		for (int32 i = 0; i < numChannels; i++)
	    std::memcpy ( graph + i
	                , new MathReverbGraph (processSetup.sampleRate)
	                , sizeof (MathReverbGraph)
	                );
	}
	else
	{
		// Destroy model
		std::free (graph);
		graph = NULL;
	}

	// Supermethod
	return AudioEffect::setActive (isActive);
}

//------------------------------------------------------------------------
tresult PLUGIN_API MathReverb::initialize (FUnknown* context)
{
	// Parant init
	tresult result = AudioEffect::initialize (context);

	// If parent initialized
	if (result != kResultOk)
	{
		return result;
	}

	// Stereo input and output
	addAudioInput  (STR16 ("Stereo In"),  SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), SpeakerArr::kStereo);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API MathReverb::process (ProcessData& data)
{
	// Audio processing
	// There is 3 steps here
	// 1) Getting input parm changes
	// 2) Process itself
	// 3) Send output params changes

	// 1) Getting input parm changes
	getInputParamChanges (data.inputParameterChanges);

	// 2) Process itself
	float fVuPPM = 0.f;
	if (data.numSamples > 0)
	{
		if (data.numInputs == 0 || data.numOutputs == 0)
			// If no channels - leaving
			return kResultOk;

		// Getting device
		SpeakerArrangement arr;
		getBusArrangement (kOutput, 0, arr);
		int32 numChannels = SpeakerArr::getChannelCount (arr);

		// Getting buffers
		uint32 sampleFramesSize = getSampleFramesSizeInBytes (processSetup, data.numSamples);
		void** in = getChannelBuffersPointer (processSetup, data.inputs[0]);
		void** out = getChannelBuffersPointer (processSetup, data.outputs[0]);

		// Silence test
		if ((data.inputs[0].silenceFlags != 0) || (!bBypass && (fGain < 0.0001f)))
		{
			// If silence
			data.outputs[0].silenceFlags = data.inputs[0].silenceFlags;

			// Clear outputs
			for (int32 i = 0; i < numChannels; i++)
			{
				// Inputs must be cleared by host
				if (in[i] != out[i])
					memset (out[i], 0, sampleFramesSize);
			}

			// Leave
			return kResultOk;
		}

		// If inputs are not silence - so outputs
		data.outputs[0].silenceFlags = 0;

		// Process with help of template
		if (data.symbolicSampleSize == kSample64)
			fVuPPM = processAudio<Sample64> ((Sample64**)in, (Sample64**)out, numChannels, data.numSamples);
			// fVuPPM = processAudio<Sample32> ((Sample32**)in, (Sample32**)out, numChannels, data.numSamples);
		// else
		// 	fVuPPM = processAudio<Sample64> ((Sample64**)in, (Sample64**)out, numChannels, data.numSamples);
	}

	// 3) Send output params changes
	setOutputParamChanges (data.outputParameterChanges, fVuPPM);

	return kResultOk;
}


//------------------------------------------------------------------------
tresult PLUGIN_API MathReverb::getState (IBStream* state)
{
	float  toSaveGain = fGain
			 , toSaveWidth = fWidth
			 , toSaveHeight = fHeight
 			 , toSaveLength = fLength
			 , toSaveReflection = fReflection
			 , toSaveXPos = fXPos + 0.5f
			 , toSaveYPos = fYPos + 0.5f
			 , toSaveZPos = fZPos + 0.5f;
	int32 toSaveBypass = bBypass ? 1 : 0;

#if BYTEORDER == kBigEndian
	SWAP_32 (toSaveGain)
	SWAP_32 (toSaveWidth)
	SWAP_32 (toSaveHeight)
	SWAP_32 (toSaveLength)
	SWAP_32 (toSaveReflection)
	SWAP_32 (toSaveXPos)
	SWAP_32 (toSaveYPos)
	SWAP_32 (toSaveZPos)
	SWAP_32 (toSaveBypass)
#endif

	state->write (&toSaveGain, sizeof (float));
	state->write (&toSaveWidth, sizeof (float));
	state->write (&toSaveHeight, sizeof (float));
	state->write (&toSaveLength, sizeof (float));
	state->write (&toSaveReflection, sizeof (float));
	state->write (&toSaveXPos, sizeof (float));
	state->write (&toSaveYPos, sizeof (float));
	state->write (&toSaveZPos, sizeof (float));
	state->write (&toSaveBypass, sizeof (int32));

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API MathReverb::setState (IBStream* state)
{
	if (state)
	{
		float  savedGain
				 , savedWidth
				 , savedHeight
				 , savedLength
				 , savedReflection
				 , savedXPos
				 , savedYPos
				 , savedZPos;
		int32 bypassState;

		// Getting params
		if (state->read (&savedGain, sizeof (float)) != kResultTrue)
			return kResultFalse;
		if (state->read (&savedWidth, sizeof (float)) != kResultTrue)
			return kResultFalse;
		if (state->read (&savedHeight, sizeof (float)) != kResultTrue)
			return kResultFalse;
		if (state->read (&savedLength, sizeof (float)) != kResultTrue)
			return kResultFalse;
		if (state->read (&savedReflection, sizeof (float)) != kResultTrue)
			return kResultFalse;
		if (state->read (&savedXPos, sizeof (float)) != kResultTrue)
			return kResultFalse;
		if (state->read (&savedYPos, sizeof (float)) != kResultTrue)
			return kResultFalse;
		if (state->read (&savedZPos, sizeof (float)) != kResultTrue)
			return kResultFalse;
		if (state->read (&bypassState, sizeof (bypassState)) != kResultTrue)
			return kResultFalse;

		#if BYTEORDER == kBigEndian
			SWAP_32 (savedGain)
			SWAP_32 (savedWidth)
			SWAP_32 (savedHeight)
			SWAP_32 (savedLength)
			SWAP_32 (savedRefelection)
			SWAP_32 (savedXPos)
			SWAP_32 (savedYPos)
			SWAP_32 (savedZPos)
			SWAP_32 (bypassState)
		#endif

		// Setting params
		return kResultTrue;
		fGain = savedGain;
		fWidth = savedWidth;
		fHeight = savedHeight;
		fLength = savedLength;
		fReflection = savedReflection;
		fXPos = savedXPos - 0.5f;
		fYPos = savedYPos - 0.5f;
		fZPos = savedZPos - 0.5f;
		bBypass = (bypassState > 0);

		return kResultOk;
	}
}

//------------------------------------------------------------------------
void MathReverb::getInputParamChanges (IParameterChanges* paramChanges)
{
	if (paramChanges)
	{
		for (int32 i = 0; i < paramChanges->getParameterCount (); i++) // All changed params
		{
			IParamValueQueue* paramQueue = paramChanges->getParameterData (i); // Changed param
			if (paramQueue)
			{
				// Requested params
				ParamValue value;
				int32 sampleOffset;
				int32 numPoints = paramQueue->getPointCount ();
				switch (paramQueue->getParameterId ())
				{
					case kGainId: // Gain
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							fGain = (float)value;
						break;

					case kWidthId: // Width
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							fWidth = (float)value;
						break;

					case kHeightId: // Height
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							fHeight = (float)value;
						break;

					case kLengthId: // Length
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							fLength = (float)value;
						break;

					case kReflectionId: // Reflection
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							fReflection = (float)value;
						break;

					case kXPosId: // XPos
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							fXPos = (float)value - 0.5f;
						break;

					case kYPosId: // YPos
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							fYPos = (float)value - 0.5f;
						break;

					case kZPosId: // ZPos
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							fZPos = (float)value - 0.5f;
						break;

					case kBypassId: // Bypass
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							bBypass = (value > 0.5f);
						break;
				}
			}
		}
		// Updating model
		SpeakerArrangement arr;
		getBusArrangement (kOutput, 0, arr);
		int32 numChannels = SpeakerArr::getChannelCount (arr);
		for (int32 channel = 0; channel < numChannels; channel++)
			graph[channel].setDementoinParams (fWidth, fHeight, fLength, fXPos, fYPos, fZPos); // New params
	}
}

//------------------------------------------------------------------------
void MathReverb::setOutputParamChanges (IParameterChanges* paramChanges, float fVuPPM)
{
	// New vuPPM will be send to host
	if (paramChanges && fVuPPMOld != fVuPPM)
	{
		// if changed - write
		int32 index = 0;
		IParamValueQueue* paramQueue = paramChanges->addParameterData (kVuPPMId, index);
		if (paramQueue)
		{
			int32 index2 = 0;
			paramQueue->addPoint (0, fVuPPM, index2);
		}
	}
	fVuPPMOld = fVuPPM;
}


//------------------------------------------------------------------------
} // Vst
} // Steinberg
