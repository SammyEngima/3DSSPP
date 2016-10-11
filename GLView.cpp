#include "stdafx.h"
#include "GLView.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "Main_frm.h"
#include "ReportView.hpp"
#include "hom.h"
#include "hom_doc.h"
#include "resource.h"
#include "GLInteractive.h"
#include "GLOblique.h"
#include "dibview.h"
#include "AngleCue.h"
#include "CDIB.H"
#include "DIB.h"

IMPLEMENT_DYNCREATE(CGLView, CView)

CGLView::CGLView() {
	glInteractive = 0;
	printView = false;
	printDib = 0;
}

CGLView::~CGLView()
{
	delete glInteractive;
	glInteractive = 0;
	delete printDib;
	printDib = 0;
}

void CGLView::cueAngle(const AngleCueData& ACD) {
	if(glInteractive)
		glInteractive->cueAngle(ACD);
}
void CGLView::cancelCue() {
	if(glInteractive)
		glInteractive->cancelCue();
}

GLInteractive* CGLView::getGLInteractive() {
	return glInteractive;
}

BEGIN_MESSAGE_MAP(CGLView, CView)
    ON_WM_CREATE()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_CONTEXTMENU()
    ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_FILE_COPYACTIVEWINDOW, &CGLView::OnFileCopyactivewindow)
	ON_WM_KEYUP()
	ON_COMMAND(ID_FILE_PRINT, &CGLView::OnFilePrint)
	ON_MESSAGE(WM_DRAWOPENGL, OnDrawOpenGL)
END_MESSAGE_MAP()
#include <sstream>
void CGLView::OnDraw(CDC* pDC)
{
	if(glInteractive) {
		glInteractive->onDraw();
	}
}

#ifdef _DEBUG
void CGLView::AssertValid() const { CView::AssertValid(); }
void CGLView::Dump(CDumpContext& dc) const { CView::Dump(dc); }
#endif

int CGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    return 0;
}

void CGLView::OnInitialUpdate()
{
	glInteractive->initialize(0,printView);
	CRect rect;
	GetClientRect(&rect);
	glInteractive->setViewport(rect);

    CView::OnInitialUpdate();

    mDibViewPtr = NULL;
}

BOOL CGLView::OnEraseBkgnd(CDC* pDC)
{
    if (pDC->IsPrinting())
        return CView::OnEraseBkgnd(pDC);
    else
        return FALSE; 
}

void CGLView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);
	CRect rect;
	GetClientRect(&rect);
	glInteractive->setViewport(rect);
}
void CGLView::OnContextMenu(CWnd* pWnd, CPoint point) {
	glInteractive->createContextMenu(point);
}

BOOL CGLView::OnPreparePrinting(CPrintInfo* pInfo)
{

	// TODO printing
	//return CView::OnPreparePrinting(pInfo);
	//printDib = new DIB(this->GetParentFrame());
	pInfo->m_bDirect = true;
	return DoPreparePrinting(pInfo);


	//if(!printView) {
//		return Get_App_Ptr()->Get_Document_Ptr()->GLPrintViews[VIEW_TOP]->OnPreparePrinting(pInfo);
	//}

    C_Main_Frame* lMainFramePtr = MainFramePtr();
    if(mDibViewPtr == NULL) mDibViewPtr = lMainFramePtr->Create_Dib_Window();

    CWnd* lFramePtr = this->GetParent();
    ASSERT(lFramePtr != NULL);

    //--- Save existing placement ---
    WINDOWPLACEMENT lSavePlacement;
    lFramePtr->GetWindowPlacement(&lSavePlacement);

    //--- Compute and set print placement ---
    WINDOWPLACEMENT lPrintPlacement;
    lFramePtr->GetWindowPlacement(&lPrintPlacement);

    //--- Create the DIB ---

    // Added so the print preview toolbars do not appear in the final output
    lFramePtr->SetActiveWindow();
    lFramePtr->BringWindowToTop();
    lFramePtr->UpdateWindow();
    mDibViewPtr->LoadDib(lFramePtr);
       
    //--- Call normal printing ---
    pInfo->m_bDirect = true;
    BOOL lPrintResult = mDibViewPtr->OnPreparePrinting(pInfo);  
       
    //--- Restore the window ---
    lFramePtr->SetWindowPlacement(&lSavePlacement);
    lFramePtr->ShowWindow(lSavePlacement.showCmd);
    lFramePtr->UpdateWindow();
       
    return lPrintResult;
}

void CGLView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	if(printDib) {
		printDib->RenderToDC(pDC);
	}

	//DIB dib(this->GetParentFrame());
	//dib.RenderToDC(pDC);

	// TODO printing
	//if(mDibViewPtr != NULL) mDibViewPtr->OnDraw(pDC);

	/*PrePrint(pDC, pInfo);
	if(glInteractive) {
		glInteractive->onDraw(m_hMemDC, m_hMemRC);
	}
	PostPrint(pDC);*/
}
void CGLView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) {
}
void CGLView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) {
	/*if(printDib) {
		delete printDib;
		printDib = 0;
	}*/
}

void CGLView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView)
{
	CView::OnEndPrintPreview(pDC, pInfo, point, pView);
}

// pixel format descriptor for printing opengl windows
/*const PIXELFORMATDESCRIPTOR PrintPFD =
{
	sizeof(PIXELFORMATDESCRIPTOR),					// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_BITMAP |						// Format Must Support Drawing to a window
		PFD_SUPPORT_OPENGL |						// Must support OpenGL,
		PFD_SUPPORT_GDI,
		PFD_TYPE_RGBA,								// Request An RGBA Format
		32,											// Select Our Color Depth [32 bit?]
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,										// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
};*/
const PIXELFORMATDESCRIPTOR PrintPFD =
{
	sizeof(PIXELFORMATDESCRIPTOR),					// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_BITMAP |						// Format Must Support Drawing to a window
		PFD_SUPPORT_OPENGL/* |						// Must support OpenGL,
		PFD_SUPPORT_GDI*/,
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,											// Select Our Color Depth [32 bit?]
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,										// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
};


bool SetDCPixelFormat(HDC hDC, DWORD dwFlags)
{
	PIXELFORMATDESCRIPTOR pixelDesc;
	
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;
	
	pixelDesc.dwFlags = dwFlags;
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 24;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;
	
	int nPixelIndex = ::ChoosePixelFormat(hDC, &pixelDesc);
	if (nPixelIndex == 0) // Choose default
	{
		nPixelIndex = 1;
		if (::DescribePixelFormat(hDC, nPixelIndex, 
			sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0)
			return false;
	}

	if (!::SetPixelFormat(hDC, nPixelIndex, &pixelDesc))
		return false;

	return true;
}
void CGLView::PrePrint(CDC* pDC, CPrintInfo* pInfo) {
	
	// 1. Determine the DIB size for either printing or print preview.
	CRect rcClient;
	this->GetClientRect(&rcClient);
	float fClientRatio = float(rcClient.Height())/rcClient.Width();

	// Get page size
	m_szPage.cx  = pDC->GetDeviceCaps(HORZRES);
	m_szPage.cy = pDC->GetDeviceCaps(VERTRES);

	CSize szDIB;
	if (pInfo->m_bPreview)
	{
		szDIB.cx = rcClient.Width();
		szDIB.cy = rcClient.Height();
	}
	else  // Printing
	{
		// Use higher resolution for printing.
		// Adjust size according screen's ratio.
		if (m_szPage.cy > fClientRatio*m_szPage.cx)
		{
			// View area is wider than Printer area
			szDIB.cx = m_szPage.cx;
			szDIB.cy = long(fClientRatio*m_szPage.cx);
		}
		else
		{
			// View area is narrower than Printer area
			szDIB.cx = long(float(m_szPage.cy)/fClientRatio);
			szDIB.cy = m_szPage.cy;
		}
		// Reduce the Resolution if the Bitmap size is too big.
		// Ajdust the maximum value, which is depends on printer's memory.
		// I use 20 MB. 
		/*while (szDIB.cx*szDIB.cy > 20e6)   
		{
			szDIB.cx = szDIB.cx/2;
			szDIB.cy = szDIB.cy/2;
		}*/
	}

	TRACE("Buffer size: %d x %d = %6.2f MB\n", szDIB.cx, szDIB.cy, szDIB.cx*szDIB.cy*0.000001);
	
	// 2. Create DIB Section
	memset(&m_bmi, 0, sizeof(BITMAPINFO));
	m_bmi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_bmi.bmiHeader.biWidth			= szDIB.cx;
	m_bmi.bmiHeader.biHeight		= szDIB.cy;
	m_bmi.bmiHeader.biPlanes		= 1;
	m_bmi.bmiHeader.biBitCount		= 24;
	m_bmi.bmiHeader.biCompression	= BI_RGB;
	m_bmi.bmiHeader.biSizeImage		= szDIB.cx * szDIB.cy * 3;

	HDC	hDC = ::GetDC(this->m_hWnd);
	m_hDib = ::CreateDIBSection(hDC, &m_bmi, DIB_RGB_COLORS, &m_pBitmapBits, NULL, (DWORD)0);
	::ReleaseDC(this->m_hWnd, hDC);

	// 3. Create memory DC, and associate it with the DIB.
	m_hMemDC = ::CreateCompatibleDC(NULL);
	if (!m_hMemDC)
	{
		DeleteObject(m_hDib);
		m_hDib = NULL;
		return;
	}
	SelectObject(m_hMemDC, m_hDib);

	// 4. Setup memory DC's pixel format.
	if (!SetDCPixelFormat(m_hMemDC, PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL | PFD_STEREO_DONTCARE))
	{
		DeleteObject(m_hDib);
		m_hDib = NULL;
		DeleteDC(m_hMemDC);
		m_hMemDC = NULL;
		return;
	}

	// 4.5 set the viewport in the gl renderer
	m_savedViewport = this->glInteractive->getViewingRect();
	glInteractive->setViewport(CRect(0,0,szDIB.cx, szDIB.cy));

	/*const PIXELFORMATDESCRIPTOR* pfd = &PrintPFD;

	int	PixelFormat;
	// Did Windows Find A Matching Pixel Format?
	if (!(PixelFormat = ChoosePixelFormat(m_hMemDC, pfd)))
	{
		MessageBox("Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		
		DeleteObject(m_hDib);
		m_hDib = NULL;
		DeleteDC(m_hMemDC);
		m_hMemDC = NULL;
		return;
	}
	
	// Are We Able To Set The Pixel Format?
	if(!SetPixelFormat(m_hMemDC,PixelFormat, pfd))
	{
		MessageBox("Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		
		DeleteObject(m_hDib);
		m_hDib = NULL;
		DeleteDC(m_hMemDC);
		m_hMemDC = NULL;
		return;
	}*/

	// 5. Create memory RC
	m_hMemRC = ::wglCreateContext(m_hMemDC);
	if (!m_hMemRC)
	{
		DeleteObject(m_hDib);
		m_hDib = NULL;
		DeleteDC(m_hMemDC);
		m_hMemDC = NULL;
		return;
	}

	// 6. Store old DC and RC
	m_hOldDC = ::wglGetCurrentDC();
	m_hOldRC = ::wglGetCurrentContext(); 

	// 7. Make the memory RC current
	::wglMakeCurrent(m_hMemDC, m_hMemRC);

	// 8. Set OpenGL state for memory RC. 
	// The state is the same as the screen RC's.
	/*SetOpenGLState();
	::glViewport(0, 0, szDIB.cx, szDIB.cy);
	SetFrustum();*/

	// 9. Create display list with the newly created memory RC
	//CreateDisplayList(1);
	glInteractive->init();
}
void CGLView::PostPrint(CDC* pDC) {
	
	// Now the image is in the DIB already. We don't need the memory RC anymore. 
	// We'll copy the image to the DC for printing or print preview.

	// 1. Release memory RC, and restore the old DC and RC.
	::wglMakeCurrent(NULL, NULL);
	::wglDeleteContext(m_hMemRC);

	// Restore last DC and RC
	::wglMakeCurrent(m_hOldDC, m_hOldRC);

	// Restore viewport
	glInteractive->setViewport(m_savedViewport);

	// 2. Calculate the target size according to the image size, and orientation of the paper.
	float fBmiRatio = float(m_bmi.bmiHeader.biHeight) / m_bmi.bmiHeader.biWidth;
	CSize szTarget;  
	if (m_szPage.cx > m_szPage.cy)	 // Landscape page
	{
		if(fBmiRatio<1)	  // Landscape image
		{
			szTarget.cx = m_szPage.cx;
			szTarget.cy = long(fBmiRatio * m_szPage.cx);
		}
		else			  // Portrait image
		{
			szTarget.cx = long(m_szPage.cy/fBmiRatio);
			szTarget.cy = m_szPage.cy;
		}
	}
	else		    // Portrait page
	{
		if(fBmiRatio<1)	  // Landscape image
		{
			szTarget.cx = m_szPage.cx;
			szTarget.cy = long(fBmiRatio * m_szPage.cx);
		}
		else			  // Portrait image
		{
			szTarget.cx = long(m_szPage.cy/fBmiRatio);
			szTarget.cy = m_szPage.cy;
		}
	}

	CSize szOffset((m_szPage.cx - szTarget.cx) / 2, (m_szPage.cy - szTarget.cy) / 2);

	// 3. Stretch image to fit in the target size.
	int nRet = ::StretchDIBits(pDC->GetSafeHdc(),
							  szOffset.cx, szOffset.cy,
							  szTarget.cx, szTarget.cy,
							  0, 0,
							  m_bmi.bmiHeader.biWidth, m_bmi.bmiHeader.biHeight,
							  (GLubyte*) m_pBitmapBits,
							  &m_bmi, DIB_RGB_COLORS, SRCCOPY);

	if(nRet == GDI_ERROR)
		TRACE0("Failed in StretchDIBits()");

	// 4. Release memory.
	DeleteObject(m_hDib);
	m_hDib = NULL;
	DeleteDC(m_hMemDC);	
	m_hMemDC = NULL;
	m_hOldDC = NULL;
}
void CGLView::OnFilePrintPreview(void) {
	/*this->SetActiveWindow();
	this->UpdateWindow();
	this->BringWindowToTop();
	this->InvalidateRect(0);*/
	if(printDib) {
		delete printDib;
		printDib = 0;
	}
	printDib = new DIB(this->GetParentFrame());
	// TODO printing
	CView::OnFilePrintPreview();
	return;

	//if(printView) {
		SSPPPrintPreview(this);
	//} else {
	//	SSPPPrintPreview(Get_App_Ptr()->Get_Document_Ptr()->GLPrintViews[VIEW_TOP]);
	//}
}

void CGLView::OnFilePrint() {
	if(printDib) {
		delete printDib;
		printDib = 0;
	}
	printDib = new DIB(this->GetParentFrame());
	CView::OnFilePrint();
}


void CGLView::OnMouseMove(UINT nFlags, CPoint point) {
	if(!glInteractive) return;
	glInteractive->handleMouseMove(nFlags, point);
}
BOOL CGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint point) {
	if(!glInteractive) return true;
	glInteractive->handleMouseWheel(nFlags, zDelta, point);
	return CView::OnMouseWheel(nFlags, zDelta, point);
}
void CGLView::OnLButtonDown(UINT nFlags, CPoint point) {
	if(!glInteractive) return;
	glInteractive->handleLButtonDown(nFlags, point);
	CView::OnLButtonDown(nFlags, point); 
}
void CGLView::OnLButtonUp(UINT nFlags, CPoint point) {
	if(!glInteractive) return;
	glInteractive->handleLButtonUp(nFlags, point);
	CView::OnLButtonUp(nFlags, point);
}
void CGLView::OnRButtonDown(UINT nFlags, CPoint point) {
	if(!glInteractive) return;
	glInteractive->handleRButtonDown(nFlags, point);
	CView::OnRButtonDown(nFlags, point);
}
void CGLView::OnRButtonUp(UINT nFlags, CPoint point) {
	if(!glInteractive) return;
	glInteractive->handleRButtonUp(nFlags, point);
	CView::OnRButtonUp(nFlags, point);
}
void CGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if(!glInteractive) return;
	glInteractive->handleKeyDown(nChar, nRepCnt, nFlags);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CGLView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if(!glInteractive) return;
	glInteractive->handleKeyUp(nChar, nRepCnt, nFlags);
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

afx_msg LRESULT CGLView::OnDrawOpenGL(WPARAM wparam, LPARAM lparam) {
	glInteractive->init();

	// set the viewport correctly to the client area of the view
	CRect rect;
	this->GetClientRect(&rect);
	CRect frameRect;
	this->GetParentFrame()->GetWindowRect(&frameRect);
	this->ScreenToClient(&frameRect);
	rect.OffsetRect(-frameRect.left, frameRect.bottom-rect.bottom);
	CRect oldRect = glInteractive->getViewingRect();
	glInteractive->setViewport(rect);

	glInteractive->onDraw((HDC)wparam, (HGLRC)lparam);

	// restore viewport
	glInteractive->setViewport(oldRect);
	return TRUE;
}

BOOL CGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= (CS_OWNDC | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	return CView::PreCreateWindow(cs);
}

void CGLView::OnFileCopyactivewindow()
{
	DIB dib(this->GetParentFrame());
	dib.PutOnClipboard();

	// TODO printing
	/*
	CDib dib(this->GetParentFrame());

	//Attempt to open the clipboard
   if ( ! this->OpenClipboard() )
   {
      AfxMessageBox( "CDibView::CopyDIBToClipboard: Cannot open the Clipboard" );
      return;
   }
   
   //Remove the current Clipboard contents
   if( ! EmptyClipboard() )
   {
      AfxMessageBox( "DibView::CopyDIBToClipboard: Cannot empty the Clipboard" );
      return;
   }

   //Get the data to copy
   HANDLE   lBitMapInfoHdl = dib.BitMapInfoHdl();

   if( lBitMapInfoHdl != NULL )
   {
      //Give to the clipboard
      if ( ::SetClipboardData( CF_DIB, lBitMapInfoHdl ) == NULL )
      {
	 AfxMessageBox( "DibView::CopyDIBToClipboard: Unable to set Clipboard data" );
      }
   }

   CloseClipboard();*/
	
}


BOOL CGLView::PreTranslateMessage(MSG* pMsg)
{
	// TODO printing 
	if(pMsg->message == WM_HOTKEY) {
		int x = 0;
	}

	return CView::PreTranslateMessage(pMsg);
}
