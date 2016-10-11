// Dg_AC_Feet.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include "Dg_AC_Feet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_AC_Feet dialog


Dg_AC_Feet::Dg_AC_Feet(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_AC_Feet::IDD, pParent)
{
	//{{AFX_DATA_INIT(Dg_AC_Feet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void Dg_AC_Feet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_AC_Feet)
	DDX_Control(pDX, IDC_UCS_FOOT_RIGHT, UCS_Foot_Right);
	DDX_Control(pDX, IDC_UCS_FOOT_LEFT, UCS_Foot_Left);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Dg_AC_Feet, CDialog)
	//{{AFX_MSG_MAP(Dg_AC_Feet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_AC_Hands Over-ridables

//#include "hom_doc.h"	// Unnecessary!

/*--------------------------------------------------------------*/
void Dg_AC_Feet :: OnOK() 
{
   C_Hom_Doc* Doc_Ptr = Get_App_Ptr()->Get_Document_Ptr();

	CString tempStr = Doc_Ptr->getUCS_Foot_Left_Value();
   Doc_Ptr->Save_Combo_String
      ( this->UCS_Foot_Left,  tempStr );
	Doc_Ptr->setUCS_Foot_Left_Value( tempStr );
   
	tempStr = Doc_Ptr->getUCS_Foot_Right_Value();
	Doc_Ptr->Save_Combo_String
      ( this->UCS_Foot_Right, tempStr );
	Doc_Ptr->setUCS_Foot_Right_Value( tempStr );
                                   
   this->CDialog::OnOK(); //Invoke Parent Class OnOK
   
   return;
}

/*-------------------------------*/ BOOL     //------------------------------
Dg_AC_Feet::OnInitDialog() 
{
   this->CDialog::OnInitDialog();
   
   C_Hom_App*  App_Ptr = Get_App_Ptr();
   C_Hom_Doc*  Doc_Ptr = App_Ptr->Get_Document_Ptr();
   
   App_Ptr->Load_Combo_List( this->UCS_Foot_Left  ); 
   App_Ptr->Load_Combo_List( this->UCS_Foot_Right ); 
   
   CString tempStr = Doc_Ptr->getUCS_Foot_Left_Value();
   Doc_Ptr->Restore_Combo_String(this->UCS_Foot_Left, " ");

   tempStr = Doc_Ptr->getUCS_Foot_Right_Value();
   Doc_Ptr->Restore_Combo_String(this->UCS_Foot_Right, " ");

   return   TRUE;
}


