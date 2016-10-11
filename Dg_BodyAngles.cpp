// Dg_BodyAngles.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include "UndoPosture.h"

#include "Dg_BodyAngles.h"

#include <string>
#include <sstream>
#include ".\dg_bodyangles.h"

using std::string;
using std::ostringstream;
using std::endl;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_BodyAngles dialog
//---------------------------------------------------------------------------
Dg_BodyAngles::Dg_BodyAngles(	C_Hom_Doc* aDocPtr,
														CWnd* pParent /*=NULL*/	)
	:	CDialog(Dg_BodyAngles::IDD, pParent),
		mDocPtr( aDocPtr ),
		Current_Control_ID( 0 ),
		Increment_Group( 0 )
		, Upperleg_LH(0)
		, Upperleg_RH(0)
		, Lowerleg_LH(0)
		, Lowerleg_RH(0)
{
	//{{AFX_DATA_INIT(Dg_BodyAngles)
    // initialize caption of neutral stance button to corresponding positioning
    //if(aDocPtr->getPositioning() & 1) NeutralButtonCaption = "Neutral Sit";
	if(aDocPtr->getPosition() == P_Sitting) NeutralButtonCaption = "Neutral Sit";
    else NeutralButtonCaption = "Neutral T-Pose";

	Forearm_RH = 0;
	Forearm_RV = 0;
	Lowerleg_LV = 0;
	Lowerleg_RV = 0;
	Trunk_Axial_Rotation = 0;
	Trunk_Flexion = 0;
	Upperarm_LH = 0;
	Upperarm_LV = 0;
	Upperarm_RH = 0;
	Upperarm_RV = 0;
	Upperleg_LV = 0;
	Upperleg_RV = 0;
	Trunk_Lateral_Bending = 0;
	Increment_Group = 2;
	Forearm_LH = 0;
	Forearm_LV = 0;
	//}}AFX_DATA_INIT

	// Read in joint angles
   this->GetCurrentJointAnglesFromDocument();
}
Dg_BodyAngles::~Dg_BodyAngles() {
	mDocPtr->cancelCue();
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_BodyAngles)
	/*DDX_Control(pDX, IDC_UPPERLEG_RV, mRightVerticalUpperlegControl);
	DDX_Control(pDX, IDC_UPPERLEG_LV, mLeftVerticalUpperlegControl);
	DDX_Control(pDX, IDC_UPPERARM_RV, mRightVerticalUpperarmControl);
	DDX_Control(pDX, IDC_UPPERARM_RH, mRightHorizontalUpperarmControl);
	DDX_Control(pDX, IDC_UPPERARM_LV, mLeftVerticalUpperarmControl);
	DDX_Control(pDX, IDC_UPPERARM_LH, mLeftHorizontalUpperarmControl);
	DDX_Control(pDX, IDC_TRUNK_LATERAL_BENDING, mTrunkLateralBendingControl);
	DDX_Control(pDX, IDC_TRUNK_AXIAL_ROTATION, mTrunkAxialRotationControl);
	DDX_Control(pDX, IDC_LOWERLEG_RV, mRightVerticalLowerlegControl);
	DDX_Control(pDX, IDC_LOWERLEG_LV, mLeftVerticalLowerlegControl);
	DDX_Control(pDX, IDC_FOREARM_RV, mRightVerticalForearmControl);
	DDX_Control(pDX, IDC_FOREARM_RH, mRightHorizontalForearmControl);
	DDX_Control(pDX, IDC_FOREARM_LV, mLeftVerticalForearmControl);
	DDX_Control(pDX, IDC_FOREARM_LH, mLeftHorizontalForearmControl);
	DDX_Control(pDX, IDC_TRUNK_FLEXION, mTrunkFlexionControl);*/
	DDX_Text(pDX, IDC_NeutralButton, NeutralButtonCaption);
	DDX_Text(pDX, IDC_FOREARM_RH, Forearm_RH);
	DDX_Text(pDX, IDC_FOREARM_RV, Forearm_RV);
	DDX_Text(pDX, IDC_LOWERLEG_LV, Lowerleg_LV);
	DDX_Text(pDX, IDC_LOWERLEG_RV, Lowerleg_RV);
	DDX_Text(pDX, IDC_TRUNK_AXIAL_ROTATION, Trunk_Axial_Rotation);
	DDX_Text(pDX, IDC_TRUNK_FLEXION, Trunk_Flexion);
	DDX_Text(pDX, IDC_UPPERARM_LH, Upperarm_LH);
	DDX_Text(pDX, IDC_UPPERARM_LV, Upperarm_LV);
	DDX_Text(pDX, IDC_UPPERARM_RH, Upperarm_RH);
	DDX_Text(pDX, IDC_UPPERARM_RV, Upperarm_RV);
	DDX_Text(pDX, IDC_UPPERLEG_LV, Upperleg_LV);
	DDX_Text(pDX, IDC_UPPERLEG_RV, Upperleg_RV);
	DDX_Text(pDX, IDC_TRUNK_LATERAL_BENDING, Trunk_Lateral_Bending);
	DDX_Radio(pDX, IDC_INCREMENT_GROUP, Increment_Group);
	DDX_Text(pDX, IDC_FOREARM_LH, Forearm_LH);
	DDX_Text(pDX, IDC_FOREARM_LV, Forearm_LV);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_UndoButton, mUndoButtonControl);
	DDX_Control(pDX, IDC_RedoButton, mRedoButtonControl);
	DDX_Text(pDX, IDC_UPPERLEG_LH, Upperleg_LH);
	DDX_Text(pDX, IDC_UPPERLEG_RH, Upperleg_RH);
	DDX_Text(pDX, IDC_LOWERLEG_LH, Lowerleg_LH);
	DDX_Text(pDX, IDC_LOWERLEG_RH, Lowerleg_RH);
}

//---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(Dg_BodyAngles, CDialog)
	//{{AFX_MSG_MAP(Dg_BodyAngles)
	ON_BN_CLICKED(IDC_REDRAW, OnRedraw)
	ON_EN_SETFOCUS(IDC_FOREARM_LH, OnSetfocusForearmLh)
	ON_BN_CLICKED(IDC_INCREMENT_PLUS, OnIncrementPlus)
	ON_EN_SETFOCUS(IDC_FOREARM_LV, OnSetfocusForearmLv)
	ON_EN_SETFOCUS(IDC_FOREARM_RH, OnSetfocusForearmRh)
	ON_EN_SETFOCUS(IDC_FOREARM_RV, OnSetfocusForearmRv)
	ON_EN_SETFOCUS(IDC_UPPERARM_LH, OnSetfocusUpperarmLh)
	ON_EN_SETFOCUS(IDC_UPPERARM_LV, OnSetfocusUpperarmLv)
	ON_EN_SETFOCUS(IDC_UPPERARM_RH, OnSetfocusUpperarmRh)
	ON_EN_SETFOCUS(IDC_UPPERARM_RV, OnSetfocusUpperarmRv)
	ON_EN_SETFOCUS(IDC_UPPERLEG_LV, OnSetfocusUpperlegLv)
	ON_EN_SETFOCUS(IDC_UPPERLEG_RV, OnSetfocusUpperlegRv)
	ON_EN_SETFOCUS(IDC_LOWERLEG_LV, OnSetfocusLowerlegLv)
	ON_EN_SETFOCUS(IDC_LOWERLEG_RV, OnSetfocusLowerlegRv)
	ON_EN_SETFOCUS(IDC_TRUNK_FLEXION, OnSetfocusTrunkFlexion)
	ON_EN_SETFOCUS(IDC_TRUNK_AXIAL_ROTATION, OnSetfocusTrunkAxialRotation)
	ON_EN_SETFOCUS(IDC_TRUNK_LATERAL_BENDING, OnSetfocusTrunkLateralBending)
	ON_BN_CLICKED(IDC_INCREMENT_MINUS, OnIncrementMinus)
	ON_BN_CLICKED(IDC_SYMMETRY_R_TO_L, OnSymmetry_R_To_L)
	ON_BN_CLICKED(IDC_SYMMETRY_L_TO_R, OnSymmetry_L_To_R)
	ON_BN_CLICKED(IDC_UndoButton, OnUndoButton)
	ON_BN_CLICKED(IDC_NeutralButton, OnNeutralButton)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_RedoButton, OnBnClickedRedobutton)
	ON_EN_SETFOCUS(IDC_UPPERLEG_LH, &Dg_BodyAngles::OnEnSetfocusUpperlegLh)
	ON_EN_SETFOCUS(IDC_LOWERLEG_LH, &Dg_BodyAngles::OnEnSetfocusLowerlegLh)
	ON_EN_SETFOCUS(IDC_UPPERLEG_RH, &Dg_BodyAngles::OnEnSetfocusUpperlegRh)
	ON_EN_SETFOCUS(IDC_LOWERLEG_RH, &Dg_BodyAngles::OnEnSetfocusLowerlegRh)
	ON_COMMAND(ID_PRINT_PRINTWINDOW, &Dg_BodyAngles::OnPrintPrintwindow)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Overridables

//---------------------------------------------------------------------------
void Dg_BodyAngles::GetCurrentJointAnglesFromDocument() 
{
   /*JointAngles currentAngles;
   unsigned int frame = mDocPtr->GetSkeleton().getCurrentFrame();
   mDocPtr->GetSkeleton().GetAngles(frame,currentAngles);
   this->SetJointAngles(currentAngles);*/
	this->SetJointAngles(mDocPtr->GetSkeleton()->GetAngles());
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::SetJointAngles
	( const JointAngles& aJointAnglesRef )
{
   this->Forearm_LH  =  aJointAnglesRef[A_ForearmLH];
   this->Forearm_LV  =  aJointAnglesRef[A_ForearmLV];
   this->Forearm_RH  =  aJointAnglesRef[A_ForearmRH];
   this->Forearm_RV  =  aJointAnglesRef[A_ForearmRV];
   
   this->Upperarm_LH  =  aJointAnglesRef[A_UpperarmLH];
   this->Upperarm_LV  =  aJointAnglesRef[A_UpperarmLV];
   this->Upperarm_RH  =  aJointAnglesRef[A_UpperarmRH];
   this->Upperarm_RV  =  aJointAnglesRef[A_UpperarmRV];

   this->Upperleg_LV  =  aJointAnglesRef[A_UpperlegLV];
   this->Upperleg_RV  =  aJointAnglesRef[A_UpperlegRV];

   this->Lowerleg_LV  =  aJointAnglesRef[A_LowerlegLV];
   this->Lowerleg_RV  =  aJointAnglesRef[A_LowerlegRV];

   this->Trunk_Flexion          =  aJointAnglesRef[A_TrunkFlexion];
   this->Trunk_Axial_Rotation   =  aJointAnglesRef[A_TrunkAxialRotation];
   this->Trunk_Lateral_Bending  =  aJointAnglesRef[A_TrunkLateralBending];

   this->Upperleg_LH  =  aJointAnglesRef[A_UpperlegLH];
   this->Upperleg_RH  =  aJointAnglesRef[A_UpperlegRH];
   this->Lowerleg_LH  =  aJointAnglesRef[A_LowerlegLH];
   this->Lowerleg_RH  =  aJointAnglesRef[A_LowerlegRH];

}

// Function returns true if document joint angles are no different
//		from dialog member joint angles.
bool Dg_BodyAngles::SameAsDocument()
{
//	unsigned int frame = mDocPtr->GetSkeleton().getCurrentFrame();
	int frame = mDocPtr->getCurrentFrame();
	JointAngles docJointAngles = mDocPtr->GetSkeleton()->GetAngles();
    //mDocPtr->GetSkeleton().GetAngles(frame,docJointAngles);

	return
		(
		docJointAngles[A_ForearmLH] == this->Forearm_LH &&
		docJointAngles[A_ForearmLV] == this->Forearm_LV &&
		docJointAngles[A_ForearmRH] == this->Forearm_RH &&
		docJointAngles[A_ForearmRV] == this->Forearm_RV &&

		docJointAngles[A_UpperarmLH] == this->Upperarm_LH &&
		docJointAngles[A_UpperarmLV] == this->Upperarm_LV &&
		docJointAngles[A_UpperarmRH] == this->Upperarm_RH &&
		docJointAngles[A_UpperarmRV] == this->Upperarm_RV &&

		docJointAngles[A_UpperlegLV] == this->Upperleg_LV &&
		docJointAngles[A_UpperlegRV] == this->Upperleg_RV &&

		docJointAngles[A_LowerlegLV] == this->Lowerleg_LV &&
		docJointAngles[A_LowerlegRV] == this->Lowerleg_RV &&

		docJointAngles[A_TrunkFlexion] == this->Trunk_Flexion &&
		docJointAngles[A_TrunkAxialRotation] == this->Trunk_Axial_Rotation &&
		docJointAngles[A_TrunkLateralBending] == this->Trunk_Lateral_Bending &&

		docJointAngles[A_UpperlegLH] == this->Upperleg_LH &&
		docJointAngles[A_UpperlegRH] == this->Upperleg_RH &&
		docJointAngles[A_LowerlegLH] == this->Lowerleg_LH &&
		docJointAngles[A_LowerlegRH] == this->Lowerleg_RH

		);
}

bool Dg_BodyAngles::UpdateDocument()
{
	if(SameAsDocument())
		return true;	// Do not update; no changes

	bool lUpdateOK = true;

	JointAngles oldAngles,newJointAngles;
	//newJointAngles.ClearLastErrorMessage();
	newJointAngles[A_ForearmLH] = this->Forearm_LH;
	newJointAngles[A_ForearmLV] = this->Forearm_LV;
	newJointAngles[A_ForearmRH] = this->Forearm_RH;
	newJointAngles[A_ForearmRV] = this->Forearm_RV;

	newJointAngles[A_UpperarmLH] = this->Upperarm_LH;
	newJointAngles[A_UpperarmLV] = this->Upperarm_LV;
	newJointAngles[A_UpperarmRH] = this->Upperarm_RH;
	newJointAngles[A_UpperarmRV] = this->Upperarm_RV;

	newJointAngles[A_UpperlegLV] = this->Upperleg_LV;
	newJointAngles[A_UpperlegRV] = this->Upperleg_RV;

	newJointAngles[A_LowerlegLV] = this->Lowerleg_LV;
	newJointAngles[A_LowerlegRV] = this->Lowerleg_RV;

	newJointAngles[A_TrunkFlexion] = this->Trunk_Flexion;
	newJointAngles[A_TrunkAxialRotation] = this->Trunk_Axial_Rotation;
	newJointAngles[A_TrunkLateralBending] = this->Trunk_Lateral_Bending;

	// to make hand angles the same as forearm angles
	// do this before setting leg horizontals because it also 0's them out,
	// but we want the error to come up
	newJointAngles.Set3DStatic();

	newJointAngles[A_UpperlegLH] = this->Upperleg_LH;
	newJointAngles[A_UpperlegRH] = this->Upperleg_RH;
	newJointAngles[A_LowerlegLH] = this->Lowerleg_LH;
	newJointAngles[A_LowerlegRH] = this->Lowerleg_RH;

	Skeleton& skel = *mDocPtr->GetSkeleton();
	//if(!skel.Validate(newJointAngles)) {
	if(!skel.ValidateJointAngles(newJointAngles)) {
		// changed to just checking joint angle limits, not posture angles,
		// so we don't have to check for the hard limit here
		/*if(!skel.HardLimitError()) {
			ostringstream message;
			message << skel.getLastError() << endl << "Continue Anyway?" << endl;
			if(AfxMessageBox(message.str().c_str(), MB_OKCANCEL) == IDCANCEL) {
				return false;
			}
	   } else {*/
		   AfxMessageBox(skel.getLastError().c_str());
		   return false;
	   //}
	}
	int frame = mDocPtr->getCurrentFrame();
	oldAngles = mDocPtr->GetSkeleton()->GetAngles();
	mDocPtr->GetSkeleton()->SetAngles(newJointAngles);
	UndoablePostureChange *undoEvent = new UndoablePostureChange(frame,oldAngles,newJointAngles,mDocPtr->getFrameType(frame), FT_Keyframe);
	mDocPtr->addUndoEvent(undoEvent);
	mDocPtr->MakeDirtyAndUpdateViews();
	// Gray/ungray the Undo and Redo buttons if necessary	
	UndoButtonStatus();
    RedoButtonStatus();


	return lUpdateOK;
} 

/////////////////////////////////////////////////////////////////////////////
// Dg_BodyAngles message handlers

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnNeutralButton() 
{
   this->SetJointAngles(mDocPtr->getNeutralJointAngles());
 
   if (this->CWnd::UpdateData(FALSE /*Put to dialog*/)) //Transfer successful 
	{
		this->UpdateDocument();
	}
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnRedraw()
{
	if (this->CWnd::UpdateData(TRUE /*Retrieve*/)) //Data transfer successful 
      this->UpdateDocument();
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnUndoButton() 
{
	if(!mDocPtr->UndoJointAngleChange())
		return;

	this->GetCurrentJointAnglesFromDocument();

    this->CWnd::UpdateData(FALSE /*Put to dialog*/);

	// Disable the Undo and Redo buttons if necessary	
	UndoButtonStatus();
    RedoButtonStatus();
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnBnClickedRedobutton()
{
	if(!mDocPtr->RedoJointAngleChange())
		return;

	this->GetCurrentJointAnglesFromDocument();

    this->CWnd::UpdateData(FALSE /*Put to dialog*/);

	// Disable the Undo and Redo buttons if necessary	
	UndoButtonStatus();
    RedoButtonStatus();
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnOK() 
{
   if(this->CWnd::UpdateData(TRUE /*Retrieve*/)) //Data transfer successful 
   {
      if(this->UpdateDocument())
         CDialog::OnOK();
   }
}

/*--------------------------------------------------------------*/
void Dg_BodyAngles::OnIncrementPlus()
{
	Increment(true);
}

void Dg_BodyAngles::OnIncrementMinus()
{
	Increment(false);
}

/*--------------------------------------------------------------*/
void Dg_BodyAngles::Increment(bool isIncrementing)
{
	// Sign will cause the function to increment or decrement,
	//		depending on the value of isIncrementing
	int sign;

	if(isIncrementing)
		sign = 1;
	else	// Decrementing
		sign = -1;

	if(this->Current_Control_ID == -1) {
		::MessageBox( NULL, "Angle must be 90 or -90", "Increment/Decrement",MB_OK | MB_ICONEXCLAMATION );
		return;
	}
	if (this->Current_Control_ID == 0)
	{
		::MessageBox(	NULL,
							"No joint angle active.",
							"Increment/Decrement",
							MB_OK | MB_ICONEXCLAMATION	);
		return;
	}

   if(!this->UpdateData(TRUE)) //Make sure data OK
		return;

   BOOL valueOK;
   int newVal = GetDlgItemInt( this->Current_Control_ID, &valueOK );
   int oldVal = newVal;
   if (!valueOK)
	{
		::MessageBox(	NULL,
							"Active joint angle contains illegal data."
							"\nJoint angles must be integers.",
							"Decrement Angle",
							MB_OK | MB_ICONEXCLAMATION	);
		return;
	}

   if (this->Increment_Group == 0)	// "1" selected
      newVal = newVal + sign * 1;
   else
      newVal = newVal + sign * 5 * (this->Increment_Group);
   
   SetDlgItemInt(this->Current_Control_ID, newVal);
   
   if (this->UpdateData(TRUE)) //Data retrieval OK
	{
		// Attempt to update the document
		if (!this->UpdateDocument())
			//	Data validation failed, restore prior value
         SetDlgItemInt(this->Current_Control_ID, oldVal);
	}
   else
		//	Data retrieval failed, restore prior value
      SetDlgItemInt(this->Current_Control_ID, oldVal);
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnSetfocusForearmLh()
{
   this->Current_Control_ID =  IDC_FOREARM_LH;
   mDocPtr->cueAngle(AngleCueData(A_ForearmLH, JT_LELBOW, JT_LWRIST, AT_LimbHL, A_ForearmLV));
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnSetfocusForearmLv()
{
   this->Current_Control_ID = IDC_FOREARM_LV;
   mDocPtr->cueAngle(AngleCueData(A_ForearmLV, JT_LELBOW, JT_LWRIST, AT_LimbVL, A_ForearmLH));
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnSetfocusForearmRh()
{
   this->Current_Control_ID =  IDC_FOREARM_RH;
   mDocPtr->cueAngle(AngleCueData(A_ForearmRH, JT_RELBOW, JT_RWRIST, AT_LimbHR, A_ForearmRV));
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnSetfocusForearmRv()
{
   this->Current_Control_ID =  IDC_FOREARM_RV;
   mDocPtr->cueAngle(AngleCueData(A_ForearmRV, JT_RELBOW, JT_RWRIST, AT_LimbVR, A_ForearmRH));
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnSetfocusUpperarmLh()
{
   this->Current_Control_ID =  IDC_UPPERARM_LH;
   mDocPtr->cueAngle(AngleCueData(A_UpperarmLH, JT_LSHOULDER, JT_LELBOW, AT_LimbHL, A_UpperarmLV));
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnSetfocusUpperarmLv()
{
   this->Current_Control_ID =  IDC_UPPERARM_LV;
   mDocPtr->cueAngle(AngleCueData(A_UpperarmLV, JT_LSHOULDER, JT_LELBOW, AT_LimbVL, A_UpperarmLH));
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnSetfocusUpperarmRh()
{
   this->Current_Control_ID =  IDC_UPPERARM_RH;
   mDocPtr->cueAngle(AngleCueData(A_UpperarmRH, JT_RSHOULDER, JT_RELBOW, AT_LimbHR, A_UpperarmRV));
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnSetfocusUpperarmRv()
{
   this->Current_Control_ID =  IDC_UPPERARM_RV;
   mDocPtr->cueAngle(AngleCueData(A_UpperarmRV, JT_RSHOULDER, JT_RELBOW, AT_LimbVR, A_UpperarmRH));
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnSetfocusUpperlegLv()
{
   this->Current_Control_ID =  IDC_UPPERLEG_LV;
   mDocPtr->cueAngle(AngleCueData(A_UpperlegLV, JT_LHIP, JT_LKNEE, AT_LimbVL, A_UpperlegLH));
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnSetfocusUpperlegRv()
{
   this->Current_Control_ID =  IDC_UPPERLEG_RV;
   mDocPtr->cueAngle(AngleCueData(A_UpperlegRV, JT_RHIP, JT_RKNEE, AT_LimbVR, A_UpperlegRH));
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnSetfocusLowerlegLv()
{
   this->Current_Control_ID =  IDC_LOWERLEG_LV;
   mDocPtr->cueAngle(AngleCueData(A_LowerlegLV, JT_LKNEE, JT_LANKLE, AT_LimbVL, A_LowerlegLH));
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnSetfocusLowerlegRv()
{
   this->Current_Control_ID =  IDC_LOWERLEG_RV;
   mDocPtr->cueAngle(AngleCueData(A_LowerlegRV, JT_RKNEE, JT_RANKLE, AT_LimbVR, A_LowerlegRH));
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnSetfocusTrunkFlexion()
{
   this->Current_Control_ID =  IDC_TRUNK_FLEXION;
   mDocPtr->cueAngle(AngleCueData(A_TrunkFlexion, JT_HIP_CENTER, JT_C7T1, AT_TrunkF));
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnSetfocusTrunkAxialRotation()
{
   this->Current_Control_ID =  IDC_TRUNK_AXIAL_ROTATION;
   mDocPtr->cancelCue();
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnSetfocusTrunkLateralBending()
{
   this->Current_Control_ID =  IDC_TRUNK_LATERAL_BENDING;
   mDocPtr->cueAngle(AngleCueData(A_TrunkLateralBending, JT_HIP_CENTER, JT_C7T1, AT_TrunkLB));
}

//---------------------------------------------------------------------------

void Dg_BodyAngles::OnEnSetfocusUpperlegLh()
{
	this->Current_Control_ID = -1; // effectively disable increment / decrement buttons
	mDocPtr->cueAngle(AngleCueData(A_UpperlegLH, JT_LHIP, JT_LKNEE, AT_LimbHL, A_UpperlegLV));
}

//---------------------------------------------------------------------------

void Dg_BodyAngles::OnEnSetfocusLowerlegLh()
{
	this->Current_Control_ID = -1; // effectively disable increment / decrement buttons
	mDocPtr->cueAngle(AngleCueData(A_LowerlegLH, JT_LKNEE, JT_LANKLE, AT_LimbHL, A_LowerlegLV));
}

//---------------------------------------------------------------------------

void Dg_BodyAngles::OnEnSetfocusUpperlegRh()
{
	this->Current_Control_ID = -1; // effectively disable increment / decrement buttons
	mDocPtr->cueAngle(AngleCueData(A_UpperlegRH, JT_RHIP, JT_RKNEE, AT_LimbHR, A_UpperlegRV));
}


//---------------------------------------------------------------------------

void Dg_BodyAngles::OnEnSetfocusLowerlegRh()
{
	this->Current_Control_ID = -1; // effectively disable increment / decrement buttons
	mDocPtr->cueAngle(AngleCueData(A_LowerlegRH, JT_RKNEE, JT_RANKLE, AT_LimbHR, A_LowerlegRV));
}


//---------------------------------------------------------------------------
void Dg_BodyAngles::OnSymmetry_R_To_L()
{
   if (!this->CWnd::UpdateData(TRUE /*Get data from dialog*/))
		return;

   // Do symmetry operation      
   this->Forearm_LH  = this->Forearm_RH;
   this->Forearm_LV  = this->Forearm_RV;
   this->Upperarm_LH = this->Upperarm_RH;
   this->Upperarm_LV = this->Upperarm_RV;
   this->Upperleg_LV = this->Upperleg_RV;
   this->Lowerleg_LV = this->Lowerleg_RV;
   this->Upperleg_LH = this->Upperleg_RH;
   this->Lowerleg_LH = this->Lowerleg_RH;
   
   this->CWnd::UpdateData(FALSE /*Return data to dialog*/);
}

//---------------------------------------------------------------------------
void Dg_BodyAngles::OnSymmetry_L_To_R()
{
   if (!this->CWnd::UpdateData(TRUE /*Get data from dialog*/))
		return;

   // Do symmetry operation      
   this->Forearm_RH  = this->Forearm_LH;
   this->Forearm_RV  = this->Forearm_LV;
   this->Upperarm_RH = this->Upperarm_LH;
   this->Upperarm_RV = this->Upperarm_LV;
   this->Upperleg_RV = this->Upperleg_LV;
   this->Lowerleg_RV = this->Lowerleg_LV;
   this->Upperleg_RH = this->Upperleg_LH;
   this->Lowerleg_RH = this->Lowerleg_LH;
   
   this->CWnd::UpdateData(FALSE /*Return data to dialog*/);
}

BOOL Dg_BodyAngles::OnInitDialog()
{
	CDialog::OnInitDialog();

	UndoButtonStatus();
    RedoButtonStatus();
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->AppendMenu(MF_SEPARATOR);// add a separator
	pSysMenu->AppendMenu(MF_STRING,IDR_MENU1,"Print Window");// add print menu
	return TRUE;  // return TRUE unless you set the focus to a control
}
void Dg_BodyAngles::UndoButtonStatus()
{
	mUndoButtonControl.EnableWindow(mDocPtr->CanUndo());
}

void Dg_BodyAngles::RedoButtonStatus()
{
	mRedoButtonControl.EnableWindow(mDocPtr->CanRedo());
}



void Dg_BodyAngles::OnPrintPrintwindow()
{
	// TODO: Add your command handler code here
	 CRect m_rect;
 GetClientRect(&m_rect);
 CDC* pImageDC=GetDC();
 int formx,formy;
 formx=pImageDC->GetDeviceCaps(LOGPIXELSX);
 formy=pImageDC->GetDeviceCaps(LOGPIXELSY);
 ReleaseDC(pImageDC);
 DWORD dwflags=PD_PAGENUMS|PD_HIDEPRINTTOFILE|PD_SELECTION;  
 CPrintDialog m_printdlg(FALSE,dwflags,NULL);     
 if(m_printdlg.DoModal()==IDOK)
 {
  CDC pdc;
  pdc.Attach(m_printdlg.GetPrinterDC());
  int printerx,printery;
  printerx=pdc.GetDeviceCaps(LOGPIXELSX);
  printery=pdc.GetDeviceCaps(LOGPIXELSY);
  double ratex,ratey;
  ratex=(double)printerx/formx;
  ratey=(double)printery/formy;
  CClientDC dc(this);
  CBitmap bmp;
  int w=m_rect.Width();
  int h =m_rect.Height();
  bmp.CreateCompatibleBitmap(&dc,m_rect.Width(),m_rect.Height());
  CDC imagedc;
  imagedc.CreateCompatibleDC(&dc);
  imagedc.SelectObject(&bmp);
  imagedc.BitBlt(0,0,m_rect.Width(),m_rect.Height(),&dc,0,0,SRCCOPY);
  BITMAP bmap;
  bmp.GetBitmap(&bmap);
  int panelsize=0;
  if(bmap.bmBitsPixel<16)
  {
   panelsize=pow(2,bmap.bmBitsPixel*sizeof(RGBQUAD));
  }
  BITMAPINFO* bInfo=(BITMAPINFO*)LocalAlloc(LPTR,sizeof(BITMAPINFO)+panelsize);
  bInfo->bmiHeader.biClrImportant=0;
  bInfo->bmiHeader.biBitCount=bmap.bmBitsPixel;
  bInfo->bmiHeader.biCompression=0;
  bInfo->bmiHeader.biHeight=bmap.bmHeight;
  bInfo->bmiHeader.biPlanes=bmap.bmPlanes;
  bInfo->bmiHeader.biSize=sizeof(BITMAPINFO);
  bInfo->bmiHeader.biSizeImage=bmap.bmWidthBytes*bmap.bmHeight;
  bInfo->bmiHeader.biWidth=bmap.bmWidth;
  bInfo->bmiHeader.biXPelsPerMeter=0;
  bInfo->bmiHeader.biYPelsPerMeter=0;
  char *pData=new char[bmap.bmWidthBytes*bmap.bmHeight];
  ::GetDIBits(imagedc.m_hDC,bmp,0,bmap.bmHeight,pData,bInfo,DIB_RGB_COLORS);
  pdc.StartDoc("print");
  ::StretchDIBits(pdc.m_hDC,0,0,(int)((m_rect.Width()*ratex)),
   (int)((m_rect.Height())*ratey),0,0,m_rect.Width(),
   m_rect.Height(),pData,bInfo,DIB_RGB_COLORS,SRCCOPY);
  pdc.EndDoc();
  LocalFree(bInfo);
  delete[] pData;
  pData = NULL;
  bmp.DeleteObject();
 }
}


void Dg_BodyAngles::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (nID ==IDR_MENU1)             
    {
		OnPrintPrintwindow();
    }
	  else
	CDialog::OnSysCommand(nID, lParam);
}
