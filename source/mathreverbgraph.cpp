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
  sourceApex = new MathReverbApex (sampleRate, kNoDelay);
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
  sourceApex->setSourceSample (inSample);
  return sourceApex->getSampleWithDelay (0.5f * mSampleRate);
  // return inSample;
}

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
