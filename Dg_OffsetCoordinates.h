#pragma once


#include "resource.h"
#include "afxwin.h"
#include "Vector3.h"
#include "Skeleton.hpp"
#include "hom_doc.h"


// Dg_OffsetCoordinates dialog

class Dg_OffsetCoordinates : public CDialog
{
	DECLARE_DYNAMIC(Dg_OffsetCoordinates)

public:
	Dg_OffsetCoordinates(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dg_OffsetCoordinates();


// Dialog Data
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox mJointDropDown;
public:
	int offsetChoice;
	CString offsetCoordinateX;
	CString offsetCoordinateY;
	CString offsetCoordinateZ;
	afx_msg void OnBnClickedApply();
	afx_msg void OnCbnSelchangeCombo1();
	
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();

	void putValuesInBoxes(double x, double y, double z);
protected:
	virtual void OnOK();
	JointID DropDownOrder[NUM_JOINTS-2];

public:
	afx_msg void OnCbnEditchangeCombo1();
	afx_msg void OnCbnSetfocusCombo1();
	afx_msg void OnEnSetfocusEdit1();
	afx_msg void OnEnSetfocusEdit2();
	afx_msg void OnEnSetfocusEdit3();
	afx_msg void OnBnClickedCheck1();
	CString DefaultDescription;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	BOOL mAddOffset;
	afx_msg void OnBnClickedAddOffset();
};