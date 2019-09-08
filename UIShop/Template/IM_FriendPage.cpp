// FriendPage.cpp : implementation of the CFriendPage class
//

#include "stdafx.h"
#include "$$root$$.h"
#include "FriendPage.h"
#include "ChatFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define IDC_TREECTRL_FRIEND						1



/////////////////////////////////////////////////////////////////////////////
// CFriendPage

BEGIN_MESSAGE_MAP(CFriendPage, CUIWnd)
	//{{AFX_MSG_MAP(CFriendPage)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLKEX, IDC_TREECTRL_FRIEND, OnNMDblclkFriendTree)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFriendPage construction/destruction

CFriendPage::CFriendPage()
{
}

CFriendPage::~CFriendPage()
{
}

void CFriendPage::DoDataExchange(CDataExchange* pDX)
{
	CUIWnd::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CFriendPage)
	//}}AFX_DATA_MAP
}


//////////////////////////////////////////////////////////////////////////
// protected

int CFriendPage::InitControls()
{
	return 0;
}

int CFriendPage::InitTreeFriend()
{
	BOOL bRet = m_treeFriend.Create(WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_TREECTRL_FRIEND);
	if (!bRet)
		return -1;

	m_treeFriend.BindStyle(_T("IDC_TREECTRL_FRIEND"));
	m_treeFriend.SetItemHeight(1);

	CIUIImage hSelectedFaceBk;
	hSelectedFaceBk.AutoLoad(_T("Icon\\FaceBorder.png"));
	m_treeFriend.SetSelectedFriendItemFaceBorder(hSelectedFaceBk);

	CIUIImage hOnlineStateBusySmall;
	hOnlineStateBusySmall.AutoLoad(_T("Icon\\OnlineState_Busy.png"));
	CIUIImage hOnlineStateBusyBig;
	hOnlineStateBusyBig.AutoLoad(_T("Icon\\OnlineState_Busy_Big.png"));
	m_treeFriend.SetOnlineStateImage(OS_BUSY, hOnlineStateBusySmall, hOnlineStateBusyBig);
	CIUIImage hOnlineStateAwaySmall;
	hOnlineStateAwaySmall.AutoLoad(_T("Icon\\OnlineState_Away.png"));
	CIUIImage hOnlineStateAwayBig;
	hOnlineStateAwayBig.AutoLoad(_T("Icon\\OnlineState_Away_Big.png"));
	m_treeFriend.SetOnlineStateImage(OS_AWAY, hOnlineStateAwaySmall, hOnlineStateAwayBig);

	CIUIImage hMobile;
	hMobile.AutoLoad(_T("Icon\\MobileTray.png"));
	m_treeFriend.SetMobileTrayImage(hMobile);
	CIUIImage hEmail;
	hEmail.AutoLoad(_T("Icon\\EmailTray.png"));
	m_treeFriend.SetEmailTrayImage(hEmail);

	HTREEITEM hGroup = m_treeFriend.InsertItem(_T("我的好友"));
	m_treeFriend.SetItemHeightEx(hGroup, 29);
	{
		HTREEITEM hItem = m_treeFriend.InsertItem(_T(""), hGroup);	// Don't set text for item to refuse horizontal scroll bar appear.
		m_treeFriend.SetItemHeightEx(hItem, 29);
		FRIENDSITEM *pItem0 = new FRIENDSITEM;
		pItem0->m_strName = _T("LibUIDK");
		pItem0->m_strSigned = _T("基于下一代界面技术：DirectHWND");
		pItem0->m_hBitmapFace.AutoLoad(_T("Icon\\1.png"));
		pItem0->m_eOnlineState = OS_BUSY;
		pItem0->m_bHasMobile = TRUE;
		pItem0->m_bHasEmail = TRUE;
		m_treeFriend.SetItemData(hItem, (DWORD_PTR)pItem0);

		hItem = m_treeFriend.InsertItem(_T(""), hGroup);
		m_treeFriend.SetItemHeightEx(hItem, 29);
		FRIENDSITEM *pItem1 = new FRIENDSITEM;
		pItem1->m_strName = _T("iUIShop");
		pItem1->m_strSigned = _T("北京优艾斯普科技有限公司");
		pItem1->m_hBitmapFace.AutoLoad(_T("Icon\\2.png"));
		pItem1->m_eOnlineState = OS_ONLINE;
		pItem1->m_bHasEmail = TRUE;
		m_treeFriend.SetItemData(hItem, (DWORD_PTR)pItem1);

		hItem = m_treeFriend.InsertItem(_T(""), hGroup);
		m_treeFriend.SetItemHeightEx(hItem, 29);
		FRIENDSITEM *pItem2 = new FRIENDSITEM;
		pItem2->m_strName = _T("特点");
		pItem2->m_strSigned = _T("零学习成本、零维护成本、零依赖、所见即所得");
		pItem2->m_hBitmapFace.AutoLoad(_T("Icon\\4.png"));
		m_treeFriend.SetItemData(hItem, (DWORD_PTR)pItem2);
		m_treeFriend.SetOnlineState(hItem, OS_AWAY);

		hItem = m_treeFriend.InsertItem(_T(""), hGroup);
		m_treeFriend.SetItemHeightEx(hItem, 29);
		FRIENDSITEM *pItem3 = new FRIENDSITEM;
		pItem3->m_strName = _T("联系方式");
		pItem3->m_strSigned = _T("www.iuishop.com");
		pItem3->m_eOnlineState = OS_ONLINE;
		pItem3->m_hBitmapFace.AutoLoad(_T("Icon\\3.png"));
		m_treeFriend.SetItemData(hItem, (DWORD_PTR)pItem3);

		hItem = m_treeFriend.InsertItem(_T(""), hGroup);
		m_treeFriend.SetItemHeightEx(hItem, 29);
		FRIENDSITEM *pItem4 = new FRIENDSITEM;
		pItem4->m_strName = _T("QQ");
		pItem4->m_strSigned = _T("1584793892");
		pItem4->m_eOnlineState = OS_OFFLINE;
		pItem4->m_hBitmapFace.AutoLoad(_T("Icon\\5.png"));
		m_treeFriend.SetItemData(hItem, (DWORD_PTR)pItem4);
	}
	m_treeFriend.Expand(hGroup, TVE_EXPAND);
/*
	HTREEITEM hGroup2 = m_treeFriend.InsertItem(_T("陌生人"));
	m_treeFriend.SetItemHeightEx(hGroup2, 29);
	for (int i = 0; i < 10 ; i++)
	{
		HTREEITEM hItem = m_treeFriend.InsertItem(_T(""), hGroup2);
		m_treeFriend.SetItemHeightEx(hItem, 29);
		FRIENDSITEM *pItem2 = new FRIENDSITEM;
		pItem2->m_strName = "超越梦想";
		pItem2->m_strSigned = "伦敦看奥运";
		pItem2->m_hBitmapFace.AutoLoad(_T("Icon\\6.png"));
		m_treeFriend.SetItemData(hItem, (DWORD)pItem2);

		hItem = m_treeFriend.InsertItem(_T("遗失"), hGroup2);
		m_treeFriend.SetItemHeightEx(hItem, 29);
		pItem2 = new FRIENDSITEM;
		pItem2->m_strName = "Josh";
		pItem2->m_strSigned = "面朝大海，春暖花开";
		pItem2->m_hBitmapFace.AutoLoad(_T("Icon\\7.png"));
		m_treeFriend.SetItemData(hItem, (DWORD)pItem2);

		hItem = m_treeFriend.InsertItem(_T("八味"), hGroup2);
		m_treeFriend.SetItemHeightEx(hItem, 29);
		pItem2 = new FRIENDSITEM;
		pItem2->m_strName = "激光";
		pItem2->m_strSigned = "让暴雨来的更猛烈些吧！";
		pItem2->hBitmapHeader.AutoLoad(_T("Icon\\8.png"));
		m_treeFriend.SetItemData(hItem, (DWORD)pItem2);
	}
	m_treeFriend.Expand(hGroup2, TVE_EXPAND);
*/
	m_treeFriend.EnsureVisible(hGroup);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CFriendPage message handlers

int CFriendPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (InitControls() != 0)
		return -1;

	InitTreeFriend();

	return 0;
}

void CFriendPage::OnSize(UINT nType, int cx, int cy)
{
	CUIWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_treeFriend.GetSafeHwnd() != NULL)
	{
		CRect rcClientArea;
		GetClientAreaRect(rcClientArea);

		m_treeFriend.MoveWindow(rcClientArea);
	}
}

void CFriendPage::OnNMDblclkFriendTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	HTREEITEM hItem = m_treeFriend.GetSelectedItem();
	if (hItem == NULL)
		return;

	BOOL bGroup = m_treeFriend.IsGroup(hItem);
	if (bGroup)
		return;

	FRIENDSITEM *pItem = (FRIENDSITEM *)m_treeFriend.GetItemData(hItem);
	ASSERT(pItem != NULL);
	if (pItem->m_pWndChat->GetSafeHwnd() == NULL)
	{
		CChatFrame *pWndChat = new CChatFrame();
		BOOL bRet = pWndChat->LoadFrame(IDR_MAINFRAME, WS_VISIBLE|WS_POPUP|WS_CLIPCHILDREN|WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX, NULL);
		if (bRet)
		{
			pItem->m_pWndChat = pWndChat;
		}

		pWndChat->CenterWindow();
	}
	else
	{
		if (pItem->m_pWndChat->IsIconic())
			pItem->m_pWndChat->SendMessage(WM_SYSCOMMAND, SC_RESTORE);
		pItem->m_pWndChat->SetForegroundWindow();
	}

	*pResult = 1;
}

void CFriendPage::OnDestroy()
{
	CUIWnd::OnDestroy();

	// TODO: Add your message handler code here

	HTREEITEM hRootItem = m_treeFriend.GetRootItem();
	while (hRootItem != NULL)
	{
		DWORD_PTR dwData = m_treeFriend.GetItemData(hRootItem);
		ASSERT(dwData == NULL);

		HTREEITEM hChildItem = m_treeFriend.GetChildItem(hRootItem);
		while (hChildItem != NULL)
		{
			FRIENDSITEM *pData = (FRIENDSITEM *)m_treeFriend.GetItemData(hChildItem);
			ASSERT(pData != NULL);

			delete pData;

			hChildItem = m_treeFriend.GetNextItem(hChildItem, TVGN_NEXT);
		}

		hRootItem = m_treeFriend.GetNextItem(hRootItem, TVGN_NEXT);
	}
}
