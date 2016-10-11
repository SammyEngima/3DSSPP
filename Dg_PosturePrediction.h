// Dg_PosturePrediction.h : header file
//
#include "afxwin.h"
#if ! defined( Dg_PosturePrediction_INCLUDE )
#define Dg_PosturePrediction_INCLUDE

/////////////////////////////////////////////////////////////////////////////
// Dg_PosturePrediction dialog

class Dg_PosturePrediction : public CDialog
{
// Construction
public:
	Dg_PosturePrediction(C_Hom_Doc *aDocPtr, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(Dg_PosturePrediction)
	enum { IDD = IDD_POSTURE_PREDICTION };
	int		mHandOrientation;
	CString	mHandLocationUnits;
	double	mLeftHorizontal;
	double	mLeftLateral;
	double	mLeftVertical;
	double	mRightHorizontal;
	double	mRightLateral;
	double	mRightVertical;
	//}}AFX_DATA

   // Custom attributes

   C_Hom_Doc *   mDocPtr;
   BOOL          mValidState;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

   // Custom member functions

   BOOL  UpdateDocument();
   void  GetHandPositionsFromDocument();

	// Disable/enable the undo button when necessary
	void UndoButtonStatus();

	// Generated message map functions
	//{{AFX_MSG(Dg_PosturePrediction)
	afx_msg void OnRedrawButton();
	virtual void OnOK();
	afx_msg void OnUndoButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CButton mUndoButtonControl;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedPpSymmetry();
	afx_msg void OnPrintPrintwindow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};

#endif
