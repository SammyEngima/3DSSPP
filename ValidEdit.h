#pragma once
#include "stdafx.h"
#include <map>
#include <set>

// CValidEdit

class CValidEdit : public CEdit
{
	DECLARE_DYNAMIC(CValidEdit)

public:
	CValidEdit(int dialogID);
	virtual ~CValidEdit();

	bool IsValid() const;

	static bool DialogHasInvalid(int dialogID);

protected:
	virtual bool Validate() = 0;
	bool valid;
	int dialogID;
	static std::map<int, std::set<int>> InvalidEdits;
	static CBrush redBrush;
	static COLORREF redColor;

	DECLARE_MESSAGE_MAP()
public:
	// should probably only do one of these. is OnEnChange sufficient?
	afx_msg BOOL OnEnChange();
	afx_msg BOOL OnEnKillfocus();
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg LRESULT OnSetText(WPARAM, LPARAM);
};
