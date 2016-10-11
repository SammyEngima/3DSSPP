#if !defined(AFX_Dg_FactorsPassword_H__ABD6FF2D_9A42_11D4_8EBE_0001031CA2E0__INCLUDED_)
#define AFX_Dg_FactorsPassword_H__ABD6FF2D_9A42_11D4_8EBE_0001031CA2E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dg_FactorsPassword.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_FactorsPassword dialog

class Dg_FactorsPassword : public CDialog
{
// Construction
public:
	Dg_FactorsPassword(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_FactorsPassword)
	enum { IDD = IDD_FACTORS_PASSWORD };
	CString	mPasswd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Dg_FactorsPassword)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Dg_FactorsPassword)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Dg_FactorsPassword_H__ABD6FF2D_9A42_11D4_8EBE_0001031CA2E0__INCLUDED_)
