// Dg_SupportSelection.cpp : implementation file
//

#include "stdafx.h"
#include "Dg_SupportSelection.h"
#include "hom_doc.h"
#include "hom.h"
#include "Types.h"
#include ".\dg_supportselection.h"

const float MIN_SEAT_HEIGHT_CM = 19.0f; // 7.5in = 19 cm
const float MIN_SEAT_HEIGHT_IN = 7.5f;
// Dg_SupportSelection dialog
IMPLEMENT_DYNAMIC(Dg_SupportSelection, CDialog)
Dg_SupportSelection::Dg_SupportSelection(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_SupportSelection::IDD, pParent)
{
    C_Hom_Doc* lDocPtr = (C_Hom_Doc*)Get_App_Ptr()->Get_Document_Ptr();
	const Support_s& support = lDocPtr->getSupport();
	currentframe = lDocPtr->getCurrentFrame();
	leftselection = lDocPtr->LeftSelect();
	rightselection = lDocPtr->RightSelect();
	if (leftselection != rightselection)
	{
		selection =true;
	}
	else
	{
		selection = false;
	}
	switch(support.feetSupport) {
		case FS_Both:
			mStance = 0;
			break;
		case FS_Left:
			mStance = 1;
			break;
		case FS_Right:
			mStance = 2;
			break;
		case FS_Neither:
			mStance = 3;
			break;
	}

	mStandingSeated = (support.position == P_Sitting) ? 1 : 0;
	mSeatBack = support.seatHasBackRest;
	mFrontSupport = support.frontSeatPanSupport;
	bool english = lDocPtr->Is_English();
	mBackRestHeight = support.backRestCenterHeight * (english ? (1/2.54) : 1);
	mBackRestUnits = english ? "in" : "cm";
	mPelvicTilt = support.additionalPelvicTilt;
}
std::vector<Frame> Dg_SupportSelection::frames;

Dg_SupportSelection::~Dg_SupportSelection()
{
}

void Dg_SupportSelection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_SupportSelection)
	DDX_Radio(pDX, IDC_BothFeet, mStance);
	DDX_Radio(pDX, IDC_Standing, mStandingSeated);
	DDX_Control(pDX, IDC_NoFoot, mNofoot);
	DDX_Check(pDX, IDC_Backrest, mSeatBack);
	DDX_Check(pDX, IDC_FrontSupport, mFrontSupport);
	DDX_Text(pDX, IDC_RestHeight, mBackRestHeight);
	//DDV_MinMaxDouble(pDX, mBackRestHeight, 0, 1000);
	DDX_Control(pDX, IDC_Backrest, mCHasBackRest);
	DDX_Control(pDX, IDC_FrontSupport, mCFrontSeatPan);
	DDX_Control(pDX, IDC_RestHeight, mCSeatHeight);
	DDX_Control(pDX, IDC_DefaultButton, mCDefaultButton);
	DDX_Text(pDX, IDC_BackRestUnits, mBackRestUnits);
	DDX_Text(pDX, IDC_ADD_PELVIC_TILT, mPelvicTilt);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LeftFoot, mLeftFoot);
	DDX_Control(pDX, IDC_RightFoot, mRightFoot);
}

BEGIN_MESSAGE_MAP(Dg_SupportSelection, CDialog)
    ON_BN_CLICKED(IDC_REDRAW, OnRedraw)
    ON_BN_CLICKED(IDOK, OnOk)
    ON_BN_CLICKED(IDC_DefaultButton, OnSetDefault)
    ON_BN_CLICKED(IDC_Standing, OnBnClickedStanding)
    ON_BN_CLICKED(IDC_Seated, OnSeated)
    ON_BN_CLICKED(IDC_REDRAW, OnRedraw)
    ON_BN_CLICKED(IDC_NoFoot, OnBnClickedNofoot)
    ON_BN_CLICKED(IDC_BothFeet, OnBnClickedBothfeet)
	ON_COMMAND(ID_PRINT_PRINTWINDOW, &Dg_SupportSelection::OnPrintPrintwindow)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

bool Dg_SupportSelection::UpdateDocument()
{
	if(Dg_SupportSelection::frames.size() < 100)
	Dg_SupportSelection::init();                 //initialize the frames,default setting is bothfeet
	if(currentframe >Dg_SupportSelection::frames.size())
	{
		do
		{
			Dg_SupportSelection::init();
		} while (currentframe >= Dg_SupportSelection::frames.size());
	}

	C_Hom_Doc* lDocPtr = Get_App_Ptr()->Get_Document_Ptr();
	Support_s support = lDocPtr->getSupport();
	Task& task = Get_App_Ptr()->Get_Document_Ptr()->getTask();
	int i = currentframe;                                       //current frame number
	bool english = lDocPtr->Is_English();
	char message[256];
	if(mSeatBack) {
		if(mBackRestHeight < (english ? MIN_SEAT_HEIGHT_IN : MIN_SEAT_HEIGHT_CM)) {
			CString units = "in";
			if(!english) {
				units = "cm";
				sprintf(message, "The minimum seat height is %.1f %s", MIN_SEAT_HEIGHT_CM, units);
			} else {
				sprintf(message, "The minimum seat height is %.1f %s", MIN_SEAT_HEIGHT_IN, units);
			}
			AfxMessageBox(message);
			return false;
		}
	}
	if(!english) {
		support.backRestCenterHeight = mBackRestHeight;
	} else {
		support.backRestCenterHeight = mBackRestHeight * 2.54;
	}
	UpdateData(TRUE);
	if(selection == FALSE)
	{
	switch(mStance) {
		case 0:
			support.feetSupport = FS_Both;
			frames[i].feetsupport=FS_Both;
			break;
		case 1:
			support.feetSupport = FS_Left;
			frames[i].feetsupport=FS_Left;
			break;
		case 2:
			support.feetSupport = FS_Right;
			frames[i].feetsupport=FS_Right;
			break;
		case 3:
			support.feetSupport = FS_Neither;
			frames[i].feetsupport=FS_Neither;
			break;
	}
	}
	if(selection == TRUE)
	{
		switch(mStance) {
		case 0:
			support.feetSupport = FS_Both;
			for(int j = leftselection;j <= rightselection;j++)
			{
			frames[j].feetsupport=FS_Both;
			}
			break;
		case 1:
			support.feetSupport = FS_Left;
			for(int j = leftselection;j <= rightselection;j++)
			{
			frames[j].feetsupport=FS_Left;
			}
			break;
		case 2:
			support.feetSupport = FS_Right;
			for(int j = leftselection;j <= rightselection;j++)
			{
			frames[j].feetsupport=FS_Right;
			}
			break;
		case 3:
			support.feetSupport = FS_Neither;
			for(int j = leftselection;j <= rightselection;j++)
			{
			frames[j].feetsupport=FS_Neither;
			}
			break;
	}
	}
	support.frontSeatPanSupport = mFrontSupport;
	support.position = mStandingSeated ? P_Sitting : P_Standing;
	support.seatHasBackRest = mSeatBack;

	// SB 09/28/07 made some changes as we don't want to display negative-signed quantities 
	// on the form for the tilt -- it is confusing to the user.

	// SB 11/08/07 changed the sign on the RHS of the assignment below, from
	// minus to plus. Why? Because we want a positive value to rotate the pelvis
	// backward, not forward.
    support.additionalPelvicTilt = mPelvicTilt;
	support.displayAdditionalPelvicTilt = mPelvicTilt;
	//
	//////////////////////////////////////////////////////////////////


	lDocPtr->setSupport(support);

	lDocPtr->MakeDirtyAndUpdateViews(true);
	return true;
}

// Dg_SupportSelection message handlers

void Dg_SupportSelection::OnRedraw()
{
    this->UpdateData(TRUE);
    this->UpdateDocument();
}

void Dg_SupportSelection::OnOk()
{
    this->UpdateData(TRUE); // put data from editboxes into member variables
    if(UpdateDocument())
    {
        CDialog::OnOK();    // Do default windows Dialog OnOK processing
    }
}

void Dg_SupportSelection::OnSetDefault()
{
    C_Hom_Doc* lDocPtr = (C_Hom_Doc*)Get_App_Ptr()->Get_Document_Ptr();
    this->UpdateData(TRUE);
	if (lDocPtr->Is_English())
        mBackRestHeight = 10;
    else
        mBackRestHeight = 25.4;

    this->UpdateData(FALSE);
}

void Dg_SupportSelection::OnBnClickedStanding()
{
    UpdateData(TRUE);

    if (mStance==3)
    {
        mStance=0;
        UpdateData(FALSE);
    }
    mNofoot.EnableWindow(FALSE);
    mCHasBackRest.EnableWindow(FALSE);
    mCFrontSeatPan.EnableWindow(FALSE);
    mCSeatHeight.EnableWindow(FALSE);
    mCDefaultButton.EnableWindow(FALSE);   
    mLeftFoot.EnableWindow(TRUE);
    mRightFoot.EnableWindow(TRUE);
    mPelvicTilt=0;
    mStandingSeated=0;
    UpdateData(FALSE);
}

void Dg_SupportSelection::OnSeated()
{
    mNofoot.EnableWindow(TRUE);
    mCHasBackRest.EnableWindow(TRUE);
    if(mStance == STANCE_NO_FEET)
    {   
        mCFrontSeatPan.EnableWindow(TRUE);
    }
    mCSeatHeight.EnableWindow(TRUE);
    mCDefaultButton.EnableWindow(TRUE);
    mLeftFoot.EnableWindow(FALSE);
    mRightFoot.EnableWindow(FALSE);
    if (mStance != STANCE_NO_FEET)
        mStance = 0;
	// SB 09/27 Changed this mPelvicTilt to negative.
    mPelvicTilt = -15;
    mStandingSeated = 1;
    UpdateData(FALSE);
}

BOOL Dg_SupportSelection::OnInitDialog()
{
    CDialog::OnInitDialog();
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->AppendMenu(MF_SEPARATOR);// add a separator
	pSysMenu->AppendMenu(MF_STRING,IDR_MENU1,"Print Window");// add print menu
    if (mStandingSeated==0) {
        mNofoot.EnableWindow(FALSE);
        mCHasBackRest.EnableWindow(FALSE);
        mCFrontSeatPan.EnableWindow(FALSE);
        mCSeatHeight.EnableWindow(FALSE);
        mCDefaultButton.EnableWindow(FALSE);
    }
	else
    {
        if(mStance != STANCE_NO_FEET)
        {
             mCFrontSeatPan.EnableWindow(FALSE);
        }
        mLeftFoot.EnableWindow(FALSE);
        mRightFoot.EnableWindow(FALSE);
    }

       
    return true;
}

void Dg_SupportSelection::OnBnClickedNofoot()
{
     mCFrontSeatPan.EnableWindow(TRUE);
}

void Dg_SupportSelection::OnBnClickedBothfeet()
{
    mCFrontSeatPan.EnableWindow(FALSE);
    mCFrontSeatPan.SetCheck(BST_UNCHECKED);
}


void Dg_SupportSelection::OnPrintPrintwindow()
{
	// TODO: Add your command handler code here
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


void Dg_SupportSelection::OnSysCommand(UINT nID, LPARAM lParam)
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
