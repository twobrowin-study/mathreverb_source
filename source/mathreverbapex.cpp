#include "mathreverbapex.h"

#include <algorithm>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbApex: Реализация
//------------------------------------------------------------------------

//------------------------------------------------------------------------
MathReverbApex::MathReverbApex (SampleRate sampleRate, delayPoint* delayArray, int32 numberOfApexes)
: mBuferLen (sampleRate)
, mDelayArrayLen (numberOfApexes)
{
  // Инициализация буфера
  // Получение размера
  size_t bufferSize = (size_t)(sampleRate * sizeof (float) + 0.5); // максимум задержки - 1 секунда

  // Разметка буфера
  mBuffer = (float*)std::malloc (bufferSize);
  memset (mBuffer, 0, bufferSize);
  mBuferPos = 0;

  // Передача параметров задержки
  // Получение размера
  size_t delayArraySize = (size_t)(numberOfApexes * sizeof (delayPoint));

  // Разметка буфера по каналлам
  mDelayArray = (float*)std::malloc (delayArraySize);
  for (int32 i = 0; i < numberOfApexes; i++)
    mDelayArray[i] = delayArray[i];
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
  int32 pos = mBuferPos - delay;
  if (pos < 0)
    pos += mBuferLen;

  if (pos >= 0)
    return mBuferPos[pos];

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

  mBufer[mBuferPos] = sampleToPush;
  mBuferPos++;
  if (mBuferPos == mBuferLen)
    mBuferPos = 0;
}

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
