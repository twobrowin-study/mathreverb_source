#include "mathreverb.h"
#include "mathreverbprocess.h"
#include "mathreverbparams/paramids.h"
#include "mathreverbcids.h"	// Идентификаторы классов

#include "public.sdk/source/vst/vstaudioprocessoralgo.h"

#include "pluginterfaces/base/ustring.h"	// Для UString128
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/vstpresetkeys.h"	// Для использованя IStreamAttributes

#include <stdio.h>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverb Реализация
//------------------------------------------------------------------------
MathReverb::MathReverb ()
: fVuPPMOld (0.f)
, fGain (1.f)
, fWidth (1.f)
, fHeight (1.f)
, fLength (1.f)
, fReflection (1.f)
, fXPos (0.f)
, fYPos (0.f)
, fZPos (0.f)
, bBypass (false)
{
	// Регистрация класса контроллера, содержащего интерфейс пользователя (тот же, что указан mathreverbentry.cpp)
	setControllerClass (MathReverbControllerUID);
}

//------------------------------------------------------------------------
tresult PLUGIN_API MathReverb::setActive (TBool isActive)
{
	// Проверка наличия устройств
	SpeakerArrangement arr;
	if (getBusArrangement (kOutput, 0, arr) != kResultTrue)
		return kResultFalse;

	// Проверка наличия аудио каналов
	int32 numChannels = SpeakerArr::getChannelCount (arr);
	if (numChannels == 0)
		return kResultFalse;

	if (isActive)
	{
		// Инициализация модели обработчика
		graph = new MathReverbGraph (processSetup.sampleRate);
	}
	else
	{
		// Уничтожение модели обработчика
		delete graph;
		graph = NULL;
	}

	// Вызов метода родителя
	return AudioEffect::setActive (isActive);
}

//------------------------------------------------------------------------
tresult PLUGIN_API MathReverb::initialize (FUnknown* context)
{
	// Инициализация родителя
	tresult result = AudioEffect::initialize (context);

	// Продолжаем, если родитель инициализирован
	if (result != kResultOk)
	{
		return result;
	}

	// Создание аудио шин
	// Стерео вход и выход
	addAudioInput  (STR16 ("Stereo In"),  SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), SpeakerArr::kStereo);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API MathReverb::process (ProcessData& data)
{
	// Метод обработки аудио
	// Здесь 3 шага обработки
	// 1) Чтение изменения параметров, приходящих от хоста
	// 2) Непосредственно обработка
	// 3) Вывод параметра выходной громкости VuPPM обратно в плагин

	// 1) Чтение изменения параметров
	getInputParamChanges (data.inputParameterChanges);

	// 2) Непосредственно обработка
	float fVuPPM = 0.f;
	if (data.numSamples > 0)
	{
		if (data.numInputs == 0 || data.numOutputs == 0)
			// Если нет входных/выходных каналов - заканчиваем
			return kResultOk;

		// Получаем устройства вывода
		SpeakerArrangement arr;
		getBusArrangement (kOutput, 0, arr);
		int32 numChannels = SpeakerArr::getChannelCount (arr);

		// Получаем буферы аудио
		uint32 sampleFramesSize = getSampleFramesSizeInBytes (processSetup, data.numSamples);
		void** in = getChannelBuffersPointer (processSetup, data.inputs[0]);
		void** out = getChannelBuffersPointer (processSetup, data.outputs[0]);

		// Проверка на заглушённые каналы
		// NOTE: проверять каждый канал
		if ((data.inputs[0].silenceFlags != 0) || (!bBypass && (fGain < 0.0001f)))
		{
			// Если входные каналы заглушены - заглушим выходные
			data.outputs[0].silenceFlags = data.inputs[0].silenceFlags;

			// Если каналы заглушены - очистим выходные аудио потоки
			for (int32 i = 0; i < numChannels; i++)
			{
				// В данном случае входные каналы должны быть очищены хостом
				if (in[i] != out[i])
					memset (out[i], 0, sampleFramesSize);
			}

			// Заканчиваем метод
			return kResultOk;
		}

		// Если входные каналы не заглушены - отметим незаглушенными выходные
		data.outputs[0].silenceFlags = 0;

		// Обработка аудио при помощи метода-шаблона
		if (data.symbolicSampleSize == kSample32)
			fVuPPM = processAudio<Sample32> ((Sample32**)in, (Sample32**)out, numChannels, data.numSamples);
		else
			fVuPPM = processAudio<Sample64> ((Sample64**)in, (Sample64**)out, numChannels, data.numSamples);
	}

	// 3) Вывод параметра выходной громкости VuMeter обратно в плагин
	setOutputParamChanges (data.outputParameterChanges, fVuPPM);

	return kResultOk;
}


//------------------------------------------------------------------------
tresult PLUGIN_API MathReverb::getState (IBStream* state)
{
	float  toSaveGain = fGain
			 , toSaveWidth = fWidth
			 , toSaveHeight = fHeight
 			 , toSaveLength = fLength
			 , toSaveReflection = fReflection
			 , toSaveXPos = fXPos + 0.5f
			 , toSaveYPos = fYPos + 0.5f
			 , toSaveZPos = fZPos + 0.5f;
	int32 toSaveBypass = bBypass ? 1 : 0;

#if BYTEORDER == kBigEndian
	SWAP_32 (toSaveGain)
	SWAP_32 (toSaveWidth)
	SWAP_32 (toSaveHeight)
	SWAP_32 (toSaveLength)
	SWAP_32 (toSaveReflection)
	SWAP_32 (toSaveXPos)
	SWAP_32 (toSaveYPos)
	SWAP_32 (toSaveZPos)
	SWAP_32 (toSaveBypass)
#endif

	state->write (&toSaveGain, sizeof (float));
	state->write (&toSaveWidth, sizeof (float));
	state->write (&toSaveHeight, sizeof (float));
	state->write (&toSaveLength, sizeof (float));
	state->write (&toSaveReflection, sizeof (float));
	state->write (&toSaveXPos, sizeof (float));
	state->write (&toSaveYPos, sizeof (float));
	state->write (&toSaveZPos, sizeof (float));
	state->write (&toSaveBypass, sizeof (int32));

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API MathReverb::setState (IBStream* state)
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

		// Получение параметров в том же порядке, что и определены
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

		// Установка значений параметров
		return kResultTrue;
		fGain = savedGain;
		fWidth = savedWidth;
		fHeight = savedHeight;
		fLength = savedLength;
		fReflection = savedReflection;
		fXPos = savedXPos - 0.5f;
		fYPos = savedYPos - 0.5f;
		fZPos = savedZPos - 0.5f;
		bBypass = (bypassState > 0);

		return kResultOk;
	}
}

//------------------------------------------------------------------------
void MathReverb::getInputParamChanges (IParameterChanges* paramChanges)
{
	if (paramChanges)
	{
		for (int32 i = 0; i < paramChanges->getParameterCount (); i++) // Проходим по всем изменённым параметрам
		{
			IParamValueQueue* paramQueue = paramChanges->getParameterData (i); // Очередной изменённый параметр
			if (paramQueue)
			{
				// Определяем необходимые переменные для получения изменений параметров
				ParamValue value;
				int32 sampleOffset;
				int32 numPoints = paramQueue->getPointCount ();
				switch (paramQueue->getParameterId ())
				{
					case kGainId: // Если параметр Gain - записываем его
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							fGain = (float)value;
						break;

					case kWidthId: // Если параметр Width - записываем его
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							fWidth = (float)value;
						break;

					case kHeightId: // Если параметр Height - записываем его
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							fHeight = (float)value;
						break;

					case kLengthId: // Если параметр Length - записываем его
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							fLength = (float)value;
						break;

					case kReflectionId: // Если параметр Reflection - записываем его
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							fReflection = (float)value;
						break;

					case kXPosId: // Если параметр XPos - записываем его
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							fXPos = (float)value - 0.5f;
						break;

					case kYPosId: // Если параметр YPos - записываем его
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							fYPos = (float)value - 0.5f;
						break;

					case kZPosId: // Если параметр ZPos - записываем его
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							fZPos = (float)value - 0.5f;
						break;

					case kBypassId: // Если параметр Bypass - записываем его
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
							bBypass = (value > 0.5f);
						break;
				}
			}
		}
	}
}

//------------------------------------------------------------------------
void MathReverb::setOutputParamChanges (IParameterChanges* paramChanges, float fVuPPM)
{
	// Новое значение VuPPM будет отправлено в хост для синхронизации,
	// после чего он передаст его контроллеру плагина
	if (paramChanges && fVuPPMOld != fVuPPM)
	{
		// Запишем новое значение, если оно изменено
		int32 index = 0;
		IParamValueQueue* paramQueue = paramChanges->addParameterData (kVuPPMId, index);
		if (paramQueue)
		{
			int32 index2 = 0;
			paramQueue->addPoint (0, fVuPPM, index2);
		}
	}
	fVuPPMOld = fVuPPM;
}


//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
