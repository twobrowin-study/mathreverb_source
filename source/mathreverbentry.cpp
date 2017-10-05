#include "mathreverb.h"	// Для MathReverb
#include "mathreverbcontroller.h" // Для MathReverbController
#include "mathreverbcids.h"	// Для идентификаторов классов
#include "version.h"	// Для получения версий

#include "public.sdk/source/main/pluginfactoryvst3.h"

#define stringPluginName "MathReverb VST3"

//------------------------------------------------------------------------
//  Инициализация - разрушение модуля
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Вызывается после того, как библиотека загружена
bool InitModule ()
{
	return true;
}

//------------------------------------------------------------------------
// Вызывается после того, как библиотека выгружена
bool DeinitModule ()
{
	return true;
}

using namespace Steinberg::Vst;

//------------------------------------------------------------------------
//  Включение плагина VST
// Здесь описываются права доступа, флаги инициализации и вызываемые
// при инициализации статические методы классов
//------------------------------------------------------------------------

BEGIN_FACTORY_DEF ("BMSTU dev by Dubrovin Egor",
				   				 "http://www.bmstu.ru",
				   		 		 "dubrovin.en@gmail.com")

	// Опишем сначала класс самого плагина (обработчика)
	DEF_CLASS2 (INLINE_UID_FROM_FUID(MathReverbProcessorUID),
				PClassInfo::kManyInstances,	// Возможно создать несколько объектов
				kVstAudioEffectClass,		// Категория компонента
				stringPluginName,			// Название плагина
				Vst::kDistributable,	// Возможен перенос на другие машины
				"Fx",					// Субкатегория плагина (эффекты здесь)
				FULL_VERSION_STR,		// Версия плагина
				kVstVersionString,		// Версия VST 3 SDK
				Steinberg::Vst::MathReverb::createInstance)	// Статический метод, вызываемый для получения экземпляра класса

	// Теперь опишем класс контроллера
	DEF_CLASS2 (INLINE_UID_FROM_FUID (MathReverbControllerUID),
				PClassInfo::kManyInstances,  // Возможно создать несколько объектов
				kVstComponentControllerClass, // Категория контроллера
				stringPluginName "Controller",	// Название контроллера
				0,						// Не используется здесь
				"",						// Не используется здесь
				FULL_VERSION_STR,		// Версия плагина
				kVstVersionString,		// Версия VST 3 SDK
				Steinberg::Vst::MathReverbController::createInstance)// Статический метод, вызываемый для получения экземпляра класса

END_FACTORY
