// SplitterBar.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define WM_SET_OFFSET		WM_SPLITTER


class CTrackingSplitterBar : public CWnd
{
public:
	CTrackingSplitterBar();
	virtual ~CTrackingSplitterBar();

	virtual BOOL LoadFrame(const RECT &rect, SPLITTER_ORIENTATION eOrientation, CSplitterBar *pNotify, CWnd *pPanel1, CWnd *pPanel2);
	int ReplacePanel12(CWnd *pNewPanel1, CWnd *pNewPanel2);

public:
	int SetBkImages(LPCTSTR lpszImageNameN, BOOL bRedraw = TRUE);
	int GetBkImages(CString *pstrImageNameN);

	virtual int StartTracking();
	virtual int StopTracking(BOOL bAccept);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrackingSplitterBar)
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	DECLARE_MESSAGE_MAP()

	HIUIIMAGE m_himgBk;
	POINT m_ptResize;
	CRect m_rcOld;
	SPLITTER_ORIENTATION m_eOrientation;
	class CSplitterBar *m_pWndNotify;
	CWnd *m_pPanel1;
	CWnd *m_pPanel2;
};


BEGIN_MESSAGE_MAP(CTrackingSplitterBar, CWnd)
END_MESSAGE_MAP()

CTrackingSplitterBar::CTrackingSplitterBar()
{
	m_eOrientation = SO_WE;
	m_pWndNotify = NULL;
	m_pPanel1 = NULL;
	m_pPanel2 = NULL;
	m_himgBk = NULL;
	m_ptResize.x = m_ptResize.y = 2;
}

CTrackingSplitterBar::~CTrackingSplitterBar()
{
	if (m_himgBk != NULL)
	{
		ReleaseIUIImage(m_himgBk);
		m_himgBk = NULL;
	}
}

BOOL CTrackingSplitterBar::LoadFrame(const RECT &rect, SPLITTER_ORIENTATION eOrientation, CSplitterBar *pNotify, CWnd *pPanel1, CWnd *pPanel2)
{
	// attempt to create the window
	CString strMyClass;
	strMyClass = _T("IUITrackSplitter");
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hIcon = NULL;
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = (LPCTSTR) strMyClass;

	// Register the new class and exit if it fails
	if (!AfxRegisterClass(&wndcls))
	{
		TRACE(_T("[LibUIDK]===Class Registration Failed\n"));
		return FALSE;
	}

	m_rcOld = rect;
	m_eOrientation = eOrientation;
	m_pWndNotify = pNotify;
	m_pPanel1 = pPanel1;
	m_pPanel2 = pPanel2;

	if (!CreateEx(WS_EX_TOOLWINDOW, strMyClass, NULL, WS_VISIBLE | WS_POPUP | WS_CLIPCHILDREN, rect, NULL, 0))
	{
		return FALSE;
	}

	return TRUE;
}

int CTrackingSplitterBar::ReplacePanel12(CWnd *pNewPanel1, CWnd *pNewPanel2)
{
	m_pPanel1 = pNewPanel1;
	m_pPanel2 = pNewPanel2;

	return 0;
}

int CTrackingSplitterBar::SetBkImages(LPCTSTR lpszImageName, BOOL bRedraw/* = TRUE*/)
{
	ASSERT(m_hWnd != NULL);

	IUISetControlImage(&m_himgBk, lpszImageName);

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CTrackingSplitterBar::GetBkImages(CString *pstrImageName)
{
	IUIGetControlImage(m_himgBk, pstrImageName);

	return 0;
}

int CTrackingSplitterBar::StartTracking()
{
	ASSERT(m_pWndNotify != NULL);
	m_pWndNotify->StartTracking();

	return 0;
}

int CTrackingSplitterBar::StopTracking(BOOL bAccept)
{
	ASSERT(m_pWndNotify != NULL);
	m_pWndNotify->StopTracking(bAccept);

	return 0;
}

LRESULT CTrackingSplitterBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CREATE)
	{
		SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) ^ 0x80000);
		HINSTANCE hInst = LoadLibrary(_T("User32.DLL"));
		if (hInst != NULL)
		{
			typedef BOOL (WINAPI * MYFUNC)(HWND, COLORREF, BYTE, DWORD);
			MYFUNC SetLayeredWindowAttributes = NULL;
			SetLayeredWindowAttributes = (MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
			if (SetLayeredWindowAttributes)
			{
				SetLayeredWindowAttributes(this->GetSafeHwnd(), 0, 150, LWA_ALPHA);
			}

			FreeLibrary(hInst);
		}
	}
	else if (message == WM_ERASEBKGND)
	{
		return TRUE;
	}
	else if (message == WM_PAINT)
	{
		CPaintDC dc(this);

		CRect rcClient;
		GetClientRect(rcClient);

		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);

		CBitmap bmpMem;
		bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

		CBitmap *pbmpMemOld = dcMem.SelectObject(&bmpMem);

		IUINineGridBlt(dcMem.GetSafeHdc(), rcClient, m_himgBk, m_ptResize, 1, 1);

		dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);
		dcMem.SelectObject(pbmpMemOld);
	}
	else if (message == WM_CANCELMODE)
	{
		StopTracking(FALSE);
	}
	else if (message == WM_KEYDOWN)
	{
		CPoint pt;
		GetCursorPos(&pt);

		int cz = GetKeyState(VK_CONTROL) < 0 ? 1 : 16;
		int dx = 0;
		int dy = 0;

		int nChar = (int)wParam;
		switch (nChar)
		{
		case VK_ESCAPE:
			StopTracking(FALSE);
			return CWnd::WindowProc(message, wParam, lParam);
		case VK_RETURN:
			StopTracking(TRUE);
			return CWnd::WindowProc(message, wParam, lParam);

		case VK_LEFT:
			dx = -1;
			break;
		case VK_RIGHT:
			dx = +1;
			break;
		case VK_UP:
			dy = -1;
			break;
		case VK_DOWN:
			dy = +1;
			break;

		default:
			Default();  // pass other keys through
			return CWnd::WindowProc(message, wParam, lParam);
		}
	}
	else if (message == WM_WINDOWPOSCHANGING)	// Limit splitter bar's position
	{
		WINDOWPOS *pwp = (WINDOWPOS *)lParam;

		// Creating
		if (pwp->cx == 0 || pwp->cy == 0)
		{
			return CWnd::WindowProc(message, wParam, lParam);
		}

		// Get max limit rect
		CRect rcPanel1;
		m_pPanel1->GetWindowRect(rcPanel1);
		CRect rcPanel2;
		m_pPanel2->GetWindowRect(rcPanel2);
		CRect rcLimit;
		rcLimit.left = rcPanel1.left;
		rcLimit.top = rcPanel1.top;
		rcLimit.right = rcPanel2.right;
		rcLimit.bottom = rcPanel2.bottom;

		// Get the minimize size of the panel.
		CRect rect;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

		CSize size1(0, 0);
		MINMAXINFO MMI1;
		MMI1.ptMinTrackSize.x = size1.cx;
		MMI1.ptMinTrackSize.y = size1.cy;
		MMI1.ptMaxSize.x = rect.Width();
		MMI1.ptMaxSize.y = rect.Height();
		m_pPanel1->SendMessage(WM_GETMINMAXINFO, 0, (LPARAM)(&MMI1));

		CSize size2(0, 0);
		MINMAXINFO MMI2;
		MMI2.ptMinTrackSize.x = size2.cx;
		MMI2.ptMinTrackSize.y = size2.cy;
		MMI2.ptMaxSize.x = rect.Width();
		MMI2.ptMaxSize.y = rect.Height();
		m_pPanel1->SendMessage(WM_GETMINMAXINFO, 0, (LPARAM)(&MMI2));

		// Limit
		if (m_eOrientation == SO_WE)
		{
			pwp->y = m_rcOld.top;

			if (pwp->x < rcLimit.left + MMI1.ptMinTrackSize.x)
			{
				pwp->x = rcLimit.left + MMI1.ptMinTrackSize.x;
			}
			if (pwp->x + pwp->cx > rcLimit.right - MMI2.ptMinTrackSize.x)
			{
				pwp->x = rcLimit.right - MMI2.ptMinTrackSize.x - pwp->cx;
			}

			m_pWndNotify->SendMessage(WM_SET_OFFSET, 0, pwp->x - m_rcOld.left);
		}
		else if (m_eOrientation == SO_NS)
		{
			pwp->x = m_rcOld.left;

			if (pwp->y < rcLimit.top + MMI1.ptMinTrackSize.y)
			{
				pwp->y = rcLimit.top + MMI1.ptMinTrackSize.y;
			}
			if (pwp->y + pwp->cy > rcLimit.bottom - MMI2.ptMinTrackSize.y)
			{
				pwp->y = rcLimit.bottom - MMI2.ptMinTrackSize.y - pwp->cy;
			}

			m_pWndNotify->SendMessage(WM_SET_OFFSET, 0, pwp->y - m_rcOld.top);
		}
	}
	else if (message == WM_EXITSIZEMOVE)
	{
		// If press esc key while tracking the bar, in WM_WINDOWPOSCHANGING message, the WINDOWPOS::x and WINDOWPOS::y will be restored.
		// and CSplitterBar::m_ptTrackOffset will be 0. so, at this step, can use TRUE to call StopTracking.
		StopTracking(TRUE);
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////

struct SPLITTERMEMBER
{
	SPLITTERMEMBER()
	{
		m_pPanel1 = NULL;
		m_pPanel2 = NULL;
		for (int i = 0; i < 4; ++i)
		{
			m_himgBk[i] = NULL;
		}

		m_hHCursor = NULL;
		m_hVCursor = NULL;
		m_eOrientation = SO_WE;
		m_bIsMouseOver = FALSE;
		m_bTracking = FALSE;
		memset(&m_rectTracker, 0, sizeof(m_rectTracker));
		memset(&m_rectLimit, 0, sizeof(m_rectLimit));
		memset(&m_ptTrackOffset, 0, sizeof(m_ptTrackOffset));
		memset(&m_ptLButtonDown, 0, sizeof(m_ptLButtonDown));
		m_dwHoverTime = HOVER_DEFAULT;
		m_bHoverRepeat = FALSE;
	}

	int Release()
	{
		m_pPanel1 = NULL;
		m_pPanel2 = NULL;
		for (int i = 0; i < 4; ++i)
		{
			ReleaseIUIImage(m_himgBk[i]);
			m_himgBk[i] = NULL;
		}

		m_hHCursor = NULL;
		m_hVCursor = NULL;
		m_eOrientation = SO_WE;
		m_bIsMouseOver = FALSE;
		m_bTracking = FALSE;
		memset(&m_rectTracker, 0, sizeof(m_rectTracker));
		memset(&m_rectLimit, 0, sizeof(m_rectLimit));
		memset(&m_ptTrackOffset, 0, sizeof(m_ptTrackOffset));
		memset(&m_ptLButtonDown, 0, sizeof(m_ptLButtonDown));
		m_dwHoverTime = HOVER_DEFAULT;
		m_bHoverRepeat = FALSE;

		return 0;
	}

	CWnd *m_pPanel1;	// Left or top panel
	CWnd *m_pPanel2;	// Right or bottom panel

	HIUIIMAGE m_himgBk[4];

	HCURSOR m_hHCursor;
	HCURSOR m_hVCursor;

	SPLITTER_ORIENTATION m_eOrientation;

	BOOL m_bIsMouseOver;

	// Tracking info - only valid when 'm_bTracking' is set
	BOOL m_bTracking;
	CRect m_rectTracker;
	CRect m_rectLimit;
	CPoint m_ptTrackOffset;
	CPoint m_ptLButtonDown;
	CTrackingSplitterBar m_wndTracking;

	// for mouse hover
	DWORD m_dwHoverTime;
	BOOL m_bHoverRepeat;
};

// CSplitterBar

CSplitterBar::CSplitterBar()
{
	m_pMember = new SPLITTERMEMBER;
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;
}

CSplitterBar::~CSplitterBar()
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	ReleaseObject();

	if (m_pMember != NULL)
	{
		delete (SPLITTERMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

int CSplitterBar::ReleaseObject()
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	pMember->Release();

	return CControlBase::ReleaseObject();
}

BEGIN_MESSAGE_MAP(CSplitterBar, CWnd)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////

BOOL CSplitterBar::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	CString strMyClass;
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;
	strMyClass = WC_SPLITTERBAR;
	wndcls.lpszClassName = strMyClass.GetBuffer(strMyClass.GetLength());

	// Register the new class and exit if it fails
	if (!AfxRegisterClass(&wndcls))
	{
		TRACE(_T("[LibUIDK]===Class \"%s\" Registration Failed\n"), WC_SPLITTERBAR);
		return FALSE;
	}

	CWnd *pWnd = this;
	return pWnd->Create(WC_SPLITTERBAR, NULL, dwStyle, rect, pParentWnd, nID);
}

BOOL CSplitterBar::Create(DWORD dwStyle, CRectCtrl *pRectCtrl, CWnd *pParentWnd, UINT nID)
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	ASSERT(pRectCtrl != NULL && pRectCtrl->IsCreated());

	CRect rect;
	pRectCtrl->GetWindowRect(rect);
	pParentWnd->ScreenToClient(rect);

	BOOL bRet = CSplitterBar::Create(dwStyle, rect, pParentWnd, nID);
	if (!bRet)
	{
		return FALSE;
	}

	pRectCtrl->AddBindWindow(this);

	return TRUE;
}

int CSplitterBar::BindStyle(LPCTSTR lpszStyleID)
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	if (!IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
		return -1;
	}

	if (lpszStyleID == NULL)
	{
		return -2;
	}

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_SPLITTER, lpszStyleID);
	return BindStyle(pCtrlProp);
}

int CSplitterBar::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

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
	if (ect != CT_STYLE_SPLITTER && ect != CT_SPLITTER)
	{
		return -3;
	}

	// Set background bitmaps for control
	CString strImageName[1 + COMBINEIMAGESIZE4];
	BOOL bCombineImage = TRUE;
	CTRLPROPERTIES::IUIGetBackground4(pCtrlProp, &bCombineImage, strImageName);
	if (bCombineImage)
	{
		SetBkCombineImage(strImageName[0]);
	}
	else
	{
		SetBkImages(CONTROL_STATE_UNCHECKED_ALL,
			strImageName[1], strImageName[2], strImageName[3], strImageName[4]);
	}
	for (int i = 0; i < 1 + COMBINEIMAGESIZE4; ++i)
	{
		ReleaseIUIImage(strImageName[i]);
	}

	SetOrientation(((SPLITTERPROPERTIES *)pCtrlProp)->m_eOrientation);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// protected

int CSplitterBar::StartTracking()
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	// GetHitRect will restrict 'pMember->m_rectLimit' as appropriate

	// set tracking state and appropriate cursor
	pMember->m_bTracking = TRUE;

	if (pMember->m_himgBk[0] == NULL)
	{
		CPoint pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		pMember->m_ptLButtonDown = pt;

		OnInvertTracker(pMember->m_rectTracker);
	}
	else
	{
		if (pMember->m_wndTracking.GetSafeHwnd() == NULL)
		{
			CRect rcWnd;
			GetWindowRect(rcWnd);

			if (!pMember->m_wndTracking.LoadFrame(rcWnd,
					pMember->m_eOrientation, this, pMember->m_pPanel1, pMember->m_pPanel2))
			{
				return -2;
			}
			pMember->m_wndTracking.SetBkImages(pMember->m_himgBk[0]->GetSafeImageName());

			pMember->m_wndTracking.SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(0, 0));
		}
		else
		{
			ASSERT(FALSE);
			pMember->m_wndTracking.ShowWindow(SW_SHOW);
		}
	}

	return 0;
}

int CSplitterBar::StopTracking(BOOL bAccept)
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	ASSERT_VALID(this);

	if (!pMember->m_bTracking)
	{
		return 1;
	}

	if (pMember->m_wndTracking.GetSafeHwnd() != NULL)
	{
		pMember->m_wndTracking.DestroyWindow();
	}

	// erase tracker rectangle
	if (pMember->m_himgBk[0] == NULL)
	{
		OnInvertTracker(pMember->m_rectTracker);
	}

	pMember->m_bTracking = FALSE;

	if (bAccept)
	{
		if (pMember->m_eOrientation == SO_WE)
		{
			if (pMember->m_pPanel1->GetSafeHwnd() != NULL)
			{
				CRect rcOld1;
				pMember->m_pPanel1->GetWindowRect(rcOld1);
				pMember->m_pPanel1->SetWindowPos(NULL,
					0, 0, rcOld1.Width() + pMember->m_ptTrackOffset.x, rcOld1.Height(), SWP_NOMOVE);
			}

			if (pMember->m_pPanel2->GetSafeHwnd() != NULL)
			{
				CRect rcOld2;
				pMember->m_pPanel2->GetWindowRect(rcOld2);
				GetParent()->ScreenToClient(rcOld2);
				pMember->m_pPanel2->MoveWindow(
					rcOld2.left + pMember->m_ptTrackOffset.x,
					rcOld2.top,
					rcOld2.Width() - pMember->m_ptTrackOffset.x, rcOld2.Height());
			}
		}
		else if (pMember->m_eOrientation == SO_NS)
		{
			if (pMember->m_pPanel1->GetSafeHwnd() != NULL)
			{
				CRect rcOld1;
				pMember->m_pPanel1->GetWindowRect(rcOld1);
				pMember->m_pPanel1->SetWindowPos(NULL,
					0, 0, rcOld1.Width(), rcOld1.Height() + pMember->m_ptTrackOffset.y, SWP_NOMOVE);
			}

			if (pMember->m_pPanel2->GetSafeHwnd() != NULL)
			{
				CRect rcOld2;
				pMember->m_pPanel2->GetWindowRect(rcOld2);
				GetParent()->ScreenToClient(rcOld2);
				pMember->m_pPanel2->MoveWindow(rcOld2.left, rcOld2.top + pMember->m_ptTrackOffset.y, rcOld2.Width(), rcOld2.Height() - pMember->m_ptTrackOffset.y);
			}
		}
		else
		{
			ASSERT(FALSE);
		}

		GetParent()->SendMessage(WM_ENDTRACK_SPLITTER, WPARAM(m_hWnd), 0);
	}

	// May be restore active panel.

	return 0;
}

// Copy from CSplitterWnd::OnInvertTracker
int CSplitterBar::OnInvertTracker(const CRect &rect)
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	ASSERT_VALID(this);
	ASSERT(!rect.IsRectEmpty());
	ASSERT((GetStyle() & WS_CLIPCHILDREN) == 0);

	// pat-blt without clip children on
	CDC *pDC = GetDC();
	// invert the brush pattern (looks just like frame window sizing)
	CBrush *pBrush = CDC::GetHalftoneBrush();
	HBRUSH hOldBrush = NULL;
	if (pBrush != NULL)
	{
		hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, pBrush->m_hObject);
	}
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
	if (hOldBrush != NULL)
	{
		SelectObject(pDC->m_hDC, hOldBrush);
	}
	ReleaseDC(pDC);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

int CSplitterBar::SetBkImages(UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
	LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD, BOOL bRedraw/* = TRUE*/)
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	ASSERT(m_hWnd != NULL);

	m_bBkCombine = false;
	IUISetControlImage4(uMask, pMember->m_himgBk,
		lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CSplitterBar::GetBkImages(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameH, CString *pstrImageNameS, CString *pstrImageNameD)
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bBkCombine;
	IUIGetControlImage4(uMask, m_himgCombineBk, pMember->m_himgBk, m_bBkCombine, pstrCombineImageName,
		pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);

	return 0;
}

int CSplitterBar::SetHCursor(HCURSOR hCursor)
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	pMember->m_hHCursor = hCursor;

	return 0;
}

HCURSOR CSplitterBar::GetHCursor()
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	return pMember->m_hHCursor;
}

int CSplitterBar::SetVCursor(HCURSOR hCursor)
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	pMember->m_hVCursor = hCursor;

	return 0;
}

HCURSOR CSplitterBar::GetVCursor()
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	return pMember->m_hVCursor;
}

int CSplitterBar::SetOrientation(SPLITTER_ORIENTATION eOrientation)
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	pMember->m_eOrientation = eOrientation;

	return 0;
}

SPLITTER_ORIENTATION CSplitterBar::GetOrientation() const
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	return pMember->m_eOrientation;
}

int CSplitterBar::AddPanel(CWnd *pPanel1, CWnd *pPanel2)
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	pMember->m_pPanel1 = pPanel1;
	pMember->m_pPanel2 = pPanel2;

	return 0;
}

CWnd *CSplitterBar::GetPanel1()
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	return pMember->m_pPanel1;
}

CWnd *CSplitterBar::GetPanel2()
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	return pMember->m_pPanel2;
}

int CSplitterBar::ReplacePanel1(CWnd *pNewPanel1)
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	pMember->m_pPanel1 = pNewPanel1;

	return 0;
}

int CSplitterBar::ReplacePanel2(CWnd *pNewPanel2)
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	pMember->m_pPanel2 = pNewPanel2;

	return 0;
}

// CSplitterBar message handlers

int CSplitterBar::OnDraw(CDC *pdcMem)
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

	CRect rcClient;
	GetClientRect(rcClient);

	// Prepare state index
	int nStateIndex = 0;
	BOOL bSelected = GetKeyState(VK_LBUTTON) < 0;
	if (!IsWindowEnabled())
	{
		// 1.disable
		nStateIndex = 3;
	}
	else
	{
		if (bSelected)
		{
			if (pMember->m_bIsMouseOver)
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
			if (pMember->m_bIsMouseOver)
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

	// Draw background
	if (m_bBkCombine)
	{
		CRect rcClient;
		GetClientRect(rcClient);

		BOOL bUpdateLayeredWindow = (BOOL)GetParent()->SendMessage(WM_ISUPDATELAYEREDWINDOW);
		if (bUpdateLayeredWindow)
		{
			GetWindowRect(rcClient);
			GetParent()->ScreenToClient(rcClient);
		}

		DrawCombineBackground(pdcMem, rcClient, COMBINEIMAGESIZE4, nStateIndex);
	}
	else
	{
		IUIDrawImage(pdcMem->GetSafeHdc(), rcClient, pMember->m_himgBk[nStateIndex],
			m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
	}

	return 0;
}

LRESULT CSplitterBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	SPLITTERMEMBER *pMember = (SPLITTERMEMBER *)m_pMember;

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
	else if (message == WM_PAINT)
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
	else if (message == WM_LBUTTONDOWN)
	{
		UpdateWindow();

		// when the button as a menu bar, click the button more then once,
		// can not accept WM_MOUSELEAVE message after the pop-up menu has been destroy
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = HOVER_DEFAULT;
		::TrackMouseEvent(&tme);
		Redraw(this);

		//
		// following code for tracking...
		//
		if (!pMember->m_bTracking)
		{
			StartTracking();
		}
	}
	else if (message == WM_MOUSEMOVE)
	{
		// UI
		if (!pMember->m_bIsMouseOver)
		{
			pMember->m_bIsMouseOver = TRUE;

			// when set RGN for highlight stats, show the tool tip when mouse over the button(the tool tip and highlight stats at the same time work).
			// don't call Invalidate() after this "if" section
			Redraw(this);
			UpdateWindow();
		}

		// when the button as a menu bar, can not accept WM_MOUSELEAVE message after the pop-up menu has been destroy
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = pMember->m_dwHoverTime;
		::TrackMouseEvent(&tme);

		//
		// the following code for tracking
		//
		StopTracking(FALSE);
	}
	else if (message == WM_MOUSEHOVER)
	{
		if (pMember->m_bHoverRepeat)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_HOVER;
			tme.hwndTrack = m_hWnd;
			tme.dwHoverTime = pMember->m_dwHoverTime;
			::TrackMouseEvent(&tme);
		}
	}
	else if (message == WM_MOUSELEAVE)
	{
		pMember->m_bIsMouseOver = FALSE;
		Redraw(this);
	}
	else if (message == WM_KILLFOCUS)
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
			Redraw(this);
			UpdateWindow();
		}

		return lr;
	}
	else if (message == WM_SETCURSOR)
	{
		HWND hWnd = HWND(wParam);
		if (hWnd == m_hWnd && !IsIconic())
		{
			if (pMember->m_eOrientation == SO_WE)
			{
				if (pMember->m_hHCursor != NULL)
				{
					SetCursor(pMember->m_hHCursor);
				}
				else
				{
					// Copy from CSplitterWnd::SetSplitCursor
					HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(AFX_IDC_HSPLITBAR), RT_GROUP_CURSOR);
					HCURSOR hCursor = ::LoadCursor(hInst, MAKEINTRESOURCE(AFX_IDC_HSPLITBAR));
					if (hCursor != NULL)
					{
						SetCursor(hCursor);
					}
					else
					{
						SetCursor(LoadCursor(NULL, IDC_SIZEWE));
					}
				}
			}
			else if (pMember->m_eOrientation == SO_NS)
			{
				if (pMember->m_hVCursor != NULL)
				{
					SetCursor(pMember->m_hVCursor);
				}
				else
				{
					HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(AFX_IDC_VSPLITBAR), RT_GROUP_CURSOR);
					HCURSOR hCursor = ::LoadCursor(hInst, MAKEINTRESOURCE(AFX_IDC_VSPLITBAR));
					if (hCursor != NULL)
					{
						SetCursor(hCursor);
					}
					else
					{
						SetCursor(LoadCursor(NULL, IDC_SIZENS));
					}
				}
			}
			else
			{
				ASSERT(FALSE);
			}

			return TRUE;
		}
	}
	else if (message == WM_SET_OFFSET)
	{
		if (pMember->m_eOrientation == SO_NS)
		{
			pMember->m_ptTrackOffset.y = (LONG)lParam;
		}
		else if (pMember->m_eOrientation == SO_WE)
		{
			pMember->m_ptTrackOffset.x = (LONG)lParam;
		}
	}

	return CWnd::WindowProc(message, wParam, lParam);
}
