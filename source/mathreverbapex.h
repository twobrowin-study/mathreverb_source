#pragma once
#include "pluginterfaces/vst/ivstparameterchanges.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// Helper structure
// -----------------------------------------------------------------------
class MathReverbApex;
struct DelayPoint
{
  DelayPoint () : apex (0), delayInSamples (0) {}
  DelayPoint (MathReverbApex* apx, int32 dly) : apex (apx), delayInSamples (dly) {}
  MathReverbApex* apex;
  int32 delayInSamples;
};

enum ApexType // Added numbers
{
    kNoDelay // Source
  , kNoBuffer // Sink
  , kNormalApex // Another Apex
};

//------------------------------------------------------------------------
// MathReverbApex: Decloration
//------------------------------------------------------------------------
class MathReverbApex
{
public:
  // Constructors
  MathReverbApex (SampleRate sampleRate, DelayPoint* delayArray, int32 numberOfApexes, float defaultReflection, ApexType key = kNormalApex);
  MathReverbApex (SampleRate sampleRate, ApexType key = kNoDelay);
  MathReverbApex (DelayPoint* delayArray, int32 numberOfApexes, float defaultReflection, ApexType key = kNoBuffer);

  // Destructor
  ~MathReverbApex ();

  // Get sample
  Sample64 getSampleWithDelay (int32 delayInSamples);

  // New sample process
  Sample64 setSampleFromApexes (float reflection = 1.f);

  // Set sample into buffer
  Sample64 setSourceSample (Sample64 sourceSample);

  // Adding new connections
  void setDelayArray (DelayPoint* delayArray, int32 numberOfApexes);

  // Settin default reflection
  void setDefaultReflection (float defaultReflection)
  {
    mDefaultReflection = defaultReflection;
  }

private:
  // Init methods
  void bufferInit (int sampleRate)
  {
    size_t bufferSize = (size_t) (sampleRate * sizeof (Sample64) + 0.5);
    mBuffer = (Sample64*)std::malloc (bufferSize); // 1 sec max
    memset (mBuffer, 0, bufferSize);
  }

  void delaysInit (DelayPoint* delayArray, int32 numberOfApexes)
  {
    mDelayArray = (DelayPoint*)std::malloc ((size_t) (numberOfApexes * sizeof (DelayPoint)));
    for (int32 i = 0; i < numberOfApexes; i++)
      mDelayArray[i] = delayArray[i];
  }

  // Buffer
	Sample64* mBuffer;
  int32 mBufferPos;
  int32 mBufferLen;

  // Another apexes
	DelayPoint* mDelayArray;
  int32 mDelayArrayLen;

  // Default reflection
  float mDefaultReflection;
};

//------------------------------------------------------------------------
} // Vst
} // Steinberg
