#pragma once

enum { TL_FRAMES, TL_SECONDS };
enum HandleState_e { HS_Nothing, HS_Trigger, HS_On, HS_Float };

class CTimelineCtrl : public CWnd
{
	DECLARE_DYNAMIC(CTimelineCtrl)

    public:
	    CTimelineCtrl();
	    virtual ~CTimelineCtrl() { }

        void setCurrentFrame(unsigned int frame);
        unsigned int getCurrentFrame(void) const { return currentFrame; }
        void makeCurrentFrameVisible(void);
        void setMouseEnabled(bool set) { mouseEnabled = set; }

        bool hasSelection(void) const { return selection; }
        void getSelection(int &left, int &right) const;
		void setSelection(int left, int right);

		int getHandleFrame();
		void onCloseInsertWindow();

    protected:
	    DECLARE_MESSAGE_MAP()
        BOOL RegisterControl();
        virtual void PreSubclassWindow();

    public:
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg void OnPaint();
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnTimer(UINT nIDEvent);

    private:
        int currentFrame;
        int leftFrame;

        int frameWidth;
        int framesVisible;

        int leftSelect;
        int rightSelect;
        bool selection;

		HandleState_e handleState;
		int handleFrame;
		int timerID;

        bool mouseEnabled;
        enum { SCROLL_NONE, SCROLL_LEFT, SCROLL_RIGHT, DRAG_LEFT, DRAG_RIGHT };
        int scrollMode;

		bool hasHandle;
		bool draggingHandle;
		CPoint handleStart;
		int leftStartFrame;
		int numberInserted;
		bool insertWindowOpen;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


