#include "mathreverbgraph.h"

#include <cmath>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbGraph: Implementation
//------------------------------------------------------------------------

//------------------------------------------------------------------------
MathReverbGraph::MathReverbGraph (SampleRate sampleRate)
: mNumberOfModelApexes (6)
, mSampleRate (sampleRate)
{
  // Creation of source
  sourceApex = new MathReverbApex (mSampleRate, kNoDelay);

  // Other apexes
  modelApexes = (MathReverbApex*)std::malloc ((size_t) (mNumberOfModelApexes * sizeof (MathReverbApex)));

  // Creation of sink - reciever
  DelayPoint sinkApexDelay [6] = {
    DelayPoint (modelApexes + 0, 0.05f * mSampleRate),
    DelayPoint (modelApexes + 1, 0.05f * mSampleRate),
    DelayPoint (modelApexes + 2, 0.05f * mSampleRate),
    DelayPoint (modelApexes + 3, 0.05f * mSampleRate),
    DelayPoint (modelApexes + 4, 0.05f * mSampleRate),
    DelayPoint (modelApexes + 5, 0.05f * mSampleRate)
  };
  sinkApex = new MathReverbApex (sinkApexDelay, mNumberOfModelApexes, 0.8f, kNoBuffer);

  // Creation of other apexes
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
  float widthModel = width
      , heightModel = height
      , lengthModel = length
      , xModel = xPos
      , yModel = yPos
      , zModel = zPos;

  // Demention params limits
  if (widthModel < 0.1f)
    widthModel = 0.1f;
  if (heightModel < 0.1f)
    heightModel = 0.1f;
  if (lengthModel < 0.1f)
    lengthModel = 0.1f;

  float widthModelHalf = 0.5f * widthModel
      , heightModelHalf = 0.5f * heightModel
      , lengthModelHalf = 0.5f * lengthModel;

  // Coordinate limits
  if (xModel > widthModelHalf - 0.02f)
    xModel = widthModelHalf - 0.02f;
  if (yModel > lengthModelHalf - 0.02f)
    yModel = lengthModelHalf - 0.02f;
  if (zModel > heightModelHalf - 0.02f)
    zModel = heightModelHalf - 0.02f;
  if (xModel < -widthModelHalf + 0.02f)
    xModel = -widthModelHalf + 0.02f;
  if (yModel < -lengthModelHalf + 0.02f)
    yModel = -lengthModelHalf + 0.02f;
  if (zModel < -heightModelHalf + 0.02f)
    zModel = -heightModelHalf + 0.02f;

  // Creation of new sink delays
  DelayPoint sinkApexDelay [6] = {
    DelayPoint (modelApexes + 0, 0.1f * (widthModelHalf + xModel) * mSampleRate),
    DelayPoint (modelApexes + 1, 0.1f * (heightModelHalf + zModel) * mSampleRate),
    DelayPoint (modelApexes + 2, 0.1f * (lengthModelHalf + yModel) * mSampleRate),
    DelayPoint (modelApexes + 3, 0.1f * (widthModelHalf - xModel) * mSampleRate),
    DelayPoint (modelApexes + 4, 0.1f * (heightModelHalf - zModel) * mSampleRate),
    DelayPoint (modelApexes + 5, 0.1f * (lengthModelHalf - yModel) * mSampleRate)
  };

  // Set delays to sink
  sinkApex->setDelayArray (sinkApexDelay, mNumberOfModelApexes);

  // Creation of new other apexes delays
  DelayPoint modelApexesDelay [6][6] = {
    {
      DelayPoint (sourceApex, 0.1f * (widthModelHalf + xModel) * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * sqrt (pow (widthModelHalf, 2) + pow (heightModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * sqrt (pow (widthModelHalf, 2) + pow (lengthModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * widthModel * mSampleRate),
      DelayPoint (modelApexes + 4, 0.1f * sqrt (pow (widthModelHalf, 2) + pow (heightModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 5, 0.1f * sqrt (pow (widthModelHalf, 2) + pow (lengthModelHalf, 2)) * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.1f * (heightModelHalf + zModel) * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * sqrt (pow (widthModelHalf, 2) + pow (heightModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * sqrt (pow (heightModelHalf, 2) + pow (lengthModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * sqrt (pow (widthModelHalf, 2) + pow (heightModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 4, 0.1f * heightModel * mSampleRate),
      DelayPoint (modelApexes + 5, 0.1f * sqrt (pow (heightModelHalf, 2) + pow (lengthModelHalf, 2)) * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.1f * (lengthModelHalf + yModel) * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * sqrt (pow (widthModelHalf, 2) + pow (lengthModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * sqrt (pow (heightModelHalf, 2) + pow (lengthModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * sqrt (pow (widthModelHalf, 2) + pow (lengthModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 4, 0.1f * sqrt (pow (heightModelHalf, 2) + pow (lengthModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 5, 0.1f * lengthModel * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.1f * (widthModelHalf - xModel) * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * width * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * sqrt (pow (widthModelHalf, 2) + pow (heightModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * sqrt (pow (widthModelHalf, 2) + pow (lengthModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 4, 0.1f * sqrt (pow (widthModelHalf, 2) + pow (heightModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 5, 0.1f * sqrt (pow (widthModelHalf, 2) + pow (lengthModelHalf, 2)) * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.1f * (heightModelHalf - zModel) * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * sqrt (pow (widthModelHalf, 2) + pow (heightModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * heightModel * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * sqrt (pow (heightModelHalf, 2) + pow (lengthModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * sqrt (pow (widthModelHalf, 2) + pow (heightModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 5, 0.1f * sqrt (pow (heightModelHalf, 2) + pow (lengthModelHalf, 2)) * mSampleRate)
    },
    {
      DelayPoint (sourceApex, 0.1f * (lengthModelHalf - yModel) * mSampleRate),
      DelayPoint (modelApexes + 0, 0.1f * sqrt (pow (widthModelHalf, 2) + pow (lengthModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 1, 0.1f * sqrt (pow (heightModelHalf, 2) + pow (lengthModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 2, 0.1f * lengthModel * mSampleRate),
      DelayPoint (modelApexes + 3, 0.1f * sqrt (pow (widthModelHalf, 2) + pow (lengthModelHalf, 2)) * mSampleRate),
      DelayPoint (modelApexes + 4, 0.1f * sqrt (pow (heightModelHalf, 2) + pow (lengthModelHalf, 2)) * mSampleRate)
    },
  };

  // Other apexes delays settin
  for (int32 i = 0; i < mNumberOfModelApexes; i++)
    modelApexes[i].setDelayArray (modelApexesDelay[i], mNumberOfModelApexes);
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
