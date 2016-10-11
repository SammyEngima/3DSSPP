#ifndef GL_BACKGROUND_H
#define GL_BACKGROUND_H

#pragma once
#include <stdafx.h>
#include <gl\gl.h>								// Header File For The OpenGL32 Library
#include <gl\glu.h>								// Header File For The GLu32 Library
#include <gl\glaux.h>							// Header File For The GLaux Library
#include "quad_frm.h"
#include "force.hpp"
#include "resource.h"
//class C_Hom_View;
class C_Hom_Doc;
//class CGLObliqueView;

class GL_Background
{
public:
	GL_Background();
	bool Load(CView *view_pointer);
	void Draw(CDC* pDC);
	bool Changed();
	void SetPicturePathName(CString newPath) { currentPicturePathName = newPath; };
	CString GetPicturePathName() { return currentPicturePathName; };
protected:
	long lHeightPixels, lWidthPixels;
	CString currentPicturePathName;
	GLuint texture_id;
	CView *view_ptr;
};

#endif
