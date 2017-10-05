#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"

#include "base/source/fstring.h"

#include "paramids.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// GlobalParams Деклорация
// В экземпляре класса содержатся все вводимые переменные
//------------------------------------------------------------------------
class GlobalParams : public Parameter
{
public:
  // Конструктор
  GlobalParams (float gain, float length,
                float width, float height,
                float reflection, float xpos,
                float ypos, float zpos);

  float getNormalParamByID (int32 id);

private:
  float fGain, fLength,
        fWidth, fHeight,
        fReflection, fXPos,
        fYPos, fZPos;
};

//------------------------------------------------------------------------
// GlobalParams Реализация
//------------------------------------------------------------------------
GlobalParams::GlobalParams (float gain, float length,
                            float width, float height,
                            float reflection, float xpos,
                            float ypos, float zpos)
{
  fGain = gain;
  fLength = length;
  fWidth = width;
  fHeight = height;
  fReflection = reflection;
  fXPos = xpos;
  fYPos = ypos;
  fZPos = zpos;
}

//------------------------------------------------------------------------
float GlobalParams::getNormalParamByID (int32 id)
{
  switch (id)
  {
    case kGainId:
      return fGain;
    case kLengthId:
      return fLength;
    case kWidthId:
      return fWidth;
    case kHeightId:
      return fHeight;
    case kReflectionId:
      return fReflection;
    case kXPosId:
      return fXPos;
    case kYPosId:
      return fYPos;
    case kZPosId:
      return fZPos;
    default:
      return 0.f;
  }
}

//------------------------------------------------------------------------
} // пространство имён Vst
} // пространство имён Steinberg
