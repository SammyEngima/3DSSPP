// Dg_FactorsROM.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "Dg_FactorsROM.h"


// Dg_FactorsROM dialog

IMPLEMENT_DYNAMIC(Dg_FactorsROM, CDialog)

Dg_FactorsROM::Dg_FactorsROM(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_FactorsROM::IDD, pParent)
{
	ForZeroValues = 0;
}

Dg_FactorsROM::~Dg_FactorsROM()
{
}

void Dg_FactorsROM::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ROMHandFlexionMin, mFactors.mROM_HandFlexion[0]);
	DDX_Text(pDX, IDC_ROMHandExtensionMax, mFactors.mROM_HandFlexion[1]);
	DDX_Text(pDX, IDC_ROMHandRadialDeviationMin, mFactors.mROM_HandDeviation[0]);
	DDX_Text(pDX, IDC_ROMHandUlnarDeviationMax, mFactors.mROM_HandDeviation[1]);
	DDX_Text(pDX, IDC_ROMForearmRotMin, mFactors.mROM_ForearmRot[0]);
	DDX_Text(pDX, IDC_ROMForearmRotMax, mFactors.mROM_ForearmRot[1]);
	DDX_Text(pDX, IDC_ROMElbowIncMin, mFactors.mROM_ElbowInc[0]);
	DDX_Text(pDX, IDC_ROMElbowIncMax, mFactors.mROM_ElbowInc[1]);
	DDX_Text(pDX, IDC_ROMShoulderVMin, mFactors.mROM_ShoulderV[0]);
	DDX_Text(pDX, IDC_ROMShoulderVMax, mFactors.mROM_ShoulderV[1]);
	DDX_Text(pDX, IDC_ROMShoulderHMin, mFactors.mROM_ShoulderH[0]);
	DDX_Text(pDX, IDC_ROMShoulderHMax, mFactors.mROM_ShoulderH[1]);
	DDX_Text(pDX, IDC_ROMHumeralRotMin, mFactors.mROM_HumerusRot[0]);
	DDX_Text(pDX, IDC_ROMHumeralRotMax, mFactors.mROM_HumerusRot[1]);
	DDX_Text(pDX, IDC_ROMHipIncMin, mFactors.mROM_HipIncluded[0]);
	DDX_Text(pDX, IDC_ROMHipIncMax, mFactors.mROM_HipIncluded[1]);
	DDX_Text(pDX, IDC_ROMHipVMin, mFactors.mROM_HipV[0]);
	DDX_Text(pDX, IDC_ROMHipVMax, mFactors.mROM_HipV[1]);
	DDX_Text(pDX, IDC_ROMHipHMin, mFactors.mROM_HipH[0]);
	DDX_Text(pDX, IDC_ROMHipHMax, mFactors.mROM_HipH[1]);
	DDX_Text(pDX, IDC_ROMFemoralRotMin, mFactors.mROM_FemurRot[0]);
	DDX_Text(pDX, IDC_ROMFemoralRotMax, mFactors.mROM_FemurRot[1]);
	DDX_Text(pDX, IDC_ROMLegRotMin, mFactors.mROM_LowerlegRot[0]);
	DDX_Text(pDX, IDC_ROMLegRotMax, mFactors.mROM_LowerlegRot[1]);
	DDX_Text(pDX, IDC_ROMKneeIncMin, mFactors.mROM_KneeInc[0]);
	DDX_Text(pDX, IDC_ROMKneeIncMax, mFactors.mROM_KneeInc[1]);
	DDX_Text(pDX, IDC_ROMAnkleIncMin, mFactors.mROM_AnkleInc[0]);
	DDX_Text(pDX, IDC_ROMAnkleIncMax, mFactors.mROM_AnkleInc[1]);

	DDX_Text(pDX, IDC_ROMTorsoFlexionMin, mFactors.mROM_AdjustedTrunkFlexion[0]);
	DDX_Text(pDX, IDC_ROMTorsoFlexionMax, mFactors.mROM_AdjustedTrunkFlexion[1]);
	DDX_Text(pDX, IDC_ROMTorsoAxialRotMin, mFactors.mROM_AdjustedTrunkAxialRotation[0]);
	DDX_Text(pDX, IDC_ROMTorsoAxialRotMax, mFactors.mROM_AdjustedTrunkAxialRotation[1]);
	DDX_Text(pDX, IDC_ROMTorsoLateralBendingMin, mFactors.mROM_AdjustedTrunkLateralBending[0]);
	DDX_Text(pDX, IDC_ROMTorsoLateralBendingMax, mFactors.mROM_AdjustedTrunkLateralBending[1]);
	DDX_Text(pDX, IDC_ROMNasionFlexionMin, mFactors.mROM_HeadFlexion[0]);	
	DDX_Text(pDX, IDC_ROMNasionFlexionMax, mFactors.mROM_HeadFlexion[1]);	
	DDX_Text(pDX, IDC_ROMUpperNeckFlexionMin, mFactors.mROM_UpperNeckFlexion[0]);
	DDX_Text(pDX, IDC_ROMUpperNeckFlexionMax, mFactors.mROM_UpperNeckFlexion[1]);
	DDX_Text(pDX, IDC_ROMUpperNeckAxialRotMin, mFactors.mROM_UpperNeckAxialRotation[0]);
	DDX_Text(pDX, IDC_ROMUpperNeckAxialRotMax, mFactors.mROM_UpperNeckAxialRotation[1]);
	DDX_Text(pDX, IDC_ROMUpperNeckLateralBendingMin, mFactors.mROM_UpperNeckLateralBending[0]);
	DDX_Text(pDX, IDC_ROMUpperNeckLateralBendingMax, mFactors.mROM_UpperNeckLateralBending[1]);
	DDX_Text(pDX, IDC_ROMLowerNeckFlexionMin, mFactors.mROM_LowerNeckFlexion[0]);
	DDX_Text(pDX, IDC_ROMLowerNeckFlexionMax, mFactors.mROM_LowerNeckFlexion[1]);
	DDX_Text(pDX, IDC_ROMLowerNeckAxialRotMin, mFactors.mROM_LowerNeckAxialRotation[0]);
	DDX_Text(pDX, IDC_ROMLowerNeckAxialRotMax, mFactors.mROM_LowerNeckAxialRotation[1]);
	DDX_Text(pDX, IDC_ROMLowerNeckLateralBendingMin, mFactors.mROM_LowerNeckLateralBending[0]);
	DDX_Text(pDX, IDC_ROMLowerNeckLateralBendingMax, mFactors.mROM_LowerNeckLateralBending[1]);
	DDX_Text(pDX, IDC_ROMPelvisForwardRotMin, mFactors.mROM_PelvisFlexion[0]);
	DDX_Text(pDX, IDC_ROMPelvisForwardRotMax, mFactors.mROM_PelvisFlexion[1]);
	DDX_Text(pDX, IDC_ROMPelvisLateralRotMin, mFactors.mROM_PelvisLateralBending[0]);
	DDX_Text(pDX, IDC_ROMPelvisLateralRotMax, mFactors.mROM_PelvisLateralBending[1]);
	//DDX_Text(pDX, IDC_ROML5S1ForwardTiltMin, 0);
	//DDX_Text(pDX, IDC_ROML5S1ForwartTiltMax, 0);
}


BEGIN_MESSAGE_MAP(Dg_FactorsROM, CDialog)
END_MESSAGE_MAP()


// Dg_FactorsROM message handlers
