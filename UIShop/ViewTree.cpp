#include "stdafx.h"
#include "ViewTree.h"
#include "Resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree()
{
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnDeleteitem)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// public

BOOL CViewTree::IsWindowExist(HTREEITEM hTINoCompare, LPCTSTR lpszItem)
{
	ASSERT(lpszItem != NULL);

	HTREEITEM hTI = GetRootItem();
	hTI = GetNextItem(hTI, TVGN_CHILD);
	while (hTI)
	{
		if (hTINoCompare != hTI)
		{
			CString strText = GetItemText(hTI);
			if (0 == strText.Compare(lpszItem))
				return TRUE;
		}
		hTI = GetNextItem(hTI, TVGN_NEXT);
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// virtual

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

/////////////////////////////////////////////////////////////////////////////
// CViewTree message handlers

int CViewTree::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here

	return 0;
}

void CViewTree::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	// TODO: Add your control notification handler code here
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CUIShopDoc *pDoc = (CUIShopDoc *)pFrm->GetUIShopDoc();
	if (pDoc != NULL)
	{
		HTREEITEM hTI = GetSelectedItem();
		if (hTI == NULL)
		{
			return;
		}

		TREE_ITEMDATA *pData = (TREE_ITEMDATA *)GetItemData(hTI);
		ASSERT(pData != NULL);
		if (pData == NULL)
		{
			return;
		}

		if (pData->eTreeItemType == WINDOWS_ROOT
			|| pData->eTreeItemType == RESOURCE_ROOT
			|| pData->eTreeItemType == RES_DEFAULT_FONT_BRANCH
			|| pData->eTreeItemType == RES_FONT_BRANCH
			|| pData->eTreeItemType == RES_DEFAULT_COLOR_BRANCH
			|| pData->eTreeItemType == RES_COLOR_BRANCH
			|| pData->eTreeItemType == STYLE_ROOT
			|| (pData->eTreeItemType >= STYLE_PUSHBUTTON_ROOT && pData->eTreeItemType <= STYLE_WL_PIC_ROOT))
		{
			return;
		}

		pDoc->UpdateAllViews(NULL);
	}
}

void CViewTree::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult = 0;

	HTREEITEM hTI = pNMTreeView->itemOld.hItem;
	TREE_ITEMDATA *pData = (TREE_ITEMDATA *)GetItemData(hTI);
	if (pData == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	if (pData->eTreeItemType == WINDOWS_NODE)
	{
		UIWNDPROPERTIES *pUIProp = pData->wndUIDesign.GetUIWndProperties();
		ASSERT(pUIProp != NULL);

		int nIndex = CUIMgr::GetWindowIndex(pUIProp);
		CUIMgr::DeleteWindowItem(nIndex);
	}
	else if (pData->eTreeItemType == RES_GENERAL_FONT_NODE)
	{
		CUIMgr::DeleteFontResourceItem(pData->pResFont);
	}
	else if (pData->eTreeItemType == RES_CONDITION_FONT_NODE)
	{
		CUIMgr::DeleteFontResourceItem(pData->pResFont);
	}
	else if (pData->eTreeItemType == RES_CONDITION_CHILD_FONT_NODE)
	{
		CUIMgr::DeleteFontResourceItem((RESFONTPROPERTIES *)pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == RES_COLOR_NODE)
	{
		CUIMgr::DeleteColorResourceItem((RESCOLORPROPERTIES *)pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_PUSHBUTTON_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_PUSHBUTTON, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_CHECK_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_CHECK, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_RADIO_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_RADIO, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_COMBOBOX_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_COMBOBOX, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_STATIC_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_STATIC, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_EDIT_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_EDIT, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_SLIDER_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_SLIDER, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_PROGRESS_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_PROGRESS, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_SPIN_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_SPIN, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_HOTKEY_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_HOTKEY, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_IPADDRESS_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_IPADDRESS, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_SLIDEREX_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_SLIDEREX, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_LISTCTRL_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_LISTCTRL, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_TREECTRL_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_TREECTRL, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_TREELIST_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_TREELIST, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_RICHEDIT_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_RICHEDIT, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_IMRICHEDIT_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_IMRICHEDIT, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_HTMLCTRL_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_HTMLCTRL, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_DATETIMECTRL_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_DATETIMECTRL, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_WAVECTRL_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_WAVECTRL, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_MENU_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_MENU, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_MENUBAR_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_MENUBAR, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_HEADERCTRL_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_HEADERCTRL, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_HORSCROLLBAR_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_HORSCROLLBAR, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_VERSCROLLBAR_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_VERSCROLLBAR, pData->pResourceOrStyle);
	}
	else if (pData->eTreeItemType == STYLE_SPLITTER_NODE)
	{
		CUIMgr::DeleteStyleItem(STYLET_SPLITTER, pData->pResourceOrStyle);
	}

	delete pData;
	pData = NULL;
	SetItemData(hTI, NULL);
}
