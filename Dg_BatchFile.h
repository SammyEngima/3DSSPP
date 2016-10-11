#if !defined(AFX_DGBATCHFILE_H__D6FF9B12_F0AC_11D1_947A_000000000000__INCLUDED_)
#define AFX_DGBATCHFILE_H__D6FF9B12_F0AC_11D1_947A_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Dg_BatchFile.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_BatchFile dialog

class Dg_BatchFile : public CDialog
{
// Construction
public:
	Dg_BatchFile(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_BatchFile)
	enum { IDD = IDD_BATCHFILE };
	int		mCommandNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Dg_BatchFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

   void   Update( int   aCommandNumber );

protected:

	// Generated message map functions
	//{{AFX_MSG(Dg_BatchFile)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGBATCHFILE_H__D6FF9B12_F0AC_11D1_947A_000000000000__INCLUDED_)
