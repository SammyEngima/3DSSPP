#pragma once
#include "validedit.h"

class CFloatEdit :	public CValidEdit
{
public:
	CFloatEdit(int dialogID);
	virtual ~CFloatEdit(void);

	virtual bool Validate();
};
