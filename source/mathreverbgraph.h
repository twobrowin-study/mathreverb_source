#pragma once

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
  float process (float inSample)
  {
    return processTemplate<float> (inSample);
  }
  double process (double inSample)
  {
    return processTemplate<double> (inSample);
  }

private:
  // Шаблон обработки очередного семпла
  template <typename SampleType>
  SampleType processTemplate (SampleType inSample);
};

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
