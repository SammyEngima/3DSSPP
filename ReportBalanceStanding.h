#if !defined(AFX_REPORTBALANCESTANDING_H__9A216392_25B0_11D4_A704_00A02400A079__INCLUDED_)
#define		 AFX_REPORTBALANCESTANDING_H__9A216392_25B0_11D4_A704_00A02400A079__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportBalanceStanding.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// ReportBalanceStanding form view

#include "stdafx.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ReportView.hpp"
#include "winuser.h"
#include "GLBalanceStanding.h"
#include "WindowData.hpp"

class ReportBalanceStanding : public ReportView
{
protected:
	ReportBalanceStanding();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ReportBalanceStanding)

// Form Data
public:
	//{{AFX_DATA(ReportBalanceStanding)
	enum { IDD = IDD_ReportBalanceStanding };
	CString	mFooter;
	CString	mHeader;
	CString	mCGBalance;
	CString	mDistToBack;
	CString	mDistToFront;
	CString	mDistToLeft;
	CString	mDistToRight;
	CString	mPWBCGy;
	CString	mPWBCGx;
	CString	mLOLM;
	CString	mLSLB;
	CString	mLSLF;
	CString	mRORM;
	CString	mRSRB;
	CString	mRSRF;
	CString mRightFrontText;
	CString mRightMiddleText;
	CString mRightBackText;
	CString mLeftFrontText;
	CString mLeftMiddleText;
	CString mLeftBackText;
	CString mRightHeelMomX;
	CString mRightHeelMomY;
	CString mRightHeelMomZ;
	CString mRightBallMomX;
	CString mRightBallMomY;
	CString mRightBallMomZ;
	CString mLeftHeelMomX;
	CString mLeftHeelMomY;
	CString mLeftHeelMomZ;
	CString mLeftBallMomX;
	CString mLeftBallMomY;
	CString mLeftBallMomZ;
    CString mResidualMomBox;
    CString mResMomX;
    CString mResMomY;
    CString mResMomZ;
	CString mCenOfGravX;
    CString mCenOfGravY;


//}}AFX_DATA

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ReportBalanceStanding)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
//--- Custom member functions ---

public:

	void OnUpdate(CView* aSenderPtr, LPARAM aHint, CObject * aHintPtr);

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

	static ReportView *cPointer;
// Implementation
protected:
	virtual ~ReportBalanceStanding();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(ReportBalanceStanding)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDisplayDialogcolor();
	virtual void OnDraw(CDC* pDC);
	//}}AFX_MSG
	//afx_msg void OnFilePrintPreview();

	DECLARE_MESSAGE_MAP()
	
private:
	bool COPBoxCreated;
	bool COPGraphicError;
	GLBalanceStanding COPgraphic;
	CStatic COPBox;
	CRect mCOPDrawingRect;
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CString LeftZPercent;
	CString RightZPercent;
	
	virtual void OnInitialUpdate();
public:
	CString mCOGLabel;
	CString mMBMLabel;
	CString mCOPLabel;
	CString mDTBLabel;
	CString mBCLabel;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTBALANCESTANDING_H__9A216392_25B0_11D4_A704_00A02400A079__INCLUDED_)
