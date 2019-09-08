#pragma once


enum ONLINE_STATS
{
	OS_UNKNOWN = -1,
	OS_ONLINE,
	OS_OFFLINE,
	OS_BUSY,
	OS_AWAY
};

class FRIENDSITEM 
{
public:
	FRIENDSITEM()
	{
		m_pWndChat = NULL;
		m_eOnlineState = OS_UNKNOWN;
		m_bHasMobile = FALSE;
		m_bHasEmail = FALSE;
	}
	~FRIENDSITEM()
	{
		if (m_pWndChat != NULL)
		{
			delete m_pWndChat;
			m_pWndChat = NULL;
		}
	}

public:

	// For draw
	CString m_strName;
	CString m_strSigned;
	CIUIImage m_hBitmapFace;
	ONLINE_STATS m_eOnlineState;

	BOOL m_bHasMobile;
	BOOL m_bHasEmail;

	// Chat window
	CWnd *m_pWndChat;
};

#define TVIF_ICON 0x00000001
#define TVIF_TEXT0 0x00000010
#define TVIF_TEXT1 0x00000100
#define TVIF_ALL TVIF_ICON|TVIF_TEXT0|TVIF_TEXT1

// CFriendsTree

class CFriendsTree : public CSkinTreeCtrl
{
public:
	CFriendsTree();
	virtual ~CFriendsTree();

protected:
	CRect GetMobileTrayRect(HTREEITEM hItem);
	CRect GetEmailTrayRect(HTREEITEM hItem);

public:
	// Set the face of friend
	int SetFriendFace(HTREEITEM hItem, CIUIImage &hFace);
	CIUIImage *GetFriendFace(HTREEITEM hItem) const;

	int SetSelectedFriendItemFaceBorder(CIUIImage &hFaceBorder);
	CIUIImage *GetSelectedFriendItemFaceBorder();

	int SetUnselectedFriendItemIconOffset(int nOffset);
	int GetUnselectedFriendItemIconOffset() const;

	int SetMobileTrayImage(CIUIImage &hMobile);
	CIUIImage *GetMobileTrayImage();

	int SetEmailTrayImage(CIUIImage &hMail);
	CIUIImage *GetEmailTrayImage();

	BOOL IsGroupEmpty(HTREEITEM hGroupItem);
	int DeleteGroup(HTREEITEM hGroupItem, BOOL bFailedWhenHasChildren=TRUE);
	int DeleteFriend(HTREEITEM hFriendItem);
	BOOL IsGroup(HTREEITEM hItem);

	int GetOnlineCount(HTREEITEM hGroupItem);

	int SetOnlineState(HTREEITEM hItem, ONLINE_STATS eOnlineState);
	ONLINE_STATS GetOnlineState(HTREEITEM hItem) const;

	int SetOnlineStateImage(ONLINE_STATS eOnlineState, CIUIImage &hStateIconSmall, CIUIImage &hStateIconBig);
	int GetOnlineStateImage(ONLINE_STATS eOnlineState, CIUIImage *phStateIconSmall, CIUIImage *phStateIconBig);

	/**
	nMask:
		TVIF_ICON,
		TVIF_TEXT0,
		TVIF_TEXT1,
		TVIF_TEXT2
	*/
	int ModifyUserProfile(HTREEITEM hItem, UINT nMask, LPCTSTR lpszImageFile, LPCTSTR lpszUserName, LPCTSTR lpszMail, LPCTSTR lpszUserState);

protected:
	// Draw the item background image, include group and friend item.
	virtual int DrawItemBk(CDC *pDC, HTREEITEM hItem);
	// Draw group item content, include icon and text.
	virtual int DrawGroupItem(CDC *pDC, HTREEITEM hItem, const CRect &rcItem, const CRect &rcText);
	// Draw friend item content, include icon and text.
	virtual int DrawFriendItem(CDC *pDC, HTREEITEM hItem, const CRect &rcItem, const CRect &rcText);
	virtual int DrawItem(CDC *pDC, HTREEITEM hItem, int nLeftMargin = 0);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHighlightChanged(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTreelistrmenuIitem0();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTreelistrmenuRename();
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnDrop(WPARAM pDropInfoClass, LPARAM lParam);
	afx_msg LRESULT OnDropEx(WPARAM pDropInfoClass, LPARAM lParam);
	afx_msg LRESULT OnDragOver(WPARAM pDropInfoClass,LPARAM lParam);
	afx_msg LRESULT OnDragScroll(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	CIUIImage m_hMobileTrayIcon;
	CIUIImage m_hEmailTrayIcon;

	CIUIOleDropTarget m_dropEx;
	CSize m_sizeTray;

	// Unselected friend item
	int m_nUnselectedFriendItemIconOffsetX;
	int m_nUnselectedFriendItemTextOffsetX;
	int m_nUnselectedFriendItemNameBetweenSignedInterval;
	CSize m_sizeUnselectedFriendItemIcon;

	// Selected friend item
	CIUIImage m_hSelectedFriendItemFaceBorder;
	int m_nSelectedFriendItemFaceOffsetX;
	CSize m_sizeSelectedFriendItemFaceBorder;
	CSize m_sizeSelectedFriendItemFace;

	int m_nSelectedFriendItemTextOffsetX;

	// For on-line state
	std::map<ONLINE_STATS, std::pair<CIUIImage, CIUIImage> > m_mapOnlineStateImage;
	CSize m_sizeUnselectedFriendItemState;
	CSize m_sizeSelectedFriendItemState;

	CFont m_fontNormal;

	// For timer
	HTREEITEM m_hSelectedItem;
	HTREEITEM m_hUnSelectedItem;
	int m_nTick;
};
