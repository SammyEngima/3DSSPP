 // ReportSagittal.cpp : implementation file
//

#include "ReportSagittal.h"

#include "hom_doc.h"	// Necessary
#include "Skeleton.hpp"
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

ReportView * ReportSagittal::cPointer = NULL;

/////////////////////////////////////////////////////////////////////////////
// ReportSagittal

IMPLEMENT_DYNCREATE(ReportSagittal, ReportView)

ReportSagittal::ReportSagittal() : ReportView(ReportSagittal::IDD)
{
	//{{AFX_DATA_INIT(ReportSagittal)
	mAbdominal = _T("");
	mCompressionBodyWeight = _T("");
	mCompressionHandLoads = _T("");
	mErectorSpinae = _T("");
	mFooter = _T("");
	mFrontal = _T("");
	mHeader = _T("");
	mRectusAbdominus = _T("");
	mStrain = _T("");
	mTotalCompression = _T("");
	mTotalShear = _T("");
	mSagittal = _T("");
	mErectorSpinaeSD = _T("");
	mRectusAbdominusSD = _T("");
	mTotalCompressionSD = _T("");
	mCompressionLabel = _T("");
	mShearLabel = _T("");

	mHandForceUnits = _T("");
	mUnitsLabel = _T("");
	mLeftHori = _T("");
	//}}AFX_DATA_INIT
   
   cPointer = this;
   return;
}

ReportSagittal::~ReportSagittal() { cPointer = NULL; }

CString ReportSagittal::ReportName(void) const { return "3DSSPP - Sagittal Plane Lowback Analysis Report"; }

void ReportSagittal::DoDataExchange(CDataExchange* pDX)
{
	ReportView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ReportSagittal)
	DDX_Text(pDX, IDC_Abdominal, mAbdominal);
	DDX_Text(pDX, IDC_CompressionBodyWeight, mCompressionBodyWeight);
	DDX_Text(pDX, IDC_CompressionHandLoads, mCompressionHandLoads);
	DDX_Text(pDX, IDC_ErectorSpinae, mErectorSpinae);
	DDX_Text(pDX, IDC_Footer, mFooter);
	DDX_Text(pDX, IDC_Frontal, mFrontal);
	DDX_Text(pDX, IDC_Header, mHeader);
	DDX_Text(pDX, IDC_RectusAbdominus, mRectusAbdominus);
	DDX_Text(pDX, IDC_Strain, mStrain);
	DDX_Text(pDX, IDC_TotalCompression, mTotalCompression);
	DDX_Text(pDX, IDC_TotalShear, mTotalShear);
	DDX_Text(pDX, IDC_Sagittal, mSagittal);
	DDX_Text(pDX, IDC_ErectorSpinaeSD, mErectorSpinaeSD);
	DDX_Text(pDX, IDC_RectusAbdominusSD, mRectusAbdominusSD);
	DDX_Text(pDX, IDC_TotalCompressionSD, mTotalCompressionSD);
	DDX_Text(pDX, IDC_CompressionLabel, mCompressionLabel);
	DDX_Text(pDX, IDC_ShearLabel, mShearLabel);

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


BEGIN_MESSAGE_MAP(ReportSagittal, ReportView)
	//{{AFX_MSG_MAP(ReportSagittal)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
	//}}AFX_MSG_MAP
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

#ifdef _DEBUG
void ReportSagittal::AssertValid() const { CFormView::AssertValid(); }
void ReportSagittal::Dump(CDumpContext& dc) const { CFormView::Dump(dc); }
#endif

//void ReportSagittal::OnFilePrintPreview() { SSPPPrintPreview(this); }

void ReportSagittal::OnUpdate(CView* aSenderPtr,LPARAM aHint,CObject* aHintPtr)
{
    C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();
    //Analysis &lResults = lDocPtr->getResultsRef();

    // Fill in measurement units text
//    Force temp(0.0F,0);
    mCompressionLabel = CString("") + "Total Compression (" + lDocPtr->ForceUnits() + "):";
    mShearLabel = CString("") + "Total Shear (" + lDocPtr->ForceUnits() + "):";

    // Get sagittal low back analysis results
    //LowBackSagittalData sagittal;
    //lResults.getLowBackSagittal(&sagittal);
	const LowBackSagittalData_s& sagittal = lDocPtr->GetSkeleton()->getLowBackSagittalData();

    // Write results to the form controls, converting to the desired system of units in the process
	const int NUM_DATUM = 12;
    CString* reported[] = {&mTotalCompression,&mTotalCompressionSD,&mErectorSpinae,&mErectorSpinaeSD,&mRectusAbdominus,
                           &mRectusAbdominusSD,&mAbdominal,&mCompressionHandLoads,&mCompressionBodyWeight,&mTotalShear,
                           &mSagittal,&mFrontal};
	double data[] = {sagittal.cmpTotal, sagittal.cmpTotalSD, sagittal.cmpSpinae, sagittal.cmpSpinaeSD, sagittal.cmpAbdominus,
					sagittal.cmpAbdominusSD, sagittal.cmpAbdominal, sagittal.cmpHandLoads, sagittal.cmpUpperBody, sagittal.shrTotal,
					sagittal.shrSagittal, sagittal.shrFrontal};
    for(int i = 0; i < NUM_DATUM; ++i)
    {
		(*(reported[i])).Format("%.0f", data[i]);
        //temp.Value(*((double*)&sagittal + i), MetricCns); // + 2 to pass by total compression / stddev
		//temp.Value(data[i], MetricCns);
        //*(reported[i]) = temp.ValueText();
    }
    mStrain.Format("%.0f",sagittal.ligamentStrain);

    // Add +/- to the standard deviations
    mTotalCompressionSD = "+/- " + mTotalCompressionSD;
    mErectorSpinaeSD = "+/- " + mErectorSpinaeSD;
    mRectusAbdominusSD = "+/- " + mRectusAbdominusSD;

	mHeader = lDocPtr->ReportHeader();
    mFooter = lDocPtr->ReportFooter();
       
    UpdateData( FALSE );

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

/////////////////////////////////////////////////////////////////////////////
// ReportSagittal message handlers

HBRUSH ReportSagittal::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
 
   pDC->SetBkMode(TRANSPARENT);
   pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
   pDC->SetBkColor(lDocPtr->getBalColor());

   return (HBRUSH)(CBrush(lDocPtr->getBalColor()));
}

void ReportSagittal::OnDisplayDialogcolor() 
{
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
	lDocPtr->setReportColor();
	
	Invalidate(true);
}

void ReportSagittal::OnInitialUpdate()
{
	ReportView::OnInitialUpdate();

	// Set the hand force box units
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();
	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";
	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}

void ReportSagittal::UpdateUnits() {
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";

	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";

	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}
