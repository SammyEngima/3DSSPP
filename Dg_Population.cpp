// Dg_Population.cpp : implementation file
//

#include "stdafx.h"
#include "AppVersion.hpp"
#include "AnthroPercentiles.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include "Factors.hpp"
#include "Dg_PopulationEdit.h"
#include "Dg_Parameters.h"

#include <string>
#include ".\dg_population.h"

using std::ifstream;
using std::string;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_Population dialog

Dg_Population::Dg_Population(CWnd* pParent /*=NULL*/)
   : CDialog(Dg_Population::IDD, pParent),
   mPriorFilePathName( "NULL" ) ,
   mFilePathName( "NULL" )
{
}

void Dg_Population::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Dg_Population, CDialog)
   ON_WM_CTLCOLOR()
   ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
   ON_BN_CLICKED(IDC_FDEFAULT, OnFdefault)
   ON_BN_CLICKED(IDC_FEDITOR, OnFeditor)
   ON_BN_CLICKED(IDC_OPEN_FACTORS, OnOpenFactors)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_Population message handlers

HBRUSH Dg_Population::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
   return (HBRUSH)(CBrush(RGB(255,255,255)));
}

void Dg_Population::OnDisplayDialogcolor() 
{
   Invalidate(true);
}

void Dg_Population::OnOpenFactors() 
{
   C_Hom_App*  App_Ptr = Get_App_Ptr();
   //C_Hom_Doc*  Doc_Ptr = App_Ptr->Get_Document_Ptr();

   //Doc_Ptr->SetPopFromFile();

   CString lFilters = "Factor Files (*.fct)|*.fct||";

   CFileDialog lDiag(TRUE,    // file open box
                     "fct",   // default extension
                     NULL,    // default file name
                     OFN_HIDEREADONLY|OFN_PATHMUSTEXIST|OFN_NOCHANGEDIR,
                     lFilters ,
                     NULL);   // parent window

   if( lDiag.DoModal() == IDOK )
   {
      mPriorFilePathName = mFilePathName;
      mFilePathName = lDiag.GetPathName();
      this->ReadFactorsFromFile();
   }
   //else do nothing
}

void Dg_Population::OnFdefault() 
{
   // Create a new Factors object as the default
   //    (defaults will automatically be set)
   C_Hom_App*  App_Ptr = Get_App_Ptr();
   C_Hom_Doc*  Doc_Ptr = App_Ptr->Get_Document_Ptr();

   Anthropometry anthro = Doc_Ptr->getAnthropometryRef();
   anthro.ResetHeightPercentiles();
   anthro.ResetWeightPercentiles();
   anthro.ResetStatureDistribution();
   anthro.ResetWeightDistribution();
   Doc_Ptr->setAnthropometry(anthro);

   // Reset the App's factors with a new Factors object
   // Restore Both default factors
   Doc_Ptr->RestoreDefaultFactors(-1);

   Doc_Ptr->MakeDirtyAndUpdateViews();

   string msgString = "Default ";
   //msgString += (this->mGender == 0) ? "male" : "female";
   msgString += " population data restored.";
   
   AfxMessageBox(msgString.c_str());
}

void Dg_Population::OnFeditor() 
{
   C_Hom_App* App_Ptr = Get_App_Ptr();
   C_Hom_Doc* Doc_Ptr = App_Ptr->Get_Document_Ptr();
   Doc_Ptr->OnDg_FactorsPassword();	
}

void Dg_Population::ReadFactorsFromFile()
{
   Factors maleFactors;
   Factors femaleFactors;
   Factors lFactors;
   C_Hom_App*  App_Ptr = Get_App_Ptr();
   C_Hom_Doc*  Doc_Ptr = App_Ptr->Get_Document_Ptr();
   float *stature, *weight;

   ifstream   lIn( (LPCSTR)mFilePathName );
   int major_version, minor_version;
   lIn >> major_version >> minor_version;
   OpenType = major_version * 100 + minor_version;		// File version number identifier
   if(major_version < 5)
   {
		lIn >> maleFactors;
		// Set the factors to be the same for both
		femaleFactors = maleFactors;
		lFactors = maleFactors;
   } else {
		maleFactors.RestoreMaleDefaults();
		femaleFactors.RestoreFemaleDefaults();
		lIn >> maleFactors;
		lIn >> femaleFactors;
		if(mGender == 0)
		{
			lFactors = maleFactors;
		} else {
			lFactors = femaleFactors;	
		}
   }
   Doc_Ptr->SetMaleFactors(maleFactors);
   Doc_Ptr->SetFemaleFactors(femaleFactors);

   // ################################################################START#
   // ACT - Lets update the antrometric distribution based on the settings
   // from the dialog
#if (USE_POPULATION_DISTRIBUTION)
   stature = Doc_Ptr->getAnthropometryRef().GetStatureDistribution();
   weight = Doc_Ptr->getAnthropometryRef().GetWeightDistribution();
   stature[0] = lFactors.mMHmean;
   stature[1] = lFactors.mMHstdev;
   stature[2] = lFactors.mFHmean;
   stature[3] = lFactors.mFHstdev;
   weight[0] = lFactors.mMWmean;
   weight[1] = lFactors.mMWstdev;
   weight[2] = lFactors.mFWmean;
   weight[3] = lFactors.mFWstdev;
#else
   Anthropometry anthro = Doc_Ptr->getAnthropometryRef();
   stature = anthro.GetHeightPercentiles();
   weight = anthro.GetWeightPercentiles();
   stature[0] = lFactors.mMH95;
   stature[1] = lFactors.mMH50;
   stature[2] = lFactors.mMH5;
   stature[3] = lFactors.mFH95;
   stature[4] = lFactors.mFH50;
   stature[5] = lFactors.mFH5;

   weight[0] = lFactors.mMW95;
   weight[1] = lFactors.mMW50;
   weight[2] = lFactors.mMW5;
   weight[3] = lFactors.mFW95;
   weight[4] = lFactors.mFW50;
   weight[5] = lFactors.mFW5;
   Doc_Ptr->setAnthropometry(anthro);
#endif

   Doc_Ptr->MakeDirtyAndUpdateViews();
}

BOOL Dg_Population::OnInitDialog()
{
   CDialog::OnInitDialog();

   string titleString;
   if(mGender == 0)
      titleString = "Male Factors";
   else
      titleString = "Female Factors";

   this->SetWindowText(titleString.c_str());

   return TRUE;  // return TRUE unless you set the focus to a control
}
