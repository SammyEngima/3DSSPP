// Dg_TaskAlreadyOpen.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
//#include "hom_doc.h"	// Unnecessary!
#include "Dg_TaskAlreadyOpen.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_TaskAlreadyOpen dialog


Dg_TaskAlreadyOpen::Dg_TaskAlreadyOpen(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_TaskAlreadyOpen::IDD, pParent)
{
	//{{AFX_DATA_INIT(Dg_TaskAlreadyOpen)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void Dg_TaskAlreadyOpen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_TaskAlreadyOpen)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Dg_TaskAlreadyOpen, CDialog)
	//{{AFX_MSG_MAP(Dg_TaskAlreadyOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dg_TaskAlreadyOpen message handlers

