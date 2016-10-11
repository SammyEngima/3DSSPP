// Dg_HeadNeck.cpp : implementation file
//

#include "stdafx.h"
#include "Dg_HeadNeck.h"
#include "afxdialogex.h"

///Yijun
#include "hom.h"
#include "hom_doc.h" // Necessary


// Dg_HeadNeck dialog

IMPLEMENT_DYNAMIC(Dg_HeadNeck, CDialog)

Dg_HeadNeck::Dg_HeadNeck(Dg_BodyAnglesDyn* bod,CWnd* pParent /*=NULL*/)
	: CDialog(Dg_HeadNeck::IDD, pParent),body(bod),Current_Control_ID( 0 )
{
	body = bod;
	HeadFlexion = Round_Double(90 - bod->HeadFlexion, 2);
	UpperNeckFlexion = Round_Double(90 - bod->UpperNeckFlexion, 2);
	UpperNeckAxialRotation = Round_Double(bod->UpperNeckAxialRotation, 2);
	UpperNeckLateralBending = Round_Double(bod->UpperNeckLateralBending, 2);
	LowerNeckFlexion = Round_Double(90 - bod->LowerNeckFlexion, 2);
	LowerNeckAxialRotation = Round_Double(bod->LowerNeckAxialRotation, 2);
	LowerNeckLateralBending = Round_Double(bod->LowerNeckLateralBending, 2);
}

Dg_HeadNeck::~Dg_HeadNeck()
{
}

void Dg_HeadNeck::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HEAD_FLEXION_DYN, mHeadFlexionControl);
	//DDX_Control(pDX, IDC_HEAD_ROT_DYN, mHeadAxialRotationControl);
	//DDX_Control(pDX, IDC_HEAD_LATERAL_DYN, mHeadLateralBendingControl);
	DDX_Control(pDX, IDC_NECK1_FLEXION_DYN, mUpperNeckFlexionControl);
	DDX_Control(pDX, IDC_NECK1_ROT_DYN, mUpperNeckAxialRotationControl);
	DDX_Control(pDX, IDC_NECK1_LATERAL_DYN, mUpperNeckLateralBendingControl);
	DDX_Control(pDX, IDC_NECK2_FLEXION_DYN, mLowerNeckFlexionControl);
	DDX_Control(pDX, IDC_NECK2_ROT_DYN, mLowerNeckAxialRotationControl);
	DDX_Control(pDX, IDC_NECK2_LATERAL_DYN, mLowerNeckLateralBendingControl);

	DDX_Text(pDX, IDC_HEAD_FLEXION_DYN, HeadFlexion);
	//DDX_Text(pDX, IDC_HEAD_ROT_DYN, HeadAxialRotation);
	//DDX_Text(pDX, IDC_HEAD_LATERAL_DYN, HeadLateralBending);
	DDX_Text(pDX, IDC_NECK1_FLEXION_DYN, UpperNeckFlexion);
	DDX_Text(pDX, IDC_NECK1_ROT_DYN, UpperNeckAxialRotation);
	DDX_Text(pDX, IDC_NECK1_LATERAL_DYN, UpperNeckLateralBending);
	DDX_Text(pDX, IDC_NECK2_FLEXION_DYN, LowerNeckFlexion);
	DDX_Text(pDX, IDC_NECK2_ROT_DYN, LowerNeckAxialRotation);
	DDX_Text(pDX, IDC_NECK2_LATERAL_DYN, LowerNeckLateralBending);
}


BEGIN_MESSAGE_MAP(Dg_HeadNeck, CDialog)
	ON_BN_CLICKED(IDOK, &Dg_HeadNeck::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &Dg_HeadNeck::OnBnClickedButton1)

	///Yijun
	ON_COMMAND(ID_PRINT_PRINTWINDOW, &Dg_HeadNeck::OnPrintPrintwindow)
	ON_WM_SYSCOMMAND()
	ON_WM_MOUSEWHEEL()

	ON_EN_SETFOCUS(IDC_HEAD_FLEXION_DYN, OnEnSetfocusHeadangleDyn)
	ON_EN_SETFOCUS(IDC_NECK1_FLEXION_DYN, OnEnSetfocusUpperFlexionDyn)
	ON_EN_SETFOCUS(IDC_NECK1_ROT_DYN, OnEnSetfocusUpperRotationDyn)
	ON_EN_SETFOCUS(IDC_NECK1_LATERAL_DYN, OnEnSetfocusUpperBendingDyn)
	ON_EN_SETFOCUS(IDC_NECK2_FLEXION_DYN, OnEnSetfocusLowerFlexionDyn)
	ON_EN_SETFOCUS(IDC_NECK2_ROT_DYN, OnEnSetfocusLowerRotationDyn)
	ON_EN_SETFOCUS(IDC_NECK2_LATERAL_DYN, OnEnSetfocusLowerBendingDyn)
END_MESSAGE_MAP()


// Dg_HeadNeck message handlers

void Dg_HeadNeck::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	body->HeadFlexion = 90 - HeadFlexion;
	body->UpperNeckFlexion = 90 - UpperNeckFlexion;
	body->UpperNeckAxialRotation = UpperNeckAxialRotation;
	body->UpperNeckLateralBending = UpperNeckLateralBending;
	body->LowerNeckFlexion = 90 - LowerNeckFlexion;
	body->LowerNeckAxialRotation = LowerNeckAxialRotation;
	body->LowerNeckLateralBending = LowerNeckLateralBending;
	body->Head_Flexion = body->Trunk_Flexion - (270 - body->HeadFlexion - body->UpperNeckFlexion - body->LowerNeckFlexion);
	body->Head_Axial_Rotation = UpperNeckAxialRotation + LowerNeckAxialRotation;
	body->Head_Lateral_Bending = UpperNeckLateralBending + LowerNeckLateralBending;
	body->Customizedneck = 1;
	body->UpdateAngles();
	// update the difference of the angles between trunk and neck, for maintaining head posture
	body->diff0 = 90 - HeadFlexion - body->Trunk_Flexion; 
	body->diff1 = 90 - UpperNeckFlexion - body->Trunk_Flexion;
    body->diff2 = 90 - LowerNeckFlexion - body->Trunk_Flexion;
	body->diff3 = UpperNeckAxialRotation - body->Trunk_Axial_Rotation;
    body->diff4 = UpperNeckLateralBending - body->Trunk_Lateral_Bending;
	body->diff5 = LowerNeckAxialRotation  - body->Trunk_Axial_Rotation;
    body->diff6 = LowerNeckLateralBending - body->Trunk_Lateral_Bending;
	body->diff7 = body->Head_Flexion - body->Trunk_Flexion;
	body->diff8 = body->Head_Lateral_Bending - body->Trunk_Lateral_Bending;
	body->diff9 = body->Head_Axial_Rotation - body->Trunk_Axial_Rotation;
	CDialog::OnOK();
}


void Dg_HeadNeck::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here	
	UpdateData();
	body->HeadFlexion = 90 - HeadFlexion;
	body->UpperNeckFlexion = 90 - UpperNeckFlexion;
	body->UpperNeckAxialRotation = UpperNeckAxialRotation;
	body->UpperNeckLateralBending = UpperNeckLateralBending;
	body->LowerNeckFlexion = 90 - LowerNeckFlexion;
	body->LowerNeckAxialRotation = LowerNeckAxialRotation;
	body->LowerNeckLateralBending = LowerNeckLateralBending;
	body->Head_Flexion = body->Trunk_Flexion - (270 - (90 - HeadFlexion + 90 - UpperNeckFlexion + 90 - LowerNeckFlexion));
	body->Head_Axial_Rotation = UpperNeckAxialRotation + LowerNeckAxialRotation;
	body->Head_Lateral_Bending = UpperNeckLateralBending + LowerNeckLateralBending;
	body->Customizedneck = 1;
	body->UpdateAngles();
	body->diff0 = 90 - HeadFlexion - body->Trunk_Flexion;
	body->diff1 = 90 - UpperNeckFlexion - body->Trunk_Flexion;
    body->diff2 = 90 - LowerNeckFlexion - body->Trunk_Flexion;
	body->diff3 = UpperNeckAxialRotation - body->Trunk_Axial_Rotation;
    body->diff4 = UpperNeckLateralBending - body->Trunk_Lateral_Bending;
	body->diff5 = LowerNeckAxialRotation  - body->Trunk_Axial_Rotation;
    body->diff6 = LowerNeckLateralBending - body->Trunk_Lateral_Bending;
	body->diff7 = body->Head_Flexion - body->Trunk_Flexion;
	body->diff8 = body->Head_Lateral_Bending - body->Trunk_Lateral_Bending;
	body->diff9 = body->Head_Axial_Rotation - body->Trunk_Axial_Rotation;
}

void Dg_HeadNeck::OnPrintPrintwindow()
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

void Dg_HeadNeck::OnSysCommand(UINT nID, LPARAM lParam)
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

BOOL Dg_HeadNeck::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->AppendMenu(MF_SEPARATOR);// add a separator
	pSysMenu->AppendMenu(MF_STRING,IDR_MENU1,"Print Window");// add print menu
	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL Dg_HeadNeck::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if(zDelta < 0) 
		Increment(false);
	else if (zDelta > 0)
		Increment(true);
	
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void Dg_HeadNeck::Increment(bool isIncrementing)
{
	// Sign will cause the function to increment or decrement,
	//		depending on the value of isIncrementing
	int sign, minval = -90, maxval = 90;
	double newVal, oldVal;

	if(isIncrementing)
		sign = 1;
	else	// Decrementing
		sign = -1;

	if (this->Current_Control_ID == 0)
	{
		::MessageBox(	NULL,
							"No joint angle active.",
							"Increment/Decrement",
							MB_OK | MB_ICONEXCLAMATION	);
		return;
	}

	if(!this->UpdateData(TRUE)) //Make sure data OK
		return;

	if (this->Current_Control_ID == IDC_HEAD_FLEXION_DYN)
		oldVal = HeadFlexion;
	else if (this->Current_Control_ID == IDC_NECK1_FLEXION_DYN)
		oldVal = UpperNeckFlexion;
	else if (this->Current_Control_ID == IDC_NECK1_LATERAL_DYN)
		oldVal = UpperNeckLateralBending;
	else if (this->Current_Control_ID == IDC_NECK1_ROT_DYN)
		oldVal = UpperNeckAxialRotation;
	else if (this->Current_Control_ID == IDC_NECK2_FLEXION_DYN)
		oldVal = LowerNeckFlexion;
	else if (this->Current_Control_ID == IDC_NECK2_ROT_DYN)
		oldVal = LowerNeckAxialRotation;
	else if (this->Current_Control_ID == IDC_NECK2_LATERAL_DYN)
		oldVal = LowerNeckLateralBending;

	//if (this->Increment_Group == 0)	// "1" selected
	newVal = oldVal + sign * 1;
	//else
		//newVal = newVal + sign * 5 * (this->Increment_Group);

	/*if (this->Current_Control_ID == IDC_FOREARM_LV_DYN || this->Current_Control_ID == IDC_UPPERARM_LV_DYN || this->Current_Control_ID == IDC_CLAVICLE_LV_DYN || 
		this->Current_Control_ID == IDC_UPPERLEG_LV_DYN || this->Current_Control_ID == IDC_LOWERLEG_LV_DYN || this->Current_Control_ID == IDC_FOOT_LV_DYN ||
		this->Current_Control_ID == IDC_FOREARM_RV_DYN || this->Current_Control_ID == IDC_UPPERARM_RV_DYN || this->Current_Control_ID == IDC_CLAVICLE_RV_DYN ||
		this->Current_Control_ID == IDC_UPPERLEG_RV_DYN || this->Current_Control_ID == IDC_LOWERLEG_RV_DYN || this->Current_Control_ID == IDC_FOOT_RV_DYN){
			minval = -90;
			maxval = 90;
	}
	else{
		minval = -180;
		maxval = 180;
	}*/

	if (newVal >= minval && newVal <= maxval){
		if (this->Current_Control_ID == IDC_HEAD_FLEXION_DYN)
			HeadFlexion = newVal;
		else if (this->Current_Control_ID == IDC_NECK1_FLEXION_DYN)
			UpperNeckFlexion = newVal;
		else if (this->Current_Control_ID == IDC_NECK1_LATERAL_DYN)
			UpperNeckLateralBending = newVal;
		else if (this->Current_Control_ID == IDC_NECK1_ROT_DYN)
			UpperNeckAxialRotation = newVal;
		else if (this->Current_Control_ID == IDC_NECK2_FLEXION_DYN)
			LowerNeckFlexion = newVal;
		else if (this->Current_Control_ID == IDC_NECK2_ROT_DYN)
			LowerNeckAxialRotation = newVal;
		else if (this->Current_Control_ID == IDC_NECK2_LATERAL_DYN)
			LowerNeckLateralBending = newVal;
		CString newangle = DblToCStr(newVal, 4, 4);
		SetDlgItemText(this->Current_Control_ID, newangle);
	}
	OnBnClickedButton1();
}

void Dg_HeadNeck::OnEnSetfocusHeadangleDyn()
{
	Current_Control_ID = IDC_HEAD_FLEXION_DYN;
}

void Dg_HeadNeck::OnEnSetfocusUpperFlexionDyn()
{
	Current_Control_ID = IDC_NECK1_FLEXION_DYN;
}

void Dg_HeadNeck::OnEnSetfocusUpperRotationDyn()
{
	Current_Control_ID = IDC_NECK1_ROT_DYN;
}

void Dg_HeadNeck::OnEnSetfocusUpperBendingDyn()
{
	Current_Control_ID = IDC_NECK1_LATERAL_DYN;
}

void Dg_HeadNeck::OnEnSetfocusLowerFlexionDyn()
{
	Current_Control_ID = IDC_NECK2_FLEXION_DYN;
}

void Dg_HeadNeck::OnEnSetfocusLowerRotationDyn()
{
	Current_Control_ID = IDC_NECK2_ROT_DYN;
}

void Dg_HeadNeck::OnEnSetfocusLowerBendingDyn()
{
	Current_Control_ID = IDC_NECK2_LATERAL_DYN;
}