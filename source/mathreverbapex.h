#pragma once
#include "pluginterfaces/vst/ivstparameterchanges.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// Стурктура передачи указателей на объеты класса
// -----------------------------------------------------------------------
class MathReverbApex;
struct DelayPoint
{
  DelayPoint () : apex (0), delayInSamples (0) {}
  MathReverbApex* apex;
  int32 delayInSamples;
};


//------------------------------------------------------------------------
// MathReverbApex: Декларация
// Математическая модель плагина
// Вершина графа
//------------------------------------------------------------------------
class MathReverbApex
{
public:
  enum // Вспомогательные определения
  {
      kNoDelay // У вершины нет линий задержек - исток
    , kNoBuffer // У вершины нет буфера - сток
  };

  // Конструктор
  MathReverbApex (SampleRate sampleRate, DelayPoint* delayArray, int32 numberOfApexes);
  // Деструктор
  ~MathReverbApex ();

  // Метод возвращает очередной семпл из буфера
  Sample64 getSample (int32 delayInSamples);

  // Метод устанавливает очередной семпл в буфер
  void setSample ();

private:
    // Буфер
  	Sample64* mBuffer;
    int32 mBufferPos;
    int32 mBufferLen;

    // Прочие вершины с задержками
  	DelayPoint* mDelayArray;
    int32 mDelayArrayLen;
};

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
