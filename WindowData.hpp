#if ! defined( WindowDataIncludeMcr )
#define WindowDataIncludeMcr

#pragma once

//#include "string.hpp"
#include "quad_frm.h"

#include <string>

class WindowData
{
public:
   WindowData();

   WindowData(const std::string& aMonitorName,
              E_Quadrant aFrameType,
              E_View_Type aViewType,
              const std::string& aCaption,
              BOOL aIsOpen = FALSE,
              UINT aShowCmd = (UINT) SW_SHOWNORMAL);

   friend CArchive& operator >> (CArchive& aArchiveRef,
                                 WindowData& aWindowDataRef);
   
   friend CArchive& operator << (CArchive& aArchive,
                                 WindowData& aWindowDataRef);

   void SavePosition();
   void LoadPosition();

   std::string       mMonitorName;
   E_Quadrant        mFrameType;
   E_View_Type       mViewType;
   std::string       mCaption;
   UINT              mShowCmd;
   BOOL              mComputePosition;
   RECT              mRectangle;

   Quadrant_Frame *  mFramePtr;
   BOOL              mIsOpen;
};

#endif