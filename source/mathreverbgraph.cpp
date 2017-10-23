#include "mathreverbgraph.h"

#include <algorithm>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbGraph: Реализация
//------------------------------------------------------------------------

//------------------------------------------------------------------------
MathReverbGraph::MathReverbGraph (SampleRate sampleRate)
: mSampleRate (sampleRate)
{

}

//------------------------------------------------------------------------
MathReverbGraph::~MathReverbGraph ()
{
  delete sourceApex
}

//------------------------------------------------------------------------
Sample64 MathReverbGraph::process (Sample64 inSample)
{
  return inSample;
}

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
