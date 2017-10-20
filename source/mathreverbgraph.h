#pragma once
#include "pluginterfaces/vst/ivstparameterchanges.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbGraph: Декларация
// Математическая модель плагина
// Помещение внутри которого происходит преобразование звука
//------------------------------------------------------------------------
class MathReverbGraph
{
public:
  // Конструктор
  MathReverbGraph (SampleRate samRate);
  // Деструктор
  ~MathReverbGraph ();

  // Метод обрабоки очередного семпла
  Sample64 process (Sample64 inSample);

private:
    // Количество семплов в 1 секунде
    SampleRate sampleRate;

    // Буфер
  	float **mBuffer;
  	int32 mBufferPos;
};

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
