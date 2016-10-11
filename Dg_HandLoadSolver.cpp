// Dg_HandLoadSolver.cpp : implementation file
//

#include "stdafx.h"
#include "Dg_HandLoadSolver.h"
#include "afxdialogex.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include "UndoApplied.h"
#include "Types.h"
#include "../c4eLibNew/Units.hpp"


// Dg_HandLoadSolver dialog

IMPLEMENT_DYNAMIC(Dg_HandLoadSolver, CDialog)

Dg_HandLoadSolver::Dg_HandLoadSolver(Dg_HandLoads* bod,CWnd* pParent /*=NULL*/)
	: CDialog(Dg_HandLoadSolver::IDD, pParent),body(bod),Increment_Solver_Group(0),Current_Control_ID(0),Solver_Condition_Group(0)
	, mdLeftMag(_T(""))
	, mdRightMag(_T(""))
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
	, leftforce(_T(""))
	, rightvalue(_T(""))
{
	body = bod;
	mDocPtr = bod -> mDocPtr;
	Skeleton &skel = *mDocPtr->GetSkeleton(); 
	Increment_Solver_Group = bod->Increment_Solver_Group;
	Solver_Condition_Group = bod -> Solver_Condition_Group;
	m_BackCompression = bod -> m_BackCompression;
	m_Balance = bod -> m_Balance;
	m_Strength = bod -> m_Strength;
	m_wrist = bod -> m_wrist;
	m_elbow = bod -> m_elbow;
	m_shoulder = bod -> m_shoulder;
	m_torso = bod -> m_torso;
	m_hip = bod -> m_hip;
	m_knee = bod -> m_knee;
	m_ankle = bod -> m_ankle;
	mLeftHorizontal = bod -> mLeftHorizontal;
	mLeftVertical = bod -> mLeftVertical;
	mRightHorizontal = bod -> mRightHorizontal;
	mRightVertical = bod -> mRightVertical;
	mdLeftMag = bod -> mdLeftMag;
	mdRightMag = bod -> mdRightMag;
	dichotomyflag = bod -> dichotomyflag;
	mLeftMagUnits = bod -> mLeftMagUnits;
	mRightMagUnits = bod -> mRightMagUnits;
	mdLeftHorizontal = bod -> mdLeftHorizontal;
	mdLeftVertical = bod -> mdLeftVertical;
	mdRightHorizontal = bod -> mdRightHorizontal;
	mdRightVertical = bod -> mdRightVertical;
	Current_Control_ID = bod -> Current_Control_ID;

	    // assign force magnitudes
    leftforce.Format("%.2f",skel.getExtForce(JT_LHAND).length());
    rightvalue.Format("%.2f",skel.getExtForce(JT_RHAND).length());
	//mLeftMagUnits = _T("");
    //mRightMagUnits = _T("");
}

Dg_HandLoadSolver::~Dg_HandLoadSolver()
{
}

void Dg_HandLoadSolver::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_INCREMENT_SOLVER_GROUP, Increment_Solver_Group);

	DDX_Control(pDX, IDC_BOTHHANDS, mBothHandsControl);
	DDX_Control(pDX, IDC_CHECK1, wristcontrol);
	DDX_Control(pDX, IDC_CHECK3, elbowcontrol);
	DDX_Control(pDX, IDC_CHECK4, shouldercontrol);
	DDX_Control(pDX, IDC_CHECK5, torsocontrol);
	DDX_Control(pDX, IDC_CHECK6, hipcontrol);
	DDX_Control(pDX, IDC_CHECK7, kneecontrol);
	DDX_Control(pDX, IDC_CHECK8, ankelcontrol);
	DDX_Control(pDX, IDC_RedrawButton, m_incmax);

	DDX_Check(pDX, IDC_BackCompression, m_BackCompression);
	DDX_Check(pDX, IDC_Balance, m_Balance);
	DDX_Check(pDX, IDC_Strength, m_Strength);
	DDX_Check(pDX, IDC_CHECK1, m_wrist);
	DDX_Check(pDX, IDC_CHECK3, m_elbow);
	DDX_Check(pDX, IDC_CHECK4, m_shoulder);
	DDX_Check(pDX, IDC_CHECK5, m_torso);
	DDX_Check(pDX, IDC_CHECK6, m_hip);
	DDX_Check(pDX, IDC_CHECK7, m_knee);
	DDX_Check(pDX, IDC_CHECK8, m_ankle);
	//DDX_Text(pDX, IDC_LEFT_MAG, mdLeftMag);
	//DDX_Text(pDX, IDC_RIGHT_MAG, mdRightMag);
	DDX_Text(pDX, IDC_LEFT_MAG, leftforce);
	DDX_Text(pDX, IDC_RIGHT_MAG, rightvalue);
	DDX_Text(pDX, IDC_LEFT_MAG_UNITS, mLeftMagUnits);
	DDX_Text(pDX, IDC_RIGHT_MAG_UNITS, mRightMagUnits);
}


BEGIN_MESSAGE_MAP(Dg_HandLoadSolver, CDialog)
	ON_BN_CLICKED(IDOK, &Dg_HandLoadSolver::OnBnClickedOk)
	ON_COMMAND(ID_PRINT_PRINTWINDOW, &Dg_HandLoadSolver::OnPrintPrintwindow)
	ON_BN_CLICKED(IDC_Strength, &Dg_HandLoadSolver::OnBnClickedStrength)
	ON_BN_CLICKED(IDC_RedrawButton, &Dg_HandLoadSolver::OnClickedIncmax)
	ON_BN_CLICKED(IDC_SelectAll, &Dg_HandLoadSolver::OnBnClickedSelectAll)
	ON_BN_CLICKED(IDC_NoneSelected, &Dg_HandLoadSolver::OnBnClickedNone)
	//ON_BN_CLICKED(IDC_RedrawButton, OnRedrawButton)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

void Dg_HandLoadSolver::OnBnClickedStrength()
{
	// TODO: Add your control notification handler code here

	 /*static CRect rectlarge;       
	 static CRect rectsmall;

	 if (rectlarge.IsRectEmpty())  
	 {                               
	  GetWindowRect(&rectlarge);
	  rectsmall.left=rectlarge.left;
	  rectsmall.top=rectlarge.top;
	  rectsmall.right=rectlarge.right - 200; // Hide the strength part
	  rectsmall.bottom=rectlarge.bottom;
	 }

	  if(m_Strength == TRUE)
	 {
		 SetWindowPos(NULL,0,0,rectsmall.Width(),rectsmall.Height(),SWP_NOMOVE|SWP_NOZORDER);
	 }
	 else            
	 {
		 SetWindowPos(NULL,0,0,rectlarge.Width(),rectlarge.Height(),SWP_NOMOVE|SWP_NOZORDER);
	 }*/

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
}

void Dg_HandLoadSolver::OnClickedIncmax()
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
	Dg_HandLoads *p = (Dg_HandLoads*)this->GetParent();
	p->UpdateData(FALSE);
    Skeleton &skel = *mDocPtr->GetSkeleton();
	leftforce.Format("%.2f",skel.getExtForce(JT_LHAND).length());
    rightvalue.Format("%.2f",skel.getExtForce(JT_RHAND).length());
	UpdateData(FALSE);
	finalupdate();
}

void Dg_HandLoadSolver::UpdateDocument()
{
	Dg_HandLoads *p = (Dg_HandLoads*)this->GetParent();
	p->UpdateData(FALSE);
    Skeleton &skel = *mDocPtr->GetSkeleton();
	/*
	body->Increment_Solver_Group = Increment_Solver_Group;
	body -> Solver_Condition_Group = Solver_Condition_Group;
	body -> m_BackCompression = m_BackCompression;
	body -> m_Balance = m_Balance;
	body -> m_Strength = m_Strength;
	body -> m_wrist = m_wrist;
	body -> m_elbow = m_elbow;
	body -> m_shoulder = m_shoulder;
	body -> m_torso = m_torso;
	body -> m_hip = m_hip;
	body -> m_knee = m_knee;
	body -> m_ankle = m_ankle;
	body -> mLeftHorizontal = mLeftHorizontal;
	body -> mLeftVertical = mLeftVertical;
	body -> mRightHorizontal = mRightHorizontal;
	body -> mRightVertical = mRightVertical;
	body -> mdLeftMag = mdLeftMag;
	body -> mdRightMag = mdRightMag;
	body -> dichotomyflag = dichotomyflag;
	body -> mLeftMagUnits = mLeftMagUnits;
	body -> mdLeftHorizontal = mdLeftHorizontal;
	body -> mdLeftVertical = mdLeftVertical;
	body -> mdRightHorizontal = mdRightHorizontal;
	body -> mdRightVertical = mdRightVertical;
	body -> Current_Control_ID = Current_Control_ID;
	*/
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
	//GroupEvent* groupEvent = new GroupEvent(left, right);

	Skeleton* skelPtr;
	for(int i = left; i <= right; i++) {
		skelPtr = mDocPtr->getSkeletonAtFrame(i);
		oldLF = skelPtr->getExtForce(JT_LHAND);
		oldRF = skelPtr->getExtForce(JT_RHAND);
		oldLT = skelPtr->getExtTorque(JT_LHAND);
		oldRT = skelPtr->getExtTorque(JT_RHAND);

		// add individual frame event
		//groupEvent->addEvent(new UndoableHandLoads(oldLF, oldRF, oldLT, oldRT,
//													leftForceVec, rightForceVec, oldLT, oldRT, i));
		// set new forces
		skelPtr->setExtForce(JT_LHAND, leftForceVec);
		skelPtr->setExtForce(JT_RHAND, rightForceVec);
	}
	 
	// add group event to history
	//mDocPtr->addUndoEvent(groupEvent);
	mDocPtr->MakeDirtyAndUpdateViews(true);
	//UndoButtonStatus();
    //RedoButtonStatus();
	//body -> mDocPtr = mDocPtr;

/*	skel.setExtForce(JT_LHAND, leftForceVec);
	skel.setExtForce(JT_RHAND, rightForceVec);

    mDocPtr->addUndoEvent(new UndoableHandLoads(oldLF,oldRF,oldLT,oldRT));
    mDocPtr->MakeDirtyAndUpdateViews(true);*/
	//body->UpdateData(FALSE);
}

void Dg_HandLoadSolver::IncreaseLeft(void)
{
	char zero[5];
	Dg_HandLoads *pParentDlg = (Dg_HandLoads*)this->GetParent(); 
	int num =0; // start point
	int cnt = 0;
	double Value =0;
	int rightmax;
	int n =0;
	itoa(num,zero,10);
	bool flag = false; // check if we can go into balanced status
	dichotomyflag = false;
	bool solutionfound =  true;//check if we can find a solution under current settings
	pParentDlg->SetDlgItemText( IDC_LEFT_MAG, zero); 
	mdLeftMag.Format("%s",zero);
	Skeleton &lSkeleton = *mDocPtr->GetSkeleton();
	const BalanceData_s& balance = lSkeleton.getBalanceData();
	HandForceKillFocus(IDC_LEFT_MAG);
    VerifyTotalsInRange(false, true);
	OnRedrawButton();	// Update the data

	while(islimit())                                //if all the percentiles are larger than 25%,it will stay in loop
	{   
		char oldValText[50], newValText[50];
		char* stopstring;
        pParentDlg->GetDlgItemText(IDC_LEFT_MAG, oldValText, 49);
		Value = strtod(oldValText, &stopstring);
		Value = Value + pow(2,n);
		n++;
		gcvt( Value, 5, newValText);
        pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValText );
		mdLeftMag.Format("%s",newValText);
		HandForceKillFocus(IDC_LEFT_MAG);
	    VerifyTotalsInRange(false, true);
	    OnRedrawButton();	// Update the data
		if (this->m_Balance == TRUE && balance.BalanceStatus != BS_Acceptable && n > 12)
		{
			flag = true;   // it means can not get to balance status
			Value = 0;
			gcvt( Value, 5, newValText);
		    pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValText );
			mdLeftMag.Format("%s",newValText);
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
		pParentDlg->SetDlgItemText( IDC_LEFT_MAG,  zero);            // let mag of both hands be zero ,then begins the loop
		mdLeftMag.Format("%s",zero);
		HandForceKillFocus(IDC_LEFT_MAG);
		VerifyTotalsInRange(false, true);
		OnRedrawButton();	// Update the data
		while(islimit())                                
		{   

			char oldValTextLeft[50], newValTextLeft[50];
			char* stopstring1;
			pParentDlg->GetDlgItemText( IDC_LEFT_MAG, oldValTextLeft, 49);
			Value = strtod(oldValTextLeft, &stopstring1);
			Value = Value + pow(2,n);
			n++;
			gcvt( Value, 5, newValTextLeft);
			pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
			mdLeftMag.Format("%s",newValTextLeft);
			HandForceKillFocus( IDC_LEFT_MAG);
			VerifyTotalsInRange(false, true);
			OnRedrawButton();	// Update the data
			if (this->m_Balance == TRUE && balance.BalanceStatus != BS_Acceptable && n > 12)
			{
				flag = true;   // it means can not get to balance status
				Value = 0;
				gcvt( Value, 5, newValTextLeft);
				pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
				mdLeftMag.Format("%s",newValTextLeft);
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
				pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
				mdLeftMag.Format("%s",newValTextLeft);
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
				pParentDlg->GetDlgItemText(IDC_LEFT_MAG, oldValText, 49);
				Value = strtod(oldValText, &stopstring);
				Value = c;
				gcvt( Value, 5, newValText);
				pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValText );
				mdLeftMag.Format("%s",newValText);
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
			pParentDlg->GetDlgItemText(IDC_LEFT_MAG, oldValText, 49);
			Value = strtod(oldValText, &stopstring);
			Value = Value - 1;
			gcvt( Value, 5, newValText);
			pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValText );
			mdLeftMag.Format("%s",newValText);
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
				pParentDlg->GetDlgItemText( IDC_LEFT_MAG, oldValTextLeft, 49);
				Value = strtod(oldValTextLeft, &stopstring1);
				Value = Value -1;
				gcvt( Value, 5, newValTextLeft);
				pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
				mdLeftMag.Format("%s",newValTextLeft);
				HandForceKillFocus( IDC_LEFT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data
			}
		}
	}
}
 

void Dg_HandLoadSolver::IncreaseRight(void)
{
	char zero[5];
	Dg_HandLoads *pParentDlg = (Dg_HandLoads*)this->GetParent(); 
	int num = 0; //  start point
	double Value = 0;
	int leftmax;
	int n = 0; 
	int cnt = 0;
	bool flag = false; // check if we can gointo balanced status
	bool solutionfound = true; //check if we can find a solution under current settings
	dichotomyflag = false;
	gcvt(num, 5, zero);
	pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, zero); 
	mdRightMag.Format("%s",zero);
	HandForceKillFocus(IDC_RIGHT_MAG);
    VerifyTotalsInRange(false, true);
	OnRedrawButton();	// Update the data
	Skeleton &lSkeleton = *mDocPtr->GetSkeleton();
	BalanceData_s balance = lSkeleton.getBalanceData();
	while(islimit())                                
	{   
		char oldValText[50], newValText[50];
		char* stopstring;
        pParentDlg->GetDlgItemText(IDC_RIGHT_MAG, oldValText, 49);
		Value = strtod(oldValText, &stopstring);
		Value = Value + pow(2,n);
		n++;
		gcvt( Value, 5, newValText);
        pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValText );
		mdRightMag.Format("%s",newValText);
		HandForceKillFocus(IDC_RIGHT_MAG);
	    VerifyTotalsInRange(false, true);
	    OnRedrawButton();	// Update the data
		if (this->m_Balance == TRUE && balance.BalanceStatus != BS_Acceptable && n > 12)
		{
			flag = true;   // it means can not get to balance status
			Value = 0;
			gcvt( Value, 5, newValText);
		    pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValText );
			mdRightMag.Format("%s",newValText);
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
		pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, zero);            // let mag of both hands be zero ,then begins the loop
		mdRightMag.Format("%s",zero);
		HandForceKillFocus(IDC_RIGHT_MAG);
		VerifyTotalsInRange(false, true);
		OnRedrawButton();	// Update the data
		while(islimit())                                
		{   
			char oldValTextRight[50], newValTextRight[50];
			char* stopstring;
			pParentDlg->GetDlgItemText( IDC_RIGHT_MAG, oldValTextRight, 49);
			Value = strtod(oldValTextRight, &stopstring);
			Value = Value + pow(2,n);
			gcvt( Value, 5, newValTextRight);
			pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValTextRight);
			mdRightMag.Format("%s",newValTextRight);
			HandForceKillFocus(IDC_RIGHT_MAG);
			VerifyTotalsInRange(false, true);
			OnRedrawButton();	// Update the data
			n++;
			if (this->m_Balance == TRUE && balance.BalanceStatus != BS_Acceptable && n > 12)
			{
				flag = true;   // it means can not get to balance status
				Value = 0;
				gcvt( Value, 5, newValTextRight);
				pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValTextRight );
				mdRightMag.Format("%s",newValTextRight);
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
				pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValTextRight );
				mdRightMag.Format("%s",newValTextRight);
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
			pParentDlg->GetDlgItemText(IDC_RIGHT_MAG, oldValText, 49);
			Value = strtod(oldValText, &stopstring);
			Value = c;
			gcvt( Value, 5, newValText);
			pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValText );
			mdRightMag.Format("%s",newValText);
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
			pParentDlg->GetDlgItemText(IDC_RIGHT_MAG, oldValText, 49);
			Value = strtod(oldValText, &stopstring);
			Value = Value - 1;
			gcvt( Value, 5, newValText);
			pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValText );
			mdRightMag.Format("%s",newValText);
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
				pParentDlg->GetDlgItemText( IDC_RIGHT_MAG, oldValTextRight, 49);
				Value = strtod(oldValTextRight, &stopstring);
				Value = Value - 1;
				gcvt( Value, 5, newValTextRight);
				pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValTextRight);
				mdRightMag.Format("%s",newValTextRight);
				HandForceKillFocus(IDC_RIGHT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data
			}
		}
	}
}


void Dg_HandLoadSolver::IncreaseBoth(void)
{
	char zero[5];
	Dg_HandLoads *pParentDlg = (Dg_HandLoads*)this->GetParent(); 
	int num =0; //  start point
	double Value = 0 ,Value1=0;
	double ratio = 1;
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
    pParentDlg->GetDlgItemText( IDC_LEFT_MAG, oldleft, 49);
	pParentDlg->GetDlgItemText( IDC_RIGHT_MAG, oldright, 49);
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
	pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, zero);
	mdRightMag.Format("%s",zero);
	pParentDlg->SetDlgItemText( IDC_LEFT_MAG,  zero);            // let mag of both hands be zero ,then begins the loop
	mdLeftMag.Format("%s",zero);
	HandForceKillFocus(IDC_RIGHT_MAG);
    VerifyTotalsInRange(false, true);
	OnRedrawButton();	// Update the data
	while(islimit())                                
	{   
		char oldValTextRight[50], newValTextRight[50];
		char* stopstring;
        pParentDlg->GetDlgItemText( IDC_RIGHT_MAG, oldValTextRight, 49);
		Value = strtod(oldValTextRight, &stopstring);
		Value = Value + pow(2,n);
		gcvt( Value, 5, newValTextRight);
        pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValTextRight);
		mdRightMag.Format("%s",newValTextRight);
		HandForceKillFocus(IDC_RIGHT_MAG);
	    VerifyTotalsInRange(false, true);
	    OnRedrawButton();	// Update the data

		char oldValTextLeft[50], newValTextLeft[50];
        char* stopstring1;
        pParentDlg->GetDlgItemText( IDC_LEFT_MAG, oldValTextLeft, 49);
		Value1 = strtod(oldValTextLeft, &stopstring1);
		Value1 = Value1 + ratio * pow(2,n);
		n++;
		gcvt( Value1, 5, newValTextLeft);
        pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
		mdLeftMag.Format("%s",newValTextLeft);
		HandForceKillFocus( IDC_LEFT_MAG);
        VerifyTotalsInRange(false, true);
	    OnRedrawButton();	// Update the data
		if (this->m_Balance == TRUE && balance.BalanceStatus != BS_Acceptable && n > 12)
		{
			flag = true;   // it means can not get to balance status
			Value = 0;
			gcvt( Value, 5, newValTextRight);
		    pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValTextRight );
			mdRightMag.Format("%s",newValTextRight);
			HandForceKillFocus(IDC_RIGHT_MAG);
		    VerifyTotalsInRange(false, true);
		    OnRedrawButton();	// Update the data
			Value1 = 0;
			gcvt( Value1, 5, newValTextLeft);
		    pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
			mdLeftMag.Format("%s",newValTextLeft);
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
		pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, zero);
		mdRightMag.Format("%s",zero);
		pParentDlg->SetDlgItemText( IDC_LEFT_MAG,  zero);            // let mag of both hands be zero ,then begins the loop
		mdLeftMag.Format("%s",zero);
		HandForceKillFocus(IDC_RIGHT_MAG);
		VerifyTotalsInRange(false, true);
		OnRedrawButton();	// Update the data
		while(islimit())                                
		{   
			char oldValTextRight[50], newValTextRight[50];
			char* stopstring;
			pParentDlg->GetDlgItemText( IDC_RIGHT_MAG, oldValTextRight, 49);
			Value = strtod(oldValTextRight, &stopstring);
			Value = Value + pow(2,n);
			gcvt( Value, 5, newValTextRight);
			pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValTextRight);
			mdRightMag.Format("%s",newValTextRight);
			HandForceKillFocus(IDC_RIGHT_MAG);
			VerifyTotalsInRange(false, true);
			OnRedrawButton();	// Update the data

			char oldValTextLeft[50], newValTextLeft[50];
			char* stopstring1;
			pParentDlg->GetDlgItemText( IDC_LEFT_MAG, oldValTextLeft, 49);
			Value1 = strtod(oldValTextLeft, &stopstring1);
			Value1 = Value1 + ratio * pow(2,n);
			n++;
			gcvt( Value1, 5, newValTextLeft);
			pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
			mdLeftMag.Format("%s",newValTextLeft);
			HandForceKillFocus( IDC_LEFT_MAG);
			VerifyTotalsInRange(false, true);
			OnRedrawButton();	// Update the data
			if (this->m_Balance == TRUE && balance.BalanceStatus != BS_Acceptable && n > 12)
			{
				flag = true;   // it means can not get to balance status
				Value = 0;
				gcvt( Value, 5, newValTextRight);
				pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValTextRight );
				mdRightMag.Format("%s",newValTextRight);
				HandForceKillFocus(IDC_RIGHT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data
				Value1 = 0;
				gcvt( Value1, 5, newValTextLeft);
				pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
				mdLeftMag.Format("%s",newValTextLeft);
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
				pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValTextRight );
				mdRightMag.Format("%s",newValTextRight);
				HandForceKillFocus(IDC_RIGHT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data
				Value1 = 0;
				gcvt( Value1, 5, newValTextLeft);
				pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
				mdLeftMag.Format("%s",newValTextLeft);
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
			int a1 = num + ratio * (pow(2,n-1)-1);
			int b1 = num + ratio * (pow(2,n)-1);  // solution(left hand) between a1 ~ b1
			dichotomyflag = true;
			while((b - a > 1)||(b1 - a1 > 1))
			{
				char oldValText[50], newValText[50];
				char* stopstring;
				pParentDlg->GetDlgItemText(IDC_RIGHT_MAG, oldValText, 49);
				Value = strtod(oldValText, &stopstring);
				int c = (a + b)/2;
				Value = c;
				gcvt( Value, 5, newValText);
				pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValText );
				mdRightMag.Format("%s",newValText);
				HandForceKillFocus(IDC_RIGHT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();
				char oldValText1[50], newValText1[50];
				char* stopstring1;
				pParentDlg->GetDlgItemText(IDC_LEFT_MAG, oldValText1, 49);
				Value1 = strtod(oldValText1, &stopstring1);
				int c1 = (a1 + b1)/2;
				Value1 = c1;
				gcvt( Value1, 5, newValText1);
				pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValText1 );
				mdLeftMag.Format("%s",newValText1);
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

			//garantee the same ratio for left and right hands
		
			char oldValTextRight[50], newValTextRight[50];
			char* stopstring;
			pParentDlg->GetDlgItemText( IDC_RIGHT_MAG, oldValTextRight, 49);
			Value = strtod(oldValTextRight, &stopstring);
			gcvt( Value, 5, newValTextRight);
			pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValTextRight);
			mdRightMag.Format("%s",newValTextRight);
			HandForceKillFocus(IDC_RIGHT_MAG);
			VerifyTotalsInRange(false, true);
			OnRedrawButton();	// Update the data

			char oldValTextLeft[50], newValTextLeft[50];
			char* stopstring1;
			Value1 = Value * ratio; // garantee the same ratio as pre-set
			gcvt( Value1, 5, newValTextLeft);
			pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
			mdLeftMag.Format("%s",newValTextLeft);
			HandForceKillFocus( IDC_LEFT_MAG);
			VerifyTotalsInRange(false, true);
			OnRedrawButton();	// Update the data

		const BalanceData_s& balance1 = lSkeleton.getBalanceData();
		if (this->m_Balance == TRUE && balance1.BalanceStatus == BS_Critical)
		{
			if(ratio >= 1)
			{
				char oldValTextRight[50], newValTextRight[50];
				char* stopstring;
				pParentDlg->GetDlgItemText( IDC_RIGHT_MAG, oldValTextRight, 49);
				Value = strtod(oldValTextRight, &stopstring);
				Value = Value - 1 / ratio;
				gcvt( Value, 5, newValTextRight);
				pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValTextRight);
				mdRightMag.Format("%s",newValTextRight);
				HandForceKillFocus(IDC_RIGHT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data

				char oldValTextLeft[50], newValTextLeft[50];
				char* stopstring1;
				pParentDlg->GetDlgItemText( IDC_LEFT_MAG, oldValTextLeft, 49);
				Value1 = strtod(oldValTextLeft, &stopstring1);
				Value1 = Value1 - 1;
				gcvt( Value1, 5, newValTextLeft);
				pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
				mdLeftMag.Format("%s",newValTextLeft);
				HandForceKillFocus( IDC_LEFT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data
			}
			else
			{
				char oldValTextRight[50], newValTextRight[50];
				char* stopstring;
				pParentDlg->GetDlgItemText( IDC_RIGHT_MAG, oldValTextRight, 49);
				Value = strtod(oldValTextRight, &stopstring);
				Value = Value - 1;
				gcvt( Value, 5, newValTextRight);
				pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValTextRight);
				mdRightMag.Format("%s",newValTextRight);
				HandForceKillFocus(IDC_RIGHT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data

				char oldValTextLeft[50], newValTextLeft[50];
				char* stopstring1;
				pParentDlg->GetDlgItemText( IDC_LEFT_MAG, oldValTextLeft, 49);
				Value1 = strtod(oldValTextLeft, &stopstring1);
				Value1 = Value1 - 1 * ratio;
				gcvt( Value1, 5, newValTextLeft);
				pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
				mdLeftMag.Format("%s",newValTextLeft);
				HandForceKillFocus( IDC_LEFT_MAG);
				VerifyTotalsInRange(false, true);
				OnRedrawButton();	// Update the data
			}
		}
		else 
		{
			if(!islimit())
			{
				if(ratio >= 1)
				{
					char oldValTextRight[50], newValTextRight[50];
					char* stopstring;
					pParentDlg->GetDlgItemText( IDC_RIGHT_MAG, oldValTextRight, 49);
					Value = strtod(oldValTextRight, &stopstring);
					Value = Value - 1 / ratio;
					gcvt( Value, 5, newValTextRight);
					pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValTextRight);
					mdRightMag.Format("%s",newValTextRight);
					HandForceKillFocus(IDC_RIGHT_MAG);
					VerifyTotalsInRange(false, true);
					OnRedrawButton();	// Update the data

					char oldValTextLeft[50], newValTextLeft[50];
					char* stopstring1;
					pParentDlg->GetDlgItemText( IDC_LEFT_MAG, oldValTextLeft, 49);
					Value1 = strtod(oldValTextLeft, &stopstring1);
					Value1 = Value1 - 1;
					gcvt( Value1, 5, newValTextLeft);
					pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
					mdLeftMag.Format("%s",newValTextLeft); 
					HandForceKillFocus( IDC_LEFT_MAG);
					VerifyTotalsInRange(false, true);
					OnRedrawButton();	// Update the data
				}
				else
				{
					char oldValTextRight[50], newValTextRight[50];
					char* stopstring;
					pParentDlg->GetDlgItemText( IDC_RIGHT_MAG, oldValTextRight, 49);
					Value = strtod(oldValTextRight, &stopstring);
					Value = Value - 1;
					gcvt( Value, 5, newValTextRight);
					pParentDlg->SetDlgItemText( IDC_RIGHT_MAG, newValTextRight);
					mdRightMag.Format("%s",newValTextRight);
					HandForceKillFocus(IDC_RIGHT_MAG);
					VerifyTotalsInRange(false, true);
					OnRedrawButton();	// Update the data

					char oldValTextLeft[50], newValTextLeft[50];
					char* stopstring1;
					pParentDlg->GetDlgItemText( IDC_LEFT_MAG, oldValTextLeft, 49);
					Value1 = strtod(oldValTextLeft, &stopstring1);
					Value1 = Value1 - 1 * ratio;
					gcvt( Value1, 5, newValTextLeft);
					pParentDlg->SetDlgItemText( IDC_LEFT_MAG, newValTextLeft );
					mdLeftMag.Format("%s",newValTextLeft); 
					HandForceKillFocus( IDC_LEFT_MAG);
					VerifyTotalsInRange(false, true);
					OnRedrawButton();	// Update the data
				}
			}
		}
	}
}

void Dg_HandLoadSolver::HandForceKillFocus(int box_ID)
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

//---------------------------------------------------------------------------
// If giveMessages is true, the function will spit out error boxes to the user.
// If constrainVars is true, the function will set the variables that
//		are out of range to the nearest in-range values.
bool Dg_HandLoadSolver::VerifyTotalsInRange(bool giveMessages, bool constrainVars)
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

void Dg_HandLoadSolver::OnRedrawButton() 
{
    //UpdateData();

	/*body->Increment_Solver_Group = Increment_Solver_Group;
	body -> Solver_Condition_Group = Solver_Condition_Group;
	body -> m_BackCompression = m_BackCompression;
	body -> m_Balance = m_Balance;
	body -> m_Strength = m_Strength;
	body -> m_wrist = m_wrist;
	body -> m_elbow = m_elbow;
	body -> m_shoulder = m_shoulder;
	body -> m_torso = m_torso;
	body -> m_hip = m_hip;
	body -> m_knee = m_knee;
	body -> m_ankle = m_ankle;
	body -> mLeftHorizontal = mLeftHorizontal;
	body -> mLeftVertical = mLeftVertical;
	body -> mRightHorizontal = mRightHorizontal;
	body -> mRightVertical = mRightVertical;
	body -> mdLeftMag = mdLeftMag;
	body -> mdRightMag = mdRightMag;
	body -> dichotomyflag = dichotomyflag;
	body -> mLeftMagUnits = mLeftMagUnits;
	body -> mdLeftHorizontal = mdLeftHorizontal;
	body -> mdLeftVertical = mdLeftVertical;
	body -> mdRightHorizontal = mdRightHorizontal;
	body -> mdRightVertical = mdRightVertical;
	body -> Current_Control_ID = Current_Control_ID;
	body -> mDocPtr = mDocPtr;*/
	body -> mdLeftMag = mdLeftMag;
	body -> mdRightMag = mdRightMag;
	Dg_HandLoads *p = (Dg_HandLoads*)this->GetParent();
	p->UpdateData(FALSE);
    UpdateDocument();
}

bool Dg_HandLoadSolver::islimit(void)
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

int Dg_HandLoadSolver::CompressionLimit(void)
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

int Dg_HandLoadSolver::StrengthLimit(void)
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

bool Dg_HandLoadSolver::isbalance(void)
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

//---------------------------------------------------------------------------
void Dg_HandLoadSolver:: OnKillfocusLeftVertical() 
{
   /*UpdateData( TRUE );
   mLeftVertical = mdLeftVertical;

   mLeftExertion = (char*)ExertionText(mLeftVertical, mLeftHorizontal);
   UpdateData( FALSE );
   CheckAndUpdate();
   return;*/
}

//---------------------------------------------------------------------------
void Dg_HandLoadSolver::OnKillfocusLeftHorizontal() 
{
   /*UpdateData( TRUE );
   mLeftHorizontal = mdLeftHorizontal;

   mLeftExertion = (char*)ExertionText(mLeftVertical, mLeftHorizontal);
   UpdateData( FALSE );
   return;*/
}

//---------------------------------------------------------------------------
void Dg_HandLoadSolver::OnKillfocusRightVertical() 
{
   /*UpdateData( TRUE );
   mRightVertical = mdRightVertical;

   mRightExertion = (char*)ExertionText(mRightVertical, mRightHorizontal);
   UpdateData( FALSE );
   CheckAndUpdate();
   return;*/
}

//---------------------------------------------------------------------------
void Dg_HandLoadSolver::OnKillfocusRightHorizontal() 
{
   /*UpdateData(TRUE);
   mRightHorizontal = mdRightHorizontal;

   mRightExertion = (char*)ExertionText(mRightVertical, mRightHorizontal);
   UpdateData(FALSE);*/
}


//---------------------------------------------------------------------------
void Dg_HandLoadSolver::OnKillfocusRightMag() 
{
   UpdateData(TRUE);
   UpdateData(FALSE);
}

//---------------------------------------------------------------------------
void Dg_HandLoadSolver::OnKillfocusLeftMag() 
{
   UpdateData(TRUE);
   UpdateData(FALSE);
}

void Dg_HandLoadSolver::OnBnClickedSelectAll()
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

void Dg_HandLoadSolver::OnBnClickedNone()
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

BOOL Dg_HandLoadSolver::OnInitDialog()
{
	CDialog::OnInitDialog();
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	//UndoButtonStatus();
    //RedoButtonStatus();
	pSysMenu->AppendMenu(MF_SEPARATOR);// add a separator
	pSysMenu->AppendMenu(MF_STRING,IDR_MENU1,"Print Window");// add print menu
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void Dg_HandLoadSolver::OnSysCommand(UINT nID, LPARAM lParam)
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

void Dg_HandLoadSolver::OnPrintPrintwindow()
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
void Dg_HandLoadSolver::finalupdate()
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

}
void Dg_HandLoadSolver::OnBnClickedOk()
{
	OnClickedIncmax();
	// TODO: Add your control notification handler code here
	//UpdateData();
	finalupdate();
	body->Increment_Solver_Group = Increment_Solver_Group;
	body -> Solver_Condition_Group = Solver_Condition_Group;
	body -> m_BackCompression = m_BackCompression;
	body -> m_Balance = m_Balance;
	body -> m_Strength = m_Strength;
	body -> m_wrist = m_wrist;
	body -> m_elbow = m_elbow;
	body -> m_shoulder = m_shoulder;
	body -> m_torso = m_torso;
	body -> m_hip = m_hip;
	body -> m_knee = m_knee;
	body -> m_ankle = m_ankle;
	body -> mLeftHorizontal = mLeftHorizontal;
	body -> mLeftVertical = mLeftVertical;
	body -> mRightHorizontal = mRightHorizontal;
	body -> mRightVertical = mRightVertical;
	body -> mdLeftMag = mdLeftMag;
	body -> mdRightMag = mdRightMag;
	body -> dichotomyflag = dichotomyflag;
	body -> mLeftMagUnits = mLeftMagUnits;
	body -> mdLeftHorizontal = mdLeftHorizontal;
	body -> mdLeftVertical = mdLeftVertical;
	body -> mdRightHorizontal = mdRightHorizontal;
	body -> mdRightVertical = mdRightVertical;
	body -> Current_Control_ID = Current_Control_ID;
	body -> mDocPtr = mDocPtr;

	CDialog::OnOK();
}

void Dg_HandLoadSolver::GetCurrentForcesFromDocument()
{
	Skeleton &skel = *mDocPtr->GetSkeleton();

    leftforce.Format("%.1f", skel.getExtForce(JT_LHAND).length());
	rightvalue.Format("%.1f", skel.getExtForce(JT_RHAND).length());

}



