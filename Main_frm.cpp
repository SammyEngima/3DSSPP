// main_frm.cpp : implementation of the C_Main_Frame class
//

#include "stdafx.h"
#include "main_frm.h"

//#include <occimpl.h>
//#include <afxstat_.h>
#include "../src/mfc/winhand_.h"
#include <multimon.h>
#include <atlacc.h>	// Accessible Proxy from ATL
#include "sal.h"
#include "hom.h"
#include "hom_doc.h"	// Necessary

#include "dibview.h"

#include "ReportAnalysisSummary.h" 
#include "ReportAnthropometry.h"
#include "ReportBalanceUnified.h"
#include "ReportBalanceStanding.h"
#include "ReportFatigue.h"
#include "ReportPosture.h"
#include "ReportJointLocations.h"
#include "ReportCoMLocations.h"
#include "ReportJointMoments.h"
#include "ReportJointForces.h"
#include "ReportLowBack.h"
#include "ReportShoulderAnalysis.h"
#include "ReportShoulderGeometry.h"
#include "ReportSagittal.h"
#include "ReportSpinal.h"
#include "ReportStrength.h"
#include "ReportStrengthDirVectors.h"
#include "ReportTaskInputSummary.h"
#include "ReportBalanceSeated.h"
#include "GLView.h"
#include "DIB.h"
#include "DummyFrame.h"
#include "DummyView.h"

#include ".\main_frm.h"

#include <string>

using std::string;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C_Main_Frame

IMPLEMENT_DYNAMIC(C_Main_Frame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(C_Main_Frame, CMDIFrameWnd)
   //{{AFX_MSG_MAP(C_Main_Frame)
   ON_WM_CREATE()
   ON_COMMAND(ID_REPORTS_JOINT_LOCATIONS, OnReportsJointLocations)
   ON_COMMAND(ID_REPORTS_CENTEROFMASSLOCATIONS, OnReportsCoMLocations)
   ON_COMMAND(ID_REPORTS_3D_LOWBACK, OnReports3dLowBack)
   ON_COMMAND(ID_REPORTS_SHOULDERANALYSIS, OnReportsShoulderAnalysis)
   ON_COMMAND(ID_REPORTS_SHOULDERGEOMETRY, OnReportsShoulderGeometry)
   ON_COMMAND(ID_REPORTS_ANTHROPOMETRY, OnReportsAnthropometry)
   ON_COMMAND(ID_REPORTS_SPINAL, OnReportsSpinal)
   ON_COMMAND(ID_REPORTS_SAGITTAL, OnReportsSagittal)
   ON_COMMAND(ID_REPORTS_STRENGTH, OnReportsStrength)
   ON_COMMAND(ID_REPORTS_ANALYSIS_SUMMARY, OnReportsAnalysisSummary)
   ON_WM_SIZE()
   ON_WM_CLOSE()
   ON_COMMAND(ID_REPORTS_TASK_SUMMARY, OnReportsTaskSummary)
   ON_COMMAND(ID_FILE_PRINTAPPLICATION, OnFilePrintApplication)
   ON_COMMAND(ID_FILE_PRINTDESKTOP, OnFilePrintDesktop)
   ON_COMMAND(ID_FILE_COPY_APPLICATION, OnCopyApplication)
   ON_COMMAND(ID_CopyDesktop, OnCopyDesktop)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_ANALYSIS_SUMMARY, OnUpdateReportsSummary)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_ANTHROPOMETRY, OnUpdateReportsAnthropometry)
   ON_COMMAND(ID_REPORTS_POSTURE, OnReportsPosture)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_TASK_SUMMARY, OnUpdateReportsTaskinputsummary)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_STRENGTH, OnUpdateReportsStrength)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_POSTURE, OnUpdateReportsPosture)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_JOINT_LOCATIONS, OnUpdateReportsMoments)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_CENTEROFMASSLOCATIONS, OnUpdateReportsMoments)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_SAGITTAL, OnUpdateReportsL5s1)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_3D_LOWBACK, OnUpdateReports3dback)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_SHOULDERANALYSIS, OnUpdateReportsShoulderAnalysis)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_SHOULDERGEOMETRY, OnUpdateReportsShoulderGeometry)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_SPINAL, OnUpdateReportsL23l44)
   ON_COMMAND(ID_REPORTS_JOINT_MOMENTS, OnReportsJointMoments)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_JOINT_MOMENTS, OnUpdateReportsJointMoments)
   ON_COMMAND(ID_REPORTS_STRENGTH_DIR_VECTORS, OnReportsStrengthDirVectors)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_STRENGTH_DIR_VECTORS, OnUpdateReportsStrengthDirVectors)
   ON_COMMAND(ID_REPORTS_BALANCE_SEATED, OnReportsBalanceSeated)
   ON_COMMAND(ID_REPORTS_BALANCE_STANDING, OnReportsBalanceStanding)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_BALANCE_SEATED, OnUpdateReportsBalanceSeated)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_BALANCE_STANDING, OnUpdateReportsBalanceStanding)
   ON_COMMAND(ID_REPORTS_JOINT_FORCES, OnReportsJointForces)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_JOINT_FORCES, OnUpdateReportsJointforces)
   ON_COMMAND(ID_REPORTS_FATIGUE, OnReportsFatigue)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_FATIGUE, OnUpdateReportsFatigue)
   ON_WM_DESTROY()
   ON_WM_SYSCOMMAND()
   //}}AFX_MSG_MAP
   ON_COMMAND(ID_REFRESH_ALL, OnRefreshAll)
   ON_COMMAND(ID_REPORTS_BALANCE, &C_Main_Frame::OnReportsBalance)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_BALANCE, &C_Main_Frame::OnUpdateReportsBalance)
   ON_WM_KEYDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars

// toolbar buttons - IDs are command buttons
static UINT BASED_CODE buttons[] =
{
   // same order as in the bitmap 'toolbar.bmp'
   ID_FILE_NEW,
      ID_FILE_OPEN,
      ID_FILE_SAVE,
      ID_SEPARATOR,
      ID_EDIT_CUT,
      ID_EDIT_COPY,
      ID_EDIT_PASTE,
      ID_SEPARATOR,
      ID_FILE_PRINT,
      ID_APP_ABOUT,
};

static UINT BASED_CODE indicators[] =
{
   ID_SEPARATOR //,           // status line indicator
      //	ID_INDICATOR_CAPS,
      //	ID_INDICATOR_NUM,
      //	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// C_Main_Frame construction/destruction

//---------------------------------------------------------------------------
C_Main_Frame::C_Main_Frame() : mStatusBarExists(FALSE)
{
   // lSummaryFramePtr = NULL;
	printDib = 0;
	dummyView = 0;
   return;
}

//---------------------------------------------------------------------------
C_Main_Frame::~C_Main_Frame()
{
	delete printDib;
   return;
}

//---------------------------------------------------------------------------
int C_Main_Frame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   int lSuccess = 0;

   if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
      lSuccess = -1;

   if (lSuccess == 0)
      lSuccess = this->CreateStatusBar();   

   return lSuccess;
}

//---------------------------------------------------------------------------
int C_Main_Frame::CreateStatusBar()
//NOTE: If status bar already exists, do nothing.
{
   int lSuccess = 0;

   if( ! mStatusBarExists )
   {   
      if (
         !m_wndStatusBar.Create(this) ||
         !m_wndStatusBar.SetIndicators
         (
         indicators,  
         sizeof(indicators)/sizeof(UINT)
         )
         )
      {
         TRACE("Failed to create status bar\n");
         assert( 0 );
         mStatusBarExists = FALSE;
         lSuccess = -1;
      }

      else
      {
         mStatusBarExists = TRUE;
         lSuccess = 0; //windows defines 0 to mean continue creation process
         this->RecalcLayout( FALSE );
      }
   }

   else
   {
      lSuccess = TRUE;
   }

   return   lSuccess;
}

//---------------------------------------------------------------------------
void  C_Main_Frame::StatusBarText( const string& aNewText )
{
   if (mStatusBarExists)
   {
      m_wndStatusBar.SetWindowText( aNewText.c_str() );
      m_wndStatusBar.UpdateWindow();
   }
   return; 
}

//---------------------------------------------------------------------------
BOOL C_Main_Frame::DestroyStatusBar()
//NOTE: If status bar already destroyed, do nothing.
{
   BOOL   lDestroyed = FALSE;

   if( mStatusBarExists )
   {
      if( m_wndStatusBar.DestroyWindow() )
      {
         mStatusBarExists = FALSE;
         lDestroyed = TRUE;
         this->RecalcLayout( FALSE );
      }
   }
   else
   {
      lDestroyed = TRUE;
   }   

   return   lDestroyed;
}

//---------------------------------------------------------------------------
void C_Main_Frame::SynchronizeStatusBar()
{
   C_Hom_Doc* Doc_Ptr = Get_App_Ptr()->Get_Document_Ptr();

   if( Doc_Ptr->StatusBar() ) // Need to activate the status bar
   {
      if( this->CreateStatusBar() == 0 ) //successfully created
      {
         if(Doc_Ptr->AutoTile())
            this->TileChildren();
         else
            this->UpdateMainTitleBar();
      }
   }
   else // Need to deactivate the status bar
   {
      if( this->DestroyStatusBar() ) //successfully destroyed
      {
         if(Doc_Ptr->AutoTile())
            this->TileChildren();
         else
            this->UpdateMainTitleBar();
      }
   }

   return;
}

/*------------------------------------------------------------*/
BOOL C_Main_Frame::PreCreateWindow(CREATESTRUCT& CS)
{
   CMDIFrameWnd::PreCreateWindow( CS );

   CS.style &= ~(long)FWS_ADDTOTITLE;  //Set NO auto label per MS document Q99182
   CS.style |= (WS_VSCROLL | WS_HSCROLL);

   return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// C_Main_Frame diagnostics

#ifdef _DEBUG
void C_Main_Frame::AssertValid() const
{
   CMDIFrameWnd::AssertValid();
}

void C_Main_Frame::Dump(CDumpContext& dc) const
{
   CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// C_Main_Frame Support

//---------------------------------------------------------------------------
int C_Main_Frame::GetClientHeight()
{
   RECT   lClientRectangle;
   ::GetClientRect(this->m_hWndMDIClient, &lClientRectangle);
   return lClientRectangle.bottom - lClientRectangle.top;
}

//---------------------------------------------------------------------------
int C_Main_Frame::GetClientWidth()
{
   RECT   lClientRectangle;
   ::GetClientRect(this->m_hWndMDIClient,&lClientRectangle);
   return lClientRectangle.right - lClientRectangle.left;
}

/////////////////////////////////////////////////////////////////////////////
// C_Main_Frame message handlers

//#include "hom_doc.h"	// Unnecessary!

//---------------------------------------------------------------------------
void C_Main_Frame::UpdateMainTitleBar()
{
   C_Hom_App* lAppPtr = Get_App_Ptr();
   C_Hom_Doc* lDocPtr = lAppPtr->Get_Document_Ptr();

   //--------------------------------sets new icon to main frame---------------
   // TODO this used to depend on task mode
   this->SetIcon(AfxGetApp()->LoadIcon(IDI_3DDSmall),true);
   //----------------------------------------------------------------------------

   String Title(lAppPtr->ApplicationName());
   Title = Title + " - " + (const char*)(lDocPtr->GetTitle());

   // Put in the "modified" asterisk if necessary
   if(lDocPtr->IsModified())
      Title += "*";

   this->SetWindowText((char*)Title);
}

void C_Main_Frame::OnPrint(CDC* pDC, CPrintInfo* pInfo) {
	printDib->RenderToDC(pDC);
}

//---------------------------------------------------------------------------
void C_Main_Frame::Update_All_Title_Bars()
{
   C_Hom_Doc* Doc_Ptr = Get_App_Ptr()->Get_Document_Ptr();

   this->UpdateMainTitleBar();

   this->SendMessageToDescendants( WM_UPDATE_TITLE_BAR );

   return;
}


//---------------------------------------------------------------------------
void C_Main_Frame::OnReportsTaskSummary()
//Task Input/Posture Summary
{
   OpenReport(ID_REPORTS_TASK_SUMMARY);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnReportsAnalysisSummary()
{
   OpenReport(ID_REPORTS_ANALYSIS_SUMMARY);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnReportsAnthropometry()
{
   OpenReport(ID_REPORTS_ANTHROPOMETRY);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnReportsPosture() 
{
   OpenReport(ID_REPORTS_POSTURE);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnReportsJointLocations()
{
   OpenReport(ID_REPORTS_JOINT_LOCATIONS);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnReportsCoMLocations()
{
   OpenReport(ID_REPORTS_CENTEROFMASSLOCATIONS);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnReportsJointMoments()
{
   OpenReport(ID_REPORTS_JOINT_MOMENTS);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnReports3dLowBack()
// 3D Lowback Linear Optimization Analysis
{
   OpenReport(ID_REPORTS_3D_LOWBACK);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnReportsShoulderAnalysis()
// 3D Lowback Linear Optimization Analysis
{
   OpenReport(ID_REPORTS_SHOULDERANALYSIS);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnReportsShoulderGeometry()
// Shoulder Geometry Analysis
{
   OpenReport(ID_REPORTS_SHOULDERGEOMETRY);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnReportsSpinal()
// Formerly OnReportsL23l44
{
   OpenReport(ID_REPORTS_SPINAL);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnReportsSagittal()
//	Sagittal Plane Lowback Analysis
// Formerly OnReportsL5s1
{
   OpenReport(ID_REPORTS_SAGITTAL);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnReportsStrength()
{
   OpenReport(ID_REPORTS_STRENGTH);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnReportsFatigue()
{
   OpenReport(ID_REPORTS_FATIGUE);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnReportsStrengthDirVectors() 
{
   OpenReport(ID_REPORTS_STRENGTH_DIR_VECTORS);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnReportsBalanceSeated() 
{
   OpenReport(ID_REPORTS_BALANCE_SEATED);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnReportsBalanceStanding() 
{
   OpenReport(ID_REPORTS_BALANCE_STANDING);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnReportsJointForces() 
{
   OpenReport(ID_REPORTS_JOINT_FORCES);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnReportsBalance()
{
	OpenReport(ID_REPORTS_BALANCE);
}
//---------------------------------------------------------------------------
CFrameWnd* C_Main_Frame::OpenReport(int aReportType)
{
   C_Hom_Doc* lDocPtr = Get_App_Ptr()->Get_Document_Ptr();
   CFrameWnd* lFramePtr = NULL;
   CMultiDocTemplate* lTemplate;

   // Set the template to the correct report type
   lTemplate = Get_App_Ptr()->mReportTemplateMap[aReportType];

   // Create and update the report frame
   lFramePtr = lTemplate->CreateNewFrame(lDocPtr, NULL);
   assert(lFramePtr);

   lTemplate->InitialUpdateFrame(lFramePtr, lDocPtr, TRUE);

   return lFramePtr;
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnSize(UINT nType, int cx, int cy)
//Sends user defined message to child windows so they can fit themselves 
//neatly into this window provided autotile is set in the active document.
{

   CMDIFrameWnd::OnSize(nType, cx, cy);

   // Recompute window positions if AutoTile is set
   C_Hom_Doc* lDocPtr = Get_App_Ptr()->Get_Document_Ptr();
   if(!lDocPtr)
      return;	// No document?  Don't tile!

   if( lDocPtr->AutoTile() )
   {
      this->TileChildren();
   }
   else //just set the main window title
   {
      this->UpdateMainTitleBar();
   }

   return;
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnClose()
//We must intercept this message and close the document ourselves since we
//are not using the document manager object.
{
   C_Hom_App* App_Ptr = Get_App_Ptr();

   if(App_Ptr->Document_Exists)
   {
      if (App_Ptr->Get_Document_Ptr()->IsModified())
      {
         int lBoxAnswer = 
            AfxMessageBox("Save changes to current task?",
            MB_YESNOCANCEL);

         if(lBoxAnswer == IDCANCEL)
            return;
         else if(lBoxAnswer == IDYES)
         {
            App_Ptr->Get_Document_Ptr()->DoSave();
            if(App_Ptr->Get_Document_Ptr()->IsModified())
               return;	// Operation cancelled
         }
      }
   }

   // remove the document

   HWND  lHdlPtr = m_hWnd;

   ASSERT(Get_App_Ptr()->Document_Exists);

   C_Hom_Doc* lDocPtr = Get_App_Ptr()->Get_Document_Ptr();
   lDocPtr->DoClose();

   // ACT - We need to check, did it actually close the document or not???
   // If it did, call CFrameWnd::OnClose(); after deleting the document container
   // If it didn't, return; without closing.

   if (!lDocPtr->ViewsExist())
   {
      // Document deletes itself automatically in OnFileClose
      delete lDocPtr;
      lDocPtr = NULL;

      CFrameWnd::OnClose();
   }
   
   //CMemoryState ms;
   //ms.DumpAllObjectsSince();

   return;
}

//---------------------------------------------------------------------------
void C_Main_Frame::TileChildren()
//Sends user defined message to child windows so they can fit themselves 
//neatly into this window.
{
   // Account for maximized/minimized windows
   //this->SendMessageToDescendants( WM_SYSCOMMAND, SC_RESTORE );

   this->SendMessageToDescendants( WM_UPDATE_POSITION );

   //Update this main MDI frame title since it
   //was changed by the Quadrant_Frame calls to
   //their CWnd::MoveWindow functions.
   this->UpdateMainTitleBar();

   return;
}

//---------------------------------------------------------------------------
CDibView* C_Main_Frame::Create_Dib_Window()
{                              //Save current active window
   CMDIChildWnd* Active_Child_Ptr = MDIGetActive();
   CDocument*   Document_Ptr;

   if ( Active_Child_Ptr != NULL  )
   {
      //active document
      Document_Ptr = Active_Child_Ptr->GetActiveDocument(); 
      if( ! Document_Ptr )
      {
         AfxMessageBox( "C_Main_Frame::On_Create_DIB_Frame: No document found." );
         return   NULL;         
      }
   }
   else //No active child window
   {
      AfxMessageBox( "C_Main_Frame::On_Create_DIB_Frame: No active child window." );
      return   NULL;
   }    

   //Get document template pointer
   CDocTemplate* Template_Ptr = static_cast<C_Hom_App*>(AfxGetApp())->Dib_DocTemplate_Ptr;

   ASSERT_VALID( Template_Ptr );
   //creates frame in 
   Frame_Ptr = (CMDIChildWnd*)( Template_Ptr->CreateNewFrame( NULL, NULL ) );
   if (Frame_Ptr == NULL)
   {
      AfxMessageBox( "C_Main_Frame::On_Create_DIB_Frame: Unable to create frame." );
      return  NULL;
   }
   //NOTE: FALSE in the following call causes the 
   //the frame and its view to not be visible.
   Template_Ptr->InitialUpdateFrame( Frame_Ptr, NULL, FALSE );

   CDibView*   View_Ptr = (CDibView*)( Frame_Ptr->GetActiveView() );
   if( View_Ptr == NULL )
   {
      AfxMessageBox( "C_Main_Frame::On_Create_DIB_Frame: Can not activate DIB view." );
      return   NULL;
   }

   return   View_Ptr;
}


//---------------------------------------------------------------------------
void C_Main_Frame::OnFilePrintDesktop()
{
	if(!dummyView) {
		// create dummy view
		CFrameWnd* frame = Get_App_Ptr()->mDummyTemplatePtr->CreateNewFrame(NULL,NULL);
		Get_App_Ptr()->mDummyTemplatePtr->InitialUpdateFrame(frame, NULL, FALSE);
		dummyView = (CDummyView*)frame->GetActiveView();
	}

	if(printDib) {
		delete printDib;
		printDib = 0;
	}
	printDib = new DIB();
	dummyView->FakeOnFilePrint();

//	CFormView::OnFilePrint();
	/*
   CDibView*   View_Ptr = this->Create_Dib_Window();   
   if( View_Ptr ) 
   {
      View_Ptr->PostMessage( WM_PRINT_DESKTOP );
   }

   return;*/
}


//---------------------------------------------------------------------------
void C_Main_Frame::OnFilePrintApplication()
{
	
	if(!dummyView) {
		// create dummy view
		CFrameWnd* frame = Get_App_Ptr()->mDummyTemplatePtr->CreateNewFrame(NULL,NULL);
		Get_App_Ptr()->mDummyTemplatePtr->InitialUpdateFrame(frame, NULL, FALSE);
		dummyView = (CDummyView*)frame->GetActiveView();
	}
	if(printDib) {
		delete printDib;
		printDib = 0;
	}
	//printDib = new DIB(this);
	printDib = new DIB;
	printDib->GrabWindow(this);
	dummyView->FakeOnFilePrint();

	//CFormView::OnFilePrint();
	/*
   CDibView*   View_Ptr = this->Create_Dib_Window();
   if( View_Ptr ) 
   {
      View_Ptr->PostMessage( WM_PRINT_APPLICATION );
   }

   return;*/
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnCopyApplication()
{
	// TODO printing
	//DIB dib(this);
	DIB dib;
	dib.GrabWindow(this);
	dib.PutOnClipboard();
	/*
   CDibView*   View_Ptr = this->Create_Dib_Window();
   View_Ptr->CopyApplicationToClipboard();
   delete Frame_Ptr;
   return;*/
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnCopyDesktop() 
{
	// TODO printing
	/*
   CDibView*   View_Ptr = this->Create_Dib_Window();
   View_Ptr->CopyDesktopToClipboard();
   delete Frame_Ptr;
   return;	*/
	DIB dib;
	dib.PutOnClipboard();
}

//---------------------------------------------------------------------------
// TODO remove
void C_Main_Frame::Refresh_Corrupted_Window(CWnd* IconWindowPtr)
//Checks to see if the provided icon window has corrupted one of this MDI 
//frame's child windows. If so, the corrupted window is marked and repainted. 
{
   ASSERT_VALID( IconWindowPtr );

   WINDOWPLACEMENT     Overlying_Window_Placement;
   Overlying_Window_Placement.length = sizeof( Overlying_Window_Placement );

   if(  IconWindowPtr->GetWindowPlacement( &Overlying_Window_Placement )  )
   {
      CWnd*   
         Corrupted_Window_Ptr = 
         this->ChildWindowFromPoint( Overlying_Window_Placement.ptMinPosition );

      if( Corrupted_Window_Ptr != this && Corrupted_Window_Ptr != NULL ) 
      {
         Corrupted_Window_Ptr->Invalidate();
         Corrupted_Window_Ptr->UpdateWindow();
      }
   }

   else  AfxMessageBox
      ( "Refresh_Corrupted_Window: Unable to get overlying window placement." );      

   return;
}

/////////////////////////////////////////////////////////////////////////////
// Menu update message handlers
//---------------------------------------------------------------------------
void C_Main_Frame::PrintReportOnDemand
(
 void         (C_Main_Frame::*aCreateReportFctPtr )( void ) ,
 BOOL         (              *aIsOpenFctPtr )      ( void ) ,
 ReportView * (              *aPointerFctPtr)      ( void )
 )
{
   BOOL   lAlreadyOpen = TRUE;

   if( ! (*aIsOpenFctPtr)() ) //not open, create one.
   {
      (this->*aCreateReportFctPtr)();
      lAlreadyOpen = FALSE;
   }

   ReportView * lPointer = (*aPointerFctPtr)();
   ASSERT( lPointer != NULL );

   lPointer->PrintDirect(); //print the thing

   if( ! lAlreadyOpen )
   {
      lPointer->Close();
   }

   return;
}

//---------------------------------------------------------------------------
void C_Main_Frame::PrintReports(BOOL aReportAnalysisSummary,
BOOL aReportAnthropometry,
BOOL aReportBalance,
//BOOL aReportBalanceStanding,
//BOOL aReportBalanceSeated,
BOOL aReportFatigue,
BOOL aReportPosture,
BOOL aReportJointLocations,
BOOL aReportCoMLocations,
BOOL aReportJointMoments,
BOOL aReportJointForces,
BOOL aReportLowBack,
BOOL aReportShoulderAnalysis,
BOOL aReportShoulderGeometry,
BOOL aReportSagittal,
//BOOL aReportSpinal,
BOOL aReportStrength,
BOOL aReportStrengthDirVectors,
BOOL aReportTaskInputSummary)
{
if(aReportTaskInputSummary)
{
    this->PrintReportOnDemand(&C_Main_Frame::OnReportsTaskSummary ,
    & ReportTaskInputSummary::IsOpen ,
    & ReportTaskInputSummary::Pointer
    );
}
//---
if( aReportAnalysisSummary )
{
    this->PrintReportOnDemand
    (
    & C_Main_Frame::OnReportsAnalysisSummary ,
    & ReportAnalysisSummary::IsOpen ,
    & ReportAnalysisSummary::Pointer
    );
}
//---
if ( aReportFatigue )
{
    this->PrintReportOnDemand
    (
    & C_Main_Frame::OnReportsFatigue,
    & ReportFatigue::IsOpen,
    & ReportFatigue::Pointer
    );
}
//---
if( aReportLowBack )
{
    this->PrintReportOnDemand
    (
    & C_Main_Frame::OnReports3dLowBack ,
    & ReportLowBack::IsOpen ,
    & ReportLowBack::Pointer
    );
}
//---
if( aReportShoulderAnalysis )
{
    this->PrintReportOnDemand
    (
    & C_Main_Frame::OnReportsShoulderAnalysis ,
    & ReportShoulderAnalysis::IsOpen ,
    & ReportShoulderAnalysis::Pointer
    );
}

//---
if( aReportShoulderGeometry )
{
   this->PrintReportOnDemand
   (
   & C_Main_Frame::OnReportsShoulderGeometry , 
   & ReportShoulderGeometry::IsOpen ,
   & ReportShoulderGeometry::Pointer
   );
}
//---
if( aReportSagittal )
{
    this->PrintReportOnDemand
    (
    & C_Main_Frame::OnReportsSagittal ,
    & ReportSagittal::IsOpen ,
    & ReportSagittal::Pointer
    );
}
//---
if( aReportStrength ) //Capabilities
{
    this->PrintReportOnDemand
    (
    & C_Main_Frame::OnReportsStrength ,
    & ReportStrength::IsOpen ,
    & ReportStrength::Pointer
    );
}
//---
if( aReportAnthropometry )
{
    this->PrintReportOnDemand
    (
    & C_Main_Frame::OnReportsAnthropometry ,
    & ReportAnthropometry::IsOpen ,
    & ReportAnthropometry::Pointer
    );
}
//---
/*if( aReportBalanceSeated )
{
this->PrintReportOnDemand
(
& C_Main_Frame::OnReportsBalanceSeated,
& ReportBalanceSeated::IsOpen ,
& ReportBalanceSeated::Pointer
);
}
//---
if( aReportBalanceStanding )
{
this->PrintReportOnDemand
(
& C_Main_Frame::OnReportsBalanceStanding,
& ReportBalanceStanding::IsOpen ,
& ReportBalanceStanding::Pointer
);
}*/
if( aReportBalance ) {
    this->PrintReportOnDemand
    (
    & C_Main_Frame::OnReportsBalance,
    & ReportBalanceUnified::IsOpen,
    & ReportBalanceUnified::Pointer
    );
}
//---
if( aReportJointForces )
{
    this->PrintReportOnDemand
    (
    & C_Main_Frame::OnReportsJointForces ,
    & ReportJointForces::IsOpen ,
    & ReportJointForces::Pointer
    );
}
//---
if( aReportJointLocations )
{
    this->PrintReportOnDemand
    (
    & C_Main_Frame::OnReportsJointLocations ,
    & ReportJointLocations::IsOpen ,
    & ReportJointLocations::Pointer
    );
}
//---
if( aReportCoMLocations )
{
    this->PrintReportOnDemand
    (
    & C_Main_Frame::OnReportsCoMLocations ,
    & ReportCoMLocations::IsOpen ,
    & ReportCoMLocations::Pointer
    );
}
//---
if( aReportJointMoments )
{
    this->PrintReportOnDemand
    (
    & C_Main_Frame::OnReportsJointMoments ,
    & ReportJointMoments::IsOpen ,
    & ReportJointMoments::Pointer
    );
}
//---
if( aReportPosture )
{
    this->PrintReportOnDemand
    (
    & C_Main_Frame::OnReportsPosture ,
    & ReportPosture::IsOpen ,
    & ReportPosture::Pointer
    );
}
//---
/*if( aReportSpinal )
{
this->PrintReportOnDemand
(
& C_Main_Frame::OnReportsSpinal ,
& ReportSpinal::IsOpen ,
& ReportSpinal::Pointer
);
}*/
//---
if( aReportStrengthDirVectors )
{
    this->PrintReportOnDemand
    (
    & C_Main_Frame::OnReportsStrengthDirVectors ,
    & ReportStrengthDirVectors::IsOpen ,
    & ReportStrengthDirVectors::Pointer
    );
}
return;
}   

//---------------------------------------------------------------------------
void C_Main_Frame::OnUpdateReportsSummary(CCmdUI* pCmdUI) 
{
   if( ReportAnalysisSummary::IsOpen() )
   {
      pCmdUI->Enable( FALSE );
   }
   else
   {
      pCmdUI->Enable( TRUE );
   }
   return;
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnUpdateReportsAnthropometry(CCmdUI* pCmdUI) 
{
   if( ReportAnthropometry::IsOpen() )
   {
      pCmdUI->Enable( FALSE );
   }
   else
   {
      pCmdUI->Enable( TRUE );
   }
   return;
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnUpdateReportsFatigue(CCmdUI* pCmdUI) 
{
   if( ReportFatigue::IsOpen() )
   {
      pCmdUI->Enable( FALSE );
   }
   else
   {
      pCmdUI->Enable( TRUE );
   }
   return;
}

void C_Main_Frame::OnUpdateReportsTaskinputsummary(CCmdUI* pCmdUI) 
{
   if( ReportTaskInputSummary::IsOpen() )
   {
      pCmdUI->Enable( FALSE );
   }
   else
   {
      pCmdUI->Enable( TRUE );
   }
   return;
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnUpdateReportsStrength(CCmdUI* pCmdUI) 
{
   if( ReportStrength::IsOpen() )
   {
      pCmdUI->Enable( FALSE );
   }
   else
   {
      pCmdUI->Enable( TRUE );
   }
   return;
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnUpdateReportsPosture(CCmdUI* pCmdUI) 
{
   if( ReportPosture::IsOpen() )
   {
      pCmdUI->Enable( FALSE );
   }
   else
   {
      pCmdUI->Enable( TRUE );
   }
   return;
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnUpdateReportsMoments(CCmdUI* pCmdUI) 
{
   if( ReportJointLocations::IsOpen() )
   {
      pCmdUI->Enable( FALSE );
   }
   else
   {
      pCmdUI->Enable( TRUE );
   }
   return;
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnUpdateReportsL5s1(CCmdUI* pCmdUI) 
{
   if( ReportSagittal::IsOpen() )
   {
      pCmdUI->Enable( FALSE );
   }
   else
   {
      pCmdUI->Enable( TRUE );
   }
   return;
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnUpdateReports3dback(CCmdUI* pCmdUI) 
{
   if( ReportLowBack::IsOpen() )
   {
      pCmdUI->Enable( FALSE );
   }
   else
   {
      pCmdUI->Enable( TRUE );
   }
   return;
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnUpdateReportsShoulderAnalysis(CCmdUI* pCmdUI) 
{
    if( ReportShoulderAnalysis::IsOpen() )
   {
      pCmdUI->Enable( FALSE );
   }
   else
   {
      pCmdUI->Enable( TRUE );
   }
   return;
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnUpdateReportsShoulderGeometry(CCmdUI* pCmdUI)
{
   if( ReportShoulderGeometry::IsOpen() )
   {
      pCmdUI->Enable( FALSE );
   }
   else 
   {
      pCmdUI->Enable( TRUE );
   }
   return;
}


//---------------------------------------------------------------------------
void C_Main_Frame::OnUpdateReportsL23l44(CCmdUI* pCmdUI) 
{
   if( ReportSpinal::IsOpen() )
   {
      pCmdUI->Enable( FALSE );
   }
   else
   {
      pCmdUI->Enable( TRUE );
   }
   return;
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnUpdateReportsJointMoments(CCmdUI* pCmdUI) 
{
   if( ReportJointMoments::IsOpen() )
      pCmdUI->Enable(FALSE);
   else
      pCmdUI->Enable(TRUE);
}

//---------------------------------------------------------------------------
void C_Main_Frame::OnUpdateReportsStrengthDirVectors(CCmdUI* pCmdUI) 
{
   if( ReportStrengthDirVectors::IsOpen() )
   {
      pCmdUI->Enable( FALSE );
   }
   else
   {
      pCmdUI->Enable( TRUE );
   }
   return;
}

void C_Main_Frame::OnUpdateReportsBalanceSeated(CCmdUI* pCmdUI) 
{
   if( ReportBalanceSeated::IsOpen() )
   {
      pCmdUI->Enable( FALSE );
   }
   else
   {
      pCmdUI->Enable( TRUE );
   }
   return;	

}

void C_Main_Frame::OnUpdateReportsBalanceStanding(CCmdUI* pCmdUI) 
{
   if( ReportBalanceStanding::IsOpen() )
   {
      pCmdUI->Enable( FALSE );
   }
   else
   {
      pCmdUI->Enable( TRUE );
   }
   return;

}

void C_Main_Frame::OnUpdateReportsJointforces(CCmdUI* pCmdUI) 
{
   if( ReportJointForces::IsOpen() )
   {
      pCmdUI->Enable( FALSE );
   }
   else
   {
      pCmdUI->Enable( TRUE );
   }
   return;
}

// TODO remove this
void C_Main_Frame::OnDestroy() 
{
   CMDIFrameWnd::OnDestroy();
}

void C_Main_Frame::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO remove this override
   /*if((0xFFF0 & nID) == SC_MOVE)	// Checks to see if it was a move
   {
      CFrameWnd::OnSysCommand( nID, lParam );	//Do typical processing
      //RedrawWindow(NULL, NULL, RDW_INTERNALPAINT | RDW_UPDATENOW | RDW_ALLCHILDREN);	// JTK - Forces it to redraw everything
      RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_UPDATENOW | RDW_ALLCHILDREN);	// JTK - Forces it to redraw everything
   }
   else
   {*/
      CFrameWnd::OnSysCommand(nID,lParam);
   //}
}

// TODO remove this
// Function to refresh all windows
void C_Main_Frame::OnRefreshAll()
{
   this->SendMessageToDescendants(WM_PAINT);
}

BOOL C_Main_Frame::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN) {
		int blah = 0;
	}
    //(BS) Prevent the user from closing any of the windows by pressing CTRL F4. This 
    //avoids an error that causes the program to crash if one of the windows is closed
    //and the user tries to save.
    if ((GetKeyState(VK_CONTROL) & 0x8000) && (GetKeyState(VK_F4) & 0x8000))
        return TRUE; //Returning true prevents windows from handling this keystroke

	if(pMsg->message == WM_HOTKEY) {
		int x = 0;
	}

    return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

void C_Main_Frame::OnUpdateReportsBalance(CCmdUI *pCmdUI)
{
	if( ReportBalanceUnified::IsOpen() ) {
		pCmdUI->Enable( FALSE );
	} else {
		pCmdUI->Enable( TRUE );
	}
}

void C_Main_Frame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_SHIFT) {
		::Get_App_Ptr()->Get_Document_Ptr()->GLViews[VIEW_FRONT]->OnKeyDown(nChar, nRepCnt, nFlags);
	}

	CMDIFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
