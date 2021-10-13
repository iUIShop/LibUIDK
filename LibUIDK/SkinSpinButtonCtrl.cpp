// SkinSpinButtonCtrl.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


struct SPINMEMBER
{
	SPINMEMBER()
	{
		m_nOver = 0;
		m_nDown = 0;
		m_bHorz = FALSE;

		for (int i = 0; i < 4; ++i)
		{
			m_himgBk[i] = NULL;
		}
	}

	int Release()
	{
		m_nOver = 0;
		m_nDown = 0;
		m_bHorz = FALSE;

		for (int i = 0; i < 4; ++i)
		{
			ReleaseIUIImage(m_himgBk[i]);
			m_himgBk[i] = NULL;
		}

		return 0;
	}

	int m_nOver; // 1: button1; 2: button2; 0: not over
	int m_nDown; // 1: button1; 2: button2; 0: not down
	BOOL m_bHorz;

	HIUIIMAGE m_himgBk[4];
};

/////////////////////////////////////////////////////////////////////////////
// CSkinSpinButtonCtrl

CSkinSpinButtonCtrl::CSkinSpinButtonCtrl()
{
	m_pBindWnd = this;

	m_pMember = new SPINMEMBER;
	SPINMEMBER *pMember = (SPINMEMBER *)m_pMember;
}

CSkinSpinButtonCtrl::~CSkinSpinButtonCtrl()
{
	SPINMEMBER *pMember = (SPINMEMBER *)m_pMember;

	CControlBase::ReleaseObject();
	pMember->Release();

	if (m_pMember != NULL)
	{
		delete (SPINMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

int CSkinSpinButtonCtrl::BindStyle(LPCTSTR lpszStyleID)
{
	SPINMEMBER *pMember = (SPINMEMBER *)m_pMember;

	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (lpszStyleID == NULL)
	{
		return -2;
	}

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_SPIN, lpszStyleID);
	return BindStyle(pCtrlProp);

	return 0;
}

int CSkinSpinButtonCtrl::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	SPINMEMBER *pMember = (SPINMEMBER *)m_pMember;

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
	if (ect != CT_STYLE_SPIN && ect != CT_SPIN)
	{
		return -3;
	}

	SPNPROPERTIES *pSpnProp = (SPNPROPERTIES *)pCtrlProp;

	// images
	if (pSpnProp->m_bSpecifyBackgroundImages)
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
	}

	return 0;
}


BEGIN_MESSAGE_MAP(CSkinSpinButtonCtrl, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CSkinSpinButtonCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// protected

int CSkinSpinButtonCtrl::OnDraw(CDC *pMemDC)
{
	SPINMEMBER *pMember = (SPINMEMBER *)m_pMember;

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcWin;
	GetWindowRect(rcWin);
	GetParent()->ScreenToClient(rcWin);

	int nLeftUpStateIndex = 0;
	if (IsWindowEnabled())
	{
		if (pMember->m_nOver == 1)
		{
			if (pMember->m_nDown == 1)
			{
				nLeftUpStateIndex = 2;
			}
			else
			{
				nLeftUpStateIndex = 1;
			}
		}
		else
		{
			nLeftUpStateIndex = 0;
		}
	}
	else
	{
		nLeftUpStateIndex = 3;
	}

	int nRightDownStateIndex = 0;
	if (IsWindowEnabled())
	{
		if (pMember->m_nOver == 2)
		{
			if (pMember->m_nDown == 2)
			{
				nRightDownStateIndex = 2;
			}
			else
			{
				nRightDownStateIndex = 1;
			}
		}
		else
		{
			nRightDownStateIndex = 0;
		}
	}
	else
	{
		nRightDownStateIndex = 3;
	}

	// Draw the parent background to control for alpha blend with background of the control.
	BOOL bDrawParentPart = TRUE;
	if (m_bBkCombine)
	{
		bDrawParentPart = IsImageHasAlphaChanne(m_himgCombineBk);
	}
	else
	{
		bDrawParentPart = IsImageHasAlphaChanne(pMember->m_himgBk[nLeftUpStateIndex])
			|| IsImageHasAlphaChanne(pMember->m_himgBk[nRightDownStateIndex]);
	}

	if (bDrawParentPart)
	{
		BOOL bUpdateLayeredWindow = FALSE;

		// If the spin control as date time control's child. the parent part is parent's parent.

		CWnd *pBackgroundParent = NULL;
		CRect rect = rcWin;
		TCHAR szName[32] = {0};
		::GetClassName(GetParent()->GetSafeHwnd(), szName, 31);
		if (_tcscmp(szName, _T("SysDateTimePick32")) == 0)
		{
			pBackgroundParent = GetParent()->GetParent();

			GetWindowRect(rect);
			pBackgroundParent->ScreenToClient(rect);
		}

		DrawParentPart(this, pMemDC, rect, &bUpdateLayeredWindow, pBackgroundParent);

		// If parent has called UpdateLayeredWindow, use direct-ui mode, adjust the rect
		if (bUpdateLayeredWindow)
		{
			rcClient = rcWin;
		}
	}

	//
	// Draw range1 (up or left)
	//
	CRect rcDest(0, 0,
		pMember->m_bHorz ? rcClient.Width() / 2 :  rcClient.Width(),
		pMember->m_bHorz ? rcClient.Height() : rcClient.Height() / 2);

	if (m_bBkCombine)
	{
		if (m_himgCombineBk->GetSafeHBITMAP() != NULL)
		{
			CRect rcSrc;
			rcSrc.left = m_himgCombineBk->GetWidth() * nLeftUpStateIndex / 4;
			rcSrc.top = 0;

			rcSrc.right = rcSrc.left
				+ (pMember->m_bHorz ? m_himgCombineBk->GetWidth() / 8 : m_himgCombineBk->GetWidth() / 4);

			rcSrc.bottom = rcSrc.top
				+ (pMember->m_bHorz ? m_himgCombineBk->GetHeight() : m_himgCombineBk->GetHeight() / 2);

			IUIPartNineGridBlt(pMemDC->GetSafeHdc(), rcDest, m_himgCombineBk,
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY, rcSrc);
		}
	}
	else
	{
		if (pMember->m_himgBk[nLeftUpStateIndex] != NULL)
		{
			CRect rcSrc;
			rcSrc.left = 0;
			rcSrc.top = 0;

			rcSrc.right = rcSrc.left
				+ (pMember->m_bHorz ? pMember->m_himgBk[nLeftUpStateIndex]->GetWidth() / 2 : pMember->m_himgBk[nLeftUpStateIndex]->GetWidth());

			rcSrc.bottom = rcSrc.top
				+ (pMember->m_bHorz ? pMember->m_himgBk[nLeftUpStateIndex]->GetHeight() : pMember->m_himgBk[nLeftUpStateIndex]->GetHeight() / 2);

			IUIPartNineGridBlt(pMemDC->GetSafeHdc(), rcDest,
				pMember->m_himgBk[nLeftUpStateIndex],
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY, rcSrc);
		}
	}

	//
	// Draw range2 (bottom or right)
	//
	rcDest.SetRect(pMember->m_bHorz ? rcClient.Width() / 2 : 0,
		pMember->m_bHorz ? 0 : rcClient.Height() / 2,
		rcClient.right,
		rcClient.bottom);

	if (m_bBkCombine)
	{
		if (m_himgCombineBk != NULL)
		{
			CRect rcSrc;

			rcSrc.left = pMember->m_bHorz
				? m_himgCombineBk->GetWidth() * (nRightDownStateIndex * 2 + 1) / 8
				: m_himgCombineBk->GetWidth() * nRightDownStateIndex / 4;

			rcSrc.right = m_himgCombineBk->GetWidth() * (nRightDownStateIndex + 1) / 4;
			rcSrc.top = pMember->m_bHorz ? 0 : m_himgCombineBk->GetHeight() / 2;
			rcSrc.bottom = m_himgCombineBk->GetHeight();

			IUIPartNineGridBlt(pMemDC->GetSafeHdc(), rcDest, m_himgCombineBk,
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY, rcSrc);
		}
	}
	else
	{
		if (pMember->m_himgBk[nRightDownStateIndex] != NULL)
		{
			CRect rcSrc;
			rcSrc.left = pMember->m_bHorz ? pMember->m_himgBk[nRightDownStateIndex]->GetWidth() / 2 : 0;
			rcSrc.right = pMember->m_himgBk[nRightDownStateIndex]->GetWidth();
			rcSrc.top = pMember->m_bHorz ? 0 : pMember->m_himgBk[nRightDownStateIndex]->GetHeight() / 2;
			rcSrc.bottom = pMember->m_himgBk[nRightDownStateIndex]->GetHeight();

			IUIPartNineGridBlt(pMemDC->GetSafeHdc(), rcDest,
				pMember->m_himgBk[nRightDownStateIndex],
				m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY, rcSrc);
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Public member functions
int CSkinSpinButtonCtrl::SetImages(UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH, LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD)
{
	SPINMEMBER *pMember = (SPINMEMBER *)m_pMember;

	ASSERT(m_hWnd != NULL);

	m_bBkCombine = false;
	IUISetControlImage4(uMask, pMember->m_himgBk,
		lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);

	return 0;
}

int CSkinSpinButtonCtrl::GetImages(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameH,
	CString *pstrImageNameS, CString *pstrImageNameD)
{
	SPINMEMBER *pMember = (SPINMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bBkCombine;
	IUIGetControlImage4(uMask,
		m_himgCombineBk, pMember->m_himgBk, *pbCombineImage,
		pstrCombineImageName,
		pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CSkinSpinButtonCtrl message handlers

LRESULT CSkinSpinButtonCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	SPINMEMBER *pMember = (SPINMEMBER *)m_pMember;

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

	if (message == WM_CREATE)
	{
		LRESULT lr = CSpinButtonCtrl::WindowProc(message, wParam, lParam);
		if (lr == -1)
		{
			return -1;
		}

		LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		// The UDS_HORZ style can't be modify when the control be created
		pMember->m_bHorz = (lStyle & UDS_HORZ) ? TRUE : FALSE;

		return 0;
	}

	if (message == WM_ERASEBKGND)
	{
		return TRUE;
	}

	if (message == WM_LBUTTONDOWN)
	{
		CPoint point((DWORD)lParam);
		CRect rcClient;
		GetClientRect(rcClient);

		CRect rc1(rcClient);
		CRect rc2(rcClient);
		if (pMember->m_bHorz)
		{
			rc1.DeflateRect(0, 0, rcClient.Width() / 2, 0);
			rc2.DeflateRect(rcClient.Width() / 2, 0, 0, 0);
		}
		else
		{
			rc1.DeflateRect(0, 0, 0, rcClient.Height() / 2);
			rc2.DeflateRect(0, rcClient.Height() / 2, 0, 0);
		}

		if (pMember->m_nDown != 1)
		{
			if (rc1.PtInRect(point))
			{
				pMember->m_nDown = 1;
				InvalidateRect(NULL);
			}
		}

		if (pMember->m_nDown != 2)
		{
			if (rc2.PtInRect(point))
			{
				pMember->m_nDown = 2;
				InvalidateRect(NULL);
			}
		}
	}

	if (message == WM_LBUTTONUP)
	{
		pMember->m_nDown = 0;
		InvalidateRect(NULL);
	}

	if (message == WM_MOUSELEAVE)
	{
		pMember->m_nOver = 0;
		InvalidateRect(NULL);
	}

	if (message == WM_MOUSEMOVE)
	{
		CPoint point((DWORD)lParam);
		CRect rcClient;
		GetClientRect(rcClient);

		CRect rc1(rcClient);
		CRect rc2(rcClient);
		if (pMember->m_bHorz)
		{
			rc1.DeflateRect(0, 0, rcClient.Width() / 2, 0);
			rc2.DeflateRect(rcClient.Width() / 2, 0, 0, 0);
		}
		else
		{
			rc1.DeflateRect(0, 0, 0, rcClient.Height() / 2);
			rc2.DeflateRect(0, rcClient.Height() / 2, 0, 0);
		}
		if (pMember->m_nOver == 0) // 0->1 or 0->2
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = m_hWnd;
			tme.dwHoverTime = HOVER_DEFAULT;
			::TrackMouseEvent(&tme);

			if (rc1.PtInRect(point))
			{
				pMember->m_nOver = 1;
			}
			if (rc2.PtInRect(point))
			{
				pMember->m_nOver = 2;
			}

			InvalidateRect(NULL);
		}
		else
		{
			if (pMember->m_nOver != 1)
			{
				if (rc1.PtInRect(point))
				{
					pMember->m_nOver = 1;
					InvalidateRect(NULL);
				}
			}

			if (pMember->m_nOver != 2)
			{
				if (rc2.PtInRect(point))
				{
					pMember->m_nOver = 2;
					InvalidateRect(NULL);
				}
			}

			if (!rcClient.PtInRect(point))
			{
				pMember->m_nOver = 0;
			}
		}
	}

	if (message == WM_PAINT)
	{
		CPaintDC dc(this); // device context for painting

		// TODO: Add your message handler code here

		CRect rcClient;
		GetClientRect(rcClient);

		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);

		CBitmap bmpMem;
		bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

		CBitmap *pbmpMemOld = dcMem.SelectObject(&bmpMem);

		// Draw content to dcMem...
		OnDraw(&dcMem);

		dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);
		dcMem.SelectObject(pbmpMemOld);
	}

	return CSpinButtonCtrl::WindowProc(message, wParam, lParam);
}
