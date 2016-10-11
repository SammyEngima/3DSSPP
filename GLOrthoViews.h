#pragma once

#include "stdafx.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "GLStickView.h"
#include "GLObliqueView.h"
#include "GLTopSideFront.h"

class CGLTopView : public CGLStickView
{
    protected:
        CGLTopView(void);

        DECLARE_DYNCREATE(CGLTopView)

    public:
        virtual ~CGLTopView(void);
};

class CGLSideView : public CGLStickView
{
    protected:
        CGLSideView(void);

        DECLARE_DYNCREATE(CGLSideView)

    public:
        virtual ~CGLSideView(void);
};

class CGLFrontView : public CGLStickView
{
    protected:
        CGLFrontView(void);

        DECLARE_DYNCREATE(CGLFrontView)

    public:
        virtual ~CGLFrontView(void);
};