// SearchLightProgressCtrl.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDT_ANIMATE_THUMB			1


struct SLPROGRESSMEMBER
{
	SLPROGRESSMEMBER()
	{
		m_crBk = RGB(255, 255, 255);

		m_bDrawThumb = TRUE;
		m_crThumbStart = -1;	// -1: solid, otherwise: Gradient
		m_crThumbEnd = RGB(255, 0, 0);

		m_bDrawThumbMask = FALSE;
		m_crThumbMaskStart = RGB(255, 255, 255);
		m_crThumbMaskEnd = RGB(255, 255, 255);
		m_nThumbMaskHeight = 60;
		m_nThumbMaskTransparent = 100;

		m_hMaskImage = NULL;
		m_ptImageResize.x = m_ptImageResize.y = 2;

		m_bAnimationMode = false;
		m_nCurTimerTick = 0;

		m_nLower = 0;
		m_nUpper = 100;
		m_nPos = 0;
	}

	COLORREF m_crBk;

	BOOL m_bDrawThumb;
	COLORREF m_crThumbStart;
	COLORREF m_crThumbEnd;

	BOOL m_bDrawThumbMask;
	COLORREF m_crThumbMaskStart;
	COLORREF m_crThumbMaskEnd;
	int m_nThumbMaskHeight;		// -1: all height of thumb
	int m_nThumbMaskTransparent;

	HIUIIMAGE m_hMaskImage;
	CPoint m_ptImageResize;

	// The following variable for animating thumb.
	bool m_bAnimationMode;
	int m_nCurTimerTick;					// Times that timer implement. same as current Frame.
	std::vector<SLPROGRESS_ANIMATION> m_vAnimationData;

	int m_nLower;
	int m_nUpper;
	int m_nPos;
};

/////////////////////////////////////////////////////////////////////////////
// CSearchLightProgressCtrl

CSearchLightProgressCtrl::CSearchLightProgressCtrl()
{
	m_pMember = new SLPROGRESSMEMBER;
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;
}

CSearchLightProgressCtrl::~CSearchLightProgressCtrl()
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	if (m_pMember != NULL)
	{
		delete (SLPROGRESSMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}


BEGIN_MESSAGE_MAP(CSearchLightProgressCtrl, CWnd)
	//{{AFX_MSG_MAP(CSearchLightProgressCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// public

BOOL CSearchLightProgressCtrl::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	CString strMyClass;
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;
	strMyClass = WC_SEARCHLIGHTPROGRESS;
	wndcls.lpszClassName = strMyClass.GetBuffer(strMyClass.GetLength());

	// Register the new class and exit if it fails
	if (!AfxRegisterClass(&wndcls))
	{
		TRACE(_T("[LibUIDK]===Class \"%s\" Registration Failed\n"), WC_SEARCHLIGHTPROGRESS);
		return FALSE;
	}

	CWnd *pWnd = this;
	return pWnd->Create(WC_SEARCHLIGHTPROGRESS, NULL, dwStyle, rect, pParentWnd, nID);
}

BOOL CSearchLightProgressCtrl::Create(DWORD dwStyle, CRectCtrl *pRectCtrl, CWnd *pParentWnd, UINT nID)
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	ASSERT(pRectCtrl != NULL && pRectCtrl->IsCreated());

	CRect rect;
	pRectCtrl->GetWindowRect(rect);
	pParentWnd->ScreenToClient(rect);

	BOOL bRet = CSearchLightProgressCtrl::Create(dwStyle, rect, pParentWnd, nID);
	if (!bRet)
	{
		return FALSE;
	}

	pRectCtrl->AddBindWindow(this);

	return TRUE;
}

int CSearchLightProgressCtrl::BindStyle(LPCTSTR lpszStyleID)
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

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

int CSearchLightProgressCtrl::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

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

	// Set Background image
	SetBkImage(pWLPicProp->m_strBKN);
	SetBkImageResizeMode(pCtrlProp->m_eBkImageResizeMode);
	SetBkImageResizePoint(pCtrlProp->m_ptImageResize);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// virtual

int CSearchLightProgressCtrl::OnDraw(CDC *pdcMem)
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	if (pdcMem == NULL)
	{
		return -1;
	}

	CRect rcClient;
	GetClientRect(rcClient);

	// Draw space
	CRect rcSpace = rcClient;
	if (pMember->m_bDrawThumb)
	{
		rcSpace.bottom = rcClient.Height() * (pMember->m_nUpper - pMember->m_nPos) / (pMember->m_nUpper - pMember->m_nLower);
	}
	pdcMem->FillSolidRect(rcSpace, pMember->m_crBk);

	// Draw Thumb
	CRect rcThumb = rcClient;
	rcThumb.top = rcClient.Height() * (pMember->m_nUpper - pMember->m_nPos) / (pMember->m_nUpper - pMember->m_nLower);

	if (pMember->m_bDrawThumb)
	{
		DrawThumb(pdcMem, rcThumb);
	}

	// Draw thumb mask
	if (pMember->m_bDrawThumbMask)
	{
		DrawThumbMask(pdcMem, rcThumb);
	}

	// Draw mask image.
	if (pMember->m_hMaskImage != NULL)
	{
		DrawMaskImage(pdcMem);
	}

	return 0;
}

int CSearchLightProgressCtrl::DrawThumb(CDC *pdcMem, const CRect &rcThumb)
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	ASSERT(pMember->m_bDrawThumb);

	if (pMember->m_crThumbStart == -1)
	{
		pdcMem->FillSolidRect(rcThumb, pMember->m_crThumbEnd);
	}
	else
	{
		Color crStart;
		crStart.SetFromCOLORREF(pMember->m_crThumbStart);
		Color crEnd;
		crEnd.SetFromCOLORREF(pMember->m_crThumbEnd);
		LinearGradientBrush linGrBrush(Point(0, rcThumb.bottom), Point(0, rcThumb.top - 1), crStart, crEnd);

		Graphics g(pdcMem->GetSafeHdc());
		g.FillRectangle(&linGrBrush, rcThumb.left, rcThumb.top, rcThumb.Width(), rcThumb.Height());
	}

	return 0;
}

int CSearchLightProgressCtrl::DrawThumbMask(CDC *pdcMem, const CRect &rcThumb)
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	ASSERT(pMember->m_bDrawThumbMask);

	Color crStart(pMember->m_nThumbMaskTransparent, GetRValue(pMember->m_crThumbMaskStart), GetGValue(pMember->m_crThumbMaskStart), GetBValue(pMember->m_crThumbMaskStart));
	Color crEnd(0, GetRValue(pMember->m_crThumbMaskEnd), GetGValue(pMember->m_crThumbMaskEnd), GetBValue(pMember->m_crThumbMaskEnd));

	Graphics g(pdcMem->GetSafeHdc());

	if (pMember->m_nThumbMaskHeight == -1)
	{
		LinearGradientBrush linGrBrush(Point(0, rcThumb.bottom), Point(0, rcThumb.top - 1), crStart, crEnd);
		g.FillRectangle(&linGrBrush, rcThumb.left, rcThumb.top, rcThumb.Width(), rcThumb.Height());
	}
	else
	{
		LinearGradientBrush linGrBrush(Point(0, rcThumb.top - 2 - pMember->m_nThumbMaskHeight), Point(0, rcThumb.top - 1), crStart, crEnd);
		g.FillRectangle(&linGrBrush, rcThumb.left, rcThumb.top, rcThumb.Width(), pMember->m_nThumbMaskHeight);
	}

	return 0;
}

int CSearchLightProgressCtrl::DrawMaskImage(CDC *pdcMem)
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	CRect rcClient;
	GetClientRect(rcClient);

	POINT pt = {0, 0};
	IUIDrawImage(pdcMem->GetSafeHdc(), rcClient, pMember->m_hMaskImage, IRM_STRETCH, pt, 1, 1);

	ASSERT(FALSE);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

int CSearchLightProgressCtrl::SetBkColor(COLORREF crBk)
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	pMember->m_crBk = crBk;

	return 0;
}

COLORREF CSearchLightProgressCtrl::GetBkColor() const
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	return pMember->m_crBk;

	return 0;
}

int CSearchLightProgressCtrl::SetDrawThumb(BOOL bDraw)
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	pMember->m_bDrawThumb = bDraw;

	return 0;
}

BOOL CSearchLightProgressCtrl::IsDrawThumb() const
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	return pMember->m_bDrawThumb;
}

int CSearchLightProgressCtrl::SetDrawThumbMask(BOOL bDraw)
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	pMember->m_bDrawThumbMask = bDraw;

	return 0;
}

BOOL CSearchLightProgressCtrl::IsDrawThumbMask() const
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	return pMember->m_bDrawThumbMask;
}

int CSearchLightProgressCtrl::SetBkImage(LPCTSTR lpszImageName, BOOL bRedraw/* = TRUE*/)
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	IUISetControlImage(&pMember->m_hMaskImage, lpszImageName);

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CSearchLightProgressCtrl::GetBkImage(CString *pstrImageName)
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	return IUIGetControlImage(pMember->m_hMaskImage, pstrImageName);
}

int CSearchLightProgressCtrl::AnimateThumb(std::vector<SLPROGRESS_ANIMATION> &vAnimationData, int nElapse/*ms*/)
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	pMember->m_bAnimationMode = true;
	pMember->m_nCurTimerTick = 0;
	pMember->m_vAnimationData = vAnimationData;

	SetTimer(IDT_ANIMATE_THUMB, nElapse, NULL);

	return 0;
}

void CSearchLightProgressCtrl::SetRange(int nLower, int nUpper)
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	pMember->m_nLower = nLower;
	pMember->m_nUpper = nUpper;
}

void CSearchLightProgressCtrl::GetRange(__out int &nLower, __out int &nUpper) const
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	nLower = pMember->m_nLower;
	nUpper = pMember->m_nUpper;
}

int CSearchLightProgressCtrl::GetPos() const
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	return pMember->m_nPos;
}

int CSearchLightProgressCtrl::SetPos(int nPos)
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

	int nOldPos = pMember->m_nPos;
	pMember->m_nPos = nPos;

	Invalidate();

	return pMember->m_nPos;
}

/////////////////////////////////////////////////////////////////////////////
// CSearchLightProgressCtrl message handlers

LRESULT LibUIDK::CSearchLightProgressCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	SLPROGRESSMEMBER *pMember = (SLPROGRESSMEMBER *)m_pMember;

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
	else if (message == WM_TIMER)
	{
		if (wParam == IDT_ANIMATE_THUMB)
		{
			if (pMember->m_nCurTimerTick >= (int)pMember->m_vAnimationData.size())
			{
				KillTimer(IDT_ANIMATE_THUMB);
				pMember->m_nCurTimerTick = 0;
				pMember->m_bAnimationMode = 0;
				pMember->m_vAnimationData.clear();

				return CWnd::WindowProc(message, wParam, lParam);
			}

			pMember->m_nPos = pMember->m_vAnimationData[pMember->m_nCurTimerTick].nPos;
			pMember->m_crBk = pMember->m_vAnimationData[pMember->m_nCurTimerTick].crBk;
			pMember->m_bDrawThumb = pMember->m_vAnimationData[pMember->m_nCurTimerTick].bDrawThumb;
			pMember->m_crThumbStart = pMember->m_vAnimationData[pMember->m_nCurTimerTick].crThumbStart;
			pMember->m_crThumbEnd = pMember->m_vAnimationData[pMember->m_nCurTimerTick].crThumbEnd;
			pMember->m_bDrawThumbMask = pMember->m_vAnimationData[pMember->m_nCurTimerTick].bDrawThumbMask;
			pMember->m_crThumbMaskStart = pMember->m_vAnimationData[pMember->m_nCurTimerTick].crThumbMaskStart;
			pMember->m_crThumbMaskEnd = pMember->m_vAnimationData[pMember->m_nCurTimerTick].crThumbMaskEnd;
			pMember->m_nThumbMaskTransparent = pMember->m_vAnimationData[pMember->m_nCurTimerTick].nThumbMaskTransparent;

			Invalidate();

			pMember->m_nCurTimerTick++;
		}
	}

	return CWnd::WindowProc(message, wParam, lParam);
}
