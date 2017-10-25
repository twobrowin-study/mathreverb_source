#include "mathreverbgraph.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbGraph: Реализация
//------------------------------------------------------------------------

//------------------------------------------------------------------------
MathReverbGraph::MathReverbGraph (SampleRate sampleRate)
: mNumberOfModelApexes (8)
{
  // Создадим исток - источник
  sourceApex = new MathReverbApex (sampleRate, kNoDelay);

  // Разметим прочие вершины
  modelApexes = (MathReverbApex*)std::malloc ((size_t) (mNumberOfModelApexes * sizeof (MathReverbApex)));

  // Создадим сток - приёмник
  DelayPoint sinkApexDelay [8] = {
    DelayPoint (modelApexes + 0, 0.05f * sampleRate),
    DelayPoint (modelApexes + 1, 0.05f * sampleRate),
    DelayPoint (modelApexes + 2, 0.05f * sampleRate),
    DelayPoint (modelApexes + 3, 0.05f * sampleRate),
    DelayPoint (modelApexes + 4, 0.05f * sampleRate),
    DelayPoint (modelApexes + 5, 0.05f * sampleRate),
    DelayPoint (modelApexes + 6, 0.05f * sampleRate),
    DelayPoint (modelApexes + 7, 0.05f * sampleRate)
  };
  sinkApex = new MathReverbApex (sinkApexDelay, mNumberOfModelApexes, kNoBuffer);

  // Создадим прочие вершины
  DelayPoint modelApexesDelay [8][8] = {
    {
      DelayPoint (sourceApex, 0.05f * sampleRate),
      DelayPoint (modelApexes + 1, 0.1f * sampleRate),
      DelayPoint (modelApexes + 2, 0.1f * sampleRate),
      DelayPoint (modelApexes + 3, 0.1f * sampleRate),
      DelayPoint (modelApexes + 4, 0.1f * sampleRate),
      DelayPoint (modelApexes + 5, 0.1f * sampleRate),
      DelayPoint (modelApexes + 6, 0.1f * sampleRate),
      DelayPoint (modelApexes + 7, 0.1f * sampleRate)
    },
    {
      DelayPoint (sourceApex, 0.05f * sampleRate),
      DelayPoint (modelApexes + 0, 0.1f * sampleRate),
      DelayPoint (modelApexes + 2, 0.1f * sampleRate),
      DelayPoint (modelApexes + 3, 0.1f * sampleRate),
      DelayPoint (modelApexes + 4, 0.1f * sampleRate),
      DelayPoint (modelApexes + 5, 0.1f * sampleRate),
      DelayPoint (modelApexes + 6, 0.1f * sampleRate),
      DelayPoint (modelApexes + 7, 0.1f * sampleRate)
    },
    {
      DelayPoint (sourceApex, 0.05f * sampleRate),
      DelayPoint (modelApexes + 0, 0.1f * sampleRate),
      DelayPoint (modelApexes + 1, 0.1f * sampleRate),
      DelayPoint (modelApexes + 3, 0.1f * sampleRate),
      DelayPoint (modelApexes + 4, 0.1f * sampleRate),
      DelayPoint (modelApexes + 5, 0.1f * sampleRate),
      DelayPoint (modelApexes + 6, 0.1f * sampleRate),
      DelayPoint (modelApexes + 7, 0.1f * sampleRate)
    },
    {
      DelayPoint (sourceApex, 0.05f * sampleRate),
      DelayPoint (modelApexes + 0, 0.1f * sampleRate),
      DelayPoint (modelApexes + 1, 0.1f * sampleRate),
      DelayPoint (modelApexes + 2, 0.1f * sampleRate),
      DelayPoint (modelApexes + 4, 0.1f * sampleRate),
      DelayPoint (modelApexes + 5, 0.1f * sampleRate),
      DelayPoint (modelApexes + 6, 0.1f * sampleRate),
      DelayPoint (modelApexes + 7, 0.1f * sampleRate)
    },
    {
      DelayPoint (sourceApex, 0.05f * sampleRate),
      DelayPoint (modelApexes + 0, 0.1f * sampleRate),
      DelayPoint (modelApexes + 1, 0.1f * sampleRate),
      DelayPoint (modelApexes + 2, 0.1f * sampleRate),
      DelayPoint (modelApexes + 3, 0.1f * sampleRate),
      DelayPoint (modelApexes + 5, 0.1f * sampleRate),
      DelayPoint (modelApexes + 6, 0.1f * sampleRate),
      DelayPoint (modelApexes + 7, 0.1f * sampleRate)
    },
    {
      DelayPoint (sourceApex, 0.05f * sampleRate),
      DelayPoint (modelApexes + 0, 0.1f * sampleRate),
      DelayPoint (modelApexes + 1, 0.1f * sampleRate),
      DelayPoint (modelApexes + 2, 0.1f * sampleRate),
      DelayPoint (modelApexes + 3, 0.1f * sampleRate),
      DelayPoint (modelApexes + 4, 0.1f * sampleRate),
      DelayPoint (modelApexes + 6, 0.1f * sampleRate),
      DelayPoint (modelApexes + 7, 0.1f * sampleRate)
    },
    {
      DelayPoint (sourceApex, 0.05f * sampleRate),
      DelayPoint (modelApexes + 0, 0.1f * sampleRate),
      DelayPoint (modelApexes + 1, 0.1f * sampleRate),
      DelayPoint (modelApexes + 2, 0.1f * sampleRate),
      DelayPoint (modelApexes + 3, 0.1f * sampleRate),
      DelayPoint (modelApexes + 4, 0.1f * sampleRate),
      DelayPoint (modelApexes + 5, 0.1f * sampleRate),
      DelayPoint (modelApexes + 7, 0.1f * sampleRate)
    },
    {
      DelayPoint (sourceApex, 0.05f * sampleRate),
      DelayPoint (modelApexes + 0, 0.1f * sampleRate),
      DelayPoint (modelApexes + 1, 0.1f * sampleRate),
      DelayPoint (modelApexes + 2, 0.1f * sampleRate),
      DelayPoint (modelApexes + 3, 0.1f * sampleRate),
      DelayPoint (modelApexes + 4, 0.1f * sampleRate),
      DelayPoint (modelApexes + 5, 0.1f * sampleRate),
      DelayPoint (modelApexes + 6, 0.1f * sampleRate)
    }
  };
  for (int32 i = 0; i < mNumberOfModelApexes; i++)
    std::memcpy ( modelApexes + i
                , new MathReverbApex (sampleRate, modelApexesDelay[i], mNumberOfModelApexes, kNormalApex)
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
Sample64 MathReverbGraph::process (Sample64 inSample, float reflection)
{
  sourceApex->setSourceSample (inSample);
  for (int32 i = 0; i < mNumberOfModelApexes; i++)
    modelApexes[i].setSampleFromApexes(reflection);
  return sinkApex->setSampleFromApexes ();
}

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
