// StatusBox.cpp : implementation file
//

// Note: Status box size 332x183 is optimized for 1024x768 maximized screen
//   res with taskbar showing.

#include "StatusBox.h"
#include "GLView.h"
#include "GLBalanceStanding.h"
//#include "GLOblique.h" // TODO for debugging, take out

#include "hom.h"
#include "hom_doc.h"
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
#define WHITE RGB(255,255,255)
#define COP_WIDTH 130
#define COP_HEIGHT 165

#define COP_BORDER_WIDTH 2
#define L4L5_COMPRESSION_MAX 8335.0F
#define L5S1_COMPRESSION_MAX 8335.0F

/////////////////////////////////////////////////////////////////////////////
// StatusBox

IMPLEMENT_DYNCREATE(StatusBox, ReportView)

StatusBox::StatusBox() : ReportView( StatusBox::IDD, FALSE )
   , mFooter(_T(""))
   , m3DLowBackCompressionUnits(_T(""))
   , glBalanceStanding(this)
   , glBalanceSitting(this)
   , shouldEditVal(_T(""))
   , m_WristDuration(_T(""))
   , m_ElbowDuration(_T(""))
   , m_ShoulderDuration(_T(""))
   , minWristDC(_T(""))
   , minElbowDC(_T(""))
   , minShoulderDC(_T(""))
   , m_CycleTime(0)
   , m_Exertion(0)
   , m_ExertionDuration(0)
   , m_ExertionDutyCycle(_T(""))
   , EDC_Calc(0)
   , flag(0)
{
   //{{AFX_DATA_INIT(StatusBox)
   mAnkle = _T("");
   mElbow = _T("");
   mFriction = _T("");
   mHip = _T("");
   mKnee = _T("");
   mShoulder = _T("");
   mTorso = _T("");
   mConditions = _T("");
   mCGBalance = _T("");
   mUnitsLabel = _T("");
   mRightHori = _T("");
   mRightLat = _T("");
   mRightVert = _T("");
   mLeftHori = _T("");
   mLeftLat = _T("");
   mLeftVert = _T("");  
   mGravityText = _T("");
   mGravityValue = _T("");
   mHandForceUnits = _T("");

   WristLeft = _T("");
   WristRight = _T("");
   ElbowLeft = _T("");
   ElbowRight = _T("");
   ShoulderLeft = _T("");
   ShoulderRight = _T("");
   HipLeft = _T("");
   HipRight = _T("");
   KneeLeft = _T("");
   KneeRight = _T("");
   AnkleLeft = _T("");
   AnkleRight = _T("");
   //Torso = _T("");

   //}}AFX_DATA_INIT

   COPBoxCreated = false;
}

StatusBox::~StatusBox()
{
	// Destroy the openGL context	
 //   COPgraphic.destroyGLContext("Status Box");
}

void StatusBox::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(StatusBox)
	DDX_Text(pDX, IDC_AnkleValue, mAnkle);
	DDX_Text(pDX, IDC_WristValue, mWrist);
	DDX_Text(pDX, IDC_ElbowValue, mElbow);
	DDX_Text(pDX, IDC_FrictionValue, mFriction);
	DDX_Text(pDX, IDC_HipValue, mHip);
	DDX_Text(pDX, IDC_KneeValue, mKnee);
	DDX_Text(pDX, IDC_ShoulderValue, mShoulder);
	DDX_Text(pDX, IDC_TorsoValue, mTorso);

	DDX_Text(pDX, IDC_Conditions, mConditions);
	DDX_Text(pDX, IDC_CGBalanceValue, mCGBalance);
	DDX_Text(pDX, IDC_UnitsLabel, mUnitsLabel);
	DDX_Text(pDX, IDC_HAND_FORCE_TXT_LVAL, mHandForceTxtLVal);
	DDX_Text(pDX, IDC_HAND_FORCE_TXT_RVAL, mHandForceTxtRVal);
	DDX_Text(pDX, IDC_RightHori, mRightHori);
	DDX_Text(pDX, IDC_RightLat2, mRightLat);
	DDX_Text(pDX, IDC_RightVert2, mRightVert);
	DDX_Text(pDX, IDC_LeftHori, mLeftHori);
	DDX_Text(pDX, IDC_LeftLat2, mLeftLat);
	DDX_Text(pDX, IDC_LeftVert2, mLeftVert);
	DDX_Text(pDX, IDC_GRAVITY_TEXT, mGravityText);
	DDX_Text(pDX, IDC_GRAVITY_VALUE, mGravityValue);
	DDX_Text(pDX, IDC_HandForceUnits, mHandForceUnits);


	DDX_Text(pDX, IDC_L4L5DiscValue, mL4L5DiscValue);

	DDX_Control(pDX, IDC_L4L5DiscBar, mL4L5DiscBar);
	DDX_Control(pDX, IDC_TorsoBar, mTorsoBar);
	DDX_Control(pDX, IDC_ShoulderBar, mShoulderBar);
	DDX_Control(pDX, IDC_KneeBar, mKneeBar);
	DDX_Control(pDX, IDC_HipBar, mHipBar);
	DDX_Control(pDX, IDC_WristBar, mWristBar);
	DDX_Control(pDX, IDC_ElbowBar, mElbowBar);
	DDX_Control(pDX, IDC_AnkleBar, mAnkleBar);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_StatusBoxCopyrightString, mFooter);
	DDX_Text(pDX, IDC_CompressionBox, m3DLowBackCompressionUnits);
	DDX_Text(pDX, IDC_SHOULDEDIT, shouldEditVal);
	DDX_Text(pDX, IDC_WristDuration, m_WristDuration);
	DDX_Text(pDX, IDC_ElbowDuration, m_ElbowDuration);
	DDX_Text(pDX, IDC_ShoulderDuration, m_ShoulderDuration);
	DDX_Text(pDX, IDC_WristMDC, minWristDC);
	DDX_Text(pDX, IDC_ElbowMDC, minElbowDC);
	DDX_Text(pDX, IDC_ShoulderMDC, minShoulderDC);
	DDX_Text(pDX, IDC_LeftWristFlex9, HipLeft);
	DDX_Text(pDX, IDC_LeftWristFlex10, KneeLeft);
	DDX_Text(pDX, IDC_LeftWristFlex11, AnkleLeft);
	//DDX_Text(pDX, IDC_LeftWristFlex18, Torso);
	DDX_Text(pDX, IDC_CycleTime, m_CycleTime);
	DDV_MinMaxUInt(pDX, m_CycleTime, 0, 100000);
	DDX_Text(pDX, IDC_Exertion, m_Exertion);
	DDV_MinMaxUInt(pDX, m_Exertion, 0, 100000);
	DDX_Text(pDX, IDC_ED, m_ExertionDuration);
	DDV_MinMaxUInt(pDX, m_ExertionDuration, 0, 100000);
	DDX_Text(pDX, IDC_ExertionDutyCycle, m_ExertionDutyCycle);
}

BEGIN_MESSAGE_MAP(StatusBox, CFormView)
	//{{AFX_MSG_MAP(StatusBox)
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_DISPLAY_DIALOGCOLOR, OnUpdateDisplayDialogcolor)
	ON_COMMAND(ID_FILE_PRINT, ReportView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
    ON_WM_SHOWWINDOW()
    //}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_EN_CHANGE(IDC_SHOULDEDIT, &StatusBox::OnEnChangeShouldedit)
	ON_COMMAND(ID_FILE_COPYACTIVEWINDOW, &StatusBox::OnFileCopyactivewindow)
	ON_MESSAGE(WM_DRAWOPENGL, OnDrawOpenGL)
	//ON_BN_CLICKED(IDC_Reset, &StatusBox::OnBnClickedReset)
	//ON_EN_CHANGE(IDC_CycleTime, &StatusBox::OnEnChangeCycletime)
	//ON_EN_CHANGE(IDC_Exertion, &StatusBox::OnEnChangeExertion)
	//ON_EN_CHANGE(IDC_ED, &StatusBox::OnEnChangeEd)
	ON_BN_CLICKED(IDC_Generate, &StatusBox::OnBnClickedGenerate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// StatusBox diagnostics

#ifdef _DEBUG
void StatusBox::AssertValid() const
{
   CFormView::AssertValid();
}

void StatusBox::Dump(CDumpContext& dc) const
{
   CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// StatusBox overrides

void StatusBox::OnFilePrintPreview()
{
	// TODO printing
   //SSPPPrintPreview(this);
	ReportView::OnFilePrintPreview();
}

LRESULT StatusBox::OnDrawOpenGL(WPARAM wParam, LPARAM lParam) {
	// draw balance graphic
	glBasic->init();
	
	// set viewing rect
	CRect oldRect = glBasic->getViewingRect();
	CWnd *ImageFrame = GetDlgItem(IDC_COP_GRAPHIC);
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
void StatusBox::OnInitialUpdate() {
	glBalanceStanding.initialize(&COPBox);
	glBalanceStanding.setViewport(mCOPDrawingRect);
	glBalanceSitting.share(&glBalanceStanding);
	glBalanceSitting.setViewport(mCOPDrawingRect);

	glBasic = &glBalanceStanding;

	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";

	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";

	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";

	//((C_Hom_Doc*)GetDocument())->getResultsRef().getAnalysisSummaryLimits(&limits);
	AnalysisSummaryLimits limits = ((C_Hom_Doc*)GetDocument())->GetSkeleton()->getAnalysisSummaryLimits();

	mWristBar.SubclassDlgItem(IDC_WristBar, this);
	mElbowBar.SubclassDlgItem(IDC_ElbowBar, this);
	mShoulderBar.SubclassDlgItem(IDC_ShoulderBar, this);
	mTorsoBar.SubclassDlgItem(IDC_TorsoBar, this);
	mHipBar.SubclassDlgItem(IDC_HipBar, this);
	mKneeBar.SubclassDlgItem(IDC_KneeBar, this);
	mAnkleBar.SubclassDlgItem(IDC_AnkleBar, this);
	mL4L5DiscBar.SubclassDlgItem(IDC_L4L5DiscBar, this);

	mWristBar.SetRange(0, 100);
	mWristBar.SetMaximum(100);

	mElbowBar.SetRange(0, 100);
	mElbowBar.SetMaximum(100);

	mShoulderBar.SetRange(0, 100);
	mShoulderBar.SetMaximum(100);

	mTorsoBar.SetRange(0, 100);
	mTorsoBar.SetMaximum(100);

	mHipBar.SetRange(0, 100);
	mHipBar.SetMaximum(100);

	mKneeBar.SetRange(0, 100);
	mKneeBar.SetMaximum(100);

	mAnkleBar.SetRange(0, 100);
	mAnkleBar.SetMaximum(100);

	UpdatePercentileLimits();

	mL4L5DiscBar.SetMaximum(100);

	int L4L5_SDL = 770; 
    int L4L5_SUL = 1430;
    int compressionMax = L4L5_COMPRESSION_MAX*U_NEWTONS_LBF;
    mL4L5DiscBar.SetOKRange(0, (int)((L4L5_SDL*100)/compressionMax) - 1);
    mL4L5DiscBar.SetDLRange((int)((L4L5_SDL*100)/compressionMax) - 1, (int)((L4L5_SUL*100)/compressionMax) + 1);
    mL4L5DiscBar.SetULRange((int)((L4L5_SUL*100)/compressionMax) + 1, 100);

	balanceStatus = (BalanceStatus_e)(-1);

	// Set the copyright footer
    mFooter = ((C_Hom_Doc*)GetDocument())->ReportFooter();

	shouldEditVal.Format("%.2f",GLOblique::shouldDisp);

	UpdateData(false);

	ReportView::OnInitialUpdate();
}
void StatusBox::OnUpdate(CView* aSenderPtr, LPARAM aHint, CObject* aHintPtr)
{
    C_Hom_Doc* lDocPtr = (C_Hom_Doc*)GetDocument();
    Skeleton &lSkeleton = *lDocPtr->GetSkeleton();

    // Retrieve analysis results
	const AnalysisSummaryData_s& analysis_summary = lSkeleton.getAnalysisSummary();
	const BalanceData_s& balance = lSkeleton.getBalanceData();

	AnalysisSummaryLimits limits = lSkeleton.getAnalysisSummaryLimits();

    //Percent Capables
	mWristBar.SetPos(int(analysis_summary.WristMinPercentile));
	mWrist.Format("%d", Round_Double(analysis_summary.WristMinPercentile));

    mElbowBar.SetPos(int(analysis_summary.ElbowMinPercentile));
	mElbow.Format("%d", Round_Double(analysis_summary.ElbowMinPercentile));
    
    mShoulderBar.SetPos(int(analysis_summary.ShoulderMinPercentile));
	mShoulder.Format("%d", Round_Double(analysis_summary.ShoulderMinPercentile));
    
    mTorsoBar.SetPos(int(analysis_summary.TorsoMinPercentile));
	mTorso.Format("%d", Round_Double(analysis_summary.TorsoMinPercentile));
    
    mHipBar.SetPos(int(analysis_summary.HipMinPercentile));
	mHip.Format("%d", Round_Double(analysis_summary.HipMinPercentile));
    
    mKneeBar.SetPos(int(analysis_summary.KneeMinPercentile));
	mKnee.Format("%d", Round_Double(analysis_summary.KneeMinPercentile));
    
    mAnkleBar.SetPos(int(analysis_summary.AnkleMinPercentile));
	mAnkle.Format("%d", Round_Double(analysis_summary.AnkleMinPercentile));

    //--- Disc Compression (3D)---
    //Update the box to show correct unit system
	mL4L5DiscValue.Format("%.0f", analysis_summary.L4L5Compression);

    // Get the English form of the force to test upper/lower limits for back
    mL4L5DiscBar.SetPos(analysis_summary.L4L5CompressionPct);

	if(balance.BalanceStatus != balanceStatus) {
		mCGBalance = "Balance: ";
		if(balance.BalanceStatus == BS_Acceptable)
			mCGBalance += "Acceptable";
		else if(balance.BalanceStatus == BS_Critical)
			mCGBalance += "Critical";
		else
			mCGBalance += "Unacceptable";
		balanceStatus = balance.BalanceStatus;
	}

    //Show the COF if it's not in seated mode and the COF was calculated
	if(lDocPtr->getPosition() == P_Standing &&
		!analysis_summary.FrictionNotCalculated &&
		balance.BalanceStatus != BS_Unacceptable)
    {   
        GetDlgItem(IDC_FrictionValue)->EnableWindow(TRUE);
        mFriction = DblToCStr(analysis_summary.frcoef, 4, 3 );
    }
    else
    {
        GetDlgItem(IDC_FrictionValue)->EnableWindow(FALSE);
        mFriction = "---";
    }
    // Set the Task Summary box text
    mConditions = lDocPtr->TaskStatusText();

	Vector3 offsetCoordinate = lDocPtr->getOffset();

	Vector3 LHand = lSkeleton.getJoint(JT_LHAND);

    mLeftHori.Format("%.1f", (LHand[1] -  offsetCoordinate[1]));
	mLeftLat.Format("%.1f", (LHand[0] - offsetCoordinate[0]));
	mLeftVert.Format("%.1f", (LHand[2] - offsetCoordinate[2]));


	Vector3 RHand = lSkeleton.getJoint(JT_RHAND);

	mRightHori.Format("%.1f", (RHand[1]  - offsetCoordinate[1]));
	mRightLat.Format("%.1f", (RHand[0] - offsetCoordinate[0]));
	mRightVert.Format("%.1f", (RHand[2] - offsetCoordinate[2]));
	const FatigueData_s& data = lDocPtr->GetSkeleton()->getFatigueData();
	//WristLeft.Format("%.0f",max(max(data.FifthPercentileDC[0],data.FifthPercentileDC[3]),max(data.FifthPercentileDC[3],data.FifthPercentileDC[6])));
	WristLeft.Format("%.1f",min(min(data.FiftiethPercentileDC[0],data.FiftiethPercentileDC[1]),min(data.FiftiethPercentileDC[0],data.FiftiethPercentileDC[2])));
	ElbowLeft.Format("%.1f",data.FiftiethPercentileDC[3]);
	ShoulderLeft.Format("%.1f",min(min(data.FiftiethPercentileDC[4],data.FiftiethPercentileDC[5]),min(data.FiftiethPercentileDC[4],data.FiftiethPercentileDC[6])));
	//Torso.Format("%.0f",min(min(data.FiftiethPercentileDC[20],data.FiftiethPercentileDC[21]),min(data.FiftiethPercentileDC[20],data.FiftiethPercentileDC[22])));
	HipLeft.Format("%.0f",min(data.FiftiethPercentileDC[7],data.FiftiethPercentileDC[17]));
	KneeLeft.Format("%.0f",min(data.FiftiethPercentileDC[8],data.FiftiethPercentileDC[18]));
	AnkleLeft.Format("%.0f",min(data.FiftiethPercentileDC[9],data.FiftiethPercentileDC[19]));


	WristRight.Format("%.1f",min(min(data.FiftiethPercentileDC[10],data.FiftiethPercentileDC[11]),min(data.FiftiethPercentileDC[10],data.FiftiethPercentileDC[12])));
	ElbowRight.Format("%.1f",data.FiftiethPercentileDC[13]);
	ShoulderRight.Format("%.1f",min(min(data.FiftiethPercentileDC[14],data.FiftiethPercentileDC[15]),min(data.FiftiethPercentileDC[14],data.FiftiethPercentileDC[16])));
	
	HipRight.Format("%.1f",data.FiftiethPercentileDC[17]);
	KneeRight.Format("%.1f",data.FiftiethPercentileDC[18]);
	AnkleRight.Format("%.1f",data.FiftiethPercentileDC[19]);

	minElbowDC.Format("%.0f",min(data.FiftiethPercentileDC[3],data.FiftiethPercentileDC[13]));
	minWristDC.Format("%.0f",min(atof(WristLeft),atof(WristRight)));
	minShoulderDC.Format("%.0f",min(atof(ShoulderLeft),atof(ShoulderRight)));
	auto a = min(data.FiftiethPercentileMT[0],data.FiftiethPercentileMT[1]);
	auto b = min(a,data.FiftiethPercentileMT[2]);
	auto c = min(b,data.FiftiethPercentileMT[10]);
	auto d = min(c,data.FiftiethPercentileMT[11]);
	auto e = min(d,data.FiftiethPercentileMT[12]);
	m_WristDuration.Format("%.0f",e);

	auto aa = min(data.FiftiethPercentileMT[4],data.FiftiethPercentileMT[5]);
	auto bb = min(aa,data.FiftiethPercentileMT[6]);
	auto cc = min(bb,data.FiftiethPercentileMT[14]);
	auto dd = min(cc,data.FiftiethPercentileMT[15]);
	auto ee = min(dd,data.FiftiethPercentileMT[16]);
	m_ShoulderDuration.Format("%.0f",ee);

	m_ElbowDuration.Format("%.0f",min(data.FiftiethPercentileMT[3],data.FiftiethPercentileMT[13]));

    mHandForceTxtLVal.Format("%.1f", lSkeleton.getExtForce(JT_LHAND).length());
	mHandForceTxtRVal.Format("%.1f", lSkeleton.getExtForce(JT_RHAND).length());

	bool standing = lDocPtr->getPosition() == P_Standing;
	if(standing) {
		glBasic = &glBalanceStanding;
	} else {
		glBasic = &glBalanceSitting;
	}
    glBasic->notDrawCOG();
	glBasic->notDrawSF(); // does not show the support forces
	glBasic->onDraw();

    UpdateData(FALSE);
}

HBRUSH StatusBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
   HBRUSH hbr = ReportView::OnCtlColor(pDC, pWnd, nCtlColor);
   C_Hom_Doc* lDocPtr = static_cast <C_Hom_Doc*> (GetDocument());
   pDC->SetBkMode(TRANSPARENT);
   pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
   pDC->SetBkColor(lDocPtr->getBalColor());
   int ID = pWnd -> GetDlgCtrlID();

   if (ID == IDC_ExertionDutyCycle) {
	   if (m_ExertionDuration == 0) pDC->SetTextColor(WHITE);
	   else pDC->SetTextColor(RGB(0, 0, 0));
	   pDC->SetBkMode(OPAQUE);
	   pDC->SetBkColor(WHITE);
	   hbr = CreateSolidBrush(WHITE);
	   return hbr;
   }

   if (flag == 1)
   {
   if (ID == IDC_WristMDC)
   {
	   switch (nCtlColor){
		 case CTLCOLOR_STATIC:
	   {
		double result =GetDlgItemInt(ID);
		UpdateData(false);
		if(EDC_Calc != 0 &&  EDC_Calc > 1.00 * result)
		{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(255, 0, 0));// Set color to Red
			hbr = CreateSolidBrush(RGB(255, 0, 0));
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		else if(EDC_Calc != 0 && 0.95 * result >  EDC_Calc)
		{
			pDC->SetTextColor(RGB(0, 0, 0));// Set color to green
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(0, 255, 0));
			hbr = CreateSolidBrush(RGB(0, 255, 0));
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		else if(EDC_Calc != 0 && 0.95 * result < EDC_Calc && 1.00 * result >= EDC_Calc)
		{
			pDC->SetTextColor(RGB(0, 0, 0));// Set color to yellow
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(255, 255, 0));
			hbr = CreateSolidBrush(RGB(255, 255, 0));
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		else{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(WHITE);
			hbr = CreateSolidBrush(WHITE);
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH);
		}
	   }
		 default:	return ReportView::OnCtlColor(pDC, pWnd, nCtlColor);
		 }
	   }
    if (ID == IDC_ElbowMDC)
   {
	   switch (nCtlColor){
		 case CTLCOLOR_STATIC:
	   {
		double result =GetDlgItemInt(ID);
		UpdateData(false);
		if(EDC_Calc != 0 &&  EDC_Calc > 1.00 * result)
		{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(255, 0, 0));// Set color to Red
			hbr = CreateSolidBrush(RGB(255, 0, 0));
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		else if(EDC_Calc != 0 && 0.95 * result >  EDC_Calc)
		{
			pDC->SetTextColor(RGB(0, 0, 0));// Set color to green
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(0, 255, 0));
			hbr = CreateSolidBrush(RGB(0, 255, 0));
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		else if(EDC_Calc != 0 && 0.95 * result < EDC_Calc && 1.00 * result >= EDC_Calc)
		{
			pDC->SetTextColor(RGB(0, 0, 0));// Set color to yellow
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(255, 255, 0));
			hbr = CreateSolidBrush(RGB(255, 255, 0));
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		else{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(WHITE);
			hbr = CreateSolidBrush(WHITE);
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH);
		}
	   }
		 default:	return ReportView::OnCtlColor(pDC, pWnd, nCtlColor);
		 }
	   }
	 if (ID == IDC_ShoulderMDC)
   {
	   switch (nCtlColor){
		 case CTLCOLOR_STATIC:
	   {
		double result =GetDlgItemInt(ID);
		UpdateData(false);
		if(EDC_Calc != 0 &&  EDC_Calc > 1.00 * result)
		{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(255, 0, 0));// Set color to Red
			hbr = CreateSolidBrush(RGB(255, 0, 0));
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		else if(EDC_Calc != 0 && 0.95 * result >  EDC_Calc)
		{
			pDC->SetTextColor(RGB(0, 0, 0));// Set color to green
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(0, 255, 0));
			hbr = CreateSolidBrush(RGB(0, 255, 0));
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		else if(EDC_Calc != 0 && 0.95 * result < EDC_Calc && 1.00 * result >= EDC_Calc)
		{
			pDC->SetTextColor(RGB(0, 0, 0));// Set color to yellow
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(255, 255, 0));
			hbr = CreateSolidBrush(RGB(255, 255, 0));
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		else{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(WHITE);
			hbr = CreateSolidBrush(WHITE);
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
	   }
		 default:	return ReportView::OnCtlColor(pDC, pWnd, nCtlColor);
		 }
	   }
	 if (ID == IDC_WristDuration)
   {
	   switch (nCtlColor){
		 case CTLCOLOR_STATIC:
	   {
		double result =GetDlgItemInt(ID);
		UpdateData(false);
		if(m_ExertionDuration != 0 &&  m_ExertionDuration > 1.00 * result)
		{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(255, 0, 0));// Set color to Red
			hbr = CreateSolidBrush(RGB(255, 0, 0));
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		else if(m_ExertionDuration != 0 && 0.95 * result >  m_ExertionDuration)
		{
			pDC->SetTextColor(RGB(0, 0, 0));// Set color to green
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(0, 255, 0));
			hbr = CreateSolidBrush(RGB(0, 255, 0));
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		else if(m_ExertionDuration != 0 && 0.95 * result < m_ExertionDuration && 1.00 * result >= m_ExertionDuration)
		{
			pDC->SetTextColor(RGB(0, 0, 0));// Set color to yellow
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(255, 255, 0));
			hbr = CreateSolidBrush(RGB(255, 255, 0));
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		else{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(WHITE);
			hbr = CreateSolidBrush(WHITE);
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
	   }
		 default:	return ReportView::OnCtlColor(pDC, pWnd, nCtlColor);
		 }
	   }
	 	 if (ID == IDC_ElbowDuration)
   {
	   switch (nCtlColor){
		 case CTLCOLOR_STATIC:
	   {
		double result =GetDlgItemInt(ID);
		UpdateData(false);
		if(m_ExertionDuration != 0 &&  m_ExertionDuration > 1.00 * result)
		{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(255, 0, 0));// Set color to Red
			hbr = CreateSolidBrush(RGB(255, 0, 0));
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		else if(m_ExertionDuration != 0 && 0.95 * result >  m_ExertionDuration)
		{
			pDC->SetTextColor(RGB(0, 0, 0));// Set color to green
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(0, 255, 0));
			hbr = CreateSolidBrush(RGB(0, 255, 0));
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		else if(m_ExertionDuration != 0 && 0.95 * result < m_ExertionDuration && 1.00 * result >= m_ExertionDuration)
		{
			pDC->SetTextColor(RGB(0, 0, 0));// Set color to yellow
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(255, 255, 0));
			hbr = CreateSolidBrush(RGB(255, 255, 0));
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		else{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(WHITE);
			hbr = CreateSolidBrush(WHITE);
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
	   }
		 default:	return ReportView::OnCtlColor(pDC, pWnd, nCtlColor);
		 }
	   }
		 	 if (ID == IDC_ShoulderDuration)
   {
	   switch (nCtlColor){
		 case CTLCOLOR_STATIC:
	   {
		double result =GetDlgItemInt(ID);
		UpdateData(false);
		if(m_ExertionDuration != 0 &&  m_ExertionDuration > 1.00 * result)
		{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(255, 0, 0));// Set color to Red
			hbr = CreateSolidBrush(RGB(255, 0, 0));
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		else if(m_ExertionDuration != 0 && 0.95 * result >  m_ExertionDuration)
		{
			pDC->SetTextColor(RGB(0, 0, 0));// Set color to green
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(0, 255, 0));
			hbr = CreateSolidBrush(RGB(0, 255, 0));
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		else if(m_ExertionDuration != 0 && 0.95 * result < m_ExertionDuration && 1.00 * result >= m_ExertionDuration)
		{
			pDC->SetTextColor(RGB(0, 0, 0));// Set color to yellow
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(RGB(255, 255, 0));
			hbr = CreateSolidBrush(RGB(255, 255, 0));
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		else{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(WHITE);
			hbr = CreateSolidBrush(WHITE);
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
	   }
		 default:	return ReportView::OnCtlColor(pDC, pWnd, nCtlColor);
		 }
	   }
	 // if (ID == IDC_LeftWristFlex18)
  // {
	 //  switch (nCtlColor){
		// case CTLCOLOR_STATIC:
	 //  {
		//double result =GetDlgItemInt(ID);
		//UpdateData(false);
		//if(m_TaskDutyCycle != 0 &&  m_TaskDutyCycle >= result)
		//{
		//	pDC->SetTextColor(RGB(255, 0, 0));// Set color to Red
		//	pDC->SetBkMode(OPAQUE);
		//	pDC->SetBkColor(RGB(0, 0, 0));
		//	return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		//}
		//else if(m_TaskDutyCycle != 0 && result > 2 + m_TaskDutyCycle)
		//{
		//	pDC->SetTextColor(RGB(0, 255, 0));// Set color to green
		//	pDC->SetBkMode(OPAQUE);
		//	pDC->SetBkColor(RGB(0, 0, 0));
		//	return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		//}
		//else if(m_TaskDutyCycle != 0 && result <= 2 + m_TaskDutyCycle && result > m_TaskDutyCycle)
		//{
		//	pDC->SetTextColor(RGB(255, 255, 0));// Set color to yellow
		//	pDC->SetBkMode(OPAQUE);
		//	pDC->SetBkColor(RGB(0, 0, 0));
		//	return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		//}
		//else{return (HBRUSH)::GetStockObject(NULL_BRUSH);}
	 //  }
		// default:	return ReportView::OnCtlColor(pDC, pWnd, nCtlColor);
		// }
	 //  }
	  }
	  else if (flag == 0)
	  {
		  if (ID == IDC_WristMDC ||ID == IDC_ElbowMDC||ID == IDC_ShoulderMDC||ID == IDC_WristDuration || ID == IDC_ElbowDuration || ID == IDC_ShoulderDuration)
		{
			if(EDC_Calc == 0)
		{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(WHITE);
			hbr = CreateSolidBrush(WHITE);
			return hbr;
			//return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		}
	}
	return ReportView::OnCtlColor(pDC, pWnd, nCtlColor);
}

#define COP_PRINT_SCALE 7
void StatusBox::OnDraw(CDC* pDC) {
   C_Hom_Doc* pDoc = (C_Hom_Doc*)GetDocument();

   if(pDoc->getPosition() == P_Standing)
   {
	   glBasic = &glBalanceStanding;
   } else { 
	   glBasic = &glBalanceSitting;
   }
   glBasic->notDrawCOG();
   glBasic->notDrawSF(); // does not show the support forces
   glBasic->onDraw();
}
/////////////////////////////////////////////////////////////////////////////
// StatusBox message handlers

//---------------------------------------------------------------------------

void StatusBox::OnDisplayDialogcolor() 
{
	C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();
	lDocPtr->setReportColor();

	Invalidate(true);
}

void StatusBox::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);

    //This would be a good place to put code that resizes the controls in the status box
    //when the window size is changed. The only way I can think of doing this is to
    //resize each control individually. 
	//Get screen resolution
    //int screen_width = GetSystemMetrics(SM_CXSCREEN);
    //int screen_height = GetSystemMetrics(SM_CYSCREEN);
}

void StatusBox::OnUpdateDisplayDialogcolor(CCmdUI* pCmdUI) 
{
	//C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument(); ??
	
	Invalidate(true);
}

void StatusBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CView::OnShowWindow(bShow, nStatus);

    CWnd *ImageFrame = GetDlgItem(IDC_COP_GRAPHIC);
	CRect copRect;
 	ImageFrame->GetClientRect(copRect);
	/*CRect frameRect;
	this->GetParentFrame()->GetWindowRect(&frameRect);
	ImageFrame->ScreenToClient(&frameRect);
	copRect.OffsetRect(-frameRect.left, frameRect.bottom-copRect.bottom);*/
	// Create OpenGL drawing box	
	if (COPBoxCreated == false)
	{
		mCOPDrawingRect = 
 			CRect(0,0,copRect.Width(),copRect.Height());
	
		if (COPBox.Create("", WS_CHILD | SS_BITMAP | WS_VISIBLE, mCOPDrawingRect, ImageFrame))
			COPBoxCreated = true;
	} 
}

void StatusBox::UpdateAll() {
	UpdateUnits();
	UpdateGravity();
	UpdatePercentileLimits();
}

void StatusBox::UpdateUnits() {
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";

	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";

	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}

void StatusBox::UpdateGravity() {
	C_Hom_Doc* lDocPtr = (C_Hom_Doc*)GetDocument();

	double gravValue = lDocPtr->getGravity();
    // Set the gravity text

    // JTK - We don't want Normal Gravity to be displayed when it is zero
    const CString NORMAL_GRAVITY =  ""; //"Normal Gravity";
    if (gravValue >= 9.80 && gravValue < 9.82)
	    mGravityText = NORMAL_GRAVITY;
    else if (gravValue >= 3.72 && gravValue <= 3.74)
        mGravityText = "Mars Gravity";
	// 6.0.4 used 1.56 for moon, 6.0.5 uses 1.6
    else if (gravValue >= 1.55 && gravValue <= 1.6)
        mGravityText = "Moon Gravity";
    else if (gravValue == 0)
        mGravityText = "Zero Gravity";
    else if (gravValue <= 0.1)
        mGravityText = "Micro Gravity";
    else if (gravValue < 9.81)
        mGravityText = "Low Gravity";
    else
        mGravityText = "High Gravity";

    // Set the gravity value

    char* accel_units;

	if( lDocPtr->Is_English() )//(E_Units)lDocPtr->getDescriptionRef().Units() == U_ENGLISH ) 
    {
        gravValue = lDocPtr->getGravity() / ft_TO_in / in_TO_cm / cm_TO_m;
		accel_units = "ft/s²";
    }
    else
    {
        gravValue = lDocPtr->getGravity();
        accel_units = "m/s²";

    }

    CString GravText;

    if(mGravityText != NORMAL_GRAVITY)
    {
		if(lDocPtr->getGravity() <= 0.1)
			GravText.Format("%.3f", gravValue);
		else if(lDocPtr->getGravity() < 9.81)
			GravText.Format("%.2f", gravValue);
		else
			GravText.Format("%.1f", gravValue);
     
	    mGravityValue = GravText;
	    mGravityValue += " ";
	    mGravityValue += accel_units;
    }
    else
        mGravityValue = "";
}
void StatusBox::UpdatePercentileLimits() {
	C_Hom_Doc* lDocPtr = (C_Hom_Doc*)GetDocument();
    Skeleton &lSkeleton = *lDocPtr->GetSkeleton();
	AnalysisSummaryLimits limits = lSkeleton.getAnalysisSummaryLimits();

	if(lDocPtr->getLimitChoice() == LC_NIOSH) {
		mWristBar.SetAllRanges(0, limits.SUL, limits.SUL, limits.SDL, limits.SDL, 100);
		mElbowBar.SetAllRanges(0, limits.SUL, limits.SUL, limits.SDL, limits.SDL, 100);
		mShoulderBar.SetAllRanges(0, limits.SUL, limits.SUL, limits.SDL, limits.SDL, 100);
		mTorsoBar.SetAllRanges(0, limits.SUL, limits.SUL, limits.SDL, limits.SDL, 100);
		mHipBar.SetAllRanges(0, limits.SUL, limits.SUL, limits.SDL, limits.SDL, 100);
		mKneeBar.SetAllRanges(0, limits.SUL, limits.SUL, limits.SDL, limits.SDL, 100);
		mAnkleBar.SetAllRanges(0, limits.SUL, limits.SUL, limits.SDL, limits.SDL, 100);
	} else {
		int lowerPercentileLimit, upperPercentileLimit;
		if(lDocPtr->getAnthropometryRef().Sex() == S_Male) {
			lowerPercentileLimit = lDocPtr->getLowerPercentileLimit();
			upperPercentileLimit = lDocPtr->getUpperPercentileLimit();
		} else {
			lowerPercentileLimit = lDocPtr->getFemaleLowerPercentileLimit();
			upperPercentileLimit = lDocPtr->getFemaleUpperPercentileLimit();
		}
		mWristBar.SetAllRanges(0, lowerPercentileLimit, lowerPercentileLimit,
								  upperPercentileLimit, upperPercentileLimit, 100);
		mElbowBar.SetAllRanges(0, lowerPercentileLimit, lowerPercentileLimit,
								  upperPercentileLimit, upperPercentileLimit, 100);
		mShoulderBar.SetAllRanges(0, lowerPercentileLimit, lowerPercentileLimit,
								  upperPercentileLimit, upperPercentileLimit, 100);
		mTorsoBar.SetAllRanges(0, lowerPercentileLimit, lowerPercentileLimit,
								  upperPercentileLimit, upperPercentileLimit, 100);
		mHipBar.SetAllRanges(0, lowerPercentileLimit, lowerPercentileLimit,
								  upperPercentileLimit, upperPercentileLimit, 100);
		mKneeBar.SetAllRanges(0, lowerPercentileLimit, lowerPercentileLimit,
								  upperPercentileLimit, upperPercentileLimit, 100);
		mAnkleBar.SetAllRanges(0, lowerPercentileLimit, lowerPercentileLimit,
								  upperPercentileLimit, upperPercentileLimit, 100);
	}
}

void StatusBox::OnEnChangeShouldedit()
{
	UpdateData(true);
	GLOblique::shouldDisp = atof(shouldEditVal);
	((C_Hom_Doc*)GetDocument())->UpdateViews();
}

void StatusBox::OnFileCopyactivewindow()
{
	ReportView::OnFileCopyactivewindow();
	return;
	DIB dib(this->GetParentFrame());
	dib.PutOnClipboard();
	// TODO printing
	/*CDib dib(this->GetParentFrame());

	//Attempt to open the clipboard
   if ( ! this->OpenClipboard() )
   {
      AfxMessageBox( "CDibView::CopyDIBToClipboard: Cannot open the Clipboard" );
      return;
   }
   
   //Remove the current Clipboard contents
   if( ! EmptyClipboard() )
   {
      AfxMessageBox( "DibView::CopyDIBToClipboard: Cannot empty the Clipboard" );
      return;
   }

   //Get the data to copy
   HANDLE   lBitMapInfoHdl = dib.BitMapInfoHdl();

   if( lBitMapInfoHdl != NULL )
   {
      //Give to the clipboard
      if ( ::SetClipboardData( CF_DIB, lBitMapInfoHdl ) == NULL )
      {
	 AfxMessageBox( "DibView::CopyDIBToClipboard: Unable to set Clipboard data" );
      }
   }

   CloseClipboard();*/
}



void StatusBox::OnBnClickedGenerate()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	EDC_Calc = (double)m_Exertion*m_ExertionDuration/m_CycleTime * 100 ;
	m_ExertionDutyCycle.Format("%.1f", EDC_Calc);
	flag = 1;

	docPtr->EDC_Value[0] = m_Exertion;
	docPtr->EDC_Value[1] = m_ExertionDuration;
	docPtr->EDC_Value[2] = m_CycleTime;
	docPtr->EDC_Value[3] = EDC_Calc;


	UpdateData(false);
	docPtr->MakeDirtyAndUpdateViews();
	Invalidate(); // refresh the window
}


//void StatusBox::OnBnClickedReset()
//{
//	// TODO: Add your control notification handler code here
//	m_ExertionDutyCycle = 0;
//	m_Exertion = 0;
//	m_ExertionDuration = 0;
//	m_CycleTime = 0;
//	flag = 0;
//	UpdateData(false);
//	Invalidate(); // refresh the winodw
//}


//void StatusBox::OnEnChangeCycletime()
//{
//	// TODO:  If this is a RICHEDIT control, the control will not
//	// send this notification unless you override the ReportView::OnInitDialog()
//	// function and call CRichEditCtrl().SetEventMask()
//	// with the ENM_CHANGE flag ORed into the mask.
//
//	// TODO:  Add your control notification handler code here
//
//	UpdateData();
//	m_ExertionDutyCycle = (double)m_Exertion*m_ExertionDuration/m_CycleTime * 100 + 0.5 ;
//	
//	if (m_ExertionDutyCycle == 0) flag = 0;
//	else flag = 1;
//	UpdateData(false);
//	Invalidate(); // refresh the window
//}
//
//
//void StatusBox::OnEnChangeExertion()
//{
//	UpdateData();
//	m_ExertionDutyCycle = (double)m_Exertion*m_ExertionDuration/m_CycleTime * 100 + 0.5 ;
//	if (m_ExertionDutyCycle == 0) flag = 0;
//	else flag = 1;
//	UpdateData(false);
//	Invalidate(); // refresh the window
//}
//
//
//void StatusBox::OnEnChangeEd()
//{
//	UpdateData();
//	m_ExertionDutyCycle = (double)m_Exertion*m_ExertionDuration/m_CycleTime * 100 + 0.5 ;
//	if (m_ExertionDutyCycle == 0) flag = 0;
//	else flag = 1;
//	UpdateData(false);
//	Invalidate(); // refresh the window
//}
