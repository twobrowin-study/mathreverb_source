#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"

#include "base/source/fstring.h"

#include <stdio.h>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// SizeParameter Деклорация
// Описание параметров координат, длины, ширины, высоты
//------------------------------------------------------------------------
class SizeParameter : public Parameter
{
public:
  // Конструктор
  SizeParameter (int32 flags, int32 id, const char* name);

  // Преобразование на вывод
  virtual void toString (ParamValue normValue, String128 string) const;
  //  Преобразование после ввода
  virtual bool fromString (const TChar* string, ParamValue& normValue) const;
  // Установка параметра
  virtual bool setNormalized (ParamValue v);
};

//------------------------------------------------------------------------
// SizeParameter Реализация
//------------------------------------------------------------------------
SizeParameter::SizeParameter (int32 flags, int32 id, const char* name)
{
  // Установка информации для хоста
  UString (info.title, USTRINGSIZE (info.title)).assign (USTRING (name));
  UString (info.units, USTRINGSIZE (info.units)).assign (USTRING ("m"));

  // Установка флагов (описывают принципы взаимодествия для ввода)
  info.flags = flags;
  info.id = id;
  info.stepCount = 0;
  info.defaultNormalizedValue = 1.f;
  info.unitId = kRootUnitId;

  // Установка начального значения
  setNormalized (0.f);
}

//------------------------------------------------------------------------
void SizeParameter::toString (ParamValue normValue, String128 string) const
{
  char text [32];
  sprintf (text, "%.2f", (float) normValue * 100);
  UString (string, 128).fromAscii (text);
}

//------------------------------------------------------------------------
bool SizeParameter::fromString (const TChar* string, ParamValue& normValue) const
{
  String wrapper ((TChar*) string);
  double tmp = 0;
  if (wrapper.scanFloat (tmp))
  {
    // Ограничение значения
    if (tmp < 1)
      tmp = 1;

    normValue = tmp / 100.f;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------
bool SizeParameter::setNormalized (ParamValue v)
{
  // Ограничение значения
  if ( v < 0.01 )
    return Parameter::setNormalized (0.01);
  return Parameter::setNormalized (v);
}

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
