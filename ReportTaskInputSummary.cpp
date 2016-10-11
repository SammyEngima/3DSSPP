// ReportTaskInputSummary.cpp : implementation file
//

#include "ReportTaskInputSummary.h"

#include "hom.h"
#include "hom_doc.h"
#include "Dg_JointLoads.h"

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
// ReportTaskInputSummary

ReportView * ReportTaskInputSummary::cPointer = NULL;

IMPLEMENT_DYNCREATE(ReportTaskInputSummary, ReportView)

ReportTaskInputSummary::ReportTaskInputSummary()
: ReportView(ReportTaskInputSummary::IDD)
, mLeftWristHori(_T(""))
, mLeftWristVert(_T(""))
, mRightWristHori(_T(""))
, mRightWristVert(_T(""))
, mLeftWristRot(_T(""))
, mRightWristRot(_T(""))
, mLeftUpperlegHori(_T(""))
, mLeftLowerlegHori(_T(""))
, mRightUpperlegHori(_T(""))
, mRightLowerlegHori(_T(""))
, mPelvicAxial(_T(""))
{
	//{{AFX_DATA_INIT(ReportTaskInputSummary)
	mLeftForeArmHori = _T("");
	mLeftForeArmVert = _T("");
	mLeftLowerLegVert = _T("");
	mLeftUpperArmHori = _T("");
	mLeftUpperArmVert = _T("");
	mLeftUpperLegVert = _T("");
	mLeftClavicleVert = _T("");
	mLeftClavicleHori = _T("");
	mRightClavicleVert = _T("");
	mRightClavicleHori = _T("");
	mLeftFootVert = _T("");
	mLeftFootHori = _T("");
	mRightFootVert = _T("");
	mRightFootHori = _T("");
	mRightForeArmHori = _T("");
	mRightForeArmVert = _T("");
	mRightLowerLegVert = _T("");
	mRightUpperArmHori = _T("");
	mRightUpperArmVert = _T("");
	mRightUpperLegVert = _T("");
	mHandOrientation = _T("");
	mLeftHandMag = _T("");
	mRightHandMag = _T("");
	mTrunkBending = _T("");
	mTrunkFlexion = _T("");
	mTrunkRotation = _T("");
	mHeadBending = _T("");
	mHeadFlexion = _T("");
	mHeadRotation = _T("");
	mPelvicTilt = _T("");
	mLeftHandForceHori = _T("");
	mLeftHandForceVert = _T("");
	mLeftHandLocLat = _T("");
	mLeftHandLocVert = _T("");
	mLeftHandLocHori = _T("");
	mRightHandForceHori = _T("");
	mRightHandForceVert = _T("");
	mRightHandLocHori = _T("");
	mRightHandLocLat = _T("");
	mRightHandLocVert = _T("");
	mRightHandForceMag = _T("");
	mLeftHandForceMag = _T("");
	mFooter = _T("");
	mHeader = _T("");
	mLeftForceMagUnits = _T("");
	mRightForceMagUnits = _T("");
	mLocationLabel = _T("");
	m_JointLoads = _T("");
	m_JointNames = _T("");
	mForceUnits = _T("");
	mTorqueUnits = _T("");
	mHandForceUnits = _T("");
	mUnitsLabel = _T("");
	mLeftHori = _T("");
	//}}AFX_DATA_INIT
	
	cPointer = this;
	return;
}

ReportTaskInputSummary::~ReportTaskInputSummary()
{
	cPointer = NULL;
	return;
}

//---------------------------------------------------------------------------
CString
ReportTaskInputSummary::
ReportName
( void ) 
const
{
	return   "3DSSPP - Task Input Summary Report";
}

void ReportTaskInputSummary::DoDataExchange(CDataExchange* pDX)
{
	ReportView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ReportTaskInputSummary)
	DDX_Text(pDX, IDC_LeftForeArmHori, mLeftForeArmHori);
	DDX_Text(pDX, IDC_LeftForeArmVert, mLeftForeArmVert);
	DDX_Text(pDX, IDC_LeftLowerLegVert, mLeftLowerLegVert);
	DDX_Text(pDX, IDC_LeftUpperArmHori, mLeftUpperArmHori);
	DDX_Text(pDX, IDC_LeftUpperArmVert, mLeftUpperArmVert);
	DDX_Text(pDX, IDC_LeftUpperLegVert, mLeftUpperLegVert);
	DDX_Text(pDX, IDC_RightForeArmHori, mRightForeArmHori);
	DDX_Text(pDX, IDC_RightForeArmVert, mRightForeArmVert);
	DDX_Text(pDX, IDC_RightLowerLegVert, mRightLowerLegVert);
	DDX_Text(pDX, IDC_RightUpperArmHori, mRightUpperArmHori);
	DDX_Text(pDX, IDC_RightUpperArmVert, mRightUpperArmVert);
	DDX_Text(pDX, IDC_RightUpperLegVert, mRightUpperLegVert);

	DDX_Text(pDX, IDC_CLAVICLE_LH_DYN, mLeftClavicleHori);
	DDX_Text(pDX, IDC_CLAVICLE_LV_DYN, mLeftClavicleVert);
	DDX_Text(pDX, IDC_CLAVICLE_RH_DYN, mRightClavicleHori);
	DDX_Text(pDX, IDC_CLAVICLE_RV_DYN, mRightClavicleVert);

	DDX_Text(pDX, IDC_FOOT_LH_DYN, mLeftFootHori);
	DDX_Text(pDX, IDC_FOOT_LV_DYN, mLeftFootVert);
	DDX_Text(pDX, IDC_FOOT_RH_DYN, mRightFootHori);
	DDX_Text(pDX, IDC_FOOT_RV_DYN, mRightFootVert);


	DDX_Text(pDX, IDC_HandOrientation, mHandOrientation);
	DDX_Text(pDX, IDC_TrunkBending, mTrunkBending);
	DDX_Text(pDX, IDC_TrunkFlexion, mTrunkFlexion);
	DDX_Text(pDX, IDC_TrunkRotation, mTrunkRotation);

	DDX_Text(pDX, IDC_HEAD_LATERAL_DYN, mHeadBending );
	DDX_Text(pDX, IDC_HEAD_FLEXION_DYN,mHeadFlexion);
	DDX_Text(pDX, IDC_HEAD_ROT_DYN,mHeadRotation);
	DDX_Text(pDX, IDC_pelvic_lateral_tilt,mPelvicTilt);

	DDX_Text(pDX, IDC_LeftHandForceHori, mLeftHandForceHori);
	DDX_Text(pDX, IDC_LeftHandForceVert, mLeftHandForceVert);
	DDX_Text(pDX, IDC_LeftHandLocLat, mLeftHandLocLat);
	DDX_Text(pDX, IDC_LeftHandLocVert, mLeftHandLocVert);
	DDX_Text(pDX, IDC_LeftHandLocHori, mLeftHandLocHori);
	DDX_Text(pDX, IDC_RightHandForceHori, mRightHandForceHori);
	DDX_Text(pDX, IDC_RightHandForceVert, mRightHandForceVert);
	DDX_Text(pDX, IDC_RightHandLocHori, mRightHandLocHori);
	DDX_Text(pDX, IDC_RightHandLocLat, mRightHandLocLat);
	DDX_Text(pDX, IDC_RightHandLocVert, mRightHandLocVert);
	DDX_Text(pDX, IDC_RightHandForceMag, mRightHandForceMag);
	DDX_Text(pDX, IDC_LeftHandForceMag, mLeftHandForceMag);
	DDX_Text(pDX, IDC_Footer, mFooter);
	DDX_Text(pDX, IDC_Header, mHeader);
	DDX_Text(pDX, IDC_LeftForceMagUnits, mLeftForceMagUnits);
	DDX_Text(pDX, IDC_RightForceMagUnits, mRightForceMagUnits);
	DDX_Text(pDX, IDC_LocationLabel, mLocationLabel);
	DDX_Text(pDX, IDD_ReportTaskInputSummaryJointLoads, m_JointLoads);
	DDX_Text(pDX, IDC_JTNAMES, m_JointNames);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_LeftWristHori, mLeftWristHori);
	DDX_Text(pDX, IDC_LeftWristVert, mLeftWristVert);
	DDX_Text(pDX, IDC_RightWristHori, mRightWristHori);
	DDX_Text(pDX, IDC_RightWristVert, mRightWristVert);
	DDX_Text(pDX, IDC_LeftWristRot, mLeftWristRot);
	DDX_Text(pDX, IDC_RightWristRot, mRightWristRot);
	DDX_Text(pDX, IDC_LeftUpperLegHori, mLeftUpperlegHori);
	DDX_Text(pDX, IDC_LeftLowerLegHori, mLeftLowerlegHori);
	DDX_Text(pDX, IDC_RightUpperLegHori, mRightUpperlegHori);
	DDX_Text(pDX, IDC_RightLowerLegHori, mRightLowerlegHori);
	DDX_Text(pDX, IDC_PELVICAXIALROT, mPelvicAxial);
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


BEGIN_MESSAGE_MAP(ReportTaskInputSummary, ReportView)
//{{AFX_MSG_MAP(ReportTaskInputSummary)
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
//ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ReportTaskInputSummary diagnostics

#ifdef _DEBUG
void ReportTaskInputSummary::AssertValid() const
{
	CFormView::AssertValid();
}

void ReportTaskInputSummary::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ReportTaskInputSummary overrides

/*void ReportTaskInputSummary::OnFilePrintPreview()
{
	SSPPPrintPreview(this);
}*/

void ReportTaskInputSummary::OnUpdate( CView* aSenderPtr,
                                       LPARAM aHint,
													CObject* aHintPtr )
{
	C_Hom_Doc* lDocPtr = static_cast <C_Hom_Doc*> (GetDocument());
	Skeleton &lSkeleton = *lDocPtr->GetSkeleton();
	JointAngles docJointAngles;
    //lSkeleton.GetAngles(lSkeleton.getCurrentFrame(),docJointAngles);
	docJointAngles = lSkeleton.GetAngles();

    Length   lLocX( 0, 1 );
    Length   lLocY( 0, 1 );
    Length   lLocZ( 0, 1 );

	mLeftFootHori = IntToCStr( docJointAngles[A_FootLH] );
	mLeftFootVert = IntToCStr( docJointAngles[A_FootLV] );
	
	mRightFootHori = IntToCStr( docJointAngles[A_FootRH] );
	mRightFootVert = IntToCStr( docJointAngles[A_FootRV] );

	mLeftWristHori = IntToCStr( docJointAngles[A_HandLH] );
	mLeftWristVert = IntToCStr( docJointAngles[A_HandLV] );
	mLeftWristRot  = IntToCStr( docJointAngles[A_HandLRot] );

	mRightWristHori = IntToCStr( docJointAngles[A_HandRH] );
	mRightWristVert = IntToCStr( docJointAngles[A_HandRV] );
	mRightWristRot  = IntToCStr( docJointAngles[A_HandRRot] );
	
	mLeftForeArmHori = IntToCStr( docJointAngles[A_ForearmLH] );
	mLeftForeArmVert = IntToCStr( docJointAngles[A_ForearmLV] );
	mLeftUpperArmHori = IntToCStr( docJointAngles[A_UpperarmLH] );
	mLeftUpperArmVert = IntToCStr( docJointAngles[A_UpperarmLV] );
	mLeftUpperlegHori = IntToCStr( docJointAngles[A_UpperlegLH] );
	mLeftUpperLegVert = IntToCStr( docJointAngles[A_UpperlegLV] );
	mLeftLowerlegHori = IntToCStr( docJointAngles[A_LowerlegLH] );
	mLeftLowerLegVert = IntToCStr( docJointAngles[A_LowerlegLV] );
	
	mRightForeArmHori = IntToCStr( docJointAngles[A_ForearmRH] );
	mRightForeArmVert = IntToCStr( docJointAngles[A_ForearmRV] );
	mRightUpperArmHori = IntToCStr( docJointAngles[A_UpperarmRH] );
	mRightUpperArmVert = IntToCStr( docJointAngles[A_UpperarmRV] );
	mRightUpperlegHori = IntToCStr( docJointAngles[A_UpperlegRH] );
	mRightUpperLegVert = IntToCStr( docJointAngles[A_UpperlegRV] );
	mRightLowerlegHori = IntToCStr( docJointAngles[A_LowerlegRH] );
	mRightLowerLegVert = IntToCStr( docJointAngles[A_LowerlegRV] );

	mLeftClavicleHori = IntToCStr( docJointAngles[A_ClavicleLH] );
	mLeftClavicleVert = IntToCStr( docJointAngles[A_ClavicleLV] );
	mRightClavicleHori = IntToCStr( docJointAngles[A_ClavicleRH] );
	mRightClavicleVert = IntToCStr( docJointAngles[A_ClavicleRV] );
	
	mTrunkBending = IntToCStr( docJointAngles[A_TrunkLateralBending] );
	mTrunkFlexion = IntToCStr( docJointAngles[A_TrunkFlexion] );
	mTrunkRotation = IntToCStr( docJointAngles[A_TrunkAxialRotation] );

	mPelvicTilt = IntToCStr( docJointAngles[A_PelvisLateralBending] );
	mPelvicAxial = IntToCStr( docJointAngles[A_PelvisAxialRotation] );

	mHeadFlexion = IntToCStr( docJointAngles[A_C4C7Flexion] ); // local angle
	C_Hom_Doc* mDocPtr = (C_Hom_Doc*)GetDocument();
	double angles[NUM_EXTRA_ANGLES];
	mDocPtr->GetSkeleton()->getExtraAngles(angles);
	mHeadFlexion = IntToCStr(mDocPtr->GetSkeleton()->GetAngles()[A_TrunkFlexion] - (270 - (angles[EA_C4C7Flexion]+16 + angles[EA_C1C3Flexion] + angles[EA_C1_Flexion]))); // global angle
	//mHeadRotation = IntToCStr( docJointAngles[A_C4C7AxialRotation] ); // local
	mHeadRotation = IntToCStr(angles[EA_C1C3AxialRotation] + angles[EA_C4C7AxialRotation]); // gloabl
	//mHeadBending = IntToCStr( docJointAngles[A_C4C7LateralBending] ); // local
	mHeadBending = IntToCStr(angles[EA_C1C3AxialRotation] + angles[EA_C4C7AxialRotation]); // global


	
	int   lPrecision = 3;
	int   lRound     = 0;
	
	mLocationLabel = CString("") + "Location(" + lLocX.UnitsText() + "):";
	//bool standing = !(lDocPtr->getPositioning() & 1);
	bool standing = lDocPtr->getPosition() == P_Standing;

    // hand locations
    /*lSkeleton.ReportPoint(JT_LHAND, lLocX, lLocY, lLocZ, standing);
	mLeftHandLocHori = lLocY.ValueText();
	mLeftHandLocLat  = lLocX.ValueText();
	mLeftHandLocVert = lLocZ.ValueText();*/
	Vector3 lhand = lSkeleton.getJoint(JT_LHAND) - lDocPtr->getOffset(&lSkeleton);
	mLeftHandLocHori.Format("%.1f", lhand[1]);
	mLeftHandLocLat.Format("%.1f", lhand[0]);
	mLeftHandLocVert.Format("%.1f", lhand[2]);

	/*lSkeleton.ReportPoint(JT_RHAND, lLocX, lLocY, lLocZ, standing);   
	mRightHandLocHori = lLocY.ValueText();
	mRightHandLocLat  = lLocX.ValueText();
	mRightHandLocVert = lLocZ.ValueText();*/
	Vector3 rhand = lSkeleton.getJoint(JT_RHAND) - lDocPtr->getOffset(&lSkeleton);
	mRightHandLocHori.Format("%.1f", rhand[1]);
	mRightHandLocLat.Format("%.1f", rhand[0]);
	mRightHandLocVert.Format("%.1f", rhand[2]);

    Force tempForce(0,1);
    mForceUnits =  CString("") + "(" + tempForce.UnitsText() + ")";
    mLeftForceMagUnits = CString("") + "Mag" + mForceUnits;
    mRightForceMagUnits = CString("") + "Mag" + mForceUnits;

	// Sayan 07/16/07
	//
	// We do the unit conversions a lot more cleanly than before, because
	// unit conversions are now handled elsewhere in the code in one clean 
	// routine.

    // hand force magnitudes
    // tempForce.Value(lSkeleton.getExtForce(JT_LHAND).length(),MetricCns);
	// mLeftHandForceMag  = tempForce.ValueText();
    // tempForce.Value(lSkeleton.getExtForce(JT_RHAND).length(),MetricCns);
	// mRightHandForceMag  = tempForce.ValueText();

	mLeftHandForceMag.Format("%.1f",lSkeleton.getExtForce(JT_LHAND).length());

	// Sayan 07/16/07
	//
	// Should the line commented out below be there? Not completely sure.
	// hingeToMember[i][0].Format("%.0f", strength.Moment[i]);

    mRightHandForceMag.Format("%.1f",lSkeleton.getExtForce(JT_RHAND).length());

    // hand force angles
    double rv,rh,lv,lh;
	Vector3 LHandForce = lSkeleton.getExtForce(JT_LHAND);
	Vector3 RHandForce = lSkeleton.getExtForce(JT_RHAND);
	// convert force vectors to spherical angles
	if(LHandForce == Vector3(0,0,0)) lv = -90;
	else lv = 90.0 - degreesBetween(LHandForce,Vector3(0,0,1));
	if(RHandForce == Vector3(0,0,0)) rv = -90;
	else rv = 90.0 - degreesBetween(RHandForce,Vector3(0,0,1));

	RHandForce[2] = 0.0;
	if(RHandForce == Vector3(0,0,0)) rh = 90.0;
	else rh = degreesBetween(RHandForce,Vector3(1,0,0));
	if(RHandForce[1] < 0) rh *= -1;

	LHandForce[2] = 0.0;
	if(LHandForce == Vector3(0,0,0)) lh = 90.0;
	else lh = degreesBetween(LHandForce,Vector3(1,0,0));
	if(LHandForce[1] < 0) lh *= -1;

	mLeftHandForceHori = IntToCStr(int(lh));
	mLeftHandForceVert = IntToCStr(int(lv));
	mRightHandForceHori = IntToCStr(int(rh));
	mRightHandForceVert = IntToCStr(int(rv)); 

	mHandOrientation = lDocPtr->getTask().HandOrientationText();
	   
	mHeader = lDocPtr->ReportHeader();
	mFooter = lDocPtr->ReportFooter();
	
	UpdateData( FALSE );
	UpdateDynamicSummary();
	UpdateUnits();
	UpdateData( FALSE );
	this->RedrawWindow(NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);

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

/////////////////////////////////////////////////////////////////////////////
// ReportTaskInputSummary message handlers

HBRUSH ReportTaskInputSummary::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	pDC->SetBkColor(lDocPtr->getBalColor());
	
	return (HBRUSH)(CBrush(lDocPtr->getBalColor()));
}

void ReportTaskInputSummary::OnDisplayDialogcolor() 
{
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
	lDocPtr->setReportColor();
	
	Invalidate(true);
}

#define TextHeight 13
void ReportTaskInputSummary::UpdateDynamicSummary(void)
{
	int NumForces;
	CFrameWnd* pFrame = GetParentFrame();
	
	CWnd			*mainWnd = (CWnd*)this;

	if (2 >= (NumForces = CountExternalForces() + 2)) {
		GetDlgItem(IDD_OutlineJointLoads)->ShowWindow(SW_HIDE);
		GetDlgItem(IDD_ReportTaskInputSummaryJointLoads)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_JTNAMES)->ShowWindow(SW_HIDE);
		
		WINDOWPLACEMENT lpwndpl;

		mainWnd->GetWindowPlacement(&lpwndpl);
		//lpwndpl.rcNormalPosition.bottom += lpwndpl.rcNormalPosition.bottom - hack_WindowBottom;
		//mainWnd->SetWindowPlacement(&lpwndpl);
		MoveWindow( 0,0,lpwndpl.rcNormalPosition.right, hack_WindowBottom, TRUE );
//		ResizeParentToFit(FALSE);
		//pFrame->SetWindowPos(NULL, 0, 0, lpwndpl.rcNormalPosition.right + 20, hack_WindowBottom,
		pFrame->SetWindowPos(NULL, 0, 0, lpwndpl.rcNormalPosition.right + 16, hack_WindowBottom,
		SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);

		m_JointLoads.Empty();
		m_JointNames.Empty();
		CWnd			*Footer;
		WINDOWPLACEMENT		FooterLoc;
		Footer = GetDlgItem(IDC_Footer);
		Footer->GetWindowPlacement( &FooterLoc );
		FooterLoc.rcNormalPosition.top		= hack_FooterTop + 13;
		FooterLoc.rcNormalPosition.bottom	= hack_FooterTop + 28+15;
		Footer->SetWindowPlacement(&FooterLoc);
				
	}
	else {
		// /*
		CWnd			*Footer, *Joints, *Names, *Outline;
		WINDOWPLACEMENT		FooterLoc, JointsLoc, OutlineLoc, NamesLoc;
		
		// Show the box for the other joints
		GetDlgItem(IDD_OutlineJointLoads)->ShowWindow(SW_SHOW);
		GetDlgItem(IDD_ReportTaskInputSummaryJointLoads)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_JTNAMES)->ShowWindow(SW_SHOW);
		
		// Move the footer down 50 units
		Footer = GetDlgItem(IDC_Footer);
		Footer->GetWindowPlacement( &FooterLoc );
		
		Outline = GetDlgItem(IDD_OutlineJointLoads);
		Outline->GetWindowPlacement( &OutlineLoc );
		
		Joints = GetDlgItem(IDD_ReportTaskInputSummaryJointLoads);
		Joints->GetWindowPlacement( &JointsLoc );
		
		Names = GetDlgItem(IDC_JTNAMES);
		Names->GetWindowPlacement( &NamesLoc );
		
		OutlineLoc.rcNormalPosition.top		= hack_FooterTop + 5;
		OutlineLoc.rcNormalPosition.left	= FooterLoc.rcNormalPosition.left;
		OutlineLoc.rcNormalPosition.right	= FooterLoc.rcNormalPosition.right;
		
		NamesLoc.rcNormalPosition.top		= OutlineLoc.rcNormalPosition.top + 15;
		NamesLoc.rcNormalPosition.left		= OutlineLoc.rcNormalPosition.left + 10;
		NamesLoc.rcNormalPosition.right		= OutlineLoc.rcNormalPosition.left + 105;
		NamesLoc.rcNormalPosition.bottom	= NamesLoc.rcNormalPosition.top + (NumForces * TextHeight) + 15;
		
		JointsLoc.rcNormalPosition.top		= OutlineLoc.rcNormalPosition.top + 15;
		JointsLoc.rcNormalPosition.left		= NamesLoc.rcNormalPosition.right + 5;
		JointsLoc.rcNormalPosition.right	= OutlineLoc.rcNormalPosition.right - 10;
		JointsLoc.rcNormalPosition.bottom	= JointsLoc.rcNormalPosition.top + (NumForces * TextHeight) + 15;
		
		OutlineLoc.rcNormalPosition.bottom	= JointsLoc.rcNormalPosition.bottom + 2.5;
		
		
		Joints->SetWindowPlacement(&JointsLoc);
		Outline->SetWindowPlacement(&OutlineLoc);
		Names->SetWindowPlacement(&NamesLoc);
		
		FooterLoc.rcNormalPosition.top		= OutlineLoc.rcNormalPosition.bottom + 13;
		FooterLoc.rcNormalPosition.bottom	= OutlineLoc.rcNormalPosition.bottom + 28+15;
		Footer->SetWindowPlacement(&FooterLoc);
		
		WINDOWPLACEMENT lpwndpl;
		mainWnd->GetWindowPlacement(&lpwndpl);
//		lpwndpl.rcNormalPosition.bottom = hack_WindowBottom - NumForces * TextHeight - 25;
//		mainWnd->SetWindowPlacement(&lpwndpl);
		MoveWindow( 0,0,lpwndpl.rcNormalPosition.right, hack_WindowBottom + NumForces * TextHeight + 25, TRUE );
//		ResizeParentToFit(FALSE);
		pFrame->SetWindowPos(NULL, 0, 0, lpwndpl.rcNormalPosition.right + 16, hack_WindowBottom + NumForces * TextHeight + 25,
		SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
//		this->OnSize(0, lpwndpl.rcNormalPosition.right, hack_WindowBottom + NumForces * TextHeight + 25);

		DisplayExtForces(&m_JointLoads);
		DisplayExtForceNames(&m_JointNames);
		
	}
}

void ReportTaskInputSummary::OnInitialUpdate() 
{	
	int i=0;

	CWnd *mainWnd = (CWnd*)this;

	{
		WINDOWPLACEMENT lpwndpl;
		mainWnd->GetWindowPlacement(&lpwndpl);

		//hack_WindowBottom = lpwndpl.rcNormalPosition.;
		//Yijun: resize the windows
		hack_WindowBottom = 550;
	}
	{
		CWnd *tempFrame = GetDlgItem(IDC_HANDSFRAME);
		WINDOWPLACEMENT tempCoords;
		tempFrame->GetWindowPlacement(&tempCoords);
		hack_FooterTop = tempCoords.rcNormalPosition.bottom;
	}
	
	this->ReportView::OnInitialUpdate();	

	// Set the hand force box units
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();
	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";
	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}

void ReportTaskInputSummary::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
}

void ReportTaskInputSummary::UpdateUnits() {
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";

	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";

	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}
