#include "mathreverb.h"	// For MathReverb
#include "mathreverbcontroller.h" // For MathReverbController
#include "mathreverbcids.h"	// For class Id's
#include "version.h"	// For versions

#include "public.sdk/source/main/pluginfactoryvst3.h"

#define stringPluginName "MathReverb VST3"

//------------------------------------------------------------------------
//  Module Init - Deinit
//------------------------------------------------------------------------

bool InitModule ()
{
	return true;
}

bool DeinitModule ()
{
	return true;
}

using namespace Steinberg::Vst;

//------------------------------------------------------------------------
// VST plugin entry
//------------------------------------------------------------------------

BEGIN_FACTORY_DEF ("BMSTU dev by Dubrovin Egor",
				   				 "http://www.bmstu.ru",
				   		 		 "dubrovin.en@gmail.com")

	// Processor class
	DEF_CLASS2 (INLINE_UID_FROM_FUID(MathReverbProcessorUID),
				PClassInfo::kManyInstances,	// Several objects
				kVstAudioEffectClass,		// Component class
				stringPluginName,			// Plugin name
				Vst::kDistributable,	// Distribution
				"Reverb",					// Subcategory
				FULL_VERSION_STR,		// Version
				kVstVersionString,		// SDK version
				Steinberg::Vst::MathReverb::createInstance)	// Instance

	// Controller
	DEF_CLASS2 (INLINE_UID_FROM_FUID (MathReverbControllerUID),
				PClassInfo::kManyInstances,  // Several objects
				kVstComponentControllerClass, // Component class
				stringPluginName "Controller",	// Controller name
				0,						// Does not use here
				"",						// Does not use here
				FULL_VERSION_STR,		// Version
				kVstVersionString,		// SDK version
				Steinberg::Vst::MathReverbController::createInstance)// Instance

END_FACTORY
