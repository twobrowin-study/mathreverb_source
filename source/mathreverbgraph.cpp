#include "mathreverbgraph.h"

#include <algorithm>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MathReverbGraph: Реализация
//------------------------------------------------------------------------

//------------------------------------------------------------------------
MathReverbGraph::MathReverbGraph (SampleRate samRate)
: sampleRate (samRate)
{
  // Инициализация буфера
  // Выделение памяти под указатели на буферы буфера
  mBuffer = (float**)std::malloc (sizeof (float*));

  // Получение размера одного блока канала
  size_t size = (size_t)(sampleRate * sizeof (float) + 0.5);

  // Разметка буфера по каналлам
    mBuffer[0] = (float*)std::malloc (size); // максимум задержки - 1 секунда
    memset (mBuffer[0], 0, size);
  mBufferPos = 0;
}

//------------------------------------------------------------------------
MathReverbGraph::~MathReverbGraph ()
{
  // Очистка буфера
  if (mBuffer)
  {
    std::free (mBuffer[0]);
    std::free (mBuffer);
    mBuffer = 0;
  }
}

//------------------------------------------------------------------------
Sample64 MathReverbGraph::process (Sample64 inSample)
{
  return inSample;
  // // Длина задержки - количество задерживаемых семплов, минимум - 1
	// int32 delayInSamples = std::max<int32> (1, 0.f * sampleRate);
  // int32 tempBufferPos = mBufferPos;
  // // Поменяем местами входной и выходной семплы через буфер
  // tmp = ptrIn[sample] * fGain;
  // ptrOut[sample] = mBuffer[0][tempBufferPos];
  // mBuffer[0][tempBufferPos] = tmp;
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
