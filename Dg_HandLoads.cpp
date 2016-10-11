#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h"
#include "UndoApplied.h"
#include "UndoPosture.h"
#include "Dg_HandLoads.h"
#include "Dg_HandLoadsAdv.h"
#include "../c4eLibNew/Units.hpp"
#include ".\dg_handloads.h"
#include "Dg_HandLoadSolver.h"

#include "Skeleton.hpp"
#include "Types.h"



#ifdef _DEBUG
    #undef THIS_FILE
    static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

enum HandEffort
{
    effUnitialized = -1,
    effByAppliedLoad = 0,
    effLift = 1,
    effLower = 2,
    effPush = 3,
    effPull = 4,
    effLeft = 5,
    effRight = 6 
};

void HorizontalForceAngleStatus(double aVerticalAngle,CEdit& aHorizontalControlRef)
{
    if (Round_Double(abs(aVerticalAngle), SAFE_DIGITS) == 90)
        aHorizontalControlRef.EnableWindow(FALSE);
    else
        aHorizontalControlRef.EnableWindow(TRUE);
}

Dg_HandLoads::Dg_HandLoads(C_Hom_Doc *aDocPtr,CWnd* pParent) : CDialog(Dg_HandLoads::IDD, pParent),
    mDocPtr(aDocPtr),Current_Control_ID(0),Increment_Forces_Group(0),Increment_Solver_Group(0),Solver_Condition_Group(0)
	, mdLeftMag(_T(""))
	, mdRightMag(_T(""))
	, m_checkhand(FALSE)
	, m_BackCompression(TRUE)
	, m_Strength(TRUE)
	, m_Balance(TRUE)
	, m_wrist(TRUE)
	, m_elbow(TRUE)
	, m_shoulder(TRUE)
	, m_torso(TRUE)
	, m_hip(TRUE)
	, m_knee(TRUE)
	, m_ankle(TRUE)
{
    //{{AFX_DATA_INIT(Dg_HandLoads)
    mLeftHorizontal = 0;
    mdLeftHorizontal = 0;
    mRightHorizontal = 0;
    mdRightHorizontal = 0;

    mLeftMagUnits = _T("");
    mRightMagUnits = _T("");

    mLeftVertical = 0;
    mdLeftVertical = 0;
    mRightVertical = 0;
    mdRightVertical = 0;
	 
    mLeftExertion = _T("");
    mRightExertion = _T("");

	/* mdLeftMag and mdRightMag are no longer doubles; they are CStrings; */
    /* mdLeftMag = 0.0f
    mdRightMag = 0.0f; */

	mdLeftMag.Format("%.2f",0.0);
    mdRightMag.Format("%.2f",0.0);


    mLeftEffortGroup = 0;
    mRightEffortGroup = 0;
    Increment_Forces_Group = 0;
	Increment_Solver_Group = 2;
    //}}AFX_DATA_INIT

    Skeleton &skel = *mDocPtr->GetSkeleton();

    // assign force magnitudes
    mdLeftMag.Format("%.2f",skel.getExtForce(JT_LHAND).length());
    mdRightMag.Format("%.2f",skel.getExtForce(JT_RHAND).length());
	
    // assign force angles
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

    //skel.GetHandForceAngles(mRightVertical,mRightHorizontal,mLeftVertical,mLeftHorizontal);
	mRightVertical = rv;
	mRightHorizontal = rh;
	mLeftVertical = lv;
	mLeftHorizontal = lh;

    mdRightHorizontal = Round_Double(mRightHorizontal, 2);
    mdRightVertical = Round_Double(mRightVertical, 2);
    mdLeftHorizontal = Round_Double(mLeftHorizontal, 2);
    mdLeftVertical = Round_Double(mLeftVertical, 2);

    mLeftExertion = (char*)ExertionText(mLeftVertical,mLeftHorizontal);
    mRightExertion = (char*)ExertionText(mRightVertical,mRightHorizontal);

	//
	//Sayan 07/12/07
	//
	// Units are now obtained directly from the Document class.

	mLeftMagUnits = mRightMagUnits = aDocPtr->ForceUnits();

	//set variable to false so advanced won't open everytime.
	openAdvHandLoads = false;
}

void Dg_HandLoads::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_HandLoads)
	DDX_Control(pDX, IDC_RIGHT_VERTICAL, mRightVerticalControl);
	DDX_Control(pDX, IDC_LEFT_VERTICAL, mLeftVerticalControl);
	DDX_Control(pDX, IDC_RIGHT_HORIZONTAL, mRightHorizontalControl);
	DDX_Control(pDX, IDC_LEFT_HORIZONTAL, mLeftHorizontalControl);
	//DDX_Control(pDX, IDC_BOTHHANDS, mBothHandsControl);

	DDX_Text(pDX, IDC_LEFT_HORIZONTAL, mdLeftHorizontal);
	DDX_Text(pDX, IDC_LEFT_MAG_UNITS, mLeftMagUnits);
	DDX_Text(pDX, IDC_LEFT_VERTICAL, mdLeftVertical);
	DDX_Text(pDX, IDC_RIGHT_HORIZONTAL, mdRightHorizontal);
	DDX_Text(pDX, IDC_RIGHT_MAG_UNITS, mRightMagUnits);
	DDX_Text(pDX, IDC_RIGHT_VERTICAL, mdRightVertical);
	DDX_Text(pDX, IDC_LeftExertion, mLeftExertion);
	DDX_Text(pDX, IDC_RightExertion, mRightExertion);
	DDX_Text(pDX, IDC_LEFT_MAG, mdLeftMag);
	DDX_Text(pDX, IDC_RIGHT_MAG, mdRightMag);
	DDX_Radio(pDX, IDC_LeftEffortGroup, mLeftEffortGroup);
	DDX_Radio(pDX, IDC_RightEffortGroup, mRightEffortGroup);
	DDX_Radio(pDX, IDC_INCREMENT_FORCES_GROUP, Increment_Forces_Group);
	//DDX_Radio(pDX, IDC_INCREMENT_SOLVER_GROUP, Increment_Solver_Group);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_LEFT_MAG, mdLeftMag);
	DDX_Text(pDX, IDC_RIGHT_MAG, mdRightMag);
	DDX_Check(pDX, IDC_CHECKHAND, m_checkhand);
	//DDX_Control(pDX, IDC_INCMAX, m_incmax);
	//DDX_Check(pDX, IDC_BackCompression, m_BackCompression);
	//DDX_Check(pDX, IDC_Strength, m_Strength);
	//DDX_Check(pDX, IDC_Balance, m_Balance);
	/*DDX_Check(pDX, IDC_CHECK1, m_wrist);
	DDX_Check(pDX, IDC_CHECK3, m_elbow);
	DDX_Check(pDX, IDC_CHECK4, m_shoulder);
	DDX_Check(pDX, IDC_CHECK5, m_torso);
	DDX_Check(pDX, IDC_CHECK6, m_hip);
	DDX_Check(pDX, IDC_CHECK7, m_knee);
	DDX_Check(pDX, IDC_CHECK8, m_ankle);
	DDX_Control(pDX, IDC_CHECK1, wristcontrol);
	DDX_Control(pDX, IDC_CHECK3, elbowcontrol);
	DDX_Control(pDX, IDC_CHECK4, shouldercontrol);
	DDX_Control(pDX, IDC_CHECK5, torsocontrol);
	DDX_Control(pDX, IDC_CHECK6, hipcontrol);
	DDX_Control(pDX, IDC_CHECK7, kneecontrol);
	DDX_Control(pDX, IDC_CHECK8, ankelcontrol);*/
	DDX_Control(pDX, IDC_REDO, redocontrol);
	DDX_Control(pDX, IDC_UNDO, undocontrol);
}

BEGIN_MESSAGE_MAP(Dg_HandLoads, CDialog)
    //{{AFX_MSG_MAP(Dg_HandLoads)
    ON_BN_CLICKED(IDC_RedrawButton, OnRedrawButton)
    ON_EN_SETFOCUS(IDC_LEFT_MAG, OnSetfocusLeftForce)
    ON_EN_SETFOCUS(IDC_RIGHT_MAG, OnSetfocusRightForce)
    ON_EN_SETFOCUS(IDC_LEFT_VERTICAL, OnSetfocusLeftVerAngle)
    ON_EN_SETFOCUS(IDC_LEFT_HORIZONTAL, OnSetfocusLeftHorAngle)
    ON_EN_SETFOCUS(IDC_RIGHT_VERTICAL, OnSetfocusRightVerAngle)
    ON_EN_SETFOCUS(IDC_RIGHT_HORIZONTAL, OnSetfocusRightHorAngle)
    ON_BN_CLICKED(IDC_INCREMENT_PLUS, OnIncrementPlus)
    ON_BN_CLICKED(IDC_INCREMENT_MINUS, OnIncrementMinus)
    ON_EN_KILLFOCUS(IDC_LEFT_VERTICAL, OnKillfocusLeftVertical)
    ON_EN_KILLFOCUS(IDC_LEFT_HORIZONTAL, OnKillfocusLeftHorizontal)
    ON_EN_KILLFOCUS(IDC_RIGHT_VERTICAL, OnKillfocusRightVertical)
    ON_EN_KILLFOCUS(IDC_RIGHT_HORIZONTAL, OnKillfocusRightHorizontal)
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_LeftEffortGroup, OnLeftEffortGroup)
    ON_BN_CLICKED(IDC_LeftExertLeft, OnLeftExertLeft)
    ON_BN_CLICKED(IDC_LeftExertRight, OnLeftExertRight)
    ON_BN_CLICKED(IDC_LeftLift, OnLeftLift)
    ON_BN_CLICKED(IDC_LeftLower, OnLeftLower)
    ON_BN_CLICKED(IDC_LeftPullBack, OnLeftPullBack)
    ON_BN_CLICKED(IDC_LeftPushForward, OnLeftPushForward)
    ON_BN_CLICKED(IDC_RightEffortGroup, OnRightEffortGroup)
    ON_BN_CLICKED(IDC_RightExertLeft, OnRightExertLeft)
    ON_BN_CLICKED(IDC_RightExertRight, OnRightExertRight)
    ON_BN_CLICKED(IDC_RightLift, OnRightLift)
    ON_BN_CLICKED(IDC_RightLower, OnRightLower)
    ON_BN_CLICKED(IDC_RightPullBack, OnRightPullBack)
    ON_BN_CLICKED(IDC_RightPushForward, OnRightPushForward)
    ON_EN_KILLFOCUS(IDC_RIGHT_MAG, OnKillfocusRightMag)
    ON_EN_KILLFOCUS(IDC_LEFT_MAG, OnKillfocusLeftMag)
    //}}AFX_MSG_MAP
    ON_EN_CHANGE(IDC_RIGHT_VERTICAL, OnEnChangeRightVertical)
    ON_EN_CHANGE(IDC_LEFT_VERTICAL, OnEnChangeLeftVertical)
	ON_BN_CLICKED(IDC_HAND_LOADS_ADV, OnBnClickedHandLoadsAdv)
	ON_BN_CLICKED(IDC_CHECKHAND, &Dg_HandLoads::OnBnClickedCheckhand)
	//ON_BN_CLICKED(IDC_INCMAX, &Dg_HandLoads::OnClickedIncmax)
	ON_WM_NCRBUTTONDOWN()
	ON_COMMAND(ID_PRINT_PRINTWINDOW, &Dg_HandLoads::OnPrintPrintwindow)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_RESETZERO, &Dg_HandLoads::OnBnClickedResetzero)
	//ON_BN_CLICKED(IDC_SelectAll, &Dg_HandLoads::OnBnClickedSelectAll)
	//ON_BN_CLICKED(IDC_NoneSelected, &Dg_HandLoads::OnBnClickedNone)
	//ON_BN_CLICKED(IDC_Strength, &Dg_HandLoads::OnBnClickedStrength)
	ON_BN_CLICKED(IDC_HANDFORCESOLVER, &Dg_HandLoads::OnBnClickedHandLoadSolver)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_REDO, &Dg_HandLoads::OnBnClickedRedo)
	ON_BN_CLICKED(IDC_UNDO, &Dg_HandLoads::OnBnClickedUndo)
END_MESSAGE_MAP()

void Dg_HandLoads::UpdateDocument()
{
    Skeleton &skel = *mDocPtr->GetSkeleton();

    // retain old forces for undo history
    Vector3 oldLF = skel.getExtForce(JT_LHAND),oldRF = skel.getExtForce(JT_RHAND);
    Vector3 oldLT = skel.getExtTorque(JT_LHAND),oldRT = skel.getExtTorque(JT_RHAND);

	// convert spherical angles to force vectors
	Vector3 rightForceVec, leftForceVec;
    rightForceVec[0] = cos(mRightVertical*M_PI/180.0) * cos(mRightHorizontal*M_PI/180.0);
    rightForceVec[1] = cos(mRightVertical*M_PI/180.0) * sin(mRightHorizontal*M_PI/180.0);
    rightForceVec[2] = sin(mRightVertical*M_PI/180.0);

    leftForceVec[0] = cos(mLeftVertical*M_PI/180.0) * cos(mLeftHorizontal*M_PI/180.0);
    leftForceVec[1] = cos(mLeftVertical*M_PI/180.0) * sin(mLeftHorizontal*M_PI/180.0);
    leftForceVec[2] = sin(mLeftVertical*M_PI/180.0);

	rightForceVec *= atof(mdRightMag);
	leftForceVec *= atof(mdLeftMag);

	// if there has been no change, we don't have to set the values and we shouldn't make a new undo event
	if(rightForceVec[0] == oldRF[0] &&
		rightForceVec[1] == oldRF[1] &&
		rightForceVec[2] == oldRF[2] &&
		leftForceVec[0] == oldLF[0] &&
		leftForceVec[1] == oldLF[1] &&
		leftForceVec[2] == oldLF[2]) {
		return;
	}

	// TODO testing new multi-frame dialog and undo
	// make new group event
	int left = mDocPtr->LeftSelect();
	int right = mDocPtr->RightSelect();
	GroupEvent* groupEvent = new GroupEvent(left, right);

	Skeleton* skelPtr;
	for(int i = left; i <= right; i++) {
		skelPtr = mDocPtr->getSkeletonAtFrame(i);
		oldLF = skelPtr->getExtForce(JT_LHAND);
		oldRF = skelPtr->getExtForce(JT_RHAND);
		oldLT = skelPtr->getExtTorque(JT_LHAND);
		oldRT = skelPtr->getExtTorque(JT_RHAND);

		// add individual frame event
		groupEvent->addEvent(new UndoableHandLoads(oldLF, oldRF, oldLT, oldRT,
													leftForceVec, rightForceVec, oldLT, oldRT, i));
		// set new forces
		skelPtr->setExtForce(JT_LHAND, leftForceVec);
		skelPtr->setExtForce(JT_RHAND, rightForceVec);
	}
	// add group event to history
	mDocPtr->addUndoEvent(groupEvent);
	mDocPtr->MakeDirtyAndUpdateViews(true);
	UndoButtonStatus();
    RedoButtonStatus();

/*	skel.setExtForce(JT_LHAND, leftForceVec);
	skel.setExtForce(JT_RHAND, rightForceVec);

    mDocPtr->addUndoEvent(new UndoableHandLoads(oldLF,oldRF,oldLT,oldRT));
    mDocPtr->MakeDirtyAndUpdateViews(true);*/
}

void Dg_HandLoads::OnRedrawButton() 
{
    //SetFocus();

    if(!VerifyTotalsInRange())
        return;

    UpdateDocument();
}

void Dg_HandLoads::OnOK() 
{   
    SetFocus();

    if(!VerifyTotalsInRange())
        return;

    UpdateDocument();
    CDialog::OnOK();
}

void Dg_HandLoads::OnIncrementPlus(void) { DoIncrement(true); }
void Dg_HandLoads::OnIncrementMinus(void) { DoIncrement(false); }

void Dg_HandLoads::HandForceKillFocus(int box_ID)
{
    switch(box_ID)
    {
        case IDC_LEFT_VERTICAL: OnKillfocusLeftVertical(); break;
        case IDC_LEFT_HORIZONTAL: OnKillfocusLeftHorizontal(); break;
        case IDC_RIGHT_VERTICAL: OnKillfocusRightVertical(); break;
        case IDC_RIGHT_HORIZONTAL: OnKillfocusRightHorizontal(); break;
        case IDC_RIGHT_MAG: OnKillfocusRightMag(); break;
        case IDC_LEFT_MAG: OnKillfocusLeftMag(); break;
    }
}

void Dg_HandLoads::DoIncrement(bool increment_up)
{
    // increment_up is 1 if we're adding, 0 if subtracting
    int sign_multiplier = (increment_up) ? 1 : -1;
	//UpdateData(TRUE);
	if(m_checkhand == 1)  //if the checkbox is selected
	{
		this->Current_Control_ID = IDC_RIGHT_MAG;      //let the focus points to the right mag value
		if( this->UpdateData( TRUE ) ) //Make sure data OK 
        {
            char oldValText[50], newValText[50];
            char* stopstring;
            GetDlgItemText( this->Current_Control_ID, oldValText, 49);
            double Value = strtod(oldValText, &stopstring);
            double Prior_Value = Value;

            if( this->Increment_Forces_Group == 0 ) // '1' selected
                Value = Value + 1*sign_multiplier;
            else	// '5' or up selected
                Value = Value + 5*sign_multiplier * ( this->Increment_Forces_Group );

            _gcvt( Value, 5, newValText);
            SetDlgItemText( this->Current_Control_ID, newValText );

            // Pretend to kill the focus on the current control so
            //   that updating occurs correctly
            HandForceKillFocus(Current_Control_ID);

            VerifyTotalsInRange(false, true);
            OnRedrawButton();	// Update the data
        }    
		this->Current_Control_ID = IDC_LEFT_MAG;   // let the focus points to the left mag value
		if( this->UpdateData( TRUE ) ) //Make sure data OK 
        {
            char oldValText[50], newValText[50];
            char* stopstring;
            GetDlgItemText( this->Current_Control_ID, oldValText, 49);
            double Value = strtod(oldValText, &stopstring);
            double Prior_Value = Value;

            if( this->Increment_Forces_Group == 0 ) // '1' selected
                Value = Value + 1*sign_multiplier;
            else	// '5' or up selected
                Value = Value + 5*sign_multiplier * ( this->Increment_Forces_Group );

            _gcvt( Value, 5, newValText);
            SetDlgItemText( this->Current_Control_ID, newValText );

            // Pretend to kill the focus on the current control so
            //   that updating occurs correctly
            HandForceKillFocus(Current_Control_ID);

            VerifyTotalsInRange(false, true);
            OnRedrawButton();	// Update the data
        }    
		
	}
	else                      //if the checkbox is not selected
	{
    if( this->Current_Control_ID != 0 ) 
    {
        if( this->UpdateData( TRUE ) ) //Make sure data OK 
        {
            char oldValText[50], newValText[50];
            char* stopstring;
            GetDlgItemText( this->Current_Control_ID, oldValText, 49);
            double Value = strtod(oldValText, &stopstring);
            double Prior_Value = Value;

            if( this->Increment_Forces_Group == 0 ) // '1' selected
                Value = Value + 1*sign_multiplier;
            else	// '5' or up selected
                Value = Value + 5*sign_multiplier * ( this->Increment_Forces_Group );

            _gcvt( Value, 5, newValText);
            SetDlgItemText( this->Current_Control_ID, newValText );

            // Pretend to kill the focus on the current control so
            //   that updating occurs correctly
            HandForceKillFocus(Current_Control_ID);

            VerifyTotalsInRange(false, true);
            OnRedrawButton();	// Update the data
        }    
    }
    else ::MessageBox(NULL,"No angle/magnitude active.","Increment Angle",MB_OK | MB_ICONEXCLAMATION);

	}
}


//---------------------------------------------------------------------------
// If giveMessages is true, the function will spit out error boxes to the user.
// If constrainVars is true, the function will set the variables that
//		are out of range to the nearest in-range values.
bool Dg_HandLoads::VerifyTotalsInRange(bool giveMessages, bool constrainVars)
{
   bool test_failed = false;
   String msgStr = "";

   int lMaxHandForce = MAX_HAND_FORCE_LB;

   ///// Check if we should use metric or English limit /////
   if(mDocPtr->Is_Metric())
      lMaxHandForce = int(lMaxHandForce * U_LBF_NEWTONS); // Set Newton limit rather than lb

   ////////////////// Check for out-of-range force values //////////////////

   if( atof(mdLeftMag) > lMaxHandForce)
   {
      if(constrainVars)
		  mdLeftMag.Format("%.2f", lMaxHandForce);
      msgStr = "Left Hand";

      if(atof(mdRightMag) > lMaxHandForce)
         msgStr += " and ";
   }

   if(atof(mdRightMag) > lMaxHandForce)
   {
      if(constrainVars)
         mdRightMag.Format("%.2f", lMaxHandForce);

      msgStr += "Right Hand";
   }

   if(msgStr != String(""))  // if(error has occurred)
   {
      if(giveMessages)
      {
         CString messageCStr = msgStr;			// Copy the String to a CString
         CString finalStr;						// Contains the final output of message box
         finalStr.Format("%s:\nMaximum total force of %d %s exceeded, CAN NOT be solved under current settings!",
            messageCStr, lMaxHandForce, mLeftMagUnits);

         msgStr = finalStr;

         AfxMessageBox(msgStr);
      }

      test_failed = true;
      msgStr = String("");
   }

   ////////////////// Check for negative force values //////////////////

   if(atof(mdLeftMag) < 0)
   {
      if(constrainVars)
		  mdLeftMag.Format("%.2f", 0);
         

      msgStr = "Left Hand";

      if(atof(mdRightMag) < 0)
         msgStr += " and ";
   }

   if(atof(mdRightMag) < 0)
   {
      if(constrainVars)
         mdRightMag.Format("%.2f", 0);

      msgStr += "Right Hand";
   }

   if(msgStr != String(""))	// if(error has occurred)
   {
      if(giveMessages)
      {
         CString messageCStr = msgStr;			// Copy the String to a CString
         CString finalStr;
         finalStr.Format("%s:\nForce magnitudes must be positive.",
            messageCStr);

         msgStr = finalStr;
         AfxMessageBox(msgStr);
      }

      test_failed = true;
      msgStr = String("");
   }

   ////////////////// Check horizontal angles //////////////////

   if(fabs(mLeftHorizontal) > 180)
   {
      if(constrainVars)
         // Set the angle to 180 or -180
         mdLeftHorizontal = mLeftHorizontal = 
         180 * (mLeftHorizontal / fabs(mLeftHorizontal));

      msgStr = "Left Hand";

      if(fabs(mRightHorizontal) > 180)
         msgStr += " and ";
   }

   if(fabs(mRightHorizontal) > 180)
   {
      if(constrainVars)
         mdRightHorizontal = mRightHorizontal = 
         180 * (mRightHorizontal / fabs(mRightHorizontal));

      msgStr += "Right Hand";
   }

   if(msgStr != String(""))	// if(error has occurred)
   {
      if(giveMessages)
         AfxMessageBox(msgStr + ":\nValid range for horizontal angles is -180 to 180.");

      test_failed = true;
      msgStr = String("");
   }

   ////////////////// Check vertical angles //////////////////

   if(fabs(mLeftVertical) > 90)
   {
      if(constrainVars)
         mdLeftVertical = mLeftVertical = 
         90 * (mLeftVertical / fabs(mLeftVertical));

      msgStr = "Left Hand";

      if(fabs(mRightVertical) > 90)
         msgStr += " and ";
   }

   if(fabs(mRightVertical) > 90)
   {
      if(constrainVars)
         mdRightVertical = mRightVertical = 
         90 * (mRightVertical / fabs(mRightVertical));

      msgStr = "Right Hand";
   }

   if(msgStr != String(""))	// if(error has occurred)
   {
      if(giveMessages)
         AfxMessageBox(msgStr + ":\nValid range for vertical angles is -90 to 90.");

      test_failed = true;
      msgStr = String("");
   }

   ////////////////// If we might have changed variables, //////////////////
   //////////////////      update them in the dialog      //////////////////
   if(constrainVars)
   {
      UpdateData(FALSE); // Send it out to the dlg
      HandForceKillFocus(this->Current_Control_ID); // Store it in the main variables
   }

   ////////////////// Return //////////////////

   if(test_failed)
      return false;	// :-(
   else
      return true;	// All values good!
}

//---------------------------------------------------------------------------
void Dg_HandLoads::CheckAndUpdate() 
{
   HorizontalForceAngleStatus( mdLeftVertical, mLeftHorizontalControl );
   HorizontalForceAngleStatus( mdRightVertical, mRightHorizontalControl );
   this->UpdateWindow();
   return;	
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnSetfocusLeftForce()
{
   UpdateData( TRUE );
   this->Current_Control_ID =  IDC_LEFT_MAG;
   CheckAndUpdate();
   UpdateData( FALSE );
   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnSetfocusRightForce()
{
   UpdateData( TRUE );
   this->Current_Control_ID =  IDC_RIGHT_MAG;
   CheckAndUpdate();
   UpdateData( FALSE );
   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnSetfocusLeftVerAngle()
{
   UpdateData( TRUE );
   mLeftEffortGroup = 0;
   this->Current_Control_ID =  IDC_LEFT_VERTICAL;
   UpdateData( FALSE );
   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnSetfocusLeftHorAngle()
{
   UpdateData( TRUE );
   mLeftEffortGroup = 0;
   this->Current_Control_ID =  IDC_LEFT_HORIZONTAL;
   UpdateData( FALSE );
   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnSetfocusRightVerAngle()
{
   UpdateData( TRUE );
   mRightEffortGroup = 0;
   this->Current_Control_ID =  IDC_RIGHT_VERTICAL;
   UpdateData( FALSE );
   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnSetfocusRightHorAngle()
{
   UpdateData( TRUE );
   mRightEffortGroup = 0;
   this->Current_Control_ID =  IDC_RIGHT_HORIZONTAL;
   UpdateData( FALSE );
   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads:: OnKillfocusLeftVertical() 
{
   UpdateData( TRUE );
   mLeftVertical = mdLeftVertical;

   mLeftExertion = (char*)ExertionText(mLeftVertical, mLeftHorizontal);
   UpdateData( FALSE );
   CheckAndUpdate();
   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnKillfocusLeftHorizontal() 
{
   UpdateData( TRUE );
   mLeftHorizontal = mdLeftHorizontal;

   mLeftExertion = (char*)ExertionText(mLeftVertical, mLeftHorizontal);
   UpdateData( FALSE );
   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnKillfocusRightVertical() 
{
   UpdateData( TRUE );
   mRightVertical = mdRightVertical;

   mRightExertion = (char*)ExertionText(mRightVertical, mRightHorizontal);
   UpdateData( FALSE );
   CheckAndUpdate();
   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnKillfocusRightHorizontal() 
{
   UpdateData(TRUE);
   mRightHorizontal = mdRightHorizontal;

   mRightExertion = (char*)ExertionText(mRightVertical, mRightHorizontal);
   UpdateData(FALSE);
}


//---------------------------------------------------------------------------
void Dg_HandLoads::OnKillfocusRightMag() 
{
   UpdateData(TRUE);
   UpdateData(FALSE);
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnKillfocusLeftMag() 
{
   UpdateData(TRUE);
   UpdateData(FALSE);
}


//---------------------------------------------------------------------------
void Dg_HandLoads::OnPaint() 
{
   HorizontalForceAngleStatus( mdLeftVertical, mLeftHorizontalControl );
   HorizontalForceAngleStatus( mdRightVertical, mRightHorizontalControl );
   CWnd::OnPaint();
   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnLeftEffortGroup() 
{
   mLeftVerticalControl.EnableWindow( TRUE );
   CheckAndUpdate();
   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads:: UpdateLeftEffort( void )
{
   mLeftExertion = (char*)ExertionText(mLeftVertical, mLeftHorizontal);
   CheckAndUpdate();
   UpdateData( FALSE );

   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnLeftExertLeft()
{
   UpdateData( TRUE );

   mLeftVertical   = 0;
   mLeftHorizontal = 0;
   mdLeftVertical   = 0;
   mdLeftHorizontal = 0;
   this->UpdateLeftEffort();

   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnLeftExertRight() 
{
   UpdateData( TRUE );

   mLeftVertical   =   0;
   mLeftHorizontal = 180;
   mdLeftVertical   =   0;
   mdLeftHorizontal = 180;

   this->UpdateLeftEffort();

   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnLeftLift() 
{
   UpdateData( TRUE );

   mLeftVertical   = -90;
   mLeftHorizontal =  90;
   mdLeftVertical   = -90;
   mdLeftHorizontal =  90;

   this->UpdateLeftEffort();

   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnLeftLower() 
{
   UpdateData( TRUE );

   mLeftVertical   = 90;
   mLeftHorizontal = 90;
   mdLeftVertical   = 90;
   mdLeftHorizontal = 90;

   this->UpdateLeftEffort();

   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnLeftPullBack() 
{
   UpdateData( TRUE );

   mLeftVertical   =  0;
   mLeftHorizontal = 90;
   mdLeftVertical   =  0;
   mdLeftHorizontal = 90;

   this->UpdateLeftEffort();

   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnLeftPushForward() 
{
   UpdateData( TRUE );

   mLeftVertical   =   0;
   mLeftHorizontal = -90;
   mdLeftVertical   =   0;
   mdLeftHorizontal = -90;

   this->UpdateLeftEffort();

   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads:: UpdateRightEffort( void ) 
{
   mRightExertion = (char*)ExertionText(mRightVertical, mRightHorizontal);
   CheckAndUpdate();
   UpdateData( FALSE );

   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnRightEffortGroup() 
{
   mRightVerticalControl.EnableWindow(TRUE);
   CheckAndUpdate();
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnRightExertLeft() 
{
   UpdateData( TRUE );

   mRightVertical   =   0;
   mRightHorizontal =   0;
   mdRightVertical   =   0;
   mdRightHorizontal =   0;

   this->UpdateRightEffort();
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnRightExertRight() 
{
   UpdateData(TRUE);

   mRightVertical = 0;
   mRightHorizontal = 180;
   mdRightVertical = 0;
   mdRightHorizontal = 180;

   this->UpdateRightEffort();
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnRightLift() 
{
   UpdateData( TRUE );

   mRightVertical   = -90;
   mRightHorizontal =  90;
   mdRightVertical   = -90;
   mdRightHorizontal =  90;

   this->UpdateRightEffort();

   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnRightLower() 
{
   UpdateData( TRUE );

   mRightVertical   =  90;
   mRightHorizontal =  90;
   mdRightVertical   =  90;
   mdRightHorizontal =  90;

   this->UpdateRightEffort();
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnRightPullBack() 
{
   UpdateData( TRUE );

   mRightVertical   =   0;
   mRightHorizontal =  90;
   mdRightVertical   =   0;
   mdRightHorizontal =  90;

   this->UpdateRightEffort();

   return;
}

//---------------------------------------------------------------------------
void Dg_HandLoads::OnRightPushForward() 
{
   UpdateData(TRUE);

   mRightVertical    =   0;
   mRightHorizontal  = -90;
   mdRightVertical   =   0;
   mdRightHorizontal = -90;

   this->UpdateRightEffort();
}

void Dg_HandLoads::OnEnChangeRightVertical()
{
   int tempInt = GetDlgItemInt(IDC_RIGHT_VERTICAL);
   HorizontalForceAngleStatus(tempInt, mRightHorizontalControl);
}

void Dg_HandLoads::OnEnChangeLeftVertical()
{
   int tempInt = GetDlgItemInt(IDC_LEFT_VERTICAL);
   HorizontalForceAngleStatus(tempInt, mLeftHorizontalControl);
}

// Arguments: vertical and horizontal angles
String Dg_HandLoads::ExertionText(double aV,double aH)
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

void Dg_HandLoads::OnBnClickedHandLoadsAdv()
{
	openAdvHandLoads = true;
	CDialog::OnOK();
}


void Dg_HandLoads::OnBnClickedCheckhand()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_checkhand)
	{
	//GetDlgItem(2596) -> EnableWindow( FALSE );
	mRightVerticalControl.EnableWindow(FALSE);
	mRightHorizontalControl.EnableWindow(FALSE);
	mLeftVerticalControl.EnableWindow(FALSE);
	mLeftVerticalControl.EnableWindow(FALSE);
	}
	else
	{
	//GetDlgItem(2596) -> EnableWindow(TRUE);
	mRightVerticalControl.EnableWindow(TRUE);
	mRightHorizontalControl.EnableWindow(TRUE);
	mLeftVerticalControl.EnableWindow(TRUE);
	mLeftVerticalControl.EnableWindow(TRUE);
	}
	

}

void Dg_HandLoads::OnClickedIncmax()
{
	UpdateData(TRUE);
	UpdateDocument();
	// TODO: Add your control notification handler code here
	if(m_Strength == TRUE 
	&&m_wrist == FALSE 
	&& m_elbow == FALSE 
	&&m_shoulder == FALSE
	&&m_torso == FALSE
	&&m_hip == FALSE
	&&m_knee == FALSE
	&&m_ankle == FALSE)
	{
		MessageBox(_T("WARNING: Please select at least one OPTION for strength limit!"));
	}
	else if(0 == this->Increment_Solver_Group && (this->m_BackCompression != FALSE || this->m_Strength != FALSE || this->m_Balance != FALSE))
	{
		
		IncreaseLeft();
	}
	else if(1 == this->Increment_Solver_Group && (this->m_BackCompression != FALSE || this->m_Strength != FALSE || this->m_Balance != FALSE))
	{
		IncreaseRight();
	}
	else if(2 == this->Increment_Solver_Group && (this->m_BackCompression != FALSE || this->m_Strength != FALSE || this->m_Balance != FALSE))
	{
 		IncreaseBoth();
	}
	else if(-1 == this->Increment_Solver_Group && (this->m_BackCompression != FALSE || this->m_Strength != FALSE || this->m_Balance != FALSE))
	{
		MessageBox(_T("WARNING:Please select one OPTION for the solver!"));
	}
	else if(-1 != this->Increment_Solver_Group && this->m_BackCompression == FALSE && this->m_Strength == FALSE && this->m_Balance == FALSE)
	{
		MessageBox(_T("WARNING:Please select one CONDITION/LIMITATION for the solver!"));
	}
	else if(-1 == this->Solver_Condition_Group && this->m_BackCompression == FALSE && this->m_Strength == FALSE && this->m_Balance == FALSE )
	{
		MessageBox(_T("WARNING:Please select one OPTION and one CONDITION/LIMITATION for the solver!"));
	}
	
}
bool Dg_HandLoads::islimit(void)
{
	static int flag =0;  // for judging balance status
	Skeleton &skel = *mDocPtr->GetSkeleton();
	const AnalysisSummaryData_s& analysis_summary = skel.getAnalysisSummary();
	AnalysisSummaryLimits limits = skel.getAnalysisSummaryLimits();
	Skeleton &lSkeleton = *mDocPtr->GetSkeleton();
	const BalanceData_s& balance = lSkeleton.getBalanceData();
	UpdateData(TRUE);
	if(this->m_BackCompression == TRUE && this->m_Strength == FALSE && this->m_Balance == FALSE)
	{
		if(analysis_summary.L4L5Compression <= CompressionLimit()) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(this->m_BackCompression == FALSE && this->m_Strength == TRUE && this->m_Balance == FALSE)
	{
		UpdateData(TRUE);
		if((Round_Double(analysis_summary.WristMinPercentile) >= StrengthLimit() || !m_wrist)
	   &&(Round_Double(analysis_summary.ElbowMinPercentile) >= StrengthLimit()  || !m_elbow)
	   &&(Round_Double(analysis_summary.ShoulderMinPercentile) >= StrengthLimit() ||!m_shoulder)
	   &&(Round_Double(analysis_summary.TorsoMinPercentile) >= StrengthLimit() || !m_torso)
	   &&(Round_Double(analysis_summary.HipMinPercentile) >= StrengthLimit() || !m_hip)
	   &&(Round_Double(analysis_summary.KneeMinPercentile) >= StrengthLimit() || !m_knee ) 
	   &&(Round_Double(analysis_summary.AnkleMinPercentile) >= StrengthLimit() ||!m_ankle))
		{
			analysis_summary.WristMinPercentile;
 		return true;
		}
		else 
			return false;

		/*int index;
		index = m_combostrength.GetCurSel();
		if (index == 0)
		{
			if(Round_Double(analysis_summary.WristMinPercentile) >= StrengthLimit()) 
			{
				return true;
			}
			else
				{
					return false;
				}
		}

		if (index == 1)
		{
			if(Round_Double(analysis_summary.ElbowMinPercentile) >= StrengthLimit() )
			{
				return true;
			}
			else
				{
					return false;
				}
		}

		if (index == 2)
		{
			if(Round_Double(analysis_summary.ShoulderMinPercentile) >= StrengthLimit()) 
			{
				return true;
			}
			else
				{
					return false;
				}
		}

	    if (index == 3)
		{
			if(Round_Double(analysis_summary.TorsoMinPercentile) >= StrengthLimit()) 
			{
				return true;
			}
			else
				{
					return false;
				}
		}

		if (index == 4)
		{
			if( Round_Double(analysis_summary.HipMinPercentile) >= StrengthLimit()) 
			{
				return true;
			}
			else
				{
					return false;
				}
		}
		if (index == 5)
		{
			if(Round_Double(analysis_summary.KneeMinPercentile)>= StrengthLimit()) 
			{
				return true;
			}
			else
				{
					return false;
				}
		}

		if (index == 6)
		{
			if(Round_Double(analysis_summary.AnkleMinPercentile) >= StrengthLimit()) 
				{
					return true;
				}
			else
				{
					return false;
				}
		}

		else
		{
			if(Round_Double(analysis_summary.WristMinPercentile) >= StrengthLimit() &&  Round_Double(analysis_summary.ElbowMinPercentile) >= StrengthLimit() &&
				Round_Double(analysis_summary.ShoulderMinPercentile) >= StrengthLimit() && Round_Double(analysis_summary.TorsoMinPercentile) >= StrengthLimit() &&
				Round_Double(analysis_summary.HipMinPercentile) >= StrengthLimit() && Round_Double(analysis_summary.KneeMinPercentile)>= StrengthLimit() &&
				Round_Double(analysis_summary.AnkleMinPercentile) >= StrengthLimit())
				{
					return true;
				}
			else
				{
					return false;
				}
		}	*/

	}

	if(this->m_BackCompression == FALSE && this->m_Strength == FALSE && this->m_Balance == TRUE)
	{
		if(dichotomyflag == false)
		{
			if(balance.BalanceStatus != BS_Acceptable)
			{
				if (flag == 1)
				{
					flag = 0;
					return false;
				}	
				return true;
			}
			else
			{
				if(flag == 0)
				{
					flag = flag + 1; //flag = 1 stands for the first time enter balance status
					return true;
				}
				else 
					return true;
			}
		}
		else
		{ 
			if(balance.BalanceStatus != BS_Acceptable)
				return false;
			else 
				return true;
		}
		
	}

	if(this->m_BackCompression == TRUE && this->m_Strength == TRUE && this->m_Balance == FALSE)
	{
		if((Round_Double(analysis_summary.WristMinPercentile) >= StrengthLimit() || !m_wrist)
	   &&(Round_Double(analysis_summary.ElbowMinPercentile) >= StrengthLimit()  || !m_elbow)
	   &&(Round_Double(analysis_summary.ShoulderMinPercentile) >= StrengthLimit() ||!m_shoulder)
	   &&(Round_Double(analysis_summary.TorsoMinPercentile) >= StrengthLimit() || !m_torso)
	   &&(Round_Double(analysis_summary.HipMinPercentile) >= StrengthLimit() || !m_hip)
	   &&(Round_Double(analysis_summary.KneeMinPercentile) >= StrengthLimit() || !m_knee ) 
	   &&(Round_Double(analysis_summary.AnkleMinPercentile) >= StrengthLimit() ||!m_ankle)
	   && analysis_summary.L4L5Compression <= CompressionLimit()) 
		{
			return true;
		}
		else
		{
 			return false;
		}
	}

	if (this->m_BackCompression == TRUE && this->m_Strength == FALSE && this->m_Balance == TRUE)
	{
		if(analysis_summary.L4L5Compression <= CompressionLimit() && isbalance())
		{
			return true;
		}
		else
			return false;

	}
	if (this->m_BackCompression == FALSE && this->m_Strength == TRUE && this->m_Balance == TRUE)
	{
		if((Round_Double(analysis_summary.WristMinPercentile) >= StrengthLimit() || !m_wrist)
	   &&(Round_Double(analysis_summary.ElbowMinPercentile) >= StrengthLimit()  || !m_elbow)
	   &&(Round_Double(analysis_summary.ShoulderMinPercentile) >= StrengthLimit() ||!m_shoulder)
	   &&(Round_Double(analysis_summary.TorsoMinPercentile) >= StrengthLimit() || !m_torso)
	   &&(Round_Double(analysis_summary.HipMinPercentile) >= StrengthLimit() || !m_hip)
	   &&(Round_Double(analysis_summary.KneeMinPercentile) >= StrengthLimit() || !m_knee ) 
	   &&(Round_Double(analysis_summary.AnkleMinPercentile) >= StrengthLimit() ||!m_ankle)
	   && isbalance()) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if(this->m_BackCompression == TRUE && this->m_Strength == TRUE && this->m_Balance == TRUE)
	{
		if((Round_Double(analysis_summary.WristMinPercentile) >= StrengthLimit() || !m_wrist)
	   &&(Round_Double(analysis_summary.ElbowMinPercentile) >= StrengthLimit()  || !m_elbow)
	   &&(Round_Double(analysis_summary.ShoulderMinPercentile) >= StrengthLimit() ||!m_shoulder)
	   &&(Round_Double(analysis_summary.TorsoMinPercentile) >= StrengthLimit() || !m_torso)
	   &&(Round_Double(analysis_summary.HipMinPercentile) >= StrengthLimit() || !m_hip)
	   &&(Round_Double(analysis_summary.KneeMinPercentile) >= StrengthLimit() || !m_knee ) 
	   &&(Round_Double(analysis_summary.AnkleMinPercentile) >= StrengthLimit() ||!m_ankle)
	   && analysis_summary.L4L5Compression <= CompressionLimit()
	   && isbalance()) 
		{
			return true;
		}
		else
		{
			return false;
		} 
	} 
	else                            //just to avoid warning,it does not matter in the code
	{
		return false;
	}							
}

void Dg_HandLoads::IncreaseLeft(void)
{
	char zero[5];
	int num =0; // start point
	int cnt = 0;
	double Value =0;
	int rightmax;
	int n =0;
	itoa(num,zero,10);
	bool flag = false; // check if we can go into balanced status
	dichotomyflag = false;
	bool solutionfound =  true;//check if we can find a solution under current settings
	SetDlgItemText( IDC_LEFT_MAG, zero); 
	Skeleton &lSkeleton = *mDocPtr->GetSkeleton();
	const BalanceData_s& balance = lSkeleton.getBalanceData();
	HandForceKillFocus(IDC_LEFT_MAG);
    VerifyTotalsInRange(false, true);
	OnRedrawButton();	// Update the data

	while(islimit())                                //if all the percentiles are larger than 25%,it will stay in loop
	{   
		char oldValText[50], newValText[50];
		char* stopstring;
        GetDlgItemText(IDC_LEFT_MAG, oldValText, 49);
		Value = strtod(oldValText, &stopstring);
		Value = Value + pow(2,n);
		n++;
		gcvt( Value, 5, newValText);
        SetDlgItemText( IDC_LEFT_MAG, newValText );
		HandForceKillFocus(IDC_LEFT_MAG);
	    VerifyTotalsInRange(false, true);
	    OnRedrawButton();	// Update the data
		if (this->m_Balance == TRUE && balance.BalanceStatus != BS_Acceptable && n > 12)
		{
			flag = true;   // it means can not get to balance status
			Value = 0;
			gcvt( Value, 5, newValText);
		    SetDlgItemText( IDC_LEFT_MAG, newValText );
			HandForceKillFocus(IDC_LEFT_MAG);
		    VerifyTotalsInRange(false, true);
		    OnRedrawButton();	// Update the data
			break;
		}
	}
	while(n == 0)
	{
		num = num + 10;
		cnt++;
		itoa(num,zero,10);
		SetDlgItemText( IDC_LEFT_MAG,  zero);            // let mag of both hands be zero ,then begins the loop
		HandForceKillFocus(IDC_LEFT_MAG);
		VerifyTotalsInRange(false, true);
		OnRedrawButton();	// Update the data
		while(islimit())                                
		{   

			char oldValTextLeft[50], newValTextLeft[50];
			char* stopstring1;
			GetDlgItemText( IDC_LEFT_MAG, oldValTextLeft, 49);
			Value = strtod(oldValTextLeft, &stopstring1);
			Value = Value + pow(2,n);
			n++;
			gcvt( Value, 5, newValTextLeft);
			SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
			HandForceKillFocus( IDC_LEFT_MAG);
			VerifyTotalsInRange(false, true);
			OnRedrawButton();	// Update the data
			if (this->m_Balance == TRUE && balance.BalanceStatus != BS_Acceptable && n > 12)
			{
				flag = true;   // it means can not get to balance status
				Value = 0;
				gcvt( Value, 5, newValTextLeft);
				SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
				HandForceKillFocus( IDC_LEFT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data
				break;
			}
		}
		if (cnt == 10)
			{
				solutionfound = false;
				char oldValTextLeft[50], newValTextLeft[50];
				cnt = 0;
				Value = 0;
				gcvt( Value, 5, newValTextLeft);
				SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
				HandForceKillFocus(IDC_LEFT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data
				MessageBox("Warning: No solution found!");
				break;
			}
	}
		// the solution is between 2^(n-1) ~ (2^n)-1
	if(solutionfound)
	{
		if (flag == false)
		{
			dichotomyflag = true;
			int a = num + pow(2,n-1)-1;
			int b = num + pow(2,n)-1;  // solution between a ~ b
			while(b - a > 1 )
			{
				int c = (a + b)/2;
				char oldValText[50], newValText[50];
				char* stopstring;
				GetDlgItemText(IDC_LEFT_MAG, oldValText, 49);
				Value = strtod(oldValText, &stopstring);
				Value = c;
				gcvt( Value, 5, newValText);
				SetDlgItemText( IDC_LEFT_MAG, newValText );
				HandForceKillFocus(IDC_LEFT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();
				if(!islimit()) 
					b = c;
				else 
 					a = c;
			}
		}
		else 
		{
			MessageBox("WARNING: CAN NOT reach the acceptable balance in current posture! ");
		}
		const BalanceData_s& balance1 = lSkeleton.getBalanceData();
		if (this->m_Balance == TRUE && balance1.BalanceStatus == BS_Critical)
		{
			char oldValText[50], newValText[50];
			char* stopstring;
			GetDlgItemText(IDC_LEFT_MAG, oldValText, 49);
			Value = strtod(oldValText, &stopstring);
			Value = Value - 1;
			gcvt( Value, 5, newValText);
			SetDlgItemText( IDC_LEFT_MAG, newValText );
			HandForceKillFocus(IDC_LEFT_MAG);
			VerifyTotalsInRange(false, true);
			OnRedrawButton();	// Update the data
		}
		else 
		{
			if(!islimit())
			{
				char oldValTextLeft[50], newValTextLeft[50];
				char* stopstring1;
				GetDlgItemText( IDC_LEFT_MAG, oldValTextLeft, 49);
				Value = strtod(oldValTextLeft, &stopstring1);
				Value = Value -1;
				gcvt( Value, 5, newValTextLeft);
				SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
				HandForceKillFocus( IDC_LEFT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data
			}
		}
	}
}
 

void Dg_HandLoads::IncreaseRight(void)
{
	char zero[5];
	int num = 0; //  start point
	double Value = 0;
	int leftmax;
	int n = 0; 
	int cnt = 0;
	bool flag = false; // check if we can gointo balanced status
	bool solutionfound = true; //check if we can find a solution under current settings
	dichotomyflag = false;
	gcvt(num, 5, zero);
	SetDlgItemText( IDC_RIGHT_MAG, zero); 
	HandForceKillFocus(IDC_RIGHT_MAG);
    VerifyTotalsInRange(false, true);
	OnRedrawButton();	// Update the data
	Skeleton &lSkeleton = *mDocPtr->GetSkeleton();
	BalanceData_s balance = lSkeleton.getBalanceData();
	while(islimit())                                
	{   
		char oldValText[50], newValText[50];
		char* stopstring;
        GetDlgItemText(IDC_RIGHT_MAG, oldValText, 49);
		Value = strtod(oldValText, &stopstring);
		Value = Value + pow(2,n);
		n++;
		gcvt( Value, 5, newValText);
        SetDlgItemText( IDC_RIGHT_MAG, newValText );
		HandForceKillFocus(IDC_RIGHT_MAG);
	    VerifyTotalsInRange(false, true);
	    OnRedrawButton();	// Update the data
		if (this->m_Balance == TRUE && balance.BalanceStatus != BS_Acceptable && n > 12)
		{
			flag = true;   // it means can not get to balance status
			Value = 0;
			gcvt( Value, 5, newValText);
		    SetDlgItemText( IDC_RIGHT_MAG, newValText );
			HandForceKillFocus(IDC_RIGHT_MAG);
		    VerifyTotalsInRange(false, true);
		    OnRedrawButton();	// Update the data
			break;
		}
	}
	while(n == 0)
	{
		num = num + 10;
		cnt++;
		itoa(num,zero,10);
		SetDlgItemText( IDC_RIGHT_MAG, zero);            // let mag of both hands be zero ,then begins the loop
		HandForceKillFocus(IDC_RIGHT_MAG);
		VerifyTotalsInRange(false, true);
		OnRedrawButton();	// Update the data
		while(islimit())                                
		{   
			char oldValTextRight[50], newValTextRight[50];
			char* stopstring;
			GetDlgItemText( IDC_RIGHT_MAG, oldValTextRight, 49);
			Value = strtod(oldValTextRight, &stopstring);
			Value = Value + pow(2,n);
			gcvt( Value, 5, newValTextRight);
			SetDlgItemText( IDC_RIGHT_MAG, newValTextRight);
			HandForceKillFocus(IDC_RIGHT_MAG);
			VerifyTotalsInRange(false, true);
			OnRedrawButton();	// Update the data
			n++;
			if (this->m_Balance == TRUE && balance.BalanceStatus != BS_Acceptable && n > 12)
			{
				flag = true;   // it means can not get to balance status
				Value = 0;
				gcvt( Value, 5, newValTextRight);
				SetDlgItemText( IDC_RIGHT_MAG, newValTextRight );
				HandForceKillFocus(IDC_RIGHT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data
				break;
			}
			if (cnt == 10)
			{
				solutionfound = false;
				char oldValTextRight[50], newValTextRight[50];
				cnt = 0;
				Value = 0;
				gcvt( Value, 5, newValTextRight);
				SetDlgItemText( IDC_RIGHT_MAG, newValTextRight );
				HandForceKillFocus(IDC_RIGHT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data
				MessageBox("Warning: No solution found!");
				break;
			}
		}
	}
	// the solution is between 2^(n-1) ~ (2^n)-1
	if (solutionfound)
	{
		if (flag == false)
		{
		int a = num + pow(2,n-1)-1;
		int b = num + pow(2,n)-1;  // solution between a ~ b
		dichotomyflag = true;
		while(b - a > 1 )
		{
			int c = (a + b)/2;
			char oldValText[50], newValText[50];
			char* stopstring;
			GetDlgItemText(IDC_RIGHT_MAG, oldValText, 49);
			Value = strtod(oldValText, &stopstring);
			Value = c;
			gcvt( Value, 5, newValText);
			SetDlgItemText( IDC_RIGHT_MAG, newValText );
			HandForceKillFocus(IDC_RIGHT_MAG);
			VerifyTotalsInRange(false, true);
			OnRedrawButton();
			if(!islimit()) 
				b = c;
			else 
 				a = c;
			}
		}
		else
		{
			MessageBox(" Can not reach the acceptable balance in current posture! ");
		}
		const BalanceData_s& balance1 = lSkeleton.getBalanceData();
		if (this->m_Balance == TRUE && balance1.BalanceStatus == BS_Critical)
		{
			char oldValText[50], newValText[50];
			char* stopstring;
			GetDlgItemText(IDC_RIGHT_MAG, oldValText, 49);
			Value = strtod(oldValText, &stopstring);
			Value = Value - 1;
			gcvt( Value, 5, newValText);
			SetDlgItemText( IDC_RIGHT_MAG, newValText );
			HandForceKillFocus(IDC_RIGHT_MAG);
			VerifyTotalsInRange(false, true);
			OnRedrawButton();	// Update the data
		}
		else 
		{
			if(!islimit())
			{
				char oldValTextRight[50], newValTextRight[50];
				char* stopstring;
				GetDlgItemText( IDC_RIGHT_MAG, oldValTextRight, 49);
				Value = strtod(oldValTextRight, &stopstring);
				Value = Value - 1;
				gcvt( Value, 5, newValTextRight);
				SetDlgItemText( IDC_RIGHT_MAG, newValTextRight);
				HandForceKillFocus(IDC_RIGHT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data
			}
		}
	}
}


void Dg_HandLoads::IncreaseBoth(void)
{
	char zero[5];
	int num =0; //  start point
	double Value = 0 ,Value1=0;
	double ratio;
	int n =0;	
	int cnt = 0; // counter
	bool flag = false; //  check if we can go into balanced status 
	dichotomyflag = false;
	bool solutionfound = true;//check if we can find a solution under current settings
	char oldleft[50],oldright[50];
	double oldLeft,oldRight;
	char* stopstring;
	Skeleton &lSkeleton = *mDocPtr->GetSkeleton();
	const BalanceData_s& balance = lSkeleton.getBalanceData();
    GetDlgItemText( IDC_LEFT_MAG, oldleft, 49);
	GetDlgItemText( IDC_RIGHT_MAG, oldright, 49);
	oldLeft = strtod(oldleft, &stopstring);
	oldRight = strtod(oldright, &stopstring);
	if ( 0 == oldLeft || 0 == oldRight)
	{
		ratio = 1;
	}
	else
	{
		ratio = oldLeft/oldRight; 
	}
	itoa(num,zero,10);
	SetDlgItemText( IDC_RIGHT_MAG, zero);
	SetDlgItemText( IDC_LEFT_MAG,  zero);            // let mag of both hands be zero ,then begins the loop
	HandForceKillFocus(IDC_RIGHT_MAG);
    VerifyTotalsInRange(false, true);
	OnRedrawButton();	// Update the data
	while(islimit())                                
	{   
		char oldValTextRight[50], newValTextRight[50];
		char* stopstring;
        GetDlgItemText( IDC_RIGHT_MAG, oldValTextRight, 49);
		Value = strtod(oldValTextRight, &stopstring);
		Value = Value + pow(2,n);
		gcvt( Value, 5, newValTextRight);
        SetDlgItemText( IDC_RIGHT_MAG, newValTextRight);
		HandForceKillFocus(IDC_RIGHT_MAG);
	    VerifyTotalsInRange(false, true);
	    OnRedrawButton();	// Update the data

		char oldValTextLeft[50], newValTextLeft[50];
        char* stopstring1;
        GetDlgItemText( IDC_LEFT_MAG, oldValTextLeft, 49);
		Value1 = strtod(oldValTextLeft, &stopstring1);
		Value1 = Value1 + ratio * pow(2,n);
		n++;
		gcvt( Value1, 5, newValTextLeft);
        SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
		HandForceKillFocus( IDC_LEFT_MAG);
        VerifyTotalsInRange(false, true);
	    OnRedrawButton();	// Update the data
		if (this->m_Balance == TRUE && balance.BalanceStatus != BS_Acceptable && n > 12)
		{
			flag = true;   // it means can not get to balance status
			Value = 0;
			gcvt( Value, 5, newValTextRight);
		    SetDlgItemText( IDC_RIGHT_MAG, newValTextRight );
			HandForceKillFocus(IDC_RIGHT_MAG);
		    VerifyTotalsInRange(false, true);
		    OnRedrawButton();	// Update the data
			Value1 = 0;
			gcvt( Value1, 5, newValTextLeft);
		    SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
			HandForceKillFocus( IDC_LEFT_MAG);
            VerifyTotalsInRange(false, true);
	        OnRedrawButton();	// Update the data
			break;
		}
	}
	while(n == 0)
	{
 		num = num + 10;
		cnt++;
		itoa(num,zero,10);
		SetDlgItemText( IDC_RIGHT_MAG, zero);
		SetDlgItemText( IDC_LEFT_MAG,  zero);            // let mag of both hands be zero ,then begins the loop
		HandForceKillFocus(IDC_RIGHT_MAG);
		VerifyTotalsInRange(false, true);
		OnRedrawButton();	// Update the data
		while(islimit())                                
		{   
			char oldValTextRight[50], newValTextRight[50];
			char* stopstring;
			GetDlgItemText( IDC_RIGHT_MAG, oldValTextRight, 49);
			Value = strtod(oldValTextRight, &stopstring);
			Value = Value + pow(2,n);
			gcvt( Value, 5, newValTextRight);
			SetDlgItemText( IDC_RIGHT_MAG, newValTextRight);
			HandForceKillFocus(IDC_RIGHT_MAG);
			VerifyTotalsInRange(false, true);
			OnRedrawButton();	// Update the data

			char oldValTextLeft[50], newValTextLeft[50];
			char* stopstring1;
			GetDlgItemText( IDC_LEFT_MAG, oldValTextLeft, 49);
			Value1 = strtod(oldValTextLeft, &stopstring1);
			Value1 = Value1 + ratio * pow(2,n);
			n++;
			gcvt( Value1, 5, newValTextLeft);
			SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
			HandForceKillFocus( IDC_LEFT_MAG);
			VerifyTotalsInRange(false, true);
			OnRedrawButton();	// Update the data
			if (this->m_Balance == TRUE && balance.BalanceStatus != BS_Acceptable && n > 12)
			{
				flag = true;   // it means can not get to balance status
				Value = 0;
				gcvt( Value, 5, newValTextRight);
				SetDlgItemText( IDC_RIGHT_MAG, newValTextRight );
				HandForceKillFocus(IDC_RIGHT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data
				Value1 = 0;
				gcvt( Value1, 5, newValTextLeft);
				SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
				HandForceKillFocus( IDC_LEFT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data
				break;
			}
		}
		if (cnt == 10)
			{
				solutionfound = false;
				char oldValTextLeft[50], newValTextLeft[50];
				char oldValTextRight[50], newValTextRight[50];
				cnt = 0;
				Value = 0;
				gcvt( Value, 5, newValTextRight);
				SetDlgItemText( IDC_RIGHT_MAG, newValTextRight );
				HandForceKillFocus(IDC_RIGHT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data
				Value1 = 0;
				gcvt( Value1, 5, newValTextLeft);
				SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
				HandForceKillFocus( IDC_LEFT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data
				MessageBox("Warning: No solution found!");
				break;
			}
	}
	if(solutionfound)
	{
		if(flag == false)
		{
			double a = num + pow(2,n-1)-1;
			int b = num + pow(2,n)-1;  // solution(right hand) between a ~ b
			int a1 = num + ratio * pow(2,n-1)-1;
			int b1 = num + ratio * pow(2,n)-1;  // solution(left hand) between a1 ~ b1
			dichotomyflag = true;
			while((b - a > 1)||(b1 - a1 > 1))
			{
				char oldValText[50], newValText[50];
				char* stopstring;
				GetDlgItemText(IDC_RIGHT_MAG, oldValText, 49);
				Value = strtod(oldValText, &stopstring);
				int c = (a + b)/2;
				Value = c;
				gcvt( Value, 5, newValText);
				SetDlgItemText( IDC_RIGHT_MAG, newValText );
				HandForceKillFocus(IDC_RIGHT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();
				char oldValText1[50], newValText1[50];
				char* stopstring1;
				GetDlgItemText(IDC_LEFT_MAG, oldValText1, 49);
				Value1 = strtod(oldValText1, &stopstring1);
				int c1 = (a1 + b1)/2;
				Value1 = c1;
				gcvt( Value1, 5, newValText1);
				SetDlgItemText( IDC_LEFT_MAG, newValText1 );
				HandForceKillFocus(IDC_LEFT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();
				if(!islimit()) 
					{
						b = c;
						b1 = c1;
					}
					else 
					{
						a = c;
 						a1 = c1;
					}
			}
		}
		else
		{
			MessageBox(" Can not reach the acceptable balance in current posture! ");
		} 
		const BalanceData_s& balance1 = lSkeleton.getBalanceData();
		if (this->m_Balance == TRUE && balance1.BalanceStatus == BS_Critical)
		{
			char oldValTextRight[50], newValTextRight[50];
			char* stopstring;
			GetDlgItemText( IDC_RIGHT_MAG, oldValTextRight, 49);
			Value = strtod(oldValTextRight, &stopstring);
			Value = Value - 1;
			gcvt( Value, 5, newValTextRight);
			SetDlgItemText( IDC_RIGHT_MAG, newValTextRight);
			HandForceKillFocus(IDC_RIGHT_MAG);
			VerifyTotalsInRange(false, true);
			OnRedrawButton();	// Update the data

			char oldValTextLeft[50], newValTextLeft[50];
			char* stopstring1;
			GetDlgItemText( IDC_LEFT_MAG, oldValTextLeft, 49);
			Value1 = strtod(oldValTextLeft, &stopstring1);
			Value1 = Value1 -1;
			gcvt( Value1, 5, newValTextLeft);
			SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
			HandForceKillFocus( IDC_LEFT_MAG);
			VerifyTotalsInRange(false, true);
			OnRedrawButton();	// Update the data
		}
		else 
		{
			if(!islimit())
			{
				char oldValTextRight[50], newValTextRight[50];
				char* stopstring;
				GetDlgItemText( IDC_RIGHT_MAG, oldValTextRight, 49);
				Value = strtod(oldValTextRight, &stopstring);
				Value = Value - 1;
				gcvt( Value, 5, newValTextRight);
				SetDlgItemText( IDC_RIGHT_MAG, newValTextRight);
				HandForceKillFocus(IDC_RIGHT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data

				char oldValTextLeft[50], newValTextLeft[50];
				char* stopstring1;
				GetDlgItemText( IDC_LEFT_MAG, oldValTextLeft, 49);
				Value1 = strtod(oldValTextLeft, &stopstring1);
				Value1 = Value1 -1;
				gcvt( Value1, 5, newValTextLeft);
				SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
				HandForceKillFocus( IDC_LEFT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data
			}
		}
	}
}



int Dg_HandLoads::StrengthLimit(void)
{
	int limitation;
	UpdateData();
	Skeleton &skel = *mDocPtr->GetSkeleton();
	const AnalysisSummaryData_s& analysis_summary = skel.getAnalysisSummary();
	AnalysisSummaryLimits limits = skel.getAnalysisSummaryLimits();
	if(mDocPtr->getLimitChoice() == LC_NIOSH) 
	{
		if (mDocPtr->getAnthropometryRef().Sex() == S_Male)
		{
			limitation = limits.SDL;
		}
		else	
		{
			limitation = limits.SDL;
		}
	}
	else
		{
			if (mDocPtr->getAnthropometryRef().Sex() == S_Male)
			{
				limitation = mDocPtr->getUpperPercentileLimit();
			}
			else
			{
				limitation = mDocPtr->getFemaleUpperPercentileLimit();
			}
	}
	return limitation;
}
int Dg_HandLoads::CompressionLimit(void)
{
	int limitation;
	Skeleton &skel = *mDocPtr->GetSkeleton();
	const AnalysisSummaryData_s& analysis_summary = skel.getAnalysisSummary();
	if (mDocPtr->getAnthropometryRef().Sex() == S_Male)
		{
			if(skel.getTask()->getMetricOrEnglish() == EnglishCns)
				limitation = 770;
			else 
				limitation = 3425;
		}
	else
		{
			if(skel.getTask()->getMetricOrEnglish() == EnglishCns)
				limitation = 770;
			else 
				limitation = 3425;
		}
	return limitation;
}

BOOL Dg_HandLoads::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	UndoButtonStatus();
    RedoButtonStatus();

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->AppendMenu(MF_SEPARATOR);// add a separator
	pSysMenu->AppendMenu(MF_STRING,IDR_MENU1,"Print Window");// add print menu
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void Dg_HandLoads::OnPrintPrintwindow()
{
	// TODO: Add your command handler code here
	 CRect m_rect;
	 GetClientRect(&m_rect); 
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
	  ::StretchDIBits(pdc.m_hDC,0,0,(int)((m_rect.Width()*ratex)),
	   (int)((m_rect.Height())*ratey),0,0,m_rect.Width(),
	   m_rect.Height(),pData,bInfo,DIB_RGB_COLORS,SRCCOPY);
	  pdc.EndDoc();
	  LocalFree(bInfo);
	  delete[] pData;
	  pData = NULL;
	  bmp.DeleteObject();
	 }
}


void Dg_HandLoads::OnSysCommand(UINT nID, LPARAM lParam)
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


void Dg_HandLoads::OnBnClickedResetzero()
{
	// TODO: Add your control notification handler code here
	char zero[5];int num =0;
	double Value = 0 ,Value1=0;
	char oldleft[50],oldright[50];
	double oldLeft,oldRight;
	char* stopstring;
	Skeleton &lSkeleton = *mDocPtr->GetSkeleton();
	const BalanceData_s& balance = lSkeleton.getBalanceData();
    GetDlgItemText( IDC_LEFT_MAG, oldleft, 49);
	GetDlgItemText( IDC_RIGHT_MAG, oldright, 49);
	oldLeft = strtod(oldleft, &stopstring);
	oldRight = strtod(oldright, &stopstring);
	itoa(num,zero,10);
	SetDlgItemText( IDC_RIGHT_MAG, zero);
	SetDlgItemText( IDC_LEFT_MAG,  zero);            // let mag of both hands be zero ,then begins the loop
	HandForceKillFocus(IDC_RIGHT_MAG);
    VerifyTotalsInRange(false, true);
	OnRedrawButton();	// Update the data
}


void Dg_HandLoads::OnBnClickedSelectAll()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE); 

	m_wrist = TRUE;
	m_elbow = TRUE;
	m_shoulder = TRUE;
	m_torso = TRUE;
	m_hip = TRUE;
	m_knee = TRUE;
	m_ankle = TRUE;

	UpdateData(FALSE);
}


void Dg_HandLoads::OnBnClickedNone()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_wrist = FALSE;
	m_elbow = FALSE;
	m_shoulder = FALSE;
	m_torso = FALSE;
	m_hip = FALSE;
	m_knee = FALSE;
	m_ankle = FALSE;

	UpdateData(FALSE);
}


/*void Dg_HandLoads::OnBnClickedStrength()
{
	// TODO: Add your control notification handler code here

	 if(m_Strength == TRUE)
	 {
		 wristcontrol.EnableWindow(FALSE);
		 elbowcontrol.EnableWindow(FALSE);
		 shouldercontrol.EnableWindow(FALSE);
		 torsocontrol.EnableWindow(FALSE);
		 hipcontrol.EnableWindow(FALSE);
		 kneecontrol.EnableWindow(FALSE);
		 ankelcontrol.EnableWindow(FALSE);
	 }
	 else            
	 {
		 wristcontrol.EnableWindow(TRUE);
		 elbowcontrol.EnableWindow(TRUE);
		 shouldercontrol.EnableWindow(TRUE);
		 torsocontrol.EnableWindow(TRUE);
		 hipcontrol.EnableWindow(TRUE);
		 kneecontrol.EnableWindow(TRUE);
		 ankelcontrol.EnableWindow(TRUE);
	 }
	 UpdateData(TRUE);
}*/



bool Dg_HandLoads::isbalance(void)
{
	static int flag =0;  // for judging balance status
	Skeleton &skel = *mDocPtr->GetSkeleton();
	const AnalysisSummaryData_s& analysis_summary = skel.getAnalysisSummary();
	AnalysisSummaryLimits limits = skel.getAnalysisSummaryLimits();
	Skeleton &lSkeleton = *mDocPtr->GetSkeleton();
	const BalanceData_s& balance = lSkeleton.getBalanceData();
	UpdateData(TRUE);
	if(dichotomyflag == false)
		{
			if(balance.BalanceStatus != BS_Acceptable)
			{
				if (flag == 1)
				{
					flag = 0;
					return false;
				}	
				return true;
			}
			else
			{
				if(flag == 0)
				{
					flag = flag + 1; //flag = 1 stands for the first time enter balance status
					return true;
				}
				else 
					return true;
			}
		}
	else
		{ 
			if(balance.BalanceStatus != BS_Acceptable)
				return false;
			else 
				return true;
		}
}

void Dg_HandLoads::OnBnClickedHandLoadSolver()
{
	// TODO: Add your control notification handler code here
	UpdateDocument();
	Dg_HandLoadSolver dgl(this);
	dgl.DoModal();
}

Dg_HandLoads::~Dg_HandLoads()
{
	mDocPtr->cancelCue();
}

BOOL Dg_HandLoads::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if(zDelta < 0) 
		DoIncrement(false);
	else if (zDelta > 0)
		DoIncrement(true);
	
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void Dg_HandLoads::OnBnClickedRedo()
{
	// TODO: Add your control notification handler code here
	if(!mDocPtr->RedoJointAngleChange())
		return;
	this->GetCurrentForcesFromDocument();
	//this->GetCurrentJointAnglesFromDocument(); 

    this->CWnd::UpdateData(FALSE /*Put to dialog*/);

	// Disable the Undo and Redo buttons if necessary	
	UndoButtonStatus();
    RedoButtonStatus();
}

void Dg_HandLoads::GetCurrentForcesFromDocument()
{
	Skeleton &skel = *mDocPtr->GetSkeleton();

    mdLeftMag.Format("%.1f", skel.getExtForce(JT_LHAND).length());
	mdRightMag.Format("%.1f", skel.getExtForce(JT_RHAND).length());

}
void Dg_HandLoads::OnBnClickedUndo()
{
	// TODO: Add your control notification handler code here
	if(!mDocPtr->UndoJointAngleChange())
		return;

	this->GetCurrentForcesFromDocument();

    this->CWnd::UpdateData(FALSE /*Put to dialog*/);

	// Disable the Undo and Redo buttons if necessary	
	UndoButtonStatus();
    RedoButtonStatus();

}
void Dg_HandLoads::UndoButtonStatus()
{
	undocontrol.EnableWindow(mDocPtr->CanUndo());
}

void Dg_HandLoads::RedoButtonStatus()
{
	redocontrol.EnableWindow(mDocPtr->CanRedo());
}