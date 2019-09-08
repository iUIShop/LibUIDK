#include "StdAfx.h"
#include "treeview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTreeCtrlEx::CTreeCtrlEx()
{

}

CTreeCtrlEx::~CTreeCtrlEx()
{
	DestroyWindow();
}

BOOL CTreeCtrlEx::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	TV_Init(NULL);

	CWnd *pWnd = this;
	return pWnd->Create(c_szTreeViewClass, NULL, dwStyle, rect, pParentWnd, nID);
}

BEGIN_MESSAGE_MAP(CTreeCtrlEx, CWnd)
	//{{AFX_MSG_MAP(CTreeCtrlEx)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

HTREEITEM CTreeCtrlEx::InsertItem(LPTVINSERTSTRUCT lpInsertStruct)
{
	ASSERT(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_INSERTITEM, 0, (LPARAM)lpInsertStruct);
}
HTREEITEM CTreeCtrlEx::InsertItem(LPCTSTR lpszItem, int nImage,
	int nSelectedImage, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	ASSERT(::IsWindow(m_hWnd));
	return InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE, lpszItem, nImage, nSelectedImage, 0, 0, 0, hParent, hInsertAfter);
}
HTREEITEM CTreeCtrlEx::InsertItem(LPCTSTR lpszItem, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	ASSERT(::IsWindow(m_hWnd));
	return InsertItem(TVIF_TEXT, lpszItem, 0, 0, 0, 0, 0, hParent, hInsertAfter);
}
BOOL CTreeCtrlEx::DeleteItem(HTREEITEM hItem)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_DELETEITEM, 0, (LPARAM)hItem);
}
#pragma warning(disable: 4310)
BOOL CTreeCtrlEx::DeleteAllItems()
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT);
}
#pragma warning(default: 4310)
BOOL CTreeCtrlEx::Expand(HTREEITEM hItem, UINT nCode)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_EXPAND, nCode, (LPARAM)hItem);
}
UINT CTreeCtrlEx::GetCount() const
{
	ASSERT(::IsWindow(m_hWnd));
	return (UINT)::SendMessage(m_hWnd, TVM_GETCOUNT, 0, 0);
}
UINT CTreeCtrlEx::GetIndent() const
{
	ASSERT(::IsWindow(m_hWnd));
	return (UINT)::SendMessage(m_hWnd, TVM_GETINDENT, 0, 0);
}
void CTreeCtrlEx::SetIndent(UINT nIndent)
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TVM_SETINDENT, nIndent, 0);
}
CImageList *CTreeCtrlEx::GetImageList(int nImageList) const
{
	ASSERT(::IsWindow(m_hWnd));
	return CImageList::FromHandle((HIMAGELIST)::SendMessage(m_hWnd, TVM_GETIMAGELIST, (WPARAM)nImageList, 0));
}
CImageList *CTreeCtrlEx::SetImageList(CImageList *pImageList, int nImageList)
{
	ASSERT(::IsWindow(m_hWnd));
	return CImageList::FromHandle((HIMAGELIST)::SendMessage(m_hWnd, TVM_SETIMAGELIST, (WPARAM)nImageList, (LPARAM)pImageList->GetSafeHandle()));
}
UINT CTreeCtrlEx::SetScrollTime(UINT uScrollTime)
{
	ASSERT(::IsWindow(m_hWnd));
	return (UINT) ::SendMessage(m_hWnd, TVM_SETSCROLLTIME, (WPARAM) uScrollTime, 0);
}
HTREEITEM CTreeCtrlEx::GetNextItem(HTREEITEM hItem, UINT nCode) const
{
	ASSERT(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, nCode, (LPARAM)hItem);
}
HTREEITEM CTreeCtrlEx::GetChildItem(HTREEITEM hItem) const
{
	ASSERT(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem);
}
HTREEITEM CTreeCtrlEx::GetNextSiblingItem(HTREEITEM hItem) const
{
	ASSERT(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItem);
}
HTREEITEM CTreeCtrlEx::GetPrevSiblingItem(HTREEITEM hItem) const
{
	ASSERT(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_PREVIOUS, (LPARAM)hItem);
}
HTREEITEM CTreeCtrlEx::GetParentItem(HTREEITEM hItem) const
{
	ASSERT(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hItem);
}
HTREEITEM CTreeCtrlEx::GetFirstVisibleItem() const
{
	ASSERT(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0);
}
HTREEITEM CTreeCtrlEx::GetNextVisibleItem(HTREEITEM hItem) const
{
	ASSERT(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_NEXTVISIBLE, (LPARAM)hItem);
}
HTREEITEM CTreeCtrlEx::GetPrevVisibleItem(HTREEITEM hItem) const
{
	ASSERT(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_PREVIOUSVISIBLE, (LPARAM)hItem);
}
HTREEITEM CTreeCtrlEx::GetLastVisibleItem() const
{
	ASSERT(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_LASTVISIBLE, 0);
}
HTREEITEM CTreeCtrlEx::GetSelectedItem() const
{
	ASSERT(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_CARET, 0);
}
UINT CTreeCtrlEx::GetScrollTime() const
{
	ASSERT(::IsWindow(m_hWnd));
	return (UINT) ::SendMessage(m_hWnd, TVM_GETSCROLLTIME, 0, 0);
}
HTREEITEM CTreeCtrlEx::GetDropHilightItem() const
{
	ASSERT(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_DROPHILITE, 0);
}
HTREEITEM CTreeCtrlEx::GetRootItem() const
{
	ASSERT(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, TVGN_ROOT, 0);
}
BOOL CTreeCtrlEx::Select(HTREEITEM hItem, UINT nCode)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_SELECTITEM, nCode, (LPARAM)hItem);
}
BOOL CTreeCtrlEx::SelectItem(HTREEITEM hItem)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL) ::SendMessage(m_hWnd, TVM_SELECTITEM, TVGN_CARET, (LPARAM)hItem);
}
BOOL CTreeCtrlEx::SelectDropTarget(HTREEITEM hItem)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL) ::SendMessage(m_hWnd, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)hItem);
}
BOOL CTreeCtrlEx::SelectSetFirstVisible(HTREEITEM hItem)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL) ::SendMessage(m_hWnd, TVM_SELECTITEM, TVGN_FIRSTVISIBLE, (LPARAM)hItem);
}
BOOL CTreeCtrlEx::GetItem(TVITEM *pItem) const
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)pItem);
}
BOOL CTreeCtrlEx::SetItem(TVITEM *pItem)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)pItem);
}
BOOL CTreeCtrlEx::SetItemText(HTREEITEM hItem, LPCTSTR lpszItem)
{
	ASSERT(::IsWindow(m_hWnd));
	return SetItem(hItem, TVIF_TEXT, lpszItem, 0, 0, 0, 0, NULL);
}
BOOL CTreeCtrlEx::SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage)
{
	ASSERT(::IsWindow(m_hWnd));
	return SetItem(hItem, TVIF_IMAGE | TVIF_SELECTEDIMAGE, NULL, nImage, nSelectedImage, 0, 0, NULL);
}
BOOL CTreeCtrlEx::SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask)
{
	ASSERT(::IsWindow(m_hWnd));
	return SetItem(hItem, TVIF_STATE, NULL, 0, 0, nState, nStateMask, NULL);
}
BOOL CTreeCtrlEx::SetItemData(HTREEITEM hItem, DWORD_PTR dwData)
{
	ASSERT(::IsWindow(m_hWnd));
	return SetItem(hItem, TVIF_PARAM, NULL, 0, 0, 0, 0, (LPARAM)dwData);
}
CEdit *CTreeCtrlEx::EditLabel(HTREEITEM hItem)
{
	ASSERT(::IsWindow(m_hWnd));
	return (CEdit *)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, TVM_EDITLABEL, 0, (LPARAM)hItem));
}
HTREEITEM CTreeCtrlEx::HitTest(TVHITTESTINFO *pHitTestInfo) const
{
	ASSERT(::IsWindow(m_hWnd));
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_HITTEST, 0, (LPARAM)pHitTestInfo);
}
CEdit *CTreeCtrlEx::GetEditControl() const
{
	ASSERT(::IsWindow(m_hWnd));
	return (CEdit *)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, TVM_GETEDITCONTROL, 0, 0));
}
UINT CTreeCtrlEx::GetVisibleCount() const
{
	ASSERT(::IsWindow(m_hWnd));
	return (UINT)::SendMessage(m_hWnd, TVM_GETVISIBLECOUNT, 0, 0);
}
BOOL CTreeCtrlEx::SortChildren(HTREEITEM hItem)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_SORTCHILDREN, 0, (LPARAM)hItem);
}
BOOL CTreeCtrlEx::EnsureVisible(HTREEITEM hItem)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_ENSUREVISIBLE, 0, (LPARAM)hItem);
}
BOOL CTreeCtrlEx::SortChildrenCB(LPTVSORTCB pSort)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, TVM_SORTCHILDRENCB, 0, (LPARAM)pSort);
}
CToolTipCtrl *CTreeCtrlEx::GetToolTips() const
{
	ASSERT(::IsWindow(m_hWnd));
	return (CToolTipCtrl *)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, TVM_GETTOOLTIPS, 0, 0L));
}
CToolTipCtrl *CTreeCtrlEx::SetToolTips(CToolTipCtrl *pWndTip)
{
	ASSERT(::IsWindow(m_hWnd));
	return (CToolTipCtrl *)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, TVM_SETTOOLTIPS, (WPARAM)pWndTip->GetSafeHwnd(), 0L));
}
COLORREF CTreeCtrlEx::GetBkColor() const
{
	ASSERT(::IsWindow(m_hWnd));
	return (COLORREF) ::SendMessage(m_hWnd, TVM_GETBKCOLOR, 0, 0L);
}
COLORREF CTreeCtrlEx::SetBkColor(COLORREF clr)
{
	ASSERT(::IsWindow(m_hWnd));
	return (COLORREF) ::SendMessage(m_hWnd, TVM_SETBKCOLOR, 0, (LPARAM)clr);
}
SHORT CTreeCtrlEx::GetItemHeight() const
{
	ASSERT(::IsWindow(m_hWnd));
	return (SHORT) ::SendMessage(m_hWnd, TVM_GETITEMHEIGHT, 0, 0L);
}
SHORT CTreeCtrlEx::SetItemHeight(SHORT cyHeight)
{
	ASSERT(::IsWindow(m_hWnd));
	return (SHORT) ::SendMessage(m_hWnd, TVM_SETITEMHEIGHT, cyHeight, 0L);
}
COLORREF CTreeCtrlEx::GetTextColor() const
{
	ASSERT(::IsWindow(m_hWnd));
	return (COLORREF) ::SendMessage(m_hWnd, TVM_GETTEXTCOLOR, 0, 0L);
}
COLORREF CTreeCtrlEx::SetTextColor(COLORREF clr)
{
	ASSERT(::IsWindow(m_hWnd));
	return (COLORREF) ::SendMessage(m_hWnd, TVM_SETTEXTCOLOR, 0, (LPARAM)clr);
}
BOOL CTreeCtrlEx::SetInsertMark(HTREEITEM hItem, BOOL fAfter)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL) ::SendMessage(m_hWnd, TVM_SETINSERTMARK, fAfter, (LPARAM)hItem);
}
COLORREF CTreeCtrlEx::GetInsertMarkColor() const
{
	ASSERT(::IsWindow(m_hWnd));
	return (COLORREF) ::SendMessage(m_hWnd, TVM_GETINSERTMARKCOLOR, 0, 0L);
}
COLORREF CTreeCtrlEx::SetInsertMarkColor(COLORREF clrNew)
{
	ASSERT(::IsWindow(m_hWnd));
	return (COLORREF) ::SendMessage(m_hWnd, TVM_SETINSERTMARKCOLOR, 0, (LPARAM)clrNew);
}
BOOL CTreeCtrlEx::EndEditLabelNow(BOOL fCancelWithoutSave)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL) ::SendMessage(m_hWnd, TVM_ENDEDITLABELNOW, (WPARAM)fCancelWithoutSave, 0);
}

#if _WIN32_IE >= 0x0500
COLORREF CTreeCtrlEx::GetLineColor() const
{
	ASSERT(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, TVM_GETLINECOLOR, 0, 0L);
}
COLORREF CTreeCtrlEx::SetLineColor(COLORREF clrNew /*= CLR_DEFAULT*/)
{
	ASSERT(::IsWindow(m_hWnd));
	return (COLORREF)::SendMessage(m_hWnd, TVM_SETLINECOLOR, 0, (LPARAM)clrNew);
}
#endif

#if (_WIN32_WINNT >= 0x0501) && defined(UNICODE)
UINT CTreeCtrlEx::MapItemToAccId(HTREEITEM hItem) const
{
	ASSERT(::IsWindow(m_hWnd));
	return (UINT) ::SendMessage(m_hWnd, TVM_MAPHTREEITEMTOACCID, (WPARAM)hItem, 0);
}
HTREEITEM CTreeCtrlEx::MapAccIdToItem(UINT uAccId) const
{
	ASSERT(::IsWindow(m_hWnd));
	return (HTREEITEM) ::SendMessage(m_hWnd, TVM_MAPACCIDTOHTREEITEM, (WPARAM)uAccId, 0);
}
#endif

#if (_WIN32_WINNT >= 0x0600) && defined(UNICODE)
UINT CTreeCtrlEx::GetSelectedCount()
{
	ASSERT(::IsWindow(m_hWnd));
	return (UINT) ::SendMessage(m_hWnd, TVM_GETSELECTEDCOUNT, 0, 0);
}
void CTreeCtrlEx::ShowInfoTip(HTREEITEM hItem)
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, TVM_SHOWINFOTIP, 0, (LPARAM)hItem);
}
BOOL CTreeCtrlEx::SetItemStateEx(HTREEITEM hItem, UINT uStateEx)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL)SetItemEx(hItem, TVIF_STATEEX, 0, 0, 0, 0, 0, 0, uStateEx, 0, 0);
}
BOOL CTreeCtrlEx::SetItemExpandedImageIndex(HTREEITEM hItem, int nIndex)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL)SetItemEx(hItem, TVIF_EXPANDEDIMAGE, 0, 0, 0, 0, 0, 0, TVIF_EXPANDEDIMAGE, 0, nIndex);
}
DWORD CTreeCtrlEx::GetExtendedStyle() const
{
	ASSERT(::IsWindow(m_hWnd));
	return (DWORD) ::SendMessage(m_hWnd, TVM_GETEXTENDEDSTYLE, (WPARAM)0, 0);
}
DWORD CTreeCtrlEx::SetExtendedStyle(DWORD dwExMask, DWORD dwExStyles)
{
	ASSERT(::IsWindow(m_hWnd));
	return (DWORD) ::SendMessage(m_hWnd, TVM_SETEXTENDEDSTYLE, (WPARAM)dwExMask, (LPARAM)dwExStyles);
}
BOOL CTreeCtrlEx::SetAutoscrollInfo(UINT uPixelsPerSec, UINT uUpdateTime)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL) ::SendMessage(m_hWnd, TVM_SETAUTOSCROLLINFO, (WPARAM)uPixelsPerSec, (LPARAM)uUpdateTime);
}
#endif

BOOL CTreeCtrlEx::GetItemRect(HTREEITEM hItem, LPRECT lpRect, BOOL bTextOnly) const
{
	ASSERT(::IsWindow(m_hWnd));
	*(HTREEITEM *)lpRect = hItem;
	return (BOOL)::SendMessage(m_hWnd, TVM_GETITEMRECT, (WPARAM)bTextOnly,
			(LPARAM)lpRect);
}

CString CTreeCtrlEx::GetItemText(HTREEITEM hItem) const
{
	ASSERT(::IsWindow(m_hWnd));
	TVITEM item;
	item.hItem = hItem;
	item.mask = TVIF_TEXT;
	CString str;
	int nLen = 128;
	int nRes;
	do
	{
		nLen *= 2;
		item.pszText = str.GetBufferSetLength(nLen);
		item.cchTextMax = nLen;
		::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item);
		nRes = lstrlen(item.pszText);
	}
	while (nRes >= nLen - 1);
	str.ReleaseBuffer();
	return str;
}

BOOL CTreeCtrlEx::GetItemImage(HTREEITEM hItem, int &nImage, int &nSelectedImage) const
{
	ASSERT(::IsWindow(m_hWnd));
	TVITEM item;
	item.hItem = hItem;
	item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	BOOL bRes = (BOOL)::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item);
	if (bRes)
	{
		nImage = item.iImage;
		nSelectedImage = item.iSelectedImage;
	}
	return bRes;
}

UINT CTreeCtrlEx::GetItemState(HTREEITEM hItem, UINT nStateMask) const
{
	ASSERT(::IsWindow(m_hWnd));
	TVITEM item;
	item.hItem = hItem;
	item.mask = TVIF_STATE;
	item.stateMask = nStateMask;
	item.state = 0;
	VERIFY(::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item));
	return item.state;
}

DWORD_PTR CTreeCtrlEx::GetItemData(HTREEITEM hItem) const
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(hItem != NULL);
	TVITEM item;
	item.hItem = hItem;
	item.mask = TVIF_PARAM;
	VERIFY(::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item));
	return item.lParam;
}

BOOL CTreeCtrlEx::ItemHasChildren(HTREEITEM hItem) const
{
	ASSERT(::IsWindow(m_hWnd));
	TVITEM item;
	item.hItem = hItem;
	item.mask = TVIF_CHILDREN;
	::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item);
	return item.cChildren;
}

BOOL CTreeCtrlEx::SetItem(HTREEITEM hItem, UINT nMask, LPCTSTR lpszItem, int nImage,
	int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam)
{
	ASSERT(::IsWindow(m_hWnd));
	TVITEM item;
	item.hItem = hItem;
	item.mask = nMask;
	item.pszText = (LPTSTR) lpszItem;
	item.iImage = nImage;
	item.iSelectedImage = nSelectedImage;
	item.state = nState;
	item.stateMask = nStateMask;
	item.lParam = lParam;
	return (BOOL)::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)&item);
}

#if (_WIN32_IE >= 0x0600)
BOOL CTreeCtrlEx::SetItemEx(HTREEITEM hItem, UINT nMask, LPCTSTR lpszItem, int nImage,
	int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam,
	UINT uStateEx, HWND hWnd, int iExpandedImage)
{
	ASSERT(::IsWindow(m_hWnd));
	TVITEMEX item;
	item.hItem = hItem;
	item.mask = nMask;
	item.pszText = (LPTSTR) lpszItem;
	item.iImage = nImage;
	item.iSelectedImage = nSelectedImage;
	item.state = nState;
	item.stateMask = nStateMask;
	item.lParam = lParam;
#if(_MSC_VER > 1200) // VC6.0
	item.uStateEx = uStateEx;
	item.hwnd = hWnd;
	item.iExpandedImage = iExpandedImage;
#endif
	return (BOOL)::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)&item);
}
#endif

HTREEITEM CTreeCtrlEx::InsertItem(UINT nMask, LPCTSTR lpszItem, int nImage,
	int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam,
	HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	ASSERT(::IsWindow(m_hWnd));
	TVINSERTSTRUCT tvis;
	tvis.hParent = hParent;
	tvis.hInsertAfter = hInsertAfter;
	tvis.item.mask = nMask;
	tvis.item.pszText = (LPTSTR) lpszItem;
	tvis.item.iImage = nImage;
	tvis.item.iSelectedImage = nSelectedImage;
	tvis.item.state = nState;
	tvis.item.stateMask = nStateMask;
	tvis.item.lParam = lParam;
	return (HTREEITEM)::SendMessage(m_hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvis);
}

HTREEITEM CTreeCtrlEx::HitTest(CPoint pt, UINT *pFlags) const
{
	ASSERT(::IsWindow(m_hWnd));
	TVHITTESTINFO hti;
	hti.pt = pt;
	HTREEITEM h = (HTREEITEM)::SendMessage(m_hWnd, TVM_HITTEST, 0,
			(LPARAM)&hti);
	if (pFlags != NULL)
	{
		*pFlags = hti.flags;
	}
	return h;
}

void CTreeCtrlEx::RemoveImageList(int nImageList)
{
	HIMAGELIST h = (HIMAGELIST)SendMessage(TVM_GETIMAGELIST,
			(WPARAM)nImageList);
	if (CImageList::FromHandlePermanent(h) != NULL)
	{
		SendMessage(TVM_SETIMAGELIST, (WPARAM)nImageList, NULL);
	}
}

void CTreeCtrlEx::OnDestroy()
{
	RemoveImageList(LVSIL_NORMAL);
	RemoveImageList(LVSIL_STATE);

	CWnd::OnDestroy();
}

CImageList *CTreeCtrlEx::CreateDragImage(HTREEITEM hItem)
{
	ASSERT(::IsWindow(m_hWnd));

	HIMAGELIST hImageList = (HIMAGELIST)::SendMessage(m_hWnd,
			TVM_CREATEDRAGIMAGE, 0, (LPARAM)hItem);
	if (hImageList == NULL)
	{
		return NULL;
	}

	CImageList *pImageList = new CImageList;
	VERIFY(pImageList->Attach(hImageList));
	return pImageList;
}

BOOL CTreeCtrlEx::GetCheck(HTREEITEM hItem) const
{
	ASSERT(::IsWindow(m_hWnd));
	TVITEM item;
	item.mask = TVIF_HANDLE | TVIF_STATE;
	item.hItem = hItem;
	item.stateMask = TVIS_STATEIMAGEMASK;
	VERIFY(::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item));
	// Return zero if it's not checked, or nonzero otherwise.
	return ((BOOL)(item.state >> 12) - 1);
}

BOOL CTreeCtrlEx::SetCheck(HTREEITEM hItem, BOOL fCheck)
{
	ASSERT(::IsWindow(m_hWnd));
	TVITEM item;
	item.mask = TVIF_HANDLE | TVIF_STATE;
	item.hItem = hItem;
	item.stateMask = TVIS_STATEIMAGEMASK;

	// Since state images are one-based, passing a 1 to the
	// INDEXTOSTATEIMAGEMASK macro turns the check off, and
	// passing 2 turns it on.
	item.state = INDEXTOSTATEIMAGEMASK((fCheck ? 2 : 1));

	return (BOOL)::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)&item);
}

#if (_WIN32_WINNT >= 0x0600) && defined(UNICODE)

BOOL CTreeCtrlEx::GetItemPartRect(HTREEITEM hItem, TVITEMPART nPart, LPRECT lpRect)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(lpRect);
	TVGETITEMPARTRECTINFO info;
	info.hti = hItem;
	info.prc = lpRect;
	info.partID = nPart;
	return (BOOL)::SendMessage((m_hWnd), TVM_GETITEMPARTRECT, 0, (LPARAM)&info);
}

UINT CTreeCtrlEx::GetItemStateEx(HTREEITEM hItem) const
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(hItem);

	TVITEMEX item;
	item.hItem = hItem;
	item.mask = TVIF_STATEEX;
	VERIFY(::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item));
	return item.uStateEx;
}

int CTreeCtrlEx::GetItemExpandedImageIndex(HTREEITEM hItem) const
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(hItem);

	TVITEMEX item;
	item.hItem = hItem;

	item.mask = TVIF_EXPANDEDIMAGE;
	item.uStateEx = TVIF_EXPANDEDIMAGE;
	VERIFY(::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item));
	return item.iExpandedImage;
}
#endif
