#include "mathreverb.h"	// for MathReverb
#include "mathreverbcontroller.h" // for MathReverbController
#include "mathreverbcids.h"	// for class ids
#include "version.h"	// for versioning

#include "public.sdk/source/main/pluginfactoryvst3.h"

#define stringPluginName "MathReverb VST3"

//------------------------------------------------------------------------
//  Module init/exit
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// called after library was loaded
bool InitModule ()
{
	return true;
}

//------------------------------------------------------------------------
// called after library is unloaded
bool DeinitModule ()
{
	return true;
}

using namespace Steinberg::Vst;

//------------------------------------------------------------------------
//  VST Plug-in Entry
//------------------------------------------------------------------------
// Windows: do not forget to include a .def file in your project to export
// GetPluginFactory function!
//------------------------------------------------------------------------

BEGIN_FACTORY_DEF ("BMSTU dev by Dubrovin Egor",
				   "http://www.bmstu.ru",
				   "dubrovin.en@gmail.com")

	//---First Plug-in included in this factory-------
	// its kVstAudioEffectClass component
	DEF_CLASS2 (INLINE_UID_FROM_FUID(MathReverbProcessorUID),
				PClassInfo::kManyInstances,	// cardinality
				kVstAudioEffectClass,		// the component category (dont changed this)
				stringPluginName,			// here the Plug-in name (to be changed)
				Vst::kDistributable,	// means that component and controller could be distributed on different computers
				"Fx",					// Subcategory for this Plug-in (to be changed)
				FULL_VERSION_STR,		// Plug-in version (to be changed)
				kVstVersionString,		// the VST 3 SDK version (dont changed this, use always this define)
				Steinberg::Vst::MathReverb::createInstance)	// function pointer called when this component should be instanciated

	// its kVstComponentControllerClass component
	DEF_CLASS2 (INLINE_UID_FROM_FUID (MathReverbControllerUID),
				PClassInfo::kManyInstances,  // cardinality
				kVstComponentControllerClass,// the Controller category (dont changed this)
				stringPluginName "Controller",	// controller name (could be the same than component name)
				0,						// not used here
				"",						// not used here
				FULL_VERSION_STR,		// Plug-in version (to be changed)
				kVstVersionString,		// the VST 3 SDK version (dont changed this, use always this define)
				Steinberg::Vst::MathReverbController::createInstance)// function pointer called when this component should be instanciated

	//----for others Plug-ins contained in this factory, put like for the first Plug-in different DEF_CLASS2---

END_FACTORY
