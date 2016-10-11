// Dg_Anthropometry.h : header file
//
#pragma once
/////////////////////////////////////////////////////////////////////////////
// Dg_Anthropometry dialog

#include "Anthropometry.hpp"
#include "Dg_PopulationEdit.h"
#include "afxwin.h"
#include <vector>
#include "hom_doc.h"
#include "Factors.hpp"
#include "PrintView.h"
#include "PrintFrame.h"
#include <map>
#include <string>
class StatusBox;

class Dg_Anthropometry : public CDialog
{
	DECLARE_DYNAMIC(Dg_Anthropometry)
// Construction
public:
	Dg_Anthropometry // standard constructor
            (
               C_Hom_Doc *      aDocPtr ,
			   StatusBox* status_box ,
               CWnd*            pParent = NULL
            );	
    ~Dg_Anthropometry();
protected:
	Anthropometry mAnthroBackup;
	Factors mnewfactor;
// Dialog Data
	//{{AFX_DATA(Dg_Anthropometry)
	enum { IDD = IDD_ANTHROPOMETRY };
   
   CString	mCGLabel;
   double	mFootCG;
   double	mFootL;
   double	mFootWt;
   double	mHipToL5CG;
   double	mHipToL5L;
   double	mHipToL5Wt;
   double	mHipToShoulderL;
   double	mHipToShoulderWt;
   double	mL5ToShoulderL;
   double   mL5ToShoulderCG; 
   double	mL5ToShoulderWt;    //need to find variable
   double    mHeadL;
   double    mHeadCG;
   double    mHeadWt;
   double    mNasionL;
   double    mNeck1L;
   double    mNeck1CG;
   double    mNeck1Wt;
   double    mNeck2L;
   double    mNeck2CG;
   double    mNeck2Wt;
   CString	mLengthLabel;
   double	mLowerArmL;
   double	mLowerArmWt;
   double	mLowerLegCG;
   double	mLowerLegL;
   double	mLowerLegWt;
   double	mLowerArmCG;
   double	mUpperArmCG;
   double	mUpperArmL;
   double	mUpperArmWt;
   double	mUpperLegL;
   double	mUpperLegWt;
   CString	mWeightLabel;
   double	mUpperLegCG;
   //double	mDiaphramMomentL;
   //double	mUpperBodyWt;
   CString	mFactorsName;
   //double	mHipToHipL;
   double	mShoulderToShoulderL;
   double	mHandL;   
   double	mHandWithFingersL;
   double	mHandWithFingersCG;
   double	mHandWithFingersWt;
   CString mleftright;  //for left/right button
   CButton	mMaintainHandPositionsControl;
    CEdit	mWeightControl;
	CEdit	mHeightControl;
	int		mPercentileValue;
	int		mGenderValue;
	CString	mHeightUnits;
    CStatic mCaution;
	CString	mWeightUnits;
	float	mHeightValue;
	float	mWeightValue;
	BOOL	mMaintainHandPositions;
	int		m_slider_percentile;
	//}}AFX_DATA
	CSliderCtrl m_Slider;

	// Custom attributes

	C_Hom_Doc *mDocPtr;
    C_Hom_Doc *lDocPtr; //for storing standard values
	Vector3 mLeftHand;
	Vector3 mRightHand;

	StatusBox* status_box;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void GetCurrentAnthropometryFromDocument();
	void GetAnthropometryFromDocument( const Anthropometry& aAnthropometryRef );

	// Custom member functions
	BOOL OnInitDialog();
	void UpdateDisplay();
    void UpdateReport();
	bool UpdateDocument();
    void UpdateStandard();
    void UpdateBackup();
    void InitializeCustomValues();
    void RetrieveBackupValues();
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	BOOL DataValidate();	// ACT - Added to validate data before applying settings.
	bool DoRedraw();
    void UpdateExtremities();
    //HBRUSH OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	// Generated message map functions
	//{{AFX_MSG(Dg_Anthropometry)
	afx_msg void OnPercentile95th();
	afx_msg void OnPercentile50th();
	afx_msg void OnPercentile5th();
	virtual void OnOK();
    afx_msg void ApplyCustomChanges(UINT ctrlID);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor);
    afx_msg void setID(UINT ctrlID);
    afx_msg void OnPercentileDataEntry();
	afx_msg void OnGenderMale();
	afx_msg void OnGenderFemale();
	afx_msg void OnRedraw();
	afx_msg void OnAdvanced();
  	afx_msg void OnKillfocusWeightValue();
	afx_msg void OnKillfocusHeightValue();
	afx_msg void OnClose();
	afx_msg void OnChangePercentile();
	virtual void OnCancel();
	afx_msg void OnEnChangeHeightvalue();
	afx_msg void OnEnSetfocusHeightvalue();
	afx_msg void OnEnChangeWeightvalue();
	afx_msg void OnEnSetfocusWeightvalue();
	//}}AFX_MSG
	BOOL mShoeCheck;
	CString mShoeHeight;
	afx_msg void OnBnClickedShoeCheck();
	CEdit mShoeHeightControl;
    afx_msg void OnBnClickedApply();
    afx_msg void OnBnClickedAnthro();

	void doApply(bool isOk);
private:
    std::vector<double> standard; //stores standard values for comparison to custom values
    std::vector<double> custom; //whatever the current set of values are
    std::vector<std::pair<double, double>> backup; //values to go back to if cancel/close is pressed
    bool dataEntry;
    bool updateClicked;
    bool applyClicked;
    bool valueChanged;
    double lengthConversion, forceConversion;    //default for lengths/CGs/Weights is metric system
    LinkLengths newLengths;
    LinkWeights newWeights;
    LinkCGs  newCGs;
    bool left; //flag for on left side of extremities or not
    bool modifyAnthro;
public:
	LinkLengths getNewLengths(){ return newLengths; }
	LinkWeights getNewWeights(){ return newWeights; }
public:
	static std::vector<bool> isred;     //bool for checking if the value is same as the standard value,if so the color should be red
	static std::vector<bool> isred_left;
	static std::vector<bool> isred_right;
	static std::vector<double> backl;   //store the data that input for left body
	static std::vector<double> backr;   // store the data that input for right body
	//static int CGchange ;
    afx_msg void OnBnClickedLeft();
    afx_msg void OnBnClickedRight();
	void OnPercentile5th_50th_95th(void);
	CString mLengthLabel2;
	CString mCGLabel2;
	CString mWeightLabel2;
	CString mWeightLabel3;

	CDC* pdc;
	CBitmap bmp;
	BITMAP bmap;
	BITMAPINFO* bInfo;
	CRect m_rect;
	double ratex,ratey;
	CDC imagedc;
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	void initialback();        //initialize backl and backr
	void updateback();
	
	afx_msg void OnBnClickedPrint();

protected:
	afx_msg LRESULT OnBeginPrinting(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndPrinting(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyPrint(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnPrintPrintwindow();
	//afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

};
