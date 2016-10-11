//############################## HEADER FILE ################################
#ifndef 	 Display_Include
#define   Display_Include
//--------------------- Include Files and Declarations

#include "afxwin.h"

#define _DISPLAY

class	Facet;
class	Int_2D_Mesh;

//============================ Class Definition =============================
class Display : public	CClientDC
{
	//-------------------------- Interface Functions -----------------------------
	//--- Constructors ---
public:
	Display(	CView&	CViewRef,
				CDC*	PrintContextPtr,
				int 	LogicalWidth,
				int 	LogicalHeight,
				bool	UseBackBuffer);
	
	CPoint	Device_To_Logical(CPoint& DevicePoint);
	
	// Converts provided rectangle to the device coordinate system
	void LogicalToDevice (CRect &aLogicalRect);
	
	void Display_Background(void);

	//--- Concrete ---
public:
	Display
		( void );
	
	Display
		(
		const Display&	 CopyRef
		);
	
	//	  Display&			operator = (Not permitted)
	//						   (
	//						   const Display&	AssignRef
	//						   );
	
	virtual 		 ~Display
		( void );
	
	//-------------------------- Internal Functions ------------------------------
private:
	void			  Set_Brush
		(
		BYTE   Red ,
		BYTE   Blue ,
		BYTE   Green
		);
	
	void			  Set_Brush
		(
		COLORREF   NewColors
		);
	
	void			  Restore_Brush
		( void );					   
	
	void			  Set_Pen
		(
		int    aLinePixelWidth ,
		BYTE   Red ,
		BYTE   Blue ,
		BYTE   Green
		);
	
	void			  Restore_Pen
		( void );					   
	
	void			  Display_Wire_Circle 
		(
		Int_2D_Mesh&   MeshRef
		);
	
	void			  Display_Wire_Polyline 
		(
		Int_2D_Mesh&   MeshRef ,
		int 		   VertexColumns
		);
	
	void			  Display_Wire_Mesh 
		(
		Int_2D_Mesh&   MeshRef ,
		int 		   VertexRows ,
		int 		   VertexColumns
		);
	
	//------------------------- Class Attributes ---------------------------------
	//--- Object Attributes ---
	//--- Contained State ---
public:
	CDC*			CDC_Ptr;
	
protected:		
	// Bitmap Buffer to remove flickering
	CRect		m_rect;
	BOOL		m_bMemDC;
	CDC*		targetCDC_Ptr;	
	CBitmap		m_bitmap;		// Offscreen bitmap
	CBitmap*	m_oldBitmap;	// bitmap originally found in CMemDC
	
	int 			Old_Map_Mode;
	CSize			Old_Window_Extent;
	CSize			Old_Port_Extent;
	CPoint			Old_Viewport_Origin;
	
	CPalette		New_Palette;
	
	CBrush* 		New_Brush_Ptr;
	CPen*			New_Pen_Ptr;
	
	CPalette*		Old_Palette_Ptr;
	CBrush* 		Old_Brush_Ptr;
	CPen*			Old_Pen_Ptr;
	
	CRect			Background_Rect;
	
	BYTE			Background_Red;
	BYTE			Background_Green;
	BYTE			Background_Blue;
	//--- Shared State ---
	//--- Derived ---
	//--- Associated Classes ---
   };
   
   //Role:
   
#endif //------------------------------
