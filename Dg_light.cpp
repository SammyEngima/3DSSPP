// dg_light.cpp : implementation file
//
#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h" // Necessary

#include ".\dg_light.h"

#include <sstream>

using std::stringstream;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

const char* LIGHT_ERROR = "All three selections can not be 'Center'.";

/////////////////////////////////////////////////////////////////////////////
// Dg_Light dialog

Dg_Light::Dg_Light(C_Hom_Doc* aDocPtr,
                   CWnd* pParent)
   : CDialog(Dg_Light::IDD, pParent),
   mDocPtr(aDocPtr)
   , mIntensity(0)
   , mIntensityPct("")
   , lightRadio(FALSE)
{
   //{{AFX_DATA_INIT(Dg_Light)
   m_FrontBack_Value = -1;
   m_LeftRight_Value = -1;
   m_TopBottom_Value = -1;
   //}}AFX_DATA_INIT
   ASSERT( mDocPtr != NULL );
}

void Dg_Light::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_Light)
	DDX_Radio(pDX, IDC_FrontBack_Value, m_FrontBack_Value);
	DDX_Radio(pDX, IDC_LeftRight_Value, m_LeftRight_Value);
	DDX_Radio(pDX, IDC_TopBottom_Value, m_TopBottom_Value);
	//}}AFX_DATA_MAP
	//DDX_Check(pDX, IDC_NEW_LIGHT_SCHEME_CHK, mNewLightSchemeChk);
	DDX_Slider(pDX, IDC_INTENSITY_SLIDER, mIntensity);
	DDX_Text(pDX, IDC_INTENSITY_VALUE, mIntensityPct);
	DDX_Radio(pDX, IDC_LIGHT_SINGLE, lightRadio);
}

BEGIN_MESSAGE_MAP(Dg_Light, CDialog)
   //{{AFX_MSG_MAP(Dg_Light)
   ON_BN_CLICKED(IDC_RedrawButton, OnRedrawButton)
   //}}AFX_MSG_MAP
   ON_NOTIFY(NM_CUSTOMDRAW, IDC_INTENSITY_SLIDER, OnNMCustomdrawIntensitySlider)
   ON_BN_CLICKED(IDC_Default_Lighting, OnBnClickedDefaultLighting)
   ON_BN_CLICKED(IDC_LIGHT_SINGLE, &Dg_Light::OnBnClickedLightSingle)
   ON_BN_CLICKED(IDC_LIGHT_THREE, &Dg_Light::OnBnClickedLightThree)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dg_Light message handlers

//---------------------------------------------------------------------------
BOOL Dg_Light::IsValid()
//All radio selections can not be equal to 1. This would place the light
//source at the center of the hominoid.
{
   this->UpdateData( TRUE ); //Move data from controls to dialog variables

   if (m_FrontBack_Value == 1
      && m_LeftRight_Value == 1
      && m_TopBottom_Value == 1)
      return FALSE;
   else
      return TRUE;
}

//---------------------------------------------------------------------------
void Dg_Light::OnOK() 
{
   if (this->IsValid())
      // Combination ok, proceed
      CDialog::OnOK();
   else
      // Invalid combination
      AfxMessageBox(LIGHT_ERROR);/*, MB_OK || MB_ICONEXCLAMATION);*/
}

//---------------------------------------------------------------------------
void Dg_Light::OnRedrawButton() 
{
   if (!(this->CWnd::UpdateData(TRUE)))
      return;

   //Data retrieval successful
   if (this->IsValid())
      mDocPtr->ObliqueViewLightUpdate(*this);
   else
      AfxMessageBox(LIGHT_ERROR);/*, MB_OK || MB_ICONEXCLAMATION);*/
}

void Dg_Light::CheckDisableSingleLightSource()
{
   // If we're using the new light scheme, disable all items related to single-source light scheme otherwise 
   //enable all. If we're not using the 3 point scheme, disable the light intensity box.
   GetDlgItem(IDC_FRONTBACK_GROUPBOX)->EnableWindow(lightRadio == 0);
   GetDlgItem(IDC_LEFTRIGHT_GROUPBOX)->EnableWindow(lightRadio == 0);
   GetDlgItem(IDC_TOPBOTTOM_GROUPBOX)->EnableWindow(lightRadio == 0);

//   GetDlgItem(IDC_LIGHTPOSITION_TEXT)->EnableWindow(lightRadio == 0);
//   GetDlgItem(IDC_SINGLEPOINT_GROUPBOX)->EnableWindow(lightRadio == 0);
   
   GetDlgItem(IDC_FrontBack_Value)->EnableWindow(lightRadio == 0);
   GetDlgItem(IDC_FrontBack_Middle)->EnableWindow(lightRadio == 0);
   GetDlgItem(IDC_FrontBack_Back)->EnableWindow(lightRadio == 0);
   GetDlgItem(IDC_LeftRight_Value)->EnableWindow(lightRadio == 0);
   GetDlgItem(IDC_LeftRight_Middle)->EnableWindow(lightRadio == 0);
   GetDlgItem(IDC_LeftRight_Right)->EnableWindow(lightRadio == 0);
   GetDlgItem(IDC_TopBottom_Value)->EnableWindow(lightRadio == 0);
   GetDlgItem(IDC_TopBottom_Middle)->EnableWindow(lightRadio == 0);
   GetDlgItem(IDC_TopBottom_Bottom)->EnableWindow(lightRadio == 0);

   //Disable light intensity if not using 3 point scheme.
   GetDlgItem(IDC_INTENSITY_SLIDER)->EnableWindow(lightRadio != 0);
   GetDlgItem(IDC_INTENSITY_VALUE)->EnableWindow(lightRadio != 0);
   GetDlgItem(IDC_INTENSITY_VALUE2)->EnableWindow(lightRadio != 0);
   GetDlgItem(IDC_LIGHT_INTENSITY_BOX)->EnableWindow(lightRadio != 0);
}

void Dg_Light::SetIntensityPctString()
{
   stringstream sstr;
   sstr << mIntensity << '%';
   mIntensityPct = sstr.str().c_str();
}

BOOL Dg_Light::OnInitDialog()
{
   CDialog::OnInitDialog();

   SetIntensityPctString();
   CheckDisableSingleLightSource();

   UpdateData(FALSE);   // Send it out

   return TRUE;  // return TRUE unless you set the focus to a control
}

void Dg_Light::OnNMCustomdrawIntensitySlider(NMHDR *pNMHDR, LRESULT *pResult)
{
   LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

   // Update the number next to the slider
   UpdateData(TRUE);    // Get slider position
   SetIntensityPctString();
   UpdateData(FALSE);   // Send out number to text

   *pResult = 0;
}


// Reset all values to default settings
void Dg_Light::OnBnClickedDefaultLighting()
{
   UpdateData(TRUE);

   this->m_FrontBack_Value = 0;
   this->m_LeftRight_Value = 0;
   this->m_TopBottom_Value = 0;
   this->lightRadio = 1;
   this->mIntensity = 60;
   SetIntensityPctString();

   UpdateData(FALSE);

   this->CheckDisableSingleLightSource();
}

void Dg_Light::OnBnClickedLightSingle()
{
	UpdateData(true);
	this->CheckDisableSingleLightSource();
}

void Dg_Light::OnBnClickedLightThree()
{
	UpdateData(true);
	this->CheckDisableSingleLightSource();
}
