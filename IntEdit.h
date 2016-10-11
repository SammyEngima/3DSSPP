#pragma once
#include "validedit.h"

class CIntEdit : public CValidEdit
{
public:
	CIntEdit(int dialogID);
	~CIntEdit(void);

	virtual bool Validate();
};
