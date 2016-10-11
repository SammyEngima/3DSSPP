// dg_acpos.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include "Dg_AC_Position.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_AC_Position dialog


Dg_AC_Position::Dg_AC_Position(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_AC_Position::IDD, pParent)
{
	//{{AFX_DATA_INIT(Dg_AC_Position)
	Tra_X = 0.0F;
	Tra_Y = 0.0F;
	Tra_Z = 0.0F;
	Data_Method = -1;
	Positioned_Point = -1;
	Units_Label = "";
	Rot_X = 0;
	Rot_Y = 0;
	Rot_Z = 0;
	Active_Zone = -1;
	Zone_Name_0 = "";
	Zone_Name_1 = "";
	Zone_Name_2 = "";
	Zone_Name_3 = "";
	Zone_Name_4 = "";
	Zone_Name_5 = "";
	//}}AFX_DATA_INIT
}

void Dg_AC_Position::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_AC_Position)
	DDX_Control(pDX, IDC_UCS_WORLD, UCS_World);
	DDX_Text(pDX, IDC_AC_TRAX, Tra_X);
	DDX_Text(pDX, IDC_AC_TRAY, Tra_Y);
	DDX_Text(pDX, IDC_AC_TRAZ, Tra_Z);
	DDX_Radio(pDX, IDC_DATA_METHOD, Data_Method);
	DDX_Radio(pDX, IDC_POSITIONED_POINT, Positioned_Point);
	DDX_Text(pDX, IDC_AC_POSITION_UNITS, Units_Label);
	DDX_Text(pDX, IDC_AC_ROTX, Rot_X);
	DDX_Text(pDX, IDC_AC_ROTY, Rot_Y);
	DDX_Text(pDX, IDC_AC_ROTZ, Rot_Z);
	DDX_Radio(pDX, IDC_UCS_ZONE, Active_Zone);
	DDX_Text(pDX, IDC_ZONE_NAME_0, Zone_Name_0);
	DDX_Text(pDX, IDC_ZONE_NAME_1, Zone_Name_1);
	DDX_Text(pDX, IDC_ZONE_NAME_2, Zone_Name_2);
	DDX_Text(pDX, IDC_ZONE_NAME_3, Zone_Name_3);
	DDX_Text(pDX, IDC_ZONE_NAME_4, Zone_Name_4);
	DDX_Text(pDX, IDC_ZONE_NAME_5, Zone_Name_5);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Dg_AC_Position, CDialog)
	//{{AFX_MSG_MAP(Dg_AC_Position)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dg_AC_Position Over-ridables

//#include "hom_doc.h"	// Unnecessary!
/*-------------------------------*/ void     //------------------------------
Dg_AC_Position ::                                                     OnOK
( void ) 
   {
   C_Hom_Doc*  Doc_Ptr = Get_App_Ptr()->Get_Document_Ptr();

	CString tempStr = Doc_Ptr->getUCS_World_Value();
   Doc_Ptr->Save_Combo_String( this->UCS_World, tempStr );
	Doc_Ptr->setUCS_World_Value(tempStr);
                                   
   this->CDialog::OnOK(); //Invoke Parent Class OnOK
   
   return;
   }

/*-------------------------------*/ BOOL     //------------------------------
Dg_AC_Position ::                                              OnInitDialog
( void ) 
   {
   this->CDialog::OnInitDialog();
   
   C_Hom_App*  App_Ptr = Get_App_Ptr();
   C_Hom_Doc*  Doc_Ptr = App_Ptr->Get_Document_Ptr();
   
   App_Ptr->Load_Combo_List( this->UCS_World ); //Populate selection list

   Doc_Ptr->Restore_Combo_String( this->UCS_World, " " );

   return   TRUE;
   }



