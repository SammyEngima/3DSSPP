#pragma once
#include "glinteractive.h"
#include "PupText.h"
#include "Vector3.h"
#include "Types.h"
#include "Skeleton.hpp"
#include "AngleCue.h"
#include "Task.h"
#include <set>
#include <vector>

#define MAX_SKEL_POLYLINES 10
#define VERTS_PER_SKEL_POLYLINE 4

#define LOOK_AT_DISTANCE 20.0
#define ARROW_LENGTH 25.0f
#define POINT_RADIUS 2.50

class HumosimFramework;

struct StickCamera
{
    StickCamera(void) { }
    StickCamera(double hoff,double voff,double dist)
    {
        horizontalOffset = hoff;
        verticalOffset = voff;
        distance = dist;
    }

    double horizontalOffset;
    double verticalOffset;
    double distance;
};

class GLSkeleton : public GLInteractive {
public:
	GLSkeleton(CWnd* owner);
	virtual ~GLSkeleton(void);

	virtual bool init();

	// **** Drawing **** //
private:
	virtual void draw();
	void drawSkeleton(void);
	void drawLinkSet(const std::set< std::vector<JointID> >& linkSet);
	void drawForces(void);
	void drawFeetForces(void);
	void drawFeet(void);
	void drawHead(void);
	void drawJoints(void);

	// **** Interaction handlers **** //
public:
	virtual void createContextMenu(CPoint point);
	virtual void handleMouseMove(UINT nFlags, CPoint point);
	virtual void handleLButtonDown(UINT nFlags, CPoint point);
	virtual void handleLButtonUp(UINT nFlags, CPoint point);
	virtual void handleKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void handleKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void handleMouseWheel(UINT nFlags, short zDelta, CPoint point);

	//virtual void handleRButtonDown(UINT nFlags, CPoint point);
	//virtual void handleRButtonUp(UINT nFlags, CPoint point);

	// **** Accessors **** //
	static void setShowForces(bool show);
	static bool getShowForces(void);
	static void setShowSupportForces(bool show);
	static bool getShowSupportForces(void);
	static void setShowGrid(bool show);
	static bool getShowGrid(void);
	static void setGridSize(double size);
	static double getGridSize(void);

	// **** Conversions and other orientation specific methods **** //
private:
	// set the modelview matrix
	virtual void setCamera(const Vector3& ankleCenter) = 0;
	virtual void drawForceTip(const Vector3 &tip,const Vector3 &handle) = 0;
	void drawFloor(void);
	// map from model space to screen space
	// reproduces the modelview matrix transform
	virtual CPoint world_to_screen(const Vector3 &point,const CRect &client, const Vector3& ankleCenter) = 0;
	virtual Vector3 world_to_screen_exact(const Vector3 &point, const CRect &client, const Vector3& ankleCenter) = 0;
	// map from screen space to model space
	// reproduces the inverse of the modelview matrix transform
	virtual Vector3 screen_to_world(const CPoint &point,const Vector3 &ref,const CRect &client, const Vector3& ankleCenter) = 0;
	virtual JointID getFrontmostOf(std::set<JointID> hits, bool force) = 0;

	// **** Angle Cues **** //
	// see GLView.h for a description of the angle cue system
public:
	virtual void cueAngle(const AngleCueData& ACD);
	virtual void updateCue();
	virtual void cancelCue();
	virtual double HowTransparent(double angle, AngleType AT) = 0;
	void CueLimbV();
	void CueLimbH();
	void CueTrunkF();
	void CueTrunkLB();
	void CuePelvisLB();
	void CueLegH();
	void makeTransform(const Vector3& about, const Vector3& From);
	// draw the vector From, and draw an arc about "about" for "angle" degrees
	// about must be unit vector
	void drawCue();

	//method to reset the camera i.e. offset and zoom
	void cameraReset();

	// **** Miscellaneous helper functions **** //
	float calculateZoom(void);
	// these are ugly, they should be unnecessary with a new skeleton
	void setLocking(LockingMode_e mode);
	// method which orders the joints
	// Orthoview implementation will call SortJoints with appropriate dimension
	
	// generate text to go on tooltips when hovering on joints or forces
	std::string toolTipText();
	void checkForHover(CPoint point);

	// methods to determine if a pair exist for a joint
	bool jointPairExists(int jnt);
	JointID findJointPair(int jnt);
	JointID findNearestJoint(int jnt);

	virtual void OrderJoints() = 0;

protected:
	// actually sorts joints based on x y or z components
	void SortJoints(int dimension);

protected:
	int viewNumber;

	static bool showForces;
	static bool showSupportForces;
	static bool showGrid;
	static double  GridSize;

	// saved on mouse downs to make an undo event
	Vector3 oldForces[NUM_JOINTS];
	JointAngles oldAngles;
	FrameType_e oldFrameType;

	int rightBound, topBound;

	CPopupText toolTip;
	bool tipShowing;
	LockingMode_e lockMode;

	StickCamera camera;

	int manipJoint;
	bool manipulating;
	bool manipForce;
	bool zoomPan;// set to true when LButton is pressed away from any joint, for panning and zooming
	bool ankleManip;//set to true when one or both of the ankles are being manipulated

	CPoint lastMousePoint;
	Vector3 lastAnkleCenter;

	Vector3 originalPosition;
	// currently used to hold the original position of the opposite hand,
	// when dragging while holding control
	Vector3 originalPosition_pair;
	// true if shift is being held, so use is doing local drag
	static bool shiftDown;
	static bool ctrlDown;

	// remember last mouse move arguments so we can fake calls to handleMouseMove
	// currently used for faking a call when control is pressed
	UINT m_nFlags;
	CPoint m_point;

	// TODO this should definitely be a class
	typedef void (GLSkeleton::* CueFunc)();
	std::vector<CueFunc> CueFunctions;
	// back up the cue data for updates
	AngleCueData angleCue;
	// angle cue stored data so it isn't computed on every draw
	bool CueEnabled;
	double CueAlpha;
	Vector3 CueAt;
	double CueT[16];
	double CueLength;
	double CueAngle;
	
	// joints in order for painter's algorithm
	JointID BackToFront[NUM_JOINTS];

	// display lists
	GLuint Selectable, Unselectable;

	// map from joints to the posture angles that, if violated, cause
	// the joint to be rendered in red
	// TODO this isn't really specific to GLSkeleton and should probably
	// be defined somewhere else
	std::set<E_ExtraAngles> jointsToPA[NUM_JOINTS];
	bool errorOnJoint(JointID joint);

	// the set of left joints
	std::set<JointID> leftJoints;
	// the set of right joints
	std::set<JointID> rightJoints;
	// the set of joints that aren't left or right
	std::set<JointID> centerJoints;

public:

	//bool currentView;

	// sets all colors to default values
	static void setDefaultColors();

	static unsigned char backgroundColor[4];
	static unsigned char skeletonColor[4];
	static unsigned char leftJointColor[4];
	static unsigned char rightJointColor[4];
	static unsigned char forceColor[4];
	static unsigned char rightfeetforceColor[4];
	static unsigned char leftfeetforceColor[4];
	static unsigned char floorColor[4];
	static unsigned char floorOutlineColor[4];
	static unsigned char GridColor[4];

	// colors for new color scheme
	static unsigned char jointColor[4];
	static unsigned char errorJointColor[4];
	static unsigned char leftLinkColor[4];
	static unsigned char rightLinkColor[4];

	static bool jointVisible[NUM_JOINTS];
	static bool jointSelectable[NUM_JOINTS];
	static bool jointPairSelectable[NUM_JOINTS];
	static bool jointRotatable[NUM_JOINTS];
	static bool* jointCurrentlySelectable; // points to either jointRotatable or jointSelectable, depending on shift status
	//static int lines[MAX_SKEL_POLYLINES][VERTS_PER_SKEL_POLYLINE];
	//static int lines2D3D[MAX_SKEL_POLYLINES][VERTS_PER_SKEL_POLYLINE];

	// new way to organize links to be drawn
	std::set< std::vector<JointID> > rightLines;
	std::set< std::vector<JointID> > leftLines;
	std::set< std::vector<JointID> > centerLines;

	std::set< std::vector<JointID> > rightLines2D3D;
	std::set< std::vector<JointID> > leftLines2D3D;
	std::set< std::vector<JointID> > centerLines2D3D;
	void drawGrid(void);
};
