// SkinButton.cpp : implementation file
//

#include "stdafx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSkinButton

CSkinButton::CSkinButton()
{
	m_pBindWnd = this;
}

CSkinButton::~CSkinButton()
{
}


BEGIN_MESSAGE_MAP(CSkinButton, CButton)
	//{{AFX_MSG_MAP(CSkinButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CSkinButton::IsCreated() const
{
	return (m_hWnd != NULL);
}

int CSkinButton::SetDraw(BOOL bDraw)
{
	ASSERT(::IsWindow(m_hWnd));
	return (int)::SendMessage(m_hWnd, WM_SET_DRAW, (WPARAM)bDraw, 0);
}

BOOL CSkinButton::IsDraw() const
{
	ASSERT(::IsWindow(m_hWnd));
	BOOL bDraw = (BOOL)::SendMessage(m_hWnd, WM_GET_DRAW, 0, 0);

	return bDraw;
}

/////////////////////////////////////////////////////////////////////////////
// CSkinButton message handlers


int CSkinButton::OnDraw(CDC *pMemDC)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	CString strText;
	GetWindowText(strText);

	CRect rcClient;
	GetClientRect(rcClient);

	// load background bitmap, and set font,color and offset
	CRect rcText(rcClient);

	int nCheck = GetCheck();
	BOOL bEnable = IsWindowEnabled();
	BOOL bSelected = GetKeyState(VK_LBUTTON) < 0;
	BOOL bActiveAreaSelected = FALSE;
	if (pMember->m_bUseActiveArea && bSelected)
	{
		CPoint pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		if (pMember->m_rcActive.PtInRect(pt))
		{
			bActiveAreaSelected = TRUE;
		}
	}

	CRect rcWin;
	GetWindowRect(rcWin);
	CWnd *pParent = GetParent();
	pParent->ScreenToClient(rcWin);

	// Prepare state index
	int nStateIndex = 0;
	if (!bEnable)
	{
		// 1.disable
		nStateIndex = 3;
	}
	else
	{
		if (bSelected && pMember->m_bRecordLButtonDown)
		{
			if (pMember->m_bMouseOver)
			{
				// 2.selected(push down)
				nStateIndex = 2;
			}
			else
			{
				// 3.highlight
				nStateIndex = 1;
			}
		}
		else
		{
			if (pMember->m_bMouseOver)
			{
				// 3.highlight
				nStateIndex = 1;
			}
			else
			{
				// 4.normal
				nStateIndex = 0;
			}
		}
	}

	//
	// 1. Draw the parent background to control for alpha blend with background of the control.
	// If the CSkinButton as radio or check box, and not specify the background image, always draw the parent part.
	//

	// CSkinButton default draw the parent part. NOT draw the parent part only background has valid and not has channel.
	BOOL bDrawParentPart = TRUE;
	if (m_bBkCombine)
	{
		// For best performance, Only save the image path while call SetBkCombineImage.
		m_himgCombineBk->SafeLoadSavedImage();
	}

	if (nCheck == BST_CHECKED)
	{
		if (m_bBkCombine)
		{
			if (m_himgCombineBk != NULL && !m_himgCombineBk->IsHasAlphaChannel())
			{
				bDrawParentPart = FALSE;
			}
		}
		else
		{
			if (pMember->m_himgBkC[nStateIndex] != NULL)
			{
				// For best performance, Only save the image path while call SetBkCombineImage.
				pMember->m_himgBkC[nStateIndex]->SafeLoadSavedImage();

				if (pMember->m_himgBkC[nStateIndex] != NULL
					&& !pMember->m_himgBkC[nStateIndex]->IsHasAlphaChannel())
				{
					bDrawParentPart = FALSE;
				}
			}
		}
	}
	else
	{
		if (m_bBkCombine)
		{
			if (m_himgCombineBk != NULL && !m_himgCombineBk->IsHasAlphaChannel())
			{
				bDrawParentPart = FALSE;
			}
		}
		else
		{
			// the background image should be NULL.
			if (pMember->m_himgBk[nStateIndex] != NULL)
			{
				// For best performance, Only save the image path while call SetBkCombineImage.
				pMember->m_himgBk[nStateIndex]->SafeLoadSavedImage();

				if (pMember->m_himgBk[nStateIndex] != NULL
					&& !pMember->m_himgBk[nStateIndex]->IsHasAlphaChannel())
				{
					bDrawParentPart = FALSE;
				}
			}
		}
	}

	if (bDrawParentPart)
	{
		BOOL bUpdateLayeredWindow = FALSE;
		DrawParentPart(this, pMemDC, rcWin, &bUpdateLayeredWindow, NULL);

		// If parent has called UpdateLayeredWindow, use direct-ui mode, adjust the rect
		if (bUpdateLayeredWindow)
		{
			rcClient = rcWin;
		}
	}

	// Give parent a chance to draw after draw parent part.
	BOOL bParentDraw = (BOOL)GetParent()->SendMessage(WM_TRANSPARENTDRAW, WPARAM(pMemDC), LPARAM(this));
	if (bParentDraw)
	{
		return 0;
	}

	//
	// 2. Draw button background, foreground and text
	//
	if (nCheck == BST_CHECKED)
	{
		pMember->m_hbmpCheckedMask[nStateIndex]->SafeLoadSavedImage();
		pMember->m_hBmpForegroundC[nStateIndex]->SafeLoadSavedImage();

		DrawButtonState(rcClient,
			pMemDC,
			nStateIndex,
			pMember->m_hbmpCheckedMask[nStateIndex],
			IRM_STRETCH,
			pMember->m_rcForegroundMargin,
			pMember->m_eForegroundAlignHorC,
			pMember->m_eForegroundAlignVerC,
			pMember->m_crMaskC,
			TRUE);

		// create mask button
		if (pMember->m_hbmpCheckedMask[nStateIndex]->GetSafeHBITMAP() != NULL)
		{
			BackgroundMaskG(this, *pMember->m_hbmpCheckedMask[nStateIndex], RGB(255, 0, 255));
		}

		::DrawIconEx(pMemDC->GetSafeHdc(), 0, 0, pMember->m_hCheckedIcon, 0, 0, 1, NULL, DI_NORMAL);

		// draw check box or radio button's text
		pMember->m_hIUIFontC[nStateIndex]->SafeLoadSavedFont();
		HFONT hFont = GetHFont(pMember->m_hIUIFontC[nStateIndex]);

		CString strDefaultFont;
		if (hFont == NULL)
		{
			strDefaultFont = CUIMgr::GetDefaultFont();
			hFont = CreateIUIFont(strDefaultFont);
			ReleaseIUIFont(strDefaultFont);
		}

		DrawText(pMemDC,
			strText,
			rcClient,
			pMember->m_rcTextMarginC[nStateIndex],
			pMember->m_eTextHorAlignModeC[nStateIndex],
			pMember->m_eTextVerAlignModeC[nStateIndex],
			hFont,
			pMember->m_crC[nStateIndex]);

		if (strDefaultFont.IsEmpty())
		{
			ReleaseIUIFont(strDefaultFont);
		}
	}
	else // Unchecked
	{
		pMember->m_hbmpMask[nStateIndex]->SafeLoadSavedImage();
		pMember->m_hBmpForeground[nStateIndex]->SafeLoadSavedImage();

		DrawButtonState(
			rcClient,
			pMemDC,
			nStateIndex,
			pMember->m_hbmpMask[nStateIndex],
			IRM_STRETCH,
			pMember->m_rcForegroundMargin,
			pMember->m_eForegroundAlignHor,
			pMember->m_eForegroundAlignVer,
			pMember->m_crMask,
			FALSE);

		::DrawIconEx(pMemDC->GetSafeHdc(), 0, 0, pMember->m_hIcon, 0, 0, 1, NULL, DI_NORMAL);

		// draw push button's text
		pMember->m_hIUIFont[nStateIndex]->SafeLoadSavedFont();
		HFONT hFont = GetHFont(pMember->m_hIUIFont[nStateIndex]);

		CString strDefaultFont;
		if (hFont == NULL)
		{
			strDefaultFont = CUIMgr::GetDefaultFont();
			hFont = CreateIUIFont(strDefaultFont);
			ReleaseIUIFont(strDefaultFont);
		}

		DrawText(
			pMemDC,
			strText,
			rcClient,
			pMember->m_rcTextMargin[nStateIndex],
			pMember->m_eTextHorAlignMode[nStateIndex],
			pMember->m_eTextVerAlignMode[nStateIndex],
			hFont,
			pMember->m_cr[nStateIndex]);

		if (strDefaultFont.IsEmpty())
		{
			ReleaseIUIFont(strDefaultFont);
		}
	}

	// draw focus rect
	if (pMember->m_bTabStop && (GetFocus() == this))
	{
		pMemDC->DrawFocusRect(rcClient);
	}

	return 0;
}

BOOL CSkinButton::PreTranslateMessage(MSG *pMsg)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class
	if (::IsWindow(pMember->m_wndToolTip.GetSafeHwnd()))
	{
		pMember->m_wndToolTip.RelayEvent(pMsg);
	}

	return CButton::PreTranslateMessage(pMsg);
}

LRESULT CSkinButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	// When use layered window, the window not redraw when its child controls hide or display
	// to solve it, handle the WM_WINDOWPOSCHANGED message.
	CWnd *pParent = GetParent();
	BOOL bLayered = IsLayeredWindow(pParent->GetSafeHwnd());
	if (message == WM_WINDOWPOSCHANGED && bLayered)
	{
		WINDOWPOS *pos = (WINDOWPOS *)lParam;
		if (pos->hwnd == m_hWnd)
		{
			// When show the control, notify itself to redraw
			if ((pos->flags & SWP_SHOWWINDOW) == SWP_SHOWWINDOW)
			{
				Redraw(this);
			}
			// When hide the control, notify its parent to redraw
			else if ((pos->flags & SWP_HIDEWINDOW) == SWP_HIDEWINDOW)
			{
				CRect rcThis;
				GetWindowRect(rcThis);
				pParent->ScreenToClient(rcThis);
				pParent->InvalidateRect(rcThis);
			}
		}
	}

	if (message == WM_CREATE)
	{
		BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

		LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		pMember->m_bMultiLine = ((lStyle & BS_MULTILINE) == BS_MULTILINE);

		if ((lStyle & BS_AUTOCHECKBOX) == BS_AUTOCHECKBOX)	// Check Box
		{
			pMember->m_eButtonControlType = CT_CHECK;
		}
		else if ((lStyle & BS_AUTORADIOBUTTON) == BS_AUTORADIOBUTTON)	// Radio button
		{
			pMember->m_eButtonControlType = CT_RADIO;
		}
		else	// Push button
		{
			pMember->m_eButtonControlType = CT_PUSHBUTTON;
		}
	}

	if (message == BM_SETCHECK || message == BM_SETSTATE || message == WM_SETTEXT || message == WM_ENABLE)
	{
		LRESULT lr = 0;
		if (IsWindowVisible())
		{
			SetRedraw(FALSE);
			lr = CButton::WindowProc(message, wParam, lParam);
			SetRedraw(TRUE);
			Redraw(this);
			if (!IsUpdateLayeredWindow())
			{
				UpdateWindow();
			}
		}
		else
		{
			lr = CButton::WindowProc(message, wParam, lParam);
		}

		return lr;
	}

	// If using XP theme style, while switch focus by press tab from one Edit to another Edit, the font of the button will not redraw.
#ifndef WM_UPDATEUISTATE
#define WM_UPDATEUISTATE                0x0128
#endif
	if (message == WM_UPDATEUISTATE)
	{
		Redraw(this);
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
#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
	case WM_MOUSEWHEEL:
	case WM_MOUSELEAVE:
#endif /* if (_WIN32_WINNT < 0x0400) */
	case WM_MOUSEHOVER:
	{
		int nID = GetDlgCtrlID();
		WPARAM wParam2 = MAKEWPARAM(message, nID);
		BOOL bRet = (BOOL)pParent->SendMessage(WM_PARENTEVENT, wParam2, lParam);
		if (bRet)
		{
			return 0;
		}
	}
	break;
	}

	if (message == WM_LBUTTONDOWN)
	{
		pMember->m_bRecordLButtonDown = TRUE;
		if (!IsUpdateLayeredWindow())
		{
			UpdateWindow();
		}

		// when the button as a menu bar, click the button more then once,
		// can not accept WM_MOUSELEAVE message after the pop-up menu has been destroy
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = HOVER_DEFAULT;
		::TrackMouseEvent(&tme);
		Redraw(this);
	}

	if (message == WM_LBUTTONUP)
	{
		pMember->m_bRecordLButtonDown = FALSE;
	}

	if (message == WM_MOUSEMOVE)
	{
		if (pMember->m_bRecordLButtonDown)
		{
			CRect rcClient;
			GetClientRect(rcClient);
			CPoint point((DWORD)lParam);
			if (!rcClient.PtInRect(point))
			{
				if (pMember->m_bMouseOver)
				{
					pMember->m_bMouseOver = FALSE;
					if (!IsUpdateLayeredWindow())
					{
						UpdateWindow();
					}
				}
			}
			else
			{
				if (!pMember->m_bMouseOver)
				{
					pMember->m_bMouseOver = TRUE;
					if (!IsUpdateLayeredWindow())
					{
						UpdateWindow();
					}
				}
			}

			if (pMember->m_bUseActiveArea)
			{
				CRect rcActiveArea;
				GetActiveArea(rcActiveArea);
				if (!rcActiveArea.PtInRect(point))
				{
					if (pMember->m_bActiveAreaMouseOver)
					{
						pMember->m_bActiveAreaMouseOver = FALSE;
						if (!IsUpdateLayeredWindow())
						{
							UpdateWindow();
						}
					}
				}
				else
				{
					if (!pMember->m_bActiveAreaMouseOver)
					{
						pMember->m_bActiveAreaMouseOver = TRUE;
						if (!IsUpdateLayeredWindow())
						{
							UpdateWindow();
						}
					}
				}
			}
		}
		else
		{
			if (!pMember->m_bMouseOver)
			{
				pMember->m_bMouseOver = TRUE;

				// when set RGN for highlight stats, show the tool tip when mouse over the button(the tool tip and highlight stats at the same time work).
				// don't call Invalidate() after this "if" section
				Redraw(this);

				if (!IsUpdateLayeredWindow())
				{
					UpdateWindow();
				}
			}

			if (pMember->m_bUseActiveArea)
			{
				CPoint point((DWORD)lParam);
				BOOL bOldMouseOver = pMember->m_bActiveAreaMouseOver;
				if (pMember->m_rcActive.PtInRect(point))
				{
					pMember->m_bActiveAreaMouseOver = TRUE;
				}
				else
				{
					pMember->m_bActiveAreaMouseOver = FALSE;
				}
				if (bOldMouseOver != pMember->m_bActiveAreaMouseOver)
				{
					Redraw(this);
					if (!IsUpdateLayeredWindow())
					{
						UpdateWindow();
					}
				}
			}

			// when the button as a menu bar, can not accept WM_MOUSELEAVE message after the pop-up menu has been destroy
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE | TME_HOVER;
			tme.hwndTrack = m_hWnd;
			tme.dwHoverTime = pMember->m_dwHoverTime;
			::TrackMouseEvent(&tme);

			// move this function call to "if" section for showing tool tip when the highlight mask bitmap is set.
			// Redraw(this);
		}
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
		pMember->m_bMouseOver = FALSE;
		pMember->m_bActiveAreaMouseOver = FALSE;
		Redraw(this);
	}

	if (message == WM_MOUSEACTIVATE)
	{
		Redraw(this);
	}

	if (message == WM_GETDLGCODE)
	{
		Redraw(this);
	}

	if (message == WM_KILLFOCUS)
	{
		BOOL bVisible = IsWindowVisible();
		if (bVisible)
		{
			SetRedraw(FALSE);
		}
		LRESULT lr = CButton::WindowProc(message, wParam, lParam);
		if (bVisible)
		{
			SetRedraw(TRUE);
			Redraw(this);
			if (!IsUpdateLayeredWindow())
			{
				UpdateWindow();
			}
		}

		pMember->m_bRecordLButtonDown = FALSE;
		return lr;
	}

	if (message == WM_SETFOCUS)
	{
		BOOL bVisible = IsWindowVisible();
		if (bVisible)
		{
			SetRedraw(FALSE);
		}
		LRESULT lr = CButton::WindowProc(message, wParam, lParam);
		if (bVisible)
		{
			SetRedraw(TRUE);
			Redraw(this);
			if (!IsUpdateLayeredWindow())
			{
				UpdateWindow();
			}
		}
		return lr;
	}

	if (message == WM_DRAWCONTROL)
	{
		CDC *pDC = (CDC *)wParam;
		OnDraw(pDC);
	}

	if (message == WM_GETCONTROLTYPE)
	{
		CONTROL_TYPE *pect = (CONTROL_TYPE *)lParam;

		if (pect == NULL)
		{
			return -1;
		}

		*pect = GetButtonControlType();
	}

	if (message == WM_PAINT)
	{
		CPaintDC dc(this); // device context for painting

		// TODO: Add your message handler code here
		if (IsDraw())
		{
			// Callback a new clip box
			NMHDR nmhdr;
			nmhdr.hwndFrom = m_hWnd;
			nmhdr.idFrom = GetDlgCtrlID();
			nmhdr.code = NM_EXCLUDE_CLIP_RECT;

			NMRECT nmRect;
			nmRect.hdr = nmhdr;
			nmRect.rect = CRect(0, 0, 0, 0);
			nmRect.lParam = 0;

			LRESULT lr = GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmRect));
			if (lr != 0)
			{
				dc.ExcludeClipRect(&nmRect.rect);
			}

			CRect rcClient;
			GetClientRect(rcClient);
			CDC memCopy;
			memCopy.CreateCompatibleDC(&dc);
			CBitmap bmpCopy;
			bmpCopy.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
			ASSERT(bmpCopy.GetSafeHandle() != NULL);
			CBitmap *pbmpCopyOld = memCopy.SelectObject(&bmpCopy);

			DRAWITEMSTRUCT dis;
			dis.CtlType = ODT_BUTTON;
			dis.CtlID = GetDlgCtrlID();
			dis.itemID = 0;
			dis.hwndItem = m_hWnd;
			dis.hDC = memCopy.GetSafeHdc();
			dis.itemData = 0;
			dis.rcItem = rcClient;

			BOOL bParentDraw = (BOOL)pParent->SendMessage(WM_DRAWITEMEX, dis.CtlID, LPARAM(&dis));
			if (!bParentDraw)
			{
				OnDraw(&memCopy);
			}

			DrawWLChildren(&memCopy);

			HRGN hRgn = CreateRectRgn(0, 0, 0, 0);
			int nRet = GetWindowRgn(hRgn);
			if (ERROR != nRet)
			{
				::SelectClipRgn(dc.m_hDC, hRgn);
			}
			dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memCopy, 0, 0, SRCCOPY);
			::DeleteObject(hRgn);
			memCopy.SelectObject(pbmpCopyOld);
			if (ERROR != nRet)
			{
				::SelectClipRgn(dc.m_hDC, NULL);
			}
		}
	}

	if (message == WM_TIMER)
	{
		CRect rcWin;
		GetWindowRect(rcWin);
		if (pMember->m_eAnimateDirection == LEFT)
		{
			pMember->m_nLeftAlign -= pMember->m_nAnimatePixel;
			if (pMember->m_bAnimateReset)
			{
				pMember->m_nLeftAlign = rcWin.Width() - 1;
				pMember->m_bAnimateReset = FALSE;
			}
		}
		else if (pMember->m_eAnimateDirection == TOP)
		{
			pMember->m_nTopAlign -= pMember->m_nAnimatePixel;
			if (pMember->m_bAnimateReset)
			{
				pMember->m_nTopAlign = rcWin.Height() - 1;
				pMember->m_bAnimateReset = FALSE;
			}
		}
		else if (pMember->m_eAnimateDirection == RIGHT)
		{
			pMember->m_nLeftAlign += pMember->m_nAnimatePixel;
			if (pMember->m_bAnimateReset)
			{
				pMember->m_nLeftAlign = -pMember->m_sizeText.cx + 1;
				pMember->m_bAnimateReset = FALSE;
			}
		}
		else if (pMember->m_eAnimateDirection == BOTTOM)
		{
			pMember->m_nTopAlign += pMember->m_nAnimatePixel;
			if (pMember->m_bAnimateReset)
			{
				pMember->m_nTopAlign = -pMember->m_sizeText.cy + 1;
				pMember->m_bAnimateReset = FALSE;
			}
		}

		Redraw(this);
	}

	if (message == WLNM_CREATE)
	{
		m_vpDynamicCreatedWLControl.push_back((CWLWnd *)lParam);
	}

	// message send from windowless route to button's parent
	if (message >= WM_WINDOWLESSFIRST && message <= WM_WINDOWLESSLAST)
	{
		RouteWLMessage(m_hWnd, message, wParam, lParam);

		NMHDR nmhdr;
		nmhdr.hwndFrom = m_hWnd;
		nmhdr.idFrom = GetDlgCtrlID();
		nmhdr.code = NM_WLCHILD;

		NMWLCHILD nmwlChild;
		nmwlChild.hdr = nmhdr;
		nmwlChild.uMsg = message;
		nmwlChild.wParam = wParam;
		nmwlChild.lParam = lParam;

		LRESULT lr = GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmwlChild));
		if (lr != 0)
		{
			return lr;
		}
	}

	if (message == WM_SETBKIMAGEN)
	{
		HBITMAP hBmp = (HBITMAP)wParam;
		BOOL bRedraw = (BOOL)lParam;

		ReleaseIUIImage(pMember->m_himgBk[0]);

		GUID guid;
		CoCreateGuid(&guid);
		wchar_t wszName[64] = {0};
		StringFromGUID2(guid, wszName, sizeof(wszName));

		AddCustomImage((LPCTSTR)_bstr_t(wszName), hBmp);
		pMember->m_himgBk[0] = CreateIUIImage((LPCTSTR)_bstr_t(wszName));
		ReleaseIUIImage(pMember->m_himgBk[0]);

		if (bRedraw)
		{
			Invalidate();
		}
	}
	if (message == WM_SETBKIMAGEH)
	{
		HBITMAP hBmp = (HBITMAP)wParam;
		BOOL bRedraw = (BOOL)lParam;

		ReleaseIUIImage(pMember->m_himgBk[1]);

		GUID guid;
		CoCreateGuid(&guid);
		wchar_t wszName[64] = {0};
		StringFromGUID2(guid, wszName, sizeof(wszName));

		AddCustomImage((LPCTSTR)_bstr_t(wszName), hBmp);
		pMember->m_himgBk[1] = CreateIUIImage((LPCTSTR)_bstr_t(wszName));
		ReleaseIUIImage(pMember->m_himgBk[1]);

		if (bRedraw)
		{
			Invalidate();
		}
	}
	if (message == WM_SETBKIMAGES)
	{
		HBITMAP hBmp = (HBITMAP)wParam;
		BOOL bRedraw = (BOOL)lParam;

		ReleaseIUIImage(pMember->m_himgBk[2]);

		GUID guid;
		CoCreateGuid(&guid);
		wchar_t wszName[64] = {0};
		StringFromGUID2(guid, wszName, sizeof(wszName));

		AddCustomImage((LPCTSTR)_bstr_t(wszName), hBmp);
		pMember->m_himgBk[2] = CreateIUIImage((LPCTSTR)_bstr_t(wszName));
		ReleaseIUIImage(pMember->m_himgBk[2]);

		if (bRedraw)
		{
			Invalidate();
		}
	}
	if (message == WM_SETBKIMAGED)
	{
		HBITMAP hBmp = (HBITMAP)wParam;
		BOOL bRedraw = (BOOL)lParam;

		ReleaseIUIImage(pMember->m_himgBk[3]);

		GUID guid;
		CoCreateGuid(&guid);
		wchar_t wszName[64] = {0};
		StringFromGUID2(guid, wszName, sizeof(wszName));

		AddCustomImage((LPCTSTR)_bstr_t(wszName), hBmp);
		pMember->m_himgBk[3] = CreateIUIImage((LPCTSTR)_bstr_t(wszName));
		ReleaseIUIImage(pMember->m_himgBk[3]);

		if (bRedraw)
		{
			Invalidate();
		}
	}
	if (message == WM_SETBKIMAGECN)
	{
		HBITMAP hBmp = (HBITMAP)wParam;
		BOOL bRedraw = (BOOL)lParam;

		pMember->m_himgBkC[0]->Destroy();
		pMember->m_himgBkC[0]->Attach(hBmp);

		if (bRedraw)
		{
			Invalidate();
		}
	}
	if (message == WM_SETBKIMAGECH)
	{
		HBITMAP hBmp = (HBITMAP)wParam;
		BOOL bRedraw = (BOOL)lParam;

		pMember->m_himgBkC[1]->Destroy();
		pMember->m_himgBkC[1]->Attach(hBmp);

		if (bRedraw)
		{
			Invalidate();
		}
	}
	if (message == WM_SETBKIMAGECS)
	{
		HBITMAP hBmp = (HBITMAP)wParam;
		BOOL bRedraw = (BOOL)lParam;

		pMember->m_himgBkC[2]->Destroy();
		pMember->m_himgBkC[2]->Attach(hBmp);

		if (bRedraw)
		{
			Invalidate();
		}
	}
	if (message == WM_SETBKIMAGECD)
	{
		HBITMAP hBmp = (HBITMAP)wParam;
		BOOL bRedraw = (BOOL)lParam;

		pMember->m_himgBkC[3]->Destroy();
		pMember->m_himgBkC[3]->Attach(hBmp);

		if (bRedraw)
		{
			Invalidate();
		}
	}
	else if (message == WM_SETBKIMGTRANSPARENT)
	{
		OnSetBkImgTransparent((BYTE)wParam, (BOOL)lParam);
	}
	if (message == WM_SET_DRAW)
	{
		BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;
		pMember->m_bDraw = (BOOL)wParam;
	}
	else if (message == WM_GET_DRAW)
	{
		BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;
		return (LRESULT)pMember->m_bDraw;
	}

	return CButton::WindowProc(message, wParam, lParam);
}
