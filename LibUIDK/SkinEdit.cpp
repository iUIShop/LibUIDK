// SkinEdit.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


struct EDITMEMBER
{
	EDITMEMBER()
	{
		m_hIUIFont = NULL;
		m_clrText = RGB(0, 0, 0);   // text
		m_rcMargin.SetRect(0, 0, 0, 0);

		m_bIsMouseOver = FALSE;
		m_bSingleLineTopBottomMargin = FALSE;

		for (int i = 0; i < 4; ++i)
		{
			m_himgBk[i] = NULL;
		}

		m_bFirstInitBrush = true;
		m_brush = NULL;
		m_nDeflateRight = 0;
		m_bHasBorder = FALSE;
	}

	int Release()
	{
		ReleaseIUIFontInternal(m_hIUIFont);
		m_hIUIFont = NULL;

		m_clrText = RGB(0, 0, 0);   // text
		m_rcMargin.SetRect(0, 0, 0, 0);

		m_bIsMouseOver = FALSE;
		m_bSingleLineTopBottomMargin = FALSE;

		for (int i = 0; i < 4; ++i)
		{
			ReleaseIUIImage(m_himgBk[i]);
			m_himgBk[i] = NULL;
		}

		m_bFirstInitBrush = true;
		::DeleteObject(m_brush);
		m_brush = NULL;

		m_nDeflateRight = 0;
		m_bHasBorder = FALSE;

		return 0;
	}

	HIUIFONT m_hIUIFont;
	COLORREF m_clrText;		// text
	CRect m_rcMargin;

	BOOL m_bIsMouseOver;
	// Edit control with single line style and specified the top or bottom or both margin.
	BOOL m_bSingleLineTopBottomMargin;

	HIUIIMAGE m_himgBk[4]; // The Highlight state is for Combo Box, the Selected state is not used.
	bool m_bFirstInitBrush;
	HBRUSH m_brush;

	// For child of CCombobBox
	int m_nDeflateRight;

	CString m_strTag;

	// If the edit use by combo box, m_bHasBorder must be FALSE,
	// otherwise, the edit in combo box draw error.
	BOOL m_bHasBorder;
};

/////////////////////////////////////////////////////////////////////////////
// CSkinEdit

CSkinEdit::CSkinEdit()
{
	m_pBindWnd = this;

	m_pMember = new EDITMEMBER;
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;
}

CSkinEdit::~CSkinEdit()
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	if (m_pMember != NULL)
	{
		pMember->Release();
		delete (EDITMEMBER *)m_pMember;
		m_pMember = NULL;
	}
	// Need not delete the m_hbmpBk array, because
	// the CUIWnd delete theme in CUIWnd::DestroyChildControls

}

int CSkinEdit::BindStyle(LPCTSTR lpszStyleID)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	EDTPROPERTIES *pCtrlProp = (EDTPROPERTIES *)CUIMgr::GetStyleItem(STYLET_EDIT, lpszStyleID);
	return BindStyle(pCtrlProp);
}

int CSkinEdit::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

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
	if (ect != CT_STYLE_EDIT && ect != CT_EDIT)
	{
		return -3;
	}

	EDTPROPERTIES *pEdtProp = (EDTPROPERTIES *)pCtrlProp;

	if (!CString(LPCTSTR(pEdtProp->m_strBindStyle)).IsEmpty())
	{
		pEdtProp = (EDTPROPERTIES *)CUIMgr::GetStyleItem(STYLET_EDIT, (LPCTSTR)pEdtProp->m_strBindStyle);
		if (pEdtProp == NULL)
		{
			ASSERT(FALSE);
			return -4;
		}
	}

	// font
	CString strResFontID;
	CTRLPROPERTIES::IUIGetControlFont1(pEdtProp, &strResFontID);
	SetTextFont(strResFontID);

	// Alignment
	SetTextMargin(&((RECT)pEdtProp->m_rcPadding));

	// Set bitmaps for background
	bool bSpecifyBackgroundImages = pEdtProp->m_bSpecifyBackgroundImages;
	if (bSpecifyBackgroundImages)
	{
		CString strImageName[1 + COMBINEIMAGESIZE4];
		BOOL bCombineImage = TRUE;
		CTRLPROPERTIES::IUIGetBackground4(pEdtProp, &bCombineImage, strImageName);
		if (bCombineImage)
		{
			SetBkCombineImage(strImageName[0]);
		}
		else
		{
			SetImages(CONTROL_STATE_UNCHECKED_ALL,
				strImageName[1], strImageName[2], strImageName[3], strImageName[4]);
		}

		SetBkImageResizePoint(pEdtProp->m_ptImageResize);

		UpdateBackgroundBrush();
	}

	// Text color
	COLORREF crN = RGB(0, 0, 0);
	CTRLPROPERTIES::IUIGetControlColor4(pEdtProp, &crN, NULL, NULL, NULL);
	SetTextColor(crN);

	SetTag(pEdtProp->m_strTag);

	return 0;
}

int CSkinEdit::ReleaseObject()
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	pMember->Release();

	return CControlBase::ReleaseObject();
}

BEGIN_MESSAGE_MAP(CSkinEdit, CEdit)
	//{{AFX_MSG_MAP(CSkinEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinEdit message handlers

LRESULT CSkinEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class

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
				if (pMember->m_bSingleLineTopBottomMargin)
				{
					::RedrawWindow(m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW);
				}
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
		// The edit always not has WS_BORDER style?
		CREATESTRUCT *pcs = (CREATESTRUCT *)lParam;
		if ((pcs->style & WS_BORDER) == WS_BORDER)
		{
			pMember->m_bHasBorder = TRUE;
		}
		else
		{
			pMember->m_bHasBorder = FALSE;
		}
	}

	else if (message == WM_MOUSEACTIVATE)
	{
		LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		BOOL bMultiline = lStyle & ES_MULTILINE;
		if (!IUIIsWindowsXPSP2OrGreater() && !bMultiline)
		{
			UpdateBackgroundBrush();
		}
	}

	else if (message == WM_NCCALCSIZE)
	{
		// Set the margin of text in edit control,
		// If the edit control is multiline, use CEdit::SetRect
		// If the edit control is singleline, and the top and
		// bottom margin is 0, use CEdit::SetMargins.
		// Others, use WM_NCCALCSIZE message to set the margin.
		if (pMember->m_bSingleLineTopBottomMargin)
		{
			BOOL bCalcValidRects = (BOOL)wParam;
			if (bCalcValidRects)
			{
				NCCALCSIZE_PARAMS *pncp = (NCCALCSIZE_PARAMS *)lParam;

				pncp->rgrc[0].left += pMember->m_rcMargin.left;
				pncp->rgrc[0].top += pMember->m_rcMargin.top;
				pncp->rgrc[0].right -= pMember->m_rcMargin.right;
				pncp->rgrc[0].bottom -= pMember->m_rcMargin.bottom;

				return 0;
			}
			else
			{
				LPRECT lpRect = LPRECT(lParam);

				lpRect->left += pMember->m_rcMargin.left;
				lpRect->top += pMember->m_rcMargin.top;
				lpRect->right -= pMember->m_rcMargin.right;
				lpRect->bottom -= pMember->m_rcMargin.bottom;

				return 0;
			}
		}
	}

	else if (message == WM_MOUSEMOVE)
	{
		if (!pMember->m_bIsMouseOver)
		{
			pMember->m_bIsMouseOver = TRUE;
			UpdateBackgroundBrush();
		}

		// when the button as a menu bar, can not accept WM_MOUSELEAVE
		// message after the pop-up menu has been destroy
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = HOVER_DEFAULT;
		::TrackMouseEvent(&tme);
	}

	else if (message == WM_MOUSELEAVE)
	{
		pMember->m_bIsMouseOver = FALSE;
		UpdateBackgroundBrush();
	}

	else if (message == WM_HSCROLL || message == WM_VSCROLL)
	{
		BOOL bVisible = IsWindowVisible();
		if (bVisible)
		{
			SetRedraw(FALSE);
		}
		LRESULT lRet = CEdit::WindowProc(message, wParam, lParam);
		if (bVisible)
		{
			SetRedraw(TRUE);
			Invalidate();
			if (pMember->m_bSingleLineTopBottomMargin)
			{
				::RedrawWindow(m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW);
			}
			UpdateWindow();
		}
		return lRet;
	}

	// For combo box's edit
	else if (message == EM_SETSEL)
	{
		Invalidate();
		if (pMember->m_bSingleLineTopBottomMargin)
		{
			::RedrawWindow(m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW);
		}
	}

	else if (message == WM_SIZE)
	{
		UpdateBackgroundBrush();

		// If the edit is multi-line, reset the rect
		LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		BOOL bMultiline = lStyle & ES_MULTILINE;
		if (bMultiline)
		{
			LRESULT lr = CEdit::WindowProc(message, wParam, lParam);

			CRect rcClient;
			GetClientRect(rcClient);
			rcClient.DeflateRect(pMember->m_rcMargin);
			SetRect(rcClient);

			return lr;
		}
	}

	else if (message == WM_KILLFOCUS || message == WM_SETFOCUS)
	{
		UpdateBackgroundBrush();
	}

	// Draw the border
	else if (message == WM_PAINT)
	{
		LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		BOOL bMultiline = lStyle & ES_MULTILINE;

		// Edit control with multi-line or in XP sp2
		// operation system using WM_CTLCOLOR message to draw.
		if ((IUIIsWindowsXPSP2OrGreater() || bMultiline))
		{
			return CEdit::WindowProc(message, wParam, lParam);
		}
		// Otherwise, using WM_PAINT message to draw.
		else
		{
			CPaintDC dc(this); // device context for painting

			// TODO: Add your message handler code here

			CRect rcWnd;
			GetWindowRect(rcWnd);
			GetParent()->ScreenToClient(rcWnd);
			CRect rcClient;
			GetClientRect(rcClient);

			// Draw default to txtDC
			CDC txtDC;
			txtDC.CreateCompatibleDC(&dc);
			CBitmap memBmp;
			memBmp.CreateCompatibleBitmap(&dc, rcWnd.Width(), rcWnd.Height());
			ASSERT(memBmp.GetSafeHandle() != NULL);
			CBitmap *pmemBmpOld = txtDC.SelectObject(&memBmp);
			txtDC.FillSolidRect(rcClient, RGB(255, 255, 255));
			DefWindowProc(WM_PAINT, (WPARAM)txtDC.m_hDC, 0);

			// Draw bitmap background to bmpDC;
			CDC bmpDC;
			bmpDC.CreateCompatibleDC(&dc);

			if (m_bmpCompatible.GetSafeHandle() != NULL)
			{
				m_bmpCompatible.DeleteObject();
			}
			m_bmpCompatible.CreateCompatibleBitmap(&dc, rcWnd.Width(), rcWnd.Height());

			ASSERT(m_bmpCompatible.GetSafeHandle() != NULL);
			CBitmap *pbmpbmpDCOld = bmpDC.SelectObject(&m_bmpCompatible);

			DWORD dwStyle = GetStyle();
			BOOL bReadOnly = ((dwStyle & ES_READONLY) == ES_READONLY);
			BOOL bFocus = (GetFocus()->GetSafeHwnd() == m_hWnd);
			BOOL bEnable = IsWindowEnabled();
			int nStatusIndex = 0;
			if (bEnable)
			{
				if (bReadOnly)
				{
					nStatusIndex = 3;
				}
				else
				{
					if (bFocus)
					{
						nStatusIndex = 2;
					}
					else
					{
						if (pMember->m_bIsMouseOver)
						{
							nStatusIndex = 1;
						}
						else
						{
							nStatusIndex = 0;
						}
					}
				}
			}
			else
			{
				nStatusIndex = 3;
			}

			if (m_bBkCombine)
			{
				m_himgCombineBk->SafeLoadSavedImage();

				if (m_himgCombineBk->IsHasAlphaChannel())
				{
					CRect rcEdit(rcWnd);
					if (pMember->m_bSingleLineTopBottomMargin)
					{
						rcEdit.DeflateRect(pMember->m_rcMargin);
					}
					DrawParentPart(this, &bmpDC, rcEdit, NULL, NULL);
				}

				DrawCombineBackground(
					&bmpDC,
					CRect(0, 0, rcWnd.Width(), rcWnd.Height()),
					COMBINEIMAGESIZE4,
					nStatusIndex);
			}
			else
			{
				if (pMember->m_himgBk[nStatusIndex] != NULL)
				{
					pMember->m_himgBk[nStatusIndex]->SafeLoadSavedImage();

					if (pMember->m_himgBk[nStatusIndex]->IsHasAlphaChannel())
					{
						CRect rcEdit(rcWnd);
						if (pMember->m_bSingleLineTopBottomMargin)
						{
							rcEdit.DeflateRect(pMember->m_rcMargin);
						}
						DrawParentPart(this, &bmpDC, rcEdit, NULL, NULL);
					}

					IUIDrawImage(bmpDC.GetSafeHdc(), CRect(0, 0, rcWnd.Width(), rcWnd.Height()),
						pMember->m_himgBk[nStatusIndex],
						m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
				}
			}

			// Merge txtDC and bmpDC
			COLORREF crBK = RGB(255, 255, 255);
			if (IsWindowEnabled())
			{
				if (bReadOnly)
				{
					crBK = GetSysColor(COLOR_3DFACE);
				}
				else
				{
					crBK = GetSysColor(COLOR_WINDOW);
				}
			}
			else
			{
				crBK = GetSysColor(COLOR_3DFACE);
			}
			// Clip the border
			TransparentBlt2(bmpDC.m_hDC,
				2, 2, rcWnd.Width() - 4, rcWnd.Height() - 4,
				txtDC.m_hDC,
				2, 2, rcWnd.Width() - 4, rcWnd.Height() - 4, crBK);
			txtDC.SelectObject(pmemBmpOld);

			dc.BitBlt(0, 0, rcWnd.Width(), rcWnd.Height(), &bmpDC, 0, 0, SRCCOPY);
			bmpDC.SelectObject(pbmpbmpDCOld);
		}
	}

	// If disable or enable edit, redraw it
	else if (message == WM_ENABLE)
	{
		UpdateBackgroundBrush();
	}

	else if (message == EM_UPDATEBKBRUSH)
	{
		BOOL bUpdate = (BOOL)lParam;
		UpdateBackgroundBrush();
	}

	if (message == EM_SETREADONLY)
	{
		LRESULT lr = CEdit::WindowProc(message, wParam, lParam);
		UpdateBackgroundBrush();
		return lr;
	}

	else if (message == WM_DRAWPARENTPART)
	{
		CDC *pdcMem = (CDC *)lParam;

		CRect rcWnd;
		GetWindowRect(rcWnd);
		GetParent()->ScreenToClient(rcWnd);

		DrawParentPart(this, pdcMem, rcWnd, NULL, NULL);
	}

	else if (message == WM_DRAWCONTROL)
	{
		CDC *pDC = (CDC *)wParam;
		OnDraw(pDC);
	}

	else if (message == WM_GETBACKGROUND)
	{
		LRESULT lr = CEdit::WindowProc(message, wParam, lParam);	// Give derived class a chance.
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

	return CEdit::WindowProc(message, wParam, lParam);
}

BOOL LibUIDK::CSkinEdit::PreTranslateMessage(MSG *pMsg)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class
	// Edit use WM_SYSTIMER with timer id IDSYS_CARET draw the caret, see caret.c line 305

	// Is the top level window called UpdateLayeredWindow?
	HWND hWndToplevel = CWnd::GetSafeOwner_(m_hWnd, NULL);
	ASSERT(hWndToplevel != NULL);
	BOOL bUpdateLayeredWindow = (BOOL)::SendMessage(hWndToplevel, WM_ISUPDATELAYEREDWINDOW, 0, 0);
	if (bUpdateLayeredWindow)
	{
#define IDSYS_CARET         0x0000FFFFL
		if (pMsg->message == 0x0118)	// WM_SYSTIMER
		{
			if (pMsg->wParam == IDSYS_CARET)
			{
				CPoint pt = GetCaretPos();

				CDC *pDC = GetDC();
				Graphics g(pDC->GetSafeHdc());

				Pen blackPen(Color(255, 0, 0, 0), 1);

				// Create two Point objects that define the line.
				Point point1(pt.x, pt.y);
				Point point2(pt.x, pt.y + 20);

				// Draw the line.
				g.DrawLine(&blackPen, point1, point2);
			}
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

// Handle general key, avoid flash when input or delete characters.
void CSkinEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	// TODO: Add your message handler code here and/or call default
	ASSERT(m_hWnd != NULL);
	LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	BOOL bMultiline = lStyle & ES_MULTILINE;
	BOOL bVisible = IsWindowVisible();
	if (!IUIIsWindowsXPSP2OrGreater() && !bMultiline && bVisible)
	{
		SetRedraw(FALSE);
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
	if (!IUIIsWindowsXPSP2OrGreater() && !bMultiline && bVisible)
	{
		SetRedraw(TRUE);
		Invalidate();
		if (pMember->m_bSingleLineTopBottomMargin)
		{
			Invalidate();
		}
		UpdateWindow();
	}
}

// Handle navigation keys, avoid flash when move the navigation or Shift+navigation key
void CSkinEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	// TODO: Add your message handler code here and/or call default
	ASSERT(m_hWnd != NULL);
	LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	BOOL bMultiline = lStyle & ES_MULTILINE;
	BOOL bVisible = IsWindowVisible();
	if (!IUIIsWindowsXPSP2OrGreater() && !bMultiline && bVisible)
	{
		SetRedraw(FALSE);
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	if (!IUIIsWindowsXPSP2OrGreater() && !bMultiline && bVisible)
	{
		SetRedraw(TRUE);
		Invalidate();
		if (pMember->m_bSingleLineTopBottomMargin)
		{
			Invalidate();
		}
		UpdateWindow();
	}
}

// For WindowXP sp2 and new version or multi-line edit control.
HBRUSH CSkinEdit::CtlColor(CDC *pDC, UINT nCtlColor)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	// 如果Edit的父窗口没有指定背景图，在第一次调用CtlColor之前生成的brush，可能是黑色的。
	if (pMember->m_bFirstInitBrush)
	{
		UpdateBackgroundBrush();
		pMember->m_bFirstInitBrush = false;
	}

	// TODO: Change any attributes of the DC here
	TCHAR szName[16] = {0};
	::GetClassName(GetParent()->GetSafeHwnd(), szName, 15);
	if (_tcscmp(szName, _T("SysIPAddress32")) == 0)
	{
		::SetBrushOrgEx(pDC->m_hDC, -2, -2, NULL);
	}
	else
	{
		::SetBrushOrgEx(pDC->m_hDC, 0, 0, NULL);
	}
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(pMember->m_clrText);
	// TODO: Return a non-NULL brush if the parent's handler should not be called

	//
	// Draw border
	//
	if (pMember->m_bSingleLineTopBottomMargin)
	{
		DWORD dwStyle = GetStyle();
		BOOL bReadOnly = ((dwStyle & ES_READONLY) == ES_READONLY);
		BOOL bFocus = (GetFocus()->GetSafeHwnd() == m_hWnd);
		BOOL bEnable = IsWindowEnabled();
		int nStatusIndex = 0;
		if (bEnable)
		{
			if (bReadOnly)
			{
				nStatusIndex = 3;
			}
			else
			{
				if (bFocus)
				{
					nStatusIndex = 2;
				}
				else
				{
					if (pMember->m_bIsMouseOver)
					{
						nStatusIndex = 1;
					}
					else
					{
						nStatusIndex = 0;
					}
				}
			}
		}
		else
		{
			nStatusIndex = 3;
		}

		if (m_bBkCombine)
		{
			IUIDrawCombineNCBackground(this, m_himgCombineBk,
				m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				4, nStatusIndex, pMember->m_rcMargin);
		}
		else
		{
			IUIDrawNCBackground(this, pMember->m_himgBk[nStatusIndex],
				m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				pMember->m_rcMargin);
		}
	}

	return pMember->m_brush;
}

BOOL CSkinEdit::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT *pLResult)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class
	if (message == WM_COMMAND)
	{
		Invalidate();
	}

	return CEdit::OnChildNotify(message, wParam, lParam, pLResult);
}

//////////////////////////////////////////////////////////////////////////
//

int CSkinEdit::Render(CDC *pDC)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	return 0;
}

BOOL CSkinEdit::SetTextColor(COLORREF clrText)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	pMember->m_clrText = clrText;

	return TRUE;
}

COLORREF CSkinEdit::GetTextColor()
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	return pMember->m_clrText;
}

void CSkinEdit::SetDeflateRight(int nDeflate)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	pMember->m_nDeflateRight = nDeflate;
	CRect rcClient;
	GetClientRect(rcClient);
	SetWindowPos(NULL, 0, 0,
		rcClient.Width() - pMember->m_nDeflateRight, rcClient.Height(),
		SWP_NOMOVE | SWP_NOZORDER);
}

BOOL CSkinEdit::SetImages(UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
	LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD, BOOL bRedraw/* = TRUE*/)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	ASSERT(m_hWnd != NULL);

	m_bBkCombine = false;
	IUISetControlImage4(uMask, pMember->m_himgBk,
		lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);

	if (bRedraw)
	{
		UpdateBackgroundBrush();
	}

	return TRUE;
}

BOOL CSkinEdit::GetImages(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameH,
	CString *pstrImageNameS, CString *pstrImageNameD)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bBkCombine;
	return IUIGetControlImage4(uMask, m_himgCombineBk, pMember->m_himgBk, *pbCombineImage,
			pstrCombineImageName,
			pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);
}

int CSkinEdit::SetTextFont(LPCTSTR lpszFontID)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	ASSERT(::IsWindow(m_hWnd));

	IUISetControlFont(&pMember->m_hIUIFont, lpszFontID);

	pMember->m_hIUIFont->SafeLoadSavedFont();
	HFONT hNewFont = GetHFont(pMember->m_hIUIFont);
	if (hNewFont == NULL)
	{
		CString strDefaultFont = CUIMgr::GetDefaultFont();
		hNewFont = CreateIUIFont(strDefaultFont);
		ReleaseIUIFont(strDefaultFont);
	}
	::SendMessage(m_hWnd, WM_SETFONT, (WPARAM)hNewFont, TRUE);

	return 0;
}

int CSkinEdit::GetTextFont(CString *pstrFontResID)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	if (pstrFontResID == NULL)
	{
		return -1;
	}

	GetFontResID(pMember->m_hIUIFont, pstrFontResID);

	return 0;
}

int CSkinEdit::SetTextMargin(LPCRECT lpRect)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	ASSERT(m_hWnd != NULL);
	ASSERT(lpRect != NULL);
	pMember->m_rcMargin = *lpRect;

	LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	BOOL bMultiline = lStyle & ES_MULTILINE;
	if (bMultiline)
	{
		pMember->m_bSingleLineTopBottomMargin = FALSE;

		CRect rcClient;
		GetClientRect(rcClient);
		rcClient.DeflateRect(lpRect);
		SetRect(rcClient);
	}
	else
	{
		if (lpRect->top == 0 && lpRect->bottom == 0)
		{
			pMember->m_bSingleLineTopBottomMargin = FALSE;

			SetMargins(lpRect->left, lpRect->right);
		}
		else
		{
			pMember->m_bSingleLineTopBottomMargin = TRUE;

			// Use WM_NCCALCSIZE message to set the margin of this edit control.
			CRect rcEdit;
			GetWindowRect(rcEdit);
			GetParent()->ScreenToClient(rcEdit);

			// Must add SWP_NOZORDER, otherwise cause the tab order changed.
			SetWindowPos(NULL, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER
				| SWP_NOACTIVATE | SWP_DRAWFRAME | SWP_FRAMECHANGED);
		}
	}

	return 0;
}

int CSkinEdit::GetTextMargin(LPRECT lpRect) const
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	ASSERT(m_hWnd != NULL);
	ASSERT(lpRect != NULL);
	*lpRect = pMember->m_rcMargin;

	return 0;
}

void CSkinEdit::SetTag(LPCTSTR lpszTag)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	pMember->m_strTag = lpszTag;
}

void CSkinEdit::GetTag(CString &strTag)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	strTag = pMember->m_strTag;
}

BOOL CSkinEdit::IsHasBorder() const
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	return pMember->m_bHasBorder;
}

int CSkinEdit::UpdateBackgroundBrush()
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	BOOL bEnable = IsWindowEnabled();

	LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	BOOL bMultiline = lStyle & ES_MULTILINE;
	BOOL bFocus = (GetFocus()->GetSafeHwnd() == m_hWnd);
	BOOL bReadOnly = ((lStyle & ES_READONLY) == ES_READONLY);

	int nStatusIndex = 0;
	if (bEnable)
	{
		if (bReadOnly)
		{
			nStatusIndex = 3;
		}
		else
		{
			if (bFocus)
			{
				nStatusIndex = 2;
			}
			else
			{
				if (pMember->m_bIsMouseOver)
				{
					nStatusIndex = 1;
				}
				else
				{
					nStatusIndex = 0;
				}
			}
		}
	}
	else
	{
		nStatusIndex = 3;
	}

	CRect rcWin;
	GetWindowRect(rcWin);
	GetParent()->ScreenToClient(rcWin);

	//
	// 9 Grid stretch background image
	//
	HIUIIMAGE himgStretch = NULL;

	// ComboBox中的Edit控件在创建过程中，会被移动很多次。
	// 所以，为了保证实时性，在生成Edit背景时，
	// 直接把ComboBox的背景图和缩放点映射到Edit上后使用
	BOOL bParentComboBox = FALSE;
	CWnd *pParent = GetParent();
	if (NULL != pParent)
	{
		TCHAR szClassName[MAX_PATH] = {0};
		GetClassName(pParent->GetSafeHwnd(), szClassName, MAX_PATH);
		if (_tcscmp(_T("ComboBox"), szClassName) == 0)
		{
			bParentComboBox = TRUE;

			POINT ptResize = {0, 0};
			pParent->SendMessage(WM_GETBKIMGRESIZEPOINT, 0, (LPARAM)&ptResize);

			ptResize.x -= rcWin.left;
			ptResize.y -= rcWin.top;
			SetBkImageResizePoint(ptResize);

			CSkinComboBox *pComboBox = (CSkinComboBox *)pParent;
			BOOL bCombineImage;
			CString strCombineImage;
			CString strImage[COMBINEIMAGESIZE4];
			pComboBox->GetBitmap(CONTROL_STATE_ALL, &bCombineImage, &strCombineImage,
				&strImage[0], &strImage[1], &strImage[2], &strImage[3]);

			CRect rcParent;
			pParent->GetWindowRect(rcParent);
			rcParent.OffsetRect(-rcParent.TopLeft());

			// Edit相对ComboBox的边距
			CRect rcEditMargin;
			rcEditMargin.left = rcWin.left;
			rcEditMargin.top = rcWin.top;
			rcEditMargin.right = rcParent.right - rcWin.right;
			rcEditMargin.bottom = rcParent.bottom - rcWin.bottom;

			if (bCombineImage)
			{
				HIUIIMAGE hComboBoxCombineBk = CreateIUIImage(strCombineImage);
				if (NULL != hComboBoxCombineBk->GetSafeHBITMAP())
				{
					// 计算缩放ComboBox背景图的区域
					CRect rcPart;
					rcPart.left = hComboBoxCombineBk->GetWidth() / 4 * nStatusIndex;
					rcPart.right = rcPart.left + hComboBoxCombineBk->GetWidth() / 4;
					rcPart.top = 0;
					rcPart.bottom = hComboBoxCombineBk->GetHeight();
					rcPart.DeflateRect(rcEditMargin);

					CIUIImage::PartNineGridBitmap(hComboBoxCombineBk,
						ptResize, m_lBkImageRepeatX, m_lBkImageRepeatY, rcPart,
						&himgStretch, rcWin.Width(), rcWin.Height());
				}

				ReleaseIUIImage(hComboBoxCombineBk);
				hComboBoxCombineBk = NULL;
			}
			else
			{
				HIUIIMAGE hComboBoxBkStatus = CreateIUIImage(strImage[nStatusIndex]);
				if (NULL != hComboBoxBkStatus->GetSafeHBITMAP())
				{
					// 计算缩放ComboBox背景图的区域
					CRect rcPart;
					rcPart.left = 0;
					rcPart.right = hComboBoxBkStatus->GetWidth();
					rcPart.top = 0;
					rcPart.bottom = hComboBoxBkStatus->GetHeight();
					rcPart.DeflateRect(rcEditMargin);

					int nRet = CIUIImage::PartNineGridBitmap(hComboBoxBkStatus,
						ptResize, m_lBkImageRepeatX, m_lBkImageRepeatY, rcPart,
						&himgStretch, rcWin.Width(), rcWin.Height());
				}

				ReleaseIUIImage(hComboBoxBkStatus);
				hComboBoxBkStatus = NULL;
			}
		}
	}

	if (!bParentComboBox)
	{
		if (m_bBkCombine)
		{
			if (m_himgCombineBk == NULL)
			{
				return 1;
			}
		}
		else
		{
			if (pMember->m_himgBk[nStatusIndex] == NULL)
			{
				return 1;
			}
		}

		if (m_bBkCombine)
		{
			CIUIImage::PartNineGridBitmap(m_himgCombineBk,
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				4, nStatusIndex, &himgStretch,
				rcWin.Width(), rcWin.Height());
		}
		else
		{
			int nRet = CIUIImage::NineGridBitmap(pMember->m_himgBk[nStatusIndex],
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				&himgStretch, rcWin.Width(), rcWin.Height());
		}
	}

	if (himgStretch->GetSafeHBITMAP() == NULL)
	{
		return 2;
	}

	// Draw content to memory DC, and get the new background bitmap.
	CClientDC dc(this);
	HFONT hFontOld = (HFONT)::GetCurrentObject(dc.GetSafeHdc(), OBJ_FONT); // store the Font object
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	if (m_bmpCompatible.GetSafeHandle() != NULL)
	{
		m_bmpCompatible.DeleteObject();
	}
	m_bmpCompatible.CreateCompatibleBitmap(&dc, rcWin.Width(), rcWin.Height());

	ASSERT(m_bmpCompatible.GetSafeHandle() != NULL);
	CBitmap *pBmpOld = dcMem.SelectObject(&m_bmpCompatible);

	//
	// Draw parent part to memory DC
	//
	if (himgStretch->IsHasAlphaChannel())
	{
		CRect rcClientNon00 = rcWin;
		if (pMember->m_bSingleLineTopBottomMargin)
		{
			rcClientNon00.DeflateRect(pMember->m_rcMargin);
		}

		// If the edit control as combo box, IP address control's child.
		// the parent part is parent's parent.
		CWnd *pBackgroundParent = NULL;
		TCHAR szName[16] = {0};
		::GetClassName(GetParent()->GetSafeHwnd(), szName, 15);
		if (_tcscmp(szName, _T("SysIPAddress32")) == 0 || _tcscmp(szName, _T("ComboBox")) == 0)
		{
			pBackgroundParent = GetParent()->GetParent();
			GetWindowRect(rcClientNon00);
			pBackgroundParent->ScreenToClient(rcClientNon00);
		}

		DrawParentPart(this, &dcMem, rcClientNon00, NULL, pBackgroundParent);
	}

	//
	// Draw stretched background image to memory DC.
	//
	CRect rcClient;
	GetClientRect(rcClient);
	if (pMember->m_bSingleLineTopBottomMargin)
	{
		IUIBitBlt(dcMem.GetSafeHdc(), 0, 0, rcClient.Width(), rcClient.Height(),
			himgStretch,
			pMember->m_rcMargin.left, pMember->m_rcMargin.top, SRCCOPY);
	}
	else
	{
		IUIBitBlt(dcMem.GetSafeHdc(), 0, 0, rcClient.Width(), rcClient.Height(),
			himgStretch, 0, 0, SRCCOPY);
	}

	ReleaseIUIImage(himgStretch);
	himgStretch = NULL;

	dcMem.SelectObject(pBmpOld);
	dc.SelectObject(hFontOld);

	// Delete old brush
	if (pMember->m_brush != NULL)
	{
		::DeleteObject(pMember->m_brush);
		pMember->m_brush = NULL;
	}

	// Windows sp2 or later, if specify ES_MULTILINE style, origin of pDC in CtlColor
	// is same as it's parent, otherwise, it's same as itself
	// Less than Windows sp2, if specify ES_MULTILINE style, origin of pDC in CtlColor
	// is same as it's parent, otherwise, NOT use CtlColor to owner-draw(use OnPaint)
	// If want to set text color, the pMember->m_brush always not set to NULL
	pMember->m_brush = CreatePatternBrushG(m_bmpCompatible, RGB(255, 255, 255));

	// Refresh Client and non-client area.
	Redraw(this);
	if (!IsUpdateLayeredWindow())
	{
		if (pMember->m_bSingleLineTopBottomMargin)
		{
			::RedrawWindow(m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW);
		}
	}

	return 0;
}

int CSkinEdit::OnDraw(CDC *pMemDC)
{
	EDITMEMBER *pMember = (EDITMEMBER *)m_pMember;

	CRect rcWnd;
	GetWindowRect(rcWnd);
	GetParent()->ScreenToClient(rcWnd);

	CDC memEditDC;
	memEditDC.CreateCompatibleDC(pMemDC);
	CBitmap bmpDC;
	bmpDC.CreateCompatibleBitmap(pMemDC, rcWnd.Width(), rcWnd.Height());
	CBitmap *pbmpOld = memEditDC.SelectObject(&bmpDC);
	DefWindowProc(WM_PAINT, (WPARAM)memEditDC.m_hDC, 0);

	StretchAlphaBlend(pMemDC->GetSafeHdc(),
		rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(),
		memEditDC.GetSafeHdc(),
		0, 0, rcWnd.Width(), rcWnd.Height(), 255);

	memEditDC.SelectObject(pbmpOld);

	return 0;
}
