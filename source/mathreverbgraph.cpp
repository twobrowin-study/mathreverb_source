#include "mathreverbgraph.h"

#include <algorithm>

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
