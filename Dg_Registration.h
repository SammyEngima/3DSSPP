#if !defined(AFX_DG_REGISTRATION_H__ACEA0822_613A_4FE6_B7A1_5427A91C340C__INCLUDED_)
#define AFX_DG_REGISTRATION_H__ACEA0822_613A_4FE6_B7A1_5427A91C340C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dg_Registration.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_Registration dialog

class Dg_Registration : public CDialog
{
// Construction
public:
   Dg_Registration(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(Dg_Registration)
   enum { IDD = IDD_REGISTRATION };
   CString mCompanyName;
   CString mRegistrationCode;
   CString mSavedRegistrationCode;
   bool mLicenseAlreadyValid;
   CString mRegCode1;
   CString mRegCode2;
   CString mRegCode3;
   CString mRegCode4;
   CButton  mDemoModeCtrl;
   //}}AFX_DATA

// Overrides
protected:
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(Dg_Registration)
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   void UpdateRegCode(CString& regString);

   // Generated message map functions
   //{{AFX_MSG(Dg_Registration)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   virtual void OnCancel();
   afx_msg void OnKillfocusLicense1();
   afx_msg void OnKillfocusLicense2();
   afx_msg void OnKillfocusLicense3();
   afx_msg void OnKillfocusLicense4();
   afx_msg void OnPaint();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnEnChangeLicenseNumber1();
   afx_msg void OnEnChangeLicenseNumber2();
   afx_msg void OnEnChangeLicenseNumber3();
   afx_msg void OnEnSetfocusLicenseNumber1();
   afx_msg void OnEnSetfocusLicenseNumber2();
   afx_msg void OnEnSetfocusLicenseNumber3();
   afx_msg void OnEnSetfocusLicenseNumber4();
   
private:
   void ClearNewRegBox(CString& regString);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DG_REGISTRATION_H__ACEA0822_613A_4FE6_B7A1_5427A91C340C__INCLUDED_)
