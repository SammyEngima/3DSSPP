// Dg_BodyAnglesDyn.cpp : implementation file
//

#include "stdafx.h"
#include "skeleton.hpp"
#include "JointAngles.hpp"
#include "Analysis.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include "UndoPosture.h"
#include "MathUtility.h"
#include "Dg_HeadNeck.h"
#include "Dg_BodyAnglesDyn.h"
#include "Dg_HandPosture.h"

#include <string>
#include <sstream>
using std::string;
using std::ostringstream;
using std::endl;
#include ".\dg_bodyanglesdyn.h"

#include "Skeleton.hpp"
#include "CQuaternion.h"

// Dg_BodyAnglesDyn dialog

IMPLEMENT_DYNAMIC(Dg_BodyAnglesDyn, CDialog)
Dg_BodyAnglesDyn::Dg_BodyAnglesDyn(C_Hom_Doc* aDocPtr, CWnd* pParent /*=NULL*/)
	: CDialog(Dg_BodyAnglesDyn::IDD, pParent),
		mDocPtr( aDocPtr ),
		Current_Control_ID( 0 ),
		Increment_Group( 0 ),
		Customizedneck ( 0 )
		//, mMaintainHandPosture(FALSE)
		//, mMaintainHeadPosture(FALSE)
{
    // initialize caption of neutral stance button to corresponding positioning
    //if(aDocPtr->getPositioning() & 1) NeutralButtonCaption = "Neutral Sit";
	if(aDocPtr->getPosition() == P_Sitting) NeutralButtonCaption = "Neutral Sit";
	else NeutralButtonCaption = "Neutral T-Pose";

	this->GetCurrentJointAnglesFromDocument();

	Increment_Group = 0;

	mMaintainHandPosture = aDocPtr -> getMaintainHandAngles();
	mMaintainHeadPosture = aDocPtr -> getMaintainHeadAngles();
}

Dg_BodyAnglesDyn::~Dg_BodyAnglesDyn()
{
	mDocPtr->cancelCue();
}

void Dg_BodyAnglesDyn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FOREARM_LH_DYN, mLeftForearmHorizontalControl);
	DDX_Control(pDX, IDC_FOREARM_LV_DYN, mLeftForearmVerticalControl);
	DDX_Control(pDX, IDC_FOREARM_RH_DYN, mRightForearmHorizontalControl);
	DDX_Control(pDX, IDC_FOREARM_RV_DYN, mRightForearmVerticalControl);
	DDX_Control(pDX, IDC_UPPERARM_LH_DYN, mLeftUpperarmHorizontalControl);
	DDX_Control(pDX, IDC_UPPERARM_LV_DYN, mLeftUpperarmVerticalControl);
	DDX_Control(pDX, IDC_UPPERARM_RH_DYN, mRightUpperarmHorizontalControl);
	DDX_Control(pDX, IDC_UPPERARM_RV_DYN, mRightUpperarmVerticalControl);
	DDX_Control(pDX, IDC_UPPERLEG_LH_DYN, mLeftUpperLegHorizontalControl);
	DDX_Control(pDX, IDC_UPPERLEG_LV_DYN, mLeftUpperlegVerticalControl);
	DDX_Control(pDX, IDC_UPPERLEG_RH_DYN, mRightUpperlegHorizontalControl);
	DDX_Control(pDX, IDC_UPPERLEG_RV_DYN, mRightUpperlegVerticalControl);
	DDX_Control(pDX, IDC_LOWERLEG_LV_DYN, mLeftLowerlegVerticalControl);
	DDX_Control(pDX, IDC_LOWERLEG_RV_DYN, mRightLowerlegVerticalControl);
	DDX_Control(pDX, IDC_FOOT_LV_DYN, mLeftFootVerticalControl);
	DDX_Control(pDX, IDC_FOOT_RV_DYN, mRightFootVerticalControl);
	DDX_Control(pDX, IDC_CLAVICLE_LH_DYN, mLeftClavicleHorizontalControl);
	DDX_Control(pDX, IDC_CLAVICLE_LV_DYN, mLeftClavicleVerticalControl);
	DDX_Control(pDX, IDC_CLAVICLE_RH_DYN, mRightClavicleHorizontalControl);
	DDX_Control(pDX, IDC_CLAVICLE_RV_DYN, mRightClavicleVerticalControl);
	DDX_Control(pDX, IDC_TRUNK_FLEXION_DYN, mTrunkFlexionControl);
	DDX_Control(pDX, IDC_TRUNK_AXIAL_ROTATION_DYN, mTrunkAxialRotationControl);
	DDX_Control(pDX, IDC_TRUNK_LATERAL_BENDING_DYN, mTrunkLateralBendingControl);
	DDX_Control(pDX, IDC_PELVIC_LATERAL_TILT_DYN, mPelvicLateralTiltControl);
	DDX_Control(pDX, IDC_HEAD_FLEXION_DYN, mHeadFlexionControl);
	//DDX_Control(pDX, IDC_HEAD_ROT_DYN, mHeadAxialRotationControl);
	//DDX_Control(pDX, IDC_HEAD_LATERAL_DYN, mHeadLateralBendingControl);
	DDX_Control(pDX, IDC_NECK1_FLEXION_DYN, mUpperNeckFlexionControl);
	DDX_Control(pDX, IDC_NECK1_ROT_DYN, mUpperNeckAxialRotationControl);
	DDX_Control(pDX, IDC_NECK1_LATERAL_DYN, mUpperNeckLateralBendingControl);
	DDX_Control(pDX, IDC_NECK2_FLEXION_DYN, mLowerNeckFlexionControl);
	DDX_Control(pDX, IDC_NECK2_ROT_DYN, mLowerNeckAxialRotationControl);
	DDX_Control(pDX, IDC_NECK2_LATERAL_DYN, mLowerNeckLateralBendingControl);
	DDX_Text(pDX, IDC_FOREARM_LH_DYN, Forearm_LH);
	DDX_Text(pDX, IDC_FOREARM_LV_DYN, Forearm_LV);
	DDX_Text(pDX, IDC_FOREARM_RH_DYN, Forearm_RH);
	DDX_Text(pDX, IDC_FOREARM_RV_DYN, Forearm_RV);
	DDX_Text(pDX, IDC_UPPERARM_LH_DYN, Upperarm_LH);
	DDX_Text(pDX, IDC_UPPERARM_LV_DYN, Upperarm_LV);
	DDX_Text(pDX, IDC_UPPERARM_RH_DYN, Upperarm_RH);
	DDX_Text(pDX, IDC_UPPERARM_RV_DYN, Upperarm_RV);
	DDX_Text(pDX, IDC_UPPERLEG_LH_DYN, Upperleg_LH);
	DDX_Text(pDX, IDC_UPPERLEG_LV_DYN, Upperleg_LV);
	DDX_Text(pDX, IDC_UPPERLEG_RH_DYN, Upperleg_RH);
	DDX_Text(pDX, IDC_UPPERLEG_RV_DYN, Upperleg_RV);
	DDX_Text(pDX, IDC_LOWERLEG_LV_DYN, Lowerleg_LV);
	DDX_Text(pDX, IDC_LOWERLEG_RV_DYN, Lowerleg_RV);
	DDX_Text(pDX, IDC_FOOT_LV_DYN, Foot_LV);
	DDX_Text(pDX, IDC_FOOT_RV_DYN, Foot_RV);
	DDX_Text(pDX, IDC_TRUNK_FLEXION_DYN, Trunk_Flexion);
	DDX_Text(pDX, IDC_TRUNK_AXIAL_ROTATION_DYN, Trunk_Axial_Rotation);
	DDX_Text(pDX, IDC_TRUNK_LATERAL_BENDING_DYN, Trunk_Lateral_Bending);
	DDX_Text(pDX, IDC_HEADFLEXION, Head_Flexion);
	DDX_Text(pDX, IDC_HEADROTATION, Head_Axial_Rotation);
	DDX_Text(pDX, IDC_HEADBENDING, Head_Lateral_Bending);
	DDX_Text(pDX, IDC_PELVIC_LATERAL_TILT_DYN, PelvicLateralTilt);
	DDX_Text(pDX, IDC_HEAD_FLEXION_DYN, HeadFlexion);
	//DDX_Text(pDX, IDC_HEAD_ROT_DYN, HeadAxialRotation);
	//DDX_Text(pDX, IDC_HEAD_LATERAL_DYN, HeadLateralBending);
	DDX_Text(pDX, IDC_NECK1_FLEXION_DYN, UpperNeckFlexion);
	DDX_Text(pDX, IDC_NECK1_ROT_DYN, UpperNeckAxialRotation);
	DDX_Text(pDX, IDC_NECK1_LATERAL_DYN, UpperNeckLateralBending);
	DDX_Text(pDX, IDC_NECK2_FLEXION_DYN, LowerNeckFlexion);
	DDX_Text(pDX, IDC_NECK2_ROT_DYN, LowerNeckAxialRotation);
	DDX_Text(pDX, IDC_NECK2_LATERAL_DYN, LowerNeckLateralBending);
	DDX_Text(pDX, IDC_CLAVICLE_LH_DYN, Clavicle_LH);
	DDX_Text(pDX, IDC_CLAVICLE_LV_DYN, Clavicle_LV);
	DDX_Text(pDX, IDC_CLAVICLE_RH_DYN, Clavicle_RH);
	DDX_Text(pDX, IDC_CLAVICLE_RV_DYN, Clavicle_RV);
	DDX_Text(pDX, IDC_NeutralButton, NeutralButtonCaption);
	DDX_Radio(pDX, IDC_INCREMENT_GROUP, Increment_Group);
	DDX_Control(pDX, IDC_UndoButton, mUndoButtonControl);
	DDX_Control(pDX, IDC_RedoButton, mRedoButtonControl);
	DDX_Control(pDX, IDC_LOWERLEG_LH_DYN, mLeftLowerlegHorizontalControl);
	DDX_Control(pDX, IDC_LOWERLEG_RH_DYN, mRightLowerlegHorizontalControl);
	DDX_Control(pDX, IDC_FOOT_LH_DYN, mLeftFootHorizontalControl);
	DDX_Control(pDX, IDC_FOOT_RH_DYN, mRightFootHorizontalControl);
	DDX_Text(pDX, IDC_LOWERLEG_LH_DYN, Lowerleg_LH);
	DDX_Text(pDX, IDC_LOWERLEG_RH_DYN, Lowerleg_RH);
	DDX_Text(pDX, IDC_FOOT_LH_DYN, Foot_LH);
	DDX_Text(pDX, IDC_FOOT_RH_DYN, Foot_RH);
	DDX_Text(pDX, IDC_PELVIC_AXIAL_ROTATION_DYN, PelvicAxialRotation);
	DDX_Check(pDX, IDC_MAINTAINHANDPOSTURE, mMaintainHandPosture);
	DDX_Check(pDX, IDC_MaintainHeadPosture, mMaintainHeadPosture);
}


BEGIN_MESSAGE_MAP(Dg_BodyAnglesDyn, CDialog)
	ON_EN_SETFOCUS(IDC_FOREARM_LH_DYN, OnEnSetfocusForearmLhDyn)
	ON_EN_SETFOCUS(IDC_FOREARM_LV_DYN, OnEnSetfocusForearmLvDyn)
	ON_EN_SETFOCUS(IDC_FOREARM_RH_DYN, OnEnSetfocusForearmRhDyn)
	ON_EN_SETFOCUS(IDC_FOREARM_RV_DYN, OnEnSetfocusForearmRvDyn)
	ON_EN_SETFOCUS(IDC_UPPERARM_LH_DYN, OnEnSetfocusUpperarmLhDyn)
	ON_EN_SETFOCUS(IDC_UPPERARM_LV_DYN, OnEnSetfocusUpperarmLvDyn)
	ON_EN_SETFOCUS(IDC_UPPERARM_RH_DYN, OnEnSetfocusUpperarmRhDyn)
	ON_EN_SETFOCUS(IDC_UPPERARM_RV_DYN, OnEnSetfocusUpperarmRvDyn)
	ON_EN_SETFOCUS(IDC_UPPERLEG_LH_DYN, OnEnSetfocusUpperlegLhDyn)
	ON_EN_SETFOCUS(IDC_UPPERLEG_LV_DYN, OnEnSetfocusUpperlegLvDyn)
	ON_EN_SETFOCUS(IDC_UPPERLEG_RH_DYN, OnEnSetfocusUpperlegRhDyn)
	ON_EN_SETFOCUS(IDC_UPPERLEG_RV_DYN, OnEnSetfocusUpperlegRvDyn)
	ON_EN_SETFOCUS(IDC_LOWERLEG_LV_DYN, OnEnSetfocusLowerlegLvDyn)
	ON_EN_SETFOCUS(IDC_LOWERLEG_RV_DYN, OnEnSetfocusLowerlegRvDyn)
	ON_EN_SETFOCUS(IDC_FOOT_LV_DYN, OnEnSetfocusFootLvDyn)
	ON_EN_SETFOCUS(IDC_FOOT_RV_DYN, OnEnSetfocusFootRvDyn)
	ON_EN_SETFOCUS(IDC_TRUNK_FLEXION_DYN, OnEnSetfocusTrunkFlexionDyn)
	ON_EN_SETFOCUS(IDC_TRUNK_AXIAL_ROTATION_DYN, OnEnSetfocusTrunkAxialRotationDyn)
	ON_EN_SETFOCUS(IDC_TRUNK_LATERAL_BENDING_DYN, OnEnSetfocusTrunkLateralBendingDyn)
	ON_EN_SETFOCUS(IDC_PELVIC_LATERAL_TILT_DYN, OnEnSetfocusPelvicLateralTiltDyn)
	ON_EN_SETFOCUS(IDC_HEADFLEXION, OnEnSetfocusHeadFlexionDyn)
	ON_EN_SETFOCUS(IDC_HEADROTATION, OnEnSetfocusHeadRotDyn)
	ON_EN_SETFOCUS(IDC_HEADBENDING, OnEnSetfocusHeadLateralDyn)
	ON_EN_SETFOCUS(IDC_NECK1_FLEXION_DYN, OnEnSetfocusNeck1FlexionDyn)
	ON_EN_SETFOCUS(IDC_NECK1_ROT_DYN, OnEnSetfocusNeck1RotDyn)
	ON_EN_SETFOCUS(IDC_NECK1_LATERAL_DYN, OnEnSetfocusNeck1LateralDyn)
	ON_EN_SETFOCUS(IDC_NECK2_FLEXION_DYN, OnEnSetfocusNeck2FlexionDyn)
	ON_EN_SETFOCUS(IDC_NECK2_ROT_DYN, OnEnSetfocusNeck2RotDyn)
	ON_EN_SETFOCUS(IDC_NECK2_LATERAL_DYN, OnEnSetfocusNeck2LateralDyn)
	ON_EN_SETFOCUS(IDC_CLAVICLE_LH_DYN, OnEnSetfocusClavicleLhDyn)
	ON_EN_SETFOCUS(IDC_CLAVICLE_LV_DYN, OnEnSetfocusClavicleLvDyn)
	ON_EN_SETFOCUS(IDC_CLAVICLE_RH_DYN, OnEnSetfocusClavicleRhDyn)
	ON_EN_SETFOCUS(IDC_CLAVICLE_RV_DYN, OnEnSetfocusClavicleRvDyn)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_REDRAW, OnBnClickedRedraw)
	ON_BN_CLICKED(IDC_NeutralButton, OnBnClickedNeutralbutton)
	ON_BN_CLICKED(IDC_UndoButton, OnBnClickedUndobutton)
	ON_BN_CLICKED(IDC_RedoButton, OnBnClickedRedobutton)
	ON_BN_CLICKED(IDC_INCREMENT_PLUS_DYN, OnBnClickedIncrementPlusDyn)
	ON_BN_CLICKED(IDC_INCREMENT_MINUS_DYN, OnBnClickedIncrementMinusDyn)
	ON_BN_CLICKED(IDC_SYMMETRY_R_TO_L_DYN, OnBnClickedSymmetryRToLDyn)
	ON_BN_CLICKED(IDC_SYMMETRY_L_TO_R_DYN, OnBnClickedSymmetryLToRDyn)
	ON_EN_SETFOCUS(IDC_LOWERLEG_LH_DYN, OnEnSetfocusLowerlegLhDyn)
	ON_EN_SETFOCUS(IDC_LOWERLEG_RH_DYN, OnEnSetfocusLowerlegRhDyn)
	ON_EN_SETFOCUS(IDC_FOOT_LH_DYN, OnEnSetfocusFootLhDyn)
	ON_EN_SETFOCUS(IDC_FOOT_RH_DYN, OnEnSetfocusFootRhDyn)
	ON_EN_CHANGE(IDC_HEADFLEXION, HeadGlobalControl)
	ON_EN_CHANGE(IDC_HEADROTATION, HeadGlobalControl)
	ON_EN_CHANGE(IDC_HEADBENDING, HeadGlobalControl)
	ON_WM_CLOSE()
	ON_EN_SETFOCUS(IDC_PELVIC_AXIAL_ROTATION_DYN, &Dg_BodyAnglesDyn::OnEnSetfocusPelvicAxialRotationDyn)
	ON_BN_CLICKED(IDC_HANDANGLES, &Dg_BodyAnglesDyn::OnBnClickedHandangles)
	ON_BN_CLICKED(IDC_MAINTAINHANDPOSTURE, &Dg_BodyAnglesDyn::OnBnClickedMaintainhandposture)
	ON_BN_CLICKED(IDC_MaintainHeadPosture, &Dg_BodyAnglesDyn::OnBnClickedMaintainheadposture)
	ON_COMMAND(ID_PRINT_PRINTWINDOW, &Dg_BodyAnglesDyn::OnPrintPrintwindow)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BUTTON1, &Dg_BodyAnglesDyn::OnBnClickedButton1)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

//---------------------------------------------------------------------------
void Dg_BodyAnglesDyn::GetCurrentJointAnglesFromDocument() 
{
	double angles[NUM_EXTRA_ANGLES];
	mDocPtr->GetSkeleton()->getExtraAngles(angles);
	this->SetJointAngles(mDocPtr->GetSkeleton()->GetAngles(),angles,0);
}

//---------------------------------------------------------------------------
void Dg_BodyAnglesDyn::SetJointAngles(const JointAngles& aJointAnglesRef, const double angles[NUM_EXTRA_ANGLES], int flag)
{
	this->Forearm_LH  =  aJointAnglesRef[A_ForearmLH];
	this->Forearm_LV  =  aJointAnglesRef[A_ForearmLV];
	this->Forearm_RH  =  aJointAnglesRef[A_ForearmRH];
	this->Forearm_RV  =  aJointAnglesRef[A_ForearmRV];

	this->Upperarm_LH  =  aJointAnglesRef[A_UpperarmLH];
	this->Upperarm_LV  =  aJointAnglesRef[A_UpperarmLV];
	this->Upperarm_RH  =  aJointAnglesRef[A_UpperarmRH];
	this->Upperarm_RV  =  aJointAnglesRef[A_UpperarmRV];

	this->Clavicle_LH  =  aJointAnglesRef[A_ClavicleLH];
	this->Clavicle_LV  =  aJointAnglesRef[A_ClavicleLV];
	this->Clavicle_RH  =  aJointAnglesRef[A_ClavicleRH];
	this->Clavicle_RV  =  aJointAnglesRef[A_ClavicleRV];

	this->Upperleg_LH  =  aJointAnglesRef[A_UpperlegLH];
	this->Upperleg_LV  =  aJointAnglesRef[A_UpperlegLV];
	this->Upperleg_RH  =  aJointAnglesRef[A_UpperlegRH];
	this->Upperleg_RV  =  aJointAnglesRef[A_UpperlegRV];

	this->Lowerleg_LH  =  aJointAnglesRef[A_LowerlegLH];
	this->Lowerleg_LV  =  aJointAnglesRef[A_LowerlegLV];
	this->Lowerleg_RH  =  aJointAnglesRef[A_LowerlegRH];
	this->Lowerleg_RV  =  aJointAnglesRef[A_LowerlegRV];

	this->Trunk_Flexion          =  aJointAnglesRef[A_TrunkFlexion];
	this->Trunk_Axial_Rotation   =  aJointAnglesRef[A_TrunkAxialRotation];
	this->Trunk_Lateral_Bending  =  aJointAnglesRef[A_TrunkLateralBending];
	
	if(flag == 1){
		this->Head_Flexion          =  90;
		this->Head_Axial_Rotation   =  0;
		this->Head_Lateral_Bending  =  0;

		this->LowerNeckFlexion			= 90;
		this->LowerNeckAxialRotation		= 0;
		this->LowerNeckLateralBending	= 0;

		this->UpperNeckFlexion			= 90;
		this->UpperNeckAxialRotation		= 0;
		this->UpperNeckLateralBending	= 0;

		this->HeadFlexion			= 90;
	}else{
		//make global local
		this->Head_Flexion          =  aJointAnglesRef[A_TrunkFlexion] - (270 - (angles[EA_C4C7Flexion]+16 + angles[EA_C1C3Flexion] + angles[EA_C1_Flexion]));
		this->Head_Axial_Rotation   =  angles[EA_C1C3AxialRotation] + angles[EA_C4C7AxialRotation];
		this->Head_Lateral_Bending  =  angles[EA_C1C3LateralBending] + angles[EA_C4C7LateralBending];

		this->LowerNeckFlexion			= angles[EA_C4C7Flexion];
		this->LowerNeckAxialRotation		= angles[EA_C4C7AxialRotation];
		this->LowerNeckLateralBending	= angles[EA_C4C7LateralBending];

		this->UpperNeckFlexion			= angles[EA_C1C3Flexion];
		this->UpperNeckAxialRotation		= angles[EA_C1C3AxialRotation];
		this->UpperNeckLateralBending	= angles[EA_C1C3LateralBending];

		this->HeadFlexion			= angles[EA_C1_Flexion]+16;
	}
	//this->HeadAxialRotation		= aJointAnglesRef[A_C4C7AxialRotation] + aJointAnglesRef[A_C1C3_AxialRotation];
	//this->HeadLateralBending	= aJointAnglesRef[A_C4C7LateralBending] + aJointAnglesRef[A_C1C3_LateralBending];

    this->NasionFlexion			= aJointAnglesRef[A_NasionFlexion];

	this->PelvicLateralTilt = aJointAnglesRef[A_PelvisLateralBending];
	this->PelvicAxialRotation = aJointAnglesRef[A_PelvisAxialRotation];

	this->Foot_LH = aJointAnglesRef[A_FootLH];
	this->Foot_LV = aJointAnglesRef[A_FootLV];
	this->Foot_RH = aJointAnglesRef[A_FootRH];
	this->Foot_RV = aJointAnglesRef[A_FootRV];

	this->Clavicle_LH = aJointAnglesRef[A_ClavicleLH];
	this->Clavicle_LV = aJointAnglesRef[A_ClavicleLV];
	this->Clavicle_RH = aJointAnglesRef[A_ClavicleRH];
	this->Clavicle_RV = aJointAnglesRef[A_ClavicleRV];
}

// Function returns true if document joint angles are no different
//		from dialog member joint angles.
bool Dg_BodyAnglesDyn::SameAsDocument()
{
	JointAngles docJointAngles = mDocPtr->GetSkeleton()->GetAngles();
	
	C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
	Skeleton &skel  = *(doc->GetSkeleton());
	double alpha = L5S1_ERECT_TILT - skel.getExtraAngle(EA_PelvisFlexion);
	double LocalAngles[NUM_EXTRA_ANGLES];
	skel.getExtraAngles(LocalAngles);
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

		docJointAngles[A_ClavicleLH] == this->Clavicle_LH &&
		docJointAngles[A_ClavicleLV] == this->Clavicle_LV &&
		docJointAngles[A_ClavicleRH] == this->Clavicle_RH &&
		docJointAngles[A_ClavicleRV] == this->Clavicle_RV &&

		docJointAngles[A_UpperlegLH] == this->Upperleg_LH &&
		docJointAngles[A_UpperlegLV] == this->Upperleg_LV &&
		docJointAngles[A_UpperlegRH] == this->Upperleg_RH &&
		docJointAngles[A_UpperlegRV] == this->Upperleg_RV &&

		docJointAngles[A_LowerlegLH] == this->Lowerleg_LH &&
		docJointAngles[A_LowerlegLV] == this->Lowerleg_LV &&
		docJointAngles[A_LowerlegRH] == this->Lowerleg_RH &&
		docJointAngles[A_LowerlegRV] == this->Lowerleg_RV &&

		docJointAngles[A_TrunkFlexion] == this->Trunk_Flexion &&
		docJointAngles[A_TrunkAxialRotation] == this->Trunk_Axial_Rotation &&
		docJointAngles[A_TrunkLateralBending] == this->Trunk_Lateral_Bending &&

		LocalAngles[EA_C1_Flexion] == this->HeadFlexion - 16 &&
		
		LocalAngles[EA_C1C3Flexion] == this->UpperNeckFlexion &&
		LocalAngles[EA_C1C3AxialRotation] == this->UpperNeckAxialRotation &&
 		LocalAngles[EA_C1C3LateralBending] == this->UpperNeckLateralBending &&

		LocalAngles[EA_C4C7Flexion] == this->LowerNeckFlexion &&
		LocalAngles[EA_C4C7AxialRotation] == this->LowerNeckAxialRotation &&
		LocalAngles[EA_C4C7LateralBending] == this->LowerNeckLateralBending &&

		// 09/21/07 Sayan
		// As per Chuck's suggestion, we need to display "L5S1 forward tilt" as well
		// pelvic lateral tilt. I think that the  "L5S1 forward tilt"  is the value 
		// of the angle (90 - alpha), but we need to confirm this with Chuck.
		// 

		/* C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
	    Skeleton &skel  = *(doc->GetSkeleton());
	    skel.getJoint((JointID) sel));
		docJointAngles[A_PelvisLateralBending] == (this->PelvicLateralTilt + alpha) && */

        docJointAngles[A_PelvisLateralBending] == this->PelvicLateralTilt  &&
		docJointAngles[A_PelvisAxialRotation] == this->PelvicAxialRotation &&

		// L5S1ForwardTilt == (90 - alpha) && 
		docJointAngles[A_FootLH] == this->Foot_LH &&
		docJointAngles[A_FootLV] == this->Foot_LV &&
		docJointAngles[A_FootRH] == this->Foot_RH &&
		docJointAngles[A_FootRV] == this->Foot_RV &&

		docJointAngles[A_ClavicleLH] == this->Clavicle_LH &&
		docJointAngles[A_ClavicleLV] == this->Clavicle_LV &&
		docJointAngles[A_ClavicleRH] == this->Clavicle_RH &&
		docJointAngles[A_ClavicleRV] == this->Clavicle_RV &&
		mDocPtr->getMaintainHandAngles() == this->mMaintainHandPosture &&
		mDocPtr->getMaintainHeadAngles() == this->mMaintainHeadPosture
		);
}

Vector3 RLimbVect(double Vertical, double Horizontal) {
	return Vector3(dcos(Vertical)*dcos(Horizontal), dcos(Vertical)*dsin(Horizontal), dsin(Vertical));
}

Vector3 LLimbVect(double Vertical, double Horizontal) {
	return Vector3(-dcos(Vertical)*dcos(Horizontal), dcos(Vertical)*dsin(Horizontal), dsin(Vertical));
}

void Dg_BodyAnglesDyn::BuildJointAngles(JointAngles& ja, double angles[NUM_EXTRA_ANGLES]) {
	ja[A_ForearmLH] = this->Forearm_LH;
	ja[A_ForearmLV] = this->Forearm_LV;
	ja[A_ForearmRH] = this->Forearm_RH;
	ja[A_ForearmRV] = this->Forearm_RV;

	ja[A_UpperarmLH] = this->Upperarm_LH;
	ja[A_UpperarmLV] = this->Upperarm_LV;
	ja[A_UpperarmRH] = this->Upperarm_RH;
	ja[A_UpperarmRV] = this->Upperarm_RV;

	ja[A_ClavicleLH] = this->Clavicle_LH;
	ja[A_ClavicleLV] = this->Clavicle_LV;
	ja[A_ClavicleRH] = this->Clavicle_RH;
	ja[A_ClavicleRV] = this->Clavicle_RV;

	ja[A_UpperlegLH] = this->Upperleg_LH;
	ja[A_UpperlegLV] = this->Upperleg_LV;
	ja[A_UpperlegRH] = this->Upperleg_RH;
	ja[A_UpperlegRV] = this->Upperleg_RV;

	ja[A_LowerlegLH] = this->Lowerleg_LH;
	ja[A_LowerlegLV] = this->Lowerleg_LV;
	ja[A_LowerlegRH] = this->Lowerleg_RH;
	ja[A_LowerlegRV] = this->Lowerleg_RV;
	
	// to unlock neck posture
	neckbase1 = this->Trunk_Flexion - ja[A_TrunkFlexion];
	neckbase2 = this->Trunk_Lateral_Bending - ja[A_TrunkLateralBending];
	neckbase3 = this->Trunk_Axial_Rotation - ja[A_TrunkAxialRotation];
	
	ja[A_TrunkFlexion] = this->Trunk_Flexion;
	ja[A_TrunkAxialRotation] = this->Trunk_Axial_Rotation;
	ja[A_TrunkLateralBending] = this->Trunk_Lateral_Bending;

	
	ja[A_PelvisLateralBending] = this->PelvicLateralTilt;
	ja[A_PelvisAxialRotation] = this->PelvicAxialRotation;
	
	if (Customizedneck == 0){
		LowerNeckFlexion = 90 - 0.21*(ja[A_TrunkFlexion] - this->Head_Flexion);
		UpperNeckFlexion = 90 - 0.61*(ja[A_TrunkFlexion] -  this->Head_Flexion);
		HeadFlexion = 90 - 0.18*(ja[A_TrunkFlexion] -  this->Head_Flexion);
		LowerNeckAxialRotation = 0.16* this->Head_Axial_Rotation;
		UpperNeckAxialRotation = 0.84* this->Head_Axial_Rotation;
		LowerNeckLateralBending = 0.43* this->Head_Lateral_Bending;
		UpperNeckLateralBending = 0.57* this->Head_Lateral_Bending;
		}
	else if(!mMaintainHeadPosture){
		LowerNeckFlexion = LowerNeckFlexion - neckbase1;
		LowerNeckLateralBending = LowerNeckLateralBending - neckbase2;
		LowerNeckAxialRotation = LowerNeckAxialRotation - neckbase3;
	}
	//Lower Neck
	ja[A_C4C7Flexion] = this->LowerNeckFlexion;
	ja[A_C4C7AxialRotation] = this->LowerNeckAxialRotation;
	ja[A_C4C7LateralBending] = this->LowerNeckLateralBending;
	
	//make global angle local
	/*angles[EA_C4C7Flexion] = this->LowerNeckFlexion - this->Trunk_Flexion + 90;
	angles[EA_C4C7AxialRotation] = this->LowerNeckAxialRotation;
	angles[EA_C4C7LateralBending] = this->LowerNeckLateralBending;
	angles[EA_C1C3Flexion] = this->UpperNeckFlexion - (ja[A_C4C7Flexion] + this->Trunk_Flexion - 90) + 90;
	angles[EA_C1C3AxialRotation] = this->UpperNeckAxialRotation;
	angles[EA_C1C3LateralBending] = this->UpperNeckLateralBending;*/


	
	//Upper Neck
	ja[A_C1C3_Flexion] = this->UpperNeckFlexion;
	ja[A_C1C3_AxialRotation] = this->UpperNeckAxialRotation;
	ja[A_C1C3_LateralBending] = this->UpperNeckLateralBending;
	
	//Head
	//For nasion flexion
	ja[A_C1_Flexion] = this->HeadFlexion - 16;
	//ja[A_C4C7AxialRotation] = this->HeadAxialRotation;
	//ja[A_C4C7LateralBending] = this->HeadLateralBending;

	ja[A_FootLH] = this->Foot_LH;
	ja[A_FootLV] = this->Foot_LV;
	ja[A_FootRH] = this->Foot_RH;
	ja[A_FootRV] = this->Foot_RV;
}

bool Dg_BodyAnglesDyn::UpdateDocument()
{
	//if(SameAsDocument())
	// 	return true;	// Do not update; no changes
	bool lUpdateOK = true;
	JointAngles newJointAngles = mDocPtr->GetSkeleton()->GetAngles();
	JointAngles oldAngles = newJointAngles;
	double newEAngles[NUM_EXTRA_ANGLES];
	mDocPtr->GetSkeleton()-> getExtraAngles(newEAngles);
	Skeleton& skel = *mDocPtr->GetSkeleton();
	
	if(mMaintainHeadPosture) {
/*
		UpperNeckFlexion = Trunk_Flexion + diff1;
		LowerNeckFlexion = Trunk_Flexion + diff2;
		UpperNeckAxialRotation = Trunk_Axial_Rotation + diff3;
		UpperNeckLateralBending = Trunk_Lateral_Bending + diff4;
		LowerNeckAxialRotation = Trunk_Axial_Rotation + diff5; 
		LowerNeckLateralBending = Trunk_Lateral_Bending + diff6;
		*/
		if (Trunk_Flexion !=  oldAngles[A_TrunkFlexion]) {
			Head_Flexion = Trunk_Flexion + diff7;
			Head_Lateral_Bending = Trunk_Lateral_Bending + diff8;
			Head_Axial_Rotation = Trunk_Axial_Rotation + diff9;
		} else {
			diff0 = HeadFlexion - Trunk_Flexion;
			diff1 = UpperNeckFlexion - Trunk_Flexion;
			diff2 = LowerNeckFlexion - Trunk_Flexion;
			diff3 = UpperNeckAxialRotation - Trunk_Axial_Rotation;
			diff4 = UpperNeckLateralBending - Trunk_Lateral_Bending;
			diff5 = LowerNeckAxialRotation - Trunk_Axial_Rotation;
			diff6 = LowerNeckLateralBending - Trunk_Lateral_Bending;
			diff7 = Head_Flexion - Trunk_Flexion;
			diff8 = Head_Lateral_Bending - Trunk_Lateral_Bending;
			diff9 = Head_Axial_Rotation - Trunk_Axial_Rotation;
		}
	}
	BuildJointAngles(newJointAngles, newEAngles);

	// if locking hand angles to forearm angles, set hand angles
	if(mDocPtr->getFixedHandAngles())
	{
		newJointAngles.SetStraightHands();
	} 
	/*else if(mMaintainHandPosture)
	{
		if(!skel.ValidateJointAngles(newJointAngles, true)) //validation failed
		{
			AfxMessageBox(skel.getLastError().c_str());
			return false;
		}
	} 
	else 
	{
		if(!skel.ValidateJointAngles(newJointAngles, false)) 
		{
 			AfxMessageBox(skel.getLastError().c_str());
			return false;
		}
	} */ 

	mDocPtr->setMaintainHandAngles(mMaintainHandPosture);
	mDocPtr->setMaintainHeadAngles(mMaintainHeadPosture);
		
	skel.SetAngles(newJointAngles);
	int frame = mDocPtr->getCurrentFrame();
	UndoablePostureChange *undoEvent = new UndoablePostureChange(frame,oldAngles,newJointAngles, mDocPtr->getFrameType(frame), FT_Keyframe);
	if(mDocPtr->getFrameType() != FT_Unset) {
		mDocPtr->setFrameType(FT_Keyframe);
	}
	mDocPtr->addUndoEvent(undoEvent);
	mDocPtr->MakeDirtyAndUpdateViews();

	GetCurrentJointAnglesFromDocument();
	

	UpdateData(false);
	// Gray/ungray the Undo and Redo buttons if necessary	
	UndoButtonStatus();
    RedoButtonStatus();
	return lUpdateOK;
}  

// Dg_BodyAnglesDyn message handlers

void Dg_BodyAnglesDyn::OnEnSetfocusForearmLhDyn()
{
	Current_Control_ID = IDC_FOREARM_LH_DYN;
	mDocPtr->cueAngle(AngleCueData(A_ForearmLH, JT_LELBOW, JT_LWRIST, AT_LimbHL, A_ForearmLV));
}

void Dg_BodyAnglesDyn::OnEnSetfocusForearmLvDyn()
{
	Current_Control_ID = IDC_FOREARM_LV_DYN;
	mDocPtr->cueAngle(AngleCueData(A_ForearmLV, JT_LELBOW, JT_LWRIST, AT_LimbVL, A_ForearmLH));
}

void Dg_BodyAnglesDyn::OnEnSetfocusForearmRhDyn()
{
	Current_Control_ID = IDC_FOREARM_RH_DYN;
	mDocPtr->cueAngle(AngleCueData(A_ForearmRH, JT_RELBOW, JT_RWRIST, AT_LimbHR, A_ForearmRV));
}

void Dg_BodyAnglesDyn::OnEnSetfocusForearmRvDyn()
{
	Current_Control_ID = IDC_FOREARM_RV_DYN;
	mDocPtr->cueAngle(AngleCueData(A_ForearmRV, JT_RELBOW, JT_RWRIST, AT_LimbVR, A_ForearmRH));
}

void Dg_BodyAnglesDyn::OnEnSetfocusUpperarmLhDyn()
{
	Current_Control_ID = IDC_UPPERARM_LH_DYN;
	mDocPtr->cueAngle(AngleCueData(A_UpperarmLH, JT_LSHOULDER, JT_LELBOW, AT_LimbHL, A_UpperarmLV));
}

void Dg_BodyAnglesDyn::OnEnSetfocusUpperarmLvDyn()
{
	Current_Control_ID = IDC_UPPERARM_LV_DYN;
	mDocPtr->cueAngle(AngleCueData(A_UpperarmLV, JT_LSHOULDER, JT_LELBOW, AT_LimbVL, A_UpperarmLH));
}

void Dg_BodyAnglesDyn::OnEnSetfocusUpperarmRhDyn()
{
	Current_Control_ID = IDC_UPPERARM_RH_DYN;
	mDocPtr->cueAngle(AngleCueData(A_UpperarmRH, JT_RSHOULDER, JT_RELBOW, AT_LimbHR, A_UpperarmRV));
}

void Dg_BodyAnglesDyn::OnEnSetfocusUpperarmRvDyn()
{
	Current_Control_ID = IDC_UPPERARM_RV_DYN;
	mDocPtr->cueAngle(AngleCueData(A_UpperarmRV, JT_RSHOULDER, JT_RELBOW, AT_LimbVR, A_UpperarmRH));
}

void Dg_BodyAnglesDyn::OnEnSetfocusUpperlegLhDyn()
{
	Current_Control_ID = IDC_UPPERLEG_LH_DYN;
	mDocPtr->cueAngle(AngleCueData(A_UpperlegLH, JT_LHIP, JT_LKNEE, AT_LimbHL, A_UpperlegLV));
}

void Dg_BodyAnglesDyn::OnEnSetfocusUpperlegLvDyn()
{
	Current_Control_ID = IDC_UPPERLEG_LV_DYN;
	mDocPtr->cueAngle(AngleCueData(A_UpperlegLV, JT_LHIP, JT_LKNEE, AT_LimbVL, A_UpperlegLH));
}

void Dg_BodyAnglesDyn::OnEnSetfocusUpperlegRhDyn()
{
	Current_Control_ID = IDC_UPPERLEG_RH_DYN;
	mDocPtr->cueAngle(AngleCueData(A_UpperlegRH, JT_RHIP, JT_RKNEE, AT_LimbHR, A_UpperlegRV));
}

void Dg_BodyAnglesDyn::OnEnSetfocusUpperlegRvDyn()
{
	Current_Control_ID = IDC_UPPERLEG_RV_DYN;
	mDocPtr->cueAngle(AngleCueData(A_UpperlegRV, JT_RHIP, JT_RKNEE, AT_LimbVR, A_UpperlegRH));
}

void Dg_BodyAnglesDyn::OnEnSetfocusLowerlegLvDyn()
{
	Current_Control_ID = IDC_LOWERLEG_LV_DYN;
	mDocPtr->cueAngle(AngleCueData(A_LowerlegLV, JT_LKNEE, JT_LANKLE, AT_LimbVL, A_LowerlegLH));
}

void Dg_BodyAnglesDyn::OnEnSetfocusLowerlegRvDyn()
{
	Current_Control_ID = IDC_LOWERLEG_RV_DYN;
	mDocPtr->cueAngle(AngleCueData(A_LowerlegRV, JT_RKNEE, JT_RANKLE, AT_LimbVR, A_LowerlegRH));
}

void Dg_BodyAnglesDyn::OnEnSetfocusFootLvDyn()
{
	Current_Control_ID = IDC_FOOT_LV_DYN;
	mDocPtr->cueAngle(AngleCueData(A_FootLV, JT_LHEEL, JT_LBALL, AT_LimbVL, A_FootLH));
}

void Dg_BodyAnglesDyn::OnEnSetfocusFootRvDyn()
{
	Current_Control_ID = IDC_FOOT_RV_DYN;
	mDocPtr->cueAngle(AngleCueData(A_FootRV, JT_RHEEL, JT_RBALL, AT_LimbVR, A_FootRH));
}

void Dg_BodyAnglesDyn::OnEnSetfocusTrunkFlexionDyn()
{
	Current_Control_ID = IDC_TRUNK_FLEXION_DYN;
	mDocPtr->cueAngle(AngleCueData(A_TrunkFlexion, JT_HIP_CENTER, JT_C7T1, AT_TrunkF));
}

void Dg_BodyAnglesDyn::OnEnSetfocusTrunkAxialRotationDyn()
{
	Current_Control_ID = IDC_TRUNK_AXIAL_ROTATION_DYN;
	mDocPtr->cancelCue();
}

void Dg_BodyAnglesDyn::OnEnSetfocusTrunkLateralBendingDyn()
{
	Current_Control_ID = IDC_TRUNK_LATERAL_BENDING_DYN;
	mDocPtr->cueAngle(AngleCueData(A_TrunkLateralBending, JT_HIP_CENTER, JT_C7T1, AT_TrunkLB));
}

void Dg_BodyAnglesDyn::OnEnSetfocusPelvicLateralTiltDyn()
{
	Current_Control_ID = IDC_PELVIC_LATERAL_TILT_DYN;
	mDocPtr->cueAngle(AngleCueData(A_PelvisLateralBending, JT_HIP_CENTER, JT_L5S1_TEMP, AT_PelvisLB));
}

void Dg_BodyAnglesDyn::OnEnSetfocusHeadFlexionDyn()
{
	Current_Control_ID = IDC_HEADFLEXION;
	mDocPtr->cancelCue();
}

void Dg_BodyAnglesDyn::OnEnSetfocusHeadRotDyn()
{
	Current_Control_ID = IDC_HEADROTATION;
	mDocPtr->cancelCue();
}

void Dg_BodyAnglesDyn::OnEnSetfocusHeadLateralDyn()
{
	Current_Control_ID = IDC_HEADBENDING;
	mDocPtr->cancelCue();
}

void Dg_BodyAnglesDyn::OnEnSetfocusNeck1FlexionDyn()
{
	Current_Control_ID = IDC_NECK1_FLEXION_DYN;
	mDocPtr->cancelCue();
}

void Dg_BodyAnglesDyn::OnEnSetfocusNeck1RotDyn()
{
	Current_Control_ID = IDC_NECK1_ROT_DYN;
	mDocPtr->cancelCue();
}

void Dg_BodyAnglesDyn::OnEnSetfocusNeck1LateralDyn()
{
	Current_Control_ID = IDC_NECK1_LATERAL_DYN;
	mDocPtr->cancelCue();
}

void Dg_BodyAnglesDyn::OnEnSetfocusNeck2FlexionDyn()
{
	Current_Control_ID = IDC_NECK2_FLEXION_DYN;
	mDocPtr->cancelCue();
}

void Dg_BodyAnglesDyn::OnEnSetfocusNeck2RotDyn()
{
	Current_Control_ID = IDC_NECK2_ROT_DYN;
	mDocPtr->cancelCue();
}

void Dg_BodyAnglesDyn::OnEnSetfocusNeck2LateralDyn()
{
	Current_Control_ID = IDC_NECK2_LATERAL_DYN;
	mDocPtr->cancelCue();
}

void Dg_BodyAnglesDyn::OnEnSetfocusClavicleLhDyn()
{
	Current_Control_ID = IDC_CLAVICLE_LH_DYN;
	mDocPtr->cancelCue();
}

void Dg_BodyAnglesDyn::OnEnSetfocusClavicleLvDyn()
{
	Current_Control_ID = IDC_CLAVICLE_LV_DYN;
	mDocPtr->cancelCue();
}

void Dg_BodyAnglesDyn::OnEnSetfocusClavicleRhDyn()
{
	Current_Control_ID = IDC_CLAVICLE_RH_DYN;
	mDocPtr->cancelCue();
}

void Dg_BodyAnglesDyn::OnEnSetfocusClavicleRvDyn()
{
	Current_Control_ID = IDC_CLAVICLE_RV_DYN;
	mDocPtr->cancelCue();
}

void Dg_BodyAnglesDyn::OnEnSetfocusLowerlegLhDyn()
{
	Current_Control_ID = IDC_LOWERLEG_LH_DYN;
	mDocPtr->cueAngle(AngleCueData(A_LowerlegLH, JT_LKNEE, JT_LANKLE, AT_LimbHL, A_LowerlegLV));
}

void Dg_BodyAnglesDyn::OnEnSetfocusLowerlegRhDyn()
{
	Current_Control_ID = IDC_LOWERLEG_RH_DYN;
	mDocPtr->cueAngle(AngleCueData(A_LowerlegRH, JT_RKNEE, JT_RANKLE, AT_LimbHR, A_LowerlegRV));
}

void Dg_BodyAnglesDyn::OnEnSetfocusFootLhDyn()
{
	Current_Control_ID = IDC_FOOT_LH_DYN;
	mDocPtr->cueAngle(AngleCueData(A_FootLH, JT_LHEEL, JT_LBALL, AT_LimbHL, A_FootLV));
}

void Dg_BodyAnglesDyn::OnEnSetfocusFootRhDyn()
{
	Current_Control_ID = IDC_FOOT_RH_DYN;
	mDocPtr->cueAngle(AngleCueData(A_FootRH, JT_RHEEL, JT_RBALL, AT_LimbHR, A_FootRV));
}

void Dg_BodyAnglesDyn::OnEnSetfocusPelvicAxialRotationDyn()
{
	Current_Control_ID = IDC_PELVIC_AXIAL_ROTATION_DYN;
	mDocPtr->cancelCue();
}


void Dg_BodyAnglesDyn::HeadGlobalControl()
{
	Customizedneck = 0;
}


void Dg_BodyAnglesDyn::OnBnClickedOk()
{
	if(this->CWnd::UpdateData(TRUE /*Retrieve*/)) //Data transfer successful 
	{
		if(this->UpdateDocument())
		{
			this->UpdateDocument();
			CDialog::OnOK();
		}
	}
}

void Dg_BodyAnglesDyn::OnBnClickedRedraw()
{
	if (this->CWnd::UpdateData(TRUE)) //Data transfer successful 
		this->UpdateDocument();
	//if (this->CWnd::UpdateData(TRUE)) //Data transfer successful 
	//	this->UpdateDocument();
}

void Dg_BodyAnglesDyn::OnBnClickedNeutralbutton()
{
	double angles[NUM_EXTRA_ANGLES];
	this->SetJointAngles( mDocPtr->getTNeutralJointAngles(), angles, 1);
 
	if (this->CWnd::UpdateData(FALSE)) //Transfer successful 
	{
		Skeleton& skel = *mDocPtr->GetSkeleton();
		JointAngles oldAngles = skel.GetAngles();
		JointAngles newJointAngles = mDocPtr->getTNeutralJointAngles();
		//int frame = skel.getCurrentFrame();
		int frame = mDocPtr->getCurrentFrame();
		//skel.SetAngles(frame,newJointAngles);
		skel.SetAngles(newJointAngles);
		UndoablePostureChange *undoEvent = new UndoablePostureChange(frame,oldAngles,newJointAngles,mDocPtr->getFrameType(frame), FT_Keyframe);
		mDocPtr->addUndoEvent(undoEvent);
		mDocPtr->MakeDirtyAndUpdateViews();

		// Gray/ungray the Undo and Redo buttons if necessary	
		UndoButtonStatus();
		RedoButtonStatus();
	}
}

void Dg_BodyAnglesDyn::OnBnClickedUndobutton()
{
	if(!mDocPtr->UndoJointAngleChange())
		return; 

	this->GetCurrentJointAnglesFromDocument();

    this->CWnd::UpdateData(FALSE /*Put to dialog*/);

	// Disable the Undo and Redo buttons if necessary	
	UndoButtonStatus();
    RedoButtonStatus();
}

void Dg_BodyAnglesDyn::OnBnClickedRedobutton()
{
	if(!mDocPtr->RedoJointAngleChange())
		return;

	this->GetCurrentJointAnglesFromDocument();

    this->CWnd::UpdateData(FALSE /*Put to dialog*/);

	// Disable the Undo and Redo buttons if necessary	
	UndoButtonStatus();
    RedoButtonStatus();
}

void Dg_BodyAnglesDyn::OnBnClickedIncrementPlusDyn()
{
	Increment(true);
}

void Dg_BodyAnglesDyn::OnBnClickedIncrementMinusDyn()
{
	Increment(false);
}

/*--------------------------------------------------------------*/
void Dg_BodyAnglesDyn::Increment(bool isIncrementing)
{
	// Sign will cause the function to increment or decrement,
	//		depending on the value of isIncrementing
	int sign, minval, maxval;

	if(isIncrementing)
		sign = 1;
	else	// Decrementing
		sign = -1;

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

	if (this->Current_Control_ID == IDC_FOREARM_LV_DYN || this->Current_Control_ID == IDC_UPPERARM_LV_DYN || this->Current_Control_ID == IDC_CLAVICLE_LV_DYN || 
		this->Current_Control_ID == IDC_UPPERLEG_LV_DYN || this->Current_Control_ID == IDC_LOWERLEG_LV_DYN || this->Current_Control_ID == IDC_FOOT_LV_DYN ||
		this->Current_Control_ID == IDC_FOREARM_RV_DYN || this->Current_Control_ID == IDC_UPPERARM_RV_DYN || this->Current_Control_ID == IDC_CLAVICLE_RV_DYN ||
		this->Current_Control_ID == IDC_UPPERLEG_RV_DYN || this->Current_Control_ID == IDC_LOWERLEG_RV_DYN || this->Current_Control_ID == IDC_FOOT_RV_DYN){
			minval = -90;
			maxval = 90;
	}
	else{
		minval = -180;
		maxval = 180;
	}

	if (newVal >= minval && newVal <= maxval){
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
}

void Dg_BodyAnglesDyn::OnBnClickedSymmetryRToLDyn()
{
	if (!this->CWnd::UpdateData(TRUE /*Get data from dialog*/))
		return;

   // Do symmetry operation      
	this->Forearm_LH  = this->Forearm_RH;
	this->Forearm_LV  = this->Forearm_RV;
	this->Upperarm_LH = this->Upperarm_RH;
	this->Upperarm_LV = this->Upperarm_RV;
	this->Clavicle_LH = this->Clavicle_RH;
	this->Clavicle_LV = this->Clavicle_RV;
	this->Upperleg_LH = this->Upperleg_RH;
	this->Upperleg_LV = this->Upperleg_RV;
	this->Lowerleg_LH = this->Lowerleg_RH;
	this->Lowerleg_LV = this->Lowerleg_RV;
	this->Foot_LH     = this->Foot_RH;
	this->Foot_LV	  = this->Foot_RV;
	
	this->CWnd::UpdateData(FALSE /*Return data to dialog*/);

	if (this->UpdateData(TRUE)) {
		if (this->UpdateDocument())
		{
			this->UpdateDocument();
			//CDialog::OnOK();
		}
	}
}

void Dg_BodyAnglesDyn::OnBnClickedSymmetryLToRDyn()
{
   if (!this->CWnd::UpdateData(TRUE /*Get data from dialog*/))
		return;

   // Do symmetry operation      
   this->Forearm_RH  = this->Forearm_LH;
   this->Forearm_RV  = this->Forearm_LV;
   this->Upperarm_RH = this->Upperarm_LH;
   this->Upperarm_RV = this->Upperarm_LV;
   this->Clavicle_RH = this->Clavicle_LH;
   this->Clavicle_RV = this->Clavicle_LV;
   this->Upperleg_RH = this->Upperleg_LH;
   this->Upperleg_RV = this->Upperleg_LV;
   this->Lowerleg_RH = this->Lowerleg_LH;
   this->Lowerleg_RV = this->Lowerleg_LV;
   this->Foot_RH     = this->Foot_LH;
   this->Foot_RV	 = this->Foot_LV;
   
   this->CWnd::UpdateData(FALSE /*Return data to dialog*/);

	if (this->UpdateData(TRUE)) {
		if (this->UpdateDocument())
		{
			this->UpdateDocument();
			//CDialog::OnOK();
		}
	}
}

void Dg_BodyAnglesDyn::UndoButtonStatus()
{
	mUndoButtonControl.EnableWindow(mDocPtr->CanUndo());
}

void Dg_BodyAnglesDyn::RedoButtonStatus()
{
	mRedoButtonControl.EnableWindow(mDocPtr->CanRedo());
}

BOOL Dg_BodyAnglesDyn::OnInitDialog()
{
	CDialog::OnInitDialog();
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->AppendMenu(MF_SEPARATOR);// add a separator
	pSysMenu->AppendMenu(MF_STRING,IDR_MENU1,"Print Window");// add print menu
	mMaintainHandPosture = mDocPtr->getMaintainHandAngles();
	mMaintainHeadPosture = mDocPtr->getMaintainHandAngles();
	UndoButtonStatus();
    RedoButtonStatus();

	//UpdateData(false);
	diff0 = HeadFlexion - Trunk_Flexion;
	diff1 = UpperNeckFlexion - Trunk_Flexion;
    diff2 = LowerNeckFlexion - Trunk_Flexion;
	diff3 = UpperNeckAxialRotation - Trunk_Axial_Rotation;
    diff4 = UpperNeckLateralBending - Trunk_Lateral_Bending;
	diff5 = LowerNeckAxialRotation - Trunk_Axial_Rotation;
    diff6 = LowerNeckLateralBending - Trunk_Lateral_Bending;
	diff7 = Head_Flexion - Trunk_Flexion;
	diff8 = Head_Lateral_Bending - Trunk_Lateral_Bending;
	diff9 = Head_Axial_Rotation - Trunk_Axial_Rotation;
	return TRUE;  // return TRUE unless you set the focus to a control
}

void Dg_BodyAnglesDyn::OnClose()
{
	mDocPtr->cancelCue();
	CDialog::OnClose();
}


void Dg_BodyAnglesDyn::OnBnClickedHandangles()
{
	Dg_HandPosture dialog;
	dialog.DoModal();
}


void Dg_BodyAnglesDyn::OnBnClickedMaintainhandposture()
{
	// TODO: Add your control notification handler code here
	mMaintainHandPosture = !mMaintainHandPosture;
	if (mMaintainHandPosture)
	{
		// Add code to save current data for wrist
	}

	UpdateDocument();
}

void Dg_BodyAnglesDyn::OnBnClickedMaintainheadposture()
{
	// TODO: Add your control notification handler code here
	mMaintainHeadPosture = !mMaintainHeadPosture;
	UpdateDocument();
}


void Dg_BodyAnglesDyn::OnPrintPrintwindow()
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


void Dg_BodyAnglesDyn::OnSysCommand(UINT nID, LPARAM lParam)
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


void Dg_BodyAnglesDyn::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	Dg_HeadNeck dgl(this);
	dgl.DoModal();
}


bool Dg_BodyAnglesDyn::UpdateAngles(void)
{
		//if(SameAsDocument())
	// 	return true;	// Do not update; no changes

	bool lUpdateOK = true;
	JointAngles newJointAngles = mDocPtr->GetSkeleton()->GetAngles();
	JointAngles oldAngles = newJointAngles;
	double newEAngles[NUM_EXTRA_ANGLES];
	mDocPtr->GetSkeleton()-> getExtraAngles(newEAngles);
	Skeleton& skel = *mDocPtr->GetSkeleton();
	BuildJointAngles(newJointAngles, newEAngles);

	// if locking hand angles to forearm angles, set hand angles
	if(mDocPtr->getFixedHandAngles())
	{
		newJointAngles.SetStraightHands();
	} 
	/*else if(mMaintainHandPosture)
	{
		if(!skel.ValidateJointAngles(newJointAngles, true)) //validation failed
		{
			AfxMessageBox(skel.getLastError().c_str());
			return false;
		}
	} 
	else 
	{
		if(!skel.ValidateJointAngles(newJointAngles, false)) 
		{
 			AfxMessageBox(skel.getLastError().c_str());
			return false;
		}
	}*/  

	mDocPtr->setMaintainHandAngles(mMaintainHandPosture);
	// redundant?
	mDocPtr->setMaintainHeadAngles(mMaintainHeadPosture);
	skel.SetAngles(newJointAngles);
	int frame = mDocPtr->getCurrentFrame();
	UndoablePostureChange *undoEvent = new UndoablePostureChange(frame,oldAngles,newJointAngles, mDocPtr->getFrameType(frame), FT_Keyframe);
	if(mDocPtr->getFrameType() != FT_Unset) {
		mDocPtr->setFrameType(FT_Keyframe);
	}
	mDocPtr->addUndoEvent(undoEvent);
	mDocPtr->MakeDirtyAndUpdateViews();

	GetCurrentJointAnglesFromDocument();

	UpdateData(false);
	// Gray/ungray the Undo and Redo buttons if necessary	
	UndoButtonStatus();
    RedoButtonStatus();
	return lUpdateOK;
}

BOOL Dg_BodyAnglesDyn::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if(zDelta < 0) 
		Increment(false);
	else if (zDelta > 0)
		Increment(true);
	
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}