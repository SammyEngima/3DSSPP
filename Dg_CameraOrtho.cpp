// Dg_CameraOrtho.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include "difcount.hpp"
#include "Dg_CameraOrtho.h"
#include "graph_db.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_CameraOrtho dialog


Dg_CameraOrtho::Dg_CameraOrtho(C_Hom_Doc * aDocPtr, CWnd* pParent /*=NULL*/)
	: CDialog(Dg_CameraOrtho::IDD, pParent),
	mDocPtr( aDocPtr ) ,
	mDifs()
{
	//{{AFX_DATA_INIT(Dg_CameraOrtho)
	mFrontFocalLength = 0;
	mSideFocalLength = 0;
	mTopFocalLength = 0;
	//}}AFX_DATA_INIT

	mFrontFocalLength = mDocPtr->getFront_Cam_Focal_Length();
	mSideFocalLength = mDocPtr->getSide_Cam_Focal_Length();
	mTopFocalLength = mDocPtr->getTop_Cam_Focal_Length();

	ASSERT( mDocPtr != NULL );
}


void Dg_CameraOrtho::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_CameraOrtho)
	DDX_Text(pDX, IDC_FRONT_FOCAL_LENGTH, mFrontFocalLength);
	DDV_MinMaxInt(pDX, mFrontFocalLength, 0, 4096);
	DDX_Text(pDX, IDC_SIDE_FOCAL_LENGTH, mSideFocalLength);
	DDV_MinMaxInt(pDX, mSideFocalLength, 0, 4096);
	DDX_Text(pDX, IDC_TOP_FOCAL_LENGTH, mTopFocalLength);
	DDV_MinMaxInt(pDX, mTopFocalLength, 0, 4096);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Dg_CameraOrtho, CDialog)
	//{{AFX_MSG_MAP(Dg_CameraOrtho)
	ON_BN_CLICKED(IDC_RedrawButton, OnRedrawButton)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_CameraOrtho message handlers
void Dg_CameraOrtho::UpdateDocument(void)
{
	this->BeginWaitCursor();
	{
		int tempLen = mDocPtr->getFront_Cam_Focal_Length();
		mDifs.Update( mFrontFocalLength, tempLen );
		mDocPtr->setFront_Cam_Focal_Length(tempLen);

		tempLen = mDocPtr->getSide_Cam_Focal_Length();
		mDifs.Update( mSideFocalLength , tempLen );
		mDocPtr->setSide_Cam_Focal_Length(tempLen);

		tempLen = mDocPtr->getTop_Cam_Focal_Length();
		mDifs.Update( mTopFocalLength  , tempLen );
		mDocPtr->setTop_Cam_Focal_Length(tempLen);
		
		if( mDifs.Present() )
		{
			Graphic_Database*   GDBPtr = mDocPtr->Get_Graphic_Database_Ptr();
			C_Hom_App&          App = *Get_App_Ptr();
			
			GDBPtr->Camera_Modify(App.Fro_Cam_Name, mFrontFocalLength, 26.0F, 35.0F);
			GDBPtr->Camera_Modify(App.Sid_Cam_Name, mSideFocalLength,  26.0F, 35.0F);
			GDBPtr->Camera_Modify(App.Top_Cam_Name, mTopFocalLength,   26.0F, 35.0F);

			mDocPtr->MakeDirtyAndUpdateViews(); 
		}
	}  
	this->EndWaitCursor();
	
	return;
}

void Dg_CameraOrtho::OnOK() 
{
   if( this->UpdateData( TRUE ) )
   {
      this->UpdateDocument();   	
      CDialog::OnOK();
   }
}

void Dg_CameraOrtho::OnRedrawButton() 
{
   if( this->UpdateData( TRUE ) )
   {
      this->UpdateDocument();   	
   }	
}

void Dg_CameraOrtho::OnDefault() 
{
	C_Hom_App&          App = *Get_App_Ptr();
	
	mFrontFocalLength =	50 * App.Ortho_Factor;		
	mSideFocalLength  =	50 * App.Ortho_Factor;
	mTopFocalLength   =	50 * App.Ortho_Factor; 
//	mBalFocalLength   =	50 * App.Ortho_Factor; 
	
	
    if ( this->UpdateData(FALSE) )			// Update dialog with new mVars
	{
		if( this->UpdateData(TRUE) )		// Read dialog back to mVars [why? I dont know... cuz I feel like it... make SURE the dialog is synched]
		{
			this->UpdateDocument();			// Make the program take the changes
		}	
	}
}
