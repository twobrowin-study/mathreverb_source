#include "mathreverbcontroller.h"
#include "mathreverbparams/paramids.h"

#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"

#include "base/source/fstring.h"

#include "vstgui/uidescription/delegationcontroller.h"
#include "vstgui/uidescription/uiattributes.h"

#include "mathreverbparams/gain.h"
#include "mathreverbparams/coordinate.h"
#include "mathreverbparams/reflection.h"
#include "mathreverbparams/globalparams.h"

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

	//---Создадим блок для параметров---
	UnitInfo unitInfo;
	Unit* unit;

	unitInfo.id = 1;
	unitInfo.parentUnitId = kRootUnitId;
	unitInfo.programListId = kNoProgramListId;

	unit = new Unit (unitInfo);
	addUnit (unit);

	//---Параметр Gain---
	GainParameter* gainParam = new GainParameter (ParameterInfo::kCanAutomate, kGainId);
	parameters.addParameter (gainParam);
	gainParam->setUnitID (unitInfo.id);

	//---Параметр Reflection---
	ReflectionParameter* reflectionParam = new ReflectionParameter (ParameterInfo::kCanAutomate, kReflectionId);
	parameters.addParameter (reflectionParam);
	reflectionParam->setUnitID (unitInfo.id);

	//---Параметр VuMeter---
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
		setParamNormalized (kReflectionId, 1.f);
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

//-----------------------------------------------------------------------
CView* MathReverbController::createCustomView (UTF8StringPtr name, const UIAttributes &attributes, const IUIDescription *description, VST3Editor *editor)
{
	// Получаем атрибуты начала
	CPoint origin, size;
	attributes.getPointAttribute ("origin", origin);
	attributes.getPointAttribute ("size", size);
	// Создаём периметр
	const CRect rect(origin, size);
	// Возвращаем объект класса графического вывода геометрии помещения, положений источника и приёмника
	// Проверки не проводятся в силу единственности переопределённых представлений
	return new CMathReverbView(rect);
}

//------------------------------------------------------------------------
} // пространство имён Vst
} // пространство имён Steinberg
