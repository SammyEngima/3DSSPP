#include "stdafx.h"
#include "hom.h"
#include "hom_doc.h"
#include "AnimationBar.h"
#include "TimelineCtrl.h"
#include "MemDC.h"
#include ".\timelinectrl.h"
#include "LinearMotion.h"
#include <sstream>
#include "Task.h"
#include "Analysis.h"
#include "TestTimer.h"
#include "UndoKeyframe.h"
using std::ostringstream;
using std::endl;

#define SIDE_SPACING 20
#define ENDCAP_SIZE 10
//#define HATCH_DEPTH 5
#define FRAME_HEIGHT 30
#define FRAME_WIDTH 12
#define HATCH_HEIGHT 20

#define SMALL_BUFF 3
#define BIG_BUFF 5
#define TRIGGER_WAIT 0.3
#define FLOAT_WAIT 0.4

#define COLOR_TIMELINE COLORREF(RGB(0,0,0))

// Frame type colors
#define COLOR_KEYFRAME COLORREF(RGB(0,160,0))
#define COLOR_INTERP COLORREF(RGB(255,0,0))
#define COLOR_LINEAR COLORREF(RGB(255,204,255))
#define COLOR_EXP0	COLORREF(RGB(238,130,238))
#define COLOR_EXP1	COLORREF(RGB(255,128,0))
#define COLOR_EXP2	COLORREF(RGB(0,255,255))
#define COLOR_EXP6	COLORREF(RGB(128,128,255))
#define COLOR_LOC	COLORREF(RGB(255,255,0))

#define COLOR_CURRENT_OUTLINE COLORREF(RGB(0,0,0))
#define COLOR_ENDCAP COLORREF(RGB(150,150,150))
#define COLOR_SELECT_EMPTY COLORREF(RGB(49,106,197))
#define COLOR_SELECT_FRAME COLORREF(RGB(30,64,120))

IMPLEMENT_DYNAMIC(CTimelineCtrl, CWnd)
CTimelineCtrl::CTimelineCtrl()
{
    RegisterControl();

    currentFrame = 0;
    leftFrame = 0;
    frameWidth = 12;

    selection = false;
    mouseEnabled = true;

	handleState = HS_Nothing;
	timerID = 2;

	hasHandle = false;
	draggingHandle = false;
	numberInserted = 0;
	insertWindowOpen = false;
}

// don't bother reading this, it is just windows garbage
BOOL CTimelineCtrl::RegisterControl()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if(!(::GetClassInfo(hInst, _T("CTimelineControl"), &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc = ::DefWindowProc;
        wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
        wndcls.hInstance = hInst;
        wndcls.hIcon = NULL;
        wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName = NULL;
        wndcls.lpszClassName = _T("CTimelineControl");

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

BEGIN_MESSAGE_MAP(CTimelineCtrl, CWnd)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_SIZE()
    ON_WM_MOUSEMOVE()
    ON_WM_KEYDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_TIMER()
	ON_WM_CAPTURECHANGED()
END_MESSAGE_MAP()

void CTimelineCtrl::PreSubclassWindow()
{
    static_cast<AnimationBar*>(GetParent())->UpdateLayout();

    CWnd::PreSubclassWindow();
}

void CTimelineCtrl::getSelection(int &left,int &right) const
{
    left = leftSelect;
    right = rightSelect;
}
void CTimelineCtrl::setSelection(int left, int right)
{
	leftSelect = left;
	rightSelect = right;
	selection = leftSelect != rightSelect;
}

int CTimelineCtrl::getHandleFrame() {
	return handleFrame;
}
void CTimelineCtrl::onCloseInsertWindow() {
	insertWindowOpen = false;
	RedrawWindow();
}

// prevent widget from erasing its background, for double buffering
BOOL CTimelineCtrl::OnEraseBkgnd(CDC* pDC) { return TRUE; }

void CTimelineCtrl::setCurrentFrame(unsigned int frame)
{
	leftSelect += frame - currentFrame;
	rightSelect += frame - currentFrame;
    currentFrame = frame;
}

// adjust the viewport to make the current frame visible
void CTimelineCtrl::makeCurrentFrameVisible(void)
{
    if(currentFrame < leftFrame) leftFrame = currentFrame;
    else if(currentFrame >= leftFrame + framesVisible) leftFrame = currentFrame - framesVisible + 1;
    else return; // no unnecessary redraws

    RedrawWindow();
}

void CTimelineCtrl::OnPaint()
{
    // Double buffering with CMemDCX
    CPaintDC painter(this);
    CMemDCX dc(&painter);
    dc.SetBkColor(GetSysColor(COLOR_3DFACE));

    // compute the number of frames that can be displayed
    CRect client;
    GetClientRect(&client);
	int height = client.Height();//(client.Height() >= 30) ? 30 : client.Height();
    
    // erase the background here, rather than in message handler,
    // to preserve double buffering
    CBrush bgBrush(GetSysColor(COLOR_3DFACE));
    CBrush *oldBrush = dc.SelectObject(&bgBrush);
    dc.FillRect(&client,&bgBrush);

    framesVisible = (client.Width() - 2*SIDE_SPACING - 1)/frameWidth;
    if(framesVisible == 0) return;

    CPen thin(PS_SOLID,1,COLOR_TIMELINE);
    CPen *oldPen = dc.SelectObject(&thin);
    CFont textFont;
    textFont.CreateFont(7, 3, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, 
                        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
    CFont *oldFont = dc.SelectObject(&textFont);

    // horizontal timeline
    dc.MoveTo(SIDE_SPACING,height/2);
    dc.LineTo(SIDE_SPACING + frameWidth*framesVisible,height/2);

	// DEBUG cjb draw outline of window just to see it
	/*dc.MoveTo(0,0);
	dc.LineTo(0,client.Height()-1);
	dc.LineTo(client.Width()-1, client.Height()-1);
	dc.LineTo(client.Width()-1, 0);
	dc.LineTo(0,0);*/

	for(int i = 0; i < framesVisible; ++i)
    {
        // draw hatch mark at frame boundary
		dc.MoveTo(SIDE_SPACING + frameWidth*(i + 1), (height - HATCH_HEIGHT)/2);//1 + HATCH_DEPTH);
		dc.LineTo(SIDE_SPACING + frameWidth*(i + 1), (height + HATCH_HEIGHT)/2);//height - HATCH_DEPTH);

        // display frame number on certain multiples
        if((leftFrame + i) % 5 == 0)
        {
            CString frame;
            frame.Format("%d",leftFrame + i);
            CRect textRegion(SIDE_SPACING + frameWidth*(i - 2),(height + FRAME_HEIGHT)/2 + 2,SIDE_SPACING + 2 + frameWidth*(i + 3),(height + FRAME_HEIGHT)/2 + 12);
            dc.DrawText(frame,&textRegion,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			frame.Format("%.1f", (float)(((float)leftFrame + i) / FRAMES_PER_SECOND));
			textRegion.SetRect(SIDE_SPACING + frameWidth*(i-2), (height - FRAME_HEIGHT)/2 - 12, SIDE_SPACING + 2 + frameWidth*(i+3),(height - FRAME_HEIGHT)/2 - 2);
			dc.DrawText(frame, &textRegion,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }

	Task& task = Get_App_Ptr()->Get_Document_Ptr()->getTask();
    // color the frames based on their status
    for(int i = 0; i < framesVisible; ++i)
    {
        CBrush fillBrush;

		bool colored = true;
		COLORREF color = COLOR_SELECT_EMPTY;
		switch(task.getFrameType(leftFrame + i)) {
			case FT_Keyframe:
				color = COLOR_KEYFRAME;
				break;
			case FT_Linear:
				color = COLOR_LINEAR;
				break;
			case FT_Point:
				color = COLOR_EXP0;
				break;
			case FT_Stand:
				color = COLOR_EXP1;
				break;
			case FT_Sit:
				color = COLOR_EXP2;
				break;
			case FT_Grasp:
				color = COLOR_EXP6;
				break;
			case FT_Loc:
				color = COLOR_LOC;
				break;
			default:
				colored = false;
				break;
		}

        // frame is part of current selection
        if(selection && leftSelect <= i + leftFrame && rightSelect >= i + leftFrame)
        {
            /*if(Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton().hasFrame(leftFrame + i))
                fillBrush.CreateSolidBrush(COLOR_SELECT_FRAME);
            else
                fillBrush.CreateSolidBrush(COLOR_SELECT_EMPTY);
			color[2] = 255;*/
			if(colored) {
				color = COLORREF(RGB((GetRValue(color) + 3*GetRValue(COLOR_SELECT_FRAME))/4,
									 (GetGValue(color) + 3*GetGValue(COLOR_SELECT_FRAME))/4,
									 (GetBValue(color) + 3*GetBValue(COLOR_SELECT_FRAME))/4));
				//color = COLOR_SELECT_FRAME;
			}
			//colored = true;
        }

		if(colored) {
			//color = COLORREF(RGB(255,0,0));
			fillBrush.CreateSolidBrush(color);
			dc.SelectObject(&fillBrush);
			CRect fillArea(SIDE_SPACING + frameWidth*i,(height - FRAME_HEIGHT)/2 ,SIDE_SPACING + 1 + frameWidth*(i + 1),(height + FRAME_HEIGHT)/2);
			dc.Rectangle(&fillArea);
		}
    }

    // if there is a selection, outline it
    int leftSelectIndex = (leftSelect < leftFrame) ? 0 : leftSelect - leftFrame;
    int rightSelectIndex = (rightSelect >= leftFrame + framesVisible) ? framesVisible - 1 : rightSelect - leftFrame;
    if(selection && leftSelectIndex < framesVisible && rightSelectIndex >= 0)
    {
        CPen thick(PS_SOLID,2,COLOR_CURRENT_OUTLINE);
        CPen *tempPen = dc.SelectObject(&thick);
        CBrush *tempBrush = (CBrush*)dc.SelectStockObject(HOLLOW_BRUSH);
        
        CRect fillArea(SIDE_SPACING + frameWidth*leftSelectIndex,(height - FRAME_HEIGHT)/2,SIDE_SPACING + 2 + frameWidth*(rightSelectIndex + 1),(height + FRAME_HEIGHT)/2);
        dc.Rectangle(&fillArea);

        dc.SelectObject(tempPen);
        dc.SelectObject(tempBrush);
    }
    // otherwise, outline the cursor if it is on the screen
    else if(currentFrame >= leftFrame && currentFrame < framesVisible + leftFrame)
    {
        CPen thick(PS_SOLID,2,COLOR_CURRENT_OUTLINE);
        CPen *tempPen = dc.SelectObject(&thick);
        CBrush *tempBrush = (CBrush*)dc.SelectStockObject(HOLLOW_BRUSH);
        
        dc.Rectangle(SIDE_SPACING + frameWidth*(currentFrame - leftFrame),(height - FRAME_HEIGHT)/2,SIDE_SPACING + 2 + frameWidth*((currentFrame - leftFrame) + 1),(height + FRAME_HEIGHT)/2);
        
        dc.SelectObject(tempPen);
        dc.SelectObject(tempBrush);
    }

    // the first vertical line (far left)
    dc.MoveTo(SIDE_SPACING,(height - FRAME_HEIGHT)/2);
    dc.LineTo(SIDE_SPACING,(height + FRAME_HEIGHT)/2);

    // draw triangular caps to indicate availability of viewport scrolling
    CBrush blackFill(COLOR_ENDCAP);
    dc.SelectObject(&blackFill);
    CPoint rightTri[3] = {CPoint(SIDE_SPACING + frameWidth*framesVisible,(height - FRAME_HEIGHT)/2),
                          CPoint(SIDE_SPACING + frameWidth*framesVisible,(height + FRAME_HEIGHT)/2),
                          CPoint(SIDE_SPACING + ENDCAP_SIZE + frameWidth*framesVisible,height/2)};
    dc.Polygon(rightTri,3);
    if(leftFrame > 0)
    {
        CPoint leftTri[3] = {CPoint(SIDE_SPACING - ENDCAP_SIZE,height/2),
                             CPoint(SIDE_SPACING,(height + FRAME_HEIGHT)/2),
                             CPoint(SIDE_SPACING,(height - FRAME_HEIGHT)/2)};
        dc.Polygon(leftTri,3);
    }

	if(handleState == HS_On || handleState == HS_Float) {
		CPoint tri[3] = {CPoint(SIDE_SPACING + frameWidth * (handleFrame - leftFrame), (height - HATCH_HEIGHT)/2),
						CPoint(SIDE_SPACING + frameWidth * (handleFrame - leftFrame) - 10, (height - FRAME_HEIGHT)/2),
						CPoint(SIDE_SPACING + frameWidth * (handleFrame - leftFrame) + 10, (height - FRAME_HEIGHT)/2)};
		dc.Polygon(tri, 3);
	}

    dc.SelectObject(oldPen);
    dc.SelectObject(oldBrush);
    dc.SelectObject(oldFont);
}

// returns true if P is above the line from A to B, false otherwise
// if the line is vertical, true if P is to the right, false otherwise
bool aboveLine(CPoint P, CPoint A_in, CPoint B_in) {
	CPoint A,B;
	if(A_in.y > B_in.y) {
		A = A_in;
		B = B_in;
	} else {
		A = B_in;
		B = A_in;
	}

	// vertical
	if(A.x == B.x) {
		if(P.x > A.x) return true;
		return false;
	}
	// horizontal
	if(A.y == B.y) {
		if(P.y < A.y) return true;
		return false;
	}
	// general
	double fraction = ((double)(A.y - B.y))/(A.x - B.x);
	if(P.x * fraction - B.x * fraction - P.y + B.y > 0) return true;
	return false;
}
void CTimelineCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    if(!mouseEnabled) return;

    // allow for mouse to maintain control outside of client region
    SetCapture();

	CRect client;
    GetClientRect(&client);
	int height = client.Height();//(client.Height() >= 30) ? 30 : client.Height();
    
	if(handleState == HS_On) {
		CPoint upperLeft(SIDE_SPACING + frameWidth * (handleFrame - leftFrame) - 10, (height - FRAME_HEIGHT)/2);
		CPoint upperRight(SIDE_SPACING + frameWidth * (handleFrame - leftFrame) + 10, (height - FRAME_HEIGHT)/2);
		CPoint bottom(SIDE_SPACING + frameWidth * (handleFrame - leftFrame), (height - HATCH_HEIGHT)/2);
		if(!aboveLine(point, upperLeft, upperRight) &&
			aboveLine(point, bottom, upperLeft) &&
			aboveLine(point, bottom, upperRight) ) {
			// clicking in the handle
			Task& task = Get_App_Ptr()->Get_Document_Ptr()->getTask();
			hasHandle = true;
			handleStart = point;
			leftStartFrame = handleFrame - 1;
			((AnimationBar*)GetParent())->showInsertWindow(bottom, 0, true);
			insertWindowOpen = true;
			numberInserted = 0;
			RedrawWindow();
			return;
		}
	}

    // scroll viewport if clicked outside of timeline
    if(point.x <= SIDE_SPACING && leftFrame > 0)
    {
        scrollMode = SCROLL_LEFT;
        SetTimer(1,50,NULL);
        return;
    }
    else if(point.x <= SIDE_SPACING) return;
    else if(point.x >= SIDE_SPACING + frameWidth*framesVisible)
    {
        scrollMode = SCROLL_RIGHT;
        SetTimer(1,50,NULL);
        return;
    }

    int frameOffset = (point.x - SIDE_SPACING)/frameWidth;
    int newFrame = leftFrame + frameOffset;
    
    if(nFlags & MK_SHIFT)
    {
        leftSelect = (currentFrame < newFrame) ? currentFrame : newFrame;
        rightSelect = (currentFrame < newFrame) ? newFrame : currentFrame;
		// if rightSelect is the same as leftSelect, there is only one frame, so don't consider it a selection
		if(leftSelect != rightSelect) {
			selection = true;
		}
		Get_App_Ptr()->Get_Document_Ptr()->UpdateViews();

		//RedrawWindow();
    }
    else
    {
        currentFrame = newFrame;
		leftSelect = currentFrame;
		rightSelect = currentFrame;
        selection = false;
		//((AnimationBar*)GetParent())->enableMotionButton(false);
        Get_App_Ptr()->Get_Document_Ptr()->setCurrentFrame(currentFrame);
		Get_App_Ptr()->Get_Document_Ptr()->UpdateViews();
    }

    CWnd::OnLButtonDown(nFlags, point);
}

void CTimelineCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    ReleaseCapture();
    if(scrollMode)
    {
        scrollMode = SCROLL_NONE;
        KillTimer(1);
    }
	if(hasHandle) {
		if(draggingHandle) {
			// close insert window
			C_Hom_Doc* pDoc = Get_App_Ptr()->Get_Document_Ptr();
			((AnimationBar*)GetParent())->showInsertWindow(CPoint(), 0, false);
			insertWindowOpen = false;
			// interpolate
			if(pDoc->getFrameType(leftStartFrame) != FT_Unset &&
				pDoc->getFrameType(leftStartFrame + numberInserted + 1) != FT_Unset) {
				Skeleton* first = pDoc->getSkeletonAtFrame(leftStartFrame);
				Skeleton* last = pDoc->getSkeletonAtFrame(leftStartFrame + numberInserted + 1);
				LinearMotion motion(&pDoc->getTask());
				motion.SetInitialFrame(*first);
				motion.SetFinalFrame(*last);
				motion.SetNumFrames(numberInserted + 2);
				Analysis analysis;
				if(motion.CreateMotion()) {
				std::vector<Skeleton*> contents = motion.GetContents();
					for(int i = 1; i < motion.GetNumFrames() - 1; i++) {
						analysis.Analyze(contents[i]);
						pDoc->setFrame(leftStartFrame + i, contents[i], FT_Linear);
					}
					delete contents.front();
					delete contents.back();

					// get vector of frames
					std::vector<Frame> frameVector(motion.GetNumFrames() - 2);
					for(int i = 0; i < frameVector.size(); i++) {
						frameVector[i].skeleton = contents[i+1];
						frameVector[i].frameType = FT_Linear;
					}
					// add undo event for the inserted frames
					UndoableKeyframeInsert* undoEvent = new UndoableKeyframeInsert(leftStartFrame+1, motion.GetNumFrames()-2, frameVector);
					pDoc->addUndoEvent(undoEvent);
				}
			}
			RedrawWindow();
		}
	}
	draggingHandle = false;
	hasHandle = false;

    CWnd::OnLButtonUp(nFlags, point);
}

void CTimelineCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    if(!mouseEnabled) return;

	int frameOffset = (point.x - SIDE_SPACING)/frameWidth;
    int newFrame = leftFrame + frameOffset;
    if(newFrame < 0) newFrame = 0;

    if(nFlags & MK_LBUTTON)
    {
        // scroll when the mouse moves outside of the timeline
        if(point.x <= SIDE_SPACING && leftFrame > 0)
        {
            if(scrollMode) return;
            scrollMode = DRAG_LEFT;
            SetTimer(1,25,NULL);
            return;
        }
        else if(point.x <= SIDE_SPACING) return;
        else if(point.x >= SIDE_SPACING + frameWidth*framesVisible)
        {
            if(scrollMode) return;
            scrollMode = DRAG_RIGHT;
            SetTimer(1,25,NULL);
            return;
		}
        // inside timeline, so stop dragging
        else if(scrollMode)
        {
            scrollMode = SCROLL_NONE;
            KillTimer(1);
        }

		if(hasHandle) {
			if(!draggingHandle) {
				if(abs(point.x - handleStart.x) > 5) {
					draggingHandle = true;
				}
			}
			// TODO doing a linear interpolation does not set application to dirty
			if(draggingHandle) {
				int frameOffset = (point.x - SIDE_SPACING + frameWidth/2)/frameWidth;
				int newFrame = leftFrame + frameOffset;
				if(newFrame < 0) newFrame = 0;
				int x;
				if(newFrame > leftStartFrame) {
					handleFrame = newFrame;
					x = point.x;
				} else {
					x = SIDE_SPACING + frameWidth * (handleFrame - leftFrame);
				}
				int frameDelta = (handleFrame - leftStartFrame - 1) - numberInserted;
				if(frameDelta != 0) {
					Task& task = Get_App_Ptr()->Get_Document_Ptr()->getTask();
					if(frameDelta > 0) {
						task.insertFrame(frameDelta, leftStartFrame);
						if(currentFrame > leftStartFrame) {
							currentFrame += frameDelta;
							leftSelect += frameDelta;
							rightSelect += frameDelta;
						}
					} else {
						for(int i = 0; i < -frameDelta; i++) {
							task.deleteFrame(leftStartFrame + 1);
						}
						if(currentFrame > leftStartFrame + numberInserted) {
							currentFrame += frameDelta;
							leftSelect += frameDelta;
							rightSelect += frameDelta;
						}
					}
					numberInserted += frameDelta;
					((AnimationBar*)GetParent())->showInsertWindow(CPoint(x, 0), handleFrame - leftStartFrame - 1, true);
					insertWindowOpen = true;
					RedrawWindow();
					Get_App_Ptr()->Get_Document_Ptr()->setCurrentFrame(currentFrame);
					TRACE("Setting cur frame to %i\n", currentFrame);
					Get_App_Ptr()->Get_Document_Ptr()->changeFrame();
				}
			}
			return;
		}
        
        if(nFlags & MK_SHIFT)
        {
            int newLeftSelect = (currentFrame < newFrame) ? currentFrame : newFrame;
            int newRightSelect = (currentFrame < newFrame) ? newFrame : currentFrame;

            // if anything changed, post a redraw
            if(newLeftSelect != leftSelect || newRightSelect != rightSelect || selection != true)
            {
                leftSelect = newLeftSelect;
                rightSelect = newRightSelect;
				if(leftSelect != rightSelect) {
					selection = true;
				}
				//RedrawWindow();
				Get_App_Ptr()->Get_Document_Ptr()->UpdateViews();
            }
        }
        else
        {
            // post a redraw if necessary
            if(currentFrame != newFrame || selection)
            {
                currentFrame = newFrame;
				leftSelect = rightSelect = currentFrame;
                selection = false;
				//TestTimer timer;
                Get_App_Ptr()->Get_Document_Ptr()->setCurrentFrame(currentFrame);
				//ostringstream out;
				//out << "mouse time: " << timer.SecondsSinceLast() << endl;
				//((AnimationBar*)GetParent())->mInsertButton.SetWindowText(out.str().c_str());
            }
        }
        // RedrawWindow();
	} else {
		int frameOffset = (point.x - SIDE_SPACING + frameWidth/2)/frameWidth;
		int newFrame = leftFrame + frameOffset;
		if(newFrame < 0) newFrame = 0;
		if(insertWindowOpen) {
			handleState = HS_On;
		} else if(hasHandle) {
			handleState = HS_On;
			handleFrame = max(newFrame, 1);
		} else {
			// if not clicking or anything, do handle checking
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = m_hWnd;
			switch(handleState) {
				// TODO compute redraw rectangle because it's small
				case HS_Nothing:
					if(((point.x - SIDE_SPACING + (SMALL_BUFF - 1)/2) % frameWidth) <= SMALL_BUFF - 1) {
						handleState = HS_Trigger;
						handleFrame = max(newFrame, 1);
						SetTimer(timerID, 1000 * TRIGGER_WAIT, NULL);
					}
					break;
				case HS_Trigger:
					if(((point.x - SIDE_SPACING + (SMALL_BUFF - 1)/2) % frameWidth) > SMALL_BUFF - 1) {
						handleState = HS_Nothing;
						KillTimer(timerID);
					}
					break;
				case HS_On:
					if(((point.x - SIDE_SPACING + (BIG_BUFF - 1)/2) % frameWidth) > BIG_BUFF - 1) {
						handleState = HS_Float;
						SetTimer(timerID, 1000 * FLOAT_WAIT, NULL);
						RedrawWindow();
					}
					break;
				case HS_Float:
					if(((point.x - SIDE_SPACING + (SMALL_BUFF - 1)/2) % frameWidth) <= SMALL_BUFF - 1) {
						handleState = HS_On;
						handleFrame = max(newFrame, 1);
						RedrawWindow();
						TrackMouseEvent(&tme);
						KillTimer(timerID);
					}
					break;
				default:
					break;
			}
		}
	}

    CWnd::OnMouseMove(nFlags, point);
}

void CTimelineCtrl::OnTimer(UINT nIDEvent)
{
    if(nIDEvent == 1) // scroll timer
    {
        if(scrollMode == SCROLL_LEFT || scrollMode == DRAG_LEFT)
        {
            if(leftFrame == 0)
            {
                KillTimer(1);
                return;
            }
            leftFrame -= 20/frameWidth;

            // if dragging, update selection/currentFrame
            if(scrollMode == DRAG_LEFT)
            {
                if(selection)
                {
                    if(leftSelect <= leftFrame) rightSelect = leftFrame;
                    else leftSelect = leftFrame;
                }
                else
                {
                    currentFrame = leftFrame;
                    // RedrawWindow();
                    Get_App_Ptr()->Get_Document_Ptr()->setCurrentFrame(currentFrame);
                }
			} else {
				RedrawWindow();
			}
        }
        else // right scrolling/dragging
        {
            leftFrame += 20/frameWidth;

            // if dragging, update selection/currentFrame
            if(scrollMode == DRAG_RIGHT)
            {
                if(selection)
                {
                    if(rightSelect >= leftFrame + framesVisible) leftSelect = leftFrame + framesVisible - 1;
                    else rightSelect = leftFrame + framesVisible - 1;
                }
                else
                {
                    currentFrame = leftFrame + framesVisible - 1;
                    // RedrawWindow();
                    Get_App_Ptr()->Get_Document_Ptr()->setCurrentFrame(currentFrame);
                }
			} else {
				RedrawWindow();
			}
        }
        // RedrawWindow();
	} else if(nIDEvent == timerID) { // handle timer
		// one time timer
		KillTimer(timerID);
		if(handleState == HS_Trigger) {
			handleState = HS_On;
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = m_hWnd;
			TrackMouseEvent(&tme);
			RedrawWindow();
		} else if(handleState == HS_Float) {
			handleState = HS_Nothing;
			RedrawWindow();
		}
	}


    CWnd::OnTimer(nIDEvent);
}
LRESULT CTimelineCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_MOUSELEAVE) {
		if(!insertWindowOpen) {
			handleState = HS_Nothing;
			KillTimer(timerID);
			RedrawWindow();
		}
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

