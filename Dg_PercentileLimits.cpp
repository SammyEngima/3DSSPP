// Dg_PercentileLimits.cpp : implementation file
//

#include "stdafx.h"
#include "Dg_PercentileLimits.h"
#include "hom.h"
#include "hom_doc.h" // Necessary
#include "Hom_doc.h"
#include "Skeleton.hpp"


// Dg_PercentileLimits dialog

IMPLEMENT_DYNAMIC(Dg_PercentileLimits, CDialog)

Dg_PercentileLimits::Dg_PercentileLimits(CWnd* pParent /*=NULL*/)
	: CDialog(Dg_PercentileLimits::IDD, pParent)
	, lowerPercentileLimit(0)
	, upperPercentileLimit(0)
	, femaleLowerPercentileLimit(0)
	, femaleUpperPercentileLimit(0)
{
}

Dg_PercentileLimits::~Dg_PercentileLimits()
{
}

void Dg_PercentileLimits::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_NIOSH, radioChoice);
	DDX_Text(pDX, IDC_LOWER_LIMIT, lowerPercentileLimit);
	DDX_Text(pDX, IDC_UPPER_LIMIT, upperPercentileLimit);

	DDX_Text(pDX, IDC_LOWER_LIMIT_FEMALE, femaleLowerPercentileLimit);
	DDX_Text(pDX, IDC_UPPER_LIMIT_FEMALE, femaleUpperPercentileLimit);
	DDV_MinMaxInt(pDX, lowerPercentileLimit, 1, 99);
	DDV_MinMaxInt(pDX, upperPercentileLimit, 1, 99);
	DDV_MinMaxInt(pDX, femaleUpperPercentileLimit, 1, 99);
	DDV_MinMaxInt(pDX, femaleLowerPercentileLimit, 1, 99);
}


BEGIN_MESSAGE_MAP(Dg_PercentileLimits, CDialog)
	ON_BN_CLICKED(IDOK, &Dg_PercentileLimits::OnBnClickedOk)
	ON_BN_CLICKED(IDC_APPLY, &Dg_PercentileLimits::OnBnClickedApply)
	ON_BN_CLICKED(IDC_RADIO_NIOSH, &Dg_PercentileLimits::OnBnClickedRadioNiosh)
    ON_BN_CLICKED(IDC_RADIO_CUSTOM, &Dg_PercentileLimits::OnBnClickedRadioCustom)
	ON_EN_CHANGE(IDC_LOWER_LIMIT, &Dg_PercentileLimits::OnEnChangeLowerLimit)
	ON_EN_CHANGE(IDC_UPPER_LIMIT, &Dg_PercentileLimits::OnEnChangeUpperLimit)
	ON_EN_CHANGE(IDC_LOWER_LIMIT_FEMALE, &Dg_PercentileLimits::OnEnChangeLowerLimitFemale)
	ON_EN_CHANGE(IDC_UPPER_LIMIT_FEMALE, &Dg_PercentileLimits::OnEnChangeUpperLimitFemale)
	ON_EN_SETFOCUS(IDC_LOWER_LIMIT, &Dg_PercentileLimits::OnEnSetfocusLowerLimit)
	ON_EN_SETFOCUS(IDC_UPPER_LIMIT, &Dg_PercentileLimits::OnEnSetfocusUpperLimit)
	ON_EN_SETFOCUS(IDC_LOWER_LIMIT_FEMALE, &Dg_PercentileLimits::OnEnSetfocusLowerLimitFemale)
	ON_EN_SETFOCUS(IDC_UPPER_LIMIT_FEMALE, &Dg_PercentileLimits::OnEnSetfocusUpperLimitFemale)
END_MESSAGE_MAP()


// Dg_PercentileLimits message handlers

void Dg_PercentileLimits::OnBnClickedOk()
{
	this->UpdateData(true);
    UpdateDocument();
    OnOK();
}

void Dg_PercentileLimits::OnBnClickedApply()
{
	
    if(this->UpdateData(TRUE))
        this->UpdateDocument();
	Dg_PercentileLimits::DisableFieldsAsNecessary();

}


void Dg_PercentileLimits::UpdateDocument() {

    C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
	doc->setLimitChoice((LimitChoice)radioChoice);
	doc->setLowerPercentileLimit(lowerPercentileLimit);
	doc->setUpperPercentileLimit(upperPercentileLimit);
	doc->setFemaleLowerPercentileLimit(femaleLowerPercentileLimit);
	doc->setFemaleUpperPercentileLimit(femaleUpperPercentileLimit);
	doc->MakeDirtyAndUpdateViews();
}

void Dg_PercentileLimits::OnBnClickedRadioNiosh()
{
	Dg_PercentileLimits::DisableFieldsAsNecessary();

	UpdateData(true);
	// Skeleton* skel = Get_App_Ptr()->Get_Document_Ptr()->GetSkeleton();
	// C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();

	
	C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();
	Skeleton &lSkeleton = *doc->GetSkeleton();
	AnalysisSummaryLimits maleLimits = lSkeleton.getAnalysisSummaryLimitsByGender(S_Male);
	AnalysisSummaryLimits femaleLimits = lSkeleton.getAnalysisSummaryLimitsByGender(S_Female);

	lowerPercentileLimit = maleLimits.SUL;
    upperPercentileLimit = maleLimits.SDL;
    femaleLowerPercentileLimit = femaleLimits.SUL;
	femaleUpperPercentileLimit = femaleLimits.SDL;
	UpdateData(false);

}



void Dg_PercentileLimits::OnBnClickedRadioCustom()
{
	Dg_PercentileLimits::DisableFieldsAsNecessary();

}

void Dg_PercentileLimits::DisableFieldsAsNecessary()
{
   UpdateData(true);
   switch ((LimitChoice)radioChoice) {
	 case LC_NIOSH:
        GetDlgItem(IDC_LOWER_LIMIT)->EnableWindow(TRUE);
        GetDlgItem(IDC_UPPER_LIMIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_LOWER_LIMIT_FEMALE)->EnableWindow(TRUE);
        GetDlgItem(IDC_UPPER_LIMIT_FEMALE)->EnableWindow(TRUE);
		break;

     case LC_CUSTOM:
        GetDlgItem(IDC_LOWER_LIMIT)->EnableWindow(TRUE);
        GetDlgItem(IDC_UPPER_LIMIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_LOWER_LIMIT_FEMALE)->EnableWindow(TRUE);
        GetDlgItem(IDC_UPPER_LIMIT_FEMALE)->EnableWindow(TRUE);
		break;
   }
}

BOOL Dg_PercentileLimits::OnInitDialog()
{
	CDialog::OnInitDialog();

	C_Hom_Doc* doc = Get_App_Ptr()->Get_Document_Ptr();

	Skeleton &lSkeleton = *doc->GetSkeleton();
	AnalysisSummaryLimits maleLimits = lSkeleton.getAnalysisSummaryLimitsByGender(S_Male);
	AnalysisSummaryLimits femaleLimits = lSkeleton.getAnalysisSummaryLimitsByGender(S_Female);

	if(doc->getLimitChoice() == LC_NIOSH) {
		radioChoice = LC_NIOSH;
	    lowerPercentileLimit = maleLimits.SUL;
		upperPercentileLimit = maleLimits.SDL;
		femaleLowerPercentileLimit = femaleLimits.SUL;
		femaleUpperPercentileLimit = femaleLimits.SDL;
	} else {
		radioChoice = LC_CUSTOM;
		lowerPercentileLimit = doc->getLowerPercentileLimit();
		upperPercentileLimit = doc->getUpperPercentileLimit();
		femaleLowerPercentileLimit = doc->getFemaleLowerPercentileLimit();
        femaleUpperPercentileLimit = doc->getFemaleUpperPercentileLimit();
		
	
	}

	UpdateData(false);

	DisableFieldsAsNecessary();

	// If the user has customized the field values but these user-inout values
	// are exactly the same as NIOSH values, then the NIOSH radiobutton ought to 
	// be set. That's what the following lines of code do.

	if(doc->getLimitChoice() == LC_CUSTOM) {
		if ((lowerPercentileLimit == maleLimits.SUL) && 
		     (upperPercentileLimit == maleLimits.SDL) && 
             (femaleLowerPercentileLimit == femaleLimits.SUL) &&
             (femaleUpperPercentileLimit == femaleLimits.SDL))
		{
			this->CheckRadioButton(IDC_RADIO_CUSTOM, IDC_RADIO_NIOSH, IDC_RADIO_NIOSH);

		}
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void Dg_PercentileLimits::OnEnChangeLowerLimit()
{
    this->CheckRadioButton(IDC_RADIO_CUSTOM, IDC_RADIO_NIOSH, IDC_RADIO_CUSTOM);
}


void Dg_PercentileLimits::OnEnChangeUpperLimit()
{
	
    this->CheckRadioButton(IDC_RADIO_CUSTOM, IDC_RADIO_NIOSH, IDC_RADIO_CUSTOM);
}

void Dg_PercentileLimits::OnEnChangeLowerLimitFemale()
{
	
    this->CheckRadioButton(IDC_RADIO_CUSTOM, IDC_RADIO_NIOSH, IDC_RADIO_CUSTOM);
}

void Dg_PercentileLimits::OnEnChangeUpperLimitFemale()
{
    this->CheckRadioButton(IDC_RADIO_CUSTOM, IDC_RADIO_NIOSH, IDC_RADIO_CUSTOM);
}

void Dg_PercentileLimits::OnEnSetfocusLowerLimit()
{
	this->CheckRadioButton(IDC_RADIO_CUSTOM, IDC_RADIO_NIOSH, IDC_RADIO_CUSTOM);
}

void Dg_PercentileLimits::OnEnSetfocusUpperLimit()
{
	this->CheckRadioButton(IDC_RADIO_CUSTOM, IDC_RADIO_NIOSH, IDC_RADIO_CUSTOM);
}

void Dg_PercentileLimits::OnEnSetfocusLowerLimitFemale()
{
	this->CheckRadioButton(IDC_RADIO_CUSTOM, IDC_RADIO_NIOSH, IDC_RADIO_CUSTOM);
}

void Dg_PercentileLimits::OnEnSetfocusUpperLimitFemale()
{
	this->CheckRadioButton(IDC_RADIO_CUSTOM, IDC_RADIO_NIOSH, IDC_RADIO_CUSTOM);
}
