#include "mathreverbgraph.h"

#include <algorithm>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbGraph: Реализация
//------------------------------------------------------------------------

//------------------------------------------------------------------------
MathReverbGraph::MathReverbGraph (int32 nChannels, SampleRate sampleRate)
: numChannels (nChannels)
{
  // Инициализация буфера
  // Выделение памяти под указатели на буферы буфера
  mBuffer = (float**)std::malloc (numChannels * sizeof (float*));

  // Получение размера одного блока канала
  size_t size = (size_t)(sampleRate * sizeof (float) + 0.5);

  // Разметка буфера по каналлам
  for (int32 channel = 0; channel < numChannels; channel ++)
  {
    mBuffer[channel] = (float*)std::malloc (size); // максимум задержки - 1 секунда
    memset (mBuffer[channel], 0, size);
  }
  mBufferPos = 0;
}

//------------------------------------------------------------------------
MathReverbGraph::~MathReverbGraph ()
{
  // Очистка буфера
  if (mBuffer)
  {
    for (int32 channel = 0; channel < numChannels; channel++)
      std::free (mBuffer[channel]);
    std::free (mBuffer);
    mBuffer = 0;
  }
}

//------------------------------------------------------------------------
Sample64 MathReverbGraph::process (Sample64 inSample)
{
  return inSample;
  // int32 tempBufferPos = mBufferPos;
  // // Поменяем местами входной и выходной семплы через буфер
  // tmp = ptrIn[sample] * fGain;
  // ptrOut[sample] = mBuffer[channel][tempBufferPos];
  // mBuffer[channel][tempBufferPos] = tmp;
  // tempBufferPos++;
  // // Если дошли до максимума обработки по длине - идём в начало буфера
  // if (tempBufferPos >= delayInSamples)
  // 	tempBufferPos = 0;


  // // Расчёт позиции в буфере после обработки на данный момент
  // mBufferPos += sampleFrames;
  // while (delayInSamples && mBufferPos >= delayInSamples)
  // 	mBufferPos -= delayInSamples;
}

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
