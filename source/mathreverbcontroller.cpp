#include "mathreverbcontroller.h"
#include "mathreverbparamids.h"

#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"

#include "base/source/fstring.h"

#include "vstgui/uidescription/delegationcontroller.h"

#include "mathreverbparams/gain.h"
#include "cmathreverbview.h"

#include <stdio.h>
#include <math.h>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbController Реализация
//------------------------------------------------------------------------
tresult PLUGIN_API MathReverbController::initialize (FUnknown* context) {
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk)
	{
		return result;
	}

	//---Create Unit for Gain---
	UnitInfo unitInfo;
	Unit* unit;

	unitInfo.id = 1;
	unitInfo.parentUnitId = kRootUnitId;
	unitInfo.programListId = kNoProgramListId;

	unit = new Unit (unitInfo);
	addUnit (unit);

	//---Gaint parameter---
	GainParameter* gainParam = new GainParameter (ParameterInfo::kCanAutomate, kGainId);
	parameters.addParameter (gainParam);
	gainParam->setUnitID (unitInfo.id);

	//---VuMeter parameter---
	int32 stepCount = 0;
	ParamValue defaultVal = 0;
	int32 flags = ParameterInfo::kIsReadOnly;
	int32 tag = kVuPPMId;
	parameters.addParameter (STR16 ("VuPPM"), 0, stepCount, defaultVal, flags, tag);

	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API MathReverbController::setComponentState (IBStream* state)
{
	if (state) {
		float receivedGain = 1.f;
		if (state->read (&receivedGain, sizeof (float)) != kResultTrue)
			return kResultFalse;
		setParamNormalized (kGainId, receivedGain);
	}
	return kResultTrue;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API MathReverbController::createView (const char* name)
{
	// someone wants my editor
	if (name && strcmp (name, "editor") == 0)
	{
		VST3Editor* view = new VST3Editor (this, "view", "mathreverb.uidesc");
		return view;
	}
	return 0;
}

CView* MathReverbController::createCustomView (UTF8StringPtr name, const UIAttributes &attributes, const IUIDescription *description, VST3Editor *editor)
{
	return new CMathReverbView(attributes.size);
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
