#ifndef DG_SUPPORTSELECTION_H
#define DG_SUPPORTSELECTION_H

#pragma once

#include "resource.h"
#include "afxwin.h"
#include "FrameSequence.h"
#include "AnimationBar.h"

// Dg_SupportSelection dialog

class Dg_SupportSelection : public CDialog
{
	DECLARE_DYNAMIC(Dg_SupportSelection)

public:
	Dg_SupportSelection(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dg_SupportSelection();

// Dialog Data
	//{{AFX_DATA(Dg_SupportSelection)
	enum { IDD = IDD_SupportSelection };
	int		mStance;
	int		mStandingSeated;
	int     currentframe;
	int     leftselection;
	int     rightselection;
	bool    selection;
	static std::vector<Frame> frames;
	static void init() {
	for (int k = 0;k < 100;k++)
	frames.push_back(Frame());
    }
	CButton mNofoot;

	double	mPelvicTilt;
	//}}AFX_DATA

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	bool UpdateDocument();

	DECLARE_MESSAGE_MAP()
public:
	BOOL mSeatBack;
	BOOL mSeatFriction;
	BOOL mFrontSupport;
	double mBackRestHeight;
	afx_msg void OnBnClickedStance();
	afx_msg void OnRedraw();
	afx_msg void OnOk();
	afx_msg void OnSetDefault();
	afx_msg void OnBnClickedStanding();
	afx_msg void OnSeated();
	afx_msg BOOL OnInitDialog();
	CButton mCHasBackRest;
	CButton mCFrontSeatPan;
	CEdit mCSeatHeight;
	CButton mCDefaultButton;
	CString mBackRestUnits;
    CButton mLeftFoot;
    CButton mRightFoot;
    afx_msg void OnBnClickedNofoot();
    afx_msg void OnBnClickedBothfeet();
	afx_msg void OnPrintPrintwindow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
#endif