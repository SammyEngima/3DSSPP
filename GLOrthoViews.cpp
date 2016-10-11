#include "GLOrthoViews.h"

// ******************** Top View ********************

IMPLEMENT_DYNCREATE(CGLTopView, CGLStickView)

CGLTopView::CGLTopView(void) {
	glInteractive = new GLSkeletonTop(this);		
}
CGLTopView::~CGLTopView(void) {
}

// ******************** Side View ********************

IMPLEMENT_DYNCREATE(CGLSideView, CGLStickView)

CGLSideView::CGLSideView(void) {
	glInteractive = new GLSkeletonSide(this);
}
CGLSideView::~CGLSideView(void) {
}
// ******************** Front View ********************

IMPLEMENT_DYNCREATE(CGLFrontView, CGLStickView)

CGLFrontView::CGLFrontView(void) {
	glInteractive = new GLSkeletonFront(this);
}
CGLFrontView::~CGLFrontView(void) {
}