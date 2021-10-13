// SliderCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#if(_MSC_VER >= 1600) // VC2010
#include <iterator>		// For checked_array_iterator
#endif
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSliderCtrlEx

CSliderCtrlEx::CSliderCtrlEx()
{
	m_pBindWnd = this;

	m_pMember = new SLIDEREXMEMBER;
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;
}

CSliderCtrlEx::~CSliderCtrlEx()
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	if (m_pMember != NULL)
	{
		delete (SLIDEREXMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

int CSliderCtrlEx::BindStyle(LPCTSTR lpszStyleID)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	if (m_hWnd == NULL)
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

	return 0;
}

int CSliderCtrlEx::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

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
	if (ect != CT_STYLE_SLIDEREX && ect != CT_SLIDEREX)
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
			SetImages(CONTROL_STATE_UNCHECKED_ALL,
				strImageName[1], strImageName[2], strImageName[3], strImageName[4]);
		}

		SetBkImageResizeMode(pCtrlProp->m_eBkImageResizeMode);
		SetBkImageResizePoint(pCtrlProp->m_ptImageResize);
		SetBkImageRepeatX(pCtrlProp->m_uXRepeatPixel);
		SetBkImageRepeatX(pCtrlProp->m_uYRepeatPixel);
	}

	// Thumb images
	CString strImageName[1 + COMBINEIMAGESIZE4];
	BOOL bCombineImage = TRUE;
	CTRLPROPERTIES::IUIGetSliderThumb4(pSldExProp, &bCombineImage, strImageName);
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

int CSliderCtrlEx::ReleaseObject()
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	pMember->Release();

	return CControlBase::ReleaseObject();
}

BEGIN_MESSAGE_MAP(CSliderCtrlEx, CWnd)
	//{{AFX_MSG_MAP(CSliderCtrlEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CSliderCtrlEx::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	pMember->m_dwStyle = dwStyle;

	CString strMyClass;
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;
	strMyClass = WC_TRACKBAREX;
	wndcls.lpszClassName = strMyClass.GetBuffer(strMyClass.GetLength());

	// Register the new class and exit if it fails
	if (!AfxRegisterClass(&wndcls))
	{
		TRACE(_T("[LibUIDK]===Class Registration Failed\n"));
		return FALSE;
	}

	CWnd *pWnd = this;
	return pWnd->Create(WC_TRACKBAREX, NULL, dwStyle, rect, pParentWnd, nID);

}

DWORD CSliderCtrlEx::GetExtendedStyle()
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	return pMember->m_dwStyleEx;
}

DWORD CSliderCtrlEx::SetExtendedStyle(DWORD dwNewStyle)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	DWORD dwOld = pMember->m_dwStyleEx;
	pMember->m_dwStyleEx = dwNewStyle;
	return dwOld;
}

void CSliderCtrlEx::SetThumbSize(SIZE sizeThumb)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	pMember->m_sizeThumb = sizeThumb;
}

int CSliderCtrlEx::GetThumbSize(SIZE *psizeThumb) const
{
	if (psizeThumb == NULL)
	{
		return -1;
	}

	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;
	*psizeThumb = pMember->m_sizeThumb;

	return 0;
}

// Set the deflate value for slider range
int CSliderCtrlEx::SetDeflate(UINT nLeftTop, UINT nRightBottom)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	pMember->m_nDeflateLTUP = nLeftTop;
	pMember->m_nDeflateRTDN = nRightBottom;

	return 0;
}

int CSliderCtrlEx::GetDeflate(UINT *pnLeftTop, UINT *pnRightBottom) const
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

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

int CSliderCtrlEx::GetPos() const
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	return pMember->m_nPos;
}

void CSliderCtrlEx::SetPos(int nPos)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

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
	InvalidateRect(NULL);
}

int CSliderCtrlEx::GetProgressPos() const
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	return pMember->m_nProgressPos;
}

void CSliderCtrlEx::SetProgressPos(int nPos)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	pMember->m_nProgressPos = nPos;
	Invalidate();
}

void CSliderCtrlEx::GetThumbRect(LPRECT lprc) const
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	if (pMember->m_dwStyle & TBS_VERT)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		int nScrollRange = rcClient.Height() - pMember->m_nDeflateLTUP - pMember->m_nDeflateRTDN - pMember->m_sizeThumb.cy;
		int nY = 0;
		if (pMember->m_nMax != pMember->m_nMin)
		{
			nY = pMember->m_nDeflateLTUP + nScrollRange * (pMember->m_nMax - pMember->m_nPos) / (pMember->m_nMax - pMember->m_nMin);
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
		int nScrollRange = rcClient.Width() - pMember->m_nDeflateLTUP - pMember->m_nDeflateRTDN - pMember->m_sizeThumb.cx;
		int nX = 0;
		if (pMember->m_nMax != pMember->m_nMin)
		{
			nX = pMember->m_nDeflateLTUP + nScrollRange * (pMember->m_nPos - pMember->m_nMin) / (pMember->m_nMax - pMember->m_nMin);
		}
		lprc->left = nX;
		lprc->top = (rcClient.Height() - pMember->m_sizeThumb.cy) / 2;
		lprc->right = lprc->left + pMember->m_sizeThumb.cx;
		lprc->bottom = lprc->top + pMember->m_sizeThumb.cy;
	}
}

void CSliderCtrlEx::SetRange(int nMin, int nMax, BOOL bRedraw/* = FALSE*/)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	pMember->m_nMin = nMin;
	pMember->m_nMax = nMax;
	if (bRedraw)
	{
		InvalidateRect(NULL);
	}
}

void CSliderCtrlEx::GetRange(int *pnMin, int *pnMax) const
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	if (pnMin != NULL)
	{
		*pnMin = pMember->m_nMin;
	}
	if (pnMax != NULL)
	{
		*pnMax = pMember->m_nMax;
	}
}

int CSliderCtrlEx::SetImages(UINT uMask, LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameMarkN,
	LPCTSTR lpszImageNameMarkD, LPCTSTR lpszImageNameD)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	ASSERT(m_hWnd != NULL);

	m_bBkCombine = false;
	IUISetControlImage4(uMask, pMember->m_himgBk,
		lpszImageNameN, lpszImageNameMarkN, lpszImageNameMarkD, lpszImageNameD);

	return 0;
}

int CSliderCtrlEx::GetImages(UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameH,
	CString *pstrImageNameS, CString *pstrImageNameD)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bBkCombine;
	IUIGetControlImage4(uMask, m_himgCombineBk, pMember->m_himgBk, m_bBkCombine, pstrCombineImageName,
		pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);

	return 0;
}

int CSliderCtrlEx::SetThumbCombineImage(LPCTSTR lpszImageName, BOOL bRedraw/* = TRUE*/)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	pMember->m_bThumbCombine = true;

	IUISetControlImage(&pMember->m_himgCombineThumb, lpszImageName);

	// set the size of thumb
	if (pMember->m_himgCombineThumb->GetSafeHBITMAP() == NULL)
	{
		pMember->m_himgCombineThumb->SafeLoadSavedImage();
	}

	if (pMember->m_himgCombineThumb->GetSafeHBITMAP() != NULL)
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

int CSliderCtrlEx::SetThumbImages(
	UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
	LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	pMember->m_bThumbCombine = false;
	ASSERT(m_hWnd != NULL);

	IUISetControlImage4(uMask, pMember->m_himgThumb,
		lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);

	// set the size of thumb
	if (pMember->m_himgThumb[0]->GetSafeHBITMAP() == NULL)
	{
		pMember->m_himgThumb[0]->SafeLoadSavedImage();
	}

	if (pMember->m_himgThumb[0]->GetSafeHBITMAP() != NULL)
	{
		pMember->m_sizeThumb.cx = pMember->m_himgThumb[0]->GetWidth();
		pMember->m_sizeThumb.cy = pMember->m_himgThumb[0]->GetHeight();
	}

	return 0;
}

int CSliderCtrlEx::GetThumbImages(UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameH,
	CString *pstrImageNameS, CString *pstrImageNameD)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = pMember->m_bThumbCombine;
	IUIGetControlImage4(uMask, pMember->m_himgCombineThumb, pMember->m_himgThumb,
		m_bBkCombine, pstrCombineImageName,
		pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);

	return 0;
}

BOOL CSliderCtrlEx::IsCombineThumbImage() const
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	return pMember->m_bThumbCombine;
}

int CSliderCtrlEx::UseMarkBitmap(BOOL bUseMark)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	pMember->m_bUseMark = bUseMark;
	return 0;
}

BOOL CSliderCtrlEx::IsUseMarkBitmap() const
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	return pMember->m_bUseMark;
}

int CSliderCtrlEx::MarkImageToThumb(BOOL bToThumb)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	pMember->m_bMarkToThumb = bToThumb;

	return 0;
}

BOOL CSliderCtrlEx::IsMarkImageToThumb() const
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	return pMember->m_bMarkToThumb;
}

// only for TBS_EX_SPLITPROGRESS style
int CSliderCtrlEx::SetSplitCount(int nCount)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	if (nCount > pMember->m_nSplitCount)
	{
		SafeDelete(pMember->m_pnSplitList);
		pMember->m_pnSplitList = new int [nCount];
	}

	pMember->m_nSplitCount = nCount;
	return 0;
}

int CSliderCtrlEx::GetSplitCount()
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	return pMember->m_nSplitCount;
}

int CSliderCtrlEx::SetSplitList(const int *pnFirst, const int *pnLast, BOOL bRedraw/*=TRUE*/)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

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
	std::copy(pnFirst, pnFirst + pMember->m_nSplitCount, stdext::checked_array_iterator<int *>(pMember->m_pnSplitList, pMember->m_nSplitCount));
#endif

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CSliderCtrlEx::AddColorImage(int nValue, HBITMAP hbmpColor)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	pMember->m_ColorMap[nValue] = hbmpColor;

	return 0;
}

int CSliderCtrlEx::RemoveColorImage(int nValue)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	pMember->m_ColorMap.erase(nValue);

	return 0;
}

int SLDEX_DrawThumb(SLIDEREXMEMBER *pMember, CDC *pDC, CSliderCtrlEx *pThis, const CRect &rcThumb)
{
	int nStateIndex = 0;
	if (pThis->IsWindowEnabled())
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
			if (pMember->m_himgCombineThumb->IsHasAlphaChannel())
			{
				IUIBitBlt(pDC->GetSafeHdc(),
					rcThumb.left, rcThumb.top, pMember->m_sizeThumb.cx, pMember->m_sizeThumb.cy,
					pMember->m_himgCombineThumb,
					pMember->m_himgCombineThumb->GetWidth() * nStateIndex / 4, 0, SRCCOPY);
			}
			else
			{
				CDC memDCThumb;
				memDCThumb.CreateCompatibleDC(pDC);
				HBITMAP hBmpThumbOld = (HBITMAP)memDCThumb.SelectObject(
						pMember->m_himgCombineThumb->GetSafeHBITMAP());
				::TransparentBlt(pDC->m_hDC,
					rcThumb.left, rcThumb.top,
					pMember->m_sizeThumb.cx, pMember->m_sizeThumb.cy,
					memDCThumb,
					pMember->m_himgCombineThumb->GetWidth() * nStateIndex / 4, 0,
					pMember->m_sizeThumb.cx, pMember->m_sizeThumb.cy,
					RGB(255, 0, 255));
				memDCThumb.SelectObject(hBmpThumbOld);
			}
		}
	}
	else
	{
		if (pMember->m_himgThumb[nStateIndex]->IsHasAlphaChannel())
		{
			IUIBitBlt(pDC->GetSafeHdc(),
				rcThumb.left, rcThumb.top, pMember->m_sizeThumb.cx, pMember->m_sizeThumb.cy,
				pMember->m_himgThumb[nStateIndex], 0, 0, SRCCOPY);
		}
		else
		{
			CDC memDCThumb;
			memDCThumb.CreateCompatibleDC(pDC);
			HBITMAP hBmpThumbOld = (HBITMAP)memDCThumb.SelectObject(pMember->m_himgThumb[nStateIndex]);
			::TransparentBlt(pDC->m_hDC,
				rcThumb.left, rcThumb.top,
				pMember->m_sizeThumb.cx, pMember->m_sizeThumb.cy,
				memDCThumb,
				0, 0,
				pMember->m_sizeThumb.cx, pMember->m_sizeThumb.cy,
				RGB(255, 0, 255));
			memDCThumb.SelectObject(hBmpThumbOld);
		}
	}

	return 0;
}

void CSliderCtrlEx::DrawVertSlider(CDC *pMemDC)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	CRect rcWin;
	GetWindowRect(rcWin);
	GetParent()->ScreenToClient(rcWin);

	// draw background
	BOOL bEnable = IsWindowEnabled();

	if (m_bBkCombine)
	{
		IUIPartNineGridBlt(pMemDC->GetSafeHdc(), 0, 0, rcWin.Width(), rcWin.Height(),
			m_himgCombineBk, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
			4, bEnable ? 0 : 3);
	}
	else
	{
		IUINineGridBlt(pMemDC->GetSafeHdc(), 0, 0, rcWin.Width(), rcWin.Height(),
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
			NineGridBltG(pMemDC->GetSafeHdc(), 0, nTop, rcWin.Width(), nHeight, hbmp,
				CPoint(1, 1), 1, 1);
		}
	}

	// draw down part
	CRect rcThumb;
	if (pMember->m_bCaptureThumb)
	{
		rcThumb = pMember->m_rcTrackingThumb;
	}
	else
	{
		GetThumbRect(rcThumb);
	}

	if (pMember->m_bUseMark)
	{
		int nUpPartHeight = 0;
		if ((pMember->m_dwStyleEx & TBS_EX_PROGRESS) == TBS_EX_PROGRESS)
		{
			nUpPartHeight = rcWin.Height() * (pMember->m_nProgressPos - pMember->m_nMin) / (pMember->m_nMax - pMember->m_nMin);
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
				4, bEnable ? 1 : 2,
				&hbmpStretchMark, rcWin.Width(), nHeight);
		}
		else
		{
			CIUIImage::NineGridBitmap(bEnable ? pMember->m_himgBk[1] : pMember->m_himgBk[2],
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				&hbmpStretchMark, rcWin.Width(), nHeight);
		}

		IUIBitBlt(pMemDC->GetSafeHdc(), 0, nUpPartHeight, rcWin.Width(), rcWin.Height() - nUpPartHeight, hbmpStretchMark, 0, IsMarkImageToThumb() ? 0 : nUpPartHeight, SRCCOPY);

		ReleaseIUIImage(hbmpStretchMark);
		hbmpStretchMark = NULL;
	}

	// Draw thumb
	SLDEX_DrawThumb(pMember, pMemDC, this, rcThumb);
}

void CSliderCtrlEx::DrawHorzSlider(CDC *pMemDC)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	CRect rcWin;
	GetWindowRect(rcWin);
	GetParent()->ScreenToClient(rcWin);

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
		IUIPartNineGridBlt(pMemDC->GetSafeHdc(), 0, 0, rcWin.Width(), rcWin.Height(),
			m_himgCombineBk,
			m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
			4, bEnable ? 0 : 3);
	}
	else
	{
		IUINineGridBlt(pMemDC->GetSafeHdc(), 0, 0, rcWin.Width(), rcWin.Height(),
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
			NineGridBltG(pMemDC->GetSafeHdc(), nLeft, 0, nWidth, rcWin.Height(), hbmp,
				CPoint(2, 2), 1, 1);
		}
	}

	// draw left part
	if (pMember->m_bUseMark)
	{
		int nSliderPos = 0;
		if ((pMember->m_dwStyleEx & TBS_EX_PROGRESS) == TBS_EX_PROGRESS)
		{
			nSliderPos = rcWin.Width() * (pMember->m_nProgressPos - pMember->m_nMin) / (pMember->m_nMax - pMember->m_nMin);
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
				4, bEnable ? 1 : 2,
				&himgStretchMark,
				IsMarkImageToThumb() ? nSliderPos : rcWin.Width(), rcWin.Height());
		}
		else
		{
			CIUIImage::NineGridBitmap(bEnable ? pMember->m_himgBk[1] : pMember->m_himgBk[2],
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				&himgStretchMark,
				IsMarkImageToThumb() ? nSliderPos : rcWin.Width(), rcWin.Height());
		}

		IUIBitBlt(pMemDC->GetSafeHdc(), 0, 0, nSliderPos, rcWin.Height(), himgStretchMark, 0, 0, SRCCOPY);

		ReleaseIUIImage(himgStretchMark);
		himgStretchMark = NULL;
	}

	// Draw thumb
	SLDEX_DrawThumb(pMember, pMemDC, this, rcThumb);
}

/////////////////////////////////////////////////////////////////////////////
// CSliderCtrlEx message handlers

LRESULT CSliderCtrlEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

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

	else if (message == WM_GETBACKGROUND)
	{
		LRESULT lr = CWnd::WindowProc(message, wParam, lParam);	// Give derived class a chance.
		if (lr != 0)	// The derived class has handle.
		{
			return lr;
		}

		BOOL *pbCompatibleBitmap = (BOOL *)lParam;

		if (pMember->m_bmpCompatible.GetSafeHandle() == NULL)
		{
			if (pbCompatibleBitmap != NULL)
			{
				*pbCompatibleBitmap = FALSE;
			}

			return (LRESULT)NULL;
		}
		else
		{
			if (pbCompatibleBitmap != NULL)
			{
				*pbCompatibleBitmap = TRUE;
			}

			return (LRESULT)pMember->m_bmpCompatible.GetSafeHandle();
		}
	}

	else if (message == WM_PAINT)
	{
		CPaintDC dc(this); // device context for painting

		// TODO: Add your message handler code here
		CRect rcClient;
		GetClientRect(rcClient);
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);

		if (pMember->m_bmpCompatible.GetSafeHandle() != NULL)
		{
			pMember->m_bmpCompatible.DeleteObject();
		}
		pMember->m_bmpCompatible.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
		ASSERT(pMember->m_bmpCompatible.GetSafeHandle() != NULL);

		CBitmap *pBmpOld = memDC.SelectObject(&pMember->m_bmpCompatible);

		// Draw the parent background to control for alpha blend with background of the control.
		BOOL bDrawParentPart = FALSE;
		if (m_bBkCombine)
		{
			m_himgCombineBk->SafeLoadSavedImage();

			if (m_himgCombineBk->GetSafeHBITMAP() != NULL)
			{
				bDrawParentPart = m_himgCombineBk->IsHasAlphaChannel();
			}
		}
		else
		{
			pMember->m_himgBk[IsWindowEnabled() ? 0 : 3]->SafeLoadSavedImage();

			if (pMember->m_himgBk[IsWindowEnabled() ? 0 : 3]->GetSafeHBITMAP() != NULL)
			{
				bDrawParentPart = pMember->m_himgBk[IsWindowEnabled() ? 0 : 3]->IsHasAlphaChannel();
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

		if (pMember->m_dwStyle & TBS_VERT)
		{
			DrawVertSlider(&memDC);
		}
		else //SBS_HORZ
		{
			DrawHorzSlider(&memDC);
		}
		dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pBmpOld);
	}

	else if (message == WM_LBUTTONDOWN)
	{
		CPoint point((DWORD)lParam);

		SetFocus();

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

					GetParent()->SendMessage(WM_VSCROLL, MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)m_hWnd);
					CRect rcThumbNew;
					GetThumbRect(rcThumbNew);
					if (rcThumbNew.PtInRect(point))
					{
						SendMessage(WM_LBUTTONDOWN, wParam, lParam);
					}
				}
				else if (point.y < rcThumb.top && point.y > pMember->m_nDeflateLTUP) // move to large position(up)
				{
					int nOldPos = GetPos();
					int nScrollRange = rcClient.Height() - pMember->m_nDeflateLTUP - pMember->m_nDeflateRTDN - pMember->m_sizeThumb.cy;
					int nMovedPos = (point.y - rcThumb.bottom + rcThumb.Height() / 2) * (pMember->m_nMax - pMember->m_nMin) / nScrollRange;
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

					GetParent()->SendMessage(WM_VSCROLL, MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)m_hWnd);
					CRect rcThumbNew;
					GetThumbRect(rcThumbNew);
					if (rcThumbNew.PtInRect(point))
					{
						SendMessage(WM_LBUTTONDOWN, wParam, lParam);
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

					GetParent()->SendMessage(WM_HSCROLL, MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)m_hWnd);
					CRect rcThumbNew;
					GetThumbRect(rcThumbNew);
					if (rcThumbNew.PtInRect(point))
					{
						SendMessage(WM_LBUTTONDOWN, wParam, lParam);
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

					GetParent()->SendMessage(WM_HSCROLL, MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)m_hWnd);
					CRect rcThumbNew;
					GetThumbRect(rcThumbNew);
					if (rcThumbNew.PtInRect(point))
					{
						SendMessage(WM_LBUTTONDOWN, wParam, lParam);
					}
				}
			}
		}
		Invalidate();
	}

	else if (message == WM_LBUTTONUP)
	{
		ReleaseCapture();
		pMember->m_bCaptureThumb = FALSE;
		InvalidateRect(NULL);

		NMHDR nmhdr;
		nmhdr.hwndFrom = this->m_hWnd;
		nmhdr.idFrom = ::GetDlgCtrlID(this->m_hWnd);
		nmhdr.code = NM_RELEASEDCAPTURE;
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
					UpdateWindow();
				}
			}
			else
			{
				if (!pMember->m_bIsMouseOverThumb)
				{
					pMember->m_bIsMouseOverThumb = TRUE;
					UpdateWindow();
				}
			}

			// send WM_XSCROLL message to parent
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
						GetParent()->SendMessage(WM_VSCROLL, MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)m_hWnd);
					}
				}
				else if (pMember->m_rcTrackingThumb.top <= rcClient.top)
				{
					if (pMember->m_nPos != pMember->m_nMax)
					{
						int nOldPos = pMember->m_nPos;
						SetPos(pMember->m_nMax);
						GetParent()->SendMessage(WM_VSCROLL,
							MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)m_hWnd);
					}
				}
				else
				{
					int nMovedPos = (pMember->m_rcTrackingThumb.top - rcOrgThumb.top)
						* (pMember->m_nMax - pMember->m_nMin) / nScrollRange;
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

						GetParent()->SendMessage(WM_VSCROLL,
							MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)m_hWnd);
					}
				}
				InvalidateRect(NULL);
			}
			else // TBS_HORZ
			{
				int nOffset = point.x - pMember->m_ptDragThumb.x;

				// calculate the original rect of the thumb
				CRect rcClient;
				GetClientRect(rcClient);
				int nScrollRange = rcClient.Width() - pMember->m_nDeflateLTUP
					- pMember->m_nDeflateRTDN - pMember->m_sizeThumb.cx;
				int nX = 0;
				if (pMember->m_nMax != pMember->m_nMin)
				{
					nX = pMember->m_nDeflateLTUP
						+ nScrollRange * (pMember->m_nDragPos - pMember->m_nMin)
						/ (pMember->m_nMax - pMember->m_nMin);
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
						GetParent()->SendMessage(WM_HSCROLL,
							MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)m_hWnd);
					}
				}
				else if (pMember->m_rcTrackingThumb.right >= rcClient.right)
				{
					if (pMember->m_nPos != pMember->m_nMax)
					{
						int nOldPos = pMember->m_nPos;
						SetPos(pMember->m_nMax);
						GetParent()->SendMessage(WM_HSCROLL,
							MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)m_hWnd);
					}
				}
				else
				{
					int nMovedPos = (pMember->m_rcTrackingThumb.left - rcOrgThumb.left)
						* (pMember->m_nMax - pMember->m_nMin) / nScrollRange;

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

						GetParent()->SendMessage(WM_HSCROLL,
							MAKEWPARAM(nOldPos, pMember->m_nPos), (LPARAM)m_hWnd);
					}
				}
				InvalidateRect(NULL);
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

			// For playback slider, may show the tip window.
			if (pMember->m_bEnableTipWindow && pMember->m_pTipWnd->GetSafeHwnd() != NULL)
			{
				CRect rcTipWnd;
				pMember->m_pTipWnd->GetWindowRect(rcTipWnd);
				CPoint ptTipWnd = point;
				ptTipWnd.Offset(pMember->m_sizeTipWindowOffset);
				ClientToScreen(&ptTipWnd);
				int nWidth = rcTipWnd.Width();
				int nHeight = rcTipWnd.Height();
				rcTipWnd.SetRect(ptTipWnd.x, ptTipWnd.y, ptTipWnd.x + nWidth, ptTipWnd.y + nHeight);

				if (!pMember->m_pTipWnd->IsWindowVisible())
				{
					pMember->m_pTipWnd->ShowWindow(SW_SHOW);
				}

				pMember->m_pTipWnd->MoveWindow(rcTipWnd);
			}

			if (!pMember->m_bIsMouseOver)
			{
				pMember->m_bIsMouseOver = TRUE;
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = m_hWnd;
				tme.dwHoverTime = HOVER_DEFAULT;
				::TrackMouseEvent(&tme);
				InvalidateRect(NULL);
			}
		}
	}

	else if (message == WM_MOUSELEAVE)
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

	else if (message == WM_KILLFOCUS || message == WM_CAPTURECHANGED)
	{
		pMember->m_bCaptureThumb = FALSE;
		Invalidate();
	}

	else if (message == WM_ENABLE)
	{
		Invalidate();
	}

	else if (message == WM_GET_INTERNAL_VAR)
	{
		if (CString((LPCTSTR)wParam) == _T("m_bCaptureThumb"))
		{
			return (LRESULT)&pMember->m_bCaptureThumb;
		}
		else if (CString((LPCTSTR)wParam) == _T("m_bIsMouseOver"))
		{
			return (LRESULT)&pMember->m_bIsMouseOver;
		}
		else if (CString((LPCTSTR)wParam) == _T("m_bIsMouseOverThumb"))
		{
			return (LRESULT)&pMember->m_bIsMouseOverThumb;
		}
		else if (CString((LPCTSTR)wParam) == _T("m_rcTrackingThumb"))
		{
			return (LRESULT)&pMember->m_rcTrackingThumb;
		}
		else if (CString((LPCTSTR)wParam) == _T("m_nDeflateLTUP"))
		{
			return (LRESULT)&pMember->m_nDeflateLTUP;
		}
		else if (CString((LPCTSTR)wParam) == _T("m_nDeflateRTDN"))
		{
			return (LRESULT)&pMember->m_nDeflateRTDN;
		}
		else if (CString((LPCTSTR)wParam) == _T("m_ptDragThumb"))
		{
			return (LRESULT)&pMember->m_ptDragThumb;
		}
		else if (CString((LPCTSTR)wParam) == _T("m_nDragPos"))
		{
			return (LRESULT)&pMember->m_nDragPos;
		}
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

BOOL CSliderCtrlEx::PreTranslateMessage(MSG *pMsg)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_MOUSEMOVE && pMember->m_bEnableTooltips)
	{
		if (pMember->m_bMouseMoveTooltips)
		{
			int nOldPos = -1;
			int nCurPos = GetPos();
			CString text;
			if ((nCurPos >= pMember->m_nMin && nCurPos <= pMember->m_nMax) && nCurPos != nOldPos)
			{
				text.Format(_T("%d"), nCurPos);
				nOldPos = nCurPos;
				GetParent()->SendMessage(WM_GETTOOLTIPS, WPARAM(&text), LPARAM(this));
				pMember->m_tooltip.UpdateTipText(text, this);
			}
			else
			{
				text = _T("");
				pMember->m_tooltip.UpdateTipText(text, this);
			}
		}
		pMember->m_tooltip.RelayEvent(pMsg);
	}

	return CWnd::PreTranslateMessage(pMsg);
}

int CSliderCtrlEx::EnableToolTipsEx(BOOL bEnable, BOOL bMouseMove/* = TRUE*/)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	pMember->m_bEnableTooltips = bEnable;
	pMember->m_bMouseMoveTooltips = bMouseMove;
	if (pMember->m_bEnableTooltips)
	{
		if (pMember->m_tooltip.GetSafeHwnd() == NULL)
		{
			pMember->m_tooltip.Create(this);
			pMember->m_tooltip.Activate(TRUE);
			pMember->m_tooltip.AddTool(this, _T(""));
		}
	}

	return 0;
}

int CSliderCtrlEx::SetToolTips(LPCTSTR lpszToolTips)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	pMember->m_tooltip.AddTool(this, lpszToolTips);
	return 0;
}

CToolTipCtrl *CSliderCtrlEx::GetToolTipCtrl()
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	return &pMember->m_tooltip;
}

int CSliderCtrlEx::EnableTipWindow(BOOL bEnable)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	pMember->m_bEnableTipWindow = bEnable;

	return 0;
}

BOOL CSliderCtrlEx::IsEnableTipWindow() const
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	return pMember->m_bEnableTipWindow;
}

int CSliderCtrlEx::SetTipWindow(CWnd *pTipWnd)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	pMember->m_pTipWnd = pTipWnd;

	return 0;
}

CWnd *CSliderCtrlEx::GetTipWindow()
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	return pMember->m_pTipWnd;
}

int CSliderCtrlEx::SetTipWindowOffset(const LPSIZE lpsizeOffset)
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	pMember->m_sizeTipWindowOffset = *lpsizeOffset;

	return 0;
}

int CSliderCtrlEx::GetTipWindowOffset(LPSIZE lpsizeOffset) const
{
	SLIDEREXMEMBER *pMember = (SLIDEREXMEMBER *)m_pMember;

	if (lpsizeOffset == NULL)
	{
		return -1;
	}

	*lpsizeOffset = pMember->m_sizeTipWindowOffset;

	return 0;
}
