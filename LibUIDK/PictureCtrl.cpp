// PictureCtrl.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define IDT_ANIMATE_IMAGE				1


struct PICCTRLMEMBER
{
	PICCTRLMEMBER()
	{
		m_hBackground = NULL;
		m_ptBkImageResize.x = m_ptBkImageResize.y = 2;

		m_hForeground = NULL;
		m_ptFgImageResize.x = m_ptFgImageResize.y = 2;
		m_rcFgMargin.SetRect(0, 0, 0, 0);
		m_eFgImageResizeMode = IRM_STRETCH;
		m_eForegroundAlignHor = FAH_CENTER;
		m_eForegroundAlignVer = FAV_CENTER;

		m_bAnimationMode = false;
		m_nCurTimerTick = 0;
		m_nTimerElapse = 40;
	}

	HIUIIMAGE m_hBackground;
	CPoint m_ptBkImageResize;

	HIUIIMAGE m_hForeground;
	CPoint m_ptFgImageResize;

	CRect m_rcFgMargin;
	IMAGE_RESIZE_MODE m_eFgImageResizeMode;
	FOREGROUND_ALIGN_HOR m_eForegroundAlignHor;
	FOREGROUND_ALIGN_VER m_eForegroundAlignVer;

	// The following variable for animating image.
	bool m_bAnimationMode;
	int m_nCurTimerTick;					// Times that timer implement. same as current Frame.
	int m_nTimerElapse;						// See SetTimer.
	std::vector<std::pair<int, BYTE> > m_vAnimationData;	// first: Stretch Rate, second: transparent.
};

/////////////////////////////////////////////////////////////////////////////
// CPictureCtrl

CPictureCtrl::CPictureCtrl()
{
	m_pBindWnd = this;

	m_pMember = new PICCTRLMEMBER;
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;
}

CPictureCtrl::~CPictureCtrl()
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	ReleaseIUIImage(pMember->m_hBackground);
	pMember->m_hBackground = NULL;

	ReleaseIUIImage(pMember->m_hForeground);
	pMember->m_hForeground = NULL;

	if (m_pMember != NULL)
	{
		delete (PICCTRLMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}


BEGIN_MESSAGE_MAP(CPictureCtrl, CWnd)
	//{{AFX_MSG_MAP(CPictureCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// public

BOOL CPictureCtrl::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	CString strMyClass;
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;
	strMyClass = WC_PICTUREVIEW;
	wndcls.lpszClassName = strMyClass.GetBuffer(strMyClass.GetLength());

	// Register the new class and exit if it fails
	if (!AfxRegisterClass(&wndcls))
	{
		TRACE(_T("[LibUIDK]===Class \"%s\" Registration Failed\n"), WC_PICTUREVIEW);
		return FALSE;
	}

	CWnd *pWnd = this;
	return pWnd->Create(WC_PICTUREVIEW, NULL, dwStyle, rect, pParentWnd, nID);
}

BOOL CPictureCtrl::Create(DWORD dwStyle, CRectCtrl *pRectCtrl, CWnd *pParentWnd, UINT nID)
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	ASSERT(pRectCtrl != NULL && pRectCtrl->IsCreated());

	CRect rect;
	pRectCtrl->GetWindowRect(rect);
	pParentWnd->ScreenToClient(rect);

	BOOL bRet = CPictureCtrl::Create(dwStyle, rect, pParentWnd, nID);
	if (!bRet)
	{
		return FALSE;
	}

	pRectCtrl->AddBindWindow(this);

	return TRUE;
}

int CPictureCtrl::BindStyle(LPCTSTR lpszStyleID)
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	if (!IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
		return -1;
	}

	if (lpszStyleID == NULL)
	{
		return -2;
	}

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_WL_PICTURE, lpszStyleID);
	return BindStyle(pCtrlProp);
}

int CPictureCtrl::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	if (!IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
		return -1;
	}

	if (pCtrlProp == NULL)
	{
		return -2;
	}
	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (ect != CT_STYLE_WL_PICTURE && ect != CT_WL_PICTURE && ect != CT_PICTURE)
	{
		return -3;
	}

	PICWLPROPERTIES *pWLPicProp = (PICWLPROPERTIES *)pCtrlProp;

	//
	// Set Background image
	//
	SetBkImage(pWLPicProp->m_strBKN);
	SetBkImageResizeMode(pCtrlProp->m_eBkImageResizeMode);
	SetBkImageResizePoint(pCtrlProp->m_ptImageResize);

	// Set transparent
	LONG lTransparent = pWLPicProp->m_lBkImgTransparent;
	SetBkImageTransparent((BYTE)lTransparent);

	//
	// Set foreground image
	//

	SetFgImage(pWLPicProp->m_strFg);

	m_btFgImgTransparent = pWLPicProp->m_btFgImgTransparent;
	pMember->m_eForegroundAlignHor = pWLPicProp->m_eForegroundHorAlignMode;
	pMember->m_eForegroundAlignVer = pWLPicProp->m_eForegroundVerAlignMode;

	pMember->m_rcFgMargin = (RECT)pWLPicProp->m_rcForegroundMargin;

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// virtual

int CPictureCtrl::OnDraw(CDC *pdcMem)
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	if (pdcMem == NULL)
	{
		return -1;
	}

	// Draw parent part.
	BOOL bDrawParentPart = TRUE;
	if (pMember->m_hBackground != NULL && !pMember->m_hBackground->IsHasAlphaChannel())
	{
		bDrawParentPart = FALSE;
	}

	// If no background, draw default.
	if (pMember->m_hBackground == NULL)
	{
		bDrawParentPart = TRUE;
	}

	if (bDrawParentPart)
	{
		CRect rcWin;
		GetWindowRect(rcWin);
		GetParent()->ScreenToClient(rcWin);

		BOOL bUpdateLayeredWindow = FALSE;
		DrawParentPart(this, pdcMem, rcWin, &bUpdateLayeredWindow, NULL);
	}

	// Give parent a chance to draw after draw parent part.
	BOOL bParentDraw = (BOOL)GetParent()->SendMessage(WM_TRANSPARENTDRAW, WPARAM(pdcMem), LPARAM(this));
	if (bParentDraw)
	{
		return 0;
	}

	CRect rcClient;
	GetClientRect(rcClient);

	// Draw background.
	if (pMember->m_hBackground != NULL)
	{
		if (pMember->m_bAnimationMode)
		{
			if (pMember->m_nCurTimerTick >= (int)pMember->m_vAnimationData.size() - 1)
			{
				KillTimer(IDT_ANIMATE_IMAGE);
				pMember->m_bAnimationMode = false;
			}

			DrawImage(pdcMem, pMember->m_vAnimationData[pMember->m_nCurTimerTick].first, pMember->m_vAnimationData[pMember->m_nCurTimerTick].second);

			if (pMember->m_nCurTimerTick >= (int)pMember->m_vAnimationData.size() - 1)
			{
				pMember->m_bAnimationMode = false;
				pMember->m_nCurTimerTick = 0;
				pMember->m_vAnimationData.clear();

				NMHDR nmhdr;
				nmhdr.hwndFrom = m_hWnd;
				nmhdr.idFrom = GetDlgCtrlID();
				nmhdr.code = PVN_ANIMATION_END;
				GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmhdr));
			}
		}
		else
		{
			DrawImage(pdcMem, 100, m_btBkImgTransparent);
		}
	}
	// Draw default
	else
	{
		DrawDefaultPictureCtrl(pdcMem->GetSafeHdc(), rcClient);
	}

	// Draw foreground
	if (pMember->m_hForeground != NULL)
	{
		IUIDrawForeground(pdcMem->GetSafeHdc(), rcClient, pMember->m_rcFgMargin,
			pMember->m_hForeground,
			pMember->m_eForegroundAlignHor, pMember->m_eForegroundAlignVer,
			RGB(255, 0, 255), m_btFgImgTransparent);
	}

	return 0;
}

int CPictureCtrl::DrawImage(CDC *pdcMem, int nStretchRate/*100%*/, BYTE btTransparent/* = 255*/)
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	CRect rcClient;
	GetClientRect(rcClient);

	IUIDrawImage(pdcMem->GetSafeHdc(), rcClient, pMember->m_hBackground,
		m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

int CPictureCtrl::SetBkImage(LPCTSTR lpszImageName, BOOL bRedraw/* = TRUE*/)
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	return IUISetControlImage(&pMember->m_hBackground, lpszImageName);
}

int CPictureCtrl::GetBkImage(CString *pstrImageName)
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	return IUIGetControlImage(pMember->m_hBackground, pstrImageName);
}

int CPictureCtrl::SetFgImage(LPCTSTR lpszImageName, BOOL bRedraw/* = TRUE*/)
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	return IUISetControlImage(&pMember->m_hForeground, lpszImageName);
}

int CPictureCtrl::GetFgImage(CString *pstrImageName)
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	return IUIGetControlImage(pMember->m_hForeground, pstrImageName);
}

int CPictureCtrl::SetFgImageAlignModeHor(FOREGROUND_ALIGN_HOR eHor, BOOL bRedraw/* = TRUE*/)
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	pMember->m_eForegroundAlignHor = eHor;

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CPictureCtrl::SetFgImageAlignModeVer(FOREGROUND_ALIGN_VER eVer, BOOL bRedraw/* = TRUE*/)
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	pMember->m_eForegroundAlignVer = eVer;

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CPictureCtrl::SetFgImageTransparent(BYTE btTransparent, BOOL bRedraw/* = TRUE*/)
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	return (int)SendMessage(WM_SETFGIMGTRANSPARENT, btTransparent, bRedraw);
}

BYTE CPictureCtrl::GetFgImageTransparent() const
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	return m_btFgImgTransparent;
}

int CPictureCtrl::SetFgMargin(LPCRECT lprcMargin)
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	if (lprcMargin == NULL)
	{
		return -1;
	}

	pMember->m_rcFgMargin = *lprcMargin;

	return 0;
}

int CPictureCtrl::GetFgMargin(LPRECT lprcMargin)
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	if (lprcMargin == NULL)
	{
		return -1;
	}

	*lprcMargin = pMember->m_rcFgMargin;

	return 0;
}

int CPictureCtrl::AnimateImage(std::vector<std::pair<int, BYTE> > &vAnimationData, int nElapse/*ms*/)
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	pMember->m_bAnimationMode = true;
	pMember->m_nCurTimerTick = 0;
	pMember->m_nTimerElapse = nElapse;
	pMember->m_vAnimationData = vAnimationData;

	SetTimer(IDT_ANIMATE_IMAGE, pMember->m_nTimerElapse, NULL);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CPictureCtrl message handlers

LRESULT LibUIDK::CPictureCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	PICCTRLMEMBER *pMember = (PICCTRLMEMBER *)m_pMember;

	// When use layered window, the window not redraw when its child controls hide or display
	// to solve it, handle the WM_WINDOWPOSCHANGED message.
	CWnd *pParent = GetParent();
	BOOL bLayered = IsLayeredWindow(pParent->GetSafeHwnd());
	if (message == WM_WINDOWPOSCHANGED && bLayered)
	{
		WINDOWPOS *pos = (WINDOWPOS *)lParam;
		if (pos->hwnd == m_hWnd)
		{
			// When show the control, notify itself to redraw
			if ((pos->flags & SWP_SHOWWINDOW) == SWP_SHOWWINDOW)
			{
				Redraw(this);
			}
			// When hide the control, notify its parent to redraw
			else if ((pos->flags & SWP_HIDEWINDOW) == SWP_HIDEWINDOW)
			{
				CRect rcThis;
				GetWindowRect(rcThis);
				pParent->ScreenToClient(rcThis);
				pParent->InvalidateRect(rcThis);
			}
		}
	}

	if (message == WM_PAINT)
	{
		CPaintDC dc(this);

		CRect rcClient;
		GetClientRect(rcClient);

		DRAWITEMSTRUCT dis;
		dis.CtlType = ODT_STATIC;
		dis.CtlID = GetDlgCtrlID();
		dis.itemID = -1;
		dis.hwndItem = m_hWnd;
		dis.hDC = dc.m_hDC;
		dis.rcItem = rcClient;
		dis.itemData = IUIGetWindowLong(m_hWnd, GWLP_USERDATA);

		BOOL bParentDraw = (BOOL)GetParent()->SendMessage(WM_DRAWITEMEX, dis.CtlID, LPARAM(&dis));
		if (bParentDraw)
		{
			return CWnd::WindowProc(message, wParam, lParam);
		}

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
	else if (message == WM_ERASEBKGND)
	{
		return TRUE;
	}
	else if (message == WM_SETBKIMAGEN)
	{
		HBITMAP hBmp = (HBITMAP)wParam;
		BOOL bRedraw = (BOOL)lParam;

		ReleaseIUIImage(pMember->m_hBackground);

		GUID guid;
		CoCreateGuid(&guid);
		wchar_t wszName[64] = {0};
		StringFromGUID2(guid, wszName, sizeof(wszName));

		AddCustomImage((LPCTSTR)_bstr_t(wszName), hBmp);
		pMember->m_hBackground = CreateIUIImage((LPCTSTR)_bstr_t(wszName));
		ReleaseIUIImage(pMember->m_hBackground);

		if (bRedraw)
		{
			Invalidate();
		}
	}
	else if (message == WM_SETBKIMGTRANSPARENT)
	{
		OnSetBkImgTransparent((BYTE)wParam, (BOOL)lParam);
	}
	else if (message == WM_SETFGIMGTRANSPARENT)
	{
		m_btFgImgTransparent = (BYTE)wParam;

		BOOL bRedraw = (BOOL)lParam;
		if (bRedraw)
		{
			Invalidate();
		}
	}
	else if (message == WM_TIMER)
	{
		if (wParam == IDT_ANIMATE_IMAGE)
		{
			Invalidate();
			pMember->m_nCurTimerTick++;
		}
	}

	// send WM_PARENTEVENT to parent
	switch (message)
	{
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
	case WM_MOUSEWHEEL:
	case WM_MOUSELEAVE:
#endif /* if (_WIN32_WINNT < 0x0400) */
	case WM_MOUSEHOVER:
	{
		int nID = GetDlgCtrlID();
		WPARAM wParam2 = MAKEWPARAM(message, nID);
		BOOL bRet = (BOOL)pParent->SendMessage(WM_PARENTEVENT, wParam2, lParam);
		if (bRet)
		{
			return 0;
		}
	}
	break;
	}

	return CWnd::WindowProc(message, wParam, lParam);
}
