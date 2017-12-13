#pragma once
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "mathreverbapex.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbGraph: Decloration
// Plugin math model
// Reverberation hall
//------------------------------------------------------------------------
class MathReverbGraph
{
public:
  // Constructor
  MathReverbGraph (SampleRate sampleRate);
  // Destructor
  ~MathReverbGraph ();

  // Another sample process
  Sample64 process (Sample64 inSample, float reflection);

  // Settion model params
  void setDementoinParams (float length, float width, float height, float xPos, float yPos, float zPos);

private:
    // Graph apexes
    MathReverbApex *sourceApex;
    MathReverbApex *modelApexes;
    MathReverbApex *sinkApex;

    int32 mNumberOfModelApexes;
    SampleRate mSampleRate;
};

//------------------------------------------------------------------------
} // Vst
} // Steinberg
