// SkinTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "treeview.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define IDTT_ANIMATION_CONTENT				123
#define IDTT_INSERT_ITEM					124
#define IDTT_REMOVE_ITEM					125
#define IDTT_DELETE_ITEM					126
#define IDTT_VER_SCROLL						127

#define TVM_GETITEMDATAINTERNAL				WM_TREECTRL + 6
#define TVM_SETITEMDATAINTERNAL				WM_TREECTRL + 7

#define TVBD_FROMWHEEL      0x0001
#define TVBD_WHEELFORWARD   0x0002
#define TVBD_WHEELBACK      0x0004

struct TV_ITEMDATA
{
	TV_ITEMDATA()
	{
		lParam = 0;
		pBindObject = NULL;
		nItemHeight = 0;
		nBranchHeight = 0;
	}

	~TV_ITEMDATA()
	{
		SafeDelete(pBindObject);
	}

	LPARAM lParam;						// The user Item Data.
	SItemBindObject *pBindObject;	// the bind object of the item.

	// 行高，如果为0，使用tree的默认高度
	int nItemHeight;

	// 当本Item是展开状态时，表示所有可见（展开的，不一定在可视范围内）子孙Item的高度之和，不包括自己
	// 当本Item是折叠状态时，表示展开时，能多展出来的高度。
	// 当本item无子item时，nBranchHeight为零。
	// 定义这个变量的目的是当展开或折叠某个Item时，能快速更新tree内容的总高度，而不需要遍历tree。
	// 在垂直滚动Tree的时候，能快速设置第一个可见的Item m_hTop
	// 当增加、删除、展开、折叠或修改item高度时，应该更新祖宗Item的这个值
	int nBranchHeight;
};

struct TREEMEMBER
{
	TREEMEMBER()
	{
		m_bCacheMode = FALSE;

		m_bSmoothScroll = FALSE;
		m_nVScrollMouseWhellStep = 20;
		m_hTop = NULL;
		m_nTopItemYOffset = 0;
		m_nTopItemYScrollPos = 0;
		m_bRedraw = TRUE;
		m_cxState = 0;
		m_cyState = 0;
		m_cxImage = 0;
		m_cyImage = 0;
		m_cxMax = 0;
		m_ptCapture.x = m_ptCapture.y = 0;

		m_bTreeFirstShowed = FALSE;
		m_hLevel1ResizedBkImage = NULL;
		m_hLevel2BlendBkTextImage = NULL;
		m_hLevel3Background = NULL;
		m_hLevel4ImageList = NULL;
		m_sizeImageInImageList = CSize(0, 0);

		m_bVisible = TRUE;
		int i = 0;
		for (i = 0; i < 2; ++i)
		{
			m_himgBk[i] = NULL;
		}
		m_pRectCtrlBind = NULL;
		for (i = 0; i < 4; ++i)
		{
			m_eTextHorAlignMode[i] = TAH_DEFAULT;
			m_eTextVerAlignMode[i] = TAV_DEFAULT;
			m_rcTextMargin[i].SetRect(0, 0, 0, 0);
		}

		for (i = 0; i < 2; ++i)
		{
			m_hIUIFont[i] = NULL;
		}

		m_bUseCustomScrollBar = FALSE;
		m_lScrollBarWidth = 17;
		m_nTotalShowHeight = 0;
		m_bHoverScrollBarMode = false;

		// Item line
		m_bEnableItemLine = FALSE;
		m_crItemLine = RGB(128, 128, 128);

		// Collapse and expand image
		m_himgCombineCollapseExpanded = NULL;
		m_eCollapseExpandedImageResizeMode = IRM_CENTER;
		m_ptCollapseExpandedImageResize.x = m_ptCollapseExpandedImageResize.y = 2;
		for (i = 0; i < 4; ++i)
		{
			m_himgCollapse[i] = NULL;
			m_himgExpanded[i] = NULL;
		}

		// Root item
		m_bEnableRootItemEffect = FALSE;
		m_crRootItemText = RGB(0, 0, 0);
		m_eRootItemBkType = BKT_COLOR;
		m_crRootItemBK = RGB(255, 255, 255);
		m_bRootItemBkCombine = TRUE;
		m_himgRootItemCombineBk = NULL;
		for (i = 0; i < 2; ++i)
		{
			m_himgRootItemBk[i] = NULL;
		}
		m_ptRootItemBkResize.x = m_ptRootItemBkResize.y = 2;

		// Selected
		m_crSelectedItemText = RGB(255, 255, 255);
		m_eSelectedItemBkType = BKT_COLOR;
		m_crSelectedItemBK = RGB(0, 0, 128);
		m_bSelectedItemBkCombine = TRUE;
		m_himgSelectedItemCombineBk = NULL;
		for (i = 0; i < 2; ++i)
		{
			m_himgSelectedItemBk[i] = NULL;
		}
		m_ptSelectedItemBkResize.x = m_ptSelectedItemBkResize.y = 2;

		// Highlight
		m_bEnableHighlightItemEffect = FALSE;
		m_hCurHighlightItem = NULL;
		m_crHighlightBK = RGB(241, 241, 241);
		m_crHighlightItemText = RGB(255, 255, 255);
		m_bHighlightItemBkCombine = TRUE;
		m_himgHighlightItemCombineBk = NULL;
		for (i = 0; i < 2; ++i)
		{
			m_himgHighlightItemBk[i] = NULL;
		}
		m_ptHighlightItemBkResize.x = m_ptHighlightItemBkResize.y = 2;

		// Disabled
		m_crDisabledItemText = RGB(128, 128, 128);

		m_bSetItemDataInternal = FALSE;
		m_bInsertingItemSetData = FALSE;

		m_dwHoverTime = HOVER_DEFAULT;
		m_bHoverRepeat = FALSE;

		// Animation
		m_bAnimationMode = false;
		m_bTogetherAnimation = FALSE;
		m_nCurTimerTick = 0;

		m_bAnimateRemoveMode = false;
		m_hRemoveItem = NULL;
		m_nCurRemoveItemTimerTick = 0;

		m_hDeletedItem = NULL;
		m_nCurDeleteItemTimerTick = 0;

		m_nCurVerScrollTimerTick = 0;

		m_bAnimationInsertItem = false;
		m_hInsertItem = NULL;
		m_nCurInsertItemTimerTick = 0;

		m_bWindowlessNotCreateAsChild = false;
	}

	int Release()
	{
		m_bCacheMode = FALSE;

		m_bSmoothScroll = FALSE;
		m_nVScrollMouseWhellStep = 20;
		m_hTop = NULL;
		m_nTopItemYOffset = 0;
		m_nTopItemYScrollPos = 0;
		m_bRedraw = TRUE;
		m_cxState = 0;
		m_cyState = 0;
		m_cxImage = 0;
		m_cyImage = 0;
		m_cxMax = 0;
		m_ptCapture.x = m_ptCapture.y = 0;

		m_bTreeFirstShowed = FALSE;
		ReleaseIUIImage(m_hLevel1ResizedBkImage);
		m_hLevel1ResizedBkImage = NULL;
		SafeDeleteHBITMAP(m_hLevel2BlendBkTextImage);
		SafeDeleteHBITMAP(m_hLevel3Background);
		SafeDeleteHBITMAP(m_hLevel4ImageList);
		m_sizeImageInImageList = CSize(0, 0);

		m_bVisible = TRUE;
		int  i = 0;
		for (i = 0; i < 2; ++i)
		{
			ReleaseIUIImage(m_himgBk[i]);
			m_himgBk[i] = NULL;
		}
		m_pRectCtrlBind = NULL;
		for (i = 0; i < 4; ++i)
		{
			m_eTextHorAlignMode[i] = TAH_DEFAULT;
			m_eTextVerAlignMode[i] = TAV_DEFAULT;
			m_rcTextMargin[i].SetRect(0, 0, 0, 0);
		}

		for (i = 0; i < 2; ++i)
		{
			ReleaseIUIFontInternal(m_hIUIFont[i]);
			m_hIUIFont[i] = NULL;
		}

		m_bUseCustomScrollBar = FALSE;
		m_lScrollBarWidth = 17;
		m_nTotalShowHeight = 0;
		m_bHoverScrollBarMode = false;

		// Item line
		m_bEnableItemLine = FALSE;
		m_crItemLine = RGB(128, 128, 128);

		// Collapse and expand image
		ReleaseIUIImage(m_himgCombineCollapseExpanded);
		m_himgCombineCollapseExpanded = NULL;
		m_ptCollapseExpandedImageResize.x = m_ptCollapseExpandedImageResize.y = 2;
		for (i = 0; i < 4; ++i)
		{
			ReleaseIUIImage(m_himgCollapse[i]);
			m_himgCollapse[i] = NULL;
			ReleaseIUIImage(m_himgExpanded[i]);
			m_himgExpanded[i] = NULL;
		}

		// Root item
		m_bEnableRootItemEffect = FALSE;
		m_crRootItemText = RGB(0, 0, 0);
		m_eRootItemBkType = BKT_COLOR;
		m_crRootItemBK = RGB(255, 255, 255);
		m_bRootItemBkCombine = TRUE;
		ReleaseIUIImage(m_himgRootItemCombineBk);
		m_himgRootItemCombineBk = NULL;
		for (i = 0; i < 2; ++i)
		{
			ReleaseIUIImage(m_himgRootItemBk[i]);
			m_himgRootItemBk[i] = NULL;
		}
		m_ptRootItemBkResize.x = m_ptRootItemBkResize.y = 2;

		// Selected
		m_crSelectedItemText = RGB(255, 255, 255);
		m_eSelectedItemBkType = BKT_COLOR;
		m_crSelectedItemBK = RGB(0, 0, 128);
		m_bSelectedItemBkCombine = TRUE;
		ReleaseIUIImage(m_himgSelectedItemCombineBk);
		m_himgSelectedItemCombineBk = NULL;
		for (i = 0; i < 2; ++i)
		{
			ReleaseIUIImage(m_himgSelectedItemBk[i]);
			m_himgSelectedItemBk[i] = NULL;
		}
		m_ptSelectedItemBkResize.x = m_ptSelectedItemBkResize.y = 2;

		// Highlight
		m_bEnableHighlightItemEffect = FALSE;
		m_hCurHighlightItem = NULL;
		m_crHighlightBK = RGB(241, 241, 241);
		m_crHighlightItemText = RGB(255, 255, 255);
		m_bHighlightItemBkCombine = TRUE;
		ReleaseIUIImage(m_himgHighlightItemCombineBk);
		m_himgHighlightItemCombineBk = NULL;
		for (i = 0; i < 2; ++i)
		{
			ReleaseIUIImage(m_himgHighlightItemBk[i]);
			m_himgHighlightItemBk[i] = NULL;
		}
		m_ptHighlightItemBkResize.x = m_ptHighlightItemBkResize.y = 2;

		// Disabled
		m_crDisabledItemText = RGB(128, 128, 128);

		m_bSetItemDataInternal = FALSE;
		m_bInsertingItemSetData = FALSE;

		m_dwHoverTime = HOVER_DEFAULT;
		m_bHoverRepeat = FALSE;

		// Animation
		m_bAnimationMode = false;
		m_bTogetherAnimation = FALSE;
		m_nCurTimerTick = 0;

		m_bAnimateRemoveMode = false;
		m_hRemoveItem = NULL;
		m_nCurRemoveItemTimerTick = 0;

		m_hDeletedItem = NULL;
		m_nCurDeleteItemTimerTick = 0;

		m_nCurVerScrollTimerTick = 0;

		m_bAnimationInsertItem = false;
		m_hInsertItem = NULL;
		m_nCurInsertItemTimerTick = 0;

		m_bWindowlessNotCreateAsChild = false;

		return 0;
	}

	BOOL m_bCacheMode;						// True: use cache mode to draw tree

	BOOL m_bSmoothScroll;
	int m_nVScrollMouseWhellStep;			// 鼠标滚轮滚动一次的步进
	HTREEITEM m_hTop;						// 代替TREE::hTop. 第一个可见的Item
	int m_nTopItemYOffset;					// 记录m_hTop相对父窗口的Y方向偏移
	int m_nTopItemYScrollPos;				// 记录m_hTop的垂直滚动条位置
	BOOL m_bRedraw;
	int m_cxState;							// 状态Imagelist中图标宽度，代替TREE::cxState，因为TREE::cxState无法直接得到
	int m_cyState;							// 状态Imagelist中图标高度，代替TREE::cyState
	int m_cxImage;							// 常规Imagelist中图标宽度，代替TREE::cxImage
	int m_cyImage;							// 常规Imagelist中图标高度，代替TREE::cyImage
	POINT m_ptCapture;						// 代替TREE::ptCapture. Point where the mouse was capture

	// 在hittest时，会用到这个变量
	WORD m_cxMax;							// 最长Item的宽度，代替TREE::cxMax

	// For cache draw
	BOOL m_bTreeFirstShowed;				// Set to TRUE after the control first call WM_PAINT.
	HIUIIMAGE m_hLevel1ResizedBkImage;		// Level-1 cache, Generate by resizing m_himgBk with 9 grid method. the size is equal to non-client size
	HBITMAP m_hLevel2BlendBkTextImage;		// Level-2 cache, Blend m_hLevel1ResizedBkImage and text(not include highlight state text). the cache can show directly if m_bEnableHighlightItemEffect is FALSE.
	HBITMAP m_hLevel3Background;			// Level-3 cache, Base level-2, update partial content, such as highlight item content, Level-3 can show directly.
	HBITMAP m_hLevel4ImageList;				// Level-4 cache, store the image in image list
	CSize m_sizeImageInImageList;

	BOOL m_bVisible;
	HIUIIMAGE m_himgBk[2];
	// resize
	CRectCtrl *m_pRectCtrlBind;
	TEXT_ALIGN_HOR m_eTextHorAlignMode[4];
	TEXT_ALIGN_VER m_eTextVerAlignMode[4];
	CRect m_rcTextMargin[4];

	CString m_strTag;
	HIUIFONT m_hIUIFont[2];

	// for custom scroll bar
	BOOL m_bUseCustomScrollBar;
	LONG m_lScrollBarWidth;
	int m_nTotalShowHeight;
	CScrollBarEx m_wndHScroll;
	CScrollBarEx m_wndVScroll;
	bool m_bHoverScrollBarMode;					// 滚动条平时隐藏，鼠标移动到Tree内后显示

	// for item line
	BOOL m_bEnableItemLine;
	COLORREF m_crItemLine;

	// for Collapse and expand image
	HIUIIMAGE m_himgCombineCollapseExpanded;
	IMAGE_RESIZE_MODE m_eCollapseExpandedImageResizeMode;
	POINT m_ptCollapseExpandedImageResize;

	HIUIIMAGE m_himgCollapse[4];
	HIUIIMAGE m_himgExpanded[4];

	// for root item
	BOOL m_bEnableRootItemEffect;
	COLORREF m_crRootItemText;
	BACKGROUND_TYPE m_eRootItemBkType;
	COLORREF m_crRootItemBK;
	BOOL m_bRootItemBkCombine;
	HIUIIMAGE m_himgRootItemCombineBk;
	HIUIIMAGE m_himgRootItemBk[2];
	POINT m_ptRootItemBkResize;

	// for selected item
	COLORREF m_crSelectedItemText;
	BACKGROUND_TYPE m_eSelectedItemBkType;
	COLORREF m_crSelectedItemBK;
	BOOL m_bSelectedItemBkCombine;
	HIUIIMAGE m_himgSelectedItemCombineBk;
	HIUIIMAGE m_himgSelectedItemBk[2];
	POINT m_ptSelectedItemBkResize;

	// for highlight item
	BOOL m_bEnableHighlightItemEffect;
	HTREEITEM m_hCurHighlightItem;
	COLORREF m_crHighlightBK;
	COLORREF m_crHighlightItemText;
	BOOL m_bHighlightItemBkCombine;
	HIUIIMAGE m_himgHighlightItemCombineBk;
	HIUIIMAGE m_himgHighlightItemBk[2];
	POINT m_ptHighlightItemBkResize;

	// for disabled text color
	COLORREF m_crDisabledItemText;

	BOOL m_bSetItemDataInternal;
	BOOL m_bInsertingItemSetData;		// Insert Item call SetItemData, SetItemData may be trigger TVN_ITEMCHANGED, If call GetItemData in TVN_ITEMCHANGED notify,
	// the return value may wrong if use m_bSetItemDataInside flag, So, define m_bInsertingItemSetData.
	// for mouse hover
	DWORD m_dwHoverTime;
	BOOL m_bHoverRepeat;

	// The following variable for animating content. like windows 8.
	bool m_bAnimationMode;
	BOOL m_bTogetherAnimation;				// The items together move or not.
	int m_nCurTimerTick;					// Times that timer implement. same as Frame.
	std::vector<int> m_vAnimationData;

	// For animate remove item.
	bool m_bAnimateRemoveMode;
	HTREEITEM m_hRemoveItem;
	CBitmap m_bmpCachedRemovedItem;
	int m_nCurRemoveItemTimerTick;
	std::vector<DELETE_TREE_ITEM_ANIMATION> m_vRemoveItemAnimationData;

	// For animate delete item.
	HTREEITEM m_hDeletedItem;
	int m_nCurDeleteItemTimerTick;
	std::vector<int> m_vDeletedItemAnimationData;

	//
	int m_nCurVerScrollTimerTick;
	std::vector<int> m_vVerScrollAnimationData;

	//
	bool m_bAnimationInsertItem;
	HTREEITEM m_hInsertItem;
	CBitmap m_bmpCachedInsertItem;
	int m_nCurInsertItemTimerTick;
	std::vector<DELETE_TREE_ITEM_ANIMATION> m_vInsertItemAnimationData;

	// 对于Item绑定的windowless控件，由tree自己维护，不需要加入到WLMgr::vpWLChildren
	// 这时，把m_bWindowlessNotCreateAsChild设置为true。
	bool m_bWindowlessNotCreateAsChild;

	// Drag & drop
	CIUIOleDropTarget m_dropTarget;
};

TV_ITEMDATA *TreeView_GetItemData(HWND hwnd, TREEMEMBER *pMember, HTREEITEM hItem);
BOOL NEAR _LibUIDK_TV_SmoothSetTopItem(HWND hTreeCtrl, TREEMEMBER *pMember, UINT wNewTop);

int UpdateCustomScrollInfo(CSkinTreeCtrl *pTree)
{
	BOOL bRet = FALSE;

	// Horz
	SCROLLINFO siHorz;
	memset(&siHorz, 0, sizeof(SCROLLINFO));
	siHorz.cbSize = sizeof(SCROLLINFO);
	siHorz.fMask = SIF_ALL;
	bRet = pTree->GetScrollInfo(SB_HORZ, &siHorz);

	pTree->GetHorzScrollBarEx()->SetScrollInfo(&siHorz);

	// Ver
	SCROLLINFO siVert;
	memset(&siVert, 0, sizeof(SCROLLINFO));
	siVert.cbSize = sizeof(SCROLLINFO);
	siVert.fMask = SIF_ALL;
	bRet = pTree->GetScrollInfo(SB_VERT, &siVert);

	pTree->GetVertScrollBarEx()->SetScrollInfo(&siVert);

	return 0;
}

int _LibUIDK_CSkinTreeCtrl_UpdateLevel1(CSkinTreeCtrl *pTreeCtrl, BOOL bTreeFirstShowed,
	HIUIIMAGE hBkN, HIUIIMAGE hBkD,
	IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	int nDestWidth, int nDestHeight,
	__out HIUIIMAGE *phLevel1ResizedBkImage)
{
	if (!bTreeFirstShowed || phLevel1ResizedBkImage == NULL)
	{
		return -1;
	}

	ReleaseIUIImage(*phLevel1ResizedBkImage);

	HIUIIMAGE hBmp = NULL;
	if (pTreeCtrl->IsWindowEnabled())
	{
		hBmp = hBkN;
	}
	else
	{
		hBmp = hBkD;
	}

	if (hBmp != NULL)
	{
		CIUIImage::NineGridBitmap(hBmp, pt9GridResize, l9GridRepeatX, l9GridRepeatY,
			phLevel1ResizedBkImage, nDestWidth, nDestHeight);
	}

	return 0;
}

int _LibUIDK_CSkinTreeCtrl_UpdateLevel2(CSkinTreeCtrl *pTreeCtrl, BOOL bTreeFirstShowed,
	__out HBITMAP &hLevel2BlendBkTextImage)
{
	if (!bTreeFirstShowed)
	{
		return -1;
	}

	//
	// Recreate the m_hLevel2BlendBkTextImage
	//
	SafeDeleteHBITMAP(hLevel2BlendBkTextImage);

	CClientDC dc(pTreeCtrl);

	CRect rcClient;
	pTreeCtrl->GetClientRect(rcClient);

	hLevel2BlendBkTextImage = ::CreateCompatibleBitmap(dc.GetSafeHdc(), rcClient.Width(), rcClient.Height());

	//
	// Select hLevel2BlendBkTextImage to DC
	//
	CDC memBkDC;
	memBkDC.CreateCompatibleDC(&dc);
	HBITMAP hOldBmpBk = (HBITMAP)::SelectObject(memBkDC.GetSafeHdc(), hLevel2BlendBkTextImage);

	//
	// draw background
	//
	pTreeCtrl->SendMessage(TVM_DRAW_BACKGROUND, (WPARAM)&memBkDC, 0);

	//
	// Draw content
	//
	// draw item(text)
	COLORREF cr = pTreeCtrl->GetTextColor();
	memBkDC.SetTextColor(cr);
	UINT uVisibleCount = pTreeCtrl->GetVisibleCount();
	HTREEITEM hTI = pTreeCtrl->GetFirstVisibleItem();
	for (UINT i = 0; (i < uVisibleCount + 1) && (hTI != NULL); ++i)
	{
		pTreeCtrl->SendMessage(TVM_DRAW_ITEM, (WPARAM)&memBkDC, (LPARAM)hTI);
		hTI = pTreeCtrl->GetNextVisibleItem(hTI);
	}

	//
	// hLevel2BlendBkTextImage is OK.
	//
	::SelectObject(memBkDC.GetSafeHdc(), hOldBmpBk);

	return 0;
}

int _LibUIDK_CSkinTreeCtrl_UpdateLevel3(CSkinTreeCtrl *pTreeCtrl, BOOL bTreeFirstShowed, HBITMAP hLevel2BlendBkTextImage, __out HBITMAP &hLevel3Background, HTREEITEM hItemNewHighlight, HTREEITEM hItemOldHighlight)
{
	if (!bTreeFirstShowed)
	{
		return -1;
	}

	//
	// Recreate the m_hLevel3Background
	//
	SafeDeleteHBITMAP(hLevel3Background);

	CClientDC dc(pTreeCtrl);

	CRect rcClient;
	pTreeCtrl->GetClientRect(rcClient);

	hLevel3Background = ::CreateCompatibleBitmap(dc.GetSafeHdc(), rcClient.Width(), rcClient.Height());

	//
	// Select m_hLevel3Background to DC
	//
	CDC memBkDC;
	memBkDC.CreateCompatibleDC(&dc);
	HBITMAP hOldBmpBk = (HBITMAP)::SelectObject(memBkDC.GetSafeHdc(), hLevel3Background);

	//
	// Copy Level-2 to Level-3
	BitBltG(memBkDC.GetSafeHdc(), 0, 0, rcClient.Width(), rcClient.Height(),  hLevel2BlendBkTextImage, 0, 0, SRCCOPY);

	//
	// Draw changed content(highlight item), draw all of the items that are completely visible
	//
	memBkDC.SetBkMode(TRANSPARENT);
	if (hItemNewHighlight != NULL)
	{
		pTreeCtrl->SendMessage(TVM_DRAW_ITEM, (WPARAM)&memBkDC, (LPARAM)hItemNewHighlight);
	}
	if (hItemOldHighlight != NULL)
	{
		pTreeCtrl->SendMessage(TVM_DRAW_ITEM, (WPARAM)&memBkDC, (LPARAM)hItemOldHighlight);
	}

	//
	// m_hLevel3Background is OK.
	//
	::SelectObject(memBkDC.GetSafeHdc(), hOldBmpBk);

	return 0;
}

int _ReleaseSubItem(CSkinTreeCtrl *pThis, TREEMEMBER *pMember, HTREEITEM hItem)
{
	if (pThis == NULL || hItem == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	TV_ITEMDATA *pItemData = TreeView_GetItemData(pThis->GetSafeHwnd(), pMember, hItem);

	if (pItemData != 0)
	{
		if (pItemData->pBindObject != NULL)
		{
			if (pItemData->pBindObject->bBindWLWndNeedRelease)
			{
				std::vector<CChildInfo>::iterator it = pItemData->pBindObject->vSubWLWnd.begin();
				for (; it != pItemData->pBindObject->vSubWLWnd.end(); ++it)
				{
					ASSERT(it->m_eControlType >= CT_WL_FIRST && it->m_eControlType <= CT_WL_LAST);
					CWLWnd *pWLWnd = (CWLWnd *)it->m_pChildCtrl;

					if (pWLWnd != NULL)	// May be already released
					{
						pWLWnd->DestroyWindow();
					}

					SafeDelete(pWLWnd);
				}
			}
		}

		SafeDelete(pItemData);

		pThis->SendMessage(TVM_SETITEMDATAINTERNAL, (WPARAM)hItem, 0);
	}

	HTREEITEM hChild = pThis->GetNextItem(hItem, TVGN_CHILD);
	while (hChild != NULL)
	{
		_ReleaseSubItem(pThis, pMember, hChild);

		hChild = pThis->GetNextItem(hChild, TVGN_NEXT);
	}

	return 0;
}

int _ReleaseItemData(CSkinTreeCtrl *pThis, TREEMEMBER *pMember)
{
	if (pThis == NULL)
	{
		return -1;
	}

	HTREEITEM hRoot = pThis->GetRootItem();
	while (hRoot != NULL)
	{
		_ReleaseSubItem(pThis, pMember, hRoot);

		hRoot = pThis->GetNextItem(hRoot, TVGN_NEXT);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CSkinTreeCtrl

CSkinTreeCtrl::CSkinTreeCtrl()
{
	CControlBase::m_pBindWnd = this;

	m_pMember = new TREEMEMBER;
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;
}

CSkinTreeCtrl::~CSkinTreeCtrl()
{
	ReleaseObject();

	if (m_pMember != NULL)
	{
		delete (TREEMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

BOOL CSkinTreeCtrl::Create(__in DWORD dwStyle, __in const RECT &rect, __in CWnd *pParentWnd, __in UINT nID)
{
	return CTreeCtrl::Create(dwStyle, rect, pParentWnd, nID);
}

BOOL CSkinTreeCtrl::Create(DWORD dwStyle, CRectCtrl *pRectCtrl, CWnd *pParentWnd, UINT nID)
{
	ASSERT(pRectCtrl != NULL && pRectCtrl->IsCreated());

	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	CRect rect;
	pRectCtrl->GetWindowRect(rect);

	BOOL bRet = CTreeCtrl::Create(dwStyle, rect, pParentWnd, nID);
	if (!bRet)
	{
		return FALSE;
	}

	pRectCtrl->AddBindWindow(this);
	pMember->m_pRectCtrlBind = pRectCtrl;

	return TRUE;
}

int CSkinTreeCtrl::BindStyle(LPCTSTR lpszStyleID)
{
	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (lpszStyleID == NULL)
	{
		return -2;
	}

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_TREECTRL, lpszStyleID);
	return BindStyle(pCtrlProp);

	return 0;
}

int CSkinTreeCtrl::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (pCtrlProp == NULL)
	{
		return -2;
	}
	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (ect != CT_STYLE_TREECTRL && ect != CT_TREECTRL)
	{
		return -3;
	}

	TREECTRLPROPERTIES *pTreeProp = (TREECTRLPROPERTIES *)pCtrlProp;

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pTreeProp);
	// Border
	if (pTreeProp->m_bBorder)
	{
		dwStyle |= WS_BORDER;
	}

	// Is check boxes
	if (pTreeProp->m_bCheckBoxes)
	{
		dwStyle |= TVS_CHECKBOXES;
	}

	// Is Disable drag drop
	if (pTreeProp->m_bDisableDragDrop)
	{
		dwStyle |= TVS_DISABLEDRAGDROP;
	}

	// Is Edit labels
	if (pTreeProp->m_bEditLabels)
	{
		dwStyle |= TVS_EDITLABELS;
	}

	// Is Full row select
	if (pTreeProp->m_bFullRowSel)
	{
		dwStyle |= TVS_FULLROWSELECT;
	}

	// Is Has buttons
	if (pTreeProp->m_bHasButtons)
	{
		dwStyle |= TVS_HASBUTTONS;
	}

	// Is Has lines
	if (pTreeProp->m_bHasLines)
	{
		dwStyle |= TVS_HASLINES;
	}

	// Is Info tip
	if (pTreeProp->m_bInfoTip)
	{
		dwStyle |= TVS_INFOTIP;
	}

	// Is Line at root
	if (pTreeProp->m_bLinesAtRoot)
	{
		dwStyle |= TVS_LINESATROOT;
	}

	// Is Non even height
	if (pTreeProp->m_bNonEvenHeight)
	{
		dwStyle |= TVS_NONEVENHEIGHT;
	}

	// Is Scroll
	if (!(BOOL)pTreeProp->m_bScroll)
	{
		dwStyle |= TVS_NOSCROLL;
	}

	// Is Show select always
	if (pTreeProp->m_bShowSelAlways)
	{
		dwStyle |= TVS_SHOWSELALWAYS;
	}

	// Is Single expand
	if (pTreeProp->m_bSingleExpand)
	{
		dwStyle |= TVS_SINGLEEXPAND;
	}

	// Is Tool tips
	if (!(bool)pTreeProp->m_bToolTips)
	{
		dwStyle |= TVS_NOTOOLTIPS;
	}

	// Is Track select
	if (pTreeProp->m_bTrackSel)
	{
		dwStyle |= TVS_TRACKSELECT;
	}

	DWORD dwOldStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	SetWindowLong(m_hWnd, GWL_STYLE, dwOldStyle | dwStyle);

	SetHoverScrollBarMode((bool)pTreeProp->m_bHoverScrollBarMode);

	SetTag(pTreeProp->m_strTag);

	// Set bitmaps for background
	if (pTreeProp->m_bSpecifyBackgroundImages)
	{
		CString strImageName[1 + COMBINEIMAGESIZE2];
		BOOL bCombineImage = TRUE;
		CTRLPROPERTIES::IUIGetBackground2(pCtrlProp, &bCombineImage, strImageName);
		if (bCombineImage)
		{
			CControlBase::SetBkCombineImage(strImageName[0]);
		}
		else
		{
			SetImages(CONTROL_STATE_UNCHECKED_ALL, strImageName[1], strImageName[2]);
		}
		for (int i = 0; i < 1 + COMBINEIMAGESIZE2; ++i)
		{
			ReleaseIUIImage(strImageName[i]);
		}

		SetBkImageResizeMode(pCtrlProp->m_eBkImageResizeMode);
		SetBkImageResizePoint(pCtrlProp->m_ptImageResize);
	}

	// Set bk color
	BindBkColor(pCtrlProp);

	// Item Line
	EnableItemLine(pTreeProp->m_bEnableItemLineEffect);
	COLORREF crItemLine = RGB(128, 128, 128);
	CUIMgr::GetColorResource(pTreeProp->m_strItemLineColorResID, &crItemLine);
	SetItemLineColor(crItemLine);

	// Collapse and Expanded image
	if (!CString((LPCTSTR)pTreeProp->m_strCombineCollapseExpandedImage).IsEmpty())
	{
		SetCombineCollapseExpandedImage((LPCTSTR)pTreeProp->m_strCombineCollapseExpandedImage);
	}
	else
	{
		SetCollapseImage(CONTROL_STATE_UNCHECKED_ALL,
			(LPCTSTR)pTreeProp->m_strCollapseImageN,
			(LPCTSTR)pTreeProp->m_strCollapseImageH,
			(LPCTSTR)pTreeProp->m_strCollapseImageS,
			(LPCTSTR)pTreeProp->m_strCollapseImageD);

		SetExpandedImage(CONTROL_STATE_CHECKED_ALL,
			(LPCTSTR)pTreeProp->m_strExpandedImageN,
			(LPCTSTR)pTreeProp->m_strExpandedImageH,
			(LPCTSTR)pTreeProp->m_strExpandedImageS,
			(LPCTSTR)pTreeProp->m_strExpandedImageD);
	}

	// Text horizontal alignment
	SetTextAlignHor(
		pTreeProp->m_eTextHorAlignModeN,
		pTreeProp->m_eTextHorAlignModeH,
		pTreeProp->m_eTextHorAlignModeS,
		pTreeProp->m_eTextHorAlignModeD);

	// Text horizontal alignment
	SetTextAlignVer(
		pTreeProp->m_eTextVerAlignModeN,
		pTreeProp->m_eTextVerAlignModeH,
		pTreeProp->m_eTextVerAlignModeS,
		pTreeProp->m_eTextVerAlignModeD);

	// Text Margin
	RECT rcTextMarginN = (RECT)pTreeProp->m_rcTextMarginN;
	GetDPIPadding(&rcTextMarginN);
	RECT rcTextMarginH = (RECT)pTreeProp->m_rcTextMarginH;
	GetDPIPadding(&rcTextMarginH);
	RECT rcTextMarginS = (RECT)pTreeProp->m_rcTextMarginS;
	GetDPIPadding(&rcTextMarginS);
	RECT rcTextMarginD = (RECT)pTreeProp->m_rcTextMarginD;
	GetDPIPadding(&rcTextMarginD);
	SetTextMargin(&rcTextMarginN, &rcTextMarginH, &rcTextMarginS, &rcTextMarginD);

	// Item Height
	if (!(bool)pTreeProp->m_bDefaultItemHeight)
	{
		int lItemHeight = GetDPIHeight((LONG)pTreeProp->m_nItemHeight);

		SetItemHeight((SHORT)lItemHeight);
	}

	// Set item font
	CString strResFontID[2];
	CTRLPROPERTIES::IUIGetControlFont2(pCtrlProp, strResFontID);
	SetTextFont(CONTROL_STATE_NORMAL | CONTROL_STATE_DISABLED, strResFontID[0], strResFontID[1]);

	// Set item text color
	COLORREF cr[4] = {0};
	CTRLPROPERTIES::IUIGetControlColor4(pTreeProp, &cr[0], &cr[1], &cr[2], &cr[3]);
	SetTextColor(cr[0]);
	SetHighlightItemTextColor(cr[1]);
	SetSelectedItemTextColor(cr[2]);
	SetDisabledItemTextColor(cr[3]);

	// Selected Item
	SetSelectedItemBkType((BACKGROUND_TYPE)pTreeProp->m_eSelectedItemBkType);
	if (BKT_COLOR == (BACKGROUND_TYPE)pTreeProp->m_eSelectedItemBkType)
	{
		COLORREF cr = RGB(0, 0, 128);
		CUIMgr::GetColorResource(pTreeProp->m_strSelectedItemBkColorResID, &cr);
		SetSelectedItemBkColor(cr);
	}
	else if (BKT_IMAGE == (BACKGROUND_TYPE)pTreeProp->m_eSelectedItemBkType)
	{
		SetSelectedItemBkImages(CONTROL_STATE_UNCHECKED_ND,
			(LPCTSTR)pTreeProp->m_strSelectedItemBkImageN,
			(LPCTSTR)pTreeProp->m_strSelectedItemBkImageD);

		pMember->m_ptSelectedItemBkResize = pTreeProp->m_ptSelectedItemBkResize;
	}

	// Highlight Item
	EnableHighlightItemEffect(pTreeProp->m_bEnableHighlightItemEffect);
	if (BKT_COLOR == (BACKGROUND_TYPE)pTreeProp->m_eHighlightItemBkType)
	{
		COLORREF cr = RGB(241, 241, 241);
		CUIMgr::GetColorResource(pTreeProp->m_strHighlightItemBkColorResID, &cr);
		SetHighlightItemBkColor(cr);
	}
	else if (BKT_IMAGE == (BACKGROUND_TYPE)pTreeProp->m_eHighlightItemBkType)
	{
		SetHighlightItemBkImages(CONTROL_STATE_UNCHECKED_ND,
			pTreeProp->m_strHighlightItemBkImageN, pTreeProp->m_strHighlightItemBkImageD);

		pMember->m_ptHighlightItemBkResize = pTreeProp->m_ptHighlightItemBkResize;
	}

	// Root item
	BOOL bEnableRootItemEffect = pTreeProp->m_bEnableRootItemEffect;
	EnableRootItemEffect(bEnableRootItemEffect);
	if (bEnableRootItemEffect)
	{
		// Text color
		COLORREF crText = RGB(0, 0, 0);
		CUIMgr::GetColorResource(pTreeProp->m_strRootItemTextColorResID, &crText);
		SetRootItemTextColor(crText);

		// Item background
		BACKGROUND_TYPE eBkType = pTreeProp->m_eRootItemBkType;
		if (BKT_COLOR == eBkType)
		{
			COLORREF crBk = RGB(255, 255, 255);
			CUIMgr::GetColorResource(pTreeProp->m_strRootItemBKColorResID, &crBk);
			SetRootItemBkColor(crBk);
		}
		else if (BKT_IMAGE == eBkType)
		{
			SetRootItemBkImages(CONTROL_STATE_UNCHECKED_ND,
				pTreeProp->m_strRootItemBkImageF, pTreeProp->m_strRootItemBkImageNonF);

			pMember->m_ptRootItemBkResize = pTreeProp->m_ptRootItemBkResize;
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	// Scroll bar
	bool bUseCustomScrollBar = pTreeProp->m_bUseCustomScrollBar;
	UseCustomScrollBar(bUseCustomScrollBar);
	if (bUseCustomScrollBar)
	{
		// Scroll bar width
		LONG lScrollBarWidth = (LONG)pTreeProp->m_lScrollBarWidth;
		if (CUIMgr::IsEnableDPI())
		{
			GetDPIPointX(lScrollBarWidth);
		}
		SetScrollBarWidth(lScrollBarWidth);

		// Horizontal scroll bar
		{
			CScrollBarEx *pHorzScroll = GetHorzScrollBarEx();
			pHorzScroll->BindStyle((LPCTSTR)pTreeProp->m_strHorScrollbarID);
		}

		// Vertical scroll bar
		{
			CScrollBarEx *pVertScroll = GetVertScrollBarEx();
			pVertScroll->BindStyle((LPCTSTR)pTreeProp->m_strVerScrollbarID);
		}
	}

	return 0;
}

int CSkinTreeCtrl::ReleaseObject()
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->Release();

	return CControlBase::ReleaseObject();
}

BEGIN_MESSAGE_MAP(CSkinTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CSkinTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinTreeCtrl message handlers

void CSkinTreeCtrl::OnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	NM_TREEVIEW *pNMTreeView = (NM_TREEVIEW *)pNMHDR;
	// TODO: Add your control notification handler code here
	pMember->m_bVisible = IsWindowVisible();
	if (pMember->m_bVisible)
	{
		SetRedraw(FALSE);
	}

	*pResult = 0;
}

void CSkinTreeCtrl::OnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	NM_TREEVIEW *pNMTreeView = (NM_TREEVIEW *)pNMHDR;
	// TODO: Add your control notification handler code here
	if (pMember->m_bVisible)
	{
		SetRedraw(TRUE);
		ReDrawControl(FALSE, TRUE, NULL);
		UpdateWindow();
	}

	*pResult = 0;
}

// While item selected state changed, refresh the tree
BOOL CSkinTreeCtrl::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT *pLResult)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class
	UINT nID = GetDlgCtrlID();
	NMHDR *pnmhr = (NMHDR *)lParam;

	if (message == WM_NOTIFY && nID == wParam && pnmhr->code == TVN_SELCHANGED && pMember->m_bCacheMode)
	{
		NMTREEVIEW *pTreeNm = (NMTREEVIEW *)lParam;

		// Refresh the new and old highlight item
		_LibUIDK_CSkinTreeCtrl_UpdateLevel2(this, pMember->m_bTreeFirstShowed, pMember->m_hLevel2BlendBkTextImage);
		_LibUIDK_CSkinTreeCtrl_UpdateLevel3(this, pMember->m_bTreeFirstShowed, pMember->m_hLevel2BlendBkTextImage, pMember->m_hLevel3Background, pTreeNm->itemNew.hItem, pTreeNm->itemOld.hItem);

		// If add UpdateWindow call after the follow code, and call the these code 1000 times,
		// the performance excelled call Invalidate();
		CRect rcOld;
		GetItemRect(pTreeNm->itemOld.hItem, rcOld, FALSE);
		CRect rcNew;
		GetItemRect(pTreeNm->itemNew.hItem, rcNew, FALSE);
		InvalidateRect(rcOld, FALSE);
		InvalidateRect(rcNew, FALSE);
	}

	return CTreeCtrl::OnChildNotify(message, wParam, lParam, pLResult);
}

BOOL _LibUIDK_TV_CalcScrollBars(HWND hTreeCtrl, TREEMEMBER *pMember)
{
	// UINT wMaxPos;
	BOOL fChange = FALSE;
	SCROLLINFO si;

	DWORD dwStyle = (DWORD)IUIGetWindowLong(hTreeCtrl, GWL_STYLE);

	if (dwStyle & TVS_NOSCROLL)
	{
		return FALSE;
	}

	si.cbSize = sizeof(SCROLLINFO);

	int nTotalShowHeight = (int)SendMessage(hTreeCtrl, TVM_GET_TOTAL_SHOW_HEIGHT, 0, 0);
	CRect rcClient;
	GetClientRect(hTreeCtrl, rcClient);

	if (nTotalShowHeight > rcClient.Height())
	{
		si.fMask = SIF_PAGE | SIF_RANGE;
		si.nMin = 0;
		si.nMax = nTotalShowHeight - 1;
		si.nPage = rcClient.Height();

		_LibUIDK_TV_SmoothSetTopItem(hTreeCtrl, pMember, (UINT)SetScrollInfo(hTreeCtrl, SB_VERT, &si, TRUE));
	}
	else
	{
		_LibUIDK_TV_SmoothSetTopItem(hTreeCtrl, pMember, 0);
		SetScrollRange(hTreeCtrl, SB_VERT, 0, 0, TRUE);
	}

	return (TRUE);
}

// ----------------------------------------------------------------------------
//
//  Returns the first visible item below the given item in the tree.
//
// ----------------------------------------------------------------------------

HTREEITEM _LibUIDK_TV_GetNextVisItem(HWND hTreeCtrl, HTREEITEM FAR hItem)
{
	if (hItem == NULL)
	{
		ASSERT(FALSE);
		return NULL;
	}

	HTREEITEM hKids = TreeView_GetChild(hTreeCtrl, hItem);
	int nState = TreeView_GetItemState(hTreeCtrl, hItem, TVIS_EXPANDED);
	if (hKids != NULL && (nState & TVIS_EXPANDED))
	{
		return hKids;
	}

checkNext:
	HTREEITEM hNext = TreeView_GetNextItem(hTreeCtrl, hItem, TVGN_NEXT);
	if (hNext != NULL)
	{
		return hNext;
	}

	hItem = TreeView_GetParent(hTreeCtrl, hItem);
	if (hItem)
	{
		goto checkNext;
	}

	return NULL;
}

// ----------------------------------------------------------------------------
//
//  根据当前垂直滚动信息，设置第一个可见Item的句柄，及这个item相对tree控件的垂直偏移
//
// ----------------------------------------------------------------------------

HTREEITEM NEAR _LibUIDK_TV_GetShownIndexItem(TREEMEMBER *pMember, HTREEITEM hItem, HWND hTreeCtrl, int wNewScrollPos)
{
	HTREEITEM hRoot = (HTREEITEM)::SendMessage(hTreeCtrl, TVM_GETNEXTITEM, TVGN_ROOT, 0);
	if (hRoot == NULL)
	{
		return NULL;
	}

	int nBranchHeight = (int)SendMessage(hTreeCtrl, TVM_GET_TOTAL_SHOW_HEIGHT, 0, 0);
	CRect rcClient;
	GetClientRect(hTreeCtrl, rcClient);

	HTREEITEM hTop = NULL;

	if (nBranchHeight <= rcClient.Height())
	{
		// 一屏可显示下
		pMember->m_hTop = hRoot;
		hTop = hRoot;
		pMember->m_nTopItemYOffset = 0;
		pMember->m_nTopItemYScrollPos = 0;
	}
	else
	{
		// 查询哪个Item的top小于tree的bottom，并且bottom大于tree的top
		int nCountHeight = 0; // 已统计过的高度
		HTREEITEM hCurItem = hRoot;
		int nDefaultItemHeight = (SHORT)::SendMessage(hTreeCtrl, TVM_GETITEMHEIGHT, 0, 0L);

		while (hCurItem != NULL)
		{
			if (nCountHeight > wNewScrollPos)
			{
				break;
			}

			//
			// 如果整组不在视野范围内，整组跳过
			//
			TV_ITEMDATA *pTvItemData = TreeView_GetItemData(hTreeCtrl, pMember, hCurItem);


			// 只有展开的Item，才统计它的branch高度。
			int nBranchHeight = 0;
			int nState = TreeView_GetItemState(hTreeCtrl, hCurItem, TVIS_EXPANDED);
			if (IsIncludeFlag(nState, TVIS_EXPANDED))
			{
				nBranchHeight = pTvItemData->nBranchHeight;
			}

			int nItemHeight = (pTvItemData->nItemHeight == 0 ? nDefaultItemHeight : pTvItemData->nItemHeight);
			if (nCountHeight + nItemHeight + nBranchHeight <= wNewScrollPos)
			{
				hCurItem = (HTREEITEM)::SendMessage(hTreeCtrl, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hCurItem);
				nCountHeight += nItemHeight + nBranchHeight;
				continue;
			}

			//
			// 命中
			//
			if (nCountHeight <= wNewScrollPos && nCountHeight + nItemHeight > wNewScrollPos)
			{
				pMember->m_hTop = hCurItem;
				hTop = hCurItem;
				pMember->m_nTopItemYOffset = nCountHeight - wNewScrollPos;
				pMember->m_nTopItemYScrollPos = wNewScrollPos;

				break;
			}

			//
			// 下一个可视Item
			//
			hCurItem = _LibUIDK_TV_GetNextVisItem(hTreeCtrl, hCurItem);
			nCountHeight += nItemHeight;
		}
	}

	return hTop;
}

// ----------------------------------------------------------------------------
//
//  与_LibUIDK_TV_GetShownIndexItem的作用相反，根据当前第一个可见Item，计算滚动位置
//
// ----------------------------------------------------------------------------

int NEAR _LibUIDK_TV_GetTopItemScrollPos(TREEMEMBER *pMember, HTREEITEM hTopItem, HWND hTreeCtrl)
{
	HTREEITEM hRoot = (HTREEITEM)::SendMessage(hTreeCtrl, TVM_GETNEXTITEM, TVGN_ROOT, 0);
	if (hRoot == NULL)
	{
		return 0;
	}

	int nBranchHeight = (int)SendMessage(hTreeCtrl, TVM_GET_TOTAL_SHOW_HEIGHT, 0, 0);
	CRect rcClient;
	GetClientRect(hTreeCtrl, rcClient);

	if (nBranchHeight <= rcClient.Height())
	{
		// 一屏可显示下
		return 0;
	}
	else
	{
		int nCountHeight = 0;
		int nItemDefaultHeight = TreeView_GetItemHeight(hTreeCtrl);
		HTREEITEM hItem = TreeView_GetRoot(hTreeCtrl);
		while (hItem != NULL)
		{
			if (hItem == hTopItem)
			{
				pMember->m_nTopItemYScrollPos = nCountHeight - pMember->m_nTopItemYOffset;
				break;
			}

			TV_ITEMDATA *pData = TreeView_GetItemData(hTreeCtrl, pMember, hItem);

			if (pData->nItemHeight == 0)
			{
				nCountHeight += nItemDefaultHeight;
			}
			else
			{
				nCountHeight += pData->nItemHeight;
			}

			hItem = _LibUIDK_TV_GetNextVisItem(hTreeCtrl, hItem);
		}

		return pMember->m_nTopItemYScrollPos;
	}

	return 0;
}

BOOL TreeView_IsRedraw(HWND hTreeCtrl)
{
	return (BOOL)SendMessage(hTreeCtrl, WM_ISREDRAW, 0, 0);
}

// ----------------------------------------------------------------------------
//
//  Sets position of vertical scroll bar and scrolls window to match that
//  position
//
//  每个树控件内部都定义了一个hTop变量，用来记录第一个肉眼可见的Item.
//  本函数，会重新计算hTop
//
// ----------------------------------------------------------------------------

BOOL NEAR _LibUIDK_TV_SmoothSetTopItem(HWND hTreeCtrl, TREEMEMBER *pMember, UINT wNewTop)
{
	HTREEITEM hItem = TreeView_GetRoot(hTreeCtrl);

	if (!hItem)
	{
		return FALSE;
	}

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	if (!GetScrollInfo(hTreeCtrl, SB_VERT, &si))
	{
		return FALSE;
	}

	DWORD dwStyle = (DWORD)IUIGetWindowLong(hTreeCtrl, GWL_STYLE);

	if ((dwStyle & TVS_NOSCROLL) || (wNewTop == (UINT) - 1))
	{
		// we've wrapped around (treat as a negative index) -- use min pos
		// or there aren't enough items to scroll
		wNewTop = 0;
	}

	if (wNewTop > si.nMax - si.nPage + 1)
	{
		// we've gone too far down -- use max pos
		wNewTop = si.nMax - si.nPage + 1;
	}

	hItem = _LibUIDK_TV_GetShownIndexItem(pMember, hItem, hTreeCtrl, wNewTop);

	// BUGBUG (scotth): refreshing in regedit sometimes hits this case
	ASSERT(hItem);

	if (NULL == hItem)
	{
		return FALSE;
	}

	SetScrollPos(hTreeCtrl, SB_VERT, wNewTop, TRUE);

	return (TRUE);
}

// ----------------------------------------------------------------------------
//
//  WM_VSCROLL的响应函数
//  只设置Tree控件的第一个可视Item m_hTop。 m_hTop下面的可视Item，在绘制的时候布局。
//
// ----------------------------------------------------------------------------
BOOL NEAR _LibUIDK_TV_VertScroll(HWND hTreeCtrl, TREEMEMBER *pMember, UINT wCode, UINT wOldPos, UINT wPos)
{
	int wNewPos = 0;
	BOOL fChanged;

	if (SendMessage(hTreeCtrl, TVM_GET_TOP_ITEM, 0, 0) == NULL)
	{
		return FALSE;
	}

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	if (!GetScrollInfo(hTreeCtrl, SB_VERT, &si))
	{
		return FALSE;
	}

	switch (wCode)
	{
	case SB_BOTTOM:
		wNewPos = si.nMax;
		break;

	case SB_ENDSCROLL:
		wNewPos = wOldPos;
		break;

	case SB_LINEDOWN:
		wNewPos = wOldPos + 1;
		break;

	case SB_LINEUP:
		wNewPos = wOldPos - 1;
		if (wNewPos < 0)
		{
			wNewPos = 0;
		}
		break;

	case SB_PAGEDOWN:
		wNewPos = wOldPos + si.nPage;
		break;

	case SB_PAGEUP:
		wNewPos = wOldPos - si.nPage;
		if (wNewPos < 0)
		{
			wNewPos = 0;
		}
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		wNewPos = wPos;
		break;

	case SB_TOP:
		wNewPos = 0;
		break;
	}

	if (fChanged = _LibUIDK_TV_SmoothSetTopItem(hTreeCtrl, pMember, wNewPos))
	{
		InvalidateRect(hTreeCtrl, NULL, FALSE);
		UpdateWindow(hTreeCtrl);
	}
	return (fChanged);
}

TV_ITEMDATA *TreeView_GetItemData(HWND hwnd, TREEMEMBER *pMember, HTREEITEM hItem)
{
	TVITEM item;
	item.hItem = hItem;
	item.mask = TVIF_PARAM;
	pMember->m_bSetItemDataInternal = TRUE;
	::SendMessage(hwnd, TVM_GETITEM, 0, (LPARAM)&item);
	pMember->m_bSetItemDataInternal = FALSE;

	TV_ITEMDATA *pItemData = (TV_ITEMDATA *)item.lParam;
	if (pItemData == NULL)
	{
		pItemData = new TV_ITEMDATA;
		item.lParam = (LPARAM)pItemData;

		pMember->m_bSetItemDataInternal = TRUE;
		::SendMessage(hwnd, TVM_SETITEM, 0, (LPARAM)&item);
		pMember->m_bSetItemDataInternal = FALSE;
	}

	return pItemData;
}

// 在tree控件插入item后，更新展开的item的总高度
// 如果这是第一个被插入的item，把它设置为m_hTop
int UpdateBranchHeightAfterAddItem(HWND hTreeCtrl, TREEMEMBER *pMember, HTREEITEM hNewItem)
{
	ASSERT(::IsWindow(hTreeCtrl));
	if (hNewItem == NULL)
	{
		return -1;
	}

	TV_ITEMDATA *pItemData = TreeView_GetItemData(hTreeCtrl, pMember, hNewItem);
	ASSERT(pItemData != NULL);

	int nBranchHeightAdd = 0;
	if (pItemData->nItemHeight == 0)
	{
		nBranchHeightAdd = (SHORT)::SendMessage(hTreeCtrl, TVM_GETITEMHEIGHT, 0, 0L);
	}
	else
	{
		nBranchHeightAdd = pItemData->nItemHeight;
	}

	// tree内容增加的总高度，与nBranchHeightAdd意义不一样。
	// 当新的Item的父Item是折叠状态时，nTotalHeightAdd为0.
	int nTotalHeightAdd = 0;

	//
	// 1. 向上一级级更新nBranchHeight
	//
	HTREEITEM hParent = (HTREEITEM)::SendMessage(hTreeCtrl, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hNewItem);

	// 如果这是第一个被插入的item, 设置第一个可见的item m_hTop
	if ((hParent == NULL) || (hParent == TVI_ROOT))
	{
		nTotalHeightAdd = (SHORT)::SendMessage(hTreeCtrl, TVM_GETITEMHEIGHT, 0, 0L);

		HTREEITEM hTop = (HTREEITEM)::SendMessage(hTreeCtrl, TVM_GET_TOP_ITEM, 0, 0);
		if (hTop == NULL)
		{
			::SendMessage(hTreeCtrl, TVM_SET_TOP_ITEM, 0, (LPARAM)hNewItem);
		}
	}

	while (hParent != NULL)
	{
		TV_ITEMDATA *pParentItemData = TreeView_GetItemData(hTreeCtrl, pMember, hParent);

		// 直接父Item,不论是不是折叠状态，nBranchHeight都增加
		pParentItemData->nBranchHeight += nBranchHeightAdd;

		// 碰到折叠的Item，就不再向上级父item更新了。
		int nState = TreeView_GetItemState(hTreeCtrl, hParent, TVIS_EXPANDED);
		if (IsIncludeFlag(nState, TVIS_EXPANDED))
		{
			nTotalHeightAdd = nBranchHeightAdd;
		}
		else
		{
			break;
		}

		hParent = TreeView_GetParent(hTreeCtrl, hParent);
	}

	//
	// 2. 更新tree内容总高度
	//
	if (nTotalHeightAdd != 0)
	{
		SendMessage(hTreeCtrl, TVM_SET_TOTAL_SHOW_HEIGHT, TRUE, nTotalHeightAdd);
	}

	return 0;
}

// 在tree控件删除item后，更新展开的item的总高度
int UpdateBranchHeightAfterDeleteItem(HWND hTreeCtrl, TREEMEMBER *pMember, HTREEITEM hDeleteItem)
{
	ASSERT(::IsWindow(hTreeCtrl));
	if (hDeleteItem == NULL)
	{
		return -1;
	}

	TV_ITEMDATA *pItemData = TreeView_GetItemData(hTreeCtrl, pMember, hDeleteItem);
	ASSERT(pItemData != NULL);

	int nBranchHeightDelete = 0;
	if (pItemData->nItemHeight == 0)
	{
		nBranchHeightDelete = (SHORT)::SendMessage(hTreeCtrl, TVM_GETITEMHEIGHT, 0, 0L);
	}
	else
	{
		nBranchHeightDelete = pItemData->nItemHeight;
	}

	int nState = TreeView_GetItemState(hTreeCtrl, hDeleteItem, TVIS_EXPANDED);
	if (IsIncludeFlag(nState, TVIS_EXPANDED))
	{
		nBranchHeightDelete += pItemData->nBranchHeight;
	}

	// tree内容减小的总高度，与nBranchHeightDelete意义不一样。
	// 当删除的Item的父Item是折叠状态时，nTotalHeighDelete为0.
	int nTotalHeighDelete = 0;

	//
	// 1. 向上一级级更新nBranchHeight
	//
	HTREEITEM hParent = (HTREEITEM)::SendMessage(hTreeCtrl, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hDeleteItem);
	while (hParent != NULL)
	{
		TV_ITEMDATA *pParentItemData = TreeView_GetItemData(hTreeCtrl, pMember, hParent);

		// 不论父item是不是折叠状态，nBranchHeight都减小
		pParentItemData->nBranchHeight -= nBranchHeightDelete;

		// 碰到折叠的Item，就不再向上级父item更新了。
		int nState = TreeView_GetItemState(hTreeCtrl, hParent, TVIS_EXPANDED);
		if (IsIncludeFlag(nState, TVIS_EXPANDED))
		{
			nTotalHeighDelete = nBranchHeightDelete;
		}
		else
		{
			break;
		}

		hParent = TreeView_GetParent(hTreeCtrl, hParent);
	}

	//
	// 2. 更新tree内容总高度
	//
	if (nTotalHeighDelete != 0)
	{
		SendMessage(hTreeCtrl, TVM_SET_TOTAL_SHOW_HEIGHT, TRUE, -nTotalHeighDelete);
	}

	return 0;
}

// 在tree控件的item高度发生变化后，更新可见的item的总高度
int UpdateBranchHeightAfterItemHeightChanged(HWND hTreeCtrl, TREEMEMBER *pMember, HTREEITEM hItem,
	int nItemOldHeight, int nItemNewHeight)
{
	ASSERT(::IsWindow(hTreeCtrl));
	if (hItem == NULL)
	{
		return -1;
	}

	// 分支高度增加的值
	int nBranchHeightAdd = nItemNewHeight - nItemOldHeight;

	// tree内容增加的总高度，与nBranchHeightAdd意义不一样。
	// 当新的Item的父Item是折叠状态时，nTotalHeightAdd为0.
	int nTotalHeightAdd = 0;

	HTREEITEM hParent = (HTREEITEM)::SendMessage(hTreeCtrl, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hItem);
	if (hParent == NULL || hParent == TVI_ROOT)
	{
		nTotalHeightAdd = nBranchHeightAdd;
	}

	//
	// 1. 向上一级级更新nBranchHeight
	//
	while (hParent != NULL)
	{
		TV_ITEMDATA *pParentItemData = TreeView_GetItemData(hTreeCtrl, pMember, hParent);

		// 直接父Item,不论是不是折叠状态，nBranchHeight都增加
		pParentItemData->nBranchHeight += nBranchHeightAdd;

		// 碰到折叠的Item，就不再向上级父item更新了。
		int nState = TreeView_GetItemState(hTreeCtrl, hParent, TVIS_EXPANDED);
		if (IsIncludeFlag(nState, TVIS_EXPANDED))
		{
			nTotalHeightAdd = nBranchHeightAdd;
		}
		else
		{
			break;
		}

		hParent = TreeView_GetParent(hTreeCtrl, hParent);
	}

	//
	// 2. 更新tree内容总高度
	//
	if (nTotalHeightAdd != 0)
	{
		SendMessage(hTreeCtrl, TVM_SET_TOTAL_SHOW_HEIGHT, TRUE, nTotalHeightAdd);
	}

	return 0;
}

// 在tree控件展开某个item后，更新可见的item的总高度
int UpdateBranchHeightAfterExpand(HWND hTreeCtrl, TREEMEMBER *pMember, HTREEITEM hExpandItem, BOOL bOldExpand, BOOL bNewExpand)
{
	ASSERT(::IsWindow(hTreeCtrl));
	if (hExpandItem == NULL)
	{
		return -1;
	}

	//
	// 1. 被展开或合拢的Item的父Item增加的分支高度
	//
	int nParentBranchHeightAdd = 0;
	BOOL fChange = FALSE; // 是否有高度的改变。

	if (bOldExpand)
	{
		// 折叠了item
		if (!bNewExpand)
		{
			TV_ITEMDATA *pData = TreeView_GetItemData(hTreeCtrl, pMember, hExpandItem);
			nParentBranchHeightAdd = -pData->nBranchHeight;
			fChange = TRUE;
		}
	}
	else
	{
		// 展开了item
		if (bNewExpand)
		{
			TV_ITEMDATA *pData = TreeView_GetItemData(hTreeCtrl, pMember, hExpandItem);
			nParentBranchHeightAdd = pData->nBranchHeight;
			fChange = TRUE;
		}
	}

	if (!fChange)
	{
		return 0;
	}

	//
	// Tree内容总高度是否增加，只有被展开或折叠的Item的所有父Item全部是展开状态，才增加。
	//
	BOOL bTotalHeightAdd = TRUE;

	//
	// 2. 向上一级级更新nBranchHeight, 被折叠或展开的item不需要更新nBranchHeight
	//
	HTREEITEM hParent = (HTREEITEM)::SendMessage(hTreeCtrl, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hExpandItem);

	// 直接父Item，不论是否折叠，都要更新nBranchHeight
	if (hParent != NULL)
	{
		TV_ITEMDATA *pParentItemData = TreeView_GetItemData(hTreeCtrl, pMember, hParent);
		pParentItemData->nBranchHeight += nParentBranchHeightAdd;

		UINT nState = TreeView_GetItemState(hTreeCtrl, hParent, TVIS_EXPANDED);
		if (!IsIncludeFlag(nState, TVIS_EXPANDED))
		{
			bTotalHeightAdd = FALSE;
		}
	}

	while (hParent != NULL)
	{
		hParent = (HTREEITEM)::SendMessage(hTreeCtrl, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hParent);
		if (hParent == NULL)
		{
			break;
		}

		UINT nState = TreeView_GetItemState(hTreeCtrl, hParent, TVIS_EXPANDED);

		// 碰到折叠的Item，就不再向上级父item更新了。
		if (IsIncludeFlag(nState, TVIS_EXPANDED))
		{
			TV_ITEMDATA *pParentItemData = TreeView_GetItemData(hTreeCtrl, pMember, hParent);
			pParentItemData->nBranchHeight += nParentBranchHeightAdd;
		}
		else
		{
			bTotalHeightAdd = FALSE;
			break;
		}
	}

	if (bTotalHeightAdd != 0)
	{
		SendMessage(hTreeCtrl, TVM_SET_TOTAL_SHOW_HEIGHT, TRUE, nParentBranchHeightAdd);
	}

	return 0;
}

// ----------------------------------------------------------------------------
//
//  Determine what part of what item is at the given (x,y) location in the
//  tree's client area.  If the location is outside the client area, NULL is
//  returned with the TVHT_TOLEFT, TVHT_TORIGHT, TVHT_ABOVE, and/or TVHT_BELOW
//  flags set in the wHitCode as appropriate.  If the location is below the
//  last item, NULL is returned with wHitCode set to TVHT_NOWHERE.  Otherwise,
//  the item is returned with wHitCode set to either TVHT_ONITEMINDENT,
//  TVHT_ONITEMBUTTON, TVHT_ONITEMICON, TVHT_ONITEMLABEL, or TVHT_ONITEMRIGHT
//
// ----------------------------------------------------------------------------

HTREEITEM _LibUIDK_TV_CheckHit(HWND hTreeCtrl, TREEMEMBER *pMember, int x, int y, UINT FAR *wHitCode)
{
	HTREEITEM hItem = (HTREEITEM)SendMessage(hTreeCtrl, TVM_GET_TOP_ITEM, 0, 0);
	int cxState;

	TVITEMEX sItem;

	*wHitCode = 0;

	CRect rcWnd;
	GetWindowRect(hTreeCtrl, rcWnd);

	if (x < 0)
	{
		*wHitCode |= TVHT_TOLEFT;
	}
	else if (x > (int)rcWnd.Width())
	{
		*wHitCode |= TVHT_TORIGHT;
	}

	if (y < 0)
	{
		*wHitCode |= TVHT_ABOVE;
	}
	else if (y > (int)rcWnd.Height())
	{
		*wHitCode |= TVHT_BELOW;
	}

	if (*wHitCode)
	{
		return NULL;
	}

	HTREEITEM hItemRet = NULL;
	HTREEITEM hWolk = hItem;
	int nTopYOffset = (int)SendMessage(hTreeCtrl, TVM_GET_TOP_ITEM_YOFFSET, 0, 0);
	int nCountHeight = 0;
	int nDefaultItemHeight = TreeView_GetItemHeight(hTreeCtrl);
	int cxIndent = TreeView_GetIndent(hTreeCtrl);
	while (hWolk != NULL)
	{
		if (nCountHeight > y - nTopYOffset)
		{
			break;
		}

		// 如果整组不在视野范围内，整组跳过
		TV_ITEMDATA *pTvItemData = TreeView_GetItemData(hTreeCtrl, pMember, hWolk);

		int nBranchHeight = pTvItemData->nBranchHeight;
		int nItemHeight = (pTvItemData->nItemHeight == 0 ? nDefaultItemHeight : pTvItemData->nItemHeight);
		if (nCountHeight + nItemHeight + nBranchHeight <= y - nTopYOffset)
		{
			HTREEITEM hOldItem = hWolk;
			hWolk = (HTREEITEM)::SendMessage(hTreeCtrl, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hWolk);

			if (hWolk == NULL)
			{
				hWolk = _LibUIDK_TV_GetNextVisItem(hTreeCtrl, hOldItem);
			}

			nCountHeight += nItemHeight + nBranchHeight;
			continue;
		}

		if (nCountHeight <= y - nTopYOffset && nCountHeight + nItemHeight > y - nTopYOffset)
		{
			hItemRet = hWolk;
			break;
		}

		hWolk = _LibUIDK_TV_GetNextVisItem(hTreeCtrl, hWolk);
		nCountHeight += nItemHeight;
	}

	if (!hItemRet)
	{
		*wHitCode = TVHT_NOWHERE;
		return NULL;
	}
	CSize sizeBorder = (DWORD)SendMessage(hTreeCtrl, TVM_GETBORDER, 0, 0);
	x -= (sizeBorder.cx + (hItemRet->iLevel * cxIndent));
	x += GetScrollPos(hTreeCtrl, SB_HORZ);

	LONG_PTR lStyle = IUIGetWindowLong(hTreeCtrl, GWL_STYLE);
	if ((lStyle & (TVS_HASLINES | TVS_HASBUTTONS)) &&
		(lStyle & TVS_LINESATROOT))
	{
		// Subtract some more to make up for the pluses at the root
		x -= cxIndent;
	}

	// instead of TV_GetItem(pTree, hItemRet, TVIF_CHILDREN, &sItem);
	sItem.mask = TVIF_CHILDREN;
	sItem.hItem = hItemRet;
	TreeView_GetItem(hTreeCtrl, &sItem);

	CSize sizeState = (DWORD)SendMessage(hTreeCtrl, TVM_GET_STATE_IMAGE_SIZE, 0, 0);
	CSize sizeImage = (DWORD)SendMessage(hTreeCtrl, TVM_GET_NORMAL_IMAGE_SIZE, 0, 0);
	cxState = TV_StateIndex(&sItem) ? sizeState.cx : 0;
	if (x <= (int)(hItemRet->iWidth + sizeImage.cx + cxState))
	{
		if (x >= 0)
		{
			if (TreeView_GetImageList(hTreeCtrl, TVSIL_STATE) != NULL && (x < cxState))
			{
				*wHitCode = TVHT_ONITEMSTATEICON;
			}
			else if (TreeView_GetImageList(hTreeCtrl, TVSIL_NORMAL) != NULL
				&& (x < (int) sizeImage.cx + cxState))
			{
				*wHitCode = TVHT_ONITEMICON;
			}
			else
			{
				*wHitCode = TVHT_ONITEMLABEL;
			}
		}
		else if ((x >= -cxIndent) && sItem.cChildren && (lStyle & TVS_HASBUTTONS))
		{
			*wHitCode = TVHT_ONITEMBUTTON;
		}
		else
		{
			*wHitCode = TVHT_ONITEMINDENT;
		}
	}
	else
	{
		*wHitCode = TVHT_ONITEMRIGHT;
	}

	return hItemRet;
}

HTREEITEM NEAR _LibUIDK_TV_OnHitTest(HWND hTreeCtrl, TREEMEMBER *pMember, LPTV_HITTESTINFO lptvh)
{
	if (!lptvh)
	{
		return 0;    //BUGBUG: Validate LPTVHITTEST
	}

	lptvh->hItem = _LibUIDK_TV_CheckHit(hTreeCtrl, pMember, lptvh->pt.x, lptvh->pt.y, &lptvh->flags);

	return lptvh->hItem;
}

void NEAR _LibUIDK_TV_CancelEditTimer(PTREE pTree)
{
	if (pTree->fNameEditPending)
	{
		KillTimer(pTree->ci.hwnd, IDT_NAMEEDIT);
		pTree->fNameEditPending = FALSE;
	}
}

//  This is tricky because CheckForDragBegin yields and the app may have
//  destroyed the item we are thinking about dragging
//
//  To give the app some feedback, we give the hItem the drop highlight
//  if it isn't already the caret.  This also allows us to check if the
//  item got deleted behind our back - TV_DeleteItemRecurse makes sure
//  that deleted items are never the hCaret or hDropTarget.
//
//  After TV_CheckForDragBegin, the caller must call TV_FinishCheckDrag
//  to clean up the UI changes that TV_CheckForDragBegin temporarily
//  performed.
//
BOOL _LibUIDK_TV_CheckForDragBegin(PTREE pTree, HWND hTreeCtrl, HTREEITEM hItem, int x, int y)
{
	BOOL fDrag;

	//
	//  If the item is not the caret, then make it the (temporary)
	//  drop target so the user gets some feedback.
	//
	//  BUGBUG raymondc - If hItem == pTree->hCaret, it still might not
	//  be visible if the control doesn't yet have focus and the treeview
	//  is not marked showselalways.  Maybe we should just always set
	//  hItem to DROPHILITE.
	//
	HTREEITEM hCaret = TreeView_GetNextItem(hTreeCtrl, NULL, TVGN_CARET);
	if (hItem == hCaret)
	{
		pTree->hOldDrop = NULL;
		pTree->fRestoreOldDrop = FALSE;
	}
	else
	{
		pTree->hOldDrop = pTree->hDropTarget;
		pTree->fRestoreOldDrop = TRUE;
		TV_SelectItem(pTree, TVGN_DROPHILITE, hItem, 0, TVC_BYMOUSE);
		ASSERT(hItem == pTree->hDropTarget);
	}

	//
	//  We are dragging the hItem if CheckForDragBegin says okay,
	//  and TV_DeleteItemRecurse didn't wipe us out.
	//
	fDrag = CheckForDragBegin(hTreeCtrl, x, y) &&
		(hItem == pTree->hDropTarget || hItem == hCaret);

	return fDrag;
}

void _LibUIDK_TV_FinishCheckDrag(PTREE pTree)
{
	//
	//  Clean up our temporary UI changes that happened when we started
	//  dragging.
	//
	if (pTree->fRestoreOldDrop)
	{
		HTREEITEM hOldDrop = pTree->hOldDrop;
		pTree->fRestoreOldDrop = FALSE;
		pTree->hOldDrop = NULL;
		TV_SelectItem(pTree, TVGN_DROPHILITE, hOldDrop, 0, TVC_BYMOUSE);
	}
}

// ----------------------------------------------------------------------------
//
// If the tooltip bubble is up, then pop it.
//
// ----------------------------------------------------------------------------

void _LibUIDK_TV_PopBubble(HWND hTreeCrl)
{
	HWND hToolTips = TreeView_GetToolTips(hTreeCrl);
	if (hToolTips)
	{
		SendMessage(hToolTips, TTM_POP, 0L, 0L);
	}
}

// ----------------------------------------------------------------------------
//
//  Sends a TVN_BEGINDRAG or TVN_BEGINRDRAG notification with information in the ptDrag and
//  itemNew fields of an NM_TREEVIEW structure
//
// ----------------------------------------------------------------------------

BOOL NEAR _LibUIDK_TV_SendBeginDrag(PTREE pTree, HWND hTreeCtrl, int code, TREEITEM FAR *hItem, int x, int y)
{
	NM_TREEVIEW nm;

	_LibUIDK_TV_PopBubble(hTreeCtrl);            // dismiss the infotip if we start to drag

	nm.itemNew.hItem = hItem;
	nm.itemNew.state = hItem->state;
	nm.itemNew.lParam = hItem->lParam;
	nm.itemNew.mask = (TVIF_HANDLE | TVIF_STATE | TVIF_PARAM);
	nm.itemOld.mask = 0;
	nm.ptDrag.x = x;
	nm.ptDrag.y = y;

	return (BOOL)CCSendNotify(&pTree->ci, code, &nm.hdr);
}

void _LibUIDK_TV_HandleStateIconClick(HWND hTreeCtrl, HTREEITEM hItem)
{
	// instead of TV_GetItem(pTree, hItem, TVIF_STATE, &tvi);
	TVITEMEX tvi;
	tvi.mask = TVIF_STATE;
	tvi.stateMask = TVIS_STATEIMAGEMASK;
	tvi.hItem = hItem;
	TreeView_GetItem(hTreeCtrl, &tvi);

	int iState = STATEIMAGEMASKTOINDEX(tvi.state & tvi.stateMask);
	iState %= (ImageList_GetImageCount(TreeView_GetImageList(hTreeCtrl, TVSIL_STATE)) - 1);
	iState++;

	tvi.mask = TVIF_STATE;
	tvi.state = INDEXTOSTATEIMAGEMASK(iState);
	tvi.hItem = hItem;

	TreeView_SetItem(hTreeCtrl, &tvi);
}

// ----------------------------------------------------------------------------
//
//  WM_LBUTTONDBLCLK message -- toggle expand/collapse state of item's children
//  WM_LBUTTONDOWN message -- on item's button, do same as WM_LBUTTONDBLCLK,
//  otherwise select item and ensure that item is fully visible
//
// ----------------------------------------------------------------------------

void NEAR _LibUIDK_TV_ButtonDown(PTREE pTree, HWND hTreeCtrl, UINT wMsg, UINT wFlags, int x, int y, UINT TVBD_flags)
{
	UINT wHitCode;
	HTREEITEM hItem;
	HWND hwndTree;
	LRESULT lResult;
#ifdef _X86_
	NMEUDORA nmeu;
	nmeu.MustBeNonzero = 1;
	COMPILETIME_ASSERT(FIELD_OFFSET(NMEUDORA, MustBeNonzero) == 0x3C);
#endif

	POINT pt;
	GetMessagePosClient(hTreeCtrl, &pt);
	SendMessage(hTreeCtrl, TVM_SET_CAPTURE_POINT, 0, MAKELPARAM(pt.x, pt.y));

	// instead of TV_DismissEdit(pTree, FALSE)
	BOOL bRet = (BOOL)SendMessage(hTreeCtrl, TVM_ENDEDITLABELNOW, FALSE, 0);
	if (!bRet)   // end any previous editing (accept it)
	{
		return;    // Something happened such that we should not process button down
	}

	// Instead of TV_CheckHit
	TV_HITTESTINFO ht;
	ht.pt = CPoint(x, y);
	TreeView_HitTest(hTreeCtrl, &ht);
	wHitCode = ht.flags;
	hItem = ht.hItem;

	// Excel likes to destroy the entire tree when it gets a double-click
	// so we need to watch the item in case it vanishes behind our back.
	hwndTree = hTreeCtrl;
	LONG_PTR lStyle = IUIGetWindowLong(hTreeCtrl, GWL_STYLE);

	if (wMsg == WM_LBUTTONDBLCLK)
	{
		//
		// Cancel any name editing that might happen.
		//

		_LibUIDK_TV_CancelEditTimer(pTree);

		int cxIndent = TreeView_GetIndent(hTreeCtrl);
		if (wHitCode & (TVHT_ONITEM | TVHT_ONITEMBUTTON))
		{
			goto ExpandItem;
		}

		//
		// Collapses node above the line double clicked on
		//
		else if ((lStyle & TVS_HASLINES) && (wHitCode & TVHT_ONITEMINDENT) &&
			(abs(x % cxIndent - cxIndent / 2) <= g_cxDoubleClk))
		{

			int i;

			for (i = hItem->iLevel - x / cxIndent + ((lStyle & TVS_LINESATROOT) ? 1 : 0); i > 1; i--)
			{
				hItem = hItem->hParent;
			}

ExpandItem:
#ifdef _X86_
			lResult = CCSendNotify(&pTree->ci, wFlags & MK_RBUTTON ? NM_RDBLCLK : NM_DBLCLK, &nmeu.nmhdr);
#else
			lResult = CCSendNotify(&pTree->ci, wFlags & MK_RBUTTON ? NM_RDBLCLK : NM_DBLCLK, NULL);
#endif
			if (!IsWindow(hwndTree))
			{
				goto bail;
			}
			if (!lResult)
			{
				// don't auto expand this if we're in single expand mode because the first click did it already
				if (!(lStyle & TVS_SINGLEEXPAND))
				{
					TV_Expand(pTree, TVE_TOGGLE, hItem, TRUE);
				}
			}

		}

		pTree->fScrollWait = FALSE;
	}
	else        // WM_LBUTTONDOWN
	{
		if (wHitCode == TVHT_ONITEMBUTTON)
		{
			if (!CCSendNotify(&pTree->ci, NM_CLICK, NULL))
			{
				if (TVBD_flags & TVBD_FROMWHEEL)
				{
					TV_Expand(pTree, (TVBD_flags & TVBD_WHEELFORWARD) ? TVE_EXPAND : TVE_COLLAPSE, hItem, TRUE);
				}
				else
				{
					TV_Expand(pTree, TVE_TOGGLE, hItem, TRUE);
				}
			}
		}
		else if (wHitCode & TVHT_ONITEM ||
			((lStyle & TVS_FULLROWSELECT) && (wHitCode & (TVHT_ONITEMRIGHT | TVHT_ONITEMINDENT))))
		{
			BOOL fSameItem, bDragging;

			ASSERT(hItem);

			HTREEITEM hCaret = TreeView_GetNextItem(hTreeCtrl, NULL, TVGN_CARET);
			fSameItem = (hItem == hCaret);

			if (TVBD_flags & TVBD_FROMWHEEL)
			{
				bDragging = FALSE;
			}
			else if (lStyle & TVS_DISABLEDRAGDROP)
			{
				bDragging = FALSE;
			}
			else
			{
				bDragging = _LibUIDK_TV_CheckForDragBegin(pTree, hTreeCtrl, hItem, x, y);
				_LibUIDK_TV_FinishCheckDrag(pTree);
			}

			if (bDragging)
			{
				pTree->htiDrag = hItem;
				_LibUIDK_TV_SendBeginDrag(pTree, hTreeCtrl, TVN_BEGINDRAG, hItem, x, y);
				return;
			}

			if (!CCSendNotify(&pTree->ci, NM_CLICK, NULL))
			{

				if (wHitCode == TVHT_ONITEMSTATEICON &&
					(lStyle & TVS_CHECKBOXES))
				{
					_LibUIDK_TV_HandleStateIconClick(hTreeCtrl, hItem);
				}
				else
				{
					// Only set the caret (selection) if not dragging
					// instead of TV_SelectItem(pTree, TVGN_CARET, hItem, TVSIF_NOTIFY | TVSIF_UPDATENOW, TVC_BYMOUSE);
					TreeView_SelectItem(hTreeCtrl, hItem);

					if (fSameItem && (wHitCode & TVHT_ONITEMLABEL) && pTree->fFocus)
					{
						//
						// The item and window are currently selected and user clicked
						// on label.  Try to enter into name editing mode.
						//
						SetTimer(hTreeCtrl, IDT_NAMEEDIT, GetDoubleClickTime(), NULL);
						pTree->fNameEditPending = TRUE;
					}

					if (fSameItem && lStyle & TVS_SINGLEEXPAND)
					{
						// single click on the focus item toggles expand state
						TV_Expand(pTree, TVE_TOGGLE, hCaret, TRUE);
					}
				}
			}
		}
		else
		{
			CCSendNotify(&pTree->ci, NM_CLICK, NULL);
		}
	}

	if (!pTree->fFocus)
	{
		SetFocus(hTreeCtrl);
	}

bail:
	;
}

// ----------------------------------------------------------------------------
//
//  Scroll window vertically as needed to make given item fully visible
//  vertically
//
// ----------------------------------------------------------------------------

BOOL NEAR _LibUIDK_TV_ScrollVertIntoView(PTREE pTree, TREEMEMBER *pMember, HWND hTreeCtrl, HTREEITEM hItem)
{
	// This function has crashed in stress before, so we need to assert the incoming parameters.
	ASSERT(hItem);
	ASSERT(pMember && pMember->m_hTop);

	// Do nothing if the parameters are invalid
	if (!hItem || pMember == NULL || pMember->m_hTop == NULL)
	{
		return FALSE;
	}

	// Do nothing if this item is not visible
	if (!ITEM_VISIBLE(hItem))
	{
		return FALSE;
	}

	if (hItem->iShownIndex <= pMember->m_hTop->iShownIndex)
	{
		pMember->m_hTop = hItem;
		pMember->m_nTopItemYOffset = 0;

		// 更新滚动条
		int nPos = _LibUIDK_TV_GetTopItemScrollPos(pMember, pMember->m_hTop, hTreeCtrl);
		SetScrollPos(hTreeCtrl, SB_VERT, nPos, TRUE);
		InvalidateRect(hTreeCtrl, NULL, FALSE);
		return 0;
	}

	if (hItem->iShownIndex > pMember->m_hTop->iShownIndex)
	{
		CRect rcClient;
		GetClientRect(hTreeCtrl, rcClient);

		// 判断这个Item是不是完整的显示在tree中。
		int nCountHeight = 0;
		HTREEITEM hWolk = pMember->m_hTop;
		// hItem是否完全在tree可见区域内
		BOOL bFullInTree = FALSE;
		do
		{
			TV_ITEMDATA *pData = TreeView_GetItemData(hTreeCtrl, pMember, hWolk);

			int nItemHeight = pData->nItemHeight;
			if (nItemHeight == 0)
			{
				nItemHeight = TreeView_GetItemHeight(hTreeCtrl);
			}
			nCountHeight += nItemHeight;

			if (nCountHeight + pMember->m_nTopItemYOffset > rcClient.Height())
			{
				break;
			}

			if (hWolk == hItem)
			{
				bFullInTree = TRUE;
				break;
			}

			hWolk = _LibUIDK_TV_GetNextVisItem(hTreeCtrl, hWolk);
		}
		while (hWolk != NULL);

		if (bFullInTree)
		{
			InvalidateRect(hTreeCtrl, NULL, FALSE);
			return 0;
		}

		// 如果hItem向下滚动时超出tree，则把它滚动条视频范围内，且与tree下对齐
		// 向上遍历
		hWolk = hItem;
		nCountHeight = 0;
		while (hWolk != NULL)
		{
			TV_ITEMDATA *pData = TreeView_GetItemData(hTreeCtrl, pMember, hWolk);

			int nItemHeight = pData->nItemHeight;
			if (nItemHeight == 0)
			{
				nItemHeight = TreeView_GetItemHeight(hTreeCtrl);
			}
			nCountHeight += nItemHeight;

			if (rcClient.Height() - nCountHeight <= 0)
			{
				pMember->m_hTop = hWolk;
				pMember->m_nTopItemYOffset = rcClient.Height() - nCountHeight;

				// 计算垂直滚动条位置
				int nPos = _LibUIDK_TV_GetTopItemScrollPos(pMember, pMember->m_hTop, hTreeCtrl);
				SetScrollPos(hTreeCtrl, SB_VERT, nPos, TRUE);

				break;
			}

			hWolk = TV_GetPrevVisItem(hWolk);
		}

		InvalidateRect(hTreeCtrl, NULL, FALSE);

		return 0;
	}

	return FALSE;
}

LRESULT TreeParentWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC wndprocOld = CWndProcList::GetOldWndProc(hwnd, (WNDPROC)TreeParentWndProc);
	if (wndprocOld == NULL)
	{
		return 0;
	}

	if (uMsg == WM_NOTIFY)
	{
		NMHDR *pnmhdr = (NMHDR *)lParam;

		switch (pnmhdr->code)
		{
		// 双击父Item，或按方向键后展开或折叠父Item，不会触发TVM_EXPAND，只有调用代码Expand才会执行
		// 所以，处理TVN_ITEMEXPANDED消息，代替处理鼠标、键盘或使用代码展开或折叠Item.
		case TVN_ITEMEXPANDED:
		{
			NMTREEVIEW *pnmTree = (NMTREEVIEW *)lParam;
			BOOL bNewExpand = ((pnmTree->itemNew.state & TVIS_EXPANDED) == TVIS_EXPANDED);
			TREEMEMBER *pMember = (TREEMEMBER *)SendMessage(pnmhdr->hwndFrom, TVM_GET_MEMBER, 0, 0);
			UpdateBranchHeightAfterExpand(pnmhdr->hwndFrom, pMember, pnmTree->itemNew.hItem,
				!bNewExpand, bNewExpand);
			_LibUIDK_TV_CalcScrollBars(pnmhdr->hwndFrom, pMember);
		}
		break;

		default:
			break;
		}
	}

	return CallWindowProc(wndprocOld, hwnd, uMsg, wParam, lParam);
}

LRESULT CSkinTreeCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	switch (message)
	{
	case WM_WINDOWPOSCHANGED:
		Ctrl_OnWindowPosChanged(this, wParam, lParam);
		break;

	case WM_CREATE:
	{

		// Hook the parent.
		WNDPROC procOld = (WNDPROC)IUISetWindowLong(GetParent()->GetSafeHwnd(),
				GWLP_WNDPROC, (LONG_PTR)TreeParentWndProc);
		CWndProcList::AddToMap(GetParent()->GetSafeHwnd(), procOld, (WNDPROC)TreeParentWndProc);
	}
	break;

	case WM_GETBACKGROUND:
	{
		LRESULT lr = CTreeCtrl::WindowProc(message, wParam, lParam);	// Give derived class a chance.
		if (lr != 0)	// The derived class has handle.
		{
			return lr;
		}

		BOOL *pbCompatibleBitmap = (BOOL *)lParam;

		if (m_bmpCompatible.GetSafeHandle() == NULL)
		{
			if (pbCompatibleBitmap != NULL)
			{
				*pbCompatibleBitmap = FALSE;
			}

			return NULL;
		}
		else
		{
			if (pbCompatibleBitmap != NULL)
			{
				*pbCompatibleBitmap = TRUE;
			}

			return (LRESULT)m_bmpCompatible.GetSafeHandle();
		}
	}
	}

	PTREE pTree = (PTREE)GetWindowPtr(m_hWnd, 0);

#define TVMP_CALCSCROLLBARS (TV_FIRST + 0x1000)

	LRESULT lr = 0;
	if (pTree != NULL && pMember->m_bSmoothScroll)
	{
		if (message == g_uDragImages)
		{
			lr = CTreeCtrl::WindowProc(message, wParam, lParam);
			return lr;
		}

		switch (message)
		{
		case TVM_GETNEXTITEM:
			if (wParam  == TVGN_FIRSTVISIBLE)
			{
				return (LRESULT)pMember->m_hTop;
			}
			break;

		case TVM_INSERTITEM:
			lr = CTreeCtrl::WindowProc(message, wParam, lParam);
			UpdateBranchHeightAfterAddItem(m_hWnd, pMember, (HTREEITEM)lr);
			_LibUIDK_TV_CalcScrollBars(m_hWnd, pMember);
			return lr;

		case TVM_DELETEITEM:
		{
			if (HTREEITEM(lParam) == pMember->m_hTop)
			{
				pMember->m_hTop = NULL;
				pMember->m_nTopItemYOffset = 0;
			}

			ReleaseItemControl(HTREEITEM(lParam));

			TV_ITEMDATA *pItemData = TreeView_GetItemData(m_hWnd, pMember, HTREEITEM(lParam));

			UpdateBranchHeightAfterDeleteItem(m_hWnd, pMember, HTREEITEM(lParam));
			lr = CTreeCtrl::WindowProc(message, wParam, lParam);
			delete pItemData;

			if (GetRootItem() == NULL)
			{
				pMember->m_cxMax = 0;
			}
			_LibUIDK_TV_CalcScrollBars(m_hWnd, pMember);
		}

		return lr;

		case TVMP_CALCSCROLLBARS:
			_LibUIDK_TV_CalcScrollBars(m_hWnd, pMember);
			return 0;

		case WM_SETREDRAW:
			pMember->m_bRedraw = (BOOL)wParam;

			// 这里不能调用CTreeCtrl::WindowProc(message, wParam, lParam);
			// 因为基类在响应WM_SETREDRAW消息时，会调用TV_ScrollBarsAfterSetWidth
			// 而TV_ScrollBarsAfterSetWidth会把滚动条位置设置成基于行的数值。
			pTree->fRedraw = TRUE && pMember->m_bRedraw;
			if (pTree->fRedraw)
			{
				// This use to only refresh the items from hTop down, this is bad as if items are inserted
				// before the visible point within the tree then we would fail!
				if (pTree->hRoot)
				{
					pTree->cShowing = TV_UpdateShownIndexes(pTree, pTree->hRoot);
				}
			}

			if ((BOOL)wParam)
			{
				_LibUIDK_TV_CalcScrollBars(m_hWnd, pMember);
				InvalidateRect(NULL, FALSE);
			}
			return lr;

		case TVM_SETBORDER:
			lr = CTreeCtrl::WindowProc(message, wParam, lParam);
			_LibUIDK_TV_CalcScrollBars(m_hWnd, pMember);
			return lr;

		case WM_ISREDRAW:
			return (LRESULT)pMember->m_bRedraw;

		case WM_VSCROLL:
		{
			int nOldPos = GetScrollPos(SB_VERT);
			// 仅在拖动滑块时有效, 因为WM_VSCROLL中保存的tackpos只有16位（最大值65535）
			// 所以使用GetScrollInfo得到32位的track pos.
			int nTrackPos = 0;
			UINT uCode = GET_WM_VSCROLL_CODE(wParam, lParam);
			if (uCode == SB_THUMBPOSITION || uCode == SB_THUMBTRACK)
			{
				SCROLLINFO si;
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_TRACKPOS;
				GetScrollInfo(SB_VERT, &si, SIF_TRACKPOS);
				nTrackPos = si.nTrackPos;
			}
			lr = CTreeCtrl::WindowProc(message, wParam, lParam);
			_LibUIDK_TV_VertScroll(m_hWnd, pMember, uCode, nOldPos, nTrackPos);
			return lr;
		}

		case WM_KEYDOWN:
			lr = CTreeCtrl::WindowProc(message, wParam, lParam);
			_LibUIDK_TV_ScrollVertIntoView(pTree, pMember, m_hWnd, TreeView_GetNextItem(m_hWnd, NULL, TVGN_CARET));
			InvalidateRect(NULL);
			return lr;

		case TVM_HITTEST:
			return (LRESULT)_LibUIDK_TV_OnHitTest(m_hWnd, pMember, (LPTV_HITTESTINFO)lParam);

		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
			_LibUIDK_TV_ButtonDown(pTree, m_hWnd, message, (UINT) wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0);
			return 0;

		case TVM_SETIMAGELIST:
			lr = CTreeCtrl::WindowProc(message, wParam, lParam);

			if (int(wParam) == TVSIL_STATE)
			{
				if (HIMAGELIST(lParam) != NULL)
				{
					ImageList_GetIconSize(HIMAGELIST(lParam), &pMember->m_cxState, &pMember->m_cyState);
				}
				else
				{
					pMember->m_cxState = 0;
				}
			}
			else if (int(wParam) == TVSIL_NORMAL)
			{
				int cx = 0;
				int cy = 0;
				if (HIMAGELIST(lParam) != NULL
					&& ImageList_GetIconSize(HIMAGELIST(lParam), &cx, &cy))
				{
					pMember->m_cxImage = (cx + MAGIC_INDENT);
					pMember->m_cyImage = (SHORT)cy;
				}
				else
				{
					pMember->m_cxImage = pMember->m_cyImage = 0;
				}
			}

			return lr;

		case TVM_SELECTITEM:
			SetRedraw(FALSE);
			lr = CTreeCtrl::WindowProc(message, wParam, lParam);
			SetRedraw(TRUE);
			return lr;

		case WM_MOUSEWHEEL:
			if (pMember->m_bSmoothScroll)
			{
				LONG_PTR lStyle = IUIGetWindowLong(m_hWnd, GWL_STYLE);
				BOOL bScroll = (!IsIncludeFlag(lStyle, TVS_NOSCROLL));

				if (bScroll)
				{
					int nOldPos = GetScrollPos(SB_VERT);
					short zDelta = HIWORD(wParam);
					if (zDelta > 0)
					{
						_LibUIDK_TV_VertScroll(m_hWnd, pMember, SB_THUMBTRACK, nOldPos, max(0, nOldPos - GetVScrollMouseWhellStep()));
					}
					else
					{
						_LibUIDK_TV_VertScroll(m_hWnd, pMember, SB_THUMBTRACK, nOldPos, nOldPos + GetVScrollMouseWhellStep());
					}
				}
			}
			break;

		// 支持平时隐藏滚动条，鼠标Hover时，显示滚动条
		case WM_MOUSEMOVE:
			if (pMember->m_bHoverScrollBarMode)
			{
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = m_hWnd;
				::TrackMouseEvent(&tme);

				LONG_PTR lStyle = IUIGetWindowLong(m_hWnd, GWL_STYLE);
				if (IsIncludeFlag(lStyle, TVS_NOSCROLL))
				{
					lStyle &= ~TVS_NOSCROLL;
					IUISetWindowLong(m_hWnd, GWL_STYLE, lStyle);

					int nOldPos = pMember->m_nTopItemYScrollPos;
					::SendMessage(m_hWnd, TVMP_CALCSCROLLBARS, 0, 0);
					::SetScrollPos(m_hWnd, SB_VERT, nOldPos, TRUE);
					pMember->m_nTopItemYScrollPos = nOldPos;
				}
			}
			break;

		// 鼠标离开联系人列表后，隐藏滚动条
		// 鼠标从Tree的滚动条移动到Tree的客户区后，也会收到WM_NCMOUSELEAVE
		case WM_NCMOUSELEAVE:
			if (pMember->m_bHoverScrollBarMode)
			{
				LONG_PTR lStyle = IUIGetWindowLong(m_hWnd, GWL_STYLE);
				if (!IsIncludeFlag(lStyle, TVS_NOSCROLL))
				{
					POINT pt;
					GetCursorPos(&pt);

					CRect rcTree;
					::GetWindowRect(m_hWnd, rcTree);

					if (!rcTree.PtInRect(pt))
					{
						lStyle |= TVS_NOSCROLL;
						IUISetWindowLong(m_hWnd, GWL_STYLE, lStyle);

						::SetScrollRange(m_hWnd, SB_VERT, 0, 0, TRUE);
					}
				}
			}
			break;

		// 鼠标离开联系人列表后，隐藏滚动条
		// 鼠标移动到Tree的滚动条上后，也会收到WM_MOUSELEAVE
		case WM_MOUSELEAVE:
			if (pMember->m_bHoverScrollBarMode)
			{
				LONG_PTR lStyle = IUIGetWindowLong(m_hWnd, GWL_STYLE);
				if (!IsIncludeFlag(lStyle, TVS_NOSCROLL))
				{
					POINT pt;
					GetCursorPos(&pt);

					CRect rcTree;
					::GetWindowRect(m_hWnd, rcTree);

					if (!rcTree.PtInRect(pt))
					{
						lStyle |= TVS_NOSCROLL;
						IUISetWindowLong(m_hWnd, GWL_STYLE, lStyle);

						::SetScrollRange(m_hWnd, SB_VERT, 0, 0, TRUE);
					}
				}
			}
			break;


		// 自定义消息
		case TVM_GET_TOTAL_SHOW_HEIGHT:
			return (LRESULT)pMember->m_nTotalShowHeight;

		case TVM_SET_TOTAL_SHOW_HEIGHT:
			if ((BOOL)wParam)
			{
				pMember->m_nTotalShowHeight += (int)lParam;
			}
			else
			{
				pMember->m_nTotalShowHeight = (int)lParam;
			}
			break;

		case TVM_SET_TOP_ITEM:
			pMember->m_hTop = (HTREEITEM)lParam;
			return 0;

		case TVM_GET_TOP_ITEM:
			return (LRESULT)pMember->m_hTop;

		case TVM_SET_TOP_ITEM_YOFFSET:
			pMember->m_nTopItemYOffset = (int)lParam;
			return 0;

		case TVM_GET_TOP_ITEM_YOFFSET:
			return (LRESULT)pMember->m_nTopItemYOffset;

		case TVM_GET_STATE_IMAGE_SIZE:
			return (LRESULT)MAKEWORD(pMember->m_cxState, pMember->m_cyState);

		case TVM_GET_NORMAL_IMAGE_SIZE:
			return (LRESULT)MAKEWORD(pMember->m_cxImage, pMember->m_cyImage);

		case TVM_SET_CXMAX:
			pMember->m_cxMax = (WORD)wParam;
			return 0;

		case TVM_GET_CXMAX:
			return pMember->m_cxMax;

		case TVM_SET_CAPTURE_POINT:
			pMember->m_ptCapture.x = GET_X_LPARAM(lParam);
			pMember->m_ptCapture.y = GET_Y_LPARAM(lParam);
			return 0;

		case TVM_GET_CAPTURE_POINT:
			return (LRESULT)MAKELPARAM(pMember->m_ptCapture.x, pMember->m_ptCapture.y);

		case TVM_SET_TOPITEM_YSCROLLPOS:
			pMember->m_nTopItemYScrollPos = (int)lParam;
			return 0;

		case TVM_GET_TOPITEM_YSCROLLPOS:
			return pMember->m_nTopItemYScrollPos;

		case WLNM_NOTCREATEASCHILD:
			return pMember->m_bWindowlessNotCreateAsChild;
		}
	}

	// the CTreeCtrl can't response SB_THUMBTRACK and SB_THUMBPOSITION message send by custom in Vista,
	// but can response in XP.
	// So, when CTreeCtrl receive these two message, use CTreeCtrl::SetScrollInfo to scroll content.
	// if the XP, SetScrollInfo call cause flash.
	if ((message == WM_VSCROLL || message == WM_HSCROLL) && pMember->m_bUseCustomScrollBar)
	{
		WORD sbCode = LOWORD(wParam);
		if ((sbCode == SB_THUMBTRACK || sbCode == SB_THUMBPOSITION)
			&& IUIIsWindowsVistaOrGreater()/*Vista or later*/)
		{
			int nPos = HIWORD(wParam);

			if (message == WM_VSCROLL)
			{
				SCROLLINFO siv = {0};
				siv.cbSize = sizeof(SCROLLINFO);
				siv.fMask = SIF_ALL;
				GetScrollInfo(SB_VERT, &siv);
				siv.nPos = nPos;
				SetRedraw(FALSE);
				SetScrollInfo(SB_VERT, &siv);
				SetRedraw(TRUE);
				ReDrawControl(FALSE, TRUE, NULL);
				UpdateWindow();
			}
			else if (message == WM_HSCROLL)
			{
				SCROLLINFO sih = {0};
				sih.cbSize = sizeof(SCROLLINFO);
				sih.fMask = SIF_ALL;
				GetScrollInfo(SB_HORZ, &sih);
				sih.nPos = nPos;
				SetScrollInfo(SB_HORZ, &sih);
			}
		}

		// If in XP, call Invalidate must before WindowProc to avoid flash
		if (IUIIsWindowsVersion(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 0, VER_LESS))
		{
			ReDrawControl(FALSE, TRUE, NULL);
		}
		LRESULT lr = CTreeCtrl::WindowProc(message, wParam, lParam);
		UpdateCustomScrollInfo(this);
		// if in vista or later, call Invalidate must after WindowProc
		if (IUIIsWindowsVistaOrGreater())
		{
			ReDrawControl(FALSE, TRUE, NULL);
		}
		return lr;
	}

	switch (message)
	{
	case WM_HSCROLL:
	case WM_MOUSEWHEEL:
		ReDrawControl(FALSE, TRUE, NULL);
		break;
	// If the bottom item is part visible, click it will start a timer with
	// id 43(IDT_SCROLLWAIT), in the timer, scroll the view to show the bottom item full.
	case WM_TIMER:
		if (wParam == IDT_SCROLLWAIT)
		{
			ReDrawControl(FALSE, TRUE, NULL);
		}
		break;
	default:
		break;
	}

	if (message == WM_ERASEBKGND)
	{
		return TRUE;
	}

	if (message == WM_NCPAINT)
	{
		// Draw the resize scroll bar
		if (pMember->m_wndHScroll.GetSafeHwnd() != NULL
			&& pMember->m_wndHScroll.IsWindowVisible()
			&& pMember->m_wndVScroll.GetSafeHwnd() != NULL
			&& pMember->m_wndVScroll.IsWindowVisible())
		{
			int nVScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
			int nHScrollBarHeight = GetSystemMetrics(SM_CYHSCROLL);

			CWindowDC dcWin(this);

			CRect rcSizeBox2This;
			GetWindowRect(rcSizeBox2This);
			ScreenToClient(rcSizeBox2This);
			rcSizeBox2This.left = rcSizeBox2This.right - nVScrollBarWidth;
			rcSizeBox2This.top = rcSizeBox2This.bottom - nHScrollBarHeight;

			// Draw parent part
			DrawParentNonClientPart(this, &dcWin, rcSizeBox2This, NULL, NULL);

			// Draw myself
			IUIBitBlt(dcWin.GetSafeHdc(), rcSizeBox2This, pMember->m_hLevel1ResizedBkImage,
				rcSizeBox2This.left, rcSizeBox2This.top, SRCCOPY);

			return 0;
		}

		LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		BOOL bBorder = ((lStyle & WS_BORDER) == WS_BORDER);
		if (!bBorder)
		{
			return CTreeCtrl::WindowProc(message, wParam, lParam);
		}

		// Draw the border and scroll bar
		DefWindowProc(message, wParam, lParam);

		if (pMember->m_bCacheMode)
		{
			IUIDrawNCBackground(this,
				pMember->m_hLevel1ResizedBkImage, FALSE, NULL, CRect(1, 1, 1, 1));
		}
		else
		{
			IUIDrawNCBackground(this, pMember->m_himgBk[0],
				m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				CRect(1, 1, 1, 1));
		}

		// returns zero if it processes this message
		return 0;
	}

	if (message == WM_PAINT)
	{
		CPaintDC dc(this); // device context for painting

		// TODO: Add your message handler code here
		if (pMember->m_bCacheMode)
		{
			// First to show
			if (!pMember->m_bTreeFirstShowed)
			{
				pMember->m_bTreeFirstShowed = TRUE;

				CRect rcWin;
				GetWindowRect(rcWin);

				_LibUIDK_CSkinTreeCtrl_UpdateLevel1(this, pMember->m_bTreeFirstShowed,
					pMember->m_himgBk[0], pMember->m_himgBk[1],
					m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
					rcWin.Width(), rcWin.Height(),
					&pMember->m_hLevel1ResizedBkImage);
				_LibUIDK_CSkinTreeCtrl_UpdateLevel2(this, pMember->m_bTreeFirstShowed,
					pMember->m_hLevel2BlendBkTextImage);
				_LibUIDK_CSkinTreeCtrl_UpdateLevel3(this, pMember->m_bTreeFirstShowed,
					pMember->m_hLevel2BlendBkTextImage, pMember->m_hLevel3Background,
					NULL, NULL);
			}

			CRect rcClient;
			GetClientRect(&rcClient);

			BitBltG(dc.GetSafeHdc(), 0, 0, rcClient.Width(), rcClient.Height(),
				pMember->m_bEnableHighlightItemEffect ? pMember->m_hLevel3Background : pMember->m_hLevel2BlendBkTextImage,
				0, 0, SRCCOPY);
		}
		else
		{
			CRect rcClient;
			GetClientRect(&rcClient);

			CDC dcMem;
			dcMem.CreateCompatibleDC(&dc);

			if (m_bmpCompatible.GetSafeHandle() != NULL)
			{
				m_bmpCompatible.DeleteObject();
			}
			m_bmpCompatible.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

			ASSERT(m_bmpCompatible.GetSafeHandle() != NULL);
			CBitmap *pbmpMemOld = dcMem.SelectObject(&m_bmpCompatible);

			// draw background
			OnDrawBackground(&dcMem);

			// draw item(text)
			// Item绑定的windowless控件的绘制，与普通窗口内的windowless控件绘制机制不同，
			// 对于普通窗口，只需要遍历所有windowless控件，把窗口内且可见的控件显示出来就行。
			// 对于item绑定的windowless控件，如果Item数据量庞大，采用普通窗口的形式，性能太差。
			// 所以，只需要从top item开始，绘制窗口内的item绑定的windowless即可。
			// 由于我们也仅布局tree内item绑定的windowless，所以，tree外Item绑定的windowless的坐标
			// 也都是不正确的，故不能使用标准方式，对windowless子控件进行hittest（Item由可见
			// 滚动到不可见前，Item绑定的windowless控件的坐标，是留在可见区域内的）。

			COLORREF cr = GetTextColor();
			dcMem.SetTextColor(cr);
			HTREEITEM hTI = GetFirstVisibleItem();
			TRACE(_T("========top %x, yOffset=%d, text=%s\n"), hTI, pMember->m_nTopItemYOffset, GetItemText(hTI));
			int nDrawHeight = 0;// 已绘制完的item的总高度
			int i = 0;
			CRect rcItem;
			rcItem.right = rcClient.right;
			while (hTI != NULL)
			{
				TV_ITEMDATA *pData = TreeView_GetItemData(m_hWnd, pMember, hTI);
				int nItemHeight = pData->nItemHeight;
				if (nItemHeight == 0)
				{
					nItemHeight = GetItemHeight();
				}

				// 第一次计算时，算上垂直偏移
				if (nDrawHeight == 0)
				{
					nDrawHeight += pMember->m_nTopItemYOffset;
				}

				rcItem.top = nDrawHeight;
				rcItem.bottom = rcItem.top + nItemHeight;

				// 超出控件，退出绘制
				if (nDrawHeight > rcClient.Height())
				{
					// After the last item animation finish, kill the timer.
					KillTimer(IDTT_ANIMATION_CONTENT);
					pMember->m_bAnimationMode = false;
					pMember->m_nCurTimerTick = 0;
					pMember->m_vAnimationData.clear();

					NMHDR nmhdr;
					nmhdr.hwndFrom = m_hWnd;
					nmhdr.idFrom = GetDlgCtrlID();
					nmhdr.code = TVN_ANIMATION_END;
					GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmhdr));

					break;
				}

				nDrawHeight += nItemHeight;

				if (pMember->m_bAnimationMode)
				{
					// Each item's left margin is different.
					int nFrameIndex = 0;
					if (pMember->m_bTogetherAnimation)
					{
						nFrameIndex = pMember->m_nCurTimerTick;
					}
					else
					{
						if (pMember->m_nCurTimerTick - i >= 0)
						{
							nFrameIndex = pMember->m_nCurTimerTick - i;
						}
						else
						{
							nFrameIndex = 0;
						}
						if (nFrameIndex >= (int)pMember->m_vAnimationData.size())
						{
							nFrameIndex = (int)pMember->m_vAnimationData.size() - 1;
						}
					}

					OnDrawItem(&dcMem, hTI, rcItem, pMember->m_vAnimationData[nFrameIndex]);
				}
				else if (pMember->m_bAnimateRemoveMode)
				{
					OnDrawItem(&dcMem, hTI, rcItem, 0);
				}
				else
				{
					OnDrawItem(&dcMem, hTI, rcItem, 0);
				}

				hTI = GetNextVisibleItem(hTI);
				i++;
			}

			// Draw windowless children control.
			// Tree内不仅有Item绑定的windowless控件，还有用户创建的其它windowless控件，
			// Item绑定的windowless控件，由上面的while循环绘制，用户创建的，由DrawWLChildren
			// 绘制，所以要区分开这两种windowless控件。

			DrawWLChildren(&dcMem);

			// Draw transparent rich edit for refuse flicker.
			HWND hRichEdit = ::FindWindowEx(m_hWnd, NULL, GetRichEditClassName(), NULL);
			while (hRichEdit != NULL)
			{
				::SendMessage(hRichEdit, WM_WL_CALLONDRAW, (WPARAM)&dcMem, 0);

				hRichEdit = ::FindWindowEx(m_hWnd, hRichEdit, GetRichEditClassName(), NULL);
			}

			dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);
			dcMem.SelectObject(pbmpMemOld);

			UpdateCustomScrollBar();
		}
	}

	if (TVM_DRAW_BACKGROUND == message)
	{
		CDC *pDC = (CDC *)wParam;

		OnDrawBackground(pDC);
	}
	else if (TVM_DRAW_ITEM == message)
	{
		CDC *pDC = (CDC *)wParam;
		HTREEITEM hItem = (HTREEITEM)lParam;

		CRect rcItem;
		GetItemRect(hItem, rcItem, FALSE);
		OnDrawItem(pDC, hItem, rcItem);
	}

	// replace item data
	if (TVM_GETITEM == message
		&& (!pMember->m_bSetItemDataInternal || pMember->m_bInsertingItemSetData))
	{
		TVITEM *ptvi = (TVITEM *)lParam;
		if ((ptvi->mask & TVIF_PARAM) == TVIF_PARAM)
		{
			BOOL bRet = (BOOL)CTreeCtrl::WindowProc(message, wParam, lParam);
			if (bRet)
			{
				TV_ITEMDATA *pData = (TV_ITEMDATA *)ptvi->lParam;
				ptvi->lParam = (pData == NULL ? 0 : pData->lParam);
			}
			return bRet;
		}
	}
	// replace item data
	if (TVM_SETITEM == message && !pMember->m_bSetItemDataInternal)
	{
		TVITEM *ptvi = (TVITEM *)lParam;
		if ((ptvi->mask & TVIF_PARAM) == TVIF_PARAM)
		{
			pMember->m_bSetItemDataInternal = TRUE;
			TV_ITEMDATA *pData = (TV_ITEMDATA *)GetItemData(ptvi->hItem);
			pMember->m_bSetItemDataInternal = FALSE;

			if (pData == NULL)
			{
				pData = new TV_ITEMDATA;
				pMember->m_bSetItemDataInternal = TRUE;
				SetItemData(ptvi->hItem, (DWORD_PTR)pData);
				pMember->m_bSetItemDataInternal = FALSE;
			}

			pData->lParam = ptvi->lParam;
			ptvi->lParam = (LPARAM)pData;

			return CTreeCtrl::WindowProc(message, wParam, (LPARAM)ptvi);
		}
	}

	if (TVM_GETITEMDATAINTERNAL == message)
	{
		pMember->m_bSetItemDataInternal = TRUE;
		TV_ITEMDATA *pData = (TV_ITEMDATA *)GetItemData(HTREEITEM(wParam));
		pMember->m_bSetItemDataInternal = FALSE;

		return (LRESULT)pData;
	}
	else if (TVM_SETITEMDATAINTERNAL == message)
	{
		pMember->m_bSetItemDataInternal = TRUE;
		SetItemData(HTREEITEM(wParam), lParam);
		pMember->m_bSetItemDataInternal = FALSE;
	}

	if (message == WM_SHOWWINDOW && pMember->m_bUseCustomScrollBar)
	{
		BOOL bShow = BOOL(wParam);
		if (bShow)
		{
			UpdateCustomScrollBar();
		}
		else
		{
			pMember->m_wndHScroll.ShowWindow(SW_HIDE);
			pMember->m_wndVScroll.ShowWindow(SW_HIDE);
		}
	}

	if (message == WM_MOUSEWHEEL && pMember->m_bUseCustomScrollBar)
	{
		LRESULT lr = CTreeCtrl::WindowProc(message, wParam, lParam);
		UpdateCustomScrollInfo(this);
		return lr;
	}

	// update scroll bar.
	// TVM_INSERTITEM, TVM_DELETEITEM and WM_GETDLGCODE/*arrow key down*/
	// can trigger WM_PAINT message, so handle in WM_PAINT message function.
	if (pMember->m_bUseCustomScrollBar && (message == WM_SIZE || message == WM_MOVE))
	{
		LRESULT hr = CTreeCtrl::WindowProc(message, wParam, lParam);
		UpdateCustomScrollBar();
		return hr;
	}

	if (message == WM_SIZE && pMember->m_bCacheMode)
	{
		WPARAM nType = wParam;
		CSize size((DWORD)lParam);

		if (pMember->m_bTreeFirstShowed)
		{
			ReDrawControl(TRUE, TRUE);
		}
	}

	if (WM_LBUTTONDBLCLK == message)
	{
		NMHDR nmhdr;
		nmhdr.hwndFrom = m_hWnd;
		nmhdr.idFrom = GetDlgCtrlID();
		nmhdr.code = NM_DBLCLKEX;
		int nRet = (int)GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmhdr));
		if (nRet != 0)
		{
			// If pop-up another window in NM_DBLCLKEX notify,
			// Call CTreeCtrl::WindowProc can cause the new window can't be get the window focus.
			return 0;
		}
	}

	if (message == WM_MOUSEMOVE)
	{
		if (IsHighlightItemEffectEnabled())
		{
			CPoint point((DWORD)lParam);

			HTREEITEM hCurItem = this->HitTest(point);
			if (hCurItem != pMember->m_hCurHighlightItem)
			{
				// send notify to tree's parent
				HTREEITEM hOldHighlightItem = pMember->m_hCurHighlightItem;
				NMHDR nmhdr;
				nmhdr.hwndFrom = m_hWnd;
				nmhdr.idFrom = GetDlgCtrlID();
				nmhdr.code = TVN_HIGHLIGHTCHANGING;
				NMTREEVIEW tvhdr;
				tvhdr.hdr = nmhdr;
				tvhdr.itemOld.hItem = hOldHighlightItem;
				tvhdr.itemNew.hItem = hCurItem;
				BOOL bRefuse = (BOOL)GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&tvhdr));
				HTREEITEM hOldHighlightIndex = pMember->m_hCurHighlightItem;
				if (!bRefuse)
				{
					pMember->m_hCurHighlightItem = hCurItem;

					// send notify to tree's parent
					NMHDR nmhdr;
					nmhdr.hwndFrom = m_hWnd;
					nmhdr.idFrom = GetDlgCtrlID();
					nmhdr.code = TVN_HIGHLIGHTCHANGED;
					NMTREEVIEW tvhdr;
					tvhdr.hdr = nmhdr;
					tvhdr.itemOld.hItem = hOldHighlightItem;
					tvhdr.itemNew.hItem = hCurItem;
					GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&tvhdr));
				}
				if (pMember->m_bCacheMode)
				{
					// Refresh the new and old highlight item
					_LibUIDK_CSkinTreeCtrl_UpdateLevel3(this,
						pMember->m_bTreeFirstShowed,
						pMember->m_hLevel2BlendBkTextImage,
						pMember->m_hLevel3Background,
						pMember->m_hCurHighlightItem,
						hOldHighlightIndex);

					// If add UpdateWindow call after the follow code,
					// and call the these code 1000 times,
					// the performance excelled call Invalidate();
					CRect rcOld;
					GetItemRect(hOldHighlightIndex, rcOld, FALSE);
					CRect rcNew;
					GetItemRect(pMember->m_hCurHighlightItem, rcNew, FALSE);
					InvalidateRect(rcOld, FALSE);
					InvalidateRect(rcNew, FALSE);
				}
				else
				{
					Invalidate();
				}
			}
		}

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = pMember->m_dwHoverTime;
		::TrackMouseEvent(&tme);
	}

	if (message == WM_MOUSEHOVER)
	{
		if (pMember->m_bHoverRepeat)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_HOVER;
			tme.hwndTrack = m_hWnd;
			tme.dwHoverTime = pMember->m_dwHoverTime;
			::TrackMouseEvent(&tme);
		}
	}

	if (message == WM_MOUSELEAVE)
	{
		if (pMember->m_bEnableHighlightItemEffect)
		{
			pMember->m_hCurHighlightItem = NULL;
			ReDrawControl(FALSE, FALSE);
		}
	}

	if (message == WM_ENABLE && pMember->m_bUseCustomScrollBar)
	{
		if (pMember->m_wndHScroll.GetSafeHwnd() != NULL)
		{
			pMember->m_wndHScroll.EnableWindow((BOOL)wParam);
		}
		if (pMember->m_wndVScroll.GetSafeHwnd() != NULL)
		{
			pMember->m_wndVScroll.EnableWindow((BOOL)wParam);
		}
	}

	if (message == WM_KILLFOCUS || message == WM_SETFOCUS)
	{
		Invalidate();
	}

	if (message == WM_DESTROY)
	{
		// Release memory
		_ReleaseItemData(this, pMember);
	}

	if (message == WM_NCDESTROY)
	{
		pMember->m_bUseCustomScrollBar = FALSE;

		if (pMember->m_wndHScroll.GetSafeHwnd() != NULL)
		{
			pMember->m_wndHScroll.DestroyWindow();
		}
		if (pMember->m_wndVScroll.GetSafeHwnd() != NULL)
		{
			pMember->m_wndVScroll.DestroyWindow();
		}
	}

	if (message == WM_TIMER)
	{
		if (wParam == IDTT_ANIMATION_CONTENT)
		{
			Invalidate();

			pMember->m_nCurTimerTick++;

			return 0;	// The List control default handle may Kill the timer.
		}

		else if (wParam == IDTT_INSERT_ITEM)
		{
			CRect rcItem;
			GetItemRect(pMember->m_hInsertItem, rcItem, FALSE);

			if (pMember->m_nCurInsertItemTimerTick >= (int)pMember->m_vInsertItemAnimationData.size())
			{
				KillTimer(IDTT_INSERT_ITEM);

				pMember->m_bAnimationInsertItem = false;
				pMember->m_hInsertItem = NULL;
				pMember->m_nCurInsertItemTimerTick = 0;
				pMember->m_vInsertItemAnimationData.clear();

				NMHDR nmhdr;
				nmhdr.hwndFrom = m_hWnd;
				nmhdr.idFrom = GetDlgCtrlID();
				nmhdr.code = TVN_ANIMATION_INSERT_ITEM_END;
				GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmhdr));

				InvalidateRect(rcItem);

				return 0;
			}

			InvalidateRect(rcItem);

			pMember->m_nCurInsertItemTimerTick++;

			return 0;
		}

		else if (wParam == IDTT_DELETE_ITEM)
		{
			if (pMember->m_nCurDeleteItemTimerTick >= (int)pMember->m_vDeletedItemAnimationData.size())
			{
				KillTimer(IDTT_DELETE_ITEM);
				DeleteItem(pMember->m_hDeletedItem);

				pMember->m_hDeletedItem = NULL;
				pMember->m_nCurDeleteItemTimerTick = 0;
				pMember->m_vDeletedItemAnimationData.clear();

				return 0;
			}

			SetItemHeightEx(pMember->m_hDeletedItem, pMember->m_vDeletedItemAnimationData[pMember->m_nCurDeleteItemTimerTick]);

			pMember->m_nCurDeleteItemTimerTick++;

			return 0;
		}

		else if (wParam == IDTT_REMOVE_ITEM)
		{
			CRect rcItem;
			GetItemRect(pMember->m_hRemoveItem, rcItem, FALSE);
			InvalidateRect(rcItem);

			if (pMember->m_nCurRemoveItemTimerTick == 0)
			{
				UpdateWindow();
			}

			pMember->m_nCurRemoveItemTimerTick++;

			if (pMember->m_nCurRemoveItemTimerTick >= (int)pMember->m_vRemoveItemAnimationData.size())
			{
				KillTimer(IDTT_REMOVE_ITEM);

				pMember->m_bAnimateRemoveMode = false;
				pMember->m_nCurRemoveItemTimerTick = 0;
				pMember->m_vRemoveItemAnimationData.clear();
				pMember->m_bmpCachedRemovedItem.DeleteObject();

				NMHDR nmhdr;
				nmhdr.hwndFrom = m_hWnd;
				nmhdr.idFrom = GetDlgCtrlID();
				nmhdr.code = TVN_ANIMATION_REMOVE_ITEM_END;
				NMTREEVIEW tv;
				tv.hdr = nmhdr;
				tv.itemNew.hItem = pMember->m_hRemoveItem;
				GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&tv));

				return 0;
			}
		}

		else if (wParam == IDTT_VER_SCROLL)
		{
			ScrollWindow(0, -pMember->m_vVerScrollAnimationData[pMember->m_nCurVerScrollTimerTick]);

			pMember->m_nCurVerScrollTimerTick++;
			if (pMember->m_nCurVerScrollTimerTick >= (int)pMember->m_vVerScrollAnimationData.size())
			{
				KillTimer(IDTT_VER_SCROLL);
				SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), (LPARAM)0);

				pMember->m_nCurVerScrollTimerTick = 0;
				pMember->m_vVerScrollAnimationData.clear();

				return 0;
			}

			return 0;
		}
	}

	if (message == TVM_GET_MEMBER)
	{
		return (LRESULT)pMember;
	}

	// Handle drop message.
	if (message >= DROPM_DRAGENTER
		&& message <= DROPM_DRAGSCROLL)
	{
		NMHDR nmhdr;
		nmhdr.hwndFrom = m_hWnd;
		nmhdr.idFrom = GetDlgCtrlID();
		nmhdr.code = NM_DROP;
		NMDROP drophdr;
		drophdr.hdr = nmhdr;
		drophdr.message = message;
		drophdr.lParam = wParam;
		LRESULT lr = GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&drophdr));
		return lr;
	}

	return CTreeCtrl::WindowProc(message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
// Public member functions

// Draw all to pMember->m_hLevel3Background to cache
int CSkinTreeCtrl::ReDrawControl(BOOL bErase, BOOL bUpdateScrollBar, LPCRECT lpInvalidateRect/* = NULL*/)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (pMember->m_bCacheMode)
	{
		if (!pMember->m_bTreeFirstShowed)
		{
			return -1;
		}

		// Update level-1 cache
		if (bErase)
		{
			CRect rcWin;
			GetWindowRect(rcWin);
			_LibUIDK_CSkinTreeCtrl_UpdateLevel1(this, pMember->m_bTreeFirstShowed,
				pMember->m_himgBk[0], pMember->m_himgBk[1],
				m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				rcWin.Width(), rcWin.Height(),
				&pMember->m_hLevel1ResizedBkImage);
		}

		if (pMember->m_bEnableHighlightItemEffect)
		{
			_LibUIDK_CSkinTreeCtrl_UpdateLevel2(this, pMember->m_bTreeFirstShowed,
				pMember->m_hLevel2BlendBkTextImage);
			_LibUIDK_CSkinTreeCtrl_UpdateLevel3(this, pMember->m_bTreeFirstShowed,
				pMember->m_hLevel2BlendBkTextImage, pMember->m_hLevel3Background,
				NULL, NULL);
		}
		else
		{
			_LibUIDK_CSkinTreeCtrl_UpdateLevel2(this, pMember->m_bTreeFirstShowed,
				pMember->m_hLevel2BlendBkTextImage);
		}

		if (bUpdateScrollBar)
		{
			UpdateCustomScrollBar();
		}

		InvalidateRect(lpInvalidateRect, FALSE);
	}
	else
	{
		Invalidate();
	}

	return 0;
}

int UpdateImageList(CImageList *pImgList, CSkinTreeCtrl *pTreeCtrl, HBITMAP &hImageListCache, __out LPSIZE lpSize)
{
	int nCount = pImgList->GetImageCount();
	if (nCount <= 0)
	{
		return 2;
	}

	IMAGEINFO ii;
	BOOL bRet = pImgList->GetImageInfo(0, &ii);

	lpSize->cx = ii.rcImage.right;
	lpSize->cy = ii.rcImage.bottom;

	CClientDC dc(pTreeCtrl);

	CRect rcClient;
	pTreeCtrl->GetClientRect(rcClient);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	SafeDeleteHBITMAP(hImageListCache);
	hImageListCache = ::CreateCompatibleBitmap(dc.GetSafeHdc(), lpSize->cx * nCount, lpSize->cy);

	HBITMAP hbmpOldMem = (HBITMAP)::SelectObject(dcMem.GetSafeHdc(), hImageListCache);

	for (int i = 0; i < nCount; ++i)
	{
		pImgList->Draw(&dcMem, i, CPoint(lpSize->cx * i, 0), ILD_NORMAL);
	}

	::SelectObject(dcMem.GetSafeHdc(), hbmpOldMem);

	return 0;
}

int CSkinTreeCtrl::UpdateImageListCache()
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	CImageList *pImgList = GetImageList(LVSIL_SMALL);
	if (pImgList == NULL)
	{
		return 1;
	}

	return UpdateImageList(pImgList, this, pMember->m_hLevel4ImageList, &pMember->m_sizeImageInImageList);
}

//////////////////////////////////////////////////////////////////////////

int CSkinTreeCtrl::SmoothScroll(BOOL bSmooth)
{
	ASSERT(!::IsWindow(m_hWnd));

	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_bSmoothScroll = bSmooth;

	return 0;
}

BOOL CSkinTreeCtrl::IsSmoothScroll() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_bSmoothScroll;
}

int CSkinTreeCtrl::SetVScrollMouseWhellStep(int nStep)
{
	if (nStep <= 0)
	{
		return -1;
	}

	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_nVScrollMouseWhellStep = nStep;

	return 0;
}

int CSkinTreeCtrl::GetVScrollMouseWhellStep() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_nVScrollMouseWhellStep;
}

// Set the images for background
int CSkinTreeCtrl::SetImages(UINT uMask, LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameD)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	m_bBkCombine = false;
	IUISetControlImage2(CONTROL_STATE_UNCHECKED_ND, pMember->m_himgBk, lpszImageNameN, lpszImageNameD);

	return 0;
}

// Get the images for background
int CSkinTreeCtrl::GetImages(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameD)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bBkCombine;
	IUIGetControlImage2(uMask, m_himgCombineBk, pMember->m_himgBk, m_bBkCombine, pstrCombineImageName,
		pstrImageNameN, pstrImageNameD);

	return 0;
}

int CSkinTreeCtrl::SetTextAlignHor(
	TEXT_ALIGN_HOR eHorAlignModeN,
	TEXT_ALIGN_HOR eHorAlignModeH,
	TEXT_ALIGN_HOR eHorAlignModeS,
	TEXT_ALIGN_HOR eHorAlignModeD,
	BOOL bRedraw/* = TRUE*/)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_eTextHorAlignMode[0] = eHorAlignModeN;
	pMember->m_eTextHorAlignMode[1] = eHorAlignModeH;
	pMember->m_eTextHorAlignMode[2] = eHorAlignModeS;
	pMember->m_eTextHorAlignMode[3] = eHorAlignModeD;

	if (bRedraw)
	{
		Redraw(this);
	}

	return 0;
}

int CSkinTreeCtrl::GetTextAlignHor(
	TEXT_ALIGN_HOR *peHorAlignModeN,
	TEXT_ALIGN_HOR *peHorAlignModeH,
	TEXT_ALIGN_HOR *peHorAlignModeS,
	TEXT_ALIGN_HOR *peHorAlignModeD)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (peHorAlignModeN != NULL)
	{
		*peHorAlignModeN = pMember->m_eTextHorAlignMode[0];
	}
	if (peHorAlignModeH != NULL)
	{
		*peHorAlignModeH = pMember->m_eTextHorAlignMode[1];
	}
	if (peHorAlignModeS != NULL)
	{
		*peHorAlignModeS = pMember->m_eTextHorAlignMode[2];
	}
	if (peHorAlignModeD != NULL)
	{
		*peHorAlignModeD = pMember->m_eTextHorAlignMode[3];
	}

	return 0;
}

int CSkinTreeCtrl::SetTextAlignVer(
	TEXT_ALIGN_VER eVerAlignModeN,
	TEXT_ALIGN_VER eVerAlignModeH,
	TEXT_ALIGN_VER eVerAlignModeS,
	TEXT_ALIGN_VER eVerAlignModeD,
	BOOL bRedraw/* = TRUE*/)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_eTextVerAlignMode[0] = eVerAlignModeN;
	pMember->m_eTextVerAlignMode[1] = eVerAlignModeH;
	pMember->m_eTextVerAlignMode[2] = eVerAlignModeS;
	pMember->m_eTextVerAlignMode[3] = eVerAlignModeD;

	if (bRedraw)
	{
		Redraw(this);
	}

	return 0;
}

int CSkinTreeCtrl::GetTextAlignVer(
	TEXT_ALIGN_VER *peVerAlignModeN,
	TEXT_ALIGN_VER *peVerAlignModeH,
	TEXT_ALIGN_VER *peVerAlignModeS,
	TEXT_ALIGN_VER *peVerAlignModeD)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (peVerAlignModeN != NULL)
	{
		*peVerAlignModeN = pMember->m_eTextVerAlignMode[0];
	}
	if (peVerAlignModeH != NULL)
	{
		*peVerAlignModeH = pMember->m_eTextVerAlignMode[1];
	}
	if (peVerAlignModeS != NULL)
	{
		*peVerAlignModeS = pMember->m_eTextVerAlignMode[2];
	}
	if (peVerAlignModeD != NULL)
	{
		*peVerAlignModeD = pMember->m_eTextVerAlignMode[3];
	}

	return 0;
}

int CSkinTreeCtrl::SetTextMargin(
	LPCRECT lprcTextMarginN,
	LPCRECT lprcTextMarginH,
	LPCRECT lprcTextMarginS,
	LPCRECT lprcTextMarginD,
	BOOL bRedraw/* = TRUE*/)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_rcTextMargin[0] = *lprcTextMarginN;
	pMember->m_rcTextMargin[1] = *lprcTextMarginH;
	pMember->m_rcTextMargin[2] = *lprcTextMarginS;
	pMember->m_rcTextMargin[3] = *lprcTextMarginD;

	if (bRedraw)
	{
		Redraw(this);
	}

	return 0;
}

int CSkinTreeCtrl::GetTextMargin(
	LPRECT lprcTextMarginN,
	LPRECT lprcTextMarginH,
	LPRECT lprcTextMarginS,
	LPRECT lprcTextMarginD) const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (lprcTextMarginN != NULL)
	{
		*lprcTextMarginN = pMember->m_rcTextMargin[0];
	}
	if (lprcTextMarginH != NULL)
	{
		*lprcTextMarginH = pMember->m_rcTextMargin[1];
	}
	if (lprcTextMarginS != NULL)
	{
		*lprcTextMarginS = pMember->m_rcTextMargin[2];
	}
	if (lprcTextMarginD != NULL)
	{
		*lprcTextMarginD = pMember->m_rcTextMargin[3];
	}

	return 0;
}

int CSkinTreeCtrl::EnableItemLine(BOOL bEnable)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_bEnableItemLine = bEnable;

	return 0;
}

BOOL CSkinTreeCtrl::IsEnableItemLine() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_bEnableItemLine;
}

int CSkinTreeCtrl::SetItemLineColor(COLORREF cr)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_crItemLine = cr;

	return 0;
}

COLORREF CSkinTreeCtrl::GetItemLineColor() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_crItemLine;
}

int CSkinTreeCtrl::SetCombineCollapseExpandedImage(LPCTSTR lpszImage)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	IUISetControlImage(&pMember->m_himgCombineCollapseExpanded, lpszImage);

	if (pMember->m_himgCombineCollapseExpanded != NULL)
	{
		pMember->m_eCollapseExpandedImageResizeMode = IRM_CENTER;
	}

	return 0;
}

int CSkinTreeCtrl::SetCollapseImage(UINT uMask,
	LPCTSTR lpszImageNameCollapseN, LPCTSTR lpszImageNameCollapseH,
	LPCTSTR lpszImageNameCollapseS, LPCTSTR lpszImageNameCollapseD)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	IUISetControlImage4(uMask, pMember->m_himgCollapse,
		lpszImageNameCollapseN, lpszImageNameCollapseH,
		lpszImageNameCollapseS, lpszImageNameCollapseD);

	for (int i = 0; i < 4; ++i)
	{
		if (pMember->m_himgCollapse[i] != NULL)
		{
			pMember->m_eCollapseExpandedImageResizeMode = IRM_CENTER;
		}
	}

	return 0;
}

int CSkinTreeCtrl::GetCollapseImage(UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameH,
	CString *pstrImageNameS, CString *pstrImageNameD) const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = false;

	return IUIGetControlImage4(uMask, pMember->m_himgCombineCollapseExpanded,
			pMember->m_himgCollapse, *pbCombineImage,
			pstrCombineImageName,
			pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);
}

int CSkinTreeCtrl::SetExpandedImage(UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
	LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	IUISetControlImage4(uMask, pMember->m_himgExpanded,
		lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);

	for (int i = 0; i < 4; ++i)
	{
		if (pMember->m_himgExpanded[i] != NULL)
		{
			pMember->m_eCollapseExpandedImageResizeMode = IRM_CENTER;
		}
	}

	return 0;
}

int CSkinTreeCtrl::GetExpandedImage(UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameH,
	CString *pstrImageNameS, CString *pstrImageNameD) const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = false;

	return IUIGetControlImage4(uMask, pMember->m_himgCombineCollapseExpanded,
			pMember->m_himgCollapse, *pbCombineImage,
			pstrCombineImageName,
			pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);

	return 0;
}

int CSkinTreeCtrl::EnableRootItemEffect(BOOL bEnable)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_bEnableRootItemEffect = bEnable;

	return 0;
}

BOOL CSkinTreeCtrl::IsRootItemEffectEnabled() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_bEnableRootItemEffect;
}

int CSkinTreeCtrl::SetRootItemTextColor(COLORREF cr)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_crRootItemText = cr;

	return 0;
}

COLORREF CSkinTreeCtrl::GetRootItemTextColor() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_crRootItemText;
}

int CSkinTreeCtrl::SetRootItemBkType(BACKGROUND_TYPE eBkType)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_eRootItemBkType = eBkType;

	return 0;
}

BACKGROUND_TYPE CSkinTreeCtrl::GetRootItemBkType() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_eRootItemBkType;
}

int CSkinTreeCtrl::SetRootItemBkColor(COLORREF cr)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_crRootItemBK = cr;

	return 0;
}

COLORREF CSkinTreeCtrl::GetRootItemBkColor() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_crRootItemBK;
}

int CSkinTreeCtrl::SetRootItemBkImages(UINT uMask,
	LPCTSTR lpszImageNameBkFocus, LPCTSTR lpszImageNameBkNoFocus)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_bRootItemBkCombine = false;
	IUISetControlImage2(uMask, pMember->m_himgRootItemBk, lpszImageNameBkFocus, lpszImageNameBkNoFocus);

	return 0;
}

int CSkinTreeCtrl::GetRootItemBkImages(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameBkFocus, CString *pstrImageNameBkNoFocus)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = pMember->m_bRootItemBkCombine;
	IUIGetControlImage2(uMask,
		pMember->m_himgRootItemCombineBk, pMember->m_himgRootItemBk, m_bBkCombine, pstrCombineImageName,
		pstrImageNameBkFocus, pstrImageNameBkNoFocus);

	return 0;
}

int CSkinTreeCtrl::SetSelectedItemBkColor(COLORREF cr)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_crSelectedItemBK = cr;
	return 0;
}

COLORREF CSkinTreeCtrl::GetSelectedItemBkColor() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_crSelectedItemBK;
}

int CSkinTreeCtrl::SetSelectedItemBkType(BACKGROUND_TYPE eBkType)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_eSelectedItemBkType = eBkType;

	return 0;
}

BACKGROUND_TYPE CSkinTreeCtrl::GetSelectedItemBkType() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_eSelectedItemBkType;
}

int CSkinTreeCtrl::SetSelectedItemBkImages(UINT uMask, LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameD)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_bSelectedItemBkCombine = false;
	IUISetControlImage2(uMask, pMember->m_himgSelectedItemBk, lpszImageNameN, lpszImageNameD);

	return 0;
}

int CSkinTreeCtrl::GetSelectedItemBkImages(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameD)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = pMember->m_bSelectedItemBkCombine;
	IUIGetControlImage2(uMask, pMember->m_himgSelectedItemCombineBk, pMember->m_himgSelectedItemBk,
		pMember->m_bSelectedItemBkCombine, pstrCombineImageName,
		pstrImageNameN, pstrImageNameD);

	return 0;
}

int CSkinTreeCtrl::SetSelectedItemTextColor(COLORREF cr)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_crSelectedItemText = cr;
	return 0;
}

COLORREF CSkinTreeCtrl::GetSelectedItemTextColor() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_crSelectedItemText;
}


int CSkinTreeCtrl::EnableHighlightItemEffect(BOOL bEnable)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_bEnableHighlightItemEffect = bEnable;
	return 0;
}

BOOL CSkinTreeCtrl::IsHighlightItemEffectEnabled() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_bEnableHighlightItemEffect;
}

HTREEITEM CSkinTreeCtrl::GetHighlightItem() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_hCurHighlightItem;
}

int CSkinTreeCtrl::SetHighlightItemBkColor(COLORREF cr)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_crHighlightBK = cr;
	return 0;
}

COLORREF CSkinTreeCtrl::GetHighlightItemBkColor() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_crHighlightBK;
}

int CSkinTreeCtrl::SetHighlightItemBkImages(UINT uMask,
	LPCTSTR lpszImageNameBkFocus, LPCTSTR lpszImageNameBkNoFocus)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_bHighlightItemBkCombine = false;
	IUISetControlImage2(uMask, pMember->m_himgHighlightItemBk, lpszImageNameBkFocus, lpszImageNameBkNoFocus);

	return 0;
}

int CSkinTreeCtrl::GetHighlightItemBkImages(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameBkFocus, CString *pstrImageNameBkNoFocus)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = pMember->m_bHighlightItemBkCombine;
	IUIGetControlImage2(uMask,
		pMember->m_himgHighlightItemCombineBk, pMember->m_himgHighlightItemBk, *pbCombineImage,
		pstrCombineImageName,
		pstrImageNameBkFocus, pstrImageNameBkNoFocus);

	return 0;
}

int CSkinTreeCtrl::SetHighlightItemTextColor(COLORREF cr)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_crHighlightItemText = cr;
	return 0;
}

COLORREF CSkinTreeCtrl::GetHighlightItemTextColor() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_crHighlightItemText;
}

int CSkinTreeCtrl::SetDisabledItemTextColor(COLORREF cr)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_crDisabledItemText = cr;
	return 0;
}

COLORREF CSkinTreeCtrl::GetDisabledItemTextColor() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_crDisabledItemText;
}

// Insert a windowless control to a report list sub item.
// The report list like a CGridPanel.
SItemBindObject *CSkinTreeCtrl::SetItemControl(HTREEITEM hItem, CWLWnd *pWLItem)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return NULL;
}

// The bind window ID will create control, and store in LV_ITEMDATA::mapWLWnd at first become visible.
SItemBindObject *CSkinTreeCtrl::SetItemControl(HTREEITEM hItem, UINT uBindWndID)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (hItem == NULL)
	{
		return NULL;
	}

	TV_ITEMDATA *pData = TreeView_GetItemData(m_hWnd, pMember, hItem);

	if (pData->pBindObject == NULL)
	{
		pData->pBindObject = new SItemBindObject;
	}

	pData->pBindObject->uBindWndID = uBindWndID;

	// Delay create the windowless control

	return pData->pBindObject;
}

int CSkinTreeCtrl::ReleaseItemControl(HTREEITEM hItem)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (hItem == NULL)
	{
		return NULL;
	}

	TV_ITEMDATA *pData = TreeView_GetItemData(m_hWnd, pMember, hItem);

	if (pData->pBindObject != NULL)
	{
		pData->pBindObject->ReleaseChildren(TRUE);
		delete pData->pBindObject;
		pData->pBindObject = NULL;
	}

	return 0;
}

CWLWnd *CSkinTreeCtrl::GetItemTemplateItem(HTREEITEM hItem, UINT uItemID)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (hItem == NULL || uItemID <= 0)
	{
		return NULL;
	}

	pMember->m_bSetItemDataInternal = TRUE;
	TV_ITEMDATA *pData = (TV_ITEMDATA *)GetItemData(hItem);
	pMember->m_bSetItemDataInternal = FALSE;

	if (pData == NULL || pData->pBindObject == NULL)
	{
		ASSERT(FALSE);
		return NULL;
	}

	if (!pData->pBindObject->bAlreadyCreateChildren)
	{
		return NULL;
	}

	CWLWnd *pRet = NULL;

	std::vector<CChildInfo>::iterator it = pData->pBindObject->vSubWLWnd.begin();
	for (; it != pData->pBindObject->vSubWLWnd.end(); ++it)
	{
		// Only windowless control can in item template.
		ASSERT(it->m_eControlType >= CT_WL_FIRST && it->m_eControlType <= CT_WL_LAST);
		if ((UINT)(LONG)it->m_pCtrlProperties->m_nID == uItemID)
		{
			pRet = (CWLWnd *)it->m_pChildCtrl;
			break;
		}
	}

	return pRet;
}

int CSkinTreeCtrl::SetTag(LPCTSTR lpszTag)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (lpszTag == NULL)
	{
		return -1;
	}

	pMember->m_strTag = lpszTag;

	return 0;
}

int CSkinTreeCtrl::GetTag(CString &strTag) const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	strTag = pMember->m_strTag;

	return 0;
}

int CSkinTreeCtrl::SetTextFont(UINT uMask, LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDD /* = NULL*/)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	IUISetControlFont2(uMask, pMember->m_hIUIFont, lpszFontIDN, lpszFontIDD);

	HFONT hFont = NULL;
	if (IsWindowEnabled())
	{
		hFont = GetHFont(pMember->m_hIUIFont[0]);
	}
	else
	{
		hFont = GetHFont(pMember->m_hIUIFont[1]);
	}
	::SendMessage(m_hWnd, WM_SETFONT, (WPARAM)hFont, TRUE);

	return 0;
}

int CSkinTreeCtrl::GetTextFont(UINT uMask, CString *pstrFontIDN, CString *pstrFontIDD/* = NULL*/) const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (pstrFontIDN != NULL)
	{
		GetFontResID(pMember->m_hIUIFont[0], pstrFontIDN);
	}

	if (pstrFontIDD != NULL)
	{
		GetFontResID(pMember->m_hIUIFont[1], pstrFontIDD);
	}

	return 0;
}

int CSkinTreeCtrl::SetHoverTime(DWORD dwHoverTime, BOOL bRepeat/*=FALSE*/)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_dwHoverTime = dwHoverTime;
	pMember->m_bHoverRepeat = bRepeat;

	return 0;
}

int CSkinTreeCtrl::GetHoverTime(DWORD *pdwHoverTime, BOOL *pbRepeat) const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (pdwHoverTime != NULL)
	{
		*pdwHoverTime = pMember->m_dwHoverTime;
	}
	if (pbRepeat != NULL)
	{
		*pbRepeat = pMember->m_bHoverRepeat;
	}

	return 0;
}

int CSkinTreeCtrl::SetItemHeightEx(HTREEITEM hItem, UINT cyItemHeight)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	TV_ITEMDATA *pTvItemData = TreeView_GetItemData(m_hWnd, pMember, hItem);

	if (pTvItemData == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	int nItemOldHeight = pTvItemData->nItemHeight;
	if (nItemOldHeight == 0)
	{
		nItemOldHeight = GetItemHeight();
	}

	pTvItemData->nItemHeight = cyItemHeight;

	UpdateBranchHeightAfterItemHeightChanged(m_hWnd, pMember, hItem,
		nItemOldHeight, pTvItemData->nItemHeight);

	_LibUIDK_TV_CalcScrollBars(m_hWnd, pMember);

	// Must call SetRedraw(TRUE) to refresh the tree to update item rect and scroll bar.
	SetRedraw(TRUE);

	return 0;
}

UINT CSkinTreeCtrl::GetItemHeightEx(HTREEITEM hItem) const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	TV_ITEMDATA *pTvItemData = TreeView_GetItemData(m_hWnd, pMember, hItem);
	if (pTvItemData == NULL)
	{
		return 0;
	}

	return pTvItemData->nItemHeight;
}

// Not include children's children.
int CSkinTreeCtrl::GetChildrenCount(HTREEITEM hItem) const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	int nCount = 0;

	hItem = GetChildItem(hItem);
	while (hItem != NULL)
	{
		++nCount;

		hItem = GetNextItem(hItem, TVGN_NEXT);
	}

	return nCount;
}

int CSkinTreeCtrl::CollapseAll()
{
	HTREEITEM hRoot = GetRootItem();
	while (hRoot != NULL)
	{
		Expand(hRoot, TVE_COLLAPSE);

		hRoot = GetNextSiblingItem(hRoot);
	}

	return 0;
}

int CSkinTreeCtrl::RegisterDropTarget()
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	BOOL bRet = pMember->m_dropTarget.Register(this);

	return bRet ? 0 : -1;
}

int CSkinTreeCtrl::OnDrawBackground(CDC *pDC)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	CRect rcClient;
	GetClientRect(rcClient);

	if (pMember->m_bCacheMode)
	{
		LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		BOOL bBorder = ((lStyle & WS_BORDER) == WS_BORDER);

		if (pMember->m_himgBk[0])
		{
			IUIBitBlt(pDC->GetSafeHdc(), 0, 0, rcClient.Width(), rcClient.Height(), pMember->m_hLevel1ResizedBkImage, bBorder ? 1 : 0, bBorder ? 1 : 0, SRCCOPY);
		}
		else
		{
			COLORREF crBk = GetBackgroundColor();
			pDC->FillSolidRect(rcClient, crBk);
		}
	}
	else
	{
		int nStatusIndex = 0;
		if (IsWindowEnabled())
		{
			nStatusIndex = 0;
		}
		else
		{
			nStatusIndex = 1;
		}

		// Draw the parent background to control for alpha blend with background of the control.
		BOOL bDrawParentPart = FALSE;
		if (pMember->m_himgBk[nStatusIndex]->GetSafeHBITMAP() == NULL
			|| pMember->m_himgBk[nStatusIndex]->IsHasAlphaChannel())
		{
			CRect rcWin;
			GetWindowRect(rcWin);
			CWnd *pParent = GetParent();
			pParent->ScreenToClient(rcWin);

			BOOL bUpdateLayeredWindow = FALSE;
			DrawParentPart(this, pDC, rcWin, &bUpdateLayeredWindow, NULL);
			bDrawParentPart = TRUE;
		}

		if (IsDrawBkColor())
		{
			CRect rcClient;
			GetClientRect(rcClient);

			COLORREF cr = GetBackgroundColor();
			pDC->FillSolidRect(rcClient, cr);
		}

		IUIDrawBackground(this, pDC, pMember->m_himgBk[nStatusIndex],
			m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
	}

	return 0;
}

int CSkinTreeCtrl::OnDrawItem(CDC *pDC, HTREEITEM hItem, LPCRECT lprcItem, int nLeftMargin/* = 0*/)
{
	if (pDC == NULL || lprcItem == 0)
	{
		return -1;
	}

	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	DRAWITEMSTRUCT dis;
	dis.CtlType = ODT_TREELIST;
	dis.CtlID = GetDlgCtrlID();
	dis.itemID = UINT(UINT_PTR(hItem));
	dis.hwndItem = m_hWnd;
	dis.hDC = pDC->GetSafeHdc();
	dis.itemData = GetItemData(hItem);
	CRect rcItem = lprcItem;
	dis.rcItem = rcItem;

	BOOL bParentDraw = (BOOL)GetParent()->SendMessage(WM_DRAWITEMEX, dis.CtlID, LPARAM(&dis));
	if (bParentDraw)
	{
		return 0;
	}

	CRect rcText;
	GetItemRect(hItem, rcText, TRUE);
	int nTextLeft = rcText.left;
	rcText = rcItem;
	rcText.left = nTextLeft;

	// Generate a bitmap by the item UI.
	if (pMember->m_bAnimateRemoveMode
		&& hItem == pMember->m_hRemoveItem
		&& pMember->m_bmpCachedRemovedItem.GetSafeHandle() == NULL)
	{
		// Cache the item draw.
		CDC dcMemDest;
		dcMemDest.CreateCompatibleDC(pDC);

		pMember->m_bmpCachedRemovedItem.CreateCompatibleBitmap(pDC, rcItem.Width(), rcItem.Height());
		HBITMAP hBmpOld = (HBITMAP)::SelectObject(dcMemDest.GetSafeHdc(),
				(HBITMAP)pMember->m_bmpCachedRemovedItem.GetSafeHandle());

		// Copy pDC the cached bitmap
		dcMemDest.BitBlt(0, 0, rcItem.Width(), rcItem.Height(),
			pDC, rcItem.left, rcItem.top, SRCCOPY);

		// Draw item background
		CRect rcItemDraw = rcItem;
		rcItemDraw.OffsetRect(-rcItem.left, -rcItem.top);
		OnDrawItemBk(&dcMemDest, hItem, rcItemDraw);

		// Draw button and image
		CRect rcTextDraw = rcText;
		rcTextDraw.OffsetRect(-rcItem.left, -rcItem.top);
		OnDrawItemIcon(&dcMemDest, hItem, rcItemDraw, rcTextDraw, nLeftMargin);

		// Draw text
		OnDrawText(&dcMemDest, hItem, rcItemDraw, rcTextDraw, nLeftMargin);

		::SelectObject(dcMemDest, hBmpOld);
		//
		// 		CWindowDC dc(NULL);
		// 		BitBltG(dc.GetSafeHdc(), rcItem,
		// (HBITMAP)pMember->m_bmpCachedRemovedItem.GetSafeHandle(), 0, 0, SRCCOPY);
	}

	// Generate a bitmap by the item UI.
	if (pMember->m_bAnimationInsertItem
		&& hItem == pMember->m_hInsertItem
		&& pMember->m_bmpCachedInsertItem.GetSafeHandle() == NULL)
	{
		// Cache the item draw.
		CDC dcMemDest;
		dcMemDest.CreateCompatibleDC(pDC);

		pMember->m_bmpCachedInsertItem.CreateCompatibleBitmap(pDC, rcItem.Width(), rcItem.Height());
		HBITMAP hBmpOld = (HBITMAP)::SelectObject(dcMemDest.GetSafeHdc(),
				(HBITMAP)pMember->m_bmpCachedInsertItem.GetSafeHandle());

		// Copy pDC the cached bitmap
		dcMemDest.BitBlt(0, 0, rcItem.Width(), rcItem.Height(),
			pDC, rcItem.left, rcItem.top, SRCCOPY);

		// Draw item background
		CRect rcItemDraw = rcItem;
		rcItemDraw.OffsetRect(-rcItem.left, -rcItem.top);
		OnDrawItemBk(&dcMemDest, hItem, rcItemDraw);

		// Draw item line
		if (IsEnableItemLine())
		{
			OnDrawItemLine(&dcMemDest, hItem, rcItemDraw);
		}

		// Draw button and image
		CRect rcTextDraw = rcText;
		rcTextDraw.OffsetRect(-rcItem.left, -rcItem.top);
		OnDrawItemIcon(&dcMemDest, hItem, rcItemDraw, rcTextDraw, nLeftMargin);

		// Draw text
		OnDrawText(&dcMemDest, hItem, rcItemDraw, rcTextDraw, nLeftMargin);

		::SelectObject(dcMemDest, hBmpOld);
	}

	// Draw item background
	OnDrawItemBk(pDC, hItem, rcItem);

	// Draw item line
	if (IsEnableItemLine())
	{
		OnDrawItemLine(pDC, hItem, rcItem);
	}

	if (pMember->m_hRemoveItem == hItem)
	{
		if (pMember->m_bAnimateRemoveMode)
		{
			if (pMember->m_nCurRemoveItemTimerTick > 0)
			{
				CRect rcDraw;
				rcDraw.left = rcItem.CenterPoint().x
					- rcItem.Width()
					* pMember->m_vRemoveItemAnimationData[pMember->m_nCurRemoveItemTimerTick].nPercent
					/ 200;
				rcDraw.top = rcItem.CenterPoint().y
					- rcItem.Height()
					* pMember->m_vRemoveItemAnimationData[pMember->m_nCurRemoveItemTimerTick].nPercent
					/ 200;
				rcDraw.right = rcDraw.left
					+ rcItem.Width()
					* pMember->m_vRemoveItemAnimationData[pMember->m_nCurRemoveItemTimerTick].nPercent
					/ 100;
				rcDraw.bottom = rcDraw.top
					+ rcItem.Height()
					* pMember->m_vRemoveItemAnimationData[pMember->m_nCurRemoveItemTimerTick].nPercent
					/ 100;

				// FadeInOut
				float fAlpha = (float)pMember->m_vRemoveItemAnimationData[pMember->m_nCurRemoveItemTimerTick].nTransparent / (float)255;
				ColorMatrix cm =
				{
					1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f, fAlpha, 0.0f,
					0.0f, 0.0f, 0.0f, 0.0f, 1.0f
				};

				ImageAttributes ia;
				ia.SetColorMatrix(&cm);

				Bitmap *bmp = Bitmap::FromHBITMAP((HBITMAP)pMember->m_bmpCachedRemovedItem.GetSafeHandle(), NULL);

				Graphics g(pDC->GetSafeHdc());
				g.DrawImage(bmp, Rect(rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height()), 0, 0, rcItem.Width(), rcItem.Height(), UnitPixel, &ia);
			}
		}
		else
		{
			// Remove item animation end, and Delete item animation not start. Don't show the item.
		}
	}
	else if (pMember->m_hInsertItem == hItem)
	{
		if (pMember->m_bAnimationInsertItem
			&& pMember->m_nCurInsertItemTimerTick < (int)pMember->m_vInsertItemAnimationData.size())
		{
			CRect rcDraw;
			rcDraw.left = rcItem.CenterPoint().x
				- rcItem.Width()
				* pMember->m_vInsertItemAnimationData[pMember->m_nCurInsertItemTimerTick].nPercent
				/ 200;
			rcDraw.top = rcItem.CenterPoint().y
				- rcItem.Height()
				* pMember->m_vInsertItemAnimationData[pMember->m_nCurInsertItemTimerTick].nPercent
				/ 200;
			rcDraw.right = rcDraw.left
				+ rcItem.Width()
				* pMember->m_vInsertItemAnimationData[pMember->m_nCurInsertItemTimerTick].nPercent
				/ 100;
			rcDraw.bottom = rcDraw.top
				+ rcItem.Height()
				* pMember->m_vInsertItemAnimationData[pMember->m_nCurInsertItemTimerTick].nPercent
				/ 100;

			// FadeInOut
			float fAlpha = (float)pMember->m_vInsertItemAnimationData[pMember->m_nCurInsertItemTimerTick].nTransparent / (float)255;
			ColorMatrix cm =
			{
				1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, fAlpha, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f, 1.0f
			};

			ImageAttributes ia;
			ia.SetColorMatrix(&cm);

			Bitmap *bmp = Bitmap::FromHBITMAP((HBITMAP)pMember->m_bmpCachedInsertItem.GetSafeHandle(), NULL);

			Graphics g(pDC->GetSafeHdc());
			g.DrawImage(bmp, Rect(rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height()),
				0, 0, rcItem.Width(), rcItem.Height(), UnitPixel, &ia);
		}
		else
		{

		}
	}
	else
	{
		pMember->m_bSetItemDataInternal = TRUE;
		TV_ITEMDATA *pData = (TV_ITEMDATA *)GetItemData(hItem);
		pMember->m_bSetItemDataInternal = FALSE;

		if (pData != NULL && pData->pBindObject != NULL)
		{
			if (pData->pBindObject->uBindWndID > 0)
			{
				if (!pData->pBindObject->bAlreadyCreateChildren)
				{
					pMember->m_bWindowlessNotCreateAsChild = true;
					_InstantiateItemBindTemplate(pData->pBindObject, this);
					pMember->m_bWindowlessNotCreateAsChild = false;

					// Do something on instantiating item template.
					NMHDR nmhdr;
					nmhdr.hwndFrom = m_hWnd;
					nmhdr.idFrom = GetDlgCtrlID();
					nmhdr.code = TVN_INSTANTIATE_ITEM_TEMPLATE;
					NMTREEVIEW tvhdr;
					tvhdr.hdr = nmhdr;
					tvhdr.itemNew.hItem = hItem;
					GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&tvhdr));
				}

				if (pData->pBindObject->bAlreadyCreateChildren)
				{
					if (!pData->pBindObject->vSubWLWnd.empty())
					{
						// Relayout the children.
						if (rcItem != pData->pBindObject->rcLastParentLayout)
						{
							_RelayoutItemBindObject(this, rcItem, pData->pBindObject);
						}

						// Draw windowless children.
						std::vector<CChildInfo>::iterator it = pData->pBindObject->vSubWLWnd.begin();
						for (; it != pData->pBindObject->vSubWLWnd.end(); ++it)
						{
							CONTROL_TYPE eControlType = it->m_eControlType;
							if (eControlType == CT_WL_RECTCTRL
								|| eControlType == CT_WL_SPLITTER
								|| eControlType == CT_WL_LINE
								|| eControlType == CT_WL_TEXT
								|| eControlType == CT_WL_PICTURE
								|| eControlType == CT_WL_BUTTON
								|| eControlType == CT_WL_CHECK
								|| eControlType == CT_WL_RADIO
								|| eControlType == CT_WL_SLIDER
								|| eControlType == CT_WL_RICHEDIT
								|| eControlType == CT_WL_RICHEDIT_IM
								|| eControlType == CT_TASK_WND_MGR
								|| eControlType == CT_PNL_DOCK
								|| eControlType == CT_UNIFORM_GRID
								|| eControlType == CT_PNL_STACK)
							{
								CWLWnd *pWLWnd = (CWLWnd *)it->m_pChildCtrl;

								if (!pWLWnd->IsCreated())
								{
									continue;
								}

								if (pWLWnd->IsWindowVisible())
								{
									//pWLWnd->SetDrawOffset(rcItem.TopLeft());

									// Apply child's region.
									HRGN hOldRgn = ApplyWLRgn(this, pDC, pWLWnd);

									DrawWLWindow(pDC->GetSafeHdc(), pWLWnd);

									::SelectClipRgn(pDC->GetSafeHdc(), hOldRgn);
									DeleteObject(hOldRgn);
								}
							}
						}
					}
				}
			}
		}
		else
		{
			// Draw button and image
			OnDrawItemIcon(pDC, hItem, rcItem, rcText, nLeftMargin);

			// Draw text
			OnDrawText(pDC, hItem, rcItem, rcText, nLeftMargin);
		}
	}

	// Draw focus rect
	LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	if ((lStyle & WS_TABSTOP) && GetFocus()->GetSafeHwnd() == m_hWnd)
	{
		UINT uState = GetItemState(hItem, TVIS_SELECTED);
		if (uState & TVIS_SELECTED)
		{
			pDC->DrawFocusRect(rcItem);
		}
	}

	return 0;
}

int CSkinTreeCtrl::OnDrawItemBk(CDC *pDC, HTREEITEM hItem, const CRect &rcItem)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	DRAWITEMSTRUCT dis;
	dis.CtlType = ODT_TREELIST;
	dis.CtlID = GetDlgCtrlID();
	dis.itemID = UINT(UINT_PTR(hItem));
	dis.hwndItem = m_hWnd;
	dis.hDC = pDC->GetSafeHdc();
	dis.itemData = GetItemData(hItem);
	dis.rcItem = rcItem;

	BOOL bParentDrawItemBk = (BOOL)GetParent()->SendMessage(WM_DRAWITEMBK, dis.CtlID, LPARAM(&dis));
	if (bParentDrawItemBk)
	{
		return 0;
	}

	UINT nSelectedState = GetItemState(hItem, TVIS_SELECTED);
	BOOL bRootItem = (GetParentItem(hItem) == NULL) ? TRUE : FALSE;

	// draw selected item.
	if (TVIS_SELECTED & nSelectedState)
	{
		pDC->SetTextColor(GetSelectedItemTextColor());
		CWnd *pFocusWnd = GetFocus();

		HIUIIMAGE himgSelItemBkN = NULL;
		HIUIIMAGE himgSelItemBkD = NULL;
		HIUIIMAGE himgItemBK = NULL;
		BACKGROUND_TYPE eItemBkType = BKT_COLOR;
		COLORREF crItemBk = RGB(0, 0, 128);

		if (bRootItem && IsRootItemEffectEnabled())
		{
			eItemBkType = GetRootItemBkType();

			if (eItemBkType == BKT_IMAGE)
			{
				himgSelItemBkN = pMember->m_himgRootItemBk[0];
				himgSelItemBkD = pMember->m_himgRootItemBk[1];
			}
			else
			{
				crItemBk = GetRootItemBkColor();
			}
		}
		else
		{
			eItemBkType = GetSelectedItemBkType();

			if (eItemBkType == BKT_IMAGE)
			{
				himgSelItemBkN = pMember->m_himgSelectedItemBk[0];
				himgSelItemBkD = pMember->m_himgSelectedItemBk[1];
			}
			else
			{
				crItemBk = GetSelectedItemBkColor();
			}
		}

		if (eItemBkType == BKT_IMAGE)
		{
			if (himgSelItemBkN != NULL && pFocusWnd == this)
			{
				himgItemBK = himgSelItemBkN;
			}
			else if (himgSelItemBkD != NULL)
			{
				himgItemBK = himgSelItemBkD;
			}

			// Draw selected item background
			IUINineGridBlt(pDC->GetSafeHdc(),
				rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(),
				himgItemBK, pMember->m_ptSelectedItemBkResize, 1, 1);
		}
		else
		{
			pDC->FillSolidRect(rcItem, crItemBk);
		}
	}

	// Item is highlight
	else if (IsHighlightItemEffectEnabled() && (GetHighlightItem() == hItem))
	{
		CWnd *pFocusWnd = GetFocus();

		HIUIIMAGE himgItemBK = NULL;
		if (pMember->m_himgHighlightItemBk[0] != NULL && pFocusWnd == this)
		{
			himgItemBK = pMember->m_himgHighlightItemBk[0];
		}
		else if (pMember->m_himgHighlightItemBk[1] != NULL)
		{
			himgItemBK = pMember->m_himgHighlightItemBk[1];
		}

		// Draw highlight item background
		if (himgItemBK != NULL && *himgItemBK != NULL)
		{
			IUINineGridBlt(pDC->GetSafeHdc(),
				rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(),
				himgItemBK, pMember->m_ptHighlightItemBkResize, 1, 1);
		}
		else
		{
			pDC->FillSolidRect(rcItem, GetHighlightItemBkColor());
		}
	}

	// Normal
	else
	{
		CWnd *pFocusWnd = GetFocus();

		HIUIIMAGE himgItemBK = NULL;
		CPoint ptResize(3, 3);
		// Normal root item
		BACKGROUND_TYPE eItemBkType = BKT_IMAGE;
		COLORREF crItemBk = RGB(255, 255, 255);
		if (bRootItem && IsRootItemEffectEnabled())
		{
			eItemBkType = GetRootItemBkType();

			if (eItemBkType == BKT_COLOR)
			{
				crItemBk = GetRootItemBkColor();
			}

			if (pFocusWnd == this)
			{
				himgItemBK = pMember->m_himgRootItemBk[0];
			}
			else
			{
				himgItemBK = pMember->m_himgRootItemBk[1];
			}
		}
		// Other normal item
		else
		{
		}

		if (eItemBkType == BKT_IMAGE)
		{
			if (himgItemBK != NULL && *himgItemBK != NULL)
			{
				IUINineGridBlt(pDC->GetSafeHdc(),
					rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(),
					himgItemBK, pMember->m_ptRootItemBkResize, 1, 1);
			}
		}
		else
		{
			pDC->FillSolidRect(rcItem, crItemBk);
		}

		pDC->SetTextColor(RGB(0, 0, 0));
	}

	return 0;
}

int CSkinTreeCtrl::OnDrawItemLine(CDC *pDC, HTREEITEM hItem, const CRect &rcItem)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	ASSERT(m_hWnd != NULL && pDC != NULL);

	LOGPEN lpLine;
	lpLine.lopnColor = GetItemLineColor();
	lpLine.lopnStyle = PS_SOLID;
	lpLine.lopnWidth = CPoint(1, 1);
	CPen penLine;
	penLine.CreatePenIndirect(&lpLine);
	CGdiObject *pOldLine = pDC->SelectObject(&penLine);

	pDC->MoveTo(rcItem.left, rcItem.bottom - 1);
	pDC->LineTo(rcItem.right, rcItem.bottom - 1);

	pDC->SelectObject(pOldLine);

	return 0;
}

int CSkinTreeCtrl::OnDrawItemIcon(CDC *pDC, HTREEITEM hItem,
	const CRect &rcItem, const CRect &rcText, int nLeftMargin/* = 0*/)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	// Get the icon size in normal image list.
	int nIconWidth = 0;
	int nIconHeight = 0;
	CImageList *pImageList = GetImageList(TVSIL_NORMAL);
	if (pImageList != NULL)
	{
		ImageList_GetIconSize(pImageList->GetSafeHandle(), &nIconWidth, &nIconHeight);
	}

	//
	// 1. Draw state icon(TVSIL_STATE, Check box is state icon)
	//
	// Get the icon size in state image list.
	int nStateIconWidth = 0;
	int nStateIconHeight = 0;
	CImageList *pImageListState = GetImageList(TVSIL_STATE);
	if (pImageListState != NULL)
	{
		ImageList_GetIconSize(pImageListState->GetSafeHandle(), &nStateIconWidth, &nStateIconHeight);

		// Get item state index
		UINT nState = GetItemState(hItem, TVIS_STATEIMAGEMASK);
		int nStateImageIndex = nState >> 12;

		if (nStateImageIndex > 0)		// Index is 1-base.(List control is 0-base)
		{
			HICON hIcon = pImageListState->ExtractIcon(nStateImageIndex);

			CRect rcIcon = rcItem;
			rcIcon.right = (pImageList == NULL) ? rcText.left : (rcText.left - nIconWidth);
			rcIcon.left = rcIcon.right - nStateIconWidth;
			// VCenter
			rcIcon.top += (rcIcon.Height() - nStateIconHeight) / 2;
			rcIcon.bottom = rcIcon.top + nStateIconHeight;

			// if the 5th or 6th parameter of DrawIconEx is 0, use full size to draw.
			if (IsWindowEnabled())
			{
				::DrawIconEx(pDC->GetSafeHdc(), rcIcon.left + nLeftMargin, rcIcon.top,
					hIcon, rcIcon.Width(), rcIcon.Height(), 1, NULL, DI_NORMAL);
			}
			else
			{
				::DrawState(pDC->GetSafeHdc(), NULL, NULL,
					(LPARAM)hIcon, 0, rcIcon.left + nLeftMargin, rcIcon.top,
					rcIcon.Width(), rcIcon.Height(), DSS_DISABLED | DST_ICON);
			}

			::DestroyIcon(hIcon);
		}
	}

	//
	// 2. Draw item icon(TVSIL_NORMAL)
	//
	// If the collapse and expanded image is valid, use the image to instead of +- button.
	if ((pMember->m_himgCombineCollapseExpanded != NULL
			|| pMember->m_himgCollapse != NULL
			|| pMember->m_himgExpanded != NULL)
		&& ItemHasChildren(hItem))
	{
		UINT nState = GetItemState(hItem, TVIS_EXPANDED);
		CRect rcDest(nLeftMargin, rcItem.top, rcText.left + nLeftMargin, rcItem.bottom);
		if (nState & TVIS_EXPANDED)
		{
			if (pMember->m_himgCombineCollapseExpanded != NULL)
			{
				IUIPartDrawImage(pDC->GetSafeHdc(), rcDest,
					pMember->m_himgCombineCollapseExpanded,
					pMember->m_eCollapseExpandedImageResizeMode,
					pMember->m_ptCollapseExpandedImageResize, 1, 1, 8, 4);
			}
			else
			{
				IUICenterBitBlt(pDC->GetSafeHdc(), rcDest, pMember->m_himgExpanded[0], SRCCOPY);
			}
		}
		else
		{
			if (pMember->m_himgCombineCollapseExpanded != NULL)
			{
				IUIPartDrawImage(pDC->GetSafeHdc(), rcDest,
					pMember->m_himgCombineCollapseExpanded,
					pMember->m_eCollapseExpandedImageResizeMode,
					pMember->m_ptCollapseExpandedImageResize, 1, 1, 8, 0);
			}
			else
			{
				IUICenterBitBlt(pDC->GetSafeHdc(), rcDest, pMember->m_himgCollapse[0], SRCCOPY);
			}
		}
	}
	else
	{
		// The interval between icon right and rcText.left(Not the text left)
		const int nIntervalIconRcTextLeft = 3;

		//
		// Draw +- button and line
		//
		LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		UINT nSelectedState = GetItemState(hItem, TVIS_SELECTED);
		BOOL bSelected = ((nSelectedState & TVIS_SELECTED) == TVIS_SELECTED);
		BOOL bHasButton = ((lStyle & TVS_HASBUTTONS) == TVS_HASBUTTONS);
		BOOL bHasLine = ((lStyle & TVS_HASLINES) == TVS_HASLINES);
		int nRight = rcText.left - nIconWidth - nIntervalIconRcTextLeft;

		if ((bHasButton || bHasLine) && nRight > 0)
		{
			CRect rcClient;
			GetClientRect(rcClient);

			// Tree content with item bk color
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap bmp;
			bmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
			CBitmap *pbmpOld = memDC.SelectObject(&bmp);
			DefWindowProc(WM_PAINT, WPARAM(memDC.m_hDC), 0);

			// Remove alpha bits.
			COLORREF cr = GetBkColor();
			cr = cr << 8;
			cr = cr >> 8;

			// If Full row select, mask the select background color
			BOOL bFullRowSelect = ((lStyle & TVS_FULLROWSELECT) == TVS_FULLROWSELECT);

			// Icon area may has 3 colors: cr, COLOR_HIGHLIGHT or COLOR_BTNFACE
			// COLOR_HIGHLIGHT: need bFullRowSelect and bSelected and focus
			// COLOR_BTNFACE: need bFullRowSelect and bSelected and no focus

			COLORREF crSelectedItemBk = cr;
			// In full row select mode. the default item background color fill all of the items.
			if (bFullRowSelect && bSelected)
			{
				// If the item has child and show +-, the item selected background color not fill the
				// item. So need transparent draw twice.
				if (GetChildItem(hItem) != NULL)
				{
					crSelectedItemBk = GetBkColor();
					crSelectedItemBk = crSelectedItemBk << 8;
					crSelectedItemBk = crSelectedItemBk >> 8;
				}
				else
				{
					BOOL bFoucs = (GetFocus() == this);
					// If the tree has focus, the select item bk color is COLOR_HIGHLIGHT;
					if (bFoucs)
					{
						crSelectedItemBk = GetSysColor(COLOR_HIGHLIGHT);
					}
					// If the tree hasn't focus, the icon area color
					// of select item is COLOR_BTNFACE(Show select always)
					else
					{
						crSelectedItemBk = GetSysColor(COLOR_BTNFACE);
					}
				}
			}

			TransparentBlt2(pDC->GetSafeHdc(), nLeftMargin, rcItem.top, nRight, rcItem.Height(),
				memDC.GetSafeHdc(), 0, rcItem.top, nRight, rcItem.Height(), crSelectedItemBk);

			memDC.SelectObject(pbmpOld);
		}

		//
		// Draw normal icon
		//
		if (pImageList == NULL)
		{
			return -2;
		}

		int nImage = -1;
		int nSelectedImage = -1;
		BOOL bRet = GetItemImage(hItem, nImage, nSelectedImage);
		if (!bRet)
		{
			return -1;
		}

		CRect rcIcon = rcItem;
		//
		rcIcon.right = rcText.left - nIntervalIconRcTextLeft;
		rcIcon.left = rcIcon.right - nIconWidth;
		// VCenter
		rcIcon.top += (rcIcon.Height() - nIconHeight) / 2;
		rcIcon.bottom = rcIcon.top + nIconHeight;

		// Get image 0-based index of this item
		HICON hIcon = pImageList->ExtractIcon(bSelected ? nSelectedImage : nImage);

		// if the 5th or 6th parameter of DrawIconEx is 0, use full size to draw.
		if (IsWindowEnabled())
		{
			::DrawIconEx(pDC->GetSafeHdc(), rcIcon.left + nLeftMargin, rcIcon.top,
				hIcon, rcIcon.Width(), rcIcon.Height(), 1, NULL, DI_NORMAL);
		}
		else
		{
			::DrawState(pDC->GetSafeHdc(), NULL, NULL,
				(LPARAM)hIcon, 0, rcIcon.left + nLeftMargin, rcIcon.top,
				rcIcon.Width(), rcIcon.Height(), DSS_DISABLED | DST_ICON);
		}

		::DestroyIcon(hIcon);
	}

	return 0;
}

int CSkinTreeCtrl::OnDrawText(CDC *pDC, HTREEITEM hItem, const CRect &rcItem, const CRect &rcText, int nLeftMargin/* = 0*/)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	UINT nSelectedState = GetItemState(hItem, TVIS_SELECTED);

	int nStateIndex = 0;
	if (TVIS_SELECTED & nSelectedState)
	{
		nStateIndex = 2;
		pDC->SetTextColor(GetSelectedItemTextColor());
	}
	else if (IsHighlightItemEffectEnabled() && (GetHighlightItem() == hItem)) // Item is highlight
	{
		nStateIndex = 1;
		pDC->SetTextColor(GetHighlightItemTextColor());
	}
	else
	{
		nStateIndex = 0;
		COLORREF cr = RGB(0, 0, 0);
		if (IsWindowEnabled())
		{
			cr = GetTextColor();
		}
		else
		{
			cr = GetDisabledItemTextColor();
		}
		pDC->SetTextColor(cr);
	}

	pDC->SetBkMode(TRANSPARENT);
	CString strText = GetItemText(hItem);

	CEdit *pEdit = GetEditControl();
	if (pEdit != NULL)
	{
		CRect rcEdit;
		pEdit->GetWindowRect(rcEdit);
		ScreenToClient(rcEdit);

		if (rcText.PtInRect(rcEdit.CenterPoint()))
		{
			strText.Empty();
		}
	}

	CRect rcNew;
	GetItemRect(hItem, rcNew, FALSE);
	rcNew.left = rcText.left;

	rcNew.DeflateRect(pMember->m_rcTextMargin[nStateIndex]);
	rcNew.left += nLeftMargin;
	rcNew.top = rcText.top;
	rcNew.bottom = rcText.bottom;

	HFONT hFont = NULL;
	if (IsWindowEnabled())
	{
		hFont = GetHFont(pMember->m_hIUIFont[0]);
	}
	else
	{
		hFont = GetHFont(pMember->m_hIUIFont[1]);
	}

	CString strDefaultFont;
	if (hFont == NULL)
	{
		strDefaultFont = CUIMgr::GetDefaultFont();
		hFont = CreateIUIFont(strDefaultFont);
		ReleaseIUIFont(strDefaultFont);
	}

	HFONT hFontOld = (HFONT)SelectObject(pDC->GetSafeHdc(), hFont);

	if (strDefaultFont.IsEmpty())
	{
		ReleaseIUIFont(strDefaultFont);
	}

	// adjust horizontal alignment
	UINT uFormat = DT_SINGLELINE;
	if (pMember->m_eTextHorAlignMode[nStateIndex] == TAH_DEFAULT
		|| pMember->m_eTextHorAlignMode[nStateIndex] == TAH_LEFT)
	{
		uFormat |= DT_LEFT;
	}
	else if (pMember->m_eTextHorAlignMode[nStateIndex] == TAH_CENTER)
	{
		uFormat |= DT_CENTER;
	}
	else if (pMember->m_eTextHorAlignMode[nStateIndex] == TAH_RIGHT)
	{
		uFormat |= DT_RIGHT;
	}

	if (pMember->m_eTextVerAlignMode[nStateIndex] == TAV_DEFAULT
		|| pMember->m_eTextVerAlignMode[nStateIndex] == TAV_CENTER)
	{
		uFormat |= DT_VCENTER;
	}
	else if (pMember->m_eTextVerAlignMode[nStateIndex] == TAV_TOP)
	{
		uFormat |= DT_TOP;
	}
	else if (pMember->m_eTextVerAlignMode[nStateIndex] == TAV_BOTTOM)
	{
		uFormat |= DT_BOTTOM;
	}

	//#ifdef _DEBUG
	//	TV_ITEMDATA *p = TreeView_GetItemData(m_hWnd, pMember, hItem);
	//	CString strBranchHeight = StringFromInt(p->nBranchHeight);
	//	int nItemHeight = p->nItemHeight;
	//	if (nItemHeight == 0)
	//	{
	//		nItemHeight = GetItemHeight();
	//	}
	//	strText += _T("--");
	//	strText += HexStringFromInt((int)hItem, FALSE, FALSE);
	//	strText += _T("--");
	//	strText += StringFromInt(nItemHeight);
	//	strText += _T("--");
	//	strText += strBranchHeight;
	//	strText += _T("--");
	//	strText += StringFromInt(hItem->iShownIndex);
	//#endif
	pDC->DrawText(strText, rcNew, uFormat);

	SelectObject(pDC->GetSafeHdc(), hFontOld);

	return 0;
}

int CSkinTreeCtrl::UseCustomScrollBar(BOOL bUseCustomScrollBar)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_bUseCustomScrollBar = bUseCustomScrollBar;

	if (pMember->m_bUseCustomScrollBar)
	{
		BOOL bRet = pMember->m_wndHScroll.Create(SBS_HORZ | WS_CHILD, CRect(0, 0, 0, 0), GetParent(), 1);
		if (!bRet)
		{
			return -1;
		}
		pMember->m_wndHScroll.SetNotifyWnd(this);
		bRet = pMember->m_wndVScroll.Create(SBS_VERT | WS_CHILD, CRect(0, 0, 0, 0), GetParent(), 2);
		if (!bRet)
		{
			return -1;
		}
		pMember->m_wndVScroll.SetNotifyWnd(this);
	}

	return 0;
}

BOOL CSkinTreeCtrl::IsUseCustomScrollBar() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_bUseCustomScrollBar;
}

int CSkinTreeCtrl::SetScrollBarWidth(LONG lScrollBarWidth)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_lScrollBarWidth = lScrollBarWidth;

	return 0;
}

LONG CSkinTreeCtrl::GetScrollBarWidth() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_lScrollBarWidth;
}

CScrollBarEx *CSkinTreeCtrl::GetHorzScrollBarEx() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (pMember->m_bUseCustomScrollBar)
	{
		return &pMember->m_wndHScroll;
	}

	return NULL;
}

CScrollBarEx *CSkinTreeCtrl::GetVertScrollBarEx() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (pMember->m_bUseCustomScrollBar)
	{
		return &pMember->m_wndVScroll;
	}

	return NULL;
}

int CSkinTreeCtrl::UpdateCustomScrollBar()
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (!pMember->m_bUseCustomScrollBar)
	{
		return -1;
	}

	BOOL bRet = FALSE;
	long lVScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
	long lHScrollBarHeight = GetSystemMetrics(SM_CYHSCROLL);
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIPointX(lVScrollBarWidth);
		GetDPIPointY(lHScrollBarHeight);
	}
	// Ver
	SCROLLINFO siVert;
	memset(&siVert, 0, sizeof(SCROLLINFO));
	siVert.cbSize = sizeof(SCROLLINFO);
	siVert.fMask = SIF_ALL;
	bRet = GetScrollInfo(SB_VERT, &siVert);

	BOOL bShowVScroll = FALSE;
	if (int(siVert.nPage) < (siVert.nMax + 1) && bRet && siVert.nPage != 0)
	{
		bShowVScroll = TRUE;
	}

	// Horz
	SCROLLINFO siHorz;
	memset(&siHorz, 0, sizeof(SCROLLINFO));
	siHorz.cbSize = sizeof(SCROLLINFO);
	siHorz.fMask = SIF_ALL;
	bRet = GetScrollInfo(SB_HORZ, &siHorz);

	BOOL bShowHScroll = FALSE;
	if (int(siHorz.nPage) < (siHorz.nMax + 1) && bRet && siHorz.nPage != 0)
	{
		bShowHScroll = TRUE;
	}

	// Show scroll bar
	BOOL bNotifyWndShow = FALSE;
	LONG lStyle = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
	if ((lStyle & WS_VISIBLE) == WS_VISIBLE)
	{
		bNotifyWndShow = TRUE;
	}

	if (bShowVScroll && bShowHScroll)
	{
		CRect rect;
		GetWindowRect(rect);

		CRgn rgnV;
		rgnV.CreateRectRgn(rect.Width() - lVScrollBarWidth, 0, rect.Width(), rect.Height() - lHScrollBarHeight);
		CRgn rgnH;
		rgnH.CreateRectRgn(0, rect.Height() - lHScrollBarHeight, rect.Width() - lVScrollBarWidth, rect.Height());
		CRgn rgnAll;
		rgnAll.CreateRectRgn(0, 0, rect.Width(), rect.Height());
		rgnAll.CombineRgn(&rgnAll, &rgnV, RGN_DIFF);
		rgnAll.CombineRgn(&rgnAll, &rgnH, RGN_DIFF);
		SetWindowRgn(HRGN(rgnAll.GetSafeHandle()), TRUE);

		GetParent()->ScreenToClient(rect);
		pMember->m_wndVScroll.MoveWindow(rect.right - lVScrollBarWidth, rect.top, pMember->m_lScrollBarWidth, rect.Height() - lHScrollBarHeight);
		if (bNotifyWndShow)
		{
			pMember->m_wndVScroll.ShowWindow(SW_SHOW);
		}

		pMember->m_wndHScroll.MoveWindow(rect.left, rect.bottom - lHScrollBarHeight, rect.Width() - lVScrollBarWidth, pMember->m_lScrollBarWidth);
		if (bNotifyWndShow)
		{
			pMember->m_wndHScroll.ShowWindow(SW_SHOW);
		}
	}
	else if (bShowVScroll)
	{
		CRect rect;
		GetWindowRect(rect);

		CRgn rgnV;
		rgnV.CreateRectRgn(rect.Width() - lVScrollBarWidth, 0, rect.Width(), rect.Height());
		CRgn rgnAll;
		rgnAll.CreateRectRgn(0, 0, rect.Width(), rect.Height());
		rgnAll.CombineRgn(&rgnAll, &rgnV, RGN_DIFF);
		SetWindowRgn(HRGN(rgnAll.GetSafeHandle()), TRUE);

		GetParent()->ScreenToClient(rect);
		pMember->m_wndVScroll.MoveWindow(rect.right - lVScrollBarWidth, rect.top, pMember->m_lScrollBarWidth, rect.Height());
		if (bNotifyWndShow)
		{
			pMember->m_wndVScroll.ShowWindow(SW_SHOW);
		}
		pMember->m_wndHScroll.ShowWindow(SW_HIDE);
	}
	else if (bShowHScroll)
	{
		CRect rect;
		GetWindowRect(rect);

		CRgn rgnH;
		rgnH.CreateRectRgn(0, rect.Height() - lHScrollBarHeight, rect.Width(), rect.Height());
		CRgn rgnAll;
		rgnAll.CreateRectRgn(0, 0, rect.Width(), rect.Height());
		rgnAll.CombineRgn(&rgnAll, &rgnH, RGN_DIFF);
		SetWindowRgn(HRGN(rgnAll.GetSafeHandle()), TRUE);

		GetParent()->ScreenToClient(rect);
		pMember->m_wndHScroll.MoveWindow(rect.left, rect.bottom - lHScrollBarHeight, rect.Width(), pMember->m_lScrollBarWidth);
		if (bNotifyWndShow)
		{
			pMember->m_wndHScroll.ShowWindow(SW_SHOW);
		}
		pMember->m_wndVScroll.ShowWindow(SW_HIDE);
	}
	else
	{
		pMember->m_wndVScroll.ShowWindow(SW_HIDE);
		pMember->m_wndHScroll.ShowWindow(SW_HIDE);

		CRect rect;
		GetWindowRect(rect);

		CRgn rgnAll;
		rgnAll.CreateRectRgn(0, 0, rect.Width(), rect.Height());
		SetWindowRgn(HRGN(rgnAll.GetSafeHandle()), TRUE);
	}

	pMember->m_wndHScroll.SetScrollInfo(&siHorz);
	pMember->m_wndVScroll.SetScrollInfo(&siVert);

	return 0;
}

int CSkinTreeCtrl::SetHoverScrollBarMode(BOOL bHoverShow)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	pMember->m_bHoverScrollBarMode = bHoverShow ? true : false;

	return 0;
}

BOOL CSkinTreeCtrl::IsHoverScrollBarMode() const
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	return pMember->m_bHoverScrollBarMode;
}

int CSkinTreeCtrl::AnimateContent(std::vector<int> &vAnimationData, int nElapse/*ms*/, BOOL bTogetherAnimate)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (vAnimationData.size() == 0)
	{
		return -1;
	}

	pMember->m_bAnimationMode = true;
	pMember->m_bTogetherAnimation = bTogetherAnimate;
	pMember->m_nCurTimerTick = 0;
	pMember->m_vAnimationData = vAnimationData;

	SetTimer(IDTT_ANIMATION_CONTENT, nElapse, NULL);

	return 0;
}

HTREEITEM CSkinTreeCtrl::AnimateInsertItem(LPCTSTR lpszItem, int nItemHeight,
	DWORD dwItemData, std::vector<DELETE_TREE_ITEM_ANIMATION> &vAnimationData,
	int nElapse/*ms*/, HTREEITEM hParent/* = TVI_ROOT*/, HTREEITEM hInsertAfter/* = TVI_LAST*/)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (vAnimationData.size() == 0)
	{
		return NULL;
	}

	pMember->m_bAnimationInsertItem = true;
	pMember->m_hInsertItem = InsertItem(lpszItem, hParent, hInsertAfter);
	if (pMember->m_hInsertItem == NULL)
	{
		pMember->m_bAnimationInsertItem = false;
		return NULL;
	}
	EnsureVisible(pMember->m_hInsertItem);

	SetItemHeightEx(pMember->m_hInsertItem, nItemHeight);
	SetItemData(pMember->m_hInsertItem, dwItemData);

	pMember->m_vInsertItemAnimationData = vAnimationData;
	SetTimer(IDTT_INSERT_ITEM, nElapse, NULL);

	return pMember->m_hInsertItem;
}

// Not delete item.
int CSkinTreeCtrl::AnimateRemoveItem(HTREEITEM hItem, std::vector<DELETE_TREE_ITEM_ANIMATION> &vAnimationData, int nElapse/*ms*/)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (vAnimationData.size() == 0)
	{
		return -1;
	}

	pMember->m_bAnimateRemoveMode = true;
	pMember->m_hRemoveItem = hItem;
	pMember->m_vRemoveItemAnimationData = vAnimationData;
	pMember->m_nCurRemoveItemTimerTick = 0;

	SetTimer(IDTT_REMOVE_ITEM, nElapse, NULL);

	return 0;
}

int CSkinTreeCtrl::AnimateDeleteItem(HTREEITEM hItem, std::vector<int> &vAnimationData, int nElapse/*ms*/)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (vAnimationData.size() == 0)
	{
		return -1;
	}

	int nHeight = GetItemHeightEx(hItem);
	if (nHeight == -1)
	{
		return -2;
	}

	pMember->m_hDeletedItem = hItem;
	pMember->m_vDeletedItemAnimationData = vAnimationData;
	pMember->m_nCurDeleteItemTimerTick = 0;

	SetTimer(IDTT_DELETE_ITEM, nElapse, NULL);

	return 0;
}

int CSkinTreeCtrl::AnimateVerticalScroll(std::vector<int> &vAnimationData, int nElapse/*ms*/)
{
	TREEMEMBER *pMember = (TREEMEMBER *)m_pMember;

	if (vAnimationData.size() == 0)
	{
		return -1;
	}

	SCROLLINFO siv = {0};
	siv.cbSize = sizeof(SCROLLINFO);
	siv.fMask = SIF_ALL;
	GetScrollInfo(SB_VERT, &siv);

	if (siv.nPos + (int)siv.nPage >= siv.nMax - siv.nMin)
	{
		return -2;    // Already scroll to end.
	}

	pMember->m_nCurVerScrollTimerTick = 0;
	pMember->m_vVerScrollAnimationData = vAnimationData;

	SetTimer(IDTT_VER_SCROLL, nElapse, NULL);

	return 0;
}
