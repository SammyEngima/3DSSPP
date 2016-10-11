#ifdef mdpDevelopmentBuild
	#include "dev_locals.h"
#else
	#include "locals.h"
#endif

namespace perihelion
{
	/* 
		Quaternions: [ v, s ]: x, y, z, w 

		Hyper-complex numbers
		Represent rotations in R3-space
	*/

	// Globals
	//pQuat g_quatIdent = { 0.0f, 1.0f, 0.0f, 0.0f };
	pQuat g_quatIdent = { 0.0f, 0.0f, 0.0f, 1.0f };
	pQuat_d g_quatIdent_d = { 0.0, 0.0, 0.0, 1.0 };


	void pQuatMultiply(const pQuat a, const pQuat b, pQuat out)
	{
		pVector4f t1, t2, t3;

		pVector4f_SCALE(a,b[3],t1);
		pVector4f_SCALE(b,a[3],t2);
		pVectorCross(b,a,t3);

		out[3] = a[3] * b[3] - pVectorDot(a,b);	
		out[0] = t1[0] + t2[0] + t3[0];
		out[1] = t1[1] + t2[1] + t3[1];
		out[2] = t1[2] + t2[2] + t3[2];
	}
	void pQuatMultiply_d(const pQuat_d q, const pQuat_d p, pQuat_d out)
	{
		double a = q[0], b = q[1], c = q[2], d = q[3];
		double x = p[0], y = p[1], z = p[2], w = p[3];
		pVector4d t1, t2, t3;

		pVector4f_SCALE(q,p[3],t1);
		pVector4f_SCALE(p,q[3],t2);
		pVectorCross3d(p,q,t3);

		out[3] = q[3] * p[3] - pVectorDot3d(q,p);	
		out[0] = t1[0] + t2[0] + t3[0];
		out[1] = t1[1] + t2[1] + t3[1];
		out[2] = t1[2] + t2[2] + t3[2];
		pVectorNormalize4d(out,out);
/*
		out[0] = a*x - b*y - c*z - d*w;
		out[1] = a*y + b*x + c*w - d*z;
		out[2] = a*z - b*w + c*x + d*y;
		out[3] = a*w + b*z - c*y + d*x;
		pVectorNormalize4d(out,out);
*/
	}


	/*================================*\
	* Invert v portion of quaternion *
	\*================================*/
	void pQuatInverse(const pQuat in, pQuat out)
	{
		static float lengthSquared;

		lengthSquared = pVectorDot(in,in);

		if (lengthSquared == 0.0)
		{
			pVector4f_COPY(g_quatIdent, out);
			return;
		}
		out[3] =  in[3] / lengthSquared;
		out[0] = -in[0] / lengthSquared;
		out[1] = -in[1] / lengthSquared;
		out[2] = -in[2] / lengthSquared;
	}
	void pQuatInverse_d(const pQuat_d in, pQuat_d out)
	{
		double lengthSquared;

		lengthSquared = pVectorDot3d(in,in);

		if (lengthSquared == 0.0)
		{
			pVector4f_COPY(g_quatIdent_d, out);
			return;
		}
		out[3] =  in[3] / lengthSquared;
		out[0] = -in[0] / lengthSquared;
		out[1] = -in[1] / lengthSquared;
		out[2] = -in[2] / lengthSquared;
		pVectorNormalize4d(out, out);
	}

	/*===========================================*\
	* Convert angle and axis to unit quaternion *
	\*===========================================*/
	void mdpQuatFromAngleAxis(const GLfloat theta, const pVector3f axis, pQuat quat)
	{
		quat[3] = 0.0f;
		pVector3f_COPY(axis,quat);
		pVectorNormalize(quat,quat);
		pVector3f_SCALE(quat,sinf(theta/2.0f),quat);
		quat[3] = cosf(theta/2.0f);
		pVectorNormalize4f(quat,quat);
	}
	void mdpQuatFromAngleAxis_d(const GLdouble theta, const pVector3d axis, pQuat_d quat)
	{
		quat[3] = 0.0;
		pVector3f_COPY(axis,quat);
		pVectorNormalize3d(quat,quat);
		pVector3f_SCALE(quat,sin(theta/2.0),quat);
		quat[3] = cos(theta/2.0);
		pVectorNormalize4d(quat,quat);
	}
	/*====================================*\
	* Convert a quaternion to a rotation *
	* matrix compatible with OpenGL.     *
	\*====================================*/
	void pQuatToMatrix(pQuat q, GLfloat m[4][4])
	{
		static pVector4f Sq, Ze, Thr;

		Sq[0] = q[0]*q[0];
		Sq[1] = q[1]*q[1];
		Sq[2] = q[2]*q[2];

		Ze[1] = q[0]*q[1];
		Ze[2] = q[0]*q[2];
		Ze[3] = q[0]*q[3];

	#define OneTwo Ze
		OneTwo[0] = q[1]*q[2];

		Thr[1] = q[1]*q[3];
		Thr[2] = q[2]*q[3];


		m[0][0] = 1.0f - 2.0f * (Sq[1] + Sq[2]);
		m[1][0] = 2.0f * (Ze[1] - Thr[2]);
		m[2][0] = 2.0f * (Ze[2] + Thr[1]);
		m[3][0] = 0.0f;

		m[0][1] = 2.0f * (Ze[1] + Thr[2]);
		m[1][1] = 1.0f - 2.0f * (Sq[2] + Sq[0]);
		m[2][1] = 2.0f * (OneTwo[0] - Ze[3]);
		m[3][1] = 0.0f;

		m[0][2] = 2.0f * (Ze[2] - Thr[1]);
		m[1][2] = 2.0f * (OneTwo[0] + Ze[3]);
		m[2][2] = 1.0f - 2.0f * (Ze[1]);
		m[3][2] = 0.0f;

		m[0][3] = 0.0f;
		m[1][3] = 0.0f;
		m[2][3] = 0.0f;
		m[3][3] = 1.0f;
	}

	/*=========================*\
	* vout = quat*vin*quat^-1 *
	\*=========================*/
	// <0,w> = inv(q) * <0,v> * q
	void pQuatRotateVector(const pQuat quat, const pVector3f vin, pVector3f vout)
	{
		pVector4f t1, t2, t3, t4, t5;


		// vout = s^2
		pVector3f_SCALE(vin,quat[3]*quat[3],t1);
		// t2 = vin . v
		pVector3f_SCALE(quat,pVectorDot(vin,quat),t2); 
		// t3 = v x vin
		pVectorCross(quat,vin,t3);
		// t3 = 2*s*t3 
		pVector3f_SCALE(t3,2*quat[3],t3);
		// t4 = v x vin
		pVectorCross(quat,vin,t4);
		// t5 = v x t4 
		pVectorCross(quat,t4,t5);

		vout[0] = t1[0] + t2[0] + t3[0] + t5[0];
		vout[1] = t1[1] + t2[1] + t3[1] + t5[1];
		vout[2] = t1[2] + t2[2] + t3[2] + t5[2];
	//*/
	}

	// <0,w> = inv(q) * <0,v> * q
	void pQuatRotateVector_d(const pQuat_d quat, const pVector3d vin, pVector3d vout)
	{
		pVector4d t1, t2, t3, t4, t5;


		// vout = s^2
		pVector3f_SCALE(vin,quat[3]*quat[3],t1);
		// t2 = vin . v
		pVector3f_SCALE(quat,pVectorDot3d(vin,quat),t2); 
		// t3 = v x vin
		pVectorCross3d(quat,vin,t3);
		// t3 = 2*s*t3 
		pVector3f_SCALE(t3,2*quat[3],t3);
		// t4 = v x vin
		pVectorCross3d(quat,vin,t4);
		// t5 = v x t4 
		pVectorCross3d(quat,t4,t5);

		vout[0] = t1[0] + t2[0] + t3[0] + t5[0];
		vout[1] = t1[1] + t2[1] + t3[1] + t5[1];
		vout[2] = t1[2] + t2[2] + t3[2] + t5[2];
	//*/
	}

}	// end namespace