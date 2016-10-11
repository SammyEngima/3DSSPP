#ifndef GLSEATCOP_H
#define GLSEATCOP_H

#include "GLStart.h"
#include "GLBalanceView.h"

#define NUM_SKIN_POINTS 39
struct floatpoint2 {double x, y, z;};

const floatpoint2 skin[NUM_SKIN_POINTS] = {	{0, -6.5}, {-1, -6.5}, 
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

#define NUM_PELVIS_POINTS 51
const floatpoint2 pelvisLessThan15[NUM_PELVIS_POINTS] = {	{0, -9}, {4.5, -7},
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

const floatpoint2 pelvisBetween20and30[NUM_PELVIS_POINTS] = {	{0, -7.5}, {4.5, -6.2},
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
const  floatpoint2 pelvisBetween30and40[NUM_PELVIS_POINTS] = {	{0, -7},
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

const floatpoint2 pelvisBetween40and50[NUM_PELVIS_POINTS] = {	{0,-5.5},
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


const floatpoint2 pelvisGreaterThan50[NUM_PELVIS_POINTS] = {{0,4}, {-7.5,4.5},
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
class C_Hom_Doc;

class GLSeatCOP : public GLStart
{
    public:
        GLSeatCOP();
		void drawSeatCOP(CRect& mCOPDrawingRect);

	private:
		CRect drawingRect;
		GLubyte lineColor[3];			// This stores the color of the black line
										// this needs to be read in because it can differ based on 
										// different display settings
	       void drawSkin( float UNITS_CM, //factor
			      float Fx,
			      float Fy,
			      float factor,
			      long center_x,
			      long center_y);
		void drawPelvis(	float w, //Width
							float h, //Height
							float pAng,
							float UNITS_CM, //factor
							float Fx,
							float Fy,
							float factor,
							long center_x,
							long center_y);

		void drawCircle(	float x, float y, 
							BYTE r_fill, BYTE g_fill, 
							BYTE b_fill, BYTE o_fill,
							BYTE r_line, BYTE g_line, 
							BYTE b_line, BYTE o_line, 
							float factor);
      void setProjectionMatrix(float w, float h);

      void pelvisCorrection(	float pAng, float UNITS_CM, 
				float Fx, float Fy,
			        float factor, long center_x,
			        long center_y);
      
      float findExtreme(   bool xcoordinate, 
			   bool find_max,
			   float UNITS_CM,
			   float Fx,
			   float Fy);
      // Finds the front stability value for the seated graphic
      void drawExcursionLine(float scalingFactor, long centerOffsetX, 
                             long centerOffsetY, float unitScalingX, 
                             float unitScalingY);
      virtual void clearColor();
    
      unsigned int listIdSkin;
      unsigned int listIdPelvis;
      unsigned int listIdTuber;
      unsigned int listIdPelvisCorrection;
      unsigned int listIdRightExcursion;
      unsigned int listIdLeftExcursion;
      unsigned int listIdFrontExcursion;
      unsigned int listIdRightCorner;
      unsigned int listIdLeftCorner;
   
};
#endif