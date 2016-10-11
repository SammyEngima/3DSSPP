// ReportAnthropometry.cpp : implementation file
//

#include "ReportAnthropometry.h"

#include "hom.h"
#include "hom_doc.h"	// Necessary
#include "Skeleton.hpp"
#include "Types.h"
#include "Dg_Anthropometry.h"

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

/////////////////////////////////////////////////////////////////////////////
// ReportAnthropometry

ReportView* ReportAnthropometry::cPointer = NULL;

IMPLEMENT_DYNCREATE(ReportAnthropometry, ReportView)

ReportAnthropometry::ReportAnthropometry()
: ReportView(ReportAnthropometry::IDD)
{
   //{{AFX_DATA_INIT(ReportAnthropometry)

   //}}AFX_DATA_INIT
	mHandForceUnits = _T("");
	mUnitsLabel = _T("");
	mLeftHori = _T("");

   cPointer = this;
}

ReportAnthropometry::~ReportAnthropometry()
{
   cPointer = NULL;
}

//---------------------------------------------------------------------------
CString ReportAnthropometry::ReportName() const
{
   return "3DSSPP - Anthropometry Report";
}

void ReportAnthropometry::DoDataExchange(CDataExchange* pDX)
{
	ReportView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ReportAnthropometry)
	DDX_Text(pDX, IDC_CGLabel, mCGLabel);
//	if (mFootCG != mFootCGR)
	{
	DDX_Text(pDX, IDC_FootCG2, mFootCGR);
	}
//	if (mFootL != mFootLR)
	{
	DDX_Text(pDX, IDC_FootL2, mFootLR);
	}
//	if (mFootWt != mFootWtR)
	{
	DDX_Text(pDX, IDC_FootWt2, mFootWtR);
	}
	DDX_Text(pDX, IDC_FootCG, mFootCG);
	DDX_Text(pDX, IDC_Footer, mFooter);
	DDX_Text(pDX, IDC_FootL, mFootL);
	DDX_Text(pDX, IDC_FootWt, mFootWt);
	DDX_Text(pDX, IDC_Header, mHeader);
	DDX_Text(pDX, IDC_HipToL5CG, mHipToL5CG);
	DDX_Text(pDX, IDC_HipToL5L, mHipToL5L);
	DDX_Text(pDX, IDC_HipToL5Wt, mHipToL5Wt);
	DDX_Text(pDX, IDC_L5ToShoulderCG, mL5ToShoulderCG);
	DDX_Text(pDX, IDC_L5ToShoulderL, mL5ToShoulderL);
	DDX_Text(pDX, IDC_L5ToShoulderWt, mL5ToShoulderWt);
	DDX_Text(pDX, IDC_LengthLabel, mLengthLabel);
//	if (mLowerArmCG != mLowerArmCGR)
	{
	DDX_Text(pDX, IDC_LowerArmCG2, mLowerArmCGR);
	}
//	if (mLowerArmL != mLowerArmLR)
	{
	DDX_Text(pDX, IDC_LowerArmL2, mLowerArmLR);
	}
//	if (mLowerArmWt != mLowerArmWtR)
	{
	DDX_Text(pDX, IDC_LowerArmWt2, mLowerArmWtR);
	}
	DDX_Text(pDX, IDC_LowerArmL, mLowerArmL);
	DDX_Text(pDX, IDC_LowerArmWt, mLowerArmWt);
	DDX_Text(pDX, IDC_LowerArmCG, mLowerArmCG);
//	if (mLowerLegCG != mLowerLegCGR)
	{
	DDX_Text(pDX, IDC_LowerLegCG2, mLowerLegCGR);
	}
//	if (mLowerLegL != mLowerLegLR)
	{
	DDX_Text(pDX, IDC_LowerLegL2, mLowerLegLR);
	}
//	if (mLowerLegWt != mLowerLegWtR)
	{
	DDX_Text(pDX, IDC_LowerLegWt2, mLowerLegWtR);
	}
	DDX_Text(pDX, IDC_LowerLegCG, mLowerLegCG);
	DDX_Text(pDX, IDC_LowerLegL, mLowerLegL);
	DDX_Text(pDX, IDC_LowerLegWt, mLowerLegWt);
//	if (mUpperArmCG != mUpperArmCGR)
	{
	DDX_Text(pDX, IDC_UpperArmCG2, mUpperArmCGR);
	}
//	if (mUpperArmL != mUpperArmLR)
	{
	DDX_Text(pDX, IDC_UpperArmL2, mUpperArmLR);
	}
//	if (mUpperArmWt != mUpperArmWtR)
	{
	DDX_Text(pDX, IDC_UpperArmWt2, mUpperArmWtR);
	}
	DDX_Text(pDX, IDC_UpperArmCG, mUpperArmCG);
	DDX_Text(pDX, IDC_UpperArmL, mUpperArmL);
	DDX_Text(pDX, IDC_UpperArmWt, mUpperArmWt);
//	if (mUpperLegCG != mUpperLegCGR)
	{
	DDX_Text(pDX, IDC_UpperLegCG2, mUpperLegCGR);
	}
//	if (mUpperLegL != mUpperLegLR)
	{
	DDX_Text(pDX, IDC_UpperLegL2, mUpperLegLR);
	}
//	if (mUpperLegWt != mUpperLegWtR)
	{
	DDX_Text(pDX, IDC_UpperLegWt2, mUpperLegWtR);
	}
	DDX_Text(pDX, IDC_UpperLegL, mUpperLegL);
	DDX_Text(pDX, IDC_UpperLegWt, mUpperLegWt);
	DDX_Text(pDX, IDC_UpperLegCG, mUpperLegCG);
	DDX_Text(pDX, IDC_WeightLabel, mWeightLabel);
	DDX_Text(pDX, IDC_DiaphramMomentL, mDiaphramMomentL);
	DDX_Text(pDX, IDC_UpperBodyWt, mUpperBodyWt);
	DDX_Text(pDX, IDC_Factors, mFactorsName);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_HipToHipL, mHipToHipL);
	//DDX_Text(pDX, IDC_HipToHipWt, mHipToHipWt);
	// DDX_Text(pDX, IDC_ShoulderToShoulderL, mShoulderToShoulderL);
//	if (mHandCG != mHandCGR)
	{
	//DDX_Text(pDX, IDC_HandCG2, mHandCGR);
	}	
//	if (mHandL != mHandLR)
	{
	DDX_Text(pDX, IDC_HandL2, mHandLR);
	}	
//	if (mHandWt != mHandWtR)
	{
	//DDX_Text(pDX, IDC_HandWt2, mHandWtR);
	}	
	DDX_Text(pDX, IDC_HandL, mHandL);
	//DDX_Text(pDX, IDC_HandCG, mHandCG);
	//DDX_Text(pDX, IDC_HandWt, mHandWt);
	DDX_Text(pDX, IDC_HeadLr, mHeadL);
	DDX_Text(pDX, IDC_HeadCGr, mHeadCG);
	DDX_Text(pDX, IDC_headWtr, mHeadWt);
	DDX_Text(pDX, IDC_NasionLr, mNasionL);
	DDX_Text(pDX, IDC_Neck1Lr, mNeck1L);
	DDX_Text(pDX, IDC_Neck2Lr, mNeck2L);
	DDX_Text(pDX, IDC_Neck1CGr, mNeck1CG);
	DDX_Text(pDX, IDC_Neck1Wtr, mNeck1Wt);
	DDX_Text(pDX, IDC_Neck2CGr, mNeck2CG);
	DDX_Text(pDX, IDC_Neck2Wtr, mNeck2Wt);
	// DDX_Text(pDX, IDC_LowerArmAndHandL, mLowerArmAndHandL);
	// DDX_Text(pDX, IDC_LowerArmAndHandCG, mLowerArmAndHandCG);
	// DDX_Text(pDX, IDC_LowerArmAndHandWt, mLowerArmAndHandWt);
//	if (mHandWithFingersL != mHandWithFingersLR)
	{
	DDX_Text(pDX, IDC_HandWithFingersL2, mHandWithFingersLR);
	}	
//	if (mHandWithFingersCG != mHandWithFingersCGR)
	{
	DDX_Text(pDX, IDC_HandWithFingersCG2, mHandWithFingersCGR);
	}
//	if (mHandWithFingersWt != mHandWithFingersWtR)
	{
	DDX_Text(pDX, IDC_HandWithFingersWt2, mHandWithFingersWtR);
	}	
	DDX_Text(pDX, IDC_HandWithFingersL, mHandWithFingersL);
	DDX_Text(pDX, IDC_HandWithFingersCG, mHandWithFingersCG);
	DDX_Text(pDX, IDC_HandWithFingersWt, mHandWithFingersWt);

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

BEGIN_MESSAGE_MAP(ReportAnthropometry, ReportView)
   //{{AFX_MSG_MAP(ReportAnthropometry)
   ON_WM_CTLCOLOR()
   ON_WM_SIZE()
   ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
   //}}AFX_MSG_MAP
   //ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
   //ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ReportAnthropometry diagnostics

#ifdef _DEBUG
void ReportAnthropometry::AssertValid() const
{
   CFormView::AssertValid();
}

void ReportAnthropometry::Dump(CDumpContext& dc) const
{
   CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ReportAnthropometry overrides
//void ReportAnthropometry::OnFilePrintPreview()
//{
//   SSPPPrintPreview(this);
//}

void ReportAnthropometry::OnUpdate(CView* aSenderPtr,
                                   LPARAM aHint,
                                   CObject* aHintPtr)
{
   C_Hom_Doc* lDocPtr = static_cast <C_Hom_Doc*> (GetDocument());
   	Factors mFactors;
	String s = "Male";
	if(lDocPtr->Gender() == s)
		mFactors.RestoreMaleDefaults();
	else
		mFactors.RestoreFemaleDefaults();
	double mCM_Hand = mFactors.mCM_Hand;
	double mCM_Forearm	= mFactors.mCM_Forearm;
	double mCM_Upperarm = mFactors.mCM_Upperarm;
	double mCM_UpperNeck = mFactors.mCM_UpperNeck;
    double mCM_LowerNeck = mFactors.mCM_LowerNeck;
	double mCM_Head	= mFactors.mCM_Head;
	double mCM_L5S1ToShoulder = mFactors.mCM_L5S1ToShoulder;
	double mCM_Pelvis = mFactors.mCM_Pelvis;
	double mCM_UpperLeg = mFactors.mCM_UpperLeg;
	double mCM_LowerLeg = mFactors.mCM_LowerLeg;
	double mCM_Foot = mFactors.mCM_Foot;
   mHeader = lDocPtr->ReportHeader();
   const LinkLengths& lengths = lDocPtr->getLinkLengths();
   const LinkWeights& weights = lDocPtr->getLinkWeights();
   const LinkCGs& CGs = lDocPtr->getLinkCGs();
	mLengthLabel = CString("Length (")+ lDocPtr->LengthUnits() + ")";
	mCGLabel = CString("CG-to-Proximal End\nDistance (")
			+ lDocPtr->LengthUnits() + ")";
	mWeightLabel = CString("Weight (")+ lDocPtr->ForceUnits() + ")";
	double lengthConversion, forceConversion;
	if(lDocPtr->Is_English()) {
		lengthConversion = 0.39370;
		forceConversion = 0.224808;
	} else {
		lengthConversion = 1;
		forceConversion = 1;
	}
	
   // TODO unit conversions

  /* mFootCG.Format("%.1f",lengthConversion * CGs.Foot);//lDocPtr->getFootCG().ValueText();
   mFootL.Format("%.1f",lengthConversion * lengths.HeelToToe);//lDocPtr->getFootL().ValueText();
   if(Dg_Anthropometry::isred[16] == True)
		   mFootCG.Format("%.1f",atof(mFootL) * mCM_Foot);*/
	/*if(atof(mFootL) != Round_Double(Dg_Anthropometry::backl[16],1) && atof(mFootCG) == Round_Double(Dg_Anthropometry::backl[17],1))
	   {
		   mFootL.Format("%.1f",lengthConversion * lengths.HeelToToe);
		   mFootCG.Format("%.1f",atof(mFootL) * mCM_Foot);
		   Dg_Anthropometry::backl[16] = atof(mFootL);
		   Dg_Anthropometry::backl[17] = atof(mFootCG);
	   }
		else
		{*/
			mFootL.Format("%.1f",lengthConversion * lengths.HeelToToe);//mDocPtr->getHandCG().ValueText(); 
		    mFootCG.Format("%.1f",lengthConversion * CGs.Foot);//mDocPtr->getHandWithFingersL().ValueText();
			/*Dg_Anthropometry::backl[16] = atof(mFootL);
			Dg_Anthropometry::backl[17] = atof(mFootCG);
		}*/
   mFootWt.Format("%.1f",forceConversion * weights.Foot);//lDocPtr->getFootWt().ValueText();
	/* if(atof(mFootLR) != Round_Double(Dg_Anthropometry::backr[16],1) && atof(mFootCGR) == Round_Double(Dg_Anthropometry::backr[17],1))
	   {
		   mFootLR.Format("%.1f",lengthConversion * lengths.HeelToToeR);
		   mFootCGR.Format("%.1f",atof(mFootLR) * mCM_Foot);
		   Dg_Anthropometry::backr[16] = atof(mFootLR);
		   Dg_Anthropometry::backr[17] = atof(mFootCGR);
	   }
		else
		{*/
			mFootLR.Format("%.1f",lengthConversion * lengths.HeelToToeR);//mDocPtr->getHandCG().ValueText(); 
		    mFootCGR.Format("%.1f",lengthConversion * CGs.FootR);//mDocPtr->getHandWithFingersL().ValueText();
		/*	Dg_Anthropometry::backr[16] = atof(mFootLR);
			Dg_Anthropometry::backr[17] = atof(mFootCGR);
		}*/
   /*mFootCGR.Format("%.1f",lengthConversion * CGs.FootR);//lDocPtr->getFootCG().ValueText();
   mFootLR.Format("%.1f",lengthConversion * lengths.HeelToToeR);//lDocPtr->getFootL().ValueText();
   if(Dg_Anthropometry::isred[16] == True)
			mFootCGR.Format("%.1f",atof(mFootLR) * mCM_Foot);*/
   mFootWtR.Format("%.1f",forceConversion * weights.FootR);//lDocPtr->getFootWt().ValueText();
    /*  if(atof(mHipToL5L) != Round_Double(Dg_Anthropometry::backl[32],1) && atof(mHipToL5CG) == Round_Double(Dg_Anthropometry::backl[33],1))
	   {
		   mHipToL5L.Format("%.1f",lengthConversion * lengths.HipToL5S1);//mDocPtr->getHandWithFingersL().ValueText();
		   mHipToL5CG.Format("%.1f",atof(mHipToL5L) * mCM_Pelvis);
		   Dg_Anthropometry::backl[32] = atof(mHipToL5L);
		   Dg_Anthropometry::backl[33] = atof(mHipToL5L);
	   }
		else
}*/			mHipToL5CG.Format("%.1f",lengthConversion * CGs.Pelvis);//mDocPtr->getHandCG().ValueText(); 
		    mHipToL5L.Format("%.1f",lengthConversion * lengths.HipToL5S1);//mDocPtr->getHandWithFingersL().ValueText();
		/*	Dg_Anthropometry::backl[32] = atof(mHipToL5L);
			Dg_Anthropometry::backl[33] = atof(mHipToL5L);
		}*/
   /*mHipToL5CG.Format("%.1f",lengthConversion * CGs.Pelvis);//lDocPtr->getHipToL5CG().ValueText();
   mHipToL5L.Format("%.1f",lengthConversion * lengths.HipToL5S1);//lDocPtr->getHipToL5L().ValueText();
   	if(Dg_Anthropometry::isred[32] == True)
		   mHipToL5CG.Format("%.1f",atof(mHipToL5L) * mCM_Pelvis);*/
   mHipToL5Wt.Format("%.1f",forceConversion * weights.Pelvis);//lDocPtr->getHipToL5Wt().ValueText();
   mHipToHipL.Format("%.1f",lengthConversion * lengths.HipToHip);//lDocPtr->getHipToHipL().ValueText();
   //mHipToHipWt = lDocPtr->getHipToHipWt().ValueText();

   //mHipToShoulderL.Format("%.1f",lengthConversion * lengths.HipToShoulder);//lDocPtr->getHipToShoulderL().ValueText();
   //mHipToShoulderWt.Format("%.1f",forceConversion * weights.TorsoAndHead + forceConversion * weights.Pelvis);//lDocPtr->getHipToShoulderWt().ValueText();
      /*   if(atof(mL5ToShoulderL) != Round_Double(Dg_Anthropometry::backl[29],1) && atof(mL5ToShoulderCG) == Round_Double(Dg_Anthropometry::backl[30],1))
	   {
		   mL5ToShoulderL.Format("%.1f",lengthConversion * lengths.L5S1ToC7T1);//mDocPtr->getHandWithFingersL().ValueText();
		  mL5ToShoulderCG.Format("%.1f",atof(mL5ToShoulderL) * mCM_L5S1ToShoulder);
		   Dg_Anthropometry::backl[29] = atof(mL5ToShoulderL);
		   Dg_Anthropometry::backl[30] = atof(mL5ToShoulderCG);
	   }
		else
		{*/
			mL5ToShoulderCG.Format("%.1f",lengthConversion * CGs.L5S1ToShoulder);//mDocPtr->getHandCG().ValueText(); 
		    mL5ToShoulderL.Format("%.1f",lengthConversion * lengths.L5S1ToC7T1);//mDocPtr->getHandWithFingersL().ValueText();
			/*Dg_Anthropometry::backl[29] = atof(mL5ToShoulderL);
			Dg_Anthropometry::backl[30] = atof(mL5ToShoulderCG);
		}*/
   /*mL5ToShoulderCG.Format("%.1f",lengthConversion * CGs.L5S1ToShoulder);//lDocPtr->getL5ToShoulderCG().ValueText();
   mL5ToShoulderL.Format("%.1f",lengthConversion * lengths.L5S1ToShoulder);//lDocPtr->getL5ToShoulderL().ValueText();
      	if(Dg_Anthropometry::isred[29] == True)
		    mL5ToShoulderCG.Format("%.1f",atof(mL5ToShoulderL) * mCM_L5S1ToShoulder);*/
   mL5ToShoulderWt.Format("%.1f",forceConversion * weights.TorsoAndHead);//lDocPtr->getL5ToShoulderWt().ValueText();

   //mHandCG.Format("%.1f",lengthConversion * CGs.Hand);//lDocPtr->getHandCG().ValueText();
   mHandL.Format("%.1f",lengthConversion * lengths.Hand);//lDocPtr->getHandL().ValueText();
   //mHandWt.Format("%.1f",forceConversion * weights.Hand);//lDocPtr->getHandWt().ValueText();

   //mHandCGR.Format("%.1f",lengthConversion * CGs.HandR);//lDocPtr->getHandCG().ValueText();
   mHandLR.Format("%.1f",lengthConversion * lengths.HandR);//lDocPtr->getHandL().ValueText();
   //mHandWtR.Format("%.1f",forceConversion * weights.HandR);//lDocPtr->getHandWt().ValueText();
       /*  if(atof(mHeadL) != Round_Double(Dg_Anthropometry::backl[19],1) && atof(mHeadCG) == Round_Double(Dg_Anthropometry::backl[20],1))
	   {
		   mHeadL.Format("%.1f",lengthConversion * lengths.C1Tragion);//mDocPtr->getHandWithFingersL().ValueText();
		   mHeadCG.Format("%.1f",lengthConversion * CGs.Head);
		  Dg_Anthropometry::backl[19] = atof(mHeadL);
		   Dg_Anthropometry::backl[20] = atof(mHeadCG);
	   }
		else
		{*/
			mHeadCG.Format("%.1f",lengthConversion * CGs.Head);//mDocPtr->getHandCG().ValueText(); 
		    mHeadL.Format("%.1f",lengthConversion * lengths.C1Tragion);//mDocPtr->getHandWithFingersL().ValueText();
/*Dg_Anthropometry::backl[19] = atof(mHeadL);
			Dg_Anthropometry::backl[20] = atof(mHeadCG);
		}*/
  /* mHeadCG.Format("%.1f",lengthConversion * CGs.Head);//lDocPtr->getHeadCG().ValueText();
   mHeadL.Format("%.1f",lengthConversion * lengths.C1Tragion);//lDocPtr->getHeadL().ValueText();
       if(Dg_Anthropometry::isred[19] == True)
		    mHeadCG.Format("%.1f",atof(mHeadL) * mCM_Head);*/
   mHeadWt.Format("%.1f",forceConversion * weights.Head);//lDocPtr->getHeadWt().ValueText();
   
   mNasionL.Format("%.1f",lengthConversion * lengths.HeadRadius);


   mNeck1CG.Format("%.1f",lengthConversion * CGs.UpperNeck);
   mNeck1L.Format("%.1f",lengthConversion * lengths.C1C3);//lDocPtr->getHeadL().ValueText();

   mNeck1Wt.Format("%.1f",forceConversion * weights.UpperNeck);
   mNeck2CG.Format("%.1f",lengthConversion * CGs.LowerNeck);
   mNeck2L.Format("%.1f",lengthConversion * lengths.C4C7);//lDocPtr->getHeadL().ValueText();

   mNeck2Wt.Format("%.1f",forceConversion * weights.LowerNeck);


  /*       if(atof(mNeck2L) != Round_Double(Dg_Anthropometry::backl[26],1) && atof(mNeck2CG) == Round_Double(Dg_Anthropometry::backl[27],1))
	   {
		   mNeck2L.Format("%.1f",lengthConversion * lengths.C4C7);//mDocPtr->getHandWithFingersL().ValueText();
		   mNeck2CG.Format("%.1f",atof(mNeck2L) * mCM_LowerNeck);
		   Dg_Anthropometry::backl[26] = atof(mNeck2L);
		   Dg_Anthropometry::backl[27] = atof(mNeck2CG);
	   }
		else
		{
			mNeck2CG.Format("%.1f",lengthConversion * CGs.LowerNeck);//mDocPtr->getHandCG().ValueText(); 
		    mNeck2L.Format("%.1f",lengthConversion * lengths.C4C7);//mDocPtr->getHandWithFingersL().ValueText();
			Dg_Anthropometry::backl[26] = atof(mNeck2L);
			Dg_Anthropometry::backl[27] = atof(mNeck2CG);
		}
		*/
   /*mNeck2CG.Format("%.1f",lengthConversion * CGs.LowerNeck);//lDocPtr->getNeck2CG().ValueText();
	mNeck2L.Format("%.1f",lengthConversion * lengths.C4C7);//lDocPtr->getNeck2L().ValueText();
       if(Dg_Anthropometry::isred[26] == True)
		    mNeck2CG.Format("%.1f",atof(mNeck2L) * mCM_LowerNeck);*/
   //mNeck2Wt.Format("%.1f",forceConversion * weights.LowerNeck);//lDocPtr->getNeck2Wt().ValueText();
  /* if(atof(mHandWithFingersL) != Round_Double(Dg_Anthropometry::backl[1],1) && atof(mHandWithFingersCG) == Round_Double(Dg_Anthropometry::backl[2],1))
	   {    //if user only change the length value or change them both ,then CG should be changed correspondlingly 
		   mHandWithFingersL.Format("%.1f",lengthConversion * lengths.HandWithFingers);
		   mHandWithFingersCG.Format("%.1f",atof(mHandWithFingersL) * mCM_Hand);
		   Dg_Anthropometry::backl[1] = atof(mHandWithFingersL);
		   Dg_Anthropometry::backl[2] = atof(mHandWithFingersCG);
	   }
		else  //if user only change the CG or does not change anything ,then let the value be what the user input.This means changing the CG will not lead to the adjustment of its length,however.
		{*/
			mHandWithFingersCG.Format("%.1f",lengthConversion * CGs.Hand);//mDocPtr->getHandCG().ValueText(); 
		    mHandWithFingersL.Format("%.1f",lengthConversion * lengths.HandWithFingers);//mDocPtr->getHandWithFingersL().ValueText();
		/*	Dg_Anthropometry::backl[1] = atof(mHandWithFingersL);
			Dg_Anthropometry::backl[2] = atof(mHandWithFingersCG);
		}*/
   /*mHandWithFingersCG.Format("%.1f",lengthConversion * CGs.Hand);//lDocPtr->getHandCG().ValueText();
   mHandWithFingersL.Format("%.1f",lengthConversion * lengths.HandWithFingers);//lDocPtr->getHandWithFingersL().ValueText();
	if(Dg_Anthropometry::isred[1] == True)
		    mHandWithFingersCG.Format("%.1f",atof(mHandWithFingersL) * mCM_Hand);*/
   mHandWithFingersWt.Format("%.1f",forceConversion * weights.Hand);//lDocPtr->getHandWt().ValueText();
     /* if(atof(mHandWithFingersLR) != Round_Double(Dg_Anthropometry::backr[1],1) && atof(mHandWithFingersCGR) == Round_Double(Dg_Anthropometry::backr[2],1))
	   {    //if user only change the length value or change them both ,then CG should be changed correspondlingly 
		   mHandWithFingersLR.Format("%.1f",lengthConversion * lengths.HandWithFingersR);
		   mHandWithFingersCGR.Format("%.1f",atof(mHandWithFingersLR) * mCM_Hand);
		   Dg_Anthropometry::backr[1] = atof(mHandWithFingersLR);
		   Dg_Anthropometry::backr[2] = atof(mHandWithFingersCGR);
	   }
		else  //if user only change the CG or does not change anything ,then let the value be what the user input.This means changing the CG will not lead to the adjustment of its length,however.
		{*/
			mHandWithFingersCGR.Format("%.1f",lengthConversion * CGs.HandR);//mDocPtr->getHandCG().ValueText(); 
		    mHandWithFingersLR.Format("%.1f",lengthConversion * lengths.HandWithFingersR);//mDocPtr->getHandWithFingersL().ValueText();
			/*Dg_Anthropometry::backr[1] = atof(mHandWithFingersLR);
			Dg_Anthropometry::backr[2] = atof(mHandWithFingersCGR);
		}*/
   /*mHandWithFingersCGR.Format("%.1f",lengthConversion * CGs.HandR);//lDocPtr->getHandCG().ValueText();
   mHandWithFingersLR.Format("%.1f",lengthConversion * lengths.HandWithFingersR);//lDocPtr->getHandWithFingersL().ValueText();
   	if(Dg_Anthropometry::isred[1] == True)
		    mHandWithFingersCGR.Format("%.1f",atof(mHandWithFingersLR) * mCM_Hand);*/
   mHandWithFingersWtR.Format("%.1f",forceConversion * weights.HandR);//lDocPtr->getHandWt().ValueText();
     		/*if(atof(mLowerArmL) != Round_Double(Dg_Anthropometry::backl[4],1) && atof(mLowerArmCG) == Round_Double(Dg_Anthropometry::backl[5],1))
	   {
		   mLowerArmL.Format("%.1f",lengthConversion * lengths.Forearm);//mDocPtr->getHandWithFingersL().ValueText();
		   mLowerArmCG.Format("%.1f",atof(mLowerArmL) * mCM_Forearm);
		   Dg_Anthropometry::backl[4] = atof(mLowerArmL);
		   Dg_Anthropometry::backl[5] = atof(mLowerArmCG);
	   }
		else
		{*/
			mLowerArmCG.Format("%.1f",lengthConversion * CGs.Forearm);//mDocPtr->getHandCG().ValueText(); 
		    mLowerArmL.Format("%.1f",lengthConversion * lengths.Forearm);//mDocPtr->getHandWithFingersL().ValueText();
		/*	Dg_Anthropometry::backl[4] = atof(mLowerArmL);
			Dg_Anthropometry::backl[5] = atof(mLowerArmCG);
		}*/
   /*mLowerArmCG.Format("%.1f",lengthConversion * CGs.Forearm);//lDocPtr->getLowerArmCG().ValueText();
   mLowerArmL.Format("%.1f",lengthConversion * lengths.Forearm);//lDocPtr->getLowerArmL().ValueText();
   if(Dg_Anthropometry::isred[4] == True)
		    mLowerArmCG.Format("%.1f",atof(mLowerArmL) * mCM_Forearm);*/
   mLowerArmWt.Format("%.1f",forceConversion * weights.Forearm);//lDocPtr->getLowerArmWt().ValueText();
        	/*	if(atof(mLowerArmLR) != Round_Double(Dg_Anthropometry::backr[4],1) && atof(mLowerArmCGR) == Round_Double(Dg_Anthropometry::backr[5],1))
	   {
		   mLowerArmLR.Format("%.1f",lengthConversion * lengths.ForearmR);//mDocPtr->getHandWithFingersL().ValueText();
		   mLowerArmCGR.Format("%.1f",atof(mLowerArmLR) * mCM_Forearm);
		   Dg_Anthropometry::backr[4] = atof(mLowerArmLR);
		   Dg_Anthropometry::backr[5] = atof(mLowerArmCGR);
	   }
		else
		{*/
			mLowerArmCGR.Format("%.1f",lengthConversion * CGs.ForearmR);//mDocPtr->getHandCG().ValueText(); 
		    mLowerArmLR.Format("%.1f",lengthConversion * lengths.ForearmR);//mDocPtr->getHandWithFingersL().ValueText();
			/*Dg_Anthropometry::backr[4] = atof(mLowerArmLR);
			Dg_Anthropometry::backr[5] = atof(mLowerArmCGR);
		}*/
   /*mLowerArmCGR.Format("%.1f",lengthConversion * CGs.ForearmR);//lDocPtr->getLowerArmCG().ValueText();
   mLowerArmLR.Format("%.1f",lengthConversion * lengths.ForearmR);//lDocPtr->getLowerArmL().ValueText();
   if(Dg_Anthropometry::isred[4] == True)
		    mLowerArmCGR.Format("%.1f",atof(mLowerArmLR) * mCM_Forearm);*/
   mLowerArmWtR.Format("%.1f",forceConversion * weights.ForearmR);//lDocPtr->getLowerArmWt().ValueText();
   /* if(atof(mLowerLegL) != Round_Double(Dg_Anthropometry::backl[13],1) && atof(mLowerLegCG) == Round_Double(Dg_Anthropometry::backl[14],1))
	   {
		  mLowerLegL.Format("%.1f",lengthConversion * lengths.LowerLeg);//mDocPtr->getHandWithFingersL().ValueText();
		   mLowerLegCG.Format("%.1f",atof(mLowerLegL) * mCM_LowerLeg);
		   Dg_Anthropometry::backl[13] = atof(mLowerLegL);
		   Dg_Anthropometry::backl[14] = atof(mLowerLegCG);
	   }
		else
		{*/
			mLowerLegCG.Format("%.1f",lengthConversion * CGs.LowerLeg);//mDocPtr->getHandCG().ValueText(); 
		    mLowerLegL.Format("%.1f",lengthConversion * lengths.LowerLeg);//mDocPtr->getHandWithFingersL().ValueText();
		/*	Dg_Anthropometry::backl[13] = atof(mLowerLegL);
			Dg_Anthropometry::backl[14] = atof(mLowerLegCG);
		}*/
   /*mLowerLegCG.Format("%.1f",lengthConversion * CGs.LowerLeg);//lDocPtr->getLowerLegCG().ValueText();
   mLowerLegL.Format("%.1f",lengthConversion * lengths.LowerLeg);//lDocPtr->getLowerLegL().ValueText();
      if(Dg_Anthropometry::isred[13] == True)
		    mLowerLegCG.Format("%.1f",atof(mLowerLegL) * mCM_LowerLeg);*/
   mLowerLegWt.Format("%.1f",forceConversion * weights.LowerLeg);//lDocPtr->getLowerLegWt().ValueText();
       /*if(atof(mLowerLegLR) != Round_Double(Dg_Anthropometry::backr[13],1) && atof(mLowerLegCGR) == Round_Double(Dg_Anthropometry::backr[14],1))
	   {
		  mLowerLegLR.Format("%.1f",lengthConversion * lengths.LowerLegR);//mDocPtr->getHandWithFingersL().ValueText();
		   mLowerLegCGR.Format("%.1f",atof(mLowerLegLR) * mCM_LowerLeg);
		   Dg_Anthropometry::backr[13] = atof(mLowerLegLR);
		   Dg_Anthropometry::backr[14] = atof(mLowerLegCGR);
	   }
		else
		*/
			mLowerLegCGR.Format("%.1f",lengthConversion * CGs.LowerLegR);//mDocPtr->getHandCG().ValueText(); 
		    mLowerLegLR.Format("%.1f",lengthConversion * lengths.LowerLegR);//mDocPtr->getHandWithFingersL().ValueText();
		/*	Dg_Anthropometry::backr[13] = atof(mLowerLegLR);
			Dg_Anthropometry::backr[14] = atof(mLowerLegCGR);
		}*/
   /*mLowerLegCGR.Format("%.1f",lengthConversion * CGs.LowerLegR);//lDocPtr->getLowerLegCG().ValueText();
   mLowerLegLR.Format("%.1f",lengthConversion * lengths.LowerLegR);//lDocPtr->getLowerLegL().ValueText();
         if(Dg_Anthropometry::isred[13] == True)
		    mLowerLegCGR.Format("%.1f",atof(mLowerLegLR) * mCM_LowerLeg);*/
   mLowerLegWtR.Format("%.1f",forceConversion * weights.LowerLegR);//lDocPtr->getLowerLegWt().ValueText();
   	   	  /* if(atof(mUpperArmL) != Round_Double(Dg_Anthropometry::backl[7],1) && atof(mUpperArmCG) == Round_Double(Dg_Anthropometry::backl[8],1))
	   {
		   mUpperArmL.Format("%.1f",lengthConversion * lengths.UpperArm);//mDocPtr->getHandWithFingersL().ValueText();
		   mUpperArmCG.Format("%.1f",atof(mUpperArmL) * mCM_Upperarm);
		   Dg_Anthropometry::backl[7] = atof(mUpperArmL);
		   Dg_Anthropometry::backl[8] = atof(mUpperArmCG);
	   }
		else
		{*/
			mUpperArmCG.Format("%.1f",lengthConversion * CGs.UpperArm);//mDocPtr->getHandCG().ValueText(); 
		    mUpperArmL.Format("%.1f",lengthConversion * lengths.UpperArm);//mDocPtr->getHandWithFingersL().ValueText();
		/*	Dg_Anthropometry::backl[7] = atof(mUpperArmL);
			Dg_Anthropometry::backl[8] = atof(mUpperArmCG);
		}*/
   /*mUpperArmCG.Format("%.1f",lengthConversion * CGs.UpperArm);//lDocPtr->getUpperArmCG().ValueText();
   mUpperArmL.Format("%.1f",lengthConversion * lengths.UpperArm);//lDocPtr->getUpperArmL().ValueText();
         if(Dg_Anthropometry::isred[7] == True)
		     mUpperArmCG.Format("%.1f",atof(mUpperArmL) * mCM_Upperarm);*/
   mUpperArmWt.Format("%.1f",forceConversion * weights.UpperArm);//lDocPtr->getUpperArmWt().ValueText();
      	  /* 	   if(atof(mUpperArmLR) != Round_Double(Dg_Anthropometry::backr[7],1) && atof(mUpperArmCGR) == Round_Double(Dg_Anthropometry::backr[8],1))
	   {
		   mUpperArmLR.Format("%.1f",lengthConversion * lengths.UpperArmR);//mDocPtr->getHandWithFingersL().ValueText();
		   mUpperArmCGR.Format("%.1f",atof(mUpperArmLR) * mCM_Upperarm);
		   Dg_Anthropometry::backr[7] = atof(mUpperArmLR);
		   Dg_Anthropometry::backr[8] = atof(mUpperArmCGR);
	   }
		else
		{*/
			mUpperArmCGR.Format("%.1f",lengthConversion * CGs.UpperArmR);//mDocPtr->getHandCG().ValueText(); 
		    mUpperArmLR.Format("%.1f",lengthConversion * lengths.UpperArmR);//mDocPtr->getHandWithFingersL().ValueText();
		/*	Dg_Anthropometry::backr[7] = atof(mUpperArmLR);
			Dg_Anthropometry::backr[8] = atof(mUpperArmCGR);
		}*/
   /*mUpperArmCGR.Format("%.1f",lengthConversion * CGs.UpperArmR);//lDocPtr->getUpperArmCG().ValueText();
   mUpperArmLR.Format("%.1f",lengthConversion * lengths.UpperArmR);//lDocPtr->getUpperArmL().ValueText();
         if(Dg_Anthropometry::isred[7] == True)
		     mUpperArmCGR.Format("%.1f",atof(mUpperArmLR) * mCM_Upperarm);*/
   mUpperArmWtR.Format("%.1f",forceConversion * weights.UpperArmR);//lDocPtr->getUpperArmWt().ValueText();
   	  /* 	   	   if((atof(mUpperLegL) != Round_Double(Dg_Anthropometry::backl[10],1) && atof(mUpperLegCG) == Round_Double(Dg_Anthropometry::backl[11],1)))
	   {
		   mUpperLegL.Format("%.1f",lengthConversion * lengths.UpperLeg);//mDocPtr->getHandWithFingersL().ValueText();
		   mUpperLegCG.Format("%.1f",atof(mUpperLegL) * mCM_UpperLeg);
		   Dg_Anthropometry::backl[10] = atof(mUpperLegL);
		   Dg_Anthropometry::backl[11] = atof(mUpperLegCG);
	   }
		else
		{*/
			mUpperLegCG.Format("%.1f",lengthConversion * CGs.UpperLeg);//mDocPtr->getHandCG().ValueText(); 
		    mUpperLegL.Format("%.1f",lengthConversion * lengths.UpperLeg);//mDocPtr->getHandWithFingersL().ValueText();
		/*	Dg_Anthropometry::backl[10] = atof(mUpperLegL);
			Dg_Anthropometry::backl[11] = atof(mUpperLegCG);
		}*/
   /*mUpperLegCG.Format("%.1f",lengthConversion * CGs.UpperLeg);//lDocPtr->getUpperLegCG().ValueText();
   mUpperLegL.Format("%.1f",lengthConversion * lengths.UpperLeg);//lDocPtr->getUpperLegL().ValueText();
            if(Dg_Anthropometry::isred[10] == True)
		      mUpperLegCG.Format("%.1f",atof(mUpperLegL) * mCM_UpperLeg);*/
   mUpperLegWt.Format("%.1f",forceConversion * weights.UpperLeg);//lDocPtr->getUpperLegWt().ValueText();
      	   	   	  /* if((atof(mUpperLegLR) != Round_Double(Dg_Anthropometry::backr[10],1) && atof(mUpperLegCGR) == Round_Double(Dg_Anthropometry::backr[11],1)))
	   {
		   mUpperLegLR.Format("%.1f",lengthConversion * lengths.UpperLegR);//mDocPtr->getHandWithFingersL().ValueText();
		   mUpperLegCGR.Format("%.1f",atof(mUpperLegLR) * mCM_UpperLeg);
		   Dg_Anthropometry::backr[10] = atof(mUpperLegLR);
		   Dg_Anthropometry::backr[11] = atof(mUpperLegCGR);
	   }
		else
		{*/
			mUpperLegCGR.Format("%.1f",lengthConversion * CGs.UpperLegR);//mDocPtr->getHandCG().ValueText(); 
		    mUpperLegLR.Format("%.1f",lengthConversion * lengths.UpperLegR);//mDocPtr->getHandWithFingersL().ValueText();
		/*	Dg_Anthropometry::backr[10] = atof(mUpperLegLR);
			Dg_Anthropometry::backr[11] = atof(mUpperLegCGR);
		}*/
  /* mUpperLegCGR.Format("%.1f",lengthConversion * CGs.UpperLegR);//lDocPtr->getUpperLegCG().ValueText();
   mUpperLegLR.Format("%.1f",lengthConversion * lengths.UpperLegR);//lDocPtr->getUpperLegL().ValueText();
            if(Dg_Anthropometry::isred[10] == True)
		      mUpperLegCGR.Format("%.1f",atof(mUpperLegLR) * mCM_UpperLeg);*/
   mUpperLegWtR.Format("%.1f",forceConversion * weights.UpperLegR);//lDocPtr->getUpperLegWt().ValueText();

   Skeleton* skel = lDocPtr->GetSkeleton();
   const LowBackSagittalData_s& sagittal = skel->getLowBackSagittalData();
   mDiaphramMomentL.Format("%.1f",sagittal.diaphramMomentArm);//lDocPtr->getDiaphramMomentL().ValueText();
   mUpperBodyWt.Format("%.1f",(skel->getNetForce(JT_L5S1_TEMP) - skel->getNetForce(JT_LHAND) - skel->getNetForce(JT_RHAND)).length());//lDocPtr->getUpperBodyWt().ValueText();
   // mShoulderToShoulderL.Format("%.1f",lengthConversion * lengths.ShoulderWidth);//lDocPtr->getShoulderToShoulderL().ValueText();
mUpperBodyL.Format("%.1f",lengthConversion * CGs.UpperLegR);
mUpperBodyCG.Format("%.1f",(skel->getNetForce(JT_L5S1_TEMP) - skel->getNetForce(JT_LHAND) - skel->getNetForce(JT_RHAND)).length());
   mFactorsName = lDocPtr->GetFactors().mName;

   mFooter = lDocPtr->ReportFooter();

   UpdateData(FALSE);
   UpdateUnits();

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

	InvalidateRect(0);
}

HBRUSH ReportAnthropometry::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	C_Hom_Doc* lDocPtr = Get_App_Ptr()->Get_Document_Ptr(); //static_cast <C_Hom_Doc*> (GetDocument());

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	pDC->SetBkColor(lDocPtr->getBalColor());

	int ID = pWnd -> GetDlgCtrlID();

	/*
	// deal with body parts that separate left/right
	if (ID >= IDC_HandL && ID <= IDC_UpperArmWt && !lDocPtr->isredl.empty()) {
		int index = ID - IDC_HandL;
        
		if (index < 34) {
			switch (nCtlColor){
				case CTLCOLOR_STATIC:
				{
					if (lDocPtr->isredl[index])
					{
						pDC->SetTextColor(RGB(255, 0, 0));// Set color to Red
						return (HBRUSH)GetStockObject(NULL_BRUSH);
					}
				}
            
			default:
				break;
			}
		}
	}
	if (ID >= IDC_HandL2 && ID <= IDC_UpperArmWt2 && !lDocPtr->isredr.empty()) {
		int index = ID - IDC_HandL2;
        
		if (index < 34) {
		switch (nCtlColor){
			case CTLCOLOR_STATIC:
			{
				if (lDocPtr->isredr[index])
				{
					pDC->SetTextColor(RGB(255, 0, 0));// Set color to Red
					return (HBRUSH)GetStockObject(NULL_BRUSH);
				}
			}
            
		default:
			break;
		}
		}
	}

	if (ID >= IDC_UpperLegL && ID <= IDC_FootWt && !lDocPtr->isredl.empty()) {
		int index = ID - IDC_HandL;
        
		if (index < 34) {
			switch (nCtlColor){
				case CTLCOLOR_STATIC:
				{
					if (lDocPtr->isredl[index])
					{
						pDC->SetTextColor(RGB(255, 0, 0));// Set color to Red
						return (HBRUSH)GetStockObject(NULL_BRUSH);
					}
				}
            
			default:
				break;
			}
		}
	}
	if (ID >= IDC_UpperLegL2 && ID <= IDC_FootWt2 && !lDocPtr->isredr.empty()) {
		int index = ID - IDC_HandL2;
        
		if (index < 34) {
		switch (nCtlColor){
			case CTLCOLOR_STATIC:
			{
				if (lDocPtr->isredr[index])
				{
					pDC->SetTextColor(RGB(255, 0, 0));// Set color to Red
					return (HBRUSH)GetStockObject(NULL_BRUSH);
				}
			}
            
		default:
			break;
		}
		}
	}
	*/

	int index = -IDC_HandL;
	switch(ID) {
	case IDC_HandL :
	case IDC_HandWithFingersL :
	case IDC_HandWithFingersCG :
	case IDC_HandWithFingersWt :
	case IDC_LowerArmL :
	case IDC_LowerArmCG :
	case IDC_LowerArmWt :
	case IDC_UpperArmL :
	case IDC_UpperArmCG :
	case IDC_UpperArmWt :
	case IDC_UpperLegL :
	case IDC_UpperLegCG :
	case IDC_UpperLegWt :
	case IDC_LowerLegL :
	case IDC_LowerLegCG :
	case IDC_LowerLegWt :
	case IDC_FootL :
	case IDC_FootCG :
	case IDC_FootWt :
		index += ID;
		break;

	default: 
		break;
	}

	if (!lDocPtr->isredl.empty()) {    
		if (0 <= index && index <= 34) {
			switch (nCtlColor){
				case CTLCOLOR_STATIC:
				{
					if (lDocPtr->isredl[index])
					{
						pDC->SetTextColor(RGB(255, 0, 0));// Set color to Red
						return (HBRUSH)GetStockObject(NULL_BRUSH);
					}
				}
            
			default:
				break;
			}
			
		}
	}

	index = -IDC_HandL;
	switch(ID) {
	case IDC_HandL2 :
		index += IDC_HandL;
		break;
	case IDC_HandWithFingersL2 :
		index += IDC_HandWithFingersL;
		break;
	case IDC_HandWithFingersCG2 :
		index += IDC_HandWithFingersCG;
		break;
	case IDC_HandWithFingersWt2 :
		index += IDC_HandWithFingersWt;
		break;
	case IDC_LowerArmL2 :
		index += IDC_LowerArmL;
		break;
	case IDC_LowerArmCG2 :
		index += IDC_LowerArmCG;
		break;
	case IDC_LowerArmWt2 :
		index += IDC_LowerArmWt;
		break;
	case IDC_UpperArmL2 :
		index += IDC_UpperArmL;
		break;
	case IDC_UpperArmCG2 :
		index += IDC_UpperArmCG;
		break;
	case IDC_UpperArmWt2 :
		index += IDC_UpperArmWt;
		break;
	case IDC_UpperLegL2 :
		index += IDC_UpperLegL;
		break;
	case IDC_UpperLegCG2 :
		index += IDC_UpperLegCG;
		break;
	case IDC_UpperLegWt2 :
		index += IDC_UpperLegWt;
		break;
	case IDC_LowerLegL2 :
		index += IDC_LowerLegL;
		break;
	case IDC_LowerLegCG2 :
		index += IDC_LowerLegCG;
		break;
	case IDC_LowerLegWt2 :
		index += IDC_LowerLegWt;
		break;
	case IDC_FootL2 :
		index += IDC_FootL;
		break;
	case IDC_FootCG2 :
		index += IDC_FootCG;
		break;
	case IDC_FootWt2 :
		index += IDC_FootWt;
		break;

	default: 
		break;
	}

	if (!lDocPtr->isredr.empty()) {    
		if (0 <= index && index <= 34) {
			switch (nCtlColor){
				case CTLCOLOR_STATIC:
				{
					if (lDocPtr->isredr[index])
					{
						pDC->SetTextColor(RGB(255, 0, 0));// Set color to Red
						return (HBRUSH)GetStockObject(NULL_BRUSH);
					}
				}
            
			default:
				break;
			}
			
		}
	}
	
	// deal with body parts that doesn't have left/right
	index = -IDC_HandL;
	switch(ID) {
	case IDC_NasionLr:
		index += IDC_NasionL;
		break;

	case IDC_HeadLr :
	case IDC_HeadCGr :
	case IDC_headWtr :
		index += IDC_HeadL;
		index += (ID - IDC_HeadLr);
		break;

	case IDC_Neck1Lr :
		index += IDC_Neck1L;
		break;
	
	case IDC_Neck1CGr :
		index += IDC_Neck1CG;
		break;

	case IDC_Neck1Wtr :
		index += IDC_Neck1Wt;
		break;

	case IDC_Neck2Lr :
	case IDC_Neck2CGr :
	case IDC_Neck2Wtr :
		index += IDC_Neck2L;
		index += (ID - IDC_Neck2Lr);
		break;

	case IDC_L5ToShoulderL :
	case IDC_L5ToShoulderCG :
	case IDC_L5ToShoulderWt :
	case IDC_HipToL5L :
	case IDC_HipToL5CG : 
	case IDC_HipToL5Wt :
		index += ID;
		break;

	default:
		break;
	}
	
	if (!lDocPtr->isredl.empty()) {    
		if (0 <= index && index <= 34) {
			switch (nCtlColor){
				case CTLCOLOR_STATIC:
				{
					if (lDocPtr->isredl[index])
					{
						pDC->SetTextColor(RGB(255, 0, 0));// Set color to Red
						return (HBRUSH)GetStockObject(NULL_BRUSH);
					}
				}
            
			default:
				break;
			}
			
		}
	}
	

	return (HBRUSH)(CBrush(lDocPtr->getBalColor()));
}

void ReportAnthropometry::OnDisplayDialogcolor () 
{
   C_Hom_Doc* lDocPtr = (C_Hom_Doc*)GetDocument();
   lDocPtr->setReportColor();

   Invalidate(true);
}

void ReportAnthropometry::OnInitialUpdate()
{
	ReportView::OnInitialUpdate();

	// Set the hand force box units
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();
	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";
	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}

void ReportAnthropometry::UpdateUnits() {
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";

	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";

	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}



