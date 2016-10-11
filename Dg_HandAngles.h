#pragma once


// Dg_HandAngles dialog
class C_Hom_Doc;

class Dg_HandAngles : public CDialog
{
	DECLARE_DYNAMIC(Dg_HandAngles)

public:
	Dg_HandAngles(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dg_HandAngles();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void UpdateDocument();
	DECLARE_MESSAGE_MAP()
private:
	C_Hom_Doc* pDoc;
	int thumbHandVert;
	int thumbHandHorz;
	int thumbLowerVert;
	int thumbLowerHorz;
	int thumbMiddleVert;
	int thumbMidHorz;
	int thumbUpperVert;
	int thumbUpperHorz;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedApply();
	afx_msg void OnPrintPrintwindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
