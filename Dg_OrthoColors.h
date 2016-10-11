#ifndef DG_ORTHOCOLORS_H
#define DG_ORTHOCOLORS_H

#pragma once

class Dg_OrthoColors : public CDialog
{
	DECLARE_DYNAMIC(Dg_OrthoColors)

public:
	Dg_OrthoColors(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dg_OrthoColors();

// Dialog Data
	enum { IDD = IDD_ORTHO_COLORS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	C_Hom_Doc*	pDoc;

	// Colors
	unsigned char backgroundColor[3];
	unsigned char forceColor[3];
	unsigned char floorFillColor[3];
	unsigned char floorOutlineColor[3];
	unsigned char linkColor[3];
	unsigned char leftJointColor[3];
	unsigned char rightJointColor[3];
	// colors for new color scheme
	unsigned char jointColor[3];
	unsigned char leftLinkColor[3];
	unsigned char rightLinkColor[3];
	unsigned char rightfeetforceColor[3];
	unsigned char leftfeetforceColor[3];
	bool UpdateDocument();
	void FillRect(CPaintDC* dc, const int nIDDlgItem, BYTE& red, BYTE& green, BYTE& blue);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnStnClickedBgcolorprev();
	afx_msg void OnBnClickedFloorfilloutline();
	afx_msg void OnBnClickedFloorfillcolor();
	afx_msg void OnBnClickedForcecolor();
	afx_msg void OnBnClickedRightFeetForcecolor();
	afx_msg void OnBnClickedLeftFeetForcecolor();
	afx_msg void OnBnClickedBackgroundcolor();
	afx_msg void OnBnClickedFlooroutlinecolor();
	afx_msg void OnBnClickedLinkcolor();
	afx_msg void OnBnClickedLeftjointscolor();
	afx_msg void OnBnClickedRightjointscolor();
	afx_msg void OnBnClickedJointscolor();
	afx_msg void OnBnClickedLeftlinkcolor();
	afx_msg void OnBnClickedRightlinkcolor();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedOk();
	

	
};

#endif
