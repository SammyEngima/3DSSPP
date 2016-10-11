#if !defined(AFX_CDGLICENSE_H__015BF03B_43B8_47E4_945A_AE24AD17849B__INCLUDED_)
#define AFX_CDGLICENSE_H__015BF03B_43B8_47E4_945A_AE24AD17849B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dg_License.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_License dialog

class Dg_License : public CDialog
{
// Construction
public:
	Dg_License(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_License)
	enum { IDD = IDD_LICENSE };
	CString	mLicense;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Dg_License)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Dg_License)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDGLICENSE_H__015BF03B_43B8_47E4_945A_AE24AD17849B__INCLUDED_)
