// Dg_OffsetCoordinates.cpp : implementation file
//
#include "stdafx.h"
#include "Dg_OffsetCoordinates.h"
#include "hom.h"
#include "Hom_doc.h"
#include "Vector3.h"
#include "Task.h"
#include "Skeleton.hpp"

#include <map> // map class-template definition

// Dg_OffsetCoordinates dialog

IMPLEMENT_DYNAMIC(Dg_OffsetCoordinates, CDialog)

Dg_OffsetCoordinates::Dg_OffsetCoordinates(CWnd* pParent)
	: CDialog(Dg_OffsetCoordinates::IDD, pParent)
	, offsetChoice(0)
	, offsetCoordinateX("")
	, offsetCoordinateY("")
	, offsetCoordinateZ("")	
	, DefaultDescription(_T(""))
	, mAddOffset(FALSE)
{
	// ZeroMemory(&offsetCoordinateZ, sizeof(DECIMAL));
	//DropDownOrder[0] = JT_ANKLE_CENTER;
	DropDownOrder[0] = JT_TRAGION;
	DropDownOrder[1] = JT_NASION;
	DropDownOrder[2] = JT_C1;
	DropDownOrder[3] = JT_C7T1;
	DropDownOrder[4] = JT_SCJ;
	DropDownOrder[5] = JT_LSHOULDER;
	DropDownOrder[6] = JT_LELBOW;
	DropDownOrder[7] = JT_LWRIST;
	DropDownOrder[8] = JT_LHAND;
	DropDownOrder[9] = JT_RSHOULDER;
	DropDownOrder[10] = JT_RELBOW;
	DropDownOrder[11] = JT_RWRIST;
	DropDownOrder[12] = JT_RHAND;
	DropDownOrder[13] = JT_L5S1_TEMP;
	DropDownOrder[14] = JT_HIP_CENTER;
	DropDownOrder[15] = JT_LIT;
	DropDownOrder[16] = JT_RIT;
	DropDownOrder[17] = JT_IT_CENTER;
	DropDownOrder[18] = JT_LHIP;
	DropDownOrder[19] = JT_LKNEE;
	DropDownOrder[20] = JT_LANKLE;
	DropDownOrder[21] = JT_LHEEL;
	DropDownOrder[22] = JT_LFOOT_CENTER;
	DropDownOrder[23] = JT_LBALL;
	DropDownOrder[24] = JT_RHIP;
	DropDownOrder[25] = JT_RKNEE;
	DropDownOrder[26] = JT_RANKLE;
	DropDownOrder[27] = JT_RHEEL;
	DropDownOrder[28] = JT_RFOOT_CENTER;
	DropDownOrder[29] = JT_RBALL;
	DropDownOrder[30] = JT_BALL_CENTER;
	DropDownOrder[31] = JT_C1;
	DropDownOrder[32] = JT_C3C4;
}


Dg_OffsetCoordinates::~Dg_OffsetCoordinates()
{
}

void Dg_OffsetCoordinates::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, mJointDropDown);
	DDX_Radio(pDX, IDC_RADIO1, offsetChoice);
	DDX_Text(pDX, IDC_EDIT1, offsetCoordinateX);
	DDX_Text(pDX, IDC_EDIT2, offsetCoordinateY);
	DDX_Text(pDX, IDC_EDIT3, offsetCoordinateZ);
	DDX_Text(pDX, IDC_DEFAULT_DESCRIPTION, DefaultDescription);
	DDX_Check(pDX, IDC_ADD_OFFSET, mAddOffset);
}


BEGIN_MESSAGE_MAP(Dg_OffsetCoordinates, CDialog)
	ON_BN_CLICKED(IDAPPLY, &Dg_OffsetCoordinates::OnBnClickedApply)
	ON_CBN_SELCHANGE(IDC_COMBO1, &Dg_OffsetCoordinates::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_RADIO1, &Dg_OffsetCoordinates::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &Dg_OffsetCoordinates::OnBnClickedRadio2)
	//ON_BN_CLICKED(IDC_RADIO3, &Dg_OffsetCoordinates::OnBnClickedRadio3)
	ON_EN_CHANGE(IDC_EDIT1, &Dg_OffsetCoordinates::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &Dg_OffsetCoordinates::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &Dg_OffsetCoordinates::OnEnChangeEdit3)
	ON_CBN_EDITCHANGE(IDC_COMBO1, &Dg_OffsetCoordinates::OnCbnEditchangeCombo1)
	ON_CBN_SETFOCUS(IDC_COMBO1, &Dg_OffsetCoordinates::OnCbnSetfocusCombo1)
	ON_EN_SETFOCUS(IDC_EDIT1, &Dg_OffsetCoordinates::OnEnSetfocusEdit1)
	ON_EN_SETFOCUS(IDC_EDIT2, &Dg_OffsetCoordinates::OnEnSetfocusEdit2)
	ON_EN_SETFOCUS(IDC_EDIT3, &Dg_OffsetCoordinates::OnEnSetfocusEdit3)
	// ON_BN_CLICKED(IDOK, &Dg_OffsetCoordinates::OnBnClickedOk)
	// ON_BN_CLICKED(IDCANCEL, &Dg_OffsetCoordinates::OnBnClickedCancel)
	// ON_BN_CLICKED(IDC_BUTTON1, &Dg_OffsetCoordinates::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_ADD_OFFSET, &Dg_OffsetCoordinates::OnBnClickedAddOffset)
END_MESSAGE_MAP()


// Dg_OffsetCoordinates message handlers

BOOL Dg_OffsetCoordinates::OnInitDialog()
{
	CDialog::OnInitDialog();
	//mJointDropDown.AddString("Default"); // "Default" instead of "Ankle Center"
	for(int i = 0; i < NUM_JOINTS-1; i++) {
		mJointDropDown.AddString(jointNames[DropDownOrder[i]]);
	}

	C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
	JointID offsetJoint = doc->getOffsetJoint();
	for(int i = 0; i < NUM_JOINTS; i++) {
		if(DropDownOrder[i] == offsetJoint) {
			mJointDropDown.SetCurSel(i);
			break;
		}
	}

	// No offset is center of ankles, so this dialog should be the one place
	// we substract off ankle center, because we have to pretend 
	// ankle center is 0,0,0 by default
	Vector3 offset;
	/*if(doc->getPosition() == P_Standing) {
		offset = doc->getOffset() - doc->GetSkeleton()->getJoint(JT_ANKLE_CENTER);
	} else {
		offset = doc->getOffset() - doc->GetSkeleton()->getSeatReference();
	}*/
	offset = doc->getOffsetCoordinate();
	putValuesInBoxes(offset[0], offset[1], offset[2]);

	offsetChoice = doc->getOffsetChoice();

	if(doc->getPosition() == P_Standing) {
		DefaultDescription = "(Center of ankles projected to lowest floor surface)";
	} else {
		DefaultDescription = "(Seat Reference Point)";
	}

	mAddOffset = doc->getAddOffset();
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void Dg_OffsetCoordinates::OnOK()
{
	UpdateData(true);
	if(offsetChoice == 1 && mJointDropDown.GetCurSel() == -1) {
		AfxMessageBox("Please choose a joint from the drop down menu");
		return;
	}
	OnBnClickedApply();
	CDialog::OnOK();
}

 void Dg_OffsetCoordinates::OnBnClickedApply() {
	this->UpdateData(true);

	if(offsetChoice == 1 && mJointDropDown.GetCurSel() == -1) {
		AfxMessageBox("Please choose a joint from the drop down menu");
		return;
	}
	
    C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
	doc->setOffsetChoice((OffsetChoice)offsetChoice);

	doc->setOffsetJoint(DropDownOrder[mJointDropDown.GetCurSel()]);
	// if custom offset isn't selected, the values in offsetCoordinate* shouldn't
	// be applied to the document
	//if(offsetChoice == COORD_OFFSET_NUMBER) {
	//	doc->setOffsetCoordinate(Vector3(atof(offsetCoordinateX), atof(offsetCoordinateY), atof(offsetCoordinateZ)));
	//}
	doc->setOffsetCoordinate(Vector3(atof(offsetCoordinateX), atof(offsetCoordinateY), atof(offsetCoordinateZ)));

	doc->setAddOffset(mAddOffset);

	doc->MakeDirtyAndUpdateViews();
}

void Dg_OffsetCoordinates::OnCbnSelchangeCombo1()
{
	// if joint offset is selected, update the text boxes to have
	// the values of the new joint
	this->CheckRadioButton(IDC_RADIO2, IDC_RADIO1, IDC_RADIO2);
	/*UpdateData(true);
	if(offsetChoice == 1) {
		Skeleton* skel = Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
		Vector3 offsetCoordinate;
		if(Get_App_Ptr()->Get_Document_Ptr()->getPosition() == P_Standing) {
			offsetCoordinate = skel->getJoint(DropDownOrder[mJointDropDown.GetCurSel()])
										- skel->getJoint(JT_ANKLE_CENTER);
		} else {
			offsetCoordinate = skel->getJoint(DropDownOrder[mJointDropDown.GetCurSel()])
										- skel->getSeatReference();
		}
		// put the values in the text boxes
		//putValuesInBoxes(offsetCoordinate[0], offsetCoordinate[1], offsetCoordinate[2]);
		UpdateData(false);
	}*/
}

void Dg_OffsetCoordinates::OnBnClickedRadio1() {
	//UpdateData(true);
	//putValuesInBoxes(0,0,0);
	//mJointDropDown.SetCurSel(0); // careful with this constant. it is the ankle center index
	//UpdateData(false);
}

void Dg_OffsetCoordinates::OnBnClickedRadio2()
{
	/*UpdateData(true);
	Skeleton* skel = Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();

	Vector3 offsetCoordinate;
	if(Get_App_Ptr()->Get_Document_Ptr()->getPosition() == P_Standing) {
		offsetCoordinate = skel->getJoint(DropDownOrder[mJointDropDown.GetCurSel()])
								- skel->getJoint(JT_ANKLE_CENTER);
	} else {
		offsetCoordinate = skel->getJoint(DropDownOrder[mJointDropDown.GetCurSel()])
								- skel->getSeatReference();
	}
	// put the values in the text boxes
	putValuesInBoxes(offsetCoordinate[0], offsetCoordinate[1], offsetCoordinate[2]);
	UpdateData(false);*/
}


/*void Dg_OffsetCoordinates::OnBnClickedRadio3()
{
} */

// Dg_OffsetCoordinates message handlers

void Dg_OffsetCoordinates::OnEnChangeEdit1()
{
	//this->CheckRadioButton(IDC_RADIO2, IDC_RADIO3, IDC_RADIO3);
	UpdateData(true);
	mAddOffset = true;
	UpdateData(false);
}

void Dg_OffsetCoordinates::OnEnChangeEdit2()
{
	//this->CheckRadioButton(IDC_RADIO2, IDC_RADIO3, IDC_RADIO3);
	UpdateData(true);
	mAddOffset = true;
	UpdateData(false);
}

void Dg_OffsetCoordinates::OnEnChangeEdit3()
{
	//this->CheckRadioButton(IDC_RADIO2, IDC_RADIO3, IDC_RADIO3);
	UpdateData(true);
	mAddOffset = true;
	UpdateData(false);
}

void Dg_OffsetCoordinates::putValuesInBoxes(double x, double y, double z) {
	offsetCoordinateX.Format("%.1f", x);
	offsetCoordinateY.Format("%.1f", y);
	offsetCoordinateZ.Format("%.1f", z);
}
void Dg_OffsetCoordinates::OnCbnEditchangeCombo1()
{
	this->CheckRadioButton(IDC_RADIO2, IDC_RADIO1, IDC_RADIO2);
}

void Dg_OffsetCoordinates::OnCbnSetfocusCombo1()
{
	this->CheckRadioButton(IDC_RADIO2, IDC_RADIO1, IDC_RADIO2);
}

void Dg_OffsetCoordinates::OnEnSetfocusEdit1()
{
    //this->CheckRadioButton(IDC_RADIO2, IDC_RADIO3, IDC_RADIO3);
	UpdateData(true);
	mAddOffset = true;
	UpdateData(false);
}

void Dg_OffsetCoordinates::OnEnSetfocusEdit2()
{
	//this->CheckRadioButton(IDC_RADIO2, IDC_RADIO3, IDC_RADIO3);
	UpdateData(true);
	mAddOffset = true;
	UpdateData(false);
}

void Dg_OffsetCoordinates::OnEnSetfocusEdit3()
{
	//this->CheckRadioButton(IDC_RADIO2, IDC_RADIO3, IDC_RADIO3);
	UpdateData(true);
	mAddOffset = true;
	UpdateData(false);
}


void Dg_OffsetCoordinates::OnBnClickedAddOffset()
{
	// TODO: Add your control notification handler code here
	/*UpdateData();
	if(mAddOffset)
	{
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT3)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	}*/
}
