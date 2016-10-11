// Dg_CameraOblique.h : header file
//
#if ! defined( Dg_CameraOblique_INCLUDE )
#define Dg_CameraOblique_INCLUDE

/////////////////////////////////////////////////////////////////////////////
// Dg_CameraOblique dialog

class Dg_CameraOblique : public CDialog
{
// Construction
public:
	Dg_CameraOblique
            (
               C_Hom_Doc *   aDocPtr ,
               CWnd* pParent = NULL
            );	// standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_CameraOblique)
	enum { IDD = IDD_CAMERA };
	int		mAltitude;
	int		mAzimuth;
	int		mDistance;
	int		mFocalLength;
	float		mHOffset;
	float		mVOffset;
	CString	mDistanceUnits;
	CString	mCam_HUnits;
	CString mCam_VUnits;
	//}}AFX_DATA
        
        // Custom attributes: Not associated with controls

        C_Hom_Doc *   mDocPtr;
        
        float mUnitsConversion;        
        
        int   mMinDistance;
        int   mMaxDistance;
        
// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Custom member functions

        void UpdateDocument( void );

        // Generated message map functions
	//{{AFX_MSG(Dg_CameraOblique)
	virtual void OnOK();
	afx_msg void OnRedrawButton();
	afx_msg void OnDefaultOblCam();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //End file guard
