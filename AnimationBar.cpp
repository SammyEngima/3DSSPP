#include "stdafx.h"
#include "hom_doc.h"
#include "skeleton.hpp"
#include "UndoKeyframe.h"
#include "Dg_MotionPrediction.h"
#include "TimelineCtrl.h"
#include "AnimationBar.h"
#include ".\animationbar.h"
#include "GLView.h"
#include "Analysis.h"
#include "task.h"
#include "LinearMotion.h"
#include "TestTimer.h"
#include <sstream>
#include "UndoKeyframe.h"
using std::ostringstream;

IMPLEMENT_DYNCREATE(AnimationBar, ReportView)

AnimationBar::AnimationBar() : ReportView(AnimationBar::IDD,FALSE)
{
    playMode = PM_STOPPED;
	insertBox = 0;
	last_numToInsert = -1;
}

AnimationBar::~AnimationBar() {
	delete insertBox;
}


void AnimationBar::OnUpdate(CView*, LPARAM, CObject*) {
	int currentFrame = mDocPtr->getCurrentFrame();
    mTimeline.setCurrentFrame(currentFrame);

	if(mDocPtr->getFrameType(currentFrame) == FT_Unset &&
		!mTimeline.hasSelection())
		mKeyframeButton.SetWindowText("Add Frame");
    else
		mKeyframeButton.SetWindowText("Remove Frame");

	bool hasSelection = mTimeline.hasSelection();
	int left, right;
	mTimeline.getSelection(left, right);
	// for the add/remove keyframe button to be enabled,
	// the animation must be stopped, the current frame must not be 0,
	// and if there is a selection, frame 0 can't be in it
	bool keyframeButtonEnable = ( (playMode == PM_STOPPED) &&
								(!hasSelection || (left >= 0)));
	// if the keyframe button has focus and becomes disabled, keyboard input is ignored,
	// so set the focus to something else if the button gets disabled
	if(mKeyframeButton.GetFocus() == &mKeyframeButton && !keyframeButtonEnable) {
		mTimeline.SetFocus();
	}
    mKeyframeButton.EnableWindow(keyframeButtonEnable);

//uable to delete first frame when second frame is unset
	if(currentFrame ==0 && mDocPtr->getFrameType(currentFrame + 1) == FT_Unset)
	{
		mKeyframeButton.EnableWindow(false);
	}
	int leftSelect,rightSelect;
	getSelection(leftSelect,rightSelect);
	if(leftSelect == 0 && mDocPtr->getFrameType(rightSelect+1) == FT_Unset)
	{
		mKeyframeButton.EnableWindow(false);
	}
	// TODO do the same thing for the insert button?

	int numToInsert = right - left + 1;
	if(numToInsert > 1) {
		if(last_numToInsert <= 1) {
			mInsertButton.SetWindowText("Insert Frames");
		}
	} else {
		if(last_numToInsert != 1) {
			mInsertButton.SetWindowText("Insert Frame");
		}
	}
	last_numToInsert = numToInsert;
	mInsertButton.EnableWindow(mDocPtr->getFrameType(left - 1) != FT_Unset || left == 0);

	switch(playMode) {
		case PM_FORWARD:
			static_cast<CButton*>(GetDlgItem(IDC_PLAYBUTTON))->SetIcon(mPlayingIcon);
			static_cast<CButton*>(GetDlgItem(IDC_REWINDBUTTON))->SetIcon(mRewindIcon);
			static_cast<CButton*>(GetDlgItem(IDC_PAUSEBUTTON))->SetIcon(mPauseIcon);
			break;
		case PM_STOPPED:
			static_cast<CButton*>(GetDlgItem(IDC_PLAYBUTTON))->SetIcon(mPlayIcon);
			static_cast<CButton*>(GetDlgItem(IDC_REWINDBUTTON))->SetIcon(mRewindIcon);
			static_cast<CButton*>(GetDlgItem(IDC_PAUSEBUTTON))->SetIcon(mPausedIcon);
			break;
		case PM_REWIND:
			static_cast<CButton*>(GetDlgItem(IDC_PLAYBUTTON))->SetIcon(mPlayIcon);
			static_cast<CButton*>(GetDlgItem(IDC_REWINDBUTTON))->SetIcon(mRewindingIcon);
			static_cast<CButton*>(GetDlgItem(IDC_PAUSEBUTTON))->SetIcon(mPauseIcon);
			break;
	}
	mTimeline.RedrawWindow();
}

void AnimationBar::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TIMELINE,mTimeline);
	DDX_Control(pDX, IDC_ADDFRAME, mKeyframeButton);
	DDX_Control(pDX, IDC_INSERTFRAME, mInsertButton);
}

BEGIN_MESSAGE_MAP(AnimationBar, ReportView)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
    //ON_WM_CTLCOLOR()
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BACKBUTTON, OnBnClickedBackbutton)
    ON_BN_CLICKED(IDC_FIRSTBUTTON, OnBnClickedFirstbutton)
    ON_BN_CLICKED(IDC_FWDBUTTON, OnBnClickedFwdbutton)
    ON_BN_CLICKED(IDC_LASTBUTTON, OnBnClickedLastbutton)
    ON_BN_CLICKED(IDC_PLAYBUTTON, OnBnClickedPlaybutton)
    ON_BN_CLICKED(IDC_REWINDBUTTON, OnBnClickedRewindbutton)
    ON_BN_CLICKED(IDC_PAUSEBUTTON, OnBnClickedPausebutton)
    ON_BN_CLICKED(IDC_ADDFRAME, OnBnClickedAddframe)
    ON_WM_TIMER()
    ON_WM_CONTEXTMENU()
    ON_BN_CLICKED(IDC_ADDMOTION, OnBnClickedAddmotion)
	ON_BN_CLICKED(IDC_INSERTFRAME, &AnimationBar::OnBnClickedInsertframe)
	ON_COMMAND(ID_ANIMATIONTOOLBARCONTEXTMENU_CUT, &AnimationBar::OnAnimationtoolbarcontextmenuCut)
	ON_UPDATE_COMMAND_UI(ID_ANIMATIONTOOLBARCONTEXTMENU_CUT, &AnimationBar::OnUpdateAnimationtoolbarcontextmenuCut)
	ON_COMMAND(ID_ANIMATIONTOOLBARCONTEXTMENU_COPY, &AnimationBar::OnAnimationtoolbarcontextmenuCopy)
	ON_COMMAND(ID_ANIMATIONTOOLBARCONTEXTMENU_PASTE, &AnimationBar::OnAnimationtoolbarcontextmenuPaste)
	ON_UPDATE_COMMAND_UI(ID_ANIMATIONTOOLBARCONTEXTMENU_PASTE, &AnimationBar::OnUpdateAnimationtoolbarcontextmenuPaste)
	ON_COMMAND(ID_ANIMATIONTOOLBARCONTEXTMENU_REMOVEFRAME, &AnimationBar::OnAnimationtoolbarcontextmenuRemoveframe)
	ON_UPDATE_COMMAND_UI(ID_ANIMATIONTOOLBARCONTEXTMENU_REMOVEFRAME, &AnimationBar::OnUpdateAnimationtoolbarcontextmenuRemoveframe)
	ON_COMMAND(ID_ANIMATIONTOOLBARCONTEXTMENU_ADDFRAME, &AnimationBar::OnAnimationtoolbarcontextmenuAddframe)
	ON_UPDATE_COMMAND_UI(ID_ANIMATIONTOOLBARCONTEXTMENU_ADDFRAME, &AnimationBar::OnUpdateAnimationtoolbarcontextmenuAddframe)
	ON_COMMAND(ID_ANIMATIONTOOLBARCONTEXTMENU_INSERTFRAME, &AnimationBar::OnAnimationtoolbarcontextmenuInsertframe)
	ON_UPDATE_COMMAND_UI(ID_ANIMATIONTOOLBARCONTEXTMENU_INSERTFRAME, &AnimationBar::OnUpdateAnimationtoolbarcontextmenuInsertframe)
	ON_COMMAND(ID_FILE_PRINT, &AnimationBar::OnFilePrint)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


void AnimationBar::UpdateLayout(void)
{
    // only update the layout if controls actually exist
    if(IsWindow(mTimeline.m_hWnd) && IsWindow(mKeyframeButton.m_hWnd))
    {
        CRect windowRect,timelineRect,keyframeRect,insertRect;

        GetClientRect(&windowRect);
        mTimeline.GetWindowRect(&timelineRect);
        ScreenToClient(&timelineRect);
        mKeyframeButton.GetWindowRect(&keyframeRect);
        ScreenToClient(&keyframeRect);
		mInsertButton.GetWindowRect(&insertRect);
		ScreenToClient(&insertRect);

        keyframeRect.left = windowRect.Width() - 10 - keyframeRect.Width();
        keyframeRect.right = windowRect.Width() - 10;
        timelineRect.right = windowRect.Width() - 10 - keyframeRect.Width() - 15;
		insertRect.left = windowRect.Width() - 10 - insertRect.Width();
		insertRect.right = windowRect.Width() - 10;
 
        mTimeline.MoveWindow(&timelineRect,TRUE);
        mKeyframeButton.MoveWindow(&keyframeRect,TRUE);
		mInsertButton.MoveWindow(&insertRect,TRUE);
    }
}

#ifdef _DEBUG
void AnimationBar::AssertValid() const
{
	CFormView::AssertValid();
}

void AnimationBar::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


void AnimationBar::insertFrames(int numFrames, bool doInterp) {
	int firstFrame = mTimeline.getHandleFrame() - 1;
	mDocPtr->getTask().insertFrame(numFrames, firstFrame);
	mDocPtr->changeFrame();
	mTimeline.onCloseInsertWindow();

	// make undo event for frame insert
	std::vector<Frame> undoFrames(numFrames);
	for(int i = 0; i < numFrames; i++) {
		undoFrames[i].skeleton = 0;
		undoFrames[i].frameType = FT_Unset;
	}
	UndoableKeyframeInsert* undoEvent = new UndoableKeyframeInsert(firstFrame + 1, numFrames, undoFrames);
	mDocPtr->addUndoEvent(undoEvent);
}

void AnimationBar::showInsertWindow(CPoint point, int numFrames, bool show) {
	RECT rect;
	int x, y;
	mTimeline.GetWindowRect(&rect);
	// redraw things that were covered by the control
	this->RedrawWindow(&rect);//, 0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
	mDocPtr->GLViews[VIEW_OBLIQUE]->GetParentFrame()->UpdateWindow();
	x = rect.left + point.x - 50;
	y = rect.top;
	insertBox->GetClientRect(&rect);
	y -= rect.bottom;
	//insertBox->SetWindowPos(&wndTop, 10, 10, 100, 100, SWP_SHOWWINDOW);
	if(show) {
		insertBox->SetWindowPos(&wndTop, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
	} else {
		insertBox->SetWindowPos(&wndTop, x, y, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);
	}
	insertBox->setNumFrames(numFrames);
}

void AnimationBar::getSelection(int& left, int& right) {
	mTimeline.getSelection(left, right);
}
void AnimationBar::setSelection(int left, int right) {
	mTimeline.setSelection(left, right);
}
void AnimationBar::setCurrentFrame(int frame) {
	mTimeline.setCurrentFrame(frame);
}

void AnimationBar::stop(void)
{
    if(playMode != PM_STOPPED)
    {
        playMode = PM_STOPPED;
        KillTimer(1);
        mTimeline.setMouseEnabled(true);
        mKeyframeButton.EnableWindow(mDocPtr->getCurrentFrame());
    }
}

void AnimationBar::DeleteFrame() {

	int leftSelect,rightSelect;
	getSelection(leftSelect,rightSelect);
	// TODO testing new undo for deleting frames
	/*for(int i = leftSelect; i <= rightSelect; ++i) {
		//mDocPtr->setFrameType(FT_Unset,i);
		// keep deleting leftSelect, cuz the rest slide over
		mDocPtr->getTask().deleteFrame(leftSelect);
	}*/

	std::vector<Frame> frames = mDocPtr->getTask().extractFrames(rightSelect - leftSelect + 1, leftSelect);
	UndoableKeyframeRemove* undo = new UndoableKeyframeRemove(leftSelect, rightSelect - leftSelect + 1, frames);
	mDocPtr->addUndoEvent(undo);
	// this kind of logic should never occur BUT:
	// read the current frame from the doc, which will get it ultimately from the framesequence where it was changed,
	// then set it to the same frame so that the timeline will update
	int newCurrentFrame = mDocPtr->getCurrentFrame();
	mDocPtr->setCurrentFrame(newCurrentFrame);
	mDocPtr->setSelection(newCurrentFrame, newCurrentFrame);
	/*mDocPtr->setCurrentFrame(leftSelect);
	mDocPtr->setSelection(leftSelect, leftSelect);*/

    mDocPtr->MakeDirtyAndUpdateViews(TRUE);
}
bool AnimationBar::isselection()
{
	return mTimeline.hasSelection();
}
void AnimationBar::AddFrame() {
	UndoableKeyframeAdd *undoEvent = new UndoableKeyframeAdd(mDocPtr->getCurrentFrame());
	mDocPtr->addUndoEvent(undoEvent);
	mDocPtr->setFrameType(FT_Keyframe);

    mDocPtr->MakeDirtyAndUpdateViews(TRUE);
}
void AnimationBar::InsertFrame() {
	Analysis analysis;
	int left, right;
	mTimeline.getSelection(left, right);
	int numToInsert = right - left + 1;
	mDocPtr->getTask().insertFrame(numToInsert, left - 1);
	std::vector<Frame> undoFrames(numToInsert);
	for(int i = 0; i < numToInsert; i++) {
		Skeleton* newSkel = new Skeleton(&mDocPtr->getTask());
		if(left != 0) {
			*newSkel = *mDocPtr->getSkeletonAtFrame(left - 1);
		} else {
			analysis.Analyze(newSkel);
		}
		mDocPtr->setFrame(left + i, newSkel, FT_Keyframe);
		undoFrames[i].skeleton = newSkel;
		undoFrames[i].frameType = FT_Keyframe;
	}

	// make undo event
	UndoableKeyframeInsert* undoEvent = new UndoableKeyframeInsert(left, numToInsert, undoFrames);
	mDocPtr->addUndoEvent(undoEvent);

	mDocPtr->changeFrame();
	mDocPtr->MakeDirtyAndUpdateViews();
}

void AnimationBar::OnBnClickedBackbutton()
{
    stop();
    if(mDocPtr->getCurrentFrame() > 0)
    {
		//TRACE("going back one frame\n");
        //skeleton->previousFrame();
		mDocPtr->previousFrame();
		int currentFrame = mDocPtr->getCurrentFrame();
		Get_App_Ptr()->Get_Document_Ptr()->setCurrentFrame(currentFrame);
        mDocPtr->MakeDirtyAndUpdateViews(TRUE);
    }
    mTimeline.makeCurrentFrameVisible();
}

void AnimationBar::OnBnClickedFirstbutton()
{
    stop();
    //if(skeleton->setCurrentFrame(0))
	mDocPtr->setCurrentFrame(0);
	mDocPtr->MakeDirtyAndUpdateViews(TRUE);
    mTimeline.makeCurrentFrameVisible();
}

void AnimationBar::OnBnClickedFwdbutton()
{
    stop();
    //skeleton->nextFrame();
	mDocPtr->nextFrame();
	int currentFrame = mDocPtr->getCurrentFrame();
	Get_App_Ptr()->Get_Document_Ptr()->setCurrentFrame(currentFrame);
    mDocPtr->MakeDirtyAndUpdateViews(TRUE);
    mTimeline.makeCurrentFrameVisible();
}

void AnimationBar::OnBnClickedLastbutton()
{
    stop();
    //if(skeleton->setCurrentFrame(skeleton->getLastFrame()))
	mDocPtr->setCurrentFrame(mDocPtr->getLastFrame());
	mDocPtr->MakeDirtyAndUpdateViews(true);
    mTimeline.makeCurrentFrameVisible();
}

void AnimationBar::OnBnClickedPlaybutton()
{
    if(playMode != PM_FORWARD && mDocPtr->getCurrentFrame() < mDocPtr->getLastFrame())
    {
        stop();
        playMode = PM_FORWARD;
		SetTimer(1,40,NULL);
        mTimeline.setMouseEnabled(false);
	} else if(playMode == PM_STOPPED && mDocPtr->getCurrentFrame() == mDocPtr->getLastFrame()) {
		stop();
		mDocPtr->setCurrentFrame(0);
		playMode = PM_FORWARD;
		SetTimer(1,40,NULL);
		mTimeline.setMouseEnabled(false);
	}
	mDocPtr->UpdateAllViews(NULL);
}

void AnimationBar::OnBnClickedRewindbutton()
{
    if(playMode != PM_REWIND && mDocPtr->getCurrentFrame() != 0)
    {
        stop();
        playMode = PM_REWIND;
		SetTimer(1,40,NULL);
        mTimeline.setMouseEnabled(false);
	} else if(playMode == PM_STOPPED && mDocPtr->getCurrentFrame() == 0) {
		stop();
		mDocPtr->setCurrentFrame(mDocPtr->getLastFrame());
		playMode = PM_REWIND;
		SetTimer(1,40,NULL);
		mTimeline.setMouseEnabled(false);
	}
	mDocPtr->UpdateAllViews(NULL);
}
//bool timer_on = false;
void AnimationBar::OnBnClickedPausebutton() {
	/*if(!timer_on) {
		this->SetTimer(2,40, NULL);
		timer_on = true;
	} else {
		timer_on = false;
		this->KillTimer(2);
	}*/
	stop();
	mDocPtr->UpdateAllViews(NULL);
}

void AnimationBar::OnBnClickedAddframe()
{
	if(mDocPtr->getFrameType(mDocPtr->getCurrentFrame()) == FT_Unset && !mTimeline.hasSelection()) {
		// add
		AddFrame();
	} else {
		DeleteFrame();
	}
}

void AnimationBar::OnBnClickedAddmotion()
{
	Task& task = mDocPtr->getTask();
	int first, last;
	mTimeline.getSelection(first, last);
	Dg_MotionPrediction diag(first, this);
	diag.DoModal();
}


// disable printing the animation bar
void AnimationBar::OnFilePrintPreview()
{
	// NOOP

    //SSPPPrintPreview(this);
}
void AnimationBar::OnFilePrint()
{
	// NOOP
}


// cjb this had no effect
/*HBRUSH AnimationBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    C_Hom_Doc* lDocPtr = static_cast<C_Hom_Doc*>(GetDocument());

    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
    pDC->SetBkColor(lDocPtr->getBalColor());

    HBRUSH hbr = (CBrush(lDocPtr->getBalColor()));
    return hbr;
	//return ReportView::OnCtlColor(pDC, pWnd, nCtlColor);
}*/

void AnimationBar::OnSize(UINT nType, int cx, int cy)
{
    ReportView::OnSize(nType, cx, cy);

    UpdateLayout();
}

void AnimationBar::OnContextMenu(CWnd* pWnd, CPoint point)
{
    CMenu menu;
    VERIFY(menu.LoadMenu(IDR_Context_Animation));
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x,point.y,AfxGetMainWnd());
}

void AnimationBar::OnInitialUpdate()
{
	mDocPtr = Get_App_Ptr()->Get_Document_Ptr();

    ReportView::OnInitialUpdate();
    UpdateLayout();

	// new icons
	mPlayIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_PLAY));
    mPauseIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_PAUSE));
    mRewindIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_REWIND));
    mFwdIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_FWD));
    mBackIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_BACK));
    mLastIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_LAST));
    mFirstIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_FIRST));
	mPlayingIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_PLAY_ON));
	mRewindingIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_REWIND_ON));
	mPausedIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_PAUSE_ON));
    
    static_cast<CButton*>(GetDlgItem(IDC_PLAYBUTTON))->SetIcon(mPlayIcon);
    static_cast<CButton*>(GetDlgItem(IDC_PAUSEBUTTON))->SetIcon(mPausedIcon);
    static_cast<CButton*>(GetDlgItem(IDC_REWINDBUTTON))->SetIcon(mRewindIcon);
    static_cast<CButton*>(GetDlgItem(IDC_FWDBUTTON))->SetIcon(mFwdIcon);
    static_cast<CButton*>(GetDlgItem(IDC_BACKBUTTON))->SetIcon(mBackIcon);
    static_cast<CButton*>(GetDlgItem(IDC_FIRSTBUTTON))->SetIcon(mFirstIcon);
    static_cast<CButton*>(GetDlgItem(IDC_LASTBUTTON))->SetIcon(mLastIcon);

	insertBox = new InsertFramesBox(this);
	insertBox->Create(InsertFramesBox::IDD, this);
	insertBox->ShowWindow(SW_HIDE);

	int curFrame = mDocPtr->getCurrentFrame();
	mTimeline.setSelection(curFrame, curFrame);
}

void AnimationBar::OnTimer(UINT nIDEvent)
{
	 // uncomment the following to see the actual time between timer events on the Motion Button
	//if(nIDEvent == 2) {
		/*static TestTimer timerx;
		double elapsed = timerx.SecondsSinceLast();
		ostringstream out;
		out << elapsed;
		mInsertButton.SetWindowText(out.str().c_str());*/
		//Sleep(50);
	//}
	//TestTimer timer;

    // if timer 1 fires, tell the document to increment the frame
    if(nIDEvent == 1)
	{
        switch(playMode)
        {
			case PM_FORWARD:
                if(!mDocPtr->nextFrame())
                {
					//TRACE("Next Frame Timer\n");
                    // animation done
                    playMode = PM_STOPPED;
                    KillTimer(1);
                    mTimeline.setMouseEnabled(true);
				}
                break;
            case PM_REWIND:
                if(mDocPtr->getCurrentFrame() == 0 || !mDocPtr->previousFrame())
                {
                    // animation done
                    playMode = PM_STOPPED;
                    KillTimer(1);
                    mTimeline.setMouseEnabled(true);
                }
                break;
        }

		//TRACE("Before %.2f\n", timer.SecondsSinceLast());
        mDocPtr->MakeDirtyAndUpdateViews(false);
		//TRACE("After %.2f\n", timer.SecondsSinceLast());

		mTimeline.setCurrentFrame(mDocPtr->getCurrentFrame());
		//TRACE("Set Cur Frame %.2f\n", timer.SecondsSinceLast());
        mTimeline.makeCurrentFrameVisible();
		//TRACE("make cur vis %.2f\n", timer.SecondsSinceLast());
    }
	//ostringstream out;
	//out << timer.SecondsSinceLast();
	//mInsertButton.SetWindowText(out.str().c_str());
}

BOOL AnimationBar::PreTranslateMessage(MSG* pMsg)
{
	/*if(GetFocus() != &mTimeline) {
		return ReportView::PreTranslateMessage(pMsg);
	}*/
	if(pMsg->message == WM_KEYDOWN) {
		switch(pMsg->wParam) {
		case VK_LEFT:
		case VK_UP:
			OnBnClickedBackbutton();
			break;
		case VK_RIGHT:
		case VK_DOWN:
			OnBnClickedFwdbutton();
			break;
		case VK_END:
		case VK_MEDIA_NEXT_TRACK:
			OnBnClickedLastbutton();
			break;
		case VK_HOME:
		case VK_MEDIA_PREV_TRACK:
			OnBnClickedFirstbutton();
			break;
		case VK_MEDIA_PLAY_PAUSE:
			if(playMode == PM_FORWARD ||
				playMode == PM_REWIND) {
				OnBnClickedPausebutton();
			} else {
				OnBnClickedPlaybutton();
			}
			break;
		case VK_PLAY:
			OnBnClickedPlaybutton();
			break;
		case VK_MEDIA_STOP:
			OnBnClickedPausebutton();
			break;
		default:
			return ReportView::PreTranslateMessage(pMsg);
			break;
		}
		return true;
	}

	return ReportView::PreTranslateMessage(pMsg);
}


void AnimationBar::OnBnClickedInsertframe()
{
	InsertFrame();
}

void AnimationBar::OnAnimationtoolbarcontextmenuCut()
{
	mDocPtr->OnEditCut();
}

void AnimationBar::OnUpdateAnimationtoolbarcontextmenuCut(CCmdUI *pCmdUI)
{
	int left, right;
	mTimeline.getSelection(left, right);
	pCmdUI->Enable(left != 0);
}

void AnimationBar::OnAnimationtoolbarcontextmenuCopy()
{
	mDocPtr->OnEditCopy();
}

void AnimationBar::OnAnimationtoolbarcontextmenuPaste()
{
	mDocPtr->OnEditPaste();
}

void AnimationBar::OnUpdateAnimationtoolbarcontextmenuPaste(CCmdUI *pCmdUI)
{
	int left, right;
	mTimeline.getSelection(left, right);
	pCmdUI->Enable(!mDocPtr->ClipboardEmpty() && left != 0);
}

void AnimationBar::OnAnimationtoolbarcontextmenuRemoveframe()
{
	DeleteFrame();
}

void AnimationBar::OnUpdateAnimationtoolbarcontextmenuRemoveframe(CCmdUI *pCmdUI)
{
	int left, right;
	mTimeline.getSelection(left, right);
	pCmdUI->Enable(!(mDocPtr->getFrameType() == FT_Unset && !mTimeline.hasSelection())
					&& left > 0);
}

void AnimationBar::OnAnimationtoolbarcontextmenuAddframe()
{
	AddFrame();
}

void AnimationBar::OnUpdateAnimationtoolbarcontextmenuAddframe(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(mDocPtr->getFrameType() == FT_Unset && !mTimeline.hasSelection());
}

void AnimationBar::OnAnimationtoolbarcontextmenuInsertframe()
{
	InsertFrame();
}

void AnimationBar::OnUpdateAnimationtoolbarcontextmenuInsertframe(CCmdUI *pCmdUI)
{
	int left, right;
	mTimeline.getSelection(left, right);
	pCmdUI->Enable(mDocPtr->getFrameType(left - 1) != FT_Unset || left == 0);
}

BOOL AnimationBar::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if(zDelta < 0) {
		this->OnBnClickedFwdbutton();
	} else if(zDelta > 0) {
		this->OnBnClickedBackbutton();
	}

	return ReportView::OnMouseWheel(nFlags, zDelta, pt);
}
