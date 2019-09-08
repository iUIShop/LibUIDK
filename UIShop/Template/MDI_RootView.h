#if !defined(AFX_$$ROOT$$VIEW_H__FBFBD861_BF91_4BB4_99D6_7EF9E29BE733__INCLUDED_)
#define AFX_$$ROOT$$VIEW_H__FBFBD861_BF91_4BB4_99D6_7EF9E29BE733__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// $$Root$$View.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C$$Root$$View window

class C$$Root$$View : public CIUIView
{
// Construction
public:
	C$$Root$$View();
	DECLARE_DYNCREATE(C$$Root$$View)
	void WindowID() { IDD = IDW_VIEW; }

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C$$Root$$View)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~C$$Root$$View();

	// Generated message map functions
protected:
	//{{AFX_MSG(C$$Root$$View)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_$$ROOT$$VIEW_H__FBFBD861_BF91_4BB4_99D6_7EF9E29BE733__INCLUDED_)
