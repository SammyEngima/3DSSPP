// Dg_2D_BodyAngles.cpp : implementation file
// 

#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include "UndoPosture.h"
#include "Dg_2D_BodyAngles.h"

#include <string>
#include ".\dg_2d_bodyangles.h"

using std::string;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_2D_BodyAngles dialog

//---------------------------------------------------------------------------
Dg_2D_BodyAngles::Dg_2D_BodyAngles(C_Hom_Doc* aDocPtr,
                                   CWnd* pParent) :
   CDialog(Dg_2D_BodyAngles::IDD, pParent),
   mDocPtr(aDocPtr),
   mCurrentControlID(0),
   mIncrementGroup(0)
   , ForearmBackward(FALSE)
   , UpperarmBackward(FALSE)
   , UpperlegBackward(FALSE)
   , LowerlegBackward(FALSE)
   {
   //{{AFX_DATA_INIT(Dg_2D_BodyAngles)
   //if(aDocPtr->getPositioning() & 1) NeutralButtonCaption = "Neutral Sit";
   if(aDocPtr->getPosition() == P_Sitting) NeutralButtonCaption = "Neutral Sit";
   else NeutralButtonCaption = "Neutral Stand";
    
   mIncrementGroup = 2;
   mLowerLeg = 0;
   mTrunkFlexion = 0;
   mUpperLeg = 0;
   mForeArm = 0;
   mUpperArm = 0;
   //}}AFX_DATA_INIT

   this->GetCurrentJointAnglesFromDocument();
}

Dg_2D_BodyAngles::~Dg_2D_BodyAngles() {
	mDocPtr->cancelCue();
}

//---------------------------------------------------------------------------
void Dg_2D_BodyAngles::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_2D_BodyAngles)
	DDX_Control(pDX, IDC_UpperLeg, mUpperLegControl);
	DDX_Control(pDX, IDC_UpperArm, mUpperArmControl);
	DDX_Control(pDX, IDC_TrunkFlexion, mTrunkFlexionControl);
	DDX_Control(pDX, IDC_LowerLeg, mLowerLegControl);
	DDX_Control(pDX, IDC_ForeArm, mForeArmControl);
	DDX_Radio(pDX, IDC_IncrementGroup, mIncrementGroup);
	DDX_Text(pDX, IDC_NeutralButton, NeutralButtonCaption);
	DDX_Text(pDX, IDC_LowerLeg, mLowerLeg);
	DDX_Text(pDX, IDC_TrunkFlexion, mTrunkFlexion);
	DDX_Text(pDX, IDC_UpperLeg, mUpperLeg);
	DDX_Text(pDX, IDC_ForeArm, mForeArm);
	DDX_Text(pDX, IDC_UpperArm, mUpperArm);
	DDX_Control(pDX, IDC_UndoButton, mUndoButtonControl);
	DDX_Control(pDX, IDC_RedoButton, mRedoButtonControl);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_FOREARM_BACKWARD_2D, ForearmBackward);
	DDX_Check(pDX, IDC_UPPERARM_BACKWARD_2D, UpperarmBackward);
	DDX_Check(pDX, IDC_UPPERLEG_BACKWARD_2D, UpperlegBackward);
	DDX_Check(pDX, IDC_LOWERLEG_BACKWARD_2D, LowerlegBackward);
}

//---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(Dg_2D_BodyAngles, CDialog)
   //{{AFX_MSG_MAP(Dg_2D_BodyAngles)
   ON_BN_CLICKED(IDC_IncrementMinusButton, OnIncrementMinusButton)
   ON_BN_CLICKED(IDC_IncrementPlusButton, OnIncrementPlusButton)
   ON_BN_CLICKED(IDC_UndoButton, OnUndoButton)
   ON_BN_CLICKED(IDC_RedoButton, OnBnClickedRedoButton)
   ON_BN_CLICKED(IDC_RedrawButton, OnRedrawButton)
   ON_BN_CLICKED(IDC_NeutralButton, OnNeutralButton)
   ON_EN_SETFOCUS(IDC_TrunkFlexion, OnSetfocusTrunkFlexion)
   ON_EN_SETFOCUS(IDC_UpperArm, OnSetfocusUpperArm)
   ON_EN_SETFOCUS(IDC_UpperLeg, OnSetfocusUpperLeg)
   ON_EN_SETFOCUS(IDC_LowerLeg, OnSetfocusLowerLeg)
   ON_EN_SETFOCUS(IDC_ForeArm, OnSetfocusForeArm)
   //}}AFX_MSG_MAP
   ON_BN_CLICKED(IDC_FOREARM_BACKWARD_2D, &Dg_2D_BodyAngles::OnBnClickedForearmBackward2d)
   ON_BN_CLICKED(IDC_UPPERARM_BACKWARD_2D, &Dg_2D_BodyAngles::OnBnClickedUpperarmBackward2d)
   ON_BN_CLICKED(IDC_UPPERLEG_BACKWARD_2D, &Dg_2D_BodyAngles::OnBnClickedUpperlegBackward2d)
   ON_BN_CLICKED(IDC_LOWERLEG_BACKWARD_2D, &Dg_2D_BodyAngles::OnBnClickedLowerlegBackward2d)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_2D_BodyAngles support
   
//---------------------------------------------------------------------------
void Dg_2D_BodyAngles::GetCurrentJointAnglesFromDocument()
{
	JointAngles currentAngles = mDocPtr->GetSkeleton()->GetAngles();
   //unsigned int frame = mDocPtr->GetSkeleton().getCurrentFrame();
   //mDocPtr->GetSkeleton().GetAngles(frame,currentAngles);
   this->SetJointAngles(currentAngles);
}

//---------------------------------------------------------------------------
void Dg_2D_BodyAngles::SetJointAngles(const JointAngles& aJointAnglesRef)
{
	JointAngles TwoDVersion = aJointAnglesRef;
//	TwoDVersion.Set2D();
   mForeArm       =  TwoDVersion[A_ForearmRV];   
   mUpperArm      =  TwoDVersion[A_UpperarmRV];
   mUpperLeg      =  TwoDVersion[A_UpperlegRV];
   mLowerLeg      =  TwoDVersion[A_LowerlegRV];
   mTrunkFlexion  =  TwoDVersion[A_TrunkFlexion];

   ForearmBackward = (TwoDVersion[A_ForearmRH] < 0);
   UpperarmBackward = (TwoDVersion[A_UpperarmRH] < 0);
   UpperlegBackward = (TwoDVersion[A_UpperlegRH] < 0);
   LowerlegBackward = (TwoDVersion[A_LowerlegRH] < 0);
}

bool Dg_2D_BodyAngles::SameAsDocument()
{
   //unsigned int frame = mDocPtr->GetSkeleton().getCurrentFrame();
	JointAngles docJointAngles = mDocPtr->GetSkeleton()->GetAngles();
   //mDocPtr->GetSkeleton().GetAngles(frame,docJointAngles);

   return (docJointAngles[A_ForearmRV] == mForeArm
      && docJointAngles[A_ForearmLV] == mForeArm
      && docJointAngles[A_UpperarmRV] == mUpperArm
	  && docJointAngles[A_UpperarmLV] == mUpperArm
      && docJointAngles[A_UpperlegRV] == mUpperLeg
      && docJointAngles[A_UpperlegLV] == mUpperLeg
      && docJointAngles[A_LowerlegRV] == mLowerLeg
      && docJointAngles[A_LowerlegLV] == mLowerLeg
      && docJointAngles[A_TrunkFlexion] == mTrunkFlexion
	  && (docJointAngles[A_ForearmRH] < 0) == ForearmBackward
	  && (docJointAngles[A_UpperarmRH] < 0) == UpperarmBackward
	  && (docJointAngles[A_UpperlegRH] < 0) == UpperlegBackward
	  && (docJointAngles[A_LowerlegRH] < 0) == LowerlegBackward);
}

//---------------------------------------------------------------------------
bool Dg_2D_BodyAngles::UpdateDocument()
{
    if(SameAsDocument())
        // Just get out, nothing's changed
        return true;

    bool lUpdateOK = true;
    this->BeginWaitCursor();

	Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();

    JointAngles newJointAngles,oldAngles;
	//skel.GetAngles(skel.getCurrentFrame(), newJointAngles);
	newJointAngles = skel.GetAngles();
    //newJointAngles.ClearLastErrorMessage();

	newJointAngles[A_ForearmRV] = mForeArm;
	newJointAngles[A_ForearmLV] = mForeArm;
	newJointAngles[A_UpperarmRV] = mUpperArm;
	newJointAngles[A_UpperarmLV] = mUpperArm;
    newJointAngles[A_UpperlegRV] = mUpperLeg;
    newJointAngles[A_UpperlegLV] = mUpperLeg;
    newJointAngles[A_LowerlegRV] = mLowerLeg;
    newJointAngles[A_LowerlegLV] = mLowerLeg;
    newJointAngles[A_TrunkFlexion] = mTrunkFlexion;
	newJointAngles[A_ForearmRH] = newJointAngles[A_ForearmLH] = ForearmBackward ? -90 : 90;
	newJointAngles[A_UpperarmRH] = newJointAngles[A_UpperarmLH] = UpperarmBackward ? -90 : 90;
	newJointAngles[A_UpperlegRH] = newJointAngles[A_UpperlegLH] = UpperlegBackward ? -90 : 90;
	newJointAngles[A_LowerlegRH] = newJointAngles[A_LowerlegLH] = LowerlegBackward ? -90 : 90;
//	newJointAngles.Set2D();

//	string errors = skel.CheckAngles(newJointAngles);
	string errors; // declaring to compile. this code is no longer used

	if (errors.empty()) //attribute validation succeeded
    {
		//unsigned int frame = mDocPtr->GetSkeleton().getCurrentFrame();
		int frame = mDocPtr->getCurrentFrame();
		//mDocPtr->GetSkeleton().GetAngles(frame,oldAngles);
		oldAngles = mDocPtr->GetSkeleton()->GetAngles();
		//mDocPtr->GetSkeleton().SetAngles(frame,newJointAngles);
		mDocPtr->GetSkeleton()->SetAngles(newJointAngles);
		UndoablePostureChange *undoEvent = new UndoablePostureChange(frame,oldAngles,newJointAngles);
		mDocPtr->addUndoEvent(undoEvent);
		mDocPtr->MakeDirtyAndUpdateViews();
    }
    else  // attribute validation failed
    {
        lUpdateOK = false;
		AfxMessageBox(errors.c_str());
    }

    this->EndWaitCursor();

    UndoButtonStatus();
    RedoButtonStatus();

    return lUpdateOK;
} 

//---------------------------------------------------------------------------
void Dg_2D_BodyAngles::JogAngle(bool increasing)
{
   int lDirection;

   if(increasing)
      lDirection = 1;
   else	// Decreasing
      lDirection = -1;

   if (mCurrentControlID == 0)
   {
      ::MessageBox(NULL,
         "No joint angle active.",
         "Increment Angle",
         MB_OK | MB_ICONEXCLAMATION);
      return;
   }

   if (!this->UpdateData(TRUE))  // Make sure data OK
      return;

   BOOL lValue_OK;
   int Value = GetDlgItemInt( mCurrentControlID, &lValue_OK );
   int Prior_Value = Value;
   
   if (!lValue_OK)
   {
      ::MessageBox(NULL,
         "Active joint angle contains illegal data.\n"
         "Joint angles must be integers.",
         "Increment Angle",
         MB_OK | MB_ICONEXCLAMATION);
      return;
   }

   if (mIncrementGroup == 0) //1 increment selected
      Value = Value + lDirection;
   else
      Value = Value + 5 * lDirection * ( mIncrementGroup );
   
   SetDlgItemInt( mCurrentControlID, Value );
   
   if (this->UpdateData(TRUE))
		//	Data OK so redraw 
      this->OnRedrawButton(); 
   else
		//	Restore object value to prior value
      SetDlgItemInt( mCurrentControlID, Prior_Value );

	return;
}

/////////////////////////////////////////////////////////////////////////////
// Dg_2D_BodyAngles message handlers

//---------------------------------------------------------------------------
void Dg_2D_BodyAngles::OnIncrementMinusButton() 
{
   this->JogAngle(false);
}

//---------------------------------------------------------------------------
void Dg_2D_BodyAngles::OnIncrementPlusButton() 
{
   this->JogAngle(true);
}

//---------------------------------------------------------------------------
void Dg_2D_BodyAngles::OnUndoButton() 
{
	if(!mDocPtr->UndoJointAngleChange())
		return;

	//unsigned int frame = mDocPtr->GetSkeleton().getCurrentFrame();
	JointAngles docJointAngles = mDocPtr->GetSkeleton()->GetAngles();
    //mDocPtr->GetSkeleton().GetAngles(frame,docJointAngles);
    this->SetJointAngles(docJointAngles);

    this->CWnd::UpdateData(FALSE);	//Transfer successful 
	
	// Do NOT update the document; the Undo function has already done the work.
   
	UndoButtonStatus();
    RedoButtonStatus();
}

//---------------------------------------------------------------------------
void Dg_2D_BodyAngles::OnBnClickedRedoButton()
{
	if(!mDocPtr->RedoJointAngleChange())
		return;

	//unsigned int frame = mDocPtr->GetSkeleton().getCurrentFrame();
	JointAngles docJointAngles = mDocPtr->GetSkeleton()->GetAngles();
    //mDocPtr->GetSkeleton().GetAngles(frame,docJointAngles);
    this->SetJointAngles(docJointAngles);

    this->CWnd::UpdateData(FALSE);	//Transfer successful 
	
	// Do NOT update the document; the Undo function has already done the work.
   
	UndoButtonStatus();
    RedoButtonStatus();
}

// Disable/enable the undo button when necessary
void Dg_2D_BodyAngles::UndoButtonStatus()
{
	mUndoButtonControl.EnableWindow(mDocPtr->CanUndo());
}

// Disable/enable the redo button when necessary
void Dg_2D_BodyAngles::RedoButtonStatus()
{
	mRedoButtonControl.EnableWindow(mDocPtr->CanRedo());
}

//---------------------------------------------------------------------------
void Dg_2D_BodyAngles::OnRedrawButton() 
{
   if (this->CWnd::UpdateData(TRUE /*Retrieve*/)) //Data transfer successful 
		UpdateDocument();
}

//---------------------------------------------------------------------------
void Dg_2D_BodyAngles::OnNeutralButton() 
{
   this->SetJointAngles(mDocPtr->getNeutralJointAngles());

   if (this->CWnd::UpdateData(FALSE /*Put to dialog*/)) //Transfer successful 
      this->UpdateDocument();
}

//---------------------------------------------------------------------------
void Dg_2D_BodyAngles::OnOK() 
{
   if (this->CWnd::UpdateData(TRUE /*Retrieve*/)) //Data transfer successful
      if (this->UpdateDocument())
         CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// Message handlers for setting control focus for increment/decrement
// operations
//

//---------------------------------------------------------------------------
void Dg_2D_BodyAngles::OnSetfocusTrunkFlexion() 
{
   mCurrentControlID =  IDC_TrunkFlexion;
   mDocPtr->cueAngle(AngleCueData(A_TrunkFlexion, JT_HIP_CENTER, JT_C7T1, AT_TrunkF));
}

//---------------------------------------------------------------------------
void Dg_2D_BodyAngles::OnSetfocusUpperArm() 
{
   mCurrentControlID =  IDC_UpperArm;
   mDocPtr->cueAngle(AngleCueData(A_UpperarmLV, JT_LSHOULDER, JT_LELBOW, AT_LimbVL, A_UpperarmLH));
}

//---------------------------------------------------------------------------
void Dg_2D_BodyAngles::OnSetfocusUpperLeg() 
{
   mCurrentControlID =  IDC_UpperLeg;
   mDocPtr->cueAngle(AngleCueData(A_UpperlegRV, JT_RHIP, JT_RKNEE, AT_LimbVR, A_UpperlegRH));
}

//---------------------------------------------------------------------------
void Dg_2D_BodyAngles::OnSetfocusLowerLeg() 
{
   mCurrentControlID =  IDC_LowerLeg;
   mDocPtr->cueAngle(AngleCueData(A_LowerlegRV, JT_RKNEE, JT_RANKLE, AT_LimbVR, A_LowerlegRH));
}

//---------------------------------------------------------------------------
void Dg_2D_BodyAngles::OnSetfocusForeArm() 
{
   mCurrentControlID =  IDC_ForeArm;
   mDocPtr->cueAngle(AngleCueData(A_ForearmRV, JT_RELBOW, JT_RWRIST, AT_LimbVR, A_ForearmRH));
}

void Dg_2D_BodyAngles::OnBnClickedForearmBackward2d()
{
	this->mCurrentControlID = 0; // effectively disable increment / decrement buttons
	mDocPtr->cueAngle(AngleCueData(A_ForearmRH, JT_RELBOW, JT_RWRIST, AT_HorizontalPosNeg, A_ForearmRV));
}

void Dg_2D_BodyAngles::OnBnClickedUpperarmBackward2d()
{
	this->mCurrentControlID = 0;
	mDocPtr->cueAngle(AngleCueData(A_UpperarmRH, JT_RSHOULDER, JT_RELBOW, AT_HorizontalPosNeg, A_UpperarmRV));
}

void Dg_2D_BodyAngles::OnBnClickedUpperlegBackward2d()
{
	this->mCurrentControlID = 0;
	mDocPtr->cueAngle(AngleCueData(A_UpperlegRH, JT_RHIP, JT_RKNEE, AT_HorizontalPosNeg, A_UpperlegRV));
}

void Dg_2D_BodyAngles::OnBnClickedLowerlegBackward2d()
{
	this->mCurrentControlID = 0;
	mDocPtr->cueAngle(AngleCueData(A_LowerlegRH, JT_RKNEE, JT_RANKLE, AT_HorizontalPosNeg, A_LowerlegRV));
}


//---------------------------------------------------------------------------
BOOL Dg_2D_BodyAngles::OnInitDialog()
{
	CDialog::OnInitDialog();

	UndoButtonStatus();
    RedoButtonStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
}
