#if !defined(AFX_Dg_CameraOrtho_H__8092AACA_AC1F_4BC8_A7AD_368C2E7837AB__INCLUDED_)
#define AFX_Dg_CameraOrtho_H__8092AACA_AC1F_4BC8_A7AD_368C2E7837AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dg_CameraOrtho.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_CameraOrtho dialog

class Dg_CameraOrtho : public CDialog
{
// Construction
public:
	Dg_CameraOrtho(C_Hom_Doc * aDocPtr, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_CameraOrtho)
	enum { IDD = IDD_CAMERA_ORTHO };
	int		mFrontFocalLength;
	int		mSideFocalLength;
	int		mTopFocalLength;
	//}}AFX_DATA

	C_Hom_Doc *   mDocPtr;
	Difference_Count   mDifs;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Dg_CameraOrtho)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateDocument( void );

	// Generated message map functions
	//{{AFX_MSG(Dg_CameraOrtho)
	virtual void OnOK();
	afx_msg void OnRedrawButton();
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Dg_CameraOrtho_H__8092AACA_AC1F_4BC8_A7AD_368C2E7837AB__INCLUDED_)
