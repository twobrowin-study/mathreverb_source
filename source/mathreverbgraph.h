#pragma once
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "mathreverbapex.h"

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
  MathReverbGraph (SampleRate sampleRate);
  // Деструктор
  ~MathReverbGraph ();

  // Метод обрабоки очередного семпла
  Sample64 process (Sample64 inSample);

private:
    // Вершины графа
    MathReverbApex *sourceApex;
    MathReverbApex *modelApexes;
    MathReverbApex *sinkApex;

    int32 mNumberOfModelApexes;
};

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
