// Dg_HandPosture.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "Dg_HandPosture.h"
#include "hom.h"
#include "hom_doc.h"
#include "JointAngles.hpp"
#include "Skeleton.hpp"
#include "UndoPosture.h"
#include <string>
#include <sstream>

using std::string;
using std::ostringstream;
using std::endl;


// Dg_HandPosture dialog

// radio button values
#define OPEN 0
#define CUPPED 1
#define FIST 2
#define POINTING 3

#define PLUS (1)
#define MINUS (-1)

IMPLEMENT_DYNAMIC(Dg_HandPosture, CDialog)

Dg_HandPosture::Dg_HandPosture(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_HandPosture::IDD, pParent)
	, HandLH(0)
	, HandRH(0)
	, HandLV(0)
	, HandLRot(0)
	, HandRV(0)
	, HandRRot(0)
	, LHandType(FALSE)
	, RHandType(FALSE)
	, CurrentAngle(0)
	, Increment_Group(FALSE)
	//, mHandPostureLockCheckbox(FALSE)
{
	mDocPtr = Get_App_Ptr()->Get_Document_Ptr();
	Increment_Value[0] = 1;
	Increment_Value[1] = 5;
	Increment_Value[2] = 10;
	Increment_Value[3] = 15;
	Increment_Value[4] = 20;
	Increment_Value[5] = 25;
}

Dg_HandPosture::~Dg_HandPosture()
{
	mDocPtr->cancelCue();
}

void Dg_HandPosture::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HAND_LH, HandLH);
	DDX_Text(pDX, IDC_HAND_RH, HandRH);
	DDX_Text(pDX, IDC_HAND_LV, HandLV);
	DDX_Text(pDX, IDC_HAND_LROT, HandLRot);
	DDX_Text(pDX, IDC_HAND_RV, HandRV);
	DDX_Text(pDX, IDC_HAND_RROT, HandRRot);
	DDX_Radio(pDX, IDC_HANDTYPE_OPEN_L, LHandType);
	DDX_Radio(pDX, IDC_HANDTYPE_OPEN_R, RHandType);
	DDX_Radio(pDX, IDC_INCREMENT_GROUP, Increment_Group);
	//DDX_Check(pDX, IDC_HANDPOSTURELOCK, mHandPostureLockCheckbox);
	DDX_Control(pDX, IDC_HAND_LH, mLeftHorz);
	DDX_Control(pDX, IDC_HAND_LV, mLeftVert);
	DDX_Control(pDX, IDC_HAND_LROT, mLeftRot);
	DDX_Control(pDX, IDC_HAND_RH, mRightHorz);
	DDX_Control(pDX, IDC_HAND_RV, mRightVert);
	DDX_Control(pDX, IDC_HAND_RROT, mRightRot);
	DDX_Control(pDX, IDC_UndoButton, mUndoButtonControl);
	DDX_Control(pDX, IDC_RedoButton, mRedoButtonControl);
}

BEGIN_MESSAGE_MAP(Dg_HandPosture, CDialog)
	ON_BN_CLICKED(IDC_HAND_POSTURE_APPLY, &Dg_HandPosture::OnBnClickedHandPostureApply)
	ON_BN_CLICKED(IDOK, &Dg_HandPosture::OnBnClickedOk)
	ON_EN_SETFOCUS(IDC_HAND_LH, &Dg_HandPosture::OnEnSetfocusHandLh)
	ON_EN_SETFOCUS(IDC_HAND_LV, &Dg_HandPosture::OnEnSetfocusHandLv)
	ON_EN_SETFOCUS(IDC_HAND_LROT, &Dg_HandPosture::OnEnSetfocusHandLrot)
	ON_EN_SETFOCUS(IDC_HAND_RH, &Dg_HandPosture::OnEnSetfocusHandRh)
	ON_EN_SETFOCUS(IDC_HAND_RV, &Dg_HandPosture::OnEnSetfocusHandRv)
	ON_BN_CLICKED(IDC_INCREMENT_PLUS_HAND, &Dg_HandPosture::OnBnClickedIncrementPlusHand)
	ON_BN_CLICKED(IDC_INCREMENT_MINUS_HAND, &Dg_HandPosture::OnBnClickedIncrementMinusHand)
	ON_EN_SETFOCUS(IDC_HAND_RROT, &Dg_HandPosture::OnEnSetfocusHandRrot)
	ON_BN_CLICKED(IDC_SYMMETRY_R_TO_L_HAND, &Dg_HandPosture::OnBnClickedSymmetryRToLHand)
	ON_BN_CLICKED(IDC_SYMMETRY_L_TO_R_HAND, &Dg_HandPosture::OnBnClickedSymmetryLToRHand)
	ON_BN_CLICKED(IDC_HANDNEUTRAL, &Dg_HandPosture::OnBnClickedHandneutral)
	//ON_BN_CLICKED(IDC_HANDPOSTURELOCK, &Dg_HandPosture::OnBnClickedHandposturelock)
	ON_BN_CLICKED(IDC_UndoButton, OnUndoButton)
	ON_BN_CLICKED(IDC_RedoButton, OnBnClickedRedobutton)
	ON_COMMAND(ID_PRINT_PRINTWINDOW, &Dg_HandPosture::OnPrintPrintwindow)
	ON_WM_SYSCOMMAND()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


BOOL Dg_HandPosture::OnInitDialog() {
	CDialog::OnInitDialog();
	// Get data from document
	JointAngles ja = Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton()->GetAngles();
	HandLH = ja[A_HandLH];
	HandLV = ja[A_HandLV];
	HandLRot = ja[A_HandLRot];
	HandRH = ja[A_HandRH];
	HandRV = ja[A_HandRV];
	HandRRot = ja[A_HandRRot];
	Increment_Group = 2; // 10 degree increment by default
	/*
	this->mHandPostureLockCheckbox = Get_App_Ptr()->Get_Document_Ptr()->getFixedHandAngles();
	// diable edit boxes
	mLeftHorz.EnableWindow(!this->mHandPostureLockCheckbox);
	mLeftVert.EnableWindow(!this->mHandPostureLockCheckbox);
	mLeftRot.EnableWindow(!this->mHandPostureLockCheckbox);
	mRightHorz.EnableWindow(!this->mHandPostureLockCheckbox);
	mRightVert.EnableWindow(!this->mHandPostureLockCheckbox);
	mRightRot.EnableWindow(!this->mHandPostureLockCheckbox);
	*/
	UpdateData(false);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->AppendMenu(MF_SEPARATOR);// add a separator
	pSysMenu->AppendMenu(MF_STRING,IDR_MENU1,"Print Window");// add print menu
	RedoButtonStatus();
	//return false;
	UndoButtonStatus();
	//return TRUE;
	return false;
}


void Dg_HandPosture::OnBnClickedHandPostureApply()
{
	// Save old angle in case this doesn't work
	int OldAngle = *CurrentAngle;
	
	if(UpdateData(TRUE)) //Data transfer successful 
		UpdateDocument();

	///Yijun
	//UpdateDocument();
	C_Hom_Doc* doc_ptr = Get_App_Ptr()->Get_Document_Ptr();
	Skeleton& skel = *doc_ptr->GetSkeleton();
	
	/*if(Round_Double(skel.getExtraAngle(EA_HandLDeviation)) < -21){
		skel.error = "Left wrist radial deviation exceeded!";
		//AfxMessageBox((char*)("Wrist Angles exceeded!"));
		AfxMessageBox(skel.getLastError().c_str());
		// if there were errors, set the changed angle back
		*CurrentAngle = OldAngle;
		UpdateData(false);
		UpdateDocument();
	}
	if(Round_Double(skel.getExtraAngle(EA_HandRDeviation)) < -21){
		skel.error = "Right wrist radial deviation exceeded!";
		//AfxMessageBox((char*)("Wrist Angles exceeded!"));
		AfxMessageBox(skel.getLastError().c_str());
		// if there were errors, set the changed angle back
		*CurrentAngle = OldAngle;
		UpdateData(false);
		UpdateDocument();
	}
	if(Round_Double(skel.getExtraAngle(EA_HandLDeviation)) > 38){
		skel.error = "Left wrist ulnar deviation exceeded!";
		//AfxMessageBox((char*)("Wrist Angles exceeded!"));
		AfxMessageBox(skel.getLastError().c_str());
		// if there were errors, set the changed angle back
		*CurrentAngle = OldAngle;
		UpdateData(false);
		UpdateDocument();
	}
	if(Round_Double(skel.getExtraAngle(EA_HandRDeviation)) > 38){
		skel.error = "Right wrist ulnar deviation exceeded!";
		//AfxMessageBox((char*)("Wrist Angles exceeded!"));
		AfxMessageBox(skel.getLastError().c_str());
		// if there were errors, set the changed angle back
		*CurrentAngle = OldAngle;
		UpdateData(false);
		UpdateDocument();
	}*/
	RedoButtonStatus();
	UndoButtonStatus();
}

bool Dg_HandPosture::UpdateDocument() {
	// Try to apply the changes
	C_Hom_Doc* doc_ptr = Get_App_Ptr()->Get_Document_Ptr();
	Skeleton& skel = *doc_ptr->GetSkeleton();
	JointAngles old_angles = skel.GetAngles();
	JointAngles new_angles = old_angles;
	new_angles[A_HandLH] = HandLH;
	new_angles[A_HandLV] = HandLV;
	new_angles[A_HandLRot] = HandLRot;
	new_angles[A_HandRH] = HandRH;
	new_angles[A_HandRV] = HandRV;
	new_angles[A_HandRRot] = HandRRot;
	//if(!skel.ValidateHandAngles(new_angles)) {
	if(!skel.ValidateJointAngles(new_angles)) {
		/*if(!skel.HardLimitError()) {
			ostringstream message;
			message << skel.getLastError() << endl << "Continue anyway?" << endl;
			if(AfxMessageBox(message.str().c_str(), MB_OKCANCEL) == IDCANCEL) {
				return false;
			}
		} else {*/
			//AfxMessageBox(skel.getLastError().c_str());
			//return false;
		/*}*/
	}
	
	//doc_ptr->setFixedHandAngles(mHandPostureLockCheckbox);
	skel.SetAngles(new_angles);
	UndoablePostureChange *undoEvent = new UndoablePostureChange(doc_ptr->getCurrentFrame(),old_angles,new_angles,this->mDocPtr->getFrameType(mDocPtr->getCurrentFrame()),FT_Keyframe);
	doc_ptr->addUndoEvent(undoEvent);
	doc_ptr->MakeDirtyAndUpdateViews();
	return true;
}

void Dg_HandPosture::OnBnClickedOk()
{
	// Save old angle in case this doesn't work
	int OldAngle = *CurrentAngle;
	
	if(CWnd::UpdateData(TRUE)) { //Data transfer successful
		if(UpdateDocument()) {
			OnOK();
		}
	}

	///Yijun
	//UpdateDocument();
	C_Hom_Doc* doc_ptr = Get_App_Ptr()->Get_Document_Ptr();
	Skeleton& skel = *doc_ptr->GetSkeleton();

	/*if(Round_Double(skel.getExtraAngle(EA_HandLDeviation)) < -21){
		skel.error = "Left wrist radial deviation exceeded!";
		//AfxMessageBox((char*)("Wrist Angles exceeded!"));
		AfxMessageBox(skel.getLastError().c_str());
		// if there were errors, set the changed angle back
		*CurrentAngle = OldAngle;
		UpdateData(false);
		UpdateDocument();
	}
	if(Round_Double(skel.getExtraAngle(EA_HandRDeviation)) < -21){
		skel.error = "Right wrist radial deviation exceeded!";
		//AfxMessageBox((char*)("Wrist Angles exceeded!"));
		AfxMessageBox(skel.getLastError().c_str());
		// if there were errors, set the changed angle back
		*CurrentAngle = OldAngle;
		UpdateData(false);
		UpdateDocument();
	}
	if(Round_Double(skel.getExtraAngle(EA_HandLDeviation)) > 38){
		skel.error = "Left wrist ulnar deviation exceeded!";
		//AfxMessageBox((char*)("Wrist Angles exceeded!"));
		AfxMessageBox(skel.getLastError().c_str());
		// if there were errors, set the changed angle back
		*CurrentAngle = OldAngle;
		UpdateData(false);
		UpdateDocument();
	}
	if(Round_Double(skel.getExtraAngle(EA_HandRDeviation)) > 38){
		skel.error = "Right wrist ulnar deviation exceeded!";
		//AfxMessageBox((char*)("Wrist Angles exceeded!"));
		AfxMessageBox(skel.getLastError().c_str());
		// if there were errors, set the changed angle back
		*CurrentAngle = OldAngle;
		UpdateData(false);
		UpdateDocument();
	}*/
}

void Dg_HandPosture::OnEnSetfocusHandLh()
{
	CurrentAngle = &HandLH;
	mDocPtr->cueAngle(AngleCueData(A_HandLH, JT_LWRIST, JT_LHAND, AT_LimbHL, A_HandLV));
}

void Dg_HandPosture::OnEnSetfocusHandLv()
{
	CurrentAngle = &HandLV;
	mDocPtr->cueAngle(AngleCueData(A_HandLV, JT_LWRIST, JT_LHAND, AT_LimbVL, A_HandLH));
}

void Dg_HandPosture::OnEnSetfocusHandLrot()
{
	CurrentAngle = &HandLRot;
	// no angle cues set up
	mDocPtr->cancelCue();
}

void Dg_HandPosture::OnEnSetfocusHandRh()
{
	CurrentAngle = &HandRH;
	mDocPtr->cueAngle(AngleCueData(A_HandRH, JT_RWRIST, JT_RHAND, AT_LimbHR, A_HandRV));
}

void Dg_HandPosture::OnEnSetfocusHandRv()
{
	CurrentAngle = &HandRV;
	mDocPtr->cueAngle(AngleCueData(A_HandRV, JT_RWRIST, JT_RHAND, AT_LimbVR, A_HandRH));
}

void Dg_HandPosture::OnEnSetfocusHandRrot()
{
	CurrentAngle = &HandRRot;
	// no cues right now
	mDocPtr->cancelCue();
}

void Dg_HandPosture::Increment(int sign) {
	if(!CurrentAngle) {
		// TODO make this the same message as the body segment angles one
		AfxMessageBox("No Angle Selected");
	}

	// Save old angle in case this doesn't work
	int OldAngle = *CurrentAngle;
	// Get text box values into variables
	if(!UpdateData(true)) return;
	// Increment the selected angle
	*CurrentAngle += sign * Increment_Value[Increment_Group];

	int minval, maxval;
	if (CurrentAngle == &HandLV || CurrentAngle == &HandRV){
			minval = -90;
			maxval = 90;
	}
	else{
		minval = -180;
		maxval = 180;
	}

	if(*CurrentAngle >= minval && *CurrentAngle <= maxval){
		// Post values (with newly increment angle) into text boxes
		UpdateData(false);

		///Yijun
		UpdateDocument();
		C_Hom_Doc* doc_ptr = Get_App_Ptr()->Get_Document_Ptr();
		Skeleton& skel = *doc_ptr->GetSkeleton();

		/*if(Round_Double(skel.getExtraAngle(EA_HandLDeviation)) < -21){
			skel.error = "Left wrist radial deviation exceeded!";
			//AfxMessageBox((char*)("Wrist Angles exceeded!"));
			AfxMessageBox(skel.getLastError().c_str());
			// if there were errors, set the changed angle back
			*CurrentAngle = OldAngle;
			UpdateData(false);
			UpdateDocument();
		}
		if(Round_Double(skel.getExtraAngle(EA_HandRDeviation)) < -21){
			skel.error = "Right wrist radial deviation exceeded!";
			//AfxMessageBox((char*)("Wrist Angles exceeded!"));
			AfxMessageBox(skel.getLastError().c_str());
			// if there were errors, set the changed angle back
			*CurrentAngle = OldAngle;
			UpdateData(false);
			UpdateDocument();
		}
		if(Round_Double(skel.getExtraAngle(EA_HandLDeviation)) > 38){
			skel.error = "Left wrist ulnar deviation exceeded!";
			//AfxMessageBox((char*)("Wrist Angles exceeded!"));
			AfxMessageBox(skel.getLastError().c_str());
			// if there were errors, set the changed angle back
			*CurrentAngle = OldAngle;
			UpdateData(false);
			UpdateDocument();
		}
		if(Round_Double(skel.getExtraAngle(EA_HandRDeviation)) > 38){
			skel.error = "Right wrist ulnar deviation exceeded!";
			//AfxMessageBox((char*)("Wrist Angles exceeded!"));
			AfxMessageBox(skel.getLastError().c_str());
			// if there were errors, set the changed angle back
			*CurrentAngle = OldAngle;
			UpdateData(false);
			UpdateDocument();
		}

		// Try to apply
		if(!UpdateDocument()) {
			// if there were errors, set the changed angle back
			*CurrentAngle = OldAngle;
			// and post values back to dialog with non-incremented value
			*CurrentAngle = OldAngle;
			UpdateData(false);
		}*/
	}
}

void Dg_HandPosture::OnBnClickedIncrementPlusHand()
{
	Increment(PLUS);
}

void Dg_HandPosture::OnBnClickedIncrementMinusHand()
{
	Increment(MINUS);
}


void Dg_HandPosture::OnBnClickedSymmetryRToLHand()
{
	UpdateData(true);
	HandLH = HandRH;
	HandLV = HandRV;
	HandLRot = HandRRot;
	LHandType = RHandType;
	UpdateData(false);
}

void Dg_HandPosture::OnBnClickedSymmetryLToRHand()
{
	UpdateData(true);
	HandRH = HandLH;
	HandRV = HandLV;
	HandRRot = HandLRot;
	RHandType = LHandType;
	UpdateData(false);
}

void Dg_HandPosture::OnBnClickedHandneutral()
{
	JointAngles ja = Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton()->GetAngles();
	HandLH = ja[A_ForearmLH];
	HandLV = ja[A_ForearmLV];
	HandLRot = 0;
	HandRH = ja[A_ForearmRH];
	HandRV = ja[A_ForearmRV];
	HandRRot = 0;
	UpdateDocument();
	UpdateData(false);
}
/*
void Dg_HandPosture::OnBnClickedHandposturelock()
{
	UpdateData(true);

	// diable edit boxes
	mLeftHorz.EnableWindow(!this->mHandPostureLockCheckbox);
	mLeftVert.EnableWindow(!this->mHandPostureLockCheckbox);
	mLeftRot.EnableWindow(!this->mHandPostureLockCheckbox);
	mRightHorz.EnableWindow(!this->mHandPostureLockCheckbox);
	mRightVert.EnableWindow(!this->mHandPostureLockCheckbox);
	mRightRot.EnableWindow(!this->mHandPostureLockCheckbox);

	// set values in edit boxes
	HandLH = mDocPtr->GetSkeleton()->GetAngle(A_ForearmLH);
	HandLV = mDocPtr->GetSkeleton()->GetAngle(A_ForearmLV);
	HandLRot = 0;
	HandRH = mDocPtr->GetSkeleton()->GetAngle(A_ForearmRH);
	HandRV = mDocPtr->GetSkeleton()->GetAngle(A_ForearmRV);
	HandRRot = 0;

	UpdateData(false);
}
*/
//----------------------------------------------------
void Dg_HandPosture::GetHandPostureFromDocument() 
{
	this->SetJointAngles(mDocPtr->GetSkeleton()->GetAngles());
}
//--------------------------------------------------
void Dg_HandPosture::OnUndoButton() 
{
	if(!mDocPtr->UndoJointAngleChange())
		return;

	this->GetHandPostureFromDocument();

    this->CWnd::UpdateData(FALSE /*Put to dialog*/);

	// Disable the Undo and Redo buttons if necessary	
	UndoButtonStatus();
    RedoButtonStatus();
}
// ----------------------------------------------
void Dg_HandPosture::OnBnClickedRedobutton()
{
	if(!mDocPtr->RedoJointAngleChange())
		return;

	this->GetHandPostureFromDocument();

    this->CWnd::UpdateData(FALSE /*Put to dialog*/);

	// Disable the Undo and Redo buttons if necessary	
	UndoButtonStatus();
    RedoButtonStatus();
}

//--------------------------------------------------------
void Dg_HandPosture::UndoButtonStatus()
{
	mUndoButtonControl.EnableWindow(mDocPtr->CanUndo());
}

void Dg_HandPosture::RedoButtonStatus()
{
	mRedoButtonControl.EnableWindow(mDocPtr->CanRedo());
}

//---------------------------------------------------------------------------
void Dg_HandPosture::SetJointAngles
	( const JointAngles& aJointAnglesRef )
{
   this->HandRV  =  aJointAnglesRef[A_HandRV];
   this->HandRH  =  aJointAnglesRef[A_HandRH];
   this->HandRRot  =  aJointAnglesRef[A_HandRRot];
   
   this->HandLV  =  aJointAnglesRef[A_HandLV];
   this->HandLH  =  aJointAnglesRef[A_HandLH];
   this->HandLRot  =  aJointAnglesRef[A_HandLRot];

}


void Dg_HandPosture::OnPrintPrintwindow()
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


void Dg_HandPosture::OnSysCommand(UINT nID, LPARAM lParam)
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

BOOL Dg_HandPosture::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if(zDelta < 0) 
		Increment(MINUS);
	else if (zDelta > 0)
		Increment(PLUS);
	
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}