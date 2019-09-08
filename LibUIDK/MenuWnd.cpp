// MenuWnd.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CMenuWnd

CMenuWnd::CMenuWnd()
{
	m_pNotifyWnd = NULL;
}

CMenuWnd::~CMenuWnd()
{
}


BEGIN_MESSAGE_MAP(CMenuWnd, CUIWnd)
	//{{AFX_MSG_MAP(CMenuWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMenuWnd message handlers

BOOL CMenuWnd::Create(CWnd *pParentWnd)
{
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hIcon = NULL;
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = _T("IUIMenuWnd");

	// Register the new class and exit if it fails
	if (!AfxRegisterClass(&wndcls))
	{
		TRACE(_T("[LibUIDK]===Class Registration Failed\n"));
		return FALSE;
	}
	DWORD dwStyle = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	BOOL bRet = CreateEx(WS_EX_TOOLWINDOW, wndcls.lpszClassName, NULL, dwStyle, CRect(0, 0, 0, 0), pParentWnd, 0);
	if (!bRet)
	{
		return FALSE;
	}

	m_pNotifyWnd = pParentWnd;

	return TRUE;
}

// CPont(x, y): the arrow position from screen
int CMenuWnd::Popup(int x, int y)
{
	// adjust the window position
	::SetWindowPos(m_hWnd, NULL, x, y, 0, 0, SWP_NOSIZE);

	// show info window
	if (!IsWindowVisible())
	{
		ShowWindow(SW_SHOW);
	}

	SetFocus();

	return 0;
}

int CMenuWnd::Hide()
{
	ShowWindow(SW_HIDE);

	return 0;
}

int CMenuWnd::SetNotifyWnd(CWnd *pWnd)
{
	m_pNotifyWnd = pWnd;

	return 0;
}

CWnd *CMenuWnd::GetNotifyWnd()
{
	return m_pNotifyWnd;
}

//////////////////////////////////////////////////////////////////////////
// message

LRESULT CMenuWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	if (message == WM_NCACTIVATE && wParam == FALSE)
	{
		LRESULT lr = CUIWnd::WindowProc(message, wParam, lParam);

		if (::IsWindow(m_pNotifyWnd->GetSafeHwnd()))
		{
			m_pNotifyWnd->SendMessage(WS_MENUWND_HIDING, 0, (LPARAM)m_hWnd);
		}

		// Hide the menu window.
		Hide();

		if (::IsWindow(m_pNotifyWnd->GetSafeHwnd()))
		{
			m_pNotifyWnd->SendMessage(WS_MENUWND_HIDDEN, 0, (LPARAM)m_hWnd);
		}

		return lr;
	}
	else if (message == WM_ACTIVATE)
	{
		BOOL bActivate = LOWORD(wParam);
		HWND hWnd = HWND(lParam);
		if (!bActivate)
		{
			LRESULT lr = CUIWnd::WindowProc(message, wParam, lParam);

			if (::IsWindow(m_pNotifyWnd->GetSafeHwnd()))
			{
				m_pNotifyWnd->SendMessage(WS_MENUWND_HIDING, 0, (LPARAM)m_hWnd);
			}

			Hide();

			if (::IsWindow(m_pNotifyWnd->GetSafeHwnd()))
			{
				m_pNotifyWnd->SendMessage(WS_MENUWND_HIDDEN, 0, (LPARAM)m_hWnd);
			}

			return lr;
		}
	}
	else if (message == WM_KILLFOCUS)
	{
		HWND hWndNewFocus = HWND(wParam);
		BOOL bParent = IsParent(hWndNewFocus, m_hWnd);
		if (!bParent)
		{
			LRESULT lr = CUIWnd::WindowProc(message, wParam, lParam);

			if (::IsWindow(m_pNotifyWnd->GetSafeHwnd()))
			{
				m_pNotifyWnd->SendMessage(WS_MENUWND_HIDING, 0, (LPARAM)m_hWnd);
			}

			Hide();

			if (::IsWindow(m_pNotifyWnd->GetSafeHwnd()))
			{
				m_pNotifyWnd->SendMessage(WS_MENUWND_HIDDEN, 0, (LPARAM)m_hWnd);
			}

			return lr;
		}
	}
	else if (message == WM_DESTROY)
	{
		if (::IsWindow(m_pNotifyWnd->GetSafeHwnd()))
		{
			m_pNotifyWnd->SendMessage(WS_MENUWND_DESTROY, 0, (LPARAM)m_hWnd);
		}
	}

	return CUIWnd::WindowProc(message, wParam, lParam);
}
