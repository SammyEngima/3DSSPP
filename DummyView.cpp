// DummyView.cpp : implementation file
//

#include "stdafx.h"
#include "DummyView.h"
#include "Hom.h"
#include "Hom_Doc.h"
#include "Main_Frm.h"


// CDummyView

IMPLEMENT_DYNCREATE(CDummyView, CView)

CDummyView::CDummyView()
{

}

CDummyView::~CDummyView()
{
}

BEGIN_MESSAGE_MAP(CDummyView, CView)
END_MESSAGE_MAP()


// CDummyView drawing

void CDummyView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CDummyView diagnostics

#ifdef _DEBUG
void CDummyView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CDummyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

void CDummyView::FakeOnFilePrint() {
	CView::OnFilePrint();
}

// CDummyView message handlers

void CDummyView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{

	Get_App_Ptr()->GetMainFramePtr()->OnPrint(pDC, pInfo);

	//CView::OnPrint(pDC, pInfo);
}

BOOL CDummyView::OnPreparePrinting(CPrintInfo* pInfo)
{

	DoPreparePrinting(pInfo);

	return CView::OnPreparePrinting(pInfo);
}
