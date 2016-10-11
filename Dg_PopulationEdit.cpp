// Dg_PopulationEdit.cpp : implementation file
//

#include "stdafx.h"
#include "AppVersion.hpp"
#include "AnthroPercentiles.h"

#include "hom.h"
#include "hom_doc.h" // Necessary
#include "factors.hpp"
#include "Dg_PopulationEdit.h"
#include "Dg_Parameters.h"
#include "Dg_HWMeans.h"

#include "Dg_FactorsFeet.h"
#include "Dg_FactorsHands.h"
#include "Dg_FactorsROM.h"

#include <string>
#include ".\dg_populationedit.h"

using std::ifstream;
using std::ofstream;
using std::string;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_PopulationEdit dialog


//---------------------------------------------------------------------------
Dg_PopulationEdit::Dg_PopulationEdit(C_Hom_Doc* aDocPtr, CWnd* pParent /*=NULL*/) :
CDialog(Dg_PopulationEdit::IDD, pParent),
mPriorFilePathName("NULL"),
mFilePathName("NULL"),
maleFactors(),
mFactors(),
femaleFactors(),
mGender(-1),
mDocPtr(aDocPtr)
, mGenderRadio(FALSE)
{

}
void Dg_PopulationEdit::OnRedraw()
{
	 UpdateData(TRUE);
	RedrawWindow();
	ofstream temp;
 if(this->mGender == 0)
		{
			mDocPtr->SetMaleFactors(mFactors);
		} else { 
			mDocPtr->SetFemaleFactors(mFactors);
	    }

    mDocPtr->MakeDirtyAndUpdateViews();
		 UpdateData(FALSE);
}

void Dg_PopulationEdit::OnOK() 
{
OnRedraw();
	CDialog::OnOK();
}

//---------------------------------------------------------------------------
void Dg_PopulationEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(Dg_PopulationEdit)
	//}}AFX_DATA_MAP

	DDX_Text(pDX, IDC_FactorsName, mFactors.mName);
	DDV_MaxChars( pDX, mFactors.mName, 20);
	DDX_Text(pDX, IDC_LinkHead,      mFactors.mLink_C1Tragion);
	DDX_Text(pDX, IDC_LinkNasion,      mFactors.mLink_HeadRadius);
	//DDX_Text(pDX, IDC_LinkNeckLength,      mFactors.mLink_NeckLength);
	// Add:
	DDX_Text(pDX, IDC_LinkNeck1Length,     mFactors.mLink_UpperneckLength);
	DDX_Text(pDX, IDC_LinkNeck2Length,    mFactors.mLink_LowerneckLength);
	DDX_Text(pDX, IDC_LinkForearm,         mFactors.mLink_Forearm);
	DDX_Text(pDX, IDC_LinkHipToL5S1,       mFactors.mLink_HipToL5S1);
	DDX_Text(pDX, IDC_LinkHipWidth,        mFactors.mLink_HipWidth);
	DDX_Text(pDX, IDC_LinkL5S1ToIT,        mFactors.mLink_L5S1ToIT);
	DDX_Text(pDX, IDC_LinkL5S1Toc7t1,  mFactors.mLink_L5S1ToC7T1);
	DDX_Text(pDX, IDC_LinkC7T1toSCJ,       mFactors.mLink_C7T1toSCJ);
	DDX_Text(pDX, IDC_LinkLowerLeg,        mFactors.mLink_LowerLeg);
	// replaced shoulder width with scj to shoulder factor
	//DDX_Text(pDX, IDC_LinkShoulderWidth,   mFactors.mLink_ShoulderWidth);
	DDX_Text(pDX, IDC_LinkShoulderWidth,   mFactors.mLink_SCJToShoulder);
	DDX_Text(pDX, IDC_LinkUpperarm,        mFactors.mLink_Upperarm);
	DDX_Text(pDX, IDC_LinkUpperLeg,        mFactors.mLink_UpperLeg);

	//   DDX_Text(pDX, IDC_MassForearmAndHand,  mFactors.mMass_ForearmAndHand);
	// Need to put in forearm and hand seperately

	DDX_Text(pDX, IDC_MassForearm,		  mFactors.mMass_Forearm);
	DDX_Text(pDX, IDC_MassUpperarm,        mFactors.mMass_Upperarm);
	DDX_Text(pDX, IDC_MassHead2,        mFactors.mMass_Head);
	DDX_Text(pDX, IDC_MassNeck1,        mFactors.mMass_UpperNeck);
	DDX_Text(pDX, IDC_MassNeck,        mFactors.mMass_LowerNeck);
	DDX_Text(pDX, IDC_MassTorso,    mFactors.mMass_Torso);
	DDX_Text(pDX, IDC_MassPelvis,          mFactors.mMass_Pelvis);
	DDX_Text(pDX, IDC_MassUpperLeg,        mFactors.mMass_UpperLeg);
	DDX_Text(pDX, IDC_MassLowerLeg,        mFactors.mMass_LowerLeg);


	//   DDX_Text(pDX, IDC_CMForearmAndHand,    mFactors.mCM_ForearmAndHand);

	DDX_Text(pDX, IDC_CMForearm,			  mFactors.mCM_Forearm);
	DDX_Text(pDX, IDC_CMUpperarm,          mFactors.mCM_Upperarm);
	DDX_Text(pDX, IDC_CMHead2,          mFactors.mCM_Head);
	DDX_Text(pDX, IDC_CMNeck1,          mFactors.mCM_UpperNeck);
	DDX_Text(pDX, IDC_CMNeck,          mFactors.mCM_LowerNeck);
	DDX_Text(pDX, IDC_CMTorso,    mFactors.mCM_L5S1ToShoulder);
	DDX_Text(pDX, IDC_CMPelvis,            mFactors.mCM_Pelvis);
	DDX_Text(pDX, IDC_CMUpperLeg,          mFactors.mCM_UpperLeg);
	DDX_Text(pDX, IDC_CMLowerLeg,          mFactors.mCM_LowerLeg);

	DDX_Text(pDX, IDC_ANGLEC7SCJ,			mFactors.mAngle_SCJ);
	DDX_Text(pDX, IDC_ANGLETRAGIONNASION, mFactors.mAngle_Tragion_Nasion);
	DDX_Text(pDX, IDC_ANGLEHEADRESTING, mFactors.mAngle_Head_Flexion);
	DDX_Radio(pDX, IDC_RADIO1, mGenderRadio);
}

BEGIN_MESSAGE_MAP(Dg_PopulationEdit, CDialog)
   //{{AFX_MSG_MAP(Dg_PopulationEdit)
   ON_BN_CLICKED(IDC_REDRAW, OnRedraw)
   ON_BN_CLICKED(IDC_OpenButton, OnOpenButton)
   ON_BN_CLICKED(IDC_SaveAsButton, OnSaveAsButton)
   ON_BN_CLICKED(IDC_SaveButton, OnSaveButton)
   ON_BN_CLICKED(IDC_ActualValueButton, OnActualValueButton)
   ON_BN_CLICKED(IDC_PredictedValueButton, OnPredictedValueButton)
   ON_BN_CLICKED(IDC_StdDevButton, OnStdDevButton)
   ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
   ON_BN_CLICKED(IDC_HWmeans, OnHWmeans)
   //ON_BN_CLICKED(IDC_GENDERBUTTON, OnGenderChange)
   //}}AFX_MSG_MAP
   ON_BN_CLICKED(IDC_FEET_FACTORS, OnBnClickedFeetFactors)
   ON_BN_CLICKED(IDC_HAND_FACTORS, OnBnClickedHandFactors)
   ON_BN_CLICKED(IDC_RADIO1, &Dg_PopulationEdit::OnBnClickedRadio1)
   ON_BN_CLICKED(IDC_RADIO2, &Dg_PopulationEdit::OnBnClickedRadio2)
   ON_BN_CLICKED(IDC_EditRomLimits, &Dg_PopulationEdit::OnBnClickedEditromlimits)
   ON_COMMAND(ID_PRINT_PRINTWINDOW, &Dg_PopulationEdit::OnPrintPrintwindow)
   ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_PopulationEdit message handlers

//---------------------------------------------------------------------------
void Dg_PopulationEdit::OnOpenButton() 
{
   CString lFilters = "Factor Files (*.fct)|*.fct||";

   CFileDialog lDiag(TRUE,    // file open box
      "fct",   // default extension
      NULL,    // default file name
      OFN_HIDEREADONLY|OFN_PATHMUSTEXIST|OFN_NOCHANGEDIR,
      lFilters,
      NULL);   // parent window

   if(lDiag.DoModal() == IDOK)
   {
      mPriorFilePathName = mFilePathName;
      mFilePathName = lDiag.GetPathName();
      this->ReadFactorsFromFile();
   }

   //else do nothing

   return;
}

//---------------------------------------------------------------------------
void Dg_PopulationEdit::ReadFactorsFromFile()
{
   ifstream   lIn( (LPCSTR)mFilePathName );
   int major_version, minor_version;
   lIn >> major_version >> minor_version;
   OpenType = major_version * 100 + minor_version;		// File version number identifier
   if(major_version < 5)
   {
		lIn >> maleFactors;
		// Set the factors to be the same for both
		femaleFactors = maleFactors;
		mFactors = maleFactors;
   } else {
		maleFactors.RestoreMaleDefaults();
		femaleFactors.RestoreFemaleDefaults();
		lIn >> maleFactors;
		lIn >> femaleFactors;
		if(mGender == 0)
		{
			mFactors = maleFactors;
		} else {
			mFactors = femaleFactors;	
		}
   }
   mFilePathName = mPriorFilePathName;
   UpdateData(FALSE);
 }

//---------------------------------------------------------------------------
void Dg_PopulationEdit::SaveFactorsToFile()
{
   ofstream   lOut( (LPCSTR)mFilePathName );
   if( ! lOut.good() )
   {
      AfxMessageBox( "Dg_PopulationEdit::SaveFactorsToFile: Output stream initialization failed." );
      mFilePathName = mPriorFilePathName;
   }

   else //stream initialized
   {
      UpdateData( TRUE );

      int major_version, minor_version;
      major_version = VERSION_NUMBER_MAJOR;
      minor_version = VERSION_NUMBER_MINOR;
	  lOut << major_version << " " << minor_version;
      OpenType = major_version * 100 + minor_version;		// File version number identifier
	  if(major_version < 5)
	  { 
	    // This code will never get executed
	    // but it's important to keep this here for 
	    // documentation purposes to verify readin of an older
		// file.  Basically, it's not used, but we should probably
	    // still keep it
		OpenType = major_version * 100 + minor_version;		// File version number identifier
		// Set the factors to the same for male and female
	    // for the older version
		lOut << mFactors;

	  } else {
	    // Make sure this is the same in readin 
	    if(this->mGender == 0)
		{
			lOut << mFactors;
		} else { 
			lOut << maleFactors;
	    }
		if(this->mGender == 1)
		{
			lOut << mFactors;
		} else {
			lOut << femaleFactors;
		}
	  }
	  if( ! lOut.good() )
	  {
		  AfxMessageBox( "Dg_PopulationEdit::SaveFactorsToFile: Problem encountered writing factors to file." );	 
		  mFilePathName = mPriorFilePathName;
	  }
      //else: do nothing

   }//end stream initialized
   lOut.close();

   return;
}

//---------------------------------------------------------------------------
void Dg_PopulationEdit::OnSaveAsButton() 
{
   CString  lFilters = "Factor files (*.fct)|*.fct||";

   CFileDialog   lDiag
      ( 
      FALSE, //file save-as box
      "fct", //default extension
      NULL,  //default file name
      OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
      lFilters ,
      NULL   //parent window
      );

   if( lDiag.DoModal() == IDOK )
   {
      CString   mPriorFilePathName = mFilePathName;
      mFilePathName = lDiag.GetPathName();
	  if (mGender == 0)
	  {
		  femaleFactors.mName = maleFactors.mName;
	  }
	  else
	  {
		  maleFactors.mName = femaleFactors.mName;
	  }
      this->SaveFactorsToFile();
   }

   //else do nothing

   return;
}

//---------------------------------------------------------------------------
void Dg_PopulationEdit::OnSaveButton() 
{
   if( mFilePathName == "NULL" )
      this->OnSaveAsButton();
   else
      this->SaveFactorsToFile();
}

//---------------------------------------------------------------------------
void Dg_PopulationEdit::OnActualValueButton() 
{
   Dg_Parameters   lDiag;
   lDiag.mParameterSetName = "Actual Value - ";

   String lSex;
   if(this->mGender == 0)
		lSex = "Male";
   else
		lSex = "Female";
   lDiag.mParameterSetName = lDiag.mParameterSetName + lSex;

   lDiag.mParameters = mFactors.mActualValue;

   if( lDiag.DoModal() == IDOK )
      mFactors.mActualValue = lDiag.mParameters;
}

//---------------------------------------------------------------------------
// JTK - April 27, 2005 Removed 
/*void Dg_PopulationEdit::OnActualMaleButton() 
{
   Dg_Parameters lDiag;
   lDiag.mParameterSetName = "Actual Male";
   lDiag.mParameters = mFactors.mActualValue;

   if(lDiag.DoModal() == IDOK)
      mFactors.mActualValue = lDiag.mParameters;
}*/

//---------------------------------------------------------------------------
void Dg_PopulationEdit::OnPredictedValueButton() 
{
   Dg_Parameters   lDiag;
   lDiag.mParameterSetName = "Predicted Value - ";

   String lSex;
   if(this->mGender == 0)
		lSex = "Male";
   else
		lSex = "Female";
   lDiag.mParameterSetName = lDiag.mParameterSetName + lSex;

   lDiag.mParameters = mFactors.mPredictedValue;

   if(lDiag.DoModal() == IDOK)
      mFactors.mPredictedValue = lDiag.mParameters;
}

//---------------------------------------------------------------------------
// JTK - Removed April 27, 2005
/*void Dg_PopulationEdit::OnStdDevFemaleButton() 
{
   Dg_Parameters lDiag;
   lDiag.mParameterSetName = "Standard Deviation Female";
   lDiag.mParameters = mFactors.mStdDev;

   if(lDiag.DoModal() == IDOK)
      mFactors.mStdDev = lDiag.mParameters;
}*/

//---------------------------------------------------------------------------
void Dg_PopulationEdit::OnStdDevButton() 
{
   Dg_Parameters   lDiag;
   lDiag.mParameterSetName = "Standard Deviation - ";
   lDiag.mParameters = mFactors.mStdDev;

   String lSex;
   if(this->mGender == 0)
		lSex = "Male";
   else
		lSex = "Female";
   lDiag.mParameterSetName = lDiag.mParameterSetName + lSex;

   if( lDiag.DoModal() == IDOK )
      mFactors.mStdDev = lDiag.mParameters;
}

void Dg_PopulationEdit::OnDefault()
{
   if(mGender == 0)
      mFactors.RestoreMaleDefaults();
   else
      mFactors.RestoreFemaleDefaults();
   // Must restore EVERYTHING!
   maleFactors.RestoreMaleDefaults();
   femaleFactors.RestoreFemaleDefaults();

   // Send the new data to the dialog
   UpdateData(FALSE);
}

#if (!USE_POPULATION_DISTRIBUTION)
void Dg_PopulationEdit::OnHWmeans() 
{
   C_Hom_App*  App_Ptr = Get_App_Ptr();
   C_Hom_Doc*  Doc_Ptr = App_Ptr->Get_Document_Ptr();

    Dg_HWMeans lDiag;

   lDiag.mMHMax   = mFactors.mMH95;
   lDiag.mMHmean  = mFactors.mMH50;
   lDiag.mMHMin   = mFactors.mMH5;
   lDiag.mFHMax   = mFactors.mFH95;
   lDiag.mFHmean  = mFactors.mFH50;
   lDiag.mFHMin   = mFactors.mFH5;

   lDiag.mMWMax   = mFactors.mMW95;
   lDiag.mMWmean  = mFactors.mMW50;
   lDiag.mMWMin   = mFactors.mMW5;
   lDiag.mFWMax   = mFactors.mFW95;
   lDiag.mFWmean  = mFactors.mFW50;
   lDiag.mFWMin   = mFactors.mFW5;

   lDiag.RecalculatePercentiles();

   // The following was determined using NORMSINV(0.95) and NORMSINV(0.05) in Excel
   // -----------------------------------------------------------------------------
   // 95% of the population is below mean + stddev * 1.6448530004709 
   // 5% of the population is below  mean - stddev * 1.6448530004709

   if (lDiag.DoModal() == IDOK)
   {
      UpdateData(TRUE);	// Get the stuff from the dialog and store it into memory
      Doc_Ptr->MakeDirtyAndUpdateViews(); 

      mFactors.mMH95 = lDiag.mMHMax;
      mFactors.mMH50 = lDiag.mMHmean;
      mFactors.mMH5  = lDiag.mMHMin;
      mFactors.mFH95 = lDiag.mFHMax;
      mFactors.mFH50 = lDiag.mFHmean;
      mFactors.mFH5  = lDiag.mFHMin;

      mFactors.mMW95 = lDiag.mMWMax;
      mFactors.mMW50 = lDiag.mMWmean;
      mFactors.mMW5  = lDiag.mMWMin;
      mFactors.mFW95 = lDiag.mFWMax;
      mFactors.mFW50 = lDiag.mFWmean;
      mFactors.mFW5  = lDiag.mFWMin;
   }
}
#else
void Dg_PopulationEdit::OnHWmeans() 
{
   C_Hom_Doc*  Doc_Ptr = App_Ptr->Get_Document_Ptr();
   Dg_HWMeans lDiag;

   lDiag.mMHmean  = mFactors.mMHmean;
   lDiag.mFHmean  = mFactors.mFHmean;
   lDiag.mMWmean  = mFactors.mMWmean;
   lDiag.mFWmean  = mFactors.mFWmean;
   lDiag.mMHstdev = mFactors.mMHstdev;
   lDiag.mFHstdev = mFactors.mFHstdev;
   lDiag.mMWstdev = mFactors.mMWstdev;
   lDiag.mFWstdev = mFactors.mFWstdev;

   lDiag.RecalculatePercentiles();

   // The following was determined using NORMSINV(0.95) and NORMSINV(0.05) in Excel
   // -----------------------------------------------------------------------------
   // 95% of the population is below mean + stddev * 1.6448530004709 
   // 5% of the population is below  mean - stddev * 1.6448530004709

   if( lDiag.DoModal() == IDOK )
   {
      UpdateData(TRUE);	// Get the stuff from the dialog and store it into memory
      Doc_Ptr->MakeDirtyAndUpdateViews(); 

      mFactors.mMHmean  = lDiag.mMHmean;
      mFactors.mFHmean  = lDiag.mFHmean;
      mFactors.mMWmean  = lDiag.mMWmean;
      mFactors.mFWmean  = lDiag.mFWmean;
      mFactors.mMHstdev = lDiag.mMHstdev;
      mFactors.mFHstdev = lDiag.mFHstdev;
      mFactors.mMWstdev = lDiag.mMWstdev;
      mFactors.mFWstdev = lDiag.mFWstdev;
   }
}
#endif

// Function to open a "Feet Factors" dialog and change
//    those values
void Dg_PopulationEdit::OnBnClickedFeetFactors()
{
   Dg_FactorsFeet dlg;

   dlg.SetFactors(mFactors);

   if (dlg.DoModal() == IDOK) {
      mFactors = dlg.GetFactors();
   }
}
/*
void Dg_PopulationEdit::OnGenderChange()
{
    string titleString = "Population Editor - ";
    UpdateData(TRUE);
    if(mGender == 0)
    {
        maleFactors = mFactors;
        this->SetGender(1);
        titleString += "Female Factors";
    }
    else { // mGender == 1
        femaleFactors = mFactors;
        this->SetGender(0);
        titleString += "Male Factors";
    }
    // Update the documents gender
    this->SetWindowText(titleString.c_str());
    UpdateData(FALSE); // send the new data to the screen
}*/
BOOL Dg_PopulationEdit::OnInitDialog()
{
   CDialog::OnInitDialog();
   	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->AppendMenu(MF_SEPARATOR);// add a separator
	pSysMenu->AppendMenu(MF_STRING,IDR_MENU1,"Print Window");// add print menu
   string titleString = "Population Editor - ";
   if(mGender == 0) {
      titleString += "Male Factors";
	  mGenderRadio = 1;
   } else {
      titleString += "Female Factors";
	  mGenderRadio = 0;
   }
   UpdateData(false);

   this->SetWindowText(titleString.c_str());

   return TRUE;  // return TRUE unless you set the focus to a control
}

void Dg_PopulationEdit::OnBnClickedHandFactors()
{
	Dg_FactorsHands dlg;

	dlg.SetFactors(mFactors);

	if(dlg.DoModal() == IDOK) {
		mFactors = dlg.GetFactors();
	}
}

void Dg_PopulationEdit::OnBnClickedRadio1()
{
    string titleString = "Population Editor - ";
    UpdateData(TRUE);

	// If we're changing, "save" the factors
	if(mGender == 0) 
		{maleFactors = mFactors;
	femaleFactors.mName = maleFactors.mName;}
    this->SetGender(1);
    titleString += "Female Factors";
    // Update the document's gender
    this->SetWindowText(titleString.c_str());
    UpdateData(FALSE); // send the new data to the screen
   
}

void Dg_PopulationEdit::OnBnClickedRadio2()
{
    string titleString = "Population Editor - ";
    UpdateData(TRUE);

	// If we're changing, "save" the factors
	if(mGender == 1) 
		{femaleFactors = mFactors;
	maleFactors.mName = femaleFactors.mName;}
	this->SetGender(0);
	titleString += "Male Factors";
    
    // Update the documents gender
    this->SetWindowText(titleString.c_str());
    UpdateData(FALSE); // send the new data to the screen
}

void Dg_PopulationEdit::OnBnClickedEditromlimits()
{
	Dg_FactorsROM dlg;

	dlg.SetFactors(mFactors);

	if(dlg.DoModal() == IDOK) {
		mFactors = dlg.GetFactors();
	}

	// TODO should we really have ROM limits separate for male and female? I guess I don't see why not. Someone might want it
}


void Dg_PopulationEdit::OnPrintPrintwindow()
{
	// TODO: Add your command handler code here
	 CRect m_rect;
 GetClientRect(&m_rect);
 CDC* pImageDC=GetDC();
 int formx,formy;
 formx=pImageDC->GetDeviceCaps(LOGPIXELSX);
 formy=pImageDC->GetDeviceCaps(LOGPIXELSY);
 ReleaseDC(pImageDC);
 DWORD dwflags=PD_PAGENUMS|PD_HIDEPRINTTOFILE|PD_SELECTION;  
 CPrintDialog m_printdlg(FALSE,dwflags,NULL);     
 if(m_printdlg.DoModal()==IDOK)
 {
  CDC pdc;
  pdc.Attach(m_printdlg.GetPrinterDC());
  int printerx,printery;
  printerx=pdc.GetDeviceCaps(LOGPIXELSX);
  printery=pdc.GetDeviceCaps(LOGPIXELSY);
  double ratex,ratey;
  ratex=(double)printerx/formx;
  ratey=(double)printery/formy;
  CClientDC dc(this);
  CBitmap bmp;
  int w=m_rect.Width();
  int h =m_rect.Height();
  bmp.CreateCompatibleBitmap(&dc,m_rect.Width(),m_rect.Height());
  CDC imagedc;
  imagedc.CreateCompatibleDC(&dc);
  imagedc.SelectObject(&bmp);
  imagedc.BitBlt(0,0,m_rect.Width(),m_rect.Height(),&dc,0,0,SRCCOPY);
  BITMAP bmap;
  bmp.GetBitmap(&bmap);
  int panelsize=0;
  if(bmap.bmBitsPixel<16)
  {
   panelsize=pow(2,bmap.bmBitsPixel*sizeof(RGBQUAD));
  }
  BITMAPINFO* bInfo=(BITMAPINFO*)LocalAlloc(LPTR,sizeof(BITMAPINFO)+panelsize);
  bInfo->bmiHeader.biClrImportant=0;
  bInfo->bmiHeader.biBitCount=bmap.bmBitsPixel;
  bInfo->bmiHeader.biCompression=0;
  bInfo->bmiHeader.biHeight=bmap.bmHeight;
  bInfo->bmiHeader.biPlanes=bmap.bmPlanes;
  bInfo->bmiHeader.biSize=sizeof(BITMAPINFO);
  bInfo->bmiHeader.biSizeImage=bmap.bmWidthBytes*bmap.bmHeight;
  bInfo->bmiHeader.biWidth=bmap.bmWidth;
  bInfo->bmiHeader.biXPelsPerMeter=0;
  bInfo->bmiHeader.biYPelsPerMeter=0;
  char *pData=new char[bmap.bmWidthBytes*bmap.bmHeight];
  ::GetDIBits(imagedc.m_hDC,bmp,0,bmap.bmHeight,pData,bInfo,DIB_RGB_COLORS);
  pdc.StartDoc("print");
  ::StretchDIBits(pdc.m_hDC,0,0,(int)((m_rect.Width()*ratex)),
   (int)((m_rect.Height())*ratey),0,0,m_rect.Width(),
   m_rect.Height(),pData,bInfo,DIB_RGB_COLORS,SRCCOPY);
  pdc.EndDoc();
  LocalFree(bInfo);
  delete[] pData;
  pData = NULL;
  bmp.DeleteObject();
 }
}


void Dg_PopulationEdit::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (nID ==IDR_MENU1)             
    {
		OnPrintPrintwindow();
    }
	  else
	CDialog::OnSysCommand(nID, lParam);
}
