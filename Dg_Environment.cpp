#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include "Dg_Environment.h"
#include ".\dg_environment.h"
#include "GLObliqueView.h"

#undef	BORDER_WIDTH
#define	BORDER_WIDTH 2

#ifdef _DEBUG
    #undef THIS_FILE
    static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

Dg_Environment::Dg_Environment(C_Hom_Doc *aDocPtr,CWnd* pParent) : CDialog(Dg_Environment::IDD, pParent),mDocPtr(aDocPtr)
{
    ASSERT( mDocPtr != NULL );
    
    //{{AFX_DATA_INIT(Dg_Environment)
    Floor_Visible = FALSE;
    Handheld_Type = -1;
    Handheld_Height = 0;
    Handheld_Width = 0;
    Bar_Thickness = 0;
    Bar_Azimuth = 0;
    Bar_Pitch = 90;
    Bar_Distance = 0;
    Bar_Elevation = 20;
    Bar_Length = 0;
    Bar_Width = 0;
    Bar_Type = -1;
    Bar_Distance_Units = "";
    Bar_Elevation_Units = "";
    Bar_Length_Units = "";
    Bar_Thickness_Units = "";
    Bar_Width_Units = "";
    Handheld_Height_Units = "";
    Handheld_Width_Units = "";
    //}}AFX_DATA_INIT

    outline = ((CGLObliqueView*)(mDocPtr->GLViews[VIEW_OBLIQUE]))->getHandHeld().outline;
    red = GLOblique::handHeldColor[0];
    green = GLOblique::handHeldColor[1];
    blue = GLOblique::handHeldColor[2];
    outlineRed = GLOblique::handHeldOutlineColor[0];
    outlineGreen = GLOblique::handHeldOutlineColor[1];
    outlineBlue = GLOblique::handHeldOutlineColor[2];
}

void Dg_Environment::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(Dg_Environment)
    DDX_Check(pDX, IDC_FLOOR_VISIBLE, Floor_Visible);
    DDX_Check(pDX, IDC_ENVOUTLINE, outline);
    DDX_Radio(pDX, IDC_HHO_NONE, Handheld_Type);
    DDX_Text(pDX, IDC_HHO_HEIGHT, Handheld_Height);
    DDV_MinMaxInt(pDX, Handheld_Height, Min_Handheld_Height, Max_Handheld_Height);
    DDX_Text(pDX, IDC_HHO_WIDTH, Handheld_Width);
    DDV_MinMaxInt(pDX, Handheld_Width, Min_Handheld_Width, Max_Handheld_Width);
    DDX_Text(pDX, IDC_BARRIER_THICKNESS, Bar_Thickness);
    DDV_MinMaxInt(pDX, Bar_Thickness, Min_Bar_Thickness, Max_Bar_Thickness);
    DDX_Text(pDX, IDC_BARRIER_AZIMUTH, Bar_Azimuth);
    DDV_MinMaxInt(pDX, Bar_Azimuth, -180, 180);
    DDX_Text(pDX, IDC_BARRIER_PITCH, Bar_Pitch);
    DDV_MinMaxInt(pDX, Bar_Pitch, 0, 180);
    DDX_Text(pDX, IDC_BARRIER_DISTANCE, Bar_Distance);
    //DDV_MinMaxInt(pDX, Bar_Distance, Min_Bar_Distance, Max_Bar_Distance);
    DDX_Text(pDX, IDC_BARRIER_ELEVATION, Bar_Elevation);
    //DDV_MinMaxInt(pDX, Bar_Elevation, Min_Bar_Elevation, Max_Bar_Elevation);
    DDX_Text(pDX, IDC_BARRIER_LENGTH, Bar_Length);
    DDV_MinMaxInt(pDX, Bar_Length, Min_Bar_Length, Max_Bar_Length);
    DDX_Text(pDX, IDC_BARRIER_WIDTH, Bar_Width);
    DDV_MinMaxInt(pDX, Bar_Width, Min_Bar_Width, Max_Bar_Width);
    DDX_Radio(pDX, IDC_BARRIER_TYPE, Bar_Type);
    DDX_Text(pDX, IDC_BD_UNITS, Bar_Distance_Units);
    DDX_Text(pDX, IDC_BE_UNITS, Bar_Elevation_Units);
    DDX_Text(pDX, IDC_BL_UNITS, Bar_Length_Units);
    DDX_Text(pDX, IDC_BT_UNITS, Bar_Thickness_Units);
    DDX_Text(pDX, IDC_BW_UNITS, Bar_Width_Units);
    DDX_Text(pDX, IDC_HOH_UNITS, Handheld_Height_Units);
    DDX_Text(pDX, IDC_HOW_UNITS, Handheld_Width_Units);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Dg_Environment, CDialog)
    //{{AFX_MSG_MAP(Dg_Environment)
    ON_BN_CLICKED(IDC_E_REDRAW, OnApply)
    ON_BN_CLICKED(IDC_OBLCOLORBUTTON, OnBnClickedOrtcolorbutton)
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_OUTLINECOLOR, OnBnClickedOutlinecolor)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void Dg_Environment::OnApply() 
{
    if(this->CWnd::UpdateData(TRUE /*Retrieve*/)) //Data transfer successful 
    {
        // Everything should be updated in this function so that the dirty flag
        // gets set correctly
        mDocPtr->ObliqueViewEnvironmentUpdate(*this);
    }
}

void Dg_Environment:: OnBnClickedOrtcolorbutton()
{
	COLORREF Ini_Color = RGB(red,green,blue);
    CColorDialog Diag(Ini_Color);
    if(Diag.DoModal() == IDOK)
    {
        COLORREF New_Color = Diag.GetColor();
        if(New_Color != Ini_Color)
        {
            red = GetRValue(New_Color);
            // GLOblique::shirtGreen = GetGValue(New_Color);
			green = GetGValue(New_Color);
            blue = GetBValue(New_Color);
        }
    }
    Invalidate(FALSE); 
}

void Dg_Environment:: OnBnClickedOutlinecolor()
{
	COLORREF Ini_Color = RGB(outlineRed,outlineGreen,outlineBlue);
    CColorDialog Diag(Ini_Color);
    if(Diag.DoModal() == IDOK)
    {
        COLORREF New_Color = Diag.GetColor();
        if(New_Color != Ini_Color)
        {
            outlineRed = GetRValue(New_Color);
            // GLOblique::shirtGreen = GetGValue(New_Color);
			outlineGreen = GetGValue(New_Color);
            outlineBlue = GetBValue(New_Color);
        }
    }
    Invalidate(FALSE); 
}


/* void Dg_Environment::OnBnClickedOrtcolorbutton()
{
    mDocPtr->GenericColorDialog(red, green, blue);
    InvalidateRect(NULL,0);
}

void Dg_Environment::OnBnClickedOutlinecolor()
{
    mDocPtr->GenericColorDialog(outlineRed, outlineGreen, outlineBlue);
    InvalidateRect(NULL,0);
} */


void Dg_Environment::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
    CWnd *ColorFrame = GetDlgItem(IDC_ENVCOLORBOX);
    CWnd *OutlineFrame = GetDlgItem(IDC_ENVOUTLINECOLOR);
    CRect ColorRect;
    CRect OutlineRect;

    ColorFrame->GetWindowRect(ColorRect);
    OutlineFrame->GetWindowRect(OutlineRect);

    CRect ColorDrawRect(ColorRect.left + BORDER_WIDTH,
                        ColorRect.top + BORDER_WIDTH,
                        ColorRect.right - BORDER_WIDTH,
                        ColorRect.bottom - BORDER_WIDTH);

    CRect OutlineDrawRect(OutlineRect.left + BORDER_WIDTH,
                          OutlineRect.top + BORDER_WIDTH,
                          OutlineRect.right - BORDER_WIDTH,
                          OutlineRect.bottom - BORDER_WIDTH);

    ScreenToClient(ColorDrawRect);
    ScreenToClient(OutlineDrawRect);
    CBrush colorBrush(RGB(red, green, blue));
    CBrush outlineBrush(RGB(outlineRed, outlineGreen, outlineBlue));
    dc.FillRect(ColorDrawRect, &colorBrush);
    dc.FillRect(OutlineDrawRect, &outlineBrush);
}
