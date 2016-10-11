#pragma once
#include "afxcmn.h"
#include "GLOblique.h"

class Dg_ObliqueBackground : public CDialog
{
    public:
        Dg_ObliqueBackground(CWnd* pParent = NULL);
        ~Dg_ObliqueBackground(void);

        virtual BOOL OnInitDialog();

        //{{AFX_DATA(Dg_ObliqueBackground)
        enum { IDD = IDD_OBLIQUE_BACKGROUND };
        //}}AFX_DATA

    //{{AFX_VIRTUAL(Dg_ObliqueBackground)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    protected:
        bool UpdateDocument(void);
        void DrawImage(CDC* pDC, CPoint  ptCenter, HWND hWnd, CRect clientRect);
        BOOL LoadPictureFile(LPCTSTR szFile, LPPICTURE* pgpPicture);

        C_Hom_Doc *lDocPtr;
        unsigned char mRGB[3];
        BOOL useBG, useCOL;
		BOOL picOrColorChoice;
        LPPICTURE picture;
        int mode;
        int alpha;

        CSliderCtrl alphaSlider;

		void DisableFieldsAsNecessary();

        //{{AFX_MSG(Dg_ObliqueBackground)
        afx_msg void OnOblColor();
        afx_msg void OnOblNewimage();
        virtual void OnCancel();
        virtual void OnOK();
        afx_msg void OnApply();
        afx_msg void OnPaint();
	    afx_msg void OnBnClickedUsimag();
	    afx_msg void OnBnClickedUscolor();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
