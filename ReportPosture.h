#if !defined(AFX_REPORTPOSTURE_H__9476BE31_F5F8_11D0_92D7_000000000000__INCLUDED_)
#define AFX_REPORTPOSTURE_H__9476BE31_F5F8_11D0_92D7_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ReportPosture.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ReportPosture form view

#include "stdafx.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ReportView.hpp"
#include "WindowData.hpp"
#include "afxwin.h"
#include "FloatRangeStatic.h"

#include "StatusBox.h"
#include "LimitScaleCtrl.h"
#include "resource.h"
#include "GLBalanceStanding.h"
#include "GLBalanceSitting.h"

class ReportPosture : public ReportView
{
protected:
	ReportPosture();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ReportPosture)

// Form Data
public:
	//{{AFX_DATA(ReportPosture)
	enum { IDD = IDD_ReportPosture };
	CString	mFooter;
	CString	mHeader;

	// angle value strings
	CString mLeftHandFlexion;
	CString mRightHandFlexion;
	CString mLeftHandExtension;
	CString mRightHandExtension;
	CString mLeftHandUlnarDeviation;
	CString mRightHandUlnarDeviation;
	CString mLeftHandRadialDeviation;
	CString mRightHandRadialDeviation;
	CString mLeftForearmRot;
	CString mRightForearmRot;
	CString	mLeftElbowIncluded;
	CString	mRightElbowIncluded;
	CString	mLeftShoulderVert;
	CString	mLeftShoulderHorz;
	CString	mRightShoulderVert;
	CString	mRightShoulderHorz;
	CString	mLeftHumeralRot;
	CString	mRightHumeralRot;
	CString mLeftHipIncluded;
	CString	mLeftHipVertical;
	CString mLeftHipHorizontal;
	CString mRightHipIncluded;
	CString	mRightHipVertical;
	CString mRightHipHorizontal;
	CString mLeftFemoralRot;
	CString mRightFemoralRot;
	CString mLeftLowerlegRot;
	CString mRightLowerlegRot;
	CString	mLeftKneeIncluded;
	CString	mLeftAnkleIncluded;
	CString	mRightKneeIncluded;
	CString	mRightAnkleIncluded;

	CString mHeadFlexion;
	CString mHeadAxial;
	CString mHeadLateral;
	CString mHeadFlexion2;
	CString mHeadAxial2;
	CString mHeadLateral2;
	CString mNeck1Flexion;//Neck 1 for upper neck and Neck 2 for lower neck
	CString mNeck1Rotation;
	CString mNeck1Bending;
	CString mNeck2Flexion;
	CString mNeck2Rotation;
	CString mNeck2Bending;
	CString	mAdjustedTrunkFlexion;
	CString	mAdjustedTrunkRotation;
	CString	mAdjustedTrunkBending;
	CString	mPelvicRotation;
	CString mPelvisLateralBending;
	CString mL5S1ForwardTilt;

	// min max strings
	CString mMinHandFlexion;
	CString mMaxHandFlexion;
	CString mMinHandExtension;
	CString mMaxHandExtension;
	CString mMinHandUlnarDeviation;
	CString mMaxHandUlnarDeviation;
	CString mMinHandRadialDeviation;
	CString mMaxHandRadialDeviation;
	CString mMinForearmRot;
	CString mMaxForearmRot;
	CString	mMinElbowIncluded;
	CString	mMaxElbowIncluded;
	CString	mMinShoulderVert;
	CString	mMaxShoulderVert;
	CString	mMinShoulderHorz;
	CString	mMaxShoulderHorz;
	CString	mMinHumeralRot;
	CString	mMaxHumeralRot;
	CString mMinHipIncluded;
	CString mMaxHipIncluded;
	CString	mMinHipVertical;
	CString	mMaxHipVertical;
	CString mMinHipHorizontal;
	CString mMaxHipHorizontal;
	CString mMinFemoralRot;
	CString mMaxFemoralRot;
	CString mMinLowerlegRot;
	CString mMaxLowerlegRot;
	CString	mMinKneeIncluded;
	CString	mMaxKneeIncluded;
	CString	mMinAnkleIncluded;
	CString	mMaxAnkleIncluded;

	CString mMinHeadFlexion;
	CString mMaxHeadFlexion;
	CString mMinHeadAxial;
	CString mMaxHeadAxial;
	CString mMinHeadLateral;
	CString mMaxHeadLateral;
	CString mMinHeadFlexion2;
	CString mMaxHeadFlexion2;
	CString mMinHeadAxial2;
	CString mMaxHeadAxial2;
	CString mMinHeadLateral2;
	CString mMaxHeadLateral2;
	CString mMinNeck1Flexion;
	CString mMaxNeck1Flexion;
	CString mMinNeck1Axial;
	CString mMaxNeck1Axial;
	CString mMinNeck1Lateral;
	CString mMaxNeck1Lateral;
	CString mMinNeck2Flexion;
	CString mMaxNeck2Flexion;
	CString mMinNeck2Axial;
	CString mMaxNeck2Axial;
	CString mMinNeck2Lateral;
	CString mMaxNeck2Lateral;
	CString	mMinAdjustedTrunkFlexion;
	CString	mMaxAdjustedTrunkFlexion;
	CString	mMinAdjustedTrunkRotation;
	CString	mMaxAdjustedTrunkRotation;
	CString	mMinAdjustedTrunkBending;
	CString	mMaxAdjustedTrunkBending;
	CString	mMinPelvicRotation;
	CString	mMaxPelvicRotation;
	CString mMinPelvisLateralBending;
	CString mMaxPelvisLateralBending;
	CString mMinL5S1ForwardTilt;
	CString mMaxL5S1ForwardTilt;


	CString mAdjustedTrunkFlexionPelv;
	CString mAdjustedTrunkRotationPelv;
	CString mAdjustedTrunkBendingPelv;

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

	// Control variables subclassed as CValidStatics to draw red text when outside ROM limits
	CFloatRangeStatic mLeftHandFlexionCtrl;
	CFloatRangeStatic mRightHandFlexionCtrl;
	CFloatRangeStatic mLeftHandExtensionCtrl;
	CFloatRangeStatic mRightHandExtensionCtrl;
	CFloatRangeStatic mLeftHandUlnarDeviationCtrl;
	CFloatRangeStatic mRightHandUlnarDeviationCtrl;
	CFloatRangeStatic mLeftHandRadialDeviationCtrl;
	CFloatRangeStatic mRightHandRadialDeviationCtrl;
	CFloatRangeStatic mLeftForearmRotCtrl;
	CFloatRangeStatic mRightForearmRotCtrl;
	CFloatRangeStatic mLeftElbowIncludedCtrl;
	CFloatRangeStatic mRightElbowIncludedCtrl;
	CFloatRangeStatic mLeftShoulderVertCtrl;
	CFloatRangeStatic mLeftShoulderHorzCtrl;
	CFloatRangeStatic mRightShoulderVertCtrl;
	CFloatRangeStatic mRightShoulderHorzCtrl;
	CFloatRangeStatic mLeftHumeralRotCtrl;
	CFloatRangeStatic mRightHumeralRotCtrl;
	CFloatRangeStatic mLeftHipIncludedCtrl;
	CFloatRangeStatic mLeftHipVerticalCtrl;
	CFloatRangeStatic mLeftHipHorizontalCtrl;
	CFloatRangeStatic mRightHipIncludedCtrl;
	CFloatRangeStatic mRightHipVerticalCtrl;
	CFloatRangeStatic mRightHipHorizontalCtrl;
	CFloatRangeStatic mLeftFemoralRotCtrl;
	CFloatRangeStatic mRightFemoralRotCtrl;
	CFloatRangeStatic mLeftLowerlegRotCtrl;
	CFloatRangeStatic mRightLowerlegRotCtrl;
	CFloatRangeStatic mLeftKneeIncludedCtrl;
	CFloatRangeStatic mLeftAnkleIncludedCtrl;
	CFloatRangeStatic mRightKneeIncludedCtrl;
	CFloatRangeStatic mRightAnkleIncludedCtrl;

	CFloatRangeStatic mHeadFlexionCtrl2;
	CFloatRangeStatic mHeadAxialCtrl2;
	CFloatRangeStatic mHeadLateralCtrl2;

	CFloatRangeStatic mHeadFlexionCtrl;

	CFloatRangeStatic mAdjustedNeck1FlexionCtrl;
	CFloatRangeStatic mAdjustedNeck1RotationCtrl;
	CFloatRangeStatic mAdjustedNeck1BendingCtrl;

	CFloatRangeStatic mAdjustedNeck2FlexionCtrl;
	CFloatRangeStatic mAdjustedNeck2RotationCtrl;
	CFloatRangeStatic mAdjustedNeck2BendingCtrl;


	CFloatRangeStatic mAdjustedTrunkFlexionCtrl;
	CFloatRangeStatic mAdjustedTrunkRotationCtrl;
	CFloatRangeStatic mAdjustedTrunkBendingCtrl;
	CFloatRangeStatic mPelvicRotationCtrl;
	CFloatRangeStatic mPelvisLateralBendingCtrl;
	CFloatRangeStatic mL5S1ForwardTiltCtrl;

	// TODO
	// debugging oblique view, want to see ankle lateral bending values
	CString mLeftAnkleBending;
	CString mRightAnkleBending;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ReportPosture)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	virtual void UpdateUnits();
	virtual void OnInitialUpdate();

	virtual void ReportPosture::OnUpdate(CView* aSenderPtr, LPARAM aHint, CObject* aHintPtr);

	inline static BOOL IsOpen(void) {
		return cPointer != NULL;
	}

	static void GetWindowData(WindowData& aWD) {
		if(cPointer != NULL) {
			aWD.mIsOpen = true;
			WINDOWPLACEMENT wp;
			cPointer->GetParentFrame()->GetWindowPlacement(&wp);
			aWD.mRectangle = wp.rcNormalPosition;
		} else {
			aWD.mIsOpen = false;
		}
	}

	inline static ReportView* Pointer(void)	{
		return cPointer;
	}

	virtual CString ReportName(void) const;

	//--- Custom Attributes ---

	private:

	static ReportView* cPointer;

	// Implementation
	protected:
	virtual ~ReportPosture();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(ReportPosture)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDisplayDialogcolor();
	//}}AFX_MSG
	//afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
public:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTPOSTURE_H__9476BE31_F5F8_11D0_92D7_000000000000__INCLUDED_)
