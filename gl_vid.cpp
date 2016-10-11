/*	gl_vic.cpp is an ANSI C compatible openGL rendering context wrapper for any
win32 application.  It allows for multiple contexts per application, referenced
by a string name.
*/

#include <stdafx.h>
//#include <windows.h>								// Header File For Windows
//#include <string.hpp>
#include <string>

#include "gl_vid.hpp"
#include "hom.h"
#include "hom_doc.h"
#include "Hom_view.h"
#include "Analysis.h"
#include "point3d.h"

using std::string;

#define MINIMUM_TOP_VIEW_WORLD_SIZE 950.0f
#define LOOK_AT_DISTANCE 200.0f
#define TOP_LOOK_AT_DISTANCE 1.0f
#define POINT_RADIUS 25.0			// Changed from 25.0
#define ARROW_STARTING_PERC .75f
#define FORCE_LENGTH 250.0f

// Used to define the joint spheres
#define SPHERE_LATITUDE 20
#define SPHERE_LONGITUDE 20

glContext_t *g_GLWindowList = NULL;
glContext_t *g_ActiveContext = NULL;
static bool glEnabled = true;

//static GL_Background background;
static GLUquadricObj *q;
static bool initialized[47] = { false, false, false, false, false, false, false, false, false, false, false,
false, false, false, false, false, false, false, false, false, false, false,
false, false, false, false, false, false, false, false, false, false, false,
false, false, false, false, false, false, false, false, false, false, false,
false, false, false };

// Points that are to be displayed
static bool DisplayPoint[47] = {	false, true, true, true, true,
true, true, true, false,
true, true, true, false,
true, true, false, true,
true, true, false, true,
false, false, false, false,
false, false,
false, false, false, false,
false, false, false, false,
false, true, true, true, true,
true, true, true, true, true,
true, true
};

// Points that are not MS_NULL are selectable.
static int SelectionCode[47] = {	MS_NULL, MS_TRUNK_FLEXION, MS_TRUNK_LATERAL_BENDING, MS_NULL, MS_HIPS,	
MS_NULL, MS_NULL, MS_ANKLE_R, MS_NULL,
MS_NULL, MS_NULL, MS_ANKLE_L, MS_NULL,
MS_TRUNK_AXIAL_ROTATION,	MS_ELBOW_R, MS_NULL, MS_HAND_CTR_R,
MS_NULL,					MS_ELBOW_L, MS_NULL, MS_HAND_CTR_L,
MS_NULL, MS_NULL, MS_NULL, MS_NULL,
MS_FORCE_R_HAND, MS_FORCE_L_HAND,
MS_NULL, MS_NULL, MS_NULL, MS_NULL,
MS_NULL, MS_NULL, MS_NULL, MS_NULL,
MS_NULL, MS_NULL, MS_NULL, MS_NULL,
MS_NULL, MS_NULL, MS_NULL, MS_NULL,
MS_NULL, MS_NULL, MS_NULL, MS_NULL
};

const int SELECTION_OFFSET=36;	// This offset determines where the joint forces (minus the hand forces) are in the array
// allowing for them to be turned on and off to eliminate the drawing of a some random
// selectable point - JTK
/*  MS_FORCE_R_ELBOW, MS_FORCE_L_ELBOW,	// Added to include force vectors on the joints
MS_FORCE_R_SHOULD ,MS_FORCE_L_SHOULD, 
MS_FORCE_L5S1, MS_FORCE_R_HIP,MS_FORCE_R_KNEE,MS_FORCE_R_ANKLE,
MS_FORCE_L_HIP,   
MS_FORCE_L_KNEE,
MS_FORCE_L_ANKLE*/

// Floor Constants
#define CENTER_OFFSET 67
#define	BOX_SIZE 3.6
#define UPPER_OFFSET 125
#define LOWER_OFFSET 125
// Returns 'true' if the context does not exist or it was successfully removed
// Returns 'false' if the context was unable to be destroyed.
bool DestroyGLContext (string FrameName) 
{
   wglMakeCurrent(NULL,NULL);

   // Delete glDC list
   glContext_t *current, *prev;
   prev	= NULL;
   current	= g_GLWindowList;
   while (current != NULL) {
      if (current->name == FrameName)
      {
         // Shut down GL window here
         if (!wglDeleteContext(current->gl_hRC))
            return false;

         // Delete GL window from linked list
         if (prev == NULL)
            g_GLWindowList = current->next;
         else
            prev->next = current->next;
         delete current;
         if (prev == NULL)
            current = g_GLWindowList;
         else 
            current = prev->next;
      }
      else 
      {
         prev = current;
         current = current->next;
      }

   };
   return true;	
}

// ==0 -> Success
// !=0 -> Error Code
int ShutdownOpenGL()
{
   glEnabled = false;

   wglMakeCurrent(NULL,NULL);
   // Delete glDC list
   glContext_t *current, *destroy;
   current = g_GLWindowList;
   while (current != NULL) {
      destroy = current;
      current = current->next;
      // Release the current context
      if (!wglDeleteContext(destroy->gl_hRC))
         return GetLastError();
      delete destroy;
   };
   g_ActiveContext = NULL;
   if (q != NULL)
      gluDeleteQuadric(q);
   g_GLWindowList  = NULL;
   glEnabled = false;
   return true;
}

bool CreateGLContext(HWND hWnd, HDC hDC, HGLRC *ret_hRC, string FrameName, int PerspectiveType) 
{
   if (!glEnabled)
      return false;
   glContext_t *newnode;
   if (!VALID_PERSPECTIVE(PerspectiveType))
      return false;
   //	CDC* dcMem = GetDC(hWnd);
   //	dcMem->CreateDC("DISPLAY",NULL,NULL,NULL);
   //	dcMem->CreateCompatibleDC(NULL);//CDC::FromHandle(GetDC(hWnd)));

   newnode = new glContext_t;
   newnode->hInstance		= NULL;				// Unused as of yet
   newnode->gl_hRC			= NULL;				// Set by OpenGL::wglCreateContext()
   newnode->gl_hWnd		= hWnd;
   newnode->gl_hDC			= hDC; // GetDC(hWnd);
   newnode->name			= FrameName;
   newnode->gl_perspective	= PerspectiveType;
   newnode->next			= NULL;

   //	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
   //	CDC* cDC = CDC::FromHandle(GetDC(hWnd));
   //	CDC* cDC;
   //	cDC = CDC::FromHandle(newnode->gl_hDC);
   //	HDC pDC = GetDC(hWnd);
   //	cDC->Attach(pDC);

   // Did We Get A Device Context?
   if (!newnode->gl_hDC)
   {
      MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
      delete newnode;
      return false;
   }	
   newnode->gl_hDC = GetDC(hWnd);
   GLuint	PixelFormat;
   // Did Windows Find A Matching Pixel Format?
   if (!(PixelFormat=ChoosePixelFormat(newnode->gl_hDC,&pfd)))
   {
      MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
      delete newnode;
      return false;
   }

   // Are We Able To Set The Pixel Format?
   if(!SetPixelFormat(newnode->gl_hDC,PixelFormat,&pfd))
   {
      MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
      delete newnode;
      return false;
   }

   // Are We Able To Get A Rendering Context?
   if (!(newnode->gl_hRC=wglCreateContext(newnode->gl_hDC)))
   {
      MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
      delete newnode;
      return false;
   }
   *ret_hRC = newnode->gl_hRC;

   //	ShowWindow(newnode->gl_hWnd,SW_SHOW);	// Show The Window [in case it is hidden]
   //	SetForegroundWindow(newnode->gl_hWnd);	// Move it to the front
   //	SetFocus(newnode->gl_hWnd);				// Sets Keyboard Focus To The Window
   //	Vid_ResizeGLScene(width, height);		// Set Up Our Perspective GL Screen

   // Try To Activate The Rendering Context

   if(!wglMakeCurrent(newnode->gl_hDC,newnode->gl_hRC))
   {
      // We just made it, but can't use it. Kill the render context.
      wglDeleteContext(newnode->gl_hRC);
      int err = glGetError();
      DWORD err2 = GetLastError() & ~0xFFFF;
      MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
      delete newnode;
      return false;
   }

   glShadeModel(GL_SMOOTH);	

   // Begin Antialiasing
   glEnable(GL_LINE_SMOOTH);
   glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   // End Antialiasing
   //glDepthFunc(GL_ALWAYS);								// Replace pixels if they're CLOSER or same distance as current pixel
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);	// Pick Perspective Calculations			

   // Leave lighting out for now. Probably enable it for the oblique view
   //InitLighting();

   glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
   glLoadIdentity();									// Reset The Projection Matrix

   q = gluNewQuadric();								// create a quadric objectto draw the spheres, cylinders, etc
   gluQuadricDrawStyle(q, GLU_FILL);
   gluQuadricOrientation(q, GLU_INSIDE);
   /*  glOrtho is called each time in Draw_GL_Stick_Figure()
   // Calculate The Aspect Ratio Of The Window
   switch (newnode->gl_perspective) 
   {
   case GL_OBLIQUE:
   case GL_ORTHO:
   gluPerspective(90.0f, 1.0f,0.1f,4096.0f);			// Set the frustum to 90°
   break;
   //		case GL_ORTHO:
   glOrtho(0,2048,2048,0,-1,4096);
   break;
   default:
   delete newnode;
   return false;
   }
   */

   glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
   glLoadIdentity();	

   // Add node to renderlist
   glContext_t *prev	 = NULL;
   glContext_t *current = g_GLWindowList;
   while (current != NULL) 
   {
      prev = current;
      current = current->next;
   };
   if (prev != NULL)
      prev->next = newnode;		// Add this node to the end of the list
   else
      g_GLWindowList = newnode;	// First node in the list
   g_ActiveContext = newnode;

   return true;
}

bool ActivateGLWindow(string NAME) 
{
   glContext_t *current = g_GLWindowList;

   while (current != NULL) {
      if (current->name == NAME)
      {
         // Try To Activate The Rendering Context
         // current->gl_hDC = GetDC(current->gl_hWnd);
         if(!wglMakeCurrent(current->gl_hDC,current->gl_hRC))
         {
            // We just made it, but can't use it. Kill the render context.
            wglDeleteContext(current->gl_hRC);
            MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
            return false;
         }

         // Clear Screen

         g_ActiveContext = current;	// change the active context to the new one
         return true;
      }
      current = current->next;
   };
   return false;
}

void InitLighting()
{
   GLfloat LightAmbient[]= { 0.1f, 0.1f, 0.1f, 1.0f }; 				// Ambient Light Values ( NEW )
   GLfloat LightDiffuse[]= { 0.8f, 0.8f, 0.8f, 1.0f };				 // Diffuse Light Values ( NEW )
   GLfloat LightPosition[]= { 250.0f, 0.0f, 500.0f };				 // Light Position ( NEW )

   glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				// Setup The Ambient Light
   glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				// Setup The Diffuse Light
   glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);			// Position The Light
   glEnable(GL_LIGHT1);										// Enable Light One
   glEnable(GL_LIGHTING);										// Turn on lighting
   glEnable(GL_COLOR_MATERIAL);								// Allow glColor to set objects color
}

#define AXIS_LENGTH 100.0f
void DrawAxes()
{
   // draw center
   glColor3f(1.0f, 0.0f, 0.0f);
   glPointSize(10);
   glBegin(GL_POINTS);
   glVertex3f(0.0f, 0.0f, 0.0f);
   glEnd();

   glLineWidth(1);
   // draw x-axis
   glColor3f(1.0f, 0.0f, 0.0f);
   glBegin(GL_LINES);
   glVertex3f(0.0f, 0.0f, 0.0f);
   glVertex3f(AXIS_LENGTH, 0.0f, 0.0f);
   glEnd();
   // draw y-axis
   glColor3f(0.0f, 1.0f, 0.0f);
   glBegin(GL_LINES);
   glVertex3f(0.0f, 0.0f, 0.0f);
   glVertex3f(0.0f, AXIS_LENGTH, 0.0f);
   glEnd();
   // draw z-axis
   glColor3f(0.0f, 0.0f, 1.0f);
   glBegin(GL_LINES);
   glVertex3f(0.0f, 0.0f, 0.0f);
   glVertex3f(0.0f, 0.0f, AXIS_LENGTH);
   glEnd();
}

#define TARGET_RECT_SIZE 6
void GenerateTargetRects(C_Hom_View * view_ptr)
{
   int i;
   Flt_3D_Polyline WorldPoints = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name ).Get_Points();
   Flt_3D_Point currentPoint;
   CPoint center;
   double	scale_x, scale_y,
      offset_x, offset_y;
   int target_rect_size;
   CRect windowRect;
   view_ptr->GetWindowRect(&windowRect);
   // Traverse all the possible points
   for (i=0; i<47;i++)	
   {
      if(SelectionCode[i] != MS_NULL)		// only generate target rect if the point is selectable
      {
         CRect TargetRect;
         currentPoint = WorldPoints(i);
         CPoint center;
         if((SelectionCode[i] == MS_FORCE_R_HAND) || (SelectionCode[i] == MS_FORCE_L_HAND))
         {
            // Adjust currentPoint according to it's corresponding hand point
            currentPoint = AdjustedForceEndPoint(	currentPoint, 
               (SelectionCode[i] == MS_FORCE_R_HAND) ? 
               WorldPoints(R_HAND_CTR) : WorldPoints(L_HAND_CTR));
         } 
         // Now, set the center and lookup the selection code
         switch(view_ptr->View_Type)
         {
         case VT_FRONT:
            center.x = long(currentPoint(1));		// map y to x
            center.y = long(currentPoint(2));		// map z to y
            break;
         case VT_SIDE:
            center.x = long(currentPoint(0));		// map x to x
            center.y = long(currentPoint(2));		// map z to y
            break;
         case VT_TOP:
            center.x = long(currentPoint(1));		// map y to x
            center.y = long(-currentPoint(0));	// map -x to y
            break;
         case VT_OBLIQUE:
         case VT_STATUS:
         case VT_REPORT:
         case VT_BAL:
            break;
         }

         int GL_viewport_size_x, GL_viewport_size_y;

		 
		 CalculateViewportSizeToRatio(GL_viewport_size_x, GL_viewport_size_y, windowRect, view_ptr);

         // Set the scale according to ...
         scale_x =	(0.275f/WIDTH_RATIO) *									// ... adjusting WorldPoints to screen points
            (ORIGINAL_WORLD_SIZE/(view_ptr->mWorldSize)) *			// ... how much the world has scaled
            (GL_viewport_size_x);										// ... how much the viewport has scaled
         scale_y =	(0.285f/HEIGHT_RATIO) * 
            (ORIGINAL_WORLD_SIZE/(view_ptr->mWorldSize)) * 
            (GL_viewport_size_y);

         offset_x = (windowRect.Width()) * WIDTH_CENTER;
         offset_y = (windowRect.Height()) * HEIGHT_CENTER;

         // Scale the points to fit the viewport
         center.x = long(center.x * scale_x);
         center.y = long(center.y * scale_y);

         // transform the window center to the upper left corner
         center.x = long(offset_x + center.x);
         center.y = long(offset_y - center.y);

         // transform from window coordinates to screen coordinates
         center.x += windowRect.left;
         center.y += windowRect.top;

         target_rect_size = GetTargetRectSize(windowRect);
         // Create target rect around center
         TargetRect.left = center.x-target_rect_size;
         TargetRect.top  = center.y-target_rect_size;
         TargetRect.right = center.x+target_rect_size;
         TargetRect.bottom  = center.y+target_rect_size;

         // Dynamically allocate a target and add to mTargets member of C_Hom_View
         view_ptr->AddTargetRect( TargetRect, SelectionCode[i]);
      }
   }
}

void Draw_Background(C_Hom_View *view_ptr, CDC* pDC)
{
   //glDisable(GL_BLEND);
   //glDisable(GL_LIGHTING);
   /*glEnable(GL_TEXTURE_2D);
   if(background.Changed() || view_ptr->GetDocument()->getOpenGLPrinting())	// load if necessary
   {
      // Load bitmap
      if(!background.Load((CView*)view_ptr))
      {		
         AfxMessageBox("Unable to load bitmap as background");
      }
   }
   background.Draw(pDC);
   //glEnable(GL_BLEND);
   //glEnable(GL_LIGHTING);
   glDisable(GL_TEXTURE_2D);*/
}

#define MAX_SKEL_POLYLINES			8
#define VERTS_PER_SKEL_POLYLINE		4
int lines[MAX_SKEL_POLYLINES][VERTS_PER_SKEL_POLYLINE] = {	
   {R_HAND_CTR,	R_ELBOW,	R_SHOULDER,		C7T1 },
   {R_SHOULDER,	L5_S1,		R_HIP,			R_KNEE },
   {R_KNEE,		R_ANKLE,	R_HEEL,			R_HEEL },
   {L_HAND_CTR,	L_ELBOW,	L_SHOULDER,		C7T1 },
   {L_SHOULDER,	L5_S1,		L_HIP,			L_KNEE },
   {L_KNEE,		L_ANKLE,	L_HEEL,			L_HEEL },
   {R_HIP,			HIPS_CTR,	L_HIP,			L_HIP },
   {C7T1,			NECK_TOP,	NECK_TOP,		NECK_TOP }
};

// This will be able to handle the 4 different views
void Draw_GL_Stick_Figure(C_Hom_View * view_ptr)
{
   C_Hom_Doc* docPtr = view_ptr->GetDocument();
   // Get the current views current color and change the clear color to that color
   ttarray& docAForce = view_ptr->GetDocument()->getAForceRef();
   glClearColor(1.0f, 1.0f, 145.0f/255.0f, 1.0f);
   // Set the clear color and clear buffers
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

   SetProjectionMatrix(view_ptr);
   SetColor();
   SetViewport(view_ptr);
   SetCamera(view_ptr);
   SetLineWidth(view_ptr);
   SetRectangle(view_ptr);
   DrawSkeleton(view_ptr);
   DrawBackgroundColorVertex(view_ptr);
   SetColor();
   DrawVertices(view_ptr);
   DrawHead(view_ptr->View_Type);
   
    

   // Draw the two joint forces with selectable end points
   if (true)
   {
      // Get the force color
      BYTE Force_red, Force_green, Force_blue;
      Force_red = 200;
      Force_green = 0;
      Force_blue = 0;
      if (view_ptr->GetDocument()->getHandLoadsRef().TrueLeftMagnitude() > 0)
      {
         // All of the force drawing functions have the parameters set up so the arrow and sphere can be drawn
         // separate colors.  The parameter list goes arrow rgb, then sphere rgb.  Currently, they are always
         // set to the same color, hence passing through the same variable twice.
         DrawJointForces(view_ptr, L_FORCE_TIP, L_HAND_CTR, Force_red, Force_green, Force_blue, Force_red, Force_green, Force_blue);
      }
      if (view_ptr->GetDocument()->getHandLoadsRef().TrueRightMagnitude() > 0)
      {
         DrawJointForces(view_ptr, R_FORCE_TIP, R_HAND_CTR, Force_red, Force_green, Force_blue, Force_red, Force_green, Force_blue);
      }
      for(int j = 0; j < EXT3D_NUM_JOINTS; j++)
      {
         // Checks to see if the forces are not zero
         if ( docAForce[JOINT_FORCES][ArrayIndex[j]][2] != 0 ||  docAForce[JOINT_FORCES][ArrayIndex[j]][1] != 0 
            ||  docAForce[JOINT_FORCES][ArrayIndex[j]][3] != 0)
         {
            // Draws the joint forces the y value is negative so that the forces will draw correctly 
            // Also take note that the coordinate system is different.  
            SelectionCode[j+SELECTION_OFFSET] = (int)GetMSForceState((JointTorqueEnm)ArrayIndex[j]);
            DrawJointForces(view_ptr, GetJointCtrlPoint((JointTorqueEnm)ArrayIndex[j]),GetJointForcePoint((JointTorqueEnm)ArrayIndex[j]),  docAForce[JOINT_FORCES][ArrayIndex[j]][2], 
               -( docAForce[JOINT_FORCES][ArrayIndex[j]][1]),  docAForce[JOINT_FORCES][ArrayIndex[j]][3], Force_red, Force_green, Force_blue, Force_red, Force_green, Force_blue);
         } else {
            SelectionCode[j+SELECTION_OFFSET] = (int)MS_NULL;
         }
      }
   }
}

void GLSwapBuffers()
{
   SwapBuffers(g_ActiveContext->gl_hDC);
}

Flt_3D_Point AdjustedForceEndPoint(Flt_3D_Point ForceEndPoint, Flt_3D_Point HandCenter)
{
   // DAC - The vertices given in WorldPoints provide the R_FORCE_TIP/L_FORCE_TIP vertices
   // and the R_HAND_CTR/L_HAND_CTR vertices. The *_FORCE_TIP vertices, however, are
   // not on the appropriate side of the hand. The force vector formed by these the 
   // *_HAND_CTR and *_FORCE_TIP goes FROM the hand into the direction of *_FORCE_TIP.
   // We are used to seeing the force vector go from some force end point TO the hand,
   // so the code here does just that.

   double x,y,z;
   Flt_3D_Point return_val;

   // Get the vector from HAND_CTR to FORCE_TIP ...
   x = ForceEndPoint(0) - HandCenter(0);
   y = ForceEndPoint(1) - HandCenter(1);
   z = ForceEndPoint(2) - HandCenter(2);	

   // Then subtract it from HAND_CTR to get the actual(more logical) force endpoint
   return_val(0) = HandCenter(0) - x;
   return_val(1) = HandCenter(1) - y;
   return_val(2) = HandCenter(2) - z;

   return(return_val);
}

void DrawHead(E_View_Type view_type)
{
   Flt_3D_Polyline WorldPoints = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name ).Get_Points();
   C_Hom_Doc* Doc_Ptr = Get_App_Ptr()->Get_Document_Ptr();
   glPushMatrix();
   glLineWidth(1.0);
   gluQuadricDrawStyle(q, GLU_SILHOUETTE);
   glTranslated(WorldPoints(HEAD_CTR)(0), WorldPoints(HEAD_CTR)(1), WorldPoints(HEAD_CTR)(2));
   switch(view_type)
   {
   case VT_FRONT:
      glRotatef(90, 0.0f, 1.0f, 0.0f);
      break;
   case VT_SIDE:
      glRotatef(90, 1.0f, 0.0f, 0.0f);
      break;
   case VT_TOP:	// don't need rotation
   default:
      break;
   }

   C_Hom_Doc* lDocPtr = Get_App_Ptr()->Get_Document_Ptr();

   gluDisk(q,
      1692.3/*110.0f*/ * Doc_Ptr->getAnthropometryRef().GetHeight(EnglishCns)
      / HEIGHT_FACTOR * lDocPtr->GetFactors().mLink_HeadRadius,
      1846.2/*120.0f*/ * Doc_Ptr->getAnthropometryRef().GetHeight(EnglishCns)
      / HEIGHT_FACTOR * lDocPtr->GetFactors().mLink_HeadRadius,
      32, 32);
   glPopMatrix();
   gluQuadricDrawStyle(q, GLU_FILL);
}

void DrawForces(C_Hom_View * view_ptr)
{
   C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
   // Variables to access the individual WorldPoints
   Flt_3D_Point right_force_end_point, left_force_end_point;
   Flt_3D_Polyline WorldPoints = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name ).Get_Points();

   right_force_end_point = AdjustedForceEndPoint(WorldPoints(R_FORCE_TIP), WorldPoints(R_HAND_CTR));
   left_force_end_point  = AdjustedForceEndPoint(WorldPoints(L_FORCE_TIP), WorldPoints(L_HAND_CTR));

   // Assign values to the point3d's (easier to work with than "Flt_3D_Point"s)
   point3d r_end_point(right_force_end_point(0), right_force_end_point(1), right_force_end_point(2)), 
      l_end_point(left_force_end_point(0), left_force_end_point(1), left_force_end_point(2)),
      r_hand_point(WorldPoints(R_HAND_CTR)(0),WorldPoints(R_HAND_CTR)(1),WorldPoints(R_HAND_CTR)(2)),
      l_hand_point(WorldPoints(L_HAND_CTR)(0),WorldPoints(L_HAND_CTR)(1),WorldPoints(L_HAND_CTR)(2));

   // Draw the end control points
   glPushMatrix();
   // Check to see if its in 2D Mode
   if (!pDoc->get2DMode())
   {
      glTranslated(r_end_point[0], r_end_point[1], r_end_point[2]);
      gluSphere(q, POINT_RADIUS * pDoc->getAnthropometryRef().GetHeight(EnglishCns)/  HEIGHT_FACTOR, 8, 8);
   }
   else 
   {
      DrawNonSelectableVertex(view_ptr, right_force_end_point, false);
   }
   glPopMatrix();
   glPushMatrix();
   if (!pDoc->get2DMode())
   {
      glTranslated(l_end_point[0], l_end_point[1], l_end_point[2]);
      gluSphere(q, POINT_RADIUS * pDoc->getAnthropometryRef().GetHeight(EnglishCns)/  HEIGHT_FACTOR, 8, 8);
   }
   else
   {
      DrawNonSelectableVertex(view_ptr, left_force_end_point, false);
   }
   glPopMatrix();


   // Draw the lines
   glLineWidth(1.0);
   glBegin(GL_LINES);
   glVertex3dv( r_end_point );
   glVertex3dv( r_hand_point );
   glVertex3dv( l_end_point );
   glVertex3dv( l_hand_point );
   glEnd();


   //
   // Draw the arrow heads by drawing a 2d triangle based on the view type
   //

   point3d r_vector(0.0, 0.0, 0.0),		// holds the 2d vector of the force
      l_vector(0.0, 0.0, 0.0);
   point3d perpendicular(0.0, 0.0, 0.0);	// contains the vector coming out of the screen to 
   // perform the cross product
   point3d r_cross, l_cross;				// holds reuslt of the cross product
   // ...
   switch(view_ptr->View_Type)
   {
   case VT_FRONT:
      perpendicular[0] = 1.0f;
      // Discard x value(leave it at 0.0)
      r_vector[1] = r_end_point[1] - r_hand_point[1];
      r_vector[2] = r_end_point[2] - r_hand_point[2];
      l_vector[1] = l_end_point[1] - l_hand_point[1];
      l_vector[2] = l_end_point[2] - l_hand_point[2];
      break;
   case VT_SIDE:
      perpendicular[1] = -1.0f;
      // Discard y value(leave it at 0.0)
      r_vector[0] = r_end_point[0] - r_hand_point[0];
      r_vector[2] = r_end_point[2] - r_hand_point[2];
      l_vector[0] = l_end_point[0] - l_hand_point[0];
      l_vector[2] = l_end_point[2] - l_hand_point[2];
      break;
   case VT_TOP:
      perpendicular[2] = 1.0f;
      // Discard z value(leave it at 0.0)
      r_vector[0] = r_end_point[0] - r_hand_point[0];
      r_vector[1] = r_end_point[1] - r_hand_point[1];
      l_vector[0] = l_end_point[0] - l_hand_point[0];
      l_vector[1] = l_end_point[1] - l_hand_point[1];
      break;
   }

   // Use cross product to get vector perpendicular to the
   // force vector on the viewing plane so we can find the other two
   // points of the triangle(the first point on the triangle is the 
   // position of the hand)

   // For some stupid reason, "cross = r_vector * perpendicular" doesn't work right,
   // so calculate the cross product here
   r_cross[0] = r_vector[1]*perpendicular[2] - r_vector[2]*perpendicular[1];
   r_cross[1] = r_vector[2]*perpendicular[0] - r_vector[0]*perpendicular[2];
   r_cross[2] = r_vector[0]*perpendicular[1] - r_vector[1]*perpendicular[0];

   l_cross[0] = l_vector[1]*perpendicular[2] - l_vector[2]*perpendicular[1];
   l_cross[1] = l_vector[2]*perpendicular[0] - l_vector[0]*perpendicular[2];
   l_cross[2] = l_vector[0]*perpendicular[1] - l_vector[1]*perpendicular[0];

   r_cross.normalize();
   l_cross.normalize();

   // Start the arrow head 8/10ths of the way up the vector
   point3d r_triangle_base = r_end_point - (r_vector * ARROW_STARTING_PERC);
   point3d l_triangle_base = l_end_point - (l_vector * ARROW_STARTING_PERC)
      ;
   // Add the cross product to the base to find the other two triangle points
   point3d r_triangle_point[] = { r_triangle_base + (r_cross * 25), r_triangle_base - (r_cross * 25) };
   point3d l_triangle_point[] = { l_triangle_base + (l_cross * 25), l_triangle_base - (l_cross * 25) };

   // Draw the arrowheads as triangles
   glBegin(GL_TRIANGLES);
   glVertex3dv( r_hand_point );
   glVertex3dv( r_triangle_point[0] );
   glVertex3dv( r_triangle_point[1] );

   glVertex3dv( l_hand_point );
   glVertex3dv( l_triangle_point[0] );
   glVertex3dv( l_triangle_point[1] );
   glEnd();
}

void DrawNonSelectableVertex(C_Hom_View * view_ptr, Flt_3D_Point vertex, bool color) 
{
   // When color is true set the interior circle to the background color
   C_Hom_Doc* Doc_Ptr = Get_App_Ptr()->Get_Document_Ptr();
   Flt_3D_Polyline WorldPoints = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name ).Get_Points();

   glPushMatrix();
   glLineWidth(1.0);
   glTranslated(vertex(0), vertex(1), vertex(2));
   RotateView(view_ptr);
   if (color == false)
   {
      gluDisk(q, POINT_RADIUS-10, POINT_RADIUS, 32, 32);

   } else {
      //Check to see if the floor is below this point
         glColor3ub(255,255,145);
      //}
      gluSphere(q, POINT_RADIUS-10 * Doc_Ptr->getAnthropometryRef().GetHeight(EnglishCns)/  HEIGHT_FACTOR, SPHERE_LONGITUDE, SPHERE_LATITUDE);
   }
   glPopMatrix();
   SetLineWidth(view_ptr);
}

void DrawVertices(C_Hom_View * view_ptr)
{
   int i;
   Flt_3D_Polyline WorldPoints = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name ).Get_Points();
   Flt_3D_Point currentPoint;
   C_Hom_Doc* Doc_Ptr = Get_App_Ptr()->Get_Document_Ptr();
   for (i=0; i<27;i++)	// Cycle through all 36 points 
   {
      if(DisplayPoint[i])		// see if point is displayable
      {
         currentPoint = WorldPoints(i);

         if(SelectionCode[i] == MS_NULL)
         {
            // Draw a non-selectable vertex
            DrawNonSelectableVertex(view_ptr, currentPoint, false);
         }
         else
         {
            // Draw a selectable vertex

            glPushMatrix();
            glTranslated(currentPoint(0), currentPoint(1), currentPoint(2));
            gluSphere(
               q,
               POINT_RADIUS * Doc_Ptr->getAnthropometryRef().GetHeight(EnglishCns) /  HEIGHT_FACTOR,
               SPHERE_LONGITUDE,
               SPHERE_LATITUDE);
            glPopMatrix();

         }
      }
   }
  // glEnd();
}

void DrawFeet(C_Hom_View * view_ptr)
{
   C_Hom_Doc* docPtr = Get_App_Ptr()->Get_Document_Ptr();
   Flt_3D_Polyline WorldPoints = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name ).Get_Points();
   // Lengths only go to the ball of the foot
   double rightFootLength = WorldPoints(R_BALL_FOOT)(0) - WorldPoints(R_HEEL)(0);
   double leftFootLength = WorldPoints(L_BALL_FOOT)(0) - WorldPoints(L_HEEL)(0);
   glBegin(GL_LINES);
        glVertex3d(WorldPoints(R_HEEL)(0),WorldPoints(R_HEEL)(1),WorldPoints(R_HEEL)(2));		// JTK  -1 subtracted to get the foot and ankle to line up
        glVertex3d(WorldPoints(R_HEEL)(0)+rightFootLength,WorldPoints(R_HEEL)(1),WorldPoints(R_HEEL)(2));
        
        glVertex3d(WorldPoints(R_HEEL)(0)+rightFootLength,WorldPoints(R_HEEL)(1),WorldPoints(R_HEEL)(2));
        glVertex3d(WorldPoints(R_ANKLE)(0), WorldPoints(R_ANKLE)(1), WorldPoints(R_ANKLE)(2));

        glVertex3d(WorldPoints(L_HEEL)(0),WorldPoints(L_HEEL)(1),WorldPoints(L_HEEL)(2));
        glVertex3d(WorldPoints(L_HEEL)(0)+leftFootLength,WorldPoints(L_HEEL)(1),WorldPoints(L_HEEL)(2));

        glVertex3d(WorldPoints(L_HEEL)(0)+leftFootLength,WorldPoints(L_HEEL)(1),WorldPoints(L_HEEL)(2));
        glVertex3d(WorldPoints(L_ANKLE)(0), WorldPoints(L_ANKLE)(1), WorldPoints(L_ANKLE)(2));
   glEnd();
   DrawEndsOfFeet(view_ptr, L_HEEL);
   DrawEndsOfFeet(view_ptr, R_HEEL);
}

void SetRectangle(C_Hom_View * view_ptr)
{
   C_Hom_Doc*  Doc_Ptr = Get_App_Ptr()->Get_Document_Ptr();
   glLineWidth(1.0);
   SetStance(Doc_Ptr, view_ptr->View_Type);
   // Reset Line Width
   SetLineWidth(view_ptr);
} 

void DrawRectangle(E_Points heel, E_View_Type view_type, bool drawOutline)
// This draws each individual floor.  It combines the floor when the two ankles are within three world points of each other.
{
   C_Hom_Doc* Doc_Ptr = Get_App_Ptr()->Get_Document_Ptr();
   Flt_3D_Polyline WorldPoints = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name ).Get_Points();
   int side = 0;
   int view = 2;		// View = 2 for VT_FRONT for VT_TOP = 0 Numbers correspond to WorldPoints Indexes
   double upper, lower, upperOpp, lowerOpp;
   E_Points oppHeel;
   // Selects which viewing angle
   if (view_type == VT_TOP)
   {
      view = 0;
   }
   if (heel == L_FOOT_CENTER)
   {
      oppHeel = R_FOOT_CENTER;
   } else {
      oppHeel = L_FOOT_CENTER;
   }
   upper = (WorldPoints(heel)(view) + UPPER_OFFSET);
   lower = (WorldPoints(heel)(view) - LOWER_OFFSET);
   upperOpp = (WorldPoints(oppHeel)(view) + UPPER_OFFSET);
   lowerOpp = (WorldPoints(oppHeel)(view) - LOWER_OFFSET);
   // Define the coordinates
   double x[9] = {	CENTER_OFFSET,									// Left Diagonal
      0,
      (WorldPoints(heel)(1)*BOX_SIZE) +(CENTER_OFFSET),
      WorldPoints(heel)(1)*BOX_SIZE,					// right diagnol
      CENTER_OFFSET,
      (WorldPoints(heel)(1)*BOX_SIZE)
      +(CENTER_OFFSET),// Top Line
      0,
      WorldPoints(heel)(1)*BOX_SIZE,
      CENTER_OFFSET	
   };// Bottom Line
   double z[9] = {	upper,
      lower,
      upper,
      lower,
      upper,
      upper,
      lower,
      lower,
      upper};
   double connectorsZ[9] = {      lower, 
      lowerOpp, 
      upperOpp,
      upper,   
      lowerOpp,
      upperOpp,
      lower, 
      upper,
      upper
   };

   double specConnectorsX[9] = { WorldPoints(heel)(1)*BOX_SIZE,					// right diagnol
      WorldPoints(oppHeel)(1)*BOX_SIZE,
      (WorldPoints(oppHeel)(1)*BOX_SIZE) +(CENTER_OFFSET),
      (WorldPoints(heel)(1)*BOX_SIZE) +(CENTER_OFFSET),
      WorldPoints(oppHeel)(1)*BOX_SIZE,
      (WorldPoints(oppHeel)(1)*BOX_SIZE) +(CENTER_OFFSET),
      WorldPoints(heel)(1)*BOX_SIZE,
      (WorldPoints(heel)(1)*BOX_SIZE) +(CENTER_OFFSET),
      (WorldPoints(heel)(1)*BOX_SIZE) +(CENTER_OFFSET)	};					// right diagnol

   glColor3ub(255,255,255);
   if (drawOutline)
   {
      glColor3ub(0,0,0);
      glBegin(GL_LINES);
   }
   else
   {
      glBegin(GL_POLYGON);
   }

   if (view_type == VT_FRONT)
   {
      // Draw the object
      for (int i = 0; i < 9; i++)
      {
         if (AreFeetLevel() && (i == 0 && drawOutline))
            i+= 2;

         glVertex3d(0, x[i], z[i]);
      }
   }
   else if (view_type == VT_SIDE)
   {
      for (int i = 0; i < 8; i++)
      {
         if(heel == R_FOOT_CENTER && (!AreFeetLevel()))
         {
            x[i] += 100;
         } else if(!AreFeetLevel()){
            //x[i] += 100;
         }
         // Rotate Plane
         if (AreFeetLevel() && (i == 0 && drawOutline && WorldPoints(L_FOOT_CENTER)(Y_COORD) - WorldPoints(R_FOOT_CENTER)(Y_COORD) >= 0))
         {
            i+= 2;
         } else if (AreFeetLevel() && (i == 2 && drawOutline && WorldPoints(L_FOOT_CENTER)(Y_COORD) - WorldPoints(R_FOOT_CENTER)(Y_COORD) < 0))
         {
            i += 2;
         }
         x[i] = x[i] + WorldPoints(heel)(0) - WorldPoints(heel)(1);
       
         glVertex3d(x[i],0, z[i]);	
      }
      if (AreFeetLevel())
      {
         for (int i = 0; i < 4; i++) 
         {
            if (i == 2 || i == 1)
               specConnectorsX[i] = specConnectorsX[i] + (WorldPoints(oppHeel)(0) - WorldPoints(oppHeel)(1));
            else 
               specConnectorsX[i] = specConnectorsX[i] + (WorldPoints(heel)(0) - WorldPoints(heel)(1));
            glVertex3d(specConnectorsX[i],0, connectorsZ[i]);
         }
      }
   } else if (view_type == VT_TOP) {
      // Make the object a square
      x[0] -= CENTER_OFFSET;
      x[2] -= CENTER_OFFSET;
      x[4] -= CENTER_OFFSET;
      x[5] -= CENTER_OFFSET; 

      if (AreFeetLevel()) {
         view = 2;
         E_Points temp;
         if(WorldPoints(L_FOOT_CENTER)(0) - WorldPoints(R_FOOT_CENTER)(0) < 0)
         {
            temp = L_FOOT_CENTER;
         } else {
            temp = R_FOOT_CENTER;
         }
         /*if (heel == R_FOOT_CENTER && WorldPoints(L_FOOT_CENTER)(view) - WorldPoints(R_FOOT_CENTER)(view) < 0) {
            glVertex3d(z[2], x[2], 0);
            glVertex3d(z[2], (WorldPoints(oppHeel)(1)*BOX_SIZE), 0);
            glVertex3d(z[2], (WorldPoints(oppHeel)(1)*BOX_SIZE), 0);
            glVertex3d(lowerOpp , (WorldPoints(oppHeel)(1)*BOX_SIZE), 0);
            glVertex3d(lowerOpp, (WorldPoints(oppHeel)(1)*BOX_SIZE), 0);
            glVertex3d(lowerOpp, x[2], 0);
            glVertex3d(lowerOpp, x[2], 0);
            glVertex3d(z[2], x[2], 0);
         } else */if (heel == temp) {
            glVertex3d(z[3], x[2], 0);
            glVertex3d(z[3], (WorldPoints(oppHeel)(1)*BOX_SIZE), 0);
            glVertex3d(z[3], (WorldPoints(oppHeel)(1)*BOX_SIZE), 0);
            glVertex3d(upperOpp , (WorldPoints(oppHeel)(1)*BOX_SIZE), 0);
            glVertex3d(upperOpp, (WorldPoints(oppHeel)(1)*BOX_SIZE), 0);
            glVertex3d(upperOpp, x[2], 0);
            glVertex3d(upperOpp, x[2], 0);
            glVertex3d(z[3], x[2], 0);
         }
      } else {
         for (int i = 0; i < 8; i++)
         {
            glVertex3d(z[i], x[i], 0);
         }
      }
   }
   glEnd();
   SetColor();
   return;
}

void DrawSkeleton(C_Hom_View * view_ptr)
{
   int i,j;
   Flt_3D_Polyline WorldPoints = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name ).Get_Points();
   Flt_3D_Point currentPoint;

   for (i=0;i<MAX_SKEL_POLYLINES;i++)
   {
      glBegin(GL_LINE_STRIP);
      for (j=0;j<VERTS_PER_SKEL_POLYLINE;j++)
      {
         currentPoint = WorldPoints(lines[i][j]);
         glVertex3d(currentPoint(0), currentPoint(1), currentPoint(2));
      }
      glEnd();
   }
   DrawFeet(view_ptr);
}

// Returns the absolute(positive) distance between the farthest point in the skeleton 
// from the origin AND the origin based on the view
float MaxDistanceFromOrigin(C_Hom_View *view_ptr)
{
   int i,j;
   float max = 0.0f;
   Flt_3D_Polyline WorldPoints = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name ).Get_Points();
   Flt_3D_Point currentPoint;
   int axes[3];		// Contains the axes to check. x = 0, y = 1, z = 2

   switch(view_ptr->View_Type)
   {
   case VT_TOP:
      // make x and y are between -world_size and world_size
      axes[0] = 0;
      axes[1] = 1;		// check x and y axes
      break;
   case VT_FRONT:
      // make y and z are between -world_size and world_size
      axes[0] = 1;
      axes[1] = 2;		// check x and y axes
      break;
   case VT_SIDE:
      // make x and z are between -world_size and world_size
      axes[0] = 0;
      axes[1] = 2;		// check x and y axes
      break;
   default:
      return 0.0f;
      //		axes[0] = 0;
      //		axes[1] = 0;		// doesn't matter what these are ... 
   }

   // Also check modified force angles, since they're not enabled in DisplayPoint[i]
   Flt_3D_Point right_force_end_point, left_force_end_point;

   // Adjust the WorldPoint to make force point down
   right_force_end_point = AdjustedForceEndPoint(WorldPoints(R_FORCE_TIP), WorldPoints(R_HAND_CTR));
   left_force_end_point  = AdjustedForceEndPoint(WorldPoints(L_FORCE_TIP), WorldPoints(L_HAND_CTR));

   for(j=0; j<2; j++)
   {
      if(abs(right_force_end_point(axes[j])) > max)
         max = float(abs(right_force_end_point(axes[j])));
      if(abs(left_force_end_point(axes[j])) > max)
         max = float(abs(left_force_end_point(axes[j])));
   }
   // Check the ones that are visible
   for(i=0; i<36; i++)
   {
      currentPoint = WorldPoints(i);
      for(j=0; j<2; j++)	// go through each component
         if(abs(currentPoint(axes[j])) > max)
            max = float(abs(currentPoint(axes[j])));

   }
   return max;
}

float FarthestDistanceFromView(C_Hom_View *view_ptr)
{
   int i,j;
   float distance, max = 0.0f;
   float world_size = view_ptr->mWorldSize;
   Flt_3D_Polyline WorldPoints = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name ).Get_Points();
   Flt_3D_Point currentPoint;
   int axes[2];		// Contains the axes to check. x = 0, y = 1, z = 2

   switch(view_ptr->View_Type)
   {
   case VT_TOP:
      // make x and y are between -world_size and world_size
      axes[0] = 0;
      axes[1] = 1;		// check x and y axes
      break;
   case VT_FRONT:
      // make y and z are between -world_size and world_size
      axes[0] = 1;
      axes[1] = 2;		// check x and y axes
      break;
   case VT_SIDE:
      // make x and z are between -world_size and world_size
      axes[0] = 0;
      axes[1] = 2;		// check x and y axes
      break;
   }

   // Also check modified force angles, since they're not enabled in DisplayPoint[i]
   Flt_3D_Point right_force_end_point, left_force_end_point;
   right_force_end_point = AdjustedForceEndPoint(WorldPoints(R_FORCE_TIP), WorldPoints(R_HAND_CTR));
   left_force_end_point  = AdjustedForceEndPoint(WorldPoints(L_FORCE_TIP), WorldPoints(L_HAND_CTR));

   for(j=0; j<2; j++)
   {
      if((right_force_end_point(axes[j]) < -world_size) || (right_force_end_point(axes[j]) > world_size))
      {
         distance = float(abs(right_force_end_point(j) - world_size));
         if(distance > max)
            max = distance;
      }
      if((left_force_end_point(axes[j]) < -world_size) || (left_force_end_point(axes[j]) > world_size))
      {
         distance = float(abs(left_force_end_point(axes[j]) - world_size));
         if(distance > max)
            max = distance;
      }
   }

   // Check the ones that are visible
   for(i=0; i<36; i++)
   {
      if(DisplayPoint[i])
      {
         currentPoint = WorldPoints(i);
         for(j=0; j<2; j++)	// go through each component
            if( (currentPoint(axes[j]) < -world_size) || (currentPoint(axes[j]) > world_size) )
            {
               distance = float(abs(currentPoint(axes[j]) - world_size));
               if(distance > max)
                  max = distance;
            }
      }
   }
   return max;
}

float FarthestDistanceFromViewingVolume(float world_size)
{
   int i,j;
   float distance, max = 0.0f;
   Flt_3D_Polyline WorldPoints = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name ).Get_Points();
   Flt_3D_Point currentPoint;

   // Also check modified force angles, since they're not enabled in DisplayPoint[i]
   Flt_3D_Point right_force_end_point, left_force_end_point;
   right_force_end_point = AdjustedForceEndPoint(WorldPoints(R_FORCE_TIP), WorldPoints(R_HAND_CTR));
   left_force_end_point  = AdjustedForceEndPoint(WorldPoints(L_FORCE_TIP), WorldPoints(L_HAND_CTR));

   for(j=0; j<3; j++)
   {
      if((right_force_end_point(j) < -world_size) || (right_force_end_point(j) > world_size))
      {
         distance = float(abs(right_force_end_point(j) - world_size));
         if(distance > max)
            max = distance;
      }
      if((left_force_end_point(j) < -world_size) || (left_force_end_point(j) > world_size))
      {
         distance = float(abs(left_force_end_point(j) - world_size));
         if(distance > max)
            max = distance;
      }
   }

   // Check the ones that are visible
   for(i=0; i<36; i++)
   {
      if(DisplayPoint[i])
      {
         currentPoint = WorldPoints(i);
         for(j=0; j<3; j++)	// go through each component
            if( (currentPoint(j) < -world_size) || (currentPoint(j) > world_size) )
            {
               distance = float(abs(currentPoint(j) - world_size));
               if(distance > max)
                  max = distance;
            }
      }
   }

   return max;
}

void CalculateViewportSizeToRatio(int & GL_viewport_size_x, int & GL_viewport_size_y, const CRect & windowRect, C_Hom_View* view_ptr)
{
   float ratio, height, width;
   C_Hom_Doc* Doc_Ptr = view_ptr->GetDocument();
   height = float(windowRect.Height());
   width = float(windowRect.Width());

   if (height == 0)
   {
      height = 1;	// avoid dividing by zero
   }

   ratio = width/height;
   if	(ratio < WIDTH_HEIGHT_RATIO)	// width is smaller than allowed by ratio, set viewport based on width
   {
      GL_viewport_size_x = windowRect.Width();
      GL_viewport_size_y = int((1/WIDTH_HEIGHT_RATIO) * windowRect.Width());
   }
   else 	// height is smaller than allowed by ratio, set viewport based on height
   {
      GL_viewport_size_x = int(WIDTH_HEIGHT_RATIO * windowRect.Height());
      GL_viewport_size_y = windowRect.Height();
   }
}

void SetViewport(C_Hom_View * view_ptr)
{
   // Set the view port (width-height ratio must be WIDTH_HEIGHT_RATIO)
   // Do this so the model's scale doesn't get distorted on resizes
   int GL_viewport_size_x, GL_viewport_size_y;
   C_Hom_Doc* pDoc = view_ptr->GetDocument();

   CRect windowRect;
   view_ptr->GetWindowRect(&windowRect);

   switch(view_ptr->View_Type)	// Set the camera based on the view
   {
   case VT_TOP:
   case VT_FRONT:
   case VT_SIDE:
      if(windowRect.Height() > 0)				// make sure we don't divide by zero
      {
         CalculateViewportSizeToRatio(GL_viewport_size_x, GL_viewport_size_y, windowRect, view_ptr);

         // Center the Viewport in the window
         int center_x, center_y;
         center_x = (windowRect.Width()/2);
         center_y = (windowRect.Height()/2);
         glViewport(	center_x - GL_viewport_size_x/2,
            center_y - GL_viewport_size_y/2,
            GL_viewport_size_x,
            GL_viewport_size_y
            );
      }
      break;
   case VT_OBLIQUE:
      CalculateViewportSizeToRatio(GL_viewport_size_x, GL_viewport_size_y, windowRect, view_ptr);

      glViewport(0,0,windowRect.Width(),windowRect.Height());

      glCullFace(GL_BACK);
      glEnable(GL_CULL_FACE);

      break;
   }
}


void SetCamera(C_Hom_View * view_ptr)
{
   GLdouble m[16];
   // Reset The Current Modelview Matrix
   glMatrixMode(GL_MODELVIEW);
   //glLoadIdentity();	JTK - added SetMatrix to fix the cutting issue with the top view May 07, 2004
   SetMatrix(view_ptr,m);
   // Hominoid is standing on a plane parallel to the xy plane, facing in the positive x direction
   glLoadMatrixd(m);
   switch(view_ptr->View_Type)	// Set the camera based on the view
   {
   case VT_OBLIQUE:
      break;
   case VT_FRONT:
      gluLookAt(	LOOK_AT_DISTANCE, 0.0f, 0.0f,
         0.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 1.0f);
      break;
   case VT_SIDE:	// right side
      gluLookAt(	0.0f, -LOOK_AT_DISTANCE, 0.0f,
         0.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 1.0f);
      break;
   case VT_TOP:
      gluLookAt(	0.0f, 0.0f, LOOK_AT_DISTANCE,
         0.0f, 0.0f, 0.0f,
         -1.0f, 0.0f, 0.0f);
      break;
   case VT_STATUS:
   case VT_REPORT:
   case VT_BAL:
      break;
   }
}

int GetTargetRectSize(const CRect & windowRect)
{
   double ratio;

   if(windowRect.Height() > 0)						// prevent divide by zero
      ratio = double(windowRect.Width()) / double(windowRect.Height());
   else
      ratio = double(windowRect.Width());

   if(ratio < WIDTH_HEIGHT_RATIO )
   {
      // Set the TargetRectSize based on the width, since is doesn't scale with the viewport 
      //glLineWidth((int)windowRect.Width()/150);
      return windowRect.Width() / 70;
   }
   else
   { 
      // Set the TargetRectSize based on the height
      return windowRect.Height() / 70;
   }
}

void SetLineWidth(C_Hom_View * view_ptr)
{
   double ratio;
   C_Hom_Doc* pDoc = view_ptr->GetDocument();
   CRect windowRect;
   view_ptr->GetWindowRect(&windowRect);			// Set line width and color

   if(windowRect.Height() > 0)						// prevent divide by zero
      ratio = windowRect.Width() / windowRect.Height();
   else
   {
      glLineWidth(windowRect.Width() / 150.0f);
      return;	// return to avoid using an undefined "ratio"
   }

   if	(ratio < WIDTH_HEIGHT_RATIO)
   {
      // Set the linewidth based on the width, since is doesn't scale with the viewport 
      glLineWidth(windowRect.Width() / LINE_WIDTH_DIVISOR);
   }
   else
   {
      // Set the linewidth based on the height
      glLineWidth(windowRect.Height() / LINE_WIDTH_DIVISOR);
   }
}

void SetColor()
{
    glColor3ub(132,93,90);
}

void SetProjectionMatrix(C_Hom_View *view_ptr)
{
   // This will change w/ the oblique view, so leave it here
   // Use orthographic projection
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // This should encapsulate the whole wireframe, and a little more so the
   // points are exactly at the edge of the viewport

   float WorldSize = MaxDistanceFromOrigin(view_ptr);
   switch(view_ptr->View_Type)
   {
   case VT_TOP:
      //view_ptr->mWorldSize = TOP_VIEW_BORDER*WorldSize;  Multiplying by the TOP_VIEW_BORDER 
      // appears to cause the click-and-drag of a joint to be off.
      view_ptr->mWorldSize = TOP_VIEW_BORDER * WorldSize;
      //view_ptr->mWorldSize = TOP_VIEW_BORDER*WorldSize; 
      // DAC - The top view will be too big compared to the other two views because
      // the all the points can be encapsulated in a smaller world size than the other views.
      // Therefore, it is necessary to set a minimum world size to make the the top view appear
      // to be a size comparable to the other views.
      if(view_ptr->mWorldSize < MINIMUM_TOP_VIEW_WORLD_SIZE)
         view_ptr->mWorldSize = MINIMUM_TOP_VIEW_WORLD_SIZE;
      break;
   case VT_FRONT:
      view_ptr->mWorldSize = FRONT_VIEW_BORDER*WorldSize;
      break;
   case VT_SIDE:
      view_ptr->mWorldSize = SIDE_VIEW_BORDER*WorldSize;
      break;
   case VT_OBLIQUE:
      view_ptr->mWorldSize = ORIGINAL_WORLD_SIZE;
      break;
   }
   switch(view_ptr->View_Type)
   {
   case VT_TOP:
   case VT_FRONT:
   case VT_SIDE:
      glOrtho(-view_ptr->mWorldSize, view_ptr->mWorldSize, 
         -view_ptr->mWorldSize, view_ptr->mWorldSize, 
         -view_ptr->mWorldSize, view_ptr->mWorldSize);
      break;
   case VT_OBLIQUE:
      {
         CRect windowRect;
         view_ptr->GetWindowRect(&windowRect);

         double fov = 45.0;

         double ratio = windowRect.Width() / double(windowRect.Height());	// Changed from Height
         double aspect = windowRect.Width() / double(windowRect.Height());
         // JTK - Hack so that the hominoid doesn't get larger as the window gets wider.
         // Also fixes it so the hominoid doesn't flip over when the width is significantly less than the height.
         if (ratio > 0.67721522)
         {
            ratio = 0.67721522;		// This is so the object doesn't get smaller as the window size changes
         }
         if (windowRect.Height() > windowRect.Width())
         {
            // Check to make sure the height does not get to big
            if (ratio < 0.253)
            {
               ratio = 0.253;
            }
         }
         double fovy = (fov / ratio) * (180.0f / M_PI);
         if (aspect < 0.001)
         {
            aspect = 0.001;
         }
         if (fovy > 180.0)
         {
            fovy = 180.0;
         }
         gluPerspective(fovy,aspect,0.1f,4096.0f);  // Set the frustum
      }
      break;
   }
}

void SetPoint(int lockingPoint, int selection)
{ 
   SelectionCode[lockingPoint] = selection; 
}

int GetPoint(int point)
{
   return SelectionCode[point];
}

void DrawBackgroundColorVertex(C_Hom_View *view_ptr)
{
   int i;
   Flt_3D_Polyline WorldPoints = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name ).Get_Points();
   Flt_3D_Point currentPoint;

   for (i=0; i<27;i++)	// Cycle through all 47 points 
   {
      if(DisplayPoint[i])		// see if point is displayable
      {
         currentPoint = WorldPoints(i);

         if(SelectionCode[i] == MS_NULL && (i != 5 || view_ptr->View_Type != VT_SIDE))
         {
            // Draw a non-selectable vertex
            DrawNonSelectableVertex(view_ptr, currentPoint, true);

         }
      }
   }
   //glPopAttrib();
}
void SetStance(C_Hom_Doc* Doc_Ptr, E_View_Type view_type)
{
   if (Doc_Ptr->getStance() == STANCE_BOTH_FEET)
   {
      DrawRectangle(R_FOOT_CENTER, view_type, false);
      DrawRectangle(L_FOOT_CENTER, view_type, false);
      DrawRectangle(L_FOOT_CENTER, view_type, true);
      DrawRectangle(R_FOOT_CENTER, view_type, true);
   }
   else if (Doc_Ptr->getStance() == STANCE_LEFT_FOOT)
   {
      DrawRectangle(L_FOOT_CENTER, view_type, false);
      DrawRectangle(L_FOOT_CENTER, view_type, true);
   }
   else if (Doc_Ptr->getStance() == STANCE_RIGHT_FOOT)
   {
      DrawRectangle(R_FOOT_CENTER, view_type, false);
      DrawRectangle(R_FOOT_CENTER, view_type, true);
   }
}
void DrawJointForces(C_Hom_View * view_ptr, E_Points ForceTip, E_Points JointCtrl, BYTE rArrow, BYTE gArrow, BYTE bArrow,
                     BYTE rSphere, BYTE gSphere, BYTE bSphere)
{

   C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
   // Variables to access the individual WorldPoints
   Flt_3D_Point force_end_point;
   Flt_3D_Polyline WorldPoints = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name ).Get_Points();

   force_end_point = AdjustedForceEndPoint(WorldPoints(ForceTip), WorldPoints(JointCtrl));

   // Assign values to the point3d's (easier to work with than "Flt_3D_Point"s)
   point3d r_end_point(force_end_point(0), force_end_point(1), force_end_point(2)), 
      r_hand_point(WorldPoints(JointCtrl)(0),WorldPoints(JointCtrl)(1),WorldPoints(JointCtrl)(2));

   // Draw the end control points
   glLineWidth(1.0);
   glPushMatrix();

   glColor3f(rSphere/255.0f, gSphere/255.0f, bSphere/255.0f);
   // Check to see if its in 2D Mode 0 = 3d mode 1 = 2d mode
   if (pDoc->get2DMode() == 0)
   {
      glTranslated(r_end_point[0], r_end_point[1], r_end_point[2]);
      gluSphere(q, POINT_RADIUS * pDoc->getAnthropometryRef().GetHeight(EnglishCns)/ HEIGHT_FACTOR, 8, 8);
   }
   else 
   {
      DrawNonSelectableVertex(view_ptr, force_end_point, false);
   }
   glPopMatrix();


   // Draw the lines
   glColor3f(rArrow/255.0f, gArrow/255.0f, bArrow/255.0f);
   glBegin(GL_LINES);
   glVertex3dv( r_end_point );
   glVertex3dv( r_hand_point );
   glEnd();


   //
   // Draw the arrow heads by drawing a 2d triangle based on the view type
   //

   point3d r_vector(0.0, 0.0, 0.0);		// holds the 2d vector of the force
   point3d perpendicular(0.0, 0.0, 0.0);	// contains the vector coming out of the screen to 
   // perform the cross product
   point3d r_cross;				// holds result of the cross product
   // ...
   switch(view_ptr->View_Type)
   {
   case VT_FRONT:
      perpendicular[0] = 1.0f;
      // Discard x value(leave it at 0.0)
      r_vector[1] = r_end_point[1] - r_hand_point[1];
      r_vector[2] = r_end_point[2] - r_hand_point[2];
      break;
   case VT_SIDE:
      perpendicular[1] = -1.0f;
      // Discard y value(leave it at 0.0)
      r_vector[0] = r_end_point[0] - r_hand_point[0];
      r_vector[2] = r_end_point[2] - r_hand_point[2];
      break;
   case VT_TOP:
      perpendicular[2] = 1.0f;
      // Discard z value(leave it at 0.0)
      r_vector[0] = r_end_point[0] - r_hand_point[0];
      r_vector[1] = r_end_point[1] - r_hand_point[1];
      break;
   }

   // Use cross product to get vector perpendicular to the
   // force vector on the viewing plane so we can find the other two
   // points of the triangle(the first point on the triangle is the 
   // position of the hand)

   // For some stupid reason, "cross = r_vector * perpendicular" doesn't work right,
   // so calculate the cross product here
   r_cross[0] = r_vector[1]*perpendicular[2] - r_vector[2]*perpendicular[1];
   r_cross[1] = r_vector[2]*perpendicular[0] - r_vector[0]*perpendicular[2];
   r_cross[2] = r_vector[0]*perpendicular[1] - r_vector[1]*perpendicular[0];

   r_cross.normalize();

   // Start the arrow head 8/10ths of the way up the vector
   point3d r_triangle_base = r_end_point - (r_vector * ARROW_STARTING_PERC);
   // Add the cross product to the base to find the other two triangle points
   point3d r_triangle_point[] = { r_triangle_base + (r_cross * 25), r_triangle_base - (r_cross * 25) };

   // Draw the arrowheads as triangles
   glBegin(GL_TRIANGLES);
   glVertex3dv( r_hand_point );
   glVertex3dv( r_triangle_point[0] );
   glVertex3dv( r_triangle_point[1] );
   glEnd();
}

void DrawJointForces(C_Hom_View * view_ptr,E_Points JointCtrl, E_Points JointForceTip, double x, double y, double z,
                     BYTE rArrow, BYTE gArrow, BYTE bArrow, BYTE rSphere, BYTE gSphere, BYTE bSphere)
{
   C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
   double forceMagnitude;
   float viewBorder;
   // Variables to access the individual WorldPoints
   Flt_3D_Point force_end_point;
   Flt_3D_Polyline WorldPoints = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name ).Get_Points();

   switch(view_ptr->View_Type)
   {
   case VT_TOP:	viewBorder = TOP_VIEW_BORDER;
      break;
   case VT_SIDE:	viewBorder = SIDE_VIEW_BORDER;
      break;
   case VT_FRONT:	viewBorder = FRONT_VIEW_BORDER;
      break;
   default:		viewBorder = SIDE_VIEW_BORDER;	// Arbitrary default (this is in case a new view is added)
      break;
   }
   x *= viewBorder;
   y *= viewBorder;
   z *= viewBorder;
   forceMagnitude = sqrt(x*x  + y * y  + z* z);
   x = x/forceMagnitude * FORCE_LENGTH + WorldPoints(JointCtrl)(0);
   y = y/forceMagnitude * FORCE_LENGTH + WorldPoints(JointCtrl)(1);
   z = z/forceMagnitude * FORCE_LENGTH + WorldPoints(JointCtrl)(2);

   force_end_point = AdjustedForceEndPoint(Flt_3D_Point(x, y, z), WorldPoints(JointCtrl));
   WorldPoints(JointForceTip).Put(force_end_point(0),force_end_point(1), force_end_point(2));

   // Assign values to the point3d's (easier to work with than "Flt_3D_Point"s)
   point3d r_end_point(force_end_point(0), force_end_point(1), force_end_point(2)), 
      r_hand_point(WorldPoints(JointCtrl)(0),WorldPoints(JointCtrl)(1),WorldPoints(JointCtrl)(2));

   // Draw the end control points
   glPushMatrix();
   glColor3f(rSphere/255.0f, gSphere/255.0f, bSphere/255.0f);
   if (pDoc->get2DMode() == 0)
   {
      glTranslated(r_end_point[0], r_end_point[1], r_end_point[2]);
      gluSphere(q, POINT_RADIUS * pDoc->getAnthropometryRef().GetHeight(EnglishCns) / HEIGHT_FACTOR, 8, 8);
   }
   else 
   {
      DrawNonSelectableVertex(view_ptr, force_end_point, false);
   }
   glPopMatrix();

   // Draw the lines
   glLineWidth(1.0);
   glColor3f(rArrow/255.0f, gArrow/255.0f, bArrow/255.0f);
   glBegin(GL_LINES);
   glVertex3dv( r_end_point );
   glVertex3dv( r_hand_point );
   glEnd();


   //
   // Draw the arrow heads by drawing a 2d triangle based on the view type
   //

   point3d r_vector(0.0, 0.0, 0.0);		// holds the 2d vector of the force
   point3d perpendicular(0.0, 0.0, 0.0);	// contains the vector coming out of the screen to 
   // perform the cross product
   point3d r_cross, l_cross;				// holds reuslt of the cross product
   // ...
   switch(view_ptr->View_Type)
   {
   case VT_FRONT:
      perpendicular[0] = 1.0f;
      // Discard x value(leave it at 0.0)
      r_vector[1] = r_end_point[1] - r_hand_point[1];
      r_vector[2] = r_end_point[2] - r_hand_point[2];
      break;
   case VT_SIDE:
      perpendicular[1] = -1.0f;
      // Discard y value(leave it at 0.0)
      r_vector[0] = r_end_point[0] - r_hand_point[0];
      r_vector[2] = r_end_point[2] - r_hand_point[2];
      break;
   case VT_TOP:
      perpendicular[2] = 1.0f;
      // Discard z value(leave it at 0.0)
      r_vector[0] = r_end_point[0] - r_hand_point[0];
      r_vector[1] = r_end_point[1] - r_hand_point[1];
      r_vector[2] = 0.0;
      break;
   }

   // Use cross product to get vector perpendicular to the
   // force vector on the viewing plane so we can find the other two
   // points of the triangle(the first point on the triangle is the 
   // position of the hand)

   // For some stupid reason, "cross = r_vector * perpendicular" doesn't work right,
   // so calculate the cross product here
   r_cross[0] = r_vector[1]*perpendicular[2] - r_vector[2]*perpendicular[1];
   r_cross[1] = r_vector[2]*perpendicular[0] - r_vector[0]*perpendicular[2];
   r_cross[2] = r_vector[0]*perpendicular[1] - r_vector[1]*perpendicular[0];

   r_cross.normalize();

   // Start the arrow head 8/10ths of the way up the vector
   point3d r_triangle_base = r_end_point - (r_vector * ARROW_STARTING_PERC);
   // Add the cross product to the base to find the other two triangle points
   point3d r_triangle_point[] = { r_triangle_base + (r_cross * (double)25), r_triangle_base - (r_cross * (double)25) };

   // Draw the arrowheads as triangles
   glBegin(GL_TRIANGLES);
   glVertex3dv( r_hand_point );
   glVertex3dv( r_triangle_point[0] );
   glVertex3dv( r_triangle_point[1] );
   glEnd();
}
E_Points GetJointCtrlPoint(JointTorqueEnm joint)
// This function converts the joint torque enum to corresponding selectable point
{
   E_Points temp;
   switch(joint)
   {
   case JT_LeftElbow:	temp =L_ELBOW;
      break;
   case JT_RightElbow: temp = R_ELBOW;
      break;
   case JT_LeftShoulder: temp = L_SHOULDER;
      break;
   case JT_RightShoulder: temp = R_SHOULDER;
      break;
   case JT_L5S1:		temp = L5_S1;
      break;
   case JT_LeftHip:	temp = L_HIP;
      break;	
   case JT_RightHip:	temp = R_HIP;
      break;
   case JT_LeftKnee:	temp = L_KNEE;
      break;
   case JT_RightKnee:	temp = R_KNEE;
      break;
   case JT_LeftAnkle:	temp = L_ANKLE;
      break;
   case JT_RightAnkle: temp = R_ANKLE;
      break;
   default:	temp = (E_Points)0;
      break;
   }
   return temp;
}

E_Points GetJointForcePoint(JointTorqueEnm joint)
// This function converts the joint torque enum to corresponding selectable point
{
   E_Points temp;
   switch(joint)
   {
   case JT_LeftElbow:	temp =L_ELB_FORCE_TIP;
      break;
   case JT_RightElbow: temp = R_ELB_FORCE_TIP;
      break;
   case JT_LeftShoulder: temp = L_SHO_FORCE_TIP;
      break;
   case JT_RightShoulder: temp = R_SHO_FORCE_TIP;
      break;
   case JT_L5S1:		temp = L5S1_FORCE_TIP;
      break;
   case JT_LeftHip:	temp = L_HIP_FORCE_TIP;
      break;	
   case JT_RightHip:	temp = R_HIP_FORCE_TIP;
      break;
   case JT_LeftKnee:	temp = L_KNEE_FORCE_TIP;
      break;
   case JT_RightKnee:	temp = R_KNEE_FORCE_TIP;
      break;
   case JT_LeftAnkle:	temp = L_ANKLE_FORCE_TIP;
      break;
   case JT_RightAnkle: temp = R_ANKLE_FORCE_TIP;
      break;
   default:	temp = (E_Points)0;
      break;
   }
   return temp;
}

E_Manipulation_State GetMSForceState(JointTorqueEnm joint)
// This function converts the joint torque enum to corresponding selectable point
{
   E_Manipulation_State temp;
   switch(joint)
   {
   case JT_LeftElbow:		temp = MS_FORCE_L_ELBOW;
      break;
   case JT_RightElbow:		temp = MS_FORCE_R_ELBOW;
      break;
   case JT_LeftShoulder:	temp = MS_FORCE_L_SHOULD;
      break;
   case JT_RightShoulder:	temp = MS_FORCE_R_SHOULD;
      break;
   case JT_L5S1:			temp = MS_FORCE_L5S1;
      break;
   case JT_LeftHip:		temp = MS_FORCE_L_HIP;
      break;	
   case JT_RightHip:		temp = MS_FORCE_R_HIP;
      break;
   case JT_LeftKnee:		temp = MS_FORCE_L_KNEE;
      break;
   case JT_RightKnee:		temp = MS_FORCE_R_KNEE;
      break;
   case JT_LeftAnkle:		temp = MS_FORCE_L_ANKLE;
      break;
   case JT_RightAnkle:		temp = MS_FORCE_R_ANKLE;
      break;
   default:				temp = MS_NULL;
      break;
   }
   return temp;
}

void DrawEndsOfFeet(C_Hom_View * view_ptr, E_Points heel)
// This function draws the circular end of each foot.
{
   GLfloat footRadius;
   Flt_3D_Polyline WorldPoints = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name ).Get_Points();
   bool temp = true;
   bool side = true;
   glGetFloatv(GL_LINE_WIDTH, &footRadius); 

   do
   {
      // This code should execute twice drawing each side of the foot
      glPushMatrix();
      if (temp)
      {
         glTranslated(WorldPoints(heel)(0)+FOOT_LENGTH,WorldPoints(heel)(1),WorldPoints(heel)(2));
         temp = !temp;
      }
      else 
      {
         glTranslated(WorldPoints(heel)(0),WorldPoints(heel)(1),WorldPoints(heel)(2));
         temp = !temp;
      }
      RotateView(view_ptr);
      gluSphere(q, footRadius, SPHERE_LATITUDE, SPHERE_LONGITUDE);
      glPopMatrix();	
   } while(temp != side);
   return;
}

void RotateView(C_Hom_View * view_ptr)
// This function rotates the view of the stick figure so that items can be drawn
// correctly
{
   switch(view_ptr->View_Type)
   {
   case VT_FRONT:
      glRotatef(90, 0.0f, 1.0f, 0.0f);
      break;
   case VT_SIDE:
      glRotatef(90, 1.0f, 0.0f, 0.0f);
      break;
   case VT_TOP:	// don't need rotation
   default:
      break;
   }
}

//=============================================================================
void SetMatrix(C_Hom_View * view_ptr, GLdouble* m)
// This function manipulates the identity matrix for the top view
{
   // Set the identity matrix
   for (int i = 0; i < 16; i++)
   {
      if (i % 5 == 0)
      {
         m[i] = 1;
      } else {
         m[i] = 0;
      }
   }
   // Set the Z-axis to zero for the top view and front view
   // This eliminates the cutting on the z-axis
   if (view_ptr->View_Type == VT_TOP || view_ptr->View_Type == VT_FRONT)
   {
      m[10] = 0;
   }
}

//=============================================================================
void setupOpenGLContext()
// This function turns on shading, blending and smooth line drawing for opengl.
// This function was created primarily for setting up printing.
{
   glShadeModel(GL_SMOOTH);	
   glEnable(GL_LINE_SMOOTH);
   glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
   // JTK - Blending disabled because of a color distortion problem
   //	glEnable(GL_BLEND);
   //	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);	// Pick Perspective Calculations			
   glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
   glLoadIdentity();									// Reset The Projection Matrix
   q = gluNewQuadric();								// create a quadric objectto draw the spheres, cylinders, etc
   gluQuadricDrawStyle(q, GLU_FILL);
   gluQuadricOrientation(q, GLU_INSIDE);
   glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
   glLoadIdentity();	
   return;
}

//=============================================================================
bool AreFeetLevel(int view)
{
   Flt_3D_Polyline WorldPoints = Get_App_Ptr()->Get_GDB_Ptr()->Skeleton_Ref_Get( Get_App_Ptr()->Hom_Ske_Name ).Get_Points();
   C_Hom_Doc* Doc_Ptr = Get_App_Ptr()->Get_Document_Ptr();
   bool success = false;
   if  (WorldPoints(L_HEEL)(view) < WorldPoints(R_HEEL)(view) + FLOOR_ERROR && WorldPoints(L_HEEL)(view) >= WorldPoints(R_HEEL)(view)) 
   {
      success = true;
   }
   if(WorldPoints(L_HEEL)(view) <= WorldPoints(R_HEEL)(view) && WorldPoints(L_HEEL)(view) > WorldPoints(R_HEEL)(view) - FLOOR_ERROR)
   {
      success = true;
   }

   if (Doc_Ptr->getStance() != STANCE_BOTH_FEET)
   {
      success = false;	// Only one foot is supported
   }
   return success;
}

//=============================================================================
bool FloorDetected(C_Hom_View* view_ptr, C_Hom_Doc* Doc_Ptr, Flt_3D_Polyline& WorldPoints, Flt_3D_Point vertex)
// This function detects when the floor is under an unselectable point so that it makes the joints appear to be 
// transparent.  
{
   bool success = false;
   if (view_ptr->View_Type == VT_TOP)
   {
      // Check Left Foot
      if (
         // Check Y
         vertex(Y_COORD) < WorldPoints(L_HEEL)(Y_COORD) + UPPER_OFFSET &&
         vertex(Y_COORD) > WorldPoints(L_HEEL)(Y_COORD) - LOWER_OFFSET &&
         // Check x
         vertex(X_COORD) < WorldPoints(L_HEEL)(X_COORD)*BOX_SIZE &&
         vertex(X_COORD) > 0 
         && Doc_Ptr->getStance() != STANCE_RIGHT_FOOT)  // Make sure the foot is supported
         // Check Right Foot
      {
         success = true;
      }
      // Check Right Foot
      if (// Check Y
         vertex(Y_COORD) < WorldPoints(R_HEEL)(Y_COORD) + UPPER_OFFSET &&
         vertex(Y_COORD) > WorldPoints(R_HEEL)(Y_COORD) - LOWER_OFFSET &&
         // Check X
         vertex(X_COORD) < WorldPoints(R_HEEL)(X_COORD)*BOX_SIZE &&
         vertex(X_COORD) > 0
         && Doc_Ptr->getStance() != 1) // Make sure the foot is supported 
      {
         success = true;
      }
      if (// Check when the floor is level (Right Foot Forward) NOTE: AreFeetLevel returns false if both feet are not supported
         AreFeetLevel() &&
         WorldPoints(L_HEEL)(Y_COORD) - WorldPoints(R_HEEL)(Y_COORD) < 0 &&
         vertex(X_COORD) >  WorldPoints(R_HEEL)(X_COORD)*BOX_SIZE &&
         vertex(X_COORD) < WorldPoints(L_HEEL)(X_COORD)*BOX_SIZE &&
         vertex(Y_COORD) > WorldPoints(L_HEEL)(Y_COORD) - LOWER_OFFSET &&
         vertex(Y_COORD) < WorldPoints(R_HEEL)(Y_COORD) + UPPER_OFFSET)
      {
         success = true;
      }
      if  ( // Check when the floor is level (Left Foot Forward)
         AreFeetLevel() &&
         WorldPoints(R_HEEL)(Y_COORD) - WorldPoints(L_HEEL)(Y_COORD) < 0 &&
         vertex(X_COORD) >  WorldPoints(R_HEEL)(X_COORD)*BOX_SIZE &&
         vertex(X_COORD) < WorldPoints(L_HEEL)(X_COORD)*BOX_SIZE &&
         vertex(Y_COORD) > WorldPoints(R_HEEL)(Y_COORD) - LOWER_OFFSET &&
         vertex(Y_COORD) < WorldPoints(L_HEEL)(Y_COORD) + UPPER_OFFSET)
      {
         success = true;
      }
   }
   return success;
}
//=============================================================================
void RenderCube(bool outline, bool shading, bool chairFix)
// Creates a 2x2x2 cube (JTK - Maybe this should be 1x1x1?)
// Shading and chair fix are specific to the hominoid's chair.
// The normals are not correct for the environment cube.
// The chair fix variable eliminates the need for the front of the cube to render
// this quad is not needed in the chair because it is in the circular base
// and causes a funny jagged line to appear.
{

   if (outline)
   {
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   }
   glBegin(GL_QUADS);
   if (shading)
   {
      glNormal3f(1.0, 0.0, 0.0);
   }	
   glVertex3f(1.0, 1.0, 1.0);
   glVertex3f(1.0, 1.0,-1.0);
   glVertex3f(1.0, -1.0, -1.0);
   glVertex3f(1.0, -1.0, 1.0);
   if (shading)
   {
      glNormal3f(-1.0, 0.0, 0.0);
   }	
   glVertex3f(-1.0, 1.0,  1.0);
   glVertex3f(-1.0, 1.0, -1.0);
   glVertex3f(-1.0, -1.0,-1.0);
   glVertex3f(-1.0, -1.0, 1.0);
   if (shading)
   {
      glNormal3f(0.0, 1.0, 0.0);
   }	
   glVertex3f(1.0, -1.0, 1.0);
   glVertex3f(-1.0, -1.0, 1.0);
   glVertex3f(-1.0, 1.0, 1.0);
   glVertex3f(1.0, 1.0, 1.0);
   if (shading)
   {
      glNormal3f(0.0, 1.0, 0.0);
   }	
   glVertex3f(1.0, -1.0, -1.0);
   glVertex3f(-1.0, -1.0, -1.0);
   glVertex3f(-1.0, 1.0,-1.0);
   glVertex3f(1.0, 1.0, -1.0);
   if (shading)
   {
      glNormal3f(0.0, 0.0, 1.0);
   }	
   if (!chairFix)
   {
      glVertex3f(1.0, 1.0, -1.0);
      glVertex3f(-1.0, 1.0, -1.0);
      glVertex3f(-1.0, 1.0, 1.0);
      glVertex3f(1.0, 1.0, 1.0);
   }
   if (shading)
   {
      glNormal3f(0.0, 0.0, 1.0);			
   }
   glVertex3f(1.0, -1.0, -1.0);
   glVertex3f(-1.0, -1.0,-1.0);
   glVertex3f(-1.0, -1.0, 1.0);
   glVertex3f(1.0, -1.0, 1.0); 
   glEnd();
   return;
}
void RenderBarrierCube(bool outline, bool shading, bool chairFix)
// This function is very similar to RenderCube except that the z value is from 
// [0, 2] so that the measurement of elevation works out correctly.

{
   if (outline)
   {
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   }
   glBegin(GL_QUADS);
   if (shading)
   {
      glNormal3f(1.0, 0.0, 0.0);
   }	
   glVertex3f(1.0, 2.0, 2.0);
   glVertex3f(1.0, 2.0, 0.0);
   glVertex3f(1.0, 0.0, 0.0);
   glVertex3f(1.0, 0.0, 2.0);
   if (shading)
   {
      glNormal3f(1.0, 0.0, 0.0);
   }	
   glVertex3f(-1.0, 2.0,  2.0);
   glVertex3f(-1.0, 2.0, 0.0);
   glVertex3f(-1.0, 0.0, 0.0);
   glVertex3f(-1.0, 0.0, 2.0);
   if (shading)
   {
      glNormal3f(0.0, 1.0, 0.0);
   }	
   glVertex3f(1.0, 0.0, 2.0);
   glVertex3f(-1.0, 0.0, 2.0);
   glVertex3f(-1.0, 2.0, 2.0);
   glVertex3f(1.0, 2.0, 2.0);
   if (shading)
   {
      glNormal3f(0.0, 1.0, 0.0);
   }	
   glVertex3f(1.0, 0.0, 0.0);
   glVertex3f(-1.0, 0.0, 0.0);
   glVertex3f(-1.0, 2.0, 0.0);
   glVertex3f(1.0, 2.0, 0.0);
   if (shading)
   {
      glNormal3f(0.0, 0.0, 1.0);
   }	
   if (!chairFix)
   {
      glVertex3f(1.0, 2.0, 0.0);
      glVertex3f(-1.0, 2.0, 0.0);
      glVertex3f(-1.0, 2.0, 2.0);
      glVertex3f(1.0, 2.0, 2.0);
   }
   if (shading)
   {
      glNormal3f(0.0, 0.0, 1.0);			
   }
   glVertex3f(1.0, 0.0, 0.0);
   glVertex3f(-1.0, 0.0, 0.0);
   glVertex3f(-1.0, 0.0, 2.0);
   glVertex3f(1.0, 0.0, 2.0); 
   glEnd();
}
bool SetPixelFormat(HDC hDC, DWORD dwFlags)
{
   PIXELFORMATDESCRIPTOR printDesc;

   printDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
   printDesc.nVersion = 1;
   printDesc.iPixelType = PFD_TYPE_RGBA;
   printDesc.cColorBits = COLOR_DEPTH;
   printDesc.cRedBits = 0;
   printDesc.cRedShift = 0;
   printDesc.cGreenBits = 0;
   printDesc.cGreenShift = 0;
   printDesc.cBlueBits = 0;
   printDesc.cBlueShift = 0;
   printDesc.cAlphaBits = 0;
   printDesc.cAlphaShift = 0;
   printDesc.cAccumBits = 0;
   printDesc.cAccumRedBits = 0;
   printDesc.cAccumGreenBits = 0;
   printDesc.cAccumBlueBits = 0;
   printDesc.cAccumAlphaBits = 0;
   printDesc.cDepthBits = 16;
   printDesc.cStencilBits = 0;
   printDesc.cAuxBuffers = 0;
   printDesc.iLayerType = PFD_MAIN_PLANE;
   printDesc.bReserved = 0;
   printDesc.dwLayerMask = 0;
   printDesc.dwVisibleMask = 0;
   printDesc.dwDamageMask = 0;
   printDesc.dwFlags = dwFlags;

   int nPixelIndex = ::ChoosePixelFormat(hDC, &printDesc);
   if (nPixelIndex == 0) // Choose default
   {
      nPixelIndex = 1;
      if (::DescribePixelFormat(hDC, nPixelIndex, 
         sizeof(PIXELFORMATDESCRIPTOR), &printDesc) == 0)
         return false;
   }

   if (!::SetPixelFormat(hDC, nPixelIndex, &printDesc))
      return false;

   return true;
}
void RenderHominoidForceArrow(GLUquadric* q)
{
   gluCylinder(q, 0.25f, 0.25f, 3.0, 32, 32);
   gluDisk(q, 0.0, 0.25f, 32, 32);
   glTranslatef(0.0, 0.0, 3.0);
   gluCylinder(q, 0.75, 0.0, 1.0, 32, 32);
   gluDisk(q, 0.0f, 0.75f, 32, 32);
   return;
}

