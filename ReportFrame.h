#if !defined(AFX_REPORTFRAME_H__7C320DD1_EC8F_11D0_92C4_000000000000__INCLUDED_)
#define AFX_REPORTFRAME_H__7C320DD1_EC8F_11D0_92C4_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ReportFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ReportFrame frame

class ReportFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(ReportFrame)
protected:
	ReportFrame();           // protected constructor used by dynamic creation

// Attributes
private:

   UINT mShowCmd;

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ReportFrame)
   protected:
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   //}}AFX_VIRTUAL
void            Update_Title_Bar( void );
// Implementation
protected:
   virtual ~ReportFrame();

   // Generated message map functions
   //{{AFX_MSG(ReportFrame)
   afx_msg void OnKillFocus(CWnd* pNewWnd);
   afx_msg void OnSetFocus(CWnd* pOldWnd);
   //}}AFX_MSG
   afx_msg LRESULT OnUpdatePosition( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnUpdateTitleBar( WPARAM wParam, LPARAM lParam );

   afx_msg LRESULT OnDrawOpenGL( WPARAM wParam, LPARAM lParam );

   DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTFRAME_H__7C320DD1_EC8F_11D0_92C4_000000000000__INCLUDED_)
