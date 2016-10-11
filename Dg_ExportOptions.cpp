// dgexport.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h"
#include "Dg_ExportOptions.h"
#include "ExportSummary.hpp"
#include "hom_doc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif 

/////////////////////////////////////////////////////////////////////////////
// Dg_ExportOptions dialog

   Dg_ExportOptions::
Dg_ExportOptions
(
   ExportSummary &   aExportSummaryRef ,
   CWnd*             aParentPtr //=NULL
)
: CDialog(Dg_ExportOptions::IDD, aParentPtr) ,
mExportSummaryRef( aExportSummaryRef )
{
//	//{{AFX_DATA_INIT(Dg_ExportOptions)
//	mBodySegmentAngles = FALSE;
//	mHandForces = FALSE;
//	mJointLocations = FALSE;
//	mJointMoments = FALSE;
//	mPostureAngles = FALSE;
//	mStrength = FALSE;
//	mSummaryResults = FALSE;
//	mLowBack = FALSE;
//	//}}AFX_DATA_INIT

   return;
}

//////////////////////////////////////////////////////////////////////////////
// Overrides
void Dg_ExportOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//	//{{AFX_DATA_MAP(Dg_ExportOptions)
	//	DDX_Radio(pDX, IDC_FILE_MODE, File_Mode);
	//	DDX_Radio(pDX, IDC_DELIMITER, Delimiter_Mode);
	//	DDX_Check(pDX, IDC_ExpBodySegmentAngles, mBodySegmentAngles);
	//	DDX_Check(pDX, IDC_ExpHandForces, mHandForces);
	//	DDX_Check(pDX, IDC_ExpJointLocations, mJointLocations);
	//	DDX_Check(pDX, IDC_ExpJointMoments, mJointMoments);
	//	DDX_Check(pDX, IDC_ExpPostureAngles, mPostureAngles);
	//	DDX_Check(pDX, IDC_ExpStrength, mStrength);
	//	DDX_Check(pDX, IDC_ExpSummaryResults, mSummaryResults);
	//	DDX_Check(pDX, IDC_LowBack, mLowBack);
	//	//}}AFX_DATA_MAP

	//{{AFX_DATA_MAP(Dg_ExportOptions)
	DDX_Radio(pDX, IDC_FILE_MODE, mExportSummaryRef.mFileMode);
	DDX_Radio(pDX, IDC_DELIMITER, mExportSummaryRef.mDelimiterMode);
	DDX_Check(pDX, IDC_ExpBodySegmentAngles, mExportSummaryRef.mBodySegmentAngles);
	DDX_Check(pDX, IDC_ExpHandForces, mExportSummaryRef.mHandForces);
	DDX_Check(pDX, IDC_ExpJointLocations, mExportSummaryRef.mJointLocations);
	DDX_Check(pDX, IDC_ExpJointForces, mExportSummaryRef.mJointForces);
	DDX_Check(pDX, IDC_ExpJointMoments, mExportSummaryRef.mJointMoments);
	DDX_Check(pDX, IDC_ExpPostureAngles, mExportSummaryRef.mPostureAngles);
	DDX_Check(pDX, IDC_ExpStrength, mExportSummaryRef.mStrength);
	DDX_Check(pDX, IDC_ExpSummaryResults, mExportSummaryRef.mSummaryResults);
	DDX_Check(pDX, IDC_LowBack, mExportSummaryRef.mLowBack);
    DDX_Check(pDX, IDC_Fatigue, mExportSummaryRef.mFatigueReport);
	DDX_Check(pDX, IDC_Balance, mExportSummaryRef.mBalanceReport);
	DDX_Check(pDX, IDC_COLUMNHEADERS, mExportSummaryRef.mColumnHeaders);
	DDX_Check(pDX, IDC_ExpShoulderAnalysis, mExportSummaryRef.mShoulderAnalysis);
	DDX_Check(pDX, IDC_ExpShoulderGeometry, mExportSummaryRef.mShoulderGeometry);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(Dg_ExportOptions, CDialog)
	ON_BN_CLICKED(IDD_SelectAllButton_Export, OnSelectAllButton)
	ON_BN_CLICKED(IDD_SelectNoneButton_Export, OnSelectNoneButton)
	ON_BN_CLICKED(IDD_OpenReports_Export, OnOpenReportsButton) 
	//ON_BN_CLICKED(IDC_ExpShoulderGeometry, &Dg_ExportOptions::OnBnClickedExpshouldergeometry)
	//ON_BN_CLICKED(IDC_ExpShoulderAnalysis, &Dg_ExportOptions::OnBnClickedExpshoulderanalysis)
END_MESSAGE_MAP()


void Dg_ExportOptions::OnSelectAllButton() {

	mExportSummaryRef.mBodySegmentAngles = TRUE;
	mExportSummaryRef.mHandForces = TRUE;
	mExportSummaryRef.mJointLocations = TRUE;
	mExportSummaryRef.mJointForces = TRUE;
	mExportSummaryRef.mJointMoments = TRUE;
	mExportSummaryRef.mPostureAngles = TRUE;
	mExportSummaryRef.mStrength = TRUE;
	mExportSummaryRef.mSummaryResults = TRUE;
	mExportSummaryRef.mLowBack = TRUE;
    mExportSummaryRef.mFatigueReport = TRUE;
	mExportSummaryRef.mBalanceReport = TRUE;
	mExportSummaryRef.mColumnHeaders = TRUE;
	mExportSummaryRef.mShoulderAnalysis = TRUE;
	mExportSummaryRef.mShoulderGeometry = TRUE;

	this->UpdateData( FALSE );
}

void Dg_ExportOptions::OnSelectNoneButton() {
	
	mExportSummaryRef.mBodySegmentAngles = FALSE;
	mExportSummaryRef.mHandForces = FALSE;
	mExportSummaryRef.mJointLocations = FALSE;
	mExportSummaryRef.mJointForces = FALSE;
	mExportSummaryRef.mJointMoments = FALSE;
	mExportSummaryRef.mPostureAngles = FALSE;
	mExportSummaryRef.mStrength = FALSE;
	mExportSummaryRef.mSummaryResults = FALSE;
	mExportSummaryRef.mLowBack = FALSE;
    mExportSummaryRef.mFatigueReport = FALSE;
	mExportSummaryRef.mBalanceReport = FALSE;
	mExportSummaryRef.mColumnHeaders = FALSE;
	mExportSummaryRef.mShoulderAnalysis = FALSE;
	mExportSummaryRef.mShoulderGeometry = FALSE;

	this->UpdateData( FALSE );
}

void Dg_ExportOptions::OnOpenReportsButton() {

	C_Hom_Doc *docptr = Get_App_Ptr()->Get_Document_Ptr();

	docptr->GetReportWindowData(ID_REPORTS_ANALYSIS_SUMMARY, docptr->mReportWDMap[ID_REPORTS_ANALYSIS_SUMMARY]);
	docptr->GetReportWindowData(ID_REPORTS_ANTHROPOMETRY, docptr->mReportWDMap[ID_REPORTS_ANTHROPOMETRY]);
	docptr->GetReportWindowData(ID_REPORTS_BALANCE, docptr->mReportWDMap[ID_REPORTS_BALANCE]);
	docptr->GetReportWindowData(ID_REPORTS_FATIGUE, docptr->mReportWDMap[ID_REPORTS_FATIGUE]);
	docptr->GetReportWindowData(ID_REPORTS_POSTURE, docptr->mReportWDMap[ID_REPORTS_POSTURE]);
	docptr->GetReportWindowData(ID_REPORTS_JOINT_LOCATIONS, docptr->mReportWDMap[ID_REPORTS_JOINT_LOCATIONS]);
	docptr->GetReportWindowData(ID_REPORTS_JOINT_MOMENTS, docptr->mReportWDMap[ID_REPORTS_JOINT_MOMENTS]);
	docptr->GetReportWindowData(ID_REPORTS_JOINT_FORCES, docptr->mReportWDMap[ID_REPORTS_JOINT_FORCES]);
	docptr->GetReportWindowData(ID_REPORTS_3D_LOWBACK, docptr->mReportWDMap[ID_REPORTS_3D_LOWBACK]);
	docptr->GetReportWindowData(ID_REPORTS_SAGITTAL, docptr->mReportWDMap[ID_REPORTS_SAGITTAL]);
	docptr->GetReportWindowData(ID_REPORTS_STRENGTH, docptr->mReportWDMap[ID_REPORTS_STRENGTH]);
	docptr->GetReportWindowData(ID_REPORTS_STRENGTH_DIR_VECTORS, docptr->mReportWDMap[ID_REPORTS_STRENGTH_DIR_VECTORS]);
	docptr->GetReportWindowData(ID_REPORTS_TASK_SUMMARY, docptr->mReportWDMap[ID_REPORTS_TASK_SUMMARY]);
	docptr->GetReportWindowData(ID_REPORTS_SHOULDERANALYSIS, docptr->mReportWDMap[ID_REPORTS_SHOULDERANALYSIS]);
	docptr->GetReportWindowData(ID_REPORTS_SHOULDERGEOMETRY, docptr->mReportWDMap[ID_REPORTS_SHOULDERGEOMETRY]);

	// Each report will by default be checked only if it is
    //		currently open.

	/*
		From email:
			Summary Results is the Analysis Summary Report values
			Strength Capabilities is the Strength Capabilities Report values
			Low Back is either the Lowback Analysis -3D or Low Back Analysis - Sagittal Plane Reports
			Localized Fatigue is the data from the Localized Fatigue Report
			Balance is from the Balance Report
			Hand Forces - no Report
			Body Segment Angles - no Report
			Posture Angles is the Posture Report
			Joint Locations is the Locations Report
			Joint Forces is the Forces Report
			Joint Moments is the Joint Moments Report
	*/

	mExportSummaryRef.mBodySegmentAngles = FALSE;	// no Report
	mExportSummaryRef.mHandForces = FALSE;	// no Report

	mExportSummaryRef.mSummaryResults = docptr->mReportWDMap[ID_REPORTS_ANALYSIS_SUMMARY].mIsOpen;
	mExportSummaryRef.mStrength = docptr->mReportWDMap[ID_REPORTS_STRENGTH].mIsOpen;
	mExportSummaryRef.mLowBack = docptr->mReportWDMap[ID_REPORTS_3D_LOWBACK].mIsOpen || 
								 docptr->mReportWDMap[ID_REPORTS_SAGITTAL].mIsOpen;
	mExportSummaryRef.mFatigueReport = docptr->mReportWDMap[ID_REPORTS_FATIGUE].mIsOpen;
	mExportSummaryRef.mBalanceReport = docptr->mReportWDMap[ID_REPORTS_BALANCE].mIsOpen;
	mExportSummaryRef.mPostureAngles = docptr->mReportWDMap[ID_REPORTS_POSTURE].mIsOpen;
	mExportSummaryRef.mJointLocations = docptr->mReportWDMap[ID_REPORTS_JOINT_LOCATIONS].mIsOpen;
	mExportSummaryRef.mJointForces = docptr->mReportWDMap[ID_REPORTS_JOINT_FORCES].mIsOpen;
	mExportSummaryRef.mJointMoments = docptr->mReportWDMap[ID_REPORTS_JOINT_MOMENTS].mIsOpen;
	mExportSummaryRef.mShoulderAnalysis = docptr->mReportWDMap[ID_REPORTS_SHOULDERANALYSIS].mIsOpen;
	mExportSummaryRef.mShoulderGeometry = docptr->mReportWDMap[ID_REPORTS_SHOULDERGEOMETRY].mIsOpen;
	
	mExportSummaryRef.mColumnHeaders = TRUE;	// default to be true

	this->UpdateData( FALSE );
}


//---------------------------------------------------------------------------
void Dg_ExportOptions ::                                               OnOK
( void ) 
{
   CDialog::OnOK();
   mExportSummaryRef.Update(); //computes derived data attributes
   return;
}

/*
BEGIN_MESSAGE_MAP(Dg_ExportOptions, CDialog)
	//{{AFX_MSG_MAP(Dg_ExportOptions)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &Dg_ExportOptions::OnBnClickedall)
	ON_BN_CLICKED(IDC_BUTTON4, &Dg_ExportOptions::OnBnClickedNone)
	ON_BN_CLICKED(IDC_BUTTON3, &Dg_ExportOptions::OnBnClickedOpenReports)
END_MESSAGE_MAP()
<<<<<<< HEAD





void Dg_ExportOptions::OnBnClickedall()
{
	// TODO: Add your control notification handler code here
	mExportSummaryRef.mSummaryResults = true;
	mExportSummaryRef.mBalanceReport = true;
	mExportSummaryRef.mBodySegmentAngles = true;
	mExportSummaryRef.mFatigueReport = true;
	mExportSummaryRef.mHandForces = true;
	mExportSummaryRef.mJointLocations = true;
	mExportSummaryRef.mJointMoments = true;
	mExportSummaryRef.mJointForces = true;
	mExportSummaryRef.mLowBack = true;
	mExportSummaryRef.mStrength = true;
	mExportSummaryRef.mPostureAngles = true;
	UpdateData(false);
}


void Dg_ExportOptions::OnBnClickedNone()
{
	// TODO: Add your control notification handler code here
	mExportSummaryRef.mSummaryResults = false;
	mExportSummaryRef.mBalanceReport = false;
	mExportSummaryRef.mBodySegmentAngles = false;
	mExportSummaryRef.mFatigueReport = false;
	mExportSummaryRef.mHandForces = false;
	mExportSummaryRef.mJointLocations = false;
	mExportSummaryRef.mJointMoments = false;
	mExportSummaryRef.mJointForces = false;
	mExportSummaryRef.mLowBack = false;
	mExportSummaryRef.mStrength = false;
	mExportSummaryRef.mPostureAngles = false;
	UpdateData(false);
}


void Dg_ExportOptions::OnBnClickedOpenReports()
{
	C_Hom_Doc *docptr = Get_App_Ptr()->Get_Document_Ptr();

   docptr->GetReportWindowData(ID_REPORTS_ANALYSIS_SUMMARY, docptr->mReportWDMap[ID_REPORTS_ANALYSIS_SUMMARY]);
   docptr->GetReportWindowData(ID_REPORTS_ANTHROPOMETRY, docptr->mReportWDMap[ID_REPORTS_ANTHROPOMETRY]);
   docptr->GetReportWindowData(ID_REPORTS_BALANCE, docptr->mReportWDMap[ID_REPORTS_BALANCE]);
   //docptr->GetReportWindowData(ID_REPORTS_BALANCE_STANDING, docptr->mReportWDMap[ID_REPORTS_BALANCE_STANDING]);
   //docptr->GetReportWindowData(ID_REPORTS_BALANCE_SEATED, docptr->mReportWDMap[ID_REPORTS_BALANCE_SEATED]);
   docptr->GetReportWindowData(ID_REPORTS_FATIGUE, docptr->mReportWDMap[ID_REPORTS_FATIGUE]);
   docptr->GetReportWindowData(ID_REPORTS_POSTURE, docptr->mReportWDMap[ID_REPORTS_POSTURE]);
   docptr->GetReportWindowData(ID_REPORTS_JOINT_LOCATIONS, docptr->mReportWDMap[ID_REPORTS_JOINT_LOCATIONS]);
   docptr->GetReportWindowData(ID_REPORTS_JOINT_MOMENTS, docptr->mReportWDMap[ID_REPORTS_JOINT_MOMENTS]);
   docptr->GetReportWindowData(ID_REPORTS_JOINT_FORCES, docptr->mReportWDMap[ID_REPORTS_JOINT_FORCES]);
   docptr->GetReportWindowData(ID_REPORTS_3D_LOWBACK, docptr->mReportWDMap[ID_REPORTS_3D_LOWBACK]);
   docptr->GetReportWindowData(ID_REPORTS_SAGITTAL, docptr->mReportWDMap[ID_REPORTS_SAGITTAL]);
   //docptr->GetReportWindowData(ID_REPORTS_SPINAL, docptr->mReportWDMap[ID_REPORTS_SPINAL]);
   docptr->GetReportWindowData(ID_REPORTS_STRENGTH, docptr->mReportWDMap[ID_REPORTS_STRENGTH]);
   docptr->GetReportWindowData(ID_REPORTS_STRENGTH_DIR_VECTORS, docptr->mReportWDMap[ID_REPORTS_STRENGTH_DIR_VECTORS]);
   docptr->GetReportWindowData(ID_REPORTS_TASK_SUMMARY, docptr->mReportWDMap[ID_REPORTS_TASK_SUMMARY]);
   docptr->GetReportWindowData(ID_REPORTS_SHOULDERANALYSIS, docptr->mReportWDMap[ID_REPORTS_SHOULDERANALYSIS]);

   // Each report will by default be checked only if it is
   //		currently open.
   mExportSummaryRef.mSummaryResults =
      docptr->mReportWDMap[ID_REPORTS_ANALYSIS_SUMMARY].mIsOpen;

   mExportSummaryRef.mBalanceReport = 
	   docptr->mReportWDMap[ID_REPORTS_BALANCE].mIsOpen;

   mExportSummaryRef.mFatigueReport =
      docptr->mReportWDMap[ID_REPORTS_FATIGUE].mIsOpen;

   mExportSummaryRef.mPostureAngles =
      docptr->mReportWDMap[ID_REPORTS_POSTURE].mIsOpen;

   mExportSummaryRef.mJointLocations =
      docptr->mReportWDMap[ID_REPORTS_JOINT_LOCATIONS].mIsOpen;

   mExportSummaryRef.mJointMoments =
      docptr->mReportWDMap[ID_REPORTS_JOINT_MOMENTS].mIsOpen;

   mExportSummaryRef.mJointForces =
      docptr->mReportWDMap[ID_REPORTS_JOINT_FORCES].mIsOpen;

   mExportSummaryRef.mLowBack =
      docptr->mReportWDMap[ID_REPORTS_3D_LOWBACK].mIsOpen;

   mExportSummaryRef.mStrength =
      docptr->mReportWDMap[ID_REPORTS_STRENGTH].mIsOpen;

   mExportSummaryRef.mHandForces = false;
   mExportSummaryRef.mBodySegmentAngles = false;

   this->UpdateData( FALSE );
}
=======
*/


//void Dg_ExportOptions::OnBnClickedExpshouldergeometry()
//{
	// TODO: Add your control notification handler code here
//}


//void Dg_ExportOptions::OnBnClickedExpshoulderanalysis()
//{
	// TODO: Add your control notification handler code here
//}

