// Dg_AC_RefreshMode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_AC_RefreshMode dialog

class Dg_AC_RefreshMode : public CDialog
{
// Construction
public:
	Dg_AC_RefreshMode(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_AC_RefreshMode)
	enum { IDD = IDD_AUTOCAD_REFRESHMODE };
	int		Refresh_Mode;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(Dg_AC_RefreshMode)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
