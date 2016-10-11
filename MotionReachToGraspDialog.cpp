// MotionReachToGraspDialog.cpp : implementation file
//
/*
#include "stdafx.h"
#include "MotionReachToGraspDialog.h"

// MotionReachToGraspDialog dialog

IMPLEMENT_DYNAMIC(MotionReachToGraspDialog, CMotionDialogTab)

MotionReachToGraspDialog::MotionReachToGraspDialog(int initialFrame, CWnd* pParent )
	: CMotionDialogTab(initialFrame, MotionReachToGraspDialog::IDD, pParent)
	, TargetLocX(_T(""))
	, TargetLocY(_T(""))
	, TargetLocZ(_T(""))
	, GraspType(FALSE)
	, SurfacePosition(FALSE)
	, ThumbPosition(FALSE)
	, Phase(FALSE)
{

}

MotionReachToGraspDialog::~MotionReachToGraspDialog()
{
}

MotionPredictionInfo MotionReachToGraspDialog::getMotionInfo() {
	UpdateData(true);
	MotionPredictionInfo info = motionInfo;
//	info.useDefaultInitial = (InitialPostureContents == 1);
//	info.useDefaultFinal = (FinalPostureContents == 1);
	info.phase = Phase;
	info.hms_task.target[0] = atof(TargetLocX);
	info.hms_task.target[1] = atof(TargetLocY);
	info.hms_task.target[2] = atof(TargetLocZ);
	info.hms_task.experno = 6;
	if(GraspType == 0) { // push
		info.hms_task.push = 0;
	} else {
		info.hms_task.push = 1;
	}
	info.hms_task.surface = SurfacePosition;
	if(ThumbPosition == 0) { // left
		info.hms_task.thumb = 4;
	} else if(ThumbPosition == 1) { // top
		info.hms_task.thumb = 1;
	} else if(ThumbPosition == 2) { // right
		info.hms_task.thumb = 2;
	} else { // bottom
		info.hms_task.thumb = 3;
	}
	info.hms_task.exptype = 'c'; // meaningless for this experiment, but time prediction checks for it
	return info;
}

void MotionReachToGraspDialog::DoDataExchange(CDataExchange* pDX)
{
	CMotionDialogTab::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TARGETX, TargetLocX);
	DDX_Text(pDX, IDC_TARGETY, TargetLocY);
	DDX_Text(pDX, IDC_TARGETZ, TargetLocZ);
	DDX_Radio(pDX, IDC_GRASPTYPE, GraspType);
	DDX_Radio(pDX, IDC_SURFACEPOSITION, SurfacePosition);
	DDX_Radio(pDX, IDC_THUMBPOSITION, ThumbPosition);
	DDX_Radio(pDX, IDC_PHASE, Phase);
}


BEGIN_MESSAGE_MAP(MotionReachToGraspDialog, CMotionDialogTab)
END_MESSAGE_MAP()


// MotionReachToGraspDialog message handlers

BOOL MotionReachToGraspDialog::OnInitDialog()
{
	CMotionDialogTab::OnInitDialog();

	TargetLocX.Format("%d", 30);
	TargetLocY.Format("%d", 80);
	TargetLocZ.Format("%d", 90);
	GraspType = 0;
	SurfacePosition = 0;
	ThumbPosition = 0;
	Phase = 0;
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
*/