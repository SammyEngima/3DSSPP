#pragma once


// CDummyView view

class CDummyView : public CView
{
	DECLARE_DYNCREATE(CDummyView)

protected:
	CDummyView();           // protected constructor used by dynamic creation
	virtual ~CDummyView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	void FakeOnFilePrint();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
};


