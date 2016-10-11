// dgtsunit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_TaskUnits dialog

class Dg_TaskUnits : public CDialog
{
// Construction
public:
	Dg_TaskUnits(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_TaskUnits)
	enum { IDD = IDD_TASK_UNITS };
	int		Units;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(Dg_TaskUnits)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
