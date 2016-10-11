#pragma once


// CDummyFrame frame

class CDummyFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CDummyFrame)
protected:
	CDummyFrame();           // protected constructor used by dynamic creation
	virtual ~CDummyFrame();

protected:
	DECLARE_MESSAGE_MAP()
};


