#pragma once

// MotionSeatedPointDialog dialog
#include "resource.h"
#include "viewcombobox.h"
#include "MotionDialogTab.h"
#include "FloatEdit.h"
#include "afxwin.h"

class MotionSeatedPointDialog : public CMotionDialogTab
{
	DECLARE_DYNAMIC(MotionSeatedPointDialog)

public:
	MotionSeatedPointDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~MotionSeatedPointDialog();

// Dialog Data
	enum { IDD = IDD_MOTION_SEATED_POINT };

protected:
	// take values from member variables and apply them to the Motion
	bool membersToMotion();
	// take values from the Motion and apply them to member variables
	void motionToMembers();
	// do work to apply changes that the user makes to the motion
	void motionChanged();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString TargetLocX;
	CString TargetLocY;
	CString TargetLocZ;
	BOOL ReachType;
	BOOL Phase;
	virtual BOOL OnInitDialog();

	CFloatEdit TargetLocXControl;
	CFloatEdit TargetLocYControl;
	CFloatEdit TargetLocZControl;
	afx_msg void OnBnClickedPhase();
	afx_msg void OnEnKillfocusTargetx();
	afx_msg void OnEnKillfocusTargety();
	afx_msg void OnEnKillfocusTargetz();
	afx_msg void OnBnKillfocusReachtype();
	CString TargetLocationLabel;
};
