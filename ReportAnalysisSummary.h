// ReportAnalysisSummary.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ReportAnalysisSummary form view
#ifndef REPORT_ANALYSIS_SUMMARY_H
#define REPORT_ANALYSIS_SUMMARY_H

#include "stdafx.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "resource.h"
#include "GLBalanceStanding.h"
#include "GLBalanceSitting.h"
#include "GLBasic.h"
#include "ReportView.hpp"
#include "WindowData.hpp"
#include "LimitScaleCtrl.h"

#include "StatusBox.h"
#include "LimitScaleCtrl.h"
#include "resource.h"
#include "GLBalanceStanding.h"
#include "GLBalanceSitting.h"

class ReportAnalysisSummary : public ReportView
{
protected:
	ReportAnalysisSummary();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ReportAnalysisSummary)

// Form Data
public:
	//{{AFX_DATA(ReportAnalysisSummary)
	enum { IDD = IDD_ReportAnalysisSummary };
	CLimitScaleCtrl	mL4L5DiscBar;
	CLimitScaleCtrl	mTorsoBar;
	CLimitScaleCtrl	mShoulderBar;
	CLimitScaleCtrl	mKneeBar;
	CLimitScaleCtrl	mHipBar;
	CLimitScaleCtrl	mElbowBar;
	CLimitScaleCtrl mWristBar;
	CLimitScaleCtrl	mAnkleBar;
	CString	mAnkleValue;
	CString	mElbowValue;
	CString mWristValue;
	CString	mHipValue;
	CString	mKneeValue;
	CString	mShoulderValue;
	CString	mTorsoValue;
	CString	mL4L5DiscValue;
	CString	mFooter;
	CString	mFrictionValue;
	CString	mHeader;
	CString	mLeftPercentLoadValue;
	CString	mRightPercentLoadValue;
	CString	mCGBalanceValue;
    CString mCompressionZeroLbl;
    CString mCompressionSDLLbl;
    CString mCompressionSULLbl;
    CString mCompressionMaxLbl;

	CString mHandForceUnits;
	CString	mUnitsLabel;
	CString m3DLowBackCompressionUnits;
	CString mHandForceTxtLVal;
	CString mHandForceTxtRVal;
	CString	mLeftHori;
	CString mRightHori;
	CString mLeftLat;
	CString mLeftVert;
	CString mRightLat;
	CString mRightVert;
	//}}AFX_DATA

// Attributes

// Operations

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ReportAnalysisSummary)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

//--- Custom member functions ---

public:
	virtual void UpdateUnits();

	void OnUpdate(CView* aSenderPtr, LPARAM aHint, CObject* aHintPtr);

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

//--- Custom Attributes ---

private:
	static ReportView * cPointer;
	bool COPBoxCreated;
	bool COPGraphicError;
	GLBalanceStanding glBalanceStanding;
	GLBalanceSitting glBalanceSitting;
	GLBasic* glBasic;
	CStatic COPBox;
	CRect mCOPDrawingRect;

// Implementation
protected:
	virtual ~ReportAnalysisSummary();


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
	//{{AFX_MSG(ReportAnalysisSummary)
	afx_msg void OnFooter();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDisplayDialogcolor();
	afx_msg LRESULT OnDrawOpenGL(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG     
	//afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()

	
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnInitialUpdate();
protected:
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
};
#endif
/////////////////////////////////////////////////////////////////////////////
