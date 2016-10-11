#ifdef mdpDevelopmentBuild
	#include "dev_locals.h"
#else
	#include "locals.h"
#endif


namespace perihelion
{
	pVector3f g_origin3f = { 0.0f, 0.0f, 0.0f };
	pVector4f g_origin4f = { 0.0f, 0.0f, 0.0f, 0.0f };


	const float pVectorAngle(const pVector3f a, const pVector3f b)
	{
		float scale = pVectorLength(a) * pVectorLength(b);
		return acosf(pVectorDot(a,b)/scale);
	}
	const float pVectorDot(const pVector3f a, const pVector3f b)
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}
	const double pVectorDot3d(const pVector3d a, const pVector3d b)
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}
	void pVectorCross(const pVector3f a, const pVector3f b, pVector3f out)
	{
		out[0] = a[1]*b[2] - a[2]*b[1];
		out[1] = a[2]*b[0] - a[0]*b[2];
		out[2] = a[0]*b[1] - a[1]*b[0];
		return;
	}
	void pVectorCross3d(const pVector3d a, const pVector3d b, pVector3d out)
	{
		out[0] = a[1]*b[2] - a[2]*b[1];
		out[1] = a[2]*b[0] - a[0]*b[2];
		out[2] = a[0]*b[1] - a[1]*b[0];
		return;
	}
	float	pVectorLength(const pVector3f in)
	{
		return sqrtf(in[0]* in[0] + in[1] * in[1] + in[2] * in[2]);
	}
	double pVectorLength3d(const pVector3d in)
	{
		return sqrt(in[0]* in[0] + in[1] * in[1] + in[2] * in[2]);
	}

	float	pVectorLength4f(const pVector4f in)
	{
		return sqrtf(in[0]*in[0] + in[1]*in[1] + in[2]*in[2] + in[3]*in[3]); // jit ... why does this return 1???
	}

	void pVectorNormalize(const pVector3f in, pVector3f out)
	{
		register float len;

		len = pVectorLength(in);
		out[0] = in[0] / len;
		out[1] = in[1] / len;
		out[2] = in[2] / len;
		return;
	}
	void pVectorNormalize3d(const pVector3d in, pVector3d out)
	{
		double len;

		len = pVectorLength3d(in);
		out[0] = in[0] / len;
		out[1] = in[1] / len;
		out[2] = in[2] / len;
		return;
	}
	void pVectorNormalize4f(const pVector4f in, pVector4f out)
	{
		float len;
		double tempWTF;

		tempWTF = in[0]*in[0] + in[1]*in[1] + in[2]*in[2] + in[3]*in[3];
		tempWTF = sqrt(tempWTF);

		len = pVector4f_LENGTH(in); 
		out[0] = in[0] / len;
		out[1] = in[1] / len;
		out[2] = in[2] / len;
		out[3] = in[3] / len;
		return;
	}
	void pVectorNormalize4d(const pVector4d in, pVector4d out)
	{
		double len;
		double tempWTF;

		tempWTF = in[0]*in[0] + in[1]*in[1] + in[2]*in[2] + in[3]*in[3];
		tempWTF = sqrt(tempWTF);

		len = pVector4d_LENGTH(in); 
		out[0] = in[0] / len;
		out[1] = in[1] / len;
		out[2] = in[2] / len;
		out[3] = in[3] / len;
		return;
	}
	void pVectorSubtract(const pVector3f a, const pVector3f b, pVector3f out)
	{
		out[0] = a[0] - b[0];
		out[1] = a[1] - b[1];
		out[2] = a[2] - b[2];
		return;
	}

	void pVectorAdd(const pVector3f a, const pVector3f b, pVector3f out)
	{
		out[0] = a[0] + b[0];
		out[1] = a[1] + b[1];
		out[2] = a[2] + b[2];
		return;
	}
}