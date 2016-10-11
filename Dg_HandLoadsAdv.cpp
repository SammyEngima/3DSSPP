#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include "UndoApplied.h"
#include "Dg_HandLoadsAdv.h"
#include "../c4eLibNew/units.hpp"
#include <string>
//#include <pasfuncs.h>
#include ".\dg_handloadsadv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Dg_HandLoads_Adv::Dg_HandLoads_Adv(C_Hom_Doc* aDocPtr,CWnd* pParent) : CDialog(Dg_HandLoads_Adv::IDD, pParent),
                                                                       mDocPtr(aDocPtr)
{
    //{{AFX_DATA_INIT(Dg_HandLoads_Adv)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    Skeleton &skel = *mDocPtr->GetSkeleton();

    Vector3 leftForce = skel.getExtForce(JT_LHAND);
    Vector3 rightForce = skel.getExtForce(JT_RHAND);
    for(int i = 0; i < 3; ++i)
    {
        
        mLeftForce[i] = leftForce[i]; 
        mRightForce[i] = rightForce[i];
    }

	// convert torques from N*m to N*cm (Torque inputs N*cm, outputs N*m... stupid)
    Vector3 leftTorque = skel.getExtTorque(JT_LHAND);
    Vector3 rightTorque = skel.getExtTorque(JT_RHAND);

    for(int i = 0; i < 3; ++i)
    {
        
	    mLeftTorque[i]  = leftTorque[i];
        mRightTorque[i] = rightTorque[i];
	}

	// Calculate total values (displayed at bottom) Must be done after the torque and force values are initialized
	RecalcLeftTotal();
	RecalcRightTotal();


	// SB 10/26/07
    RedisplayLeftForces();
	RedisplayRightForces();



	mForceUnits = mDocPtr->ForceUnits();

	mRightForceText = mLeftForceText = CString("Force (") + mDocPtr->ForceUnits() + ")";
	mRightTorqueUnits = mLeftTorqueUnits  = CString("Torque (") + mDocPtr->TorqueUnits() + ")";
}

void Dg_HandLoads_Adv::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

   //{{AFX_DATA_MAP(Dg_HandLoads_Adv)
   DDX_Text(pDX, IDC_LFRCUNITS, mLeftForceText);
   
 
   DDX_Text(pDX, IDC_LXFRC, mLeftForce[0]);
   DDX_Text(pDX, IDC_LYFRC, mLeftForce[1]);
   DDX_Text(pDX, IDC_LZFRC, mLeftForce[2]);
 

    
   /* SB Changed 10/26/07

   DDX_Text(pDX, IDC_LXFRC, mdLeftForce[0]);
   DDX_Text(pDX, IDC_LYFRC, mdLeftForce[1]);
   DDX_Text(pDX, IDC_LZFRC, mdLeftForce[2]);
   */


   DDX_Text(pDX, IDC_LFRCTOT, mdLeftMag);
   
   DDX_Text(pDX, IDC_RFRCUNITS, mRightForceText);

 
   DDX_Text(pDX, IDC_RXFRC, mRightForce[0]);
   DDX_Text(pDX, IDC_RYFRC, mRightForce[1]);
   DDX_Text(pDX, IDC_RZFRC, mRightForce[2]);


    /* SB Changed 10/26/07
   DDX_Text(pDX, IDC_RXFRC, mdRightForce[0]);
   DDX_Text(pDX, IDC_RYFRC, mdRightForce[1]);
   DDX_Text(pDX, IDC_RZFRC, mdRightForce[2]);
   */


   DDX_Text(pDX, IDC_RFRCTOT, mdRightMag);

   DDX_Text(pDX, IDC_LTRQUNITS, mLeftTorqueUnits);
   DDX_Text(pDX, IDC_LXTRQ, mLeftTorque[0]);
   DDX_Text(pDX, IDC_LYTRQ, mLeftTorque[1]);
   DDX_Text(pDX, IDC_LZTRQ, mLeftTorque[2]);
   DDX_Text(pDX, IDC_LTRQTOTAL, mdLeftTorqueMag);

   DDX_Text(pDX, IDC_RTRQUNITS, mRightTorqueUnits);
   DDX_Text(pDX, IDC_RXTRQ, mRightTorque[0]);
   DDX_Text(pDX, IDC_RYTRQ, mRightTorque[1]);
   DDX_Text(pDX, IDC_RZTRQ, mRightTorque[2]);
   DDX_Text(pDX, IDC_RTRQTOTAL, mdRightTorqueMag);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Dg_HandLoads_Adv, CDialog)
	//{{AFX_MSG_MAP(Dg_HandLoads_Adv)
	ON_EN_KILLFOCUS(IDC_LXFRC, OnKillfocusLxfrc)
	ON_EN_KILLFOCUS(IDC_LYFRC, OnKillfocusLyfrc)
	ON_EN_KILLFOCUS(IDC_LZFRC, OnKillfocusLzfrc)
	ON_EN_KILLFOCUS(IDC_RXFRC, OnKillfocusRxfrc)
	ON_EN_KILLFOCUS(IDC_RYFRC, OnKillfocusRyfrc)
	ON_EN_KILLFOCUS(IDC_RZFRC, OnKillfocusRzfrc)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_BN_CLICKED(ID_ZERO_ALL, OnZeroAll)
	ON_EN_KILLFOCUS(IDC_LXTRQ, OnEnKillfocusLxtrq)
	ON_EN_KILLFOCUS(IDC_LYTRQ, OnEnKillfocusLytrq)
	ON_EN_KILLFOCUS(IDC_LZTRQ, OnEnKillfocusLztrq)
	ON_EN_KILLFOCUS(IDC_RXTRQ, OnEnKillfocusRxtrq)
	ON_EN_KILLFOCUS(IDC_RYTRQ, OnEnKillfocusRytrq)
	ON_EN_KILLFOCUS(IDC_RZTRQ, OnEnKillfocusRztrq)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_HandLoads_Adv message handlers

void Dg_HandLoads_Adv::RecalcLeftTotal()
{
	
   mLeftMag = sqrt(pow(mLeftForce[0], 2) +
                   pow(mLeftForce[1], 2) +
                   pow(mLeftForce[2], 2));

   mdLeftMag = Round_Double(mLeftMag, 2);
   /* mdLeftMag[0].Format("%.2f",mLeftMag[0]); */


   mLeftTorqueMag = sqrt(pow(mLeftTorque[0], 2) +
						 pow(mLeftTorque[1], 2) +
						 pow(mLeftTorque[2], 2));

   mdLeftTorqueMag = Round_Double(mLeftTorqueMag, 2);
}

void Dg_HandLoads_Adv::RecalcRightTotal()
{
   mRightMag = sqrt(pow(mRightForce[0], 2) +
                    pow(mRightForce[1], 2) +
                    pow(mRightForce[2], 2));

   mdRightMag = Round_Double(mRightMag, 2);

   mRightTorqueMag = sqrt(pow(mRightTorque[0], 2) +
						  pow(mRightTorque[1], 2) +
						  pow(mRightTorque[2], 2));

   mdRightTorqueMag = Round_Double(mRightTorqueMag, 2);
}

void Dg_HandLoads_Adv::RedisplayLeftForces()
{
   for(int i = 0; i < 3; ++i)
   
     mLeftForce[i] = Round_Double(mLeftForce[i], 2);
  
}

void Dg_HandLoads_Adv::RedisplayRightForces()
{
   for(int i = 0; i < 3; ++i)
   
     mRightForce[i] = Round_Double(mRightForce[i], 2);
  
}


void Dg_HandLoads_Adv::OnKillfocusLxfrc() 
{
    this->UpdateData(true); // retrieve
    RecalcLeftTotal();

	// SB 10/26/07
    RedisplayLeftForces();
	


    this->UpdateData(false); // send
}

void Dg_HandLoads_Adv::OnKillfocusLyfrc() 
{
    this->UpdateData(true); // retrieve
    RecalcLeftTotal();

	// SB 10/26/07
    RedisplayLeftForces();
	


    this->UpdateData(false); // send
}

void Dg_HandLoads_Adv::OnKillfocusLzfrc() 
{
    this->UpdateData(true); // retrieve
    RecalcLeftTotal();

	// SB 10/26/07
    RedisplayLeftForces();



    this->UpdateData(false); // send
}

void Dg_HandLoads_Adv::OnKillfocusRxfrc() 
{
    this->UpdateData(true); // retrieve
    RecalcRightTotal();

	// SB 10/26/07
    RedisplayRightForces();


    this->UpdateData(false); // send
}

void Dg_HandLoads_Adv::OnKillfocusRyfrc() 
{
    this->UpdateData(true); // retrieve
    RecalcRightTotal();	

	// SB 10/26/07
    RedisplayRightForces();


    this->UpdateData(false); // send
}

void Dg_HandLoads_Adv::OnKillfocusRzfrc() 
{
    this->UpdateData(true); // retrieve
    RecalcRightTotal();	

	// SB 10/26/07
    RedisplayRightForces();



    this->UpdateData(false); // send
}
void Dg_HandLoads_Adv::OnEnKillfocusLxtrq()
{
	this->UpdateData( true );	// Retrieve
	this->RecalcLeftTotal();

	// SB 10/26/07
    RedisplayLeftForces();
	


	this->UpdateData( false );  // Send
}

void Dg_HandLoads_Adv::OnEnKillfocusLytrq()
{
	this->UpdateData(true);	// Retrieve
	this->RecalcLeftTotal();

	// SB 10/26/07
    RedisplayLeftForces();


	this->UpdateData(false);  // Send
}

void Dg_HandLoads_Adv::OnEnKillfocusLztrq()
{
	this->UpdateData(true);	// Retrieve
	this->RecalcLeftTotal();

	// SB 10/26/07
    RedisplayLeftForces();



	this->UpdateData(false);  // Send
}

void Dg_HandLoads_Adv::OnEnKillfocusRxtrq()
{
	this->UpdateData(true);	// Retrieve
	this->RecalcRightTotal();

	// SB 10/26/07

	RedisplayRightForces();


	this->UpdateData(false);  // Send}
}

void Dg_HandLoads_Adv::OnEnKillfocusRytrq()
{
	this->UpdateData(true);	// Retrieve
	this->RecalcRightTotal();

	// SB 10/26/07
    RedisplayRightForces();


	this->UpdateData(false);  // Send}
}

void Dg_HandLoads_Adv::OnEnKillfocusRztrq()
{
	this->UpdateData(true);	// Retrieve
	this->RecalcRightTotal();

	// SB 10/26/07
	RedisplayRightForces();


	this->UpdateData(false);  // Send
}

bool Dg_HandLoads_Adv::VerifyTotalsInRange()
{
    String msgStr = "";
    int lMaxHandForce;
   
	lMaxHandForce = MAX_HAND_FORCE_LB;
	if(mDocPtr->Is_Metric()) lMaxHandForce = int(lMaxHandForce * U_LBF_NEWTONS);

    if(mLeftMag > lMaxHandForce)
    {
        msgStr = "Left Hand";
        if(mRightMag > lMaxHandForce) msgStr += " and ";
    }

    if(mRightMag > lMaxHandForce) msgStr += "Right Hand";

    if(msgStr != String(""))
    {
	    CString messageCStr = msgStr;			// Copy the String to a CString
        CString finalStr;						// Contains the final output of message box
        finalStr.Format("%s:\nMaximum total force of %d %s exceeded.",
            messageCStr, lMaxHandForce, mForceUnits);

        msgStr = finalStr;
        AfxMessageBox(msgStr);
        return false;
    }
    else return true;
}

void Dg_HandLoads_Adv::UpdateDocument()
{
    this->BeginWaitCursor();
    Skeleton &skel = *mDocPtr->GetSkeleton();

    // retain old forces for undo history
    Vector3 oldLF = skel.getExtForce(JT_LHAND),oldRF = skel.getExtForce(JT_RHAND);
    Vector3 oldLT = skel.getExtTorque(JT_LHAND),oldRT = skel.getExtTorque(JT_RHAND);

	Vector3 newLF(mLeftForce), newRF(mRightForce), newLT(mLeftTorque), newRT(mRightTorque);
	// if there has been no change, we don't have to set the values and we shouldn't make a new undo event
	if(newRF[0] == oldRF[0] &&
		newRF[1] == oldRF[1] &&
		newRF[2] == oldRF[2] &&
		newLF[0] == oldLF[0] &&
		newLF[1] == oldLF[1] &&
		newLF[2] == oldLF[2] &&
		newRT[0] == oldRT[0] &&
		newRT[1] == oldRT[1] &&
		newRT[2] == oldRT[2] &&
		newLT[0] == oldLT[0] &&
		newLT[1] == oldLT[1] &&
		newLT[2] == oldLT[2]) {
		return;
	}

//    mDocPtr->addUndoEvent(new UndoableHandLoads(oldLF,oldRF,oldLT,oldRT));
	// TODO testing multi-frame dialog and undo
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
													newLF, newRF, newLT, newRT, i));
		// set new forces
		skelPtr->setExtForce(JT_LHAND, newLF);
		skelPtr->setExtForce(JT_RHAND, newRF);

		skelPtr->setExtTorque(JT_LHAND, newLT);
		skelPtr->setExtTorque(JT_RHAND, newRT);
	}

/*	mDocPtr->addUndoEvent(new UndoableHandLoads(oldLF, oldRF, oldLT, oldRT,
												newLF, newRF, newLT, newRT,
												mDocPtr->getCurrentFrame()));*/
	mDocPtr->addUndoEvent(groupEvent);
    mDocPtr->MakeDirtyAndUpdateViews();
    this->EndWaitCursor();
}

void Dg_HandLoads_Adv::OnApply()
{
   // SetFocus() makes sure that whatever box just got written to
   // gets its data updated.

   SetFocus();
   UpdateData(true);  // To make sure torques get applied

   if(!VerifyTotalsInRange())
      return;

   RecalcLeftTotal();
   RecalcRightTotal();

   // SB 10/26/07
   RedisplayLeftForces();
   RedisplayRightForces();

   UpdateDocument();
}

void Dg_HandLoads_Adv::OnOK() 
{
   OnApply();
   
   CDialog::OnOK();
}

void Dg_HandLoads_Adv::OnZeroAll() 
{
    for(int i = 0; i < 3; ++i)
    {
        mLeftForce[i] = 0;
        mRightForce[i] = 0;
        mLeftTorque[i] = 0;
        mRightTorque[i] = 0;
    }
    RecalcLeftTotal();
	RecalcRightTotal();

	// SB 10/26/07
    RedisplayLeftForces();
	RedisplayRightForces();

    UpdateData(false);
    OnApply();
}
