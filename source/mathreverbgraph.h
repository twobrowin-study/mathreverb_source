#pragma once

//------------------------------------------------------------------------
// MathReverbGraph: Декларация
// Математическая модель плагина
// Помещение внутри которого происходит преобразование звука
//------------------------------------------------------------------------
class MathReverbGraph
{
public:
  // Конструктор
  MathReverbGraph ()
  {

  }

  // Деструктор
  ~MathReverbGraph ()
  {

  }

  // Метод обрабоки очередного семпла
  template <typename SampleType>
  SampleType process (SampleType inSample)
  {
    return inSample;
  }
};
