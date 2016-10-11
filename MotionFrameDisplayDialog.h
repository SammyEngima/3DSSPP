#pragma once

#include "resource.h"
#include "MotionFrameDisplay.h"
#include "ViewComboBox.h"
#include "afxwin.h"
#include "afxcmn.h"

// MotionFrameDisplayDialog dialog

class JointAngles;
class Motion;

class MotionFrameDisplayDialog : public CDialog
{
	DECLARE_DYNAMIC(MotionFrameDisplayDialog)

public:
	MotionFrameDisplayDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~MotionFrameDisplayDialog();

	void changeInitialViewType();
	void changeFinalViewType();
	void changePredictedViewType();

	MotionFrameDisplay& getInitialFrameDisplay();
	MotionFrameDisplay& getFinalFrameDisplay();
	MotionFrameDisplay& getPredictedFrameDisplay();

	void setMotion(Motion* motion);
	void setFrame(int frame);

	void update();
	void updateInitial();
	void updateFinal();
	void updatePredicted();

	void enableInitial(bool enable = true);
	void enableFinal(bool enable = true);

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

	HBITMAP mPlayBitmap;
	HBITMAP mRewindBitmap;
	HBITMAP mPauseBitmap;
	HBITMAP mFwdBitmap;
	HBITMAP mBackBitmap;
	HBITMAP mLastBitmap;
	HBITMAP mFirstBitmap;
	HBITMAP mPlayingBitmap;
	HBITMAP mRewindingBitmap;
	HBITMAP mPausedBitmap;

// Dialog Data
	enum { IDD = IDD_MOTIONFRAMEDISPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	int currentFrame;
	enum PlayMode_e {PM_Pause, PM_Play, PM_Rewind} playMode;

	MotionFrameDisplay initialFrameDisplay;
	MotionFrameDisplay finalFrameDisplay;
	MotionFrameDisplay predictedFrameDisplay;

	CViewComboBox initialDisplayType;
	CViewComboBox finalDisplayType;
	CViewComboBox predictedDisplayType;

	Motion* motion;

	virtual BOOL OnInitDialog();

	afx_msg void OnCbnSelchangeInitialviewtype();
	afx_msg void OnCbnSelchangeFinalviewtype();
	afx_msg void OnCbnSelchangePredictedviewtype();
public:
	afx_msg void OnBnClickedMotionpause();
	afx_msg void OnBnClickedMotionrew();
	afx_msg void OnBnClickedMotionprev();
	afx_msg void OnBnClickedMotionfirst();
	afx_msg void OnBnClickedMotionplay();
	afx_msg void OnBnClickedMotionnext();
	afx_msg void OnBnClickedMotionlast();
	CSliderCtrl MotionSlider;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
