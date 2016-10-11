// main_frm.h : interface of the C_Main_Frame class
//
/////////////////////////////////////////////////////////////////////////////
// ACT - (WM_USER + 1) really fux0rs up the CProgressCtrl bars in ReportAnalysisSummary
// when the message hits the object.
// Lets move into the APPLICATION message space to avoid whatever problem
// the CProgressCtrl has with those messages in the WM_USER space
#ifndef MAIN_FRM_H
#define MAIN_FRM_H

#define   WM_UPDATE_POSITION  ( WM_APP + 1 ) //User defined message
#define   WM_UPDATE_TITLE_BAR ( WM_APP + 2 ) //User defined message
#define   WM_DRAWOPENGL ( WM_APP + 3) // User defined message: draw opengl content to the supplied device and rendering context

class   CDibView;
class   ReportView;
class DIB;
class CDummyView;

#include "stdafx.h"
#include <afxwin.h>	// For CMDIFrameWnd
#include <afxext.h>	// For CStatusBar
#include <string>

class C_Main_Frame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(C_Main_Frame)

public:
	C_Main_Frame();

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~C_Main_Frame();
	virtual BOOL PreCreateWindow( CREATESTRUCT&   CS_Ref );

   CDibView* Create_Dib_Window( void );
        
   void UpdateMainTitleBar( void );

   void OnPrint(CDC* pDC, CPrintInfo* pInfo);

private:
   void Refresh_Corrupted_Window( CWnd* IconWindowPtr );

   DIB* printDib;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
   BOOL        mStatusBarExists;
	
   CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(C_Main_Frame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnReportsJointLocations();
	afx_msg void OnReportsCoMLocations();
	afx_msg void OnReports3dLowBack();
    afx_msg void OnReportsShoulderAnalysis();
    afx_msg void OnReportsShoulderGeometry();
	afx_msg void OnReportsAnthropometry();
	afx_msg void OnReportsSpinal();
	afx_msg void OnReportsSagittal();
	afx_msg void OnReportsStrength();
	afx_msg void OnReportsAnalysisSummary();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnReportsTaskSummary();
	afx_msg void OnFilePrintApplication();
	afx_msg void OnFilePrintDesktop();
	afx_msg void OnCopyApplication();
	afx_msg void OnCopyDesktop();
	afx_msg void OnUpdateReportsSummary(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReportsAnthropometry(CCmdUI* pCmdUI);
	afx_msg void OnReportsPosture();
	afx_msg void OnUpdateReportsTaskinputsummary(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReportsStrength(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReportsPosture(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReportsMoments(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReportsL5s1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReports3dback(CCmdUI* pCmdUI);
    afx_msg void OnUpdateReportsShoulderAnalysis(CCmdUI* pCmdUI);
    afx_msg void OnUpdateReportsShoulderGeometry(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReportsL23l44(CCmdUI* pCmdUI);
	afx_msg void OnReportsJointMoments();
	afx_msg void OnUpdateReportsJointMoments(CCmdUI* pCmdUI);
	afx_msg void OnReportsStrengthDirVectors();
	afx_msg void OnUpdateReportsStrengthDirVectors(CCmdUI* pCmdUI);
	afx_msg void OnReportsBalanceSeated();
	afx_msg void OnReportsBalanceStanding();
	afx_msg void OnUpdateReportsBalanceSeated(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReportsBalanceStanding(CCmdUI* pCmdUI);
	afx_msg void OnReportsJointForces();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnUpdateReportsJointforces(CCmdUI* pCmdUI);
	afx_msg void OnReportsFatigue();
	afx_msg void OnUpdateReportsFatigue(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP() 
	
	//--- Support ---
	void  Create_Report_Screen( void );

public:        
	void  Update_All_Title_Bars( void );
	void  Update_Title_Bar( void );
	void  TileChildren( void );
	int   GetClientHeight( void );
	int   GetClientWidth( void );
	int   CreateStatusBar( void );
	BOOL  DestroyStatusBar( void );
        void  StatusBarText( const std::string& aNewText );
	void  SynchronizeStatusBar( void );

public:
	void C_Main_Frame::PrintReports( BOOL aReportAnalysisSummary,
            BOOL aReportAnthropometry,
            //BOOL aReportBalanceStanding,
                //BOOL aReportBalanceSeated,
            BOOL aReportBalance,
            BOOL aReportFatigue,
            BOOL aReportPosture,
            BOOL aReportJointLocations,
			BOOL aReportCoMLocations,
            BOOL aReportJointMoments,
            BOOL aReportJointForces,
            BOOL aReportLowBack,
            BOOL aReportShoulderAnalysis,
            BOOL aReportShoulderGeometry,
            BOOL aReportSagittal,
//BOOL aReportSpinal,
            BOOL aReportStrength,
            BOOL aReportStrengthDirVectors,
            BOOL aReportTaskInputSummary );

	CFrameWnd* OpenReport(int aReportType);

	void PrintReportOnDemand( void( C_Main_Frame::*aCreateReportFctPtr )(),
									  BOOL( *aIsOpenFctPtr )(),
									  ReportView*( *aPointerFctPtr )() );

	CMDIChildWnd* Frame_Ptr;
        afx_msg void OnRefreshAll();
        virtual BOOL PreTranslateMessage(MSG* pMsg);
		afx_msg void OnReportsBalance();
		afx_msg void OnUpdateReportsBalance(CCmdUI *pCmdUI);

		CDummyView* dummyView;
		afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

/////////////////////////////////////////////////////////////////////////////

#endif
