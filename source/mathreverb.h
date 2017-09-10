#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverb: directly derived from the helper class AudioEffect
//------------------------------------------------------------------------
class MathReverb : public AudioEffect
{
public:
	MathReverb ();
	virtual ~MathReverb (); // do not forget virtual here

	//--- ---------------------------------------------------------------------
	// create function required for Plug-in factory,
	// it will be called to create new instances of this Plug-in
	//--- ---------------------------------------------------------------------
	static FUnknown* createInstance (void* /*context*/) { return (IAudioProcessor*)new MathReverb; }

	//--- ---------------------------------------------------------------------
	// AudioEffect overrides:
	//--- ---------------------------------------------------------------------
	/** Called at first after constructor */
	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;
	/** Switch the Plug-in on/off */
	tresult PLUGIN_API setActive (TBool state) SMTG_OVERRIDE;
	/** Here we go...the process call */
	tresult PLUGIN_API process (ProcessData& data) SMTG_OVERRIDE;

//------------------------------------------------------------------------
protected:
	//==============================================================================
	template <typename SampleType>
	SampleType processAudio (SampleType** input, SampleType** output, int32 numChannels, int32 sampleFrames);

	// our model values
	float fVuPPMOld;
	float fGain;
	float **mBuffer;
	int32 mBufferPos;
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
