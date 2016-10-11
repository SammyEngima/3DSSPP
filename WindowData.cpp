#include "stdafx.h"
#include "WindowData.hpp"
#include <string>

using std::string;

extern int OpenType;	// To detect what version of the file we're opening

//---------------------------------------------------------------------------
WindowData::WindowData() :
	mMonitorName(),
	mFrameType((E_Quadrant)0),
	mViewType((E_View_Type)0),
	mCaption(),
	mComputePosition(TRUE),
	mRectangle(),
	mFramePtr(NULL),
	mIsOpen(TRUE),
	mShowCmd(SW_SHOWNORMAL)
{
   mRectangle.left = 0;
   mRectangle.top  = 0;
   mRectangle.right = 1;
   mRectangle.bottom = 1;
   return;
}

//---------------------------------------------------------------------------
WindowData::WindowData(//const String& aMonitorName,
                       const string& aMonitorName,
                       E_Quadrant aFrameType,
                       E_View_Type aViewType,
                       //const String& aCaption,
                       const string& aCaption,
                       BOOL aIsOpen,
                       UINT aShowCmd) :
	mMonitorName( aMonitorName ) ,
	mFrameType  ( aFrameType ) ,
	mViewType   ( aViewType ) ,
	mCaption    ( aCaption ) ,
	mComputePosition( TRUE ) ,
	mRectangle  () ,
	mFramePtr   ( NULL ) ,
	mIsOpen     ( aIsOpen ),
	mShowCmd	( aShowCmd )
{
   mRectangle.left = 0;
   mRectangle.top  = 0;
   mRectangle.right = 1;
   mRectangle.bottom = 1;
   return;
}

//---------------------------------------------------------------------------
CArchive& operator >> ( CArchive& aArchiveRef,
							   WindowData& aWindowDataRef )
{
   CString lMonitorName;
   CString lCaption;

   WORD lWord;

   aArchiveRef >> lMonitorName;
   aWindowDataRef.mMonitorName = lMonitorName;
   
   aArchiveRef >> lWord;
   aWindowDataRef.mFrameType = (E_Quadrant)(lWord);
   
   aArchiveRef >> lWord;
   aWindowDataRef.mViewType = (E_View_Type)(lWord);
   
   aArchiveRef >> lCaption;
   aWindowDataRef.mCaption = lCaption;

   aWindowDataRef.mComputePosition = FALSE;
   
   aArchiveRef >> aWindowDataRef.mRectangle.left;
   aArchiveRef >> aWindowDataRef.mRectangle.top;
   aArchiveRef >> aWindowDataRef.mRectangle.right;
   aArchiveRef >> aWindowDataRef.mRectangle.bottom;

   aArchiveRef >> aWindowDataRef.mIsOpen;

	// Old versions did not save mShowCmd, so we
	//		should not try to load it.
	if(OpenType >= 440)
		aArchiveRef >> aWindowDataRef.mShowCmd;

   return aArchiveRef;
}

//---------------------------------------------------------------------------
   //friend
CArchive& operator << ( CArchive& aArchiveRef,
							   WindowData& aWindowDataRef )
{
   CString   lMonitorName(aWindowDataRef.mMonitorName.c_str());
   CString   lCaption    (aWindowDataRef.mCaption.c_str());

   aArchiveRef << lMonitorName;
   aArchiveRef << (WORD)( aWindowDataRef.mFrameType );
   aArchiveRef << (WORD)( aWindowDataRef.mViewType  );
   aArchiveRef << lCaption;

   //	aWindowDataRef.mComputePlacement intentionally not streamed out. It is
   //		set to FALSE while this object is streamed in.
   
   aArchiveRef << aWindowDataRef.mRectangle.left; 
   aArchiveRef << aWindowDataRef.mRectangle.top; 
   aArchiveRef << aWindowDataRef.mRectangle.right; 
   aArchiveRef << aWindowDataRef.mRectangle.bottom; 
   
   aArchiveRef << aWindowDataRef.mIsOpen;

	// Added in 4.4.0
	aArchiveRef << aWindowDataRef.mShowCmd;

	return   aArchiveRef;
}

//---------------------------------------------------------------------------
void WindowData::SavePosition()
{
   if( mIsOpen )
   {
      WINDOWPLACEMENT   lWindowPlacement;
      if( mFramePtr->GetWindowPlacement( &lWindowPlacement ) )
      {
         mRectangle.left   = lWindowPlacement.rcNormalPosition.left;
         mRectangle.top    = lWindowPlacement.rcNormalPosition.top;
         mRectangle.right  = lWindowPlacement.rcNormalPosition.right;
         mRectangle.bottom = lWindowPlacement.rcNormalPosition.bottom;
			
			mShowCmd = lWindowPlacement.showCmd;	// Maximized/Minimized?
      }
      else
      {
			AfxMessageBox( "WindowData::SavePosition: Get window placement failed." );
      }
   }
   else
   {
      mRectangle.left   = 0;
      mRectangle.top    = 0;
      mRectangle.right  = 1;
      mRectangle.bottom = 1;
		
		mShowCmd = SW_SHOWNORMAL;
   }

   return;
}

void WindowData::LoadPosition()
{
	if( mIsOpen )
	{
		WINDOWPLACEMENT lWindowPlacement;
		mFramePtr->GetWindowPlacement(&lWindowPlacement);

		lWindowPlacement.rcNormalPosition.left = mRectangle.left;
		lWindowPlacement.rcNormalPosition.top = mRectangle.top;
		lWindowPlacement.rcNormalPosition.right = mRectangle.right;
		lWindowPlacement.rcNormalPosition.bottom= mRectangle.bottom;

		mFramePtr->SetWindowPlacement(&lWindowPlacement);
	}
}
