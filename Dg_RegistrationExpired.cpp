// Dg_RegistrationExpired.cpp : implementation file
//

#include "stdafx.h"
#include "Dg_RegistrationExpired.h"

// function type declaration for softwarepassport key install function
typedef bool (__stdcall *InstallKeyFn)(const char *name, const char *code);

// Dg_RegistrationExpired dialog

IMPLEMENT_DYNAMIC(Dg_RegistrationExpired, CDialog)

Dg_RegistrationExpired::Dg_RegistrationExpired(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_RegistrationExpired::IDD, pParent)
	, RegName(_T(""))
	, RegKey(_T(""))
{

}

Dg_RegistrationExpired::~Dg_RegistrationExpired()
{
}

void Dg_RegistrationExpired::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, RegName);
	DDX_Text(pDX, IDC_EDIT2, RegKey);
}


BEGIN_MESSAGE_MAP(Dg_RegistrationExpired, CDialog)
END_MESSAGE_MAP()


// Dg_RegistrationExpired message handlers

void Dg_RegistrationExpired::OnOK()
{
	UpdateData(true);

	HINSTANCE libInst = LoadLibrary("ArmAccess.DLL");
	if(!libInst) {
		AfxMessageBox("Unable to load registration dll"); /* Couldn't load library */
		return;
	}

	// get install key function
	InstallKeyFn InstallKey = (InstallKeyFn)GetProcAddress(libInst, "InstallKey");
	int returnvalue;
	if(InstallKey==0) {
		AfxMessageBox("Unable to find registration function");
		return;/* Couldn't find function */
	} else {
		if(!InstallKey(RegName, RegKey)) {
			AfxMessageBox("Name and Registration Key do not appear to be valid."
				" Please enter the name and key exactly as they were given to you");
			return;
		}
	}

	CDialog::OnOK();
}
