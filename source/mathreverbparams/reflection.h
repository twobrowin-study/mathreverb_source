#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"

#include "base/source/fstring.h"

#include <stdio.h>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// ReflectionParameter Decloration
//------------------------------------------------------------------------
class ReflectionParameter : public Parameter
{
public:
  // Constructor
  ReflectionParameter (int32 flags, int32 id);

  // Output transform
  virtual void toString (ParamValue normValue, String128 string) const;
  // Input transform
  virtual bool fromString (const TChar* string, ParamValue& normValue) const;
};

//------------------------------------------------------------------------
// ReflectionParameter Implementation
//------------------------------------------------------------------------
ReflectionParameter::ReflectionParameter (int32 flags, int32 id)
{
  // For host info
  UString (info.title, USTRINGSIZE (info.title)).assign (USTRING ("Reflection"));
  UString (info.units, USTRINGSIZE (info.units)).assign (USTRING ("%"));

  // Flags
  info.flags = flags;
  info.id = id;
  info.stepCount = 0;
  info.defaultNormalizedValue = 1.f;
  info.unitId = kRootUnitId;

  // Base value
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
    // Onlu pos value
    if (tmp < 0.0)
      tmp = -tmp;

    // 100% limit
    if (tmp > 100.0)
      tmp = 100.0;

    normValue = tmp / 100.f;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
