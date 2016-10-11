//============================ MEMBER FUNCTIONS: ===================== Display
#include "stdafx.h"

#include "Display.hpp"
#include "film_lnk.hpp"
#include "facet.hpp"
#include "palette.hpp"
#include "string.hpp"
#include "shaperec.hpp" //for enumerations


//------------------------------------- default constructor ------------------
Display ::					                       Display
( void )
   :   CClientDC( NULL )
   {
   MessageBox( NULL, "No default constructor.", "Display", MB_OK );
   assert( 0 );
   return;
   }

//------------------------------------- copy constructor ---------------------
Display ::					                       Display
(
const Display&   CopyRef
)
   :   CClientDC( NULL ) 
   { 
   MessageBox( NULL, "No copy constructor.", "Display", MB_OK );
   assert( 0 );
   return;
   }

//------------------------------------- destructor ---------------------------
Display ::					                      ~Display
( void )
   {
   if( this->Old_Palette_Ptr != 0  )
      {
      ASSERT_VALID( this->Old_Palette_Ptr );
      this->CDC_Ptr->CDC::SelectPalette ( this->Old_Palette_Ptr, 0  );
      this->CDC_Ptr->CDC::RealizePalette();
      }



if (m_bMemDC)
{
	this->CDC_Ptr->CDC::SetMapMode( MM_TEXT  );
	this->targetCDC_Ptr->SetWindowExt(this->targetCDC_Ptr->GetViewportExt());

	this->targetCDC_Ptr->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
		this->CDC_Ptr, m_rect.left, m_rect.top, SRCCOPY   );			
	
	//Swap back the original bitmap.
	SelectObject(m_oldBitmap);

	delete this->CDC_Ptr;
	this->CDC_Ptr = this->targetCDC_Ptr;
}

//   this->CDC::SetMapMode    ( this->Old_Map_Mode        );
   
//   this->CDC::SetWindowExt  ( this->Old_Window_Extent   );
//   this->CDC::SetViewportExt( this->Old_Port_Extent     );  
//   this->CDC::SetViewportOrg( this->Old_Viewport_Origin );
   
   return;
   }


#include <strstream>
//---------------------------------- additional constructor ------------------

Display ::					                       Display
(
CView&     CViewRef ,
CDC*       PrintContextPtr ,
int        LogicalWidth ,
int        LogicalHeight,
bool	   UseBackBuffer 
)
   :   CClientDC( &CViewRef ) ,
   CDC_Ptr( NULL ) ,
   New_Palette(),
   New_Brush_Ptr( NULL ),
   New_Pen_Ptr( NULL ),
   Old_Map_Mode( -1 ),
   Old_Palette_Ptr( NULL ) ,
   Old_Brush_Ptr( NULL ) ,
   Old_Pen_Ptr( NULL ) 
   
   {
   BOOL  Printing;
   m_bMemDC = FALSE;	// Dont use the MemDC by default

   if( PrintContextPtr == NULL )
      {
		this->targetCDC_Ptr		= this;
		this->CDC_Ptr			= this;
		Printing = FALSE;
      }
   else      
      {
		  this->targetCDC_Ptr	= PrintContextPtr;
		  this->CDC_Ptr			= PrintContextPtr;
		  Printing = TRUE;
		  
      }

                                     //--- Set new map mode and window extents
   this->Old_Map_Mode = this->CDC_Ptr->CDC::SetMapMode( MM_ISOTROPIC );
                                     
                                     //--- Set viewport extents and origin ---
   CRect   View_Rect;   
   CViewRef.GetClientRect( View_Rect );
   this->Background_Rect = View_Rect;
   
   int   Right, Bottom;
   if( Printing ) //use provided print context parameters
      {
      Right   = this->CDC_Ptr->GetDeviceCaps( HORZRES );
      Bottom  = this->CDC_Ptr->GetDeviceCaps( VERTRES );
      }

   else //use this view's client area parameters   
      {
      Right   = View_Rect.right; 
      Bottom  = View_Rect.bottom;
      }

   // ACT - Fix for windows of ZERO height/width causing SetWindowExt() to ASSERT
   if (Bottom == 0)
	   Bottom = 1;
   if (Right == 0)
	   Right = 1;


                                     //--- Set logical extents ---
   double Width_Ratio  = double(LogicalWidth) / Right;
   double Height_Ratio = double(LogicalHeight) / Bottom;
   
   if( Width_Ratio > Height_Ratio )
      {
      LogicalHeight = int( Bottom * Width_Ratio );
      }
   else
      {
      LogicalWidth = int( Right * Height_Ratio );
      }
                                    //Dirty means of accounting for caption bar.
                                    //The effect of the coefficient is to make
    float Caption_Bar_Fudge = 1.2F;
   this->Old_Window_Extent = this->CDC_Ptr->CDC::SetWindowExt
                                    ( 
                                    int( LogicalWidth * Caption_Bar_Fudge ) ,
                                    int( LogicalHeight * Caption_Bar_Fudge  )
                                    );

   this->Old_Port_Extent = this->CDC_Ptr->CDC::SetViewportExt
                                   ( -Right, -Bottom );

   this->Old_Viewport_Origin = this->CDC_Ptr->CDC::SetViewportOrg
                                   ( Right/2, Bottom/2 );

	// Use a memory drawing context if we aren't printing.
	// This will prevent flickering by means of drawing to memory, and 
	// rewriting the screen only when the entire new image is done
	if (Printing == FALSE && UseBackBuffer == true)
	{
			m_bMemDC = TRUE;

			this->targetCDC_Ptr	= this->CDC_Ptr;
			this->CDC_Ptr		= new CDC;
			this->CDC_Ptr->CreateCompatibleDC(this->targetCDC_Ptr); // Create()

			this->CDC_Ptr->SetMapMode(this->targetCDC_Ptr->GetMapMode());
			this->CDC_Ptr->SetWindowExt(this->targetCDC_Ptr->GetWindowExt());
			this->CDC_Ptr->SetViewportOrg(this->targetCDC_Ptr->GetViewportOrg());
			this->CDC_Ptr->SetViewportExt(this->targetCDC_Ptr->GetViewportExt());

			CViewRef.GetClientRect(&m_rect);
			m_rect.right	= m_rect.right;
			m_rect.bottom	= m_rect.bottom;
			m_rect.left		= -m_rect.right;
			m_rect.top		= -m_rect.bottom;

			m_bitmap.CreateCompatibleBitmap(this->targetCDC_Ptr, m_rect.Width(), m_rect.Height());
			m_oldBitmap = this->CDC_Ptr->SelectObject(&m_bitmap);

	}
   return;
}
   
/*-----------------------------------*/ CPoint        //----------------------
Display ::					             Device_To_Logical
( 
CPoint&   DevicePoint
)
   {
   CPoint   Logical_Point = DevicePoint;
   this->CDC_Ptr->CDC::DPtoLP(  &Logical_Point  );
   return   Logical_Point;
   }
   
//----------------------------------------------------------------------------
   void
   Display ::					             
LogicalToDevice
( CRect &   aLogicalRect )
{
   this->CDC_Ptr->CDC::LPtoDP(  &aLogicalRect  );
   return;
}

/*-----------------------------------*/ void          //----------------------
Display ::					            Display_Background
( void )
//Compute and draw background rectangle
//!!!Should add HSI color to composition
   {
   this->Set_Brush
      ( 
      this->Background_Red, 
      this->Background_Green, 
      this->Background_Blue 
      );                                     
   this->Set_Pen
      ( 
      1 ,
      this->Background_Red, 
      this->Background_Green, 
      this->Background_Blue 
      );                                     

   this->CDC_Ptr->CDC::DPtoLP(  &(this->Background_Rect)  );              
   this->CDC_Ptr->CDC::SetBkMode( OPAQUE );
   this->CDC_Ptr->CDC::Rectangle(  &(this->Background_Rect )  ) ;

   this->Restore_Brush();
   this->Restore_Pen();

   return;
   }

/*-----------------------------------*/ void          //----------------------
Display ::					           Display_Wire_Circle
(
Int_2D_Mesh&   MeshRef
)
//NOTE: Since the bounding rectangle supplied in the mesh is facing in the same
//direction as the camera (i.e., away from the camera), the points are flipped
//left to right. This is the reason for the reversal of X coordinate the 
//rectangular specification for the following graphic.
   {
   assert(  Is_Object_Ref( MeshRef )  );

   this->CDC_Ptr->CDC::Ellipse
      (  
      MeshRef(0,1)(0), MeshRef(0,0)(1) ,   //Upper left corner
      MeshRef(0,0)(0), MeshRef(0,1)(1)     //Lower right corner
      );

//   this->CDC_Ptr->CDC::Arc
//      (  
//      MeshRef(0,1)(0), MeshRef(0,0)(1) ,   //Upper left corner
//      MeshRef(0,0)(0), MeshRef(0,1)(1) ,   //Lower right corner
//      MeshRef(0,0)(0), MeshRef(0,1)(1) ,   //Start point
//      MeshRef(0,0)(0), MeshRef(0,1)(1) - 1 //Stop point
//      );

   return;
   }

/*-----------------------------------*/ void          //----------------------
Display ::					             Display_Wire_Mesh
(
Int_2D_Mesh&   MeshRef ,
int            VertexRows ,
int            VertexColumns
)
   {
   assert(  Is_Object_Ref( MeshRef )  );
                                      //--- Draw the rows --------------
   int*  Points_Ptr = 0;
   Int_2D_Polyline   Row;
   int Cnt  = 0;
   for ( Cnt = 0; Cnt < VertexRows; Cnt ++ )
      {
      Row = MeshRef.Row( Cnt );
      Points_Ptr = &( Row(0)(0) );
      this->CDC_Ptr->CDC::Polyline( (CPoint*)Points_Ptr, VertexColumns );
      }
                                      // --- Draw the columns ----------
   Int_2D_Mesh    Transposed_Mesh;
   Transposed_Mesh = -MeshRef;
   int   Transpose_Rows    =  VertexColumns;
   int   Transpose_Columns =  VertexRows;
   Row.Redimension( Transpose_Columns );
   for ( Cnt = 0; Cnt < Transpose_Rows; Cnt ++ )
      {
      Row = Transposed_Mesh.Row( Cnt );
      Points_Ptr = &( Row(0)(0) );
      this->CDC_Ptr->CDC::Polyline( (CPoint*)Points_Ptr, Transpose_Columns );
      }

   return;
   }

/*-----------------------------------*/ void          //----------------------
Display ::					         Display_Wire_Polyline
(
Int_2D_Mesh&   MeshRef ,
int            VertexColumns
)
   {
   assert(  Is_Object_Ref( MeshRef )  );
                                      //--- Draw the rows --------------
   int*  Points_Ptr = 0;
   Int_2D_Polyline   Row;

   Row = MeshRef.Row( 0 );
   Points_Ptr = &( Row(0)(0) );
   this->CDC_Ptr->CDC::Polyline( (CPoint*)Points_Ptr, VertexColumns );

   return;
   }

/*-----------------------------------*/ void          //----------------------
Display ::					                     Set_Brush
(
BYTE   Red ,
BYTE   Green ,
BYTE   Blue   
)
   {
   assert(  this->New_Brush_Ptr == NULL  );
   assert(  this->Old_Brush_Ptr == NULL  );
                 
   this->New_Brush_Ptr = new CBrush;
   this->New_Brush_Ptr->CreateSolidBrush(   PALETTERGB( Red, Green, Blue )   );

   this->Old_Brush_Ptr  =  this->CDC_Ptr->CDC::SelectObject(  this->New_Brush_Ptr  );

   return;
   }

/*-----------------------------------*/ void          //----------------------
Display ::					                     Set_Brush
(
COLORREF   NewColors
)
   {
   assert(  this->New_Brush_Ptr == NULL  );
   assert(  this->Old_Brush_Ptr == NULL  );
                 
   this->New_Brush_Ptr = new CBrush;
   this->New_Brush_Ptr->CreateSolidBrush(   NewColors   );

   this->Old_Brush_Ptr  =  this->CDC_Ptr->CDC::SelectObject(  this->New_Brush_Ptr  );

   return;
   }

/*-----------------------------------*/ void          //----------------------
Display ::					                 Restore_Brush
( void )
   {
   assert(  this->New_Brush_Ptr != NULL  );
   assert(  this->Old_Brush_Ptr != NULL  );

   this->CDC_Ptr->CDC::SelectObject( this->Old_Brush_Ptr ); 
   
   delete   this->New_Brush_Ptr;
   this->New_Brush_Ptr = NULL;
   this->Old_Brush_Ptr = NULL;

   return;
   }

/*-----------------------------------*/ void          //----------------------
Display ::					                       Set_Pen
(
int    aLinePixelWidth ,
BYTE   Red ,
BYTE   Green ,
BYTE   Blue   
)
   {
   assert( aLinePixelWidth >= 1 );
   assert(  this->New_Pen_Ptr == NULL  );
   assert(  this->Old_Pen_Ptr == NULL  );
                 
   this->New_Pen_Ptr = new CPen;
   
//   const int PixelWidth = aLinePixelWidth;
//
//   this->New_Pen_Ptr->CreatePen
//                         ( 
//                         PS_SOLID, 
//                         PixelWidth, 
//                         PALETTERGB( Red, Green, Blue ) 
//                         );

   LOGBRUSH   lLogBrush;
   lLogBrush.lbStyle = BS_SOLID;
   lLogBrush.lbColor = PALETTERGB( Red, Green, Blue );
   lLogBrush.lbHatch = 0;

   this->New_Pen_Ptr->CreatePen( PS_GEOMETRIC, aLinePixelWidth, & lLogBrush );

   this->Old_Pen_Ptr = this->CDC_Ptr->CDC::SelectObject( this->New_Pen_Ptr );            
   
   return;
   }

/*-----------------------------------*/ void          //----------------------
Display ::					                   Restore_Pen
( void )
   {
   assert(  this->New_Pen_Ptr != NULL  );
   assert(  this->Old_Pen_Ptr != NULL  );
   
   this->CDC_Ptr->CDC::SelectObject( this->Old_Pen_Ptr ); 

   delete   this->New_Pen_Ptr;
   this->New_Pen_Ptr = NULL;
   this->Old_Pen_Ptr = NULL;
   
   return;
   }
