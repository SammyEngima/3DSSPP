#include "afxwin.h"
#if !defined(AFX_DGREPORTSPRINT_H__45EC03E1_EC4A_11D0_92C2_000000000000__INCLUDED_)
#define AFX_DGREPORTSPRINT_H__45EC03E1_EC4A_11D0_92C2_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Dg_PrintReports.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_PrintReports dialog

class Dg_PrintReports : public CDialog
{
// Construction
public:
	
	Dg_PrintReports(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_PrintReports)
	enum { IDD = IDD_ReportsPrint };
	BOOL	mReportAnalysisSummary;
	BOOL	mReportAnthropometry;
	BOOL    mReportBalanceStanding;
	BOOL	mReportFatigue;
	BOOL	mReportJointLocations;
	BOOL	mReportCoMLocations;
	BOOL	mReportSagittal;
	BOOL	mReportSpinal;
	BOOL	mReportStrength;
	BOOL	mReportTaskInputSummary;
	BOOL	mReportLowBack;
    BOOL    mReportShoulderAnalysis;
    BOOL 	mReportShoulderGeometry;
	BOOL	mReportPosture;
	BOOL	mReportJointMoments;
	BOOL	mReportStrengthDirVectors;
	BOOL	mReportBalanceSeated;
	BOOL	mReportJointForces;
	BOOL	mReportBalance;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Dg_PrintReports)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Dg_PrintReports)
	afx_msg void OnSelectAllButton();
	afx_msg void OnSelectNoneButton();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnOpenReportsButton();
   CButton mReportBalanceSeatedCheck;
   CButton mReportBalanceStandingCheck;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGREPORTSPRINT_H__45EC03E1_EC4A_11D0_92C2_000000000000__INCLUDED_)
