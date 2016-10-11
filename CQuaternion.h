#ifndef CQUATERNION_H
#define CQUATERNION_H

#include <vector>
#include <string>
#include "Vector3.h"
#include "glHom/math_vector.h"

class CQuaternion
{
public:
	CQuaternion(perihelion::pVector4d quat, std::string name="no-name");
	CQuaternion(perihelion::pVector4f quat, std::string name="no-name");
	CQuaternion(perihelion::pVector3d axis, double theta, std::string name="no-name");
	CQuaternion(Vector3 axis, double theta, std::string name="no-name");
	CQuaternion(double a=0.0, double b=0.0, double c=0.0, double d=1.0, std::string name="no-name");

	CQuaternion& operator *= (const CQuaternion &other);
	CQuaternion operator * (const CQuaternion &other) const;
	CQuaternion GetInverse() const;
	CQuaternion& Normalize();

	static CQuaternion FromAxis(double theta, perihelion::pVector3d axis);
	static CQuaternion FromAxis(double theta, Vector3 axis);

	void RotateVector(perihelion::pVector3d &vin, perihelion::pVector3d &vout) const;
	void RotateVector(const Vector3& vin, Vector3& vout) const;

public:
	double q[4];
#ifdef _DEBUG
	std::vector<std::string>     m_history;
#endif
};

#endif