#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"

#include "base/source/fstring.h"

#include <stdio.h>

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
  CoordinateParameter (int32 flags, int32 id, const char* name);

  // Преобразование на вывод
  virtual void toString (ParamValue normValue, String128 string) const;
  //  Преобразование после ввода
  virtual bool fromString (const TChar* string, ParamValue& normValue) const;
};

//------------------------------------------------------------------------
// CoordinateParameter Реализация
//------------------------------------------------------------------------
CoordinateParameter::CoordinateParameter (int32 flags, int32 id, const char* name)
{
  // Установка информации дял хоста
  UString (info.title, USTRINGSIZE (info.title)).assign (USTRING (name));
  UString (info.units, USTRINGSIZE (info.units)).assign (USTRING ("m"));

  // Установка флагов (описывают принципы взаимодествия для ввода)
  info.flags = flags;
  info.id = id;
  info.stepCount = 0;
  info.defaultNormalizedValue = 0;
  info.unitId = kRootUnitId;

  // Установка начального значения
  setNormalized (0);
}

//------------------------------------------------------------------------
void CoordinateParameter::toString (ParamValue normValue, String128 string) const
{
  char text [32];
  sprintf (text, "%d", (int) normValue);
  UString (string, 128).fromAscii (text);
}

//------------------------------------------------------------------------
bool CoordinateParameter::fromString (const TChar* string, ParamValue& normValue) const
{
  String wrapper ((TChar*) string);
  int64 tmp = 0;
  if (wrapper.scanInt (tmp)) {
    if (tmp < 0.0)
      tmp = -tmp;

    normValue = tmp;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------
} // пространство имён Vst
} // пространство имён Steinberg
