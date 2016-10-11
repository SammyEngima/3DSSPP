// Dg_HWMeans.cpp : implementation file
//

#include "stdafx.h"
#include "hom.h"
#include "Dg_HWMeans.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dg_HWMeans dialog


Dg_HWMeans::Dg_HWMeans(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_HWMeans::IDD, pParent)
{
	//{{AFX_DATA_INIT(Dg_HWMeans)
	mFHmean = 0.0f;
	mFWmean = 0.0f;
	mMWmean = 0.0f;
	mMHmean = 0.0f;
	mFHMax = 0.0f;
	mFHMin = 0.0f;
	mFWMax = 0.0f;
	mFWMin = 0.0f;
	mMHMax = 0.0f;
	mMHMin = 0.0f;
	mMWMax = 0.0f;
	mMWMin = 0.0f;
	//}}AFX_DATA_INIT
	mFHstdev = 0.0f;
	mFWstdev = 0.0f;
	mMHstdev = 0.0f;
	mMWstdev = 0.0f;
}


void Dg_HWMeans::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dg_HWMeans)
	DDX_Text(pDX, IDC_Fheight, mFHmean);
	DDX_Text(pDX, IDC_Fweight, mFWmean);
	DDX_Text(pDX, IDC_Mweight, mMWmean);
	DDX_Text(pDX, IDC_Mheight, mMHmean);
	DDX_Text(pDX, IDC_FHMax, mFHMax);
	DDX_Text(pDX, IDC_FHMin, mFHMin);
	DDX_Text(pDX, IDC_FWMax, mFWMax);
	DDX_Text(pDX, IDC_FWMin, mFWMin);
	DDX_Text(pDX, IDC_MHMax, mMHMax);
	DDX_Text(pDX, IDC_MHMin, mMHMin);
	DDX_Text(pDX, IDC_MWMax, mMWMax);
	DDX_Text(pDX, IDC_MWMin, mMWMin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Dg_HWMeans, CDialog)
	//{{AFX_MSG_MAP(Dg_HWMeans)
	/*ON_EN_UPDATE(IDC_Fheight, OnValueChanged)
	ON_EN_UPDATE(IDC_FHstdev, OnValueChanged)
	ON_EN_UPDATE(IDC_Fweight, OnValueChanged)
	ON_EN_UPDATE(IDC_FWstdev, OnValueChanged)
	ON_EN_UPDATE(IDC_Mheight, OnValueChanged)
	ON_EN_UPDATE(IDC_MHstdev, OnValueChanged)
	ON_EN_UPDATE(IDC_Mweight, OnValueChanged)
	ON_EN_UPDATE(IDC_MWstdev, OnValueChanged)*/
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dg_HWMeans message handlers
void Dg_HWMeans::RecalculatePercentiles()
{
#if (USE_POPULATION_DISTRIBUTION)
	mMHMin = (mMHstdev * -1.644853) + mMHmean;
	mMHMax = (mMHstdev *  1.644853) + mMHmean;
	mMWMin = (mMWstdev * -1.644853) + mMWmean;
	mMWMax = (mMWstdev *  1.644853) + mMWmean;
	mFHMin = (mFHstdev * -1.644853) + mFHmean;
	mFHMax = (mFHstdev *  1.644853) + mFHmean;
	mFWMin = (mFWstdev * -1.644853) + mFWmean;
	mFWMax = (mFWstdev *  1.644853) + mFWmean;

#endif

}
void Dg_HWMeans::OnValueChanged() 
{
	this->UpdateData(TRUE);
	this->RecalculatePercentiles();
	this->UpdateData(FALSE);
}

