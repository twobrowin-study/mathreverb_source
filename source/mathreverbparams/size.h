#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"

#include "base/source/fstring.h"

#include <stdio.h>

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// SizeParameter Decloration
//------------------------------------------------------------------------
class SizeParameter : public Parameter
{
public:
  // Constructor
  SizeParameter (int32 flags, int32 id, const char* name);

  // Output transform
  virtual void toString (ParamValue normValue, String128 string) const;
  // Input transform
  virtual bool fromString (const TChar* string, ParamValue& normValue) const;
  // Param settin
  virtual bool setNormalized (ParamValue v);
};

//------------------------------------------------------------------------
// SizeParameter Implementation
//------------------------------------------------------------------------
SizeParameter::SizeParameter (int32 flags, int32 id, const char* name)
{
  // For host info
  UString (info.title, USTRINGSIZE (info.title)).assign (USTRING (name));
  UString (info.units, USTRINGSIZE (info.units)).assign (USTRING ("m"));

  // Flags
  info.flags = flags;
  info.id = id;
  info.stepCount = 0;
  info.defaultNormalizedValue = 1.f;
  info.unitId = kRootUnitId;

  // Base value
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
    // Value limit
    if (tmp < 10.f)
      tmp = 10.f;

    normValue = tmp / 100.f;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------
bool SizeParameter::setNormalized (ParamValue v)
{
  // Value limit
  if ( v < 0.1f )
    return Parameter::setNormalized (0.1f);
  return Parameter::setNormalized (v);
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
