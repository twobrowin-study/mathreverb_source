#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"

#include "base/source/fstring.h"

#include <stdio.h>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// ReflectionParameter Деклорация
// Описание параметров координат, длины, ширины, высоты
//------------------------------------------------------------------------
class ReflectionParameter : public Parameter
{
public:
  // Конструктор
  ReflectionParameter (int32 flags, int32 id);

  // Преобразование на вывод
  virtual void toString (ParamValue normValue, String128 string) const;
  //  Преобразование после ввода
  virtual bool fromString (const TChar* string, ParamValue& normValue) const;
};

//------------------------------------------------------------------------
// ReflectionParameter Реализация
//------------------------------------------------------------------------
ReflectionParameter::ReflectionParameter (int32 flags, int32 id)
{
  // Установка информации для хоста
  UString (info.title, USTRINGSIZE (info.title)).assign (USTRING ("Reflection"));
  UString (info.units, USTRINGSIZE (info.units)).assign (USTRING ("%"));

  // Установка флагов (описывают принципы взаимодествия для ввода)
  info.flags = flags;
  info.id = id;
  info.stepCount = 0;
  info.defaultNormalizedValue = 1.f;
  info.unitId = kRootUnitId;

  // Установка начального значения
  setNormalized (1.f);
}

//------------------------------------------------------------------------
void ReflectionParameter::toString (ParamValue normValue, String128 string) const
{
  char text [32];
  sprintf (text, "%3d", (int) (normValue * 100));
  UString (string, 128).fromAscii (text);
}

//------------------------------------------------------------------------
bool ReflectionParameter::fromString (const TChar* string, ParamValue& normValue) const
{
  String wrapper ((TChar*) string);
  double tmp = 0;
  if (wrapper.scanFloat (tmp))
  {
    // Ограничение на только положительные значения
    if (tmp < 0.0)
      tmp = -tmp;

    // Ограничение 100%
    if (tmp > 100.0)
      tmp = 100.0;

    normValue = tmp / 100.f;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------
} // Пространство имён Vst
} // Пространство имён Steinberg
