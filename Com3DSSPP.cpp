// Com3DSSPP.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
//#include "hom_doc.h"	// Unnecessary!
#include "Com3DSSPP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Com3DSSPP

IMPLEMENT_DYNCREATE(Com3DSSPP, CCmdTarget)

Com3DSSPP::Com3DSSPP()
{
	EnableAutomation();
}

Com3DSSPP::~Com3DSSPP()
{
}


void Com3DSSPP::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(Com3DSSPP, CCmdTarget)
	//{{AFX_MSG_MAP(Com3DSSPP)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(Com3DSSPP, CCmdTarget)
	//{{AFX_DISPATCH_MAP(Com3DSSPP)
	DISP_FUNCTION(Com3DSSPP, "Hello", Hello, VT_BSTR, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_Iom3DSSPP to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {5DCEDD68-03DC-11D2-948D-000000000000}
static const IID IID_ICom3DSSPP =
{ 0x5dcedd68, 0x3dc, 0x11d2, { 0x94, 0x8d, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } };

BEGIN_INTERFACE_MAP(Com3DSSPP, CCmdTarget)
	INTERFACE_PART(Com3DSSPP, IID_ICom3DSSPP, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Com3DSSPP message handlers

BSTR Com3DSSPP::Hello() 
{
	CString strResult( "Hello there." );
	return strResult.AllocSysString();
}
