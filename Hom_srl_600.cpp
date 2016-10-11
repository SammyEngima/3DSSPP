// Hom_srl_600.cpp

#include "stdafx.h"
#include "AppVersion.hpp"
#include "hom.h"
#include "hom_doc.h"	// Necessary
#include "ole2.h"
#include "ReportView.hpp"
#include "Main_frm.h"
#include "Archiver.h"
//#include "GLStickView.h"
#include "GLSkeleton.h"
#include "GLObliqueView.h"
#include "GLOblique.h"
#include "StatusBox.h"

/*###########################################################################
  
  	If ANY changes are made to this, use comments to show which
               version uses which different settings.  The ONLY CHANGES that
               will be necessary after a version's release are to read in
               dummy variables where certain variables are no longer a part
               of the program.
  
      Example - If you're reading in a variable which is no longer used at all
         do the following:

            // Previously mStatusHandsWindow - removed in v4.3x
            WindowData dummyWD;
            archiver(dummyWD);

   ##########################################################################*/
 

// CAUTION: Nowhere in this file should there be any <<'s or >>'s!  Only the
//    Archiver object is used.  This will avoid the risk of getting save/load
//    desync'd.
static COleVariant sBGPicture;

void C_Hom_Doc::Serial_Retrieve_v600(CArchive& aArch) {

	// Pre retrieve work

	ProcessArchive_600(aArch); // Retrieve

	// Post retrieval work
	undoHistory.clearHistory();
	LoadQuadFrameWindowData();
	((StatusBox*)mStatusResultsWindow.mFramePtr->GetActiveView())->UpdateAll();

}

void C_Hom_Doc::Serial_Store_v600(CArchive& aArch) {

	// Pre Store work

	// Prepare WindowData objects for saving (including reports)
	SaveQuadFrameWindowData();
	SaveReportWindowData();

	ProcessArchive_600(aArch);
}

void C_Hom_Doc::ProcessArchive_600(CArchive& Arch) {

	// Instantiate the function object - it will
	//		automatically detect whether to restore or save
	Archiver archiver(Arch);

	// Pass this variable in as the second parameter to archiver if
	//			Converting from an enumerated variable type is necessary.
	int tempInt;

	/********* Begin Archiving *********/

	// Posture Prediction
	archiver(Locking_Mode, tempInt);
	((GLSkeleton*)GLViews[VIEW_TOP]->getGLInteractive())->setLocking(Locking_Mode);
	((GLSkeleton*)GLViews[VIEW_SIDE]->getGLInteractive())->setLocking(Locking_Mode);
	((GLSkeleton*)GLViews[VIEW_FRONT]->getGLInteractive())->setLocking(Locking_Mode);

	archiver(mDescription);
	this->SetUnits((E_Units)mDescription.Units());

	archiver(mAutoTile);
	archiver(mStatusBar);
	archiver(Task_Mode, tempInt);

	// Window Data
	archiver(mFrontWindowData);
	archiver(mSideWindowData);
	archiver(mTopWindowData);
	archiver(mObliqueWindowData);
	archiver(mAnimationWindowData);
	archiver(mStatusResultsWindow);
	archiver(mMainWindowData);

	// Oblique view special things, this is annoying.  Push these back into views
	CGLObliqueView* oblique = (CGLObliqueView*)GLViews[VIEW_OBLIQUE];

	bool bgActive = oblique->getBackgroundActive();
	archiver(bgActive);
	oblique->setBackgroundActive(bgActive);
	if(bgActive) {

		int bgMode = (int)(oblique->getBackgroundMode());
		archiver(bgMode);
		oblique->setBackgroundMode((Picture_Mode)bgMode);

		COleVariant BGPicture;
		BGPicture.vt = VT_UNKNOWN;
		BGPicture.punkVal = oblique->getBackground();
		archiver(BGPicture);
		if(BGPicture.punkVal != oblique->getBackground()) {
			oblique->setBackground((LPPICTURE)BGPicture.punkVal);
		}
		BGPicture.punkVal = 0;

		float alpha = oblique->getBackgroundAlpha();
		archiver(alpha);
		oblique->setBackgroundAlpha(alpha);
	}

	E_Model_Type modelType = oblique->getModelType();
	archiver(modelType, tempInt);
	oblique->setModelType(modelType);

	E_Hand_Type Hand_Type = task.Get_Left_Hand_Type();
	archiver(Hand_Type, tempInt);
	task.Put_Hand_Type(Hand_Type, Hand_Type);

	int floorEnabled = oblique->getFloorEnabled();
	archiver(floorEnabled);
	oblique->setFloorEnabled(floorEnabled);

	/*bool shoesEnabled = oblique->getShoesEnabled();
	archiver(shoesEnabled);
	oblique->setShoesEnabled(shoesEnabled);*/

	archiver(GLOblique::backgroundColor[0]);
	archiver(GLOblique::backgroundColor[1]);
	archiver(GLOblique::backgroundColor[2]);

	archiver(GLOblique::skinColor[0]);
	archiver(GLOblique::skinColor[1]);
	archiver(GLOblique::skinColor[2]);

	archiver(GLOblique::handHeldColor[0]);
	archiver(GLOblique::handHeldColor[1]);
	archiver(GLOblique::handHeldColor[2]);

	archiver(GLOblique::handHeldOutlineColor[0]);
	archiver(GLOblique::handHeldOutlineColor[1]);
	archiver(GLOblique::handHeldOutlineColor[2]);

    archiver(GLOblique::maleShirtColor[0]);
	archiver(GLOblique::maleShirtColor[1]);
	archiver(GLOblique::maleShirtColor[2]);

	archiver(GLOblique::malePantsColor[0]);
	archiver(GLOblique::malePantsColor[1]);
	archiver(GLOblique::malePantsColor[2]);

	archiver(GLOblique::femaleShirtColor[0]);
	archiver(GLOblique::femaleShirtColor[1]);
	archiver(GLOblique::femaleShirtColor[2]);

	archiver(GLOblique::femalePantsColor[0]);
	archiver(GLOblique::femalePantsColor[1]);
	archiver(GLOblique::femalePantsColor[2]);

	archiver(mUseNewLightScheme);
	archiver(mLightIntensity);

	archiver(Obl_Light_Type_FB);
	archiver(Obl_Light_Type_LR);
	archiver(Obl_Light_Type_TB);

	archiver(oblique->getCamera().focalLength);
	archiver(oblique->getCamera().distance);
	archiver(oblique->getCamera().altitude);
	archiver(oblique->getCamera().azimuth);
	archiver(oblique->getCamera().horizontalOffset);
	archiver(oblique->getCamera().verticalOffset);

	archiver(oblique->getHandHeld().outline);
	archiver(oblique->getHandHeld().type);
	archiver(oblique->getHandHeld().depth);
	archiver(oblique->getHandHeld().height);

	archiver(Bar_Type, tempInt);
	archiver(Bar_Width);
	archiver(Bar_Length);
	archiver(Bar_Thickness);
	archiver(Bar_Distance);
	archiver(Bar_Elevation);
	archiver(Bar_Azimuth);
	if(OpenType>699){
		archiver(Bar_Pitch);}
	// Orthoview special things
	archiver(GLSkeleton::backgroundColor[0]);
	archiver(GLSkeleton::backgroundColor[1]);
	archiver(GLSkeleton::backgroundColor[2]);

	archiver(GLSkeleton::forceColor[0]);
	archiver(GLSkeleton::forceColor[1]);
	archiver(GLSkeleton::forceColor[2]);

	archiver(GLSkeleton::skeletonColor[0]);
	archiver(GLSkeleton::skeletonColor[1]);
	archiver(GLSkeleton::skeletonColor[2]);

	archiver(GLSkeleton::floorColor[0]);
	archiver(GLSkeleton::floorColor[1]);
	archiver(GLSkeleton::floorColor[2]);

	archiver(GLSkeleton::floorOutlineColor[0]);
	archiver(GLSkeleton::floorOutlineColor[1]);
	archiver(GLSkeleton::floorOutlineColor[2]);

	archiver(GLSkeleton::leftJointColor[0]);
	archiver(GLSkeleton::leftJointColor[1]);
	archiver(GLSkeleton::leftJointColor[2]);

	archiver(GLSkeleton::rightJointColor[0]);
	archiver(GLSkeleton::rightJointColor[1]);
	archiver(GLSkeleton::rightJointColor[2]);
	if(OpenType > 699){
	archiver(GLSkeleton::GridColor[0]);
	archiver(GLSkeleton::GridColor[1]);
	archiver(GLSkeleton::GridColor[2]);
	}
	bool showForces = GLSkeleton::getShowForces();
	archiver(showForces);
	GLSkeleton::setShowForces(showForces);
	if(OpenType > 699){
	bool showGrid = GLSkeleton::getShowGrid();
	archiver(showGrid);
	GLSkeleton::setShowGrid(showGrid);

   double GridSize = GLSkeleton::getGridSize();
   archiver(GridSize);
   GLSkeleton::setGridSize(GridSize);
	}
	// Report colors
	archiver(Report_BG_Red);
	archiver(Report_BG_Green);
	archiver(Report_BG_Blue);

	// This is bad
	// These are statics so they should be the same for the male and the female hominoids
	Anthropometry anthro = getAnthropometryRef();
	float* stature = anthro.GetHeightPercentiles();
	float* weight = anthro.GetWeightPercentiles();
	const Factors& mMaleFactors = task.getFactors();
	stature[0] = mMaleFactors.mMH95;
	stature[1] = mMaleFactors.mMH50;
	stature[2] = mMaleFactors.mMH5;
	stature[3] = mMaleFactors.mFH95;
	stature[4] = mMaleFactors.mFH50;
	stature[5] = mMaleFactors.mFH5;
	weight[0] = mMaleFactors.mMW95;
	weight[1] = mMaleFactors.mMW50;
	weight[2] = mMaleFactors.mMW5;
	weight[3] = mMaleFactors.mFW95;
	weight[4] = mMaleFactors.mFW50;
	weight[5] = mMaleFactors.mFW5;
	setAnthropometry(anthro);

	archiver(mReportWDMap[ID_REPORTS_ANALYSIS_SUMMARY]);
	archiver(mReportWDMap[ID_REPORTS_ANTHROPOMETRY]);
	archiver(mReportWDMap[ID_REPORTS_BALANCE_STANDING]);
	archiver(mReportWDMap[ID_REPORTS_BALANCE_SEATED]);
	archiver(mReportWDMap[ID_REPORTS_FATIGUE]);
	archiver(mReportWDMap[ID_REPORTS_POSTURE]);
	archiver(mReportWDMap[ID_REPORTS_JOINT_LOCATIONS]);
	archiver(mReportWDMap[ID_REPORTS_JOINT_MOMENTS]);
	archiver(mReportWDMap[ID_REPORTS_JOINT_FORCES]);
	archiver(mReportWDMap[ID_REPORTS_3D_LOWBACK]);
	archiver(mReportWDMap[ID_REPORTS_SAGITTAL]);
	archiver(mReportWDMap[ID_REPORTS_SPINAL]);
	archiver(mReportWDMap[ID_REPORTS_STRENGTH]);
	archiver(mReportWDMap[ID_REPORTS_STRENGTH_DIR_VECTORS]);
	archiver(mReportWDMap[ID_REPORTS_TASK_SUMMARY]);
	#if defined(SHOULDERDLL)
	archiver(mReportWDMap[ID_REPORTS_SHOULDERANALYSIS]);
	#endif


	archiver(task);
	// The new static mode is the same as the old dynamic mode, so set static for both TM_ThreeDStatic and TM_ThreeDDynamic
	switch(Task_Mode) {	case TM_ThreeDStatic: OnTaskinput3dstatic(); break;
						case TM_ThreeDDynamic: OnTaskinput3dstatic(); break; }

	archiver(limitChoice, tempInt);
	archiver(lowerPercentileLimit);
	archiver(upperPercentileLimit);
	archiver(femaleLowerPercentileLimit);
	archiver(femaleUpperPercentileLimit);

	archiver(offsetChoice, tempInt);
	archiver(offsetCoordinate[0]);
	archiver(offsetCoordinate[1]);
	archiver(offsetCoordinate[2]);
	archiver(jointToBeUsedAsOffset, tempInt);
	archiver(addOffset);

	((StatusBox*)mStatusResultsWindow.mFramePtr->GetActiveView())->UpdatePercentileLimits();

	// added 6.0.5
	if(OpenType >= 605) {
		archiver(fixedHandAngles);
	}
	if(OpenType >699){
		archiver(fixedHeadAngles);}
	if(OpenType >=605){
		archiver(maintainHandAngles);}
	if(OpenType >699){
		archiver(maintainHeadAngles);
	}
}
