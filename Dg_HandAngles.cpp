// Dg_HandAngles.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "Dg_HandAngles.h"
#include "hom_doc.h"
#include ".\dg_handangles.h"


// Dg_HandAngles dialog

IMPLEMENT_DYNAMIC(Dg_HandAngles, CDialog)
Dg_HandAngles::Dg_HandAngles(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_HandAngles::IDD, pParent)
	, thumbHandVert(0)
	, thumbHandHorz(0)
	, thumbLowerVert(0)
	, thumbLowerHorz(0)
	, thumbMiddleVert(0)
	, thumbMidHorz(0)
	, thumbUpperVert(0)
	, thumbUpperHorz(0)
{
	pDoc = Get_App_Ptr()->Get_Document_Ptr();
	thumbHandVert = pDoc->rightHand.thumbHand.vert;
	thumbHandHorz = pDoc->rightHand.thumbHand.horz;
	thumbLowerVert = pDoc->rightHand.thumb.low.vert;
	thumbLowerHorz = pDoc->rightHand.thumb.low.horz;
	thumbMiddleVert = pDoc->rightHand.thumb.mid.vert;
	thumbMidHorz = pDoc->rightHand.thumb.up.horz;
	thumbUpperVert = pDoc->rightHand.thumb.up.vert;
	thumbUpperHorz = pDoc->rightHand.thumb.up.horz;
}

Dg_HandAngles::~Dg_HandAngles()
{
}

void Dg_HandAngles::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_THUMBHANDVERT, thumbHandVert);
	DDX_Text(pDX, IDC_THUMBHANDHORZ, thumbHandHorz);
	DDX_Text(pDX, IDC_THUMBLOWVERT, thumbLowerVert);
	DDX_Text(pDX, IDC_THUMBLOWHORZ, thumbLowerHorz);
	DDX_Text(pDX, IDC_THUMBMIDVERT, thumbMiddleVert);
	DDX_Text(pDX, IDC_THUMBMIDHORZ, thumbMidHorz);
	DDX_Text(pDX, IDC_THUMBUPPERVERT, thumbUpperVert);
	DDX_Text(pDX, IDC_THUMBUPPERHORZ, thumbUpperHorz);
}


BEGIN_MESSAGE_MAP(Dg_HandAngles, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
	ON_COMMAND(ID_PRINT_PRINTWINDOW, &Dg_HandAngles::OnPrintPrintwindow)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

void Dg_HandAngles::UpdateDocument()
{ 
	assert(pDoc != NULL);
	UpdateData( TRUE );
	pDoc->rightHand.thumbHand.vert = thumbHandVert;
	pDoc->rightHand.thumb.low.vert = thumbLowerVert;
	pDoc->rightHand.thumb.low.horz = thumbLowerHorz;
	pDoc->rightHand.thumb.mid.vert = thumbMiddleVert;
	pDoc->rightHand.thumb.mid.horz = thumbMidHorz;
	pDoc->rightHand.thumb.up.vert = thumbUpperVert;
	pDoc->rightHand.thumb.up.horz = thumbUpperHorz;
	return;
}

// Dg_HandAngles message handlers

void Dg_HandAngles::OnBnClickedOk()
{
	UpdateDocument();
	pDoc->UpdateAllViews( NULL );
	OnOK();
}


void Dg_HandAngles::OnBnClickedApply()
{
	UpdateDocument();
	pDoc->UpdateAllViews( NULL );
}



void Dg_HandAngles::OnPrintPrintwindow()
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


BOOL Dg_HandAngles::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->AppendMenu(MF_SEPARATOR);// add a separator
	pSysMenu->AppendMenu(MF_STRING,IDR_MENU1,"Print Window");// add print menu
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void Dg_HandAngles::OnSysCommand(UINT nID, LPARAM lParam)
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
