#include "AngleCue.h"
#include "Skeleton.hpp"
#include "JointAngles.hpp"

AngleCueData::AngleCueData() : angleID((E_Angles)-1) {}

AngleCueData AngleCueData::Build(E_Angles AI) {
	switch(AI) {
		case A_ForearmLH:
			return AngleCueData(A_ForearmLH, JT_LELBOW, JT_LWRIST, AT_LimbHL, A_ForearmLV);
			break;
		case A_ForearmLV:
			return AngleCueData(A_ForearmLV, JT_LELBOW, JT_LWRIST, AT_LimbVL, A_ForearmLH);
			break;
		case A_ForearmRH:
			return AngleCueData(A_ForearmRH, JT_RELBOW, JT_RWRIST, AT_LimbHR, A_ForearmRV);
			break;
		case A_ForearmRV:
			return AngleCueData(A_ForearmRV, JT_RELBOW, JT_RWRIST, AT_LimbVR, A_ForearmRH);
			break;
		case A_UpperarmLH:
			return AngleCueData(A_UpperarmLH, JT_LSHOULDER, JT_LELBOW, AT_LimbHL, A_UpperarmLV);
			break;
		case A_UpperarmLV:
			return AngleCueData(A_UpperarmLV, JT_LSHOULDER, JT_LELBOW, AT_LimbVL, A_UpperarmLH);
			break;
		case A_UpperarmRH:
			return AngleCueData(A_UpperarmRH, JT_RSHOULDER, JT_RELBOW, AT_LimbHR, A_UpperarmRV);
			break;
		case A_UpperarmRV:
			return AngleCueData(A_UpperarmRV, JT_RSHOULDER, JT_RELBOW, AT_LimbVR, A_UpperarmRH);
			break;
		case A_UpperlegLH:
			return AngleCueData(A_UpperlegLH, JT_LHIP, JT_LKNEE, AT_LimbHL, A_UpperlegLV);
			break;
		case A_UpperlegLV:
			return AngleCueData(A_UpperlegLV, JT_LHIP, JT_LKNEE, AT_LimbVL, A_UpperlegLH);
			break;
		case A_UpperlegRH:
			return AngleCueData(A_UpperlegRH, JT_RHIP, JT_RKNEE, AT_LimbHR, A_UpperlegRV);
			break;
		case A_UpperlegRV:
			return AngleCueData(A_UpperlegRV, JT_RHIP, JT_RKNEE, AT_LimbVR, A_UpperlegRH);
			break;
		case A_LowerlegLV:
			return AngleCueData(A_LowerlegLV, JT_LKNEE, JT_LANKLE, AT_LimbVL, A_LowerlegLH);
			break;
		case A_LowerlegRV:
			return AngleCueData(A_LowerlegRV, JT_RKNEE, JT_RANKLE, AT_LimbVR, A_LowerlegRH);
			break;
		case A_FootLV:
			return AngleCueData(A_FootLV, JT_LHEEL, JT_LBALL, AT_LimbVL, A_FootLH);
			break;
		case A_FootRV:
			return AngleCueData(A_FootRV, JT_RHEEL, JT_RBALL, AT_LimbVR, A_FootRH);
			break;
		case A_TrunkFlexion:
			return AngleCueData(A_TrunkFlexion, JT_HIP_CENTER, JT_C7T1, AT_TrunkF);
			break;
		case A_TrunkAxialRotation:
			break;
		case A_TrunkLateralBending:
			return AngleCueData(A_TrunkLateralBending, JT_HIP_CENTER, JT_C7T1, AT_TrunkLB);
			break;
		case A_PelvisLateralBending:
			return AngleCueData(A_PelvisLateralBending, JT_HIP_CENTER, JT_L5S1_TEMP, AT_PelvisLB);
			break;
		case A_C4C7Flexion:
			break;
		case A_C4C7AxialRotation:
			break;
		case A_C4C7LateralBending:
			break;
		case A_C1C3_LateralBending:
			break;
		case A_C1C3_Flexion:
			break;
		case A_C1C3_AxialRotation:
			break;
		case A_C1_LateralBending: //Add C1
			break;
		case A_C1_Flexion:        //Add C1
			break;
		case A_C1_AxialRotation:  //Add C1
			break;
		case A_ClavicleLH:
			break;
		case A_ClavicleLV:
			break;
		case A_ClavicleRH:
			break;
		case A_ClavicleRV:
			break;
		case A_LowerlegLH:
			return AngleCueData(A_LowerlegLH, JT_LKNEE, JT_LANKLE, AT_LimbHL, A_LowerlegLV);
			break;
		case A_LowerlegRH:
			return AngleCueData(A_LowerlegRH, JT_RKNEE, JT_RANKLE, AT_LimbHR, A_LowerlegRV);
			break;
		case A_FootLH:
			return AngleCueData(A_FootLH, JT_LHEEL, JT_LBALL, AT_LimbHL, A_FootLV);
			break;
		case A_FootRH:
			return AngleCueData(A_FootRH, JT_RHEEL, JT_RBALL, AT_LimbHR, A_FootRV);
			break;
		default:
			return AngleCueData();
			break;
	}
	return AngleCueData();
}
AngleCueData::AngleCueData(E_Angles AI, JointID P, JointID D, AngleType AT, E_Angles OI) :
		angleID(AI), otherID(OI), proximal(P), distal(D), angleType(AT) {}