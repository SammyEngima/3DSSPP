#include "GLBalanceSitting.h"
#include "GLBalanceStanding.h"
#include "Skeleton.hpp"
#include "Hom_Doc.h"
#include "Length.hpp"

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

// COG
#define RED_SEAT_COG_LINE 0.0f
#define GREEN_SEAT_COG_LINE 0.0f
#define BLUE_SEAT_COG_LINE 0.0f
#define SEAT_COG_LINE_OPACITY 1.0f

#define RED_SEAT_COG_FILL 247.0f
#define GREEN_SEAT_COG_FILL 247.0f
#define BLUE_SEAT_COG_FILL 0.0f
#define SEAT_COG_FILL_OPACITY 1.0f

// COP
#define RED_SEAT_COP_LINE 0.0f
#define GREEN_SEAT_COP_LINE 0.0f
#define BLUE_SEAT_COP_LINE 0.0f
#define SEAT_COP_LINE_OPACITY 1.0f

#define RED_SEAT_COP_FILL 255.0f
#define GREEN_SEAT_COP_FILL 0.0f
#define BLUE_SEAT_COP_FILL 0.0f
#define SEAT_COP_FILL_OPACITY 1.0f

// Black
#define RED_L_IT_LINE 0.0f
#define GREEN_L_IT_LINE 0.0f
#define BLUE_L_IT_LINE 0.0f
#define L_IT_LINE_OPACITY 1.0f

// Red
#define RED_L_IT_FILL 0.0f
#define GREEN_L_IT_FILL 0.0f
#define BLUE_L_IT_FILL 0.0f
#define L_IT_FILL_OPACITY 1.0f

// Black 
#define RED_R_IT_LINE 0.0f
#define GREEN_R_IT_LINE 0.0f
#define BLUE_R_IT_LINE 0.0f
#define R_IT_LINE_OPACITY 1.0f

// Red
#define RED_R_IT_FILL 0.0f
#define GREEN_R_IT_FILL 0.0f
#define BLUE_R_IT_FILL 0.0f
#define R_IT_FILL_OPACITY 1.0f

// Green
#define RED_BOUNDARY_COLOR 0.0f
#define GREEN_BOUNDARY_COLOR 128.0f
#define BLUE_BOUNDARY_COLOR 0.0f
#define BOUNDARY_OPACITY 1.0f

#define PELVIS_OFFSET 5
const float SEAT_COP_RADIUS  = 8.0f;

GLBalanceSitting::GLBalanceSitting(CWnd* owner) : GLBasic(owner) {
	for(int i = 0; i < 3; i++) {
		clearColor[i] = 1;
	}
}

GLBalanceSitting::~GLBalanceSitting(void) {
}

void GLBalanceSitting::draw() {
// This function does all of the necessary calculations to create the seat
// center of pressure graphic.
// INPUT: Rectangle describing opengl drawing region
// OUTPUT: OpenGL graphic
// NOTE:  The opengl context must already be created for this function work correctly

   Length lLocX(0, 7);
   Length lLocY(0, 7);
   Length lLocZ(0, 7);

   Skeleton &lSkeleton = *pDoc->GetSkeleton();
   const BalanceData_s balance_data = lSkeleton.getBalanceData(true);

   const Factors &lFactors = pDoc->GetFactors();
   // Used to make sure the front boundary does not end up on the border
   const float BORDER_OFFSET = 2.0f;   
   float Fx,Fy,UNITS_CM, pAng, SeatX, SeatY;
   float LITx, LITy, RITx, RITy;
   float maxX, maxY, minX, minY;
   float w = viewingRect.Width();
   float h = viewingRect.Height();
   double frontStability;    // Used for scaling
   long center_x, center_y;
   double yOffsetLeft = 0, yOffsetRight = 0;
   double l5s1Loc;
   //lSkeleton.C4EPoint( JT_L5S1_TEMP, lLocX, lLocY, lLocZ);
   l5s1Loc = lSkeleton.C4EPoint(JT_L5S1_TEMP, true)[1];
   //l5s1Loc = lLocY.ValueIn(MetricCns);

   Vector3 IT = lSkeleton.C4EPoint(JT_LIT, true);
   IT[0] = lSkeleton.getJoint(JT_LIT, true)[0];
   //lSkeleton.C4EPoint( JT_LIT, lLocX, lLocY, lLocZ );
   LITx = IT[0];//lLocX.ValueIn(MetricCns);
   //ITy = -(l5s1Loc - lLocY.ValueIn(MetricCns));
   LITy = -(l5s1Loc - IT[1]);
   IT = lSkeleton.C4EPoint(JT_RIT, true);
   IT[0] = lSkeleton.getJoint(JT_RIT, true)[0];
   RITx = IT[0];
   RITy = -(l5s1Loc - IT[1]);
   
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   pAng = lSkeleton.getExtraAngle(EA_PelvisFlexion);
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
   UNITS_CM = ( 0.07627 * pDoc->getAnthropometryRef().GetHeight(MetricCns)) / 20;	
   
   //double convert = 1;
   double convert = 100;// U_M_CM;
   //if(lDocPtr->Is_English())
   //     convert = U_IN_CM;
   // Get values for the circle
   Vector3 ankleCenter = lSkeleton.getJoint(JT_ANKLE_CENTER, true);
   SeatY = -(l5s1Loc - balance_data.CenterOfPressure[1]);// * convert) ;
   SeatX = balance_data.CenterOfPressure[0]+ankleCenter[0];// * convert;
   float COGy = -(l5s1Loc - balance_data.CenterOfGravity[1]);
   float COGx = balance_data.CenterOfGravity[0]+ankleCenter[0];

   // Find the max/min values for zooming
   maxX = max(Fx*SeatX, Fx*RITx);
   maxY = max(Fy*(SeatY) + Y_OFFSET - PELVIS_OFFSET, max(Fy*LITy + Y_OFFSET - PELVIS_OFFSET,Fy*RITy + Y_OFFSET - PELVIS_OFFSET));
   minX = min(Fx*SeatX, Fx*LITx);
   minY = min(Fy*(SeatY) + Y_OFFSET - PELVIS_OFFSET, min(Fy*LITy + Y_OFFSET - PELVIS_OFFSET,Fy*RITy + Y_OFFSET - PELVIS_OFFSET));

   // make cog in window
   maxX = max(maxX, Fx*COGx);
   minX = min(minX, Fx*COGx);
   maxY = max(maxY, Fy*COGy + Y_OFFSET - PELVIS_OFFSET);
   minY = min(minY, Fy*COGy + Y_OFFSET - PELVIS_OFFSET);

   maxX = max(maxX,-Fx*SeatX);
   minX = min(minX, -Fx*SeatX);
   maxX = max(maxX, findExtreme(true, true, UNITS_CM, Fx, Fy));
   maxY = max(maxY, findExtreme(false, true, UNITS_CM, Fx, Fy));
   minX = min(minX, findExtreme(true, false, UNITS_CM, Fx, Fy));
   minY = min(minY, findExtreme(false, false, UNITS_CM, Fx, Fy));
   // Check the front stability
   //frontStability = Analysis::FindFrontStability() -  l5s1Loc; // the y-axis is flipped
   Vector3 seatRef = lSkeleton.getSeatReference(true);
	if(pDoc->getFeetSupport() == FS_Neither) {
		frontStability = lSkeleton.getSeatCenter(true)[1] - ankleCenter[1];//seatRef[1];
	} else if(pDoc->getFeetSupport() == FS_Left) {
		frontStability = lSkeleton.getJoint(JT_LBALL,true)[1] - ankleCenter[1];//seatRef[1];
	} else if(pDoc->getFeetSupport() == FS_Right) {
		frontStability = lSkeleton.getJoint(JT_RBALL,true)[1] - ankleCenter[1];//seatRef[1];
	} else {
		frontStability = lSkeleton.getJoint(JT_LBALL,true)[1] - ankleCenter[1];//seatRef[1];
		double rball = lSkeleton.getJoint(JT_RBALL,true)[1] - ankleCenter[1];//seatRef[1];
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
   lLocX.Value(seatRef[0] - lSkeleton.getJoint(JT_ANKLE_CENTER,true)[0], MetricCns);
   lLocY.Value(seatRef[1] - lSkeleton.getJoint(JT_ANKLE_CENTER,true)[1], MetricCns);
   maxY = max(maxY,  -(l5s1Loc - lLocY.ValueIn(MetricCns))*Fy + Y_OFFSET);
   minY = min(minY,  -(l5s1Loc - lLocY.ValueIn(MetricCns))*Fy + Y_OFFSET);

   // Check the feet
   // The order of the BOS vector corresponds to the standing
   // balance report
   // Left Foot
   // The OrderedBOS points change depending on the units 
   // set in the program so convert them if they're in english
   //if (lDocPtr->getStance() != STANCE_NO_FEET)
   if(pDoc->getFeetSupport() != FS_Neither)
   {
		std::vector<Vector3> OrderedBOS = lSkeleton.getOrderdBOS();
		maxX = max(maxX, Fx*OrderedBOS[0][0]);
		minX = min(minX, Fx*OrderedBOS[2][0]);
		maxY = max(maxY, Fy*OrderedBOS[0][1]);
		minY = min(minY, Fy*OrderedBOS[2][1]);

		// Right Foot
		maxX = max(maxX, Fx*OrderedBOS[3][0]);
		minX = min(minX, Fx*OrderedBOS[5][0]);
		maxY = max(maxY, Fy*OrderedBOS[3][1]);
		minY = min(minY, Fy*OrderedBOS[5][1]);
   }

   //	currentRatio = either dx/w or dy/h, where dy and dx are the distances
   //	between the min and max x/y points (so the width and height of the COP
   //	bouding rect)
   double currentRatio = max((maxX - minX) / double(viewingRect.Width()), (maxY - minY) / (float) viewingRect.Height() );
   double targetRatio = 0.75;			// We want a 25% buffer (minimum) for
   // top and bottom, left and right
   double factor = targetRatio/currentRatio;

   // Set true center (based on bounding rect of BOS, COP dot)
   center_x = -((maxX + minX) / 2 * factor);
   center_y = -((maxY + minY) / 2 * factor);

   glPushMatrix();
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   // JTK - If anything gets rendered outside of the graphic
   // the glPolygonMode(..., GL_LINE) does not work
   // so the box is extended slightly past what is visible
   // so that everything renders correctly
   if(viewingRect.Height() > 0) {				// make sure we don't divide by zero
	   glViewport(	viewingRect.left, viewingRect.top, viewingRect.Width(), viewingRect.Height() );
   }
   w = viewingRect.Width();
   h = viewingRect.Height();
   glOrtho(-w/2.0 - 5, w/2.0 + 5, 
	  -h/2.0 - 5, h/2 + 5, -1, 1);

   
	glColor4f(1,1,1,1);
	glBegin(GL_POLYGON);
	glVertex2f(-w/2.0 - 5,-h/2.0 - 5);
	glVertex2f(w/2.0 + 5,-h/2.0 - 5);
	glVertex2f(w/2.0 + 5,h/2 + 5);
	glVertex2f(-w/2.0 - 5,h/2 + 5);
	glEnd();

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(	0.0f, 0.0f, -1.0f,			// Gives the standard x,y plane centered at the origin
				0.0f, 0.0f, 0.0f,
				0.0f, -1.0f, 0.0f);
   //glClear(GL_COLOR_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);

	// Begin Antialiasing
	glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH); JTK - Removed so that it would not draw extra lines on certain graphics cards
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
   drawSkin(UNITS_CM, Fx, Fy, factor, center_x, center_y );	
   drawPelvis(w, h, pAng, UNITS_CM, Fx, Fy, factor, center_x, center_y);
   // Boundary Lines
   // JTK - NOTE: MUST BE DRAWN BEFORE THE COP
   drawExcursionLine(factor, center_x, center_y - PELVIS_OFFSET, Fx, Fy);

   if(pDoc->getFeetSupport() != FS_Neither)
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
		GLBalanceStanding::drawFoot(center_x, center_y + yOffsetRight * -1 * factor * Fy + Y_OFFSET - PELVIS_OFFSET, factor * Fx, -factor * Fy, lFactors, rightFoot, metric, seated);
		GLBalanceStanding::drawFoot(center_x, center_y + yOffsetLeft * -1 * factor * Fy + Y_OFFSET - PELVIS_OFFSET, factor * Fx, -factor *Fy, lFactors, !rightFoot, metric, seated);
   }
   
   // Seat COG
   //drawCircle(	Fx*SeatX*factor + center_x, Fy*(SeatY)*factor + center_y + Y_OFFSET - PELVIS_OFFSET, 
   if(COG)
   {
	   drawCircle(Fx*COGx*factor + center_x, Fy*(COGy)*factor + center_y + Y_OFFSET - PELVIS_OFFSET, 
		  RED_SEAT_COG_FILL, GREEN_SEAT_COG_FILL, 
		  BLUE_SEAT_COG_FILL, SEAT_COG_FILL_OPACITY, 
		  RED_SEAT_COG_LINE, GREEN_SEAT_COG_LINE,
		  BLUE_SEAT_COG_LINE, SEAT_COG_LINE_OPACITY,
		  factor);
   }
   // Left IT
   drawCircle(Fx*LITx*factor + center_x, Fy*RITy*factor + center_y + Y_OFFSET - PELVIS_OFFSET, 
	  RED_L_IT_FILL, GREEN_L_IT_FILL, 
	  BLUE_L_IT_FILL, L_IT_FILL_OPACITY, 
	  RED_L_IT_LINE, GREEN_L_IT_LINE, 
	  BLUE_L_IT_LINE, L_IT_LINE_OPACITY,
	  factor);
   // Right IT
   drawCircle(Fx*RITx*factor + center_x, Fy*RITy*factor + center_y + Y_OFFSET - PELVIS_OFFSET,
	  RED_R_IT_FILL, GREEN_R_IT_FILL, 
	  BLUE_R_IT_FILL, R_IT_FILL_OPACITY, 
	  RED_R_IT_LINE, GREEN_R_IT_LINE, 
	  BLUE_R_IT_LINE, R_IT_LINE_OPACITY,
	  factor);

   /*drawCOP(Fx*SeatX*factor + center_x, Fy*SeatY*factor + center_y + Y_OFFSET - PELVIS_OFFSET, factor);*/
   // Seat COP
   drawCircle(Fx*SeatX*factor + center_x, Fy*SeatY*factor + center_y + Y_OFFSET - PELVIS_OFFSET,
	  RED_SEAT_COP_FILL, GREEN_SEAT_COP_FILL, 
	  BLUE_SEAT_COP_FILL, SEAT_COP_FILL_OPACITY, 
	  RED_SEAT_COP_LINE, GREEN_SEAT_COP_LINE,
	  BLUE_SEAT_COP_LINE, SEAT_COP_LINE_OPACITY,
	  factor);

   glPopMatrix();
   glPopAttrib();
}
void GLBalanceSitting::drawSkin(float UNITS_CM, //factor
					float Fx,
					float Fy,
					float factor,
					long center_x,
					long center_y) {

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glColor4f(RED_SKIN_COLOR/255.0f,
			  GREEN_SKIN_COLOR/255.0f,
			BLUE_SKIN_COLOR/255.0f,
			SKIN_OPACITY);
   glBegin(GL_POLYGON);
	for(int i = 0; i < NUM_SKIN_POINTS; i++) {
	  glVertex3f(skin[i].x*Fx*UNITS_CM*factor + center_x, (skin[i].y*Fy*UNITS_CM)*factor + center_y + Y_OFFSET, 0.0);
	}
   glEnd();
}
void GLBalanceSitting::drawPelvis(float w, //Width
					float h, //Height
					float pAng,
					float UNITS_CM, //factor
					float Fx,
					float Fy,
					float factor,
					long center_x,
					long center_y) {
	int listIdPelvis = glGenLists(1);
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
   glDeleteLists(listIdPelvis, 1);
}

void GLBalanceSitting::drawCircle(float x, float y, 
					BYTE r_fill, BYTE g_fill, 
					BYTE b_fill, BYTE o_fill,
					BYTE r_line, BYTE g_line, 
					BYTE b_line, BYTE o_line,
					float factor) {
	int listIdTuber = glGenLists(1);
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

   glDeleteLists(listIdTuber, 1);
}

void GLBalanceSitting::drawCOP(float x, float y, float factor) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor4f(0,0,0,1);
	glLineWidth(3*factor);
	glBegin(GL_LINES);
		glVertex2f(x - 4, y);
		glVertex2f(x + 4, y);
		glVertex2f(x, y - 4);
		glVertex2f(x, y + 4);
	glEnd();
	glColor4f(0,1,0,1);
	glLineWidth(1*factor);
	glBegin(GL_LINES);
		glVertex2f(x - 3, y);
		glVertex2f(x + 3, y);
		glVertex2f(x, y - 3);
		glVertex2f(x, y + 3);
	glEnd();
	glPopAttrib();
}

void GLBalanceSitting::pelvisCorrection(float pAng, float UNITS_CM, float Fx, float Fy,
										float factor, long center_x, long center_y) {
	if(pAng > -20) {
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
}

float GLBalanceSitting::findExtreme(bool xcoordinate, bool find_max, float UNITS_CM, float Fx, float Fy) {
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
// Finds the front stability value for the seated graphic
void GLBalanceSitting::drawExcursionLine(float scalingFactor, long centerOffsetX,
							long centerOffsetY, float unitScalingX, 
							float unitScalingY) {
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
	excursionCM *= 0.1;//U_MM_CM;	            // Convert excursion into cm

   // Add in the center offset and multiply in the scaling factor=
   excursionCM = excursionCM * scalingFactor * unitScalingX + centerOffsetX;
   errorRangeX = errorRangeX * scalingFactor * unitScalingX;
   errorRangeY = errorRangeY * scalingFactor * unitScalingY;


   glPointSize(5);
   glColor3ub(255,0,0);
   glBegin(GL_POINTS);
   //glVertex3d(0,0,0);
   //glVertex3d(scalingFactor*unitScalingX,0,0);
   //glVertex3d(0,scalingFactor*unitScalingY,0);
   glEnd();

   glLineWidth(2.0f);
   Vector3 FLC, FRC, BRC, BLC, FL, FR, BR, BL;
   Vector3 L5 = lSkeleton.getJoint(JT_L5S1_TEMP, true);

   FeetSupport_e fs = lSkeleton.getTask()->getFeetSupport();
   if(fs == FS_Left || fs == FS_Both) {
	   FL = lSkeleton.getJoint(JT_LBALL, true);
   } else {
	   FL = lSkeleton.getLeftSeatFront(true);
   }
   if(fs == FS_Right || fs == FS_Both) {
	FR = lSkeleton.getJoint(JT_RBALL, true);
   } else {
	   FR = lSkeleton.getRightSeatFront(true);
   }
   BR = Vector3(excursionCM, lSkeleton.getJoint(JT_RIT,true)[1],0);
   BL = Vector3(-excursionCM, lSkeleton.getJoint(JT_LIT,true)[1],0);
   FL -= L5;
   FR -= L5;
   BR[1] -= L5[1];
   BL[1] -= L5[1];
   FL *= scalingFactor;
   FR *= scalingFactor;
   BR[1] *= scalingFactor;
   BL[1] *= scalingFactor;
   FL[0] *= unitScalingX;
   FR[0] *= unitScalingX;
   FL[1] *= unitScalingY;
   FR[1] *= unitScalingY;
   BR[1] *= unitScalingY;
   BL[1] *= unitScalingY;
   FL[2] = FR[2] = BR[2] = BL[2] = 0;
   Vector3 offset(centerOffsetX, centerOffsetY + Y_OFFSET,0);
   FL += offset;
   FR += offset;
   BR[1] += centerOffsetY + Y_OFFSET;
   BL[1] += centerOffsetY + Y_OFFSET;
   FLC = FL + Vector3(errorRangeX, -errorRangeY,0);
   FRC = FR + Vector3(-errorRangeX, -errorRangeY,0);
   BRC = BR + Vector3(-errorRangeX, 0, 0);
   BLC = BL + Vector3(errorRangeX, 0, 0);

   glColor4f(colorRed, colorGreen, colorBlue, colorOpacity);
   glBegin(GL_QUAD_STRIP);
   glVertex3dv(BL.ptr());
   glVertex3dv(BLC.ptr());
   glVertex3dv(FL.ptr());
   glVertex3dv(FLC.ptr());
   glVertex3dv(FR.ptr());
   glVertex3dv(FRC.ptr());
   glVertex3dv(BR.ptr());
   glVertex3dv(BRC.ptr());
   glEnd();

   glColor4f(0.0,0.0,0.0,1.0f);
   glBegin(GL_LINE_LOOP);
   glVertex3dv(BL.ptr());
   glVertex3dv(FL.ptr());
   glVertex3dv(FR.ptr());
   glVertex3dv(BR.ptr());
   glVertex3dv(BRC.ptr());
   glVertex3dv(FRC.ptr());
   glVertex3dv(FLC.ptr());
   glVertex3dv(BLC.ptr());
   glEnd();

/*
   lSkeleton.C4EPoint(JT_L5S1_TEMP, lLocX, lLocY, lLocZ);
   //farthestLocation = Analysis::FindFrontStability() - lLocY.ValueIn(MetricCns);
   Vector3 seatRef = lSkeleton.getSeatReference(true);
   Vector3 ankleCenter = lSkeleton.getJoint(JT_ANKLE_CENTER, true);
	if(pDoc->getFeetSupport() == FS_Neither) {
		farthestLocation = lSkeleton.getSeatCenter(true)[1] - ankleCenter[1];//seatRef[1];
	} else if(pDoc->getFeetSupport() == FS_Left) {
		farthestLocation = lSkeleton.getJoint(JT_LBALL,true)[1] - ankleCenter[1];//seatRef[1];
	} else if(pDoc->getFeetSupport() == FS_Right) {
		farthestLocation = lSkeleton.getJoint(JT_RBALL,true)[1] - ankleCenter[1];//seatRef[1];
	} else {
		farthestLocation = lSkeleton.getJoint(JT_LBALL,true)[1] - ankleCenter[1];//seatRef[1];
		double rball = lSkeleton.getJoint(JT_RBALL,true)[1] - ankleCenter[1];//seatRef[1];
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
	   Vector3 seatBack = lSkeleton.getSeatBackCenter(true);
	   lLocY.Value(seatBack[1] - lSkeleton.getJoint(JT_ANKLE_CENTER,true)[1], MetricCns);
		centerOffsetY = centerOffsetY - (l5s1Loc - lLocY.ValueIn(MetricCns)) * scalingFactor * unitScalingY;
   } else {
		lSkeleton.C4EPoint( JT_LIT, lLocX, lLocY, lLocZ);
		centerOffsetY = centerOffsetY - (l5s1Loc - lLocY.ValueIn(MetricCns)) * scalingFactor * unitScalingY;
   }
   glLineWidth(2.0f);

   //glNewList(listIdRightExcursion, GL_COMPILE);
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
   //glEndList();

   //glNewList(listIdLeftExcursion, GL_COMPILE);
  
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
   //glEndList();

   //glNewList(listIdFrontExcursion, GL_COMPILE);

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
   //glEndList();

   //glNewList(listIdRightCorner, GL_COMPILE);
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
   //glEndList();

   //glNewList(listIdLeftCorner, GL_COMPILE);
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
   //glEndList();

   glLineWidth(1.0f);*/
}

floatpoint3 GLBalanceSitting::skin[NUM_SKIN_POINTS] = {	{0, -6.5}, {-1, -6.5}, 
											{-3.5, -7.2}, {-6, -7.5}, 
											{-8.5, -7.5}, {-11, -7}, 
											{-13, -6.5}, {-15, -5.6}, 
											{-16.3, -5}, {-17.6, -4},
											{-19, -2.2}, {-20, -0.5},
											{-20.6, 1}, {-21, 3},
											{-21, 30}, {-2, 30},
											{-2, 25}, {-1.8, 23.3},
											{-1, 22.3}, {0, 22},
											{1, 22.3}, {1.8, 23.3},
											{2, 25}, {2, 30},
											{21, 30}, {21, 3},
											{20.6, 1}, {20, -0.5},
											{19, -2.2}, {17.6, -4},
											{16.3, -5}, {15, -5.6},
											{13, -6.5}, {11, -7},
											{8.5, -7.5}, {6, -7.5},
											{3.5, -7.2}, {1, -6.5},
											{0, -6.5}};

floatpoint3 GLBalanceSitting::pelvisLessThan15[NUM_PELVIS_POINTS] = {	{0, -9}, {4.5, -7},
															{5.5, -6.5}, {5.35, -7.35},
															{5.5, -8}, {15, 0},
															{19, 5}, {19, 9},
															{17.5, 12.5}, {15, 10},
															{14, 10}, {13, 9},
															{10, 10}, {4, 12.5},
															{1.5, 12}, {1.5, 10},
															{1, 9}, {6, 3},
															{6, 2}, {1, 3},
															{0, 3}, {-1, 3},
															{-6, 2}, {-6, 3},
															{-1, 9}, {-1.5, 10},
															{-1.5, 12}, {-4, 12.5},
															{-10, 10}, {-13, 9},
															{-14, 10}, {-15, 10},
															{-17.5, 12.5}, {-19, 9},
															{-19, 5}, {-15, 0},
															{-5.5, -8}, {-5.35, -7.35},
															{-5.5, -6.5}, {-4.5, -7},
															{0, -9}, {0, -9},
															{0, -9}, {0, -9},
															{0, -9}, {0, -9},
															{0, -9}, {0, -9},
															{0, -9}, {0, -9},
															{0, -9}};

floatpoint3 GLBalanceSitting::pelvisBetween20and30[NUM_PELVIS_POINTS] = {	{0, -7.5}, {4.5, -6.2},
																{5.5,-5}, {4.15,-6.5},
																{5.5, -7.7}, {7, -7},
																{15, -1}, {19, 4.5},
																{19, 9}, {17.5, 13},
																{15.5, 11}, {14, 12},
																{13, 11}, {4, 15},
																{0.5, 14.5}, {0.5, 13},
																{5.7, 7}, {7, 3},
																{7.7, 2.5}, {7.4, 2},
																{0, 3.5}, {-7.4, 2},
																{-7.7, 2.5}, {-7, 3},
																{-5.7, 7}, {-0.5, 13},
																{-0.5, 14.5}, {-4, 15},
																{-13, 11}, {-14, 12},
																{-15.5, 11}, {-17.5, 13},
																{-19, 9}, {-19, 4.5},
																{-15, -1}, {-7, -7},
																{-5.5, -7.7}, {-4.15, -6.5},
																{-5.5, -5}, {-4.5, -6.2},
																{0, -7.5}, {0, -7.5},
																{0, -7.5}, {0, -7.5},
																{0, -7.5}, {0, -7.5},
																{0, -7.5}, {0, -7.5},
																{0, -7.5}, {0, -7.5},
																{0, -7.5}};
floatpoint3 GLBalanceSitting::pelvisBetween30and40[NUM_PELVIS_POINTS] = {	{0, -7},
																{5, -5}, {6, -3},
																{6, -5}, {5, -8},
																{14, -3}, {19, 3},
																{19, 8}, {17, 12},
																{16, 11}, {14, 12.5},
																{13, 12}, {10, 14},
																{6, 16}, {4, 16.5},
																{1, 17}, {.5, 16},
																{.5, 15}, {1, 14.5},
																{3, 13}, {6, 10.5},
																{7, 6}, {8, 5},
																{8, 4}, {7.5, 2.5},
																{0, 3.5}, {-7.5, 2.5},
																{-8, 4}, {-8, 5},
																{-7, 6}, {-6, 10.5},
																{-3, 13}, {-1, 14.5},
																{-0.5, 15}, {-0.5, 16},
																{-1, 17}, {-4,16.5},
																{-6, 16}, {-10, 14},
																{-13, 12}, {-14, 12.5},
																{-16, 11}, {-17, 12},
																{-19, 8}, {-19, 3},
																{-14, -3}, {-5, -8},
																{-6, -5}, {-6, -3},
																{-5, -5},
																{0, -7}};

floatpoint3 GLBalanceSitting::pelvisBetween40and50[NUM_PELVIS_POINTS] = {	{0,-5.5},
																{4.5, -3.5}, {6.25, -2},
																{6, -3.6}, {5.25, -4.25},
																{5.5, -6.25}, {7, -6.5},
																{15,-3}, {19, 3.5},
																{19, 7.5}, {17.5,12.75},
																{15.5,12}, {14.5, 13.5},
																{13,13.5}, {10, 16},
																{6, 18}, {1.5, 19.25},
																{0.5, 18.25}, {0.5, 17.5},
																{3, 16.5}, {6,14},
																{8, 8.5}, {8, 6.5},
																{8.5, 6}, {7.5, 4},
																{0,4.5}, {-7.5,4},
																{-8.5, 6}, {-8, 6.5},
																{-8, 8.5}, {-6, 14},
																{-3, 16.5}, {-0.5, 17.5},
																{-0.5, 18.25}, {-1.5, 19.25},
																{-6, 18}, {-10, 16},
																{-13, 13.5}, {-14.5, 13.5},
																{-15.5, 12}, {-17.5, 12.75},
																{-19, 7.5}, {-19, 3.5},
																{-15, -3}, {-7, -6.5},
																{-5.5, -6.25}, {-5.25, -4.25},
																{-6, -3.6}, {-6.25, -2},
																{-4.5, -3.5},
																{0, -5.5}};


floatpoint3 GLBalanceSitting::pelvisGreaterThan50[NUM_PELVIS_POINTS] = {{0,4}, {-7.5,4.5},
															{-8.5, 7}, {-8, 8},															{-8, 11}, {-5.5, 16},
															{-4, 17}, {-0.5, 18.5},
															{-1, 20.5}, {-4, 20},
															{-7, 18}, {-10, 17},
															{-14, 14}, {-16, 12},
															{-18, 12}, {-19, 6},
															{-19, 3}, {-15, -4},
															{-12, -4.5}, {-10, -6},
															{-7,-6}, {-5.5, -5},
															{-5,-3}, {-4, -3},
															{0, -5}, {4, -3},
															{5, -3}, {5.5, -5},
															{7, -6}, {10,-6},
															{12, -4.5}, {15, -4},
															{19, 3}, {19, 6},
															{18,12}, {16,12},
															{14, 14}, {10,17},
															{7, 18}, {4, 20},
															{1, 20.5}, {0.5, 18.5},
															{4, 17}, {5.5, 16},
															{8, 11}, {8, 8},
															{8.5, 7}, {7.5, 4.5},
															{0, 4}, {0, 4},
															{0, 4}};