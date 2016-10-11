#ifndef MDP_MAIN_H
#define MDP_MAIN_H

#pragma once 
#include "math_quaternion.h"

#define makechar(x)  #@x

#define MDP_FILETYPE(VMAJOR)	( ('M' << 24) | ('D' << 16) | ('P' << 8) | (#@VMAJOR) )

#define MDP_VERSION_MAJOR	1
#define MDP_VERSION_MINOR1	0
#define MDP_VERSION_MINOR2	0
#define MDP_VERSION_MINOR3	1

#define MDP_VERSION ( (MDP_VERSION_MAJOR << 24) | (MDP_VERSION_MINOR1 << 16) | (MDP_VERSION_MINOR2 << 8) | (MDP_VERSION_MINOR3) )


#define SELECTION_ALL_OFF	(0)
#define SELECTION_ADD		(1)
#define SELECTION_TOGGLE	(2)
#define SELECTION_REMOVE	(3)
#define SELECTION_ALL_ON	(255)

#define MDP_MAX_MESHES		255
// =======================================================================================
// This is a structure which lets us make lists of indicies to draw primitives.  The list
// structure is not limited to any type of primitive, but the application must somehow
// remember which type of primitive it is dealing with
typedef struct glList_s
{
	int length;
	GLuint *indexes;
	GLenum type;		// Type of OpenGL primitve
} glList_t;

// glList types
enum 
{
	triFans		= GL_TRIANGLE_FAN		- GL_TRIANGLES,
	triList		= GL_TRIANGLES			- GL_TRIANGLES,
	triStrips	= GL_TRIANGLE_STRIP		- GL_TRIANGLES,
	quadList	= GL_QUADS				- GL_TRIANGLES,
	quadStrips	= GL_QUAD_STRIP			- GL_TRIANGLES, 
	numGLLists	= 5		// Keep this at the end
};
typedef struct pGLCommand_s
{
	int null;
} pGLCommand_t;

// =======================================================================================
// The shader class contains a list of openGL commands in user defined sequences
// which when bound to a mesh will allow for execution of the commands on those primitves
class pShader
{
public:
	pShader();
	~pShader();
	size_t size(void);
protected:
	int				mNumCommands;
	pGLCommand_t	*mCommandList;
};

class boneInfo
{
	public:
	boneInfo() : weight(1), length(1), refWeight(1), refLength(1) { pVector4f_COPY(perihelion::g_quatIdent, q); }
	boneInfo(perihelion::pQuat quat) : weight(1), length(1), refWeight(1), refLength(1) { pVector4f_COPY(quat, q); }
	boneInfo(double w, double l, double rw, double rl, perihelion::pQuat quat) :
weight(w), length(l), refWeight(rw), refLength(rl) { pVector4f_COPY(quat, q); } 
	double weight, length;
	double refWeight, refLength;
	perihelion::pQuat_d q;
};


#ifdef mdpDevelopmentBuild
	#include "dev_locals.h"
#else
	#include "locals.h"
#endif

#endif