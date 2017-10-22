#pragma once

#include <algorithm>

namespace Steinberg {
namespace Vst {

// Обработка входного аудио потока - преобразование его в выходной

//------------------------------------------------------------------------
template <typename SampleType>
SampleType MathReverb::processAudio (SampleType** in, SampleType** out, int32 numChannels, int32 sampleFrames)
{
	// Возвращаемое значение
	SampleType vuPPM = 0;

	if (bBypass) // Пропускаем обработку, если включён проброс
		for (int32 channel = 0; channel < numChannels; channel++)
			for (int32 sample = 0; sample < sampleFrames; sample++)
				out[channel][sample] = in[channel][sample];
	else
	{
		// Обработка
		SampleType processingIn = 0.f;
		SampleType processingOut = 0.f;
		for (int32 sample = 0; sample < sampleFrames; sample++)
		{
			processingOut = 0.f;
			for (int32 channel = 0; channel < numChannels; channel++)
				processingIn = in[channel][sample];
			processingOut = graph->process (processingIn) * fGain; // Получение очередного семпла из графа
			for (int32 channel = 0; channel < numChannels; channel++)
				out[channel][sample] = processingOut;
		}
	}

	// Обновляем значение выходной громкости
	for (int32 sample = 0; sample < sampleFrames; sample++)
		if (out[0][sample] > vuPPM)
			vuPPM = out[0][sample];

	return vuPPM;
}

} // Пространство имён Vst
} // Пространство имён Steinberg
