#include "mathreverbapex.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbApex: Implementation
//------------------------------------------------------------------------

//------------------------------------------------------------------------
MathReverbApex::MathReverbApex (SampleRate sampleRate, DelayPoint* delayArray, int32 numberOfApexes, float defaultReflection, ApexType key)
: mBufferLen (sampleRate)
, mBufferPos (0)
, mDelayArrayLen (numberOfApexes)
, mDefaultReflection (defaultReflection)
{
  if (key == kNormalApex)
  {
    // Buffer Init
    bufferInit (sampleRate);

    // Adding delays
    delaysInit (delayArray, numberOfApexes);
  }
}

//------------------------------------------------------------------------
MathReverbApex::MathReverbApex (SampleRate sampleRate, ApexType key)
: mBufferLen (sampleRate)
, mBufferPos (0)
, mDelayArrayLen (0)
, mDelayArray (0)
, mDefaultReflection (0)
{
  if (key == kNoDelay)
    // Buffer Init
    bufferInit (sampleRate);
}

//------------------------------------------------------------------------
MathReverbApex::MathReverbApex (DelayPoint* delayArray, int32 numberOfApexes, float defaultReflection, ApexType key)
: mBuffer (0)
, mBufferLen (0)
, mBufferPos (0)
, mDelayArrayLen (numberOfApexes)
, mDefaultReflection (defaultReflection)
{
  if (key == kNoBuffer)
    // Adding delays
    delaysInit (delayArray, numberOfApexes);
}

//------------------------------------------------------------------------
MathReverbApex::~MathReverbApex ()
{
  // Buffer clear
  if (mBuffer)
  {
    std::free (mBuffer);
    mBuffer = 0;
  }

  // Delays clear
  if (mDelayArray)
  {
    std::free (mDelayArray);
    mDelayArray = 0;
  }
}

//------------------------------------------------------------------------
Sample64 MathReverbApex::getSampleWithDelay (int32 delay)
{
  if (mBuffer)
  {
    int32 pos = mBufferPos - delay;
    if (pos < 0)
      pos += mBufferLen;

    if (pos >= 0)
      return mBuffer[pos];
  }

  return 0.f;
}

//------------------------------------------------------------------------
Sample64 MathReverbApex::setSampleFromApexes (float reflection)
{
  Sample64 sampleToPush = 0.f;
  if (mDelayArray)
  {
    // For all ather apexes
    for (int32 i = 0; i < mDelayArrayLen; i++)
      // Add delayed sample
      sampleToPush += mDefaultReflection * reflection * mDelayArray[i].apex -> getSampleWithDelay (mDelayArray[i].delayInSamples);
  }

  if (mBuffer)
    setSourceSample (sampleToPush);

  return sampleToPush;
}

//------------------------------------------------------------------------
Sample64 MathReverbApex::setSourceSample (Sample64 sourceSample)
{
  if (mBuffer)
  {
    mBuffer[mBufferPos] = sourceSample;
    mBufferPos++;
    if (mBufferPos == mBufferLen)
      mBufferPos = 0;
  }

  return sourceSample;
}

//------------------------------------------------------------------------
void MathReverbApex::setDelayArray (DelayPoint* delayArray, int32 numberOfApexes)
{
  // Change of delays
  for (int32 i = 0; i < numberOfApexes; i++)
    mDelayArray[i] = delayArray[i];
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
