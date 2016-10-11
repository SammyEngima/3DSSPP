#pragma once
#include "glskeleton.h"

///Yijun
#include "glinteractive.h"
#include "glHom/model_mdp.h"
#include "glHom/mdp_main.h"
#include "enums.h"
#include "afxwin.h"

#include <map>
#include <list>
#include <string>
#include <vector>



class GLSkeletonTop : public GLSkeleton {
public:
	GLSkeletonTop(CWnd* owner);
	virtual ~GLSkeletonTop(void);

private:
	virtual void setCamera(const Vector3& ankleCenter);
	virtual void drawForceTip(const Vector3 &tip,const Vector3 &handle);
	virtual void drawFloor(void);
	virtual CPoint world_to_screen(const Vector3 &point,const CRect &client, const Vector3& ankleCenter);
	virtual Vector3 world_to_screen_exact(const Vector3 &point, const CRect &client, const Vector3& ankleCenter);
	virtual Vector3 screen_to_world(const CPoint &point,const Vector3 &ref,const CRect &client, const Vector3& ankleCenter);
	virtual JointID getFrontmostOf(std::set<JointID> hits, bool force);

	virtual double HowTransparent(double angle, AngleType AT);
	virtual void OrderJoints();
};

class GLSkeletonFront : public GLSkeleton {
public:
	GLSkeletonFront(CWnd* owner);
	virtual ~GLSkeletonFront(void);

private:
	virtual void setCamera(const Vector3& ankleCenter);
	virtual void drawForceTip(const Vector3 &tip,const Vector3 &handle);
	virtual void drawFloor(void);
	virtual CPoint world_to_screen(const Vector3 &point,const CRect &client, const Vector3& ankleCenter);
	virtual Vector3 world_to_screen_exact(const Vector3 &point, const CRect &client, const Vector3& ankleCenter);
	virtual Vector3 screen_to_world(const CPoint &point,const Vector3 &ref,const CRect &client, const Vector3& ankleCenter);
	virtual JointID getFrontmostOf(std::set<JointID> hits, bool force);

	virtual double HowTransparent(double angle, AngleType AT);
	virtual void OrderJoints();
};

class GLSkeletonSide : public GLSkeleton {
public:
	GLSkeletonSide(CWnd* owner);
	virtual ~GLSkeletonSide(void);

private:
	virtual void setCamera(const Vector3& ankleCenter);
	virtual void drawForceTip(const Vector3 &tip,const Vector3 &handle);
	virtual void drawFloor(void);
	virtual CPoint world_to_screen(const Vector3 &point,const CRect &client, const Vector3& ankleCenter);
	virtual Vector3 world_to_screen_exact(const Vector3 &point, const CRect &client, const Vector3& ankleCenter);
	virtual Vector3 screen_to_world(const CPoint &point,const Vector3 &ref,const CRect &client, const Vector3& ankleCenter);
	virtual JointID getFrontmostOf(std::set<JointID> hits, bool force);

	virtual double HowTransparent(double angle, AngleType AT);
	virtual void OrderJoints();

};
