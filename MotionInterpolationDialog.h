#pragma once

#include "resource.h"
#include "viewcombobox.h"
#include "Task.h"
#include "MotionDialogTab.h"
#include "IntRangeEdit.h"
#include "FloatRangeEdit.h"
#include "afxwin.h"
// MotionInterpolationDialog dialog

class MotionInterpolationDialog : public CMotionDialogTab
{
	DECLARE_DYNAMIC(MotionInterpolationDialog)

public:
	MotionInterpolationDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~MotionInterpolationDialog();

// Dialog Data
	enum { IDD = IDD_MOTION_INTERPOLATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString NumSeconds;
	CString NumFrames;

	virtual BOOL OnInitDialog();
	afx_msg void OnEnKillfocusSpecifySeconds();
	afx_msg void OnEnKillfocusSpecifyFrames();
	CFloatRangeEdit LengthSeconds;
	CIntRangeEdit LengthFrames;
	CString CurrentFrame;
	CString NextFrame;
};
