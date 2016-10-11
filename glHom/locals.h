#ifndef LOCALS_H
#define LOCALS_H

/******************************************************************

	locals.h

	This header file includes all the parts of the model rendering object.

*******************************************************************/

#ifndef SHOW_WARNINGS
#pragma warning( disable : 4002 )	// disable macro problem warnings
#pragma warning( 4 : 4244 )			// disable "conversion" warnings
#endif

#pragma once

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

//#include <windows.h>		// Header File For Windows
//#include <wincon.h>			// Windows DOS console functions
#include "../SSPPApp/stdafx.h"
#include <stdio.h>			// Standard I/O libraries
#include <stdlib.h>			// Standard libraries
#include <math.h>			// 
#include <string>
#include <iostream> 
#include <fstream>
#include <time.h>

#include <gl/gl.h>					// Header File For The OpenGL32 Library
#include <gl/glu.h>					// Header File For The GLu32 Library
//#include <gl/glaux.h>				// Header File For The Glaux Library
//#include "glext.h"			

#define MDP_MAX_MESHES		255

#include "g_types.h"
#include "mdp_main.h"
#include "math_misc.h"
#include "math_vector.h"
#include "math_quaternion.h"
#include "model_bone.h"
#include "model_mesh.h"
#include "model_mdp.h" 

#define gmalloc(x) malloc(x)
#define gfree(x) if (x != NULL) do { free(x); } while (0)

#endif