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
  MathReverbGraph ();
  // Деструктор
  ~MathReverbGraph ();

  // Методы обрабоки очередного семпла
  Sample32 process (Sample32 inSample)
  {
    return processTemplate<Sample32> (inSample);
  }
  Sample64 process (Sample64 inSample)
  {
    return processTemplate<Sample64> (inSample);
  }

private:
  // Шаблон обработки очередного семпла
  template <typename SampleType>
  SampleType processTemplate (SampleType inSample);
};

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
