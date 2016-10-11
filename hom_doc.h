#pragma once

// ACT::PRAGMA [05/07/01] - Make conversion errors Warning Level 4
//    [lint level warning]
#ifndef SHOW_WARNINGS
#pragma warning( 4 : 4244 )
#pragma warning(disable : 4786)
#endif

#include "stdafx.h"

#include "Task.h"
#include "Types.h"
#include "TaskDescription.hpp"
#include "Anthropometry.hpp"
#include "HandLoads.hpp"
#include "ExportSummary.hpp"
#include <atlimage.h>
#include "Length.hpp"
#include "GLOblique.h"
#include "WindowData.hpp"
#include "enums.h"
#include "UndoRedo.h"
#include "Factors.hpp"
#include "AngleCue.h"
#include "Vector3.h"
//#include "JointAngles.hpp"

class JointAngles;
class Skeleton;
class CGLView;
enum JointID;

#include <map>
#include <list>
#include <string>

// Declared without including so we don't have to recompile
//   EVERYTHING once we change even one of these itty bitty headers
class BatchFile;
class Dg_Light;
class Dg_PosturePrediction;
class Dg_HandLoads;
class Dg_Anthropometry;
class Dg_BodyAngles;
class Dg_ObliqueBackground;
class Dg_Environment;

#define mm_TO_cm (0.1)
#define cm_TO_mm (10)
#define m_TO_cm (100.0)
#define cm_TO_m (0.01)
#define in_TO_cm (2.54)
#define cm_TO_in (1.0/in_TO_cm)
#define ft_TO_in (12.0)
#define in_TO_ft (1/ft_TO_in)
#define mm_TO_in (mm_TO_cm * cm_TO_in)

// Used in the chair graphic and analysis calculations
#define CHAIR_SEAT_DEPTH	17.6f
#define FRONT_SUPPORT_YOFFSET 8.8f

#define JOINT_FORCES 2

// The workspace file is located in the Hidden windows application data folder
#define DEFAULT_WORKSPACE "\\3DSSPP " szVERSION " Default Workspace.tsk"

#define HOM_DOC_STDAPP
extern int OpenType;  // File version number identifier for serialization

//--- Enumerations and Constants --------------------------------------------

enum LimitChoice {LC_NIOSH, LC_CUSTOM};
enum LightChoice {SINGLE,MULTIPLE};
enum OffsetChoice {NO_OFFSET, COORD_OFFSET_BODY_PART};

enum E_Task_Mode
{
	//TM_TwoDStatic,
	TM_ThreeDStatic,
	TM_ThreeDDynamic
	// TODO when dynamic mode is reenabled in the future, create a new E_Task_Mode value instead of using
	// TM_ThreeDDynamic, so the new mode isn't confused the old mode in saved files and elsewhere
};

// right now this is not used for anything
enum E_PresetPosture
{
	PP_StandLift,
	PP_StoopLift,
	PP_SquatLift,
	PP_ForwardPush,
	PP_BackwardPull,
	PP_StandNeutral,
	PP_SeatNeutral
};

struct intAngles
{
   int horz;
   int vert;
   intAngles::intAngles() : horz(0), vert(0){}
};

struct FingerAngles
{
   intAngles low;		// lower finger bone
   intAngles mid;		// middle bone
   intAngles up;		// Upper Bone
};

struct HandAngles
{
   // Thumb Hand Angles
   FingerAngles thumb;
   // Special joint for the thumb
   intAngles thumbHand;
   // Index Finger Hand Angles
   FingerAngles index;
   // Middle Finger Hand Angles
   FingerAngles middle;
   // Ring Finger Hand Angles
   FingerAngles ring;
   // Pinkey
   FingerAngles pinky;
   HandAngles::HandAngles()
   {	
	    // The default hand angles
	    // Everything starts out initialized to zero
   /*	thumb.low.x = 45;
	    thumb.up.x = 45;
	    thumb.up.x = 45;
	    thumbHand.x = 45;
	    index.up.x = 45;
	    middle.up.x = 45;
	    ring.up.x = 45;
	    pinky.up.x = 45;*/
   }
};

class StatusBox;



class C_Hom_Doc : public CDocument
{
    protected:
        DECLARE_SERIAL(C_Hom_Doc)
        C_Hom_Doc();
        virtual ~C_Hom_Doc();

        // ################# SERIALIZATION #################
        virtual void Serialize(CArchive& ar);
        void SaveQuadFrameWindowData(); // Only called by retrieving functions
        void SaveReportWindowData();	// Only called by retrieving functions
		void LoadQuadFrameWindowData();

        void Serial_Store_v440(CArchive& ArchiveRef);
        void Serial_Retrieve_v440(CArchive& ArchiveRef);
        void ProcessArchive_440 (CArchive& Arch);

		void Serial_Store_v600(CArchive& ArchiveRef);
		void Serial_Retrieve_v600(CArchive& ArchiveRef);
		void ProcessArchive_600(CArchive& Arch);

        void Initialize_To_Default_Data();   // Initialize the doc

        virtual BOOL OnNewDocument();
        virtual BOOL OnOpenDocument(const char* FileNamePtr);
		virtual CFile* GetFile(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException *pError);
        virtual void OnFileSaveAs();
        virtual void OnFileSave();
        void OnDefaultWorkspace();
        void SetDefaultWorkspace();
        void GenericColorDialog(BYTE red, BYTE green, BYTE blue);
 

    public:
        // window creation-related functions
        void CreateViews();
        void InitialOpenReports();
        WindowData* getObliqueWindowDataPtr();
        bool ViewsExist();
        BOOL StatusBar();
		StatusBox* getStatusBox();
		bool IsAnimationWindowOpen();

		

        // *** temporary *** view-related variables
        CGLView *GLViews[4];
		// TODO printing
		CGLView *GLPrintViews[4];
        bool useSmoothManipulation; // move away from public scope later (decide on interface first...)
        bool analyzeSmoothManipulation; // ditto

		std::vector<bool> isredl;
		std::vector<bool> isredr;

        void MakeDirty();
		void UpdateViews();
        void MakeDirtyAndUpdateViews(bool analyze = true, bool changeDiaphram = true);
        void Analyze(bool changeDiaphram = true);

        HandAngles rightHand; // obsolete, but remove?

        void DoExportSummary(std::fstream& aOutputFile,E_Delimiter_Mode aDelimiterMode,E_File_Mode aFileMode);

        ////////////////////////// Boeing request //////////////////////////
        void RestoreDefaultFactors(int gender = -1); // Default to "both"
        ////////////////////////////////////////////////////////////////////
        	
        void GetReportWindowData(int aReportNum, WindowData& aWD);

        // locking modes
        //void CheckDemoMode();
        LockingMode_e GetLockingMode() const;
		E_Task_Mode GetTaskMode() const;

        void setReportColor();
        COLORREF getBalColor() const;

        void OnDg_Population(int genderValue);
        void OnDg_FactorsPassword();

        BOOL AutoTile() const;
        void setAutoTile(BOOL tile);

        BOOL Is_Metric();
        BOOL Is_English();
        CString getLocationUnits() const;
        void UpdateConversions();        
        void SetUnits(E_Units aUnits);
        CString UnitsText();
        CString ForceUnits();
        CString WeightUnits();
        CString LengthUnits();
		CString TorqueUnits();
        CString PercentileText() const;
        CString ReportHeader();
        CString ReportFooter();

        String Get_Task_Name();
        TaskDescription &getDescriptionRef();
        String TaskStatusText();
        void SetDescription(const TaskDescription& aTaskDescription);

        #ifdef _DEBUG
            virtual void AssertValid() const;
            virtual void Dump(CDumpContext& dc) const;
        #endif
      
        // Added so that "new" command works properly
        void DoSave();
        void DoClose();

		// clipboard functions
		// all noops if in static mode
		// there should be no other copies of the Skeleton pointers
		// stored in clipboard_contents anywhere else

		// makes a new copy of all selected frames and stores them in
		// clipboard_contents
		bool Copy();
		// takes all the selected frames and puts the skeletons in
		// clipboard_contents, then removes the frames
		bool Cut();
		// if clipboard_contents.empty, noop
		// otherwise, prompts the user to replace any selected frames,
		// or insert before the first selected frame, then replaces
		// or inserts the skeletons in clipboard_contents
		bool Paste();
		bool ClipboardEmpty();

		void SetHandType(E_Hand_Type new_Hand);

		LimitChoice getLimitChoice();
		PictureOrColor getPictureOrColorChoice() ;

		LightChoice getLightChoice();
		void setLimitChoice(LimitChoice lc);
		void setLightChoice(LightChoice lc);
		int getLowerPercentileLimit(); 
		int getFemaleLowerPercentileLimit();
		void setLowerPercentileLimit(int limit);
		void setFemaleLowerPercentileLimit(int limit);
		int getUpperPercentileLimit();
		int getFemaleUpperPercentileLimit();
		void setUpperPercentileLimit(int limit);
		void setFemaleUpperPercentileLimit(int limit);

		// get the current offset, whether it be none (which is actually ankle center),
		// a custom offset, or a the value of a joint
		// if skel is non-zero, this uses the skeleton in the current frame
		Vector3 getOffset(Skeleton* skel = 0, bool forceMetric = false);

		OffsetChoice getOffsetChoice();
		void setOffsetChoice(OffsetChoice oc);
		// get the value stored as the custom offset (note: may not be the actual offset
		// being used if the offset choice isn't custom)
		Vector3 getOffsetCoordinate(bool forceMetric = false);
		void setOffsetCoordinate(Vector3 offset, bool forceMetric = false);
		bool getAddOffset();
		void setAddOffset(bool addOffset);
		JointID getOffsetJoint();
		void setOffsetJoint(JointID id);

        // ********** begin removal section **********

        bool UndoJointAngleChange();
        bool RedoJointAngleChange();
        BOOL PredictPosture(const Vector3 &aLeftHandRef,const Vector3 &aRightHandRef);
        JointAngles getNeutralJointAngles() const;
		JointAngles getTNeutralJointAngles() const;

		// get leftmost or rightmost selected frame
		int LeftSelect();
		int RightSelect();
		void setSelection(int left, int right);

		// ******* Task Accessors *******
		Task& getTask();

		const Support_s& getSupport() const;
		void setSupport(const Support_s& Support);
		FeetSupport_e getFeetSupport() const;
		void setFeetSupport(FeetSupport_e FeetSupport);
		Position_e getPosition() const;
		void setPosition(Position_e Position);
		bool hasFrontSeatPanSupport() const;
		void setFrontSeatPanSupport(bool value);
		bool hasSeatBackRest() const;
		void setSetBackRest(bool value);
		Length getBackRestHeight() const;
		void setBackRestHeight(double BackRestHeight);

		double getGravity() const;
        void setGravity(double new_Gravity);

		// anthropometry
        const Anthropometry& getAnthropometryRef();
		void setAnthropometry(const Anthropometry& aAnthropometry);

		String Gender();
		int GenderCode() const;

		const Factors& GetFactors() const;
		const Factors& GetFactors(int genderCode) const;
		void SetMaleFactors(const Factors& newFactors);
		void SetFemaleFactors(const Factors& newFactors);

        void setLinkLengths(LinkLengths & newLengths);
        void setLinkCGs(LinkCGs & newCGs);
        void setLinkWeights(LinkWeights & newWeights);

		const LinkLengths& getLinkLengths() const;
		const LinkWeights& getLinkWeights() const;
		const LinkCGs& getLinkCGs() const;

        Skeleton* GetSkeleton(void);
		Skeleton* getSkeletonAtFrame(int frameNumber);
		void setFrame(int frameNumber, Skeleton* skeleton, FrameType_e FrameType);
		void setFrameType(FrameType_e FrameType, int frameNumber = -1);
		FrameType_e getFrameType() const;
		FrameType_e getFrameType(int frameNumber) const;
		// return true if it sets the current frame to be lastFrame.
		bool nextFrame();
		// returns true if it sets the current frame to be 0, or if it already is 0		
		bool previousFrame();
		// changes which frame is to be manipulated
		void setCurrentFrame(int frameNumber);
		// gets the frame number of the frame being manipulated
		int getCurrentFrame();
		// gets the last frame which is not empty
		int getLastFrame();
		// tells doc to update frame number display
		void changeFrame();

		// ******* End Task Accessors ******
        void addUndoEvent(UndoableEvent *event);
        void clearUndoHistory(void);
        bool CanUndo() const;
        bool CanRedo() const;
         
        // graphics
		void cueAngle(const AngleCueData& ACD);
		void cancelCue();
        int GetOblLightTypeLR();
        int GetOblLightTypeFB();
        int GetOblLightTypeTB();
        void ObliqueViewLightUpdate(Dg_Light& aDialogRef);
        void ObliqueViewEnvironmentUpdate(Dg_Environment& aDialogRef);
        BOOL UsingNewLightScheme();
        int GetLightIntensity();
		LightChoice GetLightChoice();
        E_Barrier_Type getBarType() const;
        int getBarLength() const;
        int getBarWidth() const;
        int getBarThickness() const;
        int getBarDistance() const;
        int getBarAzimuth() const;
		int getBarElevation() const;
		int getBarPitch() const;
		bool getMaleHominoidFilePresent();
		bool getFemaleHominoidFilePresent();
		bool getShoeFilePresent();
            		
        // PictureOrColor getPictureOrColorChoice() ;
		void setPictureOrColorChoice( PictureOrColor pOrC) ;

		
		void setMaleHomoidFilePresent(bool present);
		void setFemaleHominoidFilePresent(bool present);
		void setShoeFilePresent(bool present);

		bool getFixedHandAngles();
		bool getFixedHeadAngles();
		void setFixedHandAngles(bool iFixedHandAngles);
		void setFixedHeadAngles(bool iFixedHandAngles);
		bool getMaintainHandAngles();
		bool getMaintainHeadAngles();
		void setMaintainHandAngles(bool iMaintainHandAngles);
		void setMaintainHeadAngles(bool iMaintainHandAngles);

		//exertion duty cycle entered by user
		double EDC_Value[4];

		afx_msg void OnUndoDirectManipulation();
		afx_msg void OnUpdateUndoDirectManipulation(CCmdUI* pCmdUI);
		afx_msg void OnUpdateRedoDirectManipulation(CCmdUI* pCmdUI);
        afx_msg void OnRedoDirectManipulation();

	private:
		// TODO adding options for locking hand angles to forearm angles,
		// and maintaining hand posture angles when changing forearm angles in BSA dialog
		bool fixedHandAngles;
		bool fixedHeadAngles;
		bool maintainHandAngles;
		bool maintainHeadAngles;
		// Coordinate offset
		OffsetChoice offsetChoice; // no offset, joint
		bool addOffset;
		Vector3 offsetCoordinate; // the value of a custom offset
		JointID jointToBeUsedAsOffset; // the joint which will be the origin

		// strength percentile limits
		LimitChoice limitChoice;
		LightChoice lightChoice;
	
		PictureOrColor pictureOrColorChoice;
		int lowerPercentileLimit;
		int upperPercentileLimit;
		int femaleLowerPercentileLimit;
		int femaleUpperPercentileLimit;
		

    private:
		Task task;

		// clipboard
		std::vector<Skeleton*> clipboard_contents;

        UndoHistory undoHistory;

        CString mLocationUnits; // dumb, remove

		E_Task_Mode  Task_Mode;
        LockingMode_e Locking_Mode;
        
        // OpenGL hominoid status
		bool maleHominoidFilePresent;
		bool femaleHominoidFilePresent;
		bool shoeFilePresent;

        CString DocumentSchemaVersion;
        TaskDescription	mDescription;
        std::string mCOMLastErrorMessage;     

        CString mBatchFilePathName;
        CString mBatchFileRootName;
        std::string mFilePathName;
        CString Prior_Export_File_Name;

        BOOL mAutoTile;
        BOOL mStatusBar;
        void InitReportMap(); // initializes report map

        std::auto_ptr<ExportSummary> mExportSummary;
        COLORREF mBalColor;

        // window layout information
        WindowData mFrontWindowData;
        WindowData mSideWindowData;
        WindowData mTopWindowData;
        WindowData mObliqueWindowData;
        WindowData mStatusResultsWindow;
        WindowData mAnimationWindowData;
        WindowData mMainWindowData;

        // ********** begin removal section **********

        // skeleton, some redundant
        JointAngles mNeutralJointAnglesStanding;
		JointAngles mNeutralJointAnglesTStanding;
		JointAngles mManipulationJointAnglesStart;
        JointAngles mNeutralJointAnglesSeated;
		// new pre-set postures
		JointAngles mStandLift;
		JointAngles mStoopLift;
		JointAngles mSquatLift;
		JointAngles mForwardPush;
		JointAngles mBackwardPull;
		void initializePresetPostures();

        // graphics, some old
        BOOL mUseNewLightScheme;
        int mLightIntensity;
        int Obl_Light_Type_FB;
        int Obl_Light_Type_LR;
        int Obl_Light_Type_TB;
        BYTE Report_BG_Red;
        BYTE Report_BG_Green;
        BYTE Report_BG_Blue;
        E_Barrier_Type Bar_Type;
        int Bar_Width;
        int Bar_Length;
        int Bar_Thickness;
        int Bar_Distance;
        int Bar_Elevation;
        int Bar_Azimuth;
		int Bar_Pitch;
    public:
        // Stores whether each window is open, and its position.
        // Made public for use in printreports
        std::map <int, WindowData> mReportWDMap;	

    protected:
        //{{AFX_MSG(C_Hom_Doc)
        afx_msg void OnImportLocFile();
        afx_msg void OnPredictMotion();
        afx_msg void OnPostureAnthropometry();
        afx_msg void OnPostureJointAngles();
        afx_msg void OnPosturePrediction();
        afx_msg void OnPostureLockingMode();
        afx_msg void OnPostureHandLoads();
        // afx_msg void OnPostureHandLoadsAdv(); //moved to button in hand loads
        afx_msg void OnObliqueViewHuman();
        afx_msg void OnObliqueViewCamera();
        afx_msg void OnObliqueViewLight();
        afx_msg void OnFileClose();
        afx_msg void OnOrthoBackground();
        afx_msg void OnReportsExportsummary();
        afx_msg void OnTaskinputDescription();
        afx_msg void OnTaskinputUnits();
        afx_msg void OnObliqueViewEnvironment();
        afx_msg void OnDisplayAutotile();
        afx_msg void OnDisplayForces();
        afx_msg void OnUpdateDisplayForces(CCmdUI* pCmdUI);
		afx_msg void OnDisplaySupportForces();
		afx_msg void Ondrawframe();
        afx_msg void OnUpdateDisplaySupportForces(CCmdUI* pCmdUI);
        afx_msg void OnUpdatedrawframe(CCmdUI* pCmdUI);
        afx_msg void OnSmoothManip();
        afx_msg void OnAnalyzeSmoothManip();
        afx_msg void OnUpdateSmoothManip(CCmdUI* pCmdUI);
        afx_msg void OnUpdateAnalyzeSmoothManip(CCmdUI* pCmdUI);
        afx_msg void OnUpdateDisplayAutotile(CCmdUI* pCmdUI);
        afx_msg void OnDisplayTilenow();
        afx_msg void OnDisplayStatusbar();
        afx_msg void OnHandAngles();
        afx_msg void OnUpdateDisplayStatusbar(CCmdUI* pCmdUI);
        afx_msg void OnFactors();
        afx_msg void OnReportsPrint();
        //afx_msg void OnUpdateUndoDirectManipulation(CCmdUI* pCmdUI);
        //afx_msg void OnUndoDirectManipulation();
        //afx_msg void OnUpdateRedoDirectManipulation(CCmdUI* pCmdUI);
        //afx_msg void OnRedoDirectManipulation();
        afx_msg void OnUpdateTaskinputUnits(CCmdUI* pCmdUI);
        afx_msg void OnTaskinputBatch();
        afx_msg void OnDisplayDialogcolor();
        afx_msg void OnHumanFigure();
        afx_msg void OnOblBackground();
		///Yijun
		//afx_msg void OnSidBackground();

        afx_msg void OnZerog();
        afx_msg void OnExtJointForces();
        afx_msg void OnCloseActiveWindow();
        afx_msg void OnTaskSupportSelection();
		afx_msg void OnTaskGridOptions();
        afx_msg void OnUpdateReportsBalanceSeated(CCmdUI *pCmdUI);
        afx_msg void OnUpdateReportsBalanceStanding(CCmdUI *pCmdUI);
	    afx_msg void OnTaskinput3dstatic();
	    afx_msg void OnTaskinput3ddynamic();
	    afx_msg void OnUpdateTaskinput3dstatic(CCmdUI *pCmdUI);
	    afx_msg void OnUpdateTaskinput3ddynamic(CCmdUI *pCmdUI);
	    afx_msg void OnPreStandinglift();
	    afx_msg void OnPreStooplift();
	    afx_msg void OnPreSquatlift();
	    afx_msg void OnPreForwardpush();
	    afx_msg void OnPreBackwardpull();
	    afx_msg void OnPreNeutralstand();
		afx_msg void OnPreManipulationStart();
		afx_msg void OnPreNeutralTstand();
	    afx_msg void OnPreNeutralsit();
		afx_msg void OnHandposture();
		afx_msg void OnDisplayStrengthlimits();
		afx_msg void OnDisplayOffsetcoordinatereference();
		afx_msg void On3DefaultView();
        afx_msg void OnHandSegments();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

		DECLARE_OLECREATE(C_Hom_Doc)

        // Generated OLE dispatch map functions
        //{{AFX_DISPATCH(C_Hom_Doc)
		afx_msg BSTR HelloWorld();
		afx_msg BOOL SetTaskDescription(short aUnits, BSTR FAR* aTask, BSTR FAR* aAnalyst, BSTR FAR* aComments);
		afx_msg BOOL SetAnthropometry(short aSex, short aPercentile, float aHeight, float aWeight);
		afx_msg BOOL SetPopulationDefault();
		afx_msg BOOL SetZeroGravity();
		afx_msg BOOL PosturePredict(short handOrientation, float LH, float LV, float LL, float RH, float RV, float RL);
		afx_msg BOOL SetPopulationFromFile(BSTR FAR* FilePathPtr, BSTR FAR* FileNamePtr);
		afx_msg BOOL SetBodySegmentAngles(short aForearmLH, short aForearmLV, short aForearmRH, short aForearmRV, short aUpperarmLH, short aUpperarmLV, short aUpperarmRH, short aUpperarmRV, short aUpperlegLV, short aUpperlegRV, short aLowerlegLV, short aLowerlegRV, short aTrunkFlexion, short aTrunkAxialRotation, short aTrunkLateralBending);
		afx_msg BOOL SetHandLoads(double aLeftMagnitude, double aLeftHorizontalAngle, double aLeftVerticalAngle, double aRightMagnitude, double aRightHorizontalAngle, double aRightVerticalAngle);
		afx_msg BSTR LastErrorMessage();
		afx_msg BOOL GetAnalysisSummaryA(short FAR* aElbowPercentilePtr, BSTR FAR* aElbowLimitPtr, short FAR* aShoulderPercentilePtr, BSTR FAR* aShoulderLimitPtr, short FAR* aTorsoPercentilePtr, BSTR FAR* aTorsoLimitPtr, short FAR* aNeckPercentilePtr, BSTR FAR* aNeckLimitPtr, short FAR* aHipPercentilePtr, BSTR FAR* aHipLimitPtr, short FAR* aKneePercentilePtr, BSTR FAR* aKneeLimitPtr, short FAR* aAnklePercentilePtr, BSTR FAR* aAnkleLimitPtr, short* aWristPercentilePtr, BSTR* aWristLimitPtr);
		afx_msg BOOL GetAnalysisSummaryB(float FAR* aLeftHandForceXPtr, float FAR* aLeftHandForceYPtr, float FAR* aLeftHandForceZPtr, float FAR* aLeftHandForceMagPtr, float FAR* aRightHandForceXPtr, float FAR* aRightHandForceYPtr, float FAR* aRightHandForceZPtr, float FAR* aRightHandForceMagPtr, float FAR* aDiscCompressionValuePtr, float FAR* aDiscCompressionDeviationPtr, short FAR* aLeftPercentLoadPtr, short FAR* aRightPercentLoadPtr, BSTR FAR* aBalancePtr, float FAR* aFrictionPtr);
		afx_msg BOOL GetAnthropometryA(float FAR* aLowerArmLengthPtr, float FAR* aLowerArmCGPtr, float FAR* aLowerArmWeightPtr, float FAR* aUpperArmLengthPtr, float FAR* aUpperArmCGPtr, float FAR* aUpperArmWeightPtr, float FAR* aL5ToShoulderLengthPtr, float FAR* aL5ToShoulderCGPtr, float FAR* aL5ToShoulderWeightPtr);
		afx_msg BOOL GetAnthropometryB(float FAR* aL5AboveWeightPtr, float FAR* aHipToL5LengthPtr, float FAR* aHipToL5CGPtr, float FAR* aHipToL5WeightPtr, float FAR* aHipToShoulderLengthPtr, float FAR* aHipToShoulderCGPtr, float FAR* aHipToShoulderWeightPtr);
		afx_msg BOOL GetAnthropometryC(float FAR* aUpperLegLengthPtr, float FAR* aUpperLegCGPtr, float FAR* aUpperLegWeightPtr, float FAR* aLowerLegLengthPtr, float FAR* aLowerLegCGPtr, float FAR* aLowerLegWeightPtr, float FAR* aFootLengthPtr, float FAR* aFootCGPtr, float FAR* aFootWeightPtr, float FAR* aDiaphragmMomentArmPtr);
		afx_msg BOOL GetJointMomentsA(float FAR* aLeftElbowXPtr, float FAR* aLeftElbowYPtr, float FAR* aLeftElbowZPtr, float FAR* aRightElbowXPtr, float FAR* aRightElbowYPtr, float FAR* aRightElbowZPtr, float FAR* aLeftShoulderXPtr, float FAR* aLeftShoulderYPtr, float FAR* aLeftShoulderZPtr, float FAR* aRightShoulderXPtr, float FAR* aRightShoulderYPtr, float FAR* aRightShoulderZPtr);
		afx_msg BOOL GetJointMomentsB(float FAR* aL5S1XPtr, float FAR* aL5S1YPtr, float FAR* aL5S1ZPtr, float FAR* aLeftHipXPtr, float FAR* aLeftHipYPtr, float FAR* aLeftHipZPtr, float FAR* aRightHipXPtr, float FAR* aRightHipYPtr, float FAR* aRightHipZPtr);
		afx_msg BOOL GetJointMomentsC(float FAR* aLeftKneeXPtr, float FAR* aLeftKneeYPtr, float FAR* aLeftKneeZPtr, float FAR* aRightKneeXPtr, float FAR* aRightKneeYPtr, float FAR* aRightKneeZPtr, float FAR* aLeftAnkleXPtr, float FAR* aLeftAnkleYPtr, float FAR* aLeftAnkleZPtr, float FAR* aRightAnkleXPtr, float FAR* aRightAnkleYPtr, float FAR* aRightAnkleZPtr);
		afx_msg BOOL GetJointsA(float FAR* aLeftHandXPtr, float FAR* aLeftHandYPtr, float FAR* aLeftHandZPtr, float FAR* aRightHandXPtr, float FAR* aRightHandYPtr, float FAR* aRightHandZPtr, float FAR* aLeftElbowXPtr, float FAR* aLeftElbowYPtr, float FAR* aLeftElbowZPtr, float FAR* aRightElbowXPtr, float FAR* aRightElbowYPtr, float FAR* aRightElbowZPtr);
		afx_msg BOOL GetJointsB(float FAR* aLeftShoulderXPtr, float FAR* aLeftShoulderYPtr, float FAR* aLeftShoulderZPtr, float FAR* aRightShoulderXPtr, float FAR* aRightShoulderYPtr, float FAR* aRightShoulderZPtr, float FAR* aL5S1XPtr, float FAR* aL5S1YPtr, float FAR* aL5S1ZPtr, float FAR* aLeftHipXPtr, float FAR* aLeftHipYPtr, float FAR* aLeftHipZPtr, float FAR* aRightHipXPtr, float FAR* aRightHipYPtr, float FAR* aRightHipZPtr);
		afx_msg BOOL GetJointsC(float FAR* aLeftKneeXPtr, float FAR* aLeftKneeYPtr, float FAR* aLeftKneeZPtr, float FAR* aRightKneeXPtr, float FAR* aRightKneeYPtr, float FAR* aRightKneeZPtr, float FAR* aLeftAnkleXPtr, float FAR* aLeftAnkleYPtr, float FAR* aLeftAnkleZPtr, float FAR* aRightAnkleXPtr, float FAR* aRightAnkleYPtr, float FAR* aRightAnkleZPtr);
		afx_msg BOOL GetLowBackA(float FAR* aLeftErSpiFRPtr, float FAR* aLeftErSpiFSPtr, float FAR* aLeftErSpiXPtr, float FAR* aLeftErSpiYPtr, float FAR* aLeftErSpiZPtr, float FAR* aLeftErSpiMXPtr, float FAR* aLeftErSpiMYPtr, float FAR* aRightErSpiFRPtr, float FAR* aRightErSpiFSPtr, float FAR* aRightErSpiXPtr, float FAR* aRightErSpiYPtr, float FAR* aRightErSpiZPtr, float FAR* aRightErSpiMXPtr, float FAR* aRightErSpiMYPtr);
		afx_msg BOOL GetLowBackB(float FAR* aLeftRecAbFRPtr, float FAR* aLeftRecAbFSPtr, float FAR* aLeftRecAbXPtr, float FAR* aLeftRecAbYPtr, float FAR* aLeftRecAbZPtr, float FAR* aLeftRecAbMXPtr, float FAR* aLeftRecAbMYPtr, float FAR* aRightRecAbFRPtr, float FAR* aRightRecAbFSPtr, float FAR* aRightRecAbXPtr, float FAR* aRightRecAbYPtr, float FAR* aRightRecAbZPtr, float FAR* aRightRecAbMXPtr, float FAR* aRightRecAbMYPtr);
		afx_msg BOOL GetLowBackC(float FAR* aLeftIntObFRPtr, float FAR* aLeftIntObFSPtr, float FAR* aLeftIntObXPtr, float FAR* aLeftIntObYPtr, float FAR* aLeftIntObZPtr, float FAR* aLeftIntObMXPtr, float FAR* aLeftIntObMYPtr, float FAR* aRightIntObFRPtr, float FAR* aRightIntObFSPtr, float FAR* aRightIntObXPtr, float FAR* aRightIntObYPtr, float FAR* aRightIntObZPtr, float FAR* aRightIntObMXPtr, float FAR* aRightIntObMYPtr);
		afx_msg BOOL GetLowBackD(float FAR* aLeftExtObFRPtr, float FAR* aLeftExtObFSPtr, float FAR* aLeftExtObXPtr, float FAR* aLeftExtObYPtr, float FAR* aLeftExtObZPtr, float FAR* aLeftExtObMXPtr, float FAR* aLeftExtObMYPtr, float FAR* aRightExtObFRPtr, float FAR* aRightExtObFSPtr, float FAR* aRightExtObXPtr, float FAR* aRightExtObYPtr, float FAR* aRightExtObZPtr, float FAR* aRightExtObMXPtr, float FAR* aRightExtObMYPtr);
		afx_msg BOOL GetLowBackE(float FAR* aLeftLatDorFRPtr, float FAR* aLeftLatDorFSPtr, float FAR* aLeftLatDorXPtr, float FAR* aLeftLatDorYPtr, float FAR* aLeftLatDorZPtr, float FAR* aLeftLatDorMXPtr, float FAR* aLeftLatDorMYPtr, float FAR* aRightLatDorFRPtr, float FAR* aRightLatDorFSPtr, float FAR* aRightLatDorXPtr, float FAR* aRightLatDorYPtr, float FAR* aRightLatDorZPtr, float FAR* aRightLatDorMXPtr, float FAR* aRightLatDorMYPtr);
		afx_msg BOOL GetLowBackF(float FAR* aCompressionTotalPtr, float FAR* aShearTotalPtr, float FAR* aShearAntPostPtr, float FAR* aShearLateralPtr);
		afx_msg BOOL GetPostureA(short FAR* aLeftElbowIncludedPtr, short FAR* aRightElbowIncludedPtr, short FAR* aLeftShoulderVerticalPtr, short FAR* aRightShoulderVerticalPtr, short FAR* aLeftShoulderHorizontalPtr, short FAR* aRightShoulderHorizontalPtr, short FAR* aLeftHumeralRotationPtr, short FAR* aRightHumeralRotationPtr, short FAR* aLeftHipIncludedPtr, short FAR* aRightHipIncludedPtr, short FAR* aLeftKneeIncludedPtr, short FAR* aRightKneeIncludedPtr, short FAR* aLeftAnkleIncludedPtr, short FAR* aRightAnkleIncludedPtr);
		afx_msg BOOL GetPostureB(short FAR* aAdjustedFlexionPtr, short FAR* aAdjustedRotationPtr, short FAR* aAdjustedLateralBendingPtr, short FAR* aPelvicRotationPtr);
		afx_msg BOOL GetSagittal(float FAR* aTotalPtr, float FAR* aTotalStdDevPtr, float FAR* aErectorSpinaePtr, float FAR* aErectorSpinaeStdDevPtr, float FAR* aRectusAbdominusPtr, float FAR* aRectusAbdominusStdDevPtr, float FAR* aAbdominalPtr, float FAR* aHandLoadsPtr, float FAR* aUpperBodyWeightPtr, float FAR* aShearTotalPtr, float FAR* aShearSagittalPtr, float FAR* aShearFrontalPtr, short FAR* aEstLigamentStrainPtr);
		afx_msg BOOL GetSpinalA(float FAR* aGloL2L3FXPtr, float FAR* aGloL2L3FYPtr, float FAR* aGloL2L3FZPtr, float FAR* aGloL2L3MXPtr, float FAR* aGloL2L3MYPtr, float FAR* aGloL2L3MZPtr, float FAR* aGloL4L5FXPtr, float FAR* aGloL4L5FYPtr, float FAR* aGloL4L5FZPtr, float FAR* aGloL4L5MXPtr, float FAR* aGloL4L5MYPtr, float FAR* aGloL4L5MZPtr);
		afx_msg BOOL GetSpinalB(float FAR* aGloL5S1MXPtr, float FAR* aGloL5S1MYPtr, float FAR* aGloL5S1MZPtr, float FAR* aRotL2L3FXPtr, float FAR* aRotL2L3FYPtr, float FAR* aRotL2L3FZPtr, float FAR* aRotL2L3MXPtr, float FAR* aRotL2L3MYPtr, float FAR* aRotL2L3MZPtr);
		afx_msg BOOL GetSpinalC(float FAR* aRotL3L4FXPtr, float FAR* aRotL3L4FYPtr, float FAR* aRotL3L4FZPtr, float FAR* aRotL3L4MXPtr, float FAR* aRotL3L4MYPtr, float FAR* aRotL3L4MZPtr, float FAR* aRotL4L5FXPtr, float FAR* aRotL4L5FYPtr, float FAR* aRotL4L5FZPtr, float FAR* aRotL4L5MXPtr, float FAR* aRotL4L5MYPtr, float FAR* aRotL4L5MZPtr);
		afx_msg BOOL GetStrengthA(float FAR* aLeftElbFlxExtMMPtr, BSTR FAR* aLeftElbFlxExtMEPtr, float FAR* aLeftElbFlxExtMNPtr, float FAR* aLeftElbFlxExtSDPtr, short FAR* aLeftElbFlxExtCPPtr, float FAR* aRightElbFlxExtMMPtr, BSTR FAR* aRightElbFlxExtMEPtr, float FAR* aRightElbFlxExtMNPtr, float FAR* aRightElbFlxExtSDPtr, short FAR* aRightElbFlxExtCPPtr);
		afx_msg BOOL GetStrengthB(float FAR* aLeftShoHumRotMMPtr, BSTR FAR* aLeftShoHumRotMEPtr, float FAR* aLeftShoHumRotMNPtr, float FAR* aLeftShoHumRotSDPtr, short FAR* aLeftShoHumRotCPPtr, float FAR* aRightShoHumRotMMPtr, BSTR FAR* aRightShoHumRotMEPtr, float FAR* aRightShoHumRotMNPtr, float FAR* aRightShoHumRotSDPtr, short FAR* aRightShoHumRotCPPtr);
		afx_msg BOOL GetStrengthC(float FAR* aLeftShoBkFdMMPtr, BSTR FAR* aLeftShoBkFdMEPtr, float FAR* aLeftShoBkFdMNPtr, float FAR* aLeftShoBkFdSDPtr, short FAR* aLeftShoBkFdCPPtr, float FAR* aRightShoBkFdMMPtr, BSTR FAR* aRightShoBkFdMEPtr, float FAR* aRightShoBkFdMNPtr, float FAR* aRightShoBkFdSDPtr, short FAR* aRightShoBkFdCPPtr);
		afx_msg BOOL GetStrengthD(float FAR* aLeftShoAbAdMMPtr, BSTR FAR* aLeftShoAbAdMEPtr, float FAR* aLeftShoAbAdMNPtr, float FAR* aLeftShoAbAdSDPtr, short FAR* aLeftShoAbAdCPPtr, float FAR* aRightShoAbAdMMPtr, BSTR FAR* aRightShoAbAdMEPtr, float FAR* aRightShoAbAdMNPtr, float FAR* aRightShoAbAdSDPtr, short FAR* aRightShoAbAdCPPtr);
		afx_msg BOOL GetStrengthE(float FAR* aTruFlxExtMMPtr, BSTR FAR* aTruFlxExtMEPtr, float FAR* aTruFlxExtMNPtr, float FAR* aTruFlxExtSDPtr, short FAR* aTruFlxExtCPPtr, float FAR* aTruLatBndMMPtr, BSTR FAR* aTruLatBndMEPtr, float FAR* aTruLatBndMNPtr, float FAR* aTruLatBndSDPtr, short FAR* aTruLatBndCPPtr, float FAR* aTruRotMMPtr, BSTR FAR* aTruRotMEPtr, float FAR* aTruRotMNPtr, float FAR* aTruRotSDPtr, short FAR* aTruRotCPPtr);
		afx_msg BOOL GetStrengthF(float FAR* aLeftHipFlxExtMMPtr, BSTR FAR* aLeftHipFlxExtMEPtr, float FAR* aLeftHipFlxExtMNPtr, float FAR* aLeftHipFlxExtSDPtr, short FAR* aLeftHipFlxExtCPPtr, float FAR* aRightHipFlxExtMMPtr, BSTR FAR* aRightHipFlxExtMEPtr, float FAR* aRightHipFlxExtMNPtr, float FAR* aRightHipFlxExtSDPtr, short FAR* aRightHipFlxExtCPPtr);
		afx_msg BOOL GetStrengthG(float FAR* aLeftKneeFlxExtMMPtr, BSTR FAR* aLeftKneeFlxExtMEPtr, float FAR* aLeftKneeFlxExtMNPtr, float FAR* aLeftKneeFlxExtSDPtr, short FAR* aLeftKneeFlxExtCPPtr, float FAR* aRightKneeFlxExtMMPtr, BSTR FAR* aRightKneeFlxExtMEPtr, float FAR* aRightKneeFlxExtMNPtr, float FAR* aRightKneeFlxExtSDPtr, short FAR* aRightKneeFlxExtCPPtr);
		afx_msg BOOL GetStrengthH(float FAR* aLeftAnkFlxExtMMPtr, BSTR FAR* aLeftAnkFlxExtMEPtr, float FAR* aLeftAnkFlxExtMNPtr, float FAR* aLeftAnkFlxExtSDPtr, short FAR* aLeftAnkFlxExtCPPtr, float FAR* aRightAnkFlxExtMMPtr, BSTR FAR* aRightAnkFlxExtMEPtr, float FAR* aRightAnkFlxExtMNPtr, float FAR* aRightAnkFlxExtSDPtr, short FAR* aRightAnkFlxExtCPPtr);
		afx_msg BOOL GetStrengthVectorsA(float FAR* aLeftElbFlxExtXPtr, float FAR* aLeftElbFlxExtYPtr, float FAR* aLeftElbFlxExtZPtr, float FAR* aRightElbFlxExtXPtr, float FAR* aRightElbFlxExtYPtr, float FAR* aRightElbFlxExtZPtr);
		afx_msg BOOL GetStrengthVectorsB(float FAR* aLeftShoBkFdXPtr, float FAR* aLeftShoBkFdYPtr, float FAR* aLeftShoBkFdZPtr, float FAR* aRightShoBkFdXPtr, float FAR* aRightShoBkFdYPtr, float FAR* aRightShoBkFdZPtr);
		afx_msg BOOL GetStrengthVectorsC(float FAR* aLeftShoAbAdXPtr, float FAR* aLeftShoAbAdYPtr, float FAR* aLeftShoAbAdZPtr, float FAR* aRightShoAbAdXPtr, float FAR* aRightShoAbAdYPtr, float FAR* aRightShoAbAdZPtr);
		afx_msg BOOL GetStrengthVectorsD(float FAR* aLeftShoHumRotXPtr, float FAR* aLeftShoHumRotYPtr, float FAR* aLeftShoHumRotZPtr, float FAR* aRightShoHumRotXPtr, float FAR* aRightShoHumRotYPtr, float FAR* aRightShoHumRotZPtr);
		afx_msg BOOL GetStrengthVectorsE(float FAR* aL5S1FlxExtXPtr, float FAR* aL5S1FlxExtYPtr, float FAR* aL5S1FlxExtZPtr, float FAR* aL5S1LatBndXPtr, float FAR* aL5S1LatBndYPtr, float FAR* aL5S1LatBndZPtr, float FAR* aL5S1RotXPtr, float FAR* aL5S1RotYPtr, float FAR* aL5S1RotZPtr);
		afx_msg BOOL GetStrengthVectorsF(float FAR* aLeftHipFlxExtXPtr, float FAR* aLeftHipFlxExYPtr, float FAR* aLeftHipFlxExtZPtr, float FAR* aRightHipFlxExtXPtr, float FAR* aRightHipFlxExtYPtr, float FAR* aRightHipFlxExtZPtr);
		afx_msg BOOL GetStrengthVectorsG(float FAR* aLeftKneeFlxExtXPtr, float FAR* aLeftKneeFlxExtYPtr, float FAR* aLeftKneeFlxExtZPtr, float FAR* aRightKneeFlxExtXPtr, float FAR* aRightKneeFlxExtYPtr, float FAR* aRightKneeFlxExtZPtr);
		afx_msg BOOL GetStrengthVectorsH(float FAR* aLeftAnkleFlxExtXPtr, float FAR* aLeftAnkleFlxExtYPtr, float FAR* aLeftAnkleFlxExtZPtr, float FAR* aRightAnkleFlxExtXPtr, float FAR* aRightAnkleFlxExtYPtr, float FAR* aRightAnkleFlxExtZPtr);
		afx_msg BOOL GetTaskInputSummaryA(short FAR* aLeftForeArmHPtr, short FAR* aLeftForeArmVPtr, short FAR* aRightForeArmHPtr, short FAR* aRightForeArmVPtr, short FAR* aLeftUpperArmHPtr, short FAR* aLeftUpperArmVPtr, short FAR* aRightUpperArmHPtr, short FAR* aRightUpperArmVPtr, short FAR* aLeftUpperLegVPtr, short FAR* aRightUpperLegVPtr, short FAR* aLeftLowerLegVPtr, short FAR* aRightLowerLegVPtr, short FAR* aTrunkFlexionPtr, short FAR* aTrunkRotationPtr, short FAR* aTrunkBendingPtr);
		afx_msg BOOL GetTaskInputSummaryB(BSTR FAR* aHandOrientationPtr, float FAR* aLeftHandLocHPtr, float FAR* aLeftHandLocVPtr, float FAR* aLeftHandLocLPtr, float FAR* aRightHandLocHPtr, float FAR* aRightHandLocVPtr, float FAR* aRightHandLocLPtr, short FAR* aLeftForceAngHPtr, short FAR* aLeftForceAngVPtr, float FAR* aLeftForceMagPtr, short FAR* aRightForceAngHPtr, short FAR* aRightForceAngVPtr, float FAR* aRightForceMagPtr);
		afx_msg BOOL GetFatigueReport(VARIANT_BOOL left, SHORT population_strength_percentile, SHORT* ElbowFlex, SHORT* ShoulderHumRot, SHORT* ShoulderBackFwd, SHORT* ShoulderAbdAdd, SHORT* TorsoFlex, SHORT* TorsoLatBend, SHORT* TorsoRot, SHORT* HipFlex, SHORT* KneeFlex, SHORT* AnkleFlex);
		afx_msg BOOL IsRegistered(void);
		afx_msg BOOL GetFatigueData(VARIANT_BOOL left, SHORT population_strength_percentile, SHORT* ElbowFlex, SHORT* ShoulderHumRot, SHORT* ShoulderBackFwd, SHORT* ShoulderAbdAdd, SHORT* TorsoFlex, SHORT* TorsoLatBend, SHORT* TorsoRot, SHORT* HipFlex, SHORT* KneeFlex, SHORT* AnkleFlex);
		afx_msg BOOL SetAngle(long angle, double value);
		afx_msg BOOL GetAngle(long angle, double* value);
		afx_msg BOOL SetBodySegmentAnglesWithLegs(short aForearmLH, short aForearmLV, short aForearmRH, short aForearmRV, short aUpperarmLH, short aUpperarmLV, short aUpperarmRH, short aUpperarmRV, short aUpperlegLV, short aUpperlegLH, short aUpperlegRV, short aUpperlegRH, short aLowerlegLV, short aLowerlegLH, short aLowerlegRV, short aLowerlegRH, short aTrunkFlexion, short aTrunkAxialRotation, short aTrunkLateralBending);
		//}}AFX_DISPATCH
        DECLARE_DISPATCH_MAP()
        DECLARE_INTERFACE_MAP()	
public:
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
public:
	afx_msg void OnAnimationExportavifile();
	afx_msg void OnExportavi();
	afx_msg void OnDg_Grid();
	afx_msg void OnWindowsTopview();
	afx_msg void OnUpdateWindowsTopview(CCmdUI *pCmdUI);
	afx_msg void OnWindowsFrontview();
	afx_msg void OnUpdateWindowsFrontview(CCmdUI *pCmdUI);
	afx_msg void OnWindowsSideview();
	afx_msg void OnUpdateWindowsSideview(CCmdUI *pCmdUI);
	afx_msg void OnWindowsObliqueview();
	afx_msg void OnUpdateWindowsObliqueview(CCmdUI *pCmdUI);
	afx_msg void OnWindowsStatusbox();
	afx_msg void OnUpdateWindowsStatusbox(CCmdUI *pCmdUI);
protected:

public:
	afx_msg void OnWindowsAnimationbar();
	afx_msg void OnUpdateWindowsAnimationbar(CCmdUI *pCmdUI);
public:
	 double  mmLowerArmCG ;
    double  mmLowerArmCGR;
};