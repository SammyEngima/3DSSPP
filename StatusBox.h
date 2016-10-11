#ifndef STATUS_BOX_H
#define STATUS_BOX_H

#include "stdafx.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ReportView.hpp"
#include "LimitScaleCtrl.h"
#include "resource.h"
#include "GLBalanceStanding.h"
#include "GLBalanceSitting.h"

class GLBasic;
enum BalanceStatus_e;
class StatusBox : public ReportView
{
	DECLARE_DYNCREATE(StatusBox)

    protected:
	    StatusBox();           // protected constructor used by dynamic creation
        virtual ~StatusBox();

    // Form Data
    public:
	    //{{AFX_DATA(StatusBox)
	    enum { IDD = IDD_StatusBox };
	    CString	mAnkle;
		CString mWrist;
	    CString	mElbow;
	    CString	mFriction;
	    CString	mHip;
	    CString	mKnee;
	    CString	mShoulder;
	    CString	mTorso;
	    CString	mConditions;
	    CString	mCGBalance;
	    CString	mUnitsLabel;
	    CString	mRightHori;
	    CString	mRightLat;
	    CString	mRightVert;
	    CString	mLeftHori;
	    CString	mLeftLat;
	    CString	mLeftVert;
	    CString	mGravityText;
	    CString	mGravityValue;
	    CString mHandForceTxtLVal;
		CString mHandForceTxtRVal;
	    CString mHandForceUnits;
	    CString	mAnkleValue;
	    CString	mElbowValue;
	    CString	mHipValue;
	    CString	mKneeValue;
	    CString	mShoulderValue;
	    CString	mTorsoValue;
	    CString	mL4L5DiscValue;
	    CString	mAnkleLimit;
	    CString	mElbowLimit;
	    CString	mHipLimit;
	    CString	mKneeLimit;
	    CString	mShoulderLimit;
	    CString	mTorsoLimit;

		
		CString WristLeft;
		CString WristRight;
		CString ElbowLeft;
		CString ElbowRight;
		CString ShoulderLeft;
		CString ShoulderRight;
		CString HipLeft;
		CString HipRight;
		CString KneeLeft;
		CString KneeRight;
		CString AnkleLeft;
		CString AnkleRight;
		CString Torso;

	    CLimitScaleCtrl	mL4L5DiscBar;
	    CLimitScaleCtrl	mTorsoBar;
	    CLimitScaleCtrl	mShoulderBar;
	    CLimitScaleCtrl	mKneeBar;
	    CLimitScaleCtrl	mHipBar;
	    CLimitScaleCtrl	mElbowBar;
		CLimitScaleCtrl mWristBar;
	    CLimitScaleCtrl	mAnkleBar;
	    //}}AFX_DATA

    // Attributes
    protected:

    // Operations
    public:

    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(StatusBox)
	    protected:
	    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	    //}}AFX_VIRTUAL

    //Custom overrides
	virtual void OnInitialUpdate();
    virtual void OnUpdate ( CView*    pSender, 
									    LPARAM    lHint, 
                            CObject*  pHint );

    // Implementation
    protected:
	    #ifdef _DEBUG
	        virtual void AssertValid() const;
	        virtual void Dump(CDumpContext& dc) const;
        #endif
        // Generated message map functions
	    //{{AFX_MSG(StatusBox)
	    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	    afx_msg void OnDisplayDialogcolor();
	    afx_msg void OnSize(UINT nType, int cx, int cy);
	    afx_msg void OnUpdateDisplayDialogcolor(CCmdUI* pCmdUI);
	    virtual void OnDraw(CDC* pDC);
	    //}}AFX_MSG
	    afx_msg void OnFilePrintPreview();

		afx_msg LRESULT OnDrawOpenGL(WPARAM wParam, LPARAM lParam);

//	    GLBalanceView COPgraphic;
//	    GLSeatCOP seatGraphic;
	    DECLARE_MESSAGE_MAP()
    private:
	    CBrush *mReportBrush;
	    CRect mCOPDrawingRect;	// To correct for scrolling inconsistencies
	    bool COPBoxCreated;
    public:
	    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	    CStatic COPBox;

		void UpdateAll();
		virtual void UpdateUnits();
		void UpdateGravity();
		void UpdatePercentileLimits();
		void UpdateOffsetCoordinates();

		GLBasic* glBasic;
		GLBalanceStanding glBalanceStanding;
		GLBalanceSitting glBalanceSitting;

    private:
    CString mFooter;
	BalanceStatus_e balanceStatus;

public:
	CString m3DLowBackCompressionUnits;
	CString shouldEditVal;
	afx_msg void OnEnChangeShouldedit();
	afx_msg void OnFileCopyactivewindow();
	afx_msg void OnStnClickedLeftwristflex12();
	CString m_WristDuration;
	CString m_ElbowDuration;
	CString m_ShoulderDuration;
	CString minWristDC;
	CString minElbowDC;
	CString minShoulderDC;
	double m_CycleTime;
	double m_Exertion;
	double m_ExertionDuration;
	CString m_ExertionDutyCycle;
	double EDC_Calc;
	int flag;
	afx_msg void OnBnClickedGenerate();
	afx_msg void OnBnClickedReset();
	afx_msg void OnEnChangeCycletime();
	afx_msg void OnEnChangeExertion();
	afx_msg void OnEnChangeEd();
	afx_msg void OnBnClickedGenerate1();
};

/////////////////////////////////////////////////////////////////////////////

#endif