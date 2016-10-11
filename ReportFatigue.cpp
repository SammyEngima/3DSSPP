#include "ReportFatigue.h"

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

ReportView *ReportFatigue::cPointer = NULL;

IMPLEMENT_DYNCREATE(ReportFatigue, ReportView)

ReportFatigue::ReportFatigue(void) : ReportView(ReportFatigue::IDD)
{
	//{{AFX_DATA_INIT(ReportFatigue)
	mFooter = _T("");
	mHeader = _T("");

	mHandForceUnits = _T("");
	mUnitsLabel = _T("");
	mLeftHori = _T("");

	//mEDC = 0;

    for(int i = 0; i < 3; ++i)
    {
	    mLeftAbAd[i] = _T("-");
	    mLeftAnkle[i] = _T("-");
	    mLeftElbow[i] = _T("-");
	    mLeftFlexExt[i]  = _T("-");
		mNeckFlexExt[i] = _T("-");
	    mLeftHip[i] = _T("-");
	    mLeftHumRot[i] = _T("-");
	    mLeftKnee[i] = _T("-");
	    mLeftLatBend[i] = _T("-");
	    mLeftRotation[i] = _T("-");
	    mNeckLatBend[i] = _T("-");
	    mNeckRotation[i] = _T("-");
	    mLeftRotBkFd[i] = _T("-");
	    mRightAbAd[i] = _T("-");
	    mRightAnkle[i] = _T("-");
	    mRightElbow[i] = _T("-");
	    mRightHip[i] = _T("-");
	    mRightHumRot[i] = _T("-");
	    mRightKnee[i] = _T("-");
	    mRightRotBkFd[i] = _T("-");

		mLeftAbAdDC[i] = _T("-");
	    mLeftAnkleDC[i] = _T("-");
	    mLeftElbowDC[i] = _T("-");
	    mLeftFlexExtDC[i] = _T("-");
	    mNeckFlexExtDC[i] = _T("-");
	    mLeftHipDC[i] = _T("-");
	    mLeftHumRotDC[i] = _T("-");
	    mLeftKneeDC[i] = _T("-");
	    mLeftLatBendDC[i] = _T("-");
	    mLeftRotationDC[i] = _T("-");
	    mNeckLatBendDC[i] = _T("-");
	    mNeckRotationDC[i] = _T("-");
	    mLeftRotBkFdDC[i] = _T("-");
	    mRightAbAdDC[i] = _T("-");
	    mRightAnkleDC[i] = _T("-");
	    mRightElbowDC[i] = _T("-");
	    mRightHipDC[i] = _T("-");
	    mRightHumRotDC[i] = _T("-");
	    mRightKneeDC[i] = _T("-");
	    mRightRotBkFdDC[i] = _T("-");

		mTCT = 0;
		mEC = 0;
		mED = 0;
		mEDC_text  = _T("-");
		mEDC = 0;

    }
	//}}AFX_DATA_INIT

   cPointer = this;
	hingeToMember[SH_LWristDeviation] = mLeftWristDev;
	hingeToMember[SH_LWristFlexion] = mLeftWristFlex;
	hingeToMember[SH_LForearmRotation] = mLeftForearmRot;
	hingeToMember[SH_LElbowFlexion] = mLeftElbow;
	hingeToMember[SH_LHumeralRotation] = mLeftHumRot;
	hingeToMember[SH_LShoulderAbduction] = mLeftAbAd;
	hingeToMember[SH_LShoulderRotation] = mLeftRotBkFd;
	hingeToMember[SH_LHipFlexion] = mLeftHip;
	hingeToMember[SH_LKneeFlexion] = mLeftKnee;
	hingeToMember[SH_LAnkleFlexion] = mLeftAnkle;
	hingeToMember[SH_RWristDeviation] = mRightWristDev;
	hingeToMember[SH_RWristFlexion] = mRightWristFlex;
	hingeToMember[SH_RForearmRotation] = mRightForearmRot;
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

	hingeToMemberDC[SH_LWristDeviation] = mLeftWristDevDC;
	hingeToMemberDC[SH_LWristFlexion] = mLeftWristFlexDC;
	hingeToMemberDC[SH_LForearmRotation] = mLeftForearmRotDC;
	hingeToMemberDC[SH_LElbowFlexion] = mLeftElbowDC;
	hingeToMemberDC[SH_LHumeralRotation] = mLeftHumRotDC;
	hingeToMemberDC[SH_LShoulderAbduction] = mLeftAbAdDC;
	hingeToMemberDC[SH_LShoulderRotation] = mLeftRotBkFdDC;
	hingeToMemberDC[SH_LHipFlexion] = mLeftHipDC;
	hingeToMemberDC[SH_LKneeFlexion] = mLeftKneeDC;
	hingeToMemberDC[SH_LAnkleFlexion] = mLeftAnkleDC;
	hingeToMemberDC[SH_RWristDeviation] = mRightWristDevDC;
	hingeToMemberDC[SH_RWristFlexion] = mRightWristFlexDC;
	hingeToMemberDC[SH_RForearmRotation] = mRightForearmRotDC;
	hingeToMemberDC[SH_RElbowFlexion] = mRightElbowDC;
	hingeToMemberDC[SH_RHumeralRotation] = mRightHumRotDC;
	hingeToMemberDC[SH_RShoulderAbduction] = mRightAbAdDC;
	hingeToMemberDC[SH_RShoulderRotation] = mRightRotBkFdDC;
	hingeToMemberDC[SH_RHipFlexion] = mRightHipDC;
	hingeToMemberDC[SH_RKneeFlexion] = mRightKneeDC;
	hingeToMemberDC[SH_RAnkleFlexion] = mRightAnkleDC;
	hingeToMemberDC[SH_TorsoFlexion] = mLeftFlexExtDC;
	hingeToMemberDC[SH_TorsoLateralBending] = mLeftLatBendDC;
	hingeToMemberDC[SH_TorsoRotation] = mLeftRotationDC;
	hingeToMemberDC[SH_NeckFlexion] = mNeckFlexExtDC;
	hingeToMemberDC[SH_NeckLateralBending] = mNeckLatBendDC;
	hingeToMemberDC[SH_NeckRotation] = mNeckRotationDC;

	hingeToMemberMT[SH_LWristDeviation] = mLeftWristDevMT;
	hingeToMemberMT[SH_LWristFlexion] = mLeftWristFlexMT;
	hingeToMemberMT[SH_LForearmRotation] = mLeftForearmRotMT;
	hingeToMemberMT[SH_LElbowFlexion] = mLeftElbowMT;
	hingeToMemberMT[SH_LHumeralRotation] = mLeftHumRotMT;
	hingeToMemberMT[SH_LShoulderAbduction] = mLeftAbAdMT;
	hingeToMemberMT[SH_LShoulderRotation] = mLeftRotBkFdMT;
	hingeToMemberMT[SH_LHipFlexion] = mLeftHipMT;
	hingeToMemberMT[SH_LKneeFlexion] = mLeftKneeMT;
	hingeToMemberMT[SH_LAnkleFlexion] = mLeftAnkleMT;
	hingeToMemberMT[SH_RWristDeviation] = mRightWristDevMT;
	hingeToMemberMT[SH_RWristFlexion] = mRightWristFlexMT;
	hingeToMemberMT[SH_RForearmRotation] = mRightForearmRotMT;
	hingeToMemberMT[SH_RElbowFlexion] = mRightElbowMT;
	hingeToMemberMT[SH_RHumeralRotation] = mRightHumRotMT;
	hingeToMemberMT[SH_RShoulderAbduction] = mRightAbAdMT;
	hingeToMemberMT[SH_RShoulderRotation] = mRightRotBkFdMT;
	hingeToMemberMT[SH_RHipFlexion] = mRightHipMT;
	hingeToMemberMT[SH_RKneeFlexion] = mRightKneeMT;
	hingeToMemberMT[SH_RAnkleFlexion] = mRightAnkleMT;
	hingeToMemberMT[SH_TorsoFlexion] = mLeftFlexExtMT;
	hingeToMemberMT[SH_TorsoLateralBending] = mLeftLatBendMT;
	hingeToMemberMT[SH_TorsoRotation] = mLeftRotationMT;
	hingeToMemberMT[SH_NeckFlexion] = mNeckFlexExtMT;
	hingeToMemberMT[SH_NeckLateralBending] = mNeckLatBendMT;
	hingeToMemberMT[SH_NeckRotation] = mNeckRotationMT;
}

ReportFatigue::~ReportFatigue() { cPointer = NULL; }

CString ReportFatigue::ReportName(void) const { return "3DSSPP - Localized Fatigue Report"; }

void ReportFatigue::DoDataExchange(CDataExchange* pDX)
{
	ReportView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ReportFatigue)
	DDX_Text(pDX, IDC_Footer, mFooter);
	DDX_Text(pDX, IDC_Header, mHeader);
	DDX_Text(pDX, IDC_LeftForearmRot5, mLeftForearmRot[0]);
	DDX_Text(pDX, IDC_LeftForearmRot25, mLeftForearmRot[1]);
	DDX_Text(pDX, IDC_LeftForearmRot50, mLeftForearmRot[2]);

	DDX_Text(pDX, IDC_LeftForearmRot6, mLeftForearmRotDC[0]);
	DDX_Text(pDX, IDC_LeftForearmRot26, mLeftForearmRotDC[1]);
	DDX_Text(pDX, IDC_LeftForearmRot51, mLeftForearmRotDC[2]);

	DDX_Text(pDX, IDC_LeftForearmRot7, mLeftForearmRotMT[0]);
	DDX_Text(pDX, IDC_LeftForearmRot27, mLeftForearmRotMT[1]);
	DDX_Text(pDX, IDC_LeftForearmRot52, mLeftForearmRotMT[2]);

	DDX_Text(pDX, IDC_LeftWristFlex5, mLeftWristFlex[0]);
	DDX_Text(pDX, IDC_LeftWristFlex25, mLeftWristFlex[1]);
	DDX_Text(pDX, IDC_LeftWristFlex50, mLeftWristFlex[2]);

	DDX_Text(pDX, IDC_LeftWristFlex6, mLeftWristFlexDC[0]);
	DDX_Text(pDX, IDC_LeftWristFlex26, mLeftWristFlexDC[1]);
	DDX_Text(pDX, IDC_LeftWristFlex51, mLeftWristFlexDC[2]);

	
	DDX_Text(pDX, IDC_LeftWristFlex8, mLeftWristFlexMT[0]);
	DDX_Text(pDX, IDC_LeftWristFlex27, mLeftWristFlexMT[1]);
	DDX_Text(pDX, IDC_LeftWristFlex52, mLeftWristFlexMT[2]);

	DDX_Text(pDX, IDC_LeftWristDev5, mLeftWristDev[0]);
	DDX_Text(pDX, IDC_LeftWristDev25, mLeftWristDev[1]);
	DDX_Text(pDX, IDC_LeftWristDev50, mLeftWristDev[2]);

	DDX_Text(pDX, IDC_LeftWristDev6, mLeftWristDevDC[0]);
	DDX_Text(pDX, IDC_LeftWristDev26, mLeftWristDevDC[1]);
	DDX_Text(pDX, IDC_LeftWristDev51, mLeftWristDevDC[2]);

	DDX_Text(pDX, IDC_LeftWristDev7, mLeftWristDevMT[0]);
	DDX_Text(pDX, IDC_LeftWristDev27, mLeftWristDevMT[1]);
	DDX_Text(pDX, IDC_LeftWristDev52, mLeftWristDevMT[2]);

	DDX_Text(pDX, IDC_LeftAbAd5, mLeftAbAd[0]);
	DDX_Text(pDX, IDC_LeftAbAd25, mLeftAbAd[1]);
    DDX_Text(pDX, IDC_LeftAbAd50, mLeftAbAd[2]);
	DDX_Text(pDX, IDC_LeftAnkle5, mLeftAnkle[0]);
	DDX_Text(pDX, IDC_LeftAnkle25, mLeftAnkle[1]);
    DDX_Text(pDX, IDC_LeftAnkle50, mLeftAnkle[2]);
	DDX_Text(pDX, IDC_LeftElbow5, mLeftElbow[0]);
	DDX_Text(pDX, IDC_LeftElbow25, mLeftElbow[1]);
    DDX_Text(pDX, IDC_LeftElbow50, mLeftElbow[2]);


	DDX_Text(pDX, IDC_LeftAbAd6, mLeftAbAdDC[0]);
	DDX_Text(pDX, IDC_LeftAbAd26, mLeftAbAdDC[1]);
    DDX_Text(pDX, IDC_LeftAbAd51, mLeftAbAdDC[2]);
	DDX_Text(pDX, IDC_LeftAnkle6, mLeftAnkleDC[0]);
	DDX_Text(pDX, IDC_LeftAnkle26, mLeftAnkleDC[1]);
    DDX_Text(pDX, IDC_LeftAnkle51, mLeftAnkleDC[2]);
	DDX_Text(pDX, IDC_LeftElbow6, mLeftElbowDC[0]);
	DDX_Text(pDX, IDC_LeftElbow26, mLeftElbowDC[1]);
    DDX_Text(pDX, IDC_LeftElbow51, mLeftElbowDC[2]);

	DDX_Text(pDX, IDC_LeftAbAd7, mLeftAbAdMT[0]);
	DDX_Text(pDX, IDC_LeftAbAd27, mLeftAbAdMT[1]);
    DDX_Text(pDX, IDC_LeftAbAd52, mLeftAbAdMT[2]);
	DDX_Text(pDX, IDC_LeftAnkle7, mLeftAnkleMT[0]);
	DDX_Text(pDX, IDC_LeftAnkle27, mLeftAnkleMT[1]);
    DDX_Text(pDX, IDC_LeftAnkle52, mLeftAnkleMT[2]);
	DDX_Text(pDX, IDC_LeftElbow7, mLeftElbowMT[0]);
	DDX_Text(pDX, IDC_LeftElbow27, mLeftElbowMT[1]);
    DDX_Text(pDX, IDC_LeftElbow52, mLeftElbowMT[2]);

	DDX_Text(pDX, IDC_NeckFlexExt5, mNeckFlexExt[0]);
	DDX_Text(pDX, IDC_NeckFlexExt25, mNeckFlexExt[1]);
    DDX_Text(pDX, IDC_NeckFlexExt50, mNeckFlexExt[2]);
	DDX_Text(pDX, IDC_LeftFlexExt5, mLeftFlexExt[0]);
	DDX_Text(pDX, IDC_LeftFlexExt25, mLeftFlexExt[1]);
    DDX_Text(pDX, IDC_LeftFlexExt50, mLeftFlexExt[2]);
	DDX_Text(pDX, IDC_LeftHip5, mLeftHip[0]);
	DDX_Text(pDX, IDC_LeftHip25, mLeftHip[1]);
    DDX_Text(pDX, IDC_LeftHip50, mLeftHip[2]);
	DDX_Text(pDX, IDC_LeftHumRot5, mLeftHumRot[0]);
	DDX_Text(pDX, IDC_LeftHumRot25, mLeftHumRot[1]);
    DDX_Text(pDX, IDC_LeftHumRot50, mLeftHumRot[2]);
	DDX_Text(pDX, IDC_LeftKnee5, mLeftKnee[0]);
	DDX_Text(pDX, IDC_LeftKnee25, mLeftKnee[1]);
    DDX_Text(pDX, IDC_LeftKnee50, mLeftKnee[2]);
	DDX_Text(pDX, IDC_NeckLatBend5, mNeckLatBend[0]);
	DDX_Text(pDX, IDC_NeckLatBend25, mNeckLatBend[1]);
    DDX_Text(pDX, IDC_NeckLatBend50, mNeckLatBend[2]);
	DDX_Text(pDX, IDC_LeftLatBend5, mLeftLatBend[0]);
	DDX_Text(pDX, IDC_LeftLatBend25, mLeftLatBend[1]);
    DDX_Text(pDX, IDC_LeftLatBend50, mLeftLatBend[2]);
	DDX_Text(pDX, IDC_NeckRotation5, mNeckRotation[0]);
	DDX_Text(pDX, IDC_NeckRotation25, mNeckRotation[1]);
    DDX_Text(pDX, IDC_NeckRotation50, mNeckRotation[2]);
	DDX_Text(pDX, IDC_LeftRotation5, mLeftRotation[0]);
	DDX_Text(pDX, IDC_LeftRotation25, mLeftRotation[1]);
    DDX_Text(pDX, IDC_LeftRotation50, mLeftRotation[2]);
	DDX_Text(pDX, IDC_LeftRotBkFd5, mLeftRotBkFd[0]);
	DDX_Text(pDX, IDC_LeftRotBkFd25, mLeftRotBkFd[1]);
    DDX_Text(pDX, IDC_LeftRotBkFd50, mLeftRotBkFd[2]);

	DDX_Text(pDX, IDC_NeckFlexExt6, mNeckFlexExtDC[0]);
	DDX_Text(pDX, IDC_NeckFlexExt26, mNeckFlexExtDC[1]);
    DDX_Text(pDX, IDC_NeckFlexExt51, mNeckFlexExtDC[2]);
	DDX_Text(pDX, IDC_LeftFlexExt6, mLeftFlexExtDC[0]);
	DDX_Text(pDX, IDC_LeftFlexExt26, mLeftFlexExtDC[1]);
    DDX_Text(pDX, IDC_LeftFlexExt51, mLeftFlexExtDC[2]);
	DDX_Text(pDX, IDC_LeftHip6, mLeftHipDC[0]);
	DDX_Text(pDX, IDC_LeftHip26, mLeftHipDC[1]);
    DDX_Text(pDX, IDC_LeftHip51, mLeftHipDC[2]);
	DDX_Text(pDX, IDC_LeftHumRot6, mLeftHumRotDC[0]);
	DDX_Text(pDX, IDC_LeftHumRot26, mLeftHumRotDC[1]);
    DDX_Text(pDX, IDC_LeftHumRot51, mLeftHumRotDC[2]);
	DDX_Text(pDX, IDC_LeftKnee6, mLeftKneeDC[0]);
	DDX_Text(pDX, IDC_LeftKnee26, mLeftKneeDC[1]);
    DDX_Text(pDX, IDC_LeftKnee51, mLeftKneeDC[2]);
	DDX_Text(pDX, IDC_NeckLatBend6, mNeckLatBendDC[0]);
	DDX_Text(pDX, IDC_NeckLatBend26, mNeckLatBendDC[1]);
    DDX_Text(pDX, IDC_NeckLatBend51, mNeckLatBendDC[2]);
	DDX_Text(pDX, IDC_LeftLatBend6, mLeftLatBendDC[0]);
	DDX_Text(pDX, IDC_LeftLatBend26, mLeftLatBendDC[1]);
    DDX_Text(pDX, IDC_LeftLatBend51, mLeftLatBendDC[2]);
	DDX_Text(pDX, IDC_NeckRotation6, mNeckRotationDC[0]);
	DDX_Text(pDX, IDC_NeckRotation26, mNeckRotationDC[1]);
    DDX_Text(pDX, IDC_NeckRotation51, mNeckRotationDC[2]);
	DDX_Text(pDX, IDC_LeftRotation6, mLeftRotationDC[0]);
	DDX_Text(pDX, IDC_LeftRotation26, mLeftRotationDC[1]);
    DDX_Text(pDX, IDC_LeftRotation51, mLeftRotationDC[2]);
	DDX_Text(pDX, IDC_LeftRotBkFd6, mLeftRotBkFdDC[0]);
	DDX_Text(pDX, IDC_LeftRotBkFd26, mLeftRotBkFdDC[1]);
    DDX_Text(pDX, IDC_LeftRotBkFd51, mLeftRotBkFdDC[2]);

	DDX_Text(pDX, IDC_NeckFlexExt7, mNeckFlexExtMT [0]);
	DDX_Text(pDX, IDC_NeckFlexExt27, mNeckFlexExtMT[1]);
    DDX_Text(pDX, IDC_NeckFlexExt52, mNeckFlexExtMT[2]);
	DDX_Text(pDX, IDC_LeftFlexExt7, mLeftFlexExtMT[0]);
	DDX_Text(pDX, IDC_LeftFlexExt27, mLeftFlexExtMT[1]);
    DDX_Text(pDX, IDC_LeftFlexExt52, mLeftFlexExtMT[2]);
	DDX_Text(pDX, IDC_LeftHip7, mLeftHipMT[0]);
	DDX_Text(pDX, IDC_LeftHip27, mLeftHipMT[1]);
    DDX_Text(pDX, IDC_LeftHip52, mLeftHipMT[2]);
	DDX_Text(pDX, IDC_LeftHumRot7, mLeftHumRotMT[0]);
	DDX_Text(pDX, IDC_LeftHumRot27, mLeftHumRotMT[1]);
    DDX_Text(pDX, IDC_LeftHumRot52, mLeftHumRotMT[2]);
	DDX_Text(pDX, IDC_LeftKnee7, mLeftKneeMT[0]);
	DDX_Text(pDX, IDC_LeftKnee27, mLeftKneeMT[1]);
    DDX_Text(pDX, IDC_LeftKnee52, mLeftKneeMT[2]);
	DDX_Text(pDX, IDC_NeckLatBend7, mNeckLatBendMT[0]);
	DDX_Text(pDX, IDC_NeckLatBend27, mNeckLatBendMT[1]);
    DDX_Text(pDX, IDC_NeckLatBend52, mNeckLatBendMT[2]);
	DDX_Text(pDX, IDC_LeftLatBend7, mLeftLatBendMT[0]);
	DDX_Text(pDX, IDC_LeftLatBend27, mLeftLatBendMT[1]);
    DDX_Text(pDX, IDC_LeftLatBend52, mLeftLatBendMT[2]);
	DDX_Text(pDX, IDC_NeckRotation7, mNeckRotationMT[0]);
	DDX_Text(pDX, IDC_NeckRotation27, mNeckRotationMT[1]);
    DDX_Text(pDX, IDC_NeckRotation52, mNeckRotationMT[2]);
	DDX_Text(pDX, IDC_LeftRotation7, mLeftRotationMT[0]);
	DDX_Text(pDX, IDC_LeftRotation27, mLeftRotationMT[1]);
    DDX_Text(pDX, IDC_LeftRotation52, mLeftRotationMT[2]);
	DDX_Text(pDX, IDC_LeftRotBkFd7, mLeftRotBkFdMT[0]);
	DDX_Text(pDX, IDC_LeftRotBkFd27, mLeftRotBkFdMT[1]);
    DDX_Text(pDX, IDC_LeftRotBkFd52, mLeftRotBkFdMT[2]);


	DDX_Text(pDX, IDC_RightForearmRot5, mRightForearmRot[0]);
	DDX_Text(pDX, IDC_RightForearmRot25, mRightForearmRot[1]);
	DDX_Text(pDX, IDC_RightForearmRot50, mRightForearmRot[2]);
	DDX_Text(pDX, IDC_RightWristFlex5, mRightWristFlex[0]);
	DDX_Text(pDX, IDC_RightWristFlex25, mRightWristFlex[1]);
	DDX_Text(pDX, IDC_RightWristFlex50, mRightWristFlex[2]);
	DDX_Text(pDX, IDC_RightWristDev5, mRightWristDev[0]);
	DDX_Text(pDX, IDC_RightWristDev25, mRightWristDev[1]);
	DDX_Text(pDX, IDC_RightWristDev50, mRightWristDev[2]);
	DDX_Text(pDX, IDC_RightAbAd5, mRightAbAd[0]);
	DDX_Text(pDX, IDC_RightAbAd25, mRightAbAd[1]);
    DDX_Text(pDX, IDC_RightAbAd50, mRightAbAd[2]);
	DDX_Text(pDX, IDC_RightAnkle5, mRightAnkle[0]);
	DDX_Text(pDX, IDC_RightAnkle25, mRightAnkle[1]);
    DDX_Text(pDX, IDC_RightAnkle50, mRightAnkle[2]);
	DDX_Text(pDX, IDC_RightElbow5, mRightElbow[0]);
	DDX_Text(pDX, IDC_RightElbow25, mRightElbow[1]);
    DDX_Text(pDX, IDC_RightElbow50, mRightElbow[2]);
	DDX_Text(pDX, IDC_RightHip5, mRightHip[0]);
	DDX_Text(pDX, IDC_RightHip25, mRightHip[1]);
    DDX_Text(pDX, IDC_RightHip50, mRightHip[2]);
	DDX_Text(pDX, IDC_RightHumRot5, mRightHumRot[0]);
	DDX_Text(pDX, IDC_RightHumRot25, mRightHumRot[1]);
    DDX_Text(pDX, IDC_RightHumRot50, mRightHumRot[2]);
	DDX_Text(pDX, IDC_RightKnee5, mRightKnee[0]);
	DDX_Text(pDX, IDC_RightKnee25, mRightKnee[1]);
    DDX_Text(pDX, IDC_RightKnee50, mRightKnee[2]);
	DDX_Text(pDX, IDC_RightRotBkFd5, mRightRotBkFd[0]);
	DDX_Text(pDX, IDC_RightRotBkFd25, mRightRotBkFd[1]);
    DDX_Text(pDX, IDC_RightRotBkFd50, mRightRotBkFd[2]);

	DDX_Text(pDX, IDC_RightForearmRot6, mRightForearmRotDC[0]);
	DDX_Text(pDX, IDC_RightForearmRot26, mRightForearmRotDC[1]);
	DDX_Text(pDX, IDC_RightForearmRot51, mRightForearmRotDC[2]);
	DDX_Text(pDX, IDC_RightWristFlex6, mRightWristFlexDC[0]);
	DDX_Text(pDX, IDC_RightWristFlex26, mRightWristFlexDC[1]);
	DDX_Text(pDX, IDC_RightWristFlex51, mRightWristFlexDC[2]);
	DDX_Text(pDX, IDC_RightWristDev6, mRightWristDevDC[0]);
	DDX_Text(pDX, IDC_RightWristDev26, mRightWristDevDC[1]);
	DDX_Text(pDX, IDC_RightWristDev51, mRightWristDevDC[2]);
	DDX_Text(pDX, IDC_RightAbAd6, mRightAbAdDC[0]);
	DDX_Text(pDX, IDC_RightAbAd26, mRightAbAdDC[1]);
    DDX_Text(pDX, IDC_RightAbAd51, mRightAbAdDC[2]);
	DDX_Text(pDX, IDC_RightAnkle6, mRightAnkleDC[0]);
	DDX_Text(pDX, IDC_RightAnkle26, mRightAnkleDC[1]);
    DDX_Text(pDX, IDC_RightAnkle51, mRightAnkleDC[2]);
	DDX_Text(pDX, IDC_RightElbow6, mRightElbowDC[0]);
	DDX_Text(pDX, IDC_RightElbow26, mRightElbowDC[1]);
    DDX_Text(pDX, IDC_RightElbow51, mRightElbowDC[2]);
	DDX_Text(pDX, IDC_RightHip6, mRightHipDC[0]);
	DDX_Text(pDX, IDC_RightHip26, mRightHipDC[1]);
    DDX_Text(pDX, IDC_RightHip51, mRightHipDC[2]);
	DDX_Text(pDX, IDC_RightHumRot6, mRightHumRotDC[0]);
	DDX_Text(pDX, IDC_RightHumRot26, mRightHumRotDC[1]);
    DDX_Text(pDX, IDC_RightHumRot51, mRightHumRotDC[2]);
	DDX_Text(pDX, IDC_RightKnee6, mRightKneeDC[0]);
	DDX_Text(pDX, IDC_RightKnee26, mRightKneeDC[1]);
    DDX_Text(pDX, IDC_RightKnee51, mRightKneeDC[2]);
	DDX_Text(pDX, IDC_RightRotBkFd6, mRightRotBkFdDC[0]);
	DDX_Text(pDX, IDC_RightRotBkFd26, mRightRotBkFdDC[1]);
    DDX_Text(pDX, IDC_RightRotBkFd51, mRightRotBkFdDC[2]);


	DDX_Text(pDX, IDC_RightForearmRot7, mRightForearmRotMT[0]);
	DDX_Text(pDX, IDC_RightForearmRot27, mRightForearmRotMT[1]);
	DDX_Text(pDX, IDC_RightForearmRot52, mRightForearmRotMT[2]);
	DDX_Text(pDX, IDC_RightWristFlex7, mRightWristFlexMT[0]);
	DDX_Text(pDX, IDC_RightWristFlex27, mRightWristFlexMT[1]);
	DDX_Text(pDX, IDC_RightWristFlex52, mRightWristFlexMT[2]);
	DDX_Text(pDX, IDC_RightWristDev7, mRightWristDevMT[0]);
	DDX_Text(pDX, IDC_RightWristDev27, mRightWristDevMT[1]);
	DDX_Text(pDX, IDC_RightWristDev52, mRightWristDevMT[2]);
	DDX_Text(pDX, IDC_RightAbAd7, mRightAbAdMT[0]);
	DDX_Text(pDX, IDC_RightAbAd27, mRightAbAdMT[1]);
    DDX_Text(pDX, IDC_RightAbAd52, mRightAbAdMT[2]);
	DDX_Text(pDX, IDC_RightAnkle7, mRightAnkleMT[0]);
	DDX_Text(pDX, IDC_RightAnkle27, mRightAnkleMT[1]);
    DDX_Text(pDX, IDC_RightAnkle52, mRightAnkleMT[2]);
	DDX_Text(pDX, IDC_RightElbow7, mRightElbowMT[0]);
	DDX_Text(pDX, IDC_RightElbow27, mRightElbowMT[1]);
    DDX_Text(pDX, IDC_RightElbow52, mRightElbowMT[2]);
	DDX_Text(pDX, IDC_RightHip7, mRightHipMT[0]);
	DDX_Text(pDX, IDC_RightHip27, mRightHipMT[1]);
    DDX_Text(pDX, IDC_RightHip52, mRightHipMT[2]);
	DDX_Text(pDX, IDC_RightHumRot7, mRightHumRotMT[0]);
	DDX_Text(pDX, IDC_RightHumRot27, mRightHumRotMT[1]);
    DDX_Text(pDX, IDC_RightHumRot52, mRightHumRotMT[2]);
	DDX_Text(pDX, IDC_RightKnee7, mRightKneeMT[0]);
	DDX_Text(pDX, IDC_RightKnee27, mRightKneeMT[1]);
    DDX_Text(pDX, IDC_RightKnee52, mRightKneeMT[2]);
	DDX_Text(pDX, IDC_RightRotBkFd7, mRightRotBkFdMT[0]);
	DDX_Text(pDX, IDC_RightRotBkFd27, mRightRotBkFdMT[1]);
    DDX_Text(pDX, IDC_RightRotBkFd52, mRightRotBkFdMT[2]);

	DDX_Text(pDX, IDC_HandForceUnits, mHandForceUnits);
	DDX_Text(pDX, IDC_HAND_FORCE_TXT_LVAL2, mHandForceTxtLVal);
	DDX_Text(pDX, IDC_HAND_FORCE_TXT_RVAL2, mHandForceTxtRVal);
	DDX_Text(pDX, IDC_UnitsLabel, mUnitsLabel);
	DDX_Text(pDX, IDC_RightHori, mRightHori);
	DDX_Text(pDX, IDC_RightLat2, mRightLat);
	DDX_Text(pDX, IDC_RightVert2, mRightVert);
	DDX_Text(pDX, IDC_LeftHori, mLeftHori);
	DDX_Text(pDX, IDC_LeftLat2, mLeftLat);
	DDX_Text(pDX, IDC_LeftVert2, mLeftVert);
	//DDX_Text(pDX, IDC_EDC, mEDC);
	//DDV_MinMaxUInt(pDX, mEDC, 0, 100000);
	//}}AFX_DATA_MAP

	DDX_Text(pDX, IDC_Report_TCT, mTCT);
	DDV_MinMaxUInt(pDX, mTCT, 0, 100000);
	DDX_Text(pDX, IDC_Report_EC, mEC);
	DDV_MinMaxUInt(pDX, mEC, 0, 100000);
	DDX_Text(pDX, IDC_Report_ED, mED);
	DDV_MinMaxUInt(pDX, mED, 0, 100000);
	DDX_Text(pDX, IDC_Report_EDC, mEDC_text);
}

BEGIN_MESSAGE_MAP(ReportFatigue, ReportView)
	//{{AFX_MSG_MAP(ReportFatigue)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
	//}}AFX_MSG_MAP
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

#ifdef _DEBUG
void ReportFatigue::AssertValid() const { CFormView::AssertValid(); }
void ReportFatigue::Dump(CDumpContext& dc) const { CFormView::Dump(dc); }
#endif //_DEBUG

//void ReportFatigue::OnFilePrintPreview() { SSPPPrintPreview(this); }

void ReportFatigue::OnUpdate(CView* aSenderPtr,LPARAM aHint,CObject* aHintPtr)
{
    C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();
    //Analysis &lResults = lDocPtr->getResultsRef();

	//exertion report
	mTCT = lDocPtr->EDC_Value[2];
	mEC = lDocPtr->EDC_Value[0];
	mED = lDocPtr->EDC_Value[1];
	mEDC = lDocPtr->EDC_Value[3];
	
	mEDC_text.Format("%.1f", mEDC);



    // Retrieve fatigue analysis results
    //FatigueData data;
    //lResults.getFatigue(&data);
	const FatigueData_s& data = lDocPtr->GetSkeleton()->getFatigueData();

    // Write results to the form controls, converting to the desired system of units in the process
    /*CString *reported[] = {mLeftForearmRot, mRightForearmRot, mLeftWristFlex, mRightWristFlex, mLeftWristDev, mRightWristDev,
						   mLeftElbow,mRightElbow,mLeftHumRot,mRightHumRot,mLeftRotBkFd,mRightRotBkFd,mLeftAbAd,mRightAbAd,
                           mLeftFlexExt,mLeftLatBend,mLeftRotation,mLeftHip,mRightHip,mLeftKnee,mRightKnee,mLeftAnkle,mRightAnkle};*/
	const double* arrays[] = {data.FifthPercentile, data.FiftiethPercentile, data.NinetyFifthPercentile};

	const double* arraysDC[] = {data.FifthPercentileDC, data.FiftiethPercentileDC, data.NinetyFifthPercentileDC};

	const double* arraysMT[] = {data.FifthPercentileMT, data.FiftiethPercentileMT, data.NinetyFifthPercentileMT};
	for(int i = 0; i < NUM_STRENGTH_HINGES; ++i) {
		for(int j = 0; j < 3; ++j) {
            //reported[i][j].Format("%.0f",*((double*)&data + 3*i + j));
			if (arrays[j][i] < 1)
			{
				hingeToMember[i][j] = _T("-");
				hingeToMemberDC[i][j] = _T("-");
				hingeToMemberMT[i][j] = _T("-");
				hingeToMember[i][0] = _T("0"); //  make 0 in %MVC colomn visible
			}
			else
			{
			hingeToMember[i][j].Format("%.0f", arrays[j][i]);
			hingeToMemberDC[i][j].Format("%.1f", arraysDC[j][i]); 
			hingeToMemberMT[i][j].Format("%.0f", arraysMT[j][i]);
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

	InvalidateRect(0);
}

HBRUSH ReportFatigue::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
    	
    C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();
     
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
   // pDC->SetBkColor(lDocPtr->getBalColor());

	int ID = pWnd -> GetDlgCtrlID();

	int DC_ID_List_L1[16] = {IDC_LeftWristFlex6, IDC_LeftWristDev6, IDC_LeftForearmRot6,
							IDC_LeftElbow6, IDC_LeftHumRot6, IDC_LeftRotBkFd6,  IDC_LeftAbAd6,
							IDC_NeckFlexExt6, IDC_NeckLatBend6, IDC_NeckRotation6,
							IDC_LeftFlexExt6, IDC_LeftLatBend6, IDC_LeftRotation6,
							IDC_LeftHip6, IDC_LeftKnee6, IDC_LeftAnkle6};
	int DC_ID_List_L2[16] = {IDC_LeftWristFlex26, IDC_LeftWristDev26, IDC_LeftForearmRot26,
							IDC_LeftElbow26, IDC_LeftHumRot26, IDC_LeftRotBkFd26,  IDC_LeftAbAd26,
							IDC_NeckFlexExt26, IDC_NeckLatBend26, IDC_NeckRotation26,
							IDC_LeftFlexExt26, IDC_LeftLatBend26, IDC_LeftRotation26,
							IDC_LeftHip26, IDC_LeftKnee26, IDC_LeftAnkle26
	};

	int DC_ID_List_L3[16] = {IDC_LeftWristFlex51, IDC_LeftWristDev51, IDC_LeftForearmRot51,
							IDC_LeftElbow51, IDC_LeftHumRot51, IDC_LeftRotBkFd51,  IDC_LeftAbAd51,
							IDC_NeckFlexExt51, IDC_NeckLatBend51, IDC_NeckRotation51,
							IDC_LeftFlexExt51, IDC_LeftLatBend51, IDC_LeftRotation51,
							IDC_LeftHip51, IDC_LeftKnee51, IDC_LeftAnkle51
	};
	int DC_ID_List_R1[16] = {IDC_RightWristFlex6, IDC_RightWristDev6, IDC_RightForearmRot6,
							IDC_RightElbow6, IDC_RightHumRot6, IDC_RightRotBkFd6,  IDC_RightAbAd6,
							IDC_NeckFlexExt6, IDC_NeckLatBend6, IDC_NeckRotation6,
							IDC_RightHip6, IDC_RightKnee6, IDC_RightAnkle6};
	int DC_ID_List_R2[16] = {IDC_RightWristFlex26, IDC_RightWristDev26, IDC_RightForearmRot26,
							IDC_RightElbow26, IDC_RightHumRot26, IDC_RightRotBkFd26,  IDC_RightAbAd26,
							IDC_NeckFlexExt26, IDC_NeckLatBend26, IDC_NeckRotation26,
							IDC_RightHip26, IDC_RightKnee26, IDC_RightAnkle26
	};

	int DC_ID_List_R3[16] = {IDC_RightWristFlex51, IDC_RightWristDev51, IDC_RightForearmRot51,
							IDC_RightElbow51, IDC_RightHumRot51, IDC_RightRotBkFd51,  IDC_RightAbAd51,
							IDC_NeckFlexExt51, IDC_NeckLatBend51, IDC_NeckRotation51,
							IDC_RightHip51, IDC_RightKnee51, IDC_RightAnkle51
	};

	for (int i = 0; i < 16; ++i) {

		if (ID == DC_ID_List_L1[i] || ID == DC_ID_List_L2[i] || ID == DC_ID_List_L3[i] || ID == DC_ID_List_R1[i] || ID == DC_ID_List_R2[i] || ID == DC_ID_List_R3[i])
   {
	   switch (nCtlColor){
		 case CTLCOLOR_STATIC:
	   {
		double result =GetDlgItemInt(ID);
		if(result == 0){
			continue;
		}
		UpdateData(false);
		if(mEDC != 0 &&  mEDC > result)
		{
			pDC->SetTextColor(RGB(255, 0, 0));// Set color to Red
			pDC->SetBkMode(OPAQUE);
			//pDC->SetBkColor(lDocPtr->getBalColor());
			return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		//else if(mEDC != 0 &&  mEDC >= result * 0.95 && mEDC < result)
		//{
		//	pDC->SetTextColor(RGB(255,255,0));// Set color to green
		//	pDC->SetBkMode(OPAQUE);
		//	//pDC->SetBkColor(lDocPtr->getBalColor());
		//	return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		//}
		else{return (HBRUSH)::GetStockObject(NULL_BRUSH);}
	   }
		 default:	return ReportView::OnCtlColor(pDC, pWnd, nCtlColor);
		 }
	   }
	}

	int ED_ID_List_L1[16] = {IDC_LeftWristFlex8, IDC_LeftWristDev7, IDC_LeftForearmRot7,
							IDC_LeftElbow7, IDC_LeftHumRot7, IDC_LeftRotBkFd7,  IDC_LeftAbAd7,
							IDC_NeckFlexExt7, IDC_NeckLatBend7, IDC_NeckRotation7,
							IDC_LeftFlexExt7, IDC_LeftLatBend7, IDC_LeftRotation7,
							IDC_LeftHip7, IDC_LeftKnee7, IDC_LeftAnkle7};
	int ED_ID_List_L2[16] = {IDC_LeftWristFlex27, IDC_LeftWristDev27, IDC_LeftForearmRot27,
							IDC_LeftElbow27, IDC_LeftHumRot27, IDC_LeftRotBkFd27,  IDC_LeftAbAd27,
							IDC_NeckFlexExt27, IDC_NeckLatBend27, IDC_NeckRotation27,
							IDC_LeftFlexExt27, IDC_LeftLatBend27, IDC_LeftRotation27,
							IDC_LeftHip27, IDC_LeftKnee27, IDC_LeftAnkle27
	};

	int ED_ID_List_L3[16] = {IDC_LeftWristFlex52, IDC_LeftWristDev52, IDC_LeftForearmRot52,
							IDC_LeftElbow52, IDC_LeftHumRot52, IDC_LeftRotBkFd52,  IDC_LeftAbAd52,
							IDC_NeckFlexExt52, IDC_NeckLatBend52, IDC_NeckRotation52,
							IDC_LeftFlexExt52, IDC_LeftLatBend52, IDC_LeftRotation52,
							IDC_LeftHip52, IDC_LeftKnee52, IDC_LeftAnkle52
	};
	int ED_ID_List_R1[16] = {IDC_RightWristFlex7, IDC_RightWristDev7, IDC_RightForearmRot7,
							IDC_RightElbow7, IDC_RightHumRot7, IDC_RightRotBkFd7,  IDC_RightAbAd7,
							IDC_NeckFlexExt7, IDC_NeckLatBend7, IDC_NeckRotation7,
							IDC_RightHip7, IDC_RightKnee7, IDC_RightAnkle7};
	int ED_ID_List_R2[16] = {IDC_RightWristFlex27, IDC_RightWristDev27, IDC_RightForearmRot27,
							IDC_RightElbow27, IDC_RightHumRot27, IDC_RightRotBkFd27,  IDC_RightAbAd27,
							IDC_NeckFlexExt27, IDC_NeckLatBend27, IDC_NeckRotation27,
							IDC_RightHip27, IDC_RightKnee27, IDC_RightAnkle27
	};

	int ED_ID_List_R3[16] = {IDC_RightWristFlex52, IDC_RightWristDev52, IDC_RightForearmRot52,
							IDC_RightElbow52, IDC_RightHumRot52, IDC_RightRotBkFd52,  IDC_RightAbAd52,
							IDC_NeckFlexExt52, IDC_NeckLatBend52, IDC_NeckRotation52,
							IDC_RightHip52, IDC_RightKnee52, IDC_RightAnkle52
	};

	for (int i = 0; i < 16; ++i) {

		if (ID == ED_ID_List_L1[i] || ID == ED_ID_List_L2[i] || ID == ED_ID_List_L3[i] || ID == ED_ID_List_R1[i] || ID == ED_ID_List_R2[i] || ID == ED_ID_List_R3[i])
   {
	   switch (nCtlColor){
		 case CTLCOLOR_STATIC:
	   {
		double result =GetDlgItemInt(ID);
		if(result == 0){
			continue;
		}
		UpdateData(false);
		if(mED != 0 &&  mED > result)
		{
			pDC->SetTextColor(RGB(255, 0, 0));// Set color to Red
			pDC->SetBkMode(OPAQUE);
			//pDC->SetBkColor(lDocPtr->getBalColor());
			return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		}
		//else if(mED != 0 &&  mED >= result * 0.95 && mED < result)
		//{
		//	pDC->SetTextColor(RGB(255,255,0));// Set color to green
		//	pDC->SetBkMode(OPAQUE);
		//	//pDC->SetBkColor(lDocPtr->getBalColor());
		//	return (HBRUSH)::GetStockObject(NULL_BRUSH); 
		//}
		else{return (HBRUSH)::GetStockObject(NULL_BRUSH);}
	   }
		 default:	return ReportView::OnCtlColor(pDC, pWnd, nCtlColor);
		 }
	   }
	}

    return (HBRUSH)(CBrush(lDocPtr->getBalColor()));
}

void ReportFatigue::OnDisplayDialogcolor() 
{
	C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();
	lDocPtr->setReportColor();
	
	Invalidate(true);
}

void ReportFatigue::OnInitialUpdate()
{
	ReportView::OnInitialUpdate();

	// Set the hand force box units
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();
	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";
	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}

void ReportFatigue::UpdateUnits() {
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";

	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";

	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}


