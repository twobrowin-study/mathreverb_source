#pragma once

#include <algorithm>
#include "mathreverbgraph.h"

namespace Steinberg {
namespace Vst {

// Обработка входного аудио потока - преобразование его в выходной

//------------------------------------------------------------------------
template <typename SampleType>
SampleType MathReverb::processAudio (SampleType** in, SampleType** out, int32 numChannels, int32 sampleFrames)
{
	SampleType vuPPM = 0, tmp;
	// Длина задержки - количество задерживаемых семплов, минимум - 1
	int32 delayInSamples = std::max<int32> (1, 0.f * processSetup.sampleRate);

	int32 channel = 0; // Временное ограничение в один канал

	// Обработка
	SampleType* ptrIn = (SampleType*)in[channel];
	SampleType* ptrOut = (SampleType*)out[channel];
	// int32 tempBufferPos = mBufferPos;
	for (int32 sample = 0; sample < sampleFrames; sample++)
	{
		if (bBypass) // Пропускаем обработку, если включён проброс
			ptrOut[sample] = ptrIn[sample];
		else
		{
			ptrOut[sample] = graph->process<SampleType> (ptrIn[sample]);
			// // Поменяем местами входной и выходной семплы через буфер
			// tmp = ptrIn[sample] * fGain;
			// ptrOut[sample] = mBuffer[channel][tempBufferPos];
			// mBuffer[channel][tempBufferPos] = tmp;
			// tempBufferPos++;
			// // Если дошли до максимума обработки по длине - идём в начало буфера
			// if (tempBufferPos >= delayInSamples)
			// 	tempBufferPos = 0;
		}
		// Обновляем значение выходной громкости
		if (ptrOut[sample] > vuPPM)
			vuPPM = ptrOut[sample];
	}


	// // Расчёт позиции в буфере после обработки на данный момент
	// mBufferPos += sampleFrames;
	// while (delayInSamples && mBufferPos >= delayInSamples)
	// 	mBufferPos -= delayInSamples;

	return vuPPM;
}

} // Пространство имён Vst
} // Пространство имён Steinberg
