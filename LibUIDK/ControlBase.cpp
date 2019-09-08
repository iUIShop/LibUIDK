// ControlBase.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CControlBase

CControlBase::CControlBase()
{
	m_bWLWnd = false;
	m_bDrawDefault = false;
	m_bBkCombine = true;
	m_himgCombineBk = NULL;
	m_eBkImageResizeMode = IRM_9GRID;
	m_ptBkImageResize.x = 2;
	m_ptBkImageResize.y = 2;
	m_lBkImageRepeatX = 1;
	m_lBkImageRepeatY = 1;
	m_bFgCombine = true;
	m_himgCombineFg = NULL;
	m_bDrawBkColor = false;
	m_crBk = RGB(255, 255, 255);

	m_eDrawParentPart = DPP_AUTO;
	m_btBkImgTransparent = 255;
	m_btFgImgTransparent = 255;
	m_pBindWnd = NULL;
	m_pBindWLWnd = NULL;
}

CControlBase::~CControlBase()
{
	m_vpDynamicCreatedWLControl.clear();
}

BOOL CControlBase::IsControlCreated() const
{
	if (m_bWLWnd)
	{
		return m_pBindWLWnd->IsCreated();
	}
	else
	{
		return (m_pBindWnd->GetSafeHwnd() != NULL);
	}
}

HWND CControlBase::GetHostWnd()
{
	ASSERT(FALSE);

	return NULL;
}

int CControlBase::BindStyle(LPCTSTR lpszStyleID)
{
	ASSERT(FALSE);
	return -1;
}

int CControlBase::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	ASSERT(FALSE);
	return -1;
}

int CControlBase::ReleaseObject()
{
	ReleaseIUIImage(m_himgCombineBk);
	m_himgCombineBk = NULL;
	ReleaseIUIImage(m_himgCombineFg);
	m_himgCombineFg = NULL;

	return 0;
}

int CControlBase::RouteWLMessage(HWND hWndThis, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message < WM_WINDOWLESSFIRST || message > WM_WINDOWLESSLAST)
	{
		return -1;
	}

	HWND hParent = ::GetParent(hWndThis);
	ASSERT(::IsWindow(hParent));

	if (message == WM_WL_COMMAND)
	{
		message = WM_COMMAND;		// Translate WM_WL_COMMAND to WM_COMMAND

		// The lParam is control handle, if lParam is not NULL,
		// CWnd::OnCommand call IsWindow to check the control. so set it to NULL.
		::SendMessage(hParent, message, wParam, NULL);
	}
	else
	{
		::SendMessage(hParent, message, wParam, lParam);
	}

	return 0;
}

int CControlBase::DrawWLChildren(CDC *pMemDC)
{
	if (IsWindowless())
	{
		size_t i = 0;

		// Draw windowless control create by dynamic.
		size_t nDynamicCreatedWLControlCount = m_vpDynamicCreatedWLControl.size();
		for (i = 0; i < nDynamicCreatedWLControlCount; ++i)
		{
			CWLWnd *pChild = m_vpDynamicCreatedWLControl[i];

			if (!pChild->IsCreated())
			{
				continue;
			}

			if (pChild->IsWindowVisible())
			{
				pChild->OnDraw(pMemDC);
			}
		}
	}
	else
	{
		CWLWnd *pChild = GetFirstWLWindow(GetBindWindow()->GetSafeHwnd());
		while (pChild != NULL)
		{
			if (!pChild->IsCreated())
			{
				continue;
			}

			if (pChild->IsWindowVisible())
			{
				// Apply child's region.
				HRGN hOldRgn = ApplyWLRgn(GetBindWindow(), pMemDC, pChild);

				pChild->OnDraw(pMemDC);

				::SelectClipRgn(pMemDC->GetSafeHdc(), hOldRgn);
				DeleteObject(hOldRgn);
			}

			pChild = GetNextWLWindow(pChild, GW_HWNDNEXT);
		}
	}

	return 0;
}

bool CControlBase::IsWindowless() const
{
	return m_bWLWnd;
}

CWnd *CControlBase::GetBindWindow()
{
	return m_pBindWnd;
}

CWLWnd *CControlBase::GetBindWindowless()
{
	return m_pBindWLWnd;
}

int CControlBase::Redraw(CWnd *pCtrl)
{
	if (pCtrl->GetSafeHwnd() == NULL)
	{
		return -1;
	}

	CWnd *pParent = pCtrl->GetParent();
	ASSERT(pParent != NULL);
	BOOL bUpdateLayeredWindow = (BOOL)pParent->SendMessage(WM_ISUPDATELAYEREDWINDOW);
	// If the parent call UpdateLayeredWindow. redraw the control by parent.
	if (bUpdateLayeredWindow)
	{
		pParent->SendMessage(WM_UPDATELAYEREDUIWND);
	}
	// If the parent not call UpdateLayeredWindow. redraw the control by itself.
	else
	{
		pCtrl->Invalidate();
	}

	return 0;
}

BOOL CControlBase::IsUpdateLayeredWindow()
{
	HWND hWndToplevel = NULL;
	if (m_bWLWnd)
	{
		HWND hWndHost = GetHostWnd();
		hWndToplevel = CWnd::GetSafeOwner_(hWndHost, NULL);
	}
	else
	{
		hWndToplevel = CWnd::GetSafeOwner_(m_pBindWnd->GetSafeHwnd(), NULL);
	}

	ASSERT(hWndToplevel != NULL);
	BOOL bUpdateLayeredWindow = (BOOL)SendMessage(hWndToplevel, WM_ISUPDATELAYEREDWINDOW, 0, 0);

	return bUpdateLayeredWindow;
}

int CControlBase::SetDrawParentPartMode(DRAW_PARENT_PART eDrawParent)
{
	m_eDrawParentPart = eDrawParent;

	return 0;
}

DRAW_PARENT_PART CControlBase::GetDrawParentPartMode()
{
	return m_eDrawParentPart;
}

int CControlBase::DrawParentPart(CWnd *pCtrl, CDC *pCtrlMemDC, const CRect &rcWin,
	BOOL *pbUpdateLayeredWindow, CWnd *pSpecParent/* = NULL*/)
{
	if (pCtrl == NULL || pCtrlMemDC == NULL)
	{
		return -1;
	}

	// Draw the parent background to control for alpha blend with background of the control.
	BOOL bUpdateLayeredWindow = IsUpdateLayeredWindow();

	if (pbUpdateLayeredWindow != NULL)
	{
		*pbUpdateLayeredWindow = bUpdateLayeredWindow;
	}

	if (!bUpdateLayeredWindow)
	{
		CWnd *pParent = pCtrl->GetParent();
		if (pSpecParent != NULL)
		{
			pParent = pSpecParent;
		}

		BOOL bCompatibleBitmap = TRUE;
		HBITMAP hParent = (HBITMAP)pParent->SendMessage(WM_GETBACKGROUND,
				0, (LPARAM)&bCompatibleBitmap);

		if (hParent == NULL)
		{
			return -1;
		}

		if (bCompatibleBitmap)
		{
			BitBltG(pCtrlMemDC->GetSafeHdc(), 0, 0, rcWin.Width(), rcWin.Height(),
				hParent, rcWin.left, rcWin.top, SRCCOPY);
		}
		else
		{
			// Resize the background
			POINT ptBkImageResize;
			ptBkImageResize.x = ptBkImageResize.y = 0;
			BOOL bRet = (BOOL)pParent->SendMessage(WM_GETBKIMGRESIZEPOINT,
					(WPARAM)pCtrl->GetSafeHwnd(), (LPARAM)&ptBkImageResize);

			POINT ptRepeatPixel;
			ptRepeatPixel.x = ptRepeatPixel.y = 0;
			bRet = (BOOL)pParent->SendMessage(WM_GETREPEATPIXEL,
					(WPARAM)pCtrl->GetSafeHwnd(), (LPARAM)&ptRepeatPixel);

			CRect rcClient;
			pParent->GetClientRect(rcClient);

			HBITMAP hBk = NULL;
			CIUIImage::NineGridBitmapG(hParent, ptBkImageResize, ptRepeatPixel.x, ptRepeatPixel.y,
				&hBk, rcClient.Width(), rcClient.Height());

			BitBltG(pCtrlMemDC->GetSafeHdc(), 0, 0, rcWin.Width(), rcWin.Height(),
				hBk, rcWin.left, rcWin.top, SRCCOPY);

			SafeDeleteHBITMAP(hBk);
		}
	}

	return 0;
}

int CControlBase::DrawParentBackground(CWnd *pCtrl, CDC *pCtrlMemDC,
	const CRect &rect, CWnd *pSpecParent/* = NULL*/)
{
	if (pCtrl == NULL || pCtrlMemDC == NULL)
	{
		return -1;
	}

	// Draw the parent background to control for alpha blend with background of the control.
	CWnd *pParent = pCtrl->GetParent();
	if (pSpecParent != NULL)
	{
		pParent = pSpecParent;
	}

	BOOL bCompatibleBitmap = TRUE;
	HBITMAP hParent = (HBITMAP)pParent->SendMessage(WM_GETBACKGROUND,
			0, (LPARAM)&bCompatibleBitmap);
	if (hParent == NULL)
	{
		return -1;
	}

	if (bCompatibleBitmap)
	{
		CRect rcWin;
		pCtrl->GetWindowRect(rcWin);
		pParent->ScreenToClient(rcWin);

		BitBltG(pCtrlMemDC->GetSafeHdc(), rect.left, rect.top, rect.Width(), rect.Height(),
			hParent, rcWin.left + rect.left, rcWin.top + rect.top, SRCCOPY);
	}
	else
	{
		// Resize the background
		POINT ptBkImageResize;
		ptBkImageResize.x = ptBkImageResize.y = 0;
		BOOL bRet = (BOOL)pParent->SendMessage(WM_GETBKIMGRESIZEPOINT,
				(WPARAM)pCtrl->GetSafeHwnd(), (LPARAM)&ptBkImageResize);

		POINT ptRepeatPixel;
		ptRepeatPixel.x = ptRepeatPixel.y = 0;
		bRet = (BOOL)pParent->SendMessage(WM_GETREPEATPIXEL,
				(WPARAM)pCtrl->GetSafeHwnd(), (LPARAM)&ptRepeatPixel);

		CRect rcClient;
		pParent->GetClientRect(rcClient);

		HBITMAP hBk = NULL;
		CIUIImage::NineGridBitmapG(hParent, ptBkImageResize, ptRepeatPixel.x, ptRepeatPixel.y,
			&hBk, rcClient.Width(), rcClient.Height());

		CRect rcWin;
		pCtrl->GetWindowRect(rcWin);
		pParent->ScreenToClient(rcWin);

		BitBltG(pCtrlMemDC->GetSafeHdc(), rect.left, rect.top, rect.Width(), rect.Height(),
			hBk, rcWin.left + rect.left, rcWin.top + rect.top, SRCCOPY);

		SafeDeleteHBITMAP(hBk);
	}

	return 0;
}

// rcArea2Control: rect relate to control's parent.
int CControlBase::DrawParentNonClientPart(CWnd *pCtrl, CDC *pdcNonClient,
	const CRect &rcArea2Control, BOOL *pbUpdateLayeredWindow, CWnd *pSpecParent/* = NULL*/)
{
	if (pCtrl == NULL || pdcNonClient == NULL)
	{
		return -1;
	}

	// Draw the parent background to control for alpha blend with background of the control.
	BOOL bUpdateLayeredWindow = IsUpdateLayeredWindow();

	if (pbUpdateLayeredWindow != NULL)
	{
		*pbUpdateLayeredWindow = bUpdateLayeredWindow;
	}

	if (!bUpdateLayeredWindow)
	{
		CWnd *pParent = pCtrl->GetParent();
		if (pSpecParent != NULL)
		{
			pParent = pSpecParent;
		}

		BOOL bCompatibleBitmap = TRUE;
		HBITMAP hParent = (HBITMAP)pParent->SendMessage(WM_GETBACKGROUND,
				0, (LPARAM)&bCompatibleBitmap);
		if (hParent == NULL)
		{
			return -1;
		}

		CRect rcWin;
		pCtrl->GetWindowRect(rcWin);
		pParent->ScreenToClient(rcWin);

		if (bCompatibleBitmap)
		{
			//			pdcNonClient->FillSolidRect(rcArea2Control, RGB(255, 0, 0));	// For test the rect.
			BitBltG(pdcNonClient->GetSafeHdc(),
				rcArea2Control.left, rcArea2Control.top,
				rcArea2Control.Width(), rcArea2Control.Height(),
				hParent,
				rcArea2Control.left + rcWin.left, rcArea2Control.top + rcWin.top, SRCCOPY);
		}
		else
		{
			// Resize the background
			POINT ptBkImageResize;
			ptBkImageResize.x = ptBkImageResize.y = 0;
			BOOL bRet = (BOOL)pParent->SendMessage(WM_GETBKIMGRESIZEPOINT,
					(WPARAM)pCtrl->GetSafeHwnd(), (LPARAM)&ptBkImageResize);

			POINT ptRepeatPixel;
			ptRepeatPixel.x = ptRepeatPixel.y = 0;
			bRet = (BOOL)pParent->SendMessage(WM_GETREPEATPIXEL,
					(WPARAM)pCtrl->GetSafeHwnd(), (LPARAM)&ptRepeatPixel);

			CRect rcClient;
			pParent->GetClientRect(rcClient);

			HBITMAP hBk = NULL;
			CIUIImage::NineGridBitmapG(hParent, ptBkImageResize, ptRepeatPixel.x, ptRepeatPixel.y,
				&hBk, rcClient.Width(), rcClient.Height());

			BitBltG(pdcNonClient->GetSafeHdc(),
				rcArea2Control.left, rcArea2Control.top,
				rcArea2Control.Width(), rcArea2Control.Height(),
				hBk,
				rcArea2Control.left + rcWin.left,
				rcArea2Control.top + rcWin.top, SRCCOPY);

			SafeDeleteHBITMAP(hBk);
		}
	}

	return 0;
}

int CControlBase::DrawCombineBackground(
	CDC *pDC,
	const CRect &rcDest,
	int nPart,
	int nPartIndex)
{
	if (!m_bBkCombine)
	{
		return -1;
	}

	if (m_eBkImageResizeMode == IRM_9GRID)
	{
		IUIPartNineGridBlt(pDC->GetSafeHdc(), rcDest,
			m_himgCombineBk, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY,
			nPart, nPartIndex);
	}
	else if (m_eBkImageResizeMode == IRM_STRETCH || m_eBkImageResizeMode == IRM_STRETCH_HIGH_QUALITY)
	{
		IUIPartStretchBlt(pDC->GetSafeHdc(), rcDest, m_himgCombineBk, nPart, nPartIndex,
			m_eBkImageResizeMode);
	}

	return 0;
}

int CControlBase::LoadBkCombineImage(LPCTSTR lpszImage)
{
	if (lpszImage == NULL)
	{
		return -1;
	}

	int nRet = m_himgCombineBk->AutoLoad(lpszImage);
	if (nRet != 0)
	{
		return -2;
	}
	if (m_himgCombineBk->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	m_himgCombineBk->ConvertToDPIImage(GetDPIX(), GetDPIY());
	m_bBkCombine = true;

	return 0;
}

int CControlBase::SetBkCombineImage(LPCTSTR lpszImageName, BOOL bRedraw/* = TRUE*/)
{
	m_bBkCombine = true;
	IUISetControlImage(&m_himgCombineBk, lpszImageName);

	if (bRedraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CControlBase::GetBkCombineImage(CString *pstrImageName)
{
	return IUIGetControlImage(m_himgCombineBk, pstrImageName);
}

bool CControlBase::IsBkCombineImage() const
{
	return m_bBkCombine;
}

int CControlBase::SetFgCombineImage(LPCTSTR lpszImageName, BOOL bRedraw/* = TRUE*/)
{
	m_bFgCombine = true;
	IUISetControlImage(&m_himgCombineFg, lpszImageName);

	if (bRedraw)
	{
		Redraw(m_pBindWnd);
	}

	return 0;
}

int CControlBase::GetFgCombineImage(CString *pstrImageName)
{
	return IUIGetControlImage(m_himgCombineFg, pstrImageName);
}

bool CControlBase::IsFgCombineImage() const
{
	return m_bFgCombine;
}

int CControlBase::SetDrawFrameControl(bool bDraw)
{
	m_bDrawDefault = bDraw;

	return 0;
}

bool CControlBase::IsDrawFrameControl() const
{
	return m_bDrawDefault;
}

int CControlBase::OnSetBkImgTransparent(BYTE btTransparent, BOOL bRedraw)
{
	m_btBkImgTransparent = btTransparent;

	if (bRedraw && m_pBindWnd->GetSafeHwnd() != NULL)
	{
		m_pBindWnd->Invalidate();
	}

	return 0;
}

int CControlBase::BindBkColor(const CTRLPROPERTIES *pCtrlProp)
{
	if (!CString((LPCTSTR)pCtrlProp->m_strResColorBk).IsEmpty())
	{
		SetDrawBkColor(TRUE);

		RESCOLORPROPERTIES *pColorPropBk = (RESCOLORPROPERTIES *)CUIMgr::GetColorResourceItem(
				(LPCTSTR)pCtrlProp->m_strResColorBk);
		if (pColorPropBk != NULL)
		{
			COLORREF crBk = pColorPropBk->m_crColor;
			SetBackgroundColor(crBk);
		}
	}

	return 0;
}

int CControlBase::SetBkImageTransparent(BYTE btTransparent, BOOL bRedraw/* = TRUE*/)
{
	if (m_pBindWnd->GetSafeHwnd() == NULL)
	{
		return -1;
	}

	return (int)m_pBindWnd->SendMessage(WM_SETBKIMGTRANSPARENT, btTransparent, bRedraw);
}

BYTE CControlBase::GetBkImageTransparent() const
{
	return m_btBkImgTransparent;
}

int CControlBase::SetBkImageResizeMode(IMAGE_RESIZE_MODE eIrm)
{
	m_eBkImageResizeMode = eIrm;
	return 0;
}

IMAGE_RESIZE_MODE CControlBase::GetBkImageResizeMode() const
{
	return m_eBkImageResizeMode;
}

int CControlBase::SetBkImageResizePoint(POINT pt)
{
	m_ptBkImageResize = pt;
	return 0;
}

int CControlBase::GetBkImageResizePoint(LPPOINT lpPt) const
{
	if (lpPt == NULL)
	{
		return -1;
	}

	*lpPt = m_ptBkImageResize;

	return 0;
}

int CControlBase::SetBkImageRepeatX(LONG nRepeat)
{
	m_lBkImageRepeatX = nRepeat;
	return 0;

}

LONG CControlBase::GetBkImageRepeatX() const
{
	return m_lBkImageRepeatX;
}

int CControlBase::SetBkImageRepeatY(LONG nRepeat)
{
	m_lBkImageRepeatY = nRepeat;
	return 0;
}

LONG CControlBase::GetBkImageRepeatY() const
{
	return m_lBkImageRepeatY;
}

int CControlBase::SetDrawBkColor(BOOL bDraw)
{
	m_bDrawBkColor = bDraw ? true : false;

	return 0;
}

BOOL CControlBase::IsDrawBkColor() const
{
	return m_bDrawBkColor;
}

int CControlBase::SetBackgroundColor(COLORREF cr)
{
	m_crBk = cr;

	return 0;
}

COLORREF CControlBase::GetBackgroundColor() const
{
	return m_crBk;
}
