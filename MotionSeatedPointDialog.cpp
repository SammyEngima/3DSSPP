// MotionSeatedPointDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MotionSeatedPointDialog.h"
#include "MorphedPhaseMotion.h"
#include "MotionTabCtrl.h"
#include "Hom_Doc.h"
#include "Hom.h"


// MotionSeatedPointDialog dialog

IMPLEMENT_DYNAMIC(MotionSeatedPointDialog, CMotionDialogTab)

MotionSeatedPointDialog::MotionSeatedPointDialog(CWnd* pParent)
	: CMotionDialogTab(MotionSeatedPointDialog::IDD, pParent)
	, TargetLocX(_T(""))
	, TargetLocY(_T(""))
	, TargetLocZ(_T(""))
	, ReachType(FALSE)
	, Phase(FALSE)
	, TargetLocationLabel(_T(""))
	, TargetLocXControl(IDD)
	, TargetLocYControl(IDD)
	, TargetLocZControl(IDD)
{
	// trying to use this for seated handling too:
	Task* task = &Get_App_Ptr()->Get_Document_Ptr()->getTask();
	motion = new MorphedPhaseMotion(ET_SittingPoint, task);
}

MotionSeatedPointDialog::~MotionSeatedPointDialog()
{
}

void MotionSeatedPointDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TARGETX, TargetLocX);
	DDX_Text(pDX, IDC_TARGETY, TargetLocY);
	DDX_Text(pDX, IDC_TARGETZ, TargetLocZ);
	DDX_Radio(pDX, IDC_REACHTYPE, ReachType);
	DDX_Radio(pDX, IDC_PHASE, Phase);
	DDX_Control(pDX, IDC_TARGETX, TargetLocXControl);
	DDX_Control(pDX, IDC_TARGETY, TargetLocYControl);
	DDX_Control(pDX, IDC_TARGETZ, TargetLocZControl);
	DDX_Text(pDX, IDC_SEATPOINT_TARGETLABEL, TargetLocationLabel);
}


BEGIN_MESSAGE_MAP(MotionSeatedPointDialog, CMotionDialogTab)
	ON_BN_CLICKED(IDC_PHASE, &MotionSeatedPointDialog::OnBnClickedPhase)
	ON_EN_KILLFOCUS(IDC_TARGETX, &MotionSeatedPointDialog::OnEnKillfocusTargetx)
	ON_EN_KILLFOCUS(IDC_TARGETY, &MotionSeatedPointDialog::OnEnKillfocusTargety)
	ON_EN_KILLFOCUS(IDC_TARGETZ, &MotionSeatedPointDialog::OnEnKillfocusTargetz)
	ON_BN_KILLFOCUS(IDC_REACHTYPE, &MotionSeatedPointDialog::OnBnKillfocusReachtype)
END_MESSAGE_MAP()


// take values from member variables and apply them to the Motion
bool MotionSeatedPointDialog::membersToMotion() {
	MorphedPhaseMotion* pmotion = (MorphedPhaseMotion*)motion;

	// experiment type
	if(ReachType == 0) {
		pmotion->SetExpType('c');
	} else if(ReachType == 1) {
		pmotion->SetExpType('t');
	} else if(ReachType == 2) {
		pmotion->SetExpType('w');
	} else if(ReachType == 3) {
		pmotion->SetExpType('u');
	}

	pmotion->SetDefaultInitial(Vector3(0,0,0), true);
	pmotion->SetDefaultFinal(Vector3(0,0,0), true);

	pmotion->SetTarget(atof(TargetLocX), atof(TargetLocY), atof(TargetLocZ));

	return true;

}
// take values from the Motion and apply them to member variables
void MotionSeatedPointDialog::motionToMembers() {
}
// do work to apply changes that the user makes to the motion
void MotionSeatedPointDialog::motionChanged() {
	if(CValidEdit::DialogHasInvalid(IDD))
		return;
	/*
	UpdateData(true);
	if(membersToMotion()) {
		if(motion->CreateMotion()) {
			MorphedPhaseMotion* pmotion = (MorphedPhaseMotion*)motion;
			tabControl->displayDialog.setMotion(motion);
			Skeleton* temp = pmotion->CreateDefaultSkeleton();
			C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
			//Vector3 target_wrt_lball = pmotion->GetInitialTarget(true) + pDoc->getOffset(temp, true) - temp->getJoint(JT_LBALL, true);
			Vector3 target_wrt_lball = pmotion->GetInitialTarget(true);
			tabControl->displayDialog.getInitialFrameDisplay().setDrawTarget(target_wrt_lball);//pmotion->GetInitialTarget(true));//pmotion->GetTarget(true));
			//target_wrt_lball = pmotion->GetFinalTarget(true) + pDoc->getOffset(temp, true) - temp->getJoint(JT_LBALL, true);
			target_wrt_lball = pmotion->GetFinalTarget(true);
			tabControl->displayDialog.getFinalFrameDisplay().setDrawTarget(target_wrt_lball);//pmotion->GetFinalTarget(true));//pmotion->GetTarget(true));
			delete temp;
			// not showing target on predicted frame
			tabControl->displayDialog.getPredictedFrameDisplay().setNoDrawTarget();

			tabControl->displayDialog.update();
			if(MotionLength == 0) {
				int frames = pmotion->GetNumFrames();
				LengthSeconds.Format("%.2f", 1.0 * frames / FRAMES_PER_SECOND);
				LengthFrames.Format("%d", frames);
				UpdateData(false);
			}
		} else {
			AfxMessageBox(motion->getErrorString().c_str());
		}
	}*/
}

// MotionSeatedPointDialog message handlers

BOOL MotionSeatedPointDialog::OnInitDialog()
{
	CMotionDialogTab::OnInitDialog();

	C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();

	TargetLocX.Format("%d", 30);
	TargetLocY.Format("%d", 80);
	TargetLocZ.Format("%d", 90);

	TargetLocationLabel.Format("Target Location (%s)", pDoc->LengthUnits());

	ReachType = 0;
	Phase = 0;
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void MotionSeatedPointDialog::OnBnClickedPhase() {
	motionChanged();
}

void MotionSeatedPointDialog::OnEnKillfocusTargetx() {
	motionChanged();
}

void MotionSeatedPointDialog::OnEnKillfocusTargety() {
	motionChanged();
}

void MotionSeatedPointDialog::OnEnKillfocusTargetz() {
	motionChanged();
}

void MotionSeatedPointDialog::OnBnKillfocusReachtype() {
	motionChanged();
}
