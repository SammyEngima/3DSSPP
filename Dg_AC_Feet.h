#ifndef   C_DIA_AC_FEET
#define   C_DIA_AC_FEET
// Dg_AC_Feet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_AC_Feet dialog

class Dg_AC_Feet : public CDialog
{
// Construction
public:
	Dg_AC_Feet(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_AC_Feet)
	enum { IDD = IDD_AUTOCAD_FEET };
	CComboBox	UCS_Foot_Right;
	CComboBox	UCS_Foot_Left;
	//}}AFX_DATA
        
                              //--- Overrides ---
        virtual BOOL   OnInitDialog( void );
        virtual void   OnOK( void );

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(Dg_AC_Feet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif
