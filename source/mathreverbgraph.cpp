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
  if (sourceApex)
  {
    delete sourceApex;
    sourceApex = 0;
  }
}

//------------------------------------------------------------------------
Sample64 MathReverbGraph::process (Sample64 inSample)
{
  return inSample;
}

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
