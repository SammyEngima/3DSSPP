// ReportBalanceUnified.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ReportBalanceUnified.h"
#include "Hom_Doc.h"
#include "Skeleton.hpp"
#include "Types.h"
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

// ReportBalanceUnified

ReportView* ReportBalanceUnified::cPointer = NULL;
IMPLEMENT_DYNCREATE(ReportBalanceUnified, ReportView)

ReportBalanceUnified::ReportBalanceUnified()
	: ReportView(ReportBalanceUnified::IDD)
	, glBalanceStanding(this)
	, glBalanceSitting(this)
	, mHeaderText(_T(""))
	, mCOPFB(_T(""))
	, mCOPRL(_T(""))
	, mCOGFB(_T(""))
	, mCOGRL(_T(""))
	, mCOGFB2(_T(""))
	, mCOGRL2(_T(""))
	, mCOGFB3(_T(""))
	, mCOGRL3(_T(""))
	, mBOSLabel1(_T(""))
	, mBOSLabel2(_T(""))
	, mBOSLabel3(_T(""))
	, mBOSLabel4(_T(""))
	, mBOSValue1(_T(""))
	, mBOSValue2(_T(""))
	, mBOSValue3(_T(""))
	, mBOSValue4(_T(""))
	, mBalanceStatus(_T(""))
	, mResidualMomX(_T(""))
	, mResidualMomY(_T(""))
	, mResidualMomZ(_T(""))
	, mLegLoadLeft(_T(""))
	, mLegLoadRight(_T(""))
	, mMomentLabel1(_T(""))
	, mMomentLabel2(_T(""))
	, mMomentLabel3(_T(""))
	, mMoment1XL(_T(""))
	, mMoment1YL(_T(""))
	, mMoment1ZL(_T(""))
	, mMoment2XL(_T(""))
	, mMoment2YL(_T(""))
	, mMoment2ZL(_T(""))
	, mMoment3XL(_T(""))
	, mMoment3YL(_T(""))
	, mMoment3ZL(_T(""))
	, mMoment1XR(_T(""))
	, mMoment1YR(_T(""))
	, mMoment1ZR(_T(""))
	, mMoment2XR(_T(""))
	, mMoment2YR(_T(""))
	, mMoment2ZR(_T(""))
	, mMoment3XR(_T(""))
	, mMoment3YR(_T(""))
	, mMoment3ZR(_T(""))
	, mFooter(_T(""))
	, mCOPFrame(_T(""))
	//, mCOGFrame(_T(""))
	//, mCOSFrame(_T(""))
	, mBOSFrame(_T(""))
	, mResidualMomFrame(_T(""))
	, mMomentFrame(_T(""))
	, mHandForceUnits(_T(""))
	, mSupportForceUnits(_T(""))
	, mUnitsLabel(_T(""))
	, mLeftHori(_T(""))
{
	cPointer = this;
	COPBoxCreated = false;
	COPGraphicError = false;
}

ReportBalanceUnified::~ReportBalanceUnified()
{
	cPointer = NULL;
}

CString ReportBalanceUnified::ReportName(void) const {
	return "3DSSPP - Balance Report";
}

void ReportBalanceUnified::DoDataExchange(CDataExchange* pDX)
{
	ReportView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Header, mHeaderText);
	DDX_Text(pDX, IDC_COPFB, mCOPFB);
	DDX_Text(pDX, IDC_COPRL, mCOPRL);
	DDX_Text(pDX, IDC_COGFB, mCOGFB);
	DDX_Text(pDX, IDC_COGRL, mCOGRL);
	DDX_Text(pDX, IDC_COGFB2, mCOGFB2);
	DDX_Text(pDX, IDC_COGFB3, mCOGFB3);
	DDX_Text(pDX, IDC_COGRL2, mCOGRL2);
	DDX_Text(pDX, IDC_COGRL3, mCOGRL3);
	DDX_Text(pDX, IDC_BOSLABEL1, mBOSLabel1);
	DDX_Text(pDX, IDC_BOSLABEL2, mBOSLabel2);
	DDX_Text(pDX, IDC_BOSLABEL3, mBOSLabel3);
	DDX_Text(pDX, IDC_BOSLABEL4, mBOSLabel4);
	DDX_Control(pDX, IDC_BOSLABEL4, mBOSLabel4Control);
	DDX_Text(pDX, IDC_BOSVALUE1, mBOSValue1);
	DDX_Text(pDX, IDC_BOSVALUE2, mBOSValue2);
	DDX_Text(pDX, IDC_BOSVALUE3, mBOSValue3);
	DDX_Text(pDX, IDC_BOSVALUE4, mBOSValue4);
	DDX_Control(pDX, IDC_BOSVALUE4, mBOSValue4Control);
	DDX_Text(pDX, IDC_BALANCESTATUS, mBalanceStatus);
	DDX_Text(pDX, IDC_RESIDUALMOMX, mResidualMomX);
	DDX_Text(pDX, IDC_RESIDUALMOMY, mResidualMomY);
	DDX_Text(pDX, IDC_RESIDUALMOMZ, mResidualMomZ);
	DDX_Text(pDX, IDC_LEFTLOAD, mLegLoadLeft);
	DDX_Text(pDX, IDC_RIGHTLOAD, mLegLoadRight);
	DDX_Text(pDX, IDC_MOMLABEL1, mMomentLabel1);
	DDX_Text(pDX, IDC_MOMLABEL2, mMomentLabel2);
	DDX_Text(pDX, IDC_MOMLABEL3, mMomentLabel3);
	DDX_Text(pDX, IDC_MOM1XL, mMoment1XL);
	DDX_Text(pDX, IDC_MOM1YL, mMoment1YL);
	DDX_Text(pDX, IDC_MOM1ZL, mMoment1ZL);
	DDX_Text(pDX, IDC_MOM2XL, mMoment2XL);
	DDX_Text(pDX, IDC_MOM2YL, mMoment2YL);
	DDX_Text(pDX, IDC_MOM2ZL, mMoment2ZL);
	DDX_Text(pDX, IDC_MOM3XL, mMoment3XL);
	DDX_Text(pDX, IDC_MOM3YL, mMoment3YL);
	DDX_Text(pDX, IDC_MOM3ZL, mMoment3ZL);
	DDX_Text(pDX, IDC_MOM1XR, mMoment1XR);
	DDX_Text(pDX, IDC_MOM1YR, mMoment1YR);
	DDX_Text(pDX, IDC_MOM1ZR, mMoment1ZR);
	DDX_Text(pDX, IDC_MOM2XR, mMoment2XR);
	DDX_Text(pDX, IDC_MOM2YR, mMoment2YR);
	DDX_Text(pDX, IDC_MOM2ZR, mMoment2ZR);
	DDX_Text(pDX, IDC_MOM3XR, mMoment3XR);
	DDX_Text(pDX, IDC_MOM3YR, mMoment3YR);
	DDX_Text(pDX, IDC_MOM3ZR, mMoment3ZR);
	DDX_Text(pDX, IDC_Footer, mFooter);
	DDX_Control(pDX, IDC_MOMLABEL3, mMomentLabel3Control);
	DDX_Control(pDX, IDC_MOM3XL, mMoment3XLControl);
	DDX_Control(pDX, IDC_MOM3YL, mMoment3YLControl);
	DDX_Control(pDX, IDC_MOM3ZL, mMoment3ZLControl);
	DDX_Control(pDX, IDC_MOM3XL, mMoment3XLControl);
	DDX_Control(pDX, IDC_MOM3YL, mMoment3YLControl);
	DDX_Control(pDX, IDC_MOM3ZL, mMoment3ZLControl);
	DDX_Control(pDX, IDC_MOM3XR, mMoment3XRControl);
	DDX_Control(pDX, IDC_MOM3YR, mMoment3YRControl);
	DDX_Control(pDX, IDC_MOM3ZR, mMoment3ZRControl);
	DDX_Control(pDX, IDC_MOM3XR, mMoment3XRControl);
	DDX_Control(pDX, IDC_MOM3YR, mMoment3YRControl);
	DDX_Control(pDX, IDC_MOM3ZR, mMoment3ZRControl);
	DDX_Text(pDX, IDC_COPFRAME, mCOPFrame);
	//DDX_Text(pDX, IDC_COGFRAME, mCOGFrame);
	//DDX_Text(pDX, IDC_COGFRAME2, mCOSFrame);
	DDX_Text(pDX, IDC_BOSFRAME, mBOSFrame);
	DDX_Text(pDX, IDC_RESIDUALMOMFRAME, mResidualMomFrame);
	DDX_Text(pDX, IDC_MOMENTFRAME, mMomentFrame);

	DDX_Text(pDX, IDC_HandForceUnits, mHandForceUnits);
	DDX_Text(pDX, IDC_HandForceUnits8, mSupportForceUnits);
	DDX_Text(pDX, IDC_HAND_FORCE_TXT_LVAL, mHandForceTxtLVal);
	DDX_Text(pDX, IDC_HAND_FORCE_TXT_RVAL, mHandForceTxtRVal);
	DDX_Text(pDX, IDC_HAND_FORCE_TXT_LVAL3, mSupportForceTxtLVal);
	DDX_Text(pDX, IDC_HAND_FORCE_TXT_RVAL3, mSupportForceTxtRVal);
	DDX_Text(pDX, IDC_UnitsLabel, mUnitsLabel);
	DDX_Text(pDX, IDC_RightHori, mRightHori);
	DDX_Text(pDX, IDC_RightLat2, mRightLat);
	DDX_Text(pDX, IDC_RightVert2, mRightVert);
	DDX_Text(pDX, IDC_LeftHori, mLeftHori);
	DDX_Text(pDX, IDC_LeftLat2, mLeftLat);
	DDX_Text(pDX, IDC_LeftVert2, mLeftVert);
}

BEGIN_MESSAGE_MAP(ReportBalanceUnified, ReportView)
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_MESSAGE(WM_DRAWOPENGL, OnDrawOpenGL)
END_MESSAGE_MAP()


// ReportBalanceUnified diagnostics

#ifdef _DEBUG
void ReportBalanceUnified::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ReportBalanceUnified::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ReportBalanceUnified message handlers

//void ReportBalanceUnified::OnFilePrintPreview() {
//	SSPPPrintPreview(this);
//}

LRESULT ReportBalanceUnified::OnDrawOpenGL(WPARAM wParam, LPARAM lParam) {
	// draw balance graphic
	glBasic->init();
	
	// set viewing rect
	CRect oldRect = glBasic->getViewingRect();
	CWnd *ImageFrame = GetDlgItem(IDC_COPGRAPHIC);
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
void ReportBalanceUnified::OnDraw(CDC* /*pDC*/)
{
	glBasic->drawCOG();
	glBasic->drawSF();
	glBasic->onDraw();
}

void ReportBalanceUnified::OnInitialUpdate()
{
	ReportView::OnInitialUpdate();

	CWnd* ImageFrame = GetDlgItem(IDC_COPGRAPHIC);
	CRect copRect;
	ImageFrame->GetClientRect(copRect);
	// Create OpenGL drawing box
	if(COPBoxCreated == false) {
		mCOPDrawingRect = CRect(0,0,copRect.Width(),copRect.Height());
		if(COPBox.Create("", WS_CHILD | SS_BITMAP | WS_VISIBLE, mCOPDrawingRect, ImageFrame)) {
			COPBoxCreated = true;
		}
	}

	glBalanceSitting.initialize(&COPBox);
	glBalanceSitting.setViewport(mCOPDrawingRect);
	glBalanceStanding.share(&glBalanceSitting);
	glBalanceStanding.setViewport(mCOPDrawingRect);
	C_Hom_Doc* lDocPtr = (C_Hom_Doc*)GetDocument();
	
	if(lDocPtr->getPosition() == P_Standing) {
		glBasic = &glBalanceStanding;
	} else {
		glBasic = &glBalanceSitting;
	}

	// Set the hand force box units
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();
	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";
	mSupportForceUnits = CString("Support Forces (") + docPtr->ForceUnits() + ")";
	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}

void ReportBalanceUnified::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	C_Hom_Doc* lDocPtr = (C_Hom_Doc*)GetDocument();

	const BalanceData_s& balance = lDocPtr->GetSkeleton()->getBalanceData();

	mHeaderText = lDocPtr->ReportHeader();
	mFooter = lDocPtr->ReportFooter();


	CString torque_units = lDocPtr->TorqueUnits();
	CString length_units = lDocPtr->LengthUnits();

	// Frame labels with units
	// TODO better circle / plus symbols?
	mCOPFrame = "Center of Pressure (" + length_units + ")";
	//mCOGFrame = "Center of Body Mass (" + length_units + ")";
	//mCOSFrame = "Center of Support Forces (" + length_units + ")";
	mBOSFrame = "Base of Support (" + length_units + ")";
	mResidualMomFrame = "Residual Support Moments (" + torque_units + ")";
	mMomentFrame = "Maximum Balance Moments (" + torque_units + ")";

	Skeleton& skel = *lDocPtr->GetSkeleton();
	Vector3 seatRef = skel.getSeatReference() - skel.getJoint(JT_ANKLE_CENTER);
	Position_e position = lDocPtr->getPosition();

	// display the COP/COG values WRT current origin
	// values are stored in Balance_Data WRT ankle center
	// we should change this to WRT hip center like everything else

	Vector3 cur_origin = lDocPtr->getOffset();
	Vector3 offset =  lDocPtr->getOffset(&skel, true);
	Vector3 ankle_center = skel.getJoint(JT_ANKLE_CENTER);

	// Center of pressure
	//if(position == P_Standing) {
		mCOPFB.Format("%.1f", balance.CenterOfPressure[1] + ankle_center[1] - cur_origin[1]);
		mCOPRL.Format("%.1f", balance.CenterOfPressure[0] + ankle_center[0] - cur_origin[0]);
	//} else {
	//	mCOPFB.Format("%.1f", balance.CenterOfPressure[1] - seatRef[1]);
	//	mCOPRL.Format("%.1f", balance.CenterOfPressure[0] - seatRef[0]);
	//}

	// Center of gravity
	//if(position == P_Standing) {
		mCOGFB.Format("%.1f", balance.CenterOfGravity[1] + ankle_center[1] - cur_origin[1]);
		mCOGRL.Format("%.1f", balance.CenterOfGravity[0] + ankle_center[1] - cur_origin[1]);
	//} else {
	//	mCOGFB.Format("%.1f", balance.CenterOfGravity[1] - seatRef[1]);
	//	mCOGRL.Format("%.1f", balance.CenterOfGravity[0] - seatRef[0]);
	//}

	// Center of left support forces
	Vector3 LfootForceCenter;		
	Vector3 RfootForceCenter;
				
	Vector3 x1 = skel.getJoint(JT_LHEEL, false) - cur_origin;
	Vector3 x2 = skel.getJoint(JT_LBALL, false) - cur_origin;
	Vector3 x3 = skel.getJoint(JT_RHEEL, false) - cur_origin;
	Vector3 x4 = skel.getJoint(JT_RBALL, false) - cur_origin;
				
	LfootForceCenter[2] = x1[2];
	RfootForceCenter[2] = x3[2];
				
	LfootForceCenter[0] = (x2 + (x1 - x2) * skel.getNetForce(JT_LHEEL).length() / skel.getNetFeetForce(JT_LFOOT_CENTER).length())[0];
	LfootForceCenter[1] = (x2 + (x1 - x2) * skel.getNetForce(JT_LHEEL).length() / skel.getNetFeetForce(JT_LFOOT_CENTER).length())[1];
	RfootForceCenter[0] = (x4 + (x3 - x4) * skel.getNetForce(JT_RHEEL).length() / skel.getNetFeetForce(JT_RFOOT_CENTER).length())[0];
	RfootForceCenter[1] = (x4 + (x3 - x4) * skel.getNetForce(JT_RHEEL).length() / skel.getNetFeetForce(JT_RFOOT_CENTER).length())[1];

	//if (!lDocPtr->Is_Metric()){
	//	mCOGFB2.Format("%.1f", LfootForceCenter[0] * CM_TO_IN);
	//	mCOGFB3.Format("%.1f", LfootForceCenter[1] * CM_TO_IN);
	//}
	//else{
		mCOGFB2.Format("%.1f", LfootForceCenter[0] + ankle_center[0] - cur_origin[0]);
		mCOGFB3.Format("%.1f", LfootForceCenter[1] + ankle_center[1] - cur_origin[1]);
	//}

	// Center of right support forces
	//if (!lDocPtr->Is_Metric()){
	//	mCOGRL2.Format("%.1f", RfootForceCenter[0] * CM_TO_IN);
	//	mCOGRL3.Format("%.1f", RfootForceCenter[1] * CM_TO_IN);
	//}
	//else{
		mCOGRL2.Format("%.1f", RfootForceCenter[0] + ankle_center[0] - cur_origin[0]);
		mCOGRL3.Format("%.1f", RfootForceCenter[1] + ankle_center[1] - cur_origin[1]);
	//}

	// BOS
	if(position == P_Standing) {
		mBOSLabel1 = "Dist to Front:";
		if (balance.DistToFront < 0)
	        mBOSValue1 = "outside";
		else
			mBOSValue1.Format("%0.1f", balance.DistToFront);
		mBOSLabel2 = "Dist to Back:";
		if (balance.DistToBack < 0)
	        mBOSValue2 = "outside";
		else
			mBOSValue2.Format("%0.1f", balance.DistToBack);
		mBOSLabel3 = "Dist to Right:";
		if (balance.DistToRight < 0)
	        mBOSValue3 = "outside";
		else
			mBOSValue3.Format("%0.1f", balance.DistToRight);
		mBOSLabel4 = "Dist to Left:";
		if (balance.DistToLeft < 0)
	        mBOSValue4 = "outside";
		else
			mBOSValue4.Format("%0.1f", balance.DistToLeft);
		mBOSLabel4Control.ShowWindow(SW_SHOWNA);
		mBOSValue4Control.ShowWindow(SW_SHOWNA);
	} else {
		mBOSLabel1 = "Front Boundary";
		mBOSValue1.Format("%.1f", balance.FrontSeat);
		mBOSLabel2 = "Left Boundary";
		mBOSValue2.Format("%.1f", balance.LeftSeat);
		mBOSLabel3 = "Right Boundary";
		mBOSValue3.Format("%.1f", balance.RightSeat);
		mBOSLabel4Control.ShowWindow(SW_HIDE);
		mBOSValue4Control.ShowWindow(SW_HIDE);
	}

	// balance status
	if(balance.BalanceStatus == BS_Acceptable) {
		mBalanceStatus = "Acceptable";
	} else if(balance.BalanceStatus == BS_Critical) {
		mBalanceStatus = "Critical";
	} else {
		mBalanceStatus = "Unacceptable";
	}

	// Residual Support Moments
	mResidualMomX.Format("%.1f", balance.HipResidualMoment[0]);
	mResidualMomY.Format("%.1f", balance.HipResidualMoment[1]);
	mResidualMomZ.Format("%.1f", balance.HipResidualMoment[2]);

	// Leg Loads
	mLegLoadLeft.Format("%d%%", (int)balance.LeftPctLoad);
	mLegLoadRight.Format("%d%%", (int)balance.RightPctLoad);

	// Maximum Balance Moments
	if(position == P_Standing) {
		mMomentLabel1 = "Ball:";
		mMoment1XL.Format("%.1f", balance.LeftBallMoment[0]);
		mMoment1YL.Format("%.1f", balance.LeftBallMoment[1]);
		mMoment1ZL.Format("%.1f", balance.LeftBallMoment[2]);
		mMoment1XR.Format("%.1f", balance.RightBallMoment[0]);
		mMoment1YR.Format("%.1f", balance.RightBallMoment[1]);
		mMoment1ZR.Format("%.1f", balance.RightBallMoment[2]);
		mMomentLabel2 = "Heel:";
		mMoment2XL.Format("%.1f", balance.LeftHeelMoment[0]);
		mMoment2YL.Format("%.1f", balance.LeftHeelMoment[1]);
		mMoment2ZL.Format("%.1f", balance.LeftHeelMoment[2]);
		mMoment2XR.Format("%.1f", balance.RightHeelMoment[0]);
		mMoment2YR.Format("%.1f", balance.RightHeelMoment[1]);
		mMoment2ZR.Format("%.1f", balance.RightHeelMoment[2]);
		mMomentLabel3Control.ShowWindow(SW_HIDE);
		mMoment3XLControl.ShowWindow(SW_HIDE);
		mMoment3YLControl.ShowWindow(SW_HIDE);
		mMoment3ZLControl.ShowWindow(SW_HIDE);
		mMoment3XRControl.ShowWindow(SW_HIDE);
		mMoment3YRControl.ShowWindow(SW_HIDE);
		mMoment3ZRControl.ShowWindow(SW_HIDE);
	} else {
		mMomentLabel1 = "IT:";
		mMoment1XL.Format("%.1f", balance.LeftITMoment[0]);
		mMoment1YL.Format("%.1f", balance.LeftITMoment[1]);
		mMoment1ZL.Format("%.1f", balance.LeftITMoment[2]);
		mMoment1XR.Format("%.1f", balance.RightITMoment[0]);
		mMoment1YR.Format("%.1f", balance.RightITMoment[1]);
		mMoment1ZR.Format("%.1f", balance.RightITMoment[2]);
		mMomentLabel2 = "Seat Front:";
		mMoment2XL.Format("%.1f", balance.LeftFrontSeatMoment[0]);
		mMoment2YL.Format("%.1f", balance.LeftFrontSeatMoment[1]);
		mMoment2ZL.Format("%.1f", balance.LeftFrontSeatMoment[2]);
		mMoment2XR.Format("%.1f", balance.RightFrontSeatMoment[0]);
		mMoment2YR.Format("%.1f", balance.RightFrontSeatMoment[1]);
		mMoment3ZR.Format("%.1f", balance.RightFrontSeatMoment[2]);
		mMomentLabel3 = "Ball of Foot:";
		mMoment3XL.Format("%.1f", balance.LeftBallMoment[0]);
		mMoment3YL.Format("%.1f", balance.LeftBallMoment[1]);
		mMoment3ZL.Format("%.1f", balance.LeftBallMoment[2]);
		mMoment3XR.Format("%.1f", balance.RightBallMoment[0]);
		mMoment3YR.Format("%.1f", balance.RightBallMoment[1]);
		mMoment3ZR.Format("%.1f", balance.RightBallMoment[2]);
		mMomentLabel3Control.ShowWindow(SW_SHOWNA);
		mMoment3XLControl.ShowWindow(SW_SHOWNA);
		mMoment3YLControl.ShowWindow(SW_SHOWNA);
		mMoment3ZLControl.ShowWindow(SW_SHOWNA);
		mMoment3XRControl.ShowWindow(SW_SHOWNA);
		mMoment3YRControl.ShowWindow(SW_SHOWNA);
		mMoment3ZRControl.ShowWindow(SW_SHOWNA);
	}

	// update glBasic pointer
	if(position == P_Standing) {
		glBasic = &glBalanceStanding;
	} else {
		glBasic = &glBalanceSitting;
	}
	glBasic->onDraw();

	UpdateData(false);
	UpdateUnits();

	Skeleton &lSkeleton = *lDocPtr->GetSkeleton();

	mHandForceTxtLVal.Format("%.1f", lSkeleton.getExtForce(JT_LHAND).length());
	mHandForceTxtRVal.Format("%.1f", lSkeleton.getExtForce(JT_RHAND).length());

	mSupportForceTxtLVal.Format("%.1f", lSkeleton.getNetFeetForce(JT_LFOOT_CENTER).length());
	mSupportForceTxtRVal.Format("%.1f", lSkeleton.getNetFeetForce(JT_RFOOT_CENTER).length());

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

void ReportBalanceUnified::UpdateUnits() {
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";

	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";

	// Set the feet force box units
	mSupportForceUnits = CString("Support Forces (") + docPtr->ForceUnits() + ")";

	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}