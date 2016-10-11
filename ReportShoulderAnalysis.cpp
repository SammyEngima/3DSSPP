#include "ReportShoulderAnalysis.h"

#include "hom.h"
#include "hom_doc.h"
#include "ShoulderModel.h"
#include "Skeleton.hpp"
#if defined(SHOULDERDLL)
#include "libfmincon.h"
#endif
#include <sstream>
//#include <string>
using std::ostringstream;
using std::endl;


#ifdef _DEBUG
#define DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ReportView *ReportShoulderAnalysis::cPointer = NULL;
bool ReportShoulderAnalysis::library_loaded = false;

IMPLEMENT_DYNCREATE(ReportShoulderAnalysis, ReportView)

ReportShoulderAnalysis::ReportShoulderAnalysis() : ReportView(ReportShoulderAnalysis::IDD)
, ShoulderMessage(_T(""))
{
	//{{AFX_DATA_INIT(ReportShoulderAnalysis)
	mFooter = _T("");
	mHeader = _T("");
	mForcesLabel = _T("");

	///Yijun
	mHandForceUnits = _T("");
	mUnitsLabel = _T("");
	mLeftHori = _T("");

    for(int i = 0; i < 38; ++i){
        mSXL[i] = _T("");
    }

    for(int i = 0; i < 38; ++i){
        mSXF[i] = _T("---");
        mSXP[i] = _T("---");
    }
    
	for (int i = 0; i < 22; ++i) {
		mSXAL[i] = _T("");
	}

	for (int i = 0; i < 22; ++i) {
		mSXA[i] = _T("---");
	}

    //}}AFX_DATA_INIT
   cPointer = this;
}

ReportShoulderAnalysis::~ReportShoulderAnalysis() {
	cPointer = NULL;
}

void ReportShoulderAnalysis::killMatlab() {

#if defined(SHOULDERDLL)
	if(library_loaded) {
		libfminconTerminate();
		mclTerminateApplication();
		library_loaded = false;
	}

	ShoulderModel::mMod_library_loaded = ReportShoulderAnalysis::library_loaded;
#endif
}

CString ReportShoulderAnalysis::ReportName(void) const { return "3DSSPP - Shoulder Muscle Analysis Report"; }

void ReportShoulderAnalysis::DoDataExchange(CDataExchange* pDX)
{
	ReportView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ReportShoulderAnalysis)
	DDX_Text(pDX, IDC_Footer, mFooter);
	DDX_Text(pDX, IDC_Header, mHeader);

	DDX_Text(pDX, IDC_MUSPREFL, mFUnits);
	DDX_Text(pDX, IDC_MUSPREFL2, mFUnits);
	//DDX_Text(pDX, IDC_MUSPREFL3, mFUnits);
	//DDX_Text(pDX, IDC_MUSPREFL4, mFUnits);

	DDX_Text(pDX, IDC_SXL0, mSXL[0]);
	DDX_Text(pDX, IDC_SXL1, mSXL[1]);
	DDX_Text(pDX, IDC_SXL2, mSXL[2]);
	DDX_Text(pDX, IDC_SXL3, mSXL[3]);
	DDX_Text(pDX, IDC_SXL4, mSXL[4]);
	DDX_Text(pDX, IDC_SXL5, mSXL[5]);
	DDX_Text(pDX, IDC_SXL6, mSXL[6]);
	DDX_Text(pDX, IDC_SXL7, mSXL[7]);
	DDX_Text(pDX, IDC_SXL8, mSXL[8]);
	DDX_Text(pDX, IDC_SXL9, mSXL[9]);

	DDX_Text(pDX, IDC_SXL10, mSXL[10]);
	DDX_Text(pDX, IDC_SXL11, mSXL[11]);
	DDX_Text(pDX, IDC_SXL12, mSXL[12]);
	DDX_Text(pDX, IDC_SXL13, mSXL[13]);
	DDX_Text(pDX, IDC_SXL14, mSXL[14]);
	DDX_Text(pDX, IDC_SXL15, mSXL[15]);
	DDX_Text(pDX, IDC_SXL16, mSXL[16]);
	DDX_Text(pDX, IDC_SXL17, mSXL[17]);
	DDX_Text(pDX, IDC_SXL18, mSXL[18]);
	DDX_Text(pDX, IDC_SXL19, mSXL[19]);

	DDX_Text(pDX, IDC_SXL20, mSXL[20]);
	DDX_Text(pDX, IDC_SXL21, mSXL[21]);
	DDX_Text(pDX, IDC_SXL22, mSXL[22]);
	DDX_Text(pDX, IDC_SXL23, mSXL[23]);
	DDX_Text(pDX, IDC_SXL24, mSXL[24]);
	DDX_Text(pDX, IDC_SXL25, mSXL[25]);
	DDX_Text(pDX, IDC_SXL26, mSXL[26]);
	DDX_Text(pDX, IDC_SXL27, mSXL[27]);
	DDX_Text(pDX, IDC_SXL28, mSXL[28]);
	DDX_Text(pDX, IDC_SXL29, mSXL[29]);

	DDX_Text(pDX, IDC_SXL30, mSXL[30]);
	DDX_Text(pDX, IDC_SXL31, mSXL[31]);
	DDX_Text(pDX, IDC_SXL32, mSXL[32]);
	DDX_Text(pDX, IDC_SXL33, mSXL[33]);
	DDX_Text(pDX, IDC_SXL34, mSXL[34]);
	DDX_Text(pDX, IDC_SXL35, mSXL[35]);
	DDX_Text(pDX, IDC_SXL36, mSXL[36]);
	DDX_Text(pDX, IDC_SXL37, mSXL[37]);


	DDX_Text(pDX, IDC_SX0, mSXF[0]);
	DDX_Text(pDX, IDC_SX1, mSXF[1]);
	DDX_Text(pDX, IDC_SX2, mSXF[2]);
	DDX_Text(pDX, IDC_SX3, mSXF[3]);
	DDX_Text(pDX, IDC_SX4, mSXF[4]);
	DDX_Text(pDX, IDC_SX5, mSXF[5]);
	DDX_Text(pDX, IDC_SX6, mSXF[6]);
	DDX_Text(pDX, IDC_SX7, mSXF[7]);
	DDX_Text(pDX, IDC_SX8, mSXF[8]);
	DDX_Text(pDX, IDC_SX9, mSXF[9]);

	DDX_Text(pDX, IDC_SX10, mSXF[10]);
	DDX_Text(pDX, IDC_SX11, mSXF[11]);
	DDX_Text(pDX, IDC_SX12, mSXF[12]);
	DDX_Text(pDX, IDC_SX13, mSXF[13]);
	DDX_Text(pDX, IDC_SX14, mSXF[14]);
	DDX_Text(pDX, IDC_SX15, mSXF[15]);
	DDX_Text(pDX, IDC_SX16, mSXF[16]);
	DDX_Text(pDX, IDC_SX17, mSXF[17]);
	DDX_Text(pDX, IDC_SX18, mSXF[18]);
	DDX_Text(pDX, IDC_SX19, mSXF[19]);

	DDX_Text(pDX, IDC_SX20, mSXF[20]);
	DDX_Text(pDX, IDC_SX21, mSXF[21]);
	DDX_Text(pDX, IDC_SX22, mSXF[22]);
	DDX_Text(pDX, IDC_SX23, mSXF[23]);
	DDX_Text(pDX, IDC_SX24, mSXF[24]);
	DDX_Text(pDX, IDC_SX25, mSXF[25]);
	DDX_Text(pDX, IDC_SX26, mSXF[26]);
	DDX_Text(pDX, IDC_SX27, mSXF[27]);
	DDX_Text(pDX, IDC_SX28, mSXF[28]);
	DDX_Text(pDX, IDC_SX29, mSXF[29]);

	DDX_Text(pDX, IDC_SX30, mSXF[30]);
	DDX_Text(pDX, IDC_SX31, mSXF[31]);
	DDX_Text(pDX, IDC_SX32, mSXF[32]);
	DDX_Text(pDX, IDC_SX33, mSXF[33]);
	DDX_Text(pDX, IDC_SX34, mSXF[34]);
	DDX_Text(pDX, IDC_SX35, mSXF[35]);
	DDX_Text(pDX, IDC_SX36, mSXF[36]);
	DDX_Text(pDX, IDC_SX37, mSXF[37]);

	DDX_Text(pDX, IDC_MPX0, mSXP[0]);
	DDX_Text(pDX, IDC_MPX1, mSXP[1]);
	DDX_Text(pDX, IDC_MPX2, mSXP[2]);
	DDX_Text(pDX, IDC_MPX3, mSXP[3]);
	DDX_Text(pDX, IDC_MPX4, mSXP[4]);
	DDX_Text(pDX, IDC_MPX5, mSXP[5]);
	DDX_Text(pDX, IDC_MPX6, mSXP[6]);
	DDX_Text(pDX, IDC_MPX7, mSXP[7]);
	DDX_Text(pDX, IDC_MPX8, mSXP[8]);
	DDX_Text(pDX, IDC_MPX9, mSXP[9]);

	DDX_Text(pDX, IDC_MPX10, mSXP[10]);
	DDX_Text(pDX, IDC_MPX11, mSXP[11]);
	DDX_Text(pDX, IDC_MPX12, mSXP[12]);
	DDX_Text(pDX, IDC_MPX13, mSXP[13]);
	DDX_Text(pDX, IDC_MPX14, mSXP[14]);
	DDX_Text(pDX, IDC_MPX15, mSXP[15]);
	DDX_Text(pDX, IDC_MPX16, mSXP[16]);
	DDX_Text(pDX, IDC_MPX17, mSXP[17]);
	DDX_Text(pDX, IDC_MPX18, mSXP[18]);
	DDX_Text(pDX, IDC_MPX19, mSXP[19]);

	DDX_Text(pDX, IDC_MPX20, mSXP[20]);
	DDX_Text(pDX, IDC_MPX21, mSXP[21]);
	DDX_Text(pDX, IDC_MPX22, mSXP[22]);
	DDX_Text(pDX, IDC_MPX23, mSXP[23]);
	DDX_Text(pDX, IDC_MPX24, mSXP[24]);
	DDX_Text(pDX, IDC_MPX25, mSXP[25]);
	DDX_Text(pDX, IDC_MPX26, mSXP[26]);
	DDX_Text(pDX, IDC_MPX27, mSXP[27]);
	DDX_Text(pDX, IDC_MPX28, mSXP[28]);
	DDX_Text(pDX, IDC_MPX29, mSXP[29]);

	DDX_Text(pDX, IDC_MPX30, mSXP[30]);
	DDX_Text(pDX, IDC_MPX31, mSXP[31]);
	DDX_Text(pDX, IDC_MPX32, mSXP[32]);
	DDX_Text(pDX, IDC_MPX33, mSXP[33]);
	DDX_Text(pDX, IDC_MPX34, mSXP[34]);
	DDX_Text(pDX, IDC_MPX35, mSXP[35]);
	DDX_Text(pDX, IDC_MPX36, mSXP[36]);
	DDX_Text(pDX, IDC_MPX37, mSXP[37]);

	//DDX_Text(pDX, IDC_SX38, mSXF[38]);
	//DDX_Text(pDX, IDC_SX39, mSXF[39]);
	//DDX_Text(pDX, IDC_SX40, mSXF[40]);
	//DDX_Text(pDX, IDC_SX41, mSXF[41]);
	//DDX_Text(pDX, IDC_SX42, mSXF[42]);
	//DDX_Text(pDX, IDC_SX43, mSXF[43]);
	//DDX_Text(pDX, IDC_SX44, mSXF[44]);
	//DDX_Text(pDX, IDC_SX45, mSXF[45]);
	//DDX_Text(pDX, IDC_SX46, mSXF[46]);
	//DDX_Text(pDX, IDC_SX47, mSXF[47]);

	//DDX_Text(pDX, IDC_SX48, mSXF[48]);
	//DDX_Text(pDX, IDC_SX49, mSXF[49]);
	//DDX_Text(pDX, IDC_SX50, mSXF[50]);
	//DDX_Text(pDX, IDC_SX51, mSXF[51]);
	//DDX_Text(pDX, IDC_SX52, mSXF[52]);
	//DDX_Text(pDX, IDC_SX53, mSXF[53]);
	//DDX_Text(pDX, IDC_SX54, mSXF[54]);
	//DDX_Text(pDX, IDC_SX55, mSXF[55]);
	//DDX_Text(pDX, IDC_SX56, mSXF[56]);
	//DDX_Text(pDX, IDC_SX57, mSXF[57]);

	//DDX_Text(pDX, IDC_SX58, mSXF[58]);
	//DDX_Text(pDX, IDC_SX59, mSXF[59]);
	//DDX_Text(pDX, IDC_SX60, mSXF[60]);
	//DDX_Text(pDX, IDC_SX61, mSXF[61]);
	//DDX_Text(pDX, IDC_SX62, mSXF[62]);
	//DDX_Text(pDX, IDC_SX63, mSXF[63]);
	//DDX_Text(pDX, IDC_SX64, mSXF[64]);
	//DDX_Text(pDX, IDC_SX65, mSXF[65]);
	//DDX_Text(pDX, IDC_SX66, mSXF[66]);
	//DDX_Text(pDX, IDC_SX67, mSXF[67]);

	//DDX_Text(pDX, IDC_SX68, mSXF[68]);
	//DDX_Text(pDX, IDC_SX69, mSXF[69]);
	//DDX_Text(pDX, IDC_SX70, mSXF[70]);
	//DDX_Text(pDX, IDC_SX71, mSXF[71]);
	//DDX_Text(pDX, IDC_SX72, mSXF[72]);
	//DDX_Text(pDX, IDC_SX73, mSXF[73]);
	//DDX_Text(pDX, IDC_SX74, mSXF[74]);
	//DDX_Text(pDX, IDC_SX75, mSXF[75]);


	//DDX_Text(pDX, IDC_MPX38, mSXP[38]);
	//DDX_Text(pDX, IDC_MPX39, mSXP[39]);
	//DDX_Text(pDX, IDC_MPX40, mSXP[40]);
	//DDX_Text(pDX, IDC_MPX41, mSXP[41]);
	//DDX_Text(pDX, IDC_MPX42, mSXP[42]);
	//DDX_Text(pDX, IDC_MPX43, mSXP[43]);
	//DDX_Text(pDX, IDC_MPX44, mSXP[44]);
	//DDX_Text(pDX, IDC_MPX45, mSXP[45]);
	//DDX_Text(pDX, IDC_MPX46, mSXP[46]);
	//DDX_Text(pDX, IDC_MPX47, mSXP[47]);

	//DDX_Text(pDX, IDC_MPX48, mSXP[48]);
	//DDX_Text(pDX, IDC_MPX49, mSXP[49]);
	//DDX_Text(pDX, IDC_MPX50, mSXP[50]);
	//DDX_Text(pDX, IDC_MPX51, mSXP[51]);
	//DDX_Text(pDX, IDC_MPX52, mSXP[52]);
	//DDX_Text(pDX, IDC_MPX53, mSXP[53]);
	//DDX_Text(pDX, IDC_MPX54, mSXP[54]);
	//DDX_Text(pDX, IDC_MPX55, mSXP[55]);
	//DDX_Text(pDX, IDC_MPX56, mSXP[56]);
	//DDX_Text(pDX, IDC_MPX57, mSXP[57]);

	//DDX_Text(pDX, IDC_MPX58, mSXP[58]);
	//DDX_Text(pDX, IDC_MPX59, mSXP[59]);
	//DDX_Text(pDX, IDC_MPX60, mSXP[60]);
	//DDX_Text(pDX, IDC_MPX61, mSXP[61]);
	//DDX_Text(pDX, IDC_MPX62, mSXP[62]);
	//DDX_Text(pDX, IDC_MPX63, mSXP[63]);
	//DDX_Text(pDX, IDC_MPX64, mSXP[64]);
	//DDX_Text(pDX, IDC_MPX65, mSXP[65]);
	//DDX_Text(pDX, IDC_MPX66, mSXP[66]);
	//DDX_Text(pDX, IDC_MPX67, mSXP[67]);

	//DDX_Text(pDX, IDC_MPX68, mSXP[68]);
	//DDX_Text(pDX, IDC_MPX69, mSXP[69]);
	//DDX_Text(pDX, IDC_MPX70, mSXP[70]);
	//DDX_Text(pDX, IDC_MPX71, mSXP[71]);
	//DDX_Text(pDX, IDC_MPX72, mSXP[72]);
	//DDX_Text(pDX, IDC_MPX73, mSXP[73]);
	//DDX_Text(pDX, IDC_MPX74, mSXP[74]);
	//DDX_Text(pDX, IDC_MPX75, mSXP[75]);

	DDX_Text(pDX, IDC_STORQUERL, mSTorqueLabel);
	DDX_Text(pDX, IDC_STORQUERX, mSTorqueR[0]);
	DDX_Text(pDX, IDC_STORQUERY, mSTorqueR[1]);
	DDX_Text(pDX, IDC_STORQUERZ, mSTorqueR[2]);

	DDX_Text(pDX, IDC_SFORCERL, mSForceLabel);
	DDX_Text(pDX, IDC_SFORCERX, mSForceR[0]);
	DDX_Text(pDX, IDC_SFORCERY, mSForceR[1]);
	DDX_Text(pDX, IDC_SFORCERZ, mSForceR[2]);

	DDX_Text(pDX, IDC_STORQUELL, mSTorqueLabel);
	DDX_Text(pDX, IDC_STORQUELX, mSTorqueL[0]);
	DDX_Text(pDX, IDC_STORQUELY, mSTorqueL[1]);
	DDX_Text(pDX, IDC_STORQUELZ, mSTorqueL[2]);

	DDX_Text(pDX, IDC_SFORCELL, mSForceLabel);
	DDX_Text(pDX, IDC_SFORCELX, mSForceL[0]);
	DDX_Text(pDX, IDC_SFORCELY, mSForceL[1]);
	DDX_Text(pDX, IDC_SFORCELZ, mSForceL[2]);

	DDX_Text(pDX, IDC_ETORQUERL, mETorqueLabel);
	DDX_Text(pDX, IDC_ETORQUERX, mETorqueR[0]);
	DDX_Text(pDX, IDC_ETORQUERY, mETorqueR[1]);
	DDX_Text(pDX, IDC_ETORQUERZ, mETorqueR[2]);

	DDX_Text(pDX, IDC_ETORQUELL, mETorqueLabel);
	DDX_Text(pDX, IDC_ETORQUELX, mETorqueL[0]);
	DDX_Text(pDX, IDC_ETORQUELY, mETorqueL[1]);
	DDX_Text(pDX, IDC_ETORQUELZ, mETorqueL[2]);

	DDX_Text(pDX, IDC_SXAL0, mSXAL[0]);
	DDX_Text(pDX, IDC_SXAL1, mSXAL[1]);
	DDX_Text(pDX, IDC_SXAL2, mSXAL[2]);
	DDX_Text(pDX, IDC_SXAL3, mSXAL[3]);
	DDX_Text(pDX, IDC_SXAL4, mSXAL[4]);
	DDX_Text(pDX, IDC_SXAL5, mSXAL[5]);
	DDX_Text(pDX, IDC_SXAL6, mSXAL[6]);
	DDX_Text(pDX, IDC_SXAL7, mSXAL[7]);

	DDX_Text(pDX, IDC_SXA0, mSXA[0]);
	DDX_Text(pDX, IDC_SXA1, mSXA[1]);
	DDX_Text(pDX, IDC_SXA2, mSXA[2]);
	DDX_Text(pDX, IDC_SXA3, mSXA[3]);
	DDX_Text(pDX, IDC_SXA4, mSXA[4]);
	DDX_Text(pDX, IDC_SXA5, mSXA[5]);
	DDX_Text(pDX, IDC_SXA6, mSXA[6]);
	DDX_Text(pDX, IDC_SXA7, mSXA[7]);
	DDX_Text(pDX, IDC_SXA8, mSXA[8]);
	DDX_Text(pDX, IDC_SXA9, mSXA[9]);

	DDX_Text(pDX, IDC_SXA10, mSXA[10]);
	DDX_Text(pDX, IDC_SXA11, mSXA[11]);
	DDX_Text(pDX, IDC_SXA12, mSXA[12]);
	DDX_Text(pDX, IDC_SXA13, mSXA[13]);
	DDX_Text(pDX, IDC_SXA14, mSXA[14]);
	DDX_Text(pDX, IDC_SXA15, mSXA[15]);
	DDX_Text(pDX, IDC_SXA16, mSXA[16]);
	DDX_Text(pDX, IDC_SXA17, mSXA[17]);
	DDX_Text(pDX, IDC_SXA18, mSXA[18]);
	DDX_Text(pDX, IDC_SXA19, mSXA[19]);

	DDX_Text(pDX, IDC_SXA20, mSXA[20]);
	DDX_Text(pDX, IDC_SXA21, mSXA[21]);
	//DDX_Text(pDX, IDC_SXA22, mSXA[22]);
	//DDX_Text(pDX, IDC_SXA23, mSXA[23]);
	//DDX_Text(pDX, IDC_SXA24, mSXA[24]);
	//DDX_Text(pDX, IDC_SXA25, mSXA[25]);
	//DDX_Text(pDX, IDC_SXA26, mSXA[26]);
	//DDX_Text(pDX, IDC_SXA27, mSXA[27]);
	//DDX_Text(pDX, IDC_SXA28, mSXA[28]);
	//DDX_Text(pDX, IDC_SXA29, mSXA[29]);

	//DDX_Text(pDX, IDC_SXA30, mSXA[30]);
	//DDX_Text(pDX, IDC_SXA31, mSXA[31]);
	//DDX_Text(pDX, IDC_SXA32, mSXA[32]);
	//DDX_Text(pDX, IDC_SXA33, mSXA[33]);
	//DDX_Text(pDX, IDC_SXA34, mSXA[34]);
	//DDX_Text(pDX, IDC_SXA35, mSXA[35]);
	//DDX_Text(pDX, IDC_SXA36, mSXA[36]);
	//DDX_Text(pDX, IDC_SXA37, mSXA[37]);
	//DDX_Text(pDX, IDC_SXA38, mSXA[38]);
	//DDX_Text(pDX, IDC_SXA39, mSXA[39]);

	//DDX_Text(pDX, IDC_SXA40, mSXA[40]);
	//DDX_Text(pDX, IDC_SXA41, mSXA[41]);
	//DDX_Text(pDX, IDC_SXA42, mSXA[42]);
	//DDX_Text(pDX, IDC_SXA43, mSXA[43]);

	//}}AFX_DATA_MAP
	//DDX_Text(pDX, IDC_LEFT_SHOULDER_MESSAGE, LeftShoulderMessage);
	DDX_Text(pDX, IDC_SHOULDER_MESSAGE, ShoulderMessage);
	DDX_Control(pDX, IDC_PROGRESS2, mProgressControl);
	DDX_Text(pDX, IDC_MFPT, mMFPT);
	DDX_Text(pDX, IDC_MFPAIT, mMFPAIT);
	DDX_Text(pDX, IDC_ShoulderOutputSide, mShdrOutSide);
	DDX_Text(pDX, IDC_BUTTONSIDE, mShowSide);

	///Yijun
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

BEGIN_MESSAGE_MAP(ReportShoulderAnalysis, ReportView)
	//{{AFX_MSG_MAP(ReportShoulderAnalysis)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_DISPLAY_DIALOGCOLOR, OnDisplayDialogcolor)
	//}}AFX_MSG_MAP
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)   
	ON_BN_CLICKED(IDC_SABTN, OnBnClickedSabtn)
	ON_BN_CLICKED(IDC_BUTTONSIDE, OnBnClickedButtonSide) 
END_MESSAGE_MAP()

#ifdef _DEBUG
void ReportShoulderAnalysis::AssertValid() const { CFormView::AssertValid(); }
void ReportShoulderAnalysis::Dump(CDumpContext& dc) const { CFormView::Dump(dc); }
#endif

//void ReportShoulderAnalysis::OnFilePrintPreview() { SSPPPrintPreview(this); }

void ReportShoulderAnalysis::OnUpdate(CView* aSenderPtr,LPARAM aHint,CObject* aHintPtr)
{
	if(!clear) {
		ClearValues();
		clear = true;
	}
	//updateShoulderData();
	// when data changes, the shoulder values are not valid, so clear them,
	// and only show new ones when the button is clicked
	GetDlgItem(IDC_SABTN)->ShowWindow( TRUE );
	GetDlgItem(IDC_BUTTONSIDE)->ShowWindow( FALSE );

	C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();

	Skeleton* skel = lDocPtr->GetSkeleton();
	Vector3 RTorque = skel->getNetMoment(JT_RSHOULDER);
	Vector3 LTorque = skel->getNetMoment(JT_LSHOULDER);
	Vector3 RForce = skel->getNetForce(JT_RSHOULDER);
	Vector3 LForce = skel->getNetForce(JT_LSHOULDER);
	Vector3 RETorque = skel->getNetMoment(JT_RELBOW);
	Vector3 LETorque = skel->getNetMoment(JT_LELBOW);

	mSTorqueLabel = CString("SMoment(") + lDocPtr->TorqueUnits() + ")";
	mSForceLabel = CString("SForce(") + lDocPtr->ForceUnits() + ")";
 	mETorqueLabel = CString("EMoment(") + lDocPtr->TorqueUnits() + ")";
	mFUnits = CString("Force (") + lDocPtr->ForceUnits() + ")";
	mUnitsLabel = "Hand Locations (" + lDocPtr ->LengthUnits() + ")";
	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + lDocPtr->ForceUnits() + ")";
	m3DLowBackCompressionUnits = "3D Low back Compression (" + lDocPtr->ForceUnits() + ")";

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

	UpdateUnits();
	UpdateWindow();      
    UpdateData(FALSE);
}

void ReportShoulderAnalysis::updateShoulderData(){
	ostringstream message;
	message.str("");
	clear = false;

    C_Hom_Doc* lDocPtr = (C_Hom_Doc*)GetDocument();

	//TRACE("left exitflag: %d\n", shoModLeft.data.exitflag);
	//TRACE("right exitflag: %d\n", shoModRight.data.exitflag);
    
	setMuscleLables();
	// have to do unit conversions here because the shoulder model uses only metric
	double force_conv = 1;
	double moment_conv = 1;
	if(lDocPtr->Is_English()) {
		force_conv = N_TO_LBS;
		moment_conv = NM_TO_INLBS;
	}

    if (right_side) {

		//Right side muscle forces    
		for(int i = 0; i < 38; ++i){
			mSXF[i].Format("%.1f",force_conv*shoModRight.data.mfps[i]);
		}
    
		//Right side muscle percents
		for(int i = 0; i < 38; ++i){
		//mSXP[i] = String_Cast(Round_Double(shoModRight.data.mfpercent[i] * 100, 3), 3);
			mSXP[i].Format("%.1f",100*shoModRight.data.mfpercent[i]);
		} 

		//Right side additional muscle force information
		for(int i = 0; i < 22; ++i){
			mSXA[i].Format("%.1f", force_conv*shoModRight.data.mfps[38+i]);
		}
		//shoMod.killML();
		message << "Exitflag:\t" << shoModRight.data.exitflag << "\r\n" << "Feval:\t" << shoModRight.data.feval << "\r\n" << shoModRight.data.message;
	} 
	else {

		//Left side muscle forces
		for(int i = 0; i < 38; ++i){
			mSXF[i].Format("%.1f",force_conv*shoModLeft.data.mfps[i]);
		}

		//Left side muscle percents
		for(int i = 0; i < 38; ++i){
			mSXP[i].Format("%.1f",100*shoModLeft.data.mfpercent[i]);
		}

		//Left side additional muscle force information
		for(int i = 0; i < 22; ++i){
			mSXA[i].Format("%.1f", force_conv*shoModLeft.data.mfps[38+i]);
		}

		message << "Exitflag:\t" << shoModLeft.data.exitflag << "\r\n" << "Feval:\t" << shoModLeft.data.feval << "\r\n" << shoModLeft.data.message;
	}

	ShoulderAnalysisData_s analysis;

	for (int i = 0; i < 38; ++i) {
		analysis.SX[i] = force_conv*shoModRight.data.mfps[i];
		analysis.MPX[i] = 100*shoModRight.data.mfpercent[i];
		analysis.SX[i+38] = force_conv*shoModLeft.data.mfps[i];
		analysis.MPX[i+38] = 100*shoModLeft.data.mfpercent[i];
	}

	int cnt = 41;
	for (int i = 0; i < 3; ++i) {
		analysis.LGT[i] = force_conv*shoModRight.data.mfps[38+i];
		analysis.LGT[i+3] = force_conv*shoModLeft.data.mfps[38+i];
		analysis.JCFX[i] = force_conv*shoModRight.data.mfps[cnt];
		analysis.JCFX[i+3] = force_conv*shoModLeft.data.mfps[cnt++];
		analysis.JCFY[i] = force_conv*shoModRight.data.mfps[cnt];
		analysis.JCFY[i+3] = force_conv*shoModLeft.data.mfps[cnt++];
		analysis.JCFZ[i] = force_conv*shoModRight.data.mfps[cnt];
		analysis.JCFZ[i+3] = force_conv*shoModLeft.data.mfps[cnt++];
	}

	for (int i = 0; i < 10; ++i) {
		analysis.GSC[i] = force_conv*shoModRight.data.mfps[50+i];
		analysis.GSC[i+10] = force_conv*shoModLeft.data.mfps[50+i];
	}

	lDocPtr->GetSkeleton()->setShoulderAnalysisData(analysis);

	// UPdate SForce, STorque and ETorque information
	mSTorqueR[0].Format("%.2f", shoModRight.data.STorque.x);
	mSTorqueR[1].Format("%.2f", shoModRight.data.STorque.y);
	mSTorqueR[2].Format("%.2f", shoModRight.data.STorque.z);
	mSTorqueL[0].Format("%.2f", shoModLeft.data.STorque.x);
	mSTorqueL[1].Format("%.2f", shoModLeft.data.STorque.y);
	mSTorqueL[2].Format("%.2f", shoModLeft.data.STorque.z);

	mSForceR[0].Format("%.2f", shoModRight.data.SForce.x);
	mSForceR[1].Format("%.2f", shoModRight.data.SForce.y);
	mSForceR[2].Format("%.2f", shoModRight.data.SForce.z);
	mSForceL[0].Format("%.2f", shoModLeft.data.SForce.x);
	mSForceL[1].Format("%.2f", shoModLeft.data.SForce.y);
	mSForceL[2].Format("%.2f", shoModLeft.data.SForce.z);

	mETorqueR[0].Format("%.2f", shoModRight.data.ETorque.x);
	mETorqueR[1].Format("%.2f", shoModRight.data.ETorque.y);
	mETorqueR[2].Format("%.2f", shoModRight.data.ETorque.z);
	mETorqueL[0].Format("%.2f", shoModLeft.data.ETorque.x);
	mETorqueL[1].Format("%.2f", shoModLeft.data.ETorque.y);
	mETorqueL[2].Format("%.2f", shoModLeft.data.ETorque.z);

	ShoulderMessage = message.str().c_str();

    mHeader = lDocPtr->ReportHeader();
    mFooter = lDocPtr->ReportFooter();
}

void ReportShoulderAnalysis::ClearValues() {

	//Right side muscle percents
	for(int i = 0; i < 38; ++i){
        mSXF[i].Format("%.1f",0);
    }
    
    //Right side muscle percents
    for(int i = 0; i < 38; ++i){
		mSXP[i].Format("%.1f",0);
    }

    //Right side additional muscle force information
	for(int i = 0; i < 22; ++i){
		mSXA[i].Format("%.1f", 0);
	} 

	ShoulderAnalysisData_s analysis;

	for (int i = 0; i < 76; ++i) {
		analysis.SX[i] = 0;
		analysis.MPX[i] = 0;
	}

	for (int i = 0; i < 3; ++i) {
		analysis.LGT[i] = 0;
		analysis.LGT[i+3] = 0;
		analysis.JCFX[i] = 0;
		analysis.JCFX[i+3] = 0;
		analysis.JCFY[i] = 0;
		analysis.JCFY[i+3] = 0;
		analysis.JCFZ[i] = 0;
		analysis.JCFZ[i+3] = 0;
	}

	for (int i = 0; i < 10; ++i) {
		analysis.GSC[i] = 0;
		analysis.GSC[i+10] = 0;
	}

	C_Hom_Doc* lDocPtr = (C_Hom_Doc*)GetDocument();
	lDocPtr->GetSkeleton()->setShoulderAnalysisData(analysis);


    //Left side muscle forces
    //for(int i = 0; i < 38; ++i){
		//mSXF[i+38].Format("%.1f",0);
    //}

    //Left side muscle percents
    //for(int i = 0; i < 38; ++i){
	//	mSXP[i+38].Format("%.1f",0);
    //}

    //Left side additional muscle force information
	//for(int i = 0; i < 22; ++i){
	//	mSXA[i+22].Format("%.1f", 0);
	//}

    ShoulderMessage = "";
}

HBRUSH ReportShoulderAnalysis::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
    	
    C_Hom_Doc *lDocPtr = (C_Hom_Doc*)GetDocument();

    //if(pWnd->GetDlgCtrID() == IDC_SABTN)
	//	pWnd->ShowWindow( FALSE );
     
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
    pDC->SetBkColor(lDocPtr->getBalColor());

    return (HBRUSH)(CBrush(lDocPtr->getBalColor()));
}

void ReportShoulderAnalysis::OnDisplayDialogcolor() 
{
	C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)GetDocument();
	lDocPtr->setReportColor();
	
	Invalidate(false);
}

void ReportShoulderAnalysis::OnBnClickedSabtn()
{
    BeginWaitCursor();

#if defined(SHOULDERDLL)
    mProgressControl.ShowWindow(SW_SHOW);
    mProgressControl.SetRange(0,3);
    mProgressControl.SetPos(0);
    shoModRight.RightSide = true;
    shoModRight.Analyze();

    mProgressControl.SetPos(1);

    shoModLeft.RightSide = false;
    shoModLeft.Analyze();

    mProgressControl.SetPos(2);
    updateShoulderData();
    mProgressControl.SetPos(3);
#endif

    EndWaitCursor();
    //Invalidate(false);
    UpdateWindow();
    UpdateData(FALSE);
    GetDlgItem(IDC_SABTN)->ShowWindow( FALSE );
	GetDlgItem(IDC_BUTTONSIDE)->ShowWindow( TRUE );
    mProgressControl.ShowWindow(SW_HIDE);
}

void ReportShoulderAnalysis::setMuscleLables(){
    //Muscle labels
    mSXL[0] = CString("Upper Latissimus Dorsi");
    mSXL[1] = CString("Lower Latissimus Dorsi");
    mSXL[2] = CString("Levator Scapulae");
    mSXL[3] = CString("Omohymoid");
    mSXL[4] = CString("Pectoralis Major I");
    mSXL[5] = CString("Pectoralis Major II");
    mSXL[6] = CString("Pectoralis Minor");
    mSXL[7] = CString("Rhomboid Minor");
    mSXL[8] = CString("Rhomboid Major");
    mSXL[9] = CString("Upper Serratus Anterior");
    mSXL[10] = CString("Middle Serratus Anterior");
    mSXL[11] = CString("Lower Serratus Anterior");
    mSXL[12] = CString("Sternocleidomastoid");
    mSXL[13] = CString("Sternohyoid");
    mSXL[14] = CString("Subclavius");
    mSXL[15] = CString("Trapesius I");
    mSXL[16] = CString("Trapesius II");
    mSXL[17] = CString("Trapesius III");
    mSXL[18] = CString("Trapesius IV");
    mSXL[19] = CString("Middle Deltoid");
    mSXL[20] = CString("Anterior Deltoid");
    mSXL[21] = CString("Posterior Deltoid");
    mSXL[22] = CString("Coracobrachialis");
    mSXL[23] = CString("Infraspinatus I");
    mSXL[24] = CString("Infraspinatus II");
    mSXL[25] = CString("Subscapularis I");
    mSXL[26] = CString("Subscapularis II");
    mSXL[27] = CString("Subscapularis III");
    mSXL[28] = CString("Supraspinatus");
    mSXL[29] = CString("Teres Major");
    mSXL[30] = CString("Teres Minor");
    mSXL[31] = CString("Biceps I");
    mSXL[32] = CString("Biceps II");
    mSXL[33] = CString("Triceps I");
    mSXL[34] = CString("Triceps II");
    mSXL[35] = CString("Triceps III");
    mSXL[36] = CString("Brachialis");
    mSXL[37] = CString("Brachioradialis");

	mSXAL[0] = CString("coracohumeral");
	mSXAL[1] = CString("conoid");
	mSXAL[2] = CString("trapezoid");
	mSXAL[3] = CString("Sternoclavicular");
	mSXAL[4] = CString("Acromioclavicular");
	mSXAL[5] = CString("Glenohumeral");
	mSXAL[6] = CString("superior angle");
	mSXAL[7] = CString("inferior angle");

	if (right_side) {
		mMFPT = CString("Muscle Force Prediction ( Right )");
		mMFPAIT = CString("Muscle Force Precictions ( Right ) - Additional Information");
		mShdrOutSide = CString("Shoulder Output ( Right )");
		mShowSide = CString("Show Left");
	}
	else {
		mMFPT = CString("Muscle Force Prediction ( Left )");
		mMFPAIT = CString("Muscle Force Precictions ( Left ) - Additional Information");
		mShdrOutSide = CString("Shoulder Output ( Left )");
		mShowSide = CString("Show Right");
	}
}


void ReportShoulderAnalysis::OnInitialUpdate()
{
	C_Hom_Doc* lDocPtr = (C_Hom_Doc*)GetDocument();
    
	setMuscleLables();

	mSTorqueLabel = CString("SMoment(") + lDocPtr->TorqueUnits() + ")";
	mSForceLabel = CString("SForce(") + lDocPtr->ForceUnits() + ")";
	mETorqueLabel = CString("EMoment(") + lDocPtr->TorqueUnits() + ")";
 
	mFUnits = CString("Force (") + lDocPtr->ForceUnits() + ")";

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

	ShoulderModel::mMod_library_loaded = ReportShoulderAnalysis::library_loaded;
	clear = false;
	right_side = true;
	#else
	mProgressControl.ShowWindow(SW_HIDE);

	#endif
	
	// Set the hand force box units
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();
	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";
	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateWindow();

	OnBnClickedSabtn();

	UpdateWindow();
	UpdateData(FALSE);
}

void ReportShoulderAnalysis::UpdateUnits() {
	C_Hom_Doc* docPtr = (C_Hom_Doc*)GetDocument();

	mUnitsLabel = "Hand Locations (" + docPtr->LengthUnits() + ")";
	// Set the hand force box units
	mHandForceUnits = CString("Hand Forces (") + docPtr->ForceUnits() + ")";
	m3DLowBackCompressionUnits = "3D Low back Compression (" + docPtr->ForceUnits() + ")";
	UpdateData(false);
}

void ReportShoulderAnalysis::OnBnClickedButtonSide()
{
	// TODO: Add your control notification handler code here
	BeginWaitCursor();
	
	C_Hom_Doc* lDocPtr = (C_Hom_Doc*)GetDocument();

	ostringstream message;
	message.str("");

	double force_conv = 1;
	double moment_conv = 1;
	if(lDocPtr->Is_English()) {
		force_conv = N_TO_LBS;
		moment_conv = NM_TO_INLBS;
	}

	right_side = (right_side == true ? false : true);

	if (right_side) {
				//Right side muscle forces    
		for(int i = 0; i < 38; ++i){
			mSXF[i].Format("%.1f",force_conv*shoModRight.data.mfps[i]);
		}
    
		//Right side muscle percents
		for(int i = 0; i < 38; ++i){
		//mSXP[i] = String_Cast(Round_Double(shoModRight.data.mfpercent[i] * 100, 3), 3);
			mSXP[i].Format("%.1f",100*shoModRight.data.mfpercent[i]);
		} 

		//Right side additional muscle force information
		for(int i = 0; i < 22; ++i){
			mSXA[i].Format("%.1f", force_conv*shoModRight.data.mfps[38+i]);
		}
		
		//shoMod.killML();
		message << "Exitflag:\t" << shoModRight.data.exitflag << "\r\n" << "Feval:\t" << shoModRight.data.feval << "\r\n" << shoModRight.data.message;

		mMFPT = CString("Muscle Force Prediction ( Right )");
		mMFPAIT = CString("Muscle Force Precictions ( Right ) - Additional Information");
		mShdrOutSide = CString("Shoulder Output ( Right )");
		mShowSide = CString("Show Left");
	}
	else {
		//Left side muscle forces
		for(int i = 0; i < 38; ++i){
			mSXF[i].Format("%.1f",force_conv*shoModLeft.data.mfps[i]);
		}

		//Left side muscle percents
		for(int i = 0; i < 38; ++i){
			mSXP[i].Format("%.1f",100*shoModLeft.data.mfpercent[i]);
		}

		//Left side additional muscle force information
		for(int i = 0; i < 22; ++i){
			mSXA[i].Format("%.1f", force_conv*shoModLeft.data.mfps[38+i]);
		}

		message << "Exitflag:\t" << shoModLeft.data.exitflag << "\r\n" << "Feval:\t" << shoModLeft.data.feval << "\r\n" << shoModLeft.data.message;

		mMFPT = CString("Muscle Force Prediction ( Left )");
		mMFPAIT = CString("Muscle Force Precictions ( Left ) - Additional Information");
		mShdrOutSide = CString("Shoulder Output ( Left )");
		mShowSide = CString("Show Right");
	}

	ShoulderAnalysisData_s analysis;

	for (int i = 0; i < 38; ++i) {
		analysis.SX[i] = force_conv*shoModRight.data.mfps[i];
		analysis.MPX[i] = 100*shoModRight.data.mfpercent[i];
		analysis.SX[i+38] = force_conv*shoModLeft.data.mfps[i];
		analysis.MPX[i+38] = 100*shoModLeft.data.mfpercent[i];
	}

	int cnt = 41;
	for (int i = 0; i < 3; ++i) {
		analysis.LGT[i] = force_conv*shoModRight.data.mfps[38+i];
		analysis.LGT[i+3] = force_conv*shoModLeft.data.mfps[38+i];
		analysis.JCFX[i] = force_conv*shoModRight.data.mfps[cnt];
		analysis.JCFX[i+3] = force_conv*shoModLeft.data.mfps[cnt++];
		analysis.JCFY[i] = force_conv*shoModRight.data.mfps[cnt];
		analysis.JCFY[i+3] = force_conv*shoModLeft.data.mfps[cnt++];
		analysis.JCFZ[i] = force_conv*shoModRight.data.mfps[cnt];
		analysis.JCFZ[i+3] = force_conv*shoModLeft.data.mfps[cnt++];
	}

	for (int i = 0; i < 10; ++i) {
		analysis.GSC[i] = force_conv*shoModRight.data.mfps[50+i];
		analysis.GSC[i+10] = force_conv*shoModLeft.data.mfps[50+i];
	}

	lDocPtr->GetSkeleton()->setShoulderAnalysisData(analysis);
	
	ShoulderMessage = message.str().c_str();

    EndWaitCursor();
    //Invalidate(false);
    UpdateWindow();
	UpdateData(FALSE);
}
