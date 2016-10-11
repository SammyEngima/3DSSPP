#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include "UndoApplied.h"
#include "Dg_HandLoads.h"
#include "Dg_2D_HandLoads.h"
#include "../c4eLib/Units.hpp"
#include <string>

using std::string;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Dg_2D_HandLoads::Dg_2D_HandLoads(C_Hom_Doc *aDocPtr ,CWnd* pParent) : CDialog(Dg_2D_HandLoads::IDD, pParent),mDocPtr(aDocPtr)
{
    //{{AFX_DATA_INIT(Dg_2D_HandLoads)
    mMagnitudeUnits = _T("");
    mVerticalAngle = 0;
    mExertion = _T("");
    mMagnitude = 0.0f;
    mBackward = FALSE;
    //}}AFX_DATA_INIT

    Skeleton &skel = *mDocPtr->GetSkeleton();
    Force temp(0,1);

    temp.Value(2.0 * skel.getExtForce(JT_RHAND).length(),MetricCns);
    mMagnitude = temp.Value();
    mMagnitudeUnits = temp.UnitsText();


    double rv,rh,lv,lh;
	Skeleton& lSkeleton = *mDocPtr->GetSkeleton();
	Vector3 LHandForce = lSkeleton.getExtForce(JT_LHAND);
	Vector3 RHandForce = lSkeleton.getExtForce(JT_RHAND);
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

    //skel.GetHandForceAngles(rv,rh,lv,lh);
    mVerticalAngle = int(rv);
    mHorizontalAngle = int(rh);

    if(mHorizontalAngle < 0)
        mBackward = TRUE;
    else
        mBackward = FALSE;

    mExertion = (char*)ExertionText(rv,rh);
    Increment_Group = 2;
    Current_Control_ID = 0;
}

void Dg_2D_HandLoads::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(Dg_2D_HandLoads)
    DDX_Text(pDX, IDC_MagnitudeUnits, mMagnitudeUnits);
    DDX_Text(pDX, IDC_VerticalAngle, mVerticalAngle);
    DDX_Text(pDX, IDC_Exertion, mExertion);
    DDX_Text(pDX, IDC_Magnitude, mMagnitude);
    DDX_Check(pDX, IDC_Backward, mBackward);
    DDX_Radio(pDX, IDC_INCREMENT_GROUP, Increment_Group);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Dg_2D_HandLoads, CDialog)
    //{{AFX_MSG_MAP(Dg_2D_HandLoads)
    ON_BN_CLICKED(IDC_RedrawButton, OnRedrawButton)
    ON_BN_CLICKED(IDC_Backward, OnBackward)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_INCREMENT_MINUS, OnBnClickedIncrementMinus)
    ON_BN_CLICKED(IDC_INCREMENT_PLUS, OnBnClickedIncrementPlus)
    ON_EN_SETFOCUS(IDC_Magnitude, OnEnSetfocusMagnitude)
    ON_EN_SETFOCUS(IDC_VerticalAngle, OnEnSetfocusVerticalangle)
END_MESSAGE_MAP()

bool Dg_2D_HandLoads::UpdateDocument()
{
    // Check to make sure all of the values are in range 
    if(!this->VerifyTotalsInRange())
    {
        Restore();
        UpdateData(FALSE);
        return false;
    }

    this->BeginWaitCursor();
    Skeleton &skel = *mDocPtr->GetSkeleton();
    Force temp(0,4);

    // retain old forces for undo history
    Vector3 oldLF = skel.getExtForce(JT_LHAND),oldRF = skel.getExtForce(JT_RHAND);
    Vector3 oldLT = skel.getExtTorque(JT_LHAND),oldRT = skel.getExtTorque(JT_RHAND);

    // update force magnitudes
    double lHalfMagnitude = 0.5 * mMagnitude;
    temp.Value(lHalfMagnitude);
    //skel.Put_Force_Magnitudes(temp.ValueIn(MetricCns),temp.ValueIn(MetricCns));

    // update force angles
    if(mBackward)
        mHorizontalAngle = -90;
    else
        mHorizontalAngle = 90;

    // casting/addition required to prevent floating point precision from causing drift in values
    /*skel.Put_Force_Angles((double)mVerticalAngle + 0.5,(double)mHorizontalAngle + 0.5,
                          (double)mVerticalAngle + 0.5,(double)mHorizontalAngle + 0.5);*/
	
	// convert spherical angles to force vectors
	Vector3 forceVec;
    forceVec[0] = cos(mVerticalAngle*M_PI/180.0) * cos(mHorizontalAngle*M_PI/180.0);
    forceVec[1] = cos(mVerticalAngle*M_PI/180.0) * sin(mHorizontalAngle*M_PI/180.0);
    forceVec[2] = sin(mVerticalAngle*M_PI/180.0);

	forceVec *= lHalfMagnitude;

	skel.setExtForce(JT_LHAND, forceVec);
	skel.setExtForce(JT_RHAND, forceVec);
	// TODO conversions

    mExertion = (char*)ExertionText(mVerticalAngle,mHorizontalAngle);   
    UpdateData(FALSE);

    mDocPtr->addUndoEvent(new UndoableHandLoads(oldLF,oldRF,oldLT,oldRT));
    mDocPtr->MakeDirtyAndUpdateViews(true);
    this->EndWaitCursor();
    return true;
}

void Dg_2D_HandLoads::OnOK()
{
    if(this->UpdateData(TRUE))
        if(this->UpdateDocument())
            CDialog::OnOK();
}

void Dg_2D_HandLoads::OnRedrawButton()
{
    if(this->UpdateData(TRUE))
    {
        this->UpdateDocument();
        UpdateData(FALSE);
    }
}

void Dg_2D_HandLoads::OnBackward() 
{
    UpdateData(TRUE);
    if(mBackward)
        mHorizontalAngle = -90;
    else
        mHorizontalAngle = 90;

    // This creates a temporary exertion text
    mExertion = (char*)ExertionText(mVerticalAngle, mHorizontalAngle);   
    UpdateData(FALSE);
}

void Dg_2D_HandLoads::DoIncrement(bool increment_up)
{
    // increment_up is 1 if we're adding, 0 if subtracting
    int sign_multiplier = (increment_up) ? 1 : -1;

    if(this->Current_Control_ID != 0) 
    {
        if(this->UpdateData(TRUE)) //Make sure data OK 
        {
            char oldValText[50], newValText[50];
            char* stopstring;
            GetDlgItemText( this->Current_Control_ID, oldValText, 49);
            double Value = strtod(oldValText, &stopstring);
            double Prior_Value = Value;

            if(this->Increment_Group == 0) //1 increment selected
                Value = Value + 1*sign_multiplier;
            else
                Value = Value + 5*sign_multiplier * (this->Increment_Group);

            _gcvt(Value, 5, newValText);
            SetDlgItemText(this->Current_Control_ID, newValText);

            OnRedrawButton();	// Update the data
            UpdateData(FALSE);        
        }
    }
    else
        ::MessageBox(NULL,"No angle/magnitude active.","Increment Angle",MB_OK | MB_ICONEXCLAMATION);
}

void Dg_2D_HandLoads::OnBnClickedIncrementMinus()
{
    DoIncrement(0);
}

void Dg_2D_HandLoads::OnBnClickedIncrementPlus()
{
    DoIncrement(1);
}

void Dg_2D_HandLoads::OnEnSetfocusMagnitude()
{
    this->Current_Control_ID = IDC_Magnitude;
}

void Dg_2D_HandLoads::OnEnSetfocusVerticalangle()
{
    this->Current_Control_ID = IDC_VerticalAngle;	
}

bool Dg_2D_HandLoads::VerifyTotalsInRange()
{
    bool test_failed = false;
    int lMaxHandForce;

    ///// Check if we should use metric or English limit /////
    lMaxHandForce = MAX_TWO_HAND_FORCE_LB;
    if(mDocPtr->Is_Metric())
        lMaxHandForce = int(lMaxHandForce * U_LBF_NEWTONS); // Set Newton limit rather than lb

    ///// Check for out-of-range values /////
    if(mMagnitude > lMaxHandForce)
    {
        test_failed = true;
        CString errorMsg;
        errorMsg.Format("The magnitude of the hand force must be less than or equal to %i\n", MAX_TWO_HAND_FORCE_LB);
        AfxMessageBox( errorMsg, MB_OK);          
    }

    ///// Check for negative values ////
    if(mMagnitude < 0)
    {
        test_failed = true;
        AfxMessageBox("The magnitude of the hand force must be greater than 0.", MB_OK);
    }

    ///// Check vertical angles /////	
    if(abs(mVerticalAngle) > 90)
    {
        test_failed = true;
        AfxMessageBox("The vertical angle must be between -90 and 90 degrees.", MB_OK);
    }

    if(test_failed)
        return false;
    else
        return true;
}

// This function restores the values in the dialog to the previous correct set
void Dg_2D_HandLoads::Restore()
{
    Skeleton &skel = *mDocPtr->GetSkeleton();
    Force temp(0,1);

    temp.Value(2.0 * skel.getExtForce(JT_RHAND).length(),MetricCns);
    mMagnitude = temp.Value();

    double rv,rh,lv,lh;
	Vector3 LHandForce = skel.getExtForce(JT_LHAND);
	Vector3 RHandForce = skel.getExtForce(JT_RHAND);
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

    //skel.GetHandForceAngles(rv,rh,lv,lh);
    mVerticalAngle = int(rv);
    mHorizontalAngle = int(rh);

    if(mHorizontalAngle < 0)
        mBackward = TRUE;
    else
        mBackward = FALSE;

    mExertion = (char*)ExertionText(rv,rh);
}

// Arguments: vertical and horizontal angles
String Dg_2D_HandLoads::ExertionText(double aV,double aH)
{
    String lExertion;       
    String X;
    String Y;
    String Z;

    //Assign directional strings
       
    if(Round_Double(abs(aV), SAFE_DIGITS) == 90)
    {
        if(Round_Double(aV, SAFE_DIGITS) == 90) Z = "down";
        else Z = "up";
    }
       
    else
    {
        if(aH < 0) Y = "forward";
        else Y = "backward";

        if(abs(aH) < 90) X = "left";
        else X = "right";

        if(aV < 0) Z = "up";
        else Z = "down";
    }

    //Assign exertion string

    if(Round_Double(abs(aV), SAFE_DIGITS) == 90) lExertion = Z;

    else if(Round_Double(abs(aH), SAFE_DIGITS) == 90) 
    {
        if(Round_Double(aV,SAFE_DIGITS) == 0) lExertion = Y;
        else if(abs(aV) > 0 && abs(aV) <= 45) lExertion = Y + " and " + Z;
        else lExertion = Z + " and " + Y;
    }

    else if(Round_Double(abs(aH), SAFE_DIGITS) == 0 || Round_Double(abs(aH), SAFE_DIGITS) == 180)
    {
        if(Round_Double(aV, SAFE_DIGITS) == 0) lExertion = X;
        else if(abs(aV) > 0 && abs(aV) <= 45) lExertion = X + " and " + Z;
        else lExertion = Z + " and " + X;
    }

    else if(abs(aH) > 45 && abs(aH) <= 135)
    {
        if(Round_Double(aV, SAFE_DIGITS) == 0) lExertion = Y + " and " + X;
        else if(abs(aV) > 0 && abs(aV) <= 45) lExertion = Y + ", " + X + ", and " + Z;
        else lExertion = Z + ", " + Y + ", and " + X;
    }

    else
    {
        if(Round_Double(aV,SAFE_DIGITS) == 0) lExertion = X + " and " + Y;
        else if(abs(aV) > 0 && abs(aV) <= 45) lExertion = X + ", " + Y + ", and " + Z;
        else lExertion = Z + ", " + X + ", and " + Y;
    }
       
    return lExertion;
}