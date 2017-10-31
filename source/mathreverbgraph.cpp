#include "mathreverbgraph.h"

#include <cmath>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbGraph: Реализация
//------------------------------------------------------------------------

//------------------------------------------------------------------------
MathReverbGraph::MathReverbGraph (SampleRate sampleRate)
: mNumberOfModelApexes (6)
, mSampleRate (sampleRate)
{
  // Создадим исток - источник
  sourceApex = new MathReverbApex (mSampleRate, kNoDelay);

  // Разметим прочие вершины
  modelApexes = (MathReverbApex*)std::malloc ((size_t) (mNumberOfModelApexes * sizeof (MathReverbApex)));

  // Создадим сток - приёмник
  DelayPoint sinkApexDelay [6] = {
    DelayPoint (modelApexes + 0, 0.05f * mSampleRate),
    DelayPoint (modelApexes + 1, 0.05f * mSampleRate),
    DelayPoint (modelApexes + 2, 0.05f * mSampleRate),
    DelayPoint (modelApexes + 3, 0.05f * mSampleRate),
    DelayPoint (modelApexes + 4, 0.05f * mSampleRate),
    DelayPoint (modelApexes + 5, 0.05f * mSampleRate)
  };
  sinkApex = new MathReverbApex (sinkApexDelay, mNumberOfModelApexes, 0.8f, kNoBuffer);

  // Создадим прочие вершины
  DelayPoint modelApexesDelay [6][6] = {
    {
      DelayPoint (sourceApex, 0.05f * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 4, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 5, 0.1f * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.05f * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 4, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 5, 0.1f * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.05f * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 4, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 5, 0.1f * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.05f * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 4, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 5, 0.1f * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.05f * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 5, 0.1f * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.05f * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * mSampleRate),
      DelayPoint (modelApexes + 4, 0.1f * mSampleRate)
    }
  };
  for (int32 i = 0; i < mNumberOfModelApexes; i++)
    std::memcpy ( modelApexes + i
                , new MathReverbApex (mSampleRate, modelApexesDelay[i], mNumberOfModelApexes, 0.13f, kNormalApex)
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

void MathReverbGraph::setDementoinParams (float width, float height, float length, float xPos, float yPos, float zPos)
{
  float widthHalf = 0.5f * width
      , heightHalf = 0.5f * height
      , lengthHalf = 0.5f * length;
  float xCoord = xPos
      , yCoord = yPos
      , zCoord = zPos;

  // Проверка на ограничение координат
  if (xCoord > widthHalf - 0.1f)
    xCoord = widthHalf - 0.1f;
  if (yCoord > lengthHalf - 0.1f)
    yCoord = lengthHalf - 0.1f;
  if (zCoord > heightHalf - 0.1f)
    zCoord = heightHalf - 0.1f;
  if (xCoord < -widthHalf + 0.1f)
    xCoord = -widthHalf + 0.1f;
  if (yCoord < -lengthHalf + 0.1f)
    yCoord = -lengthHalf + 0.1f;
  if (zCoord < -heightHalf + 0.1f)
    zCoord = -heightHalf + 0.1f;

  // Создадим новые задержки для стока
  DelayPoint sinkApexDelay [6] = {
    DelayPoint (modelApexes + 0, 0.1f * (widthHalf + xCoord) * mSampleRate),
    DelayPoint (modelApexes + 1, 0.1f * (heightHalf + zCoord) * mSampleRate),
    DelayPoint (modelApexes + 2, 0.1f * (lengthHalf + yCoord) * mSampleRate),
    DelayPoint (modelApexes + 3, 0.1f * (widthHalf - xCoord) * mSampleRate),
    DelayPoint (modelApexes + 4, 0.1f * (heightHalf - zCoord) * mSampleRate),
    DelayPoint (modelApexes + 5, 0.1f * (lengthHalf - yCoord) * mSampleRate)
  };

  // Зададим стоку задержки
  sinkApex->setDelayArray (sinkApexDelay, mNumberOfModelApexes);

  // Создадим новые задержки для прочих вершин
  DelayPoint modelApexesDelay [6][6] = {
    {
      DelayPoint (sourceApex, 0.1f * (widthHalf + xCoord) * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * sqrt (pow (widthHalf, 2) + pow (heightHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * sqrt (pow (widthHalf, 2) + pow (lengthHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * width * mSampleRate),
      DelayPoint (modelApexes + 4, 0.1f * sqrt (pow (widthHalf, 2) + pow (heightHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 5, 0.1f * sqrt (pow (widthHalf, 2) + pow (lengthHalf, 2)) * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.1f * (heightHalf + zCoord) * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * sqrt (pow (widthHalf, 2) + pow (heightHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * sqrt (pow (heightHalf, 2) + pow (lengthHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * sqrt (pow (widthHalf, 2) + pow (heightHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 4, 0.1f * height * mSampleRate),
      DelayPoint (modelApexes + 5, 0.1f * sqrt (pow (heightHalf, 2) + pow (lengthHalf, 2)) * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.1f * (lengthHalf + yCoord) * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * sqrt (pow (widthHalf, 2) + pow (lengthHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * sqrt (pow (heightHalf, 2) + pow (lengthHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * sqrt (pow (widthHalf, 2) + pow (lengthHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 4, 0.1f * sqrt (pow (heightHalf, 2) + pow (lengthHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 5, 0.1f * length * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.1f * (widthHalf - xCoord) * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * width * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * sqrt (pow (widthHalf, 2) + pow (heightHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * sqrt (pow (widthHalf, 2) + pow (lengthHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 4, 0.1f * sqrt (pow (widthHalf, 2) + pow (heightHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 5, 0.1f * sqrt (pow (widthHalf, 2) + pow (lengthHalf, 2)) * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.1f * (heightHalf - zCoord) * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * sqrt (pow (widthHalf, 2) + pow (heightHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * height * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * sqrt (pow (heightHalf, 2) + pow (lengthHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * sqrt (pow (widthHalf, 2) + pow (heightHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 5, 0.1f * sqrt (pow (heightHalf, 2) + pow (lengthHalf, 2)) * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.1f * (lengthHalf - yCoord) * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * sqrt (pow (widthHalf, 2) + pow (lengthHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * sqrt (pow (heightHalf, 2) + pow (lengthHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * length * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * sqrt (pow (widthHalf, 2) + pow (lengthHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 4, 0.1f * sqrt (pow (heightHalf, 2) + pow (lengthHalf, 2)) * mSampleRate)
    },
  };

  // Зададим задержки прочим вершинам
  for (int32 i = 0; i < mNumberOfModelApexes; i++)
    modelApexes[i].setDelayArray (modelApexesDelay[i], mNumberOfModelApexes);
}

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
