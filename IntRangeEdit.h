#pragma once
#include "intedit.h"

class CIntRangeEdit : public CIntEdit {
public:
	CIntRangeEdit(int dialogID);
	CIntRangeEdit(int dialogID, int min, int max);
	virtual ~CIntRangeEdit(void);

	void SetMin(int min);
	void SetMax(int max);

	virtual bool Validate();

private:
	int min_val, max_val;
};
