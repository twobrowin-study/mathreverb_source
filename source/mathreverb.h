#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverb: Декларация
// Описывает обработчик плагина - преобразование аудио потока
//------------------------------------------------------------------------
class MathReverb : public AudioEffect
{
public:
	MathReverb ();

	// Статический метод создания экземпляра класса, будет вызван при включении плагина
	static FUnknown* createInstance (void* /*context*/) { return (IAudioProcessor*)new MathReverb; }

	// Вызывается сразу после конструктора
	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;

	// Включение и выключение плагина
	tresult PLUGIN_API setActive (TBool state) SMTG_OVERRIDE;

	// Метод обработки и применения изменений плагина
	tresult PLUGIN_API process (ProcessData& data) SMTG_OVERRIDE;

	// Методы сохранения параметров
	tresult PLUGIN_API getState (IBStream* state) SMTG_OVERRIDE;
	tresult PLUGIN_API setState (IBStream* state) SMTG_OVERRIDE;

protected:
	// Метод непосредственно обработки аудио потока
	template <typename SampleType>
	SampleType processAudio (SampleType** input, SampleType** output, int32 numChannels, int32 sampleFrames);

	// Параметры модели
	float fVuPPMOld;
	float fGain;
	float **mBuffer;
	int32 mBufferPos;
};

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
