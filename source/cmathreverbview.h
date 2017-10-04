#pragma once

#include "vstgui4/vstgui/vstgui.h"

namespace VSTGUI {

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

  // --- bitmap, if one
  if(getDrawBackground())
  {
    getDrawBackground()->draw(pContext, size);
  }
  else
  {
    // --- setup the background rectangle
    pContext->setLineWidth(1);
    pContext->setFillColor(CColor(200, 200, 200, 255)); // light grey
    pContext->setFrameColor(CColor(0, 0, 0, 255)); // black

    // --- draw the rect filled (with grey) and stroked (line around rectangle)
    pContext->drawRect(size, kDrawFilledAndStroked);
  }
  setDirty (false);
}

//------------------------------------------------------------------------
} // namespace Vst
