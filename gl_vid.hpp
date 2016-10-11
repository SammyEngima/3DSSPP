#if ! defined( GL_VID_HPP )
#define GL_VID_HPP
#include <gl\gl.h>								// Header File For The OpenGL32 Library
#include <gl\glu.h>								// Header File For The GLu32 Library
#include <gl\glaux.h>							// Header File For The GLaux Library
#include "quad_frm.h"							// Header File for the E_View_Type
#include "skel_rec.hpp"
//#include "hom_doc.h"		// Unnecessary!
#include "analysis.h"	// For joint torque enums
#include "GLBalanceView.h"

#include <string>

using std::string;

class C_Hom_Doc;	// Forwarded

#define GL_OBLIQUE		1
#define GL_ORTHO		2
#define VALID_PERSPECTIVE(x) (x >= GL_OBLIQUE && x <= GL_ORTHO)



// When set to 1.0f, there will be points of the skeleton RIGHT on the
// border of the window, so make it a little more so there's distance
// between the extremities and the window border.
#define TOP_VIEW_BORDER 1.174f//2.568f
#define FRONT_VIEW_BORDER 1.174f
#define SIDE_VIEW_BORDER 1.174f

// LINE_WIDTH_DIVISOR divides either the height of width of the window 
// to determine the width of the line
#define LINE_WIDTH_DIVISOR 130.0f
#define STICK_FIG_PRINTING_SIZE 1250	// Determines the size of the printing box and figure
#define FOOT_LENGTH 100.0f	// Changed from 100 to check the ball of foot location in the joint location report

// Used to calculate the viewport and targetrects
#define ORIGINAL_WORLD_SIZE 950.0f
#define WIDTH_CENTER (259.0f/518.0f)
#define HEIGHT_CENTER (267.0f/536.0f)
#define WIDTH_RATIO 518.0f
#define HEIGHT_RATIO 536.0f
#define WIDTH_HEIGHT_RATIO (WIDTH_RATIO/HEIGHT_RATIO)
#define FLOOR_ERROR 5		// This value determines how close the ankles have to be for the floor to be drawn
							// it is given in the same units as WorldPoints (mm)

#define COLOR_DEPTH 32
/*
 *	If you modify this, also modify ArrayNameIndex 
 */
const int EXT3D_NUM_JOINTS = 11;

const int	ArrayIndex[EXT3D_NUM_JOINTS] =
{
//	JT_LeftWrist,		// "Left Wrist",	
//	JT_RightWrist,		// "Right Wrist",	
	// NOTE:  This list controls the order of which 
	JT_RightElbow,		// "Right Elbow",
	JT_RightShoulder,	// "Right Shoulder",
	JT_LeftElbow,		// "Left Elbow",	
	JT_LeftShoulder,	// "Left Shoulder",		
	JT_L5S1,			// "L5S1",	
	JT_RightHip,		// "Right Hip",	
	JT_RightKnee,		// "Right Knee",
	JT_RightAnkle,		// "Right Ankle",	
	JT_LeftHip,			// "Left Hip",		
	JT_LeftKnee,		// "Left Knee",		
	JT_LeftAnkle		// "Left Ankle",	
	// NOTE: This list needs to match the end of E_Manipulate_State and E_Points in skel_rec.hpp,
	// the dialog box, and ArrayNameIndex in Dg_JointLoads.cpp
	// otherwise it will cause a problem with where the joint forces are displayed, how they are listed
	// in the dialog box and also with the export summary
	// This probably should be re-thought and improved upon
};

struct glCamera_t
{
	double	x, y, z;		// translation coordinates
	double	pitch, yaw, roll;
//  pQuat	orientation;		// future - Use quaternion orientation data
};

struct glContext_t 
{
   std::string name;

   HDC gl_hDC;          // Private GDI Device Context
   HGLRC gl_hRC;        // Permanent Rendering Context
   HWND gl_hWnd;        // Holds Our Window Handle
   HINSTANCE hInstance; // Holds The Instance Of The Application
   int gl_perspective;
   glCamera_t camera;
   glContext_t *next;
};

// pfd Tells Windows How We Want Things To Be
//const PIXELFORMATDESCRIPTOR pfd =
//{
//	sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
//		1,											// Version Number
//		PFD_DRAW_TO_WINDOW | 						// Format Must Support Window
//		PFD_DRAW_TO_BITMAP |						// Format Must support Printing
//		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
//		//PFD_SUPPORT_GDI |							// Format Must Support GDI
//		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
//		PFD_TYPE_RGBA,								// Request An RGBA Format
//		24,											// Select Our Color Depth [32 bit?]
//		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
//		0,											// No Alpha Buffer
//		0,											// Shift Bit Ignored
//		0,											// No Accumulation Buffer
//		0, 0, 0, 0,									// Accumulation Bits Ignored
//		16,											// 16Bit Z-Buffer (Depth Buffer)  
//		0,											// No Stencil Buffer
//		0,											// No Auxiliary Buffer
//		PFD_MAIN_PLANE,								// Main Drawing Layer
//		0,											// Reserved
//		0, 0, 0										// Layer Masks Ignored
//};

class C_Hom_View;

void InitLighting();
int GetTargetRectSize(const CRect & windowRect);
void CalculateViewportSizeToRatio(int & GL_viewport_size_x, int & GL_viewport_size_y, const CRect & windowRect, C_Hom_View* view_ptr);
float MaxDistanceFromOrigin(C_Hom_View *view_ptr);
float FarthestDistanceFromView(C_Hom_View *view_ptr);
float FarthestDistanceFromViewingVolume(float world_size);
Flt_3D_Point AdjustedForceEndPoint(Flt_3D_Point ForceEndPoint, Flt_3D_Point HandCenter);
void GenerateTargetRects(C_Hom_View * view_ptr);
void Draw_Background(C_Hom_View *view_ptr, CDC* pDC);
void Draw_GL_Stick_Figure(C_Hom_View * view_ptr);
void DrawHead(E_View_Type view_type);
void DrawForces(C_Hom_View * view_ptr);
void DrawVertices(C_Hom_View * view_ptr);
void DrawSkeleton(C_Hom_View * view_ptr);
void DrawFeet(C_Hom_View * view_ptr);
void SetProjectionMatrix(C_Hom_View *view_ptr);
void SetViewport(C_Hom_View * view_ptr);
void SetCamera(C_Hom_View * view_ptr);
void SetLineWidth(C_Hom_View * view_ptr);
void DrawEndsOfFeet(C_Hom_View * view_ptr, E_Points heel);
void DrawAxes();		// Used for debugging
void SetColor();
void DrawNonSelectableVertex(C_Hom_View * view_ptr, Flt_3D_Point vertex, bool color);
void DrawBackgroundColorVertex(C_Hom_View *view_ptr);
bool CreateGLContext(HWND hWnd, HDC hDC, HGLRC *ret_hRC, std::string FrameName, int PerspectiveType);
bool ActivateGLWindow(std::string NAME);
bool DestroyGLContext (std::string FrameName);
void GLSwapBuffers();
int ShutdownOpenGL();
void SetMatrix(C_Hom_View * view_ptr, GLdouble* m);
void SetRectangle(C_Hom_View * view_ptr);
void SetStance(C_Hom_Doc* Doc_Ptr, E_View_Type view_type); 
void DrawRectangle(E_Points heel, E_View_Type view_type, bool drawOutline);
void DrawJointForces(C_Hom_View * view_ptr, E_Points ForceTip, E_Points JointCtrl, BYTE rArrow, BYTE gArrow, BYTE bArrow, BYTE rSphere, BYTE gSphere, BYTE bSphere);
void DrawJointForces(C_Hom_View * view_ptr,E_Points JointCtrl, E_Points JointForce, double x, double y, double z,  BYTE rArrow, BYTE gArrow, BYTE bArrow, BYTE rSphere, BYTE gSphere, BYTE bSphere);
void RotateView(C_Hom_View * view_ptr);
E_Points GetJointCtrlPoint(JointTorqueEnm joint);
E_Points GetJointForcePoint(JointTorqueEnm joint);
E_Manipulation_State GetMSForceState(JointTorqueEnm joint);
void RenderBarrierCube(bool outline, bool shading = false, bool chairFix = false);
void setupOpenGLContext();
bool AreFeetLevel(int view = 2);
void RenderCube(bool outline, bool shading = false, bool chairFix = false);
bool FloorDetected(C_Hom_View* view_ptr, C_Hom_Doc* Doc_Ptr, Flt_3D_Polyline& WorldPoints, Flt_3D_Point vertex);
bool SetPixelFormat(HDC hDC, DWORD dwFlags);	
void RenderHominoidForceArrow(GLUquadric* q);
// Functions used for locking the legs and trunk
void SetPoint(int lockingPoint, int selection);
int GetPoint(int point);
#endif