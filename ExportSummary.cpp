#include "stdafx.h"
#include "ExportSummary.hpp"
#include "hom.h"
#include "hom_doc.h"
//#include "units.hpp"
#include "Types.h"
#include "Skeleton.hpp"
#include "JointAngles.hpp"
#include "HandLoads.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
//to get the selected frame number for exportsummary
#include "AnimationBar.h"

using std::fstream;

//extern double gPelvicAngle;

ExportSummary::ExportSummary(C_Hom_Doc *aDocumentPtr) : mDocPtr(aDocumentPtr), mFileStreamPtr(0), mFileMode(FM_APPEND), 
                                                        mOverwrite(false), mDelimiterMode(DM_COMMA), mDelimiter( ',' ), 
                                                        mBodySegmentAngles(true), mHandForces(true), mJointLocations(true), 
                                                        mJointForces(true), mJointMoments(true), mShoulderAnalysis(true), mShoulderGeometry(true), mPostureAngles(true), 
                                                        mStrength(true), mSummaryResults(true), mLowBack(true), mFatigueReport(true), mBalanceReport(true),
														mColumnHeaders(false), newLine(true)
{
   return;
}

void ExportSummary::Update(void)
{
    //Set delimiting character
    if( mDelimiterMode == (int)DM_COMMA)
        mDelimiter = ',';
    else
        mDelimiter =  char(9); //set to TAB  

    //Set overwrite logical
    if( mFileMode == (int)FM_OVERWRITE)
        mOverwrite = TRUE; 
    else                  
        mOverwrite =  FALSE;
       
    return;
}




void ExportSummary::Export(fstream &aOutputFile, E_Delimiter_Mode aDelimiterMode, E_File_Mode aFileMode)
{
    E_Delimiter_Mode lDelimiterModeSave = (E_Delimiter_Mode)mDelimiterMode;
    E_File_Mode lFileModeSave = (E_File_Mode)mFileMode;

	
	this->Update();

    Export(aOutputFile);

    mDelimiterMode = lDelimiterModeSave;
    mFileMode = lFileModeSave;
       
    this->Update();

    return;
}
BOOL ExportSummary::Export(fstream &aOutputFile, int frameid)
{

	// Set precision
	aOutputFile << std::setprecision(2) << std::fixed;
	
    //Assign this file stream pointer to support inline writes
    mFileStreamPtr = &aOutputFile;
	
	//Commonly used references
    //Analysis &lResults = mDocPtr->getResultsRef();
	Skeleton* skel = mDocPtr->getSkeletonAtFrame(frameid);

    //AnalysisSummaryData analysis_summary;
	//lResults.getAnalysisSummary(&analysis_summary);
	AnalysisSummaryData_s analysis_summary = skel->getAnalysisSummary();

    //StrengthCapabilityData strength;
	//lResults.getStrengthCapability(&strength);
	StrengthData_s strength = skel->getStrengthData();

    //JointForceData jointForces;
    //lResults.getJointForces(&jointForces);
	Vector3 jointForces[NUM_JOINTS];
	skel->getNetForces(jointForces);

    //JointMomentData jointMoments;
    //lResults.getJointMoments(&jointMoments);
	Vector3 jointMoments[NUM_JOINTS];
	skel->getNetMoments(jointMoments);

    //LowBack3DData lowback3D;
    //lResults.getLowBack3D(&lowback3D);
	LowBack3DData_s lowback3D = skel->getLowBack3DData();

    //LowBackSagittalData lowbackSag;
    //lResults.getLowBackSagittal(&lowbackSag);
	LowBackSagittalData_s lowbackSag = skel->getLowBackSagittalData();

    //BalanceData balance;
    //lResults.getBalanceData(&balance);
	BalanceData_s balance = skel->getBalanceData();

    //BalanceMomentData balance_moments;
    //lResults.getBalanceMoments(&balance_moments);

    //SpinalForceData spinal_forces;
    //lResults.getSpinalForces(&spinal_forces);
	SpinalForces_s spinal_forces = skel->getSpinalForces();

    //SpinalMomentData spinal_moments;
    //lResults.getSpinalMoments(&spinal_moments);
	SpinalMoments_s spinal_moments = skel->getSpinalMoments();

    //StrengthVectorData strength_vectors;
    //lResults.getStrengthVectors(&strength_vectors);
	Vector3 strength_vectors[NUM_STRENGTH_HINGES];
	skel->getStrengthDirectionVectors(strength_vectors);

    //FatigueData fatigue;
    //lResults.getFatigue(&fatigue);
	FatigueData_s fatigue = skel->getFatigueData();

	//ShoulderGeometryData shoulder_geometry;
	ShoulderGeometryData_s shoulder_geometry = skel->getShoulderGeometryData();

	//ShoulderAnalysisData shoulder_analysis;
	ShoulderAnalysisData_s shoulder_analysis = skel->getShoulderAnalysisData();

	//MuscleForceModelData muscle_force;
	MuscleForceModelData_s muscle_force = skel->getMuscleForceModelData();

	double extraAngles[NUM_EXTRA_ANGLES];
	skel->getExtraAngles(extraAngles);

	int lSex = mDocPtr->getAnthropometryRef().Sex();//lResults.sex; //male = 1, female = 2
	JointAngles docJointAngles = skel->GetAngles();
    //mDocPtr->GetSkeleton().GetAngles(mDocPtr->GetSkeleton().getCurrentFrame(),docJointAngles);

	const int HEADERS = 0;
	const int CONTENT = 1;
	for(int pass = HEADERS; pass <= CONTENT; pass++) {
		// skip headers
		if(!mColumnHeaders && pass == HEADERS) continue;

		//--- Write Standard Information ---
		if(pass == HEADERS) {
			Write("Analyst");
			Write("Company Name");
			Write("Units");
			Write("Task Name");
			Write("Gender");
			Write("Height");
			Write("Weight");
		} else {

			Write(mDocPtr->getDescriptionRef().Analyst());
			Write(mDocPtr->getDescriptionRef().CompanyRef());
			Write(mDocPtr->getDescriptionRef().UnitsValueText());
			Write(mDocPtr->getDescriptionRef().Task());
			Write(mDocPtr->Gender());
			Write(mDocPtr->getAnthropometryRef().Height());
			Write(mDocPtr->getAnthropometryRef().Weight());
		}


		//--- Write Summary Results ---

		if(mSummaryResults) {
			if(pass == HEADERS) {
				Write("Summary Start");
				Write("L5/S1 Compression");
				Write("L4/L5 Compression");
				Write("Minimum Wrist Percentile");
				Write("Minimum Elbow Percentile");
				Write("Minimum Shoulder Percentile");
				Write("Minimum Torso Percentile");
				Write("Minimum Neck Percentile");
				Write("Minimum Hip Percentile");
				Write("Minimum Knee Percentile");
				Write("Minimum Ankle Percentile");
				Write("Coefficient of Friction");
				Write("Left Load Fraction");
				Write("Balance Status");
			} else {

				Write("summary");

				AnalysisSummaryData_s summary = skel->getAnalysisSummary();
		  	 
				/* double conversion = U_M_CM;
				if (mDocPtr->Is_English())
					conversion = U_NM_INLB; */

				Write(summary.L5S1Compression); //L5/S1
				Write(summary.L4L5Compression); //L4/L5

				Write(summary.WristMinPercentile);
				Write(summary.ElbowMinPercentile); 
				Write(summary.ShoulderMinPercentile);
				Write(summary.TorsoMinPercentile);
				Write(summary.NeckMinPercentile); 
				Write(summary.HipMinPercentile);
				Write(summary.KneeMinPercentile);
				Write(summary.AnkleMinPercentile);

				Write(summary.frcoef);
				Write(balance.LeftPctLoad);
				Write(balance.BalanceStatus);
			}
		}

		//--- Write Strength Capabilities ---

		if(mStrength)
		{
			StrengthHinge_e strength_hinge_order[NUM_STRENGTH_HINGES] = {
				SH_RWristFlexion, SH_RWristDeviation, SH_RForearmRotation,
				SH_RElbowFlexion, SH_RHumeralRotation, SH_RShoulderRotation, SH_RShoulderAbduction,
				SH_RHipFlexion, SH_RKneeFlexion, SH_RAnkleFlexion,
				SH_LWristFlexion, SH_LWristDeviation, SH_LForearmRotation,
				SH_LElbowFlexion, SH_LHumeralRotation, SH_LShoulderRotation, SH_LShoulderAbduction,
				SH_LHipFlexion, SH_LKneeFlexion, SH_LAnkleFlexion,
				SH_TorsoFlexion, SH_TorsoLateralBending, SH_TorsoRotation,
				SH_NeckFlexion, SH_NeckLateralBending, SH_NeckRotation
			};

			if(pass == HEADERS) {
				Write("Strength Capability Start");
				// TODO store this better somewhere. Types.h?
				Write("Right Wrist Flexion");
				Write("Right Wrist Deviation");
				Write("Right Forearm Rotation");
				Write("Right Elbow Flexion");
				Write("Right Humeral Rotation");
				Write("Right Shoulder Rotation");
				Write("Right Shoulder Abduction");
				Write("Right Hip Flexion");
				Write("Right Knee Flexion");
				Write("Right Ankle Flexion");

				Write("Left Wrist Flexion");
				Write("Left Wrist Deviation");
				Write("Left Forearm Rotation");
				Write("Left Elbow Flexion");
				Write("Left Humeral Rotation");
				Write("Left Shoulder Rotation");
				Write("Left Shoulder Abduction");
				Write("Left Hip Flexion");
				Write("Left Knee Flexion");
				Write("Left Ankle Flexion");

				Write("Torso Flexion");
				Write("Torso Lateral Bending");
				Write("Torso Rotation");
				Write("Neck Flexion");
				Write("Neck Lateral Bending");
				Write("Neck Rotation");

			} else {

				Write("str cap");

				for(int i = 0; i < NUM_STRENGTH_HINGES; i++) {
					//Write(lResults.zprob[lSex][i]);
					Write(strength.Percentile[strength_hinge_order[i]]);
				}
			}
		}

		//--- Write Low Back ---

		if(mLowBack) {
			if(pass == HEADERS) {
				Write("Low Back Start");
				Write("L5/S1 Compression");
				Write("L5/S1 Compression SD");
				Write("Sagittal Shear");
				Write("Frontal Shear");

				Write("Total L4/L5 Compression");
				Write("Anterior L4/L5 Shear");
				Write("Lateral L4/L5 Shear");

				Write("Right Erector Force Magnitude");
				Write("Right Erector Shear");
				Write("Right Erector Force X"); Write("Right Erector Force Y"); Write("Right Erector Force Z");
				Write("Right Rectus Force Magnitude");
				Write("Right Rectus Shear");
				Write("Right Rectus Force X"); Write("Right Rectus Force Y"); Write("Right Rectus Force Z");
				Write("Right Internal Force Magnitude");
				Write("Right Internal Shear");
				Write("Right Internal Force X"); Write("Right Internal Force Y"); Write("Right Internal Force Z");
				Write("Right External Force Magnitude");
				Write("Right External Shear");
				Write("Right External Force X"); Write("Right External Force Y"); Write("Right External Force Z");
				Write("Right Latissimus Force Magnitude");
				Write("Right Latissimus Shear");
				Write("Right Latissimus Force X"); Write("Right Latissimus Force Y"); Write("Right Latissimus Force Z");
				Write("Left Erector Force Magnitude");
				Write("Left Erector Shear");
				Write("Left Erector Force X"); Write("Left Erector Force Y"); Write("Left Erector Force Z");
				Write("Left Rectus Force Magnitude");
				Write("Left Rectus Shear");
				Write("Left Rectus Force X"); Write("Left Rectus Force Y"); Write("Left Rectus Force Z");
				Write("Left Internal Force Magnitude");
				Write("Left Internal Shear");
				Write("Left Internal Force X"); Write("Left Internal Force Y"); Write("Left Internal Force Z");
				Write("Left External Force Magnitude");
				Write("Left External Shear");
				Write("Left External Force X"); Write("Left External Force Y"); Write("Left External Force Z");
				Write("Left Latissimus Force Magnitude");
				Write("Left Latissimus Shear");
				Write("Left Latissimus Force X"); Write("Left Latissimus Force Y"); Write("Left Latissimus Force Z");
			} else {

				Write("low back");
				
				AnalysisSummaryData_s summary = skel->getAnalysisSummary();
		  	 
				Write(summary.L5S1Compression);
				Write(summary.L5S1CompressionSD);
				Write(lowbackSag.shrSagittal);
				Write(lowbackSag.shrFrontal);

				Write(lowback3D.totalL4L5Compression);
				Write(lowback3D.anteriorL4L5Shear);
				Write(lowback3D.lateralL4L5Shear);

				int muscleOrder[10] = {BM_RightErector,
							BM_RightRectus,
							BM_RightInternal,
							BM_RightExternal,
							BM_RightLatis,
							BM_LeftErector,
							BM_LeftRectus, 
							BM_LeftInternal, 
							BM_LeftExternal, 
							BM_LeftLatis};

				int num_back3D = 10;
				for (int i = 0; i < num_back3D; ++i)
				{
					Write(lowback3D.Resultant[muscleOrder[i]]);
					Write(lowback3D.Shear[muscleOrder[i]]);
					Write(lowback3D.XForce[muscleOrder[i]]);
					Write(lowback3D.YForce[muscleOrder[i]]);
					Write(lowback3D.ZForce[muscleOrder[i]]);
				}
			}
		} //end if(mLowBack)


		//--- Write Fatigue Data ---

		if (mFatigueReport) {
			if(pass == HEADERS) {
				Write("Fatigue Start");
				// TODO store this better somewhere. Types.h?
				const char* hinge_name_order[NUM_STRENGTH_HINGES] = {
					"Right Wrist Flx/Ext", "Right Wrist Deviation",
					"Right Forearm Rotation", "Right Elbow Flexion",
					"Right Humeral Rotation", "Right Fd/Bk Rotation", "Right Shoulder Abd/Add",
					"Right Hip Flexion", "Right Knee Flexion", "Right Ankle Flexion",

					"Left Wrist Flx/Ext", "Left Wrist Deviation",
					"Left Forearm Rotation", "Left Elbow Flexion",
					"Left Humeral Rotation", "Left Fd/Bk Rotation", "Left Shoulder Abd/Add",
					"Left Hip Flexion", "Left Knee Flexion", "Left Ankle Flexion",
				
					"Torso Flexion", "Torso Lateral Bending", "Torso Rotation"
					"Neck Flexion", "Neck Lateral Bending", "Neck Rotation"
				};
				
				std::string base;
				for(int i = 0; i < NUM_STRENGTH_HINGES; i++) {
					base = hinge_name_order[i];
					Write((base + " 5%ile" + " %MVC").c_str());
					Write((base + " 5%ile" + " %DC").c_str());
					Write((base + " 5%ile" + " Dur(s)").c_str());
					Write((base + " 25%ile" + " %MVC").c_str());
					Write((base + " 25%ile" + " %DC").c_str());
					Write((base + " 25%ile" + " Dur(s)").c_str());
					Write((base + " 50%ile" + " %MVC").c_str());
					Write((base + " 50%ile" + " %DC").c_str());
					Write((base + " 50%ile" + " Dur(s)").c_str());
				}
			} else {

				Write("fatigue");

				StrengthHinge_e hingeOrder[NUM_STRENGTH_HINGES] = {
							SH_RWristFlexion, SH_RWristDeviation,
							SH_RForearmRotation,	SH_RElbowFlexion,
							SH_RHumeralRotation, SH_RShoulderRotation, SH_RShoulderAbduction,
							SH_RHipFlexion, SH_RKneeFlexion, SH_RAnkleFlexion,
							SH_LWristFlexion, SH_LWristDeviation,
							SH_LForearmRotation,	SH_LElbowFlexion,
							SH_LHumeralRotation, SH_LShoulderRotation, SH_LShoulderAbduction,
							SH_LHipFlexion, SH_LKneeFlexion, SH_LAnkleFlexion,
							SH_TorsoFlexion, SH_TorsoLateralBending, SH_TorsoRotation,
				SH_NeckFlexion, SH_NeckLateralBending, SH_NeckRotation};
		  	 
				for(int i = 0; i < NUM_STRENGTH_HINGES; i++) {
					Write(floor(fatigue.FifthPercentile[hingeOrder[i]] + 0.5));
					Write(floor(fatigue.FifthPercentileDC[hingeOrder[i]] + 0.5));
					Write(floor(fatigue.FifthPercentileMT[hingeOrder[i]] + 0.5));

					Write(floor(fatigue.FiftiethPercentile[hingeOrder[i]] + 0.5));
					Write(floor(fatigue.FiftiethPercentileDC[hingeOrder[i]] + 0.5));
					Write(floor(fatigue.FiftiethPercentileMT[hingeOrder[i]] + 0.5));

					Write(floor(fatigue.NinetyFifthPercentile[hingeOrder[i]] + 0.5));
					Write(floor(fatigue.NinetyFifthPercentileDC[hingeOrder[i]] + 0.5));
					Write(floor(fatigue.NinetyFifthPercentileMT[hingeOrder[i]] + 0.5));
				}
			}
		}

		//--- Write Balance Data ---

		if(mBalanceReport) {
			if(pass == HEADERS) {
				Write("Balance Start");
				Write("COG X");
				Write("COG Y");
				Write("COP X");
				Write("COP Y");
				Write("Stability");
				Write("Left load");
				Write("Right load");
			} else {
				Write("balance");
				// COM
				Write(balance.CenterOfGravity[0]);
				Write(balance.CenterOfGravity[1]);
				// COP
				Write(balance.CenterOfPressure[0]);
				Write(balance.CenterOfPressure[1]);
				// stability
				if(balance.BalanceStatus == BS_Acceptable) {
					Write("Acceptable");
				} else if(balance.BalanceStatus == BS_Critical) {
					Write("Critical");
				} else {
					Write("Unacceptable");
				}
				// leg loads
				Write(balance.LeftPctLoad);
				Write(balance.RightPctLoad);
			}
		}

		//--- Write Hand Forces ---

		//Skeleton &skel = mDocPtr->GetSkeleton();
		if(mHandForces)
		{
			if(pass == HEADERS) {
				Write("Hand Forces Start");
				Write("Right Force Magnitude");
				Write("Right Vertical Angle");
				Write("Right Horizontal Angle");
				Write("Left Force Magnitude");
				Write("Left Vertical Angle");
				Write("Left Horizontal Angle");
			} else {

				Write("hand forces");

				double rv,rh,lv,lh;

				// skel->GetHandForceAngles(rv,rh,lv,lh);
				Vector3 LHandForce = skel->getExtForce(JT_LHAND);
				Vector3 RHandForce = skel->getExtForce(JT_RHAND);
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

				Write(skel->getExtForce(JT_RHAND).length());
				Write(rv);
				Write(rh);

				Write(skel->getExtForce(JT_LHAND).length());
				Write(lv);
				Write(lh);
			}
		}

		//--- Write Body Segment Angles ---

		if(mBodySegmentAngles) {

			// 39 instead of NUM_ANGLES == 40, because there's that SCJ one we don't use
			const int num_angles = 39;
			E_Angles angle_order[num_angles] = {
				A_HandRV, A_HandRH, A_HandRRot,
				A_ForearmRV, A_ForearmRH,
				A_UpperarmRV, A_UpperarmRH,
				A_ClavicleRV, A_ClavicleRH,
				A_UpperlegRV, A_UpperlegRH,
				A_LowerlegRV, A_LowerlegRH,
				A_FootRV, A_FootRH,

				A_HandLV, A_HandLH, A_HandLRot,
				A_ForearmLV, A_ForearmLH,
				A_UpperarmLV, A_UpperarmLH,
				A_ClavicleLV, A_ClavicleLH,
				A_UpperlegLV, A_UpperlegLH,
				A_LowerlegLV, A_LowerlegLH,
				A_FootLV, A_FootLH,

				A_C4C7LateralBending, A_C4C7Flexion, A_C4C7AxialRotation,
				A_TrunkLateralBending, A_TrunkFlexion, A_TrunkAxialRotation,
				A_PelvisLateralBending, A_PelvisFlexion, A_PelvisAxialRotation
			};

			if(pass == HEADERS) {
				Write("Segment Angles Start");
				for(int i = 0; i < num_angles; i++) {
					Write(angleData[angle_order[i]].name);
				}
			} else {

				Write("seg ang");

				for(int i = 0; i < num_angles; i++) {
					// bug for which we released 6.0.5:
					// this wrote the default angles instead of the current angles
					//Write(angleData[angle_order[i]]);
					Write(docJointAngles[angle_order[i]]);
				}
			}
		}

		//--- Write Posture Angles ---

		if(mPostureAngles)
		{

			const int num_extra_angles1 = 28;
			E_ExtraAngles extra_angle_order1[num_extra_angles1] = {
				EA_HandRFlexion, EA_HandRDeviation, EA_ForearmRRot, EA_ElbowRInc,
				EA_ShoulderRV, EA_ShoulderRH, EA_HumerusRRot,
				EA_HipRInc, EA_HipRV, EA_HipRH,
				EA_FemurRRot, EA_LowerlegRRot, EA_KneeRInc, EA_AnkleRInc,
				
				EA_HandLFlexion, EA_HandLDeviation, EA_ForearmLRot, EA_ElbowLInc,
				EA_ShoulderLV, EA_ShoulderLH, EA_HumerusLRot,
				EA_HipLInc, EA_HipLV, EA_HipLH,
				EA_FemurLRot, EA_LowerlegLRot, EA_KneeLInc, EA_AnkleLInc};

			const int num_angles2 = 7;
			E_Angles angle_order2[num_angles2] = {
				A_C1_Flexion, A_C1C3_Flexion, A_C1C3_AxialRotation, A_C1C3_LateralBending, A_C4C7Flexion, A_C4C7AxialRotation, A_C4C7LateralBending
			};

			const int num_extra_angles3 = 4;
			E_ExtraAngles extra_angle_order3[num_extra_angles3] = {
				EA_TrunkFlexion, EA_AdjustedTrunkAxialRotation, EA_AdjustedTrunkLateralBending,

				EA_PelvisFlexion
			};

			const int num_angles4 = 2;
			E_Angles angle_order4[num_angles4] = {
				A_PelvisAxialRotation, A_PelvisLateralBending
			};

			// Then the last crazy one
			// from ReportPosture
			#define L5S1_ERECT_TILT 50
			double alpha = L5S1_ERECT_TILT - extraAngles[EA_PelvisFlexion];
			double tilt = 90 - alpha;

			if(pass == HEADERS) {
				Write("Posture Angles Start");
				for(int i = 0; i < num_extra_angles1; i++) {
					Write(extraAngleNames[extra_angle_order1[i]]);
				}
				for(int i = 0; i < num_angles2; i++) {
					Write(angleData[angle_order2[i]].name);
				}
				for(int i = 0; i < num_extra_angles3; i++) {
					Write(extraAngleNames[extra_angle_order3[i]]);
				}
				for(int i = 0; i < num_angles4; i++) {
					Write(angleData[angle_order4[i]].name);
				}
				Write("L5S1 Tilt Angle");

			} else {

				Write("post ang");
				
				for(int i = 0; i < num_extra_angles1; i++) {
					Write(extraAngles[extra_angle_order1[i]]);
				}
				for(int i = 0; i < num_angles2; i++) {
					if (i == 0)
						Write(90 - (skel->getExtraAngle(EA_C1_Flexion)+16));
					else if (i == 1)
						Write(90 - (skel->getExtraAngle(EA_C1C3Flexion)));
					else if (i == 4)
						Write(90 - (skel->getExtraAngle(EA_C4C7Flexion)));
					else
						Write(docJointAngles[angle_order2[i]]);
				}
				for(int i = 0; i < num_extra_angles3; i++) {
					Write(extraAngles[extra_angle_order3[i]]);
				}
				for(int i = 0; i < num_angles4; i++) {
					Write(docJointAngles[angle_order4[i]]);
				}
				Write(tilt);
			}

		}

		//--- Write Joint Locations ---

		if(mJointLocations) {
			Skeleton* lSkeleton = mDocPtr->getSkeletonAtFrame(frameid); 
			JointID jointOrder[NUM_JOINTS] = {
				JT_RHAND, JT_RWRIST, JT_RELBOW, JT_RSHOULDER,
				JT_RHIP, JT_RIT, JT_RKNEE, JT_RANKLE, JT_RHEEL, JT_RFOOT_CENTER, JT_RBALL,
				JT_LHAND, JT_LWRIST, JT_LELBOW, JT_LSHOULDER,
				JT_LHIP, JT_LIT, JT_LKNEE, JT_LANKLE, JT_LHEEL, JT_LFOOT_CENTER, JT_LBALL,
				JT_TRAGIONLOC, JT_NASIONLOC, JT_C3C4, JT_C7T1, JT_SCJ, JT_L5S1_TEMP,
				JT_HIP_CENTER, JT_IT_CENTER, JT_ANKLE_CENTER, JT_BALL_CENTER};

			if(pass == HEADERS) {
				Write("Joint Locations Start");
				std::string base;
				std::string header;
				for(int i = 0; i < NUM_JOINTS; i++) {
					base = jointNames[jointOrder[i]];
					header = base + " X";
					Write(header.c_str());
					header = base + " Y";
					Write(header.c_str());
					header = base + " Z";
					Write(header.c_str());
				}
			} else {

				Write("jnt loc");

				Vector3 write_vec;
				for (int i = 0; i < NUM_JOINTS; ++i)
				{
					/*lSkeleton->C4EPoint(locations[i], lLocX, lLocY, lLocZ);
					Write(lLocX.Value()); Write(lLocY.Value()); Write(lLocZ.Value());*/
					write_vec = lSkeleton->C4EPoint(jointOrder[i]);
					Write(write_vec[0]);
					Write(write_vec[1]);
					Write(write_vec[2]);
				}
			}
		} 

		//-- Write Joint Forces ---//

		if (mJointForces)
		{

			JointID jointOrder[NUM_JOINTS] = {
				JT_RHAND, JT_RWRIST, JT_RELBOW, JT_RSHOULDER,
				JT_RHIP, JT_RIT, JT_RKNEE, JT_RANKLE, JT_RHEEL, JT_RFOOT_CENTER, JT_RBALL,
				JT_LHAND, JT_LWRIST, JT_LELBOW, JT_LSHOULDER,
				JT_LHIP, JT_LIT, JT_LKNEE, JT_LANKLE, JT_LHEEL, JT_LFOOT_CENTER, JT_LBALL,
				JT_TRAGIONLOC, JT_NASIONLOC, JT_C3C4, JT_C7T1, JT_SCJ, JT_L5S1_TEMP,
				JT_HIP_CENTER, JT_IT_CENTER, JT_ANKLE_CENTER, JT_BALL_CENTER};

			if(pass == HEADERS) {
				Write("Joint Forces Start");
				std::string base;
				std::string header;
				for(int i = 0; i < NUM_JOINTS; i++) {
					base = jointNames[jointOrder[i]];
					header = base + " X";
					Write(header.c_str());
					header = base + " Y";
					Write(header.c_str());
					header = base + " Z";
					Write(header.c_str());
				}

				Write("Right Forward Seat X");
				Write("Right Forward Seat Y");
				Write("Right Forward Seat Z");

				Write("Left Forward Seat X");
				Write("Left Forward Seat Y");
				Write("Left Forward Seat Z");

				Write("Seat Back X");
				Write("Seat Back Y");
				Write("Seat Back Z");
			} else {

				Write("jnt forces");

				for (int i = 0; i < NUM_JOINTS; ++i) {
					for (int j = 0; j < 3; ++j)
					{
						Write(jointForces[jointOrder[i]][j]);
					}
				}

				if(mDocPtr->getPosition() == P_Sitting) {
					// Forward seat edge
					Vector3 write_vec;
					write_vec = skel->getRightSeatFrontForce();
					Write(write_vec[0]);
					Write(write_vec[1]);
					Write(write_vec[2]);
					write_vec = skel->getLeftSeatFrontForce();
					Write(write_vec[0]);
					Write(write_vec[1]);
					Write(write_vec[2]);
					// Seat back
					write_vec = skel->getSeatBackForce();
					Write(-write_vec[0]);
					Write(-write_vec[1]);
					Write(-write_vec[2]);
				} else {
					// write zeros for standing
					// right forward seat
					Write(0);
					Write(0);
					Write(0);
					// left forward seat
					Write(0);
					Write(0);
					Write(0);
					// seat back
					Write(0);
					Write(0);
					Write(0);
				}
			}
		 }


		//--- Write Joint Moments ---

		// Chris suggested changing this to simply reading the jointForces array (defined earlier) and 
		// writing out the component forces therein. I think I've now done what he wanted. --Sayan.
	    

		if(mJointMoments)
		{
			JointID jointOrder[NUM_JOINTS] = {
				JT_RHAND, JT_RWRIST, JT_RELBOW, JT_RSHOULDER,
				JT_RHIP, JT_RIT, JT_RKNEE, JT_RANKLE, JT_RHEEL, JT_RFOOT_CENTER, JT_RBALL,
				JT_LHAND, JT_LWRIST, JT_LELBOW, JT_LSHOULDER,
				JT_LHIP, JT_LIT, JT_LKNEE, JT_LANKLE, JT_LHEEL, JT_LFOOT_CENTER, JT_LBALL,
				JT_TRAGIONLOC, JT_NASIONLOC, JT_C3C4, JT_C7T1, JT_SCJ, JT_L5S1_TEMP,
				JT_HIP_CENTER, JT_IT_CENTER, JT_ANKLE_CENTER, JT_BALL_CENTER
			};

			if(pass == HEADERS) {
				Write("Joint Moments Start");
				std::string base;
				std::string header;
				for(int i = 0; i < NUM_JOINTS; i++) {
					base = jointNames[jointOrder[i]];
					header = base + " X";
					Write(header.c_str());
					header = base + " Y";
					Write(header.c_str());
					header = base + " Z";
					Write(header.c_str());
				}
			} else {
				Write("jnt mom");
				for (int i = 0; i < NUM_JOINTS; ++i) {
					for (int j = 0; j < 3; ++j) {
						Write(jointMoments[jointOrder[i]][j]);
					}
				}
			}
		}

		//--- Write Shoulder Analysis ---

		if(mShoulderAnalysis)
		{
						int analysis_order[NUM_SHOULDERANALYSIS_HINGES] = {	SA_RightBicepsI, SA_RightBicepsII,
																SA_RightBrachialis, SA_RightBrachioradialis,
																SA_RightTricepsI, SA_RightTricepsII, 
																SA_RightTricepsIII, SA_RightTeresMajor,
																SA_RightSubscapularisI, SA_RightSubscapularisII,
																SA_RightSubscapularisIII, SA_RightTeresMinor,
																SA_RightInfraspinatusI, SA_RightInfraspinatusII,
																SA_RightSuprasinatus, SA_RightMiddleDeltoid,
																SA_RightAnteriorDeltoid, SA_RightPosteriorDeltoid,
																SA_RightCoracobrachialis, SA_RightUpperLatissimusDorsi,
																SA_RightLowerLatissimusDorsi, SA_RightPectoralisMajorI,
																SA_RightPectoralisMajorII, SA_RightTrapesiusI,
																SA_RightTrapesiusII, SA_RightTrapesiusIII,  
																SA_RightTrapesiusIV, SA_RightPectoralisMinor,
																SA_RightLevatorScapulae, SA_RightRhomboidMinor,
																SA_RightRhomboidMajor, SA_RightUpperSerratusAnterior,
																SA_RightMiddleSerratusAnterior, SA_RightLowerSerratusAnterior,
																SA_RightSternohyoid, SA_RightOmohymoid, 
																SA_RightSubclavius, SA_RightSternocleidomastoid, 
																SA_LeftBicepsI, SA_LeftBicepsII,
																SA_LeftBrachialis, SA_LeftBrachioradialis,
																SA_LeftTricepsI, SA_LeftTricepsII, 
																SA_LeftTricepsIII, SA_LeftTeresMajor,
																SA_LeftSubscapularisI, SA_LeftSubscapularisII,
																SA_LeftSubscapularisIII, SA_LeftTeresMinor,
																SA_LeftInfraspinatusI, SA_LeftInfraspinatusII,
																SA_LeftSuprasinatus, SA_LeftMiddleDeltoid,
																SA_LeftAnteriorDeltoid, SA_LeftPosteriorDeltoid,
																SA_LeftCoracobrachialis, SA_LeftUpperLatissimusDorsi,
																SA_LeftLowerLatissimusDorsi, SA_LeftPectoralisMajorI,
																SA_LeftPectoralisMajorII, SA_LeftTrapesiusI,
																SA_LeftTrapesiusII, SA_LeftTrapesiusIII,  
																SA_LeftTrapesiusIV, SA_LeftPectoralisMinor,
																SA_LeftLevatorScapulae, SA_LeftRhomboidMinor,
																SA_LeftRhomboidMajor, SA_LeftUpperSerratusAnterior,
																SA_LeftMiddleSerratusAnterior, SA_LeftLowerSerratusAnterior,
																SA_LeftSternohyoid, SA_LeftOmohymoid, 
																SA_LeftSubclavius, SA_LeftSternocleidomastoid
			};

			int JCF_order[6] = {
										SAJCF_RightSternoclavicular, SAJCF_RightAcromioclavicular,
										SAJCF_RightGlenohumeral, SAJCF_LeftSternoclavicular,
										SAJCF_LeftAcromioclavicular,  SAJCF_LeftGlenohumeral
			};

			int LGT_order[6] = {
										SALGT_RightCoracohumeral, SALGT_RightConoid,
										SALGT_RightTrapezoid, SALGT_LeftCoracohumeral, 
										SALGT_LeftConoid,	SALGT_LeftTrapezoid 
			};

			int GSC_order[20] = {
										SAGSC_RightSuperioAngle, SAGSC_RightInferiorAngle,
										SAGSC_RightI, SAGSC_RightII,
										SAGSC_RightIII, SAGSC_RightIV,
										SAGSC_RightV, SAGSC_RightVI,
										SAGSC_RightVII, SAGSC_RightVIII,
										SAGSC_LeftSuperioAngle, SAGSC_LeftInferiorAngle,
										SAGSC_LeftI, SAGSC_LeftII,
										SAGSC_LeftIII, SAGSC_LeftIV,
										SAGSC_LeftV, SAGSC_LeftVI,
										SAGSC_LeftVII, LeftVIII
			};

			if(pass == HEADERS) {
				Write("Shoulder Analysis Starts");

				Write("Right Biceps I");
				Write("Right Biceps II");
				Write("Right Brachialis");
				Write("Right Brachioradialis");

				Write("Right Triceps I");
				Write("Right Triceps II");
				Write("Right Triceps III");

				Write("Right Teres Major");
				Write("Right Subscapularis I");
				Write("Right Subscapularis II");
				Write("Right Subscapularis III");

				Write("Right Teres Minor");
				Write("Right Infraspinatus I");
				Write("Right Infraspinatus II");

				Write("Right Supraspinatus");
				Write("Right Middle Deltoid");
				Write("Right Anterior Deltoid");
				Write("Right Posterior Deltoid");

				Write("Right Coracobrachialis");
				Write("Right Upper Latissimus Doris");
				Write("Right Lower Latissimus Doris");
				Write("Right Pectoralis Major I");
				Write("Right Pectoralis Major II");

				Write("Right Trapesius I");
				Write("Right Trapesius II");
				Write("Right Trapesius III");
				Write("Right Trapesius IV");
				Write("Right Pectoralis Minor");

				Write("Right Levator Scapulae");
				Write("Right Rhomboid Minor");
				Write("Right Rhomboid Major");

				Write("Right Upper Serratus Anterior");
				Write("Right Middle Serratus Anterior");
				Write("Right Lower Serratus Anterior");

				Write("Right Sternohyoid");
				Write("Right Omohymoid");
				Write("Right Subclavius");
				Write("Right Sternocleidomastoid");

				Write("Right Coracohumeral");
				Write("Right Conoid");
				Write("Right Trapezoid");

				Write("Right Sternoclavicular X");
				Write("Right Sternoclavicular Y");
				Write("Right Sternoclavicular Z");

				Write("Right Acromioclavicular X");
				Write("Right Acromioclavicular Y");
				Write("Right Acromioclavicular Z");

				Write("Right Glenohumeral X");
				Write("Right Glenohumeral Y");
				Write("Right Glenohumeral Z");

				Write("Right Superior Angle");
				Write("Right Inferior Angle");

				Write("Right GSC 1");
				Write("Right GSC 2");
				Write("Right GSC 3");
				Write("Right GSC 4");
				Write("Right GSC 5");
				Write("Right GSC 6");
				Write("Right GSC 7");
				Write("Right GSC 8");

				Write("Left Biceps I");
				Write("Left Biceps II");
				Write("Left Brachialis");
				Write("Left Brachioradialis");

				Write("Left Triceps I");
				Write("Left Triceps II");
				Write("Left Triceps III");

				Write("Left Teres Major");
				Write("Left Subscapularis I");
				Write("Left Subscapularis II");
				Write("Left Subscapularis III");

				Write("Left Teres Minor");
				Write("Left Infraspinatus I");
				Write("Left Infraspinatus II");

				Write("Left Supraspinatus");
				Write("Left Middle Deltoid");
				Write("Left Anterior Deltoid");
				Write("Left Posterior Deltoid");

				Write("Left Coracobrachialis");
				Write("Left Upper Latissimus Doris");
				Write("Left Lower Latissimus Doris");
				Write("Left Pectoralis Major I");
				Write("Left Pectoralis Major II");

				Write("Left Trapesius I");
				Write("Left Trapesius II");
				Write("Left Trapesius III");
				Write("Left Trapesius IV");
				Write("Left Pectoralis Minor");

				Write("Left Levator Scapulae");
				Write("Left Rhomboid Minor");
				Write("Left Rhomboid Major");

				Write("Left Upper Serratus Anterior");
				Write("Left Middle Serratus Anterior");
				Write("Left Lower Serratus Anterior");

				Write("Left Sternohyoid");
				Write("Left Omohymoid");
				Write("Left Subclavius");
				Write("Left Sternocleidomastoid");

				Write("Left Coracohumeral");
				Write("Left Conoid");
				Write("Left Trapezoid");

				Write("Left Sternoclavicular X");
				Write("Left Sternoclavicular Y");
				Write("Left Sternoclavicular Z");

				Write("Left Acromioclavicular X");
				Write("Left Acromioclavicular Y");
				Write("Left Acromioclavicular Z");

				Write("Left Glenohumeral X");
				Write("Left Glenohumeral Y");
				Write("Left Glenohumeral Z");

				Write("Left Superior Angle");
				Write("Left Inferior Angle");

				Write("Left GSC 1");
				Write("Left GSC 2");
				Write("Left GSC 3");
				Write("Left GSC 4");
				Write("Left GSC 5");
				Write("Left GSC 6");
				Write("Left GSC 7");
				Write("Left GSC 8");

				Write("Muscle force model data start");
				Write("Right elbow aeq variables start");
				for ( int i = 0; i < 60; i++ ) {
					Write(i+1);
				}
				Write("Right humeral force aeq variables start");
				std::string base = "";
				std::string header = "";
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}

				Write("Right humeral moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}

				Write("Right scapular force aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				Write("Right scapular moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				Write("Right calvicular force aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				Write("Right calvicular moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				Write("Right glenoid stability aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				Write("Right shoulder torque variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(i+1);
				}
				Write("Right elbow torque variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(i+1);
				}
				Write("Right shoulder reaction variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(i+1);
				}
				Write("Right rotation matrix from global to forearm coordinates");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 3; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				Write("Left elbow aeq variables start");
				for ( int i = 0; i < 60; i++ ) {
					Write(i+1);
				}
				Write("Left humeral force aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				Write("Left humeral moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				Write("Left scapular force aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				Write("Left scapular moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				Write("Left calvicular force aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				Write("Left calvicular moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				Write("Left glenoid stability aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				Write("Left shoulder torque variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(i+1);
				}
				Write("Left elbow torque variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(i+1);
				}
				Write("Left shoulder reaction variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(i+1);
				}
				Write("Left rotation matrix from global to forearm coordinates");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 3; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
			} 
			else {
				Write("shdr anls");

				for(int i = 0; i < 38; ++i) {
					Write(shoulder_analysis.SX[analysis_order[i]]);
				}

				for(int i = 0; i < 3; ++i) {
					Write(shoulder_analysis.LGT[LGT_order[i]]);
				}

				for(int i = 0; i < 3; ++i) {
					Write(shoulder_analysis.JCFX[JCF_order[i]]);
					Write(shoulder_analysis.JCFY[JCF_order[i]]);
					Write(shoulder_analysis.JCFZ[JCF_order[i]]);
				}

				for(int i = 0; i < 10; ++i) {
					Write(shoulder_analysis.GSC[GSC_order[i]]);
				}

				for(int i = 0; i < 38; ++i) {
					Write(shoulder_analysis.SX[analysis_order[i+38]]);
				}

				for(int i = 0; i < 3; ++i) {
					Write(shoulder_analysis.LGT[LGT_order[i+3]]);
				}

				for(int i = 0; i < 3; ++i) {
					Write(shoulder_analysis.JCFX[JCF_order[i+3]]);
					Write(shoulder_analysis.JCFY[JCF_order[i+3]]);
					Write(shoulder_analysis.JCFZ[JCF_order[i+3]]);
				}

				for(int i = 0; i < 10; ++i) {
					Write(shoulder_analysis.GSC[GSC_order[i+10]]);
				}

				Write("muscle force model");
				Write("Right elbow aeq variables start");
				for ( int i = 0; i < 60; i++ ) {
					Write(muscle_force.ln1[0][i]);
				}
				Write("Right humeral force aeq variables start");
				std::string base = "";
				std::string header = "";
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln2to4[i][j]);
					}
				}
				Write("Right humeral moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln5to7[i][j]);
					}
				}
				Write("Right scapular force aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln8to10[i][j]);
					}
				}
				Write("Right scapular moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln11to13[i][j]);
					}
				}
				Write("Right calvicular force aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln14to16[i][j]);
					}
				}
				Write("Right calvicular moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln17to19[i][j]);
					}
				}
				Write("Right glenoid stability aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln20to22[i][j]);
					}
				}
				Write("Right shoulder torque variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(muscle_force.MSHTorque[i]);
				}
				Write("Right elbow torque variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(muscle_force.MELTorque[i]);
				}
				Write("Right shoulder reaction variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(muscle_force.MSHReact[i]);
				}
				Write("Right rotation matrix from global to forearm coordinates");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						Write(muscle_force.RGF[i][j]);
					}
				}
				Write("Left elbow aeq variables start");
				for ( int i = 0; i < 60; i++ ) {
					Write(muscle_force.ln1[1][i]);
				}
				Write("Left humeral force aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln2to4[i+3][j]);
					}
				}
				Write("Left humeral moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln5to7[i+3][j]);
					}
				}
				Write("Left scapular force aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln8to10[i+3][j]);
					}
				}
				Write("Left scapular moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln11to13[i+3][j]);
					}
				}
				Write("Left calvicular force aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln14to16[i+3][j]);
					}
				}
				Write("Left calvicular moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln17to19[i+3][j]);
					}
				}
				Write("Left glenoid stability aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln20to22[i+3][j]);
					}
				}
				Write("Left shoulder torque variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(muscle_force.MSHTorque[i+3]);
				}
				Write("Left elbow torque variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(muscle_force.MELTorque[i+3]);
				}
				Write("Left shoulder reaction variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(muscle_force.MSHReact[i+3]);
				}
				Write("Left rotation matrix from global to forearm coordinates");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						Write(muscle_force.RGF[i+3][j]);
					}
				}
			}
		} 
		
		//--- Write Shoulder Geometry ---

		if(mShoulderGeometry)
		{
			int geometry_order[NUM_GEOMETRY_HINGES] =	{	SG_FirstThoracicVertebraS, SG_TwelvthThoracicVertebraI,
															SG_FirstThoracicVertebraM, SG_RightSternoclavicularJoint,
															SG_RightACJ, SG_RightScapulaAngle,
															SG_RightScapularSpine, SG_RightHumeralHead,
															SG_RightElbowJoint, SG_RightWristJoint,
															SG_RightSJCTJoint, SG_RightACRTJoint,
															SG_RightC7T1Joint, SG_RightL5S1Joint, 
															SG_LeftSternoclavicularJoint,
															SG_LeftACJ, SG_LeftScapulaAngle,
															SG_LeftScapularSpine, SG_LeftHumeralHead,
															SG_LeftElbowJoint, SG_LeftWristJoint,
															SG_LeftSJCTJoint, SG_LeftACRTJoint,
															SG_LeftC7T1Joint, SG_LeftL5S1Joint	};

			if(pass == HEADERS) {
				Write("Shoulder Geometry Starts");

				Write("1st Thoracic Vertebra (S) X");
				Write("1st Thoracic Vertebra (S) Y");
				Write("1st Thoracic Vertebra (S) Z");

				Write("12th Thoracic Vertebra (I) X");
				Write("12th Thoracic Vertebra (I) Y");
				Write("12th Thoracic Vertebra (I) Z");

				Write("1st Thoracic Vertebra (M) X");
				Write("1st Thoracic Vertebra (M) Y");
				Write("1st Thoracic Vertebra (M) Z");

				Write("Right Sternoclavicular Joint X");
				Write("Right Sternoclavicular Joint Y");
				Write("Right Sternoclavicular Joint Z");

				Write("Right Acriomoclavicular Joint X");
				Write("Right Acriomoclavicular Joint Y");
				Write("Right Acriomoclavicular Joint Z");

				Write("Right Scapula Angle X");
				Write("Right Scapula Angle Y");
				Write("Right Scapula Angle Z");

				Write("Right Scapula Spine X");
				Write("Right Scapula Spine Y");
				Write("Right Scapula Spine Z");

				Write("Right Humeral Head X");
				Write("Right Humeral Head Y");
				Write("Right Humeral Head Z");

				Write("Right Elbow Joint X");
				Write("Right Elbow Joint Y");
				Write("Right Elbow Joint Z");

				Write("Right Wrist Joint X");
				Write("Right Wrist Joint Y");
				Write("Right Wrist Joint Z");

				Write("Right Shoulder Joint Center X");
				Write("Right Shoulder Joint Center Y");
				Write("Right Shoulder Joint Center Z");

				Write("Right Acromion X");
				Write("Right Acromion Y");
				Write("Right Acromion Z");

				Write("Right C7T1 Vertebral Joint X");
				Write("Right C7T1 Vertebral Joint Y");
				Write("Right C7T1 Vertebral Joint Z");

				Write("Right L5S1 Vertebral Joint X");
				Write("Right L5S1 Vertebral Joint Y");
				Write("Right L5S1 Vertebral Joint Z");	

				Write("Left Sternoclavicular Joint X");
				Write("Left Sternoclavicular Joint Y");
				Write("Left Sternoclavicular Joint Z");

				Write("Left Acriomoclavicular Joint X");
				Write("Left Acriomoclavicular Joint Y");
				Write("Left Acriomoclavicular Joint Z");

				Write("Left Scapula Angle X");
				Write("Left Scapula Angle Y");
				Write("Left Scapula Angle Z");

				Write("Left Scapula Spine X");
				Write("Left Scapula Spine Y");
				Write("Left Scapula Spine Z");

				Write("Left Humeral Head X");
				Write("Left Humeral Head Y");
				Write("Left Humeral Head Z");

				Write("Left Elbow Joint X");
				Write("Left Elbow Joint Y");
				Write("Left Elbow Joint Z");

				Write("Left Wrist Joint X");
				Write("Left Wrist Joint Y");
				Write("Left Wrist Joint Z");

				Write("Left Shoulder Joint Center X");
				Write("Left Shoulder Joint Center Y");
				Write("Left Shoulder Joint Center Z");

				Write("Left Acromion X");
				Write("Left Acromion Y");
				Write("Left Acromion Z");

				Write("Left C7T1 Vertebral Joint X");
				Write("Left C7T1 Vertebral Joint Y");
				Write("Left C7T1 Vertebral Joint Z");

				Write("Left L5S1 Vertebral Joint X");
				Write("Left L5S1 Vertebral Joint Y");
				Write("Left L5S1 Vertebral Joint Z");				
			}
			else {
				Write("shdr gmty");

				for (int i = 0; i < NUM_GEOMETRY_HINGES; ++i) {
					Write(shoulder_geometry.XMetric[geometry_order[i]]);
					Write(shoulder_geometry.YMetric[geometry_order[i]]);
					Write(shoulder_geometry.ZMetric[geometry_order[i]]);
				}
			}
		}

		EndL();
	}
	//Since header is needed only in the first frame, set the head tag to false after reporting the first frame. 
	this->mColumnHeaders = FALSE;
	return TRUE;
}
BOOL ExportSummary::Export(fstream &aOutputFile)
{

	// Set precision
	aOutputFile << std::setprecision(2) << std::fixed;
	
    //Assign this file stream pointer to support inline writes
    mFileStreamPtr = &aOutputFile;

    //Commonly used references
    //Analysis &lResults = mDocPtr->getResultsRef();
	Skeleton* skel = mDocPtr->GetSkeleton();

    //AnalysisSummaryData analysis_summary;
	//lResults.getAnalysisSummary(&analysis_summary);
	AnalysisSummaryData_s analysis_summary = skel->getAnalysisSummary();

    //StrengthCapabilityData strength;
	//lResults.getStrengthCapability(&strength);
	StrengthData_s strength = skel->getStrengthData();

    //JointForceData jointForces;
    //lResults.getJointForces(&jointForces);
	Vector3 jointForces[NUM_JOINTS];
	skel->getNetForces(jointForces);

    //JointMomentData jointMoments;
    //lResults.getJointMoments(&jointMoments);
	Vector3 jointMoments[NUM_JOINTS];
	skel->getNetMoments(jointMoments);

    //LowBack3DData lowback3D;
    //lResults.getLowBack3D(&lowback3D);
	LowBack3DData_s lowback3D = skel->getLowBack3DData();

    //LowBackSagittalData lowbackSag;
    //lResults.getLowBackSagittal(&lowbackSag);
	LowBackSagittalData_s lowbackSag = skel->getLowBackSagittalData();

    //BalanceData balance;
    //lResults.getBalanceData(&balance);
	BalanceData_s balance = skel->getBalanceData();

    //BalanceMomentData balance_moments;
    //lResults.getBalanceMoments(&balance_moments);

    //SpinalForceData spinal_forces;
    //lResults.getSpinalForces(&spinal_forces);
	SpinalForces_s spinal_forces = skel->getSpinalForces();

    //SpinalMomentData spinal_moments;
    //lResults.getSpinalMoments(&spinal_moments);
	SpinalMoments_s spinal_moments = skel->getSpinalMoments();

    //StrengthVectorData strength_vectors;
    //lResults.getStrengthVectors(&strength_vectors);
	Vector3 strength_vectors[NUM_STRENGTH_HINGES];
	skel->getStrengthDirectionVectors(strength_vectors);

    //FatigueData fatigue;
    //lResults.getFatigue(&fatigue);
	FatigueData_s fatigue = skel->getFatigueData();

	//ShoulderGeometryData shoulder_geometry
	ShoulderGeometryData_s shoulder_geometry = skel->getShoulderGeometryData();

	//ShoudlerAnalysisData shoulder_analysis
	ShoulderAnalysisData_s shoulder_analysis = skel->getShoulderAnalysisData();

	//MuscleForceModelData muscle_force;
	MuscleForceModelData_s muscle_force = skel->getMuscleForceModelData();

	double extraAngles[NUM_EXTRA_ANGLES];
	skel->getExtraAngles(extraAngles);

	int lSex = mDocPtr->getAnthropometryRef().Sex();//lResults.sex; //male = 1, female = 2
	JointAngles docJointAngles = skel->GetAngles();
    //mDocPtr->GetSkeleton().GetAngles(mDocPtr->GetSkeleton().getCurrentFrame(),docJointAngles);

	const int HEADERS = 0;
	const int CONTENT = 1;
	for(int pass = HEADERS; pass <= CONTENT; pass++) {
		// skip headers
		if(!mColumnHeaders && pass == HEADERS) continue;

		//--- Write Standard Information ---
		if(pass == HEADERS) {
			Write("Analyst");
			Write("Company Name");
			Write("Units");
			Write("Task Name");
			Write("Gender");
			Write("Height");
			Write("Weight");
		} else {

			Write(mDocPtr->getDescriptionRef().Analyst());
			Write(mDocPtr->getDescriptionRef().CompanyRef());
			Write(mDocPtr->getDescriptionRef().UnitsValueText());
			Write(mDocPtr->getDescriptionRef().Task());
			Write(mDocPtr->Gender());
			Write(mDocPtr->getAnthropometryRef().Height());
			Write(mDocPtr->getAnthropometryRef().Weight());
		}


		//--- Write Summary Results ---

		if(mSummaryResults) {
			if(pass == HEADERS) {
				Write("Summary Start");
				Write("L5/S1 Compression");
				Write("L4/L5 Compression");
				Write("Minimum Wrist Percentile");
				Write("Minimum Elbow Percentile");
				Write("Minimum Shoulder Percentile");
				Write("Minimum Torso Percentile");
				Write("Minimum Neck Percentile");
				Write("Minimum Hip Percentile");
				Write("Minimum Knee Percentile");
				Write("Minimum Ankle Percentile");
				Write("Coefficient of Friction");
				Write("Left Load Fraction");
				Write("Balance Status");
			} else {

				Write("summary");

				AnalysisSummaryData_s summary = skel->getAnalysisSummary();
		  	 
				/* double conversion = U_M_CM;
				if (mDocPtr->Is_English())
					conversion = U_NM_INLB; */

				Write(summary.L5S1Compression); //L5/S1
				Write(summary.L4L5Compression); //L4/L5

				Write(summary.WristMinPercentile);
				Write(summary.ElbowMinPercentile); 
				Write(summary.ShoulderMinPercentile);
				Write(summary.TorsoMinPercentile);
				Write(summary.NeckMinPercentile);
				Write(summary.HipMinPercentile);
				Write(summary.KneeMinPercentile);
				Write(summary.AnkleMinPercentile);

				Write(summary.frcoef);
				Write(balance.LeftPctLoad);
				Write(balance.BalanceStatus);
			}
		}

		//--- Write Strength Capabilities ---

		if(mStrength)
		{
			StrengthHinge_e strength_hinge_order[NUM_STRENGTH_HINGES] = {
				SH_RWristFlexion, SH_RWristDeviation, SH_RForearmRotation,
				SH_RElbowFlexion, SH_RHumeralRotation, SH_RShoulderRotation, SH_RShoulderAbduction,
				SH_RHipFlexion, SH_RKneeFlexion, SH_RAnkleFlexion,
				SH_LWristFlexion, SH_LWristDeviation, SH_LForearmRotation,
				SH_LElbowFlexion, SH_LHumeralRotation, SH_LShoulderRotation, SH_LShoulderAbduction,
				SH_LHipFlexion, SH_LKneeFlexion, SH_LAnkleFlexion,
				SH_TorsoFlexion, SH_TorsoLateralBending, SH_TorsoRotation, SH_NeckFlexion, SH_NeckLateralBending, SH_NeckRotation
			};

			if(pass == HEADERS) {
				Write("Strength Capability Start");
				// TODO store this better somewhere. Types.h?
				Write("Right Wrist Flexion");
				Write("Right Wrist Deviation");
				Write("Right Forearm Rotation");
				Write("Right Elbow Flexion");
				Write("Right Humeral Rotation");
				Write("Right Shoulder Rotation");
				Write("Right Shoulder Abduction");
				Write("Right Hip Flexion");
				Write("Right Knee Flexion");
				Write("Right Ankle Flexion");

				Write("Left Wrist Flexion");
				Write("Left Wrist Deviation");
				Write("Left Forearm Rotation");
				Write("Left Elbow Flexion");
				Write("Left Humeral Rotation");
				Write("Left Shoulder Rotation");
				Write("Left Shoulder Abduction");
				Write("Left Hip Flexion");
				Write("Left Knee Flexion");
				Write("Left Ankle Flexion");

				Write("Torso Flexion");
				Write("Torso Lateral Bending");
				Write("Torso Rotation");
				
				Write("Neck Flexion");
				Write("Neck Lateral Bending");
				Write("Neck Rotation");

			} else {

				Write("str cap");

				for(int i = 0; i < NUM_STRENGTH_HINGES; i++) {
					//Write(lResults.zprob[lSex][i]);
					Write(strength.Percentile[strength_hinge_order[i]]);
				}
			}
		}

		//--- Write Low Back ---

		if(mLowBack) {
			if(pass == HEADERS) {
				Write("Low Back Start");
				Write("L5/S1 Compression");
				Write("L5/S1 Compression SD");
				Write("Sagittal Shear");
				Write("Frontal Shear");

				Write("Total L4/L5 Compression");
				Write("Anterior L4/L5 Shear");
				Write("Lateral L4/L5 Shear");

				Write("Right Erector Force Magnitude");
				Write("Right Erector Shear");
				Write("Right Erector Force X"); Write("Right Erector Force Y"); Write("Right Erector Force Z");
				Write("Right Rectus Force Magnitude");
				Write("Right Rectus Shear");
				Write("Right Rectus Force X"); Write("Right Rectus Force Y"); Write("Right Rectus Force Z");
				Write("Right Internal Force Magnitude");
				Write("Right Internal Shear");
				Write("Right Internal Force X"); Write("Right Internal Force Y"); Write("Right Internal Force Z");
				Write("Right External Force Magnitude");
				Write("Right External Shear");
				Write("Right External Force X"); Write("Right External Force Y"); Write("Right External Force Z");
				Write("Right Latissimus Force Magnitude");
				Write("Right Latissimus Shear");
				Write("Right Latissimus Force X"); Write("Right Latissimus Force Y"); Write("Right Latissimus Force Z");
				Write("Left Erector Force Magnitude");
				Write("Left Erector Shear");
				Write("Left Erector Force X"); Write("Left Erector Force Y"); Write("Left Erector Force Z");
				Write("Left Rectus Force Magnitude");
				Write("Left Rectus Shear");
				Write("Left Rectus Force X"); Write("Left Rectus Force Y"); Write("Left Rectus Force Z");
				Write("Left Internal Force Magnitude");
				Write("Left Internal Shear");
				Write("Left Internal Force X"); Write("Left Internal Force Y"); Write("Left Internal Force Z");
				Write("Left External Force Magnitude");
				Write("Left External Shear");
				Write("Left External Force X"); Write("Left External Force Y"); Write("Left External Force Z");
				Write("Left Latissimus Force Magnitude");
				Write("Left Latissimus Shear");
				Write("Left Latissimus Force X"); Write("Left Latissimus Force Y"); Write("Left Latissimus Force Z");
			} else {

				Write("low back");
				
				AnalysisSummaryData_s summary = skel->getAnalysisSummary();
		  	 
				Write(summary.L5S1Compression);
				Write(summary.L5S1CompressionSD);
				Write(lowbackSag.shrSagittal);
				Write(lowbackSag.shrFrontal);

				Write(lowback3D.totalL4L5Compression);
				Write(lowback3D.anteriorL4L5Shear);
				Write(lowback3D.lateralL4L5Shear);

				int muscleOrder[10] = {BM_RightErector,
							BM_RightRectus,
							BM_RightInternal,
							BM_RightExternal,
							BM_RightLatis,
							BM_LeftErector,
							BM_LeftRectus, 
							BM_LeftInternal, 
							BM_LeftExternal, 
							BM_LeftLatis};

				int num_back3D = 10;
				for (int i = 0; i < num_back3D; ++i)
				{
					Write(lowback3D.Resultant[muscleOrder[i]]);
					Write(lowback3D.Shear[muscleOrder[i]]);
					Write(lowback3D.XForce[muscleOrder[i]]);
					Write(lowback3D.YForce[muscleOrder[i]]);
					Write(lowback3D.ZForce[muscleOrder[i]]);
				}
			}
		} //end if(mLowBack)


		//--- Write Fatigue Data ---

		if (mFatigueReport) {
			if(pass == HEADERS) {
				Write("Fatigue Start");
				// TODO store this better somewhere. Types.h?
				const char* hinge_name_order[NUM_STRENGTH_HINGES] = {
					"Right Wrist Flx/Ext", "Right Wrist Deviation",
					"Right Forearm Rotation", "Right Elbow Flexion",
					"Right Humeral Rotation", "Right Fd/Bk Rotation", "Right Shoulder Abd/Add",
					"Right Hip Flexion", "Right Knee Flexion", "Right Ankle Flexion",

					"Left Wrist Flx/Ext", "Left Wrist Deviation",
					"Left Forearm Rotation", "Left Elbow Flexion",
					"Left Humeral Rotation", "Left Fd/Bk Rotation", "Left Shoulder Abd/Add",
					"Left Hip Flexion", "Left Knee Flexion", "Left Ankle Flexion",
				
					"Torso Flexion", "Torso Lateral Bending", "Torso Rotation"
					"Neck Flexion", "Neck Lateral Bending", "Neck Rotation"
				};
				
				std::string base;
				for(int i = 0; i < NUM_STRENGTH_HINGES; i++) {
					base = hinge_name_order[i];
					Write((base + " 5%ile" + " %MVC").c_str());
					Write((base + " 5%ile" + " %DC").c_str());
					Write((base + " 5%ile" + " Dur(s)").c_str());
					Write((base + " 25%ile" + " %MVC").c_str());
					Write((base + " 25%ile" + " %DC").c_str());
					Write((base + " 25%ile" + " Dur(s)").c_str());
					Write((base + " 50%ile" + " %MVC").c_str());
					Write((base + " 50%ile" + " %DC").c_str());
					Write((base + " 50%ile" + " Dur(s)").c_str());
				}
			} else {

				Write("fatigue");

				StrengthHinge_e hingeOrder[NUM_STRENGTH_HINGES] = {
							SH_RWristFlexion, SH_RWristDeviation,
							SH_RForearmRotation,	SH_RElbowFlexion,
							SH_RHumeralRotation, SH_RShoulderRotation, SH_RShoulderAbduction,
							SH_RHipFlexion, SH_RKneeFlexion, SH_RAnkleFlexion,
							SH_LWristFlexion, SH_LWristDeviation,
							SH_LForearmRotation,	SH_LElbowFlexion,
							SH_LHumeralRotation, SH_LShoulderRotation, SH_LShoulderAbduction,
							SH_LHipFlexion, SH_LKneeFlexion, SH_LAnkleFlexion,
							SH_TorsoFlexion, SH_TorsoLateralBending, SH_TorsoRotation,
				SH_NeckFlexion, SH_NeckLateralBending, SH_NeckRotation};
		  	 
				for(int i = 0; i < NUM_STRENGTH_HINGES; i++) {
					Write(floor(fatigue.FifthPercentile[hingeOrder[i]] + 0.5));
					Write(floor(fatigue.FifthPercentileDC[hingeOrder[i]] + 0.5));
					Write(floor(fatigue.FifthPercentileMT[hingeOrder[i]] + 0.5));

					Write(floor(fatigue.FiftiethPercentile[hingeOrder[i]] + 0.5));
					Write(floor(fatigue.FiftiethPercentileDC[hingeOrder[i]] + 0.5));
					Write(floor(fatigue.FiftiethPercentileMT[hingeOrder[i]] + 0.5));

					Write(floor(fatigue.NinetyFifthPercentile[hingeOrder[i]] + 0.5));
					Write(floor(fatigue.NinetyFifthPercentileDC[hingeOrder[i]] + 0.5));
					Write(floor(fatigue.NinetyFifthPercentileMT[hingeOrder[i]] + 0.5));
				}
			}
		}

		//--- Write Balance Data ---

		if(mBalanceReport) {
			if(pass == HEADERS) {
				Write("Balance Start");
				Write("COG X");
				Write("COG Y");
				Write("COP X");
				Write("COP Y");
				Write("Stability");
				Write("Left load");
				Write("Right load");
			} else {
				Write("balance");
				// COM
				Write(balance.CenterOfGravity[0]);
				Write(balance.CenterOfGravity[1]);
				// COP
				Write(balance.CenterOfPressure[0]);
				Write(balance.CenterOfPressure[1]);
				// stability
				if(balance.BalanceStatus == BS_Acceptable) {
					Write("Acceptable");
				} else if(balance.BalanceStatus == BS_Critical) {
					Write("Critical");
				} else {
					Write("Unacceptable");
				}
				// leg loads
				Write(balance.LeftPctLoad);
				Write(balance.RightPctLoad);
			}
		}

		//--- Write Hand Forces ---

		//Skeleton &skel = mDocPtr->GetSkeleton();
		if(mHandForces)
		{
			if(pass == HEADERS) {
				Write("Hand Forces Start");
				Write("Right Force Magnitude");
				Write("Right Vertical Angle");
				Write("Right Horizontal Angle");
				Write("Left Force Magnitude");
				Write("Left Vertical Angle");
				Write("Left Horizontal Angle");
			} else {

				Write("hand forces");

				double rv,rh,lv,lh;

				// skel->GetHandForceAngles(rv,rh,lv,lh);
				Vector3 LHandForce = skel->getExtForce(JT_LHAND);
				Vector3 RHandForce = skel->getExtForce(JT_RHAND);
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

				Write(skel->getExtForce(JT_RHAND).length());
				Write(rv);
				Write(rh);

				Write(skel->getExtForce(JT_LHAND).length());
				Write(lv);
				Write(lh);
			}
		}

		//--- Write Body Segment Angles ---

		if(mBodySegmentAngles) {

			// 39 instead of NUM_ANGLES == 40, because there's that SCJ one we don't use
			const int num_angles = 39;
			E_Angles angle_order[num_angles] = {
				A_HandRV, A_HandRH, A_HandRRot,
				A_ForearmRV, A_ForearmRH,
				A_UpperarmRV, A_UpperarmRH,
				A_ClavicleRV, A_ClavicleRH,
				A_UpperlegRV, A_UpperlegRH,
				A_LowerlegRV, A_LowerlegRH,
				A_FootRV, A_FootRH,

				A_HandLV, A_HandLH, A_HandLRot,
				A_ForearmLV, A_ForearmLH,
				A_UpperarmLV, A_UpperarmLH,
				A_ClavicleLV, A_ClavicleLH,
				A_UpperlegLV, A_UpperlegLH,
				A_LowerlegLV, A_LowerlegLH,
				A_FootLV, A_FootLH,

				A_C4C7LateralBending, A_C4C7Flexion, A_C4C7AxialRotation,
				A_TrunkLateralBending, A_TrunkFlexion, A_TrunkAxialRotation,
				A_PelvisLateralBending, A_PelvisFlexion, A_PelvisAxialRotation
			};

			if(pass == HEADERS) {
				Write("Segment Angles Start");
				for(int i = 0; i < num_angles; i++) {
					Write(angleData[angle_order[i]].name);
				}
			} else {

				Write("seg ang");

				for(int i = 0; i < num_angles; i++) {
					// bug for which we released 6.0.5:
					// this wrote the default angles instead of the current angles
					//Write(angleData[angle_order[i]]);
					Write(docJointAngles[angle_order[i]]);
				}
			}
		}

		//--- Write Posture Angles ---

		if(mPostureAngles)
		{

			const int num_extra_angles1 = 28;
			E_ExtraAngles extra_angle_order1[num_extra_angles1] = {
				EA_HandRFlexion, EA_HandRDeviation, EA_ForearmRRot, EA_ElbowRInc,
				EA_ShoulderRV, EA_ShoulderRH, EA_HumerusRRot,
				EA_HipRInc, EA_HipRV, EA_HipRH,
				EA_FemurRRot, EA_LowerlegRRot, EA_KneeRInc, EA_AnkleRInc,
				
				EA_HandLFlexion, EA_HandLDeviation, EA_ForearmLRot, EA_ElbowLInc,
				EA_ShoulderLV, EA_ShoulderLH, EA_HumerusLRot,
				EA_HipLInc, EA_HipLV, EA_HipLH,
				EA_FemurLRot, EA_LowerlegLRot, EA_KneeLInc, EA_AnkleLInc};

			const int num_angles2 = 7;
			E_Angles angle_order2[num_angles2] = {
				A_C1_Flexion, A_C1C3_Flexion, A_C1C3_AxialRotation, A_C1C3_LateralBending, A_C4C7Flexion, A_C4C7AxialRotation, A_C4C7LateralBending
			};

			const int num_extra_angles3 = 4;
			E_ExtraAngles extra_angle_order3[num_extra_angles3] = {
				EA_TrunkFlexion, EA_AdjustedTrunkAxialRotation, EA_AdjustedTrunkLateralBending,

				EA_PelvisFlexion
			};

			const int num_angles4 = 2;
			E_Angles angle_order4[num_angles4] = {
				A_PelvisAxialRotation, A_PelvisLateralBending
			};

			// Then the last crazy one
			// from ReportPosture
			#define L5S1_ERECT_TILT 50
			double alpha = L5S1_ERECT_TILT - extraAngles[EA_PelvisFlexion];
			double tilt = 90 - alpha;

			if(pass == HEADERS) {
				Write("Posture Angles Start");
				for(int i = 0; i < num_extra_angles1; i++) {
					Write(extraAngleNames[extra_angle_order1[i]]);
				}
				for(int i = 0; i < num_angles2; i++) {
					if (i == 0)
						Write(90 - (skel->getExtraAngle(EA_C1_Flexion)+16));
					else if (i == 1)
						Write(90 - (skel->getExtraAngle(EA_C1C3Flexion)));
					else if (i == 4)
						Write(90 - (skel->getExtraAngle(EA_C4C7Flexion)));
					else
						Write(docJointAngles[angle_order2[i]]);
				}
				for(int i = 0; i < num_extra_angles3; i++) {
					Write(extraAngleNames[extra_angle_order3[i]]);
				}
				for(int i = 0; i < num_angles4; i++) {
					Write(angleData[angle_order4[i]].name);
				}
				Write("L5S1 Tilt Angle");

			} else {

				Write("post ang");
				
				for(int i = 0; i < num_extra_angles1; i++) {
					Write(extraAngles[extra_angle_order1[i]]);
				}
				for(int i = 0; i < num_angles2; i++) {
					Write(docJointAngles[angle_order2[i]]);
				}
				for(int i = 0; i < num_extra_angles3; i++) {
					Write(extraAngles[extra_angle_order3[i]]);
				}
				for(int i = 0; i < num_angles4; i++) {
					Write(docJointAngles[angle_order4[i]]);
				}
				Write(tilt);
			}

		}

		//--- Write Joint Locations ---

		if(mJointLocations) {
			Skeleton* lSkeleton = mDocPtr->GetSkeleton();
	        
			JointID jointOrder[NUM_JOINTS] = {
				JT_RHAND, JT_RWRIST, JT_RELBOW, JT_RSHOULDER,
				JT_RHIP, JT_RIT, JT_RKNEE, JT_RANKLE, JT_RHEEL, JT_RFOOT_CENTER, JT_RBALL,
				JT_LHAND, JT_LWRIST, JT_LELBOW, JT_LSHOULDER,
				JT_LHIP, JT_LIT, JT_LKNEE, JT_LANKLE, JT_LHEEL, JT_LFOOT_CENTER, JT_LBALL,
				JT_TRAGIONLOC, JT_NASIONLOC, JT_C3C4, JT_C7T1, JT_SCJ, JT_L5S1_TEMP,
				JT_HIP_CENTER, JT_IT_CENTER, JT_ANKLE_CENTER, JT_BALL_CENTER};

			if(pass == HEADERS) {
				Write("Joint Locations Start");
				std::string base;
				std::string header;
				for(int i = 0; i < NUM_JOINTS; i++) {
					base = jointNames[jointOrder[i]];
					header = base + " X";
					Write(header.c_str());
					header = base + " Y";
					Write(header.c_str());
					header = base + " Z";
					Write(header.c_str());
				}
			} else {

				Write("jnt loc");

				Vector3 write_vec;
				for (int i = 0; i < NUM_JOINTS; ++i)
				{
					/*lSkeleton->C4EPoint(locations[i], lLocX, lLocY, lLocZ);
					Write(lLocX.Value()); Write(lLocY.Value()); Write(lLocZ.Value());*/
					write_vec = lSkeleton->C4EPoint(jointOrder[i]);
					Write(write_vec[0]);
					Write(write_vec[1]);
					Write(write_vec[2]);
				}
			}
		} 

		//-- Write Joint Forces ---//

		if (mJointForces)
		{

			JointID jointOrder[NUM_JOINTS] = {
				JT_RHAND, JT_RWRIST, JT_RELBOW, JT_RSHOULDER,
				JT_RHIP, JT_RIT, JT_RKNEE, JT_RANKLE, JT_RHEEL, JT_RFOOT_CENTER, JT_RBALL,
				JT_LHAND, JT_LWRIST, JT_LELBOW, JT_LSHOULDER,
				JT_LHIP, JT_LIT, JT_LKNEE, JT_LANKLE, JT_LHEEL, JT_LFOOT_CENTER, JT_LBALL,
				JT_TRAGIONLOC, JT_NASIONLOC, JT_C3C4, JT_C7T1, JT_SCJ, JT_L5S1_TEMP,
				JT_HIP_CENTER, JT_IT_CENTER, JT_ANKLE_CENTER, JT_BALL_CENTER};

			if(pass == HEADERS) {
				Write("Joint Forces Start");
				std::string base;
				std::string header;
				for(int i = 0; i < NUM_JOINTS; i++) {
					base = jointNames[jointOrder[i]];
					header = base + " X";
					Write(header.c_str());
					header = base + " Y";
					Write(header.c_str());
					header = base + " Z";
					Write(header.c_str());
				}

				Write("Right Forward Seat X");
				Write("Right Forward Seat Y");
				Write("Right Forward Seat Z");

				Write("Left Forward Seat X");
				Write("Left Forward Seat Y");
				Write("Left Forward Seat Z");

				Write("Seat Back X");
				Write("Seat Back Y");
				Write("Seat Back Z");
			} else {

				Write("jnt forces");

				for (int i = 0; i < NUM_JOINTS; ++i) {
					for (int j = 0; j < 3; ++j)
					{
						Write(jointForces[jointOrder[i]][j]);
					}
				}

				if(mDocPtr->getPosition() == P_Sitting) {
					// Forward seat edge
					Vector3 write_vec;
					write_vec = skel->getRightSeatFrontForce();
					Write(write_vec[0]);
					Write(write_vec[1]);
					Write(write_vec[2]);
					write_vec = skel->getLeftSeatFrontForce();
					Write(write_vec[0]);
					Write(write_vec[1]);
					Write(write_vec[2]);
					// Seat back
					write_vec = skel->getSeatBackForce();
					Write(-write_vec[0]);
					Write(-write_vec[1]);
					Write(-write_vec[2]);
				} else {
					// write zeros for standing
					// right forward seat
					Write(0);
					Write(0);
					Write(0);
					// left forward seat
					Write(0);
					Write(0);
					Write(0);
					// seat back
					Write(0);
					Write(0);
					Write(0);
				}
			}
		 }


		//--- Write Joint Moments ---

		// Chris suggested changing this to simply reading the jointForces array (defined earlier) and 
		// writing out the component forces therein. I think I've now done what he wanted. --Sayan.
	    

		if(mJointMoments)
		{
			JointID jointOrder[NUM_JOINTS] = {
				JT_RHAND, JT_RWRIST, JT_RELBOW, JT_RSHOULDER,
				JT_RHIP, JT_RIT, JT_RKNEE, JT_RANKLE, JT_RHEEL, JT_RFOOT_CENTER, JT_RBALL,
				JT_LHAND, JT_LWRIST, JT_LELBOW, JT_LSHOULDER,
				JT_LHIP, JT_LIT, JT_LKNEE, JT_LANKLE, JT_LHEEL, JT_LFOOT_CENTER, JT_LBALL,
				JT_TRAGIONLOC, JT_NASIONLOC, JT_C3C4, JT_C7T1, JT_SCJ, JT_L5S1_TEMP,
				JT_HIP_CENTER, JT_IT_CENTER, JT_ANKLE_CENTER, JT_BALL_CENTER
			};

			if(pass == HEADERS) {
				Write("Joint Moments Start");
				std::string base;
				std::string header;
				for(int i = 0; i < NUM_JOINTS; i++) {
					base = jointNames[jointOrder[i]];
					header = base + " X";
					Write(header.c_str());
					header = base + " Y";
					Write(header.c_str());
					header = base + " Z";
					Write(header.c_str());
				}
			} else {
				Write("jnt mom");
				for (int i = 0; i < NUM_JOINTS; ++i) {
					for (int j = 0; j < 3; ++j) {
						Write(jointMoments[jointOrder[i]][j]);
					}
				}
			}
		}

		//--- Write Shoulder Analysis ---

		if(mShoulderAnalysis)
		{
						int analysis_order[NUM_SHOULDERANALYSIS_HINGES] = {	SA_RightBicepsI, SA_RightBicepsII,
																SA_RightBrachialis, SA_RightBrachioradialis,
																SA_RightTricepsI, SA_RightTricepsII, 
																SA_RightTricepsIII, SA_RightTeresMajor,
																SA_RightSubscapularisI, SA_RightSubscapularisII,
																SA_RightSubscapularisIII, SA_RightTeresMinor,
																SA_RightInfraspinatusI, SA_RightInfraspinatusII,
																SA_RightSuprasinatus, SA_RightMiddleDeltoid,
																SA_RightAnteriorDeltoid, SA_RightPosteriorDeltoid,
																SA_RightCoracobrachialis, SA_RightUpperLatissimusDorsi,
																SA_RightLowerLatissimusDorsi, SA_RightPectoralisMajorI,
																SA_RightPectoralisMajorII, SA_RightTrapesiusI,
																SA_RightTrapesiusII, SA_RightTrapesiusIII,  
																SA_RightTrapesiusIV, SA_RightPectoralisMinor,
																SA_RightLevatorScapulae, SA_RightRhomboidMinor,
																SA_RightRhomboidMajor, SA_RightUpperSerratusAnterior,
																SA_RightMiddleSerratusAnterior, SA_RightLowerSerratusAnterior,
																SA_RightSternohyoid, SA_RightOmohymoid, 
																SA_RightSubclavius, SA_RightSternocleidomastoid, 
																SA_LeftBicepsI, SA_LeftBicepsII,
																SA_LeftBrachialis, SA_LeftBrachioradialis,
																SA_LeftTricepsI, SA_LeftTricepsII, 
																SA_LeftTricepsIII, SA_LeftTeresMajor,
																SA_LeftSubscapularisI, SA_LeftSubscapularisII,
																SA_LeftSubscapularisIII, SA_LeftTeresMinor,
																SA_LeftInfraspinatusI, SA_LeftInfraspinatusII,
																SA_LeftSuprasinatus, SA_LeftMiddleDeltoid,
																SA_LeftAnteriorDeltoid, SA_LeftPosteriorDeltoid,
																SA_LeftCoracobrachialis, SA_LeftUpperLatissimusDorsi,
																SA_LeftLowerLatissimusDorsi, SA_LeftPectoralisMajorI,
																SA_LeftPectoralisMajorII, SA_LeftTrapesiusI,
																SA_LeftTrapesiusII, SA_LeftTrapesiusIII,  
																SA_LeftTrapesiusIV, SA_LeftPectoralisMinor,
																SA_LeftLevatorScapulae, SA_LeftRhomboidMinor,
																SA_LeftRhomboidMajor, SA_LeftUpperSerratusAnterior,
																SA_LeftMiddleSerratusAnterior, SA_LeftLowerSerratusAnterior,
																SA_LeftSternohyoid, SA_LeftOmohymoid, 
																SA_LeftSubclavius, SA_LeftSternocleidomastoid
			};

			int JCF_order[6] = {
										SAJCF_RightSternoclavicular, SAJCF_RightAcromioclavicular,
										SAJCF_RightGlenohumeral, SAJCF_LeftSternoclavicular,
										SAJCF_LeftAcromioclavicular,  SAJCF_LeftGlenohumeral
			};

			int LGT_order[6] = {
										SALGT_RightCoracohumeral, SALGT_RightConoid,
										SALGT_RightTrapezoid, SALGT_LeftCoracohumeral, 
										SALGT_LeftConoid,	SALGT_LeftTrapezoid 
			};

			int GSC_order[20] = {
										SAGSC_RightSuperioAngle, SAGSC_RightInferiorAngle,
										SAGSC_RightI, SAGSC_RightII,
										SAGSC_RightIII, SAGSC_RightIV,
										SAGSC_RightV, SAGSC_RightVI,
										SAGSC_RightVII, SAGSC_RightVIII,
										SAGSC_LeftSuperioAngle, SAGSC_LeftInferiorAngle,
										SAGSC_LeftI, SAGSC_LeftII,
										SAGSC_LeftIII, SAGSC_LeftIV,
										SAGSC_LeftV, SAGSC_LeftVI,
										SAGSC_LeftVII, LeftVIII
			};

			if (pass == HEADERS) {
				Write("Shoulder Analysis Starts");

				Write("Right Biceps I");
				Write("Right Biceps II");
				Write("Right Brachialis");
				Write("Right Brachioradialis");

				Write("Right Triceps I");
				Write("Right Triceps II");
				Write("Right Triceps III");

				Write("Right Teres Major");
				Write("Right Subscapularis I");
				Write("Right Subscapularis II");
				Write("Right Subscapularis III");

				Write("Right Teres Minor");
				Write("Right Infraspinatus I");
				Write("Right Infraspinatus II");

				Write("Right Supraspinatus");
				Write("Right Middle Deltoid");
				Write("Right Anterior Deltoid");
				Write("Right Posterior Deltoid");

				Write("Right Coracobrachialis");
				Write("Right Upper Latissimus Doris");
				Write("Right Lower Latissimus Doris");
				Write("Right Pectoralis Major I");
				Write("Right Pectoralis Major II");

				Write("Right Trapesius I");
				Write("Right Trapesius II");
				Write("Right Trapesius III");
				Write("Right Trapesius IV");
				Write("Right Pectoralis Minor");

				Write("Right Levator Scapulae");
				Write("Right Rhomboid Minor");
				Write("Right Rhomboid Major");

				Write("Right Upper Serratus Anterior");
				Write("Right Middle Serratus Anterior");
				Write("Right Lower Serratus Anterior");

				Write("Right Sternohyoid");
				Write("Right Omohymoid");
				Write("Right Subclavius");
				Write("Right Sternocleidomastoid");

				Write("Right Coracohumeral");
				Write("Right Conoid");
				Write("Right Trapezoid");

				Write("Right Sternoclavicular X");
				Write("Right Sternoclavicular Y");
				Write("Right Sternoclavicular Z");

				Write("Right Acromioclavicular X");
				Write("Right Acromioclavicular Y");
				Write("Right Acromioclavicular Z");

				Write("Right Glenohumeral X");
				Write("Right Glenohumeral Y");
				Write("Right Glenohumeral Z");

				Write("Right Superior Angle");
				Write("Right Inferior Angle");

				Write("Right GSC 1");
				Write("Right GSC 2");
				Write("Right GSC 3");
				Write("Right GSC 4");
				Write("Right GSC 5");
				Write("Right GSC 6");
				Write("Right GSC 7");
				Write("Right GSC 8");
				
				Write("Left Biceps I");
				Write("Left Biceps II");
				Write("Left Brachialis");
				Write("Left Brachioradialis");

				Write("Left Triceps I");
				Write("Left Triceps II");
				Write("Left Triceps III");

				Write("Left Teres Major");
				Write("Left Subscapularis I");
				Write("Left Subscapularis II");
				Write("Left Subscapularis III");

				Write("Left Teres Minor");
				Write("Left Infraspinatus I");
				Write("Left Infraspinatus II");

				Write("Left Supraspinatus");
				Write("Left Middle Deltoid");
				Write("Left Anterior Deltoid");
				Write("Left Posterior Deltoid");

				Write("Left Coracobrachialis");
				Write("Left Upper Latissimus Doris");
				Write("Left Lower Latissimus Doris");
				Write("Left Pectoralis Major I");
				Write("Left Pectoralis Major II");

				Write("Left Trapesius I");
				Write("Left Trapesius II");
				Write("Left Trapesius III");
				Write("Left Trapesius IV");
				Write("Left Pectoralis Minor");

				Write("Left Levator Scapulae");
				Write("Left Rhomboid Minor");
				Write("Left Rhomboid Major");

				Write("Left Upper Serratus Anterior");
				Write("Left Middle Serratus Anterior");
				Write("Left Lower Serratus Anterior");

				Write("Left Sternohyoid");
				Write("Left Omohymoid");
				Write("Left Subclavius");
				Write("Left Sternocleidomastoid");

				Write("Left Coracohumeral");
				Write("Left Conoid");
				Write("Left Trapezoid");

				Write("Left Sternoclavicular X");
				Write("Left Sternoclavicular Y");
				Write("Left Sternoclavicular Z");

				Write("Left Acromioclavicular X");
				Write("Left Acromioclavicular Y");
				Write("Left Acromioclavicular Z");

				Write("Left Glenohumeral X");
				Write("Left Glenohumeral Y");
				Write("Left Glenohumeral Z");

				Write("Left Superior Angle");
				Write("Left Inferior Angle");

				Write("Left GSC 1");
				Write("Left GSC 2");
				Write("Left GSC 3");
				Write("Left GSC 4");
				Write("Left GSC 5");
				Write("Left GSC 6");
				Write("Left GSC 7");
				Write("Left GSC 8");

				Write("Muscle force model data start");

				Write("Right elbow aeq variables start");			
				for ( int i = 0; i < 60; i++ ) {
					Write(i+1);
				}
				
				Write("Right humeral force aeq variables start");
				std::string base = "";
				std::string header = "";
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				
				Write("Right humeral moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				
				Write("Right scapular force aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				
				Write("Right scapular moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				
				Write("Right calvicular force aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				
				Write("Right calvicular moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				
				Write("Right glenoid stability aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				
				Write("Right shoulder torque variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(i+1);
				}
				
				Write("Right elbow torque variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(i+1);
				}
				
				Write("Right shoulder reaction variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(i+1);
				}
				
				Write("Right rotation matrix from global to forearm coordinates");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 3; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				
				Write("Left elbow aeq variables start");
				for ( int i = 0; i < 60; i++ ) {
					Write(i+1);
				}
				
				Write("Left humeral force aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				
				Write("Left humeral moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				
				Write("Left scapular force aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				
				Write("Left scapular moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				
				Write("Left calvicular force aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				
				Write("Left calvicular moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				
				Write("Left glenoid stability aeq variables start");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 60; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
				
				Write("Left shoulder torque variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(i+1);
				}
				
				Write("Left elbow torque variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(i+1);
				}
				
				Write("Left shoulder reaction variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(i+1);
				}
				
				Write("Left rotation matrix from global to forearm coordinates");
				for (int i = 0; i < 3; i++) {
					if ( i == 0) base = "X ";
					else if ( i == 1) base = "Y ";
					else base = "Z ";
					
					for (int j = 0; j < 3; j++) {
						header = base + std::to_string(j+1);
						Write(header.c_str());
					}
				}
			} 
			else {
				Write("shdr anls");

				for(int i = 0; i < 38; ++i) {
					Write(shoulder_analysis.SX[analysis_order[i]]);
				}

				for(int i = 0; i < 3; ++i) {
					Write(shoulder_analysis.LGT[LGT_order[i]]);
				}

				for(int i = 0; i < 3; ++i) {
					Write(shoulder_analysis.JCFX[JCF_order[i]]);
					Write(shoulder_analysis.JCFY[JCF_order[i]]);
					Write(shoulder_analysis.JCFZ[JCF_order[i]]);
				}

				for(int i = 0; i < 10; ++i) {
					Write(shoulder_analysis.GSC[GSC_order[i]]);
				}

				for(int i = 0; i < 38; ++i) {
					Write(shoulder_analysis.SX[analysis_order[i+38]]);
				}

				for(int i = 0; i < 3; ++i) {
					Write(shoulder_analysis.LGT[LGT_order[i+3]]);
				}

				for(int i = 0; i < 3; ++i) {
					Write(shoulder_analysis.JCFX[JCF_order[i+3]]);
					Write(shoulder_analysis.JCFY[JCF_order[i+3]]);
					Write(shoulder_analysis.JCFZ[JCF_order[i+3]]);
				}

				for(int i = 0; i < 10; ++i) {
					Write(shoulder_analysis.GSC[GSC_order[i+10]]);
				}

				Write("muscle force model");
				Write("Right elbow aeq variables start");
				for ( int i = 0; i < 60; i++ ) {
					Write(muscle_force.ln1[0][i]);
				}
				Write("Right humeral force aeq variables start");
				std::string base = "";
				std::string header = "";
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln2to4[i][j]);
					}
				}
				Write("Right humeral moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln5to7[i][j]);
					}
				}
				Write("Right scapular force aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln8to10[i][j]);
					}
				}
				Write("Right scapular moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln11to13[i][j]);
					}
				}
				Write("Right calvicular force aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln14to16[i][j]);
					}
				}
				Write("Right calvicular moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln17to19[i][j]);
					}
				}
				Write("Right glenoid stability aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln20to22[i][j]);
					}
				}
				Write("Right shoulder torque variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(muscle_force.MSHTorque[i]);
				}
				Write("Right elbow torque variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(muscle_force.MELTorque[i]);
				}
				Write("Right shoulder reaction variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(muscle_force.MSHReact[i]);
				}
				Write("Right rotation matrix from global to forearm coordinates");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						Write(muscle_force.RGF[i][j]);
					}
				}
				Write("Left elbow aeq variables start");
				for ( int i = 0; i < 60; i++ ) {
					Write(muscle_force.ln1[1][i]);
				}
				Write("Left humeral force aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln2to4[i+3][j]);
					}
				}
				Write("Left humeral moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln5to7[i+3][j]);
					}
				}
				Write("Left scapular force aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln8to10[i+3][j]);
					}
				}
				Write("Left scapular moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln11to13[i+3][j]);
					}
				}
				Write("Left calvicular force aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln14to16[i+3][j]);
					}
				}
				Write("Left calvicular moment aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln17to19[i+3][j]);
					}
				}
				Write("Left glenoid stability aeq variables start");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 60; j++) {
						Write(muscle_force.ln20to22[i+3][j]);
					}
				}
				Write("Left shoulder torque variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(muscle_force.MSHTorque[i+3]);
				}
				Write("Left elbow torque variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(muscle_force.MELTorque[i+3]);
				}
				Write("Left shoulder reaction variables start");
				for ( int i = 0; i < 3; i++ ) {
					Write(muscle_force.MSHReact[i+3]);
				}
				Write("Left rotation matrix from global to forearm coordinates");
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						Write(muscle_force.RGF[i+3][j]);
					}
				}
			}
		} 
		
		//--- Write Shoulder Geometry ---

		if(mShoulderGeometry)
		{
			int geometry_order[NUM_GEOMETRY_HINGES] =	{	SG_FirstThoracicVertebraS, SG_TwelvthThoracicVertebraI,
															SG_FirstThoracicVertebraM, SG_RightSternoclavicularJoint,
															SG_RightACJ, SG_RightScapulaAngle,
															SG_RightScapularSpine, SG_RightHumeralHead,
															SG_RightElbowJoint, SG_RightWristJoint,
															SG_RightSJCTJoint, SG_RightACRTJoint,
															SG_RightC7T1Joint, SG_RightL5S1Joint, 
															SG_LeftSternoclavicularJoint,
															SG_LeftACJ, SG_LeftScapulaAngle,
															SG_LeftScapularSpine, SG_LeftHumeralHead,
															SG_LeftElbowJoint, SG_LeftWristJoint,
															SG_LeftSJCTJoint, SG_LeftACRTJoint,
															SG_LeftC7T1Joint, SG_LeftL5S1Joint };

			if(pass == HEADERS) {
				Write("Shoulder Geometry Starts");

				Write("1st Thoracic Vertebra (S) X");
				Write("1st Thoracic Vertebra (S) Y");
				Write("1st Thoracic Vertebra (S) Z");

				Write("12th Thoracic Vertebra (I) X");
				Write("12th Thoracic Vertebra (I) Y");
				Write("12th Thoracic Vertebra (I) Z");

				Write("1st Thoracic Vertebra (M) X");
				Write("1st Thoracic Vertebra (M) Y");
				Write("1st Thoracic Vertebra (M) Z");

				Write("Right Sternoclavicular Joint X");
				Write("Right Sternoclavicular Joint Y");
				Write("Right Sternoclavicular Joint Z");

				Write("Right Acriomoclavicular Joint X");
				Write("Right Acriomoclavicular Joint Y");
				Write("Right Acriomoclavicular Joint Z");

				Write("Right Scapula Angle X");
				Write("Right Scapula Angle Y");
				Write("Right Scapula Angle Z");

				Write("Right Scapula Spine X");
				Write("Right Scapula Spine Y");
				Write("Right Scapula Spine Z");

				Write("Right Humeral Head X");
				Write("Right Humeral Head Y");
				Write("Right Humeral Head Z");

				Write("Right Elbow Joint X");
				Write("Right Elbow Joint Y");
				Write("Right Elbow Joint Z");

				Write("Right Wrist Joint X");
				Write("Right Wrist Joint Y");
				Write("Right Wrist Joint Z");

				Write("Right Shoulder Joint Center X");
				Write("Right Shoulder Joint Center Y");
				Write("Right Shoulder Joint Center Z");

				Write("Right Acromion X");
				Write("Right Acromion Y");
				Write("Right Acromion Z");

				Write("Right C7T1 Vertibral Joint X");
				Write("Right C7T1 Vertibral Joint Y");
				Write("Right C7T1 Vertibral Joint Z");

				Write("Right L5S1 Vertibral Joint X");
				Write("Right L5S1 Vertibral Joint Y");
				Write("Right L5S1 Vertibral Joint Z");	

				Write("Left Sternoclavicular Joint X");
				Write("Left Sternoclavicular Joint Y");
				Write("Left Sternoclavicular Joint Z");

				Write("Left Acriomoclavicular Joint X");
				Write("Left Acriomoclavicular Joint Y");
				Write("Left Acriomoclavicular Z");

				Write("Left Scapula Angle X");
				Write("Left Scapula Angle Y");
				Write("Left Scapula Angle Z");

				Write("Left Scapula Spine X");
				Write("Left Scapula Spine Y");
				Write("Left Scapula Spine Z");

				Write("Left Humeral Head X");
				Write("Left Humeral Head Y");
				Write("Left Humeral Head Z");

				Write("Left Elbow Joint X");
				Write("Left Elbow Joint Y");
				Write("Left Elbow Joint Z");

				Write("Left Wrist Joint X");
				Write("Left Wrist Joint Y");
				Write("Left Wrist Joint Z");

				Write("Left Shoulder Joint Center X");
				Write("Left Shoulder Joint Center Y");
				Write("Left Shoulder Joint Center Z");

				Write("Left Acromion X");
				Write("Left Acromion Y");
				Write("Left Acromion Z");

				Write("Left C7T1 Vertibral Joint X");
				Write("Left C7T1 Vertibral Joint Y");
				Write("Left C7T1 Vertibral Joint Z");

				Write("Left L5S1 Vertibral Joint X");
				Write("Left L5S1 Vertibral Joint Y");
				Write("Left L5S1 Vertibral Joint Z");				
			}
			else {
				Write("shdr gmty");

				for (int i = 0; i < NUM_GEOMETRY_HINGES; ++i) {
					Write(shoulder_geometry.XMetric[geometry_order[i]]);
					Write(shoulder_geometry.YMetric[geometry_order[i]]);
					Write(shoulder_geometry.ZMetric[geometry_order[i]]);
				}
			}
		}

		EndL();
	}
	return TRUE;
}

void ExportSummary::Write(const char *aCharPtr) {
	if(!newLine) (*mFileStreamPtr) << mDelimiter;
    (*mFileStreamPtr) << aCharPtr;
	newLine = false;
}

void ExportSummary::Write(int aInt) {
	if(!newLine) (*mFileStreamPtr) << mDelimiter;
    (*mFileStreamPtr) << aInt;
	newLine = false;
}

void ExportSummary::Write(float aFloat) {
	if(!newLine) (*mFileStreamPtr) << mDelimiter;
    (*mFileStreamPtr) << aFloat;
	newLine = false;
}

void ExportSummary::Write(double aDouble) {
	if(!newLine) (*mFileStreamPtr) << mDelimiter;
    (*mFileStreamPtr) << aDouble;
	newLine = false;
}

void ExportSummary::EndL() {
	(*mFileStreamPtr) << std::endl;
	newLine = true;
}