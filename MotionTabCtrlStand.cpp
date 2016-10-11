#include "MotionTabCtrlStand.h"

#include "MotionHandlingDialog.h"
#include "MotionInterpolationDialog.h"
#include "Dg_MotionPrediction.h"

CMotionTabCtrlStand::CMotionTabCtrlStand(int initialFrame, Dg_MotionPrediction* parent) :
	CMotionTabCtrl(initialFrame, parent)
{
	m_nPageCount = 2;

	m_DialogID = new int[m_nPageCount];
	m_Dialog = new CMotionDialogTab* [m_nPageCount];

	m_DialogID[0] = IDD_MOTION_HANDLING;
	m_DialogID[1] = IDD_MOTION_INTERPOLATION;

	m_Dialog[0] = new MotionHandlingDialog(this);
	m_Dialog[1] = new MotionInterpolationDialog(this);

	imageList.Create(110, 15, ILC_MASK | ILC_COLOR32, 0, 0);
	COLORREF rgbTransparentColor = RGB(255,255,255);

	CBitmap bitmap;
	bitmap.LoadBitmapA(IDB_STANDING_HANDLING);
	imageList.Add(&bitmap, rgbTransparentColor);
	bitmap.Detach();
	bitmap.LoadBitmapA(IDB_INTERPOLATION);
	imageList.Add(&bitmap, rgbTransparentColor);
	bitmap.Detach();
}

CMotionTabCtrlStand::~CMotionTabCtrlStand(void) {
	for(int i = 0; i < m_nPageCount; i++) {
		delete m_Dialog[i];
	}
	delete[] m_DialogID;
	delete[] m_Dialog;
}
