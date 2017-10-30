#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"

#include "base/source/fstring.h"

#include <stdio.h>

#include "size.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// CoordinateParameter Деклорация
// Описание параметров координат, длины, ширины, высоты
//------------------------------------------------------------------------
class CoordinateParameter : public Parameter
{
public:
  // Конструктор
  CoordinateParameter (int32 flags, int32 id, const char* name, SizeParameter *limitGiver);

  // Преобразование на вывод
  virtual void toString (ParamValue normValue, String128 string) const;
  //  Преобразование после ввода
  virtual bool fromString (const TChar* string, ParamValue& normValue) const;
  // Установка параметра
  virtual bool setNormalized (ParamValue v);

  // Обновление ограничения на максимальное значение
  void updateLimit ();

private:
  SizeParameter *mLimitGiver;
  double fLimit;
};

//------------------------------------------------------------------------
// CoordinateParameter Реализация
//------------------------------------------------------------------------
CoordinateParameter::CoordinateParameter (int32 flags, int32 id, const char* name, SizeParameter *limitGiver)
: mLimitGiver (limitGiver)
, fLimit (49.f)
{
  // Установка информации для хоста
  UString (info.title, USTRINGSIZE (info.title)).assign (USTRING (name));
  UString (info.units, USTRINGSIZE (info.units)).assign (USTRING ("m"));

  // Установка флагов (описывают принципы взаимодествия для ввода)
  info.flags = flags;
  info.id = id;
  info.stepCount = 0;
  info.defaultNormalizedValue = 0.5f;
  info.unitId = kRootUnitId;

  // Установка начального значения
  setNormalized (0.5f);
}

//------------------------------------------------------------------------
void CoordinateParameter::toString (ParamValue normValue, String128 string) const
{
  char text [32];
  normValue -= 0.5f;
  sprintf (text, "%.2f", (float) normValue * 100);
  UString (string, 128).fromAscii (text);
}

//------------------------------------------------------------------------
bool CoordinateParameter::fromString (const TChar* string, ParamValue& normValue) const
{
  String wrapper ((TChar*) string);
  double tmp = 0;
  if (wrapper.scanFloat (tmp))
  {
    if (tmp > fLimit)
      tmp = fLimit;
    if (tmp < -fLimit)
      tmp = -fLimit;

    normValue = tmp / 100.f + 0.5f;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------
bool CoordinateParameter::setNormalized (ParamValue v)
{
  if ( v * 50.f >= fLimit )
    return Parameter::setNormalized ((50.f + fLimit) / 100.f);
  if ( (v - 1.f) * 50.f <= - fLimit )
    return Parameter::setNormalized ((50.f - fLimit) / 100.f);
  return Parameter::setNormalized (v);
}


//------------------------------------------------------------------------
void CoordinateParameter::updateLimit ()
{
  fLimit = mLimitGiver->getNormalized () * 49.f;
  if ( getNormalized () * 50.f >= fLimit )
    Parameter::setNormalized ((50.f + fLimit) / 100.f);
  if ( (getNormalized () - 1.f) * 50.f <= - fLimit )
    Parameter::setNormalized ((50.f - fLimit) / 100.f);
}

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
