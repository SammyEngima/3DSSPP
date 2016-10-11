#pragma once
#include "afxwin.h"
#include "JointAngles.hpp"
#include "Skeleton.hpp"
class C_Hom_Doc;

// Dg_BodyAnglesDyn dialog

class Dg_BodyAnglesDyn : public CDialog
{
	DECLARE_DYNAMIC(Dg_BodyAnglesDyn)

public:
	Dg_BodyAnglesDyn(C_Hom_Doc* aDocPtr, CWnd* pParent = NULL);   // standard constructor
	virtual ~Dg_BodyAnglesDyn();

// Dialog Data
	enum { IDD = IDD_BODY_ANGLES_DYN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void GetCurrentJointAnglesFromDocument();
	void SetJointAngles(const JointAngles &aJointAnglesRef, const double angles[NUM_EXTRA_ANGLES], int flag);

	BOOL OnInitDialog();

public:
	void BuildJointAngles(JointAngles& ja, double angles[NUM_EXTRA_ANGLES]);
	bool SameAsDocument();
	bool UpdateDocument();
	
	void Increment(bool isIncrementing);

	void UndoButtonStatus();
    void RedoButtonStatus();

	DECLARE_MESSAGE_MAP()
private:
	int			Current_Control_ID;
	C_Hom_Doc*	mDocPtr;
	BOOL		mValue_OK;
	CString		NeutralButtonCaption;
private:
	CEdit mLeftForearmHorizontalControl;
	CEdit mLeftForearmVerticalControl;
	CEdit mRightForearmHorizontalControl;
	CEdit mRightForearmVerticalControl;
	CEdit mLeftUpperarmHorizontalControl;
	CEdit mLeftUpperarmVerticalControl;
	CEdit mRightUpperarmHorizontalControl;
	CEdit mRightUpperarmVerticalControl;
	CEdit mLeftUpperLegHorizontalControl;
	CEdit mLeftUpperlegVerticalControl;
	CEdit mRightUpperlegHorizontalControl;
	CEdit mRightUpperlegVerticalControl;
	CEdit mLeftLowerlegVerticalControl;
	CEdit mRightLowerlegVerticalControl;
	CEdit mLeftFootVerticalControl;
	CEdit mRightFootVerticalControl;
	CEdit mLeftClavicleHorizontalControl;
	CEdit mLeftClavicleVerticalControl;
	CEdit mRightClavicleHorizontalControl;
	CEdit mRightClavicleVerticalControl;
	CEdit mTrunkFlexionControl;
	CEdit mTrunkAxialRotationControl;
	CEdit mTrunkLateralBendingControl;
	CEdit mPelvicLateralTiltControl;
	CEdit mHeadFlexionControl;
	CEdit mHeadAxialRotationControl;
	CEdit mHeadLateralBendingControl;
	CEdit mUpperNeckFlexionControl; // upper neck
	CEdit mUpperNeckAxialRotationControl;
	CEdit mUpperNeckLateralBendingControl;
	CEdit mLowerNeckFlexionControl; // lower neck
	CEdit mLowerNeckAxialRotationControl;
	CEdit mLowerNeckLateralBendingControl;
public:
	int Forearm_LH;
	int Forearm_LV;
	int Forearm_LR;
	int Forearm_RH;
	int Forearm_RV;
	int Forearm_RR;
	int Upperarm_LH;
	int Upperarm_LV;
	int Upperarm_LR;
	int Upperarm_RH;
	int Upperarm_RV;
	int Upperarm_RR;
	int Upperleg_LH;
	int Upperleg_LV;
	int Upperleg_LR;
	int Upperleg_RH;
	int Upperleg_RV;
	int Upperleg_RR;
	int Lowerleg_LV;
	int Lowerleg_RV;
	int Foot_LV;
	int Foot_RV;
	int Trunk_Flexion;
	int Trunk_Axial_Rotation;
	int Trunk_Lateral_Bending;
	int Head_Flexion;
	int Head_Axial_Rotation;
	int Head_Lateral_Bending;
	int PelvicLateralTilt;
	int PelvicAxialRotation;
	int NasionFlexion;
	double HeadFlexion;
	int HeadAxialRotation;
	int HeadLateralBending;
	double UpperNeckFlexion;
	double UpperNeckAxialRotation;
	double UpperNeckLateralBending;
	double LowerNeckFlexion;
	double LowerNeckAxialRotation;
	double LowerNeckLateralBending;
	int Clavicle_LH;
	int Clavicle_LV;
	int Clavicle_RH;
	int Clavicle_RV;
	int Increment_Group;
	int Customizedneck;
	// differenc for maintaing head posture(for upper and lower neck repectively)
	double diff0; double diff1;double diff2; double diff3;double diff4; double diff5;double diff6;double diff7;double diff8;double diff9;
	double neckbase1; double neckbase2; double neckbase3;
public:
	afx_msg void OnEnSetfocusForearmLhDyn();
	afx_msg void OnEnSetfocusForearmLvDyn();
	afx_msg void OnEnSetfocusForearmRhDyn();
	afx_msg void OnEnSetfocusForearmRvDyn();
	afx_msg void OnEnSetfocusUpperarmLhDyn();
	afx_msg void OnEnSetfocusUpperarmLvDyn();
	afx_msg void OnEnSetfocusUpperarmRhDyn();
	afx_msg void OnEnSetfocusUpperarmRvDyn();
	afx_msg void OnEnSetfocusUpperlegLhDyn();
	afx_msg void OnEnSetfocusUpperlegLvDyn();
	afx_msg void OnEnSetfocusUpperlegRhDyn();
	afx_msg void OnEnSetfocusUpperlegRvDyn();
	afx_msg void OnEnSetfocusLowerlegLvDyn();
	afx_msg void OnEnSetfocusLowerlegRvDyn();
	afx_msg void OnEnSetfocusFootLvDyn();
	afx_msg void OnEnSetfocusFootRvDyn();
	afx_msg void OnEnSetfocusTrunkFlexionDyn();
	afx_msg void OnEnSetfocusTrunkAxialRotationDyn();
	afx_msg void OnEnSetfocusTrunkLateralBendingDyn();
	afx_msg void OnEnSetfocusPelvicLateralTiltDyn();
	afx_msg void OnEnSetfocusHeadFlexionDyn();
	afx_msg void OnEnSetfocusHeadRotDyn();
	afx_msg void OnEnSetfocusHeadLateralDyn();
	afx_msg void OnEnSetfocusNeck1FlexionDyn();
	afx_msg void OnEnSetfocusNeck1RotDyn();
	afx_msg void OnEnSetfocusNeck1LateralDyn();
	afx_msg void OnEnSetfocusNeck2FlexionDyn();
	afx_msg void OnEnSetfocusNeck2RotDyn();
	afx_msg void OnEnSetfocusNeck2LateralDyn();
	afx_msg void OnEnSetfocusClavicleLhDyn();
	afx_msg void OnEnSetfocusClavicleLvDyn();
	afx_msg void OnEnSetfocusClavicleRhDyn();
	afx_msg void OnEnSetfocusClavicleRvDyn();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRedraw();
	afx_msg void OnBnClickedNeutralbutton();
	afx_msg void OnBnClickedUndobutton();
	afx_msg void OnBnClickedRedobutton();
	afx_msg void OnBnClickedIncrementPlusDyn();
	afx_msg void OnBnClickedIncrementMinusDyn();
	afx_msg void OnBnClickedSymmetryRToLDyn();
	afx_msg void OnBnClickedSymmetryLToRDyn();
	CButton mUndoButtonControl;
	CButton mRedoButtonControl;
private:
	CEdit mLeftLowerlegHorizontalControl;
	CEdit mRightLowerlegHorizontalControl;
	CEdit mLeftFootHorizontalControl;
	CEdit mRightFootHorizontalControl;
public:
	afx_msg void OnEnSetfocusLowerlegLhDyn();
	afx_msg void OnEnSetfocusLowerlegRhDyn();
	afx_msg void OnEnSetfocusFootLhDyn();
	afx_msg void OnEnSetfocusFootRhDyn();
private:
	int Lowerleg_LH;
	int Lowerleg_RH;
	int Foot_LH;
	int Foot_RH;
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedGenderfemale();
	afx_msg void OnEnSetfocusPelvicAxialRotationDyn();
	afx_msg void OnBnClickedHandangles();
	BOOL mMaintainHandPosture;
	afx_msg void OnBnClickedMaintainhandposture();
	afx_msg void OnPrintPrintwindow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	BOOL mMaintainHeadPosture;
	afx_msg void OnBnClickedMaintainheadposture();
	afx_msg void OnBnClickedButton1();
	afx_msg void HeadGlobalControl();
	bool UpdateAngles(void);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
