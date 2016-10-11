// Dg_TaskAlreadyOpen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_TaskAlreadyOpen dialog

class Dg_TaskAlreadyOpen : public CDialog
{
// Construction
public:
	Dg_TaskAlreadyOpen(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_TaskAlreadyOpen)
	enum { IDD = IDD_FILE_ALREADY_OPEN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(Dg_TaskAlreadyOpen)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
