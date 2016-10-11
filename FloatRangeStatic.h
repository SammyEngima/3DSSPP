#pragma once
#include "validstatic.h"

class CFloatRangeStatic : public CValidStatic {
public:
	CFloatRangeStatic(int dialogID_in);
	CFloatRangeStatic(int dialogID_in, float min_val_in, float max_val_in);
	virtual ~CFloatRangeStatic(void);

	
	void SetMin(float min_val);
	void SetMax(float max_val);

	virtual bool Validate();

protected:
	float min_val, max_val;
};
