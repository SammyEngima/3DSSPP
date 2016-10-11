#include "ReportJointMoments.h"

#include "hom.h"
#include "hom_doc.h"
#include "skeleton.hpp"
#include "Types.h"

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

ReportView * ReportJointMoments::cPointer = NULL;

IMPLEMENT_DYNCREATE(ReportJointMoments, ReportView)

ReportJointMoments:: ReportJointMoments() : ReportView( ReportJointMoments::IDD )
{
	//{{AFX_DATA_INIT(ReportJointMoments)
		mHeader = _T("");

	mHandForceUnits = _T("");
	mUnitsLabel = _T("");
	mLeftHori = _T("");

    for(int i = 0; i < 3; ++i)
    {
		mHeadMom[i] = _T(""); 
		mUpperNeckMom[i] = _T("");
		mLowerNeckMom[i] = _T("");
		mC7T1Mom[i] = _T("");
		mL4L5Mom[i] = _T("");
	    mL5S1Mom[i] = _T("");
        mLeftHeelMom[i] = _T("");
        mLeftBallMom[i] = _T("");
	    mLeftAnkleMom[i] = _T("");
	    mLeftElbowMom[i] = _T("");
	    mLeftHipMom[i] = _T("");
	    mLeftKneeMom[i] = _T("");
	    mLeftShoulderMom[i] = _T("");
	    mRightHeelMom[i] = _T("");
        mRightBallMom[i] = _T("");
        mRightAnkleMom[i] = _T("");
	    mRightElbowMom[i] = _T("");
	    mRightHipMom[i] = _T("");
	    mRightKneeMom[i] = _T("");
	    mRightShoulderMom[i] = _T("");
	    mLeftITMom[i] = _T("");
        mRightITMom[i] = _T("");
    }
	mFooter = _T("");
	mMomentUnits = _T("");
	//}}AFX_DATA_INIT
    cPointer = this;

	jointToMember[JT_TRAGION] = 0;
	jointToMember[JT_NASION] = 0;
	jointToMember[JT_NASIONLOC] = 0;
	jointToMember[JT_TRAGIONLOC] = mHeadMom;
	jointToMember[JT_C1]	=mUpperNeckMom;
    jointToMember[JT_C3C4] = mLowerNeckMom;
	jointToMember[JT_C7T1] = mC7T1Mom;
	jointToMember[JT_SCJ] = 0;
	jointToMember[JT_L5S1_TEMP] = mL5S1Mom;
	jointToMember[JT_LSHOULDER] = mLeftShoulderMom;
	jointToMember[JT_LELBOW] = mLeftElbowMom;
	jointToMember[JT_LWRIST] = mLeftWristMom;
	jointToMember[JT_LHAND] = mLeftHandMom;
	jointToMember[JT_RSHOULDER] = mRightShoulderMom;
	jointToMember[JT_RELBOW] = mRightElbowMom;
	jointToMember[JT_RWRIST] = mRightWristMom;
	jointToMember[JT_RHAND] = mRightHandMom;
	jointToMember[JT_LHIP] = mLeftHipMom;
	jointToMember[JT_LIT] = mLeftITMom;
	jointToMember[JT_LKNEE] = mLeftKneeMom;
	jointToMember[JT_LANKLE] = mLeftAnkleMom;
	jointToMember[JT_LHEEL] = mLeftHeelMom;
	jointToMember[JT_LBALL] = mLeftBallMom;
	jointToMember[JT_RHIP] = mRightHipMom;
	jointToMember[JT_RIT] = mRightITMom;
	jointToMember[JT_RKNEE] = mRightKneeMom;
	jointToMember[JT_RANKLE] = mRightAnkleMom;
	jointToMember[JT_RHEEL] = mRightHeelMom;
	jointToMember[JT_RBALL] = mRightBallMom;
	jointToMember[JT_HIP_CENTER] = 0;
	jointToMember[JT_IT_CENTER] = 0;
	jointToMember[JT_ANKLE_CENTER] = 0;
	jointToMember[JT_LFOOT_CENTER] = 0;
	jointToMember[JT_RFOOT_CENTER] = 0;
	jointToMember[JT_BALL_CENTER] = 0;
}

ReportJointMoments::~ReportJointMoments(void) { cPointer = NULL; }

CString ReportJointMoments::ReportName(void) const { return "3DSSPP - Joint Moments Report"; }

void ReportJointMoments::DoDataExchange(CDataExchange* pDX)
{
	ReportView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ReportJointMoments)

	DDX_Text(pDX, IDC_Footer, mFooter);
	DDX_Text(pDX, IDC_Header, mHeader);

		//add head for external force&moments
	
    DDX_Text(pDX, IDC_NeckMomX3, mHeadMom[0]);
    DDX_Text(pDX, IDC_NeckMomY3, mHeadMom[1]);
    DDX_Text(pDX, IDC_NeckMomZ3, mHeadMom[2]);
    DDX_Text(pDX, IDC_NeckMomX, mLowerNeckMom[0]);
    DDX_Text(pDX, IDC_NeckMomY, mLowerNeckMom[1]);
    DDX_Text(pDX, IDC_NeckMomZ, mLowerNeckMom[2]);
    DDX_Text(pDX, IDC_NeckMomX2, mUpperNeckMom[0]);
    DDX_Text(pDX, IDC_NeckMomY2, mUpperNeckMom[1]);
    DDX_Text(pDX, IDC_NeckMomZ2, mUpperNeckMom[2]);
    DDX_Text(pDX, IDC_C7T1MomX, mC7T1Mom[0]);
    DDX_Text(pDX, IDC_C7T1MomY, mC7T1Mom[1]);
    DDX_Text(pDX, IDC_C7T1MomZ, mC7T1Mom[2]);
	DDX_Text(pDX, IDC_L5S1MomX, mL5S1Mom[0]);
	DDX_Text(pDX, IDC_L5S1MomY, mL5S1Mom[1]);
	DDX_Text(pDX, IDC_L5S1MomZ, mL5S1Mom[2]);
	DDX_Text(pDX, IDC_L4L5MomX, mL4L5Mom[0]);
	DDX_Text(pDX, IDC_L4L5MomY, mL4L5Mom[1]);
	DDX_Text(pDX, IDC_L4L5MomZ, mL4L5Mom[2]);
    DDX_Text(pDX, IDC_LeftHeelMomentX, mLeftHeelMom[0]);
	DDX_Text(pDX, IDC_LeftHeelMomentY, mLeftHeelMom[1]);
	DDX_Text(pDX, IDC_LeftHeelMomentZ, mLeftHeelMom[2]);
    DDX_Text(pDX, IDC_LeftBallMomentX, mLeftBallMom[0]);
    DDX_Text(pDX, IDC_LeftBallMomentY, mLeftBallMom[1]);
    DDX_Text(pDX, IDC_LeftBallMomentZ, mLeftBallMom[2]);
	DDX_Text(pDX, IDC_LeftAnkleMomX, mLeftAnkleMom[0]);
	DDX_Text(pDX, IDC_LeftAnkleMomY, mLeftAnkleMom[1]);
	DDX_Text(pDX, IDC_LeftAnkleMomZ, mLeftAnkleMom[2]);
	DDX_Text(pDX, IDC_LeftElbowMomX, mLeftElbowMom[0]);
	DDX_Text(pDX, IDC_LeftElbowMomY, mLeftElbowMom[1]);
	DDX_Text(pDX, IDC_LeftElbowMomZ, mLeftElbowMom[2]);
	DDX_Text(pDX, IDC_LeftHandMomX, mLeftHandMom[0]);
	DDX_Text(pDX, IDC_LeftHandMomY, mLeftHandMom[1]);
	DDX_Text(pDX, IDC_LeftHandMomZ, mLeftHandMom[2]);
	DDX_Text(pDX, IDC_LeftHipMomX, mLeftHipMom[0]);
	DDX_Text(pDX, IDC_LeftHipMomY, mLeftHipMom[1]);
	DDX_Text(pDX, IDC_LeftHipMomZ, mLeftHipMom[2]);
	DDX_Text(pDX, IDC_LeftKneeMomX, mLeftKneeMom[0]);
	DDX_Text(pDX, IDC_LeftKneeMomY, mLeftKneeMom[1]);
	DDX_Text(pDX, IDC_LeftKneeMomZ, mLeftKneeMom[2]);
	DDX_Text(pDX, IDC_LeftShoulderMomX, mLeftShoulderMom[0]);
	DDX_Text(pDX, IDC_LeftShoulderMomY, mLeftShoulderMom[1]);
	DDX_Text(pDX, IDC_LeftShoulderMomZ, mLeftShoulderMom[2]);
	DDX_Text(pDX, IDC_MomentUnits, mMomentUnits);
    DDX_Text(pDX, IDC_RightHeelMomentX, mRightHeelMom[0]);
    DDX_Text(pDX, IDC_RightHeelMomentY, mRightHeelMom[1]);
    DDX_Text(pDX, IDC_RightHeelMomentZ, mRightHeelMom[2]);
    DDX_Text(pDX, IDC_RightBallMomentX, mRightBallMom[0]);
    DDX_Text(pDX, IDC_RightBallMomentY, mRightBallMom[1]);
    DDX_Text(pDX, IDC_RightBallMomentZ, mRightBallMom[2]);
	DDX_Text(pDX, IDC_RightAnkleMomX, mRightAnkleMom[0]);
	DDX_Text(pDX, IDC_RightAnkleMomY, mRightAnkleMom[1]);
	DDX_Text(pDX, IDC_RightAnkleMomZ, mRightAnkleMom[2]);
	DDX_Text(pDX, IDC_RightElbowMomX, mRightElbowMom[0]);
	DDX_Text(pDX, IDC_RightElbowMomY, mRightElbowMom[1]);
	DDX_Text(pDX, IDC_RightElbowMomZ, mRightElbowMom[2]);
	DDX_Text(pDX, IDC_RightHandMomX, mRightHandMom[0]);
	DDX_Text(pDX, IDC_RightHandMomY, mRightHandMom[1]);
	DDX_Text(pDX, IDC_RightHandMomZ, mRightHandMom[2]);
	DDX_Text(pDX, IDC_RightHipMomX, mRightHipMom[0]);
	DDX_Text(pDX, IDC_RightHipMomY, mRightHipMom[1]);
	DDX_Text(pDX, IDC_RightHipMomZ, mRightHipMom[2]);
	DDX_Text(pDX, IDC_RightKneeMomX, mRightKneeMom[0]);
	DDX_Text(pDX, IDC_RightKneeMomY, mRightKneeMom[1]);
	DDX_Text(pDX, IDC_RightKneeMomZ, mRightKneeMom[2]);
	DDX_Text(pDX, IDC_RightShoulderMomX, mRightShoulderMom[0]);
	DDX_Text(pDX, IDC_RightShoulderMomY, mRightShoulderMom[1]);
	DDX_Text(pDX, IDC_RightShoulderMomZ, mRightShoulderMom[2]);
	DDX_Text(pDX, IDC_LeftITMomX, mLeftITMom[0]);
	DDX_Text(pDX, IDC_LeftITMomY, mLeftITMom[1]);
	DDX_Text(pDX, IDC_LeftITMomZ, mLeftITMom[2]);
    DDX_Text(pDX, IDC_RightITMomX, mRightITMom[0]);
    DDX_Text(pDX, IDC_RightITMomY, mRightITMom[1]);
    DDX_Text(pDX, IDC_RightITMomZ, mRightITMom[2]);
	DDX_Text(pDX, IDC_RightWristMomX, mRightWristMom[0]);
	DDX_Text(pDX, IDC_RightWristMomY, mRightWristMom[1]);
	DDX_Text(pDX, IDC_RightWristMomZ, mRightWristMom[2]);
	DDX_Text(pDX, IDC_LeftWristMomX, mLeftWristMom[0]);
	DDX_Text(pDX, IDC_LeftWristMomY, mLeftWristMom[1]);
	DDX_Text(pDX, IDC_LeftWristMomZ, mLeftWristMom[2]);

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

BEGIN_MESSAGE_MAP(ReportJointMoments, ReportView)
	//{{AFX_MSG_MAP(ReportJointMoments)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
	//}}AFX_MSG_MAP
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

#ifdef _DEBUG
void ReportJointMoments::AssertValid() const { CFormView::AssertValid(); }
void ReportJointMoments::Dump(CDumpContext& dc) const { CFormView::Dump(dc); }
#endif

//void ReportJointMoments::OnFilePrintPreview() { SSPPPrintPreview(this); }

void ReportJointMoments::OnUpdate(CView *aSenderPtr,LPARAM aHint,CObject *aHintPtr)
{
    C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();

    // Fill in measurement units text
	mMomentUnits = CString("Moments ") + "(" + lDocPtr->TorqueUnits() + ") -- Due to Body Weight and External Applied Loads";

    // Retrieve joint moment analysis results
	Vector3 moments[NUM_JOINTS];
	lDocPtr->GetSkeleton()->getNetMoments(moments);


    // Write results to the form controls
	for(int i = 0; i < NUM_JOINTS; ++i) { // *****34 must be replaced by NUM_JOINTS once the locations of hand joints are being handled properly*****
		if(jointToMember[i]) {
			for(int j = 0; j < 3; ++j)
			{
				jointToMember[i][j].Format("%.1f",(moments [i][j]));
				if(jointToMember[i][j] == "-0.0") {
					jointToMember[i][j].Format("0");
				}
			}
		}
	}

	Skeleton* skel = lDocPtr->GetSkeleton();
	
	// L4L5 forces
	const SpinalMoments_s& spinalMoments = skel->getSpinalMoments();
	mL4L5Mom[0].Format("%.1f", spinalMoments.globalL4L5NoAbdom[0]);
	mL4L5Mom[1].Format("%.1f", spinalMoments.globalL4L5NoAbdom[1]);
	mL4L5Mom[2].Format("%.1f", spinalMoments.globalL4L5NoAbdom[2]);
		
    // Set any irrelevant values to dashes
	BOOL enableIT = lDocPtr->getPosition() == P_Sitting;
    if(!enableIT)
    {
        mLeftITMom[0] = mLeftITMom[1] = mLeftITMom[2] = "---";
        mRightITMom[0] = mRightITMom[1] = mRightITMom[2] = "---";
    }

    // Gray-out any irrelevant data
    this->GetDlgItem(IDC_LeftITMomX)->EnableWindow(enableIT);
    this->GetDlgItem(IDC_LeftITMomY)->EnableWindow(enableIT);
    this->GetDlgItem(IDC_LeftITMomZ)->EnableWindow(enableIT);
    this->GetDlgItem(IDC_RightITMomX)->EnableWindow(enableIT);
    this->GetDlgItem(IDC_RightITMomY)->EnableWindow(enableIT);
    this->GetDlgItem(IDC_RightITMomZ)->EnableWindow(enableIT);

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

HBRUSH ReportJointMoments::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

    C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();

    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
    pDC->SetBkColor(lDocPtr->getBalColor());

    return (HBRUSH)(CBrush(lDocPtr->getBalColor()));
}

void ReportJointMoments::OnDisplayDialogcolor() 
{
	C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();
	lDocPtr->setReportColor();
	
	Invalidate(true);
}

void ReportJointMoments::OnInitialUpdate()
{
	ReportView::OnInitialUpdate();

	// Set the hand force box units
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();
	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";
	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}


void ReportJointMoments::UpdateUnits() {
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";

	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";

	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}