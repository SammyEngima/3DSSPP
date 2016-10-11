// ValidEdit.cpp : implementation file
//

#include "stdafx.h"
#include "ValidEdit.h"
using std::map; using std::set;

COLORREF CValidEdit::redColor = RGB(255,200,200);
CBrush CValidEdit::redBrush(RGB(255,200,200));
map<int, set<int>> CValidEdit::InvalidEdits;

// CValidEdit

IMPLEMENT_DYNAMIC(CValidEdit, CEdit)

CValidEdit::CValidEdit(int dialogID_in)
{
	dialogID = dialogID_in;
}

CValidEdit::~CValidEdit()
{
}

bool CValidEdit::IsValid() const {
	return valid;
}

bool CValidEdit::DialogHasInvalid(int dialogID_in) {
	return !InvalidEdits[dialogID_in].empty();
}

BEGIN_MESSAGE_MAP(CValidEdit, CEdit)
	ON_CONTROL_REFLECT_EX(EN_KILLFOCUS, &CValidEdit::OnEnKillfocus)
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT_EX(EN_CHANGE, &CValidEdit::OnEnChange)
	ON_MESSAGE(WM_SETTEXT, &CValidEdit::OnSetText)
END_MESSAGE_MAP()



// CValidEdit message handlers

BOOL CValidEdit::OnEnChange()
{
	bool oldvalid = valid;
	valid = Validate();
	if(valid && !oldvalid) {
		InvalidEdits[dialogID].erase(this->GetDlgCtrlID());
	} else if(!valid && oldvalid) {
		InvalidEdits[dialogID].insert(this->GetDlgCtrlID());
	}
	if(oldvalid != valid)
		RedrawWindow();
	return FALSE;
}


BOOL CValidEdit::OnEnKillfocus()
{
	bool oldvalid = valid;
	valid = Validate();
	if(valid && !oldvalid) {
		InvalidEdits[dialogID].erase(this->GetDlgCtrlID());
	} else if(!valid && oldvalid) {
		InvalidEdits[dialogID].insert(this->GetDlgCtrlID());
	}
	if(oldvalid != valid)
		RedrawWindow();
	return FALSE;
}

HBRUSH CValidEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	if(!valid) {
		pDC->SetBkColor(redColor);
		return (HBRUSH) redBrush;
	}
	return NULL;
}

LRESULT CValidEdit::OnSetText(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = DefWindowProc(WM_SETTEXT, wParam, lParam);
	bool oldvalid = valid;
	valid = Validate();
	if(valid && !oldvalid) {
		InvalidEdits[dialogID].erase(this->GetDlgCtrlID());
	} else if(!valid && oldvalid) {
		InvalidEdits[dialogID].insert(this->GetDlgCtrlID());
	}
	if(oldvalid != valid)
		RedrawWindow();
	return lResult;
}