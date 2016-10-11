#pragma once

#include "stdafx.h"
//#include "afxwin.h"
//#include "hom.h"
//#include "Analysis.h"
//#include "glHom/locals.h"
//#include "enums.h"
#include "GLView.h"
#include "GLOblique.h"

class CGLObliqueView : public CGLView
{
    protected:
	    CGLObliqueView(void);
        
	    DECLARE_DYNCREATE(CGLObliqueView)

    public:
	    virtual ~CGLObliqueView(void);

    protected:
	    //{{AFX_MSG(C_Hom_View)
	    afx_msg void OnOblFrontView(void);
	    afx_msg void OnOblStdZoom(void);
	    afx_msg void OnOblBackView(void);
	    afx_msg void OnOblLeftView(void);
	    afx_msg void OnOblRightView(void);
	    afx_msg void OnOblTopView(void);
	    afx_msg void OnOblCenterFigure(void);
        afx_msg void OnOblUserview(void);
        afx_msg void OnModeltypeNone(void);
        afx_msg void OnModeltypeWireframe(void);
        afx_msg void OnModeltypeFlesh(void);
	    //}}AFX_MSG

public:
		void updateMDP();

		Camera &getCamera(void);
		Camera &getUserCamera(void);
		HandHeldObject &getHandHeld(void);

		E_Model_Type getModelType(void) const;
		//bool getShoesEnabled(void) const;
		bool getFloorEnabled(void) const;
		void setModelType(E_Model_Type type);
		//void setShoesEnabled(bool enable);
		void setFloorEnabled(bool enable);

		LPPICTURE getBackground(void) const;
		Picture_Mode getBackgroundMode(void) const;
		bool getBackgroundActive(void) const;
		float getBackgroundAlpha(void) const;
		void setBackground(LPPICTURE pic);
		void setBackgroundMode(Picture_Mode mode);
		void setBackgroundActive(bool bg);
		void setBackgroundAlpha(float alpha);

	    DECLARE_MESSAGE_MAP()
		virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
//		virtual void OnInitialUpdate();
		afx_msg void OnFileCopyactivewindow();
};