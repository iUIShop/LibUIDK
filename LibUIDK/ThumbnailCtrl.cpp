// ThumbnailCtrl.cpp : implementation file
//
#include "StdAfx.h"
#pragma comment (lib, "Msimg32.lib") // for API AlphaBlend
#include <AFXCMN.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct THUMBNAIL_ITEMDATA
{
	THUMBNAIL_ITEMDATA()
	{
		hbmpThumbnail = NULL;
		dwDragDropCompareData = 0;
		dwData = 0;
	}
	HBITMAP hbmpThumbnail;
	DWORD dwDragDropCompareData; // always is the item's index
#if(_MSC_VER <= 1200) // VC6.0
	DWORD dwData;
#else
	DWORD_PTR dwData;
#endif
	CString strTooltip;
};


void SetRectFromPoints(LPRECT rc, POINT pt1, POINT pt2)
{
	rc->left = min(pt1.x, pt2.x);
	rc->right = max(pt1.x, pt2.x);

	rc->top = min(pt1.y, pt2.y);
	rc->bottom = max(pt1.y, pt2.y);
}

BOOL InsideRect(CRect Rect1, CRect Rect2)
{
	CPoint pt1, pt2, pt3, pt4;

	pt1 = CPoint(Rect1.left, Rect1.top);
	pt2 = CPoint(Rect1.right, Rect1.top);
	pt3 = CPoint(Rect1.left, Rect1.bottom);
	pt4 = CPoint(Rect1.right, Rect1.bottom);

	if (Rect2.PtInRect(pt1) && Rect2.PtInRect(pt2) && Rect2.PtInRect(pt3) && Rect2.PtInRect(pt4))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

struct THUMBANILMEMBER
{
	THUMBANILMEMBER()
	{
		m_himgBk = NULL;
	}

	~THUMBANILMEMBER()
	{
		ReleaseIUIImage(m_himgBk);
		m_himgBk = NULL;
	}

	BOOL m_bListStyle;
	BOOL m_bSingleSel;
	// insure call the
	BOOL m_bIsCallSetSize;
	// insure call the SetPosition
	BOOL m_bIsCallSetPosition;

	// support the drag select items
	CPoint m_ptCurDragSelectPoint;
	BOOL  m_bIsDragSelect;

	//Support Drag and Drop items
	CIUIOleDropTarget m_dropTarget;
	BOOL m_bDragDropEnable;
	BOOL m_bDragItem;
	BOOL m_bMouseFirstMoveRecorded;
	CRect m_rcBack;
	UINT m_uClipFormat;

	// Support select items by Keyboard
	int m_nShiftStart;
	int m_nSelectionMark;

	int m_nHotItem;

	// for scroll
	CScrollBarEx m_wndScrollEx;
	BOOL m_bIsEnableScrollBar;
	int m_nLinesPerItem;		// how much lines an item occupy
	BOOL m_bIsCurShowScroll;
	int m_nScrollBarWidth;		// the width of scroll, default is 16

	// data
	CListCtrl m_list;

	// thumbnail attrib
	CBitmap m_pBmpEmpty;		// the background bitmap of not selected
	CBitmap m_pBmpSelected;		// the background bitmap of selected
	CBitmap m_pBmpHighlight;	// the background bitmap of highlight
	CBitmap m_pBmpDisabled;		// the background bitmap of disabled
	CSize m_sizeTotal;			// the total size of an item
	CSize m_sizeBkImage;		// the background size
	CSize m_sizeHighlight;		// the highlight image size
	int m_nItemHeight;			// the height of an item for only List style, default to 20
	CFont m_ftLabel;			// the font of the item label.

	CSize m_sizeThumbnail;		// the thumbnail size

	CRect m_rcThumbnail;		// the rect of the thumbnail relative to the background image.
	CRect m_rcLabel;			// the rect of the label relative to the background image.
	CRect m_rcHLThumbnail;		// the rect of the highlight thumbnail relative to the background image.
	CRect m_rcHLLabel;			// the rect of the highlight label relative to the background image.

	COLORREF m_crText;			// the color of label

	BOOL m_bDrawBkColor;
	COLORREF m_crBk;			// the background color
	HIUIIMAGE m_himgBk;			// the background image

	// Ctrl key
	BOOL m_bHoldCtrlKey;
};

#define m_bListStyle (((THUMBANILMEMBER *)m_pMember)->m_bListStyle)
#define m_bSingleSel (((THUMBANILMEMBER *)m_pMember)->m_bSingleSel)
#define m_bIsCallSetSize (((THUMBANILMEMBER *)m_pMember)->m_bIsCallSetSize)
#define m_bIsCallSetPosition (((THUMBANILMEMBER *)m_pMember)->m_bIsCallSetPosition)
#define m_ptCurDragSelectPoint (((THUMBANILMEMBER *)m_pMember)->m_ptCurDragSelectPoint)
#define m_bIsDragSelect (((THUMBANILMEMBER *)m_pMember)->m_bIsDragSelect)
#define m_dropTarget (((THUMBANILMEMBER *)m_pMember)->m_dropTarget)
#define m_bDragDropEnable (((THUMBANILMEMBER *)m_pMember)->m_bDragDropEnable)
#define m_bDragItem (((THUMBANILMEMBER *)m_pMember)->m_bDragItem)
#define m_bMouseFirstMoveRecorded (((THUMBANILMEMBER *)m_pMember)->m_bMouseFirstMoveRecorded)
#define m_rcBack (((THUMBANILMEMBER *)m_pMember)->m_rcBack)
#define m_uClipFormat (((THUMBANILMEMBER *)m_pMember)->m_uClipFormat)
#define m_nShiftStart (((THUMBANILMEMBER *)m_pMember)->m_nShiftStart)
#define m_nSelectionMark (((THUMBANILMEMBER *)m_pMember)->m_nSelectionMark)
#define m_nHotItem (((THUMBANILMEMBER *)m_pMember)->m_nHotItem)
#define m_wndScrollEx (((THUMBANILMEMBER *)m_pMember)->m_wndScrollEx)
#define m_bIsEnableScrollBar (((THUMBANILMEMBER *)m_pMember)->m_bIsEnableScrollBar)
#define m_nShiftStart (((THUMBANILMEMBER *)m_pMember)->m_nShiftStart)
#define m_nLinesPerItem (((THUMBANILMEMBER *)m_pMember)->m_nLinesPerItem)
#define m_bIsCurShowScroll (((THUMBANILMEMBER *)m_pMember)->m_bIsCurShowScroll)
#define m_list (((THUMBANILMEMBER *)m_pMember)->m_list)
#define m_nScrollBarWidth (((THUMBANILMEMBER *)m_pMember)->m_nScrollBarWidth)
#define m_pBmpEmpty (((THUMBANILMEMBER *)m_pMember)->m_pBmpEmpty)
#define m_pBmpSelected (((THUMBANILMEMBER *)m_pMember)->m_pBmpSelected)
#define m_pBmpHighlight (((THUMBANILMEMBER *)m_pMember)->m_pBmpHighlight)
#define m_pBmpDisabled (((THUMBANILMEMBER *)m_pMember)->m_pBmpDisabled)
#define m_sizeTotal (((THUMBANILMEMBER *)m_pMember)->m_sizeTotal)
#define m_sizeBkImage (((THUMBANILMEMBER *)m_pMember)->m_sizeBkImage)
#define m_sizeHighlight (((THUMBANILMEMBER *)m_pMember)->m_sizeHighlight)
#define m_nItemHeight (((THUMBANILMEMBER *)m_pMember)->m_nItemHeight)
#define m_ftLabel (((THUMBANILMEMBER *)m_pMember)->m_ftLabel)
#define m_sizeThumbnail (((THUMBANILMEMBER *)m_pMember)->m_sizeThumbnail)
#define m_rcThumbnail (((THUMBANILMEMBER *)m_pMember)->m_rcThumbnail)
#define m_rcLabel (((THUMBANILMEMBER *)m_pMember)->m_rcLabel)
#define m_rcHLThumbnail (((THUMBANILMEMBER *)m_pMember)->m_rcHLThumbnail)
#define m_rcHLLabel (((THUMBANILMEMBER *)m_pMember)->m_rcHLLabel)
#define m_crText (((THUMBANILMEMBER *)m_pMember)->m_crText)
#define m_bDrawBkColor (((THUMBANILMEMBER *)m_pMember)->m_bDrawBkColor)
#define m_crBk (((THUMBANILMEMBER *)m_pMember)->m_crBk)
#define m_himgBk (((THUMBANILMEMBER *)m_pMember)->m_himgBk)
#define m_bHoldCtrlKey (((THUMBANILMEMBER *)m_pMember)->m_bHoldCtrlKey)


/////////////////////////////////////////////////////////////////////////////
// CThumbnailCtrl

CThumbnailCtrl::CThumbnailCtrl()
{
	m_pBindWnd = this;

	m_pMember = new THUMBANILMEMBER;

	m_bListStyle = FALSE;
	m_bSingleSel = FALSE;
	m_bIsCallSetSize = FALSE;
	m_bIsCallSetPosition = FALSE;

	m_bDragDropEnable = FALSE;
	m_bIsEnableScrollBar = TRUE;
	m_bIsDragSelect = FALSE;
	m_bDragItem = FALSE;
	m_bMouseFirstMoveRecorded = FALSE;
	m_rcBack.SetRect(0, 0, 0, 0);
	m_uClipFormat = 0;

	m_sizeTotal = CSize(-1, -1);
	m_sizeHighlight = CSize(-1, -1);
	m_sizeBkImage = CSize(-1, -1);

	m_rcThumbnail.SetRect(-1, -1, -1, -1);
	m_rcLabel.SetRect(-1, -1, -1, -1);
	m_rcHLThumbnail.SetRect(-1, -1, -1, -1);
	m_rcHLLabel.SetRect(-1, -1, -1, -1);
	m_nItemHeight = 20;
	m_ftLabel.CreateFont(-12, 0, 0, 0, FW_NORMAL, false, false, 0, DEFAULT_CHARSET, OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, DRAFT_QUALITY, 34, _T("Tahoma"));

	m_nShiftStart = -1;
	m_nHotItem = -1;

	m_bDrawBkColor = FALSE;
	m_crBk = RGB(255, 255, 255);
	m_crText = RGB(0, 0, 0);

	m_nLinesPerItem = 1;
	m_nScrollBarWidth = 16;
	m_bIsCurShowScroll = TRUE;

	m_bHoldCtrlKey = FALSE;
}

CThumbnailCtrl::~CThumbnailCtrl()
{
	if (m_pMember != NULL)
	{
		delete (THUMBANILMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}


BEGIN_MESSAGE_MAP(CThumbnailCtrl, CWnd)
	//{{AFX_MSG_MAP(CThumbnailCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThumbnailCtrl message handlers


BOOL CThumbnailCtrl::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	m_bListStyle = ((dwStyle & LVS_LIST) == LVS_LIST) ? TRUE : FALSE;
	m_bSingleSel = ((dwStyle & LVS_SINGLESEL) == LVS_SINGLESEL) ? TRUE : FALSE;

	CString strMyClass;
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;
	strMyClass = WC_THUMBNAILVIEW;
	wndcls.lpszClassName = strMyClass.GetBuffer(strMyClass.GetLength());

	// Register the new class and exit if it fails
	if (!AfxRegisterClass(&wndcls))
	{
		TRACE(_T("[LibUIDK]===Class Registration Failed\n"));
		return FALSE;
	}

	CWnd *pWnd = this;
	return pWnd->Create(WC_THUMBNAILVIEW, NULL, dwStyle, rect, pParentWnd, nID);
}

BOOL CThumbnailCtrl::SetItemBitmap(LPCTSTR lpszNormal, LPCTSTR lpszHighlight, LPCTSTR lpszSelected, LPCTSTR lpszDisabled/*=NULL*/)
{
	HBITMAP hBitmap1 = NULL, hBitmap2 = NULL, hBitmap3 = NULL;
	hBitmap1 = (HBITMAP)LoadImage(NULL, lpszNormal, IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	hBitmap2 = (HBITMAP)LoadImage(NULL, lpszSelected, IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	hBitmap3 = (HBITMAP)LoadImage(NULL, lpszHighlight, IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

	return SetItemBitmap(hBitmap1, hBitmap3, hBitmap2);
}

BOOL CThumbnailCtrl::SetItemBitmap(HBITMAP hItemBkN, HBITMAP hItemBkH, HBITMAP hItemBkS, HBITMAP hItemBkD/*=NULL*/)
{
	if (hItemBkN == NULL || hItemBkH == NULL || hItemBkS == NULL)
	{
		return FALSE;
	}

	m_pBmpEmpty.Attach(hItemBkN);
	m_pBmpHighlight.Attach(hItemBkH);
	m_pBmpSelected.Attach(hItemBkS);
	m_pBmpDisabled.Attach(hItemBkD);

	ASSERT(m_pBmpEmpty.m_hObject != NULL);
	ASSERT(m_pBmpSelected.m_hObject != NULL);
	ASSERT(m_pBmpHighlight.m_hObject != NULL);
	ASSERT(m_pBmpDisabled.m_hObject != NULL);

	if (!m_bListStyle)
	{
		// init m_sizeHighlight;
		BITMAP bmpHL;
		m_pBmpHighlight.GetObject(sizeof(BITMAP), &bmpHL);
		m_sizeHighlight = CSize(bmpHL.bmWidth, bmpHL.bmHeight);

		// init m_sizeBkImage
		BITMAP bmpBk;
		m_pBmpEmpty.GetObject(sizeof(BITMAP), &bmpBk);
		m_sizeBkImage = CSize(bmpBk.bmWidth, bmpBk.bmHeight);
	}

	return TRUE;
}

int CThumbnailCtrl::GetItemBitmap(HBITMAP *phItemBKN, HBITMAP *phItemBKH, HBITMAP *phItemBKS, HBITMAP *phItemBKD/* = NULL*/)  const
{
	if (phItemBKN != NULL)
	{
		*phItemBKN = (HBITMAP)m_pBmpEmpty.GetSafeHandle();
	}
	if (phItemBKH != NULL)
	{
		*phItemBKH = (HBITMAP)m_pBmpHighlight.GetSafeHandle();
	}
	if (phItemBKS != NULL)
	{
		*phItemBKS = (HBITMAP)m_pBmpSelected.GetSafeHandle();
	}
	if (phItemBKD != NULL)
	{
		*phItemBKD = (HBITMAP)m_pBmpDisabled.GetSafeHandle();
	}
	return 0;
}

BOOL CThumbnailCtrl::SetItemSize(CSize sizeTotal)
{
	if (m_bListStyle)
	{
		return FALSE;
	}

	if (m_sizeBkImage != CSize(-1, -1))
	{
		ASSERT(sizeTotal.cx >= m_sizeBkImage.cx);
		ASSERT(sizeTotal.cy >= m_sizeBkImage.cy);
	}

	m_sizeTotal = sizeTotal;
	if (m_sizeBkImage == CSize(-1, -1))
	{
		sizeTotal.cy -= 2;
		m_sizeBkImage = sizeTotal;
		m_sizeHighlight = sizeTotal;
	}

	m_bIsCallSetSize = TRUE;

	return TRUE;
}

// set the thumbnail and the label position relative to the normal background image
BOOL CThumbnailCtrl::SetItemPosition(LPCRECT lprcThumbnail, LPCRECT lprcHLThumbnail, LPCRECT lprcLabel, LPCRECT lprcHLLabel)
{
	// 	if (m_bListStyle)
	// 	{
	// 		return FALSE;
	// 	}

	if (lprcThumbnail != NULL)
	{
		m_rcThumbnail.CopyRect(lprcThumbnail);
	}
	if (lprcLabel != NULL)
	{
		m_rcLabel.CopyRect(lprcLabel);
	}

	if (lprcHLThumbnail != NULL)
	{
		m_rcHLThumbnail.CopyRect(lprcHLThumbnail);
	}
	if (lprcHLLabel != NULL)
	{
		m_rcHLLabel.CopyRect(lprcHLLabel);
	}

	m_bIsCallSetPosition = TRUE;

	return TRUE;
}

int CThumbnailCtrl::GetItemPosition(LPRECT lprcThumbnail, LPRECT lprcHLThumbnail, LPRECT lprcLabel, LPRECT lprcHLLabel) const
{
	// 	if (m_bListStyle)
	// 		return -1;

	if (lprcThumbnail != NULL)
	{
		*lprcThumbnail = m_rcThumbnail;
	}

	if (lprcHLThumbnail)
	{
		*lprcHLThumbnail = m_rcHLThumbnail;
	}

	if (lprcLabel != NULL)
	{
		*lprcLabel = m_rcLabel;
	}

	if (lprcHLLabel != NULL)
	{
		*lprcHLLabel = m_rcHLLabel;
	}

	return 0;
}

BOOL CThumbnailCtrl::SetLinesPerItem(int nLines)
{
	if (nLines <= 0)
	{
		return FALSE;
	}

	m_nLinesPerItem = nLines;
	return TRUE;
}

void CThumbnailCtrl::SetScrollBarWidth(int nWidth)
{
	m_nScrollBarWidth = nWidth;
}

void CThumbnailCtrl::OnDraw(CDC *pDC)
{
	CRect rcDisplay;
	GetViewRect(rcDisplay);
	if (rcDisplay.right <= rcDisplay.left)
	{
		return;
	}

	// Draw the background of all client area
	CRect rcClient;
	GetClientRect(rcClient);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bmpClip;
	bmpClip.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	ASSERT(bmpClip.GetSafeHandle() != NULL);
	CBitmap *pBmpClipOld = memDC.SelectObject(&bmpClip);
	memDC.SetBkMode(TRANSPARENT);

	// Draw Items
	SCROLLINFO si;
	m_wndScrollEx.GetScrollInfo(&si);

	if (m_bListStyle)
	{
		int nItem = GetTopIndex();
		int nLast = nItem + GetCountPerPage() + 1;
		int nCount = GetItemCount();
		if (nLast > nCount)
		{
			nLast = nCount;
		}

		int nFirst = nItem;
		for (; nItem < nLast; nItem++)
		{
			DRAWITEMSTRUCT DrawItemStruct;
			DrawItemStruct.CtlType = ODT_THUMBNAIL;
			DrawItemStruct.CtlID = GetDlgCtrlID();
			DrawItemStruct.itemID = nItem;
			DrawItemStruct.hwndItem = m_hWnd;
			DrawItemStruct.hDC = memDC.GetSafeHdc();
			DrawItemStruct.itemData = GetItemData(nItem);
			RECT rcItem = {0, m_nItemHeight *(nItem - nFirst), rcDisplay.Width(), m_nItemHeight *(nItem - nFirst + 1) };
			DrawItemStruct.rcItem = rcItem;
			DrawItem(&DrawItemStruct);
		}
	}
	else
	{
		ASSERT(m_bIsCallSetSize && m_bIsCallSetPosition);
		ASSERT(m_sizeTotal != CSize(-1, -1));
		ASSERT(m_sizeBkImage != CSize(-1, -1));

		ASSERT(m_rcThumbnail != CRect(-1, -1, -1, -1));
		ASSERT(m_rcLabel != CRect(-1, -1, -1, -1));
		ASSERT(m_rcHLThumbnail != CRect(-1, -1, -1, -1));
		ASSERT(m_rcHLLabel != CRect(-1, -1, -1, -1));

		CDC memDCItems;
		memDCItems.CreateCompatibleDC(pDC);
		CBitmap bmpItems;
		bmpItems.CreateCompatibleBitmap(pDC, rcDisplay.Width(), rcDisplay.Height());
		ASSERT(bmpItems.GetSafeHandle() != NULL);
		CBitmap *pBmpOld = memDCItems.SelectObject(&bmpItems);

		// Draw parent part.
		CRect rcWin;
		GetWindowRect(rcWin);
		GetParent()->ScreenToClient(rcWin);

		BOOL bUpdateLayeredWindow = FALSE;
		DrawParentPart(this, &memDCItems, rcWin, &bUpdateLayeredWindow, NULL);

		if (m_himgBk != NULL)
		{
			IUIDrawImage(memDCItems.GetSafeHdc(), rcClient, m_himgBk,
				m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
		}
		else
		{
			if (m_bDrawBkColor)
			{
				memDCItems.FillSolidRect(rcClient, m_crBk);
			}
		}

		memDCItems.SetBkMode(TRANSPARENT);
		CFont *pFontOld = memDCItems.SelectObject(&m_ftLabel);

		// If a grid not accommodate one thumbnail
		CSize sizeGrid = CalcThumbnailGrid();
		if (sizeGrid.cx <= 0)
		{
			//		pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
			//		memDC.SelectObject(pBmpClipOld);
			//		return;
		}
		sizeGrid.cy += 2;

		int nSpace = GetSpace();

		int nThumb = si.nPos / m_nLinesPerItem * sizeGrid.cx;
		if (GetItemCount() > 0)
		{
			for (int i = 0; i < sizeGrid.cy; i++)
			{
				for (int j = 0; j < sizeGrid.cx; j++, nThumb++)
				{
					if (IsValidIndex(nThumb))
					{
						int x = j * (m_sizeTotal.cx + 2 * nSpace) + nSpace + (m_sizeTotal.cx - m_sizeBkImage.cx) / 2 ;
						int y = i * m_sizeTotal.cy + (m_sizeTotal.cy - m_sizeBkImage.cy) / 2;
						int nY = (si.nPos % m_nLinesPerItem) * (m_sizeTotal.cy / m_nLinesPerItem);
						y -= nY;

						DRAWITEMSTRUCT DrawItemStruct;
						RECT rcItem = { x, y, x + m_sizeBkImage.cx, y + m_sizeBkImage.cy };
						DrawItemStruct.CtlType = ODT_THUMBNAIL;
						DrawItemStruct.CtlID = GetDlgCtrlID();
						DrawItemStruct.itemID = nThumb;
						DrawItemStruct.hwndItem = m_hWnd;
						DrawItemStruct.hDC = memDCItems.GetSafeHdc();
						DrawItemStruct.itemData = GetItemData(nThumb);
						DrawItemStruct.rcItem = rcItem;
						DrawItem(&DrawItemStruct);
					}
				}
			}
		}

		// draw to pDC
		memDC.BitBlt(rcDisplay.left, rcDisplay.top, rcDisplay.Width(), rcDisplay.Height(), &memDCItems, 0, 0, SRCCOPY);
		memDCItems.SelectObject(pBmpOld);
		bmpItems.DeleteObject();
		memDCItems.SelectObject(pFontOld);
	}

	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pBmpClipOld);
}

void CThumbnailCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	BOOL bParentDraw = (BOOL)GetParent()->SendMessage(WM_DRAWITEMEX, lpDrawItemStruct->CtlID, LPARAM(lpDrawItemStruct));
	if (bParentDraw)
	{
		return;
	}

	HDC memDCBk = lpDrawItemStruct->hDC;
	int nItem = lpDrawItemStruct->itemID;
	UINT nState = GetItemState(nItem, LVIS_SELECTED);
	BOOL bIsSelected = ((nState & LVIS_SELECTED) == LVIS_SELECTED);
	BOOL bIsHighlight = (nItem == m_nHotItem);

	CDC *pDC = CDC::FromHandle(memDCBk);
	pDC->SetTextColor(m_crText);
	if (m_bListStyle)
	{
		if (bIsSelected)
		{
			// Draw background
			pDC->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255, 0, 0));
		}
		else
		{
			if (bIsHighlight)
			{
				// Draw background
				pDC->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255, 255, 0));
			}
		}
		// Draw Label
		CString strLabel = GetItemText(nItem, 0);
		pDC->DrawText(strLabel, strLabel.GetLength(), &lpDrawItemStruct->rcItem, DT_LEFT | DT_END_ELLIPSIS);
	}
	else
	{
		int x = lpDrawItemStruct->rcItem.left;
		int y = lpDrawItemStruct->rcItem.top;
		// init memory background DC, position, thumbnail
		::SetStretchBltMode(memDCBk, COLORONCOLOR);

		HDC memDC;
		memDC = ::CreateCompatibleDC(memDCBk);

		HBITMAP hOldBk;

		if (bIsSelected)
		{
			// Draw background
			if (m_pBmpSelected.GetSafeHandle() != NULL)
			{
				if (IsWindowEnabled())
				{
					hOldBk = (HBITMAP)::SelectObject(memDC, m_pBmpSelected);
				}
				else
				{
					hOldBk = (HBITMAP)::SelectObject(memDC, m_pBmpDisabled);
				}
				::BitBlt(memDCBk, x, y, m_sizeBkImage.cx, m_sizeBkImage.cy, memDC, 0, 0, SRCCOPY);
				::SelectObject(memDC, hOldBk);
			}
			else
			{
				HBRUSH hBrush = ::CreateSolidBrush(RGB(10, 36, 106));
				::FillRect(memDCBk, CRect(x, y, x + m_sizeBkImage.cx, y + m_sizeBkImage.cy), hBrush);
				::DeleteObject(hBrush);

				hBrush = ::CreateSolidBrush(RGB(255, 255, 255));
				::FillRect(memDCBk, CRect(x + 2, y + 2, x + m_sizeBkImage.cx - 2, y + m_sizeBkImage.cy - 2), hBrush);
				::DeleteObject(hBrush);
			}

			// Draw Thumbnail
			HBITMAP hbmpThumbnail = GetItemThumbnail(nItem);
			HBITMAP hBmpThumOld = (HBITMAP)::SelectObject(memDC, hbmpThumbnail);
			BITMAP bmpThum;
			::GetObject(hbmpThumbnail, sizeof(bmpThum), &bmpThum);
			::StretchBlt(memDCBk, x + m_rcThumbnail.left, y + m_rcThumbnail.top, m_rcThumbnail.Width(), m_rcThumbnail.Height(), memDC, 0, 0, bmpThum.bmWidth, bmpThum.bmHeight, SRCCOPY);
			::SelectObject(memDC, hBmpThumOld);

			// Draw Label
			CString strLabel = GetItemText(nItem, 0);
			::DrawText(memDCBk, strLabel, strLabel.GetLength(), CRect(x + m_rcLabel.left, y + m_rcLabel.top, x + m_rcLabel.right, y + m_rcLabel.bottom), DT_CENTER | DT_END_ELLIPSIS);
		}
		else
		{
			if (bIsHighlight)
			{
				int nXH = (m_sizeTotal.cx - m_sizeHighlight.cx) / 2;
				int nYH = (m_sizeTotal.cy - m_sizeHighlight.cy) / 2;
				int nX = (m_sizeTotal.cx - m_sizeBkImage.cx) / 2;
				int nY = (m_sizeTotal.cy - m_sizeBkImage.cy) / 2;
				int nCurX = x - nX + nXH;
				int nCurY = y - nY + nYH;

				// Draw background
				if (m_pBmpHighlight.GetSafeHandle() != NULL)
				{
					if (IsWindowEnabled())
					{
						hOldBk = (HBITMAP)::SelectObject(memDC, m_pBmpHighlight);
					}
					else
					{
						hOldBk = (HBITMAP)::SelectObject(memDC, m_pBmpDisabled);
					}
					::BitBlt(memDCBk, nCurX, nCurY, m_sizeHighlight.cx, m_sizeHighlight.cy, memDC, 0, 0, SRCCOPY);
					::SelectObject(memDC, hOldBk);
				}
				else
				{
					HBRUSH hBrush = ::CreateSolidBrush(RGB(0, 0, 100));
					::FillRect(memDCBk, CRect(x, y, x + m_sizeBkImage.cx, y + m_sizeBkImage.cy), hBrush);
					::DeleteObject(hBrush);

					hBrush = ::CreateSolidBrush(RGB(255, 255, 255));
					::FillRect(memDCBk, CRect(x + 1, y + 1, x + m_sizeBkImage.cx - 1, y + m_sizeBkImage.cy - 1), hBrush);
					::DeleteObject(hBrush);
				}

				// Draw Thumbnail
				HBITMAP hbmpThumbnail = GetItemThumbnail(nItem);
				HBITMAP hBmpThumOld = (HBITMAP)::SelectObject(memDC, hbmpThumbnail);
				BITMAP bmpThum;
				::GetObject(hbmpThumbnail, sizeof(bmpThum), &bmpThum);
				::StretchBlt(memDCBk, nCurX + m_rcHLThumbnail.left, nCurY + m_rcHLThumbnail.top, m_rcHLThumbnail.Width(), m_rcHLThumbnail.Height(), memDC, 0, 0, bmpThum.bmWidth, bmpThum.bmHeight, SRCCOPY);
				::SelectObject(memDC, hBmpThumOld);

				// Draw Label
				CString strLabel = GetItemText(nItem, 0);
				::DrawText(memDCBk, strLabel, strLabel.GetLength(), CRect(nCurX + m_rcHLLabel.left, nCurY + m_rcHLLabel.top, nCurX + m_rcHLLabel.right, nCurY + m_rcHLLabel.bottom), DT_CENTER | DT_END_ELLIPSIS);
			}
			else
			{
				// Draw background
				if (m_pBmpEmpty.GetSafeHandle() != NULL)
				{
					if (IsWindowEnabled())
					{
						hOldBk = (HBITMAP)::SelectObject(memDC, m_pBmpEmpty);
					}
					else
					{
						hOldBk = (HBITMAP)::SelectObject(memDC, m_pBmpDisabled);
					}
					::BitBlt(memDCBk, x, y, m_sizeBkImage.cx, m_sizeBkImage.cy, memDC, 0, 0, SRCCOPY);
					::SelectObject(memDC, hOldBk);
				}
				else
				{
					HBRUSH hBrush = ::CreateSolidBrush(RGB(212, 208, 200));
					::FillRect(memDCBk, CRect(x, y, x + m_sizeBkImage.cx, y + m_sizeBkImage.cy), hBrush);
					::DeleteObject(hBrush);

					hBrush = ::CreateSolidBrush(RGB(255, 255, 255));
					::FillRect(memDCBk, CRect(x + 1, y + 1, x + m_sizeBkImage.cx - 1, y + m_sizeBkImage.cy - 1), hBrush);
					::DeleteObject(hBrush);
				}

				// Draw Thumbnail
				HBITMAP hbmpThumbnail = GetItemThumbnail(nItem);
				BITMAP bmpThum;
				::GetObject(hbmpThumbnail, sizeof(bmpThum), &bmpThum);
				HBITMAP hBmpThumOld = (HBITMAP)::SelectObject(memDC, hbmpThumbnail);
				::StretchBlt(memDCBk, x + m_rcThumbnail.left, y + m_rcThumbnail.top, m_rcThumbnail.Width(), m_rcThumbnail.Height(), memDC, 0, 0, bmpThum.bmWidth, bmpThum.bmHeight, SRCCOPY);
				::SelectObject(memDC, hBmpThumOld);

				// Draw Label
				CString strLabel = GetItemText(nItem, 0);
				::DrawText(memDCBk, strLabel, strLabel.GetLength(), CRect(x + m_rcLabel.left, y + m_rcLabel.top, x + m_rcLabel.right, y + m_rcLabel.bottom), DT_CENTER | DT_END_ELLIPSIS);
			}
		}

		::DeleteDC(memDC);
	}
}

CDC *CThumbnailCtrl::BeginRepaint()
{
	COLORREF colorBkg = RGB(255, 251, 240);

	CClientDC dc(this);
	CDC *memDC = new CDC;
	memDC->CreateCompatibleDC(&dc);
	CRect rcClient;
	GetClientRect(rcClient);
	CBitmap m_bmpCanvas;
	m_bmpCanvas.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	ASSERT(m_bmpCanvas.GetSafeHandle() != NULL);
	CBitmap *m_pOldBitmap = memDC->SelectObject(&m_bmpCanvas);
	memDC->FillSolidRect(rcClient, colorBkg);
	OnDraw(memDC);
	return memDC;
}

void CThumbnailCtrl::EndRepaint(CDC *pDC)
{
	ASSERT(pDC);

	CRect rcDisplay;
	GetViewRect(rcDisplay);

	CClientDC dc(this);
	dc.BitBlt(rcDisplay.left, rcDisplay.top, rcDisplay.Width(), rcDisplay.Height(), pDC, rcDisplay.left, rcDisplay.top, SRCCOPY);

	delete pDC;
}

void CThumbnailCtrl::SelectByRect(LPRECT rcSel, BOOL bClear)
{
	if (bClear)
	{
		SelectAll(FALSE);
	}

	CRect rcSelView(rcSel);
	WindowToClient(&rcSelView);
	CRect rcView;
	GetViewRect(&rcView);

	SCROLLINFO si;
	m_wndScrollEx.GetScrollInfo(&si);
	CSize sizeGrid = CalcThumbnailGrid();

	int nSpace = GetSpace();
	if (rcSelView.right > sizeGrid.cx * (m_sizeTotal.cx + 2 * nSpace))
	{
		rcSelView.right = sizeGrid.cx * (m_sizeTotal.cx + 2 * nSpace) - 1;
	}
	if (rcSelView.left <= 0)
	{
		rcSelView.left = 0;
	}

	int nTopLine = si.nPos + (rcSelView.top / (m_sizeTotal.cy / m_nLinesPerItem));
	int nLeftLine = rcSelView.left / (m_sizeTotal.cx + 2 * nSpace);
	int nRightLine = rcSelView.right / (m_sizeTotal.cx + 2 * nSpace);
	int nBottomLine = si.nPos + (rcSelView.bottom / (m_sizeTotal.cy / m_nLinesPerItem));

	int nTopLeftItem = nTopLine  / m_nLinesPerItem * sizeGrid.cx + nLeftLine;
	int nRightBottomItem = nBottomLine / m_nLinesPerItem * sizeGrid.cx + nRightLine;

	for (int i = nTopLeftItem; i <= nRightBottomItem; ++i)
	{
		if (IsValidIndex(i))
		{
			if (sizeGrid.cx == 0 || sizeGrid.cy == 0)
			{
				break;
			}
			if ((i % sizeGrid.cx) >= (nTopLeftItem % sizeGrid.cx) && (i % sizeGrid.cx) <= (nRightBottomItem % sizeGrid.cx))
			{
				// if the item rect and the selection rect intersect?
				CRect rcItem;
				GetItemRect(i, &rcItem, LVIR_SELECTBOUNDS);

				CRect rcTemp;
				BOOL bIsIntersect = rcTemp.IntersectRect(rcItem, rcSelView);

				if (bIsIntersect)
				{
					SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
				}
			}
		}
	}
}

int CThumbnailCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// TODO: Add your specialized creation code here

	// create a list control for save data
	if (!m_list.Create(WS_CHILD | LVS_REPORT, CRect(0, 0, 0, 0), this, 0))
	{
		return -1;
	}

	//scroll bar
	CRect rcScroll;
	GetScrollBarRect(rcScroll);
	BOOL bRet = FALSE;
	bRet = m_wndScrollEx.Create(WS_CHILD | SBS_VERT | SBS_LEFTALIGN, rcScroll, this, 1);
	if (!bRet)
	{
		TRACE(_T("[LibUIDK]===Failed to create the scroll bar within the Thumbnail control!\n"));
		return -1;
	}
	UpdateScrollingDisplay();

	bRet = m_dropTarget.Register(this);
	EnableToolTips(TRUE);

	if (m_bListStyle)
	{
		CRect rcView;
		GetViewRect(rcView);
		m_sizeTotal.cx = rcView.Width();
		m_sizeBkImage.cx = m_sizeTotal.cx - 2;
		m_sizeBkImage.cy = m_sizeTotal.cy - 2;
		m_sizeHighlight.cx = m_sizeTotal.cx - 2;
		m_sizeHighlight.cy = m_sizeTotal.cy - 2;
	}

	return 0;
}

CSize CThumbnailCtrl::CalcThumbnailGrid() const
{
	CRect rcDisplay;
	GetViewRect(rcDisplay);

	CSize sizeGrid;

	if (m_bListStyle)
	{
		sizeGrid.cx = 1;
		int nItemHeight = GetItemHeight();
		sizeGrid.cy = rcDisplay.Height() / nItemHeight;
	}
	else
	{
		sizeGrid.cx = rcDisplay.Width() / m_sizeTotal.cx;

		// Allow 'no extra border' at last line.
		sizeGrid.cy = (rcDisplay.Height() + (m_sizeTotal.cy - m_sizeBkImage.cy) / 2) / m_sizeTotal.cy;
	}

	return sizeGrid;
}


//////////////////////////////////////////////////////////////////////////
// support drag and drop
void CThumbnailCtrl::BeginDrag(UINT nFlags, CPoint point)
{
	// register custom clipboard format
	CString strContens = _T("DragItem");
	m_uClipFormat = RegisterClipboardFormat(strContens);

	// the data be dragged in memory.
	int nLen = strContens.GetLength() + 1;
	HGLOBAL hData = GlobalAlloc(GHND | GMEM_SHARE, sizeof(TCHAR) * nLen);

	TCHAR *lpString = (TCHAR *)GlobalLock(hData);
	TSTRCPY(lpString, nLen, strContens);

	GlobalUnlock(hData);

	// Init the IDataObject by memory data
	COleDataSource source;
	source.CacheGlobalData(m_uClipFormat, hData);

	// Do drag drop
	source.DoDragDrop(DROPEFFECT_COPY);

	// release memory data.
	GlobalFree(hData);
}

//////////////////////////////////////////////////////////////////////////

void CThumbnailCtrl::DoMultipleSelection(BOOL bShift, BOOL bCtrl, int nPos, BOOL bIsLButtonDown)
{
	if (bIsLButtonDown)
	{
		if (bShift) // Shift key is pressed.
		{
			if (m_nShiftStart < 0)
			{
				// Set shift start to this position.
				SetShiftStart(nPos);
			}

			// Select between shift start and new selected.
			int minSel = min(m_nShiftStart, nPos);
			int maxSel = max(m_nShiftStart, nPos);

			SelectAll(FALSE);

			for (int i = minSel; i <= maxSel; i++)
			{
				SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			}

			RefreshDisplay();
		}
		else if (!bCtrl)
		{
			int nState = GetItemState(nPos, LVIS_SELECTED);
			if ((nState & LVIS_SELECTED) != LVIS_SELECTED)
			{
				SelectAll(FALSE);
				SetItemState(nPos, LVIS_SELECTED, LVIS_SELECTED);

				RefreshDisplay();
				SetShiftStart(nPos);
			}
		}
	}
	else // OnLButtonUp
	{
		if (bCtrl)
		{
			BOOL bRowPressed = (::GetKeyState(VK_LEFT) < 0)
				|| (::GetKeyState(VK_RIGHT) < 0)
				|| (::GetKeyState(VK_UP) < 0)
				|| (::GetKeyState(VK_DOWN) < 0);
			if (bRowPressed)
			{
				for (int i = 0; i < GetItemCount(); ++i)
				{
					int nState = GetItemState(i, LVIS_SELECTED);
					if ((nState & LVIS_SELECTED) == LVIS_SELECTED)
					{
						SetItemState(i, 0, LVIS_SELECTED);
					}
				}
			}

			// Toggle selected state.
			UINT nState = GetItemState(nPos, LVIS_SELECTED);
			if ((nState & LVIS_SELECTED) == LVIS_SELECTED)
			{
				SetItemState(nPos, 0, LVIS_SELECTED);
			}
			else
			{
				SetItemState(nPos, LVIS_SELECTED, LVIS_SELECTED);
			}
			SetShiftStart(nPos);
			RefreshDisplay();
		}
		else if (!bShift)
		{
			SelectAll(FALSE);
			SetItemState(nPos, LVIS_SELECTED, LVIS_SELECTED);
			RefreshDisplay();
			SetShiftStart(nPos);
		}
	}
}

BOOL CThumbnailCtrl::DoLButtonDown(UINT nFlags, CPoint point, BOOL bIsLButtonDown)
{
	// set new highlight
	int nHitTest = HitTest(point);
	if (IsValidIndex(nHitTest))
	{
		if (m_bSingleSel)
		{
			SelectAll(FALSE);
			SetItemState(nHitTest, LVIS_SELECTED, LVIS_SELECTED);
			RefreshDisplay();
		}
		else
		{
			DoMultipleSelection(MK_SHIFT & nFlags, MK_CONTROL & nFlags, nHitTest, bIsLButtonDown);

			// Detect highlight change.
			BOOL bHighlightChanged = (nHitTest != GetFocusItem());
			if (bHighlightChanged)
			{
				SetFocusItem(nHitTest);
			}
		}

		return TRUE;
	}

	SelectAll(FALSE);
	BOOL bHighlightChanged = (-1 != GetFocusItem());
	if (bHighlightChanged)
	{
		SetFocusItem(-1);
		RefreshDisplay();
	}

	return FALSE; // Unexpected hit test.
}

void CThumbnailCtrl::RefreshDisplay(BOOL  bUpdateImmediately)
{
	UpdateScrollingDisplay();

	CRect rcDisplay;
	GetViewRect(&rcDisplay);
	Invalidate();
}

void CThumbnailCtrl::UpdateScrollingDisplay()
{
	if (!IsWindow(m_wndScrollEx.GetSafeHwnd()))
	{
		return;
	}

	SCROLLINFO si;
	si.cbSize = sizeof(si);             // size of this structure
	si.fMask = SIF_ALL;                 // parameters to set
	m_wndScrollEx.GetScrollInfo(&si);
	int nPage = 0;
	int cMaxLinesPerPage = 0;
	int nTotalLines = 0;

	if (m_bListStyle)
	{
		nTotalLines = GetItemCount();
		cMaxLinesPerPage = GetCountPerPage();
		if (nTotalLines > cMaxLinesPerPage)
		{
			nPage = cMaxLinesPerPage;
			m_bIsCurShowScroll = TRUE;
		}
		else
		{
			nPage = nTotalLines;
			m_bIsCurShowScroll = FALSE;
		}
	}
	else
	{
		// init nPage
		CRect rcDisplay;
		GetViewRect(&rcDisplay);
		CSize sizeGrid = CalcThumbnailGrid();
		if (sizeGrid.cx <= 0)
		{
			return;
		}

		cMaxLinesPerPage = rcDisplay.Height() * m_nLinesPerItem / m_sizeTotal.cy;
		if (GetItemCount() == 0)
		{
			nPage = 0;
			nTotalLines = 0;
		}
		else
		{
			nTotalLines = ((GetItemCount() - 1) / sizeGrid.cx + 1) * m_nLinesPerItem;
			if (nTotalLines > cMaxLinesPerPage)
			{
				nPage = cMaxLinesPerPage;
				m_bIsCurShowScroll = TRUE;
			}
			else
			{
				nPage = nTotalLines;
				m_bIsCurShowScroll = FALSE;
			}
		}
	}

	si.nMin = 0;						// minimum scrolling position
	si.nMax = nTotalLines - 1;			// maximum scrolling position(total lines - 1)
	si.nPage = nPage;					// the page size of the scroll box
	if (si.nPos > si.nMax)				// current pos should less then total lines.
	{
		si.nPos = si.nMax;
	}
	if (si.nPos < 0)
	{
		si.nPos = 0;
	}
	m_wndScrollEx.SetScrollInfo(&si);

	BOOL bShowScrollBar = (m_bIsCurShowScroll && m_bIsEnableScrollBar);
	if (bShowScrollBar)
	{
		CRect rcScroll;
		GetScrollBarRect(rcScroll);
		m_wndScrollEx.MoveWindow(rcScroll);
		m_wndScrollEx.ShowWindow(SW_SHOW);
	}
	else
	{
		m_wndScrollEx.ShowWindow(SW_HIDE);
	}
}

#if(_MSC_VER <= 1200) // VC6.0
int CThumbnailCtrl::OnToolHitTest(CPoint point, TOOLINFO *pTI) const
#else
INT_PTR CThumbnailCtrl::OnToolHitTest(CPoint point, TOOLINFO *pTI) const
#endif
{
	CWnd::OnToolHitTest(point, pTI);

	int ret = -1;

	CRect rcHit;
	int nHitTest = HitTest(point);

	if (!IsValidIndex(nHitTest))
	{
		return -1;
	}
	GetItemRect(nHitTest, &rcHit, 0);

	pTI->hwnd = GetSafeHwnd();
	pTI->lpszText = LPSTR_TEXTCALLBACK;
	pTI->uFlags |= TTF_CENTERTIP;

	// Save media index in uId
	pTI->uId = (UINT)nHitTest;

	CopyRect(&pTI->rect, rcHit);

	ret = int(pTI->uId);

	return ret;
}

BOOL CThumbnailCtrl::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
	// Make default tool tip multiline.
	CToolTipCtrl *pToolTip = static_cast<CToolTipCtrl *>(CWnd::FromHandle(pNMHDR->hwndFrom));
	if (pToolTip && pToolTip->GetMaxTipWidth() == -1)
	{
		pToolTip->SetMaxTipWidth(1024);
	}

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;

#if(_MSC_VER <= 1200) // VC6.0
	UINT nID = pNMHDR->idFrom;
#else
	UINT_PTR nID = pNMHDR->idFrom;
#endif
	if (pNMHDR->code == TTN_NEEDTEXT && (pTTT->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID = ::GetDlgCtrlID((HWND)nID);
	}

	CString strTipText = GetItemTooltip((int)nID);
	if (strTipText.IsEmpty())
	{
		strTipText = GetItemText(int(nID), 0);
	}
	if (strTipText.GetLength() > 0)
	{
#ifdef _DEBUG
		static TCHAR szTextBuf[1024];
		lstrcpyn(szTextBuf, strTipText, sizeof(szTextBuf));
		pTTT->lpszText = szTextBuf;
#else   // #ifdef _DEBUG
		if (pNMHDR->code == TTN_NEEDTEXT)
		{
			lstrcpyn(pTTT->szText, strTipText, sizeof(pTTT->szText));
		}
#endif  // #ifdef _DEBUG
	}

	*pResult = 0;
	return TRUE;    // message was handled
}

POSITION CThumbnailCtrl::GetFirstSelectedItemPosition() const
{
	return m_list.GetFirstSelectedItemPosition();
}

BOOL CThumbnailCtrl::DeleteSelectedItem()
{
	POSITION pos = NULL;
	BOOL bRet = FALSE;
	int nHighLight = GetFocusItem();
	while ((pos = GetFirstSelectedItemPosition()) != NULL)
	{
		int nItem = GetNextSelectedItem(pos);
		bRet = DeleteItem(nItem);
	}

	if (bRet)
	{
		if (nHighLight < GetItemCount())
		{
			SelectAll(FALSE);
			SetItemState(nHighLight, LVIS_SELECTED, LVIS_SELECTED);
			SetFocusItem(nHighLight);
		}
		else
		{
			SetFocusItem(-1);
		}

		RefreshDisplay();
	}
	return bRet;
}

void CThumbnailCtrl::MoveItem(int nDest, int nSrc)
{
	int nCount = m_list.GetItemCount();
	if (nSrc >= nCount || nSrc < 0)
	{
		return;
	}

	int nColumn = 0;
	CHeaderCtrl *pHeader = m_list.GetHeaderCtrl();
	if (pHeader == NULL)
	{
		nColumn = 1;
	}
	else
	{
		nColumn = pHeader->GetItemCount();
	}


	int nDestTemp = 0;
	if (nDest == -1)
	{
		nDestTemp = nCount;
	}
	else
	{
		nDestTemp = nDest;
	}

	if (nSrc == nDestTemp)
	{
		return;
	}
	if (nSrc + 1 == nDestTemp)
	{
		return;
	}

	int nItem = m_list.InsertItem(nDestTemp, _T(""));
	ASSERT(nItem == nDestTemp);
	if (nDestTemp < nSrc)
	{
		nSrc++;
	}

	for (int i = 0; i < nColumn; ++i)
	{
		// Get SubItem
		LVITEM item;
		item.mask = LVIF_DI_SETITEM | LVIF_IMAGE | LVIF_INDENT | LVIF_NORECOMPUTE | LVIF_PARAM | LVIF_STATE;
#if (_WIN32_WINNT >= 0x0501)
		item.mask |= LVIF_GROUPID | LVIF_COLUMNS;
#endif
		item.iItem = nSrc;
		item.iSubItem = i;
		item.state = LVIS_CUT | LVIS_DROPHILITED | LVIS_FOCUSED | LVIS_SELECTED;
		item.stateMask = (UINT) - 1;

		ASSERT(m_list.GetItem(&item));
		CString strItem = m_list.GetItemText(nSrc, i);

		// Set SubItem
		item.iItem = nItem;
		ASSERT(m_list.SetItem(&item));
		m_list.SetItemText(nItem, i, strItem);
	}

	// delete src item
	m_list.DeleteItem(nSrc);
}

int CThumbnailCtrl::HitTestDrop(CPoint point)
{
	if (m_bListStyle)
	{
		return HitTest(point);
	}
	else
	{
		CRect rcDisplay;
		GetViewRect(rcDisplay);
		CPoint ptOrg = point - rcDisplay.TopLeft();

		SCROLLINFO si;
		m_wndScrollEx.GetScrollInfo(&si);

		int nSpace = GetSpace();
		int nH = (ptOrg.x + (m_sizeTotal.cx + 2 * nSpace) / 2) / (m_sizeTotal.cx + 2 * nSpace);
		int nV = (ptOrg.y / (m_sizeTotal.cy / m_nLinesPerItem) + si.nPos) / m_nLinesPerItem;

		CSize sizeGrid = CalcThumbnailGrid();
		int nItem = nV * sizeGrid.cx + nH;
		if (IsValidIndex(nItem))
		{
			return nItem;
		}
		else
		{
			return -1;
		}
	}

	return -1;
}

BOOL CThumbnailCtrl::OnMovementKey(UINT nChar)
{
	BOOL bShiftPressed = ::GetKeyState(VK_SHIFT) & 0x8000;
	BOOL bCtrlPressed = ::GetKeyState(VK_CONTROL) & 0x8000;

	int nNewPos = -1;
	int nOldPos = GetFocusItem();

	if (m_bListStyle)
	{
		if (nChar == VK_UP)
		{
			nNewPos = nOldPos - 1;
		}
		else if (nChar == VK_DOWN)
		{
			nNewPos = nOldPos + 1;
		}
		else if (nChar == VK_HOME)
		{
			nNewPos = 0;
		}
		else if (nChar == VK_END)
		{
			nNewPos = GetItemCount() - 1;
		}
		else if (nChar == VK_PRIOR)
		{
			int nPerPage = GetCountPerPage();
			nNewPos = nOldPos - nPerPage;
		}
		else if (nChar == VK_NEXT)
		{
			int nPerPage = GetCountPerPage();
			nNewPos = nOldPos + nPerPage;
		}
	}
	else
	{
		CSize sizeGrid = CalcThumbnailGrid();
		int cThumbnailsPerPage = sizeGrid.cx * sizeGrid.cy;

		if (nChar == VK_UP)
		{
			nNewPos = nOldPos - sizeGrid.cx;
		}
		else if (nChar == VK_DOWN)
		{
			nNewPos = nOldPos + sizeGrid.cx;
		}
		else if (nChar == VK_LEFT)
		{
			nNewPos = nOldPos - 1;
		}
		else if (nChar == VK_RIGHT)
		{
			nNewPos = nOldPos + 1;
		}
		else if (nChar == VK_HOME)
		{
			nNewPos = 0;
		}
		else if (nChar == VK_END)
		{
			nNewPos = GetItemCount() - 1;
		}
		else if (nChar == VK_PRIOR)
		{
			nNewPos = nOldPos - cThumbnailsPerPage;
		}
		else if (nChar == VK_NEXT)
		{
			nNewPos = nOldPos + cThumbnailsPerPage;
		}
	}

	if (nNewPos >= GetItemCount())
	{
		nNewPos = GetItemCount() - 1;
	}

	// Transfer highlight to new selected position.
	if (IsValidIndex(nNewPos))
	{
		EnsureVisible(nNewPos);
		DoMultipleSelection(bShiftPressed, bCtrlPressed, nNewPos);

		// Highlight this item.
		BOOL bHighlightChanged = (nNewPos != GetFocusItem());
		if (bHighlightChanged)
		{
			SetFocusItem(nNewPos);
		}

		RefreshDisplay();
	}

	return TRUE;
}

int CThumbnailCtrl::GetFocusItem()
{
	int nCount = GetItemCount();
	int i = 0;
	BOOL bFound = FALSE;
	for (; i < nCount; ++i)
	{
		if (m_list.GetItemState(i, LVIS_FOCUSED) == LVIS_FOCUSED)
		{
			bFound = TRUE;
			break;
		}
	}
	if (bFound)
	{
		return i;
	}

	return -1;
}

int CThumbnailCtrl::SetFocusItem(int nItem)
{
	if (nItem == -1)
	{
		int nOld = GetFocusItem();
		m_list.SetItemState(nOld, 0, LVIS_FOCUSED);
	}
	else
	{
		m_list.SetItemState(nItem, LVIS_FOCUSED, LVIS_FOCUSED);
	}

	return 0;
}

void CThumbnailCtrl::ClientToWindow(LPRECT lpRect) const
{
	int nWidth = lpRect->right - lpRect->left;
	int nHeight = lpRect->bottom - lpRect->top;
	CRect rcDisplay;
	GetViewRect(&rcDisplay);

	lpRect->left += rcDisplay.left;
	lpRect->top += rcDisplay.top;
	lpRect->right = lpRect->left + nWidth;
	lpRect->bottom = lpRect->top + nHeight;
}

void CThumbnailCtrl::WindowToClient(LPRECT lpRect) const
{
	int nWidth = lpRect->right - lpRect->left;
	int nHeight = lpRect->bottom - lpRect->top;
	CRect rcDisplay;
	GetViewRect(&rcDisplay);

	lpRect->left -= rcDisplay.left;
	lpRect->top -= rcDisplay.top;
	lpRect->right = lpRect->left + nWidth;
	lpRect->bottom = lpRect->top + nHeight;
}

void CThumbnailCtrl::SetShiftStart(int nShiftStart)
{
	m_nShiftStart = nShiftStart;
}

BOOL CThumbnailCtrl::GetViewRect(LPRECT lpRect) const
{
	GetClientRect(lpRect);
	if (m_bIsCurShowScroll && m_bIsEnableScrollBar)
	{
		lpRect->right -= m_nScrollBarWidth;
	}

	return TRUE;
}

void CThumbnailCtrl::GetScrollBarRect(CRect &rcScrollBar) const
{
	GetViewRect(rcScrollBar);
	rcScrollBar.left = rcScrollBar.right;
	rcScrollBar.right = rcScrollBar.left + m_nScrollBarWidth;
}

void CThumbnailCtrl::EnableDragDrop(BOOL bEnable/* = TRUE  */)
{
	m_bDragDropEnable = bEnable;
}

void CThumbnailCtrl::EnableScrollBarCtrl(int nBar, BOOL bEnable/* = TRUE*/)
{
	m_bIsEnableScrollBar = bEnable;
}

void CThumbnailCtrl::EndDragSelect(POINT pt)
{
	POSITION posFirstSelected = GetFirstSelectedItemPosition();
	if (posFirstSelected != NULL)
	{
		int nItem = GetNextSelectedItem(posFirstSelected);
		SetFocusItem(nItem);
	}
}

//////////////////////////////////////////////////////////////////////////

BOOL CThumbnailCtrl::DeleteAllItems()
{
	int nCount = GetItemCount();
	for (int i = 0; i < nCount; ++i)
	{
		THUMBNAIL_ITEMDATA *pData = (THUMBNAIL_ITEMDATA *)m_list.GetItemData(i);
		if (pData != NULL)
		{
			delete pData;
			pData = NULL;
		}
	}

	return m_list.DeleteAllItems();
}

BOOL CThumbnailCtrl::DeleteItem(int nItem)
{
	THUMBNAIL_ITEMDATA *pData = (THUMBNAIL_ITEMDATA *)m_list.GetItemData(nItem);
	if (pData != NULL)
	{
		delete pData;
		pData = NULL;
	}
	return m_list.DeleteItem(nItem);
}

BOOL CThumbnailCtrl::EnsureVisible(int nItem, BOOL bPartialOK/* = TRUE*/)
{
	if (nItem >= GetItemCount() || nItem < 0)
	{
		return FALSE;
	}

	CSize sizeGrid = CalcThumbnailGrid();
	if (sizeGrid.cx <= 0)
	{
		return FALSE;
	}
	if (sizeGrid.cy < 1)
	{
		return FALSE;
	}

	int nPos = m_wndScrollEx.GetScrollPos();
	SCROLLINFO si;
	m_wndScrollEx.GetScrollInfo(&si);

	int nFirst = GetFirstVisibleItem();
	int nLast = GetLastVisibleItem();

	if (bPartialOK)
	{
		if (nItem >= nFirst && nItem <= nLast)
		{
			return TRUE;
		}

		if (nItem < nFirst)
		{
			int nLine = nFirst / sizeGrid.cx * m_nLinesPerItem - nItem / sizeGrid.cx * m_nLinesPerItem;

			int nNewPos = nPos - nLine;
			if (nNewPos < 0)
			{
				nNewPos = 0;
			}

			m_wndScrollEx.SetScrollPos(nNewPos);
		}
		if (nItem > nLast)
		{
			int nLine = nItem / sizeGrid.cx * m_nLinesPerItem - nLast / sizeGrid.cx * m_nLinesPerItem;

			int nNewPos = nPos + nLine;
			if (nNewPos > int(si.nMax + 1 - si.nPage))
			{
				nNewPos = si.nMax + 1 - si.nPage;
			}

			m_wndScrollEx.SetScrollPos(nNewPos);
		}

		InvalidateRect(NULL);
		return TRUE;
	}
	else
	{
		int nLineIndexFirst = nFirst / sizeGrid.cx * m_nLinesPerItem;
		int nLineIndexLast = nLast / sizeGrid.cx * m_nLinesPerItem;
		int nLineIndexItem = nItem / sizeGrid.cx * m_nLinesPerItem;

		if (nLineIndexItem <= nLineIndexFirst)
		{
			int nNewPos = nLineIndexItem;
			if (nNewPos < 0)
			{
				nNewPos = 0;
			}

			m_wndScrollEx.SetScrollPos(nNewPos);
		}
		else if (nLineIndexItem > nLineIndexFirst && nLineIndexItem < nLineIndexLast)
		{
			CRect rcItem;
			GetItemRect(nItem, &rcItem, LVIR_SELECTBOUNDS);
			CRect rcView;
			GetViewRect(&rcView);
			WindowToClient(&rcView);
			if (InsideRect(rcItem, rcView))
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else if (nLineIndexItem >= nLineIndexLast)
		{
			int nNewPos = nLineIndexItem;
			if (nNewPos > int(si.nMax + 1 - si.nPage))
			{
				nNewPos = si.nMax + 1 - si.nPage;
			}

			m_wndScrollEx.SetScrollPos(nNewPos);
		}

		InvalidateRect(NULL);
		return TRUE;
	}
}

COLORREF CThumbnailCtrl::GetBkColor() const
{
	return m_crBk;
}

BOOL CThumbnailCtrl::GetItem(LVITEM *pItem) const
{
	return m_list.GetItem(pItem);
}

int CThumbnailCtrl::GetItemCount() const
{
	int nCount = m_list.GetItemCount();
	return nCount;
}

BOOL CThumbnailCtrl::GetItemRect(int nItem, LPRECT lpRect, UINT nCode) const
{
	if (nItem >= GetItemCount() || nItem < 0)
	{
		return FALSE;
	}

	CSize sizeGrid = CalcThumbnailGrid();
	if (sizeGrid.cx <= 0)
	{
		return FALSE;
	}

	int nH = nItem % sizeGrid.cx;
	int nV = nItem / sizeGrid.cx;

	CRect rcDisplay;
	GetViewRect(&rcDisplay);
	int nSpace = GetSpace();

	int nPos = m_wndScrollEx.GetScrollPos();

	if (nCode == LVIR_BOUNDS)
	{
		lpRect->left = nH * (m_sizeTotal.cx + 2 * nSpace) + nSpace;
		lpRect->top = nV * m_sizeTotal.cy - nPos * (m_sizeTotal.cy / m_nLinesPerItem);
		lpRect->right = lpRect->left + m_sizeTotal.cx;
		lpRect->bottom = lpRect->top + m_sizeTotal.cy;
	}
	else if (nCode == LVIR_ICON)
	{
		lpRect->left = nH * (m_sizeTotal.cx + 2 * nSpace) + nSpace + (m_sizeTotal.cx - m_sizeBkImage.cx) / 2 + m_rcThumbnail.left;
		lpRect->top = nV * m_sizeTotal.cy + (m_sizeTotal.cy - m_sizeBkImage.cy) / 2 - nPos * (m_sizeTotal.cy / m_nLinesPerItem) + m_rcThumbnail.top;
		lpRect->right = lpRect->left + m_sizeThumbnail.cx;
		lpRect->bottom = lpRect->top + m_sizeThumbnail.cy;
	}
	else if (nCode == LVIR_LABEL)
	{
		lpRect->left = nH * (m_sizeTotal.cx + 2 * nSpace) + nSpace + (m_sizeTotal.cx - m_sizeBkImage.cx) / 2 + m_rcLabel.left;
		lpRect->top = nV * m_sizeTotal.cy + (m_sizeTotal.cy - m_sizeBkImage.cy) / 2 - nPos * (m_sizeTotal.cy / m_nLinesPerItem) + m_rcLabel.top;
		lpRect->right = lpRect->left + m_rcLabel.Width();
		lpRect->bottom = lpRect->top + m_rcLabel.Height();
	}
	else if (nCode == LVIR_SELECTBOUNDS)
	{
		lpRect->left = nH * (m_sizeTotal.cx + 2 * nSpace) + nSpace + (m_sizeTotal.cx - m_sizeBkImage.cx) / 2;
		lpRect->top = nV * m_sizeTotal.cy + (m_sizeTotal.cy - m_sizeBkImage.cy) / 2 - nPos * (m_sizeTotal.cy / m_nLinesPerItem);
		lpRect->right = lpRect->left + m_sizeBkImage.cx;
		lpRect->bottom = lpRect->top + m_sizeBkImage.cy;
	}
	else if (nCode == LVIR_SELECTBOUNDS + 1) // the rect of highlight background image
	{
		lpRect->left = nH * (m_sizeTotal.cx + 2 * nSpace) + nSpace + (m_sizeTotal.cx - m_sizeHighlight.cx) / 2;
		lpRect->top = nV * m_sizeTotal.cy + (m_sizeTotal.cy - m_sizeHighlight.cy) / 2 - nPos * (m_sizeTotal.cy / m_nLinesPerItem);
		lpRect->right = lpRect->left + m_sizeHighlight.cx;
		lpRect->bottom = lpRect->top + m_sizeHighlight.cy;
	}

	return TRUE;
}

HBITMAP CThumbnailCtrl::GetItemThumbnail(int nItem)
{
	ASSERT(m_hWnd != NULL);
	THUMBNAIL_ITEMDATA *pData = (THUMBNAIL_ITEMDATA *)m_list.GetItemData(nItem);
	if (pData != NULL)
	{
		return pData->hbmpThumbnail;
	}
	return NULL;
}

BOOL CThumbnailCtrl::SetItemThumbnail(int nItem, HBITMAP hbmpThumbnail, BOOL bDeleteOld/* = TRUE*/, BOOL bRedraw/* = TRUE*/)
{
	ASSERT(m_hWnd != NULL);
	int nCount = GetItemCount();
	if (nItem < 0 || nItem >= nCount)
	{
		return FALSE;
	}

	THUMBNAIL_ITEMDATA *pData = (THUMBNAIL_ITEMDATA *)m_list.GetItemData(nItem);
	if (pData != NULL)
	{
		if (bDeleteOld && pData->hbmpThumbnail != NULL)
		{
			::DeleteObject(pData->hbmpThumbnail);
		}

		pData->hbmpThumbnail = hbmpThumbnail;
	}
	else
	{
		pData = new THUMBNAIL_ITEMDATA;
		pData->hbmpThumbnail = hbmpThumbnail;
#if(_MSC_VER <= 1200) // VC6.0
		m_list.SetItemData(nItem, DWORD(pData));
#else
		m_list.SetItemData(nItem, DWORD_PTR(pData));
#endif
	}

	if (bRedraw)
	{
		Invalidate();
	}

	return TRUE;
}

CString CThumbnailCtrl::GetItemTooltip(int nItem) const
{
	ASSERT(m_hWnd != NULL);
	THUMBNAIL_ITEMDATA *pData = (THUMBNAIL_ITEMDATA *)m_list.GetItemData(nItem);
	if (pData == NULL)
	{
		return _T("");
	}

	return pData->strTooltip;
}

BOOL CThumbnailCtrl::SetItemTooltip(int nItem, LPCTSTR lpszTooltip)
{
	ASSERT(m_hWnd != NULL);
	int nCount = GetItemCount();
	if (nItem < 0 || nItem >= nCount)
	{
		return FALSE;
	}

	THUMBNAIL_ITEMDATA *pData = (THUMBNAIL_ITEMDATA *)m_list.GetItemData(nItem);
	if (pData != NULL)
	{
		pData->strTooltip = lpszTooltip;
	}
	else
	{
		pData = new THUMBNAIL_ITEMDATA;
		pData->strTooltip = lpszTooltip;
#if(_MSC_VER <= 1200) // VC6.0
		m_list.SetItemData(nItem, DWORD(pData));
#else
		m_list.SetItemData(nItem, DWORD_PTR(pData));
#endif
	}

	return TRUE;
}

UINT CThumbnailCtrl::GetItemState(int nItem, UINT nMask) const
{
	return m_list.GetItemState(nItem, nMask);
}

int CThumbnailCtrl::GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const
{
	return m_list.GetItemText(nItem, nSubItem, lpszText, nLen);
}

CString CThumbnailCtrl::GetItemText(int nItem, int nSubItem)
{
	return m_list.GetItemText(nItem, nSubItem);
}

#if(_MSC_VER <= 1200) // VC6.0
DWORD CThumbnailCtrl::GetItemData(int nItem)
#else
DWORD_PTR CThumbnailCtrl::GetItemData(int nItem)
#endif
{
	THUMBNAIL_ITEMDATA *pData = (THUMBNAIL_ITEMDATA *)m_list.GetItemData(nItem);
	if (pData != NULL)
	{
		return pData->dwData;
	}

	return 0;
}

int CThumbnailCtrl::GetNextItem(int nItem, int nFlags)
{
	return m_list.GetNextItem(nItem, nFlags);
}

int CThumbnailCtrl::GetNextSelectedItem(POSITION &pos)
{
	return m_list.GetNextSelectedItem(pos);
}

int CThumbnailCtrl::GetHotItem() const
{
	return m_nHotItem;
}

UINT CThumbnailCtrl::GetSelectedCount()
{
	int nSelected = 0;
	int nCount = GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		int nState = GetItemState(i, LVIS_SELECTED);

		if ((nState & LVIS_SELECTED) == LVIS_SELECTED)
		{
			nSelected++;
		}
	}

	return nSelected;
}

COLORREF CThumbnailCtrl::GetTextColor() const
{
	return m_crText;
}

int CThumbnailCtrl::GetTopIndex()
{
	SCROLLINFO si;
	m_wndScrollEx.GetScrollInfo(&si);

	return si.nPos;
}

int CThumbnailCtrl::HitTest(CPoint pt, UINT *pFlags) const
{
	CRect rcDisplay;
	GetViewRect(rcDisplay);

	// Check if pt inside thumbnail rectangle.
	if (!rcDisplay.PtInRect(pt))
	{
		return -1;
	}

	int nPos = m_wndScrollEx.GetScrollPos();
	if (m_bListStyle)
	{
		int nItem = nPos + pt.y / m_nItemHeight;
		int nCount = GetItemCount();
		if (nItem >= nCount)
		{
			return -1;
		}
		else
		{
			return nItem;
		}
	}
	else
	{
		CSize sizeGrid = CalcThumbnailGrid();
		if (sizeGrid.cx == 0)
		{
			return -1;
		}

		int nSpace = GetSpace();

		CPoint ptOrg = pt - rcDisplay.TopLeft();
		int h = ptOrg.x / (m_sizeTotal.cx + 2 * nSpace);
		int v = ptOrg.y / (m_sizeTotal.cy / m_nLinesPerItem);

		int nItem = (nPos + v) / m_nLinesPerItem * sizeGrid.cx + h;

		CRect rcItem;
		GetItemRect(nItem, &rcItem, LVIR_SELECTBOUNDS);
		ClientToWindow(&rcItem);
		if (rcItem.PtInRect(pt))
		{
			return nItem;
		}
		else
		{
			return -1;
		}
	}

	return -1;
}

int CThumbnailCtrl::InsertItem(const LVITEM *pItem)
{
	return m_list.InsertItem(pItem);
}

int  CThumbnailCtrl::InsertItem(int nItem, LPCTSTR lpszItem)
{
	return m_list.InsertItem(nItem, lpszItem);
}

int  CThumbnailCtrl::InsertItem(int nItem, LPCTSTR lpszItem, HBITMAP hThumbnail)
{
	int nIndex = m_list.InsertItem(nItem, lpszItem);

	THUMBNAIL_ITEMDATA *pData = (THUMBNAIL_ITEMDATA *)m_list.GetItemData(nIndex);
	ASSERT(pData != NULL);
	pData->hbmpThumbnail = hThumbnail;
	return nIndex;
}

int  CThumbnailCtrl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat/* = LVCFMT_LEFT*/, int nWidth/* = -1*/, int nSubItem/* = -1*/)
{
	return m_list.InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
}

BOOL CThumbnailCtrl::SetBkColor(COLORREF cr)
{
	m_crBk = cr;
	return TRUE;
}

BOOL CThumbnailCtrl::SetBkImage(LPCTSTR lpszImageName, BOOL fTile/* = TRUE*/)
{
	IUISetControlImage(&m_himgBk, lpszImageName);

	return TRUE;
}

int CThumbnailCtrl::GetBkImage(CString *pstrImageName)
{
	IUIGetControlImage(m_himgBk, pstrImageName);

	return 0;
}

BOOL CThumbnailCtrl::SetItem(const LVITEM *pItem)
{
	return m_list.SetItem(pItem);
}

BOOL CThumbnailCtrl::SetItemState(int nItem, LVITEM *pItem)
{
	return m_list.SetItemState(nItem, pItem);
}

BOOL CThumbnailCtrl::SetItemState(int nItem, UINT nState, UINT nMask)
{
	return m_list.SetItemState(nItem, nState, nMask);
}

BOOL CThumbnailCtrl::SetItemText(int nItem, int nSubItem, LPTSTR lpszText, BOOL bRedraw/* = TRUE*/)
{
	BOOL bRet = m_list.SetItemText(nItem, nSubItem, lpszText);
	if (!bRet)
	{
		return FALSE;
	}

	if (bRedraw)
	{
		Invalidate();
	}

	return TRUE;
}

BOOL CThumbnailCtrl::SetItemFont(const LOGFONT *lfItem)
{
	if (lfItem == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_ftLabel.DeleteObject();
	m_ftLabel.CreateFontIndirect(lfItem);

	return TRUE;
}

BOOL CThumbnailCtrl::GetItemFont(LOGFONT *lfItem) const
{
	if (lfItem == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (m_ftLabel.GetSafeHandle() == NULL)
	{
		return FALSE;
	}

	m_ftLabel.GetLogFont(lfItem);

	return TRUE;

	return TRUE;
}

BOOL CThumbnailCtrl::SetItemData(int nItem, DWORD dwData)
{
	THUMBNAIL_ITEMDATA *pData = (THUMBNAIL_ITEMDATA *)m_list.GetItemData(nItem);
	if (pData != NULL)
	{
		pData->dwData = dwData;
		return TRUE;
	}
	else
	{
		pData = new THUMBNAIL_ITEMDATA;
		pData->dwData = dwData;
#if(_MSC_VER <= 1200) // VC6.0
		return m_list.SetItemData(nItem, DWORD(pData));
#else
		return m_list.SetItemData(nItem, DWORD_PTR(pData));
#endif
	}
}

BOOL CThumbnailCtrl::SetTextColor(COLORREF cr, BOOL bRedraw/* = TRUE*/)
{
	m_crText = cr;

	if (bRedraw)
	{
		Invalidate();
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CThumbnailCtrl::IsValidIndex(int nIndex) const
{
	int nCount = m_list.GetItemCount();

	if (nIndex >= 0 && nIndex < nCount)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CThumbnailCtrl::SelectAll(BOOL bSelected /*= TRUE */)
{
	int nCount = m_list.GetItemCount();

	for (int i = 0; i < nCount; ++i)
	{
		if (bSelected)
		{
			SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		}
		else
		{
			SetItemState(i, 0, LVIS_SELECTED);
		}
	}

	return TRUE;
}

int CThumbnailCtrl::GetSpace() const
{
	CRect rcDisplay;
	GetViewRect(&rcDisplay);
	CSize sizeGrid = CalcThumbnailGrid();
	if (sizeGrid.cx <= 0)
	{
		return -1;
	}

	int nSpace = 0;
	if (rcDisplay.Width() > sizeGrid.cx * m_sizeTotal.cx)
	{
		int nAllSpace = rcDisplay.Width() - sizeGrid.cx * m_sizeTotal.cx;
		nSpace = nAllSpace / (sizeGrid.cx * 2);
	}

	return nSpace;
}

int CThumbnailCtrl::GetFirstVisibleItem(BOOL bPartialOK/* = TRUE*/)
{
	CRect rcView;
	GetViewRect(&rcView);
	WindowToClient(rcView);

	if (bPartialOK)
	{
		for (int i = 0; i < GetItemCount(); ++i)
		{
			CRect rcItem;
			GetItemRect(i, rcItem, LVIR_SELECTBOUNDS);

			CRect rcTemp;
			BOOL bFlag = rcTemp.IntersectRect(rcItem, rcView);
			if (bFlag)
			{
				return i;
			}
		}

		return -1;
	}
	else
	{
		for (int i = 0; i < GetItemCount(); ++i)
		{
			CRect rcItem;
			GetItemRect(i, rcItem, LVIR_SELECTBOUNDS);

			if (InsideRect(rcItem, rcView))
			{
				return i;
			}
		}

		return -1;
	}
}

int CThumbnailCtrl::GetLastVisibleItem(BOOL bPartialOK/* = TRUE*/)
{
	CRect rcView;
	GetViewRect(&rcView);
	WindowToClient(rcView);

	if (bPartialOK)
	{
		for (int i = GetItemCount() - 1; i >= 0; --i)
		{
			CRect rcItem;
			GetItemRect(i, rcItem, LVIR_SELECTBOUNDS);

			CRect rcTemp;
			BOOL bFlag = rcTemp.IntersectRect(rcItem, rcView);
			if (bFlag)
			{
				return i;
			}
		}

		return -1;
	}
	else
	{
		for (int i = GetItemCount() - 1; i >= 0; --i)
		{
			CRect rcItem;
			GetItemRect(i, rcItem, LVIR_SELECTBOUNDS);

			if (InsideRect(rcItem, rcView))
			{
				return i;
			}
		}

		return -1;
	}
}

struct  DragDropCompareLPARAM
{
	std::vector<int> *pv;
	DWORD dwInsertTarget;
};

static int CALLBACK DragDropCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	DragDropCompareLPARAM *pData = (DragDropCompareLPARAM *)lParamSort;
	THUMBNAIL_ITEMDATA *pItemData1 = (THUMBNAIL_ITEMDATA *)lParam1;
	THUMBNAIL_ITEMDATA *pItemData2 = (THUMBNAIL_ITEMDATA *)lParam2;

	size_t nSize = pData->pv->size();
	size_t i = 0;

	BOOL bItem1InDragItems = FALSE;
	for (i = 0; i < nSize; ++i)
	{
		if (pItemData1->dwDragDropCompareData == (*(pData->pv))[i])
		{
			bItem1InDragItems = TRUE;
			break;
		}
	}

	BOOL bItem2InDragItems = FALSE;
	for (i = 0; i < nSize; ++i)
	{
		if (pItemData2->dwDragDropCompareData == (*(pData->pv))[i])
		{
			bItem2InDragItems = TRUE;
			break;
		}
	}

	BOOL bItem1IsTarget = (pItemData1->dwDragDropCompareData == pData->dwInsertTarget);
	BOOL bItem2IsTarget = (pItemData2->dwDragDropCompareData == pData->dwInsertTarget);

	// 1.
	if (!bItem1InDragItems && !bItem1IsTarget && !bItem2InDragItems && !bItem2IsTarget)
	{
		return 0;
	}

	// 2.
	if (!bItem1InDragItems && !bItem1IsTarget && bItem2InDragItems && !bItem2IsTarget)
	{
		if (pItemData1->dwDragDropCompareData < pData->dwInsertTarget)
		{
			return -1;
		}
		else if (pItemData1->dwDragDropCompareData > pData->dwInsertTarget)
		{
			return 1;
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	// 3.
	if (!bItem1InDragItems && !bItem1IsTarget && !bItem2InDragItems && bItem2IsTarget)
	{
		return 0;
	}

	// 4.
	if (!bItem1InDragItems && !bItem1IsTarget && bItem2InDragItems && bItem2IsTarget)
	{
		return 0;
	}



	// 5.
	if (bItem1InDragItems && !bItem1IsTarget && !bItem2InDragItems && !bItem2IsTarget)
	{
		if (pItemData2->dwDragDropCompareData < pData->dwInsertTarget)
		{
			return 1;
		}
		else if (pItemData2->dwDragDropCompareData > pData->dwInsertTarget)
		{
			return -1;
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	// 6.
	if (bItem1InDragItems && !bItem1IsTarget && bItem2InDragItems && !bItem2IsTarget)
	{
		return 0;
	}

	// 7.
	if (bItem1InDragItems && !bItem1IsTarget && !bItem2InDragItems && bItem2IsTarget)
	{
		return -1;
	}

	// 8.
	if (bItem1InDragItems && !bItem1IsTarget && bItem2InDragItems && bItem2IsTarget)
	{
		return 0;
	}



	// 9.
	if (!bItem1InDragItems && bItem1IsTarget && !bItem2InDragItems && !bItem2IsTarget)
	{
		return 0;
	}

	// 10.
	if (!bItem1InDragItems && bItem1IsTarget && bItem2InDragItems && !bItem2IsTarget)
	{
		return 1;
	}

	// 11.
	if (!bItem1InDragItems && bItem1IsTarget && !bItem2InDragItems && bItem2IsTarget)
	{
		ASSERT(FALSE);
	}

	// 12.
	if (!bItem1InDragItems && bItem1IsTarget && bItem2InDragItems && bItem2IsTarget)
	{
		ASSERT(FALSE);
	}


	// 13.
	if (bItem1InDragItems && bItem1IsTarget && !bItem2InDragItems && !bItem2IsTarget)
	{
		if (pItemData2->dwDragDropCompareData < pData->dwInsertTarget)
		{
			return 1;
		}
		else if (pItemData2->dwDragDropCompareData > pData->dwInsertTarget)
		{
			return -1;
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	// 14.
	if (bItem1InDragItems && bItem1IsTarget && bItem2InDragItems && !bItem2IsTarget)
	{
		return 0;
	}

	// 15.
	if (bItem1InDragItems && bItem1IsTarget && !bItem2InDragItems && bItem2IsTarget)
	{
		ASSERT(FALSE);
	}

	// 16.
	if (bItem1InDragItems && bItem1IsTarget && bItem2InDragItems && bItem2IsTarget)
	{
		ASSERT(FALSE);
	}

	return 0;
}

LRESULT CThumbnailCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
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

	if (message == WM_DESTROY)
	{
		LRESULT lr = CWnd::WindowProc(message, wParam, lParam);
		if (m_list.GetSafeHwnd() != NULL)
		{
			DeleteAllItems();
		}
		return lr;
	}

	if (message == WM_PAINT)
	{
		CPaintDC dc(this); // device context for painting
		OnDraw(&dc);
	}
	if (message == WM_ERASEBKGND)
	{
		return TRUE;
	}

	if (message == WM_SIZE)
	{
		if (m_wndScrollEx.GetSafeHwnd() != NULL)
		{
			CRect rcScroll;
			GetScrollBarRect(rcScroll);
			m_wndScrollEx.MoveWindow(rcScroll);
			UpdateScrollingDisplay();
		}
	}

	if (message == WM_LBUTTONDOWN)
	{
		int nID = GetDlgCtrlID();
		WPARAM wParam2 = MAKEWPARAM(message, nID);
		GetParent()->SendMessage(WM_PARENTEVENT, wParam2, lParam);

		SetFocus();

		if (m_bHoldCtrlKey)
		{
			wParam |= MK_CONTROL;
		}
		DoLButtonDown(UINT(wParam), lParam);
	}

	if (message == WM_LBUTTONUP)
	{
		int nID = GetDlgCtrlID();
		WPARAM wParam2 = MAKEWPARAM(message, nID);
		GetParent()->SendMessage(WM_PARENTEVENT, wParam2, lParam);

		m_bMouseFirstMoveRecorded = FALSE;

		if (m_bIsDragSelect)
		{
			EndDragSelect(CPoint(lParam));

			// Release mouse capture.
			::ReleaseCapture();

			Invalidate();

			m_bIsDragSelect = FALSE;
		}
		else if (m_bDragItem)
		{
			m_bDragItem = FALSE;
		}
		else
		{
			if (m_bHoldCtrlKey)
			{
				wParam |= MK_CONTROL;
			}

			DoLButtonDown(UINT(wParam), lParam, FALSE);
		}
	}

	if (message == WM_MOUSEMOVE)
	{
		int nID = GetDlgCtrlID();
		WPARAM wParam2 = MAKEWPARAM(message, nID);
		GetParent()->SendMessage(WM_PARENTEVENT, wParam2, lParam);

		// If move the mouse with left button down, maybe
		// 1. select the items by drag rect
		// 2. or drag the selected items to move to another position.
		if ((UINT(wParam) & MK_LBUTTON) == MK_LBUTTON)
		{
			int nHitTest = HitTest(lParam);
			if (!m_bMouseFirstMoveRecorded)
			{
				m_bMouseFirstMoveRecorded = TRUE;
				m_ptCurDragSelectPoint = lParam;
				BOOL bDragItem = IsValidIndex(nHitTest);	// Only first mouse move recorder the action.
				m_bIsDragSelect = !bDragItem;

				// If drag select, m_bDragItem is must FALSE;
				if (m_bIsDragSelect)
				{
					m_bDragItem = FALSE;
				}
			}

			// If not drag select, may be drag item.
			if (!m_bIsDragSelect)
			{
				// If drag the item, only mouse move more than 10 pixel is valid.
				CPoint pt(lParam);
				if (abs(pt.x - m_ptCurDragSelectPoint.x) > 10 || abs(pt.y - m_ptCurDragSelectPoint.y) > 10)
				{
					m_bDragItem = TRUE;
				}
				else
				{
					m_bDragItem = FALSE;
				}
			}

			// 			// If the mouse position is at the side of the control, scroll the control.
			// 			CRect rcClient;
			// 			GetClientRect( rcClient);
			// 			CPoint pt(lParam);
			// 			if (pt.y < 0)
			// 			{
			// 				SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), NULL);
			// 			}
			// 			else if (pt.y > rcClient.Height())
			// 			{
			// 				SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), NULL);
			// 			}

			// Drag the selected items.
			if (m_bDragDropEnable && m_bDragItem && !m_bIsDragSelect)
			{
				NMHDR nmhdr;
				nmhdr.hwndFrom = this->m_hWnd;
				nmhdr.idFrom = ::GetDlgCtrlID(this->m_hWnd);
				nmhdr.code = LVN_BEGINDRAG;
				NMLISTVIEW lv;
				lv.hdr = nmhdr;
				lv.iItem = nHitTest;
				lv.iSubItem = 0;
				lv.uNewState = 0;
				lv.uOldState = 0;
				lv.uChanged = 0;
				lv.ptAction = CPoint(lParam);
				lv.lParam = 0;
				LRESULT lr = GetParent()->SendMessage(WM_NOTIFY, (WPARAM)(nmhdr.idFrom), (LPARAM)(&lv)); // lr == *pResult
				if (lr == 0)
				{
					BeginDrag(UINT(wParam), lParam);
				}
			}

			// Drag a rect to select items.
			if (m_bIsDragSelect && !m_bSingleSel)
			{
				SetCapture();

				CRect rcSelect;
				SetRectFromPoints(rcSelect, m_ptCurDragSelectPoint, CPoint(lParam));

				// Call derived class to update selection according to selection rect.
				SelectByRect(rcSelect);

				// Draw selection rectangle on top of it.
				CDC *pDC = BeginRepaint();
				ASSERT(pDC);
				CDC memDC;
				memDC.CreateCompatibleDC(pDC);

				int cx = rcSelect.Width();
				int cy = rcSelect.Height();
				if (cx != 0 && cy != 0)
				{
					CBitmap	newBmp;
					CBitmap *pOldBmp;
					VERIFY(newBmp.CreateCompatibleBitmap(pDC, cx, cy));
					ASSERT(newBmp.GetSafeHandle() != NULL);
					VERIFY(pOldBmp = memDC.SelectObject(&newBmp));

					memDC.FillSolidRect(0, 0, cx, cy, ::GetSysColor(COLOR_HIGHLIGHT));

					BLENDFUNCTION blendFunc;
					memset(&blendFunc, 0, sizeof(BLENDFUNCTION));
					blendFunc.AlphaFormat = 0 ;
					blendFunc.BlendFlags = 0;
					blendFunc.BlendOp = AC_SRC_OVER;
					blendFunc.SourceConstantAlpha = 99;

					::AlphaBlend(pDC->GetSafeHdc(), rcSelect.left, rcSelect.top, cx, cy, memDC.GetSafeHdc(), 0, 0, cx, cy, blendFunc);

					CGdiObject *pOld = pDC->SelectStockObject(NULL_BRUSH);
					ASSERT(pOld != NULL);
					pDC->Rectangle(rcSelect);
				}

				EndRepaint(pDC);        // Destroy repaint DC.
			}
		}
		else
		{
			int nItem = HitTest(lParam);
			if (IsValidIndex(nItem))
			{
				m_nHotItem = nItem;
				InvalidateRect(NULL);
			}
			else
			{
				m_nHotItem = -1;
				InvalidateRect(NULL);
			}
		}
	}

	if (message == WM_MOUSEWHEEL)
	{
		short zDelta = HIWORD(wParam);
		if (zDelta > 0)
		{
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), NULL);
		}
		else
		{
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), NULL);
		}
	}

	if (message == WM_LBUTTONDBLCLK || message == WM_RBUTTONDOWN || message == WM_RBUTTONUP || message == WM_RBUTTONDBLCLK)
	{
		int nID = GetDlgCtrlID();
		WPARAM wParam2 = MAKEWPARAM(message, nID);
		GetParent()->SendMessage(WM_PARENTEVENT, wParam2, lParam);
	}

	// Drag and drop
	if (message == DROPM_DRAGLEAVE)
	{
		CWnd::WindowProc(message, wParam, lParam);
		InvalidateRect(m_rcBack);
		return DROPEFFECT_NONE;
	}
	if (message == DROPM_DRAGOVER)
	{
		CWnd::WindowProc(message, wParam, lParam);

		CIUIDropMsgParam *pInfo = (CIUIDropMsgParam *)wParam;
		if (pInfo->m_pDataObject->IsDataAvailable(m_uClipFormat))
		{
			CRect rcSplit;
			CRect rcHit;
			int nHit = HitTestDrop(pInfo->m_point);
			int nSpace = GetSpace();
			CRect rcDisplay;
			GetViewRect(&rcDisplay);
			if (nHit == -1)
			{
				GetItemRect(GetItemCount() - 1, rcHit, LVIR_SELECTBOUNDS);
				ClientToWindow(rcHit);
				rcSplit.top = rcHit.top;
				rcSplit.bottom = rcHit.bottom;
				rcSplit.left = rcHit.right + nSpace + (m_sizeTotal.cx - m_sizeBkImage.cx) / 2;
				rcSplit.right = rcSplit.left + 2;
			}
			else
			{
				GetItemRect(nHit, rcHit, LVIR_SELECTBOUNDS);
				ClientToWindow(rcHit);
				rcSplit.top = rcHit.top;
				rcSplit.bottom = rcHit.bottom;
				rcSplit.left = rcHit.left - nSpace - (m_sizeTotal.cx - m_sizeBkImage.cx) / 2;
				rcSplit.right = rcSplit.left + 2;
			}


			static int snHit = -2;
			if (snHit != nHit)
			{
				InvalidateRect(m_rcBack);
				snHit = nHit;
				m_rcBack = rcSplit;
			}

			rcDisplay.left = 0;
			CRgn rgn;
			rgn.CreateRectRgnIndirect(rcDisplay);
			CClientDC dc(this);
			dc.SelectClipRgn(&rgn, RGN_AND);
			dc.FillSolidRect(rcSplit, RGB(145, 45, 45));

			return DROPEFFECT_COPY;
		}
		else
		{
			return DROPEFFECT_NONE;
		}
	}
	if (message == DROPM_DROPEX)
	{
		CWnd::WindowProc(message, wParam, lParam);
		return (DROPEFFECT) - 1;
	}
	if (message == DROPM_DROP)
	{
		CWnd::WindowProc(message, wParam, lParam);

		CIUIDropMsgParam *pInfo = (CIUIDropMsgParam *)wParam;
		if (pInfo->m_pDataObject->IsDataAvailable(m_uClipFormat))
		{
			HGLOBAL hMem = pInfo->m_pDataObject->GetGlobalData(m_uClipFormat);
			TCHAR *lp = (TCHAR *)GlobalLock((HGLOBAL) hMem);//lock source
			if (0 == StrCmp(lp, _T("DragItem")))
			{
				int nSelectedCount = GetSelectedCount();

				if (nSelectedCount > 0)
				{
					int  nItem = -1;
					std::vector<int> v;
					POSITION pos = GetFirstSelectedItemPosition();
					while (pos != NULL)
					{
						int nItem = GetNextSelectedItem(pos);
						v.push_back(nItem);
					}

					int nItemTarget = HitTestDrop(pInfo->m_point);
					ASSERT(nItemTarget < GetItemCount());

					// set dwDragDropCompareData for all items
					int nCount = m_list.GetItemCount();
					for (int i = 0; i < nCount; ++i)
					{
						THUMBNAIL_ITEMDATA *pData = (THUMBNAIL_ITEMDATA *)m_list.GetItemData(i);
						ASSERT(pData != NULL);
						if (pData != NULL)
						{
							pData->dwDragDropCompareData = i;
						}
					}

					DragDropCompareLPARAM sortParam;
					sortParam.pv = &v;
					sortParam.dwInsertTarget = nItemTarget;
#if(_MSC_VER <= 1200) // VC6.0
					m_list.SortItems(DragDropCompareProc, DWORD(&sortParam));
#else
					m_list.SortItems(DragDropCompareProc, DWORD_PTR(&sortParam));
#endif
				}
				Invalidate();

				NMHDR nmhdr;
				nmhdr.hwndFrom = this->m_hWnd;
				nmhdr.idFrom = ::GetDlgCtrlID(this->m_hWnd);
				nmhdr.code = LVN_ENDDRAG;
				NMLISTVIEW lv;
				lv.hdr = nmhdr;
				lv.iItem = 0;
				lv.iSubItem = 0;
				lv.uNewState = 0;
				lv.uOldState = 0;
				lv.uChanged = 0;
				lv.ptAction = CPoint(0);
				lv.lParam = 0;
				LRESULT lr = GetParent()->SendMessage(WM_NOTIFY, (WPARAM)(nmhdr.idFrom), (LPARAM)(&lv)); // lr == *pResult;
			}
			GlobalUnlock(hMem);//unlock source
			return TRUE;

		}
		else
		{
			return FALSE;
		}
	}

	if (message == WM_VSCROLL)
	{
		SCROLLINFO si;
		SCROLLINFO siBak;
		m_wndScrollEx.GetScrollInfo(&si);

		siBak = si;
		int nSBCode = LOWORD(wParam);
		int nPos = HIWORD(wParam);
		switch (nSBCode)
		{
		case SB_TOP:            // Scrolls to the upper left.
			si.nPos = si.nMin;
			break;

		case SB_BOTTOM:         // Scrolls to the lower right.
			si.nPos = si.nMax + 1 - si.nPage;
			break;

		case SB_ENDSCROLL:      // Ends scroll.
			break;

		case SB_LINEDOWN:       // Scrolls one line down.
			++si.nPos;
			break;

		case SB_LINEUP:         //Scrolls one line up.
			--si.nPos;
			break;

		case SB_PAGEDOWN:       //Scrolls one page down.
			si.nPos += si.nPage;
			break;

		case SB_PAGEUP:         //Scrolls one page up.
			si.nPos -= si.nPage;
			break;

		case SB_THUMBPOSITION:  //The user has dragged the scroll box (thumb) and released the mouse button. The nPos parameter indicates the position of the scroll box at the end of the drag operation.
			si.nPos = nPos;
			break;

		case SB_THUMBTRACK:     //The user is dragging the scroll box. This message is sent repeatedly until the user releases the mouse button. The nPos parameter indicates the position that the scroll box has been dragged to.
			si.nPos = nPos;
			break;
		}

		if (si.nPos < si.nMin)
		{
			si.nPos = si.nMin;
		}
		else if (si.nPos > (int)(si.nMax + 1 - si.nPage))
		{
			si.nPos = si.nMax - si.nPage + 1;
		}

		m_wndScrollEx.SetScrollInfo(&si);

		if (siBak.nPos != si.nPos)
		{
			RefreshDisplay();
		}
	}

	if (message == WM_NOTIFY)
	{
		int nID = int(wParam);
		NMHDR *pnmhdr = (NMHDR *)lParam;
		if (pnmhdr->code == LVN_DELETEALLITEMS)
		{
			SetFocusItem(-1);
			RefreshDisplay();

			if (nID == 0)// list
			{
				NMLISTVIEW *plv = (NMLISTVIEW *)lParam;
				NMHDR nmhdr;
				nmhdr.hwndFrom = this->m_hWnd; // set HWND
				nmhdr.idFrom = GetDlgCtrlID(); // set ID
				nmhdr.code = pnmhdr->code;
				NMLISTVIEW lv;
				lv.hdr = nmhdr;
				lv.iItem = plv->iItem;
				lv.iSubItem = plv->iSubItem;
				lv.uNewState = plv->uNewState;
				lv.uOldState = plv->uOldState;
				lv.uChanged = plv->uChanged;
				lv.ptAction = plv->ptAction;
				lv.lParam = plv->lParam;
				GetParent()->SendMessage(message, nmhdr.idFrom, LPARAM(&lv));
			}
		}
		if (pnmhdr->code == LVN_DELETEITEM)
		{
			NMLISTVIEW *p = (NMLISTVIEW *)lParam;
			int nFocusItem = GetFocusItem();
			if (nFocusItem == p->iItem)
			{
				SetFocusItem(nFocusItem--);
			}
			UpdateScrollingDisplay();
			Invalidate();

			if (nID == 0)
			{
				NMLISTVIEW *plv = (NMLISTVIEW *)lParam;
				NMHDR nmhdr;
				nmhdr.hwndFrom = this->m_hWnd;
				nmhdr.idFrom = GetDlgCtrlID();
				nmhdr.code = pnmhdr->code;
				NMLISTVIEW lv;
				lv.hdr = nmhdr;
				lv.iItem = plv->iItem;
				lv.iSubItem = plv->iSubItem;
				lv.uNewState = plv->uNewState;
				lv.uOldState = plv->uOldState;
				lv.uChanged = plv->uChanged;
				lv.ptAction = plv->ptAction;
				lv.lParam = plv->lParam;
				GetParent()->SendMessage(message, nmhdr.idFrom, LPARAM(&lv));
			}
		}
		if (pnmhdr->code == LVN_INSERTITEM)
		{
			LPNMLISTVIEW lpnmlv = (LPNMLISTVIEW)lParam;
			THUMBNAIL_ITEMDATA *pData = new THUMBNAIL_ITEMDATA;
#if(_MSC_VER <= 1200) // VC6.0
			m_list.SetItemData(lpnmlv->iItem, DWORD(pData));
#else
			m_list.SetItemData(lpnmlv->iItem, DWORD_PTR(pData));
#endif

			RefreshDisplay();

			if (nID == 0)
			{
				NMLISTVIEW *plv = (NMLISTVIEW *)lParam;
				NMHDR nmhdr;
				nmhdr.hwndFrom = this->m_hWnd;
				nmhdr.idFrom = GetDlgCtrlID();
				nmhdr.code = pnmhdr->code;
				NMLISTVIEW lv;
				lv.hdr = nmhdr;
				lv.iItem = plv->iItem;
				lv.iSubItem = plv->iSubItem;
				lv.uNewState = plv->uNewState;
				lv.uOldState = plv->uOldState;
				lv.uChanged = plv->uChanged;
				lv.ptAction = plv->ptAction;
				lv.lParam = plv->lParam;
				GetParent()->SendMessage(message, nmhdr.idFrom, LPARAM(&lv));
			}
		}
		if (pnmhdr->code == LVN_ITEMACTIVATE || pnmhdr->code == LVN_ITEMCHANGED || pnmhdr->code == LVN_ITEMCHANGING)
		{
			if (nID == 0)
			{
				NMLISTVIEW *plv = (NMLISTVIEW *)lParam;
				NMHDR nmhdr;
				nmhdr.hwndFrom = this->m_hWnd;
				nmhdr.idFrom = GetDlgCtrlID();
				nmhdr.code = pnmhdr->code;
				NMLISTVIEW lv;
				lv.hdr = nmhdr;
				lv.iItem = plv->iItem;
				lv.iSubItem = plv->iSubItem;
				lv.uNewState = plv->uNewState;
				lv.uOldState = plv->uOldState;
				lv.uChanged = plv->uChanged;
				lv.ptAction = plv->ptAction;
				lv.lParam = plv->lParam;
				GetParent()->SendMessage(message, nmhdr.idFrom, LPARAM(&lv));
			}
		}
		// can't receive, because m_list can't receive key down message(m_list isn't visible)
		if (pnmhdr->code == LVN_KEYDOWN)
		{
			if (nID == 0)
			{
				NMLVKEYDOWN *pkd = (NMLVKEYDOWN *)lParam;
				NMHDR nmhdr;
				nmhdr.hwndFrom = this->m_hWnd;
				nmhdr.idFrom = GetDlgCtrlID();
				nmhdr.code = pnmhdr->code;
				NMLVKEYDOWN kd;
				kd.hdr = nmhdr;
				kd.wVKey = pkd->wVKey;
				kd.flags = pkd->flags;
				GetParent()->SendMessage(message, nmhdr.idFrom, LPARAM(&kd));
			}
		}
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

CScrollBarEx *CThumbnailCtrl::GetScrollBarEx()
{
	return &m_wndScrollEx;
}

void CThumbnailCtrl::SetScrollBarExBk(
	UINT uMask,
	LPCTSTR lpszImageNameScrollbarExN,
	LPCTSTR lpszImageNameScrollbarExH,
	LPCTSTR lpszImageNameScrollbarExS,
	LPCTSTR lpszImageNameScrollbarExD)
{
	m_wndScrollEx.SetBkBitmaps(
		uMask,
		lpszImageNameScrollbarExN,
		lpszImageNameScrollbarExH,
		lpszImageNameScrollbarExS,
		lpszImageNameScrollbarExD);
}

void CThumbnailCtrl::SetScrollBarExThumb(
	UINT uMask,
	LPCTSTR lpszImageNameScrollbarExThumbN,
	LPCTSTR lpszImageNameScrollbarExThumbH,
	LPCTSTR lpszImageNameScrollbarExThumbS,
	LPCTSTR lpszImageNameScrollbarExThumbD)
{
	m_wndScrollEx.SetThumbBitmaps(
		uMask,
		lpszImageNameScrollbarExThumbN,
		lpszImageNameScrollbarExThumbH,
		lpszImageNameScrollbarExThumbS,
		lpszImageNameScrollbarExThumbD);
}

void CThumbnailCtrl::GetScrollBarExThumb(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameScrollbarExThumbN, CString *pstrImageNameScrollbarExThumbH,
	CString *pstrImageNameScrollbarExThumbS, CString *pstrImageNameScrollbarExThumbD)
{
	m_wndScrollEx.GetThumbBitmaps(
		uMask,
		pbCombineImage,
		pstrCombineImageName,
		pstrImageNameScrollbarExThumbN, pstrImageNameScrollbarExThumbH,
		pstrImageNameScrollbarExThumbS, pstrImageNameScrollbarExThumbD);
}

void CThumbnailCtrl::SetScrollBarExLTUPButton(int nValue)
{
	m_wndScrollEx.SetLTUPButton(nValue);
}

int CThumbnailCtrl::GetScrollBarExLTUPButton()
{
	return m_wndScrollEx.GetLTUPButton();
}

void CThumbnailCtrl::SetScrollBarExRTDNButton(int nValue)
{
	m_wndScrollEx.SetRTDNButton(nValue);
}

int CThumbnailCtrl::GetScrollBarExRTDNButton()
{
	return m_wndScrollEx.GetRTDNButton();
}

int CThumbnailCtrl::HoldCtrlKey(BOOL bHold)
{
	m_bHoldCtrlKey = bHold;
	return 0;
}

BOOL CThumbnailCtrl::IsHoldCtrlKey()
{
	return m_bHoldCtrlKey;
}

int CThumbnailCtrl::GetCountPerPage() const
{
	CRect rcView;
	GetViewRect(rcView);
	int nRet = rcView.Height() / m_nItemHeight;
	return nRet;
}

int CThumbnailCtrl::SetItemHeight(int nItemHeight)
{
	m_nItemHeight = nItemHeight;
	return 0;
}

int CThumbnailCtrl::GetItemHeight() const
{
	return m_nItemHeight;
}

BOOL CThumbnailCtrl::PreTranslateMessage(MSG *pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		NMHDR nmhdr;
		nmhdr.hwndFrom = this->m_hWnd;
		nmhdr.idFrom = GetDlgCtrlID();
		nmhdr.code = LVN_KEYDOWN;
		NMLVKEYDOWN kd;
		kd.hdr = nmhdr;
		kd.wVKey = WORD(pMsg->wParam);
		kd.flags = 0;
		LRESULT lr = GetParent()->SendMessage(WM_NOTIFY, (WPARAM)(nmhdr.idFrom), (LPARAM)(&kd)); // lr == *pResult;
		if (lr == 0)
		{
			if (GetItemCount() > 0)
			{
				switch (pMsg->wParam)
				{
				case VK_LEFT:
				case VK_UP:
				case VK_RIGHT:
				case VK_DOWN:
					OnMovementKey(UINT(pMsg->wParam));
					break;
				default:
					break;
				}
			}
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}
