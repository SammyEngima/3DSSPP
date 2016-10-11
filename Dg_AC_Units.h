// Dg_AC_Units.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_AC_Units dialog

class Dg_AC_Units : public CDialog
{
// Construction
public:
	Dg_AC_Units(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_AC_Units)
	enum { IDD = IDD_AUTOCAD_UNITS };
	int		Units_Mode;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(Dg_AC_Units)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
