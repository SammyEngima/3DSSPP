#pragma once

#include "stdafx.h"

class C_Hom_Doc;
class GLInteractive;
class CDibView;
struct AngleCueData;

class DIB;

class CGLView : public CView
{
	protected:
	    CGLView();
        
        DECLARE_DYNCREATE(CGLView)
        DECLARE_MESSAGE_MAP()

        C_Hom_Doc* GetDocument(void) { return (C_Hom_Doc*)m_pDocument; }

        virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
        virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
        virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
        virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
		void PrePrint(CDC* pDC, CPrintInfo* pInfo);
		void PostPrint(CDC* pDC);
        afx_msg void OnFilePrintPreview(void);

    public:
        virtual ~CGLView();
	    virtual void OnInitialUpdate();
        virtual void OnDraw(CDC* pDC);

		virtual void cueAngle(const AngleCueData& ACD);
		virtual void cancelCue();

		GLInteractive* getGLInteractive();

        #ifdef _DEBUG
	        virtual void AssertValid() const;
	        virtual void Dump(CDumpContext& dc) const;
        #endif

        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);// { createContextMenu(point); }

        // redirect windows message handlers to virtual functions declared above; attempt at abstracting
        // away windows ugliness
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);// { handleMouseMove(nFlags, point); }
        virtual afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

		afx_msg LRESULT OnDrawOpenGL(WPARAM wparam, LPARAM lparam);

    protected:
		GLInteractive* glInteractive;

        CDibView* mDibViewPtr;

		// Indicates that this view is specifically for printing
		bool printView;

		DIB* printDib;

		// printing stuff
		HDC m_hOldDC;
		HDC m_hMemDC;
		HGLRC m_hOldRC;
		HGLRC m_hMemRC;
		BITMAPINFO m_bmi;
		LPVOID m_pBitmapBits;
		HBITMAP m_hDib;
		CSize m_szPage;
		CRect m_savedViewport;
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnFileCopyactivewindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
public:
	afx_msg void OnFilePrint();
};


