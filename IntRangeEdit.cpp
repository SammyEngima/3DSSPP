#include "IntRangeEdit.h"

CIntRangeEdit::CIntRangeEdit(int dialogID)
	: CIntEdit(dialogID)
	, min_val(INT_MIN)
	, max_val(INT_MAX)
{
}

CIntRangeEdit::CIntRangeEdit(int dialogID, int min_in, int max_in) :
	CIntEdit(dialogID)
	,min_val(min_in)
	,max_val(max_in)
{
}

CIntRangeEdit::~CIntRangeEdit(void)
{
}

void CIntRangeEdit::SetMin(int min_in) {
	min_val = min_in;
}
void CIntRangeEdit::SetMax(int max_in) {
	max_val = max_in;
}

bool CIntRangeEdit::Validate() {
	if(!CIntEdit::Validate()) {
		return false;
	}
	CString text;
	this->GetWindowText(text);
	int value = atoi(text);
	return (min_val <= value && value <= max_val);
}