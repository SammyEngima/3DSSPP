#ifndef DG_FACTORSFEET
#define DG_FACTORSFEET

#pragma once

// Dg_FactorsFeet dialog

class Dg_FactorsFeet : public CDialog
{
   DECLARE_DYNAMIC(Dg_FactorsFeet)

public:
   Dg_FactorsFeet(CWnd* pParent = NULL);   // standard constructor
   virtual ~Dg_FactorsFeet();

   void SetFactors(const Factors& in_Factors) {
      mFactors = in_Factors;
   }

   Factors GetFactors() const {
      return mFactors;
   }

// Dialog Data
   enum { IDD = IDD_FACTORS_FEET };


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
