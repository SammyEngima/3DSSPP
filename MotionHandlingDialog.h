#pragma once

#include "resource.h"
#include "afxwin.h"
#include "ViewComboBox.h"
#include "MotionFrameDisplay.h"
#include "GLTopSideFront.h"
#include "GLOblique.h"
#include "MotionDialogTab.h"
#include "FloatEdit.h"
#include "IntEdit.h"
#include "FloatRangeEdit.h"
#include "IntRangeEdit.h"

class GLInteractive;
// MotionHandlingDialog dialog

class MotionHandlingDialog : public CMotionDialogTab
{
	DECLARE_DYNAMIC(MotionHandlingDialog)

public:
	MotionHandlingDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~MotionHandlingDialog();

// Dialog Data
	enum { IDD = IDD_MOTION_HANDLING };

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	// take values from member variables and apply them to the Motion
	bool membersToMotion();
	// take values from the Motion and apply them to member variables
	void motionToMembers();
	// do work to apply changes that the user makes to the motion
	void motionChanged();

	bool unattainable;
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedReachtypeTwohanded();
	afx_msg void OnBnClickedReachtypeOnehandedneutral();
	afx_msg void OnBnClickedReachtypeOnehandedprone();
	afx_msg void OnBnClickedReachtypeWithobject();
	afx_msg void OnBnClickedReachtypeWithoutobject();
	afx_msg void OnEnKillfocusHanddistx();
	afx_msg void OnEnKillfocusHanddisty();
	afx_msg void OnEnKillfocusHanddistz();
	afx_msg void OnBnClickedInitialLocation();
	afx_msg void OnBnClickedInitialSelected();
	afx_msg void OnEnKillfocusInitialLocationX();
	afx_msg void OnEnKillfocusInitialLocationY();
	afx_msg void OnEnKillfocusInitialLocationZ();
	afx_msg void OnBnClickedFinalLocation();
	afx_msg void OnBnClickedFinalSelected();
	afx_msg void OnEnKillfocusFinalLocationX();
	afx_msg void OnEnKillfocusFinalLocationY();
	afx_msg void OnEnKillfocusFinalLocationZ();
	afx_msg void OnBnClickedMotionlengthPredict();
	afx_msg void OnBnClickedMotionlengthSpecify();
	afx_msg void OnEnKillfocusSpecifySeconds();
	afx_msg void OnEnKillfocusSpecifyFrames();

	BOOL ReachTypeHanded;
	BOOL ReachTypeObject;
	CString HandDistX;
	CString HandDistY;
	CString HandDistZ;
	BOOL InitialPosture;
	CString InitialX;
	CString InitialY;
	CString InitialZ;
	BOOL FinalPosture;
	CString FinalX;
	CString FinalY;
	CString FinalZ;
	BOOL MotionLength;
	CString LengthSeconds;
	CString LengthFrames;

	CString OffsetLabel;
	CString InitialPostureLabel;
	CString FinalPostureLabel;

	CFloatEdit HandDistXEdit;
	CFloatEdit HandDistYEdit;
	CFloatEdit HandDistZEdit;
	CFloatEdit InitialXEdit;
	CFloatEdit InitialYEdit;
	CFloatEdit InitialZEdit;
	CFloatEdit FinalXEdit;
	CFloatEdit FinalYEdit;
	CFloatEdit FinalZEdit;
	CFloatRangeEdit LengthSecondsEdit;
	CIntRangeEdit LengthFramesEdit;
	CString BoxAngle;
	CFloatRangeEdit BoxAngleEdit;
	afx_msg void OnEnKillfocusHandlingboxangle();
	BOOL mFixShoulder;
	BOOL mFixHip;
	double mShoullenflex;
	afx_msg void OnBnClickedFixshoulder();
	afx_msg void OnBnClickedFixpelvis();
	afx_msg void OnEnKillfocusShoullenflex();
};
