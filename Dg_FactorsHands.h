#ifndef  DG_FACTORSHANDS
#define  DG_FACTORSHANDS

#pragma once

// Dg_FactorsHands dialog
#include "Factors.hpp"

class Dg_FactorsHands : public CDialog
{
	DECLARE_DYNAMIC(Dg_FactorsHands)

public:
	Dg_FactorsHands(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dg_FactorsHands();

	void SetFactors(const Factors& in_Factors) {
		mFactors = in_Factors;
	}

	Factors GetFactors() const {
		return mFactors;
	}

// Dialog Data
	enum { IDD = IDD_FACTORS_HANDS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	Factors mFactors;

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnPrintPrintwindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};

#endif