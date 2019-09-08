#if !defined(AFX_$$ROOT$$DOC_H__1D414216_7EFD_48AF_BCF6_B957CBF9BCDA__INCLUDED_)
#define AFX_$$ROOT$$DOC_H__1D414216_7EFD_48AF_BCF6_B957CBF9BCDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// $$Root$$Doc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C$$Root$$Doc document

class C$$Root$$Doc : public CIUIDocument
{
protected:
	C$$Root$$Doc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(C$$Root$$Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C$$Root$$Doc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL CanCloseUIView(CIUIView* pUIView);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~C$$Root$$Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(C$$Root$$Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_$$ROOT$$DOC_H__1D414216_7EFD_48AF_BCF6_B957CBF9BCDA__INCLUDED_)
