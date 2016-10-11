// MotionHandlingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MotionHandlingDialog.h"
#include "MorphedPhaseMotion.h"
#include "Hom.h"
#include "Hom_Doc.h"
#include "PosturePrediction.h"
#include "MotionFrameDisplayDialog.h"
#include "Analysis.h"
#include "MotionTabCtrl.h"
#include <sstream>
#include <string>

using std::ostringstream; using std::string;

// MotionHandlingDialog dialog

IMPLEMENT_DYNAMIC(MotionHandlingDialog, CMotionDialogTab)

MotionHandlingDialog::MotionHandlingDialog(CWnd* pParent /*=NULL*/)
	: CMotionDialogTab(MotionHandlingDialog::IDD, pParent)
	, ReachTypeHanded(FALSE)
	, ReachTypeObject(FALSE)
	, InitialPosture(FALSE)
	, InitialX(_T(""))
	, InitialY(_T(""))
	, InitialZ(_T(""))
	, FinalPosture(FALSE)
	, FinalX(_T(""))
	, FinalY(_T(""))
	, FinalZ(_T(""))
	, MotionLength(FALSE)
	, LengthSeconds(_T(""))
	, LengthFrames(_T(""))
	, HandDistX(_T(""))
	, HandDistY(_T(""))
	, HandDistZ(_T(""))
	, InitialXEdit(IDD)
	, InitialYEdit(IDD)
	, InitialZEdit(IDD)
	, FinalXEdit(IDD)
	, FinalYEdit(IDD)
	, FinalZEdit(IDD)
	, LengthFramesEdit(IDD,4,1000)
	, LengthSecondsEdit(IDD,0.16,400)
	, HandDistXEdit(IDD)
	, HandDistYEdit(IDD)
	, HandDistZEdit(IDD)
	, OffsetLabel(_T(""))
	, InitialPostureLabel(_T(""))
	, FinalPostureLabel(_T(""))
	, BoxAngle(_T(""))
	, BoxAngleEdit(IDD,-360,360)
	, mFixShoulder(FALSE)
	, mFixHip(FALSE)
	, mShoullenflex(0)
{
	// trying to use this for seated handling too:
	Task* task = &Get_App_Ptr()->Get_Document_Ptr()->getTask();
	if(task->getPosition() == P_Standing) {
		//motion = new StandingHandlingMotion(task);
		motion = new MorphedPhaseMotion(ET_StandingHandling, task);
	} else {
		//motion = new SittingHandlingMotion(task);
		motion = new MorphedPhaseMotion(ET_SittingHandling, task);
	}
	unattainable = false;
}

MotionHandlingDialog::~MotionHandlingDialog()
{
	delete motion;
}

void MotionHandlingDialog::DoDataExchange(CDataExchange* pDX)
{
	CMotionDialogTab::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_REACHTYPE_TWOHANDED, ReachTypeHanded);
	DDX_Radio(pDX, IDC_REACHTYPE_WITHOBJECT, ReachTypeObject);
	DDX_Text(pDX, IDC_HANDDISTX, HandDistX);
	DDX_Text(pDX, IDC_HANDDISTY, HandDistY);
	DDX_Text(pDX, IDC_HANDDISTZ, HandDistZ);
	DDX_Radio(pDX, IDC_INITIAL_LOCATION, InitialPosture);
	DDX_Text(pDX, IDC_INITIAL_LOCATION_X, InitialX);
	DDX_Text(pDX, IDC_INITIAL_LOCATION_Y, InitialY);
	DDX_Text(pDX, IDC_INITIAL_LOCATION_Z, InitialZ);
	DDX_Radio(pDX, IDC_FINAL_LOCATION, FinalPosture);
	DDX_Text(pDX, IDC_FINAL_LOCATION_X, FinalX);
	DDX_Text(pDX, IDC_FINAL_LOCATION_Y, FinalY);
	DDX_Text(pDX, IDC_FINAL_LOCATION_Z, FinalZ);
	DDX_Radio(pDX, IDC_MOTIONLENGTH_PREDICT, MotionLength);
	DDX_Text(pDX, IDC_SPECIFY_SECONDS, LengthSeconds);
	DDX_Text(pDX, IDC_SPECIFY_FRAMES, LengthFrames);
	DDX_Control(pDX, IDC_HANDDISTX, HandDistXEdit);
	DDX_Control(pDX, IDC_HANDDISTY, HandDistYEdit);
	DDX_Control(pDX, IDC_HANDDISTZ, HandDistZEdit);
	DDX_Control(pDX, IDC_INITIAL_LOCATION_X, InitialXEdit);
	DDX_Control(pDX, IDC_INITIAL_LOCATION_Y, InitialYEdit);
	DDX_Control(pDX, IDC_INITIAL_LOCATION_Z, InitialZEdit);
	DDX_Control(pDX, IDC_FINAL_LOCATION_X, FinalXEdit);
	DDX_Control(pDX, IDC_FINAL_LOCATION_Y, FinalYEdit);
	DDX_Control(pDX, IDC_FINAL_LOCATION_Z, FinalZEdit);
	DDX_Control(pDX, IDC_SPECIFY_SECONDS, LengthSecondsEdit);
	DDX_Control(pDX, IDC_SPECIFY_FRAMES, LengthFramesEdit);
	DDX_Text(pDX, IDC_OFFSETLABEL, OffsetLabel);
	DDX_Text(pDX, IDC_INITIAL_POSTURE_LABEL, InitialPostureLabel);
	DDX_Text(pDX, IDC_FINAL_POSTURE_LABEL, FinalPostureLabel);
	DDX_Text(pDX, IDC_HANDLINGBOXANGLE, BoxAngle);
	DDX_Control(pDX, IDC_HANDLINGBOXANGLE, BoxAngleEdit);
	DDX_Check(pDX, IDC_FIXSHOULDER, mFixShoulder);
	DDX_Check(pDX, IDC_FIXPELVIS, mFixHip);
	DDX_Text(pDX, IDC_SHOULLENFLEX, mShoullenflex);
}
bool MotionHandlingDialog::membersToMotion() {
	MorphedPhaseMotion* pmotion = (MorphedPhaseMotion*)motion;
	C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();


	/*pmotion->SetShoulderFix(mFixShoulder);
	pmotion->SetShoulLenFlex(mShoullenflex);
	pmotion->SetHipFix(mFixHip);*/
	pmotion->SetShoulderFix(true);
	pmotion->SetShoulLenFlex(0.05);
	pmotion->SetHipFix(false);

	if(ReachTypeHanded == 0)
		pmotion->SetExpType('b');
	else if(ReachTypeHanded == 1)
		pmotion->SetExpType('v');
	else if(ReachTypeHanded == 2)
		pmotion->SetExpType('h');
	
	InitialXEdit.EnableWindow(InitialPosture == 0);
	InitialYEdit.EnableWindow(InitialPosture == 0);
	InitialZEdit.EnableWindow(InitialPosture == 0);

	FinalXEdit.EnableWindow(FinalPosture == 0);
	FinalYEdit.EnableWindow(FinalPosture == 0);
	FinalZEdit.EnableWindow(FinalPosture == 0);

	pmotion->SetBox(atof(HandDistX), 30, 30, atof(BoxAngle));

	pmotion->SetWithObject(ReachTypeObject == 0);

	Vector3 handOffset(atof(HandDistX), atof(HandDistY), atof(HandDistZ));
	Vector3 initTarget(atof(InitialX), atof(InitialY), atof(InitialZ));
	Vector3 finalTarget(atof(FinalX), atof(FinalY), atof(FinalZ));

	pmotion->SetHandDiff(handOffset);

	Task* task = pmotion->GetTask();
	int curFrame = task->getCurrentFrame();
	// always set initial frame to get the forces & moments
	pmotion->SetInitialFrame(*task->getSkeletonAtFrame(curFrame));
	if(InitialPosture == 0) {
		pmotion->SetDefaultInitial(initTarget);
	} else {
		pmotion->SetNotDefaultInitial();
		pmotion->SetInitialFrame(*task->getSkeletonAtFrame(curFrame));
	}
	if(FinalPosture == 0) {
		pmotion->SetDefaultFinal(finalTarget);
	} else {
		pmotion->SetNotDefaultFinal();
		pmotion->SetFinalFrame(*task->getSkeletonAtFrame(curFrame + 1));
	}

	if(MotionLength == 0) {
		pmotion->SetPredictTime(true);
	} else {
		pmotion->SetPredictTime(false);
		pmotion->SetNumFramesPerPhase(atoi(LengthFrames));
	}
	return true;
}

void MotionHandlingDialog::motionToMembers() {
	
	MorphedPhaseMotion* pmotion = (MorphedPhaseMotion*)motion;
	C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();

	char type = pmotion->GetExpType();
	if(type == 'b')
		ReachTypeHanded = 0;
	else if(type == 'v')
		ReachTypeHanded = 1;
	else if(type == 'h')
		ReachTypeHanded = 2;
	double width, depth, height, angle;
	Vector3 handDiff = pmotion->GetHandDiff();
	HandDistX.Format("%.1f", handDiff[0]);
	HandDistY.Format("%.1f", handDiff[1]);
	HandDistZ.Format("%.1f", handDiff[2]);

	BoxAngle.Format("0");

	Skeleton& init = *pmotion->GetInitialFrame();
	Vector3 initLocation;
	if(pmotion->GetDefaultInitial()) {
		initLocation = pmotion->GetInitialTarget();
	} else {
		if(type == 'b') {
			initLocation = (init.getJoint(JT_RHAND) + init.getJoint(JT_LHAND))/2;
		} else {
			initLocation = init.getJoint(JT_RHAND);
		}
		//initLocation -= Get_App_Ptr()->Get_Document_Ptr()->getOffset(&init);
	}
	InitialX.Format("%.1f", initLocation[0]);
	InitialY.Format("%.1f", initLocation[1]);
	InitialZ.Format("%.1f", initLocation[2]);

	Skeleton& final = *pmotion->GetFinalFrame();
	Vector3 finalLocation;
	if(pmotion->GetDefaultFinal()) {
		finalLocation = pmotion->GetFinalTarget();
	} else {
		if(type == 'b') {
			finalLocation = (final.getJoint(JT_RHAND) + init.getJoint(JT_LHAND))/2;
		} else {
			finalLocation = final.getJoint(JT_RHAND);
		}
		//finalLocation -= Get_App_Ptr()->Get_Document_Ptr()->getOffset(&final);
	}
	FinalX.Format("%.1f", finalLocation[0]);
	FinalY.Format("%.1f", finalLocation[1]);
	FinalZ.Format("%.1f", finalLocation[2]);

	if(pmotion->GetPredictTime()) {
		MotionLength = 0;
	} else {
		MotionLength = 1;
	}
	this->GetDlgItem(IDC_SPECIFY_SECONDS)->EnableWindow(MotionLength == 1);
	this->GetDlgItem(IDC_SPECIFY_FRAMES)->EnableWindow(MotionLength == 1);
	int frames = pmotion->GetNumFramesPerPhase();
	LengthSeconds.Format("%.2f", 1.0 * frames / FRAMES_PER_SECOND);
	LengthFrames.Format("%d", frames);
}

void MotionHandlingDialog::motionChanged() {
	if(CValidEdit::DialogHasInvalid(IDD))
		return;
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
	}
}

BEGIN_MESSAGE_MAP(MotionHandlingDialog, CMotionDialogTab)
	ON_BN_CLICKED(IDC_REACHTYPE_TWOHANDED, &MotionHandlingDialog::OnBnClickedReachtypeTwohanded)
	ON_BN_CLICKED(IDC_REACHTYPE_ONEHANDEDNEUTRAL, &MotionHandlingDialog::OnBnClickedReachtypeOnehandedneutral)
	ON_BN_CLICKED(IDC_REACHTYPE_ONEHANDEDPRONE, &MotionHandlingDialog::OnBnClickedReachtypeOnehandedprone)
	ON_BN_CLICKED(IDC_REACHTYPE_WITHOBJECT, &MotionHandlingDialog::OnBnClickedReachtypeWithobject)
	ON_BN_CLICKED(IDC_REACHTYPE_WITHOUTOBJECT, &MotionHandlingDialog::OnBnClickedReachtypeWithoutobject)
	ON_EN_KILLFOCUS(IDC_HANDDISTX, &MotionHandlingDialog::OnEnKillfocusHanddistx)
	ON_EN_KILLFOCUS(IDC_HANDDISTY, &MotionHandlingDialog::OnEnKillfocusHanddisty)
	ON_EN_KILLFOCUS(IDC_HANDDISTZ, &MotionHandlingDialog::OnEnKillfocusHanddistz)
	ON_BN_CLICKED(IDC_INITIAL_LOCATION, &MotionHandlingDialog::OnBnClickedInitialLocation)
	ON_BN_CLICKED(IDC_INITIAL_SELECTED, &MotionHandlingDialog::OnBnClickedInitialSelected)
	ON_EN_KILLFOCUS(IDC_INITIAL_LOCATION_X, &MotionHandlingDialog::OnEnKillfocusInitialLocationX)
	ON_EN_KILLFOCUS(IDC_INITIAL_LOCATION_Y, &MotionHandlingDialog::OnEnKillfocusInitialLocationY)
	ON_EN_KILLFOCUS(IDC_INITIAL_LOCATION_Z, &MotionHandlingDialog::OnEnKillfocusInitialLocationZ)
	ON_BN_CLICKED(IDC_FINAL_LOCATION, &MotionHandlingDialog::OnBnClickedFinalLocation)
	ON_BN_CLICKED(IDC_FINAL_SELECTED, &MotionHandlingDialog::OnBnClickedFinalSelected)
	ON_EN_KILLFOCUS(IDC_FINAL_LOCATION_X, &MotionHandlingDialog::OnEnKillfocusFinalLocationX)
	ON_EN_KILLFOCUS(IDC_FINAL_LOCATION_Y, &MotionHandlingDialog::OnEnKillfocusFinalLocationY)
	ON_EN_KILLFOCUS(IDC_FINAL_LOCATION_Z, &MotionHandlingDialog::OnEnKillfocusFinalLocationZ)
	ON_BN_CLICKED(IDC_MOTIONLENGTH_PREDICT, &MotionHandlingDialog::OnBnClickedMotionlengthPredict)
	ON_BN_CLICKED(IDC_MOTIONLENGTH_SPECIFY, &MotionHandlingDialog::OnBnClickedMotionlengthSpecify)
	ON_EN_KILLFOCUS(IDC_SPECIFY_SECONDS, &MotionHandlingDialog::OnEnKillfocusSpecifySeconds)
	ON_EN_KILLFOCUS(IDC_SPECIFY_FRAMES, &MotionHandlingDialog::OnEnKillfocusSpecifyFrames)
	ON_EN_KILLFOCUS(IDC_HANDLINGBOXANGLE, &MotionHandlingDialog::OnEnKillfocusHandlingboxangle)
	ON_BN_CLICKED(IDC_FIXSHOULDER, &MotionHandlingDialog::OnBnClickedFixshoulder)
	ON_BN_CLICKED(IDC_FIXPELVIS, &MotionHandlingDialog::OnBnClickedFixpelvis)
	ON_EN_KILLFOCUS(IDC_SHOULLENFLEX, &MotionHandlingDialog::OnEnKillfocusShoullenflex)
END_MESSAGE_MAP()


// MotionHandlingDialog message handlers

BOOL MotionHandlingDialog::OnInitDialog()
{
	C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();

	OffsetLabel.Format("Offset Between Hands (%s)", pDoc->LengthUnits());
	InitialPostureLabel.Format("Initial Posture (%s)", pDoc->LengthUnits());
	FinalPostureLabel.Format("Final Posture (%s)", pDoc->LengthUnits());

	CMotionDialogTab::OnInitDialog();

	InitialPosture = 0;
	FinalPosture = 0;
	ostringstream label;
	int curFrame = motion->GetTask()->getCurrentFrame();
	label << "Selected Frame (" << curFrame << ")";
	this->SetDlgItemText(IDC_INITIAL_SELECTED, label.str().c_str());
	if(motion->GetTask()->getFrameType(curFrame) != FT_Keyframe) {
		this->GetDlgItem(IDC_INITIAL_SELECTED)->EnableWindow(false);
	}
	label.str("");
	label << "Next Frame (" << curFrame + 1 << ")";
	this->SetDlgItemText(IDC_FINAL_SELECTED, label.str().c_str());
	if(motion->GetTask()->getFrameType(curFrame + 1) != FT_Keyframe) {
		this->GetDlgItem(IDC_FINAL_SELECTED)->EnableWindow(false);
	}

	/*label.str("");
	label << "Offset Between Hands (" << pDoc->LengthUnits() << ")";
	this->SetDlgItemText(IDC_OFFSETLABEL, label.str().c_str());
	this->UpdateWindow();

	label.str("");
	label << "Initial Posture (" << pDoc->LengthUnits() << ")";
	this->SetDlgItemText(IDC_INITIAL_POSTURE_LABEL, label.str().c_str());

	label.str("");
	label << "Final Posture (" << pDoc->LengthUnits() << ")";
	this->SetDlgItemText(IDC_FINAL_POSTURE_LABEL, label.str().c_str());*/

	motionToMembers();
	MorphedPhaseMotion* pmotion = (MorphedPhaseMotion*)motion;
	InitialX.Format("%d", (int)(atof(InitialX)));
	InitialY.Format("%d", (int)(atof(InitialY)));
	InitialZ.Format("%d", (int)(atof(InitialZ)));
	FinalX.Format("%d", (int)(atof(FinalX)));
	FinalY.Format("%d", (int)(atof(FinalY)));
	FinalZ.Format("%d", (int)(atof(FinalZ)));
	UpdateData(false);
	motionChanged();
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void MotionHandlingDialog::OnBnClickedReachtypeTwohanded()
{
	motionChanged();
}

void MotionHandlingDialog::OnBnClickedReachtypeOnehandedneutral()
{
	motionChanged();
}

void MotionHandlingDialog::OnBnClickedReachtypeOnehandedprone()
{
	motionChanged();
}

void MotionHandlingDialog::OnBnClickedReachtypeWithobject()
{
	motionChanged();
}

void MotionHandlingDialog::OnBnClickedReachtypeWithoutobject()
{
	motionChanged();
}

void MotionHandlingDialog::OnEnKillfocusHanddistx()
{
	UpdateData(true);
	motionChanged();
}

void MotionHandlingDialog::OnEnKillfocusHanddisty()
{
	UpdateData(true);
	motionChanged();
}

void MotionHandlingDialog::OnEnKillfocusHanddistz()
{
	UpdateData(true);
	motionChanged();
}

void MotionHandlingDialog::OnBnClickedInitialLocation()
{
	motionChanged();
}

void MotionHandlingDialog::OnBnClickedInitialSelected()
{
	UpdateData(true);
	Skeleton* skel = motion->GetTask()->getSkeleton();
	Vector3 target;
	if(ReachTypeHanded == 0) {
		target = (skel->getJoint(JT_RHAND) + skel->getJoint(JT_LHAND))/2;
	} else {
		target = skel->getJoint(JT_RHAND);
	}
	target -= Get_App_Ptr()->Get_Document_Ptr()->getOffset(skel);
	InitialX.Format("%.1f", target[0]);
	InitialY.Format("%.1f", target[1]);
	InitialZ.Format("%.1f", target[2]);
	UpdateData(false);
	motionChanged();
}

void MotionHandlingDialog::OnEnKillfocusInitialLocationX()
{
	UpdateData(true);
	motionChanged();
}

void MotionHandlingDialog::OnEnKillfocusInitialLocationY()
{
	UpdateData(true);
	motionChanged();
}

void MotionHandlingDialog::OnEnKillfocusInitialLocationZ()
{
	UpdateData(true);
	motionChanged();
}

void MotionHandlingDialog::OnBnClickedFinalLocation()
{
	motionChanged();
}

void MotionHandlingDialog::OnBnClickedFinalSelected()
{
	UpdateData(true);
	Task* task = motion->GetTask();
	Skeleton* skel = task->getSkeletonAtFrame(task->getCurrentFrame() + 1);
	Vector3 target;
	if(ReachTypeHanded == 0) {
		target = (skel->getJoint(JT_RHAND) + skel->getJoint(JT_LHAND))/2;
	} else {
		target = skel->getJoint(JT_RHAND);
	}
	target -= Get_App_Ptr()->Get_Document_Ptr()->getOffset();
	FinalX.Format("%2.f", target[0]);
	FinalY.Format("%2.f", target[1]);
	FinalZ.Format("%2.f", target[2]);
	UpdateData(false);

	motionChanged();
}

void MotionHandlingDialog::OnEnKillfocusFinalLocationX()
{
	UpdateData(true);
	motionChanged();
}

void MotionHandlingDialog::OnEnKillfocusFinalLocationY()
{
	UpdateData(true);
	motionChanged();
}

void MotionHandlingDialog::OnEnKillfocusFinalLocationZ()
{
	UpdateData(true);
	motionChanged();
}

void MotionHandlingDialog::OnBnClickedMotionlengthPredict()
{
	motionChanged();
	PredictedMotion* pmotion = (PredictedMotion*)motion;
	LengthFrames.Format("%d", pmotion->GetNumFrames());
	LengthSeconds.Format("%.2f", 1.0 * pmotion->GetNumFrames() / FRAMES_PER_SECOND);
	UpdateData(false);
	this->GetDlgItem(IDC_SPECIFY_FRAMES)->EnableWindow(false);
	this->GetDlgItem(IDC_SPECIFY_SECONDS)->EnableWindow(false);
}

void MotionHandlingDialog::OnBnClickedMotionlengthSpecify()
{
	motionChanged();
	this->GetDlgItem(IDC_SPECIFY_FRAMES)->EnableWindow(true);
	this->GetDlgItem(IDC_SPECIFY_SECONDS)->EnableWindow(true);
}

void MotionHandlingDialog::OnEnKillfocusSpecifySeconds()
{
	UpdateData(true);
	if(LengthSecondsEdit.IsValid()) {
		LengthFrames.Format("%d", (int)(atof(LengthSeconds)*FRAMES_PER_SECOND));
		// change the seconds to the actual amount we're using
		LengthSeconds.Format("%.2f", ((float)atoi(LengthFrames))/FRAMES_PER_SECOND);
		UpdateData(false);
	}
	motionChanged();
}

void MotionHandlingDialog::OnEnKillfocusSpecifyFrames()
{
	UpdateData(true);
	if(LengthFramesEdit.IsValid()) {
		LengthSeconds.Format("%.2f", ((float)atoi(LengthFrames))/FRAMES_PER_SECOND);
		UpdateData(false);
	}
	motionChanged();
}

void MotionHandlingDialog::OnEnKillfocusHandlingboxangle()
{
	UpdateData(true);
	motionChanged();
}

void MotionHandlingDialog::OnBnClickedFixshoulder()
{
	UpdateData(true);
	motionChanged();
}

void MotionHandlingDialog::OnBnClickedFixpelvis()
{
	UpdateData(true);
	motionChanged();
}

void MotionHandlingDialog::OnEnKillfocusShoullenflex()
{
	UpdateData(true);
	motionChanged();
}
