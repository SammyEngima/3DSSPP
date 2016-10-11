#include "stdafx.h"
#include "Dg_MotionPrediction.h"
#include ".\dg_motionprediction.h"
#include "Hom.h"
#include "Hom_Doc.h"
#include "Analysis.h"

#include "MotionTabCtrlSit.h"
#include "MotionTabCtrlStand.h" 

#include "MotionInterpolationDialog.h"
//#include "MotionReachToGraspDialog.h"
//#include "MotionSeatedHandlingDialog.h"
#include "MotionSeatedPointDialog.h"
//#include "MotionHandlingDialog.h"
#include "Task.h"
#include "Motion.h"
#include <vector>

using std::vector;

IMPLEMENT_DYNAMIC(Dg_MotionPrediction, CDialog)
Dg_MotionPrediction::Dg_MotionPrediction(int initialFrame, CWnd* pParent) : CDialog(Dg_MotionPrediction::IDD, pParent)
{

	C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
	if(pDoc->getPosition() == P_Sitting) {
		mTabs = new CMotionTabCtrlSit(initialFrame, this);
	} else {
		mTabs = new CMotionTabCtrlStand(initialFrame, this);
	}

	savedOffsetChoice = pDoc->getOffsetChoice();
	savedOffsetJoint = pDoc->getOffsetJoint();
	if(pDoc->getPosition() == P_Standing) {
		pDoc->setOffsetChoice(COORD_OFFSET_BODY_PART);
		pDoc->setOffsetJoint(JT_LBALL);
	} else {
		pDoc->setOffsetChoice(NO_OFFSET);
	}
	pDoc->UpdateViews();
}

Dg_MotionPrediction::~Dg_MotionPrediction()
{
	C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
	pDoc->setOffsetChoice(savedOffsetChoice);
	pDoc->setOffsetJoint(savedOffsetJoint);
	pDoc->UpdateViews();
	delete mTabs;
	mTabs = NULL;
}

void Dg_MotionPrediction::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_HandLoads)
	//}}AFX_DATA_MAP
	//DDX_Control(pDX, IDC_TAB1, mTabs);
	DDX_Control(pDX, IDOK, mOKButton);
	DDX_Control(pDX, IDCANCEL, mCancelButton);
}

BEGIN_MESSAGE_MAP(Dg_MotionPrediction, CDialog)
	ON_BN_CLICKED(IDC_HIDDENUPDATE, &Dg_MotionPrediction::OnBnClickedHiddenupdate)
END_MESSAGE_MAP()

void Dg_MotionPrediction::OnOK()
{
	// apply the motion to the task

	// get the motion
	Motion* motion = mTabs->getMotion();
	// just grab the task from the motion instead of Get_App_...
	Task* task = motion->GetTask();
	int curFrame = task->getCurrentFrame();
	// insert the necessary amount of frames (minus one cuz we're using the selected frame)
	// if we're doing linear interpolation, we're using the current frame
	// and the one after so we have to insert two less instead of one less
	int numToInsert;
	/*if(motion->GetFrameType() == FT_Linear) {
		numToInsert = motion->GetNumFrames() - 2;
	} else {
		numToInsert = motion->GetNumFrames() - 1;
	}*/
	if(motion->UsesTwoFrames()) {
		numToInsert = motion->GetNumFrames() - 2;
	} else {
		numToInsert = motion->GetNumFrames() - 1;
	}

	task->insertFrame(numToInsert, curFrame);
	// grab the motion contents
	vector<Skeleton*> contents = motion->GetContents();
	Analysis analysis;
	for(int i = 0; i < contents.size(); i++) {
		analysis.Analyze(contents[i]);
		task->setFrame(curFrame + i, contents[i], motion->GetFrameType());
	}
	// set initial and final frames to be key frames
	task->setFrameType(FT_Keyframe, curFrame);
	task->setFrameType(FT_Keyframe, curFrame + contents.size() - 1);
	CDialog::OnOK();
	Get_App_Ptr()->Get_Document_Ptr()->changeFrame();
	Get_App_Ptr()->Get_Document_Ptr()->MakeDirtyAndUpdateViews(false);

}

BOOL Dg_MotionPrediction::OnInitDialog()
{
	mTabs->SubclassDlgItem(IDC_TAB1, this);

    CDialog::OnInitDialog();

	mTabs->initDialogs();
	mTabs->activateDialogs();

	CRect tabRect;
	mTabs->GetWindowRect(&tabRect);
	const int LEFTSPACE = 10;
	const int TOPSPACE = 70;
	const int MIDDLE = 30;
	this->ScreenToClient(&tabRect);
	mOKButton.SetWindowPos(0, tabRect.left + LEFTSPACE, tabRect.top + TOPSPACE, 0,0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	mCancelButton.SetWindowPos(0, tabRect.left + LEFTSPACE, tabRect.top + TOPSPACE + MIDDLE, 0,0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

	UpdateData(FALSE);
	
	SetDefID(IDC_HIDDENUPDATE);

    return TRUE;
}
void Dg_MotionPrediction::OnCancel()
{
	CDialog::OnCancel();
	Get_App_Ptr()->Get_Document_Ptr()->UpdateAllViews(0);
}

void Dg_MotionPrediction::OnBnClickedHiddenupdate()
{
	this->GetNextDlgTabItem(this->GetFocus())->SetFocus();
}
