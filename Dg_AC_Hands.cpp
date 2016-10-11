// Dg_AC_Hand.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include "Dg_AC_Hands.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_AC_Hands dialog


Dg_AC_Hands::Dg_AC_Hands(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_AC_Hands::IDD, pParent)
{
	//{{AFX_DATA_INIT(Dg_AC_Hands)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void Dg_AC_Hands::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_AC_Hands)
	DDX_Control(pDX, IDC_UCS_HAND_RIGHT, UCS_Hand_Right);
	DDX_Control(pDX, IDC_UCS_HAND_LEFT, UCS_Hand_Left);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Dg_AC_Hands, CDialog)
	//{{AFX_MSG_MAP(Dg_AC_Hands)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_AC_Hands Over-ridables

//#include "hom_doc.h"	// Unnecessary!
/*-------------------------------*/ void     //------------------------------
Dg_AC_Hands ::                                                   OnOK
( void ) 
   {
   C_Hom_Doc*  Doc_Ptr = Get_App_Ptr()->Get_Document_Ptr();

	CString tempStr = Doc_Ptr->getUCS_Hand_Left_Value();
   Doc_Ptr->Save_Combo_String
      ( this->UCS_Hand_Left,  tempStr );
	Doc_Ptr->setUCS_Hand_Left_Value(tempStr);

	tempStr = Doc_Ptr->getUCS_Hand_Right_Value();
   Doc_Ptr->Save_Combo_String
      ( this->UCS_Hand_Right, tempStr );
	Doc_Ptr->setUCS_Hand_Right_Value(tempStr);
                                   
   this->CDialog::OnOK(); //Invoke Parent Class OnOK
   
   return;
   }

/*-------------------------------*/ BOOL     //------------------------------
Dg_AC_Hands ::                                           OnInitDialog
( void ) 
   {
   this->CDialog::OnInitDialog();
   
   C_Hom_App*  App_Ptr = Get_App_Ptr();
   C_Hom_Doc*  Doc_Ptr = App_Ptr->Get_Document_Ptr();
   
   App_Ptr->Load_Combo_List( this->UCS_Hand_Left  ); 
   App_Ptr->Load_Combo_List( this->UCS_Hand_Right ); 
  
   Doc_Ptr->Restore_Combo_String
	   ( this->UCS_Hand_Left, " " );

   Doc_Ptr->Restore_Combo_String
      ( this->UCS_Hand_Right, " " );

   return   TRUE;
   }



