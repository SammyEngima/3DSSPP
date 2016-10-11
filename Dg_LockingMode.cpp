// Dg_LockingMode.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "Dg_LockingMode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_LockingMode dialog


Dg_LockingMode::Dg_LockingMode(CWnd* pParent /*=NULL*/)
: CDialog(Dg_LockingMode::IDD, pParent)
{
   //{{AFX_DATA_INIT(Dg_LockingMode)
   Locking_Mode = -1;
   //}}AFX_DATA_INIT
}

void Dg_LockingMode::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(Dg_LockingMode)
   DDX_Radio(pDX, IDC_NO_LOCKING, Locking_Mode);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Dg_LockingMode, CDialog)
   //{{AFX_MSG_MAP(Dg_LockingMode)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dg_LockingMode message handlers

