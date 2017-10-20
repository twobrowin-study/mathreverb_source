#pragma once

#include <algorithm>

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
			for (int32 channel = 0; channel < numChannels; channel++, processingIn += in[channel][sample]);
			processingOut = graph->process (processingIn) * fGain;
			for (int32 channel = 0; channel < numChannels; channel++, out[channel][sample] = processingIn);
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
