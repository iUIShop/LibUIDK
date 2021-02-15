// SkinButton.cpp : implementation file
//

#include "stdafx.h"
#include <atlconv.h>
#include "ControlMember.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define TIMER_ANIMATETEXT     1


/////////////////////////////////////////////////////////////////////////////
// CButtonBase

CButtonBase::CButtonBase()
{
	m_pMember = new BUTTONMEMBER;
}

CButtonBase::~CButtonBase()
{
	if (m_pMember != NULL)
	{
		ReleaseObject();

		delete (BUTTONMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

int CButtonBase::BindStyle(LPCTSTR lpszStyleID)
{
	if (!IsControlCreated())
	{
		ASSERT(FALSE);
		return -1;
	}

	// Set properties for check box, radio button or push button.
	CONTROL_TYPE eControlType = GetButtonControlType();
	if (eControlType == CT_CHECK)	// Check Box
	{
		CHKPROPERTIES *pCtrlProp = (CHKPROPERTIES *)CUIMgr::GetStyleItem(STYLET_CHECK, lpszStyleID);
		return BindCheckBoxStyle(pCtrlProp);
	}
	else if (eControlType == CT_RADIO)	// Radio button
	{
		RADPROPERTIES *pCtrlProp = (RADPROPERTIES *)CUIMgr::GetStyleItem(STYLET_RADIO, lpszStyleID);
		return BindRadioButtonStyle(pCtrlProp);
	}
	else	// Push button
	{
		BTNPROPERTIES *pCtrlProp = (BTNPROPERTIES *)CUIMgr::GetStyleItem(STYLET_PUSHBUTTON,
				lpszStyleID);
		return BindPushButtonStyle(pCtrlProp);
	}

	return 0;
}

int CButtonBase::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	if (!IsControlCreated())
	{
		ASSERT(FALSE);
		return -1;
	}

	// Set properties for check box, radio button or push button.
	CONTROL_TYPE eControlType = GetButtonControlType();
	if (eControlType == CT_CHECK)	// Check Box
	{
		return BindCheckBoxStyle((CHKPROPERTIES *)pCtrlProp);
	}
	else if (eControlType == CT_RADIO)	// Radio button
	{
		return BindRadioButtonStyle((RADPROPERTIES *)pCtrlProp);
	}
	else	// Push button
	{
		return BindPushButtonStyle((BTNPROPERTIES *)pCtrlProp);
	}

	return 0;
}

int CButtonBase::BindPushButtonStyle(const BTNPROPERTIES *pCtrlProp)
{
	if (!IsControlCreated())
	{
		ASSERT(FALSE);
		return -1;
	}

	if (pCtrlProp == NULL)
	{
		return -2;
	}

	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;

	if (ect != CT_STYLE_PUSHBUTTON && ect != CT_PUSHBUTTON && ect != CT_WL_BUTTON)
	{
		return -3;
	}

	SetTabStop(pCtrlProp->m_bTabStop);

	// Bind window
	int nID = CUIMgr::StringIDToInt(pCtrlProp->m_strModalDlgID);
	SetBindModalDlgID(nID);

	nID = CUIMgr::StringIDToInt(pCtrlProp->m_strModelessWndID);
	SetBindModelessWndID(nID);

	// Set background bitmaps for control
	m_bDrawDefault = (bool)pCtrlProp->m_bDrawDefault;

	CString strImageName[1 + COMBINEIMAGESIZE4];
	BOOL bCombineImage = TRUE;
	CTRLPROPERTIES::IUIGetBackground4(pCtrlProp, &bCombineImage, strImageName);
	if (bCombineImage)
	{
		SetBkCombineImage(strImageName[0]);
	}
	else
	{
		CButtonBase::SetBitmap(CONTROL_STATE_UNCHECKED_ALL,
			strImageName[1], strImageName[2], strImageName[3], strImageName[4]);
	}
	SetBkImageResizeMode(pCtrlProp->m_eBkImageResizeMode);
	SetBkImageResizePoint(pCtrlProp->m_ptImageResize);
	SetBkImageRepeatX(pCtrlProp->m_uXRepeatPixel);
	SetBkImageRepeatY(pCtrlProp->m_uYRepeatPixel);

	SetBkImageTransparent(pCtrlProp->m_lBkImgTransparent);

	// Set foreground bitmaps for control
	CString strImageNameFg[1 + COMBINEIMAGESIZE4];
	int nRet = CTRLPROPERTIES::IUIGetButtonForeground4(pCtrlProp, &bCombineImage, strImageNameFg);
	if (nRet == 0)
	{
		if (bCombineImage)
		{
			SetFgCombineImage(strImageNameFg[0]);
		}
		else
		{
			SetForegroundBitmap(CONTROL_STATE_UNCHECKED_ALL,
				strImageNameFg[1], strImageNameFg[2], strImageNameFg[3], strImageNameFg[4]);
		}

		CRect rcFgMargin = pCtrlProp->m_rcPadding4Foreground;
		GetDPIPadding(rcFgMargin);
		SetForegroundImageMargin(rcFgMargin);
		SetForegroundImageAlign(pCtrlProp->m_eForegroundHorAlignMode,
			pCtrlProp->m_eForegroundVerAlignMode);
	}

	// Load mask bitmap, and set RGN for this control
	if (m_pBindWnd != NULL)
	{
		HIUIIMAGE hbmp;
		hbmp = CreateIUIImage((LPCTSTR)pCtrlProp->m_strMaskBmp);
		nRet = IUIBackgroundMask(m_pBindWnd, hbmp, RGB(255, 0, 255));
		ReleaseIUIImage(hbmp);
	}

	// Set font
	CString strResFontID[4];
	CTRLPROPERTIES::IUIGetControlFont4(pCtrlProp, strResFontID);
	SetTextFont(CONTROL_STATE_UNCHECKED_ALL,
		strResFontID[0], strResFontID[1], strResFontID[2], strResFontID[3]);

	// Init color
	COLORREF cr[4] = {0};
	CTRLPROPERTIES::IUIGetControlColor4(pCtrlProp, &cr[0], &cr[1], &cr[2], &cr[3]);
	SetTextColor(cr[0], cr[1], cr[2], cr[3]);

	// Init align mode
	SetTextAlignHor(pCtrlProp->m_eTextHorAlignMode, pCtrlProp->m_eTextHorAlignMode,
		pCtrlProp->m_eTextHorAlignMode, pCtrlProp->m_eTextHorAlignMode);
	SetTextAlignVer(pCtrlProp->m_eTextVerAlignMode, pCtrlProp->m_eTextVerAlignMode,
		pCtrlProp->m_eTextVerAlignMode, pCtrlProp->m_eTextVerAlignMode);

	// Init offset
	RECT rcTextMargin = pCtrlProp->m_rcPadding4Text;
	CButtonBase::SetTextMargin(&rcTextMargin, &rcTextMargin, &rcTextMargin, &rcTextMargin);

	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;
	pMember->m_nLeftAlign = rcTextMargin.left;
	pMember->m_nTopAlign = rcTextMargin.top;

	//
	SetEndEllipsis(pCtrlProp->m_bEndEllipsis);
	SetTag(pCtrlProp->m_strTag);

	if (pCtrlProp->m_bUseToolTip)
	{
		SetToolTips(pCtrlProp->m_strToolTip);

		if ((LONG)pCtrlProp->m_lAutoPopDelayTime != GetDoubleClickTime() * 10)
		{
			SetToolTipDelayTime(TTDT_AUTOPOP, pCtrlProp->m_lAutoPopDelayTime);
		}

		if ((LONG)pCtrlProp->m_lInitialDelayTime != 500)
		{
			SetToolTipDelayTime(TTDT_INITIAL, pCtrlProp->m_lInitialDelayTime);
		}

		if ((LONG)pCtrlProp->m_lReshowDelayTime != 100)
		{
			SetToolTipDelayTime(TTDT_RESHOW, pCtrlProp->m_lReshowDelayTime);
		}
	}

	SetPushButtonType(pCtrlProp->m_eBtnType);

	return 0;
}

template <class T>
int BindToggleButtonStyle(T *pButtonCtrl, const TOGGLEBTNPROPERTIES *pCtrlProp)
{
	if (pCtrlProp == NULL)
	{
		return -2;
	}
	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (ect != CT_STYLE_CHECK
		&& ect != CT_CHECK
		&& ect != CT_WL_CHECK
		&& ect != CT_STYLE_RADIO
		&& ect != CT_RADIO
		&& ect != CT_WL_RADIO)
	{
		return -3;
	}

	pButtonCtrl->SetTabStop(pCtrlProp->m_bTabStop);

	// Bind window
	int nID = CUIMgr::StringIDToInt(pCtrlProp->m_strModalDlgID);
	pButtonCtrl->SetBindModalDlgID(nID);

	nID = CUIMgr::StringIDToInt(pCtrlProp->m_strModelessWndID);
	pButtonCtrl->SetBindModelessWndID(nID);

	// Set background
	pButtonCtrl->SetDrawFrameControl((bool)pCtrlProp->m_bDrawDefault);

	CString strImageName[1 + COMBINEIMAGESIZE8];
	BOOL bCombineImage = TRUE;
	CTRLPROPERTIES::IUIGetToggleButtonBackground8(pCtrlProp, &bCombineImage, strImageName);
	if (bCombineImage)
	{
		pButtonCtrl->SetBkCombineImage(strImageName[0]);
	}
	else
	{
		pButtonCtrl->CButtonBase::SetBitmap(CONTROL_STATE_UNCHECKED_ALL,
			strImageName[1], strImageName[2], strImageName[3], strImageName[4]);
		pButtonCtrl->SetCheckedBitmap(CONTROL_STATE_CHECKED_ALL,
			strImageName[5], strImageName[6], strImageName[7], strImageName[8]);
	}
	pButtonCtrl->SetBkImageResizeMode(pCtrlProp->m_eBkImageResizeMode);
	pButtonCtrl->SetBkImageResizePoint(pCtrlProp->m_ptImageResize);
	pButtonCtrl->SetBkImageRepeatX(pCtrlProp->m_uXRepeatPixel);
	pButtonCtrl->SetBkImageRepeatY(pCtrlProp->m_uYRepeatPixel);

	pButtonCtrl->SetBkImageTransparent(pCtrlProp->m_lBkImgTransparent);

	// Set foreground
	CString strImageNameFG[1 + COMBINEIMAGESIZE8];
	BOOL bCombnieFg = TRUE;
	int nRet = CTRLPROPERTIES::IUIGetToggleButtonForeground8(pCtrlProp, &bCombnieFg, strImageNameFG);
	if (nRet == 0)
	{
		if (bCombnieFg)
		{
			pButtonCtrl->SetFgCombineImage(strImageNameFG[0]);
		}
		else
		{
			pButtonCtrl->SetForegroundBitmap(CONTROL_STATE_UNCHECKED_ALL,
				strImageNameFG[1], strImageNameFG[2], strImageNameFG[3], strImageNameFG[4]);
			pButtonCtrl->SetCheckedForegroundBitmap(CONTROL_STATE_CHECKED_ALL,
				strImageNameFG[5], strImageNameFG[6], strImageNameFG[7], strImageNameFG[8]);
		}

		RECT rcFgMargin = pCtrlProp->m_rcPadding4Foreground;
		pButtonCtrl->SetForegroundImageAlign(pCtrlProp->m_eForegroundHorAlignMode,
			pCtrlProp->m_eForegroundVerAlignMode);
		pButtonCtrl->SetCheckedForegroundImageMargin(&rcFgMargin);
		pButtonCtrl->SetCheckedForegroundImageAlign(pCtrlProp->m_eForegroundHorAlignMode,
			pCtrlProp->m_eForegroundVerAlignMode);
	}

	// Load mask bitmap, and set RGN for this control
	CONTROL_TYPE eControlType = pCtrlProp->m_eControlType;
	if (eControlType == CT_PUSHBUTTON
		|| eControlType == CT_CHECK
		|| eControlType == CT_RADIO)
	{
		HIUIIMAGE hbmp = CreateIUIImage((LPCTSTR)pCtrlProp->m_strMaskBmp);
		nRet = IUIBackgroundMask((CSkinButton *)pButtonCtrl, hbmp, RGB(255, 0, 255));
		ReleaseIUIImage(hbmp);
	}

	// Set font
	CString strResFontID[8];
	CTRLPROPERTIES::IUIGetControlFont8(pCtrlProp, strResFontID);

	pButtonCtrl->SetTextFont(CONTROL_STATE_UNCHECKED_ALL,
		strResFontID[0], strResFontID[1], strResFontID[2], strResFontID[3]);

	pButtonCtrl->SetCheckedTextFont(CONTROL_STATE_CHECKED_ALL,
		strResFontID[4], strResFontID[5], strResFontID[6], strResFontID[7]);

	// Init color
	COLORREF cr[4] = {0}, crC[4] = {0};
	CTRLPROPERTIES::IUIGetControlColor8(pCtrlProp, &cr[0], &cr[1], &cr[2], &cr[3],
		&crC[0], &crC[1], &crC[2], &crC[3]);
	pButtonCtrl->SetTextColor(cr[0], cr[1], cr[2], cr[3]);
	pButtonCtrl->SetCheckedTextColor(crC[0], crC[1], crC[2], crC[3]);

	// Init offset
	RECT rcTextMargin = pCtrlProp->m_rcPadding4Text;
	GetDPIPadding(&rcTextMargin);
	pButtonCtrl->CButtonBase::SetTextMargin(&rcTextMargin, &rcTextMargin,
		&rcTextMargin, &rcTextMargin);
	pButtonCtrl->SetCheckedTextMargin(&rcTextMargin, &rcTextMargin,
		&rcTextMargin, &rcTextMargin);

	// Init align mode
	pButtonCtrl->SetTextAlignHor(pCtrlProp->m_eTextHorAlignMode, pCtrlProp->m_eTextHorAlignMode,
		pCtrlProp->m_eTextHorAlignMode, pCtrlProp->m_eTextHorAlignMode);
	pButtonCtrl->SetCheckedTextAlignHor(
		pCtrlProp->m_eTextHorAlignMode, pCtrlProp->m_eTextHorAlignMode,
		pCtrlProp->m_eTextHorAlignMode, pCtrlProp->m_eTextHorAlignMode);

	pButtonCtrl->SetTextAlignVer(pCtrlProp->m_eTextVerAlignMode, pCtrlProp->m_eTextVerAlignMode,
		pCtrlProp->m_eTextVerAlignMode, pCtrlProp->m_eTextVerAlignMode);
	pButtonCtrl->SetCheckedTextAlignVer(
		pCtrlProp->m_eTextVerAlignMode, pCtrlProp->m_eTextVerAlignMode,
		pCtrlProp->m_eTextVerAlignMode, pCtrlProp->m_eTextVerAlignMode);

	if (pCtrlProp->m_bUseToolTip)
	{
		pButtonCtrl->SetToolTips(pCtrlProp->m_strToolTip);

		if ((LONG)pCtrlProp->m_lAutoPopDelayTime != 5000)
		{
			pButtonCtrl->SetToolTipDelayTime(TTDT_AUTOPOP, pCtrlProp->m_lAutoPopDelayTime);
		}

		if ((LONG)pCtrlProp->m_lInitialDelayTime != 500)
		{
			pButtonCtrl->SetToolTipDelayTime(TTDT_INITIAL, pCtrlProp->m_lInitialDelayTime);
		}

		if ((LONG)pCtrlProp->m_lReshowDelayTime != 100)
		{
			pButtonCtrl->SetToolTipDelayTime(TTDT_RESHOW, pCtrlProp->m_lReshowDelayTime);
		}
	}

	pButtonCtrl->SetTag(pCtrlProp->m_strTag);

	return 0;
}

int CButtonBase::BindCheckBoxStyle(const CHKPROPERTIES *pCtrlProp)
{
	if (!IsControlCreated())
	{
		ASSERT(FALSE);
		return -1;
	}
	if (pCtrlProp == NULL)
	{
		return -2;
	}
	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (ect != CT_STYLE_CHECK && ect != CT_CHECK && ect != CT_WL_CHECK)
	{
		return -3;
	}

	SetCheckBoxType(pCtrlProp->m_eChkType);

	return BindToggleButtonStyle(this, pCtrlProp);
}

int CButtonBase::BindRadioButtonStyle(const RADPROPERTIES *pCtrlProp)
{
	if (!IsControlCreated())
	{
		ASSERT(FALSE);
		return -1;
	}
	if (pCtrlProp == NULL)
	{
		return -2;
	}

	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (ect != CT_STYLE_RADIO && ect != CT_RADIO && ect != CT_WL_RADIO)
	{
		return -3;
	}

	int nID = CUIMgr::StringIDToInt(pCtrlProp->m_strBindTaskMgrID);
	SetBindTaskWndMgr(nID);

	nID = CUIMgr::StringIDToInt(pCtrlProp->m_strBindTaskWndID);
	SetBindTaskWindow(nID);

	return BindToggleButtonStyle(this, pCtrlProp);
}

int CButtonBase::ReleaseObject()
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->Release();

	return CControlBase::ReleaseObject();
}

CONTROL_TYPE CButtonBase::GetButtonControlType() const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	return pMember->m_eButtonControlType;
}

//////////////////////////////////////////////////////////////////////////
// protected

int CButtonBase::DrawButtonState(
	const CRect &rcClient,
	CDC *pDC,
	/* Background */
	int nStateIndex,
	HIUIIMAGE himgMask,
	IMAGE_RESIZE_MODE eBkImgResizeMode,
	/* Foreground */
	const CRect &rcForegroundMargin,
	FOREGROUND_ALIGN_HOR eForegroundAlignHor,
	FOREGROUND_ALIGN_VER eForegroundAlignVer,
	COLORREF crFGMask,
	BOOL bChecked)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	BOOL bCheckBox = (GetButtonControlType() == CT_CHECK);
	BOOL bRadioButton = (GetButtonControlType() == CT_RADIO);
	BOOL bCheckOrRadio = bCheckBox || bRadioButton;

	// Draw background

	if (IsDrawFrameControl())
	{
		UINT uState = 0;
		if (bChecked)
		{
			uState |= DFCS_CHECKED;
		}

		if (nStateIndex == 1)
		{
			uState |= DFCS_HOT;
		}
		else if (nStateIndex == 2)
		{
			uState |= DFCS_PUSHED;
		}
		else if (nStateIndex == 3)
		{
			uState |= DFCS_INACTIVE;
		}

		if (bCheckOrRadio)
		{
			if (bCheckBox)
			{
				uState |= DFCS_BUTTONCHECK;
			}
			else if (bRadioButton)
			{
				uState |= DFCS_BUTTONRADIO;
			}
		}
		else
		{
			uState |= DFCS_BUTTONPUSH;
		}

		CRect rect = rcClient;
		DrawFrameControl(pDC->GetSafeHdc(), rect, DFC_BUTTON, uState);
	}

	if (m_bBkCombine)
	{
		if (m_himgCombineBk != NULL)
		{
			DrawCombineBackground(
				pDC,
				rcClient,
				bCheckOrRadio ? COMBINEIMAGESIZE8 : COMBINEIMAGESIZE4,
				bChecked ? nStateIndex + 4 : nStateIndex);
		}
	}
	else
	{
		HIUIIMAGE hImgBk = (bCheckOrRadio && bChecked)
			? pMember->m_himgBkC[nStateIndex]
			: pMember->m_himgBk[nStateIndex];

		if (hImgBk != NULL)
		{
			IUIDrawImage(pDC->GetSafeHdc(), rcClient,
				hImgBk,
				m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
		}
	}

	// Draw foreground
	if (m_bFgCombine)
	{
		if (m_himgCombineFg->GetSafeHBITMAP() == NULL)
		{
			m_himgCombineFg->SafeLoadSavedImage();
		}

		if (m_himgCombineFg->GetSafeHBITMAP() != NULL)
		{
			int nPartWidth = 0;
			if (bCheckOrRadio)
			{
				nPartWidth = m_himgCombineFg->GetWidth() / COMBINEIMAGESIZE8;
			}
			else
			{
				nPartWidth = m_himgCombineFg->GetWidth() / COMBINEIMAGESIZE4;
			}

			CRect rcFgPart;

			if (bCheckOrRadio && bChecked)
			{
				rcFgPart.left = (nStateIndex + COMBINEIMAGESIZE4) * nPartWidth;
			}
			else
			{
				rcFgPart.left = nStateIndex * nPartWidth;
			}

			rcFgPart.right = rcFgPart.left + nPartWidth;
			rcFgPart.top = 0;
			rcFgPart.bottom = m_himgCombineFg->GetHeight();

			IUIPartDrawForeground(pDC->GetSafeHdc(),
				rcClient,
				rcForegroundMargin,
				m_himgCombineFg,
				rcFgPart,
				eForegroundAlignHor,
				eForegroundAlignVer,
				crFGMask,
				m_btFgImgTransparent);
		}
	}
	else
	{
		HIUIIMAGE hImgFg = (bCheckOrRadio && bChecked)
			? pMember->m_hBmpForegroundC[nStateIndex] : pMember->m_hBmpForeground[nStateIndex];

		IUIDrawForeground(pDC->GetSafeHdc(),
			rcClient,
			rcForegroundMargin,
			hImgFg,
			eForegroundAlignHor,
			eForegroundAlignVer,
			crFGMask,
			m_btFgImgTransparent);
	}

	return 0;
}

int CButtonBase::DrawText(CDC *pDC, const CString &strText,
	const CRect &rcControl, const CRect &rcTextMargin,
	TEXT_ALIGN_HOR eTextAlignHor, TEXT_ALIGN_VER eTextAlignVer,
	HFONT hFont, COLORREF crText)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	if (!pMember->m_bWLButton)
	{
		NMHDR nmhdr;
		nmhdr.hwndFrom = m_pBindWnd->GetSafeHwnd();
		nmhdr.idFrom = m_pBindWnd->GetDlgCtrlID();
		nmhdr.code = BNN_DRAWTEXT;
		NMBUTTON bn;
		bn.hdr = nmhdr;
		bn.hDC = pDC->GetSafeHdc();
		bn.rc = rcControl;
		bn.uFormat = 0;

		BOOL bRet = (BOOL)m_pBindWnd->GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&bn));
		if (bRet)
		{
			return 0;
		}
	}

	CRect rcText(rcControl);
	rcText.DeflateRect(rcTextMargin);

	UINT uFormat = 0;
	if (pMember->m_bMultiLine)
	{
		uFormat = DT_WORDBREAK;
	}
	else
	{
		uFormat = DT_SINGLELINE;
	}

	// adjust horizontal alignment
	if (eTextAlignHor == TAH_DEFAULT || eTextAlignHor == TAH_CENTER)
	{
		uFormat |= DT_CENTER;
	}
	else if (eTextAlignHor == TAH_LEFT)
	{
		uFormat |= DT_LEFT;
	}
	else if (eTextAlignHor == TAH_RIGHT)
	{
		uFormat |= DT_RIGHT;
	}
	// adjust vertical alignment
	if (eTextAlignVer == TAV_DEFAULT || eTextAlignVer == TAV_CENTER)
	{
		CRect rcTest(rcText);
		if (IsMultiLine())
		{
			int nHeight = pDC->DrawText(strText, rcTest, uFormat | DT_CALCRECT);
			int nAdjustHeight = (rcText.Height() - nHeight) / 2;
			rcText.DeflateRect(0, nAdjustHeight, 0, 0);
		}
		else
		{
			uFormat |= DT_VCENTER;
		}
	}
	else if (eTextAlignVer == TAV_TOP)
	{
		uFormat |= DT_TOP;
	}
	else if (eTextAlignVer == TAV_BOTTOM)
	{
		CRect rcTest(rcText);
		if (pMember->m_bMultiLine)
		{
			int nHeight = pDC->DrawText(strText, rcTest, uFormat | DT_CALCRECT);
			rcText.top += (rcText.Height() - nHeight);
		}
		else
		{
			uFormat |= DT_BOTTOM;
		}
	}

	CRect rcAnimate(0, 0, 0, 0);
	if (pMember->m_bAnimateText)
	{
		rcAnimate = rcControl;
		rcAnimate.DeflateRect(pMember->m_nLeftAlign, pMember->m_nTopAlign, 0, 0);
		if (pMember->m_eAnimateDirection == LEFT)
		{
			uFormat &= ~DT_CENTER;
			uFormat &= ~DT_RIGHT;
		}
		else if (pMember->m_eAnimateDirection == TOP)
		{
			uFormat &= ~DT_VCENTER;
			uFormat &= ~DT_BOTTOM;
		}
		else if (pMember->m_eAnimateDirection == RIGHT)
		{
			uFormat &= ~DT_CENTER;
			uFormat &= ~DT_RIGHT;
		}
		else if (pMember->m_eAnimateDirection == BOTTOM)
		{
			uFormat &= ~DT_VCENTER;
			uFormat &= ~DT_BOTTOM;
		}
	}

	if (IsUpdateLayeredWindow())
	{
		if (pMember->m_bAnimateText)
		{
			pDC->DrawText(strText, rcAnimate, uFormat);

			int nHeight = pDC->DrawText(strText, rcAnimate, uFormat | DT_CALCRECT);
			pMember->m_sizeText.cx = rcAnimate.Width();
			pMember->m_sizeText.cy = rcAnimate.Height();

			BOOL bIntersect = rcAnimate.IntersectRect(rcAnimate, rcControl);
			if (!bIntersect && !pMember->m_bAnimateReset)
			{
				pMember->m_bAnimateReset = TRUE;
			}
		}
		else
		{
			if (pMember->m_bEndEllipsis)
			{
				uFormat |= DT_END_ELLIPSIS;
			}

			Graphics g(pDC->GetSafeHdc());
			DrawTextGp(&g, strText, strText.GetLength(),
				(HFONT)hFont, crText, rcText, uFormat);
		}
	}
	else
	{
		HFONT hOldFont = NULL;
		if (hFont != NULL)
		{
			hOldFont = (HFONT)SelectObject(pDC->GetSafeHdc(), hFont);
		}
		pDC->SetTextColor(crText);

		pDC->SetBkMode(TRANSPARENT);
		if (pMember->m_bAnimateText)
		{
			pDC->DrawText(strText, rcAnimate, uFormat);

			int nHeight = pDC->DrawText(strText, rcAnimate, uFormat | DT_CALCRECT);
			pMember->m_sizeText.cx = rcAnimate.Width();
			pMember->m_sizeText.cy = rcAnimate.Height();

			BOOL bIntersect = rcAnimate.IntersectRect(rcAnimate, rcControl);
			if (!bIntersect && !pMember->m_bAnimateReset)
			{
				pMember->m_bAnimateReset = TRUE;
			}
		}
		else
		{
			if (pMember->m_bEndEllipsis)
			{
				uFormat |= DT_END_ELLIPSIS;
			}
			pDC->DrawText(strText, rcText, uFormat);
		}

		if (hOldFont != NULL)
		{
			SelectObject(pDC->GetSafeHdc(), hOldFont);
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// text's color
int CButtonBase::SetTextColor(
	COLORREF crN,
	COLORREF crH,
	COLORREF crS,
	COLORREF crD,
	BOOL bRedraw/* = TRUE*/)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_cr[0] = crN;
	pMember->m_cr[1] = crH;
	pMember->m_cr[2] = crS;
	pMember->m_cr[3] = crD;

	if (bRedraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CButtonBase::GetTextColor(COLORREF *pcrN, COLORREF *pcrH, COLORREF *pcrS, COLORREF *pcrD) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	if (pcrN != NULL)
	{
		*pcrN = pMember->m_cr[0];
	}
	if (pcrH != NULL)
	{
		*pcrH = pMember->m_cr[1];
	}
	if (pcrS != NULL)
	{
		*pcrS = pMember->m_cr[2];
	}
	if (pcrD != NULL)
	{
		*pcrD = pMember->m_cr[3];
	}

	return 0;
}

int CButtonBase::SetCheckedTextColor(
	COLORREF crCN,
	COLORREF crCH,
	COLORREF crCS,
	COLORREF crCD,
	BOOL bRedraw/* = TRUE*/)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_crC[0] = crCN;
	pMember->m_crC[1] = crCH;
	pMember->m_crC[2] = crCS;
	pMember->m_crC[3] = crCD;

	if (bRedraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CButtonBase::GetCheckedTextColor(
	COLORREF *pcrCN,
	COLORREF *pcrCH,
	COLORREF *pcrCS,
	COLORREF *pcrCD) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	if (pcrCN != NULL)
	{
		*pcrCN = pMember->m_crC[0];
	}
	if (pcrCH != NULL)
	{
		*pcrCH = pMember->m_crC[1];
	}
	if (pcrCS != NULL)
	{
		*pcrCS = pMember->m_crC[2];
	}
	if (pcrCD != NULL)
	{
		*pcrCD = pMember->m_crC[3];
	}

	return 0;
}

// text's font
int CButtonBase::SetTextFont(
	UINT uMask,
	LPCTSTR lpszFontIDN,
	LPCTSTR lpszFontIDH,
	LPCTSTR lpszFontIDS,
	LPCTSTR lpszFontIDD,
	BOOL bRedraw/* = TRUE*/)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	IUISetControlFont4(uMask, pMember->m_hIUIFont,
		lpszFontIDN, lpszFontIDH, lpszFontIDS, lpszFontIDD);

	if (bRedraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CButtonBase::GetTextFont(UINT uMask,
	CString *pstrFontIDN, CString *pstrFontIDH, CString *pstrFontIDS, CString *pstrFontIDD) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

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

int CButtonBase::SetCheckedTextFont(
	UINT uMask,
	LPCTSTR lpszFontIDCN,
	LPCTSTR lpszFontIDCH,
	LPCTSTR lpszFontIDCS,
	LPCTSTR lpszFontIDCD,
	BOOL bRedraw/* = TRUE*/)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	IUISetControlCheckedFont4(uMask, pMember->m_hIUIFontC,
		lpszFontIDCN, lpszFontIDCH, lpszFontIDCS, lpszFontIDCD);

	if (bRedraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CButtonBase::GetCheckedTextFont(
	UINT uMask,
	CString *pstrFontIDCN,
	CString *pstrFontIDCH,
	CString *pstrFontIDCS,
	CString *pstrFontIDCD) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	if (pstrFontIDCN != NULL)
	{
		GetFontResID(pMember->m_hIUIFontC[0], pstrFontIDCN);
	}
	if (pstrFontIDCH != NULL)
	{
		GetFontResID(pMember->m_hIUIFontC[1], pstrFontIDCH);
	}
	if (pstrFontIDCS != NULL)
	{
		GetFontResID(pMember->m_hIUIFontC[2], pstrFontIDCS);
	}
	if (pstrFontIDCD != NULL)
	{
		GetFontResID(pMember->m_hIUIFontC[3], pstrFontIDCD);
	}

	return 0;
}

// text's offset
int CButtonBase::SetTextMargin(
	LPCRECT lprcTextMarginN,
	LPCRECT lprcTextMarginH,
	LPCRECT lprcTextMarginS,
	LPCRECT lprcTextMarginD,
	BOOL bRedraw/* = TRUE*/)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_rcTextMargin[0] = *lprcTextMarginN;
	pMember->m_rcTextMargin[1] = *lprcTextMarginH;
	pMember->m_rcTextMargin[2] = *lprcTextMarginS;
	pMember->m_rcTextMargin[3] = *lprcTextMarginD;

	if (bRedraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CButtonBase::GetTextMargin(
	LPRECT lprcTextMarginN,
	LPRECT lprcTextMarginH,
	LPRECT lprcTextMarginS,
	LPRECT lprcTextMarginD) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

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

int CButtonBase::SetCheckedTextMargin(
	LPCRECT lprcTextMarginCN,
	LPCRECT lprcTextMarginCH,
	LPCRECT lprcTextMarginCS,
	LPCRECT lprcTextMarginCD,
	BOOL bRedraw/* = TRUE*/)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_rcTextMarginC[0] = *lprcTextMarginCN;
	pMember->m_rcTextMarginC[1] = *lprcTextMarginCH;
	pMember->m_rcTextMarginC[2] = *lprcTextMarginCS;
	pMember->m_rcTextMarginC[3] = *lprcTextMarginCD;

	if (bRedraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CButtonBase::GetCheckedTextMargin(
	LPRECT lprcTextMarginCN,
	LPRECT lprcTextMarginCH,
	LPRECT lprcTextMarginCS,
	LPRECT lprcTextMarginCD) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	if (lprcTextMarginCN != NULL)
	{
		*lprcTextMarginCN = pMember->m_rcTextMarginC[0];
	}
	if (lprcTextMarginCH != NULL)
	{
		*lprcTextMarginCH = pMember->m_rcTextMarginC[1];
	}
	if (lprcTextMarginCS != NULL)
	{
		*lprcTextMarginCS = pMember->m_rcTextMarginC[2];
	}
	if (lprcTextMarginCD != NULL)
	{
		*lprcTextMarginCD = pMember->m_rcTextMarginC[3];
	}

	return 0;
}

int CButtonBase::SetBitmap(
	UINT uMask,
	LPCTSTR lpszImageNameN,
	LPCTSTR lpszImageNameH,
	LPCTSTR lpszImageNameS,
	LPCTSTR lpszImageNameD,
	BOOL bRedraw/*=TRUE*/)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	m_bBkCombine = false;
	IUISetControlImage4(uMask, pMember->m_himgBk,
		lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);

	if (bRedraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CButtonBase::GetBitmap(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN,
	CString *pstrImageNameH,
	CString *pstrImageNameS,
	CString *pstrImageNameD) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bBkCombine;
	return IUIGetControlImage4(uMask, m_himgCombineBk, pMember->m_himgBk, *pbCombineImage,
			pstrCombineImageName,
			pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);
}

int CButtonBase::SetForegroundBitmap(
	UINT uMask,
	LPCTSTR lpszImageNameN,
	LPCTSTR lpszImageNameH,
	LPCTSTR lpszImageNameS,
	LPCTSTR lpszImageNameD,
	COLORREF crMask/* = RGB(255, 0, 255)*/,
	BOOL bReDraw/* = TRUE*/)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	m_bFgCombine = false;
	IUISetControlImage4(uMask, pMember->m_hBmpForeground,
		lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);

	pMember->m_crMask = crMask;

	if (bReDraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CButtonBase::GetForegroundBitmap(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN,
	CString *pstrImageNameH,
	CString *pstrImageNameS,
	CString *pstrImageNameD,
	COLORREF *pcrMask) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bFgCombine;
	IUIGetControlImage4(uMask, m_himgCombineFg, pMember->m_hBmpForeground, *pbCombineImage,
		pstrCombineImageName,
		pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);

	if (pcrMask != NULL)
	{
		*pcrMask = pMember->m_crMask;
	}

	return 0;
}

int CButtonBase::SetForegroundIcon(HICON hIcon)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_hIcon = hIcon;

	return 0;
}

HICON CButtonBase::GetForegroundIcon() const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	return pMember->m_hIcon;
}

int CButtonBase::SetForegroundImageMargin(LPCRECT rcForegroundMargin)
{
	if (rcForegroundMargin == NULL)
	{
		return -1;
	}

	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;
	pMember->m_rcForegroundMargin = rcForegroundMargin;

	return 0;
}

int CButtonBase::GetForegroundImageMargin(LPRECT lprcForegroundMargin) const
{
	if (lprcForegroundMargin == NULL)
	{
		return -1;
	}

	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;
	*lprcForegroundMargin = pMember->m_rcForegroundMargin;

	return 0;
}

int CButtonBase::SetForegroundImageAlign(
	FOREGROUND_ALIGN_HOR eAlignModeHor,
	FOREGROUND_ALIGN_VER eAlignModeVer)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_eForegroundAlignHor = eAlignModeHor;
	pMember->m_eForegroundAlignVer = eAlignModeVer;

	Redraw(m_pBindWnd);

	return 0;
}

int CButtonBase::GetForegroundImageAlign(
	FOREGROUND_ALIGN_HOR *peAlignModeHor, FOREGROUND_ALIGN_VER *peAlignModeVer) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	if (peAlignModeHor != NULL)
	{
		*peAlignModeHor = pMember->m_eForegroundAlignHor;
	}
	if (peAlignModeVer != NULL)
	{
		*peAlignModeVer = pMember->m_eForegroundAlignVer;
	}

	return 0;
}

int CButtonBase::SetCheckedForegroundBitmap(
	UINT uMask,
	LPCTSTR lpszImageNameCN,
	LPCTSTR lpszImageNameCH,
	LPCTSTR lpszImageNameCS,
	LPCTSTR lpszImageNameCD,
	COLORREF crMaskC/* = RGB(255, 0, 255)*/,
	BOOL bReDraw/* = TRUE*/)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	m_bFgCombine = false;
	IUISetControlCheckedImage4(uMask, pMember->m_hBmpForegroundC,
		lpszImageNameCN, lpszImageNameCH, lpszImageNameCS, lpszImageNameCD);

	pMember->m_crMaskC = crMaskC;

	if (bReDraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CButtonBase::GetCheckedForegroundBitmap(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameCN,
	CString *pstrImageNameCH,
	CString *pstrImageNameCS,
	CString *pstrImageNameCD,
	COLORREF *pcrMaskC) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bFgCombine;
	return IUIGetControlImage4(uMask, m_himgCombineFg, pMember->m_hBmpForegroundC, *pbCombineImage,
			pstrCombineImageName,
			pstrImageNameCN, pstrImageNameCH, pstrImageNameCS, pstrImageNameCD);

	if (pcrMaskC != NULL)
	{
		*pcrMaskC = pMember->m_crMaskC;
	}

	return 0;
}

int CButtonBase::SetCheckedForegroundIcon(HICON hIcon)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_hCheckedIcon = hIcon;

	return 0;
}

HICON CButtonBase::GetCheckedForegroundIcon() const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	return pMember->m_hCheckedIcon;
}

int CButtonBase::SetCheckedForegroundImageMargin(LPCRECT lprcForegroundMargin)
{
	if (lprcForegroundMargin == NULL)
	{
		return -1;
	}

	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;
	pMember->m_rcForegroundMargin = *lprcForegroundMargin;

	Redraw(m_pBindWnd);

	return 0;
}

int CButtonBase::GetCheckedForegroundImageMargin(LPRECT lprcForegroundMargin) const
{
	if (lprcForegroundMargin == NULL)
	{
		return -1;
	}

	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;
	*lprcForegroundMargin = pMember->m_rcForegroundMargin;

	return 0;
}

int CButtonBase::SetCheckedForegroundImageAlign(
	FOREGROUND_ALIGN_HOR eAlignModeHor,
	FOREGROUND_ALIGN_VER eAlignModeVer)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_eForegroundAlignHorC = eAlignModeHor;
	pMember->m_eForegroundAlignVerC = eAlignModeVer;

	Redraw(m_pBindWnd);

	return 0;
}

int CButtonBase::GetCheckedForegroundImageAlign(FOREGROUND_ALIGN_HOR *peAlignModeHorC,
	FOREGROUND_ALIGN_VER *peAlignModeVerC) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	if (peAlignModeHorC != NULL)
	{
		*peAlignModeHorC = pMember->m_eForegroundAlignHorC;
	}
	if (peAlignModeVerC != NULL)
	{
		*peAlignModeVerC = pMember->m_eForegroundAlignVerC;
	}

	return 0;
}

int CButtonBase::SetCheckedBitmap(
	UINT uMask,
	LPCTSTR lpszImageNameCN,
	LPCTSTR lpszImageNameCH,
	LPCTSTR lpszImageNameCS,
	LPCTSTR lpszImageNameCD,
	BOOL bRedraw/*=TRUE*/)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	m_bBkCombine = false;
	IUISetControlCheckedImage4(uMask, pMember->m_himgBkC,
		lpszImageNameCN, lpszImageNameCH, lpszImageNameCS, lpszImageNameCD);

	if (bRedraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CButtonBase::SetCheckedBitmap(
	UINT uIDCN,
	UINT uIDCH,
	UINT uIDCS,
	UINT uIDCD,
	BOOL bDeleteOld/*=TRUE*/,
	BOOL bRedraw/*=TRUE*/)
{
	return -1; // Not support
	//CIUIImage himgCN;
	//himgCN.Attach(LoadHBITMAPFromRes(uIDCN));
	//CIUIImage himgCH;
	//himgCH.Attach(LoadHBITMAPFromRes(uIDCH));
	//CIUIImage himgCS;
	//himgCS.Attach(LoadHBITMAPFromRes(uIDCS));
	//CIUIImage himgCD;
	//himgCD.Attach(LoadHBITMAPFromRes(uIDCD));
	//return SetCheckedBitmap(CONTROL_STATE_CHECKED_ALL, himgCN, himgCH, himgCS, himgCD, bRedraw);
}

int CButtonBase::GetCheckedBitmap(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameCN,
	CString *pstrImageNameCH,
	CString *pstrImageNameCS,
	CString *pstrImageNameCD) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bFgCombine;
	return IUIGetControlImage4(uMask, m_himgCombineBk, pMember->m_himgBkC, *pbCombineImage,
			pstrCombineImageName,
			pstrImageNameCN, pstrImageNameCH, pstrImageNameCS, pstrImageNameCD);
}

int CButtonBase::SetMaskBitmap(
	UINT uMask,
	LPCTSTR lpszImageNameN,
	LPCTSTR lpszImageNameH,
	LPCTSTR lpszImageNameS,
	LPCTSTR lpszImageNameD,
	BOOL bRedraw/* = TRUE*/)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_bMaskCombine = false;
	IUISetControlImage4(uMask, pMember->m_hbmpMask,
		lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);

	if (bRedraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CButtonBase::GetMaskBitmap(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN,
	CString *pstrImageNameH,
	CString *pstrImageNameS,
	CString *pstrImageNameD) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = pMember->m_bMaskCombine;
	return IUIGetControlImage4(uMask,
			pMember->m_himgMaskCombine, pMember->m_hbmpMask, *pbCombineImage,
			pstrCombineImageName,
			pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);
}

int CButtonBase::SetCheckedMaskBitmap(
	UINT uMask,
	LPCTSTR lpszImageNameCN,
	LPCTSTR lpszImageNameCH,
	LPCTSTR lpszImageNameCS,
	LPCTSTR lpszImageNameCD,
	BOOL bRedraw/*=TRUE*/)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_bMaskCombine = false;
	IUISetControlImage4(uMask, pMember->m_hbmpCheckedMask,
		lpszImageNameCN, lpszImageNameCH, lpszImageNameCS, lpszImageNameCD);

	if (bRedraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CButtonBase::GetCheckedMaskBitmap(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameCN,
	CString *pstrImageNameCH,
	CString *pstrImageNameCS,
	CString *pstrImageNameCD) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = pMember->m_bMaskCombine;
	return IUIGetControlImage4(uMask, m_himgCombineBk, pMember->m_hbmpCheckedMask, *pbCombineImage,
			pstrCombineImageName,
			pstrImageNameCN, pstrImageNameCH, pstrImageNameCS, pstrImageNameCD);
}

int CButtonBase::SetTextAlignHor(
	TEXT_ALIGN_HOR eHorAlignModeN,
	TEXT_ALIGN_HOR eHorAlignModeH,
	TEXT_ALIGN_HOR eHorAlignModeS,
	TEXT_ALIGN_HOR eHorAlignModeD,
	BOOL bRedraw/* = TRUE*/)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_eTextHorAlignMode[0] = eHorAlignModeN;
	pMember->m_eTextHorAlignMode[1] = eHorAlignModeH;
	pMember->m_eTextHorAlignMode[2] = eHorAlignModeS;
	pMember->m_eTextHorAlignMode[3] = eHorAlignModeD;

	if (bRedraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CButtonBase::GetTextAlignHor(
	TEXT_ALIGN_HOR *peHorAlignModeN,
	TEXT_ALIGN_HOR *peHorAlignModeH,
	TEXT_ALIGN_HOR *peHorAlignModeS,
	TEXT_ALIGN_HOR *peHorAlignModeD) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

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

int CButtonBase::SetTextAlignVer(
	TEXT_ALIGN_VER eVerAlignModeN,
	TEXT_ALIGN_VER eVerAlignModeH,
	TEXT_ALIGN_VER eVerAlignModeS,
	TEXT_ALIGN_VER eVerAlignModeD,
	BOOL bRedraw/* = TRUE*/)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_eTextVerAlignMode[0] = eVerAlignModeN;
	pMember->m_eTextVerAlignMode[1] = eVerAlignModeH;
	pMember->m_eTextVerAlignMode[2] = eVerAlignModeS;
	pMember->m_eTextVerAlignMode[3] = eVerAlignModeD;

	if (bRedraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CButtonBase::GetTextAlignVer(
	TEXT_ALIGN_VER *peVerAlignModeN,
	TEXT_ALIGN_VER *peVerAlignModeH,
	TEXT_ALIGN_VER *peVerAlignModeS,
	TEXT_ALIGN_VER *peVerAlignModeD) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

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

int CButtonBase::SetCheckedTextAlignHor(
	TEXT_ALIGN_HOR eHorAlignModeCN,
	TEXT_ALIGN_HOR eHorAlignModeCH,
	TEXT_ALIGN_HOR eHorAlignModeCS,
	TEXT_ALIGN_HOR eHorAlignModeCD,
	BOOL bRedraw/* = TRUE*/)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_eTextHorAlignModeC[0] = eHorAlignModeCN;
	pMember->m_eTextHorAlignModeC[1] = eHorAlignModeCH;
	pMember->m_eTextHorAlignModeC[2] = eHorAlignModeCS;
	pMember->m_eTextHorAlignModeC[3] = eHorAlignModeCD;

	if (bRedraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CButtonBase::GetCheckedTextAlignHor(
	TEXT_ALIGN_HOR *peHorAlignModeCN,
	TEXT_ALIGN_HOR *peHorAlignModeCH,
	TEXT_ALIGN_HOR *peHorAlignModeCS,
	TEXT_ALIGN_HOR *peHorAlignModeCD) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	if (peHorAlignModeCN != NULL)
	{
		*peHorAlignModeCN = pMember->m_eTextHorAlignModeC[0];
	}
	if (peHorAlignModeCH != NULL)
	{
		*peHorAlignModeCH = pMember->m_eTextHorAlignModeC[1];
	}
	if (peHorAlignModeCS != NULL)
	{
		*peHorAlignModeCS = pMember->m_eTextHorAlignModeC[2];
	}
	if (peHorAlignModeCD != NULL)
	{
		*peHorAlignModeCD = pMember->m_eTextHorAlignModeC[3];
	}

	return 0;
}

int CButtonBase::SetCheckedTextAlignVer(
	TEXT_ALIGN_VER eVerAlignModeCN,
	TEXT_ALIGN_VER eVerAlignModeCH,
	TEXT_ALIGN_VER eVerAlignModeCS,
	TEXT_ALIGN_VER eVerAlignModeCD,
	BOOL bRedraw/* = TRUE*/)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_eTextVerAlignModeC[0] = eVerAlignModeCN;
	pMember->m_eTextVerAlignModeC[1] = eVerAlignModeCH;
	pMember->m_eTextVerAlignModeC[2] = eVerAlignModeCS;
	pMember->m_eTextVerAlignModeC[3] = eVerAlignModeCD;

	if (bRedraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CButtonBase::GetCheckedTextAlignVer(
	TEXT_ALIGN_VER *peVerAlignModeCN,
	TEXT_ALIGN_VER *peVerAlignModeCH,
	TEXT_ALIGN_VER *peVerAlignModeCS,
	TEXT_ALIGN_VER *peVerAlignModeCD) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	if (peVerAlignModeCN != NULL)
	{
		*peVerAlignModeCN = pMember->m_eTextVerAlignModeC[0];
	}
	if (peVerAlignModeCH != NULL)
	{
		*peVerAlignModeCH = pMember->m_eTextVerAlignModeC[1];
	}
	if (peVerAlignModeCS != NULL)
	{
		*peVerAlignModeCS = pMember->m_eTextVerAlignModeC[2];
	}
	if (peVerAlignModeCD != NULL)
	{
		*peVerAlignModeCD = pMember->m_eTextVerAlignModeC[3];
	}

	return 0;
}

int CButtonBase::SetToolTips(LPCTSTR lpszToolTips)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	if (!::IsWindow(pMember->m_wndToolTip.GetSafeHwnd()))
	{
		pMember->m_wndToolTip.Create(m_pBindWnd);
		ASSERT(::IsWindow(pMember->m_wndToolTip.GetSafeHwnd()));
	}

	pMember->m_wndToolTip.AddTool(m_pBindWnd, lpszToolTips);

	return 0;
}

int CButtonBase::SetToolTipDelayTime(DWORD dwDuration, int iTime)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_wndToolTip.SetDelayTime(dwDuration, iTime);

	return 0;
}

CToolTipCtrl *CButtonBase::GetToolTipCtrl()
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	return &pMember->m_wndToolTip;
}

int CButtonBase::SetTag(LPCTSTR lpszTag)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	if (lpszTag == 0)
	{
		return -1;
	}

	pMember->m_strTag = lpszTag;

	return 0;
}

int CButtonBase::GetTag(CString &strTag) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	strTag = pMember->m_strTag;

	return 0;
}

int CButtonBase::SetBindModalDlgID(UINT uID)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_uBindModalDialogID = uID;

	return (int)pMember->m_uBindModalDialogID;
}

UINT CButtonBase::GetBindModalDlgID() const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	return pMember->m_uBindModalDialogID;
}

int CButtonBase::SetBindModelessWndID(UINT uID)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_uBindModelessWndID = uID;

	return (int)pMember->m_uBindModelessWndID;
}

UINT CButtonBase::GetBindModelessWndID() const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	return pMember->m_uBindModelessWndID;
}

int CButtonBase::SetBindTaskWndMgr(UINT uID)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_uBindTaskWndMgrID = uID;

	return (int)pMember->m_uBindTaskWndMgrID;
}

UINT CButtonBase::GetBindTaskWndMgr() const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	return pMember->m_uBindTaskWndMgrID;
}

int CButtonBase::SetBindTaskWindow(UINT uID)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_uBindTaskWindowID = uID;

	return (int)pMember->m_uBindTaskWindowID;
}

UINT CButtonBase::GetBindTaskWindow() const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	return pMember->m_uBindTaskWindowID;
}

int CButtonBase::SetPushButtonType(BTN_TYPE eButtonType)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_eButtonType = eButtonType;

	return 0;
}

BTN_TYPE CButtonBase::GetPushButtonType() const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	return pMember->m_eButtonType;
}

int CButtonBase::SetCheckBoxType(CHK_TYPE eCheckBoxType)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_eCheckBoxType = eCheckBoxType;

	return 0;
}

CHK_TYPE CButtonBase::GetCheckBoxType() const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	return pMember->m_eCheckBoxType;
}

int CButtonBase::AnimateText(Direction eDirection, int nDelay, int nPixel/* = 1*/)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_bAnimateText = TRUE;
	pMember->m_eAnimateDirection = eDirection;
	pMember->m_nAnimateDelay = nDelay;
	pMember->m_nAnimatePixel = nPixel;

	SetTimer(m_pBindWnd->GetSafeHwnd(), TIMER_ANIMATETEXT, nDelay, NULL);

	return 0;
}

int CButtonBase::IsAnimateText() const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	return pMember->m_bAnimateText;
}

int CButtonBase::StopAnimateText()
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_bAnimateText = FALSE;
	KillTimer(m_pBindWnd->GetSafeHwnd(), TIMER_ANIMATETEXT);

	return 0;
}

int CButtonBase::SetEndEllipsis(BOOL bEndEllipsis)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_bEndEllipsis = bEndEllipsis;
	return 0;
}

BOOL CButtonBase::IsEndEllipsis() const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	return pMember->m_bEndEllipsis;
}

int CButtonBase::SetTabStop(BOOL bTabStop)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_bTabStop = bTabStop;
	return 0;
}

BOOL CButtonBase::IsTabStop() const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	return pMember->m_bTabStop;
}

int CButtonBase::SetActiveArea(const CRect &rect)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_bUseActiveArea = TRUE;
	pMember->m_rcActive = rect;
	return 0;
}

int CButtonBase::GetActiveArea(LPRECT lpRect) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	if (lpRect == NULL)
	{
		return -1;
	}

	*lpRect = pMember->m_rcActive;
	return 0;
}

BOOL CButtonBase::IsHasActiveArea() const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	return pMember->m_bUseActiveArea;
}

int CButtonBase::RemoveActiveArea()
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_bUseActiveArea = FALSE;
	return 0;
}

BOOL CButtonBase::IsCapture() const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	return pMember->m_bRecordLButtonDown;
}

BOOL CButtonBase::IsHighlight() const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	return pMember->m_bMouseOver;
}

BOOL CButtonBase::IsMultiLine() const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	return pMember->m_bMultiLine;
}

int CButtonBase::SetHoverTime(DWORD dwHoverTime, BOOL bRepeat/* = FALSE*/)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

	pMember->m_dwHoverTime = dwHoverTime;
	pMember->m_bHoverRepeat = bRepeat;

	return 0;
}

int CButtonBase::GetHoverTime(DWORD *pdwHoverTime, BOOL *pbRepeat) const
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)m_pMember;

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
