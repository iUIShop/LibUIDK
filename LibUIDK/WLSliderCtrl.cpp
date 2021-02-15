// WLSliderCtrl.cpp : implementation file
//

#include "stdafx.h"
#if(_MSC_VER >= 1600) // VC2010
#include <iterator>		// For checked_array_iterator
#endif
#include <map>
#include "ControlMember.h"
#include "ResourceMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define IDT_ANIMATE_THUMB		1730


struct WLSLIDERMEMBER : public SLIDEREXMEMBER
{
	WLSLIDERMEMBER()
	{
		m_bAnimationMode = false;
		m_nCurTimerTick = 0;
		m_nTimerElapse = 40;
	}

	int Release()
	{
		m_bAnimationMode = false;
		m_nCurTimerTick = 0;
		m_nTimerElapse = 40;
		m_vAnimationData.clear();

		return SLIDEREXMEMBER::Release();
	}

	// The following variable for animating image.
	bool m_bAnimationMode;
	int m_nCurTimerTick;					// Times that timer implement. same as current Frame.
	int m_nTimerElapse;						// See SetTimer.
	std::vector<int> m_vAnimationData;
};

/////////////////////////////////////////////////////////////////////////////
// CWLSliderCtrl

CWLSliderCtrl::CWLSliderCtrl()
{
	m_pMember = new WLSLIDERMEMBER;
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;
}

CWLSliderCtrl::~CWLSliderCtrl()
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	if (m_pMember != NULL)
	{
		delete (WLSLIDERMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

int CWLSliderCtrl::BindStyle(LPCTSTR lpszStyleID)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	if (!IsCreated())
	{
		ASSERT(FALSE);
		return -1;
	}

	if (lpszStyleID == NULL)
	{
		return -2;
	}

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_SLIDEREX, lpszStyleID);
	return BindStyle(pCtrlProp);
}

int CWLSliderCtrl::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

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
	if (ect != CT_STYLE_SLIDEREX && ect != CT_SLIDEREX && ect != CT_WL_SLIDER)
	{
		return -3;
	}

	SLDEXPROPERTIES *pSldExProp = (SLDEXPROPERTIES *)pCtrlProp;

	// Background images
	if (pSldExProp->m_bSpecifyBackgroundImages)
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
			SetImages(CONTROL_STATE_CHECKED_ALL,
				strImageName[1], strImageName[2], strImageName[3], strImageName[4]);
		}
	}

	// Thumb images
	CString strImageName[1 + COMBINEIMAGESIZE4];
	BOOL bCombineImage = TRUE;
	CTRLPROPERTIES::IUIGetThumbImages4(pSldExProp, &bCombineImage, strImageName);
	if (bCombineImage)
	{
		SetThumbCombineImage(strImageName[0]);
	}
	else
	{
		SetThumbImages(CONTROL_STATE_UNCHECKED_ALL,
			strImageName[1], strImageName[2], strImageName[3], strImageName[4]);
	}

	// General tool tip
	if (pSldExProp->m_bUseToolTip)
	{
		EnableToolTipsEx(TRUE);
	}
	// Custom tip window
	if (pSldExProp->m_bEnableTipWindow)
	{
		EnableTipWindow(TRUE);
		POINT pt = pSldExProp->m_sizeTipWindowOffset;
		SIZE size;
		size.cx = pt.x;
		size.cy = pt.y;
		SetTipWindowOffset(&size);
	}

	// mark
	UseMarkBitmap(pSldExProp->m_bUseMark);
	MarkImageToThumb(pSldExProp->m_bMarkToThumb);

	return 0;
}

BOOL CWLSliderCtrl::Create(DWORD dwStyle, const RECT &rect,
	CWnd *pParentWnd, UINT nID, CWLWnd *pVirtualParent)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	BOOL bRet = CWLWnd::Create(_T("IUIWL_Slider"), NULL,
			dwStyle, rect, pParentWnd, nID, pVirtualParent);
	if (!bRet)
	{
		return FALSE;
	}

	pMember->m_dwStyle = dwStyle;

	return TRUE;
}

DWORD CWLSliderCtrl::GetExtendedStyle()
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	return pMember->m_dwStyleEx;
}

DWORD CWLSliderCtrl::SetExtendedStyle(DWORD dwNewStyle)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	DWORD dwOld = pMember->m_dwStyleEx;
	pMember->m_dwStyleEx = dwNewStyle;
	return dwOld;
}

void CWLSliderCtrl::SetThumbSize(SIZE sizeThumb)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	pMember->m_sizeThumb = sizeThumb;
}

// Set the deflate value for slider range
int CWLSliderCtrl::SetDeflate(UINT nLeftTop, UINT nRightBottom)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	pMember->m_nDeflateLTUP = nLeftTop;
	pMember->m_nDeflateRTDN = nRightBottom;

	if (CUIMgr::IsEnableDPI())
	{
		long lDeflateLTUP = pMember->m_nDeflateLTUP;
		GetDPIPointX(lDeflateLTUP);
		pMember->m_nDeflateLTUP = lDeflateLTUP;

		long lDeflateRTDN = pMember->m_nDeflateRTDN;
		GetDPIPointX(lDeflateRTDN);
		pMember->m_nDeflateRTDN = lDeflateRTDN;
	}

	return 0;
}

int CWLSliderCtrl::GetDeflate(UINT *pnLeftTop, UINT *pnRightBottom)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	if (pnLeftTop != NULL)
	{
		*pnLeftTop = pMember->m_nDeflateLTUP;
	}
	if (pnRightBottom != NULL)
	{
		*pnRightBottom = pMember->m_nDeflateRTDN;
	}

	return 0;
}

int CWLSliderCtrl::GetPos() const
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	return pMember->m_nPos;
}

void CWLSliderCtrl::SetPos(int nPos)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	NMSLIDERCTRL sldn;
	sldn.nOldPos = pMember->m_nPos;

	if (nPos > pMember->m_nMax)
	{
		pMember->m_nPos = pMember->m_nMax;
	}
	else if (nPos < pMember->m_nMin)
	{
		pMember->m_nPos = pMember->m_nMin;
	}
	else
	{
		pMember->m_nPos = nPos;
	}

	Invalidate();

	NMHDR nmhdr;
	nmhdr.hwndFrom = GetParent()->GetSafeHwnd();
	nmhdr.idFrom = GetDlgCtrlID();
	nmhdr.code = SLDN_POS_CHANGED;

	sldn.hdr = nmhdr;
	sldn.nNewPos = pMember->m_nPos;

	GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&sldn));
}

int CWLSliderCtrl::GetProgressPos() const
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	return pMember->m_nProgressPos;
}

void CWLSliderCtrl::SetProgressPos(int nPos)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	pMember->m_nProgressPos = nPos;
	Invalidate();
}

void CWLSliderCtrl::GetThumbRect(LPRECT lprc) const
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	if (pMember->m_dwStyle & TBS_VERT)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		int nScrollRange = rcClient.Height()
			- pMember->m_nDeflateLTUP - pMember->m_nDeflateRTDN - pMember->m_sizeThumb.cy;
		int nY = 0;
		if (pMember->m_nMax != pMember->m_nMin)
		{
			nY = pMember->m_nDeflateLTUP
				+ nScrollRange * (pMember->m_nMax - pMember->m_nPos) / (pMember->m_nMax - pMember->m_nMin);
		}
		lprc->left = (rcClient.Width() - pMember->m_sizeThumb.cx) / 2;
		lprc->top = nY;
		lprc->right = lprc->left + pMember->m_sizeThumb.cx;
		lprc->bottom = lprc->top + pMember->m_sizeThumb.cy;
	}
	else
	{
		CRect rcClient;
		GetClientRect(rcClient);
		int nScrollRange = rcClient.Width() - pMember->m_nDeflateLTUP
			- pMember->m_nDeflateRTDN - pMember->m_sizeThumb.cx;
		int nX = 0;
		if (pMember->m_nMax != pMember->m_nMin)
		{
			nX = pMember->m_nDeflateLTUP
				+ nScrollRange * pMember->m_nPos / (pMember->m_nMax - pMember->m_nMin);
		}
		lprc->left = nX;
		lprc->top = (rcClient.Height() - pMember->m_sizeThumb.cy) / 2;
		lprc->right = lprc->left + pMember->m_sizeThumb.cx;
		lprc->bottom = lprc->top + pMember->m_sizeThumb.cy;
	}
}

void CWLSliderCtrl::SetRange(int nMin, int nMax, BOOL bRedraw/* = FALSE*/)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	pMember->m_nMin = nMin;
	pMember->m_nMax = nMax;
	if (bRedraw)
	{
		Invalidate();
	}
}

void CWLSliderCtrl::GetRange(int &nMin, int &nMax) const
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	nMin = pMember->m_nMin;
	nMax = pMember->m_nMax;
}

int CWLSliderCtrl::SetImages(UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameMarkN,
	LPCTSTR lpszImageNameMarkD, LPCTSTR lpszImageNameD)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	ASSERT(IsCreated());

	m_bBkCombine = false;
	IUISetControlImage4(uMask, pMember->m_himgBk,
		lpszImageNameN, lpszImageNameMarkN, lpszImageNameMarkD, lpszImageNameD);

	return 0;
}

int CWLSliderCtrl::GetImages(UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameH,
	CString *pstrImageNameS, CString *pstrImageNameD)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bBkCombine;
	IUIGetControlImage4(uMask, m_himgCombineBk, pMember->m_himgBk, *pbCombineImage,
		pstrCombineImageName,
		pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);

	return 0;
}

int CWLSliderCtrl::SetThumbCombineImage(LPCTSTR lpszImageNameThumbCombine, BOOL bRedraw/* = TRUE*/)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	pMember->m_bThumbCombine = true;

	IUISetControlImage(&pMember->m_himgCombineThumb, lpszImageNameThumbCombine);

	// set the size of thumb
	if (pMember->m_himgCombineThumb != NULL)
	{
		pMember->m_sizeThumb.cx = pMember->m_himgCombineThumb->GetWidth() / 4;
		pMember->m_sizeThumb.cy = pMember->m_himgCombineThumb->GetHeight();
	}

	if (bRedraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CWLSliderCtrl::SetThumbImages(UINT uMask,
	LPCTSTR lpszImageNameThumbN, LPCTSTR lpszImageNameThumbH,
	LPCTSTR lpszImageNameThumbS, LPCTSTR lpszImageNameThumbD)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	pMember->m_bThumbCombine = false;

	ASSERT(IsCreated());

	IUISetControlImage4(uMask, pMember->m_himgThumb,
		lpszImageNameThumbN, lpszImageNameThumbH, lpszImageNameThumbS, lpszImageNameThumbD);

	// set the size of thumb
	if (pMember->m_himgThumb[0] != NULL)
	{
		pMember->m_sizeThumb.cx = pMember->m_himgThumb[0]->GetWidth();
		pMember->m_sizeThumb.cy = pMember->m_himgThumb[0]->GetHeight();
	}

	return 0;
}

int CWLSliderCtrl::GetThumbImages(UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameH,
	CString *pstrImageNameS, CString *pstrImageNameD)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = pMember->m_bThumbCombine;
	IUIGetControlImage4(uMask, pMember->m_himgCombineThumb,
		pMember->m_himgThumb, pMember->m_bThumbCombine, pstrCombineImageName,
		pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);

	return 0;
}

int CWLSliderCtrl::UseMarkBitmap(BOOL bUseMark)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	pMember->m_bUseMark = bUseMark;

	return 0;
}

BOOL CWLSliderCtrl::IsUseMarkBitmap() const
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	return pMember->m_bUseMark;
}

int CWLSliderCtrl::MarkImageToThumb(BOOL bToThumb)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	pMember->m_bMarkToThumb = bToThumb;

	return 0;
}

BOOL CWLSliderCtrl::IsMarkImageToThumb() const
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	return pMember->m_bMarkToThumb;
}

// only for TBS_EX_SPLITPROGRESS style
int CWLSliderCtrl::SetSplitCount(int nCount)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	if (nCount > pMember->m_nSplitCount)
	{
		SafeDelete(pMember->m_pnSplitList);
		pMember->m_pnSplitList = new int [nCount];
	}

	pMember->m_nSplitCount = nCount;
	return 0;
}

int CWLSliderCtrl::GetSplitCount()
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	return pMember->m_nSplitCount;
}

int CWLSliderCtrl::SetSplitList(const int *pnFirst, const int *pnLast, BOOL bRedraw/*=TRUE*/)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	if (pnLast - pnFirst < pMember->m_nSplitCount)
	{
		return -1;
	}

	if (pMember->m_pnSplitList == NULL) // first init
	{
		pMember->m_pnSplitList = new int[pMember->m_nSplitCount];
	}

#if(_MSC_VER <= 1310) // VC6.0 or VC7.1
	std::copy(pnFirst, pnFirst + pMember->m_nSplitCount, pMember->m_pnSplitList);
#else
	std::copy(pnFirst, pnFirst + pMember->m_nSplitCount,
		stdext::checked_array_iterator<int *>(pMember->m_pnSplitList, pMember->m_nSplitCount));
#endif

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CWLSliderCtrl::AddColorImage(int nValue, HBITMAP hbmpColor)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	pMember->m_ColorMap[nValue] = hbmpColor;

	return 0;
}

int CWLSliderCtrl::RemoveColorImage(int nValue)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	pMember->m_ColorMap.erase(nValue);

	return 0;
}

void CWLSliderCtrl::DrawVertSlider(CDC *pMemDCParent)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	CRect rcWin;
	GetWindowRect(rcWin);
	GetParent()->ScreenToClient(rcWin);

	// Don't draw this control if it's out of the parent
	CRect rcParent;
	GetParent()->GetClientRect(rcParent);

	CRect rcIntersect;
	rcIntersect.IntersectRect(rcWin, rcParent);
	if (rcIntersect.IsRectEmpty())
	{
		return;
	}

	CRect rcThumb;
	if (pMember->m_bCaptureThumb)
	{
		rcThumb = pMember->m_rcTrackingThumb;
	}
	else
	{
		GetThumbRect(rcThumb);
	}

	// draw background
	BOOL bEnable = IsWindowEnabled();

	if (m_bBkCombine)
	{
		IUIPartNineGridBlt(pMemDCParent->GetSafeHdc(),
			rcWin.left, rcWin.top, rcWin.Width(), rcWin.Height(),
			m_himgCombineBk,
			m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
			4, bEnable ? 0 : 3);
	}
	else
	{
		IUINineGridBlt(pMemDCParent->GetSafeHdc(),
			rcWin.left, rcWin.top, rcWin.Width(), rcWin.Height(),
			bEnable ? pMember->m_himgBk[0] : pMember->m_himgBk[3],
			m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
	}

	// draw split part
	if ((pMember->m_dwStyleEx & TBS_EX_SPLITPROGRESS) == TBS_EX_SPLITPROGRESS)
	{
		for (int i = 0; i < pMember->m_nSplitCount && pMember->m_pnSplitList + i != NULL; ++i)
		{
			HBITMAP hbmp = pMember->m_ColorMap[*(pMember->m_pnSplitList + i)];

			int nTop = rcWin.Height() * (pMember->m_nSplitCount - i - 1) / pMember->m_nSplitCount;
			int nHeight = rcWin.Height() * (pMember->m_nSplitCount - i) / pMember->m_nSplitCount - nTop;
			NineGridBltG(pMemDCParent->GetSafeHdc(), rcWin.left, rcWin.top + nTop, rcWin.Width(), nHeight,
				hbmp, CPoint(1, 1), 1, 1);
		}
	}

	// draw down part
	if (pMember->m_bUseMark)
	{
		int nUpPartHeight = 0;
		if ((pMember->m_dwStyleEx & TBS_EX_PROGRESS) == TBS_EX_PROGRESS)
		{
			nUpPartHeight = rcWin.Height() * pMember->m_nProgressPos / (pMember->m_nMax - pMember->m_nMin);
		}
		else
		{
			nUpPartHeight = rcThumb.top + rcThumb.Height() / 2;
		}

		HIUIIMAGE hbmpStretchMark = NULL;
		int nHeight = rcWin.Height();
		if (IsMarkImageToThumb())
		{
			nHeight = rcWin.Height() - nUpPartHeight;
		}

		if (m_bBkCombine)
		{
			CIUIImage::PartNineGridBitmap(m_himgCombineBk,
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				4,
				bEnable ? 1 : 2,
				&hbmpStretchMark,
				rcWin.Width(), nHeight);
		}
		else
		{
			CIUIImage::NineGridBitmap(bEnable ? pMember->m_himgBk[1] : pMember->m_himgBk[2],
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				&hbmpStretchMark, rcWin.Width(), nHeight);
		}

		IUIBitBlt(pMemDCParent->GetSafeHdc(),
			rcWin.left, rcWin.top + nUpPartHeight, rcWin.Width(), rcWin.Height() - nUpPartHeight,
			hbmpStretchMark,
			0, IsMarkImageToThumb() ? 0 : nUpPartHeight, SRCCOPY);
	}

	// Draw thumb
	int nStateIndex = 0;
	if (IsWindowEnabled())
	{
		BOOL bSelected = GetKeyState(VK_LBUTTON) < 0;
		if (bSelected && pMember->m_bCaptureThumb)
		{
			if (pMember->m_bIsMouseOverThumb)
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
			if (pMember->m_bIsMouseOverThumb)
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
	else
	{
		nStateIndex = 3;
	}

	if (pMember->m_bThumbCombine)
	{
		if (pMember->m_himgCombineThumb->GetSafeHBITMAP() == NULL)
		{
			pMember->m_himgCombineThumb->SafeLoadSavedImage();
		}

		if (pMember->m_himgCombineThumb->GetSafeHBITMAP() != NULL)
		{
			IUIBitBlt(pMemDCParent->GetSafeHdc(),
				rcWin.left + rcThumb.left, rcWin.top + rcThumb.top,
				pMember->m_sizeThumb.cx, pMember->m_sizeThumb.cy,
				pMember->m_himgCombineThumb,
				pMember->m_himgCombineThumb->GetWidth() * nStateIndex / 4, 0, SRCCOPY);
		}
	}
	else
	{
		if (pMember->m_himgThumb[nStateIndex]->IsHasAlphaChannel())
		{
			IUIBitBlt(pMemDCParent->GetSafeHdc(),
				rcWin.left + rcThumb.left, rcWin.top + rcThumb.top,
				pMember->m_sizeThumb.cx, pMember->m_sizeThumb.cy,
				pMember->m_himgThumb[nStateIndex], 0, 0, SRCCOPY);
		}
		else
		{
			CDC memDCThumb;
			memDCThumb.CreateCompatibleDC(pMemDCParent);
			HBITMAP hBmpThumbOld = (HBITMAP)memDCThumb.SelectObject(pMember->m_himgThumb[nStateIndex]);
			::TransparentBlt(pMemDCParent->m_hDC,
				rcWin.left + rcThumb.left,
				rcWin.top + rcThumb.top,
				pMember->m_sizeThumb.cx,
				pMember->m_sizeThumb.cy,
				memDCThumb,
				0,
				0,
				pMember->m_sizeThumb.cx,
				pMember->m_sizeThumb.cy,
				RGB(255, 0, 255));
			memDCThumb.SelectObject(hBmpThumbOld);
		}
	}
}

void CWLSliderCtrl::DrawHorzSlider(CDC *pMemDCParent)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	CRect rcWin;
	GetWindowRect(rcWin);
	GetParent()->ScreenToClient(rcWin);

	// Don't draw this control if it's out of the parent
	CRect rcParent;
	GetParent()->GetClientRect(rcParent);

	CRect rcIntersect;
	rcIntersect.IntersectRect(rcWin, rcParent);
	if (rcIntersect.IsRectEmpty())
	{
		return;
	}

	CRect rcThumb;
	if (pMember->m_bCaptureThumb)
	{
		rcThumb = pMember->m_rcTrackingThumb;
	}
	else
	{
		GetThumbRect(rcThumb);
	}

	// draw background
	BOOL bEnable = IsWindowEnabled();

	if (m_bBkCombine)
	{
		IUIPartNineGridBlt(pMemDCParent->GetSafeHdc(),
			rcWin.left, rcWin.top, rcWin.Width(), rcWin.Height(),
			m_himgCombineBk,
			m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
			4, bEnable ? 0 : 3);
	}
	else
	{
		IUINineGridBlt(pMemDCParent->GetSafeHdc(),
			rcWin.left, rcWin.top, rcWin.Width(), rcWin.Height(),
			bEnable ? pMember->m_himgBk[0] : pMember->m_himgBk[3],
			m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
	}

	// draw split part
	if ((pMember->m_dwStyleEx & TBS_EX_SPLITPROGRESS) == TBS_EX_SPLITPROGRESS)
	{
		for (int i = 0; i < pMember->m_nSplitCount && pMember->m_pnSplitList + i != NULL; ++i)
		{
			HBITMAP hbmp = pMember->m_ColorMap[*(pMember->m_pnSplitList + i)];

			int nLeft = rcWin.Width() * i / pMember->m_nSplitCount;
			int nWidth = rcWin.Width() * (i + 1) / pMember->m_nSplitCount - nLeft;
			NineGridBltG(pMemDCParent->GetSafeHdc(), rcWin.left + nLeft, rcWin.top, nWidth, rcWin.Height(),
				hbmp, CPoint(1, 1), 1, 1);
		}
	}

	// draw left part
	if (pMember->m_bUseMark)
	{
		int nSliderPos = 0;
		if ((pMember->m_dwStyleEx & TBS_EX_PROGRESS) == TBS_EX_PROGRESS)
		{
			nSliderPos = rcWin.Width() * pMember->m_nProgressPos / (pMember->m_nMax - pMember->m_nMin);
		}
		else
		{
			nSliderPos = rcThumb.left + rcThumb.Width() / 2;
		}

		HIUIIMAGE himgStretchMark = NULL;
		if (m_bBkCombine)
		{
			CIUIImage::PartNineGridBitmap(m_himgCombineBk,
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				4,
				bEnable ? 1 : 2,
				&himgStretchMark,
				IsMarkImageToThumb() ? nSliderPos : rcWin.Width(),
				rcWin.Height());
		}
		else
		{
			CIUIImage::NineGridBitmap(bEnable ? pMember->m_himgBk[1] : pMember->m_himgBk[2],
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				&himgStretchMark,
				IsMarkImageToThumb() ? nSliderPos : rcWin.Width(),
				rcWin.Height());
		}

		IUIBitBlt(pMemDCParent->GetSafeHdc(),
			rcWin.left, rcWin.top, nSliderPos, rcWin.Height(),
			himgStretchMark, 0, 0, SRCCOPY);
	}

	// Draw thumb
	int nStateIndex = 0;
	if (IsWindowEnabled())
	{
		BOOL bSelected = GetKeyState(VK_LBUTTON) < 0;
		if (bSelected && pMember->m_bCaptureThumb)
		{
			if (pMember->m_bIsMouseOverThumb)
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
			if (pMember->m_bIsMouseOverThumb)
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
	else
	{
		nStateIndex = 3;
	}

	if (pMember->m_bThumbCombine)
	{
		if (pMember->m_himgCombineThumb != NULL)
		{
			IUIBitBlt(pMemDCParent->GetSafeHdc(),
				rcWin.left + rcThumb.left, rcWin.top + rcThumb.top, pMember->m_sizeThumb.cx, pMember->m_sizeThumb.cy,
				pMember->m_himgCombineThumb,
				pMember->m_himgCombineThumb->GetWidth() * nStateIndex / 4, 0, SRCCOPY);
		}
	}
	else
	{
		if (pMember->m_himgThumb[nStateIndex]->IsHasAlphaChannel())
		{
			IUIBitBlt(pMemDCParent->GetSafeHdc(),
				rcWin.left + rcThumb.left, rcWin.top + rcThumb.top, pMember->m_sizeThumb.cx, pMember->m_sizeThumb.cy,
				pMember->m_himgThumb[nStateIndex],
				0, 0, SRCCOPY);
		}
		else
		{
			CDC memDCThumb;
			memDCThumb.CreateCompatibleDC(pMemDCParent);
			HBITMAP hBmpThumbOld = (HBITMAP)memDCThumb.SelectObject(pMember->m_himgThumb[nStateIndex]);
			::TransparentBlt(pMemDCParent->m_hDC,
				rcWin.left + rcThumb.left, rcWin.top + rcThumb.top,
				pMember->m_sizeThumb.cx, pMember->m_sizeThumb.cy,
				memDCThumb,
				0, 0, pMember->m_sizeThumb.cx, pMember->m_sizeThumb.cy, RGB(255, 0, 255));
			memDCThumb.SelectObject(hBmpThumbOld);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWLSliderCtrl message handlers

int CWLSliderCtrl::OnDraw(CDC *pMemDCParent)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	if (pMember->m_dwStyle & TBS_VERT)
	{
		DrawVertSlider(pMemDCParent);
	}
	else //SBS_HORZ
	{
		DrawHorzSlider(pMemDCParent);
	}

	return 0;
}

LRESULT CWLSliderCtrl::WLWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class

	if (message == WM_LBUTTONDOWN)
	{
		CPoint point((DWORD)lParam);

		CRect rcThumb;
		GetThumbRect(rcThumb);
		pMember->m_rcTrackingThumb = rcThumb;
		if (rcThumb.PtInRect(point))
		{
			SetCapture();
			pMember->m_bCaptureThumb = TRUE;
			pMember->m_ptDragThumb = point;
			pMember->m_nDragPos = pMember->m_nPos;
		}
		else
		{
			if (pMember->m_dwStyle & TBS_VERT)
			{
				CRect rcClient;
				GetClientRect(rcClient);
				if (point.y > rcThumb.bottom && point.y < rcClient.Height() - pMember->m_nDeflateRTDN) // move to small position(down)
				{
					int nOldPos = GetPos();
					int nScrollRange = rcClient.Height() - pMember->m_nDeflateLTUP - pMember->m_nDeflateRTDN - pMember->m_sizeThumb.cy;
					int nMovedPos = (point.y - rcThumb.top - rcThumb.Height() / 2) * (pMember->m_nMax - pMember->m_nMin) / nScrollRange;
					int nNewPos = nOldPos - nMovedPos;
					if (nNewPos > pMember->m_nMax)
					{
						nNewPos = pMember->m_nMax;
					}
					if (nNewPos < pMember->m_nMin)
					{
						nNewPos = pMember->m_nMin;
					}
					SetPos(nNewPos);

					GetParent()->SendMessage(WM_VSCROLL, MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)this);
					CRect rcThumbNew;
					GetThumbRect(rcThumbNew);
					if (rcThumbNew.PtInRect(point))
					{
						// SendMessage(WpMember->m_LBUTTONDOWN, wParam, lParam);
					}
				}
				else if (point.y < rcThumb.top && point.y > pMember->m_nDeflateLTUP) // move to large position(up)
				{
					int nOldPos = GetPos();
					int nScrollRange = rcClient.Height()
						- pMember->m_nDeflateLTUP
						- pMember->m_nDeflateRTDN
						- pMember->m_sizeThumb.cy;
					int nMovedPos = (point.y - rcThumb.bottom + rcThumb.Height() / 2)
						* (pMember->m_nMax - pMember->m_nMin) / nScrollRange;
					int nNewPos = nOldPos - nMovedPos;
					if (nNewPos > pMember->m_nMax)
					{
						nNewPos = pMember->m_nMax;
					}
					if (nNewPos < pMember->m_nMin)
					{
						nNewPos = pMember->m_nMin;
					}
					SetPos(nNewPos);

					GetParent()->SendMessage(WM_VSCROLL, MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)this);
					CRect rcThumbNew;
					GetThumbRect(rcThumbNew);
					if (rcThumbNew.PtInRect(point))
					{
						// SendMessage(WM_LBUTTONDOWN, wParam, lParam);
					}
				}
			}
			else // TBS_HORZ
			{
				CRect rcClient;
				GetClientRect(rcClient);
				if (point.x > rcThumb.right && point.x < rcClient.Width() - pMember->m_nDeflateRTDN) // move to large position(right)
				{
					int nOldPos = GetPos();
					int nScrollRange = rcClient.Width() - pMember->m_nDeflateLTUP - pMember->m_nDeflateRTDN - pMember->m_sizeThumb.cx;
					int nMovedPos = (point.x - rcThumb.left - rcThumb.Width() / 2) * (pMember->m_nMax - pMember->m_nMin) / nScrollRange;
					int nNewPos = nOldPos + nMovedPos;
					if (nNewPos > pMember->m_nMax)
					{
						nNewPos = pMember->m_nMax;
					}
					if (nNewPos < pMember->m_nMin)
					{
						nNewPos = pMember->m_nMin;
					}
					SetPos(nNewPos);

					GetParent()->SendMessage(WM_HSCROLL, MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)this);
					CRect rcThumbNew;
					GetThumbRect(rcThumbNew);
					if (rcThumbNew.PtInRect(point))
					{
						// SendMessage(WM_LBUTTONDOWN, wParam, lParam);
					}
				}
				else if (point.x < rcThumb.left && point.x > pMember->m_nDeflateLTUP) // move to small position(left)
				{
					int nOldPos = GetPos();
					int nScrollRange = rcClient.Width() - pMember->m_nDeflateLTUP - pMember->m_nDeflateRTDN - pMember->m_sizeThumb.cx;
					int nMovedPos = (point.x - rcThumb.right + rcThumb.Width() / 2) * (pMember->m_nMax - pMember->m_nMin) / nScrollRange;
					int nNewPos = nOldPos + nMovedPos;
					if (nNewPos > pMember->m_nMax)
					{
						nNewPos = pMember->m_nMax;
					}
					if (nNewPos < pMember->m_nMin)
					{
						nNewPos = pMember->m_nMin;
					}
					SetPos(nNewPos);

					GetParent()->SendMessage(WM_HSCROLL, MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)this);
					CRect rcThumbNew;
					GetThumbRect(rcThumbNew);
					if (rcThumbNew.PtInRect(point))
					{
						// SendMessage(WM_LBUTTONDOWN, wParam, lParam);
					}
				}
			}
		}
		Invalidate();
	}

	if (message == WM_LBUTTONUP)
	{
		ReleaseCapture();
		pMember->m_bCaptureThumb = FALSE;
		Invalidate();

		NMHDR nmhdr;
		nmhdr.hwndFrom = NULL;
		nmhdr.idFrom = GetDlgCtrlID();
		nmhdr.code = 0xfffffff0;
		LRESULT lr = GetParent()->SendMessage(WM_NOTIFY, (WPARAM)(nmhdr.idFrom), (LPARAM)(&nmhdr)); // lr == *pResult
	}

	if (message == WM_MOUSEMOVE)
	{
		CPoint point((DWORD)lParam);
		if (pMember->m_bCaptureThumb)
		{
			// reset thumb's state
			if (!pMember->m_rcTrackingThumb.PtInRect(point))
			{
				if (pMember->m_bIsMouseOverThumb)
				{
					pMember->m_bIsMouseOverThumb = FALSE;
					Invalidate();
				}
			}
			else
			{
				if (!pMember->m_bIsMouseOverThumb)
				{
					pMember->m_bIsMouseOverThumb = TRUE;
					Invalidate();
				}
			}

			// send WpMember->m_XSCROLL message to parent
			if (pMember->m_dwStyle & TBS_VERT)
			{
				int nOffset = point.y - pMember->m_ptDragThumb.y;

				// calculate the original rect of the thumb
				CRect rcClient;
				GetClientRect(rcClient);
				int nScrollRange = rcClient.Height() - pMember->m_nDeflateLTUP - pMember->m_nDeflateRTDN - pMember->m_sizeThumb.cy;
				int nY = 0;
				if (pMember->m_nMax != pMember->m_nMin)
				{
					nY = pMember->m_nDeflateLTUP + nScrollRange * (pMember->m_nMax - pMember->m_nDragPos) / (pMember->m_nMax - pMember->m_nMin);
				}
				if (nY < pMember->m_nDeflateLTUP)
				{
					nY = pMember->m_nDeflateLTUP;
				}
				if (nY > rcClient.Height() - pMember->m_nDeflateRTDN)
				{
					nY = rcClient.Height() - pMember->m_nDeflateRTDN;
				}

				CRect rcOrgThumb;
				rcOrgThumb.left = (rcClient.Width() - pMember->m_sizeThumb.cx) / 2;
				rcOrgThumb.top = nY;
				rcOrgThumb.right = rcOrgThumb.left + pMember->m_sizeThumb.cx;
				rcOrgThumb.bottom = rcOrgThumb.top + pMember->m_sizeThumb.cy;

				// calculate the rect of the thumb when drag
				pMember->m_rcTrackingThumb = rcOrgThumb;
				pMember->m_rcTrackingThumb.DeflateRect(0, nOffset, 0, -nOffset);
				if (pMember->m_rcTrackingThumb.top < pMember->m_nDeflateLTUP)
				{
					pMember->m_rcTrackingThumb.top = pMember->m_nDeflateLTUP;
					pMember->m_rcTrackingThumb.bottom = pMember->m_rcTrackingThumb.top + rcOrgThumb.Height();
				}
				if (pMember->m_rcTrackingThumb.bottom > (rcClient.Height() - pMember->m_nDeflateRTDN))
				{
					pMember->m_rcTrackingThumb.bottom = rcClient.Height() - pMember->m_nDeflateRTDN;
					pMember->m_rcTrackingThumb.top = pMember->m_rcTrackingThumb.bottom - rcOrgThumb.Height();
				}

				if (pMember->m_rcTrackingThumb.bottom >= rcClient.bottom)
				{
					if (pMember->m_nPos != pMember->m_nMin)
					{
						int nOldPos = pMember->m_nPos;
						SetPos(pMember->m_nMin);
						GetParent()->SendMessage(WM_VSCROLL, MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)this);
					}
				}
				else if (pMember->m_rcTrackingThumb.top <= rcClient.top)
				{
					if (pMember->m_nPos != pMember->m_nMax)
					{
						int nOldPos = pMember->m_nPos;
						SetPos(pMember->m_nMax);
						GetParent()->SendMessage(WM_VSCROLL, MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)this);
					}
				}
				else
				{
					int nMovedPos = (pMember->m_rcTrackingThumb.top - rcOrgThumb.top) * (pMember->m_nMax - pMember->m_nMin) / nScrollRange;
					if (pMember->m_nPos != pMember->m_nDragPos - nMovedPos)
					{
						int nOldPos = pMember->m_nPos;
						int nNewPos = pMember->m_nDragPos - nMovedPos;
						if (nNewPos < pMember->m_nMin)
						{
							nNewPos = pMember->m_nMin;
						}
						if (nNewPos > pMember->m_nMax)
						{
							nNewPos = pMember->m_nMax;
						}
						SetPos(nNewPos);

						GetParent()->SendMessage(WM_VSCROLL, MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)this);
					}
				}
				Invalidate();
			}
			else // TBS_HORZ
			{
				int nOffset = point.x - pMember->m_ptDragThumb.x;

				// calculate the original rect of the thumb
				CRect rcClient;
				GetClientRect(rcClient);
				int nScrollRange = rcClient.Width() - pMember->m_nDeflateLTUP - pMember->m_nDeflateRTDN - pMember->m_sizeThumb.cx;
				int nX = 0;
				if (pMember->m_nMax != pMember->m_nMin)
				{
					nX = pMember->m_nDeflateLTUP + nScrollRange * pMember->m_nDragPos / (pMember->m_nMax - pMember->m_nMin);
				}
				if (nX < pMember->m_nDeflateLTUP)
				{
					nX = pMember->m_nDeflateLTUP;
				}
				if (nX > rcClient.Width() - pMember->m_nDeflateRTDN)
				{
					nX = rcClient.Width() - pMember->m_nDeflateRTDN;
				}

				CRect rcOrgThumb;
				rcOrgThumb.left = nX;
				rcOrgThumb.top = (rcClient.Height() - pMember->m_sizeThumb.cy) / 2;
				rcOrgThumb.right = rcOrgThumb.left + pMember->m_sizeThumb.cx;
				rcOrgThumb.bottom = rcOrgThumb.top + pMember->m_sizeThumb.cy;

				// calculate the rect of the thumb when drag
				pMember->m_rcTrackingThumb = rcOrgThumb;
				pMember->m_rcTrackingThumb.DeflateRect(nOffset, 0, -nOffset, 0);
				if (pMember->m_rcTrackingThumb.left < pMember->m_nDeflateLTUP)
				{
					pMember->m_rcTrackingThumb.left = pMember->m_nDeflateLTUP;
					pMember->m_rcTrackingThumb.right = pMember->m_rcTrackingThumb.left + rcOrgThumb.Width();
				}
				if (pMember->m_rcTrackingThumb.right > (rcClient.Width() - pMember->m_nDeflateRTDN))
				{
					pMember->m_rcTrackingThumb.right = rcClient.Width() - pMember->m_nDeflateRTDN;
					pMember->m_rcTrackingThumb.left = pMember->m_rcTrackingThumb.right - rcOrgThumb.Width();
				}

				if (pMember->m_rcTrackingThumb.left <= 0)
				{
					if (pMember->m_nPos != pMember->m_nMin)
					{
						int nOldPos = pMember->m_nPos;
						SetPos(pMember->m_nMin);
						GetParent()->SendMessage(WM_HSCROLL, MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)this);
					}
				}
				else if (pMember->m_rcTrackingThumb.right >= rcClient.right)
				{
					if (pMember->m_nPos != pMember->m_nMax)
					{
						int nOldPos = pMember->m_nPos;
						SetPos(pMember->m_nMax);
						GetParent()->SendMessage(WM_HSCROLL, MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)this);
					}
				}
				else
				{
					int nMovedPos = (pMember->m_rcTrackingThumb.left - rcOrgThumb.left) * (pMember->m_nMax - pMember->m_nMin) / nScrollRange;
					if (pMember->m_nPos != pMember->m_nDragPos - nMovedPos)
					{
						int nOldPos = pMember->m_nPos;
						int nNewPos = pMember->m_nDragPos + nMovedPos;
						if (nNewPos < pMember->m_nMin)
						{
							nNewPos = pMember->m_nMin;
						}
						if (nNewPos > pMember->m_nMax)
						{
							nNewPos = pMember->m_nMax;
						}
						SetPos(nNewPos);

						GetParent()->SendMessage(WM_HSCROLL, MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)this);
					}
				}
				Invalidate();
			}
		}
		else // not capture
		{
			CRect rcThumb;
			GetThumbRect(rcThumb);
			if (!pMember->m_bIsMouseOverThumb)
			{
				if (rcThumb.PtInRect(point))
				{
					pMember->m_bIsMouseOverThumb = TRUE;

					// when set RGN for highlight stats, show the tool tip when mouse over the button(the tool tip and highlight stats at the same time work).
					// don't call Invalidate() after this "if" section
					Invalidate();
				}
			}
			else
			{
				if (!rcThumb.PtInRect(point))
				{
					pMember->m_bIsMouseOverThumb = FALSE;
					Invalidate();
				}
			}

			if (!pMember->m_bIsMouseOver)
			{
				pMember->m_bIsMouseOver = TRUE;
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = GetParent()->GetSafeHwnd();/*m_hWnd*/
				tme.dwHoverTime = HOVER_DEFAULT;
				::TrackMouseEvent(&tme);
				Invalidate();
			}
		}
	}

	if (message == WM_MOUSELEAVE)
	{
		pMember->m_bIsMouseOverThumb = FALSE;
		pMember->m_bIsMouseOver = FALSE;
		Invalidate();

		// Hide the tip window.
		if (pMember->m_bEnableTipWindow && pMember->m_pTipWnd->GetSafeHwnd() != NULL)
		{
			pMember->m_pTipWnd->ShowWindow(SW_HIDE);
		}
	}

	if (message == WM_KILLFOCUS || message == WM_CAPTURECHANGED)
	{
		pMember->m_bCaptureThumb = FALSE;
		Invalidate();
	}

	if (message == WM_ENABLE)
	{
		Invalidate();
	}

	return CRectCtrl::WLWindowProc(message, wParam, lParam);
}

int CWLSliderCtrl::EnableToolTipsEx(BOOL bEnable, BOOL bMouseMove/* = TRUE*/)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	return 0;
}

int CWLSliderCtrl::SetToolTips(LPCTSTR lpszToolTips)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	return 0;
}

CToolTipCtrl *CWLSliderCtrl::GetToolTipCtrl()
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	return &pMember->m_tooltip;
}

int CWLSliderCtrl::EnableTipWindow(BOOL bEnable)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	pMember->m_bEnableTipWindow = bEnable;

	return 0;
}

BOOL CWLSliderCtrl::IsEnableTipWindow() const
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	return pMember->m_bEnableTipWindow;
}

int CWLSliderCtrl::SetTipWindow(CWnd *pTipWnd)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	pMember->m_pTipWnd = pTipWnd;

	return 0;
}

CWnd *CWLSliderCtrl::GetTipWindow()
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	return pMember->m_pTipWnd;
}

int CWLSliderCtrl::SetTipWindowOffset(const LPSIZE lpsizeOffset)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	pMember->m_sizeTipWindowOffset = *lpsizeOffset;

	return 0;
}

int CWLSliderCtrl::GetTipWindowOffset(LPSIZE lpsizeOffset) const
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	if (lpsizeOffset == NULL)
	{
		return -1;
	}

	*lpsizeOffset = pMember->m_sizeTipWindowOffset;

	return 0;
}

CWLSliderCtrl *g_pThis = NULL;

VOID CALLBACK CWLSliderCtrl_OnTimer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if (idEvent == IDT_ANIMATE_THUMB)
	{
		if (g_pThis->GetCurTimerTick() >= (int)g_pThis->GetAnimationData()->size() - 1)
		{
			g_pThis->KillTimer(IDT_ANIMATE_THUMB);
		}

		g_pThis->SetPos((*(g_pThis->GetAnimationData()))[g_pThis->GetCurTimerTick()]);

		g_pThis->SetCurTimerTick(g_pThis->GetCurTimerTick() + 1);

		if (g_pThis->GetCurTimerTick() >= (int)g_pThis->GetAnimationData()->size())
		{
			g_pThis->SetAnimationMode(false);
			g_pThis->SetCurTimerTick(0);
			g_pThis->GetAnimationData()->clear();

			NMHDR nmhdr;
			nmhdr.hwndFrom = g_pThis->GetParent()->GetSafeHwnd();
			nmhdr.idFrom = g_pThis->GetDlgCtrlID();
			nmhdr.code = SLDN_ANIMATION_END;
			g_pThis->GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmhdr));
		}
	}
}

int CWLSliderCtrl::AnimateThumb(std::vector<int> vAnimationData, int nElapse/*ms*/)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	pMember->m_bAnimationMode = true;
	pMember->m_nCurTimerTick = 0;
	pMember->m_nTimerElapse = nElapse;
	pMember->m_vAnimationData = vAnimationData;

	g_pThis = this;
	SetTimer(IDT_ANIMATE_THUMB, nElapse, CWLSliderCtrl_OnTimer);

	return 0;
}

int CWLSliderCtrl::GetCurTimerTick() const
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	return pMember->m_nCurTimerTick;
}

int CWLSliderCtrl::SetCurTimerTick(int nValue)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	pMember->m_nCurTimerTick = nValue;

	return 0;
}

std::vector<int> *CWLSliderCtrl::GetAnimationData()
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	return &pMember->m_vAnimationData;
}

int CWLSliderCtrl::SetAnimationMode(BOOL bAnimation)
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	pMember->m_bAnimationMode = bAnimation ? true : false;

	return 0;
}

BOOL CWLSliderCtrl::IsAnimationMode() const
{
	WLSLIDERMEMBER *pMember = (WLSLIDERMEMBER *)m_pMember;

	return pMember->m_bAnimationMode ? TRUE : FALSE;
}
