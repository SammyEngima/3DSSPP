// Dg_FactorsHands.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "Dg_FactorsHands.h"


// Dg_FactorsHands dialog

IMPLEMENT_DYNAMIC(Dg_FactorsHands, CDialog)
Dg_FactorsHands::Dg_FactorsHands(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_FactorsHands::IDD, pParent)
{
}

Dg_FactorsHands::~Dg_FactorsHands()
{
}

void Dg_FactorsHands::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MassHand, mFactors.mMass_Hand);
	DDX_Text(pDX, IDC_CMHAND, mFactors.mCM_Hand);
	DDX_Text(pDX, IDC_LinkWristToGripCenter, mFactors.mLink_Hand);
	DDX_Text(pDX, IDC_LinkWristToFingerTip, mFactors.mLink_HandWithFinger);
    DDX_Text(pDX, IDC_LinkL1CA, mFactors.mLink_L1CA);
    DDX_Text(pDX, IDC_LinkL1MC, mFactors.mLink_L1MC);
    DDX_Text(pDX, IDC_LinkL1PP, mFactors.mLink_L1PP);
    DDX_Text(pDX, IDC_LinkL1DP, mFactors.mLink_L1DP);
    DDX_Text(pDX, IDC_LinkL2CM, mFactors.mLink_L2CM);
    DDX_Text(pDX, IDC_LinkL2PP, mFactors.mLink_L2PP);
    DDX_Text(pDX, IDC_LinkL2MP, mFactors.mLink_L2MP);
    DDX_Text(pDX, IDC_LinkL2DP, mFactors.mLink_L2DP);
    DDX_Text(pDX, IDC_LinkL3CM, mFactors.mLink_L3CM);
    DDX_Text(pDX, IDC_LinkL3PP, mFactors.mLink_L3PP);
    DDX_Text(pDX, IDC_LinkL3MP, mFactors.mLink_L3MP);
    DDX_Text(pDX, IDC_LinkL3DP, mFactors.mLink_L3DP);
    DDX_Text(pDX, IDC_LinkL4CM, mFactors.mLink_L4CM);
    DDX_Text(pDX, IDC_LinkL4PP, mFactors.mLink_L4PP);
    DDX_Text(pDX, IDC_LinkL4MP, mFactors.mLink_L4MP);
    DDX_Text(pDX, IDC_LinkL4DP, mFactors.mLink_L4DP);
    DDX_Text(pDX, IDC_LinkL5CM, mFactors.mLink_L5CM);
    DDX_Text(pDX, IDC_LinkL5PP, mFactors.mLink_L5PP);
    DDX_Text(pDX, IDC_LinkL5MP, mFactors.mLink_L5MP);
    DDX_Text(pDX, IDC_LinkL5DP, mFactors.mLink_L5DP);
    DDX_Text(pDX, IDC_LinkR1CA, mFactors.mLink_R1CA);
    DDX_Text(pDX, IDC_LinkR1MC, mFactors.mLink_R1MC);
    DDX_Text(pDX, IDC_LinkR1PP, mFactors.mLink_R1PP);
    DDX_Text(pDX, IDC_LinkR1DP, mFactors.mLink_R1DP);
    DDX_Text(pDX, IDC_LinkR2CM, mFactors.mLink_R2CM);
    DDX_Text(pDX, IDC_LinkR2PP, mFactors.mLink_R2PP);
    DDX_Text(pDX, IDC_LinkR2MP, mFactors.mLink_R2MP);
    DDX_Text(pDX, IDC_LinkR2DP, mFactors.mLink_R2DP);
    DDX_Text(pDX, IDC_LinkR3CM, mFactors.mLink_R3CM);
    DDX_Text(pDX, IDC_LinkR3PP, mFactors.mLink_R3PP);
    DDX_Text(pDX, IDC_LinkR3MP, mFactors.mLink_R3MP);
    DDX_Text(pDX, IDC_LinkR3DP, mFactors.mLink_R3DP);
    DDX_Text(pDX, IDC_LinkR4CM, mFactors.mLink_R4CM);
    DDX_Text(pDX, IDC_LinkR4PP, mFactors.mLink_R4PP);
    DDX_Text(pDX, IDC_LinkR4MP, mFactors.mLink_R4MP);
    DDX_Text(pDX, IDC_LinkR4DP, mFactors.mLink_R4DP);
    DDX_Text(pDX, IDC_LinkR4CM, mFactors.mLink_R4CM);
    DDX_Text(pDX, IDC_LinkR5CM, mFactors.mLink_R5CM);
    DDX_Text(pDX, IDC_LinkR5PP, mFactors.mLink_R5PP);
    DDX_Text(pDX, IDC_LinkR5MP, mFactors.mLink_R5MP);
    DDX_Text(pDX, IDC_LinkR5DP, mFactors.mLink_R5DP);
}


BEGIN_MESSAGE_MAP(Dg_FactorsHands, CDialog)
 
	ON_COMMAND(ID_PRINT_PRINTWINDOW, &Dg_FactorsHands::OnPrintPrintwindow)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()






void Dg_FactorsHands::OnPrintPrintwindow()
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


BOOL Dg_FactorsHands::OnInitDialog()
{
	CDialog::OnInitDialog();
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->AppendMenu(MF_SEPARATOR);// add a separator
	pSysMenu->AppendMenu(MF_STRING,IDR_MENU1,"Print Window");// add print menu
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void Dg_FactorsHands::OnSysCommand(UINT nID, LPARAM lParam)
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
