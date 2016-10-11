// ReportShoulderGeometry.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ReportShoulderGeometry form view
#ifndef REPORTSHOULDERGEOMETRY
#define REPORTSHOULDERGEOMETRY
#include "stdafx.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ReportView.hpp"
#include "WindowData.hpp"
#include "ShoulderModel.h"
#include "afxcmn.h"
#include "Types.h"
#if defined(SHOULDERDLL)
#include "libfmincon.h"
#endif

// ReportShoulderGeometry form view

class ReportShoulderGeometry : public ReportView
{
protected:
	DECLARE_DYNCREATE(ReportShoulderGeometry)
	ReportShoulderGeometry();           // protected constructor used by dynamic creation
	ShoulderModel shoModRight;
    ShoulderModel shoModLeft;
public:
	enum { IDD = IDD_ReportShoulderGeometry };
	CString	mHeader;
	CString	mFooter;

	CString m1stThoracicVertebraS[3];
    CString m12thThoracicVertebraI[3];
	CString mLeftSternoclavicularJoint[3];
	CString mRightSternoclavicularJoint[3];
	CString m1stThoracicVertebraM[3];
	CString mRightACJ[3];
	CString mLeftACJ[3];

	CString mLeftScapulaAngle[3];
	CString mRightScapulaAngle[3];
	CString mLeftScapularSpine[3];
	CString mRightScapularSpine[3];
	CString mLeftHumeralHead[3];			// Glenhumeral head represented by the center of humeral head. Refer to Richard's dissertation. 
	CString mRightHumeralHead[3];			// Glenhumeral head represented by the center of humeral head. Refer to Richard's dissertation. 
	CString mLeftElbowJoint[3];
	CString mRightElbowJoint[3];
	CString mLeftWristJoint[3];
	CString mRightWristJoint[3];
	CString mLeftSJCTJoint[3];
	CString mRightSJCTJoint[3];
	CString mLeftACRTJoint[3];
	CString mRightACRTJoint[3];
	CString mLeftC7T1Joint[3];
	CString mRightC7T1Joint[3];
	CString mLeftL5S1Joint[3];
	CString mRightL5S1Joint[3];
    CProgressCtrl mProgressControl;

	CString mLocationUnits;
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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//--- Custom member functions ---

public:

void OnUpdate
( 
   CView *   aSenderPtr, 
   LPARAM    aHint, 
   CObject * aHintPtr
);
inline static BOOL IsOpen ( void )
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

private:
    static ReportView* cPointer;
	CString* hingeToMember[NUM_GEOMETRY_HINGES];
	void ClearValues();
	bool clear;

protected:
	virtual ~ReportShoulderGeometry();

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg virtual void OnDisplayDialogcolor();

	void updateShoulderData();
    void setMuscleLables();
	DECLARE_MESSAGE_MAP()


public:
	afx_msg virtual void OnBnClickedSabtn();

	virtual void OnInitialUpdate();
	static void killMatlab();

	virtual void UpdateUnits();
};


#endif
/////////////////////////////////////////////////////////////////////////////