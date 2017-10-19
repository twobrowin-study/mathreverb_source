#include "mathreverbgraph.h"

#include <algorithm>

namespace Steinberg {
namespace Vst {

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
SampleType MathReverbGraph::processTemplate (SampleType inSample)
{
  return inSample;
}

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
