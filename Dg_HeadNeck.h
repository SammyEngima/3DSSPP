#pragma once
#include "resource.h"
#include "Dg_BodyAnglesDyn.h"
#include "JointAngles.hpp"

///Yijun
#include "afxwin.h"
// Dg_HeadNeck dialog

class Dg_HeadNeck : public CDialog
{
	DECLARE_DYNAMIC(Dg_HeadNeck)

public:
	Dg_HeadNeck(Dg_BodyAnglesDyn* bod,CWnd* pParent = NULL);   //  constructor
	virtual ~Dg_HeadNeck();
	afx_msg void OnEnSetfocusHeadFlexionDyn();
	afx_msg void OnEnSetfocusNeck1FlexionDyn();
	afx_msg void OnEnSetfocusNeck1RotDyn();
	afx_msg void OnEnSetfocusNeck1LateralDyn();
	afx_msg void OnEnSetfocusNeck2FlexionDyn();
	afx_msg void OnEnSetfocusNeck2RotDyn();
	afx_msg void OnEnSetfocusNeck2LateralDyn();
// Dialog Data
	enum { IDD = IDD_HeadNeck };
private:

	Dg_BodyAnglesDyn* body;

	CEdit mHeadFlexionControl;
	CEdit mUpperNeckFlexionControl; // upper neck
	CEdit mUpperNeckAxialRotationControl;
	CEdit mUpperNeckLateralBendingControl;
	CEdit mLowerNeckFlexionControl; // lower neck
	CEdit mLowerNeckAxialRotationControl;
	CEdit mLowerNeckLateralBendingControl;

	double HeadFlexion;
	int HeadAxialRotation;
	int HeadLateralBending;
	double UpperNeckFlexion;
	double UpperNeckAxialRotation;
	double UpperNeckLateralBending;
	double LowerNeckFlexion;
	double LowerNeckAxialRotation;
	double LowerNeckLateralBending;
	int			Current_Control_ID;
	C_Hom_Doc*	mDocPtr;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetJointAngles(const JointAngles &aJointAnglesRef);
	void BuildJointAngles(JointAngles& ja);
	bool SameAsDocument();
	bool UpdateDocument();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();

	///Yijun
	afx_msg void OnPrintPrintwindow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual BOOL OnInitDialog();
	void Increment(bool isIncrementing);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnEnSetfocusHeadangleDyn();
	afx_msg void OnEnSetfocusUpperFlexionDyn();
	afx_msg void OnEnSetfocusUpperRotationDyn();
	afx_msg void OnEnSetfocusUpperBendingDyn();
	afx_msg void OnEnSetfocusLowerFlexionDyn();
	afx_msg void OnEnSetfocusLowerRotationDyn();
	afx_msg void OnEnSetfocusLowerBendingDyn();
};
