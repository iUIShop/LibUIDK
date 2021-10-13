// SkinProgressCtrl.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


struct PROGRESSMEMBER
{
	PROGRESSMEMBER()
	{
		m_bIsShowText = TRUE;
		m_sizeThumb.cx = m_sizeThumb.cy = 0;
		m_ptResizeThumb.x = m_ptResizeThumb.y = 1;
		m_bCombineThumbImage = TRUE;

		for (int i = 0; i < 2; ++i)
		{
			m_hIUIFont[i] = NULL;
			m_himgBk[i] = NULL;
			m_himgThumb[i] = NULL;
		}

		m_eThumbImageMode = PTM_STRETCH;
		m_eThumbPositionMode = PTP_DEFAULT;
		m_rcThumbOriginalSpecify.SetRect(-1, -1, -1, -1);
	}

	int Release()
	{
		m_bIsShowText = TRUE;
		m_sizeThumb.cx = m_sizeThumb.cy = 0;
		m_ptResizeThumb.x = m_ptResizeThumb.y = 1;
		m_bCombineThumbImage = TRUE;

		for (int i = 0; i < 2; ++i)
		{
			ReleaseIUIFontInternal(m_hIUIFont[i]);
			m_hIUIFont[i] = NULL;
			ReleaseIUIImage(m_himgBk[i]);
			m_himgBk[i] = NULL;
			ReleaseIUIImage(m_himgThumb[i]);
			m_himgThumb[i] = NULL;
		}

		m_eThumbImageMode = PTM_STRETCH;
		m_eThumbPositionMode = PTP_DEFAULT;
		m_rcThumbOriginalSpecify.SetRect(-1, -1, -1, -1);

		return 0;
	}

	BOOL m_bIsShowText;
	// the progress bar
	CSize m_sizeThumb;
	CPoint m_ptResizeThumb;
	BOOL m_bCombineThumbImage;

	HIUIFONT m_hIUIFont[2];

	HIUIIMAGE m_himgBk[2];
	HIUIIMAGE m_himgThumb[2];

	PROGRESS_THUMB_MODE m_eThumbImageMode;
	PROGRESS_THUMB_POSITION m_eThumbPositionMode;
	CRect m_rcThumbOriginalSpecify;
};

/////////////////////////////////////////////////////////////////////////////
// CSkinProgressCtrl

CSkinProgressCtrl::CSkinProgressCtrl()
{
	m_pBindWnd = this;

	m_pMember = new PROGRESSMEMBER;
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;
}

CSkinProgressCtrl::~CSkinProgressCtrl()
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	if (m_pMember != NULL)
	{
		delete (PROGRESSMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

int CSkinProgressCtrl::BindStyle(LPCTSTR lpszStyleID)
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (lpszStyleID == NULL)
	{
		return -2;
	}

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_PROGRESS, lpszStyleID);
	return BindStyle(pCtrlProp);

	return 0;
}

int CSkinProgressCtrl::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

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
	if (ect != CT_STYLE_PROGRESS && ect != CT_PROGRESS)
	{
		return -3;
	}

	PRGPROPERTIES *pPrgProp = (PRGPROPERTIES *)pCtrlProp;

	// Bitmap
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

	// Thumb images
	if ((bool)pPrgProp->m_bCombineThumbImages)
	{
		CombineThumbImage(TRUE);
		SetCombineThumbImage(pPrgProp->m_strThumbCombine);
	}
	else
	{
		CombineThumbImage(FALSE);
		SetThumbImages(CONTROL_STATE_UNCHECKED_ND, pPrgProp->m_strThumbN, pPrgProp->m_strThumbD);
	}

	// Thumb image stretch mode
	SetThumbImageMode((PROGRESS_THUMB_MODE)pPrgProp->m_eThumbMode);

	// Thumb position
	PROGRESS_THUMB_POSITION eThumbPosition = pPrgProp->m_eThumbPosition;
	SetThumbPositionMode(eThumbPosition);
	if (eThumbPosition == PTP_SPECIFIED)
	{
		CRect rcThumbOriginalSpecified = pPrgProp->m_rcThumbOriginalSpecified;
		SetThumbSpecifyOriginalPosition(rcThumbOriginalSpecified);
	}

	ShowText(pPrgProp->m_bShowText);

	// font
	CString strResFontID[2];
	CTRLPROPERTIES::IUIGetControlFont2(pCtrlProp, strResFontID);
	SetTextFont(CONTROL_STATE_NORMAL | CONTROL_STATE_DISABLED, strResFontID[0], strResFontID[1]);

	return 0;
}

int CSkinProgressCtrl::ReleaseObject()
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	pMember->Release();

	return CControlBase::ReleaseObject();
}


BEGIN_MESSAGE_MAP(CSkinProgressCtrl, CProgressCtrl)
	//{{AFX_MSG_MAP(CSkinProgressCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// protected

int CSkinProgressCtrl::DrawThumb(CDC *pMemDC)
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	LONG lStyle = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
	if ((lStyle & PBS_VERTICAL) == PBS_VERTICAL)
	{
		DrawVerticalThumb(pMemDC);
	}
	else
	{
		DrawHorizontalThumb(pMemDC);
	}

	return 0;
}

int CSkinProgressCtrl::DrawHorizontalThumb(CDC *pMemDC)
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	BOOL bEnabled = IsWindowEnabled();
	if (bEnabled && pMember->m_himgThumb[0] == NULL)
	{
		return -1;
	}
	if (!bEnabled && pMember->m_himgThumb[1] == NULL)
	{
		return -1;
	}

	CRect rcClient;
	GetClientRect(rcClient);

	int nPos = GetPos();
	int nLover = 0;
	int nUpper = 100;
	GetRange(nLover, nUpper);
	if (nUpper <= nLover)
	{
		nUpper = nLover + 1;
	}
	if (nPos < nLover)
	{
		nPos = nLover;
	}
	if (nPos > nUpper)
	{
		nPos = nUpper;
	}

	int nRange = 0;
	if (pMember->m_eThumbPositionMode == PTP_DEFAULT)
	{
		nRange = rcClient.Width();
	}
	else if (pMember->m_eThumbPositionMode == PTP_SPECIFIED)
	{
		nRange = pMember->m_rcThumbOriginalSpecify.Width();
	}
	else
	{
		ASSERT(FALSE);
	}
	int nWidth = nRange * (nPos - nLover) / (nUpper - nLover);

	CRect rcThumb(rcClient);
	if (pMember->m_eThumbPositionMode == PTP_DEFAULT)
	{
		rcThumb.right = rcThumb.left + nWidth;
	}
	else if (pMember->m_eThumbPositionMode == PTP_SPECIFIED)
	{
		rcThumb = pMember->m_rcThumbOriginalSpecify;
		rcThumb.right = rcThumb.left + nWidth;
	}
	else
	{
		ASSERT(FALSE);
	}

	// 9 Grid stretch the thumb image
	if (pMember->m_eThumbImageMode == PTM_STRETCH)
	{
		if (pMember->m_bCombineThumbImage)
		{
			IUIPartNineGridBlt(pMemDC->GetSafeHdc(), rcThumb,
				pMember->m_himgThumb[0],
				pMember->m_ptResizeThumb, 1, 1, 2, bEnabled ? 0 : 1);
		}
		else
		{
			IUINineGridBlt(pMemDC->GetSafeHdc(), rcThumb,
				bEnabled ? pMember->m_himgThumb[0] : pMember->m_himgThumb[1],
				pMember->m_ptResizeThumb, 1, 1);
		}
	}
	// Tile the thumb image
	else if (pMember->m_eThumbImageMode == PTM_TILE)
	{
		if (pMember->m_bCombineThumbImage)
		{
			IUITileBlt(pMemDC->GetSafeHdc(), rcThumb,
				bEnabled ? pMember->m_himgThumb[0] : pMember->m_himgThumb[1], 0, 0, -1, -1);
		}
		else
		{
			IUITileBlt(pMemDC->GetSafeHdc(), rcThumb,
				bEnabled ? pMember->m_himgThumb[0] : pMember->m_himgThumb[1], 0, 0, -1, -1);
		}
	}
	// The thumb image size is all of the client. clip the thumb.
	else if (pMember->m_eThumbImageMode == PTM_CLIP)
	{
		if (pMember->m_bCombineThumbImage)
		{
			IUIBitBlt(pMemDC->GetSafeHdc(), rcThumb,
				pMember->m_himgThumb[0], bEnabled ? 0 : pMember->m_sizeThumb.cx, 0, SRCCOPY);
		}
		else
		{
			IUIBitBlt(pMemDC->GetSafeHdc(), rcThumb,
				bEnabled ? pMember->m_himgThumb[0] : pMember->m_himgThumb[1], 0, 0, SRCCOPY);
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	return 0;
}

int CSkinProgressCtrl::DrawVerticalThumb(CDC *pMemDC)
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	BOOL bEnabled = IsWindowEnabled();
	if (bEnabled && pMember->m_himgThumb[0] == NULL)
	{
		return -1;
	}
	if (!bEnabled && pMember->m_himgThumb[1] == NULL)
	{
		return -1;
	}

	CRect rcClient;
	GetClientRect(rcClient);

#ifdef _DEBUG
	CRect rcWin;
	GetWindowRect(rcWin);
	ScreenToClient(rcWin);
#endif

	int nPos = GetPos();
	int nLover = 0;
	int nUpper = 100;
	GetRange(nLover, nUpper);
	if (nUpper <= nLover)
	{
		nUpper = nLover + 1;
	}
	if (nPos < nLover)
	{
		nPos = nLover;
	}
	if (nPos > nUpper)
	{
		nPos = nUpper;
	}

	int nRange = 0;
	if (pMember->m_eThumbPositionMode == PTP_DEFAULT)
	{
		nRange = rcClient.Height();
	}
	else if (pMember->m_eThumbPositionMode == PTP_SPECIFIED)
	{
		nRange = pMember->m_rcThumbOriginalSpecify.Height();
	}
	else
	{
		ASSERT(FALSE);
	}
	int nHeight = nRange * (nPos - nLover) / (nUpper - nLover);

	CRect rcThumb(rcClient);
	if (pMember->m_eThumbPositionMode == PTP_DEFAULT)
	{
		rcThumb.top = rcThumb.bottom - nHeight;
	}
	else if (pMember->m_eThumbPositionMode == PTP_SPECIFIED)
	{
		rcThumb = pMember->m_rcThumbOriginalSpecify;
		rcThumb.top = rcThumb.bottom - nHeight;
	}
	else
	{
		ASSERT(FALSE);
	}

	// 9 Grid stretch the thumb image
	if (pMember->m_eThumbImageMode == PTM_STRETCH)
	{
		if (pMember->m_bCombineThumbImage)
		{
			IUIPartNineGridBlt(pMemDC->GetSafeHdc(), rcThumb,
				pMember->m_himgThumb[0],
				pMember->m_ptResizeThumb, 1, 1, 2, bEnabled ? 0 : 1);
		}
		else
		{
			IUINineGridBlt(pMemDC->GetSafeHdc(), rcThumb,
				bEnabled ? pMember->m_himgThumb[0] : pMember->m_himgThumb[1],
				pMember->m_ptResizeThumb, 1, 1);
		}
	}
	// Tile the thumb image
	else if (pMember->m_eThumbImageMode == PTM_TILE)
	{
		if (pMember->m_bCombineThumbImage)
		{
			IUITileBlt(pMemDC->GetSafeHdc(), rcThumb,
				pMember->m_himgThumb[0], bEnabled ? 0 : pMember->m_sizeThumb.cx, 0,
				pMember->m_sizeThumb.cx, pMember->m_sizeThumb.cy);
		}
		else
		{
			IUITileBlt(pMemDC->GetSafeHdc(), rcThumb,
				bEnabled ? pMember->m_himgThumb[0] : pMember->m_himgThumb[1], 0, 0, -1, -1);
		}
	}
	// The thumb image size is all of the client. clip the thumb.
	else if (pMember->m_eThumbImageMode == PTM_CLIP)
	{
		if (pMember->m_bCombineThumbImage)
		{
			IUIBitBlt(pMemDC->GetSafeHdc(), rcThumb,
				pMember->m_himgThumb[0],
				bEnabled ? 0 : pMember->m_sizeThumb.cx, rcClient.Height() - nHeight, SRCCOPY);
		}
		else
		{
			IUIBitBlt(pMemDC->GetSafeHdc(), rcThumb,
				bEnabled ? pMember->m_himgThumb[0] : pMember->m_himgThumb[1],
				0, rcClient.Height() - nHeight, SRCCOPY);
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	return 0;
}

int CSkinProgressCtrl::DrawText(CDC *pMemDC)
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	if (!pMember->m_bIsShowText)
	{
		return -1;
	}

	int nPos = GetPos();
	int nLover = 0;
	int nUpper = 100;
	GetRange(nLover, nUpper);
	if (nUpper <= nLover)
	{
		nUpper = nLover + 1;
	}
	if (nPos < nLover)
	{
		nPos = nLover;
	}
	if (nPos > nUpper)
	{
		nPos = nUpper;
	}

	int nPercent = nPos * 100 / (nUpper - nLover);
	CString strText;
	strText.Format(_T("%d%%"), nPercent);
	pMemDC->SetBkMode(TRANSPARENT);
	HFONT hFont = GetHFont(IsWindowEnabled() ? pMember->m_hIUIFont[0] : pMember->m_hIUIFont[1]);

	CString strDefaultFont;
	if (hFont == NULL)
	{
		strDefaultFont = CUIMgr::GetDefaultFont();
		hFont = CreateIUIFont(strDefaultFont);
		ReleaseIUIFont(strDefaultFont);
	}

	HFONT hFontOld = (HFONT)::SelectObject(pMemDC->GetSafeHdc(), hFont);

	if (strDefaultFont.IsEmpty())
	{
		ReleaseIUIFont(strDefaultFont);
	}

	CRect rcClient;
	GetClientRect(rcClient);
	pMemDC->DrawText(strText, rcClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	SelectObject(pMemDC->GetSafeHdc(), hFontOld);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CSkinProgressCtrl message handlers

LRESULT CSkinProgressCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

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

	if (message == WM_ERASEBKGND)
	{
		return TRUE;
	}

	if (message == WM_PAINT)
	{
		CPaintDC dc(this); // device context for painting

		// TODO: Add your message handler code here
		CRect rcClient;
		GetClientRect(rcClient);

		// draw all contents to this DC
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		CBitmap bmpMem;
		bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
		ASSERT(bmpMem.GetSafeHandle() != NULL);
		CBitmap *pBmpMemOld = memDC.SelectObject(&bmpMem);

		int nStateIndex = 0;
		if (!IsWindowEnabled())
		{
			nStateIndex = 1;
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

		if (bDrawParentPart)
		{
			CRect rcWin;
			GetWindowRect(rcWin);
			CWnd *pParent = GetParent();
			pParent->ScreenToClient(rcWin);

			BOOL bUpdateLayeredWindow = FALSE;
			DrawParentPart(this, &memDC, rcWin, &bUpdateLayeredWindow, NULL);

			// If parent has called UpdateLayeredWindow, use direct-ui mode, adjust the rect
			if (bUpdateLayeredWindow)
			{
				rcClient = rcWin;
			}
		}

		// draw the background
		if (m_bBkCombine)
		{
			CRect rcClient;
			GetClientRect(rcClient);

			BOOL bUpdateLayeredWindow = (BOOL)GetParent()->SendMessage(WM_ISUPDATELAYEREDWINDOW);
			if (bUpdateLayeredWindow)
			{
				GetWindowRect(rcClient);
				GetParent()->ScreenToClient(rcClient);
			}

			DrawCombineBackground(&memDC, rcClient, 2, nStateIndex);
		}
		else
		{
			IUINineGridBlt(memDC.GetSafeHdc(),
				rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(),
				pMember->m_himgBk[nStateIndex],
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
		}

		// Draw thumb
		DrawThumb(&memDC);

		// Draw percent text
		DrawText(&memDC);

		dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pBmpMemOld);
	}

	if (message == WM_NCCALCSIZE)
	{
		return 0;	// Remove the border of the progress control.
	}

	return CProgressCtrl::WindowProc(message, wParam, lParam);
}


//////////////////////////////////////////////////////////////////////////
// member function

void CSkinProgressCtrl::ShowText(BOOL bIsShow)
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	pMember->m_bIsShowText = bIsShow;
}

BOOL CSkinProgressCtrl::IsShowText()
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	return pMember->m_bIsShowText;
}

int CSkinProgressCtrl::SetImages(UINT uMask, LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameD)
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	ASSERT(m_hWnd != NULL);

	m_bBkCombine = false;
	IUISetControlImage2(uMask, pMember->m_himgBk, lpszImageNameN, lpszImageNameD);

	return 0;
}

int CSkinProgressCtrl::GetImages(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameD)
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

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

int CSkinProgressCtrl::CombineThumbImage(BOOL bCombine)
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	pMember->m_bCombineThumbImage = bCombine;

	return 0;
}

BOOL CSkinProgressCtrl::IsCombineThumbImage() const
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	return pMember->m_bCombineThumbImage;
}

int CSkinProgressCtrl::SetCombineThumbImage(LPCTSTR lpszImageName)
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	ASSERT(m_hWnd != NULL);

	IUISetControlImage(&pMember->m_himgThumb[0], lpszImageName);

	BITMAP bmp;
	int nRet = ::GetObject(pMember->m_himgThumb[0], sizeof(bmp), &bmp) ;
	if (nRet != 0)
	{
		pMember->m_sizeThumb.cx = bmp.bmWidth / 2;
		pMember->m_sizeThumb.cy = bmp.bmHeight;
	}

	return 0;
}

int CSkinProgressCtrl::GetCombineThumbImage(CString *pstrImage) const
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	if (!pMember->m_bCombineThumbImage)
	{
		return -1;
	}

	return IUIGetControlImage(pMember->m_himgThumb[0], pstrImage);
}

int CSkinProgressCtrl::SetThumbImages(UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameD)
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	ASSERT(m_hWnd != NULL);

	IUISetControlImage2(uMask, pMember->m_himgThumb, lpszImageNameN, lpszImageNameD);

	BITMAP bmp;
	int nRet = ::GetObject(pMember->m_himgThumb[0], sizeof(bmp), &bmp) ;
	if (nRet != 0)
	{
		pMember->m_sizeThumb.cx = bmp.bmWidth;
		pMember->m_sizeThumb.cy = bmp.bmHeight;
	}

	return 0;
}

int CSkinProgressCtrl::GetThumbImages(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameD) const
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = pMember->m_bCombineThumbImage;
	IUIGetControlImage2(uMask,
		pMember->m_himgThumb[0], pMember->m_himgThumb, *pbCombineImage,
		pstrCombineImageName,
		pstrImageNameN, pstrImageNameD);

	return 0;
}

int CSkinProgressCtrl::SetThumbImageMode(PROGRESS_THUMB_MODE eMode)
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	pMember->m_eThumbImageMode = eMode;
	return 0;
}

PROGRESS_THUMB_MODE CSkinProgressCtrl::GetThumbImageMode() const
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	return pMember->m_eThumbImageMode;
}

int CSkinProgressCtrl::SetThumbPositionMode(PROGRESS_THUMB_POSITION ePositionMode)
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	pMember->m_eThumbPositionMode = ePositionMode;

	return 0;
}

PROGRESS_THUMB_POSITION CSkinProgressCtrl::GetThumbPositionMode() const
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	return pMember->m_eThumbPositionMode;
}

int CSkinProgressCtrl::SetThumbSpecifyOriginalPosition(LPCRECT lpThumbRect)
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	if (lpThumbRect == NULL)
	{
		return -1;
	}

	pMember->m_rcThumbOriginalSpecify = *lpThumbRect;

	return 0;
}

int CSkinProgressCtrl::GetThumbSpecifyOriginalPosition(LPRECT lpThumbRect) const
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	if (lpThumbRect == NULL)
	{
		return -1;
	}

	*lpThumbRect = pMember->m_rcThumbOriginalSpecify;

	return 0;
}

int CSkinProgressCtrl::SetTextFont(UINT uMask, LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDD)
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

	ASSERT(m_hWnd != NULL);

	IUISetControlFont2(uMask, pMember->m_hIUIFont, lpszFontIDN, lpszFontIDD);

	return 0;
}

int CSkinProgressCtrl::GetTextFont(UINT uMask, CString *pstrFontIDN, CString *pstrFontIDD)
{
	PROGRESSMEMBER *pMember = (PROGRESSMEMBER *)m_pMember;

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
