// ViewComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "ViewComboBox.h"


// CViewComboBox

IMPLEMENT_DYNAMIC(CViewComboBox, CComboBox)

CViewComboBox::CViewComboBox()
{
}

CViewComboBox::~CViewComboBox()
{
}


BEGIN_MESSAGE_MAP(CViewComboBox, CComboBox)
END_MESSAGE_MAP()

void CViewComboBox::Initialize() {
	AddString("ObliqueView");
	AddString("TopView");
	AddString("Front View");
	AddString("Side View");
	SetCurSel(0);
}


// CViewComboBox message handlers


//void CViewComboBox::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
//{
//	this->BringWindowToTop();
//}
