#pragma once

#include "stdafx.h"
#include "gl\gl.h"								// Header File For The OpenGL32 Library
#include "gl\glu.h"								// Header File For The GLu32 Library
//#include "gl\glaux.h"							// Header File For The GLaux Library
class C_Hom_Doc;
class CWnd;

class GLBasic
{
public:
	GLBasic(CWnd* owner_in);
	virtual ~GLBasic(void);

	// do pre-draw work, call draw, do post-draw work
	// onDraw should be called by outside classes to ask OpenGL classes to show up, not draw
	void onDraw();
	// draw to arbitrary context
	void onDraw(HDC hDC, HGLRC hRC);
private:
	virtual void draw() = 0;
public:

	// for avi exporting
	void drawToMemory(LPVOID buffer);

	void setViewport(CRect rect);
	CRect getViewport();
	void setClearColor(float color[3]);

	bool initialize(CWnd* owner = 0, bool printWindow = 0);
	virtual bool share(GLBasic* source);

	virtual bool init();

	void setPrintPFD();
	void setDefaultPFD();

	CRect getViewingRect();

	void drawCOG(void);
	void notDrawCOG(void);
	void drawSF(void);
	void notDrawSF(void);

protected:
	HDC				gl_hDC;			// Private GDI Device Context
	HGLRC			gl_hRC;			// Permanent Rendering Context
	HWND			gl_hWnd;		// Holds Our Window Handle

	bool COG;
	bool SF;

	GLUquadric *quadric;
	C_Hom_Doc* pDoc;

	CWnd* owner;

	CRect viewingRect;
	float clearColor[3];

	bool sharee;

	static bool VSYNCfailed;
};
