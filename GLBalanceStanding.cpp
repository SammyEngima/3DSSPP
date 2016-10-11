#include "GLBalanceStanding.h"
#include "Hom_Doc.h"
#include "Hom.h"
#include "../c4eLibNew/math_quat.hpp"
#include "Skeleton.hpp"
#include "Vector3.h"
#include "Factors.hpp"
#include <vector>
#include <set>
#include "../c4eLibNew/point3d.h"
using std::set;
using std::vector;

#define COP_RADIUS 3.0f // This controls the radius of the circles in both the Standing and Seat COP graphic
#define PRINT_COP_RADIUS 15.0f	// This controls the radius of the circles in both for printing

#define METATARSAL_INDEX 11
#define TIP_OF_TOE_INDEX 7
#define NUM_FOOT_POINTS 14

// Basic shape of foot, untranslatez

struct floatpoint2 {	double x, y, z; };
static floatpoint2 footOffset[] = {
								{ 0.43	, 1.00	}, // Heel Point a
								{ 0.63	, 1.00	}, // Heel Point b
								{ 0.83	, 0.97	},
								{ 0.88	, 0.86	}, // changed back to .88 from .83
								//{ 0.80	, 0.70	}, // Commented out
								{ 0.80	, 0.50	}, // changed y from .5 x from .8
								{ 0.85  , 0.40  },
								//{ 0.90	, 0.35	}, // changed x from .91 Commented out
								//{ 0.95	, 0.25	}, // changed x from .99 y from .25 Commented out
								{ 1.00	, 0.15	},	
								{ 0.71	, 0.00	},  // Tip of toe [index 7]
								{ 0.45	, 0.02	}, 
								{ 0.15	, 0.09	},
								{ 0.03	, 0.23	},	
								{ 0.00	, 0.25	},	// Metatarsal [index 11]
								{ 0.14	, 0.85	},
								{ 0.20	, 0.92	},
								
								{ 0.43	, 1.00	}
							};


#define ZOOM_FACTOR 1.1f

// Specify the color of the foot, hexagon, and cirle
// given in rgb from 0 to 255
#define RED_FOOT_COLOR 247.0f
#define GREEN_FOOT_COLOR 247.0f
#define BLUE_FOOT_COLOR 0.0f

#define COG_RED_CIRCLE_COLOR 247.0f
#define COG_GREEN_CIRCLE_COLOR 247.0f
#define COG_BLUE_CIRCLE_COLOR 0.0f

#define COP_RED_CIRCLE_COLOR 247.0f
#define COP_GREEN_CIRCLE_COLOR 0.0f
#define COP_BLUE_CIRCLE_COLOR 0.0f

#define LSF_RED_CIRCLE_COLOR 200.0f // left support force dot color
#define LSF_GREEN_CIRCLE_COLOR 100.0f
#define LSF_BLUE_CIRCLE_COLOR 100.0f

#define RSF_RED_CIRCLE_COLOR 128.0f // right support force dot color
#define RSF_GREEN_CIRCLE_COLOR 0.0f
#define RSF_BLUE_CIRCLE_COLOR 128.0f

#define YELLOW_HEXAGON_COLOR 255.0f
#define RED_FSR_COLOR 0.0f
#define GREEN_HEXAGON_COLOR 128.0f
#define BLUE_HEXAGON_COLOR 0.0f

#define RED_LINE_COLOR 0.0f
#define GREEN_LINE_COLOR 0.0f
#define BLUE_LINE_COLOR 0.0f

//#define COP_RED_LINE_COLOR 0.0f
//#define COP_GREEN_LINE_COLOR 0.0f
//#define COP_BLUE_LINE_COLOR 0.0f

// Opacity given from 0 to 1
#define FOOT_OPACITY 0.6f
#define CIRCLE_OPACITY 1.0f
#define CIRCLE_LINE_OPACITY 0.6f
#define LINE_OPACITY 1.0f
#define HEXAGON_OPACITY 1.0f

GLBalanceStanding::GLBalanceStanding(CWnd* owner) : GLBasic(owner) {
	for(int i = 0; i < 3; i++) {
		clearColor[i] = 1;
	}
}

GLBalanceStanding::~GLBalanceStanding(void) {
}

void GLBalanceStanding::draw() {
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glShadeModel(GL_SMOOTH);	

	// Begin Antialiasing
	glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH); JTK - Removed so that it would not draw extra lines on certain graphics cards
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(viewingRect.left, viewingRect.top, viewingRect.Width(), viewingRect.Height());

	double w = COPwidth = viewingRect.Width();
	double h = COPheight = viewingRect.Height();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-(w/2.0f), (w/2.0f), -(h/2.0f), (h/2.0f), -2, 1);

	glColor4f(1,1,1,1);
	glBegin(GL_POLYGON);
	glVertex2f(-(w/2.0f),-(h/2.0f));
	glVertex2f((w/2.0f),-(h/2.0f));
	glVertex2f((w/2.0f),(h/2.0f));
	glVertex2f(-(w/2.0f),(h/2.0f));
	glEnd();

	gluLookAt(	0.0f, 0.0f, -1.0f,			// Gives the standard x,y plane centered at the origin
			0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 0.0f);

	drawGLCOP();

	glPopAttrib();
	glPopMatrix();
}

void GLBalanceStanding::drawFeet(const float &center_x, const float &center_y, const float &factor, const Factors& lFactors) {
	bool rightFoot = true;
	// FactorX and FactorY are the same for GLBalanceView
	// This change was made to accomodate GLSeatCOP
	bool metric = true;
	// This will always be false because this is the standing graphic
	bool seated = false; 
            
	drawFoot(center_x, center_y, factor, factor, lFactors, rightFoot, metric, seated);
	drawFoot(center_x, center_y, factor, factor, lFactors, !rightFoot, metric, seated);
}
void GLBalanceStanding::drawFoot(const float &center_x, const float &center_y, const float &factorX, const float &factorY, const Factors& lFactors, const bool& rightFoot, const bool& metric, const bool& seated) {
	SystemOfUnitsEnm units;
	if(metric)
		units = MetricCns;
	else
		units = EnglishCns;
	float H;

	C_Hom_Doc* lDoc = Get_App_Ptr()->Get_Document_Ptr();
	Skeleton &lSkeleton = *lDoc->GetSkeleton();
	JointAngles skelAngles = lSkeleton.GetAngles();
   
	H = lDoc->getAnthropometryRef().GetHeight(MetricCns);//Ht.ValueIn(units); // retrieve Height in current units
	if(!seated) {
		H *= 0.01;//U_CM_M;
	}

	FeetSupport_e feetSupport = lDoc->getFeetSupport();
	float iAngle[2] = { 0.0f, 0.0f };		// Angles of the feet
	bool lDrawFoot;
	if(rightFoot) {
		lDrawFoot = (feetSupport != FS_Left);//(iStance != STANCE_LEFT_FOOT);
	} else {
		lDrawFoot = (feetSupport != FS_Right);//(iStance != STANCE_RIGHT_FOOT);
	}
	if(!lDrawFoot) return;
	floatpoint2 foot[NUM_FOOT_POINTS];

	float lHeelLength          = lFactors.mLink_HeelLength * H;
	float lMetatarsalLength    = lFactors.mLink_MetatarsalLength * H;
	float lLateralGreaterToe   = lFactors.mLink_LateralGreaterToe * H;
	float lFifthMetatarsal     = lFactors.mLink_FifthMetatarsalLateral * H;

	//Length lLocX(0,5);
	//Length lLocY(0,5);
	//Length lLocZ(0,5);
	Vector3 ankle;
	if(!seated) {
		if(rightFoot)
			//lSkeleton.C4EPoint(JT_RANKLE, lLocX, lLocY, lLocZ);
			ankle = lSkeleton.getJoint(JT_RANKLE, true) - lSkeleton.getJoint(JT_ANKLE_CENTER, true);
		else
			//lSkeleton.C4EPoint(JT_LANKLE, lLocX, lLocY, lLocZ);
			ankle = lSkeleton.getJoint(JT_LANKLE, true) - lSkeleton.getJoint(JT_ANKLE_CENTER, true);
	} else {
		if(rightFoot) {
			ankle = lSkeleton.getJoint(JT_RANKLE, true);
			ankle[1] -= lSkeleton.getJoint(JT_ANKLE_CENTER, true)[1];
		} else {
			ankle = lSkeleton.getJoint(JT_LANKLE, true);
			ankle[1] -= lSkeleton.getJoint(JT_ANKLE_CENTER, true)[1];
		}
	}
	//float specAnkle[] = { lLocX.ValueIn(MetricCns), lLocY.ValueIn(MetricCns), 0 };
	float specAnkle[] = {ankle[0], ankle[1], 0};
	if(!seated) {
		for(int i = 0; i < 2; ++i) {
			specAnkle[i] *= 0.01;//U_CM_M;
		}
	}
	footOffset[TIP_OF_TOE_INDEX].x = (lLateralGreaterToe + lFifthMetatarsal)
										/ (H * lFactors.mLink_FootBreadth);
	footOffset[METATARSAL_INDEX].y = 1 - (lMetatarsalLength + lHeelLength)
										/ (H * lFactors.mLink_HeelToToe);

	double specFoot[NUM_FOOT_POINTS][3];

	int loop;
	for (loop = 0; loop < NUM_FOOT_POINTS; loop++) {
		if(rightFoot) {
			specFoot[loop][0] = factorX * (lFactors.mLink_FootBreadth * H
											* (0 - footOffset[loop].x) + lFifthMetatarsal);
		} else {
			specFoot[loop][0] = factorX * (lFactors.mLink_FootBreadth * H
											* (footOffset[loop].x) - lFifthMetatarsal);
		}
		if(rightFoot) {
		specFoot[loop][1] = -factorY * (lFactors.mLink_HeelToToe * H *
			(1 - footOffset[loop].y) - lHeelLength);}
		else{
		specFoot[loop][1] = -factorY * (lFactors.mLink_HeelToToe * H *
			(1 - footOffset[loop].y) - lHeelLength);
		}
		specFoot[loop][2] = 0;
	}

	double vZAxis[] = { 0.0,	0.0, -1.0 };

	// Rotate the foot graphic
	pQuaternion qAngle;

	if(rightFoot) {
		pQuatFromAngleAxis(qAngle, vZAxis, (-(90-skelAngles[A_FootRH]) / 180) * M_PI);
	} else {
		pQuatFromAngleAxis(qAngle, vZAxis, ((90-skelAngles[A_FootLH]) / 180) * M_PI);
	}
	for (loop = 0; loop < NUM_FOOT_POINTS; loop++) {// -999 is the end sentinel
		pQuatRotateVector3f(specFoot[loop],qAngle,specFoot[loop]);
		foot[loop].x = specFoot[loop][0];
		foot[loop].y = specFoot[loop][1];

		foot[loop].x += center_x + factorX * (specAnkle[0]);
      
		foot[loop].y += center_y; 
		if(!seated) {
			foot[loop].y -= factorY * (specAnkle[1]);
		}
	}
	glLineWidth(1.0);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	if (lDrawFoot) {
		// Fill
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	
		glColor4f(128/255.0f, 128/255.0f,BLUE_FOOT_COLOR/255.0f, FOOT_OPACITY);
		glBegin(GL_POLYGON);
			for(int i = 0; i < NUM_FOOT_POINTS; i++) {
				glVertex2d(foot[i].x, foot[i].y);
			}
		glEnd();
		// Draw Outline
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor4f(RED_LINE_COLOR/255.0, GREEN_LINE_COLOR/255.0, BLUE_LINE_COLOR/255.0, LINE_OPACITY);
		glBegin(GL_POLYGON);
			for(int i = 0; i < NUM_FOOT_POINTS; i++) {
				glVertex2d(foot[i].x, foot[i].y);
			}
		glEnd();
	}
	glPopAttrib();
}

void GLBalanceStanding::drawGLCOP() {
	const BalanceData_s& balance_data = pDoc->GetSkeleton()->getBalanceData(true);
	vector<Vector3> BOSpoints = pDoc->GetSkeleton()->getBOSpoints();
	vector<Vector3> FSRpoints = pDoc->GetSkeleton()->getFSRpoints();
	float x = balance_data.CenterOfPressure[0]/100, y = balance_data.CenterOfPressure[1]/100;
	//int n = BOSpoints.size();
	const Factors& lFactors = pDoc->GetFactors();

	///////// Setup the drawing area /////////
	vector<Vector3>::iterator ptIterator;
	vector<Vector3> translatedBOS = BOSpoints;
	vector<Vector3> translatedFSR = FSRpoints;
	vector<Vector3>::iterator BOSiterator;
	BOSiterator = BOSpoints.begin(); 
	// Find the bounds of the BOS area
	double minX = (BOSpoints[findLimitPointIndex(BOSpoints, 0, false) ][0]);
	double maxX = (BOSpoints[findLimitPointIndex(BOSpoints, 0, true)  ][0]);

	double minY = (BOSpoints[findLimitPointIndex(BOSpoints, 1, false)][1]);
	double maxY = (BOSpoints[findLimitPointIndex(BOSpoints, 1, true) ][1]);

	// Expand the bounding rect to include the COP, if necessary, so that
	//   the dot is always onscreen
	minX = min(minX, x);
	maxX = max(maxX, x);
	minY = min(minY, y);
	maxY = max(maxY, y);
	minX = min(minX, balance_data.CenterOfGravity[0]/100);
	maxX = max(maxX, balance_data.CenterOfGravity[0]/100);
	minY = min(minY, balance_data.CenterOfGravity[1]/100);
	maxY = max(maxY, balance_data.CenterOfGravity[1]/100);

	// currentRatio = either dx/w or dy/h, where dy and dx are the distances
	//		between the min and max x/y points (so the width and height of the COP
	//		bouding rect)
	double currentRatio = max((maxX - minX) / COPwidth, (maxY - minY) / COPheight);
	double targetRatio = .8;			// We want a 10% buffer (minimum) for
								     // top and bottom, left and right
	double factor = targetRatio/currentRatio;

	// Set true center (based on bounding rect of BOS, COP dot)
	long center_x = - ((maxX + minX) / 2 * factor);
	long center_y = ((maxY + minY) / 2 * factor);
      
	x =  factor * x + center_x;
	y = -factor * y + center_y;

	// Convert from world to display coordinates
	for (ptIterator=translatedBOS.begin(); ptIterator != translatedBOS.end();
		ptIterator++) {
		(*ptIterator)[0] =  ((*ptIterator)[0])* factor + center_x;
		(*ptIterator)[1] = -((*ptIterator)[1])* factor + center_y;
		(*ptIterator)[2] = 0; // Changed from zero to 1
	}
      
	// Convert from world to display coordinates
	for (ptIterator=translatedFSR.begin(); ptIterator != translatedFSR.end();
		ptIterator++) {
		(*ptIterator)[0] =  ((*ptIterator)[0])* factor + center_x;
		(*ptIterator)[1] = -((*ptIterator)[1])* factor + center_y;
		(*ptIterator)[2] = 0; // Changed from zero to 1
	}
	int pointsInFeet = NUM_FOOT_POINTS; 

	// Draw the feet
	drawFeet(center_x, center_y, factor, lFactors); 

	drawHexagon(translatedBOS);
	//glDisable(GL_BLEND);
	//drawFSR(translatedFSR);
	// glEnable(GL_BLEND);
	// each vertex of the BOS
	double xx[7] = {0,0,0,0,0,0,0},yy[7] = {0,0,0,0,0,0,0};
	ptIterator = translatedBOS.begin();
	for(int i = 0;i<translatedBOS.size();i++,ptIterator++)
	{
		xx[i] = (*(ptIterator))[0];
		yy[i] = (*(ptIterator))[1];
		//drawCenterBOS(xx[i],yy[i],factor);
	} 

	xx[translatedBOS.size()] = xx[0];
	yy[translatedBOS.size()] = yy[0];


	// find the center of BOS area
	double A = 0, BOScenterX = 0, BOScenterY = 0;

	for(int i=0;i<translatedBOS.size();i++)
		A += (xx[i] * yy[i+1] - xx[i+1] * yy[i])/2;

	for(int i=0;i<translatedBOS.size();i++)
	{
		BOScenterX += (xx[i] + xx[i+1])*(xx[i] * yy[i+1] - xx[i+1] * yy[i])/6/A;
		BOScenterY += (yy[i] + yy[i+1])*(xx[i] * yy[i+1] - xx[i+1] * yy[i])/6/A;
	}
	
	//double BOScenterX = (xx[0]+xx[1]+xx[2]+xx[3]+xx[4]+xx[5])/6;
	//double BOScenterY = (yy[0]+yy[1]+yy[2]+yy[3]+yy[4]+yy[5])/6;

	 /*
	// find all vectors of BOS borders	
		vector<Vector3> BOSborder;
		vector<Vector3>::iterator Bbegin,Bend;
		Bbegin = translatedBOS.begin();
		Bend   = translatedBOS.end();*/

		const double TOP_FACTOR     = 0.68;
		const double BOT_FACTOR     = 0.55;
		const double LATERAL_FACTOR = 0.66;

	/*	double k[6] = {0,0,0,0,0,0};
		double b[6] = {0,0,0,0,0,0};

		for(int i = 0; i < translatedBOS.size();i++)
		{
			if(i != translatedBOS.size()-1)
			{
				BOSborder.push_back(translatedBOS[i+1]-translatedBOS[i]);
				k[i] = (translatedBOS[i+1][1]- translatedBOS[i][1])/(translatedBOS[i+1][0]- translatedBOS[i][0]);
				b[i] = translatedBOS[i][1] - k[i] * translatedBOS[i][0];
			}

			else 
			{
				BOSborder.push_back(translatedBOS[0]-translatedBOS[i]);
				k[i] = (translatedBOS[0][1]- translatedBOS[i][1])/(translatedBOS[0][0]- translatedBOS[i][0]);
				b[i] = translatedBOS[i][1] - k[i] * translatedBOS[i][0];
			}
		}*/
		double xborder[6] , yborder[6];

		xborder[1] = BOScenterX + TOP_FACTOR * (xx[4] - BOScenterX);
		yborder[1] = BOScenterY + TOP_FACTOR * (yy[4] - BOScenterY);

		xborder[5] = BOScenterX + TOP_FACTOR * (xx[5] - BOScenterX);
		yborder[5] = BOScenterY + TOP_FACTOR * (yy[5] - BOScenterY);

		xborder[2] = BOScenterX + LATERAL_FACTOR * (xx[3] - BOScenterX);
		yborder[2] = BOScenterY + LATERAL_FACTOR * (yy[3] - BOScenterY);

		xborder[3] = BOScenterX + LATERAL_FACTOR * (xx[0] - BOScenterX);
		yborder[3] = BOScenterY + LATERAL_FACTOR * (yy[0] - BOScenterY);

		xborder[4] = BOScenterX + BOT_FACTOR * ((xx[2] + xx[3])/2 - BOScenterX);
		yborder[4] = BOScenterY + BOT_FACTOR * ((yy[2] + yy[3])/2 - BOScenterY);


		double xleft = BOScenterX + BOT_FACTOR * (xx[1] - BOScenterX);
		double yleft = BOScenterY + BOT_FACTOR * (yy[1] - BOScenterY);
		
		double xright = BOScenterX + BOT_FACTOR * (xx[2] - BOScenterX);
		double yright = BOScenterY + BOT_FACTOR * (yy[2] - BOScenterY);
		/*
		//draw the FSR
		int i =1;
		for (ptIterator=translatedFSR.begin(); ptIterator != translatedFSR.end();
		ptIterator++,i++) {
		(*ptIterator)[0] = xborder[i] ;
		(*ptIterator)[1] = yborder[i] ;
		(*ptIterator)[2] = 0; // Changed from zero to 1
		}
		*/
	/*
 		//draw vertex
		for(int i = 1; i < 6; i++)
			drawCenterBOS(xborder[i],yborder[i],factor);

		drawCenterBOS(xleft,yleft,factor);
		drawCenterBOS(xright,yright,factor);*/
		/*
		// find the control point for Quadratic Bézier curve
		double xc = 2*xborder[4] - xborder[3]/2 - xborder[5]/2;
		double yc = 2*yborder[4] - yborder[3]/2 - yborder[5]/2;
		GLfloat controlpoints[3][3] = {
			{xborder[3],yborder[3],0.0},// first point
			{xc,yc,0.0},				// control point
			{xborder[5],yborder[5],0.0} // final point
		};*/

		//draw outline of FSR

		/*
		glColor4f(0/255.0f,0/255.0f,0/255.0f, LINE_OPACITY);
		glBegin(GL_LINE_STRIP);
		glVertex3f(xborder[5],yborder[5],0.0);
		glVertex3f(xborder[1],yborder[1],0.0);
		glVertex3f(xborder[2],yborder[2],0.0);
		glVertex3f(xleft,yleft,0.0);
		glVertex3f(xright,yright,0.0);
		glVertex3f(xborder[3],yborder[3],0.0);
		glVertex3f(xborder[5],yborder[5],0.0);
		glEnd(); 
		*/

		/*
		//  draw the curve
		glMap1f(GL_MAP1_VERTEX_3,0.0,1.0,3,3,&controlpoints[0][0]);
		glEnable(GL_MAP1_VERTEX_3);
		glBegin(GL_LINE_STRIP);
		 for (int i = 0; i <= 30; i++)
		 {
			 glEvalCoord1f((GLfloat)i/30);
		 }
		glEnd();
		*/
		drawFSR(translatedFSR);
		drawCOPCoordinate(x,y, factor);
		if(COG) 
			drawCOGCoordinate(factor*(balance_data.CenterOfGravity[0]/100) + center_x, -factor*(balance_data.CenterOfGravity[1]/100) + center_y, factor);
			

		// draw center point of BOS area 
	//drawCenterBOS(BOScenterX,BOScenterY,factor);

	//C_Hom_Doc* lDocPtr = Get_App_Ptr()->Get_Document_Ptr();
	Vector3 offsetCoordinate = pDoc->getOffset(0, true);
	Skeleton &lSkeleton = *pDoc->GetSkeleton();
	Vector3 x1 = lSkeleton.getJoint(JT_LHEEL, true) - offsetCoordinate;//lheel
	Vector3 x2 = lSkeleton.getJoint(JT_LBALL, true) - offsetCoordinate;//lball
	Vector3 x3 = lSkeleton.getJoint(JT_RHEEL, true) - offsetCoordinate;//rheel
	Vector3 x4 = lSkeleton.getJoint(JT_RBALL, true) - offsetCoordinate;// rball
	Vector3 x99 = lSkeleton.getJoint(JT_LANKLE, true) - offsetCoordinate;
			double xx99 = factor * x99[0]/100 + center_x;
			double yy99 = -factor * x99[1]/100 + center_y; //
	
	double LSupportForce[2];
	double RSupportForce[2];

	auto rheel = lSkeleton.getNetForce(JT_RHEEL);
	auto lheel = lSkeleton.getNetForce(JT_LHEEL);
	auto rball = lSkeleton.getNetForce(JT_RBALL);
	auto lball = lSkeleton.getNetForce(JT_LBALL);

		LSupportForce[0] = (x2 + (x1 - x2) * lSkeleton.getNetForce(JT_LHEEL).length() / lSkeleton.getNetFeetForce(JT_LFOOT_CENTER).length())[0];
		LSupportForce[1] = (x2 + (x1 - x2) * lSkeleton.getNetForce(JT_LHEEL).length() / lSkeleton.getNetFeetForce(JT_LFOOT_CENTER).length())[1];
		RSupportForce[0] = (x4 + (x3 - x4) * lSkeleton.getNetForce(JT_RHEEL).length() / lSkeleton.getNetFeetForce(JT_RFOOT_CENTER).length())[0];
		RSupportForce[1] = (x4 + (x3 - x4) * lSkeleton.getNetForce(JT_RHEEL).length() / lSkeleton.getNetFeetForce(JT_RFOOT_CENTER).length())[1];
	
			double xx3 = factor * x3[0]/100 + center_x;	// right heel
			double yy3 = -factor * x3[1]/100 + center_y; // right heel

			double xx4 = factor * x4[0]/100 + center_x;// right ball
			double yy4 = -factor * x4[1]/100 + center_y; // right ball

			//drawCenterBOS(xx4,yy4,factor);
		/*glColor4f(0/255.0f,0/255.0f,0/255.0f, LINE_OPACITY);
		glLineWidth(2.5f);
		glBegin(GL_LINE_STRIP);
		glVertex3f(xx3,yy3,0.0);
		glVertex3f(xx4,yy4,0.0);
		//glEnd();*/
			 double xx5 = factor * x1[0]/100 + center_x;// left heel
			 double yy5 = -factor * x1[1]/100 + center_y; // left heel
			 double xx6 = factor * x2[0]/100 + center_x;	//  left ball
			 double yy6 = -factor * x2[1]/100 + center_y; //  left ball

			 //drawCenterBOS(translatedBOS[2][0],translatedBOS[2][1],factor);
			 // xx3,y33 right heel
			 // xx5,yy5 left heal
		/*if((-BOSpoints[3][1]*factor + center_y) == translatedBOS[3][1] && (BOSpoints[3][0]*factor + center_x) == translatedBOS[3][0])
		{
			int i = 0;
		}*/
			 	//For two given points that a line passes through, and a third point whose
				//position we want to know relative to that line, the following is used:
				//                        .(x,y)
				//   (x0,y0).--------------------------------------.(x1,y1)
				//
				// f(x,y) = (x - x0)*(y1 - y0) - (y - y0)*(x1 - x0)
				// If f(x,y) > 0, the point (x,y) is below (or to the right of) the line.
				// If f(x,y) < 0, the point (x,y) is above (or to the left of) the line.
				// If f(x,y) = 0, the point (x,y) is on line.

			 // here, (x,y) is right heel,(x0,y0) is left heel, (x1,y1) is new right ball
			 // x3: rheel 
			 // x5:left heel 
			 // BOSpoints[2]: new right ball
			lSkeleton.righcase = false;
			lSkeleton.leftcase = false;
		if(((xx3 - xx5)*(translatedBOS[2][1] - yy5) - (yy3 - yy5)*(translatedBOS[2][0] - xx5)) > 0.01 &&translatedBOS[0] == translatedBOS[5])
		//if(translatedBOS[5] == translatedBOS[0])
		{
			/*drawCenterBOS(xx5,yy5,factor);
			drawCenterBOS(translatedBOS[2][0],translatedBOS[2][1],factor);
			glBegin(GL_LINE_STRIP);
			glVertex3f(xx5,yy5,0.0);
			glVertex3f(translatedBOS[2][0],translatedBOS[2][1],0.0);
			glEnd();*/
			LSupportForce[0] = (x2 + (x1 - x2) * lSkeleton.getNetForce(JT_LHEEL).length() / lSkeleton.getNetFeetForce(JT_LFOOT_CENTER).length())[0];
			LSupportForce[1] = (x2 + (x1 - x2) * lSkeleton.getNetForce(JT_LHEEL).length() / lSkeleton.getNetFeetForce(JT_LFOOT_CENTER).length())[1];
			RSupportForce[0] = (x4 + (BOSpoints[2]*100 - x4) * lSkeleton.getNetForce(JT_RHEEL).length() / lSkeleton.getNetFeetForce(JT_RFOOT_CENTER).length())[0];
			RSupportForce[1] = (x4 + (BOSpoints[2]*100 - x4) * lSkeleton.getNetForce(JT_RHEEL).length() / lSkeleton.getNetFeetForce(JT_RFOOT_CENTER).length())[1];
			lSkeleton.righcase = true;
			lSkeleton.leftcase = false;
			//lSkeleton.leftcenter = false;
		    //lSkeleton.rightcenter = false;
		}
				//For two given points that a line passes through, and a third point whose
				//position we want to know relative to that line, the following is used:
				//                        .(x,y)
				//   (x0,y0).--------------------------------------.(x1,y1)
				//
				// f(x,y) = (x - x0)*(y1 - y0) - (y - y0)*(x1 - x0)
				// If f(x,y) > 0, the point (x,y) is below (or to the right of) the line.
				// If f(x,y) < 0, the point (x,y) is above (or to the left of) the line.
				// If f(x,y) = 0, the point (x,y) is on line.
			 // here, (x,y) is left heel,(x0,y0) is right heel, (x1,y1) is new left ball
		if(((xx5 - translatedBOS[1][0])*(yy3 - translatedBOS[1][1]) - (yy5 - translatedBOS[1][1])*(yy3 - translatedBOS[1][0])) > 0.001 &&translatedBOS[0] == translatedBOS[5])
		{

			/*drawCenterBOS(xx3,yy3 ,factor);
 			drawCenterBOS(translatedBOS[1][0],translatedBOS[1][1],factor);
			glBegin(GL_LINE_STRIP);
			glVertex3f(xx3,yy3,0.0);
			glVertex3f(translatedBOS[1][0],translatedBOS[1][1],0.0);
			glEnd();*/
			LSupportForce[0] = (x2 + (BOSpoints[1]*100 - x2) * lSkeleton.getNetForce(JT_LHEEL).length() / lSkeleton.getNetFeetForce(JT_LFOOT_CENTER).length())[0];
			LSupportForce[1] = (x2 + (BOSpoints[1]*100 - x2) * lSkeleton.getNetForce(JT_LHEEL).length() / lSkeleton.getNetFeetForce(JT_LFOOT_CENTER).length())[1];
			RSupportForce[0] = (x4 + (x3 - x4) * lSkeleton.getNetForce(JT_RHEEL).length() / lSkeleton.getNetFeetForce(JT_RFOOT_CENTER).length())[0];
			RSupportForce[1] = (x4 + (x3 - x4) * lSkeleton.getNetForce(JT_RHEEL).length() / lSkeleton.getNetFeetForce(JT_RFOOT_CENTER).length())[1];
			lSkeleton.righcase = false;
			lSkeleton.leftcase = true;
		}

	if(SF && (balance_data.BalanceStatus == BS_Acceptable || balance_data.BalanceStatus == BS_Critical)){

		// Below are the code to fix balance problem when two feet are split.
		// Two bool variables are defined , leftcenter is true when left reaches its limit.
		// In other words, it is ture when right foot is in front of right foot, so the center force of left feet
		// will go to the left feet heel and stops there. Center of Pressure always lies on the line between
		// center of two feet, if we know one center and center of pressure .so we find the right feet center by calculating
		// the intercections, see the code below

		if(lSkeleton.leftcenter == false && lSkeleton.rightcenter == false)
		{
			drawLSupportForceCoordinate(factor*(LSupportForce[0]/100) + center_x, -factor*(LSupportForce[1]/100) + center_y, factor);
			drawRSupportForceCoordinate(factor*(RSupportForce[0]/100) + center_x, -factor*(RSupportForce[1]/100) + center_y, factor);
		}
		else if (lSkeleton.rightcenter == false) // special case when left foot reaches its limit
		{
			double xx1 = factor*(LSupportForce[0]/100) + center_x;
			double yy1 = -factor*(LSupportForce[1]/100) + center_y; //center of force in left foot

			double xx2 = x;
			double yy2 = y; // COP

			double xx3 = factor * x3[0]/100 + center_x;
			double yy3 = -factor * x3[1]/100 + center_y; // right heel

			double xx4 = factor * x4[0]/100 + center_x;
			double yy4 = -factor * x4[1]/100 + center_y; // right ball

			// next we will find the intersection of these two lines
			double xx = ((xx1*yy2-yy1*xx2)*(xx3-xx4)-(xx1-xx2)*(xx3*yy4-yy3*xx4))
				/((xx1-xx2)*(yy3-yy4)-(yy1-yy2)*(xx3-xx4));
			double yy = ((xx1*yy2-yy1*xx2)*(yy3-yy4)-(yy1-yy2)*(xx3*yy4-yy3*xx4))
				/((xx1-xx2)*(yy3-yy4)-(yy1-yy2)*(xx3-xx4));		

			// begin to draw the force center of each foot
			drawLSupportForceCoordinate(factor*(LSupportForce[0]/100) + center_x, -factor*(LSupportForce[1]/100) + center_y, factor);
			drawRSupportForceCoordinate(xx, yy , factor);
			
		}
		else if (lSkeleton.leftcenter == false)    // special case when right foot reaches its limit
		{
			double xx1 = factor*(RSupportForce[0]/100) + center_x;
			double yy1 = -factor*(RSupportForce[1]/100) + center_y;
			double xx2 = x;
			double yy2 = y;
			double xx3 = factor * x1[0]/100 + center_x;
			double yy3 = -factor * x1[1]/100 + center_y;
			double xx4 = factor * x2[0]/100 + center_x;
			double yy4 = -factor * x2[1]/100 + center_y;
			double xx = ((xx1*yy2-yy1*xx2)*(xx3-xx4)-(xx1-xx2)*(xx3*yy4-yy3*xx4))
				/((xx1-xx2)*(yy3-yy4)-(yy1-yy2)*(xx3-xx4));
			double yy = ((xx1*yy2-yy1*xx2)*(yy3-yy4)-(yy1-yy2)*(xx3*yy4-yy3*xx4))
				/((xx1-xx2)*(yy3-yy4)-(yy1-yy2)*(xx3-xx4));
			drawLSupportForceCoordinate(xx, yy, factor);
			drawRSupportForceCoordinate(factor*(RSupportForce[0]/100) + center_x, -factor*(RSupportForce[1]/100) + center_y, factor);

		}
	}

}
void GLBalanceStanding::setProjectionMatrix(float w, float h) {
	COPwidth = w;
	COPheight = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-(w/2.0f), (w/2.0f), 
			-(h/2.0f), (h/2.0f), -2, 1);
} 
int  GLBalanceStanding::findLimitPointIndex(const std::vector<Vector3> & ptVect, int whichCoord, bool finding_max) {
	int retIndex = 0;
	
	assert(ptVect.size());	// Gotta be something in there

	for(unsigned int i = 0; i < ptVect.size(); i++) {
		if(	ptVect.at(i)[whichCoord] > ptVect.at(retIndex)[whichCoord] && finding_max
			|| ptVect.at(i)[whichCoord] < ptVect.at(retIndex)[whichCoord] && !finding_max)
			retIndex = i;
	}

	return retIndex;
}
void GLBalanceStanding::drawHexagon(std::vector<Vector3> &iOutline) {
	vector<Vector3> lOutline = iOutline;
	lOutline.push_back( *lOutline.begin() );

	vector<Vector3>::iterator ptIterator = lOutline.begin();

	int loop = 0;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor4f(247/255.0f, 247/255.0f, BLUE_HEXAGON_COLOR/255.0f, 0.5);
	glBegin(GL_POLYGON);
	for (; ptIterator != lOutline.end(); ptIterator++, loop++) {
		glVertex3f((*ptIterator)[0], (*ptIterator)[1], 0.0);
	}
	glEnd();
	// Draw the outline
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor4f(RED_LINE_COLOR/255.0f,GREEN_LINE_COLOR/255.0f,BLUE_LINE_COLOR/255.0f, LINE_OPACITY);
	glBegin(GL_LINE_STRIP);
	for (ptIterator = lOutline.begin(); ptIterator != lOutline.end(); ptIterator++, loop++) {
		glVertex3f((*ptIterator)[0], (*ptIterator)[1], 0.0);
	}
	glEnd();
	glPopAttrib();
}

void GLBalanceStanding::drawFSR(std::vector<Vector3> &iOutline) {
	vector<Vector3> lOutline = iOutline;
	lOutline.push_back( *lOutline.begin() );

	vector<Vector3>::iterator ptIterator = lOutline.begin();

	int loop = 0;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor4f(RED_FSR_COLOR/255.0f, GREEN_HEXAGON_COLOR/255.0f, BLUE_HEXAGON_COLOR/255.0f, 0.8);
	glBegin(GL_POLYGON);
	for (; ptIterator != lOutline.end(); ptIterator++, loop++) {
		glVertex3f((*ptIterator)[0], (*ptIterator)[1], 0.0);
	}
	glEnd();
	// Draw the outline
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor4f(RED_FSR_COLOR/255.0f, GREEN_HEXAGON_COLOR/255.0f, BLUE_HEXAGON_COLOR/255.0f, LINE_OPACITY);
	glBegin(GL_LINE_STRIP);
	for (ptIterator = lOutline.begin(); ptIterator != lOutline.end(); ptIterator++, loop++) {
		glVertex3f((*ptIterator)[0], (*ptIterator)[1], 0.0);
	}
	glEnd();
	glPopAttrib();
}
void GLBalanceStanding::drawCOGCoordinate(float x, float y, float factor) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor4f(COG_RED_CIRCLE_COLOR/255.0f, COG_GREEN_CIRCLE_COLOR/255.0f, COG_BLUE_CIRCLE_COLOR/255.0f, CIRCLE_OPACITY);
	glBegin(GL_POLYGON);
		for(float i = 0; i <= 2*M_PI; i += 0.1f) {
			glVertex2f((COP_RADIUS)* cos(i) + x, (COP_RADIUS)* sin(i)  + y);
		}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor4f(RED_LINE_COLOR/255.0f, GREEN_LINE_COLOR/255.0f, BLUE_LINE_COLOR/255.0f, CIRCLE_LINE_OPACITY);
	glBegin(GL_POLYGON);
		for(float i = 0; i <= 2*M_PI; i += 0.1f) {
			glVertex2f((COP_RADIUS)* cos(i) + x, (COP_RADIUS)* sin(i)  + y);
		}
	glEnd();
	glPopAttrib();
}
void GLBalanceStanding::drawCenterBOS(float x, float y, float)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor4f(0/255.0f, 0/255.0f, 0 /255.0f, CIRCLE_OPACITY);
	glBegin(GL_POLYGON);
		for(float i = 0; i <= 2*M_PI; i += 0.1f) {
			glVertex2f((COP_RADIUS)* cos(i) + x, (COP_RADIUS)* sin(i)  + y);
		}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
	glColor4f(0/255.0f, 0/255.0f, 0/255.0f, CIRCLE_LINE_OPACITY);
	glBegin(GL_POLYGON);
		for(float i = 0; i <= 2*M_PI; i += 0.1f) {
			glVertex2f((COP_RADIUS)* cos(i) + x, (COP_RADIUS)* sin(i)  + y);
		}
	glEnd();
	glPopAttrib();
}
void GLBalanceStanding::drawCOPCoordinate(float x, float y, float  ) {
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glColor4f(0,0,0,1);
	//glLineWidth(3);
	//glBegin(GL_LINES);
	//	glVertex2f(x - 4, y);
	//	glVertex2f(x + 4, y);
	//	glVertex2f(x, y - 4);
	//	glVertex2f(x, y + 4);
	//glEnd();
	//glColor4f(0,1,0,1);
	//glLineWidth(1);
	//glBegin(GL_LINES);
	//	glVertex2f(x - 3, y);
	//	glVertex2f(x + 3, y);
	//	glVertex2f(x, y - 3);
	//	glVertex2f(x, y + 3);
	//glEnd();
	//glPopAttrib();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor4f(COP_RED_CIRCLE_COLOR/255.0f, COP_GREEN_CIRCLE_COLOR/255.0f, COP_BLUE_CIRCLE_COLOR/255.0f, CIRCLE_OPACITY);
	glBegin(GL_POLYGON);
		for(float i = 0; i <= 2*M_PI; i += 0.1f) {
			glVertex2f((COP_RADIUS)* cos(i) + x, (COP_RADIUS)* sin(i)  + y);
		}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor4f(RED_LINE_COLOR/255.0f, GREEN_LINE_COLOR/255.0f, BLUE_LINE_COLOR/255.0f, CIRCLE_LINE_OPACITY);
	glBegin(GL_POLYGON);
		for(float i = 0; i <= 2*M_PI; i += 0.1f) {
			glVertex2f((COP_RADIUS)* cos(i) + x, (COP_RADIUS)* sin(i)  + y);
		}
	glEnd();
	glPopAttrib();
}

void GLBalanceStanding::drawLSupportForceCoordinate(float x, float y, float factor) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor4f(LSF_RED_CIRCLE_COLOR/255.0f, LSF_GREEN_CIRCLE_COLOR/255.0f, LSF_BLUE_CIRCLE_COLOR/255.0f, CIRCLE_OPACITY);
	glBegin(GL_POLYGON);
		for(float i = 0; i <= 2*M_PI; i += 0.1f) {
			glVertex2f((COP_RADIUS)* cos(i) + x, (COP_RADIUS)* sin(i)  + y);
		}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor4f(RED_LINE_COLOR/255.0f, GREEN_LINE_COLOR/255.0f, BLUE_LINE_COLOR/255.0f, CIRCLE_LINE_OPACITY);
	glBegin(GL_POLYGON);
		for(float i = 0; i <= 2*M_PI; i += 0.1f) {
			glVertex2f((COP_RADIUS)* cos(i) + x, (COP_RADIUS)* sin(i)  + y);
		}
	glEnd();
	glPopAttrib();
}

void GLBalanceStanding::drawRSupportForceCoordinate(float x, float y, float factor) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor4f(RSF_RED_CIRCLE_COLOR/255.0f, RSF_GREEN_CIRCLE_COLOR/255.0f, RSF_BLUE_CIRCLE_COLOR/255.0f, CIRCLE_OPACITY);
	glBegin(GL_POLYGON);
		for(float i = 0; i <= 2*M_PI; i += 0.1f) {
			glVertex2f((COP_RADIUS)* cos(i) + x, (COP_RADIUS)* sin(i)  + y);
		}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor4f(RED_LINE_COLOR/255.0f, GREEN_LINE_COLOR/255.0f, BLUE_LINE_COLOR/255.0f, CIRCLE_LINE_OPACITY);
	glBegin(GL_POLYGON);
		for(float i = 0; i <= 2*M_PI; i += 0.1f) {
			glVertex2f((COP_RADIUS)* cos(i) + x, (COP_RADIUS)* sin(i)  + y);
		}
	glEnd();
	glPopAttrib();
}