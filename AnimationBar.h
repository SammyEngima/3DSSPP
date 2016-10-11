#ifndef _ANIMATIONBAR_H_
#define _ANIMATIONBAR_H_

#include "stdafx.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "TimelineCtrl.h"
#include "ReportView.hpp"
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "task.h"
#include "InsertFramesBox.h"

enum PlayMode_e { PM_STOPPED, PM_FORWARD, PM_REWIND };

class C_Hom_Doc;

class AnimationBar : public ReportView
{
	DECLARE_DYNCREATE(AnimationBar)

protected:
	AnimationBar();
	virtual ~AnimationBar();

	virtual void OnUpdate(CView*, LPARAM, CObject*);
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
	void UpdateLayout(void);

	#ifdef _DEBUG
	    virtual void AssertValid() const;
	    virtual void Dump(CDumpContext& dc) const;
	#endif

	enum { IDD = IDD_ANIMATIONBAR };

	void insertFrames(int numFrames, bool doInterp);
	void showInsertWindow(CPoint point, int numFrames, bool show);

	void getSelection(int& left, int& right);
	void setSelection(int left, int right);
	void setCurrentFrame(int frame);
	bool isselection();

private:
	C_Hom_Doc* mDocPtr;

	InsertFramesBox* insertBox;

	int playMode;

	int last_numToInsert;

	CTimelineCtrl mTimeline;
	CButton mKeyframeButton;
public:
	CButton mInsertButton;
private:

	HICON mPlayIcon;
	HICON mRewindIcon;
	HICON mPauseIcon;
	HICON mFwdIcon;
	HICON mBackIcon;
	HICON mLastIcon;
	HICON mFirstIcon;
	HICON mPlayingIcon;
	HICON mRewindingIcon;
	HICON mPausedIcon;

	void stop(void);

	void DeleteFrame();
	void AddFrame();
	void InsertFrame();

public:
	afx_msg void OnBnClickedBackbutton();
	afx_msg void OnBnClickedFirstbutton();
	afx_msg void OnBnClickedFwdbutton();
	afx_msg void OnBnClickedLastbutton();
	afx_msg void OnBnClickedPlaybutton();
	afx_msg void OnBnClickedRewindbutton();
	afx_msg void OnBnClickedPausebutton();

	afx_msg void OnBnClickedAddframe();
	afx_msg void OnBnClickedAddmotion();

	afx_msg void OnFilePrintPreview();
	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedInsertframe();
	afx_msg void OnAnimationtoolbarcontextmenuCut();
	afx_msg void OnUpdateAnimationtoolbarcontextmenuCut(CCmdUI *pCmdUI);
	afx_msg void OnAnimationtoolbarcontextmenuCopy();
	afx_msg void OnAnimationtoolbarcontextmenuPaste();
	afx_msg void OnUpdateAnimationtoolbarcontextmenuPaste(CCmdUI *pCmdUI);
	afx_msg void OnAnimationtoolbarcontextmenuRemoveframe();
	afx_msg void OnUpdateAnimationtoolbarcontextmenuRemoveframe(CCmdUI *pCmdUI);
	afx_msg void OnAnimationtoolbarcontextmenuAddframe();
	afx_msg void OnUpdateAnimationtoolbarcontextmenuAddframe(CCmdUI *pCmdUI);
	afx_msg void OnAnimationtoolbarcontextmenuInsertframe();
	afx_msg void OnUpdateAnimationtoolbarcontextmenuInsertframe(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedRadioNiosh();
	afx_msg void OnFilePrint();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#endif
