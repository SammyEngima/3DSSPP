// Dg_Registration.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "Dg_Registration.h"
#include ".\dg_registration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_Registration dialog

Dg_Registration::Dg_Registration(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_Registration::IDD, pParent)
{
   //{{AFX_DATA_INIT(Dg_Registration)
   mCompanyName = _T("");
   mRegistrationCode = _T("");
   mRegCode1 = _T("");
   mRegCode2 = _T("");
   mRegCode3 = _T("");
   mRegCode4 = _T("");
   //}}AFX_DATA_INIT
}

void Dg_Registration::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(Dg_Registration)
   DDX_Text(pDX, IDC_COMPANY_NAME, mCompanyName);
   DDV_MaxChars(pDX, mCompanyName, 30);
   DDX_Text(pDX, IDC_LICENSE_NUMBER1, mRegCode1);
   DDV_MaxChars(pDX, mRegCode1, 5);
   DDX_Text(pDX, IDC_LICENSE_NUMBER2, mRegCode2);
   DDV_MaxChars(pDX, mRegCode2, 5);
   DDX_Text(pDX, IDC_LICENSE_NUMBER3, mRegCode3);
   DDV_MaxChars(pDX, mRegCode3, 5);
   DDX_Text(pDX, IDC_LICENSE_NUMBER4, mRegCode4);
   DDV_MaxChars(pDX, mRegCode4, 5);
   DDX_Control(pDX, IDCANCEL, mDemoModeCtrl);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Dg_Registration, CDialog)
   //{{AFX_MSG_MAP(Dg_Registration)
   ON_EN_KILLFOCUS(IDC_LICENSE_NUMBER1, OnKillfocusLicense1)
   ON_EN_KILLFOCUS(IDC_LICENSE_NUMBER2, OnKillfocusLicense2)
   ON_EN_KILLFOCUS(IDC_LICENSE_NUMBER3, OnKillfocusLicense3)
   ON_EN_KILLFOCUS(IDC_LICENSE_NUMBER4, OnKillfocusLicense4)
   ON_WM_CREATE()
   ON_WM_PAINT()
   //}}AFX_MSG_MAP
   ON_EN_CHANGE(IDC_LICENSE_NUMBER1, OnEnChangeLicenseNumber1)
   ON_EN_CHANGE(IDC_LICENSE_NUMBER2, OnEnChangeLicenseNumber2)
   ON_EN_CHANGE(IDC_LICENSE_NUMBER3, OnEnChangeLicenseNumber3)
   ON_EN_SETFOCUS(IDC_LICENSE_NUMBER1, OnEnSetfocusLicenseNumber1)
   ON_EN_SETFOCUS(IDC_LICENSE_NUMBER2, OnEnSetfocusLicenseNumber2)
   ON_EN_SETFOCUS(IDC_LICENSE_NUMBER3, OnEnSetfocusLicenseNumber3)
   ON_EN_SETFOCUS(IDC_LICENSE_NUMBER4, OnEnSetfocusLicenseNumber4)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_Registration message handlers

BOOL Dg_Registration::OnInitDialog()
{
   C_Hom_App*  App_Ptr = Get_App_Ptr();
   mCompanyName = App_Ptr->GetProfileString("Registration", "Company", "none");
   mRegistrationCode = App_Ptr->GetProfileString("Registration",
                                                 "RegKey",
                                                 "#####-#####-#####-#####");
   mRegCode1 = mRegistrationCode.Mid(0,5);
   mRegCode2 = mRegistrationCode.Mid(6,5);
   mRegCode3 = mRegistrationCode.Mid(12,5);
   mRegCode4 = mRegistrationCode.Mid(18,5);

   if(App_Ptr->SiteData()) // Valid site data; disable entry of new code
   {
      mLicenseAlreadyValid = true;
      mSavedRegistrationCode = mRegistrationCode;
   }
   else
   {
      mLicenseAlreadyValid = false;
      mSavedRegistrationCode = "";
   }

   UpdateData(FALSE);
   return TRUE;
}

void Dg_Registration::OnOK() 
{
   SetFocus(); // Make sure registration string is updated, in case
               // keyboard was used instead of mouse
   C_Hom_App* App_Ptr = Get_App_Ptr();
   UpdateData(TRUE);
   // Copy dialog boxes to registry

   if (mCompanyName.GetLength() == 0 || mCompanyName == "none")
   {
       AfxMessageBox ("Please Enter Company Name");
       return;
   }

   // Only let them OnOK if the license is valid
   if (App_Ptr->SiteData(mRegistrationCode))
   {
      // Write the string to the registry
      BOOL B = App_Ptr->WriteProfileString
         ("Registration", "RegKey" , _T(mRegistrationCode));
      BOOL A = App_Ptr->WriteProfileString
         ("Registration", "Company", _T(mCompanyName));

//      App_Ptr->Company_Name = mCompanyName;

      if(mSavedRegistrationCode != "")
      {
         if(mSavedRegistrationCode != mRegistrationCode)
            // They changed the code to a new valid one
            AfxMessageBox("Registration number changed.  To apply changes, restart 3DSSPP.");
      }
      else
         AfxMessageBox("Registration number valid.  Thank you for registering 3DSSPP!");

      CDialog::OnOK();
   }
   else if(mLicenseAlreadyValid)
   {
      // Restore valid license so they don't lose it
      App_Ptr->WriteProfileString( "Registration", "RegKey", _T(mSavedRegistrationCode) );
      AfxMessageBox( "Invalid code - registration number not changed.");
      CDialog::OnOK();
   }
   else
   {
      AfxMessageBox("Invalid Registration Number");
   }
}

void Dg_Registration::OnCancel() 
{
   CDialog::OnCancel();
}

void Dg_Registration::OnKillfocusLicense1() 
{
   UpdateRegCode(mRegCode1);
}

void Dg_Registration::OnKillfocusLicense2() 
{
   UpdateRegCode(mRegCode2);
}

void Dg_Registration::OnKillfocusLicense3() 
{
   UpdateRegCode(mRegCode3);
}

void Dg_Registration::OnKillfocusLicense4() 
{
   UpdateRegCode(mRegCode4);
}

void Dg_Registration::UpdateRegCode(CString& regString)
{
   UpdateData(TRUE);
   if(regString == "")
   {
      regString = "#####";
      UpdateData(FALSE);
   }
   else
      mRegistrationCode = mRegCode1 + "-" + mRegCode2 + "-"
                        + mRegCode3 + "-" + mRegCode4;
}

void Dg_Registration::OnPaint() 
{
   CPaintDC dc(this); // device context for painting
	
   if(mLicenseAlreadyValid)
      mDemoModeCtrl.EnableWindow(FALSE);

   // Do not call CDialog::OnPaint() for painting messages
}

void Dg_Registration::OnEnChangeLicenseNumber1()
{
   UpdateData(TRUE);
   if(mRegCode1.GetLength() == 5)
      this->GetDlgItem(IDC_LICENSE_NUMBER2)->SetFocus();
}

void Dg_Registration::OnEnChangeLicenseNumber2()
{
   UpdateData(TRUE);
   if(mRegCode2.GetLength() == 5)
      this->GetDlgItem(IDC_LICENSE_NUMBER3)->SetFocus();
}

void Dg_Registration::OnEnChangeLicenseNumber3()
{
   UpdateData(TRUE);
   if(mRegCode3.GetLength() == 5)
      this->GetDlgItem(IDC_LICENSE_NUMBER4)->SetFocus();
}

void Dg_Registration::ClearNewRegBox(CString& regString)
{
   UpdateData(TRUE);
   if(regString == "#####")
   {
      regString = "";
      UpdateData(FALSE);
   }
}

void Dg_Registration::OnEnSetfocusLicenseNumber1()
{
   ClearNewRegBox(mRegCode1);
}

void Dg_Registration::OnEnSetfocusLicenseNumber2()
{
   ClearNewRegBox(mRegCode2);
}

void Dg_Registration::OnEnSetfocusLicenseNumber3()
{
   ClearNewRegBox(mRegCode3);
}

void Dg_Registration::OnEnSetfocusLicenseNumber4()
{
   ClearNewRegBox(mRegCode4);
}
