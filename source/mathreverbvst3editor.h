#pragma once

#include "vstgui4/vstgui/plugin-bindings/vst3editor.h"
#include "mathreverbparams/paramids.h"

namespace VSTGUI {

//------------------------------------------------------------------------
// MathReverbVST3Editor: Декларация
// Переопределённый редактор, закрывается при нажатии кнопки exit
//------------------------------------------------------------------------
class MathReverbVST3Editor : public VST3Editor
{
public:
  // Конструктор
	MathReverbVST3Editor (Steinberg::Vst::EditController *controller, UTF8StringPtr templateName, UTF8StringPtr xmlFile)
	: VST3Editor (controller, templateName, xmlFile)
	{}

	// Метод, вызываемый при изменении какого-либо параметра
	virtual void 	valueChanged (CControl *pControl) VSTGUI_OVERRIDE_VMETHOD
	{
		if (pControl->getTag () == kExitId)
			if (pControl->getValueNormalized () > 0.5f)
				((EditControllerEx1 *) originalController)->terminate() // Если нажата кнопка Закрыть - закрываем контроллер
	}
};

//------------------------------------------------------------------------
} // Пространство имён VSTGUI
