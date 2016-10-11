// ReportSpinal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ReportSpinal form view

#include "stdafx.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ReportView.hpp"
#include "WindowData.hpp"

class ReportSpinal : public ReportView
{
protected:
	ReportSpinal();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ReportSpinal)

// Form Data
public:
	//{{AFX_DATA(ReportSpinal)
	enum { IDD = IDD_ReportSpinal };
	CString	mHeader;
	CString	mFooter;
	CString	mGloForcesLabel;
	CString	mGloMomentsLabel;
	CString	mRotForcesLabel;
	CString	mRotMomentsLabel;
    CString	mGloL2L3For[3];
	CString	mGloL2L3Mom[3];
	CString mGloL3L4For[3];
	CString mGloL3L4Mom[3];
	CString	mGloL4L5For[3];
	CString	mGloL4L5Mom[3];
	CString mGloL5S1For[3];
	CString	mGloL5S1Mom[3];
	CString	mRotL2L3For[3];
	CString	mRotL2L3Mom[3];
	CString	mRotL3L4For[3];
	CString	mRotL3L4Mom[3];
	CString	mRotL4L5For[3];
	CString	mRotL4L5Mom[3];
	CString mRotL5S1For[3];
	CString mRotL5S1Mom[3];
	//CString	mRotL5S1MomZ;
	//}}AFX_DATA

// Attributes

// Operations

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ReportSpinal)
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

//--- Custom Attributes ---

private:

   static
   ReportView *
cPointer;

// Implementation
protected:
	virtual ~ReportSpinal();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(ReportSpinal)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDisplayDialogcolor();
	//}}AFX_MSG
	//afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
