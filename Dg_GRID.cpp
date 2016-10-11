// Dg_GRID.cpp : implementation file
//

#include "stdafx.h"
#include "Dg_GRID.h"
#include "afxdialogex.h"
#include "GLStickView.h"
#include "windef.h"
#include "Hom.h"
#include "Hom_doc.h"
#include <ctype.h> 

#define BORDER_WIDTH 2
// Dg_GRID dialog 

IMPLEMENT_DYNAMIC(Dg_GRID, CDialog)

Dg_GRID::Dg_GRID(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_GRID::IDD, pParent)
	, GridSpacing(1)
	, CustomSize(0)
{
	show = GLSkeleton::getShowGrid();
	if(GLSkeleton::getGridSize() == 10)
		GridSpacing = 0;
	else if (GLSkeleton::getGridSize() == 20)
		 GridSpacing = 1;
	else if(GLSkeleton::getGridSize() == 30)
		GridSpacing = 2;
	else
		 GridSpacing = 3;
	memcpy(GridColor, GLSkeleton::GridColor,3*sizeof(unsigned char));
}

Dg_GRID::~Dg_GRID()
{
}

void Dg_GRID::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECKGRID, show);
	DDX_Radio(pDX, IDC_GridSize_10, GridSpacing);
	DDX_Text(pDX, IDC_GridSize, CustomSize);
	DDX_Text(pDX, IDC_UNITS, units);
	DDX_Text(pDX, IDC_Units1, units1);
	DDX_Text(pDX, IDC_Units2, units2);
	DDX_Text(pDX, IDC_Units3, units3);
}


BEGIN_MESSAGE_MAP(Dg_GRID, CDialog)
	ON_BN_CLICKED(IDOK, &Dg_GRID::OnBnClickedOk)
	ON_BN_CLICKED(IDC_GridSize_10, &Dg_GRID::OnBnClickedGridsize10)
	ON_BN_CLICKED(IDC_GridSize_20, &Dg_GRID::OnBnClickedGridsize20)
	ON_BN_CLICKED(IDC_GridSize_30, &Dg_GRID::OnBnClickedGridsize30)
	ON_BN_CLICKED(IDC_GridSize_Custom, &Dg_GRID::OnBnClickedGridsizeCustom)
	ON_BN_CLICKED(IDC_GridButton, &Dg_GRID::OnBnClickedGridbutton)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDAPPLAY, &Dg_GRID::OnBnClickedApplay)
END_MESSAGE_MAP()


// Dg_GRID message handlers


void Dg_GRID::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if ( GridSpacing == 3 && CustomSize <= 5 || CustomSize > 200 )
	{
		AfxMessageBox("Warning : Please set the Grid Spacing between 5 and 200!");
		return;
	}
	else{ 
	UpdateData();
	GLSkeleton::setShowGrid(show); //show or hide the grid
	 if(GridSpacing == 0)
	 {
		 GLSkeleton::setGridSize(10);
	 }
	 	 else if(GridSpacing == 1)
	 {
		 GLSkeleton::setGridSize(20);
	 }
		 else if(GridSpacing == 2)
	 {
		 GLSkeleton::setGridSize(30);
	 }
		 else if(GridSpacing == 3)
	 {
		 GLSkeleton::setGridSize(CustomSize);
	 }
		 for(int i = 0; i < 3; ++i)
		 {
			 GLSkeleton::GridColor[i] = GridColor[i];
		 }
		 this->CWnd::UpdateData();
		 C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
		 doc->MakeDirtyAndUpdateViews(true);
	CDialog::OnOK();
	}
}


void Dg_GRID::OnBnClickedGridsize10()
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = GetDlgItem(IDC_GridSize);
	pWnd->EnableWindow(FALSE);
}


void Dg_GRID::OnBnClickedGridsize20()
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = GetDlgItem(IDC_GridSize);
	pWnd->EnableWindow(FALSE);
}


void Dg_GRID::OnBnClickedGridsize30()
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = GetDlgItem(IDC_GridSize);
	pWnd->EnableWindow(FALSE);
}


void Dg_GRID::OnBnClickedGridsizeCustom()
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = GetDlgItem(IDC_GridSize);
	pWnd->EnableWindow(TRUE);
}


BOOL Dg_GRID::OnInitDialog()
{
	CDialog::OnInitDialog();
	 C_Hom_Doc* mDocPtr = Get_App_Ptr()->Get_Document_Ptr();
	 units = mDocPtr->getAnthropometryRef().HeightUnitsText();
	units1 = mDocPtr->getAnthropometryRef().HeightUnitsText();
	units2 = mDocPtr->getAnthropometryRef().HeightUnitsText();
	units3 = mDocPtr->getAnthropometryRef().HeightUnitsText();

	// TODO:  Add extra initialization here
		if(GridSpacing == 3)
	{
		CWnd *pWnd = GetDlgItem(IDC_GridSize);
		pWnd->EnableWindow(TRUE);
	}
	else
	{
		 GetDlgItem(IDC_GridSize)->EnableWindow(FALSE);
	}
	CustomSize = GLSkeleton::getGridSize();
	UpdateData(FALSE); 
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void Dg_GRID::OnBnClickedGridbutton()
{
	// TODO: Add your control notification handler code here
	     COLORREF Ini_Color = RGB(GridColor[0],GridColor[1],GridColor[2] );
     CColorDialog Diag(Ini_Color);
     if(Diag.DoModal() == IDOK)
     {
        COLORREF New_Color = Diag.GetColor();
        if(New_Color != Ini_Color)
        {
          GridColor[0] = GetRValue(New_Color);
          GridColor[1] = GetGValue(New_Color);
          GridColor[2] = GetBValue(New_Color);
        }
	 }
     Invalidate(FALSE); 
}

void Dg_GRID::FillRect(CPaintDC* dc, const int nIDDlgItem, BYTE& red, BYTE& green, BYTE& blue)
{
	CWnd *colorFrame = GetDlgItem(nIDDlgItem);
	CRect ColorRect;
	colorFrame->GetWindowRect(ColorRect);
	CRect ColorDrawRect(ColorRect.left + BORDER_WIDTH,ColorRect.top + BORDER_WIDTH,
						ColorRect.right - BORDER_WIDTH,ColorRect.bottom - BORDER_WIDTH);

	ScreenToClient(ColorDrawRect);
	CBrush colorBrush(RGB(red, green, blue));
	dc->FillRect(ColorDrawRect, &colorBrush);
	return;
}
void Dg_GRID::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	FillRect(&dc, IDC_GRIDCOLOR, GridColor[0], GridColor[1], GridColor[2]);
	// Do not call CDialog::OnPaint() for painting messages
}


void Dg_GRID::OnBnClickedApplay()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (GridSpacing == 3 && CustomSize <= 5 || CustomSize > 200 )
	{
		AfxMessageBox("Warning : Please set the Grid Spacing between 5 and 200!");
		return;
	}
	else
	{
	GLSkeleton::setShowGrid(show); //show or hide the grid
	 if(GridSpacing == 0)
	 {
		 GLSkeleton::setGridSize(10);
	 }
	 	 else if(GridSpacing == 1)
	 {
		 GLSkeleton::setGridSize(20);
	 }
		 else if(GridSpacing == 2)
	 {
		 GLSkeleton::setGridSize(30);
	 }
		 else if(GridSpacing == 3)
	 {
		 GLSkeleton::setGridSize(CustomSize);
	 }
		 for(int i = 0; i < 3; ++i)
		 {
			 GLSkeleton::GridColor[i] = GridColor[i];
		 }
		 this->CWnd::UpdateData();
		 C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
		 doc->MakeDirtyAndUpdateViews(true);
	}
}
