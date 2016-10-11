// Dg_FactorsPassword.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "Dg_FactorsPassword.h"
#include ".\dg_factorspassword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_FactorsPassword dialog


Dg_FactorsPassword::Dg_FactorsPassword(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_FactorsPassword::IDD, pParent)
{
	//{{AFX_DATA_INIT(Dg_FactorsPassword)
	mPasswd = _T("");
	//}}AFX_DATA_INIT
}


void Dg_FactorsPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_FactorsPassword)
	DDX_Text(pDX, IDC_FPASSWD, mPasswd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Dg_FactorsPassword, CDialog)
	//{{AFX_MSG_MAP(Dg_FactorsPassword)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_FactorsPassword message handlers
