// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently

#if !defined(AFX_STDAFX_H__B0FAC2F6_929A_11D1_B107_00A0C9229122__INCLUDED_)
#define      AFX_STDAFX_H__B0FAC2F6_929A_11D1_B107_00A0C9229122__INCLUDED_

#ifndef WINVER
#define WINVER 0x0502	// To make WIN95 Compatible
#endif

#include <stdlib.h>
#include <crtdbg.h>
/*
Windows Server 2003 family _WIN32_WINNT>=0x0502
WINVER>=0x0502
 
Windows XP _WIN32_WINNT>=0x0501
WINVER>=0x0501
 
Windows 2000 _WIN32_WINNT>=0x0500
WINVER>=0x0500
 
Windows NT 4.0 _WIN32_WINNT>=0x0400
WINVER>=0x0400
 
Windows Me _WIN32_WINDOWS=0x0500
WINVER>=0x0500
 
Windows 98 _WIN32_WINDOWS>=0x0410
WINVER>=0x0410
 
Windows 95 _WIN32_WINDOWS>=0x0400
WINVER>=0x0400
*/ 
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
// ACT - Make truncation warnings lint level (lev 4)
#ifndef SHOW_WARNINGS
#pragma warning(4 : 4244)
#endif

#include <afxwin.h>     // MFC core and standard components
#include <afxext.h>     // MFC extensions
#include <afxdisp.h>		// MFC OLE automation classes
#include <afxpriv.h>		// Print Preview enabling	
//#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdhtml.h>
#include <afxdisp.h>
#include <afxdlgs.h>
#include <afxcontrolbars.h>
#ifdef _3DSSPP_DEBUG
#pragma optimize("", off)
#endif
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B0FAC2F6_929A_11D1_B107_00A0C9229122__INCLUDED_)
