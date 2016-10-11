
#include "GLStickView.h"
#include "Hom_Doc.h"
#include "GLSkeleton.h"
#include "GLOblique.h"

IMPLEMENT_DYNCREATE(CGLStickView, CGLView)

CGLStickView::CGLStickView(void)
{
}
void CGLStickView::OnUpdate(CView* pSender, LPARAM lHint, CObject *pHint) {
	if(glInteractive) {
		GLSkeleton* glSkeleton = (GLSkeleton*)glInteractive;
		glSkeleton->updateCue();
		//glSkeleton->setLocking(GetDocument()->GetLockingMode());
		glSkeleton->onDraw();
	}
}

CGLStickView::~CGLStickView(void) {
}