#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"

#include "base/source/fstring.h"

#include <stdio.h>

#include "size.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// CoordinateParameter Decloration
//------------------------------------------------------------------------
class CoordinateParameter : public Parameter
{
public:
  // Constructor
  CoordinateParameter (int32 flags, int32 id, const char* name, SizeParameter *limitGiver);

  // Output transform
  virtual void toString (ParamValue normValue, String128 string) const;
  // Input transform
  virtual bool fromString (const TChar* string, ParamValue& normValue) const;
  // Param set
  virtual bool setNormalized (ParamValue v);

  // Limit of normValue
  void updateLimit ();

private:
  SizeParameter *mLimitGiver;
  double fLimit;
};

//------------------------------------------------------------------------
// CoordinateParameter Implementation
//------------------------------------------------------------------------
CoordinateParameter::CoordinateParameter (int32 flags, int32 id, const char* name, SizeParameter *limitGiver)
: mLimitGiver (limitGiver)
, fLimit (0.49f)
{
  // For host info
  UString (info.title, USTRINGSIZE (info.title)).assign (USTRING (name));
  UString (info.units, USTRINGSIZE (info.units)).assign (USTRING ("m"));

  // Flags
  info.flags = flags;
  info.id = id;
  info.stepCount = 0;
  info.defaultNormalizedValue = 0.5f;
  info.unitId = kRootUnitId;

  // Base value
  setNormalized (0.5f);
}

//------------------------------------------------------------------------
void CoordinateParameter::toString (ParamValue normValue, String128 string) const
{
  char text [32];
  normValue -= 0.5f; // Model config
  sprintf (text, "%.2f", (float) normValue * 100);
  UString (string, 128).fromAscii (text);
}

//------------------------------------------------------------------------
bool CoordinateParameter::fromString (const TChar* string, ParamValue& normValue) const
{
  String wrapper ((TChar*) string);
  double tmp = 0;
  if (wrapper.scanFloat (tmp))
  {
    // Value limits
    if (tmp > fLimit * 100.f)
      normValue = 0.5f + fLimit;
    else
      if (tmp < -fLimit * 100.f)
        normValue = 0.5f - fLimit;
      else
        // Value settin
        normValue = tmp / 100.f + 0.5f;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------
bool CoordinateParameter::setNormalized (ParamValue v)
{
  // Limits
  if ( v > 0.5f + fLimit )
    return Parameter::setNormalized (0.5f + fLimit);
  if ( v < 0.5f - fLimit )
    return Parameter::setNormalized (0.5f - fLimit);
  // Value settin
  return Parameter::setNormalized (v);
}


//------------------------------------------------------------------------
void CoordinateParameter::updateLimit ()
{
  // Updated limit
  fLimit = mLimitGiver->getNormalized () * 0.49f;
  // Limit settin
  setNormalized (getNormalized ());
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
