#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include "Dg_CameraOblique.h"
#include "../c4eLibNew/Units.hpp"
#include "GLObliqueView.h"
#include "GLOblique.h"

#ifdef _DEBUG
    #undef THIS_FILE
    static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

Dg_CameraOblique::Dg_CameraOblique(C_Hom_Doc *aDocPtr,CWnd* pParent) : CDialog(Dg_CameraOblique::IDD,pParent),mDocPtr(aDocPtr)
{
	//{{AFX_DATA_INIT(Dg_CameraOblique)
	mAltitude = 0;
	mAzimuth = 0;
	mDistance = 0;
	mFocalLength = 0;
	mHOffset = 0;
	mVOffset = 0;
	mDistanceUnits = _T("");
	//}}AFX_DATA_INIT

    ASSERT(mDocPtr != NULL);

    if(aDocPtr->Is_English())
    {
        mDistanceUnits = "in"; 
	    mCam_HUnits = "in";
	    mCam_VUnits = "in";
        mUnitsConversion = U_MM_IN;
        mMinDistance = Round_Int(MIN_CAM_DISTANCE * U_MM_IN);
        mMaxDistance = Round_Int(MAX_CAM_DISTANCE * U_MM_IN);
    }
    else //Units in metric
    {   
        mDistanceUnits = "cm"; 
        mCam_HUnits = "cm";
	    mCam_VUnits = "cm";
	    mUnitsConversion = U_MM_CM;
        mMinDistance = Round_Int(MIN_CAM_DISTANCE * U_MM_CM);
        mMaxDistance = Round_Int(MAX_CAM_DISTANCE * U_MM_CM);
    }

    CGLObliqueView *oblique = (CGLObliqueView*)(mDocPtr->GLViews[VIEW_OBLIQUE]);
    mFocalLength = oblique->getCamera().focalLength; //Always in mm
    mAltitude = oblique->getCamera().altitude; //Always in degrees
    mAzimuth = oblique->getCamera().azimuth; //Always in degrees
	mHOffset = oblique->getCamera().horizontalOffset * mm_TO_cm;
	mVOffset = oblique->getCamera().verticalOffset * mm_TO_cm;

	if(mDocPtr->getDescriptionRef().Units() == U_ENGLISH) 
	{
        mDistance = int(oblique->getCamera().distance * mm_TO_cm * cm_TO_in);
	    mHOffset = mHOffset * cm_TO_in;
	    mVOffset = mVOffset * cm_TO_in;
    }
    else
        mDistance = int(oblique->getCamera().distance * mm_TO_cm);
}

void Dg_CameraOblique::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(Dg_CameraOblique)
    DDX_Text(pDX, IDC_CAMERA_ALTITUDE, mAltitude);
    DDV_MinMaxInt(pDX, mAltitude, -90, 90);
    DDX_Text(pDX, IDC_CAMERA_AZIMUTH, mAzimuth);
    DDV_MinMaxInt(pDX, mAzimuth, -180, 180);
    DDX_Text(pDX, IDC_CAMERA_DISTANCE, mDistance);
    DDV_MinMaxInt(pDX, mDistance, mMinDistance, mMaxDistance );
    DDX_Text(pDX, IDC_CAMERA_FOCAL_LENGTH, mFocalLength);
    DDV_MinMaxInt(pDX, mFocalLength, 20, 2000);
    DDX_Text(pDX, IDC_CAMERADISTANCE_UNITS, mDistanceUnits);
    DDX_Text(pDX, IDC_HORIZONTAL_OFFSET, mHOffset);
    DDX_Text(pDX, IDC_VERTICAL_OFFSET, mVOffset);
    DDX_Text(pDX, IDC_CAMERA_DIST_HOR, mCam_HUnits);
    DDX_Text(pDX, IDC_CAMERA_DIST_VERT, mCam_VUnits);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Dg_CameraOblique, CDialog)
	//{{AFX_MSG_MAP(Dg_CameraOblique)
	ON_BN_CLICKED(IDC_RedrawButton, OnRedrawButton)
	ON_BN_CLICKED(ID_Default_Obl_Cam, OnDefaultOblCam)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void Dg_CameraOblique::UpdateDocument()
{
    CGLObliqueView *oblique = (CGLObliqueView*)(mDocPtr->GLViews[VIEW_OBLIQUE]);

    if(mDistanceUnits == "in")
		oblique->getCamera().distance = int(mDistance / mm_TO_in);
	else
		oblique->getCamera().distance = int(mDistance / mm_TO_cm);

	oblique->getCamera().focalLength = mFocalLength;
    oblique->getCamera().altitude = mAltitude;
	oblique->getCamera().azimuth = mAzimuth;

    if(mDocPtr->getDescriptionRef().Units() == U_ENGLISH) 
	{    	
		oblique->getCamera().horizontalOffset = mHOffset / mm_TO_in;
    	oblique->getCamera().verticalOffset = mVOffset / mm_TO_in;
	}
	else
	{
		oblique->getCamera().horizontalOffset = mHOffset * cm_TO_mm;
    	oblique->getCamera().verticalOffset = mVOffset * cm_TO_mm;
	}

    mDocPtr->MakeDirtyAndUpdateViews();
}

void Dg_CameraOblique::OnOK() 
{
    if( this->UpdateData( TRUE ) )
    {
        this->UpdateDocument();   	
        CDialog::OnOK();
    }
}

void Dg_CameraOblique::OnRedrawButton() 
{
    if( this->UpdateData( TRUE ) )
        this->UpdateDocument();
}

void Dg_CameraOblique::OnDefaultOblCam() 
{
	mAltitude = 20;
	mAzimuth = -20;
	mDistance = 2290;
    if(mDistanceUnits == "in")
		mDistance = int(mDistance * mm_TO_in);
	else
		mDistance = int(mDistance * mm_TO_cm);

	mFocalLength = 50;
	mHOffset = 0;
	mVOffset = 0;

	if(this->CWnd::UpdateData(FALSE /*Put to dialog*/))
	{
		this->UpdateDocument();
	}
}
