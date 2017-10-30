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
, fLimit (0.49f)
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
  normValue -= 0.5f; // Установка значения в соответсвие с моделью
  sprintf (text, "%.2f", (float) normValue * 100);
  UString (string, 128).fromAscii (text);
}

//------------------------------------------------------------------------
bool CoordinateParameter::fromString (const TChar* string, ParamValue& normValue) const
{
  // Создание дополнительных переменных
  String wrapper ((TChar*) string);
  double tmp = 0;
  if (wrapper.scanFloat (tmp))
  {
    // Ограничение значений
    if (tmp > fLimit * 100.f)
      normValue = 0.5f + fLimit;
    else
      if (tmp < -fLimit * 100.f)
        normValue = 0.5f - fLimit;
      else
        // Установка значения
        normValue = tmp / 100.f + 0.5f;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------
bool CoordinateParameter::setNormalized (ParamValue v)
{
  ParamValue toSet = v;
  // Ограничение значений
  if ( toSet > 0.5f + fLimit )
    toSet = 0.5f + fLimit;
  if ( toSet < 0.5f - fLimit )
    toSet = 0.5f - fLimit;
  // Установка значения
  return Parameter::setNormalized (toSet);
}


//------------------------------------------------------------------------
void CoordinateParameter::updateLimit ()
{
  // Обносление ограничения
  fLimit = mLimitGiver->getNormalized () * 0.49f;
  // Обновление значения
  setNormalized (getNormalized ());
}

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
