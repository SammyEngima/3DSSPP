// ReportView.cpp : implementation file
//
//#using <mscorlib.dll>

#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h"
#include "ReportView.hpp"
#include "GLView.h"
#include "analysis.h"
//#include "C:\program files\microsoft visual studio\vc98\mfc\src\auxdata.h"

#include "main_frm.h"
#include ".\reportview.hpp"
#include "CDIB.H"
#include "DIB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(ReportView, CFormView)

/////////////////////////////////////////////////////////////////////////////
// ReportView

//-------------------------------------------------------------------
ReportView::ReportView() : CFormView(1) {}
ReportView::ReportView(UINT aDialogResourceId, BOOL aInitializeFrame)
   : CFormView(aDialogResourceId),
   mDibViewPtr(NULL),
   mPrintDirect(TRUE),
   mInitializeFrame(aInitializeFrame),
   gl_hDC(0)
{
	printDib = 0;
	mPrintMultiple = false;
   return;
}

//---------------------------------------------------------------------------
ReportView::~ReportView()
{
   if(mDibViewPtr != NULL)
      mDibViewPtr->GetParent()->PostMessage(WM_CLOSE);
   delete printDib;
}

void ReportView::initializeGL(){
	// Make sure that there is only one context for the entire program.
    // Windows 98 breaks when using more than 4 contexts.  
    // One context should be all that is necessary for the entire program
    /*GLuint PixelFormat;
    if (!(PixelFormat=ChoosePixelFormat(gl_hDC,&pfd)))
    {
        ::MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return;
    }

    if(!SetPixelFormat(gl_hDC,PixelFormat,&pfd))
    {
        ::MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return;
    }

	if(!CGLView::openglContext)
    {
		if(!(CGLView::openglContext=wglCreateContext(gl_hDC)))
        {
            ::MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
            return;
        }
    }*/
}

//---------------------------------------------------------------------------
CString ReportView::ReportName() const
{
   return "Default";
}

//---------------------------------------------------------------------------  

void ReportView::Close()
{
   this->GetParent()->PostMessage(WM_CLOSE);
}

//---------------------------------------------------------------------------
void ReportView::OnInitialUpdate()
{
   if (mInitializeFrame)
   {
      // Shrink report if necessary, then center
	   this->GetParentFrame()->RecalcLayout();
      this->ResizeParentToFit(false);
      this->GetParentFrame()->CenterWindow();

      // Put report's name in the title bar
      this->GetParentFrame()->SetWindowText( this->ReportName() );

      // Make it un-minimizable, un-maximizable
      this->GetParentFrame()->ModifyStyle(WS_MINIMIZEBOX, NULL);
      this->GetParentFrame()->ModifyStyle(WS_MAXIMIZEBOX, NULL);

      // Scrollbars
      //this->GetParentFrame()->ModifyStyle(NULL, WS_HSCROLL | WS_VSCROLL);

      // Set the icon to 2D or 3D
	  // TODO this used to depend on task mode
	  this->GetParentFrame()->SetIcon(AfxGetApp()->LoadIcon(IDI_3DDSmall),true);
   }

   this->CFormView::OnInitialUpdate();
}

int ReportView::OnCreate(LPCREATESTRUCT lpcs) {

	if (CFormView::OnCreate(lpcs) == -1)
        return -1;
    return 0;
}


/////////////////////////////////////////////////////////////////////////////
// ReportView overrides

/////////////////////////////////////////////////////////////////////////////
// C_Hom_View printing

//---------------------------------------------------------------------------
void ReportView::PrintDirect()
{
   mPrintDirect = TRUE;
   //Wait(this,TimeSpan(100),false);
//   Sleep(400);

   // TODO printing
   //OnPaint();
   //OnDraw(this->GetDC());
   Invalidate();
   this->BringWindowToTop();
   UpdateWindow();
   mPrintMultiple = true;
   this->OnFilePrint();
   return;
}

//---------------------------------------------------------------------------
BOOL ReportView::OnPreparePrinting(CPrintInfo* pInfo)
{
	if(pInfo->m_bPreview) {
		return this->DoPreparePrinting(pInfo);
	}
	
	//if(!mPrintMultiple) {

		pInfo->m_bDirect = this->mPrintDirect;

		this->GetParentFrame()->SetActiveWindow();
		this->GetParentFrame()->BringWindowToTop();
		//this->SetActiveWindow();
		//this->BringWindowToTop();
		return this->DoPreparePrinting(pInfo);
	//}


	// printing from print multiple reports


   C_Main_Frame* lMainFramePtr = MainFramePtr();

   if (mDibViewPtr == NULL) {
		mDibViewPtr = lMainFramePtr->Create_Dib_Window();
   }

   CWnd* lFramePtr = this->GetParent();
   ASSERT( lFramePtr != NULL );

   //--- Save existing placement ---
   
   WINDOWPLACEMENT   lSavePlacement;
   lFramePtr->GetWindowPlacement( & lSavePlacement );

   //--- Compute and set print placement ---

   /*lFramePtr->ShowWindow( SW_RESTORE );
   this->ResizeParentToFit( FALSE );*/

   WINDOWPLACEMENT   lPrintPlacement;
   lFramePtr->GetWindowPlacement( & lPrintPlacement );

 // JTK - Commented out to fix print preview issue  
   /*lPrintPlacement.rcNormalPosition.right  -= lPrintPlacement.rcNormalPosition.left;
   lPrintPlacement.rcNormalPosition.bottom -= lPrintPlacement.rcNormalPosition.top ;

   lPrintPlacement.rcNormalPosition.left  = 0;
   lPrintPlacement.rcNormalPosition.top   = 0;
   lFramePtr->MoveWindow(&lPrintPlacement.rcNormalPosition);
   lFramePtr->SetWindowPlacement( & lPrintPlacement );*/
   
   //--- Create the DIB ---
   lFramePtr->SetActiveWindow();
   lFramePtr->BringWindowToTop();
   lFramePtr->UpdateWindow();
   mDibViewPtr->LoadDib(lFramePtr);
   
   //--- Call normal printing ---
   
   pInfo->m_bDirect = mPrintDirect;
   BOOL lPrintResult = mDibViewPtr->OnPreparePrinting( pInfo );  
   
   //--- Restore the window ---

   lFramePtr->SetWindowPlacement( & lSavePlacement );
   lFramePtr->ShowWindow( lSavePlacement.showCmd );
   lFramePtr->UpdateWindow();
   
   return lPrintResult;  
}

//---------------------------------------------------------------------------
void ReportView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
   return;
}

//---------------------------------------------------------------------------
void ReportView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

//---------------------------------------------------------------------------
void ReportView::OnPrint( CDC* pDC, CPrintInfo* pInf )
{
	//if(!mPrintMultiple) {
		if(printDib) {
			printDib->RenderToDC(pDC);
		}
	/*} else {
		if (mDibViewPtr != NULL)
			mDibViewPtr->OnDraw( pDC );
	}*/
}

void SSPPPrintPreview(CView* viewPtr)
{
   // In derived classes, implement special window handling here
   // Be sure to Unhook Frame Window close if hooked.

   // Must not create this on the frame.  Must outlive this function.
   CPrintPreviewState* pState = new CPrintPreviewState;

   // DoPrintPreview's return value does not necessarily indicate that
   // Print preview succeeded or failed, but rather what actions are necessary
   // at this point.  If DoPrintPreview returns TRUE, it means that
   // OnEndPrintPreview will be (or has already been) called and the
   // pState structure will be/has been deleted.
   // If DoPrintPreview returns FALSE, it means that OnEndPrintPreview
   // WILL NOT be called and that cleanup, including deleting pState
   // must be done here.

   if (!viewPtr->DoPrintPreview(IDD_SSPP_PREVIEW_TOOLBAR, viewPtr,
					     RUNTIME_CLASS(CPreviewView), pState))
   {
      // In derived classes, reverse special window handling here for
      // Preview failure case

      TRACE(traceAppMsg, 0, "Error: DoPrintPreview failed.\n");
      AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
      delete pState;      // preview failed to initialize, delete State now
   }
}


BEGIN_MESSAGE_MAP(ReportView, CFormView)
   ON_WM_WINDOWPOSCHANGED()
   ON_WM_CREATE()
   ON_COMMAND(ID_FILE_COPYACTIVEWINDOW, &ReportView::OnFileCopyactivewindow)
   ON_COMMAND(ID_FILE_PRINT_PREVIEW, &ReportView::OnFilePrintPreview)
   ON_COMMAND(ID_FILE_PRINT, &ReportView::OnFilePrint)
END_MESSAGE_MAP()


void ReportView::OnFileCopyactivewindow()
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

void ReportView::OnFilePrintPreview()
{
	if(printDib) {
		delete printDib;
		printDib = 0;
	}
	printDib = new DIB(this->GetParentFrame());
	CFormView::OnFilePrintPreview();
}

void ReportView::OnFilePrint()
{
	if(printDib) {
		delete printDib;
		printDib = 0;
	}
	printDib = new DIB(this->GetParentFrame());
	CFormView::OnFilePrint();
}
