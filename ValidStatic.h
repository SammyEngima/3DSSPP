#pragma once
#include "stdafx.h"
#include <map>
#include <set>

// CValidStatic

class CValidStatic : public CStatic
{
	DECLARE_DYNAMIC(CValidStatic)

public:
	CValidStatic(int dialogID);
	virtual ~CValidStatic();

	bool IsValid() const;

protected:
	virtual bool Validate() = 0;
	bool valid;
	int dialogID;
	// TODO we don't maintain or use InvalidStatics yet, like we do for InvalidEdits in CValidEdit
	static std::map<int, std::set<int>> InvalidStatics;
	static CBrush redBrush;
	static COLORREF redColor;
	static CBrush defaultBrush;

	DECLARE_MESSAGE_MAP()

public:
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};


