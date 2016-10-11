#pragma once

#include "stdafx.h"
#include "GLView.h"

#define LOOK_AT_DISTANCE 20.0
#define ARROW_LENGTH 25.0f
#define POINT_RADIUS 2.50

class CGLStickView: public CGLView
{
    protected:
        CGLStickView(void);

        DECLARE_DYNCREATE(CGLStickView)

    public:
        virtual ~CGLStickView(void);
		virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject *pHint);
};

