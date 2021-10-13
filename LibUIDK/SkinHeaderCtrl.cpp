// SkinHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct HEADERMEMBER
{
	HEADERMEMBER()
	{
		m_pWndTree = NULL;
		m_hBackground = NULL;
		m_himgCombineButtonBk = NULL;
		m_bButtonBkCombine = true;

		int i = 0;
		for (i = 0; i < 4; ++i)
		{
			m_himgButtonBk[i] = NULL;
		}

		m_ptButtonBkImageResize.x = m_ptButtonBkImageResize.y = 2;
		m_sizeButtonBkImageRepeat.cx = m_sizeButtonBkImageRepeat.cy = 1;

		for (i = 0; i < 4; ++i)
		{
			m_crText[i] = RGB(0, 0, 0);
			m_hIUIFont[i] = NULL;
		}

		m_nHeaderHeight = 17; // must be init
		m_nHeaderOrgHeight = 17;

		m_bHeaderTrack = TRUE;
		m_bHeaderDragDrop = FALSE;

		m_nCapture = -1;
		m_nMouseOver = -1;
	}

	int Release()
	{
		m_pWndTree = NULL;

		ReleaseIUIImage(m_hBackground);
		m_hBackground = NULL;

		ReleaseIUIImage(m_himgCombineButtonBk);
		m_himgCombineButtonBk = NULL;

		m_bButtonBkCombine = true;

		int i = 0;
		for (i = 0; i < 4; ++i)
		{
			ReleaseIUIImage(m_himgButtonBk[i]);
			m_himgButtonBk[i] = NULL;
		}

		m_ptButtonBkImageResize.x = m_ptButtonBkImageResize.y = 2;
		m_sizeButtonBkImageRepeat.cx = m_sizeButtonBkImageRepeat.cy = 1;

		for (i = 0; i < 4; ++i)
		{
			m_crText[i] = RGB(0, 0, 0);
			ReleaseIUIFontInternal(m_hIUIFont[i]);
			m_hIUIFont[i] = NULL;
		}

		m_nHeaderHeight = 17; // must be init
		m_nHeaderOrgHeight = 17;

		m_bHeaderTrack = TRUE;
		m_bHeaderDragDrop = FALSE;

		m_nCapture = -1;
		m_nMouseOver = -1;

		return 0;
	}

	CWnd *m_pWndTree;

	// For Header
	HIUIIMAGE m_hBackground;
	HIUIIMAGE m_himgCombineButtonBk;
	HIUIIMAGE m_himgButtonBk[4];
	BOOL m_bButtonBkCombine;
	CPoint m_ptButtonBkImageResize;
	CSize m_sizeButtonBkImageRepeat;

	COLORREF m_crText[4];
	HIUIFONT m_hIUIFont[4];

	int m_nHeaderHeight;
	int m_nHeaderOrgHeight;

	BOOL m_bHeaderTrack;
	BOOL m_bHeaderDragDrop;

	// for state
	int m_nCapture; // no item is captured if pMember->m_nCapture is -1
	int m_nMouseOver; // no item is highlight if pMember->m_nMouseOver is -1;
};

/////////////////////////////////////////////////////////////////////////////
// CSkinHeaderCtrl

CSkinHeaderCtrl::CSkinHeaderCtrl()
{
	m_pBindWnd = this;

	m_pMember = new HEADERMEMBER;
}

CSkinHeaderCtrl::~CSkinHeaderCtrl()
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	if (m_pMember != NULL)
	{
		delete (HEADERMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

int CSkinHeaderCtrl::BindStyle(LPCTSTR lpszStyleID)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	HEADERPROPERTIES *pHeaderProp = (HEADERPROPERTIES *)CUIMgr::GetStyleItem(STYLET_HEADERCTRL,
			lpszStyleID);

	return BindStyle(pHeaderProp);
}

int CSkinHeaderCtrl::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

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
	if (ect != CT_STYLE_HEADERCTRL && ect != CT_HEADERCTRL)
	{
		return -3;
	}

	HEADERPROPERTIES *pHeaderProp = (HEADERPROPERTIES *)pCtrlProp;

	// Header background
	SetBkImage(pHeaderProp->m_strHeaderBk);
	SetBkImageResizePoint(pHeaderProp->m_ptHeaderBkImageResize);

	// Header button background
	CString strHeaderButton[1 + COMBINEIMAGESIZE4];
	BOOL bCombineHeaderButton = TRUE;
	CTRLPROPERTIES::IUIGetBackground4(pHeaderProp, &bCombineHeaderButton, strHeaderButton);
	if (bCombineHeaderButton)
	{
		SetButtonCombineImages(strHeaderButton[0]);
	}
	else
	{
		SetButtonImages(CONTROL_STATE_UNCHECKED_ALL,
			strHeaderButton[1], strHeaderButton[2], strHeaderButton[3], strHeaderButton[4]);
	}

	SetButtonBkImageResizePoint(pCtrlProp->m_ptImageResize);
	SetButtonBkImageRepeatX(pCtrlProp->m_uXRepeatPixel);
	SetButtonBkImageRepeatY(pCtrlProp->m_uYRepeatPixel);

	// Header font
	CString strResFontID[4];
	CTRLPROPERTIES::IUIGetControlFont4(pCtrlProp, strResFontID);
	SetTextFont(CONTROL_STATE_UNCHECKED_ALL,
		strResFontID[0], strResFontID[1], strResFontID[2], strResFontID[3]);

	// Header text color
	COLORREF cr[4] = {0};
	CTRLPROPERTIES::IUIGetControlColor4(pCtrlProp, &cr[0], &cr[1], &cr[2], &cr[3]);
	SetTextColor(cr[0], cr[1], cr[2], cr[3]);

	EnableHeaderTrack(pHeaderProp->m_bHeaderTrack);
	EnableHeaderDragDrop(pHeaderProp->m_bHeaderDragDrop);

	return 0;
}

BEGIN_MESSAGE_MAP(CSkinHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CSkinHeaderCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(HDN_ENDTRACK, OnNMEndTrack)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinHeaderCtrl message handlers

void CSkinHeaderCtrl::OnPaint()
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	BOOL bHasButtonBk = FALSE;
	if (pMember->m_bButtonBkCombine)
	{
		bHasButtonBk = (pMember->m_himgCombineButtonBk != NULL);
	}
	else
	{
		bHasButtonBk = (pMember->m_himgButtonBk[0] != NULL);
	}

	if (pMember->m_hBackground == NULL
		&& !bHasButtonBk)
	{
		Default();
		return;
	}

	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	CRect rcClient;
	GetClientRect(rcClient);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	ASSERT(bmpMem.GetSafeHandle() != NULL);
	CBitmap *pBmpMemOld = memDC.SelectObject(&bmpMem);
	memDC.SetBkMode(TRANSPARENT);

	// Draw all of the width of background.
	if (pMember->m_hBackground != NULL)
	{
		IUINineGridBlt(memDC.GetSafeHdc(), 0, 0, rcClient.Width() + 1, rcClient.Height(),
			pMember->m_hBackground, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
	}

	// The right side of the header not include the right side of button.
	int nCount = GetItemCount();
	if (nCount == 0 && pMember->m_hBackground->GetSafeHBITMAP() == NULL)
	{
		IUINineGridBlt(memDC.GetSafeHdc(), 0, 0, rcClient.Width() + 1, rcClient.Height(),
			pMember->m_himgButtonBk[0], m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
	}
	else
	{
		CRect rcLastItem;
		GetItemRect(nCount - 1, rcLastItem);

		if (rcClient.Width() > rcLastItem.right)
			IUINineGridBlt(memDC.GetSafeHdc(),
				rcLastItem.right, 0, rcClient.Width() - rcLastItem.right + 1, rcClient.Height(),
				pMember->m_himgButtonBk[0], m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
	}

	// Draw items
	for (int i = 0; i < nCount; ++i)
	{
		DrawItem(&memDC, i);
	}

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pBmpMemOld);

	// Do not call CHeaderCtrl::OnPaint() for painting messages
}

void CSkinHeaderCtrl::DrawItem(CDC *pDC, int nItem)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	CRect rcItem;
	GetItemRect(nItem, rcItem);
	DRAWITEMSTRUCT dis;
	dis.CtlType = ODT_HEADER;
	dis.CtlID = GetDlgCtrlID();
	dis.itemID = nItem;
	dis.hwndItem = m_hWnd;
	dis.hDC = pDC->GetSafeHdc();
	dis.rcItem = rcItem;
	dis.itemData = 0;

	BOOL bParentDraw = (BOOL)GetParent()->SendMessage(WM_DRAWITEMEX, dis.CtlID, LPARAM(&dis));
	if (bParentDraw)
	{
		return;
	}


	BOOL bSelected = GetKeyState(VK_LBUTTON) < 0;

	// draw pMember->m_himgButtonBk to pDC
	HIUIIMAGE himgButtonBk = NULL;
	COLORREF crText = RGB(0, 0, 0);
	HFONT hFont = NULL;
	int nStateIndex = 0;
	if (IsWindowEnabled())
	{
		if (bSelected && (pMember->m_nCapture == nItem))
		{
			if (pMember->m_nMouseOver == nItem)
			{
				nStateIndex = 2;
				// 2.selected(push down)
			}
			else
			{
				nStateIndex = 1;
				// 3.highlight
			}
		}
		else
		{
			if ((pMember->m_nMouseOver == nItem) && (pMember->m_nCapture == -1))
			{
				nStateIndex = 1;
				// 3.highlight
			}
			else
			{
				nStateIndex = 0;
				// 4.normal
			}
		}
	}
	else
	{
		nStateIndex = 3;
	}
	crText = pMember->m_crText[nStateIndex];
	hFont = GetHFont(pMember->m_hIUIFont[nStateIndex]);

	// Button background
	if (pMember->m_bButtonBkCombine)
	{
		IUIPartNineGridBlt(pDC->GetSafeHdc(), rcItem.left, 0, rcItem.Width(), rcItem.Height(),
			pMember->m_himgCombineButtonBk, pMember->m_ptButtonBkImageResize,
			pMember->m_sizeButtonBkImageRepeat.cx, pMember->m_sizeButtonBkImageRepeat.cy,
			4, nStateIndex);
	}
	else
	{
		himgButtonBk = pMember->m_himgButtonBk[nStateIndex];
		if (himgButtonBk != NULL)
		{
			IUINineGridBlt(pDC->GetSafeHdc(), rcItem.left, 0, rcItem.Width(), rcItem.Height(),
				himgButtonBk, pMember->m_ptButtonBkImageResize,
				pMember->m_sizeButtonBkImageRepeat.cx, pMember->m_sizeButtonBkImageRepeat.cy);
		}
	}

	// draw text
	HDITEM hdi;
	TCHAR lpBuffer[MAX_PATH + 1];
	hdi.mask = HDI_TEXT | HDI_BITMAP | HDI_FORMAT;
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = MAX_PATH;
	GetItem(nItem, &hdi);

	int nBmpX = 0;
	int nBmpY = 0;
	if (hdi.hbm != NULL)
	{
		BITMAP bmp;
		::GetObject(hdi.hbm, sizeof(BITMAP), &bmp);
		nBmpX = bmp.bmWidth;
		nBmpY = bmp.bmHeight;
	}

	// draw text
#define TEXTLEFT_OFFSET_ITEMLEFT 4
#define TEXTRIGHT_OFFSET_ICONLEFT 4
#define ICONRIGHT_OFFSET_ITEMRIGHT 4
	CRect rcText(rcItem);
	if (_tcslen(hdi.pszText) > 0)
	{
		CString strDefaultFont;
		if (hFont == NULL)
		{
			strDefaultFont = CUIMgr::GetDefaultFont();
			hFont = CreateIUIFont(strDefaultFont);
			ReleaseIUIFont(strDefaultFont);
		}

		pDC->SetTextColor(crText);
		HFONT hFontOld = NULL;
		if (hFont != NULL)
		{
			hFontOld = (HFONT)::SelectObject(pDC->GetSafeHdc(), hFont);
		}

		if (strDefaultFont.IsEmpty())
		{
			ReleaseIUIFont(strDefaultFont);
		}
		UINT uFormat = DT_VCENTER | DT_SINGLELINE;
		if ((hdi.fmt & HDF_CENTER) == HDF_CENTER)
		{
			uFormat |= DT_CENTER;
		}
		else if ((hdi.fmt & HDF_RIGHT) == HDF_RIGHT)
		{
			uFormat |= DT_RIGHT;
		}
		else if ((hdi.fmt & HDF_RTLREADING) == HDF_RTLREADING)
		{
			uFormat |= DT_RTLREADING;
		}
		else
		{
			uFormat |= DT_LEFT;
		}
		if (hdi.hbm != NULL)
		{
			rcText.DeflateRect(TEXTLEFT_OFFSET_ITEMLEFT, 0,
				TEXTRIGHT_OFFSET_ICONLEFT + nBmpX + ICONRIGHT_OFFSET_ITEMRIGHT, 0);

			if (rcText.Width() > 0)
			{
				CRect rcTest(rcText);
				int nHeight = pDC->DrawText(hdi.pszText, rcTest, uFormat | DT_CALCRECT);

				if (rcTest.Width() < rcText.Width())
				{
					if ((uFormat & DT_CENTER) == DT_CENTER)
					{
						rcText.left += (rcText.Width() - rcTest.Width()) / 2;
						rcText.right = rcText.left + rcTest.Width();
					}
					else if ((uFormat & DT_RIGHT) == DT_RIGHT)
					{
						rcText.left = rcText.right - rcTest.Width();
					}
					else
					{
						rcText.right = rcText.left + rcTest.Width();
					}
				}
				else
				{
					uFormat &= ~DT_CENTER;
					uFormat &= ~DT_RIGHT;
				}
			}
		}
		else
		{
			rcText.DeflateRect(TEXTLEFT_OFFSET_ITEMLEFT, 0, TEXTRIGHT_OFFSET_ICONLEFT, 0);
			if (rcText.Width() > 0)
			{
				CRect rcTest(rcText);
				int nHeight = pDC->DrawText(hdi.pszText, rcTest, uFormat | DT_CALCRECT);

				if (rcTest.Width() > rcText.Width())
				{
					uFormat &= ~DT_CENTER;
					uFormat &= ~DT_RIGHT;
				}
			}
		}

		if (rcText.Width() > 0)
		{
			pDC->DrawText(hdi.pszText, rcText, uFormat | DT_END_ELLIPSIS);
		}

		if (hFontOld != NULL)
		{
			pDC->SelectObject(hFontOld);
		}
	}

	// draw sort image
	if (hdi.hbm != NULL)
	{
		CDC memDCSort;
		memDCSort.CreateCompatibleDC(pDC);
		HBITMAP hbmpOld = (HBITMAP)memDCSort.SelectObject(hdi.hbm);

		GetItemRect(nItem, rcItem);
		CRect rcIcon(rcItem);
		if (rcText.Width() > 0)
		{
			rcIcon.left = rcText.right + TEXTRIGHT_OFFSET_ICONLEFT;
			rcIcon.right = rcIcon.left + nBmpX;
		}
		else
		{
			rcIcon.left = max(rcItem.left + TEXTLEFT_OFFSET_ITEMLEFT,
					rcItem.right - ICONRIGHT_OFFSET_ITEMRIGHT - nBmpX);

			rcIcon.right = min(rcIcon.left + nBmpX, rcItem.right);
		}

		::TransparentBlt(pDC->GetSafeHdc(), rcIcon.left, 0, rcIcon.Width(), nBmpY,
			memDCSort.GetSafeHdc(), 0, 0, min(nBmpX, rcIcon.Width()), nBmpY, RGB(255, 0, 255));

		memDCSort.SelectObject(hbmpOld);
	}
}

void CSkinHeaderCtrl::OnNMEndTrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	NMHEADER *pNMHeader = (NMHEADER *)pNMHDR;
	CRect rcClient;
	GetClientRect(&rcClient);
	int nWidth = 0;
	for (int i = 0; i < pNMHeader->iItem; ++i)
	{
		CRect rcItem;
		GetItemRect(i, &rcItem);
		nWidth += rcItem.Width();
	}

	if (pNMHeader->pitem->cxy > rcClient.Width() - nWidth && pMember->m_pWndTree->GetSafeHwnd() != NULL)
	{
		pNMHeader->pitem->cxy = rcClient.Width() - nWidth;
	}

	*pResult = 0;
}

void CSkinHeaderCtrl::SetTreeCtrlEx(CWnd *pWnd)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	pMember->m_pWndTree = pWnd;
}

CWnd *CSkinHeaderCtrl::GetTreeCtrlEx()
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	return pMember->m_pWndTree;
}

int CSkinHeaderCtrl::SetBkImage(LPCTSTR lpszImageName)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	return IUISetControlImage(&pMember->m_hBackground, lpszImageName);
}

int CSkinHeaderCtrl::GetBkImage(CString *pstrImageName) const
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	return IUIGetControlImage(pMember->m_hBackground, pstrImageName);
}

int CSkinHeaderCtrl::SetButtonCombineImages(LPCTSTR lpszImageName)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	pMember->m_bButtonBkCombine = true;
	return IUISetControlImage(&pMember->m_himgCombineButtonBk, lpszImageName);
}

int CSkinHeaderCtrl::SetButtonImages(
	UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH, LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	pMember->m_bButtonBkCombine = false;
	return IUISetControlImage4(uMask, pMember->m_himgButtonBk,
			lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);
}

int CSkinHeaderCtrl::GetButtonImages(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameH,
	CString *pstrImageNameS, CString *pstrImageNameD) const
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bBkCombine;
	return IUIGetControlImage4(uMask, m_himgCombineBk, pMember->m_himgButtonBk, *pbCombineImage,
			pstrCombineImageName,
			pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);
}

int CSkinHeaderCtrl::SetButtonBkImageResizePoint(POINT pt)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	pMember->m_ptButtonBkImageResize = pt;

	return 0;
}

int CSkinHeaderCtrl::GetButtonBkImageResizePoint(LPPOINT lpPt) const
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	if (lpPt == NULL)
	{
		return -1;
	}

	*lpPt = pMember->m_ptButtonBkImageResize;

	return 0;
}

int CSkinHeaderCtrl::SetButtonBkImageRepeatX(LONG nRepeat)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	pMember->m_sizeButtonBkImageRepeat.cx = nRepeat;

	return 0;

}

LONG CSkinHeaderCtrl::GetButtonBkImageRepeatX() const
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	return pMember->m_sizeButtonBkImageRepeat.cx;
}

int CSkinHeaderCtrl::SetButtonBkImageRepeatY(LONG nRepeat)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	pMember->m_sizeButtonBkImageRepeat.cy = nRepeat;

	return 0;
}

LONG CSkinHeaderCtrl::GetButtonBkImageRepeatY() const
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	return pMember->m_sizeButtonBkImageRepeat.cy;
}

int CSkinHeaderCtrl::SetTextColor(COLORREF crN, COLORREF crH, COLORREF crS, COLORREF crD,
	BOOL bRedraw/* = TRUE */)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	pMember->m_crText[0] = crN;
	pMember->m_crText[1] = crH;
	pMember->m_crText[2] = crS;
	pMember->m_crText[3] = crD;

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CSkinHeaderCtrl::GetTextColor(
	COLORREF *pcrN, COLORREF *pcrH, COLORREF *pcrS, COLORREF *pcrD) const
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	if (pcrN != NULL)
	{
		*pcrN = pMember->m_crText[0];
	}
	if (pcrH != NULL)
	{
		*pcrH = pMember->m_crText[1];
	}
	if (pcrS != NULL)
	{
		*pcrS = pMember->m_crText[2];
	}
	if (pcrD != NULL)
	{
		*pcrD = pMember->m_crText[3];
	}

	return 0;
}

int CSkinHeaderCtrl::SetTextFont(UINT uMask, LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDH,
	LPCTSTR lpszFontIDS, LPCTSTR lpszFontIDD, BOOL bRedraw/* = TRUE */)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	IUISetControlFont4(uMask, pMember->m_hIUIFont,
		lpszFontIDN, lpszFontIDH, lpszFontIDS, lpszFontIDD);

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CSkinHeaderCtrl::GetTextFont(UINT uMask, CString *pstrFontIDN, CString *pstrFontIDH,
	CString *pstrFontIDS, CString *pstrFontIDD) const
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

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

int CSkinHeaderCtrl::SetHeaderHeight(int nHeight)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	ASSERT(m_hWnd != NULL);

	pMember->m_nHeaderHeight = nHeight;

	return 0;
}

int CSkinHeaderCtrl::GetHeaderHeight() const
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	return pMember->m_nHeaderHeight;
}

int CSkinHeaderCtrl::EnableHeaderTrack(BOOL bEnable)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	pMember->m_bHeaderTrack = bEnable;

	return 0;
}

BOOL CSkinHeaderCtrl::IsHeaderTrackEnable() const
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	return pMember->m_bHeaderTrack;
}

int CSkinHeaderCtrl::EnableHeaderDragDrop(BOOL bEnable)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	pMember->m_bHeaderDragDrop = bEnable;

	return 0;
}

BOOL CSkinHeaderCtrl::IsHeaderDragDropEnable() const
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	return pMember->m_bHeaderDragDrop;
}

LRESULT CSkinHeaderCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

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

	else if (message == WM_LBUTTONDOWN)
	{
		CPoint point((DWORD)lParam);
		pMember->m_nCapture = HitTest(point);
		UpdateWindow();

		// when the button as a menu bar, click the button more then once,
		// can not accept WM_MOUSELEAVE message after the pop-up menu has been destroy
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = HOVER_DEFAULT;
		::TrackMouseEvent(&tme);
		InvalidateRect(NULL);
	}

	else if (message == WM_LBUTTONUP)
	{
		pMember->m_nCapture = -1;
	}

	else if (message == WM_MOUSEMOVE)
	{
		CPoint point((DWORD)lParam);
		pMember->m_nMouseOver = HitTest(point);

		// when set RGN for highlight stats, show the tool tip when mouse over
		// the button(the tool tip and highlight stats at the same time work).
		// don't call Invalidate() after this "if" section
		Invalidate();

		if (pMember->m_nCapture == -1)
		{
			// when the button as a menu bar, can not accept WM_MOUSELEAVE
			// message after the pop-up menu has been destroy
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = m_hWnd;
			tme.dwHoverTime = HOVER_DEFAULT;
			::TrackMouseEvent(&tme);

			// move this function call to "if" section for showing tool
			// tip when the highlight mask bitmap is set.
			// InvalidateRect(NULL);
		}
	}

	else if (message == WM_MOUSELEAVE)
	{
		pMember->m_nMouseOver = -1;
		InvalidateRect(NULL);
	}

	// handle the HDM_LAYOUT and WM_WINDOWPOSCHANGING message
	// to set the Height of Header within CListCtrl.
	else if (message == HDM_LAYOUT)
	{
		HDLAYOUT *pLayout = (HDLAYOUT *)lParam;

		// the top of header from its parent top
		pLayout->prc->top = pMember->m_nHeaderHeight - pMember->m_nHeaderOrgHeight;
	}
	else if (message == WM_WINDOWPOSCHANGING)
	{
		WINDOWPOS *pPos = (WINDOWPOS *)lParam;
		pPos->y = 0;

		// if the header's parent is List control and have LVS_NOCOLUMNHEADER style,
		// DO NOT set pPos->cy value,
		// otherwise, set pPos->cy to pMember->m_nHeaderHeight.
		BOOL bSet = TRUE;
		CWnd *pParent = GetParent();
		TCHAR szClass[MAX_PATH] = {0};
		GetClassName(pParent->GetSafeHwnd(), szClass, MAX_PATH - 1);
		int nRet = _tcscmp(szClass, _T("SysListView32"));
		if (nRet == 0)
		{
			DWORD dwStyle = ((CListCtrl *)pParent)->GetStyle();
			if ((dwStyle & LVS_NOCOLUMNHEADER) != 0)
			{
				bSet = FALSE;
			}
		}
		if (bSet)
		{
			pPos->cy = pMember->m_nHeaderHeight;
		}
	}
	else if (message == WM_DESTROY)
	{
		if (m_pMember != NULL)
		{
			pMember->Release();

			delete (HEADERMEMBER *)m_pMember;
			m_pMember = NULL;
		}
	}
	else if (message == WM_ERASEBKGND)
	{
		return TRUE;
	}
	else if (message == WM_SETCURSOR)
	{
		if (!pMember->m_bHeaderTrack)
		{
			return TRUE;	// return TRUE to refuse change cursor.
		}
	}

	return CHeaderCtrl::WindowProc(message, wParam, lParam);
}

BOOL CSkinHeaderCtrl::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT *pLResult)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	int nID = GetDlgCtrlID();

	if (wParam == nID && message == WM_NOTIFY)
	{
		NMHDR *pnmhdr = (NMHDR *)lParam;

		// Lock header item track.
		if (pnmhdr->code == HDN_BEGINTRACKA
			|| pnmhdr->code == HDN_BEGINTRACKW
			|| pnmhdr->code == HDN_DIVIDERDBLCLICKA
			|| pnmhdr->code == HDN_DIVIDERDBLCLICKW)
		{
			CHeaderCtrl::OnChildNotify(message, wParam, lParam, pLResult);

			if (pLResult != NULL)
			{
				if (pMember->m_bHeaderTrack)
				{
					*pLResult = 0;
				}
				else
				{
					*pLResult = 1;
				}
			}

			return !pMember->m_bHeaderTrack;
		}
		// Lock header item drag.
		else if (pnmhdr->code == HDN_BEGINDRAG)
		{
			CHeaderCtrl::OnChildNotify(message, wParam, lParam, pLResult);

			if (pLResult != NULL)
			{
				if (pMember->m_bHeaderDragDrop)
				{
					*pLResult = 0;
				}
				else
				{
					*pLResult = 1;
				}
			}

			// To allow the header control to automatically manage drag-and-drop operations,
			// return FALSE. If the owner of the control is manually
			// performing drag-and-drop reordering, return TRUE.
			return !pMember->m_bHeaderDragDrop;
		}
	}

	return CHeaderCtrl::OnChildNotify(message, wParam, lParam, pLResult);
}

int CSkinHeaderCtrl::HitTest(CPoint pt)
{
	HEADERMEMBER *pMember = (HEADERMEMBER *)m_pMember;

	HDHITTESTINFO hdHitTestInfo;
	hdHitTestInfo.pt = pt;
	SendMessage(HDM_HITTEST, 0, LPARAM(&hdHitTestInfo));

	if (hdHitTestInfo.flags == HHT_ONDIVIDER)
	{
		return -1;
	}

	int nCount = GetItemCount();
	for (int i = 0; i < nCount; ++i)
	{
		CRect rect;
		GetItemRect(i, rect);

		if (rect.PtInRect(pt))
		{
			return i;
		}
	}

	return -1;
}
