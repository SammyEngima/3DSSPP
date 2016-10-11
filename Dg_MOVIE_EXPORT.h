#ifndef Dg_MOVIE_EXPORT
#define Dg_MOVIE_EXPROT
#include "afxwin.h"
#include "resource.h"
// Dg_MOVIE_EXPORT dialog
#pragma once
class Dg_MOVIE_EXPORT : public CDialog
{
	DECLARE_DYNAMIC(Dg_MOVIE_EXPORT)

public:
	Dg_MOVIE_EXPORT(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dg_MOVIE_EXPORT();

// Dialog Data
	enum { IDD = IDD_MOVIE_EXPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_option_group;
	int flag;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
#endif
