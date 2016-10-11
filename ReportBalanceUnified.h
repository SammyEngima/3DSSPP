#pragma once

#include "stdafx.h"
#include "ReportView.hpp"
#include "GLBalanceSitting.h"
#include "GLBalanceStanding.h"
#include "WindowData.hpp"
#include "afxwin.h"

#include "StatusBox.h"
#include "LimitScaleCtrl.h"
#include "resource.h"
#include "GLBalanceStanding.h"
#include "GLBalanceSitting.h"

#define ARROW_LENGTH 25.0f

// ReportBalanceUnified form view

class ReportBalanceUnified : public ReportView {
	DECLARE_DYNCREATE(ReportBalanceUnified)

protected:
	ReportBalanceUnified();           // protected constructor used by dynamic creation
	virtual ~ReportBalanceUnified();

public:
	enum { IDD = IDD_REPORTBALANCEUNIFIED };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	inline static BOOL IsOpen(void) {
		return cPointer != NULL;
	}

	static void GetWindowData(WindowData& aWD) {
		if(cPointer != NULL) {
			aWD.mIsOpen = true;
			WINDOWPLACEMENT wp;
			cPointer->GetParentFrame()->GetWindowPlacement(&wp);
			aWD.mRectangle = wp.rcNormalPosition;
		} else {
			aWD.mIsOpen = false;
		}
	}

	inline static ReportView* Pointer(void) {
		return cPointer;
	}

	virtual CString ReportName(void) const;

private:
	static ReportView* cPointer;
	bool COPBoxCreated;
	bool COPGraphicError;
	GLBalanceStanding glBalanceStanding;
	GLBalanceSitting glBalanceSitting;
	GLBasic* glBasic;
	CStatic COPBox;
	CRect mCOPDrawingRect;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//afx_msg void OnFilePrintPreview();
	afx_msg LRESULT OnDrawOpenGL(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	CString mHeaderText;
	CString mCOPFB;
	CString mCOPRL;
	CString mCOGFB;
	CString mCOGRL;
	CString mCOGFB2;
	CString mCOGRL2;
	CString mCOGFB3;
	CString mCOGRL3;
	CString mBOSLabel1;
	CString mBOSLabel2;
	CString mBOSLabel3;
	CString mBOSLabel4;
	CStatic mBOSLabel4Control;
	CString mBOSValue1;
	CString mBOSValue2;
	CString mBOSValue3;
	CString mBOSValue4;
	CStatic mBOSValue4Control;
	CString mBalanceStatus;
	CString mResidualMomX;
	CString mResidualMomY;
	CString mResidualMomZ;
	CString mLegLoadLeft;
	CString mLegLoadRight;
	CString mMomentLabel1;
	CString mMomentLabel2;
	CString mMomentLabel3;
	CString mMoment1XL;
	CString mMoment1YL;
	CString mMoment1ZL;
	CString mMoment2XL;
	CString mMoment2YL;
	CString mMoment2ZL;
	CString mMoment3XL;
	CString mMoment3YL;
	CString mMoment3ZL;
	CString mMoment1XR;
	CString mMoment1YR;
	CString mMoment1ZR;
	CString mMoment2XR;
	CString mMoment2YR;
	CString mMoment2ZR;
	CString mMoment3XR;
	CString mMoment3YR;
	CString mMoment3ZR;
	CString mFooter;
	CStatic mMomentLabel3Control;
	CStatic mMoment3XLControl;
	CStatic mMoment3YLControl;
	CStatic mMoment3ZLControl;
	CStatic mMoment3XRControl;
	CStatic mMoment3YRControl;
	CStatic mMoment3ZRControl;
	CString mCOPFrame;
	//CString mCOGFrame;
	//CString mCOSFrame;
	CString mBOSFrame;
	CString mResidualMomFrame;
	CString mMomentFrame;

	CString mHandForceUnits;
	CString mSupportForceUnits;
	CString	mUnitsLabel;
	CString m3DLowBackCompressionUnits;
	CString mHandForceTxtLVal;
	CString mHandForceTxtRVal;
	CString mSupportForceTxtLVal;
	CString mSupportForceTxtRVal;
	CString	mLeftHori;
	CString mRightHori;
	CString mLeftLat;
	CString mLeftVert;
	CString mRightLat;
	CString mRightVert;
protected:
	virtual void OnDraw(CDC* /*pDC*/);
public:
	virtual void OnInitialUpdate();
	virtual void UpdateUnits();
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};


