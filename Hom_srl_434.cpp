// Not used!
/*
#include "stdafx.h"
#include "AppVersion.hpp"
#include "hom.h"
#include "hom_doc.h"

// ##########################################################################
//	 ATTENTION	 ATTENTION	 ATTENTION	 ATTENTION	 ATTENTION	 ATTENTION
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	If ANY changes are made to this, use compiler defines to make sure the
//	versions remain pure
// 
// 
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	 ATTENTION	 ATTENTION	 ATTENTION	 ATTENTION	 ATTENTION	 ATTENTION
// ##########################################################################

//---------------------------------------------------------------------------


void C_Hom_Doc::Serial_Retrieve_v440_a ( CArchive &aArch )
{
	this->Serial_Retrieve_v440_b( aArch );
	
	// ### Additional 32-2 members
	aArch >> Get_App_Ptr()->mFactors;
	
	aArch >> mAutoTile;
	aArch >> mStatusBar;
	
	LONG	Signed_Int;
	aArch >> Signed_Int;
	m2DMode = (BOOL) Signed_Int;
	
	aArch >> this->mFrontWindow;
	aArch >> this->mSideWindow;
	aArch >> this->mTopWindow;
	
	aArch >> this->mCOPWind;
	aArch >> this->mSeatCOPWind;
	aArch >> this->mObliqueWindow;
	aArch >> this->mStatusResultsWindow;
	
	// ACT::ADDED [05/03/01]- Special Seating Conditions
	aArch >> this->mDeltaX;
	aArch >> this->mDeltaY;
	aArch >> this->mPelvicTilt;
	
	aArch  >> this->gravity;	// ACT - For BOEING
	
	
	aArch >> this->mReportAnalysisSummary;	 
	aArch >> this->mReportAnthropometry;
	aArch >> this->mReportBalanceStanding;
	aArch >> this->mReportBalanceSeated;
	aArch >> this->mReportPosture;
	
	aArch >> this->mReportJoints;	 
	aArch >> this->mReportJointMoments; 
	aArch >> this->mReportJointForces;
	
	aArch >> this->mReportLowBack;	 
	aArch >> this->mReportSagital;	 
	aArch >> this->mReportSpinal;	 
	aArch >> this->mReportStrength;	 
	aArch >> this->mReportStrengthDirVectors; 
	aArch >> this->mReportTaskInputSummary; 
	
	aArch >> this->Ort_Light_Type_FB;
	aArch >> this->Ort_Light_Type_LR;
	aArch >> this->Ort_Light_Type_TB;
	
	aArch >> this->Obl_Light_Type_FB;
	aArch >> this->Obl_Light_Type_LR;
	aArch >> this->Obl_Light_Type_TB;
	
	return;
}


// ------------------------------- void	  //------------------------------
C_Hom_Doc::Serial_Retrieve_v440_b ( CArchive& Arch )
{
	LONG	Signed_Int;
	
	
	
	// Task Description
	Arch >> mDescription;
	
	// ### Anthropometry Settings ###
	Arch >> mAnthropometry;
	
	// ### Posture Prediction ###
	Arch >> Signed_Int; this->Locking_Mode	  = (E_Locking_Mode)Signed_Int;
	Arch >> Signed_Int; this->Hand_Orientation = (E_Hand_Orientation)Signed_Int;
	Arch >> Signed_Int; this->Manipulate_Mode  = (int)Signed_Int;
	
	Arch >> mHandLocLH;
	Arch >> mHandLocLL;
	Arch >> mHandLocLV;									
	Arch >> mHandLocRH;
	Arch >> mHandLocRL;
	Arch >> mHandLocRV;									
	
	// ### Joint Angles ###
	Arch >> mCurrentJointAngles;
	
	// ### Hand Forces ###
	Arch >> mHandLoads;								  
	
	// ### Oblique View ###
	Arch >> Signed_Int; this->Obl_Model_Type 	= (E_Model_Type)Signed_Int; 
	Arch >> Signed_Int; this->Hand_Type			= (E_Hand_Type)Signed_Int; 
	Arch >> Signed_Int; this->Obl_Render_Type	= (E_Render_Type)Signed_Int; 
	Arch >> Signed_Int; this->Obl_Shade_Number	= (int)Signed_Int; 
	
	Arch >> Signed_Int; this->Obl_Light_Type 	= (int)Signed_Int;
	this->ConvertLightType
		(
		this->Obl_Light_Type ,
		this->Obl_Light_Type_FB , 
		this->Obl_Light_Type_LR , 
		this->Obl_Light_Type_TB
		);
	
	Arch >> this->Obl_BG_Red;
	Arch >> this->Obl_BG_Green;
	Arch >> this->Obl_BG_Blue;
	
	Arch >> this->Report_BG_Red;
	Arch >> this->Report_BG_Green;
	Arch >> this->Report_BG_Blue;
	
	Arch >> Signed_Int; this->Cam_Focal_Length  = (int)Signed_Int;
	Arch >> Signed_Int; this->Cam_Film_Width    = (int)Signed_Int;
	Arch >> Signed_Int; this->Cam_Film_Height   = (int)Signed_Int;
	Arch >> Signed_Int; this->Cam_Distance	   = (int)Signed_Int; 
	Arch >> Signed_Int; this->Cam_Altitude	   = (int)Signed_Int;
	Arch >> Signed_Int; this->Cam_Azimuth	   = (int)Signed_Int;
	//Arch >> Signed_Int; this->Cam_Horizontal_Offset = (int)Signed_Int;
	Arch >> Signed_Int; this->Cam_Horizontal_Offset_float = (float)Signed_Int;
	//Arch >> Signed_Int; this->Cam_Vertical_Offset = (int)Signed_Int;
	Arch >> Signed_Int; this->Cam_Vertical_Offset_float = (float)Signed_Int;
	
	//--- Orthographic Views ---
	Arch >> Signed_Int; this->Ort_Model_Type   = (E_Model_Type)Signed_Int;
	Arch >> Signed_Int; this->Ort_Render_Type  = (E_Render_Type)Signed_Int;
	Arch >> Signed_Int; this->Ort_Shade_Number = (int)Signed_Int;
	
	
	Arch >> Signed_Int; this->Ort_Light_Type   = (int)Signed_Int; 
	this->ConvertLightType
		(
		this->Ort_Light_Type ,
		this->Ort_Light_Type_FB , 
		this->Ort_Light_Type_LR , 
		this->Ort_Light_Type_TB
		);
	
	Arch >> this->Ort_BG_Red;
	Arch >> this->Ort_BG_Green;
	Arch >> this->Ort_BG_Blue;
	
	//--- Environment ---
	Arch >> Signed_Int; this->Env_Shade_Number = (int)Signed_Int;
	
	Arch >> Signed_Int; this->Sea_Visible  = (int)Signed_Int;
	Arch >> Signed_Int; this->Flo_Visible  = (int)Signed_Int;
	
	Arch >> Signed_Int; this->HHO_Type	  = (int)Signed_Int; 
	Arch >> Signed_Int; this->HHO_Width	  = (int)Signed_Int;
	Arch >> Signed_Int; this->HHO_Height   = (int)Signed_Int;
	
	Arch >> Signed_Int; this->Bar_Type		 = (E_Barrier_Type)Signed_Int; 
	Arch >> Signed_Int; this->Bar_Width		 = (int)Signed_Int; 
	Arch >> Signed_Int; this->Bar_Length 	 = (int)Signed_Int; 
	Arch >> Signed_Int; this->Bar_Thickness	 = (int)Signed_Int; 
	Arch >> Signed_Int; this->Bar_Distance	 = (int)Signed_Int; 
	Arch >> Signed_Int; this->Bar_Elevation	 = (int)Signed_Int; 
	Arch >> Signed_Int; this->Bar_Azimuth	 = (int)Signed_Int; 
	
	
	// ### AutoCAD ##############################
	// ### Dialog states
	Arch >> Signed_Int; this->Hom_AC_Refresh_Mode		  = (int)Signed_Int;
	Arch >> Signed_Int; this->Hom_AC_Units_Mode			  = (int)Signed_Int;
	Arch >> Signed_Int; this->Hom_AC_Position_Source_Mode  = (int)Signed_Int;
	Arch >> Signed_Int; this->Hom_AC_Positioned_Frame_Mode = (int)Signed_Int;
	Arch >> Signed_Int; this->AC_Active_Zone 			  = (int)Signed_Int;
	
	//### Position data
	Arch >> this->Hom_AC_Tra_X;
	Arch >> this->Hom_AC_Tra_Y;
	Arch >> this->Hom_AC_Tra_Z;
	Arch >> Signed_Int; this->Hom_AC_Rot_X = (int)Signed_Int;
	Arch >> Signed_Int; this->Hom_AC_Rot_Y = (int)Signed_Int;
	Arch >> Signed_Int; this->Hom_AC_Rot_Z = (int)Signed_Int;
	
	//### Zone data
	for( int ZN = 0; ZN < Number_Of_Zones; ZN++ )
	{
		Arch >> this->Zone_Names[ ZN ];
		Arch >> this->Zone_TX[ ZN ];
		Arch >> this->Zone_TY[ ZN ];
		Arch >> this->Zone_TZ[ ZN ];
		Arch >> Signed_Int; this->Zone_RX[ ZN ] = (int)Signed_Int;
		Arch >> Signed_Int; this->Zone_RY[ ZN ] = (int)Signed_Int;
		Arch >> Signed_Int; this->Zone_RZ[ ZN ] = (int)Signed_Int;
	}
	
	//### Saved UCS Name Strings
	Arch >> this->UCS_Foot_Left_Value;
	Arch >> this->UCS_Foot_Right_Value;
	Arch >> this->UCS_Hand_Left_Value;
	Arch >> this->UCS_Hand_Right_Value;
	Arch >> this->UCS_World_Value;
	
	
	
	return;
}


#define SIGNED_INT LONG


void C_Hom_Doc::Serial_Store_v440( CArchive& Arch )
{
	
	this->SaveWindowPositions();
	
	Arch << this->DocumentSchemaVersion;
	int major_version, minor_version;
	major_version = VERSION_NUMBER_MAJOR;
	minor_version = VERSION_NUMBER_MINOR;
	Arch << major_version << minor_version;
	
	
	//Task related variables
	Arch << mDescription; 
	//--- Anthropometry ---
	Arch << mAnthropometry;
	
	//--- Posture Prediction ---
	Arch << (SIGNED_INT) this->Locking_Mode;
	Arch << (SIGNED_INT) this->Hand_Orientation;
	Arch << (SIGNED_INT) this->Manipulate_Mode;
	
	Arch << mHandLocLH;
	Arch << mHandLocLL;
	Arch << mHandLocLV;								   
	Arch << mHandLocRH;
	Arch << mHandLocRL;
	Arch << mHandLocRV;								   
	
	//--- Joint Angles ---
	Arch << mCurrentJointAngles;
	
	//--- Hand Forces ---
	Arch << mHandLoads;
	
	//--- Oblique View ---
	Arch << (SIGNED_INT) this->Obl_Model_Type; 
	Arch << (SIGNED_INT) this->Hand_Type; 
	Arch << (SIGNED_INT) this->Obl_Render_Type; 
	Arch << (SIGNED_INT) this->Obl_Shade_Number; 
	Arch << (SIGNED_INT) this->Obl_Light_Type; 
	Arch << this->Obl_BG_Red;
	Arch << this->Obl_BG_Green;
	Arch << this->Obl_BG_Blue;
	
	Arch << this->Report_BG_Red;
	Arch << this->Report_BG_Green;
	Arch << this->Report_BG_Blue;
	
	Arch << (SIGNED_INT) this->Cam_Focal_Length;
	Arch << (SIGNED_INT) this->Cam_Film_Width;
	Arch << (SIGNED_INT) this->Cam_Film_Height;
	Arch << (SIGNED_INT) this->Cam_Distance; 
	Arch << (SIGNED_INT) this->Cam_Altitude;
	Arch << (SIGNED_INT) this->Cam_Azimuth;
	//Arch << (SIGNED_INT) this->Cam_Horizontal_Offset;
	Arch << (SIGNED_INT) this->Cam_Horizontal_Offset_float;
	//Arch << (SIGNED_INT) this->Cam_Vertical_Offset;
	Arch << (SIGNED_INT) this->Cam_Vertical_Offset_float;
	
	//--- Orthographic Views ---
	Arch << (SIGNED_INT) this->Ort_Model_Type;
	Arch << (SIGNED_INT) this->Ort_Render_Type;
	Arch << (SIGNED_INT) this->Ort_Shade_Number; 
	Arch << (SIGNED_INT) this->Ort_Light_Type;
	Arch << this->Ort_BG_Red;
	Arch << this->Ort_BG_Green;
	Arch << this->Ort_BG_Blue;
	
	//--- Environment ---
	Arch << (SIGNED_INT) this->Env_Shade_Number;
	
	Arch << (SIGNED_INT) this->Sea_Visible;
	Arch << (SIGNED_INT) this->Flo_Visible;
	
	Arch << (SIGNED_INT) this->HHO_Type; 
	Arch << (SIGNED_INT) this->HHO_Width;
	Arch << (SIGNED_INT) this->HHO_Height;
	// this->HHO_Length ... //Computed  
	
	Arch << (SIGNED_INT) this->Bar_Type; 
	Arch << (SIGNED_INT) this->Bar_Width; 
	Arch << (SIGNED_INT) this->Bar_Length; 
	Arch << (SIGNED_INT) this->Bar_Thickness; 
	Arch << (SIGNED_INT) this->Bar_Distance; 
	Arch << (SIGNED_INT) this->Bar_Elevation; 
	Arch << (SIGNED_INT) this->Bar_Azimuth; 
	
	
	//--- AutoCAD ------------------------------
	//--- Dialog states
	Arch << (SIGNED_INT) this->Hom_AC_Refresh_Mode;
	Arch << (SIGNED_INT) this->Hom_AC_Units_Mode;
	Arch << (SIGNED_INT) this->Hom_AC_Position_Source_Mode;
	Arch << (SIGNED_INT) this->Hom_AC_Positioned_Frame_Mode;
	Arch << (SIGNED_INT) this->AC_Active_Zone;
	
	//--- Position data
	Arch << this->Hom_AC_Tra_X;
	Arch << this->Hom_AC_Tra_Y;
	Arch << this->Hom_AC_Tra_Z;
	Arch << (SIGNED_INT) this->Hom_AC_Rot_X;
	Arch << (SIGNED_INT) this->Hom_AC_Rot_Y;
	Arch << (SIGNED_INT) this->Hom_AC_Rot_Z;
	
	//--- Zone data
	for( int ZN = 0; ZN < Number_Of_Zones; ZN++ )
	{
		Arch << this->Zone_Names[ ZN ];
		Arch << this->Zone_TX[ ZN ];
		Arch << this->Zone_TY[ ZN ];
		Arch << this->Zone_TZ[ ZN ];
		Arch << (SIGNED_INT) this->Zone_RX[ ZN ];
		Arch << (SIGNED_INT) this->Zone_RY[ ZN ];
		Arch << (SIGNED_INT) this->Zone_RZ[ ZN ]; 	 
	}									 
	//--- Saved UCS Name Strings
	Arch << this->UCS_Foot_Left_Value;
	Arch << this->UCS_Foot_Right_Value;
	Arch << this->UCS_Hand_Left_Value;
	Arch << this->UCS_Hand_Right_Value;
	Arch << this->UCS_World_Value;
	
	//--- Additional 32-2 members
	Arch << Get_App_Ptr()->mFactors;
	
	Arch << mAutoTile;
	Arch << mStatusBar;
	
	Arch << (SIGNED_INT) m2DMode;
	
	Arch << this->mFrontWindow;
	Arch << this->mSideWindow;
	Arch << this->mTopWindow;
	
	Arch << this->mCOPWind;
	Arch << this->mSeatCOPWind;
	Arch << this->mObliqueWindow;
	Arch << this->mStatusResultsWindow;
	
	
	// ACT::ADDED [05/03/01]- Special Seating Conditions
	Arch << this->mDeltaX;
	Arch << this->mDeltaY;
	Arch << this->mPelvicTilt;
	
	Arch   << this->gravity;	// ACT - For BOEING
	
	Arch << this->mReportAnalysisSummary;   
	Arch << this->mReportAnthropometry;
	Arch << this->mReportBalanceStanding;
	Arch << this->mReportBalanceSeated;
	Arch << this->mReportPosture;
	Arch << this->mReportJoints;   
	Arch << this->mReportJointMoments; 
	Arch << this->mReportJointForces;
	
	Arch << this->mReportLowBack;   
	Arch << this->mReportSagital;   
	Arch << this->mReportSpinal;   
	Arch << this->mReportStrength;   
	Arch << this->mReportStrengthDirVectors; 
	Arch << this->mReportTaskInputSummary;
	
	Arch << this->Ort_Light_Type_FB;
	Arch << this->Ort_Light_Type_LR;
	Arch << this->Ort_Light_Type_TB;
	
	Arch << this->Obl_Light_Type_FB;
	Arch << this->Obl_Light_Type_LR;
	Arch << this->Obl_Light_Type_TB;
	
	
	return;
}	

*/