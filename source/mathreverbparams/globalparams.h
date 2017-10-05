#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"

#include "base/source/fstring.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// GlobalParams Деклорация
// В экземпляре класса содержатся все вводимые переменные
//------------------------------------------------------------------------
class GlobalParams
{
public:
  // Конструктор
  GlobalParams (float gain, float length,
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

  // Хранимые параметры
  float fGain, fLength,
        fWidth, fHeight,
        fReflection, fXPos,
        fYPos, fZPos;
};

//------------------------------------------------------------------------
} // пространство имён Vst
} // пространство имён Steinberg
