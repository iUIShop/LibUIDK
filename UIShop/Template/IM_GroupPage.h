// GroupPage.h : interface of the CGroupPage class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GROUPPAGE_H__72FCBF17_E777_4909_9764_8F1EAB6E0EA3__INCLUDED_)
#define AFX_GROUPPAGE_H__72FCBF17_E777_4909_9764_8F1EAB6E0EA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGroupPage : public CUIWnd
{
public:
	CGroupPage();
	virtual ~CGroupPage();
	void WindowID() { IDD = IDW_GROUP_PAGE; }

protected:
	int InitControls();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CGroupPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CSkinStatic *m_pStaGroup;

public:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// 

#endif // !defined(AFX_GROUPPAGE_H__72FCBF17_E777_4909_9764_8F1EAB6E0EA3__INCLUDED_)
