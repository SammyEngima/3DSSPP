#include "stdafx.h"
#include "ReportSpinal.h"
#include "hom_doc.h"
#include "Skeleton.hpp"
#include "Types.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ReportView *ReportSpinal::cPointer = NULL;

IMPLEMENT_DYNCREATE(ReportSpinal, ReportView)

ReportSpinal::ReportSpinal() : ReportView(ReportSpinal::IDD)
{
	//{{AFX_DATA_INIT(ReportSpinal)
	mHeader = _T("");
	mFooter = _T("");
	mGloForcesLabel = _T("");
	mGloMomentsLabel = _T("");
	mRotForcesLabel = _T("");
	mRotMomentsLabel = _T("");
    for(int i = 0; i < 3; ++i)
    {
        mGloL2L3For[i] = _T("");
	    mGloL2L3Mom[i] = _T("");
	    mGloL4L5For[i] = _T("");
	    mGloL4L5Mom[i] = _T("");
	    mGloL5S1Mom[i] = _T("");
	    mRotL2L3For[i] = _T("");
	    mRotL2L3Mom[i] = _T("");
	    mRotL3L4For[i] = _T("");
	    mRotL3L4Mom[i] = _T("");
	    mRotL4L5For[i] = _T("");
	    mRotL4L5Mom[i] = _T("");
	}
//	mRotL5S1MomZ = _T("");
	//}}AFX_DATA_INIT

   cPointer = this;
   return;
}

ReportSpinal::~ReportSpinal() { cPointer = NULL; }

CString ReportSpinal::ReportName(void) const { return "3DSSPP - Spinal Forces and Moments"; }

void ReportSpinal::DoDataExchange(CDataExchange* pDX)
{
	ReportView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ReportSpinal)
	DDX_Text(pDX, IDC_Header, mHeader);
	DDX_Text(pDX, IDC_Footer, mFooter);
	DDX_Text(pDX, IDC_GloForcesLabel, mGloForcesLabel);
	DDX_Text(pDX, IDC_GloMomentsLabel, mGloMomentsLabel);
	DDX_Text(pDX, IDC_RotForcesLabel, mRotForcesLabel);
	DDX_Text(pDX, IDC_RotMomentsLabel, mRotMomentsLabel);
    DDX_Text(pDX, IDC_GloL2L3ForX, mGloL2L3For[0]);
	DDX_Text(pDX, IDC_GloL2L3ForY, mGloL2L3For[1]);
	DDX_Text(pDX, IDC_GloL2L3ForZ, mGloL2L3For[2]);
	DDX_Text(pDX, IDC_GloL2L3MomX, mGloL2L3Mom[0]);
	DDX_Text(pDX, IDC_GloL2L3MomY, mGloL2L3Mom[1]);
	DDX_Text(pDX, IDC_GloL2L3MomZ, mGloL2L3Mom[2]);
    DDX_Text(pDX, IDC_GloL3L4ForX, mGloL3L4For[0]);
	DDX_Text(pDX, IDC_GloL3L4ForY, mGloL3L4For[1]);
	DDX_Text(pDX, IDC_GloL3L4ForZ, mGloL3L4For[2]);
	DDX_Text(pDX, IDC_GloL3L4MomX, mGloL3L4Mom[0]);
	DDX_Text(pDX, IDC_GloL3L4MomY, mGloL3L4Mom[1]);
	DDX_Text(pDX, IDC_GloL3L4MomZ, mGloL3L4Mom[2]);
	DDX_Text(pDX, IDC_GloL4L5ForX, mGloL4L5For[0]);
	DDX_Text(pDX, IDC_GloL4L5ForY, mGloL4L5For[1]);
	DDX_Text(pDX, IDC_GloL4L5ForZ, mGloL4L5For[2]);
	DDX_Text(pDX, IDC_GloL4L5MomX, mGloL4L5Mom[0]);
	DDX_Text(pDX, IDC_GloL4L5MomY, mGloL4L5Mom[1]);
	DDX_Text(pDX, IDC_GloL4L5MomZ, mGloL4L5Mom[2]);
	DDX_Text(pDX, IDC_GloL5S1ForX, mGloL5S1For[0]);
	DDX_Text(pDX, IDC_GloL5S1ForY, mGloL5S1For[1]);
	DDX_Text(pDX, IDC_GloL5S1ForZ, mGloL5S1For[2]);
	DDX_Text(pDX, IDC_GloL5S1MomX, mGloL5S1Mom[0]);
	DDX_Text(pDX, IDC_GloL5S1MomY, mGloL5S1Mom[1]);
	DDX_Text(pDX, IDC_GloL5S1MomZ, mGloL5S1Mom[2]);
	DDX_Text(pDX, IDC_RotL2L3ForX, mRotL2L3For[0]);
	DDX_Text(pDX, IDC_RotL2L3ForY, mRotL2L3For[1]);
	DDX_Text(pDX, IDC_RotL2L3ForZ, mRotL2L3For[2]);
	DDX_Text(pDX, IDC_RotL2L3MomX, mRotL2L3Mom[0]);
	DDX_Text(pDX, IDC_RotL2L3MomY, mRotL2L3Mom[1]);
	DDX_Text(pDX, IDC_RotL2L3MomZ, mRotL2L3Mom[2]);
	DDX_Text(pDX, IDC_RotL3L4ForX, mRotL3L4For[0]);
	DDX_Text(pDX, IDC_RotL3L4ForY, mRotL3L4For[1]);
	DDX_Text(pDX, IDC_RotL3L4ForZ, mRotL3L4For[2]);
	DDX_Text(pDX, IDC_RotL3L4MomX, mRotL3L4Mom[0]);
	DDX_Text(pDX, IDC_RotL3L4MomY, mRotL3L4Mom[1]);
	DDX_Text(pDX, IDC_RotL3L4MomZ, mRotL3L4Mom[2]);
	DDX_Text(pDX, IDC_RotL4L5ForX, mRotL4L5For[0]);
	DDX_Text(pDX, IDC_RotL4L5ForY, mRotL4L5For[1]);
	DDX_Text(pDX, IDC_RotL4L5ForZ, mRotL4L5For[2]);
	DDX_Text(pDX, IDC_RotL4L5MomX, mRotL4L5Mom[0]);
	DDX_Text(pDX, IDC_RotL4L5MomY, mRotL4L5Mom[1]);
	DDX_Text(pDX, IDC_RotL4L5MomZ, mRotL4L5Mom[2]);
	DDX_Text(pDX, IDC_RotL5S1ForX, mRotL5S1For[0]);
	DDX_Text(pDX, IDC_RotL5S1ForY, mRotL5S1For[1]);
	DDX_Text(pDX, IDC_RotL5S1ForZ, mRotL5S1For[2]);
	DDX_Text(pDX, IDC_RotL5S1MomX, mRotL5S1Mom[0]);
	DDX_Text(pDX, IDC_RotL5S1MomY, mRotL5S1Mom[1]);
	DDX_Text(pDX, IDC_RotL5S1MomZ, mRotL5S1Mom[2]);


	 //DDX_Text(pDX, IDC_RotL5S1MomZ, mRotL5S1MomZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ReportSpinal, ReportView)
	//{{AFX_MSG_MAP(ReportSpinal)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
	//}}AFX_MSG_MAP
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

#ifdef _DEBUG
void ReportSpinal::AssertValid() const { CFormView::AssertValid(); }
void ReportSpinal::Dump(CDumpContext& dc) const { CFormView::Dump(dc); }
#endif

//void ReportSpinal::OnFilePrintPreview() { SSPPPrintPreview(this); }

void ReportSpinal::OnUpdate(CView* aSenderPtr,LPARAM aHint,CObject* aHintPtr)
{
    C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();
    //Analysis &lResults = lDocPtr->getResultsRef();
	const SpinalForces_s& forces = lDocPtr->GetSkeleton()->getSpinalForces();
	const SpinalMoments_s& moments = lDocPtr->GetSkeleton()->getSpinalMoments();
	Vector3 L5S1NetForce = lDocPtr->GetSkeleton()->getNetForce(JT_L5S1_TEMP);


    // Fill in measurement units text
    
	// Sayan 07/16/07

	// The few lines below are no longer necessary now, since the units are now being
	// handled differently and in a cleaner, more modular way, elsewhere in the code.

	// Force tempf(0,1);
    // mRotForcesLabel = mGloForcesLabel = CString("") + "Forces(" + tempf.UnitsText() + ")";
	mRotForcesLabel = mGloForcesLabel = CString("Forces (") + lDocPtr->ForceUnits() + ")";
    // Torque tempm(0,1);
    // mRotMomentsLabel = mGloMomentsLabel = CString("") + "Moments(" + tempm.UnitsText() + ")";
	mRotMomentsLabel = mGloMomentsLabel = CString("Moments (") + lDocPtr->TorqueUnits() + ")";

    // Retrieve joint moment analysis results
    /*SpinalForceData forces;
    SpinalMomentData moments;
    lResults.getSpinalForces(&forces);
    lResults.getSpinalMoments(&moments);*/

    // Write results to the form controls, converting to the desired system of units in the process
    CString *reportedForces[] = {mGloL2L3For, mGloL3L4For, mGloL4L5For, mGloL5S1For, mRotL2L3For, mRotL3L4For, mRotL4L5For};
    CString *reportedMoments[] = {mGloL2L3Mom, mGloL3L4Mom, mGloL4L5Mom, mGloL5S1Mom, mRotL2L3Mom, mRotL3L4Mom, mRotL4L5Mom};
	// L3L4 values are average of L2L3 and L4L5
	double forcesglobalL3L4[3] = {(forces.globalL2L3[0] + forces.globalL4L5[0])/2,
								(forces.globalL2L3[1] + forces.globalL4L5[1])/2,
								(forces.globalL2L3[2] + forces.globalL4L5[2])/2};
	double momentsglobalL3L4[3] = {(moments.globalL2L3[0] + moments.globalL4L5[0])/2,
								(moments.globalL2L3[1] + moments.globalL4L5[1])/2,
								(moments.globalL2L3[2] + moments.globalL4L5[2])/2};
	const double* force_array[] = {forces.globalL2L3, forcesglobalL3L4, forces.globalL4L5, L5S1NetForce.ptr(), forces.rotatedL2L3, forces.rotatedL3L4, forces.rotatedL4L5};
	const double* moment_array[] = {moments.globalL2L3, momentsglobalL3L4, moments.globalL4L5, moments.globalL5S1, moments.rotatedL2L3, moments.rotatedL3L4, moments.rotatedL4L5};

	for(int i = 0; i < 7; ++i) {
        for(int j = 0; j < 3; ++j) {
			reportedForces[i][j].Format("%.1f", force_array[i][j]);
        }
	}
	for(int i = 0; i < 7; ++i) {
        for(int j = 0; j < 3; ++j) {
			reportedMoments[i][j].Format("%.1f", moment_array[i][j]);
        }
	}

    // L5/S1: Value for z moment only

    // Sayan 07/18/07
    // 
    // We now handle the unit conversions in a much cleaner way.

    // tempm.Value(100.0f * moments.rotatedL5S1_z, MetricCns);
    // mRotL5S1MomZ = tempm.ValueText();

    //mRotL5S1MomZ.Format("%.1f", moments.rotatedL5S1_z);
	mRotL5S1Mom[2].Format("%.1f", moments.rotatedL5S1_z);

    mHeader = lDocPtr->ReportHeader();
	mFooter = lDocPtr->ReportFooter();
       
    UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// ReportSpinal message handlers

HBRUSH ReportSpinal::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
		HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
 
   pDC->SetBkMode(TRANSPARENT);
   pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
   pDC->SetBkColor(lDocPtr->getBalColor());

   return (HBRUSH)(CBrush(lDocPtr->getBalColor()));
}

void ReportSpinal::OnDisplayDialogcolor() 
{
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
	lDocPtr->setReportColor();
	
	Invalidate(true);
}
