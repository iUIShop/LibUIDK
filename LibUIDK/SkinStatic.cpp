// SkinStatic.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct STATICMEMBER
{
	STATICMEMBER()
	{
		m_rcPadding4TextN.SetRect(0, 0, 0, 0);
		m_rcPadding4TextD.SetRect(0, 0, 0, 0);
		m_eHorAlignModeN = TAH_DEFAULT;
		m_eHorAlignModeD = TAH_DEFAULT;
		m_eVerAlignModeN = TAV_DEFAULT;
		m_eVerAlignModeD = TAV_DEFAULT;
		m_uTextFormat = DT_WORDBREAK;

		int i = 0;
		for (i = 0; i < 2; ++i)
		{
			m_hIUIFont[i] = NULL;
		}

		m_crN = RGB(0, 0, 0);
		m_crD = RGB(0, 0, 0);

		for (i = 0; i < 2; ++i)
		{
			m_himgBk[i] = NULL;
		}

		m_nXSrc = 0;
		m_nYSrc = 0;

		for (i = 0; i < 2; ++i)
		{
			m_himgForeground[i] = NULL;
		}

		m_eForegroundAlignHor = FAH_LEFT;
		m_eForegroundAlignVer = FAV_CENTER;
		m_rcForegroundMargin.SetRect(0, 0, 0, 0);
		m_crMask = RGB(255, 0, 255);

		// for shadow text
		m_bShadowText = FALSE;
		m_crTextShadow = RGB(192, 192, 192);
		m_ptTextShadowOffset = CPoint(1, 1);
	}

	int Release()
	{
		m_rcPadding4TextN.SetRect(0, 0, 0, 0);
		m_rcPadding4TextD.SetRect(0, 0, 0, 0);
		m_eHorAlignModeN = TAH_DEFAULT;
		m_eHorAlignModeD = TAH_DEFAULT;
		m_eVerAlignModeN = TAV_DEFAULT;
		m_eVerAlignModeD = TAV_DEFAULT;
		m_uTextFormat = DT_WORDBREAK;

		int i = 0;
		for (i = 0; i < 2; ++i)
		{
			ReleaseIUIFontInternal(m_hIUIFont[i]);
			m_hIUIFont[i] = NULL;
		}

		m_crN = RGB(0, 0, 0);
		m_crD = RGB(0, 0, 0);

		for (i = 0; i < 2; ++i)
		{
			ReleaseIUIImage(m_himgBk[i]);
			m_himgBk[i] = NULL;
		}

		m_nXSrc = 0;
		m_nYSrc = 0;

		for (i = 0; i < 2; ++i)
		{
			ReleaseIUIImage(m_himgForeground[i]);
			m_himgForeground[i] = NULL;
		}

		m_eForegroundAlignHor = FAH_LEFT;
		m_eForegroundAlignVer = FAV_CENTER;
		m_rcForegroundMargin.SetRect(0, 0, 0, 0);
		m_crMask = RGB(255, 0, 255);

		// for shadow text
		m_bShadowText = FALSE;
		m_crTextShadow = RGB(192, 192, 192);
		m_ptTextShadowOffset = CPoint(1, 1);

		return 0;
	}

	CRect m_rcPadding4TextN;
	CRect m_rcPadding4TextD;
	TEXT_ALIGN_HOR m_eHorAlignModeN;
	TEXT_ALIGN_HOR m_eHorAlignModeD;
	TEXT_ALIGN_VER m_eVerAlignModeN;
	TEXT_ALIGN_VER m_eVerAlignModeD;
	UINT m_uTextFormat;

	// for font
	HIUIFONT m_hIUIFont[2];
	// for color
	COLORREF m_crN;
	COLORREF m_crD;

	CToolTipCtrl m_wndToolTip;
	HIUIIMAGE m_himgBk[2];
	int m_nXSrc;
	int m_nYSrc;

	// foreground bitmap
	HIUIIMAGE m_himgForeground[2];
	FOREGROUND_ALIGN_HOR m_eForegroundAlignHor;
	FOREGROUND_ALIGN_VER m_eForegroundAlignVer;
	CRect m_rcForegroundMargin;
	COLORREF m_crMask;

	// for shadow text
	BOOL m_bShadowText;
	COLORREF m_crTextShadow;
	CPoint m_ptTextShadowOffset;
};


/////////////////////////////////////////////////////////////////////////////
// CSkinStatic

CSkinStatic::CSkinStatic()
{
	m_pBindWnd = this;

	m_pMember = new STATICMEMBER;
}

CSkinStatic::~CSkinStatic()
{
	if (m_pMember != NULL)
	{
		delete (STATICMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

int CSkinStatic::BindStyle(LPCTSTR lpszStyleID)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (lpszStyleID == NULL)
	{
		return -2;
	}

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_STATIC, lpszStyleID);
	return BindStyle(pCtrlProp);

	return 0;
}

int CSkinStatic::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

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
	if (ect != CT_STYLE_STATIC && ect != CT_STATIC)
	{
		return -3;
	}

	TXTPROPERTIES *pStaProp = (TXTPROPERTIES *)pCtrlProp;

	// Set Background
	bool bSpecifyBackgroundImages = pCtrlProp->m_bSpecifyBackgroundImages;
	if (bSpecifyBackgroundImages)
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
			SetBitmap(CONTROL_STATE_UNCHECKED_ND, strImageName[1], strImageName[2]);
		}

		for (int i = 0; i < 1 + COMBINEIMAGESIZE2; ++i)
		{
			ReleaseIUIImage(strImageName[i]);
		}

		SetBkImageResizeMode(pCtrlProp->m_eBkImageResizeMode);
		SetBkImageResizePoint(pCtrlProp->m_ptImageResize);
	}

	// Set foreground
	CString strImageName[1 + COMBINEIMAGESIZE2];
	BOOL bCombineImage = TRUE;
	CTRLPROPERTIES::IUIGetStaticForeground2(pStaProp, &bCombineImage, strImageName);
	if (bCombineImage)
	{
		SetFgCombineImage(strImageName[0]);
	}
	else
	{
		SetForegroundBitmap(CONTROL_STATE_UNCHECKED_ND, strImageName[1], strImageName[2],
			pStaProp->m_rcPadding4Foreground,
			pStaProp->m_eForegroundHorAlignMode, pStaProp->m_eForegroundVerAlignMode);
	}

	// font
	CString strResFontID[2];
	CTRLPROPERTIES::IUIGetControlFont2(pCtrlProp, strResFontID);
	SetTextFont(CONTROL_STATE_NORMAL | CONTROL_STATE_DISABLED, strResFontID[0], strResFontID[1]);

	// control color
	COLORREF crN, crD;
	CTRLPROPERTIES::IUIGetControlColor4(pStaProp, &crN, NULL, NULL, &crD);
	SetTextColor(crN, crD);

	// set offset for the window's text
	// Horizontal Align
	SetTextAlignHor(pStaProp->m_eTextHorAlignMode, pStaProp->m_eTextHorAlignMode);

	// Vertical Align
	SetTextAlignVer(pStaProp->m_eTextVerAlignMode, pStaProp->m_eTextVerAlignMode);

	CRect rcMargin = pStaProp->m_rcPadding4Text;
	SetPadding4TextN(rcMargin);
	SetPadding4TextN(rcMargin); // Default to same as Normal

	SetTextMultiline(!(bool)pStaProp->m_bNoWrap);
	SetPathEllipsis(pStaProp->m_bPathEllipsis);
	SetEndEllipsis(pStaProp->m_bEndEllipsis);

	// Use tool tip
	if (pStaProp->m_bUseToolTip)
	{
		SetToolTips(pStaProp->m_strToolTip);
	}

	// Shadow text
	if (pStaProp->m_bShadowText)
	{
		ShadowText(TRUE);
		SetTextShadowColor(pStaProp->m_crTextShadow);
		POINT ptOffset = pStaProp->m_ptTextShadowOffset;
		SetTextShadowOffset(&ptOffset);
	}

	return 0;
}

int CSkinStatic::ReleaseObject()
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	pMember->Release();

	return CControlBase::ReleaseObject();
}

BEGIN_MESSAGE_MAP(CSkinStatic, CStatic)
	//{{AFX_MSG_MAP(CSkinStatic)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinStatic message handlers

LRESULT CSkinStatic::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class

	if (message == WM_SETTEXT && IsWindowVisible())
	{
		SetRedraw(FALSE);
		Default();
		SetRedraw(TRUE); // Call SetRedraw(FALSE) and SetRedraw(TRUE) can cause parent receive WM_MOUSEMOVE repeat. See the chat history with QQ:47326180 at 2013-01-23.
		Invalidate();
		UpdateWindow();
		return 0;
	}

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
		LPCREATESTRUCT	lpCreateStruct = LPCREATESTRUCT(lParam);
		lpCreateStruct->style &= ~SS_SUNKEN;
		lpCreateStruct->dwExStyle &= ~WS_EX_CLIENTEDGE;

		if (CStatic::OnCreate(lpCreateStruct) == -1)
		{
			return -1;
		}

		return 0;
	}

	else if (message == WM_ENABLE)
	{
		Invalidate();
	}

	else if (message == WM_PAINT)
	{
		CPaintDC dc(this); // device context for painting

		// init rect
		CRect rcClient;
		GetClientRect(rcClient);

		DRAWITEMSTRUCT dis;
		dis.CtlType = ODT_LISTVIEW;
		dis.CtlID = GetDlgCtrlID();
		dis.itemID = -1;
		dis.hwndItem = m_hWnd;
		dis.hDC = dc.m_hDC;
		dis.rcItem = rcClient;
		dis.itemData = IUIGetWindowLong(m_hWnd, GWLP_USERDATA);

		BOOL bParentDraw = (BOOL)GetParent()->SendMessage(WM_DRAWITEMEX, dis.CtlID, LPARAM(&dis));
		if (bParentDraw)
		{
			return CStatic::WindowProc(message, wParam, lParam);
		}

		// TODO: Add your message handler code here

		// init background dc
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
		ASSERT(bmp.GetSafeHandle() != NULL);
		CBitmap *pBmpOld = memDC.SelectObject(&bmp);

		OnDraw(&memDC);

		dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pBmpOld);
	}

	else if (message == WM_DRAWCONTROL)
	{
		CDC *pDC = (CDC *)wParam;
		OnDraw(pDC);
	}

	else if (message == WM_SETBKIMGTRANSPARENT)
	{
		OnSetBkImgTransparent((BYTE)wParam, (BOOL)lParam);
	}

	return CStatic::WindowProc(message, wParam, lParam);
}

BOOL CSkinStatic::PreTranslateMessage(MSG *pMsg)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class
	if (::IsWindow(pMember->m_wndToolTip.GetSafeHwnd()))
	{
		pMember->m_wndToolTip.RelayEvent(pMsg);
	}

	return CStatic::PreTranslateMessage(pMsg);
}

int CSkinStatic::OnDraw(CDC *pMemDC)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	if (pMemDC == NULL)
	{
		return -1;
	}

	BOOL bEnabled = IsWindowEnabled();
	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcWin;
	GetWindowRect(rcWin);
	GetParent()->ScreenToClient(rcWin);

	int nStateIndex = 0;
	if (!bEnabled)
	{
		nStateIndex = 1;
	}

	// Draw the parent background to control for alpha blend with background of the control.
	BOOL bDrawParentPart = TRUE;
	if (m_bBkCombine)
	{
		if (m_himgCombineBk != NULL && !m_himgCombineBk->IsHasAlphaChannel())
		{
			bDrawParentPart = FALSE;
		}
	}
	else
	{
		if (pMember->m_himgBk[nStateIndex] != NULL && !pMember->m_himgBk[nStateIndex]->IsHasAlphaChannel())
		{
			bDrawParentPart = FALSE;
		}
	}

	BOOL bUpdateLayeredWindow = FALSE;
	if (!IsUpdateLayeredWindow())
	{
		if (bDrawParentPart)
		{
			DrawParentPart(this, pMemDC, rcWin, &bUpdateLayeredWindow, NULL);
		}
	}
	else
	{
		bUpdateLayeredWindow = TRUE;
	}

	// If parent has called UpdateLayeredWindow, use direct-ui mode, adjust the rect
	if (bUpdateLayeredWindow)
	{
		rcClient = rcWin;
	}

	if (m_bBkCombine)
	{
		IUIPartNineGridBlt(pMemDC->GetSafeHdc(), rcClient, m_himgCombineBk,
			m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY, 2, nStateIndex);
	}
	else
	{
		IUIDrawImage(pMemDC->GetSafeHdc(), rcClient, pMember->m_himgBk[nStateIndex],
			m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
	}

	// copy foreground bitmap to background dc
	if (pMember->m_himgForeground[nStateIndex] != NULL)
		IUIDrawForeground(pMemDC->GetSafeHdc(), rcClient,
			pMember->m_rcForegroundMargin,
			pMember->m_himgForeground[nStateIndex],
			pMember->m_eForegroundAlignHor, pMember->m_eForegroundAlignVer,
			pMember->m_crMask, 255);

	// draw window's text to background dc
	DrawText(pMemDC, rcClient, bEnabled);

	return 0;
}

int CSkinStatic::DrawText(CDC *pMemDC, LPCRECT lprcClient, BOOL bEnabled)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	CString strText;
	GetWindowText(strText);
	if (strText.IsEmpty())
	{
		return 1;
	}

	CRect rcClient(lprcClient);

	CRect rcPadding4Text;
	CRect rcDraw(lprcClient);
	HFONT hFont = NULL;
	COLORREF cr = RGB(0, 0, 0);
	TEXT_ALIGN_HOR eHorAlignMode = TAH_LEFT;
	TEXT_ALIGN_VER eVerAlignMode = TAV_CENTER;
	if (bEnabled)
	{
		rcPadding4Text = pMember->m_rcPadding4TextN;
		rcDraw.DeflateRect(pMember->m_rcPadding4TextN);
		hFont = GetHFont(pMember->m_hIUIFont[0]);
		cr = pMember->m_crN;
		eHorAlignMode = pMember->m_eHorAlignModeN;
		eVerAlignMode = pMember->m_eVerAlignModeN;
	}
	else
	{
		rcPadding4Text = pMember->m_rcPadding4TextD;
		rcDraw.DeflateRect(pMember->m_rcPadding4TextD);
		hFont = GetHFont(pMember->m_hIUIFont[1]);
		cr = pMember->m_crD;
		eHorAlignMode = pMember->m_eHorAlignModeD;
		eVerAlignMode = pMember->m_eVerAlignModeD;
	}

	CString strDefaultFont;
	if (hFont == NULL)
	{
		strDefaultFont = CUIMgr::GetDefaultFont();
		hFont = CreateIUIFont(strDefaultFont);
		ReleaseIUIFont(strDefaultFont);
	}

	// ULW use GDIplus to draw the text for support alpha.
	if (IsUpdateLayeredWindow())
	{
		Graphics graphics(pMemDC->GetSafeHdc());

		Font font(pMemDC->GetSafeHdc(), hFont);
		StringAlignment eHorAlign = StringAlignmentNear;
		StringAlignment eVerAlign = StringAlignmentNear;
		Font *pFont = &font;

		switch (eHorAlignMode)
		{
		case 0: // default : left
		case 1: // left
		default: // left
			eHorAlign = StringAlignmentNear;
			break;
		case 2: // right
			eHorAlign = StringAlignmentFar;
			break;
		case 3: // center
			eHorAlign = StringAlignmentCenter;
			break;
		}
		switch (eVerAlignMode)
		{
		case 0: // default: top
		case 1: // top
		default: // top
			eVerAlign = StringAlignmentNear;
			break;
		case 2: // bottom
		{
			CRect rcTest(rcDraw);
			if (!IsTextMultiline())
			{
				int nHeight = pMemDC->DrawText(strText, rcTest, DT_CALCRECT);
				rcDraw.top += (rcDraw.Height() - nHeight);
			}
			else
			{
				eVerAlign = StringAlignmentFar;
			}
			break;
		}
		case 3: // vcenter
		{
			CRect rcTest(rcClient);
			if (IsTextMultiline())
			{
				int nHeight = pMemDC->DrawText(strText, rcTest, DT_CALCRECT);
				int nAdjustHeight = (rcClient.Height() - nHeight) / 2;
				rcDraw.DeflateRect(0, nAdjustHeight, 0, 0);
			}
			else
			{
				eVerAlign = StringAlignmentCenter;
			}
			break;
		}
		}

		// left, top, width, height
		RectF rcText(
			REAL(lprcClient->left),
			REAL(lprcClient->top),
			REAL(lprcClient->right - lprcClient->left),
			REAL(lprcClient->bottom - lprcClient->top));
		SolidBrush brush(Color(255, GetRValue(cr), GetGValue(cr), GetBValue(cr)));
		StringFormat format;
		format.SetAlignment(eHorAlign);

		// Draw string.
		graphics.DrawString(
			(LPCWSTR)_bstr_t((LPCTSTR)strText),
			strText.GetLength(),
			pFont,
			rcText,
			&format,
			&brush);
	}
	else
	{
		DrawFormatText(pMemDC->GetSafeHdc(), strText, strText.GetLength(), rcClient, rcPadding4Text,
			eHorAlignMode, eVerAlignMode, pMember->m_uTextFormat, hFont, cr, CT_STATIC);
	}

	if (strDefaultFont.IsEmpty())
	{
		ReleaseIUIFont(strDefaultFont);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Public member functions

void CSkinStatic::SetToolTips(LPCTSTR lpszToolTips)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	if (!::IsWindow(pMember->m_wndToolTip.GetSafeHwnd()))
	{
		pMember->m_wndToolTip.Create(this);
		ASSERT(::IsWindow(pMember->m_wndToolTip.GetSafeHwnd()));
	}
	pMember->m_wndToolTip.AddTool(this, lpszToolTips);
}

CToolTipCtrl *CSkinStatic::GetToolTipCtrl()
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	return &pMember->m_wndToolTip;
}

// text's color
void CSkinStatic::SetTextColor(COLORREF crN, COLORREF crD)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	pMember->m_crN = crN;
	pMember->m_crD = crD;
}

void CSkinStatic::GetTextColor(COLORREF *pcrN, COLORREF *pcrD)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	if (pcrN != NULL)
	{
		*pcrN = pMember->m_crN;
	}
	if (pcrD != NULL)
	{
		*pcrD = pMember->m_crD;
	}
}

// text's font
void CSkinStatic::SetTextFont(UINT uMask, LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDD)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	IUISetControlFont2(uMask, pMember->m_hIUIFont, lpszFontIDN, lpszFontIDD);

	Invalidate();
}

void CSkinStatic::GetTextFont(UINT uMask, CString *pstrFontIDN, CString *pstrFontIDD)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	if (pstrFontIDN != NULL)
	{
		GetFontResID(pMember->m_hIUIFont[0], pstrFontIDN);
	}

	if (pstrFontIDD != NULL)
	{
		GetFontResID(pMember->m_hIUIFont[1], pstrFontIDD);
	}
}

int CSkinStatic::SetPadding4TextN(LPCRECT lprcMargin)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	if (lprcMargin == NULL)
	{
		return -1;
	}

	pMember->m_rcPadding4TextN = *lprcMargin;

	return 0;
}

int CSkinStatic::GetPadding4TextN(LPRECT lprcMargin) const
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	if (lprcMargin == NULL)
	{
		return -1;
	}

	*lprcMargin = pMember->m_rcPadding4TextN;

	return 0;
}

int CSkinStatic::SetPadding4TextD(LPCRECT lprcMargin)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	if (lprcMargin == NULL)
	{
		return -1;
	}

	pMember->m_rcPadding4TextD = *lprcMargin;

	return 0;
}

int CSkinStatic::GetPadding4TextD(LPRECT lprcMargin) const
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	if (lprcMargin == NULL)
	{
		return -1;
	}

	*lprcMargin = pMember->m_rcPadding4TextD;

	return 0;
}

void CSkinStatic::SetTextAlignHor(TEXT_ALIGN_HOR eHorAlignModeN, TEXT_ALIGN_HOR eHorAlignModeD)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	pMember->m_eHorAlignModeN = eHorAlignModeN;
	pMember->m_eHorAlignModeD = eHorAlignModeD;
}

void CSkinStatic::GetTextAlignHor(TEXT_ALIGN_HOR *peHorAlignModeN, TEXT_ALIGN_HOR *peHorAlignModeD)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	if (peHorAlignModeN != NULL)
	{
		*peHorAlignModeN = pMember->m_eHorAlignModeN;
	}
	if (peHorAlignModeD != NULL)
	{
		*peHorAlignModeD = pMember->m_eHorAlignModeD;
	}
}

void CSkinStatic::SetTextAlignVer(TEXT_ALIGN_VER eVerAlignModeN, TEXT_ALIGN_VER eVerAlignModeD)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	pMember->m_eVerAlignModeN = eVerAlignModeN;
	pMember->m_eVerAlignModeD = eVerAlignModeD;
}

void CSkinStatic::GetTextAlignVer(TEXT_ALIGN_VER *peVerAlignModeN, TEXT_ALIGN_VER *peVerAlignModeD)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	if (peVerAlignModeN != NULL)
	{
		*peVerAlignModeN = pMember->m_eVerAlignModeN;
	}
	if (peVerAlignModeD != NULL)
	{
		*peVerAlignModeD = pMember->m_eVerAlignModeD;
	}
}

void CSkinStatic::SetTextMultiline(BOOL bMultiline)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	if (bMultiline)
	{
		pMember->m_uTextFormat &= ~DT_SINGLELINE;
		pMember->m_uTextFormat |= DT_WORDBREAK;
	}
	else
	{
		pMember->m_uTextFormat &= ~DT_WORDBREAK;
		pMember->m_uTextFormat |= DT_SINGLELINE;
	}
}

BOOL CSkinStatic::IsTextMultiline()
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	return IsIncludeFlag(pMember->m_uTextFormat, DT_WORDBREAK);
}

int CSkinStatic::SetPathEllipsis(BOOL bPathEllipsis)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	if (bPathEllipsis)
	{
		pMember->m_uTextFormat |= DT_PATH_ELLIPSIS;
	}
	else
	{
		pMember->m_uTextFormat &= ~DT_PATH_ELLIPSIS;
	}

	return 0;
}

BOOL CSkinStatic::IsPathEllipsis() const
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	return IsIncludeFlag(pMember->m_uTextFormat, DT_PATH_ELLIPSIS);
}

int CSkinStatic::SetEndEllipsis(BOOL bEndEllipsis)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	if (bEndEllipsis)
	{
		pMember->m_uTextFormat |= DT_END_ELLIPSIS;
	}
	else
	{
		pMember->m_uTextFormat &= ~DT_END_ELLIPSIS;
	}

	return 0;
}

BOOL CSkinStatic::IsEndEllipsis() const
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	return IsIncludeFlag(pMember->m_uTextFormat, DT_END_ELLIPSIS);
}

int CSkinStatic::SetBitmap(UINT uMask, LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameD, BOOL bRedraw/* = TRUE*/)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	m_bBkCombine = false;
	IUISetControlImage2(uMask, pMember->m_himgBk, lpszImageNameN, lpszImageNameD);

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CSkinStatic::GetBitmap(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameD) const
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bBkCombine;
	IUIGetControlImage2(uMask,
		m_himgCombineBk, pMember->m_himgBk, *pbCombineImage,
		pstrCombineImageName,
		pstrImageNameN, pstrImageNameD);

	return 0;
}

void CSkinStatic::SetForegroundBitmap(
	UINT uMask,
	LPCTSTR lpszImageNameN,
	LPCTSTR lpszImageNameD,
	const CRect &rcForegroundMargin,
	FOREGROUND_ALIGN_HOR eAlignModeHor/*=FAH_LEFT*/,
	FOREGROUND_ALIGN_VER eAlignModeVer/*=FAV_CENTER*/,
	COLORREF crMask/* = RGB(255, 0, 255)*/,
	BOOL bReDraw/* = TRUE*/)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	m_bBkCombine = false;
	IUISetControlImage2(uMask, pMember->m_himgForeground, lpszImageNameN, lpszImageNameD);

	pMember->m_eForegroundAlignHor = eAlignModeHor;
	pMember->m_eForegroundAlignVer = eAlignModeVer;
	pMember->m_rcForegroundMargin = rcForegroundMargin;
	pMember->m_crMask = crMask;

	if (bReDraw)
	{
		Redraw(this);
	}
}

void CSkinStatic::GetForegroundBitmap(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameD,
	LPRECT lprcForegroundMargin, int *pnAlignModeHor, int *pnAlignModeVer, COLORREF *pcrMask)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return;
	}

	*pbCombineImage = m_bBkCombine;
	IUIGetControlImage2(uMask,
		m_himgCombineBk, pMember->m_himgForeground, *pbCombineImage,
		pstrCombineImageName,
		pstrImageNameN, pstrImageNameD);

	if (lprcForegroundMargin != NULL)
	{
		*lprcForegroundMargin = pMember->m_rcForegroundMargin;
	}
	if (pnAlignModeHor != NULL)
	{
		*pnAlignModeHor = pMember->m_eForegroundAlignHor;
	}
	if (pnAlignModeVer != NULL)
	{
		*pnAlignModeVer = pMember->m_eForegroundAlignVer;
	}
	if (pcrMask != NULL)
	{
		*pcrMask = pMember->m_crMask;
	}
}


int CSkinStatic::ShadowText(BOOL bShadowText)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	pMember->m_bShadowText = bShadowText;

	return 0;
}

BOOL CSkinStatic::IsShadowText() const
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	return pMember->m_bShadowText;
}

int CSkinStatic::SetTextShadowColor(COLORREF crTextShadow)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	pMember->m_crTextShadow = crTextShadow;

	return 0;
}

COLORREF CSkinStatic::GetTextShadowColor() const
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	return pMember->m_crTextShadow;
}

int CSkinStatic::SetTextShadowOffset(LPPOINT lpptOffset)
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	if (lpptOffset == NULL)
	{
		return -1;
	}

	pMember->m_ptTextShadowOffset = *lpptOffset;

	return 0;
}

int CSkinStatic::GetTextShadowOffset(LPPOINT lpptOffset) const
{
	STATICMEMBER *pMember = (STATICMEMBER *)m_pMember;

	if (lpptOffset == NULL)
	{
		return -1;
	}

	*lpptOffset = pMember->m_ptTextShadowOffset;

	return 0;
}
