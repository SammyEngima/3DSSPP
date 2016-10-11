#pragma once
#include "afxwin.h"

class Dg_HandSegments : public CDialog
{
public:
	Dg_HandSegments(C_Hom_Doc *aDocPtr,	CWnd *pParent = NULL);

private:
	enum{ IDD = IDD_HAND_SEGMENTS };

	int L1CA_V; int L1CA_H; int L1MC_V; int L1MC_H; int L1PP_V; int L1PP_H; int L1DP_V; int L1DP_H;
	int L2CM_V; int L2CM_H; int L2PP_V; int L2PP_H; int L2MP_V; int L2MP_H; int L2DP_V; int L2DP_H;
	int L3CM_V; int L3CM_H; int L3PP_V; int L3PP_H; int L3MP_V; int L3MP_H; int L3DP_V; int L3DP_H;
	int L4CM_V; int L4CM_H; int L4PP_V; int L4PP_H; int L4MP_V; int L4MP_H; int L4DP_V; int L4DP_H;
	int L5CM_V; int L5CM_H; int L5PP_V; int L5PP_H; int L5MP_V; int L5MP_H; int L5DP_V; int L5DP_H;

	int R1CA_V; int R1CA_H; int R1MC_V; int R1MC_H; int R1PP_V; int R1PP_H; int R1DP_V; int R1DP_H;
	int R2CM_V; int R2CM_H; int R2PP_V; int R2PP_H; int R2MP_V; int R2MP_H; int R2DP_V; int R2DP_H;
	int R3CM_V; int R3CM_H; int R3PP_V; int R3PP_H; int R3MP_V; int R3MP_H; int R3DP_V; int R3DP_H;
	int R4CM_V; int R4CM_H; int R4PP_V; int R4PP_H; int R4MP_V; int R4MP_H; int R4DP_V; int R4DP_H;
	int R5CM_V; int R5CM_H; int R5PP_V; int R5PP_H; int R5MP_V; int R5MP_H; int R5DP_V; int R5DP_H;

	C_Hom_Doc*	mDocPtr;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	void GetCurrentJointAnglesFromDocument();
	void SetJointAngles(const JointAngles &aJointAnglesRef);

	bool SameAsDocument();
	bool UpdateDocument();
	
    virtual void OnOK();
	afx_msg void OnApply();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPrintPrintwindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};