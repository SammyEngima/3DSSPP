// Dg_MOVIE_EXPORT.cpp : implementation file
//

#include "stdafx.h"
#include "Dg_MOVIE_EXPORT.h"
#include "afxdialogex.h"


// Dg_MOVIE_EXPORT dialog

IMPLEMENT_DYNAMIC(Dg_MOVIE_EXPORT, CDialog)

Dg_MOVIE_EXPORT::Dg_MOVIE_EXPORT(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_MOVIE_EXPORT::IDD, pParent)
	, m_option_group(3),flag(0)
{
	m_option_group = 3;
	flag = 0;               //initialization
}

Dg_MOVIE_EXPORT::~Dg_MOVIE_EXPORT()
{
}

void Dg_MOVIE_EXPORT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_OPTIONS, m_option_group);
}


BEGIN_MESSAGE_MAP(Dg_MOVIE_EXPORT, CDialog)
	ON_BN_CLICKED(IDOK, &Dg_MOVIE_EXPORT::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Dg_MOVIE_EXPORT::OnBnClickedCancel)
END_MESSAGE_MAP()


// Dg_MOVIE_EXPORT message handlers


void Dg_MOVIE_EXPORT::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	flag =1;
	CDialog::OnOK();
}


void Dg_MOVIE_EXPORT::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	flag = 0;
	CDialog::OnCancel();
}
