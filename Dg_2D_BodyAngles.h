// Dg_2D_BodyAngles.h : header file
//
#include "afxwin.h"
#if ! defined( Dg_2D_BodyAnglesMcr )
#define Dg_2D_BodyAnglesMcr

/////////////////////////////////////////////////////////////////////////////
// Dg_2D_BodyAngles dialog

class Dg_2D_BodyAngles : public CDialog
{
// Construction
public:
	Dg_2D_BodyAngles
      (
         C_Hom_Doc * aDocPtr ,
         CWnd*       pParent = NULL
      );	// standard constructor
	virtual ~Dg_2D_BodyAngles();

// Dialog Data
	//{{AFX_DATA(Dg_2D_BodyAngles)
	enum { IDD = IDD_2DJointAngles };
	CEdit	mUpperLegControl;
	CEdit	mUpperArmControl;
	CEdit	mTrunkFlexionControl;
	CEdit	mLowerLegControl;
	CEdit	mForeArmControl;
    CString NeutralButtonCaption;
	int		mIncrementGroup;
	int		mLowerLeg;
	int		mTrunkFlexion;
	int		mUpperLeg;
	int		mForeArm;
	int		mUpperArm;
	//}}AFX_DATA
        
   int             mCurrentControlID;
   C_Hom_Doc *     mDocPtr;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Dg_2D_BodyAngles)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   void GetCurrentJointAnglesFromDocument();

	void SetJointAngles( const JointAngles& aJointAnglesRef );

	bool SameAsDocument();
	bool UpdateDocument();

	void JogAngle(bool increasing);

	// Disable/enable the undo button when necessary
	void UndoButtonStatus();
    void RedoButtonStatus();

	// Generated message map functions
	//{{AFX_MSG(Dg_2D_BodyAngles)
	afx_msg void OnIncrementMinusButton();
	afx_msg void OnIncrementPlusButton();
	afx_msg void OnUndoButton();
    afx_msg void OnBnClickedRedoButton();
	afx_msg void OnRedrawButton();
	afx_msg void OnNeutralButton();
	afx_msg void OnSetfocusTrunkFlexion();
	afx_msg void OnSetfocusUpperArm();
	afx_msg void OnSetfocusUpperLeg();
	afx_msg void OnSetfocusLowerLeg();
	afx_msg void OnSetfocusForeArm();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CButton mUndoButtonControl;
    CButton mRedoButtonControl;
public:
	virtual BOOL OnInitDialog();
	BOOL ForearmBackward;
	BOOL UpperarmBackward;
	BOOL UpperlegBackward;
	BOOL LowerlegBackward;
	afx_msg void OnBnClickedForearmBackward2d();
	afx_msg void OnBnClickedUpperarmBackward2d();
	afx_msg void OnBnClickedUpperlegBackward2d();
	afx_msg void OnBnClickedLowerlegBackward2d();
	afx_msg void OnBnSetfocusForearmBackward2d();
	afx_msg void OnBnHotItemChangeForearmBackward2d(NMHDR *pNMHDR, LRESULT *pResult);
};

#endif
