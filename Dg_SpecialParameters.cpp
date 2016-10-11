// Dg_SpecialParameters.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "Dg_SpecialParameters.h"
#include "length.hpp"
#include "hom_doc.h" // Necessary
#include ".\dg_specialparameters.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_SpecialParameters dialog


Dg_SpecialParameters::Dg_SpecialParameters( C_Hom_Doc *   aDocPtr ,CWnd* pParent /*=NULL*/)
    : CDialog(Dg_SpecialParameters::IDD, pParent),
    mDocPtr( aDocPtr )
{
    //{{AFX_DATA_INIT(Dg_SpecialParameters)
    mDeltaX = 0.0f;
    mDeltaY = 0.0f;
    mUnits1 = _T("");
    mPelvicTilt = 0.0f;
    mStance = -1;
    mRightFootAngle = 0.0f;
	mLeftFootAngle = 0.0f;
    //}}AFX_DATA_INIT
    Length l(0,1);
    mUnits1 = CString("") + "(" + l.UnitsText() + ")";
    mPelvicTilt = mDocPtr->getPelvicTilt();
    mStance   = mDocPtr->getStance();
    mRightFootAngle = mDocPtr->getRightFootAngle();
	mLeftFootAngle = mDocPtr->getLeftFootAngle();
}


void Dg_SpecialParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_SpecialParameters)
	DDX_Text(pDX, IDC_deltaX, mDeltaX);
	DDX_Text(pDX, IDC_deltaY, mDeltaY);
	DDX_Text(pDX, IDC_UnitsLabel, mUnits1);
	DDX_Text(pDX, IDC_ADD_PELVIC_TILT, mPelvicTilt);
	DDX_Radio(pDX, IDC_STANCE, mStance);
	DDX_Text(pDX, IDC_RightFootAngle, mRightFootAngle);
	DDX_Text(pDX, IDC_LeftFootAngle, mLeftFootAngle);
	DDV_MinMaxDouble(pDX, mRightFootAngle, -90.f, 90.f);
	DDV_MinMaxDouble(pDX, mLeftFootAngle, -90.f, 90.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Dg_SpecialParameters, CDialog)
    //{{AFX_MSG_MAP(Dg_SpecialParameters)
    ON_BN_CLICKED(ID_STATIC, OnStatic)
    ON_BN_CLICKED(IDC_REDRAW, OnRedraw)
    //}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_STANCE, OnBnClickedStance)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_SpecialParameters message handlers

// ===============
// OnOK() for Special Seating Parameters Dialog
//
// - Run when the OK button is pressed
// - Tells the graphics dialog box to used the modified pelvic angle
// ===============
void Dg_SpecialParameters::OnOK() 
{
    // ACT::ADDED [05/02/01]
    // Use the PelvicAngleModify function built into the Graphics dialog to
    // set a new default pelvic angle.

    if (this->UpdateData(TRUE)) // put data from editboxes into member variables
    {
        UpdateDocument();
        CDialog::OnOK();    // Do default windows Dialog OnOK processing
    }
    return;
}

void Dg_SpecialParameters::OnStatic() 
{
    // TO DO: Add your control notification handler code here
    
}


void Dg_SpecialParameters::UpdateDocument()
{
    mDocPtr->setDeltaX(mDeltaX);
    mDocPtr->setDeltaY(mDeltaY);
    mDocPtr->setPelvicTilt(mPelvicTilt);
    mDocPtr->setStance(mStance);
	mDocPtr->setRightFootAngle(mRightFootAngle);
	mDocPtr->setLeftFootAngle(mLeftFootAngle);
    Get_App_Ptr()->Get_GDB_Ptr()->PelvicAngleModify ( Get_App_Ptr()->Hom_Ske_Name, this->mPelvicTilt );
    mDocPtr->UpdateStamp();
    mDocPtr->MakeDirtyAndUpdateViews();

}
void Dg_SpecialParameters::OnRedraw() 
{
   if( this->UpdateData( TRUE ) )
   {
      this->UpdateDocument() ;
   }
   return ;
}

void Dg_SpecialParameters::OnBnClickedStance()
{
	// TODO: Add your control notification handler code here
}

void Dg_SpecialParameters::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
}
