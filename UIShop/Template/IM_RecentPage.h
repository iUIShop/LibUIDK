// RecentPage.h : interface of the CRecentPage class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECENTPAGE_H__783BA621_0A2B_4FF6_940F_9170A4C3D4E2__INCLUDED_)
#define AFX_RECENTPAGE_H__783BA621_0A2B_4FF6_940F_9170A4C3D4E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRecentPage : public CUIWnd
{
public:
	CRecentPage();
	virtual ~CRecentPage();
	void WindowID() { IDD = IDW_RECENT_PAGE; }

protected:
	int InitControls();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecentPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CRecentPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CSkinStatic *m_pStaRecent;

public:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// 

#endif // !defined(AFX_RECENTPAGE_H__783BA621_0A2B_4FF6_940F_9170A4C3D4E2__INCLUDED_)
