#pragma once

#include "vstgui4/vstgui/vstgui.h"

namespace VSTGUI {

//------------------------------------------------------------------------
// CMathReverbView: Декларация
// Предназначен для графического вывода геометрии помещения, положений источника и приёмника
//------------------------------------------------------------------------
class CMathReverbView : public CView
{
public:
  // Конструктор
	CMathReverbView(const CRect& rect): CView(rect) {}
  // Метод рисования - вывода в заданое поле графической информации
  virtual void draw(CDrawContext *pContext);
};

void CMathReverbView::draw(CDrawContext *pContext)
{

  // --- Выведем установленное извне изображение, если оно есть
  if(getDrawBackground())
  {
    getDrawBackground()->draw(pContext, size);
  }
  else
  {
    // --- Нарисуем прямоугольнык заднего фона
    pContext->setLineWidth(1);
    pContext->setFillColor(CColor(200, 200, 200, 255)); // светло серый
    pContext->setFrameColor(CColor(0, 0, 0, 255)); // черный

    // --- Нарисовать прямоугольник по всей площади
    pContext->drawRect(size, kDrawFilledAndStroked);
  }
  setDirty (false);
}

//------------------------------------------------------------------------
} // Пространство имён VSTGUI
