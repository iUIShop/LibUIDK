// TaskWebWnd.h : interface of the CTaskWebWnd class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKWEBWND_H__AA865F75_7FE7_48F6_A2ED_AB7244EC4784__INCLUDED_)
#define AFX_TASKWEBWND_H__AA865F75_7FE7_48F6_A2ED_AB7244EC4784__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTaskWebWnd : public CUIWnd
{
public:
	CTaskWebWnd();
	virtual ~CTaskWebWnd();
	void WindowID() { IDD = IDW_WEB; }

protected:
	int InitControls();
	int InitHtml1();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskWebWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CTaskWebWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CHtmlCtrl *m_pHtml1;

public:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// 

#endif // !defined(AFX_TASKWEBWND_H__AA865F75_7FE7_48F6_A2ED_AB7244EC4784__INCLUDED_)
