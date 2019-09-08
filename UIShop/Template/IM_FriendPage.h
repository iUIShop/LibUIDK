// FriendPage.h : interface of the CFriendPage class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRIENDPAGE_H__170637C4_8670_4A92_BCF6_3D0D6EB94DCE__INCLUDED_)
#define AFX_FRIENDPAGE_H__170637C4_8670_4A92_BCF6_3D0D6EB94DCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "FriendsTree.h"


class CFriendPage : public CUIWnd
{
public:
	CFriendPage();
	virtual ~CFriendPage();
	void WindowID() { IDD = IDW_FRIEND_PAGE; }

protected:
	int InitControls();
	int InitTreeFriend();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFriendPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CFriendPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMDblclkFriendTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CFriendsTree m_treeFriend;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// 

#endif // !defined(AFX_FRIENDPAGE_H__170637C4_8670_4A92_BCF6_3D0D6EB94DCE__INCLUDED_)
