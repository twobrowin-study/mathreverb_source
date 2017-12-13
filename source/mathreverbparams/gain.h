#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"

#include "base/source/fstring.h"

#include <stdio.h>
#include <math.h>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// GainParameter Decloration
//------------------------------------------------------------------------
class GainParameter : public Parameter
{
public:
  // Constructor
  GainParameter (int32 flags, int32 id);

  // Output transform
  virtual void toString (ParamValue normValue, String128 string) const;
  // Input transform
  virtual bool fromString (const TChar* string, ParamValue& normValue) const;
};

//------------------------------------------------------------------------
// GainParameter Implementation
//------------------------------------------------------------------------
GainParameter::GainParameter (int32 flags, int32 id)
{
  // For host info
  UString (info.title, USTRINGSIZE (info.title)).assign (USTRING ("Gain"));
  UString (info.units, USTRINGSIZE (info.units)).assign (USTRING ("dB"));

  // Flags
  info.flags = flags;
  info.id = id;
  info.stepCount = 0;
  info.defaultNormalizedValue = 1.f;
  info.unitId = kRootUnitId;

  // Base value
  setNormalized (1.0);
}

//------------------------------------------------------------------------
void GainParameter::toString (ParamValue normValue, String128 string) const
{
  char text [32];
  if (normValue > 0.001f) // Min limit
    sprintf (text, "%.1f", 20 * log10f ( (float) normValue)); // From normValue to dB formula
  else
    strcpy (text, "-oo");
  UString (string, 128).fromAscii (text);
}

//------------------------------------------------------------------------
bool GainParameter::fromString (const TChar* string, ParamValue& normValue) const
{
  String wrapper ((TChar*) string);
  double tmp = 0;
  if (wrapper.scanFloat (tmp))
  {
    // No minus values
    if (tmp > 0.0)
      tmp = -tmp;

    normValue = expf (logf (10.f) * (float) tmp / 20.f); // From dB to normValue
    return true;
  }
  return false;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
