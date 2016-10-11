#pragma once

#include "resource.h"

// Dg_RegistrationExpired dialog

class Dg_RegistrationExpired : public CDialog
{
	DECLARE_DYNAMIC(Dg_RegistrationExpired)

public:
	Dg_RegistrationExpired(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dg_RegistrationExpired();

// Dialog Data
	enum { IDD = IDD_REGISTRATION_EXPIRED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString RegName;
	CString RegKey;
protected:
	virtual void OnOK();
};
