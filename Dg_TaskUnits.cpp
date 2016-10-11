// dgtsunit.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "dgtsunit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C_Dia_Task_Units dialog


C_Dia_Task_Units::C_Dia_Task_Units(CWnd* pParent /*=NULL*/)
	: CDialog(C_Dia_Task_Units::IDD, pParent)
{
	//{{AFX_DATA_INIT(C_Dia_Task_Units)
	Units = -1;
	//}}AFX_DATA_INIT
}

void C_Dia_Task_Units::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C_Dia_Task_Units)
	DDX_Radio(pDX, IDC_UNITS, Units);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(C_Dia_Task_Units, CDialog)
	//{{AFX_MSG_MAP(C_Dia_Task_Units)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// C_Dia_Task_Units message handlers


