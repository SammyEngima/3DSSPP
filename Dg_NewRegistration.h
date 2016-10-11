#pragma once

#include "resource.h"

// Dg_NewRegistration dialog

class Dg_NewRegistration : public CDialog
{
	DECLARE_DYNAMIC(Dg_NewRegistration)

public:
	Dg_NewRegistration(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dg_NewRegistration();

// Dialog Data
	enum { IDD = IDD_REGISTRATION_NEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString RegName;
	CString RegKey;
	CString TrialText;
protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
};
