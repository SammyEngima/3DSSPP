// Dg_Intro.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "Dg_Intro.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_Intro dialog


Dg_Intro::Dg_Intro(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_Intro::IDD, pParent)
{
	//{{AFX_DATA_INIT(Dg_Intro)
	Intro_Message = "";
	//}}AFX_DATA_INIT
}

void Dg_Intro::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_Intro)
	DDX_Text(pDX, IDC_INTRO_MSG, Intro_Message);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Dg_Intro, CDialog)
	//{{AFX_MSG_MAP(Dg_Intro)
	ON_BN_CLICKED(IDC_INTRO_MSG, OnIntroMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dg_Intro message handlers

void Dg_Intro::OnIntroMsg() 
{
	// TO DO: Add your control notification handler code here
	
}
