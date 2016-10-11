// Dg_AC_Zone.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
//#include "hom_doc.h"	// Unnecessary!
#include "Dg_AC_Zone.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_AC_Zone dialog


Dg_AC_Zone::Dg_AC_Zone(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_AC_Zone::IDD, pParent)
{
	//{{AFX_DATA_INIT(Dg_AC_Zone)
	ZRX_0 = 0;
	ZRX_1 = 0;
	ZRX_2 = 0;
	ZRX_3 = 0;
	ZRX_4 = 0;
	ZRX_5 = 0;
	ZRY_0 = 0;
	ZRY_1 = 0;
	ZRY_2 = 0;
	ZRY_3 = 0;
	ZRY_4 = 0;
	ZRY_5 = 0;
	ZRZ_0 = 0;
	ZRZ_1 = 0;
	ZRZ_2 = 0;
	ZRZ_3 = 0;
	ZRZ_4 = 0;
	ZRZ_5 = 0;
	ZTX_0 = 0.0F;
	ZTX_1 = 0.0F;
	ZTX_2 = 0.0F;
	ZTX_3 = 0.0F;
	ZTX_4 = 0.0F;
	ZTX_5 = 0.0F;
	ZTY_0 = 0.0F;
	ZTY_1 = 0.0F;
	ZTY_2 = 0.0F;
	ZTY_3 = 0.0F;
	ZTY_4 = 0.0F;
	ZTY_5 = 0.0F;
	ZTZ_0 = 0.0F;
	ZTZ_1 = 0.0F;
	ZTZ_2 = 0.0F;
	ZTZ_3 = 0.0F;
	ZTZ_4 = 0.0F;
	ZTZ_5 = 0.0F;
	ZName_0 = "";
	ZName_1 = "";
	ZName_2 = "";
	ZName_3 = "";
	ZName_4 = "";
	ZName_5 = "";
	Units_Label = "";
	//}}AFX_DATA_INIT
}

void Dg_AC_Zone::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_AC_Zone)
	DDX_Text(pDX, IDC_ZRX_0, ZRX_0);
	DDV_MinMaxInt(pDX, ZRX_0, -180, 180);
	DDX_Text(pDX, IDC_ZRX_1, ZRX_1);
	DDV_MinMaxInt(pDX, ZRX_1, -180, 180);
	DDX_Text(pDX, IDC_ZRX_2, ZRX_2);
	DDV_MinMaxInt(pDX, ZRX_2, -180, 180);
	DDX_Text(pDX, IDC_ZRX_3, ZRX_3);
	DDX_Text(pDX, IDC_ZRX_4, ZRX_4);
	DDX_Text(pDX, IDC_ZRX_5, ZRX_5);
	DDX_Text(pDX, IDC_ZRY_0, ZRY_0);
	DDV_MinMaxInt(pDX, ZRY_0, -180, 180);
	DDX_Text(pDX, IDC_ZRY_1, ZRY_1);
	DDV_MinMaxInt(pDX, ZRY_1, -180, 180);
	DDX_Text(pDX, IDC_ZRY_2, ZRY_2);
	DDV_MinMaxInt(pDX, ZRY_2, -180, 180);
	DDX_Text(pDX, IDC_ZRY_3, ZRY_3);
	DDX_Text(pDX, IDC_ZRY_4, ZRY_4);
	DDX_Text(pDX, IDC_ZRY_5, ZRY_5);
	DDX_Text(pDX, IDC_ZRZ_0, ZRZ_0);
	DDV_MinMaxInt(pDX, ZRZ_0, -180, 180);
	DDX_Text(pDX, IDC_ZRZ_1, ZRZ_1);
	DDV_MinMaxInt(pDX, ZRZ_1, -180, 180);
	DDX_Text(pDX, IDC_ZRZ_2, ZRZ_2);
	DDV_MinMaxInt(pDX, ZRZ_2, -180, 180);
	DDX_Text(pDX, IDC_ZRZ_3, ZRZ_3);
	DDX_Text(pDX, IDC_ZRZ_4, ZRZ_4);
	DDX_Text(pDX, IDC_ZRZ_5, ZRZ_5);
	DDX_Text(pDX, IDC_ZTX_0, ZTX_0);
	DDX_Text(pDX, IDC_ZTX_1, ZTX_1);
	DDX_Text(pDX, IDC_ZTX_2, ZTX_2);
	DDX_Text(pDX, IDC_ZTX_3, ZTX_3);
	DDX_Text(pDX, IDC_ZTX_4, ZTX_4);
	DDX_Text(pDX, IDC_ZTX_5, ZTX_5);
	DDX_Text(pDX, IDC_ZTY_0, ZTY_0);
	DDX_Text(pDX, IDC_ZTY_1, ZTY_1);
	DDX_Text(pDX, IDC_ZTY_2, ZTY_2);
	DDX_Text(pDX, IDC_ZTY_3, ZTY_3);
	DDX_Text(pDX, IDC_ZTY_4, ZTY_4);
	DDX_Text(pDX, IDC_ZTY_5, ZTY_5);
	DDX_Text(pDX, IDC_ZTZ_0, ZTZ_0);
	DDX_Text(pDX, IDC_ZTZ_1, ZTZ_1);
	DDX_Text(pDX, IDC_ZTZ_2, ZTZ_2);
	DDX_Text(pDX, IDC_ZTZ_3, ZTZ_3);
	DDX_Text(pDX, IDC_ZTZ_4, ZTZ_4);
	DDX_Text(pDX, IDC_ZTZ_5, ZTZ_5);
	DDX_Text(pDX, IDC_ZNAME_0, ZName_0);
	DDX_Text(pDX, IDC_ZNAME_1, ZName_1);
	DDX_Text(pDX, IDC_ZNAME_2, ZName_2);
	DDX_Text(pDX, IDC_ZNAME_3, ZName_3);
	DDX_Text(pDX, IDC_ZNAME_4, ZName_4);
	DDX_Text(pDX, IDC_ZNAME_5, ZName_5);
	DDX_Text(pDX, IDC_AC_ZONE_UNITS, Units_Label);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Dg_AC_Zone, CDialog)
	//{{AFX_MSG_MAP(Dg_AC_Zone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_AC_Zone Over-ridables

//#include "hom_doc.h"	// Unnecessary!
/*-------------------------------*/ void     //------------------------------
Dg_AC_Zone ::                                                         OnOK
( void ) 
   {
   this->CDialog::OnOK(); //Invoke Parent Class OnOK
   return;
   }

/*-------------------------------*/ BOOL     //------------------------------
Dg_AC_Zone ::                                                 OnInitDialog
( void ) 
   {
   return this->CDialog::OnInitDialog();
   }




