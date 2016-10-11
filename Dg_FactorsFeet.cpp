// Dg_FactorsFeet.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "Dg_FactorsFeet.h"



// Dg_FactorsFeet dialog

IMPLEMENT_DYNAMIC(Dg_FactorsFeet, CDialog)
Dg_FactorsFeet::Dg_FactorsFeet(CWnd* pParent /*=NULL*/)
: CDialog(Dg_FactorsFeet::IDD, pParent)
{
}

Dg_FactorsFeet::~Dg_FactorsFeet()
{
}

void Dg_FactorsFeet::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   float temp;
   DDX_Text(pDX, IDC_MassFoot, mFactors.mMass_Foot);
   DDX_Text(pDX, IDC_CMFoot, mFactors.mCM_Foot);
   DDX_Text(pDX, IDC_LinkHeelToToe, mFactors.mLink_HeelToToe);
   DDX_Text(pDX, IDC_LinkFloorToAnkle, mFactors.mLink_FloorToAnkle);
   DDX_Text(pDX, IDC_LinkFootWidth, mFactors.mLink_FootBreadth);
   DDX_Text(pDX, IDC_LinkHeelBreadth, mFactors.mLink_HeelBreadth);
   // This is done because the rounded factor should not be stored
   // and rounding in the paramater does not work
   temp = Round_Float(mFactors.mLink_HeelLength, 4);
   DDX_Text(pDX, IDC_LinkHeelLength, temp);
   mFactors.mLink_HeelLength = temp;
   temp = Round_Float(mFactors.mLink_GreaterToeLength, 3);
   DDX_Text(pDX, IDC_LinkGreaterToeLength, temp);
   mFactors.mLink_GreaterToeLength = temp;
   temp = Round_Float(mFactors.mLink_LateralGreaterToe, 4);
   DDX_Text(pDX, IDC_LinkLateralGreaterToe, temp);
   mFactors.mLink_LateralGreaterToe = temp;
   temp =  Round_Float(mFactors.mLink_MetatarsalLength, 4);
   DDX_Text(pDX, IDC_LinkMetatarsalLength, temp);
   mFactors.mLink_MetatarsalLength = temp;
   DDX_Text(pDX, IDC_LinkFifthMetatarsal, mFactors.mLink_FifthMetatarsalLateral);
}


BEGIN_MESSAGE_MAP(Dg_FactorsFeet, CDialog)
	ON_COMMAND(ID_PRINT_PRINTWINDOW, &Dg_FactorsFeet::OnPrintPrintwindow)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// Dg_FactorsFeet message handlers


void Dg_FactorsFeet::OnPrintPrintwindow()
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

BOOL Dg_FactorsFeet::OnInitDialog()
{
	CDialog::OnInitDialog();
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->AppendMenu(MF_SEPARATOR);// add a separator
	pSysMenu->AppendMenu(MF_STRING,IDR_MENU1,"Print Window");// add print menu
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void Dg_FactorsFeet::OnSysCommand(UINT nID, LPARAM lParam)
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
