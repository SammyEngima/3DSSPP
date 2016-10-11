#include "CQuaternion.h"
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include "glHom/math_vector.h"
using namespace perihelion;
using std::string;
using std::vector;	using std::reverse;
using std::iterator;

CQuaternion::CQuaternion(pVector4d quat, string name) 
{
	q[0] = quat[0];
	q[1] = quat[1];
	q[2] = quat[2];
	q[3] = quat[3];
	Normalize();
#ifdef _DEBUG
	m_history.push_back(name);
#endif
}

CQuaternion::CQuaternion(pVector4f quat, string name) 
{
	q[0] = quat[0];
	q[1] = quat[1];
	q[2] = quat[2];
	q[3] = quat[3];
	Normalize();
#ifdef _DEBUG
	m_history.push_back(name);
#endif
}

CQuaternion::CQuaternion(pVector3d axis, double theta, string name)
{
	double scale = sin(theta/2.0);
	q[0] = axis[0];
	q[1] = axis[1];
	q[2] = axis[2];
	q[3] = 0;
	Normalize();
	q[0] *= scale;
	q[1] *= scale;
	q[2] *= scale;
	q[3] = cos(theta/2.0);
	Normalize();
#ifdef _DEBUG
	m_history.push_back(name);
#endif
}
CQuaternion::CQuaternion(Vector3 axis, double theta, string name)
{
	double scale = sin(theta/2.0);
	q[0] = axis[0];
	q[1] = axis[1];
	q[2] = axis[2];
	q[3] = 0;
	Normalize();
	q[0] *= scale;
	q[1] *= scale;
	q[2] *= scale;
	q[3] = cos(theta/2.0);
	Normalize();
#ifdef _DEBUG
	m_history.push_back(name);
#endif
}

CQuaternion::CQuaternion(double a, double b, double c, double d, string name) 
{
	q[0] = a;
	q[1] = b;
	q[2] = c;
	q[3] = d;
	Normalize();
#ifdef _DEBUG
	m_history.push_back(name);
#endif
}

CQuaternion& CQuaternion::operator *= (const CQuaternion &other) 
{
	*this = operator *(other);
	return *this;
}

CQuaternion CQuaternion::operator * (const CQuaternion &other) const
{
	pVector4d t1, t2, t3;

	pVector4f_SCALE(q, other.q[3], t1);
	pVector4f_SCALE(other.q, q[3], t2);
	pVectorCross3d(other.q, q, t3);

	CQuaternion retQuat( t1[0] + t2[0] + t3[0],
						 t1[1] + t2[1] + t3[1],
						 t1[2] + t2[2] + t3[2],
						 q[3] * other.q[3] - pVectorDot3d(q,other.q));
#ifdef _DEBUG
	retQuat.m_history = m_history;
	copy(other.m_history.begin(), other.m_history.end(), back_inserter(retQuat.m_history));
#endif
	return retQuat;
}

/*================================*\
* Invert v portion of quaternion *
\*================================*/
CQuaternion CQuaternion::GetInverse() const
{
	double lengthSquared = pVectorDot3d(q, q);
	if (lengthSquared == 0.0)
		return CQuaternion();
	CQuaternion retQuat( -q[0] / lengthSquared,
					     -q[1] / lengthSquared,
					   	 -q[2] / lengthSquared,
					   	  q[3] / lengthSquared);
#ifdef _DEBUG
	retQuat.m_history.clear();
	for (std::vector<std::string>::const_iterator it = m_history.begin(); it != m_history.end(); it ++)
	{
		retQuat.m_history.push_back( string("Inverse ") + *it );
	}
	reverse(retQuat.m_history.begin(), retQuat.m_history.end());
#endif
	return retQuat;
}

CQuaternion& CQuaternion::Normalize()
{
	pVectorNormalize4d(q, q);
	return *this;
}

CQuaternion CQuaternion::FromAxis(double theta, pVector3d axis)
{
	return CQuaternion(axis, theta);
}
CQuaternion CQuaternion::FromAxis(double theta, Vector3 axis)
{
	return CQuaternion(axis, theta);
}


void CQuaternion::RotateVector(pVector3d &vin, pVector3d &vout) const
{
	pVector4d t1, t2, t3, t4, t5;
	// vout = s^2
	pVector3f_SCALE(vin,q[3]*q[3],t1);
	// t2 = vin . v
	pVector3f_SCALE(q,pVectorDot3d(vin,q),t2); 
	// t3 = v x vin
	pVectorCross3d(q,vin,t3);
	// t3 = 2*s*t3 
	pVector3f_SCALE(t3,2*q[3],t3);
	// t4 = v x vin
	pVectorCross3d(q,vin,t4);
	// t5 = v x t4 
	pVectorCross3d(q,t4,t5);

	vout[0] = t1[0] + t2[0] + t3[0] + t5[0];
	vout[1] = t1[1] + t2[1] + t3[1] + t5[1];
	vout[2] = t1[2] + t2[2] + t3[2] + t5[2];
}
void CQuaternion::RotateVector(const Vector3& vin, Vector3& vout) const
{
	Vector3 t1, t2, t3, t4, t5;
	Vector3 Q; Q[0] = q[0]; Q[1] = q[1]; Q[2] = q[2];
	// vout = s^2
	//pVector3f_SCALE(vin,q[3]*q[3],t1);
	t1 = vin * q[3] * q[3];
	// t2 = vin . v
	//pVector3f_SCALE(q,pVectorDot3d(vin,q),t2);
	t2 = Q * (Q * vin);	
	// t3 = v x vin
	//pVectorCross3d(q,vin,t3);
	t3 = cross(Q, vin);
	// t3 = 2*s*t3 
	//pVector3f_SCALE(t3,2*q[3],t3);
	t3 *= 2 * q[3];
	// t4 = v x vin
	//pVectorCross3d(q,vin,t4);
	t4 = cross(Q,vin);
	// t5 = v x t4 
	//pVectorCross3d(q,t4,t5);
	t5 = cross(Q, t4);

	vout[0] = t1[0] + t2[0] + t3[0] + t5[0];
	vout[1] = t1[1] + t2[1] + t3[1] + t5[1];
	vout[2] = t1[2] + t2[2] + t3[2] + t5[2];
}