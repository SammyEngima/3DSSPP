#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h"
#include "Skeleton.hpp"

#include "Dg_Anthropometry.h"
#include "GLOblique.h"
#include "StatusBox.h"
#include "AnthroPercentiles.h"
#include "DIB.h"
#include "PrintView.h"
#include "PrintFrame.h"

#include "afxwin.h"


//#include "units.hpp"

#ifdef _DEBUG
    #undef THIS_FILE
    static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(Dg_Anthropometry, CDialog)

Dg_Anthropometry::Dg_Anthropometry(C_Hom_Doc *aDocPtr, StatusBox* status_box_in, CWnd *pParent)
: CDialog(Dg_Anthropometry::IDD, pParent)
 , mShoeCheck(FALSE)
 , mShoeHeight(_T(""))
 , status_box(status_box_in)
 , mLengthLabel2(_T(""))
 , mCGLabel2(_T(""))
 , mWeightLabel2(_T(""))
 , mWeightLabel3(_T(""))
{
    //{{AFX_DATA_INIT(Dg_Anthropometry)
    mPercentileValue = -1;
    mGenderValue = -1;
    mHeightUnits = _T("");
    mWeightUnits = _T("");
    mHeightValue = 0.0f;
    mWeightValue = 0.0f;
	mHandL= 0.0f;
	 mHandWithFingersL= 0.0f;
	 mLowerArmL= 0.0f;
	 mUpperArmL= 0.0f;
	 mUpperLegL= 0.0f;
	 mLowerLegL= 0.0f;
	 mFootL= 0.0f;
	//Add C1
	 mNasionL= 0.0f;
	// Add:
	 mNeck1L= 0.0f;
	 mNeck2L= 0.0f;
	 mL5ToShoulderL= 0.0f;
	 mHipToL5L= 0.0f; 
    mMaintainHandPositions = FALSE;
    m_slider_percentile = 0;
    //}}AFX_DATA_INIT
	mDocPtr = aDocPtr;
	// memory leaks#1
	lDocPtr = new C_Hom_Doc;
    ASSERT(mDocPtr != NULL);
    newLengths = mDocPtr -> getLinkLengths();
    Length lLocX(0,1),lLocY(0,1),lLocZ(0,1);
	bool standing = mDocPtr->getPosition() == P_Standing;
	Vector3 lhand = mDocPtr->GetSkeleton()->getJoint(JT_LHAND, true);// - mDocPtr->getOffset(0, true);
	mLeftHand[0] = lhand[0];
	mLeftHand[1] = lhand[1];
	mLeftHand[2] = lhand[2];

	Vector3 rhand = mDocPtr->GetSkeleton()->getJoint(JT_RHAND, true);// - mDocPtr->getOffset(0, true);
	mRightHand[0] = rhand[0];
	mRightHand[1] = rhand[1];
	mRightHand[2] = rhand[2];
    updateClicked = false;
    applyClicked = true; //nothing to apply in the beginning
    if(mDocPtr->Is_English()) {
		lengthConversion = 0.39370;
		forceConversion = 0.224808;
	} else {
		lengthConversion = 1;
		forceConversion = 1;
	}
    standard = std::vector<double>(35);
    custom = std::vector<double>(35, 0.0);
    backup = std::vector<std::pair<double, double>>(35);
    dataEntry = false;
    modifyAnthro = false;
    valueChanged = false;
    left = true;
    UpdateStandard();
    UpdateBackup();
    newLengths = mDocPtr -> getLinkLengths();
    newWeights = mDocPtr -> getLinkWeights();
    newCGs = mDocPtr -> getLinkCGs();
	mDocPtr->setLinkCGs(newCGs);

	C_Hom_Doc * pDoc = Get_App_Ptr()->Get_Document_Ptr();
	Anthropometry& anthropometry = pDoc->getTask().getAnthropometryRef();
	
	if(anthropometry.getRefWeights().Hand < 0) {
		anthropometry.setRefWeights(newWeights);
		anthropometry.setNewWeights(newWeights);
	}
}
std::vector<bool> Dg_Anthropometry::isred  = std::vector<bool>(35);  //bool for checking if the value is same as the standard value
std::vector<bool> Dg_Anthropometry::isred_left  = std::vector<bool>(35); 
std::vector<bool> Dg_Anthropometry::isred_right  = std::vector<bool>(35); 
std::vector<double> Dg_Anthropometry::backl  = std::vector<double>(35);
std::vector<double> Dg_Anthropometry::backr  = std::vector<double>(35);
//double Dg_Anthropometry::mmLowerArmCG = 0.0f;
//double Dg_Anthropometry::mmLowerArmCGR = 0.0f;
//int Dg_Anthropometry::CGchange = 0;				 			    //double check if we can change the CG

BOOL Dg_Anthropometry::OnInitDialog()
{
	if(!CDialog::OnInitDialog())
		return false;
	this->UpdateDisplay();

	SetWindowText("Anthropometry");
	
	double shoeHeight = mDocPtr->GetFactors().mShoeHeight;
	if(mDocPtr->Is_English()) {
		shoeHeight *= CM_TO_IN;
	}
	mleftright = "LEFT SIDE";
	left = true;
	mShoeHeight.Format("%.1f", shoeHeight);
	mShoeCheck = (shoeHeight != 0);
	mShoeHeightControl.EnableWindow(shoeHeight != 0);
    UpdateReport();
	UpdateData(false);
	if (valueChanged != TRUE)  
		{
			mCaution.ShowWindow(SW_HIDE);
			valueChanged = FALSE;
		}

    CRect rect;
    GetWindowRect(&rect);
    ScreenToClient(&rect);
	// Yijun: resize the windows
    MoveWindow(0, 0, rect.Width() / 3, rect.Height(), true);
    CenterWindow();

	initialback();
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->AppendMenu(MF_SEPARATOR);// add a separator
	pSysMenu->AppendMenu(MF_STRING,IDR_MENU1,"Print Window");// add print menu
	return true;
}

void Dg_Anthropometry::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{	
	CSliderCtrl* pSlide = (CSliderCtrl*) pScrollBar;
}

//---------------------------------------------------------------------------
void Dg_Anthropometry::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CGLabel, mCGLabel);
	DDX_Text(pDX, IDC_HeadL, mHeadL);
	DDX_Text(pDX, IDC_HeadCG, mHeadCG);
	DDX_Text(pDX, IDC_HeadWt, mHeadWt);
	DDX_Text(pDX, IDC_FootCG, mFootCG);
	DDX_Text(pDX, IDC_FootL, mFootL);
	DDX_Text(pDX, IDC_FootWt, mFootWt);
	DDX_Text(pDX, IDC_HipToL5CG, mHipToL5CG);
	DDX_Text(pDX, IDC_HipToL5L, mHipToL5L);
	DDX_Text(pDX, IDC_HipToL5Wt, mHipToL5Wt);
	DDX_Text(pDX, IDC_L5ToShoulderL, mL5ToShoulderL);
	DDX_Text(pDX, IDC_L5ToShoulderCG, mL5ToShoulderCG);
	DDX_Text(pDX, IDC_L5ToShoulderWt, mL5ToShoulderWt);
	DDX_Text(pDX, IDC_NasionL, mNasionL);
	DDX_Text(pDX, IDC_Neck1L, mNeck1L);
	DDX_Text(pDX, IDC_Neck1CG, mNeck1CG);
	DDX_Text(pDX, IDC_Neck1Wt, mNeck1Wt);
	DDX_Text(pDX, IDC_Neck2L, mNeck2L);
	DDX_Text(pDX, IDC_Neck2CG, mNeck2CG);
	DDX_Text(pDX, IDC_Neck2Wt, mNeck2Wt);
	DDX_Text(pDX, IDC_LengthLabel, mLengthLabel);
	DDX_Text(pDX, IDC_LowerArmL, mLowerArmL);
	DDX_Text(pDX, IDC_LowerArmWt, mLowerArmWt);
	DDX_Text(pDX, IDC_LowerLegCG, mLowerLegCG);
	DDX_Text(pDX, IDC_LowerLegL, mLowerLegL);
	DDX_Text(pDX, IDC_LowerLegWt, mLowerLegWt);
	DDX_Text(pDX, IDC_LowerArmCG, mLowerArmCG);
	DDX_Text(pDX, IDC_UpperArmCG, mUpperArmCG);
	DDX_Text(pDX, IDC_UpperArmL, mUpperArmL);
	DDX_Text(pDX, IDC_UpperArmWt, mUpperArmWt);
	DDX_Text(pDX, IDC_UpperLegL, mUpperLegL);
	DDX_Text(pDX, IDC_UpperLegWt, mUpperLegWt);
	DDX_Text(pDX, IDC_WeightLabel, mWeightLabel);
	DDX_Text(pDX, IDC_UpperLegCG, mUpperLegCG);
	//DDX_Text(pDX, IDC_DiaphramMomentL, mDiaphramMomentL);
	DDX_Text(pDX, IDC_Factors, mFactorsName);
	//}}AFX_DATA_MAP
	//DDX_Text(pDX, IDC_HipToHipL, mHipToHipL);
	//DDX_Text(pDX, IDC_HipToHipWt, mHipToHipWt);
	// DDX_Text(pDX, IDC_ShoulderToShoulderL, mShoulderToShoulderL);
	DDX_Text(pDX, IDC_HandL, mHandL);

	// DDX_Text(pDX, IDC_LowerArmAndHandL, mLowerArmAndHandL);
	// DDX_Text(pDX, IDC_LowerArmAndHandCG, mLowerArmAndHandCG);
	// DDX_Text(pDX, IDC_LowerArmAndHandWt, mLowerArmAndHandWt);
	DDX_Text(pDX, IDC_HandWithFingersL, mHandWithFingersL);
	DDX_Text(pDX, IDC_HandWithFingersCG, mHandWithFingersCG);
	DDX_Text(pDX, IDC_HandWithFingersWt, mHandWithFingersWt);
	DDX_Control(pDX, IDC_MaintainHandPositions, mMaintainHandPositionsControl);
	DDX_Control(pDX, IDC_WeightValue, mWeightControl);
	DDX_Control(pDX, IDC_HeightValue, mHeightControl);
	DDX_Radio(pDX, IDC_PercentileValue, mPercentileValue);
	DDX_Radio(pDX, IDC_GenderValue, mGenderValue);
	DDX_Text(pDX, IDC_HeightUnits, mHeightUnits);
	DDX_Text(pDX, IDC_HeightUnits2, mHeightUnits);
	DDX_Text(pDX, IDC_WeightUnits, mWeightUnits);
	DDX_Text(pDX, IDC_HeightValue, mHeightValue);
	DDX_Text(pDX, IDC_WeightValue, mWeightValue);
	DDX_Check(pDX, IDC_MaintainHandPositions, mMaintainHandPositions);
	DDX_Check(pDX, IDC_SHOE_CHECK, mShoeCheck);
	DDX_Control(pDX, IDC_CAUTION, mCaution);
	DDX_Text(pDX, IDC_SHOE_HEIGHT_EDIT, mShoeHeight);
	DDX_Control(pDX, IDC_SHOE_HEIGHT_EDIT, mShoeHeightControl);
	DDX_Text(pDX, IDC_STATIC29, mleftright);
	DDV_MinMaxDouble(pDX, mHeadCG, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mFootCG, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mHipToL5CG, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mNeck1CG, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mNeck2CG, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mLowerLegCG, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mLowerArmCG, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mUpperArmCG, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mUpperLegCG, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mHandWithFingersCG, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mHeadL, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mFootL, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mHipToL5L, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mL5ToShoulderL, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mL5ToShoulderCG, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mL5ToShoulderWt, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mNasionL, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mNeck1L, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mNeck2L, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mLowerArmL, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mLowerLegL, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mUpperArmL, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mUpperLegL, 0, DBL_MAX);
	//DDV_MinMaxDouble(pDX, mHipToHipL, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mHandL, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mHandWithFingersL, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mHeadWt, 0, DBL_MAX); 
	DDV_MinMaxDouble(pDX, mFootWt, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mHipToL5Wt, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mNeck1Wt, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mNeck2Wt, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mLowerArmWt, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mUpperArmWt, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mUpperLegWt, 0, DBL_MAX);
	DDV_MinMaxDouble(pDX, mHandWithFingersWt, 0, DBL_MAX);
	DDX_Text(pDX, IDC_LengthLabel2, mLengthLabel2);
	DDX_Text(pDX, IDC_CGLabel2, mCGLabel2);
	DDX_Text(pDX, IDC_WeightLabel2, mWeightLabel2);
	DDX_Text(pDX, IDC_WeightLabel3, mWeightLabel3);
}

BEGIN_MESSAGE_MAP(Dg_Anthropometry, CDialog)
	ON_BN_CLICKED(IDC_PercentileValue, OnPercentile95th)
	ON_BN_CLICKED(IDC_Percentile50th, OnPercentile50th)
	ON_BN_CLICKED(IDC_Percentile5th, OnPercentile5th)
	ON_BN_CLICKED(IDC_PercentileDataEntry, OnPercentileDataEntry)
	ON_BN_CLICKED(IDC_GenderValue, OnGenderMale)
	ON_BN_CLICKED(IDC_GenderFemale, OnGenderFemale)
	ON_WM_PAINT()
    ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_REDRAW, OnRedraw)
	ON_BN_CLICKED(IDC_ADVANCED, OnAdvanced)
	//ON_EN_KILLFOCUS(IDC_WeightValue, OnKillfocusWeightValue)
	//ON_EN_KILLFOCUS(IDC_HeightValue, OnKillfocusHeightValue)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_HeightValue, &Dg_Anthropometry::OnEnChangeHeightvalue)
	ON_EN_SETFOCUS(IDC_HeightValue, &Dg_Anthropometry::OnEnSetfocusHeightvalue)
	ON_EN_CHANGE(IDC_WeightValue, &Dg_Anthropometry::OnEnChangeWeightvalue)
	ON_EN_SETFOCUS(IDC_WeightValue, &Dg_Anthropometry::OnEnSetfocusWeightvalue)
	ON_BN_CLICKED(IDC_SHOE_CHECK, &Dg_Anthropometry::OnBnClickedShoeCheck)
    ON_BN_CLICKED(IDAPPLY, &Dg_Anthropometry::OnBnClickedApply)
    ON_CONTROL_RANGE(EN_UPDATE, IDC_HandL, IDC_HipToL5Wt, &Dg_Anthropometry::ApplyCustomChanges) //applies to all figures in report

    ON_BN_CLICKED(IDC_ANTHRO, &Dg_Anthropometry::OnBnClickedAnthro)
    ON_BN_CLICKED(IDC_LEFT, &Dg_Anthropometry::OnBnClickedLeft)
    //ON_BN_CLICKED(IDC_RIGHT, &Dg_Anthropometry::OnBnClickedRight)
	ON_BN_CLICKED(IDC_Print, &Dg_Anthropometry::OnBnClickedPrint)
	ON_MESSAGE(WM_BEGIN_PRINTING, &Dg_Anthropometry::OnBeginPrinting)
	ON_MESSAGE(WM_END_PRINTING, &Dg_Anthropometry::OnEndPrinting)
	ON_MESSAGE(WM_MY_PRINT, &Dg_Anthropometry::OnMyPrint)
	ON_COMMAND(ID_PRINT_PRINTWINDOW, &Dg_Anthropometry::OnPrintPrintwindow)
	//ON_WM_CONTEXTMENU()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

HBRUSH Dg_Anthropometry::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor)
{
    int ID = pWnd -> GetDlgCtrlID();
    if (ID >= IDC_HandL && ID <= IDC_HipToL5Wt)
    {
        int index = ID - IDC_HandL;
        
        switch (nCtlColor)
	    {
	    case CTLCOLOR_EDIT:
            {
                if (custom[index] != Round_Double(standard[index], 1))
                {
                    valueChanged = true;
	                pDC->SetTextColor(RGB(255, 0, 0));// Set color to Red
	                return (HBRUSH)GetStockObject(NULL_BRUSH);
                }
				else 
				{
					isred[index] = false; 
				}
            }
            
	    default:
	        return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	    }
    }
	if (ID == IDC_CAUTION)
    {        
        switch (nCtlColor)
	    {
	    case CTLCOLOR_STATIC:
            {
	                pDC->SetTextColor(RGB(255, 0, 0));// Set color to Red
					pDC->SetBkMode(TRANSPARENT); 
	                return (HBRUSH)GetStockObject(NULL_BRUSH);
            }
            
	    default:
	        return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	    }
    }
    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}


void Dg_Anthropometry::InitializeCustomValues()
{
	UpdateStandard();
	UpdateData(TRUE);
	//UpdateData(False);
	valueChanged = FALSE;
	CString ss;
    GetDlgItemTextA(IDC_Neck1Wt, ss);

	isred = std::vector<bool>(35, false);
	isred_left = std::vector<bool>(35, false);
	isred_right = std::vector<bool>(35, false);
	
	for (int ID = IDC_HandL; ID <= IDC_HipToL5Wt; ++ID) {
		// fill in the custom array
		CString s;
        GetDlgItemTextA(ID, s);
        custom[ID - IDC_HandL] = atof(s);

		// check both sides if applicable
		double victim, victim2 = -5.0;
		switch(ID)
		{
		case IDC_HeadL:
			//wait for more data for head
			victim = newLengths.C1Tragion * lengthConversion;
			break;
		case IDC_HeadCG:
			victim = newCGs.Head * lengthConversion;
			break;
		case IDC_HeadWt:
			victim = newWeights.Head * forceConversion;
			break;
		case IDC_HipToL5L:
			victim = newLengths.HipToL5S1 * lengthConversion;
			break;
		case IDC_HipToL5Wt:
			victim = newWeights.Pelvis * forceConversion;
			break;
		case IDC_HipToL5CG:
			victim = newCGs.Pelvis * lengthConversion;
			break;
		case IDC_L5ToShoulderL:
			victim = newLengths.L5S1ToC7T1 * lengthConversion;
			break;
		case IDC_L5ToShoulderCG:
			victim = newCGs.L5S1ToShoulder * lengthConversion;
			break;
		case IDC_L5ToShoulderWt:
			victim = newWeights.TorsoAndHead * forceConversion;
			break;
		case IDC_NasionL:
			victim = newLengths.HeadRadius * lengthConversion;
			break;
		case IDC_Neck1L:
			victim = newLengths.C1C3 * lengthConversion;
			break;
		case IDC_Neck1Wt:
			victim = newWeights.UpperNeck * forceConversion;
			break;
		case IDC_Neck1CG:
			victim = newCGs.UpperNeck * lengthConversion;
			break;
		case IDC_Neck2L:
			victim = newLengths.C4C7 * lengthConversion;
			break;
		case IDC_Neck2Wt:
			victim = newWeights.LowerNeck * forceConversion;
			break;
		case IDC_Neck2CG:
			victim = newCGs.LowerNeck * lengthConversion; 
			break;

		case IDC_FootL:
			victim = newLengths.HeelToToe * lengthConversion;
			victim2 = newLengths.HeelToToeR * lengthConversion;
			break;
		case IDC_FootWt:
			victim = newWeights.Foot * forceConversion;
			victim2 = newWeights.FootR * forceConversion;
			break;
		case IDC_FootCG:
			victim = newCGs.Foot * lengthConversion;
			victim2 = newCGs.FootR * lengthConversion;
			break;
		case IDC_LowerArmL:
			victim = newLengths.Forearm * lengthConversion;
			victim2 = newLengths.ForearmR * lengthConversion;
			break;
		case IDC_LowerArmWt:
			victim = newWeights.Forearm * forceConversion;
			victim2 = newWeights.ForearmR * forceConversion;
			break;
		case IDC_LowerArmCG:
			victim = newCGs.Forearm * lengthConversion;
			victim2 = newCGs.ForearmR * lengthConversion;
			break;
		case IDC_LowerLegL:
			victim = newLengths.LowerLeg * lengthConversion;
			victim2 = newLengths.LowerLegR * lengthConversion;
			break;
		case IDC_LowerLegCG:
			victim = newCGs.LowerLeg * lengthConversion;
			victim2 = newCGs.LowerLegR * lengthConversion;
			break;
		case IDC_LowerLegWt:
			victim = newWeights.LowerLeg * forceConversion;
			victim2 = newWeights.LowerLegR * forceConversion;
			break;
		case IDC_UpperArmL:
			victim = newLengths.UpperArm * lengthConversion;
			victim2 = newLengths.UpperArmR * lengthConversion;
			break;
		case IDC_UpperArmCG:
			victim = newCGs.UpperArm * lengthConversion;
			victim2 = newCGs.UpperArmR * lengthConversion;
			break;
		case IDC_UpperArmWt:
			victim = newWeights.UpperArm * forceConversion;
			victim2 = newWeights.UpperArmR * forceConversion;
			break;
		case IDC_UpperLegL:
			victim = newLengths.UpperLeg * lengthConversion;
			victim2 = newLengths.UpperLegR * lengthConversion;
			break;
		case IDC_UpperLegCG:
			victim = newCGs.UpperLeg * lengthConversion;
			victim2 = newCGs.UpperLegR * lengthConversion;
			break;
		case IDC_UpperLegWt:
			victim = newWeights.UpperLeg * forceConversion;
			victim2 = newWeights.UpperLegR * forceConversion;
			break;
		case IDC_HandL:
			victim = newLengths.Hand * lengthConversion;
			victim2 = newLengths.HandR * lengthConversion;
			break;
		case IDC_HandWithFingersL:
			victim = newLengths.HandWithFingers * lengthConversion;
			victim2 = newLengths.HandWithFingersR * lengthConversion;
			break;
		case IDC_HandWithFingersCG: //same as HandCG
			victim = newCGs.Hand * lengthConversion;
			victim2 = newCGs.HandR * lengthConversion;
			break;
		case IDC_HandWithFingersWt: //same as HandWt
			victim = newWeights.Hand * forceConversion;
			victim2 = newWeights.HandR * forceConversion;
			break;
		}	

		if (Round_Double(victim, 1) != Round_Double(standard[ID - IDC_HandL], 1) ) {
			int N = ID-IDC_HandL;
			valueChanged = TRUE;
			isred_left[ID - IDC_HandL] = TRUE;
			if(!dataEntry)
				mCaution.ShowWindow(SW_SHOW);
			if(left)
				isred[ID - IDC_HandL] = TRUE;
			
		}

		if ( victim2 > 0 
			&& Round_Double(victim2, 1) != Round_Double(standard[ID - IDC_HandL], 1) ) {
			int N = ID-IDC_HandL;
			valueChanged = TRUE;
			isred_right[ID - IDC_HandL] = TRUE;
			if(!dataEntry)
				mCaution.ShowWindow(SW_SHOW);
			if(!left)
				isred[ID - IDC_HandL] = TRUE;
		}
	}
	
	
	if (valueChanged != TRUE) {
		mCaution.ShowWindow(SW_HIDE);
		valueChanged = FALSE;
	}
	

	if(valueChanged) {
		mDocPtr -> getTask().getAnthropometryRef().setUnique(true);
	} else {
		mDocPtr -> getTask().getAnthropometryRef().setUnique(false);
	}
	
	/*
	if (valueChanged == TRUE && dataEntry) {
		mCaution.ShowWindow(SW_SHOW);
	}
	*/
}

void Dg_Anthropometry::RetrieveBackupValues() {
    //puts what's in the textboxes into the variables

    LinkLengths lengths = mDocPtr -> getLinkLengths(); 
    LinkCGs CGs = mDocPtr -> getLinkCGs();
    LinkWeights weights = mDocPtr -> getLinkWeights();
    //extremities
    lengths.Hand = backup[0].first / lengthConversion;
    lengths.HandR = backup[0].second / lengthConversion;
    lengths.HandWithFingers = backup[1].first / lengthConversion;
    lengths.HandWithFingersR = backup[1].second / lengthConversion;
    CGs.Hand = backup[2].first / lengthConversion;
    CGs.HandR = backup[2].second / lengthConversion;
    weights.Hand = backup[3].first / forceConversion;
    weights.HandR = backup[3].second / forceConversion;
    lengths.Forearm = backup[4].first / lengthConversion;
    lengths.ForearmR = backup[4].second / lengthConversion;
    CGs.Forearm = backup[5].first / lengthConversion;
    CGs.ForearmR = backup[5].second / lengthConversion;
    weights.Forearm = backup[6].first / forceConversion;
    weights.ForearmR = backup[6].second / forceConversion;
    lengths.UpperArm = backup[7].first / lengthConversion;
    lengths.UpperArmR = backup[7].second / lengthConversion;
    CGs.UpperArm = backup[8].first / lengthConversion;
    CGs.UpperArmR = backup[8].second / lengthConversion;
    weights.UpperArm = backup[9].first / forceConversion;
    weights.UpperArmR = backup[9].second / forceConversion;
    lengths.UpperLeg = backup[10].first / lengthConversion;
    lengths.UpperLegR = backup[10].second / lengthConversion;
    CGs.UpperLeg = backup[11].first / lengthConversion;
    CGs.UpperLegR = backup[11].second / lengthConversion;
    weights.UpperLeg = backup[12].first / forceConversion;
    weights.UpperLegR = backup[12].second / forceConversion;
    lengths.LowerLeg = backup[13].first / lengthConversion;
    lengths.LowerLegR = backup[13].second / lengthConversion;
    CGs.LowerLeg = backup[14].first / lengthConversion;
    CGs.LowerLegR = backup[14].second / lengthConversion;
    weights.LowerLeg = backup[15].first / forceConversion;
    weights.LowerLegR = backup[15].second / forceConversion;
    lengths.HeelToToe = backup[16].first / lengthConversion;
    lengths.HeelToToeR = backup[16].second / lengthConversion;
    CGs.Foot = backup[17].first / lengthConversion;
    CGs.FootR = backup[17].second / lengthConversion;
    weights.Foot = backup[18].first / forceConversion;
    weights.FootR = backup[18].second / forceConversion;
    //head and trunk

    //head stuff here (19,20,21)
	lengths.C1Tragion = backup[19].first / lengthConversion;
	CGs.Head = backup[20].first / lengthConversion;
	weights.Head = backup[21].first / forceConversion;
    lengths.HeadRadius = backup[22].first / lengthConversion;
    lengths.C1C3 = backup[23].first / lengthConversion;
    CGs.UpperNeck = backup[24].first / lengthConversion;
	weights.UpperNeck = backup[25].first / forceConversion;
	//neck1 CG and weights here (24, 25)
    lengths.C4C7 = backup[26].first / lengthConversion;
    //neck2 CG and weights here (27, 28)
	CGs.LowerNeck = backup[27].first / lengthConversion;
	weights.LowerNeck = backup[28].first / forceConversion;
    lengths.L5S1ToC7T1 = backup[29].first / lengthConversion;
    CGs.L5S1ToShoulder = backup[30].first / lengthConversion;
    weights.TorsoAndHead = backup[31].first / forceConversion;
    lengths.HipToL5S1 = backup[32].first / lengthConversion;
    CGs.Pelvis = backup[33].first / lengthConversion;
    weights.Pelvis = backup[34].first / forceConversion;
    //lengths.HipToHip = backup[35] / lengthConversion;
  
    mDocPtr -> setLinkLengths(lengths);
    mDocPtr -> setLinkCGs(CGs);
    mDocPtr -> setLinkWeights(weights);
}
void Dg_Anthropometry::UpdateBackup()
{
	mAnthroBackup = mDocPtr -> getAnthropometryRef();
    LinkLengths lengths = mDocPtr -> getLinkLengths();
    LinkCGs CGs = mDocPtr ->getLinkCGs();
    LinkWeights weights = mDocPtr ->getLinkWeights();
    //extremities
    backup[0].first = lengths.Hand * lengthConversion;
    backup[0].second = lengths.HandR * lengthConversion;
    backup[1].first = lengths.HandWithFingers * lengthConversion;
    backup[1].second = lengths.HandWithFingersR * lengthConversion;
    backup[2].first = CGs.Hand * lengthConversion;
    backup[2].second = CGs.HandR * lengthConversion;
    backup[3].first = weights.Hand * forceConversion;
    backup[3].second = weights.HandR * forceConversion;
    backup[4].first = lengths.Forearm * lengthConversion;
    backup[4].second =  lengths.ForearmR * lengthConversion;
    backup[5].first = CGs.Forearm * lengthConversion;
    backup[5].second = CGs.ForearmR * lengthConversion;
    backup[6].first = weights.Forearm * forceConversion;
    backup[6].second = weights.ForearmR * forceConversion;
    backup[7].first = lengths.UpperArm * lengthConversion;
    backup[7].second = lengths.UpperArmR * lengthConversion;
    backup[8].first = CGs.UpperArm * lengthConversion;
    backup[8].second = CGs.UpperArmR * lengthConversion;
    backup[9].first = weights.UpperArm * forceConversion;
    backup[9].second = weights.UpperArmR * forceConversion;
    backup[10].first = lengths.UpperLeg * lengthConversion;
    backup[10].second = lengths.UpperLegR * lengthConversion;
    backup[11].first = CGs.UpperLeg * lengthConversion;
    backup[11].second = CGs.UpperLegR * lengthConversion;
    backup[12].first = weights.UpperLeg * forceConversion;
    backup[12].second = weights.UpperLegR * forceConversion;
    backup[13].first = lengths.LowerLeg * lengthConversion;
    backup[13].second = lengths.LowerLegR * lengthConversion;
    backup[14].first = CGs.LowerLeg * lengthConversion;
    backup[14].second = CGs.LowerLegR * lengthConversion;
    backup[15].first = weights.LowerLeg * forceConversion;
    backup[15].second = weights.LowerLegR * forceConversion;
    backup[16].first = lengths.HeelToToe * lengthConversion;
    backup[16].second = lengths.HeelToToeR * lengthConversion;
    backup[17].first = CGs.Foot * lengthConversion;
    backup[17].second = CGs.FootR * lengthConversion;
    backup[18].first = weights.Foot * forceConversion;
    backup[18].second = weights.FootR * forceConversion;

    //head and trunk
 
    backup[19].first = lengths.C1Tragion * lengthConversion ; //HeadL
    backup[20].first = CGs.Head * lengthConversion; //HeadCG
    backup[21].first = weights.Head * forceConversion ; //HeadWt
    backup[22].first = lengths.HeadRadius * lengthConversion ;//nasionL
    backup[23].first = lengths.C1C3 * lengthConversion ;
    backup[24].first = CGs.UpperNeck * lengthConversion; //neck1CG
    backup[25].first = weights.UpperNeck * forceConversion; //neck1Wt
    backup[26].first = lengths.C4C7 * lengthConversion ;
    backup[27].first = CGs.LowerNeck * lengthConversion; //neck2CG
    backup[28].first = weights.LowerNeck * forceConversion ; //neck2Wt
    backup[29].first = lengths.L5S1ToC7T1 * lengthConversion;
    backup[30].first = CGs.L5S1ToShoulder * lengthConversion;
    backup[31].first = weights.TorsoAndHead * forceConversion ;
    backup[32].first = lengths.HipToL5S1 * lengthConversion ;
    backup[33].first = CGs.Pelvis * lengthConversion ;
    backup[34].first = weights.Pelvis * forceConversion ;
    //backup[26].first = lengths.HipToHip * lengthConversion ;
}
void Dg_Anthropometry::UpdateStandard()
{
	Anthropometry Anthr = mDocPtr -> getAnthropometryRef();
	mnewfactor = mDocPtr -> GetFactors();
	double weight = Anthr.GetWeight(MetricCns);
	double height = Anthr.GetHeight(MetricCns);
	double gravity = mDocPtr -> getTask().getGravity();
	if (mGenderValue ==0){
		lDocPtr -> SetMaleFactors(mnewfactor);}
	else{
		lDocPtr -> SetFemaleFactors(mnewfactor);}
    //lDocPtr -> setAnthropometry(mAnthroBackup);
    lDocPtr -> Analyze();
	LinkLengths lengths = lDocPtr -> getLinkLengths();
    LinkCGs CGs = lDocPtr -> getLinkCGs();
    LinkWeights weights = lDocPtr ->getLinkWeights();
	
	
	lengths.Hand = mnewfactor.mLink_Hand * height;
	lengths.HandR = mnewfactor.mLink_Hand * height;
	lengths.HandWithFingers = mnewfactor.mLink_HandWithFinger * height;
	lengths.HandWithFingersR = mnewfactor.mLink_HandWithFinger * height;
 	lengths.Forearm = mnewfactor.mLink_Forearm * height;
	lengths.ForearmR = mnewfactor.mLink_Forearm * height;
	lengths.UpperArm = mnewfactor.mLink_Upperarm * height;
	lengths.UpperArmR = mnewfactor.mLink_Upperarm * height;
	lengths.ShoulderWidth = mnewfactor.mLink_ShoulderWidth * height;
	lengths.Neck = mnewfactor.mLink_NeckLength * height;

	// Add C1
	lengths.C1Tragion = mnewfactor.mLink_C1Tragion * height;

	// Add:
	lengths.C1C3 = mnewfactor.mLink_UpperneckLength * height;
	lengths.C4C7 = mnewfactor.mLink_LowerneckLength * height;
	lengths.HeadRadius = mnewfactor.mLink_HeadRadius * height;
	lengths.C7T1ToSCJ = mnewfactor.mLink_C7T1toSCJ * height;
	if(mnewfactor.mLink_SCJToShoulder == 0) {
		lengths.SCJToShoulder = sqrt(lengths.C7T1ToSCJ*lengths.C7T1ToSCJ +
			lengths.ShoulderWidth*lengths.ShoulderWidth/4);
	} else {
 		lengths.SCJToShoulder = mnewfactor.mLink_SCJToShoulder * height;
	}
	lengths.L5S1ToC7T1 = mnewfactor.mLink_L5S1ToC7T1 * height;
	lengths.L5S1ToIT = mnewfactor.mLink_L5S1ToIT * height;
	lengths.ITBreadth = mnewfactor.mLink_ITBreadth * height;
	lengths.HipToL5S1 = mnewfactor.mLink_HipToL5S1 * height;
	lengths.HipToHip = mnewfactor.mLink_HipWidth * height;
	lengths.UpperLeg = mnewfactor.mLink_UpperLeg * height;
	lengths.UpperLegR = mnewfactor.mLink_UpperLeg * height;
	lengths.LowerLeg = mnewfactor.mLink_LowerLeg * height;
	lengths.LowerLegR = mnewfactor.mLink_LowerLeg * height;
	lengths.Heel = mnewfactor.mLink_HeelLength * height;
	lengths.HeelToToe = mnewfactor.mLink_HeelToToe * height;
	lengths.HeelToToeR = mnewfactor.mLink_HeelToToe * height;
	lengths.FootHeight = mnewfactor.mLink_FloorToAnkle * height;
	lengths.FootBreadth = mnewfactor.mLink_FootBreadth * height;
	lengths.HeelBreadth = mnewfactor.mLink_HeelBreadth * height;
	lengths.GreaterToe = mnewfactor.mLink_GreaterToeLength * height;
	lengths.Metatarsal = mnewfactor.mLink_MetatarsalLength * height;
	lengths.LateralGreaterToe = mnewfactor.mLink_LateralGreaterToe * height;
	lengths.FifthMetatarsalLateral = mnewfactor.mLink_FifthMetatarsalLateral * height;

	lengths.SCJAngle = mnewfactor.mAngle_SCJ;
	lengths.TragionNasionANgle = mnewfactor.mAngle_Tragion_Nasion;
	lengths.HeadFlexionAngle = mnewfactor.mAngle_Head_Flexion;

	lengths.ShoeHeight = mnewfactor.mShoeHeight;
	
	
    CGs.Hand = mnewfactor.mCM_Hand * lengths.HandWithFingers;
    CGs.HandR = mnewfactor.mCM_Hand * lengths.HandWithFingersR;
	CGs.Forearm = mnewfactor.mCM_Forearm * lengths.Forearm;
    CGs.ForearmR = mnewfactor.mCM_Forearm * lengths.ForearmR;
	CGs.UpperArm = mnewfactor.mCM_Upperarm * lengths.UpperArm;
	CGs.UpperArmR = mnewfactor.mCM_Upperarm * lengths.UpperArmR;
	CGs.L5S1ToShoulder = mnewfactor.mCM_L5S1ToShoulder * lengths.L5S1ToC7T1;
	CGs.UpperNeck = mnewfactor.mCM_UpperNeck * lengths.C1C3;
	CGs.LowerNeck = mnewfactor.mCM_LowerNeck * lengths.C4C7;
	CGs.Head = mnewfactor.mCM_Head * lengths.C1Tragion;
	CGs.Pelvis = mnewfactor.mCM_Pelvis * lengths.HipToL5S1;
	CGs.UpperLeg = mnewfactor.mCM_UpperLeg * lengths.UpperLeg;
	CGs.UpperLegR = mnewfactor.mCM_UpperLeg * lengths.UpperLegR;
	CGs.LowerLeg = mnewfactor.mCM_LowerLeg * lengths.LowerLeg;
	CGs.LowerLegR = mnewfactor.mCM_LowerLeg * lengths.LowerLegR;
	CGs.Foot = mnewfactor.mCM_Foot * lengths.HeelToToe;
	CGs.FootR = mnewfactor.mCM_Foot * lengths.HeelToToeR;

	
	

	weights.Hand = mnewfactor.mMass_Hand * weight * gravity;
	weights.HandR = mnewfactor.mMass_Hand * weight * gravity;
	weights.Forearm = mnewfactor.mMass_Forearm * weight * gravity;
	weights.ForearmR = mnewfactor.mMass_Forearm * weight * gravity;
	weights.UpperArm = mnewfactor.mMass_Upperarm * weight * gravity;
	weights.UpperArmR = mnewfactor.mMass_Upperarm * weight * gravity;
	weights.Head = mnewfactor.mMass_Head * weight * gravity;
	weights.UpperNeck = mnewfactor.mMass_UpperNeck * weight * gravity;
	weights.LowerNeck = mnewfactor.mMass_LowerNeck * weight * gravity;
	weights.TorsoAndHead = mnewfactor.mMass_Torso * weight * gravity;
	weights.Pelvis = mnewfactor.mMass_Pelvis * weight * gravity;
	weights.UpperLeg = mnewfactor.mMass_UpperLeg * weight * gravity;
	weights.UpperLegR = mnewfactor.mMass_UpperLeg * weight * gravity;
	weights.LowerLeg = mnewfactor.mMass_LowerLeg * weight * gravity;
	weights.LowerLegR = mnewfactor.mMass_LowerLeg * weight * gravity;
	weights.Foot = mnewfactor.mMass_Foot * weight * gravity;
	weights.FootR = mnewfactor.mMass_Foot * weight * gravity;

	lDocPtr -> setLinkLengths(lengths);
    lDocPtr -> setLinkCGs(CGs);
    lDocPtr -> setLinkWeights(weights);
	
    standard[0] = lengths.Hand * lengthConversion;
    standard[1] = lengths.HandWithFingers * lengthConversion;
    standard[2] = CGs.Hand * lengthConversion;
    standard[3] = weights.Hand * forceConversion;
    standard[4] = lengths.Forearm * lengthConversion;
    standard[5] = CGs.Forearm * lengthConversion;
    standard[6] = weights.Forearm * forceConversion;
    standard[7] = lengths.UpperArm * lengthConversion;
    standard[8] = CGs.UpperArm * lengthConversion;
    standard[9] = weights.UpperArm * forceConversion;
    standard[10] = lengths.UpperLeg * lengthConversion;
    standard[11] = CGs.UpperLeg * lengthConversion;
    standard[12] = weights.UpperLeg * forceConversion;
    standard[13] = lengths.LowerLeg * lengthConversion;
    standard[14] = CGs.LowerLeg * lengthConversion;
    standard[15] = weights.LowerLeg * forceConversion;
    standard[16] = lengths.HeelToToe * lengthConversion;
    standard[17] = CGs.Foot * lengthConversion;
    standard[18] = weights.Foot * forceConversion;

    //head and trunk
    standard[19] = lengths.C1Tragion * lengthConversion; //HeadL
    standard[20] = CGs.Head * lengthConversion; //HeadCG
    standard[21] = weights.Head * forceConversion; //HeadWt
    standard[22] = lengths.HeadRadius * lengthConversion ;
    standard[23] = lengths.C1C3 * lengthConversion ;
    standard[24] = CGs.UpperNeck * lengthConversion; //neck1CG
    standard[25] = weights.UpperNeck * forceConversion; //neck1Wt
    standard[26] = lengths.C4C7 * lengthConversion ;
    standard[27] = CGs.LowerNeck * lengthConversion; //neck2CG
    standard[28] = weights.LowerNeck * forceConversion; //neck2Wt
    standard[29] = lengths.L5S1ToC7T1 * lengthConversion ;
    standard[30] = CGs.L5S1ToShoulder * lengthConversion;
    standard[31] = weights.TorsoAndHead * forceConversion;
    standard[32] = lengths.HipToL5S1 * lengthConversion ;
    standard[33] = CGs.Pelvis * lengthConversion ;
    standard[34] = weights.Pelvis * forceConversion ;
    //standard[35] = lengths.HipToHip * lengthConversion ;
    
}
void Dg_Anthropometry::ApplyCustomChanges(UINT ctrlID)
{
	applyClicked = false;
	CString s;
	GetDlgItemTextA(ctrlID, s);
	double newValue = atof(s);

	C_Hom_Doc * pDoc = Get_App_Ptr()->Get_Document_Ptr();
	Anthropometry& anthropometry = pDoc->getTask().getAnthropometryRef();
	LinkWeights & anthroNewWeights = anthropometry.getNewWeights();

	switch(ctrlID)
	{
	case IDC_HeadL:
		//wait for more data for head
		newLengths.C1Tragion = newValue / lengthConversion;
		break;
	case IDC_HeadCG:
		newCGs.Head = newValue / lengthConversion;
		break;
	case IDC_HeadWt:
		newWeights.Head = newValue / forceConversion;
		anthropometry.getNewWeights().Head = newWeights.Head;
		break;
	case IDC_FootL:
		if (left)
			newLengths.HeelToToe = newValue / lengthConversion;
		else
			newLengths.HeelToToeR = newValue / lengthConversion;
		break;
	case IDC_FootWt:
		if (left) {
			newWeights.Foot = newValue / forceConversion;
			anthropometry.getNewWeights().Foot = newWeights.Foot;
		} else {
			newWeights.FootR = newValue / forceConversion;
			anthropometry.getNewWeights().FootR = newWeights.FootR;
		}
		break;
	case IDC_FootCG:
		if (left)
			newCGs.Foot = newValue / lengthConversion;
		else
			newCGs.FootR = newValue / lengthConversion;
		break;
	case IDC_HipToL5L:
		newLengths.HipToL5S1 = newValue / lengthConversion;
		break;
	case IDC_HipToL5Wt:
		newWeights.Pelvis = newValue / forceConversion;
		anthropometry.getNewWeights().Pelvis = newWeights.Pelvis;
		break;
	case IDC_HipToL5CG:
		newCGs.Pelvis = newValue / lengthConversion;
		break;
	case IDC_L5ToShoulderL:
		newLengths.L5S1ToC7T1 = newValue / lengthConversion;
		break;
	case IDC_L5ToShoulderCG:
		newCGs.L5S1ToShoulder = newValue / lengthConversion;
		break;
	case IDC_L5ToShoulderWt:
		newWeights.TorsoAndHead = newValue / forceConversion;
		anthropometry.getNewWeights().TorsoAndHead = newWeights.TorsoAndHead;
		break;
	case IDC_NasionL:
		newLengths.HeadRadius = newValue / lengthConversion;
		break;
	case IDC_Neck1L:
		newLengths.C1C3 = newValue / lengthConversion;
		break;
	case IDC_Neck1Wt:
		newWeights.UpperNeck = newValue / forceConversion;
		anthropometry.getNewWeights().UpperNeck = newWeights.UpperNeck;
		break;
	case IDC_Neck1CG:
		newCGs.UpperNeck = newValue / lengthConversion;
		break;
	case IDC_Neck2L:
		newLengths.C4C7 = newValue / lengthConversion;
		break;
	case IDC_Neck2Wt:
		newWeights.LowerNeck = newValue / forceConversion;
		anthropometry.getNewWeights().LowerNeck = newWeights.LowerNeck;
		break;
	case IDC_Neck2CG:
		newCGs.LowerNeck = newValue / lengthConversion; 
		break;
	case IDC_LowerArmL:
		if (left)
			newLengths.Forearm = newValue / lengthConversion;
		else
			newLengths.ForearmR = newValue / lengthConversion;
		break;
	case IDC_LowerArmWt:
		if (left) {
			newWeights.Forearm = newValue / forceConversion;
			anthropometry.getNewWeights().Forearm = newWeights.Forearm;
		} else {
			newWeights.ForearmR = newValue / forceConversion;
			anthropometry.getNewWeights().ForearmR = newWeights.ForearmR;
		}
		break;
	case IDC_LowerArmCG:
		if (left)
			newCGs.Forearm = newValue / lengthConversion;
		else
			newCGs.ForearmR = newValue / lengthConversion;
		break;
	case IDC_LowerLegL:
		if (left)
			newLengths.LowerLeg = newValue / lengthConversion;
		else
			newLengths.LowerLegR = newValue / lengthConversion;
		break;
	case IDC_LowerLegCG:
		if (left)
			newCGs.LowerLeg = newValue / lengthConversion;
		else
			newCGs.LowerLegR = newValue / lengthConversion;
		break;
	case IDC_LowerLegWt:
		if (left) {
			newWeights.LowerLeg = newValue / forceConversion;
			anthropometry.getNewWeights().LowerLeg = newWeights.LowerLeg;
		} else {
			newWeights.LowerLegR = newValue / forceConversion;
			anthropometry.getNewWeights().LowerLegR = newWeights.LowerLegR;
		}
		break;
	case IDC_UpperArmL:
		if (left)
			newLengths.UpperArm = newValue / lengthConversion;
		else 
			newLengths.UpperArmR = newValue / lengthConversion;
		break;
	case IDC_UpperArmCG:
		if (left)
			newCGs.UpperArm = newValue / lengthConversion;
		else
			newCGs.UpperArmR = newValue / lengthConversion;
		break;
	case IDC_UpperArmWt:
		if (left) {
			newWeights.UpperArm = newValue / forceConversion;
			anthropometry.getNewWeights().UpperArm = newWeights.UpperArm;
		} else {
			newWeights.UpperArmR = newValue / forceConversion;
			anthropometry.getNewWeights().UpperArmR = newWeights.UpperArmR;
		}
		break;
	case IDC_UpperLegL:
		if (left)
			newLengths.UpperLeg = newValue / lengthConversion;
		else
			newLengths.UpperLegR = newValue / lengthConversion;
		break;
	case IDC_UpperLegCG:
		if (left)
			newCGs.UpperLeg = newValue / lengthConversion;
		else
			newCGs.UpperLegR = newValue / lengthConversion;
		break;
	case IDC_UpperLegWt:
		if (left) {
			newWeights.UpperLeg = newValue / forceConversion;
			anthropometry.getNewWeights().UpperLeg = newWeights.UpperLeg;
		} else {
			newWeights.UpperLegR = newValue / forceConversion;
			anthropometry.getNewWeights().UpperLegR = newWeights.UpperLegR;
		}
		break;
	/* case IDC_HipToHipL:
		newLengths.HipToHip = newValue / lengthConversion;
		break;*/
	case IDC_HandL:
		if (left)
			newLengths.Hand = newValue / lengthConversion;
		else
			newLengths.HandR = newValue / lengthConversion;
		break;
	case IDC_HandWithFingersL:
		if (left)
			newLengths.HandWithFingers = newValue / lengthConversion;
		else
			newLengths.HandWithFingersR = newValue / lengthConversion;
		break;
	case IDC_HandWithFingersCG: //same as HandCG
		if (left)
			newCGs.Hand = newValue / lengthConversion;
		else
			newCGs.HandR = newValue / lengthConversion;
		break;
	case IDC_HandWithFingersWt: //same as HandWt
		if (left) {
			newWeights.Hand = newValue / forceConversion;
			anthropometry.getNewWeights().Hand = newWeights.Hand;
		} else {
			newWeights.HandR = newValue / forceConversion;
			anthropometry.getNewWeights().HandR = newWeights.HandR;
		}
		break;
	}

	// get "apply" button
	CButton * apply = (CButton*)GetDlgItem(IDAPPLY);
	// make it clickable 
	apply -> EnableWindow(TRUE);
	// get "ok" button
	CButton * ok = (CButton*)GetDlgItem(IDOK);
	// make it clickable
	ok -> EnableWindow(TRUE);
	custom[ctrlID - IDC_HandL] = newValue;
	UpdateData(true);
	CEdit* a = (CEdit*)GetDlgItem(ctrlID);
	a -> RedrawWindow();

}

void Dg_Anthropometry::UpdateDisplay()
{
    
    mGenderValue = mDocPtr->getAnthropometryRef().Sex();

    mPercentileValue =  mDocPtr->getAnthropometryRef().Percentile();//isn't set to new one
       
    mHeightValue = Round_Double(mDocPtr->getAnthropometryRef().Height(),1);
    mHeightUnits = mDocPtr->getAnthropometryRef().HeightUnitsText();

    mWeightValue = Round_Double(mDocPtr->getAnthropometryRef().Weight(), 1);
    mWeightUnits = mDocPtr->getAnthropometryRef().WeightUnitsText();

	double shoeHeight = mDocPtr->GetFactors(mGenderValue).mShoeHeight;
	if(mDocPtr->Is_English()) {
		shoeHeight *= CM_TO_IN;
	}
	mShoeCheck = (shoeHeight != 0);
	mShoeHeightControl.EnableWindow(mShoeCheck);
}

bool Dg_Anthropometry::UpdateDocument()
{
	String lErrorMessage;
    bool lUpdateOK = true;
	Anthropometry anthro = mDocPtr->getAnthropometryRef();
	anthro.ClearLastErrorMessage();
	UpdateData();
	if (mGenderValue == 1)
	{
		if(!anthro.fHeight(mHeightValue))
		{
			AfxMessageBox((char*)(anthro.feHeight.mLastErrorMessage));
			lUpdateOK = false;
			mDocPtr->setAnthropometry(anthro);
		}
		if(!anthro.fWeight(mWeightValue))
		{//error with percentile going back after entering invalid height/weight and trying a different percentile
			AfxMessageBox((char*)(anthro.feWeight.mLastErrorMessage)); 
			lUpdateOK = false;
			mDocPtr->setAnthropometry(anthro);
		}
	}
	else if(mGenderValue == 0)
	{
    if(!anthro.Sex(mGenderValue))
    {
        //AfxMessageBox((char*)(mDocPtr->getAnthropometryRef().LastErrorMessage()));
		AfxMessageBox((char*)(anthro.LastErrorMessage()));
	    lUpdateOK = false;
		mDocPtr->setAnthropometry(anthro);
    }

    if(!anthro.Height(mHeightValue))
    {
        AfxMessageBox((char*)(anthro.LastErrorMessage()));
        lUpdateOK = false;
		mDocPtr->setAnthropometry(anthro);
    }

    if(!anthro.Weight(mWeightValue))
    {//error with percentile going back after entering invalid height/weight and trying a different percentile
        AfxMessageBox((char*)(anthro.LastErrorMessage())); 
	    lUpdateOK = false;
		mDocPtr->setAnthropometry(anthro);
    }

    if(!anthro.Percentile(mPercentileValue))
    {
        AfxMessageBox((char*)(anthro.LastErrorMessage()));
	    lUpdateOK = false;
		mDocPtr->setAnthropometry(anthro);
    }
	}
	if(lUpdateOK) 
	{
		mDocPtr->setAnthropometry(anthro);
	}
    return lUpdateOK;
}

void Dg_Anthropometry::OnPercentile95th() {
	dataEntry = false;
	OnPercentile5th_50th_95th();
	OnRedraw();
}

void Dg_Anthropometry::OnPercentile50th() { 
	dataEntry = false;
	OnPercentile5th_50th_95th();
	OnRedraw();
}

void Dg_Anthropometry::OnPercentile5th(void) { 
	dataEntry = false;
	OnPercentile5th_50th_95th();
	OnRedraw();

}

void Dg_Anthropometry::OnPercentileDataEntry(void) { 
    this->OnPercentile95th();
    dataEntry = true;

	// enable "apply height & body weight"
	CButton * update = (CButton*)GetDlgItem(IDC_REDRAW);
	update -> EnableWindow(TRUE);
}

void Dg_Anthropometry::OnGenderMale() {
    updateClicked = false;
	OnPercentile5th_50th_95th();
	if(!dataEntry) {
		OnRedraw();
	} else {
		// enable "apply height & body weight"
		CButton * update = (CButton*)GetDlgItem(IDC_REDRAW);
		update -> EnableWindow(TRUE);
	}

	// if (modifyAnthro) {
	CButton * apply = (CButton*)GetDlgItem(IDAPPLY);
	apply -> EnableWindow(TRUE);
	// }
	// OnRedraw();
}
void Dg_Anthropometry::OnGenderFemale() {
    updateClicked = false;
	OnPercentile5th_50th_95th();
	if(!dataEntry) {
		OnRedraw();
	} else {
		// enable "apply height & body weight"
		CButton * update = (CButton*)GetDlgItem(IDC_REDRAW);
		update -> EnableWindow(TRUE);
	}


	// if (modifyAnthro) {
	CButton * apply = (CButton*)GetDlgItem(IDAPPLY);
	apply -> EnableWindow(TRUE);
	// }
	// OnRedraw();
}

void Dg_Anthropometry::UpdateReport()
{
	/*mAnthroBackup = mDocPtr -> getAnthropometryRef();
    lDocPtr =  new C_Hom_Doc();  
    lDocPtr -> setAnthropometry(mAnthroBackup);
    lDocPtr -> Analyze();*/
   const LinkLengths& lengths = mDocPtr->getLinkLengths();
   const LinkWeights& weights = mDocPtr->getLinkWeights();
   const LinkCGs& CGs = mDocPtr->getLinkCGs();
	mLengthLabel = (("Length (")+ mDocPtr->LengthUnits() + ")");
	mLengthLabel2 = (("Length (")+ mDocPtr->LengthUnits() + ")");
	mCGLabel = CString("CG-to-Proximal End\nDistance (")
			+ mDocPtr->LengthUnits() + ")";
	mCGLabel2 = CString("CG-to-Proximal End\nDistance (")
			+ mDocPtr->LengthUnits() + ")";
	mWeightLabel = CString("Weight (")+ mDocPtr->ForceUnits() + ")";
	mWeightLabel2 = CString("Weight (")+ mDocPtr->ForceUnits() + ")";
	mWeightLabel3 = CString("Weight (")+ mDocPtr->ForceUnits() + ")";

	Factors mFactors;
	if(mGenderValue == 0)
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


   // TODO unit conversions
   if (left) 
   {
       mHandL = Round_Double(lengthConversion * lengths.Hand, 1);//mDocPtr->getHandL().ValueText();
		if(mHandWithFingersL != Round_Double(backl[1],1) && mHandWithFingersCG == Round_Double(backl[2],1))
	   {    //if user only change the length value or change them both ,then CG should be changed correspondlingly 
		   mHandWithFingersL = Round_Double(lengthConversion * lengths.HandWithFingers, 1);
		   mHandWithFingersCG = Round_Double(mHandWithFingersL * mCM_Hand, 1);
		   backl[1] = mHandWithFingersL;
		   backl[2] = mHandWithFingersCG;
		   newCGs.Hand = mHandWithFingersCG/lengthConversion;
		   mDocPtr -> setLinkCGs(newCGs);
	   }
		else  //if user only change the CG or does not change anything ,then let the value be what the user input.This means changing the CG will not lead to the adjustment of its length,however.
		{
			mHandWithFingersCG = Round_Double(lengthConversion * CGs.Hand, 1);//mDocPtr->getHandCG().ValueText(); 
		    mHandWithFingersL = Round_Double(lengthConversion * lengths.HandWithFingers, 1);//mDocPtr->getHandWithFingersL().ValueText();
			backl[1] = mHandWithFingersL;
			backl[2] = mHandWithFingersCG;
		}
		
       mHandWithFingersWt = Round_Double(forceConversion * weights.Hand, 1);//mDocPtr->getHandWt().ValueText();
	   		if(mLowerArmL != Round_Double(backl[4],1) && mLowerArmCG == Round_Double(backl[5],1))
	   {
		   mLowerArmL = Round_Double(lengthConversion * lengths.Forearm, 1);//mDocPtr->getHandWithFingersL().ValueText();
		   mLowerArmCG = Round_Double(mLowerArmL * mCM_Forearm, 1);
		   backl[4] = mLowerArmL;
		   backl[5] = mLowerArmCG;
		   newCGs.Forearm = mLowerArmCG/lengthConversion;
		   mDocPtr -> setLinkCGs(newCGs);
	   }
		else
		{
			mLowerArmCG = Round_Double(lengthConversion * CGs.Forearm, 1);//mDocPtr->getHandCG().ValueText(); 
		    mLowerArmL = Round_Double(lengthConversion * lengths.Forearm, 1);//mDocPtr->getHandWithFingersL().ValueText();
			backl[4] = mLowerArmL;
			backl[5] = mLowerArmCG;
		}
       /*mLowerArmCG = Round_Double(lengthConversion * CGs.Forearm, 1);//mDocPtr->getLowerArmCG().ValueText();
       mLowerArmL = Round_Double(lengthConversion * lengths.Forearm, 1);//mDocPtr->getLowerArmL().ValueText();
	   if(isred[4] == true)
		   mLowerArmCG = Round_Double(mLowerArmL * mCM_Forearm, 1);*/
       mLowerArmWt = Round_Double(forceConversion * weights.Forearm, 1);//mDocPtr->getLowerArmWt().ValueText();

	   if(mLowerLegL != Round_Double(backl[13],1) && mLowerLegCG == Round_Double(backl[14],1))
	   {
		   mLowerLegL = Round_Double(lengthConversion * lengths.LowerLeg, 1);//mDocPtr->getHandWithFingersL().ValueText();
		   mLowerLegCG = Round_Double(mLowerLegL * mCM_LowerLeg, 1);
		   backl[13] = mLowerLegL;
		   backl[14] = mLowerLegCG;
		   newCGs.LowerLeg = mLowerLegCG/lengthConversion;
		   mDocPtr -> setLinkCGs(newCGs);
	   }
		else
		{
			mLowerLegCG = Round_Double(lengthConversion * CGs.LowerLeg, 1);//mDocPtr->getHandCG().ValueText(); 
		    mLowerLegL = Round_Double(lengthConversion * lengths.LowerLeg, 1);//mDocPtr->getHandWithFingersL().ValueText();
			backl[13] = mLowerLegL;
			backl[14] = mLowerLegCG;
		}

      /* mLowerLegCG = Round_Double(lengthConversion * CGs.LowerLeg, 1);//mDocPtr->getLowerLegCG().ValueText();
       mLowerLegL = Round_Double(lengthConversion * lengths.LowerLeg, 1);//mDocPtr->getLowerLegL().ValueText();
	   if(isred[13] == true)
		   mLowerLegCG = Round_Double(mLowerLegL * mCM_LowerLeg, 1);*/
       mLowerLegWt = Round_Double(forceConversion * weights.LowerLeg, 1);//mDocPtr->getLowerLegWt().ValueText();
	   	   if(mUpperArmL != Round_Double(backl[7],1) && mUpperArmCG == Round_Double(backl[8],1))
	   {
		   mUpperArmL = Round_Double(lengthConversion * lengths.UpperArm, 1);//mDocPtr->getHandWithFingersL().ValueText();
		   mUpperArmCG = Round_Double(mUpperArmL * mCM_Upperarm, 1);
		   backl[7] = mUpperArmL;
		   backl[8] = mUpperArmCG;
		   newCGs.UpperArm = mUpperArmCG/lengthConversion;
		   mDocPtr -> setLinkCGs(newCGs);
	   }
		else
		{
			mUpperArmCG = Round_Double(lengthConversion * CGs.UpperArm, 1);//mDocPtr->getHandCG().ValueText(); 
		    mUpperArmL = Round_Double(lengthConversion * lengths.UpperArm, 1);//mDocPtr->getHandWithFingersL().ValueText();
			backl[7] = mUpperArmL;
			backl[8] = mUpperArmCG;
		}

       /*mUpperArmCG = Round_Double(lengthConversion * CGs.UpperArm, 1);//mDocPtr->getUpperArmCG().ValueText();
       mUpperArmL = Round_Double(lengthConversion * lengths.UpperArm, 1);//mDocPtr->getUpperArmL().ValueText();
	   if(isred[7] == true)
		   mUpperArmCG = Round_Double( mUpperArmL * mCM_Upperarm, 1);*/
       mUpperArmWt = Round_Double(forceConversion * weights.UpperArm, 1);//mDocPtr->getUpperArmWt().ValueText()
	   	   	   if((mUpperLegL != Round_Double(backl[10],1) && mUpperLegCG == Round_Double(backl[11],1)))
	   {
		   mUpperLegL = Round_Double(lengthConversion * lengths.UpperLeg, 1);//mDocPtr->getHandWithFingersL().ValueText();
		   mUpperLegCG = Round_Double(mUpperLegL * mCM_UpperLeg, 1);
		   backl[10] = mUpperLegL;
		   backl[11] = mUpperLegCG;
		   newCGs.UpperLeg = mUpperLegCG/lengthConversion;
		   mDocPtr -> setLinkCGs(newCGs);
	   }
		else
		{
			mUpperLegCG = Round_Double(lengthConversion * CGs.UpperLeg, 1);//mDocPtr->getHandCG().ValueText(); 
		    mUpperLegL = Round_Double(lengthConversion * lengths.UpperLeg, 1);//mDocPtr->getHandWithFingersL().ValueText();
			backl[10] = mUpperLegL;
			backl[11] = mUpperLegCG;
		}
       /*mUpperLegCG = Round_Double(lengthConversion * CGs.UpperLeg, 1);//mDocPtr->getUpperLegCG().ValueText();
       mUpperLegL = Round_Double(lengthConversion * lengths.UpperLeg, 1);//mDocPtr->getUpperLegL().ValueText();
	   if(isred[10] == true)
		   mUpperLegCG = Round_Double(mUpperLegL * mCM_UpperLeg, 1);*/
       mUpperLegWt = Round_Double(forceConversion * weights.UpperLeg, 1);//mDocPtr->getUpperLegWt().ValueText();
	   if(mFootL != Round_Double(backl[16],1) && mFootCG == Round_Double(backl[17],1))
	   {
		   mFootL = Round_Double(lengthConversion * lengths.HeelToToe, 1);//mDocPtr->getHandWithFingersL().ValueText();
		   mFootCG = Round_Double(mFootL * mCM_Foot, 1);
		   backl[16] = mFootL;
		   backl[17] = mFootCG;
		   newCGs.Foot = mFootCG/lengthConversion;
		   mDocPtr -> setLinkCGs(newCGs);
	   }
		else
		{
			mFootCG = Round_Double(lengthConversion * CGs.Foot, 1);//mDocPtr->getHandCG().ValueText(); 
		    mFootL = Round_Double(lengthConversion * lengths.HeelToToe, 1);//mDocPtr->getHandWithFingersL().ValueText();
			backl[16] = mFootL;
			backl[17] = mFootCG;
		}
      /* mFootCG = Round_Double(lengthConversion * CGs.Foot, 1);
       mFootL = Round_Double(lengthConversion * lengths.HeelToToe, 1);//mDocPtr->getFootL().ValueText();
	   if(isred[16] == true)
		   mFootCG = Round_Double(mFootL * mCM_Foot, 1);*/
       mFootWt = Round_Double(forceConversion * weights.Foot, 1);//mDocPtr->getFootWt().ValueText();

   }
   else
   {
        mHandL = Round_Double(lengthConversion * lengths.HandR, 1);//mDocPtr->getHandL().ValueText();
		if(mHandWithFingersL != Round_Double(backr[1],1) && mHandWithFingersCG == Round_Double(backr[2],1))
	   {    //if user only change the length value or change them both ,then CG should be changed correspondlingly 
		   mHandWithFingersL = Round_Double(lengthConversion * lengths.HandWithFingersR, 1);
		   mHandWithFingersCG = Round_Double(mHandWithFingersL * mCM_Hand, 1);
		   backr[1] = mHandWithFingersL;
		   backr[2] = mHandWithFingersCG;
		   newCGs.HandR = mHandWithFingersCG/lengthConversion;
		   mDocPtr -> setLinkCGs(newCGs);
	   }
		else  //if user only change the CG or does not change anything ,then let the value be what the user input.This means changing the CG will not lead to the adjustment of its length,however.
		{
			mHandWithFingersCG = Round_Double(lengthConversion * CGs.HandR, 1);//mDocPtr->getHandCG().ValueText(); 
		    mHandWithFingersL = Round_Double(lengthConversion * lengths.HandWithFingersR, 1);//mDocPtr->getHandWithFingersL().ValueText();
			backr[1] = mHandWithFingersL;
			backr[2] = mHandWithFingersCG;
		}
        /*mHandWithFingersCG = Round_Double(lengthConversion * CGs.HandR, 1);//mDocPtr->getHandCG().ValueText();
        mHandWithFingersL = Round_Double(lengthConversion * lengths.HandWithFingersR, 1);//mDocPtr->getHandWithFingersL().ValueText();
		if(isred[1] == true)
		   mHandWithFingersCG = Round_Double(mHandWithFingersL * mCM_Hand, 1);*/
        mHandWithFingersWt = Round_Double(forceConversion * weights.HandR, 1);//mDocPtr->getHandWt().ValueText();
			   		if(mLowerArmL != Round_Double(backr[4],1) && mLowerArmCG == Round_Double(backr[5],1))
	   {
		   mLowerArmL = Round_Double(lengthConversion * lengths.ForearmR, 1);//mDocPtr->getHandWithFingersL().ValueText();
		   mLowerArmCG = Round_Double(mLowerArmL * mCM_Forearm, 1);
		   backr[4] = mLowerArmL;
		   backr[5] = mLowerArmCG;
		   newCGs.ForearmR = mLowerArmCG/lengthConversion;
		   mDocPtr -> setLinkCGs(newCGs);
	   }
		else
		{
			mLowerArmCG = Round_Double(lengthConversion * CGs.ForearmR, 1);//mDocPtr->getHandCG().ValueText(); 
		    mLowerArmL = Round_Double(lengthConversion * lengths.ForearmR, 1);//mDocPtr->getHandWithFingersL().ValueText();
			backr[4] = mLowerArmL;
			backr[5] = mLowerArmCG;
		}
        /*mLowerArmCG = Round_Double(lengthConversion * CGs.ForearmR, 1);//mDocPtr->getLowerArmCG().ValueText();
        mLowerArmL = Round_Double(lengthConversion * lengths.ForearmR, 1);//mDocPtr->getLowerArmL().ValueText();
		if(isred[4] == true)
		   mLowerArmCG = Round_Double(mLowerArmL * mCM_Forearm, 1);*/
        mLowerArmWt = Round_Double(forceConversion * weights.ForearmR, 1);//mDocPtr->getLowerArmWt().ValueText();
			   if(mLowerLegL != Round_Double(backr[13],1) && mLowerLegCG == Round_Double(backr[14],1))
	   {
		   mLowerLegL = Round_Double(lengthConversion * lengths.LowerLegR, 1);//mDocPtr->getHandWithFingersL().ValueText();
		   mLowerLegCG = Round_Double(mLowerLegL * mCM_LowerLeg, 1);
		   backr[13] = mLowerLegL;
		   backr[14] = mLowerLegCG;
		   newCGs.LowerLegR = mLowerLegCG/lengthConversion;
		   mDocPtr -> setLinkCGs(newCGs);
	   }
		else
		{
			mLowerLegCG = Round_Double(lengthConversion * CGs.LowerLegR, 1);//mDocPtr->getHandCG().ValueText(); 
		    mLowerLegL = Round_Double(lengthConversion * lengths.LowerLegR, 1);//mDocPtr->getHandWithFingersL().ValueText();
			backr[13] = mLowerLegL;
			backr[14] = mLowerLegCG;
		}
        /*mLowerLegCG = Round_Double(lengthConversion * CGs.LowerLegR, 1);//mDocPtr->getLowerLegCG().ValueText();
        mLowerLegL = Round_Double(lengthConversion * lengths.LowerLegR, 1);//mDocPtr->getLowerLegL().ValueText();
		if(isred[13] == true)
		   mLowerLegCG = Round_Double(mLowerLegL * mCM_LowerLeg, 1);*/
        mLowerLegWt = Round_Double(forceConversion * weights.LowerLegR, 1);//mDocPtr->getLowerLegWt().ValueText();
			   	   if(mUpperArmL != Round_Double(backr[7],1) && mUpperArmCG == Round_Double(backr[8],1))
	   {
		   mUpperArmL = Round_Double(lengthConversion * lengths.UpperArmR, 1);//mDocPtr->getHandWithFingersL().ValueText();
		   mUpperArmCG = Round_Double(mUpperArmL * mCM_Upperarm, 1);
		   backr[7] = mUpperArmL;
		   backr[8] = mUpperArmCG;
		   newCGs.UpperArmR = mUpperArmCG/lengthConversion;
		   mDocPtr -> setLinkCGs(newCGs);
	   }
		else
		{
			mUpperArmCG = Round_Double(lengthConversion * CGs.UpperArmR, 1);//mDocPtr->getHandCG().ValueText(); 
		    mUpperArmL = Round_Double(lengthConversion * lengths.UpperArmR, 1);//mDocPtr->getHandWithFingersL().ValueText();
			backr[7] = mUpperArmL;
			backr[8] = mUpperArmCG;
		}

        /*mUpperArmCG = Round_Double(lengthConversion * CGs.UpperArmR, 1);//mDocPtr->getUpperArmCG().ValueText();
        mUpperArmL = Round_Double(lengthConversion * lengths.UpperArmR, 1);//mDocPtr->getUpperArmL().ValueText();
		if(isred[7] == true)
		   mUpperArmCG = Round_Double( mUpperArmL * mCM_Upperarm, 1);*/
        mUpperArmWt = Round_Double(forceConversion * weights.UpperArmR, 1);//mDocPtr->getUpperArmWt().ValueText();
			   	   	   if((mUpperLegL != Round_Double(backr[10],1) && mUpperLegCG == Round_Double(backr[11],1)))
	   {
		   mUpperLegL = Round_Double(lengthConversion * lengths.UpperLegR, 1);//mDocPtr->getHandWithFingersL().ValueText();
		   mUpperLegCG = Round_Double(mUpperLegL * mCM_UpperLeg, 1);
		   backr[10] = mUpperLegL;
		   backr[11] = mUpperLegCG;
		   newCGs.UpperLegR = mUpperLegCG/lengthConversion;
		   mDocPtr -> setLinkCGs(newCGs);

	   }
		else
		{
			mUpperLegCG = Round_Double(lengthConversion * CGs.UpperLegR, 1);//mDocPtr->getHandCG().ValueText(); 
		    mUpperLegL = Round_Double(lengthConversion * lengths.UpperLegR, 1);//mDocPtr->getHandWithFingersL().ValueText();
			backr[10] = mUpperLegL;
			backr[11] = mUpperLegCG;
		}
       /* mUpperLegCG = Round_Double(lengthConversion * CGs.UpperLegR, 1);//mDocPtr->getUpperLegCG().ValueText();
        mUpperLegL = Round_Double(lengthConversion * lengths.UpperLegR, 1);//mDocPtr->getUpperLegL().ValueText();
		if(isred[10] == true)
		   mUpperLegCG = Round_Double(mUpperLegL * mCM_UpperLeg, 1);*/
        mUpperLegWt = Round_Double(forceConversion * weights.UpperLegR, 1);//mDocPtr->getUpperLegWt().ValueText();
			   	   	   	   if(mFootL != Round_Double(backr[16],1) && mFootCG == Round_Double(backr[17],1))
	   {
		   mFootL = Round_Double(lengthConversion * lengths.HeelToToeR, 1);//mDocPtr->getHandWithFingersL().ValueText();
		   mFootCG = Round_Double(mFootL * mCM_Foot, 1);
		   backr[16] = mFootL;
		   backr[17] = mFootCG;
		   newCGs.FootR = mFootCG/lengthConversion;
		   mDocPtr -> setLinkCGs(newCGs);
	   }
		else
		{
			mFootCG = Round_Double(lengthConversion * CGs.FootR, 1);//mDocPtr->getHandCG().ValueText(); 
		    mFootL = Round_Double(lengthConversion * lengths.HeelToToeR, 1);//mDocPtr->getHandWithFingersL().ValueText();
			backr[16] = mFootL;
			backr[17] = mFootCG;
		}
       /* mFootCG = Round_Double(lengthConversion * CGs.FootR, 1);
        mFootL = Round_Double(lengthConversion * lengths.HeelToToeR, 1);
		if(isred[16] == true)
		   mFootCG = Round_Double(mFootL * mCM_Foot, 1);*/
        mFootWt = Round_Double(forceConversion * weights.FootR, 1);//mDocPtr->getFootWt().ValueText();

   }
//mDocPtr->getFootCG().ValueText();
  
  
  /* mHipToL5CG = Round_Double(lengthConversion * CGs.Pelvis, 1);//mDocPtr->getHipToL5CG().ValueText();

   mHipToL5L = Round_Double(lengthConversion * lengths.HipToL5S1, 1);//mDocPtr->getHipToL5L().ValueText();
   if(isred[32] == true)
	   mHipToL5CG = Round_Double(mHipToL5L * mCM_Pelvis, 1);*/
   if(mHipToL5L != Round_Double(backl[32],1) && mHipToL5CG == Round_Double(backl[33],1))
	   {
		   mHipToL5L = Round_Double(lengthConversion * lengths.HipToL5S1, 1);//mDocPtr->getHandWithFingersL().ValueText();
		   mHipToL5CG = Round_Double(mHipToL5L * mCM_Pelvis, 1);
		   backl[32] = mHipToL5L;
		   backl[33] = mHipToL5CG;
		   newCGs.Pelvis = mHipToL5CG/lengthConversion;
		   mDocPtr -> setLinkCGs(newCGs);
	   }
		else
		{
			mHipToL5CG = Round_Double(lengthConversion * CGs.Pelvis, 1);//mDocPtr->getHandCG().ValueText(); 
		    mHipToL5L = Round_Double(lengthConversion * lengths.HipToL5S1, 1);//mDocPtr->getHandWithFingersL().ValueText();
			backl[32] = mHipToL5L;
			backl[33] = mHipToL5CG;
		}
   mHipToL5Wt = Round_Double(forceConversion * weights.Pelvis, 1);//mDocPtr->getHipToL5Wt().ValueText();
   //mHipToHipL = Round_Double(lengthConversion * lengths.HipToHip, 1);//mDocPtr->getHipToHipL().ValueText();
   
   //mHipToHipWt = mDocPtr->getHipToHipWt().ValueText();

   //mHipToShoulderL.Format("%.1f",lengthConversion * lengths.HipToShoulder);//mDocPtr->getHipToShoulderL().ValueText();
   //mHipToShoulderWt.Format("%.1f",forceConversion * weights.TorsoAndHead + forceConversion * weights.Pelvis);//mDocPtr->getHipToShoulderWt().ValueText();
   mHeadWt = Round_Double(forceConversion * weights.Head, 1);
  /* mHeadL = Round_Double(lengthConversion * lengths.HeadRadius, 1);
   mHeadCG = Round_Double(lengthConversion * CGs.Head, 1);
   if(isred[19] == true)
	   mHeadCG = Round_Double(mHeadL * mCM_Head, 1);*/
      if(mHeadL != Round_Double(backl[19],1) && mHeadCG == Round_Double(backl[20],1))
	   {
		   mHeadL = Round_Double(lengthConversion * lengths.C1Tragion, 1);//mDocPtr->getHandWithFingersL().ValueText();
		   mHeadCG = Round_Double(mHeadL * mCM_Head, 1);
		   backl[19] = mHeadL;
		   backl[20] = mHeadCG;
		   newCGs.Head = mHeadCG/lengthConversion;
		   mDocPtr -> setLinkCGs(newCGs);
	   }
		else
		{
			mHeadCG = Round_Double(lengthConversion * CGs.Head, 1);//mDocPtr->getHandCG().ValueText(); 
		    mHeadL = Round_Double(lengthConversion * lengths.C1Tragion, 1);//mDocPtr->getHandWithFingersL().ValueText();
			backl[19] = mHeadL;
			backl[20] = mHeadCG;
		}
	  mL5ToShoulderWt = Round_Double(forceConversion * weights.TorsoAndHead, 1);//need to get just torso weight

  /* mL5ToShoulderL = Round_Double(lengthConversion * lengths.L5S1ToShoulder, 1);//mDocPtr->getL5ToShoulderL().ValueText();
   mL5ToShoulderCG = Round_Double(lengthConversion * CGs.L5S1ToShoulder, 1);
   if(isred[29] == true)
	   mL5ToShoulderCG = Round_Double(mL5ToShoulderL * mCM_L5S1ToShoulder, 1);*/
      if(mL5ToShoulderL != Round_Double(backl[29],1) && mL5ToShoulderCG == Round_Double(backl[30],1))
	   {
		   mL5ToShoulderL = Round_Double(lengthConversion * lengths.L5S1ToC7T1, 1);//mDocPtr->getHandWithFingersL().ValueText();
		   mL5ToShoulderCG = Round_Double(mL5ToShoulderL * mCM_L5S1ToShoulder, 1);
		   backl[29] = mL5ToShoulderL;
		   backl[30] = mL5ToShoulderCG;
		   newCGs.L5S1ToShoulder = mL5ToShoulderCG/lengthConversion;
		   mDocPtr -> setLinkCGs(newCGs);
	   }
		else
		{
			mL5ToShoulderCG = Round_Double(lengthConversion * CGs.L5S1ToShoulder, 1);//mDocPtr->getHandCG().ValueText(); 
		    mL5ToShoulderL = Round_Double(lengthConversion * lengths.L5S1ToC7T1, 1);//mDocPtr->getHandWithFingersL().ValueText();
			backl[29] = mL5ToShoulderL;
			backl[30] = mL5ToShoulderCG;
		}
   mNeck1Wt = Round_Double(forceConversion * weights.UpperNeck, 1);  
      if(mNeck1L != Round_Double(backl[23],1) && mNeck1CG == Round_Double(backl[24],1))
	   {
		   mNeck1L = Round_Double(lengthConversion * lengths.C1C3, 1);//mDocPtr->getHandWithFingersL().ValueText();
		   mNeck1CG = Round_Double(mNeck1L * mCM_UpperNeck, 1);
		   backl[23] = mNeck1L;
		   backl[24] = mNeck1CG;
		   newCGs.UpperNeck = mNeck1CG/lengthConversion;
		   mDocPtr -> setLinkCGs(newCGs);
	   }
		else
		{
			mNeck1CG = Round_Double(lengthConversion * CGs.UpperNeck, 1);//mDocPtr->getHandCG().ValueText(); 
		    mNeck1L = Round_Double(lengthConversion * lengths.C1C3, 1);//mDocPtr->getHandWithFingersL().ValueText();
			backl[23] = mNeck1L;
			backl[24] = mNeck1CG;
		}
   //mNeck1CG = 0.0; //Round_Double(lengthConversion * CGs.Neck, 1);
   //mNeck1L = Round_Double(lengthConversion * lengths.C1C3, 1);
   //if(isred[23] == true)
	//   mNeck1CG = Round_Double(mNeck1L * 0, 1);
   //mNeck1Wt = 0.0; //Round_Double(forceConversion * weights.Neck, 1);
     /*mNeck2CG = Round_Double(lengthConversion * CGs.LowerNeck, 1);
   mNeck2L = Round_Double(lengthConversion * lengths.C4C7, 1);
   if(isred[26] == true)
	   mNeck2CG = Round_Double(mNeck2L * mCM_LowerNeck, 1);*/
   mNeck2Wt = Round_Double(forceConversion * weights.LowerNeck, 1);  
      if(mNeck2L != Round_Double(backl[26],1) && mNeck2CG == Round_Double(backl[27],1))
	   {
		   mNeck2L = Round_Double(lengthConversion * lengths.C4C7, 1);//mDocPtr->getHandWithFingersL().ValueText();
		   mNeck2CG = Round_Double(mNeck2L * mCM_LowerNeck, 1);
		   backl[26] = mNeck2L;
		   backl[27] = mNeck2CG;
		   newCGs.LowerNeck = mNeck2CG/lengthConversion;
		   mDocPtr -> setLinkCGs(newCGs);
	   }
		else
		{
			mNeck2CG = Round_Double(lengthConversion * CGs.LowerNeck, 1);//mDocPtr->getHandCG().ValueText(); 
		    mNeck2L = Round_Double(lengthConversion * lengths.C4C7, 1);//mDocPtr->getHandWithFingersL().ValueText();
			backl[26] = mNeck2L;
			backl[27] = mNeck2CG;
		}
   mNasionL = Round_Double(lengthConversion * lengths.HeadRadius, 1);

   mL5ToShoulderL = Round_Double(lengthConversion * lengths.L5S1ToC7T1, 1);//mDocPtr->getL5ToShoulderL().ValueText();
   mL5ToShoulderCG = Round_Double(lengthConversion * CGs.L5S1ToShoulder, 1);
   mL5ToShoulderWt = Round_Double(forceConversion * weights.TorsoAndHead, 1);//need to get just torso weight


   mNeck1CG = Round_Double(lengthConversion * CGs.UpperNeck, 1);
   mNeck1L = Round_Double(lengthConversion * lengths.C1C3, 1);
   mNeck1Wt = Round_Double(forceConversion * weights.UpperNeck, 1);

   mNeck2CG = Round_Double(lengthConversion * CGs.LowerNeck, 1);
   mNeck2L = Round_Double(lengthConversion * lengths.C4C7, 1);
   mNeck2Wt = Round_Double(forceConversion * weights.LowerNeck, 1);  


   //Skeleton* skel = mDocPtr->GetSkeleton();
   //const LowBackSagittalData_s& sagittal = skel->getLowBackSagittalData();
   //mDiaphramMomentL = Round_Double(mDocPtr -> GetSkeleton() -> lowBackSagittalData.diaphramMomentArm * lengthConversion, 1);//mDocPtr->getDiaphramMomentL().ValueText();
   //mUpperBodyWt = Round_Double((skel->getNetForce(JT_L5S1_TEMP) - skel->getNetForce(JT_LHAND) - skel->getNetForce(JT_RHAND)).length(), 1);//mDocPtr->getUpperBodyWt().ValueText();
   // mShoulderToShoulderL.Format("%.1f",lengthConversion * lengths.ShoulderWidth);//mDocPtr->getShoulderToShoulderL().ValueText();

   mFactorsName = mDocPtr->GetFactors().mName;
   UpdateData(FALSE);
   InitializeCustomValues();
   // if(!dataEntry)
   //		RedrawWindow();
}

void Dg_Anthropometry::OnOK()
{
	doApply(true);
}



void Dg_Anthropometry::OnKillfocusWeightValue()
{
    if(this->UpdateData(TRUE))
    {
        if(mWeightValue != mDocPtr->getAnthropometryRef().Weight())
        {
			Anthropometry anthro = mDocPtr->getAnthropometryRef();
			if(!anthro.Weight(mWeightValue))
            //if(!mDocPtr->getAnthropometryRef().Weight(mWeightValue))
			{
				anthro.Percentile(mDocPtr->getAnthropometryRef().cDataEntry);
                mPercentileValue = anthro.Percentile();
				mDocPtr->setAnthropometry(anthro);
                return;
			}

            else
            {
                anthro.Percentile(mDocPtr->getAnthropometryRef().cDataEntry);
                mPercentileValue = anthro.Percentile();
				mDocPtr->setAnthropometry(anthro);
                this->UpdateData(FALSE);
            }
        }
    }
}

BOOL Dg_Anthropometry::DataValidate(void)
{
  
    if(this->UpdateData(TRUE))
    {
		Anthropometry anthro = mDocPtr->getAnthropometryRef();
		if (mGenderValue == 1)
		{
			UpdateData(TRUE);
			if(mHeightValue != anthro.fHeight())
			{
				if(!anthro.fHeight(mHeightValue))
				{
					AfxMessageBox((char*)(anthro.feHeight.mLastErrorMessage));
					mHeightControl.SetFocus();
					mPercentileValue = anthro.Percentile();
					this->UpdateData(FALSE);
					mDocPtr->setAnthropometry(anthro);
					return FALSE;
				}
				else
				{
					mPercentileValue = anthro.Percentile();
					this->UpdateData(FALSE);
					mDocPtr->setAnthropometry(anthro);
					return TRUE;
				}
			}
			if(mWeightValue != anthro.fWeight())
			{
				if(!anthro.fWeight(mWeightValue))
				{
					AfxMessageBox((char*)(anthro.feWeight.mLastErrorMessage));
					mWeightControl.SetFocus();
					mPercentileValue = anthro.Percentile();
					this->UpdateData(FALSE);
					mDocPtr->setAnthropometry(anthro);
					return FALSE;
				}
			}
		}
		else if(mGenderValue == 0)
		{
        if(mHeightValue != anthro.Height())
        {
            if(!anthro.Height(mHeightValue))
            {
                AfxMessageBox((char*)(anthro.LastErrorMessage()));
                mHeightControl.SetFocus();
				mPercentileValue = anthro.Percentile();
			    this->UpdateData(FALSE);
				mDocPtr->setAnthropometry(anthro);
			    return FALSE;
            }

            else
            {
                mPercentileValue = anthro.Percentile();
			    this->UpdateData(FALSE);
				mDocPtr->setAnthropometry(anthro);
                return TRUE;
            }
        }

        if(mWeightValue != anthro.Weight())
        {
            if(!anthro.Weight(mWeightValue))
            {
                AfxMessageBox((char*)(anthro.LastErrorMessage()));
                mWeightControl.SetFocus();
				mPercentileValue = anthro.Percentile();
                this->UpdateData(FALSE);
				mDocPtr->setAnthropometry(anthro);
			    return FALSE;
            }
		}
        else
            {
                mPercentileValue = anthro.Percentile();
                this->UpdateData(FALSE);
				mDocPtr->setAnthropometry(anthro);
			    return TRUE;
            }
        }
    }

    else return FALSE;

	return TRUE;
}

void Dg_Anthropometry::OnKillfocusHeightValue() {
    if(this->UpdateData(TRUE))
    {
		Anthropometry anthro = mDocPtr->getAnthropometryRef();
        if(mHeightValue != anthro.Height())
        {
            if(!anthro.Height(mHeightValue))
			{
				anthro.Percentile(mDocPtr->getAnthropometryRef().cDataEntry);
                mPercentileValue = anthro.Percentile();
				mDocPtr->setAnthropometry(anthro);
				UpdateData(FALSE);
                return;
			}
            else
            {
                anthro.Percentile(mDocPtr->getAnthropometryRef().cDataEntry);
                mPercentileValue = anthro.Percentile();
				mDocPtr->setAnthropometry(anthro);
                UpdateData(FALSE);
            }
        }
    }
}

bool Dg_Anthropometry::DoRedraw()
{

    updateClicked = true;
    if (dataEntry)
    {
        OnKillfocusHeightValue();
        OnKillfocusWeightValue();
    }
	lDocPtr -> setAnthropometry(lDocPtr -> getAnthropometryRef());
	lDocPtr -> Analyze();
    mDocPtr -> Analyze();
	newLengths = lDocPtr -> getLinkLengths();
    newWeights = lDocPtr -> getLinkWeights();
    newCGs = lDocPtr -> getLinkCGs();
	mDocPtr -> setLinkLengths(newLengths);
	mDocPtr -> setLinkWeights(newWeights);
	mDocPtr -> setLinkCGs(newCGs);
	C_Hom_Doc * pDoc = Get_App_Ptr()->Get_Document_Ptr();
	Anthropometry& anthropometry = pDoc->getTask().getAnthropometryRef();
	anthropometry.setRefWeights(newWeights);
	anthropometry.setNewWeights(newWeights);
   
	UpdateReport();
	
	
    if(!this->DataValidate())
       return false;
	
	if(dataEntry)
		RedrawWindow();

	
	if (valueChanged && dataEntry)
		mCaution.ShowWindow(SW_SHOW);
	else
		mCaution.ShowWindow(SW_HIDE);
	

    if(!this->CWnd::UpdateData(TRUE))
        return false;

    applyClicked = false;
	
	bool isUpdateDocument = UpdateDocument();
	if (isUpdateDocument) {
        CButton * apply = (CButton*)GetDlgItem(IDAPPLY);
        apply -> EnableWindow(TRUE);
        CButton * ok = (CButton*)GetDlgItem(IDOK);
        ok -> EnableWindow(TRUE);
		bool isSuccess = CWnd::UpdateData(FALSE);
        return true;
    }
    else
    {
        bool isSuccess = CWnd::UpdateData(FALSE);
        return false;
    }
	
}

void Dg_Anthropometry::OnRedraw() { 
	DoRedraw(); 
	
	
	updateClicked = true;
    if (dataEntry) {
        OnKillfocusHeightValue();
        OnKillfocusWeightValue();
    }
	
	//lDocPtr -> setAnthropometry(lDocPtr -> getAnthropometryRef());
	lDocPtr -> Analyze();
    mDocPtr -> Analyze();
	newLengths = lDocPtr -> getLinkLengths();
    newWeights = lDocPtr -> getLinkWeights();
    newCGs = lDocPtr -> getLinkCGs();
	mDocPtr -> setLinkLengths(newLengths);
	mDocPtr -> setLinkWeights(newWeights);
	mDocPtr -> setLinkCGs(newCGs);
	
	C_Hom_Doc * pDoc = Get_App_Ptr()->Get_Document_Ptr();
	Anthropometry& anthropometry = pDoc->getTask().getAnthropometryRef();
	anthropometry.setRefWeights(newWeights);
	anthropometry.setNewWeights(newWeights);
	
    UpdateReport();

	if(!modifyAnthro) {
		doApply(false);
	}
	
	// mCaution.ShowWindow(SW_HIDE);
	
}

void Dg_Anthropometry::OnAdvanced() 
{
   // if(!this->DoRedraw())
     //   return;
    mDocPtr->OnDg_Population(this->mGenderValue);

    if( this->UpdateData(TRUE))
    {
        this->UpdateDocument();
        this->UpdateDisplay();
    }
	this->UpdateData(FALSE);
	mAnthroBackup = mDocPtr->getAnthropometryRef();
}

void Dg_Anthropometry::OnClose() { CDialog::OnClose(); }

void Dg_Anthropometry::OnCancel() 
{
	//mDocPtr->getAnthropometryRef() = mAnthroBackup;
    if (!applyClicked)
    {
        mDocPtr -> setAnthropometry(mAnthroBackup); //to get percentiles and height/weight back
        RetrieveBackupValues();//retrieve previous lengths/CGs/weights here
        CWnd::UpdateData(FALSE);
    }
    CDialog::OnCancel();
}

void Dg_Anthropometry::OnEnChangeHeightvalue()
{
    updateClicked = false;
	this->CheckRadioButton(IDC_PercentileValue, IDC_PercentileDataEntry, IDC_PercentileDataEntry);
	
	// enable "apply height & body weight"
	CButton * update = (CButton*)GetDlgItem(IDC_REDRAW);
	update -> EnableWindow(TRUE);
}

void Dg_Anthropometry::OnEnSetfocusHeightvalue()
{
    dataEntry = true;
	this->CheckRadioButton(IDC_PercentileValue, IDC_PercentileDataEntry, IDC_PercentileDataEntry);

	// enable "apply height & body weight"
	CButton * update = (CButton*)GetDlgItem(IDC_REDRAW);
	update -> EnableWindow(TRUE);
}

void Dg_Anthropometry::OnEnChangeWeightvalue()
{
    updateClicked = false;
	this->CheckRadioButton(IDC_PercentileValue, IDC_PercentileDataEntry, IDC_PercentileDataEntry);

	// enable "apply height & body weight"
	CButton * update = (CButton*)GetDlgItem(IDC_REDRAW);
	update -> EnableWindow(TRUE);
}

void Dg_Anthropometry::OnEnSetfocusWeightvalue()
{
    dataEntry = true;
	this->CheckRadioButton(IDC_PercentileValue, IDC_PercentileDataEntry, IDC_PercentileDataEntry);

	// enable "apply height & body weight"
	CButton * update = (CButton*)GetDlgItem(IDC_REDRAW);
	update -> EnableWindow(TRUE);
}

void Dg_Anthropometry::OnBnClickedShoeCheck()
{
	UpdateData(true);
	mShoeHeightControl.EnableWindow(mShoeCheck);
	if(!mShoeCheck) {
		mShoeHeight.Format("0");
	} else {
		// default to 2.5 cm
		mShoeHeight.Format("%.1f", 2.5 * (mDocPtr->Is_English() ? CM_TO_IN : 1));
	}
	UpdateData(false);
}


void Dg_Anthropometry::OnBnClickedApply() 
{   
	doApply(false);
	// doApply(false);
}


void Dg_Anthropometry::doApply(bool isOK) {

	Factors changed;
	if (isOK) {
		if (applyClicked) //nothing to change
			CDialog::OnOK();
	} else {	// isApply
		 // if (applyClicked) //nothing to change		
		 //     return;		
		UpdateData(TRUE);		
		//if(left)		
		//	mDocPtr->getTask().mmLowerArmCGR = mLowerArmCG;		
		//else 		
		//	mDocPtr->getTask().mmLowerArmCGR = mLowerArmCG;		
		applyClicked = true;		
		valueChanged = false;
	}
	//if (true)
	if (modifyAnthro)
    {
		if(!updateClicked)
		{       
            mDocPtr -> setAnthropometry(mAnthroBackup);
		}
    }
    else
    {
        // updateClicked = true;
		// if (dataEntry)
		// {
		// 	OnKillfocusHeightValue();
		// 	OnKillfocusWeightValue();
		// }
	    // lDocPtr -> setAnthropometry(lDocPtr -> getAnthropometryRef());
		// lDocPtr -> Analyze();
        // mDocPtr -> Analyze();
		// newLengths = lDocPtr -> getLinkLengths();
        // newWeights = lDocPtr -> getLinkWeights();
        // newCGs = lDocPtr -> getLinkCGs();
		// mDocPtr -> setLinkLengths(newLengths);
		// mDocPtr -> setLinkWeights(newWeights);
		// mDocPtr -> setLinkCGs(newCGs);
        // UpdateReport();
		// DoRedraw();
        // updateClicked = false;
    }
 
    UpdateData(true);
    if(mShoeCheck)
    {
        float shoeHeight = atof(mShoeHeight);
	    if(mDocPtr->Is_English()) 
        {
		    shoeHeight *= IN_TO_CM;
	    }
        changed = mDocPtr->GetFactors(0); 
        changed.mShoeHeight = shoeHeight;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mShoeHeight = shoeHeight;
	    mDocPtr->SetFemaleFactors(changed);
        newLengths.ShoeHeight = shoeHeight;
		if(shoeHeight == 0) 
        {
			mShoeCheck = false;
			mShoeHeightControl.EnableWindow(false);
			UpdateData(false);
        }

        else 
			UpdateData(false);

        this->EndWaitCursor();
    }
    else
    {
        newLengths.ShoeHeight = 0;
    }
    if(mMaintainHandPositions)
    {
	    Vector3 adjustedLeft = mDocPtr->GetSkeleton()->getJoint(JT_LHAND, true) - mDocPtr->getOffset(0, true);
		Vector3 adjustedRight = mDocPtr->GetSkeleton()->getJoint(JT_RHAND, true) - mDocPtr->getOffset(0, true);
		mDocPtr->Analyze();
		adjustedLeft += mDocPtr->getOffset(0, true);
		adjustedRight += mDocPtr->getOffset(0, true);
        //if(!mDocPtr->PredictPosture(mLeftHand,mRightHand))
		if(!mDocPtr->PredictPosture(adjustedLeft, adjustedRight))
        {
            String lMsg("Unable to maintain hand positions.");
            lMsg = lMsg + " Anthropometry completed with alternate hand positions.";
            AfxMessageBox((char*)lMsg,MB_OK);
        }
    }
    status_box->UpdatePercentileLimits();
    mDocPtr -> getTask().setAllSkeletonsDirty();
    mDocPtr -> setLinkLengths(newLengths);
    mDocPtr -> setLinkCGs(newCGs);
    mDocPtr -> setLinkWeights(newWeights);
	// loop through all six views and update them one by one
	

    // mDocPtr -> MakeDirtyAndUpdateViews(); 
    UpdateReport();

    if(!isOK) 
		UpdateStandard();
	else
		UpdateBackup();
	

	//UpdateStandard();
	//UpdateBackup();
    InitializeCustomValues();

	

    if (modifyAnthro)
    {
        CButton * apply = (CButton*)GetDlgItem(IDAPPLY);
        apply -> EnableWindow(FALSE);
        CButton * ok = (CButton*)GetDlgItem(IDOK);
		if(!isOK)
			ok -> EnableWindow(TRUE);
    }
    // RedrawWindow();
	if (isOK){
		CDialog::OnOK();
	} else {
		if (valueChanged)
			mCaution.ShowWindow(SW_SHOW);
		else
			mCaution.ShowWindow(SW_HIDE);
	}


	//saving
	//savingOnBnClickedAnthro
	double mLHand;
	double mLHandR;
	double mLHandWithFingers;
	double mLHandWithFingersR;
	double mLForearm;
	double mLForearmR;
	double mLUpperArm;
	double mLUpperArmR;
	double mLUpperLeg;
	double mLUpperLegR;
	double mLLowerLeg;
	double mLLowerLegR;
	double mLHeelToToe;
	double mLHeelToToeR;
	//Add C1
	double mLC1Tragion;
	double mLHeadRadius;
	// Add:
	double mLC1C3;
	double mLC4C7;
	double mLL5S1ToShoulder;
	double mLHipToL5S1;

	double mCGHandWithFingers;
	double mCGHandWithFingersR;
	double mCGForearm;
	double mCGForearmR;
	double mCGUpperArm;
	double mCGUpperArmR;
	double mCGUpperLeg;
	double mCGUpperLegR;
	double mCGLowerLeg;
	double mCGLowerLegR;
	double mCGHeelToToe;
	double mCGHeelToToeR;
	//Add C1
	double mCGC1Tragion;
	// Add:
	double mCGC1C3;
	double mCGC4C7;
	double mCGL5S1ToShoulder;
	double mCGHipToL5S1;

	double mWtHand;
	double mWtHandR;
	double mWtHandWithFingers;
	double mWtHandWithFingersR;
	double mWtForearm;
	double mWtForearmR;
	double mWtUpperArm;
	double mWtUpperArmR;
	double mWtUpperLeg;
	double mWtUpperLegR;
	double mWtLowerLeg;
	double mWtLowerLegR;
	double mWtHeelToToe;
	double mWtHeelToToeR;
	//Add C1
	double mWtC1Tragion;
	// Add:
	double mWtC1C3;
	double mWtC4C7;
	double mWtL5S1ToShoulder;
	double mWtHipToL5S1;


    LinkLengths lengths = mDocPtr -> getLinkLengths(); 
    LinkCGs CGs = mDocPtr -> getLinkCGs();
    LinkWeights weights = mDocPtr -> getLinkWeights();	
	mLHand = lengths.Hand;
        changed = mDocPtr->GetFactors(0);    
        changed.mLHand = mLHand;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLHand = mLHand;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.Hand = mLHand;

	mLHandR = lengths.HandR;
        changed = mDocPtr->GetFactors(0);          
        changed.mLHandR = mLHandR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLHandR = mLHandR;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.HandR = mLHandR;

	mLHandWithFingers = lengths.HandWithFingers;
        changed = mDocPtr->GetFactors(0);         
        changed.mLHandWithFingers = mLHandWithFingers;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLHandWithFingers = mLHandWithFingers;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.HandWithFingers = mLHandWithFingers;

	mLHandWithFingersR = lengths.HandWithFingersR;
        changed = mDocPtr->GetFactors(0);         
        changed.mLHandWithFingersR = mLHandWithFingersR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLHandWithFingersR = mLHandWithFingersR;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.HandWithFingersR = mLHandWithFingersR;

	mLForearm = lengths.Forearm;
        changed = mDocPtr->GetFactors(0);         
        changed.mLForearm = mLForearm;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLForearm = mLForearm;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.Forearm = mLForearm;

	mLForearmR = lengths.ForearmR;
        changed = mDocPtr->GetFactors(0);         
        changed.mLForearmR = mLForearmR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLForearmR = mLForearmR;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.ForearmR = mLForearmR;

	mLUpperArm = lengths.UpperArm;
        changed = mDocPtr->GetFactors(0);         
        changed.mLUpperArm = mLUpperArm;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLUpperArm = mLUpperArm;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.UpperArm = mLUpperArm;

			mLUpperArmR = lengths.UpperArmR;
        changed = mDocPtr->GetFactors(0);         
        changed.mLUpperArmR = mLUpperArmR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLUpperArmR = mLUpperArmR;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.UpperArmR = mLUpperArmR;

	mLLowerLeg = lengths.LowerLeg;
        changed = mDocPtr->GetFactors(0);        
        changed.mLLowerLeg = mLLowerLeg;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLLowerLeg = mLLowerLeg;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.LowerLeg = mLLowerLeg;

	mLLowerLegR = lengths.LowerLegR;
        changed = mDocPtr->GetFactors(0);         
        changed.mLLowerLegR = mLLowerLegR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLLowerLegR = mLLowerLegR;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.LowerLegR = mLLowerLegR;

	mLUpperLeg = lengths.UpperLeg;
        changed = mDocPtr->GetFactors(0);          
        changed.mLUpperLeg = mLUpperLeg;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLUpperLeg = mLUpperLeg;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.UpperLeg = mLUpperLeg;

	mLUpperLegR = lengths.UpperLegR;
        changed = mDocPtr->GetFactors(0);        
        changed.mLUpperLegR = mLUpperLegR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLUpperLegR = mLUpperLegR;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.UpperLegR = mLUpperLegR;

	mLHeelToToe = lengths.HeelToToe;
        changed = mDocPtr->GetFactors(0);          
        changed.mLHeelToToe = mLHeelToToe;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLHeelToToe = mLHeelToToe;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.HeelToToe = mLHeelToToe;

	mLHeelToToeR = lengths.HeelToToeR;
        changed = mDocPtr->GetFactors(0);        
        changed.mLHeelToToeR = mLHeelToToeR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLHeelToToeR = mLHeelToToeR;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.HeelToToeR = mLHeelToToeR;

	mLC1C3 = lengths.C1C3;
        changed = mDocPtr->GetFactors(0);       
        changed.mLC1C3 = mLC1C3;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLC1C3 = mLC1C3;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.C1C3 = mLC1C3;

	mLC4C7 = lengths.C4C7;
        changed = mDocPtr->GetFactors(0);        
        changed.mLC4C7 = mLC4C7;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLC4C7 = mLC4C7;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.C4C7 = mLC4C7;

	mLC1Tragion = lengths.C1Tragion;
        changed = mDocPtr->GetFactors(0);       
        changed.mLC1Tragion = mLC1Tragion;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLC1Tragion = mLC1Tragion;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.C1Tragion = mLC1Tragion;

	mLHeadRadius = lengths.HeadRadius;
        changed = mDocPtr->GetFactors(0);       
        changed.mLHeadRadius = mLHeadRadius;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLHeadRadius = mLHeadRadius;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.HeadRadius = mLHeadRadius;

	mLL5S1ToShoulder = lengths.L5S1ToC7T1;
        changed = mDocPtr->GetFactors(0);         
        changed.mLL5S1ToShoulder = mLL5S1ToShoulder;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLL5S1ToShoulder = mLL5S1ToShoulder;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.L5S1ToC7T1 = mLL5S1ToShoulder;

	mLHipToL5S1 = lengths.HipToL5S1;
        changed = mDocPtr->GetFactors(0);        
        changed.mLHipToL5S1 = mLHipToL5S1;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mLHipToL5S1 = mLHipToL5S1;
	    mDocPtr->SetFemaleFactors(changed);
		newLengths.HipToL5S1 = mLHipToL5S1;

		//weight
	mWtHandWithFingers = weights.Hand;
        changed = mDocPtr->GetFactors(0);         
        changed.mWtHandWithFingers = mWtHandWithFingers;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mWtHandWithFingers = mWtHandWithFingers;
	    mDocPtr->SetFemaleFactors(changed);
		newWeights.Hand = mWtHandWithFingers;

	mWtHandWithFingersR = weights.HandR;
        changed = mDocPtr->GetFactors(0);         
        changed.mWtHandWithFingersR = mWtHandWithFingersR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mWtHandWithFingersR = mWtHandWithFingersR;
	    mDocPtr->SetFemaleFactors(changed);
		newWeights.HandR = mWtHandWithFingersR;

	mWtForearm = weights.Forearm;
        changed = mDocPtr->GetFactors(0);         
        changed.mWtForearm = mWtForearm;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mWtForearm = mWtForearm;
	    mDocPtr->SetFemaleFactors(changed);
		newWeights.Forearm = mWtForearm;

	mWtForearmR = weights.ForearmR;
        changed = mDocPtr->GetFactors(0);         
        changed.mWtForearmR = mWtForearmR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mWtForearmR = mWtForearmR;
	    mDocPtr->SetFemaleFactors(changed);
		newWeights.ForearmR = mWtForearmR;

	mWtUpperArm = weights.UpperArm;
        changed = mDocPtr->GetFactors(0);         
        changed.mWtUpperArm = mWtUpperArm;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mWtUpperArm = mWtUpperArm;
	    mDocPtr->SetFemaleFactors(changed);
		newWeights.UpperArm = mWtUpperArm;

			mWtUpperArmR = weights.UpperArmR;
        changed = mDocPtr->GetFactors(0);         
        changed.mWtUpperArmR = mWtUpperArmR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mWtUpperArmR = mWtUpperArmR;
	    mDocPtr->SetFemaleFactors(changed);
		newWeights.UpperArmR = mWtUpperArmR;

	mWtLowerLeg = weights.LowerLeg;
        changed = mDocPtr->GetFactors(0);        
        changed.mWtLowerLeg = mWtLowerLeg;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mWtLowerLeg = mWtLowerLeg;
	    mDocPtr->SetFemaleFactors(changed);
		newWeights.LowerLeg = mWtLowerLeg;

	mWtLowerLegR = weights.LowerLegR;
        changed = mDocPtr->GetFactors(0);         
        changed.mWtLowerLegR = mWtLowerLegR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mWtLowerLegR = mWtLowerLegR;
	    mDocPtr->SetFemaleFactors(changed);
		newWeights.LowerLegR = mWtLowerLegR;

	mWtUpperLeg = weights.UpperLeg;
        changed = mDocPtr->GetFactors(0);          
        changed.mWtUpperLeg = mWtUpperLeg;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mWtUpperLeg = mWtUpperLeg;
	    mDocPtr->SetFemaleFactors(changed);
		newWeights.UpperLeg = mWtUpperLeg;

	mWtUpperLegR = weights.UpperLegR;
        changed = mDocPtr->GetFactors(0);        
        changed.mWtUpperLegR = mWtUpperLegR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mWtUpperLegR = mWtUpperLegR;
	    mDocPtr->SetFemaleFactors(changed);
		newWeights.UpperLegR = mWtUpperLegR;

	mWtHeelToToe = weights.Foot;
        changed = mDocPtr->GetFactors(0);          
        changed.mWtHeelToToe = mWtHeelToToe;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mWtHeelToToe = mWtHeelToToe;
	    mDocPtr->SetFemaleFactors(changed);
		newWeights.Foot = mWtHeelToToe;

	mWtHeelToToeR = weights.FootR;
        changed = mDocPtr->GetFactors(0);        
        changed.mWtHeelToToeR = mWtHeelToToeR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mWtHeelToToeR = mWtHeelToToeR;
	    mDocPtr->SetFemaleFactors(changed);
		newWeights.FootR = mWtHeelToToeR;

	mWtC1C3 = weights.UpperNeck;
        changed = mDocPtr->GetFactors(0);       
        changed.mWtC1C3 = mWtC1C3;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mWtC1C3 = mWtC1C3;
	    mDocPtr->SetFemaleFactors(changed);
		newWeights.UpperNeck = mWtC1C3;

	mWtC4C7 = weights.LowerNeck;
        changed = mDocPtr->GetFactors(0);        
        changed.mWtC4C7 = mWtC4C7;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mWtC4C7 = mWtC4C7;
	    mDocPtr->SetFemaleFactors(changed);
		newWeights.LowerNeck = mWtC4C7;

	mWtC1Tragion = weights.Head;
        changed = mDocPtr->GetFactors(0);       
        changed.mWtC1Tragion = mWtC1Tragion;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mWtC1Tragion = mWtC1Tragion;
	    mDocPtr->SetFemaleFactors(changed);
		newWeights.Head = mWtC1Tragion;

	mWtL5S1ToShoulder = weights.TorsoAndHead;
        changed = mDocPtr->GetFactors(0);         
        changed.mWtL5S1ToShoulder = mWtL5S1ToShoulder;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mWtL5S1ToShoulder = mWtL5S1ToShoulder;
	    mDocPtr->SetFemaleFactors(changed);
		newWeights.TorsoAndHead = mWtL5S1ToShoulder;

	mWtHipToL5S1 = weights.Pelvis;
        changed = mDocPtr->GetFactors(0);        
        changed.mWtHipToL5S1 = mWtHipToL5S1;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mWtHipToL5S1 = mWtHipToL5S1;
	    mDocPtr->SetFemaleFactors(changed);
		newWeights.Pelvis = mWtHipToL5S1;

		//CGs
	mCGHandWithFingers = CGs.Hand;
        changed = mDocPtr->GetFactors(0);         
        changed.mCGHandWithFingers = mCGHandWithFingers;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mCGHandWithFingers = mCGHandWithFingers;
	    mDocPtr->SetFemaleFactors(changed);
		newCGs.Hand = mCGHandWithFingers;

	mCGHandWithFingersR = CGs.HandR;
        changed = mDocPtr->GetFactors(0);         
        changed.mCGHandWithFingersR = mCGHandWithFingersR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mCGHandWithFingersR = mCGHandWithFingersR;
	    mDocPtr->SetFemaleFactors(changed);
		newCGs.HandR = mCGHandWithFingersR;

	mCGForearm = CGs.Forearm;
        changed = mDocPtr->GetFactors(0);         
        changed.mCGForearm = mCGForearm;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mCGForearm = mCGForearm;
	    mDocPtr->SetFemaleFactors(changed);
		newCGs.Forearm = mCGForearm;

	mCGForearmR = CGs.ForearmR;
        changed = mDocPtr->GetFactors(0);         
        changed.mCGForearmR = mCGForearmR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mCGForearmR = mCGForearmR;
	    mDocPtr->SetFemaleFactors(changed);
		newCGs.ForearmR = mCGForearmR;

	mCGUpperArm = CGs.UpperArm;
        changed = mDocPtr->GetFactors(0);         
        changed.mCGUpperArm = mCGUpperArm;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mCGUpperArm = mCGUpperArm;
	    mDocPtr->SetFemaleFactors(changed);
		newCGs.UpperArm = mCGUpperArm;

			mCGUpperArmR = CGs.UpperArmR;
        changed = mDocPtr->GetFactors(0);         
        changed.mCGUpperArmR = mCGUpperArmR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mCGUpperArmR = mCGUpperArmR;
	    mDocPtr->SetFemaleFactors(changed);
		newCGs.UpperArmR = mCGUpperArmR;

	mCGLowerLeg = CGs.LowerLeg;
        changed = mDocPtr->GetFactors(0);        
        changed.mCGLowerLeg = mCGLowerLeg;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mCGLowerLeg = mCGLowerLeg;
	    mDocPtr->SetFemaleFactors(changed);
		newCGs.LowerLeg = mCGLowerLeg;

	mCGLowerLegR = CGs.LowerLegR;
        changed = mDocPtr->GetFactors(0);         
        changed.mCGLowerLegR = mCGLowerLegR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mCGLowerLegR = mCGLowerLegR;
	    mDocPtr->SetFemaleFactors(changed);
		newCGs.LowerLegR = mCGLowerLegR;

	mCGUpperLeg = CGs.UpperLeg;
        changed = mDocPtr->GetFactors(0);          
        changed.mCGUpperLeg = mCGUpperLeg;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mCGUpperLeg = mCGUpperLeg;
	    mDocPtr->SetFemaleFactors(changed);
		newCGs.UpperLeg = mCGUpperLeg;

	mCGUpperLegR = CGs.UpperLegR;
        changed = mDocPtr->GetFactors(0);        
        changed.mCGUpperLegR = mCGUpperLegR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mCGUpperLegR = mCGUpperLegR;
	    mDocPtr->SetFemaleFactors(changed);
		newCGs.UpperLegR = mCGUpperLegR;

	mCGHeelToToe = CGs.Foot;
        changed = mDocPtr->GetFactors(0);          
        changed.mCGHeelToToe = mCGHeelToToe;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mCGHeelToToe = mCGHeelToToe;
	    mDocPtr->SetFemaleFactors(changed);
		newCGs.Foot = mCGHeelToToe;

	mCGHeelToToeR = CGs.FootR;
        changed = mDocPtr->GetFactors(0);        
        changed.mCGHeelToToeR = mCGHeelToToeR;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mCGHeelToToeR = mCGHeelToToeR;
	    mDocPtr->SetFemaleFactors(changed);
		newCGs.FootR = mCGHeelToToeR;

	mCGC1C3 = CGs.UpperNeck;
        changed = mDocPtr->GetFactors(0);       
        changed.mCGC1C3 = mCGC1C3;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mCGC1C3 = mCGC1C3;
	    mDocPtr->SetFemaleFactors(changed);
		newCGs.UpperNeck = mCGC1C3;

	mCGC4C7 = CGs.LowerNeck;
        changed = mDocPtr->GetFactors(0);        
        changed.mCGC4C7 = mCGC4C7;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mCGC4C7 = mCGC4C7;
	    mDocPtr->SetFemaleFactors(changed);
		newCGs.LowerNeck = mCGC4C7;

	mCGC1Tragion = CGs.Head;
        changed = mDocPtr->GetFactors(0);       
        changed.mCGC1Tragion = mCGC1Tragion;
	    mDocPtr->SetMaleFactors(changed);
	    changed = mDocPtr->GetFactors(1);
	    changed.mCGC1Tragion = mCGC1Tragion;
	    mDocPtr->SetFemaleFactors(changed);
		newCGs.Head = mCGC1Tragion;

	// if(!isOK) {
	if(true) {
		mCGL5S1ToShoulder = CGs.L5S1ToShoulder;
			changed = mDocPtr->GetFactors(0);         
			changed.mCGL5S1ToShoulder = mCGL5S1ToShoulder;
			mDocPtr->SetMaleFactors(changed);
			changed = mDocPtr->GetFactors(1);
			changed.mCGL5S1ToShoulder = mCGL5S1ToShoulder;
			mDocPtr->SetFemaleFactors(changed);
			newCGs.L5S1ToShoulder = mCGL5S1ToShoulder;

		mCGHipToL5S1 = CGs.Pelvis;
			changed = mDocPtr->GetFactors(0);        
			changed.mCGHipToL5S1 = mCGHipToL5S1;
			mDocPtr->SetMaleFactors(changed);
			changed = mDocPtr->GetFactors(1);
			changed.mCGHipToL5S1 = mCGHipToL5S1;
			mDocPtr->SetFemaleFactors(changed);
			newCGs.Pelvis = mCGHipToL5S1;

		UpdateReport();
		applyClicked = true;
		valueChanged = false;
		//if(true)
		if (modifyAnthro)
		{
			if (!updateClicked)
			{
				mDocPtr -> setAnthropometry(mAnthroBackup);
			}
		}
		else
		{
			// DoRedraw();
			// updateClicked = false;
		}
 
		UpdateData(true);
		if(mShoeCheck)
		{
			float shoeHeight = atof(mShoeHeight);
			if(mDocPtr->Is_English()) 
			{
				shoeHeight *= IN_TO_CM;
			}
			changed = mDocPtr->GetFactors(0); 
			changed.mShoeHeight = shoeHeight;
			mDocPtr->SetMaleFactors(changed);
			changed = mDocPtr->GetFactors(1);
			changed.mShoeHeight = shoeHeight;
			mDocPtr->SetFemaleFactors(changed);
			newLengths.ShoeHeight = shoeHeight;
			if(shoeHeight == 0) 
			{
				mShoeCheck = false;
				mShoeHeightControl.EnableWindow(false);
				UpdateData(false);
			}

			else UpdateData(false);

			this->EndWaitCursor();
		}
		else
		{
			newLengths.ShoeHeight = 0;
		}
		if(mMaintainHandPositions)
		{
			Vector3 adjustedLeft = mDocPtr->GetSkeleton()->getJoint(JT_LHAND, true) - mDocPtr->getOffset(0, true);
			Vector3 adjustedRight = mDocPtr->GetSkeleton()->getJoint(JT_RHAND, true) - mDocPtr->getOffset(0, true);
			mDocPtr->Analyze();
			adjustedLeft += mDocPtr->getOffset(0, true);
			adjustedRight += mDocPtr->getOffset(0, true);
			//if(!mDocPtr->PredictPosture(mLeftHand,mRightHand))
			if(!mDocPtr->PredictPosture(adjustedLeft, adjustedRight))
			{
				String lMsg("Unable to maintain hand positions.");
				lMsg = lMsg + " Anthropometry completed with alternate hand positions.";
				AfxMessageBox((char*)lMsg,MB_OK);
			}
		}
		status_box->UpdatePercentileLimits();
		mDocPtr -> getTask().setAllSkeletonsDirty();
		mDocPtr -> setLinkLengths(newLengths);
		mDocPtr -> setLinkCGs(newCGs);
		mDocPtr -> setLinkWeights(newWeights);
		mDocPtr -> MakeDirtyAndUpdateViews(); 


		UpdateReport();

		UpdateStandard();
		InitializeCustomValues();
		/*
		if(left)
			mDocPtr->isredl = std::vector<bool>(isred);
		else
			mDocPtr->isredr = std::vector<bool>(isred);
			*/
			
		mDocPtr->isredl = std::vector<bool>(isred_left);
		mDocPtr->isredr = std::vector<bool>(isred_right);
		if (modifyAnthro)
		{
			CButton * apply = (CButton*)GetDlgItem(IDAPPLY);
			apply -> EnableWindow(FALSE);
			CButton * ok = (CButton*)GetDlgItem(IDOK);
			ok -> EnableWindow(TRUE);
		}
		RedrawWindow();
		if (valueChanged)
			mCaution.ShowWindow(SW_SHOW);
		else
			mCaution.ShowWindow(SW_HIDE);
	} else {
		//Skeleton* skel = mDocPtr->GetSkeleton();
		//const LowBackSagittalData_s& sagittal = skel->getLowBackSagittalData();
		//mDiaphramMomentL = Round_Double(mDocPtr -> GetSkeleton() -> lowBackSagittalData.diaphramMomentArm * lengthConversion, 1);//mDocPtr->getDiaphramMomentL().ValueText();
		//mUpperBodyWt = Round_Double((skel->getNetForce(JT_L5S1_TEMP) - skel->getNetForce(JT_LHAND) - skel->getNetForce(JT_RHAND)).length(), 1);//mDocPtr->getUpperBodyWt().ValueText();
		// mShoulderToShoulderL.Format("%.1f",lengthConversion * lengths.ShoulderWidth);//mDocPtr->getShoulderToShoulderL().ValueText();

		mFactorsName = mDocPtr->GetFactors().mName;
		UpdateData(FALSE);
		InitializeCustomValues();
		if(left)
			mDocPtr->isredl = std::vector<bool>(isred);
		else
			mDocPtr->isredr = std::vector<bool>(isred);
		RedrawWindow();
	}

}


Dg_Anthropometry::~Dg_Anthropometry()
{
	delete lDocPtr;
}

void Dg_Anthropometry::OnBnClickedAnthro()
{ 
	UpdateStandard();
	UpdateReport();

    CButton * update = (CButton*)GetDlgItem(IDC_REDRAW);
    CRect rect;
    GetWindowRect(&rect);
    ScreenToClient(&rect);
    if (!modifyAnthro)
    {
        MoveWindow(0,0,rect.Width() * 3,rect.Height(),true);
        CenterWindow();
        //update -> ShowWindow(SW_SHOW);
        modifyAnthro = true;
        CButton * apply = (CButton*)GetDlgItem(IDAPPLY);
        apply -> EnableWindow(FALSE);
        CButton * ok = (CButton*)GetDlgItem(IDOK);
        ok -> EnableWindow(FALSE);

		// disable advanced mode
		CButton * advanced = (CButton *)GetDlgItem(IDC_ADVANCED);
		advanced -> EnableWindow(FALSE);

		//OnBnClickedApply();
		CButton * anthro = (CButton*)GetDlgItem(IDC_ANTHRO);
        anthro ->SetWindowTextA("Hide Current Anthropometry Values");
		SetWindowText("Current Anthropometry");
    } else {
        MoveWindow(0,0,rect.Width() / 3, rect.Height(), true);
        CenterWindow();
        modifyAnthro = false;
        CButton * apply = (CButton*)GetDlgItem(IDAPPLY);
        apply -> EnableWindow(TRUE);
        CButton * ok = (CButton*)GetDlgItem(IDOK);
        ok -> EnableWindow(TRUE);

		// enable advanced mode
		CButton * advanced = (CButton *)GetDlgItem(IDC_ADVANCED);
		advanced -> EnableWindow(TRUE);

		CButton * anthro = (CButton*)GetDlgItem(IDC_ANTHRO);
        anthro -> SetWindowTextA("Display/Modifiy Anthropometry Values");
		SetWindowText("Anthropometry");
    }

}


void Dg_Anthropometry::OnBnClickedLeft()
{
	if (left == true)
	{
    left = false;
	mleftright = "RIGHT SIDE";

		/*if(mmLowerArmCGR != 0)
	{
		newCGs = mDocPtr -> getLinkCGs();
		   newCGs.ForearmR = mmLowerArmCGR / lengthConversion;
		   mDocPtr ->setLinkCGs(newCGs);
	}*/
	}
	else
	{
	left = true;
	mleftright = "LEFT SIDE";

		/*	if(mmLowerArmCG != 0)
	{
		newCGs = mDocPtr -> getLinkCGs();
		   newCGs.Forearm = mmLowerArmCG/lengthConversion;
		    mDocPtr ->setLinkCGs(newCGs);
	}*/

	}
	CString str;
	if(GetDlgItemText(IDC_LEFT,str),str=="Change to Right")
	{
		SetDlgItemText(IDC_LEFT,"Change to Left");
	}
	else
	{
		SetDlgItemText(IDC_LEFT,"Change to Right");
	}

    // UpdateReport();

	mHeadL = Round_Double( newLengths.C1Tragion * lengthConversion, 1);
	mHeadCG = Round_Double( newCGs.Head * lengthConversion, 1);
	mHeadWt = Round_Double( newWeights.Head * forceConversion, 1);
	mHipToL5L = Round_Double( newLengths.HipToL5S1 * lengthConversion, 1);
	mHipToL5Wt = Round_Double( newWeights.Pelvis * forceConversion, 1);
	mHipToL5CG = Round_Double( newCGs.Pelvis * lengthConversion, 1);
	mL5ToShoulderL = Round_Double( newLengths.L5S1ToC7T1 * lengthConversion, 1);
	mL5ToShoulderCG = Round_Double( newCGs.L5S1ToShoulder * lengthConversion, 1);
	mL5ToShoulderWt = Round_Double( newWeights.TorsoAndHead * forceConversion, 1);
	mNasionL = Round_Double( newLengths.HeadRadius * lengthConversion, 1);
	mNeck1L = Round_Double( newLengths.C1C3 * lengthConversion, 1);
	mNeck1Wt = Round_Double( newWeights.UpperNeck * forceConversion, 1);
	mNeck1CG = Round_Double( newCGs.UpperNeck * lengthConversion, 1);
	mNeck2L = Round_Double( newLengths.C4C7 * lengthConversion, 1);
	mNeck2Wt = Round_Double( newWeights.LowerNeck * forceConversion, 1);
	mNeck2CG = Round_Double( newCGs.LowerNeck * lengthConversion, 1); 

	const LinkLengths& Lengths = mDocPtr->getLinkLengths();
    const LinkWeights& Weights = mDocPtr->getLinkWeights();
    const LinkCGs& CGs = mDocPtr->getLinkCGs();


	if(left) {
		mFootL = Round_Double( newLengths.HeelToToe * lengthConversion, 1);
		newLengths.HeelToToeR = Lengths.HeelToToeR;

		mFootWt = Round_Double( newWeights.Foot * forceConversion, 1);
		newWeights.FootR = Weights.FootR;

		mFootCG = Round_Double( newCGs.Foot * lengthConversion, 1);
		newCGs.FootR = CGs.FootR;

		mLowerArmL = Round_Double( newLengths.Forearm * lengthConversion, 1);
		newLengths.ForearmR = Lengths.ForearmR;

		mLowerArmWt = Round_Double( newWeights.Forearm * forceConversion, 1);
		newWeights.ForearmR = Weights.ForearmR;

		mLowerArmCG = Round_Double( newCGs.Forearm * lengthConversion, 1);
		newCGs.ForearmR = CGs.ForearmR;

		mLowerLegL = Round_Double( newLengths.LowerLeg * lengthConversion, 1);
		newLengths.LowerLegR = Lengths.LowerLegR;

		mLowerLegCG = Round_Double( newCGs.LowerLeg * lengthConversion, 1);
		newCGs.LowerLegR = CGs.LowerLegR;

		mLowerLegWt = Round_Double( newWeights.LowerLeg * forceConversion, 1);
		newWeights.LowerLegR = Weights.LowerLegR;

		mUpperArmL = Round_Double( newLengths.UpperArm * lengthConversion, 1);
		newLengths.UpperArmR = Lengths.UpperArmR;

		mUpperArmCG = Round_Double( newCGs.UpperArm * lengthConversion, 1);
		newCGs.UpperArmR = CGs.UpperArmR;

		mUpperArmWt = Round_Double( newWeights.UpperArm * forceConversion, 1);
		newWeights.UpperArmR = Weights.UpperArmR;

		mUpperLegL = Round_Double( newLengths.UpperLeg * lengthConversion, 1);
		newLengths.UpperLegR = Lengths.UpperLegR;

		mUpperLegCG = Round_Double( newCGs.UpperLeg * lengthConversion, 1);
		newCGs.UpperLegR = CGs.UpperLegR;

		mUpperLegWt = Round_Double( newWeights.UpperLeg * forceConversion, 1);
		newWeights.UpperLegR = Weights.UpperLegR;

		mHandL = Round_Double( newLengths.Hand * lengthConversion, 1);
		newLengths.HandR = Lengths.HandR;

		mHandWithFingersL = Round_Double( newLengths.HandWithFingers * lengthConversion, 1);
		newLengths.HandWithFingersR = Lengths.HandWithFingersR;

		mHandWithFingersCG = Round_Double( newCGs.Hand * lengthConversion, 1);
		newCGs.HandR = CGs.HandR;

		mHandWithFingersWt = Round_Double( newWeights.Hand * forceConversion, 1);
		newWeights.HandR = Weights.HandR;


	} else { 
		mFootL = Round_Double( newLengths.HeelToToeR * lengthConversion, 1);
		newLengths.HeelToToe = Lengths.HeelToToe;

		mFootWt = Round_Double( newWeights.FootR * forceConversion, 1);
		newWeights.Foot = Weights.Foot;

		mFootCG = Round_Double( newCGs.FootR * lengthConversion, 1);
		newCGs.Foot = CGs.Foot;

		mLowerArmL = Round_Double( newLengths.ForearmR * lengthConversion, 1);
		newLengths.Forearm = Lengths.Forearm;

		mLowerArmWt = Round_Double( newWeights.ForearmR * forceConversion, 1);
		newWeights.Forearm = Weights.Forearm;

		mLowerArmCG = Round_Double( newCGs.ForearmR * lengthConversion, 1);
		newCGs.Forearm = CGs.Forearm;

		mLowerLegL = Round_Double( newLengths.LowerLegR * lengthConversion, 1);
		newLengths.LowerLeg = Lengths.LowerLeg;

		mLowerLegCG = Round_Double( newCGs.LowerLegR * lengthConversion, 1);
		newCGs.LowerLeg = CGs.LowerLeg;

		mLowerLegWt = Round_Double( newWeights.LowerLegR * forceConversion, 1);
		newWeights.LowerLeg = Weights.LowerLeg;

		mUpperArmL = Round_Double( newLengths.UpperArmR * lengthConversion, 1);
		newLengths.UpperArm = Lengths.UpperArm;

		mUpperArmCG = Round_Double( newCGs.UpperArmR * lengthConversion, 1);
		newCGs.UpperArm = CGs.UpperArm;

		mUpperArmWt = Round_Double( newWeights.UpperArmR * forceConversion, 1);
		newWeights.UpperArm = Weights.UpperArm;

		mUpperLegL = Round_Double( newLengths.UpperLegR * lengthConversion, 1);
		newLengths.UpperLeg = Lengths.UpperLeg;

		mUpperLegCG = Round_Double( newCGs.UpperLegR * lengthConversion, 1);
		newCGs.UpperLeg = CGs.UpperLeg;

		mUpperLegWt = Round_Double( newWeights.UpperLegR * forceConversion, 1);
		newWeights.UpperLeg = Weights.UpperLeg;

		mHandL = Round_Double( newLengths.HandR * lengthConversion, 1);
		newLengths.Hand = Lengths.Hand;

		mHandWithFingersL = Round_Double( newLengths.HandWithFingersR * lengthConversion, 1);
		newLengths.HandWithFingers = Lengths.HandWithFingers;

		mHandWithFingersCG = Round_Double( newCGs.HandR * lengthConversion, 1);
		newCGs.Hand = CGs.Hand;

		mHandWithFingersWt = Round_Double( newWeights.HandR * forceConversion, 1);
		newWeights.Hand = Weights.Hand;
	}

	UpdateData(FALSE);

    //UpdateExtremities();
    InitializeCustomValues();
	 CButton * left = (CButton*) GetDlgItem(IDC_LEFT);
}

void Dg_Anthropometry::UpdateExtremities()
{
   if (left) 
   {
       mHandL = Round_Double(lengthConversion * newLengths.Hand, 1);//mDocPtr->getHandL().ValueText();
       mHandWithFingersCG = Round_Double(lengthConversion * newCGs.Hand, 1);//mDocPtr->getHandCG().ValueText();
       mHandWithFingersL = Round_Double(lengthConversion * newLengths.HandWithFingers, 1);//mDocPtr->getHandWithFingersL().ValueText();
       mHandWithFingersWt = Round_Double(forceConversion * newWeights.Hand, 1);//mDocPtr->getHandWt().ValueText();
       mLowerArmCG = Round_Double(lengthConversion * newCGs.Forearm, 1);//mDocPtr->getLowerArmCG().ValueText();
       mLowerArmL = Round_Double(lengthConversion * newLengths.Forearm, 1);//mDocPtr->getLowerArmL().ValueText();
       mLowerArmWt = Round_Double(forceConversion * newWeights.Forearm, 1);//mDocPtr->getLowerArmWt().ValueText();
       mLowerLegCG = Round_Double(lengthConversion * newCGs.LowerLeg, 1);//mDocPtr->getLowerLegCG().ValueText();
       mLowerLegL = Round_Double(lengthConversion * newLengths.LowerLeg, 1);//mDocPtr->getLowerLegL().ValueText();
       mLowerLegWt = Round_Double(forceConversion * newWeights.LowerLeg, 1);//mDocPtr->getLowerLegWt().ValueText();
       mUpperArmCG = Round_Double(lengthConversion * newCGs.UpperArm, 1);//mDocPtr->getUpperArmCG().ValueText();
       mUpperArmL = Round_Double(lengthConversion * newLengths.UpperArm, 1);//mDocPtr->getUpperArmL().ValueText();
       mUpperArmWt = Round_Double(forceConversion * newWeights.UpperArm, 1);//mDocPtr->getUpperArmWt().ValueText();
       mUpperLegCG = Round_Double(lengthConversion * newCGs.UpperLeg, 1);//mDocPtr->getUpperLegCG().ValueText();
       mUpperLegL = Round_Double(lengthConversion * newLengths.UpperLeg, 1);//mDocPtr->getUpperLegL().ValueText();
       mUpperLegWt = Round_Double(forceConversion * newWeights.UpperLeg, 1);//mDocPtr->getUpperLegWt().ValueText();
       mFootCG = Round_Double(lengthConversion * newCGs.Foot, 1);
       mFootL = Round_Double(lengthConversion * newLengths.HeelToToe, 1);//mDocPtr->getFootL().ValueText();
       mFootWt = Round_Double(forceConversion * newWeights.Foot, 1);//mDocPtr->getFootWt().ValueText();

   }
   else
   {
        mHandL = Round_Double(lengthConversion * newLengths.HandR, 1);//mDocPtr->getHandL().ValueText();
        mHandWithFingersCG = Round_Double(lengthConversion * newCGs.HandR, 1);//mDocPtr->getHandCG().ValueText();
        mHandWithFingersL = Round_Double(lengthConversion * newLengths.HandWithFingersR, 1);//mDocPtr->getHandWithFingersL().ValueText();
        mHandWithFingersWt = Round_Double(forceConversion * newWeights.HandR, 1);//mDocPtr->getHandWt().ValueText();
        mLowerArmCG = Round_Double(lengthConversion * newCGs.ForearmR, 1);//mDocPtr->getLowerArmCG().ValueText();
        mLowerArmL = Round_Double(lengthConversion * newLengths.ForearmR, 1);//mDocPtr->getLowerArmL().ValueText();
        mLowerArmWt = Round_Double(forceConversion * newWeights.ForearmR, 1);//mDocPtr->getLowerArmWt().ValueText();
        mLowerLegCG = Round_Double(lengthConversion * newCGs.LowerLegR, 1);//mDocPtr->getLowerLegCG().ValueText();
        mLowerLegL = Round_Double(lengthConversion * newLengths.LowerLegR, 1);//mDocPtr->getLowerLegL().ValueText();
        mLowerLegWt = Round_Double(forceConversion * newWeights.LowerLegR, 1);//mDocPtr->getLowerLegWt().ValueText();
        mUpperArmCG = Round_Double(lengthConversion * newCGs.UpperArmR, 1);//mDocPtr->getUpperArmCG().ValueText();
        mUpperArmL = Round_Double(lengthConversion * newLengths.UpperArmR, 1);//mDocPtr->getUpperArmL().ValueText();
        mUpperArmWt = Round_Double(forceConversion * newWeights.UpperArmR, 1);//mDocPtr->getUpperArmWt().ValueText();
        mUpperLegCG = Round_Double(lengthConversion * newCGs.UpperLegR, 1);//mDocPtr->getUpperLegCG().ValueText();
        mUpperLegL = Round_Double(lengthConversion * newLengths.UpperLegR, 1);//mDocPtr->getUpperLegL().ValueText();
        mUpperLegWt = Round_Double(forceConversion * newWeights.UpperLegR, 1);//mDocPtr->getUpperLegWt().ValueText();
        mFootCG = Round_Double(lengthConversion * newCGs.FootR, 1);
        mFootL = Round_Double(lengthConversion * newLengths.HeelToToeR, 1);
        mFootWt = Round_Double(forceConversion * newWeights.FootR, 1);//mDocPtr->getFootWt().ValueText();

   }
     UpdateData(FALSE);
}

void Dg_Anthropometry::OnPercentile5th_50th_95th(void) { //without warning

	// disable "apply height & body weight"
	CButton * update = (CButton*)GetDlgItem(IDC_REDRAW);
	update -> EnableWindow(FALSE);
	
	if (!modifyAnthro)
		applyClicked = false;
    updateClicked = false;
    // dataEntry = false;
	bool updateAnthro = this->UpdateData(TRUE);
	if(updateAnthro) {
		Anthropometry anthro = mDocPtr->getAnthropometryRef();
		anthro.Sex(mGenderValue);
		anthro.Height(mHeightValue);
		anthro.Weight(mWeightValue);
		anthro.Percentile(mPercentileValue);
		mDocPtr->setAnthropometry(anthro);
		UpdateDisplay();
	}
	//UpdateStandard();
	this->UpdateData(FALSE);
	
	for(int i = 0; i < 35; i++) {
		backl[i] = standard[i];
		backr[i] = standard[i];
	}
	// UpdateReport();
	//OnRedraw();
}

void Dg_Anthropometry::initialback()
{
	for(int i = 0; i < 35; i++)
	{
		backl[i] = backup[i].first;
		backr[i] = backup[i].second;        //store the last data that input (for altering CG according to its Length)
	}
}



void Dg_Anthropometry::OnBnClickedPrint()
{
	CPrintFrame *pFrame = new CPrintFrame;
	pFrame->m_pCallerDlg = this;
	CRect m_rect;
	GetWindowRect(m_rect);
	pFrame->Create(NULL,"Anthropometry",WS_OVERLAPPEDWINDOW,m_rect);
	pFrame->m_pView->OnMyPrintPreview();
	
	delete pFrame;
	pFrame = NULL;
}


afx_msg LRESULT Dg_Anthropometry::OnBeginPrinting(WPARAM wParam, LPARAM lParam)
{
	CDC* pDC			= (CDC*)wParam;
	CPrintInfo* pInfo	= (CPrintInfo *)lParam;
	
	return 0;
}


afx_msg LRESULT Dg_Anthropometry::OnEndPrinting(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


afx_msg LRESULT Dg_Anthropometry::OnMyPrint(WPARAM wParam, LPARAM lParam)
{
	CDC* pdc			= (CDC*)wParam;
	CPrintInfo* pInfo	= (CPrintInfo *)lParam;
	CRect m_rect;
	GetWindowRect(m_rect);
	CDC* pImageDC=GetDC();
	int formx,formy;
	formx=pImageDC->GetDeviceCaps(LOGPIXELSX);
	formy=pImageDC->GetDeviceCaps(LOGPIXELSY);
	ReleaseDC(pImageDC);
	DWORD dwflags=PD_PAGENUMS|PD_HIDEPRINTTOFILE|PD_SELECTION;  
  
	int printerx,printery;
	printerx=pdc->GetDeviceCaps(LOGPIXELSX);
	printery=pdc->GetDeviceCaps(LOGPIXELSY);
	double ratex,ratey;
	ratex=(double)printerx/formx;
	ratey=(double)printery/formy;
	CClientDC dc(this);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc,m_rect.Width(),m_rect.Height());
	CDC imagedc;
	imagedc.CreateCompatibleDC(&dc);
	imagedc.SelectObject(&bmp);
	imagedc.BitBlt(0,0,m_rect.Width(),m_rect.Height(),&dc,0,0,SRCCOPY);
	BITMAP bmap;
	bmp.GetBitmap(&bmap);
	int panelsize=0;
	if(bmap.bmBitsPixel<16)
	{
		panelsize=pow(2,bmap.bmBitsPixel*sizeof(RGBQUAD));
	}
	
	BITMAPINFO* bInfo=(BITMAPINFO*)LocalAlloc(LPTR,sizeof(BITMAPINFO)+panelsize);
	bInfo->bmiHeader.biClrImportant=0;
	bInfo->bmiHeader.biBitCount=bmap.bmBitsPixel;
	////bInfo->bmiHeader.biCompression=0;
	bInfo->bmiHeader.biHeight=bmap.bmHeight;
	bInfo->bmiHeader.biPlanes=bmap.bmPlanes;
	bInfo->bmiHeader.biSize=sizeof(BITMAPINFO);
	bInfo->bmiHeader.biSizeImage=bmap.bmWidthBytes*bmap.bmHeight;
	bInfo->bmiHeader.biWidth=bmap.bmWidth;
	bInfo->bmiHeader.biXPelsPerMeter=0;
	bInfo->bmiHeader.biYPelsPerMeter=0;
	char *pData=new char[bmap.bmWidthBytes*bmap.bmHeight];
	::GetDIBits(imagedc.m_hDC,bmp,0,bmap.bmHeight,pData,bInfo,DIB_RGB_COLORS);
	pdc->StartDoc("print");
	::StretchDIBits(pdc->m_hDC,50,50,(int)((m_rect.Width()*ratex)),
	(int)((m_rect.Height())*ratey),0,0,m_rect.Width(),
	m_rect.Height(),pData,bInfo,DIB_RGB_COLORS,SRCCOPY);
	pdc->EndDoc();
	LocalFree(bInfo);
	
	delete[] pData;
	pData = NULL;
	bmp.DeleteObject();
	return 0;
}

void Dg_Anthropometry::OnPrintPrintwindow()
{
	// TODO: Add your command handler code here
	CRect m_rect;
	GetWindowRect(&m_rect);
	ScreenToClient(&m_rect);
	CDC* pImageDC=GetDC();
	
	int formx,formy;
	formx=pImageDC->GetDeviceCaps(LOGPIXELSX);
	formy=pImageDC->GetDeviceCaps(LOGPIXELSY);
	ReleaseDC(pImageDC);
	DWORD dwflags=PD_PAGENUMS|PD_HIDEPRINTTOFILE|PD_SELECTION;  
	CPrintDialog m_printdlg(FALSE,dwflags,NULL);     
	if(m_printdlg.DoModal()==IDOK)
	{
		CDC pdc;
		pdc.Attach(m_printdlg.GetPrinterDC());
		 
		int px=pdc.GetDeviceCaps(HORZRES);  // total number of pix in x direction
		int py=pdc.GetDeviceCaps(VERTRES);  // total number of pix in y direction
		int printerx,printery;
		
		printerx=pdc.GetDeviceCaps(LOGPIXELSX);
		printery=pdc.GetDeviceCaps(LOGPIXELSY);
		
		double ratex,ratey;
		ratex=(double)printerx/formx;
		ratey=(double)printery/formy;
		CClientDC dc(this);
		CBitmap bmp;
		
		int w=m_rect.Width();
		int h =m_rect.Height();
		bmp.CreateCompatibleBitmap(&dc,m_rect.Width(),m_rect.Height());
		CDC imagedc;
		imagedc.CreateCompatibleDC(&dc);
		imagedc.SelectObject(&bmp);
		imagedc.BitBlt(0,0,m_rect.Width(),m_rect.Height(),&dc,m_rect.left,m_rect.top,SRCCOPY);
		  
		BITMAP bmap;
		bmp.GetBitmap(&bmap);
		int panelsize=0;
		
		if(bmap.bmBitsPixel<16)
		{
			panelsize=pow(2,bmap.bmBitsPixel*sizeof(RGBQUAD));
		}
		
		BITMAPINFO* bInfo=(BITMAPINFO*)LocalAlloc(LPTR,sizeof(BITMAPINFO)+panelsize);
		bInfo->bmiHeader.biClrImportant=0;
		bInfo->bmiHeader.biBitCount=bmap.bmBitsPixel;
		bInfo->bmiHeader.biCompression=0;
		bInfo->bmiHeader.biHeight=bmap.bmHeight;
		bInfo->bmiHeader.biPlanes=bmap.bmPlanes;
		bInfo->bmiHeader.biSize=sizeof(BITMAPINFO);
		bInfo->bmiHeader.biSizeImage=bmap.bmWidthBytes*bmap.bmHeight;
		bInfo->bmiHeader.biWidth=bmap.bmWidth;
		bInfo->bmiHeader.biXPelsPerMeter=0;
		bInfo->bmiHeader.biYPelsPerMeter=0;
		
		char *pData=new char[bmap.bmWidthBytes*bmap.bmHeight];
		::GetDIBits(imagedc.m_hDC,bmp,0,bmap.bmHeight,pData,bInfo,DIB_RGB_COLORS);
		pdc.StartDoc("print");
		::StretchDIBits(pdc.m_hDC,(px-(ratex-0.2)*m_rect.Width())/2,(py-(ratey-0.2)*m_rect.Height())/2,(int)((m_rect.Width()*(ratex-0.2 ))),
		(int)((m_rect.Height())*(ratey-0.2)),0,0 ,m_rect.Width(),
		m_rect.Height(),pData,bInfo,DIB_RGB_COLORS,SRCCOPY);
		pdc.EndDoc();
		LocalFree(bInfo);
		
		delete[] pData;
		pData = NULL;
		bmp.DeleteObject();
	}
}

void Dg_Anthropometry::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (nID ==IDR_MENU1)             
    {
		OnPrintPrintwindow();
    }
	  else
		  CDialog::OnSysCommand(nID, lParam);

}

