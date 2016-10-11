// quad_frm.h : header file
//
#ifndef Quadrant_Frame_Include
#define Quadrant_Frame_Include

#include "afxwin.h"

enum E_Quadrant
{ 
   QD_TOPLEFT,
   QD_TOPMIDDLE,
   QD_TOPRIGHT,
   QD_BOTTOMLEFT,
   QD_BOTTOMRIGHT,
   QD_ANIMATION
};

enum E_View_Type
{
   VT_OBLIQUE,
   VT_FRONT,
   VT_SIDE,
   VT_TOP,
   VT_STATUS,
   VT_REPORT,
   VT_ANIMATION
};

/////////////////////////////////////////////////////////////////////////////
// Quadrant_Frame frame

class Quadrant_Frame : public CMDIChildWnd
{
   DECLARE_DYNCREATE(Quadrant_Frame)
   protected:
   Quadrant_Frame(); // protected constructor used by dynamic creation

protected:	// Attributes
   CString Caption;
   E_Quadrant Quadrant;
   E_View_Type mViewType;

public:		// Operations
   virtual BOOL PreCreateWindow( CREATESTRUCT& CreateStructureRef );
   void Update_Title_Bar();

protected:	// Implementation
   virtual ~Quadrant_Frame();

private:
   void Compute_Placement(int& RetLeftEdge,
      int& RetTopEdge,
      int& RetWidth,
      int& RetHeight);

protected:	 
   // Generated message map functions
   //{{AFX_MSG(Quadrant_Frame)
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   //}}AFX_MSG

   // Re-routed message handlers
   afx_msg void OnSysCommand( UINT nID, LPARAM lParam );

   //	User defined message handlers
   afx_msg LRESULT OnUpdatePosition( WPARAM wParam, LPARAM lParam );                      
   afx_msg LRESULT OnUpdateTitleBar( WPARAM wParam, LPARAM lParam );

   afx_msg LRESULT OnDrawOpenGL( WPARAM wParam, LPARAM lParam );
                  
   DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////
#endif
