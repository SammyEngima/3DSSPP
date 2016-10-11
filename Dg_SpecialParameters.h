#if !defined(AFX_Dg_SpecialParameters_H__322775A0_FB96_11D4_8EBE_0001031CA2E0__INCLUDED_)
#define AFX_Dg_SpecialParameters_H__322775A0_FB96_11D4_8EBE_0001031CA2E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dg_SpecialParameters.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_SpecialParameters dialog

class Dg_SpecialParameters : public CDialog
{
// Construction
public:
	Dg_SpecialParameters( C_Hom_Doc *   aDocPtr ,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_SpecialParameters)
	enum { IDD = IDD_SPECIAL_PARAMS };
	double	mDeltaX;
	double	mDeltaY;
	CString	mUnits1;
	double	mPelvicTilt;
	int		mStance;
	double mRightFootAngle;
	double mLeftFootAngle;
	//}}AFX_DATA
	C_Hom_Doc *   mDocPtr;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Dg_SpecialParameters)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateDocument();
	// Generated message map functions
	//{{AFX_MSG(Dg_SpecialParameters)
	virtual void OnOK();
	afx_msg void OnStatic();
	afx_msg void OnRedraw();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStance();
	afx_msg void OnBnClickedRadio2();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Dg_SpecialParameters_H__322775A0_FB96_11D4_8EBE_0001031CA2E0__INCLUDED_)
