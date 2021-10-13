// WLText.cpp : implementation file
//

#include "stdafx.h"
#include "ResourceMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


struct WLTXTMEMBER
{
	WLTXTMEMBER()
	{
		int i = 0;
		for (i = 0; i < 2; ++i)
		{
			m_himgBk[i] = NULL;
		}
		m_ptBkImageResize.x = m_ptBkImageResize.y = 3;

		for (i = 0; i < 2; ++i)
		{
			m_himgFg[i] = NULL;
		}
		m_ptFkImageResize.x = m_ptFkImageResize.y = 3;

		m_eForegroundAlignHor = FAH_LEFT;
		m_eForegroundAlignVer = FAV_CENTER;
		m_rcForegroundMargin.SetRect(0, 0, 0, 0);
		m_crForegroundMask = RGB(255, 0, 255);

		for (i = 0; i < 2; ++i)
		{
			m_eHorAlignMode[i] = TAH_LEFT;
			m_eVerAlignMode[i] = TAV_CENTER;
		}

		m_uTextFormat = DT_WORDBREAK;
		m_rcPadding4Text.SetRect(0, 0, 0, 0);

		// for font and color
		for (i = 0; i < 2; ++i)
		{
			m_hIUIFont[i] = NULL;
			m_cr[i] = RGB(0, 0, 0);
		}

		// for shadow text
		m_bShadowText = FALSE;
		m_crTextShadow = RGB(192, 192, 192);
		m_ptTextShadowOffset = CPoint(1, 1);
	}

	int Release()
	{
		int i = 0;
		for (i = 0; i < 2; ++i)
		{
			ReleaseIUIImage(m_himgBk[i]);
			m_himgBk[i] = NULL;
		}
		m_ptBkImageResize.x = m_ptBkImageResize.y = 3;

		for (i = 0; i < 2; ++i)
		{
			ReleaseIUIImage(m_himgFg[i]);
			m_himgFg[i] = NULL;
		}
		m_ptFkImageResize.x = m_ptFkImageResize.y = 3;

		m_eForegroundAlignHor = FAH_LEFT;
		m_eForegroundAlignVer = FAV_CENTER;
		m_rcForegroundMargin.SetRect(0, 0, 0, 0);
		m_crForegroundMask = RGB(255, 0, 255);

		for (i = 0; i < 2; ++i)
		{
			m_eHorAlignMode[i] = TAH_LEFT;
			m_eVerAlignMode[i] = TAV_CENTER;
		}

		m_uTextFormat = DT_WORDBREAK;
		m_rcPadding4Text.SetRect(0, 0, 0, 0);

		// for font and color
		for (i = 0; i < 2; ++i)
		{
			ReleaseIUIFontInternal(m_hIUIFont[i]);
			m_hIUIFont[i] = NULL;
			m_cr[i] = RGB(0, 0, 0);
		}

		// for shadow text
		m_bShadowText = FALSE;
		m_crTextShadow = RGB(192, 192, 192);
		m_ptTextShadowOffset = CPoint(1, 1);

		return 0;
	}

	HIUIIMAGE m_himgBk[2];
	CPoint m_ptBkImageResize;
	HIUIIMAGE m_himgFg[2];
	CPoint m_ptFkImageResize;
	FOREGROUND_ALIGN_HOR m_eForegroundAlignHor;
	FOREGROUND_ALIGN_VER m_eForegroundAlignVer;
	CRect m_rcForegroundMargin;
	COLORREF m_crForegroundMask;

	TEXT_ALIGN_HOR m_eHorAlignMode[2];
	TEXT_ALIGN_VER m_eVerAlignMode[2];
	UINT m_uTextFormat;

	CRect m_rcPadding4Text;

	// for font
	HIUIFONT m_hIUIFont[2];
	// for color
	COLORREF m_cr[2];

	CToolTipCtrl m_wndToolTip;

	// for shadow text
	BOOL m_bShadowText;
	COLORREF m_crTextShadow;
	CPoint m_ptTextShadowOffset;
};

//////////////////////////////////////////////////////////////////////////
// CWLText

CWLText::CWLText()
{
	m_pMember = new WLTXTMEMBER;
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;
}

CWLText::~CWLText()
{
	if (m_pMember != NULL)
	{
		delete (WLTXTMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}


BOOL CWLText::Create(LPCTSTR lpszControlName, DWORD dwStyle, const RECT &rect,
	CWnd *pParentWnd, UINT nID, CWLWnd *pVirtualParent)
{
	BOOL bRet = CRectCtrl::Create(lpszControlName, dwStyle, rect, pParentWnd, nID, pVirtualParent);
	if (!bRet)
	{
		return FALSE;
	}

	return TRUE;
}

int CWLText::BindStyle(LPCTSTR lpszStyleID)
{
	if (!IsCreated())
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
}

int CWLText::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	if (!IsCreated())
	{
		ASSERT(FALSE);
		return -1;
	}

	if (pCtrlProp == NULL)
	{
		return -2;
	}
	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (ect != CT_STYLE_STATIC && ect != CT_WL_TEXT)
	{
		return -3;
	}

	TXTPROPERTIES *pWLTxtProp = (TXTPROPERTIES *)pCtrlProp;

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

		SetBkImageResizeMode(pCtrlProp->m_eBkImageResizeMode);
		SetBkImageResizePoint(pCtrlProp->m_ptImageResize);
	}

	// Set foreground
	if (pWLTxtProp->m_bSpecifyForegroundImages)
	{
		CString strImageName[1 + COMBINEIMAGESIZE2];
		BOOL bCombineImage = TRUE;
		CTRLPROPERTIES::IUIGetStaticForeground2(pWLTxtProp, &bCombineImage, strImageName);
		if (bCombineImage)
		{
			SetFgCombineImage(strImageName[0]);
		}
		else
			SetForegroundBitmap(CONTROL_STATE_CHECKED_NORMAL | CONTROL_STATE_CHECKED_DISABLED,
				strImageName[1], strImageName[2],
				pWLTxtProp->m_rcPadding4Foreground,
				pWLTxtProp->m_eForegroundHorAlignMode,
				pWLTxtProp->m_eForegroundVerAlignMode);
	}

	SetTextMultiline(!(bool)pWLTxtProp->m_bNoWrap);
	SetPathEllipsis(pWLTxtProp->m_bPathEllipsis);
	SetEndEllipsis(pWLTxtProp->m_bEndEllipsis);
	SetTextAlignHor(pWLTxtProp->m_eTextHorAlignMode, pWLTxtProp->m_eTextHorAlignMode);
	SetTextAlignVer(pWLTxtProp->m_eTextVerAlignMode, pWLTxtProp->m_eTextVerAlignMode);

	COLORREF crN, crD;
	CTRLPROPERTIES::IUIGetControlColor4(pWLTxtProp, &crN, NULL, NULL, &crD);
	SetTextColor(crN, crD);

	// font
	CString strResFontID[2];
	CTRLPROPERTIES::IUIGetControlFont2(pCtrlProp, strResFontID);
	SetTextFont(CONTROL_STATE_NORMAL | CONTROL_STATE_DISABLED, strResFontID[0], strResFontID[1]);

	// Use tool tip
	if (pWLTxtProp->m_bUseToolTip)
	{
		SetToolTips(pWLTxtProp->m_strToolTip);
	}

	RECT rcMargin = pWLTxtProp->m_rcPadding4Text;
	SetTextMargin(&rcMargin);

	// Shadow text
	if (pWLTxtProp->m_bShadowText)
	{
		ShadowText(TRUE);
		SetTextShadowColor(pWLTxtProp->m_crTextShadow);
		POINT ptOffset = pWLTxtProp->m_ptTextShadowOffset;
		SetTextShadowOffset(&ptOffset);
	}

	return 0;
}

int CWLText::ReleaseObject()
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	for (int i = 0; i < 2; ++i)
	{
		ReleaseIUIFontInternal(pMember->m_hIUIFont[i]);
		pMember->m_hIUIFont[i] = NULL;

		ReleaseIUIImage(pMember->m_himgBk[i]);
		pMember->m_himgBk[i] = NULL;
	}

	return CControlBase::ReleaseObject();
}

//////////////////////////////////////////////////////////////////////////
// virtual

int CWLText::OnDraw(CDC *pMemDCParent)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	if (!IsWindowVisible())
	{
		return 1;
	}

	if (pMemDCParent == NULL)
	{
		return -1;
	}

	CRect rcWin;
	GetWindowRect(rcWin);
	GetParent()->ScreenToClient(rcWin);
	POINT ptDrawOffset = {0};
	GetDrawOffset(&ptDrawOffset);
	rcWin.OffsetRect(ptDrawOffset);

	// Don't draw this control if it's out of the parent
	CRect rcParent;
	GetParent()->GetClientRect(rcParent);

	CRect rcIntersect;
	rcIntersect.IntersectRect(rcWin, rcParent);
	if (rcIntersect.IsRectEmpty())
	{
		return 0;
	}

	BOOL bEnabled = IsWindowEnabled();

	int nImageState = 0;
	if (!bEnabled)
	{
		nImageState = 1;
	}

	// Draw background image.
	if (m_bBkCombine)
	{
		if (m_eBkImageResizeMode == IRM_9GRID)
		{
			IUIPartNineGridBlt(pMemDCParent->GetSafeHdc(), rcWin, m_himgCombineBk,
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				2, nImageState);
		}
		else if (m_eBkImageResizeMode == IRM_STRETCH
			|| m_eBkImageResizeMode == IRM_STRETCH_HIGH_QUALITY)
		{
			IUIPartStretchBlt(pMemDCParent->GetSafeHdc(), rcWin, m_himgCombineBk, 2, nImageState,
				m_eBkImageResizeMode);
		}
	}
	else
	{
		IUIDrawImage(pMemDCParent->GetSafeHdc(), rcWin, pMember->m_himgBk[nImageState],
			m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
	}

	// Draw foreground image.
	IUIDrawForeground(pMemDCParent->GetSafeHdc(), rcWin, pMember->m_rcForegroundMargin,
		pMember->m_himgFg[nImageState],
		pMember->m_eForegroundAlignHor, pMember->m_eForegroundAlignVer,
		pMember->m_crForegroundMask, 255);

	// Draw window's text to background DC.
	pMemDCParent->SetBkMode(TRANSPARENT);
	CString strText;
	GetWindowText(strText);

	pMember->m_hIUIFont[nImageState]->SafeLoadSavedFont();
	DrawFormatText(pMemDCParent->GetSafeHdc(), strText, strText.GetLength(),
		rcWin, pMember->m_rcPadding4Text,
		pMember->m_eHorAlignMode[nImageState], pMember->m_eVerAlignMode[nImageState],
		pMember->m_uTextFormat, pMember->m_hIUIFont[nImageState]->GetSafeHFont(),
		pMember->m_cr[nImageState],
		CT_WL_TEXT);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

int CWLText::SetBitmap(UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameD, BOOL bRedraw/* = TRUE*/)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	m_bBkCombine = false;
	IUISetControlImage2(uMask, pMember->m_himgBk, lpszImageNameN, lpszImageNameD);

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CWLText::GetBitmap(UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameD)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	if (pstrImageNameN != NULL)
	{
		GetImageFileName(pMember->m_himgBk[0], pstrImageNameN);
	}

	if (pstrImageNameD != NULL)
	{
		GetImageFileName(pMember->m_himgBk[1], pstrImageNameD);
	}

	return 0;
}

void CWLText::SetForegroundBitmap(UINT uMask,
	LPCTSTR lpszImageNameForegroundN, LPCTSTR lpszImageNameForegroundD,
	const CRect &rcForegroundMargin,
	FOREGROUND_ALIGN_HOR eAlignModeHor/* = FAH_LEFT*/,
	FOREGROUND_ALIGN_VER eAlignModeVer/* = FAV_CENTER*/,
	COLORREF crMask/* = RGB(255, 0, 255)*/, BOOL bReDraw/* = TRUE*/)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	IUISetControlImage2(uMask, pMember->m_himgFg,
		lpszImageNameForegroundN, lpszImageNameForegroundD);

	pMember->m_rcForegroundMargin = rcForegroundMargin;
	pMember->m_eForegroundAlignHor = eAlignModeHor;
	pMember->m_eForegroundAlignVer = eAlignModeVer;
	pMember->m_crForegroundMask = crMask;

	if (bReDraw)
	{
		Invalidate();
	}
}

void CWLText::GetForegroundBitmap(UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameD,
	LPRECT lprcForegroundMargin,
	int *pnAlignModeHor, int *pnAlignModeVer, COLORREF *pcrMask)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	if (pstrImageNameN != NULL)
	{
		GetImageFileName(pMember->m_himgFg[0], pstrImageNameN);
	}

	if (pstrImageNameD != NULL)
	{
		GetImageFileName(pMember->m_himgFg[1], pstrImageNameD);
	}

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
		*pcrMask = pMember->m_crForegroundMask;
	}
}

int CWLText::SetTextMultiline(BOOL bMultiline)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

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

	return 0;
}

BOOL CWLText::IsTextMultiline()
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	return IsIncludeFlag(pMember->m_uTextFormat, DT_WORDBREAK);
}

int CWLText::SetPathEllipsis(BOOL bPathEllipsis)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

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

BOOL CWLText::IsPathEllipsis() const
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	return IsIncludeFlag(pMember->m_uTextFormat, DT_PATH_ELLIPSIS);
}

int CWLText::SetEndEllipsis(BOOL bEndEllipsis)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

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

BOOL CWLText::IsEndEllipsis() const
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	return IsIncludeFlag(pMember->m_uTextFormat, DT_END_ELLIPSIS);
}

int CWLText::SetTextAlignHor(TEXT_ALIGN_HOR eHorAlignModeN, TEXT_ALIGN_HOR eHorAlignModeD)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	pMember->m_eHorAlignMode[0] = eHorAlignModeN;
	pMember->m_eHorAlignMode[1] = eHorAlignModeD;

	return 0;
}

int CWLText::GetTextAlignHor(TEXT_ALIGN_HOR *peHorAlignModeN, TEXT_ALIGN_HOR *peHorAlignModeD)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	if (peHorAlignModeN != NULL)
	{
		*peHorAlignModeN = pMember->m_eHorAlignMode[0];
	}
	if (peHorAlignModeD != NULL)
	{
		*peHorAlignModeD = pMember->m_eHorAlignMode[1];
	}

	return 0;
}

int CWLText::SetTextAlignVer(TEXT_ALIGN_VER eVerAlignModeN, TEXT_ALIGN_VER eVerAlignModeD)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	pMember->m_eVerAlignMode[0] = eVerAlignModeN;
	pMember->m_eVerAlignMode[1] = eVerAlignModeD;

	return 0;
}

int CWLText::GetTextAlignVer(TEXT_ALIGN_VER *peVerAlignModeN, TEXT_ALIGN_VER *peVerAlignModeD)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	if (peVerAlignModeN != NULL)
	{
		*peVerAlignModeN = pMember->m_eVerAlignMode[0];
	}
	if (peVerAlignModeD != NULL)
	{
		*peVerAlignModeD = pMember->m_eVerAlignMode[1];
	}

	return 0;
}

int CWLText::SetTextColor(COLORREF crN, COLORREF crD, BOOL bRedraw/* = TRUE*/)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	pMember->m_cr[0] = crN;
	pMember->m_cr[1] = crD;

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CWLText::GetTextColor(COLORREF *pcrN, COLORREF *pcrD)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	if (pcrN != NULL)
	{
		*pcrN = pMember->m_cr[0];
	}
	if (pcrD != NULL)
	{
		*pcrD = pMember->m_cr[1];
	}

	return 0;
}

int CWLText::SetTextFont(UINT uMask, LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDD)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	IUISetControlFont2(uMask, pMember->m_hIUIFont, lpszFontIDN, lpszFontIDD);

	return 0;
}

int CWLText::GetTextFont(UINT uMask, CString *pstrFontIDN, CString *pstrFontIDD)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

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

int CWLText::SetToolTips(LPCTSTR lpszToolTips)
{
	return 0;
}

CToolTipCtrl *CWLText::GetToolTipCtrl()
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	return &pMember->m_wndToolTip;
}

int CWLText::SetTextFont(const LOGFONT *lf)
{
	return 0;
}

int CWLText::GetTextFont(LOGFONT *lf) const
{
	return 0;
}

int CWLText::SetTextMargin(LPCRECT lpRect)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	if (lpRect == NULL)
	{
		return -1;
	}

	pMember->m_rcPadding4Text = *lpRect;

	return 0;
}

int CWLText::GetTextMargin(LPRECT lpRect) const
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	if (lpRect == NULL)
	{
		return -1;
	}

	*lpRect = pMember->m_rcPadding4Text;

	return 0;
}

int CWLText::ShadowText(BOOL bShadow)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	pMember->m_bShadowText = bShadow;

	return 0;
}

BOOL CWLText::IsShadowText() const
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	return pMember->m_bShadowText;
}

int CWLText::SetTextShadowColor(COLORREF crShadow)
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	pMember->m_crTextShadow = crShadow;

	return 0;
}

COLORREF CWLText::GetTextShadowColor() const
{
	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	return pMember->m_crTextShadow;
}

int CWLText::SetTextShadowOffset(LPPOINT lpptOffset)
{
	if (lpptOffset == NULL)
	{
		return -1;
	}

	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	pMember->m_ptTextShadowOffset = *lpptOffset;

	return 0;
}

int CWLText::GetTextShadowOffset(LPPOINT lpptOffset) const
{
	if (lpptOffset == NULL)
	{
		return -1;
	}

	WLTXTMEMBER *pMember = (WLTXTMEMBER *)m_pMember;

	*lpptOffset = pMember->m_ptTextShadowOffset;

	return 0;
}

LRESULT CWLText::WLWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_WLDESTROY)
	{
		ReleaseObject();
	}

	return CRectCtrl::WLWindowProc(message, wParam, lParam);
}
