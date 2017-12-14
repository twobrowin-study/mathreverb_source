#include "mathreverbcontroller.h"
#include "mathreverbparams/paramids.h"

#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"

#include "base/source/fstring.h"

#include "vstgui/uidescription/delegationcontroller.h"
#include "vstgui/uidescription/uiattributes.h"

#include "mathreverbparams/gain.h"
#include "mathreverbparams/reflection.h"

#include "cmathreverbview.h"

#include <stdio.h>
#include <math.h>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbController Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API MathReverbController::initialize (FUnknown* context)
{
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk)
		return result;

	// Params unit creation
	UnitInfo unitInfo;
	Unit* unit;

	unitInfo.id = 1;
	unitInfo.parentUnitId = kRootUnitId;
	unitInfo.programListId = kNoProgramListId;

	unit = new Unit (unitInfo);
	addUnit (unit);

	// Gain
	GainParameter* gainParam = new GainParameter (ParameterInfo::kCanAutomate, kGainId);
	parameters.addParameter (gainParam);
	gainParam->setUnitID (unitInfo.id);

	// Width
	SizeParameter* widthParam = new SizeParameter (ParameterInfo::kCanAutomate, kWidthId, "Width");
	parameters.addParameter (widthParam);
	widthParam->setUnitID (unitInfo.id);

	// Height
	SizeParameter* heightParam = new SizeParameter (ParameterInfo::kCanAutomate, kHeightId, "Height");
	parameters.addParameter (heightParam);
	heightParam->setUnitID (unitInfo.id);

	// Length
	SizeParameter* lengthParam = new SizeParameter (ParameterInfo::kCanAutomate, kLengthId, "Length");
	parameters.addParameter (lengthParam);
	lengthParam->setUnitID (unitInfo.id);

	// Reflection
	ReflectionParameter* reflectionParam = new ReflectionParameter (ParameterInfo::kCanAutomate, kReflectionId);
	parameters.addParameter (reflectionParam);
	reflectionParam->setUnitID (unitInfo.id);

	// X Pos
	CoordinateParameter* xPosParam = new CoordinateParameter (ParameterInfo::kCanAutomate, kXPosId, "X Pos", widthParam);
	parameters.addParameter (xPosParam);
	xPosParam->setUnitID (unitInfo.id);

	// Y Pos
	CoordinateParameter* yPosParam = new CoordinateParameter (ParameterInfo::kCanAutomate, kYPosId, "Y Pos", lengthParam);
	parameters.addParameter (yPosParam);
	yPosParam->setUnitID (unitInfo.id);

	// Z Pos
	CoordinateParameter* zPosParam = new CoordinateParameter (ParameterInfo::kCanAutomate, kZPosId, "Z Pos", heightParam);
	parameters.addParameter (zPosParam);
	zPosParam->setUnitID (unitInfo.id);

	// VuMeter
	int32 stepCount = 0;
	ParamValue defaultVal = 0;
	int32 flags = ParameterInfo::kIsReadOnly;
	int32 tag = kVuPPMId;
	parameters.addParameter (STR16 ("VuPPM"), 0, stepCount, defaultVal, flags, tag);

	// Bypass
	stepCount = 1;
	defaultVal = 0;
	flags = ParameterInfo::kCanAutomate | ParameterInfo::kIsBypass;
	tag = kBypassId;
	parameters.addParameter (STR16 ("Bypass"), 0, stepCount, defaultVal, flags, tag);

	// mathReverbView
	CRect size (CPoint (0, 0), CPoint (593, 370));
	mathReverbView = (CView*) new CMathReverbView (size, widthParam, heightParam, lengthParam, xPosParam, yPosParam, zPosParam);

	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API MathReverbController::setComponentState (IBStream* state)
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

		// Takking parms
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
		setParamNormalized (kGainId, savedGain, kNoCMathReverb);
		setParamNormalized (kWidthId, savedWidth, kNoCMathReverb);
		setParamNormalized (kHeightId, savedHeight, kNoCMathReverb);
		setParamNormalized (kLengthId, savedLength, kNoCMathReverb);
		setParamNormalized (kReflectionId, savedReflection, kNoCMathReverb);
		setParamNormalized (kXPosId, savedXPos, kNoCMathReverb);
		setParamNormalized (kYPosId, savedYPos, kNoCMathReverb);
		setParamNormalized (kZPosId, savedZPos, kNoCMathReverb);
		setParamNormalized (kBypassId, bypassState, kNoCMathReverb);
	}
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult MathReverbController::setParamNormalized (ParamID tag, ParamValue value, int32 key)
{
	tresult result = setParamNormalized (tag, value);
	// Updating objects
	((CoordinateParameter *) getParameterObject (kXPosId)) -> updateLimit ();
	((CoordinateParameter *) getParameterObject (kYPosId)) -> updateLimit ();
	((CoordinateParameter *) getParameterObject (kZPosId)) -> updateLimit ();
	if (key == kWithCMathReverb)
		((CMathReverbView *) mathReverbView) -> update ();
	return result;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API MathReverbController::createView (const char* name)
{
	// We need user interface
	if (name && strcmp (name, "editor") == 0)
	{
		VST3Editor* view = new VST3Editor (this, "view", "mathreverb.uidesc");
		return view;
	}
	return 0;
}

//-----------------------------------------------------------------------
CView* MathReverbController::createCustomView (UTF8StringPtr name, const UIAttributes &attributes, const IUIDescription *description, VST3Editor *editor)
{
	// There is no other options
	return mathReverbView;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
