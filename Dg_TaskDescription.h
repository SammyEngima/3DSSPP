// Dg_TaskDescription.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_TaskDescription dialog

class Dg_TaskDescription : public CDialog
{
// Construction
public:
	Dg_TaskDescription(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_TaskDescription)
	enum { IDD = IDD_TASK_DESCRIPTION };
	CString	Task_Comments;
	CString	Analyst_Name;
	CString	Task_Name;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(Dg_TaskDescription)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString Company_Name;
	CString Frame_Name;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPrintPrintwindow();
};
