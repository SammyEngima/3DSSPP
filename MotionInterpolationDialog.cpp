// MotionInterpolationDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MotionInterpolationDialog.h"
#include "LinearMotion.h"
#include "Hom.h"
#include "Hom_Doc.h"
#include "MotionTabCtrl.h"
#include "MotionFrameDisplayDialog.h"

// MotionInterpolationDialog dialog

IMPLEMENT_DYNAMIC(MotionInterpolationDialog, CDialog)

MotionInterpolationDialog::MotionInterpolationDialog(CWnd* pParent )
	: CMotionDialogTab(MotionInterpolationDialog::IDD, pParent)
	, NumSeconds(_T(""))
	, NumFrames(_T(""))
	, LengthSeconds(IDD,0.04,1000)
	, LengthFrames(IDD,2,10000)
	, CurrentFrame(_T(""))
	, NextFrame(_T(""))
{
	motion = new LinearMotion(&Get_App_Ptr()->Get_Document_Ptr()->getTask());
}

MotionInterpolationDialog::~MotionInterpolationDialog()
{
	delete motion;
}

void MotionInterpolationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SPECIFY_SECONDS, NumSeconds);
	DDX_Text(pDX, IDC_SPECIFY_FRAMES, NumFrames);
	DDX_Control(pDX, IDC_SPECIFY_SECONDS, LengthSeconds);
	DDX_Control(pDX, IDC_SPECIFY_FRAMES, LengthFrames);
	DDX_Text(pDX, IDC_CURRENT_FRAME, CurrentFrame);
	DDX_Text(pDX, IDC_NEXT_FRAME, NextFrame);
}

BEGIN_MESSAGE_MAP(MotionInterpolationDialog, CDialog)
	ON_EN_KILLFOCUS(IDC_SPECIFY_SECONDS, &MotionInterpolationDialog::OnEnKillfocusSpecifySeconds)
	ON_EN_KILLFOCUS(IDC_SPECIFY_FRAMES, &MotionInterpolationDialog::OnEnKillfocusSpecifyFrames)
END_MESSAGE_MAP()


// MotionInterpolationDialog message handlers

BOOL MotionInterpolationDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	Task* task = motion->GetTask();
	int curFrame = task->getCurrentFrame();
	CurrentFrame.Format("Current Frame (%d)", curFrame);
	NextFrame.Format("Next Frame (%d)", curFrame + 1);
	NumFrames.Format("%d", FRAMES_PER_SECOND);
	NumSeconds.Format("%d", 1);
	LinearMotion* lmotion = (LinearMotion*)motion;
	lmotion->SetInitialFrame(*task->getSkeleton());
	lmotion->SetFinalFrame(*task->getSkeletonAtFrame(curFrame + 1));
	lmotion->SetNumFrames(FRAMES_PER_SECOND);
	lmotion->CreateMotion();
	tabControl->displayDialog.setMotion(lmotion);
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void MotionInterpolationDialog::OnEnKillfocusSpecifySeconds()
{
	UpdateData(true);
	if(LengthSeconds.Validate()) {
		NumFrames.Format("%d", (int)(atof(NumSeconds)*FRAMES_PER_SECOND));
		// change the seconds to the actual amount we're using
		NumSeconds.Format("%.2f", ((float)atoi(NumFrames))/FRAMES_PER_SECOND);
		UpdateData(false);
	}
	if(!CValidEdit::DialogHasInvalid(IDD)) {
		((LinearMotion*)motion)->SetNumFrames(atoi(NumFrames));
		motion->CreateMotion();
		tabControl->displayDialog.setMotion(motion);
	}
	UpdateData(false);
}

void MotionInterpolationDialog::OnEnKillfocusSpecifyFrames()
{
	UpdateData(true);
	if(LengthFrames.Validate()) {
		NumSeconds.Format("%.2f", ((float)atoi(NumFrames))/FRAMES_PER_SECOND);
		UpdateData(false);
	}
	if(!CValidEdit::DialogHasInvalid(IDD)) {
		((LinearMotion*)motion)->SetNumFrames(atoi(NumFrames));
		motion->CreateMotion();
		tabControl->displayDialog.setMotion(motion);
	}
	UpdateData(false);
}
