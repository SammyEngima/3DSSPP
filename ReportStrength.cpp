#include "stdafx.h"
#include "ReportStrength.h"
#include "hom_doc.h"
#include "Skeleton.hpp"

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

ReportView *ReportStrength::cPointer = NULL;

IMPLEMENT_DYNCREATE(ReportStrength, ReportView)

ReportStrength::ReportStrength(void) : ReportView(ReportStrength::IDD)
{
	//{{AFX_DATA_INIT(ReportStrength)
	mFooter = _T("");
	mHeader = _T("");

	mLeftMeanLabel = _T("");
	mLeftMomentLabel = _T("");
	mLeftSDLabel = _T("");
	mRightMeanLabel = _T("");
	mRightMomentLabel = _T("");
	mRightSDLabel = _T("");

	mHandForceUnits = _T("");
	mUnitsLabel = _T("");
	mLeftHori = _T("");

    for(int i = 0; i < 5; ++i)
    {
        mLeftAbAd[i] = _T("");
	    mLeftAnkle[i] = _T("");
	    mLeftElbow[i] = _T("");
	    mLeftFlexExt[i] = _T("");
		mNeckFlexExt[i] = _T("");
		mNeckLatBend[i] = _T("");
	    mNeckRotation[i] = _T("");
	    mLeftHip[i] = _T("");
	    mLeftHumRot[i] = _T("");
	    mLeftKnee[i] = _T("");
	    mLeftLatBend[i] = _T("");
	    mLeftRotation[i] = _T("");
	    mLeftRotBkFd[i] = _T("");
	    mRightAbAd[i] = _T("");
	    mRightAnkle[i] = _T("");
	    mRightElbow[i] = _T("");
	    mRightHip[i] = _T("");
	    mRightHumRot[i] = _T("");
	    mRightKnee[i] = _T("");
	    mRightRotBkFd[i] = _T("");
    }
	//}}AFX_DATA_INIT

   cPointer = this;

   	hingeToMember[SH_LWristDeviation] = mLeftWristDev;
	hingeToMember[SH_LWristFlexion] = mLeftWristFlex;
	hingeToMember[SH_LForearmRotation] = mLeftWristRot;
	hingeToMember[SH_LElbowFlexion]  = mLeftElbow;
	hingeToMember[SH_LHumeralRotation] = mLeftHumRot;
	hingeToMember[SH_LShoulderAbduction] = mLeftAbAd;
	hingeToMember[SH_LShoulderRotation] = mLeftRotBkFd;
	hingeToMember[SH_LHipFlexion] = mLeftHip;
	hingeToMember[SH_LKneeFlexion] = mLeftKnee;
	hingeToMember[SH_LAnkleFlexion] = mLeftAnkle;
	hingeToMember[SH_RWristDeviation] = mRightWristDev;
	hingeToMember[SH_RWristFlexion] = mRightWristFlex;
	hingeToMember[SH_RForearmRotation] = mRightWristRot;
	hingeToMember[SH_RElbowFlexion] = mRightElbow;
	hingeToMember[SH_RHumeralRotation] = mRightHumRot;
	hingeToMember[SH_RShoulderAbduction] = mRightAbAd;
	hingeToMember[SH_RShoulderRotation] = mRightRotBkFd;
	hingeToMember[SH_RHipFlexion] = mRightHip;
	hingeToMember[SH_RKneeFlexion] = mRightKnee;
	hingeToMember[SH_RAnkleFlexion] = mRightAnkle;
	hingeToMember[SH_TorsoFlexion] = mLeftFlexExt;
	hingeToMember[SH_TorsoLateralBending] = mLeftLatBend;
	hingeToMember[SH_TorsoRotation] = mLeftRotation;
	hingeToMember[SH_NeckFlexion] = mNeckFlexExt;
	hingeToMember[SH_NeckLateralBending] = mNeckLatBend;
	hingeToMember[SH_NeckRotation] = mNeckRotation;
}

ReportStrength::~ReportStrength() { cPointer = NULL; }

CString ReportStrength::ReportName(void) const { return "3DSSPP - Strength Capabilities Report"; }

void ReportStrength::DoDataExchange(CDataExchange* pDX)
{
	ReportView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ReportStrength)
	DDX_Text(pDX, IDC_Footer, mFooter);
	DDX_Text(pDX, IDC_Header, mHeader);

	DDX_Text(pDX, IDC_LeftMeanLabel, mLeftMeanLabel);
	DDX_Text(pDX, IDC_LeftMomentLabel, mLeftMomentLabel);
	DDX_Text(pDX, IDC_LeftSDLabel, mLeftSDLabel);
	DDX_Text(pDX, IDC_RightMeanLabel, mRightMeanLabel);
	DDX_Text(pDX, IDC_RightMomentLabel, mRightMomentLabel);
	DDX_Text(pDX, IDC_RightSDLabel, mRightSDLabel);

	DDX_Text(pDX, IDC_LeftWristFlexMoment, mLeftWristFlex[0]);
	DDX_Text(pDX, IDC_LeftWristFlexEffect, mLeftWristFlex[1]);
	DDX_Text(pDX, IDC_LeftWristFlexMean, mLeftWristFlex[2]);
	DDX_Text(pDX, IDC_LeftWristFlexSD, mLeftWristFlex[3]);
	DDX_Text(pDX, IDC_LeftWristFlexCap, mLeftWristFlex[4]);

	DDX_Text(pDX, IDC_RightWristFlexMoment, mRightWristFlex[0]);
	DDX_Text(pDX, IDC_RightWristFlexEffect, mRightWristFlex[1]);
	DDX_Text(pDX, IDC_RightWristFlexMean, mRightWristFlex[2]);
	DDX_Text(pDX, IDC_RightWristFlexSD, mRightWristFlex[3]);
	DDX_Text(pDX, IDC_RightWristFlexCap, mRightWristFlex[4]);

	DDX_Text(pDX, IDC_LeftWristDevMoment, mLeftWristDev[0]);
	DDX_Text(pDX, IDC_LeftWristDevEffect, mLeftWristDev[1]);
	DDX_Text(pDX, IDC_LeftWristDevMean, mLeftWristDev[2]);
	DDX_Text(pDX, IDC_LeftWristDevSD, mLeftWristDev[3]);
	DDX_Text(pDX, IDC_LeftWristDevCap, mLeftWristDev[4]);

	DDX_Text(pDX, IDC_RightWristDevMoment, mRightWristDev[0]);
	DDX_Text(pDX, IDC_RightWristDevEffect, mRightWristDev[1]);
	DDX_Text(pDX, IDC_RightWristDevMean, mRightWristDev[2]);
	DDX_Text(pDX, IDC_RightWristDevSD, mRightWristDev[3]);
	DDX_Text(pDX, IDC_RightWristDevCap, mRightWristDev[4]);

	DDX_Text(pDX, IDC_LeftWristRotMoment, mLeftWristRot[0]);
	DDX_Text(pDX, IDC_LeftWristRotEffect, mLeftWristRot[1]);
	DDX_Text(pDX, IDC_LeftWristRotMean, mLeftWristRot[2]);
	DDX_Text(pDX, IDC_LeftWristRotSD, mLeftWristRot[3]);
	DDX_Text(pDX, IDC_LeftWristRotCap, mLeftWristRot[4]);

	DDX_Text(pDX, IDC_RightWristRotMoment, mRightWristRot[0]);
	DDX_Text(pDX, IDC_RightWristRotEffect, mRightWristRot[1]);
	DDX_Text(pDX, IDC_RightWristRotMean, mRightWristRot[2]);
	DDX_Text(pDX, IDC_RightWristRotSD, mRightWristRot[3]);
	DDX_Text(pDX, IDC_RightWristRotCap, mRightWristRot[4]);

    DDX_Text(pDX, IDC_LeftAbAdMoment, mLeftAbAd[0]);
	DDX_Text(pDX, IDC_LeftAbAdEffect, mLeftAbAd[1]);
	DDX_Text(pDX, IDC_LeftAbAdMean, mLeftAbAd[2]);
	DDX_Text(pDX, IDC_LeftAbAdSD, mLeftAbAd[3]);
    DDX_Text(pDX, IDC_LeftAbAdCap, mLeftAbAd[4]);

	DDX_Text(pDX, IDC_LeftAnkleMoment, mLeftAnkle[0]);
	DDX_Text(pDX, IDC_LeftAnkleEffect, mLeftAnkle[1]);
	DDX_Text(pDX, IDC_LeftAnkleMean, mLeftAnkle[2]);
	DDX_Text(pDX, IDC_LeftAnkleSD, mLeftAnkle[3]);
    DDX_Text(pDX, IDC_LeftAnkleCap, mLeftAnkle[4]);

	DDX_Text(pDX, IDC_LeftElbowMoment, mLeftElbow[0]);
	DDX_Text(pDX, IDC_LeftElbowEffect, mLeftElbow[1]);
	DDX_Text(pDX, IDC_LeftElbowMean, mLeftElbow[2]);
	DDX_Text(pDX, IDC_LeftElbowSD, mLeftElbow[3]);
    DDX_Text(pDX, IDC_LeftElbowCap, mLeftElbow[4]);
	
	// for neck
	DDX_Text(pDX, IDC_NeckFlexExtMoment, mNeckFlexExt[0]);
	DDX_Text(pDX, IDC_NeckFlexExtEffect, mNeckFlexExt[1]);
	DDX_Text(pDX, IDC_NeckFlexExtMean, mNeckFlexExt[2]);
	DDX_Text(pDX, IDC_NeckFlexExtSD, mNeckFlexExt[3]);
    DDX_Text(pDX, IDC_NeckFlexExtCap, mNeckFlexExt[4]);

	DDX_Text(pDX, IDC_NeckLatBendMoment, mNeckLatBend[0]);
	DDX_Text(pDX, IDC_NeckLatBendEffect, mNeckLatBend[1]);
	DDX_Text(pDX, IDC_NeckLatBendMean, mNeckLatBend[2]);
	DDX_Text(pDX, IDC_NeckLatBendSD, mNeckLatBend[3]);
    DDX_Text(pDX, IDC_NeckLatBendCap, mNeckLatBend[4]);
	
	DDX_Text(pDX, IDC_NeckRotationMoment, mNeckRotation[0]);
	DDX_Text(pDX, IDC_NeckRotationEffect, mNeckRotation[1]);
	DDX_Text(pDX, IDC_NeckRotationMean, mNeckRotation[2]);
	DDX_Text(pDX, IDC_NeckRotationSD, mNeckRotation[3]);
    DDX_Text(pDX, IDC_NeckRotationCap, mNeckRotation[4]);

	DDX_Text(pDX, IDC_LeftFlexExtMoment, mLeftFlexExt[0]);
	DDX_Text(pDX, IDC_LeftFlexExtEffect, mLeftFlexExt[1]);
	DDX_Text(pDX, IDC_LeftFlexExtMean, mLeftFlexExt[2]);
	DDX_Text(pDX, IDC_LeftFlexExtSD, mLeftFlexExt[3]);
    DDX_Text(pDX, IDC_LeftFlexExtCap, mLeftFlexExt[4]);
	
	DDX_Text(pDX, IDC_LeftHipMoment, mLeftHip[0]);
	DDX_Text(pDX, IDC_LeftHipEffect, mLeftHip[1]);
	DDX_Text(pDX, IDC_LeftHipMean, mLeftHip[2]);
	DDX_Text(pDX, IDC_LeftHipSD, mLeftHip[3]);
    DDX_Text(pDX, IDC_LeftHipCap, mLeftHip[4]);
	
	DDX_Text(pDX, IDC_LeftHumRotMoment, mLeftHumRot[0]);
	DDX_Text(pDX, IDC_LeftHumRotEffect, mLeftHumRot[1]);
	DDX_Text(pDX, IDC_LeftHumRotMean, mLeftHumRot[2]);
	DDX_Text(pDX, IDC_LeftHumRotSD, mLeftHumRot[3]);
    DDX_Text(pDX, IDC_LeftHumRotCap, mLeftHumRot[4]);
	
	DDX_Text(pDX, IDC_LeftKneeMoment, mLeftKnee[0]);
	DDX_Text(pDX, IDC_LeftKneeEffect, mLeftKnee[1]);
	DDX_Text(pDX, IDC_LeftKneeMean, mLeftKnee[2]);
	DDX_Text(pDX, IDC_LeftKneeSD, mLeftKnee[3]);
    DDX_Text(pDX, IDC_LeftKneeCap, mLeftKnee[4]);
	
	DDX_Text(pDX, IDC_LeftLatBendMoment, mLeftLatBend[0]);
	DDX_Text(pDX, IDC_LeftLatBendEffect, mLeftLatBend[1]);
	DDX_Text(pDX, IDC_LeftLatBendMean, mLeftLatBend[2]);
	DDX_Text(pDX, IDC_LeftLatBendSD, mLeftLatBend[3]);
    DDX_Text(pDX, IDC_LeftLatBendCap, mLeftLatBend[4]);
	
	DDX_Text(pDX, IDC_LeftRotationMoment, mLeftRotation[0]);
	DDX_Text(pDX, IDC_LeftRotationEffect, mLeftRotation[1]);
	DDX_Text(pDX, IDC_LeftRotationMean, mLeftRotation[2]);
	DDX_Text(pDX, IDC_LeftRotationSD, mLeftRotation[3]);
    DDX_Text(pDX, IDC_LeftRotationCap, mLeftRotation[4]);
	
	DDX_Text(pDX, IDC_LeftRotBkFdMoment, mLeftRotBkFd[0]);
	DDX_Text(pDX, IDC_LeftRotBkFdEffect, mLeftRotBkFd[1]);
	DDX_Text(pDX, IDC_LeftRotBkFdMean, mLeftRotBkFd[2]);
	DDX_Text(pDX, IDC_LeftRotBkFdSD, mLeftRotBkFd[3]);
    DDX_Text(pDX, IDC_LeftRotBkFdCap, mLeftRotBkFd[4]);
	
	DDX_Text(pDX, IDC_RightAbAdMoment, mRightAbAd[0]);
	DDX_Text(pDX, IDC_RightAbAdEffect, mRightAbAd[1]);
	DDX_Text(pDX, IDC_RightAbAdMean, mRightAbAd[2]);
	DDX_Text(pDX, IDC_RightAbAdSD, mRightAbAd[3]);
    DDX_Text(pDX, IDC_RightAbAdCap, mRightAbAd[4]);
	
	DDX_Text(pDX, IDC_RightAnkleMoment, mRightAnkle[0]);
	DDX_Text(pDX, IDC_RightAnkleEffect, mRightAnkle[1]);
	DDX_Text(pDX, IDC_RightAnkleMean, mRightAnkle[2]);
	DDX_Text(pDX, IDC_RightAnkleSD, mRightAnkle[3]);
    DDX_Text(pDX, IDC_RightAnkleCap, mRightAnkle[4]);
	
	DDX_Text(pDX, IDC_RightElbowMoment, mRightElbow[0]);
	DDX_Text(pDX, IDC_RightElbowEffect, mRightElbow[1]);
	DDX_Text(pDX, IDC_RightElbowMean, mRightElbow[2]);
	DDX_Text(pDX, IDC_RightElbowSD, mRightElbow[3]);
    DDX_Text(pDX, IDC_RightElbowCap, mRightElbow[4]);
	
	DDX_Text(pDX, IDC_RightHipMoment, mRightHip[0]);
	DDX_Text(pDX, IDC_RightHipEffect, mRightHip[1]);
	DDX_Text(pDX, IDC_RightHipMean, mRightHip[2]);
	DDX_Text(pDX, IDC_RightHipSD, mRightHip[3]);
    DDX_Text(pDX, IDC_RightHipCap, mRightHip[4]);
	
	DDX_Text(pDX, IDC_RightHumRotMoment, mRightHumRot[0]);
	DDX_Text(pDX, IDC_RightHumRotEffect, mRightHumRot[1]);
	DDX_Text(pDX, IDC_RightHumRotMean, mRightHumRot[2]);
	DDX_Text(pDX, IDC_RightHumRotSD, mRightHumRot[3]);
    DDX_Text(pDX, IDC_RightHumRotCap, mRightHumRot[4]);
	
	DDX_Text(pDX, IDC_RightKneeMoment, mRightKnee[0]);
    DDX_Text(pDX, IDC_RightKneeEffect, mRightKnee[1]);
	DDX_Text(pDX, IDC_RightKneeMean, mRightKnee[2]);
	DDX_Text(pDX, IDC_RightKneeSD, mRightKnee[3]);
    DDX_Text(pDX, IDC_RightKneeCap, mRightKnee[4]);
	
	DDX_Text(pDX, IDC_RightRotBkFdMoment, mRightRotBkFd[0]);
	DDX_Text(pDX, IDC_RightRotBkFdEffect, mRightRotBkFd[1]);
	DDX_Text(pDX, IDC_RightRotBkFdMean, mRightRotBkFd[2]);
	DDX_Text(pDX, IDC_RightRotBkFdSD, mRightRotBkFd[3]);	
    DDX_Text(pDX, IDC_RightRotBkFdCap, mRightRotBkFd[4]);

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
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ReportStrength, ReportView)
	//{{AFX_MSG_MAP(ReportStrength)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
	//}}AFX_MSG_MAP
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ReportStrength diagnostics

#ifdef _DEBUG
void ReportStrength::AssertValid() const { CFormView::AssertValid(); }
void ReportStrength::Dump(CDumpContext& dc) const { CFormView::Dump(dc); }
#endif

//void ReportStrength::OnFilePrintPreview() { SSPPPrintPreview(this); }

void ReportStrength::OnUpdate(CView* aSenderPtr,LPARAM aHint,CObject* aHintPtr)
{
    C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();
    //Analysis &lResults = lDocPtr->getResultsRef();

    // Fill in measurement units text
	mRightMomentLabel = mLeftMomentLabel = "Moment\n(" + lDocPtr->TorqueUnits() + ")";
	mRightMeanLabel = mLeftMeanLabel = "Mean\n(" + lDocPtr->TorqueUnits() + ")";
	mRightSDLabel = mLeftSDLabel = "SD\n(" + lDocPtr->TorqueUnits() + ")";

    // Retrieve joint force analysis results
	const StrengthData_s& strength = lDocPtr->GetSkeleton()->getStrengthData();
    //StrengthCapabilityData strength;
    //lResults.getStrengthCapability(&strength);

    // Write results to the form controls, converting to the desired system of units in the process
    /*CString *reported[] = {mLeftElbow,mRightElbow,mLeftHumRot,mRightHumRot,mLeftRotBkFd,mRightRotBkFd,mLeftAbAd,mRightAbAd,
                           mLeftFlexExt,mLeftLatBend,mLeftRotation,mLeftHip,mRightHip,mLeftKnee,mRightKnee,mLeftAnkle,mRightAnkle,
						   mLeftWristFlex, mRightWristFlex, mLeftWristDev, mRightWristDev, mLeftWristRot, mRightWristRot};*/
    for(int i = 0; i < NUM_STRENGTH_HINGES; ++i)
    {
        //temp.Value(100.0f * *((double*)&strength + 4*i), MetricCns);

		// Sayan 07/16/07 
		//
		// Unit conversions are now handled in a much cleaner way, and so the few lines
		// below can be cleaned up.

		// temp.Value(100.0f * strength.Moment[i], MetricCns);
		// hingeToMember[i][0] = temp.ValueText();
		// TODO trying to get better sig figs
		//(hingeToMember[i][0]).Format("%.0f", strength.Moment[i]);
		//(hingeToMember[i][0]).Format("%.1f", strength.Moment[i]);
		double mom = strength.Moment[i];
		double absmom = abs(mom);
		double log10mom = log10(absmom);
		int intmom = (int)log10mom;
		//hingeToMember[i][0].Format("%.*f", min(3,max(0,3-(int)log10(abs(strength.Moment[i])))), strength.Moment[i]);
		hingeToMember[i][0].Format("%.1f", strength.Moment[i]);


        hingeToMember[i][1] = strength.Effect[i];

		if(fabs(strength.Moment[i]) < 0.1F) // if moment is negligible, sd/mean not applicable
        {
			hingeToMember[i][1] = "----";
            hingeToMember[i][2] = "----";
            hingeToMember[i][3] = "----";
			hingeToMember[i][4] = "100";
        }
        else
        {
		  // Sayan 07/16/07 
		  //
	  	  // Unit conversions are now handled in a much cleaner way, and so the few lines
		  // below can be cleaned up.

			// temp.Value(100.0f * strength.Mean[i], MetricCns);
            // hingeToMember[i][2] = temp.ValueText();
			// temp.Value(100.0f * strength.SD[i], MetricCns);
            // hingeToMember[i][3] = temp.ValueText();



			// nothing after decimal point
            //hingeToMember[i][2].Format("%.0f", strength.Mean[i]);
			//hingeToMember[i][3].Format("%.0f", strength.SD[i]);

			// 4 sig figs
			//hingeToMember[i][2].Format("%.*f", min(3,max(0,3-(int)log10(abs(strength.Mean[i])))), strength.Mean[i]);
			//hingeToMember[i][3].Format("%.*f", min(3,max(0,3-(int)log10(abs(strength.SD[i])))), strength.SD[i]);

			// one after decimal point
			hingeToMember[i][2].Format("%.1f", strength.Mean[i]);
			hingeToMember[i][3].Format("%.1f", strength.SD[i]);
        
		
		if(strength.Percentile[i] >= 99.95) 
		{
			hingeToMember[i][4].Format("%.0f",strength.Percentile[i]);
		}
		else
		{
			hingeToMember[i][4].Format("%.1f",strength.Percentile[i]);
		}
		}
	}

    mHeader = lDocPtr->ReportHeader();
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
}

/*void ReportStrength::ReportLine()
{
    aEffect = aResult.muscleact[lSex][aIndex];
}*/


HBRUSH ReportStrength::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
    	
    C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();
     
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
    pDC->SetBkColor(lDocPtr->getBalColor());

    return (HBRUSH)(CBrush(lDocPtr->getBalColor()));
}

void ReportStrength::OnDisplayDialogcolor() 
{
	C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();
	lDocPtr->setReportColor();
	
	Invalidate(true);
}

void ReportStrength::OnInitialUpdate()
{
	ReportView::OnInitialUpdate();

	// Set the hand force box units
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();
	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";
	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}

void ReportStrength::UpdateUnits() {
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";

	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";

	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}
