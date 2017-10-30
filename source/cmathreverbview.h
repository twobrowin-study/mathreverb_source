#pragma once

#include "vstgui4/vstgui/vstgui.h"
#include "vstgui4/vstgui/lib/clinestyle.h"

#include <stdlib.h>

#include "mathreverbparams/coordinate.h"

namespace VSTGUI {

//------------------------------------------------------------------------
// CMathReverbView: Декларация
// Предназначен для графического вывода геометрии помещения,
// положений источника и приёмника
//------------------------------------------------------------------------
class CMathReverbView : public CView
{
public:
  // Конструктор
	CMathReverbView ( const CRect& size
									, Steinberg::Vst::SizeParameter *widthGiver
									, Steinberg::Vst::SizeParameter *heightGiver
									, Steinberg::Vst::SizeParameter *lengthGiver
									, Steinberg::Vst::CoordinateParameter *xPosGiver
									, Steinberg::Vst::CoordinateParameter *yPosGiver
									, Steinberg::Vst::CoordinateParameter *zPosGiver
								)
								 : CView (size)
								 , mWidth (100)
								 , mHeight (100)
								 , mLength (100)
								 , mXPos (0)
								 , mYPos (0)
								 , mZPos (0)
								 , mWidthGiver (widthGiver)
								 , mHeightGiver (heightGiver)
								 , mLengthGiver (lengthGiver)
								 , mXPosGiver (xPosGiver)
								 , mYPosGiver (yPosGiver)
								 , mZPosGiver (zPosGiver)
								 {}

  // Метод рисования - вывода в заданое поле графической информации
  virtual void draw (CDrawContext *pContext);

	// Метод обновления параметров и перерисовки
	void update ();
private:
	int mWidth, mHeight, mLength;
	int mXPos, mYPos, mZPos;
	Steinberg::Vst::SizeParameter *mWidthGiver, *mHeightGiver, *mLengthGiver;
	Steinberg::Vst::CoordinateParameter *mXPosGiver, *mYPosGiver, *mZPosGiver;
};

//------------------------------------------------------------------------
// CMathReverbView: Реализация
//------------------------------------------------------------------------

//------------------------------------------------------------------------
void CMathReverbView::draw (CDrawContext *pContext)
{
	// Центр графика
	int centerX = (int) getWidth () / 2,
			centerY = (int) getHeight () / 2;

	// Точки параллелепипеда
	CPoint parallPoint[8] = { CPoint (centerX-(int) (mWidth*1.5+mLength/4.7), centerY-(int) (mHeight*1.5+mLength/4.7)),
														CPoint (centerX-(int) (mWidth*1.5+mLength/4.7), centerY+(int) (mHeight*1.5-mLength/4.7)),
														CPoint (centerX+(int) (mWidth*1.5-mLength/4.7), centerY+(int) (mHeight*1.5-mLength/4.7)),
														CPoint (centerX+(int) (mWidth*1.5-mLength/4.7), centerY-(int) (mHeight*1.5+mLength/4.7)),
														CPoint (centerX-(int) (mWidth*1.5-mLength/4.7), centerY-(int) (mHeight*1.5-mLength/4.7)),
														CPoint (centerX-(int) (mWidth*1.5-mLength/4.7), centerY+(int) (mHeight*1.5+mLength/4.7)),
														CPoint (centerX+(int) (mWidth*1.5+mLength/4.7), centerY+(int) (mHeight*1.5+mLength/4.7)),
														CPoint (centerX+(int) (mWidth*1.5+mLength/4.7), centerY-(int) (mHeight*1.5-mLength/4.7))
													};

	// Точки центрального куба
	int xIntend = mXPos*3-(mXPos/abs (mXPos))*10
		 ,yIntend = mYPos*3-(mYPos/abs (mYPos))*10;
	CPoint centralPoints[8] = { CPoint (centerX+xIntend-mZPos/3-10, centerY-yIntend-mZPos/3-10),
															CPoint (centerX+xIntend-mZPos/3-10, centerY-yIntend-mZPos/3+5),
															CPoint (centerX+xIntend-mZPos/3+5, centerY-yIntend-mZPos/3+5),
															CPoint (centerX+xIntend-mZPos/3+5, centerY-yIntend-mZPos/3-10),
															CPoint (centerX+xIntend-mZPos/3-5, centerY-yIntend-mZPos/3-5),
															CPoint (centerX+xIntend-mZPos/3-5, centerY-yIntend-mZPos/3+10),
															CPoint (centerX+xIntend-mZPos/3+10, centerY-yIntend-mZPos/3+10),
															CPoint (centerX+xIntend-mZPos/3+10, centerY-yIntend-mZPos/3-5)
														};

	// Сплошная линия
	pContext->setLineStyle (CLineStyle ());
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
	//
	// // Сплошные линии центрального куба
	// pContext->drawLine (centralPoints[0], centralPoints[1]);
	// pContext->drawLine (centralPoints[3], centralPoints[0]);
	//
	// pContext->drawLine (centralPoints[0], centralPoints[4]);
	// pContext->drawLine (centralPoints[4], centralPoints[5]);
	// pContext->drawLine (centralPoints[5], centralPoints[6]);
	// pContext->drawLine (centralPoints[6], centralPoints[7]);
	//
	// pContext->drawLine (centralPoints[5], centralPoints[4]);
	// pContext->drawLine (centralPoints[7], centralPoints[3]);
	//
	// pContext->drawLine (centralPoints[1], centralPoints[5]);
	// pContext->drawLine (centralPoints[4], centralPoints[7]);

	// Пунктирные линии параллелепипеда
	CCoord mLengthsDashLong[6] = { CCoord (3.0),
																CCoord (3.0),
																CCoord (3.0),
																CCoord (3.0),
																CCoord (3.0),
																CCoord (3.0)
															};
	pContext->setLineStyle (CLineStyle(VSTGUI::CLineStyle::kLineCapButt, VSTGUI::CLineStyle::kLineJoinMiter, 1.0, 6, mLengthsDashLong));
	pContext->drawLine (parallPoint[2], parallPoint[6]);
	pContext->drawLine (parallPoint[2], parallPoint[3]);
	pContext->drawLine (parallPoint[2], parallPoint[1]);
	//
	// // Пунктирные линии центрального куба
	// CCoord mLengthsDashShort[10] = { CCoord (0.1),
	// 																CCoord (0.1),
	// 																CCoord (0.1),
	// 																CCoord (0.1),
	// 																CCoord (0.1),
	// 																CCoord (0.1),
	// 																CCoord (0.1),
	// 																CCoord (0.1),
	// 																CCoord (0.1),
	// 																CCoord (0.1)
	// 															};
	// pContext->setLineStyle (CLineStyle(VSTGUI::CLineStyle::kLineCapButt, VSTGUI::CLineStyle::kLineJoinMiter, 1.0, 10, mLengthsDashShort));
	// pContext->drawLine (centralPoints[2], centralPoints[6]);
	// pContext->drawLine (centralPoints[2], centralPoints[3]);
	// pContext->drawLine (centralPoints[2], centralPoints[1]);


  setDirty (false);
}

//------------------------------------------------------------------------
void CMathReverbView::update ()
{
	invalid ();
}

//------------------------------------------------------------------------
} // Пространство имён VSTGUI
