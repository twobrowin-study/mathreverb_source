#pragma once

#include "vstgui/plugin-bindings/vst3editor.h"
#include "public.sdk/source/vst/vsteditcontroller.h"

#include <vector>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbController: Деклорация
// Описывает взаимодействие плагина с пользователем и хостом
//------------------------------------------------------------------------
class MathReverbController : public EditControllerEx1, public VST3EditorDelegate
{
public:
	// Статический метод создания экземпляра класса, будет вызван при включении плагина
	static FUnknown* createInstance (void* /*context*/) { return (IEditController*)new MathReverbController; }

	//---Из IPluginBase----------
	// Инициализация контроллера
	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;

	//---Из EditControllerEx1----
	// Создать представление (Интерфейс пользователя)
	IPlugView* PLUGIN_API createView (const char* name) SMTG_OVERRIDE;

	// Установить состояние параметров
	tresult PLUGIN_API setComponentState (IBStream* state) SMTG_OVERRIDE;

	//---Из VST3EditorDelegate---
	// Создать описанное внешне представление
	virtual CView* createCustomView (UTF8StringPtr name, const UIAttributes &attributes, const IUIDescription *description, VST3Editor *editor)  SMTG_OVERRIDE;

private:
		CView* mathReverbView;
};

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
