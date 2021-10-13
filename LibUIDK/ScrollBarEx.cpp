// ScrollBarEx.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define T_LTUP               1
#define T_RTDN               2
#define T_SPACE1             3
#define T_SPACE2             4

#define T_SCROLLLTUP         10
#define T_SCROLLRTDN         11
#define T_SCROLLPAGEUP       12
#define T_SCROLLPAGEDOWN     13

#define THUMB_MIN_LENGTH		8	// XP is 8, win7 is 15

/////////////////////////////////////////////////////////////////////////////
// CScrollBarEx

CScrollBarEx::CScrollBarEx()
{
	m_nLTUP = 15;
	m_nRTDN = 15;
	memset(&m_si, 0, sizeof(SCROLLINFO));
	m_si.cbSize = sizeof(SCROLLINFO);
	m_si.fMask = SIF_ALL;
	m_dwStyle = SBS_HORZ;

	int i = 0;
	for (i = 0; i < 4; ++i)
	{
		m_himgBk[i] = NULL;
	}
	m_himgButton1Arrow = NULL;
	m_himgButton2Arrow = NULL;

	m_bCombineThumb = true;
	m_himgCombineThumb = NULL;
	for (i = 0; i < 4; ++i)
	{
		m_himgThumb[i] = NULL;
	}
	m_ptThumbImageResize.x = m_ptThumbImageResize.y = 2;
	m_nCapture = -1;
	m_nArrowFlags = ESB_ENABLE_BOTH;
	m_ptDragThumb.x = m_ptDragThumb.y = 0;
	memset(&m_siDrag, 0, sizeof(SCROLLINFO));
	m_nDragOffset = 0;
	m_pNotifyWnd = NULL;
	m_bCaptureThumb = FALSE;
	m_bIsMouseOverThumb = FALSE;
	m_bCaptureLTUPButton = FALSE;
	m_bIsMouseOverLTUPButton = FALSE;
	m_bCaptureRTDNButton = FALSE;
	m_bIsMouseOverRTDNButton = FALSE;
}

CScrollBarEx::~CScrollBarEx()
{
	ReleaseObject();
}

int CScrollBarEx::ReleaseObject()
{
	int i = 0;
	for (i = 0; i < 4; ++i)
	{
		ReleaseIUIImage(m_himgBk[i]);
		m_himgBk[i] = NULL;
	}
	ReleaseIUIImage(m_himgButton1Arrow);
	m_himgButton1Arrow = NULL;
	ReleaseIUIImage(m_himgButton2Arrow);
	m_himgButton2Arrow = NULL;
	ReleaseIUIImage(m_himgCombineThumb);
	m_himgCombineThumb = NULL;
	for (i = 0; i < 4; ++i)
	{
		ReleaseIUIImage(m_himgThumb[i]);
		m_himgThumb[i] = NULL;
	}

	return CControlBase::ReleaseObject();
}

BEGIN_MESSAGE_MAP(CScrollBarEx, CWnd)
	//{{AFX_MSG_MAP(CScrollBarEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// CScrollBar functions

BOOL CScrollBarEx::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	m_dwStyle = dwStyle;

	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = WC_SCROLLBAREX;

	// Register the new class and exit if it fails
	if (!AfxRegisterClass(&wndcls))
	{
		TRACE(_T("[LibUIDK]===Class Registration Failed\n"));
		return FALSE;
	}

	CWnd *pWnd = this;
	return pWnd->Create(WC_SCROLLBAREX, NULL, dwStyle, rect, pParentWnd, nID);
}

int CScrollBarEx::BindStyle(LPCTSTR lpszStyleID)
{
	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (m_dwStyle & SBS_VERT)
	{
		VERSBPROPERTIES *pVertScrollbarProp = (VERSBPROPERTIES *)CUIMgr::GetStyleItem(STYLET_VERSCROLLBAR, lpszStyleID);
		return BindVertScrollbarStyle(pVertScrollbarProp);
	}
	else if (m_dwStyle & SBS_SIZEBOX)
	{
	}
	else //SBS_HORZ
	{
		HORSBPROPERTIES *pHorzScrollbarProp = (HORSBPROPERTIES *)CUIMgr::GetStyleItem(STYLET_HORSCROLLBAR, lpszStyleID);
		return BindHorzScrollbarStyle(pHorzScrollbarProp);
	}

	return 0;
}

int CScrollBarEx::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (m_dwStyle & SBS_VERT)
	{
		return BindVertScrollbarStyle((VERSBPROPERTIES *)pCtrlProp);
	}
	else if (m_dwStyle & SBS_SIZEBOX)
	{
	}
	else //SBS_HORZ
	{
		return BindHorzScrollbarStyle((HORSBPROPERTIES *)pCtrlProp);
	}

	return 0;
}

int CScrollBarEx::BindHorzScrollbarStyle(const HORSBPROPERTIES *pHorzScrollbarProp)
{
	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (pHorzScrollbarProp == NULL)
	{
		return -2;
	}

	CONTROL_TYPE ect = (CONTROL_TYPE)pHorzScrollbarProp->m_eControlType;
	if (ect != CT_STYLE_HORSCROLLBAR && ect != CT_HORSCROLLBAR)
	{
		return -3;
	}

	// Position
	LONG lLeftButtonWidth = pHorzScrollbarProp->m_nLeftButtonWidth;
	LONG lRightButtonWidth = pHorzScrollbarProp->m_nRightButtonWidth;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIPointX(lLeftButtonWidth);
		GetDPIPointX(lRightButtonWidth);
	}
	SetLTUPButton(lLeftButtonWidth);
	SetRTDNButton(lRightButtonWidth);

	// Background
	CString strImageName[1 + COMBINEIMAGESIZE4];
	BOOL bCombineImage = TRUE;
	CTRLPROPERTIES::IUIGetBackground4(pHorzScrollbarProp, &bCombineImage, strImageName);
	if (bCombineImage)
	{
		SetBkCombineImage(strImageName[0]);
	}
	else
	{
		SetBkBitmaps(CONTROL_STATE_UNCHECKED_ALL,
			strImageName[1], strImageName[2], strImageName[3], strImageName[4]);
	}

	SetBkImageResizeMode(pHorzScrollbarProp->m_eBkImageResizeMode);
	SetBkImageResizePoint(pHorzScrollbarProp->m_ptImageResize);
	SetBkImageRepeatX(pHorzScrollbarProp->m_uXRepeatPixel);
	SetBkImageRepeatY(pHorzScrollbarProp->m_uYRepeatPixel);

	// Thumb
	CString strHorThumb[1 + COMBINEIMAGESIZE4];
	BOOL bCombineThumb = TRUE;
	CTRLPROPERTIES::IUIGetThumbImages4(pHorzScrollbarProp, &bCombineThumb, strHorThumb);
	if (bCombineThumb)
	{
		SetCombineThumbBitmap(strHorThumb[0]);
	}
	else
	{
		SetThumbBitmaps(CONTROL_STATE_UNCHECKED_ALL,
			strHorThumb[1], strHorThumb[2], strHorThumb[3], strHorThumb[4]);
	}
	SetThumbImageResizePoint(pHorzScrollbarProp->m_ptThumbResize);

	// Arrow
	SetLTUPArrow(pHorzScrollbarProp->m_strLeftArrow);
	SetRTDNArrow(pHorzScrollbarProp->m_strRightArrow);

	return 0;
}

int CScrollBarEx::BindVertScrollbarStyle(const VERSBPROPERTIES *pVertScrollbarProp)
{
	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (pVertScrollbarProp == NULL)
	{
		return -2;
	}

	CONTROL_TYPE ect = (CONTROL_TYPE)pVertScrollbarProp->m_eControlType;
	if (ect != CT_STYLE_VERSCROLLBAR && ect != CT_VERSCROLLBAR)
	{
		return -3;
	}

	// Position
	LONG lUpButtonHeight = pVertScrollbarProp->m_nUpButtonHeight;
	LONG lDownButtonHeight = pVertScrollbarProp->m_nDownButtonHeight;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIPointY(lUpButtonHeight);
		GetDPIPointY(lDownButtonHeight);
	}
	SetLTUPButton(lUpButtonHeight);
	SetRTDNButton(lDownButtonHeight);

	// Background
	CString strImageName[1 + COMBINEIMAGESIZE4];
	BOOL bCombineImage = TRUE;
	CTRLPROPERTIES::IUIGetBackground4(pVertScrollbarProp, &bCombineImage, strImageName);
	if (bCombineImage)
	{
		SetBkCombineImage(strImageName[0]);
	}
	else
	{
		SetBkBitmaps(CONTROL_STATE_UNCHECKED_ALL,
			strImageName[1], strImageName[2], strImageName[3], strImageName[4]);
	}

	SetBkImageResizeMode(pVertScrollbarProp->m_eBkImageResizeMode);
	SetBkImageResizePoint(pVertScrollbarProp->m_ptImageResize);
	SetBkImageRepeatX(pVertScrollbarProp->m_uXRepeatPixel);
	SetBkImageRepeatY(pVertScrollbarProp->m_uYRepeatPixel);

	// Thumb
	CString strVerThumb[1 + COMBINEIMAGESIZE4];
	BOOL bCombineThumb = TRUE;
	CTRLPROPERTIES::IUIGetThumbImages4(pVertScrollbarProp, &bCombineThumb, strVerThumb);
	if (bCombineThumb)
	{
		SetCombineThumbBitmap(strVerThumb[0]);
	}
	else
	{
		SetThumbBitmaps(CONTROL_STATE_UNCHECKED_ALL, strVerThumb[1], strVerThumb[2], strVerThumb[3], strVerThumb[4]);
	}
	SetThumbImageResizePoint(pVertScrollbarProp->m_ptThumbResize);

	// Arrow
	SetLTUPArrow(pVertScrollbarProp->m_strUpArrow);
	SetRTDNArrow(pVertScrollbarProp->m_strDownArrow);

	return 0;
}

BOOL CScrollBarEx::EnableScrollBar(UINT nArrowFlags)
{
	ASSERT(::IsWindow(m_hWnd));
	if (nArrowFlags == ESB_ENABLE_BOTH)
	{
		m_nArrowFlags = ESB_ENABLE_BOTH;
	}
	else if (nArrowFlags == ESB_DISABLE_LTUP)
	{
		m_nArrowFlags |= ESB_DISABLE_LTUP;
	}
	else if (nArrowFlags == ESB_DISABLE_RTDN)
	{
		m_nArrowFlags |= ESB_DISABLE_RTDN;
	}
	else if (nArrowFlags == ESB_DISABLE_BOTH)
	{
		m_nArrowFlags |= ESB_DISABLE_BOTH;
	}
	else
	{
		return  FALSE;
	}
	return TRUE;
}

#if(WINVER >= 0x0500)
BOOL CScrollBarEx::GetScrollBarInfo(PSCROLLBARINFO pScrollInfo) const
{
	return FALSE;
}
#endif //(WINVER >= 0x0500)

BOOL CScrollBarEx::GetScrollInfo(LPSCROLLINFO lpScrollInfo, UINT nMask)
{
	ASSERT(lpScrollInfo != NULL);

	lpScrollInfo->cbSize = sizeof(*lpScrollInfo);
	lpScrollInfo->fMask = nMask;

	//
	if (lpScrollInfo->fMask & SIF_PAGE)
	{
		lpScrollInfo->nPage = m_si.nPage;
	}
	if (lpScrollInfo->fMask & SIF_POS)
	{
		lpScrollInfo->nPos = m_si.nPos;
	}
	if (lpScrollInfo->fMask & SIF_RANGE)
	{
		lpScrollInfo->nMin = m_si.nMin;
		lpScrollInfo->nMax = m_si.nMax;
	}
	if (lpScrollInfo->fMask & SIF_TRACKPOS)
	{
		lpScrollInfo->nTrackPos = m_si.nTrackPos;
	}

	return TRUE;
}

int CScrollBarEx::GetScrollLimit()
{
	int nMin, nMax;
	GetScrollRange(&nMin, &nMax);
	SCROLLINFO info;
	if (GetScrollInfo(&info, SIF_PAGE))
	{
		nMax -= __max(info.nPage - 1, 0);
	}
	if (nMax < 0)
	{
		nMax = 0;
	}

	return nMax;
}

int CScrollBarEx::GetScrollPos() const
{
	ASSERT(::IsWindow(m_hWnd));
	return m_si.nPos;
}

void CScrollBarEx::GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos) const
{
	ASSERT(::IsWindow(m_hWnd));
	*lpMinPos = m_si.nMin;
	*lpMaxPos = m_si.nMax;
}

BOOL CScrollBarEx::SetScrollInfo(LPCSCROLLINFO lpScrollInfo, BOOL bRedraw)
{
	ASSERT(lpScrollInfo != NULL);
	ASSERT(lpScrollInfo->cbSize == sizeof(*lpScrollInfo));

	if (lpScrollInfo->fMask & SIF_PAGE)
	{
		m_si.nPage = lpScrollInfo->nPage;
	}
	if (lpScrollInfo->fMask & SIF_POS)
	{
		m_si.nPos = lpScrollInfo->nPos;
	}
	if (lpScrollInfo->fMask & SIF_RANGE)
	{
		m_si.nMin = lpScrollInfo->nMin;
		m_si.nMax = lpScrollInfo->nMax;
	}
	if (lpScrollInfo->fMask & SIF_TRACKPOS)
	{
		m_si.nTrackPos = lpScrollInfo->nTrackPos;
	}

	if (bRedraw && IsWindowVisible())
	{
		Invalidate();
		UpdateWindow();
	}

	return TRUE;
}

BOOL CScrollBarEx::SetScrollPos(int nPos, BOOL bRedraw)
{
	ASSERT(::IsWindow(m_hWnd));
	if (nPos > m_si.nMax || nPos < m_si.nMin)
	{
		return FALSE;
	}

	m_si.nPos = nPos;

	if (bRedraw)
	{
		InvalidateRect(NULL);
	}
	return TRUE;
}

void CScrollBarEx::SetScrollRange(int nMinPos, int nMaxPos, BOOL bRedraw)
{
	ASSERT(::IsWindow(m_hWnd));
	if (nMaxPos >= nMinPos)
	{
		m_si.nMin = nMinPos;
		m_si.nMax = nMaxPos;

		if (bRedraw)
		{
			InvalidateRect(NULL);
		}
	}
}

void CScrollBarEx::ShowScrollBar(BOOL bShow)
{
	ASSERT(::IsWindow(m_hWnd));
	ShowWindow(bShow ? SW_SHOW : SW_HIDE);
}

int GetButtonImage(__in HIUIIMAGE himgBk,
	__out HIUIIMAGE *phimgButton1, __out HIUIIMAGE *phimgButton2,
	DWORD dwStyle, int nLTUP, int nRTDN)
{
	if (himgBk == NULL)
	{
		return -1;
	}

	if (dwStyle & SBS_VERT)
	{
		CIUIImage::SubBitmap(himgBk, phimgButton1, 0, 0, himgBk->GetWidth(), nLTUP);
		CIUIImage::SubBitmap(himgBk, phimgButton2, 0, himgBk->GetHeight() - nRTDN, himgBk->GetWidth(), nRTDN);
	}
	else if (dwStyle & SBS_SIZEBOX)
	{
	}
	else //SBS_HORZ
	{
		CIUIImage::SubBitmap(himgBk, phimgButton1, 0, 0, nLTUP, himgBk->GetHeight());
		CIUIImage::SubBitmap(himgBk, phimgButton2, himgBk->GetWidth() - nRTDN, 0, nRTDN, himgBk->GetHeight());
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////
// CScrollBarEx functions

int CScrollBarEx::SetBkBitmaps(
	UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
	LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD)
{
	m_bBkCombine = false;
	IUISetControlImage4(uMask, m_himgBk,
		lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);

	return 0;
}

int CScrollBarEx::SetCombineThumbBitmap(LPCTSTR lpszImageName)
{
	m_bCombineThumb = true;
	m_himgCombineThumb = CreateIUIImage(lpszImageName);

	return 0;
}

int CScrollBarEx::SetThumbBitmaps(
	UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
	LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD)
{
	m_bCombineThumb = false;
	return IUISetControlImage4(uMask, m_himgThumb,
			lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);
}

int CScrollBarEx::GetThumbBitmaps(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameH,
	CString *pstrImageNameS, CString *pstrImageNameD)
{
	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bBkCombine;
	return IUIGetControlImage4(uMask, m_himgCombineBk, m_himgThumb, *pbCombineImage,
			pstrCombineImageName,
			pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);
}

int CScrollBarEx::SetThumbImageResizePoint(POINT pt)
{
	m_ptThumbImageResize = pt;
	return 0;
}

POINT CScrollBarEx::GetThumbImageResizePoint() const
{
	return m_ptThumbImageResize;
}

int CScrollBarEx::SetLTUPArrow(LPCTSTR lpszImageName)
{
	IUISetControlImage(&m_himgButton1Arrow, lpszImageName);

	Invalidate();

	return 0;
}

int CScrollBarEx::GetLTUPArrow(CString *pstrImageName)
{
	return IUIGetControlImage(m_himgButton1Arrow, pstrImageName);
}

int CScrollBarEx::SetRTDNArrow(LPCTSTR lpszImageName)
{
	return IUISetControlImage(&m_himgButton2Arrow, lpszImageName);
}

int CScrollBarEx::GetRTDNArrow(CString *pstrImageName)
{
	return IUIGetControlImage(m_himgButton2Arrow, pstrImageName);
}

void CScrollBarEx::SetLTUPButton(int nValue)
{
	m_nLTUP = nValue;
}

int CScrollBarEx::GetLTUPButton()
{
	return m_nLTUP;
}

void CScrollBarEx::SetRTDNButton(int nValue)
{
	m_nRTDN = nValue;
}

int CScrollBarEx::GetRTDNButton()
{
	return m_nRTDN;
}

void CScrollBarEx::SetNotifyWnd(CWnd *pWnd)
{
	m_pNotifyWnd = pWnd;
}

CWnd *CScrollBarEx::GetNotifyWnd()
{
	return m_pNotifyWnd;
}

//////////////////////////////////////////////////////////////////////////
//
void CScrollBarEx::DrawVertItem(CDC *pMemDC)
{
	ASSERT(m_dwStyle & SBS_VERT);
	CRect rcClient;
	GetClientRect(rcClient);

	// init the item rect
	CRect rcUp, rcDown, rcThumb, rcSpace1, rcSpace2;
	GetItemRect(rcUp, rcDown, rcThumb);

	int nThumbHeight = rcThumb.Height();
	if (rcThumb.top < rcUp.bottom)
	{
		rcThumb.top = rcUp.bottom;
		rcThumb.bottom = rcThumb.top + nThumbHeight;
	}
	if (rcThumb.bottom > rcDown.top)
	{
		rcThumb.bottom = rcDown.top;
		rcThumb.top = rcThumb.bottom - nThumbHeight;
	}

	rcSpace1 = rcClient;
	rcSpace1.top = rcUp.Height();
	rcSpace1.bottom = rcThumb.top + rcThumb.Height() / 2;
	rcSpace2 = rcClient;
	rcSpace2.top = rcThumb.bottom - rcThumb.Height() / 2;
	rcSpace2.bottom = rcDown.top;

	// Draw Left or Up button
	int nTopStateIndex = 0;
	if (IsWindowEnabled())
	{
		if (rcThumb.Height() <= 0)
		{
			nTopStateIndex = 0;
		}
		else
		{
			BOOL bSelected = GetKeyState(VK_LBUTTON) < 0;
			if (bSelected && m_bCaptureLTUPButton)
			{
				if (m_bIsMouseOverLTUPButton)
				{
					// 2.selected(push down)
					nTopStateIndex = 2;
				}
				else
				{
					// 3.highlight
					nTopStateIndex = 1;
				}
			}
			else
			{
				if (m_bIsMouseOverLTUPButton)
				{
					// 3.highlight
					nTopStateIndex = 1;
				}
				else
				{
					// 4.normal
					nTopStateIndex = 0;
				}
			}
		}
	}
	else
	{
		nTopStateIndex = 3;
	}

	if (m_bBkCombine && m_himgCombineBk != NULL)
	{
		m_himgCombineBk->SafeLoadSavedImage();

		CRect rcSrc;
		rcSrc.left = m_himgCombineBk->GetWidth() * nTopStateIndex / 4;
		rcSrc.top = 0;
		rcSrc.right = rcSrc.left + m_himgCombineBk->GetWidth() / 4;
		rcSrc.bottom = rcSrc.top + m_nLTUP;
		IUIPartNineGridBlt(pMemDC->GetSafeHdc(), rcUp,
			m_himgCombineBk, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY, rcSrc);
	}
	else
	{
		if (m_himgBk[nTopStateIndex]->GetSafeHBITMAP() == NULL)
		{
			m_himgBk[nTopStateIndex]->SafeLoadSavedImage();
		}

		if (m_himgBk[nTopStateIndex]->GetSafeHBITMAP() != NULL)
		{
			IUIPartStretchBlt(pMemDC->GetSafeHdc(), 0, 0, rcClient.Width(), rcUp.Height(),
				m_himgBk[nTopStateIndex], 0, 0, m_himgBk[nTopStateIndex]->GetWidth(), m_nLTUP,
				IRM_STRETCH);
		}
	}

	// Draw left or up arrow
	IUICenterBitBlt(pMemDC->GetSafeHdc(), rcUp, m_himgButton1Arrow, SRCCOPY);

	// Draw right or bottom button
	int nDownStateIndex = 0;
	if (IsWindowEnabled())
	{
		if (rcThumb.Height() <= 0)
		{
			nDownStateIndex = 0;
		}
		else
		{
			BOOL bSelected = GetKeyState(VK_LBUTTON) < 0;
			if (bSelected && m_bCaptureRTDNButton)
			{
				if (m_bIsMouseOverRTDNButton)
				{
					// 2.selected(push down)
					nDownStateIndex = 2;
				}
				else
				{
					// 3.highlight
					nDownStateIndex = 1;
				}
			}
			else
			{
				if (m_bIsMouseOverRTDNButton)
				{
					// 3.highlight
					nDownStateIndex = 1;
				}
				else
				{
					// 4.normal
					nDownStateIndex = 0;
				}
			}
		}
	}
	else
	{
		nDownStateIndex = 3;
	}

	CRect rcDrawDown(0, rcClient.Height() - rcDown.Height(), rcClient.Width(), rcClient.Height());
	if (m_bBkCombine && m_himgCombineBk != NULL)
	{
		CRect rcSrc;
		rcSrc.left = m_himgCombineBk->GetWidth() * nDownStateIndex / 4;
		rcSrc.top = m_himgCombineBk->GetHeight() - m_nRTDN;
		rcSrc.right = rcSrc.left + m_himgCombineBk->GetWidth() / 4;
		rcSrc.bottom = rcSrc.top + m_nRTDN;
		IUIPartNineGridBlt(pMemDC->GetSafeHdc(), rcDrawDown,
			m_himgCombineBk, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY, rcSrc);
	}
	else
	{
		if (m_himgBk[nDownStateIndex]->GetSafeHBITMAP() == NULL)
		{
			m_himgBk[nDownStateIndex]->SafeLoadSavedImage();
		}

		if (m_himgBk[nDownStateIndex]->GetSafeHBITMAP() != NULL)
		{
			IUIPartStretchBlt(pMemDC->GetSafeHdc(),
				0, rcClient.Height() - rcDown.Height(), rcClient.Width(), rcDown.Height(),
				m_himgBk[nDownStateIndex],
				0, m_himgBk[nDownStateIndex]->GetHeight() - m_nRTDN,
				m_himgBk[nDownStateIndex]->GetWidth(), m_nRTDN,
				IRM_STRETCH);
		}
	}

	// Draw right down arrow
	IUICenterBitBlt(pMemDC->GetSafeHdc(), rcDrawDown, m_himgButton2Arrow, SRCCOPY);

	// Draw space
	CRect rcSpace;
	rcSpace.left = 0;
	rcSpace.top = rcUp.bottom;
	rcSpace.right = rcClient.right;
	rcSpace.bottom = rcClient.Height() - rcDown.Height();
	if (m_bBkCombine && m_himgCombineBk != NULL)
	{
		IUIPartNineGridBltResetResizePoint(pMemDC->GetSafeHdc(), rcSpace, m_himgCombineBk,
			m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
			CRect(0, m_nLTUP, m_himgCombineBk->GetWidth() / 4, m_himgCombineBk->GetHeight() - m_nRTDN));
	}
	else
	{
		if (m_himgBk[0]->GetSafeHBITMAP() == NULL)
		{
			m_himgBk[0]->SafeLoadSavedImage();
		}

		if (m_himgBk[0]->GetSafeHBITMAP() != NULL)
		{
			IUIPartNineGridBltResetResizePoint(pMemDC->GetSafeHdc(), rcSpace, m_himgBk[0],
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				CRect(0, m_nLTUP, m_himgBk[0]->GetWidth(), m_himgBk[0]->GetHeight() - m_nRTDN));
		}
	}

	// Draw thumb
	DrawThumb(pMemDC, rcThumb);
}

int CScrollBarEx::DrawThumb(CDC *pMemDC, const CRect &rcThumb)
{
	int nStateIndex = 0;
	if (IsWindowEnabled())
	{
		BOOL bSelected = GetKeyState(VK_LBUTTON) < 0;
		if (bSelected && m_bCaptureThumb)
		{
			if (m_bIsMouseOverThumb)
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
			if (m_bIsMouseOverThumb)
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

	HIUIIMAGE hThumb = NULL;
	if (m_bCombineThumb)
	{
		m_himgCombineThumb->SafeLoadSavedImage();

		if (m_himgCombineThumb->GetSafeHBITMAP() == NULL)
		{
			return -1;
		}

		CIUIImage::SubBitmap(m_himgCombineThumb, &hThumb,
			m_himgCombineThumb->GetWidth() * nStateIndex / 4, 0,
			m_himgCombineThumb->GetWidth() / 4, m_himgCombineThumb->GetHeight());
	}
	else
	{
		hThumb = m_himgThumb[nStateIndex];
		hThumb->SafeLoadSavedImage();
	}

	if (hThumb->GetSafeHBITMAP() == NULL)
	{
		return -2;
	}

	CRect rcClient;
	GetClientRect(rcClient);

	if (m_dwStyle & SBS_VERT)
	{
		if (rcThumb.Height() < hThumb->GetHeight())
		{
			int nUpHeight = rcThumb.Height() / 2;
			int nBottomHeight = rcThumb.Height() - nUpHeight;

			CRect rcSrc1(0, 0, hThumb->GetWidth(), nUpHeight);
			CRect rcThumb1(rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.top + nUpHeight);
			int nRet = IUIPartNineGridBlt(pMemDC->GetSafeHdc(),
					rcThumb1, hThumb, m_ptThumbImageResize, 1, 1, rcSrc1);

			CRect rcSrc2(0, hThumb->GetHeight() - nBottomHeight,
				hThumb->GetWidth(), hThumb->GetHeight());
			CRect rcThumb2(rcThumb.left, rcThumb1.bottom, rcThumb.right, rcThumb.bottom);
			nRet = IUIPartNineGridBlt(pMemDC->GetSafeHdc(),
					rcThumb2, hThumb, m_ptThumbImageResize, 1, 1, rcSrc2);
		}
		else
		{
			if (hThumb->IsHasAlphaChannel())
			{
				IUINineGridBlt(pMemDC->GetSafeHdc(),
					0, rcThumb.top, rcThumb.Width(), rcThumb.Height(),
					hThumb, m_ptThumbImageResize, 1, 1);
			}
			else // Mask RGB(255, 0, 255)
			{
				CDC dcMem;
				dcMem.CreateCompatibleDC(pMemDC);
				CBitmap bmpMem;
				bmpMem.CreateCompatibleBitmap(pMemDC, rcThumb.Width(), rcThumb.Height());
				CBitmap *pOldMemBmp = dcMem.SelectObject(&bmpMem);

				NineGridBltG(dcMem.GetSafeHdc(),
					0, 0, rcThumb.Width(), rcThumb.Height(),
					hThumb->GetSafeHBITMAP(), m_ptThumbImageResize, 1, 1);
				TransparentBlt2(pMemDC->GetSafeHdc(),
					0, rcThumb.top, rcThumb.Width(), rcThumb.Height(),
					dcMem.GetSafeHdc(),
					0, 0, rcThumb.Width(), rcThumb.Height(), RGB(255, 0, 255));
				dcMem.SelectObject(pOldMemBmp);
			}
		}
	}
	else if (m_dwStyle & SBS_SIZEBOX)
	{
	}
	else //SBS_HORZ
	{
		if (rcThumb.Width() < hThumb->GetWidth())
		{
			int nLeftWidth = rcThumb.Width() / 2;
			int nRightWidth = rcThumb.Width() - nLeftWidth;

			CRect rcSrc1(0, 0, nLeftWidth, hThumb->GetHeight());
			CRect rcThumb1(rcThumb.left, rcThumb.top, rcThumb.left + nLeftWidth, rcThumb.bottom);
			int nRet = IUIPartNineGridBlt(pMemDC->GetSafeHdc(), rcThumb1,
					hThumb, m_ptThumbImageResize, 1, 1, rcSrc1);

			CRect rcSrc2(hThumb->GetWidth() - nRightWidth, 0,
				hThumb->GetWidth(), hThumb->GetHeight());
			CRect rcThumb2(rcThumb1.right, rcThumb.top, rcThumb.right, rcThumb.bottom);
			nRet = IUIPartNineGridBlt(pMemDC->GetSafeHdc(), rcThumb2,
					hThumb, m_ptThumbImageResize, 1, 1, rcSrc2);
		}
		else
		{
			if (hThumb->IsHasAlphaChannel())
			{
				IUINineGridBlt(pMemDC->GetSafeHdc(),
					rcThumb.left, 0, rcThumb.Width(), rcThumb.Height(),
					hThumb, m_ptThumbImageResize, 1, 1);
			}
			else // Mask RGB(255, 0, 255)
			{
				CDC dcMem;
				dcMem.CreateCompatibleDC(pMemDC);
				CBitmap bmpMem;
				bmpMem.CreateCompatibleBitmap(pMemDC, rcThumb.Width(), rcThumb.Height());
				CBitmap *pOldMemBmp = dcMem.SelectObject(&bmpMem);

				NineGridBltG(dcMem.GetSafeHdc(),
					0, 0, rcThumb.Width(), rcThumb.Height(),
					hThumb->GetSafeHBITMAP(), m_ptThumbImageResize, 1, 1);
				TransparentBlt2(pMemDC->GetSafeHdc(),
					rcThumb.left, 0, rcThumb.Width(), rcThumb.Height(),
					dcMem.GetSafeHdc(), 0, 0, rcThumb.Width(), rcThumb.Height(), RGB(255, 0, 255));
				dcMem.SelectObject(pOldMemBmp);
			}
		}
	}

	if (m_bCombineThumb)
	{
		ReleaseIUIImage(hThumb);
		hThumb = NULL;
	}

	return 0;
}

void CScrollBarEx::DrawHorzItem(CDC *pMemDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	// init the item rect
	CRect rcLeft, rcRight, rcThumb, rcSpace1, rcSpace2;
	GetItemRect(rcLeft, rcRight, rcThumb);

	int nThumbWidth = rcThumb.Width();
	if (rcThumb.left < rcLeft.right)
	{
		rcThumb.left = rcLeft.right;
		rcThumb.right = rcThumb.left + nThumbWidth;
	}
	if (rcThumb.right > rcRight.left)
	{
		rcThumb.right = rcRight.left;
		rcThumb.left = rcThumb.right - nThumbWidth;
	}

	rcSpace1 = rcClient;
	rcSpace1.left = rcLeft.Width();
	rcSpace1.right = rcThumb.left + rcThumb.Width() / 2;
	rcSpace2 = rcClient;
	rcSpace2.left = rcThumb.right - rcThumb.Width() / 2;
	rcSpace2.right = rcRight.left;

	// Draw Left or Up button
	int nLeftStateIndex = 0;
	if (IsWindowEnabled())
	{
		if (rcThumb.Width() <= 0)
		{
			nLeftStateIndex = 0;
		}
		else
		{
			BOOL bSelected = GetKeyState(VK_LBUTTON) < 0;
			if (bSelected && m_bCaptureLTUPButton)
			{
				if (m_bIsMouseOverLTUPButton)
				{
					// 2.selected(push down)
					nLeftStateIndex = 2;
				}
				else
				{
					// 3.highlight
					nLeftStateIndex = 1;
				}
			}
			else
			{
				if (m_bIsMouseOverLTUPButton)
				{
					// 3.highlight
					nLeftStateIndex = 1;
				}
				else
				{
					// 4.normal
					nLeftStateIndex = 0;
				}
			}
		}
	}
	else
	{
		nLeftStateIndex = 3;
	}

	if (m_bBkCombine)
	{
		m_himgCombineBk->SafeLoadSavedImage();

		if (m_himgCombineBk->GetSafeHBITMAP() != NULL)
		{
			CRect rcSrc;
			rcSrc.left = m_himgCombineBk->GetWidth() * nLeftStateIndex / 4;
			rcSrc.top = 0;
			rcSrc.right = rcSrc.left + m_nLTUP;
			rcSrc.bottom = rcSrc.top + m_himgCombineBk->GetHeight();
			IUIPartNineGridBlt(pMemDC->GetSafeHdc(), rcLeft,
				m_himgCombineBk, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY, rcSrc);
		}
	}
	else
	{
		if (m_himgBk[nLeftStateIndex]->GetSafeHBITMAP() == NULL)
		{
			m_himgBk[nLeftStateIndex]->SafeLoadSavedImage();
		}

		if (m_himgBk[nLeftStateIndex]->GetSafeHBITMAP() != NULL)
		{
			IUIPartStretchBlt(pMemDC->GetSafeHdc(), 0, 0, rcLeft.Width(), rcClient.Height(),
				m_himgBk[nLeftStateIndex],
				0, 0, m_nLTUP, m_himgBk[nLeftStateIndex]->GetHeight(), IRM_STRETCH);
		}
	}

	// Draw left or up arrow
	IUICenterBitBlt(pMemDC->GetSafeHdc(), rcLeft, m_himgButton1Arrow, SRCCOPY);

	// Draw right or bottom button
	int nRightStateIndex = 0;
	if (IsWindowEnabled())
	{
		if (rcThumb.Width() <= 0)
		{
			nRightStateIndex = 0;
		}
		else
		{
			BOOL bSelected = GetKeyState(VK_LBUTTON) < 0;
			if (bSelected && m_bCaptureRTDNButton)
			{
				if (m_bIsMouseOverRTDNButton)
				{
					// 2.selected(push down)
					nRightStateIndex = 2;
				}
				else
				{
					// 3.highlight
					nRightStateIndex = 1;
				}
			}
			else
			{
				if (m_bIsMouseOverRTDNButton)
				{
					// 3.highlight
					nRightStateIndex = 1;
				}
				else
				{
					// 4.normal
					nRightStateIndex = 0;
				}
			}
		}
	}
	else
	{
		nRightStateIndex = 3;
	}

	if (m_bBkCombine)
	{
		m_himgCombineBk->SafeLoadSavedImage();

		if (m_himgCombineBk->GetSafeHBITMAP() != NULL)
		{
			CRect rcSrc;
			rcSrc.left = m_himgCombineBk->GetWidth() * (nRightStateIndex + 1) / 4 - m_nRTDN;
			rcSrc.top = 0;
			rcSrc.right = rcSrc.left + m_nRTDN;
			rcSrc.bottom = rcSrc.top + m_himgCombineBk->GetHeight();
			IUIPartNineGridBlt(pMemDC->GetSafeHdc(), rcRight,
				m_himgCombineBk, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY, rcSrc);
		}
	}
	else
	{
		if (m_himgBk[nRightStateIndex]->GetSafeHBITMAP() == NULL)
		{
			m_himgBk[nRightStateIndex]->SafeLoadSavedImage();
		}

		if (m_himgBk[nRightStateIndex]->GetSafeHBITMAP() != NULL)
		{
			IUIPartStretchBlt(pMemDC->GetSafeHdc(),
				rcClient.right - rcRight.Width(), 0, rcRight.Width(), rcClient.Height(),
				m_himgBk[nRightStateIndex],
				m_himgBk[nRightStateIndex]->GetWidth() - m_nRTDN, 0,
				m_nRTDN, m_himgBk[nRightStateIndex]->GetHeight(), IRM_STRETCH);
		}
	}

	// Draw right or down arrow
	IUICenterBitBlt(pMemDC->GetSafeHdc(), rcRight, m_himgButton2Arrow, SRCCOPY);

	// Draw space
	CRect rcSpace;
	rcSpace.left = rcLeft.right;
	rcSpace.top = 0;
	rcSpace.right = rcClient.Width() - rcRight.Width();
	rcSpace.bottom = rcClient.Height();
	if (m_bBkCombine)
	{
		m_himgCombineBk->SafeLoadSavedImage();

		if (m_himgCombineBk->GetSafeHBITMAP() != NULL)
		{
			IUIPartNineGridBltResetResizePoint(pMemDC->GetSafeHdc(), rcSpace, m_himgCombineBk,
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				CRect(m_nLTUP, 0, m_himgCombineBk->GetWidth() / 4 - m_nRTDN, m_himgCombineBk->GetHeight()));
		}
	}
	else
	{
		if (m_himgBk[0]->GetSafeHBITMAP() == NULL)
		{
			m_himgBk[0]->SafeLoadSavedImage();
		}

		if (m_himgBk[0]->GetSafeHBITMAP() != NULL)
		{
			IUIPartNineGridBltResetResizePoint(pMemDC->GetSafeHdc(), rcSpace, m_himgBk[0],
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				CRect(m_nLTUP, 0, m_himgBk[0]->GetWidth() - m_nRTDN, m_himgBk[0]->GetHeight()));
		}
	}

	// Draw thumb
	DrawThumb(pMemDC, rcThumb);
}

void CScrollBarEx::DrawSizeBox(CDC *pMemDC)
{

}


/////////////////////////////////////////////////////////////////////////////
// CScrollBarEx message handlers

void CScrollBarEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CRect rcClient;
	GetClientRect(rcClient);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	ASSERT(bmp.GetSafeHandle() != NULL);
	CBitmap *pBmpOld = memDC.SelectObject(&bmp);

	memDC.SetStretchBltMode(COLORONCOLOR);

	DRAWITEMSTRUCT dis;
	dis.CtlType = ODT_SCROLLBAREX;
	dis.CtlID = GetDlgCtrlID();
	dis.itemID = 0;
	dis.itemState = 0;	// Defore paint
	dis.hwndItem = m_hWnd;
	dis.hDC = memDC.GetSafeHdc();
	dis.rcItem = rcClient;
	dis.itemData = 0;

	BOOL bParentDraw = (BOOL)GetParent()->SendMessage(WM_DRAWITEMEX, dis.CtlID, LPARAM(&dis));
	if (bParentDraw)
	{
		dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pBmpOld);

		return;
	}

	CRect rcWin;
	GetWindowRect(rcWin);
	GetParent()->ScreenToClient(rcWin);

	BOOL bUpdateLayeredWindow = FALSE;
	DrawParentPart(this, &memDC, rcWin, &bUpdateLayeredWindow, NULL);

	if (m_dwStyle & SBS_VERT)
	{
		DrawVertItem(&memDC);
	}
	else if (m_dwStyle & SBS_SIZEBOX)
	{
		DrawSizeBox(&memDC);
	}
	else //SBS_HORZ
	{
		DrawHorzItem(&memDC);
	}

	dis.itemState = 1;	// End paint
	bParentDraw = (BOOL)GetParent()->SendMessage(WM_DRAWITEMEX, dis.CtlID, LPARAM(&dis));

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pBmpOld);

	// Do not call CWnd::OnPaint() for painting messages
}

void CScrollBarEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	// Set witch item get the capture
	SetCapture();
	CRect rcClient;
	GetClientRect(rcClient);
	if (m_dwStyle & SBS_VERT)
	{
		CRect rcUp, rcDown, rcThumb, rcSpace1, rcSpace2;
		GetItemRect(rcUp, rcDown, rcThumb);
		rcSpace1 = rcClient;
		rcSpace1.top = m_nLTUP;
		rcSpace1.bottom = rcThumb.top;
		rcSpace2 = rcClient;
		rcSpace2.top = rcThumb.bottom;
		rcSpace2.bottom = rcDown.top;

		if (rcUp.PtInRect(point)) // LTUP
		{
			m_nCapture = 0;
			if (m_pNotifyWnd->GetSafeHwnd() != NULL)
			{
				m_pNotifyWnd->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), (LPARAM)m_hWnd);
			}
			else
			{
				GetParent()->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), (LPARAM)m_hWnd);
			}
			SetTimer(T_LTUP, 400, NULL);
		}
		else if (rcDown.PtInRect(point)) // RTDN
		{
			m_nCapture = 1;
			if (m_pNotifyWnd->GetSafeHwnd() != NULL)
			{
				m_pNotifyWnd->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), (LPARAM)m_hWnd);
			}
			else
			{
				GetParent()->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), (LPARAM)m_hWnd);
			}
			SetTimer(T_RTDN, 400, NULL);
		}
		else if (rcThumb.PtInRect(point)) // Thumb
		{
			m_ptDragThumb = point;
			m_siDrag = m_si;
			m_nCapture = 2;
		}
		else if (rcSpace1.PtInRect(point)) // Space1
		{
			m_nCapture = 3;
			if (m_pNotifyWnd->GetSafeHwnd() != NULL)
			{
				m_pNotifyWnd->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEUP, 0), (LPARAM)m_hWnd);
			}
			else
			{
				GetParent()->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEUP, 0), (LPARAM)m_hWnd);
			}
			SetTimer(T_SPACE1, 400, NULL);
		}
		else if (rcSpace2.PtInRect(point)) // Space2
		{
			m_nCapture = 4;
			if (m_pNotifyWnd->GetSafeHwnd() != NULL)
			{
				m_pNotifyWnd->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEDOWN, 0), (LPARAM)m_hWnd);
			}
			else
			{
				GetParent()->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEDOWN, 0), (LPARAM)m_hWnd);
			}
			SetTimer(T_SPACE2, 400, NULL);
		}
	}
	else if (m_dwStyle & SBS_SIZEBOX)
	{

	}
	else // SBS_HORZ
	{
		CRect rcLeft, rcRight, rcThumb, rcSpace1, rcSpace2;
		GetItemRect(rcLeft, rcRight, rcThumb);
		rcSpace1 = rcClient;
		rcSpace1.left = m_nLTUP;
		rcSpace1.right = rcThumb.left;
		rcSpace2 = rcClient;
		rcSpace2.left = rcThumb.right;
		rcSpace2.right = rcRight.left;

		if (rcLeft.PtInRect(point)) // LTUP
		{
			m_nCapture = 0;
			if (m_pNotifyWnd->GetSafeHwnd() != NULL)
			{
				m_pNotifyWnd->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINELEFT, 0), (LPARAM)m_hWnd);
			}
			else
			{
				GetParent()->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINELEFT, 0), (LPARAM)m_hWnd);
			}
			SetTimer(T_LTUP, 400, NULL);
		}
		else if (rcRight.PtInRect(point)) // RTDN
		{
			m_nCapture = 1;
			if (m_pNotifyWnd->GetSafeHwnd() != NULL)
			{
				m_pNotifyWnd->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINERIGHT, 0), (LPARAM)m_hWnd);
			}
			else
			{
				GetParent()->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINERIGHT, 0), (LPARAM)m_hWnd);
			}
			SetTimer(T_RTDN, 400, NULL);
		}
		else if (rcThumb.PtInRect(point)) // Thumb
		{
			m_ptDragThumb = point;
			m_siDrag = m_si;
			m_nCapture = 2;
		}
		else if (rcSpace1.PtInRect(point)) // Space1
		{
			m_nCapture = 3;
			if (m_pNotifyWnd->GetSafeHwnd() != NULL)
			{
				m_pNotifyWnd->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_PAGELEFT, 0), (LPARAM)m_hWnd);
			}
			else
			{
				GetParent()->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_PAGELEFT, 0), (LPARAM)m_hWnd);
			}
			SetTimer(T_SPACE1, 400, NULL);
		}
		else if (rcSpace2.PtInRect(point)) // Space2
		{
			m_nCapture = 4;
			if (m_pNotifyWnd->GetSafeHwnd() != NULL)
			{
				m_pNotifyWnd->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_PAGERIGHT, 0), (LPARAM)m_hWnd);
			}
			else
			{
				GetParent()->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_PAGERIGHT, 0), (LPARAM)m_hWnd);
			}
			SetTimer(T_SPACE2, 400, NULL);
		}
	}

	InvalidateRect(NULL);

	CWnd::OnLButtonDown(nFlags, point);
}

void CScrollBarEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	// Kill all timer
	KillTimer(T_LTUP);
	KillTimer(T_RTDN);
	KillTimer(T_SPACE1);
	KillTimer(T_SPACE2);
	KillTimer(T_SCROLLLTUP);
	KillTimer(T_SCROLLRTDN);
	KillTimer(T_SCROLLPAGEUP);
	KillTimer(T_SCROLLPAGEDOWN);

	if (m_nCapture == 2) // thumb
	{
		UINT uMsg = WM_HSCROLL;
		if (m_dwStyle & SBS_VERT)
		{
			uMsg = WM_VSCROLL;
		}

		if (m_pNotifyWnd->GetSafeHwnd() != NULL)
		{
			m_pNotifyWnd->SendMessage(uMsg, MAKEWPARAM(SB_ENDSCROLL, 0), (LPARAM)m_hWnd);
		}
		else
		{
			GetParent()->SendMessage(uMsg, MAKEWPARAM(SB_ENDSCROLL, 0), (LPARAM)m_hWnd);
		}
	}

	// Release the mouse capture
	ReleaseCapture();
	m_nCapture = -1;
	m_nDragOffset = 0;
	InvalidateRect(NULL);

	CWnd::OnLButtonUp(nFlags, point);
}

LRESULT CScrollBarEx::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	InvalidateRect(NULL);
	return 0;
}

void CScrollBarEx::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (m_nCapture == 2) // thumb
	{
		if ((m_dwStyle & SBS_VERT) && m_si.nMax != -1)
		{
			m_nDragOffset = point.y - m_ptDragThumb.y;

			if (m_nDragOffset != 0)
			{
				CRect rcClient;
				GetClientRect(rcClient);
				int nSpaceRange = rcClient.Height() - m_nLTUP - m_nRTDN;
				int nScrollRange = nSpaceRange - GetThumbLength();

				// Note: The max value of nPos is m_si.nMax + 1 - m_si.nPage

				// Get current actual position
				CRect rcThumb; // the top value can -25 if the mouse move up.
				GetItemRect(NULL, NULL, rcThumb, GIR_THUMB);

				// The new pos after scroll.
				int nNewPos = (rcThumb.top - m_nLTUP) * (m_si.nMax + 1 - m_si.nPage) / nScrollRange;

				if (nNewPos != m_si.nTrackPos)
				{
					m_si.nTrackPos = nNewPos;

					if (m_pNotifyWnd->GetSafeHwnd() != NULL)
					{
						m_pNotifyWnd->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, nNewPos), (LPARAM)m_hWnd);
					}
					else
					{
						GetParent()->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, nNewPos), (LPARAM)m_hWnd);
					}
				}

				InvalidateRect(NULL);
			}
		}
		else if (m_dwStyle & SBS_SIZEBOX)
		{

		}
		else if (m_si.nMax != -1) // SBS_HORZ
		{
			m_nDragOffset = point.x - m_ptDragThumb.x;

			if (m_nDragOffset != 0)
			{
				CRect rcClient;
				GetClientRect(rcClient);
				int nSpaceRange = rcClient.Width() - m_nLTUP - m_nRTDN;
				int nScrollRange = nSpaceRange - GetThumbLength();

				// Note: The max value of nPos is m_si.nMax + 1 - m_si.nPage

				// Get current actual position
				CRect rcThumb;
				GetItemRect(NULL, NULL, rcThumb, GIR_THUMB);

				// The new pos after scroll
				int nNewPos = (rcThumb.left - m_nLTUP) * (m_si.nMax + 1 - m_si.nPage) / nScrollRange;

				if (nNewPos != m_si.nTrackPos)
				{
					m_si.nTrackPos = nNewPos;

					if (m_pNotifyWnd->GetSafeHwnd() != NULL)
					{
						m_pNotifyWnd->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, nNewPos), (LPARAM)m_hWnd);
					}
					else
					{
						GetParent()->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, nNewPos), (LPARAM)m_hWnd);
					}
				}

				InvalidateRect(NULL);
			}
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CScrollBarEx::GetItemRect(LPRECT lprcLTUP, LPRECT lprcRTDN, LPRECT lprcThumb, UINT nMask)
{
	CRect rcClient;
	GetClientRect(rcClient);

	long lLTUP = m_nLTUP;
	long lRTDN = m_nRTDN;

	if (m_dwStyle & SBS_VERT)
	{
		if (nMask & GIR_LTUP)
		{
			ASSERT(lprcLTUP != NULL);
			*lprcLTUP = rcClient;
			lprcLTUP->bottom = lLTUP;
		}
		if (nMask & GIR_RTDN)
		{
			ASSERT(lprcRTDN != NULL);
			*lprcRTDN = rcClient;
			lprcRTDN->top = lprcRTDN->bottom - lRTDN;
		}
		if (nMask & GIR_THUMB)
		{
			ASSERT(lprcThumb != NULL);

			*lprcThumb = rcClient;
			int nSpaceRange = rcClient.Height() - lLTUP - lRTDN;

			int nThumbLength = THUMB_MIN_LENGTH;	// the min length of thumb.

			if (m_nCapture == 2)
			{
				if (m_siDrag.nMax != -1)
				{
					nThumbLength = nSpaceRange * m_siDrag.nPage / (m_siDrag.nMax + 1);
				}
			}
			else
			{
				if (m_si.nMax != -1)
				{
					nThumbLength = nSpaceRange * m_si.nPage / (m_si.nMax + 1);
				}
			}

			// If the thumb is visible, it's min size is THUMB_MIN_LENGTH.
			if (nThumbLength < THUMB_MIN_LENGTH)
			{
				nThumbLength = THUMB_MIN_LENGTH;
			}
			if (nThumbLength > nSpaceRange)
			{
				nThumbLength = 0;
			}

			//
			int nScrollRange = nSpaceRange - nThumbLength;

			// The thumb is tracking.
			if (m_nCapture == 2)
			{
				// calculate rcThumb.
				if (m_siDrag.nMax != -1)
				{
					lprcThumb->top = lLTUP + m_siDrag.nPos * nScrollRange / (m_siDrag.nMax + 1 - m_siDrag.nPage);
				}
				lprcThumb->bottom = lprcThumb->top + nThumbLength;

				lprcThumb->top += m_nDragOffset;
				lprcThumb->bottom += m_nDragOffset;
			}
			else
			{
				// calculate rcThumb.
				if (m_si.nMax + 1 - m_si.nPage > 0)
				{
					lprcThumb->top = lLTUP + m_si.nPos * nScrollRange / (m_si.nMax + 1 - m_si.nPage);
				}
				else
				{
					lprcThumb->top = 0;
				}
				lprcThumb->bottom = lprcThumb->top + nThumbLength;
			}

			if (lprcThumb->top < lLTUP)
			{
				lprcThumb->top = lLTUP;
				lprcThumb->bottom = lprcThumb->top + nThumbLength;
			}
			if (lprcThumb->bottom > rcClient.Height() - lRTDN)
			{
				lprcThumb->bottom = rcClient.Height() - lRTDN;
				lprcThumb->top = lprcThumb->bottom - nThumbLength;
			}
		}
	}
	else if (m_dwStyle & SBS_SIZEBOX)
	{
	}
	else // SBS_HORZ
	{
		if (nMask & GIR_LTUP)
		{
			ASSERT(lprcLTUP != NULL);
			*lprcLTUP = rcClient;
			lprcLTUP->right = lLTUP;
		}
		if (nMask & GIR_RTDN)
		{
			ASSERT(lprcRTDN != NULL);
			*lprcRTDN = rcClient;
			lprcRTDN->left = lprcRTDN->right - lRTDN;
		}
		if (nMask & GIR_THUMB)
		{
			ASSERT(lprcThumb != NULL);

			*lprcThumb = rcClient;
			int nSpaceRange = rcClient.Width() - lLTUP - lRTDN;

			int nThumbLength = THUMB_MIN_LENGTH;	// the min length of thumb.

			if (m_nCapture == 2)
			{
				if (m_siDrag.nMax != -1)
				{
					nThumbLength = nSpaceRange * m_siDrag.nPage / (m_siDrag.nMax + 1);
				}
			}
			else
			{
				if (m_si.nMax != -1)
				{
					nThumbLength = nSpaceRange * m_si.nPage / (m_si.nMax + 1);
				}
			}

			// If the thumb is visible, it's min size is THUMB_MIN_LENGTH.
			if (nThumbLength < THUMB_MIN_LENGTH)
			{
				nThumbLength = THUMB_MIN_LENGTH;
			}
			if (nThumbLength > nSpaceRange)
			{
				nThumbLength = 0;
			}

			//
			int nScrollRange = nSpaceRange - nThumbLength;

			// The thumb is tracking.
			if (m_nCapture == 2)
			{
				// calculate rcThumb.
				if (m_siDrag.nMax != -1)
				{
					lprcThumb->left = lLTUP + m_siDrag.nPos * nScrollRange / (m_siDrag.nMax + 1 - m_siDrag.nPage);
				}
				lprcThumb->right = lprcThumb->left + nThumbLength;

				lprcThumb->left += m_nDragOffset;
				lprcThumb->right += m_nDragOffset;
			}
			else
			{
				// calculate rcThumb.
				if (m_si.nMax + 1 - m_si.nPage > 0)
				{
					lprcThumb->left = lLTUP + m_si.nPos * nScrollRange / (m_si.nMax + 1 - m_si.nPage);
				}
				else
				{
					lprcThumb->left = 0;
				}
				lprcThumb->right = lprcThumb->left + nThumbLength;
			}

			if (lprcThumb->left < lLTUP)
			{
				lprcThumb->left = lLTUP;
				lprcThumb->right = lprcThumb->left + nThumbLength;
			}
			if (lprcThumb->right > rcClient.Width() - lRTDN)
			{
				lprcThumb->right = rcClient.Width() - lRTDN;
				lprcThumb->left = lprcThumb->right - nThumbLength;
			}
		}
	}
}

int CScrollBarEx::GetThumbLength() const
{
	CRect rcClient;
	GetClientRect(rcClient);

	int nSpaceRange = 0;
	if (m_dwStyle & SBS_VERT)
	{
		nSpaceRange = rcClient.Height() - m_nLTUP - m_nRTDN;
	}
	else if (m_dwStyle & SBS_SIZEBOX)
	{
	}
	else // SBS_HORZ
	{
		nSpaceRange = rcClient.Width() - m_nLTUP - m_nRTDN;
	}
	int nThumbLength = nSpaceRange * m_si.nPage / (m_si.nMax + 1);
	if (nThumbLength < THUMB_MIN_LENGTH)
	{
		nThumbLength = THUMB_MIN_LENGTH;
	}
	if (nThumbLength >= nSpaceRange)
	{
		nThumbLength = 0;
	}

	return nThumbLength;
}

void CScrollBarEx::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case T_LTUP:
		KillTimer(T_LTUP);
		SetTimer(T_SCROLLLTUP, 100, NULL);
		break;
	case T_RTDN:
		KillTimer(T_RTDN);
		SetTimer(T_SCROLLRTDN, 100, NULL);
		break;
	case T_SPACE1:
		KillTimer(T_SPACE1);
		SetTimer(T_SCROLLPAGEUP, 100, NULL);
		break;
	case T_SPACE2:
		KillTimer(T_SPACE2);
		SetTimer(T_SCROLLPAGEDOWN, 100, NULL);
		break;

	case T_SCROLLLTUP:
		if (m_dwStyle & SBS_VERT)
		{
			if (m_pNotifyWnd->GetSafeHwnd() != NULL)
			{
				m_pNotifyWnd->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), (LPARAM)m_hWnd);
			}
			else
			{
				GetParent()->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), (LPARAM)m_hWnd);
			}
		}
		else if (m_dwStyle & SBS_SIZEBOX)
			;
		else
		{
			if (m_pNotifyWnd->GetSafeHwnd() != NULL)
			{
				m_pNotifyWnd->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINELEFT, 0), (LPARAM)m_hWnd);
			}
			else
			{
				GetParent()->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINELEFT, 0), (LPARAM)m_hWnd);
			}
		}
		break;
	case T_SCROLLRTDN:
		if (m_dwStyle & SBS_VERT)
		{
			if (m_pNotifyWnd->GetSafeHwnd() != NULL)
			{
				m_pNotifyWnd->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), (LPARAM)m_hWnd);
			}
			else
			{
				GetParent()->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), (LPARAM)m_hWnd);
			}
		}
		else if (m_dwStyle & SBS_SIZEBOX)
			;
		else
		{
			if (m_pNotifyWnd->GetSafeHwnd() != NULL)
			{
				m_pNotifyWnd->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINERIGHT, 0), (LPARAM)m_hWnd);
			}
			else
			{
				GetParent()->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINERIGHT, 0), (LPARAM)m_hWnd);
			}
		}
		break;
	case T_SCROLLPAGEUP:
		if (m_dwStyle & SBS_VERT)
		{
			CRect rcThumb;
			GetItemRect(NULL, NULL, rcThumb, GIR_THUMB);
			CPoint pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);
			if (pt.y < rcThumb.top)
			{
				if (m_pNotifyWnd->GetSafeHwnd() != NULL)
				{
					m_pNotifyWnd->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEUP, 0), (LPARAM)m_hWnd);
				}
				else
				{
					GetParent()->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEUP, 0), (LPARAM)m_hWnd);
				}
			}
		}
		else if (m_dwStyle & SBS_SIZEBOX)
			;
		else// SB_HORZ
		{
			if (m_pNotifyWnd->GetSafeHwnd() != NULL)
			{
				m_pNotifyWnd->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_PAGELEFT, 0), (LPARAM)m_hWnd);
			}
			else
			{
				GetParent()->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_PAGELEFT, 0), (LPARAM)m_hWnd);
			}
		}
		break;
	case T_SCROLLPAGEDOWN:
		if (m_dwStyle & SBS_VERT)
		{
			CRect rcThumb;
			GetItemRect(NULL, NULL, rcThumb, GIR_THUMB);
			CPoint pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);
			if (pt.y > rcThumb.bottom)
			{
				if (m_pNotifyWnd->GetSafeHwnd() != NULL)
				{
					m_pNotifyWnd->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEDOWN, 0), (LPARAM)m_hWnd);
				}
				else
				{
					GetParent()->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEDOWN, 0), (LPARAM)m_hWnd);
				}
			}
		}
		else if (m_dwStyle & SBS_SIZEBOX)
			;
		else
		{
			if (m_pNotifyWnd->GetSafeHwnd() != NULL)
			{
				m_pNotifyWnd->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_PAGERIGHT, 0), (LPARAM)m_hWnd);
			}
			else
			{
				GetParent()->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_PAGERIGHT, 0), (LPARAM)m_hWnd);
			}
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

LRESULT CScrollBarEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
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
		return 1;
	}

	if (message == WM_LBUTTONDOWN)
	{
		// send WM_PARENTEVENT to parent
		int nID = GetDlgCtrlID();
		WPARAM wParam2 = MAKEWPARAM(message, nID);
		GetParent()->SendMessage(WM_PARENTEVENT, wParam2, lParam);

		CRect rcUp, rcDown, rcThumb;
		GetItemRect(rcUp, rcDown, rcThumb);
		CPoint point((DWORD)lParam);
		if (rcThumb.PtInRect(point))
		{
			m_bCaptureThumb = TRUE;
			UpdateWindow();
		}
		else if (rcUp.PtInRect(point))
		{
			m_bCaptureLTUPButton = TRUE;
			UpdateWindow();
		}
		else if (rcDown.PtInRect(point))
		{
			m_bCaptureRTDNButton = TRUE;
			UpdateWindow();
		}

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

	if (message == WM_LBUTTONUP)
	{
		// send WM_PARENTEVENT to parent
		int nID = GetDlgCtrlID();
		WPARAM wParam2 = MAKEWPARAM(message, nID);
		GetParent()->SendMessage(WM_PARENTEVENT, wParam2, lParam);

		m_bCaptureThumb = FALSE;
		m_bCaptureLTUPButton = FALSE;
		m_bCaptureRTDNButton = FALSE;
	}

	if (message == WM_MOUSEMOVE)
	{
		CRect rcUp, rcDown, rcThumb;
		GetItemRect(rcUp, rcDown, rcThumb);
		if (m_bCaptureThumb)
		{
			CPoint point((DWORD)lParam);
			if (!rcThumb.PtInRect(point))
			{
				if (m_bIsMouseOverThumb)
				{
					m_bIsMouseOverThumb = FALSE;
					UpdateWindow();
				}
			}
			else
			{
				if (!m_bIsMouseOverThumb)
				{
					m_bIsMouseOverThumb = TRUE;
					UpdateWindow();
				}
			}
		}
		else
		{
			CPoint point((DWORD)lParam);
			if (!rcThumb.PtInRect(point))
			{
				if (m_bIsMouseOverThumb)
				{
					m_bIsMouseOverThumb = FALSE;
					Invalidate();
				}
			}
			else
			{
				if (!m_bIsMouseOverThumb)
				{
					m_bIsMouseOverThumb = TRUE;
					// when set RGN for highlight stats, show the tool tip when mouse over the button(the tool tip and highlight stats at the same time work).
					// don't call Invalidate() after this "if" section
					Invalidate();
				}
			}
		}

		// left or up button
		if (m_bCaptureLTUPButton)
		{
			CPoint point((DWORD)lParam);
			if (!rcUp.PtInRect(point))
			{
				if (m_bIsMouseOverLTUPButton)
				{
					m_bIsMouseOverLTUPButton = FALSE;
					UpdateWindow();
				}
			}
			else
			{
				if (!m_bIsMouseOverLTUPButton)
				{
					m_bIsMouseOverLTUPButton = TRUE;
					UpdateWindow();
				}
			}
		}
		else
		{
			CPoint point((DWORD)lParam);
			if (!rcUp.PtInRect(point))
			{
				if (m_bIsMouseOverLTUPButton)
				{
					m_bIsMouseOverLTUPButton = FALSE;
					Invalidate();
				}
			}
			else
			{
				if (!m_bIsMouseOverLTUPButton)
				{
					m_bIsMouseOverLTUPButton = TRUE;
					// when set RGN for highlight stats, show the tool tip when mouse over the button(the tool tip and highlight stats at the same time work).
					// don't call Invalidate() after this "if" section
					Invalidate();
				}
			}
		}

		// right or down button
		if (m_bCaptureRTDNButton)
		{
			CPoint point((DWORD)lParam);
			if (!rcDown.PtInRect(point))
			{
				if (m_bIsMouseOverRTDNButton)
				{
					m_bIsMouseOverRTDNButton = FALSE;
					UpdateWindow();
				}
			}
			else
			{
				if (!m_bIsMouseOverRTDNButton)
				{
					m_bIsMouseOverRTDNButton = TRUE;
					UpdateWindow();
				}
			}
		}
		else
		{
			CPoint point((DWORD)lParam);
			if (!rcDown.PtInRect(point))
			{
				if (m_bIsMouseOverRTDNButton)
				{
					m_bIsMouseOverRTDNButton = FALSE;
					Invalidate();
				}
			}
			else
			{
				if (!m_bIsMouseOverRTDNButton)
				{
					m_bIsMouseOverRTDNButton = TRUE;
					// when set RGN for highlight stats, show the tool tip when mouse over the button(the tool tip and highlight stats at the same time work).
					// don't call Invalidate() after this "if" section
					Invalidate();
				}
			}
		}

		// when the button as a menu bar, can not accept WM_MOUSELEAVE message after the pop-up menu has been destroy
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = HOVER_DEFAULT;
		::TrackMouseEvent(&tme);

		// move this function call to "if" section for showing tool tip when the highlight mask bitmap is set.
		// InvalidateRect(NULL);
	}

	if (message == WM_MOUSELEAVE)
	{
		m_bIsMouseOverThumb = FALSE;
		m_bIsMouseOverLTUPButton = FALSE;
		m_bIsMouseOverRTDNButton = FALSE;
		InvalidateRect(NULL);
	}

	if (message == WM_KILLFOCUS || message == WM_CAPTURECHANGED)
	{
		BOOL bVisible = IsWindowVisible();
		if (bVisible)
		{
			SetRedraw(FALSE);
		}
		LRESULT lr = CWnd::WindowProc(message, wParam, lParam);
		if (bVisible)
		{
			SetRedraw(TRUE);
			Invalidate();
			UpdateWindow();
		}

		m_bCaptureThumb = FALSE;
		m_bCaptureLTUPButton = FALSE;
		m_bCaptureRTDNButton = FALSE;

		// stop work
		// Kill all timer
		KillTimer(T_LTUP);
		KillTimer(T_RTDN);
		KillTimer(T_SPACE1);
		KillTimer(T_SPACE2);
		KillTimer(T_SCROLLLTUP);
		KillTimer(T_SCROLLRTDN);
		KillTimer(T_SCROLLPAGEUP);
		KillTimer(T_SCROLLPAGEDOWN);

		if (m_nCapture == 2) // thumb
		{
			UINT uMsg = WM_HSCROLL;
			if (m_dwStyle & SBS_VERT)
			{
				uMsg = WM_VSCROLL;
			}

			if (m_pNotifyWnd->GetSafeHwnd() != NULL)
			{
				m_pNotifyWnd->SendMessage(uMsg, MAKEWPARAM(SB_ENDSCROLL, 0), (LPARAM)m_hWnd);
			}
			else
			{
				GetParent()->SendMessage(uMsg, MAKEWPARAM(SB_ENDSCROLL, 0), (LPARAM)m_hWnd);
			}
		}

		// Release the mouse capture
		ReleaseCapture();
		m_nCapture = -1;
		m_nDragOffset = 0;
		InvalidateRect(NULL);

		return lr;
	}

	return CWnd::WindowProc(message, wParam, lParam);
}
