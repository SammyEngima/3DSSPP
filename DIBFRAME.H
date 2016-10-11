// dibframe.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMDIDibChildFrame frame

class CMDIDibChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CMDIDibChildFrame)
protected:
	CMDIDibChildFrame();			// protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CMDIDibChildFrame();
	virtual BOOL   PreCreateWindow( CREATESTRUCT&   CS );

	// Generated message map functions
	//{{AFX_MSG(CMDIDibChildFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
