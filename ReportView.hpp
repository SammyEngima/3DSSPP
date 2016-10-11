// ReportView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ReportView form view
#ifndef ReportViewIncludeMcr
#define ReportViewIncludeMcr

#ifndef __AFXEXT_H__
#include <afxext.h>
#include <afxcmn.h>

#endif

#include "dibview.h" 
#include "CDib.h"
#include "resource.h"

class DIB;

class ReportView : public CFormView
{
protected:

	ReportView();
	ReportView(UINT aDialogResourceID,
					BOOL aInitializeFrame = TRUE);
	DECLARE_DYNCREATE(ReportView)

// Operations
public:
	virtual void OnInitialUpdate();

	//virtual int OnCreate(LPCREATESTRUCT lpcs);
	virtual void OnPrint( CDC* pDC, CPrintInfo* pInf );

	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void PrintDirect();
	void Close();

	virtual CString ReportName() const;

private:
   CDibView* mDibViewPtr;

   DIB* printDib;

	BOOL mPrintDirect;
   BOOL mInitializeFrame;
   BOOL mPrintMultiple;

// Implementation
protected:
	void initializeGL();
	HDC gl_hDC;
	virtual ~ReportView();
public:
	DECLARE_MESSAGE_MAP()
protected:
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	virtual void UpdateUnits() {};
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileCopyactivewindow();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrint();
};

void SSPPPrintPreview(CView* viewPtr);

#endif
/////////////////////////////////////////////////////////////////////////////
