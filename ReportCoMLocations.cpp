// Report


//ReportCoMLocations.cpp : implementation file
//

#include "ReportCoMLocations.h"

#include "hom.h"
#include "hom_doc.h"	    // Necessary
#include "skeleton.hpp"
#include "Task.h"
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
// ReportCoMLocations

ReportView * ReportCoMLocations::cPointer = NULL;

IMPLEMENT_DYNCREATE(ReportCoMLocations, ReportView)

//---------------------------------------------------------------------------
   ReportCoMLocations::
ReportCoMLocations( void )
:  ReportView( ReportCoMLocations::IDD )
{
	//{{AFX_DATA_INIT(ReportCoMLocations)
	mFooter = _T("");
	mHeader = _T("");
	mLocationUnits = _T("");

	mHandForceUnits = _T("");
	mUnitsLabel = _T("");
	mLeftHori = _T("");

	//}}AFX_DATA_INIT
   cPointer = this;
}

//---------------------------------------------------------------------------
   ReportCoMLocations::
~ReportCoMLocations( void )
{
   cPointer = NULL;
   return;
}

//---------------------------------------------------------------------------
   CString
   ReportCoMLocations::
ReportName
( void ) 
const
{
   return   "3DSSPP - Center of Mass Locations";
}

//---------------------------------------------------------------------------
void ReportCoMLocations::DoDataExchange(CDataExchange* pDX)
{
	ReportView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ReportCoMLocations)
	DDX_Text(pDX, IDC_Footer, mFooter);
	DDX_Text(pDX, IDC_Header, mHeader);
	DDX_Text(pDX, IDC_LocationUnits, mLocationUnits);

	DDX_Text(pDX, IDC_LeftHandCoMX, mLeftHandCoM[0]);
	DDX_Text(pDX, IDC_LeftHandCoMY, mLeftHandCoM[1]);
	DDX_Text(pDX, IDC_LeftHandCoMZ, mLeftHandCoM[2]);

	DDX_Text(pDX, IDC_RightHandCoMX, mRightHandCoM[0]);
	DDX_Text(pDX, IDC_RightHandCoMY, mRightHandCoM[1]);
	DDX_Text(pDX, IDC_RightHandCoMZ, mRightHandCoM[2]);

	DDX_Text(pDX, IDC_LeftLowerArmCoMX, mLeftLowerArmCoM[0]);
	DDX_Text(pDX, IDC_LeftLowerArmCoMY, mLeftLowerArmCoM[1]);
	DDX_Text(pDX, IDC_LeftLowerArmCoMZ, mLeftLowerArmCoM[2]);

	DDX_Text(pDX, IDC_RightLowerArmCoMX, mRightLowerArmCoM[0]);
	DDX_Text(pDX, IDC_RightLowerArmCoMY, mRightLowerArmCoM[1]);
	DDX_Text(pDX, IDC_RightLowerArmCoMZ, mRightLowerArmCoM[2]);

	DDX_Text(pDX, IDC_LeftUpperArmCoMX, mLeftUpperArmCoM[0]);
	DDX_Text(pDX, IDC_LeftUpperArmCoMY, mLeftUpperArmCoM[1]);
	DDX_Text(pDX, IDC_LeftUpperArmCoMZ, mLeftUpperArmCoM[2]);

	DDX_Text(pDX, IDC_RightUpperArmCoMX, mRightUpperArmCoM[0]);
	DDX_Text(pDX, IDC_RightUpperArmCoMY, mRightUpperArmCoM[1]);
	DDX_Text(pDX, IDC_RightUpperArmCoMZ, mRightUpperArmCoM[2]);

	DDX_Text(pDX, IDC_HeadCoMX, mHeadCoM[0]);
	DDX_Text(pDX, IDC_HeadCoMY, mHeadCoM[1]);
	DDX_Text(pDX, IDC_HeadCoMZ, mHeadCoM[2]);

	DDX_Text(pDX, IDC_UpperNeckCoMX, mUpperNeckCoM[0]);
	DDX_Text(pDX, IDC_UpperNeckCoMY, mUpperNeckCoM[1]);
	DDX_Text(pDX, IDC_UpperNeckCoMZ, mUpperNeckCoM[2]);

	DDX_Text(pDX, IDC_LowerNeckCoMX, mLowerNeckCoM[0]);
	DDX_Text(pDX, IDC_LowerNeckCoMY, mLowerNeckCoM[1]);
	DDX_Text(pDX, IDC_LowerNeckCoMZ, mLowerNeckCoM[2]);

	DDX_Text(pDX, IDC_TorsoCoMX, mTorsoCoM[0]);
	DDX_Text(pDX, IDC_TorsoCoMY, mTorsoCoM[1]);
	DDX_Text(pDX, IDC_TorsoCoMZ, mTorsoCoM[2]);

	DDX_Text(pDX, IDC_LeftPelvisCoMX, mLeftPelvisCoM[0]);
	DDX_Text(pDX, IDC_LeftPelvisCoMY, mLeftPelvisCoM[1]);
	DDX_Text(pDX, IDC_LeftPelvisCoMZ, mLeftPelvisCoM[2]);

	/*DDX_Text(pDX, IDC_RightPelvisCoMX, mRightPelvisCoM[0]);
	DDX_Text(pDX, IDC_RightPelvisCoMY, mRightPelvisCoM[1]);
	DDX_Text(pDX, IDC_RightPelvisCoMZ, mRightPelvisCoM[2]);*/

	DDX_Text(pDX, IDC_LeftUpperLegCoMX, mLeftUpperLegCoM[0]);
	DDX_Text(pDX, IDC_LeftUpperLegCoMY, mLeftUpperLegCoM[1]);
	DDX_Text(pDX, IDC_LeftUpperLegCoMZ, mLeftUpperLegCoM[2]);

	DDX_Text(pDX, IDC_RightUpperLegCoMX, mRightUpperLegCoM[0]);
	DDX_Text(pDX, IDC_RightUpperLegCoMY, mRightUpperLegCoM[1]);
	DDX_Text(pDX, IDC_RightUpperLegCoMZ, mRightUpperLegCoM[2]);

	DDX_Text(pDX, IDC_LeftLowerLegCoMX, mLeftLowerLegCoM[0]);
	DDX_Text(pDX, IDC_LeftLowerLegCoMY, mLeftLowerLegCoM[1]);
	DDX_Text(pDX, IDC_LeftLowerLegCoMZ, mLeftLowerLegCoM[2]);

	DDX_Text(pDX, IDC_RightLowerLegCoMX, mRightLowerLegCoM[0]);
	DDX_Text(pDX, IDC_RightLowerLegCoMY, mRightLowerLegCoM[1]);
	DDX_Text(pDX, IDC_RightLowerLegCoMZ, mRightLowerLegCoM[2]);

	DDX_Text(pDX, IDC_LeftFootCoMX, mLeftFootCoM[0]);
	DDX_Text(pDX, IDC_LeftFootCoMY, mLeftFootCoM[1]);
	DDX_Text(pDX, IDC_LeftFootCoMZ, mLeftFootCoM[2]);

	DDX_Text(pDX, IDC_RightFootCoMX, mRightFootCoM[0]);
	DDX_Text(pDX, IDC_RightFootCoMY, mRightFootCoM[1]);
	DDX_Text(pDX, IDC_RightFootCoMZ, mRightFootCoM[2]);

	DDX_Text(pDX, IDC_BodyCoMX, mBodyCoM[0]);
	DDX_Text(pDX, IDC_BodyCoMY, mBodyCoM[1]);
	DDX_Text(pDX, IDC_BodyCoMZ, mBodyCoM[2]);

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
BEGIN_MESSAGE_MAP(ReportCoMLocations, ReportView)
	//{{AFX_MSG_MAP(ReportCoMLocations)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
	//}}AFX_MSG_MAP
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
    //ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ReportCoMLocations diagnostics

#ifdef _DEBUG
void ReportCoMLocations::AssertValid() const
{
	CFormView::AssertValid();
}

void ReportCoMLocations::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ReportCoMLocations overrides

//void ReportCoMLocations::OnFilePrintPreview()
//{
//	SSPPPrintPreview(this);
//}

//---------------------------------------------------------------------------
void ReportCoMLocations::OnUpdate(CView *   aSenderPtr,
                                    LPARAM    aHint, 
                                    CObject * aHintPtr
                                   )
{
    C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();

    mHeader = lDocPtr->ReportHeader();
    
	bool standing = lDocPtr->getPosition() == P_Standing;

    //--- CoM Locations ---
    Skeleton &lSkeleton = *lDocPtr->GetSkeleton();
	Vector3* locations = lSkeleton.joints;
	Task& task = lDocPtr->getTask();
	LinkCGs& CGs = task.CGs;
	LinkWeights weights = task.weights;

	double metrics = 1;

	if(task.getMetricOrEnglish() == EnglishCns) metrics = CM_TO_IN;

	mLocationUnits = CString("Locations ") + "(" + lDocPtr->LengthUnits() + ")";

	Vector3 tempCoM;
	Vector3 offsetCoordinate = lDocPtr->getOffset(&lSkeleton, true);
//	offsetCoordinate += origin;
	Vector3 CoMs[NUM_COM];

	//Head
	tempCoM = locations[JT_TRAGIONLOC] - locations[JT_C1];
	tempCoM.normalize();
	tempCoM *= CGs.Head;
	CoMs[COM_HEAD] = (tempCoM + locations[JT_C1])  - offsetCoordinate;

	//Upper Neck
	tempCoM =  locations[JT_C1] - locations[JT_C3C4];
	tempCoM.normalize();
	tempCoM *= CGs.UpperNeck;
	CoMs[COM_UPPER_NECK] = (tempCoM + locations[JT_C3C4]) - offsetCoordinate ;

	//Lower Neck
	tempCoM = locations[JT_C3C4] - locations[JT_C7T1];
	tempCoM.normalize();
	tempCoM *= CGs.LowerNeck;
	CoMs[COM_LOWER_NECK] = tempCoM + locations[JT_C7T1]  - offsetCoordinate;

	//Hands
	tempCoM = locations[JT_LHAND] - locations[JT_LWRIST];
	tempCoM.normalize();
	tempCoM *= CGs.Hand;
	CoMs[COM_HANDL] = tempCoM + locations[JT_LWRIST] - offsetCoordinate;
	tempCoM = locations[JT_RHAND] - locations[JT_RWRIST];
	tempCoM.normalize();
	tempCoM *= CGs.HandR;
	CoMs[COM_HANDR] = tempCoM + locations[JT_RWRIST] - offsetCoordinate;

	//Forearm
	tempCoM = locations[JT_LWRIST] - locations[JT_LELBOW];
	tempCoM.normalize();
	tempCoM *= CGs.Forearm;
	CoMs[COM_FOREARML] = tempCoM + locations[JT_LELBOW] - offsetCoordinate;
	tempCoM = locations[JT_RWRIST] - locations[JT_RELBOW];
	tempCoM.normalize();
	tempCoM *= CGs.ForearmR;
	CoMs[COM_FOREARMR] = tempCoM + locations[JT_RELBOW] - offsetCoordinate;

	//Upper Arm
	tempCoM = locations[JT_LELBOW] - locations[JT_LSHOULDER];
	tempCoM.normalize();
	tempCoM *= CGs.UpperArm;
	CoMs[COM_UPPER_ARML] = tempCoM + locations[JT_LSHOULDER] - offsetCoordinate;
	tempCoM = locations[JT_RELBOW] - locations[JT_RSHOULDER];
	tempCoM.normalize();
	tempCoM *= CGs.UpperArmR;
	CoMs[COM_UPPER_ARMR] = tempCoM + locations[JT_RSHOULDER] - offsetCoordinate;

	//Torsor
	tempCoM = locations[JT_C7T1] - locations[JT_L5S1_TEMP];
	tempCoM.normalize();
	tempCoM *= CGs.L5S1ToShoulder;
	CoMs[COM_TORSO] = tempCoM + locations[JT_L5S1_TEMP] - offsetCoordinate;

	//Pelvis
	tempCoM = locations[JT_L5S1_TEMP] - locations[JT_HIP_CENTER];
	tempCoM.normalize();
	tempCoM *= CGs.Pelvis;
	CoMs[COM_PELVIS] = tempCoM + locations[JT_HIP_CENTER] - offsetCoordinate;

	//Upper Leg
	tempCoM = locations[JT_LKNEE]-locations[JT_LHIP];
	tempCoM.normalize();
	tempCoM *= CGs.UpperLeg;
	CoMs[COM_UPPER_LEGL] = tempCoM + locations[JT_LHIP] - offsetCoordinate;
	tempCoM = locations[JT_RKNEE]-locations[JT_RHIP];
	tempCoM.normalize();
	tempCoM *= CGs.UpperLegR;
	CoMs[COM_UPPER_LEGR] = tempCoM + locations[JT_RHIP] - offsetCoordinate;

	//Lower Leg
	tempCoM = locations[JT_LANKLE]-locations[JT_LKNEE];
	tempCoM.normalize();
	tempCoM *= CGs.LowerLeg;
	CoMs[COM_LOWER_LEGL] = tempCoM + locations[JT_LKNEE] - offsetCoordinate;
	tempCoM = locations[JT_RANKLE]-locations[JT_RKNEE];
	tempCoM.normalize();
	tempCoM *= CGs.LowerLegR;
	CoMs[COM_LOWER_LEGR] = tempCoM + locations[JT_RKNEE] - offsetCoordinate;

	//Foot
	tempCoM = locations[JT_LANKLE] - locations[JT_LBALL];
	tempCoM.normalize();
	tempCoM *= CGs.Foot;
	CoMs[COM_FOOTL] = tempCoM+locations[JT_LBALL] - offsetCoordinate;
	tempCoM = locations[JT_RANKLE]-locations[JT_RBALL];
	tempCoM.normalize();
	tempCoM *= CGs.FootR;
	CoMs[COM_FOOTR] = tempCoM+locations[JT_RBALL] - offsetCoordinate;

	CoMs[COM_BODY] = (CoMs[COM_HEAD] * weights.Head + CoMs[COM_UPPER_NECK] * weights.UpperNeck + CoMs[COM_LOWER_NECK] * weights.LowerNeck
		+ CoMs[COM_HANDL] * weights.Hand + CoMs[COM_HANDR] * weights.HandR
		+ CoMs[COM_FOREARML] * weights.Forearm + CoMs[COM_FOREARMR] * weights.ForearmR
		+ CoMs[COM_UPPER_ARML] * weights.UpperArm + CoMs[COM_FOREARMR] * weights.UpperArmR
		+ CoMs[COM_UPPER_LEGL] * weights.UpperLeg + CoMs[COM_UPPER_LEGR] * weights.UpperLegR
		+ CoMs[COM_LOWER_LEGL] * weights.LowerLeg + CoMs[COM_LOWER_LEGR] * weights.LowerLegR
		+ CoMs[COM_FOOTL] * weights.Foot + CoMs[COM_FOOTR] * weights.FootR
		+ CoMs[COM_TORSO] * weights.TorsoAndHead + CoMs[COM_PELVIS] * weights.Pelvis)/
		( weights.Foot + weights.FootR + weights.Forearm + weights.ForearmR + weights.Hand
		+ weights.HandR + weights.Head + weights.UpperNeck + weights.LowerNeck + weights.Hand + weights.HandR
		+ weights.LowerLeg + weights.LowerLegR + weights.UpperLeg + weights.UpperLegR + weights.Pelvis + weights.TorsoAndHead);
    
	for (int i = 0; i < 3; i++)
	{
		mLeftHandCoM[i].Format("%.1f", CoMs[COM_HANDL][i] * metrics );
		mRightHandCoM[i].Format("%.1f", CoMs[COM_HANDR][i] * metrics );
		mLeftLowerArmCoM[i].Format("%.1f", CoMs[COM_FOREARML][i] * metrics );
		mRightLowerArmCoM[i].Format("%.1f", CoMs[COM_FOREARMR][i] * metrics );
		mLeftUpperArmCoM[i].Format("%.1f", CoMs[COM_UPPER_ARML][i] * metrics );
		mRightUpperArmCoM[i].Format("%.1f", CoMs[COM_UPPER_ARMR][i] * metrics );
		mHeadCoM[i].Format("%.1f", CoMs[COM_HEAD][i] * metrics );
		mUpperNeckCoM[i].Format("%.1f", CoMs[COM_UPPER_NECK][i] * metrics );
		mLowerNeckCoM[i].Format("%.1f",CoMs[COM_LOWER_NECK][i] * metrics );
		mTorsoCoM[i].Format("%.1f", CoMs[COM_TORSO][i] * metrics );
		mLeftPelvisCoM[i].Format("%.1f", CoMs[COM_PELVIS][i] * metrics );
		//mRightPelvisCoM[i].Format("%.1f", CoMs[COM_HANDR][i]);
		mLeftUpperLegCoM[i].Format("%.1f", CoMs[COM_UPPER_LEGL][i] * metrics );
		mRightUpperLegCoM[i].Format("%.1f", CoMs[COM_UPPER_LEGR][i] * metrics );
		mLeftLowerLegCoM[i].Format("%.1f", CoMs[COM_LOWER_LEGL][i] * metrics );
		mRightLowerLegCoM[i].Format("%.1f", CoMs[COM_LOWER_LEGR][i] * metrics );
		mLeftFootCoM[i].Format("%.1f", CoMs[COM_FOOTL][i] * metrics );
		mRightFootCoM[i].Format("%.1f", CoMs[COM_FOOTR][i] * metrics );

		mBodyCoM[i].Format("%.1f", CoMs[COM_BODY][i] * metrics );
	}

	//--------------------------------------

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
// ReportCoMLocations message handlers


HBRUSH ReportCoMLocations::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
		HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
 
   pDC->SetBkMode(TRANSPARENT);
   pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
   pDC->SetBkColor(lDocPtr->getBalColor());

   return (HBRUSH)(CBrush(lDocPtr->getBalColor()));
}

void ReportCoMLocations::OnDisplayDialogcolor() 
{
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
	lDocPtr->setReportColor();
	
	Invalidate(true);
}

void ReportCoMLocations::OnInitialUpdate()
{
	ReportView::OnInitialUpdate();

	// Set the hand force box units
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();
	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";
	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}


void ReportCoMLocations::UpdateUnits() {
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";

	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";

	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}