#include "stdafx.h"
#include "FloatEdit.h"

CFloatEdit::CFloatEdit(int dialogID) : CValidEdit(dialogID)
{
}

CFloatEdit::~CFloatEdit(void)
{
}

bool CFloatEdit::Validate() {
	CString text;
	this->GetWindowTextA(text);
	text.Trim();
	// empty string
	if(text.IsEmpty()) {
		return false;
	}
	// there is at least one number
	if(text.FindOneOf("0123456789") == -1) {
		return false;
	}
	// character besides digits or period?
	if(text.SpanIncluding("0123456789.-") != text) {
		return false;
	}
	// minus sign is not only in the first spot. implies only one minus sign
	if(text.ReverseFind('-') > 0) {
		return false;
	}
	// more than one period?
	if(text.Remove('.') > 1) {
		return false;
	}
	return true;
}