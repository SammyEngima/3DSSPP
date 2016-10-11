#if !defined Dg_JointLoads_CPP
#define Dg_JointLoads_CPP

#include "stdafx.h"
#include "force.hpp"
#include "torque.hpp"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include "Dg_JointLoads.h"
#include "../c4eLibNew/Units.hpp"
#include "UndoApplied.h"
#include "GLStickView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// Joints that display in 3D view and 'External Applied Forces and Moments' section
#define EXT3D_NUM_JOINTS 14

using std::set;


int	Dg_JointLoads::m_whichjoint = 0;
/////////////////////////////////////////////////////////////////////////////
// Dg_JointLoads dialog

int CountExternalForces(void)
{
    Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
    int count = 0;

    // Check the torques of the right and left hands
    if(skel.getExtTorque(JT_LHAND) != Vector3(0,0,0))
        ++count;
    if(skel.getExtTorque(JT_RHAND) != Vector3(0,0,0))
        ++count;

    // Check other joints
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        if(i == JT_RHAND || i == JT_LHAND) continue;
        if(skel.getExtForce((JointID)i) != Vector3(0,0,0) || skel.getExtTorque((JointID)i) != Vector3(0,0,0))
            ++count;
    }

    return count;
}

// This is where the text for 
void DisplayExtForces(CString *ret)
{
	Force tempForce(0,3);
	Torque tempTorque(0,3);
	ret->Empty();
	*ret = "Force\t\t  X\t  Y\t  Z\t\tMoment\t\t  X\t  Y\t  Z\n";
	*ret = *ret + "  (" + tempForce.UnitsText() + ")\t\t\t\t\t\t" + "  (" +  tempTorque.UnitsText() + ")\n";

	Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();

	// Check the torques of the right and left hand loads and if so display them
    if(skel.getExtTorque(JT_LHAND) != Vector3(0,0,0))
        CreateJointString(ret, "Left Hand", skel.getExtForce(JT_LHAND), skel.getExtTorque(JT_LHAND));
    if(skel.getExtTorque(JT_RHAND) != Vector3(0,0,0))
        CreateJointString(ret, "Right Hand", skel.getExtForce(JT_RHAND), skel.getExtTorque(JT_RHAND));

    // Check other joints
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        if(i == JT_RHAND || i == JT_LHAND) continue;
        if(skel.getExtForce((JointID)i) != Vector3(0,0,0) || skel.getExtTorque((JointID)i) != Vector3(0,0,0))
            CreateJointString(ret, jointNames[i], skel.getExtForce((JointID)i), skel.getExtTorque((JointID)i));
    }
}

void DisplayExtForceNames(CString *ret)
{
	ret->Empty();
	*ret += "Joint Name\n\n";
	
	Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();

	// Insert the names of the right and left hand
    if(skel.getExtTorque(JT_LHAND) != Vector3(0,0,0))
        *ret += "Left Hand\n";
    if(skel.getExtTorque(JT_RHAND) != Vector3(0,0,0))
        *ret += "Right Hand\n";

    // Insert the names of the other joints
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        if(i == JT_LHAND || i == JT_RHAND) continue;
        if(skel.getExtForce((JointID)i) != Vector3(0,0,0) || skel.getExtTorque((JointID)i) != Vector3(0,0,0))
        {
            *ret += jointNames[i];
			*ret += "\n";
        }
    }
}

Dg_JointLoads::Dg_JointLoads(C_Hom_Doc *aDocPtr, CWnd* pParent) : CDialog(Dg_JointLoads::IDD, pParent),
                                                                  mDocPtr(aDocPtr),tempTorque(0,3),tempForce(0,3)
{
	//{{AFX_DATA_INIT(Dg_JointLoads)
	mXComp = 0.0f;
	mYComp = 0.0f;
	mZComp = 0.0f;
	mXMoment = 0.0f;
	mYMoment = 0.0f;
	mZMoment = 0.0f;
	mMomentUnits = _T("");
	mForceUnits = _T("");
	//}}AFX_DATA_INIT

	ASSERT( mDocPtr != NULL );

    Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();

	const JointID order[EXT3D_NUM_JOINTS] = {JT_LANKLE,JT_LELBOW,JT_LHIP,JT_LKNEE,JT_LSHOULDER, JT_LWRIST,JT_L5S1_TEMP,JT_RANKLE,JT_RELBOW, JT_RHIP,JT_RKNEE,JT_RSHOULDER,
	JT_RWRIST, JT_TRAGIONLOC};

	// Put all the values into the Dialog
	for(int i = 0; i < EXT3D_NUM_JOINTS; i++)
	{
        for(int j = 0; j < 3; ++j)
		{
			loadStorage[i][j] = skel.getExtForce(order[i])[j];
    		loadStorage[i][3+j] = skel.getExtTorque(order[i])[j];
		}
	}

	mXComp = loadStorage[m_whichjoint][0];
	mYComp = loadStorage[m_whichjoint][1];
	mZComp = loadStorage[m_whichjoint][2];
	mXMoment = loadStorage[m_whichjoint][3];
	mYMoment = loadStorage[m_whichjoint][4];
	mZMoment = loadStorage[m_whichjoint][5];
}


void Dg_JointLoads::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_JointLoads)
	DDX_Text(pDX, IDC_XComp, mXComp);
	DDX_Text(pDX, IDC_YComp, mYComp);
	DDX_Text(pDX, IDC_ZComp, mZComp);
	DDX_Text(pDX, IDC_XMoment, mXMoment);
	DDX_Text(pDX, IDC_YMoment, mYMoment);
	DDX_Text(pDX, IDC_MomentUnits, mMomentUnits);
	DDX_Text(pDX, IDC_ForceUnits, mForceUnits);
	DDX_Text(pDX, IDC_ZMoment, mZMoment);
	DDV_MinMaxDouble(pDX, mXComp, -10000.f, 10000.f);
	DDV_MinMaxDouble(pDX, mYComp, -10000.f, 10000.f);
	DDV_MinMaxDouble(pDX, mZComp, -10000.f, 10000.f);
	DDV_MinMaxDouble(pDX, mXMoment, -10000.f, 10000.f);
	DDV_MinMaxDouble(pDX, mYMoment, -10000.f, 10000.f);
	DDV_MinMaxDouble(pDX, mZMoment, -10000.f, 10000.f);
	DDX_CBIndex(pDX, IDC_JOINTNAMECOMBO, m_whichjoint);

	///Yijun
	DDX_Control(pDX, IDC_UndoButton, mUndoButtonControl);
	DDX_Control(pDX, IDC_RedoButton, mRedoButtonControl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Dg_JointLoads, CDialog)
	//{{AFX_MSG_MAP(Dg_JointLoads)
	ON_BN_CLICKED(IDC_REDRAW, OnRedrawJointApp)
	ON_BN_CLICKED(IDC_ZERO_ALL, OnZeroAll)
	ON_BN_CLICKED(IDC_ZERO_Visible, OnZEROVisible)

	///Yijun
	ON_BN_CLICKED(IDC_UndoButton, OnUndoDirectManipulation)
	ON_BN_CLICKED(IDC_RedoButton, OnRedoDirectManipulation)
	ON_CBN_SELCHANGE(IDC_JOINTNAMECOMBO, OnSelchangeJointnamecombo)
	ON_EN_CHANGE(IDC_XComp, OnChangeXComp)
	ON_UPDATE_COMMAND_UI(IDC_UndoButton, OnUpdateUndoDirectManipulation)
	ON_UPDATE_COMMAND_UI(IDC_RedoButton, OnUpdateRedoDirectManipulation)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_YComp, &Dg_JointLoads::OnEnChangeYcomp)
	ON_EN_CHANGE(IDC_ZComp, &Dg_JointLoads::OnEnChangeZcomp)
	ON_EN_CHANGE(IDC_XMoment, &Dg_JointLoads::OnEnChangeXmoment)
	ON_EN_CHANGE(IDC_YMoment, &Dg_JointLoads::OnEnChangeYmoment)
	ON_EN_CHANGE(IDC_ZMoment, &Dg_JointLoads::OnEnChangeZmoment)
	ON_COMMAND(ID_PRINT_PRINTWINDOW, &Dg_JointLoads::OnPrintPrintwindow)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_JointLoads message handlers

void Dg_JointLoads::OnRedrawJointApp() 
{
	// ARTHUR::ADDED [05/07/01] - Put the data from editboxes into member vars
	this->UpdateData(true);
	UpdateStorage(m_whichjoint);

	// ARTHUR::ADDED [05/07/01] - Update the data in the HomDoc

	const JointID order[EXT3D_NUM_JOINTS] = {JT_LANKLE,JT_LELBOW,JT_LHIP,JT_LKNEE,JT_LSHOULDER, 
		JT_LWRIST,JT_L5S1_TEMP,JT_RANKLE,JT_RELBOW, JT_RHIP,JT_RKNEE,JT_RSHOULDER,JT_RWRIST, JT_TRAGIONLOC};

	// make group undo event
	int left = Get_App_Ptr()->Get_Document_Ptr()->LeftSelect();
	int right = Get_App_Ptr()->Get_Document_Ptr()->RightSelect();
	GroupEvent* groupEvent = new GroupEvent(left, right);

	for(int frame = left; frame <= right; frame++) {
		//Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
		Skeleton& skel = *Get_App_Ptr()->Get_Document_Ptr()->getSkeletonAtFrame(frame);

		// retain current forces and torques for undo/redo event
		Vector3 oldForces[NUM_JOINTS],oldTorques[NUM_JOINTS];
		skel.getExtForces(oldForces, true);
		skel.getExtTorques(oldTorques, true);

		// Put all the values from the dialog into the HomDoc
		// Put only the values that the user changed, because we may be setting on multiple frames
		/*for(int j = 0; j < 11; ++j)
		{
			skel.setExtForce(order[j],loadStorage[j]);
			skel.setExtTorque(order[j],loadStorage[j] + 3);
		}*/
		for(std::set<int>::iterator it = changedForces.begin(); it != changedForces.end(); ++it) {
			skel.setExtForce(order[*it], loadStorage[*it]);
		}
		for(std::set<int>::iterator it = changedTorques.begin(); it != changedTorques.end(); ++it) {
			skel.setExtTorque(order[*it], loadStorage[*it] + 3);
		}
		
		// get new forces and torques for undo/redo event
		Vector3 newForces[NUM_JOINTS], newTorques[NUM_JOINTS];
		skel.getExtForces(newForces, true);
		skel.getExtTorques(newTorques, true);

		// add undo event for changes to externally applied forces
		//UndoableAppliedLoads *undoEvent = new UndoableAppliedLoads(oldForces,oldTorques);
		UndoableAppliedLoads* undoEvent = new UndoableAppliedLoads(oldForces, oldTorques, newForces, newTorques, frame);
		groupEvent->addEvent(undoEvent);
	}
	
	mDocPtr->addUndoEvent(groupEvent);

	mDocPtr->MakeDirtyAndUpdateViews();

	RedoButtonStatus();
	UndoButtonStatus();
}

// ==========================
// Reset ALL additional loads
void Dg_JointLoads::OnZeroAll() 
{
	this->mXComp = 0;
	this->mYComp = 0;
	this->mZComp = 0;
	this->mXMoment = 0;
	this->mYMoment = 0;
	this->mZMoment = 0;

	// Put all the values from the dialog into the HomDoc
	for (int j = 0; j < EXT3D_NUM_JOINTS; ++j) {
		changedForces.insert(j);
		changedTorques.insert(j);
		for (int k = 0; k < 6; ++k) {
			loadStorage[j][k] = 0;
		}
	}

	this->UpdateData(FALSE);

	OnRedrawJointApp();
}

void Dg_JointLoads::OnZEROVisible() 
{
	this->mXComp = 0;
	this->mYComp = 0;
	this->mZComp = 0;
	this->mXMoment = 0;
	this->mYMoment = 0;
	this->mZMoment = 0;

	for (int k = 0; k < 6; ++k) {
		loadStorage[m_whichjoint][k] = 0;
	}

	changedForces.insert(m_whichjoint);
	changedTorques.insert(m_whichjoint);

	this->UpdateData(FALSE);

	OnRedrawJointApp();
}

void Dg_JointLoads::OnOK()
{
	OnRedrawJointApp();
	CDialog::OnOK();
}

void Dg_JointLoads::UpdateStorage(int aJointNum) 
{
	loadStorage[aJointNum][0] = mXComp ;
	loadStorage[aJointNum][1] = mYComp ;
	loadStorage[aJointNum][2] = mZComp ;
	loadStorage[aJointNum][3] = mXMoment;
	loadStorage[aJointNum][4] = mYMoment;
	loadStorage[aJointNum][5] = mZMoment;
}

void Dg_JointLoads::ForceChanged() {
	changedForces.insert(m_whichjoint);
}
void Dg_JointLoads::TorqueChanged() {
	changedTorques.insert(m_whichjoint);
}

void Dg_JointLoads::OnSelchangeJointnamecombo() 
{
	int oldJoint = m_whichjoint;
	this->UpdateData(TRUE);
	UpdateStorage(oldJoint);

	mXComp   = loadStorage[m_whichjoint][0];
	mYComp   = loadStorage[m_whichjoint][1];
	mZComp   = loadStorage[m_whichjoint][2];
	mXMoment = loadStorage[m_whichjoint][3];
	mYMoment = loadStorage[m_whichjoint][4];
	mZMoment = loadStorage[m_whichjoint][5];

	this->UpdateData(FALSE);
}

void Dg_JointLoads::OnChangeXComp() 
{
	ForceChanged();
}

void Dg_JointLoads::OnEnChangeYcomp()
{
	ForceChanged();
}

void Dg_JointLoads::OnEnChangeZcomp()
{
	ForceChanged();
}

void Dg_JointLoads::OnEnChangeXmoment()
{
	TorqueChanged();
}

void Dg_JointLoads::OnEnChangeYmoment()
{
	TorqueChanged();
}

void Dg_JointLoads::OnEnChangeZmoment()
{
	TorqueChanged();
}


void CreateJointString(CString* ret, const char* jointString, const Vector3 &force, const Vector3 &torque)
{
	CString tmpString;
	tmpString += jointString;
	/*double tempLoads[6];

	for(int k = 0; k < 3; ++k)
	{
		tempForce.Value(force[k], MetricCns);
		tempTorque.Value(torque[k] * U_M_CM, MetricCns);
		tempLoads[k] = Round_Double(tempForce.Value(), 2);
		tempLoads[3+k] = Round_Double(tempTorque.Value(), 2);
	}

	tmpString.Format("\t\t%.1f\t%.1f\t%.1f\t\t\t\t%.1f\t%.1f\t%.1f\n",
                     tempLoads[0],tempLoads[1],tempLoads[2],tempLoads[3],tempLoads[4],tempLoads[5]);*/
	tmpString.Format("\t\t%.1f\t%.1f\t%.1f\t\t\t\t%.1f\t%.1f\t%.1f\n",
					Round_Double(force[0],2),Round_Double(force[1],2),Round_Double(force[2],2),
					Round_Double(torque[0],2),Round_Double(torque[1],2),Round_Double(torque[2],2));
	*ret += tmpString;
}

#endif



void Dg_JointLoads::OnPrintPrintwindow()
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


BOOL Dg_JointLoads::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->AppendMenu(MF_SEPARATOR);// add a separator
	pSysMenu->AppendMenu(MF_STRING,IDR_MENU1,"Print Window");// add print menu

	//grey out
	RedoButtonStatus();
	UndoButtonStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void Dg_JointLoads::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
		if (nID ==IDR_MENU1)             
    {
		OnPrintPrintwindow();
    }
	  else
	CDialog::OnSysCommand(nID, lParam);
}

///Yijun
void Dg_JointLoads::OnUndoDirectManipulation()
{
   // Should not be callable when undo is impossible
   //assert(this->CanUndo());
   C_Hom_Doc* doc_ptr = Get_App_Ptr()->Get_Document_Ptr();
   //this->UndoJointAngleChange();
   doc_ptr -> OnUndoDirectManipulation();

    ASSERT( mDocPtr != NULL );

    Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();

	const JointID order[EXT3D_NUM_JOINTS] = {JT_LANKLE,JT_LELBOW,JT_LHIP,JT_LKNEE,JT_LSHOULDER, JT_LWRIST,JT_L5S1_TEMP,JT_RANKLE,JT_RELBOW, JT_RHIP,JT_RKNEE,JT_RSHOULDER,
	JT_RWRIST, JT_TRAGIONLOC};

	// Put all the values into the Dialog
	for(int i = 0; i < EXT3D_NUM_JOINTS; i++)
	{
        for(int j = 0; j < 3; ++j)
		{
			loadStorage[i][j] = skel.getExtForce(order[i])[j];
    		loadStorage[i][3+j] = skel.getExtTorque(order[i])[j];
		}
	}
    mXComp = loadStorage[m_whichjoint][0];
	mYComp = loadStorage[m_whichjoint][1];
	mZComp = loadStorage[m_whichjoint][2];
	mXMoment = loadStorage[m_whichjoint][3];
	mYMoment = loadStorage[m_whichjoint][4];
	mZMoment = loadStorage[m_whichjoint][5];

	this->UpdateData(false);

	// Gray/ungray the Undo and Redo buttons if necessary	
	UndoButtonStatus();
    RedoButtonStatus();
}

void Dg_JointLoads::OnUpdateUndoDirectManipulation(CCmdUI* pCmdUI) 
{
    C_Hom_Doc* doc_ptr = Get_App_Ptr()->Get_Document_Ptr();
    doc_ptr -> OnUpdateUndoDirectManipulation(pCmdUI);
}

void Dg_JointLoads::UndoButtonStatus()
{
	mUndoButtonControl.EnableWindow(mDocPtr->CanUndo());
}

void Dg_JointLoads::RedoButtonStatus()
{
	mRedoButtonControl.EnableWindow(mDocPtr->CanRedo());
}

void Dg_JointLoads::OnRedoDirectManipulation()
{
   // Should not be callable when undo is impossible
   //assert(this->CanUndo());
   C_Hom_Doc* doc_ptr = Get_App_Ptr()->Get_Document_Ptr();
   //this->UndoJointAngleChange();
   doc_ptr -> OnRedoDirectManipulation();

    ASSERT( mDocPtr != NULL );

    Skeleton &skel = *Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();

	const JointID order[EXT3D_NUM_JOINTS] = {JT_LANKLE,JT_LELBOW,JT_LHIP,JT_LKNEE,JT_LSHOULDER, JT_LWRIST,JT_L5S1_TEMP,JT_RANKLE,JT_RELBOW, JT_RHIP,JT_RKNEE,JT_RSHOULDER,
	JT_RWRIST, JT_TRAGIONLOC};

	// Put all the values into the Dialog
	for(int i = 0; i < EXT3D_NUM_JOINTS; i++)
	{
        for(int j = 0; j < 3; ++j)
		{
			loadStorage[i][j] = skel.getExtForce(order[i])[j];
    		loadStorage[i][3+j] = skel.getExtTorque(order[i])[j];
		}
	}
    mXComp = loadStorage[m_whichjoint][0];
	mYComp = loadStorage[m_whichjoint][1];
	mZComp = loadStorage[m_whichjoint][2];
	mXMoment = loadStorage[m_whichjoint][3];
	mYMoment = loadStorage[m_whichjoint][4];
	mZMoment = loadStorage[m_whichjoint][5];

	this->UpdateData(false);

	// Gray/ungray the Undo and Redo buttons if necessary	
	UndoButtonStatus();
    RedoButtonStatus();
}

void Dg_JointLoads::OnUpdateRedoDirectManipulation(CCmdUI* pCmdUI) 
{
    C_Hom_Doc* doc_ptr = Get_App_Ptr()->Get_Document_Ptr();
    doc_ptr -> OnUpdateRedoDirectManipulation(pCmdUI);
}