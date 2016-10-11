#pragma once
#include "resource.h"
#include "afxwin.h"

// Dg_PercentileLimits dialog

class Dg_PercentileLimits : public CDialog
{
	DECLARE_DYNAMIC(Dg_PercentileLimits)

public:
	Dg_PercentileLimits(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dg_PercentileLimits();

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void UpdateDocument();
	void DisableFieldsAsNecessary();
	DECLARE_MESSAGE_MAP()
public:
	BOOL radioChoice;
	int lowerPercentileLimit;
	int upperPercentileLimit;
	int femaleLowerPercentileLimit;
	int femaleUpperPercentileLimit;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedRadioNiosh();
	afx_msg void OnBnClickedRadioCustom();
	// afx_msg void OnBnDoubleclickedRadioNiosh();
	CButton m;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeLowerLimit();
	afx_msg void OnEnChangeUpperLimit();
	afx_msg void OnEnChangeLowerLimitFemale();
	afx_msg void OnEnChangeUpperLimitFemale();
	afx_msg void OnEnSetfocusLowerLimit();
	afx_msg void OnEnSetfocusUpperLimit();
	afx_msg void OnEnSetfocusLowerLimitFemale();
	afx_msg void OnEnSetfocusUpperLimitFemale();
};
