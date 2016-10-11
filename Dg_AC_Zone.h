#ifndef   INCLUDE_C_DIA_AC_ZONE
#define   INCLUDE_C_DIA_AC_ZONE
// Dg_AC_Zone.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_AC_Zone dialog

class Dg_AC_Zone : public CDialog
{
// Construction
public:
	Dg_AC_Zone(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_AC_Zone)
	enum { IDD = IDD_AUTOCAD_ZONE };
	int		ZRX_0;
	int		ZRX_1;
	int		ZRX_2;
	int		ZRX_3;
	int		ZRX_4;
	int		ZRX_5;
	int		ZRY_0;
	int		ZRY_1;
	int		ZRY_2;
	int		ZRY_3;
	int		ZRY_4;
	int		ZRY_5;
	int		ZRZ_0;
	int		ZRZ_1;
	int		ZRZ_2;
	int		ZRZ_3;
	int		ZRZ_4;
	int		ZRZ_5;
	double	ZTX_0;
	double	ZTX_1;
	double	ZTX_2;
	double	ZTX_3;
	double	ZTX_4;
	double	ZTX_5;
	double	ZTY_0;
	double	ZTY_1;
	double	ZTY_2;
	double	ZTY_3;
	double	ZTY_4;
	double	ZTY_5;
	double	ZTZ_0;
	double	ZTZ_1;
	double	ZTZ_2;
	double	ZTZ_3;
	double	ZTZ_4;
	double	ZTZ_5;
	CString	ZName_0;
	CString	ZName_1;
	CString	ZName_2;
	CString	ZName_3;
	CString	ZName_4;
	CString	ZName_5;
	CString	Units_Label;
	//}}AFX_DATA
        
                              //--- Overrides ---
        virtual BOOL   OnInitDialog( void );
        virtual void   OnOK( void );

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(Dg_AC_Zone)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif
