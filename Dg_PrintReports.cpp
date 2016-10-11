// Dg_PrintReports.cpp : implementation file
//

#include "stdafx.h"
#include "AppVersion.hpp"
#include "hom.h"
//#include "hom_doc.h"	// Unnecessary!
#include "Dg_PrintReports.h"
#include ".\dg_printreports.h"
#include "hom_doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_PrintReports dialog


Dg_PrintReports::Dg_PrintReports(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_PrintReports::IDD, pParent)
{
	static bool initialized = false;
	if(!initialized)
	{
		//{{AFX_DATA_INIT(Dg_PrintReports)
		mReportAnalysisSummary = FALSE;
		mReportAnthropometry = FALSE;
		mReportBalanceStanding = FALSE;
		mReportJointLocations = FALSE;
		mReportCoMLocations = FALSE;
		mReportSagittal = FALSE;
		mReportSpinal = FALSE;
		mReportStrength = FALSE;
		mReportTaskInputSummary = FALSE;
		mReportLowBack = FALSE;
        mReportShoulderAnalysis = FALSE;
        mReportShoulderGeometry = FALSE;
		mReportPosture = FALSE;
		mReportJointMoments = FALSE;
		mReportStrengthDirVectors = FALSE;
		mReportBalanceSeated = FALSE;
		mReportJointForces = FALSE;
		mReportFatigue = FALSE;
		mReportBalance = FALSE;
		initialized = true;
	}
	//}}AFX_DATA_INIT
}

void Dg_PrintReports::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_PrintReports)
	DDX_Check(pDX, IDC_ReportAnaylsisSummary, mReportAnalysisSummary);
	DDX_Check(pDX, IDC_ReportAnthropometry, mReportAnthropometry);
	//DDX_Check(pDX, IDC_ReportBalanceStanding, mReportBalanceStanding);
	DDX_Check(pDX, IDC_ReportFatigue, mReportFatigue);
	DDX_Check(pDX, IDC_ReportJointLocations, mReportJointLocations);
	DDX_Check(pDX, IDC_ReportJointLocations, mReportCoMLocations);
	DDX_Check(pDX, IDC_ReportSagittal, mReportSagittal);
	//DDX_Check(pDX, IDC_ReportSpinal, mReportSpinal);
	DDX_Check(pDX, IDC_ReportStrength, mReportStrength);
	DDX_Check(pDX, IDC_ReportTaskInputSummary, mReportTaskInputSummary);
	DDX_Check(pDX, IDC_ReportLowBack, mReportLowBack);
	#if defined SHOULDERDLL
	DDX_Check(pDX, IDC_ReportShoulderAnalysis, mReportShoulderAnalysis);
	DDX_Check(pDX, IDC_ReportShoulderGeometry, mReportShoulderGeometry);
	#endif
	DDX_Check(pDX, IDC_ReportPosture, mReportPosture);
	DDX_Check(pDX, IDC_ReportJointMoments, mReportJointMoments);
	DDX_Check(pDX, IDC_ReportStrengthDirVectors, mReportStrengthDirVectors);
	//DDX_Check(pDX, IDC_ReportBalanceSeated, mReportBalanceSeated);
	DDX_Check(pDX, IDC_ReportJointForces, mReportJointForces);
	//}}AFX_DATA_MAP
	//DDX_Control(pDX, IDC_ReportBalanceSeated, mReportBalanceSeatedCheck);
	//DDX_Control(pDX, IDC_ReportBalanceStanding, mReportBalanceStandingCheck);
	DDX_Check(pDX, IDC_ReportBalance, mReportBalance);
}


BEGIN_MESSAGE_MAP(Dg_PrintReports, CDialog)
	//{{AFX_MSG_MAP(Dg_PrintReports)
	ON_BN_CLICKED(IDC_SelectAllButton, OnSelectAllButton)
	ON_BN_CLICKED(IDC_SelectNoneButton, OnSelectNoneButton)
	ON_BN_CLICKED(IDC_OpenReports, OnOpenReportsButton)
	//}}AFX_MSG_MAP
        
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_PrintReports message handlers

void Dg_PrintReports::OnSelectAllButton() 
{
    C_Hom_Doc* docptr = Get_App_Ptr()->Get_Document_Ptr();

    mReportAnalysisSummary = TRUE;
	mReportAnthropometry = TRUE;
    
	//if(!(docptr->getPositioning() & 1)) // standing
	/*if(docptr->getPosition() == P_Standing)
        mReportBalanceStanding = TRUE;
	else
        mReportBalanceSeated = TRUE;*/

    mReportFatigue = TRUE;
	mReportJointLocations = TRUE;
	mReportCoMLocations = TRUE;
	mReportSagittal = TRUE;
	//mReportSpinal = TRUE;
	mReportStrength = TRUE;
	mReportTaskInputSummary = TRUE;
	mReportLowBack = TRUE;
    mReportShoulderAnalysis = TRUE;
    mReportShoulderGeometry = TRUE;
	mReportPosture = TRUE;
	mReportJointMoments = TRUE;
	mReportStrengthDirVectors = TRUE;
	mReportJointForces = TRUE;
	mReportBalance = TRUE;
	
   this->UpdateData( FALSE );
}


void Dg_PrintReports::OnSelectNoneButton() 
{
	mReportAnalysisSummary = FALSE;
	mReportAnthropometry = FALSE;
	//mReportBalanceStanding = FALSE;
	mReportFatigue = FALSE;
	mReportJointLocations = FALSE;
	mReportCoMLocations = FALSE;
	mReportSagittal = FALSE;
	//mReportSpinal = FALSE;
	mReportStrength = FALSE;
	mReportTaskInputSummary = FALSE;
	mReportLowBack = FALSE;
    mReportShoulderAnalysis = FALSE;
    mReportShoulderGeometry = FALSE;
	mReportPosture = FALSE;
	mReportJointMoments = FALSE;
	mReportJointForces = FALSE;
	mReportStrengthDirVectors = FALSE;
	//mReportBalanceSeated = FALSE;
	mReportBalance = FALSE;

	this->UpdateData( FALSE );

   return;	
}

BOOL Dg_PrintReports::OnInitDialog() 
{
    CDialog::OnInitDialog();

    C_Hom_Doc *docptr = Get_App_Ptr()->Get_Document_Ptr();
	//mReportBalanceSeatedCheck.EnableWindow(docptr->getPosition() == P_Sitting);//getPositioning() & 1);
	//mReportBalanceStandingCheck.EnableWindow(docptr->getPosition() == P_Standing);//!(docptr->getPositioning() & 1));

    //this->UpdateData( FALSE );	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void Dg_PrintReports::OnOpenReportsButton()
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
   docptr->GetReportWindowData(ID_REPORTS_CENTEROFMASSLOCATIONS, docptr->mReportWDMap[ID_REPORTS_CENTEROFMASSLOCATIONS]);
   docptr->GetReportWindowData(ID_REPORTS_JOINT_MOMENTS, docptr->mReportWDMap[ID_REPORTS_JOINT_MOMENTS]);
   docptr->GetReportWindowData(ID_REPORTS_JOINT_FORCES, docptr->mReportWDMap[ID_REPORTS_JOINT_FORCES]);
   docptr->GetReportWindowData(ID_REPORTS_3D_LOWBACK, docptr->mReportWDMap[ID_REPORTS_3D_LOWBACK]);
   docptr->GetReportWindowData(ID_REPORTS_SAGITTAL, docptr->mReportWDMap[ID_REPORTS_SAGITTAL]);
   //docptr->GetReportWindowData(ID_REPORTS_SPINAL, docptr->mReportWDMap[ID_REPORTS_SPINAL]);
   docptr->GetReportWindowData(ID_REPORTS_STRENGTH, docptr->mReportWDMap[ID_REPORTS_STRENGTH]);
   docptr->GetReportWindowData(ID_REPORTS_STRENGTH_DIR_VECTORS, docptr->mReportWDMap[ID_REPORTS_STRENGTH_DIR_VECTORS]);
   docptr->GetReportWindowData(ID_REPORTS_TASK_SUMMARY, docptr->mReportWDMap[ID_REPORTS_TASK_SUMMARY]);
   docptr->GetReportWindowData(ID_REPORTS_SHOULDERANALYSIS, docptr->mReportWDMap[ID_REPORTS_SHOULDERANALYSIS]);
   docptr->GetReportWindowData(ID_REPORTS_SHOULDERGEOMETRY, docptr->mReportWDMap[ID_REPORTS_SHOULDERGEOMETRY]);

   // Each report will by default be checked only if it is
   //		currently open.
   mReportAnalysisSummary =
      docptr->mReportWDMap[ID_REPORTS_ANALYSIS_SUMMARY].mIsOpen;

   mReportAnthropometry =
      docptr->mReportWDMap[ID_REPORTS_ANTHROPOMETRY].mIsOpen;

   mReportBalance = 
	   docptr->mReportWDMap[ID_REPORTS_BALANCE].mIsOpen;

   //if(!(docptr->getPositioning() & 1))
   /*if(docptr->getPosition() == P_Standing)
        mReportBalanceStanding =
            docptr->mReportWDMap[ID_REPORTS_BALANCE_STANDING].mIsOpen;
   else
        mReportBalanceSeated =
            docptr->mReportWDMap[ID_REPORTS_BALANCE_SEATED].mIsOpen;*/

   mReportFatigue =
      docptr->mReportWDMap[ID_REPORTS_FATIGUE].mIsOpen;

   mReportPosture =
      docptr->mReportWDMap[ID_REPORTS_POSTURE].mIsOpen;

   mReportJointLocations =
      docptr->mReportWDMap[ID_REPORTS_JOINT_LOCATIONS].mIsOpen;

   mReportCoMLocations =
      docptr->mReportWDMap[ID_REPORTS_CENTEROFMASSLOCATIONS].mIsOpen;

   mReportJointMoments =
      docptr->mReportWDMap[ID_REPORTS_JOINT_MOMENTS].mIsOpen;

   mReportJointForces =
      docptr->mReportWDMap[ID_REPORTS_JOINT_FORCES].mIsOpen;

   mReportLowBack =
      docptr->mReportWDMap[ID_REPORTS_3D_LOWBACK].mIsOpen;

   mReportShoulderAnalysis =
      docptr->mReportWDMap[ID_REPORTS_SHOULDERANALYSIS].mIsOpen;

   mReportShoulderGeometry =
      docptr->mReportWDMap[ID_REPORTS_SHOULDERGEOMETRY].mIsOpen;      

   mReportSagittal =
      docptr->mReportWDMap[ID_REPORTS_SAGITTAL].mIsOpen;

   mReportSpinal =
      //docptr->mReportWDMap[ID_REPORTS_SPINAL].mIsOpen;

   mReportStrength =
      docptr->mReportWDMap[ID_REPORTS_STRENGTH].mIsOpen;

   mReportStrengthDirVectors =
      docptr->mReportWDMap[ID_REPORTS_STRENGTH_DIR_VECTORS].mIsOpen;

   mReportTaskInputSummary =
      docptr->mReportWDMap[ID_REPORTS_TASK_SUMMARY].mIsOpen;

   this->UpdateData( FALSE );
}
