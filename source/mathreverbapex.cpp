#include "mathreverbapex.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbApex: Реализация
//------------------------------------------------------------------------

//------------------------------------------------------------------------
MathReverbApex::MathReverbApex (SampleRate sampleRate, DelayPoint* delayArray, int32 numberOfApexes, ApexType key)
: mBufferLen (sampleRate)
, mBufferPos (0)
, mDelayArrayLen (numberOfApexes)
{
  if (key == kNormalApex)
  {
    // Инициализация буфера
    size_t bufferSize = (size_t) (sampleRate * sizeof (Sample64) + 0.5);
    mBuffer = (Sample64*)std::malloc (bufferSize); // максимум задержки - 1 секунда
    memset (mBuffer, 0, bufferSize);

    // Передача параметров задержки
    mDelayArray = (DelayPoint*)std::malloc ((size_t) (numberOfApexes * sizeof (DelayPoint)));
    for (int32 i = 0; i < numberOfApexes; i++)
      mDelayArray[i] = delayArray[i];
  }
}

//------------------------------------------------------------------------
MathReverbApex::MathReverbApex (SampleRate sampleRate, ApexType key)
: mBufferLen (sampleRate)
, mBufferPos (0)
, mDelayArrayLen (0)
, mDelayArray (0)
{
  if (key == kNoDelay)
  {
    // Инициализация буфера
    size_t bufferSize = (size_t) (sampleRate * sizeof (Sample64) + 0.5);
    mBuffer = (Sample64*)std::malloc (bufferSize); // максимум задержки - 1 секунда
    memset (mBuffer, 0, bufferSize);
  }
}

//------------------------------------------------------------------------
MathReverbApex::MathReverbApex (DelayPoint* delayArray, int32 numberOfApexes, ApexType key)
: mBuffer (0)
, mBufferLen (0)
, mBufferPos (0)
, mDelayArrayLen (numberOfApexes)
{
  if (key == kNoBuffer)
  {
    // Передача параметров задержки
    mDelayArray = (DelayPoint*)std::malloc ((size_t) (numberOfApexes * sizeof (DelayPoint)));
    for (int32 i = 0; i < numberOfApexes; i++)
      mDelayArray[i] = delayArray[i];
  }
}

//------------------------------------------------------------------------
MathReverbApex::~MathReverbApex ()
{
  // Очистка буфера
  if (mBuffer)
  {
    std::free (mBuffer);
    mBuffer = 0;
  }

  // Очистка задержки
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
Sample64 MathReverbApex::setSampleFromApexes ()
{
  Sample64 sampleToPush = 0.f;
  if (mDelayArray)
  {
    // Пройдёмся по всем вершинам
    for (int32 i = 0; i < mDelayArrayLen; i++)
      // Добавим приглушенное отражение
      sampleToPush += 0.01 * mDelayArray[i].apex -> getSampleWithDelay (mDelayArray[i].delayInSamples);
  }

  if (mBuffer)
  {
    mBuffer[mBufferPos] = sampleToPush;
    mBufferPos++;
    if (mBufferPos == mBufferLen)
      mBufferPos = 0;
  }

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
} // Пространство имён Vst
} // Пространство имён Steinberg
