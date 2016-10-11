// ReportBalanceSeated.cpp : implementation file
//

#include "ReportBalanceSeated.h"

#include "hom.h"
#include "hom_doc.h"	// Necessary
#include "../c4eLibNew/units.hpp"
#include "Analysis.h"
// #include "SeatCOP.h" //for seated COP graphic
#include ".\reportbalanceseated.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#undef	BORDER_WIDTH
#define	BORDER_WIDTH	2

/////////////////////////////////////////////////////////////////////////////
// ReportBalanceSeated
ReportView * ReportBalanceSeated::cPointer = NULL;

IMPLEMENT_DYNCREATE(ReportBalanceSeated, ReportView)

ReportBalanceSeated::ReportBalanceSeated()
	: ReportView(ReportBalanceSeated::IDD),
	glBalanceSitting(this)
{
	//{{AFX_DATA_INIT(ReportBalanceSeated)
	mFooter = _T("");
	mHeader = _T("");
	mUnits4 = _T("");
	mCOPseat_y = _T("");
	mCOPseat_x = _T("");
	mSeatBalance = _T("");
	mUnits5 = _T("");
	m_seatFB = _T("");
	m_seatLB = _T("");
	m_seatRB = _T("");
	mResMomX = _T("");
    mResMomY = _T("");
    mResMomZ = _T("");
	//}}AFX_DATA_INIT
	cPointer = this;
	COPBoxCreated = false;
	COPGraphicError = false;
	return;
}

ReportBalanceSeated::~ReportBalanceSeated()
{
	cPointer = NULL;
	return;
}

CString ReportBalanceSeated::ReportName() const
{
   return   "3DSSPP - Balance Report - Seated";
}

void ReportBalanceSeated::DoDataExchange(CDataExchange* pDX)
{
	ReportView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ReportBalanceSeated)
	DDX_Text(pDX, IDC_Footer, mFooter);
	DDX_Text(pDX, IDC_Header, mHeader);
	DDX_Text(pDX, IDC_LocationUnits4, mUnits4);
	DDX_Text(pDX, IDC_yCOPseat, mCOPseat_y);
	DDX_Text(pDX, IDC_xCOPseat, mCOPseat_x);
	DDX_Text(pDX, IDC_CGBalanceValue, mSeatBalance);
	DDX_Text(pDX, IDC_LocationUnits5, mUnits5);
	DDX_Text(pDX, IDC_LocationUnits7, mUnits7);
	DDX_Text(pDX, IDC_seatFB, m_seatFB);
	DDX_Text(pDX, IDC_seatLB, m_seatLB);
	DDX_Text(pDX, IDC_seatRB, m_seatRB);

	DDX_Text(pDX, IDC_RightITMomX, mRightITMomX);
	DDX_Text(pDX, IDC_RightITMomY, mRightITMomY);
	DDX_Text(pDX, IDC_RightITMomZ, mRightITMomZ);
	DDX_Text(pDX, IDC_RightSFMomX, mRightSFMomX);
	DDX_Text(pDX, IDC_RightSFMomY, mRightSFMomY);
	DDX_Text(pDX, IDC_RightSFMomZ, mRightSFMomZ);
	DDX_Text(pDX, IDC_LeftITMomX, mLeftITMomX);
	DDX_Text(pDX, IDC_LeftITMomY, mLeftITMomY);
	DDX_Text(pDX, IDC_LeftITMomZ, mLeftITMomZ);
    DDX_Text(pDX, IDC_LeftBallMomX, mLeftBallMomX);
	DDX_Text(pDX, IDC_LeftBallMomY, mLeftBallMomY);
	DDX_Text(pDX, IDC_LeftBallMomZ, mLeftBallMomZ);
    DDX_Text(pDX, IDC_RightBallMomX, mRightBallMomX);
	DDX_Text(pDX, IDC_RightBallMomY, mRightBallMomY);
	DDX_Text(pDX, IDC_RightBallMomZ, mRightBallMomZ);
	DDX_Text(pDX, IDC_LeftSFMomX, mLeftSFMomX);
	DDX_Text(pDX, IDC_LeftSFMomY, mLeftSFMomY);
	DDX_Text(pDX, IDC_LeftSFMomZ, mLeftSFMomZ);
	DDX_Text(pDX, IDC_ResidualMomBox, mResidualMomBox);
    DDX_Text(pDX, IDC_ResMomX, mResMomX);
    DDX_Text(pDX, IDC_ResMomY, mResMomY);
    DDX_Text(pDX, IDC_ResMomZ, mResMomZ);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ReportBalanceSeated, ReportView)
	//{{AFX_MSG_MAP(ReportBalanceSeated)
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
	//}}AFX_MSG_MAP
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ReportBalanceSeated diagnostics

#ifdef _DEBUG
void ReportBalanceSeated::AssertValid() const
{
	CFormView::AssertValid();
}

void ReportBalanceSeated::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ReportBalanceSeated message handlers

//void ReportBalanceSeated::OnFilePrintPreview()
//{
//	SSPPPrintPreview(this);
//}

HBRUSH ReportBalanceSeated::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
 
   pDC->SetBkMode(TRANSPARENT);
   pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));

   return (HBRUSH)(CBrush(lDocPtr->getBalColor()));
}

void ReportBalanceSeated::OnDisplayDialogcolor() 
{
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
	lDocPtr->setReportColor();
	
	Invalidate(true);
	return;
	
}

void ReportBalanceSeated::OnUpdate(	CView* pSender,
												LPARAM lHint,
												CObject* pHint	)
{
	C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();
    Skeleton &skel = *lDocPtr->GetSkeleton();

	mHeader = lDocPtr->ReportHeader();
    mFooter = lDocPtr->ReportFooter(); 

	mResidualMomBox = "Residual Pelvic Moments (" + lDocPtr->TorqueUnits() + ")";

	const BalanceData_s& balance = skel.getBalanceData();

	// Sayan 07/13/07
	//
	// Commented out the unit conversion stuff below, as we do cleaner
	// unit conversion elsewhere

    /*double conversion = U_M_CM;
    double torqueConv = U_M_CM;
    if (lDocPtr->Is_English())
    {
        conversion = U_M_IN;
        torqueConv = U_NM_INLB;
    }

    Length   lLocX( 0, 1 );
    Length   lLocY( 0, 1 );
    Length   lLocZ( 0, 1 );*/

    //describes the stability at the seat
    //mSeatBalance = lResults.getBalanceString((int)balance.seatBal);
	if(balance.BalanceStatus == BS_Acceptable)
		mSeatBalance = "Acceptable";
	else if(balance.BalanceStatus == BS_Critical)
		mSeatBalance = "Critical";
	else
		mSeatBalance = "Unacceptable";
       
    //lSkeleton.C4EPoint( JT_SEAT_REF, lLocX, lLocY, lLocZ );
	Vector3 seatRef = skel.getSeatReference() - skel.getJoint(JT_ANKLE_CENTER);
    double seatRefX = seatRef[0];
    double seatRefY = seatRef[1];

	// Sayan 07/18/07 
	// I'm not completely sure about the "conversion" stuff below. Do they refer to
	// units conversion (in which case they would need to be changed), or about something
	// else (in which case they probably won't). For the moment, I've just taken out the 
	// "conversion" and "torqueConv" factors from all the lines below.

	mCOPseat_x.Format("%.1f", balance.CenterOfPressure[0] - seatRefX);
	mCOPseat_y.Format("%.1f", balance.CenterOfPressure[1] - seatRefY);

	m_seatFB.Format("%.1f", balance.FrontSeat);
	m_seatLB.Format("%.1f", balance.LeftSeat);
	m_seatRB.Format("%.1f", balance.RightSeat);

	mRightITMomX.Format("%.1f", balance.RightITMoment[0]);
    mRightITMomY.Format("%.1f", balance.RightITMoment[1]);
    mRightITMomZ.Format("%.1f", balance.RightITMoment[2]);

	mLeftITMomX.Format("%.1f", balance.LeftITMoment[0]);
    mLeftITMomY.Format("%.1f", balance.LeftITMoment[1]);
    mLeftITMomZ.Format("%.1f", balance.LeftITMoment[2]);

	mRightSFMomX.Format("%.1f", balance.RightFrontSeatMoment[0]);
    mRightSFMomY.Format("%.1f", balance.RightFrontSeatMoment[1]);
    mRightSFMomZ.Format("%.1f", balance.RightFrontSeatMoment[2]);

	mLeftSFMomX.Format("%.1f", balance.LeftFrontSeatMoment[0]);
    mLeftSFMomY.Format("%.1f", balance.LeftFrontSeatMoment[1]);
    mLeftSFMomZ.Format("%.1f", balance.LeftFrontSeatMoment[2]);

	mRightBallMomX.Format("%.1f", balance.RightBallMoment[0]);
    mRightBallMomY.Format("%.1f", balance.RightBallMoment[1]);
    mRightBallMomZ.Format("%.1f", balance.RightBallMoment[2]);

	mLeftBallMomX.Format("%.1f", balance.LeftBallMoment[0]);
    mLeftBallMomY.Format("%.1f", balance.LeftBallMoment[1]);
    mLeftBallMomZ.Format("%.1f", balance.LeftBallMoment[2]);

	mResMomX.Format("%.1f", balance.HipResidualMoment[0]);
	mResMomY.Format("%.1f", balance.HipResidualMoment[1]);
	mResMomZ.Format("%.1f", balance.HipResidualMoment[2]);

    mUnits4 = mUnits5 = mUnits7 = lDocPtr->getLocationUnits();

    UpdateData( FALSE );

	glBalanceSitting.onDraw();
    return;
}

#define COP_PRINT_SCALE 7
void ReportBalanceSeated::OnDraw(CDC* pDC) 
{
	glBalanceSitting.onDraw();
}
void ReportBalanceSeated::OnShowWindow(BOOL bShow, UINT nStatus)
{
	ReportView::OnShowWindow(bShow, nStatus);

	CWnd *ImageFrame = GetDlgItem(IDC_COP_GRAPHIC4);
	CRect copRect;
	ImageFrame->GetClientRect(copRect);
	// Create OpenGL drawing box
	if (COPBoxCreated == false)
	{
		mCOPDrawingRect = 
			CRect(0,0,copRect.Width(),copRect.Height());
	
	if (COPBox.Create("", WS_CHILD | SS_BITMAP | WS_VISIBLE, mCOPDrawingRect, ImageFrame))
		COPBoxCreated = true;
	} 
}

void ReportBalanceSeated::OnInitialUpdate()
{
	glBalanceSitting.initialize(&COPBox);
	glBalanceSitting.setViewport(mCOPDrawingRect);

	ReportView::OnInitialUpdate();
}
