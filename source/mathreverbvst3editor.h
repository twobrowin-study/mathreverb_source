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
      {
        // Установка Gain в исходное значение
        pControl->setTag (kGainId);
        pControl->setValue (1.f);
				// Установка Length в исходное значение
        pControl->setTag (kLengthId);
        pControl->setValue (1.f);
				// Установка Width в исходное значение
        pControl->setTag (kWidthId);
        pControl->setValue (1.f);
				// Установка Height в исходное значение
        pControl->setTag (kHeightId);
        pControl->setValue (1.f);
				// Установка Reflection в исходное значение
        pControl->setTag (kReflectionId);
        pControl->setValue (1.f);
				// Установка XPos в исходное значение
        pControl->setTag (kXPosId);
        pControl->setValue (1.f);
				// Установка YPos в исходное значение
        pControl->setTag (kYPosId);
        pControl->setValue (1.f);
				// Установка ZPos в исходное значение
        pControl->setTag (kZPosId);
        pControl->setValue (1.f);
				// Установка Bypass в исходное значение
        pControl->setTag (kBypassId);
        pControl->setValue (0.f);
				recreateView ();
      }
    VST3Editor::valueChanged (pControl);
	}
};

//------------------------------------------------------------------------
} // Пространство имён VSTGUI
