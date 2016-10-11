// QUAD_FRM.CPP : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h"	// Necessary
#include "quad_frm.h"
#include "main_frm.h"

#include "WindowData.hpp"
#include ".\quad_frm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Quadrant_Frame

IMPLEMENT_DYNCREATE(Quadrant_Frame, CMDIChildWnd)

Quadrant_Frame::Quadrant_Frame()
{
}

Quadrant_Frame::~Quadrant_Frame()
{
}


BEGIN_MESSAGE_MAP(Quadrant_Frame, CMDIChildWnd)
	//{{AFX_MSG_MAP(Quadrant_Frame)
	ON_WM_CREATE()
//	ON_WM_SETFOCUS()
//	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP

	   //Re-routed messages
	   ON_WM_SYSCOMMAND()

	//User Defined Message
        ON_MESSAGE( WM_UPDATE_POSITION, OnUpdatePosition ) 	                          
        ON_MESSAGE( WM_UPDATE_TITLE_BAR, OnUpdateTitleBar ) 	                          

		ON_MESSAGE( WM_DRAWOPENGL, OnDrawOpenGL )

		  ON_WM_WINDOWPOSCHANGED()
                  ON_WM_SIZE()
				  ON_WM_CONTEXTMENU()
				  ON_WM_NCRBUTTONUP()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Quadrant_Frame message handlers

/*-----------------------------------*///----------------------
BOOL Quadrant_Frame::PreCreateWindow(CREATESTRUCT & CS)
{
   int OK = this->CMDIChildWnd::PreCreateWindow( CS );
   WindowData* lWindowDataPtr = Get_App_Ptr()->mNewWindowDataPtr;

   CS.style &= ~(long)FWS_ADDTOTITLE;  //Set NO auto label per MS document Q99182

	lWindowDataPtr->mFramePtr = this;
   
   this->Quadrant	= lWindowDataPtr->mFrameType;
   mViewType		= lWindowDataPtr->mViewType;

   if(mViewType == VT_ANIMATION)
   {
	   int lskdfj = 0;
   }

	// --- Set window dimensions and placement ---

   if( CS.cy < 0 ) //Indicates set to default which indicates the 2nd pass.
   {
		// --- Set window caption ---
      this->Caption = Get_App_Ptr()->mNewWindowDataPtr->mCaption.c_str();   
      CS.lpszName = (const char*)( this->Caption );

		// --- Set window placement
      if( lWindowDataPtr->mComputePosition )
      {
         this->Compute_Placement
            (
            CS.x  ,   //LeftEdge
            CS.y  ,   //TopEdge
            CS.cx ,   //Width
            CS.cy     //Height
            );
      }
      else //get placement from window data
      {
         RECT &  lRect = lWindowDataPtr->mRectangle;
         CS.x =  lRect.left;
         CS.y =  lRect.top;
         CS.cx = lRect.right  - lRect.left;
         CS.cy = lRect.bottom - lRect.top;
         assert( CS.cx > 0 );
         assert( CS.cy > 0 );
      }
   } //End CS.cy < 0

   return   OK;   
}

//----------------------------------------------------------------------------
void Quadrant_Frame::OnSysCommand( UINT nID, LPARAM lParam )
//The framework calls this member function when the user selects a command from 
//the Control menu, or when the user selects the Maximize or the Minimize button. 
//Intercept sizing commands and compute quadrant frame size and location if
//the sizing command is SC_RESTORE.  In the case of a move, it redraws
//the window so the joints can be selected.
{
   if ((0xFFF0 & nID) == SC_RESTORE)
   {
      //	Do typical processing
		this->CWnd::OnSysCommand( nID, lParam ); 
		
		//	Now do custom sizing and placement.
		//		Only restore to autotile position if autotile
		if(Get_App_Ptr()->Get_Document_Ptr()->AutoTile())
		{
			int Left_Edge, Top_Edge, Width, Height;

			this->Compute_Placement( Left_Edge, Top_Edge, Width, Height );
			this->MoveWindow( Left_Edge, Top_Edge, Width, Height );
		}
	}
	else if ((0xFFF0 & nID) == SC_MOVE)
	{
		this->CWnd::OnSysCommand( nID, lParam );	//Do typical processing
		this->RedrawWindow();	// JTK - Redraws the window so that points can be selected
   } else if ((0xFFF0 & nID) == SC_CLOSE && mViewType == VT_ANIMATION) {
	   // if closing the animation window, actually just hide it
	   this->ShowWindow(SW_HIDE);
   }
   else //process normally
   {
      this->CWnd::OnSysCommand( nID, lParam );
   }  
   
   return;
}

/*-----------------------------------*/ //----------------------
LRESULT Quadrant_Frame::OnUpdatePosition(WPARAM wParam, LPARAM lParam)
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
	// This function computes window sizes to fit in the main frame -
	// Only called on a tilenow, I believe

   int Left_Edge, Top_Edge, Width, Height;
   this->Compute_Placement( Left_Edge, Top_Edge, Width, Height );

   //this->Update_Title_Bar();

	if(!this->IsZoomed())
	{
	   this->MoveWindow( Left_Edge, Top_Edge, Width, Height );
	}
	// else we're maximized, so just leave the window where it's at.
	//		Otherwise we get strange results.

	return TRUE;
}

/*----------------------------------------------------------------------*/
LRESULT Quadrant_Frame::OnUpdateTitleBar(WPARAM wParam, LPARAM lParam)
//User-defined message handler (message sent by parent main frame). Since there
//is no direct association between parent and child frames in the MFC scheme of
//things, one must send messages (kind of cold don't you think). The associated
//view, on the other hand, calls Update_Title_Bar directly during OnInitialUpdate.
{
   this->Update_Title_Bar();
   return TRUE;
}


afx_msg LRESULT Quadrant_Frame::OnDrawOpenGL( WPARAM wParam, LPARAM lParam ) {
	return this->GetActiveView()->SendMessage(WM_DRAWOPENGL, wParam, lParam);
}

//---------------------------------------------------------------------------
//User-defined message handler (sent by parent main frame)
void Quadrant_Frame::Compute_Placement(int& RetLeftEdge, //x
                                       int& RetTopEdge,  //y
                                       int& RetWidth,    //cx
                                       int& RetHeight)   //cy
{
    C_Main_Frame* lMainWndPtr = MainFramePtr();

    ASSERT_VALID( (CWnd*)lMainWndPtr );

    int lMainWidth  = lMainWndPtr->GetClientWidth();
    int lMainHeight = lMainWndPtr->GetClientHeight();

	// Have to hide animation bar if in static mode
	C_Hom_Doc* lDocPtr = Get_App_Ptr()->Get_Document_Ptr();
	const int animation_height = Get_App_Ptr()->Get_Document_Ptr()->IsAnimationWindowOpen() ? 105 : 0;

    switch (this->Quadrant)
    {
        case QD_TOPLEFT:
            RetWidth     = lMainWidth / 3;
            RetHeight    = (lMainHeight - animation_height)/ 2;
            RetLeftEdge  = 0;
            RetTopEdge   = 0;
            break;
        case QD_TOPMIDDLE:
            RetWidth     = lMainWidth / 3;
            RetHeight    = (lMainHeight - animation_height)/ 2;
            RetLeftEdge  = 0 + (lMainWidth / 3); // QD_TOPLEFT RetLeftEdge + QD_TOPLEFT RetWidth
            RetTopEdge   = 0;
            break;
        case QD_TOPRIGHT:
            RetWidth     = lMainWidth - (lMainWidth/3 + lMainWidth/3);//lMainWidth  / 3;
            RetHeight    = (lMainHeight - animation_height)/ 2;
            RetLeftEdge  = (0 + (lMainWidth / 3)) + lMainWidth/3; // QD_TOPMIDDLE RetLeftEdge + RetWidth
            RetTopEdge   = 0;
            break;
        case QD_BOTTOMLEFT:
            RetWidth     = lMainWidth / 2;
            RetHeight    = lMainHeight - ((lMainHeight - animation_height)/2 + animation_height);//(lMainHeight - animation_height)/ 2;
            RetLeftEdge  = 0;
            RetTopEdge   = 0 + (lMainHeight - animation_height)/2; // QD_TOPLEFT RetTopEdge + RetHeight
            break;
        case QD_BOTTOMRIGHT:
            RetWidth     = lMainWidth - (lMainWidth / 2);
            RetHeight    = lMainHeight - ((lMainHeight - animation_height)/2 + animation_height);//(lMainHeight - animation_height)/ 2;
            RetLeftEdge  = 0 + (lMainWidth / 2);
            RetTopEdge   = 0 + (lMainHeight - animation_height)/2; // QD_TOPRIGHT RetTopEdge + RetHeight
            break;
        case QD_ANIMATION:
            RetWidth    = lMainWidth;
            RetHeight   = animation_height;
            RetLeftEdge = 0;
            RetTopEdge  = lMainHeight - animation_height;
            break;
    }
}

//----------------------------------------------------------------------------                                  
void Quadrant_Frame::Update_Title_Bar()
{
   CString lCaption;
   C_Hom_Doc* lDocPtr = Get_App_Ptr()->Get_Document_Ptr();

	// Set the caption
   switch (mViewType)
   {
	case VT_OBLIQUE:
		lCaption = lDocPtr->Get_Task_Name();
		if(lDocPtr->GenderCode() == 0) { //male
			if(!lDocPtr->getMaleHominoidFilePresent())
			{
				lCaption += " --- ";
				lCaption += lDocPtr->Gender();
				lCaption += " hominoid file not loaded";
			}
		} else {
			if(!lDocPtr->getFemaleHominoidFilePresent()) {
				lCaption += " --- ";
				lCaption += lDocPtr->Gender();
				lCaption += " hominoid file not loaded";
			}
		}
		break;
    case VT_STATUS:
		{
        lCaption = CString("3DSSPP - Status - ") + lDocPtr->Get_Task_Name();
		lCaption.AppendFormat(" - Frame %i", lDocPtr->LeftSelect());
		String fn = lDocPtr->getDescriptionRef().FrameName(lDocPtr->getCurrentFrame());
		if(fn.Get_Length() > 0) {
			lCaption.AppendFormat(" (%s)", fn.Get_Start_Ptr());
		}
        break;
		}
	default:
		lCaption = this->Caption; 
		break;
   }

	this->SetWindowText( (LPCTSTR)( lCaption ) );

	// Set the icon
	// TODO this used to depend on task mode
	this->SetIcon(AfxGetApp()->LoadIcon(IDI_3DDSmall),true);


   return;
}

//----------------------------------------------------------------------------                                  
int Quadrant_Frame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
        return -1;
       
    //	Remove Close, Next, and separators from system menu
    CMenu* System_Menu_Ptr = GetSystemMenu( FALSE );   
    assert(System_Menu_Ptr);	// Make sure we got a menu
     
    if(mViewType == VT_ANIMATION)
    {
		int Left_Edge, Top_Edge, Width, Height;
		this->Compute_Placement( Left_Edge, Top_Edge, Width, Height );
		this->MoveWindow( Left_Edge, Top_Edge, Width, Height );
    }     
    else
    {
        // remove unnecessary frame controls, since view windows will have frames
        System_Menu_Ptr->RemoveMenu( 8, MF_BYPOSITION );	// Next Window
        System_Menu_Ptr->RemoveMenu( 7, MF_BYPOSITION );	// Separator
        System_Menu_Ptr->RemoveMenu( 6, MF_BYPOSITION );	// Close
        System_Menu_Ptr->RemoveMenu( 5, MF_BYPOSITION );	// Separator
    }

	// Redraw the menu now that it's changed
    this->CWnd::DrawMenuBar();

    return 0;
}

//----------------------------------------------------------------------------                                  
// Overridden to update main window scrollbars on a move
void Quadrant_Frame::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
   CMDIChildWnd::OnWindowPosChanged(lpwndpos);

   // Get the child frame of the main window (which has the scrollbars)
   CWnd* mainChildWnd = MainFramePtr()->GetWindow(GW_CHILD);
   RECT wndRect;

   if (mainChildWnd != NULL)
   {
      // Redraw to update main window scrollbars
      mainChildWnd->GetWindowRect(&wndRect);
      mainChildWnd->RedrawWindow(0, 0, RDW_FRAME | RDW_INVALIDATE );
   }
}

void Quadrant_Frame::OnSize(UINT nType, int cx, int cy)
{
   CMDIChildWnd::OnSize(nType, cx, cy);

   //if (this->mViewType == VT_STATUS) {
   //   this->SendMessageToDescendants(WM_UPDATE);
   //}
}

void Quadrant_Frame::OnNcRButtonUp(UINT nHitTest, CPoint point)
{
	if(nHitTest == HTCAPTION) {
		CMenu menu;
		menu.LoadMenu(IDR_QuadFrameContext);
		C_Hom_Doc* pDoc = ::Get_App_Ptr()->Get_Document_Ptr();
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}

	CMDIChildWnd::OnNcRButtonUp(nHitTest, point);
}
