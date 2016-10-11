#ifndef   C_DIA_AC_HAND
#define   C_DIA_AC_HAND
// Dg_AC_Hands.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_AC_Hands dialog

class Dg_AC_Hands : public CDialog
{
// Construction
public:
	Dg_AC_Hands(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_AC_Hands)
	enum { IDD = IDD_AUTOCAD_HANDS };
	CComboBox	UCS_Hand_Right;
	CComboBox	UCS_Hand_Left;
	//}}AFX_DATA
        
                              //--- Overrides ---
        virtual BOOL   OnInitDialog( void );
        virtual void   OnOK( void );

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(Dg_AC_Hands)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif
