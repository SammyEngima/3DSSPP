#if !defined(AFX_TESTCOMDOC_H__8F41A65D_F62A_11D1_9480_000000000000__INCLUDED_)
#define AFX_TESTCOMDOC_H__8F41A65D_F62A_11D1_9480_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TestCOMDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TestCOMDoc document

class TestCOMDoc : public CDocument
{
protected:
	TestCOMDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(TestCOMDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TestCOMDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~TestCOMDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(TestCOMDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTCOMDOC_H__8F41A65D_F62A_11D1_9480_000000000000__INCLUDED_)
