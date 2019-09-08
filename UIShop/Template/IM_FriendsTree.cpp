// FriendsTree.cpp : implementation file
//

#include "stdafx.h"
#include "$$root$$.h"
#include "FriendsTree.h"

#ifdef _DEBUG
#define new	IUI_DEBUG_NEW
#endif // _DEBUG


#define ROOTITEM_ICONTEXT_SPACE 4
#define SUBITEM_ICONTEXT_SPACE 10

#define CF_PRIVATEHTREEITEM	CF_PRIVATEFIRST+1

#define TIMER_ID_SET_ITEM_HEIGHT			1


// CFriendsTree

CFriendsTree::CFriendsTree()
{
	m_sizeTray.cx = m_sizeTray.cy = 14;
	m_nUnselectedFriendItemIconOffsetX = 5;
	m_nUnselectedFriendItemTextOffsetX = 37;
	m_nUnselectedFriendItemNameBetweenSignedInterval = 8;
	m_sizeUnselectedFriendItemIcon.cx = m_sizeUnselectedFriendItemIcon.cy = 20;

	m_nSelectedFriendItemFaceOffsetX = 6;
	m_sizeSelectedFriendItemFaceBorder.cx = m_sizeSelectedFriendItemFaceBorder.cy = 46;
	m_sizeSelectedFriendItemFace.cx = m_sizeSelectedFriendItemFace.cy = 40;
	m_nSelectedFriendItemTextOffsetX = 52;

	m_sizeUnselectedFriendItemState.cx = m_sizeUnselectedFriendItemState.cy = 9;
	m_sizeSelectedFriendItemState.cx = m_sizeSelectedFriendItemState.cy = 11;

	m_hSelectedItem = NULL;
	m_hUnSelectedItem = NULL;
	m_nTick = 0;		// Max 6
}

CFriendsTree::~CFriendsTree()
{
}


BEGIN_MESSAGE_MAP(CFriendsTree, CSkinTreeCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
//	ON_COMMAND(IDSM_TREELISTRMENU_IITEM0, OnTreelistrmenuIitem0)
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnTvnEndlabeledit)
//	ON_COMMAND(ID_TREELISTRMENU_RENAME, OnTreelistrmenuRename)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnTvnBegindrag)
	ON_MESSAGE(DROPM_DRAGOVER,OnDragOver)
	ON_MESSAGE(DROPM_DROPEX,OnDropEx)
	ON_MESSAGE(DROPM_DROP,OnDrop)
	ON_MESSAGE(DROPM_DRAGSCROLL, OnDragScroll)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(TVN_HIGHLIGHTCHANGED, OnHighlightChanged)
END_MESSAGE_MAP()



//////////////////////////////////////////////////////////////////////////
// protect

CRect CFriendsTree::GetMobileTrayRect(HTREEITEM hItem)
{
	if (hItem == NULL)
		return 0;
	
	CRect rcItem;
	GetItemRect(hItem, rcItem, FALSE);
	
	CRect rcRet = rcItem;
	rcRet.left = m_nSelectedFriendItemTextOffsetX;
	rcRet.top = rcItem.top + rcItem.Height() * 2 / 3;
	rcRet.right = rcRet.left + m_sizeTray.cx;
	rcRet.bottom = rcRet.top + m_sizeTray.cy;
	
	return rcRet;
}

CRect CFriendsTree::GetEmailTrayRect(HTREEITEM hItem)
{
	if (hItem == NULL)
		return 0;

	FRIENDSITEM * pItem = (FRIENDSITEM *)GetItemData(hItem);
	if (pItem == NULL)
		return 0;
	
	CRect rcItem;
	GetItemRect(hItem, rcItem, FALSE);

	CRect rcRet = rcItem;
	rcRet.left = m_nSelectedFriendItemTextOffsetX;
	if (pItem->m_bHasMobile)
		rcRet.left += m_sizeTray.cx;
	rcRet.top = rcItem.top + rcItem.Height() * 2 / 3;
	rcRet.right = rcRet.left + m_sizeTray.cx;
	rcRet.bottom = rcRet.top + m_sizeTray.cy;

	return rcRet;
}

//////////////////////////////////////////////////////////////////////////
// 

// QQ Item背景特点：
// 1. 组名在选中后，无Selected状态和Highlight状态背景
// 2. 组名在未选中时，无Selected状态，但有Highlight状态背景
// 3. 组员Highlight时，为蓝色Item背景，选中后，为黄色Item背景，且Item高度增加
int CFriendsTree::DrawItemBk(CDC *pDC, HTREEITEM hItem)
{
	UINT nSelectedState = GetItemState(hItem, TVIS_SELECTED);
	BOOL bRootItem = (GetParentItem(hItem) == NULL) ? TRUE : FALSE;
	CRect rcItem;
	GetItemRect(hItem, rcItem, FALSE);

	// draw selected item.
	if (TVIS_SELECTED & nSelectedState)
	{
		pDC->SetTextColor(GetSelectedItemTextColor());
		CWnd *pFocusWnd = GetFocus();
		
		// Group item do not draw item background
		CIUIImage *phimgItemBK = NULL;
		if (!bRootItem)
		{
			CIUIImage *phimgSelItemBkN = NULL;
			CIUIImage *phimgSelItemBkD = NULL;
			GetSelectedItemBkImages(&phimgSelItemBkN, &phimgSelItemBkD);
			
			if (phimgSelItemBkN && pFocusWnd == this)
				phimgItemBK = phimgSelItemBkN;
			else if(phimgSelItemBkD != NULL)
				phimgItemBK = phimgSelItemBkD;
		}
		
		// Draw selected item background
		POINT ptResize;
		GetSelectedItemBkResizePoint(&ptResize);
		if (phimgItemBK != NULL)
			IUINineGridBlt(pDC->GetSafeHdc(), rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(), *phimgItemBK, ptResize.x, ptResize.y);
	}
	
	// Item is highlight
	else if (IsHighlightItemEffectEnabled() && (GetHighlightItem() == hItem))
	{
		CWnd *pFocusWnd = GetFocus();
		
		CIUIImage *phimgHighlightItemBkN = NULL;
		CIUIImage *phimgHighlightItemBkD = NULL;
		GetHighlightItemBkImages(&phimgHighlightItemBkN, &phimgHighlightItemBkD);
		
		CIUIImage *phimgItemBK = NULL;
		if (phimgHighlightItemBkN != NULL && pFocusWnd == this)
			phimgItemBK = phimgHighlightItemBkN;
		else if(phimgHighlightItemBkD != NULL)
			phimgItemBK = phimgHighlightItemBkD;
		
		// Draw highlight item background
		POINT ptResize;
		GetHighlightItemBkResizePoint(&ptResize);
		if (phimgItemBK != NULL)
			IUINineGridBlt(pDC->GetSafeHdc(), rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(), *phimgItemBK, ptResize.x, ptResize.y);
		else
			pDC->FillSolidRect(rcItem, GetHighlightItemBkColor());
	}
	
	// Normal
	else
	{
		CWnd *pFocusWnd = GetFocus();
		
		CIUIImage *phimgItemBK = NULL;
		CPoint ptResize(3, 3);
		// Normal root item
		if (bRootItem && IsRootItemEffectEnabled())
		{
			CIUIImage *phimgRootBkFocus = NULL;
			CIUIImage *phimgRootBkNoFocus = NULL;
			GetRootItemBkImages(&phimgRootBkFocus, &phimgRootBkNoFocus);
			if (pFocusWnd == this)
				phimgItemBK = phimgRootBkFocus;
			else
				phimgItemBK = phimgRootBkNoFocus;
			
			POINT ptResize;
			GetRootItemBkResizePoint(&ptResize);
		}
		// Other normal item
		else
		{
		}
		
		if (phimgItemBK != NULL)
			IUINineGridBlt(pDC->GetSafeHdc(), rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(), *phimgItemBK, ptResize.x, ptResize.y);
		
		pDC->SetTextColor(RGB(0, 0, 0));
	}

	return 0;
}

int CFriendsTree::DrawGroupItem(CDC *pDC, HTREEITEM hItem, const CRect &rcItem, const CRect &rcText)
{
	// icon
	DrawItemIcon(pDC, hItem, rcItem, rcText);
	
	// text
	LOGFONT logfont0;
	memset(&logfont0, 0, sizeof(LOGFONT)); 
	logfont0.lfWeight = FW_NORMAL; 
	logfont0.lfHeight = -12; 
	lstrcpy(logfont0.lfFaceName, _T("微软雅黑"));
	CFont fontLine0;
	fontLine0.CreateFontIndirect(&logfont0); 
	CFont *pOldFont0 = pDC->SelectObject(&fontLine0);
	pDC->SetTextColor(RGB(0, 0, 0));
	
	CString strItem = GetItemText(hItem);
	int nOnlineCount = GetOnlineCount(hItem);
	int nChildrenCount = GetChildrenCount(hItem);

	CString strGroup;
	strGroup.Format(_T("%s [%d/%d]"), strItem, nOnlineCount, nChildrenCount);

	CRect rect = rcText;
	pDC->DrawText(strGroup, rect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	//
	pDC->SelectObject(pOldFont0);

	return 0;
}

int CFriendsTree::DrawFriendItem(CDC *pDC, HTREEITEM hItem, const CRect &rcItem, const CRect &rcText)
{
	FRIENDSITEM * pItem = (FRIENDSITEM *)GetItemData(hItem);
	if (pItem == NULL)
		return -1;

	UINT nSelectedState = GetItemState(hItem, TVIS_SELECTED);
	
	// Draw selected friend
	if (TVIS_SELECTED & nSelectedState)
	{
		// Draw face border
		int nXDeflate = (m_sizeSelectedFriendItemFaceBorder.cx - m_sizeSelectedFriendItemFace.cx) / 2;
		int nYDeflate = (m_sizeSelectedFriendItemFaceBorder.cy - m_sizeSelectedFriendItemFace.cy) / 2;

		if (m_hSelectedFriendItemFaceBorder != NULL)
		{
			CRect rcFaceBorder;
			int nTopOffset = (rcItem.Height() - m_sizeSelectedFriendItemFaceBorder.cy) / 2;
			rcFaceBorder.left = m_nSelectedFriendItemFaceOffsetX - nXDeflate;
			rcFaceBorder.top = rcItem.top + nTopOffset;
			rcFaceBorder.right = rcFaceBorder.left + m_sizeSelectedFriendItemFaceBorder.cx;
			rcFaceBorder.bottom = rcFaceBorder.top + m_sizeSelectedFriendItemFaceBorder.cy;

			IUIStretchBlt(pDC->GetSafeHdc(), rcFaceBorder, m_hSelectedFriendItemFaceBorder);
		}

		// Draw face
		CRect rcFace;
		if (pItem->m_hBitmapFace != NULL)
		{
			int nTopOffset = (rcItem.Height() - m_sizeSelectedFriendItemFace.cy) / 2;
			rcFace.left = m_nSelectedFriendItemFaceOffsetX;
			rcFace.top = rcItem.top + nTopOffset;
			rcFace.right = rcFace.left + m_sizeSelectedFriendItemFace.cx;
			rcFace.bottom = rcFace.top + m_sizeSelectedFriendItemFace.cy;

			if (pItem->m_eOnlineState == OS_OFFLINE)
			{
				HGPBITMAP h = Bitmap::FromHBITMAP(pItem->m_hBitmapFace, NULL);
				StretchBltEx2GPBlackWhite(pDC->GetSafeHdc(), rcFace, h);
			}
			else
			{
				IUIStretchBlt(pDC->GetSafeHdc(), rcFace, pItem->m_hBitmapFace);
			}
		}

		// Draw on-line state
		CIUIImage hState;
		hState.Attach(m_mapOnlineStateImage[pItem->m_eOnlineState].second);
		if (hState != NULL)
		{
			CRect rcState = rcFace;
			rcState.left = rcState.right - m_sizeSelectedFriendItemState.cx;
			rcState.top = rcState.bottom - m_sizeSelectedFriendItemState.cy;
			IUIStretchBlt(pDC->GetSafeHdc(), rcState, hState);
		}

		// draw text
		{
			CRect rcLine0 = rcItem;
			rcLine0.left = m_nSelectedFriendItemTextOffsetX;
			rcLine0.bottom = rcLine0.top + rcItem.Height() / 3;
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->DrawText(pItem->m_strName, rcLine0, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
		}
		
		{
			CRect rcLine1 = rcItem;
			rcLine1.left = m_nSelectedFriendItemTextOffsetX;
			rcLine1.top += rcItem.Height() / 3;
			rcLine1.bottom = rcLine1.top + rcItem.Height() / 3;
			pDC->SetTextColor(RGB(128, 128, 128));
			pDC->DrawText(pItem->m_strSigned, rcLine1, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}

		// Draw tray icon
		if (pItem->m_bHasMobile)
		{
			CRect rcTrayIcon = GetMobileTrayRect(hItem);
			IUIStretchBlt(pDC->GetSafeHdc(), rcTrayIcon, m_hMobileTrayIcon);
		}
		if (pItem->m_bHasEmail)
		{
			CRect rcTrayIcon = GetEmailTrayRect(hItem);
			IUIStretchBlt(pDC->GetSafeHdc(), rcTrayIcon, m_hEmailTrayIcon);
		}
	}
	else
	{
		// Draw face
		CRect rcFace;
		if (pItem->m_hBitmapFace != NULL)
		{
			int nTopOffset = (rcItem.Height() - m_sizeUnselectedFriendItemIcon.cy) / 2;
			rcFace.left = m_nUnselectedFriendItemIconOffsetX;
			rcFace.top = rcItem.top + nTopOffset;
			rcFace.right = rcFace.left + m_sizeUnselectedFriendItemIcon.cx;
			rcFace.bottom = rcFace.top + m_sizeUnselectedFriendItemIcon.cy;

			if (pItem->m_eOnlineState == OS_OFFLINE)
			{
				HGPBITMAP h = Bitmap::FromHBITMAP(pItem->m_hBitmapFace, NULL);
				StretchBltEx2GPBlackWhite(pDC->GetSafeHdc(), rcFace, h);
			}
			else
				IUIStretchBlt(pDC->GetSafeHdc(), rcFace, pItem->m_hBitmapFace);
		}

		// Draw online state
		CIUIImage hState;
		hState.Attach(m_mapOnlineStateImage[pItem->m_eOnlineState].first);
		if (hState != NULL)
		{
			CRect rcState = rcFace;
			rcState.left = rcState.right - m_sizeUnselectedFriendItemState.cx;
			rcState.top = rcState.bottom - m_sizeUnselectedFriendItemState.cy;
			IUIStretchBlt(pDC->GetSafeHdc(), rcState, hState);
		}

		// Draw name
		CRect rcName(rcItem);
		rcName.left = m_nUnselectedFriendItemTextOffsetX;

		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->DrawText(pItem->m_strName, rcName, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		CRect rcCal = rcName;
		int nHeight = pDC->DrawText(pItem->m_strName, rcCal, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_CALCRECT);
		int nNameWidth = rcCal.Width();

		// Draw personal signed
		CRect rcSigned(rcItem);
		rcSigned.left = m_nUnselectedFriendItemTextOffsetX + nNameWidth + m_nUnselectedFriendItemNameBetweenSignedInterval;

		pDC->SetTextColor(RGB(128, 128, 128));
		pDC->DrawText(pItem->m_strSigned, rcSigned, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}

	return 0;
}

int CFriendsTree::DrawItem(CDC *pDC, HTREEITEM hItem, int nLeftMargin/* = 0*/)
{
	CRect rcItem;
	GetItemRect(hItem, rcItem, FALSE);
	CRect rcText;
	GetItemRect(hItem, rcText, TRUE);
	rcText.right = rcItem.right;

	DWORD_PTR dwData = GetItemData(hItem);
	UINT nSelectedState = GetItemState(hItem, TVIS_SELECTED);
	pDC->SetBkMode(TRANSPARENT);

	// Draw item background
	DrawItemBk(pDC, hItem);

	HTREEITEM hParent = GetParentItem(hItem);
	BOOL bHasChildren = ItemHasChildren(hItem);

	// Draw icon and text
	CFont *pOldFont = pDC->SelectObject(&m_fontNormal);

	if (hParent == NULL && bHasChildren) // Group item
	{
		DrawGroupItem(pDC, hItem, rcItem, rcText);
	}
	else
	{
		DrawFriendItem(pDC, hItem, rcItem, rcText);
	}

	pDC->SelectObject(pOldFont);

	return 0;
}

int CFriendsTree::SetFriendFace(HTREEITEM hItem, CIUIImage &hFace)
{
	HTREEITEM hParentItem = GetParentItem(hItem);
	if (hParentItem == NULL)
		return -1;

	FRIENDSITEM * pItem = (FRIENDSITEM *)GetItemData(hItem);
	ASSERT(pItem != NULL);
	
	pItem->m_hBitmapFace.Destroy();
	pItem->m_hBitmapFace.Attach(hFace);

	Invalidate();

	return 0;
}

CIUIImage *CFriendsTree::GetFriendFace(HTREEITEM hItem) const
{
	HTREEITEM hParentItem = GetParentItem(hItem);
	if (hParentItem == NULL)
		return NULL;

	FRIENDSITEM * pItem = (FRIENDSITEM *)GetItemData(hItem);
	ASSERT(pItem != NULL);

	return &pItem->m_hBitmapFace;
}

int CFriendsTree::SetSelectedFriendItemFaceBorder(CIUIImage &hFaceBorder)
{
	m_hSelectedFriendItemFaceBorder.Attach(hFaceBorder);

	return 0;
}

CIUIImage *CFriendsTree::GetSelectedFriendItemFaceBorder()
{
	return &m_hSelectedFriendItemFaceBorder;
}

int CFriendsTree::SetUnselectedFriendItemIconOffset(int nOffset)
{
	m_nUnselectedFriendItemIconOffsetX = nOffset;

	return 0;
}

int CFriendsTree::GetUnselectedFriendItemIconOffset() const
{
	return m_nUnselectedFriendItemIconOffsetX;
}

int CFriendsTree::SetMobileTrayImage(CIUIImage &hMobile)
{
	m_hMobileTrayIcon.Destroy();

	m_hMobileTrayIcon.Attach(hMobile);

	return 0;
}

CIUIImage *CFriendsTree::GetMobileTrayImage()
{
	return &m_hMobileTrayIcon;
}

int CFriendsTree::SetEmailTrayImage(CIUIImage &hMail)
{
	m_hEmailTrayIcon.Destroy();

	m_hEmailTrayIcon.Attach(hMail);

	return 0;
}

CIUIImage *CFriendsTree::GetEmailTrayImage()
{
	return &m_hEmailTrayIcon;
}

int CFriendsTree::ModifyUserProfile(HTREEITEM hItem, UINT nMask, LPCTSTR lpszImageFile, LPCTSTR lpszUserName, LPCTSTR lpszMail, LPCTSTR lpszUserState)
{
	HTREEITEM hParentItem = GetParentItem(hItem);
	if (hParentItem == NULL)
		return -1;

	FRIENDSITEM * pItem = (FRIENDSITEM *)GetItemData(hItem);
	ASSERT(pItem != NULL);

	if (nMask & TVIF_ICON)
	{
		pItem->m_hBitmapFace.Destroy();
		pItem->m_hBitmapFace.Load(lpszImageFile);
	}
	if (nMask & TVIF_TEXT0)
	{
		pItem->m_strName = lpszUserName;
	}
	if (nMask & TVIF_TEXT1)
	{
		pItem->m_strSigned = lpszMail;
	}

	Invalidate();

	return 0;
}

void CFriendsTree::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	HTREEITEM hItem = HitTest(point);
	if (hItem == NULL)
		return;

	BOOL bRootItem = (GetParentItem(hItem) == NULL);
	if (bRootItem)
	{
		// Not clicked the +- button, the +- button can expanded the parent item itseft.
		CRect rcText;
		GetItemRect(hItem, rcText, TRUE);

		rcText.right = rcText.left;
		rcText.left = 0;

		if (!rcText.PtInRect(point))
		{
			SetRedraw(FALSE);
			Expand(hItem, TVE_TOGGLE);
			SetRedraw(TRUE);
			Invalidate();
		}
	}
	else
	{
		CRect rcEmailTray = GetEmailTrayRect(hItem);
		CRect rcMobileTray = GetMobileTrayRect(hItem);
		if (rcEmailTray.PtInRect(point))		
		{
			MessageBox(_T("Clicked Email tray!"));
		}
		else if (rcMobileTray.PtInRect(point))
		{
			MessageBox(_T("Clicked Mobile tray!"));
		}
	}
}

void CFriendsTree::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

// 	CMenu menu;
// 	menu.LoadMenu(IDR_TREELISTMENU);
// 	CMenu *pMenu;
// 	pMenu = menu.GetSubMenu(0);
// 	CPoint pt;
// 	GetCursorPos(&pt);
// 	pMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);

	*pResult = 0;
}

BOOL CFriendsTree::IsGroupEmpty(HTREEITEM hGroupItem)
{
	return (!ItemHasChildren(hGroupItem));
}

int CFriendsTree::DeleteGroup(HTREEITEM hGroupItem, BOOL bFailedWhenHasChildren/*=TRUE*/)
{
	if (bFailedWhenHasChildren)
	{
		if (!IsGroupEmpty(hGroupItem))
			return -1;
	}
	BOOL bRet = DeleteItem(hGroupItem);

	if (bRet)
		return 0;
	else
		return -1;
}

int CFriendsTree::DeleteFriend(HTREEITEM hFriendItem)
{
	BOOL bRet = DeleteItem(hFriendItem);

	if (bRet)
		return 0;
	else
		return -1;
}

BOOL CFriendsTree::IsGroup(HTREEITEM hItem)
{
	BOOL bRootItem = (GetParentItem(hItem) == NULL && (hItem!=NULL));
	if (bRootItem)
	{
		return TRUE;
	}

	return FALSE;
}

int CFriendsTree::GetOnlineCount(HTREEITEM hGroupItem)
{
	int nOnlineCount = 0;

	HTREEITEM hChild = GetChildItem(hGroupItem);
	while (hChild != NULL)
	{
		ONLINE_STATS eOnlineState = GetOnlineState(hChild);
		if (eOnlineState == OS_ONLINE || eOnlineState == OS_BUSY || eOnlineState == OS_AWAY)
		{
			++nOnlineCount;
		}

		hChild = GetNextItem(hChild, TVGN_NEXT);
	}

	return nOnlineCount;
}

int CFriendsTree::SetOnlineState(HTREEITEM hItem, ONLINE_STATS eOnlineState)
{
	FRIENDSITEM * pItem = (FRIENDSITEM *)GetItemData(hItem);
	if (pItem == NULL)
		return -1;

	pItem->m_eOnlineState = eOnlineState;

	return 0;
}

ONLINE_STATS CFriendsTree::GetOnlineState(HTREEITEM hItem) const
{
	FRIENDSITEM * pItem = (FRIENDSITEM *)GetItemData(hItem);
	if (pItem == NULL)
		return OS_UNKNOWN;

	return pItem->m_eOnlineState;
}

int CFriendsTree::SetOnlineStateImage(ONLINE_STATS eOnlineState, CIUIImage &hStateIconSmall, CIUIImage &hStateIconBig)
{
	m_mapOnlineStateImage[eOnlineState].first.Attach(hStateIconSmall);
	m_mapOnlineStateImage[eOnlineState].second.Attach(hStateIconBig);

	return 0;
}

int CFriendsTree::GetOnlineStateImage(ONLINE_STATS eOnlineState, CIUIImage *phStateIconSmall, CIUIImage *phStateIconBig)
{
	if (phStateIconSmall == NULL || phStateIconBig == NULL)
		return -1;

	phStateIconSmall->Attach(m_mapOnlineStateImage[eOnlineState].first);
	phStateIconBig->Attach(m_mapOnlineStateImage[eOnlineState].second);

	return 0;
}

void CFriendsTree::OnTreelistrmenuIitem0()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetSelectedItem();
	if (hItem == NULL)
		return;

	BOOL bGroup = IsGroup(hItem);
	if (bGroup)
	{
		DeleteGroup(hItem);
	}
	else
	{
		DeleteFriend(hItem);
	}
}

void CFriendsTree::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	HTREEITEM hItem = HitTest(point);
	SelectItem(hItem);

	CSkinTreeCtrl::OnRButtonDown(nFlags, point);
}

void CFriendsTree::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: Add your control notification handler code here

	*pResult = 0;

	// the text of the item has been edited
	if (pTVDispInfo->item.pszText != NULL)
	{
		if (_tcslen(pTVDispInfo->item.pszText) == 0)
		{
			AfxMessageBox(_T("The name cann't be empty, please input a new name"));
			EditLabel(pTVDispInfo->item.hItem);
			return;
		}

		if (!IsGroup(pTVDispInfo->item.hItem))
		{
			FRIENDSITEM *pItemData = (FRIENDSITEM *)GetItemData(pTVDispInfo->item.hItem);
			ASSERT(pItemData != NULL);
			pItemData->m_strName = pTVDispInfo->item.pszText;
		}

		SelectItem(pTVDispInfo->item.hItem);

		*pResult = 1;

		Invalidate();
		return;
	}
	// the text of the item has not been edited
	else
	{
		CString str = GetItemText(pTVDispInfo->item.hItem);
		if (str.GetLength() == 0)
		{
			AfxMessageBox(_T("The name cann't be empty, please input a new name"));
			EditLabel(pTVDispInfo->item.hItem);
			return;
		}

		if (!IsGroup(pTVDispInfo->item.hItem))
		{
			FRIENDSITEM *pItemData = (FRIENDSITEM *)GetItemData(pTVDispInfo->item.hItem);
			ASSERT(pItemData != NULL);
			pItemData->m_strName = str;
		}

		*pResult = 1;

		Invalidate();
		return;
	}
}

void CFriendsTree::OnTreelistrmenuRename()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetSelectedItem();
	if (hItem == NULL)
		return;

	CEdit *pEdit = this->EditLabel(hItem);
	if (!IsGroup(hItem))
	{
		FRIENDSITEM *pItemData = (FRIENDSITEM *)GetItemData(hItem);
		ASSERT(pItemData != NULL);
		pEdit->SetWindowText(pItemData->m_strName);
	}
}

void CFriendsTree::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	if (GetParentItem(hItem) != NULL)
	{
		HGLOBAL hData = GlobalAlloc(GHND|GMEM_SHARE, sizeof(HTREEITEM));
		HTREEITEM *pS = (HTREEITEM *)GlobalLock(hData);
		*pS = pNMTreeView->itemNew.hItem;
		GlobalUnlock(hData);

		COleDataSource source;
		source.CacheGlobalData(CF_PRIVATEHTREEITEM, hData);
		source.DoDragDrop(DROPEFFECT_COPY);
		GlobalFree(hData);
	}

	*pResult = 0;
}

LRESULT CFriendsTree::OnDrop(WPARAM pDropInfoClass, LPARAM lParm)
{
	CIUIDropMsgParam* pInfo = (CIUIDropMsgParam* )pDropInfoClass;
	if (pInfo->m_pDataObject->IsDataAvailable(CF_PRIVATEHTREEITEM))
	{
		HGLOBAL hMem = pInfo->m_pDataObject->GetGlobalData(CF_PRIVATEHTREEITEM);
		HTREEITEM *hItemSource = (HTREEITEM *)GlobalLock((HGLOBAL)hMem);//lock source
		if (hItemSource != NULL)
		{
			HTREEITEM hItemTarget = this->HitTest(pInfo->m_point);
			CString strItem = GetItemText(*hItemSource);
		}
		GlobalUnlock(hMem);//unlock source
		return TRUE;
	}
	else
		return FALSE;

	return 0;
}

LRESULT CFriendsTree::OnDragScroll(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CFriendsTree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult = 0;

	// animate
#if 1
	// Kill the old timer, if the old item's height still changed with animation. change the height directly.
	KillTimer(TIMER_ID_SET_ITEM_HEIGHT);
	if (m_hSelectedItem != NULL && GetParentItem(m_hSelectedItem) != NULL)
		SetItemHeightEx(m_hSelectedItem, 53);
	if (m_hUnSelectedItem != NULL && GetParentItem(m_hUnSelectedItem) != NULL)
		SetItemHeightEx(m_hUnSelectedItem, 29);

	m_hSelectedItem = pNMTreeView->itemNew.hItem;
	m_hUnSelectedItem = pNMTreeView->itemOld.hItem;

	SetTimer(TIMER_ID_SET_ITEM_HEIGHT, 30, NULL);
#else
	// No animate
	if (pNMTreeView->itemNew.hItem != NULL && GetParentItem(pNMTreeView->itemNew.hItem) != NULL)
		SetItemHeightEx(pNMTreeView->itemNew.hItem, 53);

	if (pNMTreeView->itemOld.hItem != NULL && GetParentItem(pNMTreeView->itemOld.hItem) != NULL)
		SetItemHeightEx(pNMTreeView->itemOld.hItem, 29);
#endif
}

void CFriendsTree::OnHighlightChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMTREEVIEW *pNMTreeView = (NMTREEVIEW *)pNMHDR;
	
	HTREEITEM hHighlightItem = GetHighlightItem();
	SelectItem(hHighlightItem);

	*pResult = 0;
}

LRESULT CFriendsTree::OnDropEx(WPARAM pDropInfoClass, LPARAM lParm)
{
	return (DROPEFFECT)-1;
}

LRESULT CFriendsTree::OnDragOver(WPARAM pDropInfoClass,LPARAM lParm)
{
	CIUIDropMsgParam* pInfo = (CIUIDropMsgParam* )pDropInfoClass;
	if (pInfo->m_pDataObject->IsDataAvailable(CF_PRIVATEHTREEITEM))
		return DROPEFFECT_MOVE;
	else
		return DROPEFFECT_NONE;

	return 0;
}

int CFriendsTree::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSkinTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_dropEx.Register(this);

	if (m_fontNormal.GetSafeHandle() == NULL)
	{
		LOGFONT lf; 
		memset(&lf, 0, sizeof(LOGFONT)); 
		lf.lfWeight = FW_NORMAL; 
		lf.lfHeight = -11; 
		lstrcpy(lf.lfFaceName, _T("微软雅黑"));
		m_fontNormal.CreateFontIndirect(&lf); 
	}

	return 0;
}

void CFriendsTree::OnDestroy() 
{
	CSkinTreeCtrl::OnDestroy();

	// TODO: Add your message handler code here
	m_dropEx.Revoke();
}

void CFriendsTree::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == TIMER_ID_SET_ITEM_HEIGHT)
	{
		m_nTick++;

		// Change the height of the selected friend.
		if (m_hSelectedItem != NULL && GetParentItem(m_hSelectedItem) != NULL)
		{
			SetItemHeightEx(m_hSelectedItem, 29 + 4 * m_nTick);
		}
		
		// Change the height of the unselected friend.
		if (m_hUnSelectedItem != NULL && GetParentItem(m_hUnSelectedItem) != NULL)
		{
			SetItemHeightEx(m_hUnSelectedItem, 53 - 4 * m_nTick);
		}

		if (m_nTick >= 6)
		{
			m_nTick = 0;
			KillTimer(TIMER_ID_SET_ITEM_HEIGHT);
		}
	}
	
	CSkinTreeCtrl::OnTimer(nIDEvent);
}
