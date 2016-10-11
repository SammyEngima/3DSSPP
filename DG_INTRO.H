// Dg_Intro.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_Intro dialog

class Dg_Intro : public CDialog
{
// Construction
public:
	Dg_Intro(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_Intro)
	enum { IDD = IDD_INTRODUCTION };
	CString	Intro_Message;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(Dg_Intro)
	afx_msg void OnIntroMsg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
