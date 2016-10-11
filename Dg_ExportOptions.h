// Dg_ExportOptions.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_ExportOptions dialog

#ifndef DG_EXPORTOPTIONS_H
#define DG_EXPORTOPTIONS_H

#include "afxwin.h"

class   ExportSummary;

class Dg_ExportOptions : public CDialog
{
// Construction
public:

//	Dg_ExportOptions(CWnd* pParent = NULL);	// standard constructor

   Dg_ExportOptions::
Dg_ExportOptions
(
   ExportSummary &   aExportSummaryRef ,
   CWnd*             aParentPtr
);

// Dialog Data
	//{{AFX_DATA(Dg_ExportOptions)
	enum { IDD = IDD_EXPORT_OPTIONS };
//	int		File_Mode;
//	int		Delimiter_Mode;
//	BOOL	mBodySegmentAngles;
//	BOOL	mHandForces;
//	BOOL	mJointLocations;
//	BOOL	mJointMoments;
//	BOOL	mPostureAngles;
//	BOOL	mStrength;
//	BOOL	mSummaryResults;
//	BOOL	mLowBack;
	//}}AFX_DATA

   ExportSummary &   mExportSummaryRef;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void OnOK( void );

	afx_msg void OnSelectAllButton();
	afx_msg void OnSelectNoneButton();

	// Generated message map functions
	//{{AFX_MSG(Dg_ExportOptions)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()



public:
	afx_msg void OnOpenReportsButton();

	//afx_msg void OnBnClickedExpshouldergeometry();
	//afx_msg void OnBnClickedExpshoulderanalysis();
	afx_msg void OnBnClickedCheck3();
};

#endif