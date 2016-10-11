#if !defined(AFX_REPORTJOINTFORCES_H__1DAF8300_1BE5_11D5_8EBE_0001031CA2E0__INCLUDED_)
#define AFX_REPORTJOINTFORCES_H__1DAF8300_1BE5_11D5_8EBE_0001031CA2E0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportJointForces.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ReportJointForces form view

#include "stdafx.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ReportView.hpp"
#include "WindowData.hpp"
#include "Skeleton.hpp"
#include "afxwin.h"

#include "StatusBox.h"
#include "LimitScaleCtrl.h"
#include "resource.h"
#include "GLBalanceStanding.h"
#include "GLBalanceSitting.h"

class ReportJointForces : public ReportView
{
protected:
   ReportJointForces();           // protected constructor used by dynamic creation
   DECLARE_DYNCREATE(ReportJointForces)

// Form Data
public:
   //{{AFX_DATA(ReportJointForces)
   enum { IDD = IDD_ReportJointForces };
   CString	mHeader;
   CString	mForceUnits;
   CString	mFooter;
   CString  mHeadForce[3];
   CString  mUpperNeckForce[3];
   CString  mLowerNeckForce[3];
   CString  mC7T1Force[3];
   CString	mL5S1Force[3];
   CString	mL4L5Force[3];
   CString	mLeftBallForce[3];
   CString	mLeftHeelForce[3];
   CString	mLeftAnkleForce[3];
   CString	mLeftWristForce[3];
   CString	mLeftHandForce[3];
   CString	mLeftElbowForce[3];
   CString	mLeftHipForce[3];
   CString	mLeftKneeForce[3];
   CString	mLeftShoulderForce[3];
   CString	mRightBallForce[3];
   CString	mRightHeelForce[3];
   CString	mRightAnkleForce[3];
   CString	mRightWristForce[3];
   CString	mRightHandForce[3];
   CString	mRightElbowForce[3];
   CString	mRightHipForce[3];
   CString	mRightKneeForce[3];
   CString	mRightShoulderForce[3];
   CString  mSeatBack[3];
   CString  mLeftIT[3];
   CString  mRightIT[3];
   CString  mSeatFrontRight[3];
   CString  mSeatFrontLeft[3];

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
   //{{AFX_VIRTUAL(ReportJointForces)
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

   static ReportView* Pointer() {
      return cPointer;
   }

   virtual CString ReportName() const;

//--- Custom Attributes ---
private:
   static ReportView* cPointer;
   CString* jointToMember[NUM_JOINTS]; // *****34 must be replaced by NUM_JOINTS once the locations of hand joints are being handled properly*****

// Implementation
protected:
   virtual ~ReportJointForces();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

   // Generated message map functions
   //{{AFX_MSG(ReportJointForces)
   afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
   afx_msg void OnDisplayDialogcolor();
   //}}AFX_MSG
   //afx_msg void OnFilePrintPreview();
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTJOINTFORCES_H__1DAF8300_1BE5_11D5_8EBE_0001031CA2E0__INCLUDED_)
