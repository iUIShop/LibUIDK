// StartMenu.h : interface of the CStartMenu class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STARTMENU_H__D5D326C3_ED07_4525_9031_D9E6F1CEACBD__INCLUDED_)
#define AFX_STARTMENU_H__D5D326C3_ED07_4525_9031_D9E6F1CEACBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStartMenu : public CMenuWnd
{
public:
	CStartMenu();
	virtual ~CStartMenu();
	void WindowID() { IDD = IDW_START_MENU; }

protected:
	int InitControls();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStartMenu)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CStartMenu)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// 

#endif // !defined(AFX_STARTMENU_H__D5D326C3_ED07_4525_9031_D9E6F1CEACBD__INCLUDED_)
