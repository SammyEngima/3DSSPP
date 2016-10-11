#if !defined(AFX_HWMEANS_H__649AFC60_F6D6_11D4_8EBE_0001031CA2E0__INCLUDED_)
#define AFX_HWMEANS_H__649AFC60_F6D6_11D4_8EBE_0001031CA2E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dg_HWMeans.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_HWMeans dialog

class Dg_HWMeans : public CDialog
{
// Construction
	friend class Dg_PopulationEdit;
public:
	Dg_HWMeans(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_HWMeans)
	enum { IDD = IDD_HW_MEANS };
	float	mFHmean;
	float	mFWmean;
	float	mMWmean;
	float	mMHmean;
	float	mFHMax;
	float	mFHMin;
	float	mFWMax;
	float	mFWMin;
	float	mMHMax;
	float	mMHMin;
	float	mMWMax;
	float	mMWMin;
	//}}AFX_DATA
	float	mFHstdev;
	float	mFWstdev;
	float	mMHstdev;
	float	mMWstdev;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Dg_HWMeans)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(Dg_HWMeans)
	afx_msg void OnValueChanged();
	afx_msg void OnKillfocusFHstdev();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//float *weight, *stature;
public:
	void RecalculatePercentiles();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HWMEANS_H__649AFC60_F6D6_11D4_8EBE_0001031CA2E0__INCLUDED_)
