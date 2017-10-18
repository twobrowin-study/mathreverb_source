#pragma once

#include "vstgui4/vstgui/vstgui.h"
#include "vstgui4/vstgui/lib/clinestyle.h"

#include <stdlib.h>

namespace VSTGUI {

//------------------------------------------------------------------------
// CMathReverbView: Декларация
// Предназначен для графического вывода геометрии помещения, положений источника и приёмника
//------------------------------------------------------------------------
class CMathReverbView : public CView
{
public:
  // Конструктор
	CMathReverbView (const CRect& size): CView (size)
																			 ,width (100)
																			 ,height (100)
																			 ,length (100)
																			 ,xPos (0)
																			 ,yPos (0)
																			 ,zPos (0)
																			 {}

  // Метод рисования - вывода в заданое поле графической информации
  virtual void draw (CDrawContext *pContext);
	void setWidthNormalized (double w)
	{
		width =
		invalid ();
	}
private:
	int width, height, length;
	int xPos, yPos, zPos;
};

void CMathReverbView::draw (CDrawContext *pContext)
{
	// Центр графика
	int centerX = (int) getWidth () / 2,
			centerY = (int) getHeight () / 2;

	// Точки параллелепипеда
	CPoint parallPoint[8] = { CPoint (centerX-(int) (width*1.5+length/4.7), centerY-(int) (height*1.5+length/4.7)),
														CPoint (centerX-(int) (width*1.5+length/4.7), centerY+(int) (height*1.5-length/4.7)),
														CPoint (centerX+(int) (width*1.5-length/4.7), centerY+(int) (height*1.5-length/4.7)),
														CPoint (centerX+(int) (width*1.5-length/4.7), centerY-(int) (height*1.5+length/4.7)),
														CPoint (centerX-(int) (width*1.5-length/4.7), centerY-(int) (height*1.5-length/4.7)),
														CPoint (centerX-(int) (width*1.5-length/4.7), centerY+(int) (height*1.5+length/4.7)),
														CPoint (centerX+(int) (width*1.5+length/4.7), centerY+(int) (height*1.5+length/4.7)),
														CPoint (centerX+(int) (width*1.5+length/4.7), centerY-(int) (height*1.5-length/4.7))
													};

	// Точки центрального куба
	int xIntend = xPos*3-(xPos/abs (xPos))*10
		 ,yIntend = yPos*3-(yPos/abs (yPos))*10;
	CPoint centralPoints[8] = { CPoint (centerX+xIntend-zPos/3-10, centerY-yIntend-zPos/3-10),
															CPoint (centerX+xIntend-zPos/3-10, centerY-yIntend-zPos/3+5),
															CPoint (centerX+xIntend-zPos/3+5, centerY-yIntend-zPos/3+5),
															CPoint (centerX+xIntend-zPos/3+5, centerY-yIntend-zPos/3-10),
															CPoint (centerX+xIntend-zPos/3-5, centerY-yIntend-zPos/3-5),
															CPoint (centerX+xIntend-zPos/3-5, centerY-yIntend-zPos/3+10),
															CPoint (centerX+xIntend-zPos/3+10, centerY-yIntend-zPos/3+10),
															CPoint (centerX+xIntend-zPos/3+10, centerY-yIntend-zPos/3-5)
														};

	// Сплошная линия
  pContext->setLineWidth (2);
  pContext->setFrameColor (CColor (0, 255, 0, 255)); // зелёный

	// Сплошные линии параллелепипеда
	pContext->drawLine (parallPoint[0], parallPoint[1]);
	pContext->drawLine (parallPoint[3], parallPoint[0]);

	pContext->drawLine (parallPoint[0], parallPoint[4]);
	pContext->drawLine (parallPoint[4], parallPoint[5]);
	pContext->drawLine (parallPoint[5], parallPoint[6]);
	pContext->drawLine (parallPoint[6], parallPoint[7]);

	pContext->drawLine (parallPoint[5], parallPoint[4]);
	pContext->drawLine (parallPoint[7], parallPoint[3]);

	pContext->drawLine (parallPoint[1], parallPoint[5]);
	pContext->drawLine (parallPoint[4], parallPoint[7]);

	// Сплошные линии центрального куба
	pContext->drawLine (centralPoints[0], centralPoints[1]);
	pContext->drawLine (centralPoints[3], centralPoints[0]);

	pContext->drawLine (centralPoints[0], centralPoints[4]);
	pContext->drawLine (centralPoints[4], centralPoints[5]);
	pContext->drawLine (centralPoints[5], centralPoints[6]);
	pContext->drawLine (centralPoints[6], centralPoints[7]);

	pContext->drawLine (centralPoints[5], centralPoints[4]);
	pContext->drawLine (centralPoints[7], centralPoints[3]);

	pContext->drawLine (centralPoints[1], centralPoints[5]);
	pContext->drawLine (centralPoints[4], centralPoints[7]);

	// Пунктирные линии параллелепипеда
	CCoord lengthsDashLong[6] = { CCoord (3.0),
																CCoord (3.0),
																CCoord (3.0),
																CCoord (3.0),
																CCoord (3.0),
																CCoord (3.0)
															};
	pContext->setLineStyle (CLineStyle(VSTGUI::CLineStyle::kLineCapButt, VSTGUI::CLineStyle::kLineJoinMiter, 1.0, 6, lengthsDashLong));
	pContext->drawLine (parallPoint[2], parallPoint[6]);
	pContext->drawLine (parallPoint[2], parallPoint[3]);
	pContext->drawLine (parallPoint[2], parallPoint[1]);

	// Пунктирные линии центральный куба
	CCoord lengthsDashShort[10] = { CCoord (0.1),
																	CCoord (0.1),
																	CCoord (0.1),
																	CCoord (0.1),
																	CCoord (0.1),
																	CCoord (0.1),
																	CCoord (0.1),
																	CCoord (0.1),
																	CCoord (0.1),
																	CCoord (0.1)
																};
	pContext->setLineStyle (CLineStyle(VSTGUI::CLineStyle::kLineCapButt, VSTGUI::CLineStyle::kLineJoinMiter, 1.0, 10, lengthsDashShort));
	pContext->drawLine (centralPoints[2], centralPoints[6]);
	pContext->drawLine (centralPoints[2], centralPoints[3]);
	pContext->drawLine (centralPoints[2], centralPoints[1]);


  setDirty (false);
}

//------------------------------------------------------------------------
} // Пространство имён VSTGUI
