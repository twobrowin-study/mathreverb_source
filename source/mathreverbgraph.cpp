#include "mathreverbgraph.h"

#include <algorithm>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbGraph: Реализация
//------------------------------------------------------------------------

//------------------------------------------------------------------------
MathReverbGraph::MathReverbGraph ()
{

}

//------------------------------------------------------------------------
MathReverbGraph::~MathReverbGraph ()
{

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
