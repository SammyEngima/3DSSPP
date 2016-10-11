#include "stdafx.h"
#include "hom_doc.h"

#include "AppVersion.hpp"
#include "AnthroPercentiles.h"
#include "Afxwin.h"

#include "strsafe.h"
#include "hom.h"

#include "OfflineSafeCFile.h"
#include "JointAngles.hpp"

#include "../c4eLibNew/units.hpp"
#include "main_frm.h"
#include "UndoPosture.h"

#include "Dg_Light.h"
#include "Dg_CameraOblique.h" 
#include "Dg_PosturePrediction.h"
#include "Dg_HandLoads.h"
#include "Dg_HandLoadsAdv.h"
#include "Dg_HandAngles.h"
#include "Dg_Anthropometry.h"
#include "Dg_BodyAngles.h"
#include "Dg_BodyAnglesDyn.h"
#include "Dg_LockingMode.h"
#include "Dg_PopulationEdit.h"
#include "Dg_FactorsPassword.h"
#include "Dg_Population.h"
#include "Dg_Gravity.h"
#include "Factors.hpp"
#include "Dg_HWMeans.h"
#include "Dg_JointLoads.h"
#include "Dg_PrintReports.h"
#include "Dg_ObliqueFigure.h"
#include "Dg_TaskDescription.h"
#include "Dg_Environment.h"
#include "Dg_ObliqueBackground.h"
#include "Dg_SupportSelection.h"
#include "Dg_ClothingColor.h"
#include "Dg_OrthoColors.h"
#include "Dg_HWMeans.h"
#include "BatchFile.hpp"
#include "Dg_ExportOptions.h"
#include "Dg_MotionPrediction.h"
#include "Dg_HandPosture.h"
#include "Dg_PercentileLimits.h"
#include "Dg_OffsetCoordinates.h"
#include "Dg_HandSegments.h"
#include "Dg_MOVIE_EXPORT.h"
#include "Dg_GRID.h"

#include "ReportAnalysisSummary.h"
#include "ReportAnthropometry.h"
#include "ReportBalanceUnified.h"
#include "ReportBalanceSeated.h"
#include "ReportBalanceStanding.h"
#include "ReportFatigue.h"
#include "ReportJointForces.h"
#include "ReportJointLocations.h"
#include "ReportCoMLocations.h"
#include "ReportJointMoments.h"
#include "ReportLowBack.h"
#include "ReportShoulderAnalysis.h"
#include "ReportShoulderGeometry.h"
#include "ReportPosture.h"
#include "ReportSagittal.h"
#include "ReportSpinal.h"
#include "ReportStrength.h"
#include "ReportStrengthDirVectors.h"
#include "ReportTaskInputSummary.h"
#include "ReportLowBack.h"

#include <afxole.h>
#include <strstream>
#include <list>
#include <fstream>
#include <winuser.h>
#include <windows.h>
#include <sstream>
#include <algorithm>
#include <functional>
#include ".\hom_doc.h"

#include "GLOblique.h"
#include "GLObliqueView.h"
#include "GLSkeleton.h"
#include "GLStickView.h"
#include "ReportView.hpp"
#include "StatusBox.h"

// for getting the selected range for copy/paste
// this should actually be stored in the document and read
// from the animation bar / timeline control
#include "AnimationBar.h"

#include "CQuaternion.h"
#include "Analysis.h"

#include "AVIExporter.h"
#include "TestTimer.h"

using std::endl;
using std::ends;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::list;
using std::map;
using std::string;
using std::stringstream;
using std::strstream;
using std::find;
using std::distance;
using std::for_each;
using std::ptr_fun;

// Used to define report background colors
const BYTE REPORT_BG_RED = 255;
const BYTE REPORT_BG_GREEN = 255;
const BYTE REPORT_BG_BLUE = 255;

// Used to define the default 3-view stick figure color
const BYTE STICK_RED = 132;
const BYTE STICK_GREEN = 93;
const BYTE STICK_BLUE = 90;

// Used to define the default hominoid color
/*const BYTE HOMINOID_RED = 169;
const BYTE HOMINOID_GREEN = 128;
const BYTE HOMINOID_BLUE = 90;*/

extern Length*	gLengthUnitsPtr;
extern Mass* 	gMassUnitsPtr;
extern const char* DEMO_MSG;
extern const char* DEFAULT_WORKSPACE_MSG;

static const double DTOR = 3.14159265358979323 / 180.0;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// C_Hom_Doc
int OpenType;	// ACT - Used to determine if loading old 4.21 filetype

//IMPLEMENT_DYNCREATE(C_Hom_Doc, CDocument)
#if defined( WIN32 ) //32 bit environment
IMPLEMENT_SERIAL ( C_Hom_Doc, CDocument, VERSIONABLE_SCHEMA|1 )
#else				 //16 bit environment
//	 IMPLEMENT_SERIAL( C_Hom_Doc, CDocument, VERSIONABLE_SCHEMA|2 )
IMPLEMENT_SERIAL( C_Hom_Doc, CDocument, 2 )
#endif

BEGIN_MESSAGE_MAP(C_Hom_Doc, CDocument)
   //{{AFX_MSG_MAP(C_Hom_Doc)
   ON_COMMAND(ID_POSTURE_ANTHROPOMETRY, OnPostureAnthropometry)
   ON_COMMAND(ID_POSTURE_JOINTANGLES, OnPostureJointAngles)
   ON_COMMAND(ID_POSTURE_POSTUREPREDICTION, OnPosturePrediction)
   ON_COMMAND(ID_POSTURE_LOCKINGMODE, OnPostureLockingMode)
   ON_COMMAND(ID_SUPPORT_SELECTION_3View, OnTaskSupportSelection)
   ON_COMMAND(ID_Gridoptions, OnTaskGridOptions)
   ON_COMMAND(ID_POSTURE_HANDFORCES, OnPostureHandLoads)

// ON_COMMAND(ID_POSTURE_HANDFORCES_ADV, OnPostureHandLoadsAdv)

   //ON_COMMAND(ID_POSTURE_HANDFORCES_ADV, OnPostureHandLoadsAdv)

   ON_COMMAND(ID_OBLIQUEVIEW_HUMAN, OnObliqueViewHuman)		
   ON_COMMAND(ID_OBLIQUEVIEW_CAMERA, OnObliqueViewCamera)
   ON_COMMAND(ID_OBLIQUEVIEW_LIGHT, OnObliqueViewLight)
   ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
   ON_COMMAND(ID_ORTHO_BACKGROUND, OnOrthoBackground)
   ON_COMMAND(ID_REPORTS_EXPORTSUMMARY, OnReportsExportsummary)
   ON_COMMAND(ID_TASKINPUT_DESCRIPTION, OnTaskinputDescription)
   ON_COMMAND(ID_TASKINPUT_UNITS, OnTaskinputUnits)
   ON_COMMAND(ID_OBLIQUEVIEW_ENVIRONMENT, OnObliqueViewEnvironment)
   ON_COMMAND(ID_DISPLAY_AUTOTILE, OnDisplayAutotile)
   ON_COMMAND(ID_DISPLAY_FORCES, OnDisplayForces)
   ON_UPDATE_COMMAND_UI(ID_DISPLAY_FORCES, OnUpdateDisplayForces)
   ON_COMMAND(ID_STICK_SHOWSUPPORTFORCES, OnDisplaySupportForces)
   ON_UPDATE_COMMAND_UI(ID_STICK_SHOWSUPPORTFORCES, OnUpdateDisplaySupportForces)
   ON_COMMAND(ID_STICK_SHOWFRAME, Ondrawframe)
   ON_UPDATE_COMMAND_UI(ID_STICK_SHOWFRAME, OnUpdatedrawframe)


   ON_COMMAND(ID_SMOOTH_MANIP, OnSmoothManip)
   ON_COMMAND(ID_SMOOTH_ANALYZE, OnAnalyzeSmoothManip)
   ON_UPDATE_COMMAND_UI(ID_SMOOTH_MANIP, OnUpdateSmoothManip)
   ON_UPDATE_COMMAND_UI(ID_SMOOTH_ANALYZE, OnUpdateAnalyzeSmoothManip)
   ON_UPDATE_COMMAND_UI(ID_DISPLAY_AUTOTILE, OnUpdateDisplayAutotile)
   ON_COMMAND(ID_DISPLAY_TILENOW, OnDisplayTilenow)
   ON_COMMAND(ID_DISPLAY_STATUSBAR, OnDisplayStatusbar)
   ON_UPDATE_COMMAND_UI(ID_DISPLAY_STATUSBAR, OnUpdateDisplayStatusbar)
   ON_COMMAND(ID_Factors, OnFactors)
   ON_COMMAND(ID_REPORTS_PRINT, OnReportsPrint)
   ON_UPDATE_COMMAND_UI(ID_UndoDirectManip, OnUpdateUndoDirectManipulation)
   ON_COMMAND(ID_RedoDirectManip, OnRedoDirectManipulation)
   ON_UPDATE_COMMAND_UI(ID_RedoDirectManip, OnUpdateRedoDirectManipulation)
   ON_COMMAND(ID_UndoDirectManip, OnUndoDirectManipulation)
   ON_UPDATE_COMMAND_UI(ID_TASKINPUT_UNITS, OnUpdateTaskinputUnits)
   ON_COMMAND(ID_HANDANGLES, OnHandAngles)
   ON_COMMAND(ID_TASKINPUT_BATCH, OnTaskinputBatch)
   ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
   ON_COMMAND(ID_HumanFigure, OnHumanFigure)
   ON_COMMAND(ID_OBLIQUEVIEW_BACKGROUND, OnOblBackground)
   ON_COMMAND(ID_ZEROG, OnZerog)
   ON_COMMAND(ID_ExtJointForces, OnExtJointForces)
   ON_COMMAND(ID_FILE_SET_STARTUP_TASK, OnDefaultWorkspace)
   ON_COMMAND(ID_CLOSE_ACTIVE_WINDOW, OnCloseActiveWindow)
   ON_COMMAND(ID_IMPORTLOCFILE, OnImportLocFile)
   ON_COMMAND(ID_INSERTMOTION, OnPredictMotion)
   ///YH
   //ON_COMMAND(ID_3_BACKGROUND, OnSidBackground)
   //}}AFX_MSG_MAP

   //Standard command rerouted here
   ON_COMMAND(ID_FILE_SAVE_AS, C_Hom_Doc::OnFileSaveAs)
   ON_COMMAND(ID_FILE_SAVE, C_Hom_Doc::OnFileSave)
   ON_COMMAND(ID_TASK_SUPPORTSELECTION, OnTaskSupportSelection)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_BALANCE_SEATED, OnUpdateReportsBalanceSeated)
   ON_UPDATE_COMMAND_UI(ID_REPORTS_BALANCE_STANDING, OnUpdateReportsBalanceStanding)
   ON_COMMAND(ID_TASKINPUT_3DSTATIC, OnTaskinput3dstatic)
   ON_COMMAND(ID_TASKINPUT_3DDYNAMIC, OnTaskinput3ddynamic)
   ON_UPDATE_COMMAND_UI(ID_TASKINPUT_3DSTATIC, OnUpdateTaskinput3dstatic)
   ON_UPDATE_COMMAND_UI(ID_TASKINPUT_3DDYNAMIC, OnUpdateTaskinput3ddynamic)
   ON_COMMAND(ID_PRE_STANDINGLIFT, OnPreStandinglift)
   ON_COMMAND(ID_PRE_STOOPLIFT, OnPreStooplift)
   ON_COMMAND(ID_PRE_SQUATLIFT, OnPreSquatlift)
   ON_COMMAND(ID_PRE_FORWARDPUSH, OnPreForwardpush)
   ON_COMMAND(ID_PRE_BACKWARDPULL, OnPreBackwardpull)
   ON_COMMAND(ID_PRE_NEUTRALSTAND, OnPreNeutralstand)
   ON_COMMAND(ID_PRE_MANIPULATIONSTART, OnPreManipulationStart)
   ON_COMMAND(ID_PRE_NEUTRALT, OnPreNeutralTstand)
   ON_COMMAND(ID_PRE_NEUTRALSIT, OnPreNeutralsit)
   ON_COMMAND(ID_HANDPOSTURE, &C_Hom_Doc::OnHandposture)
   ON_COMMAND(ID_DISPLAY_STRENGTHLIMITS, &C_Hom_Doc::OnDisplayStrengthlimits)
   ON_COMMAND(ID_DISPLAY_OFFSETCOORDINATEREFERENCE, &C_Hom_Doc::OnDisplayOffsetcoordinatereference)
   ON_COMMAND(ID_EDIT_CUT, &C_Hom_Doc::OnEditCut)
   ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &C_Hom_Doc::OnUpdateEditCut)
   ON_COMMAND(ID_EDIT_COPY, &C_Hom_Doc::OnEditCopy)
   ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &C_Hom_Doc::OnUpdateEditCopy)
   ON_COMMAND(ID_EDIT_PASTE, &C_Hom_Doc::OnEditPaste)
   ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &C_Hom_Doc::OnUpdateEditPaste)
   ON_COMMAND(ID_ANIMATION_EXPORTAVIFILE, &C_Hom_Doc::OnAnimationExportavifile)
   ON_COMMAND(ID_EXPORTAVI, &C_Hom_Doc::OnExportavi)
   ON_COMMAND(ID_GridDisplay, &C_Hom_Doc::OnDg_Grid)
   ON_COMMAND(ID_WINDOWS_TOPVIEW, &C_Hom_Doc::OnWindowsTopview)
   ON_UPDATE_COMMAND_UI(ID_WINDOWS_TOPVIEW, &C_Hom_Doc::OnUpdateWindowsTopview)
   ON_COMMAND(ID_WINDOWS_FRONTVIEW, &C_Hom_Doc::OnWindowsFrontview)
   ON_UPDATE_COMMAND_UI(ID_WINDOWS_FRONTVIEW, &C_Hom_Doc::OnUpdateWindowsFrontview)
   ON_COMMAND(ID_WINDOWS_SIDEVIEW, &C_Hom_Doc::OnWindowsSideview)
   ON_UPDATE_COMMAND_UI(ID_WINDOWS_SIDEVIEW, &C_Hom_Doc::OnUpdateWindowsSideview)
   ON_COMMAND(ID_WINDOWS_OBLIQUEVIEW, &C_Hom_Doc::OnWindowsObliqueview)
   ON_UPDATE_COMMAND_UI(ID_WINDOWS_OBLIQUEVIEW, &C_Hom_Doc::OnUpdateWindowsObliqueview)
   ON_COMMAND(ID_WINDOWS_STATUSBOX, &C_Hom_Doc::OnWindowsStatusbox)
   ON_UPDATE_COMMAND_UI(ID_WINDOWS_STATUSBOX, &C_Hom_Doc::OnUpdateWindowsStatusbox)
   ON_COMMAND(ID_3_DEFAULTVIEW_CONTEXT, On3DefaultView)
   ON_COMMAND(ID_3_DEFAULTVIEW_STANDARD, On3DefaultView)
   ON_COMMAND(ID_3_HANDPOSTURE, &C_Hom_Doc::OnHandposture)
   ON_COMMAND(ID_HAND_SEGMENTS, &C_Hom_Doc::OnHandSegments)
   ON_COMMAND(ID_WINDOWS_ANIMATIONBAR, &C_Hom_Doc::OnWindowsAnimationbar)
   ON_UPDATE_COMMAND_UI(ID_WINDOWS_ANIMATIONBAR, &C_Hom_Doc::OnUpdateWindowsAnimationbar)
END_MESSAGE_MAP()

C_Hom_Doc::C_Hom_Doc() : mDescription(),mNeutralJointAnglesStanding(),mNeutralJointAnglesSeated(),
		mCOMLastErrorMessage(),

        Prior_Export_File_Name("summary.txt"),

        // Window Descriptors
        mFrontWindowData("Front Monitor",QD_TOPMIDDLE,VT_FRONT,"Front - View from Y Axis",TRUE),
        mSideWindowData("Sid Monitor",QD_TOPRIGHT,VT_SIDE,"Side - View from X Axis",TRUE),
        mTopWindowData("Top Monitor",QD_TOPLEFT,VT_TOP,"Top - View from Z Axis",TRUE),
        mObliqueWindowData("Main Monitor",QD_BOTTOMLEFT,VT_OBLIQUE,"Oblique",TRUE),
        mStatusResultsWindow("",QD_BOTTOMRIGHT,VT_STATUS,"Status",TRUE),
        mAnimationWindowData("",QD_ANIMATION,VT_ANIMATION,"Animation",TRUE),

		// memory leaks#2
        mExportSummary(new ExportSummary(this))
{

   /* -------------------------------------------------
   Set file storage schema title

   ... to v3.32, v3.34 to v3.37	: "Schema Version 32-2A"
   v3.33, v4.40 to current			: "Version Detecting Schema"
   */
   this->DocumentSchemaVersion = "Version Detecting Schema";

   // Reset anthropometric factors to default values
   task.initializeToDefault();



   Anthropometry anthropometry = task.getAnthropometryRef();

   anthropometry.Percentile(1);  // 1 == 50th percentile
   task.setAnthropometry(anthropometry);

   limitChoice = LC_NIOSH;
   lowerPercentileLimit = 0;
   upperPercentileLimit = 0;

   EnableAutomation();
   AfxOleLockApp();		// Prevent application destruction until this class is destroyed

   Skeleton::initializeErrors();
   JointAngles::initializeErrors();

   maleHominoidFilePresent = true;
   femaleHominoidFilePresent = true;
   shoeFilePresent = true;

   fixedHandAngles = false;
   fixedHeadAngles = false;
   maintainHandAngles = false;
   maintainHeadAngles = false;



 }

//----------------------------------------------------------------------------
C_Hom_Doc::~C_Hom_Doc()
{
    AfxOleUnlockApp();
}

//////////////////////////////////////////////////////////////////////////////
// C_Hom_Doc serialization

//----------------------------------------------------------------------------
void C_Hom_Doc::Serialize( CArchive& ArchiveRef )
{
//Need to change to 5.00 - currently same as 440
   if ( ArchiveRef.IsStoring() )
   {
      // If this assertion fails (due to making a new
      //		version), correct the next three lines for
      //		the new version.

      assert(VERSION_NUMBER_MAJOR == 7 &&
         VERSION_NUMBER_MINOR == 0);

      OpenType = 700;

      // Write the version data first
      ArchiveRef << DocumentSchemaVersion;
      ArchiveRef << VERSION_NUMBER_MAJOR << VERSION_NUMBER_MINOR;

      this->Serial_Store_v600( ArchiveRef );
   }
   else //Is loading
   {
      CString SchemaVersion;
      ArchiveRef >> SchemaVersion;
      int major_version, minor_version;

      if(SchemaVersion == "Version Detecting Schema")
      {
         // File version number identifier
         ArchiveRef >> major_version >> minor_version;

         OpenType = major_version * 100 + minor_version;
         // Let's call the right function to load this file version
         switch(major_version) 
         {
		 // we do not support anything before version 5
         /*case 4:
            switch(minor_version)
            {
            case 31:
            case 32:
            case 34:
            case 35:
            case 36:
            case 37:
               // All of these used 431
               this->Serial_Retrieve_v431_a(ArchiveRef);
               break;
            case 33:
               this->Serial_Retrieve_v433_a( ArchiveRef );
               break;
            case 40: // never released publicly
               this->Serial_Retrieve_v440( ArchiveRef );
               break;
            default:
               CString lMsg;
               lMsg.Format("File version %i.%i unreadable by 3DSSPP v%i.%i\n",
                  major_version, minor_version,
                  VERSION_NUMBER_MAJOR, VERSION_NUMBER_MINOR);
               AfxMessageBox( lMsg, MB_OK );
               AfxThrowArchiveException(CArchiveException::badSchema);
               return;
            }
            break;*/
		 case 5:
			 this->Serial_Retrieve_v440( ArchiveRef ); // 4.40 is 5.0!
			 break;
		 case 6:
			 this->Serial_Retrieve_v600( ArchiveRef );
			 break;
		 case 7:
			 this->Serial_Retrieve_v600( ArchiveRef );
			 break;
		 default:
               CString lMsg;
               lMsg.Format("File version %i.%i unreadable by 3DSSPP v%i.%i\n",
                  major_version, minor_version,
                  VERSION_NUMBER_MAJOR, VERSION_NUMBER_MINOR);
               AfxMessageBox( lMsg, MB_OK );
               AfxThrowArchiveException(CArchiveException::badSchema);
               return;
         }
		 // cjb 2/25/2011 adding because conversions in Mass class aren't set upon loading
		 // so if the file used non-earth gravity, it would show incorrect weights
		 this->UpdateConversions();
      }
      // Else it's anything pre-440 (except 433)
      else if( SchemaVersion == "Schema Version 32-2A" )
      {
         /*if (OpenType == 421)
            this->Serial_Retrieve_v42x_a( ArchiveRef );
         else
         {*/
            int retval = AfxMessageBox(
               "You are attempting to open a file saved by "
               "a previous version of 3DSSPP.  This version of "
               "3DSSPP can only open files created by 3DSSPP 5.0.0 "
               "and later.");

            /*if(retval == IDYES)
            {
               OpenType = 431;
               this->Serial_Retrieve_v440( ArchiveRef );
            }
            else
               AfxThrowArchiveException(CArchiveException::badSchema);
         }*/
      }
      else
      {
         AfxMessageBox( "Unsupported filetype.  (Version may be too old.)" );
         AfxThrowArchiveException(CArchiveException::none);	// Just get out
      }

      // Check the validity of the anthropometry data loaded
      if (OpenType != VERSION_NUMBER_MAJOR * 100 + VERSION_NUMBER_MINOR)
      {
		  Factors maleFactors = this->GetFactors();
		  Anthropometry anthropometry = task.getAnthropometryRef();
         // male and female factors names are the same
		 // check if the population data is current version and the input is not data entry. 
		if (maleFactors.mName != szDEFAULT_POPULATION_NAME && !(anthropometry.DataEntry()))
         {

		  if (std::string::npos !=
				maleFactors.mName.Find("Production") )
            {
               CString Information;
               Information =
                  "The loading task file uses default population "
                  "data from a previous version of 3DSSPP, \""
				  + maleFactors.mName
                  + "\".  The height and weight is set as DATA ENTRY ";
				AfxMessageBox(Information,MB_OK);
				AfxMessageBox( "Some factors are from a previous version of 3DSSPP; length, weight, and center of mass of the head, neck, and torso will be updated." );

			   	anthropometry.mPercentile.Value(anthropometry.cDataEntry);
				task.setAnthropometry(anthropometry);
            }
         }

		 Anthropometry defaultAnthropometry;
		 defaultAnthropometry.Sex(anthropometry.mSex.Value());
         if (!(anthropometry.DataEntry()))
		 {
			defaultAnthropometry.Percentile(anthropometry.mPercentile.Value());

			if ((anthropometry.GetWeight( MetricCns ) !=defaultAnthropometry.GetWeight(MetricCns) && anthropometry.GetWeight(MetricCns) != defaultAnthropometry.cWeightPercentiles[anthropometry.mSex.Value()][anthropometry.mPercentile.Value()])|| 
				(anthropometry.GetHeight( MetricCns ) !=defaultAnthropometry.GetHeight(MetricCns) && anthropometry.GetHeight(MetricCns) != defaultAnthropometry.cHeightPercentiles[anthropometry.mSex.Value()][anthropometry.mPercentile.Value()]))
				
             {
                  // No match!
                  anthropometry.ResetHeightPercentiles();
                  anthropometry.ResetWeightPercentiles();
                  if (AfxMessageBox("The height/weight in the task file did not"
                     " match 3DSSPP " szVERSION " gender/percentile information.  "
                     "Click YES to use the programs height/weight data, or NO to use the height/weight "
                     "from the task file, as manually entered values.", MB_YESNO)
                     == IDYES)
                  {
                     anthropometry.HeightByPercentile();
                     anthropometry.WeightByPercentile();
                  }
                  else
                  {
                     anthropometry.mPercentile.Value(anthropometry.cDataEntry);
                  }
              }
		  task.setAnthropometry(anthropometry);
		 }
      }
   }
   return;
}

//---------------------------------------------------------------------------
void C_Hom_Doc::SaveQuadFrameWindowData()
{
   WINDOWPLACEMENT place;
   Get_App_Ptr()->m_pMainWnd->GetWindowPlacement(&place);
   mMainWindowData.mRectangle = place.rcNormalPosition;
   mMainWindowData.mShowCmd = place.showCmd;	// Maximized, minimized, etc.

   mFrontWindowData.SavePosition();
   mSideWindowData.SavePosition();
   mTopWindowData.SavePosition();
   mObliqueWindowData.SavePosition();
   mAnimationWindowData.SavePosition();
   mStatusResultsWindow.SavePosition();
}

// Iterates through mReportWDMap and sets the WindowData
//		for each report
void C_Hom_Doc::SaveReportWindowData()
{
   map <int, WindowData>::iterator mapItr;

   for(mapItr = mReportWDMap.begin();
      mapItr != mReportWDMap.end();
      ++mapItr)
      GetReportWindowData(mapItr->first, mapItr->second);
}

void C_Hom_Doc::LoadQuadFrameWindowData()
{
	WINDOWPLACEMENT place;
	Get_App_Ptr()->m_pMainWnd->GetWindowPlacement(&place);
	place.rcNormalPosition = mMainWindowData.mRectangle;
	place.showCmd = mMainWindowData.mShowCmd;
	Get_App_Ptr()->m_pMainWnd->SetWindowPlacement(&place);

	mFrontWindowData.LoadPosition();
	mSideWindowData.LoadPosition();
	mTopWindowData.LoadPosition();
	mObliqueWindowData.LoadPosition();
	mAnimationWindowData.LoadPosition();
	mStatusResultsWindow.LoadPosition();
}

/*--------------------------------------------------------------*/
// NOTE: THIS FILE SHOULD ONLY CHANGE DOCUMENT DATA, NOTHING IN THE APP
//		OR ELSEWHERE!  This function will get called before loading a
//		file.  If the load fails, the document will be deleted, but
//		if we made changes to the app in this function, those changes
//		will remain even after this doc is deleted!

// Will return false if it cannot load all necessary elements.
void C_Hom_Doc::Initialize_To_Default_Data()
{
    //--- Task related variables ---
    mDescription.Task( "Untitled Task" );
	mDescription.Company( "Unknown Company" );
    mDescription.Analyst( "Unknown" );
    mDescription.Comments( "" );
	// init mDescription.TaskComplete
	int frmnum = getCurrentFrame();
	char str[80];
	char cfrmnum[39];

	sprintf(cfrmnum, "%d", frmnum);
	strcpy(str, "Untitled Task - Frame ");
	strcat(str, cfrmnum);
	mDescription.TaskComplete( LPCSTR( str ));

    //this->SetUnits( U_METRIC ); //sets the mDescription value as well	// use metric for loading files

    this->SetTitle( "Untitled" );
	Task_Mode = TM_ThreeDStatic;

    //--- Posture Prediction ---
    this->Locking_Mode		  = LM_None;

	// neutral positions are initialized with the other preset postures now
	initializePresetPostures();

    this->Obl_Light_Type_FB = 0;
    this->Obl_Light_Type_LR = 0;
    this->Obl_Light_Type_TB = 0;

    this->mUseNewLightScheme = TRUE;
    this->mLightIntensity = 60;   // Default to 60%

    this->Report_BG_Red = REPORT_BG_RED;  
    this->Report_BG_Green = REPORT_BG_GREEN;
    this->Report_BG_Blue = REPORT_BG_BLUE;

    // Set the handheld default color the same the hominoid default color
    // This may need to be changed if we add specific colors to the new hominoid
    /*this->Obl_HHO_Red = HOMINOID_RED;
    this->Obl_HHO_Green = HOMINOID_GREEN;
    this->Obl_HHO_Blue = HOMINOID_BLUE;

    this->Obl_HHO_Outline_Red = 0;
    this->Obl_HHO_Outline_Green = 0;
    this->Obl_HHO_Outline_Blue = 0;*/


	this->offsetChoice = NO_OFFSET;

    //--------------------------------------------------------------------------
    this->mBalColor = RGB(255,255,255);

    //--- All Views ---
    useSmoothManipulation = true;
    analyzeSmoothManipulation = true;

    this->mAutoTile = TRUE;
    this->mStatusBar = TRUE;

    this->Bar_Type = BT_NOBARRIER;   // None
    this->Bar_Width = 508;     // mm (20" along local X)
    this->Bar_Length = 1016;   // mm (40" along local Y)
    this->Bar_Thickness = 50;  // mm ( 2" along local Z)
    this->Bar_Distance = 508;  // mm
    this->Bar_Elevation = 508;   // mm (20")
    this->Bar_Azimuth = 0;     // deg
	this->Bar_Pitch = 90;		// deg
    //--- Reports ---

    InitReportMap();

    // --- Main Window Data ---
    CRect workArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, workArea, NULL);

    RECT newRect;
    newRect.left	= workArea.left + long(workArea.Width() * .05);
    newRect.right = workArea.left + long(workArea.Width() * .95);
    newRect.top = workArea.top + long(workArea.Height() * .05);
    newRect.bottom = workArea.top + long(workArea.Height() * .95);
    // Main window will take up 90% of width, length when restored
    mMainWindowData.mRectangle = newRect;

    mFrontWindowData.mShowCmd = SW_SHOWNORMAL;
    mSideWindowData.mShowCmd = SW_SHOWNORMAL;
    mTopWindowData.mShowCmd = SW_SHOWNORMAL;
    mObliqueWindowData.mShowCmd = SW_SHOWNORMAL;
    mAnimationWindowData.mShowCmd = SW_SHOWNORMAL;
    mStatusResultsWindow.mShowCmd = SW_SHOWNORMAL;

    mMainWindowData.mShowCmd = SW_MAXIMIZE;	// Maximized by default

	task.initializeToDefault();


    // Reset anthropometric factors to default values
	Anthropometry anthropometry = task.getAnthropometryRef();

	anthropometry.Percentile(1);  // 1 == 50th percentile
	task.setAnthropometry(anthropometry);

	//this->SetUnits( (E_Units)mDescription.Units() );

	offsetChoice = NO_OFFSET;
	jointToBeUsedAsOffset = JT_ANKLE_CENTER;

	/* for(int i = 0; i < clipboard_contents.size(); i++) {
		delete clipboard_contents[i];
	}*/

	fixedHandAngles = false;
	fixedHeadAngles = false;
	maintainHandAngles = false;
	maintainHeadAngles = false;

	clipboard_contents.clear();

	// Reset color defaults
	GLOblique::setDefaultColors();
	GLSkeleton::setDefaultColors();

	//default edc_value
	for (int i = 0; i < 4; ++i) {
		EDC_Value[i] = 0;
   }

}

/*------------------------------------------------------------*/
BOOL C_Hom_Doc::OnNewDocument()
//If something goes wrong (particularly if a memo file is present to be read)
//returning false signals the framework functions calling this function to 
//destroy and clean up the attempted document creation.
{
   if (!CDocument::OnNewDocument())
      return FALSE;

   Initialize_To_Default_Data();

   CreateViews();
   	((GLSkeleton*)GLViews[VIEW_TOP]->getGLInteractive())->setLocking(Locking_Mode);
	((GLSkeleton*)GLViews[VIEW_SIDE]->getGLInteractive())->setLocking(Locking_Mode);
	((GLSkeleton*)GLViews[VIEW_FRONT]->getGLInteractive())->setLocking(Locking_Mode);
   // this should be in Initialize_To_Default_Data(). in fact, 
   // that function sets units to metric. twice.
   this->SetUnits(U_ENGLISH); // blank documents start in English

   this->Analyze();
   this->UpdateViews();
	MainFramePtr()->Update_All_Title_Bars();

   return TRUE;
}

/*--------------------------------------------------------------*/
BOOL C_Hom_Doc::OnOpenDocument(const char *FileNamePtr)
{
   //--- Initialize document data ---
   Initialize_To_Default_Data();
   CreateViews();
   	((GLSkeleton*)GLViews[VIEW_TOP]->getGLInteractive())->setLocking(Locking_Mode);
	((GLSkeleton*)GLViews[VIEW_SIDE]->getGLInteractive())->setLocking(Locking_Mode);
	((GLSkeleton*)GLViews[VIEW_FRONT]->getGLInteractive())->setLocking(Locking_Mode);
   this->Analyze();	// So that the balance graphic will be computed.
   //		If this line is missing, and the program
   //		asks which population data to use, the
   //		program will crash.

   //NOTE: CDocument::OnOpenDocument (below) calls Serialize which assigns the 
   //document specific data. The default initialization (immediately above) is
   //necessary in the event a task file from a prior version of the program is
   //being opened. These prior versions may not have as many document variables
   //as the current version and therefore no way to properly initialize them.
   //The default initialization accomplishes this.

   if (!CDocument::OnOpenDocument(FileNamePtr))
      return FALSE;
   
   this->Analyze();
   this->UpdateViews();
   this->SetPathName(FileNamePtr);
   MainFramePtr()->Update_All_Title_Bars();

   MainFramePtr()->SynchronizeStatusBar();

   return TRUE;
}

// This will be called virtually instead of CDocument::GetFile so that we use an OfflineSafeCFile(.h, .cpp).
// this is to work around the exception thrown when CFile::GetStatus is called on an offline file
// I think MS fixed the problem now, we should test sometime (CJB)
CFile* C_Hom_Doc::GetFile(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException *pError)
{
	OfflineSafeCFile* pFile = new OfflineSafeCFile;
	ASSERT(pFile != NULL);
	if (!pFile->Open(lpszFileName, nOpenFlags, pError))
	{
		delete pFile;
		pFile = NULL;
	}
	return pFile;
}

//---------------------------------------------------------------------------
void C_Hom_Doc::OnFileSaveAs() 
{
   // The pipe evidently delimits the filter string...
   CFileDialog fileDlg(false, "", "*.tsk", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
      "3DSSPP Task Files (*.tsk)|*.tsk|All Files (*.*)|*.*||");

   // Pop up the box
   if(fileDlg.DoModal() != IDOK)
      return;

   if(!CDocument::DoSave(fileDlg.GetPathName()))
      return; // Save failed

   MainFramePtr()->Update_All_Title_Bars();
}

//---------------------------------------------------------------------------
void C_Hom_Doc::OnFileSave()
{
   // Call SaveAs if we've not yet saved
   if(this->GetTitle() == "Untitled")
   {	
      OnFileSaveAs();
   }
   else
   {
      CDocument::OnFileSave();
      MainFramePtr()->Update_All_Title_Bars();  
   }

   return;
}


// This function sets the default workspace when it is selected from the menu and 
//		when the program does not find a file already created.  It puts it in the default 
//		common application data folder (this may need to be changed if problems arise).
void C_Hom_Doc::OnDefaultWorkspace()
{
   int proceed = AfxMessageBox(DEFAULT_WORKSPACE_MSG, MB_YESNO);

   if(proceed == IDNO)
      return;

   SetDefaultWorkspace();
}

void C_Hom_Doc::SetDefaultWorkspace()
{
   BOOL wasModified = this->IsModified();
   TCHAR pszPath[MAX_PATH];
   // JTK - Make sure if you change CSIDL_APPDATA
   //    you also change it in the loading code
   GetModuleFileName(NULL, pszPath, MAX_PATH);
   PathRemoveFileSpec(pszPath);
   StringCchCatN(pszPath,MAX_PATH,DEFAULT_WORKSPACE, strlen(DEFAULT_WORKSPACE)+1);
   //PathFileExists(pszPath);
   CDocument::DoSave(pszPath, false);
   // Was not saved, only backed up - don't set as unmodified
   if(wasModified)
         this->MakeDirty();
      
   return;
}

void C_Hom_Doc::GenericColorDialog(BYTE red, BYTE green, BYTE blue)
{
   COLORREF Ini_Color = RGB( red, green, blue ); 

   CColorDialog   Diag( Ini_Color );
   int i = 0;
   bool succeed = false;
   while (succeed == false && i < 16) // 16 is the max number of color boxes
   {
      if (Diag.m_cc.lpCustColors[i] == RGB( 255, 255, 255)) // if the box is white 
      {
         Diag.m_cc.lpCustColors[i] = RGB( STICK_RED, STICK_GREEN, STICK_BLUE);
         succeed = true;
      }

      if (Diag.m_cc.lpCustColors[i] == RGB( STICK_RED, STICK_GREEN, STICK_BLUE))
      {
         succeed = true;
      }
      if (succeed == false && i == 15) // if they've define all 16 custom colors the last one
         // gets overwritten
      {
         Diag.m_cc.lpCustColors[i] = RGB( STICK_RED, STICK_GREEN, STICK_BLUE);
         succeed = true;
      }

      i++;	// incrementing i is important
   }

   if (Diag.DoModal() != IDOK)
      return;

   COLORREF New_Color = Diag.GetColor();
   if( New_Color != Ini_Color )
   {
      bool changed = false;
      if (GetRValue( New_Color ) != red)
      {
         red	= GetRValue( New_Color );
         changed = true;
      }
      if (GetGValue( New_Color ) != green)
      {
         green = GetGValue( New_Color );
         changed = true;
      }
      if (GetBValue( New_Color ) != blue)
      {
         blue = GetBValue( New_Color );
         changed = true;
      }

      if (changed)
         MakeDirtyAndUpdateViews(false);
   }
}

void C_Hom_Doc::CreateViews()
{
    // we must perform computations here so that status window will display correctly
    //mResults.Analyze();
 
    // prepare pointers for additional views       
    if(!(Get_App_Ptr()->mRunInvisible))
    {
        C_Hom_App* App_Ptr = Get_App_Ptr();
        MainFramePtr()->UpdateMainTitleBar();

        // set the window rectangle based on either the initial data or the loaded data,
        // whichever is in the mRectangle struct
        WINDOWPLACEMENT mainPlace;
        MainFramePtr()->GetWindowPlacement(&mainPlace);
        mainPlace.rcNormalPosition = mMainWindowData.mRectangle;
        MainFramePtr()->SetWindowPlacement(&mainPlace);

        // maximize main window
        MainFramePtr()->ShowWindow(mMainWindowData.mShowCmd);

        // create animation control
        App_Ptr->mNewWindowDataPtr = &mAnimationWindowData;
        CFrameWnd *animationFrame = App_Ptr->mAnimationBarDocTmpPtr->CreateNewFrame(this,NULL);
        App_Ptr->mAnimationBarDocTmpPtr->InitialUpdateFrame(animationFrame,this,TRUE);

        // create the openGL views            
        App_Ptr->mNewWindowDataPtr = &mObliqueWindowData;
        CFrameWnd *obliqueFrame = App_Ptr->mViewTemplateMap[VIEW_OBLIQUE]->CreateNewFrame(this,NULL);
		obliqueFrame->ModifyStyle(WS_MINIMIZEBOX, 0);
        App_Ptr->mViewTemplateMap[VIEW_OBLIQUE]->InitialUpdateFrame(obliqueFrame,this,TRUE);
        GLViews[VIEW_OBLIQUE] = static_cast<CGLView*>(obliqueFrame->GetActiveView());
        
        App_Ptr->mNewWindowDataPtr = &mTopWindowData;
        CFrameWnd *topFrame = App_Ptr->mViewTemplateMap[VIEW_TOP]->CreateNewFrame(this,NULL);
		topFrame->ModifyStyle(WS_MINIMIZEBOX, 0);
        App_Ptr->mViewTemplateMap[VIEW_TOP]->InitialUpdateFrame(topFrame,this,TRUE);
        GLViews[VIEW_TOP] = static_cast<CGLView*>(topFrame->GetActiveView());
        
        App_Ptr->mNewWindowDataPtr = &mFrontWindowData;
        CFrameWnd *frontFrame = App_Ptr->mViewTemplateMap[VIEW_FRONT]->CreateNewFrame(this,NULL);
		frontFrame->ModifyStyle(WS_MINIMIZEBOX, 0);
        App_Ptr->mViewTemplateMap[VIEW_FRONT]->InitialUpdateFrame(frontFrame,this,TRUE);
        GLViews[VIEW_FRONT] = static_cast<CGLView*>(frontFrame->GetActiveView());
            
        App_Ptr->mNewWindowDataPtr = &mSideWindowData;
        CFrameWnd *sideFrame = App_Ptr->mViewTemplateMap[VIEW_SIDE]->CreateNewFrame(this,NULL);
		sideFrame->ModifyStyle(WS_MINIMIZEBOX, 0);
        App_Ptr->mViewTemplateMap[VIEW_SIDE]->InitialUpdateFrame(sideFrame,this,TRUE);
        GLViews[VIEW_SIDE] = static_cast<CGLView*>(sideFrame->GetActiveView());
        
        // create the status box
        App_Ptr->mNewWindowDataPtr = &mStatusResultsWindow;
        CFrameWnd *statusFrame = App_Ptr->mStatusResultsDocTmpPtr->CreateNewFrame(this,NULL);
        App_Ptr->mStatusResultsDocTmpPtr->InitialUpdateFrame(statusFrame,this,TRUE);
		statusFrame->ModifyStyle(WS_MINIMIZEBOX, 0);

		// TODO printing
		// create the print windows
		/*App_Ptr->mNewWindowDataPtr = &mTopWindowData;
		// TODO use corresponding non-print frame as second parameter here?
		CFrameWnd* topFramePrint = App_Ptr->mPrintViewTemplateMap[VIEW_TOP]->CreateNewFrame(this, NULL);
		// false so it's invisible
		App_Ptr->mPrintViewTemplateMap[VIEW_TOP]->InitialUpdateFrame(topFramePrint, this, TRUE);
		GLPrintViews[VIEW_TOP] = static_cast<CGLView*>(topFramePrint->GetActiveView());*/

        // create the report windows
        InitialOpenReports();
    }
}

// Function to open reports upon loading a file, if reports
//		were open last time the file was open.
void C_Hom_Doc::InitialOpenReports()
{
   map <int, WindowData>::iterator mapItr;

   // Iterate through the map and open all necessary reports.
   //		mapItr->first will be the key (the int that identifies the report).
   //		mapItr->second will be the WindowData associated with that report.
   for (mapItr = mReportWDMap.begin();
      mapItr != mReportWDMap.end();
      ++mapItr)
   {
      if(mapItr->second.mIsOpen)
      {
         // Create the report window
         CFrameWnd* reportPtr = MainFramePtr()->OpenReport(mapItr->first);

         // Restore its saved size
         WINDOWPLACEMENT wp;
         reportPtr->GetWindowPlacement(&wp);
         wp.rcNormalPosition = mapItr->second.mRectangle;
         reportPtr->SetWindowPlacement(&wp);
      }
   }
}

WindowData* C_Hom_Doc::getObliqueWindowDataPtr() {
	return &mObliqueWindowData;
}

bool C_Hom_Doc::ViewsExist()
{
   if (m_viewList.IsEmpty())
      return false;
   else
      return true;
}

BOOL C_Hom_Doc::StatusBar() {
	return mStatusBar;
}

StatusBox* C_Hom_Doc::getStatusBox() {
	return mStatusResultsWindow.mFramePtr ? (StatusBox*)mStatusResultsWindow.mFramePtr->GetActiveView() : 0;
	//return (StatusBox*)mStatusResultsWindow.mFramePtr->GetActiveView();
}

bool C_Hom_Doc::IsAnimationWindowOpen() {
	return mAnimationWindowData.mFramePtr->GetActiveView() == 0 || mAnimationWindowData.mFramePtr->IsWindowVisible();
}

void C_Hom_Doc::MakeDirty()
{
   if(!IsModified())
   {
      SetModifiedFlag();
      MainFramePtr()->UpdateMainTitleBar();
   }
   //else do nothing, already dirty
}
void C_Hom_Doc::UpdateViews() {
	if ((E_Units)mDescription.Units() == U_ENGLISH )
    {
        mLocationUnits="(in)";
    }
    else
    {
        mLocationUnits="(cm)";
    }

	//TestTimer timer;
	((CGLObliqueView*)GLViews[VIEW_OBLIQUE])->updateMDP();
	//TRACE("updating MDP: %.2f\n", timer.SecondsSinceLast());

	//MainFramePtr()->Update_All_Title_Bars();

	//TRACE("update title bars: %.2f\n", timer.SecondsSinceLast());
	this->UpdateAllViews(0);
	//TRACE("update all views: %.2f\n", timer.SecondsSinceLast());
}

/*------------------------------------------------------------*/
// NOTE: This function will ALWAYS make the document dirty
//		(set it as modified), so be careful when you call it.
void C_Hom_Doc::MakeDirtyAndUpdateViews(bool analyze, bool diaphramChanged)
{
    this->MakeDirty();
	//TestTimer timer;
	if(analyze) this->Analyze(diaphramChanged);
	//TRACE("Analysis: %.2f\n", timer.SecondsSinceLast());
	this->UpdateViews();
	//TRACE("Updating views: %.2f\n", timer.SecondsSinceLast());
}

// ===========================
// C_Hom_Doc::Analyze( void );
// ============================
void C_Hom_Doc::Analyze(bool diaphramChanged)
{
    if(!analyzeSmoothManipulation) BeginWaitCursor();

	/*Analysis analysis;
	analysis.Analyze(&task);*/
	task.Analyze(diaphramChanged);

    if(!analyzeSmoothManipulation) EndWaitCursor();
}

//---------------------------------------------------------------------------
void C_Hom_Doc::DoExportSummary(fstream& aOutputFile,
                                E_Delimiter_Mode aDelimiterMode,
                                E_File_Mode aFileMode)
{
   mExportSummary->Export(aOutputFile, aDelimiterMode, aFileMode);
}

// 0 == Male, 1 == Female, -1 = Both

void C_Hom_Doc::RestoreDefaultFactors(int gender)
{
	if(gender == 0 || gender == -1) {
		Factors mMaleFactors = task.getFactors(S_Male);
		mMaleFactors.RestoreMaleDefaults();
		task.setMaleFactors(mMaleFactors);
	}

	if(gender == 1 || gender == -1) {
		Factors mFemaleFactors = task.getFactors(S_Female);
		mFemaleFactors.RestoreFemaleDefaults();
		task.setFemaleFactors(mFemaleFactors);
	}
}

// Sets aWD to the window data of the appropriate report
void C_Hom_Doc::GetReportWindowData(int aReportNum, WindowData& aWD)
{
   switch(aReportNum)
   {
   case ID_REPORTS_TASK_SUMMARY:
      ReportTaskInputSummary::GetWindowData(aWD);
      break;
   case ID_REPORTS_ANALYSIS_SUMMARY:
      ReportAnalysisSummary::GetWindowData(aWD);
      break;
   case ID_REPORTS_ANTHROPOMETRY:
      ReportAnthropometry::GetWindowData(aWD);
      break;
   case ID_REPORTS_BALANCE:
	   ReportBalanceUnified::GetWindowData(aWD);
	   break;
   case ID_REPORTS_BALANCE_STANDING:
      ReportBalanceStanding::GetWindowData(aWD);
      break;
   case ID_REPORTS_BALANCE_SEATED:
      ReportBalanceSeated::GetWindowData(aWD);
      break;
   case ID_REPORTS_FATIGUE:
      ReportFatigue::GetWindowData(aWD);
      break;
   case ID_REPORTS_POSTURE:
      ReportPosture::GetWindowData(aWD);
      break;
   case ID_REPORTS_JOINT_LOCATIONS:
      ReportJointLocations::GetWindowData(aWD);
      break;
   case ID_REPORTS_CENTEROFMASSLOCATIONS:
      ReportCoMLocations::GetWindowData(aWD);
      break;
   case ID_REPORTS_JOINT_MOMENTS:
      ReportJointMoments::GetWindowData(aWD);
      break;
   case ID_REPORTS_JOINT_FORCES:
      ReportJointForces::GetWindowData(aWD);
      break;
   case ID_REPORTS_SPINAL:
      ReportSpinal::GetWindowData(aWD);
      break;
   case ID_REPORTS_STRENGTH:
      ReportStrength::GetWindowData(aWD);
      break;
   case ID_REPORTS_STRENGTH_DIR_VECTORS:
      ReportStrengthDirVectors::GetWindowData(aWD);
      break;
   case ID_REPORTS_SAGITTAL:
      ReportSagittal::GetWindowData(aWD);
      break;
   case ID_REPORTS_3D_LOWBACK:
      ReportLowBack::GetWindowData(aWD);
      break;
   case ID_REPORTS_SHOULDERANALYSIS:
      ReportShoulderAnalysis::GetWindowData(aWD);
      break;
   case ID_REPORTS_SHOULDERGEOMETRY:
      ReportShoulderGeometry::GetWindowData(aWD);
      break;
   default:
      AfxMessageBox("Unrecognized report type in GetReportWindowData");
   }
}

//---------------------------------------------------------------------------
/*void C_Hom_Doc::CheckDemoMode()
{
   if(Get_App_Ptr()->Is_Demonstration())
   {
      // Limit to Male 5th percentile
	  Anthropometry anthropometry = task.getAnthropometryRef();
      anthropometry.Percentile(2);
      anthropometry.Sex(0);
	  task.setAnthropometry(anthropometry);
   }
}*/

LockingMode_e C_Hom_Doc::GetLockingMode() const {
	return task.getLockingMode();
}
E_Task_Mode C_Hom_Doc::GetTaskMode() const {
	return this->Task_Mode;
}

void C_Hom_Doc::setReportColor() 
{
   COLORREF Ini_Color = RGB( this->Report_BG_Red, this->Report_BG_Green, this->Report_BG_Blue ); 

   CColorDialog   Diag( Ini_Color );
   if( Diag.DoModal() == IDOK )
   {
      COLORREF New_Color = Diag.GetColor();
      if( New_Color != Ini_Color )
      {
         this->Report_BG_Red	 = GetRValue( New_Color );
         this->Report_BG_Green = GetGValue( New_Color );
         this->Report_BG_Blue  = GetBValue( New_Color );
      }
   }

   Ini_Color = RGB( this->Report_BG_Red, this->Report_BG_Green, this->Report_BG_Blue );

   mBalColor = Ini_Color;

   UpdateAllViews(NULL);
   return;
}
COLORREF C_Hom_Doc::getBalColor() const {
	return mBalColor;
}

void C_Hom_Doc::OnDg_Population(int genderValue)
{
   Dg_Population lDialog;
   lDialog.SetGender(genderValue);

   // Get out if the user cancelled
   if (lDialog.DoModal() != IDOK)
      return;

   this->MakeDirtyAndUpdateViews(true);
}

void C_Hom_Doc::OnDg_FactorsPassword()
{
   // Do not ask for the password if we're doing an alpha build;
   // just open the dialog.  This speeds up the programmer's job.
#ifdef _3DSSPP_ALPHA
   this->OnFactors();
#else
   Dg_FactorsPassword Diag;

   // Get out if the user cancels
   if (Diag.DoModal() != IDOK)
      return;

   if (Diag.mPasswd == "password7")
      this->OnFactors();
   else
      AfxMessageBox("Incorrect Access Code");

#endif
}

BOOL C_Hom_Doc::AutoTile() const {
	return this->mAutoTile;
}
void C_Hom_Doc::setAutoTile(BOOL tile) {
	mAutoTile = tile;
}

BOOL C_Hom_Doc::Is_Metric() {
	return (E_Units(mDescription.Units()) == U_METRIC);
}
BOOL C_Hom_Doc::Is_English() {
	return !this->Is_Metric();
}
CString C_Hom_Doc::getLocationUnits() const {
	return mLocationUnits;
}

void C_Hom_Doc::UpdateConversions()
{
   // Tell MASS what the current grav setting is so it can decide if it wants
   // to display LBs [earth grav] or SLUGS [all others]
	Mass::GravitySet( task.getGravity() );//mGravity );				
   if( this->Is_English() ) 
   {
      Length::SystemOfUnits( EnglishCns );
      Force:: SystemOfUnits( EnglishCns );
      Torque::SystemOfUnits( EnglishCns );
      Mass::  SystemOfUnits( EnglishCns );
   }

   else //== U_METRIC
   {
      Length::SystemOfUnits( MetricCns );
      Force:: SystemOfUnits( MetricCns );
      Torque::SystemOfUnits( MetricCns );
      Mass::  SystemOfUnits( MetricCns );
   }
}
//----------------------------------------------------------------------------
void C_Hom_Doc::SetUnits(E_Units aUnits )
{

   mDescription.Units( (int)aUnits );
   UpdateConversions();
   task.setMetricOrEnglish((aUnits == U_ENGLISH) ? EnglishCns : MetricCns);
   if(getStatusBox()) {
	   getStatusBox()->UpdateAll();
   }
   //this->getStatusBox()->UpdateAll();
   return;
}

CString C_Hom_Doc::UnitsText() {
	return Is_Metric() ? "Metric" : "English";
}

//----------------------------------------------------------------------------
CString C_Hom_Doc::ForceUnits()
{
   if( (E_Units)mDescription.Units() == U_ENGLISH )
      return	"lb";
   else
      return	"N";
}

//----------------------------------------------------------------------------
CString C_Hom_Doc::WeightUnits()
{
   if( (E_Units)mDescription.Units() == U_ENGLISH )
      return	"lb";
   else
      return	"Kg";
}

//----------------------------------------------------------------------------
CString C_Hom_Doc::LengthUnits()
{
   if( (E_Units)mDescription.Units() == U_ENGLISH )
      return	"in";
   else
      return	"cm";
}
CString C_Hom_Doc::TorqueUnits()
{
	if( (E_Units)mDescription.Units() == U_ENGLISH)
		return "in·lb";
	else
		return "N·m";

}

//----------------------------------------------------------------------------
CString C_Hom_Doc::PercentileText() const
{
	// return (char*)( task.getAnthropometryRef().PercentileValueText() +" -- UNIQUE");
	string unique;
	if(task.getAnthropometryRef().getUnique())
		unique = " -- UNIQUE";

	return (char*)( task.getAnthropometryRef().PercentileValueText() + unique.c_str());
}

//----------------------------------------------------------------------------
CString C_Hom_Doc::ReportHeader()
{
   char   lDateBuf[9]; 
   _strdate( lDateBuf ); 

   CString	lHeader;
   const Anthropometry& anthropometry = task.getAnthropometryRef();

   lHeader = lHeader 
	   + "Company: "   + mDescription.Company()//Get_App_Ptr()->Company_Name
      + ", Analyst: " + mDescription.Analyst()
      + ", Date: "    + lDateBuf
      + "\nTask: " + mDescription.TaskComplete();
   CString frameName = mDescription.FrameName(this->getCurrentFrame());
   if(frameName.GetLength() > 0) {
	   lHeader.AppendFormat(" (%s)", frameName.GetString());
   }


   if (GetFactors().mName !=
      szDEFAULT_POPULATION_NAME )
   {
      lHeader = lHeader
         + "\tPopulation: "
		 + GetFactors().mName;
   }
   lHeader = lHeader
      + "\nGender: " 
      + this->Gender() 
      + ", Percentile: " 
      + this->PercentileText() 
      + ", Height: " + anthropometry.HeightValueText() 
      + " " + anthropometry.HeightUnitsText();
   // ACT - If they're in EARTH GRAVITY, behave normally & give the weight
   if (task.getGravity() >= 9.80 && task.getGravity() < 9.82)
   {
      lHeader = lHeader + ", Weight: " + anthropometry.WeightValueText() 
         + " " + anthropometry.WeightUnitsText();
   }
   // ACT - If not... Let's give their Mass, Grav value, Earth Weight, and 'real' weight
   else
   {
	   double gravValue;
      char* accel_units;

      if( (E_Units)mDescription.Units() == U_ENGLISH ) 
      {
		 gravValue = task.getGravity() / ft_TO_in / in_TO_cm / cm_TO_m;
         accel_units = "ft/s²";
      }
      else
      {
		 gravValue = task.getGravity();
         accel_units = "m/s²";
      }

      CString WeightText;
      CString GravText;

      GravText.Format("%.1f",gravValue);

      lHeader = lHeader + ", Mass: " + anthropometry.WeightValueText()
         + " " + anthropometry.WeightUnitsText();
      lHeader = lHeader + ", Gravity: " + GravText
         + " " + accel_units;

      Force TempForce;
      WeightText.Format("%.1f",(anthropometry.Weight() * gravValue));
      lHeader = lHeader + ", Weight: " + WeightText
         + " " + TempForce.UnitsText();
   }

   lHeader = lHeader + "\nComment: " + mDescription.Comments();

   return lHeader;
}

//----------------------------------------------------------------------------
CString C_Hom_Doc::ReportFooter()
{
   CString lFooter;
   lFooter = "3DSSPP ";

   lFooter = lFooter + szVERSION
	   + " Licensed to: " + Get_App_Ptr()->GetLicenseName() + "\nCopyright " + szCOPYRIGHT_YEAR + ", The Regents of the University of Michigan - " 
      + "ALL RIGHTS RESERVED";

   return lFooter;			  
}

String C_Hom_Doc::Get_Task_Name() {
	return mDescription.Task();
}
TaskDescription & C_Hom_Doc::getDescriptionRef() {
	return mDescription;
}

//----------------------------------------------------------------------------
String C_Hom_Doc::TaskStatusText()
{
   strstream lStream;
   const Anthropometry& anthropometry = task.getAnthropometryRef();

   lStream	<< "Gender: " << this->Gender() 
      << ", Percentile: " << this->PercentileText() 

      << "\nHt (" << anthropometry.HeightUnitsText() 
      << "): "    << anthropometry.HeightValueText();
   if (task.getGravity() >= 9.80 && task.getGravity() < 9.82)
   {
      lStream	<< ", Wt (" << anthropometry.WeightUnitsText() 
         << "): "    << anthropometry.WeightValueText();
   }
   else 
   {
      lStream	<< ", Mass (" << anthropometry.WeightUnitsText() 
         << "): "    << anthropometry.WeightValueText();

      CString WeightText;
      Force TempForce;
	  // task.getGravity is always metric so we have to convert here if we're in english units
	  double weight;
	  if(this->Is_English()) {
		  weight = anthropometry.Weight() * task.getGravity() / ft_TO_in / in_TO_cm / cm_TO_m;
	  } else {
		  weight = anthropometry.Weight() * task.getGravity();
	  }
	  //WeightText.Format("%.1f",(anthropometry.Weight() * task.getGravity()));
	  WeightText.Format("%.1f", weight);
      lStream	<< ", Wt: " << WeightText
         << " " << TempForce.UnitsText();
   }

   lStream << ends;	// Null character to end the string

   String lString( lStream.str() );

   return lString;
}

// ---------------------------------------------------------------------------
// Set the task description to the given value
void C_Hom_Doc::SetDescription( const TaskDescription& aDescription )
{
   mDescription = aDescription;
   this->SetUnits((E_Units)mDescription.Units());
   
   // TODO
   // this is called here and in OnTaskSetUnits, should it just be called in C_Hom_Doc::SetUnits?
   if(mStatusResultsWindow.mFramePtr) {
	((ReportView*)mStatusResultsWindow.mFramePtr->GetActiveView())->UpdateUnits();
   }
}

//---------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////
// C_Hom_Doc diagnostics													//
//////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
/*--------------------------------------------------------------*/
void C_Hom_Doc::AssertValid() const
{
   CDocument::AssertValid();
   return;
}

/*--------------------------------------------------------------*/
void C_Hom_Doc::Dump( CDumpContext& dc ) const
{
   CDocument::Dump(dc);
}
#endif //_DEBUG

void C_Hom_Doc::DoSave() {
	OnFileSave();
}
void C_Hom_Doc::DoClose() {
	OnFileClose();
}

// makes a new copy of all selected frames and stores them in
// clipboard_contents
bool C_Hom_Doc::Copy() {
	// clear current clipboard contents
	for(int i = 0; i < clipboard_contents.size(); i++) {
		delete clipboard_contents[i];
	}
	clipboard_contents.clear();

	// get selected frames
	int left, right;
	AnimationBar* AB = ((AnimationBar*)mAnimationWindowData.mFramePtr->GetActiveView());
	AB->getSelection(left, right);

	// reserve space in vector
	clipboard_contents.reserve(right - left + 1);

	// copy selected skeletons into clipboard
	Skeleton* newSkel = 0;
	for(int i = 0; i < right - left + 1; i++) {
		newSkel = 0;
		if(task.getFrameType(left + i) != FT_Unset) {
			newSkel = new Skeleton(&task);
			*newSkel = *task.getSkeletonAtFrame(left + i);
		}
		// TODO copy frame types?
		clipboard_contents.push_back(newSkel);
	}
	return true;
}
// takes all the selected frames and puts the skeletons in
// clipboard_contents, then removes the frames
bool C_Hom_Doc::Cut() {
	// copy frames
	Copy();

	// get selection
	int left, right;
	AnimationBar* AB = ((AnimationBar*)mAnimationWindowData.mFramePtr->GetActiveView());
	AB->getSelection(left, right);

	// delete frames
	for(int i = 0; i < right - left + 1; i++) {
		task.deleteFrame(left);
	}

	// set selection to the be the one frame after
	AB->setCurrentFrame(left);
	AB->setSelection(left, left);
	this->undoHistory.clearHistory();
	return true;

}
// if clipboard_contents.empty, noop
// otherwise, prompts the user to replace any selected frames,
// or insert before the first selected frame, then replaces
// or inserts the skeletons in clipboard_contents
bool C_Hom_Doc::Paste() {
	// if there's nothing on the clipboard, don't do anything
	if(clipboard_contents.empty()) return false;

	// get selection
	int left, right;
	AnimationBar* AB = ((AnimationBar*)mAnimationWindowData.mFramePtr->GetActiveView());
	AB->getSelection(left, right);

	task.insertFrame(clipboard_contents.size(), left - 1);
	for(int i = 0; i < clipboard_contents.size(); i++) {
		if(clipboard_contents[i]) {
			task.setFrame(left + i, clipboard_contents[i], FT_Keyframe);
		}
	}

	// update selection
	AB->setCurrentFrame(task.getCurrentFrame());
	AB->setSelection(left + clipboard_contents.size(), right + clipboard_contents.size());

	changeFrame();
	
	// make fresh copy of clipboard
	Skeleton* newSkel = 0;
	for(int i = 0; i < clipboard_contents.size(); i++) {
		newSkel = 0;
		if(clipboard_contents[i]) {
			newSkel = new Skeleton(&task);
			*newSkel = *clipboard_contents[i];
		}
		clipboard_contents[i] = newSkel;
	}
	return true;
}


bool C_Hom_Doc::ClipboardEmpty() {
	return clipboard_contents.empty();
}


void C_Hom_Doc::SetHandType(E_Hand_Type new_Hand) {
	task.Put_Hand_Type(new_Hand, new_Hand);
}


LimitChoice C_Hom_Doc::getLimitChoice() 
{ 
	return limitChoice;
}

PictureOrColor C_Hom_Doc::getPictureOrColorChoice() 
{ 
	return pictureOrColorChoice;
}


void C_Hom_Doc::setPictureOrColorChoice(PictureOrColor pOrC) 
{ 
	 pictureOrColorChoice = pOrC;
}

void C_Hom_Doc::setLimitChoice(LimitChoice lc) 
{
	limitChoice = lc;
	((StatusBox*)mStatusResultsWindow.mFramePtr->GetActiveView())->UpdatePercentileLimits();
}

int C_Hom_Doc::getLowerPercentileLimit() 
{ 
	return lowerPercentileLimit; 
}

int C_Hom_Doc::getFemaleLowerPercentileLimit() 
{ 
	return femaleLowerPercentileLimit; 
}

void C_Hom_Doc::setLowerPercentileLimit(int LL) 
{ 
	lowerPercentileLimit = LL;
	((StatusBox*)mStatusResultsWindow.mFramePtr->GetActiveView())->UpdatePercentileLimits();
}

void C_Hom_Doc::setFemaleLowerPercentileLimit(int LL) 
{ 
	femaleLowerPercentileLimit = LL;
	((StatusBox*)mStatusResultsWindow.mFramePtr->GetActiveView())->UpdatePercentileLimits();
}

int C_Hom_Doc::getUpperPercentileLimit() 
{ 
	return upperPercentileLimit; 
}

int C_Hom_Doc::getFemaleUpperPercentileLimit() 
{ 
	return femaleUpperPercentileLimit; 

}

void C_Hom_Doc::setUpperPercentileLimit(int UL) 
{ 
	upperPercentileLimit = UL;
	((StatusBox*)mStatusResultsWindow.mFramePtr->GetActiveView())->UpdatePercentileLimits();
}
	
void C_Hom_Doc::setFemaleUpperPercentileLimit(int UL) 
{ 
	femaleUpperPercentileLimit = UL;
	((StatusBox*)mStatusResultsWindow.mFramePtr->GetActiveView())->UpdatePercentileLimits();
}


Vector3 C_Hom_Doc::getOffset(Skeleton* skel, bool forceMetric) {
	if(skel == 0) skel = task.getSkeleton();

	Vector3 defaultOrigin;
	if(task.getPosition() == P_Standing)
		defaultOrigin = skel->getJoint(JT_ANKLE_CENTER, true);
	else
		defaultOrigin = skel->getSeatReference(true);

	Vector3 ret;
	switch(offsetChoice) {
		case NO_OFFSET:
			ret = defaultOrigin;
			break;
		case COORD_OFFSET_BODY_PART:
			ret = skel->getJoint(jointToBeUsedAsOffset, true);
			break;
		/*case COORD_OFFSET_NUMBER:
			ret = defaultOrigin + offsetCoordinate;
			break;*/
	}
	if(addOffset) {
		ret += offsetCoordinate;
	}
	if(!forceMetric && Is_English()) {
		return ret * CM_TO_IN;
	}
	return ret;
}

OffsetChoice C_Hom_Doc::getOffsetChoice() 
{ 
	return offsetChoice; 
}

void C_Hom_Doc::setOffsetChoice(OffsetChoice rc) 
{
	offsetChoice = rc;
}

Vector3 C_Hom_Doc::getOffsetCoordinate(bool forceMetric)
{ 
	if(!forceMetric && Is_English()) {
		return offsetCoordinate * CM_TO_IN;
	}
	return offsetCoordinate; 
}
bool C_Hom_Doc::getAddOffset() {
	return addOffset;
}
void C_Hom_Doc::setAddOffset(bool AO) {
	addOffset = AO;
}

void C_Hom_Doc::setOffsetCoordinate(Vector3 OC, bool forceMetric) 
{ 
	offsetCoordinate = OC;
	if(!forceMetric && Is_English()) {
		offsetCoordinate *= IN_TO_CM;
	}
	task.offsetCoordinate = offsetCoordinate;
}

JointID C_Hom_Doc::getOffsetJoint() 
{ 
	return jointToBeUsedAsOffset; 
}

void C_Hom_Doc::setOffsetJoint(JointID joint) 
{
	jointToBeUsedAsOffset = joint;
}


//---------------------------------------------------------------------------
bool C_Hom_Doc::UndoJointAngleChange() 
{
   if(!undoHistory.undo())
      return false;

   this->MakeDirtyAndUpdateViews(true);

   return true;
}

//---------------------------------------------------------------------------
bool C_Hom_Doc::RedoJointAngleChange()
{
   if(!undoHistory.redo())
      return false;

   this->MakeDirtyAndUpdateViews(true);

   return true;
}

//---------------------------------------------------------------------------
BOOL C_Hom_Doc::PredictPosture(const Vector3 &aLeftHandRef,
                               const Vector3 &aRightHandRef)
{
    JointAngles oldAngles,newAngles;
    int frame = task.getCurrentFrame();
	oldAngles = GetSkeleton()->GetAngles();

    if(GetSkeleton()->Predict_Posture_Angles(aLeftHandRef,aRightHandRef))
    {
        newAngles = GetSkeleton()->GetAngles();
		UndoablePostureChange *undoEvent = new UndoablePostureChange(frame,oldAngles,newAngles,this->getFrameType(frame), FT_Keyframe);
        addUndoEvent(undoEvent);

        return TRUE;
    }

    return FALSE;
}

JointAngles C_Hom_Doc::getNeutralJointAngles() const
{
	if(task.getPosition() == P_Sitting) return mNeutralJointAnglesSeated;
	else return mNeutralJointAnglesStanding;
}

JointAngles C_Hom_Doc::getTNeutralJointAngles() const
{
	if(task.getPosition() == P_Sitting) return mNeutralJointAnglesSeated;
	else return mNeutralJointAnglesTStanding;
}

int C_Hom_Doc::LeftSelect() {
	int left, right;
	AnimationBar* AB = ((AnimationBar*)mAnimationWindowData.mFramePtr->GetActiveView());
	AB->getSelection(left, right);
	return left;
}
int C_Hom_Doc::RightSelect() {
	int left, right;
	AnimationBar* AB = ((AnimationBar*)mAnimationWindowData.mFramePtr->GetActiveView());
	AB->getSelection(left, right);
	return right;
}
void C_Hom_Doc::setSelection(int left, int right) {
	((AnimationBar*)mAnimationWindowData.mFramePtr->GetActiveView())->setSelection(left, right);
}

Task& C_Hom_Doc::getTask() {
	return task;
}

const Support_s& C_Hom_Doc::getSupport() const {
	return task.getSupport();
}
void C_Hom_Doc::setSupport(const Support_s& Support) {
	task.setSupport(Support);
}
FeetSupport_e C_Hom_Doc::getFeetSupport() const {
	return task.getFeetSupport();
}
void C_Hom_Doc::setFeetSupport(FeetSupport_e FeetSupport) {
	task.setFeetSupport(FeetSupport);
}
Position_e C_Hom_Doc::getPosition() const {
	return task.getPosition();
}
void C_Hom_Doc::setPosition(Position_e Position) {
	task.setPosition(Position);
}
bool C_Hom_Doc::hasFrontSeatPanSupport() const {
	return task.hasFrontSeatPanSupport();
}
void C_Hom_Doc::setFrontSeatPanSupport(bool value) {
	task.setFrontSeatPanSupport(value);
}
bool C_Hom_Doc::hasSeatBackRest() const {
	return task.hasSeatBackRest();
}
void C_Hom_Doc::setSetBackRest(bool value) {
	task.setBackRest(value);
}
Length C_Hom_Doc::getBackRestHeight() const {
	Length SeatHeight;
	SeatHeight.Value(task.getBackRestHeight());
	return SeatHeight;
}
void C_Hom_Doc::setBackRestHeight(double BackRestHeight) {
	task.setBackRestHeight(BackRestHeight);
}

double C_Hom_Doc::getGravity() const {
	return task.getGravity();
}
void C_Hom_Doc::setGravity(double new_Gravity) {
	task.setGravity(new_Gravity);
	((StatusBox*)mStatusResultsWindow.mFramePtr->GetActiveView())->UpdateGravity();
}

/*------------------------------------------------------------*/
const Anthropometry& C_Hom_Doc::getAnthropometryRef() {
	return task.getAnthropometryRef();
}

//---------------------------------------------------------------------------
void C_Hom_Doc::setAnthropometry( const Anthropometry& aAnthropometry )
{
	task.setAnthropometry(aAnthropometry);
}

String C_Hom_Doc::Gender() {
	return task.getAnthropometryRef().SexValueText();
}
int C_Hom_Doc::GenderCode() const {
	return task.getAnthropometryRef().Sex();
}

const Factors& C_Hom_Doc::GetFactors() const {
	return task.getFactors();
}
const Factors& C_Hom_Doc::GetFactors(int genderCode) const {
	return task.getFactors((genderCode == 0) ? S_Male : S_Female);
}
void C_Hom_Doc::SetMaleFactors(const Factors& newFactors) {
	task.setMaleFactors(newFactors);
}
void C_Hom_Doc::SetFemaleFactors(const Factors& newFactors) {
	task.setFemaleFactors(newFactors);
}

void C_Hom_Doc::setLinkLengths(LinkLengths & newLengths)
{
    task.setLinkLengths(newLengths);
}
void C_Hom_Doc::setLinkCGs(LinkCGs & newCGs)
{
    task.setLinkCGs(newCGs);
}
void C_Hom_Doc::setLinkWeights(LinkWeights & newWeights)
{
    task.setLinkWeights(newWeights);
}
const LinkLengths& C_Hom_Doc::getLinkLengths() const {
	return task.getLinkLengths();
}
const LinkWeights& C_Hom_Doc::getLinkWeights() const {
	return task.getLinkWeights();
}
const LinkCGs& C_Hom_Doc::getLinkCGs() const {
	return task.getLinkCGs();
}

Skeleton* C_Hom_Doc::GetSkeleton(void) {
	return task.getSkeleton();
}

Skeleton* C_Hom_Doc::getSkeletonAtFrame(int frameNumber) {
	return task.getSkeletonAtFrame(frameNumber);
}
void C_Hom_Doc::setFrame(int frameNumber, Skeleton* skeleton, FrameType_e FrameType) {
	task.setFrame(frameNumber, skeleton, FrameType);
}
void C_Hom_Doc::setFrameType(FrameType_e FrameType, int frameNumber) {
	task.setFrameType(FrameType, frameNumber);
}
FrameType_e C_Hom_Doc::getFrameType() const {
	return task.getFrameType();
}
FrameType_e C_Hom_Doc::getFrameType(int frameNumber) const {
	return task.getFrameType(frameNumber);
}

// returns true if it sets the current frame to be 0, or if it already is 0
bool C_Hom_Doc::nextFrame() {
	bool ret = task.nextFrame();
	mStatusResultsWindow.mFramePtr->PostMessage(WM_UPDATE_TITLE_BAR);
	return ret;
}
// return true if it sets the current frame to be lastFrame.
bool C_Hom_Doc::previousFrame() {
	bool ret = task.previousFrame();
	mStatusResultsWindow.mFramePtr->PostMessage(WM_UPDATE_TITLE_BAR);
	return ret;
}

// changes which frame is to be manipulated
void C_Hom_Doc::setCurrentFrame(int frameNumber) {
	if(Dg_SupportSelection::frames.size() < 100)
	Dg_SupportSelection::init();
	if(frameNumber >Dg_SupportSelection::frames.size())
	{
		do
		{
			Dg_SupportSelection::init();
		} while (frameNumber >= Dg_SupportSelection::frames.size());
	}
	task.setCurrentFrame(frameNumber);
	if(getFrameType(frameNumber)==FT_Unset && getFrameType(frameNumber-1)==FT_Keyframe)
	{
		Dg_SupportSelection::frames[frameNumber].feetsupport=Dg_SupportSelection::frames[frameNumber-1].feetsupport;
	}
	Support_s support=task.getSupport();
	support.feetSupport =Dg_SupportSelection::frames[frameNumber].feetsupport;
	task.setSupport(support);
	MakeDirtyAndUpdateViews(TRUE);
	mStatusResultsWindow.mFramePtr->PostMessage(WM_UPDATE_TITLE_BAR);
	this->UpdateViews();
}
// gets the frame number of the frame being manipulated
int C_Hom_Doc::getCurrentFrame() {
	return task.getCurrentFrame();
}
// gets the last frame which is not empty
int C_Hom_Doc::getLastFrame() {
	return task.getLastFrame();
}
void C_Hom_Doc::changeFrame() {
	mStatusResultsWindow.mFramePtr->SendMessage(WM_UPDATE_TITLE_BAR);
}
/*
bool C_Hom_Doc::predictMotion(const MotionPredictionInfo& info_in) {
	MotionPredictionInfo info = info_in;
	if(task.predictMotion(info)) {
		return true;
	}
	return false;
}
void C_Hom_Doc::updateMotion(int frameNumber) {
	task.updateMotion(frameNumber);
}
void C_Hom_Doc::cancelMotion(int frameNumber) {
	task.cancelMotion(frameNumber);
}*/

void C_Hom_Doc::addUndoEvent(UndoableEvent *event) {
	undoHistory.addUndoableEvent(event);
}
void C_Hom_Doc::clearUndoHistory(void) {
	undoHistory.clearHistory();
}
bool C_Hom_Doc::CanUndo() const {
	return undoHistory.canUndo();
}
bool C_Hom_Doc::CanRedo() const {
	return undoHistory.canRedo();
}

void C_Hom_Doc::cueAngle(const AngleCueData& ACD) {
	for(int i = 0; i < 4; ++i) {
		GLViews[i]->cueAngle(ACD);
	}
	this->UpdateAllViews( NULL );
}
void C_Hom_Doc::cancelCue() {
	for(int i = 0; i < 4; ++i) {
		GLViews[i]->cancelCue();
	}
	this->UpdateAllViews(NULL);
}

int C_Hom_Doc::GetOblLightTypeLR() {
	return Obl_Light_Type_LR;
}
int C_Hom_Doc::GetOblLightTypeFB() {
	return Obl_Light_Type_FB;
}
int C_Hom_Doc::GetOblLightTypeTB() {
	return Obl_Light_Type_TB;
}

/*------------------------------------------------------------*/
void C_Hom_Doc::ObliqueViewLightUpdate(Dg_Light& aDialogRef)
{
    this->Obl_Light_Type_FB = aDialogRef.m_FrontBack_Value;
    this->Obl_Light_Type_LR = aDialogRef.m_LeftRight_Value;
    this->Obl_Light_Type_TB = aDialogRef.m_TopBottom_Value;
	this->mUseNewLightScheme = (aDialogRef.lightRadio == 1);
    this->mLightIntensity = aDialogRef.mIntensity;

    this->MakeDirtyAndUpdateViews(false);	
}

/*------------------------------------------------------------*/
void C_Hom_Doc::ObliqueViewEnvironmentUpdate(Dg_Environment& aDialogRef)
{
   C_Hom_App& App = *Get_App_Ptr();
   CGLObliqueView *oblique = (CGLObliqueView*)GLViews[VIEW_OBLIQUE];
   Length lLocX(0,1), lLocY(0,1), lLocZ(0,1);

   //--- Update floor data ---
   oblique->setFloorEnabled((bool)aDialogRef.Floor_Visible);

   if(getPosition() == P_Sitting)
   {
	   Vector3 seatRef = GetSkeleton()->getSeatReference() - GetSkeleton()->getJoint(JT_ANKLE_CENTER);
        // Get the lowest ankle
		double lowestAnkle[3];
        GetSkeleton()->C4EPoint(JT_ANKLE_CENTER, lLocX, lLocY, lLocZ);
        lowestAnkle[0] = lLocX.Value();
        lowestAnkle[1] = lLocY.Value();
        lowestAnkle[2] = lLocZ.Value();

        aDialogRef.Bar_Distance += seatRef[1] - lowestAnkle[1];
        aDialogRef.Bar_Elevation += seatRef[2] - lowestAnkle[2];
   }

   oblique->getHandHeld().outline = aDialogRef.outline;

   float Units_Conversion = ((E_Units)mDescription.Units() == U_ENGLISH) ? U_IN_MM : U_CM_MM;

   oblique->getHandHeld().height = (float)aDialogRef.Handheld_Height * Units_Conversion;
   oblique->getHandHeld().depth = (float)aDialogRef.Handheld_Width * Units_Conversion;
   oblique->getHandHeld().type = aDialogRef.Handheld_Type;

   // Check Colors
   GLOblique::handHeldColor[0] = aDialogRef.red;
   GLOblique::handHeldColor[1] = aDialogRef.green;
   GLOblique::handHeldColor[2] = aDialogRef.blue;

   GLOblique::handHeldOutlineColor[0] = aDialogRef.outlineRed;
   GLOblique::handHeldOutlineColor[1] = aDialogRef.outlineGreen;
   GLOblique::handHeldOutlineColor[2] = aDialogRef.outlineBlue;

   this->Bar_Type = (E_Barrier_Type)aDialogRef.Bar_Type;

   this->Bar_Distance = (float)aDialogRef.Bar_Distance * Units_Conversion;
   this->Bar_Azimuth = aDialogRef.Bar_Azimuth;
   this->Bar_Elevation = (float)aDialogRef.Bar_Elevation * Units_Conversion;
   this->Bar_Pitch = aDialogRef.Bar_Pitch;
   this->Bar_Length = (float)aDialogRef.Bar_Length * Units_Conversion;
   this->Bar_Width = (float)aDialogRef.Bar_Width * Units_Conversion;
   this->Bar_Thickness = (float)aDialogRef.Bar_Thickness * Units_Conversion;

   this->MakeDirtyAndUpdateViews(false); 

   if(getPosition() == P_Sitting)
   {
	   Vector3 seatRef = GetSkeleton()->getSeatReference() - GetSkeleton()->getJoint(JT_ANKLE_CENTER);
        // Get the lowest ankle
	   double lowestAnkle[3];
        GetSkeleton()->C4EPoint(JT_ANKLE_CENTER, lLocX, lLocY, lLocZ);
        lowestAnkle[0] = lLocX.Value();
        lowestAnkle[1] = lLocY.Value();
        lowestAnkle[2] = lLocZ.Value();

        aDialogRef.Bar_Distance -= seatRef[1] + lowestAnkle[1];
        aDialogRef.Bar_Elevation -= seatRef[2] + lowestAnkle[2];
   }	  
}

BOOL C_Hom_Doc::UsingNewLightScheme() {
	return mUseNewLightScheme;
}
int C_Hom_Doc::GetLightIntensity() {
	return mLightIntensity;
}
E_Barrier_Type C_Hom_Doc::getBarType() const {
	return Bar_Type;
}
int C_Hom_Doc::getBarLength() const {
	return Bar_Length;
}
int C_Hom_Doc::getBarWidth() const {
	return Bar_Width;
}
int C_Hom_Doc::getBarThickness() const {
	return Bar_Thickness;
}
int C_Hom_Doc::getBarDistance() const {
	return Bar_Distance;
}
int C_Hom_Doc::getBarAzimuth() const {
	return Bar_Azimuth;
}
int C_Hom_Doc::getBarPitch() const {
	return Bar_Pitch;
}
int C_Hom_Doc::getBarElevation() const {
	return Bar_Elevation;
}

bool C_Hom_Doc::getMaleHominoidFilePresent() {
	return maleHominoidFilePresent;
}
bool C_Hom_Doc::getFemaleHominoidFilePresent() {
	return femaleHominoidFilePresent;
}
bool C_Hom_Doc::getShoeFilePresent() {
	return shoeFilePresent;
}
void C_Hom_Doc::setMaleHomoidFilePresent(bool present) {
	maleHominoidFilePresent = present;
}
void C_Hom_Doc::setFemaleHominoidFilePresent(bool present) {
	femaleHominoidFilePresent = present;
}
void C_Hom_Doc::setShoeFilePresent(bool present) {
	shoeFilePresent = present;
}

bool C_Hom_Doc::getFixedHandAngles() {
	return fixedHandAngles;
}

bool C_Hom_Doc::getFixedHeadAngles() {
	return fixedHeadAngles;
}

void C_Hom_Doc::setFixedHandAngles(bool iFixedHandAngles) {
	fixedHandAngles = iFixedHandAngles;
}
void C_Hom_Doc::setFixedHeadAngles(bool iFixedHandAngles) {
	fixedHeadAngles = iFixedHandAngles;
}
bool C_Hom_Doc::getMaintainHandAngles() {
	return maintainHandAngles;
}
bool C_Hom_Doc::getMaintainHeadAngles() {
	return maintainHeadAngles;
}
void C_Hom_Doc::setMaintainHandAngles(bool iMaintainHandAngles) {
	maintainHandAngles = iMaintainHandAngles;
}
void C_Hom_Doc::setMaintainHeadAngles(bool iMaintainHeadAngles) {
	maintainHeadAngles = iMaintainHeadAngles;
}

//////////////////////////////////////////////////////////////////////////////
// C_Hom_Doc Support														//
////////////////////////////////////////////////////////////////////////////// 

void C_Hom_Doc::InitReportMap()
{
   // Set all reports to "not open"
   mReportWDMap[ID_REPORTS_ANALYSIS_SUMMARY].mIsOpen = FALSE;
   mReportWDMap[ID_REPORTS_ANTHROPOMETRY].mIsOpen = FALSE;
   mReportWDMap[ID_REPORTS_BALANCE].mIsOpen = FALSE;
   mReportWDMap[ID_REPORTS_BALANCE_STANDING].mIsOpen = FALSE;
   mReportWDMap[ID_REPORTS_BALANCE_SEATED].mIsOpen = FALSE;
   mReportWDMap[ID_REPORTS_FATIGUE].mIsOpen = FALSE;
   mReportWDMap[ID_REPORTS_POSTURE].mIsOpen = FALSE;
   mReportWDMap[ID_REPORTS_JOINT_LOCATIONS].mIsOpen = FALSE;
   mReportWDMap[ID_REPORTS_CENTEROFMASSLOCATIONS].mIsOpen = FALSE;
   mReportWDMap[ID_REPORTS_JOINT_MOMENTS].mIsOpen = FALSE;
   mReportWDMap[ID_REPORTS_JOINT_FORCES].mIsOpen = FALSE;
   mReportWDMap[ID_REPORTS_3D_LOWBACK].mIsOpen = FALSE;
   mReportWDMap[ID_REPORTS_SAGITTAL].mIsOpen = FALSE;
   mReportWDMap[ID_REPORTS_SPINAL].mIsOpen = FALSE;
   mReportWDMap[ID_REPORTS_STRENGTH].mIsOpen = FALSE;
   mReportWDMap[ID_REPORTS_STRENGTH_DIR_VECTORS].mIsOpen = FALSE;
   mReportWDMap[ID_REPORTS_TASK_SUMMARY].mIsOpen = FALSE;
   #if defined(SHOULDERDLL)
   mReportWDMap[ID_REPORTS_SHOULDERANALYSIS].mIsOpen = FALSE;
   #endif

}

void C_Hom_Doc::initializePresetPostures()
{
	mStandLift.Assign(90, 0, 90, 0, 90, -60, 90, -60, //arms
						90, -90, 90, -90, 90, -90, 90, -90,//legs
						90, 0, 0, //trunk
						0,0,-20,0,-20,0, // clavicle stuff
						90, 0, 90, 0, // feet
						90, 0, 0, 90, 0, 0, // hands
						0, 90, 0, 0, 90, 0, 74); // head and neck
	mStoopLift.Assign(90, -60, 90, -60, -90, -70, -90, -70, // arms
						90, -80, 90, -80, 90, -80, 90, -80, // legs
						18, 0, 0, // trunk
						0,0,-20,0,-20,0, // clavicle stuff
						90, 0, 90, 0, // feet
						90, -60, 0, 90, -60, 0, // hands
						0, 90, 0, 0, 90, 0, 74); // head and neck
	mSquatLift.Assign(99, -68, 99, -68, 73, -76, 73, -76, // arms
						90, -3, 90, -3, -89, -52, -89, -52, // legs
						60, 0, 0, // trunk
						0,0,-20,0,-20,0, // clavicle stuff
						90, 0, 90, 0, // feet
						99, -68, 0, 99, -68, 0, // hands
						0, 90, 0, 0, 90, 0, 74); // head and neck
	mForwardPush.Assign(90, 10, 90, 10, 90, -10, 90, -10, // arms
						90, -70, -90, -60, -90, -40, -90, -50, // legs
						63, 0, 0, // trunk
						0,0,-20,0,-20,0, // clavicle stuff
						90, 0, 90, 0, // feet
						90, 10, 0, 90, 10, 0, // hands
						0, 90, 0, 0, 90, 0, 74); // head and neck
	mBackwardPull.Assign(90, 10, 90, 10, 90, -10, 90, -10, // arms
						90, -45, 90, -65, 90, -65, -90, -40, // legs
						108, 0, 0, // trunk
						0,0,-20,0,-20,0, // clavicle stuff
						90, 0, 90, 0, // feet
						90, 10, 0, 90, 10, 0, // hands
						0, 90, 0, 0, 90, 0, 74); // head and neck
	mNeutralJointAnglesStanding.Assign(90, -90, 90, -90, 0, -90, 0, -90, // arms
										90, -90, 90, -90, 90, -90, 90, -90, // legs
										90, 0, 0, // trunk
										0,0,-20,0,-20,0, // clavicle stuff
										90, 0, 90, 0, // feet
										90, -90, 0, 90, -90, 0, // hands
										0, 90, 0, 0, 90, 0, 74); // head and neck
	mManipulationJointAnglesStart.Assign(100, -30, 100, -30, 0, -45, 0, -45, // arms
										90, -90, 90, -90, 90, -90, 90, -90, // legs
										90, 0, 0, // trunk
										0,0,-19,0,-19,0, // clavicle stuff
										90, 0, 90, 0, // feet
										100, -30, 0, 100, -30, 0, // hands
										0, 90, 0, 0, 90, 0, 74); // head and neck
	mNeutralJointAnglesTStanding.Assign(0, -0, 0, -0, 0, -0, 0, -0, // arms
										90, -90, 90, -90, 90, -90, 90, -90, // legs
										90, 0, 0, // trunk
										0,0,-20,0,-20,0, // clavicle stuff
										90, 0, 90, 0, // feet
										0, -0, 0, 0, -0, 0, // hands
										0, 90, 0, 0, 90, 0, 74); // head and neck
	mNeutralJointAnglesSeated.Assign(90, 0, 90, 0, 90, -90, 90, -90, // arms
										90, 0, 90, 0, 90, -90, 90, -90, // legs
										90, 0, 0, // trunk
										0,0,-20,0,-20,0, // clavicle stuff
										90, 0, 90, 0,  // feet
										90, 0, 0, 90, 0, 0, // hands
										0, 90, 0, 0, 90, 0, 74); // head and neck
}


//////////////////////////////////////////////////////////////////////////////
// C_Hom_Doc Commands
//////////////////////////////////////////////////////////////////////////////

void C_Hom_Doc::OnImportLocFile()
{
    CFileDialog fileDlg(TRUE, NULL, "*.loc", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
	    "HUMOSIM Loc Files (*.loc)|*.loc|Standard Loc Files (*.sloc)|*.sloc|All Files (*.*)|*.*||");

    // Show the file selection box
    if(fileDlg.DoModal() != IDOK) return;
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	CString extension;
	_splitpath(fileDlg.GetPathName(),drive,dir,fname,ext);
	extension = ext;
    task.readLocFile(fileDlg.GetPathName(),extension);
    MakeDirtyAndUpdateViews(true);

}


void C_Hom_Doc::OnPredictMotion()
{
    Dg_MotionPrediction diag(NULL);
    diag.DoModal();
}

/*------------------------------------------------------------*/
void C_Hom_Doc::OnPostureAnthropometry()
{
	Dg_Anthropometry Diag(this, (StatusBox*)mStatusResultsWindow.mFramePtr->GetActiveView());
    Diag.DoModal();
	((StatusBox*)mStatusResultsWindow.mFramePtr->GetActiveView())->UpdatePercentileLimits();
	/*if(Diag.getvaluechanged() == True)
	{
		Diag.OnBnClickedAnthro();
	}*/
}

/*------------------------------------------------------------*/
void C_Hom_Doc::OnPostureJointAngles()
{
	Dg_BodyAnglesDyn Diag(this);
	Diag.DoModal();
}

/*------------------------------------------------------------*/
void C_Hom_Doc::OnPosturePrediction()
{
   Dg_PosturePrediction Diag( this );
   Diag.DoModal();
}

/*------------------------------------------------------------*/
void C_Hom_Doc::OnPostureLockingMode()
{ 
    Dg_LockingMode Diag;

	this->Locking_Mode = task.getLockingMode();
    Diag.Locking_Mode = static_cast<int> (this->Locking_Mode);
    int DLM = Diag.Locking_Mode;

    // Get out if the user cancels
    if (Diag.DoModal() != IDOK)
        return;

    DLM = Diag.Locking_Mode;
    this->Locking_Mode = (LockingMode_e)DLM;
	task.setLockingMode(this->Locking_Mode);

	((GLSkeleton*)GLViews[VIEW_TOP]->getGLInteractive())->setLocking(Locking_Mode);
	((GLSkeleton*)GLViews[VIEW_SIDE]->getGLInteractive())->setLocking(Locking_Mode);
	((GLSkeleton*)GLViews[VIEW_FRONT]->getGLInteractive())->setLocking(Locking_Mode);

    this->MakeDirtyAndUpdateViews(true);
}

/*------------------------------------------------------------*/
void C_Hom_Doc::OnPostureHandLoads()
{
	Dg_HandLoads Diag(this);
	Diag.DoModal();
	
	//check for opening advanced hand loads
	if (Diag.openAdvHandLoads == TRUE)
	{
		Dg_HandLoads_Adv   Diag( this );
		Diag.DoModal();
	}
}


/*------------------------------------------------------------*/
/*
Moved functionality to a button in regular hand loads dialog - cslong 6/4/14


void C_Hom_Doc::OnPostureHandLoadsAdv()
{
	Dg_HandLoads_Adv   Diag( this );
	Diag.DoModal();

}*/


/*------------------------------------------------------------*/
void C_Hom_Doc::OnObliqueViewHuman()
{
    Dg_ObliqueFigure Diag;
    
    //Initialize dialog variables
    switch(((CGLObliqueView*)GLViews[VIEW_OBLIQUE])->getModelType())
    {
        case MT_FLESH:
            Diag.Model_Mode = 0; // Number corresponds to its order in the dialog
            break;
        case MT_WIREFRAME:
            Diag.Model_Mode = 1;
            break;
        case MT_NONE:
            Diag.Model_Mode = 2;
            break;
    }

    // The dialog updates the document itself;
    //    just return after dialog closes
    Diag.DoModal();
}

void C_Hom_Doc::OnObliqueViewCamera() {

   Dg_CameraOblique Diag(this);
   Diag.DoModal();
}

/*------------------------------------------------------------*/
void C_Hom_Doc::OnObliqueViewLight()
{
   //Create and initilize dialog

   Dg_Light Diag(this);

   Diag.m_FrontBack_Value = this->Obl_Light_Type_FB;
   Diag.m_LeftRight_Value = this->Obl_Light_Type_LR;
   Diag.m_TopBottom_Value = this->Obl_Light_Type_TB;
   Diag.lightRadio = (this->mUseNewLightScheme) ? 1 : 0;
   Diag.mIntensity = this->mLightIntensity;

   if (Diag.DoModal() == IDOK)
      this->ObliqueViewLightUpdate( Diag );
}

//----------------------------------------------------------------------------
void C_Hom_Doc::OnFileClose()
{
   CDocument::OnCloseDocument();
   return;
}

void C_Hom_Doc::OnOrthoBackground()
{
    COLORREF Ini_Color = RGB(GLSkeleton::backgroundColor[0],GLSkeleton::backgroundColor[1],
                             GLSkeleton::backgroundColor[2]);

    CColorDialog Diag( Ini_Color );
    if( Diag.DoModal() == IDOK )
    {
        COLORREF New_Color = Diag.GetColor();
        if( New_Color != Ini_Color )
        {
            GLSkeleton::backgroundColor[0] = GetRValue( New_Color );
            GLSkeleton::backgroundColor[1] = GetGValue( New_Color );
            GLSkeleton::backgroundColor[2] = GetBValue( New_Color );
            this->MakeDirtyAndUpdateViews(false);
        }
    }  
}

void C_Hom_Doc::OnReportsExportsummary()
{
   /*CString	 Filters( "Text Files (*.txt) | *.txt ||" );
   CFileDialog	 File_Dialog
      ( 
      FALSE,//save as box
      ".txt", //default extension
      this->Prior_Export_File_Name, //default file name
      OFN_HIDEREADONLY,
      Filters, //filters
      NULL  //parent window
      );*/

   //if( File_Dialog.DoModal() == IDOK )
   //{
      //In the following, the Export_Dialog object transfers via DDX its 
      //data to the ExportSummary object provided in the argument list.

	Dg_ExportOptions	Export_Dialog( *mExportSummary, 0 );

	if( Export_Dialog.DoModal() == IDOK ) {

		CString	 Filters( "Text Files (*.txt) | *.txt ||" );
		CFileDialog	 File_Dialog (
			FALSE,//save as box
			".txt", //default extension
			this->Prior_Export_File_Name, //default file name
			OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
			Filters, //filters
			NULL  //parent window
			);

		if(File_Dialog.DoModal() == IDOK) {

         this->Prior_Export_File_Name = File_Dialog.GetPathName();

         //get export options

         int	 Open_Options;
         if( mExportSummary->Overwrite() )
         {
            String	Warning( "Confirm overwriting of file " );
            Warning = Warning + (const char*)Prior_Export_File_Name + ". All data in the file will be lost.";
            if( AfxMessageBox( (const char*)Warning, MB_OKCANCEL ) == IDOK )
               Open_Options = ios::in | ios::out | ios::trunc; 
            else
            {
               AfxMessageBox( "Canceling file overwrite at users request." );
               return; 
            }   
         }
         else
         {			
            Open_Options = ios::in | ios::out | ios::app;
         }

         //create a file stream

         fstream	 Output_File( Prior_Export_File_Name, Open_Options);
         if( Output_File.fail() )
         {
            AfxMessageBox( "Problem opening summary export file. Process aborted." );
            return;
         }


		 int framestart, frameend;
		 AnimationBar* AB = ((AnimationBar*)mAnimationWindowData.mFramePtr->GetActiveView());
		 AB->getSelection(framestart, frameend);
		 bool result = true;
		 for (int i = framestart; i <= frameend; i++)
		 {
			 result = result && mExportSummary->Export( Output_File, i);
		 }
         if(	result )
         {  

            AfxMessageBox( "Summary successfully exported." );
         }
         else
         {  
            AfxMessageBox( "Summary export failed." );
         }

         Output_File.close();
      }
      //	 else export options not accepted

   }
   // else file not selected

   return;
}

/*------------------------------------------------------------*/
void C_Hom_Doc::OnTaskinputDescription()
{
	const String MULTIPLE_PLACEHOLDER = "<multiple names>";
    Dg_TaskDescription	Diag;

    Diag.Task_Name = mDescription.Task();
    Diag.Analyst_Name = mDescription.Analyst();
	Diag.Company_Name = mDescription.Company();
    Diag.Task_Comments = mDescription.Comments();

	CString frameName = mDescription.FrameName(LeftSelect());
	for(int i = LeftSelect(); i <= RightSelect(); i++) {
		if(mDescription.FrameName(i) != frameName) {
			frameName = MULTIPLE_PLACEHOLDER;
			break;
		}
	}
	Diag.Frame_Name = frameName;

    // Get out if the user cancels
    if (Diag.DoModal() != IDOK)
        return;

	// no more than 40 char shown in the report
	if (strlen(Diag.Task_Name) <= 40){
		int frmnum = getCurrentFrame();
		char str[80];
		char cfrmnum[39];

		sprintf(cfrmnum, "%d", frmnum);
		strcpy(str, Diag.Task_Name);
		strcat(str, " - Frame ");
		strcat(str, cfrmnum);
		mDescription.TaskComplete( LPCSTR( str ));
		mDescription.Task( LPCSTR( Diag.Task_Name ));
	}
	else
		mDescription.Task( LPCSTR( Diag.Task_Name.Left(40) ) );

	// no more than 40 char shown in the report
	if (strlen(Diag.Analyst_Name) <= 40)
		mDescription.Analyst( LPCSTR( Diag.Analyst_Name ) );
	else
		mDescription.Analyst( LPCSTR( Diag.Analyst_Name.Left(40) ) );

	// no more than 40 char shown in the report
	if (strlen(Diag.Company_Name) <= 40)
		mDescription.Company( LPCSTR( Diag.Company_Name ) );
	else
		mDescription.Company( LPCSTR( Diag.Company_Name.Left(40) ) );

	// no more than 60 char shown in the report
	if (strlen(Diag.Task_Comments) <= 60)
		mDescription.Comments( LPCSTR( Diag.Task_Comments ) );
	else
		mDescription.Comments( LPCSTR( Diag.Task_Comments.Left(60) ) );

	// set frame name of currently selected frames if not empty or placeholder text
	if(Diag.Frame_Name.GetLength() > 0 && Diag.Frame_Name != MULTIPLE_PLACEHOLDER) {
		for(int i = LeftSelect(); i <= RightSelect(); i++) {
			mDescription.FrameName(i, Diag.Frame_Name.GetString());
		}
	}
	
    Get_App_Ptr()->GetMainFramePtr()->Update_All_Title_Bars();
    this->MakeDirtyAndUpdateViews(false); //to refresh any open reports
}

//----------------------------------------------------------------------------
void C_Hom_Doc ::OnTaskinputUnits()
{
   if( this->Is_Metric() ) //then toggle to English
   {
      this->SetUnits( U_ENGLISH );
	  //GLSkeleton::setGridSize(GLSkeleton::getGridSize() /CM_TO_IN);
   }

   else //units must be English; toggle to Metric
   {
      this->SetUnits( U_METRIC );
	 // GLSkeleton::setGridSize(GLSkeleton::getGridSize() *CM_TO_IN);
   }

   ((ReportView*)mStatusResultsWindow.mFramePtr->GetActiveView())->UpdateUnits();
   /*for(std::map<int, WindowData>::iterator it = mReportWDMap.begin();
	   it != mReportWDMap.end();
	   ++it) {
		if(it->second.mIsOpen) {
			((ReportView*)it->second.mFramePtr->GetActiveView())->UpdateUnits();
		}
   }*/
 
   this->MakeDirtyAndUpdateViews(true); //to refresh any open reports
}

void C_Hom_Doc::OnObliqueViewEnvironment()
{
   Dg_Environment Diag(this);
   C_Hom_App& App = *Get_App_Ptr();
   CGLObliqueView *oblique = (CGLObliqueView*)GLViews[VIEW_OBLIQUE];
   Length lLocX(0,1), lLocY(0,1), lLocZ(0,1);
   float Units_Conversion;

   //Set Units Dependent Variables
   CString Units_Label;

   //	if( this->Units == U_ENGLISH ) 
   if( (E_Units)mDescription.Units() == U_ENGLISH ) 
   {
      Units_Label = "in";
      Units_Conversion = U_MM_IN;
      Diag.Min_Handheld_Height = Round_Int( MIN_HHO_DIM * U_MM_IN );
      Diag.Max_Handheld_Height = Round_Int( MAX_HHO_DIM * U_MM_IN );
      Diag.Min_Handheld_Width = Round_Int( MIN_HHO_DIM * U_MM_IN );
      Diag.Max_Handheld_Width = Round_Int( MAX_HHO_DIM * U_MM_IN );
      Diag.Min_Bar_Thickness = Round_Int( MIN_BAR_DIM * U_MM_IN );
      Diag.Max_Bar_Thickness = Round_Int( MAX_BAR_DIM * U_MM_IN );
      Diag.Min_Bar_Distance = Round_Int( MIN_BAR_DISTANCE * U_MM_IN );
      Diag.Max_Bar_Distance = Round_Int( MAX_BAR_DISTANCE * U_MM_IN );
      Diag.Min_Bar_Elevation = Round_Int( MIN_BAR_ELEVATION * U_MM_IN );
      Diag.Max_Bar_Elevation = Round_Int( MAX_BAR_ELEVATION * U_MM_IN );
      Diag.Min_Bar_Length = Round_Int( MIN_BAR_DIM * U_MM_IN );
      Diag.Max_Bar_Length = Round_Int( MAX_BAR_DIM * U_MM_IN );
      Diag.Min_Bar_Width = Round_Int( MIN_BAR_DIM * U_MM_IN );
      Diag.Max_Bar_Width = Round_Int( MAX_BAR_DIM * U_MM_IN );
   }
   else //units are metric
   {
      Units_Label = "cm";
      Units_Conversion = U_MM_CM;
      Diag.Min_Handheld_Height = Round_Int( MIN_HHO_DIM * U_MM_CM );
      Diag.Max_Handheld_Height = Round_Int( MAX_HHO_DIM * U_MM_CM );
      Diag.Min_Handheld_Width = Round_Int( MIN_HHO_DIM * U_MM_CM );
      Diag.Max_Handheld_Width = Round_Int( MAX_HHO_DIM * U_MM_CM ); 
      Diag.Min_Bar_Thickness = Round_Int( MIN_BAR_DIM * U_MM_CM );
      Diag.Max_Bar_Thickness = Round_Int( MAX_BAR_DIM * U_MM_CM );
      Diag.Min_Bar_Distance = Round_Int( MIN_BAR_DISTANCE * U_MM_CM );
      Diag.Max_Bar_Distance = Round_Int( MAX_BAR_DISTANCE * U_MM_CM );
      Diag.Min_Bar_Elevation = Round_Int( MIN_BAR_ELEVATION * U_MM_CM );
      Diag.Max_Bar_Elevation = Round_Int( MAX_BAR_ELEVATION * U_MM_CM );
      Diag.Min_Bar_Length = Round_Int( MIN_BAR_DIM * U_MM_CM );
      Diag.Max_Bar_Length = Round_Int( MAX_BAR_DIM * U_MM_CM );
      Diag.Min_Bar_Width = Round_Int( MIN_BAR_DIM * U_MM_CM );
      Diag.Max_Bar_Width = Round_Int( MAX_BAR_DIM * U_MM_CM );
   }
   //Initialize dialog labels
   Diag.Handheld_Height_Units = Units_Label;
   Diag.Handheld_Width_Units = Units_Label;

   Diag.Bar_Length_Units = Units_Label;
   Diag.Bar_Width_Units = Units_Label;
   Diag.Bar_Thickness_Units = Units_Label;
   Diag.Bar_Distance_Units = Units_Label;
   Diag.Bar_Elevation_Units = Units_Label;

   Diag.Floor_Visible = oblique->getFloorEnabled();
   Diag.Handheld_Type = oblique->getHandHeld().type;
   Diag.Bar_Type = this->Bar_Type;
   Diag.Bar_Azimuth = this->Bar_Azimuth;
   Diag.Bar_Pitch = this->Bar_Pitch;
   //Initialize dialog variables   
   Diag.Handheld_Height = floor((float)oblique->getHandHeld().height * Units_Conversion + 0.5);
   Diag.Handheld_Width = floor((float)oblique->getHandHeld().depth * Units_Conversion + 0.5);
   Diag.Bar_Length = floor((float)this->Bar_Length * Units_Conversion + 0.5);
   Diag.Bar_Width = floor((float)this->Bar_Width * Units_Conversion + 0.5);
   Diag.Bar_Thickness = floor((float)this->Bar_Thickness * Units_Conversion + 0.5);
   Diag.Bar_Distance = floor((float)this->Bar_Distance * Units_Conversion + 0.5);
   Diag.Bar_Elevation = floor((float)this->Bar_Elevation * Units_Conversion + 0.5);

   if(getPosition() == P_Sitting)
   {
	   Vector3 seatRef = GetSkeleton()->getSeatReference() - GetSkeleton()->getJoint(JT_ANKLE_CENTER);
        // Get the lowest ankle
		 double lowestAnkle[3];
        GetSkeleton()->C4EPoint(JT_ANKLE_CENTER, lLocX, lLocY, lLocZ);
        lowestAnkle[1] = lLocY.Value();
        lowestAnkle[2] = lLocZ.Value();

        Diag.Bar_Distance -= seatRef[1] +  lowestAnkle[1];
        Diag.Bar_Elevation -= seatRef[2] + lowestAnkle[2];
   }
   // Get out if the user cancels
   if (Diag.DoModal() != IDOK)
      return;

   this->ObliqueViewEnvironmentUpdate(Diag);
}

void C_Hom_Doc::OnDisplayAutotile() 
//Toggle the AutoTile member
{
   mAutoTile = ( ! mAutoTile );
   if(mAutoTile)
      OnDisplayTilenow(); // tile everything when autotile is clicked
}

// Toggles the forces being displayed
void C_Hom_Doc::OnDisplayForces()
{
	GLSkeleton::setShowForces(!GLSkeleton::getShowForces());
   this->MakeDirtyAndUpdateViews(false);
}

// Check Show Forces if it is on
void C_Hom_Doc::OnUpdateDisplayForces(CCmdUI* pCmdUI)
{
    if(GLSkeleton::getShowForces())
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

// Toggles the support forces being displayed
void C_Hom_Doc::OnDisplaySupportForces()
{
	GLSkeleton::setShowSupportForces(!GLSkeleton::getShowSupportForces());
   this->MakeDirtyAndUpdateViews(false);
}

void C_Hom_Doc::Ondrawframe()
{
	GLOblique::setframenum(!GLOblique::getframenum());
	this->MakeDirtyAndUpdateViews(false);
}

// Check Show Support Forces if it is on
void C_Hom_Doc::OnUpdateDisplaySupportForces(CCmdUI* pCmdUI)
{
    if(GLSkeleton::getShowSupportForces())
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

void C_Hom_Doc::OnUpdatedrawframe(CCmdUI* pCmdUI)
{
    if(GLOblique::getframenum())
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

// Toggle smooth manipulation in 2D stick figure views
void C_Hom_Doc::OnSmoothManip()
{
    useSmoothManipulation = !useSmoothManipulation;
}

// Toggle continuous analyzis of smooth manipulation
void C_Hom_Doc::OnAnalyzeSmoothManip()
{
    analyzeSmoothManipulation = !analyzeSmoothManipulation;
}

// Update checkbox status in main menu
void C_Hom_Doc::OnUpdateSmoothManip(CCmdUI *pCmdUI)
{
    if(useSmoothManipulation)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

// Update checkbox status in main menu
void C_Hom_Doc::OnUpdateAnalyzeSmoothManip(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(useSmoothManipulation);
    if(analyzeSmoothManipulation)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

// Check "Autotile" in the menu if it's on
void C_Hom_Doc::OnUpdateDisplayAutotile(CCmdUI* pCmdUI) 
{
   if( mAutoTile )
      pCmdUI->SetCheck();
   else
      pCmdUI->SetCheck( 0 );
}

//---------------------------------------------------------------------------
void C_Hom_Doc::OnDisplayTilenow() 
{
   MainFramePtr()->TileChildren();
   return; 
}

//---------------------------------------------------------------------------
//NOTE: Toggles the status bar: if on, turns off; if off, turns on. 
void C_Hom_Doc::OnDisplayStatusbar()
{
   mStatusBar = ! mStatusBar;

   MainFramePtr()->SynchronizeStatusBar();

   return;
}

void C_Hom_Doc::OnHandAngles()
{
   Dg_HandAngles Dialog;
   if (Dialog.DoModal() == IDOK) {
      this->MakeDirtyAndUpdateViews(true);
   }
   return;
}

//---------------------------------------------------------------------------
void C_Hom_Doc::OnUpdateDisplayStatusbar(CCmdUI* pCmdUI) 
{
   if( mStatusBar )
      pCmdUI->SetCheck();
   else
      pCmdUI->SetCheck( 0 );
   return;
}

//---------------------------------------------------------------------------
void C_Hom_Doc::OnFactors()
{
   C_Hom_App* App_Ptr = Get_App_Ptr();
   C_Hom_Doc* Doc_Ptr = App_Ptr->Get_Document_Ptr();
   Dg_PopulationEdit lDialog(Doc_Ptr);
   lDialog.SetFemaleFactors(GetFactors(1)); // 1 for female 0 for male
   lDialog.SetMaleFactors(GetFactors(0));
   lDialog.SetGender(this->GenderCode());

   // Get out if the user cancelled
   if (lDialog.DoModal() != IDOK)
      return;

   float *stature, *weight;

   // ################################################################START#
   // ACT - Lets update the antrometric percentiles based on the settings
   // from the dialog
   // JTK - April 11, 2005
   /*if(GenderCode() == 0)
      SetMaleFactors(lDialog.GetFactors());
   else
      SetFemaleFactors(lDialog.GetFactors());*/
   // We're always going to have to update the male and female factors in the
   // case of the a file being opened
   SetMaleFactors(lDialog.GetFactors(0));
   SetFemaleFactors(lDialog.GetFactors(1));

   Anthropometry anthro = Doc_Ptr->getAnthropometryRef();
   stature = anthro.GetHeightPercentiles();
   weight = anthro.GetWeightPercentiles();
   stature[0] = GetFactors().mMH95;
   stature[1] = GetFactors().mMH50;
   stature[2] = GetFactors().mMH5;
   stature[3] = GetFactors().mFH95;
   stature[4] = GetFactors().mFH50;
   stature[5] = GetFactors().mFH5;

   weight[0] = GetFactors().mMW95;
   weight[1] = GetFactors().mMW50;
   weight[2] = GetFactors().mMW5;
   weight[3] = GetFactors().mFW95;
   weight[4] = GetFactors().mFW50;
   weight[5] = GetFactors().mFW5;

   setAnthropometry(anthro);

   this->MakeDirtyAndUpdateViews(true);
}

//---------------------------------------------------------------------------
void C_Hom_Doc::OnReportsPrint() 
{
   static Dg_PrintReports lDiag;

   // The report map must be updated to store which reports are open and which aren't
   GetReportWindowData(ID_REPORTS_ANALYSIS_SUMMARY, mReportWDMap[ID_REPORTS_ANALYSIS_SUMMARY]);
   GetReportWindowData(ID_REPORTS_ANTHROPOMETRY, mReportWDMap[ID_REPORTS_ANTHROPOMETRY]);
   GetReportWindowData(ID_REPORTS_BALANCE, mReportWDMap[ID_REPORTS_BALANCE]);
   //GetReportWindowData(ID_REPORTS_BALANCE_STANDING, mReportWDMap[ID_REPORTS_BALANCE_STANDING]);
   //GetReportWindowData(ID_REPORTS_BALANCE_SEATED, mReportWDMap[ID_REPORTS_BALANCE_SEATED]);
   GetReportWindowData(ID_REPORTS_FATIGUE, mReportWDMap[ID_REPORTS_FATIGUE]);
   GetReportWindowData(ID_REPORTS_POSTURE, mReportWDMap[ID_REPORTS_POSTURE]);
   GetReportWindowData(ID_REPORTS_JOINT_LOCATIONS, mReportWDMap[ID_REPORTS_JOINT_LOCATIONS]);
   GetReportWindowData(ID_REPORTS_CENTEROFMASSLOCATIONS, mReportWDMap[ID_REPORTS_CENTEROFMASSLOCATIONS]);
   GetReportWindowData(ID_REPORTS_JOINT_MOMENTS, mReportWDMap[ID_REPORTS_JOINT_MOMENTS]);
   GetReportWindowData(ID_REPORTS_JOINT_FORCES, mReportWDMap[ID_REPORTS_JOINT_FORCES]);
   GetReportWindowData(ID_REPORTS_3D_LOWBACK, mReportWDMap[ID_REPORTS_3D_LOWBACK]);
   GetReportWindowData(ID_REPORTS_SAGITTAL, mReportWDMap[ID_REPORTS_SAGITTAL]);
   //GetReportWindowData(ID_REPORTS_SPINAL, mReportWDMap[ID_REPORTS_SPINAL]);
   GetReportWindowData(ID_REPORTS_STRENGTH, mReportWDMap[ID_REPORTS_STRENGTH]);
   GetReportWindowData(ID_REPORTS_STRENGTH_DIR_VECTORS, mReportWDMap[ID_REPORTS_STRENGTH_DIR_VECTORS]);
   GetReportWindowData(ID_REPORTS_TASK_SUMMARY, mReportWDMap[ID_REPORTS_TASK_SUMMARY]);
   #if defined(SHOULDERDLL)
   GetReportWindowData(ID_REPORTS_SHOULDERANALYSIS, mReportWDMap[ID_REPORTS_SHOULDERANALYSIS]);
   GetReportWindowData(ID_REPORTS_SHOULDERGEOMETRY, mReportWDMap[ID_REPORTS_SHOULDERGEOMETRY]);
   #endif
   // Display the dialog for input
   if (lDiag.DoModal() == IDOK) {
      // Print the selected reports
      MainFramePtr()->PrintReports(lDiag.mReportAnalysisSummary,
         lDiag.mReportAnthropometry,
		   lDiag.mReportBalance,
         //lDiag.mReportBalanceStanding,
         //lDiag.mReportBalanceSeated,
         lDiag.mReportFatigue,
         lDiag.mReportPosture,
         lDiag.mReportJointLocations,
		 lDiag.mReportCoMLocations,
         lDiag.mReportJointMoments,
         lDiag.mReportJointForces,
         lDiag.mReportLowBack,
         lDiag.mReportShoulderAnalysis,
         lDiag.mReportShoulderGeometry,
         lDiag.mReportSagittal,
         //lDiag.mReportSpinal,
         lDiag.mReportStrength,
         lDiag.mReportStrengthDirVectors,
         lDiag.mReportTaskInputSummary);
   }
}

//---------------------------------------------------------------------------
// Gray out "Undo" if the queue cannot perform the operation
void C_Hom_Doc::OnUpdateUndoDirectManipulation(CCmdUI* pCmdUI) 
{
    if(undoHistory.canUndo())
    {
        string str = "Undo " + undoHistory.nextUndoString() + "\tCtrl+Z";
        pCmdUI->SetText(str.c_str());
    }
    else
        pCmdUI->SetText("Undo\tCtrl+Z");

    pCmdUI->Enable(undoHistory.canUndo());
}

/*------------------------------------------------------------*/
void C_Hom_Doc::OnUndoDirectManipulation()
{
   // Should not be callable when undo is impossible
   assert(this->CanUndo());
   this->UndoJointAngleChange();
}

//---------------------------------------------------------------------------
// Gray out "Redo" if the queue cannot perform the operation
void C_Hom_Doc::OnUpdateRedoDirectManipulation(CCmdUI* pCmdUI) 
{
    if(undoHistory.canRedo())
    {
        string str = "Redo " + undoHistory.nextRedoString() + "\tCtrl+Y";
        pCmdUI->SetText(str.c_str());
    }
    else
        pCmdUI->SetText("Redo\tCtrl+Y");

    pCmdUI->Enable(undoHistory.canRedo());
}

/*------------------------------------------------------------*/
void C_Hom_Doc::OnRedoDirectManipulation()
{
   // Should not be callable when redo is impossible
   assert(this->CanRedo());
   this->RedoJointAngleChange();
}

//---------------------------------------------------------------------------
void C_Hom_Doc::OnUpdateTaskinputUnits(CCmdUI* pCmdUI) 
{
   if (this->Is_Metric())
      // Then set menu pick for English
      pCmdUI->SetText("Set English &Units");
   else
      // Must be English; set menu pick for Metric
      pCmdUI->SetText("Set Metric &Units");
}


//---------------------------------------------------------------------------
void C_Hom_Doc::OnTaskinputBatch() 
{
   CString lFilters = "Text Files (*.txt) | *.txt ||";

   CFileDialog	 lDiag
      ( 
      TRUE, //file open box
      "txt", //default extension
      "*.txt", //default file name
      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR,
      lFilters ,
      NULL  //parent window
      );

   if( lDiag.DoModal() == IDOK )
   {
      mBatchFilePathName = lDiag.GetPathName();
      mBatchFileRootName = lDiag.GetFileTitle();

      //NOTE: Since the file common dialog will not be removed until this
      //message is finished processing, we must invoke actual processing of
      //the batch file by putting a message in the queue. If we attempt
      //batch file processing here, the file common dialog interferes with
      //the batch dialogs. Looks very messy. For convenience, we are letting 
      //the main frame window handle the message.

      CString lMsg("Begin processing batch file '");
      lMsg += mBatchFilePathName;
      lMsg += "'?";

      if(AfxMessageBox(lMsg, MB_OKCANCEL) == IDOK)
      {
         BatchFile   lBatchFile
            (
            LPCTSTR( mBatchFilePathName ) ,
            LPCTSTR( mBatchFileRootName ) ,
            this
            );

         lBatchFile.Process();
      }
      else
      {
         AfxMessageBox( "Canceling batch processing." );
      }
   }
}

void C_Hom_Doc::OnDisplayDialogcolor() 
{
   this->setReportColor();
   return;
}

/* void Dg_ObliqueFigure::OnBnClickedMaleShirtcolor()
{
     C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
     int gender = pDoc->GenderCode();
     COLORREF Ini_Color = RGB(GLOblique::genderedShirtRed[MALE],GLOblique::genderedShirtGreen[MALE],GLOblique::genderedShirtBlue[MALE] );
     CColorDialog Diag(Ini_Color);
     if(Diag.DoModal() == IDOK)
     {
        COLORREF New_Color = Diag.GetColor();
        if(New_Color != Ini_Color)
        {
          GLOblique::genderedShirtRed[MALE] = GetRValue(New_Color);
          GLOblique::genderedShirtGreen[MALE] = GetGValue(New_Color);
          GLOblique::genderedShirtBlue[MALE] = GetBValue(New_Color);
        }
	 }
     Invalidate(FALSE); 
}*/





void C_Hom_Doc::OnHumanFigure()
{
   Dg_OrthoColors diag;
   diag.DoModal();
}

/*------------------------------------------------------------*/

void C_Hom_Doc::OnOblBackground() 
{
   Dg_ObliqueBackground Diag;

   // Get out if the user cancelled
   if (Diag.DoModal() != IDOK)
      return;
}
/*
void C_Hom_Doc::OnSidBackground() 
{
   Dg_ObliqueBackground Diag;

   // Get out if the user cancelled
   if (Diag.DoModal() != IDOK)
      return;
}
*/

void C_Hom_Doc::OnZerog() 
{
   Dg_Gravity GravDlg;

   double Gravity = task.getGravity();

   if (Gravity >= 9.80 && Gravity < 9.82) { GravDlg.m_ComboBox = 0; }
   else if (Gravity >= 3.72 && Gravity <= 3.74) { GravDlg.m_ComboBox = 4; }
   else if (Gravity >= 1.55 && Gravity <= 1.57) { GravDlg.m_ComboBox = 3; }
   else if (Gravity == 0 ) { GravDlg.m_ComboBox = 1; }
   else if (Gravity <= 0.1 ) { GravDlg.m_ComboBox = 2; }
   else { GravDlg.m_ComboBox = -1; }


   // if we're in english
   if( (E_Units)this->mDescription.Units() == U_ENGLISH ) 
   {
      GravDlg.m_accel_units = "ft/s²";
      GravDlg.m_GravValue = Gravity * m_TO_cm * cm_TO_in * in_TO_ft;
   }
   else
   {
      GravDlg.m_accel_units = "m/s²";
      GravDlg.m_GravValue = Gravity;
   }
   double fractional_remainder;
   GravDlg.m_GravValue *= 1000.0;
   fractional_remainder = fmod(GravDlg.m_GravValue,1);
   GravDlg.m_GravValue -= fractional_remainder;
   if (fractional_remainder >= 0.5)
      GravDlg.m_GravValue ++;
   GravDlg.m_GravValue /= 1000.0;

   GravDlg.DoModal();
}

void C_Hom_Doc::OnExtJointForces() 
{
   Dg_JointLoads lDiag( this );

   Force mXComp(0.0F, 1);
   Torque mXMoment(0.0F, 1);

   lDiag.mMomentUnits = CString("") + "(" + mXMoment.UnitsText() + ")";
   lDiag.mForceUnits = CString("") + "(" + mXComp.UnitsText() + ")";

   lDiag.DoModal();

   return;
}

void C_Hom_Doc::OnCloseActiveWindow()
{
   C_Main_Frame* mainFrame = Get_App_Ptr()->GetMainFramePtr();

   // Get the active report
   CFrameWnd* pFrame = mainFrame->GetActiveFrame();
   ASSERT_VALID(pFrame);

   // Close it
   PreCloseFrame(pFrame);
   pFrame->DestroyWindow();
}

void C_Hom_Doc::OnTaskSupportSelection()
{
   Dg_SupportSelection Diag;
   Diag.DoModal();
}
void C_Hom_Doc::OnTaskGridOptions()
{
   Dg_GRID Diag;
   Diag.DoModal();
}

void C_Hom_Doc::OnUpdateReportsBalanceSeated(CCmdUI *pCmdUI)
{
   GetReportWindowData(ID_REPORTS_BALANCE_SEATED, mReportWDMap[ID_REPORTS_BALANCE_SEATED]);
   bool already_open =
      mReportWDMap[ID_REPORTS_BALANCE_SEATED].mIsOpen != 0;
   pCmdUI->Enable((task.getPosition() == P_Sitting) && !already_open);
}

void C_Hom_Doc::OnUpdateReportsBalanceStanding(CCmdUI *pCmdUI)
{
   GetReportWindowData(ID_REPORTS_BALANCE_STANDING, mReportWDMap[ID_REPORTS_BALANCE_STANDING]);
   bool already_open =
      mReportWDMap[ID_REPORTS_BALANCE_STANDING].mIsOpen != 0;
   pCmdUI->Enable((task.getPosition() == P_Standing) && !already_open);
}

//---------------------------------------------------------------------------
void C_Hom_Doc::OnTaskinput3dstatic()
{
	// Change to 3d static mode
	// TODO for each frame.
	// skeleton->Set3DStatic();
	//GetSkeleton()->Set3DStatic();

	if(Task_Mode != TM_ThreeDStatic) {
		// clear undo/redo history only if we actually change modes
        undoHistory.clearHistory();
	}

	Task_Mode = TM_ThreeDStatic;

	// Add joint angle constraints specific to 3d static
	JointAngles::Set3DStaticErrors();

	// If going from 3D to 2D, additional processing required
	// I don't know if this is necessary for switching to 3D,
	//   but it used to happen in both directions, so I'll leave it in for now

	MainFramePtr()->Update_All_Title_Bars();

	// Updates the hominoid figure if necessary, among other things
	MakeDirtyAndUpdateViews(true);
}

//---------------------------------------------------------------------------
void C_Hom_Doc::OnTaskinput3ddynamic()
{
	// Change to 3d dynamic mode
	if(Task_Mode != TM_ThreeDDynamic) {
		// clear undo/redo history only if we actually change
        undoHistory.clearHistory();
	}

	Task_Mode = TM_ThreeDDynamic;

	// remove horizontal +-90 constraints
	JointAngles::Set3DDynamicErrors();
	
	// enable the animation menu
	// TODO this should now always be enabled
	CMenu* menu = MainFramePtr()->GetMenu();
	menu->EnableMenuItem(6, MF_BYPOSITION | MF_ENABLED);
	MainFramePtr()->DrawMenuBar();

	// If going from 3D to 2D, additional processing required
	// I don't know if this is necessary for switching to 3D,
	//   but it used to happen in both directions, so I'll leave it in for now

	MainFramePtr()->Update_All_Title_Bars();

	// Updates the hominoid figure if necessary, among other things
	MakeDirtyAndUpdateViews(true);

	// retile windows to hide animation bar
	// going to dynamic mode, we have to retile, or windows can overlap
	MainFramePtr()->TileChildren();
	this->mAnimationWindowData.mFramePtr->ShowWindow(SW_SHOWNORMAL);
}

void C_Hom_Doc::OnUpdateTaskinput3dstatic(CCmdUI *pCmdUI)
{
	//Commented out to disable selection of different modes for now - cslong
	//pCmdUI->SetRadio(Task_Mode == TM_ThreeDStatic);
	pCmdUI->Enable(FALSE);
}

void C_Hom_Doc::OnUpdateTaskinput3ddynamic(CCmdUI *pCmdUI)
{
	//Commented out to disable selection of different modes for now - cslong
	pCmdUI->Enable(FALSE);
	//pCmdUI->SetRadio(Task_Mode == TM_ThreeDDynamic);
}

void C_Hom_Doc::OnPreStandinglift()
{
	Skeleton* skel = GetSkeleton();
	int frame = this->getCurrentFrame();
	JointAngles oldAngles = skel->GetAngles();
	skel->SetAngles(mStandLift);

    Vector3 oldForces[NUM_JOINTS];
	for(int i = 0; i < NUM_JOINTS; ++i)
    {
        oldForces[i] = skel->getExtForce((JointID)i);
        //skel->setExtForce((JointID)i, Vector3(0,0,0));
    }
    //skel->setExtForce(JT_LHAND, Vector3(0,0,-10 * U_LBF_NEWTONS));
    //skel->setExtForce(JT_RHAND, Vector3(0,0,-10 * U_LBF_NEWTONS));
	Vector3 rightForceVec, leftForceVec;

		double mRightVertical = -90,mRightHorizontal = 90,mLeftVertical = -90,mLeftHorizontal =90;

		rightForceVec[0] = cos(mRightVertical*M_PI/180.0) * cos(mRightHorizontal*M_PI/180.0);
		rightForceVec[1] = cos(mRightVertical*M_PI/180.0) * sin(mRightHorizontal*M_PI/180.0);
		rightForceVec[2] = sin(mRightVertical*M_PI/180.0);

		leftForceVec[0] = cos(mLeftVertical*M_PI/180.0) * cos(mLeftHorizontal*M_PI/180.0);
		leftForceVec[1] = cos(mLeftVertical*M_PI/180.0) * sin(mLeftHorizontal*M_PI/180.0);
		leftForceVec[2] = sin(mLeftVertical*M_PI/180.0);

		rightForceVec *= 10;
	    leftForceVec *= 10;

		skel->setExtForce(JT_LHAND, leftForceVec);
		skel->setExtForce(JT_RHAND, rightForceVec);
	// update undo history
    addUndoEvent(new UndoablePresetPosture(frame,oldAngles,mStandLift,oldForces));

	MakeDirtyAndUpdateViews();
}

void C_Hom_Doc::OnPreStooplift()
{
	unsigned int frame = getCurrentFrame();
	Skeleton* skeleton = GetSkeleton();
	JointAngles oldAngles = skeleton->GetAngles();
    skeleton->SetAngles(mStoopLift);

    Vector3 oldForces[NUM_JOINTS];
	for(int i = 0; i < NUM_JOINTS; ++i)
    {
        oldForces[i] = skeleton->getExtForce((JointID)i);
        //skeleton->setExtForce((JointID)i, Vector3(0,0,0));
    }
    //skeleton->setExtForce(JT_LHAND, Vector3(0,0,-10 * U_LBF_NEWTONS));
    //skeleton->setExtForce(JT_RHAND, Vector3(0,0,-10 * U_LBF_NEWTONS));
	Vector3 rightForceVec, leftForceVec;

		double mRightVertical = -90,mRightHorizontal = 90,mLeftVertical = -90,mLeftHorizontal =90;

		rightForceVec[0] = cos(mRightVertical*M_PI/180.0) * cos(mRightHorizontal*M_PI/180.0);
		rightForceVec[1] = cos(mRightVertical*M_PI/180.0) * sin(mRightHorizontal*M_PI/180.0);
		rightForceVec[2] = sin(mRightVertical*M_PI/180.0);

		leftForceVec[0] = cos(mLeftVertical*M_PI/180.0) * cos(mLeftHorizontal*M_PI/180.0);
		leftForceVec[1] = cos(mLeftVertical*M_PI/180.0) * sin(mLeftHorizontal*M_PI/180.0);
		leftForceVec[2] = sin(mLeftVertical*M_PI/180.0);

		rightForceVec *= 10;
	    leftForceVec *= 10;

		skeleton->setExtForce(JT_LHAND, leftForceVec);
		skeleton->setExtForce(JT_RHAND, rightForceVec);
	// update undo history
    addUndoEvent(new UndoablePresetPosture(frame,oldAngles,mStoopLift,oldForces));

	MakeDirtyAndUpdateViews();
}

void C_Hom_Doc::OnPreSquatlift()
{
	unsigned int frame = getCurrentFrame();
	Skeleton* skeleton = GetSkeleton();
	JointAngles oldAngles = skeleton->GetAngles();
    skeleton->SetAngles(mSquatLift);

    Vector3 oldForces[NUM_JOINTS];
	for(int i = 0; i < NUM_JOINTS; ++i)
    {
        oldForces[i] = skeleton->getExtForce((JointID)i);
        //skeleton->setExtForce((JointID)i, Vector3(0,0,0));
    }
    //skeleton->setExtForce(JT_LHAND, Vector3(0,0,-10 * U_LBF_NEWTONS));
    //skeleton->setExtForce(JT_RHAND, Vector3(0,0,-10 * U_LBF_NEWTONS));
	Vector3 rightForceVec, leftForceVec;

		double mRightVertical = -90,mRightHorizontal = 90,mLeftVertical = -90,mLeftHorizontal =90;

		rightForceVec[0] = cos(mRightVertical*M_PI/180.0) * cos(mRightHorizontal*M_PI/180.0);
		rightForceVec[1] = cos(mRightVertical*M_PI/180.0) * sin(mRightHorizontal*M_PI/180.0);
		rightForceVec[2] = sin(mRightVertical*M_PI/180.0);

		leftForceVec[0] = cos(mLeftVertical*M_PI/180.0) * cos(mLeftHorizontal*M_PI/180.0);
		leftForceVec[1] = cos(mLeftVertical*M_PI/180.0) * sin(mLeftHorizontal*M_PI/180.0);
		leftForceVec[2] = sin(mLeftVertical*M_PI/180.0);

		rightForceVec *= 10;
	    leftForceVec *= 10;

		skeleton->setExtForce(JT_LHAND, leftForceVec);
		skeleton->setExtForce(JT_RHAND, rightForceVec);
	// update undo history
    addUndoEvent(new UndoablePresetPosture(frame,oldAngles,mSquatLift,oldForces));

	MakeDirtyAndUpdateViews();
}


void C_Hom_Doc::OnPreForwardpush()
{
	unsigned int frame = getCurrentFrame();
	Skeleton* skeleton = GetSkeleton();
	JointAngles oldAngles = skeleton->GetAngles();
    skeleton->SetAngles(mForwardPush);

    Vector3 oldForces[NUM_JOINTS];

	for(int i = 0; i < NUM_JOINTS; ++i)
    {
        oldForces[i] = skeleton->getExtForce((JointID)i);
        //skeleton->setExtForce((JointID)i, Vector3(0,0,0));
		Vector3 oldLF = skeleton->getExtForce(JT_LHAND),oldRF = skeleton->getExtForce(JT_RHAND);
		Vector3 oldLT = skeleton->getExtTorque(JT_LHAND),oldRT = skeleton->getExtTorque(JT_RHAND);

    }
    //skeleton->setExtForce(JT_LHAND, Vector3(0,0,-10 * U_LBF_NEWTONS));
    //skeleton->setExtForce(JT_RHAND, Vector3(0,0,-10 * U_LBF_NEWTONS));
		Vector3 rightForceVec, leftForceVec;

		double mRightVertical = 0,mRightHorizontal = -90,mLeftVertical = 0,mLeftHorizontal =-90;

		rightForceVec[0] = cos(mRightVertical*M_PI/180.0) * cos(mRightHorizontal*M_PI/180.0);
		rightForceVec[1] = cos(mRightVertical*M_PI/180.0) * sin(mRightHorizontal*M_PI/180.0);
		rightForceVec[2] = sin(mRightVertical*M_PI/180.0);

		leftForceVec[0] = cos(mLeftVertical*M_PI/180.0) * cos(mLeftHorizontal*M_PI/180.0);
		leftForceVec[1] = cos(mLeftVertical*M_PI/180.0) * sin(mLeftHorizontal*M_PI/180.0);
		leftForceVec[2] = sin(mLeftVertical*M_PI/180.0);

		rightForceVec *= 10;
	    leftForceVec *= 10;

		skeleton->setExtForce(JT_LHAND, leftForceVec);
		skeleton->setExtForce(JT_RHAND, rightForceVec);
	// update undo history
    addUndoEvent(new UndoablePresetPosture(frame,oldAngles,mForwardPush,oldForces));

	MakeDirtyAndUpdateViews();
}

void C_Hom_Doc::OnPreBackwardpull()
{
	unsigned int frame = getCurrentFrame();
	Skeleton* skeleton = GetSkeleton();
	JointAngles oldAngles = skeleton->GetAngles();
    skeleton->SetAngles(mBackwardPull);

    Vector3 oldForces[NUM_JOINTS];
	for(int i = 0; i < NUM_JOINTS; ++i)
    {
        oldForces[i] = skeleton->getExtForce((JointID)i);
        //skeleton->setExtForce((JointID)i, Vector3(0,0,0));
    }
    //skeleton->setExtForce(JT_LHAND, Vector3(0,0,-10 * U_LBF_NEWTONS));
    //skeleton->setExtForce(JT_RHAND, Vector3(0,0,-10 * U_LBF_NEWTONS));

		Vector3 rightForceVec, leftForceVec;

		double mRightVertical = 0,mRightHorizontal = 90,mLeftVertical = 0,mLeftHorizontal =90;

		rightForceVec[0] = cos(mRightVertical*M_PI/180.0) * cos(mRightHorizontal*M_PI/180.0);
		rightForceVec[1] = cos(mRightVertical*M_PI/180.0) * sin(mRightHorizontal*M_PI/180.0);
		rightForceVec[2] = sin(mRightVertical*M_PI/180.0);

		leftForceVec[0] = cos(mLeftVertical*M_PI/180.0) * cos(mLeftHorizontal*M_PI/180.0);
		leftForceVec[1] = cos(mLeftVertical*M_PI/180.0) * sin(mLeftHorizontal*M_PI/180.0);
		leftForceVec[2] = sin(mLeftVertical*M_PI/180.0);

		rightForceVec *= 10;
	    leftForceVec *= 10;

		skeleton->setExtForce(JT_LHAND, leftForceVec);
		skeleton->setExtForce(JT_RHAND, rightForceVec);
	// update undo history
    addUndoEvent(new UndoablePresetPosture(frame,oldAngles,mBackwardPull,oldForces));

	MakeDirtyAndUpdateViews();
}

void C_Hom_Doc::OnPreNeutralstand()

{
	unsigned int frame = getCurrentFrame();
	Skeleton* skeleton = GetSkeleton();
	JointAngles oldAngles = skeleton->GetAngles();
    skeleton->SetAngles(mNeutralJointAnglesStanding);

    Vector3 oldForces[NUM_JOINTS];
	for(int i = 0; i < NUM_JOINTS; ++i)
    {
        oldForces[i] = skeleton->getExtForce((JointID)i);
        //skeleton->setExtForce((JointID)i, Vector3(0,0,0));
    }
    //skeleton->setExtForce(JT_LHAND, Vector3(0,0,-10 * U_LBF_NEWTONS));
    //skeleton->setExtForce(JT_RHAND, Vector3(0,0,-10 * U_LBF_NEWTONS));
	Vector3 rightForceVec, leftForceVec;

		double mRightVertical = -90,mRightHorizontal = 90,mLeftVertical = -90,mLeftHorizontal =90;

		rightForceVec[0] = cos(mRightVertical*M_PI/180.0) * cos(mRightHorizontal*M_PI/180.0);
		rightForceVec[1] = cos(mRightVertical*M_PI/180.0) * sin(mRightHorizontal*M_PI/180.0);
		rightForceVec[2] = sin(mRightVertical*M_PI/180.0);

		leftForceVec[0] = cos(mLeftVertical*M_PI/180.0) * cos(mLeftHorizontal*M_PI/180.0);
		leftForceVec[1] = cos(mLeftVertical*M_PI/180.0) * sin(mLeftHorizontal*M_PI/180.0);
		leftForceVec[2] = sin(mLeftVertical*M_PI/180.0);

		rightForceVec *= 10;
	    leftForceVec *= 10;

		skeleton->setExtForce(JT_LHAND, leftForceVec);
		skeleton->setExtForce(JT_RHAND, rightForceVec);
	// update undo history
    addUndoEvent(new UndoablePresetPosture(frame,oldAngles,mNeutralJointAnglesStanding,oldForces));

	MakeDirtyAndUpdateViews();
}

void C_Hom_Doc::OnPreManipulationStart()

{
	unsigned int frame = getCurrentFrame();
	Skeleton* skeleton = GetSkeleton();
	JointAngles oldAngles = skeleton->GetAngles();
    skeleton->SetAngles(mManipulationJointAnglesStart);

    Vector3 oldForces[NUM_JOINTS];
	for(int i = 0; i < NUM_JOINTS; ++i)
    {
        oldForces[i] = skeleton->getExtForce((JointID)i);
        //skeleton->setExtForce((JointID)i, Vector3(0,0,0));
    }
    //skeleton->setExtForce(JT_LHAND, Vector3(0,0,-10 * U_LBF_NEWTONS));
    //skeleton->setExtForce(JT_RHAND, Vector3(0,0,-10 * U_LBF_NEWTONS));

	// update undo history
    addUndoEvent(new UndoablePresetPosture(frame,oldAngles,mManipulationJointAnglesStart,oldForces));

	MakeDirtyAndUpdateViews();
}

void C_Hom_Doc::OnPreNeutralTstand()
{
	unsigned int frame = getCurrentFrame();
	Skeleton* skeleton = GetSkeleton();
	JointAngles oldAngles = skeleton->GetAngles();
    skeleton->SetAngles(mNeutralJointAnglesTStanding);

    Vector3 oldForces[NUM_JOINTS];
	for(int i = 0; i < NUM_JOINTS; ++i)
    {
        oldForces[i] = skeleton->getExtForce((JointID)i);
        //skeleton->setExtForce((JointID)i, Vector3(0,0,0));
    }
    //skeleton->setExtForce(JT_LHAND, Vector3(0,0,-10 * U_LBF_NEWTONS));
    //skeleton->setExtForce(JT_RHAND, Vector3(0,0,-10 * U_LBF_NEWTONS));

	// update undo history
    addUndoEvent(new UndoablePresetPosture(frame,oldAngles,mNeutralJointAnglesTStanding,oldForces));

	MakeDirtyAndUpdateViews();
}

void C_Hom_Doc::OnPreNeutralsit()
{
	unsigned int frame = getCurrentFrame();
	Skeleton* skeleton = GetSkeleton();
	JointAngles oldAngles = skeleton->GetAngles();
    skeleton->SetAngles(mNeutralJointAnglesSeated);

    Vector3 oldForces[NUM_JOINTS];
	for(int i = 0; i < NUM_JOINTS; ++i)
    {
        oldForces[i] = skeleton->getExtForce((JointID)i);
        //skeleton->setExtForce((JointID)i, Vector3(0,0,0));
    }
    //skeleton->setExtForce(JT_LHAND, Vector3(0,0,-10 * U_LBF_NEWTONS));
    //skeleton->setExtForce(JT_RHAND, Vector3(0,0,-10 * U_LBF_NEWTONS));

    // update undo history
    addUndoEvent(new UndoablePresetPosture(frame,oldAngles,mNeutralJointAnglesSeated,oldForces));

	MakeDirtyAndUpdateViews();
}

void C_Hom_Doc::OnHandposture()
{
	Dg_HandPosture dialog;
	dialog.DoModal();
}

void C_Hom_Doc::OnDisplayStrengthlimits()
{
	Dg_PercentileLimits dialog;
	dialog.DoModal();

	// we call this in set upper limit and set lower limit now
	//((StatusBox*)mStatusResultsWindow.mFramePtr->GetActiveView())->UpdatePercentileLimits();
}

void C_Hom_Doc::OnDisplayOffsetcoordinatereference()
{
	Dg_OffsetCoordinates dialog;
	dialog.DoModal();
}

void C_Hom_Doc::OnEditCut()
{
	if(Cut()) {
		this->MakeDirtyAndUpdateViews();
	}
}

void C_Hom_Doc::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	int left, right;
	AnimationBar* AB = ((AnimationBar*)mAnimationWindowData.mFramePtr->GetActiveView());
	AB->getSelection(left, right);
	pCmdUI->Enable(left != 0);
}

void C_Hom_Doc::OnEditCopy()
{
	Copy();
	UpdateViews();
}

void C_Hom_Doc::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	// TODO is this ever disabled?
}

void C_Hom_Doc::OnEditPaste()
{
	if(Paste()) {
		this->MakeDirtyAndUpdateViews();
	}
}

void C_Hom_Doc::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// get selection
	int left, right;
	AnimationBar* AB = ((AnimationBar*)mAnimationWindowData.mFramePtr->GetActiveView());
	AB->getSelection(left, right);
	pCmdUI->Enable(!clipboard_contents.empty() && left != 0);
}

LightChoice C_Hom_Doc::GetLightChoice() {
	return lightChoice;
}

void C_Hom_Doc::setLightChoice(LightChoice lc) {
	lightChoice = lc;
}



void C_Hom_Doc::OnAnimationExportavifile()
{
	Dg_MOVIE_EXPORT Dgl;
	Dgl.DoModal();
	if(Dgl.flag == 0)	
		return;
	// The pipe evidently delimits the filter string...
	CFileDialog fileDlg(false, ".avi", "Output.avi", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
		"AVI Files (*.avi)|*.avi||");
	 fileDlg.m_ofn.lpstrTitle ="Export to ...";
	// Pop up the box
	if(fileDlg.DoModal() != IDOK)
		return;

	BeginWaitCursor();

	CGLObliqueView* ov = (CGLObliqueView*)GLViews[Dgl.m_option_group];
	GLOblique* ob = (GLOblique*)ov->getGLInteractive();
	//((CGLObliqueView*)GLViews[VIEW_OBLIQUE])->updateMDP();
	int width_saved = ob->getViewingRect().Width();
	int height_saved = ob->getViewingRect().Height();
	// round to multiple of 8 because i think it's more compatible
	int width = 8*(width_saved/8);
	int height = 8*(height_saved/8);

	ob->setViewport(CRect(0,0,width,height));
	AVIExporter aviExporter(string(fileDlg.GetPathName()), width, height, 32);
	int lastframe = task.getLastFrame();

	if(!aviExporter.Initialize()) {
		AfxMessageBox(aviExporter.getError().c_str());
		EndWaitCursor();
		return;
	}

	// save oblique view state so we can restore it after saving avi
	DrawType_e savedDrawType = ob->getDrawType();
	int savedFrame = ob->getFrame();
	Skeleton* savedSkeleton = ob->getSkeleton();


	LPVOID buffer;
	buffer = malloc(width * height * 32 / 8);
	ob->setDrawType(DT_Frame);
	for(int i = 0; i <= lastframe; i++) {
		ob->setFrame(i);
		ob->updateMDP();
		ob->drawToMemory(buffer);
		if(!aviExporter.WriteFrame(buffer)) {
			AfxMessageBox(aviExporter.getError().c_str());
			free(buffer);
			EndWaitCursor();
			return;
		}
	}
	free(buffer);

	ob->setDrawType(savedDrawType);
	ob->setFrame(savedFrame);
	ob->setSkeleton(savedSkeleton);
	ob->setViewport(CRect(0,0,width_saved, height_saved));
	ob->updateMDP();
	ob->onDraw();

	EndWaitCursor();

	return;
}
void C_Hom_Doc::OnDg_Grid()
{
	Dg_GRID Dgl;
	Dgl.DoModal();
	this->MakeDirtyAndUpdateViews(false);
}
void C_Hom_Doc::OnExportavi()
{
	OnAnimationExportavifile();
}

void C_Hom_Doc::OnWindowsTopview()
{
	// toggle between restored and minimized
	this->GLViews[VIEW_TOP]->GetParentFrame()->ShowWindow(
		this->GLViews[VIEW_TOP]->GetParentFrame()->IsIconic() ? SW_RESTORE : SW_MINIMIZE
	);
}

void C_Hom_Doc::OnUpdateWindowsTopview(CCmdUI *pCmdUI)
{
	// checked if window is visible
	pCmdUI->SetCheck(!this->GLViews[VIEW_TOP]->GetParentFrame()->IsIconic());
}

void C_Hom_Doc::OnWindowsFrontview()
{
	// toggle between restored and minimized
	this->GLViews[VIEW_FRONT]->GetParentFrame()->ShowWindow(
		this->GLViews[VIEW_FRONT]->GetParentFrame()->IsIconic() ? SW_RESTORE : SW_MINIMIZE
	);
}

void C_Hom_Doc::OnUpdateWindowsFrontview(CCmdUI *pCmdUI)
{
	// checked if window is visible
	pCmdUI->SetCheck(!this->GLViews[VIEW_FRONT]->GetParentFrame()->IsIconic());
}

void C_Hom_Doc::OnWindowsSideview()
{
	// toggle between restored and minimized
	this->GLViews[VIEW_SIDE]->GetParentFrame()->ShowWindow(
		this->GLViews[VIEW_SIDE]->GetParentFrame()->IsIconic() ? SW_RESTORE : SW_MINIMIZE
	);
}

void C_Hom_Doc::OnUpdateWindowsSideview(CCmdUI *pCmdUI)
{
	// checked if window is visible
	pCmdUI->SetCheck(!this->GLViews[VIEW_SIDE]->GetParentFrame()->IsIconic());
}

void C_Hom_Doc::OnHandSegments()
{
    Dg_HandSegments Diag(this);
    Diag.DoModal();
}
void C_Hom_Doc::OnWindowsObliqueview()
{
	// toggle between restored and minimized
	this->GLViews[VIEW_OBLIQUE]->GetParentFrame()->ShowWindow(
		this->GLViews[VIEW_OBLIQUE]->GetParentFrame()->IsIconic() ? SW_RESTORE : SW_MINIMIZE
	);
}

void C_Hom_Doc::OnUpdateWindowsObliqueview(CCmdUI *pCmdUI)
{
	// checked if window is visible
	pCmdUI->SetCheck(!this->GLViews[VIEW_OBLIQUE]->GetParentFrame()->IsIconic());
}

void C_Hom_Doc::OnWindowsStatusbox()
{
	// toggle between restored and minimized
	this->getStatusBox()->GetParentFrame()->ShowWindow(
		this->getStatusBox()->GetParentFrame()->IsIconic() ? SW_RESTORE : SW_MINIMIZE
	);
}

void C_Hom_Doc::OnUpdateWindowsStatusbox(CCmdUI *pCmdUI)
{
	// checked if window is visible
	pCmdUI->SetCheck(!this->getStatusBox()->GetParentFrame()->IsIconic());
}

void C_Hom_Doc::On3DefaultView()
{
	//Switch to default offset and zoom for the orthogonal views
	GLSkeletonTop* TopView = (GLSkeletonTop*)this->GLViews[VIEW_TOP]->getGLInteractive();
	TopView->cameraReset();

	GLSkeletonFront* FrontView = (GLSkeletonFront*)this->GLViews[VIEW_FRONT]->getGLInteractive();
	FrontView->cameraReset();

	GLSkeletonSide* SideView = (GLSkeletonSide*)this->GLViews[VIEW_SIDE]->getGLInteractive();
	SideView->cameraReset();

	MakeDirtyAndUpdateViews(false);
}

void C_Hom_Doc::OnWindowsAnimationbar()
{
	// toggle animation bar visibility
	Quadrant_Frame* animationFrame = mAnimationWindowData.mFramePtr;

	if(animationFrame->IsWindowVisible()) {
		animationFrame->ShowWindow(SW_HIDE);
	} else {
		animationFrame->ShowWindow(SW_NORMAL);
	}

	// check auto tile
	C_Hom_Doc* Doc_Ptr = Get_App_Ptr()->Get_Document_Ptr();

	if(Doc_Ptr->AutoTile())
		MainFramePtr()->TileChildren();
	else
		MainFramePtr()->UpdateMainTitleBar();
}


void C_Hom_Doc::OnUpdateWindowsAnimationbar(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(mAnimationWindowData.mFramePtr->IsWindowVisible());
}
