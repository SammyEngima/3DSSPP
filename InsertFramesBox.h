#pragma once

#include "resource.h"
// InsertFramesBox dialog

class AnimationBar;

class InsertFramesBox : public CDialog
{
	DECLARE_DYNAMIC(InsertFramesBox)

public:
	InsertFramesBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~InsertFramesBox();

	void setNumFrames(int numFrames);
	void setNumSeconds(double numSeconds);

// Dialog Data
	enum { IDD = IDD_INSERTFRAMES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	AnimationBar* animationBar;
	int mNumFrames;
	CString mNumSeconds;
	afx_msg void OnEnChangeInsertnumframes();
	afx_msg void OnEnChangeInsertnumseconds();
	afx_msg void OnBnClickedButton1();
protected:
	virtual void OnOK();
};
