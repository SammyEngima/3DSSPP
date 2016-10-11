// Dg_AC_RefreshMode.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "Dg_AC_RefreshMode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_AC_RefreshMode dialog


Dg_AC_RefreshMode::Dg_AC_RefreshMode(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_AC_RefreshMode::IDD, pParent)
{
	//{{AFX_DATA_INIT(Dg_AC_RefreshMode)
	Refresh_Mode = -1;
	//}}AFX_DATA_INIT
}

void Dg_AC_RefreshMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_AC_RefreshMode)
	DDX_Radio(pDX, IDC_AC_MANUAL, Refresh_Mode);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Dg_AC_RefreshMode, CDialog)
	//{{AFX_MSG_MAP(Dg_AC_RefreshMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dg_AC_RefreshMode message handlers


