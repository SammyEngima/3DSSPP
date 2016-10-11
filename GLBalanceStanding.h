#pragma once
#include "glbasic.h"
#include <vector>
#include "Vector3.h"

class Factors;

class GLBalanceStanding : public GLBasic {
public:
	GLBalanceStanding(CWnd* owner);
	virtual ~GLBalanceStanding(void);

private:
	virtual void draw();

public:
	// these should be refactored into a common superclass of GLBalanceSitting and GLBalanceStanding
	static void drawFeet(const float &center_x, const float &center_y, const float &factor, const Factors& lFactors);
	static void drawFoot(const float &center_x, const float &center_y, const float &factorX, const float &factorY, const Factors& lFactors, const bool& rightFoot, const bool& metric, const bool& seated);

	
	// Private Functions
	//virtual void clearColor();
	void drawGLCOP();
	void setProjectionMatrix(float w, float h);
	int  findLimitPointIndex(const std::vector<Vector3> & ptVect, int whichCoord, bool finding_max);
	void drawHexagon(std::vector<Vector3> &iOutline);
	void drawFSR(std::vector<Vector3> &iOutline);
	void drawCOPCoordinate(float x, float y, float factor);
	void drawCenterBOS(float x, float y, float factor);
	void drawCOGCoordinate(float x, float y, float factor);
	void drawLSupportForceCoordinate(float x, float y, float factor);
	void drawRSupportForceCoordinate(float x, float y, float factor);

private:	
	float COPwidth;
	float COPheight;
	bool isPrinting;

	unsigned int listIdHexagon;
	unsigned int listIdCircle;
};
