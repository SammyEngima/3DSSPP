// Dg_Gravity.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include "Dg_Gravity.h"
#include "Dimension.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_Gravity dialog


Dg_Gravity::Dg_Gravity(CWnd* pParent /*=NULL*/)
: CDialog(Dg_Gravity::IDD, pParent)
{
	//{{AFX_DATA_INIT(Dg_Gravity)
	m_GravValue = 0.0;
	m_GravList = _T("");
	m_accel_units = _T("");
	m_ComboBox = -1;
	//}}AFX_DATA_INIT
}


void Dg_Gravity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_Gravity)
	DDX_Text(pDX, IDC_EDIT1, m_GravValue);
	DDX_Text(pDX, IDC_ACCEL_UNITS, m_accel_units);
	DDX_CBIndex(pDX, IDC_COMBO1, m_ComboBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Dg_Gravity, CDialog)
//{{AFX_MSG_MAP(Dg_Gravity)
ON_CBN_EDITCHANGE(IDC_COMBO1, OnEditchangeCombo1)
ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
ON_BN_CLICKED(IDC_APPLY, OnApply)
ON_CBN_SELCHANGE(IDC_COMBO1, OnEditchangeCombo1)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_PRINT_PRINTWINDOW, &Dg_Gravity::OnPrintPrintwindow)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_Gravity message handlers


void Dg_Gravity::OnEditchangeCombo1() 
{
	if (this->UpdateData(TRUE))
	{
		double metricGrav;
		C_Hom_App*  App_Ptr = Get_App_Ptr();
		C_Hom_Doc*  Doc_Ptr = App_Ptr->Get_Document_Ptr();
		
		switch (m_ComboBox) {
		case 0:
			metricGrav = 9.81f; break;
		case 1:
			metricGrav = 0.00f; break;
		case 2:
			metricGrav = 0.01f; break;
		case 3:
			metricGrav = 1.6f; break;
		case 4:
			metricGrav = 3.73f; break;
		default:
			metricGrav = 9.81f; break;
		}
		if( (E_Units)Doc_Ptr->getDescriptionRef().Units() == U_ENGLISH ) 
			m_GravValue = metricGrav / ft_TO_in / in_TO_cm / cm_TO_m;
		else
			m_GravValue = metricGrav;
		
		double fractional_remainder;
		m_GravValue *= 1000.0;
		fractional_remainder = fmod(m_GravValue,1);
		m_GravValue -= fractional_remainder;
		if (fractional_remainder >= 0.5)
			m_GravValue ++;
		m_GravValue /= 1000.0;
	}
	UpdateData(FALSE);
	
}
BOOL Dg_Gravity::UpdateCombobox(BOOL bSaveAndValidate)
{
	ASSERT(::IsWindow(m_hWnd)); // calling UpdateData before DoModal?
	
	CDataExchange dx(this, bSaveAndValidate);
	
	// prevent control notifications from being dispatched during UpdateData
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	HWND hWndOldLockout = pThreadState->m_hLockoutNotifyWindow;
	ASSERT(hWndOldLockout != m_hWnd);   // must not recurse
	pThreadState->m_hLockoutNotifyWindow = m_hWnd;
	
	BOOL bOK = FALSE;       // assume failure
	TRY
	{
		DDX_CBIndex(&dx, IDC_COMBO1, m_ComboBox);
		bOK = TRUE;         // it worked
	}
	CATCH(CUserException, e)
	{
		// validation failed - user already alerted, fall through
		ASSERT(!bOK);
		// Note: DELETE_EXCEPTION_(e) not required
	}
	AND_CATCH_ALL(e)
	{
		// validation failed due to OOM or other resource failure
		e->ReportError(MB_ICONEXCLAMATION, AFX_IDP_INTERNAL_FAILURE);
		ASSERT(!bOK);
	}
	END_CATCH_ALL
		
		pThreadState->m_hLockoutNotifyWindow = hWndOldLockout;
	return bOK;
}
void Dg_Gravity::OnChangeEdit1() 
{
	// Todo : Make the exception NOT pop up on errors, unless they try hitting OK with a bad value.
	
	if (this->UpdateData(TRUE))
	{
		double metricGrav;
		C_Hom_App*  App_Ptr = Get_App_Ptr();
		C_Hom_Doc*  Doc_Ptr = App_Ptr->Get_Document_Ptr();
		if( (E_Units)Doc_Ptr->getDescriptionRef().Units() == U_ENGLISH ) 
			metricGrav = m_GravValue * ft_TO_in * in_TO_cm * cm_TO_m;
		else
			metricGrav = m_GravValue;
		
		if (metricGrav >= 9.80 && metricGrav < 9.82) { m_ComboBox = 0; }
		else if (metricGrav >= 3.72 && metricGrav <= 3.74) { m_ComboBox = 4; }
		else if (metricGrav >= 1.55 && metricGrav <= 1.57) { m_ComboBox = 3; }
		else if (metricGrav == 0 ) { m_ComboBox = 1; }
		else if (metricGrav < 0.1 ) { m_ComboBox = 2; }
		else { m_ComboBox = -1; }
		UpdateCombobox(FALSE);
		//UpdateData(FALSE);	
	}
	else
		UpdateData(FALSE);
	
}

// Sets the gravity of the entire program to value specified in dialog
// This is called by the "Apply" button, and the "OK" button
void Dg_Gravity::OnApply() 
{
   C_Hom_App*  App_Ptr = Get_App_Ptr();
   C_Hom_Doc*  Doc_Ptr = App_Ptr->Get_Document_Ptr();
   bool changed = false;
   if( (E_Units)Doc_Ptr->getDescriptionRef().Units() == U_ENGLISH ) 
   {
      if (Doc_Ptr->getGravity() != m_GravValue * ft_TO_in * in_TO_cm * cm_TO_m)
      {
         changed = true;
      }
      Doc_Ptr->setGravity(m_GravValue * ft_TO_in * in_TO_cm * cm_TO_m);      
   }
   else
   {
            if(Doc_Ptr->getGravity() != m_GravValue)
            {
               changed = true;
            }
	    Doc_Ptr->setGravity(m_GravValue);
   }
   Doc_Ptr->UpdateConversions();	// Make sure to use SLUGS in english non earth grav
   if(changed)
   {
      Doc_Ptr->MakeDirtyAndUpdateViews();
   }
}

void Dg_Gravity::OnOK() 
{
	OnApply();
	CDialog::OnOK();
}


void Dg_Gravity::OnPrintPrintwindow()
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


BOOL Dg_Gravity::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->AppendMenu(MF_SEPARATOR);// add a separator
	pSysMenu->AppendMenu(MF_STRING,IDR_MENU1,"Print Window");// add print menu
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void Dg_Gravity::OnSysCommand(UINT nID, LPARAM lParam)
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
