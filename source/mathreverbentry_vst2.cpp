#include "public.sdk/source/vst/vst2wrapper/vst2wrapper.h"
#include "mathreverbcids.h"	// for class ids

//------------------------------------------------------------------------
::AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return Steinberg::Vst::Vst2Wrapper::create (GetPluginFactory (), Steinberg::Vst::MathReverbProcessorUID, 'MRbV2', audioMaster);
}
