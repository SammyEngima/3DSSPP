// ReportShoulderGeometry.cpp : implementation file
//

#include "ReportShoulderGeometry.h"

#include "hom.h"
#include "hom_doc.h"	    // Necessary
#include "skeleton.hpp"
#include "ShoulderModel.h"
#include "Units.hpp"		// Added for UM_M_CM
#include "enums.h"          //For positioning
#include <cstdlib>
#if defined(SHOULDERDLL)
#include "libfmincon.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ReportShoulderGeometry

ReportView * ReportShoulderGeometry::cPointer = NULL;
bool ReportShoulderGeometry::library_loaded = false;

IMPLEMENT_DYNCREATE(ReportShoulderGeometry, ReportView)

//---------------------------------------------------------------------------
   ReportShoulderGeometry::
ReportShoulderGeometry( void )
:  ReportView( ReportShoulderGeometry::IDD )
{
	//{{AFX_DATA_INIT(ReportJointLocations)
	mFooter = _T("--");
	mHeader = _T("--");
	//}}AFX_DATA_INIT
   for (int i = 0; i < 3; i++)
   {
		m1stThoracicVertebraS[i] = _T("0.00");
		m12thThoracicVertebraI[i] = _T("0.00");
		m1stThoracicVertebraM[i] = _T("0.00");

		mLeftSternoclavicularJoint[i] = _T("0.00");
		mRightSternoclavicularJoint[i] = _T("0.00");
		mLeftACJ[i] = _T("0.00");
		mRightACJ[i] = _T("0.00");
		mLeftScapulaAngle[i] = _T("0.00");
		mRightScapulaAngle[i] = _T("0.00");
		mLeftScapularSpine[i] = _T("0.00");
		mRightScapularSpine[i] = _T("0.00");
		mLeftHumeralHead[i] = _T("0.00");
		mRightHumeralHead[i] = _T("0.00");
		mLeftElbowJoint[i] = _T("0.00");
		mRightElbowJoint[i] = _T("0.00");
		mLeftWristJoint[i] = _T("0.00");
		mRightWristJoint[i] = _T("0.00");
	    mLeftSJCTJoint[i] = _T("0.00");
	    mRightSJCTJoint[i] = _T("0.00");
	    mLeftACRTJoint[i] = _T("0.00");
	    mRightACRTJoint[i] = _T("0.00");
	    mLeftC7T1Joint[i] = _T("0.00");
	    mRightC7T1Joint[i] = _T("0.00");
	    mLeftL5S1Joint[i] = _T("0.00");
	    mRightL5S1Joint[i] = _T("0.00");
   }

	mLocationUnits = _T("");
	mHandForceUnits = _T("");
	mUnitsLabel = _T("");
	mLeftHori = _T("");

	cPointer = this;

	hingeToMember[SG_FirstThoracicVertebraS] = m1stThoracicVertebraS;
	hingeToMember[SG_TwelvthThoracicVertebraI] = m12thThoracicVertebraI;
	hingeToMember[SG_FirstThoracicVertebraM] = m1stThoracicVertebraM;
	hingeToMember[SG_RightSternoclavicularJoint] = mRightSternoclavicularJoint;
	hingeToMember[SG_RightACJ] = mRightACJ;
	hingeToMember[SG_RightScapulaAngle] = mRightScapulaAngle;
	hingeToMember[SG_RightScapularSpine] = mRightScapularSpine;
	hingeToMember[SG_RightHumeralHead] = mRightHumeralHead;
	hingeToMember[SG_RightElbowJoint] = mRightElbowJoint;
	hingeToMember[SG_RightWristJoint] = mRightWristJoint;
	hingeToMember[SG_RightSJCTJoint] = mRightSJCTJoint;
	hingeToMember[SG_RightACRTJoint] = mRightACRTJoint;
	hingeToMember[SG_RightC7T1Joint] = mRightC7T1Joint;
	hingeToMember[SG_RightL5S1Joint] = mRightL5S1Joint;
	hingeToMember[SG_LeftSternoclavicularJoint] = mLeftSternoclavicularJoint;
	hingeToMember[SG_LeftACJ] = mLeftACJ;
	hingeToMember[SG_LeftScapulaAngle] = mLeftScapulaAngle;
	hingeToMember[SG_LeftScapularSpine] = mLeftScapularSpine;
	hingeToMember[SG_LeftHumeralHead] = mLeftHumeralHead;
	hingeToMember[SG_LeftElbowJoint] = mLeftElbowJoint;
	hingeToMember[SG_LeftWristJoint] = mLeftWristJoint;
	hingeToMember[SG_LeftSJCTJoint] = mLeftSJCTJoint;
	hingeToMember[SG_LeftACRTJoint] = mLeftACRTJoint;
	hingeToMember[SG_LeftC7T1Joint] = mLeftC7T1Joint;
	hingeToMember[SG_LeftL5S1Joint] = mLeftL5S1Joint;
}

//---------------------------------------------------------------------------
   ReportShoulderGeometry::
~ReportShoulderGeometry( void )
{
   cPointer = NULL;
   return;
}
void ReportShoulderGeometry::killMatlab() {

#if defined(SHOULDERDLL)
	if(library_loaded) {
		libfminconTerminate();
		mclTerminateApplication();
		library_loaded = false;
	}

	ShoulderModel::geoMod_library_loaded = ReportShoulderGeometry::library_loaded;
#endif
}
//---------------------------------------------------------------------------
   CString
   ReportShoulderGeometry::
ReportName
( void ) 
const
{
   return   "3DSSPP - Shoulder Geometry Report";
}

//---------------------------------------------------------------------------
void ReportShoulderGeometry::DoDataExchange(CDataExchange* pDX)
{
	ReportView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ReportJointLocations)
    //--- Joint Locations ---
	DDX_Text(pDX, IDC_Footer, mFooter);
	DDX_Text(pDX, IDC_Header, mHeader);
	DDX_Text(pDX, IDC_1stThoracicVertebraSLocX, m1stThoracicVertebraS[0]);
	DDX_Text(pDX, IDC_1stThoracicVertebraSLocY, m1stThoracicVertebraS[1]);
	DDX_Text(pDX, IDC_1stThoracicVertebraSLocZ, m1stThoracicVertebraS[2]);
	DDX_Text(pDX, IDC_12thThoracicVertebraILocX, m12thThoracicVertebraI[0]);
	DDX_Text(pDX, IDC_12thThoracicVertebraILocY, m12thThoracicVertebraI[1]);
	DDX_Text(pDX, IDC_12thThoracicVertebraILocZ, m12thThoracicVertebraI[2]);
	DDX_Text(pDX, IDC_LeftSternoclavicularJointLocX, mLeftSternoclavicularJoint[0]);
	DDX_Text(pDX, IDC_LeftSternoclavicularJointLocY, mLeftSternoclavicularJoint[1]);
	DDX_Text(pDX, IDC_LeftSternoclavicularJointLocZ, mLeftSternoclavicularJoint[2]);
	DDX_Text(pDX, IDC_RightSternoclavicularJointLocX, mRightSternoclavicularJoint[0]);
	DDX_Text(pDX, IDC_RightSternoclavicularJointLocY, mRightSternoclavicularJoint[1]);
	DDX_Text(pDX, IDC_RightSternoclavicularJointLocZ, mRightSternoclavicularJoint[2]);
	DDX_Text(pDX, IDC_1stThoracicVertebraMLocX, m1stThoracicVertebraM[0]);
	DDX_Text(pDX, IDC_1stThoracicVertebraMLocY, m1stThoracicVertebraM[1]);
	DDX_Text(pDX, IDC_1stThoracicVertebraMLocZ, m1stThoracicVertebraM[2]);
	DDX_Text(pDX, IDC_LeftAcromioclavicularJointLocX, mLeftACJ[0]);
	DDX_Text(pDX, IDC_LeftAcromioclavicularJointLocY, mLeftACJ[1]);
	DDX_Text(pDX, IDC_LeftAcromioclavicularJointLocZ, mLeftACJ[2]);
	DDX_Text(pDX, IDC_RightAcromioclavicularJointLocX, mRightACJ[0]);
	DDX_Text(pDX, IDC_RightAcromioclavicularJointLocY, mRightACJ[1]);
	DDX_Text(pDX, IDC_RightAcromioclavicularJointLocZ, mRightACJ[2]);
	DDX_Text(pDX, IDC_LeftScapulaAngleLocX, mLeftScapulaAngle[0]);
	DDX_Text(pDX, IDC_LeftScapulaAngleLocY, mLeftScapulaAngle[1]);
	DDX_Text(pDX, IDC_LeftScapulaAngleLocZ, mLeftScapulaAngle[2]);
	DDX_Text(pDX, IDC_RightScapulaAngleLocX, mRightScapulaAngle[0]);
	DDX_Text(pDX, IDC_RightScapulaAngleLocY, mRightScapulaAngle[1]);
	DDX_Text(pDX, IDC_RightScapulaAngleLocZ, mRightScapulaAngle[2]);
	DDX_Text(pDX, IDC_LeftScapularSpineLocX, mLeftScapularSpine[0]);
	DDX_Text(pDX, IDC_LeftScapularSpineLocY, mLeftScapularSpine[1]);
	DDX_Text(pDX, IDC_LeftScapularSpineLocZ, mLeftScapularSpine[2]);
	DDX_Text(pDX, IDC_RightScapularSpineLocX, mRightScapularSpine[0]);
	DDX_Text(pDX, IDC_RightScapularSpineLocY, mRightScapularSpine[1]);
	DDX_Text(pDX, IDC_RightScapularSpineLocZ, mRightScapularSpine[2]);
	DDX_Text(pDX, IDC_LeftHumeralHeadLocX, mLeftHumeralHead[0]);
	DDX_Text(pDX, IDC_LeftHumeralHeadLocY, mLeftHumeralHead[1]);
	DDX_Text(pDX, IDC_LeftHumeralHeadLocZ, mLeftHumeralHead[2]);
	DDX_Text(pDX, IDC_RightHumeralHeadLocX, mRightHumeralHead[0]);
	DDX_Text(pDX, IDC_RightHumeralHeadLocY, mRightHumeralHead[1]);
	DDX_Text(pDX, IDC_RightHumeralHeadLocZ, mRightHumeralHead[2]);
	DDX_Text(pDX, IDC_LeftElbowJointLocX, mLeftElbowJoint[0]);
	DDX_Text(pDX, IDC_LeftElbowJointLocY, mLeftElbowJoint[1]);
	DDX_Text(pDX, IDC_LeftElbowJointLocZ, mLeftElbowJoint[2]);
	DDX_Text(pDX, IDC_RightElbowJointLocX, mRightElbowJoint[0]);
	DDX_Text(pDX, IDC_RightElbowJointLocY, mRightElbowJoint[1]);
	DDX_Text(pDX, IDC_RightElbowJointLocZ, mRightElbowJoint[2]);
	DDX_Text(pDX, IDC_LeftWristJointLocX, mLeftWristJoint[0]);
	DDX_Text(pDX, IDC_LeftWristJointLocY, mLeftWristJoint[1]);
	DDX_Text(pDX, IDC_LeftWristJointLocZ, mLeftWristJoint[2]);
	DDX_Text(pDX, IDC_RightWristJointLocX, mRightWristJoint[0]);
	DDX_Text(pDX, IDC_RightWristJointLocY, mRightWristJoint[1]);
	DDX_Text(pDX, IDC_RightWristJointLocZ, mRightWristJoint[2]);
	DDX_Text(pDX, IDC_LeftSJCTLocX, mLeftSJCTJoint[0]);
	DDX_Text(pDX, IDC_LeftSJCTLocY, mLeftSJCTJoint[1]);
	DDX_Text(pDX, IDC_LeftSJCTLocZ, mLeftSJCTJoint[2]);
	DDX_Text(pDX, IDC_RightSJCTLocX, mRightSJCTJoint[0]);
	DDX_Text(pDX, IDC_RightSJCTLocY, mRightSJCTJoint[1]);
	DDX_Text(pDX, IDC_RightSJCTLocZ, mRightSJCTJoint[2]);
	DDX_Text(pDX, IDC_LeftACRTLocX, mLeftACRTJoint[0]);
	DDX_Text(pDX, IDC_LeftACRTLocY, mLeftACRTJoint[1]);
	DDX_Text(pDX, IDC_LeftACRTLocZ, mLeftACRTJoint[2]);
	DDX_Text(pDX, IDC_RightACRTLocX, mRightACRTJoint[0]);
	DDX_Text(pDX, IDC_RightACRTLocY, mRightACRTJoint[1]);
	DDX_Text(pDX, IDC_RightACRTLocZ, mRightACRTJoint[2]);
	DDX_Text(pDX, IDC_LeftC7T1LocX, mLeftC7T1Joint[0]);
	DDX_Text(pDX, IDC_LeftC7T1LocY, mLeftC7T1Joint[1]);
	DDX_Text(pDX, IDC_LeftC7T1LocZ, mLeftC7T1Joint[2]);
	DDX_Text(pDX, IDC_RightC7T1LocX, mRightC7T1Joint[0]);
	DDX_Text(pDX, IDC_RightC7T1LocY, mRightC7T1Joint[1]);
	DDX_Text(pDX, IDC_RightC7T1LocZ, mRightC7T1Joint[2]);
	DDX_Text(pDX, IDC_LeftL5S1LocX, mLeftL5S1Joint[0]);
	DDX_Text(pDX, IDC_LeftL5S1LocY, mLeftL5S1Joint[1]);
	DDX_Text(pDX, IDC_LeftL5S1LocZ, mLeftL5S1Joint[2]);
	DDX_Text(pDX, IDC_RightL5S1LocX, mRightL5S1Joint[0]);
	DDX_Text(pDX, IDC_RightL5S1LocY, mRightL5S1Joint[1]);
	DDX_Text(pDX, IDC_RightL5S1LocZ, mRightL5S1Joint[2]);

	DDX_Control(pDX, IDC_PROGRESS1, mProgressControl);

	DDX_Text(pDX, IDC_LocationUnits, mLocationUnits);
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

//---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(ReportShoulderGeometry, ReportView)
	//{{AFX_MSG_MAP(ReportShoulderGeometry)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
	//}}AFX_MSG_MAP
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
    //ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedSabtn)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ReportJointLocations diagnostics

#ifdef _DEBUG
void ReportShoulderGeometry::AssertValid() const
{
	CFormView::AssertValid();
}

void ReportShoulderGeometry::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ReportShoulderGeometry overrides

//void ReportShoulderGeometry::OnFilePrintPreview()
//{
//	SSPPPrintPreview(this);
//}

//---------------------------------------------------------------------------
void ReportShoulderGeometry::OnUpdate(CView *   aSenderPtr,
                                    LPARAM    aHint, 
                                    CObject * aHintPtr
                                   )
{

	if(!clear) {
		ClearValues();
		clear = true;
	}

    C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();

    mHeader = lDocPtr->ReportHeader();
	mFooter = lDocPtr->ReportFooter();
    
	bool standing = lDocPtr->getPosition() == P_Standing;
	GetDlgItem(IDC_BUTTON1)->ShowWindow( TRUE );

	Skeleton &lSkeleton = *lDocPtr->GetSkeleton();
	Vector3 offsetCoordinate = lDocPtr->getOffset();

	ShoulderGeometryData_s geometry;
	
	for (int i = 0; i < NUM_GEOMETRY_HINGES; ++i) {
		geometry.XMetric[i] = atof(hingeToMember[i][0]);
		geometry.YMetric[i] = atof(hingeToMember[i][1]);
		geometry.ZMetric[i] = atof(hingeToMember[i][2]);
	}

	lSkeleton.setShoulderGeometryData(geometry);

	mHandForceTxtLVal.Format("%.1f", lSkeleton.getExtForce(JT_LHAND).length());
	mHandForceTxtRVal.Format("%.1f", lSkeleton.getExtForce(JT_RHAND).length());

	//Vector3 offsetCoordinate = lDocPtr->getOffset();

	Vector3 LHand = lSkeleton.getJoint(JT_LHAND);

    mLeftHori.Format("%.1f", (LHand[1] -  offsetCoordinate[1]));
	mLeftLat.Format("%.1f", (LHand[0] - offsetCoordinate[0]));
	mLeftVert.Format("%.1f", (LHand[2] - offsetCoordinate[2]));


	Vector3 RHand = lSkeleton.getJoint(JT_RHAND);

	mRightHori.Format("%.1f", (RHand[1]  - offsetCoordinate[1]));
	mRightLat.Format("%.1f", (RHand[0] - offsetCoordinate[0]));
	mRightVert.Format("%.1f", (RHand[2] - offsetCoordinate[2]));

	UpdateUnits();
    UpdateWindow();
    UpdateData(FALSE);

	return;
}

void ReportShoulderGeometry::updateShoulderData(){
	clear = false;
    C_Hom_Doc* lDocPtr = (C_Hom_Doc*)GetDocument();

	setMuscleLables();

	double position_conv = 1;
	
	if(lDocPtr->Is_English()) {
		position_conv = U_CM_IN; 
	}

	mLeftHumeralHead[0].Format("%.2f", position_conv*shoModLeft.geoMod->hhc.x);		// Glenoidhumeral represented by the center of humeral head
	mLeftHumeralHead[1].Format("%.2f", position_conv*shoModLeft.geoMod->hhc.y);
	mLeftHumeralHead[2].Format("%.2f", position_conv*shoModLeft.geoMod->hhc.z);

	mRightHumeralHead[0].Format("%.2f", position_conv*shoModRight.geoMod->hhc.x);
	mRightHumeralHead[1].Format("%.2f", position_conv*shoModRight.geoMod->hhc.y);
	mRightHumeralHead[2].Format("%.2f", position_conv*shoModRight.geoMod->hhc.z);

	mLeftWristJoint[0].Format("%.2f", position_conv*shoModLeft.geoMod->wjct.x);
	mLeftWristJoint[1].Format("%.2f", position_conv*shoModLeft.geoMod->wjct.y);
	mLeftWristJoint[2].Format("%.2f", position_conv*shoModLeft.geoMod->wjct.z);
	
	mRightWristJoint[0].Format("%.2f", position_conv*shoModRight.geoMod->wjct.x);
	mRightWristJoint[1].Format("%.2f", position_conv*shoModRight.geoMod->wjct.y);
	mRightWristJoint[2].Format("%.2f", position_conv*shoModRight.geoMod->wjct.z);
	
	mLeftElbowJoint[0].Format("%.2f", position_conv*shoModLeft.geoMod->ejct.x);
	mLeftElbowJoint[1].Format("%.2f", position_conv*shoModLeft.geoMod->ejct.y);
	mLeftElbowJoint[2].Format("%.2f", position_conv*shoModLeft.geoMod->ejct.z);
	
	mRightElbowJoint[0].Format("%.2f", position_conv*shoModRight.geoMod->ejct.x);
	mRightElbowJoint[1].Format("%.2f", position_conv*shoModRight.geoMod->ejct.y);
	mRightElbowJoint[2].Format("%.2f", position_conv*shoModRight.geoMod->ejct.z);
	
	mLeftSternoclavicularJoint[0].Format("%.2f", position_conv*shoModLeft.geoMod->scjt.x);
	mLeftSternoclavicularJoint[1].Format("%.2f", position_conv*shoModLeft.geoMod->scjt.y);
	mLeftSternoclavicularJoint[2].Format("%.2f", position_conv*shoModLeft.geoMod->scjt.z);
	
	mRightSternoclavicularJoint[0].Format("%.2f", position_conv*shoModRight.geoMod->scjt.x);
	mRightSternoclavicularJoint[1].Format("%.2f", position_conv*shoModRight.geoMod->scjt.y);
	mRightSternoclavicularJoint[2].Format("%.2f", position_conv*shoModRight.geoMod->scjt.z);

	mLeftSJCTJoint[0].Format("%.2f", position_conv*shoModLeft.geoMod->sjct.x);
	mLeftSJCTJoint[1].Format("%.2f", position_conv*shoModLeft.geoMod->sjct.y);
	mLeftSJCTJoint[2].Format("%.2f", position_conv*shoModLeft.geoMod->sjct.z);
	
	mRightSJCTJoint[0].Format("%.2f", position_conv*shoModRight.geoMod->sjct.x);
	mRightSJCTJoint[1].Format("%.2f", position_conv*shoModRight.geoMod->sjct.y);
	mRightSJCTJoint[2].Format("%.2f", position_conv*shoModRight.geoMod->sjct.z);
	
	mLeftACRTJoint[0].Format("%.2f", position_conv*shoModLeft.geoMod->acrt.x);
	mLeftACRTJoint[1].Format("%.2f", position_conv*shoModLeft.geoMod->acrt.y);
	mLeftACRTJoint[2].Format("%.2f", position_conv*shoModLeft.geoMod->acrt.z);
	
	mRightACRTJoint[0].Format("%.2f", position_conv*shoModRight.geoMod->acrt.x);
	mRightACRTJoint[1].Format("%.2f", position_conv*shoModRight.geoMod->acrt.y);
	mRightACRTJoint[2].Format("%.2f", position_conv*shoModRight.geoMod->acrt.z);
	
	mLeftC7T1Joint[0].Format("%.2f", position_conv*shoModLeft.geoMod->c7t1t.x);
	mLeftC7T1Joint[1].Format("%.2f", position_conv*shoModLeft.geoMod->c7t1t.y);
	mLeftC7T1Joint[2].Format("%.2f", position_conv*shoModLeft.geoMod->c7t1t.z);
	
	mRightC7T1Joint[0].Format("%.2f", position_conv*shoModRight.geoMod->c7t1t.x);
	mRightC7T1Joint[1].Format("%.2f", position_conv*shoModRight.geoMod->c7t1t.y);
	mRightC7T1Joint[2].Format("%.2f", position_conv*shoModRight.geoMod->c7t1t.z);
	
	mLeftL5S1Joint[0].Format("%.2f", position_conv*shoModLeft.geoMod->l5s1t.x);
	mLeftL5S1Joint[1].Format("%.2f", position_conv*shoModLeft.geoMod->l5s1t.y);
	mLeftL5S1Joint[2].Format("%.2f", position_conv*shoModLeft.geoMod->l5s1t.z);
	
	mRightL5S1Joint[0].Format("%.2f", position_conv*shoModRight.geoMod->l5s1t.x);
	mRightL5S1Joint[1].Format("%.2f", position_conv*shoModRight.geoMod->l5s1t.y);
	mRightL5S1Joint[2].Format("%.2f", position_conv*shoModRight.geoMod->l5s1t.z);

	Skeleton &lSkeleton = *lDocPtr->GetSkeleton();
	ShoulderGeometryData_s geometry;
	
	for (int i = 0; i < NUM_GEOMETRY_HINGES; ++i) {
		geometry.XMetric[i] = atof(hingeToMember[i][0]);
		geometry.YMetric[i] = atof(hingeToMember[i][1]);
		geometry.ZMetric[i] = atof(hingeToMember[i][2]);
	}

	lSkeleton.setShoulderGeometryData(geometry);

	mHeader = lDocPtr->ReportHeader();
    mFooter = lDocPtr->ReportFooter();

    UpdateWindow();
    UpdateData(FALSE);  
    //UpdateData( FALSE );
}
/////////////////////////////////////////////////////////////////////////////
// ReportShoulderGeometry message handlers


HBRUSH ReportShoulderGeometry::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
 
   pDC->SetBkMode(TRANSPARENT);
   pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
   pDC->SetBkColor(lDocPtr->getBalColor());

   return (HBRUSH)(CBrush(lDocPtr->getBalColor()));
}

void ReportShoulderGeometry::ClearValues() {

	//Right side muscle percents
	for(int i = 0; i < 3; ++i){
		m1stThoracicVertebraS[i].Format("%.2f",0);
		m12thThoracicVertebraI[i].Format("%.2f",0);
		m1stThoracicVertebraM[i].Format("%.2f",0);
		mLeftSternoclavicularJoint[i].Format("%.2f",0);
		mRightSternoclavicularJoint[i].Format("%.2f",0);
		mLeftACJ[i].Format("%.2f",0);
		mRightACJ[i].Format("%.2f",0);
		mLeftScapulaAngle[i].Format("%.2f",0);
		mRightScapulaAngle[i].Format("%.2f",0);
		mLeftScapularSpine[i].Format("%.2f",0);
		mRightScapularSpine[i].Format("%.2f",0);
		mLeftHumeralHead[i].Format("%.2f",0);
		mRightHumeralHead[i].Format("%.2f",0);
		mLeftElbowJoint[i].Format("%.2f",0);
		mRightElbowJoint[i].Format("%.2f",0);
		mLeftWristJoint[i].Format("%.2f",0);
		mRightWristJoint[i].Format("%.2f",0);
	    mLeftSJCTJoint[i].Format("%.2f",0);
	    mRightSJCTJoint[i].Format("%.2f",0);
	    mLeftACRTJoint[i].Format("%.2f",0);
	    mRightACRTJoint[i].Format("%.2f",0);
	    mLeftC7T1Joint[i].Format("%.2f",0);
	    mRightC7T1Joint[i].Format("%.2f",0);
	    mLeftL5S1Joint[i].Format("%.2f",0);
	    mRightL5S1Joint[i].Format("%.2f",0);
    }
}
void ReportShoulderGeometry::OnDisplayDialogcolor() 
{
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
	lDocPtr->setReportColor();
	
	Invalidate(false);
}

void ReportShoulderGeometry::OnBnClickedSabtn()
{
    BeginWaitCursor();
#if defined(SHOULDERDLL)
    mProgressControl.ShowWindow(SW_SHOW);
    mProgressControl.SetRange(0,2);
    mProgressControl.SetPos(0);
    shoModRight.RightSide = true;
    shoModRight.Analyze();

    mProgressControl.SetPos(1);

    shoModLeft.RightSide = false;
    shoModLeft.Analyze();
    updateShoulderData();
    mProgressControl.SetPos(2);
#endif
    EndWaitCursor();
    //Invalidate(false);
    UpdateWindow();
    UpdateData(FALSE);
    GetDlgItem(IDC_BUTTON1)->ShowWindow( FALSE );
    mProgressControl.ShowWindow(SW_HIDE);
}

void ReportShoulderGeometry::setMuscleLables(){
    
	
}

void ReportShoulderGeometry::OnInitialUpdate()
{
	C_Hom_Doc* lDocPtr = (C_Hom_Doc*)GetDocument();
    
	setMuscleLables();

    mHeader = lDocPtr->ReportHeader();
    mFooter = lDocPtr->ReportFooter();

	ReportView::OnInitialUpdate();

	this->Invalidate();
	this->UpdateWindow();

	// initialize matlab
#if defined(SHOULDERDLL)
	try {
	if(!library_loaded){
		mProgressControl.SetRange(0,2);
		mProgressControl.SetPos(0);
		library_loaded = true;
		if (!mclInitializeApplication(NULL, 0)) {
			TRACE("could not initialize the library properly\n");
			library_loaded = false;
		}

		mProgressControl.SetPos(1);

		if(!libfminconInitialize()) // both of these fail
		{
			TRACE("could not initialize the library properly\n");
			library_loaded = false;
		}
		mProgressControl.SetPos(2);
		mProgressControl.ShowWindow(SW_HIDE);
	} else {
		mProgressControl.ShowWindow(SW_HIDE);
	}
	} catch (const mwException& e) {
		int i = 0;
	} catch (...) {
		int j = 0;
	}

	ShoulderModel::geoMod_library_loaded = ReportShoulderGeometry::library_loaded;
	clear = false;
#else
	mProgressControl.ShowWindow(SW_HIDE);

#endif
	// Set the hand force box units
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mLocationUnits = CString("Location (") + docPtr->LengthUnits() + ")";
	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";
	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	
	OnBnClickedSabtn();
	UpdateWindow();
	UpdateData(FALSE);
}

void ReportShoulderGeometry::UpdateUnits() {
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";

	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";

	mLocationUnits = CString("Location (") + docPtr->LengthUnits() + ")";

	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}
