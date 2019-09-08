// SkinHotKeyCtrl.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define F_EXT	    0x01000000L

#define GWU_VIRTKEY  0
#define GWU_MODS     1*sizeof(UINT)
#define GWU_INVALID  2*sizeof(UINT)
#define GWU_DEFAULT  3*sizeof(UINT)
#define GWU_HFONT    4*sizeof(UINT)
#define GWU_YFONT    5*sizeof(UINT)
#define NUM_WND_EXTRA (GWU_YFONT+sizeof(UINT))
#define GetWindowInt GetWindowLong


struct HOTKEYMEMBER
{
	HOTKEYMEMBER()
	{
		for (int i = 0; i < 4; ++i)
		{
			m_himgBk[i] = NULL;
		}
		m_bMouseOver = FALSE;
	}

	int Release()
	{
		for (int i = 0; i < 4; ++i)
		{
			ReleaseIUIImage(m_himgBk[i]);
			m_himgBk[i] = NULL;
		}

		m_bMouseOver = FALSE;
		return 0;
	}

	BOOL m_bMouseOver;
	HIUIIMAGE m_himgBk[4];
};

///////////////////////////////////////////////////////////////////////////
// CSkinHotKeyCtrl

CSkinHotKeyCtrl::CSkinHotKeyCtrl()
{
	m_pBindWnd = this;

	m_pMember = new HOTKEYMEMBER;
	HOTKEYMEMBER *pMember = (HOTKEYMEMBER *)m_pMember;
}

CSkinHotKeyCtrl::~CSkinHotKeyCtrl()
{
	HOTKEYMEMBER *pMember = (HOTKEYMEMBER *)m_pMember;

	pMember->Release();

	if (m_pMember != NULL)
	{
		delete (HOTKEYMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

int CSkinHotKeyCtrl::BindStyle(LPCTSTR lpszStyleID)
{
	HOTKEYMEMBER *pMember = (HOTKEYMEMBER *)m_pMember;

	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (lpszStyleID == NULL)
	{
		return -2;
	}

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_HOTKEY, lpszStyleID);
	return BindStyle(pCtrlProp);

	return 0;
}

int CSkinHotKeyCtrl::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	HOTKEYMEMBER *pMember = (HOTKEYMEMBER *)m_pMember;

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
	if (ect != CT_STYLE_HOTKEY && ect != CT_HOTKEY)
	{
		return -3;
	}

	HOTPROPERTIES *pHotProp = (HOTPROPERTIES *)pCtrlProp;

	if (pHotProp->m_bSpecifyBackgroundImages)
	{
		CString strImageName[1 + COMBINEIMAGESIZE4];
		BOOL bCombineImage = TRUE;
		CTRLPROPERTIES::IUIGetBackground4(pCtrlProp, &bCombineImage, strImageName);
		if (bCombineImage)
		{
			SetBkCombineImage(strImageName[0]);
		}
		else
		{
			SetImages(CONTROL_STATE_UNCHECKED_ALL, strImageName[1], strImageName[2],
				strImageName[3], strImageName[4]);
		}
		for (int i = 0; i < 1 + COMBINEIMAGESIZE4; ++i)
		{
			ReleaseIUIImage(strImageName[i]);
		}
	}

	return 0;
}


BEGIN_MESSAGE_MAP(CSkinHotKeyCtrl, CHotKeyCtrl)
	//{{AFX_MSG_MAP(CSkinHotKeyCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinHotKeyCtrl message handlers

void IUIGetKeyName(UINT vk, LPTSTR lpsz, BOOL fExt)
{
	LONG scan;

	scan = (LONG)MapVirtualKey(vk, 0) << 16;
	if (fExt)
	{
		scan |= F_EXT;
	}

	GetKeyNameText(scan, lpsz, 50);
}

BOOL FAR PASCAL MGetTextExtent(HDC hdc, LPCTSTR lpstr, int cnt, int FAR *pcx, int FAR *pcy)
{
	BOOL fSuccess;
	SIZE size = {0, 0};
	fSuccess = GetTextExtentPoint(hdc, lpstr, cnt, &size);
	if (pcx)
	{
		*pcx = size.cx;
	}
	if (pcy)
	{
		*pcy = size.cy;
	}

	return fSuccess;
}

// Copy void NEAR PASCAL PaintHotKey(register HWND hwnd) from
// \nt4\private\windows\cairoshl\commctrl\hotkey.c
void NEAR PASCAL PaintHotKey(register HWND hwnd)
{
	TCHAR sz[128];
	TCHAR szPlus[10] = {0};
	WORD cch;
	register HDC hdc;
	UINT wMods;
	UINT wVirtKey;
	PAINTSTRUCT ps;
	int x, y;
	HANDLE hFont;

	TSTRCPY(szPlus, ARRAYSIZE(szPlus), _T(" + "));
	//LoadString(HINST_THISDLL, IDS_PLUS, szPlus, ARRAYSIZE(szPlus));

	wVirtKey = GetWindowInt(hwnd, GWU_VIRTKEY);
	wMods = GetWindowInt(hwnd, GWU_MODS);
	if (wVirtKey || wMods)
	{
		sz[0] = 0;
		cch = 0;
		if (wMods & HOTKEYF_CONTROL)
		{
			IUIGetKeyName(VK_CONTROL, sz, FALSE);
			lstrcat(sz, (LPTSTR)szPlus);
		}
		if (wMods & HOTKEYF_SHIFT)
		{
			IUIGetKeyName(VK_SHIFT, sz + lstrlen(sz), FALSE);
			lstrcat(sz, szPlus);
		}
		if (wMods & HOTKEYF_ALT)
		{
			IUIGetKeyName(VK_MENU, sz + lstrlen(sz), FALSE);
			lstrcat(sz, szPlus);
		}

		IUIGetKeyName(wVirtKey, sz + lstrlen(sz), wMods & HOTKEYF_EXT);
	}
	else
	{
		TSTRCPY(sz, 100, _T("None"));
		//			LoadString(NULL, IDS_NONE, sz, 100);
	}

	cch = lstrlen(sz);

	::HideCaret(hwnd);

	::InvalidateRect(hwnd, NULL, TRUE);
	hdc = ::BeginPaint(hwnd, &ps);

	HFONT hf = (HFONT)GetWindowInt(hwnd, GWU_HFONT);
	hFont = SelectObject(hdc, hf);

	x = GetSystemMetrics(SM_CXBORDER);
	y = GetSystemMetrics(SM_CYBORDER);

	::SetBkMode(hdc, TRANSPARENT);
	if (::IsWindowEnabled(hwnd))
	{
		::SetTextColor(hdc, RGB(255, 0, 0));
		TextOut(hdc, x, y, sz, cch);
	}
	else
	{
		GrayString(hdc, NULL, NULL, (DWORD)(LPTSTR)sz, cch, x, y, 0, 0);
	}

	MGetTextExtent(hdc, sz, cch, &x, NULL);
	if (::GetFocus() == hwnd)
	{
		::SetCaretPos(x + GetSystemMetrics(SM_CXBORDER), GetSystemMetrics(SM_CYBORDER));
	}
	::ShowCaret(hwnd);

#if 0
	if (hFont)
	{
		SelectObject(hdc, hFont);
	}
#endif

	::EndPaint(hwnd, &ps);
}

LRESULT CSkinHotKeyCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	HOTKEYMEMBER *pMember = (HOTKEYMEMBER *)m_pMember;

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

	if (message == WM_PAINT)
	{
		//CPaintDC dc(this);
		//CDC *pDC = &dc;
		////CDC *pDC = CDC::FromHandle(HDC(wParam));
		//CRect rcClient;
		//GetClientRect(rcClient);
		//pDC->SetBkMode(TRANSPARENT);

		//CRect rcWin;
		//GetWindowRect(rcWin);
		//GetParent()->ScreenToClient(rcWin);

		//BOOL bSelected = (GetKeyState(VK_LBUTTON) < 0);
		//int nStateIndex = 0;
		//if (IsWindowEnabled())
		//{
		//	if (pMember->m_bMouseOver)
		//	{
		//		if (bSelected)
		//		{
		//			nStateIndex = 2;
		//		}
		//		else
		//		{
		//			nStateIndex = 1;
		//		}
		//	}
		//	else
		//	{
		//		nStateIndex = 0;
		//	}
		//}
		//else
		//{
		//	nStateIndex = 3;
		//}

		//// Draw the parent background to control for alpha blend with background of the control.
		//BOOL bDrawParentPart = FALSE;
		//if (m_bBkCombine)
		//{
		//	if (m_himgCombineBk != NULL)
		//	{
		//		bDrawParentPart = m_himgCombineBk->IsHasAlphaChannel();
		//	}
		//}
		//else
		//{
		//	if (pMember->m_himgBk[nStateIndex] != NULL)
		//	{
		//		bDrawParentPart = pMember->m_himgBk[nStateIndex]->IsHasAlphaChannel();
		//	}
		//}
		//BOOL bUpdateLayeredWindow = (BOOL)GetParent()->SendMessage(WM_ISUPDATELAYEREDWINDOW);
		//if (bUpdateLayeredWindow)
		//{
		//	bDrawParentPart = FALSE;
		//}

		//if (bDrawParentPart)
		//{
		//	BOOL bUpdateLayeredWindow = FALSE;
		//	DrawParentPart(this, pDC, rcWin, &bUpdateLayeredWindow);
		//}

		//if (m_bBkCombine)
		//{
		//	CRect rcClient;
		//	GetClientRect(rcClient);

		//	BOOL bUpdateLayeredWindow = (BOOL)GetParent()->SendMessage(WM_ISUPDATELAYEREDWINDOW);
		//	if (bUpdateLayeredWindow)
		//	{
		//		GetWindowRect(rcClient);
		//		GetParent()->ScreenToClient(rcClient);
		//	}

		//	m_himgCombineBk->SafeLoadSavedImage();

		//	if (m_himgCombineBk->GetSafeHBITMAP() != NULL)
		//	{
		//		int nCXBorder = (rcWin.Width() - rcClient.Width()) / 2;
		//		ASSERT((rcWin.Width() - rcClient.Width()) % 2 == 0);
		//		int nCYBorder = (rcWin.Height() - rcClient.Height()) / 2;
		//		ASSERT((rcWin.Height() - rcClient.Height()) % 2 == 0);

		//		CRect rcSrc;
		//		rcSrc.left = nCXBorder;
		//		rcSrc.top = nCYBorder;
		//		rcSrc.right = m_himgCombineBk->GetWidth() / 4 - nCXBorder;
		//		rcSrc.bottom = m_himgCombineBk->GetHeight() - nCYBorder;

		//		IUIPartNineGridBlt(pDC->GetSafeHdc(), rcClient, m_himgCombineBk, rcSrc);
		//	}
		//}
		//else
		//{
		//	IUINineGridBlt(pDC->GetSafeHdc(),
		//		rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(),
		//		pMember->m_himgBk[nStateIndex]);
		//}

		//pDC->SetBkMode(TRANSPARENT);

		////return TRUE;

		PaintHotKey(m_hWnd);

		return 0;
	}

	if (message == WM_ERASEBKGND)
	{
		return TRUE;
	}

	if (message == WM_NCPAINT)
	{
		CWindowDC dc(this);

		CRect rcWin;
		GetWindowRect(rcWin);
		GetParent()->ScreenToClient(rcWin);

		BOOL bSelected = (GetKeyState(VK_LBUTTON) < 0);
		int nStateIndex = 0;
		if (IsWindowEnabled())
		{
			if (pMember->m_bMouseOver)
			{
				if (bSelected)
				{
					nStateIndex = 2;
				}
				else
				{
					nStateIndex = 1;
				}
			}
			else
			{
				nStateIndex = 0;
			}
		}
		else
		{
			nStateIndex = 3;
		}

		// Draw the parent background to control for alpha blend with background of the control.
		BOOL bDrawParentPart = FALSE;
		if (m_bBkCombine)
		{
			if (m_himgCombineBk != NULL)
			{
				bDrawParentPart = m_himgCombineBk->IsHasAlphaChannel();
			}
		}
		else
		{
			if (pMember->m_himgBk[nStateIndex] != NULL)
			{
				bDrawParentPart = pMember->m_himgBk[nStateIndex]->IsHasAlphaChannel();
			}
		}
		BOOL bUpdateLayeredWindow = (BOOL)GetParent()->SendMessage(WM_ISUPDATELAYEREDWINDOW);
		if (bUpdateLayeredWindow)
		{
			bDrawParentPart = FALSE;
		}

		if (bDrawParentPart)
		{
			BOOL bUpdateLayeredWindow = FALSE;
			DrawParentPart(this, &dc, rcWin, &bUpdateLayeredWindow, NULL);
		}

		rcWin.OffsetRect(-rcWin.TopLeft());

		if (m_bBkCombine)
		{
			DrawCombineBackground(&dc, rcWin, 4, nStateIndex);
		}
		else
		{
			IUINineGridBlt(dc.GetSafeHdc(), rcWin, pMember->m_himgBk[nStateIndex],
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
		}

		return TRUE;
	}

	if (message == WM_MOUSEMOVE)
	{
		if (!pMember->m_bMouseOver)
		{
			pMember->m_bMouseOver = TRUE;
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = m_hWnd;
			tme.dwHoverTime = HOVER_DEFAULT;
			TrackMouseEvent(&tme);

			// Can cause WM_NCPAINT message.
			::RedrawWindow(m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW);
		}
	}

	if (message == WM_MOUSELEAVE)
	{
		pMember->m_bMouseOver = FALSE;

		// Can cause WM_NCPAINT message.
		::RedrawWindow(m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW);
	}

	if (message == WM_KEYDOWN)
	{
		SetRedraw(FALSE);
		CHotKeyCtrl::WindowProc(message, wParam, lParam);
		SetRedraw(TRUE);
		Invalidate();
		UpdateWindow();
	}

	return CHotKeyCtrl::WindowProc(message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
//

int CSkinHotKeyCtrl::SetImages(UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
	LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD)
{
	HOTKEYMEMBER *pMember = (HOTKEYMEMBER *)m_pMember;

	ASSERT(m_hWnd != NULL);

	m_bBkCombine = false;
	return IUISetControlImage4(uMask, pMember->m_himgBk,
			lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);
}

int CSkinHotKeyCtrl::GetImages(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameD)
{
	HOTKEYMEMBER *pMember = (HOTKEYMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bBkCombine;
	return IUIGetControlImage2(uMask, m_himgCombineBk, pMember->m_himgBk, *pbCombineImage,
			pstrCombineImageName, pstrImageNameN, pstrImageNameD);
}
