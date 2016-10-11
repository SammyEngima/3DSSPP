#include "ReportJointForces.h"

#include "hom.h"
#include "hom_doc.h"
#include "skeleton.hpp"
#include "Types.h"

#include "Dg_HandLoadsAdv.h"

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

ReportView* ReportJointForces::cPointer = NULL;

IMPLEMENT_DYNCREATE(ReportJointForces, ReportView)

ReportJointForces::ReportJointForces() : ReportView(ReportJointForces::IDD)
{
    //{{AFX_DATA_INIT(ReportJointForces)
    mHeader = _T("");

	mHandForceUnits = _T("");
	mUnitsLabel = _T("");
	mLeftHori = _T("");

    for(int i = 0; i < 3; ++i)
    {
		mHeadForce[i] = _T("");
		mUpperNeckForce[i] = _T("");
		mLowerNeckForce[i] = _T("");
		mC7T1Force[i] = _T("");
        mL5S1Force[i] = _T("");
		mL4L5Force[i] = _T("");
        mLeftAnkleForce[i] = _T("");
        mLeftElbowForce[i] = _T("");
        mLeftHipForce[i] = _T("");
        mLeftKneeForce[i] = _T("");
        mLeftShoulderForce[i] = _T("");
        mLeftWristForce[i] = _T("");
		mLeftHandForce[i] = _T("");
        mRightAnkleForce[i] = _T("");
        mRightElbowForce[i] = _T("");
        mRightHipForce[i] = _T("");
        mRightKneeForce[i] = _T("");
        mRightShoulderForce[i] = _T("");
        mRightWristForce[i] = _T("");
		mRightHandForce[i] = _T("");
        mSeatBack[i] = _T("");
        mLeftIT[i] = _T("");
        mRightIT[i] = _T("");
        mSeatFrontRight[i] = _T("");
        mSeatFrontLeft[i] = _T("");
        mLeftHeelForce[i] = _T("");
        mRightHeelForce[i] = _T("");
        mLeftBallForce[i] = _T("");
        mRightBallForce[i] = _T("");
    }
    mFooter = _T("");
    mForceUnits = _T("");
    //}}AFX_DATA_INIT
    cPointer = this;
	//jointToMember[JT_TRAGION] = mHeadForce;
	jointToMember[JT_NASIONLOC] = 0;
	jointToMember[JT_NASION] = 0;
	jointToMember[JT_TRAGION] = 0;
	jointToMember[JT_C1] = mUpperNeckForce;
	jointToMember[JT_TRAGIONLOC] = mHeadForce;

    jointToMember[JT_C3C4] = mLowerNeckForce;
	jointToMember[JT_C7T1] = mC7T1Force;

	jointToMember[JT_SCJ] = 0;
	jointToMember[JT_L5S1_TEMP] = mL5S1Force;
	jointToMember[JT_LSHOULDER] = mLeftShoulderForce;
	jointToMember[JT_LELBOW] = mLeftElbowForce;
	jointToMember[JT_LWRIST] = mLeftWristForce;
	jointToMember[JT_LHAND] = mLeftHandForce;
	jointToMember[JT_RSHOULDER] = mRightShoulderForce;
	jointToMember[JT_RELBOW] = mRightElbowForce;
	jointToMember[JT_RWRIST] = mRightWristForce;
	jointToMember[JT_RHAND] = mRightHandForce;
	jointToMember[JT_LHIP] = mLeftHipForce;
	jointToMember[JT_LIT] = mLeftIT;
	jointToMember[JT_LKNEE] = mLeftKneeForce;
	jointToMember[JT_LANKLE] = mLeftAnkleForce;
	jointToMember[JT_LHEEL] = mLeftHeelForce;
	jointToMember[JT_LBALL] = mLeftBallForce;
	jointToMember[JT_RHIP] = mRightHipForce;
	jointToMember[JT_RIT] = mRightIT;
	jointToMember[JT_RKNEE] = mRightKneeForce;
	jointToMember[JT_RANKLE] = mRightAnkleForce;
	jointToMember[JT_RHEEL] = mRightHeelForce;
	jointToMember[JT_RBALL] = mRightBallForce;
	jointToMember[JT_HIP_CENTER] = 0;
	jointToMember[JT_IT_CENTER] = 0;
	jointToMember[JT_ANKLE_CENTER] = 0;
	jointToMember[JT_LFOOT_CENTER] = 0;
	jointToMember[JT_RFOOT_CENTER] = 0;
	jointToMember[JT_BALL_CENTER] = 0;
}

ReportJointForces::~ReportJointForces() { cPointer = NULL; }

CString ReportJointForces::ReportName() const { return "3DSSPP - Joint Forces Report"; }

void ReportJointForces::DoDataExchange(CDataExchange* pDX)
{
    ReportView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(ReportJointForces)
    DDX_Text(pDX, IDC_Header, mHeader);
    DDX_Text(pDX, IDC_HeadForceX, mHeadForce[0]);
    DDX_Text(pDX, IDC_HeadForceY, mHeadForce[1]);
    DDX_Text(pDX, IDC_HeadForceZ, mHeadForce[2]);
    DDX_Text(pDX, IDC_NeckForceX2, mUpperNeckForce[0]);
    DDX_Text(pDX, IDC_NeckForceY2, mUpperNeckForce[1]);
    DDX_Text(pDX, IDC_NeckForceZ2, mUpperNeckForce[2]);
    DDX_Text(pDX, IDC_NeckForceX, mLowerNeckForce[0]);
    DDX_Text(pDX, IDC_NeckForceY, mLowerNeckForce[1]);
    DDX_Text(pDX, IDC_NeckForceZ, mLowerNeckForce[2]);
    DDX_Text(pDX, IDC_C7T1ForceX, mC7T1Force[0]);
    DDX_Text(pDX, IDC_C7T1ForceY, mC7T1Force[1]);
    DDX_Text(pDX, IDC_C7T1ForceZ, mC7T1Force[2]);
    DDX_Text(pDX, IDC_L5S1ForceX, mL5S1Force[0]);
    DDX_Text(pDX, IDC_L5S1ForceY, mL5S1Force[1]);
    DDX_Text(pDX, IDC_L5S1ForceZ, mL5S1Force[2]);
	DDX_Text(pDX, IDC_L4L5ForceX, mL4L5Force[0]);
	DDX_Text(pDX, IDC_L4L5ForceY, mL4L5Force[1]);
	DDX_Text(pDX, IDC_L4L5ForceZ, mL4L5Force[2]);
    DDX_Text(pDX, IDC_LeftAnkleForceX, mLeftAnkleForce[0]);
    DDX_Text(pDX, IDC_LeftAnkleForceY, mLeftAnkleForce[1]);
    DDX_Text(pDX, IDC_LeftAnkleForceZ, mLeftAnkleForce[2]);
    DDX_Text(pDX, IDC_LeftWristForceX, mLeftWristForce[0]);
    DDX_Text(pDX, IDC_LeftWristForceY, mLeftWristForce[1]);
    DDX_Text(pDX, IDC_LeftWristForceZ, mLeftWristForce[2]);
	DDX_Text(pDX, IDC_LeftHandForceX, mLeftHandForce[0]);
	DDX_Text(pDX, IDC_LeftHandForceY, mLeftHandForce[1]);
	DDX_Text(pDX, IDC_LeftHandForceZ, mLeftHandForce[2]);
    DDX_Text(pDX, IDC_LeftElbowForceX, mLeftElbowForce[0]);
    DDX_Text(pDX, IDC_LeftElbowForceY, mLeftElbowForce[1]);
    DDX_Text(pDX, IDC_LeftElbowForceZ, mLeftElbowForce[2]);
    DDX_Text(pDX, IDC_LeftHipForceX, mLeftHipForce[0]);
    DDX_Text(pDX, IDC_LeftHipForceY, mLeftHipForce[1]);
    DDX_Text(pDX, IDC_LeftHipForceZ, mLeftHipForce[2]);
    DDX_Text(pDX, IDC_LeftKneeForceX, mLeftKneeForce[0]);
    DDX_Text(pDX, IDC_LeftKneeForceY, mLeftKneeForce[1]);
    DDX_Text(pDX, IDC_LeftKneeForceZ, mLeftKneeForce[2]);
    DDX_Text(pDX, IDC_LeftShoulderForceX, mLeftShoulderForce[0]);
    DDX_Text(pDX, IDC_LeftShoulderForceY, mLeftShoulderForce[1]);
    DDX_Text(pDX, IDC_LeftShoulderForceZ, mLeftShoulderForce[2]);
    DDX_Text(pDX, IDC_ForceUnits, mForceUnits);
    DDX_Text(pDX, IDC_RightAnkleForceX, mRightAnkleForce[0]);
    DDX_Text(pDX, IDC_RightAnkleForceY, mRightAnkleForce[1]);
    DDX_Text(pDX, IDC_RightAnkleForceZ, mRightAnkleForce[2]);
    DDX_Text(pDX, IDC_RightWristForceX, mRightWristForce[0]);
    DDX_Text(pDX, IDC_RightWristForceY, mRightWristForce[1]);
    DDX_Text(pDX, IDC_RightWristForceZ, mRightWristForce[2]);
	DDX_Text(pDX, IDC_RightHandForceX, mRightHandForce[0]);
	DDX_Text(pDX, IDC_RightHandForceY, mRightHandForce[1]);
	DDX_Text(pDX, IDC_RightHandForceZ, mRightHandForce[2]);
    DDX_Text(pDX, IDC_RightElbowForceX, mRightElbowForce[0]);
    DDX_Text(pDX, IDC_RightElbowForceY, mRightElbowForce[1]);
    DDX_Text(pDX, IDC_RightElbowForceZ, mRightElbowForce[2]);
    DDX_Text(pDX, IDC_RightHipForceX, mRightHipForce[0]);
    DDX_Text(pDX, IDC_RightHipForceY, mRightHipForce[1]);
    DDX_Text(pDX, IDC_RightHipForceZ, mRightHipForce[2]);
    DDX_Text(pDX, IDC_RightKneeForceX, mRightKneeForce[0]);
    DDX_Text(pDX, IDC_RightKneeForceY, mRightKneeForce[1]);
    DDX_Text(pDX, IDC_RightKneeForceZ, mRightKneeForce[2]);
    DDX_Text(pDX, IDC_RightShoulderForceX, mRightShoulderForce[0]);
    DDX_Text(pDX, IDC_RightShoulderForceY, mRightShoulderForce[1]);
    DDX_Text(pDX, IDC_RightShoulderForceZ, mRightShoulderForce[2]);
    DDX_Text(pDX, IDC_Footer, mFooter);
    DDX_Text(pDX, IDC_SeatBackY, mSeatBack[1]);
    DDX_Text(pDX, IDC_SeatBackZ, mSeatBack[2]);
    DDX_Text(pDX, IDC_LeftITX, mLeftIT[0]);
    DDX_Text(pDX, IDC_LeftITY, mLeftIT[1]);
    DDX_Text(pDX, IDC_LeftITZ, mLeftIT[2]);
    DDX_Text(pDX, IDC_RightITX, mRightIT[0]);
    DDX_Text(pDX, IDC_RightITY, mRightIT[1]);
    DDX_Text(pDX, IDC_RightITZ, mRightIT[2]);
    DDX_Text(pDX, IDC_SeatFrontLeftX, mSeatFrontLeft[0]);
    DDX_Text(pDX, IDC_SeatFrontLeftY, mSeatFrontLeft[1]);
    DDX_Text(pDX, IDC_SeatFrontLeftZ, mSeatFrontLeft[2]);
    DDX_Text(pDX, IDC_SeatFrontRightX, mSeatFrontRight[0]);
    DDX_Text(pDX, IDC_SeatFrontRightY, mSeatFrontRight[1]);
    DDX_Text(pDX, IDC_SeatFrontRightZ, mSeatFrontRight[2]);
    DDX_Text(pDX, IDC_LEFTHEELX, mLeftHeelForce[0]);
	DDX_Text(pDX, IDC_LEFTHEELY, mLeftHeelForce[1]);
	DDX_Text(pDX, IDC_LEFTHEELZ, mLeftHeelForce[2]);
	DDX_Text(pDX, IDC_RIGHTHEELX, mRightHeelForce[0]);
	DDX_Text(pDX, IDC_RIGHTHEELY, mRightHeelForce[1]);
	DDX_Text(pDX, IDC_RIGHTHEELZ, mRightHeelForce[2]);
	DDX_Text(pDX, IDC_LeftBallOfFootX, mLeftBallForce[0]);
	DDX_Text(pDX, IDC_LeftBallOfFootY, mLeftBallForce[1]);
	DDX_Text(pDX, IDC_LeftBallOfFootZ, mLeftBallForce[2]);
	DDX_Text(pDX, IDC_RightBallOfFootX, mRightBallForce[0]);
	DDX_Text(pDX, IDC_RightBallOfFootY, mRightBallForce[1]);
	DDX_Text(pDX, IDC_RightBallOfFootZ, mRightBallForce[2]);

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
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(ReportJointForces, ReportView)
    //{{AFX_MSG_MAP(ReportJointForces)
    ON_WM_CTLCOLOR()
	ON_WM_SIZE()
    ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
    //}}AFX_MSG_MAP
    //ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
    //ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

#ifdef _DEBUG
void ReportJointForces::AssertValid() const { CFormView::AssertValid(); }
void ReportJointForces::Dump(CDumpContext& dc) const { CFormView::Dump(dc); }
#endif

void ReportJointForces::OnUpdate(CView* aSenderPtr,LPARAM aHint,CObject* aHintPtr)
{
    C_Hom_Doc* lDocPtr = (C_Hom_Doc*)GetDocument();
    //Analysis& lResults = lDocPtr->getResultsRef();

    // Determine which data members are relevant to the current posture
    BOOL enableHips = TRUE;
    BOOL enableFront = TRUE;
    BOOL enableBack = TRUE;
	if(lDocPtr->getPosition() == P_Standing)
        enableHips = FALSE;
	if((lDocPtr->getPosition() == P_Standing) || !lDocPtr->hasFrontSeatPanSupport()) // Not seated or front not supported
        enableFront = FALSE;
	if((lDocPtr->getPosition() == P_Standing) || !lDocPtr->hasSeatBackRest()) // Not seated or no seat back
        enableBack = FALSE;

	mForceUnits = CString("Force (") + lDocPtr->ForceUnits() + ") -- Due to Body Weight and External Applied Loads";

    // Retrieve joint force analysis results
	Vector3 forces[NUM_JOINTS];
	lDocPtr->GetSkeleton()->getNetForces(forces);

	for(int i = 0; i < NUM_JOINTS; ++i) {    // *****34 must be replaced by NUM_JOINTS once the locations of hand joints are being handled properly*****
		if(jointToMember[i]) {
			for(int j = 0; j < 3; ++j) {
				jointToMember[i][j].Format("%.1f",  forces[i] [j]);
				if(jointToMember[i][j] == "-0.0")
					jointToMember[i][j].Format("0.0");
			}
		}
	}

	Skeleton* skel = lDocPtr->GetSkeleton();

	// L4L5 forces
	const SpinalForces_s& spinalForces = skel->getSpinalForces();
	mL4L5Force[0].Format("%.1f", spinalForces.globalL4L5NoAbdom[0]);
	mL4L5Force[1].Format("%.1f", spinalForces.globalL4L5NoAbdom[1]);
	mL4L5Force[2].Format("%.1f", spinalForces.globalL4L5NoAbdom[2]);

    // Set any irrelevant data to dashes
    if(!enableBack) {
        mSeatBack[1] = mSeatBack[2] = "---";
	} else {
		Vector3 seatBackForce = skel->getSeatBackForce();
		for(int i = 0; i < 3; i++) {
			// force applied to the seat back is negative of the value we calculated,
			// which was the force applied to the back by the seat back
			mSeatBack[i].Format("%.1f", -seatBackForce[i]);
			if(mSeatBack[i] == "-0.0")
				mSeatBack[i].Format("0.0");
		}
	}
    if(!enableHips) {
		mLeftIT[0] = mLeftIT[1] = mLeftIT[2] = "---";
		mRightIT[0] = mRightIT[1] = mRightIT[2] = "---";
    }
    if(!enableFront) {
		mSeatFrontRight[0] = mSeatFrontRight[1] = mSeatFrontRight[2] = "---";
		mSeatFrontLeft[0] = mSeatFrontLeft[1] = mSeatFrontLeft[2] = "---";
	} else {
		Vector3 frontRightForce = skel->getRightSeatFrontForce();
		Vector3 frontLeftForce = skel->getLeftSeatFrontForce();
		for(int i = 0; i < 3; i++) {
			mSeatFrontRight[i].Format("%.1f",frontRightForce[i]);
			if(mSeatFrontRight[i] == "-0.0")
				mSeatFrontRight[i].Format("0.0");
			mSeatFrontLeft[i].Format("%.1f",frontLeftForce[i]);
			if(mSeatFrontLeft[i] == "-0.0")
				mSeatFrontLeft[i] = "0.0";
		}
	}

    // Gray-out any irrelevent data
	this->GetDlgItem(IDC_LeftITX)->EnableWindow(enableHips);
	this->GetDlgItem(IDC_LeftITY)->EnableWindow(enableHips);
    this->GetDlgItem(IDC_LeftITZ)->EnableWindow(enableHips);
    this->GetDlgItem(IDC_RightITX)->EnableWindow(enableHips);
    this->GetDlgItem(IDC_RightITY)->EnableWindow(enableHips);
    this->GetDlgItem(IDC_RightITZ)->EnableWindow(enableHips);
    this->GetDlgItem(IDC_SeatFrontRightX)->EnableWindow(enableFront);
    this->GetDlgItem(IDC_SeatFrontRightY)->EnableWindow(enableFront);
    this->GetDlgItem(IDC_SeatFrontRightZ)->EnableWindow(enableFront);
    this->GetDlgItem(IDC_SeatFrontLeftX)->EnableWindow(enableFront);
    this->GetDlgItem(IDC_SeatFrontLeftY)->EnableWindow(enableFront);
    this->GetDlgItem(IDC_SeatFrontLeftZ)->EnableWindow(enableFront);
    this->GetDlgItem(IDC_SeatBackY)->EnableWindow(enableBack);
    this->GetDlgItem(IDC_SeatBackZ)->EnableWindow(enableBack);

    mHeader = lDocPtr->ReportHeader();
    mFooter = lDocPtr->ReportFooter();

    UpdateData(FALSE);
	UpdateUnits();

	Skeleton &lSkeleton = *lDocPtr->GetSkeleton();

	mHandForceTxtLVal.Format("%.1f", lSkeleton.getExtForce(JT_LHAND).length());
	mHandForceTxtRVal.Format("%.1f", lSkeleton.getExtForce(JT_RHAND).length());

	Vector3 offsetCoordinate = lDocPtr->getOffset();

	Vector3 LHand = lSkeleton.getJoint(JT_LHAND);

    mLeftHori.Format("%.1f", (LHand[1] -  offsetCoordinate[1]));
	mLeftLat.Format("%.1f", (LHand[0] - offsetCoordinate[0]));
	mLeftVert.Format("%.1f", (LHand[2] - offsetCoordinate[2]));


	Vector3 RHand = lSkeleton.getJoint(JT_RHAND);

	mRightHori.Format("%.1f", (RHand[1]  - offsetCoordinate[1]));
	mRightLat.Format("%.1f", (RHand[0] - offsetCoordinate[0]));
	mRightVert.Format("%.1f", (RHand[2] - offsetCoordinate[2]));
}

//void ReportJointForces::OnFilePrintPreview()
//{
//    SSPPPrintPreview(this);
//}

HBRUSH ReportJointForces::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
    	
    C_Hom_Doc* lDocPtr = (C_Hom_Doc*)GetDocument();
     
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
    pDC->SetBkColor(lDocPtr->getBalColor());

    return (HBRUSH)(CBrush(lDocPtr->getBalColor()));
}

void ReportJointForces::OnDisplayDialogcolor() 
{
    C_Hom_Doc* lDocPtr = (C_Hom_Doc*)GetDocument();
    lDocPtr->setReportColor();
       
    Invalidate(true);
}

void ReportJointForces::OnInitialUpdate()
{
	ReportView::OnInitialUpdate();

	// Set the hand force box units
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();
	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";
	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}


void ReportJointForces::UpdateUnits() {
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";

	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";

	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}
