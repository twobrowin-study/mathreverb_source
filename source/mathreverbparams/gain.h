#include "mathreverbcontroller.h"
#include "mathreverbparamids.h"

#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"

#include "base/source/fstring.h"

#include <stdio.h>
#include <math.h>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// GainParameter Declaration
//------------------------------------------------------------------------
class GainParameter : public Parameter
{
public:
  GainParameter (int32 flags, int32 id);

  virtual void toString (ParamValue normValue, String128 string) const;
  virtual bool fromString (const TChar* string, ParamValue& normValue) const;
};

//------------------------------------------------------------------------
// GainParameter Implementation
//------------------------------------------------------------------------
GainParameter::GainParameter (int32 flags, int32 id)
{
  UString (info.title, USTRINGSIZE (info.title)).assign (USTRING ("Gain"));
  UString (info.units, USTRINGSIZE (info.units)).assign (USTRING ("dB"));

  info.flags = flags;
  info.id = id;
  info.stepCount = 0;
  info.defaultNormalizedValue = 1.f;
  info.unitId = kRootUnitId;

  setNormalized (1.0);
}

//------------------------------------------------------------------------
void GainParameter::toString (ParamValue normValue, String128 string) const
{
  char text [32];
  if (normValue > 0.001f)
    sprintf (text, "%.2f", 20 * log10f ( (float) normValue));
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

    normValue = expf (logf (10.f) * (float) tmp / 20.f);
    return true;
  }
  return false;
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
