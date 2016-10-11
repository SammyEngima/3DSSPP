// Dg_BatchFile.cpp : implementation file
//

#include "stdafx.h"
#include "assert.h"
#include "hom.h"
//#include "hom_doc.h"	// Unnecessary!
#include "Dg_BatchFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_BatchFile dialog


Dg_BatchFile::Dg_BatchFile(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_BatchFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(Dg_BatchFile)
	mCommandNumber = 0;
	//}}AFX_DATA_INIT
}


void Dg_BatchFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_BatchFile)
	DDX_Text(pDX, IDC_CommandNumber, mCommandNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Dg_BatchFile, CDialog)
	//{{AFX_MSG_MAP(Dg_BatchFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_BatchFile message handlers

//---------------------------------------------------------------------------
   void
   Dg_BatchFile::
Update
( int   aCommandNumber )
{
   assert( aCommandNumber>0 );
   mCommandNumber = aCommandNumber;
   this->UpdateData( FALSE );
   this->UpdateWindow();
   return;
}

