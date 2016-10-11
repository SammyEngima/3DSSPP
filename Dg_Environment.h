// Dg_Environment.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_Environment dialog
#if ! defined( C_Dia_Environment_IncludeMcr )
#define C_Dia_Environment_IncludeMcr

class Dg_Environment : public CDialog
{
// Construction
public:
   Dg_Environment(C_Hom_Doc* aHomDocPtr,
      CWnd* pParent = NULL);	// standard constructor

   // Dialog Data
   //{{AFX_DATA(Dg_Environment)
   enum { IDD = IDD_ENVIRONMENT };
   BOOL	Floor_Visible;
   BOOL	outline;
   int		Handheld_Type;
   int		Handheld_Height;
   int		Handheld_Width;
   int		Bar_Thickness;
   int		Bar_Azimuth;
   int		Bar_Pitch;
   int		Bar_Distance;
   int		Bar_Elevation;
   int		Bar_Length;
   int		Bar_Width;
   int		Bar_Type;
   CString	Bar_Distance_Units;
   CString	Bar_Elevation_Units;
   CString	Bar_Length_Units;
   CString	Bar_Thickness_Units;
   CString	Bar_Width_Units;
   CString	Handheld_Height_Units;
   CString	Handheld_Width_Units;
   CWnd*	colorPreview;
   //}}AFX_DATA 

   // Custom attributes

   C_Hom_Doc* mDocPtr;

   int		Min_Handheld_Height;
   int		Max_Handheld_Height;
   int		Min_Handheld_Width;
   int		Max_Handheld_Width;

   int		Min_Bar_Thickness;
   int		Max_Bar_Thickness;
   int		Min_Bar_Distance;
   int		Max_Bar_Distance;
   int		Min_Bar_Elevation;
   int		Max_Bar_Elevation;
   int		Min_Bar_Length;
   int		Max_Bar_Length;
   int		Min_Bar_Width;
   int		Max_Bar_Width;

   // Fill Color
   BYTE red;
   BYTE green;
   BYTE blue;
   // Outline Color
   BYTE outlineRed;
   BYTE outlineGreen;
   BYTE outlineBlue;
	

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	

	// Generated message map functions
	//{{AFX_MSG(Dg_Environment)
	afx_msg void OnApply();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOrtcolorbutton();
	afx_msg void OnBnClickedOutlinecolor();
};

#endif
