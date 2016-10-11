// Dg_AC_Conversation.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dg_AC_Conversation dialog

class Dg_AC_Conversation : public CDialog
{
// Construction
public:
	Dg_AC_Conversation(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Dg_AC_Conversation)
	enum { IDD = IDD_ACConversation };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Dg_AC_Conversation)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Dg_AC_Conversation)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
