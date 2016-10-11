#pragma once
#include "floatedit.h"

class CFloatRangeEdit : public CFloatEdit {
public:
	CFloatRangeEdit(int dialogID);
	CFloatRangeEdit(int dialogID, float min, float max);
	virtual ~CFloatRangeEdit(void);

	void SetMin(float min);
	void SetMax(float max);

	virtual bool Validate();

private:
	float min_val, max_val;
};
