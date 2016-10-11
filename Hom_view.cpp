// Hom_view.cpp : implementation of the C_Hom_View class
//

#include "Hom_view.h"

#include "stdafx.h"
#include "hom.h"
#include "afxwin.h"
#include "hom_doc.h"	// Necessary
#include "Wingdi.h" 
#include "main_frm.h"
#include "AppVersion.hpp"
#include "gl_vid.hpp"
#include "glHom/locals.h"
#include "Analysis.h"
#include "crossx.h"

#include <atlimage.h>
#include <windows.h>

using namespace perihelion;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif
// These correspond to the left and right hand's selection code
// They're used for the right and left hand symmetric movement
#define LEFT_HAND_SELECTION_CODE 3
#define RIGHT_HAND_SELECTION_CODE 4
#define LEFT_ANKLE_SELECTION_CODE 8
#define RIGHT_ANKLE_SELECTION_CODE 9
#define USE_OPENGL 0
// Used for rending the floor with the new hominoid
#define FLOOR_CORRECTION 5.277f
#define OBL_FLOOR_DEPTH	 8.8f
#define OBL_FLOOR_WIDTH 13.2f
#define OBL_FLOOR_HEIGHT 26.4f

//Which is right?
//#define BLOCK_SEAT_OFFSET 10.56f
#define BLOCK_SEAT_OFFSET 2.64f

// When the feet are level the floor is combined into one
#define COMBINED_OBL_FLOOR_WIDTH 79.2f
#define COMBINED_OBL_FLOOR_HEIGHT 79.2f 

#define CHAIR_SEAT_OFFSET	8.8f
#define CHAIR_LEG_DEPTH		1.76f
#define CHAIR_LEG_WIDTH		1.76f

#define CHAIR_SEAT_WIDTH	22.0f
#define CHAIR_SEAT_HEIGHT	12.0f

#define CHAIR_BACK_OFFSET 1.76f
#define CHAIR_BACK_DEPTH 1.76f
#define CHAIR_BACK_HEIGHT 2.2f
#define SEAT_FORCE_HEIGHT 4.0f

const char* DEFAULT_STATUS_BAR_TEXT = "Ready";

// Hand Held Object corrections
#define LOC_CORRECTION 4.576f
#define MAG_CORRECTION 9.328f // Excludes the sphere
#define SPHERE_MAG_CORRECT 3.52f

struct JointForces;

/////////////////////////////////////////////////////////////////////////////
// C_Hom_View

IMPLEMENT_DYNCREATE(C_Hom_View, CView)

BEGIN_MESSAGE_MAP(C_Hom_View, CView)
   //{{AFX_MSG_MAP(C_Hom_View)
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
   ON_WM_MOUSEMOVE()
   ON_WM_SIZE()
   ON_WM_DESTROY()
   ON_WM_PAINT()
   ON_WM_ERASEBKGND()
   ON_WM_CREATE()
   ON_WM_CONTEXTMENU()
   ON_COMMAND(ID_Obl_FrontView, OnOblFrontView)
   ON_COMMAND(ID_Obl_StdZoom, OnOblStdZoom)
   ON_COMMAND(ID_Obl_BackView, OnOblBackView)
   ON_COMMAND(ID_Obl_LeftView, OnOblLeftView)
   ON_COMMAND(ID_Obl_RightView, OnOblRightView)
   ON_COMMAND(ID_Obl_TopView, OnOblTopView)
   ON_COMMAND(ID_Obl_CenterFigure, OnOblCenterFigure)
   ON_WM_MOUSEWHEEL()
   ON_WM_RBUTTONDOWN()
   ON_WM_RBUTTONUP()
   ON_WM_KEYDOWN()
   //}}AFX_MSG_MAP
   // Custom message handlers
   ON_WM_NCHITTEST()
   // Standard printing commands
   ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
   ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
   ON_COMMAND(ID_Obl_UserView, OnOblUserview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C_Hom_View construction/destruction

C_Hom_View::C_Hom_View()
:
mSelectInProgress( false ) ,
mCtrlClick(false),
mShiftClick(false),
mTargets() ,
mSelectionCode( 0 ),
mCameraManipulationInProgress( 0 ),
mWindowScale_x( 0 ),
mWindowScale_y( 0 ),
mOldViewport_x( 0 ),
mOldViewport_y( 0 ), 
quadric( NULL ),
msaved_altitude( 20 ),
msaved_azimuth( -20 )
{
   // TODO: add construction code here

}


C_Hom_View::~C_Hom_View()
{ 
   // ACT - When the parent class is destroyed, try to clean up our memory
   //    allocation
   TryDeleteTargets();
   if (quadric != NULL)
   {	
      gluDeleteQuadric(quadric);
   }
}

#include "graph_db.hpp"
#include "ReportView.hpp" // For SSPPPrintPreview
#include ".\hom_view.h"

/////////////////////////////////////////////////////////////////////////////
// C_Hom_View support

void C_Hom_View::OnFilePrintPreview()
{
   SSPPPrintPreview(this);
}

bool TargetIsForce(int selectionCode)
{
   switch(selectionCode)
   {
   case MS_FORCE_L_HAND:
   case MS_FORCE_R_HAND:
   case MS_FORCE_R_ELBOW:
   case MS_FORCE_R_SHOULD:
   case MS_FORCE_L_ELBOW:
   case MS_FORCE_L_SHOULD:
   case MS_FORCE_L5S1:
   case MS_FORCE_R_HIP:
   case MS_FORCE_R_KNEE:
   case MS_FORCE_R_ANKLE:
   case MS_FORCE_L_HIP:
   case MS_FORCE_L_KNEE:
   case MS_FORCE_L_ANKLE:
      return true;
   default:
      return false;
   }
}

//--------------------------------------------------------------                                  
void C_Hom_View::Select_Screen_Point(CPoint DevicePoint)
{
    ;
   /*C_Hom_App& App = (C_Hom_App&)(*AfxGetApp());
   // Variables for Angle Calculations
   JointForces jointForces;

   C_Hom_Doc* Doc_Ptr = GetDocument();

   ASSERT_VALID( Doc_Ptr );
   if (mCameraManipulationInProgress) 
      return;	// We're manipulating the camera, not selecting stuff!

   //--- Conditions OK for pick ---
   this->BeginWaitCursor();

   Graphic_Database&   GDB = *(  Doc_Ptr->Get_Graphic_Database_Ptr()  );
   assert(  Is_Object_Ref( GDB )  );
   Flt_3D_Polyline WorldPoints =
      GDB.Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name ).Get_Points();
   Monitor_Record& Monitor = this->Get_Monitor_Ref();

   //Dummy logical palette

   CClientDC DC( this ); //Dummy device context 

   // this is only used for converting device to logical coordinates -> why not use CClientDC's native function?
   //  -> Display sets up its own strange mapping mode, coordinate systems differ
   Display Dsp(*this, NULL,2*(int)Monitor.Get_Film_Max_X(),2*(int)Monitor.Get_Film_Max_Y(),false);

   // ACT - In a control-drag we only move on one axis.
   if (mCtrlClick && View_Type == VT_OBLIQUE)
   {
      if(abs(DevicePoint.x - mStartPoint.x) > abs(DevicePoint.y - mStartPoint.y)) 
         DevicePoint.y = mStartPoint.y;
      else 
         DevicePoint.x = mStartPoint.x;
   }

   CPoint Logical = Dsp.Device_To_Logical(DevicePoint);

   if (View_Type != VT_OBLIQUE)
      AdjustCoordinates(Logical.x, Logical.y);

   String Result;

   Doc_Ptr->Put_Manipulate_Mode( mSelectionCode );

   if (mCtrlClick && (mSelectionCode== RIGHT_HAND_SELECTION_CODE
      || mSelectionCode == LEFT_HAND_SELECTION_CODE))
   {
      Result = GDB.Skeleton_Manipulation_Symmetric_Hands(App.Hom_Ske_Name,
         (int)(this->View_Type), Monitor, Logical.x , Logical.y);
   }
   else if (mCtrlClick && (mSelectionCode == LEFT_ANKLE_SELECTION_CODE
      || mSelectionCode == RIGHT_ANKLE_SELECTION_CODE))
   {
      double changeZ;
      changeZ = WorldPoints(L_ANKLE)(2) - WorldPoints(R_ANKLE)(2);
      WorldPoints(L_KNEE).Put(WorldPoints(L_KNEE)(0), WorldPoints(L_KNEE)(1),
         WorldPoints(L_KNEE)(2) + changeZ);
      WorldPoints(L_ANKLE).Put(WorldPoints(L_ANKLE)(0),
         WorldPoints(L_ANKLE)(1), WorldPoints(R_ANKLE)(2));
      Result = GDB.Skeleton_Level_Ankle(App.Hom_Ske_Name,
         (int)(this->View_Type), true);
   }
   else
   {
      Result = GDB.Skeleton_Manipulation_Point_Move
         (
         App.Hom_Ske_Name ,
         (int)( this->View_Type ) ,
         Monitor ,
         Logical.x , Logical.y
         );
   }

   Doc_Ptr->Put_Manipulate_Mode(0);

   if (Result == String("Successful")) //posture modified
   {
      if(TargetIsForce(mSelectionCode))
      {
         GDB.Skeleton_Joint_Force_Angles_Get(Get_App_Ptr()->Hom_Ske_Name,
            &jointForces);

         switch(mSelectionCode)
         {
         case MS_FORCE_L_HAND:
         case MS_FORCE_R_HAND:
            Doc_Ptr->SetHandForceAnglesFromDB();
            break;
         case MS_FORCE_L_ELBOW:
            CalculateJointForce(JT_LeftElbow, GDB, jointForces.leftElbowForceHorz, jointForces.leftElbowForceVert);  
            break;
         case MS_FORCE_R_ELBOW:
            CalculateJointForce(JT_RightElbow, GDB, jointForces.rightElbowForceHorz, jointForces.rightElbowForceVert);
            break;
         case MS_FORCE_L_SHOULD:
            CalculateJointForce(JT_LeftShoulder, GDB, jointForces.leftShoulderForceHorz, jointForces.leftShoulderForceVert);
            break;
         case MS_FORCE_R_SHOULD:
            CalculateJointForce(JT_RightShoulder, GDB, jointForces.rightShoulderForceHorz, jointForces.rightShoulderForceVert);
            break;
         case MS_FORCE_L5S1:
            CalculateJointForce(JT_L5S1, GDB, jointForces.l5s1ForceHorz, jointForces.l5s1ForceVert);
            break;
         case MS_FORCE_L_HIP:
            CalculateJointForce(JT_LeftHip, GDB, jointForces.leftHipForceHorz, jointForces.leftHipForceVert);
            break;
         case MS_FORCE_R_HIP:
            CalculateJointForce(JT_RightHip, GDB, jointForces.rightHipForceHorz, jointForces.rightHipForceVert);
            break;
         case MS_FORCE_L_KNEE:
            CalculateJointForce(JT_LeftKnee, GDB, jointForces.leftKneeForceHorz, jointForces.leftKneeForceVert);
            break;
         case MS_FORCE_R_KNEE:
            CalculateJointForce(JT_RightKnee, GDB, jointForces.rightKneeForceHorz, jointForces.rightKneeForceVert);
            break;
         case MS_FORCE_L_ANKLE:
            CalculateJointForce(JT_LeftAnkle, GDB, jointForces.leftAnkleForceHorz, jointForces.leftAnkleForceVert);
            break;
         case MS_FORCE_R_ANKLE:
            CalculateJointForce(JT_RightAnkle, GDB, jointForces.rightAnkleForceHorz, jointForces.rightAnkleForceVert);
            break;
         default:
            assert(0);	// Bad force type!
            break;
         }
      }
      else	// We hit a joint
         Doc_Ptr->SetJointAnglesFromDB(); // newly computed from database to document

      Doc_Ptr->MakeDirtyAndUpdateViews(true);
   }
   else //attempt to manipulate posture directly unsuccessful
   {
      //AfxMessageBox( Result );
      ::MessageBox( NULL, "Posture not attainable.", "Direct Manipulation", MB_OK );
   }   

   this->EndWaitCursor();*/
}                

/////////////////////////////////////////////////////////////////////////////
// C_Hom_View drawing

/*------------------------------------------------------------*/
void C_Hom_View::OnInitialUpdate()
{
   C_Hom_App& App = *Get_App_Ptr();

   this->Monitor_Name = App.mNewWindowDataPtr->mMonitorName;
   this->View_Type = App.mNewWindowDataPtr->mViewType;

   CView::OnInitialUpdate(); //Per MS document Q99182

   ((Quadrant_Frame*)(this->GetParent()))->Update_Title_Bar();

   this->mDibViewPtr = NULL;
}

//----------------------------------------------------------------------------
void C_Hom_View::OnPrepareDC(CDC *pDC, CPrintInfo* pInfo) { CView::OnPrepareDC(pDC, pInfo); }

// ACT - ADDED to allow us to call on program termination to destroy dynamically
// allocated memory     
void C_Hom_View :: TryDeleteTargets(void) 
{
   if( mTargets.Not_Empty() ) //ALWAYS clear the target list
   {
      const void * lVoidPtr = NULL;
      mTargets.Reset();
      while( mTargets.Next_Link_OK() )
      {
         lVoidPtr = mTargets ++;
         mTargets - lVoidPtr; //Remove the pointer from the list
         delete (Target*)( lVoidPtr ); //Destroy the thing pointed to
      }
   }	
}
//----------------------------------------------------------------------------                                  
void C_Hom_View::OnDraw(CDC* pDC)
{

   C_Hom_Doc* pDoc = GetDocument();
   C_Hom_App& App = *Get_App_Ptr();

   //this->BeginWaitCursor();

   //Display monitor contents   
   int   lHorizontalPixels;
   int   lVerticalPixels;
   if( pDC->IsPrinting()) 
   {
#define HOM_VIEW_PRINT_SCALING 0.5f 
      lHorizontalPixels = pDC->GetDeviceCaps( HORZRES );
      lVerticalPixels	= pDC->GetDeviceCaps( VERTRES );

      CSize WindowSize;
      WindowSize = pDC->GetWindowExt();
   }
   if (pDC->IsPrinting()) 
   {	
      Print_GL_Figure(pDC);		// Contains the printing routines to print the opengl stick figure
   }

   return;
}

/////////////////////////////////////////////////////////////////////////////
// C_Hom_View printing
BOOL C_Hom_View::OnPreparePrinting(CPrintInfo* pInfo)
// Identical to the function in ReportView.hpp
// This copy was made because C_Hom_View could not be derived from 
// ReportView.  It was either make a copy or create another class that was 
// almost an exact copy of ReportView
{
   //pInfo->SetMaxPage(1);
   //pInfo->m_bDirect = TRUE;

   //// default preparation
   //return DoPreparePrinting(pInfo);

	C_Main_Frame* lMainFramePtr = MainFramePtr();
   if (mDibViewPtr == NULL) {
		mDibViewPtr = lMainFramePtr->Create_Dib_Window();
   }
   CWnd* lFramePtr = this->GetParent();
  /* CWnd* newFrame = new CWnd();
   
   lFramePtr->Set*/

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
   // Added so the print preview toolbars do not appear in the final output

   lFramePtr->SetActiveWindow();
   lFramePtr->BringWindowToTop();
   lFramePtr->UpdateWindow();
   mDibViewPtr->LoadDib(lFramePtr);
   
   //--- Call normal printing ---
   
   pInfo->m_bDirect = true;
   BOOL lPrintResult = mDibViewPtr->OnPreparePrinting( pInfo );  
   
   //--- Restore the window ---

   lFramePtr->SetWindowPlacement( & lSavePlacement );
   lFramePtr->ShowWindow( lSavePlacement.showCmd );
   lFramePtr->UpdateWindow();
   
   return lPrintResult;  
}

void C_Hom_View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void C_Hom_View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
   // TODO: add cleanup after printing
}
void C_Hom_View::OnPrint(CDC* pDC, CPrintInfo* pInf )
{

	if (mDibViewPtr != NULL)
      mDibViewPtr->OnDraw( pDC ); 
}
/////////////////////////////////////////////////////////////////////////////
// C_Hom_View diagnostics

#ifdef _DEBUG
void C_Hom_View::AssertValid() const
{
   CView::AssertValid();
}

void C_Hom_View::Dump(CDumpContext& dc) const
{
   CView::Dump(dc);
}

//@@@C_Hom_Doc* C_Hom_View::GetDocument() // non-debug version is inline
//{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(C_Hom_Doc)));
//	return (C_Hom_Doc*)m_pDocument;
//}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// C_Hom_View message handlers

/*---------------------------------------------------------------*/
void C_Hom_View::OnLButtonDown (UINT nFlags, CPoint Point)
{
   //For some reason, the main frame title is modified on this event. Given,
   //that we have NO main frame handler for this message I can only assume
   //that MFC is doing it somewhere. The following line is a workaround until
   //we figure out what is going on or MFC is fixed.

   MainFramePtr()->UpdateMainTitleBar();
   mStartPoint = Point;			// Updated on every move
   mAbsoluteStartPoint = Point;	// Always remains as the initial value

   if (MK_CONTROL & nFlags)
   { 
      mCtrlClick = true;
   }
   else if (MK_SHIFT & nFlags) 
   {
      mShiftClick = true;
   }
   if( this->View_Type == VT_OBLIQUE )
   {
      this->SetCapture();
      mCameraManipulationInProgress = 1;

      //if (MK_CONTROL & nFlags)
      //{
      //	ActivateGLWindow(Monitor_Name);	// Clear and load the GL Drawing Context
      //	//for (int i=0;i<200;i++)
      //	Draw_GL_Stick_Figure(this);
      //}
      return;
   }

   if( mSelectionCode != 0 )
   {
      this->SetCapture();
      mSelectInProgress = true;

      //Clip the cursor

      RECT   lRect;
      this->CWnd::GetWindowRect( & lRect );
      ClipCursor( & lRect );

      //Set the cursor image

      SetClassLong
         (
         m_hWnd ,    
         GCL_HCURSOR ,
         (long)( Get_App_Ptr()->mCrossCursorHdl )
         );
      SetCursor( Get_App_Ptr()->mCrossCursorHdl );
   }

   return;
}

/*------------------------------------------------------------*/
void C_Hom_View::OnLButtonUp(UINT nFlags, CPoint aPoint)
{
   if( this->View_Type == VT_OBLIQUE )
   {
      ReleaseCapture();
      mCameraManipulationInProgress = 0;

   }
   else if( mSelectInProgress == true )
   {
      ReleaseCapture();
      mSelectInProgress = false;
      ClipCursor( NULL );

      SetClassLong
         (
         m_hWnd ,    
         GCL_HCURSOR ,
         (long)( Get_App_Ptr()->mArrowCursorHdl )
         ); 
      SetCursor( Get_App_Ptr()->mArrowCursorHdl );

      this->Select_Screen_Point( aPoint );
   }
   mCtrlClick = false;
   mShiftClick = false;
   return;
}

//----------------------------------------------------------------------------
UINT C_Hom_View::OnNcHitTest(CPoint aPoint)
{
   UINT   lHitResult = CWnd::OnNcHitTest( aPoint );

   if( this->View_Type == VT_OBLIQUE ) return lHitResult;

   if( GetDocument()->get2DMode() )
   {
      if( this->View_Type == VT_TOP )   return lHitResult;
      if( this->View_Type == VT_FRONT ) return lHitResult;
      if( this->View_Type == VT_SIDE ) return lHitResult;
   }

   if( ! mSelectInProgress ) //free roaming; test for over target
   {
      BOOL   lNoHit = TRUE;
      Target *   lTargetPtr = NULL;
      mTargets.Reset();
      while( mTargets.Next_Link_OK() && lNoHit )
      {
         lTargetPtr = (Target*)( mTargets ++ );
         if(	aPoint.x > lTargetPtr->mRect.left	&&
            aPoint.x < lTargetPtr->mRect.right	&&
            aPoint.y > lTargetPtr->mRect.top	&&
            aPoint.y < lTargetPtr->mRect.bottom		) //you hit a target
         {
            lNoHit = FALSE;
            mSelectionCode = lTargetPtr->mSelectionCode;
         }
      }

      //Set cursor

      if( lNoHit )
      {
         SetClassLong
            (
            m_hWnd ,    
            GCL_HCURSOR ,
            (long)( Get_App_Ptr()->mArrowCursorHdl )
            ); 
        SetCursor( Get_App_Ptr()->mArrowCursorHdl );
        mSelectionCode = 0;
        MainFramePtr()->StatusBarText(DEFAULT_STATUS_BAR_TEXT);
      }

      else //there was a hit on a target
      {
         SetClassLong
            (
            m_hWnd ,    
            GCL_HCURSOR ,
            (long)( Get_App_Ptr()->mSelectableCursorHdl )
            ); 
         SetCursor( Get_App_Ptr()->mSelectableCursorHdl );
         MainFramePtr()->StatusBarText( STR_MANIPULATION_STATE[mSelectionCode]);
      }
   }

   return lHitResult;
}

void C_Hom_View::OnMouseMove(UINT nFlags, CPoint point) 
{
    CView::OnMouseMove(nFlags,point);
}

void C_Hom_View::OnPaint() 
{
   C_Hom_Doc* pDoc = GetDocument();
   CPaintDC dc(this);		// DAC - Does some stuff w/ the device context that doesn't 
   // break all the messageboxe when using opengl context

   if(true)
   {
      switch(this->View_Type)
      {
      case VT_FRONT:
      case VT_SIDE:
      case VT_TOP:
      case VT_OBLIQUE:
         if (ActivateGLWindow(Monitor_Name) || 
            CreateGLContext(this->m_hWnd,mhDC,&mhRC, this->Monitor_Name,(this->View_Type == VT_OBLIQUE) ? GL_OBLIQUE : GL_ORTHO))
         {
            if(this->View_Type == VT_OBLIQUE)
            {
               C_Hom_Doc* pDoc = GetDocument();
               glClearColor(1.0f, 1.0f, 145.0f/255.0f, 1.0f);
               glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
               /*if(pDoc->IsPicture())
               {
                  Draw_Background(this, &dc);
               }*/
               RenderGLOblique(pDoc->getPelvicAngle());
               GLSwapBuffers();
            }
            else
            {
               SetLocking();
               Draw_GL_Stick_Figure(this);
               GLSwapBuffers();
               // Do TargetRect creation here
               // Only create a TargetRect if it's a selectable rect from SelectionCode[] in gl_vid.cpp)
               TryDeleteTargets();				// delete the previously created rects
               GenerateTargetRects(this);		// make new ones
            }
            wglMakeCurrent(NULL,NULL);
         }
         break;

      case VT_STATUS:
      case VT_REPORT:
      case VT_BAL:
         // Call the normal paint functions
         OnPrepareDC(&dc);
         OnDraw(&dc);
         break;
      }	// end switch(this->View_Type)
   }
   else
   {
      OnPrepareDC(&dc);
      OnDraw(&dc);
   }
}

void C_Hom_View::OnSize(UINT nType, int cx, int cy) 
{
   if (ActivateGLWindow(Monitor_Name))	
   {
      Draw_GL_Stick_Figure(this);
   }
   CView::OnSize(nType, cx, cy);
}

void C_Hom_View::OnDestroy() 
{
   DestroyGLContext(this->Monitor_Name);
   CView::OnDestroy();
}

BOOL C_Hom_View::OnEraseBkgnd(CDC* pDC) 
{
   // TODO: Add your message handler code here and/or call default
   if (pDC->IsPrinting())
      return CView::OnEraseBkgnd(pDC);
   else
      return 1; 
}

// Override basic class used for the CView window
// to allow us to have persistant DCs, that openGL can use
// [Windows usually reuses a set of 5 DCs each time a window
// is drawn... each openGL rendering conects needs its own 'permanent' DC]
BOOL C_Hom_View::PreCreateWindow( CREATESTRUCT &cs ) 
{
   cs.style |= ( WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CS_OWNDC ); 

   return CView::PreCreateWindow(cs); 
}

int C_Hom_View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
   C_Hom_Doc* pDoc = GetDocument();
   if (CView::OnCreate(lpCreateStruct) == -1)
      return -1;

   mhDC	= ::GetDC(m_hWnd);
   mWorldSize = 0.0f;

   // Using Invalidate and UpdateWindow causes only the current
   // window to update, eliminating flicker in other windows
   Invalidate();
   UpdateWindow();

   return 0;
}

void C_Hom_View::OnContextMenu(CWnd* pWnd, CPoint point) 
{
   if(this->View_Type == VT_OBLIQUE)	// We're in the Oblique view window
   {
      CMenu menu;
      menu.LoadMenu(IDR_Context_Oblique);

      menu.CheckMenuItem(ID_MODELTYPE_NONE, MF_CHECKED | MF_BYCOMMAND);

      menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
         point.x, point.y, this);
   }
   else if(this->View_Type == VT_FRONT
      || this->View_Type == VT_SIDE
      || this->View_Type == VT_TOP )
   {
      CMenu menu;
      menu.LoadMenu(IDR_Context_3View);
      if(!GetDocument()->CanUndo())
         menu.EnableMenuItem(ID_UndoDirectManip, MF_GRAYED);
      if(!GetDocument()->CanRedo())
         menu.EnableMenuItem(ID_RedoDirectManip, MF_GRAYED);
      if(GetDocument()->get2DMode())
      {
         // Gray out posture prediction, locking mode if we're in 2d
         menu.EnableMenuItem(ID_POSTURE_POSTUREPREDICTION, MF_GRAYED);
         menu.EnableMenuItem(ID_POSTURE_LOCKINGMODE, MF_GRAYED);
      }

      menu.GetSubMenu(0)
         ->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
         point.x, point.y, this);
   }
}

void C_Hom_View::OnOblFrontView() 
{
   ;
}

void C_Hom_View::OnOblStdZoom() 
{
   ;
}

void C_Hom_View::OnOblBackView() 
{
   ;
}

void C_Hom_View::OnOblLeftView() 
{
   ;
}

void C_Hom_View::OnOblRightView() 
{
   ;
}

void C_Hom_View::OnOblTopView() 
{
   ;
}


void C_Hom_View::OnOblUserview()
{
   ;
}

void C_Hom_View::OnOblCenterFigure() 
{
   ;
}

BOOL C_Hom_View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
   return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void C_Hom_View::OnRButtonDown(UINT nFlags, CPoint point) 
{
   mBigMove = false;

   mStartPoint = point;
   mAbsoluteStartPoint = point;

   if (this->View_Type == VT_OBLIQUE) {
      this->SetCapture();
      mCameraManipulationInProgress = 1;
      mCtrlClick = true;

      ActivateGLWindow(Monitor_Name);	// Clear and load the GL Drawing Context
   }

   CView::OnRButtonDown(nFlags, point);
}

void C_Hom_View::OnRButtonUp(UINT nFlags, CPoint point) 
{
   if(!mBigMove)  // Mouse pointer has not moved more than 5 pixels
      CView::OnRButtonUp(nFlags, point);  // Do context menu

   if (this->View_Type == VT_OBLIQUE) {
      ReleaseCapture();
      mCameraManipulationInProgress = 0;
   }

   mCtrlClick = false;
   mShiftClick = false;
}

void C_Hom_View::AddTargetRect(CRect rect, int SelectionCode)
{
   // Dynamically allocate a target
   Target * lTargetPtr = new Target(rect.left, rect.top,
      rect.right, rect.bottom, SelectionCode);

   // Add target to list of mTargets
   mTargets + (void*)lTargetPtr;
}

void C_Hom_View::SetLocking()
{
   int lock = GetDocument()->IsLocked();

   if(GetDocument()->get2DMode())
      lock = LM_ALL;

   if (BOOL(mModeChange) == GetDocument()->get2DMode()
      && lock == mLockChange)
      return;

   // Remembers the previous locking and mode setting
   mLockChange = lock;
   mModeChange = (GetDocument()->get2DMode()) ? true : false;
   switch (lock) {
case LM_NONE :
   // If locking changed, restore the values
   if (GetPoint(PELVIS) == (int)MS_NULL)
   {
      SetPoint(RIGHT_ANKLE, MS_ANKLE_R);
      SetPoint(LEFT_ANKLE, MS_ANKLE_L);
      SetPoint(PELVIS, MS_HIPS);
   }
   if (GetPoint(RIGHT_ELBOW) == (int)MS_NULL)
   {
      SetPoint(RIGHT_ELBOW, MS_ELBOW_R);
      SetPoint(RIGHT_HAND_CTRL, MS_HAND_CTR_R);
      SetPoint(LEFT_ELBOW, MS_ELBOW_L);
      SetPoint(LEFT_HAND_CTRL, MS_HAND_CTR_L);
      SetPoint(FORCE_R, MS_FORCE_R_HAND);
      SetPoint(FORCE_L, MS_FORCE_L_HAND);
      SetPoint(FORCE_EL_L, MS_FORCE_L_ELBOW);
      SetPoint(FORCE_EL_R, MS_FORCE_R_ELBOW);
      SetPoint(FORCE_SHOL_L, MS_FORCE_L_SHOULD);
      SetPoint(FORCE_SHOL_R, MS_FORCE_R_SHOULD);
      SetPoint(FORCE_L5_S1, MS_FORCE_L5S1);
      SetPoint(FORCE_HIP_L, MS_FORCE_L_HIP);
      SetPoint(FORCE_HIP_R, MS_FORCE_R_HIP);
      SetPoint(FORCE_KNEE_L, MS_FORCE_L_KNEE);
      SetPoint(FORCE_KNEE_R, MS_FORCE_R_KNEE);
      SetPoint(FORCE_ANKLE_L, MS_FORCE_L_ANKLE);
      SetPoint(FORCE_ANKLE_R, MS_FORCE_R_ANKLE);
   }
   if (GetPoint(UPPER_NECK) == (int)MS_NULL)
   {
      SetPoint(UPPER_NECK, MS_TRUNK_FLEXION);
      SetPoint(LOWER_NECK, MS_TRUNK_LATERAL_BENDING);
      SetPoint(LEFT_SHOULDER, MS_TRUNK_AXIAL_ROTATION);
   }
   break;
case LM_ARMS_AND_TRUNK:
   SetPoint(UPPER_NECK, MS_NULL);
   SetPoint(LOWER_NECK, MS_NULL);
   SetPoint(LEFT_SHOULDER, MS_NULL);
   SetPoint(RIGHT_ELBOW, MS_NULL);
   SetPoint(RIGHT_HAND_CTRL, MS_NULL);
   SetPoint(LEFT_ELBOW, MS_NULL);
   SetPoint(LEFT_HAND_CTRL, MS_NULL);
   SetPoint(PELVIS, MS_NULL);
   if (GetPoint(RIGHT_ANKLE) == (int) MS_NULL)
   {
      SetPoint(RIGHT_ANKLE, MS_ANKLE_R);
      SetPoint(LEFT_ANKLE, MS_ANKLE_L);
   }
   break;   // This one does not fall through
case LM_ALL:
   SetPoint(RIGHT_ELBOW, MS_NULL);
   SetPoint(RIGHT_HAND_CTRL, MS_NULL);
   SetPoint(LEFT_ELBOW, MS_NULL);
   SetPoint(LEFT_HAND_CTRL, MS_NULL);
   SetPoint(FORCE_R, MS_NULL);
   SetPoint(FORCE_L, MS_NULL);
   SetPoint(FORCE_EL_L, MS_NULL);
   SetPoint(FORCE_EL_R, MS_NULL);
   SetPoint(FORCE_SHOL_L, MS_NULL);
   SetPoint(FORCE_SHOL_R, MS_NULL);
   SetPoint(FORCE_L5_S1, MS_NULL);
   SetPoint(FORCE_HIP_L, MS_NULL);
   SetPoint(FORCE_HIP_R, MS_NULL);
   SetPoint(FORCE_KNEE_L, MS_NULL);
   SetPoint(FORCE_KNEE_R, MS_NULL);
   SetPoint(FORCE_ANKLE_L, MS_NULL);
   SetPoint(FORCE_ANKLE_R, MS_NULL);
   // Continues through to lock the legs and trunk
case LM_LEGS_AND_TRUNK:
   SetPoint(UPPER_NECK, MS_NULL);		// Upper Neck
   SetPoint(LOWER_NECK, MS_NULL);		// Lower Neck
   SetPoint(LEFT_SHOULDER, MS_NULL);	// Left Shoulder
   if (lock == LM_LEGS_AND_TRUNK)
   {
      if (GetPoint(RIGHT_ELBOW) == int(MS_NULL))
         // Checks to see if anything needs to be restored
      {
         SetPoint(RIGHT_ELBOW, MS_ELBOW_R);
         SetPoint(RIGHT_HAND_CTRL, MS_HAND_CTR_R);
         SetPoint(LEFT_ELBOW, MS_ELBOW_L);
         SetPoint(LEFT_HAND_CTRL, MS_HAND_CTR_L);
         SetPoint(FORCE_R, MS_FORCE_R_HAND);
         SetPoint(FORCE_L, MS_FORCE_L_HAND);
      }
   }
   // Continues through to the legs so no break;
case LM_LEGS :
   SetPoint(RIGHT_ANKLE, MS_NULL);		// Right Ankle
   SetPoint(LEFT_ANKLE, MS_NULL);		// Left Ankle
   SetPoint(PELVIS, MS_NULL);			// Pelvis (MS_HIPS)
   if (lock == LM_LEGS)
   {
      if (GetPoint(UPPER_NECK) == (int) MS_NULL) 
         // Checks to see if anything needs to be restored
      {
         SetPoint(UPPER_NECK, MS_TRUNK_FLEXION);
         SetPoint(LOWER_NECK, MS_TRUNK_LATERAL_BENDING);
         SetPoint(LEFT_SHOULDER, MS_TRUNK_AXIAL_ROTATION);
      }
      if (GetPoint(RIGHT_ELBOW) == (int) MS_NULL)
         // Checks to see if anything needs to be restored
      {
         SetPoint(RIGHT_ELBOW, MS_ELBOW_R);
         SetPoint(RIGHT_HAND_CTRL, MS_HAND_CTR_R);
         SetPoint(LEFT_ELBOW, MS_ELBOW_L);
         SetPoint(LEFT_HAND_CTRL, MS_HAND_CTR_L);
         SetPoint(FORCE_R, MS_FORCE_R_HAND);
         SetPoint(FORCE_L, MS_FORCE_L_HAND);
      }
   }
   break;
   }  // End 
}

void C_Hom_View::CalculateJointForce(JointTorqueEnm joint,
                                     Graphic_Database& GDB,
                                     double horzAngle, double vertAngle)
                                     // NOTE 1 = x, 2 = y, 3 = z (subtract one for the unit vector)
{
   ttarray& docAForce = GetDocument()->getAForceRef();
   // Before doing anything, setup undo

   // Calculates the magnitude of the vector
   double magnitudeOfForce = sqrt(
      pow(docAForce[JOINT_FORCES][joint][1], 2)
      + pow(docAForce[JOINT_FORCES][joint][2], 2)
      + pow(docAForce[JOINT_FORCES][joint][3], 2));

   // Finds the unit vector
   double unitVector[3];
   unitVector[0] = cos(horzAngle*((M_PI/180.0)));
   unitVector[1] = sin(horzAngle*((M_PI/180.0)))*cos(vertAngle*((M_PI/180)));
   unitVector[2] = sin(vertAngle*((M_PI/180.0)));
   double magnitudeOfUnitVector = sqrt(pow(unitVector[0], 2)
      + pow(unitVector[1], 2) + pow(unitVector[2], 2));

   // Sets the new values (negative because the force is drawn opposite)
   docAForce[JOINT_FORCES][joint][1] = -((unitVector[0]/magnitudeOfUnitVector) * magnitudeOfForce);
   docAForce[JOINT_FORCES][joint][2] = -((unitVector[1]/magnitudeOfUnitVector) * magnitudeOfForce);
   docAForce[JOINT_FORCES][joint][3] = -((unitVector[2]/magnitudeOfUnitVector) * magnitudeOfForce);

   //(BS) A quick fix for the problem where only applied forces on females have any effect.
   docAForce[1][joint][1] = -((unitVector[0]/magnitudeOfUnitVector) * magnitudeOfForce);
   docAForce[1][joint][2] = -((unitVector[1]/magnitudeOfUnitVector) * magnitudeOfForce);
   docAForce[1][joint][3] = -((unitVector[2]/magnitudeOfUnitVector) * magnitudeOfForce);
}

void C_Hom_View::AdjustCoordinates(long& x, long& y)  
// DAC - The original CDC method of drawing shifted the whole model up some
//    models to make it fit in the window better. It only did that for the front
//    and side views, however.  The selection points are based on the CDC
//    method, so if we're using the opengl context, we need to compensate for
//    this shifting, but only for the front and top views.
// JTK - The View border and scaling to the center also had to be accounted for.
{
   y = (long(y * (mWorldSize / ORIGINAL_WORLD_SIZE)));
   x = (long(x * (mWorldSize / ORIGINAL_WORLD_SIZE)));
   
   return;
}

void C_Hom_View::Print_GL_Figure(CDC* pDC)
// This function was deprecated for the new 3DSSPP print code
{
   C_Hom_Doc* pDoc = this->GetDocument();
   HDC	 memoryDC;
   HDC	 oldDC;
   HGLRC	 oldRC;
   HGLRC  currentRC;
   LPVOID bitmapBits;
   CSize	 bitmapSize;
   HBITMAP hBmp;
   BITMAPINFO bitmapHeader;
   bitmapHeader.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
   bitmapHeader.bmiHeader.biWidth		= STICK_FIG_PRINTING_SIZE;
   bitmapHeader.bmiHeader.biHeight		= STICK_FIG_PRINTING_SIZE;
   bitmapHeader.bmiHeader.biPlanes		= 1;
   bitmapHeader.bmiHeader.biBitCount	= COLOR_DEPTH;
   bitmapHeader.bmiHeader.biCompression	= BI_RGB;
   bitmapHeader.bmiHeader.biSizeImage	=  STICK_FIG_PRINTING_SIZE * STICK_FIG_PRINTING_SIZE * 3; 

   hBmp = CreateDIBSection(pDC->GetSafeHdc(), &bitmapHeader, DIB_RGB_COLORS, &bitmapBits, NULL, (DWORD) 0);

   if (!hBmp)
   {
      AfxMessageBox("Error!:  Problem creating rendering context for printing");
   }

   memoryDC = CreateCompatibleDC(NULL);
   if (!memoryDC)
   {
      DeleteObject(hBmp);
      hBmp = NULL;
      return;
   }
   SelectObject(memoryDC, hBmp);
   if (!SetPixelFormat(memoryDC, PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL))
   {
      DeleteObject(hBmp);
      hBmp = NULL;
      DeleteDC(memoryDC);
      memoryDC = NULL;
      return;
   }
   currentRC = wglCreateContext(memoryDC);
   if (!currentRC)
   {
      DeleteObject(hBmp);
      hBmp = NULL;
      DeleteDC(memoryDC);
      memoryDC = NULL;
      return;
   }
   oldDC = wglGetCurrentDC();
   oldRC = wglGetCurrentContext();
   wglMakeCurrent(memoryDC, currentRC);
   if (View_Type != VT_OBLIQUE)
   {
      // Print the stick figures
      setupOpenGLContext();
      Draw_GL_Stick_Figure(this);
   } else {
      // Print the hominoid
      C_Hom_Doc* pDoc = GetDocument();

      glClearColor(1.0f, 1.0f, 145.0f/255.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      //pDC->SelectPalette(&colorPalette, true);
      /*if(pDoc->IsPicture())
      {
         Draw_Background(this, pDC);
      }*/
      RenderGLOblique(pDoc->getPelvicAngle());
      //GLSwapBuffers();
   }
   // Force opengl to finish	
   glFinish();
   wglMakeCurrent(NULL, NULL);
   wglDeleteContext(currentRC);
   wglMakeCurrent(oldDC, oldRC);

   int printResults = StretchDIBits(pDC->GetSafeHdc(), STICK_FIG_PRINTING_SIZE/2, STICK_FIG_PRINTING_SIZE/2, -STICK_FIG_PRINTING_SIZE, -STICK_FIG_PRINTING_SIZE, 0, 0, STICK_FIG_PRINTING_SIZE, STICK_FIG_PRINTING_SIZE, (GLubyte*) bitmapBits, &bitmapHeader, DIB_RGB_COLORS, SRCCOPY);
   if (printResults == GDI_ERROR)
      AfxMessageBox("Error: Unable to render image to printer");

   // Release Memory
   DeleteObject(hBmp);
   hBmp = NULL;
   DeleteDC(memoryDC);
   memoryDC = NULL;
   oldDC = NULL;

   return;
}
void HominoidToWorldConversion(const perihelion::pVector3f &in, perihelion::pVector3f &out, C_Hom_Doc* doc=NULL)
{
   static C_Hom_Doc* pDoc=NULL;
   if (doc != NULL) pDoc = doc;
   ASSERT (pDoc != NULL);

   const perihelion::pVector3f XAXIS = { 1.0, 0.0, 0.0 };
   float height = pDoc->getAnthropometryRef().GetHeight( MetricCns );
   float pelvicAngle = pDoc->getPelvicAngle();
   perihelion::MDP* pHominoid = pDoc->GetHominoidModelPtr();

   perihelion::pVector3f leftFoot, rightFoot, root, intermediate;
   pHominoid->GetJointDistalByName("Left Lower Leg", leftFoot);
   pHominoid->GetJointDistalByName("Right Lower Leg", rightFoot);
   perihelion::pVectorAdd(leftFoot, rightFoot, root);
   pVector3f_SCALE(root, 0.5, root);
   pVector3f_COPY(in, intermediate);

   perihelion::pQuat beta;
   perihelion::mdpQuatFromAngleAxis( -pelvicAngle * DTOR, XAXIS, beta);

   // Treat the center of the feet as the origin, and correct for the
   // missing hip rotation.
   perihelion::pVectorSubtract(intermediate, root, intermediate);
   perihelion::pQuatRotateVector(beta, intermediate, intermediate);

   // Scale the point properly.
   pVector3f_SCALE(intermediate, 0.1 * height, out); 
}
void SkeletonToWorldConversion(perihelion::pVector3f& jointLoc, C_Hom_Doc* pDoc)
{
   float lowestAnkle;
   perihelion::pVector3f rightAnkle, leftAnkle;
   perihelion::MDP* pHominoid = pDoc->GetHominoidModelPtr();

   pHominoid->GetJointDistalByName("Right Ankle", rightAnkle);
   pHominoid->GetJointDistalByName("Left Ankle", leftAnkle);
   HominoidToWorldConversion(leftAnkle, leftAnkle, pDoc); 
   HominoidToWorldConversion(rightAnkle, rightAnkle);

   if(leftAnkle[2] < rightAnkle[2])
   {
		lowestAnkle = leftAnkle[2]; 
   }else {
		lowestAnkle = rightAnkle[2];
   }
   jointLoc[2] = jointLoc[2] + lowestAnkle;
   return;
}

void C_Hom_View::RenderGLOblique(double PelvAng)
{
   C_Hom_Doc* pDoc = GetDocument();
   perihelion::MDP* pHominoid = pDoc->GetHominoidModelPtr();
   // Don't render if the hominoid is not loaded
   if(!pDoc->HominoidLoaded())
      return;

   // Figure out the location to rotate about on the GL Hominoid

   perihelion::pVector3f hipLocation;
   perihelion::pVector3f leftFoot, rightFoot;

   pHominoid->GetJointDistalByName("Root", hipLocation);
   pHominoid->GetJointDistalByName("Left Lower Leg", leftFoot);
   pHominoid->GetJointDistalByName("Right Lower Leg", rightFoot);

   perihelion::pVector3f feetCenter;
   perihelion::pVectorAdd(leftFoot,  rightFoot,  feetCenter);
   pVector3f_SCALE(feetCenter, 0.5, feetCenter);

   perihelion::pVector3f worldFeetCenter, worldHip, rotationCenter;
   HominoidToWorldConversion(feetCenter, worldFeetCenter, pDoc);
   HominoidToWorldConversion(hipLocation, worldHip);
   rotationCenter[0] = 0;
   rotationCenter[1] = 0;
   rotationCenter[2] = worldHip[2]; // JTK - The world feet center is always  
   // zero removed - worldFeetCenter[2];

   // We want to be at the center of the hips.  Vertically, thats just the
   //    hiplocation.  Front to back, because the foot is zero in 3dsspp,
   //    and L5S1 in the glHom - we need to find the difference between these,
   //    and move that far.
   // (LCW) Edited to center the hominoid around both feet, rather than just
   //    the left, which can make rotation look strange when the legs are
   //    spread apart.
   SetProjectionMatrix(this);
   SetViewport(this);


   // SETUP THE CAMERA
   {
      C_Hom_Doc* pDoc = GetDocument();
      double Cam_Horizontal_Offset_float = 0.0;
      double Cam_Vertical_Offset_float = 0.0;
      int Cam_Distance = 20;

      // Move the camera to the correct location in the world. This is 
      // done by translating the world NEGATIVE the camera location.
      // TODO: figure out why we dont have a negative on the horizontal
      //       axis. Possibly have mirrored coordinate system?
      glTranslated( Cam_Horizontal_Offset_float / 10.0f, 
         -Cam_Vertical_Offset_float / 10.0f,
         0);

      // Now we move the camera backwards from where we're looking.
      glTranslated(0, 0, -Cam_Distance / 10.0);


      // And we rotate around this point we're looking at by the specified
      // orientations.
      glRotated(0.0, 1.0, 0.0, 0.0);
      glRotated(0.0, 0.0, -1.0, 0.0);
      glRotated(0.0, 0.0, 0.0, 1.0);


      // And because the coordinate system is different in openGL than in
      // our world, we rotate the camera to bypass this.
      glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
      glRotatef(180.0f, 0.0f, 0.0f, 1.0f);

      // Now we translate the entire camera to look at some point relative to
      // the (0,0,0) of the rendered hominoid, as specified by the user.
      glTranslatef(-rotationCenter[0], -rotationCenter[1], -rotationCenter[2]);
   }

   // Now everything will be put into the world as needed.
   glMatrixMode(GL_MODELVIEW);		
   glLoadIdentity();

   //
   // Put lights into the scene.
   //

   glEnable(GL_LIGHTING);
   glEnable(GL_BLEND);
   //glEnable(GL_NORMALIZE);  // Make sure anything with strange normals
   //    doesn't look weird
   glClearDepth(1.0f);        // Clear depth buffer to our front clipping
   //    plane depth.
   glEnable(GL_DEPTH_TEST);   
   glDepthFunc(GL_LEQUAL);  

   // JTK - Currently disabled so that the background can be rendered correctly
   //glEnable(GL_TEXTURE_2D); // Enables Texture Mapping

   // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   int lightCount;
   float lightIntensity;

   pVector4f lightpos[10];
   pVector4f lightcolor[10];

   if (GetDocument()->UsingNewLightScheme()) {
      lightCount = 3;
      lightIntensity = 1.0f * GetDocument()->GetLightIntensity() / 100.0f;

      pVector4f_SET(lightpos[0],    200.0f,  0.00f,   0.0f,    0.0f);
      pVector4f_SET(lightpos[1],    0.0f,    200.0f,  0.0f,    0.0f);
      pVector4f_SET(lightpos[2],    0.0f,    0.0,     200.0f,  0.0f);
   } else {
      lightCount = 1;
      lightIntensity = 1.6f * GetDocument()->GetLightIntensity() / 100.0f;

      pVector4f_SET(lightpos[0],
         (GetDocument()->GetOblLightTypeLR() - 1) * 200.0f,
         (GetDocument()->GetOblLightTypeFB() - 1) * -200.0f,
         (GetDocument()->GetOblLightTypeTB() - 1) * -200.0f,
         0.0f);
   }

   for(int i = 0; i < lightCount; ++i)
      pVector4f_SET(lightcolor[i],
      lightIntensity,
      lightIntensity,
      lightIntensity,
      0);

   pVector4f_SET(lightcolor[9], 0.1f, 0.1f, 0.1f, 0.0f);

   pVector4f mat_specular = {0.3f ,0.3f ,0.3f ,0.3f};
   GLfloat mat_shininess[] = {50.0f};

   glEnable(GL_NORMALIZE);
   glEnable(GL_COLOR_MATERIAL);
   for(int i = 0; i < lightCount; i++)
   {
      glEnable(GL_LIGHT0 + i);
      glLightfv(GL_LIGHT0 + i, GL_POSITION, lightpos[i]);
      glLightfv(GL_LIGHT0 + i, GL_DIFFUSE,  lightcolor[i]);
      glLightfv(GL_LIGHT0 + i, GL_AMBIENT,  lightcolor[9]);
      //glLightf(GL_LIGHT0+i, GL_CONSTANT_ATTENUATION,	0.075f);
      //glLightf(GL_LIGHT0+i, GL_LINEAR_ATTENUATION,	0.05f);
      //glLightf(GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION, 0.02f);  
      //glLightModelfv(GL_LIGHT_MODEL_AMBIENT+i, lightcolor[9]);
   }
   // Disable the rest
   for(int i = lightCount; i < 9; ++i)
      glDisable(GL_LIGHT0 + i);

   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

   ASSERT( Get_App_Ptr()->Get_Document_Ptr()->GetHominoidModelPtr() != NULL );

   if (MT_FLESH != MT_NONE) 
   {
      glPushMatrix();
      glPushAttrib(GL_POLYGON_BIT);


      // Setup some rendering modes
      if(MT_FLESH == MT_FLESH)
      {
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      } else if (MT_FLESH == MT_WIREFRAME) {
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      }

      //
      // Convert hominoid to unit height.
      //
      // TODO: add ancilary data structure to model format, store 
      // 'reference height', and use the ancilary data to scale to
      // unit height.  Right now the meshes are just 10 units tall
      // so it's hard coded.
      glScalef(0.1f, 0.1f, 0.1f);	

      float height = pDoc->getAnthropometryRef().GetHeight( MetricCns );
      //
      // Scale the model to the overall correct height.
      //
      glScalef(height, height, height);

      //
      // Convert to cm, as mm gives us precision problems
      // in the rendering of the model.
      //
      //glScalef(0.1f, 0.1f, 0.1f);	

      // Draw the actual hominoid
      const perihelion::pVector3f XAXIS = { 1.0, 0.0, 0.0 };


      perihelion::pVector3f leftFoot, rightFoot, feetCenter;
      pHominoid->GetJointDistalByName("Left Lower Leg", leftFoot);
      pHominoid->GetJointDistalByName("Right Lower Leg", rightFoot);
      perihelion::pVectorAdd(leftFoot, rightFoot, feetCenter);
      pVector3f_SCALE(feetCenter, 0.5, feetCenter);

      perihelion::pQuat beta;
      perihelion::mdpQuatFromAngleAxis( -pDoc->getPelvicAngle() * DTOR, XAXIS, beta);
      perihelion::pQuatRotateVector(beta, feetCenter, feetCenter);


      glTranslatef(-feetCenter[0], -feetCenter[1], -feetCenter[2]);
      glRotated(-pDoc->getPelvicAngle(), 1.0, 0.0, 0.0);
      pDoc->GetHominoidModelPtr()->Draw(false, true);

      glPopMatrix();
      glPopAttrib();
   }

   if (0)
   {
      // Draw red dots at all of the joint locations.  Note, needs to be set to metric, and
      // anchor points in two coordinate systems not equivalent.  The -7 on z is to correct
      // for the ankle vs. heel height difference, as our anchor point is at the center of
      // the ankles.  It rather should be a the left heel.
      glPushMatrix();
      glColor3f(1.0, 0.0, 0.0);
      glPointSize(4);
      glBegin(GL_POINTS);
      Length   lLocX( 0, 7 );
      Length   lLocY( 0, 7 );
      Length   lLocZ( 0, 7 );
      perihelion::pVector3f loc;
#define CVT { loc[0] = lLocX.Value(); loc[1] = lLocY.Value(); loc[2] = lLocZ.Value(); SkeletonToWorldConversion(loc, pDoc); glVertex3f(loc[0], loc[1], loc[2]); }
      Skeleton_Record &   lSkeleton = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name );
      lSkeleton.C4EPoint( L_HAND_CTR, lLocX, lLocY, lLocZ );	CVT	
      lSkeleton.C4EPoint( R_HAND_CTR, lLocX, lLocY, lLocZ );	CVT	
      lSkeleton.C4EPoint( L_ELBOW, lLocX, lLocY, lLocZ );		CVT	
      lSkeleton.C4EPoint( R_ELBOW, lLocX, lLocY, lLocZ );		CVT	
      lSkeleton.C4EPoint( L_SHOULDER, lLocX, lLocY, lLocZ );	CVT	
      lSkeleton.C4EPoint( R_SHOULDER, lLocX, lLocY, lLocZ );	CVT	
      lSkeleton.C4EPoint( L_HIP, lLocX, lLocY, lLocZ );		CVT	
      lSkeleton.C4EPoint( R_HIP, lLocX, lLocY, lLocZ );		CVT	
      lSkeleton.C4EPoint( L5_S1, lLocX, lLocY, lLocZ );		CVT	
      lSkeleton.C4EPoint( L_KNEE, lLocX, lLocY, lLocZ );		CVT	
      lSkeleton.C4EPoint( R_KNEE, lLocX, lLocY, lLocZ );		CVT	
      lSkeleton.C4EPoint( L_ANKLE, lLocX, lLocY, lLocZ );		CVT	
      lSkeleton.C4EPoint( R_ANKLE, lLocX, lLocY, lLocZ );		CVT	
      lSkeleton.C4EPoint( R_HEEL, lLocX, lLocY, lLocZ );		CVT	
      lSkeleton.C4EPoint( L_HEEL, lLocX, lLocY, lLocZ );		CVT	
      lSkeleton.C4EPoint( HIPS_CTR, lLocX, lLocY, lLocZ );		CVT	
      lSkeleton.C4EPoint( SEAT_REF_POINT, lLocX, lLocY, lLocZ );		CVT	
      lSkeleton.C4EPoint( CENTER_SEAT_LOCATION, lLocX, lLocY, lLocZ );		CVT	
       lSkeleton.C4EPoint( SEAT_BACK_TOP_CTR, lLocX, lLocY, lLocZ );		CVT	
       lSkeleton.C4EPoint( L_SEAT_LOCATION, lLocX, lLocY, lLocZ );		CVT
        lSkeleton.C4EPoint( R_SEAT_LOCATION, lLocX, lLocY, lLocZ );		CVT	
      glEnd();
      glPopMatrix();
   }
   glColor3ub(169,128,90);
   RenderEnvironment();

   return;
}

void C_Hom_View::RenderFloor(perihelion::pVector3f& heel, float floorWidth, 
                             float floorHeight,  float floorDepth,
                             GLubyte red, GLubyte green, 
                             GLubyte blue,				// red, green, and blue in RGB 255 possible colors
                             bool outline)
{
   C_Hom_Doc* pDoc = GetDocument();
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glDisable(GL_CULL_FACE);
   // Set the color
   glColor3ub(red, green, blue);

   if (outline)
   {
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   }
   glBegin(GL_QUADS);

   glVertex3f(heel[0]+floorWidth, heel[1]-floorHeight, heel[2]); 
   glVertex3f(heel[0]+floorWidth, heel[1]+floorHeight, heel[2]);
   glVertex3f(heel[0]-floorWidth, heel[1]+floorHeight, heel[2]);
   glVertex3f(heel[0]-floorWidth, heel[1]-floorHeight, heel[2]);

   // Sides
   glVertex3f(heel[0]+floorWidth, heel[1]-floorHeight, heel[2]); 
   glVertex3f(heel[0]+floorWidth, heel[1]+floorHeight, heel[2]);
   glVertex3f(heel[0]+floorWidth, heel[1]+floorHeight, heel[2] - floorDepth); 
   glVertex3f(heel[0]+floorWidth, heel[1]-floorHeight, heel[2] - floorDepth);

   glVertex3f(heel[0]+floorWidth, heel[1]+floorHeight, heel[2]);
   glVertex3f(heel[0]-floorWidth, heel[1]+floorHeight, heel[2]);
   glVertex3f(heel[0]-floorWidth, heel[1]+floorHeight, heel[2] - floorDepth);
   glVertex3f(heel[0]+floorWidth, heel[1]+floorHeight, heel[2] - floorDepth);

   glVertex3f(heel[0]-floorWidth, heel[1]+floorHeight, heel[2]);
   glVertex3f(heel[0]-floorWidth, heel[1]-floorHeight, heel[2]);
   glVertex3f(heel[0]-floorWidth, heel[1]-floorHeight, heel[2] - floorDepth);
   glVertex3f(heel[0]-floorWidth, heel[1]+floorHeight, heel[2] - floorDepth);

   glVertex3f(heel[0]-floorWidth, heel[1]-floorHeight, heel[2]);
   glVertex3f(heel[0]+floorWidth, heel[1]-floorHeight, heel[2]); 
   glVertex3f(heel[0]+floorWidth, heel[1]-floorHeight, heel[2] - floorDepth); 
   glVertex3f(heel[0]-floorWidth, heel[1]-floorHeight, heel[2] - floorDepth);

   // Bottom
   glVertex3f(heel[0]+floorWidth, heel[1]-floorHeight, heel[2] - floorDepth); 
   glVertex3f(heel[0]+floorWidth, heel[1]+floorHeight, heel[2] - floorDepth);
   glVertex3f(heel[0]-floorWidth, heel[1]+floorHeight, heel[2] - floorDepth);
   glVertex3f(heel[0]-floorWidth, heel[1]-floorHeight, heel[2] - floorDepth);

   glEnd();
   // Reinitialize what was changed
   glPopAttrib();
   return;
}

void C_Hom_View::RenderEnvironment()
{
   C_Hom_Doc* pDoc = GetDocument();
   perihelion::MDP* pHominoid = pDoc->GetHominoidModelPtr();

   if(!pDoc->HominoidLoaded())
      return;  // Don't render if hominoid not loaded

   perihelion::pVector3f leftAnkle, rightAnkle,
      leftUpperLeg, rightUpperLeg,
      rightHand, leftHand,
      midFront, midNormal, midPoint;

   glPushAttrib(GL_ALL_ATTRIB_BITS);
   // Make sure cull face is disabled so that the objects render correctly
   glDisable(GL_CULL_FACE);   
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   // This renders the chair for a supported calculation.  
   // The environment seat and the seated support are now the same
   if ((pDoc->getPositioning() & 1))
      // mStandingSeated 1 (true) when it is set to seated
   {
      this->RenderChair(leftAnkle, rightAnkle, midNormal, midFront);
      if (pDoc->getPositioning() & HAS_BACK)	// mSeatBack 1 (true) when the back is supported
         this->RenderChairBack();
   }
   if (false)
   {
      pHominoid->GetJointDistalByName("Left Ankle", leftAnkle);
      pHominoid->GetJointDistalByName("Right Ankle", rightAnkle);

      HominoidToWorldConversion(leftAnkle, leftAnkle, pDoc);
      HominoidToWorldConversion(rightAnkle, rightAnkle);


      if (!AreFeetLevel())
      {
         if (pDoc->getStance() != STANCE_RIGHT_FOOT
            && pDoc->getStance() != STANCE_NO_FEET)
         {
            RenderFloor(leftAnkle,  OBL_FLOOR_WIDTH, OBL_FLOOR_HEIGHT,
               OBL_FLOOR_DEPTH, 169,128,90,
               false);
            // Render the outline after the object is rendered so depth testing works correctly
            if (false)
               RenderFloor(leftAnkle,  OBL_FLOOR_WIDTH, OBL_FLOOR_HEIGHT,
               OBL_FLOOR_DEPTH, 0,0,0,true);
         }
         if (pDoc->getStance() != STANCE_LEFT_FOOT
            && pDoc->getStance() != STANCE_NO_FEET)
         {
            RenderFloor(rightAnkle, OBL_FLOOR_WIDTH, OBL_FLOOR_HEIGHT,
               OBL_FLOOR_DEPTH, 169,128,90,
               false );
            // Render hte outline after the object is rendered so depth testing works correctly
            if (false)
               RenderFloor(rightAnkle, OBL_FLOOR_WIDTH, OBL_FLOOR_HEIGHT,
               OBL_FLOOR_DEPTH,0,0,0, true);
         }
      } else {
         pHominoid->GetJointProximalByName("Left Upper Leg", leftUpperLeg);
         pHominoid->GetJointProximalByName("Right Upper Leg", rightUpperLeg);
         HominoidToWorldConversion(leftUpperLeg, leftUpperLeg, pDoc);
         HominoidToWorldConversion(rightUpperLeg, rightUpperLeg);

         // Find the midPoint of the two midpoints
         midPoint[0] = (leftUpperLeg[0] + rightUpperLeg[0])/2.0f;
         midPoint[1] = (((leftUpperLeg[1] + rightUpperLeg[1])/2.0f)
            + ((leftAnkle[1] + rightAnkle[1])/2.0f)/2.0f);
         midPoint[2] = (leftAnkle[2] + rightAnkle[2])/2.0f;
         RenderFloor(midPoint, COMBINED_OBL_FLOOR_WIDTH,
            COMBINED_OBL_FLOOR_HEIGHT, OBL_FLOOR_DEPTH,
            169,128,90, false );
         if (false)
            RenderFloor(midPoint, COMBINED_OBL_FLOOR_WIDTH,
            COMBINED_OBL_FLOOR_HEIGHT, OBL_FLOOR_DEPTH,0,0,0, true);
      }
   }
   if (HHO_NONE != HHO_NONE)
   {
	  
	  Skeleton_Record &   lSkeleton = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name );
	  Length lLocX(0, 2);
	  Length lLocY(0, 2);
	  Length lLocZ(0, 2);


	  lSkeleton.C4EPoint(L_HAND_CTR, lLocX, lLocY, lLocZ);
	  leftHand[0] = lLocX.ValueIn(MetricCns); //+ avgPoint[0];
	  leftHand[1] = lLocY.ValueIn(MetricCns); //+ avgPoint[1];
	  leftHand[2] = lLocZ.ValueIn(MetricCns);
	  SkeletonToWorldConversion(leftHand, pDoc);
	  lSkeleton.C4EPoint(R_HAND_CTR, lLocX, lLocY, lLocZ);
	  rightHand[0] = lLocX.ValueIn(MetricCns); //+ avgPoint[0];
	  rightHand[1] = lLocY.ValueIn(MetricCns); //+ avgPoint[1];
	  rightHand[2] = lLocZ.ValueIn(MetricCns);
	  SkeletonToWorldConversion(rightHand, pDoc);
      RenderHandHeldObjects(leftHand, rightHand);
   }
   glPopAttrib();
   if (pDoc->getBarType() != BT_NOBARRIER)
   {
      RenderBarrier();
   }

}

void C_Hom_View::RenderBarrier()
{
   C_Hom_Doc* pDoc = GetDocument();
   perihelion::MDP* pHominoid = pDoc->GetHominoidModelPtr();
   perihelion::pVector3f leftFoot, rightFoot, hipLocation;
   float lowestFoot;
   // Pop the matrix so the barriers render in the correct location
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glPushMatrix();

   glDisable(GL_CULL_FACE);

   if (pDoc->getBarType() != BT_NOBARRIER)
   {
      pHominoid->GetJointDistalByName("Left Ankle", leftFoot);
      pHominoid->GetJointDistalByName("Right Ankle", rightFoot);
      pHominoid->GetJointDistalByName("Root", hipLocation);

      HominoidToWorldConversion(leftFoot, leftFoot, pDoc);
      HominoidToWorldConversion(rightFoot, rightFoot);
      HominoidToWorldConversion(hipLocation, hipLocation);

      if (leftFoot[2] < rightFoot[2])
         lowestFoot = leftFoot[2];
      else
         lowestFoot = rightFoot[2];

      // Make it so the floor is measured from the lowest foot
      glTranslatef(0.0f, 0.0f, lowestFoot);
      float height = pDoc->getAnthropometryRef().GetHeight( MetricCns );

      // Convert from mm to cm
      glScaled(0.1, 0.1, 0.1);


      glRotatef(float(pDoc->getBarAzimuth()), 0.0, 0.0, 1.0); 
      glTranslatef(0.0, float(pDoc->getBarDistance()),
         float(pDoc->getBarElevation()));
      // if the barrier type is BT_WALL there are no rotations 
      if (pDoc->getBarType() == BT_WALL)
      {
         // JTK - This odd rotation and translation was done to make the shading on the 
         // wall look correct.  This is really just a a glRotatef(90.0f, 1.0f, 0.0f, 0.0f)
         // but for some reason that did not appear correctly.
         glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
         glTranslatef(0.0, -float(pDoc->getBarWidth()), 0.0 );
      } else {
         glTranslatef(0.0, 0.0, -pDoc->getBarThickness());
      }
      glScalef(float(pDoc->getBarLength()), float(pDoc->getBarWidth()),
         float(pDoc->getBarThickness()));

      glScalef(0.5, 0.5, 0.5);
      RenderBarrierCube(false);	// don't generate the outline
      if (false)
      {
         glColor3ub(0,0,0);
         RenderBarrierCube( true ); // generate the outline
      }
   }
   glPopAttrib();
   glPopMatrix();
}
void C_Hom_View::RenderHandHeldObjects(perihelion::pVector3f& leftHand,
                                       perihelion::pVector3f& rightHand)
{
   C_Hom_Doc* pDoc = GetDocument();
   double magnitudeX, magnitudeY, magnitudeZ, magnitudeXY, magnitudeXYZ,
      theta, phi;
   perihelion::pVector3f midPoint;

   // Create a quadric if one has not already been created
   if (quadric == NULL)
   {
      // Make sure this object gets destroyed at the end of C_Hom_View
      quadric = gluNewQuadric();
   }
   // Make sure depth testing is enabled
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);

   // Find the midpoint
   midPoint[0] = (rightHand[0] + leftHand[0]) / 2.0f;
   midPoint[1] = (rightHand[1] + leftHand[1]) / 2.0f;
   midPoint[2] = (rightHand[2] + leftHand[2]) / 2.0f;
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glColor3ub(169,128,90);
   // Disable culling so that the objects render correctly
   glDisable(GL_CULL_FACE);

   // Save the matrix
   glPushMatrix();
   if (HHO_NONE != HHO_CYLINDER)
      // Translate to the midpoint for the sphere and the box
      glTranslatef(midPoint[0], midPoint[1], midPoint[2]);
   else if (HHO_NONE == HHO_CYLINDER)
      glTranslatef(rightHand[0] - LOC_CORRECTION, rightHand[1], rightHand[2]);

   // Find the distances between in the hands in each direction
   magnitudeX = rightHand[0] - leftHand[0];
   if (HHO_NONE != HHO_SPHERE)
   {
      magnitudeX = magnitudeX - MAG_CORRECTION;
   } else { // Hand held type is sphere
      magnitudeX = magnitudeX - SPHERE_MAG_CORRECT;
   }

   magnitudeY = rightHand[1] - leftHand[1];
   magnitudeZ = rightHand[2] - leftHand[2];
   magnitudeXY = sqrt(pow(magnitudeX, 2) + pow(magnitudeY, 2));
   magnitudeXYZ = 
      sqrt(pow(magnitudeX, 2) + pow(magnitudeY, 2) + pow(magnitudeZ, 2));

   // Calculate the horizontal angle
   theta = atan(magnitudeY/magnitudeX);
   phi = atan(magnitudeZ/magnitudeXY);
   // Rotate the object
   if (HHO_NONE != HHO_CYLINDER) {
      glRotated(theta * 180.0f/M_PI, 0.0, 0.0, 1.0);
      glRotated(phi* 180.0f/M_PI, sin(theta), -cos(theta),0.0);
   } else {
      // The rotation is different for the cylinder because
      // it needs to be rotated about the y-axis first
      glRotated(-90, 0.0, 1.0, 0.0);
      glRotated(theta * 180.0f/M_PI, 1.0, 0.0, 0.0);
      glRotated(phi * 180.0f/M_PI, 0.0, -cos(theta), -sin(theta));
   }
   if (HHO_NONE != HHO_CYLINDER) {
      glScalef( float(magnitudeXYZ), 305 / 10.0,
         305 / 10.0);
   } else if (HHO_NONE == HHO_CYLINDER)
   {
      // An extra rotation is needed for the cylinder otherwise it would be
      //    standing straight up from the hand
      glScaled(305 / 10.0,
         305 / 10.0, magnitudeXYZ);
   }
   // JTK - Scale everything by .5.  I'm really not sure why this appears to
   //    work.  Everything needs to be in cm.
   // the hand coordinates are given in the current scaling.
   glScalef(.5, .5, .5);	

   if (HHO_NONE == HHO_SPHERE)
   {
      gluSphere(quadric,1.0, 20, 20);
      if(false)
      {
         glDepthFunc(GL_LEQUAL);
         glColor3ub(0,0,0);
         gluQuadricDrawStyle(quadric, GLU_SILHOUETTE);
         gluSphere(quadric, 1.0f, 20, 20);
         gluQuadricDrawStyle(quadric, GLU_FILL);
      }
   } else if (HHO_NONE == HHO_CYLINDER) {
      gluCylinder(quadric, 1.0, 1.0, 2.0, 20, 20);
      gluDisk(quadric, 0.0, 1.0, 20, 20);
      glTranslatef(0.0, 0.0, 2.0);
      gluDisk(quadric, 0.0, 1.0, 20, 20);
      if (false)
      {	
         glDepthFunc(GL_LEQUAL);
         glColor3ub(0,0,0);
         glTranslatef(0.0, 0.0, -2.0f);
         gluQuadricDrawStyle(quadric, GLU_SILHOUETTE);
         gluCylinder(quadric, 1.0, 1.0, 2.0, 20, 20);
         gluDisk(quadric, 0.0, 1.0, 20, 20);
         glTranslatef(0.0, 0.0, 2.0);
         gluDisk(quadric, 0.0, 1.0, 20, 20);
         gluQuadricDrawStyle(quadric, GLU_FILL);
      }
   } else if (HHO_NONE == HHO_BOX)
   {
      // Define the box to be rendered without an outline
      RenderCube(false);
      if (false)
      {
         glDepthFunc(GL_LEQUAL);
         glColor3ub(0,0,0);
         RenderCube(true);		// Render cube with outline
      }
   }
   // Restore the matrix and attributes
   glPopMatrix();
   glPopAttrib();
}

void C_Hom_View::RenderChair(perihelion::pVector3f& leftAnkle, perihelion::pVector3f& rightAnkle, perihelion::pVector3f& midNormal, perihelion::pVector3f& midFront)
{
   //perihelion::pVector3f midPoint;
   C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
   Skeleton_Record &   lSkeleton = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name );
   JointAngles currentRef = Get_App_Ptr()->Get_Document_Ptr()->getCurrentJointAnglesRef();

   Length lLocX(0,7), lLocY(0,7), lLocZ(0,7);
   perihelion::pVector3f seatRef, forwardCTR, leftFront, rightFront;
   lSkeleton.C4EPoint(SEAT_REF_POINT, lLocX, lLocY, lLocZ);
   seatRef[0] = lLocX.ValueIn(MetricCns);
   seatRef[1] = lLocY.ValueIn(MetricCns);
   seatRef[2] = lLocZ.ValueIn(MetricCns);
   
   lSkeleton.C4EPoint(CENTER_SEAT_LOCATION, lLocX, lLocY, lLocZ);
   forwardCTR[0] = lLocX.ValueIn(MetricCns);
   forwardCTR[1] = lLocY.ValueIn(MetricCns);
   forwardCTR[2] = lLocZ.ValueIn(MetricCns);
   
   lSkeleton.C4EPoint(L_SEAT_LOCATION, lLocX, lLocY, lLocZ);
   leftFront[0] = lLocX.ValueIn(MetricCns);
   leftFront[1] = lLocY.ValueIn(MetricCns);
   leftFront[2] = lLocZ.ValueIn(MetricCns);
   
   lSkeleton.C4EPoint(R_SEAT_LOCATION, lLocX, lLocY, lLocZ);
   rightFront[0] = lLocX.ValueIn(MetricCns);
   rightFront[1] = lLocY.ValueIn(MetricCns);
   rightFront[2] = lLocZ.ValueIn(MetricCns);
   SkeletonToWorldConversion(seatRef, pDoc);
   SkeletonToWorldConversion(forwardCTR, pDoc);
   SkeletonToWorldConversion(leftFront, pDoc);
   SkeletonToWorldConversion(rightFront, pDoc);
   
 
   double width = fabs(rightFront[0] - leftFront[0]);
   double lengthY = forwardCTR[1] - seatRef[1];
   double lengthZ = forwardCTR[2] - seatRef[2];
   double magnitude = sqrt(pow(lengthY,2) + pow(lengthZ, 2));
   double slope = 0;
   if(lengthY > 0.001 || lengthZ < 0.001) slope = (lengthY / lengthZ);
   double height = fabs(rightFront[2] - seatRef[2]);
   double centerX = seatRef[0];
   double centerY = 0;
   double centerZ = 0;// + (lengthZ / 2.0);
   double yAverage = (seatRef[1] + forwardCTR[1]) / 2.0;
   double zAverage = (seatRef[2] + forwardCTR[2]) / 2.0;
   double firstTerm = 0;
   if(!((forwardCTR[2] - seatRef[2]) < 0.001 && (forwardCTR[2] - seatRef[2]) > -0.001))
   {
    firstTerm = (seatRef[1] - forwardCTR[1]) / (forwardCTR[2] - seatRef[2]);
    double sign = 1;
    if((forwardCTR[1] - seatRef[1]) / (forwardCTR[2] - seatRef[2]) < 0)
    {
        sign = -1;
    }
    // Generate the two solutions then check to see which one is correct
    double xOne, yOne;
    xOne = ((sign * CHAIR_SEAT_HEIGHT) / (sqrt(1+(firstTerm*firstTerm)))) + yAverage;
    yOne = firstTerm * (sign * CHAIR_SEAT_HEIGHT / (sqrt(1 + firstTerm*firstTerm))) + zAverage;
    centerY = xOne;
    centerZ = yOne;
    /*xTwo = (((1+firstTerm*firstTerm)*2*yAverage) - sqrt(pow(((1+firstTerm*firstTerm)*2*yAverage),2) - (4 * (1+firstTerm*firstTerm) * (pow(yAverage, 2) - CHAIR_SEAT_HEIGHT*CHAIR_SEAT_HEIGHT)))) / (2 * pow(1+firstTerm,2));
    yOne = firstTerm * (xOne - yAverage) + zAverage;
    yTwo = firstTerm * (xTwo - yAverage) + zAverage;
    if(yTwo > yOne)
    {
        centerY = xTwo;
        centerZ = yTwo;
    } else {
        centerY = xOne;
        centerZ = yTwo;
    }*/
   } else {
        centerY = yAverage;
        centerZ = zAverage - CHAIR_SEAT_HEIGHT;
   }
   float yOffset = 0;			// Don't add anything if the front is not supported
   glDisable(GL_CULL_FACE);
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glPushMatrix();

   if (quadric == NULL)
      quadric = gluNewQuadric();

   double upperLegAngle = 0;
 /* perihelion::pVector3f center;
   center[0] = centerX;
   center[1] = centerY;
   center[2] = centerZ;
   SkeletonToWorldConversion(center, pDoc);*/
   //upperLegAngle -= 180;
   // Render the seat
   glTranslatef(centerX, centerY, centerZ); //- cos(atan(slope)) * CHAIR_SEAT_HEIGHT, centerZ - sin(atan(slope)) * CHAIR_SEAT_HEIGHT);
   //upperLegAngle -= 90;
   // Rotate the seat pan so that it is rotated the average of the upper leg angles
   float upperLegAngleAvg = 0;
   lSkeleton.C4EPoint(L_KNEE, lLocX, lLocY, lLocZ);
   double leftKnee = lLocZ.ValueIn(MetricCns);
   lSkeleton.C4EPoint(R_KNEE, lLocX, lLocY, lLocZ);
   double rightKnee = lLocZ.ValueIn(MetricCns);
   if(leftKnee < rightKnee)
   {
        upperLegAngle = currentRef.UpperlegLV(); 
   } else {
        upperLegAngle = currentRef.UpperlegRV();
   }
   upperLegAngle -= 180;
   
   glRotatef(upperLegAngle, 1.0f, 0.0f, 0.0f);

   glScalef(width, (magnitude)/ 2.0, CHAIR_SEAT_HEIGHT);

   gluCylinder(quadric, 1.0, 1.0, 1.0, 32, 32);
   gluDisk(quadric, 0.0, 1.0, 32, 32);
   glTranslatef(0.0f, 0.0f, 1.0f);
   gluDisk(quadric, 0.0, 1.0, 32, 32);

   glPopMatrix();
   glPopAttrib();
}

void C_Hom_View::RenderChairSupport(float x,
                                    float y,
                                    float z,
                                    float seatLocation,
                                    bool chairBack)
{
   glPushMatrix();
   //glTranslated(leftAnkle[0] - .25, midPoint[1] - 0.45, leftAnkle[2]);
   glTranslated(x, y, z);
   JointAngles currentRef = Get_App_Ptr()->Get_Document_Ptr()->getCurrentJointAnglesRef();
   if (currentRef.TrunkFlexion() > 90 && chairBack )
   {
      glRotatef(currentRef.TrunkFlexion() - 90.0f, 1.0f, 0.0f, 0.0f);
   }
   glScalef(CHAIR_LEG_WIDTH, CHAIR_LEG_DEPTH, seatLocation);
   gluCylinder(quadric, 1.0, 1.0, 1.0, 32, 32);
   gluDisk(quadric, 0.0, 1.0, 32, 32);
   glTranslatef(0.0f, 0.0f, 1.0f);
   gluDisk(quadric, 0.0, 1.0, 32, 32);
   glPopMatrix();
}

void C_Hom_View::RenderChairBack()
{	
   perihelion::pVector3f midPoint;
   C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
   JointAngles currentRef = Get_App_Ptr()->Get_Document_Ptr()->getCurrentJointAnglesRef();
   //float yOffset =  OBL_FLOOR_DEPTH + CHAIR_BACK_OFFSET;
   Skeleton_Record &   lSkeleton = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name );
   Length lLocX(0,7), lLocY(0,7), lLocZ(0,7);
   /*if (pDoc->getPositioning() & HAS_FRONTSUPPORT) 
      yOffset += FRONT_SUPPORT_YOFFSET;*/	// 0.5f
   lSkeleton.C4EPoint(SEAT_BACK_TOP_CTR, lLocX, lLocY, lLocZ);
   midPoint[0] = lLocX.ValueIn(MetricCns);
   midPoint[1] = lLocY.ValueIn(MetricCns);
   midPoint[2] = lLocZ.ValueIn(MetricCns);
   lSkeleton.C4EPoint(SEAT_REF_POINT, lLocX, lLocY, lLocZ);

   glPushMatrix();
   double angle = currentRef.TrunkFlexion() - 90.0f;
   SkeletonToWorldConversion(midPoint, pDoc);
   glTranslatef(midPoint[0], midPoint[1], midPoint[2]); 

   //if (currentRef.TrunkFlexion() > 90) // don't rotate forward of vertical
   glRotatef(angle, 1.0f, 0.0f, 0.0f);

   glTranslatef(0, 0, -CHAIR_BACK_HEIGHT);
   
   glRotatef(currentRef.TrunkAxialRotation(), 0.0f, 0.0f, 1.0f);

  
  
   double temp = (pDoc->getSeatHeight().ValueIn(MetricCns));
   //glTranslatef(0.0, 0.0, BLOCK_SEAT_OFFSET + (pDoc->getSeatHeight().ValueIn(MetricCns)));
   glScalef(CHAIR_SEAT_WIDTH, CHAIR_BACK_DEPTH, CHAIR_BACK_HEIGHT);	// The hips are scaled by .5 and scaled by another .5 because the cube is 2x2x2 so .25 
   //glPolygonMode(GL_FRONT, GL_FILL);
   glDisable(GL_CULL_FACE);
   RenderCube( false ); // turn outline off, but turn shading on and chair fix
   glPopMatrix();
}
