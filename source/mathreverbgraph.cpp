#include "mathreverbgraph.h"

#include <cmath>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbGraph: Реализация
//------------------------------------------------------------------------

//------------------------------------------------------------------------
MathReverbGraph::MathReverbGraph (SampleRate sampleRate)
: mNumberOfModelApexes (4)
, mSampleRate (sampleRate)
{
  // Создадим исток - источник
  sourceApex = new MathReverbApex (mSampleRate, kNoDelay);

  // Разметим прочие вершины
  modelApexes = (MathReverbApex*)std::malloc ((size_t) (mNumberOfModelApexes * sizeof (MathReverbApex)));

  // Создадим сток - приёмник
  DelayPoint sinkApexDelay [4] = {
    DelayPoint (modelApexes + 0, 0.05f * mSampleRate),
    DelayPoint (modelApexes + 1, 0.05f * mSampleRate),
    DelayPoint (modelApexes + 2, 0.05f * mSampleRate),
    DelayPoint (modelApexes + 3, 0.05f * mSampleRate)
  };
  sinkApex = new MathReverbApex (sinkApexDelay, mNumberOfModelApexes, kNoBuffer);

  // Создадим прочие вершины
  DelayPoint modelApexesDelay [4][4] = {
    {
      DelayPoint (sourceApex, 0.05f * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.05f * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * mSampleRate + 1),
      DelayPoint (modelApexes + 2, 0.1f * mSampleRate + 1),
      DelayPoint (modelApexes + 3, 0.1f * mSampleRate + 1)
    },
    {
      DelayPoint (sourceApex, 0.05f * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.05f * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * mSampleRate + 1),
      DelayPoint (modelApexes + 1, 0.1f * mSampleRate + 1),
      DelayPoint (modelApexes + 2, 0.1f * mSampleRate + 1)
    }
  };
  for (int32 i = 0; i < mNumberOfModelApexes; i++)
    std::memcpy ( modelApexes + i
                , new MathReverbApex (mSampleRate, modelApexesDelay[i], mNumberOfModelApexes, kNormalApex)
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
    modelApexes[i].setSampleFromApexes (reflection);
  return sinkApex->setSampleFromApexes ();
}

void MathReverbGraph::setDementoinParams (float length, float width, float height, float xPos, float yPos, float zPos)
{
  // Создадим новые задержки для стока
  float lengthABSCoord = 0.5f * length
      , widthABSCoord = 0.5f * width
      , heightABSCoord = 0.5f * height;

  DelayPoint sinkApexDelay [4] = {
    DelayPoint (modelApexes + 0, 0.1f * sqrt (pow (-widthABSCoord - xPos, 2) + pow (-heightABSCoord - yPos, 2) + pow (-lengthABSCoord - zPos, 2)) * 0.2778f * mSampleRate),
    DelayPoint (modelApexes + 1, 0.1f * sqrt (pow (-widthABSCoord - xPos, 2) + pow (-heightABSCoord - yPos, 2) + pow (+lengthABSCoord - zPos, 2)) * 0.2778f * mSampleRate),
    DelayPoint (modelApexes + 2, 0.1f * sqrt (pow (-widthABSCoord - xPos, 2) + pow (+heightABSCoord - yPos, 2) + pow (-lengthABSCoord - zPos, 2)) * 0.2778f * mSampleRate),
    DelayPoint (modelApexes + 3, 0.1f * sqrt (pow (-widthABSCoord - xPos, 2) + pow (+heightABSCoord - yPos, 2) + pow (+lengthABSCoord - zPos, 2)) * 0.2778f * mSampleRate)
  };

  // Зададим стоку задержки
  sinkApex->setDelayArray (sinkApexDelay, mNumberOfModelApexes);

  // Создадим новые задержки для прочих вершин
  DelayPoint modelApexesDelay [4][4] = {
    {
      DelayPoint (sourceApex, 0.1f * sqrt (pow (-widthABSCoord - xPos, 2) + pow (-heightABSCoord - yPos, 2) + pow (-lengthABSCoord - zPos, 2)) * 0.2778f * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * length * 0.2778f * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * height * 0.2778f * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * sqrt (pow (height, 2) + pow (length, 2)) * 0.2778f * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.1f * sqrt (pow (-widthABSCoord - xPos, 2) + pow (-heightABSCoord - yPos, 2) + pow (+lengthABSCoord - zPos, 2)) * 0.2778f * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * length * 0.2778f * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * sqrt (pow (height, 2) + pow (length, 2)) * 0.2778f * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * height * 0.2778f * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.1f * sqrt (pow (-widthABSCoord - xPos, 2) + pow (+heightABSCoord - yPos, 2) + pow (-lengthABSCoord - zPos, 2)) * 0.2778f * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * height * 0.2778f * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * sqrt (pow (height, 2) + pow (length, 2)) * 0.2778f * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * length * 0.2778f * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.1f * sqrt (pow (-widthABSCoord - xPos, 2) + pow (+heightABSCoord - yPos, 2) + pow (+lengthABSCoord - zPos, 2)) * 0.2778f * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * sqrt (pow (height, 2) + pow (length, 2)) * 0.2778f * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * height * 0.2778f * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * length * 0.2778f * mSampleRate)
    }
  };

  // Зададим задержки прочим вершинам
  for (int32 i = 0; i < mNumberOfModelApexes; i++)
    modelApexes[i].setDelayArray (modelApexesDelay[i], mNumberOfModelApexes);
}

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
