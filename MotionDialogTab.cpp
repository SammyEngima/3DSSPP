// MotionDialogTab.cpp : implementation file
//

#include "stdafx.h"
#include "MotionDialogTab.h"
#include "MotionTabCtrl.h"

// CMotionDialogTab dialog

IMPLEMENT_DYNAMIC(CMotionDialogTab, CDialog)

CMotionDialogTab::CMotionDialogTab(UINT nIDTemplate, CWnd* pParentWnd)
	: CDialog(nIDTemplate, pParentWnd) {
	tabControl = (CMotionTabCtrl*)pParentWnd;
}

CMotionDialogTab::~CMotionDialogTab()
{
}


void CMotionDialogTab::notifyUpdate() {
	// TODO
}
Motion* CMotionDialogTab::getMotion() {
	return motion;
}

// CMotionDialogTab message handlers
BEGIN_MESSAGE_MAP(CMotionDialogTab, CDialog)
END_MESSAGE_MAP()