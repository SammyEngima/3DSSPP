// Dg_ObliqueFigure.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_ObliqueFigure dialog
#include "afxwin.h"
#if ! defined( C_Dia_Hominoid_IncludeMcr )
#define C_Dia_Hominoid_IncludeMcr

class Dg_ObliqueFigure : public CDialog
{
   // Construction
public:	
   Dg_ObliqueFigure( CWnd* pParent = NULL);   // standard constructor

   // Dialog Data
   //{{AFX_DATA(Dg_ObliqueFigure)
   enum { IDD = IDD_OBLIQUE_FIGURE };
   int Model_Mode;

   //}}AFX_DATA

   // Implementation

protected:
   virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
   bool UpdateDocument();
   COLORREF skin;
   COLORREF maleShirt;
   COLORREF malePants;
   COLORREF femaleShirt;
   COLORREF femalePants;

	CRect ColorDrawRect;
	CRect malePantDrawRect;
	CRect maleShirtDrawRect;
	CRect femalePantDrawRect;
	CRect femaleShirtDrawRect;

   // Generated message map functions
   //{{AFX_MSG(Dg_ObliqueFigure)
   afx_msg void OnClothingColor();
   afx_msg void OnApply();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedFigurecolor();
   afx_msg void OnPaint();
   afx_msg void OnBnClickedOk();
   // afx_msg void OnStnClickedFigurecolorprev2();
   afx_msg void OnBnClickedMaleShirtcolor();
   afx_msg void OnBnClickedFemaleShirtcolor();
   afx_msg void OnBnClickedMalePantscolor();
   afx_msg void OnBnClickedFemalePantscolor();

   virtual BOOL OnInitDialog();
};
#endif
