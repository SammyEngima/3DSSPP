#pragma once

#include "resource.h"
#include "viewcombobox.h"
#include "MotionDialogTab.h"
// MotionReachToGraspDialog dialog

class MotionReachToGraspDialog : public CMotionDialogTab
{
	DECLARE_DYNAMIC(MotionReachToGraspDialog)

public:
	MotionReachToGraspDialog(int initialFrame, CWnd* pParent = NULL);   // standard constructor
	virtual ~MotionReachToGraspDialog();

	virtual MotionPredictionInfo getMotionInfo();

// Dialog Data
	enum { IDD = IDD_MOTION_REACH_TO_GRASP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString TargetLocX;
	CString TargetLocY;
	CString TargetLocZ;
	BOOL GraspType;
	BOOL SurfacePosition;
	BOOL ThumbPosition;
	BOOL Phase;
	virtual BOOL OnInitDialog();
};
