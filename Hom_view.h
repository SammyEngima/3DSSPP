#ifndef HOM_VIEW_H
#define HOM_VIEW_H

#include "stdafx.h"
#include "afxwin.h"
#include "Analysis.h"
#include "Graph_db.hpp"
#include "ptrchain.hpp"
#include "glHom/locals.h"
#include "ReportView.hpp"
#include "hom_doc.h"

#include <string>

using std::string;

// Used for locking
#define UPPER_NECK 1
#define LOWER_NECK 2
#define LEFT_SHOULDER 13
#define RIGHT_ANKLE 7
#define LEFT_ANKLE 11
#define PELVIS 4
#define RIGHT_ELBOW 14
#define RIGHT_HAND_CTRL 16
#define LEFT_ELBOW	18
#define LEFT_HAND_CTRL 20
#define FORCE_R 25
#define FORCE_L 26
#define FORCE_EL_L 36
#define FORCE_EL_R 37
#define FORCE_SHOL_L 38
#define FORCE_SHOL_R 39
#define FORCE_L5_S1 40
#define FORCE_HIP_L	41
#define FORCE_HIP_R 42
#define FORCE_KNEE_L 43
#define FORCE_KNEE_R 44
#define FORCE_ANKLE_L 45
#define FORCE_ANKLE_R 46

// Used for scaling the GL stick figure
#define FLOOR_OFFSET 35
#define SCALED_ORIGINAL_WORLD_SIZE 1098.2578f // World Size before scaling to the center
#define HEIGHT_FACTOR 69.3f


class C_Hom_View : public CView // Changed from CView
{
protected:

   //Nested target class

   struct Target
   {
   public:   
      Target
      (
         int   aLeft , int   aTop ,
         int   aRight, int   aBottom ,
         int   aSelectionCode
      )
      : 
      mRect( aLeft, aTop, aRight, aBottom ) , 
      mSelectionCode( aSelectionCode )
      {}
   
      CRect mRect;
      int   mSelectionCode; 
   };

protected: // create from serialization only
	C_Hom_View();
	DECLARE_DYNCREATE(C_Hom_View)
	
// Attributes
public:
   C_Hom_Doc*    GetDocument();

   std::string        Monitor_Name;
   E_View_Type   View_Type;

   int			  mCameraManipulationInProgress;
   CPoint		  mStartPoint;	// Used for Mut-Excl events: OBL cam manip/ORTHO posture select
   CPoint		  mAbsoluteStartPoint;
   bool          mSelectInProgress;
   bool			  mBigMove;
   bool			  mCtrlClick;
   bool			  mShiftClick;
   bool			  mModeChange;
   int			  mLockChange;
   Ptr_Chain      mTargets;
   int            mSelectionCode;
   CDC*			  mpDC;
   int                    msaved_altitude;
   int                    msaved_azimuth;

// Implementation
public:
	virtual         ~C_Hom_View();
	virtual void     OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void     OnInitialUpdate( void );
	virtual BOOL	 PreCreateWindow(CREATESTRUCT& cs);
	virtual void	 OnPrint(CDC* pDC, CPrintInfo* pInf );
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	
	

protected:
        GLUquadric*		  quadric;
        CDibView*		  mDibViewPtr;
        void Select_Screen_Point( CPoint   DevicePoint );
	void AdjustCoordinates(long& x, long& y);
		//void DrawBackground(C_Hom_View *view_ptr);
protected:
	
	                          //--- Printing support ---
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnPrepareDC( CDC *pDC, CPrintInfo* pInfo = NULL );
	void SetLocking();
	void CalculateJointForce(JointTorqueEnm joint, Graphic_Database&   GDB, double horzAngle, double vertAngle);
	void Print_GL_Figure(CDC* pDC);
	void RenderFloor(	perihelion::pVector3f& heel, float floorWidth, 
						float floorHeight, float floorDepth, 
						GLubyte fillRed, GLubyte fillGreen, 
						GLubyte fillBlue, bool outline);
	void RenderGLOblique(double aPelvicTilt);
	void RenderEnvironment();
	void SetupHominoidScaling(float x, float y, float z, double PelvAng);
	void RenderHandHeldObjects(perihelion::pVector3f& leftHand, perihelion::pVector3f& rightHand);
	void RenderBarrier();
	void RenderChairBack();
	void RenderChair(perihelion::pVector3f& leftAnkle, perihelion::pVector3f& rightAnkle, perihelion::pVector3f& midNormal, perihelion::pVector3f& midFront);
	void RenderChairSupport(float x, float y, float z, float seatLocation, bool chairBack = false);

	float mWindowScale_x;			// Stores the scaling of a specific window size for the glstickfigure.
	float mWindowScale_y;
	float mOldViewport_x;
	float mOldViewport_y;
	bool			SetDCPixelFormat(HDC hDC, DWORD dwFlags);	
	
public:
	HDC		mhDC;
	HGLRC	mhRC;
	float	mWorldSize;

public:
	void			TryDeleteTargets(void);	// ACT - memory cleanup
	void			AddTargetRect(CRect rect, int SelectionCode);	// DAC - allows gl_vid.cpp to add to mTargets
	
	// Generated message map functions
protected:
	//{{AFX_MSG(C_Hom_View)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnOblFrontView();
	afx_msg void OnOblStdZoom();
	afx_msg void OnOblBackView();
	afx_msg void OnOblLeftView();
	afx_msg void OnOblRightView();
	afx_msg void OnOblTopView();
	afx_msg void OnOblCenterFigure();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
   
   // Custom message handlers
   afx_msg UINT OnNcHitTest( CPoint point );
	afx_msg void OnFilePrintPreview();

	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnOblUserview();
};

//@@@#ifndef _DEBUG  // debug version in Hom_view.cpp
inline C_Hom_Doc* C_Hom_View::GetDocument()
   { return (C_Hom_Doc*)m_pDocument; }
//#endif

// Helper function for direct manipulation
bool TargetIsForce(int selectionCode);


/////////////////////////////////////////////////////////////////////////////

#endif   // Include guard
