#pragma once

#include "vstgui4/vstgui/plugin-bindings/vst3editor.h"
#include "mathreverbcontroller.h"
#include "mathreverbparams/paramids.h"
#include "cmathreverbview.h"

namespace VSTGUI {

//------------------------------------------------------------------------
// MathReverbVST3Editor: Декларация
// Переопределённый редактор. Здесь происходит обработка событий изменения параметров.
//------------------------------------------------------------------------
class MathReverbVST3Editor : public VST3Editor
{
public:
  // Конструктор
	MathReverbVST3Editor (Steinberg::Vst::EditController *controller, UTF8StringPtr templateName, UTF8StringPtr xmlFile, CView* internalView)
	: VST3Editor (controller, templateName, xmlFile)
	,mathReverbView((CMathReverbView*) internalView)
	{}

	// Метод, вызываемый при изменении какого-либо параметра
	virtual void 	valueChanged (CControl *pControl) VSTGUI_OVERRIDE_VMETHOD
	{
		VST3Editor::valueChanged (pControl);
		if (pControl->getTag () == kWidthId) // Изменён параметр ширины
			mathReverbView->setWidthNormalized (pControl->getValueNormalized ());
	}

private:
	CMathReverbView* mathReverbView;
};

//------------------------------------------------------------------------
} // Пространство имён VSTGUI
