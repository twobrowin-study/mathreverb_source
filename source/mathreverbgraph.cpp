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
      DelayPoint (modelApexes + 0, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.05f * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.05f * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * mSampleRate)
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
  float lengthHalf = 0.5f * length
      , widthHalf = 0.5f * width
      , heightHalf = 0.5f * height;

  DelayPoint sinkApexDelay [4] = {
    DelayPoint (modelApexes + 0, 0.1f * lengthHalf * mSampleRate),
    DelayPoint (modelApexes + 1, 0.1f * widthHalf * mSampleRate),
    DelayPoint (modelApexes + 2, 0.1f * lengthHalf * mSampleRate),
    DelayPoint (modelApexes + 3, 0.1f * widthHalf * mSampleRate)
  };

  // Зададим стоку задержки
  sinkApex->setDelayArray (sinkApexDelay, mNumberOfModelApexes);

  // // Создадим новые задержки для прочих вершин
  // DelayPoint modelApexesDelay [4][4] = {
  //   {
  //     DelayPoint (sourceApex, 0.1f * sqrt (pow (-widthHalf - xPos, 2) + pow (-heightHalf - yPos, 2) + pow (-lengthHalf - zPos, 2)) * mSampleRate),
  //     DelayPoint (modelApexes + 1, 0.1f * length * mSampleRate),
  //     DelayPoint (modelApexes + 2, 0.1f * height * mSampleRate),
  //     DelayPoint (modelApexes + 3, 0.1f * sqrt (pow (height, 2) + pow (length, 2)) * mSampleRate)
  //   },
  //   {
  //     DelayPoint (sourceApex, 0.1f * sqrt (pow (-widthHalf - xPos, 2) + pow (-heightHalf - yPos, 2) + pow (+lengthHalf - zPos, 2)) * mSampleRate),
  //     DelayPoint (modelApexes + 0, 0.1f * length * mSampleRate),
  //     DelayPoint (modelApexes + 2, 0.1f * sqrt (pow (height, 2) + pow (length, 2)) * mSampleRate),
  //     DelayPoint (modelApexes + 3, 0.1f * height * mSampleRate)
  //   },
  //   {
  //     DelayPoint (sourceApex, 0.1f * sqrt (pow (-widthHalf - xPos, 2) + pow (+heightHalf - yPos, 2) + pow (-lengthHalf - zPos, 2)) * mSampleRate),
  //     DelayPoint (modelApexes + 0, 0.1f * height * mSampleRate),
  //     DelayPoint (modelApexes + 1, 0.1f * sqrt (pow (height, 2) + pow (length, 2)) * mSampleRate),
  //     DelayPoint (modelApexes + 3, 0.1f * length * mSampleRate)
  //   },
  //   {
  //     DelayPoint (sourceApex, 0.1f * sqrt (pow (-widthHalf - xPos, 2) + pow (+heightHalf - yPos, 2) + pow (+lengthHalf - zPos, 2)) * mSampleRate),
  //     DelayPoint (modelApexes + 0, 0.1f * sqrt (pow (height, 2) + pow (length, 2)) * mSampleRate),
  //     DelayPoint (modelApexes + 1, 0.1f * height * mSampleRate),
  //     DelayPoint (modelApexes + 2, 0.1f * length * mSampleRate)
  //   }
  // };
  //
  // // Зададим задержки прочим вершинам
  // for (int32 i = 0; i < mNumberOfModelApexes; i++)
  //   modelApexes[i].setDelayArray (modelApexesDelay[i], mNumberOfModelApexes);
}

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
