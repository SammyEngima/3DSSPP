#pragma once
#include "glinteractive.h"
#include "glHom/model_mdp.h"
#include "glHom/mdp_main.h"
#include "enums.h"
#include "afxwin.h"
#include "Types.h"

#include <map>
#include <list>
#include <string>
#include <vector>

class JointAngles;

// various limits; all of these are in mm
#define MIN_CAM_DISTANCE 914
#define MAX_CAM_DISTANCE 15240
#define MIN_HHO_DIM 25
#define MAX_HHO_DIM 2540
#define MIN_BAR_DIM 25
#define MAX_BAR_DIM 2540
#define MIN_BAR_DISTANCE 25
#define MAX_BAR_DISTANCE 2540
#define MIN_BAR_ELEVATION -2540
#define MAX_BAR_ELEVATION 2540

// Used for rending the floor with the new hominoid
#define OBL_FLOOR_DEPTH	 8.8f
#define OBL_FLOOR_WIDTH 13.2f
#define OBL_FLOOR_HEIGHT 26.4f

// When the feet are level the floor is combined into one
#define COMBINED_OBL_FLOOR_WIDTH 79.2f
#define COMBINED_OBL_FLOOR_HEIGHT 79.2f 

#define CHAIR_LEG_DEPTH	1.76f
#define CHAIR_LEG_WIDTH 1.76f
#define CHAIR_SEAT_WIDTH 22.0f
#define CHAIR_SEAT_HEIGHT 12.0f
#define CHAIR_BACK_DEPTH 1.76f
#define CHAIR_BACK_HEIGHT 2.2f

// Hand Held Object corrections
#define LOC_CORRECTION 4.576f
#define MAG_CORRECTION 9.328f // Excludes the sphere
#define SPHERE_MAG_CORRECT 3.52f

enum Picture_Mode { PM_CENTER,PM_STRETCH };
enum PictureOrColor { PICTURE,COLOR};

struct Camera
{
    Camera(void) { }
    Camera(double hoff,double voff,int dist,int focal,int alt,int azi)
    {
        horizontalOffset = hoff;
        verticalOffset = voff;
        distance = dist;
        focalLength = focal;
        altitude = alt;
        azimuth = azi;
    }

    double horizontalOffset;
    double verticalOffset;
    int distance;
    int focalLength;
    int altitude;
    int azimuth;
};

struct HandHeldObject
{
    HandHeldObject(void) { }
    HandHeldObject(int t,bool o,float h,float d) : type(t),outline(o),height(h),depth(d) { }

    int type;
    bool outline;
    float height;
    float depth;
};

class GLOblique : public GLInteractive {
public:
	GLOblique(CWnd* owner);

private:
	void LoadHominoidMDP();
public:
	virtual ~GLOblique(void);

	void updateMDP();
	virtual void drawtext(int frame);
	virtual void draw(void);

private:
	void getBoneInfoFromModel(perihelion::MDP* model, std::map<std::string, boneInfo> &boneInfoMap, std::list<std::string> &boneNames);
	void generateQuaternions(JointAngles angle,float aPelvicTilt,std::map<std::string, boneInfo>& bones,float defWeight,float Weight);


	void drawBackground(float alpha = 1.0f);
	void updateBackground(void);
	void RenderEnvironment();
	void RenderHandHeldObjects(perihelion::pVector3f& leftHand, perihelion::pVector3f& rightHand);
	void RenderBarrier();
	void RenderChairBack();
	void RenderChair(perihelion::pVector3f& leftAnkle, perihelion::pVector3f& rightAnkle, perihelion::pVector3f& midNormal, perihelion::pVector3f& midFront);
	void RenderChairSupport(float x, float y, float z, float seatLocation, bool chairBack = false);
	void RenderCube(void);
	void RenderBarrierCube(void);
	void RenderFloor(perihelion::pVector3f& heel, float floorWidth, float floorHeight, float floorDepth);

public:
	virtual bool init(); 
	void HominoidToWorldConversion(const perihelion::pVector3f &in, perihelion::pVector3f &out);
	void SkeletonToWorldConversion(perihelion::pVector3f& jointLoc);
	bool AreFeetLevel(void);               // Custom GL "Print" Routine

	static void setframenum(bool show);
	static bool getframenum(void);
	static bool showFrameText;

	virtual void createContextMenu(CPoint point);

	virtual void handleMouseMove(UINT nFlags, CPoint point);
	virtual void handleMouseWheel(UINT nFlags, short zDelta, CPoint point);
	virtual void handleRButtonDown(UINT nFlags, CPoint point);
	virtual void handleRButtonUp(UINT nFlags, CPoint point);
	virtual void handleLButtonDown(UINT nFlags, CPoint point);
	virtual void handleLButtonUp(UINT nFlags, CPoint point);

	Camera &getCamera(void);
	Camera &getUserCamera(void);
	HandHeldObject &getHandHeld(void);

	E_Model_Type getModelType(void) const;
	//bool getShoesEnabled(void) const;
	bool getFloorEnabled(void) const;
	void setModelType(E_Model_Type type);
	//void setShoesEnabled(bool enable);
	void setFloorEnabled(bool enable);

	LPPICTURE getBackground(void) const;
	Picture_Mode getBackgroundMode(void) const;
	bool getBackgroundActive(void) const;
	float getBackgroundAlpha(void) const;
	void setBackground(LPPICTURE pic);
	void setBackgroundMode(Picture_Mode mode);
	void setBackgroundActive(bool bg);
	void setBackgroundAlpha(float alpha);


	// initializes colors to default values
	static void setDefaultColors();

	static unsigned char backgroundColor[3];
	static unsigned char skinColor[3];
	// point to either male*Color or female*Color
	static unsigned char* shirtColor;
	static unsigned char* pantsColor;

	// As a result, we now have to store both male and female colors 

    static unsigned char maleShirtColor[3];
	static unsigned char malePantsColor[3];
	static unsigned char femaleShirtColor[3];
	static unsigned char femalePantsColor[3];

	static unsigned char handHeldColor[3];
	static unsigned char handHeldOutlineColor[3];

	static double shouldDisp;
	static Vector3 motionvecs[100][32];

private:
	int axesListID;
	// hominoid
	perihelion::MDP maleMDP;
	perihelion::MDP femaleMDP;
	perihelion::MDP* currentMDP;

	// color assignment
	std::vector<int> MaleBoneListShirt;
	std::vector<int> MaleBoneListPant;
	std::vector<int> FemaleBoneListShirt;
	std::vector<int> FemaleBoneListPant;

	CPoint lastMousePoint;
	CPoint initMousePoint;
	bool bigMouseMove;

	LPPICTURE bgPicture;
	unsigned int bgTexture;
	Picture_Mode bgMode;
	bool bgActive;
	bool bgPostUpdate;
	float bgAlpha;
	bool mouseDown; 

	Camera camera,userCamera;
	HandHeldObject handheld;
	E_Model_Type modelType;
	bool floorEnabled;
};
