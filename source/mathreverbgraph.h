#pragma once
#include "pluginterfaces/vst/ivstparameterchanges.h"

typedef float Sample32;
typedef double Sample64;

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
  SampleType32 process (SampleType32 inSample)
  {
    return processTemplate<SampleType32> (inSample);
  }
  SampleType64 process (SampleType64 inSample)
  {
    return processTemplate<SampleType64> (inSample);
  }

private:
  // Шаблон обработки очередного семпла
  template <typename SampleType>
  SampleType processTemplate (SampleType inSample);
};

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
