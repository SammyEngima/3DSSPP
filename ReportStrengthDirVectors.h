#if !defined(AFX_REPORTSTRENGTHDIRVECTORS_H__000A0C73_3B2D_11D1_9349_000000000000__INCLUDED_)
#define AFX_REPORTSTRENGTHDIRVECTORS_H__000A0C73_3B2D_11D1_9349_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ReportStrengthDirVectors.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ReportStrengthDirVectors form view

#include "stdafx.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ReportView.hpp"
#include "WindowData.hpp"
#include "Types.h"

#include "StatusBox.h"
#include "LimitScaleCtrl.h"
#include "resource.h"
#include "GLBalanceStanding.h"
#include "GLBalanceSitting.h"

class Analysis;

class ReportStrengthDirVectors : public ReportView
{
protected:
	ReportStrengthDirVectors();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ReportStrengthDirVectors)

// Form Data
public:
	//{{AFX_DATA(ReportStrengthDirVectors)
	enum { IDD = IDD_ReportStrengthDirVectors };
	CString	mFooter;
	CString	mHeader;
	CString mWristFlexL[3];
	CString mWristFlexR[3];
	CString mWristDevL[3];
	CString mWristDevR[3];
	CString mWristRotL[3];
	CString mWristRotR[3];
	CString	mElbowL[3];
	CString	mElbowR[3];
	CString	mHipFlexL[3];
	CString	mHipFlexR[3];
	CString	mKneeFlexL[3];
	CString	mKneeFlexR[3];
	CString	mNeckBendL[3];
	CString	mNeckFlexL[3];
	CString	mNeckRotL[3];
	CString	mL5S1BendL[3];
	CString	mL5S1FlexL[3];
	CString	mL5S1RotL[3];
	CString	mShoulderAddL[3];
	CString	mShoulderAddR[3];
	CString	mShoulderHumL[3];
	CString	mShoulderHumR[3];
	CString	mShoulderRotL[3];
	CString	mShoulderRotR[3];
	CString	mAnkleFlexL[3];
	CString	mAnkleFlexR[3];

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
	//{{AFX_VIRTUAL(ReportStrengthDirVectors)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

//--- Custom member functions ---

public:
	virtual void UpdateUnits();
	virtual void OnInitialUpdate();

   void
OnUpdate
( 
   CView *   aSenderPtr, 
   LPARAM    aHint, 
   CObject * aHintPtr
);

   inline
   static
   BOOL
IsOpen
( void )
{ return cPointer != NULL; }

static void GetWindowData( WindowData& aWD )
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

   inline
   static
   ReportView *
Pointer
( void )
{ return cPointer; }

   virtual
   CString
ReportName
( void ) 
const;

//--- Custom Attributes ---

private:

   static
   ReportView *
cPointer;

// Implementation
protected:
	virtual ~ReportStrengthDirVectors();
	// map from strength hinges to the member of this report which show them
	CString* hingeToMember[NUM_STRENGTH_HINGES];
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(ReportStrengthDirVectors)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDisplayDialogcolor();
	//}}AFX_MSG
	//afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTSTRENGTHDIRVECTORS_H__000A0C73_3B2D_11D1_9349_000000000000__INCLUDED_)
