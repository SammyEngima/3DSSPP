// Dg_PopulationEdit.h : header file
//

#if !defined(AFX_Dg_PopulationEdit_H__58111F11_E60B_11D0_92BB_000000000000__INCLUDED_)
#define AFX_Dg_PopulationEdit_H__58111F11_E60B_11D0_92BB_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// Dg_PopulationEdit dialog
#include "Dg_Anthropometry.h"
#include "hom_doc.h"
#include "afxwin.h"

class Dg_PopulationEdit : public CDialog
{
// Construction
public:
   Dg_PopulationEdit(C_Hom_Doc* aDocPtr, CWnd* pParent = NULL);   // standard constructor

   // Member factors (will be auto-deleted when object goes out of scope)
   Factors mFactors;
   Factors maleFactors;
   Factors femaleFactors;

   CString mPriorFilePathName;
   CString mFilePathName;

   void SaveFactorsToFile();
   void ReadFactorsFromFile();

   void SetMaleFactors(const Factors& aFactors) {
      maleFactors = aFactors;
   }
   void SetFemaleFactors(const Factors& aFactors) {
	  femaleFactors = aFactors;
   }

   void SetGender(int gender) {
      mGender = gender;
	  if(gender == 0)
	  {
		mFactors = maleFactors;
	  } else {
		mFactors = femaleFactors;
	  }
   }

   const Factors& GetFactors(const int& genderCode) {
	  if(genderCode == 0)
	  {
	    if(genderCode == this->mGender)
		{
			return mFactors;
		} else {
			return maleFactors;
		}
	  } else {
		if(genderCode == this->mGender)
		{
			return mFactors;
		} else {
		    return femaleFactors;
		}
	  }
   }

   // Dialog Data
   //{{AFX_DATA(Dg_PopulationEdit)
   enum { IDD = IDD_FACTORS };
   //}}AFX_DATA
     friend class Dg_Anthropometry;
private:
   int mGender;
   C_Hom_Doc* mDocPtr;
   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(Dg_PopulationEdit)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

   // Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(Dg_PopulationEdit)
   afx_msg void OnOpenButton();
   afx_msg void OnSaveAsButton();
   afx_msg void OnSaveButton();
   afx_msg void OnActualValueButton();
   afx_msg void OnPredictedValueButton();
   afx_msg void OnStdDevButton();
   afx_msg void OnDefault();
   afx_msg void OnHWmeans();
   afx_msg void OnGenderChange();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnRedraw();
   afx_msg void OnBnClickedFeetFactors();
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnBnClickedHandFactors();
   afx_msg void OnBnClickedRadio1();
   afx_msg void OnBnClickedRadio2();
   // careful, female is 0, male is 1, unlike convention everywhere else
   BOOL mGenderRadio;
   afx_msg void OnBnClickedEditromlimits();
   afx_msg void OnPrintPrintwindow();
   afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Dg_PopulationEdit_H__58111F11_E60B_11D0_92BB_000000000000__INCLUDED_)
