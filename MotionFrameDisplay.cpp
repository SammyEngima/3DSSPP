// MotionFrameDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "MotionFrameDisplay.h"
#include "GLInteractive.h"
#include "Hom.h"
#include "Hom_Doc.h"

// MotionFrameDisplay

IMPLEMENT_DYNAMIC(MotionFrameDisplay, CStatic)

MotionFrameDisplay::MotionFrameDisplay() :
glOblique(this),
glTop(this),
glSide(this),
glFront(this)
{
	glCurrent = &glOblique;
}

MotionFrameDisplay::~MotionFrameDisplay()
{
}

void MotionFrameDisplay::initialize() {
	CRect rect;
	this->GetClientRect(&rect);
	glOblique.initialize();
	glOblique.setViewport(rect);
	glTop.share(&glOblique);
	glTop.setViewport(rect);
	glSide.share(&glOblique);
	glSide.setViewport(rect);
	glFront.share(&glOblique);
	glFront.setViewport(rect);

}

GLInteractive* MotionFrameDisplay::getView() {
	return glCurrent;
}
void MotionFrameDisplay::setView(int viewType) {
	switch(viewType) {
		case OBLIQUE_VIEW:
			glCurrent = &glOblique;
			glOblique.updateMDP();
			break;
		case TOP_VIEW:
			glCurrent = &glTop;
			break;
		case SIDE_VIEW:
			glCurrent = &glSide;
			break;
		case FRONT_VIEW:
			glCurrent = &glFront;
			break;
	}
}

void MotionFrameDisplay::setDrawType(DrawType_e type) {
	glOblique.setDrawType(type);
	glOblique.updateMDP();
	glTop.setDrawType(type);
	glFront.setDrawType(type);
	glSide.setDrawType(type);
}
void MotionFrameDisplay::setSkeleton(Skeleton* skel) {
	glOblique.setSkeleton(skel);
	glOblique.updateMDP();
	glTop.setSkeleton(skel);
	glFront.setSkeleton(skel);
	glSide.setSkeleton(skel);
}
void MotionFrameDisplay::setFrame(int frame) {
	glOblique.setFrame(frame);
	glOblique.updateMDP();
	glTop.setFrame(frame);
	glFront.setFrame(frame);
	glSide.setFrame(frame);
}

void MotionFrameDisplay::setDrawTarget(const Vector3& targetLocation) {
	glOblique.setDrawTarget(targetLocation);
	glTop.setDrawTarget(targetLocation);
	glSide.setDrawTarget(targetLocation);
	glFront.setDrawTarget(targetLocation);
}
void MotionFrameDisplay::setNoDrawTarget() {
	glOblique.setNoDrawTarget();
	glTop.setNoDrawTarget();
	glSide.setNoDrawTarget();
	glFront.setNoDrawTarget();
}

void MotionFrameDisplay::enableDragging(bool enable) {
	if(enable) {
		LockingMode_e LM = Get_App_Ptr()->Get_Document_Ptr()->GetLockingMode();
		glTop.setLocking(LM);
		glSide.setLocking(LM);
		glFront.setLocking(LM);
	} else {
		glTop.setLocking(LM_All);
		glSide.setLocking(LM_All);
		glFront.setLocking(LM_All);
	}
}

BEGIN_MESSAGE_MAP(MotionFrameDisplay, CStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// MotionFrameDisplay message handlers



void MotionFrameDisplay::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(glCurrent)
		glCurrent->onDraw();
}

void MotionFrameDisplay::OnMouseMove(UINT nFlags, CPoint point)
{
	if(glCurrent) {
		glCurrent->handleMouseMove(nFlags, point);
		glCurrent->onDraw();
	}
	CButton::OnMouseMove(nFlags, point);
}

BOOL MotionFrameDisplay::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(glCurrent) {
		glCurrent->handleMouseWheel(nFlags, zDelta, pt);
		glCurrent->onDraw();
	}
	return CButton::OnMouseWheel(nFlags, zDelta, pt);
}

void MotionFrameDisplay::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(glCurrent && glCurrent == &glOblique) {
		glCurrent->handleLButtonDown(nFlags, point);
		glCurrent->onDraw();
	}
	CButton::OnLButtonDown(nFlags, point);
}

void MotionFrameDisplay::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(glCurrent && glCurrent == &glOblique) {
		glCurrent->handleLButtonUp(nFlags, point);
		glCurrent->onDraw();
	}
	CButton::OnLButtonUp(nFlags, point);
}

void MotionFrameDisplay::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(glCurrent) {
		glCurrent->handleRButtonDown(nFlags, point);
		glCurrent->onDraw();
	}
	CButton::OnRButtonDown(nFlags, point);
}

void MotionFrameDisplay::OnRButtonUp(UINT nFlags, CPoint point)
{
	if(glCurrent) {
		glCurrent->handleRButtonUp(nFlags, point);
		glCurrent->onDraw();
	}
	CButton::OnRButtonUp(nFlags, point);
}

void MotionFrameDisplay::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if(glCurrent) {
		//glCurrent->createContextMenu(point);
	}
}
