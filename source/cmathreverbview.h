#pragma once

#include "vstgui4/vstgui/vstgui.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// CMathReverbView: Наследник класса CView
// Предназначен для графического вывода геометрии помещения, положений источника и приёмника
//------------------------------------------------------------------------
class CMathReverbView : public CView
{
public:
	CMathReverbView(const CRect& size): CView(size) {}
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
