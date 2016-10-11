// ValidStatic.cpp : implementation file
//

#include "stdafx.h"
#include "Hom.h"
#include "Hom_Doc.h"
#include "ValidStatic.h"


COLORREF CValidStatic::redColor = RGB(255,0,0);
CBrush CValidStatic::redBrush(RGB(255,0,0));
CBrush CValidStatic::defaultBrush(GetSysColor(COLOR_3DFACE));
std::map<int, std::set<int>> CValidStatic::InvalidStatics;

// CValidStatic

IMPLEMENT_DYNAMIC(CValidStatic, CStatic)

CValidStatic::CValidStatic(int dialogID_in) {
	this->dialogID = dialogID_in;
	//redBrush.CreateSolidBrush(RGB(255,0,255));
}

CValidStatic::~CValidStatic() {
}

bool CValidStatic::IsValid() const {
	return valid;
}

BEGIN_MESSAGE_MAP(CValidStatic, CStatic)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

// CValidStatic message handlers

HBRUSH CValidStatic::CtlColor(CDC* pDC, UINT nCtlColor)
{

	pDC->SetBkMode(TRANSPARENT);
	if(!Validate()) {
		pDC->SetTextColor(redColor);
	}
	//return CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	return defaultBrush;
}
