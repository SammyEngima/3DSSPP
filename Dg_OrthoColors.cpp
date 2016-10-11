#include "stdafx.h"
#include "Hom.h"
#include "Hom_doc.h"
#include "Dg_OrthoColors.h"
#include ".\dg_orthocolors.h"
#include "GLStickView.h"
#include "GLSkeleton.h"
#include "windef.h"

#define BORDER_WIDTH 2

IMPLEMENT_DYNAMIC(Dg_OrthoColors, CDialog)

Dg_OrthoColors::Dg_OrthoColors(CWnd* pParent) : CDialog(Dg_OrthoColors::IDD, pParent)
{
    pDoc = Get_App_Ptr()->Get_Document_Ptr();
    assert(pDoc != NULL);

	memcpy(linkColor,GLSkeleton::skeletonColor,3*sizeof(unsigned char));
    memcpy(backgroundColor,GLSkeleton::backgroundColor,3*sizeof(unsigned char));
    memcpy(forceColor,GLSkeleton::forceColor,3*sizeof(unsigned char));
    memcpy(floorFillColor,GLSkeleton::floorColor,3*sizeof(unsigned char));
	memcpy(leftJointColor,GLSkeleton::leftJointColor,3*sizeof(unsigned char));
    memcpy(rightJointColor,GLSkeleton::rightJointColor,3*sizeof(unsigned char));
	memcpy(floorOutlineColor,GLSkeleton::floorOutlineColor,3*sizeof(unsigned char));
	// new color scheme
	memcpy(jointColor, GLSkeleton::jointColor,3*sizeof(unsigned char));
	memcpy(leftLinkColor, GLSkeleton::leftLinkColor,3*sizeof(unsigned char));
	memcpy(rightLinkColor, GLSkeleton::rightLinkColor,3*sizeof(unsigned char));
	memcpy(rightfeetforceColor,GLSkeleton::rightfeetforceColor,3*sizeof(unsigned char));
	memcpy(leftfeetforceColor,GLSkeleton::leftfeetforceColor,3*sizeof(unsigned char));
}

Dg_OrthoColors::~Dg_OrthoColors() { }

void Dg_OrthoColors::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Dg_OrthoColors, CDialog)
	ON_BN_CLICKED(IDC_FLOORFILLCOLOR, OnBnClickedFloorfillcolor)
	ON_BN_CLICKED(IDC_FORCECOLOR, OnBnClickedForcecolor)
	ON_BN_CLICKED(IDC_FORCECOLOR2, OnBnClickedRightFeetForcecolor)
	ON_BN_CLICKED(IDC_FORCECOLOR2, OnBnClickedLeftFeetForcecolor)
	ON_BN_CLICKED(IDC_BACKGROUNDCOLOR, OnBnClickedBackgroundcolor)
	ON_BN_CLICKED(IDC_FLOOROUTLINECOLOR, OnBnClickedFlooroutlinecolor)
	ON_BN_CLICKED(IDC_LINKKCOLOR, OnBnClickedLinkcolor)
	ON_BN_CLICKED(IDC_LEFTTJOINTSCOLOR, OnBnClickedLeftjointscolor)
	ON_BN_CLICKED(IDC_RIGHTTJOINTSCOLOR,OnBnClickedRightjointscolor)
	ON_BN_CLICKED(IDC_JOINTSCOLOR, OnBnClickedJointscolor)
	ON_BN_CLICKED(IDC_LEFTLINKCOLOR, OnBnClickedLeftlinkcolor)
	ON_BN_CLICKED(IDC_RIGHTLINKCOLOR, OnBnClickedRightlinkcolor)
    ON_WM_PAINT()
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)	
	
END_MESSAGE_MAP()



void Dg_OrthoColors::OnBnClickedFloorfillcolor()
{
     // C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
	
     COLORREF Ini_Color = RGB(floorFillColor[0],floorFillColor[1],floorFillColor[2] );
     CColorDialog Diag(Ini_Color);
     if(Diag.DoModal() == IDOK)
     {
        COLORREF New_Color = Diag.GetColor();
        if(New_Color != Ini_Color)
        {
          floorFillColor[0] = GetRValue(New_Color);
          floorFillColor[1] = GetGValue(New_Color);
          floorFillColor[2] = GetBValue(New_Color);
        }
	 }
     Invalidate(FALSE); 
}

void Dg_OrthoColors::OnBnClickedForcecolor()
{
     // C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
     
	
     COLORREF Ini_Color = RGB(forceColor[0],forceColor[1],forceColor[2] );
     CColorDialog Diag(Ini_Color);
     if(Diag.DoModal() == IDOK)
     {
        COLORREF New_Color = Diag.GetColor();
        if(New_Color != Ini_Color)
        {
          forceColor[0] = GetRValue(New_Color);
          forceColor[1] = GetGValue(New_Color);
          forceColor[2] = GetBValue(New_Color);
        }
	 }
     Invalidate(FALSE); 
}

void Dg_OrthoColors::OnBnClickedRightFeetForcecolor()
{
     // C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
     
	
     COLORREF Ini_Color = RGB(rightfeetforceColor[0],rightfeetforceColor[1],rightfeetforceColor[2] );
     CColorDialog Diag(Ini_Color);
     if(Diag.DoModal() == IDOK)
     {
        COLORREF New_Color = Diag.GetColor();
        if(New_Color != Ini_Color)
        {
          rightfeetforceColor[0] = GetRValue(New_Color);
          rightfeetforceColor[1] = GetGValue(New_Color);
          rightfeetforceColor[2] = GetBValue(New_Color);
        }
	 }
     Invalidate(FALSE); 
}

void Dg_OrthoColors::OnBnClickedLeftFeetForcecolor()
{
     // C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
     
	
     COLORREF Ini_Color = RGB(leftfeetforceColor[0],leftfeetforceColor[1],leftfeetforceColor[2] );
     CColorDialog Diag(Ini_Color);
     if(Diag.DoModal() == IDOK)
     {
        COLORREF New_Color = Diag.GetColor();
        if(New_Color != Ini_Color)
        {
          leftfeetforceColor[0] = GetRValue(New_Color);
          leftfeetforceColor[1] = GetGValue(New_Color);
          leftfeetforceColor[2] = GetBValue(New_Color);
        }
	 }
     Invalidate(FALSE); 
}

void Dg_OrthoColors::OnBnClickedBackgroundcolor()
{
     // C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
     
	
     COLORREF Ini_Color = RGB(backgroundColor[0],backgroundColor[1],backgroundColor[2] );
     CColorDialog Diag(Ini_Color);
     if(Diag.DoModal() == IDOK)
     {
        COLORREF New_Color = Diag.GetColor();
        if(New_Color != Ini_Color)
        {
          backgroundColor[0] = GetRValue(New_Color);
          backgroundColor[1] = GetGValue(New_Color);
          backgroundColor[2] = GetBValue(New_Color);
        }
	 }
     Invalidate(FALSE); 
}

void Dg_OrthoColors::OnBnClickedFlooroutlinecolor()
{
     // C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
     
	
     COLORREF Ini_Color = RGB(floorOutlineColor[0],floorOutlineColor[1],floorOutlineColor[2] );
     CColorDialog Diag(Ini_Color);
     if(Diag.DoModal() == IDOK)
     {
        COLORREF New_Color = Diag.GetColor();
        if(New_Color != Ini_Color)
        {
          floorOutlineColor[0] = GetRValue(New_Color);
          floorOutlineColor[1] = GetGValue(New_Color);
          floorOutlineColor[2] = GetBValue(New_Color);
        }
	 }
     Invalidate(FALSE); 
}

bool Dg_OrthoColors::UpdateDocument()
{
	bool changed = false;	

	// compare with and update current color settings
	for(int i = 0; i < 3; ++i)
	{

		if(GLSkeleton::skeletonColor[i] != linkColor[i]) {
			GLSkeleton::skeletonColor[i] = linkColor[i];
			changed = true;
		}
		if(GLSkeleton::backgroundColor[i] != backgroundColor[i])
		{
			GLSkeleton::backgroundColor[i] = backgroundColor[i];
			changed = true;
		}
		if(GLSkeleton::forceColor[i] != forceColor[i])
		{
			GLSkeleton::forceColor[i] = forceColor[i];
			changed = true;
		}
		if(GLSkeleton::rightfeetforceColor[i] != rightfeetforceColor[i])
		{
			GLSkeleton::rightfeetforceColor[i] = rightfeetforceColor[i];
			changed = true;
		}
		if(GLSkeleton::leftfeetforceColor[i] != leftfeetforceColor[i])
		{
			GLSkeleton::leftfeetforceColor[i] = leftfeetforceColor[i];
			changed = true;
		}
		if(GLSkeleton::floorColor[i] != floorFillColor[i])
		{
			GLSkeleton::floorColor[i] = floorFillColor[i];
			changed = true;
		}
		if(GLSkeleton::floorOutlineColor[i] != floorOutlineColor[i])
		{
			GLSkeleton::floorOutlineColor[i] = floorOutlineColor[i];
			changed = true;
		}
		if(GLSkeleton::rightJointColor[i] != rightJointColor[i])
		{
			GLSkeleton::rightJointColor[i] = rightJointColor[i];
			changed = true;
		}
		if(GLSkeleton::leftJointColor[i] != leftJointColor[i])
		{
			GLSkeleton::leftJointColor[i] = leftJointColor[i];
			changed = true;
		}
		if(GLSkeleton::jointColor[i] != jointColor[i])
		{
			GLSkeleton::jointColor[i] = jointColor[i];
			changed = true;
		}
		if(GLSkeleton::leftLinkColor[i] != leftLinkColor[i])
		{
			GLSkeleton::leftLinkColor[i] = leftLinkColor[i];
			changed = true;
		}
		if(GLSkeleton::rightLinkColor[i] != rightLinkColor[i])
		{
			GLSkeleton::rightLinkColor[i] = rightLinkColor[i];
			changed = true;
		}
		// currently no floor outline update needed
	}

	if(changed) pDoc->MakeDirtyAndUpdateViews();

	return true;
}

void Dg_OrthoColors::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	FillRect(&dc, IDC_BGCOLORPREV, backgroundColor[0], backgroundColor[1], backgroundColor[2]);
	FillRect(&dc, IDC_FORCECOLORPREV, forceColor[0], forceColor[1], forceColor[2]);
	FillRect(&dc, IDC_FL_FILLPREV, floorFillColor[0], floorFillColor[1], floorFillColor[2]);
	FillRect(&dc, IDC_FL_OUTLINEPREV, floorOutlineColor[0], floorOutlineColor[1], floorOutlineColor[2]);
	FillRect(&dc, IDC_LINKCOLORPREV, linkColor[0], linkColor[1], linkColor[2]);
	// TODO these are going to be removed completely
	//FillRect(&dc, IDC_LEFTJOINTSCOLORPREV, leftJointColor[0], leftJointColor[1], leftJointColor[2]);
	//FillRect(&dc, IDC_RIGHTJOINTSCOLORPREV, rightJointColor[0], rightJointColor[1], rightJointColor[2]);
	FillRect(&dc, IDC_JOINTSCOLORPREV, jointColor[0], jointColor[1], jointColor[2]);
	FillRect(&dc, IDC_LEFTLINKCOLORPREV, leftLinkColor[0], leftLinkColor[1], leftLinkColor[2]);
	FillRect(&dc, IDC_RIGHTLINKCOLORPREV, rightLinkColor[0], rightLinkColor[1], rightLinkColor[2]);
	FillRect(&dc, IDC_FORCECOLORPREV2, rightfeetforceColor[0], rightfeetforceColor[1], rightfeetforceColor[2]);
	FillRect(&dc, IDC_FORCECOLORPREV3, leftfeetforceColor[0], leftfeetforceColor[1], leftfeetforceColor[2]);
}

void Dg_OrthoColors::FillRect(CPaintDC* dc, const int nIDDlgItem, BYTE& red, BYTE& green, BYTE& blue)
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

void Dg_OrthoColors::OnBnClickedApply()
{
	if (this->UpdateData( TRUE ) )
		this->UpdateDocument();
}

void Dg_OrthoColors::OnBnClickedOk()
{
	OnBnClickedApply();	// Update document
	OnOK();
}

void Dg_OrthoColors::OnBnClickedLinkcolor()
{
	 // C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
     COLORREF Ini_Color = RGB(linkColor[0],linkColor[1],linkColor[2] );
     CColorDialog Diag(Ini_Color);
     if(Diag.DoModal() == IDOK)
     {
        COLORREF New_Color = Diag.GetColor();
        if(New_Color != Ini_Color)
        {
          linkColor[0] = GetRValue(New_Color);
          linkColor[1] = GetGValue(New_Color);
          linkColor[2] = GetBValue(New_Color);
        }
	 }
     Invalidate(FALSE); 
}

void Dg_OrthoColors::OnBnClickedLeftjointscolor()
{
	
	 COLORREF Ini_Color = RGB(leftJointColor[0],leftJointColor[1],leftJointColor[2] );
     CColorDialog Diag(Ini_Color);
     if(Diag.DoModal() == IDOK)
     {
        COLORREF New_Color = Diag.GetColor();
        if(New_Color != Ini_Color)
        {
          leftJointColor[0] = GetRValue(New_Color);
          leftJointColor[1] = GetGValue(New_Color);
          leftJointColor[2] = GetBValue(New_Color);
        }
	 }
     Invalidate(FALSE); 
}

void Dg_OrthoColors::OnBnClickedRightjointscolor()
{ 
	 COLORREF Ini_Color = RGB(rightJointColor[0],rightJointColor[1],rightJointColor[2] );
     CColorDialog Diag(Ini_Color);
     if(Diag.DoModal() == IDOK)
     {
        COLORREF New_Color = Diag.GetColor();
        if(New_Color != Ini_Color)
        {
          
          rightJointColor[0] = GetRValue(New_Color);
          rightJointColor[1] = GetGValue(New_Color);
          rightJointColor[2] = GetBValue(New_Color);
        }
	 }
     Invalidate(FALSE); 
	
}

void Dg_OrthoColors::OnBnClickedJointscolor() {
	COLORREF Ini_Color = RGB(jointColor[0],jointColor[1],jointColor[2]);
	CColorDialog Diag(Ini_Color);
	if(Diag.DoModal() == IDOK) {
		COLORREF New_Color = Diag.GetColor();
		if(New_Color != Ini_Color) {
			jointColor[0] = GetRValue(New_Color);
			jointColor[1] = GetGValue(New_Color);
			jointColor[2] = GetBValue(New_Color);
		}
	}
	Invalidate(FALSE);
}

void Dg_OrthoColors::OnBnClickedLeftlinkcolor() {
	COLORREF Ini_Color = RGB(leftLinkColor[0],leftLinkColor[1],leftLinkColor[2]);
	CColorDialog Diag(Ini_Color);
	if(Diag.DoModal() == IDOK) {
		COLORREF New_Color = Diag.GetColor();
		if(New_Color != Ini_Color) {
			leftLinkColor[0] = GetRValue(New_Color);
			leftLinkColor[1] = GetGValue(New_Color);
			leftLinkColor[2] = GetBValue(New_Color);
		}
	}
	Invalidate(FALSE);
}

void Dg_OrthoColors::OnBnClickedRightlinkcolor() {
	COLORREF Ini_Color = RGB(rightLinkColor[0],rightLinkColor[1],rightLinkColor[2]);
	CColorDialog Diag(Ini_Color);
	if(Diag.DoModal() == IDOK) {
		COLORREF New_Color = Diag.GetColor();
		if(New_Color != Ini_Color) {
			rightLinkColor[0] = GetRValue(New_Color);
			rightLinkColor[1] = GetGValue(New_Color);
			rightLinkColor[2] = GetBValue(New_Color);
		}
	}
	Invalidate(FALSE);
}



