#pragma once

#include "vstgui/plugin-bindings/vst3editor.h"
#include "public.sdk/source/vst/vsteditcontroller.h"

#include <vector>

namespace Steinberg {
namespace Vst {

// Addictive key structure
enum ParamSets
{
		kWithCMathReverb
	, kNoCMathReverb
};

//------------------------------------------------------------------------
// MathReverbController: Decloration
//------------------------------------------------------------------------
class MathReverbController : public EditControllerEx1, public VST3EditorDelegate
{
public:
	// Instance
	static FUnknown* createInstance (void* /*context*/) { return (IEditController*)new MathReverbController; }

	//---From IPluginBase----------
	// Controller inint
	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;

	//---From EditControllerEx1----
	// View creation
	IPlugView* PLUGIN_API createView (const char* name) SMTG_OVERRIDE;

	// Set starting state
	tresult PLUGIN_API setComponentState (IBStream* state) SMTG_OVERRIDE;

	// Set some param
	tresult PLUGIN_API setParamNormalized (ParamID tag, ParamValue value, int32 key = kWithCMathReverb) SMTG_OVERRIDE;

	//---From VST3EditorDelegate---
	// Create our view
	virtual CView* createCustomView (UTF8StringPtr name, const UIAttributes &attributes, const IUIDescription *description, VST3Editor *editor)  SMTG_OVERRIDE;

private:
	CView *mathReverbView;
};

//------------------------------------------------------------------------
} // Vst
} // Steinberg
