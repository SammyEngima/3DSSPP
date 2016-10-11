#pragma once
#include "glbasic.h"

#define NUM_PELVIS_POINTS 51
#define NUM_SKIN_POINTS 39

struct floatpoint3 {double x, y, z;};

class GLBalanceSitting : public GLBasic {
public:
	GLBalanceSitting(CWnd* owner);
	virtual ~GLBalanceSitting(void);

private:
	virtual void draw();
public:
	void drawSkin(float UNITS_CM, //factor
					float Fx,
					float Fy,
					float factor,
					long center_x,
					long center_y);
	void drawPelvis(float w, //Width
					float h, //Height
					float pAng,
					float UNITS_CM, //factor
					float Fx,
					float Fy,
					float factor,
					long center_x,
					long center_y);

	void drawCircle(float x, float y, 
					BYTE r_fill, BYTE g_fill, 
					BYTE b_fill, BYTE o_fill,
					BYTE r_line, BYTE g_line, 
					BYTE b_line, BYTE o_line,
					float factor);
	void drawCOP(float x, float y, float factor);
	void setProjectionMatrix(float w, float h);

	void pelvisCorrection(float pAng, float UNITS_CM, float Fx, float Fy,
							float factor, long center_x, long center_y);

	float findExtreme(bool xcoordinate, bool find_max, float UNITS_CM, float Fx, float Fy);
	// Finds the front stability value for the seated graphic
	void drawExcursionLine(float scalingFactor, long centerOffsetX,
							long centerOffsetY, float unitScalingX, 
							float unitScalingY);

private:
	static floatpoint3 skin[NUM_SKIN_POINTS];
	static floatpoint3 pelvisLessThan15[NUM_PELVIS_POINTS];
	static floatpoint3 pelvisBetween20and30[NUM_PELVIS_POINTS];
	static floatpoint3 pelvisBetween30and40[NUM_PELVIS_POINTS];
	static floatpoint3 pelvisBetween40and50[NUM_PELVIS_POINTS];
	static floatpoint3 pelvisGreaterThan50[NUM_PELVIS_POINTS];
};
