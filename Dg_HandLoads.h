#pragma once
#include "afxwin.h"
class C_Hom_Doc;
class String;

#define MAX_HAND_FORCE_LB 1000

class Dg_HandLoads : public CDialog
{
	//DECLARE_DYNAMIC(Dg_HandLoads)
    public:
	    Dg_HandLoads(C_Hom_Doc *aDocPtr,CWnd *pParent = NULL);	
		virtual ~Dg_HandLoads();

	    //{{AFX_DATA(Dg_HandLoads)
	    enum { IDD = IDD_HAND_LOADS };
	    CEdit mRightVerticalControl;
	    CEdit mLeftVerticalControl;
	    CEdit mRightHorizontalControl;
	    CEdit mLeftHorizontalControl;
		CEdit mBothHandsControl;

	    double mLeftHorizontal;
	    double mLeftVertical;
	    double mRightHorizontal;
	    double mRightVertical;

        /* double mdLeftMag;
	    double mdRightMag; */

	    double mdLeftHorizontal;
	    double mdLeftVertical;
	    double mdRightHorizontal;
	    double mdRightVertical;

        CString	mRightMagUnits;
	    CString	mLeftMagUnits;
	    CString	mLeftExertion;
	    CString	mRightExertion;

	    int mLeftEffortGroup;
	    int mRightEffortGroup;
	    int Increment_Forces_Group;
		int Increment_Solver_Group;
		int Solver_Condition_Group;
	    //}}AFX_DATA

		bool openAdvHandLoads;

    public:
        int Current_Control_ID;
        C_Hom_Doc *mDocPtr;


    private:
	    // Function to kill focus on the specified box;
	    //   used to force a check on that box's value
	    void HandForceKillFocus(int box_ID);
	    void DoIncrement(bool increment_up);

    protected:
	    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
		void GetCurrentForcesFromDocument();
        // Custom member functions
        void UpdateDocument(void);
        void UpdateLeftEffort(void);
        void UpdateRightEffort(void);        


		
		void UndoButtonStatus();
		void RedoButtonStatus();
        void CheckAndUpdate( void );
	    bool VerifyTotalsInRange(bool giveMessages = true, bool constrainVars = false);
        String ExertionText(double aV,double aH);

	    // Generated message map functions
	    //{{AFX_MSG(Dg_HandLoads)
	    afx_msg void OnRedrawButton();
	    virtual void OnOK();
	    afx_msg void OnSetfocusLeftForce();
	    afx_msg void OnSetfocusRightForce();
	    afx_msg void OnSetfocusLeftVerAngle();
	    afx_msg void OnSetfocusLeftHorAngle();
	    afx_msg void OnSetfocusRightVerAngle();
	    afx_msg void OnSetfocusRightHorAngle();
	    afx_msg void OnIncrementPlus();
	    afx_msg void OnIncrementMinus();
	    afx_msg void OnKillfocusLeftVertical();
	    afx_msg void OnKillfocusLeftHorizontal();
	    afx_msg void OnKillfocusRightVertical();
	    afx_msg void OnKillfocusRightHorizontal();
	    afx_msg void OnPaint();
	    afx_msg void OnLeftEffortGroup();
	    afx_msg void OnLeftExertLeft();
	    afx_msg void OnLeftExertRight();
	    afx_msg void OnLeftLift();
	    afx_msg void OnLeftLower();
	    afx_msg void OnLeftPullBack();
	    afx_msg void OnLeftPushForward();
	    afx_msg void OnRightEffortGroup();
	    afx_msg void OnRightExertLeft();
	    afx_msg void OnRightExertRight();
	    afx_msg void OnRightLift();
	    afx_msg void OnRightLower();
	    afx_msg void OnRightPullBack();
	    afx_msg void OnRightPushForward();
	    afx_msg void OnKillfocusRightMag();
	    afx_msg void OnKillfocusLeftMag();
	    afx_msg void OnEnChangeRightVertical();
        afx_msg void OnEnChangeLeftVertical();
		afx_msg void OnBnClickedHandLoadsAdv();
		//}}AFX_MSG
	    DECLARE_MESSAGE_MAP()
public:
	CString mdLeftMag;
	CString mdRightMag;
//	bool m_checkhand;
	afx_msg void OnBnClickedCheckhand();
//	bool m_hand;
	BOOL m_checkhand;
	CButton m_incmax;
	bool dichotomyflag; // true if dichotomy algorithm entered
	afx_msg void OnClickedIncmax();
	bool islimit(void);

	void IncreaseLeft(void);
	void IncreaseRight(void);
	void IncreaseBoth(void);

	int StrengthLimit(void);
	int CompressionLimit(void);
	virtual BOOL OnInitDialog();
	afx_msg void OnPrintPrintwindow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	BOOL m_BackCompression;
	BOOL m_Strength;
	BOOL m_Balance;
	afx_msg void OnBnClickedResetzero();
	BOOL m_wrist;
	BOOL m_elbow;
	BOOL m_shoulder;
	BOOL m_torso;
	BOOL m_hip;
	BOOL m_knee; 
	BOOL m_ankle;
	afx_msg void OnBnClickedSelectAll();
	afx_msg void OnBnClickedNone();
	//afx_msg void OnBnClickedStrength();
	CButton wristcontrol;
	CButton elbowcontrol;
	CButton shouldercontrol;
	CButton torsocontrol;
	CButton hipcontrol;
	CButton kneecontrol;
	CButton ankelcontrol;
	bool isbalance(void);// check balance status. return false only when it comes out from balanced status
	afx_msg void OnBnClickedHandLoadSolver();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedRedo();
	afx_msg void OnBnClickedUndo();
	CButton redocontrol;
	CButton undocontrol;
};

//#endif
