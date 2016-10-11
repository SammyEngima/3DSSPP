// Dg_AC_Position.h : header file
//
#ifndef C_DIA_AC_POSITION_H
#define C_DIA_AC_POSITION_H

/////////////////////////////////////////////////////////////////////////////
// Dg_AC_Position dialog

class Dg_AC_Position : public CDialog
{
// Construction
public:
	Dg_AC_Position(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_AC_Position)
	enum { IDD = IDD_AUTOCAD_POSITION };
	CComboBox	UCS_World;
	double	Tra_X;
	double	Tra_Y;
	double	Tra_Z;
	int		Data_Method;
	int		Positioned_Point;
	CString	Units_Label;
	int		Rot_X;
	int		Rot_Y;
	int		Rot_Z;
	int		Active_Zone;
	CString	Zone_Name_0;
	CString	Zone_Name_1;
	CString	Zone_Name_2;
	CString	Zone_Name_3;
	CString	Zone_Name_4;
	CString	Zone_Name_5;
	//}}AFX_DATA
        
                              //--- Overrides ---
        virtual BOOL   OnInitDialog( void );
        virtual void   OnOK( void );

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(Dg_AC_Position)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif
