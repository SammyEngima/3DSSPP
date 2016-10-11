#include "GLSeatCOP.h"

#include "Hom.h"
#include "hom_doc.h" // Necessary
#include "../c4eLib/Units.hpp"
#include "Analysis.h"
#include "GL/glu.h"
#include "GLBalanceView.h"

#define Y_OFFSET 20.0f 
#define ERROR_RANGEY 3.0f
#define ERROR_RANGEX 3.0f

#define SKIN_OPACITY 1.0f
#define RED_SKIN_COLOR 180.0f
#define GREEN_SKIN_COLOR 180.0f
#define BLUE_SKIN_COLOR 180.0f

#define RED_PELVIS_LINE 0.0f
#define GREEN_PELVIS_LINE 0.0f
#define BLUE_PELVIS_LINE 0.0f
#define PELVIS_LINE_OPACITY 1.0f

#define RED_PELVIS_FILL 247.0f
#define GREEN_PELVIS_FILL 247.0f
#define BLUE_PELVIS_FILL 0.0f
#define PELVIS_FILL_OPACITY 1.0f

// Black
#define RED_SEAT_COP_LINE 0.0f
#define GREEN_SEAT_COP_LINE 0.0f
#define BLUE_SEAT_COP_LINE 0.0f
#define SEAT_COP_LINE_OPACITY 1.0f

// YELLOW
#define RED_SEAT_COP_FILL 247.0f
#define GREEN_SEAT_COP_FILL 247.0f
#define BLUE_SEAT_COP_FILL 0.0f
#define SEAT_COP_FILL_OPACITY 1.0f

// Black
#define RED_L_IT_LINE 0.0f
#define GREEN_L_IT_LINE 0.0f
#define BLUE_L_IT_LINE 0.0f
#define L_IT_LINE_OPACITY 1.0f

// Red
#define RED_L_IT_FILL 255.0f
#define GREEN_L_IT_FILL 0.0f
#define BLUE_L_IT_FILL 0.0f
#define L_IT_FILL_OPACITY 1.0f

// Black 
#define RED_R_IT_LINE 0.0f
#define GREEN_R_IT_LINE 0.0f
#define BLUE_R_IT_LINE 0.0f
#define R_IT_LINE_OPACITY 1.0f

// Red
#define RED_R_IT_FILL 255.0f
#define GREEN_R_IT_FILL 0.0f
#define BLUE_R_IT_FILL 0.0f
#define R_IT_FILL_OPACITY 1.0f

// Green
#define RED_BOUNDARY_COLOR 0.0f
#define GREEN_BOUNDARY_COLOR 128.0f
#define BLUE_BOUNDARY_COLOR 0.0f
#define BOUNDARY_OPACITY 1.0f

#define PELVIS_OFFSET 5
const float SEAT_COP_RADIUS  = 4.0f;

static void CALLBACK vertex(void * v);
static void CALLBACK end();
static void CALLBACK start(GLenum type);
static void CALLBACK combine(GLdouble c[3], void *d[4], GLfloat w[4], void **out);
static void CALLBACK error(GLenum errno);

GLSeatCOP::GLSeatCOP()
{
    listIdSkin = 0;
    listIdPelvis = 0;
    listIdTuber = 0;
    listIdPelvisCorrection = 0;
    listIdRightExcursion = 0;
    listIdLeftExcursion = 0;
    listIdFrontExcursion = 0;
    listIdRightCorner = 0;
    listIdLeftCorner = 0;
}
//=============================================================================
void GLSeatCOP::drawSkin(float UNITS_CM,  //factor
                         float Fx,
                         float Fy,
                         float factor,
                         long center_x,
                         long center_y)
{

   glNewList(listIdSkin, GL_COMPILE);
   for(int i = 0; i < NUM_SKIN_POINTS; i++)
   {
      glVertex3f(skin[i].x*Fx*UNITS_CM*factor + center_x, (skin[i].y*Fy*UNITS_CM)*factor + center_y + Y_OFFSET, 0.0);
   }
   glEndList();
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glColor4f(RED_SKIN_COLOR/255.0f,
      GREEN_SKIN_COLOR/255.0f,
      BLUE_SKIN_COLOR/255.0f,
      SKIN_OPACITY);
   glBegin(GL_POLYGON);
   glCallList(listIdSkin);
   glEnd();

}
//=============================================================================
void GLSeatCOP::drawPelvis(float w, //Width
                           float h, //Height
                           float pAng,
                           float UNITS_CM, //factor
                           float Fx,
                           float Fy,
                           float factor,
                           long center_x,
                           long center_y)
{	

   glNewList(listIdPelvis, GL_COMPILE);
   if(pAng > -20)
   {
      for(int i = 0; i < NUM_PELVIS_POINTS; i++)
      {
         glVertex2f(pelvisLessThan15[i].x*Fx*UNITS_CM*factor + center_x, pelvisLessThan15[i].y*Fy*UNITS_CM*factor + center_y+ Y_OFFSET - PELVIS_OFFSET);
      }
   } else if( (pAng <= -20) && (pAng > -30) ) {
      for(int i = 0; i < NUM_PELVIS_POINTS; i++)
      {
         glVertex2f(pelvisBetween20and30[i].x*Fx*UNITS_CM*factor + center_x, pelvisBetween20and30[i].y*Fy*UNITS_CM*factor + center_y + Y_OFFSET - PELVIS_OFFSET);
      }
   } else if( (pAng <= -30) && (pAng > -40) ) {
      for(int i = 0; i < NUM_PELVIS_POINTS; i++)
      {
         glVertex2f(pelvisBetween30and40[i].x*Fx*UNITS_CM*factor + center_x, pelvisBetween30and40[i].y*Fy*UNITS_CM*factor + center_y + Y_OFFSET - PELVIS_OFFSET);
      }
   } else if ((pAng <= -40) && (pAng > -50)) {
      for(int i = 0; i < NUM_PELVIS_POINTS; i++)
      {
         glVertex2f(pelvisBetween40and50[i].x*Fx*UNITS_CM*factor + center_x, pelvisBetween40and50[i].y*Fy*UNITS_CM*factor + center_y + Y_OFFSET - PELVIS_OFFSET);
      }
   } else {
      for(int i = 0; i < NUM_PELVIS_POINTS; i++)
      {
         glVertex2f(pelvisGreaterThan50[i].x*Fx*UNITS_CM*factor + center_x, pelvisGreaterThan50[i].y*Fy*UNITS_CM*factor + center_y + Y_OFFSET - PELVIS_OFFSET);
      }
   }
   glEndList();

   
   glColor4f(RED_PELVIS_FILL/255.0f, GREEN_PELVIS_FILL/255.0f, BLUE_PELVIS_FILL/255.0, PELVIS_FILL_OPACITY);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   float checkMode[2];
   glGetFloatv(GL_POLYGON_MODE, checkMode);
   glBegin(GL_POLYGON);
   glCallList(listIdPelvis);
   glEnd();
   glColor4f(RED_PELVIS_LINE/255.0f, GREEN_PELVIS_LINE/255.0f, BLUE_PELVIS_LINE/255.0f, PELVIS_LINE_OPACITY);
   glLineWidth(1.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 
   glGetFloatv(GL_POLYGON_MODE, checkMode);
   glBegin(GL_POLYGON);
   glCallList(listIdPelvis);
   glEnd();
   pelvisCorrection(pAng, UNITS_CM, Fx, Fy, factor, center_x, center_y);
   	// This takes care of any corrections that need to be made
   
   return;
}

//=============================================================================
void GLSeatCOP::drawSeatCOP(CRect& mCOPDrawingRect)
// This function does all of the necessary calculations to create the seat
// center of pressure graphic.
// INPUT: Rectangle describing opengl drawing region
// OUTPUT: OpenGL graphic
// NOTE:  The opengl context must already be created for this function work correctly
{
   // Initialize the display lists
   listIdSkin = glGenLists(1);
   listIdPelvis = glGenLists(1);
   listIdTuber = glGenLists(1);
   listIdPelvisCorrection = glGenLists(1);
   listIdRightExcursion = glGenLists(1);
   listIdFrontExcursion = glGenLists(1);
   listIdLeftExcursion = glGenLists(1);
   listIdRightCorner = glGenLists(1);
   listIdLeftCorner = glGenLists(1);

   Length lLocX(0, 7);
   Length lLocY(0, 7);
   Length lLocZ(0, 7);

   C_Hom_Doc* lDocPtr = (C_Hom_Doc*)Get_App_Ptr()->Get_Document_Ptr();
   //Analysis & lResults =lDocPtr->getResultsRef();
   Skeleton &lSkeleton = *lDocPtr->GetSkeleton();
   //BalanceData balance_data;
   //lResults.getBalanceData(&balance_data);
   const BalanceData_s balance_data = lSkeleton.getBalanceData();

   const Factors &lFactors = lDocPtr->GetFactors();
   // Used to make sure the front boundary does not end up on the border
   const float BORDER_OFFSET = 2.0f;   
   float Fx,Fy,UNITS_CM, pAng, SeatX, SeatY;
   float ITx, ITy;
   float maxX, maxY, minX, minY;
   float w = mCOPDrawingRect.Width();
   float h = mCOPDrawingRect.Height();
   double frontStability;    // Used for scaling
   long center_x, center_y;
   double yOffsetLeft = 0, yOffsetRight = 0;
   double l5s1Loc;
   lSkeleton.C4EPoint( JT_L5S1_TEMP, lLocX, lLocY, lLocZ);
   l5s1Loc = lLocY.ValueIn(MetricCns);

   lSkeleton.C4EPoint( JT_LIT, lLocX, lLocY, lLocZ );
   ITx = lLocX.ValueIn(MetricCns);
   ITy = -(l5s1Loc - lLocY.ValueIn(MetricCns));
   
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   pAng = lDocPtr->getPelvicAngle();
   drawingRect = mCOPDrawingRect; // used to convert to windows coordinates for the 
   if (w < h) {
      //     * 35.328264
      Fx = w / (( 0.07627*193 )*2*1.2);	
      Fy = -w/ (( 0.07627*193 )*2*1.2);	
      Fx =  w / 35;
      Fy = -w / 35;
   }
   else {
      //     / 32.1
      Fx =  h/(( 26 )*1.2);	
      Fy = -h/(( 26 )*1.2);	

      Fx =  h / 35;
      Fy = -h / 35;
   }
   // Units are in mm and scaled by Fx, Fy

   // This appears to be some sort of conversion
   // for the pelvis and the skin.  
   UNITS_CM = ( 0.07627 * lDocPtr->getAnthropometryRef().GetHeight(MetricCns)) / 20;	
   
   //double convert = 1;
   double convert = U_M_CM;
   //if(lDocPtr->Is_English())
   //     convert = U_IN_CM;
   // Get values for the circle
   SeatY = -(l5s1Loc - balance_data.CenterOfPressure[1] * convert) ;
   SeatX = balance_data.CenterOfPressure[0] * convert;

   // Find the max/min values for zooming
   maxX = max(Fx*SeatX, Fx*ITx);
   maxY = max(Fy*(SeatY) + Y_OFFSET - PELVIS_OFFSET, Fy*ITy + Y_OFFSET - PELVIS_OFFSET);
   minX = min(Fx*SeatX, Fx*ITx);
   minY = min(Fy*(SeatY) + Y_OFFSET - PELVIS_OFFSET, Fy*ITy + Y_OFFSET - PELVIS_OFFSET);

   maxX = max(maxX,-Fx*SeatX);
   minX = min(minX, -Fx*SeatX);
   maxX = max(maxX, findExtreme(true, true, UNITS_CM, Fx, Fy));
   maxY = max(maxY, findExtreme(false, true, UNITS_CM, Fx, Fy));
   minX = min(minX, findExtreme(true, false, UNITS_CM, Fx, Fy));
   minY = min(minY, findExtreme(false, false, UNITS_CM, Fx, Fy));
   // Check the front stability
   //frontStability = Analysis::FindFrontStability() -  l5s1Loc; // the y-axis is flipped
   Vector3 seatRef = lSkeleton.getSeatReference();
   	if(lDocPtr->getFeetSupport() == FS_Neither) {
		frontStability = lSkeleton.getSeatCenter()[1] - seatRef[1];
	} else if(lDocPtr->getFeetSupport() == FS_Left) {
		frontStability = lSkeleton.getJoint(JT_LBALL)[1] - seatRef[1];
	} else if(lDocPtr->getFeetSupport() == FS_Right) {
		frontStability = lSkeleton.getJoint(JT_RBALL)[1] - seatRef[1];
	} else {
		frontStability = lSkeleton.getJoint(JT_LBALL)[1] - seatRef[1];
		double rball = lSkeleton.getJoint(JT_RBALL)[1] - seatRef[1];
		if(rball > frontStability) {
			frontStability = rball;
		}
	}
   frontStability -= l5s1Loc; // the y-axis is flipped

   maxY = max(maxY, frontStability*Fy + Y_OFFSET + ERROR_RANGEY - BORDER_OFFSET);  
   minY = min(minY, frontStability*Fy + Y_OFFSET + ERROR_RANGEY - BORDER_OFFSET);
   maxY = max(maxY, frontStability*Fy + Y_OFFSET);
   minY = min(minY, frontStability*Fy + Y_OFFSET);

   //lSkeleton.C4EPoint( JT_SEAT_REF, lLocX, lLocY, lLocZ );
   lLocX.Value(seatRef[0] - lSkeleton.getJoint(JT_ANKLE_CENTER)[0], MetricCns);
   lLocY.Value(seatRef[1] - lSkeleton.getJoint(JT_ANKLE_CENTER)[1], MetricCns);
   maxY = max(maxY,  -(l5s1Loc - lLocY.ValueIn(MetricCns))*Fy + Y_OFFSET);
   minY = min(minY,  -(l5s1Loc - lLocY.ValueIn(MetricCns))*Fy + Y_OFFSET);

   // Check the feet
   // The order of the BOS vector corresponds to the standing
   // balance report
   // Left Foot
   // The OrderedBOS points change depending on the units 
   // set in the program so convert them if they're in english
   //if (lDocPtr->getStance() != STANCE_NO_FEET)
   if(lDocPtr->getFeetSupport() != FS_Neither)
   {
        double conversion = 1;
        if(lDocPtr->Is_English())
                conversion = U_IN_CM;
		std::vector<Vector3> OrderedBOS = lSkeleton.getOrderdBOS();
        maxX = max(maxX, Fx*OrderedBOS[0][0]*conversion);
        minX = min(minX, Fx*OrderedBOS[2][0]*conversion);
        maxY = max(maxY, Fy*OrderedBOS[0][1]*conversion);
        minY = min(minY, Fy*OrderedBOS[2][1]*conversion);

        // Right Foot
        maxX = max(maxX, Fx*OrderedBOS[3][0]*conversion);
        minX = min(minX, Fx*OrderedBOS[5][0]*conversion);
        maxY = max(maxY, Fy*OrderedBOS[3][1]*conversion);
        minY = min(minY, Fy*OrderedBOS[5][1]*conversion);
   }

   //	currentRatio = either dx/w or dy/h, where dy and dx are the distances
   //	between the min and max x/y points (so the width and height of the COP
   //	bouding rect)
   double currentRatio = max((maxX - minX) / double(mCOPDrawingRect.Width()), (maxY - minY) / (float) mCOPDrawingRect.Height() );
   double targetRatio = 0.75;			// We want a 25% buffer (minimum) for
   // top and bottom, left and right
   double factor = targetRatio/currentRatio;

   // Set true center (based on bounding rect of BOS, COP dot)
   center_x = -((maxX + minX) / 2 * factor);
   center_y = -((maxY + minY) / 2 * factor);

   setProjectionMatrix(mCOPDrawingRect.Width(),mCOPDrawingRect.Height());
   setCamera();
   setViewport(mCOPDrawingRect);
   clearColor();
    
   drawSkin(UNITS_CM, Fx, Fy, factor, center_x, center_y );	
   drawPelvis(w, h, pAng, UNITS_CM, Fx, Fy, factor, center_x, center_y);
   // Boundary Lines
   // JTK - NOTE: MUST BE DRAWN BEFORE THE COP
   drawExcursionLine(factor, center_x, center_y - PELVIS_OFFSET, Fx, Fy);
   //if (lDocPtr->getStance() != STANCE_NO_FEET)
   if(lDocPtr->getFeetSupport() != FS_Neither)
   {
        double xOffsetLeft = 0;
        
        lSkeleton.C4EPoint( JT_LFOOT_CENTER, lLocX, lLocY, lLocZ );
        yOffsetLeft = l5s1Loc - lLocY.ValueIn(MetricCns); //+ lLocY.ValueIn(MetricCns);
        lSkeleton.C4EPoint( JT_RFOOT_CENTER, lLocX, lLocY, lLocZ );
        yOffsetRight = l5s1Loc - lLocY.ValueIn(MetricCns);
       /* yOffsetLeft -= ankleCenterY;
        yOffsetRight -= ankleCenterY;*/
           

        bool rightFoot = true;
        bool metric = true;
        bool seated = true;
        GLBalanceView::drawFoot(center_x, center_y + yOffsetRight * -1 * factor * Fy + Y_OFFSET - PELVIS_OFFSET, factor * Fx, -factor * Fy, lFactors, rightFoot, metric, seated);
        GLBalanceView::drawFoot(center_x, center_y + yOffsetLeft * -1 * factor * Fy + Y_OFFSET - PELVIS_OFFSET, factor * Fx, -factor *Fy, lFactors, !rightFoot, metric, seated);
   }
   
   // Seat COP
   drawCircle(	Fx*SeatX*factor + center_x, Fy*(SeatY)*factor + center_y + Y_OFFSET - PELVIS_OFFSET, 
      RED_SEAT_COP_FILL, GREEN_SEAT_COP_FILL, 
      BLUE_SEAT_COP_FILL, SEAT_COP_FILL_OPACITY, 
      RED_SEAT_COP_LINE, GREEN_SEAT_COP_LINE,
      BLUE_SEAT_COP_LINE, SEAT_COP_LINE_OPACITY,
      factor);
   // Left IT
   drawCircle(-Fx*ITx*factor + center_x, Fy*ITy*factor + center_y + Y_OFFSET - PELVIS_OFFSET, 
      RED_L_IT_FILL, GREEN_L_IT_FILL, 
      BLUE_L_IT_FILL, L_IT_FILL_OPACITY, 
      RED_L_IT_LINE, GREEN_L_IT_LINE, 
      BLUE_L_IT_LINE, L_IT_LINE_OPACITY,
      factor);
   // Right IT
   drawCircle(Fx*ITx*factor + center_x, Fy*ITy*factor + center_y + Y_OFFSET - PELVIS_OFFSET,
      RED_R_IT_FILL, GREEN_R_IT_FILL, 
      BLUE_R_IT_FILL, R_IT_FILL_OPACITY, 
      RED_R_IT_LINE, GREEN_R_IT_LINE, 
      BLUE_R_IT_LINE, R_IT_LINE_OPACITY,
      factor);
   
   swap();
   glPopAttrib();
   
   glDeleteLists(listIdSkin, 1);
   glDeleteLists(listIdPelvis, 1);
   glDeleteLists(listIdTuber, 1);
   glDeleteLists(listIdPelvisCorrection, 1);
   glDeleteLists(listIdRightExcursion, 1);
   glDeleteLists(listIdFrontExcursion, 1);
   glDeleteLists(listIdLeftExcursion, 1);
   glDeleteLists(listIdRightCorner, 1);
   glDeleteLists(listIdLeftCorner, 1);
   return;
}
//=============================================================================
void GLSeatCOP::setProjectionMatrix(float w, float h)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   // JTK - If anything gets rendered outside of the graphic
   // the glPolygonMode(..., GL_LINE) does not work
   // so the box is extended slightly past what is visible
   // so that everything renders correctly
   glOrtho(-w/2.0 - 5, w/2.0 + 5, 
      -h/2.0 - 5, h/2 + 5, -1, 1);

}
//=============================================================================
void GLSeatCOP::drawCircle(float x, float y, 
                           BYTE r_fill, BYTE g_fill, 
                           BYTE b_fill, BYTE o_fill,
                           BYTE r_line, BYTE g_line, 
                           BYTE b_line, BYTE o_line,
                           float factor)
{
   glNewList(listIdTuber, GL_COMPILE);
   for(float i = 0; i <= 2*M_PI; i += 0.1f)
   {
      glVertex3f((SEAT_COP_RADIUS) * cos(i)*factor + x, (SEAT_COP_RADIUS)*sin(i)*factor + y, 0.0f);
   }
   glEndList();

   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_POLYGON);
   glColor4f(r_fill/255.0, g_fill/255.0, b_fill/255.0, o_fill );
   glCallList(listIdTuber);
   glEnd();
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glBegin(GL_POLYGON);
   glColor4f(r_line/255.0, g_line/255.0, b_line/255.0, o_line);
   glCallList(listIdTuber);
   glEnd();
   glPopAttrib();
} 
//=============================================================================
void GLSeatCOP::pelvisCorrection(float pAng, float UNITS_CM, float Fx, float Fy, float factor, long center_x, long center_y)
// This function uses the angle to create a correction display list that makes
// polygons to fill in the extra areas that were filled outside of the outline
// essentially this function makes sure the images look correct by rendering 
// polygons over the extra filled parts. Because of the difference betweeen a 
// filled polygon and the outline, there is no clear cut formula as to how
// each polygon needed to be corrected.  This leads to adding and subtracting
// some manipulation of the values by hand which in turn has left hardcoded 
// values.  Each angle range has its own chunk of code with the top being 
// corrected first.  Also note, that there are two sides to be corrected so
// there is a polygon and then another polygon flipped (changing x to negative).
// It is possible for the flipped polygon to be slightly different (I believe this occurs once).
// Many (Most but not all) of the hardcoded values correspond to certain values in 
// the footoffset but to add those by indexing in to the array would lead to several different 
// #defines for each different index that could be added or subtracted to anyway.
// NOTE: This really should be changed to tesselate the polygons to be drawn correctly
// 
// INPUT:  Angle measure.
// OUTPUT: D_CORRECT_PELVIS display list

{
   glNewList(listIdPelvisCorrection, GL_COMPILE);
   if(pAng > -20)
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glColor4f(RED_SKIN_COLOR/255.0f, GREEN_SKIN_COLOR/255.0f, BLUE_SKIN_COLOR/255.0f, SKIN_OPACITY);
      glBegin(GL_POLYGON);
      glVertex2f(6*Fx*UNITS_CM*factor+center_x - 1, 3*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET); 
      glVertex2f(6*Fx*UNITS_CM*factor+center_x, 2*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);	  
      glVertex2f(0*Fx*UNITS_CM*factor+center_x, 3*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);	  
      glVertex2f(0*Fx*UNITS_CM*factor+center_x, 9*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);	 
      glVertex2f(6*Fx*UNITS_CM*factor+center_x - 1, 3*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glEnd();
      glBegin(GL_POLYGON);
      glVertex2f(-6*Fx*UNITS_CM*factor+center_x + 2, 3*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1 - PELVIS_OFFSET);
      glVertex2f(-6*Fx*UNITS_CM*factor+center_x, 2*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-0*Fx*UNITS_CM*factor+center_x, 3*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-0*Fx*UNITS_CM*factor+center_x, 9*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 2  - PELVIS_OFFSET);
      glVertex2f(-6*Fx*UNITS_CM*factor+center_x + 2, 3*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glEnd();
      glBegin(GL_POLYGON);
      glVertex2f(4.5*Fx*UNITS_CM*factor+center_x + 1, -7*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1 - PELVIS_OFFSET);
      glVertex2f(1*Fx*UNITS_CM*factor+center_x, -8*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 2 - PELVIS_OFFSET);
      glVertex2f(5.5*Fx*UNITS_CM*factor+center_x + 1, -8*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1 - PELVIS_OFFSET);
      glVertex2f(4.5*Fx*UNITS_CM*factor+center_x + 1, -7*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1 - PELVIS_OFFSET);
      glEnd();
      glBegin(GL_POLYGON);
      glVertex2f(-4.5*Fx*UNITS_CM*factor+center_x - 1, -7*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1 - PELVIS_OFFSET);
      glVertex2f(-1*Fx*UNITS_CM*factor+center_x, -8*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 2 - PELVIS_OFFSET);
      glVertex2f(-5.5*Fx*UNITS_CM*factor+center_x - 1, -8*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1 - PELVIS_OFFSET);
      glVertex2f(-4.5*Fx*UNITS_CM*factor+center_x - 1, -7*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1 - PELVIS_OFFSET);
      glEnd();
   }
   else if (( (pAng <= -20) && (pAng > -30) ) )
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glColor4f(RED_SKIN_COLOR/255.0f, GREEN_SKIN_COLOR/255.0f, BLUE_SKIN_COLOR/255.0f, SKIN_OPACITY);
      glBegin(GL_POLYGON);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-7.4*Fx*UNITS_CM*factor+center_x + 1, 2*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-7.7*Fx*UNITS_CM*factor+center_x + 1, 2.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glEnd();

      glBegin(GL_POLYGON);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-7.4*Fx*UNITS_CM*factor+center_x + 2, 2*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-7.4*Fx*UNITS_CM*factor+center_x + 2, 2.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-5.7*Fx*UNITS_CM*factor+center_x + 1, 7*Fy*UNITS_CM*factor+center_y + Y_OFFSET - PELVIS_OFFSET);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glEnd();

      glBegin(GL_TRIANGLES);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-0.5*Fx*UNITS_CM*factor+center_x + 1, 13*Fy*UNITS_CM*factor+center_y + Y_OFFSET  - PELVIS_OFFSET);
      glVertex2f(-5.7*Fx*UNITS_CM*factor+center_x, 6.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - PELVIS_OFFSET);
      glEnd();

      // The other side
      glBegin(GL_POLYGON);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(7.4*Fx*UNITS_CM*factor+center_x - 1, 2*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(7.7*Fx*UNITS_CM*factor+center_x - 1, 2.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glEnd();

      glBegin(GL_POLYGON);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(7.4*Fx*UNITS_CM*factor+center_x - 2, 2*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(7.4*Fx*UNITS_CM*factor+center_x - 2, 2.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(5.7*Fx*UNITS_CM*factor+center_x - 1, 7*Fy*UNITS_CM*factor+center_y + Y_OFFSET - PELVIS_OFFSET);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glEnd();

      glBegin(GL_TRIANGLES);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(0.5*Fx*UNITS_CM*factor+center_x - 1, 13*Fy*UNITS_CM*factor+center_y + Y_OFFSET  - PELVIS_OFFSET);
      glVertex2f(5.7*Fx*UNITS_CM*factor+center_x, 6.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - PELVIS_OFFSET);

      // Bottom Part
      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 1, -7.5*Fy*UNITS_CM*factor + center_y + Y_OFFSET  - PELVIS_OFFSET);
      glVertex2f(4.15*Fx*UNITS_CM*factor+center_x + 3, - 8*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 3 - PELVIS_OFFSET);
      glVertex2f(4.5*Fx*UNITS_CM*factor+center_x - 1, -4.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 6 - PELVIS_OFFSET);
      // Other side
      glVertex2f(-0*Fx*UNITS_CM*factor+center_x - 1, -7.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET  - PELVIS_OFFSET);
      glVertex2f(-4.15*Fx*UNITS_CM*factor+center_x - 3, - 8*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 3 - PELVIS_OFFSET);
      glVertex2f(-4.5*Fx*UNITS_CM*factor+center_x + 1, -4.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 6 - PELVIS_OFFSET);
      glEnd();
   }
   else if( (pAng <= -30) && (pAng > -40) ) 
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glColor4f(RED_SKIN_COLOR/255.0f, GREEN_SKIN_COLOR/255.0f, BLUE_SKIN_COLOR/255.0f, SKIN_OPACITY);
      glBegin(GL_POLYGON);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-7.5*Fx*UNITS_CM*factor+center_x + 1, 2.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-8*Fx*UNITS_CM*factor+center_x + 1, 4*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-8*Fx*UNITS_CM*factor+center_x, 5*Fy*UNITS_CM*factor+center_y + Y_OFFSET- PELVIS_OFFSET);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glEnd();

      glBegin(GL_TRIANGLES);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-0.5*Fx*UNITS_CM*factor+center_x + 2, 15*Fy*UNITS_CM*factor+center_y + Y_OFFSET - PELVIS_OFFSET);
      glVertex2f(-6*Fx*UNITS_CM*factor+center_x + 2, 10.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-6*Fx*UNITS_CM*factor+center_x + 2, 10.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 2 - PELVIS_OFFSET);
      glVertex2f(-7.1*Fx*UNITS_CM*factor+center_x + 1, 6*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(-7*Fx*UNITS_CM*factor+center_x + 1, 6*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-8*Fx*UNITS_CM*factor+center_x, 5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - PELVIS_OFFSET);
      glEnd();
      // The other side
      glBegin(GL_POLYGON);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(7.5*Fx*UNITS_CM*factor+center_x - 1, 2.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(8*Fx*UNITS_CM*factor+center_x - 1, 4*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(8*Fx*UNITS_CM*factor+center_x, 5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - PELVIS_OFFSET);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glEnd();
      glBegin(GL_TRIANGLES);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(0.5*Fx*UNITS_CM*factor+center_x - 2, 15*Fy*UNITS_CM*factor+center_y + Y_OFFSET - PELVIS_OFFSET);
      glVertex2f(6*Fx*UNITS_CM*factor+center_x - 2, 10.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(6*Fx*UNITS_CM*factor+center_x - 2, 10.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 2 - PELVIS_OFFSET);
      glVertex2f(7.1*Fx*UNITS_CM*factor+center_x - 1, 6*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(7*Fx*UNITS_CM*factor+center_x - 1, 6*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(8*Fx*UNITS_CM*factor+center_x, 5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - PELVIS_OFFSET);
      glEnd();
      // The bottom
      glBegin(GL_TRIANGLES);
      glVertex2f(5*Fx*UNITS_CM*factor+center_x + 1, -5*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glVertex2f(6*Fx*UNITS_CM*factor+center_x - 1, -3*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glVertex2f(6*Fx*UNITS_CM*factor+center_x - 1, -5*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 1, -7*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glVertex2f(6*Fx*UNITS_CM*factor+center_x - 1, -5*Fy*UNITS_CM*factor+center_y + Y_OFFSET  - PELVIS_OFFSET);
      glVertex2f(5*Fx*UNITS_CM*factor+center_x - 1, -8*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glEnd();
      // The other side
      glBegin(GL_TRIANGLES);
      glVertex2f(-5*Fx*UNITS_CM*factor+center_x - 1, -5*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glVertex2f(-6*Fx*UNITS_CM*factor+center_x + 1, -3*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glVertex2f(-6*Fx*UNITS_CM*factor+center_x + 1, -5*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);

      glVertex2f(-0*Fx*UNITS_CM*factor+center_x - 1, -7*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glVertex2f(-6*Fx*UNITS_CM*factor+center_x + 1, -5*Fy*UNITS_CM*factor+center_y + Y_OFFSET  - PELVIS_OFFSET);
      glVertex2f(-5*Fx*UNITS_CM*factor+center_x + 1, -8*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glEnd();
   }
   else if ((pAng <= -40) && (pAng > -50))
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glColor4f(RED_SKIN_COLOR/255.0f, GREEN_SKIN_COLOR/255.0f, BLUE_SKIN_COLOR/255.0f, SKIN_OPACITY);
      glBegin(GL_TRIANGLES);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 4.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-7.5*Fx*UNITS_CM*factor+center_x + 1, 4*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-8.5*Fx*UNITS_CM*factor+center_x, 6*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 4.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-8*Fx*UNITS_CM*factor+center_x, 6.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1 - PELVIS_OFFSET);
      glVertex2f(-8*Fx*UNITS_CM*factor+center_x + 2, 8.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(-7.5*Fx*UNITS_CM*factor+center_x, 4*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 4.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-8*Fx*UNITS_CM*factor+center_x + 1, 8.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 4.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-8*Fx*UNITS_CM*factor+center_x + 1, 8.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-6*Fx*UNITS_CM*factor+center_x + 1, 14*Fy*UNITS_CM*factor+center_y + Y_OFFSET - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 4.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-6*Fx*UNITS_CM*factor+center_x + 1, 14*Fy*UNITS_CM*factor+center_y + Y_OFFSET - PELVIS_OFFSET);
      glVertex2f(-3*Fx*UNITS_CM*factor+center_x + 2, 16.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 4.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-3*Fx*UNITS_CM*factor+center_x + 2, 16.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-0.5*Fx*UNITS_CM*factor+center_x + 2, 17.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      // The other side 
      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 4.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(7.5*Fx*UNITS_CM*factor+center_x - 1, 4*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(8.5*Fx*UNITS_CM*factor+center_x, 6*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 4.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(8*Fx*UNITS_CM*factor+center_x, 6.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1 - PELVIS_OFFSET);
      glVertex2f(8*Fx*UNITS_CM*factor+center_x - 2, 8.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(7.5*Fx*UNITS_CM*factor+center_x, 4*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 4.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(8*Fx*UNITS_CM*factor+center_x - 1, 8.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 4.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(8*Fx*UNITS_CM*factor+center_x - 1, 8.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(6*Fx*UNITS_CM*factor+center_x - 1, 14*Fy*UNITS_CM*factor+center_y + Y_OFFSET - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 4.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(6*Fx*UNITS_CM*factor+center_x - 1, 14*Fy*UNITS_CM*factor+center_y + Y_OFFSET - PELVIS_OFFSET);
      glVertex2f(3*Fx*UNITS_CM*factor+center_x - 2, 16.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 4.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(3*Fx*UNITS_CM*factor+center_x - 2, 16.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(0.5*Fx*UNITS_CM*factor+center_x - 2, 17.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glEnd();

      // The bottom
      glBegin(GL_TRIANGLES);
      glVertex2f(4.5*Fx*UNITS_CM*factor+center_x + 1, -3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glVertex2f(6.25*Fx*UNITS_CM*factor+center_x - 1, -2*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glVertex2f(6*Fx*UNITS_CM*factor+center_x - 1, -3.6*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 1, -5.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glVertex2f(6*Fx*UNITS_CM*factor+center_x - 1, -3.6*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1 - PELVIS_OFFSET);
      glVertex2f(5.5*Fx*UNITS_CM*factor+center_x - 1, -6.25*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      // The other side
      glVertex2f(-4.5*Fx*UNITS_CM*factor+center_x - 1, -3.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glVertex2f(-6.25*Fx*UNITS_CM*factor+center_x + 1, -2*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glVertex2f(-6*Fx*UNITS_CM*factor+center_x + 1, -3.6*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 1, -5.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glVertex2f(-6*Fx*UNITS_CM*factor+center_x + 1, -3.6*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1 - PELVIS_OFFSET);
      glVertex2f(-5.5*Fx*UNITS_CM*factor+center_x + 1, -6.25*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glEnd();
   } else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glColor4f(RED_SKIN_COLOR/255.0f, GREEN_SKIN_COLOR/255.0f, BLUE_SKIN_COLOR/255.0f, SKIN_OPACITY);
      glBegin(GL_TRIANGLES);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x, 4*Fy*UNITS_CM*factor+center_y + Y_OFFSET  - PELVIS_OFFSET);
      glVertex2f(-7.5*Fx*UNITS_CM*factor+center_x + 1, 4.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-8.5*Fx*UNITS_CM*factor+center_x, 7*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 4*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-8.5*Fx*UNITS_CM*factor+center_x, 7*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1 - PELVIS_OFFSET);
      glVertex2f(-8*Fx*UNITS_CM*factor+center_x + 2, 8*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(-7.5*Fx*UNITS_CM*factor+center_x, 4.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 4*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-8*Fx*UNITS_CM*factor+center_x + 1, 11*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 4*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-8*Fx*UNITS_CM*factor+center_x + 1, 11*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-5.5*Fx*UNITS_CM*factor+center_x + 1, 16*Fy*UNITS_CM*factor+center_y + Y_OFFSET - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 4*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-5.5*Fx*UNITS_CM*factor+center_x + 1, 16*Fy*UNITS_CM*factor+center_y + Y_OFFSET - PELVIS_OFFSET);
      glVertex2f(-4*Fx*UNITS_CM*factor+center_x + 2, 17*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 2, 4*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-4*Fx*UNITS_CM*factor+center_x + 2, 17*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(-0.5*Fx*UNITS_CM*factor+center_x + 2, 18.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      // The other side 
      glVertex2f(0*Fx*UNITS_CM*factor+center_x, 4*Fy*UNITS_CM*factor+center_y + Y_OFFSET  - PELVIS_OFFSET);
      glVertex2f(7.5*Fx*UNITS_CM*factor+center_x - 1, 4.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(8.5*Fx*UNITS_CM*factor+center_x, 7*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 4*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(8.5*Fx*UNITS_CM*factor+center_x, 7*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1 - PELVIS_OFFSET);
      glVertex2f(8*Fx*UNITS_CM*factor+center_x - 2, 8*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(7.5*Fx*UNITS_CM*factor+center_x, 4.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 4*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(8*Fx*UNITS_CM*factor+center_x - 1, 11*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 4*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(8*Fx*UNITS_CM*factor+center_x - 1, 11*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(5.5*Fx*UNITS_CM*factor+center_x - 1, 16*Fy*UNITS_CM*factor+center_y + Y_OFFSET - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 4*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(5.5*Fx*UNITS_CM*factor+center_x - 1, 16*Fy*UNITS_CM*factor+center_y + Y_OFFSET - PELVIS_OFFSET);
      glVertex2f(4*Fx*UNITS_CM*factor+center_x - 2, 17*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x - 2, 4*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(4*Fx*UNITS_CM*factor+center_x - 2, 17*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);
      glVertex2f(0.5*Fx*UNITS_CM*factor+center_x - 2, 18.5*Fy*UNITS_CM*factor+center_y + Y_OFFSET - 1  - PELVIS_OFFSET);

      // The bottom
      glVertex2f(4*Fx*UNITS_CM*factor+center_x + 1, -3*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glVertex2f(5*Fx*UNITS_CM*factor+center_x - 1, -3*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glVertex2f(5.5*Fx*UNITS_CM*factor+center_x - 1, -5*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);

      glVertex2f(0*Fx*UNITS_CM*factor+center_x + 1, -5*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glVertex2f(4*Fx*UNITS_CM*factor+center_x - 1, -3*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1 - PELVIS_OFFSET);
      glVertex2f(5.5*Fx*UNITS_CM*factor+center_x - 1, -5*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      // The other side
      glVertex2f(-4*Fx*UNITS_CM*factor+center_x - 1, -3*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glVertex2f(-5*Fx*UNITS_CM*factor+center_x + 1, -3*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glVertex2f(-5.5*Fx*UNITS_CM*factor+center_x + 1, -5*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);

      glVertex2f(-0*Fx*UNITS_CM*factor+center_x - 1, -5*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glVertex2f(-4*Fx*UNITS_CM*factor+center_x + 1, -3*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1 - PELVIS_OFFSET);
      glVertex2f(-5.5*Fx*UNITS_CM*factor+center_x + 1, -5*Fy*UNITS_CM*factor+center_y + Y_OFFSET + 1  - PELVIS_OFFSET);
      glEnd();
   }
   glEndList();
   glCallList(listIdPelvisCorrection);
}
//=============================================================================
float GLSeatCOP::findExtreme(	bool xcoordinate, 
                             bool find_max, 
                             float UNITS_CM,
                             float Fx,
                             float Fy)
                             // if xcoordinate is true find the maximum x if not find the maximum y
{
   int rtIndex = 0;
   for (int i = 0; i < NUM_SKIN_POINTS; i++)
   {
      if ((skin[i].x*Fx*UNITS_CM > skin[rtIndex].x*Fx*UNITS_CM && xcoordinate && find_max) || 
         (skin[i].x*Fx*UNITS_CM < skin[rtIndex].x*Fx*UNITS_CM && xcoordinate && !find_max) ||
         (skin[i].y*Fy*UNITS_CM > skin[rtIndex].y*Fy*UNITS_CM && !xcoordinate && find_max) ||
         (skin[i].y*Fy*UNITS_CM < skin[rtIndex].y*Fy*UNITS_CM && !xcoordinate && !find_max))
      {
         rtIndex = i;
      }
   }
   if (xcoordinate)
      return skin[rtIndex].x * Fx * UNITS_CM;
   else
      return skin[rtIndex].y * Fy * UNITS_CM + Y_OFFSET;
}
//=============================================================================
void GLSeatCOP::clearColor()
{
   C_Hom_Doc *   lDocPtr = (C_Hom_Doc*)Get_App_Ptr()->Get_Document_Ptr();
   glClearColor((lDocPtr->getBalColor() & 0xff)/255.0f, ((lDocPtr->getBalColor() & 0xff00) >> 8)/255.0f, ((lDocPtr->getBalColor() & 0xff0000) >> 16)/255.0f, 0.0);
   //glClearColor(0.0, 0.0, 0.0, 1.0); // Used for positioning for printing
   glClear(GL_COLOR_BUFFER_BIT);
}
void GLSeatCOP::drawExcursionLine(float scalingFactor, long centerOffsetX, 
                                  long centerOffsetY, float unitScalingX, 
                                  float unitScalingY)
{
   C_Hom_Doc* pDoc = (C_Hom_Doc*)Get_App_Ptr()->Get_Document_Ptr();
   Skeleton &lSkeleton = *pDoc->GetSkeleton();
   Length lLocX(0,7);
   Length lLocY(0,7);
   Length lLocZ(0,7);
   
   double excursionLineSize = 20;
   double errorRangeX = ERROR_RANGEX;     // Make sure to make these constants
   double errorRangeY = ERROR_RANGEY;  
   // The colors for the boundary object
   float colorRed = RED_BOUNDARY_COLOR / 255.0f;
   float colorGreen = GREEN_BOUNDARY_COLOR / 255.0f;
   float colorBlue = BLUE_BOUNDARY_COLOR / 255.0f;
   float colorOpacity = BOUNDARY_OPACITY;
   excursionLineSize =  (excursionLineSize * scalingFactor * unitScalingY);
   excursionLineSize += centerOffsetY + Y_OFFSET;
   double excursionCM, farthestLocation = 0;
   //excursionCM = Analysis::CalculateSeatedExcursion();
    const double EXCURSION_CONST = 0.54;
    const double AZIMUTH_CONST = 0.275;
    const double STATURE_CONST = .000167;       // 1.69 * 10^-4
    const double AZIMUTH = 1;                   // 1 if Azimuth is 0 or 30 and -1 if Azimuth is -30
    const double AVG_AGE = 41.5;		        // The average of a person 18 - 65 so (18+65) / 2.0
    const double AGE_CONST = .00255;            // 2.55 * 10^-3

    // The excusion formula is from Center of Pressure Excursion Capability in Performance of Seated Lateral-Reaching Tasks
    // Azimuth is currently set to zero because the hips don't rotate.  
	excursionCM = EXCURSION_CONST + (STATURE_CONST * pDoc->getAnthropometryRef().GetHeight(MetricCns) * 10/*U_CM_MM*/) - (AGE_CONST * AVG_AGE) + (pDoc->GetFactors().mLink_HipWidth * pDoc->getAnthropometryRef().GetHeight(MetricCns) * 10/*U_CM_MM*/) + (AZIMUTH_CONST * AZIMUTH);

    excursionCM *= 0.01;//U_MM_CM;	            // Convert excursion into cm

   // Add in the center offset and multiply in the scaling factor=
   excursionCM = excursionCM * scalingFactor * unitScalingX + centerOffsetX;
   errorRangeX = errorRangeX * scalingFactor * unitScalingX;
   errorRangeY = errorRangeY * scalingFactor * unitScalingY;

   lSkeleton.C4EPoint(JT_L5S1_TEMP, lLocX, lLocY, lLocZ);
   //farthestLocation = Analysis::FindFrontStability() - lLocY.ValueIn(MetricCns);
   Vector3 seatRef = lSkeleton.getSeatReference();
    if(pDoc->getFeetSupport() == FS_Neither) {
		farthestLocation = lSkeleton.getSeatCenter()[1] - seatRef[1];
	} else if(pDoc->getFeetSupport() == FS_Left) {
		farthestLocation = lSkeleton.getJoint(JT_LBALL)[1] - seatRef[1];
	} else if(pDoc->getFeetSupport() == FS_Right) {
		farthestLocation = lSkeleton.getJoint(JT_RBALL)[1] - seatRef[1];
	} else {
		farthestLocation = lSkeleton.getJoint(JT_LBALL)[1] - seatRef[1];
		double rball = lSkeleton.getJoint(JT_RBALL)[1] - seatRef[1];
		if(rball > farthestLocation) {
			farthestLocation = rball;
		}
	}
   farthestLocation -= lLocY.ValueIn(MetricCns);
   farthestLocation = (farthestLocation * scalingFactor 
      * unitScalingY);
   farthestLocation +=  centerOffsetY + Y_OFFSET;        
   // Set up all of the objects to be rendered

   lSkeleton.C4EPoint( JT_L5S1_TEMP, lLocX, lLocY, lLocZ);
   double l5s1Loc = lLocY.ValueIn(MetricCns);
   //if(pDoc->getPositioning() & HAS_BACK)
   if(pDoc->hasSeatBackRest())
   {
		//lSkeleton.C4EPoint( JT_SEATBACK_CENTER, lLocX, lLocY, lLocZ );
	   Vector3 seatBack = lSkeleton.getSeatBackCenter();
	   lLocY.Value(seatBack[1] - lSkeleton.getJoint(JT_ANKLE_CENTER)[1], MetricCns);
		centerOffsetY = centerOffsetY - (l5s1Loc - lLocY.ValueIn(MetricCns)) * scalingFactor * unitScalingY;
   } else {
		lSkeleton.C4EPoint( JT_LIT, lLocX, lLocY, lLocZ);
		centerOffsetY = centerOffsetY - (l5s1Loc - lLocY.ValueIn(MetricCns)) * scalingFactor * unitScalingY;
   }
   glLineWidth(2.0f);
   glNewList(listIdRightExcursion, GL_COMPILE);
   glColor4f(colorRed, colorGreen, colorBlue, colorOpacity);
   glBegin(GL_POLYGON);
   glVertex3d(excursionCM,centerOffsetY + Y_OFFSET, 0.0f);
   glVertex3d(excursionCM-errorRangeX, centerOffsetY + Y_OFFSET, 0.0f);
   glVertex3d(excursionCM-errorRangeX, farthestLocation, 0.0f);
   glVertex3d(excursionCM, farthestLocation, 0.0f);
   glEnd();
   glColor4f(0.0, 0.0, 0.0, 1.0f);
   glBegin(GL_LINES);
   // Bottom
   glVertex3d(excursionCM,centerOffsetY + Y_OFFSET, 0.0f);
   glVertex3d(excursionCM-errorRangeX, centerOffsetY + Y_OFFSET, 0.0f);
   // Side (Left - user perspective)
   glVertex3d(excursionCM-errorRangeX, centerOffsetY + Y_OFFSET, 0.0f);
   glVertex3d(excursionCM-errorRangeX, farthestLocation, 0.0f);
   // Side (Right - user perspective)
   glVertex3d(excursionCM,centerOffsetY + Y_OFFSET, 0.0f);
   glVertex3d(excursionCM, farthestLocation, 0.0f);
   glEnd();
   glEndList();

   glNewList(listIdLeftExcursion, GL_COMPILE);
  
   glColor4f(colorRed, colorGreen, colorBlue, colorOpacity);
   glBegin(GL_POLYGON);
   glVertex3d(-excursionCM,centerOffsetY + Y_OFFSET, 0.0f);
   glVertex3d(-excursionCM+errorRangeX, centerOffsetY + Y_OFFSET, 0.0f);
   glVertex3d(-excursionCM+errorRangeX, farthestLocation, 0.0f);
   glVertex3d(-excursionCM, farthestLocation, 0.0f);
   glEnd();
   glColor4f(0.0, 0.0, 0.0, 1.0f);

   

   glBegin(GL_LINES);
   // Bottom
   glVertex3d(-excursionCM,centerOffsetY + Y_OFFSET, 0.0f);
   glVertex3d(-excursionCM+errorRangeX, centerOffsetY + Y_OFFSET, 0.0f);
   // Side (Left - user perspective)
   glVertex3d(-excursionCM+errorRangeX, centerOffsetY + Y_OFFSET, 0.0f);
   glVertex3d(-excursionCM+errorRangeX, farthestLocation, 0.0f);
   // Side (Right - user perspective)
   glVertex3d(-excursionCM,centerOffsetY + Y_OFFSET, 0.0f);
   glVertex3d(-excursionCM, farthestLocation, 0.0f);
   glEnd();
   glEndList();
   glNewList(listIdFrontExcursion, GL_COMPILE);

   glColor4f(colorRed, colorGreen, colorBlue, colorOpacity);
   glBegin(GL_POLYGON);
   glVertex3d(-excursionCM, farthestLocation, 0.0f);
   glVertex3d(excursionCM, farthestLocation, 0.0f);
   glVertex3d(excursionCM, farthestLocation - errorRangeY, 0.0f);
   glVertex3d(-excursionCM, farthestLocation - errorRangeY, 0.0f);
   glEnd();
   glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
   glBegin(GL_LINES);
   // Bottom
   glVertex3d(-excursionCM + errorRangeX, farthestLocation, 0.0f);
   glVertex3d(excursionCM - errorRangeX, farthestLocation, 0.0f);
   // Top
   glVertex3d(excursionCM - errorRangeX, farthestLocation - errorRangeY, 0.0f);
   glVertex3d(-excursionCM + errorRangeX, farthestLocation - errorRangeY, 0.0f);
   glEnd();
   glEndList();
   glNewList(listIdRightCorner, GL_COMPILE);
   glColor4f(colorRed, colorGreen, colorBlue, colorOpacity);
   glBegin(GL_POLYGON);
   glVertex3d(-excursionCM + errorRangeX, farthestLocation, 0.0f);
   glVertex3d(-excursionCM + errorRangeX, farthestLocation-errorRangeY, 0.0f);
   glVertex3d(-excursionCM, farthestLocation-errorRangeY, 0.0f);
   glVertex3d(-excursionCM, farthestLocation, 0.0f);
   glEnd();
   glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
   glBegin(GL_LINES);
   glVertex3d(-excursionCM, farthestLocation-errorRangeY, 0.0f);
   glVertex3d(-excursionCM, farthestLocation, 0.0f);
   glVertex3d(-excursionCM, farthestLocation, 0.0f);
   glVertex3d(-excursionCM + errorRangeX, farthestLocation, 0.0f);
   glEnd();
   glEndList();
   glNewList(listIdLeftCorner, GL_COMPILE);
   glColor4f(colorRed, colorGreen, colorBlue, colorOpacity);
   glBegin(GL_POLYGON);
   glVertex3d(excursionCM - errorRangeX, farthestLocation, 0.0f);
   glVertex3d(excursionCM - errorRangeX, farthestLocation-errorRangeY, 0.0f);
   glVertex3d(excursionCM, farthestLocation-errorRangeY, 0.0f);
   glVertex3d(excursionCM, farthestLocation, 0.0f);
   glEnd();

   glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

   glBegin(GL_LINES);
   glVertex3d(excursionCM, farthestLocation-errorRangeY, 0.0f);
   glVertex3d(excursionCM, farthestLocation, 0.0f);
   glVertex3d(excursionCM, farthestLocation, 0.0f);
   glVertex3d(excursionCM - errorRangeX, farthestLocation, 0.0f);
   glEnd();
   glEndList();
   // Render Everything
   glCallList(listIdLeftExcursion);
   glCallList(listIdRightExcursion);
   glCallList(listIdFrontExcursion); 
   glCallList(listIdRightCorner); 
   glCallList(listIdLeftCorner);
   glLineWidth(1.0f);

   return;
}

