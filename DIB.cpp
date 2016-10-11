#include "stdafx.h"
#include "DIB.h"
#include "Hom.h"
#include "MAIN_FRM.h"

DIB::DIB(void) {
	// get DIB of desktop

	CSize szDIB;
	HDC Screen_Hdl = GetDC( NULL ); //Handle to screen

	CDC   Screen_DC;
	Screen_DC.Attach( Screen_Hdl );

	//--- Create Screen Compatible Memory DC ---
	CDC*   Screen_Memory_DC_Ptr = new CDC;
	Screen_Memory_DC_Ptr->CreateCompatibleDC( &Screen_DC );

	//--- Create Screen Compatible Bitmap ---
	int   Width  = Screen_DC.GetDeviceCaps( HORZRES );
	int   Height = Screen_DC.GetDeviceCaps( VERTRES );

	szDIB.SetSize(Width, Height);
	
	// Create DIB Section
	memset(&m_bmi, 0, sizeof(BITMAPINFO));
	m_bmi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_bmi.bmiHeader.biWidth			= szDIB.cx;
	m_bmi.bmiHeader.biHeight		= szDIB.cy;
	m_bmi.bmiHeader.biPlanes		= 1;
	m_bmi.bmiHeader.biBitCount		= 24;
	m_bmi.bmiHeader.biCompression	= BI_RGB;
	// round up to the nearest 4 bytes on the width because it word aligns
	m_bmi.bmiHeader.biSizeImage		= (szDIB.cx * 3 + ((4-(szDIB.cx*3 % 4))%4) ) * szDIB.cy;

	m_hDib = ::CreateDIBSection(Screen_Hdl, &m_bmi, DIB_RGB_COLORS, &m_pBitmapBits, NULL, (DWORD)0);

	// Create memory DC, and associate it with the DIB.
	m_hMemDC = ::CreateCompatibleDC(NULL);
	if (!m_hMemDC)
	{
		DeleteObject(m_hDib);
		m_hDib = NULL;
		return;
	}
	SelectObject(m_hMemDC, m_hDib);

	// Copy screen bits to memory context
	::BitBlt(m_hMemDC, 0,0, Width, Height, Screen_Hdl, 0,0, SRCCOPY);
}

DIB::~DIB(void) {
	// 4. Release memory.
	DeleteObject(m_hDib);
	m_hDib = NULL;
	DeleteDC(m_hMemDC);	
	m_hMemDC = NULL;
}

// pixel format descriptor for printing opengl windows
const PIXELFORMATDESCRIPTOR PrintPFD =
{
	sizeof(PIXELFORMATDESCRIPTOR),					// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_BITMAP |						// Format Must Support Drawing to a window
		PFD_SUPPORT_OPENGL |						// Must support OpenGL,
		PFD_SUPPORT_GDI,
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

DIB::DIB(CWnd* window) {
	// get DIB of window
	CSize szDIB;
	HDC WindowHDC = window->GetDCEx(0, DCX_CACHE | DCX_WINDOW)->m_hDC;
	
	// width & height of window
	CRect windowRect;
	window->GetWindowRect(&windowRect);
	int Width = windowRect.Width();
	int Height = windowRect.Height();
	int left = windowRect.left;
	int top = windowRect.top;

	szDIB.SetSize(Width, Height);
	
	// Create DIB Section
	memset(&m_bmi, 0, sizeof(BITMAPINFO));
	m_bmi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_bmi.bmiHeader.biWidth			= szDIB.cx;
	m_bmi.bmiHeader.biHeight		= szDIB.cy;
	m_bmi.bmiHeader.biPlanes		= 1;
	m_bmi.bmiHeader.biBitCount		= 24;
	m_bmi.bmiHeader.biCompression	= BI_RGB;
	// round up to the nearest 4 bytes on the width because it word aligns
	m_bmi.bmiHeader.biSizeImage		= (szDIB.cx * 3 + ((4-(szDIB.cx*3 % 4))%4) ) * szDIB.cy;

	m_hDib = ::CreateDIBSection(WindowHDC, &m_bmi, DIB_RGB_COLORS, &m_pBitmapBits, NULL, (DWORD)0);

	// Create memory DC, and associate it with the DIB.
	m_hMemDC = ::CreateCompatibleDC(NULL);

	if (!m_hMemDC)
	{
		DeleteObject(m_hDib);
		m_hDib = NULL;
		return;
	}

	SelectObject(m_hMemDC, m_hDib);

	

	/*** set up the memory hdc to we can use opengl to draw to bitmap ***/

	HGLRC gl_hRC;
	const PIXELFORMATDESCRIPTOR* pfd = &PrintPFD;

	int	PixelFormat;
	// Did Windows Find A Matching Pixel Format?
	if (!(PixelFormat = ChoosePixelFormat(m_hMemDC, pfd)))
	{
		//MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	}
	
	// Are We Able To Set The Pixel Format?
	if(!SetPixelFormat(m_hMemDC,PixelFormat, pfd))
	{
		//MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	}
	
	// Are We Able To Get A Rendering Context?
 	if (!(gl_hRC = wglCreateContext(m_hMemDC)))
	{
		//MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	}
		
	// Try To Activate The Rendering Context
	if(!wglMakeCurrent(m_hMemDC, gl_hRC))
	{
		// We just made it, but can't use it. Kill the render context.
		wglDeleteContext(gl_hRC);
		//MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	}

	/*** ***/
	
	// Copy screen bits to memory context
	// only copy bits for the selected window
	::BitBlt(m_hMemDC, 0,0, Width, Height, WindowHDC, 0, 0, SRCCOPY);

	// tell window to render any opengl that it wants to
	window->SendMessage(WM_DRAWOPENGL, (WPARAM)m_hMemDC, (LPARAM)gl_hRC);
}

void DIB::GrabWindow(CWnd* window) {
	// get DIB of window
	
	// get handle for whole desktop

	CSize szDIB;
	HDC Screen_Hdl = GetDC( NULL ); //Handle to screen
	
	// width & height of window
	CRect windowRect;
	window->GetWindowRect(&windowRect);
	int Width = windowRect.Width();
	int Height = windowRect.Height();
	int left = windowRect.left;
	int top = windowRect.top;

	szDIB.SetSize(Width, Height);
	
	// Create DIB Section
	memset(&m_bmi, 0, sizeof(BITMAPINFO));
	m_bmi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_bmi.bmiHeader.biWidth			= szDIB.cx;
	m_bmi.bmiHeader.biHeight		= szDIB.cy;
	m_bmi.bmiHeader.biPlanes		= 1;
	m_bmi.bmiHeader.biBitCount		= 24;
	m_bmi.bmiHeader.biCompression	= BI_RGB;
	// round up to the nearest 4 bytes on the width because it word aligns
	m_bmi.bmiHeader.biSizeImage		= (szDIB.cx * 3 + ((4-(szDIB.cx*3 % 4))%4) ) * szDIB.cy;

	m_hDib = ::CreateDIBSection(Screen_Hdl, &m_bmi, DIB_RGB_COLORS, &m_pBitmapBits, NULL, (DWORD)0);

	// Create memory DC, and associate it with the DIB.
	m_hMemDC = ::CreateCompatibleDC(NULL);

	if (!m_hMemDC)
	{
		DeleteObject(m_hDib);
		m_hDib = NULL;
		return;
	}

	SelectObject(m_hMemDC, m_hDib);
	
	// Copy screen bits to memory context
	// only copy bits for the selected window
	::BitBlt(m_hMemDC, 0,0, Width, Height, Screen_Hdl, left, top, SRCCOPY);
}

void DIB::PutOnClipboard() {
	// give bitmap to clipboard

	// copy bitmap data
	// TODO: wild guess from gimp forums to add 1
	long lSize = m_bmi.bmiHeader.biSize + m_bmi.bmiHeader.biSizeImage + 1;
	TRACE("width %d, height %d\n", m_bmi.bmiHeader.biWidth, m_bmi.bmiHeader.biHeight);
	int bmi_size = sizeof(m_bmi);

	HANDLE lBMIHdl = GlobalAlloc( GMEM_DDESHARE | GMEM_MOVEABLE, lSize );

	if( lBMIHdl == NULL ) {
		AfxMessageBox( "CDibView::BitMapInfoHandle: Unable to allocate BITMAPINFO structure." );
	} else {
		LPVOID   lBMIPtr = (LPBITMAPINFO)( GlobalLock( lBMIHdl ) );
		if( lBMIPtr == NULL ) {
			AfxMessageBox( "CDibView::BitMapInfoHandle: Unable to lock BITMAPINFO handle." );
		} else {
			memcpy( lBMIPtr, &m_bmi, m_bmi.bmiHeader.biSize );
			memcpy( (char*)lBMIPtr + m_bmi.bmiHeader.biSize, this->m_pBitmapBits, m_bmi.bmiHeader.biSizeImage+1);
			GlobalUnlock( lBMIHdl ); 
		}
	}

	HWND owner = Get_App_Ptr()->GetMainFramePtr()->GetSafeHwnd();
	if(!::OpenClipboard(owner)) {
		TRACE("Failed to open clipboard\n");
	}
	if(!::EmptyClipboard()) {
		TRACE("Failed to empty clipboard\n");
	}
	if(!::SetClipboardData(CF_DIB, lBMIHdl)) {
		TRACE("Failed to set clipboard data\n");
	}
	if(!::CloseClipboard()) {
		TRACE("Failed to close clipboard\n");
	}
}

void DIB::RenderToDC(CDC* pDC) {

	// define page properties
	const float lMargin     =  1.0F; //in
	const float lPageHeight = 11.0F; //in
	const float lPageWidth  =  8.5F; //in

	// get device parameters
	int lHorizontalPixels = pDC->GetDeviceCaps( HORZRES );
	int lVerticalPixels   = pDC->GetDeviceCaps( VERTRES );

	// define target rectangle
	CRect   Target_Rect;
	Target_Rect.top     = ( lMargin / lPageHeight ) * lVerticalPixels;
	Target_Rect.left    = ( lMargin / lPageWidth  ) * lHorizontalPixels;

	Target_Rect.bottom  = ( ( lPageHeight - lMargin ) / lPageHeight ) * lVerticalPixels;
	Target_Rect.right   = ( ( lPageWidth  - lMargin ) / lPageWidth  ) * lHorizontalPixels;

	CSize   Dib_Size =  CSize(m_bmi.bmiHeader.biWidth, m_bmi.bmiHeader.biHeight);

	CSize    Adjusted_Rect_Size;
	CPoint   Start_Pixel( Target_Rect.left, Target_Rect.top );

	float Target_Width  = float((  Target_Rect.Width()  / pDC->GetDeviceCaps( LOGPIXELSX )  ));
	float Target_Height = float((  Target_Rect.Height() / pDC->GetDeviceCaps( LOGPIXELSY )  ));

	ASSERT( Target_Width  > 0 );
	ASSERT( Target_Height > 0 );

	float   Width_Ratio  = Target_Width/Dib_Size.cx;
	float   Height_Ratio = Target_Height/Dib_Size.cy;

	float   Dib_Aspect_Ratio = float(Dib_Size.cy) / float(Dib_Size.cx);

	if( Width_Ratio > Height_Ratio ) //Height is limiting factor, readjust width
	{
		Adjusted_Rect_Size.cx = (long)(  Target_Rect.Height() / Dib_Aspect_Ratio  );
		Start_Pixel.x += (int)(  0.5 * ( (float)Target_Rect.Width()  - Adjusted_Rect_Size.cx )  );
		Adjusted_Rect_Size.cy = Target_Rect.Height();
	}

	else //Width is limiting factor, readjust height.
	{
		Adjusted_Rect_Size.cx = Target_Rect.Width();
		Adjusted_Rect_Size.cy = (long)(  Target_Rect.Width() * Dib_Aspect_Ratio  );
		Start_Pixel.y += (int)(  0.5 * ( (float)Target_Rect.Height() - Adjusted_Rect_Size.cy )  );
	}

	::StretchDIBits(
			pDC->GetSafeHdc(), 
			Start_Pixel.x, Start_Pixel.y,  //Destination origin
			Adjusted_Rect_Size.cx, Adjusted_Rect_Size.cy,    //Destination size
			0, 0,                //Source origin
			m_bmi.bmiHeader.biWidth, m_bmi.bmiHeader.biHeight, //Source size
			m_pBitmapBits, &m_bmi,
			DIB_RGB_COLORS, SRCCOPY);
}