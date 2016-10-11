#pragma once


// CViewComboBox

class CViewComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CViewComboBox)

public:
	CViewComboBox();
	virtual ~CViewComboBox();

	void Initialize();

protected:
	DECLARE_MESSAGE_MAP()
public:
//	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};
