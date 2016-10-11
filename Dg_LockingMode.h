// Dg_LockingMode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_LockingMode dialog

class Dg_LockingMode : public CDialog
{
// Construction
public:
	Dg_LockingMode(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_LockingMode)
	enum { IDD = IDD_LOCKING_MODE };
	int		Locking_Mode;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(Dg_LockingMode)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
