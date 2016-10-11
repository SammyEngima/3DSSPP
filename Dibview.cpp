// dibview.cpp : implementation file
//

#include "stdafx.h"
#include "dibview.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDibView

IMPLEMENT_DYNCREATE(CDibView, CView)

const PIXELFORMATDESCRIPTOR PrintPFD =
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
};
CDibView::CDibView()
{
   this->Dib_Ptr = new CDib(); //Create default empty DIB
   return;
}

CDibView::~CDibView()
   {
   if( this->Dib_Ptr != NULL )
      {
      delete this->Dib_Ptr;
      this->Dib_Ptr = NULL;
      }

   return;   
   }


BEGIN_MESSAGE_MAP(CDibView, CView)
	//{{AFX_MSG_MAP(CDibView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP 
	
	//User defined Messages
        ON_MESSAGE( WM_PRINT_APPLICATION, OnPrintApplication ) 	                          
        ON_MESSAGE( WM_PRINT_DESKTOP,     OnPrintDesktop     ) 	                          
END_MESSAGE_MAP()


//----------------------------------------------------------------------------
BOOL CDibView :: OnPreparePrinting
( CPrintInfo* pInfo )
{
   pInfo->SetMaxPage( 1 );
   
   pInfo->m_bDirect = TRUE;

   return DoPreparePrinting( pInfo );
}  
   

/////////////////////////////////////////////////////////////////////////////
// CDibView drawing

void CDibView::OnDraw(CDC* pDC)
//Draws the DIB object contained by this view
   {
   
   if( this->Dib_Ptr->Is_Empty() ) return; //No DIB. Return without doing anything.

   if( pDC->IsPrinting() )
      {
      this->Dib_Ptr->Best_Fit_To_Printer( pDC );
      }
   
   else
      {
		/*
	   pDC->GetBkColor();
	   pDC->SetBkColor( RGB(255,255,255) );*/
      this->Dib_Ptr->Best_Fit_To_Window( pDC, this );
	  }   
   
   return;
   }

/////////////////////////////////////////////////////////////////////////////
// CDibView message handlers

//////////////////////////////////////////////////////////////////////////////
// User defined message handlers

//----------------------------------------------------------------------------
   void
   CDibView::
LoadDib
( CWnd * aWndPtr )
{
   ASSERT( this->Dib_Ptr != NULL );

   delete this->Dib_Ptr;

   this->Dib_Ptr = new CDib( aWndPtr );

   return;
}

//----------------------------------------------------------------------------
   void
   CDibView::
PrintDib
( void )
{   
   this->BeginWaitCursor();

   this->InvalidateRect( NULL ); //View contents have changed; invalidate client area

   this->CView::OnFilePrint();

   this->GetParent()->PostMessage( WM_CLOSE );

   this->EndWaitCursor();
   
   return;
}

/*-----------------------------------*/ LRESULT       //----------------------
CDibView ::					            OnPrintApplication
(
   WPARAM   wParam,
   LPARAM   lParam
)
{
   this->LoadDib( AfxGetApp()->m_pMainWnd ); //The application main frame
   this->PrintDib();
   return TRUE;
}

/*-----------------------------------*/ LRESULT       //----------------------
CDibView ::					                OnPrintDesktop
(
   WPARAM   wParam,
   LPARAM   lParam
)
{
   this->LoadDib( NULL ); //NULL refers to the desktop.
   this->PrintDib();
   return TRUE;
}
   
//---------------------------------------------------------------------------
   void
   CDibView::
CopyDIBToClipboard
( void )
{
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
   HANDLE   lBitMapInfoHdl = this->Dib_Ptr->BitMapInfoHdl();

   if( lBitMapInfoHdl != NULL )
   {
      //Give to the clipboard
      if ( ::SetClipboardData( CF_DIB, lBitMapInfoHdl ) == NULL )
      {
	 AfxMessageBox( "DibView::CopyDIBToClipboard: Unable to set Clipboard data" );
      }
   }

   CloseClipboard();

   return;
}

//---------------------------------------------------------------------------
   void
   CDibView::
CopyApplicationToClipboard
( void )
{
   this->BeginWaitCursor();
                                  //--- Replace empty DIB with a new DIB
                                  //    created from the entire screen
                                  //    (i.e., NULL)
   ASSERT( this->Dib_Ptr != NULL );
   delete this->Dib_Ptr;

   this->Dib_Ptr = new CDib( AfxGetApp()->m_pMainWnd );

   this->CopyDIBToClipboard();

   this->EndWaitCursor();

   return;
}

//---------------------------------------------------------------------------
   void
   CDibView::
CopyDesktopToClipboard
( void )
{
   this->BeginWaitCursor();
                                  //--- Replace empty DIB with a new DIB
                                  //    created from the entire screen
                                  //    (i.e., NULL)
   ASSERT( this->Dib_Ptr != NULL );
   delete this->Dib_Ptr;
   
   CWnd*   Window_Ptr = NULL;    //For now, use window ptr to resolve ambiguity
                                  //with other window constructors
   
   this->Dib_Ptr = new CDib( Window_Ptr );

   this->CopyDIBToClipboard();

   this->EndWaitCursor();

   return;
}

void CDibView::OnInitialUpdate()
{
   CView::OnInitialUpdate();
}
