// LimitScaleCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "LimitScaleCtrl.h"
#include ".\limitscalectrl.h"
#include "memdc.h"

// CLimitScaleCtrl

IMPLEMENT_DYNAMIC(CLimitScaleCtrl, CSliderCtrl)
CLimitScaleCtrl::CLimitScaleCtrl()
{
    mUpperLimitClr  = COLORREF(RGB(200,0,0));    //Red
    mDesignLimitClr = COLORREF(RGB(255,255,0));  //Yellow
    mAcceptableClr  = COLORREF(RGB(0,160,0));    //Green
    maximum = 100;                               //Default maximum
    upperLimitText  = CString("");
    designLimitText = CString("");
}

CLimitScaleCtrl::~CLimitScaleCtrl()
{
}


BEGIN_MESSAGE_MAP(CLimitScaleCtrl, CSliderCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CLimitScaleCtrl message handlers
void CLimitScaleCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    
    //Request item-specific notifications
	if (pNMCD->dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;        
		return;
	}

    //We want to change the channel color before the slider is painted
    if (pNMCD->dwDrawStage == CDDS_ITEMPREPAINT)
    {
        if (pNMCD->dwItemSpec == TBCD_CHANNEL) //The item being drawn is the channel
        {
            DrawChannel(pNMCD);

		    *pResult = CDRF_SKIPDEFAULT;
		    return;
        }
        else if (pNMCD->dwItemSpec ==  TBCD_THUMB)   //Drawing the thumb
		{
            CRect thumbRect(pNMCD->rc); //The thumb's rectangle

            if (thumbRect.Height() > thumbRect.Width())   //Horizontal slider thumb
            {
			    DrawThumb(pNMCD);
                *pResult = CDRF_SKIPDEFAULT;
		        return;
            }
        }
    }

    *pResult = CDRF_NOTIFYITEMDRAW;
}


//Private drawing functions
void CLimitScaleCtrl::DrawChannel(LPNMCUSTOMDRAW pNMCD)
{
    //Get a device context to paint into and save the old device context
    CDC *pDC = CDC::FromHandle(pNMCD->hdc);
	int nSavedDC = pDC->SaveDC();

    //Double buffered context
    CMemDCX dc(pDC);

    //Color the background of the control to be the same as the dialog color
    //This erases the entire control, so the insets will need to be drawn later
    CRect client_rect, window_rect;
    GetClientRect(client_rect);
    CBrush bg(GetSysColor(COLOR_3DFACE));
    dc.FillRect(client_rect, &bg);

    //Calculate widths of each color range
    CRect channel;
    GetChannelRect(&channel);
    channel.DeflateRect(2, 2);
    int totalWidth = channel.Width();
    int upperLimitStart  = channel.left + (int)((upperLimit.start * totalWidth / maximum));
    int upperLimitEnd    = channel.left + (int)((upperLimit.end * totalWidth / maximum));
    int designLimitStart = channel.left + (int)((designLimit.start * totalWidth / maximum));
    int designLimitEnd   = channel.left + (int)((designLimit.end * totalWidth / maximum));
    int acceptableStart  = channel.left + (int)((acceptable.start * totalWidth / maximum));
    int acceptableEnd    = channel.left + (int)((acceptable.end * totalWidth / maximum));
    int height           = channel.Height();

    //Paint the channel of the slider
    //Draw the colors
    dc.FillSolidRect(upperLimitStart, channel.top, upperLimitEnd - upperLimitStart, height, mUpperLimitClr);
    dc.FillSolidRect(designLimitStart, channel.top, designLimitEnd - designLimitStart, height, mDesignLimitClr);
    dc.FillSolidRect(acceptableStart, channel.top, acceptableEnd - acceptableStart, height, mAcceptableClr);

    //Add text to describe what each color is
    CString upperTextToDraw, designTextToDraw;
    dc.SetTextColor(RGB(0,0,0));
    CFont textFont;
    textFont.CreateFont(8, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, 
                        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
    CRect textRect(designLimitStart, channel.top, designLimitEnd, channel.bottom);
    CFont* oldFont = dc.SelectObject(&textFont);
    dc.SetBkColor(mDesignLimitClr);
    //Determine which direction the arrows should point for the text descriptions of the limits
    if (upperLimitStart < acceptableStart)
    {
        if (!upperLimitText.IsEmpty())
            upperTextToDraw = "<- " + upperLimitText;
        if (!designLimitText.IsEmpty())
            designTextToDraw = designLimitText + " ->";
        dc.DrawText(upperTextToDraw, &textRect, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
        dc.DrawText(designTextToDraw, &textRect, DT_VCENTER | DT_RIGHT | DT_SINGLELINE);
    }
    else
    {
        if (!upperLimitText.IsEmpty())
            upperTextToDraw = upperLimitText + " ->";
        if (!designLimitText.IsEmpty())
            designTextToDraw = "<- " + designLimitText;
        dc.DrawText(upperTextToDraw, &textRect, DT_VCENTER | DT_RIGHT | DT_SINGLELINE);
        dc.DrawText(designTextToDraw, &textRect, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
    }

    //Draw the inset edges
    channel.InflateRect(1,1);
    dc.Draw3dRect(channel, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DHILIGHT));

    //Restore the old device context and tell the control to skip default painting
    //for this item and free memory
    dc.SelectObject(oldFont);
    pDC->RestoreDC(nSavedDC);
    DeleteObject(textFont);
    DeleteObject(bg);
}

void CLimitScaleCtrl::DrawThumb(LPNMCUSTOMDRAW pNMCD)
{
    CRect thumbRect(pNMCD->rc); //The thumb's rectangle

    //Create a device context
    CDC *pDC = CDC::FromHandle(pNMCD->hdc);
	int nSavedDC = pDC->SaveDC();

    //Find the rectangle for the thumb
    int position = GetPos();
    int min, max;
    GetRange(min, max);
	thumbRect.left+=2;
	thumbRect.right-=2;
    if (position > ((min + max) / 2))
        thumbRect.left = thumbRect.right - 3;
    else
        thumbRect.right = thumbRect.left + 3;

    //Get the rectangle for the indicator that goes over the channel
    CRect channelRect, thumbLineRect;
    GetChannelRect(channelRect);
    channelRect.DeflateRect(1,1);

    thumbLineRect = CRect(thumbRect.left, channelRect.top, thumbRect.right, channelRect.bottom);
    pDC->FillSolidRect(thumbLineRect, RGB(0,0,0));

    //Draw the two arrows
    channelRect.InflateRect(1,1);
    CPen arrowPen(PS_SOLID, 1, RGB(0,0,0));
    CPen * pOldPen = (CPen*)pDC->SelectObject(&arrowPen);
    for (int i = 0; i < 2; i++)
    {
        //Top arrow
        pDC->MoveTo(thumbRect.left - i, channelRect.top - i);
        pDC->LineTo(thumbRect.right + i, channelRect.top - i);

        //Bottom arrow
        pDC->MoveTo(thumbRect.left - i, channelRect.bottom + i - 1);
        pDC->LineTo(thumbRect.right + i, channelRect.bottom + i - 1);
    }

    //Restore the old pens and device context
    pDC->SelectObject(pOldPen);
    pDC->RestoreDC(nSavedDC);
    DeleteObject(arrowPen);
}

void CLimitScaleCtrl::Refresh()
{
    int min,max;
	GetRange(min,max);
	CSliderCtrl::SetRange(min,max,TRUE);
}


//Mutator Functions

//Set the maximum range. This number is used to determine what percentage of the entire
//progress bar is taken up by the upper, design, and acceptable limit ranges.
void CLimitScaleCtrl::SetMaximum(int max)
{
    //Set the maximum and re-draw the window
    maximum = max;
    Refresh();
}

//Set the range for the upper limit. These numbers must be between 0 and the maximum
//defined by SetMaximum()
void CLimitScaleCtrl::SetULRange(int lower, int upper)
{
    //Set the new limits and redraw the window
    upperLimit.start = (lower < 0) ? 0:lower;
    upperLimit.end   = (upper > maximum) ? maximum:upper;
    Refresh();
}

//Set the range for the design limit. These numbers must be between 0 and the maximum
//defined by SetMaximum()
void CLimitScaleCtrl::SetDLRange(int lower, int upper)
{
    //Set the new limits and redraw the window
    designLimit.start = (lower < 0) ? 0:lower;
    designLimit.end   = (upper > maximum) ? maximum:upper;
    Refresh();
}

//Set the range for the acceptable limits. These numbers must be between 0 and the maximum
//defined by SetMaximum()
void CLimitScaleCtrl::SetOKRange(int lower, int upper)
{
    //Set the new limits and redraw the window
    acceptable.start = (lower < 0) ? 0:lower;
    acceptable.end   = (upper > maximum) ? maximum:upper;
    Refresh();
}


//Set all 3 ranges at once. These numbers must be between 0 and the maximum defined by SetMaximum().
void CLimitScaleCtrl::SetAllRanges(int ULLower, int ULUpper, int DLLower, int DLUpper, int OKLower, int OKUpper)
{
    upperLimit.start = (ULLower < 0) ? 0:ULLower;
    upperLimit.end = (ULUpper > maximum) ? maximum:ULUpper;
    designLimit.start = (DLLower < 0) ? 0:DLLower;
    designLimit.end = (DLUpper > maximum) ? maximum:DLUpper;
    acceptable.start = (OKLower < 0) ? 0:OKLower;
    acceptable.end   = (OKUpper > maximum) ? maximum:OKUpper;
    Refresh();
}

//Set the text to be shown at the border of the limits.
void CLimitScaleCtrl::SetLimitsText(CString upper, CString design)
{
    //Set the text to be drawn in each limit region and redraw the window
    upperLimitText = upper;
    designLimitText = design;
    Refresh();
}