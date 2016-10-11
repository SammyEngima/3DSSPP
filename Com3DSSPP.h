#if !defined(AFX_COM3DSSPP_H__5DCEDD69_03DC_11D2_948D_000000000000__INCLUDED_)
#define AFX_COM3DSSPP_H__5DCEDD69_03DC_11D2_948D_000000000000__INCLUDED_

//[ uuid(5DCEDD6A-03DC-11D2-948D-000000000000) ]
DEFINE_GUID( CLSID_Com3DSSPP, 0x5DCEDD6A, 0x03DC, 0x11D2, 0x94, 0x8D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 );

//[ uuid(5DCEDD68-03DC-11D2-948D-000000000000) ]
DEFINE_GUID( IID_Com3DSSPP, 0x5DCEDD68, 0x03DC, 0x11D2, 0x94, 0x8D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 );

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Com3DSSPP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Com3DSSPP command target

class Com3DSSPP : public CCmdTarget
{
	DECLARE_DYNCREATE(Com3DSSPP)

	Com3DSSPP();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Com3DSSPP)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~Com3DSSPP();

	// Generated message map functions
	//{{AFX_MSG(Com3DSSPP)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()
   public:
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(Com3DSSPP)
	afx_msg BSTR Hello();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//class __declspec( uuid("5DCEDD6A-03DC-11D2-948D-000000000000") ) Com3DSSPP;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COM3DSSPP_H__5DCEDD69_03DC_11D2_948D_000000000000__INCLUDED_)
