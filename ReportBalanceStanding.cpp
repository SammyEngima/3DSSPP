// ReportBalanceStanding.cpp : implementation file
//
#include "stdafx.h"
#include "ReportBalanceStanding.h"

#include "../c4eLibNew/units.hpp"
//#include "string.hpp"
//#include "winuser.h"
//#include <afxcmn.h>
//#include "afxwin.h"
#include <vector>
#include "Hom_Doc.h"
#include "Skeleton.hpp"
#include "Types.h"

using std::vector;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
static CString bothFtSupported[6] = { "Right Front", "Right Middle",
									 "Right Back", "Left Front", 
									 "Left Middle", "Left Back"};
static CString singleFtSupported[4] = { "Front", "Middle", "Back Left", "Back Right" };
/////////////////////////////////////////////////////////////////////////////
// ReportBalanceStanding

ReportView * ReportBalanceStanding::cPointer = NULL;
IMPLEMENT_DYNCREATE(ReportBalanceStanding, ReportView)

ReportBalanceStanding::ReportBalanceStanding()
	: ReportView(ReportBalanceStanding::IDD)
	, LeftZPercent(_T(""))
	, RightZPercent(_T(""))
	, mCenOfGravX(_T(""))
	, mCenOfGravY(_T(""))
	, COPgraphic(this)
	, mCOGLabel(_T(""))
	, mMBMLabel(_T(""))
	, mCOPLabel(_T(""))
	, mDTBLabel(_T(""))
	, mBCLabel(_T(""))
{
	//{{AFX_DATA_INIT(ReportBalanceStanding)
	mFooter = _T("");
	mHeader = _T("");
	mCGBalance = _T("");
	mDistToBack = _T("");
	mDistToFront = _T("");
	mDistToLeft = _T("");
	mDistToRight = _T("");
	mPWBCGy = _T("");
	mPWBCGx = _T("");
	mLOLM = _T("");
	mLSLB = _T("");
	mLSLF = _T("");
	mRORM = _T("");
	mRSRB = _T("");
	mRSRF = _T("");
	mRightFrontText		= bothFtSupported[0];
	mRightMiddleText	= bothFtSupported[1];
	mRightBackText		= bothFtSupported[2];
	mLeftFrontText		= bothFtSupported[3];
	mLeftMiddleText		= bothFtSupported[4];
	mLeftBackText		= bothFtSupported[5];
    mResMomX = _T("");
    mResMomY = _T("");
    mResMomZ = _T("");
	//}}AFX_DATA_INIT

   cPointer = this;
   COPBoxCreated = false;
   COPGraphicError = false;
   return;
}

ReportBalanceStanding::~ReportBalanceStanding()
{
   cPointer = NULL;
   return;
}
//---------------------------------------------------------------------------
CString ReportBalanceStanding::ReportName(void) const {
   return   "3DSSPP - Balance Report - Standing";
}

void ReportBalanceStanding::DoDataExchange(CDataExchange* pDX)
{
	ReportView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ReportBalanceStanding)
	DDX_Text(pDX, IDC_Footer, mFooter);
	DDX_Text(pDX, IDC_Header, mHeader);
	DDX_Text(pDX, IDC_CGBalanceValue, mCGBalance);
	DDX_Text(pDX, IDC_DIST_BACK, mDistToBack);
	DDX_Text(pDX, IDC_DIST_FRONT, mDistToFront);
	DDX_Text(pDX, IDC_DIST_LEFT, mDistToLeft);
	DDX_Text(pDX, IDC_DIST_RIGHT, mDistToRight);
	DDX_Text(pDX, IDC_PWBCG_LR, mPWBCGx);
	DDX_Text(pDX, IDC_PWBCG_FB, mPWBCGy);
	DDX_Text(pDX, IDC_LOLM, mLOLM);
	DDX_Text(pDX, IDC_LSLB, mLSLB);
	DDX_Text(pDX, IDC_LSLF, mLSLF);
	DDX_Text(pDX, IDC_RORM, mRORM);
	DDX_Text(pDX, IDC_RSRB, mRSRB);
	DDX_Text(pDX, IDC_RSRF, mRSRF);
	DDX_Text(pDX, IDC_RightFrontText, mRightFrontText);
	DDX_Text(pDX, IDC_RightMiddleText, mRightMiddleText);
	DDX_Text(pDX, IDC_RightBackText, mRightBackText);
	DDX_Text(pDX, IDC_LeftFrontText, mLeftFrontText);
	DDX_Text(pDX, IDC_LeftMiddleText, mLeftMiddleText);
	DDX_Text(pDX, IDC_LeftBackText, mLeftBackText);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_LeftZPercent, LeftZPercent);
	DDX_Text(pDX, IDC_RightZPercent, RightZPercent);

	DDX_Text(pDX, IDC_RightHeelMomX, mRightHeelMomX);
	DDX_Text(pDX, IDC_RightHeelMomY, mRightHeelMomY);
	DDX_Text(pDX, IDC_RightHeelMomZ, mRightHeelMomZ);
	DDX_Text(pDX, IDC_RightBallMomX, mRightBallMomX);
	DDX_Text(pDX, IDC_RightBallMomY, mRightBallMomY);
	DDX_Text(pDX, IDC_RightBallMomZ, mRightBallMomZ);
	DDX_Text(pDX, IDC_LeftHeelMomX, mLeftHeelMomX);
	DDX_Text(pDX, IDC_LeftHeelMomY, mLeftHeelMomY);
	DDX_Text(pDX, IDC_LeftHeelMomZ, mLeftHeelMomZ);
	DDX_Text(pDX, IDC_LeftBallMomX, mLeftBallMomX);
	DDX_Text(pDX, IDC_LeftBallMomY, mLeftBallMomY);
	DDX_Text(pDX, IDC_LeftBallMomZ, mLeftBallMomZ);
	DDX_Text(pDX, IDC_ResidualMomBox, mResidualMomBox);
	DDX_Text(pDX, IDC_ResMomX, mResMomX);
	DDX_Text(pDX, IDC_ResMomY, mResMomY);
	DDX_Text(pDX, IDC_ResMomZ, mResMomZ);
	DDX_Text(pDX, IDC_CENOFGRAVX, mCenOfGravX);
	DDX_Text(pDX, IDC_CENOFGRAVY, mCenOfGravY);
	DDX_Text(pDX, IDC_COGLABEL, mCOGLabel);
	DDX_Text(pDX, IDC_MBM_LABEL, mMBMLabel);
	DDX_Text(pDX, IDC_COP_LABEL, mCOPLabel);
	DDX_Text(pDX, IDC_DTB_LABEL, mDTBLabel);
	DDX_Text(pDX, IDC_BC_LABEL, mBCLabel);
}

BEGIN_MESSAGE_MAP(ReportBalanceStanding, ReportView)
	//{{AFX_MSG_MAP(ReportBalanceStanding)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
	//}}AFX_MSG_MAP
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ReportBalanceStanding diagnostics

#ifdef _DEBUG
void ReportBalanceStanding::AssertValid() const
{
	CFormView::AssertValid();
}

void ReportBalanceStanding::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// ReportPosture overrides

void ReportBalanceStanding::OnUpdate
( 
   CView *   aSenderPtr, 
   LPARAM    aHint, 
   CObject * aHintPtr
)
{
    C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();

    //Analysis &lResults = lDocPtr->getResultsRef();
    //BalanceData balance;
    //BalanceMomentData moments;
    //lResults.getBalanceData(&balance);
    //lResults.getBalanceMoments(&moments);
	const BalanceData_s& balance = lDocPtr->GetSkeleton()->getBalanceData();

    double conversion = U_M_CM;
    double torqueConv = U_M_CM;
    if (lDocPtr->Is_English())
    {
        conversion = U_M_IN;
        torqueConv = U_NM_INLB;
    }

    mHeader = lDocPtr->ReportHeader();
    mFooter = lDocPtr->ReportFooter();
//    Torque temp(0, 1);
    mResidualMomBox = "Residual Support Moments (" + lDocPtr->TorqueUnits() + ")";
	mCOGLabel = "Center of Gravity Coordinates (" + lDocPtr->LengthUnits() + ")";
	mBCLabel = "Balance Coordinates (x,y) (" + lDocPtr->LengthUnits() + ")";
	mDTBLabel = "Distance to Boundaries (" + lDocPtr->LengthUnits() + ")";
	mCOPLabel = "Center of Pressure Coordinates (" + lDocPtr->LengthUnits() + ")";
	mMBMLabel = "Maximum Balance Moments (" + lDocPtr->TorqueUnits() + ")";

	//mCGBalance = lResults.getBalanceString((int)balance.PWBCGBal);
	if(balance.BalanceStatus == BS_Acceptable)
		mCGBalance = "Acceptable";
	else if(balance.BalanceStatus == BS_Critical)
		mCGBalance = "Critical";
	else
		mCGBalance = "Unacceptable";

	if (balance.DistToBack < 0)
        mDistToBack = "outside";
	else
		//mDistToBack.Format("%0.1f",balance.PWBCGBal_DistToBack*conversion);
		mDistToBack.Format("%0.1f", balance.DistToBack);
	if (balance.DistToFront < 0)
        mDistToFront = "outside";
	else
		//mDistToFront.Format("%0.1f",balance.PWBCGBal_DistToFront*conversion);
		mDistToFront.Format("%0.1f", balance.DistToFront);
	if (balance.DistToLeft < 0)
        mDistToLeft = "outside";
	else
		//mDistToLeft.Format("%0.1f",balance.PWBCGBal_DistToLeft*conversion);
		mDistToLeft.Format("%0.1f", balance.DistToLeft);
	if (balance.DistToRight < 0)
        mDistToRight = "outside";
	else
		//mDistToRight.Format("%0.1f",balance.PWBCGBal_DistToRight*conversion);
		mDistToRight.Format("%0.1f", balance.DistToRight);

    //mPWBCGx.Format("%.1f", balance.PWBCG_x*conversion);
	mPWBCGx.Format("%.1f", balance.CenterOfPressure[0]);
    //mPWBCGy.Format("%.1f", balance.PWBCG_y*conversion);
	mPWBCGy.Format("%.1f", balance.CenterOfPressure[1]);

  	//if (lDocPtr->getStance() == STANCE_BOTH_FEET)
	// these should not be set on every update, only when it changes
	if(lDocPtr->getFeetSupport() == FS_Both)
	{
		mRightFrontText		= bothFtSupported[0];
		mRightMiddleText	= bothFtSupported[1];
		mRightBackText		= bothFtSupported[2];
		mLeftFrontText		= bothFtSupported[3];
		mLeftMiddleText		= bothFtSupported[4];
		mLeftBackText		= bothFtSupported[5];
	} 
    else 
    {
		mRightFrontText		= singleFtSupported[0];
		mRightMiddleText	= singleFtSupported[1];
		mRightBackText		= singleFtSupported[2];
		mLeftFrontText		= singleFtSupported[3];
		mLeftMiddleText		= _T("");
		mLeftBackText		= _T("");
	}

    mLSLF.Format("(%.1f,%.1f)", balance.LeftFront[0], balance.LeftFront[1]);
    mLOLM.Format("(%.1f,%.1f)", balance.LeftMiddle[0], balance.LeftMiddle[1]);
    mLSLB.Format("(%.1f,%.1f)", balance.LeftBack[0], balance.LeftBack[1]);
    mRSRB.Format("(%.1f,%.1f)", balance.RightBack[0], balance.RightBack[1]);
    mRORM.Format("(%.1f,%.1f)", balance.RightMiddle[0], balance.RightMiddle[1]);
    mRSRF.Format("(%.1f,%.1f)", balance.RightFront[0], balance.RightFront[1]);
    //if (lDocPtr->getStance() != STANCE_BOTH_FEET)
	if(lDocPtr->getFeetSupport() != FS_Both)
    {
        mLOLM = "";
        mLSLB = "";
    }

    LeftZPercent.Format("%d%%",(int)balance.LeftPctLoad);
    RightZPercent.Format("%d%%",(int)balance.RightPctLoad);

    mRightBallMomX.Format("%0.1f",balance.RightBallMoment[0]);
    mRightBallMomY.Format("%0.1f",balance.RightBallMoment[1]);
    mRightBallMomZ.Format("%0.1f",balance.RightBallMoment[2]);

    mLeftBallMomX.Format("%0.1f",balance.LeftBallMoment[0]);
    mLeftBallMomY.Format("%0.1f",balance.LeftBallMoment[1]);
    mLeftBallMomZ.Format("%0.1f",balance.LeftBallMoment[2]);

    mRightHeelMomX.Format("%0.1f",balance.RightHeelMoment[0]);
    mRightHeelMomY.Format("%0.1f",balance.RightHeelMoment[1]);
    mRightHeelMomZ.Format("%0.1f",balance.RightHeelMoment[2]);

    mLeftHeelMomX.Format("%0.1f",balance.LeftHeelMoment[0]);
    mLeftHeelMomY.Format("%0.1f",balance.LeftHeelMoment[1]);
    mLeftHeelMomZ.Format("%0.1f",balance.LeftHeelMoment[2]);

	mResMomX.Format("%0.1f",balance.HipResidualMoment[0]);
	mResMomY.Format("%0.1f",balance.HipResidualMoment[1]);
	mResMomZ.Format("%0.1f",balance.HipResidualMoment[2]);

	mCenOfGravX.Format("%.1f", balance.CenterOfGravity[0]);
	mCenOfGravY.Format("%.1f", balance.CenterOfGravity[1]);

    UpdateData(false);

	COPgraphic.onDraw();

	return;
}

/////////////////////////////////////////////////////////////////////////////
// ReportBalanceStanding message handlers

//void ReportBalanceStanding::OnFilePrintPreview()
//{
//	SSPPPrintPreview(this);
//}
#define COP_PRINT_SCALE 7 // The amount the COP graphic gets scaled for printing
void ReportBalanceStanding::OnDraw(CDC* pDC) 
{
	COPgraphic.onDraw();
}

HBRUSH ReportBalanceStanding::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
   C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
 
   pDC->SetBkMode(TRANSPARENT);
   pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));

   return (HBRUSH)(CBrush(lDocPtr->getBalColor()));
}


void ReportBalanceStanding::OnDisplayDialogcolor() 
{
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
	lDocPtr->setReportColor();
	
	Invalidate(true);
	return;	
}

void ReportBalanceStanding::OnShowWindow(BOOL bShow, UINT nStatus)
{
	ReportView::OnShowWindow(bShow, nStatus);

	CWnd *ImageFrame = GetDlgItem(IDC_COP_GRAPHIC2);
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

void ReportBalanceStanding::OnInitialUpdate()
{
	ReportView::OnInitialUpdate();

	COPgraphic.initialize(&COPBox);
	COPgraphic.setViewport(mCOPDrawingRect);
}
