#if !defined(AFX_REPORTBALANCESEATED_H__27A61040_FC1E_11D4_8EBE_0001031CA2E0__INCLUDED_)
#define AFX_REPORTBALANCESEATED_H__27A61040_FC1E_11D4_8EBE_0001031CA2E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportBalanceSeated.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ReportBalanceSeated form view

#include "stdafx.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ReportView.hpp"
#include "winuser.h"
//#include "GLSeatCOP.h"
#include "GLBalanceSitting.h"
#include "WindowData.hpp"

class ReportBalanceSeated : public ReportView
{
protected:
	ReportBalanceSeated();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ReportBalanceSeated)

// Form Data
public:
	//{{AFX_DATA(ReportBalanceSeated)
	enum { IDD = IDD_ReportBalanceSeated };
	CString	mFooter;
	CString	mHeader;
	CString	mUnits4;
	CString	mCOPseat_y;
	CString	mCOPseat_x;
	CString	mSeatBalance;
	CString	mUnits5;
	CString	mUnits7;
	CString	m_seatFB;
	CString	m_seatLB;
	CString	m_seatRB;
	CString	mRightITMomX;
	CString	mRightITMomY;
	CString	mRightITMomZ;
	CString	mRightSFMomX;
	CString	mRightSFMomY;
	CString	mRightSFMomZ;
	CString	mLeftITMomX;
	CString	mLeftITMomY;
	CString	mLeftITMomZ;
    CString	mLeftBallMomX;
	CString	mLeftBallMomY;
	CString	mLeftBallMomZ;
    CString	mRightBallMomX;
	CString	mRightBallMomY;
	CString	mRightBallMomZ;
	CString	mLeftSFMomX;
	CString	mLeftSFMomY;
	CString	mLeftSFMomZ;
	CString mResidualMomBox;
    CString mResMomX;
    CString mResMomY;
    CString mResMomZ;
    //}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ReportBalanceSeated)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:

	void OnUpdate (CView* aSenderPtr, LPARAM aHint, CObject* aHintPtr);

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
   GLBalanceSitting glBalanceSitting;
   bool COPBoxCreated;
   CStatic COPBox;
   CRect mCOPDrawingRect;
   bool COPGraphicError;

   static ReportView *cPointer;
// Implementation
protected:
   virtual ~ReportBalanceSeated();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

   // Generated message map functions
   //{{AFX_MSG(ReportBalanceSeated)
   afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
   afx_msg void OnDisplayDialogcolor();
   virtual void OnDraw(CDC* pDC);
   //}}AFX_MSG
   
   //afx_msg void OnFilePrintPreview();

   DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
   virtual void OnInitialUpdate();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTBALANCESEATED_H__27A61040_FC1E_11D4_8EBE_0001031CA2E0__INCLUDED_)
