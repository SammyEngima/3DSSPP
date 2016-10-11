#pragma once
#include "glbasic.h"
#include "Vector3.h"

#define OBLIQUE_VIEW 4
#define TOP_VIEW 3
#define SIDE_VIEW 2
#define FRONT_VIEW 1

enum DrawType_e {DT_Current, DT_Frame, DT_Skeleton};

struct AngleCueData;
class Skeleton;

class GLInteractive : public GLBasic {
public:
	GLInteractive(CWnd* owner);
	virtual ~GLInteractive(void);

	//void setMotionPrediction(int frame_number, const Vector3& target_location);
	//void setNotMotionPrediction();
	void setDrawType(DrawType_e type);
	DrawType_e getDrawType() const;
	void setSkeleton(Skeleton* skel);
	Skeleton* getSkeleton();
	void setFrame(int frame_number);
	int getFrame();
	//void drawCurrent();

	void setDrawTarget(const Vector3& target_location);
	void setNoDrawTarget();

	virtual void createContextMenu(CPoint point) { }
	virtual void handleMouseMove(UINT nFlags, CPoint point) { }
	virtual void handleMouseWheel(UINT nFlags, short zDelta, CPoint point) { }
	virtual void handleLButtonDown(UINT nFlags, CPoint point) { }
	virtual void handleLButtonUp(UINT nFlags, CPoint point) { }
	virtual void handleRButtonDown(UINT nFlags, CPoint point) { }
	virtual void handleRButtonUp(UINT nFlags, CPoint point) { }
	virtual void handleKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) { }
	virtual void handleKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) { }

	virtual void cueAngle(const AngleCueData& ACD) {}
	virtual void updateCue() {}
	virtual void cancelCue() {}

protected:
	// Tells whether to draw the current frame, a frame specified by
	// frameNumber, or the skeleton pointed to by skel
	DrawType_e drawType;
	//bool drawAtFrame;
	// which frame to draw, used if drawAtFrame is true
	int frameNumber;
	// skeleton to draw if drawType == DT_Skeleton
	Skeleton* drawSkel;

	// true if the graphic should draw a target at targetLocation
	bool drawTarget;
	// the location at which to draw the target
	Vector3 targetLocation;

	// returns either the current skeleton, the skeleton at frameNumber,
	// or drawSkel depending on drawType.
	Skeleton* workingSkeleton();
};
