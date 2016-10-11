#include "stdafx.h"

#include "cdib.h"
#include <windowsx.h>  // for GlobalAllocPtr

IMPLEMENT_SERIAL(CDib, CObject, 0)

#ifndef WIN32
	#define CHAR_HUGE_PTR char huge*
#else
	#define CHAR_HUGE_PTR char*
#endif

///////////////////////////////////////////////////////////////////
//-------------------------------- default constructor -----------------------
CDib :: CDib
( void )
   :
   m_dwLength( 0L   ),
   m_nBits   ( 0    ),
   m_lpBuf   ( NULL )
   { }
    
//-------------------------------- constructor -------------------------------
CDib::CDib(CWnd* WindowPtr)
//Constructs a DIB from a pointer to a Window object. If the pointer is NULL,
//a DIB for the entire desktop is created in keeping with the Windows GetDC
//convention.
{
   if( WindowPtr == NULL )
      {
      this->Desktop_DIB();
      }
      
   else
      {
      this->Window_DIB( WindowPtr );
      }
   
   return;      
}
    
/*------------------------------*/ void        //-----------------------------
CDib ::                                                            Desktop_DIB
( void )
   {
                                  //--- Create Screen DC ---
   HDC   Screen_Hdl = GetDC( NULL ); //Handle to screen
   ASSERT( Screen_Hdl != NULL );
   
	CDC   Screen_DC;
   Screen_DC.Attach( Screen_Hdl );

                                  //--- Create Screen Compatible Memory DC ---
   CDC*   Screen_Memory_DC_Ptr = new CDC;
   Screen_Memory_DC_Ptr->CreateCompatibleDC( &Screen_DC );                                  

                                  //--- Create Screen Compatible Bitmap ---
   int   Width  = Screen_DC.GetDeviceCaps( HORZRES );
   int   Height = Screen_DC.GetDeviceCaps( VERTRES );

   // TODO printing
   CBitmap*   Screen_Bitmap_Ptr = new CBitmap;
   Screen_Bitmap_Ptr->CreateCompatibleBitmap( &Screen_DC, Width, Height );

   
                                  //--- Select Bitmap into Memory DC ---
   CBitmap* Prior_BM_Ptr = Screen_Memory_DC_Ptr->SelectObject( Screen_Bitmap_Ptr );
                                  //Transfer screen bitmap to memory context
   Screen_Memory_DC_Ptr->BitBlt( 0, 0, Width, Height, &Screen_DC, 0, 0, SRCCOPY );  
   
                                  //--- Release the Screen DC handle ---
   Screen_DC.Detach();
   ReleaseDC( NULL, Screen_Hdl );                                  

                                  //--- Create the DIB ---
   this->Memory_DC_To_DIB( Screen_Memory_DC_Ptr, 0, FALSE );
   
   if( this->GetLength() == 0 )  
      AfxMessageBox( "Unable to create desktop DIB." );
      
                                  //--- Release other resources ---
   delete   Screen_Memory_DC_Ptr->SelectObject( Prior_BM_Ptr );      
   delete   Screen_Memory_DC_Ptr;
   
   return;

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

/*------------------------------*/ void        //-----------------------------
CDib ::                                                             Window_DIB
( 
CWnd*   WindowPtr 
)
   {
   ASSERT( WindowPtr != NULL );
                                  //--- Create Window DC ---
   HWND  Window_Hdl =  WindowPtr->GetSafeHwnd();
   ASSERT( Window_Hdl != NULL );                                     
   HDC   Window_DC_Hdl = GetWindowDC( Window_Hdl ); 
   ASSERT( Window_DC_Hdl != NULL );
   
   CDC   Window_DC;
   Window_DC.Attach( Window_DC_Hdl );

                                  //--- Create Screen Compatible Memory DC ---
   /*CDC*   Screen_Memory_DC_Ptr = new CDC;
   Screen_Memory_DC_Ptr->CreateCompatibleDC( &Window_DC );*/
   HDC ScreenHDC = ::CreateCompatibleDC(NULL);
   //CDC* Screen_Memory_DC_Ptr = new CDC;
   //Screen_Memory_DC_Ptr->Attach(ScreenHDC);

   
                                  //--- Create Screen Compatible Bitmap ---
   CRect   Window_Rect;
   WindowPtr->GetWindowRect( Window_Rect );
   
   int   Width  = Window_Rect.Width();
   int   Height = Window_Rect.Height();

   // TODO printing
   /*CBitmap*   Screen_Bitmap_Ptr = new CBitmap;
    Screen_Bitmap_Ptr->CreateCompatibleBitmap( &Window_DC, Width, Height );*/

   BITMAPINFO m_bmi;
   	// Create DIB Section
	memset(&m_bmi, 0, sizeof(BITMAPINFO));
	m_bmi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_bmi.bmiHeader.biWidth			= Width;
	m_bmi.bmiHeader.biHeight		= Height;
	m_bmi.bmiHeader.biPlanes		= 1;
	m_bmi.bmiHeader.biBitCount		= 24;
	m_bmi.bmiHeader.biCompression	= BI_RGB;
	m_bmi.bmiHeader.biSizeImage		= Width * Height * 3;
	LPVOID m_pBitmapBits;
   HBITMAP hbitmap = ::CreateDIBSection(ScreenHDC, &m_bmi, DIB_RGB_COLORS, &m_pBitmapBits, NULL, (DWORD)0);
   
                                  //--- Select Bitmap into Memory DC ---
   CBitmap* Prior_BM_Ptr = 0;//Screen_Memory_DC_Ptr->SelectObject( Screen_Bitmap_Ptr );
	//::SelectObject(ScreenHDC, Screen_Bitmap_Ptr->m_hObject);
   ::SelectObject(ScreenHDC, hbitmap);

   
   
	/*** TODO try to set up the memory hdc to we can use opengl to draw to bitmap ***/

    //HDC gl_hDC = Screen_Memory_DC_Ptr->m_hDC;
	HGLRC gl_hRC;
	const PIXELFORMATDESCRIPTOR* pfd = &PrintPFD;

	int	PixelFormat;
	// Did Windows Find A Matching Pixel Format?
	if (!(PixelFormat = ChoosePixelFormat(ScreenHDC, pfd)))
	{
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	}
	
	// Are We Able To Set The Pixel Format?
	if(!SetPixelFormat(ScreenHDC,PixelFormat, pfd))
	{
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	}
	
	// Are We Able To Get A Rendering Context?
	if (!(gl_hRC = wglCreateContext(ScreenHDC)))
	{
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	}
		
	// Try To Activate The Rendering Context
	if(!wglMakeCurrent(ScreenHDC, gl_hRC))
	{
		// We just made it, but can't use it. Kill the render context.
		wglDeleteContext(gl_hRC);
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	}

	/*** ***/

	
   //CBitmap* Screen_Bitmap_Ptr = new CBitmap;
   //Screen_Bitmap_Ptr->Attach(hbitmap);


                                  //Transfer screen bitmap to memory context
   //Screen_Memory_DC_Ptr->BitBlt( 0, 0, Width, Height, &Window_DC, 0, 0, SRCCOPY );
	::BitBlt(ScreenHDC, 0,0, Width, Height, Window_DC.m_hDC, 0,0, SRCCOPY);

	// TODO
	// render opengl stuff to memory context if the window has any
	// OMG how are we going to do this?

                                  //--- Release the Screen DC handle ---
   Window_DC.Detach();
   ReleaseDC( NULL, Window_DC_Hdl );

                                  //--- Create the DIB ---
   CDC* Screen_Memory_DC_Ptr = new CDC;
   Screen_Memory_DC_Ptr->Attach(ScreenHDC);
   this->Memory_DC_To_DIB( Screen_Memory_DC_Ptr, 0, FALSE );
   
   if( this->GetLength() == 0 )  
      AfxMessageBox( "Unable to create desktop DIB." );
      
                                  //--- Release other resources ---
   delete   Screen_Memory_DC_Ptr->SelectObject( Prior_BM_Ptr );      
   delete   Screen_Memory_DC_Ptr;
   
   return;
   }
    
//-------------------------------- constructor -------------------------------
CDib ::                                                                   CDib
(
CDC*   pDC,    // pDC is the memory DC ptr
int    nBt,    // nBt is color bits per pixel (default = 0)
BOOL   bCompr  // bCompr is compression (default = TRUE)
)
//Constructs a DIB from the contents of a GDI bitmap. The bitmap 
//must previously have been selected into the memory device context
//pointed to by pDC. Most of this function has to do with reconciling the
//differences of defining meta-data concerning the bitmap. Without the nBT and
//bCompr arguments, this could be a conversion constructor.
//Optionally compresses the GDI bitmap into the DIBitmap.
   {
   this->Memory_DC_To_DIB( pDC, nBt, bCompr );
   return;
   }
    
/*------------------------------*/ void        //-----------------------------
CDib ::                                                       Memory_DC_To_DIB
(
CDC*   pDC,    // pDC is the memory DC ptr
int    nBt,    // nBt is color bits per pixel (default = 0)
BOOL   bCompr  // bCompr is compression (default = TRUE)
)
//Constructs a DIB from the contents of a GDI bitmap which has been selected
//into the provided memory device context.
   {
    int    nPaletteSize;

//    ASSERT(!LookupPermanent(h, pObject))
    
    CBitmap* pEmptyBitmap = new CBitmap;
//  pEmptyBitmap->CreateCompatibleBitmap(pDC, 0, 0);
    pEmptyBitmap->CreateBitmap( 1, 1, 1, 1, NULL );
    
                                  //Select the empty bitmap into the
                                  //device context and at the same 
                                  //time get a pointer to the
                                  //bitmap already in the memory
                                  //device context
    CBitmap* MemGDIBitmapObj_Ptr = (CBitmap*)( pDC->SelectObject( pEmptyBitmap ) );
    BITMAP MemGDIBitmap;  //Bitmap data structure
    MemGDIBitmapObj_Ptr->GetObject( sizeof(MemGDIBitmap), &MemGDIBitmap ); //Get the BITMAP data structure
                                  
                                  //Determine total color bits per pixel
    switch( nBt )
       {                          //If zero: = Planes * BitsPerPlanePixel
       case  0: m_nBits = MemGDIBitmap.bmPlanes * MemGDIBitmap.bmBitsPixel; break;
       case  1:                   //All others set to passed value
       case  4: 
       case  8: 
       case 24: m_nBits = nBt; break;
       default: AfxMessageBox( "CDib::CDib: Unknown color bits per pixel." );
       }
                                  //Determine Palette Size
    switch( m_nBits )
       {
       case  1: nPaletteSize =   2; break; //Monochrome: fore and background
       case  4: nPaletteSize =  16; break;
       case  8: nPaletteSize = 256; break;
       default: nPaletteSize =   0; break; //No palette for 24 bits: 
                                             //  > On a 24 bit display board, 
                                             //    RGB colors are shown directly.
                                             //  > On smaller bit boards, Windows
                                             //    will match the 24 bit RGB to
                                             //    the closest standard Windows 
                                             //    color
       }

                                  //Fills out row to 4-byte boundary
    DWORD dw4Bytes = ((DWORD) MemGDIBitmap.bmWidth * m_nBits) / 32;
    if (   (  (DWORD)MemGDIBitmap.bmWidth * m_nBits  ) % 32   ) //If there is a remainder 
       {
       dw4Bytes ++;               //Increment number of 4-byte groups
       }
    DWORD BytesPerRow = 4 * dw4Bytes;
                                  
                                  //Compute memory needs for headers and attempt 
                                  //to allocate
    m_dwLength = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
                 sizeof(RGBQUAD) * nPaletteSize;
    if ( !AllocateMemory() ) 
       {
       AfxMessageBox( "CDib::CDib: Insufficient memory for DIB header." );
       return;
       }
       
                                 //Assign this object's data members
    m_lpBMIH->biSize      = sizeof(BITMAPINFOHEADER);
    m_lpBMIH->biWidth     = MemGDIBitmap.bmWidth;
    m_lpBMIH->biHeight    = MemGDIBitmap.bmHeight;
    m_lpBMIH->biPlanes    = 1;
    m_lpBMIH->biBitCount  = m_nBits; // 1, 4, 8, or 24
    
    if ( bCompr ) 
       {
       switch( m_nBits )
          {
          case  4: m_lpBMIH->biCompression = BI_RLE4; break;
          case  8: m_lpBMIH->biCompression = BI_RLE8; break;
          default: m_lpBMIH->biCompression = BI_RGB;  break; 
          }
       }
    else //No compression 
       {
       m_lpBMIH->biCompression = BI_RGB;
       }
    
    m_lpBMIH->biSizeImage     = 0;
    m_lpBMIH->biXPelsPerMeter = 0;
    m_lpBMIH->biYPelsPerMeter = 0;
    m_lpBMIH->biClrUsed       = 0;
    m_lpBMIH->biClrImportant  = 0;

                                  //OK, this is what we have been waiting for.
                                  //First call GetDIBits with null data pointer
                                  //to get size of DIB
    ::GetDIBits
       (
       pDC->GetSafeHdc(),         //Memory device context handle.
       (HBITMAP)MemGDIBitmapObj_Ptr->GetSafeHandle(), //GDI bitmap handle.
       0,                         //First scan line.
       (WORD) MemGDIBitmap.bmHeight, //Lines to be copied.
       NULL,                      //Pointer to memory location is NULL so ... 
       m_lpBMI,                   //load this bitmap info (not header) structure 
                                  //with data from the GDI bitmap.
       DIB_RGB_COLORS             //Color table is to contain literal RGB values
                                  //instead of pointers into the palette table.
       );

    if ( m_lpBMIH->biSizeImage == 0) //BI_RGB format in effect, no compression
       {
       m_dwLength += BytesPerRow * MemGDIBitmap.bmHeight;
       m_lpBMIH->biCompression = BI_RGB;
        // escape route for device drivers that don't do compression
       TRACE("Can't do compression\n");
       }
    else //Add actual compressed image size 
       {
       m_dwLength += m_lpBMIH->biSizeImage;
       } 
       
    if ( !AllocateMemory(TRUE) )  //Attempt to reallocate memory
       {
       AfxMessageBox( "CDib::CDib: Insufficient memory for entire DIBitmap." );
       return;
       }
                                  //Compute beginning of bits data memory
    m_lpData = (LPSTR) m_lpBMIH + 
                       sizeof(BITMAPINFOHEADER) +
                       sizeof(RGBQUAD) * nPaletteSize;
                       
                                  //Assign file header info
    m_lpBMFH->bfType      = 0x4d42; // 'BM'
    m_lpBMFH->bfSize      = m_dwLength;
    m_lpBMFH->bfReserved1 = 0;
    m_lpBMFH->bfReserved2 = 0;

    m_lpBMFH->bfOffBits   = (CHAR_HUGE_PTR) m_lpData - m_lpBuf;

	                              //All set and ready to get actual DIB bits.
                                  //Call GetDIBits again with pointer to 
                                  //memory location to
    if ( !::GetDIBits
               (
               pDC->GetSafeHdc(), 
               (HBITMAP)MemGDIBitmapObj_Ptr->GetSafeHandle(), 
               0, 
               (WORD) MemGDIBitmap.bmHeight, 
               m_lpData,          //Bits start here.
               m_lpBMI, 
               DIB_RGB_COLORS
               )
         ) 
         {
         AfxMessageBox( "CDib::CDib: DIBitmap copy bits failed." );
         m_dwLength = 0L; //Copy failed.
         }
    
    delete   pDC->SelectObject( MemGDIBitmapObj_Ptr ); // delete pEmptyBitmap
    
    return;
    }

///////////////////////////////////////////////////////////////////
CDib::~CDib()
{
    if (m_lpBuf) {
        GlobalFreePtr(m_lpBuf);  // free the DIB memory
    }
}

///////////////////////////////////////////////////////////////////
void CDib::Serialize(CArchive& ar)
{
    ar.Flush();
    if (ar.IsStoring()) {
      Write(ar.GetFile());
    }
    else {
      Read(ar.GetFile());
    }
}

///////////////////////////////////////////////////////////////////
/*------------------------------*/ BOOL    //---------------------------------
CDib ::                                                                   Read
(
CFile* pFile
)
   {
    // file assumed to be open
    ASSERT(m_dwLength == 0L); // DIB must be empty
    m_dwLength = pFile->GetLength();
    if (!AllocateMemory()) {
        return FALSE;       
    }
    DWORD dwCount = pFile->Read(m_lpBuf, m_dwLength);
    if (dwCount != m_dwLength) {
        AfxMessageBox("Read error");
        return FALSE;
    }
    if (m_lpBMFH->bfType != 0x4d42) {
        AfxMessageBox("Invalid bitmap file");
        return FALSE;
    }
    ASSERT((m_lpBMIH->biBitCount == 1) || (m_lpBMIH->biBitCount == 4) ||
           (m_lpBMIH->biBitCount == 8) || (m_lpBMIH->biBitCount == 24));
    m_lpData = (LPSTR) m_lpBMFH + m_lpBMFH->bfOffBits;
    m_nBits = m_lpBMIH->biBitCount;
    return TRUE;
}

///////////////////////////////////////////////////////////////////
BOOL CDib::Write(CFile* pFile)
{
//    TRY {
        pFile->Write(m_lpBuf, m_dwLength);
//    }
//    CATCH (CException, e) {
//        AfxMessageBox("Write error--possible disk full condition");
//        return FALSE;
//    }
//    END_CATCH
    return TRUE;
}

///////////////////////////////////////////////////////////////////
CBitmap* CDib::MakeBitmap(CDC* pDC, CSize& bmSize)
{
    // replaces the DC's existing bitmap with a new one from the DIB
    // returns the old one
    BITMAP bm;
    DWORD  dwFore, dwBack;
    // checks to see whether DIB buffer is properly loaded
    if (m_dwLength == 0L) {
        bmSize.cx = bmSize.cy = 0;
        return NULL;
    }

    // this code conditions the DC for mono or color
    int nPlanes = pDC->GetDeviceCaps(PLANES);
    int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
    CBitmap* pConfigBitmap = new CBitmap;
    char bits[100];
    if (m_lpBMIH->biBitCount == 1) {
        pConfigBitmap->CreateBitmap(1, 1, 1, 1, bits);
    }
    else {
        pConfigBitmap->CreateBitmap(1, 1, nPlanes, nBitsPixel, bits);
    }
    CBitmap* pOriginalBitmap =
        (CBitmap*) pDC->SelectObject(pConfigBitmap);

// CreateDIBitmap "switches bits" for mono bitmaps, depending on colors,
//  so we'll fool it
    if (GetMonoColors(dwFore, dwBack)) {
        SetMonoColors(0L, 0xFFFFFFL);
    }
    
#ifdef _WIN32
    HBITMAP hBitmap = ::CreateDIBitmap(pDC->GetSafeHdc(), m_lpBMIH,
            CBM_INIT, (CONST BYTE*) (m_lpBuf + m_lpBMFH->bfOffBits),
            m_lpBMI, DIB_RGB_COLORS);
#else
    HBITMAP hBitmap = ::CreateDIBitmap(pDC->GetSafeHdc(), m_lpBMIH,
            CBM_INIT, (LPSTR) (m_lpBuf + m_lpBMFH->bfOffBits),
            m_lpBMI, DIB_RGB_COLORS);
#endif
    if (hBitmap == NULL) {
        TRACE("null bitmap\n");  
        delete pDC->SelectObject(pOriginalBitmap); // delete config bitmap
        return NULL; // untested error logic
    }
    
    SetMonoColors(dwFore, dwBack);
    
    // Can't use CBitmap::FromHandle here because we need to
    //  delete the object later
    CBitmap* pBitmap = new CBitmap;
    pBitmap->Attach(hBitmap);
    pBitmap->GetObject(sizeof(bm), &bm);
    bmSize.cx = bm.bmWidth;
    bmSize.cy = bm.bmHeight;
    delete pDC->SelectObject(pBitmap); // delete configuration bitmap
    return pOriginalBitmap;
}

///////////////////////////////////////////////////////////////////
BOOL CDib::Display(CDC* pDC, CPoint origin)
{
    // direct to device--bypass the GDI bitmap
    if (!m_lpBuf) {
        return FALSE; // nothing to display
    }
    if (!::SetDIBitsToDevice(pDC->GetSafeHdc(), origin.x, origin.y,
       (WORD) m_lpBMIH->biWidth, (WORD) m_lpBMIH->biHeight, 0, 0, 0,
       (WORD) m_lpBMIH->biHeight, m_lpData, m_lpBMI,
       DIB_RGB_COLORS)) {
        return FALSE;
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////
BOOL CDib::Stretch(CDC* pDC, CPoint origin, CSize size)
   {
    // direct to device--bypass the GDI bitmap
    if (!m_lpBuf) 
       {
       return FALSE; // nothing to display
       }
       
    if (!::StretchDIBits
              (
              pDC->GetSafeHdc(), 
              origin.x, origin.y,  //Destination origin
              size.cx, size.cy,    //Destination size
              0, 0,                //Source origin
              (WORD)m_lpBMIH->biWidth,(WORD)m_lpBMIH->biHeight, //Source size
              m_lpData, m_lpBMI,
              DIB_RGB_COLORS, SRCCOPY)
              ) 
              { return FALSE; }
    
    return TRUE;
    }

///////////////////////////////////////////////////////////////////
BOOL CDib::Is_Empty()
   {
   BOOL   Empty = FALSE;
   
   if( this->m_lpBuf == NULL )
      {
      ASSERT( this->m_dwLength == 0 );
      Empty = TRUE;
      }
   
   return Empty;
   }

///////////////////////////////////////////////////////////////////
int CDib::GetColorBits()
{
    return m_nBits;
}

///////////////////////////////////////////////////////////////////
DWORD CDib::GetLength()
{
    return m_dwLength;
}

CSize CDib::GetSize()
{
    return CSize(int (m_lpBMIH->biWidth), int (m_lpBMIH->biHeight));
}

///////////////////////////////////////////////////////////////////
void CDib::SetMonoColors(DWORD dwForeground, DWORD dwBackground)
{
    if (m_nBits != 1) {
        return;
    }
    unsigned long far* pPalette = (unsigned long far*)
                       ((LPSTR) m_lpBMIH + sizeof(BITMAPINFOHEADER));
    *pPalette = dwForeground;
    *(++pPalette) = dwBackground;
    return;
}

///////////////////////////////////////////////////////////////////
BOOL CDib::GetMonoColors(DWORD& dwForeground, DWORD& dwBackground)
{
    if (m_nBits != 1) {
        return FALSE;
    }
    unsigned long far* pPalette = (unsigned long far*)
                       ((LPSTR) m_lpBMIH + sizeof(BITMAPINFOHEADER));
    dwForeground = *pPalette;
    dwBackground = *(++pPalette);
    return TRUE;
}

///////////////////////////////////////////////////////////////////
BOOL CDib::AllocateMemory(BOOL bRealloc) // bRealloc default = FALSE
{
    if (bRealloc)
    {  
        m_lpBuf = (CHAR_HUGE_PTR) GlobalReAllocPtr(m_lpBuf,
                                                m_dwLength, GHND);
    }
    else 
    {
        m_lpBuf = (CHAR_HUGE_PTR) GlobalAllocPtr(GHND, m_dwLength);
    }
    
    if (!m_lpBuf)
    {
        AfxMessageBox("Unable to allocate DIB memory");
        m_dwLength = 0L;
        m_nBits = 0;
        return FALSE;
    }
    
    m_lpBMFH = (LPBITMAPFILEHEADER) m_lpBuf;
    m_lpBMIH = (LPBITMAPINFOHEADER) (m_lpBuf + sizeof(BITMAPFILEHEADER));
    m_lpBMI = (LPBITMAPINFO) m_lpBMIH;
    
    return TRUE;
}

//---------------------------------------------------------------------------
   HANDLE
   CDib::
BitMapInfoHdl
( void )
//Allocates memory for copying the contents of this DIB (except for the 
//file header data) to a BITMAPINFO structure followed by the bits. Memory
//is not freed by this object. This member function is intended to be used
//only with the clipboard which takes ownership of the memory. Returns a
//HANDLE to the created memory object.
{
   long	 lSize = m_dwLength - sizeof( BITMAPFILEHEADER );

   HANDLE   lBMIHdl = GlobalAlloc( GMEM_DDESHARE | GMEM_MOVEABLE, lSize );

   if( lBMIHdl == NULL )
   {
      AfxMessageBox( "CDibView::BitMapInfoHandle: Unable to allocate BITMAPINFO structure." );
   }

   else
   {
      LPVOID   lBMIPtr = (LPBITMAPINFO)( GlobalLock( lBMIHdl ) );
      if( lBMIPtr == NULL )
      {
	 AfxMessageBox( "CDibView::BitMapInfoHandle: Unable to lock BITMAPINFO handle." );
      }
      else
      {
	 memcpy( lBMIPtr, m_lpBMIH, lSize );
	 GlobalUnlock( lBMIHdl ); 
      }
   }

   return   lBMIHdl;
}

/*-------------------------------*/ void   //---------------------------------
CDib ::                                                    Best_Fit_To_Printer
(
CDC*   PrinterDCPtr
)
   {
   ASSERT_VALID(  PrinterDCPtr  );

   const float lMargin     =  1.0F; //in
   const float lPageHeight = 11.0F; //in
   const float lPageWidth  =  8.5F; //in

   int   lHorizontalPixels = PrinterDCPtr->GetDeviceCaps( HORZRES );
   int   lVerticalPixels   = PrinterDCPtr->GetDeviceCaps( VERTRES );

   CRect   Target_Rect;
    
   Target_Rect.top     = ( lMargin / lPageHeight ) * lVerticalPixels;
   Target_Rect.left    = ( lMargin / lPageWidth  ) * lHorizontalPixels;

   Target_Rect.bottom  = ( ( lPageHeight - lMargin ) / lPageHeight ) * lVerticalPixels;
   Target_Rect.right   = ( ( lPageWidth  - lMargin ) / lPageWidth  ) * lHorizontalPixels;
   
   this->Best_Fit_To_DC_Rect( PrinterDCPtr, Target_Rect );
   
   return;
   }

/*-------------------------------*/ void   //---------------------------------
CDib ::                                                     Best_Fit_To_Window
(
CDC*    DCPtr ,
CWnd*   WindowPtr 
)
   {
   ASSERT_VALID(  DCPtr  );
   ASSERT_VALID(  WindowPtr  );

   CRect   Target_Rect;
   WindowPtr->GetClientRect( Target_Rect );
   
   this->Best_Fit_To_DC_Rect( DCPtr, Target_Rect );
   
   return;
   }   

/*-------------------------------*/ void   //---------------------------------
CDib ::                                                    Best_Fit_To_DC_Rect
(
      CDC*     DCPtr ,
const CRect&   PixelRect
)
   {
   ASSERT_VALID( DCPtr );
   ASSERT( PixelRect.Width() > 0 );
   ASSERT( PixelRect.Height() > 0 );
   
   ASSERT( ! this->Is_Empty() );

   CSize   Dib_Size =  this->GetSize();
    
   CSize    Adjusted_Rect_Size;
   CPoint   Start_Pixel( PixelRect.left, PixelRect.top );

   float Target_Width  = float((  PixelRect.Width()  / DCPtr->GetDeviceCaps( LOGPIXELSX )  ));
   float Target_Height = float((  PixelRect.Height() / DCPtr->GetDeviceCaps( LOGPIXELSY )  ));
   																						   
   ASSERT( Target_Width  > 0 );
   ASSERT( Target_Height > 0 );

   float   Width_Ratio  = Target_Width/Dib_Size.cx;
   float   Height_Ratio = Target_Height/Dib_Size.cy;

   float   Dib_Aspect_Ratio = float(Dib_Size.cy) / float(Dib_Size.cx);

   if( Width_Ratio > Height_Ratio ) //Height is limiting factor, readjust width
   {
      Adjusted_Rect_Size.cx = (long)(  PixelRect.Height() / Dib_Aspect_Ratio  );
      Start_Pixel.x += (int)(  0.5 * ( (float)PixelRect.Width()  - Adjusted_Rect_Size.cx )  );
      Adjusted_Rect_Size.cy = PixelRect.Height();
   }

   else //Width is limiting factor, readjust height.
   {
      Adjusted_Rect_Size.cx = PixelRect.Width();
      Adjusted_Rect_Size.cy = (long)(  PixelRect.Width() * Dib_Aspect_Ratio  );
      Start_Pixel.y += (int)(  0.5 * ( (float)PixelRect.Height() - Adjusted_Rect_Size.cy )  );
   }

   //Center
   this->Stretch( DCPtr, Start_Pixel, Adjusted_Rect_Size );
   
   return;
   }
