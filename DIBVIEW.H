// dibview.h : header file
//
#ifndef CDibViewIncludeMcr
#define CDibViewIncludeMcr

/////////////////////////////////////////////////////////////////////////////
// CDibView view

#include "cdib.h" //*** Added
#include "GLBasic.h"


#define   WM_PRINT_DESKTOP     ( WM_USER + 11 ) //User defined message
#define   WM_PRINT_APPLICATION ( WM_USER + 12 ) //User defined message



class CDibView : public CView
{
	DECLARE_DYNCREATE(CDibView)
protected:
	CDibView();			// protected constructor used by dynamic creation

// Attributes
protected:
      CDib*   Dib_Ptr;                //A pointer to the DIB
public:

// Operations
public:
	void CopyApplicationToClipboard( void );
	void CopyDesktopToClipboard( void );

        void LoadDib( CWnd * aWndPtr );
        void PrintDib( void );

// Implementation
public:
	virtual ~CDibView();
	virtual	void OnDraw(CDC* pDC);		// overridden to draw this view
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo); //*** Added
	void CopyDIBToClipboard( void );

	//GLBasic glbasic;

	// Generated message map functions
protected:
	//{{AFX_MSG(CDibView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	//User defined message handlers
	afx_msg LRESULT OnPrintDesktop( WPARAM wParam, LPARAM lParam );                      
	afx_msg LRESULT OnPrintApplication( WPARAM wParam, LPARAM lParam );                      
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual void OnInitialUpdate();
};

#endif
/////////////////////////////////////////////////////////////////////////////
