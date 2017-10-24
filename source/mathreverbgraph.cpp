#include "mathreverbgraph.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbGraph: Реализация
//------------------------------------------------------------------------

//------------------------------------------------------------------------
MathReverbGraph::MathReverbGraph (SampleRate sampleRate)
: mNumberOfModelApexes (1)
{
  // Создадим исток - источник
  sourceApex = new MathReverbApex (sampleRate, kNoDelay);

  // Разметим прочие вершины
  modelApexes = (MathReverbApex*)std::malloc ((size_t) (mNumberOfModelApexes * sizeof (MathReverbApex)));

  // Создадим сток - приёмник
  sinkApex = new MathReverbApex (new DelayPoint (modelApexes, 0.5f * sampleRate), 1, kNoBuffer);

  // Создадим прочие вершины
  for (int32 i = 0; i < mNumberOfModelApexes; i++)
    std::memcpy ( modelApexes + i
                , new MathReverbApex (sampleRate, new DelayPoint (sourceApex, 0.5f * sampleRate), 1, kNormalApex)
                , sizeof (MathReverbApex) 
                );

}

//------------------------------------------------------------------------
MathReverbGraph::~MathReverbGraph ()
{
  if (sourceApex)
  {
    delete sourceApex;
    sourceApex = 0;
  }

  if (modelApexes)
  {
    std::free (modelApexes);
    modelApexes = 0;
  }

  if (sinkApex)
  {
    delete sinkApex;
    sinkApex = 0;
  }
}

//------------------------------------------------------------------------
Sample64 MathReverbGraph::process (Sample64 inSample)
{
  sourceApex->setSourceSample (inSample);
  modelApexes[0].setSampleFromApexes();
  return sinkApex->setSampleFromApexes ();
}

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
