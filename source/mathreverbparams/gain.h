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
// example of custom parameter (overwriting to and fromString)
//------------------------------------------------------------------------
class GainParameter : public Parameter {
public:
  GainParameter (int32 flags, int32 id);

  virtual void toString (ParamValue normValue, String128 string) const;
  virtual bool fromString (const TChar* string, ParamValue& normValue) const;
};

//------------------------------------------------------------------------
// GainParameter Implementation
//------------------------------------------------------------------------
GainParameter::GainParameter (int32 flags, int32 id) {
  UString (info.title, USTRINGSIZE (info.title)).assign (USTRING ("Gain"));
  UString (info.units, USTRINGSIZE (info.units)).assign (USTRING ("%%")); // NOTE: dB (find formula)

  info.flags = flags;
  info.id = id;
  info.stepCount = 0;
  info.defaultNormalizedValue = 0.5f;
  info.unitId = kRootUnitId;

  setNormalized (1.0);
}

//------------------------------------------------------------------------
void GainParameter::toString (ParamValue normValue, String128 string) const {
  char text [32];
  if (normValue > 0.001f)
    sprintf (text, "%.4f", normValue);
  else
    strcpy (text, "-oo");
  UString (string, 128).fromAscii (text);
}

//------------------------------------------------------------------------
bool GainParameter::fromString (const TChar* string, ParamValue& normValue) const {
  String wrapper ((TChar*) string);
  double tmp = 0;
  if (wrapper.scanFloat (tmp)) {
    normValue = tmp;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
