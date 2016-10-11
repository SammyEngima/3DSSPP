// MotionTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MotionTabCtrl.h"
#include "Dg_MotionPrediction.h"

#include "PredictedMotion.h"
#include "MorphedPhaseMotion.h"
#include "LinearMotion.h"


// CMotionTabCtrl

IMPLEMENT_DYNAMIC(CMotionTabCtrl, CTabCtrl)

CMotionTabCtrl::CMotionTabCtrl(int initialFrame, Dg_MotionPrediction* parent_in) :
	parent(parent_in)
{
}

CMotionTabCtrl::~CMotionTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CMotionTabCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMotionTabCtrl::OnTcnSelchange)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()



// CMotionTabCtrl message handlers


//This function creates the Dialog boxes once
void CMotionTabCtrl::initDialogs() {
	displayDialog.Create(IDD_MOTIONFRAMEDISPLAY, GetParent());

	for(int i = 0; i < m_nPageCount; i++) {
		m_Dialog[i]->Create(m_DialogID[i], GetParent());
		this->InsertItem(TCIF_IMAGE, i, "something", i, 0);
	}

	this->SetImageList(&imageList);

	this->SetItemSize(CSize(25, 120));
	this->SetPadding(CSize(2,2));

	CRect l_rectClient;
	CRect l_rectWnd;

	GetClientRect(l_rectClient);
	AdjustRect(FALSE,l_rectClient);
	GetWindowRect(l_rectWnd);
	GetParent()->ScreenToClient(l_rectWnd);
	l_rectClient.OffsetRect(l_rectWnd.left,l_rectWnd.top);
	displayDialog.SetWindowPos(&wndBottom, l_rectClient.left, l_rectClient.top, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

	CRect topHalf;
	displayDialog.GetWindowRect(&topHalf);
	l_rectClient.top += topHalf.Height();
	for(int nCount=0; nCount < m_nPageCount; nCount++){
		m_Dialog[nCount]->SetWindowPos(&wndBottom, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_HIDEWINDOW);
	}
}

void CMotionTabCtrl::activateDialogs() {
	int nSel = GetCurSel(); 
	m_Dialog[nSel]->ShowWindow(SW_SHOW);
	for(int i = 0; i < m_nPageCount; i++) {
		if(i != nSel)
			m_Dialog[i]->ShowWindow(SW_HIDE);
	}

	m_Dialog[nSel]->SetFocus();
	displayDialog.setMotion(m_Dialog[nSel]->getMotion());

	// if we're on interpolation, disable drawing the target
	// Assumption: Interpolation is the last tab
	if(nSel == m_nPageCount - 1) {
		displayDialog.getInitialFrameDisplay().setNoDrawTarget();
		displayDialog.getFinalFrameDisplay().setNoDrawTarget();
		displayDialog.getPredictedFrameDisplay().setNoDrawTarget();
	} else {
		// otherwise, enable the target, and disable dragging in one of the displays and
		// set the frame/default option depending on the phase

		// target
		//displayDialog.getInitialFrameDisplay().setDrawTarget((static_cast<MorphedPhaseMotion*>(m_Dialog[nSel]->getMotion()))->GetInitialTarget(true));
		//displayDialog.getFinalFrameDisplay().setDrawTarget((static_cast<MorphedPhaseMotion*>(m_Dialog[nSel]->getMotion()))->GetFinalTarget(true));
		//displayDialog.getPredictedFrameDisplay().setDrawTarget((static_cast<PredictedMotion*>(m_Dialog[nSel]->getMotion()))->GetTarget(true));

		// disable dragging in display
		/*if(info.phase == 0 || info.phase == 2) {
			displayDialog.getInitialFrameDisplay().enableDragging(true);
			displayDialog.getFinalFrameDisplay().enableDragging(false);
		} else {
			displayDialog.getInitialFrameDisplay().enableDragging(false);
			displayDialog.getFinalFrameDisplay().enableDragging(true);
		}*/
	}

	displayDialog.update();
}

Motion* CMotionTabCtrl::getMotion() {
	int curTab = GetCurSel();
	return m_Dialog[curTab]->getMotion();
}

void CMotionTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	activateDialogs();
	*pResult = 0;
}
