#include "ReportLowBack.h"

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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ReportView *ReportLowBack::cPointer = NULL;

IMPLEMENT_DYNCREATE(ReportLowBack, ReportView)

ReportLowBack::ReportLowBack() : ReportView(ReportLowBack::IDD)
{
	//{{AFX_DATA_INIT(ReportLowBack)
	mFooter = _T("");
	mHeader = _T("");
    mArmsLabel = _T("");
	mCompressionLabel = _T("");
	mForcesLabel = _T("");
    mShearLabel = _T("");

	mHandForceUnits = _T("");
	mUnitsLabel = _T("");
	mLeftHori = _T("");

    for(int i = 0; i < 5; ++i)
	{
        mLEOForce[i] = _T("");
        mLESForce[i] = _T("");
        mLIOForce[i] = _T("");
        mLLDForce[i] = _T("");
        mLRAForce[i] = _T("");
        mREOForce[i] = _T("");
        mRESForce[i] = _T("");
        mRIOForce[i] = _T("");
        mRLDForce[i] = _T("");
        mRRAForce[i] = _T("");
    }
	    
    for(int i = 0; i < 2; ++i)
    {
        mLEOArm[i] = _T("");
        mLESArm[i] = _T("");
        mLIOArm[i] = _T("");
        mLLDArm[i] = _T("");
        mLRAArm[i] = _T("");
        mREOArm[i] = _T("");
        mRESArm[i] = _T("");
        mRIOArm[i] = _T("");
        mRLDArm[i] = _T("");
        mRRAArm[i] = _T("");
    }
	
	mDiscCompression = _T("");
    mDiscAntPostShear = _T("");
	mDiscLateralShear = _T("");
	mDiscShear = _T("");
    //}}AFX_DATA_INIT
   cPointer = this;

	muscleToMember[BM_LeftErector] = mLESForce;
	muscleToMember[BM_RightErector] = mRESForce;
	muscleToMember[BM_LeftRectus] = mLRAForce;
	muscleToMember[BM_RightRectus] = mRRAForce;
	muscleToMember[BM_LeftInternal] = mLIOForce;
	muscleToMember[BM_RightInternal] = mRIOForce;
	muscleToMember[BM_LeftExternal] = mLEOForce;
	muscleToMember[BM_RightExternal] = mREOForce;
	muscleToMember[BM_LeftLatis] = mLLDForce;
	muscleToMember[BM_RightLatis] = mRLDForce;

	armToMember[BM_LeftErector] = mLESArm;
	armToMember[BM_RightErector] = mRESArm;
	armToMember[BM_LeftRectus] = mLRAArm;
	armToMember[BM_RightRectus] = mRRAArm;
	armToMember[BM_LeftInternal] = mLIOArm;
	armToMember[BM_RightInternal] = mRIOArm;
	armToMember[BM_LeftExternal] = mLEOArm;
	armToMember[BM_RightExternal] = mREOArm;
	armToMember[BM_LeftLatis] = mLLDArm;
	armToMember[BM_RightLatis] = mRLDArm;
}

ReportLowBack::~ReportLowBack() { cPointer = NULL; }

CString ReportLowBack::ReportName(void) const { return "3DSSPP - 3D Lowback Analysis Report"; }

void ReportLowBack::DoDataExchange(CDataExchange* pDX)
{
	ReportView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ReportLowBack)
	DDX_Text(pDX, IDC_Footer, mFooter);
	DDX_Text(pDX, IDC_Header, mHeader);
	DDX_Text(pDX, IDC_ArmsLabel, mArmsLabel);
	DDX_Text(pDX, IDC_CompressionLabel, mCompressionLabel);
	DDX_Text(pDX, IDC_ForcesLabel, mForcesLabel);
	DDX_Text(pDX, IDC_ShearLabel, mShearLabel);

	DDX_Text(pDX, IDC_DiscCompression, mDiscCompression);
	DDX_Text(pDX, IDC_DiscAntPostShear, mDiscAntPostShear);
	DDX_Text(pDX, IDC_DiscLateralShear, mDiscLateralShear);
	DDX_Text(pDX, IDC_DiscShear, mDiscShear);

	DDX_Text(pDX, IDC_LEOArmX, mLEOArm[0]);
	DDX_Text(pDX, IDC_LEOArmY, mLEOArm[1]);
    DDX_Text(pDX, IDC_LESArmX, mLESArm[0]);
	DDX_Text(pDX, IDC_LESArmY, mLESArm[1]);
    DDX_Text(pDX, IDC_LIOArmX, mLIOArm[0]);
	DDX_Text(pDX, IDC_LIOArmY, mLIOArm[1]);
    DDX_Text(pDX, IDC_LLDArmX, mLLDArm[0]);
	DDX_Text(pDX, IDC_LLDArmY, mLLDArm[1]);
    DDX_Text(pDX, IDC_LRAArmX, mLRAArm[0]);
	DDX_Text(pDX, IDC_LRAArmY, mLRAArm[1]);
    DDX_Text(pDX, IDC_REOArmX, mREOArm[0]);
	DDX_Text(pDX, IDC_REOArmY, mREOArm[1]);
    DDX_Text(pDX, IDC_RESArmX, mRESArm[0]);
	DDX_Text(pDX, IDC_RESArmY, mRESArm[1]);
    DDX_Text(pDX, IDC_RIOArmX, mRIOArm[0]);
	DDX_Text(pDX, IDC_RIOArmY, mRIOArm[1]);
    DDX_Text(pDX, IDC_RLDArmX, mRLDArm[0]);
	DDX_Text(pDX, IDC_RLDArmY, mRLDArm[1]);
    DDX_Text(pDX, IDC_RRAArmX, mRRAArm[0]);
	DDX_Text(pDX, IDC_RRAArmY, mRRAArm[1]);

	DDX_Text(pDX, IDC_LEOResult, mLEOForce[0]);
    DDX_Text(pDX, IDC_LEOShear, mLEOForce[1]);
	DDX_Text(pDX, IDC_LEOForceX, mLEOForce[2]);
	DDX_Text(pDX, IDC_LEOForceY, mLEOForce[3]);
	DDX_Text(pDX, IDC_LEOForceZ, mLEOForce[4]);
	DDX_Text(pDX, IDC_LESResult, mLESForce[0]);
	DDX_Text(pDX, IDC_LESShear, mLESForce[1]);
    DDX_Text(pDX, IDC_LESForceX, mLESForce[2]);
	DDX_Text(pDX, IDC_LESForceY, mLESForce[3]);
	DDX_Text(pDX, IDC_LESForceZ, mLESForce[4]);
	DDX_Text(pDX, IDC_LIOResult, mLIOForce[0]);
	DDX_Text(pDX, IDC_LIOShear, mLIOForce[1]);
    DDX_Text(pDX, IDC_LIOForceX, mLIOForce[2]);
	DDX_Text(pDX, IDC_LIOForceY, mLIOForce[3]);
	DDX_Text(pDX, IDC_LIOForceZ, mLIOForce[4]);
	DDX_Text(pDX, IDC_LLDResult, mLLDForce[0]);
	DDX_Text(pDX, IDC_LLDShear, mLLDForce[1]);
    DDX_Text(pDX, IDC_LLDForceX, mLLDForce[2]);
	DDX_Text(pDX, IDC_LLDForceY, mLLDForce[3]);
	DDX_Text(pDX, IDC_LLDForceZ, mLLDForce[4]);
	DDX_Text(pDX, IDC_LRAResult, mLRAForce[0]);
	DDX_Text(pDX, IDC_LRAShear, mLRAForce[1]);
    DDX_Text(pDX, IDC_LRAForceX, mLRAForce[2]);
	DDX_Text(pDX, IDC_LRAForceY, mLRAForce[3]);
	DDX_Text(pDX, IDC_LRAForceZ, mLRAForce[4]);
	DDX_Text(pDX, IDC_REOResult, mREOForce[0]);
	DDX_Text(pDX, IDC_REOShear, mREOForce[1]);
    DDX_Text(pDX, IDC_REOForceX, mREOForce[2]);
	DDX_Text(pDX, IDC_REOForceY, mREOForce[3]);
	DDX_Text(pDX, IDC_REOForceZ, mREOForce[4]);
	DDX_Text(pDX, IDC_RESResult, mRESForce[0]);
	DDX_Text(pDX, IDC_RESShear, mRESForce[1]);
    DDX_Text(pDX, IDC_RESForceX, mRESForce[2]);
	DDX_Text(pDX, IDC_RESForceY, mRESForce[3]);
	DDX_Text(pDX, IDC_RESForceZ, mRESForce[4]);
	DDX_Text(pDX, IDC_RIOResult, mRIOForce[0]);
	DDX_Text(pDX, IDC_RIOShear, mRIOForce[1]);
    DDX_Text(pDX, IDC_RIOForceX, mRIOForce[2]);
	DDX_Text(pDX, IDC_RIOForceY, mRIOForce[3]);
	DDX_Text(pDX, IDC_RIOForceZ, mRIOForce[4]);
	DDX_Text(pDX, IDC_RLDResult, mRLDForce[0]);
	DDX_Text(pDX, IDC_RLDShear, mRLDForce[1]);
    DDX_Text(pDX, IDC_RLDForceX, mRLDForce[2]);
	DDX_Text(pDX, IDC_RLDForceY, mRLDForce[3]);
	DDX_Text(pDX, IDC_RLDForceZ, mRLDForce[4]);
	DDX_Text(pDX, IDC_RRAResult, mRRAForce[0]);
	DDX_Text(pDX, IDC_RRAShear, mRRAForce[1]);
	DDX_Text(pDX, IDC_RRAForceX, mRRAForce[2]);
	DDX_Text(pDX, IDC_RRAForceY, mRRAForce[3]);
	DDX_Text(pDX, IDC_RRAForceZ, mRRAForce[4]);

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

BEGIN_MESSAGE_MAP(ReportLowBack, ReportView)
	//{{AFX_MSG_MAP(ReportLowBack)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
	//}}AFX_MSG_MAP
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

#ifdef _DEBUG
void ReportLowBack::AssertValid() const { CFormView::AssertValid(); }
void ReportLowBack::Dump(CDumpContext& dc) const { CFormView::Dump(dc); }
#endif

//void ReportLowBack::OnFilePrintPreview() { SSPPPrintPreview(this); }

void ReportLowBack::OnUpdate(CView* aSenderPtr,LPARAM aHint,CObject* aHintPtr)
{
    C_Hom_Doc* lDocPtr = (C_Hom_Doc*)GetDocument();
    //Analysis &lResults = lDocPtr->getResultsRef();

    // Fill in measurement units text
    //Length templ(0.0F,1);
    //Force tempf(0.0F,0);
	mArmsLabel = CString("") + "Mom. Arms (" + lDocPtr->LengthUnits() + ")";
	mForcesLabel = CString("") + "Forces (" + lDocPtr->ForceUnits() + ")";
    mCompressionLabel = CString("") + "Compression (" + lDocPtr->ForceUnits() + ")"; 
    mShearLabel = CString("") + "Shear (" + lDocPtr->ForceUnits() + ")"; 

    // Get 3d low back analysis results
    //LowBack3DData lowback;
    //lResults.getLowBack3D(&lowback);
	const LowBack3DData_s& lowBack = lDocPtr->GetSkeleton()->getLowBack3DData();

    // Report lowback forces
    //CString *reportedForces[] = {mLESForce,mRESForce,mLRAForce,mRRAForce,mLIOForce,mRIOForce,mLEOForce,mREOForce,mLLDForce,mRLDForce};
	const double* forceData[] = {lowBack.Resultant, lowBack.Shear, lowBack.XForce, lowBack.YForce, lowBack.ZForce };
	for(int i = 0; i < NUM_BACK_MUSCLES; ++i) {
        for(int j = 0; j < 5; ++j) {
            //tempf.Value(*((double*)&lowback + 5*i + j), MetricCns);
            //reportedForces[i][j] = tempf.ValueText();
			
			// Sayan 07/13/07
			//
			// Cleaned this up as unit conversion is now handled elsewhere

			muscleToMember[i][j].Format("%.0f",forceData[j][i]);
		}
	}

    // Report lowback moment arms
    //CString *reportedArms[] = {mLESArm,mRESArm,mLRAArm,mRRAArm,mLIOArm,mRIOArm,mLEOArm,mREOArm,mLLDArm,mRLDArm};
	const double* armData[] = {lowBack.XArm, lowBack.YArm};
	for(int i = 0; i < NUM_BACK_MUSCLES; ++i) {
        for(int j = 0; j < 2; ++j)
        {
            //templ.Value(*((double*)&lowback + 50 + 2*i + j), MetricCns);
            //reportedArms[i][j] = templ.ValueText();

		    // Sayan 07/13/07
			//
			// Cleaned this up as unit conversion is now handled elsewhere

			armToMember[i][j].Format("%.1f",fabs(armData[j][i]));
        }
	}

    // Report L4L5 disc compression and shear
    /*CString *reportedL4L5[] = {&mDiscCompression, &mDiscShear, &mDiscAntPostShear, &mDiscLateralShear};
    for(int i = 0; i < 4; ++i)
    {
        tempf.Value(*((double*)&lowback + 70 + i), MetricCns);
        *(reportedL4L5[i]) = tempf.ValueText();
    }*/

	// Sayan 07/13/07
    //
    // Cleaned this up as unit conversion is now handled elsewhere
	UpdateUnits();
	
	mDiscCompression.Format("%.0f",lowBack.totalL4L5Compression); 
	mDiscShear.Format("%.0f",lowBack.totalL4L5Shear);
	mDiscAntPostShear.Format("%.0f",lowBack.anteriorL4L5Shear);   
	mDiscLateralShear.Format("%.0f",lowBack.lateralL4L5Shear);       

    mHeader = lDocPtr->ReportHeader();
    mFooter = lDocPtr->ReportFooter();
       
    UpdateData(FALSE);

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

HBRUSH ReportLowBack::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
    	
    C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();
     
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
    pDC->SetBkColor(lDocPtr->getBalColor());

    return (HBRUSH)(CBrush(lDocPtr->getBalColor()));
}

void ReportLowBack::OnDisplayDialogcolor() 
{
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
	lDocPtr->setReportColor();
	
	Invalidate(true);
}

void ReportLowBack::OnInitialUpdate()
{
	ReportView::OnInitialUpdate();

	// Set the hand force box units
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();
	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";
	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}


void ReportLowBack::UpdateUnits() {
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";

	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";

	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}