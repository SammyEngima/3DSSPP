#pragma once

#include "GLOblique.h"
#include "GLTopSideFront.h"
// MotionFrameDisplay

#define DISPLAY_WIDTH 230
#define DISPLAY_HEIGHT 225

class MotionFrameDisplay : public CButton
{
	DECLARE_DYNAMIC(MotionFrameDisplay)

public:
	MotionFrameDisplay();
	virtual ~MotionFrameDisplay();

	void initialize();

	GLInteractive* getView();
	void setView(int viewType);

	void setDrawType(DrawType_e type);
	void setSkeleton(Skeleton* skel);
	void setFrame(int frame);
	void setDrawTarget(const Vector3& targetLocation);
	void setNoDrawTarget();

	void enableDragging(bool enable = true);

protected:
	DECLARE_MESSAGE_MAP()

	GLInteractive* glCurrent;

	GLOblique glOblique;
	GLSkeletonTop glTop;
	GLSkeletonSide glSide;
	GLSkeletonFront glFront;

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


