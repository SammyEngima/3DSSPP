#if !defined(AFX_ADVANCEDSET_H__8C6D4500_93FC_11D4_8EBE_0001031CA2E0__INCLUDED_)
#define AFX_ADVANCEDSET_H__8C6D4500_93FC_11D4_8EBE_0001031CA2E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dg_Population.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_Population dialog
//#include "Dg_PopulationEdit.h"

class Dg_Population : public CDialog
{
// Construction
public:
   Dg_Population(CWnd* pParent = NULL);   // standard constructor

   CString   mPriorFilePathName;
   CString   mFilePathName;

   void SetGender(int genderCode) {
      mGender = genderCode;
   }
// Dialog Data
   //{{AFX_DATA(Dg_Population)
   enum { IDD = IDD_POPULATION};
	    // NOTE: the ClassWizard will add data members here
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(Dg_Population)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation

   void ReadFactorsFromFile();

private:
   int mGender;

protected:

	// Generated message map functions
	//{{AFX_MSG(Dg_Population)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDisplayDialogcolor();
	afx_msg void OnFdefault();
	afx_msg void OnFeditor();
	afx_msg void OnOpenFactors();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVANCEDSET_H__8C6D4500_93FC_11D4_8EBE_0001031CA2E0__INCLUDED_)
