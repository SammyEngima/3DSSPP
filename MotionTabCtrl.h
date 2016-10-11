#pragma once

#include "stdafx.h"
#include "resource.h"
#include "atlimage.h"
#include "MotionDialogTab.h"
#include "MotionFrameDisplayDialog.h"

// CMotionTabCtrl

class Dg_MotionPrediction;
class Motion;

class CMotionTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMotionTabCtrl)

public:
	CMotionTabCtrl(int initialFrame, Dg_MotionPrediction* parent);
	virtual ~CMotionTabCtrl();

	// initialize the tab dialogs
	void initDialogs();
	// activate the dialog for the current tab
	void activateDialogs();

	// returns the motion of the current tab
	Motion* getMotion();

protected:
	DECLARE_MESSAGE_MAP()

public:
	int* m_DialogID;
	CMotionDialogTab** m_Dialog;
	int m_nPageCount;
	CImageList imageList;

	Dg_MotionPrediction* parent;

	MotionFrameDisplayDialog displayDialog;

	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


