#include "FloatRangeEdit.h"
#include <float.h>

CFloatRangeEdit::CFloatRangeEdit(int dialogID) 
	: CFloatEdit(dialogID)
	, min_val(-FLT_MAX)
	, max_val(FLT_MAX)
{
}

CFloatRangeEdit::CFloatRangeEdit(int dialogID, float min_in, float max_in)
	: CFloatEdit(dialogID)
	, min_val(min_in)
	, max_val(max_in)
{
}

CFloatRangeEdit::~CFloatRangeEdit(void)
{
}

void CFloatRangeEdit::SetMin(float min_in) {
	min_val = min_in;
}

void CFloatRangeEdit::SetMax(float max_in) {
	max_val = max_in;
}

bool CFloatRangeEdit::Validate() {
	if(!CFloatEdit::Validate()) {
		return false;
	}
	CString text;
	this->GetWindowText(text);
	float value = atof(text);
	return (min_val <= value && value <= max_val);
}