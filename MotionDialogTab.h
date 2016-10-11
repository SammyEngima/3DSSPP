#pragma once

#include "Task.h"
// CMotionDialogTab dialog
class CMotionTabCtrl;
class Motion;

class CMotionDialogTab : public CDialog
{
	DECLARE_DYNAMIC(CMotionDialogTab)

public:
	CMotionDialogTab(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CMotionDialogTab();

	void notifyUpdate();
	CMotionTabCtrl* tabControl;
	Motion* getMotion();

// Dialog Data
	//enum { IDD = IDD_MOTIONDIALOGTAB };

//protected:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	Motion* motion;
};
