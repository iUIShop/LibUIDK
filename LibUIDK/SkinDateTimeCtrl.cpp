// SkinDateTimeCtrl.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



struct DATETIMEMEMBER
{
	DATETIMEMEMBER()
	{
		for (int i = 0; i < 4; ++i)
		{
			m_hIUIFont[i] = NULL;
			m_himgBk[i] = NULL;
		}

		m_clrText = RGB(0, 0, 0);   // text

		m_ptImageResize.x = m_ptImageResize.y = 2;

		m_rcMargin.SetRect(0, 0, 0, 0);

		m_bNeedCallNCPaint = FALSE;
		m_bMouseOver = FALSE;
		m_bMarginAreaOver = FALSE;
		m_bNCMouseOver = FALSE;
	}

	int Release()
	{
		int i = 0;
		for (i = 0; i < 4; ++i)
		{
			ReleaseIUIFontInternal(m_hIUIFont[i]);
			m_hIUIFont[i] = NULL;
		}
		m_clrText = RGB(0, 0, 0);   // text

		for (i = 0; i < 4; ++i)
		{
			ReleaseIUIImage(m_himgBk[i]);
			m_himgBk[i] = NULL;
		}
		m_ptImageResize.x = m_ptImageResize.y = 2;

		m_rcMargin.SetRect(0, 0, 0, 0);
		m_bNeedCallNCPaint = FALSE;
		m_bMouseOver = FALSE;
		m_bMarginAreaOver = FALSE;
		m_bNCMouseOver = FALSE;

		return 0;
	}

	HIUIFONT m_hIUIFont[4];
	COLORREF m_clrText;		// text

	HIUIIMAGE m_himgBk[4]; // The Highlight state is for Combo Box, the Selected state is not used.
	CPoint m_ptImageResize;

	CRect m_rcMargin;

	BOOL m_bNeedCallNCPaint;
	BOOL m_bMouseOver;		// Client area highlight
	BOOL m_bMarginAreaOver;	// Margin area (use WM_NCCALCSIZE message) highlight
	BOOL m_bNCMouseOver;	// Non-client area (2 pixel border) area highlight.
};

/////////////////////////////////////////////////////////////////////////////
// CSkinDateTimeCtrl

CSkinDateTimeCtrl::CSkinDateTimeCtrl()
{
	m_pBindWnd = this;

	m_pMember = new DATETIMEMEMBER;
}

CSkinDateTimeCtrl::~CSkinDateTimeCtrl()
{
	DATETIMEMEMBER *pMember = (DATETIMEMEMBER *)m_pMember;

	if (m_ctrlSpinButton.GetSafeHwnd() != NULL)
	{
		m_ctrlSpinButton.UnsubclassWindow();
	}

	if (m_pMember != NULL)
	{
		delete (DATETIMEMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

int CSkinDateTimeCtrl::BindStyle(LPCTSTR lpszStyleID)
{
	DATETIMEMEMBER *pMember = (DATETIMEMEMBER *)m_pMember;

	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (lpszStyleID == NULL)
	{
		return -2;
	}

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_DATETIMECTRL, lpszStyleID);
	return BindStyle(pCtrlProp);

	return 0;
}

int CSkinDateTimeCtrl::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	DATETIMEMEMBER *pMember = (DATETIMEMEMBER *)m_pMember;

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
	if (ect != CT_STYLE_DATETIMECTRL && ect != CT_DATETIMECTRL)
	{
		return -3;
	}

	DATETIMECTRLPROPERTIES *pDateTimeProp = (DATETIMECTRLPROPERTIES *)pCtrlProp;

	// images
	if (pDateTimeProp->m_bSpecifyBackgroundImages)
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
			SetBkImages(CONTROL_STATE_UNCHECKED_ALL,
				strImageName[1], strImageName[2], strImageName[3], strImageName[4]);
		}
	}
	SetBkImageResizeMode(pCtrlProp->m_eBkImageResizeMode);
	SetBkImageResizePoint(pCtrlProp->m_ptImageResize);
	SetBkImageRepeatX(pCtrlProp->m_uXRepeatPixel);
	SetBkImageRepeatY(pCtrlProp->m_uYRepeatPixel);

	//
	SetButtonWidth((LONG)pDateTimeProp->m_lButtonWidth);

	// Set font
	CString strResFontID[4];
	CTRLPROPERTIES::IUIGetControlFont4(pCtrlProp, strResFontID);
	SetTextFont(CONTROL_STATE_UNCHECKED_ALL, strResFontID[0], strResFontID[1], strResFontID[2], strResFontID[3]);

	//
	POINT ptTextOffset = pDateTimeProp->m_ptTextOffset;
	SetTextOffset(&ptTextOffset);

	// Set Spin button
	if (m_ctrlSpinButton.GetSafeHwnd() != NULL)
	{
		m_ctrlSpinButton.BindStyle((LPCTSTR)pDateTimeProp->m_strSpinButtonStyleID);
	}

	return 0;
}

int CSkinDateTimeCtrl::ReleaseObject()
{
	DATETIMEMEMBER *pMember = (DATETIMEMEMBER *)m_pMember;

	pMember->Release();

	return CControlBase::ReleaseObject();
}

BEGIN_MESSAGE_MAP(CSkinDateTimeCtrl, CDateTimeCtrl)
	//{{AFX_MSG_MAP(CSkinDateTimeCtrl)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinDateTimeCtrl message handlers

LRESULT CSkinDateTimeCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	DATETIMEMEMBER *pMember = (DATETIMEMEMBER *)m_pMember;

	ASSERT(m_hWnd != NULL);
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
		LRESULT lr = CDateTimeCtrl::WindowProc(message, wParam, lParam);
		if (lr == -1)
		{
			return -1;
		}

		CWnd *pSpinButton = GetWindow(GW_CHILD);
		if (pSpinButton->GetSafeHwnd() != NULL)
		{
			m_ctrlSpinButton.SubclassWindow(pSpinButton->m_hWnd);

			// Move the spin button
			LayoutSpinButton();
		}

		return lr;
	}
	else if (message == WM_NCCALCSIZE)
	{
		// Set the margin of text in control;
		// Set the width of button of the control.
		LPRECT lpRect = NULL;

		BOOL bCalcValidRects = (BOOL)wParam;
		if (bCalcValidRects)
		{
			NCCALCSIZE_PARAMS *pncp = (NCCALCSIZE_PARAMS *)lParam;
			lpRect = &(pncp->rgrc[0]);
		}
		else
		{
			lpRect = LPRECT(lParam);
		}

		lpRect->left += pMember->m_rcMargin.left;
		lpRect->top += pMember->m_rcMargin.top;
		lpRect->right -= pMember->m_rcMargin.right;
		lpRect->bottom -= pMember->m_rcMargin.bottom;

		return 0;		// return 0 to refuse to generate border.
	}
	else if (message == WM_ERASEBKGND)
	{
		return TRUE;
	}

	else if (message == WM_NCLBUTTONDOWN)
	{
		CPoint point(lParam);	// Screen coordinate

		CRect rcButton;
		GetWindowRect(rcButton);
		rcButton.left = rcButton.right - pMember->m_rcMargin.right - GetSystemMetrics(SM_CXVSCROLL);

		// Simulate pressing the drop-down button to show the month control
		if (rcButton.PtInRect(point))
		{
			SendMessage(WM_KEYDOWN, VK_F4);
		}
	}
	else if (message == WM_LBUTTONDOWN)
	{
		CPoint point(lParam);

		// The API 'DrawFrameControl' while be called after
		// receive WM_LBUTTONDOWN message. So refuse it.
		SetRedraw(FALSE);
		LRESULT lr = CDateTimeCtrl::WindowProc(message, wParam, lParam);
		SetRedraw(TRUE);
		Invalidate();

		// If call SetRedraw(FALSE) while handling WM_LBUTTONDOWN
		// message, the Month drop-down control while can't be shown,
		// So send F4 (or Alt+Down) key down message to pop-up the month control.
		CRect rcButton;
		GetWindowRect(rcButton);
		rcButton.OffsetRect(-rcButton.left, -rcButton.top);
		rcButton.DeflateRect(pMember->m_rcMargin);
		rcButton.OffsetRect(-rcButton.left, -rcButton.top);
		rcButton.left = rcButton.right - GetSystemMetrics(SM_CXVSCROLL);

		// Simulate pressing the drop-down button
		if (rcButton.PtInRect(point))
		{
			SendMessage(WM_KEYDOWN, VK_F4);
		}

		return lr;
	}
	else if (message == WM_LBUTTONUP)
	{
		InvalidateRect(NULL);
		if (pMember->m_rcMargin != CRect(0, 0, 0, 0))
		{
			// Can cause WM_NCPAINT message.
			::RedrawWindow(m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW);
		}
	}

	// Instead of WM_NCLBUTTONDOWN and WM_NCMOUSEMOVE.
	// the CDateTimeCtrl can't receive WM_NCLBUTTONDOWN and WM_NCMOUSEMOVE message.
	else if (message == WM_SETCURSOR)
	{
		WORD wTestCode = LOWORD(lParam);
		WORD wMouseMsg = HIWORD(lParam);

		if (wTestCode == HTNOWHERE)
		{
			if (wMouseMsg == WM_LBUTTONDOWN)	// Instead of WM_NCLBUTTONDOWN
			{
				CMonthCalCtrl *pMonthCtrl = GetMonthCalCtrl();
				if (pMonthCtrl->GetSafeHwnd() != NULL)
				{

					// Close the Month control
				}
				else
				{
					CPoint point;
					GetCursorPos(&point);

					CRect rcButton;
					GetWindowRect(rcButton);

					point.x = point.x - rcButton.left;
					point.y = point.y - rcButton.top;

					rcButton.OffsetRect(-rcButton.left, -rcButton.top);
					rcButton.left = rcButton.right - pMember->m_rcMargin.right
						- GetSystemMetrics(SM_CXVSCROLL);

					// Simulate pressing the drop-down button to show the month control
					if (rcButton.PtInRect(point))
					{
						SendMessage(WM_KEYDOWN, VK_F4);
					}
				}
			}
			else if (wMouseMsg == WM_MOUSEMOVE)	// Instead of WM_NCMOUSEMOVE
			{
				if (!pMember->m_bMarginAreaOver)
				{
					pMember->m_bMarginAreaOver = TRUE;

					TRACKMOUSEEVENT tme;
					tme.cbSize = sizeof(TRACKMOUSEEVENT);
					tme.dwFlags = TME_LEAVE | TME_NONCLIENT;
					tme.hwndTrack = m_hWnd;
					tme.dwHoverTime = HOVER_DEFAULT;
					TrackMouseEvent(&tme);

					Invalidate();
					if (pMember->m_rcMargin != CRect(0, 0, 0, 0))
					{
						// Can cause WM_NCPAINT message.
						::RedrawWindow(m_hWnd, NULL, NULL,
							RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW);
					}
					else
					{
						ASSERT(FALSE);
					}
				}
			}
		}
	}
	else if (message == WM_NCMOUSEMOVE)
	{
		if (!pMember->m_bNCMouseOver)
		{
			pMember->m_bNCMouseOver = TRUE;
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE | TME_NONCLIENT;
			tme.hwndTrack = m_hWnd;
			tme.dwHoverTime = HOVER_DEFAULT;
			TrackMouseEvent(&tme);

			InvalidateRect(NULL);
			if (pMember->m_rcMargin != CRect(0, 0, 0, 0))
			{
				// Can cause WM_NCPAINT message.
				::RedrawWindow(m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW);
			}
		}
	}
	else if (message == WM_NCMOUSELEAVE)
	{
		pMember->m_bNCMouseOver = FALSE;
		pMember->m_bMarginAreaOver = FALSE;
		InvalidateRect(NULL);
		if (pMember->m_rcMargin != CRect(0, 0, 0, 0))
		{
			// Can cause WM_NCPAINT message.
			::RedrawWindow(m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW);
		}
	}
	else if (message == WM_MOUSEMOVE)
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

			InvalidateRect(NULL);
			if (pMember->m_rcMargin != CRect(0, 0, 0, 0))
			{
				// Can cause WM_NCPAINT message.
				::RedrawWindow(m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW);
			}
		}
	}
	else if (message == WM_MOUSELEAVE)
	{
		pMember->m_bMouseOver = FALSE;
		InvalidateRect(NULL);
		if (pMember->m_rcMargin != CRect(0, 0, 0, 0))
		{
			// Can cause WM_NCPAINT message.
			::RedrawWindow(m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW);
		}
	}
	// draw the edit control and drop-down button.
	if (message == WM_PAINT)
	{
		CPaintDC dc(this); // device context for painting

		// TODO: Add your message handler code here
		int nStatusIndex = 0;
		BOOL bEnabled = IsWindowEnabled();
		BOOL bSelected = (GetKeyState(VK_LBUTTON) < 0);
		if (bEnabled)
		{
			if (pMember->m_bMouseOver
				|| pMember->m_bNCMouseOver
				|| pMember->m_bMarginAreaOver)
			{
				if (bSelected)
				{
					nStatusIndex = 2;
				}
				else
				{
					nStatusIndex = 1;
				}
			}
			else
			{
				nStatusIndex = 0;
			}
		}
		else
		{
			nStatusIndex = 3;
		}

		::SendMessage(m_hWnd, WM_SETFONT,
			(WPARAM)GetHFont(pMember->m_hIUIFont[nStatusIndex]), FALSE);

		CRect rcWnd;
		GetWindowRect(rcWnd);

		// Client coordinate relate to parent.
		CRect rcClient2Parent(rcWnd);
		GetParent()->ScreenToClient(rcClient2Parent);

		// Get the original size of client area.
		CRect rcClient;
		GetClientRect(rcClient);
		rcClient.right += pMember->m_rcMargin.left;
		rcClient.right += pMember->m_rcMargin.right;
		rcClient.bottom += pMember->m_rcMargin.top;
		rcClient.bottom += pMember->m_rcMargin.bottom;

		// Calculate the width of the border.
		int nBorderCX = (rcClient2Parent.Width() - rcClient.Width()) / 2;
		int nBorderCY = (rcClient2Parent.Height() - rcClient.Height()) / 2;

		rcClient2Parent.DeflateRect(nBorderCX, nBorderCY);
		rcClient2Parent.DeflateRect(pMember->m_rcMargin);

		CDC memDCBK;
		memDCBK.CreateCompatibleDC(&dc); // the size of memDC is same as it's parent
		CBitmap bmpBK;
		bmpBK.CreateCompatibleBitmap(&dc, rcClient2Parent.Width(), rcClient2Parent.Height());
		ASSERT(bmpBK.GetSafeHandle() != NULL);
		CBitmap *pBmpBKOld = memDCBK.SelectObject(&bmpBK);

		//
		// 1. Draw parent part.
		//
		BOOL bDrawParentPart = TRUE;
		if (m_bBkCombine)
		{
			m_himgCombineBk->SafeLoadSavedImage();

			if (!m_himgCombineBk->IsHasAlphaChannel())
			{
				bDrawParentPart = FALSE;
			}
		}
		else
		{
			pMember->m_himgBk[nStatusIndex]->SafeLoadSavedImage();

			if (!pMember->m_himgBk[nStatusIndex]->IsHasAlphaChannel())
			{
				bDrawParentPart = FALSE;
			}
		}

		BOOL bUpdateLayeredWindow = FALSE;
		if (!IsUpdateLayeredWindow())
		{
			if (bDrawParentPart)
			{
				DrawParentPart(this, &memDCBK, rcClient2Parent, &bUpdateLayeredWindow, NULL);
			}
		}
		else
		{
			bUpdateLayeredWindow = TRUE;
		}

		//
		// 2. Draw background image
		//

		HIUIIMAGE hSubBmp = NULL;

		if (m_bBkCombine)
		{
			if (m_himgCombineBk != NULL)
			{
				m_himgCombineBk->SafeLoadSavedImage();

				if (m_himgCombineBk->GetSafeHBITMAP() != NULL)
				{
					CRect rcDest(0, 0, rcClient2Parent.Width(), rcClient2Parent.Height());

					int nLeftBorderWidth = nBorderCX + pMember->m_rcMargin.left;
					int nRigthBorderWidth = nBorderCX + pMember->m_rcMargin.right;
					int nTopBorderHeight = nBorderCY + pMember->m_rcMargin.top;
					int nBottomBorderHeight = nBorderCY + pMember->m_rcMargin.bottom;
					CRect rcSrc;
					rcSrc.left = m_himgCombineBk->GetWidth() * nStatusIndex / 4
						+ nLeftBorderWidth;
					rcSrc.right = m_himgCombineBk->GetWidth() * (nStatusIndex + 1) / 4
						- nRigthBorderWidth;
					rcSrc.top = nTopBorderHeight;
					rcSrc.bottom = m_himgCombineBk->GetHeight() - nBottomBorderHeight;

					if (m_eBkImageResizeMode == IRM_9GRID)
					{
						CIUIImage::PartNineGridBitmap(m_himgCombineBk,
							m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
							4, nStatusIndex, &hSubBmp,
							rcWnd.Width(), rcWnd.Height());
					}
					else if (m_eBkImageResizeMode == IRM_STRETCH
						|| m_eBkImageResizeMode == IRM_STRETCH_HIGH_QUALITY)
					{
						IUIPartStretchBlt(memDCBK.GetSafeHdc(), rcDest, m_himgCombineBk, rcSrc,
							m_eBkImageResizeMode);
					}

					// Then sub the background of client area.
					IUIBitBlt(memDCBK.GetSafeHdc(),
						0, 0, rcClient2Parent.Width(), rcClient2Parent.Height(),
						hSubBmp,
						nBorderCX + pMember->m_rcMargin.left, nBorderCY + pMember->m_rcMargin.top,
						SRCCOPY);
				}
			}
		}
		else
		{
			if (pMember->m_himgBk[nStatusIndex] != NULL)
			{
				// First stretch the background
				CIUIImage::NineGridBitmap(pMember->m_himgBk[nStatusIndex],
					m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
					&hSubBmp, rcWnd.Width(), rcWnd.Height());

				// Then sub the background of client area.
				IUIBitBlt(memDCBK.GetSafeHdc(),
					0, 0, rcClient2Parent.Width(), rcClient2Parent.Height(),
					hSubBmp,
					nBorderCX + pMember->m_rcMargin.left, nBorderCY + pMember->m_rcMargin.top,
					SRCCOPY);
			}
		}

		//
		// 3. Draw the default content(text)
		//
		DrawText(&memDCBK, rcClient2Parent, nBorderCX, nBorderCY, hSubBmp);

		ReleaseIUIImage(hSubBmp);
		hSubBmp = NULL;

		//
		// 4. Paint to dc.
		//
		dc.BitBlt(0, 0, rcClient2Parent.Width(), rcClient2Parent.Height(), &memDCBK, 0, 0, SRCCOPY);
		memDCBK.SelectObject(pBmpBKOld);
	}
	else if (message == WM_NCPAINT)
	{
		if (pMember->m_bNeedCallNCPaint)
		{
			CWindowDC dc(this);

			// Prepare non-client area.
			CRect rcWnd;
			GetWindowRect(rcWnd);
			rcWnd.OffsetRect(-rcWnd.left, -rcWnd.top);	// Set origin to (0, 0)

			// Get the original size of client area.
			CRect rcClient;
			GetClientRect(rcClient);
			rcClient.right += pMember->m_rcMargin.left;
			rcClient.right += pMember->m_rcMargin.right;
			rcClient.bottom += pMember->m_rcMargin.top;
			rcClient.bottom += pMember->m_rcMargin.bottom;

			// Calculate the width of the border.
			int nBorderCX = (rcWnd.Width() - rcClient.Width()) / 2;
			int nBorderCY = (rcWnd.Height() - rcClient.Height()) / 2;

			// Clip client area.
			CRect rcClip(rcWnd);
			rcClip.DeflateRect(nBorderCX, nBorderCY);
			rcClip.DeflateRect(pMember->m_rcMargin);
			dc.ExcludeClipRect(rcClip);

			CDC *pDC = &dc;

			int nStatusIndex = 0;
			BOOL bEnabled = IsWindowEnabled();
			BOOL bSelected = (GetKeyState(VK_LBUTTON) < 0);
			if (bEnabled)
			{
				if (pMember->m_bMouseOver || pMember->m_bNCMouseOver || pMember->m_bMarginAreaOver)
				{
					if (bSelected)
					{
						nStatusIndex = 2;
					}
					else
					{
						nStatusIndex = 1;
					}
				}
				else
				{
					nStatusIndex = 0;
				}
			}
			else
			{
				nStatusIndex = 3;
			}

			CDC memDCBK;
			CBitmap bmpBK;
			CBitmap *pBmpBKOld = NULL;

			// Draw the parent background to control for alpha blend with background of the control.
			BOOL bDrawParentPart = TRUE;
			if (m_bBkCombine)
			{
				m_himgCombineBk->SafeLoadSavedImage();

				if (!m_himgCombineBk->IsHasAlphaChannel())
				{
					bDrawParentPart = FALSE;
				}
			}
			else
			{
				pMember->m_himgBk[nStatusIndex]->SafeLoadSavedImage();

				if (!pMember->m_himgBk[nStatusIndex]->IsHasAlphaChannel())
				{
					bDrawParentPart = FALSE;
				}
			}

			BOOL bUpdateLayeredWindow = FALSE;
			if (!IsUpdateLayeredWindow())
			{
				if (bDrawParentPart)
				{
					CRect rcWnd2Parent;
					GetWindowRect(rcWnd2Parent);
					GetParent()->ScreenToClient(rcWnd2Parent);

					memDCBK.CreateCompatibleDC(&dc); // the size of memDC is same as it's parent
					bmpBK.CreateCompatibleBitmap(&dc, rcWnd2Parent.Width(), rcWnd2Parent.Height());
					ASSERT(bmpBK.GetSafeHandle() != NULL);
					pBmpBKOld = memDCBK.SelectObject(&bmpBK);

					memDCBK.ExcludeClipRect(rcClip);

					BOOL bUpdateLayeredWindow = FALSE;
					DrawParentPart(this, &memDCBK, rcWnd2Parent, &bUpdateLayeredWindow, NULL);

					pDC = &memDCBK;
				}
			}
			else
			{
				bUpdateLayeredWindow = TRUE;
			}

			// Draw non-client area.

			if (m_bBkCombine)
			{
				IUIPartNineGridBlt(pDC->GetSafeHdc(), rcWnd, m_himgCombineBk,
					m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY, 4, nStatusIndex);
			}
			else
			{
				IUINineGridBlt(pDC->GetSafeHdc(), rcWnd, pMember->m_himgBk[nStatusIndex],
					m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
			}

			if (bDrawParentPart)
			{
				dc.BitBlt(0, 0, rcWnd.Width(), rcWnd.Height(), &memDCBK, 0, 0, SRCCOPY);
				memDCBK.SelectObject(pBmpBKOld);
			}

			return 0;
		}
	}
	else if (message == WM_DESTROY)
	{
		if (m_ctrlSpinButton.GetSafeHwnd() != NULL)
		{
			m_ctrlSpinButton.UnsubclassWindow();
		}
	}
	// Adjust the size of child up-down control
	else if (message == WM_SIZE && m_ctrlSpinButton.GetSafeHwnd() != NULL)
	{
		int cx = LOWORD(lParam);
		int cy = HIWORD(lParam);

		LayoutSpinButton();

		// If the date time control moved or sized, the spin button may not be refresh.
		// The WS_CLIPCHILDREN and CDC::SelectClipRgn not valid.
		// Make date time control to draw.
		LRESULT lr = CDateTimeCtrl::WindowProc(message, wParam, lParam);

		m_ctrlSpinButton.Invalidate();

		return lr;
	}

	return CDateTimeCtrl::WindowProc(message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
// protected

int CSkinDateTimeCtrl::UpdateNeedNCPaintState()
{
	DATETIMEMEMBER *pMember = (DATETIMEMEMBER *)m_pMember;

	if (pMember->m_rcMargin == CRect(0, 0, 0, 0))
	{
		pMember->m_bNeedCallNCPaint = FALSE;
	}
	else
	{
		pMember->m_bNeedCallNCPaint = TRUE;
	}

	return 0;
}

int CSkinDateTimeCtrl::DrawText(CDC *pDCMem, const CRect &rcClient2Parent,
	int nBorderCX, int nBorderCY, HIUIIMAGE hSubBmp)
{
	DATETIMEMEMBER *pMember = (DATETIMEMEMBER *)m_pMember;

	CDC dcText;
	dcText.CreateCompatibleDC(pDCMem);
	CBitmap bmpText;
	bmpText.CreateCompatibleBitmap(pDCMem, rcClient2Parent.Width(), rcClient2Parent.Height());
	ASSERT(bmpText.GetSafeHandle() != NULL);
	CBitmap *pbmpTextOld = dcText.SelectObject(&bmpText);

	// The clip area is only valid for text, but not for the
	// right button while call CWnd::DefWindowProc(WM_PAINT, (WPARAM)HDC, 0)
	// the right button can't be clip. So, create a text dc to draw the default text.

	//
	// 1. Draw parent part
	//
	if (hSubBmp->IsHasAlphaChannel())
	{
		// Draw the parent background to control for alpha blend with background of the control.
		BOOL bUpdateLayeredWindow = FALSE;
		DrawParentPart(this, &dcText, rcClient2Parent, &bUpdateLayeredWindow, NULL);
	}

	//
	// 2. Draw background
	//
	IUIBitBlt(dcText.GetSafeHdc(),
		0, 0, rcClient2Parent.Width(), rcClient2Parent.Height(),
		hSubBmp,
		nBorderCX + pMember->m_rcMargin.left, nBorderCY + pMember->m_rcMargin.top, SRCCOPY);

	// store the Font object before call DefWindowProc
	HFONT hFontOld = (HFONT)::GetCurrentObject(dcText.GetSafeHdc(), OBJ_FONT);

	//
	// 3. Draw default content
	//
	CWnd::DefWindowProc(WM_PAINT, (WPARAM)dcText.m_hDC, 0);

	//
	// 4. Draw to memory dc
	//

	// draw the text to memBkDC by erase background text color,
	// clip the button contains drop-down arrow.
	int nButtonWidth = GetSystemMetrics(SM_CXVSCROLL);
	pDCMem->BitBlt(0, 0, rcClient2Parent.Width() - nButtonWidth, rcClient2Parent.Height(),
		&dcText, 0, 0, SRCCOPY);
	dcText.SelectObject(pbmpTextOld);

	return 0;
}

int CSkinDateTimeCtrl::LayoutSpinButton()
{
	DATETIMEMEMBER *pMember = (DATETIMEMEMBER *)m_pMember;

	// The spin button's width can't be adjust.
	return -1;

	ASSERT(::IsWindow(m_hWnd));

	if (pMember->m_rcMargin.right <= 0)
	{
		return 1;
	}

	if (m_ctrlSpinButton.GetSafeHwnd() == NULL)
	{
		return 2;
	}

	CRect rcSpinButton;
	m_ctrlSpinButton.GetWindowRect(rcSpinButton);
	ScreenToClient(rcSpinButton);
	int nRight = rcSpinButton.right;
	rcSpinButton.OffsetRect(-pMember->m_rcMargin.right, 0);
	rcSpinButton.right = nRight;

	m_ctrlSpinButton.MoveWindow(rcSpinButton);

	return 0;
}


//////////////////////////////////////////////////////////////////////////
// public

int CSkinDateTimeCtrl::SetBkImages(
	UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH, LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD)
{
	DATETIMEMEMBER *pMember = (DATETIMEMEMBER *)m_pMember;

	m_bBkCombine = false;
	return IUISetControlImage4(uMask, pMember->m_himgBk,
			lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);
}

int CSkinDateTimeCtrl::GetBkImages(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameH,
	CString *pstrImageNameS, CString *pstrImageNameD)
{
	DATETIMEMEMBER *pMember = (DATETIMEMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bBkCombine;
	return IUIGetControlImage4(uMask, m_himgCombineBk, pMember->m_himgBk, *pbCombineImage,
			pstrCombineImageName,
			pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);
}

int CSkinDateTimeCtrl::SetButtonWidth(int nWidth)
{
	DATETIMEMEMBER *pMember = (DATETIMEMEMBER *)m_pMember;

	int nDropButtonCX = GetSystemMetrics(SM_CXVSCROLL);
	pMember->m_rcMargin.right = nWidth - nDropButtonCX;
	if (pMember->m_rcMargin.right < 0)
	{
		pMember->m_rcMargin.right = 0;
		UpdateNeedNCPaintState();

		return -1;
	}

	UpdateNeedNCPaintState();

	// if use time style
	LayoutSpinButton();

	return 0;
}

int CSkinDateTimeCtrl::GetButtonWidth() const
{
	DATETIMEMEMBER *pMember = (DATETIMEMEMBER *)m_pMember;

	return pMember->m_rcMargin.right + GetSystemMetrics(SM_CXVSCROLL);
}

// text's font
int CSkinDateTimeCtrl::SetTextFont(UINT uMask, LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDH,
	LPCTSTR lpszFontIDS, LPCTSTR lpszFontIDD, BOOL bRedraw/* = TRUE*/)
{
	DATETIMEMEMBER *pMember = (DATETIMEMEMBER *)m_pMember;

	IUISetControlFont4(uMask, pMember->m_hIUIFont,
		lpszFontIDN, lpszFontIDH, lpszFontIDS, lpszFontIDD);

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CSkinDateTimeCtrl::GetTextFont(UINT uMask, CString *pstrFontIDN, CString *pstrFontIDH,
	CString *pstrFontIDS, CString *pstrFontIDD) const
{
	DATETIMEMEMBER *pMember = (DATETIMEMEMBER *)m_pMember;

	if (pstrFontIDN != NULL)
	{
		GetFontResID(pMember->m_hIUIFont[0], pstrFontIDN);
	}
	if (pstrFontIDH != NULL)
	{
		GetFontResID(pMember->m_hIUIFont[1], pstrFontIDH);
	}
	if (pstrFontIDS != NULL)
	{
		GetFontResID(pMember->m_hIUIFont[2], pstrFontIDS);
	}
	if (pstrFontIDD != NULL)
	{
		GetFontResID(pMember->m_hIUIFont[3], pstrFontIDD);
	}

	return 0;
}

int CSkinDateTimeCtrl::SetTextOffset(const LPPOINT lpPt)
{
	DATETIMEMEMBER *pMember = (DATETIMEMEMBER *)m_pMember;

	if (lpPt == NULL)
	{
		return -1;
	}

	pMember->m_rcMargin.left = lpPt->x;
	pMember->m_rcMargin.top = lpPt->y;

	// Make to call WM_NCCALCSIZE
	UpdateNeedNCPaintState();

	// Must add SWP_NOZORDER, otherwise cause the tab order changed.
	SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER
		| SWP_NOACTIVATE | SWP_DRAWFRAME | SWP_FRAMECHANGED);

	return 0;
}

int CSkinDateTimeCtrl::GetTextOffset(LPPOINT lpPt) const
{
	DATETIMEMEMBER *pMember = (DATETIMEMEMBER *)m_pMember;

	if (lpPt == NULL)
	{
		return -1;
	}

	lpPt->x = pMember->m_rcMargin.left;
	lpPt->y = pMember->m_rcMargin.top;

	return 0;
}
