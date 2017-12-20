#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "mathreverbgraph.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverb: Decloration
//------------------------------------------------------------------------
class MathReverb : public AudioEffect
{
public:
	MathReverb ();

	// New object
	static FUnknown* createInstance (void* /*context*/) { return (IAudioProcessor*)new MathReverb; }

	// Calls after constructor
	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;

	// Activation and deactivation
	tresult PLUGIN_API setActive (TBool state) SMTG_OVERRIDE;

	// Process itself
	tresult PLUGIN_API process (ProcessData& data) SMTG_OVERRIDE;

	// Prams Saving
	tresult PLUGIN_API getState (IBStream* state) SMTG_OVERRIDE;
	tresult PLUGIN_API setState (IBStream* state) SMTG_OVERRIDE;

	// Asks if a given sample size is supported
	tresult PLUGIN_API canProcessSampleSize (int32 symbolicSampleSize) SMTG_OVERRIDE;

protected:
	// Process
	template <typename SampleType>
	SampleType processAudio (SampleType** input, SampleType** output, int32 numChannels, int32 sampleFrames);

	// From host
	void getInputParamChanges (IParameterChanges* paramChanges);
	// To host
	void setOutputParamChanges (IParameterChanges* paramChanges, float fVuPPM);

	// Model params
	float fVuPPMOld;

	float fGain
			, fWidth
			, fHeight
			, fLength
			, fReflection
			, fXPos
			, fYPos
			, fZPos;
	bool bBypass;

	MathReverbGraph* graph;
};

//------------------------------------------------------------------------
} // Vst
} // Steinberg
