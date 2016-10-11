// ReportAnthropometry.h : header file
//
#ifndef REPORTANTHROPOMETRY_H
#define REPORTANTHROPOMETRY_H

/////////////////////////////////////////////////////////////////////////////
// ReportAnthropometry form view

#include "stdafx.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ReportView.hpp"
#include "WindowData.hpp"

#include "StatusBox.h"
#include "LimitScaleCtrl.h"
#include "resource.h"
#include "GLBalanceStanding.h"
#include "GLBalanceSitting.h"

class ReportAnthropometry : public ReportView
{
protected:
   ReportAnthropometry();           // protected constructor used by dynamic creation
   DECLARE_DYNCREATE(ReportAnthropometry)

   // Form Data
public:
   //{{AFX_DATA(ReportAnthropometry)
   enum { IDD = IDD_ReportAnthropometry };
   CString	mCGLabel;
   CString	mFootCG;
   CString	mFooter;
   CString	mFootL;
   CString	mFootWt;
   CString	mFootCGR;
   CString	mFootLR;
   CString	mFootWtR;
   CString	mHeader;
   CString	mHipToL5CG;
   CString	mHipToL5L;
   CString	mHipToL5Wt;
   CString	mHipToShoulderL;
   CString	mHipToShoulderWt;
   CString	mL5ToShoulderCG;
   CString	mL5ToShoulderL;
   CString	mL5ToShoulderWt;
   CString	mLengthLabel;
   CString	mLowerLegCG;
   CString	mLowerLegL;
   CString	mLowerLegWt;
   CString	mLowerLegCGR;
   CString	mLowerLegLR;
   CString	mLowerLegWtR;
   CString	mLowerArmL;
   CString	mLowerArmWt;
   CString	mLowerArmCG;
   CString	mLowerArmLR;
   CString	mLowerArmWtR;
   CString	mLowerArmCGR;
   CString	mUpperArmCG;
   CString	mUpperArmL;
   CString	mUpperArmWt;
   CString	mUpperArmCGR;
   CString	mUpperArmLR;
   CString	mUpperArmWtR;
   CString	mUpperLegL;
   CString	mUpperLegWt;
   CString	mUpperLegCG;
   CString	mUpperLegLR;
   CString	mUpperLegWtR;
   CString	mUpperLegCGR;
   CString	mWeightLabel;
   CString	mDiaphramMomentL;
   CString	mUpperBodyWt;
   CString	mUpperBodyL;
   CString	mUpperBodyCG;
   CString	mFactorsName;
   CString	mHipToHipL;
   CString	mShoulderToShoulderL;
   CString	mHandL;
   CString	mHandCG;
   CString	mHandWt;
   CString	mHandLR;
   CString	mHandCGR;
   CString	mHandWtR;
   CString	mHeadL;
   CString	mHeadCG;
   CString	mHeadWt;
   CString	mNasionL;
   CString	mNeck1L;
   CString	mNeck2L;
   CString	mNeck1CG;
   CString	mNeck1Wt;
   CString	mNeck2CG;
   CString	mNeck2Wt;
   // CString	mLowerArmAndHandL;
   // CString	mLowerArmAndHandCG;
   // CString	mLowerArmAndHandWt;
   CString	mHandWithFingersL;
   CString	mHandWithFingersCG;
   CString	mHandWithFingersWt;
   CString	mHandWithFingersLR;
   CString	mHandWithFingersCGR;
   CString	mHandWithFingersWtR;

	CString mHandForceUnits;
	CString	mUnitsLabel;
	CString m3DLowBackCompressionUnits;
	CString mHandForceTxtLVal;
	CString mHandForceTxtRVal;
	CString	mLeftHori;
	CString mRightHori;
	CString mLeftLat;
	CString mLeftVert;
	CString mRightLat;
	CString mRightVert;
   //}}AFX_DATA

   // Attributes

   // Operations

   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ReportAnthropometry)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL
   //--- Custom member functions ---
public:
	virtual void UpdateUnits();
	virtual void OnInitialUpdate();

   void OnUpdate(CView* aSenderPtr, LPARAM aHint, CObject* aHintPtr);

   static BOOL IsOpen() {
      return cPointer != NULL;
   }

   static void GetWindowData(WindowData& aWD)
   {
      if(cPointer != NULL)
      {
         aWD.mIsOpen = true;
         WINDOWPLACEMENT wp;
         cPointer->GetParentFrame()->GetWindowPlacement(&wp);
         aWD.mRectangle = wp.rcNormalPosition;
      }
      else
         aWD.mIsOpen = false;
   }

   static ReportView* Pointer() {
      return cPointer;
   }

   virtual CString ReportName()  const;

   //--- Custom Attributes ---

private:

   static ReportView* cPointer;

   // Implementation
protected:
   virtual ~ReportAnthropometry();

#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

   // Generated message map functions
   //{{AFX_MSG(ReportAnthropometry)
   afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
   afx_msg void OnDisplayDialogcolor();
   //}}AFX_MSG
   //afx_msg void OnFilePrintPreview();

   DECLARE_MESSAGE_MAP()
   public:
	afx_msg void OnStnClickedL5toshoulderl();
};

/////////////////////////////////////////////////////////////////////////////

#endif	// End include guard
