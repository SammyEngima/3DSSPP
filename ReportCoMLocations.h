// ReportCoMLocations.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ReportCoMLocations form view
#ifndef REPORTCOMLOCATIONS
#define REPORTCOMLOCATIONS
#include "stdafx.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ReportView.hpp"
#include "WindowData.hpp"
#include "Skeleton.hpp" // for NUM_JOINTS. if that moves to Types.h, remove this include

#include "StatusBox.h"
#include "LimitScaleCtrl.h"
#include "resource.h"
#include "GLBalanceStanding.h"
#include "GLBalanceSitting.h"

class ReportCoMLocations : public ReportView
{
protected:
	ReportCoMLocations();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ReportCoMLocations)

// Form Data
public:
	//{{AFX_DATA(ReportCoMLocations)
	enum { IDD = IDD_ReportCoMLocations };
	CString	mFooter;
	CString	mHeader;

	CString	mLeftHandCoM[3];
	CString	mRightHandCoM[3];
	CString mLeftLowerArmCoM[3];
	CString mRightLowerArmCoM[3];
	CString mLeftUpperArmCoM[3];
	CString mRightUpperArmCoM[3];
	CString mHeadCoM[3];
	CString mUpperNeckCoM[3];
	CString mLowerNeckCoM[3];
	CString	mTorsoCoM[3];
	CString	mLeftPelvisCoM[3];
	//CString	mRightPelvisCoM[3];
	CString	mLeftUpperLegCoM[3];
	CString	mRightUpperLegCoM[3];
	CString	mLeftLowerLegCoM[3];
	CString	mRightLowerLegCoM[3];
	CString	mLeftFootCoM[3];
	CString	mRightFootCoM[3];


	CString mBodyCoM[3];

	CString	mLocationUnits;

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
	//{{AFX_VIRTUAL(ReportCoMLocations)
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

inline static BOOL IsOpen()
{
	return cPointer != NULL;
}

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

  // CString* jointToMember[34/*NUM_JOINTS*/];    // *****33 must be replaced by NUM_JOINTS once the locations of hand joints are being handled properly*****

// Implementation
protected:
	virtual ~ReportCoMLocations();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(ReportCoMLocations)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDisplayDialogcolor();
	//}}AFX_MSG
	//afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif