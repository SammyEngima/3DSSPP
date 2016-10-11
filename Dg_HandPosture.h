#pragma once
#include "afxwin.h"
#include "JointAngles.hpp"

// Dg_HandPosture dialog
class C_Hom_Doc;

class Dg_HandPosture : public CDialog
{
	DECLARE_DYNAMIC(Dg_HandPosture)

public:
	Dg_HandPosture(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dg_HandPosture();

// Dialog Data
	enum { IDD = IDD_HAND_POSTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	
	void UndoButtonStatus();
    void RedoButtonStatus();

	void GetHandPostureFromDocument();
	void SetJointAngles(const JointAngles&);

	BOOL OnInitDialog();
	bool UpdateDocument();

	DECLARE_MESSAGE_MAP()
public:
	int HandLH;
	int HandRH;
	int HandLV;
	int HandLRot;
	int HandRV;
	int HandRRot;
	int* CurrentAngle; // points to a variable above whose text box has focus
	C_Hom_Doc* mDocPtr;
	BOOL LHandType;
	afx_msg void OnBnClickedHandPostureApply();
	afx_msg void OnBnClickedOk();
	BOOL RHandType;
	afx_msg void OnEnSetfocusHandLh();
	afx_msg void OnEnSetfocusHandLv();
	afx_msg void OnEnSetfocusHandLrot();
	afx_msg void OnEnSetfocusHandRh();
	afx_msg void OnEnSetfocusHandRv();
	BOOL Increment_Group;
private:
	int Increment_Value[6];
	void Increment(int sign);
	private:

	CButton mUndoButtonControl;
    CButton mRedoButtonControl;
public:
	afx_msg void OnBnClickedIncrementPlusHand();
	afx_msg void OnBnClickedIncrementMinusHand();
	afx_msg void OnEnSetfocusHandRrot();
	afx_msg void OnBnClickedSymmetryRToLHand();
	afx_msg void OnBnClickedSymmetryLToRHand();
	afx_msg void OnBnClickedHandneutral();

	afx_msg void OnUndoButton();
	afx_msg void OnBnClickedRedobutton();


	//BOOL mHandPostureLockCheckbox;

	afx_msg void OnBnClickedHandposturelock();
	CEdit mLeftHorz;
	CEdit mLeftVert;
	CEdit mLeftRot;
	CEdit mRightHorz;
	CEdit mRightVert;
	CEdit mRightRot;
	afx_msg void OnPrintPrintwindow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
