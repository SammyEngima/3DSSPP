#ifndef C_Dg_2DCombineHandLoad_INCLUDE
#define C_Dg_2DCombineHandLoad_INCLUDE

#include "Force.hpp"

class Dg_2D_HandLoads : public CDialog
{
    public:
	    Dg_2D_HandLoads(C_Hom_Doc *aDocPtr,CWnd* pParent = NULL);	

	    //{{AFX_DATA(Dg_2D_HandLoads)
	    enum { IDD = IDD_2DCombinedHandLoad };
	    CString mMagnitudeUnits;
	    int mVerticalAngle;
	    CString mExertion;
	    float mMagnitude;
	    BOOL mBackward;
	    //}}AFX_DATA

    protected:
	    //{{AFX_VIRTUAL(Dg_2D_HandLoads) 
	    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	    //}}AFX_VIRTUAL

        C_Hom_Doc *mDocPtr;
        int mHorizontalAngle;
        int Current_Control_ID;

        // Custom member functions
        bool UpdateDocument( void );
        void Restore();
	    void DoIncrement( bool increment_up );
	    bool VerifyTotalsInRange();
	    int Increment_Group;
        String ExertionText(double aV,double aH);

	    //{{AFX_MSG(Dg_2D_HandLoads)
	    virtual void OnOK();
	    afx_msg void OnRedrawButton();
	    afx_msg void OnBackward();
	    afx_msg void OnBnClickedIncrementMinus();
	    afx_msg void OnBnClickedIncrementPlus();
	    afx_msg void OnEnSetfocusMagnitude();
	    afx_msg void OnEnSetfocusVerticalangle();
        //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()
};

#endif