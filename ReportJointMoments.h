#if !defined(AFX_REPORTJOINTMOMENTS_H__000A0C72_3B2D_11D1_9349_000000000000__INCLUDED_)
#define AFX_REPORTJOINTMOMENTS_H__000A0C72_3B2D_11D1_9349_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ReportJointMoments.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ReportJointMoments form view

#include "stdafx.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ReportView.hpp"
#include "WindowData.hpp"
#include "Skeleton.hpp" // for NUM_JOINTS. if that moves to Types.h, remove this include
#include "afxwin.h"

#include "StatusBox.h"
#include "LimitScaleCtrl.h"
#include "resource.h"
#include "GLBalanceStanding.h"
#include "GLBalanceSitting.h"

class ReportJointMoments : public ReportView
{
protected:
	ReportJointMoments();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ReportJointMoments)

// Form Data
public:
	//{{AFX_DATA(ReportJointMoments)
	enum { IDD = IDD_ReportJointMoments };
	CString	mFooter;
	CString	mHeader;
	CString	mMomentUnits;
	CString mHeadMom[3];
    CString mUpperNeckMom[3];
    CString mLowerNeckMom[3];
    CString mC7T1Mom[3];
	CString mL4L5Mom[3];    
    CString	mL5S1Mom[3];
    CString mLeftHeelMom[3];
    CString mLeftBallMom[3];
	CString	mLeftAnkleMom[3];
	CString	mLeftElbowMom[3];
	CString mLeftWristMom[3];
	CString	mLeftHandMom[3];
	CString	mLeftHipMom[3];
	CString	mLeftKneeMom[3];
	CString	mLeftShoulderMom[3];
    CString mRightHeelMom[3];
    CString mRightBallMom[3];
	CString	mRightAnkleMom[3];
	CString	mRightElbowMom[3];
	CString mRightWristMom[3];
	CString	mRightHandMom[3];
	CString	mRightHipMom[3];
	CString	mRightKneeMom[3];
	CString	mRightShoulderMom[3];
	CString	mLeftITMom[3];
    CString mRightITMom[3];

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
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ReportJointMoments)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

//--- Custom member functions ---

public:
	virtual void UpdateUnits();
	virtual void OnInitialUpdate();

   void OnUpdate(CView* aSenderPtr, LPARAM aHint, CObject* aHintPtr);

   static BOOL IsOpen() {
	  return cPointer != NULL;
   }

   static void GetWindowData(WindowData& aWD)
   {
      if(cPointer != NULL)
	  {
	     aWD.mIsOpen = true;
		 WINDOWPLACEMENT wp;
		 cPointer->GetParentFrame()->GetWindowPlacement(&wp);
		 aWD.mRectangle = wp.rcNormalPosition;
	  }
	  else
	     aWD.mIsOpen = false;
   }

   static ReportView* Pointer(){
	   return cPointer;
   }

   virtual CString ReportName() const;

//--- Custom Attributes ---

private:

   static ReportView* cPointer;
   CString* jointToMember[NUM_JOINTS]; // *****33 must be replaced by NUM_JOINTS once the locations of hand joints are being handled properly*****

// Implementation
protected:
	virtual ~ReportJointMoments();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(ReportJointMoments)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDisplayDialogcolor();
	//}}AFX_MSG
	//afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTJOINTMOMENTS_H__000A0C72_3B2D_11D1_9349_000000000000__INCLUDED_)
