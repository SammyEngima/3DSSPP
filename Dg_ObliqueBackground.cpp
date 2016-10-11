#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h"
#include "Dg_ObliqueBackground.h"
#include "GLObliqueView.h"
#include ".\dg_obliquebackground.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

Dg_ObliqueBackground::Dg_ObliqueBackground(CWnd* pParent) : CDialog(Dg_ObliqueBackground::IDD, pParent)
{
    lDocPtr = Get_App_Ptr()->Get_Document_Ptr();
    CGLObliqueView *oblique = (CGLObliqueView*)(lDocPtr->GLViews[VIEW_OBLIQUE]);

    mode = oblique->getBackgroundMode();
    picture = oblique->getBackground();
    if(picture) picture->AddRef();
    useBG = (BOOL)(oblique->getBackgroundActive());
    alpha = (int)floor(10.0*oblique->getBackgroundAlpha() + 0.5);

    memcpy(mRGB,GLOblique::backgroundColor,3*sizeof(unsigned char));
}

Dg_ObliqueBackground::~Dg_ObliqueBackground(void)
{
    if(picture) picture->Release();
}


void Dg_ObliqueBackground::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(Dg_ObliqueBackground)
    DDX_Radio(pDX, IDC_USIMAG, picOrColorChoice);
	// DDX_Check(pDX, IDC_USCOLOR, useCOL);
    DDX_Radio(pDX, IDC_CENTER_DISPLAY, mode);
    DDX_Slider(pDX, IDC_ALPHASLIDER, alpha);
    DDX_Control(pDX, IDC_ALPHASLIDER, alphaSlider);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Dg_ObliqueBackground, CDialog)
    //{{AFX_MSG_MAP(Dg_ObliqueBackground)
    ON_BN_CLICKED(IDC_OBL_COLOR, OnOblColor)
    ON_BN_CLICKED(IDC_BROWSE, OnOblNewimage)
    ON_BN_CLICKED(IDAPPLY, OnApply)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_USIMAG, &Dg_ObliqueBackground::OnBnClickedUsimag)
	ON_BN_CLICKED(IDC_USCOLOR, &Dg_ObliqueBackground::OnBnClickedUscolor)
END_MESSAGE_MAP()



void Dg_ObliqueBackground::OnOblColor() 
{
    COLORREF Ini_Color = RGB(mRGB[0], mRGB[1], mRGB[2]);
    CColorDialog Diag(Ini_Color);
    if(Diag.DoModal() == IDOK)
    {
        COLORREF New_Color = Diag.GetColor();
        if(New_Color != Ini_Color)
        {
            mRGB[0] = GetRValue(New_Color);
            mRGB[1] = GetGValue(New_Color);
            mRGB[2] = GetBValue(New_Color);
        }
    }

    Invalidate(FALSE);
}

// Returns true if OK to close dialog now, otherwise false
bool Dg_ObliqueBackground::UpdateDocument()

{
	C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
	doc->setPictureOrColorChoice((PictureOrColor)picOrColorChoice);
    CGLObliqueView *oblique = (CGLObliqueView*)(lDocPtr->GLViews[VIEW_OBLIQUE]);
     
	int background_is_picture = 0;
    oblique->setBackgroundMode((Picture_Mode)mode);
    if(picture != oblique->getBackground())
    {
        oblique->setBackground(picture);

		// Sayan 09/13/07 
		// Now we're keeping track of whether the background is a picture or not!
		background_is_picture = 1;

    }
    oblique->setBackgroundActive(((PictureOrColor)picOrColorChoice)==PICTURE);

	for(int i = 0; i < 3; ++i) {
        GLOblique::backgroundColor[i] = mRGB[i];
	}
    oblique->setBackgroundAlpha((float)alpha/10.0f);

    lDocPtr->MakeDirtyAndUpdateViews(FALSE);
    return true;
}

// Arthur -- Called when the browse button is pressed
void Dg_ObliqueBackground::OnOblNewimage() 
{
    CString lFilters = "Image Files|*.jpg; *.bmp; *.png; *.ico|Jpeg Images (*.jpg)|*.jpg|GIF Images (*.gif)|*.gif|Bitmap Images (*.bmp)|*.bmp|PNG Imges (*.png)|*.png|Icon Images (*.ico)|*.ico||";

    CFileDialog	lDiag(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR,lFilters,NULL);

    if(lDiag.DoModal() == IDOK)
    {
        UpdateData(TRUE);
        LoadPictureFile((LPCTSTR)(lDiag.GetPathName()),&picture);
          
        // If we've got a picture, enable the checkbox to use it.
        //GetDlgItem(IDC_USEIMG)->EnableWindow(TRUE);
        GetDlgItem(IDC_LABEL_POSITION)->EnableWindow(TRUE);
        GetDlgItem(IDC_CENTER_DISPLAY)->EnableWindow(TRUE);
        GetDlgItem(IDC_STRETCH_DISPLAY)->EnableWindow(TRUE);
        alphaSlider.EnableWindow(TRUE);

        useBG = TRUE;
        UpdateData(FALSE);
    }

    Invalidate(FALSE);
}

void Dg_ObliqueBackground::OnCancel() 
{
    CDialog::OnCancel();
}

void Dg_ObliqueBackground::OnOK() 
{
    if(!(this->UpdateData(TRUE)))
        return;

    if(!(this->UpdateDocument()))
        return;
    Dg_ObliqueBackground::OnApply();
    CDialog::OnOK();
}

void Dg_ObliqueBackground::OnApply() 
{
    if(this->UpdateData(TRUE))
        this->UpdateDocument();
}

#undef BORDER_WIDTH
#define BORDER_WIDTH 2

BOOL Dg_ObliqueBackground::LoadPictureFile(LPCTSTR szFile,LPPICTURE* pgpPicture)
{
    // open file
    HANDLE hFile = CreateFile(szFile,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
    if(hFile == INVALID_HANDLE_VALUE)
    {
        AfxMessageBox ("Could not read file");
        return FALSE;
    }

    // get file size
    DWORD dwFileSize = GetFileSize(hFile, NULL);

    if(dwFileSize == (DWORD)-1)
    {
        CloseHandle(hFile);
        AfxMessageBox ("File seems to be empty");
        return FALSE;
    }

    LPVOID pvData = NULL;

    // alloc memory based on file size
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);

    if(hGlobal == NULL)
    {
        CloseHandle(hFile);
        AfxMessageBox("Could not allocate memory for image");
        return FALSE;
    }

    pvData = GlobalLock(hGlobal);

    if(pvData == NULL)
    {
        GlobalUnlock(hGlobal);
        CloseHandle(hFile);
        AfxMessageBox("Could not lock memory for image");
        return FALSE;
    }

    DWORD dwBytesRead = 0;

    // read file and store in global memory
    BOOL bRead = ReadFile(hFile,pvData,dwFileSize,&dwBytesRead,NULL);

    GlobalUnlock(hGlobal);
    CloseHandle(hFile);

    if(!bRead)
    {
        AfxMessageBox("Could not read file");
        return FALSE;
    }

    LPSTREAM pstm = NULL;

    // create IStream* from global memory
    HRESULT hr = CreateStreamOnHGlobal(hGlobal,TRUE,&pstm);

    if(!(SUCCEEDED(hr)))
    {
        AfxMessageBox("CreateStreamOnHGlobal() failed");
        if(pstm != NULL) pstm->Release();
        return FALSE;
    }

    else if(pstm == NULL)
    {
        AfxMessageBox("CreateStreamOnHGlobal() failed");
        return FALSE;
    }

    // Create IPicture from image file
    if(*pgpPicture) (*pgpPicture)->Release();

    hr = ::OleLoadPicture(pstm,dwFileSize,FALSE,IID_IPicture,(LPVOID *)&(*pgpPicture));

    if(!(SUCCEEDED(hr)))
    {
        pstm->Release();
        AfxMessageBox("Could not load image (hr failure)");
        return FALSE;
    }

    else if(*pgpPicture == NULL)
    {
        pstm->Release();
        AfxMessageBox("Could not load image (pgpPicture failure)");
        return FALSE;
    }

    pstm->Release();

    return TRUE;
}

void Dg_ObliqueBackground::DrawImage(CDC* pDC,CPoint ptCenter,HWND hWnd,CRect clientRect)
{
    if(pDC == NULL || picture == NULL || hWnd == NULL)
        return;

    //Get width and height of picture
    CSize HMimgRect;
    CSize DPimgRect;
    CSize LPimgRect;

    picture->get_Width (&HMimgRect.cx);
    picture->get_Height(&HMimgRect.cy);
    LPimgRect = HMimgRect;
    DPimgRect = HMimgRect;
    pDC->HIMETRICtoDP(&DPimgRect);
    pDC->HIMETRICtoLP(&LPimgRect);
    CRect rc;
    GetWindowRect(&rc);
    HRESULT hrP = NULL;

    hrP = picture->Render(pDC->m_hDC, ptCenter.x - clientRect.Width()/2,
        ptCenter.y - clientRect.Height()/2,
        clientRect.Width(), clientRect.Height(), 0, HMimgRect.cy,
        HMimgRect.cx, -HMimgRect.cy, &rc);
}

void Dg_ObliqueBackground::OnPaint() 
{
    CPaintDC dc(this); // device context for painting

    CWnd *ImageFrame = GetDlgItem(IDC_PICTURE);
    CWnd *ColorFrame = GetDlgItem(IDC_COLOR);

    CRect ImageRect;
    CRect ColorRect;

    ImageFrame->GetWindowRect(ImageRect);
    ColorFrame->GetWindowRect(ColorRect);
    CRect ImageDrawRect(ImageRect.left + BORDER_WIDTH,ImageRect.top + BORDER_WIDTH,
                        ImageRect.right - BORDER_WIDTH,ImageRect.bottom - BORDER_WIDTH);
    CRect ColorDrawRect(ColorRect.left + BORDER_WIDTH,ColorRect.top + BORDER_WIDTH,
                        ColorRect.right - BORDER_WIDTH,ColorRect.bottom - BORDER_WIDTH);

    ScreenToClient(ImageDrawRect);
    ScreenToClient(ColorDrawRect);
    CBrush colorBrush(RGB(mRGB[0],mRGB[1],mRGB[2]));
    dc.FillRect(ColorDrawRect, &colorBrush);

    CPoint center;
    int x = ImageDrawRect.Width() / 2;
    int y = ImageDrawRect.Height() / 2;
    center = CPoint(x+ImageDrawRect.left,y+ImageDrawRect.top);

    DrawImage(&dc,center,GetSafeHwnd(),ImageDrawRect);
}

BOOL Dg_ObliqueBackground::OnInitDialog()
{
    CDialog::OnInitDialog();
	C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();

	

	if(doc->getPictureOrColorChoice() == PICTURE) {

    // Initialize transparency slider
    // alphaSlider.SetRange(0,10,TRUE);

    // gray-out irrelevant items based on status of background image
    // if((PictureOrColor)picOrColorChoice == COLOR)
    
        // GetDlgItem(IDC_USIMAG)->EnableWindow(FALSE);

		picOrColorChoice = PICTURE;
        GetDlgItem(IDC_LABEL_POSITION)->EnableWindow(TRUE);
        GetDlgItem(IDC_CENTER_DISPLAY)->EnableWindow(TRUE);
        GetDlgItem(IDC_STRETCH_DISPLAY)->EnableWindow(TRUE);
		alphaSlider.SetRange(0,10,TRUE);
        alphaSlider.EnableWindow(FALSE);
	
    }
    else 
    {
        picOrColorChoice = COLOR;
        GetDlgItem(IDC_LABEL_POSITION)->EnableWindow(FALSE);
        GetDlgItem(IDC_CENTER_DISPLAY)->EnableWindow(FALSE);
        GetDlgItem(IDC_STRETCH_DISPLAY)->EnableWindow(FALSE);
	    alphaSlider.SetRange(0,10,TRUE);
        alphaSlider.EnableWindow(FALSE);




		
	 
	}

    UpdateData(false);
	DisableFieldsAsNecessary();

    return TRUE;
}

void Dg_ObliqueBackground::OnBnClickedUsimag()
{
	CGLObliqueView *oblique = (CGLObliqueView*)(lDocPtr->GLViews[VIEW_OBLIQUE]);
	if(!oblique->getBackground()) {
		CString lFilters = "Image Files|*.jpg; *.bmp; *.png; *.ico|Jpeg Images (*.jpg)|*.jpg|GIF Images (*.gif)|*.gif|Bitmap Images (*.bmp)|*.bmp|PNG Imges (*.png)|*.png|Icon Images (*.ico)|*.ico||";

		CFileDialog	lDiag(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR,lFilters,NULL);

		if(lDiag.DoModal() == IDOK) {
			UpdateData(TRUE);
			LoadPictureFile((LPCTSTR)(lDiag.GetPathName()),&picture);

			// If we've got a picture, enable the checkbox to use it.
			//GetDlgItem(IDC_USEIMG)->EnableWindow(TRUE);
			GetDlgItem(IDC_LABEL_POSITION)->EnableWindow(TRUE);
			GetDlgItem(IDC_CENTER_DISPLAY)->EnableWindow(TRUE);
			GetDlgItem(IDC_STRETCH_DISPLAY)->EnableWindow(TRUE);
			alphaSlider.EnableWindow(TRUE);

			useBG = TRUE;
			UpdateData(FALSE);
		} else {
			UpdateData(false);
			return;
		}
	}

    Invalidate(FALSE);

	UpdateData(TRUE);
	this->DisableFieldsAsNecessary();
	UpdateData(false); 
}

void Dg_ObliqueBackground::OnBnClickedUscolor()
{
	UpdateData(TRUE);
	this->DisableFieldsAsNecessary();
	UpdateData(false);


}

void Dg_ObliqueBackground::DisableFieldsAsNecessary()
{
   UpdateData(true);
   switch ((PictureOrColor)picOrColorChoice) {
	 case COLOR:
        GetDlgItem(IDC_LABEL_POSITION)->EnableWindow(FALSE);
        GetDlgItem(IDC_CENTER_DISPLAY)->EnableWindow(FALSE);
        GetDlgItem(IDC_STRETCH_DISPLAY)->EnableWindow(FALSE);
        alphaSlider.EnableWindow(FALSE);
	    GetDlgItem(IDC_OBL_COLOR)->EnableWindow(TRUE);
	    GetDlgItem(IDC_BROWSE)->EnableWindow(FALSE);
        // GetDlgItem(IDC_USCOLOR)->EnableWindow(TRUE);

      
		break;

     case PICTURE:

		// GetDlgItem(IDC_USIMAG)->EnableWindow(TRUE);
        GetDlgItem(IDC_LABEL_POSITION)->EnableWindow(TRUE);
        GetDlgItem(IDC_CENTER_DISPLAY)->EnableWindow(TRUE);
        GetDlgItem(IDC_STRETCH_DISPLAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BROWSE)->EnableWindow(TRUE);
        alphaSlider.EnableWindow(TRUE);
		GetDlgItem(IDC_OBL_COLOR)->EnableWindow(FALSE);
	    GetDlgItem(IDC_BROWSE)->EnableWindow(TRUE);
     
		break;
   }
} 

