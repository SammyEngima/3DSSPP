// ReportShoulderAnalysis.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ReportShoulderAnalysis form view
#ifndef ReportShoulderAnalysisMcr
#define ReportShoulderAnalysisMcr

#include "stdafx.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ReportView.hpp"
#include "WindowData.hpp"
#include "ShoulderModel.h"
#include "afxcmn.h"


class Analysis;


class ReportShoulderAnalysis : public ReportView
{
protected:
    ShoulderModel shoModRight;
    ShoulderModel shoModLeft;
	ReportShoulderAnalysis();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ReportShoulderAnalysis)

// Form Data
public:
	//{{AFX_DATA(ReportShoulderAnalysis)
	enum { IDD = IDD_ReportShoulderAnalysis };
	CString	mHeader;
	CString	mFooter;
    
    CString mSTorqueLabel;
    CString mSForceLabel;
	CString mETorqueLabel;

    CString mSTorqueR[3];	// right shoulder torque
    CString mSForceR[3];	// right shoulder force
	CString mETorqueR[3];	// right elbow torque

    CString mSTorqueL[3];	// left shoulder torque
    CString mSForceL[3];	// left shoulder force
	CString mETorqueL[3];	// left elbow torque

    CString mSXL[38];		// lables

    CString mSXF[38];	// muscle force, 0-37: right side; 38-75: left side
    CString mSXP[38];	// muscle percents, 0-37: right side; 38-75: left side
    CString mFUnits;
	
	CString	mForcesLabel;

	// added for additional shoulder muscle force information
	CString mSXAL[22];	// additional shoulder muscle information label
	CString mSXA[22];	// additional shoulder muscle information

	///Yijun
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
	CString mMFPT;
	CString mMFPAIT;
	CString mShdrOutSide;
	CString mShowSide;

	//}}AFX_DATA

// Attributes

// Operations

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ReportShoulderAnalysis)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

//--- Custom member functions ---

public:

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

   static bool library_loaded;

//--- Custom Attributes ---

private:
   static ReportView* cPointer;
   void ClearValues();
   bool clear;
   bool right_side; 

// Implementation
protected:
	virtual ~ReportShoulderAnalysis();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(ReportShoulderAnalysis)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDisplayDialogcolor();
	//}}AFX_MSG
	
	//afx_msg void OnFilePrintPreview();
    void updateShoulderData();
    void setMuscleLables();
	virtual void UpdateUnits();

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedSabtn();
	virtual void OnInitialUpdate();
	

	static void killMatlab();
	CString ShoulderMessage;
	CProgressCtrl mProgressControl;

	//virtual void UpdateUnits();
	afx_msg void OnBnClickedButtonSide();
};
#endif
/////////////////////////////////////////////////////////////////////////////
