#if !defined(AFX_CDGREGISTRATIONINFO_H__F662FE40_241B_4CA6_B3BF_F7625EC33717__INCLUDED_)
#define AFX_CDGREGISTRATIONINFO_H__F662FE40_241B_4CA6_B3BF_F7625EC33717__INCLUDED_
#include "HyperlinkStatic.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dg_RegistrationInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_RegistrationInfo dialog

class Dg_RegistrationInfo : public CDialog
{
// Construction
public:
   Dg_RegistrationInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(Dg_RegistrationInfo)
   enum { IDD = IDD_REGINFO };
   CHyperlinkStatic m_3DSSPPurl;
   CStatic m_3DSSPPurl_noHyper;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(Dg_RegistrationInfo)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(Dg_RegistrationInfo)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDGREGISTRATIONINFO_H__F662FE40_241B_4CA6_B3BF_F7625EC33717__INCLUDED_)
