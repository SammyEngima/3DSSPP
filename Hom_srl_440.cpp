// Hom_srl_440.cpp

#include "stdafx.h"
#include "AppVersion.hpp"
#include "hom.h"
#include "hom_doc.h"	// Necessary
//#include "ole2.h"
#include "ReportView.hpp"
#include "Main_frm.h"
#include "Archiver.h"
#include "GLStickView.h"
#include "GLSkeleton.h"
#include "GLObliqueView.h"
#include "Types.h"
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

// Objects used only in this file, as temporary objects used only in
//    retrieving/storing
// "static" denotes that they will only be visible to this file
//    (Hom_srl_440.cpp)
static JointAngles sJointAnglesForArchiving;
static COleVariant sBGPictureForArchiving;

//---------------------------------------------------------------------------
//	Put any retrieve-specific code in this function
void C_Hom_Doc::Serial_Retrieve_v440(CArchive &aArch)
{
   // ---------------------- Pre-Retrieve Code ----------------------
   sBGPictureForArchiving.vt = VT_UNKNOWN;
   sBGPictureForArchiving.punkVal = NULL;

   //----------------------//
   ProcessArchive_440(aArch); // ---------- Retrieve ---------- //
   //----------------------//
   
   //	--------------------- Post-Retrieve Code ----------------------

   CGLObliqueView *oblique = (CGLObliqueView*)GLViews[VIEW_OBLIQUE];
   if(oblique->getBackgroundActive())
   {
      oblique->setBackground((LPPICTURE)sBGPictureForArchiving.punkVal);      
      sBGPictureForArchiving.punkVal = NULL;
   }

   // Set the current joint angles and clear the queue (all
   //		other values will be erased, because we don't want
   //		undoing to be possible after a load operation)
   undoHistory.clearHistory();
   this->setCurrentFrame(0);
   //skeleton->SetAngles(0,sJointAnglesForArchiving);
   this->GetSkeleton()->SetAngles(sJointAnglesForArchiving);
   /* Since this is a format from before the leg definitions were changed, we have
    * to swap them to the new definitions.
	* furthermore, this was before there were foot angles, so the defaults from JointAngles
	* will be used, which are specified in the new definitions, so they will actually be swapped
	* to the old definitions, so we have to manually swap them back */
   sJointAnglesForArchiving.oldToNew();
   sJointAnglesForArchiving[A_FootLV] = 0;
   sJointAnglesForArchiving[A_FootLH] = 90;
   sJointAnglesForArchiving[A_FootRV] = 0;
   sJointAnglesForArchiving[A_FootRH] = 90;
   //skeleton->SetAngles(0, sJointAnglesForArchiving);
   
   // hands were always straight, so make hand angles same as forearm angles
   sJointAnglesForArchiving[A_HandLV] = sJointAnglesForArchiving[A_ForearmLV];
   sJointAnglesForArchiving[A_HandLH] = sJointAnglesForArchiving[A_ForearmLH];
   sJointAnglesForArchiving[A_HandRV] = sJointAnglesForArchiving[A_ForearmRV];
   sJointAnglesForArchiving[A_HandRH] = sJointAnglesForArchiving[A_ForearmRH];

   this->GetSkeleton()->SetAngles(sJointAnglesForArchiving);
	((StatusBox*)mStatusResultsWindow.mFramePtr->GetActiveView())->UpdateAll();
}

//---------------------------------------------------------------------------
//	Put any store-specific code in this function
void C_Hom_Doc::Serial_Store_v440(CArchive& aArch)
{
   // -------------------------- Pre-Store Code --------------------------

   // Prepare WindowData objects for saving (including reports)
   SaveQuadFrameWindowData();
   SaveReportWindowData();

   // Prepare BG picture for saving, if necessary
   CGLObliqueView *oblique = (CGLObliqueView*)GLViews[VIEW_OBLIQUE];
   if(oblique->getBackgroundActive())
   {
      sBGPictureForArchiving.vt = VT_UNKNOWN;
      sBGPictureForArchiving.punkVal = oblique->getBackground();	// This code converts the IPicture to a COleVariant for writing	
   }

   // Prepare current joint angles for archiving
   sJointAnglesForArchiving = this->GetSkeleton()->GetAngles();

   //////////////////////////
   ProcessArchive_440(aArch);		// Store!
   //////////////////////////
   
   //	------------------------- Post-Store Code --------------------------

   if(oblique->getBackgroundActive())
      sBGPictureForArchiving.punkVal = NULL;						// This stops it from losing the picture
}

//---------------------------------------------------------------------------
// ProcessArchive_440 is used both to store and load archives.  This is
//		to ensure that our storing and retrieving of variables always
//		happen in the same order as one another.
//
void C_Hom_Doc::ProcessArchive_440 (CArchive& Arch)
{
   // Instantiate the function object - it will
   //		automatically detect whether to restore or save
   Archiver archiver(Arch);

   // Pass this variable in as the second parameter to archiver if
   //		converting from an enumerated variable type is necessary.
   int tempInt;

   /////////////////////////////////////////////////////////////////////////
   //////////////////////////// BEGIN ARCHIVING ////////////////////////////
   /////////////////////////////////////////////////////////////////////////

// --- Task related variables ---
   archiver(mDescription);
   if(mDescription.Units() == 1) {
	   this->SetUnits(U_METRIC);
   } else {
	   this->SetUnits(U_ENGLISH);
   }
   double gravity = task.getGravity();
   archiver(gravity);
   task.setGravity(gravity);
   
// --- Anthropometry ---
   Anthropometry anth = task.getAnthropometryRef();
   archiver(anth);
   task.setAnthropometry(anth);
   
// --- Posture Prediction ---
   archiver(Locking_Mode, tempInt);

   E_Hand_Orientation tempHO = task.Get_Hand_Orientation();
   archiver(tempHO, tempInt);
   task.Put_Hand_Orientation(tempHO);

   //int Manipulate_Mode = (int)skeleton->Get_Manipulation_State();
   archiver(tempInt);
   //skeleton->Put_Manipulation_State((E_Manipulation_State)Manipulate_Mode);

   Length tempLength;
   archiver(tempLength); // mHandLocLH
   archiver(tempLength); // mHandLocLL
   archiver(tempLength); // mHandLocLV
   archiver(tempLength); // mHandLocRH
   archiver(tempLength); // mHandLocRL
   archiver(tempLength); // mHandLocRV

   // --- Hand Forces ---
   Skeleton& lSkeleton = *this->GetSkeleton();
   double rv,rh,lv,lh,rmag,lmag;
   rmag = lSkeleton.getExtForce(JT_RHAND).length();
   lmag = lSkeleton.getExtForce(JT_LHAND).length();

   //skeleton->GetHandForceAngles(rv,rh,lv,lh);
	Vector3 LHandForce = lSkeleton.getExtForce(JT_LHAND);
	Vector3 RHandForce = lSkeleton.getExtForce(JT_RHAND);
	// convert force vectors to spherical angles
	if(LHandForce == Vector3(0,0,0)) lv = -90;
	else lv = 90.0 - degreesBetween(LHandForce,Vector3(0,0,1));
	if(RHandForce == Vector3(0,0,0)) rv = -90;
	else rv = 90.0 - degreesBetween(RHandForce,Vector3(0,0,1));

	RHandForce[2] = 0.0;
	if(RHandForce == Vector3(0,0,0)) rh = 90.0;
	else rh = degreesBetween(RHandForce,Vector3(1,0,0));
	if(RHandForce[1] < 0) rh *= -1;

	LHandForce[2] = 0.0;
	if(LHandForce == Vector3(0,0,0)) lh = 90.0;
	else lh = degreesBetween(LHandForce,Vector3(1,0,0));
	if(LHandForce[1] < 0) lh *= -1;

   archiver(lmag);
   archiver(lh);
   archiver(lv);
   archiver(rmag);
   archiver(rh);
   archiver(rv);

   //skeleton->Put_Force_Magnitudes(rmag,lmag);
   //skeleton->Put_Force_Angles(rv,rh,lv,lh);
   	// convert spherical angles to force vectors
    RHandForce[0] = cos(rv*M_PI/180.0) * cos(rh*M_PI/180.0);
    RHandForce[1] = cos(rv*M_PI/180.0) * sin(rh*M_PI/180.0);
    RHandForce[2] = sin(rv*M_PI/180.0);

    LHandForce[0] = cos(lv*M_PI/180.0) * cos(lh*M_PI/180.0);
    LHandForce[1] = cos(lv*M_PI/180.0) * sin(lh*M_PI/180.0);
    LHandForce[2] = sin(lv*M_PI/180.0);

	LHandForce *= lmag;
	RHandForce *= rmag;
	lSkeleton.setExtForce(JT_LHAND, LHandForce, true);
	lSkeleton.setExtForce(JT_RHAND, RHandForce, true);

   // --- Other Additional Joint Torques and Forces ---
   // ttarray [ 2 + 1 ][ 16 + 1 ][ 3 + 1 ]
   JointID order[17] = {JT_RHAND,JT_RHAND,JT_RELBOW,JT_RSHOULDER,JT_LHAND,JT_LELBOW,JT_LSHOULDER,JT_HIP_CENTER,
                        JT_RHIP,JT_RKNEE,JT_RANKLE,JT_LHIP,JT_LKNEE,JT_LANKLE,JT_L5S1_TEMP,JT_RIT,JT_LIT};

   // CJB
   // Problems:
   // 1) k<=3 got changed to k<3 somehow, this is not ok, it would leave 17 * 3 doubles in the file
   // 2) I don't know what i is supposed to represent, but we're totally disregarding it, and also
   //	 clobbering the i = 0,1 data with the final i = 2 data.
   // 3) I doubt that it should be getExtForce(order[j])[j] and not [j])[k]
   //    This must have been a typo, j goes to 16, far out of the Vector3 bounds [0..2]
   Vector3 torque, force;
   double dummydouble;
   for (int i=0;i<=2;i++) {
	   for (int j=0;j<=16;j++) {
		for (int k=0;k<=3;k++) {
			if(k == 0 || i != 1 || order[j] == JT_RHAND || order[j] == JT_LHAND) {
				archiver(dummydouble);
				archiver(dummydouble);
			} else {
				dummydouble = lSkeleton.getExtTorque(order[j], true)[k-1];
				archiver(dummydouble); //mATorq[i][j][k]);
				torque[k - 1] = dummydouble;
				dummydouble = lSkeleton.getExtForce(order[j], true)[k-1];
				archiver(dummydouble); //mAForce[i][j][k]);
				force[k - 1] = dummydouble;
			}
	    }
		if(i == 1 && order[j] != JT_RHAND && order[j] != JT_LHAND) {
			lSkeleton.setExtTorque(order[j],torque, true);
			lSkeleton.setExtForce(order[j],force, true);
		}
	   }
   }

// --- Misc Booleans ---
   archiver(mAutoTile);
   archiver(mStatusBar);
   BOOL m2DMode = false;//(Task_Mode == TM_TwoDStatic); // Even though we will never save with 440.
   archiver(m2DMode);
   // TODO 2d clean
   //if(m2DMode) {
//	   OnTaskinput2dstatic();
  // } else {
	   OnTaskinput3dstatic();
   //}
// --- Support Selection ---
   //mStance is saved in special parameters
   int mPositioning = 0;
   if(task.getPosition() == P_Sitting) mPositioning |= 0x1;
   if(task.hasSeatBackRest()) mPositioning |= 0x2;
   if(task.hasFrontSeatPanSupport()) mPositioning |= 0x4;
   archiver(mPositioning);
   task.setPosition((mPositioning & 0x1) ? P_Sitting : P_Standing);
   task.setBackRest(mPositioning & 0x2);
   task.setFrontSeatPanSupport(mPositioning & 0x4);

   Length mSeatHeight;
   mSeatHeight.Value(task.getBackRestHeight());
   archiver(mSeatHeight);
   task.setBackRestHeight(mSeatHeight.Value());

// --- Window Data ---
   archiver(mFrontWindowData);
   archiver(mSideWindowData);
   archiver(mTopWindowData);
   archiver(mObliqueWindowData);
   archiver(mStatusResultsWindow);
   archiver(mMainWindowData);

// --- Oblique View ----
   CGLObliqueView *oblique = (CGLObliqueView*)GLViews[VIEW_OBLIQUE];
   E_Model_Type modelType = oblique->getModelType();
   archiver(modelType, tempInt);
   oblique->setModelType(modelType);

   E_Hand_Type Hand_Type = task.Get_Left_Hand_Type();
   archiver(Hand_Type, tempInt);
   task.Put_Hand_Type(Hand_Type, Hand_Type);

// --- Colors ---
   // Background Colors
   archiver(GLOblique::backgroundColor[0]);
   archiver(GLOblique::backgroundColor[1]);
   archiver(GLOblique::backgroundColor[2]);
   
   archiver(GLSkeleton::backgroundColor[0]);
   archiver(GLSkeleton::backgroundColor[1]);
   archiver(GLSkeleton::backgroundColor[2]);

   // Report Colors
   archiver(Report_BG_Red);
   archiver(Report_BG_Green);
   archiver(Report_BG_Blue);

   // Force Arrows Visible
   bool showForces = GLSkeleton::getShowForces();
   archiver(showForces);
   GLSkeleton::setShowForces(showForces);

      // Grid  Visible
   bool showGrid = GLSkeleton::getShowGrid();
   archiver(showGrid);
   GLSkeleton::setShowGrid(showGrid);

   double GridSize = GLSkeleton::getGridSize();
   archiver(GridSize);
   GLSkeleton::setGridSize(GridSize);
   // Force Colors
   archiver(GLSkeleton::forceColor[0]);
   archiver(GLSkeleton::forceColor[1]);
   archiver(GLSkeleton::forceColor[2]);

   // Right Feet Force Colors
   archiver(GLSkeleton::rightfeetforceColor[0]);
   archiver(GLSkeleton::rightfeetforceColor[1]);
   archiver(GLSkeleton::rightfeetforceColor[2]);

   // Feet Force Colors
   archiver(GLSkeleton::leftfeetforceColor[0]);
   archiver(GLSkeleton::leftfeetforceColor[1]);
   archiver(GLSkeleton::leftfeetforceColor[2]);

   // Figure Colors
   archiver(GLSkeleton::skeletonColor[0]);
   archiver(GLSkeleton::skeletonColor[1]);
   archiver(GLSkeleton::skeletonColor[2]);

   archiver(GLOblique::skinColor[0]);
   archiver(GLOblique::skinColor[1]);
   archiver(GLOblique::skinColor[2]);
   //	Grid colors
   archiver(GLSkeleton::GridColor[0]);
   archiver(GLSkeleton::GridColor[1]);
   archiver(GLSkeleton::GridColor[2]);

   // Clothing toggles
   bool temp = (task.getFactors().mShoeHeight != 0);//oblique->getShoesEnabled();
   archiver(temp);
   // oblique->setShoesEnabled(temp);
   float shoeHeight;
   if(temp) {
	    Factors def;
		def.RestoreMaleDefaults();
		shoeHeight = def.mShoeHeight;
   } else {
	   shoeHeight = 0;
   }
	Factors factors = task.getFactors(S_Male);
	factors.mShoeHeight = shoeHeight;
	task.setMaleFactors(factors);
	factors = task.getFactors(S_Female);
	factors.mShoeHeight = shoeHeight;
	task.setFemaleFactors(factors);


   // Environment Colors
   BYTE tempBYTE;
   /*archiver(Obl_HHO_Red);
   archiver(Obl_HHO_Green);
   archiver(Obl_HHO_Blue);

   archiver(Obl_HHO_Outline_Red);
   archiver(Obl_HHO_Outline_Green);
   archiver(Obl_HHO_Outline_Blue);*/

   tempBYTE = (BYTE)GLOblique::handHeldColor[0];
   archiver(tempBYTE);
   GLOblique::handHeldColor[0] = (double)tempBYTE;

   tempBYTE = (BYTE)GLOblique::handHeldColor[1];
   archiver(tempBYTE);
   GLOblique::handHeldColor[1] = (double)tempBYTE;

   tempBYTE = (BYTE)GLOblique::handHeldColor[2];
   archiver(tempBYTE);
   GLOblique::handHeldColor[2] = (double)tempBYTE;

   tempBYTE = (BYTE)GLOblique::handHeldOutlineColor[0];
   archiver(tempBYTE);
   GLOblique::handHeldOutlineColor[0] = (double)tempBYTE;

   tempBYTE = (BYTE)GLOblique::handHeldOutlineColor[1];
   archiver(tempBYTE);
   GLOblique::handHeldOutlineColor[1] = (double)tempBYTE;

   tempBYTE = (BYTE)GLOblique::handHeldOutlineColor[2];
   archiver(tempBYTE);
   GLOblique::handHeldOutlineColor[2] = (double)tempBYTE;


   // Clothing Colors
   archiver(GLOblique::pantsColor[0]);
   archiver(GLOblique::pantsColor[1]);
   archiver(GLOblique::pantsColor[2]);

   archiver(GLOblique::shirtColor[0]);
   archiver(GLOblique::shirtColor[1]);
   archiver(GLOblique::shirtColor[2]);

// --- Lighting ---
   archiver(mUseNewLightScheme);
   archiver(mLightIntensity);

   archiver(Obl_Light_Type_FB);
   archiver(Obl_Light_Type_LR);
   archiver(Obl_Light_Type_TB);

// --- Camera Data ---
   // (mpt) removed serialization of film height/width, they were essentially constants, and useless at that
   int camDummyInt = 0;
   archiver(oblique->getCamera().focalLength);
   archiver(camDummyInt);
   archiver(camDummyInt);
   archiver(oblique->getCamera().distance);
   archiver(oblique->getCamera().altitude);
   archiver(oblique->getCamera().azimuth);
   archiver(oblique->getCamera().horizontalOffset);
   archiver(oblique->getCamera().verticalOffset);
   
// --- Environment ---
   int floorEnabled = (int)oblique->getFloorEnabled();
   archiver(camDummyInt); // seat visible not really an option
   archiver(floorEnabled);
   oblique->setFloorEnabled((bool)floorEnabled);

   // These were BOOLs (ints) in the old code, so we have to leave them as ints, not real bools
   BOOL fakeBOOLForRealbool;
   archiver(fakeBOOLForRealbool);
   oblique->getHandHeld().outline = fakeBOOLForRealbool;
   archiver(fakeBOOLForRealbool);
   oblique->getHandHeld().type = fakeBOOLForRealbool;

   // These were ints in the old code, so we have to leave them as ints, not floats
   int intForFloat;
   archiver(intForFloat);
   oblique->getHandHeld().depth = intForFloat;
   archiver(intForFloat);
   oblique->getHandHeld().height = intForFloat;

   archiver(Bar_Type, tempInt);
   archiver(Bar_Width);
   archiver(Bar_Length);
   archiver(Bar_Thickness);
   archiver(Bar_Distance);
   archiver(Bar_Elevation);
   archiver(Bar_Azimuth);
   archiver(Bar_Pitch);
   // --- AutoCAD ---
   // (mpt) obsolete, dummy variables used
   int AC_dummyInt = 0;
   archiver(AC_dummyInt); //Hom_AC_Refresh_Mode
   archiver(AC_dummyInt); //Hom_AC_Units_Mode
   archiver(AC_dummyInt); //Hom_AC_Position_Source_Mode
   archiver(AC_dummyInt); //Hom_AC_Positioned_Frame_Mode
   archiver(AC_dummyInt); //AC_Active_Zone
   
   // Position data
   double AC_dummyDbl = 0.0;
   archiver(AC_dummyDbl); // Hom_AC_Tra_X
   archiver(AC_dummyDbl); // Hom_AC_Tra_Y
   archiver(AC_dummyDbl); // Hom_AC_Tra_Z

   archiver(AC_dummyInt); //Hom_AC_Rot_X
   archiver(AC_dummyInt); //Hom_AC_Rot_Y
   archiver(AC_dummyInt); //Hom_AC_Rot_Z
   
   // Zone data
   CString AC_dummyCStr = "dummy";
   for( int ZN = 0; ZN < 6; ZN++ ) // Number_Of_Zones
   {
      archiver(AC_dummyCStr); //Zone_Names[ ZN ]
      archiver(AC_dummyDbl); //Zone_TX[ ZN ]
      archiver(AC_dummyDbl); //Zone_TY[ ZN ]
      archiver(AC_dummyDbl); //Zone_TZ[ ZN ]

      archiver(AC_dummyInt); //Zone_RX[ ZN ]
      archiver(AC_dummyInt); //Zone_RY[ ZN ]
      archiver(AC_dummyInt); //Zone_RZ[ ZN ]
   }

// --- Saved UCS Name Strings --- (mpt) obsolete, load into dummy for compatability
   CString dummy = "dummy";
   archiver(dummy); //UCS_Foot_Left_Value
   archiver(dummy); //UCS_Foot_Right_Value
   archiver(dummy); //UCS_Hand_Left_Value
   archiver(dummy); //UCS_Hand_Right_Value
   archiver(dummy); //UCS_World_Value
   
//--- Additional members ---
   //archiver(mDocFactors);
   Factors mMaleFactors = task.getFactors(S_Male);
   Factors mFemaleFactors = task.getFactors(S_Female);
   archiver(mMaleFactors);
   archiver(mFemaleFactors);
   task.setMaleFactors(mMaleFactors);
   task.setFemaleFactors(mFemaleFactors);
 
   float *stature, *weight;
   Anthropometry anthro = getAnthropometryRef();
   stature = anthro.GetHeightPercentiles();
   weight =  anthro.GetWeightPercentiles();
   // These are statics so they should be the same
   // for the male and female hominoids
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

// --- Special Parameters ---
   int mStance;
   FeetSupport_e fs = this->getFeetSupport();
   if(fs == FS_Both) mStance = 0;
   else if(fs == FS_Left) mStance = 1;
   else if(fs == FS_Right) mStance = 2;
   else if(fs == FS_Neither) mStance = 3;
   archiver(mStance);
   if(mStance == 0) this->setFeetSupport(FS_Both);
   else if(mStance == 1) this->setFeetSupport(FS_Left);
   else if(mStance == 2) this->setFeetSupport(FS_Right);
   else if(mStance == 3) this->setFeetSupport(FS_Neither);

   double tempFootAngle = sJointAnglesForArchiving[A_FootRH];
   archiver(tempFootAngle);//mRightFootAngle);
   sJointAnglesForArchiving[A_FootRH] = 90-tempFootAngle;
   tempFootAngle = sJointAnglesForArchiving[A_FootLH];
   archiver(tempFootAngle);//mLeftFootAngle);
   sJointAnglesForArchiving[A_FootLH] = 90-tempFootAngle;

   double tempDelta = 0.0;
   archiver(tempDelta); //mDeltaX
   archiver(tempDelta); //mDeltaY

   double tempTilt = this->GetSkeleton()->GetAngle(A_PelvisFlexion);//getPelvicTilt();
   archiver(tempTilt);
   this->GetSkeleton()->SetAngle(A_PelvisFlexion,tempTilt);//setPelvicTilt(tempTilt);

// --- Open Reports ---
   // (DO NOT CHANGE THIS ORDER, nor try to do this with a map
   //		iterator.  If the report ID's get reordered automatically,
   //		it might try to read these in a different order, so keep
   //		them like this.)
   archiver(mReportWDMap[ID_REPORTS_ANALYSIS_SUMMARY]);
   archiver(mReportWDMap[ID_REPORTS_ANTHROPOMETRY]);
   archiver(mReportWDMap[ID_REPORTS_BALANCE_STANDING]);
   archiver(mReportWDMap[ID_REPORTS_BALANCE_SEATED]);
   archiver(mReportWDMap[ID_REPORTS_FATIGUE]);
   archiver(mReportWDMap[ID_REPORTS_POSTURE]);
   archiver(mReportWDMap[ID_REPORTS_JOINT_LOCATIONS]);
   archiver(mReportWDMap[ID_REPORTS_CENTEROFMASSLOCATIONS]);
   archiver(mReportWDMap[ID_REPORTS_JOINT_MOMENTS]);
   archiver(mReportWDMap[ID_REPORTS_JOINT_FORCES]);
   archiver(mReportWDMap[ID_REPORTS_3D_LOWBACK]);
   archiver(mReportWDMap[ID_REPORTS_SAGITTAL]);
   archiver(mReportWDMap[ID_REPORTS_SPINAL]);
   archiver(mReportWDMap[ID_REPORTS_STRENGTH]);
   archiver(mReportWDMap[ID_REPORTS_STRENGTH_DIR_VECTORS]);
   archiver(mReportWDMap[ID_REPORTS_TASK_SUMMARY]);

   /*archiver(mCOPWind);
   archiver(mSeatCOPWind);*/		// These windows no longer exist

// --- Background Picture ---
   bool bgActive = oblique->getBackgroundActive();
   archiver(bgActive);
   oblique->setBackgroundActive(bgActive);
   if(bgActive)
   {
        int bgMode = (int)(oblique->getBackgroundMode());
        archiver(bgMode);
        oblique->setBackgroundMode((Picture_Mode)bgMode);
        archiver(sBGPictureForArchiving);
   }

   // --- Joint Angles ---
   // (Leave these last so we can immediately tell if something
   //		is awry with the loading - the joint angles will be messed up.
   archiver(sJointAnglesForArchiving);

   /////////////////////////////////////////////////////////////////////////
   ///////////////////////////// END ARCHIVING /////////////////////////////
   /////////////////////////////////////////////////////////////////////////
}
