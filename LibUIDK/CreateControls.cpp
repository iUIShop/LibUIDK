#include "stdafx.h"
#include <atlconv.h>
#include <map>
#include <comutil.h>
#include <ERRNO.H>
#include <algorithm>
#include <afxpriv.h>
#if(_MSC_VER <= 1200) // VC6.0
#include <../src/afximpl.h>
#else
#include <../src/mfc/afximpl.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// Create controls

void LibUIDK::SetButtonImagesByParent(CRect rect, CWnd *pChild,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	if (pChild == NULL)
	{
		return;
	}

	HIUIIMAGE hBmpN = NULL;
	HIUIIMAGE hBmpH = NULL;
	HIUIIMAGE hBmpS = NULL;
	HIUIIMAGE hBmpD = NULL;
	if (himgN != NULL)
	{
		CIUIImage::SubBitmap(himgN, &hBmpN, rect);
	}
	if (himgH != NULL)
	{
		CIUIImage::SubBitmap(himgH, &hBmpH, rect);
	}
	if (himgS != NULL)
	{
		CIUIImage::SubBitmap(himgS, &hBmpS, rect);
	}
	if (himgD != NULL)
	{
		CIUIImage::SubBitmap(himgD, &hBmpD, rect);
	}

	CSkinButton *pButton = (CSkinButton *)pChild;
	pButton->CButtonBase::SetBitmap(CONTROL_STATE_UNCHECKED_ALL,
		hBmpN->GetSafeImageName(), hBmpH->GetSafeImageName(),
		hBmpS->GetSafeImageName(), hBmpD->GetSafeImageName());

	// SetBitmap中会增加HIUIIMAGE的引用计数，所以这里设置完后需要
	ReleaseIUIImage(hBmpN);
	hBmpN = NULL;
	ReleaseIUIImage(hBmpH);
	hBmpH = NULL;
	ReleaseIUIImage(hBmpS);
	hBmpS = NULL;
	ReleaseIUIImage(hBmpD);
	hBmpD = NULL;
}

CSkinButton *LibUIDK::CreateButton(BTNPROPERTIES *pBtn, CWnd *pParent,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	int nID = LONG(pBtn->m_nID);
	ASSERT(nID != -1);

	DWORD dwStyle = BS_PUSHBUTTON | GetControlBaseStyle(pBtn);
	if (pBtn->m_bDefPushButton)
	{
		dwStyle |= BS_DEFPUSHBUTTON;
	}
	if (pBtn->m_bMultiline)
	{
		dwStyle |= BS_MULTILINE;
	}

	CSkinButton *pChild = new CSkinButton;
	ASSERT(pChild != NULL);

	CRect rect = pBtn->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(pBtn->m_strCaption, dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	// CWnd through DM_GETDEFID to handle default button
	if ((dwStyle & BS_DEFPUSHBUTTON) == BS_DEFPUSHBUTTON)
	{
		// If already has default push button. set its style
		// See the remark section of DM_SETDEFID in MSDN.

		int nRet = (int)pParent->SendMessage(DM_GETDEFID);
		if (nRet != 0)
		{
			int nHas = HIWORD(nRet);
			int nPreviousID = LOWORD(nRet);
			CWnd *pPreviousBtn = pParent->GetDlgItem(nPreviousID);

			if (nHas == DC_HASDEFID && pPreviousBtn->GetSafeHwnd() != NULL)
			{
				LONG_PTR lStyle = GetWindowLongPtr(pChild->GetSafeHwnd(), GWL_STYLE);
				lStyle &= ~BS_DEFPUSHBUTTON;
				pPreviousBtn->SendMessage(BM_SETSTYLE, lStyle, MAKELPARAM(FALSE, 0));
			}
		}

		BOOL bRet = (BOOL)pParent->SendMessage(DM_SETDEFID, nID, 0);
	}

	pChild->BindPushButtonStyle(pBtn);

	// Set background bitmaps for control
	if (pBtn->m_bSpecifyBackgroundImages)
	{
		// Set by CSkinButton::BindPushButtonStyle
	}
	else
	{
		SetButtonImagesByParent(pBtn->m_rcControl, pChild, himgN, himgH, himgS, himgD);
	}

	return pChild;
}

void LibUIDK::SetToggleButtonImagesByParent(TOGGLEBTNPROPERTIES *pToggleProp,
	CRect rect, CSkinButton *pChild,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	// Set background
	bool bSpecifyBackgroundImages = pToggleProp->m_bSpecifyBackgroundImages;
	if (!bSpecifyBackgroundImages)
	{
		HIUIIMAGE hBmpBK[8] = {NULL};
		if (himgN != NULL)
		{
			CIUIImage::SubBitmap(himgN, &hBmpBK[0], rect);
		}
		if (himgH != NULL)
		{
			CIUIImage::SubBitmap(himgH, &hBmpBK[1], rect);
		}
		if (himgS != NULL)
		{
			CIUIImage::SubBitmap(himgS, &hBmpBK[2], rect);
		}
		if (himgD != NULL)
		{
			CIUIImage::SubBitmap(himgD, &hBmpBK[3], rect);
		}

		int i = 0;
		for (i = 0; i < 4 ; i++)
		{
			if (hBmpBK[i] != NULL)
			{
				hBmpBK[i]->ConvertToDPIImage(GetDPIX(), GetDPIY());
			}
		}

		hBmpBK[4] = CreateIUIImage((LPCTSTR)pToggleProp->m_strBKCN);
		hBmpBK[5] = CreateIUIImage((LPCTSTR)pToggleProp->m_strBKCH);
		hBmpBK[6] = CreateIUIImage((LPCTSTR)pToggleProp->m_strBKCS);
		hBmpBK[7] = CreateIUIImage((LPCTSTR)pToggleProp->m_strBKCD);

		((CSkinButton *)pChild)->CButtonBase::SetBitmap(CONTROL_STATE_UNCHECKED_ALL,
			hBmpBK[0]->GetSafeImageName(), hBmpBK[1]->GetSafeImageName(),
			hBmpBK[2]->GetSafeImageName(), hBmpBK[3]->GetSafeImageName());

		((CSkinButton *)pChild)->CButtonBase::SetCheckedBitmap(CONTROL_STATE_CHECKED_ALL,
			hBmpBK[4]->GetSafeImageName(), hBmpBK[5]->GetSafeImageName(),
			hBmpBK[6]->GetSafeImageName(), hBmpBK[7]->GetSafeImageName());

		// SetBitmap中会增加hBmpBK[i]的引用计数，所以这里设置完后需要
		// 释放（减少引用计数）
		for (i = 0; i < 8 ; i++)
		{
			ReleaseIUIImage(hBmpBK[i]);
			hBmpBK[i] = NULL;
		}
	}
}

CSkinButton *LibUIDK::CreateCheckBox(CHKPROPERTIES *pChk, CWnd *pParent,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	int nID = LONG(pChk->m_nID);
	ASSERT(nID != -1);

	DWORD dwStyle = BS_AUTOCHECKBOX | GetControlBaseStyle(pChk);
	if (pChk->m_bMultiline)
	{
		dwStyle |= BS_MULTILINE;
	}

	CSkinButton *pChild = new CSkinButton;
	ASSERT(pChild != NULL);

	CRect rect = pChk->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(pChk->m_strCaption, dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindCheckBoxStyle(pChk);

	// Set bitmaps
	SetToggleButtonImagesByParent(pChk, pChk->m_rcControl, pChild, himgN, himgH, himgS, himgD);

	return pChild;
}

CSkinButton *LibUIDK::CreateRadioButton(RADPROPERTIES *pRad, CWnd *pParent,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	// ID
	int nID = LONG(pRad->m_nID);
	ASSERT(nID != -1);

	// Properties
	DWORD dwStyle = BS_AUTORADIOBUTTON | GetControlBaseStyle(pRad);
	if (pRad->m_bGroup)
	{
		dwStyle |= WS_GROUP;
	}
	if (pRad->m_bMultiline)
	{
		dwStyle |= BS_MULTILINE;
	}

	CSkinButton *pChild = new CSkinButton;
	ASSERT(pChild != NULL);

	CRect rect = pRad->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(pRad->m_strCaption, dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindRadioButtonStyle(pRad);

	// Set bitmaps
	SetToggleButtonImagesByParent(pRad, pRad->m_rcControl, pChild, himgN, himgH, himgS, himgD);

	return pChild;
}

void LibUIDK::SetEditImagesByParent(EDTPROPERTIES *pEdtProp, CRect rect, CWnd *pChild,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	bool bSpecifyBackgroundImages = pEdtProp->m_bSpecifyBackgroundImages;
	if (!bSpecifyBackgroundImages)
	{
		HIUIIMAGE hBmpBK[4] = {NULL};
		if (himgN != NULL)
		{
			CIUIImage::SubBitmap(himgN, &hBmpBK[0], rect);
		}
		if (himgH != NULL)
		{
			CIUIImage::SubBitmap(himgH, &hBmpBK[1], rect);
		}
		if (himgS != NULL)
		{
			CIUIImage::SubBitmap(himgS, &hBmpBK[2], rect);
		}
		if (himgD != NULL)
		{
			CIUIImage::SubBitmap(himgD, &hBmpBK[3], rect);
		}

		int i = 0;
		for (i = 0; i < 4 ; i++)
		{
			hBmpBK[i]->ConvertToDPIImage(GetDPIX(), GetDPIY());
		}

		((CSkinEdit *)pChild)->SetImages(CONTROL_STATE_UNCHECKED_ALL,
			hBmpBK[0]->GetSafeImageName(), hBmpBK[1]->GetSafeImageName(),
			hBmpBK[2]->GetSafeImageName(), hBmpBK[3]->GetSafeImageName());

		// SetBitmap中会增加hBmpBK[i]的引用计数，所以这里设置完后需要
		// 释放（减少引用计数）
		for (i = 0; i < 4; i++)
		{
			ReleaseIUIImage(hBmpBK[i]);
			hBmpBK[i] = NULL;
		}
	}
}

CSkinEdit *LibUIDK::CreateEdit(EDTPROPERTIES *pEdt, CWnd *pParent,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	int nID = LONG(pEdt->m_nID);
	ASSERT(nID != -1);

	DWORD dwStyle = GetControlBaseStyle(pEdt);
	if (pEdt->m_bHScroll)
	{
		dwStyle |= WS_HSCROLL;
	}
	if (pEdt->m_bVScroll)
	{
		dwStyle |= WS_VSCROLL;
	}
	if (pEdt->m_bWantReturn)
	{
		dwStyle |= ES_WANTRETURN;
	}
	if (pEdt->m_bAutoHScroll)
	{
		dwStyle |= ES_AUTOHSCROLL;
	}
	if (pEdt->m_bAutoVScroll)
	{
		dwStyle |= ES_AUTOVSCROLL;
	}
	if (pEdt->m_bLowercase)
	{
		dwStyle |= ES_LOWERCASE;
	}
	if (pEdt->m_bUppercase)
	{
		dwStyle |= ES_UPPERCASE;
	}
	if (pEdt->m_bNumber)
	{
		dwStyle |= ES_NUMBER;
	}
	if (pEdt->m_bPassword)
	{
		dwStyle |= ES_PASSWORD;
	}
	if (pEdt->m_bReadOnly)
	{
		dwStyle |= ES_READONLY;
	}
	if (pEdt->m_bMultiline)
	{
		dwStyle |= ES_MULTILINE;
	}
	if (TAH_DEFAULT == (TEXT_ALIGN_HOR)pEdt->m_eTextHorAlignMode
		|| TAH_LEFT == (TEXT_ALIGN_HOR)pEdt->m_eTextHorAlignMode)
	{
		dwStyle |= ES_LEFT;
	}
	else if (TAH_CENTER == (TEXT_ALIGN_HOR)pEdt->m_eTextHorAlignMode)
	{
		dwStyle |= ES_CENTER;
	}
	else if (TAH_RIGHT == (TEXT_ALIGN_HOR)pEdt->m_eTextHorAlignMode)
	{
		dwStyle |= ES_RIGHT;
	}

	CSkinEdit *pChild = new CSkinEdit;
	ASSERT(pChild != NULL);

	CRect rect = pEdt->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pEdt);

	SetEditImagesByParent(pEdt, pEdt->m_rcControl, pChild, himgN, himgH, himgS, himgD);

	return pChild;
}

void LibUIDK::SetComboBoxImagesByParent(CRect rect, CWnd *pChild,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	CRect rcCombo;
	pChild->GetWindowRect(rcCombo);
	rect.bottom = rect.top + rcCombo.Height();

	HIUIIMAGE hBmpBK[4] = {NULL};
	if (himgN != NULL)
	{
		CIUIImage::SubBitmap(himgN, &hBmpBK[0], rect);
	}
	if (himgH != NULL)
	{
		CIUIImage::SubBitmap(himgH, &hBmpBK[1], rect);
	}
	if (himgS != NULL)
	{
		CIUIImage::SubBitmap(himgS, &hBmpBK[2], rect);
	}
	if (himgD != NULL)
	{
		CIUIImage::SubBitmap(himgD, &hBmpBK[3], rect);
	}

	int i = 0;
	for (i = 0; i < 4 ; i++)
	{
		hBmpBK[i]->ConvertToDPIImage(GetDPIX(), GetDPIY());
	}

	((CSkinComboBox *)pChild)->SetBitmap(CONTROL_STATE_UNCHECKED_ALL,
		hBmpBK[0]->GetSafeImageName(), hBmpBK[1]->GetSafeImageName(),
		hBmpBK[2]->GetSafeImageName(), hBmpBK[3]->GetSafeImageName());

	// SetBitmap中会增加hBmpBK[i]的引用计数，所以这里设置完后需要
	// 释放（减少引用计数）
	for (i = 0; i < 4; i++)
	{
		ReleaseIUIImage(hBmpBK[i]);
		hBmpBK[i] = NULL;
	}
}

CSkinComboBox *LibUIDK::CreateComboBox(CMBPROPERTIES *pCmb, CWnd *pParent,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	CRect rect = pCmb->m_rcControl;
	int nID = LONG(pCmb->m_nID);
	ASSERT(nID != -1);

	rect.bottom += (LONG)pCmb->m_lDropListHeight;

	DWORD dwStyle = GetControlBaseStyle(pCmb);
	switch ((CMB_STYLE)pCmb->m_eComboStyle)
	{
	case CS_SIMPLE:
		dwStyle |= CBS_SIMPLE;
		break;
	case CS_DROPDOWN:
		dwStyle |= CBS_DROPDOWN;
		break;
	case CS_DROPDOWNLIST:
		dwStyle |= CBS_DROPDOWNLIST;
		break;
	default:
		break;
	}
	if (pCmb->m_bVScroll)
	{
		dwStyle |= WS_VSCROLL;
	}
	if (pCmb->m_bAutoHScroll)
	{
		dwStyle |= CBS_AUTOHSCROLL;
	}
	if (pCmb->m_bDisableNoScroll)
	{
		dwStyle |= CBS_DISABLENOSCROLL;
	}
	if (pCmb->m_bSort)
	{
		dwStyle |= CBS_SORT;
	}
	if (pCmb->m_bUppercase)
	{
		dwStyle |= CBS_UPPERCASE;
	}
	if (pCmb->m_bLowercase)
	{
		dwStyle |= CBS_LOWERCASE;
	}

	CSkinComboBox *pChild = new CSkinComboBox;
	ASSERT(pChild != NULL);

	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pCmb);

	if (pCmb->m_bSpecifyBackgroundImages)
	{
	}
	else
	{
		SetComboBoxImagesByParent(pCmb->m_rcControl, pChild, himgN, himgH, himgS, himgD);
	}

	return pChild;
}

void LibUIDK::SetStaticImagesByParent(CRect rect, CWnd *pChild,
	HIUIIMAGE himgN, HIUIIMAGE himgD)
{
	HIUIIMAGE hBmpBK[2] = {NULL};
	if (himgN != NULL)
	{
		CIUIImage::SubBitmap(himgN, &hBmpBK[0], rect);
	}
	if (himgD != NULL)
	{
		CIUIImage::SubBitmap(himgD, &hBmpBK[1], rect);
	}

	int i = 0;
	for (i = 0; i < 2 ; i++)
	{
		hBmpBK[i]->ConvertToDPIImage(GetDPIX(), GetDPIY());
	}

	((CSkinStatic *)pChild)->SetBitmap(CONTROL_STATE_UNCHECKED_ND,
		hBmpBK[0]->GetSafeImageName(), hBmpBK[1]->GetSafeImageName());

	// SetBitmap中会增加hBmpBK[i]的引用计数，所以这里设置完后需要
	// 释放（减少引用计数）
	for (i = 0; i < 2; i++)
	{
		ReleaseIUIImage(hBmpBK[i]);
		hBmpBK[i] = NULL;
	}
}

CSkinStatic *LibUIDK::CreateStatic(TXTPROPERTIES *pSta, CWnd *pParent,
	HIUIIMAGE himgN, HIUIIMAGE himgD)
{
	int nID = LONG(pSta->m_nID);
	ASSERT(nID != -1);

	DWORD dwStyle = SS_NOTIFY | GetControlBaseStyle(pSta);

	CSkinStatic *pChild = new CSkinStatic;
	ASSERT(pChild != NULL);

	CRect rect = pSta->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(pSta->m_strCaption, dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pSta);

	if (pSta->m_bSpecifyBackgroundImages)
	{
	}
	else
	{
		// if not specify background images, the disable image same as the normal.
		SetStaticImagesByParent(pSta->m_rcControl, pChild, himgN, himgD);
	}

	return pChild;
}

void LibUIDK::SetSliderImagesByParent(CRect rect, CWnd *pChild,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	HIUIIMAGE hBmpBK[2] = {NULL};
	if (himgN != NULL)
	{
		CIUIImage::SubBitmap(himgN, &hBmpBK[0], rect);
	}
	if (himgD != NULL)
	{
		CIUIImage::SubBitmap(himgD, &hBmpBK[1], rect);
	}

	int i = 0;
	for (i = 0; i < 2 ; i++)
	{
		hBmpBK[i]->ConvertToDPIImage(GetDPIX(), GetDPIY());
	}

	((CSkinSliderCtrl *)pChild)->SetImages(CONTROL_STATE_UNCHECKED_ND,
		hBmpBK[0]->GetSafeImageName(), hBmpBK[1]->GetSafeImageName());

	// SetBitmap中会增加hBmpBK[i]的引用计数，所以这里设置完后需要
	// 释放（减少引用计数）
	for (i = 0; i < 2; i++)
	{
		ReleaseIUIImage(hBmpBK[i]);
		hBmpBK[i] = NULL;
	}
}

CSkinSliderCtrl *LibUIDK::CreateSlider(SLDPROPERTIES *pSld, CWnd *pParent,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	int nID = LONG(pSld->m_nID);
	ASSERT(nID != -1);

	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pSld);
	if (pSld->m_bVertical)
	{
		dwStyle |= TBS_VERT;
	}
	if (pSld->m_bTabStop)
	{
		dwStyle |= WS_TABSTOP;
	}

	CSkinSliderCtrl *pChild = new CSkinSliderCtrl;
	ASSERT(pChild != NULL);

	CRect rect = pSld->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pSld);

	bool bSpecifyBackgroundImages = pSld->m_bSpecifyBackgroundImages;
	if (!bSpecifyBackgroundImages)
	{
		SetSliderImagesByParent(pSld->m_rcControl, pChild, himgN, himgH, himgS, himgD);
	}

	return pChild;
}

void LibUIDK::SetSpinButtonImagesByParent(CRect rect, CWnd *pChild,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	HIUIIMAGE hBmpBK[4] = {NULL};
	if (himgN != NULL)
	{
		CIUIImage::SubBitmap(himgN, &hBmpBK[0], rect);
	}
	if (himgH != NULL)
	{
		CIUIImage::SubBitmap(himgH, &hBmpBK[1], rect);
	}
	if (himgS != NULL)
	{
		CIUIImage::SubBitmap(himgS, &hBmpBK[2], rect);
	}
	if (himgD != NULL)
	{
		CIUIImage::SubBitmap(himgD, &hBmpBK[3], rect);
	}

	int i = 0;
	for (i = 0; i < 4 ; i++)
	{
		hBmpBK[i]->ConvertToDPIImage(GetDPIX(), GetDPIY());
	}

	((CSkinSpinButtonCtrl *)pChild)->SetImages(CONTROL_STATE_UNCHECKED_ALL,
		hBmpBK[0]->GetSafeImageName(), hBmpBK[1]->GetSafeImageName(),
		hBmpBK[2]->GetSafeImageName(), hBmpBK[3]->GetSafeImageName());

	// SetBitmap中会增加hBmpBK[i]的引用计数，所以这里设置完后需要
	// 释放（减少引用计数）
	for (i = 0; i < 4; i++)
	{
		ReleaseIUIImage(hBmpBK[i]);
		hBmpBK[i] = NULL;
	}
}

CSkinSpinButtonCtrl *LibUIDK::CreateSpinButton(SPNPROPERTIES *pSpn, CWnd *pParent,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	int nID = LONG(pSpn->m_nID);
	ASSERT(nID != -1);

	DWORD dwStyle = GetControlBaseStyle(pSpn);
	if (!(BOOL)pSpn->m_bVertical)
	{
		dwStyle |= UDS_HORZ;
	}
	if (pSpn->m_bAutoBuddy)
	{
		dwStyle |= UDS_AUTOBUDDY;
	}
	if (pSpn->m_bSetBuddyInt)
	{
		dwStyle |= UDS_SETBUDDYINT;
	}
	if (pSpn->m_bNoThousands)
	{
		dwStyle |= UDS_NOTHOUSANDS;
	}
	if (pSpn->m_bWrap)
	{
		dwStyle |= UDS_WRAP;
	}
	if (pSpn->m_bArrowKeys)
	{
		dwStyle |= UDS_ARROWKEYS;
	}
	if (pSpn->m_bHotTrack)
	{
		dwStyle |= UDS_HOTTRACK;
	}
	if (SPNA_LEFT == (SPN_ALIGN)pSpn->m_eAlignMode)
	{
		dwStyle |= UDS_ALIGNLEFT;
	}
	else if (SPNA_RIGHT == (SPN_ALIGN)pSpn->m_eAlignMode)
	{
		dwStyle |= UDS_ALIGNRIGHT;
	}

	CSkinSpinButtonCtrl *pChild = new CSkinSpinButtonCtrl;
	ASSERT(pChild != NULL);

	CRect rect = pSpn->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}

	// CSpinButtonCtrl ignore the width while creating. so need call MoveWindow after created.
	BOOL bShow = IsIncludeFlag(dwStyle, WS_VISIBLE);
	dwStyle &= ~WS_VISIBLE;

	if (!pChild->Create(dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	pChild->MoveWindow(rect);
	if (bShow)
	{
		pChild->ShowWindow(SW_SHOW);
	}

	pChild->BindStyle(pSpn);

	bool bSpecifyBackgroundImages = pSpn->m_bSpecifyBackgroundImages;
	if (!bSpecifyBackgroundImages)
	{
		SetSpinButtonImagesByParent(pSpn->m_rcControl, pChild, himgN, himgH, himgS, himgD);
	}

	return pChild;
}

void LibUIDK::SetProgressImagesByParent(PRGPROPERTIES *pPrg, CRect rect, CWnd *pChild,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	HIUIIMAGE hBmpBK[2] = {NULL};
	if (himgN != NULL)
	{
		CIUIImage::SubBitmap(himgN, &hBmpBK[0], rect);
	}
	if (himgD != NULL)
	{
		CIUIImage::SubBitmap(himgD, &hBmpBK[1], rect);
	}

	int i = 0;
	for (i = 0; i < 2 ; i++)
	{
		hBmpBK[i]->ConvertToDPIImage(GetDPIX(), GetDPIY());
	}

	((CSkinProgressCtrl *)pChild)->SetImages(CONTROL_STATE_UNCHECKED_ND,
		hBmpBK[0]->GetSafeImageName(), hBmpBK[1]->GetSafeImageName());

	// SetBitmap中会增加hBmpBK[i]的引用计数，所以这里设置完后需要
	// 释放（减少引用计数）
	for (i = 0; i < 2; i++)
	{
		ReleaseIUIImage(hBmpBK[i]);
		hBmpBK[i] = NULL;
	}
}

CSkinProgressCtrl *LibUIDK::CreateProgress(PRGPROPERTIES *pPrg, CWnd *pParent,
	HIUIIMAGE bmpN, HIUIIMAGE bmpH, HIUIIMAGE bmpS, HIUIIMAGE bmpD)
{
	int nID = LONG(pPrg->m_nID);
	ASSERT(nID != -1);

	CSkinProgressCtrl *pChild = new CSkinProgressCtrl;
	ASSERT(pChild != NULL);
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pPrg);
	if (pPrg->m_bVertical)
	{
		dwStyle |= PBS_VERTICAL;
	}
	if (pPrg->m_bTabStop)
	{
		dwStyle |= WS_TABSTOP;
	}

	CRect rect = pPrg->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pPrg);

	bool bSpecifyBackgroundImages = pPrg->m_bSpecifyBackgroundImages;
	if (!bSpecifyBackgroundImages)
	{
		SetProgressImagesByParent(pPrg, pPrg->m_rcControl, pChild, bmpN, bmpH, bmpS, bmpD);
	}

	return pChild;
}

void LibUIDK::SetHotKeyImagesByParent(CRect rect, CWnd *pChild,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	HIUIIMAGE hBmpBK[4] = {NULL};
	if (himgN != NULL)
	{
		CIUIImage::SubBitmap(himgN, &hBmpBK[0], rect);
	}
	if (himgH != NULL)
	{
		CIUIImage::SubBitmap(himgH, &hBmpBK[1], rect);
	}
	if (himgS != NULL)
	{
		CIUIImage::SubBitmap(himgS, &hBmpBK[2], rect);
	}
	if (himgD != NULL)
	{
		CIUIImage::SubBitmap(himgD, &hBmpBK[3], rect);
	}

	((CSkinHotKeyCtrl *)pChild)->SetImages(CONTROL_STATE_UNCHECKED_ALL,
		hBmpBK[0]->GetSafeImageName(), hBmpBK[1]->GetSafeImageName(),
		hBmpBK[2]->GetSafeImageName(), hBmpBK[3]->GetSafeImageName());

	// SetBitmap中会增加hBmpBK[i]的引用计数，所以这里设置完后需要
	// 释放（减少引用计数）
	for (int i = 0; i < 4; i++)
	{
		ReleaseIUIImage(hBmpBK[i]);
		hBmpBK[i] = NULL;
	}
}

CSkinHotKeyCtrl *LibUIDK::CreateHotKey(HOTPROPERTIES *pHot, CWnd *pParent,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	int nID = LONG(pHot->m_nID);
	ASSERT(nID != -1);

	CSkinHotKeyCtrl *pChild = new CSkinHotKeyCtrl;
	ASSERT(pChild != NULL);

	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pHot);

	CRect rect = pHot->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pHot);

	bool bSpecifyBackgroundImages = pHot->m_bSpecifyBackgroundImages;
	if (!bSpecifyBackgroundImages)
	{
		SetHotKeyImagesByParent(pHot->m_rcControl, pChild, himgN, himgH, himgS, himgD);
	}

	return pChild;
}

void LibUIDK::SetIPAddressImagesByParent(CRect rect, CWnd *pChild,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	HIUIIMAGE hBmpBK[4] = {NULL};
	if (himgN != NULL)
	{
		CIUIImage::SubBitmap(himgN, &hBmpBK[0], rect);
	}
	if (himgH != NULL)
	{
		CIUIImage::SubBitmap(himgH, &hBmpBK[1], rect);
	}
	if (himgS != NULL)
	{
		CIUIImage::SubBitmap(himgS, &hBmpBK[2], rect);
	}
	if (himgD != NULL)
	{
		CIUIImage::SubBitmap(himgD, &hBmpBK[3], rect);
	}

	int i = 0;
	for (i = 0; i < 4 ; i++)
	{
		hBmpBK[i]->ConvertToDPIImage(GetDPIX(), GetDPIY());
	}

	((CSkinIPAddressCtrl *)pChild)->SetImages(CONTROL_STATE_UNCHECKED_ALL,
		hBmpBK[0]->GetSafeImageName(), hBmpBK[1]->GetSafeImageName(),
		hBmpBK[2]->GetSafeImageName(), hBmpBK[3]->GetSafeImageName());

	// SetBitmap中会增加hBmpBK[i]的引用计数，所以这里设置完后需要
	// 释放（减少引用计数）
	for (i = 0; i < 4; i++)
	{
		ReleaseIUIImage(hBmpBK[i]);
		hBmpBK[i] = NULL;
	}
}

CSkinIPAddressCtrl *LibUIDK::CreateIPAddress(IPAPROPERTIES *pIPA, CWnd *pParent,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	int nID = LONG(pIPA->m_nID);
	ASSERT(nID != -1);

	CSkinIPAddressCtrl *pChild = new CSkinIPAddressCtrl;
	ASSERT(pChild != NULL);

	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pIPA);

	CRect rect = pIPA->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pIPA);

	bool bSpecifyBackgroundImages = pIPA->m_bSpecifyBackgroundImages;
	if (!bSpecifyBackgroundImages)
	{
		SetIPAddressImagesByParent(pIPA->m_rcControl, pChild, himgN, himgH, himgS, himgD);
	}

	return pChild;
}

void LibUIDK::SetSliderExImagesByParent(SLDEXPROPERTIES *pSldEx, CRect rect, CWnd *pChild,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	HIUIIMAGE hBmpBK[4] = {NULL};
	if (himgN != NULL)
	{
		CIUIImage::SubBitmap(himgN, &hBmpBK[0], rect);
	}
	if (himgH != NULL)
	{
		CIUIImage::SubBitmap(himgH, &hBmpBK[1], rect);
	}
	if (himgS != NULL)
	{
		CIUIImage::SubBitmap(himgS, &hBmpBK[2], rect);
	}
	if (himgD != NULL)
	{
		CIUIImage::SubBitmap(himgD, &hBmpBK[3], rect);
	}

	int i = 0;
	for (i = 0; i < 4 ; i++)
	{
		hBmpBK[i]->ConvertToDPIImage(GetDPIX(), GetDPIY());
	}

	((CSliderCtrlEx *)pChild)->SetImages(CONTROL_STATE_UNCHECKED_ALL,
		hBmpBK[0]->GetSafeImageName(), hBmpBK[1]->GetSafeImageName(),
		hBmpBK[2]->GetSafeImageName(), hBmpBK[3]->GetSafeImageName());

	// SetBitmap中会增加hBmpBK[i]的引用计数，所以这里设置完后需要
	// 释放（减少引用计数）
	for (i = 0; i < 4; i++)
	{
		ReleaseIUIImage(hBmpBK[i]);
		hBmpBK[i] = NULL;
	}
}

CSliderCtrlEx *LibUIDK::CreateSliderEx(SLDEXPROPERTIES *pSldEx, CWnd *pParent,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	int nID = LONG(pSldEx->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pSldEx);
	if (pSldEx->m_bVertical)
	{
		dwStyle |= TBS_VERT;
	}

	// Create extended slider
	CSliderCtrlEx *pChild = new CSliderCtrlEx;
	ASSERT(pChild != NULL);

	CRect rect = pSldEx->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	if (SLDES_PROGRESS == (SLD_EXT_STYLE)pSldEx->m_eExtendedStyle)
	{
		pChild->SetExtendedStyle(TBS_EX_PROGRESS);
	}
	if (SLDES_SPLITPROGRESS == (SLD_EXT_STYLE)pSldEx->m_eExtendedStyle)
	{
		pChild->SetExtendedStyle(TBS_EX_SPLITPROGRESS);
	}

	pChild->BindStyle(pSldEx);

	// Background images
	bool bSpecifyBackgroundImages = pSldEx->m_bSpecifyBackgroundImages;
	if (!bSpecifyBackgroundImages)
	{
		SetSliderExImagesByParent(pSldEx, pSldEx->m_rcControl, pChild, himgN, himgH, himgS, himgD);
	}

	return pChild;
}

CSkinListCtrl *LibUIDK::CreateListCtrl(LSTCTRLPROPERTIES *pList, CWnd *pParent,
	HIUIIMAGE himgN, HIUIIMAGE himgD)
{
	int nID = LONG(pList->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pList);
	LST_STYLE eListStyle = (LST_STYLE)pList->m_eListStyle;
	switch (eListStyle)
	{
	case LSTS_THUMBNAIL:
	case LSTS_ICON:
		dwStyle |= LVS_ICON;
		break;
	case LSTS_SMALLICON:
		dwStyle |= LVS_SMALLICON;
		break;
	case LSTS_LIST:
		dwStyle |= LVS_LIST;
		break;
	case LSTS_REPORT:
		dwStyle |= LVS_REPORT;
		break;
	default:
		break;
	}
	switch ((LST_ALIGN)pList->m_eListAlign)
	{
	case 0:
		dwStyle |= LVS_ALIGNTOP;
		break;
	case 1:
		dwStyle |= LVS_ALIGNLEFT;
		break;
	default:
		break;
	}
	switch ((LST_SORT)pList->m_eListSort)
	{
	case 0:
		break;
	case LSTST_ASCENDING:
		dwStyle |= LVS_SORTASCENDING;
		break;
	case LSTST_DESCENDING:
		dwStyle |= LVS_SORTDESCENDING;
		break;
	default:
		break;
	}

	if (pList->m_bBorder)
	{
		dwStyle |= WS_BORDER;
	}
	// Is Auto Arrange
	if (pList->m_bAutoArrange)
	{
		dwStyle |= LVS_AUTOARRANGE;
	}
	// Is Edit Labels
	if (pList->m_bEditLabels)
	{
		dwStyle |= LVS_EDITLABELS;
	}
	// Is No Header
	if (pList->m_bNoHeader)
	{
		dwStyle |= LVS_NOCOLUMNHEADER;
	}
	// Is No Label Wrap
	if (pList->m_bNoLabelWrap)
	{
		dwStyle |= LVS_NOLABELWRAP;
	}
	// Is No Scroll
	if (pList->m_bNoScroll)
	{
		dwStyle |= LVS_NOSCROLL;
	}
	// Is No Sort Header
	if (pList->m_bNoSortHeader)
	{
		dwStyle |= LVS_NOSORTHEADER;
	}
	// Is Show Selection Always
	if (pList->m_bShowSelAlways)
	{
		dwStyle |= LVS_SHOWSELALWAYS;
	}
	// Is Single Selection
	if (pList->m_bSingleSel)
	{
		dwStyle |= LVS_SINGLESEL;
	}
	// Is owner data
	if (pList->m_bOwnerData)
	{
		dwStyle |= LVS_OWNERDATA;
	}

	CSkinListCtrl *pChild = new CSkinListCtrl;
	ASSERT(pChild != NULL);

	CRect rect = pList->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pList);

	// Set background images
	bool bSpecifyBackgroundImages = pList->m_bSpecifyBackgroundImages;
	if (!bSpecifyBackgroundImages)
	{
		HIUIIMAGE himgCtrlN = NULL;
		HIUIIMAGE himgCtrlD = NULL;
		CRect rect = pList->m_rcControl;
		IUIGetBackgroundFromParent2(pList, rect, himgN, himgD, &himgCtrlN, &himgCtrlD);
		pChild->SetImages(CONTROL_STATE_UNCHECKED_ND,
			himgCtrlN->GetSafeImageName(), himgCtrlD->GetSafeImageName());
	}

	return pChild;
}

CSkinTreeCtrl *LibUIDK::CreateTreeCtrl(TREECTRLPROPERTIES *pTree, CWnd *pParent,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	int nID = LONG(pTree->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pTree);

	CSkinTreeCtrl *pChild = new CSkinTreeCtrl;
	ASSERT(pChild != NULL);
	RECT rect = pTree->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(&rect);
	}
	if (!pChild->Create(dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pTree);

	// Set bitmaps for background
	bool bSpecifyBackgroundImages = pTree->m_bSpecifyBackgroundImages;
	if (!bSpecifyBackgroundImages)
	{
		HIUIIMAGE hBmpN = NULL;
		HIUIIMAGE hBmpD = NULL;
		CRect rect = pTree->m_rcControl;
		IUIGetBackgroundFromParent2(pTree, rect, himgN, himgD, &hBmpN, &hBmpD);
		pChild->SetImages(CONTROL_STATE_UNCHECKED_ND, hBmpN->GetSafeImageName(), hBmpD->GetSafeImageName());
	}

	return pChild;
}

CScrollBarEx *LibUIDK::CreateScrollBarEx(SBEXPROPERTIES *pSB, CWnd *pParent,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
	int nID = LONG(pSB->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pSB);
	if (pSB->m_bVertical)
	{
		dwStyle |= SBS_VERT;
	}

	CScrollBarEx *pChild = new CScrollBarEx;
	ASSERT(pChild != NULL);

	CRect rect = pSB->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	// Horizontal scroll bar
	if (pSB->m_bVertical)
	{
		pChild->BindStyle((LPCTSTR)pSB->m_strVerScrollbarStyleID);
	}
	else
	{
		pChild->BindStyle((LPCTSTR)pSB->m_strHorScrollbarStyleID);
	}

	return pChild;
}

CThumbnailCtrl *LibUIDK::CreateThumbnail(THMPROPERTIES *pThm, CWnd *pParent,
	HIUIIMAGE bmpN, HIUIIMAGE bmpD)
{
	int nID = LONG(pThm->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pThm);

	// Is List style
	if (pThm->m_bListStyle)
	{
		dwStyle |= LVS_LIST;
	}

	CThumbnailCtrl *pChild = new CThumbnailCtrl;
	ASSERT(pChild != NULL);

	if (!(BOOL)pThm->m_bListStyle)
	{
		HBITMAP hItemBkN = CIUIImage::AutoLoadHBITMAP((LPCTSTR)pThm->m_strItemBkN);
		HBITMAP hItemBkH = CIUIImage::AutoLoadHBITMAP((LPCTSTR)pThm->m_strItemBkH);
		HBITMAP hItemBkS = CIUIImage::AutoLoadHBITMAP((LPCTSTR)pThm->m_strItemBkS);
		HBITMAP hItemBkD = CIUIImage::AutoLoadHBITMAP((LPCTSTR)pThm->m_strItemBkD);
		pChild->SetItemBitmap(hItemBkN, hItemBkH, hItemBkS, hItemBkD);

		// Total Item Size
		CSize sizeTotal;
		sizeTotal.cx = pThm->m_lTotalItemWidth;
		sizeTotal.cy = pThm->m_lTotalItemHeight;
		pChild->SetItemSize(sizeTotal);

		// Normal thumbnail rectangle
		CRect rcNormal;
		rcNormal.left = pThm->m_lThumbnailLeftN;
		rcNormal.top = pThm->m_lThumbnailTopN;
		rcNormal.right = pThm->m_lThumbnailRightN;
		rcNormal.bottom = pThm->m_lThumbnailBottomN;

		// Hight thumbnail rectangle
		CRect rcHight;
		rcHight.left = pThm->m_lThumbnailLeftH;
		rcHight.top = pThm->m_lThumbnailTopH;
		rcHight.right = pThm->m_lThumbnailRightH;
		rcHight.bottom = pThm->m_lThumbnailBottomH;

		// Normal label rectangle
		CRect rcLabelNormal;
		rcLabelNormal.left = pThm->m_lLabelLeftN;
		rcLabelNormal.top = pThm->m_lLabelTopN;
		rcLabelNormal.right = pThm->m_lLabelRightN;
		rcLabelNormal.bottom = pThm->m_lLabelBottomN;

		// Hight label rectangle
		CRect rcLabelHight;
		rcLabelHight.left = pThm->m_lLabelLeftH;
		rcLabelHight.top = pThm->m_lLabelTopH;
		rcLabelHight.right = pThm->m_lLabelRightH;
		rcLabelHight.bottom = pThm->m_lLabelBottomH;
		pChild->SetItemPosition(rcNormal, rcHight, rcLabelNormal, rcLabelHight);
	}
	else
	{
		int nItemHeight = pThm->m_lTotalItemHeight;
		pChild->SetItemHeight(nItemHeight);
	}

	CRect rect = pThm->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	// Hold Ctrl key
	pChild->HoldCtrlKey(pThm->m_bHoldCtrlKey);

	// Enable item drag drop
	pChild->EnableDragDrop(pThm->m_bDragDrop);

	// Background
	HIUIIMAGE hBmpBK[2] = {NULL};
	if (bmpN != NULL)
	{
		CIUIImage::SubBitmap(bmpN, &hBmpBK[0], rect);
	}
	if (bmpD != NULL)
	{
		CIUIImage::SubBitmap(bmpD, &hBmpBK[1], rect);
	}

	for (int i = 0; i < 2 ; i++)
	{
		if (hBmpBK[i]->GetSafeHBITMAP() != NULL)
		{
			hBmpBK[i]->ConvertToDPIImage(GetDPIX(), GetDPIY());
		}
	}

	pChild->SetBkImage(hBmpBK[0]->GetSafeImageName());

	// Scroll bar
	bool bEnableScrollBar = pThm->m_bEnableScrollBar;
	pChild->EnableScrollBar(bEnableScrollBar);
	if (bEnableScrollBar)
	{
		// Vertical scroll bar
		{
			CScrollBarEx *pVertScroll = pChild->GetScrollBarEx();
			pVertScroll->BindStyle((LPCTSTR)pThm->m_strVerScrollbarID);
		}
	}

	return pChild;
}

CTreeListCtrl *LibUIDK::CreateTreeList(TREELISTPROPERTIES *pTreeList, CWnd *pParent,
	HIUIIMAGE himgN, HIUIIMAGE himgD)
{
	int nID = LONG(pTreeList->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pTreeList);

	CTreeListCtrl *pChild = new CTreeListCtrl;
	ASSERT(pChild != NULL);

	CRect rect = pTreeList->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (pTreeList->m_bVariableItemHeight)
	{
		if (!pChild->Create(dwStyle, rect, pParent, nID, 24, TRUE, 16))
		{
			delete pChild;
			return NULL;
		}
	}
	else
	{
		if (!pChild->Create(dwStyle, rect, pParent, nID))
		{
			delete pChild;
			return NULL;
		}
	}

	pChild->BindStyle(pTreeList);

	bool bSpecifyBackgroundImages = pTreeList->m_bSpecifyBackgroundImages;
	if (!bSpecifyBackgroundImages)
	{
		HIUIIMAGE hBmpN = NULL;
		HIUIIMAGE hBmpD = NULL;
		CRect rect = pTreeList->m_rcControl;
		IUIGetBackgroundFromParent2(pTreeList, rect, himgN, himgD, &hBmpN, &hBmpD);
		pChild->SetBkImage(CONTROL_STATE_UNCHECKED_ND, hBmpN->GetSafeImageName(), hBmpD->GetSafeImageName());
	}

	return pChild;
}

CString g_strRichEditClassName;
CString LibUIDK::GetRichEditClassName()
{
	if (g_strRichEditClassName.IsEmpty())
	{
		g_strRichEditClassName = RICHEDIT_CLASS;
	}

	return g_strRichEditClassName;
}

CSkinRichEditCtrl *LibUIDK::CreateRichEdit(RICHEDITPROPERTIES *pRichEdt, CWnd *pParent,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
#if(_MSC_VER > 1200) // VC6.0
	if (!AfxInitRichEdit2())
#else
	if (!AfxInitRichEdit())
#endif
	{
		return NULL;
	}

	int nID = LONG(pRichEdt->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pRichEdt);
	if (pRichEdt->m_bHScroll)
	{
		dwStyle |= WS_HSCROLL;
	}
	if (pRichEdt->m_bVScroll)
	{
		dwStyle |= WS_VSCROLL;
	}
	if (pRichEdt->m_bWantReturn)
	{
		dwStyle |= ES_WANTRETURN;
	}
	if (pRichEdt->m_bAutoHScroll)
	{
		dwStyle |= ES_AUTOHSCROLL;
	}
	if (pRichEdt->m_bAutoVScroll)
	{
		dwStyle |= ES_AUTOVSCROLL;
	}
	if (pRichEdt->m_bLowercase)
	{
		dwStyle |= ES_LOWERCASE;
	}
	if (pRichEdt->m_bUppercase)
	{
		dwStyle |= ES_UPPERCASE;
	}
	if (pRichEdt->m_bNumber)
	{
		dwStyle |= ES_NUMBER;
	}
	if (pRichEdt->m_bPassword)
	{
		dwStyle |= ES_PASSWORD;
	}
	if (pRichEdt->m_bReadOnly)
	{
		dwStyle |= ES_READONLY;
	}
	if (pRichEdt->m_bMultiline)
	{
		dwStyle |= ES_MULTILINE;
	}

	CSkinRichEditCtrl *pChild = new CSkinRichEditCtrl();
	ASSERT(pChild != NULL);

	CRect rect = pRichEdt->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}

#if(_MSC_VER <= 1200) // VC6.0
	if (!pChild->CreateEx(WS_EX_TRANSPARENT | WS_EX_NOPARENTNOTIFY,
			_T("RICHEDIT20A"), _T(""), dwStyle, rect, pParent, nID))
#else
	if (!pChild->CreateEx(WS_EX_TRANSPARENT | WS_EX_NOPARENTNOTIFY, dwStyle, rect, pParent, nID))
#endif // (_MSC_VER <= 1200)
	{
		delete pChild;
		return NULL;
	}

	// 由于使用不同的VS，创建的RichEdit的类名也不同，所以，把这个类名记录下来，
	// 在UIWnd绘制控件的时候，需要根据这个类名，查找RichEdit，进而绘制RichEdit。
	GetClassName(pChild->GetSafeHwnd(), g_strRichEditClassName.GetBufferSetLength(MAX_PATH), MAX_PATH);
	g_strRichEditClassName.ReleaseBuffer();

	pChild->BindStyle(pRichEdt);

	return pChild;
}

CIMRichEditCtrl *LibUIDK::CreateIMRichEdit(IMREPROPERTIES *pRichEdt, CWnd *pParent,
	HIUIIMAGE himgN, HIUIIMAGE himgH, HIUIIMAGE himgS, HIUIIMAGE himgD)
{
#if(_MSC_VER > 1200) // VC6.0
	if (!AfxInitRichEdit2())
#else
	if (!AfxInitRichEdit())
#endif
	{
		return NULL;
	}

	int nID = LONG(pRichEdt->m_nID);
	ASSERT(nID != -1);

	// 制作IM聊天记录控件时，RichEdit恒设为禁止水平滚动、只读、多行、非password、非数字
	// Style
	DWORD dwStyle = WS_CHILD | WS_VISIBLE;
	dwStyle |= WS_VSCROLL;
	dwStyle |= ES_AUTOVSCROLL;
	dwStyle |= ES_MULTILINE;
	dwStyle |= ES_LEFT;
	dwStyle |= ES_WANTRETURN;
	dwStyle &= ~ES_AUTOHSCROLL;
	dwStyle &= ~ES_LOWERCASE;
	dwStyle &= ~ES_UPPERCASE;

	// 加了只读属性后，就无法设置左缩进
	//	dwStyle |= ES_READONLY;


	CIMRichEditCtrl *pChild = new CIMRichEditCtrl();
	ASSERT(pChild != NULL);

	CRect rect = pRichEdt->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}

#if(_MSC_VER <= 1200) // VC6.0
	if (!pChild->CreateEx(WS_EX_TRANSPARENT | WS_EX_NOPARENTNOTIFY,
			_T("RICHEDIT20A"), _T(""), dwStyle, rect, pParent, nID))
#else
	if (!pChild->CreateEx(WS_EX_TRANSPARENT, dwStyle, rect, pParent, nID))
#endif // (_MSC_VER <= 1200)
	{
		delete pChild;
		return NULL;
	}

	// 由于使用不同的VS，创建的RichEdit的类名也不同，所以，把这个类名记录下来，
	// 在UIWnd绘制控件的时候，需要根据这个类名，查找RichEdit，进而绘制RichEdit。
	GetClassName(pChild->GetSafeHwnd(), g_strRichEditClassName.GetBufferSetLength(MAX_PATH), MAX_PATH);
	g_strRichEditClassName.ReleaseBuffer();

	pChild->CIMRichEditCtrlBase::BindStyle(pRichEdt);

	return pChild;
}

CHtmlCtrl *LibUIDK::CreateHtmlCtrl(HTMLCTRLPROPERTIES *pHtmlCtrl, CWnd *pParent)
{
	int nID = LONG(pHtmlCtrl->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pHtmlCtrl);

	CHtmlCtrl *pChild = new CHtmlCtrl;
	ASSERT(pChild != NULL);

	CRect rect = pHtmlCtrl->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(NULL, NULL, dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pHtmlCtrl);

	return pChild;
}

CSkinDateTimeCtrl *LibUIDK::CreateDateTimeCtrl(DATETIMECTRLPROPERTIES *pDateTimeCtrl, CWnd *pParent)
{
	int nID = LONG(pDateTimeCtrl->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pDateTimeCtrl);
	DATETIME_FORMAT eFormat = (DATETIME_FORMAT)pDateTimeCtrl->m_eFormat;
	if (eFormat == DTF_SHORT_DATE)
	{
		dwStyle |= DTS_SHORTDATEFORMAT;
	}
	else if (eFormat == DTF_LONG_DATE)
	{
		dwStyle |= DTS_LONGDATEFORMAT;
	}
	else if (eFormat == DTF_TIME)
	{
		dwStyle |= DTS_TIMEFORMAT;
	}
	else
	{
		ASSERT(FALSE);
	}

	if (pDateTimeCtrl->m_bAllowEdit)
	{
		dwStyle |= DTS_APPCANPARSE;
	}
	if (pDateTimeCtrl->m_bShowNone)
	{
		dwStyle |= DTS_SHOWNONE;
	}
	if (pDateTimeCtrl->m_bUseSpinControl)
	{
		dwStyle |= DTS_UPDOWN;
	}

	CSkinDateTimeCtrl *pChild = new CSkinDateTimeCtrl;
	ASSERT(pChild != NULL);

	CRect rect = pDateTimeCtrl->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pDateTimeCtrl);

	return pChild;
}

CWaveCtrl *LibUIDK::CreateWaveCtrl(WAVECTRLPROPERTIES *pWaveCtrl, CWnd *pParent,
	HIUIIMAGE hbmpN, HIUIIMAGE hbmpH, HIUIIMAGE hbmpS, HIUIIMAGE hbmpD)
{
	int nID = LONG(pWaveCtrl->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pWaveCtrl);

	CWaveCtrl *pChild = new CWaveCtrl;
	ASSERT(pChild != NULL);

	CRect rect = pWaveCtrl->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pWaveCtrl);

	// Set bitmaps for background
	bool bSpecifyBackgroundImages = pWaveCtrl->m_bSpecifyBackgroundImages;
	if (!bSpecifyBackgroundImages)
	{
		HIUIIMAGE hBmpN = NULL;
		HIUIIMAGE hBmpD = NULL;
		CRect rect = pWaveCtrl->m_rcControl;
		IUIGetBackgroundFromParent2(pWaveCtrl, rect, hBmpN, hbmpD, &hBmpN, &hBmpD);
		pChild->SetBkImage(CONTROL_STATE_UNCHECKED_ND,
			hBmpN->GetSafeImageName(), hBmpD->GetSafeImageName());
	}

	return pChild;
}

CPictureCtrl *LibUIDK::CreatePictureCtrl(PICWLPROPERTIES *pPicCtrl, CWnd *pParent,
	HIUIIMAGE hbmpN, HIUIIMAGE hbmpH, HIUIIMAGE hbmpS, HIUIIMAGE hbmpD)
{
	int nID = LONG(pPicCtrl->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pPicCtrl);

	CPictureCtrl *pChild = new CPictureCtrl;
	ASSERT(pChild != NULL);

	CRect rect = pPicCtrl->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pPicCtrl);

	return pChild;
}


CMenuBar *LibUIDK::CreateMenuBar(MENUBARPROPERTIES *pMenuBar, CWnd *pParent)
{
	int nID = LONG(pMenuBar->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pMenuBar);

	CMenuBar *pChild = new CMenuBar;
	ASSERT(pChild != NULL);

	//////////////////////////////////////////////////////////////////////////
	// Set menu item properties before the menu bar be created.

	// Set menu border
	MENUPROPERTIES menuPropDefault;
	MENUPROPERTIES *pMenuProp = (MENUPROPERTIES *)CUIMgr::GetStyleItem(STYLET_MENU,
			(LPCTSTR)pMenuBar->m_strMenuStyleID);
	if (pMenuProp == NULL)
	{
		pMenuProp = &menuPropDefault;
	}

	// Bind menu style
	pChild->BindMenuStyle(pMenuProp);

	//////////////////////////////////////////////////////////////////////////
	// For menu bar's button

	// Set bitmaps for menu bar's button background
	if (pMenuBar->m_bCombineBackgroundImages)
	{
		pChild->SetMenuButtonImages((LPCTSTR)pMenuBar->m_strBKCombine);
		pChild->SetBkImageResizeMode(pMenuBar->m_ptImageResize);
	}
	else
	{
		pChild->SetMenuButtonImages(CONTROL_STATE_UNCHECKED_ALL,
			(LPCTSTR)pMenuBar->m_strBKN, (LPCTSTR)pMenuBar->m_strBKH,
			(LPCTSTR)pMenuBar->m_strBKS, (LPCTSTR)pMenuBar->m_strBKD);

		pChild->SetMenuButtonCheckedImages(CONTROL_STATE_CHECKED_ALL,
			(LPCTSTR)pMenuBar->m_strBKCN, (LPCTSTR)pMenuBar->m_strBKCH,
			(LPCTSTR)pMenuBar->m_strBKCS, (LPCTSTR)pMenuBar->m_strBKCD);

		pChild->SetBkImageResizePoint(pMenuBar->m_ptImageResize);
	}

	// Set color for menu bar's button
	COLORREF cr[4] = {0};
	CTRLPROPERTIES::IUIGetControlColor4(pMenuBar, &cr[0], &cr[1], &cr[2], &cr[3]);
	pChild->SetMenuButtonTextColor(cr[0], cr[1], cr[2], cr[3]);

	// Set font for menu bar's button
	pChild->SetMenuButtonTextFont(pMenuBar->m_strResFontIDN, pMenuBar->m_strResFontIDH,
		pMenuBar->m_strResFontIDS, pMenuBar->m_strResFontIDD);

	CRect rect = pMenuBar->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	return pChild;
}

CSplitterBar *LibUIDK::CreateSplitter(SPLITTERPROPERTIES *pSplitter, CWnd *pParent)
{
	int nID = LONG(pSplitter->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pSplitter);

	CSplitterBar *pChild = new CSplitterBar;
	ASSERT(pChild != NULL);

	CRect rect = pSplitter->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(dwStyle, rect, pParent, nID))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pSplitter);

	return pChild;
}

CRectCtrl *LibUIDK::CreateWLRect(RECTWLPROPERTIES *pRectCtrl, CWnd *pParent)
{
	int nID = LONG(pRectCtrl->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pRectCtrl);;

	CRectCtrl *pChild = new CRectCtrl;
	ASSERT(pChild != NULL);

	// CWLWnd::m_bFlag: TRUE: the windowless control created by user; FALSE: created by LIBUIDK.
	// If the control create by LIBUIDK, its memory be new and need free.
	pChild->m_bFlag = FALSE;

	CRect rect = pRectCtrl->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(NULL, dwStyle, rect, pParent, nID, NULL))
	{
		delete pChild;
		return NULL;
	}

	return pChild;
}

CWLSplitterBar *LibUIDK::CreateWLSplitter(SPLITTERWLPROPERTIES *pSplitter, CWnd *pParent)
{
	int nID = LONG(pSplitter->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pSplitter);

	CWLSplitterBar *pChild = new CWLSplitterBar;
	ASSERT(pChild != NULL);

	pChild->m_bFlag = FALSE;
	CRect rect = pSplitter->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(NULL, dwStyle, rect, pParent, nID, NULL))
	{
		delete pChild;
		return NULL;
	}

	return pChild;
}

CWLLine *LibUIDK::CreateWLLine(LINEWLPROPERTIES *pLine, CWnd *pParent)
{
	int nID = LONG(pLine->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pLine);

	CWLLine *pChild = new CWLLine;
	ASSERT(pChild != NULL);

	pChild->m_bFlag = FALSE;
	CRect rect = pLine->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(NULL, dwStyle, rect, pParent, nID, NULL))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pLine);

	return pChild;
}

CWLText *LibUIDK::CreateWLText(TXTPROPERTIES *pText, CWnd *pParent)
{
	int nID = LONG(pText->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pText);

	CWLText *pChild = new CWLText;
	ASSERT(pChild != NULL);

	pChild->m_bFlag = FALSE;
	CRect rect = pText->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(pText->m_strCaption, dwStyle, rect, pParent, nID, NULL))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pText);

	return pChild;
}

CWLButton *LibUIDK::CreateWLButton(BTNPROPERTIES *pBtn, CWnd *pParent)
{
	int nID = LONG(pBtn->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pBtn);

	CWLButton *pChild = new CWLButton;
	ASSERT(pChild != NULL);

	pChild->m_bFlag = FALSE;
	CRect rect = pBtn->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}

	if (!pChild->Create(pBtn->m_strCaption, dwStyle, rect, pParent, nID, NULL))
	{
		delete pChild;
		return NULL;
	}

	pChild->CButtonBase::BindStyle(pBtn);

	return pChild;
}

CWLButton *LibUIDK::CreateWLCheck(CHKPROPERTIES *pChk, CWnd *pParent)
{
	int nID = LONG(pChk->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | BS_AUTOCHECKBOX | GetControlBaseStyle(pChk);

	CWLButton *pChild = new CWLButton;
	ASSERT(pChild != NULL);

	pChild->m_bFlag = FALSE;
	CRect rect = pChk->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(pChk->m_strCaption, dwStyle, rect, pParent, nID, NULL))
	{
		delete pChild;
		return NULL;
	}

	pChild->CButtonBase::BindStyle(pChk);

	return pChild;
}

CWLButton *LibUIDK::CreateWLRadio(RADPROPERTIES *pRad, CWnd *pParent)
{
	int nID = LONG(pRad->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | BS_AUTORADIOBUTTON | GetControlBaseStyle(pRad);

	CWLButton *pChild = new CWLButton;
	ASSERT(pChild != NULL);

	pChild->m_bFlag = FALSE;
	CRect rect = pRad->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(pRad->m_strCaption, dwStyle, rect, pParent, nID, NULL))
	{
		delete pChild;
		return NULL;
	}

	pChild->CButtonBase::BindStyle(pRad);

	return pChild;
}

CWLSliderCtrl *LibUIDK::CreateWLSlider(SLDEXPROPERTIES *pWLSld, CWnd *pParent)
{
	int nID = LONG(pWLSld->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pWLSld);

	CWLSliderCtrl *pChild = new CWLSliderCtrl;
	ASSERT(pChild != NULL);

	pChild->m_bFlag = FALSE;
	CRect rect = pWLSld->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(dwStyle, rect, pParent, nID, NULL))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pWLSld);

	return pChild;
}

CWLPicture *LibUIDK::CreateWLPic(PICWLPROPERTIES *pPic, CWnd *pParent)
{
	int nID = LONG(pPic->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pPic);

	CWLPicture *pChild = new CWLPicture;
	ASSERT(pChild != NULL);

	pChild->m_bFlag = FALSE;
	CRect rect = pPic->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(NULL, dwStyle, rect, pParent, nID, NULL))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pPic);

	return pChild;
}

CWLRichEditCtrl *LibUIDK::CreateWLRichEdit(RICHEDITPROPERTIES *pRichEdt, CWnd *pParent)
{
	int nID = LONG(pRichEdt->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pRichEdt);

	CWLRichEditCtrl *pChild = new CWLRichEditCtrl;
	ASSERT(pChild != NULL);

	pChild->m_bFlag = FALSE;
	CRect rect = pRichEdt->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(dwStyle, rect, pParent, nID, NULL))
	{
		delete pChild;
		return NULL;
	}

	pChild->CRichEditCtrlBase::BindStyle(pRichEdt);

	return pChild;
}

CWLIMRichEditCtrl *LibUIDK::CreateWLIMRichEdit(IMREPROPERTIES *pIMRichEdt, CWnd *pParent)
{
	int nID = LONG(pIMRichEdt->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pIMRichEdt);

	CWLIMRichEditCtrl *pChild = new CWLIMRichEditCtrl;
	ASSERT(pChild != NULL);

	pChild->m_bFlag = FALSE;
	CRect rect = pIMRichEdt->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->CWLRichEditCtrl::Create(dwStyle, rect, pParent, nID, NULL))
	{
		delete pChild;
		return NULL;
	}

#if(_MSC_VER > 1200) // VC6.0
	pChild->CWLRichEditCtrl::CRichEditCtrlBase::BindStyle(pIMRichEdt);
#else
	pChild->CRichEditCtrlBase::BindStyle(pIMRichEdt);
#endif

	return pChild;
}

CTaskWndMgr *LibUIDK::CreateTaskWndMgr(TWMGRPROPERTIES *pTaskWndMgrProp, CWnd *pParent)
{
	int nID = LONG(pTaskWndMgrProp->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pTaskWndMgrProp);

	CTaskWndMgr *pChild = new CTaskWndMgr;
	ASSERT(pChild != NULL);

	pChild->m_bFlag = FALSE;
	CRect rect = pTaskWndMgrProp->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(NULL, dwStyle, rect, pParent, nID, NULL))
	{
		delete pChild;
		return NULL;
	}

	return pChild;
}

CDockPanel *LibUIDK::CreatePanelDock(DOCKPNLPROPERTIES *pDockPanelProp, CWnd *pParent)
{
	int nID = LONG(pDockPanelProp->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pDockPanelProp);

	CDockPanel *pChild = new CDockPanel;
	ASSERT(pChild != NULL);

	pChild->m_bFlag = FALSE;
	CRect rect = pDockPanelProp->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->CWLWnd::Create(_T("IUIWL_DockPanel"), NULL, dwStyle, rect, pParent, nID, NULL))
	{
		delete pChild;
		return NULL;
	}

	pChild->SetLastChildFill((bool)pDockPanelProp->m_bLastChildFill);

	return pChild;
}

CUniformGridPanel *LibUIDK::CreatePanelUniformGrid(
	UGRIDPNLPROPERTIES *pUGridPanelProp,
	CWnd *pParent)
{
	int nID = LONG(pUGridPanelProp->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pUGridPanelProp);

	CUniformGridPanel *pChild = new CUniformGridPanel;
	ASSERT(pChild != NULL);

	pChild->m_bFlag = FALSE;
	CRect rect = pUGridPanelProp->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(NULL, dwStyle, rect, pParent, nID, NULL))
	{
		delete pChild;
		return NULL;
	}

	return pChild;
}

CStackPanel *LibUIDK::CreatePanelStack(STACKPNLPROPERTIES *pStackPanelProp, CWnd *pParent)
{
	int nID = LONG(pStackPanelProp->m_nID);
	ASSERT(nID != -1);

	// Style
	DWORD dwStyle = WS_CHILD | GetControlBaseStyle(pStackPanelProp);

	CStackPanel *pChild = new CStackPanel;
	ASSERT(pChild != NULL);

	pChild->m_bFlag = FALSE;
	CRect rect = pStackPanelProp->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rect);
	}
	if (!pChild->Create(NULL, dwStyle, rect, pParent, nID, NULL))
	{
		delete pChild;
		return NULL;
	}

	pChild->BindStyle(pStackPanelProp);

	return pChild;
}
