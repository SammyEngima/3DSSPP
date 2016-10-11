// ReportTaskInputSummary.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ReportTaskInputSummary form view

#include "StatusBox.h"

#include "stdafx.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ReportView.hpp"
#include "Analysis.h"	// For JointTorqueEnm
#include "WindowData.hpp"

#include "LimitScaleCtrl.h"
#include "resource.h"
#include "GLBalanceStanding.h"
#include "GLBalanceSitting.h"

class ReportTaskInputSummary : public ReportView
{
protected:
	ReportTaskInputSummary();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ReportTaskInputSummary)

// Form Data
public:
	//{{AFX_DATA(ReportTaskInputSummary)
	enum { IDD = IDD_ReportTaskInputSummary };

    CString mLeftClavicleHori;
	CString mLeftClavicleVert;
	CString mRightClavicleHori;
	CString mRightClavicleVert;
	
    CString mLeftFootHori;
    CString mLeftFootVert;
    CString mRightFootHori;
    CString mRightFootVert;

    CString	mHeadBending ;
    CString	mHeadFlexion;
	CString	mHeadRotation;
    CString	mPelvicTilt;

	CString	mLeftForeArmHori;
	CString	mLeftForeArmVert;
	CString	mLeftLowerLegVert;
	CString	mLeftUpperArmHori;
	CString	mLeftUpperArmVert;
	CString	mLeftUpperLegVert;
	CString	mRightForeArmHori;
	CString	mRightForeArmVert;
	CString	mRightLowerLegVert;
	CString	mRightUpperArmHori;
	CString	mRightUpperArmVert;
	CString	mRightUpperLegVert;
	CString	mHandOrientation;
	CString	mLeftHandMag;
	CString	mRightHandMag;
	CString	mTrunkBending;
	CString	mTrunkFlexion;
	CString	mTrunkRotation;
	CString	mLeftHandForceHori;
	CString	mLeftHandForceVert;
	CString	mLeftHandLocLat;
	CString	mLeftHandLocVert;
	CString	mLeftHandLocHori;
	CString	mRightHandForceHori;
	CString	mRightHandForceVert;
	CString	mRightHandLocHori;
	CString	mRightHandLocLat;
	CString	mRightHandLocVert;
	CString	mRightHandForceMag;
	CString	mLeftHandForceMag;
	CString	mFooter;
	CString	mHeader;
	CString	mLeftForceMagUnits;
	CString	mRightForceMagUnits;
	CString	mLocationLabel;
	CString	m_JointLoads;
	CString	m_JointNames;
	CString mForceUnits;
	CString mTorqueUnits;
	CString mLeftWristHori;
	CString mLeftWristVert;
	CString mRightWristHori;
	CString mRightWristVert;
	CString mLeftWristRot;
	CString mRightWristRot;
	CString mLeftUpperlegHori;
	CString mLeftLowerlegHori;
	CString mRightUpperlegHori;
	CString mRightLowerlegHori;
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
	//{{AFX_VIRTUAL(ReportTaskInputSummary)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


//--- Custom member functions ---
private:
	int hack_WindowBottom;	// y coord for bottom of window, because it seems to not use normal units...
	int hack_FooterTop;		// y coord for the beginning of the footer

public:
	virtual void UpdateUnits();

   void OnUpdate(	CView* aSenderPtr,
						LPARAM aHint,
						CObject* aHintPtr	);

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
// JTK - August 16, 2004 No Longer used
//void CalculateTorqueParameters(JointTorqueEnm joint, CString& mHandTorqueHori, CString& mHandTorqueVert, CString& mHandTorqueMag, CString& mHandTorqueUnits);
private:
	void UpdateDynamicSummary(void);
   static
   ReportView *
cPointer;

// Implementation
protected:
	virtual ~ReportTaskInputSummary();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(ReportTaskInputSummary)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDisplayDialogcolor();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	//afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
public:
	CString mPelvicAxial;
};

/////////////////////////////////////////////////////////////////////////////
