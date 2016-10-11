// Dg_AC_Conversation.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
//#include "hom_doc.h"	// Unnecessary!
#include "Dg_AC_Conversation.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_AC_Conversation dialog


Dg_AC_Conversation::Dg_AC_Conversation(CWnd* pParent /*=NULL*/)
	: CDialog( Dg_AC_Conversation::IDD, pParent )
{
	//{{AFX_DATA_INIT(Dg_AC_Conversation)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
                                  //Create the window object, 
                                  //attaching it to the main
                                  //frame window
this->Create( Dg_AC_Conversation::IDD , AfxGetApp()->m_pMainWnd );
return;
}


void Dg_AC_Conversation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_AC_Conversation)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Dg_AC_Conversation, CDialog)
	//{{AFX_MSG_MAP(Dg_AC_Conversation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_AC_Conversation message handlers

void Dg_AC_Conversation::PostNcDestroy() //Modeless support
{
//@@@#if defined ( _DEBUG )
//this->AssertValid();
//#endif
delete   this;
return;
}

