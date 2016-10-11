#ifndef REPORTFATIGUE_H
#define REPORTFATIGUE_H

#include "stdafx.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ReportView.hpp"
#include "resource.h"
#include "WindowData.hpp"
#include "Types.h"

#include "StatusBox.h"
#include "LimitScaleCtrl.h"
#include "resource.h"
#include "GLBalanceStanding.h"
#include "GLBalanceSitting.h"
class Analysis;

// ReportFatigue form view

class ReportFatigue : public ReportView
{
	DECLARE_DYNCREATE(ReportFatigue)

protected:
	ReportFatigue();           // protected constructor used by dynamic creation
	virtual ~ReportFatigue();

public:
	enum { IDD = IDD_ReportFatigue };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString	mFooter;
	CString	mHeader;
	//for average
	CString mLeftForearmRot[3];
	CString mLeftWristFlex[3];
	CString mLeftWristDev[3];
	CString	mLeftAbAd[3];
	CString	mLeftAnkle[3];
	CString	mLeftElbow[3];
	CString mNeckFlexExt[3];
	CString	mLeftFlexExt[3];
	CString	mLeftHip[3];
	CString	mLeftHumRot[3];
	CString	mLeftKnee[3];
	CString	mNeckLatBend[3];
	CString	mLeftLatBend[3];
	CString	mNeckRotation[3];
	CString	mLeftRotation[3];
	CString	mLeftRotBkFd[3];

	CString mRightForearmRot[3];
	CString mRightWristFlex[3];
	CString mRightWristDev[3];
	CString	mRightAbAd[3];
	CString	mRightAnkle[3];
	CString	mRightElbow[3];
	CString	mRightHip[3];
	CString	mRightHumRot[3];
	CString	mRightKnee[3];
	CString	mRightRotBkFd[3];

	// For the acceptabe duty cycle limit 
	CString mLeftForearmRotDC[3];
	CString mLeftWristFlexDC[3];
	CString mLeftWristDevDC[3];
	CString	mLeftAbAdDC[3];
	CString	mLeftAnkleDC[3];
	CString	mLeftElbowDC[3];
	CString mNeckFlexExtDC[3];
	CString	mLeftFlexExtDC[3];
	CString	mLeftHipDC[3];
	CString	mLeftHumRotDC[3];
	CString	mLeftKneeDC[3];
	CString	mNeckLatBendDC[3];
	CString	mLeftLatBendDC[3];
	CString	mNeckRotationDC[3];
	CString	mLeftRotationDC[3];
	CString	mLeftRotBkFdDC[3];

	CString mRightForearmRotDC[3];
	CString mRightWristFlexDC[3];
	CString mRightWristDevDC[3];
	CString	mRightAbAdDC[3];
	CString	mRightAnkleDC[3];
	CString	mRightElbowDC[3];
	CString	mRightHipDC[3];
	CString	mRightHumRotDC[3];
	CString	mRightKneeDC[3];
	CString	mRightRotBkFdDC[3];

	//MAX TIME(s)
	CString mLeftForearmRotMT[3];
	CString mLeftWristFlexMT[3];
	CString mLeftWristDevMT[3];
	CString	mLeftAbAdMT[3];
	CString	mLeftAnkleMT[3];
	CString	mLeftElbowMT[3];
	CString mNeckFlexExtMT[3];
	CString	mLeftFlexExtMT[3];
	CString	mLeftHipMT[3];
	CString	mLeftHumRotMT[3];
	CString	mLeftKneeMT[3];
	CString	mNeckLatBendMT[3];
	CString	mLeftLatBendMT[3];
	CString	mNeckRotationMT[3];
	CString	mLeftRotationMT[3];
	CString	mLeftRotBkFdMT[3];

	CString mRightForearmRotMT[3];
	CString mRightWristFlexMT[3];
	CString mRightWristDevMT[3];
	CString	mRightAbAdMT[3];
	CString	mRightAnkleMT[3];
	CString	mRightElbowMT[3];
	CString	mRightHipMT[3];
	CString	mRightHumRotMT[3];
	CString	mRightKneeMT[3];
	CString	mRightRotBkFdMT[3];

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


	//Exertion per cycle

	double mTCT;
	double mEC;
	double mED;

	CString mEDC_text;

	double mEDC;

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

   static ReportView* cPointer;

   CString* hingeToMember[NUM_STRENGTH_HINGES];
   CString* hingeToMemberDC[NUM_STRENGTH_HINGES];
   CString* hingeToMemberMT[NUM_STRENGTH_HINGES];

	// Generated message map functions
	//{{AFX_MSG(ReportStrength)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDisplayDialogcolor();
	//afx_msg void OnFilePrintPreview();
	//}}AFX_MSG
};


#endif