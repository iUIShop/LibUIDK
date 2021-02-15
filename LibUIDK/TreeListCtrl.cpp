// TreeListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _DEBUG_SHOWOLDSCROLLBAR 0

#define WM_GETREDRAWFLAG WM_TREELIST+1 // wParam: 0, not used;  lParam: redraw flag
#define WM_SETREDRAWFLAG WM_TREELIST+2


struct TREELISTMEMBER
{
	TREELISTMEMBER()
	{
		m_pHeader = NULL;
		m_crSelectedBK = RGB(0, 0, 128);
		int i = 0;
		for (i = 0; i < 2; ++i)
		{
			m_himgBk[i] = NULL;
		}
		m_dwHoverTime = HOVER_DEFAULT;
		m_bHoverRepeat = FALSE;
		for (i = 0; i < 2; ++i)
		{
			m_hIUIFont[i] = NULL;
		}
		m_bRedrawFlag = TRUE;
	}

	int Release()
	{
		m_pHeader = NULL;
		m_crSelectedBK = RGB(0, 0, 128);
		int i = 0;
		for (i = 0; i < 2; ++i)
		{
			ReleaseIUIImage(m_himgBk[i]);
			m_himgBk[i] = NULL;
		}
		m_dwHoverTime = HOVER_DEFAULT;
		m_bHoverRepeat = FALSE;
		for (i = 0; i < 2; ++i)
		{
			ReleaseIUIFontInternal(m_hIUIFont[i]);
			m_hIUIFont[i] = NULL;
		}
		m_bRedrawFlag = TRUE;
	}

	CListCtrl m_List; //temp to store data
	CSkinHeaderCtrl *m_pHeader;
	COLORREF m_crSelectedBK;
	HIUIIMAGE m_himgBk[2];
	DWORD m_dwHoverTime;
	BOOL m_bHoverRepeat;
	HIUIFONT m_hIUIFont[2];
	BOOL m_bRedrawFlag; // only change this value by "USER", the CTreeList must not change it.
};

/////////////////////////////////////////////////////////////////////////////
// CTreeList

CTreeList::CTreeList()
{
	m_pBindWnd = this;

	m_pMember = new TREELISTMEMBER;
	TREELISTMEMBER *pMember = (TREELISTMEMBER *)m_pMember;
}

CTreeList::~CTreeList()
{
}


BEGIN_MESSAGE_MAP(CTreeList, CTreeCtrl)
	//{{AFX_MSG_MAP(CTreeList)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeList message handlers

int CTreeList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// TODO: Add your specialized creation code here

	TREELISTMEMBER *pMember = (TREELISTMEMBER *)m_pMember;

	// create ListCtrl for restore data
	if (!pMember->m_List.Create(WS_CHILD, CRect(0, 0, 0, 0), this, 1112))
	{
		return -1;
	}

	// Set background color to RGB(255, 255, 255), default is RGB(0, 0, 0).
	SetBkColor(RGB(255, 255, 255));

	return 0;
}

int CTreeList::SetHeaderPtr(CSkinHeaderCtrl *pHeader)
{
	TREELISTMEMBER *pMember = (TREELISTMEMBER *)m_pMember;

	ASSERT(pHeader != NULL);
	pMember->m_pHeader = pHeader;

	return 0;
}

BOOL CTreeList::SetBkImage(UINT uMask, LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameD)
{
	TREELISTMEMBER *pMember = (TREELISTMEMBER *)m_pMember;

	m_bBkCombine = false;
	IUISetControlImage2(uMask, pMember->m_himgBk, lpszImageNameN, lpszImageNameD);

	return TRUE;
}

int CTreeList::GetBkImage(UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameD)
{
	TREELISTMEMBER *pMember = (TREELISTMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bBkCombine;
	IUIGetControlImage2(uMask, m_himgCombineBk, pMember->m_himgBk, *pbCombineImage,
		pstrCombineImageName, pstrImageNameN, pstrImageNameD);

	return 0;
}

int CTreeList::SetTextFont(UINT uMask, LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDD /* = NULL*/)
{
	TREELISTMEMBER *pMember = (TREELISTMEMBER *)m_pMember;

	IUISetControlFont2(uMask, pMember->m_hIUIFont, lpszFontIDN, lpszFontIDD);

	return 0;
}

int CTreeList::GetTextFont(UINT uMask, CString *pstrFontIDN, CString *pstrFontIDD/* = NULL*/) const
{
	TREELISTMEMBER *pMember = (TREELISTMEMBER *)m_pMember;

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

int CTreeList::SetHoverTime(DWORD dwHoverTime, BOOL bRepeat/*=FALSE*/)
{
	TREELISTMEMBER *pMember = (TREELISTMEMBER *)m_pMember;

	pMember->m_dwHoverTime = dwHoverTime;
	pMember->m_bHoverRepeat = bRepeat;

	return 0;
}

int CTreeList::GetHoverTime(DWORD *pdwHoverTime, BOOL *pbRepeat) const
{
	TREELISTMEMBER *pMember = (TREELISTMEMBER *)m_pMember;

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

int CTreeList::DrawItem(CDC *pDC, HTREEITEM hTI)
{
	TREELISTMEMBER *pMember = (TREELISTMEMBER *)m_pMember;

	DRAWITEMSTRUCT dis;
	dis.CtlType = ODT_TREELIST;
	dis.CtlID = GetDlgCtrlID();
	dis.itemID = UINT(UINT_PTR(hTI));
	dis.hwndItem = m_hWnd;
	dis.hDC = pDC->GetSafeHdc();
	dis.itemData = GetItemData(hTI);
	CRect rcItem;
	GetItemRect(hTI, rcItem, FALSE);
	dis.rcItem = rcItem;

	BOOL bParentDraw = (BOOL)GetParent()->SendMessage(WM_DRAWITEMEX, dis.CtlID, LPARAM(&dis));
	if (bParentDraw)
	{
		return 1;
	}

	// draw selected item.
	UINT nState = GetItemState(hTI, TVIS_SELECTED);
	if (TVIS_SELECTED & nState)
	{
		pDC->FillSolidRect(rcItem, pMember->m_crSelectedBK);
	}

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

	pDC->SetBkMode(TRANSPARENT);
	for (int i = 0; i < pMember->m_pHeader->GetItemCount(); ++i)
	{
		CString strText = GetItemTextEx(hTI, i);

		CRect rcHeader;
		pMember->m_pHeader->GetItemRect(i, rcHeader);

		CRect rcRgn; // text bound.
		if (i == 0)
		{
			CRect rcText;
			GetItemRect(hTI, rcText, TRUE);
			rcRgn = rcText;
		}
		else
		{
			rcRgn = rcItem;
			rcRgn.left = rcHeader.left + 5;
		}
		rcRgn.right = rcHeader.right;

		pDC->DrawText(strText, rcRgn, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_VCENTER);
	}
	pDC->SelectObject(hFontOld);

	return 0;
}

void CTreeList::OnPaint()
{
	TREELISTMEMBER *pMember = (TREELISTMEMBER *)m_pMember;

	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CRect rcClient;
	GetClientRect(&rcClient);

	// draw background
	CDC dcBk;
	dcBk.CreateCompatibleDC(&dc);
	CBitmap bmpMask;
	bmpMask.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	ASSERT(bmpMask.GetSafeHandle() != NULL);
	CBitmap *pBmpMaskOld = dcBk.SelectObject(&bmpMask);

	if (pMember->m_himgBk[0] != NULL)
	{
		CRect rcWin;
		GetWindowRect(rcWin);
		GetParent()->ScreenToClient(rcWin);

		if (IsWindowEnabled())
		{
			IUINineGridBlt(dcBk.GetSafeHdc(), 0, 0, rcClient.Width(), rcClient.Height(),
				pMember->m_himgBk[0], m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
		}
		else
		{
			IUINineGridBlt(dcBk.GetSafeHdc(), 0, 0, rcClient.Width(), rcClient.Height(),
				pMember->m_himgBk[1], m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
		}
	}
	else
	{
		COLORREF cr = GetBkColor();
		dcBk.FillSolidRect(rcClient, cr);
	}

	// draw item(text)
	COLORREF cr = GetTextColor();
	dcBk.SetTextColor(cr);
	UINT uVisibleCount = GetVisibleCount();
	HTREEITEM hTI = GetFirstVisibleItem();
	for (UINT i = 0; (i < uVisibleCount + 1) && (hTI != NULL); ++i)
	{
		DrawItem(&dcBk, hTI);
		hTI = GetNextVisibleItem(hTI);
	}

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcBk, 0, 0, SRCCOPY);
	dcBk.SelectObject(pBmpMaskOld);

	// Do not call CTreeCtrl::OnPaint() for painting messages
}

int CTreeList::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat /*= LVCFMT_LEFT*/, int nWidth/* = -1*/, int nSubItem/* = -1*/)
{
	TREELISTMEMBER *pMember = (TREELISTMEMBER *)m_pMember;

	HDITEM hdItem;
	hdItem.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
	hdItem.cxy = nWidth;
	hdItem.fmt = HDF_STRING | HDF_LEFT;
	CString strColumn(lpszColumnHeading);
	hdItem.pszText = strColumn.GetBuffer(strColumn.GetLength());
	pMember->m_pHeader->InsertItem(nCol, &hdItem);

	return pMember->m_List.InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
}

CListCtrl *CTreeList::GetListCtrl()
{
	TREELISTMEMBER *pMember = (TREELISTMEMBER *)m_pMember;

	return &pMember->m_List;
}

CSkinHeaderCtrl *CTreeList::GetHeaderCtrl()
{
	TREELISTMEMBER *pMember = (TREELISTMEMBER *)m_pMember;

	return pMember->m_pHeader;
}

int CTreeList::ListIndexByHTREEITEM(HTREEITEM hTI) const
{
	TREELISTMEMBER *pMember = (TREELISTMEMBER *)m_pMember;

	int nItem = -1;
	int nCount = pMember->m_List.GetItemCount();
	for (int i = nCount - 1; i >= 0; --i)
	{
#if(_MSC_VER <= 1200) // VC6.0
		DWORD dwData = pMember->m_List.GetItemData(i);
#else
		DWORD_PTR dwData = pMember->m_List.GetItemData(i);
#endif
		if ((HTREEITEM)dwData == hTI)
		{
			nItem = i;
			return nItem;
		}
	}

	return nItem;
}

BOOL CTreeList::SetItemTextEx(HTREEITEM hTI, int nCol, LPCTSTR lpszText)
{
	TREELISTMEMBER *pMember = (TREELISTMEMBER *)m_pMember;

	int nItem = ListIndexByHTREEITEM(hTI);
	return pMember->m_List.SetItemText(nItem, nCol, lpszText);
}

CString CTreeList::GetItemTextEx(HTREEITEM hTI, int nCol) const
{
	TREELISTMEMBER *pMember = (TREELISTMEMBER *)m_pMember;

	if (nCol >= pMember->m_pHeader->GetItemCount() || nCol < 0)
	{
		return _T("");
	}
	else if (nCol == 0)
	{
		return CTreeCtrl::GetItemText(hTI);
	}
	else
	{
		int nItem = ListIndexByHTREEITEM(hTI);
		return pMember->m_List.GetItemText(nItem, nCol);
	}
}

int CTreeList::SetItemHeightEx(HTREEITEM hItem, UINT cyItemHeight)
{
	int nOldHeight = GetItemHeight();
	TVITEMEX itemEx;
	itemEx.mask = TVIF_INTEGRAL;
	itemEx.hItem = hItem;
	itemEx.iIntegral = cyItemHeight / nOldHeight;
	BOOL bRet = (BOOL)SendMessage(TVM_SETITEM, 0, LPARAM(&itemEx));

	return bRet ? 0 : -1;
}

UINT CTreeList::GetItemHeightEx(HTREEITEM hItem) const
{
	int nOldHeight = GetItemHeight();
	TVITEMEX itemEx;
	itemEx.mask = TVIF_INTEGRAL;
	itemEx.hItem = hItem;

	CTreeList *pThis = (CTreeList *)this;
	BOOL bRet = (BOOL)pThis->SendMessage(TVM_GETITEM, 0, LPARAM(&itemEx));
	if (!bRet)
	{
		return -1;
	}

	return itemEx.iIntegral * nOldHeight;
}

#ifndef TVS_NOHSCROLL
#define TVS_NOHSCROLL 0x8000	// Version 5.80
#endif

BOOL CTreeList::PreCreateWindow(CREATESTRUCT &cs)
{
	// TODO: Add your specialized code here and/or call the base class

	cs.style |= TVS_NOHSCROLL;	// Disables horizontal scrolling in the control.
	//The control will not display any horizontal scroll bars
	return CTreeCtrl::PreCreateWindow(cs);
}

LRESULT CTreeList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	TREELISTMEMBER *pMember = (TREELISTMEMBER *)m_pMember;

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
#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
	case WM_MOUSEWHEEL:
#endif /* if (_WIN32_WINNT < 0x0400) */
	case WM_MOUSEHOVER:
	{
		int nID = GetDlgCtrlID();
		WPARAM wParam2 = MAKEWPARAM(message, nID);
		GetParent()->SendMessage(WM_PARENTEVENT, wParam2, lParam);
	}

	break;
	default:
		break;
	}

	// When use layered window, the window not redraw when its child controls hide or display
	// to solve it, handle the WM_WINDOWPOSCHANGED message.
	if (message == WM_WINDOWPOSCHANGED)
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

	else if (message == WM_ERASEBKGND)
	{
		return 1;
	}

	else if (message == WM_TIMER)
	{
		if (wParam == IDT_SCROLLWAIT) // click a item witch part visual, when it scroll to full visual
		{
			Invalidate();
			GetParent()->SendMessage(message, wParam, lParam);
		}
	}

	// when "USER" call SetRedraw(FALSE) or send WM_SETREDRAW message with FALSE,
	// after receive WM_VSCROLL and TVM_EXPAND message, the "CTreeList" must not call SetRedraw(TRUE).
	else if (message == WM_SETREDRAW)
	{
		pMember->m_bRedrawFlag = BOOL(wParam);
	}

	else if (message == WM_GETREDRAWFLAG)
	{
		return pMember->m_bRedrawFlag;
	}
	else if (message == WM_SETREDRAWFLAG)
	{
		pMember->m_bRedrawFlag = BOOL(lParam);
	}

	else if (message == WM_VSCROLL)
	{
		BOOL bVisible = IsWindowVisible();
		if (bVisible)
		{
			if (pMember->m_bRedrawFlag)
			{
				SetRedraw(FALSE);
				pMember->m_bRedrawFlag = TRUE; // Don't change the m_bRedrawFlag
			}
		}
		LRESULT lr = CTreeCtrl::WindowProc(message, wParam, lParam);
		if (bVisible)
		{
			if (pMember->m_bRedrawFlag)
			{
				SetRedraw(TRUE);
				Invalidate(NULL);
				UpdateWindow();
				pMember->m_bRedrawFlag = TRUE;
			}
		}

		GetParent()->SendMessage(message, wParam, 0);
		return lr;
	}

	else if (message == TVM_INSERTITEM)
	{
		HTREEITEM hItem = (HTREEITEM)CTreeCtrl::WindowProc(message, wParam, lParam);
		if (hItem == NULL)
		{
			return NULL;
		}

		int nItem = pMember->m_List.InsertItem(0, _T(""));
		if (nItem == -1)
		{
			DeleteItem(hItem);
			return NULL;
		}

#if(_MSC_VER <= 1200) // VC6.0
		if (!pMember->m_List.SetItemData(nItem, (DWORD)hItem))
#else
		if (!pMember->m_List.SetItemData(nItem, (DWORD_PTR)hItem))
#endif
		{
			DeleteItem(hItem);
			pMember->m_List.DeleteItem(nItem);
			return NULL;
		}

		return LRESULT(hItem);
	}

	else if (message == TVM_EXPAND)
	{
		BOOL bVisible = IsWindowVisible();
		if (bVisible)
		{
			if (pMember->m_bRedrawFlag)
			{
				SetRedraw(FALSE);
				pMember->m_bRedrawFlag = TRUE; // Don't change the m_bRedrawFlag
			}
		}
		LRESULT lr = CTreeCtrl::WindowProc(message, wParam, lParam);
		if (bVisible)
		{
			if (pMember->m_bRedrawFlag)
			{
				SetRedraw(TRUE);
				Invalidate();
				UpdateWindow();
				pMember->m_bRedrawFlag = TRUE;
			}
		}

		CTreeListCtrl *pParent = (CTreeListCtrl *)GetParent();
		if (pParent->GetSafeHwnd() != NULL)
		{
			pParent->UpdateScrollingDisplay();
		}
		return lr;
	}

	else if (message == WM_HSCROLL)
	{
		InvalidateRect(NULL);
	}

	else if (message == WM_MOUSEMOVE)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_HOVER;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = pMember->m_dwHoverTime;
		::TrackMouseEvent(&tme);
	}

	else if (message == WM_MOUSEHOVER)
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

	switch (message)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:

	case TVM_SORTCHILDRENCB:
	case TVM_SORTCHILDREN:

	case WM_MOUSEWHEEL:

	case TVM_INSERTITEM:
	case TVM_ENSUREVISIBLE:
	case TVM_DELETEITEM:
	{
		LRESULT lr = CTreeCtrl::WindowProc(message, wParam, lParam);
		GetParent()->SendMessage(message, wParam, lParam);
		return lr;
	}
	break;
	default:
		break;
	}

	return CTreeCtrl::WindowProc(message, wParam, lParam);
}



/////////////////////////////////////////////////////////////////////////////
// CTreeListCtrl

struct TREELISTCTRLMEMBER
{
	TREELISTCTRLMEMBER()
	{
		m_bUseScrollBarEx = FALSE;
		m_nScrollWidth = 16;
		m_nHeaderHeight = 24;
	}

	~TREELISTCTRLMEMBER()
	{

	}

	CTreeList m_wndTreeList;
	CSkinHeaderCtrl m_wndHeader;
	CScrollBarEx m_wndScrollEx;
	BOOL m_bUseScrollBarEx;
	int m_nScrollWidth;
	int m_nHeaderHeight;
};


CTreeListCtrl::CTreeListCtrl()
{
	m_pBindWnd = this;

	m_pMember = new TREELISTCTRLMEMBER;
}

CTreeListCtrl::~CTreeListCtrl()
{
	if (m_pMember != NULL)
	{
		delete (TREELISTCTRLMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

int CTreeListCtrl::BindStyle(LPCTSTR lpszStyleID)
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

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_TREELIST, lpszStyleID);
	return BindStyle(pCtrlProp);

	return 0;
}

int CTreeListCtrl::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
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
	if (ect != CT_STYLE_TREELIST && ect != CT_TREELIST)
	{
		return -3;
	}

	TREELISTPROPERTIES *pTreeListProp = (TREELISTPROPERTIES *)pCtrlProp;

	if (pTreeListProp->m_bSpecifyBackgroundImages)
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
			SetBkImage(CONTROL_STATE_CHECKED_ALL,
				strImageName[1], strImageName[2]);
		}

		SetBkImageResizeMode(pCtrlProp->m_eBkImageResizeMode);
		SetBkImageResizePoint(pCtrlProp->m_ptImageResize);
	}

	return 0;
}

BEGIN_MESSAGE_MAP(CTreeListCtrl, CWnd)
	//{{AFX_MSG_MAP(CTreeListCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTreeListCtrl message handlers

BOOL CTreeListCtrl::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, int nHeaderHeight/*=24*/, BOOL bUseCustomScroll/*=FALSE*/, int nScrollWidth/*=16*/)
{
	TREELISTCTRLMEMBER *pMember = (TREELISTCTRLMEMBER *)m_pMember;

	pMember->m_bUseScrollBarEx = bUseCustomScroll;
	pMember->m_nScrollWidth = nScrollWidth;
	pMember->m_nHeaderHeight = nHeaderHeight;

	CString strMyClass;
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;
	strMyClass = WC_TREELISTVIEW;
	wndcls.lpszClassName = strMyClass.GetBuffer(strMyClass.GetLength());

	// Register the new class and exit if it fails
	if (!AfxRegisterClass(&wndcls))
	{
		TRACE(_T("[LibUIDK]===Class Registration Failed\n"));
		return FALSE;
	}

	CWnd *pWnd = this;
	return pWnd->Create(WC_TREELISTVIEW, NULL, dwStyle, rect, pParentWnd, nID);
}


int CTreeListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// TODO: Add your specialized creation code here
	TREELISTCTRLMEMBER *pMember = (TREELISTCTRLMEMBER *)m_pMember;

	CRect rcClient;
	GetClientRect(rcClient);

	// create Tree
	if (pMember->m_bUseScrollBarEx)
	{
		int nXScroll = ::GetSystemMetrics(SM_CXVSCROLL);
		CRect rcTreeList = CRect(0, pMember->m_nHeaderHeight,
				lpCreateStruct->cx + nXScroll, lpCreateStruct->cy);
		if (!pMember->m_wndTreeList.Create(lpCreateStruct->style, rcTreeList, this, IDW_TREELIST))
		{
			return -1;
		}
	}
	else
	{
		CRect rcTreeList = CRect(0, pMember->m_nHeaderHeight, lpCreateStruct->cx, lpCreateStruct->cy);
		if (!pMember->m_wndTreeList.Create(lpCreateStruct->style, rcTreeList, this, IDW_TREELIST))
		{
			return -1;
		}
	}

	// create scroll bar
	if (pMember->m_bUseScrollBarEx)
	{
		CRect rcScroll;
		GetClientRect(rcScroll);
		rcScroll.top += pMember->m_nHeaderHeight;
		rcScroll.left = rcScroll.right - pMember->m_nScrollWidth;
		BOOL bRet = pMember->m_wndScrollEx.Create(WS_CHILD | SBS_VERT | SBS_LEFTALIGN, rcScroll, this, 1111);
		if (!bRet)
		{
			return -1;
		}
	}

	// style of header control
	LONG lStyle = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
	// Set the style of header within tree list control to same as it within CListCtrl.
	// only use HDS_BUTTONS style can receive HDN_ITEMCLICK notify
	DWORD dwHeaderStyle = WS_CHILD | WS_VISIBLE | HDS_FULLDRAG | HDS_HORZ | HDS_BUTTONS | HDS_DRAGDROP;
	if (lStyle & WS_BORDER)
	{
		dwHeaderStyle |= WS_BORDER;
	}

	CRect rcHeader(0, 0, lpCreateStruct->cx, pMember->m_nHeaderHeight);
	if (!pMember->m_wndHeader.Create(dwHeaderStyle, rcHeader, this, IDW_TREELISTHEADER))
	{
		return -1;
	}
	pMember->m_wndHeader.SetTreeCtrlEx(&pMember->m_wndTreeList);
	pMember->m_wndTreeList.SetHeaderPtr(&pMember->m_wndHeader);
	pMember->m_wndHeader.SetHeaderHeight(pMember->m_nHeaderHeight);

	CFont *pFont = GetFont();
	pMember->m_wndHeader.CHeaderCtrl::SetFont(pFont);

	return 0;
}

BOOL CTreeListCtrl::SetBkImage(UINT uMask, LPCTSTR lpszImageNameBkN, LPCTSTR lpszImageNameBkD)
{
	TREELISTCTRLMEMBER *pMember = (TREELISTCTRLMEMBER *)m_pMember;

	return pMember->m_wndTreeList.SetBkImage(uMask, lpszImageNameBkN, lpszImageNameBkD);
}

int CTreeListCtrl::GetBkImage(UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameBkN, CString *pstrImageNameBkD)
{
	TREELISTCTRLMEMBER *pMember = (TREELISTCTRLMEMBER *)m_pMember;

	return pMember->m_wndTreeList.GetBkImage(uMask, pbCombineImage, pstrCombineImageName,
			pstrImageNameBkN, pstrImageNameBkD);
}

int CTreeListCtrl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem)
{
	TREELISTCTRLMEMBER *pMember = (TREELISTCTRLMEMBER *)m_pMember;

	return pMember->m_wndTreeList.InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
}

HTREEITEM CTreeListCtrl::InsertItem(LPCTSTR lpszItem, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	TREELISTCTRLMEMBER *pMember = (TREELISTCTRLMEMBER *)m_pMember;

	return pMember->m_wndTreeList.InsertItem(lpszItem, hParent, hInsertAfter);
}

int ArrangeControl(CTreeListCtrl *pWnd)
{
	CRect rect;
	pWnd->GetClientRect(rect);
	int cx = rect.Width();
	int cy = rect.Height();
	int nHeaderHeight = 0;

	CSkinHeaderCtrl *pHeader = pWnd->GetHeaderCtrl();
	CTreeList *pTreeList = pWnd->GetTreeListWnd();
	CScrollBarEx *pScroll = pWnd->GetScrollBarEx();
	BOOL bUseScrollBarEx = pWnd->IsUseScrollBarEx();
	CRect rcScroll;
	if (bUseScrollBarEx)
	{
		pScroll->GetClientRect(rcScroll);
	}

	if (pHeader->GetSafeHwnd() != NULL)
	{
		nHeaderHeight = pHeader->GetHeaderHeight();
		pHeader->SetWindowPos(NULL, 0, 0, cx, nHeaderHeight, SWP_NOMOVE | SWP_NOZORDER);
	}

	BOOL bShowScroll = FALSE;
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	BOOL bRet = pTreeList->GetScrollInfo(SB_VERT, &si);

	if (bRet)
	{
		// On XP Style, after create tree, the pTreeList->GetScrollInfo return TRUE,
		// but on classic style, it return FALSE, use si.nPage==0 to differentiate it.
		if (si.nPage == si.nMax + 1 || si.nPage == 0)
		{
			bShowScroll = FALSE;
		}
		else
		{
			bShowScroll = TRUE;
		}
	}

	if (pTreeList->GetSafeHwnd() != NULL)
	{
		int nXScroll = ::GetSystemMetrics(SM_CXVSCROLL);
		if (bUseScrollBarEx)
		{
			if (bShowScroll)
			{
				// first set RGN for tree control, otherwise the old scroll will be show.
				CRect rcTreeList;
				pTreeList->GetClientRect(rcTreeList);
				rcTreeList.right = cx - rcScroll.Width();
				rcTreeList.bottom = cy - nHeaderHeight;
#if (_DEBUG_SHOWOLDSCROLLBAR==0)
				CRgn rgn;
				rgn.CreateRectRgnIndirect(rcTreeList);
				pTreeList->SetWindowRgn(rgn, TRUE);
#endif
				// then move the tree.
				int nWidth = cx - rcScroll.Width() + nXScroll;
				pTreeList->SetWindowPos(NULL, 0, 0, nWidth, cy - nHeaderHeight, SWP_NOMOVE | SWP_NOZORDER);
			}
			else
			{
				// set the tree control's width more than its parent to nXScroll,
				// if not, when get TVN_ITEMEXPANDING message, the old scroll bar will appear first.
				pTreeList->SetWindowPos(NULL, 0, 0, cx + nXScroll, cy - nHeaderHeight, SWP_NOMOVE | SWP_NOZORDER);
				pTreeList->SetWindowRgn(NULL, TRUE);
			}
		}
		else
		{
			pTreeList->SetWindowPos(NULL, 0, 0, cx, cy - nHeaderHeight, SWP_NOMOVE | SWP_NOZORDER);
		}
	}

	if (bUseScrollBarEx && pScroll->GetSafeHwnd() != NULL)
	{
		if (bShowScroll)
		{
#if (_DEBUG_SHOWOLDSCROLLBAR==0)
			pScroll->SetWindowPos(NULL, cx - rcScroll.Width(), nHeaderHeight, rcScroll.Width(), cy - nHeaderHeight, SWP_NOZORDER);
#else
			int nXScroll = ::GetSystemMetrics(SM_CXVSCROLL);
			pScroll->SetWindowPos(NULL, cx - rcScroll.Width() + nXScroll, nHeaderHeight, rcScroll.Width(), cy - nHeaderHeight, SWP_NOZORDER);
#endif
		}
		else
		{
			pScroll->ShowWindow(SW_HIDE);
		}
	}

	return 0;
}

LRESULT CTreeListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	TREELISTCTRLMEMBER *pMember = (TREELISTCTRLMEMBER *)m_pMember;

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

	if (message == WM_PARENTEVENT)
	{
		GetParent()->SendMessage(message, wParam, lParam);
	}

	else if (message == WM_KEYUP)
	{
		switch (wParam)
		{
		case VK_PRIOR:
		case VK_NEXT:
		case VK_END:
		case VK_HOME:
		case VK_UP:
		case VK_DOWN:
		case VK_LEFT:
		case VK_RIGHT:
			UpdateScrollingDisplay();
			break;
		default:
			break;
		}
	}

	else if (message == WM_TIMER)
	{
		if (wParam == IDT_SCROLLWAIT) // click a item witch part visual, when it scroll to full visual
		{
			UpdateScrollingDisplay();
		}
	}

	else if (message == WM_SIZE)
	{
		ArrangeControl(this);
		UpdateScrollingDisplay();
	}

	else if (message == WM_PAINT)
	{
		CPaintDC dc(this);
		CRect rcClient;
		GetClientRect(rcClient);
		dc.FillSolidRect(rcClient, RGB(128, 128, 128));
	}

	else if (message == WM_ERASEBKGND)
	{
		return 1;
	}

	else if (message == WM_VSCROLL)
	{
		LRESULT lr = CWnd::WindowProc(message, wParam, lParam);
		// ScrollBarEx send to Tree
		HWND hScrollBar = HWND(lParam);
		if (hScrollBar == pMember->m_wndScrollEx.GetSafeHwnd() && pMember->m_bUseScrollBarEx)
		{
			pMember->m_wndTreeList.SendMessage(WM_VSCROLL, wParam, lParam);
		}

		UpdateScrollingDisplay();
		return lr;
	}

	switch (message)
	{
	case TVM_SORTCHILDRENCB:
	case TVM_SORTCHILDREN:
	case TVM_INSERTITEM:
	case TVM_ENSUREVISIBLE:
	case TVM_DELETEITEM:
	case WM_MOUSEWHEEL:
		if (pMember->m_bUseScrollBarEx)
		{
			LRESULT lr = CWnd::WindowProc(message, wParam, lParam);
			UpdateScrollingDisplay();
			return lr;
		}
		break;
	default:
		break;
	}

	if (message == WM_NOTIFY)
	{
		NMHDR *pnmhdr = (NMHDR *)lParam;

		// when adjust the width of the header's column, fresh the tree list.
		if (pnmhdr->code == HDN_ITEMCHANGED)
		{
			pMember->m_wndTreeList.Invalidate();
		}

		// when click + or - icon or double click an item to expand or collapse the item.
		// the TVM_EXPAND to handle CTreeCtrl::Expand() calling.
		if (pnmhdr->code == TVN_ITEMEXPANDING)
		{
			BOOL bRedrawFlag = (BOOL)pMember->m_wndTreeList.SendMessage(WM_GETREDRAWFLAG);
			if (bRedrawFlag)
			{
				pMember->m_wndTreeList.SetRedraw(FALSE);
				pMember->m_wndTreeList.SendMessage(WM_SETREDRAWFLAG, 0, TRUE);
			}
		}
		if (pnmhdr->code == TVN_ITEMEXPANDED)
		{
			BOOL bRedrawFlag = (BOOL)pMember->m_wndTreeList.SendMessage(WM_GETREDRAWFLAG);
			if (bRedrawFlag)
			{
				pMember->m_wndTreeList.SetRedraw(TRUE);
				pMember->m_wndTreeList.Invalidate();
				pMember->m_wndTreeList.UpdateWindow();
				UpdateScrollingDisplay();
				pMember->m_wndTreeList.SendMessage(WM_SETREDRAWFLAG, 0, TRUE);
			}
		}
		// when delete a parent item, while deleting each child item can send TVN_DELETEITEM notify to tree's parent window,
		// but only one TVM_DELETEITEM message can receive in tree control, so don't use TVM_DELETEITEM to instead of TVN_DELETEITEM notify.
		if (pnmhdr->code == TVN_DELETEITEM)
		{
			NM_TREEVIEW *pNMTreeView = (NM_TREEVIEW *)pnmhdr;

			HTREEITEM hTI = pNMTreeView->itemOld.hItem;
			int nItem = pMember->m_wndTreeList.ListIndexByHTREEITEM(hTI);
			pMember->m_wndTreeList.GetListCtrl()->DeleteItem(nItem);
		}
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

int CTreeListCtrl::UpdateScrollingDisplay()
{
	TREELISTCTRLMEMBER *pMember = (TREELISTCTRLMEMBER *)m_pMember;

	if (!pMember->m_bUseScrollBarEx)
	{
		return -1;
	}

	if (!IsWindow(pMember->m_wndScrollEx.GetSafeHwnd()))
	{
		return -2;
	}

	SCROLLINFO si;
	si.cbSize = sizeof(si);             // size of this structure
	si.fMask = SIF_ALL;                 // parameters to set
	BOOL bRet = pMember->m_wndTreeList.GetScrollInfo(SB_VERT, &si);
	if (!bRet)
	{
		return -3;
	}

	// On XP Style, after create tree, the pTreeList->GetScrollInfo return TRUE,
	// but on classic style, it return FALSE, use si.nPage==0 to differentiate it.
	if (si.nPage == si.nMax + 1 || si.nPage == 0)
	{
		pMember->m_wndScrollEx.ShowWindow(SW_HIDE);
		ArrangeControl(this);
	}
	else
	{
		pMember->m_wndScrollEx.ShowWindow(SW_SHOW);
		ArrangeControl(this);
	}

	pMember->m_wndScrollEx.SetScrollInfo(&si);

	return 0;
}

int CTreeListCtrl::SetScrollBarExBk(
	UINT uMask,
	LPCTSTR lpszImageNameScrollbarExN,
	LPCTSTR lpszImageNameScrollbarExH,
	LPCTSTR lpszImageNameScrollbarExS,
	LPCTSTR lpszImageNameScrollbarExD)
{
	TREELISTCTRLMEMBER *pMember = (TREELISTCTRLMEMBER *)m_pMember;

	ASSERT(pMember->m_bUseScrollBarEx);
	return pMember->m_wndScrollEx.SetBkBitmaps(uMask,
			lpszImageNameScrollbarExN,
			lpszImageNameScrollbarExH,
			lpszImageNameScrollbarExS,
			lpszImageNameScrollbarExD);
}

int CTreeListCtrl::SetScrollBarExThumb(
	UINT uMask,
	LPCTSTR lpszImageNameScrollbarExThumbN,
	LPCTSTR lpszImageNameScrollbarExThumbH,
	LPCTSTR lpszImageNameScrollbarExThumbS,
	LPCTSTR lpszImageNameScrollbarExThumbD)
{
	TREELISTCTRLMEMBER *pMember = (TREELISTCTRLMEMBER *)m_pMember;

	ASSERT(pMember->m_bUseScrollBarEx);
	return pMember->m_wndScrollEx.SetThumbBitmaps(
			uMask,
			lpszImageNameScrollbarExThumbN,
			lpszImageNameScrollbarExThumbH,
			lpszImageNameScrollbarExThumbS,
			lpszImageNameScrollbarExThumbD);
}

int CTreeListCtrl::GetScrollBarExThumb(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameScrollbarExThumbN, CString *pstrImageNameScrollbarExThumbH,
	CString *pstrImageNameScrollbarExThumbS, CString *pstrImageNameScrollbarExThumbD)
{
	TREELISTCTRLMEMBER *pMember = (TREELISTCTRLMEMBER *)m_pMember;

	ASSERT(pMember->m_bUseScrollBarEx);
	return pMember->m_wndScrollEx.GetThumbBitmaps(
			uMask,
			pbCombineImage,
			pstrCombineImageName,
			pstrImageNameScrollbarExThumbN, pstrImageNameScrollbarExThumbH,
			pstrImageNameScrollbarExThumbS, pstrImageNameScrollbarExThumbD);
}

SHORT CTreeListCtrl::SetItemHeight(SHORT cyHeight)
{
	TREELISTCTRLMEMBER *pMember = (TREELISTCTRLMEMBER *)m_pMember;

	return pMember->m_wndTreeList.SetItemHeight(cyHeight);
}

SHORT CTreeListCtrl::GetItemHeight() const
{
	TREELISTCTRLMEMBER *pMember = (TREELISTCTRLMEMBER *)m_pMember;

	return pMember->m_wndTreeList.GetItemHeight();
}

CTreeList *CTreeListCtrl::GetTreeListWnd()
{
	TREELISTCTRLMEMBER *pMember = (TREELISTCTRLMEMBER *)m_pMember;

	return &pMember->m_wndTreeList;
}

// only valid while using custom scroll bar.
CScrollBarEx *CTreeListCtrl::GetScrollBarEx()
{
	TREELISTCTRLMEMBER *pMember = (TREELISTCTRLMEMBER *)m_pMember;

	if (!pMember->m_bUseScrollBarEx)
	{
		return NULL;
	}

	return &pMember->m_wndScrollEx;
}

CSkinHeaderCtrl *CTreeListCtrl::GetHeaderCtrl()
{
	TREELISTCTRLMEMBER *pMember = (TREELISTCTRLMEMBER *)m_pMember;

	return &pMember->m_wndHeader;
}

BOOL CTreeListCtrl::IsUseScrollBarEx()
{
	TREELISTCTRLMEMBER *pMember = (TREELISTCTRLMEMBER *)m_pMember;

	return pMember->m_bUseScrollBarEx;
}

BOOL CTreeListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT *pResult)
{
	// TODO: Add your specialized code here and/or call the base class

	BOOL bRet = CWnd::OnNotify(wParam, lParam, pResult);
	if (bRet)
	{
		return bRet;
	}

	NMHDR *pNMHDR = (NMHDR *)lParam;
	if (pNMHDR->code == HDN_ITEMCLICK)
	{
		NMHEADER *phdr = (NMHEADER *)lParam;
		NMHDR nmhdr;
		nmhdr.hwndFrom = this->m_hWnd;
		nmhdr.idFrom = ::GetDlgCtrlID(this->m_hWnd);
		nmhdr.code = LVN_COLUMNCLICK;
		NMLISTVIEW lv;
		lv.hdr = nmhdr;
		lv.iItem = -1;
		lv.iSubItem = phdr->iItem;
		lv.uNewState = 0;
		lv.uOldState = 0;
		lv.uChanged = 0;
		lv.ptAction.x = 0;
		lv.ptAction.y = 0;
		lv.lParam = 0;
		LRESULT lr = GetParent()->SendMessage(WM_NOTIFY, (WPARAM)(nmhdr.idFrom), (LPARAM)(&lv)); // lr == *pResult;
	}
	return TRUE;
}
