#include "GLInteractive.h"
#include "Hom_Doc.h"
#include "Skeleton.hpp"

GLInteractive::GLInteractive(CWnd* owner) : GLBasic(owner) {
	drawType = DT_Current;
	//drawAtFrame = false;
	drawTarget = false;
	drawSkel = 0;
}

GLInteractive::~GLInteractive(void) {
}

/*void GLInteractive::setMotionPrediction(int frame_number, const Vector3& target_location) {
	drawForMotionPrediction = true;
	frameNumber = frame_number;
	targetLocation = target_location;
}
void GLInteractive::setNotMotionPrediction() {
	drawForMotionPrediction = false;
}*/

void GLInteractive::setDrawType(DrawType_e type) {
	drawType = type;
}
DrawType_e GLInteractive::getDrawType() const {
	return drawType;
}
void GLInteractive::setSkeleton(Skeleton* skel_in) {
	drawSkel = skel_in;
}
Skeleton* GLInteractive::getSkeleton() {
	return drawSkel;
}
void GLInteractive::setFrame(int frame_number) {
	frameNumber = frame_number;
}
int GLInteractive::getFrame() {
	return frameNumber;
}
/*void GLInteractive::drawFrame(int frame_number) {
	drawAtFrame = true;
	frameNumber = frame_number;
}

void GLInteractive::drawCurrent() {
	drawAtFrame = false;
}*/

void GLInteractive::setDrawTarget(const Vector3& target_location) {
	drawTarget = true;
	//Skeleton* skel = workingSkeleton();
	//targetLocation = target_location + skel->getJoint(JT_LBALL, true) - skel->getJoint(JT_ANKLE_CENTER, true);
	targetLocation = target_location;
}
void GLInteractive::setNoDrawTarget() {
	drawTarget = false;
}

Skeleton* GLInteractive::workingSkeleton() {
	switch(drawType) {
		case DT_Current:
			return pDoc->GetSkeleton();
			break;
		case DT_Frame:
			return pDoc->getSkeletonAtFrame(frameNumber);
			break;
		case DT_Skeleton:
			return drawSkel;
			break;
		default:
			assert(0);
			return 0;
			break;
	}
}