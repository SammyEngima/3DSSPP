// dg_light.h : header file
//
#if ! defined( Dg_Light_INCLUDE )
#define Dg_Light_INCLUDE


/////////////////////////////////////////////////////////////////////////////
// Dg_Light dialog

class Dg_Light : public CDialog
{
   // Construction
public:
   Dg_Light // standard constructor
      (
      C_Hom_Doc* aHomDocPtr ,
      CWnd* pParent = NULL
      );	

   // Dialog Data
   //{{AFX_DATA(Dg_Light)
   enum { IDD = IDD_LIGHT };
   int m_FrontBack_Value;
   int m_LeftRight_Value;
   int m_TopBottom_Value;
   //}}AFX_DATA
   C_Hom_Doc* mDocPtr;

   // Implementation
protected:
   virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

   BOOL IsValid(void);

   // Generated message map functions
   //{{AFX_MSG(Dg_Light)
   virtual void OnOK();
   afx_msg void OnRedrawButton();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
public:
   int mIntensity;
   CString mIntensityPct;
   virtual BOOL OnInitDialog();
   void SetIntensityPctString();
   void CheckDisableSingleLightSource();
   afx_msg void OnNMCustomdrawIntensitySlider(NMHDR *pNMHDR, LRESULT *pResult);
   afx_msg void OnBnClickedDefaultLighting();
   BOOL lightRadio;
   afx_msg void OnBnClickedLightSingle();
   afx_msg void OnBnClickedLightThree();
};

#endif //End file guard
