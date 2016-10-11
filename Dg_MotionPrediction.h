#pragma once

#include "stdafx.h"
#include "afxcmn.h"
#include "MotionTabCtrl.h"
#include "afxwin.h"
#include "Hom_Doc.h"

class Dg_MotionPrediction : public CDialog
{
	DECLARE_DYNAMIC(Dg_MotionPrediction)

    public:
	    Dg_MotionPrediction(int initialFrame, CWnd* pParent = NULL);
	    virtual ~Dg_MotionPrediction();

	    enum { IDD = IDD_MOTIONPRED };

    protected:
	    virtual void DoDataExchange(CDataExchange* pDX);
        virtual void OnOK();
        virtual BOOL OnInitDialog();
		virtual void OnCancel();

	    DECLARE_MESSAGE_MAP()

public:
	CMotionTabCtrl* mTabs;

public:
	afx_msg void OnBnClickedHiddenupdate();
	CButton mOKButton;
	CButton mCancelButton;

private:
	OffsetChoice savedOffsetChoice;
	JointID savedOffsetJoint;

};
