// Dg_BodyAngles.h : header file
//
#ifndef   C_Dia_Joint_Angles_Include
#define   C_Dia_Joint_Angles_Include
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// Dg_BodyAngles dialog

class Dg_BodyAngles : public CDialog
{
// Construction
public:
	Dg_BodyAngles(	C_Hom_Doc *aDocPtr,
								CWnd *pParent = NULL	);	// standard constructor
	virtual ~Dg_BodyAngles();

private:
// Dialog Data
	//{{AFX_DATA(Dg_BodyAngles)
	enum { IDD = IDD_BODY_ANGLES };
	/*CEdit	mRightVerticalUpperlegControl;
	CEdit	mLeftVerticalUpperlegControl;
	CEdit	mRightVerticalUpperarmControl;
	CEdit	mRightHorizontalUpperarmControl;
	CEdit	mLeftVerticalUpperarmControl;
	CEdit	mLeftHorizontalUpperarmControl;
	CEdit	mTrunkLateralBendingControl;
	CEdit	mTrunkAxialRotationControl;
	CEdit	mRightVerticalLowerlegControl;
	CEdit	mLeftVerticalLowerlegControl;
	CEdit	mRightVerticalForearmControl;
	CEdit	mRightHorizontalForearmControl;
	CEdit	mLeftVerticalForearmControl;
	CEdit	mLeftHorizontalForearmControl;
	CEdit	mTrunkFlexionControl;*/

    // So the caption on the neutral stance button changes with seated/standing mode
    CString NeutralButtonCaption;

	// LCW If any variables are added here, update the function
	//		SameAsDocument() to test for these new variables!
	int		Forearm_LH;
	int		Forearm_LV;
	int		Forearm_RH;
	int		Forearm_RV;
	int		Lowerleg_LV;
	int		Lowerleg_RV;
	int		Trunk_Axial_Rotation;
	int		Trunk_Flexion;
	int		Trunk_Lateral_Bending;
	int		Upperarm_LH;
	int		Upperarm_LV;
	int		Upperarm_RH;
	int		Upperarm_RV;
	int		Upperleg_LV;
	int		Upperleg_RV;
	int		Increment_Group;
	//}}AFX_DATA

	int			Current_Control_ID;
	C_Hom_Doc*	mDocPtr;
	BOOL			mValue_OK;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	void GetCurrentJointAnglesFromDocument();
	void SetJointAngles(const JointAngles &aJointAnglesRef);

	bool SameAsDocument();
	bool UpdateDocument();
	
	void Increment(bool isIncrementing);

	void UndoButtonStatus();
    void RedoButtonStatus();

	// Generated message map functions
	//{{AFX_MSG(Dg_BodyAngles)
	afx_msg void OnRedraw();
	afx_msg void OnSetfocusForearmLh();
	afx_msg void OnIncrementPlus();
	afx_msg void OnSetfocusForearmLv();
	afx_msg void OnSetfocusForearmRh();
	afx_msg void OnSetfocusForearmRv();
	afx_msg void OnSetfocusUpperarmLh();
	afx_msg void OnSetfocusUpperarmLv();
	afx_msg void OnSetfocusUpperarmRh();
	afx_msg void OnSetfocusUpperarmRv();
	afx_msg void OnSetfocusUpperlegLv();
	afx_msg void OnSetfocusUpperlegRv();
	afx_msg void OnSetfocusLowerlegLv();
	afx_msg void OnSetfocusLowerlegRv();
	afx_msg void OnSetfocusTrunkFlexion();
	afx_msg void OnSetfocusTrunkAxialRotation();
	afx_msg void OnSetfocusTrunkLateralBending();
	afx_msg void OnIncrementMinus();
	afx_msg void OnSymmetry_R_To_L();
	afx_msg void OnSymmetry_L_To_R();
	afx_msg void OnUndoButton();
	afx_msg void OnBnClickedRedobutton();
    virtual void OnOK();
	afx_msg void OnNeutralButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CButton mUndoButtonControl;
    CButton mRedoButtonControl;
public:
	int Upperleg_LH;
	int Upperleg_RH;
	int Lowerleg_LH;
	int Lowerleg_RH;
	afx_msg void OnEnSetfocusUpperlegLh();
	afx_msg void OnEnSetfocusLowerlegLh();
	afx_msg void OnEnSetfocusUpperlegRh();
	afx_msg void OnEnSetfocusLowerlegRh();
	afx_msg void OnPrintPrintwindow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};

#endif //End ifndef C_Dia_Joint_Angles_Include

