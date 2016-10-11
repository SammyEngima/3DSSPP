#include "ReportStrengthDirVectors.h"

#include <cmath>

#include "hom_doc.h"
#include "Skeleton.hpp"

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

//#include "units.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ReportView *ReportStrengthDirVectors::cPointer = NULL;

IMPLEMENT_DYNCREATE(ReportStrengthDirVectors, ReportView)

ReportStrengthDirVectors::ReportStrengthDirVectors(void) : ReportView(ReportStrengthDirVectors::IDD)
{
	//{{AFX_DATA_INIT(ReportStrengthDirVectors)
	mFooter = _T("");
	mHeader = _T("");

	mHandForceUnits = _T("");
	mUnitsLabel = _T("");
	mLeftHori = _T("");

    for(int i = 0; i < 3; ++i)
    {
	    mElbowL[i] = _T("");
	    mElbowR[i] = _T("");
	    mHipFlexL[i] = _T("");
	    mHipFlexR[i] = _T("");
	    mKneeFlexL[i] = _T("");
	    mKneeFlexR[i] = _T("");
	    mL5S1BendL[i] = _T("");
	    mL5S1FlexL[i] = _T("");
	    mL5S1RotL[i] = _T("");
	    mShoulderAddL[i] = _T("");
	    mShoulderAddR[i] = _T("");
	    mShoulderHumL[i] = _T("");
	    mShoulderHumR[i] = _T("");
	    mShoulderRotL[i] = _T("");
	    mShoulderRotR[i] = _T("");
	    mAnkleFlexL[i] = _T("");
	    mAnkleFlexR[i] = _T("");
    }
	//}}AFX_DATA_INIT
	hingeToMember[SH_LWristDeviation] = mWristDevL;
	hingeToMember[SH_LWristFlexion] = mWristFlexL;
	hingeToMember[SH_LForearmRotation] = mWristRotL;
	hingeToMember[SH_LElbowFlexion] = mElbowL;
	hingeToMember[SH_LHumeralRotation] = mShoulderHumL;
	hingeToMember[SH_LShoulderAbduction] = mShoulderAddL;
	hingeToMember[SH_LShoulderRotation] = mShoulderRotL;
	hingeToMember[SH_LHipFlexion] = mHipFlexL;
	hingeToMember[SH_LKneeFlexion] = mKneeFlexL;
	hingeToMember[SH_LAnkleFlexion] = mAnkleFlexL;
	hingeToMember[SH_RWristDeviation] = mWristDevR;
	hingeToMember[SH_RWristFlexion] = mWristFlexR;
	hingeToMember[SH_RForearmRotation] = mWristRotR;
	hingeToMember[SH_RElbowFlexion] = mElbowR;
	hingeToMember[SH_RHumeralRotation] = mShoulderHumR;
	hingeToMember[SH_RShoulderAbduction] = mShoulderAddR;
	hingeToMember[SH_RShoulderRotation] = mShoulderRotR;
	hingeToMember[SH_RHipFlexion] = mHipFlexR;
	hingeToMember[SH_RKneeFlexion] = mKneeFlexR;
	hingeToMember[SH_RAnkleFlexion] = mAnkleFlexR;
	hingeToMember[SH_TorsoFlexion] = mL5S1FlexL;
	hingeToMember[SH_TorsoLateralBending] = mL5S1BendL;
	hingeToMember[SH_TorsoRotation] = mL5S1RotL;
	hingeToMember[SH_NeckFlexion] = mNeckFlexL;
	hingeToMember[SH_NeckLateralBending] = mNeckBendL;
	hingeToMember[SH_NeckRotation] = mNeckRotL;

   cPointer = this;
   return;
}

ReportStrengthDirVectors::~ReportStrengthDirVectors(void) { cPointer = NULL; }

CString ReportStrengthDirVectors::ReportName(void) const { return "3DSSPP - Strength Direction Vectors Report"; }

void ReportStrengthDirVectors::DoDataExchange(CDataExchange* pDX)
{
	ReportView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ReportStrengthDirVectors)
	DDX_Text(pDX, IDC_Footer, mFooter);
	DDX_Text(pDX, IDC_Header, mHeader);
	DDX_Text(pDX, IDC_ElbowLX, mElbowL[0]);
	DDX_Text(pDX, IDC_ElbowLY, mElbowL[1]);
	DDX_Text(pDX, IDC_ElbowLZ, mElbowL[2]);
	DDX_Text(pDX, IDC_ElbowRX, mElbowR[0]);
	DDX_Text(pDX, IDC_ElbowRY, mElbowR[1]);
	DDX_Text(pDX, IDC_ElbowRZ, mElbowR[2]);
	DDX_Text(pDX, IDC_WristFlexLX, mWristFlexL[0]);
	DDX_Text(pDX, IDC_WristFlexLY, mWristFlexL[1]);
	DDX_Text(pDX, IDC_WristFlexLZ, mWristFlexL[2]);
	DDX_Text(pDX, IDC_WristFlexRX, mWristFlexR[0]);
	DDX_Text(pDX, IDC_WristFlexRY, mWristFlexR[1]);
	DDX_Text(pDX, IDC_WristFlexRZ, mWristFlexR[2]);
	DDX_Text(pDX, IDC_WristDevLX, mWristDevL[0]);
	DDX_Text(pDX, IDC_WristDevLY, mWristDevL[1]);
	DDX_Text(pDX, IDC_WristDevLZ, mWristDevL[2]);
	DDX_Text(pDX, IDC_WristDevRX, mWristDevR[0]);
	DDX_Text(pDX, IDC_WristDevRY, mWristDevR[1]);
	DDX_Text(pDX, IDC_WristDevRZ, mWristDevR[2]);
	DDX_Text(pDX, IDC_WristRotLX, mWristRotL[0]);
	DDX_Text(pDX, IDC_WristRotLY, mWristRotL[1]);
	DDX_Text(pDX, IDC_WristRotLZ, mWristRotL[2]);
	DDX_Text(pDX, IDC_WristRotRX, mWristRotR[0]);
	DDX_Text(pDX, IDC_WristRotRY, mWristRotR[1]);
	DDX_Text(pDX, IDC_WristRotRZ, mWristRotR[2]);
	DDX_Text(pDX, IDC_HipFlexLX, mHipFlexL[0]);
	DDX_Text(pDX, IDC_HipFlexLY, mHipFlexL[1]);
	DDX_Text(pDX, IDC_HipFlexLZ, mHipFlexL[2]);
	DDX_Text(pDX, IDC_HipFlexRX, mHipFlexR[0]);
	DDX_Text(pDX, IDC_HipFlexRY, mHipFlexR[1]);
	DDX_Text(pDX, IDC_HipFlexRZ, mHipFlexR[2]);
	DDX_Text(pDX, IDC_KneeFlexLX, mKneeFlexL[0]);
	DDX_Text(pDX, IDC_KneeFlexLY, mKneeFlexL[1]);
	DDX_Text(pDX, IDC_KneeFlexLZ, mKneeFlexL[2]);
	DDX_Text(pDX, IDC_KneeFlexRX, mKneeFlexR[0]);
	DDX_Text(pDX, IDC_KneeFlexRY, mKneeFlexR[1]);
	DDX_Text(pDX, IDC_KneeFlexRZ, mKneeFlexR[2]);
	DDX_Text(pDX, IDC_NeckBendLX, mNeckBendL[0]);
	DDX_Text(pDX, IDC_NeckBendLY, mNeckBendL[1]);
	DDX_Text(pDX, IDC_NeckBendLZ, mNeckBendL[2]);
	DDX_Text(pDX, IDC_NeckFlexLX, mNeckFlexL[0]);
	DDX_Text(pDX, IDC_NeckFlexLY, mNeckFlexL[1]);
	DDX_Text(pDX, IDC_NeckFlexLZ, mNeckFlexL[2]);
	DDX_Text(pDX, IDC_NeckRotLX, mNeckRotL[0]);
	DDX_Text(pDX, IDC_NeckRotLY, mNeckRotL[1]);
	DDX_Text(pDX, IDC_NeckRotLZ, mNeckRotL[2]);
	DDX_Text(pDX, IDC_L5S1BendLX, mL5S1BendL[0]);
	DDX_Text(pDX, IDC_L5S1BendLY, mL5S1BendL[1]);
	DDX_Text(pDX, IDC_L5S1BendLZ, mL5S1BendL[2]);
	DDX_Text(pDX, IDC_L5S1FlexLX, mL5S1FlexL[0]);
	DDX_Text(pDX, IDC_L5S1FlexLY, mL5S1FlexL[1]);
	DDX_Text(pDX, IDC_L5S1FlexLZ, mL5S1FlexL[2]);
	DDX_Text(pDX, IDC_L5S1RotLX, mL5S1RotL[0]);
	DDX_Text(pDX, IDC_L5S1RotLY, mL5S1RotL[1]);
	DDX_Text(pDX, IDC_L5S1RotLZ, mL5S1RotL[2]);
	DDX_Text(pDX, IDC_ShoulderAddLX, mShoulderAddL[0]);
	DDX_Text(pDX, IDC_ShoulderAddLY, mShoulderAddL[1]);
	DDX_Text(pDX, IDC_ShoulderAddLZ, mShoulderAddL[2]);
	DDX_Text(pDX, IDC_ShoulderAddRX, mShoulderAddR[0]);
	DDX_Text(pDX, IDC_ShoulderAddRY, mShoulderAddR[1]);
	DDX_Text(pDX, IDC_ShoulderAddRZ, mShoulderAddR[2]);
	DDX_Text(pDX, IDC_ShoulderHumLX, mShoulderHumL[0]);
	DDX_Text(pDX, IDC_ShoulderHumLY, mShoulderHumL[1]);
	DDX_Text(pDX, IDC_ShoulderHumLZ, mShoulderHumL[2]);
	DDX_Text(pDX, IDC_ShoulderHumRX, mShoulderHumR[0]);
	DDX_Text(pDX, IDC_ShoulderHumRY, mShoulderHumR[1]);
	DDX_Text(pDX, IDC_ShoulderHumRZ, mShoulderHumR[2]);
	DDX_Text(pDX, IDC_ShoulderRotLX, mShoulderRotL[0]);
	DDX_Text(pDX, IDC_ShoulderRotLY, mShoulderRotL[1]);
	DDX_Text(pDX, IDC_ShoulderRotLZ, mShoulderRotL[2]);
	DDX_Text(pDX, IDC_ShoulderRotRX, mShoulderRotR[0]);
	DDX_Text(pDX, IDC_ShoulderRotRY, mShoulderRotR[1]);
	DDX_Text(pDX, IDC_ShoulderRotRZ, mShoulderRotR[2]);
	DDX_Text(pDX, IDC_AnkleLX, mAnkleFlexL[0]);
	DDX_Text(pDX, IDC_AnkleLY, mAnkleFlexL[1]);
	DDX_Text(pDX, IDC_AnkleLZ, mAnkleFlexL[2]);
	DDX_Text(pDX, IDC_AnkleRX, mAnkleFlexR[0]);
	DDX_Text(pDX, IDC_AnkleRY, mAnkleFlexR[1]);
	DDX_Text(pDX, IDC_AnkleRZ, mAnkleFlexR[2]);

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


BEGIN_MESSAGE_MAP(ReportStrengthDirVectors, ReportView)
	//{{AFX_MSG_MAP(ReportStrengthDirVectors)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
	//}}AFX_MSG_MAP
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

#ifdef _DEBUG
void ReportStrengthDirVectors::AssertValid() const { CFormView::AssertValid(); }
void ReportStrengthDirVectors::Dump(CDumpContext& dc) const { CFormView::Dump(dc); }
#endif

//void ReportStrengthDirVectors::OnFilePrintPreview() { SSPPPrintPreview(this); }

void ReportStrengthDirVectors::OnUpdate(CView* aSenderPtr,LPARAM aHint,CObject* aHintPtr)
{
    C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();
	Skeleton* skel = lDocPtr->GetSkeleton();
	Vector3 vectors[NUM_STRENGTH_HINGES];
	skel->getStrengthDirectionVectors(vectors);

    // Write results to the form controls, converting to the desired system of units in the process
    /*CString *reported[NUM_STRENGTH_HINGES] = {mElbowL,mElbowR,mShoulderRotL,mShoulderRotR,mShoulderAddL,mShoulderAddR,mShoulderHumL,mShoulderHumR,
                           mL5S1FlexL,mL5S1BendL,mL5S1RotL,mHipFlexL,mHipFlexR,mKneeFlexL,mKneeFlexR,mAnkleFlexL,mAnkleFlexR};*/
	for(int i = 0; i < NUM_STRENGTH_HINGES; ++i) {
        for(int j = 0; j < 3; ++j)
        {
            // yeah...
			double val = vectors[i][j];
			float rounded = (float)floor(val * 1000.0 + 0.5) / 1000.0f;
			hingeToMember[i][j].Format("%.3g",rounded);
            /*double val = *((double*)&vectors + 3*i + j);
            float rounded = (float)floor(val * 1000.0 + 0.5) / 1000.0f;
            reported[i][j].Format("%.3g",rounded);*/
        }
	}

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

HBRUSH ReportStrengthDirVectors::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();
 
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
    pDC->SetBkColor(lDocPtr->getBalColor());

    return (HBRUSH)(CBrush(lDocPtr->getBalColor()));
}

void ReportStrengthDirVectors::OnDisplayDialogcolor() 
{
	C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();
	lDocPtr->setReportColor();
	
	Invalidate(true);
}

void ReportStrengthDirVectors::OnInitialUpdate()
{
	ReportView::OnInitialUpdate();

	// Set the hand force box units
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();
	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";
	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}


void ReportStrengthDirVectors::UpdateUnits() {
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";

	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";

	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}