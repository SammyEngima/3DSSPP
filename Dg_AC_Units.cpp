// Dg_AC_Units.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "Dg_AC_Units.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_AC_Units dialog


Dg_AC_Units::Dg_AC_Units(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_AC_Units::IDD, pParent)
{
	//{{AFX_DATA_INIT(Dg_AC_Units)
	Units_Mode = -1;
	//}}AFX_DATA_INIT
}

void Dg_AC_Units::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_AC_Units)
	DDX_Radio(pDX, IDC_AC_MM, Units_Mode);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Dg_AC_Units, CDialog)
	//{{AFX_MSG_MAP(Dg_AC_Units)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dg_AC_Units message handlers

