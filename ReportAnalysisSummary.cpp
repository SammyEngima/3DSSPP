#include "ReportAnalysisSummary.h"

#include "hom_doc.h"	// Necessary

#include "../c4eLibNew/units.hpp" // for newton conversion
#include "Skeleton.hpp"

#include "Main_Frm.h"

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

#define RED RGB(200,0,0)
#define YELLOW RGB(255,255,0)
#define GREEN RGB(0,160,0)
#define BLACK RGB(0,0,0)

ReportView * ReportAnalysisSummary::cPointer = NULL;

#define L4L5_COMPRESSION_MAX 8335.0F
#define L5S1_COMPRESSION_MAX 8335.0F

IMPLEMENT_DYNCREATE(ReportAnalysisSummary, ReportView)

ReportAnalysisSummary::ReportAnalysisSummary()
:  ReportView( ReportAnalysisSummary::IDD ),
	glBalanceStanding(this),
	glBalanceSitting(this)
{
	//{{AFX_DATA_INIT(ReportAnalysisSummary)
	mAnkleValue = _T("");
	mElbowValue = _T("");
	mFooter = _T("");
	mFrictionValue = _T("");
	mHeader = _T("");
	mHipValue = _T("");
	mKneeValue = _T("");
	mShoulderValue = _T("");
	mTorsoValue = _T("");
	mLeftPercentLoadValue = _T("");
	mRightPercentLoadValue = _T("");
	mCGBalanceValue = _T("");
	mL4L5DiscValue = _T("");
    mCompressionZeroLbl = _T("");
    mCompressionSDLLbl = _T("");
    mCompressionSULLbl = _T("");
    mCompressionMaxLbl = _T("");

	mHandForceUnits = _T("");
	mUnitsLabel = _T("");
	mLeftHori = _T("");
	//}}AFX_DATA_INIT

   cPointer = this;
   COPBoxCreated = false;
   COPGraphicError = false;
   return;
}

ReportAnalysisSummary::~ReportAnalysisSummary()
{
   cPointer = NULL;
   return;
}

//---------------------------------------------------------------------------
CString ReportAnalysisSummary::ReportName(void) const
{
   return "3DSSPP - Analysis Summary Report";
}

void ReportAnalysisSummary::DoDataExchange(CDataExchange* pDX)
{
	ReportView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ReportAnalysisSummary)
	DDX_Control(pDX, IDC_L4L5DiscBar, mL4L5DiscBar);
	DDX_Control(pDX, IDC_TorsoBar, mTorsoBar);
	DDX_Control(pDX, IDC_ShoulderBar, mShoulderBar);
	DDX_Control(pDX, IDC_KneeBar, mKneeBar);
	DDX_Control(pDX, IDC_HipBar, mHipBar);
	DDX_Control(pDX, IDC_ElbowBar, mElbowBar);
	DDX_Control(pDX, IDC_WristBar, mWristBar);
	DDX_Control(pDX, IDC_AnkleBar, mAnkleBar);
	DDX_Text(pDX, IDC_AnkleValue, mAnkleValue);
	DDX_Text(pDX, IDC_ElbowValue, mElbowValue);
	DDX_Text(pDX, IDC_WristValue, mWristValue);
	DDX_Text(pDX, IDC_Footer, mFooter);
	DDX_Text(pDX, IDC_FrictionValue, mFrictionValue);
	DDX_Text(pDX, IDC_Header, mHeader);
	DDX_Text(pDX, IDC_HipValue, mHipValue);
	DDX_Text(pDX, IDC_KneeValue, mKneeValue);
	DDX_Text(pDX, IDC_ShoulderValue, mShoulderValue);
	DDX_Text(pDX, IDC_TorsoValue, mTorsoValue);
	DDX_Text(pDX, IDC_LeftPercentLoadValue, mLeftPercentLoadValue);
	DDX_Text(pDX, IDC_RightPercentLoadValue, mRightPercentLoadValue);
	DDX_Text(pDX, IDC_CGBalanceValue, mCGBalanceValue);
	DDX_Text(pDX, IDC_L4L5DiscValue, mL4L5DiscValue);
    DDX_Text(pDX, IDC_CompressionZeroLbl, mCompressionZeroLbl);
    DDX_Text(pDX, IDC_CompressionSDLLbl, mCompressionSDLLbl);
    DDX_Text(pDX, IDC_CompressionSULLbl, mCompressionSULLbl);
    DDX_Text(pDX, IDC_CompressionMaxLbl, mCompressionMaxLbl);

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


BEGIN_MESSAGE_MAP(ReportAnalysisSummary, ReportView)
	//{{AFX_MSG_MAP(ReportAnalysisSummary)
	ON_BN_CLICKED(IDC_Footer, OnFooter)
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
	//}}AFX_MSG_MAP
	// Standard printing commands
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)                       
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)

	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()

	ON_MESSAGE(WM_DRAWOPENGL, OnDrawOpenGL)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ReportAnalysisSummary diagnostics
#ifdef _DEBUG
void ReportAnalysisSummary::AssertValid() const
{
	CFormView::AssertValid();
}

void ReportAnalysisSummary::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ReportAnalysisSummary overrides
/*void ReportAnalysisSummary::OnFilePrintPreview()
{
	SSPPPrintPreview(this);
}*/

void ReportAnalysisSummary::OnUpdate(CView* aSenderPtr, LPARAM aHint, CObject* aHintPtr)
{
    C_Hom_Doc* lDocPtr = static_cast<C_Hom_Doc*>(GetDocument());
    //Analysis& lResults = lDocPtr->getResultsRef();
	Skeleton* skel = lDocPtr->GetSkeleton();

	Skeleton &lSkeleton = *lDocPtr->GetSkeleton();

    // Retrieve analysis summary results and gender-specific capability limits

	const AnalysisSummaryData_s& analysis_summary = skel->getAnalysisSummary();
	AnalysisSummaryLimits limits = skel->getAnalysisSummaryLimits();
	const BalanceData_s& balance = skel->getBalanceData();

    //Percent Capables
	mWristBar.SetRange(0, 100);
	mWristBar.SetPos(int(analysis_summary.WristMinPercentile));
	mWristValue = (char*)String_Cast(Round_Double(analysis_summary.WristMinPercentile,0),3);
	mWristBar.SetMaximum(100);
	mWristBar.SetAllRanges(0, limits.SUL, limits.SUL, limits.SDL, limits.SDL, 100);

    mElbowBar.SetRange(0, 100);
    mElbowBar.SetPos(int(analysis_summary.ElbowMinPercentile));
    mElbowValue = (char*)String_Cast(Round_Double(analysis_summary.ElbowMinPercentile,0),3);
    mElbowBar.SetMaximum(100);
    mElbowBar.SetAllRanges(0, limits.SUL, limits.SUL, limits.SDL, limits.SDL, 100);
    
    mShoulderBar.SetRange(0, 100);
    mShoulderBar.SetPos(int(analysis_summary.ShoulderMinPercentile));
    mShoulderValue = (char*)String_Cast(Round_Double(analysis_summary.ShoulderMinPercentile,0),3);
    mShoulderBar.SetMaximum(100);
    mShoulderBar.SetAllRanges(0, limits.SUL, limits.SUL, limits.SDL, limits.SDL, 100);
    
    mTorsoBar.SetRange(0, 100);
    mTorsoBar.SetPos(int(analysis_summary.TorsoMinPercentile));
    mTorsoValue = (char*)String_Cast(Round_Double(analysis_summary.TorsoMinPercentile,0),3);
    mTorsoBar.SetMaximum(100);
    mTorsoBar.SetAllRanges(0, limits.SUL, limits.SUL, limits.SDL, limits.SDL, 100);
    
    mHipBar.SetRange(0, 100);
    mHipBar.SetPos(int(analysis_summary.HipMinPercentile));
    mHipValue = (char*)String_Cast(Round_Double(analysis_summary.HipMinPercentile,0),3);
    mHipBar.SetMaximum(100);
    mHipBar.SetAllRanges(0, limits.SUL, limits.SUL, limits.SDL, limits.SDL, 100);
    
    mKneeBar.SetRange(0, 100);
    mKneeBar.SetPos(int(analysis_summary.KneeMinPercentile));
    mKneeValue = (char*)String_Cast(Round_Double(analysis_summary.KneeMinPercentile,0),3);
    mKneeBar.SetMaximum(100);
    mKneeBar.SetAllRanges(0, limits.SUL, limits.SUL, limits.SDL, limits.SDL, 100);
    
    mAnkleBar.SetRange(0, 100);
    mAnkleBar.SetPos(int(analysis_summary.AnkleMinPercentile));
    mAnkleValue = (char*)String_Cast(Round_Double(analysis_summary.AnkleMinPercentile,0),3);
    mAnkleBar.SetMaximum(100);
    mAnkleBar.SetAllRanges(0, limits.SUL, limits.SUL, limits.SDL, limits.SDL, 100);

	//--- Disc Compression (3D)---
    //Update the scale to show correct values in the current unit system
    //Force temp;
    //temp.Value(analysis_summary.L4L5Compression, MetricCns);
	if (lDocPtr->Is_Metric())
    {
        GetDlgItem(IDC_CompressionBox)->SetWindowText("3D Low back Compression (N):");
        mCompressionZeroLbl = "0";
        mCompressionSDLLbl  = "3425";
        mCompressionSULLbl  = "6361";
        mCompressionMaxLbl  = "8335";
    }
    else
    {
        GetDlgItem(IDC_CompressionBox)->SetWindowText("3D Low back Compression (lb):");
        mCompressionZeroLbl = "0";
        mCompressionSDLLbl  = "770";
        mCompressionSULLbl  = "1430";
        mCompressionMaxLbl  = "1874";
    }
	mL4L5DiscValue.Format("%.1f",analysis_summary.L4L5Compression);//temp.ValueText();

	// Get the English form of the force to test UL/
    mL4L5DiscBar.SetPos(analysis_summary.L4L5CompressionPct);
    mL4L5DiscBar.SetMaximum(100);

    //SDL == 770 lb, SUL == 1430 lb
    //(BS) In Hom_doc.cpp, mL4L5CompressionMax is defined as 8335.0N. This is the maximum
    //range of the slider, and the DL and UL ranges are set as percentages of this
    int L4L5_SDL = 770; 
    int L4L5_SUL = 1430;
    int compressionMax = L4L5_COMPRESSION_MAX*U_NEWTONS_LBF;
    mL4L5DiscBar.SetOKRange(0, (int)((L4L5_SDL*100)/compressionMax) - 1);
    mL4L5DiscBar.SetDLRange((int)((L4L5_SDL*100)/compressionMax) - 1, (int)((L4L5_SUL*100)/compressionMax) + 1);
    mL4L5DiscBar.SetULRange((int)((L4L5_SUL*100)/compressionMax) + 1, 100);

    //--- Feet ---
    GetDlgItem(IDC_FrictionValue)->EnableWindow(TRUE);
    mLeftPercentLoadValue.Format("%d%%",Round_Double(balance.LeftPctLoad));
    mRightPercentLoadValue.Format("%d%%",Round_Double(balance.RightPctLoad));
    mFrictionValue = (char*)String_Cast(Round_Double(analysis_summary.frcoef, 2), 2); 

    //mCGBalanceValue = lResults.getBalanceString((int)balance.PWBCGBal);
	if(balance.BalanceStatus == BS_Acceptable)
		mCGBalanceValue = "Acceptable";
	else if(balance.BalanceStatus == BS_Critical)
		mCGBalanceValue = "Critical";
	else
		mCGBalanceValue = "Unacceptable";
    
    //If seated, we don't want to show friction or leg loads
	if(lDocPtr->getPosition() == P_Sitting)
        mLeftPercentLoadValue = mRightPercentLoadValue = mFrictionValue = "---";

    //If the COF wasn't calculated because there are no X and Y forces or because the
    //body is out of balance, gray out the COF.
	if(analysis_summary.FrictionNotCalculated || balance.BalanceStatus == BS_Unacceptable)
    //if(lResults.COFNotCalculated || balance.BalanceStatus == BS_Unacceptable)//(lResults.PWBCGbal == 0))
    {
        mFrictionValue = "---";
        GetDlgItem(IDC_FrictionValue)->EnableWindow(FALSE);
    }   

    mHeader = lDocPtr->ReportHeader();
	mFooter = lDocPtr->ReportFooter();

	bool standing = lDocPtr->getPosition() == P_Standing;
	if(standing) {
		glBasic = &glBalanceStanding;
	} else {
		glBasic = &glBalanceSitting;
	}
	glBasic->onDraw();
	//Invalidate(false);
   
    UpdateData(FALSE);
	UpdateUnits();

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
    
    return;
}

void ReportAnalysisSummary::OnFooter() 
{
}

HBRUSH ReportAnalysisSummary::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();
 
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
    pDC->SetBkColor(lDocPtr->getBalColor());
       
    return (HBRUSH)(CBrush(lDocPtr->getBalColor()));
}

void ReportAnalysisSummary::OnDisplayDialogcolor() 
{
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
	lDocPtr->setReportColor();
	
	Invalidate(true);
	return;
}

LRESULT ReportAnalysisSummary::OnDrawOpenGL(WPARAM wParam, LPARAM lParam) {
	// draw balance graphic
	glBasic->init();
	
	// set viewing rect
	CRect oldRect = glBasic->getViewingRect();
	CWnd *ImageFrame = GetDlgItem(IDC_COP_GRAPHIC3);
	CRect rect;
	ImageFrame->GetClientRect(rect);
	CRect frameRect;
	this->GetParentFrame()->GetWindowRect(&frameRect);
	ImageFrame->ScreenToClient(&frameRect);
	rect.OffsetRect(-frameRect.left, frameRect.bottom-rect.bottom);
	glBasic->setViewport(rect);

	glBasic->onDraw((HDC)wParam, (HGLRC)lParam);

	// restore old viewing rect
	glBasic->setViewport(oldRect);
	return TRUE;
}

#define COP_PRINT_SCALE 7
void ReportAnalysisSummary::OnDraw(CDC* pDC) 
{
	C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();
	if(lDocPtr->getPosition() == P_Standing)
	{
		glBasic = &glBalanceStanding;
	} 
    else 
    {
		glBasic = &glBalanceSitting;
	}
	glBasic->onDraw();
	
}

void ReportAnalysisSummary::OnShowWindow(BOOL bShow, UINT nStatus)
{
	ReportView::OnShowWindow(bShow, nStatus);

	CWnd *ImageFrame = GetDlgItem(IDC_COP_GRAPHIC3);
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

void ReportAnalysisSummary::OnInitialUpdate()
{
	ReportView::OnInitialUpdate();

	glBalanceStanding.initialize(&COPBox);
	glBalanceStanding.setViewport(mCOPDrawingRect);
	glBalanceSitting.share(&glBalanceStanding);
	glBalanceSitting.setViewport(mCOPDrawingRect);
	glBasic = & glBalanceStanding;

	glBalanceStanding.notDrawCOG();
	glBalanceSitting.notDrawCOG();
	glBalanceStanding.notDrawSF();

	// Set the hand force box units
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();
	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";
	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}

void ReportAnalysisSummary::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class

	ReportView::OnPrint(pDC, pInfo);
}

void ReportAnalysisSummary::UpdateUnits() {
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";

	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";

	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}
