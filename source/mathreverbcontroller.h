#pragma once

#include "vstgui/plugin-bindings/vst3editor.h"
#include "public.sdk/source/vst/vsteditcontroller.h"

#include <vector>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbController
//------------------------------------------------------------------------
class MathReverbController : public EditControllerEx1
{
public:
	//--- ---------------------------------------------------------------------
	// create function required for Plug-in factory,
	// it will be called to create new instances of this controller
	//--- ---------------------------------------------------------------------
	static FUnknown* createInstance (void* /*context*/)
	{
		return (IEditController*)new MathReverbController;
	}

	//---from IPluginBase--------
	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;

	//---from EditController-----
	IPlugView* PLUGIN_API createView (const char* name) SMTG_OVERRIDE;
	tresult PLUGIN_API setComponentState (IBStream* state) SMTG_OVERRIDE;
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
