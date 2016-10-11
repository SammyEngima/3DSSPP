// ReportJointLocations.cpp : implementation file
//

#include "ReportJointLocations.h"

#include "hom.h"
#include "hom_doc.h"	    // Necessary
#include "skeleton.hpp"
//#include "Units.hpp"		// Added for UM_M_CM
#include "enums.h"          //For positioning

#include "GLView.h"
#include "GLBalanceStanding.h"
//#include "GLOblique.h" // TODO for debugging, take out

#include "Main_Frm.h"

#include "wingdi.h"
#include "Analysis.h"
#include "../c4eLibNew/units.hpp"
#include "Anthropometry.hpp"

#include "CDIB.h"
#include "DIB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ReportJointLocations

ReportView * ReportJointLocations::cPointer = NULL;

IMPLEMENT_DYNCREATE(ReportJointLocations, ReportView)

//---------------------------------------------------------------------------
   ReportJointLocations::
ReportJointLocations( void )
:  ReportView( ReportJointLocations::IDD )
{
	//{{AFX_DATA_INIT(ReportJointLocations)
	mFooter = _T("");
	mHeader = _T("");
	mLocationUnits = _T("");

	mHandForceUnits = _T("");
	mUnitsLabel = _T("");
	mLeftHori = _T("");

	//}}AFX_DATA_INIT
   cPointer = this;

   	jointToMember[JT_TRAGION] = 0;
	jointToMember[JT_NASION] = 0;
	jointToMember[JT_NASIONLOC] = mNasion;
	jointToMember[JT_TRAGIONLOC] = mTragion;
	jointToMember[JT_C1] = mNeckTop;
    jointToMember[JT_C3C4] = mNeckMid;
	jointToMember[JT_C7T1] = mC7T1Loc;
	jointToMember[JT_SCJ] = mSCJLoc;
	jointToMember[JT_L5S1_TEMP] = mL5S1Loc;
	jointToMember[JT_LSHOULDER] = mLeftShoulderLoc;
	jointToMember[JT_LELBOW] = mLeftElbowLoc;
	jointToMember[JT_LWRIST] = mLeftWristLoc;
	jointToMember[JT_LHAND] = mLeftHandLoc;
	jointToMember[JT_RSHOULDER] = mRightShoulderLoc;
	jointToMember[JT_RELBOW] = mRightElbowLoc;
	jointToMember[JT_RWRIST] = mRightWristLoc;
	jointToMember[JT_RHAND] = mRightHandLoc;
	jointToMember[JT_LHIP] = mLeftHipLoc;
	jointToMember[JT_LIT] = mLeftITLoc;
	jointToMember[JT_LKNEE] = mLeftKneeLoc;
	jointToMember[JT_LANKLE] = mLeftAnkleLoc;
	jointToMember[JT_LHEEL] = mLeftHeelLoc;
	jointToMember[JT_LBALL] = mLeftBallLoc;
	jointToMember[JT_RHIP] = mRightHipLoc;
	jointToMember[JT_RIT] = mRightITLoc;
	jointToMember[JT_RKNEE] = mRightKneeLoc;
	jointToMember[JT_RANKLE] = mRightAnkleLoc;
	jointToMember[JT_RHEEL] = mRightHeelLoc;
	jointToMember[JT_RBALL] = mRightBallLoc;
	jointToMember[JT_HIP_CENTER] = mHipCenter;
	jointToMember[JT_IT_CENTER] = mITCenter;
	jointToMember[JT_ANKLE_CENTER] = mAnkleCenter;
	jointToMember[JT_LFOOT_CENTER] = mLeftFootCenter;
	jointToMember[JT_RFOOT_CENTER] = mRightFootCenter;
	jointToMember[JT_BALL_CENTER] = mBallCenter;
}

//---------------------------------------------------------------------------
   ReportJointLocations::
~ReportJointLocations( void )
{
   cPointer = NULL;
   return;
}

//---------------------------------------------------------------------------
   CString
   ReportJointLocations::
ReportName
( void ) 
const
{
   return   "3DSSPP - Joint Locations Report";
}

//---------------------------------------------------------------------------
void ReportJointLocations::DoDataExchange(CDataExchange* pDX)
{
	ReportView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ReportJointLocations)
	DDX_Text(pDX, IDC_Footer, mFooter);
	DDX_Text(pDX, IDC_Header, mHeader);
	DDX_Text(pDX, IDC_LeftElbowLocX, mLeftElbowLoc[0]);
	DDX_Text(pDX, IDC_LeftElbowLocY, mLeftElbowLoc[1]);
	DDX_Text(pDX, IDC_LeftElbowLocZ, mLeftElbowLoc[2]);
	DDX_Text(pDX, IDC_LeftHipLocX, mLeftHipLoc[0]);
	DDX_Text(pDX, IDC_LeftHipLocY, mLeftHipLoc[1]);
	DDX_Text(pDX, IDC_LeftHipLocZ, mLeftHipLoc[2]);
	DDX_Text(pDX, IDC_LeftKneeLocX, mLeftKneeLoc[0]);
	DDX_Text(pDX, IDC_LeftKneeLocY, mLeftKneeLoc[1]);
	DDX_Text(pDX, IDC_LeftKneeLocZ, mLeftKneeLoc[2]);
	DDX_Text(pDX, IDC_LeftShoulderLocX, mLeftShoulderLoc[0]);
	DDX_Text(pDX, IDC_LeftShoulderLocY, mLeftShoulderLoc[1]);
	DDX_Text(pDX, IDC_LeftShoulderLocZ, mLeftShoulderLoc[2]);
	DDX_Text(pDX, IDC_RightAnkleLocX, mRightAnkleLoc[0]);
	DDX_Text(pDX, IDC_RightAnkleLocY, mRightAnkleLoc[1]);
	DDX_Text(pDX, IDC_RightElbowLocX, mRightElbowLoc[0]);
	DDX_Text(pDX, IDC_RightElbowLocY, mRightElbowLoc[1]);
	DDX_Text(pDX, IDC_RightAnkleLocZ, mRightAnkleLoc[2]);
	DDX_Text(pDX, IDC_RightElbowLocZ, mRightElbowLoc[2]);
	DDX_Text(pDX, IDC_RightHipLocX, mRightHipLoc[0]);
	DDX_Text(pDX, IDC_RightHipLocY, mRightHipLoc[1]);
	DDX_Text(pDX, IDC_RightHipLocZ, mRightHipLoc[2]);
	DDX_Text(pDX, IDC_RightKneeLocX, mRightKneeLoc[0]);
	DDX_Text(pDX, IDC_RightKneeLocY, mRightKneeLoc[1]);
	DDX_Text(pDX, IDC_RightKneeLocZ, mRightKneeLoc[2]);
	DDX_Text(pDX, IDC_RightShoulderLocX, mRightShoulderLoc[0]);
	DDX_Text(pDX, IDC_RightShoulderLocY, mRightShoulderLoc[1]);
	DDX_Text(pDX, IDC_RightShoulderLocZ, mRightShoulderLoc[2]);
	DDX_Text(pDX, IDC_TragionLocX, mTragion[0]);
	DDX_Text(pDX, IDC_TragionLocY, mTragion[1]);
	DDX_Text(pDX, IDC_TragionLocZ, mTragion[2]);
	DDX_Text(pDX, IDC_NasionLocX, mNasion[0]);
	DDX_Text(pDX, IDC_NasionLocY, mNasion[1]);
	DDX_Text(pDX, IDC_NasionLocZ, mNasion[2]);
	DDX_Text(pDX, IDC_C1LocX, mNeckTop[0]);
	DDX_Text(pDX, IDC_C1LocY, mNeckTop[1]);
	DDX_Text(pDX, IDC_C1LocZ, mNeckTop[2]);
	DDX_Text(pDX, IDC_NeckMidLocX, mNeckMid[0]);
    DDX_Text(pDX, IDC_NeckMidLocY, mNeckMid[1]);
    DDX_Text(pDX, IDC_NeckMidLocZ, mNeckMid[2]);  
	DDX_Text(pDX, IDC_C7T1LocX, mC7T1Loc[0]);
	DDX_Text(pDX, IDC_C7T1LocY, mC7T1Loc[1]);
	DDX_Text(pDX, IDC_C7T1LocZ, mC7T1Loc[2]);
	DDX_Text(pDX, IDC_SCJLocX, mSCJLoc[0]);
	DDX_Text(pDX, IDC_SCJLocY, mSCJLoc[1]);
	DDX_Text(pDX, IDC_SCJLocZ, mSCJLoc[2]);
	DDX_Text(pDX, IDC_L5S1LocX, mL5S1Loc[0]);
	DDX_Text(pDX, IDC_L5S1LocY, mL5S1Loc[1]);
	DDX_Text(pDX, IDC_L5S1LocZ, mL5S1Loc[2]);
	DDX_Text(pDX, IDC_HipCenterLocX, mHipCenter[0]);
	DDX_Text(pDX, IDC_HipCenterLocY, mHipCenter[1]);
	DDX_Text(pDX, IDC_HipCenterLocZ, mHipCenter[2]);
	DDX_Text(pDX, IDC_ITCenterLocX, mITCenter[0]);
	DDX_Text(pDX, IDC_ITCenterLocY, mITCenter[1]);
	DDX_Text(pDX, IDC_ITCenterLocZ, mITCenter[2]);
	DDX_Text(pDX, IDC_AnkleCenterLocX, mAnkleCenter[0]);
	DDX_Text(pDX, IDC_AnkleCenterLocY, mAnkleCenter[1]);
	DDX_Text(pDX, IDC_AnkleCenterLocZ, mAnkleCenter[2]);
	DDX_Text(pDX, IDC_BallCenterLocX, mBallCenter[0]);
	DDX_Text(pDX, IDC_BallCenterLocY, mBallCenter[1]);
	DDX_Text(pDX, IDC_BallCenterLocZ, mBallCenter[2]);
	
	DDX_Text(pDX, IDC_SeatBackLocX, mSeatBack[0]);
	DDX_Text(pDX, IDC_SeatBackLocY, mSeatBack[1]);
	DDX_Text(pDX, IDC_SeatBackLocZ, mSeatBack[2]);
	DDX_Text(pDX, IDC_SRPLocX, mSRP[0]);
	DDX_Text(pDX, IDC_SRPLocY, mSRP[1]);
	DDX_Text(pDX, IDC_SRPLocZ, mSRP[2]);
	DDX_Text(pDX, IDC_ForwardSeatLocX, mSeatForward[0]);
	DDX_Text(pDX, IDC_ForwardSeatLocY, mSeatForward[1]);
	DDX_Text(pDX, IDC_ForwardSeatLocZ, mSeatForward[2]);

	DDX_Text(pDX, IDC_LeftFootCenterX, mLeftFootCenter[0]);
	DDX_Text(pDX, IDC_LeftFootCenterY, mLeftFootCenter[1]);
	DDX_Text(pDX, IDC_LeftFootCenterZ, mLeftFootCenter[2]);
	DDX_Text(pDX, IDC_RightFootCenterX, mRightFootCenter[0]);
	DDX_Text(pDX, IDC_RightFootCenterY, mRightFootCenter[1]);
	DDX_Text(pDX, IDC_RightFootCenterZ, mRightFootCenter[2]);

	DDX_Text(pDX, IDC_LeftAnkleLocX, mLeftAnkleLoc[0]);
	DDX_Text(pDX, IDC_LeftAnkleLocY, mLeftAnkleLoc[1]);
	DDX_Text(pDX, IDC_LeftAnkleLocZ, mLeftAnkleLoc[2]);
	DDX_Text(pDX, IDC_LocationUnits, mLocationUnits);
	DDX_Text(pDX, IDC_LeftHandLocX, mLeftHandLoc[0]);
	DDX_Text(pDX, IDC_LeftHandLocY, mLeftHandLoc[1]);
	DDX_Text(pDX, IDC_LeftHandLocZ, mLeftHandLoc[2]);
	DDX_Text(pDX, IDC_RightHandLocX, mRightHandLoc[0]);
	DDX_Text(pDX, IDC_RightHandLocY, mRightHandLoc[1]);
	DDX_Text(pDX, IDC_RightHandLocZ, mRightHandLoc[2]);
	DDX_Text(pDX, IDC_RightBallOfFootX, mRightBallLoc[0]);
	DDX_Text(pDX, IDC_RightBallOfFootY, mRightBallLoc[1]);
	DDX_Text(pDX, IDC_RightBallOfFootZ, mRightBallLoc[2]);
	DDX_Text(pDX, IDC_LeftBallOfFootX, mLeftBallLoc[0]);
	DDX_Text(pDX, IDC_LeftBallOfFootY, mLeftBallLoc[1]);
	DDX_Text(pDX, IDC_LeftBallOfFootZ, mLeftBallLoc[2]);
	DDX_Text(pDX, IDC_RIGHTHEELX, mRightHeelLoc[0]);
	DDX_Text(pDX, IDC_RIGHTHEELY, mRightHeelLoc[1]);
	DDX_Text(pDX, IDC_RIGHTHEELZ, mRightHeelLoc[2]);
	DDX_Text(pDX, IDC_LEFTHEELX, mLeftHeelLoc[0]);
	DDX_Text(pDX, IDC_LEFTHEELY, mLeftHeelLoc[1]);
	DDX_Text(pDX, IDC_LEFTHEELZ, mLeftHeelLoc[2]);
	DDX_Text(pDX, IDC_LeftITLocX, mLeftITLoc[0]);
	DDX_Text(pDX, IDC_LeftITLocY, mLeftITLoc[1]);
	DDX_Text(pDX, IDC_LeftITLocZ, mLeftITLoc[2]);
	DDX_Text(pDX, IDC_RightITLocX, mRightITLoc[0]);
	DDX_Text(pDX, IDC_RightITLocY, mRightITLoc[1]);
	DDX_Text(pDX, IDC_RightITLocZ, mRightITLoc[2]);
	DDX_Text(pDX, IDC_RIGHTFORSEATX, mRightForSeat[0]);
	DDX_Text(pDX, IDC_RIGHTFORSEATY, mRightForSeat[1]);
	DDX_Text(pDX, IDC_RIGHTFORSEATZ, mRightForSeat[2]);
	DDX_Text(pDX, IDC_LEFTFORSEATX, mLeftForSeat[0]);
	DDX_Text(pDX, IDC_LEFTFORSEATY, mLeftForSeat[1]);
	DDX_Text(pDX, IDC_LEFTFORSEATZ, mLeftForSeat[2]);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_LeftWristLocX, mLeftWristLoc[0]);
	DDX_Text(pDX, IDC_LeftWristLocY, mLeftWristLoc[1]);
	DDX_Text(pDX, IDC_LeftWristLocZ, mLeftWristLoc[2]);
	DDX_Text(pDX, IDC_RightWristLocX, mRightWristLoc[0]);
	DDX_Text(pDX, IDC_RightWristLocY, mRightWristLoc[1]);
	DDX_Text(pDX, IDC_RightWristLocZ, mRightWristLoc[2]);

	DDX_Text(pDX, IDC_HandForceUnits, mHandForceUnits);
	DDX_Text(pDX, IDC_HAND_FORCE_TXT_LVAL, mHandForceTxtLVal);
	DDX_Text(pDX, IDC_HAND_FORCE_TXT_RVAL, mHandForceTxtRVal);
	DDX_Text(pDX, IDC_UnitsLabel, mUnitsLabel);
	DDX_Text(pDX, IDC_RightHori, mRightHori);
	DDX_Text(pDX, IDC_RightLat2, mRightLat);
	DDX_Text(pDX, IDC_RightVert2, mRightVert);
	DDX_Text(pDX, IDC_LeftHori, mLeftHori);
	DDX_Text(pDX, IDC_LeftLat2, mLeftLat);
	DDX_Text(pDX, IDC_LeftVert2, mLeftVert);
}

//---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(ReportJointLocations, ReportView)
	//{{AFX_MSG_MAP(ReportJointLocations)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
	//}}AFX_MSG_MAP
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
    //ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ReportJointLocations diagnostics

#ifdef _DEBUG
void ReportJointLocations::AssertValid() const
{
	CFormView::AssertValid();
}

void ReportJointLocations::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ReportJointLocations overrides

//void ReportJointLocations::OnFilePrintPreview()
//{
//	SSPPPrintPreview(this);
//}

//---------------------------------------------------------------------------
void ReportJointLocations::OnUpdate(CView *   aSenderPtr,
                                    LPARAM    aHint, 
                                    CObject * aHintPtr
                                   )
{
    C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();

    mHeader = lDocPtr->ReportHeader();
    
	bool standing = lDocPtr->getPosition() == P_Standing;

    //--- Joint Locations ---
    Skeleton &lSkeleton = *lDocPtr->GetSkeleton();

    //--------------------------------------

	mLocationUnits = CString("Locations ") + "(" + lDocPtr->LengthUnits() + ")";

	Vector3 tempPoint;
	Vector3 offsetCoordinate = lDocPtr->getOffset();

	for(int i = 0; i < NUM_JOINTS; i++) { // *****33 must be replaced by NUM_JOINTS once the locations of hand joints are being handled properly*****
		if(jointToMember[i]) {
			tempPoint = lSkeleton.getJoint((JointID)i);
			for(int j = 0; j < 3; j++) {
				switch (j) {
					 case 0:
				          jointToMember[i][j].Format("%.1f", (tempPoint[j] - offsetCoordinate[0]));
						  break;
					 case 1:
						  jointToMember[i][j].Format("%.1f", (tempPoint[j] - offsetCoordinate[1])); 
						  break;
					 case 2:
                          jointToMember[i][j].Format("%.1f", (tempPoint[j] - offsetCoordinate[2])); 
						  break;
				}		   
			}
		}
	}

	bool backrest = lDocPtr->hasSeatBackRest();
	if(!standing) {
		Vector3 rightFrontSeat = lSkeleton.getRightSeatFront() - offsetCoordinate;
		Vector3 leftFrontSeat = lSkeleton.getLeftSeatFront() - offsetCoordinate;
		Vector3 seatBack = lSkeleton.getSeatBackCenter() - offsetCoordinate;
		Vector3 SRP = lSkeleton.getSeatReference() - offsetCoordinate;
		Vector3 forwardSeat = lSkeleton.getSeatCenter() - offsetCoordinate;
		for(int i = 0; i < 3; i++) {
			mRightForSeat[i].Format("%.1f", rightFrontSeat[i]);
			mLeftForSeat[i].Format("%.1f", leftFrontSeat[i]);
			mSeatBack[i].Format("%.1f", seatBack[i]);
			mSRP[i].Format("%.1f", SRP[i]);
			mSeatForward[i].Format("%.1f", forwardSeat[i]);
			if(!backrest) {
				mSeatBack[i] = "---";
			}
		}
	} else {
		for(int i = 0; i < 3; ++i) {
			mLeftITLoc[i] = "---";
			mRightITLoc[i] = "---";
			mLeftForSeat[i] = "---";
			mRightForSeat[i] = "---";
			mITCenter[i] = "---";
			mSeatBack[i] = "---";
			mSeatForward[i] = "---";
			mSRP[i] = "---";
		}
    }

	this->GetDlgItem(IDC_LeftITLocX)->EnableWindow(!standing);
    this->GetDlgItem(IDC_LeftITLocY)->EnableWindow(!standing);
    this->GetDlgItem(IDC_LeftITLocZ)->EnableWindow(!standing);
    this->GetDlgItem(IDC_RightITLocX)->EnableWindow(!standing);
    this->GetDlgItem(IDC_RightITLocY)->EnableWindow(!standing);
    this->GetDlgItem(IDC_RightITLocZ)->EnableWindow(!standing);
    this->GetDlgItem(IDC_LEFTFORSEATX)->EnableWindow(!standing);
    this->GetDlgItem(IDC_LEFTFORSEATY)->EnableWindow(!standing);
    this->GetDlgItem(IDC_LEFTFORSEATZ)->EnableWindow(!standing);
    this->GetDlgItem(IDC_RIGHTFORSEATX)->EnableWindow(!standing);
    this->GetDlgItem(IDC_RIGHTFORSEATY)->EnableWindow(!standing);
    this->GetDlgItem(IDC_RIGHTFORSEATZ)->EnableWindow(!standing);
	this->GetDlgItem(IDC_ITCenterLocX)->EnableWindow(!standing);
	this->GetDlgItem(IDC_ITCenterLocY)->EnableWindow(!standing);
	this->GetDlgItem(IDC_ITCenterLocZ)->EnableWindow(!standing);
	this->GetDlgItem(IDC_SeatBackLocX)->EnableWindow(!standing && backrest);
	this->GetDlgItem(IDC_SeatBackLocY)->EnableWindow(!standing && backrest);
	this->GetDlgItem(IDC_SeatBackLocZ)->EnableWindow(!standing && backrest);
	this->GetDlgItem(IDC_ForwardSeatLocX)->EnableWindow(!standing);
	this->GetDlgItem(IDC_ForwardSeatLocY)->EnableWindow(!standing);
	this->GetDlgItem(IDC_ForwardSeatLocZ)->EnableWindow(!standing);
	this->GetDlgItem(IDC_SRPLocX)->EnableWindow(!standing);
	this->GetDlgItem(IDC_SRPLocY)->EnableWindow(!standing);
	this->GetDlgItem(IDC_SRPLocZ)->EnableWindow(!standing);

    mFooter = lDocPtr->ReportFooter();
       
    UpdateData( FALSE );
	UpdateUnits();

	mHandForceTxtLVal.Format("%.1f", lSkeleton.getExtForce(JT_LHAND).length());
	mHandForceTxtRVal.Format("%.1f", lSkeleton.getExtForce(JT_RHAND).length());

	//Vector3 offsetCoordinate = lDocPtr->getOffset();

	Vector3 LHand = lSkeleton.getJoint(JT_LHAND);

    mLeftHori.Format("%.1f", (LHand[1] -  offsetCoordinate[1]));
	mLeftLat.Format("%.1f", (LHand[0] - offsetCoordinate[0]));
	mLeftVert.Format("%.1f", (LHand[2] - offsetCoordinate[2]));


	Vector3 RHand = lSkeleton.getJoint(JT_RHAND);

	mRightHori.Format("%.1f", (RHand[1]  - offsetCoordinate[1]));
	mRightLat.Format("%.1f", (RHand[0] - offsetCoordinate[0]));
	mRightVert.Format("%.1f", (RHand[2] - offsetCoordinate[2]));

    return;
}

/////////////////////////////////////////////////////////////////////////////
// ReportJointLocations message handlers


HBRUSH ReportJointLocations::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
		HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
 
   pDC->SetBkMode(TRANSPARENT);
   pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
   pDC->SetBkColor(lDocPtr->getBalColor());

   return (HBRUSH)(CBrush(lDocPtr->getBalColor()));
}

void ReportJointLocations::OnDisplayDialogcolor() 
{
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
	lDocPtr->setReportColor();
	
	Invalidate(true);
}

void ReportJointLocations::OnInitialUpdate()
{
	ReportView::OnInitialUpdate();

	// Set the hand force box units
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();
	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";
	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}


void ReportJointLocations::UpdateUnits() {
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";

	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";

	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}