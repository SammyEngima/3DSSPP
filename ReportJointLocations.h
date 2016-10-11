// ReportJointLocations.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ReportJointLocations form view
#ifndef REPORTJOINTLOCATIONS
#define REPORTJOINTLOCATIONS
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

class ReportJointLocations : public ReportView
{
protected:
	ReportJointLocations();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ReportJointLocations)

// Form Data
public:
	//{{AFX_DATA(ReportJointLocations)
	enum { IDD = IDD_ReportJointLocations };
	CString	mFooter;
	CString	mHeader;

	CString	mLeftHandLoc[3];
	CString	mRightHandLoc[3];
	CString mLeftWristLoc[3];
	CString mRightWristLoc[3];
	CString	mLeftElbowLoc[3];
	CString	mRightElbowLoc[3];
	CString	mLeftShoulderLoc[3];
	CString	mRightShoulderLoc[3];

	CString mTragion[3];
	CString mNasion[3];
	CString mNeckTop[3];
    CString mNeckMid[3];
    CString mC7T1Loc[3];
	CString mSCJLoc[3];
	CString	mL5S1Loc[3];
	CString mHipCenter[3];
	CString mITCenter[3];
	CString mAnkleCenter[3];
	CString mBallCenter[3];

	CString mSeatBack[3];
	CString mSRP[3];
	CString mSeatForward[3];
	CString mRightForSeat[3];
	CString mLeftForSeat[3];

	CString	mLeftHipLoc[3];
	CString	mRightHipLoc[3];
	CString	mLeftITLoc[3];
	CString	mRightITLoc[3];
	CString	mLeftKneeLoc[3];
	CString	mRightKneeLoc[3];
	CString	mLeftAnkleLoc[3];
	CString	mRightAnkleLoc[3];
	CString mLeftHeelLoc[3];
	CString mRightHeelLoc[3];
	CString mLeftFootCenter[3];
	CString mRightFootCenter[3];
	CString mLeftBallLoc[3];
	CString mRightBallLoc[3];

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
	//{{AFX_VIRTUAL(ReportJointLocations)
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

   CString* jointToMember[NUM_JOINTS];    // *****33 must be replaced by NUM_JOINTS once the locations of hand joints are being handled properly*****

// Implementation
protected:
	virtual ~ReportJointLocations();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(ReportJointLocations)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDisplayDialogcolor();
	//}}AFX_MSG
	//afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif