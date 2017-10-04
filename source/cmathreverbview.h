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
  // Конструктор
	CMathReverbView(const CRect& size): CView(size) {}
  // Метод рисования - вывода в заданое поле графической информации
  virtual void draw(CDrawContext *pContext);
};

void CMathReverbView::draw(CDrawContext *pContext)
{
  CView::draw(pContext);
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
