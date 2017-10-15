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
		return result;

	// Создаём периметр
	const CRect rect (CPoint(0, 0), CPoint(593, 370));
	// Создадим переопределённое представление
	mathReverbView = new CMathReverbView (rect);

	// Создадим блок для параметров
	UnitInfo unitInfo;
	Unit* unit;

	unitInfo.id = 1;
	unitInfo.parentUnitId = kRootUnitId;
	unitInfo.programListId = kNoProgramListId;

	unit = new Unit (unitInfo);
	addUnit (unit);

	// Параметр Gain
	GainParameter* gainParam = new GainParameter (ParameterInfo::kCanAutomate, kGainId);
	parameters.addParameter (gainParam);
	gainParam->setUnitID (unitInfo.id);

	// Параметр Length
	CoordinateParameter* lengthParam = new CoordinateParameter (ParameterInfo::kCanAutomate, kLengthId, "Length");
	parameters.addParameter (lengthParam);
	lengthParam->setUnitID (unitInfo.id);

	// Параметр Width
	CoordinateParameter* widthParam = new CoordinateParameter (ParameterInfo::kCanAutomate, kWidthId, "Width");
	parameters.addParameter (widthParam);
	widthParam->setUnitID (unitInfo.id);

	// Параметр Height
	CoordinateParameter* heightParam = new CoordinateParameter (ParameterInfo::kCanAutomate, kHeightId, "Height");
	parameters.addParameter (heightParam);
	heightParam->setUnitID (unitInfo.id);

	// Параметр Reflection
	ReflectionParameter* reflectionParam = new ReflectionParameter (ParameterInfo::kCanAutomate, kReflectionId);
	parameters.addParameter (reflectionParam);
	reflectionParam->setUnitID (unitInfo.id);

	// Параметр X Pos
	CoordinateParameter* xPosParam = new CoordinateParameter (ParameterInfo::kCanAutomate, kXPosId, "X Pos");
	parameters.addParameter (xPosParam);
	xPosParam->setUnitID (unitInfo.id);

	// Параметр Y Pos
	CoordinateParameter* yPosParam = new CoordinateParameter (ParameterInfo::kCanAutomate, kYPosId, "Y Pos");
	parameters.addParameter (yPosParam);
	yPosParam->setUnitID (unitInfo.id);

	// Параметр Z Pos
	CoordinateParameter* zPosParam = new CoordinateParameter (ParameterInfo::kCanAutomate, kZPosId, "Z Pos");
	parameters.addParameter (zPosParam);
	zPosParam->setUnitID (unitInfo.id);

	// Параметр VuMeter
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
	if (state)
	{
		// Начальные параметры
		float receivedGain = 1.f
					,receivedLength = 1.f
					,receivedWidth = 1.f
					,receivedHeight = 1.f
					,receivedReflection = 1.f
					,receivedXPos = 0.5
					,receivedYPos = 0.5
					,receivedZPos = 0.5;

		// Получение параметров в том же порядке, что и определены
		if (state->read (&receivedGain, sizeof (float)) != kResultTrue)
			return kResultFalse;
		if (state->read (&receivedLength, sizeof (float)) != kResultTrue)
			return kResultFalse;
		if (state->read (&receivedWidth, sizeof (float)) != kResultTrue)
			return kResultFalse;
		if (state->read (&receivedHeight, sizeof (float)) != kResultTrue)
			return kResultFalse;
		if (state->read (&receivedReflection, sizeof (float)) != kResultTrue)
			return kResultFalse;
		if (state->read (&receivedXPos, sizeof (float)) != kResultTrue)
			return kResultFalse;
		if (state->read (&receivedYPos, sizeof (float)) != kResultTrue)
			return kResultFalse;
		if (state->read (&receivedZPos, sizeof (float)) != kResultTrue)
			return kResultFalse;

		// Установка значений параметров
		setParamNormalized (kGainId, receivedGain);
		setParamNormalized (kLengthId, receivedLength);
		setParamNormalized (kWidthId, receivedWidth);
		setParamNormalized (kHeightId, receivedHeight);
		setParamNormalized (kReflectionId, receivedReflection);
		setParamNormalized (kXPosId, receivedXPos);
		setParamNormalized (kYPosId, receivedYPos);
		setParamNormalized (kZPosId, receivedZPos);

		((CMathReverbView*) mathReverbView)->setParams ((int) receivedWidth*100
																									 ,(int) receivedHeight*100
																									 ,(int) receivedLength*100
																									 ,50
																									 ,-50
																									 ,50
																								   );
		mathReverbView->invalid ();
	}
	return kResultTrue;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API MathReverbController::createView (const char* name)
{
	// Получен запрос интерфейса пользователя
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
	// Возвращаем объект класса графического вывода геометрии помещения, положений источника и приёмника
	// Проверки не проводятся в силу единственности переопределённых представлений
	return mathReverbView;
}

//------------------------------------------------------------------------
} // пространство имён Vst
} // пространство имён Steinberg
