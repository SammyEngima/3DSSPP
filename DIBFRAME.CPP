// dibframe.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "dibframe.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDIDibChildFrame

IMPLEMENT_DYNCREATE(CMDIDibChildFrame, CMDIChildWnd)

CMDIDibChildFrame::CMDIDibChildFrame()
{
}

CMDIDibChildFrame::~CMDIDibChildFrame()
{
}


BEGIN_MESSAGE_MAP(CMDIDibChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CMDIDibChildFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMDIDibChildFrame message handlers
/*----------------------------------------------------------------------*/
BOOL CMDIDibChildFrame::PreCreateWindow(CREATESTRUCT& CS)
{
   int OK = this->CMDIChildWnd::PreCreateWindow(CS);

   CS.style |= long(WS_MINIMIZE);  //Set window initially minimized
//   CS.style |= (long)WS_DISABLED;  //Set window initially disabled
//   CS.style &= ~(long)WS_VISIBLE;   //Set window initially invisible

   return   OK;
   }

