// Dg_RegistrationInfo.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "Dg_RegistrationInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_RegistrationInfo dialog


Dg_RegistrationInfo::Dg_RegistrationInfo(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_RegistrationInfo::IDD, pParent)
{
   //{{AFX_DATA_INIT(Dg_RegistrationInfo)
   //}}AFX_DATA_INIT
}

// Disable this if the compiler doesn't like the DDX_control for the hyperlink
#define USE_HYPERLINK

void Dg_RegistrationInfo::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(Dg_RegistrationInfo)
#ifdef USE_HYPERLINK
   DDX_Control(pDX, IDC_3DSSPP_WEBPAGE, m_3DSSPPurl);
#else
   DDX_Control(pDX, IDC_3DSSPP_WEBPAGE, m_3DSSPPurl_noHyper);
#endif
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Dg_RegistrationInfo, CDialog)
   //{{AFX_MSG_MAP(Dg_RegistrationInfo)
   ON_WM_PAINT()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_RegistrationInfo message handlers

BOOL Dg_RegistrationInfo::OnInitDialog() 
{
   CDialog::OnInitDialog();

   // LCW - changed 10/12/04
   //m_3DSSPPurl.SetHyperlink("http://www.engin.umich.edu/dept/ioe/3DSSPP/");
   m_3DSSPPurl.SetHyperlink("http://www.umichergo.org");

   return TRUE;   // return TRUE unless you set the focus to a control
}
