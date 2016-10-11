// ReportStrength.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ReportStrength form view
#ifndef ReportStrengthMcr
#define ReportStrengthMcr

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

class ReportStrength : public ReportView
{
protected:
	ReportStrength();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ReportStrength)

// Form Data
public:
	//{{AFX_DATA(ReportStrength)
	enum { IDD = IDD_ReportStrength };
	CString	mFooter;
	CString	mHeader;

	CString	mLeftMeanLabel;
	CString	mLeftMomentLabel;
	CString	mLeftSDLabel;
	CString	mRightMeanLabel;
	CString	mRightMomentLabel;
	CString	mRightSDLabel;

	CString mLeftWristFlex[5];
	CString mLeftWristDev[5];
	CString mLeftWristRot[5];
	CString mRightWristFlex[5];
	CString mRightWristDev[5];
	CString mRightWristRot[5];

    CString	mLeftAbAd[5];
	CString	mLeftAnkle[5];
	CString	mLeftElbow[5];
	CString	mLeftFlexExt[5];
	CString	mNeckFlexExt[5];
	CString	mNeckLatBend[5];
	CString	mNeckRotation[5];
	CString	mLeftHip[5];
	CString	mLeftHumRot[5];
	CString	mLeftKnee[5];
	CString	mLeftLatBend[5];
	CString	mLeftRotation[5];
	CString	mLeftRotBkFd[5];
	CString	mRightAbAd[5];
	CString	mRightAnkle[5];
	CString	mRightElbow[5];
	CString	mRightHip[5];
	CString	mRightHumRot[5];
	CString	mRightKnee[5];
	CString	mRightRotBkFd[5];

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
	//{{AFX_VIRTUAL(ReportStrength)
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

   void
ReportLine
(
   Analysis &  aResult ,
   int         aIndex ,
   CString &   aMoment ,
   CString &   aEffect ,
   CString &   aMean ,
   CString &   aSD ,
   CString &   aCap
);

//--- Custom Attributes ---

private:

   static
   ReportView *
cPointer;

   CString* hingeToMember[NUM_STRENGTH_HINGES];

// Implementation
protected:
	virtual ~ReportStrength();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(ReportStrength)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDisplayDialogcolor();
	//}}AFX_MSG
	//afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};
#endif
/////////////////////////////////////////////////////////////////////////////
