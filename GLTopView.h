#pragma once

#include <stdafx.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "GLStickView.h"

class CGLTopView: public CGLStickView
{
    protected:
        CGLTopView(void);
        virtual ~CGLTopView(void) { }

        DECLARE_DYNCREATE(CGLTopView)

        // these must be overridden in the vantage-specific child classes
        virtual void setCamera(void) { }
        virtual void getRelevantAxes(int axes[2]) { axes[0] = 0; axes[1] = 1; }
};