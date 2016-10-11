// Dg_NewRegistration.cpp : implementation file
//

#include "stdafx.h"
#include "Dg_NewRegistration.h"


// function type declaration for softwarepassport key install function
typedef bool (__stdcall *InstallKeyFn)(const char *name, const char *code);

// Dg_NewRegistration dialog

IMPLEMENT_DYNAMIC(Dg_NewRegistration, CDialog)

Dg_NewRegistration::Dg_NewRegistration(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_NewRegistration::IDD, pParent)
	, RegName(_T(""))
	, RegKey(_T(""))
	, TrialText(_T(""))
{

}

Dg_NewRegistration::~Dg_NewRegistration()
{
}

void Dg_NewRegistration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, RegName);
	DDX_Text(pDX, IDC_EDIT2, RegKey);
	DDX_Text(pDX, IDC_TRIALTEXT, TrialText);
}


BEGIN_MESSAGE_MAP(Dg_NewRegistration, CDialog)
END_MESSAGE_MAP()


// Dg_NewRegistration message handlers

void Dg_NewRegistration::OnOK()
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

BOOL Dg_NewRegistration::OnInitDialog()
{
	CDialog::OnInitDialog();

	char daysleft[1024];
	if(GetEnvironmentVariable("DAYSLEFT", daysleft, 1023)) {
		TrialText = CString("Enter the name and registration key exactly as they were given to you."
							" Press Cancel to continue using the evaluation period.  You have ") +
					daysleft +
					CString(" days left.");
	}

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
