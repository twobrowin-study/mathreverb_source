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

enum ApexType // Вспомогательные определения
{
    kNoDelay // У вершины нет линий задержек - исток
  , kNoBuffer // У вершины нет буфера - сток
  , kNormalApex // Обычная вершина
};

//------------------------------------------------------------------------
// MathReverbApex: Декларация
// Математическая модель плагина
// Вершина графа
//------------------------------------------------------------------------
class MathReverbApex
{
public:
  // Конструктор
  MathReverbApex (SampleRate sampleRate, DelayPoint* delayArray, int32 numberOfApexes, ApexType key = kNormalApex);
  MathReverbApex (SampleRate sampleRate, ApexType key = kNoDelay);
  MathReverbApex (DelayPoint* delayArray, int32 numberOfApexes, ApexType key = kNoBuffer);

  // Деструктор
  ~MathReverbApex ();

  // Метод возвращает очередной семпл из буфера
  Sample64 getSampleWithDelay (int32 delayInSamples);

  // Метод устанавливает очередной семпл в буфер
  Sample64 setSampleFromApexes ();

  // Метод устанавливает семпл в буфер источника
  Sample64 setSourceSample (Sample64 sourceSample);

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
