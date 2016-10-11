#include "stdafx.h"
#include "IntEdit.h"

CIntEdit::CIntEdit(int dialogID) : CValidEdit(dialogID)
{
}

CIntEdit::~CIntEdit(void)
{
}

bool CIntEdit::Validate() {
	CString text;
	this->GetWindowText(text);
	text.Trim();
	// string is non-empty
	if(text.IsEmpty()) {
		return false;
	}
	// all characters are digits or a minus sign
	if(text.SpanIncluding("0123456789-") != text) {
		return false;
	}
	// there is at least one number
	if(text.FindOneOf("0123456789") == -1) {
		return false;
	}
	// minus sign is not only in the first spot. implies only one minus sign
	if(text.ReverseFind('-') > 0) {
		return false;
	}
	return true;
}