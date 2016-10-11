// Dg_TaskDescription.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "Dg_TaskDescription.h"
#include "ReportView.hpp"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_TaskDescription dialog


Dg_TaskDescription::Dg_TaskDescription(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_TaskDescription::IDD, pParent)
	, Company_Name(_T(""))
	, Frame_Name(_T(""))
{
	//{{AFX_DATA_INIT(Dg_TaskDescription)
	Task_Comments = "";
	Analyst_Name = "";
	Task_Name = "";
	//}}AFX_DATA_INIT
}

void Dg_TaskDescription::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_TaskDescription)
	DDX_Text(pDX, IDC_TASKCOMMENTS, Task_Comments);
	DDX_Text(pDX, IDC_ANYLYST, Analyst_Name);
	DDX_Text(pDX, IDC_TASK, Task_Name);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_COMPANY, Company_Name);
	DDX_Text(pDX, IDC_FRAMENAME, Frame_Name);
}

BEGIN_MESSAGE_MAP(Dg_TaskDescription, CDialog)
	//{{AFX_MSG_MAP(Dg_TaskDescription)
	//}}AFX_MSG_MAP
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_PRINT_PRINTWINDOW, &Dg_TaskDescription::OnPrintPrintwindow)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dg_TaskDescription message handlers




BOOL Dg_TaskDescription::OnInitDialog()
{
	CDialog::OnInitDialog();



	// TODO:  Add extra initialization here
	CMenu* pSysMenu = GetSystemMenu(FALSE);


	pSysMenu->AppendMenu(MF_SEPARATOR);// add a separator
	pSysMenu->AppendMenu(MF_STRING,IDR_MENU1,"Print Window");// add print menu

	//pSysMenu->AppendMenu(MF_STRING,IDR_PRINT_WINDOW,"Print Window");// add print menu
	//pSysMenu->AppendMenu(MF_STRING,IDR_PRINT_PREVIEW,"Print Preview");// add print menu
	//pSysMenu->AppendMenu(MF_STRING,IDR_MENU1,"Print Setup");// add print menu
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void Dg_TaskDescription::OnSysCommand(UINT nID, LPARAM lParam)
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


void Dg_TaskDescription::OnPrintPrintwindow()
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


