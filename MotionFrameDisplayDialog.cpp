// MotionFrameDisplayDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MotionFrameDisplayDialog.h"
#include "Hom.h"
#include "Hom_Doc.h"
#include "Motion.h"

#include "JointAngles.hpp"

// MotionFrameDisplayDialog dialog

IMPLEMENT_DYNAMIC(MotionFrameDisplayDialog, CDialog)

MotionFrameDisplayDialog::MotionFrameDisplayDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MotionFrameDisplayDialog::IDD, pParent)
{
	C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
	currentFrame = 0;
	playMode = PM_Pause;
}

MotionFrameDisplayDialog::~MotionFrameDisplayDialog()
{
}

void MotionFrameDisplayDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INITIALFRAMEDISPLAY, initialFrameDisplay);
	DDX_Control(pDX, IDC_FINALFRAMEDISPLAY, finalFrameDisplay);
	DDX_Control(pDX, IDC_PREDICTEDFRAMEDISPLAY, predictedFrameDisplay);
	DDX_Control(pDX, IDC_INITIALVIEWTYPE, initialDisplayType);
	DDX_Control(pDX, IDC_FINALVIEWTYPE, finalDisplayType);
	DDX_Control(pDX, IDC_PREDICTEDVIEWTYPE, predictedDisplayType);
	DDX_Control(pDX, IDC_MOTIONSLIDE, MotionSlider);
}


BEGIN_MESSAGE_MAP(MotionFrameDisplayDialog, CDialog)
	ON_CBN_SELCHANGE(IDC_INITIALVIEWTYPE, &MotionFrameDisplayDialog::OnCbnSelchangeInitialviewtype)
	ON_CBN_SELCHANGE(IDC_FINALVIEWTYPE, &MotionFrameDisplayDialog::OnCbnSelchangeFinalviewtype)
	ON_CBN_SELCHANGE(IDC_PREDICTEDVIEWTYPE, &MotionFrameDisplayDialog::OnCbnSelchangePredictedviewtype)
	ON_BN_CLICKED(IDC_MOTIONPAUSE, &MotionFrameDisplayDialog::OnBnClickedMotionpause)
	ON_BN_CLICKED(IDC_MOTIONREW, &MotionFrameDisplayDialog::OnBnClickedMotionrew)
	ON_BN_CLICKED(IDC_MOTIONPREV, &MotionFrameDisplayDialog::OnBnClickedMotionprev)
	ON_BN_CLICKED(IDC_MOTIONFIRST, &MotionFrameDisplayDialog::OnBnClickedMotionfirst)
	ON_BN_CLICKED(IDC_MOTIONPLAY, &MotionFrameDisplayDialog::OnBnClickedMotionplay)
	ON_BN_CLICKED(IDC_MOTIONNEXT, &MotionFrameDisplayDialog::OnBnClickedMotionnext)
	ON_BN_CLICKED(IDC_MOTIONLAST, &MotionFrameDisplayDialog::OnBnClickedMotionlast)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// MotionFrameDisplayDialog message handlers

BOOL MotionFrameDisplayDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	initialFrameDisplay.initialize();
	finalFrameDisplay.initialize();
	predictedFrameDisplay.initialize();

	initialDisplayType.Initialize();
	finalDisplayType.Initialize();
	predictedDisplayType.Initialize();

	// new icons
	mPlayIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_PLAY_MOTION));
    mPauseIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_PAUSE_MOTION));
    mRewindIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_REWIND_MOTION));
    mFwdIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_NEXT_MOTION));
    mBackIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_BACK_MOTION));
    mLastIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_LAST_MOTION));
    mFirstIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_FIRST_MOTION));
	mPlayingIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_PLAY_ON_MOTION));
	mRewindingIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_REWIND_ON_MOTION));
	mPausedIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_PAUSE_ON_MOTION));

	static_cast<CButton*>(GetDlgItem(IDC_MOTIONFIRST))->SetIcon(mFirstIcon);
	static_cast<CButton*>(GetDlgItem(IDC_MOTIONPREV))->SetIcon(mBackIcon);
	static_cast<CButton*>(GetDlgItem(IDC_MOTIONREW))->SetIcon(mRewindIcon);
	static_cast<CButton*>(GetDlgItem(IDC_MOTIONPAUSE))->SetIcon(mPausedIcon);
	static_cast<CButton*>(GetDlgItem(IDC_MOTIONPLAY))->SetIcon(mPlayIcon);
	static_cast<CButton*>(GetDlgItem(IDC_MOTIONNEXT))->SetIcon(mFwdIcon);
	static_cast<CButton*>(GetDlgItem(IDC_MOTIONLAST))->SetIcon(mLastIcon);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void MotionFrameDisplayDialog::OnCbnSelchangeInitialviewtype()
{
	changeInitialViewType();
	/*if(displayTypeLink.GetCheck() == BST_CHECKED) {
		finalDisplayType.SetCurSel(initialDisplayType.GetCurSel());
		changeFinalViewType();
	}*/
}
void MotionFrameDisplayDialog::changeInitialViewType() {
	switch(initialDisplayType.GetCurSel()) {
		case 0:
			initialFrameDisplay.setView(OBLIQUE_VIEW);
			break;
		case 1:
			initialFrameDisplay.setView(TOP_VIEW);
			break;
		case 2:
			initialFrameDisplay.setView(FRONT_VIEW);
			break;
		case 3:
			initialFrameDisplay.setView(SIDE_VIEW);
			break;
	}
	updateInitial();
	//initialFrameDisplay.DrawItem(0);
	//finalTypeChangedLast = false;
}

void MotionFrameDisplayDialog::OnCbnSelchangeFinalviewtype()
{
	changeFinalViewType();
	/*if(displayTypeLink.GetCheck() == BST_CHECKED) {
		initialDisplayType.SetCurSel(finalDisplayType.GetCurSel());
		changeInitialViewType();
	}*/
}
void MotionFrameDisplayDialog::changeFinalViewType() {
	switch(finalDisplayType.GetCurSel()) {
		case 0:
			finalFrameDisplay.setView(OBLIQUE_VIEW);
			break;
		case 1:
			finalFrameDisplay.setView(TOP_VIEW);
			break;
		case 2:
			finalFrameDisplay.setView(FRONT_VIEW);
			break;
		case 3:
			finalFrameDisplay.setView(SIDE_VIEW);
			break;
	}
	updateFinal();
	//finalFrameDisplay.DrawItem(0);
	//finalTypeChangedLast = true;
}

void MotionFrameDisplayDialog::OnCbnSelchangePredictedviewtype()
{
	changePredictedViewType();
}

void MotionFrameDisplayDialog::changePredictedViewType() {
	switch(predictedDisplayType.GetCurSel()) {
		case 0:
			predictedFrameDisplay.setView(OBLIQUE_VIEW);
			break;
		case 1:
			predictedFrameDisplay.setView(TOP_VIEW);
			break;
		case 2:
			predictedFrameDisplay.setView(FRONT_VIEW);
			break;
		case 3:
			predictedFrameDisplay.setView(SIDE_VIEW);
			break;
	}
	updatePredicted();
}

MotionFrameDisplay& MotionFrameDisplayDialog::getInitialFrameDisplay() {
	return initialFrameDisplay;
}
MotionFrameDisplay& MotionFrameDisplayDialog::getFinalFrameDisplay() {
	return finalFrameDisplay;
}
MotionFrameDisplay& MotionFrameDisplayDialog::getPredictedFrameDisplay() {
	return predictedFrameDisplay;
}

void MotionFrameDisplayDialog::setMotion(Motion* motion_in) {
	motion = motion_in;
	initialFrameDisplay.setSkeleton(motion->GetInitialFrame());
	initialFrameDisplay.setDrawType(DT_Skeleton);
	finalFrameDisplay.setSkeleton(motion->GetFinalFrame());
	finalFrameDisplay.setDrawType(DT_Skeleton);
	if(!(0 <= currentFrame && currentFrame < motion->GetNumFrames()))
		currentFrame = 0;
	predictedFrameDisplay.setSkeleton((*motion)[currentFrame]);
	predictedFrameDisplay.setDrawType(DT_Skeleton);
	MotionSlider.SetRange(0, motion->GetNumFrames() - 1, 1);
	MotionSlider.SetPos(currentFrame);
	MotionSlider.SetPageSize(motion->GetNumFrames() / 4);
	updateInitial();
	updateFinal();
	updatePredicted();
}

void MotionFrameDisplayDialog::setFrame(int frame) {
	currentFrame = frame;
	if(!(0 <= currentFrame && currentFrame < motion->GetNumFrames()))
		currentFrame = 0;
	predictedFrameDisplay.setSkeleton((*motion)[currentFrame]);
	MotionSlider.SetPos(currentFrame);
	updatePredicted();
}

void MotionFrameDisplayDialog::update() {
	updateInitial();
	updateFinal();
	updatePredicted();
}
void MotionFrameDisplayDialog::updateInitial() {
	initialFrameDisplay.DrawItem(0);
}
void MotionFrameDisplayDialog::updateFinal() {
	finalFrameDisplay.DrawItem(0);
}
void MotionFrameDisplayDialog::updatePredicted() {
	predictedFrameDisplay.DrawItem(0);
}

void MotionFrameDisplayDialog::enableInitial(bool enable) {
	initialFrameDisplay.EnableWindow(enable);
	initialFrameDisplay.enableDragging(enable);
}
void MotionFrameDisplayDialog::enableFinal(bool enable) {
	finalFrameDisplay.EnableWindow(enable);
	finalFrameDisplay.enableDragging(enable);
}
void MotionFrameDisplayDialog::OnBnClickedMotionpause()
{
	playMode = PM_Pause;
	static_cast<CButton*>(GetDlgItem(IDC_MOTIONPLAY))->SetIcon(mPlayIcon);
	static_cast<CButton*>(GetDlgItem(IDC_MOTIONREW))->SetIcon(mRewindIcon);
	static_cast<CButton*>(GetDlgItem(IDC_MOTIONPAUSE))->SetIcon(mPausedIcon);
	KillTimer(0);
}

void MotionFrameDisplayDialog::OnBnClickedMotionrew()
{
	OnBnClickedMotionpause();
	playMode = PM_Rewind;
	static_cast<CButton*>(GetDlgItem(IDC_MOTIONPLAY))->SetIcon(mPlayIcon);
	static_cast<CButton*>(GetDlgItem(IDC_MOTIONREW))->SetIcon(mRewindingIcon);
	static_cast<CButton*>(GetDlgItem(IDC_MOTIONPAUSE))->SetIcon(mPauseIcon);
	SetTimer(0, 40, NULL);
}

void MotionFrameDisplayDialog::OnBnClickedMotionprev()
{
	OnBnClickedMotionpause();
	if(currentFrame > 0) {
		setFrame(currentFrame - 1);
	}
}

void MotionFrameDisplayDialog::OnBnClickedMotionfirst()
{
	OnBnClickedMotionpause();
	setFrame(0);
}

void MotionFrameDisplayDialog::OnBnClickedMotionplay()
{
	OnBnClickedMotionpause();
	playMode = PM_Play;

	// change icon
	static_cast<CButton*>(GetDlgItem(IDC_MOTIONPLAY))->SetIcon(mPlayingIcon);
	static_cast<CButton*>(GetDlgItem(IDC_MOTIONREW))->SetIcon(mRewindIcon);
	static_cast<CButton*>(GetDlgItem(IDC_MOTIONPAUSE))->SetIcon(mPauseIcon);


	SetTimer(0, 40, NULL);
}

void MotionFrameDisplayDialog::OnBnClickedMotionnext()
{
	OnBnClickedMotionpause();
	if(currentFrame < motion->GetNumFrames() - 1) {
		setFrame(currentFrame + 1);
	}
}

void MotionFrameDisplayDialog::OnBnClickedMotionlast()
{
	OnBnClickedMotionpause();
	setFrame(motion->GetNumFrames() - 1);
}

void MotionFrameDisplayDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	OnBnClickedMotionpause();
	switch (nSBCode) {
		case SB_LEFT:      // Scroll to far left.
			setFrame(0);
			break;
		case SB_RIGHT:      // Scroll to far right.
			setFrame(motion->GetNumFrames() - 1);
			break;
		case SB_ENDSCROLL:   // End scroll.
			break;
		case SB_LINELEFT:      // Scroll left.
			OnBnClickedMotionprev();
			break;
		case SB_LINERIGHT:   // Scroll right.
			OnBnClickedMotionnext();
			break;
		case SB_PAGELEFT:    // Scroll one page left.
			setFrame(max(0, currentFrame - MotionSlider.GetPageSize()));
			break;
		case SB_PAGERIGHT:      // Scroll one page right.
			setFrame(min(currentFrame + MotionSlider.GetPageSize(), motion->GetNumFrames() - 1));
			break;
		case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
			setFrame(nPos);      // of the scroll box at the end of the drag operation.
			break;
		case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
			setFrame(nPos);     // position that the scroll box has been dragged to.
			break;
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void MotionFrameDisplayDialog::OnTimer(UINT_PTR nIDEvent)
{
	if(playMode == PM_Play) {
		if(currentFrame < motion->GetNumFrames() - 1) {
			setFrame(currentFrame + 1);
		} else {
			playMode = PM_Pause;
		}
	} else if(playMode == PM_Rewind) {
		if(currentFrame > 0) {
			setFrame(currentFrame - 1);
		} else {
			playMode = PM_Pause;
		}
	} else {
		playMode = PM_Pause;
		KillTimer(0);
	}

	if(playMode == PM_Pause) {
		static_cast<CButton*>(GetDlgItem(IDC_MOTIONPLAY))->SetIcon(mPlayIcon);
		static_cast<CButton*>(GetDlgItem(IDC_MOTIONREW))->SetIcon(mRewindIcon);
		static_cast<CButton*>(GetDlgItem(IDC_MOTIONPAUSE))->SetIcon(mPausedIcon);
	}

	CDialog::OnTimer(nIDEvent);
}
