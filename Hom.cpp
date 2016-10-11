// hom.cpp : Defines the class behaviors for the application.
//
#include "stdafx.h"
#include "AppVersion.hpp"
#include "hom.h"
#include "afxctl.h"
#include "afx.h"
#include "strsafe.h"
#include "main_frm.h"
#include "hom_doc.h" // Necessary
#include "GLOrthoViews.h"
#include "GLObliqueView.h"
//#include "GLPrintViews.h"

#include "dibframe.h"  
#include "dibview.h"

#include "Dimension.hpp"
#include "Unitless.hpp"
#include "Length.hpp"
#include "Force.hpp"
#include "Torque.hpp"
#include "Mass.hpp"

#include "StatusBox.h"
#include "AnimationBar.h"

#include "ReportTaskInputSummary.h" 
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
#include "ReportSpinal.h"
#include "ReportStrength.h"
#include "ReportStrengthDirVectors.h"
#include "ReportSagittal.h"
#include "ReportLowBack.h"
#include "ReportBalanceSeated.h"
#include "ReportShoulderAnalysis.h"
#include "ReportShoulderGeometry.h"

#include "ReportFrame.h"

#include "DummyView.h"
#include "DummyFrame.h"

#include "Dg_License.h"              // License agreement window
#include "Dg_RegistrationInfo.h"     // Registration information window

#include "Dg_NewRegistration.h"
#include "Dg_RegistrationExpired.h"

#include "Dg_TaskAlreadyOpen.h"

#include "HomDocTemplate.hpp"  
#include "WindowData.hpp"
#include "glHom/locals.h"
#include <winuser.h>

#include "PredictedMotion.h"

#include <map>
#include <algorithm>

using std::fstream;
using std::map;
using std::for_each;

// softwarepassport function declaration
typedef bool (__stdcall *SetUserStringFn)(int which, const char *string);
typedef DWORD (__stdcall *GetUserStringFn)(int which, char *buffer, DWORD bufferlength);
typedef bool (__stdcall *UninstallKeyFn)(void);
typedef bool (__stdcall *ExpireCurrentKeyFn)(void); // for debugging

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif     

/////////////////////////////////////////////////////////////////////////////
// C_Hom_App

BEGIN_MESSAGE_MAP(C_Hom_App, CWinApp)
   //{{AFX_MSG_MAP(C_Hom_App)
   ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
   ON_UPDATE_COMMAND_UI(ID_APP_ABOUT, OnUpdateAppAbout)
   ON_COMMAND(ID_ABOUT_LICENSE, OnLicense)
   ON_COMMAND(ID_ABOUT_REGISTRATION, OnRegistration)
   ON_COMMAND(ID_FILE_RESET_STARTUP_TASK, OnRestoreWorkspace)
   ON_COMMAND(ID_APP_EXIT, OnAppExit)
   //}}AFX_MSG_MAP
   // Standard file based document commands
   ON_COMMAND(ID_FILE_NEW, C_Hom_App::OnFileNew)
   ON_COMMAND(ID_FILE_OPEN, C_Hom_App::OnFileOpen)
   // Standard print setup command
   ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

//=== Units =================================================================

Unitless    gUnitlessUnits( 0.0F, 1 );
extern   Unitless *  gUnitlessUnitsPtr = & gUnitlessUnits;

Length      gLengthUnits( 0.0F, 1 );
extern   Length *    gLengthUnitsPtr = & gLengthUnits;

Mass        gMassUnits( 0.0F, 1 );
extern   Mass *      gMassUnitsPtr = & gMassUnits;

Force       gForceUnits( 0.0F, 1 );
extern   Force *     gForceUnitsPtr = & gForceUnits;

//===========================================================================

const char* DEMO_MSG = "DEMONSTRATION OPERATION:\nThe anthropometry is locked"
   " at 5th percentile male in demonstration operation mode.\n"
   "See the About->Registration dialog for purchase instructions.";

const char* DEFAULT_WORKSPACE_MSG = "Warning: This will overwrite any current "
      "defaults you may have set.  Proceed?";

/////////////////////////////////////////////////////////////////////////////
// The one and only C_Hom_App object

C_Hom_App theApp;

/////////////////////////////////////////////////////////////////////////////
// C_Hom_App construction

//---------------------------------------------------------------------------
C_Hom_App::C_Hom_App() :

mNewWindowDataPtr(NULL),

mApplicationName(),
mRunInvisible(false),
mRestoringDefaultWorkspace(false)
{
	CWinApp::m_pMainWnd = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only C_Hom_App object

//C_Hom_App theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {8F41A645-F62A-11D1-9480-000000000000}
static const CLSID clsid =
{ 0x8F41A650, 0xf62a, 0x11d1, { 0x94, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } };
static const CLSID tlid =
{ 0x8F41A646, 0xf62a, 0x11d1, { 0x94, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } };

/////////////////////////////////////////////////////////////////////////////
// C_Hom_App initialization

/*------------------------------------------------------------*/
BOOL C_Hom_App::InitInstance()
{
   //--- Application ---------------  
   
   mApplicationName = SZ_APP_TITLE; 
   
   //--- Initialize units ----------
   
   Length::SystemOfUnits( MetricCns );
   Force::SystemOfUnits( MetricCns );
   Torque::SystemOfUnits( MetricCns );
   Mass::SystemOfUnits( MetricCns );
   
   //--- COM -----------------------
   
   // Initialize OLE libraries
   if (!AfxOleInit())
   {
      AfxMessageBox( "COM initialization failed." );
      return FALSE;
   }
   
   // Standard initialization
   // If you are not using these features and wish to reduce the size
   //  of your final executable, you should remove from the following
   //  the specific initialization routines you do not need.

   // Change the registry key under which our settings are stored.
   // You should modify this string to be something appropriate
   // such as the name of your company or organization.

   SetRegistryKey(_T("University of Michigan"));

   // ACT - Read in User Configured Options
   CString DisplayType = GetProfileString( "Settings", "Display", "OpenGL");

   // ACT - Write them, in case they were missing and we're using the default values
   WriteProfileString( "Settings", "Display", DisplayType);

   LoadStdProfileSettings();  // Load standard INI file options (including MRU)

   // Other application data

//   this->Release_Type = "";
   // This command must be after the SetRegistryKey call
/*   Company_Name = GetProfileString
   ( "Registration", "Company", "UnknownCompany" );*/

   HINSTANCE libInst = LoadLibrary("ArmAccess.DLL");
#if defined(ReleaseGM)
   LicenseName = "General Motors / UAW";
#elif defined(ReleaseFord)
   LicenseName = "Ford Motor Company";
#elif defined(ReleaseUM)
   LicenseName = "Univ. of Michigan Use Only";
#else
   //AfxMessageBox("Before checking");
	if(!libInst) {
		AfxMessageBox("Unable to load registration dll, Unprotected."); /* Couldn't load library */
		LicenseName = "Unprotected";
	} else {

		// get function to set the licensed to name
		SetUserStringFn SetUserString = (SetUserStringFn)GetProcAddress(libInst, "SetUserString");
		GetUserStringFn GetUserString = (GetUserStringFn)GetProcAddress(libInst, "GetUserString");

		// check if it's registered, if not, show reg dialog
		char keyType[1024];
		if(GetEnvironmentVariable("TYPE", keyType, 1023)) {
			if(strcmp(keyType, "Full")) {
	
				// if trial license is expired, show the expired registration dialog
				char expired[1024];
				if(GetEnvironmentVariable("EXPIRED",expired,1023) && !strcmp(expired, "True")) {
					Dg_RegistrationExpired regexp;
					if(regexp.DoModal() != IDOK) {
						return false;
					} else {
						LicenseName = regexp.RegName;
						SetUserString(0, LicenseName.GetString());
					}
				} else { // show registration dialog for non-expired trial
					Dg_NewRegistration newReg;
					if(newReg.DoModal() == IDOK) {
						LicenseName = newReg.RegName;
						SetUserString(0, LicenseName.GetString());
					} else {
						LicenseName = "Unlicensed";
					}
				}
			} else {
				/*AfxMessageBox("Full reg");
				// cjb DEBUGGING
				// check for expiration even if full registration type
				char expired[1024];
				if(GetEnvironmentVariable("EXPIRED",expired,1023) && !strcmp(expired,"True")) {
					AfxMessageBox("expired");
					Dg_RegistrationExpired regexp;
					if(regexp.DoModal() != IDOK) {
						return false;
					} else {
						LicenseName = regexp.RegName;
						SetUserString(0, LicenseName.GetString());
					}
				}
				AfxMessageBox("not expired");*/
				// it's registered. get the licensed to name from the key
				char ln[1024];
				GetUserString(0, ln, 1023);
				LicenseName = ln;
			}
		} // if not, it's unprotected
	}
	//AfxMessageBox("after checking");
#endif

   //-------------------------------
   UINT lResources = IDR_Standard;

   //--- Create principal document template 
   //    NOTE: The default CMDIChildWnd class
   //    has been replaced by Quadrant_Frame
   //    which does customized initial 
   //    sizing of the window. C_Hom_View
   //    windows are used for displaying
   //    hominoid graphics.
   CMultiDocTemplate* pDocTemplate;
   pDocTemplate = new HomDocTemplate  //custom subtype document template
      (
      lResources,
      RUNTIME_CLASS(C_Hom_Doc),
      RUNTIME_CLASS(Quadrant_Frame), //Special quadrant frame 
      RUNTIME_CLASS(CGLView)
      );

   AddDocTemplate(pDocTemplate);

   // Connect the COleTemplateServer to the document template.
   //  The COleTemplateServer creates new documents on behalf
   //  of requesting OLE containers by using information
   //  specified in the document template.
   
   //m_server.ConnectTemplate(clsid, pDocTemplate, FALSE);
   
   //	// Note: MDI applications register all server objects without regard
   //	//  to the /Embedding or /Automation on the command line.
   
   //--- Create templates for the openGL view windows
   mViewTemplateMap[VIEW_TOP] = new HomDocTemplate(lResources,RUNTIME_CLASS(C_Hom_Doc),
                                                              RUNTIME_CLASS(Quadrant_Frame),
                                                              RUNTIME_CLASS(CGLTopView));

   mViewTemplateMap[VIEW_FRONT] = new HomDocTemplate(lResources,RUNTIME_CLASS(C_Hom_Doc),
                                                                RUNTIME_CLASS(Quadrant_Frame),
                                                                RUNTIME_CLASS(CGLFrontView));

   mViewTemplateMap[VIEW_SIDE] = new HomDocTemplate(lResources,RUNTIME_CLASS(C_Hom_Doc),
                                                               RUNTIME_CLASS(Quadrant_Frame),
                                                               RUNTIME_CLASS(CGLSideView));

   mViewTemplateMap[VIEW_OBLIQUE] = new HomDocTemplate(lResources,RUNTIME_CLASS(C_Hom_Doc),
                                                                  RUNTIME_CLASS(Quadrant_Frame),
                                                                  RUNTIME_CLASS(CGLObliqueView));

/*   mPrintViewTemplateMap[VIEW_TOP] = new HomDocTemplate(lResources, RUNTIME_CLASS(C_Hom_Doc),
																	RUNTIME_CLASS(Quadrant_Frame),
																	RUNTIME_CLASS(CGLTopViewPrint));
   mPrintViewTemplateMap[VIEW_FRONT] = new HomDocTemplate(lResources, RUNTIME_CLASS(C_Hom_Doc),
																	RUNTIME_CLASS(Quadrant_Frame),
																	RUNTIME_CLASS(CGLFrontViewPrint));
   mPrintViewTemplateMap[VIEW_SIDE] = new HomDocTemplate(lResources, RUNTIME_CLASS(C_Hom_Doc),
																	RUNTIME_CLASS(Quadrant_Frame),
																	RUNTIME_CLASS(CGLSideViewPrint));
   mPrintViewTemplateMap[VIEW_OBLIQUE] = new HomDocTemplate(lResources, RUNTIME_CLASS(C_Hom_Doc),
																	RUNTIME_CLASS(Quadrant_Frame),
																	RUNTIME_CLASS(CGLObliqueViewPrint));*/
   
   //--- Create templates for the status and hands
   //    windows. These use the Quadrant_Frame as for
   //    the above graphic windows, but use a 
   //    different view class for each
   
   //--- Template for Status Results view.
   mStatusResultsDocTmpPtr = new HomDocTemplate //custom subtype document template
	    (
	    lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( Quadrant_Frame ), //Special quadrant frame 
	    RUNTIME_CLASS( StatusBox )
	    );

   //--- Template for Animation Bar
   mAnimationBarDocTmpPtr = new HomDocTemplate //custom subtype document template
	    (
	    lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( Quadrant_Frame ), //Special quadrant frame 
	    RUNTIME_CLASS( AnimationBar )
	    );
   
   //--- Create templates for the all the report
   //    windows. These all use a special ReportFrame
   //    and individual view classes specific
   //    to each. This is because each of the view
   //    classes use a different dialog resource. 
   
   mReportTemplateMap[ID_REPORTS_TASK_SUMMARY] =
	    new CMultiDocTemplate(
	    IDR_REPORT_ACCEL,	// lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( ReportFrame ),
	    RUNTIME_CLASS( ReportTaskInputSummary )
	    );

   mReportTemplateMap[ID_REPORTS_ANALYSIS_SUMMARY] =
	    new CMultiDocTemplate(
	    IDR_REPORT_ACCEL,	// lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( ReportFrame ),
	    RUNTIME_CLASS( ReportAnalysisSummary )
	    );

   mReportTemplateMap[ID_REPORTS_ANTHROPOMETRY] =
	    new CMultiDocTemplate(
	    IDR_REPORT_ACCEL,	// lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( ReportFrame ), 
	    RUNTIME_CLASS( ReportAnthropometry ) 
	    );

   mReportTemplateMap[ID_REPORTS_BALANCE_STANDING] =
	    new CMultiDocTemplate(
	    IDR_REPORT_ACCEL,	// lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( ReportFrame ), 
	    RUNTIME_CLASS( ReportBalanceStanding ) 
	    );

   mReportTemplateMap[ID_REPORTS_BALANCE_SEATED] =
	    new CMultiDocTemplate(
	    IDR_REPORT_ACCEL,	// lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( ReportFrame ), 
	    RUNTIME_CLASS( ReportBalanceSeated ) 
	    );

   mReportTemplateMap[ID_REPORTS_FATIGUE] =
	    new CMultiDocTemplate(
	    IDR_REPORT_ACCEL,	// lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( ReportFrame ), 
	    RUNTIME_CLASS( ReportFatigue ) 
	    );

   mReportTemplateMap[ID_REPORTS_POSTURE] =
	    new CMultiDocTemplate(
	    IDR_REPORT_ACCEL,	// lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( ReportFrame ), 
	    RUNTIME_CLASS( ReportPosture ) 
	    );

   mReportTemplateMap[ID_REPORTS_JOINT_LOCATIONS] =
	    new CMultiDocTemplate(
	    IDR_REPORT_ACCEL,	// lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( ReportFrame ), 
	    RUNTIME_CLASS( ReportJointLocations )
	    );

   mReportTemplateMap[ID_REPORTS_CENTEROFMASSLOCATIONS] =
	    new CMultiDocTemplate(
	    IDR_REPORT_ACCEL,	// lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( ReportFrame ), 
	    RUNTIME_CLASS( ReportCoMLocations )
	    );

   mReportTemplateMap[ID_REPORTS_JOINT_MOMENTS] =
	    new CMultiDocTemplate(
	    IDR_REPORT_ACCEL,	// lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( ReportFrame ), 
	    RUNTIME_CLASS( ReportJointMoments )
	    );

   mReportTemplateMap[ID_REPORTS_JOINT_FORCES] =
	    new CMultiDocTemplate(
	    IDR_REPORT_ACCEL,	// lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( ReportFrame ), 
	    RUNTIME_CLASS( ReportJointForces )
	    );

   mReportTemplateMap[ID_REPORTS_SPINAL] =
	    new CMultiDocTemplate(
	    IDR_REPORT_ACCEL,	// lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( ReportFrame ),
	    RUNTIME_CLASS( ReportSpinal )
	    );

   mReportTemplateMap[ID_REPORTS_STRENGTH] =
        new CMultiDocTemplate(
	    IDR_REPORT_ACCEL,	// lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( ReportFrame ),
	    RUNTIME_CLASS( ReportStrength )
	    );
	    
   mReportTemplateMap[ID_REPORTS_STRENGTH_DIR_VECTORS] =
	    new CMultiDocTemplate(
	    IDR_REPORT_ACCEL,	// lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( ReportFrame ),
	    RUNTIME_CLASS( ReportStrengthDirVectors )
	    );
   
   mReportTemplateMap[ID_REPORTS_SAGITTAL] =
	    new CMultiDocTemplate(
	    IDR_REPORT_ACCEL,	// lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( ReportFrame ),
	    RUNTIME_CLASS( ReportSagittal ) 
	    );

   mReportTemplateMap[ID_REPORTS_3D_LOWBACK] =
	    new CMultiDocTemplate(
	    IDR_REPORT_ACCEL,	// lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( ReportFrame ),
	    RUNTIME_CLASS( ReportLowBack ) 
	    );

   mReportTemplateMap[ID_REPORTS_SHOULDERANALYSIS] =
	    new CMultiDocTemplate(
	    IDR_REPORT_ACCEL,	// lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( ReportFrame ),
        RUNTIME_CLASS( ReportShoulderAnalysis ) 
	    );

   mReportTemplateMap[ID_REPORTS_SHOULDERGEOMETRY] =
	   	new CMultiDocTemplate(
	    IDR_REPORT_ACCEL,	// lResources,
	    RUNTIME_CLASS( C_Hom_Doc ),
	    RUNTIME_CLASS( ReportFrame ),
        RUNTIME_CLASS( ReportShoulderGeometry ) 
	    );

   mReportTemplateMap[ID_REPORTS_BALANCE] =
		new CMultiDocTemplate(
		IDR_REPORT_ACCEL,
		RUNTIME_CLASS( C_Hom_Doc ),
		RUNTIME_CLASS( ReportFrame ),
		RUNTIME_CLASS( ReportBalanceUnified )
		);

   //--- Special template for printing DIBs directly. 
   //    Stored data member pointer for same reasons
   //    as above template.
   this->Dib_DocTemplate_Ptr =
	    new CMultiDocTemplate(
	    lResources,
	    RUNTIME_CLASS(C_Hom_Doc),
	    RUNTIME_CLASS(CMDIDibChildFrame),  //Special MDI child DIB frame
	    RUNTIME_CLASS(CDibView)            //Special DIB view class
	    );

   // dummy view to pass printing events back to main frame so we can print the whole desktop or application
   this->mDummyTemplatePtr =
	   new CMultiDocTemplate(
	   lResources,
	   RUNTIME_CLASS(C_Hom_Doc),
	   RUNTIME_CLASS(CDummyFrame),
	   RUNTIME_CLASS(CDummyView)
	   );
   
   
   //--- Create main MDI Frame window
   C_Main_Frame* pMainFrame = new C_Main_Frame;
   if ( !pMainFrame->LoadFrame( lResources ) )
	    return FALSE;
   this->m_pMainWnd = pMainFrame;
   
   //--- Initialize cursors ---
   mCrossCursorHdl = this->LoadStandardCursor( IDC_CROSS );
   mArrowCursorHdl = this->LoadStandardCursor( IDC_ARROW );
   mSelectableCursorHdl = this->LoadCursor( IDC_Finger );
   
   //--- Parse command line for standard shell 
   //commands, DDE, file open, automation, etc.
   CCommandLineInfo cmdInfo;
   ParseCommandLine(cmdInfo);
   
   // if /regserver, register and exit
   if(cmdInfo.m_nShellCommand == cmdInfo.AppRegister || cmdInfo.m_nShellCommand == cmdInfo.AppUnregister) {
	   COleObjectFactory::UpdateRegistryAll();
	   return FALSE;
   }

   mRunInvisible = ( cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated );
   
   //--- COM ----------------------
   
   // Register all OLE server factories as running.  This enables the
   // OLE libraries to create objects from other applications.
   
   COleTemplateServer::RegisterAll();
   
   //NOTE: After RegisterAll, the client and server threads proceed in parallel.
   //IF the app has been started as a server. The client makes subsequent calls
   //to the server for document creation and further use of the document interface.
   //The server continues on but does not create a document.

   // we use Software Passport for registration now
   /*if(!this->SiteData())          // Reg info not found
   {
      Dg_Registration myRegWnd;    // Try to get reg-code from user
      myRegWnd.DoModal();
   }*/
   
   if (!mRunInvisible) // normal stand-alone execution
   {
//#ifndef _3DSSPP_ALPHA
      if (!this->Log_On()) // Logon problem 
      {
	    return FALSE;
      }
//#endif
      /*if( this->Release_Type == "Demonstration" ) //Demo warning
      {
          AfxMessageBox(DEMO_MSG);
      }*/
      //--- Create first document: Overridden to 
      //    create additional views.
	  if(cmdInfo.m_nShellCommand != CCommandLineInfo::FileOpen) {
		this->Document_Exists = FALSE;
		this->Create_Document( CT_INITIAL );
	  } else {
		if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen) {
			this->Document_Exists = false;
			openfilename = cmdInfo.m_strFileName;
			this->Create_Document(CT_OPEN_INITIAL);
		}
		/*if(!ProcessShellCommand(cmdInfo)) {
			   return false;
		}*/
	  }
   }
   else //launched as OLE server: get out of here
   {
	    return TRUE;
   }
	
   //When a server application is launched stand-alone, it is a good idea
   //to update the system registry in case it has been damaged.
   //m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
   COleObjectFactory::UpdateRegistryAll();

   // ACT - Register the type library in case its not there! It needs to be there to run!
   // Don't panic if this doesnt work
   AfxOleRegisterTypeLib(AfxGetInstanceHandle(), tlid, "SSPPApp.tlb");

   return TRUE;
}

// Helper for New, Open
bool C_Hom_App::CheckForSave()
{
   if(this->Document_Exists)
   {
      if (this->Get_Document_Ptr()->IsModified())
      {
         int lBoxAnswer = 
            AfxMessageBox("Save changes to current task?",
            MB_YESNOCANCEL);

         if(lBoxAnswer == IDCANCEL)
			{
				return false;	// User cancelled
			}
			else if(lBoxAnswer == IDYES)
			{
				this->Get_Document_Ptr()->DoSave();
         
				if(this->Get_Document_Ptr()->IsModified())
					return false;	// Didn't save; do not continue
			}
			// else lBoxAnswer == IDNO, so it's okay to wipe the document
      }
   }

	return true;	// Everything dandy; go 'head and do whatever you want
}

void C_Hom_App::OnFileNew()
{
	this->Create_Document( CT_NEW );
	return;
}

void C_Hom_App::OnFileOpen( void )
{
	this->Create_Document( CT_OPEN );
	return;
}

CString C_Hom_App::GetLicenseName() {
	return LicenseName;
}

void C_Hom_App::Create_Document( E_Create_Type   CreateType )
{
    this->BeginWaitCursor();

    // For subsequent document creation, destroy the existing document and its associated views.
    C_Hom_Doc* Delete_Document_Ptr = NULL;
    C_Hom_Doc* Doc_Ptr = NULL;
    if(this->Document_Exists)
    {
        // Deletion of this document deferred until another document is successfully opened or created.
        Delete_Document_Ptr = this->Get_Document_Ptr();
    }   

    #ifndef WIN32
        CDocTemplate* Doc_Template_Ptr = (CDocTemplate*)m_templateList.GetHead();
    #else
        POSITION Template_Position = GetFirstDocTemplatePosition();
        CDocTemplate* Doc_Template_Ptr = this->GetNextDocTemplate(Template_Position);
    #endif

    OpenType = 431;

    // Perform creation based on whether document is new or existing (load from file)
    switch(CreateType)
    {
        case CT_INITIAL:
            // Program started; just follow through into new document creation
        case CT_NEW:
            if(!CheckForSave())
            {
                EndWaitCursor();
                return;
            }
			GLSkeleton::setShowGrid(false);
			GLSkeleton::setShowForces(true);
			GLSkeleton::setShowSupportForces(false);
			GLOblique::setframenum(false);
			GLSkeleton::setGridSize(20);
            TCHAR pszPath[MAX_PATH];
            // JTK - Make sure if you change CSIDL_APPDATA you also change it in the saving code
            GetModuleFileName(NULL, pszPath, MAX_PATH);
            PathRemoveFileSpec(pszPath);
            StringCchCatN(pszPath,MAX_PATH,DEFAULT_WORKSPACE, strlen(DEFAULT_WORKSPACE)+1);
            if (!mRestoringDefaultWorkspace)
            {
                if (PathFileExists(pszPath)) // Check to see if the file exists
                {                    
	                Doc_Ptr = static_cast< C_Hom_Doc* > ( Doc_Template_Ptr->OpenDocumentFile( pszPath));
					if (Doc_Ptr != NULL) {
	                    Doc_Template_Ptr->SetDefaultTitle(Doc_Ptr);
						this->GetMainFramePtr()->Update_All_Title_Bars();
					}
                } 
            }

            if (Doc_Ptr == NULL)
            {	
                // Failed to load file - just make a new doc
                CWinApp::OnFileNew();
            }

            break;
        case CT_IMPORT_42x:
            OpenType = 421;
		case CT_OPEN_INITIAL:
			Doc_Ptr = static_cast<C_Hom_Doc*>(Doc_Template_Ptr->OpenDocumentFile(openfilename));
			if(Doc_Ptr == 0) {
				   // Reset data here?
	            this->Get_Document_Ptr()->Analyze();
	            EndWaitCursor();
	            return;
            }
	
			if(!mRunInvisible) {
				Doc_Ptr->InitialOpenReports();
			}
			break;
        case CT_OPEN:
            if(!CheckForSave())
            {
	            EndWaitCursor();
	            return;
            }

            // LCW - the pipes separate lines in the open file dialog, and the double-pipe delimits the string
            CFileDialog fileDlg(true, NULL, "*.tsk", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
	            "3DSSPP Task Files (*.tsk)|*.tsk|All Files (*.*)|*.*||");

            // Show the file selection box
            if(fileDlg.DoModal() != IDOK)
            {
	            EndWaitCursor();
	            return;
            }

            // if selected file is currently open, notify user
            if(fileDlg.GetPathName() == Get_Document_Ptr()->GetPathName())
            {
                Dg_TaskAlreadyOpen Diag;
                if(Diag.DoModal() == IDOK) 
                {
	                //Mark clean so it will be deleted without querying the user to save changes
	                Delete_Document_Ptr->SetModifiedFlag(FALSE);
                }
                else
                {
	                EndWaitCursor();
	                return;
                }
            }
        		
            // attempt to open the file
            Doc_Ptr = static_cast< C_Hom_Doc* > ( Doc_Template_Ptr->OpenDocumentFile( fileDlg.GetPathName()));

            if(Doc_Ptr == 0)
            {
	            // Reset data here?
	            this->Get_Document_Ptr()->Analyze();
	            EndWaitCursor();
	            return;
            }
	
			if(!mRunInvisible) {
				Doc_Ptr->InitialOpenReports();
			}

            break;
    }

    // assuming execution reaches this point, the new document exists
    this->Document_Exists = TRUE;
       
    // now remove the previously open document, if it exists
    if(Delete_Document_Ptr)
    {
        Delete_Document_Ptr->DoClose();
        delete Delete_Document_Ptr;
        Delete_Document_Ptr = NULL;
    }

    // run an initial analysis, so that the status box is filled correctly
    this->Get_Document_Ptr()->Analyze();

    this->EndWaitCursor();
}

//---------------------------------------------------------------------------
C_Main_Frame* C_Hom_App::GetMainFramePtr()
{ 
   assert( m_pMainWnd != NULL );
   return  (C_Main_Frame*)( this->m_pMainWnd ); 
}

/*------------------------------------------------------------*/
int C_Hom_App::ExitInstance()
{
   if(mStatusResultsDocTmpPtr)
      delete mStatusResultsDocTmpPtr;

   if(mAnimationBarDocTmpPtr)
      delete mAnimationBarDocTmpPtr;

   // Clean up doc template map
   map< int, CMultiDocTemplate* >::iterator mItr;
   for(mItr = mReportTemplateMap.begin();
      mItr != mReportTemplateMap.end();
      ++mItr)
      delete mItr->second;

   map<int, HomDocTemplate*>::iterator itr;
   for(itr = mViewTemplateMap.begin();
	   itr != mViewTemplateMap.end();
	   ++itr)
	   delete itr->second;

   delete Dib_DocTemplate_Ptr;

   delete mDummyTemplatePtr;

   PredictedMotion::CleanUp();
   ReportShoulderAnalysis::killMatlab();

   return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Dg_About dialog used for App About

class Dg_About : public CDialog
{
public:
   Dg_About();
   
   // Dialog Data
   //{{AFX_DATA(Dg_About)
   enum { IDD = IDD_ABOUTBOX };
   CString App_Name;
   CString m_copyright;
   //}}AFX_DATA
	
   // Implementation
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //{{AFX_MSG(Dg_About)
   // No message handlers
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedAppName();
};

//---------------------------------------------------------------------------
Dg_About::Dg_About() : CDialog(Dg_About::IDD)
{
   //{{AFX_DATA_INIT(Dg_About)
   App_Name = "";
   m_copyright = _T("");
   //}}AFX_DATA_INIT
}

//---------------------------------------------------------------------------
void Dg_About::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(Dg_About)
   DDX_Text(pDX, IDC_APP_NAME, App_Name);
   DDX_Text(pDX, IDC_COPYRIGHT, m_copyright);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Dg_About, CDialog)
   //{{AFX_MSG_MAP(Dg_About)
   // No message handlers
   //}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_APP_NAME, &Dg_About::OnStnClickedAppName)
END_MESSAGE_MAP()
	
// App command to run the dialog
//---------------------------------------------------------------------------
void C_Hom_App::OnAppAbout()
{
   Dg_About aboutDlg;
   aboutDlg.App_Name = Get_App_Ptr()->ApplicationName();
   aboutDlg.m_copyright = Get_App_Ptr()->Get_Document_Ptr()->ReportFooter();
   aboutDlg.DoModal();
   return;
}

void C_Hom_App::OnLicense()
{
   Dg_License lDialog;
   lDialog.DoModal();
   return;
}

void C_Hom_App::OnRegistration()
{
   Dg_RegistrationInfo lDialog;
   lDialog.DoModal();
   return;
}

//---------------------------------------------------------------------------
void C_Hom_App::OnUpdateAppAbout(CCmdUI* pCmdUI) 
{
	pCmdUI->SetText( "&About 3DSSPP ..." );
}

/*----------------------------------------------------------------*/
C_Hom_App* Get_App_Ptr()
	//static class scope function   
{
   C_Hom_App* App_Ptr = (C_Hom_App*)AfxGetApp();
   assert(App_Ptr != NULL);
   ASSERT_VALID(App_Ptr);
   return App_Ptr;
}

//---------------------------------------------------------------------------
const CString& C_Hom_App::ApplicationName() const
{
   return mApplicationName;
}

/*--------------------------------*/ /*global*/ //--------------
C_Main_Frame* MainFramePtr()
//static class scope function   
{
   return (C_Main_Frame*)(Get_App_Ptr()->m_pMainWnd);
}

/*----------------------------------------------------------------*/
C_Hom_Doc* C_Hom_App::Get_Document_Ptr() 
{
#ifndef WIN32
	//Get template pointer: Assume just one
	CMultiDocTemplate* Temp_Ptr = static_cast<CMultiDocTemplate*>
		(m_templateList.GetHead());
#else  //is Win32
	POSITION   Template_Position = GetFirstDocTemplatePosition();
	CMultiDocTemplate* Temp_Ptr = static_cast<CMultiDocTemplate*>
		(GetNextDocTemplate(Template_Position));
#endif
	
   //Get first document pointer
   POSITION  Doc_Position = Temp_Ptr->GetFirstDocPosition();

   if (Doc_Position == NULL)
   {
      TRACE("Warning: Get_Document_Ptr could not find a document.  Returning NULL.\n");
      return NULL;
   }
   //assert( Doc_Position != NULL );

   C_Hom_Doc* Doc_Ptr = (C_Hom_Doc*)(Temp_Ptr->GetNextDoc( Doc_Position ));

   if( Doc_Position != NULL ) //A second document exists. In this application,
      //this happens for a short time during the opening
      //of a new document. The old one is kept around
      //until it is certain that the new one is valid.
      //During this time, requests for the active 
      //document require the second document.
   {
      Doc_Ptr = (C_Hom_Doc*)(  Temp_Ptr->GetNextDoc( Doc_Position )  );
      assert( Doc_Position == NULL ); //There should be no more  
   }

   ASSERT_VALID( Doc_Ptr );

   return Doc_Ptr; 
};

/////////////////////////////////////////////////////////////////////////////
// C_Hom_App command

void C_Hom_App::OnAppExit()
{
   // Exit code now in C_Main_Frame::OnClose()
   CWinApp::OnAppExit();
}

// This function simply creates a new document, and
//    saves that document as the default workspace
void C_Hom_App::OnRestoreWorkspace()
{
   int proceed = AfxMessageBox(DEFAULT_WORKSPACE_MSG, MB_YESNO);

   if(proceed == IDNO)
      return;

   C_Hom_Doc* Doc_Ptr = NULL;
   this->mRestoringDefaultWorkspace = true;
   this->Create_Document(CT_NEW);
   
   // If the creation was cancelled, the doc will still be in 'modified'
   //		state and we should not overwrite it.
   if(!Get_Document_Ptr()->IsModified())
      Get_Document_Ptr()->SetDefaultWorkspace();

   // This needs to be set in the case of file new
   // after a restore has occurred
   this->mRestoringDefaultWorkspace = false;
}

BOOL C_Hom_App::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN) {
		int z = 0;
	}
	// TODO printing
	if(pMsg->message == WM_HOTKEY) {
		int x = 0;
	}
	if(pMsg->lParam & MOD_ALT) {
		int y = 0;
	}

	return CWinApp::PreTranslateMessage(pMsg);
}


void Dg_About::OnStnClickedAppName()
{
	// TODO: Add your control notification handler code here
}
