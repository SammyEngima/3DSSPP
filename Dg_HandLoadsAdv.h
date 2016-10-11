#if !defined( Dg_HandLoads_ADV_INCLUDE )
#define Dg_HandLoads_ADV_INCLUDE

#include "Dg_HandLoads.h"

class Dg_HandLoads_Adv : public CDialog
{
    public:
	    Dg_HandLoads_Adv(C_Hom_Doc* mDocPtr,CWnd* pParent = NULL);

	    //{{AFX_DATA(Dg_HandLoads_Adv)
	    enum { IDD = IDD_HAND_LOADS_ADV };
        double mLeftForce[3];
        double mRightForce[3];
		double mdLeftForce[3];
        double mdRightForce[3];
        double mLeftTorque[3];
        double mRightTorque[3];

        double mLeftMag;
        double mLeftTorqueMag;
        double mdLeftTorqueMag;
        double mRightMag;
        double mRightTorqueMag;
        double mdRightTorqueMag;
        double mdLeftMag;
        double mdRightMag;

        CString mForceUnits;
        CString mLeftForceText;
        CString mRightForceText;
        CString mLeftTorqueUnits;
        CString mRightTorqueUnits;
	    //}}AFX_DATA

        C_Hom_Doc *mDocPtr;   

    protected:
	    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        void UpdateDocument();

        void RecalcRightTotal();
        void RecalcLeftTotal();

	    //SB 10/25/07 We should be displaying only two decimal points, so we 
		// redisplay
        void RedisplayRightForces();
		void RedisplayLeftForces();


        bool VerifyTotalsInRange();

        // Generated message map functions
	    //{{AFX_MSG(Dg_HandLoads_Adv)
	    afx_msg void OnKillfocusLxfrc();
	    afx_msg void OnKillfocusLyfrc();
	    afx_msg void OnKillfocusLzfrc();
	    afx_msg void OnKillfocusRxfrc();
	    afx_msg void OnKillfocusRyfrc();
	    afx_msg void OnKillfocusRzfrc();
	    afx_msg void OnApply();
	    virtual void OnOK();
	    afx_msg void OnZeroAll();
	    //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()

    public:
	    afx_msg void OnEnKillfocusLxtrq();
	    afx_msg void OnEnKillfocusLytrq();
	    afx_msg void OnEnKillfocusLztrq();
	    afx_msg void OnEnKillfocusRxtrq();
	    afx_msg void OnEnKillfocusRytrq();
	    afx_msg void OnEnKillfocusRztrq();
};

#endif
