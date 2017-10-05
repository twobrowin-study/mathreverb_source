#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"

#include "base/source/fstring.h"

#include <stdio.h>
#include <math.h>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// GainParameter Деклорация
// Описывает параметр громкости звука в дБ
//------------------------------------------------------------------------
class GainParameter : public Parameter
{
public:
  // Конструктор
  GainParameter (int32 flags, int32 id);

  // Преобразование на вывод
  virtual void toString (ParamValue normValue, String128 string) const;
  //  Преобразование после ввода
  virtual bool fromString (const TChar* string, ParamValue& normValue) const;
};

//------------------------------------------------------------------------
// GainParameter Реализация
//------------------------------------------------------------------------
GainParameter::GainParameter (int32 flags, int32 id)
{
  // Установка информации для хоста
  UString (info.title, USTRINGSIZE (info.title)).assign (USTRING ("Gain"));
  UString (info.units, USTRINGSIZE (info.units)).assign (USTRING ("dB"));

  // Установка флагов (описывают принципы взаимодествия для ввода)
  info.flags = flags;
  info.id = id;
  info.stepCount = 0;
  info.defaultNormalizedValue = 1.f;
  info.unitId = kRootUnitId;

  // Установка начального значения
  setNormalized (1.0);
}

//------------------------------------------------------------------------
void GainParameter::toString (ParamValue normValue, String128 string) const
{
  char text [32];
  if (normValue > 0.001f)
    sprintf (text, "%.2f", 20 * log10f ( (float) normValue)); // Формула преобразования из долей в дБ
  else
    strcpy (text, "-oo");
  UString (string, 128).fromAscii (text);
}

//------------------------------------------------------------------------
bool GainParameter::fromString (const TChar* string, ParamValue& normValue) const
{
  String wrapper ((TChar*) string);
  double tmp = 0;
  if (wrapper.scanFloat (tmp)) {
    if (tmp > 0.0)
      tmp = -tmp;

    normValue = expf (logf (10.f) * (float) tmp / 20.f); // Формула преобразования из дБ в доли
    return true;
  }
  return false;
}

//------------------------------------------------------------------------
} // пространство имён Vst
} // пространство имён Steinberg
