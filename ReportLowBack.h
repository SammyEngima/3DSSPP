// ReportLowBack.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ReportLowBack form view
#ifndef ReportLowBackMcr
#define ReportLowBackMcr

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

class ReportLowBack : public ReportView
{
protected:
	ReportLowBack();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ReportLowBack)

// Form Data
public:
	//{{AFX_DATA(ReportLowBack)
	enum { IDD = IDD_ReportLowBack };
	CString	mHeader;
	CString	mFooter;
	CString	mArmsLabel;
	CString	mCompressionLabel;
	CString	mForcesLabel;
	CString	mShearLabel;
	CString	mDiscCompression;
	CString	mDiscAntPostShear;
	CString	mDiscLateralShear;
	CString	mDiscShear;

	CString	mLEOArm[2];
    CString	mLESArm[2];
    CString	mLIOArm[2];
    CString	mLLDArm[2];
    CString	mLRAArm[2];
    CString	mREOArm[2];
    CString	mRESArm[2];
    CString	mRIOArm[2];
    CString	mRLDArm[2];
    CString	mRRAArm[2];

	CString	mLEOForce[5];
    CString	mLESForce[5];
    CString	mLIOForce[5];
    CString	mLLDForce[5];
    CString	mLRAForce[5];
    CString	mREOForce[5];
    CString	mRESForce[5];
    CString	mRIOForce[5];
    CString	mRLDForce[5];
    CString	mRRAForce[5];

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
	//{{AFX_VIRTUAL(ReportLowBack)
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
   CString* muscleToMember[NUM_BACK_MUSCLES];
   CString* armToMember[NUM_BACK_MUSCLES];

// Implementation
protected:
	virtual ~ReportLowBack();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(ReportLowBack)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDisplayDialogcolor();
	//}}AFX_MSG
	
	//afx_msg void OnFilePrintPreview();

	DECLARE_MESSAGE_MAP()
};
#endif
/////////////////////////////////////////////////////////////////////////////
