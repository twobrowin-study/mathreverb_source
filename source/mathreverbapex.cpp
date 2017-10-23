#include "mathreverbapex.h"

#include <algorithm>

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
  if (key != kNoBuffer)
  {
    // Инициализация буфера
    size_t bufferSize = (size_t) (sampleRate * sizeof (Sample64) + 0.5);
    mBuffer = (Sample64*)std::malloc (bufferSize); // максимум задержки - 1 секунда
    memset (mBuffer, 0, bufferSize);
  }

  if (key != kNoDelay)
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
Sample64 MathReverbApex::getSample (int32 delay)
{
  int32 pos = mBufferPos - delay;
  if (pos < 0)
    pos += mBufferLen;

  if (pos >= 0)
    return mBuffer[pos];

  return 0.f;
}

//------------------------------------------------------------------------
void MathReverbApex::setSample ()
{
  Sample64 sampleToPush = 0.f;
  // Пройдёмся по всем вершинам
  for (int32 i = 0; i < mDelayArrayLen; i++)
    if (mDelayArray[i].delayInSamples != -1) // Если задержка -1 - вершина совпадает с текущей
      sampleToPush += mDelayArray[i].apex -> getSample (mDelayArray[i].delayInSamples);

  mBuffer[mBufferPos] = sampleToPush;
  mBufferPos++;
  if (mBufferPos == mBufferLen)
    mBufferPos = 0;
}

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
