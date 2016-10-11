// ReportPosture.cpp : implementation file
//

#include "ReportPosture.h"
#include "Skeleton.hpp"
#include "hom.h"
#include "Analysis.h"
#include "hom_doc.h" // Necessary

#include "Skeleton.hpp"
#include "Factors.hpp"

#include "GLView.h"
#include "GLBalanceStanding.h"
//#include "GLOblique.h" // TODO for debugging, take out

#include "Main_Frm.h"

#include "wingdi.h"
#include "Analysis.h"
#include "../c4eLibNew/units.hpp"
#include "Anthropometry.hpp"

#include "CDIB.h"
#include "DIB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--- Globals defined and computed in FindPoi.cpp --- 

//extern double gPelvicAngle;

/////////////////////////////////////////////////////////////////////////////
// ReportPosture

ReportView * ReportPosture::cPointer = NULL;

IMPLEMENT_DYNCREATE(ReportPosture, ReportView)

ReportPosture::ReportPosture()
: ReportView( ReportPosture::IDD )
, mLeftHipHorizontal(_T(""))
, mRightHipHorizontal(_T(""))
, mLeftFemoralRot(_T(""))
, mRightFemoralRot(_T(""))
, mLeftHipIncluded(_T(""))
, mRightHipIncluded(_T(""))
, mLeftForearmRot(_T(""))
, mRightForearmRot(_T(""))
, mLeftLowerlegRot(_T(""))
, mRightLowerlegRot(_T(""))
, mPelvisLateralBending(_T(""))
, mLeftHandUlnarDeviation(_T(""))
, mRightHandUlnarDeviation(_T(""))
, mLeftHandRadialDeviation(_T(""))
, mRightHandRadialDeviation(_T(""))
, mL5S1ForwardTilt(_T(""))
, mAdjustedTrunkFlexionPelv(_T(""))
, mAdjustedTrunkRotationPelv(_T(""))
, mAdjustedTrunkBendingPelv(_T(""))
, mHeadFlexion(_T(""))
, mHeadAxial(_T(""))
, mHeadLateral(_T(""))
, mHeadFlexion2(_T(""))
, mHeadAxial2(_T(""))
, mHeadLateral2(_T(""))
// Add:
, mNeck1Flexion(_T(""))
, mNeck1Rotation(_T(""))
, mNeck1Bending(_T(""))
, mNeck2Flexion(_T(""))
, mNeck2Rotation(_T(""))
, mNeck2Bending(_T(""))
, mLeftHandFlexionCtrl(ReportPosture::IDD)
, mRightHandFlexionCtrl(ReportPosture::IDD)
, mLeftHandExtensionCtrl(ReportPosture::IDD)
, mRightHandExtensionCtrl(ReportPosture::IDD)
, mLeftHandUlnarDeviationCtrl(ReportPosture::IDD)
, mRightHandUlnarDeviationCtrl(ReportPosture::IDD)
, mLeftHandRadialDeviationCtrl(ReportPosture::IDD)
, mRightHandRadialDeviationCtrl(ReportPosture::IDD)
, mLeftForearmRotCtrl(ReportPosture::IDD)
, mRightForearmRotCtrl(ReportPosture::IDD)
, mLeftElbowIncludedCtrl(ReportPosture::IDD)
, mRightElbowIncludedCtrl(ReportPosture::IDD)
, mLeftShoulderVertCtrl(ReportPosture::IDD)
, mLeftShoulderHorzCtrl(ReportPosture::IDD)
, mRightShoulderVertCtrl(ReportPosture::IDD)
, mRightShoulderHorzCtrl(ReportPosture::IDD)
, mLeftHumeralRotCtrl(ReportPosture::IDD)
, mRightHumeralRotCtrl(ReportPosture::IDD)
, mLeftHipIncludedCtrl(ReportPosture::IDD)
, mLeftHipVerticalCtrl(ReportPosture::IDD)
, mLeftHipHorizontalCtrl(ReportPosture::IDD)
, mRightHipIncludedCtrl(ReportPosture::IDD)
, mRightHipVerticalCtrl(ReportPosture::IDD)
, mRightHipHorizontalCtrl(ReportPosture::IDD)
, mLeftFemoralRotCtrl(ReportPosture::IDD)
, mRightFemoralRotCtrl(ReportPosture::IDD)
, mLeftLowerlegRotCtrl(ReportPosture::IDD)
, mRightLowerlegRotCtrl(ReportPosture::IDD)
, mLeftKneeIncludedCtrl(ReportPosture::IDD)
, mLeftAnkleIncludedCtrl(ReportPosture::IDD)
, mRightKneeIncludedCtrl(ReportPosture::IDD)
, mRightAnkleIncludedCtrl(ReportPosture::IDD)

, mHeadFlexionCtrl2(ReportPosture::IDD)
, mHeadAxialCtrl2(ReportPosture::IDD)
, mHeadLateralCtrl2(ReportPosture::IDD)
, mHeadFlexionCtrl(ReportPosture::IDD)
, mAdjustedNeck1FlexionCtrl(ReportPosture::IDD)
, mAdjustedNeck1RotationCtrl(ReportPosture::IDD)
, mAdjustedNeck1BendingCtrl(ReportPosture::IDD)
, mAdjustedNeck2FlexionCtrl(ReportPosture::IDD)
, mAdjustedNeck2RotationCtrl(ReportPosture::IDD)
, mAdjustedNeck2BendingCtrl(ReportPosture::IDD)
, mAdjustedTrunkFlexionCtrl(ReportPosture::IDD)
, mAdjustedTrunkRotationCtrl(ReportPosture::IDD)
, mAdjustedTrunkBendingCtrl(ReportPosture::IDD)
, mPelvicRotationCtrl(ReportPosture::IDD)
, mPelvisLateralBendingCtrl(ReportPosture::IDD)
, mL5S1ForwardTiltCtrl(ReportPosture::IDD)

{
	//{{AFX_DATA_INIT(ReportPosture)
	mAdjustedTrunkBending = _T("");
	mAdjustedTrunkFlexion = _T("");
	mAdjustedTrunkRotation = _T("");
	mFooter = _T("");
	mHeader = _T("");
	mLeftAnkleIncluded = _T("");
	mLeftElbowIncluded = _T("");
	mLeftHipVertical = _T("");
	mLeftHumeralRot = _T("");
	mLeftKneeIncluded = _T("");
	mLeftShoulderHorz = _T("");
	mLeftShoulderVert = _T("");
	mPelvicRotation = _T("");
	mRightAnkleIncluded = _T("");
	mRightElbowIncluded = _T("");
	mRightHipVertical = _T("");
	mRightHumeralRot = _T("");
	mRightKneeIncluded = _T("");
	mRightShoulderHorz = _T("");
	mRightShoulderVert = _T("");
    mL5S1ForwardTilt = _T("");

	// TODO
	// debugging oblique view
	mLeftAnkleBending = _T("");
	mRightAnkleBending = _T("");

	mHandForceUnits = _T("");
	mUnitsLabel = _T("");
	mLeftHori = _T("");



	//}}AFX_DATA_INIT

   cPointer = this;
   return;
}

ReportPosture::~ReportPosture()
{
   cPointer = NULL;
   return;
}

//---------------------------------------------------------------------------
   CString
   ReportPosture::
ReportName
( void ) 
const
{
   return   "3DSSPP - Posture (Local Angles) Report";
}

void ReportPosture::DoDataExchange(CDataExchange* pDX)
{
	ReportView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ReportPosture)
	DDX_Text(pDX, IDC_AdjustedTrunkBending, mAdjustedTrunkBending);
	DDX_Text(pDX, IDC_AdjustedTrunkFlexion, mAdjustedTrunkFlexion);
	DDX_Text(pDX, IDC_AdjustedTrunkRotation, mAdjustedTrunkRotation);
	DDX_Text(pDX, IDC_Footer, mFooter);
	DDX_Text(pDX, IDC_Header, mHeader);
	DDX_Text(pDX, IDC_LeftAnkleIncluded, mLeftAnkleIncluded);
	DDX_Text(pDX, IDC_LeftElbowIncluded, mLeftElbowIncluded);
	DDX_Text(pDX, IDC_LeftHipVertical, mLeftHipVertical);
	DDX_Text(pDX, IDC_LeftHumeralRot, mLeftHumeralRot);
	DDX_Text(pDX, IDC_LeftKneeIncluded, mLeftKneeIncluded);
	DDX_Text(pDX, IDC_LeftShoulderHorz, mLeftShoulderHorz);
	DDX_Text(pDX, IDC_LeftShoulderVert, mLeftShoulderVert);
	DDX_Text(pDX, IDC_PelvicRotation, mPelvicRotation);
	DDX_Text(pDX, IDC_RightAnkleIncluded, mRightAnkleIncluded);
	DDX_Text(pDX, IDC_RightElbowIncluded2, mRightElbowIncluded);
	DDX_Text(pDX, IDC_RightHipVertical, mRightHipVertical);
	DDX_Text(pDX, IDC_RightHumeralRot, mRightHumeralRot);
	DDX_Text(pDX, IDC_RightKneeIncluded, mRightKneeIncluded);
	DDX_Text(pDX, IDC_RightShoulderHorz, mRightShoulderHorz);
	DDX_Text(pDX, IDC_RightShoulderVert, mRightShoulderVert);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_LeftHipHorizontal, mLeftHipHorizontal);
	DDX_Text(pDX, IDC_RightHipHorizontal, mRightHipHorizontal);
	DDX_Text(pDX, IDC_LeftFemoralRot, mLeftFemoralRot);
	DDX_Text(pDX, IDC_RightFemoralRot, mRightFemoralRot);
	DDX_Text(pDX, IDC_LeftHipIncluded, mLeftHipIncluded);
	DDX_Text(pDX, IDC_RightHipIncluded, mRightHipIncluded);
	DDX_Text(pDX, IDC_LeftForearmRot, mLeftForearmRot);
	DDX_Text(pDX, IDC_RightForearmRot, mRightForearmRot);
	DDX_Text(pDX, IDC_LeftHandFlexion, mLeftHandFlexion);
	DDX_Text(pDX, IDC_RightHandFlexion, mRightHandFlexion);
	DDX_Text(pDX, IDC_LeftLowerlegRot, mLeftLowerlegRot);
	DDX_Text(pDX, IDC_RightLowerlegRot, mRightLowerlegRot);
	DDX_Text(pDX, IDC_PelvisLateralBending, mPelvisLateralBending);
	DDX_Text(pDX, IDC_LeftHandExtension, mLeftHandExtension);
	DDX_Text(pDX, IDC_RightHandExtension, mRightHandExtension);
	DDX_Text(pDX, IDC_LeftHandUlnarDeviation, mLeftHandUlnarDeviation);
	DDX_Text(pDX, IDC_RightHandUlnarDeviation, mRightHandUlnarDeviation);
	DDX_Text(pDX, IDC_LeftHandRadialDeviation, mLeftHandRadialDeviation);
	DDX_Text(pDX, IDC_RightHandRadialDeviation, mRightHandRadialDeviation);
	DDX_Text(pDX, IDC_L5S1ForwardTilt, mL5S1ForwardTilt);
	// DDX_Text(pDX, IDC_STANCE, mPelvisLateralBending);
	DDX_Text(pDX, IDC_AdjustedTrunkFlexionPelv, mAdjustedTrunkFlexionPelv);
	DDX_Text(pDX, IDC_AdjustedTrunkRotationPelv, mAdjustedTrunkRotationPelv);
	DDX_Text(pDX, IDC_AdjustedTrunkBendingPelv, mAdjustedTrunkBendingPelv);
	//For nasion flexion
	DDX_Text(pDX, IDC_HEADFLEX, mHeadFlexion);
	DDX_Text(pDX, IDC_HEADAXIAL, mHeadAxial);
	//DDX_Text(pDX, IDC_HEADLAT, mHeadLateral);

	//for head/torso angle
	DDX_Text(pDX, IDC_HEADFLEX2, mHeadFlexion2);
	DDX_Text(pDX, IDC_HEADAXIAL2, mHeadAxial2);
	DDX_Text(pDX, IDC_HEADLAT2, mHeadLateral2);
	DDX_Text(pDX, IDC_MinHeadFlexion2, mMinHeadFlexion2);
	DDX_Text(pDX, IDC_MaxHeadFlexion2, mMaxHeadFlexion2);
	DDX_Text(pDX, IDC_MinHeadRotation2, mMinHeadAxial2);
	DDX_Text(pDX, IDC_MaxHeadRotation2, mMaxHeadAxial2);
	DDX_Text(pDX, IDC_MinHeadBending2, mMinHeadLateral2);
	DDX_Text(pDX, IDC_MaxHeadBending2, mMaxHeadLateral2);


	DDX_Text(pDX, IDC_AdjustedNeck1Flexion, mNeck1Flexion);
	DDX_Text(pDX, IDC_AdjustedNeck1Rotation, mNeck1Rotation);
	DDX_Text(pDX, IDC_AdjustedNeck1Bending, mNeck1Bending);
	DDX_Text(pDX, IDC_AdjustedNeck2Flexion, mNeck2Flexion);
	DDX_Text(pDX, IDC_AdjustedNeck2Rotation, mNeck2Rotation);
	DDX_Text(pDX, IDC_AdjustedNeck2Bending, mNeck2Bending);


	DDX_Text(pDX, IDC_MinHandFlexion, mMinHandFlexion);
	DDX_Text(pDX, IDC_MaxHandFlexion, mMaxHandFlexion);
	DDX_Text(pDX, IDC_MinHandExtension, mMinHandExtension);
	DDX_Text(pDX, IDC_MaxHandExtension, mMaxHandExtension);
	DDX_Text(pDX, IDC_MinHandUlnarDeviation, mMinHandUlnarDeviation);
	DDX_Text(pDX, IDC_MaxHandUlnarDeviation, mMaxHandUlnarDeviation);
	DDX_Text(pDX, IDC_MinHandRadialDeviation, mMinHandRadialDeviation);
	DDX_Text(pDX, IDC_MaxHandRadialDeviation, mMaxHandRadialDeviation);
	DDX_Text(pDX, IDC_MinForearmRot, mMinForearmRot);
	DDX_Text(pDX, IDC_MaxForearmRot, mMaxForearmRot);
	DDX_Text(pDX, IDC_MinElbowIncluded, mMinElbowIncluded);
	DDX_Text(pDX, IDC_MaxElbowIncluded, mMaxElbowIncluded);
	DDX_Text(pDX, IDC_MinShoulderVert, mMinShoulderVert);
	DDX_Text(pDX, IDC_MaxShoulderVert, mMaxShoulderVert);
	DDX_Text(pDX, IDC_MinShoulderHorz, mMinShoulderHorz);
	DDX_Text(pDX, IDC_MaxShoulderHorz, mMaxShoulderHorz);
	DDX_Text(pDX, IDC_MinHumeralRot, mMinHumeralRot);
	DDX_Text(pDX, IDC_MaxHumeralRot, mMaxHumeralRot);
	DDX_Text(pDX, IDC_MinHipIncluded, mMinHipIncluded);
	DDX_Text(pDX, IDC_MaxHipIncluded, mMaxHipIncluded);
	DDX_Text(pDX, IDC_MinHipVertical, mMinHipVertical);
	DDX_Text(pDX, IDC_MaxHipVertical, mMaxHipVertical);
	DDX_Text(pDX, IDC_MinHipHorizontal, mMinHipHorizontal);
	DDX_Text(pDX, IDC_MaxHipHorizontal, mMaxHipHorizontal);
	DDX_Text(pDX, IDC_MinFemoralRot, mMinFemoralRot);
	DDX_Text(pDX, IDC_MaxFemoralRot, mMaxFemoralRot);
	DDX_Text(pDX, IDC_MinLowerlegRot, mMinLowerlegRot);
	DDX_Text(pDX, IDC_MaxLowerlegRot, mMaxLowerlegRot);
	DDX_Text(pDX, IDC_MinKneeIncluded, mMinKneeIncluded);
	DDX_Text(pDX, IDC_MaxKneeIncluded, mMaxKneeIncluded);
	DDX_Text(pDX, IDC_MinAnkleIncluded, mMinAnkleIncluded);
	DDX_Text(pDX, IDC_MaxAnkleIncluded, mMaxAnkleIncluded);

	DDX_Text(pDX, IDC_MinHeadFlexion, mMinHeadFlexion);
	DDX_Text(pDX, IDC_MaxHeadFlexion, mMaxHeadFlexion);
	DDX_Text(pDX, IDC_MinNeck1Flexion, mMinNeck1Flexion);
	DDX_Text(pDX, IDC_MaxNeck1Flexion, mMaxNeck1Flexion);
	DDX_Text(pDX, IDC_MinNeck1Rotation, mMinNeck1Axial);
	DDX_Text(pDX, IDC_MaxNeck1Rotation, mMaxNeck1Axial);
	DDX_Text(pDX, IDC_MinNeck1Bending, mMinNeck1Lateral);
	DDX_Text(pDX, IDC_MaxNeck1Bending, mMaxNeck1Lateral);
	DDX_Text(pDX, IDC_MinNeck2Flexion, mMinNeck2Flexion);
	DDX_Text(pDX, IDC_MaxNeck2Flexion, mMaxNeck2Flexion);
	DDX_Text(pDX, IDC_MinNeck2Rotation, mMinNeck2Axial);
	DDX_Text(pDX, IDC_MaxNeck2Rotation, mMaxNeck2Axial);
	DDX_Text(pDX, IDC_MinNeck2Bending, mMinNeck2Lateral);
	DDX_Text(pDX, IDC_MaxNeck2Bending, mMaxNeck2Lateral);
	DDX_Text(pDX, IDC_MinTorsoFlexion, mMinAdjustedTrunkFlexion);
	DDX_Text(pDX, IDC_MaxTorsoFlexion, mMaxAdjustedTrunkFlexion);
	DDX_Text(pDX, IDC_MinTorsoRotation, mMinAdjustedTrunkRotation);
	DDX_Text(pDX, IDC_MaxTorsoRotation, mMaxAdjustedTrunkRotation);
	DDX_Text(pDX, IDC_MinTorsoBending,  mMinAdjustedTrunkBending);
	DDX_Text(pDX, IDC_MaxTorsoBending, mMaxAdjustedTrunkBending);
	DDX_Text(pDX, IDC_MinPelvisRotation, mMinPelvicRotation);
	DDX_Text(pDX, IDC_MaxPelvisRotation, mMaxPelvicRotation);
	DDX_Text(pDX, IDC_MinPelvisLateralBending, mMinPelvisLateralBending);
	DDX_Text(pDX, IDC_MaxPelvisLateralBending, mMaxPelvisLateralBending);

	// TODO
	// debugging oblique view
	DDX_Text(pDX, IDC_LANKLEBEND, mLeftAnkleBending);
	DDX_Text(pDX, IDC_RANKLEBEND, mRightAnkleBending);
	DDX_Control(pDX, IDC_LeftHandFlexion, mLeftHandFlexionCtrl);
	DDX_Control(pDX, IDC_RightHandFlexion, mRightHandFlexionCtrl);
	DDX_Control(pDX, IDC_LeftHandExtension, mLeftHandExtensionCtrl);
	DDX_Control(pDX, IDC_RightHandExtension, mRightHandExtensionCtrl);
	DDX_Control(pDX, IDC_LeftHandUlnarDeviation, mLeftHandUlnarDeviationCtrl);
	DDX_Control(pDX, IDC_RightHandUlnarDeviation, mRightHandUlnarDeviationCtrl);
	DDX_Control(pDX, IDC_LeftHandRadialDeviation, mLeftHandRadialDeviationCtrl);
	DDX_Control(pDX, IDC_RightHandRadialDeviation, mRightHandRadialDeviationCtrl);
	DDX_Control(pDX, IDC_LeftForearmRot, mLeftForearmRotCtrl);
	DDX_Control(pDX, IDC_RightForearmRot, mRightForearmRotCtrl);
	DDX_Control(pDX, IDC_LeftElbowIncluded, mLeftElbowIncludedCtrl);
	DDX_Control(pDX, IDC_RightElbowIncluded2, mRightElbowIncludedCtrl);
	DDX_Control(pDX, IDC_LeftShoulderVert, mLeftShoulderVertCtrl);
	DDX_Control(pDX, IDC_LeftShoulderHorz, mLeftShoulderHorzCtrl);
	DDX_Control(pDX, IDC_RightShoulderVert, mRightShoulderVertCtrl);
	DDX_Control(pDX, IDC_RightShoulderHorz, mRightShoulderHorzCtrl);
	DDX_Control(pDX, IDC_LeftHumeralRot, mLeftHumeralRotCtrl);
	DDX_Control(pDX, IDC_RightHumeralRot, mRightHumeralRotCtrl);
	DDX_Control(pDX, IDC_LeftHipIncluded, mLeftHipIncludedCtrl);
	DDX_Control(pDX, IDC_LeftHipVertical, mLeftHipVerticalCtrl);
	DDX_Control(pDX, IDC_LeftHipHorizontal, mLeftHipHorizontalCtrl);
	DDX_Control(pDX, IDC_RightHipIncluded, mRightHipIncludedCtrl);
	DDX_Control(pDX, IDC_RightHipVertical, mRightHipVerticalCtrl);
	DDX_Control(pDX, IDC_RightHipHorizontal, mRightHipHorizontalCtrl);
	DDX_Control(pDX, IDC_LeftFemoralRot, mLeftFemoralRotCtrl);
	DDX_Control(pDX, IDC_RightFemoralRot, mRightFemoralRotCtrl);
	DDX_Control(pDX, IDC_LeftLowerlegRot, mLeftLowerlegRotCtrl);
	DDX_Control(pDX, IDC_RightLowerlegRot, mRightLowerlegRotCtrl);
	DDX_Control(pDX, IDC_LeftKneeIncluded, mLeftKneeIncludedCtrl);
	DDX_Control(pDX, IDC_LeftAnkleIncluded, mLeftAnkleIncludedCtrl);
	DDX_Control(pDX, IDC_RightKneeIncluded, mRightKneeIncludedCtrl);
	DDX_Control(pDX, IDC_RightAnkleIncluded, mRightAnkleIncludedCtrl);

	DDX_Control(pDX, IDC_HEADFLEX2, mHeadFlexionCtrl2);
	DDX_Control(pDX, IDC_HEADAXIAL2, mHeadAxialCtrl2);
	DDX_Control(pDX, IDC_HEADLAT2, mHeadLateralCtrl2);

	DDX_Control(pDX, IDC_HEADFLEX, mHeadFlexionCtrl);

	DDX_Control(pDX, IDC_AdjustedNeck1Flexion, mAdjustedNeck1FlexionCtrl);
	DDX_Control(pDX, IDC_AdjustedNeck1Rotation, mAdjustedNeck1RotationCtrl);
	DDX_Control(pDX, IDC_AdjustedNeck1Bending, mAdjustedNeck1BendingCtrl);

	DDX_Control(pDX, IDC_AdjustedNeck2Flexion, mAdjustedNeck2FlexionCtrl);
	DDX_Control(pDX, IDC_AdjustedNeck2Rotation, mAdjustedNeck2RotationCtrl);
	DDX_Control(pDX, IDC_AdjustedNeck2Bending, mAdjustedNeck2BendingCtrl);


	DDX_Control(pDX, IDC_AdjustedTrunkFlexion, mAdjustedTrunkFlexionCtrl);
	DDX_Control(pDX, IDC_AdjustedTrunkRotation, mAdjustedTrunkRotationCtrl);
	DDX_Control(pDX, IDC_AdjustedTrunkBending, mAdjustedTrunkBendingCtrl);
	DDX_Control(pDX, IDC_PelvicRotation, mPelvicRotationCtrl);
	DDX_Control(pDX, IDC_PelvisLateralBending, mPelvisLateralBendingCtrl);
	DDX_Control(pDX, IDC_L5S1ForwardTilt, mL5S1ForwardTiltCtrl);

	DDX_Text(pDX, IDC_HandForceUnits, mHandForceUnits);
	DDX_Text(pDX, IDC_HAND_FORCE_TXT_LVAL, mHandForceTxtLVal);
	DDX_Text(pDX, IDC_HAND_FORCE_TXT_RVAL, mHandForceTxtRVal);
	DDX_Text(pDX, IDC_UnitsLabel, mUnitsLabel);
	DDX_Text(pDX, IDC_RightHori, mRightHori);
	DDX_Text(pDX, IDC_RightLat2, mRightLat);
	DDX_Text(pDX, IDC_RightVert2, mRightVert);
	DDX_Text(pDX, IDC_LeftHori, mLeftHori);
	DDX_Text(pDX, IDC_LeftLat2, mLeftLat);
	DDX_Text(pDX, IDC_LeftVert2, mLeftVert);
}


BEGIN_MESSAGE_MAP(ReportPosture, ReportView)
	//{{AFX_MSG_MAP(ReportPosture)
	//ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
	//}}AFX_MSG_MAP
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ReportPosture diagnostics

#ifdef _DEBUG
void ReportPosture::AssertValid() const
{
	CFormView::AssertValid();
}

void ReportPosture::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ReportPosture overrides

//void ReportPosture::OnFilePrintPreview()
//{
//	SSPPPrintPreview(this);
//}

void ReportPosture::OnUpdate(CView *aSenderPtr,
                             LPARAM aHint,
									  CObject * aHintPtr)
{
   C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
   //Analysis &lResult = lDocPtr->getResultsRef();
   
   //--- Additional hip angles
   //JointAngles docJointAngles;
   Skeleton& skel = *lDocPtr->GetSkeleton();
   /*unsigned int frame = lDocPtr->GetSkeleton().getCurrentFrame();
   lDocPtr->GetSkeleton().GetAngles(frame,docJointAngles);*/
   

	//--- Assign dialog members ---
   
	mLeftElbowIncluded		= IntToCStr(Round_Double(skel.getExtraAngle(EA_ElbowLInc)));//lResult.LeftElbowIncluded()));
	mLeftShoulderVert       = IntToCStr(Round_Double(skel.getExtraAngle(EA_ShoulderLV)));//lResult.LeftShoulderVertical()));
	mLeftShoulderHorz       = IntToCStr(Round_Double(skel.getExtraAngle(EA_ShoulderLH)));//lResult.LeftShoulderHorizontal()));
	mLeftHumeralRot         = IntToCStr(Round_Double(skel.getExtraAngle(EA_HumerusLRot)));//lResult.LeftHumeralRotation()));
	mLeftForearmRot			= IntToCStr(Round_Double(skel.getExtraAngle(EA_ForearmLRot)));

	// 09/17/07 Sayan 
	// We comment this out as we will be showing Flexion and Extension separately.
	// mLeftHandFlexion		= IntToCStr(int(skel.getExtraAngle(EA_HandLFlexion)));

	// Flexion is always negative and Extension is always positive?
	if ((int(skel.getExtraAngle(EA_HandLFlexion))) >=0) {
		mLeftHandExtension = IntToCStr(Round_Double(skel.getExtraAngle(EA_HandLFlexion)));
		mLeftHandFlexion = IntToCStr(0);
	} else {
        mLeftHandFlexion = IntToCStr(Round_Double(skel.getExtraAngle(EA_HandLFlexion)));
		mLeftHandExtension =  IntToCStr(0); 
	}

	// mLeftHandFlexion		    = IntToCStr(int(skel.getExtraAngle(EA_HandLFlexion)));
	// mLeftHandDeviation		= IntToCStr(int(skel.getExtraAngle(EA_HandLDeviation)));

	// 09/17/07 Sayan 
	// We comment this out as we will be showing ulnar deviation and radial deviation separately.
	// mRightHandDeviation		= IntToCStr(int(skel.getExtraAngle(EA_HandRDeviation)));
   
	// Radial deviation is always negative and ulnar deviation is always positive?

	if ((int(skel.getExtraAngle(EA_HandLDeviation))) >=0) {
		mLeftHandUlnarDeviation = IntToCStr(Round_Double(skel.getExtraAngle(EA_HandLDeviation)));
		mLeftHandRadialDeviation = IntToCStr(0);
	} else {
        mLeftHandRadialDeviation = IntToCStr(Round_Double(skel.getExtraAngle(EA_HandLDeviation)));
		mLeftHandUlnarDeviation = IntToCStr(0);
	}


	mLeftHipIncluded		= IntToCStr(Round_Double(skel.getExtraAngle(EA_HipLInc)));
	mLeftHipVertical        = IntToCStr(Round_Double(skel.getExtraAngle(EA_HipLV)));//lLeftHipIncluded);
	mLeftHipHorizontal		= IntToCStr(Round_Double(skel.getExtraAngle(EA_HipLH)));
	mLeftKneeIncluded       = IntToCStr(Round_Double(skel.getExtraAngle(EA_KneeLInc)));//lLeftKneeIncluded);
	mLeftAnkleIncluded      = IntToCStr(Round_Double(skel.getExtraAngle(EA_AnkleLInc)));//lResult.LeftAnkleIncluded()));
	mLeftFemoralRot			= IntToCStr(Round_Double(skel.getExtraAngle(EA_FemurLRot)));
	mLeftLowerlegRot		= IntToCStr(Round_Double(skel.getExtraAngle(EA_LowerlegLRot)));

	mHeadFlexion			= IntToCStr(90 - (Round_Double(skel.getExtraAngle(EA_C1_Flexion))+16));
	/*mHeadAxial				= IntToCStr(Round_Double(skel.GetAngle(A_C4C7AxialRotation)));
	mHeadLateral			= IntToCStr(Round_Double(skel.GetAngle(A_C4C7LateralBending)));*/


	mHeadAxial				= IntToCStr(Round_Double(skel.getExtraAngle(EA_C4C7AxialRotation)) + Round_Double(skel.getExtraAngle(EA_C1C3AxialRotation)));

	
	mHeadLateral			= IntToCStr(Round_Double(skel.getExtraAngle(EA_C4C7LateralBending)));

	// Add:
	//head/torso
	mHeadFlexion2			= IntToCStr(90 - (Round_Double(16 + skel.getExtraAngle(EA_C1C3Flexion)+skel.getExtraAngle(EA_C4C7Flexion)+skel.getExtraAngle(EA_C1_Flexion)-180)));
	mHeadAxial2			= IntToCStr(Round_Double(skel.getExtraAngle(EA_C1C3AxialRotation)+skel.getExtraAngle(EA_C4C7AxialRotation)));
	mHeadLateral2			= IntToCStr(Round_Double(skel.getExtraAngle(EA_C1C3LateralBending)+skel.getExtraAngle(EA_C4C7LateralBending)));
	//Upper neck
	mNeck1Flexion			= IntToCStr(90 - (Round_Double(skel.getExtraAngle(EA_C1C3Flexion))));
	mNeck1Rotation			= IntToCStr(Round_Double(skel.getExtraAngle(EA_C1C3AxialRotation)));
	mNeck1Bending			= IntToCStr(Round_Double(skel.getExtraAngle(EA_C1C3LateralBending)));
	//Lower neck

	mNeck2Flexion			= IntToCStr(90 - (Round_Double(skel.getExtraAngle(EA_C4C7Flexion))));
	mNeck2Rotation			= IntToCStr(Round_Double(skel.getExtraAngle(EA_C4C7AxialRotation)));

	mNeck2Bending			= IntToCStr(Round_Double(skel.getExtraAngle(EA_C4C7LateralBending)));

/*	mAdjustedTrunkFlexion   = IntToCStr(Round_Double(skel.getExtraAngle(EA_TrunkFlexion)));//lResult.torsoang));
	mAdjustedTrunkRotation  = IntToCStr(Round_Double(skel.getExtraAngle(EA_TrunkAxialRotation)));//lResult.trrot));
	mAdjustedTrunkBending   = IntToCStr(Round_Double(skel.getExtraAngle(EA_TrunkLateralBending)));//skel.GetAngle(A_TrunkLateralBending)));*/
	mAdjustedTrunkFlexion	= IntToCStr(Round_Double(skel.getExtraAngle(EA_TrunkFlexion)));
	mAdjustedTrunkRotation	= IntToCStr(Round_Double(skel.getExtraAngle(EA_AdjustedTrunkAxialRotation)));
	mAdjustedTrunkBending	= IntToCStr(Round_Double(skel.getExtraAngle(EA_AdjustedTrunkLateralBending)));

//	mAdjustedTrunkFlexionPelv = IntToCStr(Round_Double(skel.getExtraAngle(EA_AdjustedTrunkFlexion)));
//	mAdjustedTrunkRotationPelv = IntToCStr(Round_Double(skel.getExtraAngle(EA_AdjustedTrunkAxialRotation)));
//	mAdjustedTrunkBendingPelv	= IntToCStr(Round_Double(skel.getExtraAngle(EA_AdjustedTrunkLateralBending)));

	mPelvicRotation         = IntToCStr((Round_Double(skel.getExtraAngle(EA_PelvisFlexion))));//gPelvicAngle));
	//mPelvisLateralBending   = IntToCStr((Round_Double(skel.GetAngle(A_PelvisLateralBending))));
	mPelvisLateralBending	= IntToCStr((Round_Double(skel.getExtraAngle(EA_PelvisLateralBending))));

	mRightElbowIncluded     = IntToCStr(Round_Double(skel.getExtraAngle(EA_ElbowRInc)));//lResult.RightElbowIncluded()));
	mRightShoulderVert      = IntToCStr(Round_Double(skel.getExtraAngle(EA_ShoulderRV)));//lResult.RightShoulderVertical()));
	mRightShoulderHorz      = IntToCStr(Round_Double(skel.getExtraAngle(EA_ShoulderRH)));//lResult.RightShoulderHorizontal()));
	mRightHumeralRot        = IntToCStr(Round_Double(skel.getExtraAngle(EA_HumerusRRot)));//lResult.RightHumeralRotation()));
	mRightForearmRot		= IntToCStr(Round_Double(skel.getExtraAngle(EA_ForearmRRot)));
		
	// C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
	// Skeleton &skel  = *(doc->GetSkeleton());
	double alpha = L5S1_ERECT_TILT - skel.getExtraAngle(EA_PelvisFlexion);
    mL5S1ForwardTilt = IntToCStr(Round_Double(90 - alpha));


	// 09/17/07 Sayan 
	// We comment this out as we will be showing Flexion and Extension separately.
	// mRightHandFlexion		= IntToCStr(int(skel.getExtraAngle(EA_HandRFlexion)));

	// Flexion is always negative and Extension is always positive?
	if ((int(skel.getExtraAngle(EA_HandRFlexion))) >= 0) {
		mRightHandExtension = IntToCStr(Round_Double(skel.getExtraAngle(EA_HandRFlexion)));
		mRightHandFlexion = IntToCStr(0);
	} else {
        mRightHandFlexion = IntToCStr(Round_Double(skel.getExtraAngle(EA_HandRFlexion)));
		mRightHandExtension = IntToCStr(0);
	};
    
	
	// 09/17/07 Sayan 
	// We comment this out as we will be showing ulnar deviation and radial deviation separately.
	// mRightHandDeviation		= IntToCStr(int(skel.getExtraAngle(EA_HandRDeviation)));
   
	// Radial deviation is always negative and ulnar deviation is always positive?

	if ((int(skel.getExtraAngle(EA_HandRDeviation))) >=0) {
		mRightHandUlnarDeviation = IntToCStr(Round_Double(skel.getExtraAngle(EA_HandRDeviation)));
		mRightHandRadialDeviation = IntToCStr(0);
	} else {
        mRightHandRadialDeviation = IntToCStr(Round_Double(skel.getExtraAngle(EA_HandRDeviation)));
		mRightHandUlnarDeviation =  IntToCStr(0);
	};

	mRightHipIncluded		= IntToCStr(Round_Double(skel.getExtraAngle(EA_HipRInc)));
	mRightHipVertical       = IntToCStr(Round_Double(skel.getExtraAngle(EA_HipRV)));//lRightHipIncluded);
	mRightHipHorizontal		= IntToCStr(Round_Double(skel.getExtraAngle(EA_HipRH)));
	mRightKneeIncluded      = IntToCStr(Round_Double(skel.getExtraAngle(EA_KneeRInc)));//lRightKneeIncluded);
	mRightAnkleIncluded     = IntToCStr(Round_Double(skel.getExtraAngle(EA_AnkleRInc)));//lResult.RightAnkleIncluded()));
	mRightFemoralRot		= IntToCStr(Round_Double(skel.getExtraAngle(EA_FemurRRot)));
	mRightLowerlegRot		= IntToCStr(Round_Double(skel.getExtraAngle(EA_LowerlegRRot)));

	// write min / max values
	// TODO these only change if the user changes the values in the factors dialog (ie almost never)
	// we shouldn't have to write them every update. just on init and when they are changed
	// TODO the CFloatRangeStatic limits should be changed exactly when these are changed
	const Factors& factors = lDocPtr->GetFactors();
	mMinHandFlexion				= IntToCStr(factors.mROM_HandFlexion[0]);
	mMaxHandFlexion				= IntToCStr(0); // intentionally zero
	mMinHandExtension			= IntToCStr(0); // intentionally zero
	mMaxHandExtension			= IntToCStr(factors.mROM_HandFlexion[1]);
	mMinHandUlnarDeviation		= IntToCStr(0); // intentionally zero
	mMaxHandUlnarDeviation		= IntToCStr(factors.mROM_HandDeviation[1]);
	mMinHandRadialDeviation		= IntToCStr(factors.mROM_HandDeviation[0]);
	mMaxHandRadialDeviation		= IntToCStr(0); // intentionally zero
	mMinForearmRot				= IntToCStr(factors.mROM_ForearmRot[0]);
	mMaxForearmRot				= IntToCStr(factors.mROM_ForearmRot[1]);
	mMinElbowIncluded			= IntToCStr(factors.mROM_ElbowInc[0]);
	mMaxElbowIncluded			= IntToCStr(factors.mROM_ElbowInc[1]);
	mMinShoulderVert			= IntToCStr(factors.mROM_ShoulderV[0]);
	mMaxShoulderVert			= IntToCStr(factors.mROM_ShoulderV[1]);
	mMinShoulderHorz			= IntToCStr(factors.mROM_ShoulderH[0]);
	mMaxShoulderHorz			= IntToCStr(factors.mROM_ShoulderH[1]);
	mMinHumeralRot				= IntToCStr(factors.mROM_HumerusRot[0]);
	mMaxHumeralRot				= IntToCStr(factors.mROM_HumerusRot[1]);
	mMinHipIncluded				= IntToCStr(factors.mROM_HipIncluded[0]);
	mMaxHipIncluded				= IntToCStr(factors.mROM_HipIncluded[1]);
	mMinHipVertical				= IntToCStr(factors.mROM_HipV[0]);
	mMaxHipVertical				= IntToCStr(factors.mROM_HipV[1]);
	mMinHipHorizontal			= IntToCStr(factors.mROM_HipH[0]);
	mMaxHipHorizontal			= IntToCStr(factors.mROM_HipH[1]);
	mMinFemoralRot				= IntToCStr(factors.mROM_FemurRot[0]);
	mMaxFemoralRot				= IntToCStr(factors.mROM_FemurRot[1]);
	mMinLowerlegRot				= IntToCStr(factors.mROM_LowerlegRot[0]);
	mMaxLowerlegRot				= IntToCStr(factors.mROM_LowerlegRot[1]);
	mMinKneeIncluded			= IntToCStr(factors.mROM_KneeInc[0]);
	mMaxKneeIncluded			= IntToCStr(factors.mROM_KneeInc[1]);
	mMinAnkleIncluded			= IntToCStr(factors.mROM_AnkleInc[0]);
	mMaxAnkleIncluded			= IntToCStr(factors.mROM_AnkleInc[1]);

	mMaxHeadFlexion				= IntToCStr(90 - (factors.mROM_HeadFlexion[0]+16));
	mMinHeadFlexion				= IntToCStr(90 - (factors.mROM_HeadFlexion[1]+16));
	mMaxNeck1Flexion			= IntToCStr(90 - (factors.mROM_UpperNeckFlexion[0]));
	mMinNeck1Flexion			= IntToCStr(90 - (factors.mROM_UpperNeckFlexion[1]));
	mMinNeck1Axial				= IntToCStr(factors.mROM_UpperNeckAxialRotation[0]);
	mMaxNeck1Axial				= IntToCStr(factors.mROM_UpperNeckAxialRotation[1]);
	mMinNeck1Lateral			= IntToCStr(factors.mROM_UpperNeckLateralBending[0]);
	mMaxNeck1Lateral			= IntToCStr(factors.mROM_UpperNeckLateralBending[1]);
	//head/torso
	mMaxHeadFlexion2			= IntToCStr(90 - (16 + factors.mROM_LowerNeckFlexion[0]+factors.mROM_UpperNeckFlexion[0]+factors.mROM_HeadFlexion[0]-180));
	mMinHeadFlexion2			= IntToCStr(90 - (16 + factors.mROM_LowerNeckFlexion[1]+factors.mROM_UpperNeckFlexion[1]+factors.mROM_HeadFlexion[1]-180));
	mMinHeadAxial2				= IntToCStr(factors.mROM_LowerNeckAxialRotation[0]+factors.mROM_UpperNeckAxialRotation[0]);
	mMaxHeadAxial2				= IntToCStr(factors.mROM_LowerNeckAxialRotation[1]+factors.mROM_UpperNeckAxialRotation[1]);
	mMinHeadLateral2			= IntToCStr(factors.mROM_LowerNeckLateralBending[0]+factors.mROM_UpperNeckLateralBending[0]);
	mMaxHeadLateral2			= IntToCStr(factors.mROM_LowerNeckLateralBending[1]+factors.mROM_UpperNeckLateralBending[1]);

	mMaxNeck2Flexion			= IntToCStr(90 - factors.mROM_LowerNeckFlexion[0]);
	mMinNeck2Flexion			= IntToCStr(90 - factors.mROM_LowerNeckFlexion[1]);
	mMinNeck2Axial				= IntToCStr(factors.mROM_LowerNeckAxialRotation[0]);
	mMaxNeck2Axial				= IntToCStr(factors.mROM_LowerNeckAxialRotation[1]);
	mMinNeck2Lateral			= IntToCStr(factors.mROM_LowerNeckLateralBending[0]);
	mMaxNeck2Lateral			= IntToCStr(factors.mROM_LowerNeckLateralBending[1]);
	mMinAdjustedTrunkFlexion	= IntToCStr(factors.mROM_AdjustedTrunkFlexion[0]);
	mMaxAdjustedTrunkFlexion	= IntToCStr(factors.mROM_AdjustedTrunkFlexion[1]);
	mMinAdjustedTrunkRotation	= IntToCStr(factors.mROM_AdjustedTrunkAxialRotation[0]);
	mMaxAdjustedTrunkRotation	= IntToCStr(factors.mROM_AdjustedTrunkAxialRotation[1]);
	mMinAdjustedTrunkBending	= IntToCStr(factors.mROM_AdjustedTrunkLateralBending[0]);
	mMaxAdjustedTrunkBending	= IntToCStr(factors.mROM_AdjustedTrunkLateralBending[1]);
	mMinPelvicRotation			= IntToCStr(factors.mROM_PelvisFlexion[0]);
	mMaxPelvicRotation			= IntToCStr(factors.mROM_PelvisFlexion[1]);
	mMinPelvisLateralBending	= IntToCStr(factors.mROM_PelvisLateralBending[0]);
	mMaxPelvisLateralBending	= IntToCStr(factors.mROM_PelvisLateralBending[1]);
	mMinL5S1ForwardTilt			= IntToCStr(0); // TODO
	mMaxL5S1ForwardTilt			= IntToCStr(0); // TODO

	
	// TODO adding red text on limit violation
	mLeftHandFlexionCtrl.SetMin(factors.mROM_HandFlexion[0]);
	mLeftHandFlexionCtrl.SetMax(0); // intentionally zero
	mRightHandFlexionCtrl.SetMin(factors.mROM_HandFlexion[0]);
	mRightHandFlexionCtrl.SetMax(0); // intentionally zero
	mLeftHandExtensionCtrl.SetMin(0); // intentionally zero
	mLeftHandExtensionCtrl.SetMax(factors.mROM_HandFlexion[1]);
	mRightHandExtensionCtrl.SetMin(0); // intentionally zero
	mRightHandExtensionCtrl.SetMax(factors.mROM_HandFlexion[1]);
	mLeftHandUlnarDeviationCtrl.SetMin(0); // intentionally zero
	mLeftHandUlnarDeviationCtrl.SetMax(factors.mROM_HandDeviation[1]);
	mRightHandUlnarDeviationCtrl.SetMin(0); // intentionally zero
	mRightHandUlnarDeviationCtrl.SetMax(factors.mROM_HandDeviation[1]);
	mLeftHandRadialDeviationCtrl.SetMin(factors.mROM_HandDeviation[0]);
	mLeftHandRadialDeviationCtrl.SetMax(0); // intentionally zero
	mRightHandRadialDeviationCtrl.SetMin(factors.mROM_HandDeviation[0]);
	mRightHandRadialDeviationCtrl.SetMax(0);
	mLeftForearmRotCtrl.SetMin(factors.mROM_ForearmRot[0]);
	mLeftForearmRotCtrl.SetMax(factors.mROM_ForearmRot[1]);
	mRightForearmRotCtrl.SetMin(factors.mROM_ForearmRot[0]);
	mRightForearmRotCtrl.SetMax(factors.mROM_ForearmRot[1]);
	mLeftElbowIncludedCtrl.SetMin(factors.mROM_ElbowInc[0]);
	mLeftElbowIncludedCtrl.SetMax(factors.mROM_ElbowInc[1]);
	mRightElbowIncludedCtrl.SetMin(factors.mROM_ElbowInc[0]);
	mRightElbowIncludedCtrl.SetMax(factors.mROM_ElbowInc[1]);
	mLeftShoulderVertCtrl.SetMin(factors.mROM_ShoulderV[0]);
	mLeftShoulderVertCtrl.SetMax(factors.mROM_ShoulderV[1]);
	mLeftShoulderHorzCtrl.SetMin(factors.mROM_ShoulderH[0]);
	mLeftShoulderHorzCtrl.SetMax(factors.mROM_ShoulderH[1]);
	mRightShoulderVertCtrl.SetMin(factors.mROM_ShoulderV[0]);
	mRightShoulderVertCtrl.SetMax(factors.mROM_ShoulderV[1]);
	mRightShoulderHorzCtrl.SetMin(factors.mROM_ShoulderH[0]);
	mRightShoulderHorzCtrl.SetMax(factors.mROM_ShoulderH[1]);
	mLeftHumeralRotCtrl.SetMin(factors.mROM_HumerusRot[0]);
	mLeftHumeralRotCtrl.SetMax(factors.mROM_HumerusRot[1]);
	mRightHumeralRotCtrl.SetMin(factors.mROM_HumerusRot[0]);
	mRightHumeralRotCtrl.SetMax(factors.mROM_HumerusRot[1]);
	mLeftHipIncludedCtrl.SetMin(factors.mROM_HipIncluded[0]);
	mLeftHipIncludedCtrl.SetMax(factors.mROM_HipIncluded[1]);
	mLeftHipVerticalCtrl.SetMin(factors.mROM_HipV[0]);
	mLeftHipVerticalCtrl.SetMax(factors.mROM_HipV[1]);
	mLeftHipHorizontalCtrl.SetMin(factors.mROM_HipH[0]);
	mLeftHipHorizontalCtrl.SetMax(factors.mROM_HipH[1]);
	mRightHipIncludedCtrl.SetMin(factors.mROM_HipIncluded[0]);
	mRightHipIncludedCtrl.SetMax(factors.mROM_HipIncluded[1]);
	mRightHipVerticalCtrl.SetMin(factors.mROM_HipV[0]);
	mRightHipVerticalCtrl.SetMax(factors.mROM_HipV[1]);
	mRightHipHorizontalCtrl.SetMin(factors.mROM_HipH[0]);
	mRightHipHorizontalCtrl.SetMax(factors.mROM_HipH[1]);
	mLeftFemoralRotCtrl.SetMin(factors.mROM_FemurRot[0]);
	mLeftFemoralRotCtrl.SetMax(factors.mROM_FemurRot[1]);
	mRightFemoralRotCtrl.SetMin(factors.mROM_FemurRot[0]);
	mRightFemoralRotCtrl.SetMax(factors.mROM_FemurRot[1]);
	mLeftLowerlegRotCtrl.SetMin(factors.mROM_LowerlegRot[0]);
	mLeftLowerlegRotCtrl.SetMax(factors.mROM_LowerlegRot[1]);
	mRightLowerlegRotCtrl.SetMin(factors.mROM_LowerlegRot[0]);
	mRightLowerlegRotCtrl.SetMax(factors.mROM_LowerlegRot[1]);
	mLeftKneeIncludedCtrl.SetMin(factors.mROM_KneeInc[0]);
	mLeftKneeIncludedCtrl.SetMax(factors.mROM_KneeInc[1]);
	mLeftAnkleIncludedCtrl.SetMin(factors.mROM_AnkleInc[0]);
	mLeftAnkleIncludedCtrl.SetMax(factors.mROM_AnkleInc[1]);
	mRightKneeIncludedCtrl.SetMin(factors.mROM_KneeInc[0]);
	mRightKneeIncludedCtrl.SetMax(factors.mROM_KneeInc[1]);
	mRightAnkleIncludedCtrl.SetMin(factors.mROM_AnkleInc[0]);
	mRightAnkleIncludedCtrl.SetMax(factors.mROM_AnkleInc[1]);


	mHeadFlexionCtrl2.SetMax(90 - (16 + factors.mROM_LowerNeckFlexion[0]+factors.mROM_UpperNeckFlexion[0]+factors.mROM_HeadFlexion[0]-180));
	mHeadFlexionCtrl2.SetMin(90 - (16 + factors.mROM_LowerNeckFlexion[1]+factors.mROM_UpperNeckFlexion[1]+factors.mROM_HeadFlexion[1]-180));
	mHeadAxialCtrl2.SetMin(factors.mROM_LowerNeckAxialRotation[0]+factors.mROM_UpperNeckAxialRotation[0]);
	mHeadAxialCtrl2.SetMax(factors.mROM_LowerNeckAxialRotation[1]+factors.mROM_UpperNeckAxialRotation[1]);
	mHeadLateralCtrl2.SetMin(factors.mROM_LowerNeckLateralBending[0]+factors.mROM_UpperNeckLateralBending[0]);
	mHeadLateralCtrl2.SetMax(factors.mROM_LowerNeckLateralBending[1]+factors.mROM_UpperNeckLateralBending[1]);
	
	mHeadFlexionCtrl.SetMax(90 - (16 + factors.mROM_HeadFlexion[0]));
	mHeadFlexionCtrl.SetMin(90 - (16 + factors.mROM_HeadFlexion[1]));

	mAdjustedNeck1FlexionCtrl.SetMax(90 - factors.mROM_UpperNeckFlexion[0]);
	mAdjustedNeck1FlexionCtrl.SetMin(90 - factors.mROM_UpperNeckFlexion[1]);
	mAdjustedNeck1RotationCtrl.SetMin(factors.mROM_UpperNeckAxialRotation[0]);
	mAdjustedNeck1RotationCtrl.SetMax(factors.mROM_UpperNeckAxialRotation[1]);
	mAdjustedNeck1BendingCtrl.SetMin(factors.mROM_UpperNeckLateralBending[0]);
	mAdjustedNeck1BendingCtrl.SetMax(factors.mROM_UpperNeckLateralBending[1]);

	mAdjustedNeck2FlexionCtrl.SetMax(90 - factors.mROM_LowerNeckFlexion[0]);
	mAdjustedNeck2FlexionCtrl.SetMin(90 - factors.mROM_LowerNeckFlexion[1]);
	mAdjustedNeck2RotationCtrl.SetMin(factors.mROM_LowerNeckAxialRotation[0]);
	mAdjustedNeck2RotationCtrl.SetMax(factors.mROM_LowerNeckAxialRotation[1]);
	mAdjustedNeck2BendingCtrl.SetMin(factors.mROM_LowerNeckLateralBending[0]);
	mAdjustedNeck2BendingCtrl.SetMax(factors.mROM_LowerNeckLateralBending[1]);


	mAdjustedTrunkFlexionCtrl.SetMin(factors.mROM_AdjustedTrunkFlexion[0]);
	mAdjustedTrunkFlexionCtrl.SetMax(factors.mROM_AdjustedTrunkFlexion[1]);
	mAdjustedTrunkRotationCtrl.SetMin(factors.mROM_AdjustedTrunkAxialRotation[0]);
	mAdjustedTrunkRotationCtrl.SetMax(factors.mROM_AdjustedTrunkAxialRotation[1]);
	mAdjustedTrunkBendingCtrl.SetMin(factors.mROM_AdjustedTrunkLateralBending[0]);
	mAdjustedTrunkBendingCtrl.SetMax(factors.mROM_AdjustedTrunkLateralBending[1]);
	mPelvicRotationCtrl.SetMin(factors.mROM_PelvisFlexion[0]);
	mPelvicRotationCtrl.SetMax(factors.mROM_PelvisFlexion[1]);
	mPelvisLateralBendingCtrl.SetMin(factors.mROM_PelvisLateralBending[0]);
	mPelvisLateralBendingCtrl.SetMax(factors.mROM_PelvisLateralBending[1]);
	// TODO
	//mL5S1ForwardTiltCtrl.SetMin(factors.mROM_PelvisFlexion[0]);
	//mL5S1ForwardTiltCtrl.SetMax(factors.mROM_PelvisFlexion[1]);
	UpdateUnits();


	// TODO
	// debugging oblique view
	mLeftAnkleBending = DblToCStr(skel.getLAnkleLateralBending(), 4, 4);
	mRightAnkleBending = DblToCStr(skel.getRAnkleLateralBending(), 4, 4);

    mHeader = lDocPtr->ReportHeader();

	mFooter = lDocPtr->ReportFooter();

	UpdateData(FALSE);

	Skeleton &lSkeleton = *lDocPtr->GetSkeleton();

	mHandForceTxtLVal.Format("%.1f", lSkeleton.getExtForce(JT_LHAND).length());
	mHandForceTxtRVal.Format("%.1f", lSkeleton.getExtForce(JT_RHAND).length());

	Vector3 offsetCoordinate = lDocPtr->getOffset();

	Vector3 LHand = lSkeleton.getJoint(JT_LHAND);

    mLeftHori.Format("%.1f", (LHand[1] -  offsetCoordinate[1]));
	mLeftLat.Format("%.1f", (LHand[0] - offsetCoordinate[0]));
	mLeftVert.Format("%.1f", (LHand[2] - offsetCoordinate[2]));


	Vector3 RHand = lSkeleton.getJoint(JT_RHAND);

	mRightHori.Format("%.1f", (RHand[1]  - offsetCoordinate[1]));
	mRightLat.Format("%.1f", (RHand[0] - offsetCoordinate[0]));
	mRightVert.Format("%.1f", (RHand[2] - offsetCoordinate[2]));
}

HBRUSH ReportPosture::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();

	//pDC->SetBkMode(TRANSPARENT);
	//pDC->SetBkMode(OPAQUE);
	//pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	//pDC->SetBkColor(lDocPtr->getBalColor());
	//pDC->SetBkColor(RGB(0,0,255));

	//return (HBRUSH)(CBrush(lDocPtr->getBalColor()));
	return hbr;
}

void ReportPosture::OnDisplayDialogcolor() 
{
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
	lDocPtr->setReportColor();
	
	Invalidate(true);
}

void ReportPosture::OnInitialUpdate()
{
	ReportView::OnInitialUpdate();

	// Set the hand force box units
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();
	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";
	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}


void ReportPosture::UpdateUnits() {
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";

	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";

	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}
