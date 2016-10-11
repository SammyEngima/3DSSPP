#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h"
#include "UndoPosture.h"
#include "Types.h"
#include "Dg_PosturePrediction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

int Round(double ToBeRounded) {
    double Absolute  = fabs(ToBeRounded);
    double Remainder = fmod(Absolute,(double)(1.0F));

    int Rounded;
    if(Remainder >= 0.5F)
        Rounded = int(Absolute) + 1;
    else
        Rounded = int(Absolute);
       
    if(ToBeRounded < 0.0F)
        return -Rounded;
    else
        return Rounded;
}

double Round(double value,int decimals)
{
    assert(decimals >= 0);
    assert(decimals <= 10);
       
    if(decimals > 0)
    {
        double multiplier = 1.0F;
        multiplier = pow( (float)10, decimals );
        double rounded = Round(  ((double)value * multiplier)  ) / multiplier;
        return rounded;
    } 
       
    else return Round(value); 
}

Dg_PosturePrediction::Dg_PosturePrediction(C_Hom_Doc* aDocPtr,CWnd* pParent) : CDialog(Dg_PosturePrediction::IDD, pParent),
                                                                               mDocPtr(aDocPtr),mValidState(TRUE)
{
	//{{AFX_DATA_INIT(Dg_PosturePrediction)
	mHandOrientation = -1;
	mHandLocationUnits = _T("");
	mLeftHorizontal = 0.0f;
	mLeftLateral = 0.0f;
	mLeftVertical = 0.0f;
	mRightHorizontal = 0.0f;
	mRightLateral = 0.0f;
	mRightVertical = 0.0f;
	//}}AFX_DATA_INIT

    this->GetHandPositionsFromDocument();
}

void Dg_PosturePrediction::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_PosturePrediction)
	DDX_Radio(pDX, IDC_HAND_ORIENTATION, mHandOrientation);
	DDX_Text(pDX, IDC_HL_UNITS, mHandLocationUnits);
	DDX_Text(pDX, IDC_LEFT_HORIZONTAL, mLeftHorizontal);
	DDX_Text(pDX, IDC_LEFT_LATERAL, mLeftLateral);
	DDX_Text(pDX, IDC_LEFT_VERTICAL, mLeftVertical);
	DDX_Text(pDX, IDC_RIGHT_HORIZONTAL, mRightHorizontal);
	DDX_Text(pDX, IDC_RIGHT_LATERAL, mRightLateral);
	DDX_Text(pDX, IDC_RIGHT_VERTICAL, mRightVertical);
    DDX_Control(pDX, IDC_UndoButton, mUndoButtonControl);
	//}}AFX_DATA_MAP
}

void Dg_PosturePrediction::GetHandPositionsFromDocument()
{
    ASSERT(mDocPtr != NULL);
    Skeleton &lSkeleton = *mDocPtr->GetSkeleton();
    
    // Get hand orientation from document
	mHandOrientation = int(mDocPtr->getTask().Get_Hand_Orientation());

    // Assign display variables
	bool standing = mDocPtr->getPosition() == P_Standing;
	Vector3 leftHand = lSkeleton.getJoint(JT_LHAND) - mDocPtr->getOffset();
    mLeftHorizontal = Round(leftHand[1], 1);
    mLeftLateral = Round(leftHand[0], 1);
    mLeftVertical = Round(leftHand[2], 1);

	Vector3 rightHand = lSkeleton.getJoint(JT_RHAND) - mDocPtr->getOffset();
    mRightHorizontal = Round(rightHand[1], 1);
    mRightLateral = Round(rightHand[0], 1);
    mRightVertical = Round(rightHand[2], 1);

	mHandLocationUnits = CString("") + "(" + mDocPtr->LengthUnits() + ")"; 
}

BOOL Dg_PosturePrediction::UpdateDocument()
{
	Skeleton &lSkeleton = *mDocPtr->GetSkeleton();
	JointAngles tempAngles = lSkeleton.GetAngles();
	LockingMode_e seatPredictMode = mDocPtr->GetLockingMode();
	JointAngles oldAngles;

	mDocPtr->getTask().Put_Hand_Orientation(E_Hand_Orientation(mHandOrientation));

	/*Vector3 ankleCenter = lSkeleton.getJoint(JT_ANKLE_CENTER);
	if(mDocPtr->getPosition() == P_Sitting) {
		Vector3 seatRef = lSkeleton.getSeatReference();
		seatRef -= ankleCenter;
		mLeftHorizontal += seatRef[1];
		mRightHorizontal += seatRef[1];
		mLeftLateral += seatRef[0];
		mRightLateral += seatRef[0];
		mLeftVertical += seatRef[2];
		mRightVertical += seatRef[2];
	}

	mLeftHorizontal += ankleCenter[1];
	mRightHorizontal += ankleCenter[1];
	mLeftLateral += ankleCenter[0];
	mRightLateral += ankleCenter[0];
	mLeftVertical += ankleCenter[2];
	mRightVertical += ankleCenter[2];

	// store current hand positions in case of error
	Vector3 oldLeft = lSkeleton.getJoint(JT_LHAND);
	Vector3 oldRight = lSkeleton.getJoint(JT_RHAND);

	//--- Update hand position data ---

	Vector3 lLeftHand = Vector3(mLeftLateral, mLeftHorizontal, mLeftVertical);
	Vector3 lRightHand = Vector3(mRightLateral, mRightHorizontal, mRightVertical);
	if(mDocPtr->Is_English()) {
		lLeftHand *= 2.54;
		lRightHand *= 2.54;
	}

	oldAngles = tempAngles;

	bool standing = (mDocPtr->getPosition() == P_Standing);
	int frame = mDocPtr->getCurrentFrame();
	if(standing)
	{
		if(seatPredictMode == LM_None)
		{
			tempAngles[A_UpperlegLV] = 180;
			tempAngles[A_UpperlegRV] = 180;
			tempAngles[A_LowerlegLV] = 90;
			tempAngles[A_LowerlegRV] = 90;
			seatPredictMode = LM_Legs;

			UndoablePostureChange *undoEvent = new UndoablePostureChange(frame,oldAngles,tempAngles);
			mDocPtr->addUndoEvent(undoEvent);
		}
	}*/
	Vector3 lLeftHand(mLeftLateral, mLeftHorizontal, mLeftVertical);
	Vector3 lRightHand(mRightLateral, mRightHorizontal, mRightVertical);
	lLeftHand += mDocPtr->getOffset();
	lRightHand += mDocPtr->getOffset();

	if(mDocPtr->Is_English()) {
		lLeftHand *= IN_TO_CM;
		lRightHand *= IN_TO_CM;
	}

	LockingMode_e check = mDocPtr->GetLockingMode();
	if(mDocPtr->PredictPosture(lLeftHand,lRightHand)) {
		mValidState = TRUE;
		mDocPtr->MakeDirtyAndUpdateViews();

		// Set the hand positions to those in the document to make sure they line up exactly
		// 7/10/2014 - Stopped updating to actual positions immediately per Chuck's request
		// GetHandPositionsFromDocument();

		UpdateData(FALSE);

		// Fix undo
		/*if(mDocPtr->getPosition() == P_Sitting)
		{
			// Remove the second to last joint angle on the queue
			// it was the temporary angle used for posture prediction
			if(seatPredictMode == LM_None)
			{
				tempAngles = lSkeleton.GetAngles();
				mDocPtr->UndoJointAngleChange();
				mDocPtr->UndoJointAngleChange();

				UndoablePostureChange *undoEvent = new UndoablePostureChange(frame,oldAngles,tempAngles);
				mDocPtr->addUndoEvent(undoEvent);
				mDocPtr->MakeDirtyAndUpdateViews();
			}
		}*/
		//oldAngles = lSkeleton.GetAngles();
	}
	else
	{
		mValidState = FALSE;

		::MessageBox(NULL, "Posture not attainable.", "Posture Prediction", MB_OK);
		//if(mDocPtr->getPosition() == P_Sitting)
			//mDocPtr->UndoJointAngleChange();
	}

	// Disable/enable Undo button if necessary
	UndoButtonStatus();

	GetHandPositionsFromDocument();
	return mValidState;
}

BEGIN_MESSAGE_MAP(Dg_PosturePrediction, CDialog)
	//{{AFX_MSG_MAP(Dg_PosturePrediction)
	ON_BN_CLICKED(IDC_RedrawButton, OnRedrawButton)
	ON_BN_CLICKED(IDC_UndoButton, OnUndoButton)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_PP_SYMMETRY, &Dg_PosturePrediction::OnBnClickedPpSymmetry)
	ON_COMMAND(ID_PRINT_PRINTWINDOW, &Dg_PosturePrediction::OnPrintPrintwindow)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

void Dg_PosturePrediction::OnRedrawButton() 
{
	if(!this->UpdateData(TRUE))
		return;

	this->UpdateDocument();
}

void Dg_PosturePrediction::OnOK() 
{
    if(!this->UpdateData(TRUE))
        return;

    if(this->UpdateDocument())
        CDialog::OnOK();
}

void Dg_PosturePrediction::OnUndoButton() 
{
    if(!mDocPtr->UndoJointAngleChange())
        return;

    this->GetHandPositionsFromDocument();

    if(this->UpdateData(FALSE))
    {
        mDocPtr->MakeDirtyAndUpdateViews();
        mValidState = TRUE;
    }

	// Disable/enable Undo button if necessary
	UndoButtonStatus();
}

void Dg_PosturePrediction::UndoButtonStatus()
{
	mUndoButtonControl.EnableWindow(mDocPtr->CanUndo());
}

BOOL Dg_PosturePrediction::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Disable/enable Undo button if necessary
	UndoButtonStatus();
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->AppendMenu(MF_SEPARATOR);// add a separator
	pSysMenu->AppendMenu(MF_STRING,IDR_MENU1,"Print Window");// add print menu
	return TRUE;
}

void Dg_PosturePrediction::OnBnClickedPpSymmetry()
{
	this->UpdateData(true);
	this->mLeftHorizontal = this->mRightHorizontal;
	this->mLeftLateral = -this->mRightLateral;
	this->mLeftVertical = this->mRightVertical;
	this->UpdateData(false);
}


void Dg_PosturePrediction::OnPrintPrintwindow()
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


void Dg_PosturePrediction::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
		if (nID ==IDR_MENU1)             
    {
		OnPrintPrintwindow();
    }
	  else
	CDialog::OnSysCommand(nID, lParam);
}
