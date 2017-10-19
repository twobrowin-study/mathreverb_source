#include "mathreverbgraph.h"

#include <algorithm>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbGraph: Декларация
// Математическая модель плагина
// Помещение внутри которого происходит преобразование звука
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
