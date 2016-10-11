#include "GLTopSideFront.h"
#include "Hom_Doc.h"
#include <algorithm>
using std::for_each;

///Yijun
#include "GLSkeleton.h"
#include "glHom/locals.h"
#include "AnthroPercentiles.h"
#include "Main_Frm.h"
#include "Hom.h"
#include "Hom_Doc.h"
#include "CQuaternion.h"
#include "Skeleton.hpp"
#include "resource.h"
#include "enums.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include <strsafe.h>
#include <olectl.h> 
#include <ole2.h>
#include <sstream>
#include "enums.h"
#include <string>
#include <algorithm>
#include "Analysis.h" // for RightVerticalAngle() etc
#include "AVIExporter.h"
//#include "StatusBox.h" // TODO, for debugging, take out
using namespace perihelion;
using std::map;
using std::list;
using std::stringstream;
using std::string;
using std::find;

#define DTOR (3.14159265358979323 / 180.0)

/* findClosest is a function object used to determine which joint or force is 'closest'
 * to the user.  the constructor takes:
 * coord - 0, 1, or 2, to tell which coordinate it should use to determine closeness. eg, front view is 1 for y.
 * force - to tell whether the JointIDs refer to the joints themselves, or the forces.
 * skel - so it can get the actual points
 */
class findClosest {
public:
	findClosest(int Coord /*0, 1, or 2*/, bool Force, Skeleton& Skel)
		: coord(Coord), closest((JointID)-1), force(Force), skel(Skel) {}
	void operator()(JointID jointID) {
		if(closest == -1) {
			closest = jointID;
			closestCoord = skel.getJoint(jointID, true)[coord];
		} else {
			// Doing Joints:
			if(!force) {
				Vector3 joint = skel.getJoint(jointID, true);
				if(joint[coord] > closestCoord) {
					closestCoord = joint[coord];
					closest = jointID;
				}

			// Doing Forces:
			} else {
				Vector3 forceUnit = skel.getExtForce(jointID);
				forceUnit /= forceUnit.length();
				Vector3 handle = skel.getJoint(jointID, true) - ARROW_LENGTH*forceUnit;
				if(handle[coord] > closestCoord) {
					closestCoord = handle[coord];
					closest = jointID;
				}
			}
		}
	}

	JointID result() { return closest; }

	JointID closest;
	double closestCoord;
	bool force;
	int coord;
	Skeleton& skel;
};

GLSkeletonTop::GLSkeletonTop(CWnd* owner) : GLSkeleton(owner) {
	GLSkeleton::viewNumber = TOP_VIEW;

	CueFunctions[AT_LimbHR] = &GLSkeleton::CueLimbH;
	CueFunctions[AT_LimbHL] = &GLSkeleton::CueLimbH;
	CueFunctions[AT_TrunkLB] = &GLSkeleton::CueTrunkLB;
	CueFunctions[AT_PelvisLB] = &GLSkeleton::CuePelvisLB;
}

GLSkeletonTop::~GLSkeletonTop(void) {
}

void GLSkeletonTop::setCamera(const Vector3& ankleCenter) {
	if(!ankleManip) {
		// set camera based on ankler center location
	    gluLookAt(ankleCenter[0], ankleCenter[1] + 10, ankleCenter[2] + LOOK_AT_DISTANCE,
		ankleCenter[0], ankleCenter[1] + 10, ankleCenter[2],
		0.0f,-1.0f,0.0f);
	} else {
		// if we're dragging the ankles, set camera based on the ankle center before we started dragging
		gluLookAt(lastAnkleCenter[0], lastAnkleCenter[1] + 10, lastAnkleCenter[2] + LOOK_AT_DISTANCE,
		lastAnkleCenter[0], lastAnkleCenter[1] + 10, lastAnkleCenter[2],
		0.0f,-1.0f,0.0f);
	}
	
	// do camera translation
	glTranslated(-camera.horizontalOffset*2*rightBound/viewingRect.Width(),camera.verticalOffset*2*topBound/viewingRect.Height(),0);
	// do camera scaling
	glScaled(camera.distance,camera.distance,camera.distance);
}
void GLSkeletonTop::drawForceTip(const Vector3 &tip,const Vector3 &handle) {
	// project direction vector onto x-z plane
	Vector3 dir = handle - tip;
	dir[2] = 0;
	float mag = dir.length();
	if(mag < 4.0) return;
	float ang = degreesBetween(dir,Vector3(0,1,0));
	if(dir[0] > 0) ang *= -1;

	glPushMatrix();
	glTranslatef(tip[0],tip[1],tip[2]);
	glRotatef(ang,0,0,1);
	glRotatef(90.0f,-1,0,0);
	gluCylinder(quadric,0,2.5,0.25f * ARROW_LENGTH,8,8);
	glPopMatrix();
}
void GLSkeletonTop::drawFloor(void) {
}
CPoint GLSkeletonTop::world_to_screen(const Vector3 &point,const CRect &client, const Vector3& ankleCenter_in) {
	Vector3 exact = world_to_screen_exact(point, client, ankleCenter_in);
	return CPoint(exact[0], exact[1]);
}
Vector3 GLSkeletonTop::world_to_screen_exact(const Vector3 &point, const CRect &client, const Vector3& ankleCenter_in) {
	Vector3 ankleCenter = ankleManip ? lastAnkleCenter : ankleCenter_in;
	return Vector3((-point[0]*camera.distance + ankleCenter[0] + rightBound)*client.Width()/(2*rightBound)+camera.horizontalOffset,
		(-point[1]*camera.distance + (ankleCenter[1] + 10)+ topBound)*client.Height()/(2*topBound)-camera.verticalOffset,
		0);
}
Vector3 GLSkeletonTop::screen_to_world(const CPoint &point,const Vector3 &ref,const CRect &client, const Vector3& ankleCenter_in) {
	Vector3 ankleCenter = ankleManip ? lastAnkleCenter : ankleCenter_in;
	return Vector3((ankleCenter[0] + rightBound - (double)(point.x-camera.horizontalOffset)*2*rightBound/client.Width()) / camera.distance,
		(-(double)((point.y+camera.verticalOffset)*2*topBound/client.Height() - topBound - (ankleCenter[1] + 10))) / camera.distance,
		ref[2]);
}
JointID GLSkeletonTop::getFrontmostOf(std::set<JointID> hits, bool force) {
	// redundant check
	if(hits.empty()) return (JointID)-1;

	else return for_each(hits.begin(), hits.end(), findClosest(2, force, *pDoc->GetSkeleton())).result();
}

double GLSkeletonTop::HowTransparent(double angle, AngleType AT) {
	switch (AT) {
		case AT_TrunkLB:
			// given trunk flexion, [-180, 180] adjust when near +- 90
			angle = abs(angle); // [0, 180] adjust when near 90
			angle -= 90; // [-90, 90] adjust when near 0
			angle = abs(angle); // [0, 90] adjust when low (near 0)
			if(angle < 15) return angle / 15;
			return 1;
			break;
		case AT_PelvisLB:
			angle = abs(angle);
			angle -= 90;
			angle = 90-abs(angle);
			if(angle < 15) return angle / 15;
			return 1;
			break;
		default:
			return 1;
			break;
	}
}
void GLSkeletonTop::OrderJoints() {
	SortJoints(2); // sort by z
}

GLSkeletonFront::GLSkeletonFront(CWnd* owner) : GLSkeleton(owner) {
	GLSkeleton::viewNumber = FRONT_VIEW;

	CueFunctions[AT_LimbVR] = &GLSkeleton::CueLimbV;
	CueFunctions[AT_LimbVL] = &GLSkeleton::CueLimbV;
	CueFunctions[AT_TrunkLB] = &GLSkeleton::CueTrunkLB;
	CueFunctions[AT_PelvisLB] = &GLSkeleton::CuePelvisLB;
}

GLSkeletonFront::~GLSkeletonFront(void) {
}

void GLSkeletonFront::setCamera(const Vector3& ankleCenter) {
	if(!ankleManip) {
		// set camera based on ankle center location
		gluLookAt(ankleCenter[0], ankleCenter[1] + LOOK_AT_DISTANCE, ankleCenter[2] + 80,
				ankleCenter[0], ankleCenter[1], ankleCenter[2] + 80,
				0.0f,0.0f,1.0f);
	} else {
		// if we're dragging the ankles, set camera based on the ankle center before we started dragging
		gluLookAt(lastAnkleCenter[0], lastAnkleCenter[1] + LOOK_AT_DISTANCE, lastAnkleCenter[2] + 80,
				lastAnkleCenter[0], lastAnkleCenter[1], lastAnkleCenter[2] + 80,
				0.0f,0.0f,1.0f);
	}
	
	// do camera translation
	glTranslated(-camera.horizontalOffset*2*rightBound/viewingRect.Width(),0,-camera.verticalOffset*2*topBound/viewingRect.Height());
	// do camera scaling
	glScaled(camera.distance,camera.distance,camera.distance);
}
void GLSkeletonFront::drawForceTip(const Vector3 &tip,const Vector3 &handle) {
	// project direction vector onto x-z plane
	Vector3 dir = handle - tip;
	dir[1] = 0;
	float mag = dir.length();
	if(mag < 4.0) return;
	float ang = -degreesBetween(dir,Vector3(0,0,1));
	if(dir[0] > 0) ang *= -1;

	glPushMatrix();
	glTranslatef(tip[0],tip[1],tip[2]);
	glRotatef(ang,0,1,0);
	gluCylinder(quadric,0,2.5,0.25f * ARROW_LENGTH,8,8);
	glPopMatrix();
}
void GLSkeletonFront::drawFloor(void) {
}
CPoint GLSkeletonFront::world_to_screen(const Vector3 &point,const CRect &client, const Vector3& ankleCenter_in) {
	Vector3 exact = world_to_screen_exact(point, client, ankleCenter_in);
	return CPoint(exact[0], exact[1]);
}
Vector3 GLSkeletonFront::world_to_screen_exact(const Vector3 &point, const CRect &client, const Vector3& ankleCenter_in) {
	Vector3 ankleCenter = ankleManip ? lastAnkleCenter : ankleCenter_in;
	return Vector3((-point[0]*camera.distance + ankleCenter[0] + rightBound)*client.Width()/(2*rightBound)+camera.horizontalOffset,
		(point[2]*camera.distance - (ankleCenter[2] + 80) + topBound)*client.Height()/(2*topBound)-camera.verticalOffset,
		0);
}
Vector3 GLSkeletonFront::screen_to_world(const CPoint &point,const Vector3 &ref,const CRect &client, const Vector3& ankleCenter_in) {
	Vector3 ankleCenter = ankleManip ? lastAnkleCenter : ankleCenter_in;
	return Vector3((rightBound + ankleCenter[0] - (double)(point.x-camera.horizontalOffset)*2*rightBound/client.Width()) / camera.distance,
		ref[1],
		((double)(point.y+camera.verticalOffset)*2*topBound/client.Height() - topBound + ankleCenter[2] + 80) / camera.distance);
}
JointID GLSkeletonFront::getFrontmostOf(std::set<JointID> hits, bool force) {
	// redundant check
	if(hits.empty()) return (JointID)-1;

	else return for_each(hits.begin(), hits.end(), findClosest(1, force, *pDoc->GetSkeleton())).result();
}

double GLSkeletonFront::HowTransparent(double angle, AngleType AT) {
	switch (AT) {
		case AT_LimbVR:
		case AT_LimbVL:
		case AT_PelvisLB:
			// start making the angle cue transparent if the limb is pointing close to the camera
			angle = abs(angle);
			angle -= 90;
			angle = abs(angle);
			if(angle < 15) return angle / 15;
			return 1;
			break;
		case AT_TrunkLB:
			angle = abs(angle);
			angle -= 90;
			angle = 90-abs(angle);
			if(angle < 15) return angle / 15;
			return 1;
			break;
		default:
			return 1;
			break;
	}
}
void GLSkeletonFront::OrderJoints() {
	SortJoints(1); // sort by y
}

GLSkeletonSide::GLSkeletonSide(CWnd* owner) : GLSkeleton(owner) {
	GLSkeleton::viewNumber = SIDE_VIEW;

	CueFunctions[AT_LimbVR] = &GLSkeleton::CueLimbV;
	CueFunctions[AT_LimbVL] = &GLSkeleton::CueLimbV;
	CueFunctions[AT_TrunkF] = &GLSkeleton::CueTrunkF;
	CueFunctions[AT_HorizontalPosNeg] = &GLSkeleton::CueLegH;
}

GLSkeletonSide::~GLSkeletonSide(void) {
}

void GLSkeletonSide::setCamera(const Vector3& ankleCenter) {
	if(!ankleManip) {
		// set camera based on the ankle center location
		gluLookAt(ankleCenter[0] + LOOK_AT_DISTANCE, ankleCenter[1] + 20, ankleCenter[2] + 80,
				ankleCenter[0], ankleCenter[1] + 20, ankleCenter[2] + 80,
				0.0f,0.0f,1.0f);
	} else {
		// if we're dragging the ankles, set camera based on the ankle center before we started dragging
		gluLookAt(lastAnkleCenter[0] + LOOK_AT_DISTANCE, lastAnkleCenter[1] + 20, lastAnkleCenter[2] + 80,
				lastAnkleCenter[0], lastAnkleCenter[1] + 20, lastAnkleCenter[2] + 80,
				0.0f,0.0f,1.0f);
	}
	// do camera translation
	glTranslated(0,camera.horizontalOffset*2*rightBound/viewingRect.Width(),-camera.verticalOffset*2*topBound/viewingRect.Height());
	// do camera scaling
	glScaled(camera.distance,camera.distance,camera.distance);
}
void GLSkeletonSide::drawForceTip(const Vector3 &tip,const Vector3 &handle) {
	// project direction vector onto y-z plane
	Vector3 dir = handle - tip;
	dir[0] = 0;
	float mag = dir.length();
	if(mag < 4.0) return;
	float ang = -degreesBetween(dir,Vector3(0,0,1));
	if(dir[1] < 0) ang *= -1;

	glPushMatrix();
	glTranslatef(tip[0],tip[1],tip[2]);
	glRotatef(ang,1,0,0);
	gluCylinder(quadric,0,2.5,0.25f * ARROW_LENGTH,8,8);
	glPopMatrix();
}
void GLSkeletonSide::drawFloor(void) {
}
CPoint GLSkeletonSide::world_to_screen(const Vector3 &point,const CRect &client, const Vector3& ankleCenter_in) {
	Vector3 exact = world_to_screen_exact(point, client, ankleCenter_in);
	return CPoint(exact[0], exact[1]);
}
Vector3 GLSkeletonSide::world_to_screen_exact(const Vector3 &point, const CRect &client, const Vector3& ankleCenter_in) {
	Vector3 ankleCenter = ankleManip ? lastAnkleCenter : ankleCenter_in;
	return Vector3((point[1]*camera.distance - (ankleCenter[1] + 20) + rightBound)*client.Width()/(2*rightBound)+camera.horizontalOffset,
		((point[2]*camera.distance - ankleCenter[2] - 80) + topBound)*client.Height()/(2*topBound)-camera.verticalOffset,
		0);
}
Vector3 GLSkeletonSide::screen_to_world(const CPoint &point,const Vector3 &ref,const CRect &client, const Vector3& ankleCenter_in) {
	Vector3 ankleCenter = ankleManip ? lastAnkleCenter : ankleCenter_in;
	return Vector3(ref[0],
		((double)(point.x-camera.horizontalOffset)*2*rightBound/client.Width() - rightBound + (ankleCenter[1] + 20)) / camera.distance,
		((double)(point.y+camera.verticalOffset)*2*topBound/client.Height() - topBound + ankleCenter[2] + 80) / camera.distance);
}
JointID GLSkeletonSide::getFrontmostOf(std::set<JointID> hits, bool force) {
	// redundant check
	if(hits.empty()) return (JointID)-1;

	else return for_each(hits.begin(), hits.end(), findClosest(0, force, *pDoc->GetSkeleton())).result();
}

double GLSkeletonSide::HowTransparent(double angle, AngleType AT) {
	switch (AT) {
		case AT_LimbVR:
		case AT_LimbVL:
			// start making the angle cue transparent if the limb is pointing close to the camera
			angle = abs(angle);
			angle -= 90;
			angle = 90-abs(angle);
			if(angle < 15) return angle / 15;
			return 1;
			break;
		default:
			return 1;
			break;
	}
}
void GLSkeletonSide::OrderJoints() {
	SortJoints(0); // sort by x
}

