#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h"
#include ".\Dg_ObliqueFigure.h"
#include "GLObliqueView.h"
#include "GLOblique.h"
#include "windef.h"
// #include enums.h
#include "Types.h"

#ifdef _DEBUG
    #undef THIS_FILE
    static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#undef BORDER_WIDTH
#define	BORDER_WIDTH 2  // Yes... it is redefined in a bunch of places but it
//    would allow you to change the width depending
//    on the dialog box

Dg_ObliqueFigure::Dg_ObliqueFigure(CWnd* pParent) : CDialog(Dg_ObliqueFigure::IDD, pParent)//, mShoeEnable(false)
{	
   //{{AFX_DATA_INIT(Dg_ObliqueFigure)
   Model_Mode = 0;
   //}}AFX_DATA_INIT
}

void Dg_ObliqueFigure::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_FLESH, Model_Mode);
}

bool Dg_ObliqueFigure::UpdateDocument()
{
    C_Hom_Doc* lDocPtr = Get_App_Ptr()->Get_Document_Ptr();
    CGLObliqueView *oblique = (CGLObliqueView*)(lDocPtr->GLViews[VIEW_OBLIQUE]);

    switch(Model_Mode)
    {
        case 0:
            oblique->setModelType(MT_FLESH);
            break;
        case 1:
            oblique->setModelType(MT_WIREFRAME);
            break;
        case 2:
            oblique->setModelType(MT_NONE);
            break;
	}

    C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
    int gender = pDoc->GenderCode();

	GLOblique::skinColor[0] = GetRValue(skin);
	GLOblique::skinColor[1] = GetGValue(skin);
	GLOblique::skinColor[2] = GetBValue(skin);

	GLOblique::maleShirtColor[0] = GetRValue(maleShirt);
    GLOblique::maleShirtColor[1] = GetGValue(maleShirt);
    GLOblique::maleShirtColor[2] = GetBValue(maleShirt);

	GLOblique::malePantsColor[0] = GetRValue(malePants);
    GLOblique::malePantsColor[1] = GetGValue(malePants);
    GLOblique::malePantsColor[2] = GetBValue(malePants);

	GLOblique::femaleShirtColor[0] = GetRValue(femaleShirt);
    GLOblique::femaleShirtColor[1] = GetGValue(femaleShirt);
    GLOblique::femaleShirtColor[2] = GetBValue(femaleShirt);

	GLOblique::femalePantsColor[0] = GetRValue(femalePants);
    GLOblique::femalePantsColor[1] = GetGValue(femalePants);
    GLOblique::femalePantsColor[2] = GetBValue(femalePants);

    // Now refresh screen
    lDocPtr->MakeDirtyAndUpdateViews(false);
	return true;
}

BEGIN_MESSAGE_MAP(Dg_ObliqueFigure, CDialog)
    
    ON_BN_CLICKED(IDAPPLY, OnApply)
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_FIGURECOLOR, OnBnClickedFigurecolor)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_MALESSHIRTCOLOR, &Dg_ObliqueFigure::OnBnClickedMaleShirtcolor)
	ON_BN_CLICKED(IDC_FEMALESSHIRTCOLOR, &Dg_ObliqueFigure::OnBnClickedFemaleShirtcolor)
	ON_BN_CLICKED(IDC_MALESPANTSCOLOR, &Dg_ObliqueFigure::OnBnClickedMalePantscolor)
	ON_BN_CLICKED(IDC_FEMALESPANTSCOLOR, &Dg_ObliqueFigure::OnBnClickedFemalePantscolor)

END_MESSAGE_MAP()


void Dg_ObliqueFigure::OnApply() 
{
    if(this->UpdateData(TRUE))     // Write on-screen to variables
        this->UpdateDocument();    // Hand-written function
}

void Dg_ObliqueFigure::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    CBrush maleshirtBrush(maleShirt);
    CBrush malepantBrush(malePants);
    dc.FillRect(maleShirtDrawRect, &maleshirtBrush);
    dc.FillRect(malePantDrawRect, &malepantBrush);
  
    CBrush femaleshirtBrush(femaleShirt);
    CBrush femalepantBrush(femalePants);
    dc.FillRect(femaleShirtDrawRect, &femaleshirtBrush);
    dc.FillRect(femalePantDrawRect, &femalepantBrush);

    CBrush colorBrush(skin);
    dc.FillRect(ColorDrawRect, &colorBrush);
}

afx_msg void Dg_ObliqueFigure::OnBnClickedOk()
{
    if(this->UpdateData(TRUE))
    {
        if(this->UpdateDocument()) //was successful
        {
            CDialog::OnOK(); //go ahead and exit the dialog
        }
    }
}

void Dg_ObliqueFigure:: OnBnClickedFigurecolor()
{
    CColorDialog Diag(skin);
    if(Diag.DoModal() == IDOK)
    {
        COLORREF New_Color = Diag.GetColor();
        if(New_Color != skin)
        {
			skin = New_Color;
        }
    }
    Invalidate(FALSE); 
}

void Dg_ObliqueFigure::OnBnClickedMaleShirtcolor()
{
     CColorDialog Diag(maleShirt);
     if(Diag.DoModal() == IDOK)
     {
        COLORREF New_Color = Diag.GetColor();
        if(New_Color != maleShirt)
        {
			maleShirt = New_Color;
        }
	 }
     Invalidate(FALSE); 
}

void Dg_ObliqueFigure::OnBnClickedFemaleShirtcolor()
{
	CColorDialog Diag(femaleShirt);
    if(Diag.DoModal() == IDOK)
    {
        COLORREF New_Color = Diag.GetColor();
		if(New_Color != femaleShirt)
        {
			femaleShirt = New_Color;
        }
    }
    Invalidate(FALSE); 
 
}

void Dg_ObliqueFigure::OnBnClickedMalePantscolor()
{
    CColorDialog Diag(malePants);
    if(Diag.DoModal() == IDOK)
    {
        COLORREF New_Color = Diag.GetColor();
        if(New_Color != malePants)
        {
			malePants = New_Color;
        }
    }
    Invalidate(FALSE); 
}

void Dg_ObliqueFigure::OnBnClickedFemalePantscolor()
{
    CColorDialog Diag(femalePants);
    if(Diag.DoModal() == IDOK)
    {
        COLORREF New_Color = Diag.GetColor();
        if(New_Color != femalePants)
        {
			femalePants = New_Color;
        }
    }
    Invalidate(FALSE); 
}


BOOL Dg_ObliqueFigure::OnInitDialog()
{
	CDialog::OnInitDialog();

	C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();

	// get colors from globlique class
	skin = RGB(GLOblique::skinColor[0], GLOblique::skinColor[1], GLOblique::skinColor[2]);
	maleShirt = RGB(GLOblique::maleShirtColor[0], GLOblique::maleShirtColor[1], GLOblique::maleShirtColor[2]);
	malePants = RGB(GLOblique::malePantsColor[0], GLOblique::malePantsColor[1], GLOblique::malePantsColor[2]);
	femaleShirt = RGB(GLOblique::femaleShirtColor[0], GLOblique::femaleShirtColor[1], GLOblique::femaleShirtColor[2]);
	femalePants = RGB(GLOblique::femalePantsColor[0], GLOblique::femalePantsColor[1], GLOblique::femalePantsColor[2]);

    CWnd *ColorFrame = GetDlgItem(IDC_FIGURECOLORPREV);
	CWnd *malePantFrame = GetDlgItem(IDC_MALEPANTSCOLOR);
    CWnd *maleShirtFrame = GetDlgItem(IDC_MALESHIRTCOLOR);
	CWnd *femalePantFrame = GetDlgItem(IDC_FEMALEPANTSCOLOR);
    CWnd *femaleShirtFrame = GetDlgItem(IDC_FEMALESHIRTCOLOR);

	CRect ColorRect;
	CRect malePantRect;
	CRect maleShirtRect;
	CRect femalePantRect;
	CRect femaleShirtRect;

    ColorFrame->GetWindowRect(ColorRect);
	malePantFrame->GetWindowRect(malePantRect);
	femalePantFrame->GetWindowRect(femalePantRect);
    maleShirtFrame->GetWindowRect(maleShirtRect);
	femaleShirtFrame->GetWindowRect(femaleShirtRect);

    ColorDrawRect = CRect(ColorRect.left + BORDER_WIDTH,
        ColorRect.top + BORDER_WIDTH,
        ColorRect.right - BORDER_WIDTH,
        ColorRect.bottom - BORDER_WIDTH);

    malePantDrawRect = CRect(malePantRect.left + BORDER_WIDTH,
                      malePantRect.top + BORDER_WIDTH,
                      malePantRect.right - BORDER_WIDTH,
                      malePantRect.bottom - BORDER_WIDTH);
	
    femalePantDrawRect = CRect(femalePantRect.left + BORDER_WIDTH,
                      femalePantRect.top + BORDER_WIDTH,
                      femalePantRect.right - BORDER_WIDTH,
                      femalePantRect.bottom - BORDER_WIDTH);

    maleShirtDrawRect = CRect(maleShirtRect.left + BORDER_WIDTH,
                       maleShirtRect.top + BORDER_WIDTH,
                       maleShirtRect.right - BORDER_WIDTH,
                       maleShirtRect.bottom - BORDER_WIDTH);

    femaleShirtDrawRect = CRect(femaleShirtRect.left + BORDER_WIDTH,
                       femaleShirtRect.top + BORDER_WIDTH,
                       femaleShirtRect.right - BORDER_WIDTH,
                       femaleShirtRect.bottom - BORDER_WIDTH);

    ScreenToClient(ColorDrawRect);
	ScreenToClient(malePantDrawRect);
    ScreenToClient(femalePantDrawRect);
    ScreenToClient(maleShirtDrawRect);
    ScreenToClient(femaleShirtDrawRect);

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
