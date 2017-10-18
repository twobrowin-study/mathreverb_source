#pragma once

#include "vstgui4/vstgui/plugin-bindings/vst3editor.h"
#include "mathreverbcontroller.h"
#include "mathreverbparams/paramids.h"

namespace VSTGUI {

//------------------------------------------------------------------------
// MathReverbVST3Editor: Декларация
// Переопределённый редактор. Здесь происходит обработка событий изменения параметров.
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
		if (pControl->getTag () == kResetId) // Нажата кнопка сброса параметров
			if (pControl->getValueNormalized () > 0.5f) // Нажатие кнопки соответвует значению 1.f
        // Сброс параметров
				((Steinberg::Vst::MathReverbController *) originalController)->reset ();
	}
};

//------------------------------------------------------------------------
} // Пространство имён VSTGUI
