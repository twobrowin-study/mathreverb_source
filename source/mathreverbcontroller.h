#pragma once

#include "vstgui/plugin-bindings/vst3editor.h"
#include "public.sdk/source/vst/vsteditcontroller.h"

#include <vector>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbController
//------------------------------------------------------------------------
class MathReverbController : public EditControllerEx1, public VST3EditorDelegate
{
public:
	//--- ---------------------------------------------------------------------
	// create function required for Plug-in factory,
	// it will be called to create new instances of this controller
	//--- ---------------------------------------------------------------------
	static FUnknown* createInstance (void* /*context*/) { return (IEditController*)new MathReverbController; }

	//---из VST3EditorDelegate---
	virtual CView* createCustomView (UTF8StringPtr name, const UIAttributes &attributes, const IUIDescription *description, VST3Editor *editor)  SMTG_OVERRIDE;

	//---from IPluginBase--------
	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;

	//---from EditController-----
	IPlugView* PLUGIN_API createView (const char* name) SMTG_OVERRIDE;
	tresult PLUGIN_API setComponentState (IBStream* state) SMTG_OVERRIDE;
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
