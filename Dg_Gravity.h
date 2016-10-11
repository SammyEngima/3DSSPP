#if !defined(AFX_GRAVITY_H__30069A80_BFD7_11D4_8EBE_0001031CA2E0__INCLUDED_)
#define AFX_GRAVITY_H__30069A80_BFD7_11D4_8EBE_0001031CA2E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dg_Gravity.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_Gravity dialog

class Dg_Gravity : public CDialog
{
// Construction
public:
	Dg_Gravity(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_Gravity)
	enum { IDD = IDD_GRAVITY };
	double	m_GravValue;
	CString	m_GravList;
	CString	m_accel_units;
	int		m_ComboBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Dg_Gravity)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL UpdateCombobox(BOOL bSaveAndValidate);
	// Generated message map functions
	//{{AFX_MSG(Dg_Gravity)
	afx_msg void OnEditchangeCombo1();
	afx_msg void OnChangeEdit1();
	afx_msg void OnApply();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPrintPrintwindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAVITY_H__30069A80_BFD7_11D4_8EBE_0001031CA2E0__INCLUDED_)
