#if !defined Dg_JointLoads_H
#define Dg_JointLoads_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dg_JointLoads.h : header file

// ACT - For good measure
#include "Length.hpp"
#include "Force.hpp"
#include "Mass.hpp"
#include "Torque.hpp"
#include <set>
#include "UndoRedo.h"

/////////////////////////////////////////////////////////////////////////////
// Dg_JointLoads dialog
int CountExternalForces(void);
void DisplayExtForces(CString *ret);
void DisplayExtForceNames(CString *ret);
void CreateJointString(CString* ret, const char* jointString, const Vector3 &force, const Vector3 &torque);
class Dg_JointLoads : public CDialog
{
// Construction
public:
	Dg_JointLoads(	C_Hom_Doc* aDocPtr,
						CWnd* pParent = NULL	);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(Dg_JointLoads)
	enum { IDD = IDD_JOINT_LOADS };
	double	mXComp;
	double	mYComp;
	double	mZComp;
	double	mXMoment;
	double	mYMoment;
	double	mZMoment;
	CString	mMomentUnits;
	CString	mForceUnits;
	static int		m_whichjoint;
	//}}AFX_DATA

	// Custom Attributes
   C_Hom_Doc* mDocPtr;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void UpdateDocument();
//	bool   UpdateControl( double &   aControlRef );
	///Yijun
	afx_msg void OnUndoDirectManipulation();
	afx_msg void OnUpdateUndoDirectManipulation(CCmdUI* pCmdUI);
	afx_msg void OnRedoDirectManipulation();
	afx_msg void OnUpdateRedoDirectManipulation(CCmdUI* pCmdUI);
	void UndoButtonStatus();
    void RedoButtonStatus();

	// Generated message map functions
	//{{AFX_MSG(Dg_JointLoads)
	afx_msg void OnRedrawJointApp();
	afx_msg void OnZeroAll();
	afx_msg void OnZEROVisible();
	virtual void OnOK();
	afx_msg void OnSelchangeJointnamecombo();
	afx_msg void OnChangeXComp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void UpdateStorage(int aJointNum);
	void ForceChanged();
	void TorqueChanged();
	
	double loadStorage[13][6];
	Force	tempForce;
	Torque tempTorque;

	std::set<int> changedForces;
	std::set<int> changedTorques;
public:
	afx_msg void OnEnChangeYcomp();
	afx_msg void OnEnChangeZcomp();
	afx_msg void OnEnChangeXmoment();
	afx_msg void OnEnChangeYmoment();
	afx_msg void OnEnChangeZmoment();
	afx_msg void OnPrintPrintwindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

	///Yijun
	CButton mUndoButtonControl;
	CButton mRedoButtonControl;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined Dg_JointLoads_H
