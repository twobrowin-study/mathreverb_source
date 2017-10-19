#pragma once

#include <algorithm>

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

  // Метод обрабоки очередного семпла
  template <typename SampleType>
  SampleType process (SampleType inSample);
};

//------------------------------------------------------------------------
// MathReverbGraph: Реализация
//------------------------------------------------------------------------

//------------------------------------------------------------------------
MathReverbGraph::MathReverbGraph ()
{

}

//------------------------------------------------------------------------
MathReverbGraph::~MathReverbGraph ()
{

}

//------------------------------------------------------------------------
template <typename SampleType>
SampleType MathReverbGraph::process (SampleType inSample)
{
  return inSample;
}


//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
