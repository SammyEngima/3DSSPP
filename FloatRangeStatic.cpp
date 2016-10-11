#include "FloatRangeStatic.h"
#include <float.h>

CFloatRangeStatic::CFloatRangeStatic(int dialogID_in) : CValidStatic(dialogID_in) {
	min_val = -FLT_MAX;
	max_val = FLT_MAX;
}
CFloatRangeStatic::CFloatRangeStatic(int dialogID_in, float min_val_in, float max_val_in) : CValidStatic(dialogID_in) {
	min_val = min_val_in;
	max_val = max_val_in;
}

CFloatRangeStatic::~CFloatRangeStatic(void) {
}

void CFloatRangeStatic::SetMin(float min_val_in) {
	min_val = min_val_in;
}
void CFloatRangeStatic::SetMax(float max_val_in) {
	max_val = max_val_in;
}


bool CFloatRangeStatic::Validate() {
	CString text;
	this->GetWindowText(text);
	float value = atof(text);
	return (min_val <= value && value <= max_val);
}