// skinsliderctrl.cpp : implementation file
//

#include "stdafx.h"
#pragma comment (lib, "Msimg32.lib") // for TransparentBlt

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct SLIDERMEMBER
{
	SLIDERMEMBER()
	{
		int i = 0;
		for (i = 0; i < 2; ++i)
		{
			m_himgBk[i] = NULL;
		}

		for (i = 0; i < 4 ; i++)
		{
			m_hbmpThumb[i] = NULL;
		}

		m_bUseMark = FALSE;
		m_bVert = FALSE;
		m_nSplit = 0;
	}

	int Release()
	{
		m_brush.DeleteObject();

		int i = 0;
		for (i = 0; i < 2; ++i)
		{
			ReleaseIUIImage(m_himgBk[i]);
			m_himgBk[i] = NULL;
		}

		for (i = 0; i < 4 ; i++)
		{
			SafeDeleteHBITMAP(m_hbmpThumb[i]);
		}

		m_bUseMark = FALSE;
		m_bVert = FALSE;
		m_nSplit = 0;

		return 0;
	}

	CBrush m_brush;
	HIUIIMAGE m_himgBk[2];
	HBITMAP m_hbmpThumb[4];
	BOOL m_bUseMark;
	BOOL m_bVert; // Is has TBS_VERT style
	int m_nSplit;
};


/////////////////////////////////////////////////////////////////////////////
// CSkinSliderCtrl

CSkinSliderCtrl::CSkinSliderCtrl()
{
	m_pBindWnd = this;

	m_pMember = new SLIDERMEMBER;
	SLIDERMEMBER *pMember = (SLIDERMEMBER *)m_pMember;
}

CSkinSliderCtrl::~CSkinSliderCtrl()
{
	SLIDERMEMBER *pMember = (SLIDERMEMBER *)m_pMember;

	if (m_pMember != NULL)
	{
		delete (SLIDERMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

int CSkinSliderCtrl::BindStyle(LPCTSTR lpszStyleID)
{
	SLIDERMEMBER *pMember = (SLIDERMEMBER *)m_pMember;

	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_SLIDER, lpszStyleID);
	return BindStyle(pCtrlProp);
}

int CSkinSliderCtrl::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	SLIDERMEMBER *pMember = (SLIDERMEMBER *)m_pMember;

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
	if (ect != CT_STYLE_SLIDER && ect != CT_SLIDER)
	{
		return -3;
	}

	SLDPROPERTIES *pSldProp = (SLDPROPERTIES *)pCtrlProp;

	// Background bitmap
	if (pSldProp->m_bSpecifyBackgroundImages)
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
			SetImages(CONTROL_STATE_UNCHECKED_ND, strImageName[1], strImageName[2]);
		}

		SetBkImageResizeMode(pCtrlProp->m_eBkImageResizeMode);
		SetBkImageResizePoint(pCtrlProp->m_ptImageResize);
	}

	return 0;
}

BEGIN_MESSAGE_MAP(CSkinSliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(CSkinSliderCtrl)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdraw)
	ON_MESSAGE(TBM_GETTHUMBRECT, OnGetThumbRect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSkinSliderCtrl message handlers

int CSkinSliderCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SLIDERMEMBER *pMember = (SLIDERMEMBER *)m_pMember;

	if (CSliderCtrl::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// TODO: Add your specialized creation code here

	// Init m_bHorz
	LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	pMember->m_bVert = lStyle & TBS_VERT;

	return 0;
}

HBRUSH CSkinSliderCtrl::CtlColor(CDC *pDC, UINT nCtlColor)
{
	SLIDERMEMBER *pMember = (SLIDERMEMBER *)m_pMember;

	// TODO: Change any attributes of the DC here

	// Draw the whole background except channel, the channel drawn in OnCustomdraw.
	if (pMember->m_brush.GetSafeHandle() != NULL)
	{
		pMember->m_brush.DeleteObject();
	}

	CRect rcClient;
	GetClientRect(rcClient);

	// prepare the bitmap for create brush object
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bmpBrush;
	bmpBrush.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	ASSERT(bmpBrush.GetSafeHandle() != NULL);
	CBitmap *pBmpOld = memDC.SelectObject(&bmpBrush);

	CDC memDC1;
	memDC1.CreateCompatibleDC(pDC);
	HBITMAP pBmpBkOld = NULL;

	CRect rcWin;
	GetWindowRect(rcWin);
	GetParent()->ScreenToClient(rcWin);

	HIUIIMAGE himgStretch[2];
	BOOL bNeedResize = (m_ptBkImageResize.x >= 0 || m_ptBkImageResize.y >= 0);
	BOOL bResized = FALSE;
	if (bNeedResize)
	{
		CIUIImage::NineGridBitmap(pMember->m_himgBk[0],
			m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
			&himgStretch[0], rcWin.Width(), rcWin.Height());
		CIUIImage::NineGridBitmap(pMember->m_himgBk[1],
			m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
			&himgStretch[1], rcWin.Width(), rcWin.Height());
		bResized = TRUE;
	}
	if (pMember->m_bVert)
	{
		if (pMember->m_bUseMark)
		{
			// Top area draw the mark image (use highlight background)
			pBmpBkOld = (HBITMAP)memDC1.SelectObject(bNeedResize ? himgStretch[1] : pMember->m_himgBk[1]);
			memDC.BitBlt(0, 0, rcClient.Width(), pMember->m_nSplit, &memDC1, 0, 0, SRCCOPY);
			memDC1.SelectObject(pBmpBkOld);

			// Bottom area draw the normal image
			pBmpBkOld = (HBITMAP)memDC1.SelectObject(bNeedResize ? himgStretch[0] : pMember->m_himgBk[0]);
			memDC.BitBlt(0, pMember->m_nSplit, rcClient.Width(), rcClient.Height() - pMember->m_nSplit,
				&memDC1,
				0, pMember->m_nSplit, SRCCOPY);
			memDC1.SelectObject(pBmpBkOld);
		}
		else
		{
			pBmpBkOld = (HBITMAP)memDC1.SelectObject(bNeedResize ? himgStretch[0] : pMember->m_himgBk[0]);
			memDC.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC1, 0, 0, SRCCOPY);
			memDC1.SelectObject(pBmpBkOld);
		}
	}
	else
	{
		if (pMember->m_bUseMark)
		{
			// Left area draw the mark image (use highlight background)
			pBmpBkOld = (HBITMAP)memDC1.SelectObject(bNeedResize ? himgStretch[1] : pMember->m_himgBk[1]);
			memDC.BitBlt(0, 0, pMember->m_nSplit, rcClient.Height(), &memDC1, 0, 0, SRCCOPY);
			memDC1.SelectObject(pBmpBkOld);

			// Right area draw the normal image
			pBmpBkOld = (HBITMAP)memDC1.SelectObject(bNeedResize ? himgStretch[0] : pMember->m_himgBk[0]);
			memDC.BitBlt(pMember->m_nSplit, 0, rcClient.Width() - pMember->m_nSplit, rcClient.Height(), &memDC1, pMember->m_nSplit, 0, SRCCOPY);
			memDC1.SelectObject(pBmpBkOld);
		}
		else
		{
			pBmpBkOld = (HBITMAP)memDC1.SelectObject(bNeedResize ? himgStretch[0] : pMember->m_himgBk[0]);
			memDC.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC1, 0, 0, SRCCOPY);
			memDC1.SelectObject(pBmpBkOld);
		}
	}

	// create the brush
	memDC.SelectObject(pBmpOld);
	pMember->m_brush.CreatePatternBrush(&bmpBrush);

	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return pMember->m_brush;
}

void CSkinSliderCtrl::OnCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	SLIDERMEMBER *pMember = (SLIDERMEMBER *)m_pMember;

	// TODO: Add your control notification handler code here
	NMCUSTOMDRAW *pNMCD = (NMCUSTOMDRAW *)pNMHDR;
	switch (pNMCD->dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		if (pNMCD->dwItemSpec == TBCD_THUMB)
		{
			CDC *pDC = CDC::FromHandle(pNMCD->hdc);
			*pResult = CDRF_SKIPDEFAULT;

			BITMAP bmp;
			if (pMember->m_hbmpThumb[0] != NULL)
			{
				ASSERT(OBJ_BITMAP == GetObjectType(pMember->m_hbmpThumb[0]));
				int nRet = ::GetObject(pMember->m_hbmpThumb[0], sizeof(BITMAP), &bmp);
				if (0 == nRet)
				{
					return;
				}
			}
			else
			{
				return;
			}

			int nBKWidth = bmp.bmWidth;

			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			HBITMAP hbmpOld = NULL;

			CRect rcThumb = pNMCD->rc;
			if (pMember->m_bVert)
			{
				pMember->m_nSplit = rcThumb.top + rcThumb.Height() / 2;
			}
			else
			{
				pMember->m_nSplit = rcThumb.left + rcThumb.Width() / 2;
			}

			BOOL bIsEnable = IsWindowEnabled();
			if (!bIsEnable)
			{
				// disable
				hbmpOld = (HBITMAP)memDC.SelectObject(pMember->m_hbmpThumb[3]);
			}
			else
			{
				if (GetCapture() == this)
				{
					// selected
					hbmpOld = (HBITMAP)memDC.SelectObject(pMember->m_hbmpThumb[2]);
				}
				else
				{
					BOOL bIsOver = IsMouseOverThumb();
					if (bIsOver)
					{
						// highlight
						hbmpOld = (HBITMAP)memDC.SelectObject(pMember->m_hbmpThumb[1]);
					}
					else
					{
						// normal
						hbmpOld = (HBITMAP)memDC.SelectObject(pMember->m_hbmpThumb[0]);
					}
				}
			}
			TransparentBlt(pNMCD->hdc, rcThumb.left, rcThumb.top, rcThumb.Width(), rcThumb.Height(), memDC.GetSafeHdc(),
				0, 0, nBKWidth, bmp.bmHeight, RGB(255, 0, 255));
			memDC.SelectObject(hbmpOld);
		}
		else if (pNMCD->dwItemSpec == TBCD_CHANNEL)
		{
			// origin of pDC is from client origin
			CDC *pDC = CDC::FromHandle(pNMCD->hdc);
			CRect rcClient;
			GetClientRect(rcClient);
			CRect rcChannel(pNMCD->rc);

			CRect rcWin;
			GetWindowRect(rcWin);
			GetParent()->ScreenToClient(rcWin);

			BOOL bResized = FALSE;
			HIUIIMAGE himgStretch[2] = {NULL};
			CIUIImage::NineGridBitmap(pMember->m_himgBk[0],
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				&himgStretch[0], rcWin.Width(), rcWin.Height());
			CIUIImage::NineGridBitmap(pMember->m_himgBk[1],
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
				&himgStretch[1], rcWin.Width(), rcWin.Height());
			bResized = TRUE;

			if (pMember->m_bUseMark)
			{
				CDC memDC;
				memDC.CreateCompatibleDC(pDC);
				CBitmap memBmp;
				memBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
				ASSERT(memBmp.GetSafeHandle() != NULL);
				CBitmap *pBmpOld = memDC.SelectObject(&memBmp);

				// draw channel background image to memDC
				CDC memDC2;
				memDC2.CreateCompatibleDC(pDC);
				HBITMAP phBmpBKOld = (HBITMAP)memDC2.SelectObject(himgStretch[0]);
				memDC.BitBlt(rcChannel.left, rcChannel.top, rcChannel.Width(), rcChannel.Height(),
					&memDC2, rcChannel.left, rcChannel.top, SRCCOPY);
				memDC2.SelectObject(phBmpBKOld);

				// draw mark image to memDC
				if (!pMember->m_himgBk[1]->IsHasAlphaChannel())
				{
					CDC memDC3;
					memDC3.CreateCompatibleDC(pDC);
					HBITMAP hbmpOld = (HBITMAP)memDC3.SelectObject(himgStretch[1]);
					CRect rcThumb;
					this->GetThumbRect(rcThumb);
					if (pMember->m_bVert)
					{
						memDC.BitBlt(rcChannel.left, rcChannel.top, rcChannel.Width(), rcThumb.top,
							&memDC3, rcChannel.left, rcChannel.top, SRCCOPY);
					}
					else
					{
						memDC.BitBlt(rcChannel.left, rcChannel.top, rcThumb.left, rcChannel.Height(),
							&memDC3, rcChannel.left, rcChannel.top, SRCCOPY);
					}
					memDC3.SelectObject(hbmpOld);
				}

				// memDC to pDC
				pDC->BitBlt(rcChannel.left, rcChannel.top, rcChannel.Width(), rcChannel.Height(),
					&memDC, rcChannel.left, rcChannel.top, SRCCOPY);
				memDC.SelectObject(pBmpOld);
			}
			else
			{
				CDC memDC;
				memDC.CreateCompatibleDC(pDC);
				HBITMAP hBmpBKOld = (HBITMAP)memDC.SelectObject(himgStretch[0]);
				pDC->BitBlt(rcChannel.left, rcChannel.top, rcChannel.Width(), rcChannel.Height(),
					&memDC, rcChannel.left, rcChannel.top, SRCCOPY);
				memDC.SelectObject(hBmpBKOld);
			}

			*pResult = CDRF_SKIPDEFAULT;
		}
		else if (pNMCD->dwItemSpec == TBCD_TICS)
		{
			*pResult = CDRF_SKIPDEFAULT;
		}

		break;
	default:
		break;
	}
}

BOOL CSkinSliderCtrl::IsMouseOverThumb()
{
	SLIDERMEMBER *pMember = (SLIDERMEMBER *)m_pMember;

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(&ptMouse);

	CRect rcThumb;
	GetThumbRect(rcThumb);

	return FALSE;
	return rcThumb.PtInRect(ptMouse);
}

LRESULT CSkinSliderCtrl::OnGetThumbRect(WPARAM wParam, LPARAM lParam)
{
	SLIDERMEMBER *pMember = (SLIDERMEMBER *)m_pMember;

	if (IsWindowVisible())
	{
		SetRedraw(FALSE);
		Default();
		SetRedraw(TRUE);
		Invalidate();
	}
	else
	{
		Default();
	}

	return 0;
}

LRESULT CSkinSliderCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	SLIDERMEMBER *pMember = (SLIDERMEMBER *)m_pMember;

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

	return CSliderCtrl::WindowProc(message, wParam, lParam);
}

int CSkinSliderCtrl::SetImages(UINT uMask, LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameD)
{
	SLIDERMEMBER *pMember = (SLIDERMEMBER *)m_pMember;

	m_bBkCombine = false;
	IUISetControlImage2(uMask, pMember->m_himgBk, lpszImageNameN, lpszImageNameD);

	return 0;
}

int CSkinSliderCtrl::GetImages(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameD)
{
	SLIDERMEMBER *pMember = (SLIDERMEMBER *)m_pMember;

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
