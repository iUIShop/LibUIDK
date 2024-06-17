// SkinListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define IDT_WIN8				123


struct LV_ITEMDATA
{
	LV_ITEMDATA()
	{
		lParam = 0;
	}

	LPARAM lParam;						// The user Item Data.
	std::map<int, SItemBindObject *> mapSubItemObject;		// first: 0-base sub item index; second: the bind object of the sub item.
};

struct LV_SORTDATA
{
	CSkinListCtrl *pThis;
	PFNLVCOMPARE pfnUserCompare;
	DWORD dwUserData;
};

struct LV_ITEMPROPERTIES
{
	LV_ITEMPROPERTIES()
	{
		m_eBkType = BKT_COLOR;
		m_crItemBk = RGB(255, 255, 255);
		m_hItemBkBmp = NULL;
		m_ptResize.x = m_ptResize.y = 2;
		m_crItemText = RGB(0, 0, 0);
		m_hIUIFont = NULL;
	}
	~LV_ITEMPROPERTIES()
	{
		ReleaseIUIImage(m_hItemBkBmp);
		m_hItemBkBmp = NULL;

		if (NULL != m_hIUIFont)
		{
			ReleaseIUIFontInternal(m_hIUIFont);
			m_hIUIFont = NULL;
		}
	}

	// Background
	BACKGROUND_TYPE m_eBkType;
	COLORREF m_crItemBk;		// If the item background image is valid, the background color is ignored.
	HIUIIMAGE m_hItemBkBmp;
	POINT m_ptResize;

	// Text
	COLORREF m_crItemText;
	HIUIFONT m_hIUIFont;
};

struct LISTCTRLMEMBER
{
	LISTCTRLMEMBER()
	{
		m_bCacheMode = FALSE;

		m_bListFirstShowed = FALSE;
		m_hLevel1ResizedBkImage = NULL;
		m_hLevel2BlendBkTextImage = NULL;
		m_hLevel3Background = NULL;
		m_hLevel4ImageList = NULL;
		m_hLevel5ImageList = NULL;
		m_sizeImageInImageList = m_sizeImageInSubItemImageList = CSize(0, 0);

		m_bAnimationMode = false;
		m_bTogetherAnimation = FALSE;
		m_nCurTimerTick = 0;
		m_nTimerElapse = 40;

		m_hImageList = NULL;
		m_bSetItemDataInternal = FALSE;
		m_bInsertingItemSetData = FALSE;
		m_bGetItemRectInternal = FALSE;

		m_nEditingItem = -1;

		// Background
		m_bUseBitmap = TRUE;
		int i = 0;
		for (i = 0; i < 2; ++i)
		{
			m_himgBk[i] = NULL;
		}

		m_pRectCtrlBind = NULL;

		// Grid line
		m_bDraw0Row = FALSE;
		m_bVerLineTo0Row = FALSE;
		m_ptLineOffset = CPoint(0, 0);
		m_crGridLine = RGB(212, 208, 200);

		// Normal item
		m_bEnableNormalItemEffect = FALSE;
		m_lvpItemN.m_crItemBk = RGB(255, 255, 255);
		m_lvpItemN.m_crItemText = RGB(0, 0, 0);

		// Highlight Item
		m_bEnableHighlightItemEffect = FALSE;
		m_nCurHighlightItem = -1;
		m_lvpItemH.m_crItemBk = RGB(241, 241, 241);
		m_lvpItemH.m_crItemText = RGB(0, 0, 0);

		// Selected Item
		m_bEnableSelectedFitClient = FALSE;
		m_lvpItemS.m_crItemBk = RGB(200, 200, 200);
		m_lvpItemS.m_crItemText = RGB(255, 255, 255);

		// Normal Disabled Item
		m_bEnableNormalDisabledItemEffect = TRUE;
		m_lvpItemND.m_crItemBk = RGB(255, 255, 255);
		m_lvpItemND.m_crItemText = RGB(128, 128, 128);

		// Selected Disabled Item
		m_bEnableSelectedDisabledItemEffect = TRUE;
		m_lvpItemSD.m_crItemBk = RGB(212, 208, 200);
		m_lvpItemSD.m_crItemText = RGB(0, 0, 0);

		// Thumbnail view
		m_bThumbnailView = FALSE;

		// Thumbnail item position
		for (i = 0; i < 4 ; ++i)
		{
			m_rcThumbnailBkPosition[i] = CRect(10, 10, 110, 130);
			m_rcThumbnailPosition[i] = CRect(20, 20, 100, 100);
			m_rcThumbnailLabelPosition[i] = CRect(10, 110, 130, 130);
			m_rcThumbnailIconPosition[i] = CRect(20, 20, 100, 100);

		}

		// Header
		m_bHeaderTrack = TRUE;

		// Custom scroll bar
		m_bUseCustomScrollBar = FALSE;
	}

	int Release()
	{
		m_bCacheMode = FALSE;

		m_bListFirstShowed = FALSE;
		ReleaseIUIImage(m_hLevel1ResizedBkImage);
		m_hLevel1ResizedBkImage = NULL;
		SafeDeleteHBITMAP(m_hLevel2BlendBkTextImage);
		SafeDeleteHBITMAP(m_hLevel3Background);

		SafeDeleteHBITMAP(m_hLevel4ImageList);
		SafeDeleteHBITMAP(m_hLevel5ImageList);
		m_sizeImageInImageList = m_sizeImageInSubItemImageList = CSize(0, 0);

		m_bAnimationMode = false;
		m_bTogetherAnimation = FALSE;
		m_nCurTimerTick = 0;
		m_nTimerElapse = 40;

		m_hImageList = NULL;
		m_bSetItemDataInternal = FALSE;
		m_bInsertingItemSetData = FALSE;
		m_bGetItemRectInternal = FALSE;

		m_nEditingItem = -1;

		// Background
		m_bUseBitmap = TRUE;
		int i = 0;
		for (i = 0; i < 2; ++i)
		{
			ReleaseIUIImage(m_himgBk[i]);
			m_himgBk[i] = NULL;
		}

		m_pRectCtrlBind = NULL;

		// Grid line
		m_bDraw0Row = FALSE;
		m_bVerLineTo0Row = FALSE;
		m_ptLineOffset = CPoint(0, 0);
		m_crGridLine = RGB(212, 208, 200);

		// Normal item
		m_bEnableNormalItemEffect = FALSE;
		m_lvpItemN.m_crItemBk = RGB(255, 255, 255);
		m_lvpItemN.m_crItemText = RGB(0, 0, 0);

		// Highlight Item
		m_bEnableHighlightItemEffect = FALSE;
		m_nCurHighlightItem = -1;
		m_lvpItemH.m_crItemBk = RGB(241, 241, 241);
		m_lvpItemH.m_crItemText = RGB(0, 0, 0);

		// Selected Item
		m_bEnableSelectedFitClient = FALSE;
		m_lvpItemS.m_crItemBk = RGB(200, 200, 200);
		m_lvpItemS.m_crItemText = RGB(255, 255, 255);

		// Normal Disabled Item
		m_bEnableNormalDisabledItemEffect = TRUE;
		m_lvpItemND.m_crItemBk = RGB(255, 255, 255);
		m_lvpItemND.m_crItemText = RGB(128, 128, 128);

		// Selected Disabled Item
		m_bEnableSelectedDisabledItemEffect = TRUE;
		m_lvpItemSD.m_crItemBk = RGB(212, 208, 200);
		m_lvpItemSD.m_crItemText = RGB(0, 0, 0);

		// Thumbnail view
		m_bThumbnailView = FALSE;

		// Thumbnail item position
		for (i = 0; i < 4 ; ++i)
		{
			m_rcThumbnailBkPosition[i] = CRect(10, 10, 110, 130);
			m_rcThumbnailPosition[i] = CRect(20, 20, 100, 100);
			m_rcThumbnailLabelPosition[i] = CRect(10, 110, 130, 130);
			m_rcThumbnailIconPosition[i] = CRect(20, 20, 100, 100);

		}

		// Header
		m_bHeaderTrack = TRUE;

		// Custom scroll bar
		m_bUseCustomScrollBar = FALSE;

		return 0;
	}

	BOOL m_bCacheMode;						// True: use cache mode to draw list

	// The following variable for cache mode
	BOOL m_bListFirstShowed;				// Set to TRUE after the list first call WM_PAINT.
	HIUIIMAGE m_hLevel1ResizedBkImage;		// Level-1 cache, Generate by resizing m_himgBk with 9 grid method. the size is equal to non-client size
	HBITMAP m_hLevel2BlendBkTextImage;		// Level-2 cache, Blend m_hLevel1ResizedBkImage and text(not include highlight state text). the cache can show directly if m_bEnableHighlightItemEffect is FALSE.
	HBITMAP m_hLevel3Background;			// Level-3 cache, Base level-2, update partial content, such as highlight item content, Level-3 can show directly.
	HBITMAP m_hLevel4ImageList;				// Level-4 cache, store the image of first column in image list
	CSize m_sizeImageInImageList;
	HBITMAP m_hLevel5ImageList;				// Level-5 cache, store the image of sub item column in image list
	CSize m_sizeImageInSubItemImageList;

	// The following variable for animating text. like windows 8.
	bool m_bAnimationMode;
	BOOL m_bTogetherAnimation;				// The items together move or not.
	int m_nCurTimerTick;					// Times that timer implement. same as Frame.
	int m_nTimerElapse;						// See SetTimer.
	std::vector<int> m_vAnimationData;

	// for user-define string
	CString m_strTag;

	// for image in other columns
	HIMAGELIST m_hImageList;	// Don't use CImageList *, sine the the pointer can be delete.

	BOOL m_bSetItemDataInternal;
	// Insert Item call SetItemData, SetItemData may be trigger LVN_ITEMCHANGED,
	// If call GetItemData in LVN_ITEMCHANGED notify,
	// the return value may wrong if use m_bSetItemDataInside flag, So, define m_bInsertingItemSetData.
	BOOL m_bInsertingItemSetData;
	BOOL m_bGetItemRectInternal;

	int m_nEditingItem;

	// for sort item
	LV_SORTDATA m_LibUIDKSortData;

	// Background
	BOOL m_bUseBitmap;
	HIUIIMAGE m_himgBk[2];				// 0: Enabled; 1: Disabled.

	// resize
	CRectCtrl *m_pRectCtrlBind;

	// for grid line
	BOOL m_bDraw0Row;
	BOOL m_bVerLineTo0Row; // TRUE: to 0 row; FALSE: to header bottom.
	CPoint m_ptLineOffset;
	COLORREF m_crGridLine;

	// for normal item
	BOOL m_bEnableNormalItemEffect;
	LV_ITEMPROPERTIES m_lvpItemN;

	// for highlight item
	BOOL m_bEnableHighlightItemEffect;
	int m_nCurHighlightItem;
	LV_ITEMPROPERTIES m_lvpItemH;

	// for selected item
	BOOL m_bEnableSelectedFitClient;	// the selected background fit all client width while item is selected.
	LV_ITEMPROPERTIES m_lvpItemS;

	// for normal disabled item
	BOOL m_bEnableNormalDisabledItemEffect;
	LV_ITEMPROPERTIES m_lvpItemND;

	// for select disabled item
	BOOL m_bEnableSelectedDisabledItemEffect;
	LV_ITEMPROPERTIES m_lvpItemSD;

	// for thumbnail view
	BOOL m_bThumbnailView;

	CRect m_rcThumbnailBkPosition[4];
	CRect m_rcThumbnailPosition[4];
	CRect m_rcThumbnailLabelPosition[4];
	CRect m_rcThumbnailIconPosition[4];

	// Report header
	CSkinHeaderCtrl m_wndHeader;
	BOOL m_bHeaderTrack;

	// for custom scroll bar
	BOOL m_bUseCustomScrollBar;
	CScrollBarEx m_wndHScroll;
	CScrollBarEx m_wndVScroll;
};

// only update scroll information of custom scroll bar,
// if you want to update both scroll information and position,
// please call CSkinListCtrl::UpdateCustomScrollBar().
int UpdateCustomScrollInfo(CSkinListCtrl *pList)
{
	BOOL bRet = FALSE;

	// Horz
	SCROLLINFO siHorz;
	memset(&siHorz, 0, sizeof(SCROLLINFO));
	siHorz.cbSize = sizeof(SCROLLINFO);
	siHorz.fMask = SIF_ALL;
	bRet = pList->GetScrollInfo(SB_HORZ, &siHorz);

	pList->GetHorzScrollBarEx()->SetScrollInfo(&siHorz);

	// Ver
	SCROLLINFO siVert;
	memset(&siVert, 0, sizeof(SCROLLINFO));
	siVert.cbSize = sizeof(SCROLLINFO);
	siVert.fMask = SIF_ALL;
	bRet = pList->GetScrollInfo(SB_VERT, &siVert);

	pList->GetVertScrollBarEx()->SetScrollInfo(&siVert);

	return 0;
}

int CALLBACK _LibUIDK_CSkinListCtrl_Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lData)
{
	LV_SORTDATA *pData = (LV_SORTDATA *)lData;

	CSkinListCtrl *pThis = pData->pThis;
	DWORD dwUserData = pData->dwUserData;
	PFNLVCOMPARE pUserFunc = pData->pfnUserCompare;

	LV_ITEMDATA *pData1 = (LV_ITEMDATA *)lParam1;
	LV_ITEMDATA *pData2 = (LV_ITEMDATA *)lParam2;

	return pUserFunc(pData1->lParam, pData2->lParam, dwUserData);
}

int _LibUIDK_CSkinListCtrl_UpdateLevel1(CSkinListCtrl *pListCtrl, BOOL bListFirstShowed,
	HIUIIMAGE hBkN, HIUIIMAGE hBkD,
	IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	int nDestWidth, int nDestHeight,
	__out HIUIIMAGE *phLevel1ResizedBkImage)
{
	if (!bListFirstShowed || phLevel1ResizedBkImage == NULL)
	{
		return -1;
	}

	ReleaseIUIImage(*phLevel1ResizedBkImage);

	HIUIIMAGE himgBk = NULL;
	if (pListCtrl->IsWindowEnabled())
	{
		himgBk = hBkN;
	}
	else
	{
		himgBk = hBkD;
	}

	if (himgBk != NULL)
	{
		CIUIImage::NineGridBitmap(himgBk, pt9GridResize, l9GridRepeatX, l9GridRepeatY,
			phLevel1ResizedBkImage, nDestWidth, nDestHeight);
	}

	return 0;
}

int _LibUIDK_CSkinListCtrl_UpdateLevel2(CSkinListCtrl *pListCtrl, BOOL bListFirstShowed,
	__out HBITMAP &hLevel2BlendBkTextImage)
{
	if (!bListFirstShowed)
	{
		return -1;
	}

	//
	// Recreate the m_hLevel2BlendBkTextImage
	//
	SafeDeleteHBITMAP(hLevel2BlendBkTextImage);

	CClientDC dc(pListCtrl);

	CRect rcClient;
	pListCtrl->GetClientRect(rcClient);

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
	pListCtrl->SendMessage(LVM_DRAW_BACKGROUND, (WPARAM)&memBkDC, 0);

	//
	// Draw content
	//

	// Draw grid line of list with report style.
	pListCtrl->SendMessage(LVM_DRAW_GRIDLINE, (WPARAM)&memBkDC, 0);

	// draw all of the items that are completely visible
	int nItem = pListCtrl->GetTopIndex();
	int nLast = nItem + pListCtrl->GetCountPerPage() + 1;
	int nCount = pListCtrl->GetItemCount();
	if (nLast > nCount)
	{
		nLast = nCount;
	}

	memBkDC.SetBkMode(TRANSPARENT);

	for (; nItem < nLast; nItem++)
	{
		pListCtrl->SendMessage(LVM_DRAW_ITEM, (WPARAM)&memBkDC, nItem);
	}

	//
	// hLevel2BlendBkTextImage is OK.
	//
	::SelectObject(memBkDC.GetSafeHdc(), hOldBmpBk);

	return 0;
}

int _LibUIDK_CSkinListCtrl_UpdateLevel3(CSkinListCtrl *pListCtrl, BOOL bListFirstShowed, HBITMAP hLevel2BlendBkTextImage, __out HBITMAP &hLevel3Background, int nItemNewHighlight, int nItemOldHighlight)
{
	if (!bListFirstShowed)
	{
		return -1;
	}

	//
	// Recreate the m_hLevel3Background
	//
	SafeDeleteHBITMAP(hLevel3Background);

	CClientDC dc(pListCtrl);

	CRect rcClient;
	pListCtrl->GetClientRect(rcClient);

	hLevel3Background = ::CreateCompatibleBitmap(dc.GetSafeHdc(), rcClient.Width(), rcClient.Height());

	//
	// Select m_hLevel3Background to DC
	//
	CDC memBkDC;
	memBkDC.CreateCompatibleDC(&dc);
	HBITMAP hOldBmpBk = (HBITMAP)::SelectObject(memBkDC.GetSafeHdc(), hLevel3Background);

	BitBltG(memBkDC.GetSafeHdc(), 0, 0, rcClient.Width(), rcClient.Height(),  hLevel2BlendBkTextImage, 0, 0, SRCCOPY);

	//
	// Draw changed content(highlight item)
	//
	// 		// Draw grid line of list with report style.
	// 		pListCtrl->SendMessage(LVM_DRAW_GRIDLINE, (WPARAM)&memBkDC, 0);

	// draw all of the items that are completely visible

	memBkDC.SetBkMode(TRANSPARENT);
	if (nItemNewHighlight >= 0)
	{
		pListCtrl->SendMessage(LVM_DRAW_ITEM, (WPARAM)&memBkDC, nItemNewHighlight);
	}
	if (nItemOldHighlight >= 0)
	{
		pListCtrl->SendMessage(LVM_DRAW_ITEM, (WPARAM)&memBkDC, nItemOldHighlight);
	}

	//
	// m_hLevel3Background is OK.
	//
	::SelectObject(memBkDC.GetSafeHdc(), hOldBmpBk);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CSkinListCtrl

CSkinListCtrl::CSkinListCtrl()
{
	m_pBindWnd = this;

	m_pMember = new LISTCTRLMEMBER;
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;
}

CSkinListCtrl::~CSkinListCtrl()
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	ReleaseObject();

	if (m_pMember != NULL)
	{
		delete (LISTCTRLMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

BOOL CSkinListCtrl::Create(__in DWORD dwStyle, __in const RECT &rect, __in CWnd *pParentWnd, __in UINT nID)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return CListCtrl::Create(dwStyle, rect, pParentWnd, nID);
}

BOOL CSkinListCtrl::Create(DWORD dwStyle, CRectCtrl *pRectCtrl, CWnd *pParentWnd, UINT nID)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	ASSERT(pRectCtrl != NULL && pRectCtrl->IsCreated());

	CRect rect;
	pRectCtrl->GetWindowRect(rect);

	BOOL bRet = CListCtrl::Create(dwStyle, rect, pParentWnd, nID);
	if (!bRet)
	{
		return FALSE;
	}

	pRectCtrl->AddBindWindow(this);
	pMember->m_pRectCtrlBind = pRectCtrl;

	return TRUE;
}

int CSkinListCtrl::BindStyle(LPCTSTR lpszStyleID)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (lpszStyleID == NULL)
	{
		return -2;
	}

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_LISTCTRL, lpszStyleID);
	return BindStyle(pCtrlProp);

	return 0;
}

int CSkinListCtrl::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

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
	if (ect != CT_STYLE_LISTCTRL && ect != CT_LISTCTRL)
	{
		return -3;
	}

	LSTCTRLPROPERTIES *pLstProp = (LSTCTRLPROPERTIES *)pCtrlProp;

	DWORD dwExStyle = GetExtendedStyle();
	if (pLstProp->m_bCheckBoxes)
	{
		dwExStyle |= LVS_EX_CHECKBOXES;
	}
	if (pLstProp->m_bFullRowSelect)
	{
		dwExStyle |= LVS_EX_FULLROWSELECT;
	}
	if (pLstProp->m_bHeaderDragDrop)
	{
		dwExStyle |= LVS_EX_HEADERDRAGDROP;
	}
	if (pLstProp->m_bInfoTooltip)
	{
		dwExStyle |= LVS_EX_INFOTIP;
	}
	if (pLstProp->m_bSubItemImages)
	{
		dwExStyle |= LVS_EX_SUBITEMIMAGES;
	}
	if (pLstProp->m_bTrackSelect)
	{
		dwExStyle |= LVS_EX_TRACKSELECT;
	}
	if (pLstProp->m_bTwoClickActivate)
	{
		dwExStyle |= LVS_EX_TWOCLICKACTIVATE;
	}

	LST_STYLE eListStyle = (LST_STYLE)pLstProp->m_eListStyle;
	if (eListStyle == LSTS_THUMBNAIL)
	{
		dwExStyle |= LVS_EX_JUSTIFYCOLUMNS;
		SetThumbnailView(TRUE);
	}

	SetExtendedStyle(dwExStyle);

	// Set background images
	if (pLstProp->m_bSpecifyBackgroundImages)
	{
		CString strImageName[1 + COMBINEIMAGESIZE2];
		BOOL bCombineImage = TRUE;
		CTRLPROPERTIES::IUIGetBackground2(pCtrlProp, &bCombineImage, strImageName);
		if (bCombineImage)
		{
			SetBkCombineImage(strImageName[0]);
		}
		else
		{
			SetImages(CONTROL_STATE_UNCHECKED_ND, strImageName[1], strImageName[2]);
		}

		SetBkImageResizeMode(pCtrlProp->m_eBkImageResizeMode);
		SetBkImageResizePoint(pCtrlProp->m_ptImageResize);
	}

	// Set Item Font
	CString strFont[5];
	int nIndex = 0;
	strFont[nIndex++] = (LPCTSTR)pLstProp->m_strResFontIDN;

	if (pLstProp->m_bHSameNFont)
	{
		strFont[nIndex++] = (LPCTSTR)pLstProp->m_strResFontIDN;
	}
	else
	{
		strFont[nIndex++] = (LPCTSTR)pLstProp->m_strResFontIDH;
	}

	if (pLstProp->m_bSSameNFont)
	{
		strFont[nIndex++] = (LPCTSTR)pLstProp->m_strResFontIDN;
	}
	else
	{
		strFont[nIndex++] = (LPCTSTR)pLstProp->m_strResFontIDS;
	}

	if (pLstProp->m_bDSameNFont)
	{
		strFont[nIndex++] = (LPCTSTR)pLstProp->m_strResFontIDN;
	}
	else
	{
		strFont[nIndex++] = (LPCTSTR)pLstProp->m_strResFontIDD;
	}

	if (pLstProp->m_bSDSameNFont)
	{
		strFont[nIndex++] = (LPCTSTR)pLstProp->m_strResFontIDN;
	}
	else
	{
		strFont[nIndex++] = (LPCTSTR)pLstProp->m_strResFontIDSD;
	}

	SetItemTextFont(strFont[0], strFont[1], strFont[2], strFont[3], strFont[4]);

	// Set Header
	if (pMember->m_wndHeader.GetSafeHwnd() != NULL)
	{
		// Header height
		if (CUIMgr::IsEnableDPI())
		{
			long nHeight = pLstProp->m_nHeaderHeight;
			GetDPIPointY(nHeight);
			SetHeaderHeight(nHeight);
		}
		else
		{
			SetHeaderHeight(pLstProp->m_nHeaderHeight);
		}

		pMember->m_wndHeader.BindStyle((LPCTSTR)pLstProp->m_strHeaderStyleID);
	}

	EnableHeaderTrack(pLstProp->m_bHeaderTrack);

	SetTag(pLstProp->m_strTag);

	// Set item text color
	COLORREF cr[4] = {0};
	CTRLPROPERTIES::IUIGetControlColor4(pLstProp, &cr[0], &cr[1], &cr[2], &cr[3]);
	SetTextColor(cr[0]);
	SetNormalItemTextColor(cr[0]);
	SetHighlightItemTextColor(cr[1]);
	SetSelectedItemTextColor(cr[2]);
	SetNormalDisabledItemTextColor(cr[3]);
	COLORREF crSD;
	if (pLstProp->m_bSDSameNColor)
	{
		crSD = cr[0];
	}
	else
	{
		RESCOLORPROPERTIES *pColorPropSD = (RESCOLORPROPERTIES *)CUIMgr::GetColorResourceItem((LPCTSTR)pLstProp->m_strResColorIDSD);
		crSD = (pColorPropSD == NULL) ? cr[0] : (long)pColorPropSD->m_crColor;
	}
	SetSelectedDisabledItemTextColor(crSD);

	// Normal item
	EnableNormalItemEffect(pLstProp->m_bEnableNormalItemEffect);
	SetNormalItemBkType(pLstProp->m_eNormalItemBkType);
	if (BKT_COLOR == (BACKGROUND_TYPE)pLstProp->m_eNormalItemBkType)
	{
		COLORREF cr = RGB(255, 255, 255);
		CUIMgr::GetColorResource(pLstProp->m_strNormalItemBkColorResID, &cr);
		SetNormalItemBkColor(cr);
	}
	else if (BKT_IMAGE == (BACKGROUND_TYPE)pLstProp->m_eNormalItemBkType)
	{
		SetNormalItemBkImages(pLstProp->m_strNormalItemBkImage);

		pMember->m_lvpItemN.m_ptResize = pLstProp->m_ptNormalItemBkResize;
	}

	// Highlight Item
	EnableHighlightItemEffect(pLstProp->m_bEnableHighlightItemEffect);
	SetHighlightItemBkType(pLstProp->m_eHighlightItemBkType);
	if (BKT_COLOR == (BACKGROUND_TYPE)pLstProp->m_eHighlightItemBkType)
	{
		COLORREF cr = RGB(241, 241, 241);
		CUIMgr::GetColorResource(pLstProp->m_strHighlightItemBkColorResID, &cr);
		SetHighlightItemBkColor(cr, FALSE);
	}
	else if (BKT_IMAGE == (BACKGROUND_TYPE)pLstProp->m_eHighlightItemBkType)
	{
		SetHighlightItemBkImages(pLstProp->m_strHighlightItemBkImage);

		pMember->m_lvpItemH.m_ptResize = pLstProp->m_ptNormalItemBkResize;
	}

	// Selected Item
	EnableSelectedFitClient(pLstProp->m_bSelectedFitClient);
	SetSelectedItemBkType(pLstProp->m_eSelectedItemBkType);
	if (BKT_COLOR == (BACKGROUND_TYPE)pLstProp->m_eSelectedItemBkType)
	{
		COLORREF cr = RGB(0, 0, 128);
		CUIMgr::GetColorResource(pLstProp->m_strSelectedItemBkColorResID, &cr);
		SetSelectedItemBkColor(cr, FALSE);
	}
	else if (BKT_IMAGE == (BACKGROUND_TYPE)pLstProp->m_eSelectedItemBkType)
	{
		SetSelectedItemBkImages(pLstProp->m_strSelectedItemBkImage);

		pMember->m_lvpItemS.m_ptResize = pLstProp->m_ptNormalItemBkResize;
	}

	// Normal Disabled Item
	EnableNormalDisabledItemEffect(pLstProp->m_bEnableNormalDisabledItemEffect);
	SetNormalDisabledItemBkType(pLstProp->m_eNormalDisabledItemBkType);
	if (BKT_COLOR == (BACKGROUND_TYPE)pLstProp->m_eNormalDisabledItemBkType)
	{
		COLORREF cr = RGB(255, 255, 255);
		CUIMgr::GetColorResource(pLstProp->m_strNormalDisabledItemBkColorResID, &cr);
		SetNormalDisabledItemBkColor(cr);
	}
	else if (BKT_IMAGE == (BACKGROUND_TYPE)pLstProp->m_eNormalDisabledItemBkType)
	{
		SetNormalDisabledItemBkImages(pLstProp->m_strNormalDisabledItemBkImage);

		pMember->m_lvpItemN.m_ptResize = pLstProp->m_ptNormalItemBkResize;
	}

	// Selected Disabled Item
	EnableSelectedDisabledItemEffect(pLstProp->m_bEnableSelectedDisabledItemEffect);
	SetSelectedDisabledItemBkType(pLstProp->m_eSelectedDisabledItemBkType);
	if (BKT_COLOR == (BACKGROUND_TYPE)pLstProp->m_eSelectedDisabledItemBkType)
	{
		COLORREF cr = RGB(255, 255, 255);
		CUIMgr::GetColorResource(pLstProp->m_strSelectedDisabledItemBkColorResID, &cr);
		SetSelectedDisabledItemBkColor(cr);
	}
	else if (BKT_IMAGE == (BACKGROUND_TYPE)pLstProp->m_eSelectedDisabledItemBkType)
	{
		SetSelectedDisabledItemBkImages(pLstProp->m_strSelectedDisabledItemBkImage);

		pMember->m_lvpItemSD.m_ptResize = pLstProp->m_ptNormalItemBkResize;
	}

	// Grid line
	if (pLstProp->m_bShowGridLine)
	{
		ShowGridLine(TRUE);
		DrawFirstRow(pLstProp->m_bDraw0Row);
		GridVerLineToFirstRow(pLstProp->m_bVerLineTo0Row);
		POINT ptOffset;
		ptOffset.x = (LONG)pLstProp->m_lXOffset;
		ptOffset.y = 0;
		SetGridLineOffset(&ptOffset);
		SetGridLineColor(pLstProp->m_crGridLine);
	}

	// Thumbnail view
	if (IsThumbnailView())
	{
		SIZE sizeItem;
		sizeItem.cx = (LONG)pLstProp->m_lTotalItemWidth;
		sizeItem.cy = (LONG)pLstProp->m_lTotalItemHeight;
		SetThumbnailItemTotalSize(sizeItem);

		CRect rectN = pLstProp->m_rcThumbnailBkN;
		CRect rectH = pLstProp->m_rcThumbnailBkH;
		CRect rectS = pLstProp->m_rcThumbnailBkS;
		CRect rectD = pLstProp->m_rcThumbnailBkD;
		SetThumbnailItemBkPosition(rectN, rectH, rectS, rectD);
	}

	// Scroll bar
	bool bUseCustomScrollBar = pLstProp->m_bUseCustomScrollBar;
	UseCustomScrollBar(bUseCustomScrollBar);
	if (bUseCustomScrollBar)
	{
		// Horizontal scroll bar
		{
			CScrollBarEx *pHorzScroll = GetHorzScrollBarEx();
			pHorzScroll->BindStyle((LPCTSTR)pLstProp->m_strHorScrollbarID);
		}

		// Vertical scroll bar
		{
			CScrollBarEx *pVertScroll = GetVertScrollBarEx();
			pVertScroll->BindStyle((LPCTSTR)pLstProp->m_strVerScrollbarID);
		}
	}

	return 0;
}

int CSkinListCtrl::ReleaseObject()
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	// Release header image on CSkinListCtrl::WM_DESTROY
	pMember->Release();

	return CControlBase::ReleaseObject();
}

BEGIN_MESSAGE_MAP(CSkinListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSkinListCtrl)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnSetTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnSetTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinListCtrl message handlers

#if(_MSC_VER <= 1200) // VC6.0
int CSkinListCtrl::OnToolHitTest(CPoint point, TOOLINFO *pTI) const
#else
INT_PTR CSkinListCtrl::OnToolHitTest(CPoint point, TOOLINFO *pTI) const
#endif
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	LVHITTESTINFO info;
	info.pt = point;
	ListView_SubItemHitTest(m_hWnd, &info);
	CRect rcItem;
	pMember->m_bGetItemRectInternal = TRUE;
	GetItemRect(info.iItem, rcItem, LVIR_BOUNDS);
	pMember->m_bGetItemRectInternal = FALSE;

	if (info.iItem == -1)
	{
		return -1;
	}

	pTI->hwnd = m_hWnd;
	pTI->uId = MAKELPARAM(info.iItem + 1, info.iSubItem);
	pTI->lpszText = LPSTR_TEXTCALLBACK;
	pTI->rect = rcItem;

	return pTI->uId;

	//	return CListCtrl::OnToolHitTest(point, pTI);
}

BOOL CSkinListCtrl::OnSetTipText(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	TOOLTIPTEXTA *pTTTA = (TOOLTIPTEXTA *)pNMHDR;
	TOOLTIPTEXTW *pTTTW = (TOOLTIPTEXTW *)pNMHDR;
	CString strTipText;

#if(_MSC_VER <= 1200) // VC6.0
	UINT nID = pNMHDR->idFrom;
#else
	UINT_PTR nID = pNMHDR->idFrom;
#endif
	if (nID == 0)
	{
		return FALSE;
	}

	int nItem = LOWORD(nID) - 1;
	int nCol = HIWORD(nID);

	strTipText = GetItemText(nItem, nCol);
	GetParent()->SendMessage(WM_GETTOOLTIPS, WPARAM(&strTipText), LPARAM(this));

#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
	{
		lstrcpyn(pTTTA->szText, strTipText, 80);
	}
	else
	{
		_mbstowcsz(pTTTW->szText, strTipText, 80);
	}
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
	{
		_wcstombsz(pTTTA->szText, strTipText, 80);
	}
	else
	{
		lstrcpyn(pTTTW->szText, strTipText, 80);
	}
#endif
	*pResult = 0;

	return TRUE;
}

int CSkinListCtrl::OnDrawBackground(CDC *pDC)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	int nStatusIndex = 0;
	if (IsWindowEnabled())
	{
		nStatusIndex = 0;
	}
	else
	{
		nStatusIndex = 1;
	}

	if (pMember->m_himgBk[nStatusIndex] == NULL || pMember->m_himgBk[nStatusIndex]->IsHasAlphaChannel())
	{
		CRect rcWnd;
		GetWindowRect(rcWnd);
		GetParent()->ScreenToClient(rcWnd);
		// Draw the parent background to control for alpha blend with background of the control.
		BOOL bUpdateLayeredWindow = FALSE;
		DrawParentPart(this, pDC, rcWnd, &bUpdateLayeredWindow, NULL);
	}

	CRect rcClient;
	GetClientRect(rcClient);

	LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	BOOL bBorder = ((lStyle & WS_BORDER) == WS_BORDER);

	if (pMember->m_bUseBitmap)
	{
		if (pMember->m_bCacheMode)
		{
			IUIBitBlt(pDC->GetSafeHdc(),
				0, 0, rcClient.Width(), rcClient.Height(),
				pMember->m_hLevel1ResizedBkImage,
				bBorder ? 1 : 0,
				bBorder ? 1 : 0,
				SRCCOPY);
		}
		else
		{
			IUIDrawBackground(this, pDC, pMember->m_himgBk[nStatusIndex],
				m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
		}
	}
	else
	{
		COLORREF crBk = GetBkColor();
		pDC->FillSolidRect(rcClient, crBk);
	}

	return 0;
}

// 奇偶行不同背景色
int CSkinListCtrl::OnDrawItemInterlacedHighlight(CDC *pDC)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	CRect rcClient;
	GetClientRect(rcClient);

	LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	if ((lStyle & LVS_REPORT) != LVS_REPORT)
	{
		return 1;
	}

	int nIndex = 0;
	while (true)
	{
		CPoint pt;
		BOOL bOK = GetItemPosition(nIndex, &pt);
		if (!bOK)
		{
			break;
		}

		int nY = pt.y - 1;
		if (nY >= rcClient.Height())
		{
			break;
		}

		CPoint pt2;
		BOOL bOK2 = GetItemPosition(nIndex + 1, &pt2);
		if (!bOK2)
		{
			break;
		}

		CRect rcItem;
		rcItem.left = 0;
		rcItem.top = nY;
		rcItem.right = rcClient.right;
		rcItem.bottom = rcItem.top + (pt2.y - pt.y);

		pDC->FillSolidRect(rcItem, (nIndex % 2 == 0) ? RGB(255, 0, 0) : RGB(255, 255, 0));

		// Fixed bug: scroll the vertical scroll bot to bottom, the while loop can't break.
		if (nIndex > 0 && pt.y == 0)
		{
			break;
		}

		++nIndex;
	}

	return 0;
}

int CSkinListCtrl::OnDrawGridLine(CDC *pDC)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	CRect rcClient;
	GetClientRect(rcClient);

	LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	if ((lStyle & LVS_REPORT) != LVS_REPORT)
	{
		return 1;
	}

	BOOL bShow = IsGridLineVisible();
	if (!bShow)
	{
		return 2;
	}

	CPen penDefault;
	CPen *pPen = NULL;

	// Draw horizontal line
	HPEN hPenHor = NULL;
	int nRet = (int)GetParent()->SendMessage(WM_GETLISTGRIDLINEPEN, MAKEWPARAM(GetDlgCtrlID(), 1), (LPARAM)&hPenHor);	// A chance to modify the pen for user.
	if (nRet == 0)
	{
		if (hPenHor == NULL)
		{
			penDefault.CreatePen(PS_SOLID, 1, pMember->m_crGridLine);
			pPen = &penDefault;
		}
		else
		{
			pPen = CPen::FromHandle(hPenHor);
		}
		CPen *pPenOld = pDC->SelectObject(pPen);

		int nIndex = pMember->m_bDraw0Row ? 0 : 1;
		while (true)
		{
			CPoint pt;
			BOOL bOK = GetItemPosition(nIndex, &pt);
			if (!bOK)
			{
				break;
			}

			int nY = pt.y - 1;

			if (nY + pMember->m_ptLineOffset.y >= rcClient.Height())
			{
				break;
			}

			pDC->MoveTo(0, nY + pMember->m_ptLineOffset.y);
			pDC->LineTo(rcClient.right, nY + pMember->m_ptLineOffset.y);

			// Fixed bug: scroll the vertical scroll bot to bottom, the while loop can't break.
			if (nIndex > 0 && pt.y == 0)
			{
				break;
			}

			++nIndex;
		}
		pDC->SelectObject(pPenOld);
	}

	// Draw vertical line
	HPEN hPenVer = NULL;
	nRet = (int)GetParent()->SendMessage(WM_GETLISTGRIDLINEPEN, MAKEWPARAM(GetDlgCtrlID(), 2), (LPARAM)&hPenVer);
	if (nRet == 0)
	{
		if (hPenVer == NULL)
		{
			if (penDefault.GetSafeHandle() == NULL)
			{
				penDefault.CreatePen(PS_SOLID, 1, pMember->m_crGridLine);
			}

			pPen = &penDefault;
		}
		else
		{
			pPen = CPen::FromHandle(hPenVer);
		}
		CPen *pPenOld = pDC->SelectObject(pPen);

		CPoint pt;
		BOOL bOK = GetItemPosition(0, &pt);

		int nHeaderItemCount = pMember->m_wndHeader.GetItemCount();
		for (int nCloumn = 0; nCloumn < nHeaderItemCount; ++nCloumn)
		{
			CRect rcHeaderItem;
			pMember->m_wndHeader.GetItemRect(nCloumn, rcHeaderItem);
			pMember->m_wndHeader.ClientToScreen(rcHeaderItem);
			ScreenToClient(rcHeaderItem);

			pDC->MoveTo(rcHeaderItem.right + pMember->m_ptLineOffset.x, pMember->m_bVerLineTo0Row ? pt.y : rcHeaderItem.Height());
			pDC->LineTo(rcHeaderItem.right + pMember->m_ptLineOffset.x, rcClient.bottom);
		}

		pDC->SelectObject(pPenOld);
	}

	return 0;
}

int CSkinListCtrl::OnDrawItemEx(CDC *pDC, int nItem, int nLeftMargin/* = 0*/)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	CRect rcItem;
	pMember->m_bGetItemRectInternal = TRUE;
	GetItemRect(nItem, rcItem, LVIR_BOUNDS);
	pMember->m_bGetItemRectInternal = FALSE;

	LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	if ((lStyle & LVS_ICON) == LVS_ICON && pMember->m_bThumbnailView)
	{
		int nWidth = 0;
		int nHeight = 0;
		GetItemSpacing(FALSE, &nWidth, &nHeight);

		rcItem.left = rcItem.left + (rcItem.Width() - nWidth) / 2;
		rcItem.right = rcItem.left + nWidth;
		rcItem.bottom = rcItem.top + nHeight;
	}

	DRAWITEMSTRUCT dis;
	dis.CtlType = ODT_LISTVIEW;
	dis.CtlID = GetDlgCtrlID();
	dis.itemID = nItem;
	dis.hwndItem = m_hWnd;
	dis.hDC = pDC->GetSafeHdc();
	dis.rcItem = rcItem;
	pMember->m_bSetItemDataInternal = TRUE;
	dis.itemData = GetItemData(nItem);
	pMember->m_bSetItemDataInternal = FALSE;

	BOOL bParentDraw = (BOOL)GetParent()->SendMessage(WM_DRAWITEMEX, dis.CtlID, LPARAM(&dis));
	if (bParentDraw)
	{
		return 0;
	}

	// draw selected item.
	if ((lStyle & LVS_REPORT) == LVS_REPORT)
	{
		OnDrawReportItem(pDC, nItem, rcItem, nLeftMargin);
	}
	else if ((lStyle & LVS_ICON) == LVS_ICON)
	{
		if (pMember->m_bThumbnailView)
		{
			OnDrawThumbnailItem(pDC, nItem, rcItem);
		}
		else
		{
			OnDrawIconItem(pDC, nItem, rcItem);
		}
	}

	return 0;
}

class CCanvasPanel
{
public:
	CCanvasPanel();
	~CCanvasPanel();

public:
	virtual int MoveWindow(LPCRECT lprc);

protected:
	const UIWNDPROPERTIES *m_pBindUIWndProp;
	std::vector<CChildInfo *> m_vpWLChildren;
};

CCanvasPanel::CCanvasPanel()
{
	m_pBindUIWndProp = NULL;
}

CCanvasPanel::~CCanvasPanel()
{

}

int CCanvasPanel::MoveWindow(LPCRECT lprc)
{
	if (lprc == NULL)
	{
		return -1;
	}

	POINT ptWindowResize = m_pBindUIWndProp->m_bWindowResizePointSameAsImageResizePoint
		? m_pBindUIWndProp->m_ptImageResize
		: m_pBindUIWndProp->m_ptWindowResize;

	bool bKeepAspectRatio = m_pBindUIWndProp->m_bKeepChildrenAspectRatio;
	long lMaxHorRatio = m_pBindUIWndProp->m_lMaxHorRatio;
	long lMaxVerRatio = m_pBindUIWndProp->m_lMaxVerRatio;

	// resize child control
	size_t i = 0;
	size_t nChildCount = m_vpWLChildren.size();
	for (i = 0; i < nChildCount; ++i)
	{
		CONTROL_TYPE eControlType = m_vpWLChildren[i]->m_eControlType;

		CONTROL_LAYOUT_HOR eHorLayoutMode = m_vpWLChildren[i]->m_pCtrlProperties->m_eHorLayoutMode;
		CONTROL_LAYOUT_VER eVerLayoutMode = m_vpWLChildren[i]->m_pCtrlProperties->m_eVerLayoutMode;
		CRect rcParentMargin = m_vpWLChildren[i]->m_pCtrlProperties->m_rcLayoutMargin;
		if (CUIMgr::IsEnableDPI())
		{
			GetDPIRect(rcParentMargin);
		}

		if (eHorLayoutMode == CLH_FREE && eVerLayoutMode == CLV_FREE)
		{
			continue;
		}

		if (eControlType == CT_WL_RECTCTRL
			|| eControlType == CT_WL_SPLITTER
			|| eControlType == CT_WL_LINE
			|| eControlType == CT_WL_TEXT
			|| eControlType == CT_WL_BUTTON
			|| eControlType == CT_WL_CHECK
			|| eControlType == CT_WL_RADIO
			|| eControlType == CT_WL_SLIDER
			|| eControlType == CT_WL_PICTURE
			|| eControlType == CT_WL_RICHEDIT
			|| eControlType == CT_WL_RICHEDIT_IM
			|| eControlType == CT_TASK_WND_MGR
			|| eControlType == CT_PNL_DOCK
			|| eControlType == CT_UNIFORM_GRID
			|| eControlType == CT_PNL_STACK)
		{
			CRectCtrl *pRectCtrl = (CRectCtrl *)m_vpWLChildren[i]->m_pChildCtrl;

			CRect rcChildOrig;
			pRectCtrl->GetOrigWindowRect(rcChildOrig);
			CRect rcNew;
			RelayoutElement(m_pBindUIWndProp, rcChildOrig, ptWindowResize, rcNew,
				0, 0, lprc->right - lprc->left, lprc->bottom - lprc->top,
				eHorLayoutMode, eVerLayoutMode, rcParentMargin,
				bKeepAspectRatio, lMaxHorRatio, lMaxVerRatio);

			pRectCtrl->SendMessage(WM_WL_MOVEWINDOW, 0, (LPARAM)(LPCRECT)rcNew);
		}
	}

	return 0;
}

int CSkinListCtrl::OnDrawReportItem(CDC *pDC, int nItem, const CRect &rcItem, int nLeftMargin/* = 0*/)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	LVITEM item;
	item.mask = LVIF_IMAGE | LVIF_STATE;
	item.iItem = nItem;
	item.iSubItem = 0;
	item.stateMask = LVIS_SELECTED;
	BOOL bRet = GetItem(&item);
	if (!bRet)
	{
		return -1;
	}

	BOOL bEnabled = IsWindowEnabled();

	//
	// 1.Draw select state
	//
	if (LVIS_SELECTED & item.state)
	{
		CRect rcFill(rcItem);
		if (pMember->m_bEnableSelectedFitClient)
		{
			CRect rcClient;
			GetClientRect(rcClient);
			rcFill.right = rcClient.right;
		}

		if (bEnabled)
		{
			if (pMember->m_lvpItemS.m_hItemBkBmp == NULL)
			{
				pDC->FillSolidRect(rcFill, GetSelectedItemBkColor());
			}
			else
			{
				IUINineGridBlt(pDC->GetSafeHdc(),
					rcFill.left, rcFill.top, rcFill.Width(), rcFill.Height(),
					pMember->m_lvpItemS.m_hItemBkBmp, pMember->m_lvpItemS.m_ptResize, 1, 1);
			}
		}
		else
		{
			if (pMember->m_lvpItemSD.m_hItemBkBmp == NULL)
			{
				pDC->FillSolidRect(rcFill, GetSelectedItemBkColor());
			}
			else
			{
				IUINineGridBlt(pDC->GetSafeHdc(),
					rcFill.left, rcFill.top, rcFill.Width(), rcFill.Height(),
					pMember->m_lvpItemSD.m_hItemBkBmp, pMember->m_lvpItemSD.m_ptResize, 1, 1);
			}
		}
	}
	else if (pMember->m_bEnableHighlightItemEffect && (pMember->m_nCurHighlightItem == nItem))
	{
		CRect rcFill(rcItem);
		if (pMember->m_bEnableSelectedFitClient)
		{
			CRect rcClient;
			GetClientRect(rcClient);
			rcFill.right = rcClient.right;
		}

		if (pMember->m_lvpItemH.m_hItemBkBmp != NULL)
		{
			IUINineGridBlt(pDC->GetSafeHdc(),
				rcFill.left, rcFill.top, rcFill.Width(), rcFill.Height(),
				pMember->m_lvpItemH.m_hItemBkBmp, pMember->m_lvpItemH.m_ptResize, 1, 1);
		}
		else
		{
			pDC->FillSolidRect(rcFill, GetHighlightItemBkColor());
		}
	}

	//
	// 2. Draw icon, text and windowless sub item, last most CPU time
	//
	pMember->m_bSetItemDataInternal = TRUE;
	LV_ITEMDATA *pData = (LV_ITEMDATA *)GetItemData(nItem);
	pMember->m_bSetItemDataInternal = FALSE;

	int nColumnCount = pMember->m_wndHeader.GetItemCount();
	for (int nColumn = 0; nColumn < nColumnCount; ++nColumn)
	{
		// text string
		CString strText = GetItemText(nItem, nColumn);
		CRect rcText;
		pMember->m_bGetItemRectInternal = TRUE;
		GetItemRect(nItem, rcText, LVIR_LABEL);
		pMember->m_bGetItemRectInternal = FALSE;

		// text rect
		CRect rcHeaderItem;
		pMember->m_wndHeader.GetItemRect(nColumn, rcHeaderItem);
		pMember->m_wndHeader.ClientToScreen(rcHeaderItem);
		ScreenToClient(rcHeaderItem);
		CRect rcSubItem(rcItem);

		if (nColumn == 0)	// first column, may be has an icon
		{
			int nIconWidth = 0;
			int nIconHeight = 0;
			CImageList *pImageList = GetImageList(LVSIL_SMALL);
			if (pImageList != NULL)
			{
				ImageList_GetIconSize(pImageList->GetSafeHandle(), &nIconWidth, &nIconHeight);
			}

			//
			// 1. Draw state icon, such as check box
			//
			int nStateIconWidth = 0;
			int nStateIconHeight = 0;
			CImageList *pImageListState = GetImageList(LVSIL_STATE);
			if (pImageListState != NULL)
			{
				ImageList_GetIconSize(pImageListState->GetSafeHandle(), &nStateIconWidth, &nStateIconHeight);

				// Get item state index
				UINT nState = GetItemState(nItem, LVIS_STATEIMAGEMASK);
				int nStateImageIndex = nState >> 12;
				nStateImageIndex -= 1;

				if (nStateImageIndex >= 0)		// Index is 0-base.(Tree control is 1-base)
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
						::DrawIconEx(pDC->GetSafeHdc(),
							rcIcon.left + nLeftMargin, rcIcon.top,
							hIcon,
							rcIcon.Width(), rcIcon.Height(), 1, NULL, DI_NORMAL);
					}
					else
					{
						::DrawState(pDC->GetSafeHdc(), NULL, NULL,
							(LPARAM)hIcon, 0,
							rcIcon.left + nLeftMargin, rcIcon.top,
							rcIcon.Width(), rcIcon.Height(), DSS_DISABLED | DST_ICON);
					}

					::DestroyIcon(hIcon);
				}
			}

			//
			// 2. Draw small icon
			//
			if (pImageList != NULL)
			{
				// Get image 0-based index of this item
				UINT nImageIndex = GetItemImage(nItem, nColumn);

				CRect rcIcon;
				GetItemRect(nItem, rcIcon, LVIR_ICON);
				int nXOffset = (rcIcon.Width() - nIconWidth) / 2;
				if (nXOffset < 0)
				{
					nXOffset = 0;
				}
				int nYOffset = (rcIcon.Height() - nIconHeight) / 2;
				if (nYOffset < 0)
				{
					nYOffset = 0;
				}

				HICON hIcon = pImageList->ExtractIcon(nImageIndex);
				int nVisibleIconCX = min(nIconWidth, rcHeaderItem.right - (rcIcon.left + nXOffset));
				// if the 5th or 6th parameter of DrawIconEx is 0, use full size to draw.
				if (nVisibleIconCX != 0)
				{
					if (IsWindowEnabled())
					{
						::DrawIconEx(pDC->GetSafeHdc(),
							rcIcon.left + nLeftMargin, rcIcon.top + nYOffset,
							hIcon,
							nVisibleIconCX, nIconHeight, 1, NULL, DI_NORMAL);
					}
					else
					{
						::DrawState(pDC->GetSafeHdc(), NULL, NULL,
							(LPARAM)hIcon, 0,
							rcIcon.left + nLeftMargin, rcIcon.top + nYOffset,
							nVisibleIconCX, nIconHeight, DSS_DISABLED | DST_ICON);
					}
				}
				::DestroyIcon(hIcon);
			}

			//
			rcSubItem.left = rcText.left + 4; // space
		}
		else
		{
			rcSubItem.left = rcHeaderItem.left + 4;// space

			// draw sub-item icon
			int nImage = GetItemImage(nItem, nColumn);
			if (nImage != -1 && pMember->m_hImageList != NULL && pMember->m_hLevel5ImageList != NULL)
			{

				int nYOffset = (rcItem.Height() - pMember->m_sizeImageInSubItemImageList.cy) / 2;
				if (nYOffset < 0)
				{
					nYOffset = 0;
				}

				int nVisibleIconCX = min(pMember->m_sizeImageInSubItemImageList.cx, rcHeaderItem.Width());
				CPoint pt(rcHeaderItem.left + 1, rcItem.top + nYOffset);
				CSize size(nVisibleIconCX, pMember->m_sizeImageInSubItemImageList.cy);
				TransparentBltHBITMAP(pDC->GetSafeHdc(), pt.x + nLeftMargin, pt.y, size.cx, size.cy,
					pMember->m_hLevel5ImageList, pMember->m_sizeImageInSubItemImageList.cx * nImage, 0, size.cx, size.cy, RGB(255, 255, 255));
			}

			int nIconWidth = pMember->m_sizeImageInSubItemImageList.cx;
			if (pMember->m_hImageList != NULL)
			{
				rcSubItem.left += (nIconWidth + 2);
			}
		}
		rcSubItem.right = rcHeaderItem.right;

		// text align
		HDITEM hdi;
		hdi.mask = HDI_FORMAT;
		pMember->m_wndHeader.GetItem(nColumn, &hdi);

		UINT uFormat = DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
		if ((hdi.fmt & HDF_RIGHT) == HDF_RIGHT)
		{
			uFormat |= DT_RIGHT;
		}
		else if ((hdi.fmt & HDF_CENTER) == HDF_CENTER)
		{
			uFormat |= DT_CENTER;
		}
		else if ((hdi.fmt & HDF_RTLREADING) == HDF_RTLREADING)
		{
			uFormat |= DT_RTLREADING;
		}
		else
		{
			uFormat |= DT_LEFT;
		}

		HFONT hFontOld = NULL;
		HFONT hFont = NULL;
		if (LVIS_SELECTED & item.state)
		{
			hFont = GetHFont(pMember->m_lvpItemS.m_hIUIFont);
			pDC->SetTextColor(GetSelectedItemTextColor());
		}
		else if (IsEnableHighlightItemEffect() && (GetHighlightItem() == nItem)) // Item is highlight
		{
			hFont = GetHFont(pMember->m_lvpItemH.m_hIUIFont);
			pDC->SetTextColor(GetHighlightItemTextColor());
		}
		else
		{
			COLORREF cr = RGB(0, 0, 0);
			if (IsWindowEnabled())
			{
				cr = GetTextColor();
				hFont = GetHFont(pMember->m_lvpItemN.m_hIUIFont);
			}
			else
			{
				cr = GetNormalDisabledItemTextColor();
				hFont = GetHFont(pMember->m_lvpItemND.m_hIUIFont);
			}
			pDC->SetTextColor(cr);
		}

		CString strDefaultFont;
		if (hFont == NULL)
		{
			strDefaultFont = CUIMgr::GetDefaultFont();
			hFont = CreateIUIFont(strDefaultFont);
			ReleaseIUIFont(strDefaultFont);
		}

		hFontOld = (HFONT)SelectObject(pDC->GetSafeHdc(), hFont);

		if (strDefaultFont.IsEmpty())
		{
			ReleaseIUIFont(strDefaultFont);
		}

		// Reset text color by customs
		ITEMTEXTCOLOR itc;
		itc.nItem = nItem;
		itc.nSubItem = nColumn;
		LRESULT nRet = GetParent()->SendMessage(LVM_ITEM_TEXT_COLOR, 0, LPARAM(&itc));
		if (nRet != 0)
		{
			pDC->SetTextColor(itc.crItemText);
		}

		rcSubItem.OffsetRect(nLeftMargin, 0);

		// Give parent a chance
		DRAWCHILDTEXT dct;
		dct.hWndSender = m_hWnd;
		dct.item = nItem;
		dct.iSubItem = nColumn;
		dct.hDC = pDC->GetSafeHdc();
		dct.rcText = rcSubItem;
		dct.uFormat = uFormat;
		dct.lParam = 0;

		nRet = GetParent()->SendMessage(WM_DRAWCHILDTEXT, GetDlgCtrlID(), LPARAM(&dct));
		if (nRet == 0)
		{
			pDC->DrawText(strText, rcSubItem, uFormat);
		}

		if (hFontOld != NULL)
		{
			SelectObject(pDC->GetSafeHdc(), hFontOld);
		}

		// Draw windowless sub item. owner data list not set item data.
		if (nullptr != pData)
		{
			std::map<int, SItemBindObject*>::iterator it = pData->mapSubItemObject.find(nColumn);
			if (it != pData->mapSubItemObject.end())
			{
				SItemBindObject* pSubItemObject = it->second;

				if (pSubItemObject->uBindWndID > 0)
				{
					if (!pSubItemObject->bAlreadyCreateChildren)
					{
						_InstantiateItemBindTemplate(pSubItemObject, this);

						// Do something on instantiating item template.
						NMHDR nmhdr;
						nmhdr.hwndFrom = m_hWnd;
						nmhdr.idFrom = GetDlgCtrlID();
						nmhdr.code = LVN_INSTANTIATE_ITEM_TEMPLATE;
						NMLISTVIEW lvhdr;
						lvhdr.hdr = nmhdr;
						lvhdr.iItem = nItem;
						lvhdr.iSubItem = nColumn;
						GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&lvhdr));
					}

					if (pSubItemObject->bAlreadyCreateChildren)
					{
						if (!pSubItemObject->vSubWLWnd.empty())
						{
							CRect rcSubItem = rcItem;
							rcSubItem.left = rcHeaderItem.left;
							rcSubItem.right = rcHeaderItem.right;

							// Relayout the children.
							if (rcSubItem != pSubItemObject->rcLastParentLayout)
							{
								_RelayoutItemBindObject(this, rcSubItem, pSubItemObject);
							}

							// Draw windowless children.
							std::vector<CChildInfo>::iterator itChild = pSubItemObject->vSubWLWnd.begin();
							std::vector<CChildInfo>::iterator itChildEnd = pSubItemObject->vSubWLWnd.end();
							for (; itChild != itChildEnd; ++itChild)
							{
								CONTROL_TYPE eControlType = itChild->m_eControlType;
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
									CWLWnd* pWLWnd = (CWLWnd*)itChild->m_pChildCtrl;

									if (!pWLWnd->IsCreated())
									{
										continue;
									}

									if (pWLWnd->IsWindowVisible())
									{
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
		}
	}

	return 0;
}

int CSkinListCtrl::OnDrawIconItem(CDC *pDC, int nItem, const CRect &rcItem)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	LVITEM item;
	item.mask = LVIF_IMAGE | LVIF_STATE;
	item.iItem = nItem;
	item.iSubItem = 0;
	item.stateMask = LVIS_SELECTED;
	BOOL bRet = GetItem(&item);

	// Draw select state
	if (LVIS_SELECTED & item.state)
	{
		pDC->FillSolidRect(rcItem, pMember->m_lvpItemS.m_crItemBk);
	}
	else if (pMember->m_bEnableHighlightItemEffect && (pMember->m_nCurHighlightItem == nItem))
	{
		pDC->FillSolidRect(rcItem, pMember->m_lvpItemH.m_crItemBk);
	}

	// Draw icon
	CImageList *pImgList = GetImageList(LVSIL_NORMAL);
	if (bRet && pImgList != NULL)
	{
		IMAGEINFO ii;
		bRet = pImgList->GetImageInfo(item.iImage, &ii);
		if (bRet)
		{
			CRect rcImage = ii.rcImage;

			CRect rcIcon;
			GetItemRect(nItem, rcIcon, LVIR_ICON);
			int nXOffset = (rcIcon.Width() - rcImage.Width()) / 2;
			if (nXOffset < 0)
			{
				nXOffset = 0;
			}
			int nYOffset = (rcIcon.Height() - rcImage.Height()) / 2;
			if (nYOffset < 0)
			{
				nYOffset = 0;
			}

			HICON hIcon = pImgList->ExtractIcon(item.iImage);
			if (IsWindowEnabled())
			{
				::DrawIconEx(pDC->GetSafeHdc(), rcIcon.left + nXOffset, rcIcon.top + nYOffset, hIcon, rcImage.Width(), rcImage.Height(), 1, NULL, DI_NORMAL);
			}
			else
			{
				::DrawState(pDC->GetSafeHdc(), NULL, NULL, (LPARAM)hIcon, 0, rcIcon.left + nXOffset, rcIcon.top + nYOffset, rcImage.Width(), rcImage.Height(), DSS_DISABLED | DST_ICON);
			}
			::DestroyIcon(hIcon);
		}
	}

	// Draw text
	CString strText = GetItemText(nItem, 0);

	CRect rcText;
	GetItemRect(nItem, rcText, LVIR_LABEL);
	rcText.left = rcItem.left;
	rcText.right = rcItem.right;

	if (LVIS_SELECTED & item.state)
	{
		pDC->SetTextColor(GetSelectedItemTextColor());
	}
	else if (IsEnableHighlightItemEffect() && (GetHighlightItem() == nItem)) // Item is highlight
	{
		pDC->SetTextColor(GetHighlightItemTextColor());
	}
	else
	{
		COLORREF cr = RGB(0, 0, 0);
		if (IsWindowEnabled())
		{
			cr = GetTextColor();
		}
		else
		{
			cr = GetNormalDisabledItemTextColor();
		}
		pDC->SetTextColor(cr);
	}

	// Reset text color by customs
	ITEMTEXTCOLOR itc;
	itc.hWndSender = m_hWnd;
	itc.nItem = nItem;
	itc.nSubItem = 0;
	LRESULT nRet = GetParent()->SendMessage(LVM_ITEM_TEXT_COLOR, 0, LPARAM(&itc));
	if (nRet != 0)
	{
		pDC->SetTextColor(itc.crItemText);
	}

	pDC->DrawText(strText, rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);

	return 0;
}

int CSkinListCtrl::OnDrawThumbnailItem(CDC *pDC, int nItem, const CRect &rcItem)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	LVITEM item;
	item.mask = LVIF_IMAGE | LVIF_STATE;
	item.iItem = nItem;
	item.iSubItem = 0;
	item.stateMask = LVIS_SELECTED;
	BOOL bRet = GetItem(&item);

	// State
	BOOL bEnabled = IsWindowEnabled();
	BOOL bSelected = ((LVIS_SELECTED & item.state) == LVIS_SELECTED);
	BOOL bHighlight = pMember->m_bEnableHighlightItemEffect && (pMember->m_nCurHighlightItem == nItem);

	//
	BOOL bDrawItemBk = FALSE;
	LV_ITEMPROPERTIES *plvipDrawInfo = NULL;
	CRect rcThumbnailBkPosition(10, 10, 110, 130);
	CRect rcThumbnailPosition(20, 20, 100, 100);
	CRect rcLabelPosition(10, 110, 130, 130);
	CRect rcIconPosition(20, 20, 100, 100);
	if (bSelected)
	{
		bDrawItemBk = TRUE;
		if (bEnabled)
		{
			plvipDrawInfo = &pMember->m_lvpItemS;
		}
		else
		{
			plvipDrawInfo = &pMember->m_lvpItemSD;
		}

		rcThumbnailBkPosition = pMember->m_rcThumbnailBkPosition[2];
		rcThumbnailPosition = pMember->m_rcThumbnailPosition[2];
		rcLabelPosition = pMember->m_rcThumbnailLabelPosition[2];
		rcIconPosition = pMember->m_rcThumbnailIconPosition[2];
	}
	else if (pMember->m_bEnableHighlightItemEffect && bHighlight)
	{
		bDrawItemBk = TRUE;
		plvipDrawInfo = &pMember->m_lvpItemH;

		rcThumbnailBkPosition = pMember->m_rcThumbnailBkPosition[1];
		rcThumbnailPosition = pMember->m_rcThumbnailPosition[1];
		rcLabelPosition = pMember->m_rcThumbnailLabelPosition[1];
		rcIconPosition = pMember->m_rcThumbnailIconPosition[1];
	}
	else	// Normal
	{
		if (bEnabled)
		{
			if (pMember->m_bEnableNormalItemEffect)
			{
				bDrawItemBk = TRUE;
				plvipDrawInfo = &pMember->m_lvpItemN;
			}

			rcThumbnailBkPosition = pMember->m_rcThumbnailBkPosition[0];
			rcThumbnailPosition = pMember->m_rcThumbnailPosition[0];
			rcLabelPosition = pMember->m_rcThumbnailLabelPosition[0];
			rcIconPosition = pMember->m_rcThumbnailIconPosition[0];
		}
		else
		{
			if (pMember->m_bEnableNormalDisabledItemEffect)
			{
				bDrawItemBk = TRUE;
				plvipDrawInfo = &pMember->m_lvpItemND;
			}

			rcThumbnailBkPosition = pMember->m_rcThumbnailBkPosition[3];
			rcThumbnailPosition = pMember->m_rcThumbnailPosition[3];
			rcLabelPosition = pMember->m_rcThumbnailLabelPosition[3];
			rcIconPosition = pMember->m_rcThumbnailIconPosition[3];
		}
	}

	// Draw thumbnail item background
	if (bDrawItemBk)
	{
		CRect rcThumbnailBk(rcThumbnailBkPosition);
		rcThumbnailBk.OffsetRect(rcItem.TopLeft());

		if (plvipDrawInfo->m_eBkType == BKT_COLOR)
		{
			pDC->FillSolidRect(rcThumbnailBk, plvipDrawInfo->m_crItemBk);
		}
		else
		{
			IUINineGridBlt(pDC->GetSafeHdc(), rcThumbnailBk.left, rcThumbnailBk.top,
				rcThumbnailBk.Width(), rcThumbnailBk.Height(),
				plvipDrawInfo->m_hItemBkBmp, plvipDrawInfo->m_ptResize, 1, 1);
		}
	}

	// Draw Thumbnail if exist

	// Draw icon
	CRect rcIcon(rcIconPosition);
	rcIcon.OffsetRect(rcItem.TopLeft());

	CImageList *pImgList = GetImageList(LVSIL_NORMAL);
	if (bRet && pImgList != NULL)
	{
		IMAGEINFO ii;
		bRet = pImgList->GetImageInfo(item.iImage, &ii);
		if (bRet)
		{
			CRect rcImage = ii.rcImage;

			int nXOffset = (rcIcon.Width() - rcImage.Width()) / 2;
			if (nXOffset < 0)
			{
				nXOffset = 0;
			}
			int nYOffset = (rcIcon.Height() - rcImage.Height()) / 2;
			if (nYOffset < 0)
			{
				nYOffset = 0;
			}

			HICON hIcon = pImgList->ExtractIcon(item.iImage);
			if (IsWindowEnabled())
			{
				::DrawIconEx(pDC->GetSafeHdc(), rcIcon.left + nXOffset, rcIcon.top + nYOffset, hIcon, rcImage.Width(), rcImage.Height(), 1, NULL, DI_NORMAL);
			}
			else
			{
				::DrawState(pDC->GetSafeHdc(), NULL, NULL, (LPARAM)hIcon, 0, rcIcon.left + nXOffset, rcIcon.top + nYOffset, rcImage.Width(), rcImage.Height(), DSS_DISABLED | DST_ICON);
			}
			::DestroyIcon(hIcon);
		}
	}

	// Draw text
	CString strText = GetItemText(nItem, 0);

	CRect rcText(rcLabelPosition);
	rcText.OffsetRect(rcItem.TopLeft());

	if (LVIS_SELECTED & item.state)
	{
		pDC->SetTextColor(GetSelectedItemTextColor());
	}
	else if (IsEnableHighlightItemEffect() && (GetHighlightItem() == nItem)) // Item is highlight
	{
		pDC->SetTextColor(GetHighlightItemTextColor());
	}
	else
	{
		COLORREF cr = RGB(0, 0, 0);
		if (bEnabled)
		{
			cr = GetTextColor();
		}
		else
		{
			cr = GetNormalDisabledItemTextColor();
		}
		pDC->SetTextColor(cr);
	}

	// Reset text color by customs
	ITEMTEXTCOLOR itc;
	itc.nItem = nItem;
	itc.nSubItem = 0;
	LRESULT nRet = GetParent()->SendMessage(LVM_ITEM_TEXT_COLOR, 0, LPARAM(&itc));
	if (nRet != 0)
	{
		pDC->SetTextColor(itc.crItemText);
	}

	pDC->DrawText(strText, rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);

	return 0;
}

LRESULT CSkinListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class

	// When use layered window, the window not redraw when its child controls hide or display
	// to solve it, handle the WM_WINDOWPOSCHANGED message.
	BOOL bLayered = IsLayeredWindow(GetParent()->GetSafeHwnd());
	if (message == WM_WINDOWPOSCHANGED && bLayered)
	{
		WINDOWPOS *pos = (WINDOWPOS *)lParam;
		if (pos->hwnd == m_hWnd)
		{
			// When show the control, notify itself to redraw
			if ((pos->flags & SWP_SHOWWINDOW) == SWP_SHOWWINDOW)
			{
				Invalidate();
			}
			// When hide the control, notify its parent to redraw
			else if ((pos->flags & SWP_HIDEWINDOW) == SWP_HIDEWINDOW)
			{
				CRect rcThis;
				GetWindowRect(rcThis);
				GetParent()->ScreenToClient(rcThis);
				GetParent()->InvalidateRect(rcThis);
			}
		}
	}

	if (message == WM_CREATE)
	{
		LRESULT lr = CListCtrl::WindowProc(message, wParam, lParam);
		if (lr == -1)
		{
			return -1;
		}

		SetTextBkColor(RGB(255, 255, 255)); // Don't set RGB(0, 0, 0)

		return 0;
	}

	if (message == WM_DESTROY)
	{
		int nCount = GetItemCount();
		for (int i = nCount - 1; i >= 0 ; i--)
		{
			DeleteItem(i);
		}

		pMember->m_bListFirstShowed = FALSE;
	}

	if (message == WM_NCDESTROY)
	{
		if (pMember->m_wndHScroll.GetSafeHwnd() != NULL)
		{
			pMember->m_wndHScroll.DestroyWindow();
		}
		if (pMember->m_wndVScroll.GetSafeHwnd() != NULL)
		{
			pMember->m_wndVScroll.DestroyWindow();
		}
	}

	if (LVM_DRAW_BACKGROUND == message)
	{
		CDC *pDC = (CDC *)wParam;

		OnDrawBackground(pDC);
	}
	else if (LVM_DRAW_GRIDLINE == message)
	{
		CDC *pDC = (CDC *)wParam;

		OnDrawGridLine(pDC);
	}
	else if (LVM_DRAW_ITEM == message)
	{
		CDC *pDC = (CDC *)wParam;
		int nItem = (int)lParam;

		OnDrawItemEx(pDC, nItem, 0);
	}
	else if (LVM_DRAW_DEFAULT == message)
	{
		HDC hDC = (HDC)wParam;

		CWnd::DefWindowProc(WM_PAINT, wParam, 0);
	}

	// send WM_PARENTEVENT to parent
	switch (message)
	{
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
		// Simulate mouse on original item.
		if (pMember->m_bThumbnailView)
		{
			CPoint point(lParam);

			LVHITTESTINFO info;
			info.pt = point;
			ListView_SubItemHitTest(m_hWnd, &info);

			if (info.iItem >= 0)
			{
				// If on the label
				CRect rcItemLabel;
				BOOL bRet2 = GetThumbnailItemRect(info.iItem, rcItemLabel, LVIR_LABEL, CONTROL_STATE_NORMAL);
				if (bRet2 == 0 && rcItemLabel.PtInRect(point))
				{
					CRect rcItemLabelOriginal;
					pMember->m_bGetItemRectInternal = TRUE;
					GetItemRect(info.iItem, rcItemLabelOriginal, LVIR_LABEL);
					pMember->m_bGetItemRectInternal = FALSE;

					point = rcItemLabelOriginal.CenterPoint();
					lParam = MAKELPARAM(point.x, point.y);
				}
				// on the icon
				else
				{
					CRect rcItemOriginal;
					pMember->m_bGetItemRectInternal = TRUE;
					BOOL bRet1 = GetItemRect(info.iItem, rcItemOriginal, LVIR_BOUNDS);
					pMember->m_bGetItemRectInternal = FALSE;
					if (bRet1 && !rcItemOriginal.PtInRect(point))
					{
						point = rcItemOriginal.CenterPoint();
						lParam = MAKELPARAM(point.x, point.y);
					}
				}
			}
		}
#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
	case WM_MOUSEWHEEL:
#endif /* if (_WIN32_WINNT < 0x0400) */
		{
			int nID = GetDlgCtrlID();
			WPARAM wParam2 = MAKEWPARAM(message, nID);
			GetParent()->SendMessage(WM_PARENTEVENT, wParam2, lParam);
		}
		break;
	}

	switch (message)
	{
	case WM_MOUSEWHEEL:
	case WM_HSCROLL:
	case WM_VSCROLL:
	{
		if (pMember->m_bCacheMode)
		{
			ReDrawControl(FALSE, TRUE);
		}
		else
		{
			Invalidate(FALSE);
		}
	}
	break;
	case WM_TIMER:	// If the bottom item is part visible, click it will start a timer with id 43(IDT_SCROLLWAIT), in the timer, scroll the view to show the bottom item full.
		if (wParam == IDT_SCROLLWAIT) // Copy from \win2k source code\private\shell\comctl32\listview.c line 6303.
		{
			if (pMember->m_bCacheMode)
			{
				ReDrawControl(FALSE, TRUE);
			}
			else
			{
				Invalidate();
			}
		}
		break;
	default:
		break;
	}

	if (message == WM_MOUSEWHEEL && pMember->m_bUseCustomScrollBar)
	{
		LRESULT lr = CListCtrl::WindowProc(message, wParam, lParam);
		UpdateCustomScrollInfo(this);
		return lr;
	}

	// the CListCtrl can't response SB_THUMBTRACK and SB_THUMBPOSITION message send by custom,
	// but CTreeCtrl(can only in XP, can't response in Vista also) and CListBox can.
	// So, when CListCtrl receive these two message, use CListCtrl::Scroll to scroll content.
	if ((message == WM_VSCROLL || message == WM_HSCROLL) && pMember->m_bUseCustomScrollBar)
	{
		WORD sbCode = LOWORD(wParam);

		if (sbCode == SB_THUMBTRACK || sbCode == SB_THUMBPOSITION)
		{
			INT16 nPos = HIWORD(wParam);
			//
			SCROLLINFO siv = {0};
			siv.cbSize = sizeof(SCROLLINFO);
			siv.fMask = SIF_ALL;
			SCROLLINFO sih = siv;

			GetScrollInfo(SB_VERT, &siv);
			GetScrollInfo(SB_HORZ, &sih);

			// Calculate all scroll range.
			CRect rcClient;
			GetClientRect(&rcClient);
			SIZE sizeAll;
			if (sih.nPage == 0)
			{
				sizeAll.cx = rcClient.right;
			}
			else
			{
				sizeAll.cx = rcClient.right * (sih.nMax + 1) / sih.nPage ;
			}
			if (siv.nPage == 0)
			{
				sizeAll.cy = rcClient.bottom;
			}
			else
			{
				sizeAll.cy = rcClient.bottom * (siv.nMax + 1) / siv.nPage ;
			}

			// Value of size.cx that specifies the amount of horizontal scrolling in pixels. If the list-view control is in list-view, this value specifies the number of columns to scroll.
			// Value of size.cy that specifies the amount of vertical scrolling in pixels.
			// See also: LVM_SCROLL Message in MSDN.
			SIZE size = {0, 0};
			if (WM_VSCROLL == message)
			{
				size.cx = 0;

				LONGLONG llAllCy = sizeAll.cy;
				INT16 nPosAdd = nPos - (INT16)siv.nPos;		// 由于nPos只有16位，所以siv.nPos要按16位来计算。
				LONGLONG sizeCy = llAllCy * nPosAdd / (siv.nMax + 1);
				size.cy = (LONG)sizeCy;
			}
			else
			{
				LONGLONG llAllCx = sizeAll.cx;
				INT16 nPosAdd = nPos - (INT16)sih.nPos;
				LONGLONG sizeCx = llAllCx * nPosAdd / (sih.nMax + 1);
				size.cx = (LONG)sizeCx;

				size.cy = 0;
			}

			Scroll(size);	// size是每次滚动相对于上次的增量，而不是绝对量。
		}

		LRESULT lr = CListCtrl::WindowProc(message, wParam, lParam);
		UpdateCustomScrollInfo(this);
		return lr;
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
			IUIBitBlt(dcWin.GetSafeHdc(), rcSizeBox2This, pMember->m_hLevel1ResizedBkImage, rcSizeBox2This.left, rcSizeBox2This.top, SRCCOPY);

			return 0;
		}

		LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		BOOL bBorder = ((lStyle & WS_BORDER) == WS_BORDER);
		if (!bBorder)
		{
			return CListCtrl::WindowProc(message, wParam, lParam);
		}

		// Draw the border and scroll bar
		DefWindowProc(message, wParam, lParam);

		if (pMember->m_bCacheMode)
		{
			IUIDrawNCBackground(this, pMember->m_hLevel1ResizedBkImage, FALSE, NULL, CRect(1, 1, 1, 1));
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

		BOOL bParentDraw = (BOOL)GetParent()->SendMessage(WM_DRAWLIST, WPARAM(&dc), LPARAM(this));
		if (bParentDraw)
		{
			return CListCtrl::WindowProc(message, wParam, lParam);
		}

		// TODO: Add your message handler code here

		// First to show
		if (!pMember->m_bListFirstShowed)
		{
			pMember->m_bListFirstShowed = TRUE;

			CRect rcWin;
			GetWindowRect(rcWin);

			_LibUIDK_CSkinListCtrl_UpdateLevel1(this, pMember->m_bListFirstShowed,
				pMember->m_himgBk[0], pMember->m_himgBk[1],
				m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				rcWin.Width(), rcWin.Height(), &pMember->m_hLevel1ResizedBkImage);
		}

		CRect rcClient;
		GetClientRect(&rcClient);

		if (pMember->m_bCacheMode)
		{
			_LibUIDK_CSkinListCtrl_UpdateLevel2(this, pMember->m_bListFirstShowed,
				pMember->m_hLevel2BlendBkTextImage);
			_LibUIDK_CSkinListCtrl_UpdateLevel3(this, pMember->m_bListFirstShowed,
				pMember->m_hLevel2BlendBkTextImage, pMember->m_hLevel3Background, -1, -1);

			BitBltG(dc.GetSafeHdc(), 0, 0, rcClient.Width(), rcClient.Height(),
				pMember->m_bEnableHighlightItemEffect ? pMember->m_hLevel3Background : pMember->m_hLevel2BlendBkTextImage,
				0, 0, SRCCOPY);
		}
		else
		{
			// draw background
			CDC memBkDC;
			memBkDC.CreateCompatibleDC(&dc);
			CBitmap bmpBk;
			bmpBk.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
			CBitmap *pbmpBkOld = memBkDC.SelectObject(&bmpBk);

			OnDrawBackground(&memBkDC);

			// Draw content

			// Draw 
			OnDrawItemInterlacedHighlight(&memBkDC);
			// Draw grid line of list with report style.
			OnDrawGridLine(&memBkDC);

			// draw all of the items that are completely visible
			int nItem = GetTopIndex();
			int nLast = nItem + GetCountPerPage() + 1;
			int nCount = GetItemCount();
			if (nLast > nCount)
			{
				nLast = nCount;
			}

			memBkDC.SetBkMode(TRANSPARENT);

			for (; nItem < nLast; nItem++)
			{
				if (pMember->m_bAnimationMode)
				{
					// Each item's left margin is different.
					int nIndex = 0;
					if (pMember->m_bTogetherAnimation)
					{
						nIndex = pMember->m_nCurTimerTick;
					}
					else
					{
						if (pMember->m_nCurTimerTick - nItem >= 0)
						{
							nIndex = pMember->m_nCurTimerTick - nItem;
						}
						else
						{
							nIndex = 0;
						}
						if (nIndex >= (int)pMember->m_vAnimationData.size())
						{
							nIndex = (int)pMember->m_vAnimationData.size() - 1;
						}
					}

					OnDrawItemEx(&memBkDC, nItem, pMember->m_vAnimationData[nIndex]);

					// After the last item animation finish, kill the timer.
					if (nItem == nLast - 1 && nIndex == pMember->m_vAnimationData.size() - 1)
					{
						KillTimer(IDT_WIN8);
						pMember->m_bAnimationMode = false;
						pMember->m_nCurTimerTick = 0;
						pMember->m_vAnimationData.clear();

						NMHDR nmhdr;
						nmhdr.hwndFrom = m_hWnd;
						nmhdr.idFrom = GetDlgCtrlID();
						nmhdr.code = LVN_ANIMATION_END;
						GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmhdr));
					}
				}
				else
				{
					OnDrawItemEx(&memBkDC, nItem, 0);
				}
			}

			dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memBkDC, 0, 0, SRCCOPY);
			memBkDC.SelectObject(pbmpBkOld);

			UpdateCustomScrollBar();
		}
	}

	// when the Header is created.
	if (message == WM_PARENTNOTIFY)
	{
		UINT uEvent = LOWORD(wParam);
		UINT uID = HIWORD(wParam);
		if (uEvent == WM_CREATE && uID == 0)
		{
			HWND hHeader = HWND(lParam);
			TCHAR szClassName[MAX_PATH] = {0};
			GetClassName(hHeader, szClassName, MAX_PATH);
			CString strClassName(szClassName);
			if (strClassName.Compare(WC_HEADER) == 0)
			{
				if (hHeader != NULL)
				{
					if (pMember->m_wndHeader.GetSafeHwnd() == NULL)
					{
						pMember->m_wndHeader.SubclassWindow(hHeader);
					}
				}
			}
		}
	}

	// while the header adjust the column's width, refresh the list
	if (message == WM_NOTIFY)
	{
		int nCtrlID = (int)wParam;
		NMHDR *pnmhdr = (NMHDR *)lParam;

		if (nCtrlID == 0)	// From header control.
		{
			if (!pMember->m_bHeaderTrack)
			{
				// Lock header item track or not.
				if (pnmhdr->code == HDN_BEGINTRACKA || pnmhdr->code == HDN_BEGINTRACKW)
				{
					CListCtrl::WindowProc(message, wParam, lParam);

					return TRUE;	// Returns FALSE to allow tracking of the divider, or TRUE to prevent tracking.
				}
				else if (pnmhdr->code == HDN_DIVIDERDBLCLICKA || pnmhdr->code == HDN_DIVIDERDBLCLICKW)
				{
					return TRUE;	// Don't call CListCtrl::WindowProc(message, wParam, lParam); otherwise can't refuse adjust the width of column by double click the divider.
				}
			}
			else
			{
				if (pMember->m_bCacheMode)
				{
					// Adjust the width of the header.
					switch (pnmhdr->code)
					{
					case HDN_ITEMCHANGINGA:		// ed4
					case HDN_ITEMCHANGINGW:		// ec0
					// 		case HDN_ITEMCHANGEDA:		// ed3
					// 		case HDN_ITEMCHANGEDW:		// ebf
					// 		case HDN_BEGINTRACKA:
					// 		case HDN_BEGINTRACKW:		// eba
					case HDN_ENDTRACKA:
					case HDN_ENDTRACKW:			// eb9
						// 		case HDN_TRACKA:
						// 		case HDN_TRACKW:
					{
						//				TRACE("[LibUIDK]===Notify code========%08x\n", pnmhdr->code);

						// Through testing, Using the following code to refresh only change area can't improve the performance.
						// But, if the separator out the control, and don't refresh, can improve the performance.
						NMHEADER *pnmheader = (NMHEADER *)lParam;
						int nItem = pnmheader->iItem;
						CRect rcItem;
						pMember->m_wndHeader.GetItemRect(nItem, rcItem);
						CRect rcClient;
						GetClientRect(rcClient);
						rcClient.left = rcItem.right;

						// If the separator out the control, don't refresh.
						if (rcItem.right < rcClient.right)
						{
							ReDrawControl(FALSE, FALSE, rcClient);
						}
					}
					break;
					default:
						break;
					}
				}
				else
				{
					if (pnmhdr->code == HDN_ITEMCHANGED)
					{
						UpdateCustomScrollBar();
						Invalidate();
					}
				}
			}
		}
	}

	if (message == WM_MOUSEMOVE)
	{
		if (pMember->m_bEnableHighlightItemEffect)
		{
			CPoint point((DWORD)lParam);

			LVHITTESTINFO info;
			info.pt = point;
			ListView_SubItemHitTest(m_hWnd, &info);
			if (pMember->m_nCurHighlightItem != info.iItem)
			{
				if (pMember->m_bCacheMode)
				{
					int nOldHighlightIndex = pMember->m_nCurHighlightItem;
					pMember->m_nCurHighlightItem = info.iItem;

					_LibUIDK_CSkinListCtrl_UpdateLevel3(this, pMember->m_bListFirstShowed, pMember->m_hLevel2BlendBkTextImage, pMember->m_hLevel3Background, pMember->m_nCurHighlightItem, nOldHighlightIndex);

					// If add UpdateWindow call after the follow code, and call the these code 1000 times,
					// the performance excelled call Invalidate();
					CRect rcOld;
					GetItemRect(nOldHighlightIndex, rcOld, LVIR_BOUNDS);
					CRect rcNew;
					GetItemRect(pMember->m_nCurHighlightItem, rcNew, LVIR_BOUNDS);
					InvalidateRect(rcOld, FALSE);
					InvalidateRect(rcNew, FALSE);
				}
				else
				{
					pMember->m_nCurHighlightItem = info.iItem;
					Invalidate();
				}
			}
		}

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = HOVER_DEFAULT;
		::TrackMouseEvent(&tme);
	}

	// Hit test the thumbnail item. if Handle this message, the old tooltip will can't be show.
	if (IsThumbnailView() && message == LVM_SUBITEMHITTEST)
	{
		LVHITTESTINFO *pInfo = (LVHITTESTINFO *)lParam;

		LRESULT hr = CListCtrl::WindowProc(message, wParam, lParam);

		// to determine whether to scroll the contents of a list-view control
		if ((pInfo->flags & LVHT_ABOVE) == LVHT_ABOVE
			|| (pInfo->flags & LVHT_BELOW) == LVHT_BELOW
			|| (pInfo->flags & LVHT_TOLEFT) == LVHT_TOLEFT
			|| (pInfo->flags & LVHT_TORIGHT) == LVHT_TORIGHT)
		{
			return hr;
		}

		int nItem = GetTopIndex();
		int nLast = nItem + GetCountPerPage() + 1;
		int nCount = GetItemCount();
		if (nLast > nCount)
		{
			nLast = nCount;
		}

		BOOL bItem = FALSE;
		for (; nItem < nLast; ++nItem)
		{
			// Total item rect
			CRect rcItem;
			GetThumbnailItemTotalRect(nItem,  rcItem);

			// Thumbnail Background rect.
			CRect rcThumbnailBk(pMember->m_rcThumbnailBkPosition[0]);
			rcThumbnailBk.OffsetRect(rcItem.TopLeft());

			if (rcThumbnailBk.PtInRect(pInfo->pt))
			{
				bItem = TRUE;
				break;
			}
		}

		if (bItem)
		{
			pInfo->flags = LVHT_ONITEM;
			pInfo->iItem = nItem;
			pInfo->iSubItem = -1;
			return nItem;
		}
		else
		{
			pInfo->flags = LVHT_NOWHERE;
			pInfo->iItem = -1;
			pInfo->iSubItem = -1;
			return -1;
		}
	}

	if (message == LVM_GETITEMRECT)
	{
		// if the list is thumbnail view, using GetThumbnailItemRect to get thumbnail item's rect.
		if (pMember->m_bThumbnailView && !pMember->m_bGetItemRectInternal)
		{
			return FALSE;
		}
	}

	if (message == WM_MOUSELEAVE)
	{
		if (pMember->m_bEnableHighlightItemEffect)
		{
			if (pMember->m_bCacheMode)
			{
				int nOldHighlightItem = pMember->m_nCurHighlightItem;
				pMember->m_nCurHighlightItem = -1;

				ReDrawControl(FALSE, FALSE);
			}
			else
			{
				pMember->m_nCurHighlightItem = -1;
				Invalidate();
			}
		}
	}

	// when the tree lose focus, and pMember->m_bEnableSelectedFitClient is TREE, the right area of last sub item need refresh.
	if (message == WM_KILLFOCUS)
	{
		if (pMember->m_bCacheMode)
		{
			ReDrawControl(FALSE, FALSE);
		}
		else
		{
			Invalidate();
		}
	}

	if (message == WM_SIZE)
	{
		if (pMember->m_bCacheMode)
		{
			WPARAM nType = wParam;
			CSize size((DWORD)lParam);

			if (pMember->m_bListFirstShowed)
			{
				ReDrawControl(TRUE, TRUE);
			}
		}
		else
		{
			// The WM_PAINT and WM_NCPAINT both need background image to draw,
			// so, always cache the resized background image.
			CRect rcWin;
			GetWindowRect(rcWin);

			_LibUIDK_CSkinListCtrl_UpdateLevel1(this, pMember->m_bListFirstShowed,
				pMember->m_himgBk[0], pMember->m_himgBk[1],
				m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				rcWin.Width(), rcWin.Height(), &pMember->m_hLevel1ResizedBkImage);
		}
	}

	// update scroll bar.
	// LVM_INSERTITEM, LVM_DELETEITEM, LVM_DELETEALLITEMS and
	// WM_GETDLGCODE/*arrow key down*/ can trigger WM_PAINT message,
	// so handle in WM_PAINT message function.
	if (pMember->m_bUseCustomScrollBar && (message == WM_SIZE || message == WM_MOVE))
	{
		LRESULT lr = CListCtrl::WindowProc(message, wParam, lParam);
		UpdateCustomScrollBar();
		return lr;
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

	if (LVM_INSERTITEM == message)
	{
		int nItem = (int)CListCtrl::WindowProc(message, wParam, lParam);

		// Copy from \win2k source code\private\shell\comctl32\listview.c line 2471. But new version of list view may be not use sturct LV to store the data.
		// and DPA_GetPtr is available through Microsoft Windows XP Service Pack 2 (SP2) and Windows Server 2003. It might be altered or unavailable in subsequent versions of Windows.
		// 		LV *plv = (LV *)GetWindowLong(m_hWnd, 0); // Is ListView_GetPtr
		// 		LISTITEM *pItem = (LISTITEM *)DPA_GetPtr(plv->hdpa, nItem);

		pMember->m_bSetItemDataInternal = TRUE;
		pMember->m_bInsertingItemSetData = TRUE;
		LV_ITEMDATA *pData = new LV_ITEMDATA;

		// The SetItemData can cause LVN_ITEMCHANGED notify be send to list's parent window, So, if you call GetItemData in LVN_ITEMCHANGED, may be get the pData, since m_bSetItemDataInside is TRUE.
#if(_MSC_VER <= 1200) // VC6.0
		SetItemData(nItem, DWORD(pData));
#else
		SetItemData(nItem, DWORD_PTR(pData));
#endif

		pMember->m_bInsertingItemSetData = FALSE;
		pMember->m_bSetItemDataInternal = FALSE;

		return nItem;

	}
	if (LVM_DELETEITEM == message)
	{
		int nItem = (int)wParam;
		pMember->m_bSetItemDataInternal = TRUE;
		LV_ITEMDATA *pData = (LV_ITEMDATA *)GetItemData(nItem);
		pMember->m_bSetItemDataInternal = FALSE;

		int nColumnCount = 0;
		CHeaderCtrl *pHeaderCtrl = GetHeaderCtrl();
		if (pHeaderCtrl != NULL)
		{
			nColumnCount = pHeaderCtrl->GetItemCount();
		}

		LRESULT lr = CListCtrl::WindowProc(message, wParam, lParam);

		// The the sub item bind window id, release the memory
		if (pData == NULL)
		{
			ASSERT(FALSE);
			return lr;
		}

		// Destroy old.
		for (int nColumn = 0; nColumn < nColumnCount; ++nColumn)
		{
			std::map<int, SItemBindObject *>::iterator it = pData->mapSubItemObject.find(nColumn);
			if (it != pData->mapSubItemObject.end())
			{
				it->second->ReleaseChildren(it->second->bBindWLWndNeedRelease);

				delete it->second;
			}
		}

		SafeDelete(pData);
		return lr;
	}
	if (LVM_DELETEALLITEMS == message)
	{
		int nCount = GetItemCount();
		for (int i = nCount - 1; i >= 0 ; i--)
		{
			pMember->m_bSetItemDataInternal = TRUE;
			LV_ITEMDATA *pData = (LV_ITEMDATA *)GetItemData(i);
			pMember->m_bSetItemDataInternal = FALSE;
			SafeDelete(pData);
		}
	}
	// replace item data
	if (LVM_GETITEM == message && (!pMember->m_bSetItemDataInternal || pMember->m_bInsertingItemSetData))
	{
		LVITEM *plvi = (LVITEM *)lParam;
		if ((plvi->mask & LVIF_PARAM) == LVIF_PARAM)
		{
			BOOL bRet = (BOOL)CListCtrl::WindowProc(message, wParam, lParam);
			if (bRet)
			{
				LV_ITEMDATA *pData = (LV_ITEMDATA *)plvi->lParam;
				plvi->lParam = pData == NULL ? 0 : pData->lParam;
			}
			return bRet;
		}
	}
	// replace item data
	if (LVM_SETITEM == message && !pMember->m_bSetItemDataInternal)
	{
		LVITEM *plvi = (LVITEM *)lParam;
		if ((plvi->mask & LVIF_PARAM) == LVIF_PARAM)
		{
			pMember->m_bSetItemDataInternal = TRUE;
			LV_ITEMDATA *pData = (LV_ITEMDATA *)GetItemData(plvi->iItem);
			pMember->m_bSetItemDataInternal = FALSE;
			pData->lParam = plvi->lParam;
			plvi->lParam = (LPARAM)pData;
			return CListCtrl::WindowProc(message, wParam, (LPARAM)plvi);
		}
	}
	// replace item data
	if (LVM_SORTITEMS == message)
	{
		pMember->m_LibUIDKSortData.pThis = this;
		pMember->m_LibUIDKSortData.pfnUserCompare = (PFNLVCOMPARE)lParam;
		pMember->m_LibUIDKSortData.dwUserData = (DWORD)wParam;

		wParam = (WPARAM)&pMember->m_LibUIDKSortData;
		lParam = (LPARAM)_LibUIDK_CSkinListCtrl_Compare;
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

	if (message == WM_TIMER && wParam == IDT_WIN8)
	{
		Invalidate();

		pMember->m_nCurTimerTick++;

		return 0;	// The List control default handle may Kill the timer.
	}

	// Child CWLButton send.
	if (message == WM_WL_COMMAND)
	{
		UINT uID = LOWORD(wParam);
		UINT uCode = HIWORD(wParam);
		CWLButton *pSender = (CWLButton *)lParam;

		// Hit test sub item while left button down.
		POINT point = {0};
		GetCursorPos(&point);
		ScreenToClient(&point);
		LVHITTESTINFO infoLBtnDown;
		infoLBtnDown.pt = point;
		infoLBtnDown.flags = LVHT_ONITEM;
		ListView_SubItemHitTest(m_hWnd, &infoLBtnDown);

		//
		NMHDR nmhdr;
		nmhdr.hwndFrom = m_hWnd;
		nmhdr.idFrom = GetDlgCtrlID();
		nmhdr.code = LVN_WLBUTTON_CLICK;

		NMITEMACTIVATE nmia = {0};
		nmia.hdr = nmhdr;
		nmia.iItem = infoLBtnDown.iItem;
		nmia.iSubItem = infoLBtnDown.iSubItem;
		nmia.ptAction = point;
		nmia.lParam = (LPARAM)pSender;

		GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmia));
	}

	// 	if (message == WM_NCCALCSIZE)
	// 	{
	// 		BOOL bCalcValidRects = (BOOL)wParam;
	//
	// 		if (bCalcValidRects)
	// 		{
	// 			NCCALCSIZE_PARAMS *lpncsp = (NCCALCSIZE_PARAMS *)lParam;
	//
	// 			// 减小List控件滚动条区域宽度
	// 			LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	// 			// 下面的10,为滚动条的宽度。这个需要为List增加个接口，设置这个宽度。
	// 			if (IsIncludeFlag(lStyle, WS_HSCROLL))
	// 			{
	// 				lpncsp->rgrc[0].bottom += (GetSystemMetrics(SM_CYVSCROLL) - 10);
	// 			}
	// 			if (IsIncludeFlag(lStyle, WS_VSCROLL))
	// 			{
	// 				lpncsp->rgrc[0].right += (GetSystemMetrics(SM_CXVSCROLL) - 10);
	// 			}
	// 		}
	// 	}

	return CListCtrl::WindowProc(message, wParam, lParam);
}

// when use EnableOwnerDraw to set owner-draw, redraw the first item's select state immediately,
// otherwise, the first item's will redraw after it received the WM_TIMER message.
// CAN'T use ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged) to handle, if you use it, the parent will can't receive LVN_ITEMCHANGED message.
BOOL CSkinListCtrl::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT *pLResult)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class
	int nID = GetDlgCtrlID();

	if (wParam == nID && message == WM_NOTIFY)
	{
		NMHDR *pnmhdr = (NMHDR *)lParam;
		if (pnmhdr->code == LVN_ITEMCHANGED)
		{
			if (pMember->m_bCacheMode)
			{
				NMLISTVIEW *pnmlv = (NMLISTVIEW *)lParam;

				if (pnmlv->uChanged != LVIF_PARAM)
				{
					ReDrawControl(FALSE, FALSE);
				}
			}
			else
			{
				Invalidate();
			}
		}
		else if (pnmhdr->code == NM_CLICK)
		{
			NMITEMACTIVATE *pnmItemActivate = (NMITEMACTIVATE *)lParam;
			POINT point = pnmItemActivate->ptAction;

			//
			// Generate sub item click notify
			//

			// Hit test sub item while left button down.
			LVHITTESTINFO infoLBtnDown;
			infoLBtnDown.pt = point;
			infoLBtnDown.flags = LVHT_ONITEM;
			ListView_SubItemHitTest(m_hWnd, &infoLBtnDown);

			// If click valid sub item while left button down.
			if (infoLBtnDown.iItem != -1 && infoLBtnDown.iSubItem != -1)
			{
				CPoint ptLBtnUp;
				GetCursorPos(&ptLBtnUp);
				ScreenToClient(&ptLBtnUp);

				LVHITTESTINFO infoLBtnUp;
				infoLBtnUp.pt = ptLBtnUp;
				infoLBtnUp.flags = LVHT_ONITEM;
				ListView_SubItemHitTest(m_hWnd, &infoLBtnUp);

				// If release left button on same sub item.
				if (infoLBtnDown.iItem == infoLBtnUp.iItem && infoLBtnDown.iSubItem == infoLBtnUp.iSubItem)
				{
					// Generate sub item click notify
					// Handler: 	ON_NOTIFY(LVN_SUBITEMCLICK, IDC_LIST1, OnSubItemClickList1)
					// void CYourWnd::OnSubItemClickList1(NMHDR* pNMHDR, LRESULT* pResult)
					NMHDR nmhdr;
					nmhdr.hwndFrom = m_hWnd;
					nmhdr.idFrom = GetDlgCtrlID();
					nmhdr.code = LVN_SUBITEMCLICK;

					NMITEMACTIVATE nmia;
					nmia.hdr = nmhdr;
					nmia.iItem = infoLBtnDown.iItem;
					nmia.iSubItem = infoLBtnDown.iSubItem;
					nmia.uNewState = pnmItemActivate->uNewState;
					nmia.uOldState = pnmItemActivate->uOldState;
					nmia.uChanged = pnmItemActivate->uChanged;
					nmia.ptAction = pnmItemActivate->ptAction;
					nmia.lParam = pnmItemActivate->lParam;
					nmia.uKeyFlags = pnmItemActivate->uKeyFlags;

					GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmia));
				}
			}

			//
			// Move the position of Edit control
			//
			LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
			if ((lStyle & LVS_EDITLABELS) == LVS_EDITLABELS)
			{
				if (infoLBtnDown.iItem >= 0)
				{
					// If on the label
					CRect rcItemLabel;
					BOOL bRet2 = GetThumbnailItemRect(infoLBtnDown.iItem, rcItemLabel, LVIR_LABEL, CONTROL_STATE_NORMAL);
					if (bRet2 == 0 && rcItemLabel.PtInRect(point))
					{
						pMember->m_nEditingItem = infoLBtnDown.iItem;
						EditLabel(infoLBtnDown.iItem);

						CEdit *pEdit = GetEditControl();
						CRect rcLabel;
						GetThumbnailItemRect(infoLBtnDown.iItem, rcLabel, LVIR_LABEL, CONTROL_STATE_NORMAL);
						pEdit->MoveWindow(rcLabel);

						pMember->m_nEditingItem = -1;
					}
				}
			}
		}
	}

	return CListCtrl::OnChildNotify(message, wParam, lParam, pLResult);
}

//////////////////////////////////////////////////////////////////////////
// Public member functions

#if(_MSC_VER <= 1400) // VC8.0

BOOL CSkinListCtrl::GetItemSpacing(BOOL fSmall, int *pnHorzSpacing, int *pnVertSpacing) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pnHorzSpacing != NULL);
	ASSERT(pnVertSpacing != NULL);
	if (pnHorzSpacing == NULL || pnVertSpacing == NULL)
	{
		return FALSE;
	}
	DWORD dwSpacing = (DWORD)::SendMessage(m_hWnd, LVM_GETITEMSPACING, (WPARAM)fSmall, 0L);
	*pnHorzSpacing = (int)LOWORD(dwSpacing);
	*pnVertSpacing = (int)HIWORD(dwSpacing);
	return TRUE;
}

#endif

// Draw all to pMember->m_hLevel3Background to cache
int CSkinListCtrl::ReDrawControl(BOOL bErase, BOOL bUpdateScrollBar, LPCRECT lpInvalidateRect/* = NULL*/)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (!pMember->m_bListFirstShowed)
	{
		return -1;
	}

	// Update level-1 cache
	if (bErase)
	{
		// 		CRect rcWin;
		// 		GetWindowRect(rcWin);
		// 		_LibUIDK_CSkinListCtrl_UpdateLevel1(this, pMember->m_bListFirstShowed, pMember->m_himgBk[0], pMember->m_himgBk[1], ptResize.x, ptResize.y, rcWin.Width(), rcWin.Height(), pMember->m_hLevel1ResizedBkImage);
	}

	if (pMember->m_bEnableHighlightItemEffect)
	{
		_LibUIDK_CSkinListCtrl_UpdateLevel2(this, pMember->m_bListFirstShowed, pMember->m_hLevel2BlendBkTextImage);
		_LibUIDK_CSkinListCtrl_UpdateLevel3(this, pMember->m_bListFirstShowed, pMember->m_hLevel2BlendBkTextImage, pMember->m_hLevel3Background, -1, -1);
	}
	else
	{
		_LibUIDK_CSkinListCtrl_UpdateLevel2(this, pMember->m_bListFirstShowed, pMember->m_hLevel2BlendBkTextImage);
	}

	if (bUpdateScrollBar)
	{
		UpdateCustomScrollBar();
	}

	InvalidateRect(lpInvalidateRect, FALSE);

	return 0;
}

int UpdateImageList(CImageList *pImgList, CSkinListCtrl *pListCtrl, HBITMAP &hImageListCache, __out LPSIZE lpSize)
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

	CClientDC dc(pListCtrl);

	CRect rcClient;
	pListCtrl->GetClientRect(rcClient);

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

int CSkinListCtrl::UpdateImageListCache()
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	CImageList *pImgList = GetImageList(LVSIL_SMALL);
	if (pImgList == NULL)
	{
		return 1;
	}

	return UpdateImageList(pImgList, this, pMember->m_hLevel4ImageList, &pMember->m_sizeImageInImageList);
}

int CSkinListCtrl::UpdateSubItemImageListCache()
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	CImageList *pImgList = GetImageListEx();
	if (pImgList == NULL)
	{
		return 1;
	}

	return UpdateImageList(pImgList, this, pMember->m_hLevel5ImageList, &pMember->m_sizeImageInSubItemImageList);
}

int CSkinListCtrl::SetHeaderHeight(int nHeight)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_wndHeader.SetHeaderHeight(nHeight);
}

int CSkinListCtrl::GetHeaderHeight() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_wndHeader.GetHeaderHeight();
}

int CSkinListCtrl::SetHeaderBkImage(LPCTSTR lpszImageNameHeaderBk)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_wndHeader.SetBkImage(lpszImageNameHeaderBk);
}

int CSkinListCtrl::GetHeaderBkImage(CString *pstrImageNameHeaderBk) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_wndHeader.GetBkImage(pstrImageNameHeaderBk);
}

int CSkinListCtrl::SetHeaderButtonImages(
	UINT uMask,
	LPCTSTR lpszImageNameHeaderN, LPCTSTR lpszImageNameHeaderH,
	LPCTSTR lpszImageNameHeaderS, LPCTSTR lpszImageNameHeaderD)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_wndHeader.SetButtonImages(uMask,
			lpszImageNameHeaderN, lpszImageNameHeaderH,
			lpszImageNameHeaderS, lpszImageNameHeaderD);
}

int CSkinListCtrl::GetHeaderButtonImages(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameHeaderN, CString *pstrImageNameHeaderH,
	CString *pstrImageNameHeaderS, CString *pstrImageNameHeaderD) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_wndHeader.GetButtonImages(uMask, pbCombineImage, pstrCombineImageName,
			pstrImageNameHeaderN, pstrImageNameHeaderH, pstrImageNameHeaderS, pstrImageNameHeaderD);
}

int CSkinListCtrl::EnableHeaderTrack(BOOL bEnable)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_bHeaderTrack = bEnable;

	return 0;
}

BOOL CSkinListCtrl::IsHeaderTrackEnable() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_bHeaderTrack;
}

// Since the sub item image size not equal to the size of image in first column. use a new image list to store the sub item image.
int CSkinListCtrl::SetImageListEx(CImageList *pImageList)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_hImageList = pImageList->GetSafeHandle();

	if (pMember->m_bCacheMode)
	{
		UpdateSubItemImageListCache();
	}

	return 0;
}

CImageList *CSkinListCtrl::GetImageListEx() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return CImageList::FromHandle(pMember->m_hImageList);
}

int CSkinListCtrl::GetSubItemIconRect(int nItem, int nSubItem, LPRECT lpRect) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (nSubItem <= 0)
	{
		return -1;
	}

	if (pMember->m_wndHeader.GetSafeHwnd() == NULL)
	{
		return -8;
	}

	int nCount = GetItemCount();
	if (nItem < 0 || nItem >= nCount)
	{
		return -2;
	}

	if (lpRect == NULL)
	{
		return -3;
	}

	if (pMember->m_hImageList == NULL)
	{
		return -4;
	}

	int nImage = GetItemImage(nItem, nSubItem);
	if (nImage == -1)
	{
		return -5;
	}

	IMAGEINFO ii;
	BOOL bRet = ImageList_GetImageInfo(pMember->m_hImageList, nImage, &ii);
	if (!bRet)
	{
		return -6;
	}

	// save the sub icon rect
	pMember->m_bSetItemDataInternal = TRUE;
	LV_ITEMDATA *pData = (LV_ITEMDATA *)GetItemData(nItem);
	pMember->m_bSetItemDataInternal = FALSE;
	if (pData == NULL)
	{
		return -7;
	}

	HICON hIcon = ImageList_ExtractIcon(NULL, pMember->m_hImageList, nImage);
	if (hIcon == NULL)
	{
		return -9;
	}

	CRect rcImage = ii.rcImage;

	CRect rcHeaderItem;
	pMember->m_wndHeader.GetItemRect(nSubItem, rcHeaderItem);
	pMember->m_wndHeader.ClientToScreen(rcHeaderItem);
	ScreenToClient(rcHeaderItem);

	CRect rcIcon;
	GetItemRect(nItem, rcIcon, LVIR_ICON);
	int nYOffset = (rcIcon.Height() - rcImage.Height()) / 2;
	if (nYOffset < 0)
	{
		nYOffset = 0;
	}

	int nVisibleIconCX = min(rcImage.Width(), rcHeaderItem.Width());

	CPoint pt(rcHeaderItem.left + 1, rcIcon.top + nYOffset);
	CSize size(nVisibleIconCX, rcImage.Height());

	*lpRect = CRect(pt, size);

	return 0;
}

BOOL CSkinListCtrl::SetItemTextEx(int nItem, int nSubItem, LPCTSTR lpszText, int nImageEx)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (nSubItem == 0)
	{
		return FALSE;
	}

	BOOL bRet = SetItemText(nItem, nSubItem, lpszText);
	if (!bRet)
	{
		return FALSE;
	}

	pMember->m_bSetItemDataInternal = TRUE;
	LV_ITEMDATA *pData = (LV_ITEMDATA *)GetItemData(nItem);
	pMember->m_bSetItemDataInternal = FALSE;

	// If not set, new SSubItemBindObject
	std::map<int, SItemBindObject *>::iterator it = pData->mapSubItemObject.find(nSubItem);
	if (it == pData->mapSubItemObject.end())
	{
		SItemBindObject *pBindObject = new SItemBindObject;

		pData->mapSubItemObject[nSubItem] = pBindObject;
	}

	it = pData->mapSubItemObject.find(nSubItem);
	ASSERT(it != pData->mapSubItemObject.end());

	it->second->nImage = nImageEx;

	return bRet;
}

// If failed, must return -1.
int CSkinListCtrl::GetItemImage(int nIndex, int nSubItem) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	// The first column get from LVSIL_SMALL CImageList.
	if (nSubItem == 0)
	{
		LVITEM lvi;
		lvi.mask = LVIF_IMAGE;
		lvi.iItem = nIndex;
		lvi.iSubItem = nSubItem;

		BOOL bRet = GetItem(&lvi);
		if (!bRet)
		{
			return -1;
		}

		return lvi.iImage;
	}

	int nCount = GetItemCount();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return -1;
	}

	pMember->m_bSetItemDataInternal = TRUE;
	LV_ITEMDATA *pData = (LV_ITEMDATA *)GetItemData(nIndex);
	pMember->m_bSetItemDataInternal = FALSE;

	if (pData == NULL)
	{
		return -1;
	}

	std::map<int, SItemBindObject *>::iterator it = pData->mapSubItemObject.find(nSubItem);
	if (it == pData->mapSubItemObject.end())	// Not found
	{
		return -1;
	}

	int nImage = it->second->nImage;

	return nImage;
}

int CSkinListCtrl::SetItemControl(int nItem, int nSubItem, CWLWnd *pWLItem)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (nSubItem == 0)
	{
		return FALSE;
	}

	pMember->m_bSetItemDataInternal = TRUE;
	LV_ITEMDATA *pData = (LV_ITEMDATA *)GetItemData(nItem);
	pMember->m_bSetItemDataInternal = FALSE;

	if (pData == NULL)
	{
		pData = new LV_ITEMDATA;
		pData->lParam = 0;

		pMember->m_bSetItemDataInternal = TRUE;
		SetItemData(nItem, (DWORD_PTR)pData);
		pMember->m_bSetItemDataInternal = FALSE;
	}

	// Destroy old.
	SItemBindObject *pBindObject = NULL;
	std::map<int, SItemBindObject *>::iterator it = pData->mapSubItemObject.find(nSubItem);
	if (it == pData->mapSubItemObject.end())
	{
		pBindObject = new SItemBindObject;

		pData->mapSubItemObject[nSubItem] = pBindObject;

		it = pData->mapSubItemObject.find(nSubItem);
		ASSERT(it != pData->mapSubItemObject.end());
	}
	else
	{
		SItemBindObject *pSubBindObject = it->second;

		pSubBindObject->ReleaseChildren(TRUE);
	}

	ASSERT(pBindObject != NULL);
	CChildInfo ci;
	ci.m_pChildCtrl = (CWnd *)pWLItem;
	pBindObject->vSubWLWnd.push_back(ci);

	// Delay create the windowless control

	return 0;
}

// The bind window ID will create control, and store in LV_ITEMDATA::mapWLWnd at first become visible.
int CSkinListCtrl::SetItemControl(int nItem, int nSubItem, UINT uBindWndID)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (nSubItem == 0)
	{
		return FALSE;
	}

	pMember->m_bSetItemDataInternal = TRUE;
	LV_ITEMDATA *pData = (LV_ITEMDATA *)GetItemData(nItem);
	pMember->m_bSetItemDataInternal = FALSE;

	if (pData == NULL)
	{
		pData = new LV_ITEMDATA;
		pData->lParam = 0;

		pMember->m_bSetItemDataInternal = TRUE;
		SetItemData(nItem, (DWORD_PTR)pData);
		pMember->m_bSetItemDataInternal = FALSE;
	}

	// Destroy old.
	SItemBindObject *pBindObject = NULL;
	std::map<int, SItemBindObject *>::iterator it = pData->mapSubItemObject.find(nSubItem);
	if (it == pData->mapSubItemObject.end())
	{
		pBindObject = new SItemBindObject;

		pData->mapSubItemObject[nSubItem] = pBindObject;

		it = pData->mapSubItemObject.find(nSubItem);
		ASSERT(it != pData->mapSubItemObject.end());
	}
	else
	{
		SItemBindObject *pSubBindObject = it->second;

		pSubBindObject->ReleaseChildren(TRUE);
	}

	ASSERT(pBindObject != NULL);
	pBindObject->uBindWndID = uBindWndID;

	// Delay create the windowless control

	return 0;
}

CWLWnd *CSkinListCtrl::GetItemControl(int nItem, int nSubItem) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (nSubItem == 0)
	{
		return FALSE;
	}

	pMember->m_bSetItemDataInternal = TRUE;
	LV_ITEMDATA *pData = (LV_ITEMDATA *)GetItemData(nItem);
	pMember->m_bSetItemDataInternal = FALSE;

	if (pData == NULL)
	{
		return NULL;
	}

	std::map<int, SItemBindObject *>::iterator it = pData->mapSubItemObject.find(nSubItem);
	if (it == pData->mapSubItemObject.end())
	{
		return NULL;
	}

	return NULL;
}

UINT CSkinListCtrl::GetItemBindWndID(int nItem, int nSubItem) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (nSubItem == 0)
	{
		return -1;
	}

	pMember->m_bSetItemDataInternal = TRUE;
	LV_ITEMDATA *pData = (LV_ITEMDATA *)GetItemData(nItem);
	pMember->m_bSetItemDataInternal = FALSE;

	if (pData == NULL)
	{
		return -1;
	}

	std::map<int, SItemBindObject *>::iterator it = pData->mapSubItemObject.find(nSubItem);
	if (it == pData->mapSubItemObject.end())
	{
		return -1;
	}

	return -1;
}

int CSkinListCtrl::EnableBkImage(BOOL bEnable)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_bUseBitmap = bEnable;

	if (pMember->m_bCacheMode)
	{
		ReDrawControl(TRUE, FALSE);
	}
	else
	{
		Invalidate();
	}

	return 0;
}

BOOL CSkinListCtrl::IsEnableBkImage() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_bUseBitmap;
}

// Set the images for background
int CSkinListCtrl::SetImages(UINT uMask, LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameD)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	m_bBkCombine = false;
	IUISetControlImage2(uMask, pMember->m_himgBk, lpszImageNameN, lpszImageNameD);

	return 0;
}

// Get the images for background
int CSkinListCtrl::GetImages(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameD) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bBkCombine;
	IUIGetControlImage2(uMask, m_himgCombineBk, pMember->m_himgBk, *pbCombineImage,
		pstrCombineImageName, pstrImageNameN, pstrImageNameD);

	return 0;
}

int CSkinListCtrl::ShowGridLine(BOOL bShow)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	DWORD dwStyle = GetExtendedStyle();
	SetExtendedStyle(dwStyle | LVS_EX_GRIDLINES);

	return 0;
}

BOOL CSkinListCtrl::IsGridLineVisible()
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	DWORD dwStyle = GetExtendedStyle();
	BOOL bShowGridLine = ((dwStyle & LVS_EX_GRIDLINES) == LVS_EX_GRIDLINES);
	return bShowGridLine;
}

int CSkinListCtrl::DrawFirstRow(BOOL bDraw0Row)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_bDraw0Row = bDraw0Row;

	return 0;
}

BOOL CSkinListCtrl::IsDrawFirstRow() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_bDraw0Row;
}

int CSkinListCtrl::GridVerLineToFirstRow(BOOL bTo0Row)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_bVerLineTo0Row = bTo0Row;

	return 0;
}

BOOL CSkinListCtrl::IsGridVerLineToFirstRow() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_bVerLineTo0Row;
}

int CSkinListCtrl::SetGridLineOffset(const LPPOINT lpPtOffset)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (lpPtOffset == NULL)
	{
		return -1;
	}

	pMember->m_ptLineOffset = *lpPtOffset;

	return 0;
}

int CSkinListCtrl::GetGridLineOffset(LPPOINT lpPtOffset) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (lpPtOffset == NULL)
	{
		return -1;
	}

	*lpPtOffset = pMember->m_ptLineOffset;

	return 0;
}

int CSkinListCtrl::SetGridLineColor(COLORREF crGridLine)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_crGridLine = crGridLine;

	return 0;
}

COLORREF CSkinListCtrl::GetGridLineColor() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_crGridLine;
}

int CSkinListCtrl::EnableSelectedFitClient(BOOL bEnable)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_bEnableSelectedFitClient = bEnable;
	return 0;
}

BOOL CSkinListCtrl::IsEnableSelectedFitClient() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_bEnableSelectedFitClient;
}

int CSkinListCtrl::SetSelectedItemBkType(BACKGROUND_TYPE eBkType)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_lvpItemS.m_eBkType = eBkType;

	return 0;
}

BACKGROUND_TYPE CSkinListCtrl::GetSelectedItemBkType() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_lvpItemS.m_eBkType;
}

int CSkinListCtrl::SetSelectedItemBkColor(COLORREF cr, BOOL bRedraw/* = TRUE*/)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_lvpItemS.m_crItemBk = cr;

	if (bRedraw)
	{
		if (pMember->m_bCacheMode)
		{
			ReDrawControl(FALSE, FALSE);
		}
		else
		{
			Invalidate();
		}
	}

	return 0;
}

COLORREF CSkinListCtrl::GetSelectedItemBkColor() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_lvpItemS.m_crItemBk;
}

int CSkinListCtrl::SetSelectedItemBkImages(LPCTSTR lpszImageName)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return IUISetControlImage(&pMember->m_lvpItemS.m_hItemBkBmp, lpszImageName);
}

int CSkinListCtrl::GetSelectedItemBkImages(CString *pstrImageName)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return IUIGetControlImage(pMember->m_lvpItemS.m_hItemBkBmp, pstrImageName);
}

int CSkinListCtrl::SetSelectedItemTextColor(COLORREF cr)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_lvpItemS.m_crItemText = cr;
	return 0;
}

COLORREF CSkinListCtrl::GetSelectedItemTextColor() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_lvpItemS.m_crItemText;
}

int CSkinListCtrl::EnableNormalDisabledItemEffect(BOOL bEnable)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_bEnableNormalDisabledItemEffect = bEnable;

	return 0;
}

BOOL CSkinListCtrl::IsEnableNormalDisabledItemEffect() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_bEnableNormalDisabledItemEffect;
}

int CSkinListCtrl::SetNormalDisabledItemBkType(BACKGROUND_TYPE eBkType)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_lvpItemND.m_eBkType = eBkType;

	return 0;
}

BACKGROUND_TYPE CSkinListCtrl::GetNormalDisabledItemBkType() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_lvpItemND.m_eBkType;
}

int CSkinListCtrl::SetNormalDisabledItemBkColor(COLORREF cr)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_lvpItemND.m_crItemBk = cr;

	return 0;
}

COLORREF CSkinListCtrl::GetNormalDisabledItemBkColor() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_lvpItemND.m_crItemBk;
}

int CSkinListCtrl::SetNormalDisabledItemBkImages(LPCTSTR lpszImageName)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return IUISetControlImage(&pMember->m_lvpItemND.m_hItemBkBmp, lpszImageName);
}

int CSkinListCtrl::GetNormalDisabledItemBkImages(CString *pstrImageName)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	IUIGetControlImage(pMember->m_lvpItemND.m_hItemBkBmp, pstrImageName);

	return 0;
}

int CSkinListCtrl::SetNormalDisabledItemTextColor(COLORREF cr)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_lvpItemND.m_crItemText = cr;

	return 0;
}

COLORREF CSkinListCtrl::GetNormalDisabledItemTextColor() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_lvpItemND.m_crItemText;
}

int CSkinListCtrl::EnableSelectedDisabledItemEffect(BOOL bEnable)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_bEnableSelectedDisabledItemEffect = bEnable;

	return 0;
}

BOOL CSkinListCtrl::IsEnableSelectedDisabledItemEffect() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_bEnableSelectedDisabledItemEffect;
}

int CSkinListCtrl::SetSelectedDisabledItemBkType(BACKGROUND_TYPE eBkType)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_lvpItemSD.m_eBkType = eBkType;

	return 0;
}

BACKGROUND_TYPE CSkinListCtrl::GetSelectedDisabledItemBkType() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_lvpItemSD.m_eBkType;
}

int CSkinListCtrl::SetSelectedDisabledItemBkColor(COLORREF cr)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_lvpItemSD.m_crItemBk = cr;

	return 0;
}

COLORREF CSkinListCtrl::GetSelectedDisabledItemBkColor() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_lvpItemSD.m_crItemBk;
}

int CSkinListCtrl::SetSelectedDisabledItemBkImages(LPCTSTR lpszImageName)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return IUISetControlImage(&pMember->m_lvpItemSD.m_hItemBkBmp, lpszImageName);
}

int CSkinListCtrl::GetSelectedDisabledItemBkImages(CString *pstrImageName)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return IUIGetControlImage(pMember->m_lvpItemSD.m_hItemBkBmp, pstrImageName);
}

int CSkinListCtrl::SetSelectedDisabledItemTextColor(COLORREF cr)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_lvpItemSD.m_crItemText = cr;

	return 0;
}

COLORREF CSkinListCtrl::GetSelectedDisabledItemTextColor() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_lvpItemSD.m_crItemText;
}

int CSkinListCtrl::EnableHighlightItemEffect(BOOL bEnable)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_bEnableHighlightItemEffect = bEnable;
	return 0;
}

BOOL CSkinListCtrl::IsEnableHighlightItemEffect() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_bEnableHighlightItemEffect;
}

int CSkinListCtrl::SetHighlightItemBkType(BACKGROUND_TYPE eBkType)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_lvpItemH.m_eBkType = eBkType;

	return 0;
}

BACKGROUND_TYPE CSkinListCtrl::GetHighlightItemBkType() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_lvpItemH.m_eBkType;
}

// valid after call EnableOwnerDraw(TRUE)
int CSkinListCtrl::SetHighlightItemBkColor(COLORREF cr, BOOL bRedraw/* = TRUE*/)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_lvpItemH.m_crItemBk = cr;

	if (bRedraw)
	{
		if (pMember->m_bCacheMode)
		{
			ReDrawControl(FALSE, FALSE);
		}
		else
		{
			Invalidate();
		}
	}

	return 0;
}

COLORREF CSkinListCtrl::GetHighlightItemBkColor() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_lvpItemH.m_crItemBk;
}

int CSkinListCtrl::SetHighlightItemBkImages(LPCTSTR lpszImageName)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return IUISetControlImage(&pMember->m_lvpItemH.m_hItemBkBmp, lpszImageName);
}

int CSkinListCtrl::GetHighlightItemBkImages(CString *pstrImageName)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return IUIGetControlImage(pMember->m_lvpItemH.m_hItemBkBmp, pstrImageName);
}

int CSkinListCtrl::SetHighlightItemTextColor(COLORREF cr)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_lvpItemH.m_crItemText = cr;

	return 0;
}

COLORREF CSkinListCtrl::GetHighlightItemTextColor() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_lvpItemH.m_crItemText;
}

int CSkinListCtrl::GetHighlightItem() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_nCurHighlightItem;
}

int CSkinListCtrl::SetItemTextFont(LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDH, LPCTSTR lpszFontIDS, LPCTSTR lpszFontIDND, LPCTSTR lpszFontIDSD)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	ASSERT(::IsWindow(m_hWnd));

	if (lpszFontIDN != NULL)
	{
		ReleaseIUIFontInternal(pMember->m_lvpItemN.m_hIUIFont);
		pMember->m_lvpItemN.m_hIUIFont = CreateIUIFontInternal(lpszFontIDN);
	}

	if (lpszFontIDH != NULL)
	{
		ReleaseIUIFontInternal(pMember->m_lvpItemH.m_hIUIFont);
		pMember->m_lvpItemH.m_hIUIFont = CreateIUIFontInternal(lpszFontIDH);
	}
	if (lpszFontIDS != NULL)
	{
		ReleaseIUIFontInternal(pMember->m_lvpItemS.m_hIUIFont);
		pMember->m_lvpItemS.m_hIUIFont = CreateIUIFontInternal(lpszFontIDS);
	}

	if (lpszFontIDND != NULL)
	{
		ReleaseIUIFontInternal(pMember->m_lvpItemND.m_hIUIFont);
		pMember->m_lvpItemND.m_hIUIFont = CreateIUIFontInternal(lpszFontIDND);
	}

	if (lpszFontIDSD != NULL)
	{
		ReleaseIUIFontInternal(pMember->m_lvpItemSD.m_hIUIFont);
		pMember->m_lvpItemSD.m_hIUIFont = CreateIUIFontInternal(lpszFontIDSD);
	}

	HFONT hFont = GetHFont(IsWindowEnabled() ? pMember->m_lvpItemN.m_hIUIFont : pMember->m_lvpItemND.m_hIUIFont);
	CString strDefaultFont;
	if (hFont == NULL)
	{
		strDefaultFont = CUIMgr::GetDefaultFont();
		hFont = CreateIUIFont(strDefaultFont);
		ReleaseIUIFont(strDefaultFont);
	}

	::SendMessage(m_hWnd, WM_SETFONT, (WPARAM)hFont, TRUE);

	if (strDefaultFont.IsEmpty())
	{
		ReleaseIUIFont(strDefaultFont);
	}

	return 0;
}

int CSkinListCtrl::GetItemTextFont(CString *pstrFontIDN, CString *pstrFontIDH, CString *pstrFontIDS,
	CString *pstrFontIDND, CString *pstrFontIDSD) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (pstrFontIDN != NULL)
	{
		GetFontResID(pMember->m_lvpItemN.m_hIUIFont, pstrFontIDN);
	}
	if (pstrFontIDH != NULL)
	{
		GetFontResID(pMember->m_lvpItemH.m_hIUIFont, pstrFontIDH);
	}
	if (pstrFontIDS != NULL)
	{
		GetFontResID(pMember->m_lvpItemS.m_hIUIFont, pstrFontIDS);
	}
	if (pstrFontIDND != NULL)
	{
		GetFontResID(pMember->m_lvpItemND.m_hIUIFont, pstrFontIDND);
	}
	if (pstrFontIDSD != NULL)
	{
		GetFontResID(pMember->m_lvpItemSD.m_hIUIFont, pstrFontIDSD);
	}

	return 0;
}

int CSkinListCtrl::SetThumbnailView(BOOL bThumbnailView)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	// Only icon style can be as thumbnail view.
	LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	if ((lStyle & LVS_ICON) != LVS_ICON)
	{
		return -1;
	}

	pMember->m_bThumbnailView = bThumbnailView;

	return 0;
}

BOOL CSkinListCtrl::IsThumbnailView() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_bThumbnailView;
}

int CSkinListCtrl::SetThumbnailItemTotalSize(SIZE sizeItem)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	ListView_SetIconSpacing(m_hWnd, sizeItem.cx, sizeItem.cy);

	return 0;
}

int CSkinListCtrl::GetThumbnailItemTotalSize(LPSIZE lpSizeItem) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (lpSizeItem == NULL)
	{
		return -1;
	}

	GetItemSpacing(FALSE, (int *)&lpSizeItem->cx, (int *)&lpSizeItem->cy);

	return 0;
}

int CSkinListCtrl::EnableNormalItemEffect(BOOL bEnable)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_bEnableNormalItemEffect = bEnable;

	return 0;
}

BOOL CSkinListCtrl::IsEnableNormalItemEffect() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_bEnableNormalItemEffect;
}

int CSkinListCtrl::SetNormalItemBkType(BACKGROUND_TYPE eBkType)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_lvpItemN.m_eBkType = eBkType;

	return 0;
}

BACKGROUND_TYPE CSkinListCtrl::GetNormalItemBkType() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_lvpItemN.m_eBkType;
}

int CSkinListCtrl::SetNormalItemBkColor(COLORREF cr)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_lvpItemN.m_crItemBk = cr;

	return 0;
}

COLORREF CSkinListCtrl::GetNormalItemBkColor() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_lvpItemN.m_crItemBk;
}

int CSkinListCtrl::SetNormalItemBkImages(LPCTSTR lpszImageName)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return IUISetControlImage(&pMember->m_lvpItemN.m_hItemBkBmp, lpszImageName);
}

int CSkinListCtrl::GetNormalItemBkImages(CString *pstrImageName) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return IUIGetControlImage(pMember->m_lvpItemN.m_hItemBkBmp, pstrImageName);
}

int CSkinListCtrl::SetNormalItemTextColor(COLORREF cr)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_lvpItemN.m_crItemText = cr;

	return 0;
}

COLORREF CSkinListCtrl::GetNormalItemTextColor() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_lvpItemN.m_crItemText;
}

int CSkinListCtrl::SetThumbnailItemBkPosition(LPCRECT lprcPositionN, LPCRECT lprcPositionH, LPCRECT lprcPositionS, LPCRECT lprcPositionD)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_rcThumbnailBkPosition[0] = *lprcPositionN;
	pMember->m_rcThumbnailBkPosition[1] = *lprcPositionH;
	pMember->m_rcThumbnailBkPosition[2] = *lprcPositionS;
	pMember->m_rcThumbnailBkPosition[3] = *lprcPositionD;

	return 0;
}

int CSkinListCtrl::GetThumbnailItemBkPosition(LPRECT lprcPositionN, LPRECT lprcPositionH, LPRECT lprcPositionS, LPRECT lprcPositionD) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (lprcPositionN != NULL)
	{
		*lprcPositionN = pMember->m_rcThumbnailBkPosition[0];
	}
	if (lprcPositionH != NULL)
	{
		*lprcPositionH = pMember->m_rcThumbnailBkPosition[1];
	}
	if (lprcPositionS != NULL)
	{
		*lprcPositionS = pMember->m_rcThumbnailBkPosition[2];
	}
	if (lprcPositionD != NULL)
	{
		*lprcPositionD = pMember->m_rcThumbnailBkPosition[3];
	}

	return 0;
}

int CSkinListCtrl::SetThumbnailItemPosition(LPCRECT lprcPositionN, LPCRECT lprcPositionH, LPCRECT lprcPositionS, LPCRECT lprcPositionD)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_rcThumbnailPosition[0] = *lprcPositionN;
	pMember->m_rcThumbnailPosition[1] = *lprcPositionH;
	pMember->m_rcThumbnailPosition[2] = *lprcPositionS;
	pMember->m_rcThumbnailPosition[3] = *lprcPositionD;

	return 0;
}

int CSkinListCtrl::GetThumbnailItemPosition(LPRECT lprcPositionN, LPRECT lprcPositionH, LPRECT lprcPositionS, LPRECT lprcPositionD) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (lprcPositionN != NULL)
	{
		*lprcPositionN = pMember->m_rcThumbnailPosition[0];
	}
	if (lprcPositionH != NULL)
	{
		*lprcPositionH = pMember->m_rcThumbnailPosition[1];
	}
	if (lprcPositionS != NULL)
	{
		*lprcPositionS = pMember->m_rcThumbnailPosition[2];
	}
	if (lprcPositionD != NULL)
	{
		*lprcPositionD = pMember->m_rcThumbnailPosition[3];
	}

	return 0;
}

int CSkinListCtrl::SetThumbnailItemLabelPosition(LPCRECT lprcPositionN, LPCRECT lprcPositionH, LPCRECT lprcPositionS, LPCRECT lprcPositionD)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_rcThumbnailLabelPosition[0] = *lprcPositionN;
	pMember->m_rcThumbnailLabelPosition[1] = *lprcPositionH;
	pMember->m_rcThumbnailLabelPosition[2] = *lprcPositionS;
	pMember->m_rcThumbnailLabelPosition[3] = *lprcPositionD;

	return 0;
}

int CSkinListCtrl::GetThumbnailItemLabelPosition(LPRECT lprcPositionN, LPRECT lprcPositionH, LPRECT lprcPositionS, LPRECT lprcPositionD) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (lprcPositionN != NULL)
	{
		*lprcPositionN = pMember->m_rcThumbnailLabelPosition[0];
	}
	if (lprcPositionH != NULL)
	{
		*lprcPositionH = pMember->m_rcThumbnailLabelPosition[1];
	}
	if (lprcPositionS != NULL)
	{
		*lprcPositionS = pMember->m_rcThumbnailLabelPosition[2];
	}
	if (lprcPositionD != NULL)
	{
		*lprcPositionD = pMember->m_rcThumbnailLabelPosition[3];
	}

	return 0;
}

int CSkinListCtrl::SetThumbnailItemIconPosition(LPCRECT lprcPositionN, LPCRECT lprcPositionH, LPCRECT lprcPositionS, LPCRECT lprcPositionD)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_rcThumbnailIconPosition[0] = *lprcPositionN;
	pMember->m_rcThumbnailIconPosition[1] = *lprcPositionH;
	pMember->m_rcThumbnailIconPosition[2] = *lprcPositionS;
	pMember->m_rcThumbnailIconPosition[3] = *lprcPositionD;

	return 0;
}

int CSkinListCtrl::GetThumbnailItemIconPosition(LPRECT lprcPositionN, LPRECT lprcPositionH, LPRECT lprcPositionS, LPRECT lprcPositionD) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (lprcPositionN != NULL)
	{
		*lprcPositionN = pMember->m_rcThumbnailIconPosition[0];
	}
	if (lprcPositionH != NULL)
	{
		*lprcPositionH = pMember->m_rcThumbnailIconPosition[1];
	}
	if (lprcPositionS != NULL)
	{
		*lprcPositionS = pMember->m_rcThumbnailIconPosition[2];
	}
	if (lprcPositionD != NULL)
	{
		*lprcPositionD = pMember->m_rcThumbnailIconPosition[3];
	}

	return 0;
}

int CSkinListCtrl::GetThumbnailItemTotalRect(int nItem, LPRECT lpRectTotal) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (lpRectTotal == NULL)
	{
		return -1;
	}

	if (!pMember->m_bThumbnailView)
	{
		return -2;
	}

	LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	if ((lStyle & LVS_ICON) != LVS_ICON)
	{
		return -1;
	}

	CRect rcItem;
	pMember->m_bGetItemRectInternal = TRUE;
	GetItemRect(nItem, rcItem, LVIR_BOUNDS);
	pMember->m_bGetItemRectInternal = FALSE;

	int nWidth = 0;
	int nHeight = 0;
	GetItemSpacing(FALSE, &nWidth, &nHeight);

	rcItem.left = rcItem.left + (rcItem.Width() - nWidth) / 2;
	rcItem.right = rcItem.left + nWidth;
	rcItem.bottom = rcItem.top + nHeight;

	*lpRectTotal = rcItem;

	return 0;
}

int CSkinListCtrl::GetThumbnailItemRect(int nItem, LPRECT lpRect, UINT nCode, CONTROL_STATE eControlState) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (!pMember->m_bThumbnailView)
	{
		return -1;
	}

	if (nItem < 0 || lpRect == NULL)
	{
		return -2;
	}

	int nCount = GetItemCount();
	if (nItem >= nCount)
	{
		return -2;
	}

	CRect rcItemTotal;
	int nRet = GetThumbnailItemTotalRect(nItem, rcItemTotal);
	if (nRet != 0)
	{
		return -3;
	}

	CRect rcItem;
	if (eControlState == CONTROL_STATE_NORMAL)
	{
		if (nCode == LVIR_BOUNDS)
		{
			rcItem = pMember->m_rcThumbnailBkPosition[0];
			rcItem.OffsetRect(rcItemTotal.TopLeft());
		}
		else if (nCode == LVIR_ICON)
		{
			rcItem = pMember->m_rcThumbnailPosition[0];
			rcItem.OffsetRect(rcItemTotal.TopLeft());
		}
		else if (nCode == LVIR_LABEL)
		{
			rcItem = pMember->m_rcThumbnailLabelPosition[0];
			rcItem.OffsetRect(rcItemTotal.TopLeft());
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if (eControlState == CONTROL_STATE_HIGHLIGHT)
	{
		if (nCode == LVIR_BOUNDS)
		{
			rcItem = pMember->m_rcThumbnailBkPosition[1];
			rcItem.OffsetRect(rcItemTotal.TopLeft());
		}
		else if (nCode == LVIR_ICON)
		{
			rcItem = pMember->m_rcThumbnailPosition[1];
			rcItem.OffsetRect(rcItemTotal.TopLeft());
		}
		else if (nCode == LVIR_LABEL)
		{
			rcItem = pMember->m_rcThumbnailLabelPosition[1];
			rcItem.OffsetRect(rcItemTotal.TopLeft());
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if (eControlState == CONTROL_STATE_SELECTED)
	{
		if (nCode == LVIR_BOUNDS)
		{
			rcItem = pMember->m_rcThumbnailBkPosition[2];
			rcItem.OffsetRect(rcItemTotal.TopLeft());
		}
		else if (nCode == LVIR_ICON)
		{
			rcItem = pMember->m_rcThumbnailPosition[2];
			rcItem.OffsetRect(rcItemTotal.TopLeft());
		}
		else if (nCode == LVIR_LABEL)
		{
			rcItem = pMember->m_rcThumbnailLabelPosition[2];
			rcItem.OffsetRect(rcItemTotal.TopLeft());
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if (eControlState == CONTROL_STATE_DISABLED)
	{
		if (nCode == LVIR_BOUNDS)
		{
			rcItem = pMember->m_rcThumbnailBkPosition[3];
			rcItem.OffsetRect(rcItemTotal.TopLeft());
		}
		else if (nCode == LVIR_ICON)
		{
			rcItem = pMember->m_rcThumbnailPosition[3];
			rcItem.OffsetRect(rcItemTotal.TopLeft());
		}
		else if (nCode == LVIR_LABEL)
		{
			rcItem = pMember->m_rcThumbnailLabelPosition[3];
			rcItem.OffsetRect(rcItemTotal.TopLeft());
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	*lpRect = rcItem;

	return 0;
}

int CSkinListCtrl::SetTag(LPCTSTR lpszTag)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (lpszTag == NULL)
	{
		return -1;
	}

	pMember->m_strTag = lpszTag;

	return 0;
}

int CSkinListCtrl::GetTag(CString &strTag) const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	strTag = pMember->m_strTag;

	return 0;
}

int CSkinListCtrl::UseCustomScrollBar(BOOL bUseCustomScrollBar)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

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

BOOL CSkinListCtrl::IsUseCustomScrollBar() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	return pMember->m_bUseCustomScrollBar;
}

CScrollBarEx *CSkinListCtrl::GetHorzScrollBarEx() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (pMember->m_bUseCustomScrollBar)
	{
		return &pMember->m_wndHScroll;
	}

	return NULL;
}

CScrollBarEx *CSkinListCtrl::GetVertScrollBarEx() const
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (pMember->m_bUseCustomScrollBar)
	{
		return &pMember->m_wndVScroll;
	}

	return NULL;
}

// update scroll information and position
int CSkinListCtrl::UpdateCustomScrollBar()
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	if (!pMember->m_bUseCustomScrollBar)
	{
		return -1;
	}

	BOOL bRet = FALSE;
	int nVScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
	int nHScrollBarHeight = GetSystemMetrics(SM_CYHSCROLL);
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
		rgnV.CreateRectRgn(rect.Width() - nVScrollBarWidth, 0, rect.Width(), rect.Height() - nHScrollBarHeight);
		CRgn rgnH;
		rgnH.CreateRectRgn(0, rect.Height() - nHScrollBarHeight, rect.Width() - nVScrollBarWidth, rect.Height());
		CRgn rgnAll;
		rgnAll.CreateRectRgn(0, 0, rect.Width(), rect.Height());
		rgnAll.CombineRgn(&rgnAll, &rgnV, RGN_DIFF);
		rgnAll.CombineRgn(&rgnAll, &rgnH, RGN_DIFF);
		SetWindowRgn(HRGN(rgnAll.GetSafeHandle()), TRUE);

		GetParent()->ScreenToClient(rect);
		pMember->m_wndVScroll.MoveWindow(rect.right - nVScrollBarWidth, rect.top, nVScrollBarWidth, rect.Height() - nHScrollBarHeight);
		if (bNotifyWndShow)
		{
			pMember->m_wndVScroll.ShowWindow(SW_SHOW);
		}

		pMember->m_wndHScroll.MoveWindow(rect.left, rect.bottom - nHScrollBarHeight, rect.Width() - nVScrollBarWidth, nHScrollBarHeight);
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
		rgnV.CreateRectRgn(rect.Width() - nVScrollBarWidth, 0, rect.Width(), rect.Height());
		CRgn rgnAll;
		rgnAll.CreateRectRgn(0, 0, rect.Width(), rect.Height());
		rgnAll.CombineRgn(&rgnAll, &rgnV, RGN_DIFF);
		SetWindowRgn(HRGN(rgnAll.GetSafeHandle()), TRUE);

		GetParent()->ScreenToClient(rect);
		pMember->m_wndVScroll.MoveWindow(rect.right - nVScrollBarWidth, rect.top, nVScrollBarWidth, rect.Height());
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
		rgnH.CreateRectRgn(0, rect.Height() - nHScrollBarHeight, rect.Width(), rect.Height());
		CRgn rgnAll;
		rgnAll.CreateRectRgn(0, 0, rect.Width(), rect.Height());
		rgnAll.CombineRgn(&rgnAll, &rgnH, RGN_DIFF);
		SetWindowRgn(HRGN(rgnAll.GetSafeHandle()), TRUE);

		GetParent()->ScreenToClient(rect);
		pMember->m_wndHScroll.MoveWindow(rect.left, rect.bottom - nHScrollBarHeight, rect.Width(), nHScrollBarHeight);
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

int CSkinListCtrl::AnimateReportContent(std::vector<int> &vAnimationData, int nElapse/*ms*/, BOOL bTogetherAnimate)
{
	LISTCTRLMEMBER *pMember = (LISTCTRLMEMBER *)m_pMember;

	pMember->m_bAnimationMode = true;
	pMember->m_bTogetherAnimation = bTogetherAnimate;
	pMember->m_nCurTimerTick = 0;
	pMember->m_nTimerElapse = nElapse;
	pMember->m_vAnimationData = vAnimationData;

	SetTimer(IDT_WIN8, pMember->m_nTimerElapse, NULL);

	return 0;
}
