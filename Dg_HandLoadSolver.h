#pragma once
#include "resource.h"
#include "Dg_HandLoads.h"
#include "JointAngles.hpp"
#include "afxwin.h"


class Dg_HandLoadSolver : public CDialog
{
	DECLARE_DYNAMIC(Dg_HandLoadSolver)

public:
	Dg_HandLoadSolver(Dg_HandLoads* bod,CWnd* pParent = NULL);   //  constructor
	virtual ~Dg_HandLoadSolver();
	bool islimit(void);

	int Increment_Solver_Group;
	int Solver_Condition_Group;
	int Current_Control_ID;

	double mLeftHorizontal;
	double mLeftVertical;
	double mRightHorizontal;
	double mRightVertical;
	double mdLeftHorizontal;
	double mdLeftVertical;
	double mdRightHorizontal;
	double mdRightVertical;

	CEdit mBothHandsControl;

	BOOL m_BackCompression;
	BOOL m_Balance;
	BOOL m_Strength;

	CString	mLeftMagUnits;
	CString	mRightMagUnits;

// Dialog Data
	enum { IDD = IDD_HandLoadSolver };
private:

	Dg_HandLoads* body;

	C_Hom_Doc*	mDocPtr;

	void HandForceKillFocus(int box_ID);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void UpdateDocument(void);
	bool VerifyTotalsInRange(bool giveMessages = true, bool constrainVars = false);
	afx_msg void OnRedrawButton();
	afx_msg void OnKillfocusLeftVertical();
	afx_msg void OnKillfocusLeftHorizontal();
	afx_msg void OnKillfocusRightVertical();
	afx_msg void OnKillfocusRightHorizontal();
	afx_msg void OnKillfocusRightMag();
	afx_msg void OnKillfocusLeftMag();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStrength();
	afx_msg void OnClickedIncmax();
	afx_msg void OnBnClickedSelectAll();
	afx_msg void OnBnClickedNone();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPrintPrintwindow();
	afx_msg void OnBnClickedOk();

	void IncreaseLeft(void);
	void IncreaseRight(void);
	void IncreaseBoth(void);
	void finalupdate();
	int CompressionLimit(void);
	int StrengthLimit(void);
	bool isbalance(void);// check balance status. return false only when it comes out from balanced status

	//virtual void OnOK();
	virtual BOOL OnInitDialog();

	CButton wristcontrol;
	CButton elbowcontrol;
	CButton shouldercontrol;
	CButton torsocontrol;
	CButton hipcontrol;
	CButton kneecontrol;
	CButton ankelcontrol;
	CButton m_incmax;

	BOOL m_wrist;
	BOOL m_elbow;
	BOOL m_shoulder;
	BOOL m_torso;
	BOOL m_hip;
	BOOL m_knee; 
	BOOL m_ankle;
	bool dichotomyflag; // true if dichotomy algorithm entered

	CString mdLeftMag;
	CString mdRightMag;
	CString leftforce;
	CString rightvalue;
	void GetCurrentForcesFromDocument();

};
