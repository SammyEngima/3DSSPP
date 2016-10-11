#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H

#include "../stdafx.h"
#include <gl/glu.h>

#pragma once
namespace perihelion
{

	typedef struct pVector2f_s {	GLfloat x, y;		} pVector2f_t;
	typedef struct pVector3f_s {	GLfloat x, y, z;	} pVector3f_t;
	typedef struct pVector4f_s {	GLfloat x, y, z, t;	} pVector4f_t;
	typedef struct pVector2d_s {	GLdouble x, y;		} pVector2d_t;
	typedef struct pVector3d_s {	GLdouble x, y, z;	} pVector3d_t;
	typedef struct pVector4d_s {	GLdouble x, y, z, t;} pVector4d_t;

	typedef GLdouble pVector2d[2];
	typedef GLdouble pVector3d[3];
	typedef GLdouble pVector4d[4];

	typedef GLfloat pVector2f[2];
	typedef GLfloat pVector3f[3];
	typedef GLfloat pVector4f[4];

	typedef GLint	pVector2i[2];
	typedef GLint	pVector3i[3];
	typedef GLint	pVector4i[4];

	extern pVector3f g_origin3f;
	extern pVector4f g_origin4f;

	extern pVector3d g_origin3d;
	extern pVector4d g_origin4d;

	#define pVector3f_SUBTRACT(a,b,out)	(out[0]=a[0]-b[0], out[1]=a[1]-b[1], out[2]=a[2]-b[2])
	#define pVector3f_ADD(a,b,out)		(out[0]=a[0]+b[0], out[1]=a[1]+b[1], out[2]=a[2]+b[2])
	#define pVector3f_DOT(a,b)			(a[0]*b[0] + a[1]*b[1] + a[2]*b[2])
	#define pVector3f_COPY(a,out)		(out[0]=a[0], out[1]=a[1], out[2]=a[2])
	#define pVector3f_SET(v, x, y, z)	(v[0]=(x), v[1]=(y), v[2]=(z))
	#define pVector3f_ZERO(v)			(v[0]=0.0f, v[1]=0.0f, v[2]=0.0f)
	#define pVector3f_INV(a, out)		(out[0]=-a[0], out[1]=-a[1], out[2]=-a[2])
	#define pVector3f_SCALE(v,s,out)	((out)[0]=(v)[0]*(s),(out)[1]=(v)[1]*(s),(out)[2]=(v)[2]*(s))

	#define pVector4f_SUBTRACT(a,b,out)	(out[0]=a[0]-b[0], out[1]=a[1]-b[1], out[2]=a[2]-b[2], out[3]=a[3]-b[3])
	#define pVector4f_ADD(a,b,out)		(out[0]=a[0]+b[0], out[1]=a[1]+b[1], out[2]=a[2]+b[2], out[3]=a[3]+b[3])
	#define pVector4f_DOT(a,b)			(a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3])
	#define pVector4f_COPY(a,out)		(out[0]=a[0], out[1]=a[1], out[2]=a[2], out[3]=a[3])
	#define pVector4f_SET(v, w, x, y, z)(v[0]=(w), v[1]=(x), v[2]=(y), v[3]=(z))
	#define pVector4f_ZERO(v)			(v[0]=0.0f, v[1]=0.0f, v[2]=0.0f, v[3]=0.0f)
	#define pVector4f_INV(a, out)		(out[0]=-a[0], out[1]=-a[1], out[2]=-a[2], out[3]=-a[3])
	#define pVector4f_SCALE(v,s,out)	((out)[0]=(v)[0]*(s),(out)[1]=(v)[1]*(s),(out)[2]=(v)[2]*(s),(out)[3]=(v)[3]*(s))
	#define pVector4f_LENGTH(v)			(sqrtf((v)[0]*(v)[0] + (v)[1]*(v)[1] + (v)[2]*(v)[2] + (v)[3]*(v)[3])) // jit

	#define pVector4d_LENGTH(v)			(sqrt((v)[0]*(v)[0] + (v)[1]*(v)[1] + (v)[2]*(v)[2] + (v)[3]*(v)[3])) // jit

	const float pVectorAngle	  (const pVector3f a,	const pVector3f b					);
	const float pVectorDot		  (const pVector3f a,	const pVector3f b					);
	void		pVectorCross	  (const pVector3f a,	const pVector3f b,	pVector3f out	);
	float		pVectorLength	  (const pVector3f in										);
	float		pVectorLength4f	  (const pVector4f in										);
	void		pVectorNormalize  (const pVector3f in,	pVector3f out						);
	void		pVectorNormalize4f(const pVector3f in,	pVector3f out						);
	void		pVectorSubtract	  (const pVector3f a,	const pVector3f b,	pVector3f out	);
	void		pVectorAdd		  (const pVector3f a,	const pVector3f b,	pVector3f out	);

	const double pVectorAngle3d	  (const pVector3d a,	const pVector3d b					);
	const double pVectorDot3d	  (const pVector3d a,	const pVector3d b					);
	void		pVectorCross3d	  (const pVector3d a,	const pVector3d b,	pVector3d out	);
	double		pVectorLength3d	  (const pVector3d in										);
	double		pVectorLength4d	  (const pVector4d in										);
	void		pVectorNormalize3d(const pVector3d in,	pVector3d out						);
	void		pVectorNormalize4d(const pVector3d in,	pVector3d out						);
	void		pVectorSubtract3d (const pVector3d a,	const pVector3d b,	pVector3d out	);
	void		pVectorAdd3d	  (const pVector3d a,	const pVector3d b,	pVector3d out	);
}

#endif