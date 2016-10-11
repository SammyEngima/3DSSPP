#if !defined(AFX_DGPARAMETERS_H__45176101_EEA0_11D0_92C8_000000000000__INCLUDED_)
#define AFX_DGPARAMETERS_H__45176101_EEA0_11D0_92C8_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Dg_Parameters.h : header file
//

#include "Parameters.hpp"

/////////////////////////////////////////////////////////////////////////////
// Dg_Parameters dialog

class Dg_Parameters : public CDialog
{
// Construction
public:
	Dg_Parameters(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_Parameters)
	enum { IDD = IDD_PARAMETERS };
	CString	mParameterSetName;
	//}}AFX_DATA

   Parameters   mParameters;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Dg_Parameters)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Dg_Parameters)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGPARAMETERS_H__45176101_EEA0_11D0_92C8_000000000000__INCLUDED_)
