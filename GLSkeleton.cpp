#include "GLSkeleton.h"
#include "Hom.h"
#include "Hom_Doc.h"
#include "MAIN_FRM.H"
#include "UndoPosture.h"
#include "TestTimer.h"
#include "ManipulatePosture.h" // for options
#include "MathUtility.h"
#include <sstream>

using std::set;
using std::vector;
using std::string;
using std::ostringstream;

const float HEIGHT_FACTOR = 69.3f;
const double DTOR = 3.1415926535897932384 / 180.0;

GLSkeleton::GLSkeleton(CWnd* owner) : GLInteractive(owner) {
	manipJoint = -1;
	manipulating = false;
	zoomPan=false;
	ankleManip=false;
	lockMode = LM_None;

	camera=StickCamera(0.0,0.0,1.0);
	//currentView=false;

	toolTip.Create(CPoint(0,0),NULL,0);
	tipShowing = false;
	CueEnabled = false;
	CueFunctions.resize(NUM_ANGLE_TYPES);
	for(int i = 0; i < NUM_JOINTS; i++) BackToFront[i] = (JointID)i;
	for(int i = 0; i < 3; i++) clearColor[i] = ((float)backgroundColor[i])/255;

	// set up left/right/center joint partition
	centerJoints.insert(JT_TRAGION);
	centerJoints.insert(JT_NASION);
		centerJoints.insert(JT_NASIONLOC);
	centerJoints.insert(JT_TRAGIONLOC);
	//Add C1
	centerJoints.insert(JT_C1);
	// Add:
	centerJoints.insert(JT_C3C4);
	centerJoints.insert(JT_C7T1);
	centerJoints.insert(JT_SCJ);
	centerJoints.insert(JT_L5S1_TEMP);
	leftJoints.insert(JT_LSHOULDER);
	leftJoints.insert(JT_LELBOW);
	leftJoints.insert(JT_LWRIST);
	leftJoints.insert(JT_LHAND);
	rightJoints.insert(JT_RSHOULDER);
	rightJoints.insert(JT_RELBOW);
	rightJoints.insert(JT_RWRIST);
	rightJoints.insert(JT_RHAND);
	leftJoints.insert(JT_LHIP);
	leftJoints.insert(JT_LIT);
	leftJoints.insert(JT_LKNEE);
	leftJoints.insert(JT_LANKLE);
	leftJoints.insert(JT_LHEEL);
	leftJoints.insert(JT_LBALL);
	rightJoints.insert(JT_RHIP);
	rightJoints.insert(JT_RIT);
	rightJoints.insert(JT_RKNEE);
	rightJoints.insert(JT_RANKLE);
	rightJoints.insert(JT_RHEEL);
	rightJoints.insert(JT_RBALL);
	centerJoints.insert(JT_HIP_CENTER);
	centerJoints.insert(JT_IT_CENTER);
	centerJoints.insert(JT_ANKLE_CENTER);
	leftJoints.insert(JT_LFOOT_CENTER);
	rightJoints.insert(JT_RFOOT_CENTER);
	centerJoints.insert(JT_BALL_CENTER);

	// set up lines for drawing
	vector<JointID> line;
	line.push_back(JT_RHAND); line.push_back(JT_RWRIST);
	line.push_back(JT_RELBOW); line.push_back(JT_RSHOULDER); line.push_back(JT_SCJ);
	rightLines.insert(line);
	line.clear();
	line.push_back(JT_L5S1_TEMP); line.push_back(JT_RHIP); line.push_back(JT_RKNEE);
	line.push_back(JT_RANKLE); line.push_back(JT_RHEEL); line.push_back(JT_RBALL); line.push_back(JT_RANKLE);
	rightLines.insert(line);
	line.clear();
	line.push_back(JT_LHAND); line.push_back(JT_LWRIST);
	line.push_back(JT_LELBOW); line.push_back(JT_LSHOULDER); line.push_back(JT_SCJ);
	leftLines.insert(line);
	line.clear();
	line.push_back(JT_L5S1_TEMP); line.push_back(JT_LHIP); line.push_back(JT_LKNEE);
	line.push_back(JT_LANKLE); line.push_back(JT_LHEEL); line.push_back(JT_LBALL); line.push_back(JT_LANKLE);
	leftLines.insert(line);
	line.clear();

	line.push_back(JT_NASION); line.push_back(JT_TRAGION); line.push_back(JT_C1); line.push_back(JT_C3C4); line.push_back(JT_C7T1); line.push_back(JT_L5S1_TEMP);
	line.push_back(JT_SCJ); line.push_back(JT_C7T1);
	centerLines.insert(line);
	line.clear();
	line.push_back(JT_RHIP); line.push_back(JT_LHIP); 
	centerLines.insert(line);
	line.clear();

	line.push_back(JT_RHAND); line.push_back(JT_RWRIST);
	line.push_back(JT_RELBOW); line.push_back(JT_RSHOULDER); line.push_back(JT_C7T1);
	rightLines2D3D.insert(line);
	line.clear();
	line.push_back(JT_L5S1_TEMP); line.push_back(JT_RHIP); line.push_back(JT_RKNEE);
	line.push_back(JT_RANKLE); line.push_back(JT_RHEEL); line.push_back(JT_RBALL); line.push_back(JT_RANKLE);
	rightLines2D3D.insert(line);
	line.clear();
	line.push_back(JT_LHAND); line.push_back(JT_LWRIST);
	line.push_back(JT_LELBOW); line.push_back(JT_LSHOULDER); line.push_back(JT_C7T1);
	rightLines2D3D.insert(line);
	line.clear();
	line.push_back(JT_L5S1_TEMP); line.push_back(JT_LHIP); line.push_back(JT_LKNEE);
	line.push_back(JT_LANKLE); line.push_back(JT_LHEEL); line.push_back(JT_LBALL); line.push_back(JT_LANKLE);
	rightLines2D3D.insert(line);
	line.clear();


	line.push_back(JT_NASION); line.push_back(JT_TRAGION); line.push_back(JT_C1); line.push_back(JT_C3C4); line.push_back(JT_C7T1); line.push_back(JT_L5S1_TEMP);
	centerLines2D3D.insert(line);
	line.clear();
	line.push_back(JT_RHIP); line.push_back(JT_LHIP);
	centerLines2D3D.insert(line);

	// set up the map from joints to PA
	//jointsToPA[JT_TRAGION];
	//jointsToPA[JT_NASION];
	//jointsToPA[JT_TRAGIONLOC];
	/*jointsToPA[JT_C7T1].insert(EA_C4C7Flexion);
	jointsToPA[JT_C7T1].insert(EA_C4C7AxialRotation);
	jointsToPA[JT_C7T1].insert(EA_C4C7LateralBending);*/
	//jointsToPA[JT_SCJ];

	jointsToPA[JT_C7T1].insert(EA_C4C7Flexion);
	jointsToPA[JT_C7T1].insert(EA_C4C7AxialRotation);
	jointsToPA[JT_C7T1].insert(EA_C4C7LateralBending);
	jointsToPA[JT_C3C4].insert(EA_C1C3Flexion);
	jointsToPA[JT_C3C4].insert(EA_C1C3AxialRotation);
	jointsToPA[JT_C3C4].insert(EA_C1C3LateralBending);


	//Add C1
	//jointsToPA[JT_C1].insert(EA_NeckUpperFlexion);
	//jointsToPA[JT_C1].insert(EA_NeckUpperAxialRotation);
	//jointsToPA[JT_C1].insert(EA_NeckUpperLateralBending);
	jointsToPA[JT_C1].insert(EA_C1_Flexion);
	jointsToPA[JT_L5S1_TEMP].insert(EA_AdjustedTrunkAxialRotation);
	jointsToPA[JT_L5S1_TEMP].insert(EA_AdjustedTrunkLateralBending);
	jointsToPA[JT_L5S1_TEMP].insert(EA_AdjustedTrunkFlexion);
	jointsToPA[JT_L5S1_TEMP].insert(EA_PelvisFlexion);
	jointsToPA[JT_L5S1_TEMP].insert(EA_PelvisLateralBending);
	jointsToPA[JT_LSHOULDER].insert(EA_ShoulderLH);
	jointsToPA[JT_LSHOULDER].insert(EA_ShoulderLV);
	jointsToPA[JT_LSHOULDER].insert(EA_HumerusLRot);
	jointsToPA[JT_LELBOW].insert(EA_ElbowLInc);
	jointsToPA[JT_LELBOW].insert(EA_ForearmLRot);
	jointsToPA[JT_LWRIST].insert(EA_HandLFlexion);
	jointsToPA[JT_LWRIST].insert(EA_HandLDeviation);
	//jointsToPA[JT_LHAND];
	jointsToPA[JT_RSHOULDER].insert(EA_ShoulderRH);
	jointsToPA[JT_RSHOULDER].insert(EA_ShoulderRV);
	jointsToPA[JT_RSHOULDER].insert(EA_HumerusRRot);
	jointsToPA[JT_RELBOW].insert(EA_ElbowRInc);
	jointsToPA[JT_RELBOW].insert(EA_ForearmRRot);
	jointsToPA[JT_RWRIST].insert(EA_HandRFlexion);
	jointsToPA[JT_RWRIST].insert(EA_HandRDeviation);
	//jointsToPA[JT_RHAND];
	jointsToPA[JT_LHIP].insert(EA_HipLV);
	jointsToPA[JT_LHIP].insert(EA_HipLH);
	jointsToPA[JT_LHIP].insert(EA_FemurLRot);
	jointsToPA[JT_LHIP].insert(EA_HipLInc);
	//jointsToPA[JT_LIT];
	jointsToPA[JT_LKNEE].insert(EA_KneeLInc);
	jointsToPA[JT_LKNEE].insert(EA_LowerlegLRot);
	jointsToPA[JT_LANKLE].insert(EA_AnkleLInc);
	//jointsToPA[JT_LHEEL];
	//jointsToPA[JT_LBALL];
	jointsToPA[JT_RHIP].insert(EA_HipRV);
	jointsToPA[JT_RHIP].insert(EA_HipRH);
	jointsToPA[JT_RHIP].insert(EA_FemurRRot);
	jointsToPA[JT_RHIP].insert(EA_HipRInc);
	//jointsToPA[JT_RIT];
	jointsToPA[JT_RKNEE].insert(EA_KneeRInc);
	jointsToPA[JT_RKNEE].insert(EA_LowerlegRRot);
	jointsToPA[JT_RANKLE].insert(EA_AnkleRInc);
	//jointsToPA[JT_RHEEL];
	//jointsToPA[JT_RBALL];
	//jointsToPA[JT_HIP_CENTER];
	//jointsToPA[JT_IT_CENTER];
	//jointsToPA[JT_ANKLE_CENTER];
	//jointsToPA[JT_LFOOT_CENTER];
	//jointsToPA[JT_RFOOT_CENTER];
	//jointsToPA[JT_BALL_CENTER];

	lastAnkleCenter = workingSkeleton()->getJoint(JT_ANKLE_CENTER, true);
}

GLSkeleton::~GLSkeleton(void) {

}

bool GLSkeleton::init() {

	gluQuadricDrawStyle(quadric,GLU_FILL);

	Selectable = glGenLists(2);
	glNewList(Selectable, GL_COMPILE);
		// draw inner part filled
		//gluSphere(quadric, POINT_RADIUS * 0.8, 10, 10);
		gluDisk(quadric, 0, POINT_RADIUS * 0.8, 10,1);
		// draw edges antialiased
		glEnable(GL_POLYGON_SMOOTH);
		gluDisk(quadric, POINT_RADIUS * 0.7, POINT_RADIUS, 10,1);
		//gluSphere(quadric, POINT_RADIUS, 10, 10);
		glDisable(GL_POLYGON_SMOOTH);
	glEndList();
	
	Unselectable = Selectable + 1;
	glNewList(Unselectable, GL_COMPILE);
		// draw inner part filled
		//gluSphere(quadric, (POINT_RADIUS - 1) * 0.8, 10, 10);
		gluDisk(quadric, 0, (POINT_RADIUS - 1)*0.8,10,1);
		// draw edges antialiased
		glEnable(GL_POLYGON_SMOOTH);
		gluDisk(quadric, (POINT_RADIUS - 1)* 0.7, POINT_RADIUS - 1,10,1);
		//gluSphere(quadric, POINT_RADIUS - 1, 10, 10);
		glDisable(GL_POLYGON_SMOOTH);
	glEndList();
	return true;
}

void GLSkeleton::draw() {

	// TODO debugging motion prediction
	/*if(workingSkeleton()->GetDrawMotion()) {
		workingSkeleton()->SwapMotionJoints();
	}*/
	
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glDisable(GL_LIGHTING);

	// define the viewport (rendering area) to be the entire client rectangle
	glViewport(viewingRect.left, viewingRect.top, viewingRect.Width(), viewingRect.Height());

	// clear the background
	// clear by drawing a polygon, because sometimes we are only rendering to part of the context,
	// but glClear clears all the pixels
	glClearColor(backgroundColor[0]/255.0f, backgroundColor[1]/255.0f, backgroundColor[2]/255.0f, 1.0f);
	glColor4f(backgroundColor[0]/255.0f, backgroundColor[1]/255.0f, backgroundColor[2]/255.0f, 1.0f);
	glOrtho(0,1,0,1,-1,1);
	glBegin(GL_POLYGON);
	glVertex2f(0,0);
	glVertex2f(1,0);
	glVertex2f(1,1);
	glVertex2f(0,1);
	glEnd();

	glClearDepth(100.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	float zoom = calculateZoom();
	zoom *= 1.174f;
	if(zoom < 95.0f) zoom = 95.0f;
	float aspectRatio = (viewingRect.Height() > 0) ? (float)viewingRect.Width()/viewingRect.Height() : 1.0f;
	if(aspectRatio > 1) {
		rightBound = zoom * aspectRatio;
		topBound = zoom;
	} else {
		rightBound = zoom;
		topBound = zoom / aspectRatio;
	}

	// need to calculate near and far planes; this works for now
	glOrtho(-rightBound,rightBound,-topBound,topBound,-15*LOOK_AT_DISTANCE,15*LOOK_AT_DISTANCE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Skeleton* skel = workingSkeleton();
	setCamera(skel->getJoint(JT_ANKLE_CENTER, true));
			// draw the grid first so it will always stay under the figure
	if(showGrid) drawGrid(); 
	// scale width of lines so that larger windows have heavier lines
	if(viewingRect.Width() < viewingRect.Height()) glLineWidth((float)viewingRect.Width() / 130.0f);
	else glLineWidth((float)viewingRect.Height() / 130.0f);
	
	// Draw the floor
	drawFloor();
	// Draw the stick figure
	drawSkeleton();
	// Draw the force indicator
	if(showForces)
		drawForces();
	if(showSupportForces)
		drawFeetForces();

	if(drawTarget) {
		glPushMatrix();

		//Vector3 targetWRTHip = targetLocation + pDoc->getOffset(skel, true);//skel->getJoint(JT_ANKLE_CENTER, true);
		Vector3 targetWRTHip;
		if(pDoc->getPosition() == P_Standing) {
			targetWRTHip = targetLocation + skel->getJoint(JT_LBALL, true);//
		} else {
			targetWRTHip = targetLocation;
			}

		// This draws the target (red dot) for the "Reach to grasp" experiment
		glTranslated(targetWRTHip[0], targetWRTHip[1], targetWRTHip[2]);
		glColor3ubv(forceColor);
		gluSphere(quadric, 4, 10, 10);
		glColor3ubv(backgroundColor);
		gluSphere(quadric, 2.5, 10, 10);
		glColor3ubv(forceColor);
		gluSphere(quadric, 1, 10, 10);
		glColor3ubv(rightfeetforceColor);
		gluSphere(quadric, 4, 10, 10);
		glColor3ubv(rightfeetforceColor);
		gluSphere(quadric, 1, 10, 10);
		glColor3ubv(leftfeetforceColor);
		gluSphere(quadric, 4, 10, 10);
		glColor3ubv(leftfeetforceColor);
		gluSphere(quadric, 1, 10, 10);
		glPopMatrix();
	}

	// draw angle cue
	updateCue();
	if(CueEnabled) {
		drawCue();
	}
					//drawtext(pDoc->getCurrentFrame());
			

	glPopAttrib();
	glPopMatrix();
	// TODO debugging motion prediction
	/*if(workingSkeleton()->GetDrawMotion()) {
		workingSkeleton()->SwapMotionJoints();
	}*/
}
void GLSkeleton::drawSkeleton(void) {
	// if you want to see (0, 0)
	//glColor3ubv(forceColor);
	//gluSphere(quadric, 4, 10, 10);
	glColor3ubv(skeletonColor);

	//draw head - this goes first, so that the other body parts will show through the hollow head
	drawHead();

	// draw bones
	Skeleton &skel = *workingSkeleton();
	Vector3 currentPoint;

	// draw left links
	glColor3ubv(leftLinkColor);
	drawLinkSet(leftLines);

	// draw right links
	glColor3ubv(rightLinkColor);
	drawLinkSet(rightLines);

	// draw center links
	glColor3ubv(skeletonColor);
	drawLinkSet(centerLines);

	drawFeet();
	drawJoints();
}

void GLSkeleton::drawLinkSet(const std::set< std::vector<JointID> >& linkSet) {
	Vector3 currentPoint;
	Skeleton* skel = workingSkeleton();
	for(set< vector<JointID> >::const_iterator it = linkSet.begin(); it != linkSet.end(); ++it) {
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < (*it).size(); i++) {
			currentPoint = skel->getJoint((*it)[i], true);
			glVertex3d(currentPoint[0], currentPoint[1], currentPoint[2]);
		}
		glEnd();
	}
}

void GLSkeleton::drawForces(void) {
	glPushAttrib(GL_LINE_BIT);
	glLineWidth(1.0f);

	Skeleton &skel = *workingSkeleton();

	glColor3ubv(forceColor);

	glEnable(GL_POLYGON_SMOOTH);
	for(int i = 0; i < NUM_JOINTS; ++i) {
		if(skel.getExtForce((JointID)i) != Vector3(0,0,0) && (JointID)i != JT_LHEEL && (JointID)i != JT_RHEEL && (JointID)i != JT_LBALL && (JointID)i != JT_RBALL) {
			Vector3 forceUnit = skel.getExtForce((JointID)i);
			forceUnit /= forceUnit.length();

			Vector3 tip = skel.getJoint((JointID)i, true);
			Vector3 handle = tip - ARROW_LENGTH*forceUnit;

			// draw arrow tip -> base radius 25, top radius 0, starts 75% of the way down the force arrow
			drawForceTip(tip,handle);

			double radius = POINT_RADIUS * pDoc->getAnthropometryRef().GetHeight(EnglishCns) / HEIGHT_FACTOR;
			// draw handle
			glPushMatrix();
			glTranslated(handle[0],handle[1],handle[2]);
			gluSphere(quadric,radius, 8, 8);
			glPopMatrix();

			// draw line
			glBegin(GL_LINES);
				glVertex3d(handle[0],handle[1],handle[2]);
				glVertex3d(tip[0],tip[1],tip[2]);
			glEnd();
		}
	}

	glPopAttrib();
}

void GLSkeleton::drawFeetForces(void) {
	glPushAttrib(GL_LINE_BIT);
	glLineWidth(1.0f);

	Skeleton &skel = *workingSkeleton();

	//glColor3ubv(rightfeetforceColor);

	glEnable(GL_POLYGON_SMOOTH);
	for(int i = 0; i < NUM_JOINTS; ++i) {
		if (i == 31 || i == 32){
			if (i == 31)
				glColor3ubv(leftfeetforceColor);
			else
				glColor3ubv(rightfeetforceColor);
			if(skel.getNetFeetForce(i) != Vector3(0,0,0)) {
				Vector3 feetforceUnit = skel.getNetFeetForce(i);
				feetforceUnit /= feetforceUnit.length();

				Vector3 LfootForceCenter;
				Vector3 RfootForceCenter;
				
				Vector3 x1 = skel.getJoint(JT_LHEEL, true);
				Vector3 x2 = skel.getJoint(JT_LBALL, true);
				Vector3 x3 = skel.getJoint(JT_RHEEL, true);
				Vector3 x4 = skel.getJoint(JT_RBALL, true);
				
				LfootForceCenter[2] = x1[2];
				RfootForceCenter[2] = x3[2];
				
				LfootForceCenter[0] = (x2 + (x1 - x2) * skel.getNetForce(JT_LHEEL).length() / skel.getNetFeetForce(JT_LFOOT_CENTER).length())[0];
				LfootForceCenter[1] = (x2 + (x1 - x2) * skel.getNetForce(JT_LHEEL).length() / skel.getNetFeetForce(JT_LFOOT_CENTER).length())[1];
				RfootForceCenter[0] = (x4 + (x3 - x4) * skel.getNetForce(JT_RHEEL).length() / skel.getNetFeetForce(JT_RFOOT_CENTER).length())[0];
				RfootForceCenter[1] = (x4 + (x3 - x4) * skel.getNetForce(JT_RHEEL).length() / skel.getNetFeetForce(JT_RFOOT_CENTER).length())[1];

				BalanceData_s& balance = skel.getBalanceData();
				if(balance.BalanceStatus == BS_Acceptable || balance.BalanceStatus == BS_Critical){

					Vector3 tip;
					if (i == 31)
						tip = LfootForceCenter;
					else
						tip = RfootForceCenter;
					Vector3 handle = tip + ARROW_LENGTH*feetforceUnit;

					// draw arrow tip -> base radius 25, top radius 0, starts 75% of the way down the force arrow
					drawForceTip(tip,handle);

					double radius = POINT_RADIUS * pDoc->getAnthropometryRef().GetHeight(EnglishCns) / HEIGHT_FACTOR;
					// draw handle
					glPushMatrix();
					glTranslated(handle[0],handle[1],handle[2]);
					gluSphere(quadric,radius, 8, 8);
					glPopMatrix();

					// draw line
					glBegin(GL_LINES);
						glVertex3d(handle[0],handle[1],handle[2]);
						glVertex3d(tip[0],tip[1],tip[2]);
					glEnd();
				}
			}
		}
	}

	glPopAttrib();
}

void GLSkeleton::drawFeet(void) {
	/*
	C_Hom_Doc* docPtr = pDoc;
	Skeleton &skel = *workingSkeleton();
	Vector3 RHeel = skel.getJoint(JT_RHEEL, true);
	Vector3 RBall = skel.getJoint(JT_RBALL, true);
	Vector3 RAnkle = skel.getJoint(JT_RANKLE, true);
	Vector3 LHeel = skel.getJoint(JT_LHEEL, true);
	Vector3 LBall = skel.getJoint(JT_LBALL, true);
	Vector3 LAnkle = skel.getJoint(JT_LANKLE, true);

	glBegin(GL_LINES);
		glVertex3d(RHeel[0], RHeel[1], RHeel[2]);
		glVertex3d(RBall[0], RBall[1], RBall[2]);

		glVertex3d(RBall[0], RBall[1], RBall[2]);
		glVertex3d(RAnkle[0], RAnkle[1], RAnkle[2]);

		glVertex3d(LHeel[0], LHeel[1], LHeel[2]);
		glVertex3d(LBall[0], LBall[1], LBall[2]);

		glVertex3d(LBall[0], LBall[1], LBall[2]);
		glVertex3d(LAnkle[0], LAnkle[1], LAnkle[2]);
	glEnd();
	*/
}
void GLSkeleton::drawHead(void) {
	Skeleton &skel = *workingSkeleton();

	glPushMatrix();
	glPushAttrib(GL_LINE_BIT);
	glLineWidth(1.0);
	gluQuadricDrawStyle(quadric, GLU_SILHOUETTE);

	Vector3 Tragion = skel.getJoint(JT_TRAGION, true);
	glTranslated(Tragion[0], Tragion[1], Tragion[2]);
	if(viewNumber == VT_FRONT) glRotatef(90,-1,0,0);
	else if(viewNumber == VT_SIDE) glRotatef(90,0,1,0);

	glColor3ubv(skeletonColor);
	glEnable(GL_POLYGON_SMOOTH);
	if(pDoc->GetFactors().mLHeadRadius >=0)
	{
			gluDisk(quadric,169.23/*110.0f*/ * CM_TO_IN
					/ HEIGHT_FACTOR * pDoc->GetFactors().mLHeadRadius,
					184.62/*120.0f*/ * CM_TO_IN
					/ HEIGHT_FACTOR * pDoc->GetFactors().mLHeadRadius,
					32, 32);
	}
	else{
	gluDisk(quadric,169.23/*110.0f*/ * pDoc->getAnthropometryRef().GetHeight(EnglishCns)
					/ HEIGHT_FACTOR * pDoc->GetFactors().mLink_HeadRadius,
					184.62/*120.0f*/ * pDoc->getAnthropometryRef().GetHeight(EnglishCns)
					/ HEIGHT_FACTOR * pDoc->GetFactors().mLink_HeadRadius,
					32, 32);
	}
	gluQuadricDrawStyle(quadric, GLU_FILL);

	glPopAttrib();
	glPopMatrix();
}
void GLSkeleton::drawJoints(void) {
	Skeleton &skel = *workingSkeleton();
	Vector3 currentPoint;
	gluQuadricDrawStyle(quadric, GLU_FILL);
	glDisable(GL_POLYGON_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	OrderJoints();
	for(int i = 0; i < NUM_JOINTS; ++i) {

		JointID currentID = BackToFront[i];

		if(jointVisible[currentID]) {
			currentPoint = skel.getJoint(currentID, true);

			glPushMatrix();
			glTranslated(currentPoint[0], currentPoint[1], currentPoint[2]);

			if(viewNumber == VT_FRONT) glRotatef(90,-1,0,0);
			else if(viewNumber == VT_SIDE) glRotatef(90,0,1,0);

			// TODO this is heavily dependent on the order of the JointID definitions
			// we should look the id's up from a set or something
			/*if((JT_LSHOULDER <= currentID && currentID <= JT_LHAND) || (JT_LHIP <= currentID && currentID <= JT_LBALL)) {
				glColor4ubv(leftJointColor);
			} else if((JT_RSHOULDER <= currentID && currentID <= JT_RHAND) || (JT_RHIP <= currentID && currentID <= JT_RBALL)) {
				glColor4ubv(rightJointColor);
			} else {
				glColor4ubv(skeletonColor);
			}*/

			/*if(errorOnJoint(currentID)) {
				glColor4ubv(errorJointColor);
			} else {
				glColor4ubv(jointColor);
			}*/

			if(errorOnJoint(currentID)) {
				glColor4ubv(jointColor);
				glCallList(Selectable);
				glColor4ubv(errorJointColor);
				glCallList(Unselectable);
			//} else if(jointSelectable[currentID]) {
			} else if(jointCurrentlySelectable[currentID]) {
				glColor4ubv(jointColor);
				glCallList(Selectable);
			} else {
				glColor4ubv(jointColor);
				glCallList(Selectable);
				glColor4ubv(backgroundColor);
				glCallList(Unselectable);
			}

			glPopMatrix();
		}
	}

	glColor3ubv(skeletonColor);
	jointVisible[JT_SCJ] = true;
}

void GLSkeleton::createContextMenu(CPoint point) {
	CMenu menu;
	menu.LoadMenu(IDR_Context_3View);
	if(!pDoc->CanUndo())
		menu.EnableMenuItem(ID_UndoDirectManip, MF_GRAYED);
	if(!pDoc->CanRedo())
		menu.EnableMenuItem(ID_RedoDirectManip, MF_GRAYED);

	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, owner);
}
void GLSkeleton::handleMouseMove(UINT nFlags, CPoint point) {
	//TRACE("skeleton mouse move\n");

	// hack to watch for shift up/down when no skeleton view has focus
	if(!shiftDown && (nFlags & MK_SHIFT)) {
		shiftDown = true;
		jointCurrentlySelectable = jointRotatable;
		if(tipShowing) {
			toolTip.Cancel();
			tipShowing = false;
		}
		pDoc->UpdateViews();
	}
	if(shiftDown && !(nFlags & MK_SHIFT)) {
		shiftDown = false;
		jointCurrentlySelectable = jointSelectable;
		if(tipShowing) {
			toolTip.Cancel();
			tipShowing = false;
		}
		pDoc->UpdateViews();
	}

	if(!ctrlDown && (nFlags & MK_CONTROL)) {
		ctrlDown = true;
		if(tipShowing) {
			toolTip.Cancel();
			tipShowing = false;
		}
		pDoc->UpdateViews();
	}
	if(ctrlDown && !(nFlags & MK_CONTROL)) {
		ctrlDown = false;
		if(tipShowing) {
			toolTip.Cancel();
			tipShowing = false;
		}
		pDoc->UpdateViews();
	}

	m_nFlags = nFlags;
	m_point = point;
	
	/*CPoint toolTipLocation = point;
	owner->ClientToScreen(&toolTipLocation);*/

/*	CRect client;
	GetClientRect(&client);
	point.y = client.Height() - point.y; // i like the origin in lower-left...
	point.y = viewingRect.Height() - point.y; // i like the origin in lower-lef...*/

	Skeleton &skel = *workingSkeleton();
	Vector3 ankleCenter = skel.getJoint(JT_ANKLE_CENTER, true);
	//float jointScale = (viewingRect.Width() > viewingRect.Height()) ? (float)viewingRect.Width()/(2*rightBound) : (float)viewingRect.Height()/(2*topBound);

	//set<JointID> hits;

	if(!manipulating) {

		checkForHover(point);

	} else if(zoomPan && shiftDown) {// if pointer not near joint and shift key is pressed, start zooming
		CPoint Offset = CPoint(point - lastMousePoint);
		//TRACE("%d %d \n",viewingRect.Width(),viewingRect.Height());
		lastMousePoint= point;

		camera.distance -= Offset.y/25.0;
		if(camera.distance<=0)
			camera.distance=0.2;
		else
			if(camera.distance>10)
				camera.distance=10;
		
		//TRACE("%f %f\n",camera.horizontalOffset,camera.verticalOffset);

		onDraw();
		pDoc->MakeDirty();

	} else if(zoomPan) {// if pointer not near joint, start panning the camera
		CPoint Offset = CPoint(point - lastMousePoint);
		lastMousePoint= point;

		camera.horizontalOffset += Offset.x;
		camera.verticalOffset += Offset.y;

		onDraw();
		pDoc->MakeDirty();

	} else if(pDoc->useSmoothManipulation) // if using smooth joint dragging, update the joint position
	{
		//If ankle is being manipulated, then gluLookAt(in setCamera) uses the last ankle center before mouse click to centre the stick views
		if(manipJoint==JT_LANKLE || manipJoint==JT_RANKLE)
			ankleManip=true;
		else
			ankleManip=false;
		
		point.y = viewingRect.Height() - point.y; // i like the origin in lower-lef...
		CPoint Offset= CPoint(point - lastMousePoint);
		lastMousePoint=point;

		/*static TestTimer timer;
		ostringstream out;
		out << timer.SecondsSinceLast();
		this->owner->GetParentFrame()->SetWindowText(out.str().c_str());*/

		Vector3 newPoint = screen_to_world(point,originalPosition,viewingRect, ankleCenter);

		string statusText = toolTipText();
		/*string statusText = jointNames[manipJoint];
		if(manipForce) statusText = "Applied " + statusText + " Force";

		// special manipulation strings
		if(manipJoint == JT_TRAGIONLOC) statusText = "Trunk Flexion";
		else if(manipJoint == JT_RSHOULDER) statusText = "Trunk Axial Rotation";
		else if(manipJoint == JT_C7T1) statusText = "Trunk Lateral Bending";*/

		// if forces were selected, do the manipulation here
		bool manipulate_succeed;
		Vector3 newUnit = skel.getJoint((JointID)manipJoint, true) - newPoint;
		newUnit /= newUnit.length();

		int flag; // to identify the joint
		if(manipForce) {
			skel.setExtForce((JointID)manipJoint, newUnit * skel.getExtForce((JointID)manipJoint, true).length(), true);
			MainFramePtr()->StatusBarText(statusText);
		} else {
			/*if(nFlags & MK_SHIFT && manipJoint != JT_HIP_CENTER) {
				Vector3 distance = skel.getJoint(findJointPair(manipJoint), true) - skel.getJoint(JointID(manipJoint), true);
				skel.LocalDrag(newPoint, (JointID)manipJoint);
				if(nFlags & MK_CONTROL && jointPairExists(manipJoint)){
					//Knees are not working
					if (manipJoint != JT_RKNEE && manipJoint != JT_LKNEE){
						skel.LocalDrag(skel.getJoint((JointID)manipJoint, true) + distance, findJointPair(manipJoint));
					}
				}
			}*/
			if(nFlags & MK_SHIFT && manipJoint != JT_HIP_CENTER) {
				Vector3 distance = skel.getJoint(findJointPair(manipJoint), true) - skel.getJoint(JointID(manipJoint), true);
				if (manipJoint == JT_NASION)
				{
					if (viewNumber == TOP_VIEW ||viewNumber == FRONT_VIEW)
					{flag = 3;} // For nasion rotation
					else
					{flag = 2;} // For nasion flexion
					skel.LocalDrag(newPoint, (JointID)manipJoint, flag);
				}
				else{ 
					skel.LocalDrag(newPoint, (JointID)manipJoint);
				}
				if(nFlags & MK_CONTROL && jointPairExists(manipJoint)){
					if (manipJoint != JT_RKNEE && manipJoint != JT_LKNEE){
						skel.LocalDrag(skel.getJoint((JointID)manipJoint, true) + distance, findJointPair(manipJoint));
					}
				}
			}
			else {
				int options = 0;
				if(nFlags & MK_CONTROL && (manipJoint==JT_LHAND || manipJoint==JT_RHAND)) {
					options |= OPT_BOTHHANDS;
				}
				else if(nFlags & MK_CONTROL && (manipJoint==JT_LANKLE || manipJoint==JT_RANKLE)) {
					options |= OPT_BOTHANKLES;
				}
				// use local drag implementation for tragion
				if(manipJoint == JT_TRAGION) {
						flag = 1; // For tragion
					skel.LocalDrag(newPoint, (JointID)manipJoint, flag);
					manipulate_succeed = true;
				}
				else if(manipJoint == JT_LHAND){
					manipulate_succeed = skel.Move_Manipulation_Point(viewNumber, newPoint, (JointID)manipJoint, false, options,
																		originalPosition, originalPosition_pair);
				} else if(manipJoint == JT_RHAND){
					manipulate_succeed = skel.Move_Manipulation_Point(viewNumber, newPoint, (JointID)manipJoint, false, options,
																		originalPosition_pair, originalPosition);
				} else {
					manipulate_succeed = skel.Move_Manipulation_Point(viewNumber,newPoint,(JointID)manipJoint, false, options);
				}
				if(manipulate_succeed) {
					if(pDoc->getFrameType() != FT_Unset)
						pDoc->setFrameType(FT_Keyframe);
					MainFramePtr()->StatusBarText(statusText);
				} else {
					MainFramePtr()->StatusBarText(skel.getLastError());
				}
			}
		}

		pDoc->MakeDirty();
		pDoc->Analyze();
		pDoc->UpdateViews();
	}
}
void GLSkeleton::handleLButtonDown(UINT nFlags, CPoint point) {

	Skeleton &skel = *workingSkeleton();

	//start dragging/panning even if pointer is not near a joint
	manipulating = true;
	lastMousePoint= point;

	// if a joint is near the mouse, begin dragging
	if(manipJoint >= 0) {
		if(tipShowing) {
			toolTip.Cancel();
			tipShowing = false;
		}

		//coordinate_center = skel.getJoint(JT_ANKLE_CENTER, true);

		// maintain original position and applied forces for undo history
		oldAngles = skel.GetAngles();
		skel.getExtForces(oldForces);
		oldFrameType = pDoc->getFrameType();

		SetClassLong(owner->GetSafeHwnd(),GCL_HCURSOR,(long)(Get_App_Ptr()->mCrossCursorHdl));
		SetCursor(Get_App_Ptr()->mCrossCursorHdl);
		owner->SetCapture();

		lastMousePoint.y = viewingRect.Height() - lastMousePoint.y;
		lastAnkleCenter = skel.getJoint(JT_ANKLE_CENTER, true);
	}
	else{ // if no joint nearby, start panning
		zoomPan=true;
	}
}
void GLSkeleton::handleLButtonUp(UINT nFlags, CPoint point) {

	// don't manipulate on mouse-up, because users get it exactly where they want, but then
	// it changes a little when they let go of the mouse button

	if(manipulating) {
		manipulating = false;
		zoomPan=false;

		//If ankles were manipulated, then reset the stick views
		if(ankleManip){
			ankleManip=false;

			Vector3 ankleCenter = workingSkeleton()->getJoint(JT_ANKLE_CENTER,true);
			Vector3 ankCen = world_to_screen_exact(ankleCenter,viewingRect,ankleCenter);
			Vector3 lastAnkCen = world_to_screen_exact(lastAnkleCenter,viewingRect,ankleCenter);
			camera.horizontalOffset += (ankCen[0] - lastAnkCen[0])/camera.distance;
			camera.verticalOffset +=-(ankCen[1] - lastAnkCen[1])/camera.distance;
			pDoc->UpdateViews();
		}

		if(manipForce) {
			// TODO make undo for force changes
		} else {
			if(drawType == DT_Current) {
				UndoablePostureChange* upc = new UndoablePostureChange(pDoc->getCurrentFrame(), oldAngles, workingSkeleton()->GetAngles(),oldFrameType,pDoc->getFrameType());
				pDoc->addUndoEvent(upc);
			} else if(drawType == DT_Frame) {
				// TODO i don't think this ever happens. DT_Frame is for motion previews when the frames are read only
				UndoablePostureChange* upc = new UndoablePostureChange(frameNumber, oldAngles, workingSkeleton()->GetAngles(), oldFrameType, FT_Keyframe);
				pDoc->addUndoEvent(upc);
			}
		}


		SetClassLong(owner->GetSafeHwnd(),GCL_HCURSOR,(long)(Get_App_Ptr()->mArrowCursorHdl)); 
		SetCursor(Get_App_Ptr()->mArrowCursorHdl);
	}
	if(!(nFlags & MK_RBUTTON))
		ReleaseCapture();
}

void GLSkeleton::handleKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if(nChar == VK_CONTROL && manipulating && !manipForce && !(nFlags & 0x4000) /* bit 14 indicates whether the key was down previously*/) {
		if(jointPairExists(manipJoint)) {
			originalPosition = workingSkeleton()->getJoint((JointID)manipJoint, true);
			originalPosition_pair = workingSkeleton()->getJoint(findJointPair(manipJoint), true);
		}
		if(manipJoint == JT_LHAND || manipJoint == JT_RHAND || manipJoint == JT_LANKLE || manipJoint == JT_RANKLE) {
			handleMouseMove(m_nFlags | MK_CONTROL, m_point);
		}
	}

	// engage local drag if shift goes down
	if(nChar == VK_SHIFT && !(nFlags & 0x4000)) { // bit 14 indicates whether the key was down previously
		shiftDown = true;
		//TRACE("Shift going down\n");
		jointCurrentlySelectable = jointRotatable;
		// update tooltip if it exists
		if(this->tipShowing) {
			toolTip.Cancel();
			tipShowing = false;
			CPoint mouseLoc;
			::GetCursorPos(&mouseLoc);
			owner->ScreenToClient(&mouseLoc);
			this->checkForHover(mouseLoc);
		}
		// redraw to show new selectable joints
		pDoc->UpdateViews();
	}

	if(nChar == VK_CONTROL && !(nFlags & 0x4000)) { // bit 14 indicates whether the key was down previously
		ctrlDown = true;
		if(shiftDown)
			jointCurrentlySelectable = jointRotatable;
		else
			jointCurrentlySelectable = jointPairSelectable;
		// update tooltip if it exists
		if(this->tipShowing) {
			toolTip.Cancel();
			tipShowing = false;
			CPoint mouseLoc;
			::GetCursorPos(&mouseLoc);
			owner->ScreenToClient(&mouseLoc);
			this->checkForHover(mouseLoc);
		}
		// redraw to show new selectable joints
		pDoc->UpdateViews();
	}

}
void GLSkeleton::handleKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	// disengage local drag if shift goes up
	if(nChar == VK_SHIFT) {
		shiftDown = false;
		//TRACE("Shift going up\n");
		if(!ctrlDown)
			jointCurrentlySelectable = jointSelectable;
		else
			jointCurrentlySelectable = jointPairSelectable;

		// update tooltip if it exists
		if(this->tipShowing) {
			toolTip.Cancel();
			tipShowing = false;
			CPoint mouseLoc;
			::GetCursorPos(&mouseLoc);
			owner->ScreenToClient(&mouseLoc);
			this->checkForHover(mouseLoc);
		}
		
		// redraw to show new selectable joints
		pDoc->UpdateViews();
	}

	if(nChar == VK_CONTROL) {
		ctrlDown = false;
		if(!shiftDown)
			jointCurrentlySelectable = jointSelectable;
		else
			jointCurrentlySelectable = jointRotatable;

		// update tooltip if it exists
		if(this->tipShowing) {
			toolTip.Cancel();
			tipShowing = false;
			CPoint mouseLoc;
			::GetCursorPos(&mouseLoc);
			owner->ScreenToClient(&mouseLoc);
			this->checkForHover(mouseLoc);
		}
		
		// redraw to show new selectable joints
		pDoc->UpdateViews();
	}
}

void GLSkeleton::handleMouseWheel(UINT nFlags, short zDelta, CPoint point){
	camera.distance+=zDelta/2000.0;

	if(camera.distance<=0)
		camera.distance=0.2;
	else
	if(camera.distance>10)
		camera.distance=10;

	onDraw();
	pDoc->MakeDirty();
}

void GLSkeleton::setShowForces(bool show) {
	showForces = show;
}
bool GLSkeleton::getShowForces(void) {
	return showForces;
}

void GLSkeleton::setShowSupportForces(bool show) {
	showSupportForces = show;
}
bool GLSkeleton::getShowSupportForces(void) {
	return showSupportForces;
}

void GLSkeleton::setShowGrid(bool show)
{
	showGrid = show;
}

bool GLSkeleton::getShowGrid(void)
{
	return showGrid;
}
void GLSkeleton::setGridSize(double  size)
{
	GridSize = size;
}

double GLSkeleton::getGridSize(void)
{
	return GridSize;
}
void drawCube(bool hideBottom = false) {
	glBegin(GL_QUADS);

		glVertex3f(0,0,0);
		glVertex3f(0,0,1);
		glVertex3f(0,1,1);
		glVertex3f(0,1,0);

		glVertex3f(1,0,0);
		glVertex3f(1,1,0);
		glVertex3f(1,1,1);
		glVertex3f(1,0,1);

		glVertex3f(0,0,0);
		glVertex3f(1,0,0);
		glVertex3f(1,0,1);
		glVertex3f(0,0,1);

		glVertex3f(0,1,0);
		glVertex3f(0,1,1);
		glVertex3f(1,1,1);
		glVertex3f(1,1,0);

		glVertex3f(0,0,1);
		glVertex3f(1,0,1);
		glVertex3f(1,1,1);
		glVertex3f(0,1,1);

		if(hideBottom) {
			// end quads cuz you can't change color mid-glBegin/end
			glEnd();
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glColor4ub(GLSkeleton::floorColor[0], GLSkeleton::floorColor[1], GLSkeleton::floorColor[2], 128);
			glBegin(GL_QUADS);
		}

		glVertex3f(0,0,0);
		glVertex3f(0,1,0);
		glVertex3f(1,1,0);
		glVertex3f(1,0,0);

		glEnd();

		if(hideBottom) {
			glPopAttrib();
		}
}

void drawCombined(double* corners[8], bool hidebottom = false) {
	glBegin(GL_QUADS);
		// right side
		glVertex3dv(corners[0]);
		glVertex3dv(corners[2]);
		glVertex3dv(corners[3]);
		glVertex3dv(corners[1]);

		// left side
		glVertex3dv(corners[4]);
		glVertex3dv(corners[5]);
		glVertex3dv(corners[7]);
		glVertex3dv(corners[6]);

		// front side
		glVertex3dv(corners[0]);
		glVertex3dv(corners[1]);
		glVertex3dv(corners[5]);
		glVertex3dv(corners[4]);

		// back side
		glVertex3dv(corners[3]);
		glVertex3dv(corners[2]);
		glVertex3dv(corners[6]);
		glVertex3dv(corners[7]);

		// top
		glVertex3dv(corners[2]);
		glVertex3dv(corners[0]);
		glVertex3dv(corners[4]);
		glVertex3dv(corners[6]);

		if(hidebottom) {
			// end quads cuz you can't change color mid-glBegin/end
			glEnd();
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glColor4ub(GLSkeleton::floorColor[0], GLSkeleton::floorColor[1], GLSkeleton::floorColor[2], 128);
			glBegin(GL_QUADS);
		}

		// bottom
		glVertex3dv(corners[1]);
		glVertex3dv(corners[3]);
		glVertex3dv(corners[7]);
		glVertex3dv(corners[5]);

	glEnd();

	if(hidebottom) {
		glPopAttrib();
	}
}

void GLSkeleton::drawFloor(void) {
	// don't draw any floor if no feet are supported
	if(pDoc->getSupport().feetSupport == FS_Neither) {
		return;
	}
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	Skeleton* skel = this->workingSkeleton();
	Vector3 rball = skel->getJoint(JT_RBALL, true);
	Vector3 rheel = skel->getJoint(JT_RHEEL, true);
	Vector3 lball = skel->getJoint(JT_LBALL, true);
	Vector3 lheel = skel->getJoint(JT_LHEEL, true);
	double footLength = (rball - rheel).length();
	double floorLength = 1.5*footLength;
	double floorWidth = footLength;
	double floorHeight = 3;

	double heightDiff = abs(skel->getJoint(JT_RANKLE, true)[2] - skel->getJoint(JT_LANKLE, true)[2]);
	double angleDiff = abs(skel->GetAngle(A_FootRV) - skel->GetAngle(A_FootLV));

	// draw connected floor if feet are same height and angle
	// or, if it's not the case that both feet are supported,
	// this checks for single foot support
	if(!(heightDiff < 0.5 && angleDiff < 2) || pDoc->getSupport().feetSupport != FS_Both) {

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0,1.0);
		Vector3 origin;

		if(pDoc->getSupport().feetSupport != FS_Left) {
			// right foot
			origin = rheel;
			glPushMatrix();
			glTranslated(origin[0], origin[1], origin[2]);
			glRotated(skel->GetAngle(A_FootRH)-90, 0,0,1);
			glRotated(skel->GetAngle(A_FootRV), 1,0,0);
			glTranslated(-footLength/2, -(floorLength - footLength)/2, -floorHeight);
			glScaled(floorWidth, floorLength, floorHeight);

			// draw outline
			glColor3ubv(floorOutlineColor);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(1.0*viewingRect.Width()/1024);
			drawCube();

			// draw faces
			glColor3ubv(floorColor);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			drawCube(true);

			glPopMatrix();
		}

		if(pDoc->getSupport().feetSupport != FS_Right) {

			origin = lheel;
			// left foot faces
			glPushMatrix();
			glTranslated(origin[0], origin[1], origin[2]);
			glRotated(90-skel->GetAngle(A_FootLH), 0,0,1);
			glRotated(skel->GetAngle(A_FootLV), 1,0,0);
			glTranslated(-footLength/2, -(floorLength - footLength)/2, -floorHeight);
			glScaled(floorWidth, floorLength, floorHeight);

			// draw outline
			glColor3ubv(floorOutlineColor);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(1.0*viewingRect.Width()/1024);
			drawCube();

			// draw faces
			glColor3ubv(floorColor);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			drawCube(true);

			glPopMatrix();
		}

	} else {
		
		Vector3 rforward = rball - rheel;
		rforward.normalize();
		rforward *= (floorLength - footLength)/2;

		Vector3 lforward = lball - lheel;
		lforward.normalize();
		lforward *= (floorLength - footLength)/2;

		Vector3 right = cross(rball - rheel, skel->getJoint(JT_RANKLE, true) - rheel);
		right.normalize();
		right *= floorWidth/2;

		Vector3 left = cross(skel->getJoint(JT_LANKLE, true) - lheel, lball - lheel);
		left.normalize();
		left *= floorWidth/2;

		Vector3 down = cross(rforward, right);
		down.normalize();
		down *= floorHeight;

		double* corners[8];
		Vector3 fru = rball + rforward + right;
		Vector3 frd = fru + down;
		Vector3 bru = rheel - rforward + right;
		Vector3 brd = bru + down;
		Vector3 flu = lball + lforward + left;
		Vector3 fld = flu + down;
		Vector3 blu = lheel - lforward + left;
		Vector3 bld = blu + down;
		corners[0] = fru.ptr();
		corners[1] = frd.ptr();
		corners[2] = bru.ptr();
		corners[3] = brd.ptr();
		corners[4] = flu.ptr();
		corners[5] = fld.ptr();
		corners[6] = blu.ptr();
		corners[7] = bld.ptr();

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0,1.0);

		glColor3ubv(floorOutlineColor);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.0*viewingRect.Width()/1024);
		drawCombined(corners);

		glColor3ubv(floorColor);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		drawCombined(corners, true);

		//glClear(GL_DEPTH_BUFFER_BIT);
	}

	glPopAttrib();
}

void GLSkeleton::cueAngle(const AngleCueData& ACD) {
	CueEnabled = true;
	angleCue = ACD;
	if(CueFunctions[angleCue.angleType]) {
		(this->*(CueFunctions[angleCue.angleType]))();
	} else {
		// child did not make an entry for this angle type
		cancelCue();
	}
}
void GLSkeleton::updateCue() {
	if(CueEnabled) cueAngle(angleCue);
}
void GLSkeleton::cancelCue() {
	CueEnabled = false;
}

void GLSkeleton::CueLimbV() {
	Skeleton &skel = *workingSkeleton();
	double horiz = skel.GetAngle(angleCue.otherID);

	CueAlpha = HowTransparent(horiz, angleCue.angleType);
	CueAt = skel.getJoint(angleCue.proximal, true);
	CueLength = (skel.getJoint(angleCue.distal, true) - CueAt).length();
	CueAngle = skel.GetAngle(angleCue.angleID);

	Vector3 From(dcos(horiz), dsin(horiz), 0);
	if(angleCue.angleType == AT_LimbVL) {
		From[0] *= -1;
	}
	Vector3 about = cross(From, Vector3(0, 0, 1));
	makeTransform(about, From); //CueT
}
void GLSkeleton::CueLimbH() {
	Skeleton &skel = *workingSkeleton();
	CueAlpha = 1;
	CueAt = skel.getJoint(angleCue.proximal, true);
	CueLength = (skel.getJoint(angleCue.distal, true) - CueAt).length();
	CueAngle = skel.GetAngle(angleCue.angleID);

	Vector3 From(1, 0, 0);
	Vector3 about(0, 0, 1);
	if(angleCue.angleType == AT_LimbHL) {
		From[0] = -1;
		about[2] = -1;
	}
	makeTransform(about, From); // CueT
}
void GLSkeleton::CueTrunkF() {
	Skeleton& skel = *workingSkeleton();
	CueAlpha = 1;
	CueAt = skel.getJoint(JT_HIP_CENTER, true);
	CueLength = (skel.getJoint(JT_C7T1, true) - CueAt).length();
	CueAngle = skel.GetAngle(A_TrunkFlexion);

	Vector3 From(0, 1, 0);
	Vector3 about(1, 0, 0);
	makeTransform(about, From); // CueT
}
void GLSkeleton::CueTrunkLB() {
	Skeleton& skel = *workingSkeleton();
	CueAlpha = HowTransparent(skel.getExtraAngle(EA_TrunkFlexion), angleCue.angleType);
	CueAt = skel.getJoint(JT_L5S1_TEMP, true);
	CueLength = (skel.getJoint(JT_C7T1, true) - CueAt).length();
	CueAngle = skel.GetAngle(A_TrunkLateralBending);

	double TF = skel.getExtraAngle(EA_TrunkFlexion);
	Vector3 From(0, dcos(TF), dsin(TF));
	Vector3 about(0, From[2], -From[1]);
	makeTransform(about, From); // CueT
}
void GLSkeleton::CuePelvisLB() {
	Skeleton& skel = *workingSkeleton();
	CueAlpha = HowTransparent(skel.getExtraAngle(EA_PelvisFlexion), angleCue.angleType);
	CueAt = skel.getJoint(JT_HIP_CENTER, true);
	CueLength = 2*(skel.getJoint(JT_L5S1_TEMP, true) - CueAt).length();
	CueAngle = skel.GetAngle(A_PelvisLateralBending);

	double PF = skel.getExtraAngle(EA_PelvisFlexion);
	Vector3 From(0, dsin(PF), dcos(PF));
	Vector3 about(0, From[2], -From[1]);
	makeTransform(about, From); // CueT
}
void GLSkeleton::CueLegH() {
	Skeleton& skel = *workingSkeleton();
	CueAlpha = 1;
	CueAt = skel.getJoint(angleCue.proximal, true);
	CueLength = (skel.getJoint(angleCue.distal, true) - CueAt).length();
	int sign = skel.GetAngle(angleCue.angleID) < 0 ? -1 : 1;
	CueAngle = abs(skel.GetAngle(angleCue.otherID) + 90) * sign;
	Vector3 From(0, 0, -1);
	Vector3 about(1, 0, 0);
	makeTransform(about, From); // CueT
}
void GLSkeleton::makeTransform(const Vector3& about, const Vector3& From) {
	Vector3 third = cross(about, From);

	// gluPartial disk draws an arc on the x-y plane, starting from +y, with positive going towards +x, that is, about -z
	// we want an arc about "about" starting at "From".  This transformation matrix will take -z to about, and y to From.
	CueT[0] = third[0]; CueT[1] = third[1]; CueT[2] = third[2]; CueT[3] = 0;
	CueT[4] = From[0]; CueT[5] = From[1]; CueT[6] = From[2]; CueT[7] = 0;
	CueT[8] = -about[0]; CueT[9] = -about[1]; CueT[10] = -about[2]; CueT[11] = 0;
	CueT[12] = 0; CueT[13] = 0; CueT[14] = 0; CueT[15] = 1;
}


// draw the vector From, and draw an arc about "about" for "angle" degrees
// about must be unit vector
void GLSkeleton::drawCue() {
	glPushMatrix();
	glPushAttrib(GL_LINE_BIT);
	glLineWidth(1.5f);
	glColor4f(1.0, 0.0, 0.0, CueAlpha);
	
	// go to where the line will start
	glTranslated(CueAt[0], CueAt[1], CueAt[2]);
	// transform so the arc is drawn starting at the right spot, and in the correct orientation
	glMultMatrixd(CueT);
	// a close line and a far line. looks pretty good
	gluQuadricDrawStyle(quadric, GLU_SILHOUETTE);
	// draw the angle arc
	glEnable(GL_LINE_SMOOTH);
	gluPartialDisk(quadric, 0.65 * CueLength, 0.75 * CueLength, 10, 1, 0, CueAngle);
	// draw the reference line
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, CueLength, 0);
	glEnd();
	// draw - + for horizontal leg angles in 3d mode
	if(angleCue.angleType == AT_HorizontalPosNeg) {
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		const int dash_length = 2;
		const int hor_offset = 7;
		const int vert_offset = CueLength/2.0;
		// plus
		glVertex3f(hor_offset - dash_length, vert_offset, 0);
		glVertex3f(hor_offset + dash_length, vert_offset, 0);
		glVertex3f(hor_offset, vert_offset - dash_length, 0);
		glVertex3f(hor_offset, vert_offset + dash_length, 0);
		// minus
		glVertex3f(-hor_offset - dash_length, vert_offset, 0);
		glVertex3f(-hor_offset + dash_length, vert_offset, 0);
		glEnd();
	}
	glDisable(GL_POLYGON_SMOOTH);

	glPopAttrib();
	glPopMatrix();
}

float GLSkeleton::calculateZoom(void) {
	float maxDist = 0.0f;
	Skeleton &skel = *workingSkeleton();

	for(int i = 0; i < NUM_JOINTS; ++i) {
		if(!jointVisible[i]) continue;

		Vector3 location = skel.getJoint((JointID)i, true);
		Vector3 force = skel.getExtForce((JointID)i);
		Vector3 forceHandle = location - ARROW_LENGTH*force/force.length();

		for(int j = 0; j < 3; ++j) {
			if(abs(location[j]) > maxDist)
				maxDist = (float)abs(location[j]);
			if(abs(forceHandle[j]) > maxDist)
				maxDist = (float)abs(forceHandle[j]);
		}
	}

	return maxDist;
}
// these are ugly, they should be unnecessary with a new skeleton
void GLSkeleton::setLocking(LockingMode_e mode) {

	// declare the selectable joints for each locking mode
	const JointID noLocking[] = { JT_TRAGION, JT_RANKLE,JT_LANKLE,JT_HIP_CENTER,JT_RELBOW,JT_RHAND,
								JT_LELBOW,JT_LHAND,JT_C7T1,JT_SCJ,
								JT_RKNEE, JT_LKNEE};
	const int NUM_NO_LOCKING = 12;

	// armsTrunkLocking[] is telling us what remains selectable when arms and Trunk are locked
	const JointID armsTrunkLocking[] = {JT_TRAGION,JT_RANKLE,JT_LANKLE, JT_RKNEE, JT_LKNEE};
	const int NUM_ARMS_TRUNK_LOCKING = 5;

    // legsTrunkLocking[] is telling us what remains selectable when legs and Trunk are locked
	const JointID legsTrunkLocking[] = {JT_TRAGION, JT_RELBOW,JT_RHAND,JT_LELBOW,JT_LHAND};
	const int NUM_LEGS_TRUNK_LOCKING = 5;

	const JointID legsLocking[] = {JT_TRAGION, JT_RELBOW,JT_RHAND,JT_LELBOW,JT_LHAND,JT_C7T1,JT_SCJ};
	const int NUM_LEGS_LOCKING = 7;

	// reset all selectable joints
	for(int i = 0; i < NUM_JOINTS; ++i) jointSelectable[i] = false;

	// set specified joints
	switch(mode) {
		case LM_None:
			for(int i = 0; i < NUM_NO_LOCKING; ++i)
				jointSelectable[noLocking[i]] = true;
			break;
		case LM_ArmsAndTrunk:
			for(int i = 0; i < NUM_ARMS_TRUNK_LOCKING; ++i)
				jointSelectable[armsTrunkLocking[i]] = true;
			break;
		case LM_LegsAndTrunk:
			for(int i = 0; i < NUM_LEGS_TRUNK_LOCKING; ++i)
				jointSelectable[legsTrunkLocking[i]] = true;
			break;
		case LM_Legs:
			for(int i = 0; i < NUM_LEGS_LOCKING; ++i)
				jointSelectable[legsLocking[i]] = true;
			break;
		case LM_All:
			// no joints selectable
			break;
	}

	lockMode = mode;
}

std::string GLSkeleton::toolTipText() {
	string statusText = jointNames[manipJoint];
	if(manipForce)
		statusText = "Applied " + statusText + " Force";
	else{
		if(shiftDown){
			if(ctrlDown)
				statusText = "Rotate " + string(rotationNames_pairs[manipJoint]);
			else
				statusText = "Rotate " + string(rotationNames[manipJoint]);
		}
		else{
			if(!ctrlDown && manipJoint == JT_SCJ)
				statusText = "Move Torso About Spine";
			else if(ctrlDown && (manipJoint == JT_LHAND || manipJoint == JT_RHAND || manipJoint == JT_LANKLE || manipJoint == JT_RANKLE))
				statusText = "Move " + string(rotationNames_pairs[manipJoint]);
			else if(!ctrlDown && manipJoint != JT_TRAGIONLOC && manipJoint != JT_RSHOULDER && manipJoint != JT_C7T1 && manipJoint != JT_TRAGION)
				statusText = "Move " + string(jointNames[manipJoint]);
			/*else if(!ctrlDown && manipJoint == JT_TRAGIONLOC)
				statusText = "Move Trunk Flexion";*/
			else if(!ctrlDown && manipJoint == JT_RSHOULDER)
				statusText = "Move Trunk Axial Rotation";
			else if(!ctrlDown && manipJoint == JT_C7T1)
				statusText = "Move Trunk"; // previously Trunk Lateral Bending
			else if(!ctrlDown && manipJoint == JT_TRAGION)
				statusText = "Move Head/Neck";
		}
	}
	return statusText;
}

void GLSkeleton::checkForHover(CPoint point) {
	
	Skeleton &skel = *workingSkeleton();
	Vector3 ankleCenter = skel.getJoint(JT_ANKLE_CENTER, true);
	float jointScale = (viewingRect.Width() > viewingRect.Height()) ? (float)viewingRect.Width()*camera.distance/(2*rightBound) : (float)viewingRect.Height()*camera.distance/(2*topBound);
	//TRACE("%f\n",jointScale);

	set<JointID> hits;
	
	CPoint toolTipLocation = point;
	owner->ClientToScreen(&toolTipLocation);

	point.y = viewingRect.Height() - point.y; // put the origin in lower-left

	// TODO debugging posture->locations
	// set posture from pa every mouse move to see if there's an error without dragging
	/*Vector3 new_joints[NUM_JOINTS];
	Vector3 rback, lback;
	skel.SetPostureFromPA(new_joints, rback, lback);
	Analysis analysis;
	analysis.SetSkeleton(&skel);
	JointAngles ja;
	analysis.ConvertGeneralPtsToAngles(new_joints, ja, rback, lback);
	skel.SetAngles(ja);
	analysis.Analyze(&skel);
	pDoc->UpdateViews();*/

	JointID newJoint = (JointID)-1;

	// find all the joints under the cursor
	for(int i = 0; i < NUM_JOINTS; ++i) {
		// cjb changed to jointCurrentlySelectable to allow local drag tooltips
		//if(!jointSelectable[i]) continue;

		//If control is pressed, we should not be able to manipulate any joint except the hands and ankles, unless shift is also pressed
		if(!(m_nFlags & MK_SHIFT) && m_nFlags & MK_CONTROL && i!=JT_LHAND && i!=JT_RHAND && i!=JT_LANKLE && i!=JT_RANKLE) continue;

		if(!jointCurrentlySelectable[i]) continue;

		CPoint screenPoint = world_to_screen(skel.getJoint((JointID)i, true),viewingRect, ankleCenter);
        CPoint hipPoint = world_to_screen(skel.getJoint(JT_HIP_CENTER),viewingRect, ankleCenter);

		if(pow(screenPoint.x - point.x,(double)2) + pow(screenPoint.y - point.y,(double)2) < pow(POINT_RADIUS*jointScale,(double)2)) {
			hits.insert((JointID)i);
			//TRACE("hits=%d\n", *hits.begin());
		}
	}

	// if we hit one
	if(!hits.empty()) {

		// virtually find the joint 'on top'
		newJoint = getFrontmostOf(hits, false);

		// determine if the mouse moved over a 'new' joint from another one or a force
		// and cancel the old toolTip if it's showing.
		// this is to correct the case that two toolTippable things overlap,
		// and the mouse goes from one to the other, and the tip doesn't change
		if(manipJoint != newJoint || manipForce) {
			toolTip.Cancel();
			tipShowing = false;
		}
		manipForce = false;
		originalPosition = skel.getJoint((JointID)newJoint, true);
		if(jointPairExists(manipJoint)) {
			originalPosition_pair = skel.getJoint(findJointPair(manipJoint), true);
		}
		//TRACE("getting original position with manipJoint=%d\n", newJoint);

	} else {
	// if no joint is found, check applied forces

		Vector3 handle;
		for(int i = 0; i < NUM_JOINTS; ++i) {
			if(skel.getExtForce((JointID)i) == Vector3(0,0,0)) continue;

			Vector3 forceUnit = skel.getExtForce((JointID)i);
			forceUnit /= forceUnit.length();

			handle = skel.getJoint((JointID)i, true) - ARROW_LENGTH*forceUnit;
			CPoint screenPoint = world_to_screen(handle,viewingRect, ankleCenter);
			CPoint hipPoint = world_to_screen(skel.getJoint(JT_HIP_CENTER),viewingRect, ankleCenter);

			if(pow(screenPoint.x - point.x,(double)2) + pow(screenPoint.y - point.y,(double)2) < pow(POINT_RADIUS*jointScale,(double)2)) {
				hits.insert((JointID)i);
			}
		}

		if(!hits.empty()) {

			// virtually find the force 'on top'
			newJoint = getFrontmostOf(hits, true);

			// determine if the mouse moved over a 'new' force from another one or a joint
			// and cancel the toolTip if it's showing.
			// this is to correct the case that two toolTippable things overlap,
			// and the mouse goes from one to the other, and the tip doesn't change
			if(newJoint != manipJoint || !manipForce) {
				toolTip.Cancel();
				tipShowing = false;
			}
			manipForce = true;
			Vector3 forceUnit = skel.getExtForce(newJoint); forceUnit /= forceUnit.length();
			originalPosition = skel.getJoint(newJoint, true) - ARROW_LENGTH*forceUnit;
		}
	}
	manipJoint = newJoint;
	//TRACE("manipJoint = %d\n", newJoint);

	// set window behavior based on whether a joint/force is highlighted or not
	if(manipJoint != -1) {

		SetClassLong(owner->GetSafeHwnd(), GCL_HCURSOR, (long)(Get_App_Ptr()->mSelectableCursorHdl));
		//SetClassLong(m_hWnd,GCL_HCURSOR,(long)(Get_App_Ptr()->mSelectableCursorHdl)); 
		SetCursor(Get_App_Ptr()->mSelectableCursorHdl);

		string statusText = toolTipText();
        
		MainFramePtr()->StatusBarText(statusText);
		if(!tipShowing) {
			tipShowing = true;
			toolTip.SetWindowPos(NULL, toolTipLocation.x+10, toolTipLocation.y+5, 5, 14, SWP_NOZORDER|SWP_NOACTIVATE);						
			toolTip.SetWindowText(statusText.c_str());
			toolTip.ShowWindow(SW_SHOWNA);
		}
	} else {
		SetClassLong(owner->GetSafeHwnd(),GCL_HCURSOR,(long)(Get_App_Ptr()->mArrowCursorHdl)); 
		SetCursor(Get_App_Ptr()->mArrowCursorHdl);
		MainFramePtr()->StatusBarText("Ready");
		if(tipShowing) {
			toolTip.Cancel();
			tipShowing = false;
		}
		manipJoint = -1;
	}
}

// actually sorts joints based on x y or z components
void GLSkeleton::SortJoints(int dimension) {
	bool OutOfOrder = true;
	Skeleton& skel = *workingSkeleton();
	while(OutOfOrder) {
		OutOfOrder = false;
		for(int i = 0; i < NUM_JOINTS - 1; ++i) {
			if(skel.getJoint(BackToFront[i], true)[dimension] > skel.getJoint(BackToFront[i + 1], true)[dimension]) {
				JointID temp = BackToFront[i];
				BackToFront[i] = BackToFront[i+1];
				BackToFront[i+1] = temp;
				OutOfOrder = true;
			}
		}
	}
}

bool GLSkeleton::errorOnJoint(JointID joint) {
	Skeleton* skel = workingSkeleton();
	const Factors& factors = skel->getTask()->getFactors();
	// check each associated posture angle
	// if any are outside the ROM, return true
	for(set<E_ExtraAngles>::iterator it = jointsToPA[joint].begin();
		it != jointsToPA[joint].end(); ++it) {
		//if(!skel->ValidatePostureAngle(*it, skel->getExtraAngle(*it))) {
		if(factors.ValidatePostureAngle(*it, skel->getExtraAngle(*it))) {
			return true;
		}
	}
	// if no ROM violations found, return false
	return false;
}

void GLSkeleton::cameraReset(){
	//method to reset the camera i.e. offset and zoom for use in the Default View menu button
	camera.horizontalOffset=0;
	camera.verticalOffset=0;
	camera.distance=1;
}

bool GLSkeleton::jointPairExists(int jnt){
	if(jnt>=9 && jnt<=27)
		return true;
	else
		return false;
}

JointID GLSkeleton::findJointPair(int jnt){
	if(jnt>=9 && jnt<=11)
		return (JointID)(jnt+4);
	else if(jnt>=13 && jnt<=15)
		return (JointID)(jnt-4);
	else if(jnt>=18 && jnt<=21)
		return (JointID)(jnt+6);
	else if(jnt>=25 && jnt<=27)
		return (JointID)(jnt-6);
	else
		return (JointID)-1;
}

JointID GLSkeleton::findNearestJoint(int jnt){
	if(jnt>=7 && jnt<=13)
		return (JointID)(jnt-1);
	else if(jnt==17 || jnt==23)
		return (JointID)(jnt-1);
	else if(jnt>=16 && jnt<=25)
		return (JointID)(jnt-2);
	else
		return (JointID)-1;
}

void GLSkeleton::setDefaultColors() {
	backgroundColor[0] = 255;
	backgroundColor[1] = 255;
	backgroundColor[2] = 145;
	backgroundColor[3] = 255;

	leftJointColor[0] = 133;
	leftJointColor[1] = 182;
	leftJointColor[2] = 231;
	leftJointColor[3] = 255;

	rightJointColor[0] = 54;
	rightJointColor[1] = 54;
	rightJointColor[2] = 218;
	rightJointColor[3] = 255;

	forceColor[0] = 200;
	forceColor[1] = 0;
	forceColor[2] = 0;
	forceColor[3] = 255;

	floorColor[0] = 255;
	floorColor[1] = 255;
	floorColor[2] = 255;
	floorColor[3] = 255;

	floorOutlineColor[0] = 0;
	floorOutlineColor[1] = 0;
	floorOutlineColor[2] = 0;
	floorOutlineColor[3] = 255;

	skeletonColor[0] = 132;
	skeletonColor[1] = 93;
	skeletonColor[2] = 90;
	skeletonColor[3] = 255;

	jointColor[0] = 0;
	jointColor[1] = 0;
	jointColor[2] = 255;
	jointColor[3] = 255;		// bright blue for testing

	errorJointColor[0] = 255;
	errorJointColor[1] = 0;
	errorJointColor[2] = 0;
	errorJointColor[3] = 255;	// reddish testing

	leftLinkColor[0] = 133; // 154
	leftLinkColor[1] = 182; // 109
	leftLinkColor[2] = 231; // 93
	leftLinkColor[3] = 255; // copied values from leftJointColor

	rightLinkColor[0] = 54; // 154
	rightLinkColor[1] = 54; // 93
	rightLinkColor[2] = 218; // 109
	rightLinkColor[3] = 255; // copied values from rightJointColor

	GridColor[0] = 211;
	GridColor[1] = 211;
	GridColor[2] = 211;
	GridColor[3] = 255;

	rightfeetforceColor[0] = 100;
	rightfeetforceColor[1] = 54;
	rightfeetforceColor[2] = 218;
	rightfeetforceColor[3] = 255;

	leftfeetforceColor[0] = 125;
	leftfeetforceColor[1] = 125;
	leftfeetforceColor[2] = 218;
	leftfeetforceColor[3] = 255;
}

unsigned char GLSkeleton::backgroundColor[4] = {255,255,145,255};
unsigned char GLSkeleton::leftJointColor[4] = {133,182,231,255};
unsigned char GLSkeleton::rightJointColor[4] = {54,54,218,255};
unsigned char GLSkeleton::forceColor[4] = {200,0,0,255};
unsigned char GLSkeleton::floorColor[4] = {255,255,255,255};
unsigned char GLSkeleton::floorOutlineColor[4] = {0,0,0,255};
unsigned char GLSkeleton::skeletonColor[4] = {132,93,90,255};
unsigned char GLSkeleton::GridColor[4] = {211,211,211,255};

// colors for new color scheme
unsigned char GLSkeleton::jointColor[4] = {0,0,255,255};		// bright blue for testing
unsigned char GLSkeleton::errorJointColor[4] = {255,0,0,255};	// reddish testing
unsigned char GLSkeleton::leftLinkColor[4] = {133,182,231,255}; // copied values from leftJointColor
unsigned char GLSkeleton::rightLinkColor[4] = {54,54,218,255}; // copied values from rightJointColor
unsigned char GLSkeleton::rightfeetforceColor[4] = {100,54,218,255}; // purple
unsigned char GLSkeleton::leftfeetforceColor[4] = {20,250,0,250}; // lighht purple

bool GLSkeleton::showForces = true;
bool GLSkeleton::showSupportForces = false;
bool GLSkeleton::showGrid = false;
double GLSkeleton::GridSize = 20;

/*int GLSkeleton::lines[MAX_SKEL_POLYLINES][VERTS_PER_SKEL_POLYLINE] = {
   {JT_RWRIST,	    JT_RELBOW,	    JT_RSHOULDER,	JT_SCJ },
   {JT_SCJ,			JT_C7T1,		JT_L5S1_TEMP,	JT_RHIP },
   {JT_RHIP,		JT_RKNEE,		JT_RANKLE,	    JT_RHEEL },
   {JT_LWRIST,	    JT_LELBOW,	    JT_LSHOULDER,	JT_SCJ },
   {JT_TRAGION,		JT_C7T1,		JT_L5S1_TEMP,	JT_LHIP },
   {JT_LHIP,		JT_LKNEE,		JT_LANKLE,	    JT_LHEEL },
   {JT_RHIP,		JT_HIP_CENTER,	JT_LHIP,		JT_LHIP },
   {JT_RHAND,		JT_RWRIST,		JT_RWRIST,		JT_RWRIST},
   {JT_LHAND,		JT_LWRIST,		JT_LWRIST,		JT_LWRIST},
   {JT_SCJ,			JT_L5S1_TEMP,	JT_L5S1_TEMP,	JT_L5S1_TEMP}
};
int GLSkeleton::lines2D3D[MAX_SKEL_POLYLINES][VERTS_PER_SKEL_POLYLINE] = {
   {JT_RWRIST,	    JT_RELBOW,	    JT_RSHOULDER,	JT_C7T1 },
   {JT_C7T1,		JT_C7T1,		JT_L5S1_TEMP,	JT_RHIP },
   {JT_RHIP,		JT_RKNEE,		JT_RANKLE,	    JT_RHEEL },
   {JT_LWRIST,	    JT_LELBOW,	    JT_LSHOULDER,	JT_C7T1 },
   {JT_TRAGION,		JT_C7T1,		JT_L5S1_TEMP,	JT_LHIP },
   {JT_LHIP,		JT_LKNEE,		JT_LANKLE,	    JT_LHEEL },
   {JT_RHIP,		JT_HIP_CENTER,	JT_LHIP,		JT_LHIP },
   {JT_RHAND,		JT_RWRIST,		JT_RWRIST,		JT_RWRIST},
   {JT_LHAND,		JT_LWRIST,		JT_LWRIST,		JT_LWRIST},
   {JT_L5S1_TEMP,	JT_L5S1_TEMP,	JT_L5S1_TEMP,	JT_L5S1_TEMP}
};*/

/* From Skeleton.h:

enum JointID { JT_TRAGION, JT_NASION, JT_C1, JT_TRAGIONLOC, JT_C3C4, JT_C7T1, JT_SCJ, JT_L5S1_TEMP,

               JT_LSHOULDER, JT_LELBOW, JT_LWRIST, JT_LHAND,
               JT_RSHOULDER, JT_RELBOW, JT_RWRIST, JT_RHAND,
               JT_LHIP, JT_LIT, JT_LKNEE, JT_LANKLE, JT_LHEEL, JT_LBALL,
               JT_RHIP, JT_RIT, JT_RKNEE, JT_RANKLE, JT_RHEEL, JT_RBALL,
               JT_HIP_CENTER, JT_IT_CENTER, JT_ANKLE_CENTER, JT_LFOOT_CENTER, JT_RFOOT_CENTER, JT_BALL_CENTER}*/



bool GLSkeleton::jointVisible[NUM_JOINTS] = { true, true, true, false, true, true, true, true,
                                         true, true, true, true,
                                         true, true, true, true,
                                         true, false, true, true, false, true,
                                         true, false, true, true, false, true,
										 true, false, false, false, false, false, false};

bool GLSkeleton::jointSelectable[NUM_JOINTS] = {false, false, false, false, false, true, true, false,
                                            false, true, false, true,
                                            false, true, false, true,
                                            false, false, true, true, false, false,
                                            false, false, true, true, false, false,
											true, false, false, false, false, false, false};

bool GLSkeleton::jointPairSelectable[NUM_JOINTS] = {false, false, false, false, false, false, false, false,
                                            false, false, false, true,
                                            false, false, false, true,
                                            false, false, false, true, false, false,
                                            false, false, false, true, false, false,
											false, false, false, false, false, false, false};

bool GLSkeleton::jointRotatable[NUM_JOINTS] = {	false, true, false, false, false, true, true, false,
												false, true, true, true,
												false, true, true, true,
												false, false, true, true, false, true,
												false, false, true, true, false, true,
												false, false, false, false, false, false, false };

bool* GLSkeleton::jointCurrentlySelectable = GLSkeleton::jointSelectable;

bool GLSkeleton::shiftDown = false;
bool GLSkeleton::ctrlDown = false;

void GLSkeleton::drawGrid(void)
{
	 C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
	int Choice =doc->getOffsetChoice();								//if user use the default setting(0=default,1=customerized joint)
	JointID  ID =JT_ANKLE_CENTER;
	if(Choice == 1)
	{
		ID =doc->getOffsetJoint();	//ID number of the joint that we choose
	}							
	else {
			ID = JT_ANKLE_CENTER;
	}
	Skeleton &skel = *workingSkeleton();
	Vector3 Origin = skel.getJoint(ID, true);	
	if (Choice == 0)
	{
		if(doc->getPosition() == P_Standing) 
			Origin = skel.getJoint(ID, true);
		else
			Origin = doc->GetSkeleton()->getSeatReference();
	}
	Vector3 OriginOffset = doc->getOffsetCoordinate();	//the offsetvalue if it is defined
	BOOL Offset = doc->getAddOffset(); // if the user has a customerized offset from the origin
	glLineWidth(0.25f);


	 if(!doc->Is_English())
	 {
		 /*     if metric   */
	if ( !Offset )
	{
	glBegin(GL_LINES);
	glColor3f(GridColor[0]/255.0f, GridColor[1]/255.0f, GridColor[2]/255.0f);
	for(double k=Origin[2];k<=10000;k+=GridSize)	
	{
			for(double i=Origin[1];i<=10000 ;i+=GridSize)									
			{
				glVertex3f(-10000,i,k);
				glVertex3f(10000,i,k);										//draw the horizontal  line in 2-d
			}
			for(double i=Origin[1]-GridSize;i>=-10000 ;i-=GridSize)										
			{
				glVertex3f(-10000,i,k);
				glVertex3f(10000,i,k);										//draw the horizontal  line in 2-d
			}
			for(double j=Origin[0];j<=10000 ;j+=GridSize)										
			{
				glVertex3f(j,-10000,k);
				glVertex3f(j,10000,k);								//draw the verticle line 2-d
			}
			for(double j=Origin[0]-GridSize;j>=-10000 ;j-=GridSize)										
			{
				glVertex3f(j,-10000,k);
				glVertex3f(j,10000,k);								//draw the verticle line in 2-d
			}
	}
	for(double k=Origin[2]-GridSize;k>=-10000;k-=GridSize)	
	{
			for(double i=Origin[1];i<=10000 ;i+=GridSize)									
			{
				glVertex3f(-10000,i,k);
				glVertex3f(10000,i,k);										//draw the horizontal  line in 2-d
			}
			for(double i=Origin[1]-GridSize;i>=-10000 ;i-=GridSize)										
			{
				glVertex3f(-10000,i,k);
				glVertex3f(10000,i,k);										//draw the horizontal  line in 2-d
			}
			for(double j=Origin[0];j<=10000 ;j+=GridSize)										
			{
				glVertex3f(j,-10000,k);
				glVertex3f(j,10000,k);								//draw the verticle line 2-d
			}
			for(double j=Origin[0]-GridSize;j>=-10000 ;j-=GridSize)										
			{
				glVertex3f(j,-10000,k);
				glVertex3f(j,10000,k);								//draw the verticle line in 2-d
			}
	}
	//Next begin to draw verticle line in 3-d
		for(double i=Origin[1];i<=10000 ;i+=GridSize)
	{
		for (double j =Origin[0];j<=10000;j+=GridSize) 
		{
		glVertex3f(j,i,-10000);
		glVertex3f(j,i,10000);
		}
	}
		for(double i=Origin[1];i<=10000 ;i+=GridSize)
	{
		for (double j =Origin[0]-GridSize;j>=-10000;j-=GridSize) 
		{
		glVertex3f(j,i,-10000);
		glVertex3f(j,i,10000);
		}
	}
		for(double i=Origin[1]-GridSize;i>=-10000 ;i-=GridSize)
	{
		for (double j =Origin[0];j<=10000;j+=GridSize) 
		{
		glVertex3f(j,i,-10000);
		glVertex3f(j,i,10000);
		}
	}
		for(double i=Origin[1]-GridSize;i>=-10000 ;i-=GridSize)
	{
		for (double j =Origin[0]-GridSize;j>=-10000;j-=GridSize) 
		{
		glVertex3f(j,i,-10000);
		glVertex3f(j,i,10000);
		}
	}
	}

	/*
	  when  user has custom offset from the origin
	*/
	else
	{
	glBegin(GL_LINES);
	glColor3f(GridColor[0]/255.0f, GridColor[1]/255.0f, GridColor[2]/255.0f);
	for(double k=Origin[2]+OriginOffset[2];k<=10000;k+=GridSize)	
	{
			for(double i=Origin[1]+OriginOffset[1];i<=10000 ;i+=GridSize)									
			{
				glVertex3f(-10000,i,k);
				glVertex3f(10000,i,k);										//draw the horizontal  line in 2-d
			}
			for(double i=Origin[1]-GridSize+OriginOffset[1];i>=-10000 ;i-=GridSize)										
			{
				glVertex3f(-10000,i,k);
				glVertex3f(10000,i,k);										//draw the horizontal  line in 2-d
			}
			for(double j=Origin[0]+OriginOffset[0];j<=10000 ;j+=GridSize)										
			{
				glVertex3f(j,-10000,k);
				glVertex3f(j,10000,k);								//draw the verticle line 2-d
			}
			for(double j=Origin[0]-GridSize+OriginOffset[0];j>=-10000 ;j-=GridSize)										
			{
				glVertex3f(j,-10000,k);
				glVertex3f(j,10000,k);								//draw the verticle line in 2-d
			}
	}
	for(double k=Origin[2]-GridSize+OriginOffset[2];k>=-10000;k-=GridSize)	
	{
			for(double i=Origin[1]+OriginOffset[1];i<=10000 ;i+=GridSize)									
			{
				glVertex3f(-10000,i,k);
				glVertex3f(10000,i,k);										//draw the horizontal  line in 2-d
			}
			for(double i=Origin[1]-GridSize+OriginOffset[1];i>=-10000 ;i-=GridSize)										
			{
				glVertex3f(-10000,i,k);
				glVertex3f(10000,i,k);										//draw the horizontal  line in 2-d
			}
			for(double j=Origin[0]+OriginOffset[0];j<=10000 ;j+=GridSize)										
			{
				glVertex3f(j,-10000,k);
				glVertex3f(j,10000,k);								//draw the verticle line 2-d
			}
			for(double j=Origin[0]-GridSize+OriginOffset[0];j>=-10000 ;j-=GridSize)										
			{
				glVertex3f(j,-10000,k);
				glVertex3f(j,10000,k);								//draw the verticle line in 2-d
			}
	}
	//Next begin to draw verticle line in 3-d
		for(double i=Origin[1]+OriginOffset[1];i<=10000 ;i+=GridSize)
	{
		for (double j =Origin[0]+OriginOffset[0];j<=10000;j+=GridSize) 
		{
		glVertex3f(j,i,-10000);
		glVertex3f(j,i,10000);
		}
	}
		for(double i=Origin[1]+OriginOffset[1];i<=10000 ;i+=GridSize)
	{
		for (double j =Origin[0]-GridSize+OriginOffset[0];j>=-10000;j-=GridSize) 
		{
		glVertex3f(j,i,-10000);
		glVertex3f(j,i,10000);
		}
	}
		for(double i=Origin[1]-GridSize+OriginOffset[1];i>=-10000 ;i-=GridSize)
	{
		for (double j =Origin[0]+OriginOffset[0];j<=10000;j+=GridSize) 
		{
		glVertex3f(j,i,-10000);
		glVertex3f(j,i,10000);
		}
	}
		for(double i=Origin[1]-GridSize+OriginOffset[1];i>=-10000 ;i-=GridSize)
	{
		for (double j =Origin[0]-GridSize+OriginOffset[0];j>=-10000;j-=GridSize) 
		{
		glVertex3f(j,i,-10000);
		glVertex3f(j,i,10000);
		}
	}
	}
	}
	else
		/*       if  Entric*/
	{
			if ( !Offset )
	{
glBegin(GL_LINES);
	glColor3f(GridColor[0]/255.0f, GridColor[1]/255.0f, GridColor[2]/255.0f);
	for(double k=Origin[2];k<=10000;k+=GridSize/CM_TO_IN)	
	{
			for(double i=Origin[1];i<=10000 ;i+=GridSize/CM_TO_IN)									
			{
				glVertex3f(-10000,i,k);
				glVertex3f(10000,i,k);										//draw the horizontal  line in 2-d
			}
			for(double i=Origin[1]-GridSize/CM_TO_IN;i>=-10000 ;i-=GridSize/CM_TO_IN)										
			{
				glVertex3f(-10000,i,k);
				glVertex3f(10000,i,k);										//draw the horizontal  line in 2-d
			}
			for(double j=Origin[0];j<=10000 ;j+=GridSize/CM_TO_IN)										
			{
				glVertex3f(j,-10000,k);
				glVertex3f(j,10000,k);								//draw the verticle line 2-d
			}
			for(double j=Origin[0]-GridSize/CM_TO_IN;j>=-10000 ;j-=GridSize/CM_TO_IN)										
			{
				glVertex3f(j,-10000,k);
				glVertex3f(j,10000,k);								//draw the verticle line in 2-d
			}
	}
	for(double k=Origin[2]-GridSize/CM_TO_IN;k>=-10000;k-=GridSize/CM_TO_IN)	
	{
			for(double i=Origin[1];i<=10000 ;i+=GridSize/CM_TO_IN)									
			{
				glVertex3f(-10000,i,k);
				glVertex3f(10000,i,k);										//draw the horizontal  line in 2-d
			}
			for(double i=Origin[1]-GridSize/CM_TO_IN;i>=-10000 ;i-=GridSize/CM_TO_IN)										
			{
				glVertex3f(-10000,i,k);
				glVertex3f(10000,i,k);										//draw the horizontal  line in 2-d
			}
			for(double j=Origin[0];j<=10000 ;j+=GridSize/CM_TO_IN)										
			{
				glVertex3f(j,-10000,k);
				glVertex3f(j,10000,k);								//draw the verticle line 2-d
			}
			for(double j=Origin[0]-GridSize/CM_TO_IN;j>=-10000 ;j-=GridSize/CM_TO_IN)										
			{
				glVertex3f(j,-10000,k);
				glVertex3f(j,10000,k);								//draw the verticle line in 2-d
			}
	}
	//Next begin to draw verticle line in 3-d
		for(double i=Origin[1];i<=10000 ;i+=GridSize/CM_TO_IN)
	{
		for (double j =Origin[0];j<=10000;j+=GridSize/CM_TO_IN) 
		{
		glVertex3f(j,i,-10000);
		glVertex3f(j,i,10000);
		}
	}
		for(double i=Origin[1];i<=10000 ;i+=GridSize/CM_TO_IN)
	{
		for (double j =Origin[0]-GridSize/CM_TO_IN;j>=-10000;j-=GridSize/CM_TO_IN) 
		{
		glVertex3f(j,i,-10000);
		glVertex3f(j,i,10000);
		}
	}
		for(double i=Origin[1]-GridSize/CM_TO_IN;i>=-10000 ;i-=GridSize/CM_TO_IN)
	{
		for (double j =Origin[0];j<=10000;j+=GridSize/CM_TO_IN) 
		{
		glVertex3f(j,i,-10000);
		glVertex3f(j,i,10000);
		}
	}
		for(double i=Origin[1]-GridSize/CM_TO_IN;i>=-10000 ;i-=GridSize/CM_TO_IN)
	{
		for (double j =Origin[0]-GridSize/CM_TO_IN;j>=-10000;j-=GridSize/CM_TO_IN) 
		{
		glVertex3f(j,i,-10000);
		glVertex3f(j,i,10000);
		}
	}
	}

	/*
	  when  user has custom offset from the origin
	*/
	else
	{
	glBegin(GL_LINES);
	glColor3f(GridColor[0]/255.0f, GridColor[1]/255.0f, GridColor[2]/255.0f);
	for(double k=Origin[2]+OriginOffset[2]/CM_TO_IN;k<=10000;k+=GridSize/CM_TO_IN)	
	{
			for(double i=Origin[1]+OriginOffset[1]/CM_TO_IN;i<=10000 ;i+=GridSize/CM_TO_IN)									
			{
				glVertex3f(-10000,i,k);
				glVertex3f(10000,i,k);										//draw the horizontal  line in 2-d
			}
			for(double i=Origin[1]-GridSize/CM_TO_IN+OriginOffset[1]/CM_TO_IN;i>=-10000 ;i-=GridSize/CM_TO_IN)										
			{
				glVertex3f(-10000,i,k);
				glVertex3f(10000,i,k);										//draw the horizontal  line in 2-d
			}
			for(double j=Origin[0]+OriginOffset[0]/CM_TO_IN;j<=10000 ;j+=GridSize/CM_TO_IN)										
			{
				glVertex3f(j,-10000,k);
				glVertex3f(j,10000,k);								//draw the verticle line 2-d
			}
			for(double j=Origin[0]-GridSize/CM_TO_IN+OriginOffset[0]/CM_TO_IN;j>=-10000 ;j-=GridSize/CM_TO_IN)										
			{
				glVertex3f(j,-10000,k);
				glVertex3f(j,10000,k);								//draw the verticle line in 2-d
			}
	}
	for(double k=Origin[2]-GridSize/CM_TO_IN+OriginOffset[2]/CM_TO_IN;k>=-10000;k-=GridSize/CM_TO_IN)	
	{
			for(double i=Origin[1]+OriginOffset[1]/CM_TO_IN;i<=10000 ;i+=GridSize/CM_TO_IN)									
			{
				glVertex3f(-10000,i,k);
				glVertex3f(10000,i,k);										//draw the horizontal  line in 2-d
			}
			for(double i=Origin[1]-GridSize/CM_TO_IN+OriginOffset[1]/CM_TO_IN;i>=-10000 ;i-=GridSize/CM_TO_IN)										
			{
				glVertex3f(-10000,i,k);
				glVertex3f(10000,i,k);										//draw the horizontal  line in 2-d
			}
			for(double j=Origin[0]+OriginOffset[0]/CM_TO_IN;j<=10000 ;j+=GridSize/CM_TO_IN)										
			{
				glVertex3f(j,-10000,k);
				glVertex3f(j,10000,k);								//draw the verticle line 2-d
			}
			for(double j=Origin[0]-GridSize/CM_TO_IN+OriginOffset[0]/CM_TO_IN;j>=-10000 ;j-=GridSize/CM_TO_IN)										
			{
				glVertex3f(j,-10000,k);
				glVertex3f(j,10000,k);								//draw the verticle line in 2-d
			}
	}
	//Next begin to draw verticle line in 3-d
		for(double i=Origin[1]+OriginOffset[1]/CM_TO_IN;i<=10000 ;i+=GridSize/CM_TO_IN)
	{
		for (double j =Origin[0]+OriginOffset[0]/CM_TO_IN;j<=10000;j+=GridSize/CM_TO_IN) 
		{
		glVertex3f(j,i,-10000);
		glVertex3f(j,i,10000);
		}
	}
		for(double i=Origin[1]+OriginOffset[1]/CM_TO_IN;i<=10000 ;i+=GridSize/CM_TO_IN)
	{
		for (double j =Origin[0]-GridSize/CM_TO_IN+OriginOffset[0]/CM_TO_IN;j>=-10000;j-=GridSize/CM_TO_IN) 
		{
		glVertex3f(j,i,-10000);
		glVertex3f(j,i,10000);
		}
	}
		for(double i=Origin[1]-GridSize/CM_TO_IN+OriginOffset[1]/CM_TO_IN;i>=-10000 ;i-=GridSize/CM_TO_IN)
	{
		for (double j =Origin[0]+OriginOffset[0]/CM_TO_IN;j<=10000;j+=GridSize/CM_TO_IN) 
		{
		glVertex3f(j,i,-10000);
		glVertex3f(j,i,10000);
		}
	}
		for(double i=Origin[1]-GridSize/CM_TO_IN+OriginOffset[1]/CM_TO_IN;i>=-10000 ;i-=GridSize/CM_TO_IN)
	{
		for (double j =Origin[0]-GridSize/CM_TO_IN+OriginOffset[0]/CM_TO_IN;j>=-10000;j-=GridSize/CM_TO_IN) 
		{
		glVertex3f(j,i,-10000);
		glVertex3f(j,i,10000);
		}
	}
	}
	}
	glEnd();
	glFlush();
}


