#include "stdafx.h"
#include "AppVersion.hpp"
#include "hom.h"
#include "hom_doc.h"
#include "GLStickView.h"
#include "GLObliqueView.h"

/* ##########################################################################
 *
 *	If ANY changes are made to this, use comments to show which
 *		version uses which different settings.  The ONLY CHANGES that will
 *		be necessary after a version's release are to read in dummy variables
 *		where certain variables are no longer a part of the program.
 *
 *  Example - If you're reading in a variable which is no longer used at all
 *  do the following: 
 *
 *		WindowData dummyWD;
 *		aArch >> dummyWD;	// Previously mStatusHandsWindow - removed in v4.3x
 * 
 * ##########################################################################
 */

//---------------------------------------------------------------------------
/*void C_Hom_Doc::Serial_Retrieve_v42x_a (CArchive & aArch)
{
   this->Serial_Retrieve_v42x_b( aArch );
   
   //--- Additional 32-2 members
   Factors lFactors;
   aArch >> lFactors;
   this->SetMaleFactors(lFactors);
   this->SetFemaleFactors(lFactors);
   
   aArch >> mAutoTile;
   aArch >> mStatusBar;
   
   //long	Signed_Int;
   //aArch >> Signed_Int;
   //m2DMode = (bool) Signed_Int;
   
   BOOL m2DMode;
   aArch >> m2DMode;
   if(m2DMode) Task_Mode = TM_TwoDStatic;
   else	Task_Mode = TM_ThreeDStatic;
   
   aArch >> this->mFrontWindowData;
   aArch >> this->mSideWindowData;
   aArch >> this->mTopWindowData;
   aArch >> this->mObliqueWindowData;
   
   aArch >> this->mStatusResultsWindow;

   WindowData dummyWD;
   aArch >> dummyWD;	// Was mStatusHandsWindow - removed in v4.3x
   
   // The way these reports are read has changed -
   //		don't try to open them.
   BOOL dummyBOOL;
   aArch >> dummyBOOL;   //this->mReportAnalysisSummary;	 
   aArch >> dummyBOOL;   //this->mReportAnthropometry;
   aArch >> dummyBOOL;   //this->mReportPosture;	 
   aArch >> dummyBOOL;   //this->mReportJointLocations;	 
   aArch >> dummyBOOL;   //this->mReportJointMoments;	 
   aArch >> dummyBOOL;   //this->mReportLowBack;	 
   aArch >> dummyBOOL;   //this->mReportSagittal;	 
   aArch >> dummyBOOL;   //this->mReportSpinal;	 
   aArch >> dummyBOOL;   //this->mReportStrength;	 
   aArch >> dummyBOOL;   //this->mReportStrengthDirVectors; 
   aArch >> dummyBOOL;   //this->mReportTaskInputSummary; 
   
   int dummyInt;
   aArch >> dummyInt; //this->Ort_Light_Type_FB;
   aArch >> dummyInt; //this->Ort_Light_Type_LR;
   aArch >> dummyInt; //this->Ort_Light_Type_TB;
   
   aArch >> this->Obl_Light_Type_FB;
   aArch >> this->Obl_Light_Type_LR;
   aArch >> this->Obl_Light_Type_TB;
   
   return;
}

void C_Hom_Doc::Serial_Retrieve_v42x_b( CArchive& Arch )
{
   long	Signed_Int;

   // The old versions had less precision in some cases;
   // we still need to read in floats to avoid getting off
   // in the serialization
   float tempFloat;   
   int tempInt;

   //Task related variables
   Arch >> mDescription;

   //--- Anthropometry ---
   Arch >> mAnthropometry;

   //--- Posture Prediction ---
   Arch >> Signed_Int; this->Locking_Mode	  = (E_Locking_Mode)Signed_Int;
   Arch >> Signed_Int; skeleton->Put_Hand_Orientation((E_Hand_Orientation)Signed_Int);
   Arch >> Signed_Int; //skeleton->Put_Manipulation_State((E_Manipulation_State)Signed_Int);

   // Account for inferior precision of older versions
   Arch >> tempFloat;   //mHandLocLH.Value(tempFloat);
   Arch >> tempInt;     //mHandLocLH.Decimals(tempInt);
   Arch >> tempFloat;   //mHandLocLL.Value(tempFloat);
   Arch >> tempInt;     //mHandLocLL.Decimals(tempInt);
   Arch >> tempFloat;   //mHandLocLV.Value(tempFloat);
   Arch >> tempInt;     //mHandLocLV.Decimals(tempInt);
   Arch >> tempFloat;   //mHandLocRH.Value(tempFloat);
   Arch >> tempInt;     //mHandLocRH.Decimals(tempInt);
   Arch >> tempFloat;   //mHandLocRL.Value(tempFloat);
   Arch >> tempInt;     //mHandLocRL.Decimals(tempInt);
   Arch >> tempFloat;   //mHandLocRV.Value(tempFloat);
   Arch >> tempInt;     //mHandLocRV.Decimals(tempInt);

   //--- Joint Angles ---
   JointAngles lCurrentJointAngles;
   Arch >> lCurrentJointAngles;
   // Set current joint angles and clear all undo/redo objects
   undoHistory.clearHistory();
   skeleton->SetAngles(0,lCurrentJointAngles);

   // --- Hand Forces ---
   int rv,rh,lv,lh;
   float rmag,lmag;

   Arch >> lmag;
   Arch >> lh;
   Arch >> lv;
   Arch >> rmag;
   Arch >> rh;
   Arch >> rv;

   skeleton->Put_Force_Magnitudes(rmag,lmag);
   skeleton->Put_Force_Angles(rv,rh,lv,lh);

   // --- Oblique View ---
   CGLObliqueView *oblique = (CGLObliqueView*)GLViews[VIEW_OBLIQUE];
   Arch >> Signed_Int;	// Formerly this->Obl_Model_Type; changed in 4.4.0
   Arch >> Signed_Int; this->Hand_Type = (E_Hand_Type)Signed_Int;
   Arch >> Signed_Int;	// Formerly this->Obl_Render_Type; changed in 4.4.0
   if(Signed_Int == 0)
      // 0 was the old wireframe number
      oblique->setModelType(MT_WIREFRAME);
   else
      // Otherwise, make it flesh
      oblique->setModelType(MT_FLESH);

   // Set hominoid color based on old shade number
   Arch >> Signed_Int;
   ConvertOldShadeNumber(E_Shade(Signed_Int),CGLObliqueView::skinColor);

   Arch >> Signed_Int;// this->Obl_Light_Type 	= int(Signed_Int);
   this->ConvertLightType(Signed_Int,
                          this->Obl_Light_Type_FB,
                          this->Obl_Light_Type_LR,
                          this->Obl_Light_Type_TB);

   Arch >> CGLObliqueView::backgroundColor[0];
   Arch >> CGLObliqueView::backgroundColor[1];
   Arch >> CGLObliqueView::backgroundColor[2];

   // (mpt) film height/width obsolete
   Arch >> Signed_Int; oblique->getCamera().focalLength = int(Signed_Int);
   Arch >> Signed_Int; //this->Cam_Film_Width = int(Signed_Int);
   Arch >> Signed_Int; //this->Cam_Film_Height = int(Signed_Int);
   Arch >> Signed_Int; oblique->getCamera().distance = int(Signed_Int); 
   Arch >> Signed_Int; oblique->getCamera().altitude = int(Signed_Int);
   Arch >> Signed_Int; oblique->getCamera().azimuth = int(Signed_Int);

   //--- Orthographic Views ---
   Arch >> Signed_Int;	// Used to be Ort_Model_Type (pre-440)
   Arch >> Signed_Int;	// Used to be Ort_Render_Type (pre-440)

   // Set ort color based on old shade number (LCW - Added 8/3/04)
   Arch >> Signed_Int;
   ConvertOldShadeNumber(E_Shade(Signed_Int),CGLStickView::skeletonColor);

   Arch >> Signed_Int;  // Used to be Ort_Light_Type (pre-440) 

   Arch >> CGLStickView::backgroundColor[0];
   Arch >> CGLStickView::backgroundColor[1];
   Arch >> CGLStickView::backgroundColor[2];

   //--- Environment ---
   // LCW - Added 8/3/04
   //	Set handheld object color based on old-style "shade" numbers
   Arch >> Signed_Int;
   ConvertOldShadeNumber(E_Shade(Signed_Int),CGLObliqueView::handHeldColor);

   Arch >> Signed_Int; //oblique->setSeatEnabled((bool)Signed_Int);
   Arch >> Signed_Int; oblique->setFloorEnabled((bool)Signed_Int);

   Arch >> Signed_Int; oblique->getHandHeld().type = int(Signed_Int); 
   Arch >> Signed_Int; oblique->getHandHeld().depth = int(Signed_Int);
   Arch >> Signed_Int; oblique->getHandHeld().height = int(Signed_Int);

   Arch >> Signed_Int; this->Bar_Type        = (E_Barrier_Type)Signed_Int;
   Arch >> Signed_Int; this->Bar_Width       = int(Signed_Int);
   Arch >> Signed_Int; this->Bar_Length      = int(Signed_Int);
   Arch >> Signed_Int; this->Bar_Thickness   = int(Signed_Int);
   Arch >> Signed_Int; this->Bar_Distance    = int(Signed_Int);
   Arch >> Signed_Int; this->Bar_Elevation   = int(Signed_Int);
   Arch >> Signed_Int; this->Bar_Azimuth     = int(Signed_Int);


   //--- AutoCAD ------------------------------
   // (mpt) obsolete, functional code now commented out
   Arch >> Signed_Int; //this->Hom_AC_Refresh_Mode            = int(Signed_Int);
   Arch >> Signed_Int; //this->Hom_AC_Units_Mode              = int(Signed_Int);
   Arch >> Signed_Int; //this->Hom_AC_Position_Source_Mode    = int(Signed_Int);
   Arch >> Signed_Int; //this->Hom_AC_Positioned_Frame_Mode   = int(Signed_Int);
   Arch >> Signed_Int; //this->AC_Active_Zone                 = int(Signed_Int);

   //--- Position data
   Arch >> tempFloat; //this->Hom_AC_Tra_X = tempFloat;
   Arch >> tempFloat; //this->Hom_AC_Tra_Y = tempFloat;
   Arch >> tempFloat; //this->Hom_AC_Tra_Z = tempFloat;

   Arch >> Signed_Int; //this->Hom_AC_Rot_X = int(Signed_Int);
   Arch >> Signed_Int; //this->Hom_AC_Rot_Y = int(Signed_Int);
   Arch >> Signed_Int; //this->Hom_AC_Rot_Z = int(Signed_Int);

   //--- Zone data
   CString dummyCStr;
   for( int ZN = 0; ZN < 6; ZN++ ) // Number_Of_Zones
   {
      Arch >> dummyCStr; //this->Zone_Names[ ZN ];
      Arch >> tempFloat; //this->Zone_TX[ ZN ] = tempFloat;
      Arch >> tempFloat; //this->Zone_TY[ ZN ] = tempFloat;
      Arch >> tempFloat; //this->Zone_TZ[ ZN ] = tempFloat;

      Arch >> Signed_Int; //this->Zone_RX[ ZN ] = int(Signed_Int);
      Arch >> Signed_Int; //this->Zone_RY[ ZN ] = int(Signed_Int);
      Arch >> Signed_Int; //this->Zone_RZ[ ZN ] = int(Signed_Int);
   }

   //--- Saved UCS Name Strings -> (mpt) obsolete, just load into dummy
   CString dummy;
   Arch >> dummy; //this->UCS_Foot_Left_Value;
   Arch >> dummy; //this->UCS_Foot_Right_Value;
   Arch >> dummy; //this->UCS_Hand_Left_Value;
   Arch >> dummy; //this->UCS_Hand_Right_Value;
   Arch >> dummy; //this->UCS_World_Value;
}
*/