#ifndef DG_GRID_H
#define DG_GRID_H
#pragma once
#include "resource.h"
#include "GLSkeleton.h"
#include "afxwin.h"
// Dg_GRID dialog

class Dg_GRID : public CDialog
{
	DECLARE_DYNAMIC(Dg_GRID)

public:
	Dg_GRID(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dg_GRID();

// Dialog Data
	enum { IDD = IDD_GRID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	BOOL show;
	int GridSpacing;
	afx_msg void OnBnClickedGridsize10();
	afx_msg void OnBnClickedGridsize20();
	afx_msg void OnBnClickedGridsize30();
	afx_msg void OnBnClickedGridsizeCustom();
	double CustomSize;
	virtual BOOL OnInitDialog();
	unsigned char GridColor[3];
	afx_msg void OnBnClickedGridbutton();
	afx_msg void OnPaint();
	void FillRect(CPaintDC* dc, const int nIDDlgItem, BYTE& red, BYTE& green, BYTE& blue);
	CString units;
	CString units1;
	CString units2;
	CString units3;
	bool flag;
	bool flag1;
	afx_msg void OnBnClickedApplay();
};
#endif
