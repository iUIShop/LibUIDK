// skincombobox.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


struct COMBOBOXMEMBER
{
	COMBOBOXMEMBER()
	{
		int i = 0;
		for (i = 0; i < 4; ++i)
		{
			m_himgBk[i] = NULL;
		}
		m_hIUIFont = NULL;

		for (i = 0; i < 4; ++i)
		{
			m_eTextHorAlignMode[i] = TAH_DEFAULT;
			m_eTextVerAlignMode[i] = TAV_DEFAULT;
			m_rcTextMargin[i].SetRect(1, 1, 1, 1);
		}

		m_clrText = RGB(0, 0, 0);
		m_crListText = RGB(0, 0, 0);

		m_nBtnWidth = 16;
		m_nBorderWidth = 3;
		m_bMouseOver = FALSE;
		m_bDrawFocusRectangle = FALSE;
	}

	int Release()
	{
		int i = 0;
		for (i = 0; i < 4; ++i)
		{
			ReleaseIUIImage(m_himgBk[i]);
			m_himgBk[i] = NULL;
		}
		ReleaseIUIFontInternal(m_hIUIFont);
		m_hIUIFont = NULL;

		for (i = 0; i < 4; ++i)
		{
			m_eTextHorAlignMode[i] = TAH_DEFAULT;
			m_eTextVerAlignMode[i] = TAV_DEFAULT;
			m_rcTextMargin[i].SetRect(1, 1, 1, 1);
		}

		m_clrText = RGB(0, 0, 0);
		m_crListText = RGB(0, 0, 0);

		m_nBtnWidth = 16;
		m_nBorderWidth = 3;
		m_bMouseOver = FALSE;
		m_bDrawFocusRectangle = FALSE;

		return 0;
	}

	HIUIIMAGE m_himgBk[4];

	TEXT_ALIGN_HOR m_eTextHorAlignMode[4];
	TEXT_ALIGN_VER m_eTextVerAlignMode[4];
	CRect m_rcTextMargin[4];

	HIUIFONT m_hIUIFont;
	COLORREF m_clrText;		// text color
	COLORREF m_crListText;	// drop-down list text color
	int m_nBtnWidth; // Width of the drop button with drop down list style
	int m_nBorderWidth;
	BOOL m_bMouseOver;
	CString m_strTag;
	BOOL m_bDrawFocusRectangle;
};

/////////////////////////////////////////////////////////////////////////////
// CSkinComboBox

CSkinComboBox::CSkinComboBox()
{
	m_pBindWnd = this;

	m_pMember = new COMBOBOXMEMBER;
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;
}

CSkinComboBox::~CSkinComboBox()
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	if (m_ctrlEdit.GetSafeHwnd() != NULL)
	{
		m_ctrlEdit.UnsubclassWindow();
	}

	if (m_pMember != NULL)
	{
		delete (COMBOBOXMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

int CSkinComboBox::BindStyle(LPCTSTR lpszStyleID)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_COMBOBOX, lpszStyleID);
	return BindStyle(pCtrlProp);

	return 0;
}

int CSkinComboBox::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

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
	if (ect != CT_STYLE_COMBOBOX && ect != CT_COMBOBOX)
	{
		return -3;
	}

	CMBPROPERTIES *pCmbProp = (CMBPROPERTIES *)pCtrlProp;

	// 1. font
	CString strResFontID;
	CTRLPROPERTIES::IUIGetControlFont1(pCtrlProp, &strResFontID);
	SetTextFont(strResFontID);

	// set size of the combo box
	int nBtnWidth = pCmbProp->m_lBtnWidth;
	SetButtonWidth(nBtnWidth);

	// 2. Must be call after calling SetTextFont(SetFont) if call. Otherwise SetFont modify the height.
	CRect rect = pCmbProp->m_rcControl;
	int nHeight = GetDPIHeight(rect.Height());
	SetComboBoxHeight(nHeight);
	int lDropListItemHeight = GetDPIHeight((LONG)pCmbProp->m_lDropListItemHeight);
	SetItemHeight(0, lDropListItemHeight);

	// 3. Must be call after calling SetSelectionFieldHeight if call
	if (pCmbProp->m_bSpecifyBackgroundImages)
	{
		// 先设置缩放参数，因为ComboBox给里面的Edit设置背景图的时候，需要这些参数
		SetBkImageResizeMode(pCtrlProp->m_eBkImageResizeMode);
		SetBkImageResizePoint(pCtrlProp->m_ptImageResize);
		SetBkImageRepeatX(pCtrlProp->m_uXRepeatPixel);
		SetBkImageRepeatY(pCtrlProp->m_uYRepeatPixel);

		CString strImageName[1 + COMBINEIMAGESIZE4];
		BOOL bCombineImage = TRUE;
		CTRLPROPERTIES::IUIGetBackground4(pCtrlProp, &bCombineImage, strImageName);
		if (bCombineImage)
		{
			SetBkCombineImage(strImageName[0]);
		}
		else
		{
			SetBitmap(CONTROL_STATE_UNCHECKED_ALL,
				strImageName[1], strImageName[2], strImageName[3], strImageName[4]);
		}
		for (int i = 0; i < 1 + COMBINEIMAGESIZE4; ++i)
		{
			ReleaseIUIImage(strImageName[i]);
		}
	}

	// Text color
	COLORREF crN = RGB(0, 0, 0);
	CTRLPROPERTIES::IUIGetControlColor4(pCmbProp, &crN, NULL, NULL, NULL);
	SetTextColor(crN);

	// Text horizontal alignment
	SetTextAlignHor(
		pCmbProp->m_eTextHorAlignModeN,
		pCmbProp->m_eTextHorAlignModeH,
		pCmbProp->m_eTextHorAlignModeS,
		pCmbProp->m_eTextHorAlignModeD);

	// Text horizontal alignment
	SetTextAlignVer(
		pCmbProp->m_eTextVerAlignModeN,
		pCmbProp->m_eTextVerAlignModeH,
		pCmbProp->m_eTextVerAlignModeS,
		pCmbProp->m_eTextVerAlignModeD);

	// Text Margin
	RECT rcTextMarginN = (RECT)pCmbProp->m_rcTextMarginN;
	RECT rcTextMarginH = (RECT)pCmbProp->m_rcTextMarginH;
	RECT rcTextMarginS = (RECT)pCmbProp->m_rcTextMarginS;
	RECT rcTextMarginD = (RECT)pCmbProp->m_rcTextMarginD;
	SetTextMargin(&rcTextMarginN, &rcTextMarginH, &rcTextMarginS, &rcTextMarginD);

	SetTag(pCmbProp->m_strTag);

	return 0;
}

int CSkinComboBox::ReleaseObject()
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	pMember->Release();

	return CControlBase::ReleaseObject();
}

BEGIN_MESSAGE_MAP(CSkinComboBox, CComboBox)
	//{{AFX_MSG_MAP(CSkinComboBox)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// protected

int CSkinComboBox::OnDrawBackground(CDC *pdcMem, int nStateIndex)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	CRect rcWin;
	GetWindowRect(rcWin);
	GetParent()->ScreenToClient(rcWin);

	// Draw the parent background to control for alpha blend with background of the control.
	BOOL bUpdateLayeredWindow = FALSE;
	DrawParentPart(this, pdcMem, rcWin, &bUpdateLayeredWindow, NULL);

	if (m_bBkCombine)
	{
		if (m_himgCombineBk != NULL)
		{
			DrawCombineBackground(
				pdcMem,
				CRect(0, 0, rcWin.Width(), rcWin.Height()),
				COMBINEIMAGESIZE4,
				nStateIndex);
		}
	}
	else
	{
		IUIDrawImage(pdcMem->GetSafeHdc(), CRect(0, 0, rcWin.Width(), rcWin.Height()),
			pMember->m_himgBk[nStateIndex],
			m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CSkinComboBox message handlers

int CSkinComboBox::SetBkCombineImage(LPCTSTR strCombineImage, BOOL bRedraw/* = TRUE*/)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	CControlBase::SetBkCombineImage(strCombineImage, bRedraw);

	// 在ComboBox控件创建过程中，子Edit控件被创建出来后，会被移动多次。
	// 所以，在设置ComboBox背景图的时候，不设置Edit的背景图
	// 采用的策略是绘制Edit的时候，直接使用ComboBox的背景图。
	return 0;
}

int CSkinComboBox::SetBitmap(UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH, LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	m_bBkCombine = false;
	IUISetControlImage4(uMask, pMember->m_himgBk,
		lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);

	// reset background bitmap for edit
	if (m_ctrlEdit.GetSafeHwnd() != NULL)
	{
		CRect rcWin;
		GetWindowRect(rcWin);
		GetParent()->ScreenToClient(rcWin);

		// when Build to Release mode, the rcEdit.Width() is 0, so use (rcWin.Width()-rcEdit.left-pMember->m_nBtnWidth) as the width.
		CRect rcEdit;
		m_ctrlEdit.GetWindowRect(rcEdit);
		ScreenToClient(rcEdit);
		if (rcEdit.Width() == 0)
		{
			rcEdit.right = rcEdit.left + (rcWin.Width() - rcEdit.left - pMember->m_nBtnWidth);
		}

		// 1. Stretch the background of combo box
		HIUIIMAGE hbmpComboStretchN = NULL;
		HIUIIMAGE hbmpComboStretchH = NULL;
		HIUIIMAGE hbmpComboStretchS = NULL;
		HIUIIMAGE hbmpComboStretchD = NULL;
		CIUIImage::NineGridBitmap(pMember->m_himgBk[0],
			m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
			&hbmpComboStretchN, rcWin.Width(), rcWin.Height());
		CIUIImage::NineGridBitmap(pMember->m_himgBk[1],
			m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
			&hbmpComboStretchH, rcWin.Width(), rcWin.Height());
		CIUIImage::NineGridBitmap(pMember->m_himgBk[2],
			m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
			&hbmpComboStretchS, rcWin.Width(), rcWin.Height());
		CIUIImage::NineGridBitmap(pMember->m_himgBk[3],
			m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
			&hbmpComboStretchD, rcWin.Width(), rcWin.Height());

		// 2. then get sub bitmap for edit control.
		HIUIIMAGE hbmpEdtN = NULL;
		HIUIIMAGE hbmpEdtH = NULL;
		HIUIIMAGE hbmpEdtS = NULL;
		HIUIIMAGE hbmpEdtD = NULL;
		CIUIImage::SubBitmap(hbmpComboStretchN, &hbmpEdtN, rcEdit);
		CIUIImage::SubBitmap(hbmpComboStretchH, &hbmpEdtH, rcEdit);
		CIUIImage::SubBitmap(hbmpComboStretchS, &hbmpEdtS, rcEdit);
		CIUIImage::SubBitmap(hbmpComboStretchD, &hbmpEdtD, rcEdit);

		ReleaseIUIImage(hbmpComboStretchN);
		hbmpComboStretchN = NULL;
		ReleaseIUIImage(hbmpComboStretchH);
		hbmpComboStretchH = NULL;
		ReleaseIUIImage(hbmpComboStretchS);
		hbmpComboStretchS = NULL;
		ReleaseIUIImage(hbmpComboStretchD);
		hbmpComboStretchD = NULL;

		if (hbmpEdtN != NULL && hbmpEdtH != NULL && hbmpEdtS != NULL && hbmpEdtD != NULL)
		{
			m_ctrlEdit.SetImages(uMask, hbmpEdtN->GetSafeImageName(), hbmpEdtH->GetSafeImageName(),
				hbmpEdtS->GetSafeImageName(), hbmpEdtD->GetSafeImageName(), TRUE);

			POINT pt;
			GetBkImageResizePoint(&pt);
			pt.x = pt.x - rcEdit.left;
			pt.y = pt.y - rcEdit.top;
			SetBkImageResizePoint(pt);
		}
	}

	return 0;
}

int CSkinComboBox::GetBitmap(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameH, CString *pstrImageNameS, CString *pstrImageNameD) const
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bBkCombine;
	return IUIGetControlImage4(uMask, m_himgCombineBk, pMember->m_himgBk, *pbCombineImage,
			pstrCombineImageName,
			pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);
}

// text's font
int CSkinComboBox::SetTextFont(LPCTSTR lpszFontIDN)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	ASSERT(::IsWindow(m_hWnd));

	IUISetControlFont(&pMember->m_hIUIFont, lpszFontIDN);

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

int CSkinComboBox::GetTextFont(CString *pstrFontIDN) const
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	if (pstrFontIDN == NULL)
	{
		return -1;
	}

	GetFontResID(pMember->m_hIUIFont, pstrFontIDN);

	return 0;
}

BOOL CSkinComboBox::SetTextColor(COLORREF clrText)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	pMember->m_clrText = clrText;

	return TRUE;
}

COLORREF CSkinComboBox::GetTextColor() const
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	return pMember->m_clrText;
}

BOOL CSkinComboBox::SetDropdownListTextColor(COLORREF crText)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	pMember->m_crListText = crText;

	return TRUE;
}

COLORREF CSkinComboBox::GetDropdownListTextColor() const
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	return pMember->m_crListText;
}

int _LayoutEditContrl(HWND hEdit, HWND hComboBox, int nBtnWidth, LPCRECT lprcEditMargin)
{
	if (lprcEditMargin == NULL)
	{
		return -1;
	}

	CRect rcMargin = lprcEditMargin;

	if (::IsWindow(hEdit))
	{
		LONG lStyle = GetWindowLong(hComboBox, GWL_STYLE);
		if ((lStyle & CBS_DROPDOWN) == CBS_DROPDOWN)	// With edit control
		{
			CRect rcWnd;
			GetWindowRect(hComboBox, rcWnd);

			CRect rcEdit;
			GetWindowRect(hEdit, rcEdit);

			IUIScreenToClient(hComboBox, rcEdit);

			int cxBorder = GetSystemMetrics(SM_CXBORDER);
			int cyBorder = GetSystemMetrics(SM_CYBORDER);
			int nEditWidth = rcWnd.Width() - rcEdit.left - nBtnWidth - rcMargin.left - cxBorder;
			int nEditHeight = rcWnd.Height() - cyBorder * 2 - rcMargin.top - rcMargin.bottom;
			SetWindowPos(hEdit, NULL, rcMargin.left - cxBorder, rcMargin.top + cyBorder,
				nEditWidth, nEditHeight, SWP_NOZORDER);
		}
	}

	return 0;
}

int CSkinComboBox::SetTextAlignHor(
	TEXT_ALIGN_HOR eHorAlignModeN,
	TEXT_ALIGN_HOR eHorAlignModeH,
	TEXT_ALIGN_HOR eHorAlignModeS,
	TEXT_ALIGN_HOR eHorAlignModeD,
	BOOL bRedraw/* = TRUE*/)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	pMember->m_eTextHorAlignMode[0] = eHorAlignModeN;
	pMember->m_eTextHorAlignMode[1] = eHorAlignModeH;
	pMember->m_eTextHorAlignMode[2] = eHorAlignModeS;
	pMember->m_eTextHorAlignMode[3] = eHorAlignModeD;

	if (bRedraw)
	{
		Redraw(this);
	}

	return 0;
}

int CSkinComboBox::GetTextAlignHor(
	TEXT_ALIGN_HOR *peHorAlignModeN,
	TEXT_ALIGN_HOR *peHorAlignModeH,
	TEXT_ALIGN_HOR *peHorAlignModeS,
	TEXT_ALIGN_HOR *peHorAlignModeD)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	if (peHorAlignModeN != NULL)
	{
		*peHorAlignModeN = pMember->m_eTextHorAlignMode[0];
	}
	if (peHorAlignModeH != NULL)
	{
		*peHorAlignModeH = pMember->m_eTextHorAlignMode[1];
	}
	if (peHorAlignModeS != NULL)
	{
		*peHorAlignModeS = pMember->m_eTextHorAlignMode[2];
	}
	if (peHorAlignModeD != NULL)
	{
		*peHorAlignModeD = pMember->m_eTextHorAlignMode[3];
	}

	return 0;
}

int CSkinComboBox::SetTextAlignVer(
	TEXT_ALIGN_VER eVerAlignModeN,
	TEXT_ALIGN_VER eVerAlignModeH,
	TEXT_ALIGN_VER eVerAlignModeS,
	TEXT_ALIGN_VER eVerAlignModeD,
	BOOL bRedraw/* = TRUE*/)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	pMember->m_eTextVerAlignMode[0] = eVerAlignModeN;
	pMember->m_eTextVerAlignMode[1] = eVerAlignModeH;
	pMember->m_eTextVerAlignMode[2] = eVerAlignModeS;
	pMember->m_eTextVerAlignMode[3] = eVerAlignModeD;

	if (bRedraw)
	{
		Redraw(this);
	}

	return 0;
}

int CSkinComboBox::GetTextAlignVer(
	TEXT_ALIGN_VER *peVerAlignModeN,
	TEXT_ALIGN_VER *peVerAlignModeH,
	TEXT_ALIGN_VER *peVerAlignModeS,
	TEXT_ALIGN_VER *peVerAlignModeD)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	if (peVerAlignModeN != NULL)
	{
		*peVerAlignModeN = pMember->m_eTextVerAlignMode[0];
	}
	if (peVerAlignModeH != NULL)
	{
		*peVerAlignModeH = pMember->m_eTextVerAlignMode[1];
	}
	if (peVerAlignModeS != NULL)
	{
		*peVerAlignModeS = pMember->m_eTextVerAlignMode[2];
	}
	if (peVerAlignModeD != NULL)
	{
		*peVerAlignModeD = pMember->m_eTextVerAlignMode[3];
	}

	return 0;
}

int CSkinComboBox::SetTextMargin(
	LPCRECT lprcTextMarginN,
	LPCRECT lprcTextMarginH,
	LPCRECT lprcTextMarginS,
	LPCRECT lprcTextMarginD,
	BOOL bRedraw/* = TRUE*/)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	pMember->m_rcTextMargin[0] = *lprcTextMarginN;
	pMember->m_rcTextMargin[1] = *lprcTextMarginH;
	pMember->m_rcTextMargin[2] = *lprcTextMarginS;
	pMember->m_rcTextMargin[3] = *lprcTextMarginD;

	_LayoutEditContrl(m_ctrlEdit.GetSafeHwnd(), m_hWnd, pMember->m_nBtnWidth,
		pMember->m_rcTextMargin[0]);

	if (bRedraw)
	{
		Redraw(this);
	}

	return 0;
}

int CSkinComboBox::GetTextMargin(
	LPRECT lprcTextMarginN,
	LPRECT lprcTextMarginH,
	LPRECT lprcTextMarginS,
	LPRECT lprcTextMarginD) const
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	if (lprcTextMarginN != NULL)
	{
		*lprcTextMarginN = pMember->m_rcTextMargin[0];
	}
	if (lprcTextMarginH != NULL)
	{
		*lprcTextMarginH = pMember->m_rcTextMargin[1];
	}
	if (lprcTextMarginS != NULL)
	{
		*lprcTextMarginS = pMember->m_rcTextMargin[2];
	}
	if (lprcTextMarginD != NULL)
	{
		*lprcTextMarginD = pMember->m_rcTextMargin[3];
	}

	return 0;
}

int CSkinComboBox::SetButtonWidth(int nWidth)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	pMember->m_nBtnWidth = nWidth;

	if (m_hWnd != NULL && m_ctrlEdit.GetSafeHwnd() != NULL)
	{
		CRect rcWnd;
		GetWindowRect(rcWnd);
		GetParent()->ScreenToClient(rcWnd);

		CRect rcEdit;
		m_ctrlEdit.GetWindowRect(rcEdit);
		ScreenToClient(rcEdit);
		int nEditWidth = rcWnd.Width() - rcEdit.left - pMember->m_nBtnWidth;
		m_ctrlEdit.SetWindowPos(NULL, 0, 0, nEditWidth, rcEdit.Height(), SWP_NOMOVE | SWP_NOZORDER);

		m_ctrlEdit.GetWindowRect(rcEdit);
		int n = 0;
	}

	return 0;
}

int CSkinComboBox::GetButtonWidth(int *pnWidth) const
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	if (pnWidth != NULL)
	{
		*pnWidth = pMember->m_nBtnWidth;
	}

	return 0;
}

// GetClientRect equal to GetWindowRect for Combo box. The combo box's height is the selection field height add combo box's border.
// The selection field is edit control if combo box with dropdown style or static control if with drop down list style.
int CSkinComboBox::SetComboBoxHeight(int nHeight)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	ASSERT(::IsWindow(m_hWnd));

	int nRet = (int)::SendMessage(m_hWnd, CB_SETITEMHEIGHT, -1, MAKELONG(nHeight - pMember->m_nBorderWidth * 2, 0));
	if (nRet == CB_ERR)
	{
		return CB_ERR;
	}

	return 0;
}

int CSkinComboBox::GetComboBoxHeight(int *pnHeight) const
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	ASSERT(::IsWindow(m_hWnd));
	if (pnHeight == NULL)
	{
		return -2;
	}

	int nRet = (int)::SendMessage(m_hWnd, CB_GETITEMHEIGHT, -1, 0);
	if (nRet == CB_ERR)
	{
		return CB_ERR;
	}

	*pnHeight = (nRet + pMember->m_nBorderWidth * 2);

	return 0;
}

#if(_MSC_VER <= 1200) // VC6.0

// Copy from CComboBox::SetItemHeight of mfc code in VC2003.
int CSkinComboBox::SetItemHeight(int nIndex, UINT cyItemHeight)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	ASSERT(::IsWindow(m_hWnd));
	return (int)::SendMessage(m_hWnd, CB_SETITEMHEIGHT, nIndex, MAKELONG(cyItemHeight, 0));
}

int CSkinComboBox::GetItemHeight(int nIndex) const
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	ASSERT(::IsWindow(m_hWnd));
	return (int)::SendMessage(m_hWnd, CB_GETITEMHEIGHT, nIndex, 0L);
}

#endif

int CSkinComboBox::SetTag(LPCTSTR lpszTag)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	if (lpszTag == NULL)
	{
		return -1;
	}

	pMember->m_strTag = lpszTag;

	return 0;
}

int CSkinComboBox::GetTag(CString &strTag) const
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	strTag = pMember->m_strTag;

	return 0;
}

int CSkinComboBox::DrawFocusRectangle(BOOL bDraw)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	pMember->m_bDrawFocusRectangle = bDraw;

	return 0;
}

BOOL CSkinComboBox::IsDrawFocusRectangle()
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	return pMember->m_bDrawFocusRectangle;
}

//////////////////////////////////////////////////////////////////////////

void CSkinComboBox::OnCloseup()
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	// TODO: Add your control notification handler code here
	InvalidateRect(NULL);
}

BOOL CSkinComboBox::PreTranslateMessage(MSG *pMsg)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		InvalidateRect(NULL);
	}
	return CComboBox::PreTranslateMessage(pMsg);
}

HBRUSH CSkinComboBox::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	if (nCtlColor == CTLCOLOR_EDIT)
	{
		if (m_ctrlEdit.GetSafeHwnd() == NULL)
		{
			m_ctrlEdit.SubclassWindow(pWnd->GetSafeHwnd());

			int nCX = GetSystemMetrics(SM_CXVSCROLL);
			if (pMember->m_nBtnWidth > nCX)
			{
				m_ctrlEdit.SetDeflateRight(pMember->m_nBtnWidth - nCX);
			}
		}
	}
	if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		// when there is no item within the combo box, not drop down the list box
		LONG lStyle = GetWindowLong(this->m_hWnd, GWL_STYLE);
		int nCount = GetCount();
		// if operator systems is Windows2003 or Windows98 or WindowsMe?, use default behavior
		//  if operator systems is Windows2000, the drop down style can't drop the list, but drop list styly can when there is no items in it.
		BOOL b2000 = IUIIsWindows2000();
		BOOL bXP = IUIIsWindowsXPOrGreater();

		BOOL b2000DropDown = b2000 && ((lStyle & CBS_DROPDOWN) == CBS_DROPDOWN);
		BOOL bNoItems = (nCount <= 0);
		BOOL bNotSimpleStyle = (lStyle & CBS_SIMPLE) != CBS_SIMPLE;

		if (bNoItems && bNotSimpleStyle)
		{
			if (bXP || b2000DropDown)
			{
				// don't show the drop down list box
				SendMessage(WM_KEYDOWN, VK_ESCAPE, 0);
			}
		}
	}

	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SetTextColor(pMember->m_clrText);
	}
	else if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		pDC->SetTextColor(pMember->m_crListText);
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

LRESULT CSkinComboBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	COMBOBOXMEMBER *pMember = (COMBOBOXMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class

	// If the left button down not within the area of the button that contains the drop-down arrow.
	// ___________________________________
	// | abc			|		Add	| ▼ |
	// ___________________________________
	if (message == WM_LBUTTONDOWN && m_ctrlEdit.GetSafeHwnd() != NULL)
	{
		CPoint point(lParam);

		COMBOBOXINFO cmbInfo;
		cmbInfo.cbSize = sizeof(COMBOBOXINFO);
		::GetComboBoxInfo(m_hWnd, &cmbInfo);		// The rcItem field that specifies the coordinates of the edit box is always the old value.

		CRect rcEdit;
		m_ctrlEdit.GetWindowRect(rcEdit);
		ScreenToClient(rcEdit);

		CRect rcAdd;
		GetWindowRect(rcAdd);
		rcAdd.OffsetRect(-rcAdd.left, -rcAdd.top);

		rcAdd.left = rcEdit.right;
		rcAdd.right = cmbInfo.rcButton.left;

		if (rcAdd.PtInRect(point))
		{
			// Determines whether the list box of a combo box is dropped down.
			BOOL bListBoxVisible = (BOOL)SendMessage(CB_GETDROPPEDSTATE);

			// To show or hide the list box of a combo box that has the CBS_DROPDOWN or CBS_DROPDOWNLIST style
			SendMessage(CB_SHOWDROPDOWN, !bListBoxVisible);
		}
	}

	if (message == WM_LBUTTONUP)
	{
		Invalidate();
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
		LRESULT lr = CComboBox::WindowProc(message, wParam, lParam);
		if (lr == -1)
		{
			return -1;
		}

		CWnd *pEdit = GetWindow(GW_CHILD);
		if (pEdit->GetSafeHwnd() != NULL)
		{
			m_ctrlEdit.SubclassWindow(pEdit->m_hWnd);
			int nCX = GetSystemMetrics(SM_CXVSCROLL);
			if (pMember->m_nBtnWidth > nCX)
			{
				m_ctrlEdit.SetDeflateRight(pMember->m_nBtnWidth - nCX);
			}
		}

		return 0;
	}

	if (message == WM_DESTROY)
	{
		CComboBox::SetFont(NULL);

		if (m_ctrlEdit.GetSafeHwnd() != NULL)
		{
			m_ctrlEdit.UnsubclassWindow();
		}
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

			InvalidateRect(NULL);
		}
	}

	if (message == WM_MOUSELEAVE)
	{
		pMember->m_bMouseOver = FALSE;
		InvalidateRect(NULL);
	}

	if (message == WM_MOUSEWHEEL)
	{
		InvalidateRect(NULL);
	}

	if (message == WM_MOUSEACTIVATE)
	{
		InvalidateRect(NULL);
	}

	if (message == WM_GETDLGCODE)
	{
		InvalidateRect(NULL);
	}

	if (message == WM_KILLFOCUS)
	{
		BOOL bVisible = IsWindowVisible();
		if (bVisible)
		{
			SetRedraw(FALSE);
		}
		LRESULT lr = CComboBox::WindowProc(message, wParam, lParam);

		// TODO: Add your message handler code here
		if (bVisible)
		{
			SetRedraw(TRUE);
			Invalidate();
			UpdateWindow();
		}
		return lr;
	}

	// draw the edit control and drop-down button.
	if (message == WM_PAINT)
	{
		CPaintDC dc(this); // device context for painting

		// TODO: Add your message handler code here
		CRect rcWin;
		GetWindowRect(rcWin);
		GetParent()->ScreenToClient(rcWin);

		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc); // the size of memDC is same as it's parent
		CBitmap bmpBK;
		bmpBK.CreateCompatibleBitmap(&dc, rcWin.Width(), rcWin.Height());
		ASSERT(bmpBK.GetSafeHandle() != NULL);
		CBitmap *pBmpBKOld = dcMem.SelectObject(&bmpBK);

		int nStateIndex = 0;
		COMBOBOXINFO ci;
		ci.cbSize = sizeof(COMBOBOXINFO);
		::GetComboBoxInfo(m_hWnd, &ci);
		if (::IsWindow(ci.hwndList) && ::IsWindowVisible(ci.hwndList))
		{
			nStateIndex = 2;
		}
		else
		{
			BOOL bSelected = (GetKeyState(VK_LBUTTON) < 0);
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
		}

		LONG lStyle = GetWindowLong(this->m_hWnd, GWL_STYLE);
		if ((lStyle & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST)	// Read-only
		{
			OnDrawBackground(&dcMem, nStateIndex);

			// Draw window's text
			dcMem.SetBkMode(TRANSPARENT);
			CString sContent;
			GetWindowText(sContent);
			CRect rcClient;
			GetClientRect(rcClient);
			CRect rcText(rcClient);
			rcText.DeflateRect(pMember->m_rcTextMargin[nStateIndex].left,
				pMember->m_rcTextMargin[nStateIndex].top,
				pMember->m_rcTextMargin[nStateIndex].right + pMember->m_nBtnWidth,
				pMember->m_rcTextMargin[nStateIndex].bottom);
			HFONT hFont = GetHFont(pMember->m_hIUIFont);

			CString strDefaultFont;
			if (hFont == NULL)
			{
				strDefaultFont = CUIMgr::GetDefaultFont();
				hFont = CreateIUIFont(strDefaultFont);
				ReleaseIUIFont(strDefaultFont);
			}

			HFONT hFontOld = (HFONT)SelectObject(dcMem.GetSafeHdc(), hFont);

			if (strDefaultFont.IsEmpty())
			{
				ReleaseIUIFont(strDefaultFont);
			}

			dcMem.SetTextColor(pMember->m_clrText);

			UINT uFormat = DT_SINGLELINE | DT_NOPREFIX;
			if (pMember->m_eTextHorAlignMode[nStateIndex] == TAH_DEFAULT
				|| pMember->m_eTextHorAlignMode[nStateIndex] == TAH_LEFT)
			{
				uFormat |= DT_LEFT;
			}
			else if (pMember->m_eTextHorAlignMode[nStateIndex] == TAH_CENTER)
			{
				uFormat |= DT_CENTER;
			}
			else if (pMember->m_eTextHorAlignMode[nStateIndex] == TAH_RIGHT)
			{
				uFormat |= DT_RIGHT;
			}

			if (pMember->m_eTextVerAlignMode[nStateIndex] == TAV_DEFAULT
				|| pMember->m_eTextVerAlignMode[nStateIndex] == TAV_CENTER)
			{
				uFormat |= DT_VCENTER;
			}
			else if (pMember->m_eTextVerAlignMode[nStateIndex] == TAV_TOP)
			{
				uFormat |= DT_TOP;
			}
			else if (pMember->m_eTextVerAlignMode[nStateIndex] == TAV_BOTTOM)
			{
				uFormat |= DT_BOTTOM;
			}

			dcMem.DrawText(sContent, &rcText, uFormat);
			SelectObject(dcMem.GetSafeHdc(), hFontOld);

			// Draw focus rectangle
			if (pMember->m_bDrawFocusRectangle)
			{
				LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
				if ((lStyle & WS_TABSTOP) && (GetFocus() == this))
				{
					CRect rcFocus;
					GetClientRect(rcFocus);
					rcFocus.DeflateRect(1, 1, pMember->m_nBtnWidth, 2);
					dcMem.DrawFocusRect(rcFocus);
				}
			}

			dc.BitBlt(0, 0, rcWin.Width(), rcWin.Height(), &dcMem, 0, 0, SRCCOPY);
			dcMem.SelectObject(pBmpBKOld);
		}
		else if ((lStyle & CBS_DROPDOWN) == CBS_DROPDOWN)	// Can edit
		{
			OnDrawBackground(&dcMem, nStateIndex);

			dc.BitBlt(0, 0, rcWin.Width(), rcWin.Height(), &dcMem, 0, 0, SRCCOPY);
			dcMem.SelectObject(pBmpBKOld);
		}
		else if ((lStyle & CBS_SIMPLE) == CBS_SIMPLE)
		{
		}
		else
		{
			Default();
		}
	}

	// Adjust the size of child edit
	if (message == WM_SIZE && m_ctrlEdit.GetSafeHwnd() != NULL)
	{
		_LayoutEditContrl(m_ctrlEdit.GetSafeHwnd(), m_hWnd, pMember->m_nBtnWidth,
			pMember->m_rcTextMargin[0]);
	}

	// Adjust the size of child edit
	if (message == CB_SETITEMHEIGHT && m_ctrlEdit.GetSafeHwnd() != NULL)
	{
		// CB_SETITEMHEIGHT message can reset the size of Edit control.
		LRESULT lr = CComboBox::WindowProc(message, wParam, lParam);

		int nIndex = (int)wParam;
		if (nIndex == -1)
		{
			CRect rcEdit;
			m_ctrlEdit.GetWindowRect(rcEdit);
			ScreenToClient(rcEdit);

			CRect rcCombo;
			GetWindowRect(rcCombo);
			int nEditWidth = rcCombo.Width() - rcEdit.left - pMember->m_nBtnWidth;
			m_ctrlEdit.SetWindowPos(NULL, 0, 0, nEditWidth, rcEdit.Height(), SWP_NOMOVE | SWP_NOZORDER);
		}

		return lr;
	}

	if (message == CB_SETCURSEL)
	{
		Invalidate();
	}
	else if (message == CB_RESETCONTENT)
	{
		Invalidate();
	}

	else if (message == WM_GETBKIMGRESIZEPOINT)
	{
		POINT *ppt = (POINT *)lParam;

		if (ppt != NULL)
		{
			*ppt = m_ptBkImageResize;

			return TRUE;
		}
	}

	return CComboBox::WindowProc(message, wParam, lParam);
}
