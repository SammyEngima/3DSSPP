// Dg_ClothingColor.cpp : implementation file
//
#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include "Dg_ClothingColor.h"
#include ".\dg_clothingcolor.h"
#include "GLObliqueView.h"
#include "GLOblique.h"

#define BORDER_WIDTH 2			

// Dg_ClothingColor dialog

IMPLEMENT_DYNAMIC(Dg_ClothingColor, CDialog)

Dg_ClothingColor::Dg_ClothingColor(CWnd* pParent) : CDialog(Dg_ClothingColor::IDD, pParent)
{
    pDoc = Get_App_Ptr()->Get_Document_Ptr();

	int gender = pDoc->GenderCode();

    if(gender == 0 || gender == -1) {
		// gender is male
	  
	   GLOblique::shirtColor[0] = 0;
	   GLOblique::shirtColor[1] = 108;
	   GLOblique::shirtColor[2] = 54;
	  
	   GLOblique::pantsColor[0] = 0;
	   GLOblique::pantsColor[1] = 64;
	   GLOblique::pantsColor[2] = 128; 
    }

    if (gender == 1 || gender == -1) {
          	// gender is female

       GLOblique::shirtColor[0] = 204;
	   GLOblique::shirtColor[1] = 0;
	   GLOblique::shirtColor[2] = 153;
	  
	   GLOblique::pantsColor[0] = 51;
	   GLOblique::pantsColor[1] = 0;
	   GLOblique::pantsColor[2] = 255;
    }

   
    GLOblique::pantsRed = GLOblique::pantsColor[0];
    GLOblique::pantsGreen = GLOblique::pantsColor[1];
    GLOblique::pantsBlue = GLOblique::pantsColor[2];

    GLOblique::shirtRed = GLOblique::shirtColor[0];
    GLOblique::shirtGreen = GLOblique::shirtColor[1];
    GLOblique::shirtBlue = GLOblique::shirtColor[2]; 

}

Dg_ClothingColor::~Dg_ClothingColor() { }

void Dg_ClothingColor::DoDataExchange(CDataExchange* pDX) { CDialog::DoDataExchange(pDX); }


BEGIN_MESSAGE_MAP(Dg_ClothingColor, CDialog)
   ON_BN_CLICKED(IDC_PANTCOLOR, OnBnClickedPantcolor)
   ON_BN_CLICKED(IDC_SHIRTCOLOR, OnBnClickedShirtcolor)
   ON_WM_PAINT()
   ON_BN_CLICKED(IDC_CLOTHAPPLY, OnBnClickedClothapply)
   ON_STN_CLICKED(IDC_SHIRTPREV, OnStnClickedShirtprev)
   ON_BN_CLICKED(IDOK, &Dg_ClothingColor::OnBnClickedOk)
END_MESSAGE_MAP()



/* void Dg_ClothingColor::OnBnClickedPantcolor()
{
   pDoc->GenericColorDialog(pantsRed, pantsGreen, pantsBlue);
   InvalidateRect(NULL, 0);
}

void Dg_ClothingColor::OnBnClickedShirtcolor()
{
   pDoc->GenericColorDialog(shirtRed, shirtGreen, shirtBlue);
   InvalidateRect(NULL, 0);
}
*/


void Dg_ClothingColor::OnPaint()
{
   CPaintDC dc(this); // device context for painting

   CWnd *pantFrame = GetDlgItem(IDC_PANTPREV);
   CWnd *shirtFrame = GetDlgItem(IDC_SHIRTPREV);

   CRect pantRect;
   CRect shirtRect;

   pantFrame->GetWindowRect(pantRect);
   shirtFrame->GetWindowRect(shirtRect);

   CRect pantDrawRect(pantRect.left + BORDER_WIDTH,
                      pantRect.top + BORDER_WIDTH,
                      pantRect.right - BORDER_WIDTH,
                      pantRect.bottom - BORDER_WIDTH);
   CRect shirtDrawRect(shirtRect.left + BORDER_WIDTH,
                       shirtRect.top + BORDER_WIDTH,
                       shirtRect.right - BORDER_WIDTH,
                       shirtRect.bottom - BORDER_WIDTH);

   ScreenToClient(pantDrawRect);
   ScreenToClient(shirtDrawRect);
   CBrush shirtBrush(RGB(GLOblique::shirtRed, GLOblique::shirtGreen, GLOblique::shirtBlue));
   CBrush pantBrush(RGB(GLOblique::pantsRed, GLOblique::pantsGreen, GLOblique::pantsBlue));

   dc.FillRect(pantDrawRect, &pantBrush);
   dc.FillRect(shirtDrawRect, &shirtBrush);
}



// Sayan 07/18/07
// 
// We need to pass gender as a parameter to UpdateDocument,
// since male and female figures have differently colored clothing
//


void Dg_ClothingColor::OnBnClickedClothapply()
{
   if(this->UpdateData(TRUE))
      this->UpdateDocument();
}


// Sayan 07/18/07
//
// We now have separate clothing colors for male and female figures.


bool  Dg_ClothingColor::UpdateDocument()
{
	

		GLOblique::shirtColor[0] = GLOblique::shirtRed;
        GLOblique::shirtColor[1] = GLOblique::shirtGreen;
        GLOblique::shirtColor[2] = GLOblique::shirtBlue; 
          	
		GLOblique::pantsColor[0] = GLOblique::pantsRed;
        GLOblique::pantsColor[1] = GLOblique::pantsGreen;
        GLOblique::pantsColor[2] = GLOblique::pantsBlue;

		pDoc = Get_App_Ptr()->Get_Document_Ptr();
        int gender = pDoc->GenderCode();

		if(gender == 0 || gender == -1) {

			// gender is male
			GLOblique::maleShirtColor[0] = GLOblique::shirtRed;
            GLOblique::maleShirtColor[1] = GLOblique::shirtGreen;
            GLOblique::maleShirtColor[2] = GLOblique::shirtBlue; 
          	
	     	GLOblique::malePantsColor[0] = GLOblique::pantsRed;
            GLOblique::malePantsColor[1] = GLOblique::pantsGreen;
            GLOblique::malePantsColor[2] = GLOblique::pantsBlue;

		}

		if(gender == 1 || gender == -1) {

			// gender is female
			GLOblique::femaleShirtColor[0] = GLOblique::shirtRed;
            GLOblique::femaleShirtColor[1] = GLOblique::shirtGreen;
            GLOblique::femaleShirtColor[2] = GLOblique::shirtBlue; 
          	
	     	GLOblique::femalePantsColor[0] = GLOblique::pantsRed;
            GLOblique::femalePantsColor[1] = GLOblique::pantsGreen;
            GLOblique::femalePantsColor[2] = GLOblique::pantsBlue;

		}
	    pDoc->MakeDirtyAndUpdateViews(false);
        return true;
}



void Dg_ClothingColor::OnOK() 
{
    if(this->UpdateData(TRUE))
    {
        if(this->UpdateDocument()) //was successful
        {
            CDialog::OnOK(); //go ahead and exit the dialog
        }
    }
}

void Dg_ClothingColor::OnStnClickedShirtprev()
{
	// TODO: Add your control notification handler code here
}

void Dg_ClothingColor::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
