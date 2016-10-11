//---------------------------------------------------------------------------
//--- COM Interface
//---------------------------------------------------------------------------

//!!!Design Note: Much of what follows is duplicated in the individual report
//dialog classes. This duplication can be reduced or eliminated by redesigning
//the C4E Analysis class to include accessors that compute the derived results
//found here. Done properly, this would be a significant effort and exceeds
//the scope of the present work.


#include "stdafx.h"
#include "AppVersion.hpp"

#include "hom.h"
#include "hom_doc.h"	// Necessary
#include "length.hpp"
#include "force.hpp"
#include "torque.hpp"

//#include "units.hpp"
//#include "SSPPApp_h.h"

#include <string>

using std::ifstream;
using std::ofstream;
using std::string;


IMPLEMENT_OLECREATE(C_Hom_Doc, "Um3DSSPP.IUm3DSSPP", 0x8f41a650, 0xf62a, 0x11d1, 0x94, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0)

//--- Globals defined and computed in FindPoi.cpp --- 

//extern double gPelvicAngle;

/* 
*****************************************************************************
The order of the folowing dispatch map is EXTREMELY important.  This is how
OLE/COM assigns ID numbers to the specific tasks.  Do _NOT_ manually edit
these.  If a reordering occurs, the COMFunctionList.cpp in the comClientApp
project must be updated to reflect this reordering, or the COM client will
NOT function.
*****************************************************************************
*/

BEGIN_DISPATCH_MAP(C_Hom_Doc, CDocument)
	//{{AFX_DISPATCH_MAP(C_Hom_Doc)
	DISP_FUNCTION(C_Hom_Doc, "HelloWorld", HelloWorld, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(C_Hom_Doc, "SetTaskDescription", SetTaskDescription, VT_BOOL, VTS_I2 VTS_PBSTR VTS_PBSTR VTS_PBSTR)
	DISP_FUNCTION(C_Hom_Doc, "SetAnthropometry", SetAnthropometry, VT_BOOL, VTS_I2 VTS_I2 VTS_R4 VTS_R4)
	DISP_FUNCTION(C_Hom_Doc, "SetPopulationDefault", SetPopulationDefault, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(C_Hom_Doc, "SetZeroGravity", SetZeroGravity, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(C_Hom_Doc, "PosturePredict", PosturePredict, VT_BOOL, VTS_I2 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_R4)
	DISP_FUNCTION(C_Hom_Doc, "SetPopulationFromFile", SetPopulationFromFile, VT_BOOL, VTS_PBSTR VTS_PBSTR)
	DISP_FUNCTION(C_Hom_Doc, "SetBodySegmentAngles", SetBodySegmentAngles, VT_BOOL, VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2)
	DISP_FUNCTION(C_Hom_Doc, "SetHandLoads", SetHandLoads, VT_BOOL, VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8)
	DISP_FUNCTION(C_Hom_Doc, "LastErrorMessage", LastErrorMessage, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(C_Hom_Doc, "GetAnalysisSummaryA", GetAnalysisSummaryA, VT_BOOL, VTS_PI2 VTS_PBSTR VTS_PI2 VTS_PBSTR VTS_PI2 VTS_PBSTR VTS_PI2 VTS_PBSTR VTS_PI2 VTS_PBSTR VTS_PI2 VTS_PBSTR VTS_PI2 VTS_PBSTR)
	DISP_FUNCTION(C_Hom_Doc, "GetAnalysisSummaryB", GetAnalysisSummaryB, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PI2 VTS_PI2 VTS_PBSTR VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetAnthropometryA", GetAnthropometryA, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetAnthropometryB", GetAnthropometryB, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetAnthropometryC", GetAnthropometryC, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetJointMomentsA", GetJointMomentsA, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetJointMomentsB", GetJointMomentsB, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetJointMomentsC", GetJointMomentsC, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetJointsA", GetJointsA, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetJointsB", GetJointsB, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetJointsC", GetJointsC, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetLowBackA", GetLowBackA, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetLowBackB", GetLowBackB, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetLowBackC", GetLowBackC, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetLowBackD", GetLowBackD, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetLowBackE", GetLowBackE, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetLowBackF", GetLowBackF, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetPostureA", GetPostureA, VT_BOOL, VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2)
	DISP_FUNCTION(C_Hom_Doc, "GetPostureB", GetPostureB, VT_BOOL, VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2)
	DISP_FUNCTION(C_Hom_Doc, "GetSagittal", GetSagittal, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PI2)
	DISP_FUNCTION(C_Hom_Doc, "GetSpinalA", GetSpinalA, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetSpinalB", GetSpinalB, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetSpinalC", GetSpinalC, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetStrengthA", GetStrengthA, VT_BOOL, VTS_PR4 VTS_PBSTR VTS_PR4 VTS_PR4 VTS_PI2 VTS_PR4 VTS_PBSTR VTS_PR4 VTS_PR4 VTS_PI2)
	DISP_FUNCTION(C_Hom_Doc, "GetStrengthB", GetStrengthB, VT_BOOL, VTS_PR4 VTS_PBSTR VTS_PR4 VTS_PR4 VTS_PI2 VTS_PR4 VTS_PBSTR VTS_PR4 VTS_PR4 VTS_PI2)
	DISP_FUNCTION(C_Hom_Doc, "GetStrengthC", GetStrengthC, VT_BOOL, VTS_PR4 VTS_PBSTR VTS_PR4 VTS_PR4 VTS_PI2 VTS_PR4 VTS_PBSTR VTS_PR4 VTS_PR4 VTS_PI2)
	DISP_FUNCTION(C_Hom_Doc, "GetStrengthD", GetStrengthD, VT_BOOL, VTS_PR4 VTS_PBSTR VTS_PR4 VTS_PR4 VTS_PI2 VTS_PR4 VTS_PBSTR VTS_PR4 VTS_PR4 VTS_PI2)
	DISP_FUNCTION(C_Hom_Doc, "GetStrengthE", GetStrengthE, VT_BOOL, VTS_PR4 VTS_PBSTR VTS_PR4 VTS_PR4 VTS_PI2 VTS_PR4 VTS_PBSTR VTS_PR4 VTS_PR4 VTS_PI2 VTS_PR4 VTS_PBSTR VTS_PR4 VTS_PR4 VTS_PI2)
	DISP_FUNCTION(C_Hom_Doc, "GetStrengthF", GetStrengthF, VT_BOOL, VTS_PR4 VTS_PBSTR VTS_PR4 VTS_PR4 VTS_PI2 VTS_PR4 VTS_PBSTR VTS_PR4 VTS_PR4 VTS_PI2)
	DISP_FUNCTION(C_Hom_Doc, "GetStrengthG", GetStrengthG, VT_BOOL, VTS_PR4 VTS_PBSTR VTS_PR4 VTS_PR4 VTS_PI2 VTS_PR4 VTS_PBSTR VTS_PR4 VTS_PR4 VTS_PI2)
	DISP_FUNCTION(C_Hom_Doc, "GetStrengthH", GetStrengthH, VT_BOOL, VTS_PR4 VTS_PBSTR VTS_PR4 VTS_PR4 VTS_PI2 VTS_PR4 VTS_PBSTR VTS_PR4 VTS_PR4 VTS_PI2)
	DISP_FUNCTION(C_Hom_Doc, "GetStrengthVectorsA", GetStrengthVectorsA, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetStrengthVectorsB", GetStrengthVectorsB, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetStrengthVectorsC", GetStrengthVectorsC, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetStrengthVectorsD", GetStrengthVectorsD, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetStrengthVectorsE", GetStrengthVectorsE, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetStrengthVectorsF", GetStrengthVectorsF, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetStrengthVectorsG", GetStrengthVectorsG, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetStrengthVectorsH", GetStrengthVectorsH, VT_BOOL, VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4)
	DISP_FUNCTION(C_Hom_Doc, "GetTaskInputSummaryA", GetTaskInputSummaryA, VT_BOOL, VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2)
	DISP_FUNCTION(C_Hom_Doc, "GetTaskInputSummaryB", GetTaskInputSummaryB, VT_BOOL, VTS_PBSTR VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4 VTS_PI2 VTS_PI2 VTS_PR4 VTS_PI2 VTS_PI2 VTS_PR4)
	//DISP_FUNCTION_ID(C_Hom_Doc, "GetFatigueReport", dispidGetFatigueReport, GetFatigueReport, VT_BOOL, VTS_BOOL VTS_I2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2)
	DISP_FUNCTION(C_Hom_Doc, "GetFatigueReport", GetFatigueReport, VT_BOOL, VTS_BOOL VTS_I2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI2)
	//DISP_FUNCTION_ID(C_Hom_Doc, "IsRegistered", dispidIsRegistered, IsRegistered, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(C_Hom_Doc, "IsRegistered", IsRegistered, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(C_Hom_Doc, "SetAngle", SetAngle, VT_BOOL, VTS_I4 VTS_R8)
	DISP_FUNCTION(C_Hom_Doc, "GetAngle", GetAngle, VT_BOOL, VTS_I4 VTS_PR8)
	DISP_FUNCTION(C_Hom_Doc, "SetBodySegmentAnglesWithLegs", SetBodySegmentAnglesWithLegs, VT_BOOL, VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IUm3DSSPP to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {8F41A647-F62A-11D1-9480-000000000000}
//static const IID IID_IUm3DSSPP = { 0x8f41a651, 0xf62a, 0x11d1, { 0x94, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } };
static const IID IID_IUm3DSSPP = { 0x8f41a647, 0xf62a, 0x11d1, { 0x94, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } };

BEGIN_INTERFACE_MAP(C_Hom_Doc, CDocument)
	INTERFACE_PART(C_Hom_Doc, IID_IUm3DSSPP, Dispatch)
END_INTERFACE_MAP()

BSTR C_Hom_Doc::HelloWorld()
{
	CString strResult( "3DSSPP " szVERSION " says Hello World!");
	return strResult.AllocSysString();
}

// TODO figure out what units we should be returning from all of these
//---------------------------------------------------------------------------
BOOL C_Hom_Doc::SetTaskDescription(	short aUnits,
												BSTR FAR* aTask,
												BSTR FAR* aAnalyst,
												BSTR FAR* aComments	)
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   //Convert BSTR to CString

   CString   lTask( * aTask );
   CString   lAnalyst( * aAnalyst );
   CString   lComments( * aComments );
	String llTask(lTask);
	String llAnalyst(lAnalyst);
	String llComments(lComments);

   //Attempt to assign values to a temporary description

   TaskDescription lDescription;

	// NOTE Assign method of TaskDescription was removed
	lDescription.Units(aUnits);
	lDescription.Task(llTask);
	lDescription.Analyst(llAnalyst);
	lDescription.Comments(llComments);
	//return true;

	//if(lDescription.Assign(aUnits,(LPCSTR)lTask,(LPCSTR)lAnalyst,(LPCSTR)lComments))
	if(true)
   {
      //succeeded; assign to this object
      this->SetDescription( lDescription );
      return   TRUE;
   }

   else //assign failed
   {
      mCOMLastErrorMessage = lDescription.LastErrorMessage();
      return   FALSE;
   }
}

BOOL C_Hom_Doc::PosturePredict(short handOrientation, float LH, float LV, float LL, float RH, float RV, float RL)
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

	// NOTE hand orientation setting has changed
	//setHand_Orientation(E_Hand_Orientation(handOrientation));
	this->getTask().Put_Hand_Orientation((E_Hand_Orientation)handOrientation);

	//HandOrientationSync();

/*	Length tmpLength;
	tmpLength = LH; setHandLocLH(tmpLength);
	tmpLength = LL; setHandLocLL(tmpLength);
	tmpLength = LV; setHandLocLV(tmpLength);

	tmpLength = RH; setHandLocRH(tmpLength);
	tmpLength = RL; setHandLocRL(tmpLength);
	tmpLength = RV; setHandLocRV(tmpLength);


	Flt_3D_Point   lLeftHand;
	Flt_3D_Point   lRightHand;

	lLeftHand( C_X ) = getHandLocLH().ValueIn(MetricCns);
	lLeftHand( C_Y ) = getHandLocLL().ValueIn(MetricCns);
	lLeftHand( C_Z ) = getHandLocLV().ValueIn(MetricCns);

	lRightHand( C_X ) = getHandLocRH().ValueIn(MetricCns);
	lRightHand( C_Y ) = getHandLocRL().ValueIn(MetricCns);
	lRightHand( C_Z ) = getHandLocRV().ValueIn(MetricCns);*/

	// TODO it looks like the 509 implementation had lateral being x and horizontal being y,
	// but it should definitely be the other way around
	Vector3 lLeftHand(LL, LH, LV);
	Vector3 lRightHand(RL, RH, RV);

    if( PredictPosture( lLeftHand, lRightHand ) )
    {
		this->Analyze();
		return TRUE;
	}
	else
	{
		mCOMLastErrorMessage = " Posture Predict: Posture not attainable.";
		return FALSE;
	}
}
//---------------------------------------------------------------------------
BOOL C_Hom_Doc::SetAnthropometry(short aSex,
                                 short aPercentile,
                                 float aHeight,
                                 float aWeight)
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   //Attempt to assign values to a temporary anthropometry

   Anthropometry lAnthropometry;

   // NOTE demonstration mode no longer exists
   /*if (Get_App_Ptr()->Is_Demonstration())
   {
       mCOMLastErrorMessage = " Anthropometry: DEMO MODE! Forced to short male";
       Get_App_Ptr()->Get_Document_Ptr()->getAnthropometryRef().Sex(0);
       Get_App_Ptr()->Get_Document_Ptr()->getAnthropometryRef().Percentile(1);
       Get_App_Ptr()->Get_Document_Ptr()->StatureSync();
       Get_App_Ptr()->Get_Document_Ptr()->SexSync();
       return false;
   }*/

   if( lAnthropometry.Assign( aSex, aPercentile, aHeight, aWeight ) )
   {
      //succeeded; assign to this object
      this->setAnthropometry( lAnthropometry );
	  this->Analyze();
      return   TRUE;
   }

   else //assign failed
   {
      mCOMLastErrorMessage = lAnthropometry.LastErrorMessage();
      return   FALSE;
   }
}

BOOL C_Hom_Doc::SetBodySegmentAngles(	short aForearmLH, 
													short aForearmLV, 
													short aForearmRH, 
													short aForearmRV, 
												   
													short aUpperarmLH, 
													short aUpperarmLV, 
													short aUpperarmRH, 
													short aUpperarmRV, 
												   
													short aUpperlegLV, 
													short aUpperlegRV, 
												   
													short aLowerlegLV, 
													short aLowerlegRV, 
												   
													short aTrunkFlexion, 
													short aTrunkAxialRotation, 
													short aTrunkLateralBending	)
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   //Attempt to assign values to a temporary joint angles object

   JointAngles   lJointAngles;

   double newUpperlegLV = aUpperlegLV;
   double newUpperlegLH = (aUpperlegLV >= -90 && aUpperlegLV <=90) ? 90 : 90;
   double newUpperlegRV = aUpperlegRV;
   double newUpperlegRH = (aUpperlegRV >= -90 && aUpperlegRV <=90) ? 90 : 90;
   double newLowerlegLV = aLowerlegLV;
   double newLowerlegLH = (aLowerlegLV >= -90 && aLowerlegLV <=90) ? 90 : 90;
   double newLowerlegRV = aLowerlegRV;
   double newLowerlegRH = (aLowerlegRV >= -90 && aLowerlegRV <=90) ? 90 : 90;

   // take leg angles in new convention
   /*JointAngles::swapConventions(newUpperlegLV, newUpperlegLH);
   JointAngles::swapConventions(newUpperlegRV, newUpperlegRH);
   JointAngles::swapConventions(newLowerlegLV, newLowerlegLH);
   JointAngles::swapConventions(newLowerlegRV, newLowerlegRH);*/

   // NOTE
   // JointAngles::Assign takes more angles now
   // more importantly, the leg angles definitions changed
   lJointAngles.Assign
         ( 
            aForearmLH, 
            aForearmLV, 
            aForearmRH, 
            aForearmRV, 
   
            aUpperarmLH, 
            aUpperarmLV, 
            aUpperarmRH, 
            aUpperarmRV, 
   
			newUpperlegLH, // upperleg lh
            newUpperlegLV,
			newUpperlegRH, // upperleg rh
            newUpperlegRV, 
   
			newLowerlegLH, // lowerleg lh
            newLowerlegLV,
			newLowerlegRH, // lowerleg rh
            newLowerlegRV, 
   
            aTrunkFlexion, 
            aTrunkAxialRotation, 
            aTrunkLateralBending,

			0,  //double aSCJH,	// Todo - when SCJ/clavivles added to 3dsspp,activate
			0,  //double aSCJV,

			lJointAngles[A_ClavicleLH],//0,  //double aClavicleLH,
			lJointAngles[A_ClavicleLV],//0,  //double aClavicleLV,
			lJointAngles[A_ClavicleRH],//0,  //double aClavicleRH,
			lJointAngles[A_ClavicleRV],//0,   //double aClavicleRV

			90, // footlh
			0, // footlv
			90, // footrh
			0, // footrv

			aForearmLH, // handlh
			aForearmLV, // handlv
			0, // handlrot
			aForearmRH, // handrh
			aForearmRV, // handrv
			0,

			aTrunkLateralBending,
			aTrunkFlexion,
			0,
			aTrunkLateralBending,
			aTrunkFlexion,
			0,
			0


         );
	if(lJointAngles.Validate()  )
   {
      //	Succeeded - assign to this object.  "true" means
		//		we clear the undo queue - "undo" does not apply
		//		to COM changes
      //this->SetJointAngles( lJointAngles, true );
	   this->GetSkeleton()->SetAngles(lJointAngles);
      
      //MPM - Update the skeleton with the passed in values
      //this->SkeletonAnglesSync();

      /*if( Get_App_Ptr()->TwoDModeOnly() )
         this->Set2D();*/

	   this->Analyze();

      return   TRUE;
   }

   else //assign failed
   {
      //mCOMLastErrorMessage = lJointAngles.LastErrorMessage();
	   mCOMLastErrorMessage = lJointAngles.getLastError();
      return   FALSE;
   }
}

//---------------------------------------------------------------------------
BOOL C_Hom_Doc::SetHandLoads(	double aLeftMagnitude, 
										double aLeftHorizontalAngle, 
										double aLeftVerticalAngle, 
										double aRightMagnitude, 
										double aRightHorizontalAngle, 
										double aRightVerticalAngle	)
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   //Attempt to assign values to a temporary handloads object

   HandLoads lHandLoads;

   if (true)
	   // NOTE HandLoads isn't used anymore and doesn't have Assign
	   /* 
         lHandLoads.Assign
            ( 
               aLeftMagnitude, 
               aLeftHorizontalAngle, 
               aLeftVerticalAngle, 
               aRightMagnitude, 
               aRightHorizontalAngle, 
               aRightVerticalAngle
            ) 
      )*/
   {
      //succeeded; assign to this object
      //this->setHandLoads( lHandLoads );
		Vector3 RHandForce, LHandForce;
		RHandForce[0] = cos(aRightVerticalAngle*M_PI/180.0) * cos(aRightHorizontalAngle*M_PI/180.0);
		RHandForce[1] = cos(aRightVerticalAngle*M_PI/180.0) * sin(aRightHorizontalAngle*M_PI/180.0);
		RHandForce[2] = sin(aRightVerticalAngle*M_PI/180.0);

		LHandForce[0] = cos(aLeftVerticalAngle*M_PI/180.0) * cos(aLeftHorizontalAngle*M_PI/180.0);
		LHandForce[1] = cos(aLeftVerticalAngle*M_PI/180.0) * sin(aLeftHorizontalAngle*M_PI/180.0);
		LHandForce[2] = sin(aLeftVerticalAngle*M_PI/180.0);

		LHandForce *= aLeftMagnitude;
		RHandForce *= aRightMagnitude;
		this->GetSkeleton()->setExtForce(JT_LHAND, LHandForce, true);
		this->GetSkeleton()->setExtForce(JT_RHAND, RHandForce, true);


		// NOTE 2d mode no longer exists
      /*if( Get_App_Ptr()->TwoDModeOnly() )
         this->Set2D();*/

		this->Analyze();
      return   TRUE;
   }

   else //assign failed
   {
      mCOMLastErrorMessage = lHandLoads.LastErrorMessage();
      return   FALSE;
   }
}

//---------------------------------------------------------------------------
BSTR C_Hom_Doc::LastErrorMessage() 
{
   CString strResult( mCOMLastErrorMessage.c_str() );
   return strResult.AllocSysString();
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetAnalysisSummaryA
(  
   short FAR* aElbowPercentilePtr, 
   BSTR FAR* aElbowLimitPtr, 
   
   short FAR* aShoulderPercentilePtr, 
   BSTR FAR* aShoulderLimitPtr, 
   
   short FAR* aTorsoPercentilePtr, 
   BSTR FAR* aTorsoLimitPtr, 

   short FAR* aNeckPercentilePtr, 
   BSTR FAR* aNeckLimitPtr, 

   short FAR* aHipPercentilePtr, 
   BSTR FAR* aHipLimitPtr, 
   
   short FAR* aKneePercentilePtr, 
   BSTR FAR* aKneeLimitPtr, 
   
   short FAR* aAnklePercentilePtr, 
   BSTR FAR* aAnkleLimitPtr,

   short FAR* aWristPercentilePtr,
   BSTR FAR* aWristLimitPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
         aElbowPercentilePtr != 0
      && aElbowLimitPtr != 0

      && aShoulderPercentilePtr != 0
      && aShoulderLimitPtr != 0

      && aTorsoPercentilePtr != 0
      && aTorsoLimitPtr != 0

      && aHipPercentilePtr != 0
      && aHipLimitPtr != 0

      && aKneePercentilePtr != 0
      && aKneeLimitPtr != 0

      && aAnklePercentilePtr != 0
      && aAnkleLimitPtr != 0

	  && aWristPercentilePtr != 0
	  && aWristLimitPtr != 0;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetAnalysisSummaryA: One or more argument pointers are null.";
      return   FALSE;
   }

   //continue

   this->Analyze();

   //--- Percent Capables ---
   AnalysisSummaryData_s summary = this->GetSkeleton()->getAnalysisSummary();
   //* aElbowPercentilePtr    = short(Round_Double( mResults.mElbowMinPercentile, 0 ));
   *aElbowPercentilePtr = short(Round_Double(summary.ElbowMinPercentile));
   //* aShoulderPercentilePtr = short(Round_Double( mResults.mShoulderMinPercentile, 0 ));
   *aShoulderPercentilePtr = short(Round_Double(summary.ShoulderMinPercentile));
   //* aTorsoPercentilePtr    = short(Round_Double( mResults.mTorsoMinPercentile, 0 ));
   * aTorsoPercentilePtr = short(Round_Double(summary.TorsoMinPercentile));
   * aNeckPercentilePtr = short(Round_Double(summary.NeckMinPercentile));
   //* aHipPercentilePtr      = short(Round_Double( mResults.mHipMinPercentile, 0 ));
   * aHipPercentilePtr = short(Round_Double(summary.HipMinPercentile));
   //* aKneePercentilePtr     = short(Round_Double( mResults.mKneeMinPercentile, 0 ));
   * aKneePercentilePtr = short(Round_Double(summary.KneeMinPercentile));
   //* aAnklePercentilePtr    = short(Round_Double( mResults.mAnkleMinPercentile, 0 ));
   * aAnklePercentilePtr = short(Round_Double(summary.AnkleMinPercentile));
   * aWristPercentilePtr = short(Round_Double(summary.WristMinPercentile));
   
   // TODO we don't have these strings anymore, not sure if we have anything equivalent
   /*CString   lElbowLimit( mResults.mElbowLimitText );
   lElbowLimit.SetSysString( aElbowLimitPtr );

   CString   lShoulderLimit( mResults.mShoulderLimitText );
   lShoulderLimit.SetSysString( aShoulderLimitPtr );

   CString   lTorsoLimit( mResults.mTorsoLimitText );
   lTorsoLimit.SetSysString( aTorsoLimitPtr );

   CString   lHipLimit( mResults.mHipLimitText );
   lHipLimit.SetSysString( aHipLimitPtr );

   CString   lKneeLimit( mResults.mKneeLimitText );
   lKneeLimit.SetSysString( aKneeLimitPtr );

   CString   lAnkleLimit( mResults.mAnkleLimitText );
   lAnkleLimit.SetSysString( aAnkleLimitPtr );*/

	return TRUE;
}

//---------------------------------------------------------------------------
BOOL C_Hom_Doc::GetAnalysisSummaryB(
   float FAR* aLeftHandForceXPtr, 
   float FAR* aLeftHandForceYPtr, 
   float FAR* aLeftHandForceZPtr, 
   float FAR* aLeftHandForceMagPtr, 
   
   float FAR* aRightHandForceXPtr, 
   float FAR* aRightHandForceYPtr, 
   float FAR* aRightHandForceZPtr, 
   float FAR* aRightHandForceMagPtr, 
   
   float FAR* aDiscCompressionValuePtr, 
   float FAR* aDiscCompressionDeviationPtr, 
   
   short FAR* aLeftPercentLoadPtr, 
   short FAR* aRightPercentLoadPtr, 
   BSTR  FAR* aBalancePtr, 
   float FAR* aFrictionPtr)
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
         aLeftHandForceXPtr != 0
      && aLeftHandForceYPtr != 0
      && aLeftHandForceZPtr != 0
      && aLeftHandForceMagPtr != 0

      && aRightHandForceXPtr != 0
      && aRightHandForceYPtr != 0
      && aRightHandForceZPtr != 0
      && aRightHandForceMagPtr != 0

      && aDiscCompressionValuePtr != 0
      && aDiscCompressionDeviationPtr != 0

      && aLeftPercentLoadPtr != 0
      && aRightPercentLoadPtr != 0
      && aBalancePtr != 0
      && aFrictionPtr != 0;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetAnalysisSummaryB: One or more argument pointers are null.";
      return   FALSE;
   }

   //continue

   this->Analyze();

	//--- Hand Loads ---

/*   * aLeftHandForceXPtr = float(mLeftForceX.Value());
	* aLeftHandForceYPtr = float(mLeftForceY.Value());
	* aLeftHandForceZPtr = float(mLeftForceZ.Value());
   * aLeftHandForceMagPtr = float(mHandLoads.LeftMagnitudeRef().Value());

	* aRightHandForceXPtr = float(mRightForceX.Value());
	* aRightHandForceYPtr = float(mRightForceY.Value());
	* aRightHandForceZPtr = float(mRightForceZ.Value());
   * aRightHandForceMagPtr = float(mHandLoads.RightMagnitudeRef().Value());*/

   Vector3 lforce = this->GetSkeleton()->getExtForce(JT_LHAND, true);
   Vector3 rforce = this->GetSkeleton()->getExtForce(JT_RHAND, true);
   *aLeftHandForceXPtr = float(lforce[0]);
   *aLeftHandForceYPtr = float(lforce[1]);
   *aLeftHandForceZPtr = float(lforce[2]);
   *aLeftHandForceMagPtr = float(lforce.length());
   *aRightHandForceXPtr = float(rforce[0]);
   *aRightHandForceYPtr = float(rforce[1]);
   *aRightHandForceZPtr = float(rforce[2]);
   *aRightHandForceMagPtr = float(rforce.length());
   
   //--- Disc Compression ---
   //* aDiscCompressionValuePtr = float(mL5S1Compression.Value());
   * aDiscCompressionValuePtr = float(this->GetSkeleton()->getAnalysisSummary(true).L5S1Compression);
   //* aDiscCompressionDeviationPtr = float(mL5S1CompressionSD.Value());
   * aDiscCompressionDeviationPtr = float(this->GetSkeleton()->getAnalysisSummary(true).L5S1CompressionSD);

   //--- Feet ---
   
   //* aLeftPercentLoadPtr = short(Round_Double(mLeftPercentLoad, 0));
   * aLeftPercentLoadPtr = short(Round_Double(this->GetSkeleton()->getBalanceData(true).LeftPctLoad, 0));
   //* aRightPercentLoadPtr = short(Round_Double(mRightPercentLoad, 0));
   * aRightPercentLoadPtr = short(Round_Double(this->GetSkeleton()->getBalanceData(true).RightPctLoad, 0));

   CString   lBalance;

   // seated
/*   if(mPositioning & 1) {
		lBalance = this->getSeatBalanceString();
   } else { // standing
		lBalance = this->getBalanceString();
   }*/
   switch(this->GetSkeleton()->getBalanceData(true).BalanceStatus) {
	   case BS_Unacceptable:
		   lBalance = "Unacceptable";
		   break;
	   case BS_Acceptable:
		   lBalance = "Acceptable";
		   break;
	   case BS_Critical:
		   lBalance = "Critical";
		   break;
	   default:
		   lBalance = "";
		   break;
   }

   lBalance.SetSysString( aBalancePtr );
   
   //* aFrictionPtr = float(Round_Double(mFriction, 2));
   // TODO is this the equivalent thing?
   * aFrictionPtr = float(Round_Double(this->GetSkeleton()->getAnalysisSummary(true).frcoef));

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetAnthropometryA
(
   float FAR* aLowerArmLengthPtr, 
   float FAR* aLowerArmCGPtr, 
   float FAR* aLowerArmWeightPtr, 
   
   float FAR* aUpperArmLengthPtr, 
   float FAR* aUpperArmCGPtr, 
   float FAR* aUpperArmWeightPtr, 
   
   float FAR* aL5ToShoulderLengthPtr, 
   float FAR* aL5ToShoulderCGPtr, 
   float FAR* aL5ToShoulderWeightPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
         aLowerArmLengthPtr != 0
      && aLowerArmCGPtr != 0
      && aLowerArmWeightPtr != 0

      && aUpperArmLengthPtr != 0
      && aUpperArmCGPtr != 0
      && aUpperArmWeightPtr != 0

      && aL5ToShoulderLengthPtr != 0
      && aL5ToShoulderCGPtr != 0
      && aL5ToShoulderWeightPtr != 0;

   if(!lOK) 
   {
      mCOMLastErrorMessage = "GetAnthropometryA: One or more argument pointers are null.";
      return FALSE;
   }

   //continue

   this->Analyze();

   //* aLowerArmCGPtr     = float(mLowerArmCG.Value());
   * aLowerArmCGPtr = float(this->getTask().getLinkCGs().Forearm);
   //* aLowerArmLengthPtr = float(mLowerArmL.Value());
   * aLowerArmLengthPtr = float(this->getTask().getLinkLengths().Forearm);
   //* aLowerArmWeightPtr = float(mLowerArmWt.Value());
   * aLowerArmWeightPtr = float(this->getTask().getLinkWeights().Forearm);

   //* aUpperArmCGPtr     = float(mUpperArmCG.Value());
   * aUpperArmCGPtr = float(this->getTask().getLinkCGs().UpperArm);
   //* aUpperArmLengthPtr = float(mUpperArmL.Value());
   * aUpperArmLengthPtr = float(this->getTask().getLinkLengths().UpperArm);
   //* aUpperArmWeightPtr = float(mUpperArmWt.Value());
   * aUpperArmWeightPtr = float(this->getTask().getLinkWeights().UpperArm);

   //* aL5ToShoulderCGPtr     = float(mL5ToShoulderCG.Value());
   * aL5ToShoulderCGPtr = float(this->getTask().getLinkCGs().L5S1ToShoulder);
   //* aL5ToShoulderLengthPtr = float(mL5ToShoulderL.Value());
   * aL5ToShoulderLengthPtr = float(this->getTask().getLinkLengths().L5S1ToC7T1);
   //* aL5ToShoulderWeightPtr = float(mL5ToShoulderWt.Value());
   // TODO is this the equivalent thing?
   * aL5ToShoulderWeightPtr = float(this->getTask().getLinkWeights().TorsoAndHead);
   
   return   TRUE;
}

//---------------------------------------------------------------------------
BOOL C_Hom_Doc::GetAnthropometryB(float FAR* aL5AboveWeightPtr,

                                  float FAR* aHipToL5LengthPtr,
                                  float FAR* aHipToL5CGPtr,
                                  float FAR* aHipToL5WeightPtr,

                                  float FAR* aHipToShoulderLengthPtr,
                                  float FAR* aHipToShoulderCGPtr,
                                  float FAR* aHipToShoulderWeightPtr)
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL lOK = 
         aL5AboveWeightPtr != 0

      && aHipToL5LengthPtr != 0
      && aHipToL5CGPtr != 0
      && aHipToL5WeightPtr != 0

      && aHipToShoulderLengthPtr != 0
      && aHipToShoulderCGPtr != 0
      && aHipToShoulderWeightPtr != 0;

   if (!lOK)
   {
      mCOMLastErrorMessage = "GetAnthropometryB: One or more argument pointers are null.";
      return FALSE;
   }

   //continue

   this->Analyze();

   //* aL5AboveWeightPtr = float(mUpperBodyWt.Value());
   // TODO is this the equivalent thing?
   * aL5AboveWeightPtr = float(this->getTask().getLinkWeights().TorsoAndHead);

   //* aHipToL5CGPtr     = float(mHipToL5CG.Value());
	// TODO is this the equivalent thing?
   * aHipToL5CGPtr = float(this->getTask().getLinkCGs().Pelvis);
   //* aHipToL5LengthPtr = float(mHipToL5L.Value());
   * aHipToL5LengthPtr = float(this->getTask().getLinkLengths().HipToL5S1);
   //* aHipToL5WeightPtr = float(mHipToL5Wt.Value());
   * aHipToL5WeightPtr = float(this->getTask().getLinkWeights().Pelvis);

   //* aHipToShoulderCGPtr     = float(mHipToShoulderCG.Value());
   // TODO is this the equivalent thing?
   * aHipToShoulderCGPtr = float(this->getTask().getLinkCGs().L5S1ToShoulder);
   //* aHipToShoulderLengthPtr = float(mHipToShoulderL.Value());
   // TODO is this the equivalent thing?
   * aHipToShoulderLengthPtr = float(this->getTask().getLinkLengths().L5S1ToC7T1);
   //* aHipToShoulderWeightPtr = float(mHipToShoulderWt.Value());
   // TODO this is definitely not the equivalent thing
   * aHipToShoulderWeightPtr = float(this->getTask().getLinkWeights().TorsoAndHead);

   return TRUE;
}

//---------------------------------------------------------------------------
BOOL C_Hom_Doc::GetAnthropometryC(float FAR* aUpperLegLength,
                                  float FAR* aUpperLegCG,
                                  float FAR* aUpperLegWeight,

                                  float FAR* aLowerLegLength,
                                  float FAR* aLowerLegCG,
                                  float FAR* aLowerLegWeight,
                                  
                                  float FAR* aFootLength,
                                  float FAR* aFootCG,
                                  float FAR* aFootWeight,

                                  float FAR* aDiaphragmMomentArm)
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL lOK =
         aUpperLegLength != 0
      && aUpperLegCG != 0
      && aUpperLegWeight != 0

      && aLowerLegLength != 0
      && aLowerLegCG != 0
      && aLowerLegWeight != 0

      && aFootLength != 0
      && aFootCG != 0
      && aFootWeight != 0 
      
      && aDiaphragmMomentArm != 0;

   if(!lOK) 
   {
      mCOMLastErrorMessage =
         "GetAnthropometryC: One or more argument pointers are null.";
      return FALSE;
   }

   //continue

   this->Analyze();

   //* aUpperLegCG     = float(mUpperLegCG.Value());
   * aUpperLegCG = float(this->getTask().getLinkCGs().UpperLeg);
   //* aUpperLegLength = float(mUpperLegL.Value());
   * aUpperLegLength = float(this->getTask().getLinkLengths().UpperLeg);
   //* aUpperLegWeight = float(mUpperLegWt.Value());
   * aUpperLegWeight = float(this->getTask().getLinkWeights().UpperLeg);

   //* aLowerLegCG     = float(mLowerLegCG.Value());
   * aLowerLegCG = float(this->getTask().getLinkCGs().LowerLeg);
   //* aLowerLegLength = float(mLowerLegL.Value());
   * aLowerLegLength = float(this->getTask().getLinkLengths().LowerLeg);
//   * aLowerLegWeight = float(mLowerLegWt.Value());
   * aLowerLegWeight = float(this->getTask().getLinkWeights().LowerLeg);

   //* aFootCG         = float(mFootCG.Value());
   * aFootCG = float(this->getTask().getLinkCGs().Foot);
   //* aFootLength     = float(mFootL.Value());
   // TODO is this the equivalent thing?
   * aFootLength = float(this->getTask().getLinkLengths().HeelToToe);
   //* aFootWeight     = float(mFootWt.Value());
   * aFootWeight = float(this->getTask().getLinkWeights().Foot);

   //* aDiaphragmMomentArm = float(mDiaphramMomentL.Value());
   * aDiaphragmMomentArm = float(this->GetSkeleton()->getLowBackSagittalData(true).diaphramMomentArm);

   return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetJointMomentsA
(
   float FAR* aLeftElbowXPtr, 
   float FAR* aLeftElbowYPtr, 
   float FAR* aLeftElbowZPtr, 
   
   float FAR* aRightElbowXPtr, 
   float FAR* aRightElbowYPtr, 
   float FAR* aRightElbowZPtr, 
   
   float FAR* aLeftShoulderXPtr, 
   float FAR* aLeftShoulderYPtr, 
   float FAR* aLeftShoulderZPtr, 
   
   float FAR* aRightShoulderXPtr, 
   float FAR* aRightShoulderYPtr, 
   float FAR* aRightShoulderZPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
         aLeftElbowXPtr != 0
      && aLeftElbowYPtr != 0
      && aLeftElbowZPtr != 0

      && aRightElbowXPtr != 0
      && aRightElbowYPtr != 0
      && aRightElbowZPtr != 0

      && aLeftShoulderXPtr != 0
      && aLeftShoulderYPtr != 0
      && aLeftShoulderZPtr != 0

      && aRightShoulderXPtr != 0
      && aRightShoulderYPtr != 0
      && aRightShoulderZPtr != 0;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetJointMomentsA: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   /*int lSex = this->mResults.sex;

   Torque   lMomX( 0, 1 );
   Torque   lMomY( 0, 1 );
   Torque   lMomZ( 0, 1 );
   
   mResults.JointTorque( lSex, JT_LeftElbow, lMomX, lMomY, lMomZ );
      * aLeftElbowXPtr = float(lMomX.Value());
      * aLeftElbowYPtr = float(lMomY.Value());
      * aLeftElbowZPtr = float(lMomZ.Value());

   mResults.JointTorque( lSex, JT_RightElbow, lMomX, lMomY, lMomZ );
      * aRightElbowXPtr = float(lMomX.Value());
      * aRightElbowYPtr = float(lMomY.Value());
      * aRightElbowZPtr = float(lMomZ.Value());

   mResults.JointTorque( lSex, JT_LeftShoulder, lMomX, lMomY, lMomZ );
      * aLeftShoulderXPtr = float(lMomX.Value());
      * aLeftShoulderYPtr = float(lMomY.Value());
      * aLeftShoulderZPtr = float(lMomZ.Value());

   mResults.JointTorque( lSex, JT_RightShoulder, lMomX, lMomY, lMomZ );
      * aRightShoulderXPtr = float(lMomX.Value());
      * aRightShoulderYPtr = float(lMomY.Value());
      * aRightShoulderZPtr = float(lMomZ.Value());*/

   Vector3 lelbow = this->GetSkeleton()->getNetMoment(JT_LELBOW, true);
   Vector3 relbow = this->GetSkeleton()->getNetMoment(JT_RELBOW, true);
   Vector3 lshoulder = this->GetSkeleton()->getNetMoment(JT_LSHOULDER, true);
   Vector3 rshoulder = this->GetSkeleton()->getNetMoment(JT_RSHOULDER, true);
   * aLeftElbowXPtr = float(lelbow[0]);
   * aLeftElbowYPtr = float(lelbow[1]);
   * aLeftElbowZPtr = float(lelbow[2]);
   * aRightElbowXPtr = float(relbow[0]);
   * aRightElbowYPtr = float(relbow[1]);
   * aRightElbowZPtr = float(relbow[2]);
   * aLeftShoulderXPtr = float(lshoulder[0]);
   * aLeftShoulderYPtr = float(lshoulder[1]);
   * aLeftShoulderZPtr = float(lshoulder[2]);
   * aRightShoulderXPtr = float(rshoulder[0]);
   * aRightShoulderYPtr = float(rshoulder[1]);
   * aRightShoulderZPtr = float(rshoulder[2]);

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetJointMomentsB
(
   float FAR* aL5S1XPtr, 
   float FAR* aL5S1YPtr, 
   float FAR* aL5S1ZPtr, 
   
   float FAR* aLeftHipXPtr, 
   float FAR* aLeftHipYPtr, 
   float FAR* aLeftHipZPtr, 
   
   float FAR* aRightHipXPtr, 
   float FAR* aRightHipYPtr, 
   float FAR* aRightHipZPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
         aL5S1XPtr != 0
      && aL5S1YPtr != 0
      && aL5S1ZPtr != 0

      && aLeftHipXPtr != 0
      && aLeftHipYPtr != 0
      && aLeftHipZPtr != 0

      && aRightHipXPtr != 0
      && aRightHipYPtr != 0
      && aRightHipZPtr != 0;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetJointMomentsB: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   /*int   lSex = this->mResults.sex;

   Torque   lMomX( 0, 1 );
   Torque   lMomY( 0, 1 );
   Torque   lMomZ( 0, 1 );

   mResults.JointTorque( lSex, JT_L5S1, lMomX, lMomY, lMomZ );
      * aL5S1XPtr = float(lMomX.Value());
      * aL5S1YPtr = float(lMomY.Value());
      * aL5S1ZPtr = float(lMomZ.Value());

   mResults.JointTorque( lSex, JT_LeftHip, lMomX, lMomY, lMomZ );
      * aLeftHipXPtr = float(lMomX.Value());
      * aLeftHipYPtr = float(lMomY.Value());
      * aLeftHipZPtr = float(lMomZ.Value());

   mResults.JointTorque( lSex, JT_RightHip, lMomX, lMomY, lMomZ );
      * aRightHipXPtr = float(lMomX.Value());
      * aRightHipYPtr = float(lMomY.Value());
      * aRightHipZPtr = float(lMomZ.Value());*/

   Vector3 L5 = this->GetSkeleton()->getNetMoment(JT_L5S1_TEMP, true);
   Vector3 lhip = this->GetSkeleton()->getNetMoment(JT_LHIP, true);
   Vector3 rhip = this->GetSkeleton()->getNetMoment(JT_RHIP, true);
   * aL5S1XPtr = float(L5[0]);
   * aL5S1YPtr = float(L5[1]);
   * aL5S1ZPtr = float(L5[2]);
   * aLeftHipXPtr = float(lhip[0]);
   * aLeftHipYPtr = float(lhip[1]);
   * aLeftHipZPtr = float(lhip[2]);
   * aRightHipXPtr = float(rhip[0]);
   * aRightHipYPtr = float(rhip[1]);
   * aRightHipZPtr = float(rhip[2]);

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetJointMomentsC
(
   float FAR* aLeftKneeXPtr, 
   float FAR* aLeftKneeYPtr, 
   float FAR* aLeftKneeZPtr, 

   float FAR* aRightKneeXPtr, 
   float FAR* aRightKneeYPtr, 
   float FAR* aRightKneeZPtr, 

   float FAR* aLeftAnkleXPtr, 
   float FAR* aLeftAnkleYPtr, 
   float FAR* aLeftAnkleZPtr, 

   float FAR* aRightAnkleXPtr, 
   float FAR* aRightAnkleYPtr, 
   float FAR* aRightAnkleZPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
         aLeftKneeXPtr != 0
      && aLeftKneeYPtr != 0
      && aLeftKneeZPtr != 0

      && aRightKneeXPtr != 0
      && aRightKneeYPtr != 0
      && aRightKneeZPtr != 0

      && aLeftAnkleXPtr != 0
      && aLeftAnkleYPtr != 0
      && aLeftAnkleZPtr != 0

      && aRightAnkleXPtr != 0
      && aRightAnkleYPtr != 0
      && aRightAnkleZPtr != 0;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetJointMomentsC: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   /*int lSex = this->mResults.sex;

   Torque   lMomX( 0, 1 );
   Torque   lMomY( 0, 1 );
   Torque   lMomZ( 0, 1 );

   mResults.JointTorque( lSex, JT_LeftKnee, lMomX, lMomY, lMomZ );
      * aLeftKneeXPtr = float(lMomX.Value());
      * aLeftKneeYPtr = float(lMomY.Value());
      * aLeftKneeZPtr = float(lMomZ.Value());

   mResults.JointTorque( lSex, JT_RightKnee, lMomX, lMomY, lMomZ );
      * aRightKneeXPtr = float(lMomX.Value());
      * aRightKneeYPtr = float(lMomY.Value());
      * aRightKneeZPtr = float(lMomZ.Value());

   mResults.JointTorque( lSex, JT_LeftAnkle, lMomX, lMomY, lMomZ );
      * aLeftAnkleXPtr = float(lMomX.Value());
      * aLeftAnkleYPtr = float(lMomY.Value());
      * aLeftAnkleZPtr = float(lMomZ.Value());

   mResults.JointTorque( lSex, JT_RightAnkle, lMomX, lMomY, lMomZ );
      * aRightAnkleXPtr = float(lMomX.Value());
      * aRightAnkleYPtr = float(lMomY.Value());
      * aRightAnkleZPtr = float(lMomZ.Value());*/
   Vector3 lknee = this->GetSkeleton()->getNetMoment(JT_LKNEE, true);
   Vector3 rknee = this->GetSkeleton()->getNetMoment(JT_RKNEE, true);
   Vector3 lankle = this->GetSkeleton()->getNetMoment(JT_LANKLE, true);
   Vector3 rankle = this->GetSkeleton()->getNetMoment(JT_RANKLE, true);
   * aLeftKneeXPtr = float(lknee[0]);
   * aLeftKneeYPtr = float(lknee[1]);
   * aLeftKneeZPtr = float(lknee[2]);
   * aRightKneeXPtr = float(rknee[0]);
   * aRightKneeYPtr = float(rknee[1]);
   * aRightKneeZPtr = float(rknee[2]);
   * aLeftAnkleXPtr = float(lankle[0]);
   * aLeftAnkleYPtr = float(lankle[1]);
   * aLeftAnkleZPtr = float(lankle[2]);
   * aRightAnkleXPtr = float(rankle[0]);
   * aRightAnkleYPtr = float(rankle[1]);
   * aRightAnkleZPtr = float(rankle[2]);

	return TRUE;
}

// for syntactic convenience
void VtoPf(Vector3 v, float* p1, float* p2, float* p3) {
	*p1 = (float)v[0];
	*p2 = (float)v[1];
	*p3 = (float)v[2];
}
void AtoPf(double* a, float* p1, float* p2, float* p3) {
	*p1 = (float)a[0];
	*p2 = (float)a[1];
	*p3 = (float)a[2];
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetJointsA
(
   float FAR* aLeftHandXPtr, 
   float FAR* aLeftHandYPtr, 
   float FAR* aLeftHandZPtr, 

   float FAR* aRightHandXPtr, 
   float FAR* aRightHandYPtr, 
   float FAR* aRightHandZPtr, 

   float FAR* aLeftElbowXPtr, 
   float FAR* aLeftElbowYPtr, 
   float FAR* aLeftElbowZPtr, 

   float FAR* aRightElbowXPtr, 
   float FAR* aRightElbowYPtr, 
   float FAR* aRightElbowZPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
         aLeftHandXPtr != 0
      && aLeftHandYPtr != 0
      && aLeftHandZPtr != 0

      && aRightHandXPtr != 0
      && aRightHandYPtr != 0
      && aRightHandZPtr != 0

      && aLeftElbowXPtr != 0
      && aLeftElbowYPtr != 0
      && aLeftElbowZPtr != 0

      && aRightElbowXPtr != 0
      && aRightElbowYPtr != 0
      && aRightElbowZPtr != 0;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetJointsA: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();
   
   /*Skeleton_Record &   lSkeleton = 
      Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get
         ( 
            Get_App_Ptr()->Hom_Ske_Name 
         );

   Length   lLocX( 0, 1 );
   Length   lLocY( 0, 1 );
   Length   lLocZ( 0, 1 );

   lSkeleton.C4EPoint( L_HAND_CTR, lLocX, lLocY, lLocZ );
      * aLeftHandXPtr = float(lLocX.Value());
      * aLeftHandYPtr = float(lLocY.Value());
      * aLeftHandZPtr = float(lLocZ.Value());

   lSkeleton.C4EPoint( R_HAND_CTR, lLocX, lLocY, lLocZ );
      * aRightHandXPtr = float(lLocX.Value());
      * aRightHandYPtr = float(lLocY.Value());
      * aRightHandZPtr = float(lLocZ.Value());

   lSkeleton.C4EPoint( L_ELBOW, lLocX, lLocY, lLocZ );
      * aLeftElbowXPtr = float(lLocX.Value());
      * aLeftElbowYPtr = float(lLocY.Value());
      * aLeftElbowZPtr = float(lLocZ.Value());

   lSkeleton.C4EPoint( R_ELBOW, lLocX, lLocY, lLocZ );
      * aRightElbowXPtr = float(lLocX.Value());
      * aRightElbowYPtr = float(lLocY.Value());
      * aRightElbowZPtr = float(lLocZ.Value());
	  */

	Vector3 lhand = this->GetSkeleton()->getJoint(JT_LHAND, true);
	Vector3 rhand = this->GetSkeleton()->getJoint(JT_RHAND, true);
	Vector3 lelbow = this->GetSkeleton()->getJoint(JT_LELBOW, true);
	Vector3 relbow = this->GetSkeleton()->getJoint(JT_RELBOW, true);
	VtoPf(lhand, aLeftHandXPtr, aLeftHandYPtr, aLeftHandZPtr);
	VtoPf(rhand, aRightHandXPtr, aRightHandYPtr, aRightHandZPtr);
	VtoPf(lelbow, aLeftElbowXPtr, aLeftElbowYPtr, aLeftElbowZPtr);
	VtoPf(relbow, aRightElbowXPtr, aRightElbowYPtr, aRightElbowZPtr);


	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetJointsB
(
   float FAR* aLeftShoulderXPtr, 
   float FAR* aLeftShoulderYPtr, 
   float FAR* aLeftShoulderZPtr, 

   float FAR* aRightShoulderXPtr, 
   float FAR* aRightShoulderYPtr, 
   float FAR* aRightShoulderZPtr, 

   float FAR* aL5S1XPtr, 
   float FAR* aL5S1YPtr, 
   float FAR* aL5S1ZPtr, 

   float FAR* aLeftHipXPtr, 
   float FAR* aLeftHipYPtr, 
   float FAR* aLeftHipZPtr, 

   float FAR* aRightHipXPtr, 
   float FAR* aRightHipYPtr, 
   float FAR* aRightHipZPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
         aLeftShoulderXPtr != 0
      && aLeftShoulderYPtr != 0
      && aLeftShoulderZPtr != 0

      && aRightShoulderXPtr != 0
      && aRightShoulderYPtr != 0
      && aRightShoulderZPtr != 0

      && aL5S1XPtr != 0
      && aL5S1YPtr != 0
      && aL5S1ZPtr != 0

      && aLeftHipXPtr != 0
      && aLeftHipYPtr != 0
      && aLeftHipZPtr != 0

      && aRightHipXPtr != 0
      && aRightHipYPtr != 0
      && aRightHipZPtr != 0;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetJointsB: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();
   
   /*Skeleton_Record &   lSkeleton = 
      Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get
         ( 
            Get_App_Ptr()->Hom_Ske_Name 
         );

   Length   lLocX( 0, 1 );
   Length   lLocY( 0, 1 );
   Length   lLocZ( 0, 1 );

   lSkeleton.C4EPoint( L_SHOULDER, lLocX, lLocY, lLocZ );
      * aLeftShoulderXPtr = float(lLocX.Value());
      * aLeftShoulderYPtr = float(lLocY.Value());
      * aLeftShoulderZPtr = float(lLocZ.Value());

   lSkeleton.C4EPoint( R_SHOULDER, lLocX, lLocY, lLocZ );
      * aRightShoulderXPtr = float(lLocX.Value());
      * aRightShoulderYPtr = float(lLocY.Value());
      * aRightShoulderZPtr = float(lLocZ.Value());

   lSkeleton.C4EPoint( L5_S1, lLocX, lLocY, lLocZ );
      * aL5S1XPtr = float(lLocX.Value());
      * aL5S1YPtr = float(lLocY.Value());
      * aL5S1ZPtr = float(lLocZ.Value());

   lSkeleton.C4EPoint( L_HIP, lLocX, lLocY, lLocZ );
      * aLeftHipXPtr = float(lLocX.Value());
      * aLeftHipYPtr = float(lLocY.Value());
      * aLeftHipZPtr = float(lLocZ.Value());

   lSkeleton.C4EPoint( R_HIP, lLocX, lLocY, lLocZ );
      * aRightHipXPtr = float(lLocX.Value());
      * aRightHipYPtr = float(lLocY.Value());
      * aRightHipZPtr = float(lLocZ.Value());*/

   Vector3 lshoulder = this->GetSkeleton()->getJoint(JT_LSHOULDER, true);
   Vector3 rshoulder = this->GetSkeleton()->getJoint(JT_RSHOULDER, true);
   Vector3 l5 = this->GetSkeleton()->getJoint(JT_L5S1_TEMP, true);
   Vector3 lhip = this->GetSkeleton()->getJoint(JT_LHIP, true);
   Vector3 rhip = this->GetSkeleton()->getJoint(JT_RHIP, true);
   VtoPf(lshoulder, aLeftShoulderXPtr, aLeftShoulderYPtr, aLeftShoulderZPtr);
   VtoPf(rshoulder, aRightShoulderXPtr, aRightShoulderYPtr, aRightShoulderZPtr);
   VtoPf(l5, aL5S1XPtr, aL5S1YPtr, aL5S1ZPtr);
   VtoPf(lhip, aLeftHipXPtr, aLeftHipYPtr, aLeftHipZPtr);
   VtoPf(rhip, aRightHipXPtr, aRightHipYPtr, aRightHipZPtr);

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetJointsC
(
   float FAR* aLeftKneeXPtr, 
   float FAR* aLeftKneeYPtr, 
   float FAR* aLeftKneeZPtr, 

   float FAR* aRightKneeXPtr, 
   float FAR* aRightKneeYPtr, 
   float FAR* aRightKneeZPtr, 

   float FAR* aLeftAnkleXPtr, 
   float FAR* aLeftAnkleYPtr, 
   float FAR* aLeftAnkleZPtr, 

   float FAR* aRightAnkleXPtr, 
   float FAR* aRightAnkleYPtr, 
   float FAR* aRightAnkleZPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
         aLeftKneeXPtr != 0
      && aLeftKneeYPtr != 0
      && aLeftKneeZPtr != 0

      && aRightKneeXPtr != 0
      && aRightKneeYPtr != 0
      && aRightKneeZPtr != 0

      && aLeftAnkleXPtr != 0
      && aLeftAnkleYPtr != 0
      && aLeftAnkleZPtr != 0

      && aRightAnkleXPtr != 0
      && aRightAnkleYPtr != 0
      && aRightAnkleZPtr != 0;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetJointsC: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();
   
   /*Skeleton_Record &   lSkeleton = 
      Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get
         ( 
            Get_App_Ptr()->Hom_Ske_Name 
         );

   Length   lLocX( 0, 1 );
   Length   lLocY( 0, 1 );
   Length   lLocZ( 0, 1 );

   lSkeleton.C4EPoint( L_KNEE, lLocX, lLocY, lLocZ );
      * aLeftKneeXPtr = float(lLocX.Value());
      * aLeftKneeYPtr = float(lLocY.Value());
      * aLeftKneeZPtr = float(lLocZ.Value());

   lSkeleton.C4EPoint( R_KNEE, lLocX, lLocY, lLocZ );
      * aRightKneeXPtr = float(lLocX.Value());
      * aRightKneeYPtr = float(lLocY.Value());
      * aRightKneeZPtr = float(lLocZ.Value());

   lSkeleton.C4EPoint( L_ANKLE, lLocX, lLocY, lLocZ );
      * aLeftAnkleXPtr = float(lLocX.Value());
      * aLeftAnkleYPtr = float(lLocY.Value());
      * aLeftAnkleZPtr = float(lLocZ.Value());

   lSkeleton.C4EPoint( R_ANKLE, lLocX, lLocY, lLocZ );
      * aRightAnkleXPtr = float(lLocX.Value());
      * aRightAnkleYPtr = float(lLocY.Value());
      * aRightAnkleZPtr = float(lLocZ.Value());*/
   Vector3 lknee = this->GetSkeleton()->getJoint(JT_LKNEE, true);
   Vector3 rknee = this->GetSkeleton()->getJoint(JT_RKNEE, true);
   Vector3 lankle = this->GetSkeleton()->getJoint(JT_LANKLE, true);
   Vector3 rankle = this->GetSkeleton()->getJoint(JT_RANKLE, true);
   VtoPf(lknee, aLeftKneeXPtr, aLeftKneeYPtr, aLeftKneeZPtr);
   VtoPf(rknee, aRightKneeXPtr, aRightKneeYPtr, aRightKneeZPtr);
   VtoPf(lankle, aLeftAnkleXPtr, aLeftAnkleYPtr, aLeftAnkleZPtr);
   VtoPf(rankle, aRightAnkleXPtr, aRightAnkleYPtr, aRightAnkleZPtr);

	return TRUE;
}
   
//---------------------------------------------------------------------------
   void 
LowBackLine
(
   //Analysis &  aResults ,  //Input
   Skeleton* skeleton,
   int         aIndex ,    //Input
   float       aArmXCm ,   //Input
   float       aArmYCm ,   //Input
   float &     aResultant ,//Returned
   float &     aShear ,    //Returned
   float &     aForceX ,   //Returned
   float &     aForceY ,   //Returned
   float &     aForceZ ,   //Returned
   float &     aArmX ,     //Returned
   float &     aArmY       //Returned
)
{
	LowBack3DData_s data = skeleton->getLowBack3DData(true);
	aResultant = data.Resultant[aIndex];
	aShear = data.Shear[aIndex];
	aForceX = data.XForce[aIndex];
	aForceY = data.YForce[aIndex];
	aForceZ = data.ZForce[aIndex];
	aArmX = data.XArm[aIndex];
	aArmY = data.YArm[aIndex];

   /*int   lSex = aResults.sex;
   
   Force   lForce( 0.0F, 0 ); //Temp dimension with 0 decimals.

   lForce.Value( aResults.Resultant[aIndex][lSex], MetricCns );
   aResultant = float(lForce.Value());

   lForce.Value( aResults.Shear[aIndex][lSex], MetricCns );
   aShear = float(lForce.Value());

   lForce.Value( aResults.Xforce[aIndex][lSex], MetricCns );
   aForceX = float(lForce.Value());

   lForce.Value( aResults.Yforce[aIndex][lSex], MetricCns );
   aForceY = float(lForce.Value());

   lForce.Value( aResults.Zforce[aIndex][lSex], MetricCns );
   aForceZ = float(lForce.Value());

   Length   lLength( 0.0F, 1 ); //Temp dimension with 1 decimal

   lLength.Value( aArmXCm, MetricCns );
   aArmX = float(lLength.Value());

   lLength.Value( aArmYCm, MetricCns );
   aArmY = float(lLength.Value());

   return;*/
}


#define   MomArm( m, f ) (lGender?(f):(m))

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetLowBackA
(
   float FAR* aLeftErSpiFRPtr, 
   float FAR* aLeftErSpiFSPtr, 
   float FAR* aLeftErSpiXPtr, 
   float FAR* aLeftErSpiYPtr, 
   float FAR* aLeftErSpiZPtr, 
   float FAR* aLeftErSpiMXPtr, 
   float FAR* aLeftErSpiMYPtr, 

   float FAR* aRightErSpiFRPtr, 
   float FAR* aRightErSpiFSPtr, 
   float FAR* aRightErSpiXPtr, 
   float FAR* aRightErSpiYPtr, 
   float FAR* aRightErSpiZPtr, 
   float FAR* aRightErSpiMXPtr, 
   float FAR* aRightErSpiMYPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
      aLeftErSpiFRPtr != 0 && 
      aLeftErSpiFSPtr != 0 && 
      aLeftErSpiXPtr  != 0 && 
      aLeftErSpiYPtr  != 0 && 
      aLeftErSpiZPtr  != 0 && 
      aLeftErSpiMXPtr != 0 && 
      aLeftErSpiMYPtr != 0 && 

      aRightErSpiFRPtr != 0 && 
      aRightErSpiFSPtr != 0 && 
      aRightErSpiXPtr  != 0 && 
      aRightErSpiYPtr  != 0 && 
      aRightErSpiZPtr  != 0 && 
      aRightErSpiMXPtr != 0 && 
      aRightErSpiMYPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetLowBackA: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   int lGender = this->GenderCode(); //Used by moment macro
   Skeleton* skel = this->GetSkeleton();

   // Muscles: Note use of text macro MomArm defined above.
   
   // TODO check that indices are in the same order that they used to be
   LowBackLine
   ( 
      //mResults, 1, MomArm(3.3F,3.5F), MomArm(5.9F,5.4F), 
	  skel, 0, MomArm(3.3F,3.5F), MomArm(5.9F,5.4F), 
      * aLeftErSpiFRPtr, * aLeftErSpiFSPtr, * aLeftErSpiXPtr, * aLeftErSpiYPtr, * aLeftErSpiZPtr, 
      * aLeftErSpiMXPtr, * aLeftErSpiMYPtr
   );

   LowBackLine
   ( 
      //mResults, 2, MomArm(3.3F,3.4F), MomArm(5.9F,5.2F),
	  skel, 1, MomArm(3.3F,3.4F), MomArm(5.9F,5.2F),
      * aRightErSpiFRPtr, * aRightErSpiFSPtr, * aRightErSpiXPtr, * aRightErSpiYPtr, * aRightErSpiZPtr, 
      * aRightErSpiMXPtr, * aRightErSpiMYPtr
   );

   return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetLowBackB
(
   float FAR* aLeftRecAbFRPtr, 
   float FAR* aLeftRecAbFSPtr, 
   float FAR* aLeftRecAbXPtr, 
   float FAR* aLeftRecAbYPtr, 
   float FAR* aLeftRecAbZPtr, 
   float FAR* aLeftRecAbMXPtr, 
   float FAR* aLeftRecAbMYPtr, 

   float FAR* aRightRecAbFRPtr, 
   float FAR* aRightRecAbFSPtr, 
   float FAR* aRightRecAbXPtr, 
   float FAR* aRightRecAbYPtr, 
   float FAR* aRightRecAbZPtr, 
   float FAR* aRightRecAbMXPtr, 
   float FAR* aRightRecAbMYPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
      aLeftRecAbFRPtr != 0 && 
      aLeftRecAbFSPtr != 0 && 
      aLeftRecAbXPtr  != 0 && 
      aLeftRecAbYPtr  != 0 && 
      aLeftRecAbZPtr  != 0 && 
      aLeftRecAbMXPtr != 0 && 
      aLeftRecAbMYPtr != 0 && 

      aRightRecAbFRPtr != 0 && 
      aRightRecAbFSPtr != 0 && 
      aRightRecAbXPtr  != 0 && 
      aRightRecAbYPtr  != 0 && 
      aRightRecAbZPtr  != 0 && 
      aRightRecAbMXPtr != 0 && 
      aRightRecAbMYPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetLowBackB: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   int lGender = this->GenderCode(); //Used by moment macro
   Skeleton* skel = this->GetSkeleton();

   // Muscles: Note use of text macro MomArm defined above.

   LowBackLine
   ( 
      //mResults, 3, MomArm(4.1F,4.1F), MomArm(8.3F,7.0F),
	  skel, 2, MomArm(4.1F,4.1F), MomArm(8.3F,7.0F),
      * aLeftRecAbFRPtr, * aLeftRecAbFSPtr, * aLeftRecAbXPtr, * aLeftRecAbYPtr, * aLeftRecAbZPtr, 
      * aLeftRecAbMXPtr, * aLeftRecAbMYPtr
   );

   LowBackLine
   ( 
      //mResults, 4, MomArm(4.1F,4.2), MomArm(8.3F,6.9F),
	  skel, 3, MomArm(4.1F,4.2), MomArm(8.3F,6.9F),
	 
      * aRightRecAbFRPtr, * aRightRecAbFSPtr, * aRightRecAbXPtr, * aRightRecAbYPtr, * aRightRecAbZPtr, 
      * aRightRecAbMXPtr, * aRightRecAbMYPtr
   );

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetLowBackC
(
   float FAR* aLeftIntObFRPtr, 
   float FAR* aLeftIntObFSPtr, 
   float FAR* aLeftIntObXPtr, 
   float FAR* aLeftIntObYPtr, 
   float FAR* aLeftIntObZPtr, 
   float FAR* aLeftIntObMXPtr, 
   float FAR* aLeftIntObMYPtr, 

   float FAR* aRightIntObFRPtr, 
   float FAR* aRightIntObFSPtr, 
   float FAR* aRightIntObXPtr, 
   float FAR* aRightIntObYPtr, 
   float FAR* aRightIntObZPtr, 
   float FAR* aRightIntObMXPtr, 
   float FAR* aRightIntObMYPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK =
      aLeftIntObFRPtr != 0 && 
      aLeftIntObFSPtr != 0 && 
      aLeftIntObXPtr  != 0 && 
      aLeftIntObYPtr  != 0 && 
      aLeftIntObZPtr  != 0 && 
      aLeftIntObMXPtr != 0 && 
      aLeftIntObMYPtr != 0 && 

      aRightIntObFRPtr != 0 && 
      aRightIntObFSPtr != 0 && 
      aRightIntObXPtr  != 0 && 
      aRightIntObYPtr  != 0 && 
      aRightIntObZPtr  != 0 && 
      aRightIntObMXPtr != 0 && 
      aRightIntObMYPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetLowBackC: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   int   lGender = this->GenderCode(); //Used by moment macro
   Skeleton* skel = this->GetSkeleton();

   // Muscles: Note use of text macro MomArm defined above.

   LowBackLine
   ( 
      //mResults, 5, MomArm(11.7F,12.3F), MomArm(3.5F,2.8F),
	  skel, 4, MomArm(11.7F,12.3F), MomArm(3.5F,2.8F),
      * aLeftIntObFRPtr, * aLeftIntObFSPtr, * aLeftIntObXPtr, * aLeftIntObYPtr, * aLeftIntObZPtr, 
      * aLeftIntObMXPtr, * aLeftIntObMYPtr
   );

   LowBackLine
   ( 
      //mResults, 6, MomArm(11.7F,12.1F), MomArm(3.5F,3.0F),
	  skel, 5, MomArm(11.7F,12.1F), MomArm(3.5F,3.0F),
      * aRightIntObFRPtr, * aRightIntObFSPtr, * aRightIntObXPtr, * aRightIntObYPtr, * aRightIntObZPtr, 
      * aRightIntObMXPtr, * aRightIntObMYPtr
   );
	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetLowBackD
(
   float FAR* aLeftExtObFRPtr, 
   float FAR* aLeftExtObFSPtr, 
   float FAR* aLeftExtObXPtr, 
   float FAR* aLeftExtObYPtr, 
   float FAR* aLeftExtObZPtr, 
   float FAR* aLeftExtObMXPtr, 
   float FAR* aLeftExtObMYPtr, 

   float FAR* aRightExtObFRPtr, 
   float FAR* aRightExtObFSPtr, 
   float FAR* aRightExtObXPtr, 
   float FAR* aRightExtObYPtr, 
   float FAR* aRightExtObZPtr, 
   float FAR* aRightExtObMXPtr, 
   float FAR* aRightExtObMYPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK =
      aLeftExtObFRPtr != 0 && 
      aLeftExtObFSPtr != 0 && 
      aLeftExtObXPtr  != 0 && 
      aLeftExtObYPtr  != 0 && 
      aLeftExtObZPtr  != 0 && 
      aLeftExtObMXPtr != 0 && 
      aLeftExtObMYPtr != 0 && 

      aRightExtObFRPtr != 0 && 
      aRightExtObFSPtr != 0 && 
      aRightExtObXPtr  != 0 && 
      aRightExtObYPtr  != 0 && 
      aRightExtObZPtr  != 0 && 
      aRightExtObMXPtr != 0 && 
      aRightExtObMYPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetLowBackD: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   int   lGender = this->GenderCode(); //Used by moment macro
   Skeleton* skel = this->GetSkeleton();

   // Muscles: Note use of text macro MomArm defined above.

   LowBackLine
   ( 
      //mResults, 7, MomArm(13.2F,11.4), MomArm(3.3F,3.0F),
	  skel, 6, MomArm(13.2F,11.4), MomArm(3.3F,3.0F),
      * aLeftExtObFRPtr, * aLeftExtObFSPtr, * aLeftExtObXPtr, * aLeftExtObYPtr, * aLeftExtObZPtr, 
      * aLeftExtObMXPtr, * aLeftExtObMYPtr
   );

   LowBackLine
   ( 
      //mResults, 8, MomArm(13.2F,11.5F), MomArm(3.3F,3.0F),
	  skel, 7, MomArm(13.2F,11.5F), MomArm(3.3F,3.0F),
      * aRightExtObFRPtr, * aRightExtObFSPtr, * aRightExtObXPtr, * aRightExtObYPtr, * aRightExtObZPtr, 
      * aRightExtObMXPtr, * aRightExtObMYPtr
   );
	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetLowBackE
(
   float FAR* aLeftLatDorFRPtr, 
   float FAR* aLeftLatDorFSPtr, 
   float FAR* aLeftLatDorXPtr, 
   float FAR* aLeftLatDorYPtr, 
   float FAR* aLeftLatDorZPtr, 
   float FAR* aLeftLatDorMXPtr, 
   float FAR* aLeftLatDorMYPtr, 

   float FAR* aRightLatDorFRPtr, 
   float FAR* aRightLatDorFSPtr, 
   float FAR* aRightLatDorXPtr, 
   float FAR* aRightLatDorYPtr, 
   float FAR* aRightLatDorZPtr, 
   float FAR* aRightLatDorMXPtr, 
   float FAR* aRightLatDorMYPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK =  
      aLeftLatDorFRPtr != 0 && 
      aLeftLatDorFSPtr != 0 && 
      aLeftLatDorXPtr  != 0 && 
      aLeftLatDorYPtr  != 0 && 
      aLeftLatDorZPtr  != 0 && 
      aLeftLatDorMXPtr != 0 && 
      aLeftLatDorMYPtr != 0 && 

      aRightLatDorFRPtr != 0 && 
      aRightLatDorFSPtr != 0 && 
      aRightLatDorXPtr  != 0 && 
      aRightLatDorYPtr  != 0 && 
      aRightLatDorZPtr  != 0 && 
      aRightLatDorMXPtr != 0 && 
      aRightLatDorMYPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetLowBackE: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   int   lGender = this->GenderCode(); //Used by moment macro
   Skeleton* skel = this->GetSkeleton();

   // Muscles: Note use of text macro MomArm defined above.

   LowBackLine
   ( 
      //mResults, 9, MomArm(7.2F,11.8F), MomArm(5.4F,1.4F),
	  skel, 8, MomArm(7.2F,11.8F), MomArm(5.4F,1.4F),
      * aLeftLatDorFRPtr, * aLeftLatDorFSPtr, * aLeftLatDorXPtr, * aLeftLatDorYPtr, * aLeftLatDorZPtr, 
      * aLeftLatDorMXPtr, * aLeftLatDorMYPtr
   );

   LowBackLine
   ( 
      //mResults, 10, MomArm(7.2F,11.9F), MomArm(5.4F,1.7F),
	  skel, 9, MomArm(7.2F,11.9F), MomArm(5.4F,1.7F),
      * aRightLatDorFRPtr, * aRightLatDorFSPtr, * aRightLatDorXPtr, * aRightLatDorYPtr, * aRightLatDorZPtr, 
      * aRightLatDorMXPtr, * aRightLatDorMYPtr
   );

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetLowBackF
(
   float FAR* aCompressionTotalPtr, 
   float FAR* aShearTotalPtr, 
   float FAR* aShearAntPostPtr, 
   float FAR* aShearLateralPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK =
      aCompressionTotalPtr != 0 && 
      aShearTotalPtr       != 0 && 
      aShearAntPostPtr     != 0 && 
      aShearLateralPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetLowBackF: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   

   * aCompressionTotalPtr = float(this->GetSkeleton()->getLowBack3DData(true).totalL4L5Compression);
   * aShearTotalPtr = float(this->GetSkeleton()->getLowBack3DData(true).totalL4L5Shear);
   * aShearAntPostPtr = float(this->GetSkeleton()->getLowBack3DData(true).anteriorL4L5Shear);
   * aShearLateralPtr = float(this->GetSkeleton()->getLowBack3DData(true).lateralL4L5Shear);

   /*
int lSex = mResults.sex;
   Force   lForce( 0.0F, 0 ); //Temp dimension with 0 decimals
  
   //NOTE: Sign reversal: Compression is positive.
//   lForce.Value( - mResults.Disc_Compression[lSex], MetricCns );
//   * aCompressionTotalPtr = lForce.Value();
//   
//   lForce.Value( mResults.Disc_Shear[lSex], MetricCns );
//   * aShearTotalPtr = lForce.Value();
//   
//   lForce.Value( mResults.Disc_APShear[lSex], MetricCns );
//   * aShearAntPostPtr = lForce.Value();
//   
//   lForce.Value( mResults.Disc_LATShear[lSex], MetricCns );
//   * aShearLateralPtr = lForce.Value();

   * aCompressionTotalPtr = float(mResults.L4L5DiscCompression( lSex ).Value());
   * aShearTotalPtr = float(mResults.L4L5ShearTotal( lSex ).Value());
   * aShearAntPostPtr = float(mResults.L4L5ShearAntPost( lSex ).Value());
   * aShearLateralPtr = float(mResults.L4L5ShearLateral( lSex ).Value());*/


	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetPostureA
(
   short FAR* aLeftElbowIncludedPtr, 
   short FAR* aRightElbowIncludedPtr, 

   short FAR* aLeftShoulderVerticalPtr, 
   short FAR* aRightShoulderVerticalPtr, 

   short FAR* aLeftShoulderHorizontalPtr, 
   short FAR* aRightShoulderHorizontalPtr, 

   short FAR* aLeftHumeralRotationPtr, 
   short FAR* aRightHumeralRotationPtr, 

   short FAR* aLeftHipIncludedPtr, 
   short FAR* aRightHipIncludedPtr, 

   short FAR* aLeftKneeIncludedPtr, 
   short FAR* aRightKneeIncludedPtr, 

   short FAR* aLeftAnkleIncludedPtr, 
   short FAR* aRightAnkleIncludedPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK =
      aLeftElbowIncludedPtr != 0 && 
      aRightElbowIncludedPtr != 0 && 

      aLeftShoulderVerticalPtr != 0 && 
      aRightShoulderVerticalPtr != 0 && 

      aLeftShoulderHorizontalPtr != 0 && 
      aRightShoulderHorizontalPtr != 0 && 

      aLeftHumeralRotationPtr != 0 && 
      aRightHumeralRotationPtr != 0 && 

      aLeftHipIncludedPtr != 0 && 
      aRightHipIncludedPtr != 0 && 

      aLeftKneeIncludedPtr != 0 && 
      aRightKneeIncludedPtr != 0 && 

      aLeftAnkleIncludedPtr != 0 && 
      aRightAnkleIncludedPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetPostureA: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   //--- Additional hip angles
/*
   int   lLeftHipIncluded;
   if( mResults.angle[13] < 0 )
   {
      lLeftHipIncluded = short(mResults.angle[21] - mResults.angle[13] - 180);
   }
   else
   {
      lLeftHipIncluded = short(mResults.angle[21] - mResults.angle[13] + 180);
   }

   int   lRightHipIncluded;
   if( mResults.angle[ 3] < 0 )
   {
      lRightHipIncluded = short(mResults.angle[21] - mResults.angle[ 3] - 180);
   }
   else
   {
      lRightHipIncluded = short(mResults.angle[21] - mResults.angle[ 3] + 180 );
   }

   //--- Additional knee angles

   int   lLeftKneeIncluded;
   if( mResults.angle[13] < 0 )
   {
      lLeftKneeIncluded = short(mResults.angle[11] - mResults.angle[13] - 180);
   }
   else
   {
      lLeftKneeIncluded = short(mResults.angle[11] - mResults.angle[13] + 180);
   }

   int   lRightKneeIncluded;
   if( mResults.angle[ 3] < 0 )
   {
      lRightKneeIncluded = short(mResults.angle[ 1] - mResults.angle[ 3] - 180);
   }
   else
   {
      lRightKneeIncluded = short(mResults.angle[ 1] - mResults.angle[ 3] + 180);
   }*/

	//--- Assign dialog members ---
   * aLeftElbowIncludedPtr			= short(this->GetSkeleton()->getExtraAngle(EA_ElbowLInc));
   * aLeftShoulderVerticalPtr		= short(this->GetSkeleton()->getExtraAngle(EA_ShoulderLV));
   * aLeftShoulderHorizontalPtr		= short(this->GetSkeleton()->getExtraAngle(EA_ShoulderLH));
   * aLeftHumeralRotationPtr		= short(this->GetSkeleton()->getExtraAngle(EA_HumerusLRot));
   * aLeftHipIncludedPtr			= short(this->GetSkeleton()->getExtraAngle(EA_HipLInc));
   * aLeftKneeIncludedPtr			= short(this->GetSkeleton()->getExtraAngle(EA_KneeLInc));
   * aLeftAnkleIncludedPtr			= short(this->GetSkeleton()->getExtraAngle(EA_AnkleLInc));
   
   * aRightElbowIncludedPtr			= short(this->GetSkeleton()->getExtraAngle(EA_ElbowRInc));
   * aRightShoulderVerticalPtr		= short(this->GetSkeleton()->getExtraAngle(EA_ShoulderRV));
   * aRightShoulderHorizontalPtr	= short(this->GetSkeleton()->getExtraAngle(EA_ShoulderRH));
   * aRightHumeralRotationPtr		= short(this->GetSkeleton()->getExtraAngle(EA_HumerusRRot));
   * aRightHipIncludedPtr			= short(this->GetSkeleton()->getExtraAngle(EA_HipRInc));
   * aRightKneeIncludedPtr			= short(this->GetSkeleton()->getExtraAngle(EA_KneeRInc));
   * aRightAnkleIncludedPtr			= short(this->GetSkeleton()->getExtraAngle(EA_AnkleRInc));
   
   /*
   * aLeftElbowIncludedPtr       = short(mResults.angle[17]);
	* aLeftShoulderVerticalPtr    = short(mResults.angle[15]);
	* aLeftShoulderHorizontalPtr  = short(mResults.angle[16]);
	* aLeftHumeralRotationPtr     = short(mResults.angle[18]);
	* aLeftHipIncludedPtr         = short(lLeftHipIncluded);
	* aLeftKneeIncludedPtr        = short(lLeftKneeIncluded);
	* aLeftAnkleIncludedPtr       = short(mResults.angle[11]);

	* aRightElbowIncludedPtr      = short(mResults.angle[ 7]);
	* aRightShoulderVerticalPtr   = short(mResults.angle[ 5]);
	* aRightShoulderHorizontalPtr = short(mResults.angle[ 6]);
	* aRightHumeralRotationPtr    = short(mResults.angle[ 8]);
	* aRightHipIncludedPtr        = short(lRightHipIncluded);
	* aRightKneeIncludedPtr       = short(lRightKneeIncluded);
	* aRightAnkleIncludedPtr      = short(mResults.angle[ 1]);*/

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetPostureB
(
   short FAR* aAdjustedFlexionPtr, 
   short FAR* aAdjustedRotationPtr, 
   short FAR* aAdjustedLateralBendingPtr, 
   short FAR* aPelvicRotationPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK =  
      aAdjustedFlexionPtr != 0 && 
      aAdjustedRotationPtr != 0 && 
      aAdjustedLateralBendingPtr != 0 && 
      aPelvicRotationPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetPostureB: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   * aAdjustedFlexionPtr = short(this->GetSkeleton()->getExtraAngle(EA_AdjustedTrunkFlexion));
   * aAdjustedRotationPtr = short(this->GetSkeleton()->getExtraAngle(EA_AdjustedTrunkAxialRotation));
   * aAdjustedLateralBendingPtr = short(this->GetSkeleton()->getExtraAngle(EA_AdjustedTrunkLateralBending));
   // TODO check that this is what gPelvicAngle corresponds to
   * aPelvicRotationPtr = short(this->GetSkeleton()->getExtraAngle(EA_PelvisFlexion));

   /*
	* aAdjustedFlexionPtr         = short(mResults.torsoang);
	* aAdjustedRotationPtr        = short(mResults.trrot);
	* aAdjustedLateralBendingPtr  = short(mResults.angle[24]);
	* aPelvicRotationPtr          = short(gPelvicAngle);*/

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetSagittal
(
   float FAR* aTotalPtr,
   float FAR* aTotalStdDevPtr, 
   float FAR* aErectorSpinaePtr, 
   float FAR* aErectorSpinaeStdDevPtr, 
   float FAR* aRectusAbdominusPtr, 
   float FAR* aRectusAbdominusStdDevPtr, 
   float FAR* aAbdominalPtr, 
   float FAR* aHandLoadsPtr, 
   float FAR* aUpperBodyWeightPtr, 

   float FAR* aShearTotalPtr, 
   float FAR* aShearSagittalPtr, 
   float FAR* aShearFrontalPtr, 

   short FAR* aEstLigamentStrainPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK =
      aTotalPtr != 0 && 
      aTotalStdDevPtr != 0 && 
      aErectorSpinaePtr != 0 && 
      aErectorSpinaeStdDevPtr != 0 && 
      aRectusAbdominusPtr != 0 && 
      aRectusAbdominusStdDevPtr != 0 && 
      aAbdominalPtr != 0 && 
      aHandLoadsPtr != 0 && 
      aUpperBodyWeightPtr != 0 && 

      aShearTotalPtr != 0 && 
      aShearSagittalPtr != 0 && 
      aShearFrontalPtr != 0 &&
      aEstLigamentStrainPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetSagittal: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();
   LowBackSagittalData_s sag = this->GetSkeleton()->getLowBackSagittalData(true);
   *aTotalPtr = float(sag.cmpTotal);
   *aTotalStdDevPtr = float(sag.cmpTotalSD);

   *aErectorSpinaePtr = float(sag.cmpSpinae);
   *aErectorSpinaeStdDevPtr = float(sag.cmpSpinaeSD);

   *aRectusAbdominusPtr = float(sag.cmpAbdominus);
   *aRectusAbdominusStdDevPtr = float(sag.cmpAbdominusSD);

   // NOTE negated because they were below
   *aAbdominalPtr = float(-sag.cmpAbdominal);
   *aHandLoadsPtr = float(-sag.cmpHandLoads);

   // TODO we don't have alpha that is used below
   // it's something like that l5s1 rotation constant minus the pelvic tilt
   // NOTE negated because it was below
   *aUpperBodyWeightPtr = float(-sag.cmpUpperBody);// * sin(alpha)...

   *aShearTotalPtr = float(sag.shrTotal);
   *aShearSagittalPtr = float(sag.shrSagittal);
   *aShearFrontalPtr = float(sag.shrFrontal);

   // TODO check on this
   *aEstLigamentStrainPtr = float(sag.ligamentStrain);

/*   Force   lForce( 0.0F, 0 );

   int lSex = mResults.sex;

   *aTotalPtr = float(mL5S1Compression.Value());

   *aTotalStdDevPtr = float(mL5S1CompressionSD.Value());
	   
   lForce.Value( mResults.fe[lSex], MetricCns );
   *aErectorSpinaePtr = float(lForce.Value());
	   
   lForce.Value( mResults.sdfe[lSex], MetricCns );
   *aErectorSpinaeStdDevPtr = float(lForce.Value());
	   
   lForce.Value( mResults.fr[lSex], MetricCns );
   *aRectusAbdominusPtr = float(lForce.Value());
	   
   lForce.Value( mResults.sdfr[lSex], MetricCns );
   * aRectusAbdominusStdDevPtr = float(lForce.Value());
	   
   lForce.Value( - mResults.abdomforces[lSex], MetricCns );
   * aAbdominalPtr = float(lForce.Value());
	   
   lForce.Value( - mResults.cf[lSex], MetricCns );
   * aHandLoadsPtr = float(lForce.Value());

   double   lBodyComp[3]; 
   lBodyComp[1] = mResults.bw[1] * sin( mResults.alpha * U_DEGREES_RADIANS); 
   lBodyComp[2] = mResults.bw[2] * sin( mResults.alpha * U_DEGREES_RADIANS); 
	   
   lForce.Value( - lBodyComp[lSex], MetricCns );
   * aUpperBodyWeightPtr = float(lForce.Value());
	   
   lForce.Value( mResults.shear1[lSex], MetricCns );
   * aShearTotalPtr = float(lForce.Value());
	   
   lForce.Value( mResults.shearY[lSex], MetricCns );
   * aShearSagittalPtr = float(lForce.Value());
	   
   lForce.Value( mResults.shearX[lSex], MetricCns );
   * aShearFrontalPtr = float(lForce.Value());
	      
   * aEstLigamentStrainPtr = short(Round_Double(mResults.lumfas[lSex], 0));*/

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetSpinalA
(
   float FAR* aGloL2L3FXPtr, 
   float FAR* aGloL2L3FYPtr, 
   float FAR* aGloL2L3FZPtr, 

   float FAR* aGloL2L3MXPtr, 
   float FAR* aGloL2L3MYPtr, 
   float FAR* aGloL2L3MZPtr, 

   float FAR* aGloL4L5FXPtr, 
   float FAR* aGloL4L5FYPtr, 
   float FAR* aGloL4L5FZPtr, 

   float FAR* aGloL4L5MXPtr, 
   float FAR* aGloL4L5MYPtr, 
   float FAR* aGloL4L5MZPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK =
      aGloL2L3FXPtr != 0 && 
      aGloL2L3FYPtr != 0 && 
      aGloL2L3FZPtr != 0 && 

      aGloL2L3MXPtr != 0 && 
      aGloL2L3MYPtr != 0 && 
      aGloL2L3MZPtr != 0 && 

      aGloL4L5FXPtr != 0 && 
      aGloL4L5FYPtr != 0 && 
      aGloL4L5FZPtr != 0 && 

      aGloL4L5MXPtr != 0 && 
      aGloL4L5MYPtr != 0 && 
      aGloL4L5MZPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetSpinalA: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   AtoPf(this->GetSkeleton()->getSpinalForces(true).globalL2L3, aGloL2L3FXPtr, aGloL2L3FYPtr, aGloL2L3FZPtr);
   AtoPf(this->GetSkeleton()->getSpinalMoments(true).globalL2L3, aGloL2L3MXPtr, aGloL2L3MYPtr, aGloL2L3MZPtr);
   AtoPf(this->GetSkeleton()->getSpinalForces(true).globalL4L5, aGloL4L5FXPtr, aGloL4L5FYPtr, aGloL4L5FZPtr);
   AtoPf(this->GetSkeleton()->getSpinalMoments(true).globalL4L5, aGloL4L5MXPtr, aGloL4L5MYPtr, aGloL4L5MZPtr);

   //--- Local References

   /*int        lSex              =  mResults.sex;
   link3 &    lL2L3GloForce     =  mResults.forceonL2L3;
   link3 &    lL4L5GloForce     =  mResults.forceonL4L5; 
   ttarray &  lGloMom           =  mResults.ttorq;

   const float   lC = U_M_CM;


   //--- Global Forces and Moments ---
   
   Force   lForX( 0, 1 );
   Force   lForY( 0, 1 );
   Force   lForZ( 0, 1 );

   Torque  lMomX( 0, 1 );
   Torque  lMomY( 0, 1 );
   Torque  lMomZ( 0, 1 );

   // L2/L3

   lForX.Value( lL2L3GloForce[lSex][1], MetricCns );
   lForY.Value( lL2L3GloForce[lSex][2], MetricCns );
   lForZ.Value( lL2L3GloForce[lSex][3], MetricCns );


   * aGloL2L3FXPtr = float(lForX.Value());
   * aGloL2L3FYPtr = float(lForY.Value());
   * aGloL2L3FZPtr = float(lForZ.Value());

   lMomX.Value( lC * lGloMom[lSex][15][1], MetricCns );
   lMomY.Value( lC * lGloMom[lSex][15][2], MetricCns );
   lMomZ.Value( lC * lGloMom[lSex][15][3], MetricCns );


   * aGloL2L3MXPtr = float(lMomX.Value());
   * aGloL2L3MYPtr = float(lMomY.Value());
   * aGloL2L3MZPtr = float(lMomZ.Value());

   // L3/L4: None given

   // L4/L5

   lForX.Value( lL4L5GloForce[lSex][1], MetricCns );
   lForY.Value( lL4L5GloForce[lSex][2], MetricCns );
   lForZ.Value( lL4L5GloForce[lSex][3], MetricCns );

   * aGloL4L5FXPtr = float(lForX.Value());
   * aGloL4L5FYPtr = float(lForY.Value());
   * aGloL4L5FZPtr = float(lForZ.Value());

   lMomX.Value( lC * lGloMom[lSex][16][1], MetricCns );
   lMomY.Value( lC * lGloMom[lSex][16][2], MetricCns );
   lMomZ.Value( lC * lGloMom[lSex][16][3], MetricCns );

   * aGloL4L5MXPtr = float(lMomX.Value());
   * aGloL4L5MYPtr = float(lMomY.Value());
   * aGloL4L5MZPtr = float(lMomZ.Value());*/

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetSpinalB
(
   float FAR* aGloL5S1MXPtr, 
   float FAR* aGloL5S1MYPtr, 
   float FAR* aGloL5S1MZPtr, 

   float FAR* aRotL2L3FXPtr, 
   float FAR* aRotL2L3FYPtr, 
   float FAR* aRotL2L3FZPtr, 

   float FAR* aRotL2L3MXPtr, 
   float FAR* aRotL2L3MYPtr, 
   float FAR* aRotL2L3MZPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL lOK =
      aGloL5S1MXPtr != 0 && 
      aGloL5S1MYPtr != 0 && 
      aGloL5S1MZPtr != 0 && 

      aRotL2L3FXPtr != 0 && 
      aRotL2L3FYPtr != 0 && 
      aRotL2L3FZPtr != 0 && 

      aRotL2L3MXPtr != 0 && 
      aRotL2L3MYPtr != 0 && 
      aRotL2L3MZPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetSpinalB: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   VtoPf(this->GetSkeleton()->getNetMoment(JT_L5S1_TEMP, true), aGloL5S1MXPtr, aGloL5S1MYPtr, aGloL5S1MZPtr);
   AtoPf(this->GetSkeleton()->getSpinalForces(true).rotatedL2L3, aRotL2L3FXPtr, aRotL2L3FYPtr, aRotL2L3FZPtr);
   AtoPf(this->GetSkeleton()->getSpinalMoments(true).rotatedL2L3, aRotL2L3MXPtr, aRotL2L3MYPtr, aRotL2L3MZPtr);
/*
   //--- Local References

   int        lSex              =  mResults.sex;
   link3 &    lL2L3RotForce     =  mResults.L3_force_prime;
   link22 &   lRotMom           =  mResults.mhinge;

   const float   lC = U_M_CM;

   //--- Global Forces and Moments ---
   
   Force   lForX( 0, 1 );
   Force   lForY( 0, 1 );
   Force   lForZ( 0, 1 );

   Torque  lMomX( 0, 1 );
   Torque  lMomY( 0, 1 );
   Torque  lMomZ( 0, 1 );

   // L5/S1

   mResults.JointTorque( lSex, JT_L5S1, lMomX, lMomY, lMomZ );
      * aGloL5S1MXPtr = float(lMomX.Value());
      * aGloL5S1MYPtr = float(lMomY.Value());
      * aGloL5S1MZPtr = float(lMomZ.Value());

   //--- Rotated Forces and Moments ---

   // L2/L3

   lForX.Value( lL2L3RotForce[lSex][1], MetricCns );
   lForY.Value( lL2L3RotForce[lSex][2], MetricCns );
   lForZ.Value( lL2L3RotForce[lSex][3], MetricCns );

      * aRotL2L3FXPtr = float(lForX.Value());
      * aRotL2L3FYPtr = float(lForY.Value());
      * aRotL2L3FZPtr = float(lForZ.Value());

   lMomX.Value( lC * lRotMom[lSex][21], MetricCns );
   lMomY.Value( lC * lRotMom[lSex][22], MetricCns );
   lMomZ.Value( lC * lRotMom[lSex][23], MetricCns );

      * aRotL2L3MXPtr = float(lMomX.Value());
      * aRotL2L3MYPtr = float(lMomY.Value());
      * aRotL2L3MZPtr = float(lMomZ.Value());
*/
	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetSpinalC
(
   float FAR* aRotL3L4FXPtr, 
   float FAR* aRotL3L4FYPtr, 
   float FAR* aRotL3L4FZPtr, 

   float FAR* aRotL3L4MXPtr, 
   float FAR* aRotL3L4MYPtr, 
   float FAR* aRotL3L4MZPtr, 

   float FAR* aRotL4L5FXPtr, 
   float FAR* aRotL4L5FYPtr, 
   float FAR* aRotL4L5FZPtr, 

   float FAR* aRotL4L5MXPtr, 
   float FAR* aRotL4L5MYPtr, 
   float FAR* aRotL4L5MZPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL  lOK =
      aRotL3L4FXPtr != 0 && 
      aRotL3L4FYPtr != 0 && 
      aRotL3L4FZPtr != 0 && 

      aRotL3L4MXPtr != 0 && 
      aRotL3L4MYPtr != 0 && 
      aRotL3L4MZPtr != 0 && 

      aRotL4L5FXPtr != 0 && 
      aRotL4L5FYPtr != 0 && 
      aRotL4L5FZPtr != 0 && 

      aRotL4L5MXPtr != 0 && 
      aRotL4L5MYPtr != 0 && 
      aRotL4L5MZPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetSpinalC: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();
   
   VtoPf((Vector3(this->GetSkeleton()->getSpinalForces(true).rotatedL2L3) + Vector3(this->GetSkeleton()->getSpinalForces(true).rotatedL3L4))/2,
	   aRotL3L4FXPtr, aRotL3L4FYPtr, aRotL3L4FZPtr);

   VtoPf((Vector3(this->GetSkeleton()->getSpinalMoments(true).rotatedL2L3) + Vector3(this->GetSkeleton()->getSpinalMoments(true).rotatedL4L5))/2,
	   aRotL3L4MXPtr, aRotL3L4MYPtr, aRotL3L4MZPtr);

   AtoPf(this->GetSkeleton()->getSpinalForces(true).rotatedL3L4,
	   aRotL4L5FXPtr, aRotL4L5FYPtr, aRotL4L5FZPtr);

   AtoPf(this->GetSkeleton()->getSpinalMoments(true).rotatedL4L5,
	   aRotL4L5MXPtr, aRotL4L5MYPtr, aRotL4L5MZPtr);

   //--- Local References
/*
   int        lSex              =  this->mResults.sex;
   link3 &    lL2L3RotForce     =  mResults.L3_force_prime;
   link3 &    lL3L4RotForce     =  this->mResults.L4_force_prime;
   link22 &   lRotMom           =  this->mResults.mhinge;
   ttarray &  lGloMom           =  this->mResults.ttorq;

   const float   lC = U_M_CM;

   //--- Global Forces and Moments ---
   
   Force   lForX( 0, 1 );
   Force   lForY( 0, 1 );
   Force   lForZ( 0, 1 );

   Torque  lMomX( 0, 1 );
   Torque  lMomY( 0, 1 );
   Torque  lMomZ( 0, 1 );

		
   // L3/L4

   lForX.Value( 0.5F * ( lL3L4RotForce[lSex][1] + lL2L3RotForce[lSex][1] ), MetricCns );
   lForY.Value( 0.5F * ( lL3L4RotForce[lSex][2] + lL2L3RotForce[lSex][2] ), MetricCns );
   lForZ.Value( 0.5F * ( lL3L4RotForce[lSex][3] + lL2L3RotForce[lSex][3] ), MetricCns );

   * aRotL3L4FXPtr = float(lForX.Value());
   * aRotL3L4FYPtr = float(lForY.Value());
   * aRotL3L4FZPtr = float(lForZ.Value());
   
   lMomX.Value( lC * ( 0.5F * ( lRotMom[lSex][24] + lRotMom[lSex][21] ) ), MetricCns );
   lMomY.Value( lC * ( 0.5F * ( lRotMom[lSex][25] + lRotMom[lSex][22] ) ), MetricCns );
   lMomZ.Value( lC * ( 0.5F * ( lRotMom[lSex][26] + lRotMom[lSex][23] ) ), MetricCns );

   * aRotL3L4MXPtr = float(lMomX.Value());
   * aRotL3L4MYPtr = float(lMomY.Value());
   * aRotL3L4MZPtr = float(lMomZ.Value());

   // L4/L5

   lForX.Value( lL3L4RotForce[lSex][1], MetricCns );
   lForY.Value( lL3L4RotForce[lSex][2], MetricCns );
   lForZ.Value( lL3L4RotForce[lSex][3], MetricCns );

   * aRotL4L5FXPtr = float(lForX.Value());
   * aRotL4L5FYPtr = float(lForY.Value());
   * aRotL4L5FZPtr = float(lForZ.Value());
   
   lMomX.Value( lC * lRotMom[lSex][24], MetricCns );
   lMomY.Value( lC * lRotMom[lSex][25], MetricCns );
   lMomZ.Value( lC * lRotMom[lSex][26], MetricCns );

   * aRotL4L5MXPtr = float(lMomX.Value());
   * aRotL4L5MYPtr = float(lMomY.Value());
   * aRotL4L5MZPtr = float(lMomZ.Value());*/

	return TRUE;
}

//---------------------------------------------------------------------------
   void
StrengthLine
(
   //Analysis &  aResult ,
   Skeleton* skel,
   int         aIndex ,
   float *     aMomentPtr ,
   BSTR  *     aEffectPtr ,
   float *     aMeanPtr ,
   float *     aSDPtr ,
   short *     aCapPtr
)
{

	
   *aMomentPtr = float(skel->getStrengthData(true).Moment[aIndex]);
   *aMeanPtr = float(skel->getStrengthData(true).Mean[aIndex]);
   *aSDPtr = float(skel->getStrengthData(true).SD[aIndex]);
   if(*aMeanPtr < 0.5) {
	   *aMeanPtr = 0;
	   *aSDPtr = 0;
   }
   CString lEffect(skel->getStrengthData(true).Effect[aIndex]);
   lEffect.SetSysString(aEffectPtr);
   *aCapPtr = short(skel->getStrengthData(true).Percentile[aIndex]);

   /*int      lSex    = aResult.sex;
   float    lMomentRaw;
   
   Torque   lMoment( 0.0F, 0 );
   Torque   lMean( 0.0F, 0 );
   Torque   lSD( 0.0F, 0 );

   const float   lC = U_M_CM;

   lMomentRaw = aResult.mhinge[lSex][aIndex];

   lMoment.Value( lC * lMomentRaw, MetricCns );
   * aMomentPtr = float(lMoment.Value());

   CString   lEffect( aResult.muscleact[lSex][aIndex] );
   lEffect.SetSysString( aEffectPtr );
   
   if( fabs( lMomentRaw ) < 0.5F )
   {
      * aMeanPtr = 0.0;
      * aSDPtr   = 0.0;
   }
   
   else
   {
      lMean.Value( lC * aResult.vtorq[lSex][aIndex], MetricCns );
      * aMeanPtr  = float(lMean.Value());
      
      lSD.Value( lC * aResult.stddev[lSex][aIndex], MetricCns );
      * aSDPtr = float(lSD.Value());
   }

   * aCapPtr = short(aResult.zprob[lSex][aIndex]);*/
   
   return;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetStrengthA
(
   float FAR* aLeftElbFlxExtMMPtr, 
   BSTR  FAR* aLeftElbFlxExtMEPtr, 
   float FAR* aLeftElbFlxExtMNPtr, 
   float FAR* aLeftElbFlxExtSDPtr, 
   short FAR* aLeftElbFlxExtCPPtr, 

   float FAR* aRightElbFlxExtMMPtr, 
   BSTR  FAR* aRightElbFlxExtMEPtr, 
   float FAR* aRightElbFlxExtMNPtr, 
   float FAR* aRightElbFlxExtSDPtr, 
   short FAR* aRightElbFlxExtCPPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
      aLeftElbFlxExtMMPtr != 0 && 
      aLeftElbFlxExtMEPtr != 0 && 
      aLeftElbFlxExtMNPtr != 0 && 
      aLeftElbFlxExtSDPtr != 0 && 
      aLeftElbFlxExtCPPtr != 0 && 

      aRightElbFlxExtMMPtr != 0 && 
      aRightElbFlxExtMEPtr != 0 && 
      aRightElbFlxExtMNPtr != 0 && 
      aRightElbFlxExtSDPtr != 0 && 
      aRightElbFlxExtCPPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetStrengthA: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();
   
   StrengthLine
      ( 
         //this->mResults, 5, 
		 this->GetSkeleton(), SH_LElbowFlexion,
         aLeftElbFlxExtMMPtr, aLeftElbFlxExtMEPtr, 
         aLeftElbFlxExtMNPtr, aLeftElbFlxExtSDPtr, aLeftElbFlxExtCPPtr
      );

   StrengthLine
      ( 
         //this->mResults, 1, 
		 this->GetSkeleton(), SH_RElbowFlexion,
         aRightElbFlxExtMMPtr, aRightElbFlxExtMEPtr, 
         aRightElbFlxExtMNPtr, aRightElbFlxExtSDPtr, aRightElbFlxExtCPPtr
      );

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetStrengthB
(
   float FAR* aLeftShoHumRotMMPtr, 
   BSTR  FAR* aLeftShoHumRotMEPtr, 
   float FAR* aLeftShoHumRotMNPtr, 
   float FAR* aLeftShoHumRotSDPtr, 
   short FAR* aLeftShoHumRotCPPtr, 

   float FAR* aRightShoHumRotMMPtr, 
   BSTR  FAR* aRightShoHumRotMEPtr, 
   float FAR* aRightShoHumRotMNPtr, 
   float FAR* aRightShoHumRotSDPtr, 
   short FAR* aRightShoHumRotCPPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
      aLeftShoHumRotMMPtr != 0 && 
      aLeftShoHumRotMEPtr != 0 && 
      aLeftShoHumRotMNPtr != 0 && 
      aLeftShoHumRotSDPtr != 0 && 
      aLeftShoHumRotCPPtr != 0 && 

      aRightShoHumRotMMPtr != 0 && 
      aRightShoHumRotMEPtr != 0 && 
      aRightShoHumRotMNPtr != 0 && 
      aRightShoHumRotSDPtr != 0 && 
      aRightShoHumRotCPPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetStrengthB: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();
   
   StrengthLine
      ( 
         //this->mResults, 6, 
		 this->GetSkeleton(), SH_LShoulderRotation,
         aLeftShoHumRotMMPtr, aLeftShoHumRotMEPtr, 
         aLeftShoHumRotMNPtr, aLeftShoHumRotSDPtr, aLeftShoHumRotCPPtr
      );

   StrengthLine
      ( 
         //this->mResults, 2,
		 this->GetSkeleton(), SH_RShoulderRotation,
         aRightShoHumRotMMPtr, aRightShoHumRotMEPtr, 
         aRightShoHumRotMNPtr, aRightShoHumRotSDPtr, aRightShoHumRotCPPtr
      );

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetStrengthC
(
   float FAR* aLeftShoBkFdMMPtr, 
   BSTR  FAR* aLeftShoBkFdMEPtr, 
   float FAR* aLeftShoBkFdMNPtr, 
   float FAR* aLeftShoBkFdSDPtr, 
   short FAR* aLeftShoBkFdCPPtr, 

   float FAR* aRightShoBkFdMMPtr, 
   BSTR  FAR* aRightShoBkFdMEPtr, 
   float FAR* aRightShoBkFdMNPtr, 
   float FAR* aRightShoBkFdSDPtr, 
   short FAR* aRightShoBkFdCPPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
      aLeftShoBkFdMMPtr != 0 &&
      aLeftShoBkFdMEPtr != 0 &&
      aLeftShoBkFdMNPtr != 0 &&
      aLeftShoBkFdSDPtr != 0 &&
      aLeftShoBkFdCPPtr != 0 &&

      aRightShoBkFdMMPtr != 0 &&
      aRightShoBkFdMEPtr != 0 &&
      aRightShoBkFdMNPtr != 0 &&
      aRightShoBkFdSDPtr != 0 &&
      aRightShoBkFdCPPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetStrengthC: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();
   
   StrengthLine
      ( 
         //this->mResults, 7, 
		 // TODO does rotation correspond to back / forward?
		 this->GetSkeleton(), SH_LShoulderRotation,
         aLeftShoBkFdMMPtr, aLeftShoBkFdMEPtr, 
         aLeftShoBkFdMNPtr, aLeftShoBkFdSDPtr, aLeftShoBkFdCPPtr
      );

   StrengthLine
      ( 
         //this->mResults, 3, 
		 // TODO does rotation correspond to shoulder back/forward?
		 this->GetSkeleton(), SH_RShoulderRotation,
         aRightShoBkFdMMPtr, aRightShoBkFdMEPtr, 
         aRightShoBkFdMNPtr, aRightShoBkFdSDPtr, aRightShoBkFdCPPtr
      );

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetStrengthD
(
   float FAR* aLeftShoAbAdMMPtr, 
   BSTR  FAR* aLeftShoAbAdMEPtr, 
   float FAR* aLeftShoAbAdMNPtr, 
   float FAR* aLeftShoAbAdSDPtr, 
   short FAR* aLeftShoAbAdCPPtr, 

   float FAR* aRightShoAbAdMMPtr, 
   BSTR  FAR* aRightShoAbAdMEPtr, 
   float FAR* aRightShoAbAdMNPtr, 
   float FAR* aRightShoAbAdSDPtr, 
   short FAR* aRightShoAbAdCPPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
      aLeftShoAbAdMMPtr != 0 && 
      aLeftShoAbAdMEPtr != 0 && 
      aLeftShoAbAdMNPtr != 0 && 
      aLeftShoAbAdSDPtr != 0 && 
      aLeftShoAbAdCPPtr != 0 && 

      aRightShoAbAdMMPtr != 0 && 
      aRightShoAbAdMEPtr != 0 && 
      aRightShoAbAdMNPtr != 0 && 
      aRightShoAbAdSDPtr != 0 && 
      aRightShoAbAdCPPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetStrengthD: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();
   
   StrengthLine
      ( 
         //this->mResults, 8, 
		 this->GetSkeleton(), SH_LShoulderAbduction,
         aLeftShoAbAdMMPtr, aLeftShoAbAdMEPtr, 
         aLeftShoAbAdMNPtr, aLeftShoAbAdSDPtr, aLeftShoAbAdCPPtr
      );

   StrengthLine
      ( 
         //this->mResults, 4, 
		 this->GetSkeleton(), SH_RShoulderAbduction,
         aRightShoAbAdMMPtr, aRightShoAbAdMEPtr, 
         aRightShoAbAdMNPtr, aRightShoAbAdSDPtr, aRightShoAbAdCPPtr
      );

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetStrengthE
(
   float FAR* aTruFlxExtMMPtr, 
   BSTR  FAR* aTruFlxExtMEPtr, 
   float FAR* aTruFlxExtMNPtr, 
   float FAR* aTruFlxExtSDPtr, 
   short FAR* aTruFlxExtCPPtr, 

   float FAR* aTruLatBndMMPtr, 
   BSTR  FAR* aTruLatBndMEPtr, 
   float FAR* aTruLatBndMNPtr, 
   float FAR* aTruLatBndSDPtr, 
   short FAR* aTruLatBndCPPtr, 

   float FAR* aTruRotMMPtr, 
   BSTR  FAR* aTruRotMEPtr, 
   float FAR* aTruRotMNPtr, 
   float FAR* aTruRotSDPtr, 
   short FAR* aTruRotCPPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
      aTruFlxExtMMPtr != 0 && 
      aTruFlxExtMEPtr != 0 && 
      aTruFlxExtMNPtr != 0 && 
      aTruFlxExtSDPtr != 0 && 
      aTruFlxExtCPPtr != 0 && 

      aTruLatBndMMPtr != 0 && 
      aTruLatBndMEPtr != 0 && 
      aTruLatBndMNPtr != 0 && 
      aTruLatBndSDPtr != 0 && 
      aTruLatBndCPPtr != 0 && 

      aTruRotMMPtr != 0 && 
      aTruRotMEPtr != 0 && 
      aTruRotMNPtr != 0 && 
      aTruRotSDPtr != 0 && 
      aTruRotCPPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetStrengthE: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();
   
   StrengthLine
      ( 
         //this->mResults, 9, 
		 this->GetSkeleton(), SH_TorsoFlexion,
         aTruFlxExtMMPtr, aTruFlxExtMEPtr, 
         aTruFlxExtMNPtr, aTruFlxExtSDPtr, aTruFlxExtCPPtr
      );

   StrengthLine
      ( 
         //this->mResults, 10, 
		 this->GetSkeleton(), SH_TorsoLateralBending,
         aTruLatBndMMPtr, aTruLatBndMEPtr, 
         aTruLatBndMNPtr, aTruLatBndSDPtr, aTruLatBndCPPtr
      );

   StrengthLine
      ( 
         //this->mResults, 11, 
		 this->GetSkeleton(), SH_TorsoRotation,
         aTruRotMMPtr, aTruRotMEPtr, 
         aTruRotMNPtr, aTruRotSDPtr, aTruRotCPPtr
      );

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetStrengthF
(
   float FAR* aLeftHipFlxExtMMPtr, 
   BSTR  FAR* aLeftHipFlxExtMEPtr, 
   float FAR* aLeftHipFlxExtMNPtr, 
   float FAR* aLeftHipFlxExtSDPtr, 
   short FAR* aLeftHipFlxExtCPPtr, 

   float FAR* aRightHipFlxExtMMPtr, 
   BSTR  FAR* aRightHipFlxExtMEPtr, 
   float FAR* aRightHipFlxExtMNPtr, 
   float FAR* aRightHipFlxExtSDPtr, 
   short FAR* aRightHipFlxExtCPPtr
) 
{
	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
      aLeftHipFlxExtMMPtr != 0 && 
      aLeftHipFlxExtMEPtr != 0 && 
      aLeftHipFlxExtMNPtr != 0 && 
      aLeftHipFlxExtSDPtr != 0 && 
      aLeftHipFlxExtCPPtr != 0 && 

      aRightHipFlxExtMMPtr != 0 && 
      aRightHipFlxExtMEPtr != 0 && 
      aRightHipFlxExtMNPtr != 0 && 
      aRightHipFlxExtSDPtr != 0 && 
      aRightHipFlxExtCPPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetStrengthF: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();
   
   StrengthLine
      ( 
         //this->mResults, 15, 
		 this->GetSkeleton(), SH_LHipFlexion,
         aLeftHipFlxExtMMPtr, aLeftHipFlxExtMEPtr, 
         aLeftHipFlxExtMNPtr, aLeftHipFlxExtSDPtr, aLeftHipFlxExtCPPtr
      );

   StrengthLine
      ( 
         //this->mResults, 12, 
		 this->GetSkeleton(), SH_RHipFlexion,
         aRightHipFlxExtMMPtr, aRightHipFlxExtMEPtr, 
         aRightHipFlxExtMNPtr, aRightHipFlxExtSDPtr, aRightHipFlxExtCPPtr
      );

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetStrengthG
(
   float FAR* aLeftKneeFlxExtMMPtr, 
   BSTR  FAR* aLeftKneeFlxExtMEPtr, 
   float FAR* aLeftKneeFlxExtMNPtr, 
   float FAR* aLeftKneeFlxExtSDPtr, 
   short FAR* aLeftKneeFlxExtCPPtr, 

   float FAR* aRightKneeFlxExtMMPtr, 
   BSTR  FAR* aRightKneeFlxExtMEPtr, 
   float FAR* aRightKneeFlxExtMNPtr, 
   float FAR* aRightKneeFlxExtSDPtr, 
   short FAR* aRightKneeFlxExtCPPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
      aLeftKneeFlxExtMMPtr != 0 && 
      aLeftKneeFlxExtMEPtr != 0 && 
      aLeftKneeFlxExtMNPtr != 0 && 
      aLeftKneeFlxExtSDPtr != 0 && 
      aLeftKneeFlxExtCPPtr != 0 && 

      aRightKneeFlxExtMMPtr != 0 && 
      aRightKneeFlxExtMEPtr != 0 && 
      aRightKneeFlxExtMNPtr != 0 && 
      aRightKneeFlxExtSDPtr != 0 && 
      aRightKneeFlxExtCPPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetStrengthG: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();
   
   StrengthLine
      ( 
         //this->mResults, 16, 
		 this->GetSkeleton(), SH_LKneeFlexion,
         aLeftKneeFlxExtMMPtr, aLeftKneeFlxExtMEPtr, 
         aLeftKneeFlxExtMNPtr, aLeftKneeFlxExtSDPtr, aLeftKneeFlxExtCPPtr
      );

   StrengthLine
      ( 
         //this->mResults, 13, 
		 this->GetSkeleton(), SH_RKneeFlexion,
         aRightKneeFlxExtMMPtr, aRightKneeFlxExtMEPtr, 
         aRightKneeFlxExtMNPtr, aRightKneeFlxExtSDPtr, aRightKneeFlxExtCPPtr
      );

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetStrengthH
(
   float FAR* aLeftAnkFlxExtMMPtr, 
   BSTR  FAR* aLeftAnkFlxExtMEPtr, 
   float FAR* aLeftAnkFlxExtMNPtr, 
   float FAR* aLeftAnkFlxExtSDPtr, 
   short FAR* aLeftAnkFlxExtCPPtr, 

   float FAR* aRightAnkFlxExtMMPtr, 
   BSTR  FAR* aRightAnkFlxExtMEPtr, 
   float FAR* aRightAnkFlxExtMNPtr, 
   float FAR* aRightAnkFlxExtSDPtr,
   short FAR* aRightAnkFlxExtCPPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
      aLeftAnkFlxExtMMPtr != 0 && 
      aLeftAnkFlxExtMEPtr != 0 && 
      aLeftAnkFlxExtMNPtr != 0 && 
      aLeftAnkFlxExtSDPtr != 0 && 
      aLeftAnkFlxExtCPPtr != 0 && 

      aRightAnkFlxExtMMPtr != 0 && 
      aRightAnkFlxExtMEPtr != 0 && 
      aRightAnkFlxExtMNPtr != 0 && 
      aRightAnkFlxExtSDPtr != 0 &&
      aRightAnkFlxExtCPPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetStrengthH: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();
   
   StrengthLine
      ( 
         //this->mResults, 17, 
		 this->GetSkeleton(), SH_LAnkleFlexion,
         aLeftAnkFlxExtMMPtr, aLeftAnkFlxExtMEPtr, 
         aLeftAnkFlxExtMNPtr, aLeftAnkFlxExtSDPtr, aLeftAnkFlxExtCPPtr
      );

   StrengthLine
      ( 
         //this->mResults, 14, 
		 this->GetSkeleton(), SH_RAnkleFlexion,
         aRightAnkFlxExtMMPtr, aRightAnkFlxExtMEPtr, 
         aRightAnkFlxExtMNPtr, aRightAnkFlxExtSDPtr, aRightAnkFlxExtCPPtr
      );

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetStrengthVectorsA
(
   float FAR* aLeftElbFlxExtXPtr, 
   float FAR* aLeftElbFlxExtYPtr, 
   float FAR* aLeftElbFlxExtZPtr, 

   float FAR* aRightElbFlxExtXPtr, 
   float FAR* aRightElbFlxExtYPtr, 
   float FAR* aRightElbFlxExtZPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
      aLeftElbFlxExtXPtr != 0 && 
      aLeftElbFlxExtYPtr != 0 && 
      aLeftElbFlxExtZPtr != 0 && 

      aRightElbFlxExtXPtr != 0 && 
      aRightElbFlxExtYPtr != 0 && 
      aRightElbFlxExtZPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetStrengthH: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   VtoPf(this->GetSkeleton()->getHinge(SH_LElbowFlexion),
	   aLeftElbFlxExtXPtr, aLeftElbFlxExtYPtr, aLeftElbFlxExtZPtr);
   VtoPf(this->GetSkeleton()->getHinge(SH_RElbowFlexion),
	   aRightElbFlxExtXPtr, aRightElbFlxExtYPtr, aRightElbFlxExtZPtr);
/*
   Analysis &   lResult = this->mResults;

   * aLeftElbFlxExtXPtr = Round_Float( float(lResult.hingev[5][1]), 3 );
   * aLeftElbFlxExtYPtr = Round_Float( float(lResult.hingev[5][2]), 3 );
   * aLeftElbFlxExtZPtr = Round_Float( float(lResult.hingev[5][3]), 3 );
   * aRightElbFlxExtXPtr = Round_Float( float(lResult.hingev[1][1]), 3 );
   * aRightElbFlxExtYPtr = Round_Float( float(lResult.hingev[1][2]), 3 );
   * aRightElbFlxExtZPtr = Round_Float( float(lResult.hingev[1][3]), 3 );*/

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetStrengthVectorsB
(
   float FAR* aLeftShoBkFdXPtr, 
   float FAR* aLeftShoBkFdYPtr, 
   float FAR* aLeftShoBkFdZPtr, 

   float FAR* aRightShoBkFdXPtr, 
   float FAR* aRightShoBkFdYPtr, 
   float FAR* aRightShoBkFdZPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
      aLeftShoBkFdXPtr != 0 && 
      aLeftShoBkFdYPtr != 0 && 
      aLeftShoBkFdZPtr != 0 && 

      aRightShoBkFdXPtr != 0 && 
      aRightShoBkFdYPtr != 0 && 
      aRightShoBkFdZPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetStrengthH: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   // TODO does shoulder rotation correspond to back / forward?
   VtoPf(this->GetSkeleton()->getHinge(SH_LShoulderRotation),
	   aLeftShoBkFdXPtr, aLeftShoBkFdYPtr, aLeftShoBkFdZPtr);
   VtoPf(this->GetSkeleton()->getHinge(SH_RShoulderRotation),
	   aRightShoBkFdXPtr, aRightShoBkFdYPtr, aRightShoBkFdZPtr);
/*
   Analysis &   lResult = this->mResults;

   * aLeftShoBkFdXPtr = Round_Float( float(lResult.hingev[7][1]), 3 );
   * aLeftShoBkFdYPtr = Round_Float( float(lResult.hingev[7][2]), 3 );
   * aLeftShoBkFdZPtr = Round_Float( float(lResult.hingev[7][3]), 3 );
   * aRightShoBkFdXPtr = Round_Float( float(lResult.hingev[3][1]), 3 );
   * aRightShoBkFdYPtr = Round_Float( float(lResult.hingev[3][2]), 3 );
   * aRightShoBkFdZPtr = Round_Float( float(lResult.hingev[3][3]), 3 );*/

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetStrengthVectorsC
(
   float FAR* aLeftShoAbAdXPtr, 
   float FAR* aLeftShoAbAdYPtr, 
   float FAR* aLeftShoAbAdZPtr, 

   float FAR* aRightShoAbAdXPtr, 
   float FAR* aRightShoAbAdYPtr, 
   float FAR* aRightShoAbAdZPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
      aLeftShoAbAdXPtr != 0 && 
      aLeftShoAbAdYPtr != 0 && 
      aLeftShoAbAdZPtr != 0 && 

      aRightShoAbAdXPtr != 0 && 
      aRightShoAbAdYPtr != 0 && 
      aRightShoAbAdZPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetStrengthVectorsC: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   VtoPf(this->GetSkeleton()->getHinge(SH_LShoulderAbduction),
	   aLeftShoAbAdXPtr, aLeftShoAbAdYPtr, aLeftShoAbAdZPtr);
   VtoPf(this->GetSkeleton()->getHinge(SH_RShoulderAbduction),
	   aRightShoAbAdXPtr, aRightShoAbAdYPtr, aRightShoAbAdZPtr);
/*
   Analysis &   lResult = this->mResults;

   * aLeftShoAbAdXPtr = Round_Float( float(lResult.hingev[8][1]), 3 );
   * aLeftShoAbAdYPtr = Round_Float( float(lResult.hingev[8][2]), 3 );
   * aLeftShoAbAdZPtr = Round_Float( float(lResult.hingev[8][3]), 3 );
   * aRightShoAbAdXPtr = Round_Float( float(lResult.hingev[4][1]), 3 );
   * aRightShoAbAdYPtr = Round_Float( float(lResult.hingev[4][2]), 3 );
   * aRightShoAbAdZPtr = Round_Float( float(lResult.hingev[4][3]), 3 );*/

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetStrengthVectorsD
(
   float FAR* aLeftShoHumRotXPtr, 
   float FAR* aLeftShoHumRotYPtr, 
   float FAR* aLeftShoHumRotZPtr, 

   float FAR* aRightShoHumRotXPtr, 
   float FAR* aRightShoHumRotYPtr, 
   float FAR* aRightShoHumRotZPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
      aLeftShoHumRotXPtr != 0 && 
      aLeftShoHumRotYPtr != 0 && 
      aLeftShoHumRotZPtr != 0 && 

      aRightShoHumRotXPtr != 0 && 
      aRightShoHumRotYPtr != 0 && 
      aRightShoHumRotZPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetStrengthVectorsD: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   VtoPf(this->GetSkeleton()->getHinge(SH_LHumeralRotation),
	   aLeftShoHumRotXPtr, aLeftShoHumRotYPtr, aLeftShoHumRotZPtr);
   VtoPf(this->GetSkeleton()->getHinge(SH_RHumeralRotation),
	   aRightShoHumRotXPtr, aRightShoHumRotYPtr, aRightShoHumRotZPtr);
/*
   Analysis &   lResult = this->mResults;

   * aLeftShoHumRotXPtr = Round_Float( float(lResult.hingev[6][1]), 3 );
   * aLeftShoHumRotYPtr = Round_Float( float(lResult.hingev[6][2]), 3 );
   * aLeftShoHumRotZPtr = Round_Float( float(lResult.hingev[6][3]), 3 );
   * aRightShoHumRotXPtr = Round_Float( float(lResult.hingev[2][1]), 3 );
   * aRightShoHumRotYPtr = Round_Float( float(lResult.hingev[2][2]), 3 );
   * aRightShoHumRotZPtr = Round_Float( float(lResult.hingev[2][3]), 3 );*/

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetStrengthVectorsE
(
   float FAR* aL5S1FlxExtXPtr, 
   float FAR* aL5S1FlxExtYPtr, 
   float FAR* aL5S1FlxExtZPtr, 

   float FAR* aL5S1LatBndXPtr, 
   float FAR* aL5S1LatBndYPtr, 
   float FAR* aL5S1LatBndZPtr, 

   float FAR* aL5S1RotXPtr, 
   float FAR* aL5S1RotYPtr, 
   float FAR* aL5S1RotZPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
      aL5S1FlxExtXPtr != 0 && 
      aL5S1FlxExtYPtr != 0 && 
      aL5S1FlxExtZPtr != 0 && 

      aL5S1LatBndXPtr != 0 && 
      aL5S1LatBndYPtr != 0 && 
      aL5S1LatBndZPtr != 0 && 

      aL5S1RotXPtr != 0 && 
      aL5S1RotYPtr != 0 && 
      aL5S1RotZPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetStrengthVectorsE: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   // TODO we do not have these vectors anymore ?
/*
   Analysis &   lResult = this->mResults;

   * aL5S1FlxExtXPtr = Round_Float( float(lResult.hingev[9][1]), 3 );
   * aL5S1FlxExtYPtr = Round_Float( float(lResult.hingev[9][2]), 3 );
   * aL5S1FlxExtZPtr = Round_Float( float(lResult.hingev[9][3]), 3 );

   * aL5S1LatBndXPtr = Round_Float( float(lResult.hingev[10][1]), 3 );
   * aL5S1LatBndYPtr = Round_Float( float(lResult.hingev[10][2]), 3 );
   * aL5S1LatBndZPtr = Round_Float( float(lResult.hingev[10][3]), 3 );      

   * aL5S1RotXPtr = Round_Float( float(lResult.hingev[11][1]), 3 );
   * aL5S1RotYPtr = Round_Float( float(lResult.hingev[11][2]), 3 );
   * aL5S1RotZPtr = Round_Float( float(lResult.hingev[11][3]), 3 );      */

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetStrengthVectorsF
(
   float FAR* aLeftHipFlxExtXPtr, 
   float FAR* aLeftHipFlxExYPtr, 
   float FAR* aLeftHipFlxExtZPtr, 

   float FAR* aRightHipFlxExtXPtr, 
   float FAR* aRightHipFlxExtYPtr, 
   float FAR* aRightHipFlxExtZPtr
) 
{

	 // disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
      aLeftHipFlxExtXPtr != 0 && 
      aLeftHipFlxExYPtr != 0 && 
      aLeftHipFlxExtZPtr != 0 && 

      aRightHipFlxExtXPtr != 0 && 
      aRightHipFlxExtYPtr != 0 && 
      aRightHipFlxExtZPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetStrengthVectorsF: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   VtoPf(this->GetSkeleton()->getHinge(SH_LHipFlexion),
	   aLeftHipFlxExtXPtr, aLeftHipFlxExtZPtr, aLeftHipFlxExYPtr);
   VtoPf(this->GetSkeleton()->getHinge(SH_RHipFlexion),
	   aRightHipFlxExtXPtr, aRightHipFlxExtYPtr, aRightHipFlxExtZPtr);

   /*
   Analysis &   lResult = this->mResults;

   * aLeftHipFlxExtXPtr = Round_Float( float(lResult.hingev[15][1]), 3 );
   * aLeftHipFlxExYPtr = Round_Float( float(lResult.hingev[15][2]), 3 );
   * aLeftHipFlxExtZPtr = Round_Float( float(lResult.hingev[15][3]), 3 );
   * aRightHipFlxExtXPtr = Round_Float( float(lResult.hingev[12][1]), 3 );
   * aRightHipFlxExtYPtr = Round_Float( float(lResult.hingev[12][2]), 3 );
   * aRightHipFlxExtZPtr = Round_Float( float(lResult.hingev[12][3]), 3 );*/

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetStrengthVectorsG
(
   float FAR* aLeftKneeFlxExtXPtr, 
   float FAR* aLeftKneeFlxExtYPtr, 
   float FAR* aLeftKneeFlxExtZPtr, 

   float FAR* aRightKneeFlxExtXPtr, 
   float FAR* aRightKneeFlxExtYPtr, 
   float FAR* aRightKneeFlxExtZPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
      aLeftKneeFlxExtXPtr != 0 && 
      aLeftKneeFlxExtYPtr != 0 && 
      aLeftKneeFlxExtZPtr != 0 && 

      aRightKneeFlxExtXPtr != 0 && 
      aRightKneeFlxExtYPtr != 0 && 
      aRightKneeFlxExtZPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetStrengthVectorsG: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   VtoPf(this->GetSkeleton()->getHinge(SH_LKneeFlexion),
	   aLeftKneeFlxExtXPtr, aLeftKneeFlxExtYPtr, aLeftKneeFlxExtZPtr);
   VtoPf(this->GetSkeleton()->getHinge(SH_RKneeFlexion),
	   aRightKneeFlxExtXPtr, aRightKneeFlxExtYPtr, aRightKneeFlxExtZPtr);

   /*
   Analysis &   lResult = this->mResults;

   * aLeftKneeFlxExtXPtr = Round_Float( float(lResult.hingev[16][1]), 3 );
   * aLeftKneeFlxExtYPtr = Round_Float( float(lResult.hingev[16][2]), 3 );
   * aLeftKneeFlxExtZPtr = Round_Float( float(lResult.hingev[16][3]), 3 );
   * aRightKneeFlxExtXPtr = Round_Float( float(lResult.hingev[13][1]), 3 );
   * aRightKneeFlxExtYPtr = Round_Float( float(lResult.hingev[13][2]), 3 );
   * aRightKneeFlxExtZPtr = Round_Float( float(lResult.hingev[13][3]), 3 );*/

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetStrengthVectorsH
(
   float FAR* aLeftAnkleFlxExtXPtr, 
   float FAR* aLeftAnkleFlxExtYPtr, 
   float FAR* aLeftAnkleFlxExtZPtr, 

   float FAR* aRightAnkleFlxExtXPtr, 
   float FAR* aRightAnkleFlxExtYPtr, 
   float FAR* aRightAnkleFlxExtZPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK = 
      aLeftAnkleFlxExtXPtr != 0 && 
      aLeftAnkleFlxExtYPtr != 0 && 
      aLeftAnkleFlxExtZPtr != 0 && 

      aRightAnkleFlxExtXPtr != 0 && 
      aRightAnkleFlxExtYPtr != 0 && 
      aRightAnkleFlxExtZPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetStrengthVectorsH: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   VtoPf(this->GetSkeleton()->getHinge(SH_LAnkleFlexion),
	   aLeftAnkleFlxExtXPtr, aLeftAnkleFlxExtYPtr, aLeftAnkleFlxExtZPtr);
   VtoPf(this->GetSkeleton()->getHinge(SH_RAnkleFlexion),
	   aRightAnkleFlxExtXPtr, aRightAnkleFlxExtYPtr, aRightAnkleFlxExtZPtr);

   /*
   Analysis &   lResult = this->mResults;

   * aLeftAnkleFlxExtXPtr = Round_Float( float(lResult.hingev[17][1]), 3 );
   * aLeftAnkleFlxExtYPtr = Round_Float( float(lResult.hingev[17][2]), 3 );
   * aLeftAnkleFlxExtZPtr = Round_Float( float(lResult.hingev[17][3]), 3 );
   * aRightAnkleFlxExtXPtr = Round_Float( float(lResult.hingev[14][1]), 3 );
   * aRightAnkleFlxExtYPtr = Round_Float( float(lResult.hingev[14][2]), 3 );
   * aRightAnkleFlxExtZPtr = Round_Float( float(lResult.hingev[14][3]), 3 );  */    

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetTaskInputSummaryA
(
   short FAR* aLeftForeArmHPtr, 
   short FAR* aLeftForeArmVPtr, 

   short FAR* aRightForeArmHPtr, 
   short FAR* aRightForeArmVPtr, 

   short FAR* aLeftUpperArmHPtr, 
   short FAR* aLeftUpperArmVPtr, 

   short FAR* aRightUpperArmHPtr, 
   short FAR* aRightUpperArmVPtr, 

   short FAR* aLeftUpperLegVPtr, 
   short FAR* aRightUpperLegVPtr, 

   short FAR* aLeftLowerLegVPtr, 
   short FAR* aRightLowerLegVPtr, 

   short FAR* aTrunkFlexionPtr, 
   short FAR* aTrunkRotationPtr,
   short FAR* aTrunkBendingPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL   lOK =
       aLeftForeArmHPtr != 0 && 
       aLeftForeArmVPtr != 0 && 

       aRightForeArmHPtr != 0 && 
       aRightForeArmVPtr != 0 && 

       aLeftUpperArmHPtr != 0 && 
       aLeftUpperArmVPtr != 0 && 

       aRightUpperArmHPtr != 0 && 
       aRightUpperArmVPtr != 0 && 

       aLeftUpperLegVPtr != 0 && 
       aRightUpperLegVPtr != 0 && 

       aLeftLowerLegVPtr != 0 && 
       aRightLowerLegVPtr != 0 && 

       aTrunkFlexionPtr != 0 && 
       aTrunkRotationPtr != 0 &&
       aTrunkBendingPtr;

   if( ! lOK ) 
   {
      mCOMLastErrorMessage = "GetTaskInputSummaryA: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();
   JointAngles ja = this->GetSkeleton()->GetAngles();
   *aLeftForeArmHPtr = short(ja[A_ForearmLH]);
   *aLeftForeArmVPtr = short(ja[A_ForearmLV]);
   *aRightForeArmHPtr = short(ja[A_ForearmRH]);
   *aRightForeArmVPtr = short(ja[A_ForearmRV]);
   *aLeftUpperArmHPtr = short(ja[A_UpperarmLH]);
   *aLeftUpperArmVPtr = short(ja[A_UpperarmLV]);
   *aRightUpperArmHPtr = short(ja[A_UpperarmRH]);
   *aRightUpperArmVPtr = short(ja[A_UpperarmRV]);

   // NOTE leg angle defs have changed so we convert the to old style here
   //ja.swapMyConventions();
   *aLeftUpperLegVPtr = short(ja[A_UpperlegLV]);
   *aLeftLowerLegVPtr = short(ja[A_LowerlegLV]);
   *aRightUpperLegVPtr = short(ja[A_UpperlegRV]);
   *aRightLowerLegVPtr = short(ja[A_LowerlegRV]);

   *aTrunkFlexionPtr = short(ja[A_TrunkFlexion]);
   *aTrunkBendingPtr = short(ja[A_TrunkLateralBending]);
   *aTrunkRotationPtr = short(ja[A_TrunkAxialRotation]);

   /*
	const JointAngles& lCurrentJointAngles = this->JointAngleQueue.GetCurrent();

   * aLeftForeArmHPtr = short(lCurrentJointAngles.ForearmLH());
   * aLeftForeArmVPtr = short(lCurrentJointAngles.ForearmLV());
   
   * aRightForeArmHPtr = short(lCurrentJointAngles.ForearmRH());
   * aRightForeArmVPtr = short(lCurrentJointAngles.ForearmRV());

   * aLeftUpperArmHPtr = short(lCurrentJointAngles.UpperarmLH());
   * aLeftUpperArmVPtr = short(lCurrentJointAngles.UpperarmLV());

   * aRightUpperArmHPtr = short(lCurrentJointAngles.UpperarmRH());
   * aRightUpperArmVPtr = short(lCurrentJointAngles.UpperarmRV());

   * aLeftUpperLegVPtr = short(lCurrentJointAngles.UpperlegLV());
   * aLeftLowerLegVPtr = short(lCurrentJointAngles.LowerlegLV());

   * aRightUpperLegVPtr = short(lCurrentJointAngles.UpperlegRV());
   * aRightLowerLegVPtr = short(lCurrentJointAngles.LowerlegRV());

	* aTrunkFlexionPtr = short(lCurrentJointAngles.TrunkFlexion());
	* aTrunkBendingPtr = short(lCurrentJointAngles.TrunkLateralBending());
	* aTrunkRotationPtr = short(lCurrentJointAngles.TrunkAxialRotation());*/

	return TRUE;
}

//---------------------------------------------------------------------------
   BOOL 
   C_Hom_Doc::
GetTaskInputSummaryB
(
   BSTR  FAR* aHandOrientationPtr, 

   float FAR* aLeftHandLocHPtr, 
   float FAR* aLeftHandLocVPtr, 
   float FAR* aLeftHandLocLPtr, 

   float FAR* aRightHandLocHPtr, 
   float FAR* aRightHandLocVPtr, 
   float FAR* aRightHandLocLPtr, 

   short FAR* aLeftForceAngHPtr, 
   short FAR* aLeftForceAngVPtr, 
   float FAR* aLeftForceMagPtr, 

   short FAR* aRightForceAngHPtr, 
   short FAR* aRightForceAngVPtr, 
   float FAR* aRightForceMagPtr
) 
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   BOOL lOK =
      aHandOrientationPtr != 0 && 

      aLeftHandLocHPtr != 0 && 
      aLeftHandLocVPtr != 0 && 
      aLeftHandLocLPtr != 0 && 

      aRightHandLocHPtr != 0 && 
      aRightHandLocVPtr != 0 && 
      aRightHandLocLPtr != 0 && 

      aLeftForceAngHPtr != 0 && 
      aLeftForceAngVPtr != 0 && 
      aLeftForceMagPtr != 0 && 

      aRightForceAngHPtr != 0 && 
      aRightForceAngVPtr != 0 && 
      aRightForceMagPtr;

   if( !lOK )
   {
      mCOMLastErrorMessage = "GetTaskInputSummaryB: One or more argument pointers are null.";
      return   FALSE;
   }

   this->Analyze();

   // TODO hand orientation text
   //aHandOrientationPtr

   VtoPf(this->GetSkeleton()->getJoint(JT_LHAND, true),
	   aLeftHandLocLPtr, aLeftHandLocHPtr, aLeftHandLocVPtr);
   VtoPf(this->GetSkeleton()->getJoint(JT_RHAND, true),
	   aRightHandLocLPtr, aRightHandLocHPtr, aRightHandLocVPtr);

    double rv,rh,lv,lh;
	Vector3 LHandForce = this->GetSkeleton()->getExtForce(JT_LHAND, true);
	Vector3 RHandForce = this->GetSkeleton()->getExtForce(JT_RHAND, true);
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

	*aLeftForceAngHPtr = short(lh);
	*aLeftForceAngVPtr = short(lv);
	*aLeftForceMagPtr = float(this->GetSkeleton()->getExtForce(JT_LHAND, true).length());

	*aRightForceAngHPtr = short(rh);
	*aRightForceAngVPtr = short(rv);
	*aRightForceMagPtr = float(this->GetSkeleton()->getExtForce(JT_RHAND, true).length());

	/*
	this->UpdateHandPositionsFromDB();

	Skeleton_Record &   SkeletonRecordRef = 
      Get_Graphic_Database_Ptr()->
         Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name );
   
   CString lHandOrientation( SkeletonRecordRef.HandOrientationText() );
   lHandOrientation.SetSysString( aHandOrientationPtr );
   
   *aLeftHandLocHPtr = float(mHandLocLH.Value());
   *aLeftHandLocVPtr = float(mHandLocLV.Value());
   *aLeftHandLocLPtr  = float(mHandLocLL.Value());

   *aRightHandLocHPtr = float(mHandLocRH.Value());
   *aRightHandLocVPtr = float(mHandLocRV.Value());
   *aRightHandLocLPtr = float(mHandLocRL.Value());

   *aLeftForceAngHPtr = short(mHandLoads.LeftHorizontalAngle());
   *aLeftForceAngVPtr = short(mHandLoads.LeftVerticalAngle());
   *aLeftForceMagPtr  = float(mHandLoads.LeftMagnitudeRef().Value());

   *aRightForceAngHPtr = short(mHandLoads.RightHorizontalAngle());
   *aRightForceAngVPtr = short(mHandLoads.RightVerticalAngle());
   *aRightForceMagPtr  = float(mHandLoads.RightMagnitudeRef().Value());*/

   return TRUE;
}

//---------------------------------------------------------------------------
BOOL C_Hom_Doc::SetPopulationDefault()
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   this->RestoreDefaultFactors();
   this->Analyze();
   return true;
}

BOOL C_Hom_Doc::SetZeroGravity()	// Better titled "ToggleZeroGravity"
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   //if (this->Is_Gravity()) 
	if(this->getTask().getGravity() != 0)
      //this->mGravity = 0;
		this->getTask().setGravity(0);
   else
      //this->mGravity = 9.81;
	  this->getTask().setGravity(9.81);

   //this->mResults.Analyze();
	this->Analyze();

   return true;
}

BOOL C_Hom_Doc::SetPopulationFromFile(BSTR FAR* FilePathPtr, BSTR FAR* FileNamePtr)
{

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   CString lFilePathPtr( *FilePathPtr );
   CString lFileNamePtr( *FileNamePtr );

   //this->SetPopFromFile(lFilePathPtr.GetString(), lFileNamePtr.GetString());
   ifstream lIn(mFilePathName.c_str());
   float *stature, *weight;

   Factors lFactors;
   lIn >> lFactors;

   if(GenderCode() == 0)
      SetMaleFactors(lFactors);
   else
      SetFemaleFactors(lFactors);

   C_Hom_App* App_Ptr = Get_App_Ptr();
   C_Hom_Doc* Doc_Ptr = App_Ptr->Get_Document_Ptr();

#if (USE_POPULATION_DISTRIBUTION)
   stature = Doc_Ptr->getAnthropometryRef().GetStatureDistribution();
   weight = Doc_Ptr->getAnthropometryRef().GetWeightDistribution();
   stature[0] = lFactors.mMHmean;
   stature[1] = lFactors.mMHstdev;
   stature[2] = lFactors.mFHmean;
   stature[3] = lFactors.mFHstdev;
   weight[0]  = lFactors.mMWmean;
   weight[1]  = lFactors.mMWstdev;
   weight[2]  = lFactors.mFWmean;
   weight[3]  = lFactors.mFWstdev;
#else
   stature = Doc_Ptr->getAnthropometryRef().GetHeightPercentiles();
   weight = Doc_Ptr->getAnthropometryRef().GetWeightPercentiles();
   stature[0] = lFactors.mMH95;
   stature[1] = lFactors.mMH50;
   stature[2] = lFactors.mMH5;
   stature[3] = lFactors.mFH95;
   stature[4] = lFactors.mFH50;
   stature[5] = lFactors.mFH5;

   weight[0] = lFactors.mMW95;
   weight[1] = lFactors.mMW50;
   weight[2] = lFactors.mMW5;
   weight[3] = lFactors.mFW95;
   weight[4] = lFactors.mFW50;
   weight[5] = lFactors.mFW5;
#endif

   this->Analyze();
   return true;
}

//---------------------------------------------------------------------------

BOOL C_Hom_Doc::GetFatigueReport(VARIANT_BOOL left, SHORT population_strength_percentile, SHORT* ElbowFlex, SHORT* ShoulderHumRot, SHORT* ShoulderBackFwd, SHORT* ShoulderAbdAdd, SHORT* TorsoFlex, SHORT* TorsoLatBend, SHORT* TorsoRot, SHORT* HipFlex, SHORT* KneeFlex, SHORT* AnkleFlex)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return VARIANT_FALSE;
	}

	if(population_strength_percentile != 5 &&
		population_strength_percentile != 25 &&
		population_strength_percentile != 50) {
		return VARIANT_FALSE;
	}

	this->Analyze();
	// set up the joint indices
	int indices[10];
	StrengthHinge_e hinges[10];
	if(left) {
		indices[0] = 5;
		indices[1] = 6;
		indices[2] = 7;
		indices[3] = 8;
		indices[4] = 9;
		indices[5] = 10;
		indices[6] = 11;
		indices[7] = 15;
		indices[8] = 16;
		indices[9] = 17;
		hinges[0] = SH_LElbowFlexion;
		hinges[1] = SH_LHumeralRotation;
		hinges[2] = SH_LShoulderRotation;
		hinges[3] = SH_LShoulderAbduction;
		hinges[4] = SH_TorsoFlexion;
		hinges[5] = SH_TorsoLateralBending;
		hinges[6] = SH_TorsoRotation;
		hinges[7] = SH_LHipFlexion;
		hinges[8] = SH_LKneeFlexion;
		hinges[9] = SH_LAnkleFlexion;
	} else {
		indices[0] = 1;
		indices[1] = 2;
		indices[2] = 3;
		indices[3] = 4;
		indices[4] = 9;
		indices[5] = 10;
		indices[6] = 11;
		indices[7] = 12;
		indices[8] = 13;
		indices[9] = 14;
		hinges[0] = SH_RElbowFlexion;
		hinges[1] = SH_RHumeralRotation;
		hinges[2] = SH_RShoulderRotation;
		hinges[3] = SH_RShoulderAbduction;
		hinges[4] = SH_TorsoFlexion;
		hinges[5] = SH_TorsoLateralBending;
		hinges[6] = SH_TorsoRotation;
		hinges[7] = SH_RHipFlexion;
		hinges[8] = SH_RKneeFlexion;
		hinges[9] = SH_RAnkleFlexion;
	}

	short* pointers[10] = {ElbowFlex, ShoulderHumRot, ShoulderBackFwd, ShoulderAbdAdd, TorsoFlex, TorsoLatBend, TorsoRot, HipFlex, KneeFlex, AnkleFlex};
	
	// set up standard deviation scale
	double scale;
	if(population_strength_percentile == 5) {
		scale = 1.645;
	}
	if(population_strength_percentile == 25) {
		scale = 0.645;
	}
	if(population_strength_percentile == 50) {
		scale = 0;
	}

	//int sex = mResults.sex;
	double MtoCM = 100.0;
	double FracToPerc = 100.0;
	StrengthData_s data = this->GetSkeleton()->getStrengthData(true);
	for(int i = 0; i < 10; i++) {

		double moment = data.Moment[hinges[i]];
		*pointers[i] = FracToPerc * MtoCM * fabs(moment) / 
			(
				MtoCM * fabs(data.Mean[hinges[i]]) -
				scale * MtoCM * data.SD[hinges[i]]
			);

		/*double moment = mResults.mhinge[sex][indices[i]];
		*pointers[i] = FracToPerc * MtoCM * fabs(moment) /
			(
				MtoCM * fabs(mResults.vtorq[sex][indices[i]]) - 
				scale * MtoCM * mResults.stddev[sex][indices[i]]
			);*/

	}
	/* From the report code:*/
	/*
	a5 = String_Cast( Round_Double( lC * fabs(lMomentRaw) / 
	(lC * fabs(aResult.vtorq[lSex][aIndex]) - 1.645 * lC * aResult.stddev[lSex][aIndex])*100.0, 0 ), 3 );
	a25 = String_Cast( Round_Double( lC * fabs(lMomentRaw) /
	(lC * fabs(aResult.vtorq[lSex][aIndex]) - 0.6745 * lC * aResult.stddev[lSex][aIndex])*100.0, 0 ), 3 );
	a50 = String_Cast( Round_Double( lC * fabs(lMomentRaw) /
	(lC * fabs(aResult.vtorq[lSex][aIndex])                                             )*100.0, 0 ), 3 );
	*/

	return VARIANT_TRUE;
}

BOOL C_Hom_Doc::IsRegistered(void)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	// TODO: Add your dispatch handler code here
	//return(!Get_App_Ptr()->Is_Demonstration());

	// TODO SiteData no longer exists
	// we have to check for software passport data
	//return Get_App_Ptr()->SiteData();
	

	return VARIANT_TRUE;
}

BOOL C_Hom_Doc::SetAngle(long angle, double value) {
	
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	
	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

	this->GetSkeleton()->SetAngle((E_Angles)angle, value);
	this->Analyze();
	bool ret = this->GetSkeleton()->ValidateJointAngles();
	if(!ret) {
		this->mCOMLastErrorMessage = this->GetSkeleton()->getLastError();
	}
	return ret;
}

BOOL C_Hom_Doc::GetAngle(long angle, double *value) {
	
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}
	*value = this->GetSkeleton()->GetAngle((E_Angles)angle);
	return true;
}

BOOL C_Hom_Doc::SetBodySegmentAnglesWithLegs(short aForearmLH, short aForearmLV, short aForearmRH, short aForearmRV, short aUpperarmLH, short aUpperarmLV, short aUpperarmRH, short aUpperarmRV, short aUpperlegLV, short aUpperlegLH, short aUpperlegRV, short aUpperlegRH, short aLowerlegLV, short aLowerlegLH, short aLowerlegRV, short aLowerlegRH, short aTrunkFlexion, short aTrunkAxialRotation, short aTrunkLateralBending) {

	AFX_MANAGE_STATE(AfxGetAppModuleState());

	// disallow use of COM if 3DSSPP isn't registered
	if(!IsRegistered()) {
		mCOMLastErrorMessage = "Unregistered 3DSSPP";
		return false;
	}

   //Attempt to assign values to a temporary joint angles object

   JointAngles   lJointAngles;

   // NOTE
   // JointAngles::Assign takes more angles now
   // more importantly, the leg angles definitions changed
   lJointAngles.Assign
         ( 
            aForearmLH, 
            aForearmLV, 
            aForearmRH, 
            aForearmRV, 
   
            aUpperarmLH, 
            aUpperarmLV, 
            aUpperarmRH, 
            aUpperarmRV, 
   
			aUpperlegLH, // upperleg lh
            aUpperlegLV,
			aUpperlegRH, // upperleg rh
            aUpperlegRV, 
   
			aLowerlegLH, // lowerleg lh
            aLowerlegLV,
			aLowerlegRH, // lowerleg rh
            aLowerlegRV, 
   
            aTrunkFlexion, 
            aTrunkAxialRotation, 
            aTrunkLateralBending,

			0,  //double aSCJH,	// Todo - when SCJ/clavivles added to 3dsspp,activate
			0,  //double aSCJV,

			lJointAngles[A_ClavicleLH],//0,  //double aClavicleLH,
			lJointAngles[A_ClavicleLV],//0,  //double aClavicleLV,
			lJointAngles[A_ClavicleRH],//0,  //double aClavicleRH,
			lJointAngles[A_ClavicleRV],//0,   //double aClavicleRV

			90, // footlh
			0, // footlv
			90, // footrh
			0, // footrv

			aForearmLH, // handlh
			aForearmLV, // handlv
			0, // handlrot
			aForearmRH, // handrh
			aForearmRV, // handrv
			0,

			aTrunkLateralBending,
			aTrunkFlexion,
			0,
			aTrunkLateralBending,
			aTrunkFlexion,
			0,
			0
		);
	if(lJointAngles.Validate()  )
	{
		this->GetSkeleton()->SetAngles(lJointAngles);

		this->Analyze();

		return   TRUE;
	}

	else //assign failed
	{
		mCOMLastErrorMessage = lJointAngles.getLastError();
		return   FALSE;
	}
}