// ReportFrame.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h"	// Necessary
#include "ReportView.hpp"
#include "ReportFrame.h"
#include "main_frm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ReportFrame

IMPLEMENT_DYNCREATE(ReportFrame, CMDIChildWnd)

ReportFrame::ReportFrame()
:
mShowCmd( SW_SHOWNORMAL )
{
}

ReportFrame::~ReportFrame()
{
}


BEGIN_MESSAGE_MAP(ReportFrame, CMDIChildWnd)
   //{{AFX_MSG_MAP(ReportFrame)
   ON_WM_KILLFOCUS()
   ON_WM_SETFOCUS()
   //}}AFX_MSG_MAP
   ON_MESSAGE( WM_UPDATE_POSITION, OnUpdatePosition )
   ON_MESSAGE( WM_UPDATE_TITLE_BAR, OnUpdateTitleBar )
	ON_MESSAGE( WM_DRAWOPENGL, OnDrawOpenGL )

	ON_WM_NCRBUTTONUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ReportFrame message handlers

//---------------------------------------------------------------------------
   BOOL 
   ReportFrame::
PreCreateWindow
( CREATESTRUCT & aCS ) 
{
   int    lOK  =   this->CMDIChildWnd::PreCreateWindow( aCS );
   aCS.style &= ~(long)FWS_ADDTOTITLE;  //Set NO auto label per MS document Q99182
   return   lOK;
}

//---------------------------------------------------------------------------
   void 
   ReportFrame::
OnKillFocus
( CWnd* pNewWnd ) 
{
   CMDIChildWnd::OnKillFocus(pNewWnd);

   WINDOWPLACEMENT   lWindowPlacement;
   this->GetWindowPlacement( & lWindowPlacement );
   mShowCmd = lWindowPlacement.showCmd;

   return;
}

//---------------------------------------------------------------------------
   void 
   ReportFrame::
OnSetFocus
( CWnd* pOldWnd ) 
{
   WINDOWPLACEMENT   lWindowPlacement;
   this->GetWindowPlacement( & lWindowPlacement );
   lWindowPlacement.showCmd = mShowCmd;
   this->SetWindowPlacement( & lWindowPlacement );

   CMDIChildWnd::OnSetFocus(pOldWnd);

   return;	
}
/*-----------------------------------*/ LRESULT       //----------------------
ReportFrame ::					      OnUpdatePosition
(
WPARAM   wParam,
LPARAM   lParam
)
//User-defined message handler (message sent by parent main frame). Since there
//is no direct association between parent and child frames in the MFC scheme of
//things, one must send messages (kind of cold don't you think). If the 
//window is maximized or iconized, nothing is done and the default window
//behavior results are left in tact. If the window is not zoomed, the window
//size and position is computed depending upon the quadrant of the parent frame
//windows client area that it occupies.

//NOTE: This message handler is in addition to the re-routed message handler 
//OnSysCommand. The difference is when each is called. OnSysCommand is called
//whenever the user takes any action that results in a system command for the 
//window such as maximize, restore, resize, etc. either by system menu selection
//or window button activation. This function is necessary because the system does
//not issue system commands to child windows when the size of the containing
//window is modified. We must do it ourselves.

//NOTE: The new 32 MFC infrastructure may provide a more direct way of doing
//this over issuing user defined messages.
{

   this->Update_Title_Bar();
   
   return TRUE;
}

/*-----------------------------------*/ LRESULT       //----------------------
ReportFrame ::					      OnUpdateTitleBar
(
WPARAM   wParam,
LPARAM   lParam
)
//User-defined message handler (message sent by parent main frame). Since there
//is no direct association between parent and child frames in the MFC scheme of
//things, one must send messages (kind of cold don't you think). The associated
//view, on the other hand, calls Update_Title_Bar directly during OnInitialUpdate.
{
   this->Update_Title_Bar();
   return TRUE;
}

LRESULT ReportFrame::OnDrawOpenGL(WPARAM wParam, LPARAM lParam) {
	return this->GetActiveView()->SendMessage(WM_DRAWOPENGL, wParam, lParam);
}

   //----------------------------------------------------------------------------                                  
void ReportFrame::Update_Title_Bar()
{
   CString   lCaption;
   C_Hom_Doc*   lDocPtr = Get_App_Ptr()->Get_Document_Ptr();
      
//--------------------------------sets new icon to main frame---------------
   // Set big icon
   // TODO this used to depend on the task mode (static vs dynamic). When dynamic becomes available again,
   // we may need a new icon
   this->SetIcon(AfxGetApp()->LoadIcon(IDI_3DDSmall),true);
//----------------------------------------------------------------------------

 /* ACT - The window title for reports is currently only stored in the RESOURCE file!
  Therefore we can't load lCaption from anywhere, and just have to hope it doesn't
  get overwritten anywhere.  This function is only useful if the titles ever need to
  change, and need to be updated.  In this case, the value for lCaption can be grabbed
  from the proper member variable, and executing this next statement will actually be
  helpful.  As of now, all this next command does is erase title bars when the app is
  resized.  
  */

//     lCaption = ((ReportView*)this)->ReportName();
//     this->SetWindowText( (LPCTSTR)( lCaption ) );



   return;
}



void ReportFrame::OnNcRButtonUp(UINT nHitTest, CPoint point)
{

	if(nHitTest == HTCAPTION) {
		CMenu menu;
		menu.LoadMenu(IDR_QuadFrameContext);
		C_Hom_Doc* pDoc = ::Get_App_Ptr()->Get_Document_Ptr();
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}

	CMDIChildWnd::OnNcRButtonUp(nHitTest, point);
}
