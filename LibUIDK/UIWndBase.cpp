// UIWnd.cpp : implementation file
//

#include "stdafx.h"

#include <map>
#include <AFXPRIV.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


struct UIWNDBASEMEMBER
{
	// for resizing the window
	int m_nHitTestRet; // the result of Hit test the resizing window witch can be resizing by dragging its board. default to HTCLIENT;
	BOOL m_bDragResize; // If the window can be resized by dragging the board, default to FALSE
	BOOL m_bUseLayeredWindow;
	BOOL m_bIsUpdateLayeredWindow;
	CSize m_sizeScroll;
	int m_nCXBorder; // see SM_CXBORDER of GetSystemMetrics
	int m_nCYBorder; // see SM_CYBORDER of GetSystemMetrics
	CPoint m_ptLButtonDown;
	CRect m_rcLButtonDown;
	int m_nTitleBarHeight;	// -1: all client as title bar; 0: no title bar.

	// Draw
	HIUIIMAGE m_himgBK;   // the base bitmap background( there is no control on it ), store the image without alpha channel or image with alpha blend bk color and m_himgAlphaBk
	CBitmap m_bmpCompatible;	// the compatible bitmap to CPaintDC in WM_PAINT message.
	BOOL m_bUpdateLayeredWindow;	// Use UpdateLayeredWindow to draw window

	// for Dialog
	BOOL m_bCreateAsDialog;
	HWND m_hWndTop;                 // top level parent window (may be disabled)

	// for menu
	UINT m_uDefaultMenuID;

	HACCEL m_hAccelTable;
};

#define m_nHitTestRet (((UIWNDBASEMEMBER *)m_pMember)->m_nHitTestRet)
#define m_bDragResize (((UIWNDBASEMEMBER *)m_pMember)->m_bDragResize)
#define m_bUseLayeredWindow (((UIWNDBASEMEMBER *)m_pMember)->m_bUseLayeredWindow)
#define m_bIsUpdateLayeredWindow (((UIWNDBASEMEMBER *)m_pMember)->m_bIsUpdateLayeredWindow)
#define m_sizeScroll (((UIWNDBASEMEMBER *)m_pMember)->m_sizeScroll)
#define m_nCXBorder (((UIWNDBASEMEMBER *)m_pMember)->m_nCXBorder)
#define m_nCYBorder (((UIWNDBASEMEMBER *)m_pMember)->m_nCYBorder)
#define m_ptLButtonDown (((UIWNDBASEMEMBER *)m_pMember)->m_ptLButtonDown)
#define m_rcLButtonDown (((UIWNDBASEMEMBER *)m_pMember)->m_rcLButtonDown)
#define m_nTitleBarHeight (((UIWNDBASEMEMBER *)m_pMember)->m_nTitleBarHeight)

#define m_himgBK (((UIWNDBASEMEMBER *)m_pMember)->m_himgBK)
#define m_bmpCompatible (((UIWNDBASEMEMBER *)m_pMember)->m_bmpCompatible)
#define m_bUpdateLayeredWindow (((UIWNDBASEMEMBER *)m_pMember)->m_bUpdateLayeredWindow)

#define m_bCreateAsDialog (((UIWNDBASEMEMBER *)m_pMember)->m_bCreateAsDialog)
#define m_hWndTop (((UIWNDBASEMEMBER *)m_pMember)->m_hWndTop)

#define m_uDefaultMenuID (((UIWNDBASEMEMBER *)m_pMember)->m_uDefaultMenuID)

#define m_hAccelTable (((UIWNDBASEMEMBER *)m_pMember)->m_hAccelTable)

/////////////////////////////////////////////////////////////////////////////
// CUIWndBase

IMPLEMENT_DYNCREATE(CUIWndBase, CWnd)

CUIWndBase::CUIWndBase(CWnd *pParentWnd/* = NULL*/)
{
	m_pParentWnd = pParentWnd;
	m_pMember = new UIWNDBASEMEMBER;

	m_nHitTestRet = HTCLIENT;
	m_bDragResize = FALSE;
	m_bUseLayeredWindow = TRUE;
	m_bIsUpdateLayeredWindow = FALSE;
	m_sizeScroll = CSize(16, 16);
	m_nCXBorder = 4;
	m_nCYBorder = 4;
	m_ptLButtonDown.x = m_ptLButtonDown.y = 0;
	m_rcLButtonDown.SetRectEmpty();
	m_nTitleBarHeight = 24;

	m_bUpdateLayeredWindow = TRUE;

	m_bCreateAsDialog = FALSE;
	m_hWndTop = NULL;

	m_uDefaultMenuID = -1;

	m_hAccelTable = NULL;
	m_himgBK = NULL;
}

CUIWndBase::~CUIWndBase()
{
	m_sizeScroll = CSize(16, 16);

	if (m_himgBK != NULL)
	{
		ReleaseIUIImage(m_himgBK);
		m_himgBK = NULL;
	}

	if (m_pMember != NULL)
	{
		delete (UIWNDBASEMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}


BEGIN_MESSAGE_MAP(CUIWndBase, CWnd)
	//{{AFX_MSG_MAP(CUIWndBase)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CUIWndBase::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd *pParentWnd, BOOL bUseLayeredWindow/* = TRUE*/)
{
	m_bUseLayeredWindow = bUseLayeredWindow;
	m_uDefaultMenuID = nIDResource;

	// attempt to create the window
	CString strMyClass;
	strMyClass = _T("UIWND");
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(nIDResource));
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

	BOOL bVisible = (dwDefaultStyle & WS_VISIBLE);
	if (bVisible)
	{
		dwDefaultStyle &= ~WS_VISIBLE;
	}

	CString strTitle;
	strTitle.LoadString(nIDResource);
	if (!CreateEx(0, strMyClass, strTitle, dwDefaultStyle, CRect(0, 0, 100, 100), pParentWnd, 0))
	{
		return FALSE;
	}

	// send initial update to all views (and other controls) in the frame
	SendMessageToDescendants(WM_IUIINITIALUPDATE, 0, 0, TRUE, TRUE);

	m_hAccelTable = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(nIDResource));

	if (bVisible)
	{
		ShowWindow(SW_SHOWNA);
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// protected

HWND CUIWndBase::PreModal()
{
	// cannot call DoModal on a dialog already constructed as modeless
	ASSERT(m_hWnd == NULL);

	// allow OLE servers to disable themselves
	CWinApp *pApp = AfxGetApp();
	if (pApp != NULL)
	{
		pApp->EnableModeless(FALSE);
	}

	// find parent HWND
	HWND hWnd = CWnd::GetSafeOwner_(m_pParentWnd->GetSafeHwnd(), &m_hWndTop);

	// hook for creation of dialog
	AfxHookWindowCreate(this);

	// return window to use as parent for dialog
	return hWnd;
}

void CUIWndBase::PostModal()
{
	AfxUnhookWindowCreate();   // just in case
	Detach();               // just in case

	// re-enable windows
	if (::IsWindow(m_hWndTop))
	{
		::EnableWindow(m_hWndTop, TRUE);
	}
	m_hWndTop = NULL;
	CWinApp *pApp = AfxGetApp();
	if (pApp != NULL)
	{
		pApp->EnableModeless(TRUE);
	}
}

int CUIWndBase::HitTest(CPoint pt, BOOL bSetCursor)
{
	CRect rc, rcClient;

	if (IsZoomed())
	{
		return HTCLIENT;
	}

	GetClientRect(&rcClient);
	rc.left = rc.top = 0;
	rc.right = rc.bottom = m_nCXBorder;

	int nRet = HTCLIENT;

	// left top
	if (PtInRect(&rc, pt))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
		}
		nRet = HTTOPLEFT;
		return nRet;
	}

	// top
	rc.left = m_nCXBorder + 1;
	rc.right = rcClient.right - m_nCXBorder - 1;
	rc.bottom = m_nCYBorder;
	if (PtInRect(&rc, pt))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
		}
		nRet = HTTOP;
		return nRet;
	}

	// right top
	rc.left = rcClient.right - m_nCXBorder;
	rc.right = rcClient.right;
	rc.bottom = m_nCXBorder;
	if (PtInRect(&rc, pt))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENESW));
		}
		nRet = HTTOPRIGHT;
		return nRet;
	}

	// right
	rc.top = m_nCXBorder + 1;
	rc.bottom = rcClient.bottom - m_sizeScroll.cy - 1;
	rc.left = rcClient.right - m_nCYBorder;
	if (PtInRect(&rc, pt))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		}
		nRet = HTRIGHT;
		return nRet;
	}

	// right bottom
	rc.top = rcClient.bottom - m_sizeScroll.cy;
	rc.bottom = rcClient.bottom;
	rc.left = rcClient.right - m_sizeScroll.cx;
	if (PtInRect(&rc, pt))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
		}
		nRet = HTBOTTOMRIGHT;
		return nRet;
	}

	// bottom
	rc.left = m_nCXBorder + 1;
	rc.right = rcClient.right - m_sizeScroll.cx - 1;
	rc.top = rcClient.bottom - m_nCYBorder;
	if (PtInRect(&rc, pt))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
		}
		nRet = HTBOTTOM;
		return nRet;
	}

	// left bottom
	rc.left = 0;
	rc.right = m_nCXBorder;
	rc.top = rcClient.bottom - m_nCXBorder;
	if (PtInRect(&rc, pt))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENESW));
		}
		nRet = HTBOTTOMLEFT;
		return nRet;
	}

	// left
	rc.top = m_nCXBorder + 1;
	rc.bottom = rcClient.bottom - m_nCXBorder - 1;
	rc.right = m_nCYBorder;
	if (PtInRect(&rc, pt))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		}
		nRet = HTLEFT;
		return nRet;
	}

	if (bSetCursor)
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
	return nRet;
}

int CUIWndBase::SetLayeredWindow(BOOL bSet)
{
	// use layered window
	LibUIDK::SetLayeredWindow(m_hWnd, bSet);

	UpdateUIWindow();

	return 0;
}

int CUIWndBase::OnDraw(CDC *pMemDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	// Draw background color
	if (m_himgBK->IsHasAlphaChannel())
	{
	}
	else
	{
		// If not set background image, fill solid color.
		if (m_himgBK == NULL)
		{
		}
	}

	// Draw background image
	if (m_himgBK == NULL)
	{
		// Draw windowless children control.

		return 0;
	}

	IUIBitBlt(pMemDC->GetSafeHdc(), rcClient, m_himgBK, 0, 0, SRCCOPY);

	// Draw windowless children control.

	return 0;
}

int CUIWndBase::UpdateUIWindow(int nTransparent/*=255*/)
{
	// ::UpdateLayeredWindow only used with layered window.
	if (!IsLayeredWindow(m_hWnd))
	{
		return -1;
	}

	DWORD dw = GetTickCount();
	if (nTransparent < 0)
	{
		nTransparent = 0;
	}
	if (nTransparent > 255)
	{
		nTransparent = 255;
	}

	CRect rcWin;
	GetWindowRect(rcWin);

	int nWidth = rcWin.Width();
	int nHeight = rcWin.Height();

	// draw image to memory DC
	CDC *pDC = GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rcWin.Width(), rcWin.Height());
	CBitmap *pOldBmp = memDC.SelectObject(&bitmap);

	CRect rcClient;
	GetClientRect(rcClient);
	memDC.FillSolidRect(rcClient, RGB(255, 255, 255));

	// Update Layered Window
	POINT ptWinPos = {rcWin.left, rcWin.top};
	SIZE sizeWindow = {rcWin.Width(), rcWin.Height()};
	POINT ptSrc = {0, 0};

	BLENDFUNCTION blend;
	blend.BlendOp = 0x00;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = nTransparent;//1~255
	blend.AlphaFormat = 0;
	blend.AlphaFormat = 0x01; // AC_SRC_ALPHA;

#if(_MSC_VER <= 1200) // VC6.0
	BOOL bRet = UpdateLayeredWindow(m_hWnd, pDC->GetSafeHdc(), &ptWinPos, &sizeWindow, memDC.GetSafeHdc(), &ptSrc, 0, &blend, 2);
#else
	BOOL bRet = ::UpdateLayeredWindow(m_hWnd, pDC->GetSafeHdc(), &ptWinPos, &sizeWindow, memDC.GetSafeHdc(), &ptSrc, 0, &blend, 2);
#endif
	m_bIsUpdateLayeredWindow = TRUE;

	memDC.SelectObject(pOldBmp);
	ReleaseDC(pDC);

	DWORD dw2 = GetTickCount() - dw;
	return bRet ? 0 : -1;
}

int CUIWndBase::HandleTabKey(MSG *pMsg)
{
	CWnd *pWndFocus = GetFocus();
	if (pWndFocus == NULL)
	{
		return 1;    //IsDialogMessage(pMsg);
	}

	BOOL bCtrl = GetKeyState(VK_CONTROL) < 0;
	BOOL bShift = GetKeyState(VK_SHIFT) < 0;

	int nFocusID = pWndFocus->GetDlgCtrlID();

	//
	// 1. If current focus control is a tab control in a CTaskWndMgr.
	// press Ctrl+Tab switch next tab, press Ctrl+Shift+tab switch to prev tab, Press Tab switch to the first child of task window.
	//

	// Find in all CTaskWndMgr
	CTaskWndInfo TaskInfo;
	CTaskWndMgr *pTaskWndMgr = FindTaskWindow(nFocusID, &TaskInfo);

	if (pTaskWndMgr != NULL)
	{
		// Loop between tabs
		if (bCtrl)
		{
			if (bShift)
			{
				pTaskWndMgr->SwitchToPrevTaskWindow();
			}
			else
			{
				pTaskWndMgr->SwitchToNextTaskWindow();
			}

			return 2;//CWnd::PreTranslateMessage(pMsg);
		}
		// Loop between controls
		else
		{
			if (!bShift)
			{
				if (TaskInfo.m_pTaskWnd->GetSafeHwnd() == NULL)
				{
					return 1;    //IsDialogMessage(pMsg);
				}

				CWnd *pFirstChild = TaskInfo.m_pTaskWnd->GetWindow(GW_CHILD);
				if (pFirstChild->GetSafeHwnd() == NULL)
				{
					return 1;    //IsDialogMessage(pMsg);
				}

				pFirstChild->SetFocus();
				return 3;//TRUE;
			}

			return 1;//IsDialogMessage(pMsg);
		}
	}

	//
	// 2. If the focus control is last control of task window. Press tab key switch to next tab-stop control in parent of task window.
	//
	CWnd *pWndLastTabstop = GetLastTabstopCtrl(NULL);
	if (pWndFocus->GetSafeHwnd() == pWndLastTabstop->GetSafeHwnd())
	{
		CTaskWndMgr *pTaskWndMgr = NULL;
		BOOL bHasTaskWndParent = IsControlInRadioTaskWnd(pWndFocus, &pTaskWndMgr);
		if (bHasTaskWndParent)
		{
			//pTaskWndMgr->GetParent()->GetDlgItem(pTaskWndMgr->GetCurrentTaskWindow())->SetFocus();
			CWnd *pRadio = pTaskWndMgr->GetParent()->GetDlgItem(pTaskWndMgr->GetCurrentTaskWindow());
			CWnd *pNextTabstop = pTaskWndMgr->GetParent()->GetNextDlgTabItem(pRadio, FALSE);
			if (pNextTabstop != NULL)
			{
				pNextTabstop->SetFocus();
				return 3;//TRUE;
			}
		}
	}

	return 1;
}


//////////////////////////////////////////////////////////////////////////
// public

// For dialog
int CUIWndBase::DoModal(DWORD dwStyle/* = 0*/)
{
	m_bCreateAsDialog = TRUE;
	// can be constructed with a resource template or InitModalIndirect

	// load resource as necessary

	// return -1 in case of failure to load the dialog template resource

	// disable parent (before creating dialog)
	HWND hWndParent = PreModal();
	AfxUnhookWindowCreate();
	BOOL bEnableParent = FALSE;
	if (hWndParent != NULL && ::IsWindowEnabled(hWndParent))
	{
		::EnableWindow(hWndParent, FALSE);
		bEnableParent = TRUE;
	}

	TRY
	{
		// create modeless dialog
		AfxHookWindowCreate(this);

		TCHAR szDlgClass[] = _T("UIDIALOG");
		WNDCLASS wndcls;
		memset(&wndcls, 0, sizeof(WNDCLASS));
		wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.hInstance = AfxGetInstanceHandle();
		wndcls.hIcon = NULL;
		wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = szDlgClass;

		// Register the new class and exit if it fails
		if (!AfxRegisterClass(&wndcls))
		{
			TRACE(_T("[LibUIDK]===Class Registration Failed\n"));
			// The call DoModal in DLL release mode. Register class fail.
#ifdef _AFXDLL	// Debug mode
			return FALSE;
#endif
		}

		BOOL bRet = FALSE;
		bRet = CreateEx(0, szDlgClass, _T("UIDIALOG"), WS_POPUP | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | dwStyle, CRect(0, 0, 390, 390), CWnd::FromHandle(hWndParent), NULL);
		if (bRet)
		{
			CenterWindow(CWnd::FromHandle(hWndParent));
			//			if (m_nFlags & WF_CONTINUEMODAL)
			{
				// enter modal loop
				DWORD dwFlags = MLF_SHOWONIDLE;
				if (GetStyle() & DS_NOIDLEMSG)
				{
					dwFlags |= MLF_NOIDLEMSG;
				}

				//				VERIFY(RunModalLoop(dwFlags) == m_nModalResult);
				// when close the dialog by task bar pop-up menu or ALT+F4, the return value is -1;
				int nRet = RunModalLoop(dwFlags);
			}

			// hide the window before enabling the parent, etc.
			if (m_hWnd != NULL)
				SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW |
					SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
		}
	}
	CATCH_ALL(e)
	{
		DELETE_EXCEPTION(e);
		m_nModalResult = -1;
	}
	END_CATCH_ALL

	if (bEnableParent)
	{
		::EnableWindow(hWndParent, TRUE);
	}
	if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
	{
		::SetActiveWindow(hWndParent);
	}

	// destroy modal window
	DestroyWindow();
	PostModal();

	// unlock/free resources as necessary

	return m_nModalResult;
}

void CUIWndBase::EndDialog(int nResult)
{
	ASSERT(::IsWindow(m_hWnd));

	if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL))
	{
		EndModalLoop(nResult);
	}

	::EndDialog(m_hWnd, nResult);
}

int CUIWndBase::SetBkImage(UINT uMask,
	LPCTSTR lpszImageName,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
	LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD,
	BOOL bResetSize/*=TRUE*/)
{
	IUISetControlImage(&m_himgBK, lpszImageNameN);

	if (bResetSize && m_himgBK != NULL)
	{
		int cx = m_himgBK->GetWidth();
		int cy = m_himgBK->GetHeight();
		::SetWindowPos(m_hWnd, NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER);
	}

	Invalidate();

	return 0;
}

int CUIWndBase::SetBkImage(LPCTSTR lpszImageName)
{
	IUISetControlImage(&m_himgBK, lpszImageName);

	Invalidate();
	return 0;
}

HIUIIMAGE CUIWndBase::GetBkImage()
{
	return m_himgBK;
}

int CUIWndBase::SetTitleBarHeight(int nTitleBarHeight)
{
	m_nTitleBarHeight = nTitleBarHeight;

	return 0;
}

int CUIWndBase::GetTitleBarHeight() const
{
	return m_nTitleBarHeight;
}

// Set the size of the scroll that resizing the window
int CUIWndBase::SetScrollSize(CSize size)
{
	CRect rcClient;
	GetClientRect(rcClient);
	if (size.cx >= rcClient.Width() || size.cy >= rcClient.Height())
	{
		return -1;
	}

	m_sizeScroll = size;
	return 0;
}

// Get the size of the scroll that resizing the window
int CUIWndBase::GetScrollSize(CSize *pSize) const
{
	*pSize = m_sizeScroll;
	return 0;
}

// Set width of a window border, in pixels.
int CUIWndBase::SetCXBorder(int cx)
{
	m_nCXBorder = cx;
	return 0;
}

int CUIWndBase::GetCXBorder() const
{
	return m_nCXBorder;
}

// Set height of a window border, in pixels.
int CUIWndBase::SetCYBorder(int cy)
{
	m_nCYBorder = cy;
	return 0;
}

int CUIWndBase::GetCYBorder() const
{
	return m_nCYBorder;
}

int CUIWndBase::SetDragResizeFlag(BOOL bDragResize)
{
	m_bDragResize = bDragResize;
	return 0;
}

BOOL CUIWndBase::GetDragResizeFlag() const
{
	return m_bDragResize;
}

int CUIWndBase::UseLayeredWindow(BOOL bUsed)
{
	ASSERT(m_hWnd != NULL);

	if (bUsed)
	{
		// Add the window WS_EX_LAYERED style, and Set the window transparent
		LibUIDK::SetLayeredWindow(m_hWnd, bUsed);

		// Must call UpdateLayeredWindow or SetLayeredWindowAttributes to show the layered window.

		// Call UpdateLayeredWindow show the layered window.
		if (m_bUpdateLayeredWindow)
		{
			UpdateUIWindow(255);
		}
		// Call SetLayeredWindowAttributes to set the opacity and transparency color key of a layered window.
		else
		{
			HINSTANCE hInst = LoadLibrary(_T("User32.DLL"));
			if (hInst)
			{
				typedef BOOL (WINAPI * MYFUNC)(HWND, COLORREF, BYTE, DWORD);
				MYFUNC SetLayeredWindowAttributes = NULL;
				SetLayeredWindowAttributes = (MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
				if (SetLayeredWindowAttributes)
				{
					DWORD dwFlags = 0;
					dwFlags |= LWA_ALPHA;

					COLORREF crMask = RGB(255, 0, 255);
					SetLayeredWindowAttributes(this->GetSafeHwnd(), crMask, 255, dwFlags);
				}
				FreeLibrary(hInst);
			}
		}
	}
	else
	{
		// Remove the window WS_EX_LAYERED style, and Set the window transparent
		LibUIDK::SetLayeredWindow(m_hWnd, bUsed);
	}

	m_bUseLayeredWindow = bUsed;
	return 0;
}

BOOL CUIWndBase::IsUseLayeredWindow() const
{
	return m_bUseLayeredWindow;
}

CTaskWndMgr *CUIWndBase::FindTaskWindow(CWnd *pTaskWnd, CTaskWndInfo *pTaskInfo)
{
	return NULL;

	// 	// Find in all CTaskWndMgr
	// 	CTaskWndMgr *pTaskWndMgrRet = NULL;
	//
	// 	size_t nChildCount = m_vChildWndList.size();
	// 	for (size_t i = 0; i<nChildCount; ++i)
	// 	{
	// 		CHILDINFO *pci = &m_vChildWndList[i];
	// 		CONTROL_TYPE eControlType = pci->eControlType;
	// 		if (eControlType != CT_TASK_WND_MGR)
	// 			continue;
	//
	// 		CTaskWndMgr *pTaskWndMgr = (CTaskWndMgr *)pci->pWnd;
	// 		if (!pTaskWndMgr->IsCreated())
	// 			continue;
	//
	// 		const CTaskWndInfo *pInfo = pTaskWndMgr->FindTaskWindow(pTaskWnd);
	// 		if (pInfo != NULL)
	// 		{
	// 			pTaskWndMgrRet = pTaskWndMgr;
	//
	// 			if (pTaskInfo != NULL)
	// 			{
	// 				*pTaskInfo = *pInfo;
	// 			}
	//
	// 			break;
	// 		}
	// 	}
	//
	// 	return pTaskWndMgrRet;
}

CTaskWndMgr *CUIWndBase::FindTaskWindow(int nTabRadioID, CTaskWndInfo *pTaskInfo)
{
	return NULL;

	// 	// Find in all CTaskWndMgr
	// 	CTaskWndMgr *pTaskWndMgrRet = NULL;
	//
	// 	size_t nChildCount = m_vChildWndList.size();
	// 	for (size_t i = 0; i<nChildCount; ++i)
	// 	{
	// 		CHILDINFO *pci = &m_vChildWndList[i];
	// 		CONTROL_TYPE eControlType = pci->eControlType;
	// 		if (eControlType != CT_TASK_WND_MGR)
	// 			continue;
	//
	// 		CTaskWndMgr *pTaskWndMgr = (CTaskWndMgr *)pci->pWnd;
	// 		if (!pTaskWndMgr->IsCreated())
	// 			continue;
	//
	// 		const CTaskWndInfo *pInfo = pTaskWndMgr->FindTaskWindow(nTabRadioID);
	// 		if (pInfo != NULL)
	// 		{
	// 			pTaskWndMgrRet = pTaskWndMgr;
	//
	// 			if (pTaskInfo != NULL)
	// 			{
	// 				*pTaskInfo = *pInfo;
	// 			}
	//
	// 			break;
	// 		}
	// 	}
	//
	// 	return pTaskWndMgrRet;
}

CWnd *CUIWndBase::GetLastTabstopCtrl(BOOL *pbHWND)
{
	CWnd *pWndRet = NULL;

	//
	// 1. Search for all HWND children.
	//
	CWnd *pLast = GetWindow(GW_CHILD);
	if (pLast == NULL)
	{
		return pWndRet;
	}

	pLast = pLast->GetWindow(GW_HWNDLAST);
	while (pLast->GetSafeHwnd() != NULL)
	{
		LONG lStyle = GetWindowLong(pLast->GetSafeHwnd(), GWL_STYLE);
		if ((lStyle & WS_TABSTOP))
		{
			pWndRet = pLast;
			if (pbHWND != NULL)
			{
				*pbHWND = TRUE;
			}
			break;
		}

		pLast = pLast->GetWindow(GW_HWNDPREV);
	}

	if (pWndRet != NULL)
	{
		return pWndRet;
	}

	return pWndRet;
}

// Check if a control is a child of the task window manager witch tab is a radio button.
BOOL CUIWndBase::IsControlInRadioTaskWnd(CWnd *pCtrl, CTaskWndMgr **ppTaskWndMgr)
{
	BOOL bFount = FALSE;
	CUIWnd *pParent = (CUIWnd *)pCtrl->GetParent();
	CUIWnd *pParentParent = (CUIWnd *)pParent->GetParent();
	while (pParentParent != NULL)
	{
		// Find in all CTaskWndMgr
		CTaskWndInfo ti;
		CTaskWndMgr *pTaskWndMgr = NULL;
		pTaskWndMgr = pParentParent->FindTaskWindow(pParent, &ti);

		if (pTaskWndMgr != NULL && ti.m_bRadioButton)
		{
			bFount = TRUE;

			if (ppTaskWndMgr != NULL)
			{
				*ppTaskWndMgr = pTaskWndMgr;
			}

			break;
		}

		pParent = pParentParent;
		pParentParent = (CUIWnd *)pParentParent->GetParent();
	}

	return bFount;
}


//////////////////////////////////////////////////////////////////////////

void CUIWndBase::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CRect rcClient;
	GetClientRect(rcClient);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	if (m_bmpCompatible.GetSafeHandle() != NULL)
	{
		m_bmpCompatible.DeleteObject();
	}
	m_bmpCompatible.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	ASSERT(m_bmpCompatible.GetSafeHandle() != NULL);
	CBitmap *pOldMemBmp = memDC.SelectObject(&m_bmpCompatible);
	memDC.FillSolidRect(rcClient, RGB(255, 255, 255));

	OnDraw(&memDC);

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldMemBmp);

	// Do not call CWnd::OnPaint() for painting messages
}

// Support section508
BOOL CUIWndBase::PreTranslateMessage(MSG *pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	// Following code copy from "BOOL CWnd::PreTranslateInput(LPMSG lpMsg)"
	// don't translate non-input events
	if ((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
		(pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
	{
		return CWnd::PreTranslateMessage(pMsg);
	}

	if (m_bCreateAsDialog)
	{
		// The following code copy from CDialog::PreTranslateMessage in Dlgcore.cpp
		// fix around for VK_ESCAPE in a multiline Edit that is on a Dialog
		// that doesn't have a cancel or the cancel is disabled.
		if (pMsg->message == WM_KEYDOWN)
		{
			if ((pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_CANCEL) &&
				(::GetWindowLong(pMsg->hwnd, GWL_STYLE) & ES_MULTILINE) &&
				IUICompareClassName(pMsg->hwnd, _T("Edit")))
			{
				HWND hItem = ::GetDlgItem(m_hWnd, IDCANCEL);
				if (hItem == NULL || ::IsWindowEnabled(hItem))
				{
					SendMessage(WM_COMMAND, IDCANCEL, 0);
					return TRUE;
				}
			}
			else if (pMsg->wParam == VK_TAB)
			{
				int nRet = HandleTabKey(pMsg);
				if (nRet == 1)
				{
					return IsDialogMessage(pMsg);
				}
				else if (nRet == 2)
				{
					return CWnd::PreTranslateMessage(pMsg);
				}
				else if (nRet == 3)
				{
					return TRUE;
				}
			}
		}

		// End copy
	}
	else
	{
		// The general window don't handle esc key.
		if (pMsg->message == WM_KEYDOWN)
		{
			if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_CANCEL)	// The general window NOT handle the escape key.
			{
				return CWnd::PreTranslateMessage(pMsg);
			}
			else if (pMsg->wParam == VK_TAB)
			{
				int nRet = HandleTabKey(pMsg);
				if (nRet == 1)
				{
					return IsDialogMessage(pMsg);
				}
				else if (nRet == 2)
				{
					return CWnd::PreTranslateMessage(pMsg);
				}
				else if (nRet == 3)
				{
					return TRUE;
				}
			}
		}
	}

	// Copy from CMDIFrameWnd::PreTranslateMessage
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		// translate accelerators for frame and any children
		if (m_hAccelTable != NULL && ::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
		{
			return TRUE;
		}
	}

	return IsDialogMessage(pMsg);
}

LRESULT CUIWndBase::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	if (message == WM_SYSCOMMAND)
	{
		if (SC_CLOSE == wParam)
		{
			if (m_bCreateAsDialog)
			{
				EndDialog(IDCANCEL);
			}
		}
	}

	// if the window can be resized by dragging its board, set its position while mouse moving.
	else if (message == WM_MOUSEMOVE)
	{
		CPoint point((DWORD)lParam);
		if (m_bDragResize)
		{
			CRect rect;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
			MINMAXINFO MMI;
			MMI.ptMinTrackSize.x = GetSystemMetrics(SM_CXMINTRACK);
			MMI.ptMinTrackSize.y = GetSystemMetrics(SM_CYMINTRACK);
			MMI.ptMaxSize.x = rect.Width();
			MMI.ptMaxSize.y = rect.Height();
			SendMessage(WM_GETMINMAXINFO, 0, (LPARAM)(&MMI));

			CRect rcWnd;
			GetWindowRect(&rcWnd);

			CPoint pt = point;
			ClientToScreen(&pt);
			int nDx, nDy;
			nDx = pt.x - m_ptLButtonDown.x;
			nDy = pt.y - m_ptLButtonDown.y;

			int nNewW, nNewH;

			switch (m_nHitTestRet)
			{
			case HTTOPLEFT:
				nNewW = m_rcLButtonDown.Width() - nDx;
				if (nNewW < MMI.ptMinTrackSize.x)
				{
					nNewW = MMI.ptMinTrackSize.x;
				}
				nNewH = m_rcLButtonDown.Height() - nDy;
				if (nNewH < MMI.ptMinTrackSize.y)
				{
					nNewH = MMI.ptMinTrackSize.y;
				}
				SetWindowPos(NULL, m_rcLButtonDown.right - nNewW, m_rcLButtonDown.bottom - nNewH, nNewW, nNewH, 0);
				UpdateWindow();
				break;
			case HTTOP:
				nNewW = m_rcLButtonDown.Width();
				nNewH = m_rcLButtonDown.Height() - nDy;
				if (nNewH < MMI.ptMinTrackSize.y)
				{
					nNewH = MMI.ptMinTrackSize.y;
				}
				SetWindowPos(NULL, m_rcLButtonDown.left, m_rcLButtonDown.bottom - nNewH, nNewW, nNewH, 0);
				UpdateWindow();
				break;
			case HTTOPRIGHT:
				nNewW = m_rcLButtonDown.Width() + nDx;
				nNewH = m_rcLButtonDown.Height() - nDy;
				if (nNewH < MMI.ptMinTrackSize.y)
				{
					nNewH = MMI.ptMinTrackSize.y;
				}
				SetWindowPos(NULL, m_rcLButtonDown.left, m_rcLButtonDown.bottom - nNewH, nNewW, nNewH, 0);
				UpdateWindow();
				break;
			case HTRIGHT:
				nNewW = m_rcLButtonDown.Width() + nDx;
				nNewH = m_rcLButtonDown.Height();
				SetWindowPos(NULL, m_rcLButtonDown.left, m_rcLButtonDown.top, nNewW, nNewH, 0);
				UpdateWindow();
				break;
			case HTBOTTOMRIGHT:
				nNewW = m_rcLButtonDown.Width() + nDx;
				nNewH = m_rcLButtonDown.Height() + nDy;
				SetWindowPos(NULL, m_rcLButtonDown.left, m_rcLButtonDown.top, nNewW, nNewH, 0);
				UpdateWindow();
				break;
			case HTBOTTOM:
				nNewW = m_rcLButtonDown.Width();
				nNewH = m_rcLButtonDown.Height() + nDy;
				SetWindowPos(NULL, m_rcLButtonDown.left, m_rcLButtonDown.top, nNewW, nNewH, 0);
				UpdateWindow();
				break;
			case HTBOTTOMLEFT:
				nNewW = m_rcLButtonDown.Width() - nDx;
				if (nNewW < MMI.ptMinTrackSize.x)
				{
					nNewW = MMI.ptMinTrackSize.x;
				}
				nNewH = m_rcLButtonDown.Height() + nDy;
				SetWindowPos(NULL, m_rcLButtonDown.right - nNewW, m_rcLButtonDown.top, nNewW, nNewH, 0);
				UpdateWindow();
				break;
			case HTLEFT:
				nNewW = m_rcLButtonDown.Width() - nDx;
				if (nNewW < MMI.ptMinTrackSize.x)
				{
					nNewW = MMI.ptMinTrackSize.x;
				}
				nNewH = m_rcLButtonDown.Height();
				SetWindowPos(NULL, m_rcLButtonDown.right - nNewW, m_rcLButtonDown.top, nNewW, nNewH, 0);
				UpdateWindow();
				break;

			case HTCLIENT:
				HitTest(point, TRUE);
				break;
			}
		}
	}

	// resize the window by dragging its board
	if (m_bDragResize)
	{
		if (message == WM_LBUTTONDOWN)
		{
			CPoint point((DWORD)lParam);

			m_nHitTestRet = HitTest(point, TRUE);
			if (m_nHitTestRet != HTCLIENT)
			{
				SetCapture();
				m_ptLButtonDown = point;
				ClientToScreen(&m_ptLButtonDown);
				GetWindowRect(&m_rcLButtonDown);
			}
		}

		if (message == WM_LBUTTONUP)
		{
			m_nHitTestRet = HTCLIENT;
			m_rcLButtonDown.SetRectEmpty();
			m_ptLButtonDown.x = m_ptLButtonDown.y = 0;
			ReleaseCapture();
		}

		if (message == WM_CAPTURECHANGED)
		{
			HWND hWndNew = HWND(lParam);
			if (hWndNew != m_hWnd)
			{
				m_nHitTestRet = HTCLIENT;
				m_rcLButtonDown.SetRectEmpty();
				m_ptLButtonDown.x = m_ptLButtonDown.y = 0;
			}
		}
	}

	if (message == WM_LBUTTONDOWN && 0 != m_nTitleBarHeight)
	{
		CPoint point((DWORD)lParam);

		m_nHitTestRet = HitTest(point, TRUE);

		if (m_nHitTestRet == HTCLIENT && !IsZoomed() && !IsIconic())
		{
			if ((m_nTitleBarHeight > 0 && point.y < m_nTitleBarHeight) || (m_nTitleBarHeight < 0))
			{
				SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
			}
		}
	}

	if (message == WM_LBUTTONDBLCLK && m_nTitleBarHeight != 0)
	{
		CPoint point((DWORD)lParam);

		m_nHitTestRet = HitTest(point, TRUE);

		if (m_nHitTestRet == HTCLIENT)
		{
			if ((m_nTitleBarHeight > 0 && point.y < m_nTitleBarHeight) || (m_nTitleBarHeight < 0))
			{
				if (IsZoomed())
				{
					SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
				}
				else
				{
					SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
				}
			}
		}
	}

	if (message == WM_ERASEBKGND)
	{
		return TRUE;
	}

	if (message == WM_WINDOWPOSCHANGING)
	{
		WINDOWPOS *pPos = (WINDOWPOS *)lParam;

		CRect rect;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
		MINMAXINFO MMI;
		MMI.ptMinTrackSize.x = GetSystemMetrics(SM_CXMINTRACK);
		MMI.ptMinTrackSize.y = GetSystemMetrics(SM_CYMINTRACK);
		MMI.ptMaxSize.x = rect.Width();
		MMI.ptMaxSize.y = rect.Height();
		SendMessage(WM_GETMINMAXINFO, 0, (LPARAM)(&MMI));

		if (pPos->cx < MMI.ptMinTrackSize.x)
		{
			pPos->cx = MMI.ptMinTrackSize.x;
		}
		if (pPos->cy < MMI.ptMinTrackSize.y)
		{
			pPos->cy = MMI.ptMinTrackSize.y;
		}
		if (pPos->cx > MMI.ptMaxSize.x)
		{
			pPos->cx = MMI.ptMaxSize.x;
		}
		if (pPos->cy > MMI.ptMaxSize.y)
		{
			pPos->cy = MMI.ptMaxSize.y;
		}
	}

	if (message == WM_UPDATELAYEREDUIWND)
	{
		UpdateUIWindow();
	}

	if (message == WM_ISUPDATELAYEREDWINDOW)
	{
		CWnd::WindowProc(message, wParam, lParam);
		return m_bIsUpdateLayeredWindow;
	}

	if (message == WM_GETBACKGROUND)
	{
		CWnd::WindowProc(message, wParam, lParam);
		return (LRESULT)m_bmpCompatible.GetSafeHandle();
	}

	if (message == WM_SHOWWINDOW && lParam == 0)
	{
		CWnd *pParent = GetParent();
		if (pParent != NULL)
		{
			pParent->SendMessage(WM_SHOWUIWINDOW, wParam, (LPARAM)this);
		}
	}

	return CWnd::WindowProc(message, wParam, lParam);
}
