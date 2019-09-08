// WLWnd.cpp : implementation file
//

#include "stdafx.h"
#include <algorithm>
#include <WINDOWSX.H>
#include "ControlMember.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


struct WLMgr
{
public:
	WLMgr()
	{
		pFocus = NULL;
	}

	CWLWnd *pFocus;
	std::vector<CWLWnd *> vpWLChildren;
};


CWLWnd *LibUIDK::GetDlgWLItem(HWND hWnd, int nItemID)
{
	WLMgr *pWLMgr = (WLMgr *)::GetProp(hWnd, GETPROP_WLMGR);
	if (pWLMgr == NULL)
	{
		return NULL;
	}

	CWLWnd *pRet = NULL;

	int nCount = (int)pWLMgr->vpWLChildren.size();
	for (int i = 0; i < nCount; ++i)
	{
		int nID = pWLMgr->vpWLChildren[i]->GetDlgCtrlID();
		if (nID == nItemID)
		{
			pRet = pWLMgr->vpWLChildren[i];
			break;
		}
	}

	return pRet;
}

CWLWnd *LibUIDK::GetFirstWLWindow(HWND hWnd)
{
	WLMgr *pWLMgr = (WLMgr *)::GetProp(hWnd, GETPROP_WLMGR);
	if (pWLMgr == NULL)
	{
		return NULL;
	}

	int nCount = (int)pWLMgr->vpWLChildren.size();
	if (nCount <= 0)
	{
		return NULL;
	}

	return pWLMgr->vpWLChildren[0];
}

CWLWnd *LibUIDK::GetNextWLWindow(CWLWnd *pWnd, UINT wCmd)
{
	if (pWnd == NULL)
	{
		return NULL;
	}

	WLMgr *pWLMgr = (WLMgr *)::GetProp(pWnd->GetParent()->GetSafeHwnd(), GETPROP_WLMGR);
	if (pWLMgr == NULL)
	{
		return NULL;
	}

	int nCount = (int)pWLMgr->vpWLChildren.size();
	if (nCount <= 0)
	{
		return NULL;
	}

	int nIndex = -1;
	int i = 0;
	for (; i < nCount; ++i)
	{
		CWLWnd *pChild = pWLMgr->vpWLChildren[i];
		if (pChild == pWnd)
		{
			nIndex = i;
			break;
		}
	}

	if (nIndex == -1 || i == nCount)
	{
		return NULL;
	}

	CWLWnd *pNext = NULL;
	if (wCmd == GW_HWNDNEXT)
	{
		if (nIndex < nCount - 1)
		{
			pNext = pWLMgr->vpWLChildren[nIndex + 1];
		}
	}
	else
	{
		if (nIndex > 0)
		{
			pNext = pWLMgr->vpWLChildren[nIndex - 1];
		}
	}

	return pNext;
}

int LibUIDK::SetWLWindowZOrder(CWLWnd *pWLWnd, CWLWnd *pWLWndInsertAfter)
{
	if (pWLWnd == NULL)
	{
		return -1;
	}

	WLMgr *pWLMgr = (WLMgr *)::GetProp(pWLWnd->GetParent()->GetSafeHwnd(), GETPROP_WLMGR);
	if (pWLMgr == NULL)
	{
		return -2;
	}

	int nCount = (int)pWLMgr->vpWLChildren.size();
	if (nCount <= 0)
	{
		return -3;
	}

	// Erase
	int nIndex = -1;
	int nInsertAfterIndex = -1;
	int i = 0;
	for (; i < nCount; ++i)
	{
		CWLWnd *pChild = pWLMgr->vpWLChildren[i];

		if (pChild == pWLWnd)
		{
			nIndex = i;
			break;
		}
	}

	if (nIndex == -1 || i == nCount)
	{
		return -4;
	}

	pWLMgr->vpWLChildren.erase(pWLMgr->vpWLChildren.begin() + nIndex);

	// Re-insert
	for (i = 0; i < nCount; ++i)
	{
		CWLWnd *pChild = pWLMgr->vpWLChildren[i];
		if (pChild == pWLWndInsertAfter)
		{
			nInsertAfterIndex = i;
			break;
		}
	}

	if (nInsertAfterIndex ==  -1 || i == nCount)
	{
		pWLMgr->vpWLChildren.insert(pWLMgr->vpWLChildren.begin() + nIndex, pWLWnd);
		return -4;
	}

	pWLMgr->vpWLChildren.insert(pWLMgr->vpWLChildren.begin() + nInsertAfterIndex, pWLWnd);

	return 0;
}

int LibUIDK::SetDlgWLItemText(HWND hDlg, int nIDDlgItem, LPCTSTR lpString)
{
	int nRet = (int)::SendMessage(hDlg, WM_SETWLITEMTEXT, nIDDlgItem, (LPARAM)lpString);

	return nRet;
}

CWLWnd *LibUIDK::GetWLCapture()
{
	_IUI_THREAD_STATE *pThreadState = IUIGetThreadState();
	return pThreadState->m_pWLCapture;
}

HRGN LibUIDK::ApplyWLRgn(CWnd *pParent, CDC *pParentMemDC, CWLWnd *pChild)
{
	CRect rcChild;
	pChild->GetWindowRect(rcChild);
	pParent->ScreenToClient(rcChild);

	// Get the windowless's region, The system does not make a copy of the region.
	HRGN hRgnControl = CreateRectRgn(0, 0, 0, 0);
	int nRet = pChild->GetWindowRgn(hRgnControl);
	if (nRet == ERROR)
	{
		::DeleteObject(hRgnControl);
		return NULL;
	}

	// Convert to relative to parent
	::OffsetRgn(hRgnControl, rcChild.left, rcChild.top);

	// Get old region.
	HRGN hOldParentRgn = CreateRectRgn(0, 0, 0, 0);
	int nRet2 = GetClipRgn(pParentMemDC->GetSafeHdc(), hOldParentRgn);	// If the function succeeds, the region is a handle to a copy of the current clipping region.
	if (nRet2 == 1)	// the function succeeds and there is a clipping region for the given device context.
	{
		// combine region
		HRGN hNewRgn = CreateRectRgn(0, 0, 0, 0);

		CombineRgn(hNewRgn, hOldParentRgn, hRgnControl, RGN_AND);
		::SelectClipRgn(pParentMemDC->GetSafeHdc(), hNewRgn);
		DeleteObject(hNewRgn);
	}
	else
	{
		::SelectClipRgn(pParentMemDC->GetSafeHdc(), hRgnControl);
	}

	::DeleteObject(hRgnControl);

	return hOldParentRgn;
}

#define WM_WL_GETPROPMAP				WM_WINDOWLESSLAST - 1
BOOL LibUIDK::SetWLProp(CWLWnd *pWLWnd, LPCTSTR lpString, HANDLE hData)		// Same as SetProp for windowless object.
{
	if (pWLWnd == NULL || lpString == NULL)
	{
		return FALSE;
	}

	std::map<CString, HANDLE> *pmapProp = (std::map<CString, HANDLE> *)pWLWnd->SendMessage(WM_WL_GETPROPMAP);
	if (pmapProp == NULL)
	{
		return FALSE;
	}

	(*pmapProp)[lpString] = hData;

	return TRUE;
}

HANDLE LibUIDK::GetWLProp(CWLWnd *pWLWnd, LPCTSTR lpString)
{
	if (pWLWnd == NULL || lpString == NULL)
	{
		return FALSE;
	}

	std::map<CString, HANDLE> *pmapProp = (std::map<CString, HANDLE> *)pWLWnd->SendMessage(WM_WL_GETPROPMAP);
	if (pmapProp == NULL)
	{
		return FALSE;
	}

	std::map<CString, HANDLE>::iterator it = pmapProp->find(lpString);
	if (it == pmapProp->end())
	{
		return FALSE;
	}

	return it->second;
}

int LibUIDK::DrawWLWindow(HDC hDstDC, CWLWnd *pChild)
{
	if (hDstDC == NULL || pChild == NULL)
	{
		return -1;
	}

	if (!pChild->IsCreated() || !pChild->IsWindowVisible())
	{
		return -2;
	}

	return pChild->OnDraw(CDC::FromHandle(hDstDC));
}

/////////////////////////////////////////////////////////////////////////////
// CWLWnd

#ifdef _DEBUG
class CWLWndMap
{
public:
	CWLWndMap()
	{
		m_nWLWndCount = 0;
	}

	~CWLWndMap()
	{
		// Check CWLWnd * leak.
		int n = 0;
	}

public:
	std::map<CWLWnd *, int> m_mapWLWnd; // second: alloc index
	int m_nWLWndCount;
};

CWLWndMap g_mapWLWnd;
#endif // _DEBUG

CWLWnd::CWLWnd()
{
	m_pMember = new WLWNDMEMBER;
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

#ifdef _DEBUG
	static int nWLWndAllocIndex = 0;
	pMember->m_nAllocIndex = nWLWndAllocIndex;
	g_mapWLWnd.m_mapWLWnd[this] = nWLWndAllocIndex++;
	g_mapWLWnd.m_nWLWndCount = nWLWndAllocIndex;
#endif // _DEBUG


	m_bFlag = TRUE;	// TRUE: the windowless control created by user; FALSE: created by LIBUIDK.
	CControlBase::m_bWLWnd = true;
}

CWLWnd::~CWLWnd()
{
#ifdef _DEBUG
	std::map<CWLWnd *, int>::iterator it = g_mapWLWnd.m_mapWLWnd.find(this);
	if (it != g_mapWLWnd.m_mapWLWnd.end())
	{
		g_mapWLWnd.m_mapWLWnd.erase(it);
	}
	else
	{
		ASSERT(FALSE);
	}
#endif // _DEBUG

	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	if (IsCreated())
	{
		TRACE(_T("[LibUIDK]===Warning: calling DestroyWindow in CWLWnd::~CWLWnd, OnDestroy or PostNcDestroy in derived class will not be called.\n"));
		DestroyWindow();
	}

	// Call after IsCreated, since IsCreated use pMember->m_bCreated.
	pMember->Release();

	if (m_pMember != NULL)
	{
		delete (WLWNDMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

int GeneralWindowlessMouseLeaveMessage(HWND hwnd, POINT pt)
{
	// If the mouse leave the control, send leave message.
	_IUI_THREAD_STATE *pThreadState = IUIGetThreadState();
	if (pThreadState->m_pWLMouseIn == NULL)
	{
		return 1;
	}

	pThreadState->m_pWLMouseIn->SendMessage(WM_MOUSELEAVE);

#ifdef _DEBUG
	CString strName;
	pThreadState->m_pWLMouseIn->GetWindowText(strName);
	CString strInfo;
	strInfo.Format(
		_T("[LibUIDK]===Info: Mouse leave windowless '%s'\n"), strName);
	OutputDebugString(strInfo);
#endif

	pThreadState->m_pWLMouseIn = NULL;

	return 0;
}

LRESULT WLParentWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC wndprocOld = CWndProcList::GetOldWndProc(hwnd, (WNDPROC)WLParentWndProc);
	if (wndprocOld == NULL)
	{
		return 0;
	}

#ifdef _DEBUG
	CString strClassName;
	GetClassName(hwnd, strClassName.GetBufferSetLength(MAX_PATH), MAX_PATH);
	strClassName.ReleaseBuffer();
#endif

	// Send WM_MOUSELEAVE to the last windowless control that mouse in.
	if (uMsg == WM_MOUSELEAVE)
	{
		_IUI_THREAD_STATE *pThreadState = IUIGetThreadState();
		if (pThreadState->m_pWLMouseIn != NULL)
		{
			pThreadState->m_pWLMouseIn->SendMessage(WM_MOUSELEAVE);
			pThreadState->m_pWLMouseIn = NULL;
		}
	}

	// If the message is a mouse message, do a hit test, then, pass the message to the child.
	if (uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST)
	{
		HWND hCapture = GetCapture();

		// If there has a capture windowless control, pass the message to it directly.
		_IUI_THREAD_STATE *pThreadState = IUIGetThreadState();
		if (pThreadState->m_pWLCapture != NULL)
		{
			if (pThreadState->m_pWLCapture->GetParent()->GetSafeHwnd() == hwnd)
			{
				ASSERT(pThreadState->m_pWLCapture->IsWindowEnabled() && pThreadState->m_pWLCapture->IsWindowVisible());

				// Convert point to relate to captured windowless child.
				CPoint point(lParam);
				ClientToScreen(hwnd, &point);
				pThreadState->m_pWLCapture->ScreenToClient(&point);

				return pThreadState->m_pWLCapture->WLWindowProc(uMsg, wParam, MAKELPARAM(point.x, point.y));
			}
		}
		else if (hCapture != NULL)
		{
			// If there has a window captured mouse input and no windowless control captured mouse input,
			// don't pass mouse message to windowless control.
		}
		else
		{
			CPoint pt(lParam);	// lParam: relate to hwnd
			WLMgr *pWLMgr = (WLMgr *)::GetProp(hwnd, GETPROP_WLMGR);
			int nCount = (int)pWLMgr->vpWLChildren.size();
			for (int i = nCount - 1; i >= 0; --i)		// Z-order
			{
				CWLWnd *pChild = (pWLMgr->vpWLChildren)[i];

				RECT rcChild;
				if (pChild->IsOleControl())
				{
					((IOleCtrlBase *)pChild)->GetWindowRect(&rcChild);
				}
				else
				{
					pChild->GetWindowRect(&rcChild);
				}
				::ScreenToClient(hwnd, (LPPOINT)&rcChild);
				::ScreenToClient(hwnd, ((LPPOINT)&rcChild) + 1);

				BOOL bPtInRegion = TRUE;
				HRGN hRgnChild = (HRGN)pChild->WLWindowProc(WM_WL_GETWINDOWRGNREF, 0, 0);
				if (hRgnChild != NULL)
				{
					bPtInRegion = ::PtInRegion(hRgnChild, pt.x - rcChild.left, pt.y - rcChild.top);
				}

				CRect rect(rcChild);

				// If the windowless control scroll out of virtual parent
				if (pChild->GetVirtualParent() != NULL)
				{
					CRect rcVirtualParent;
					pChild->GetVirtualParent()->GetWindowRect(rcVirtualParent);
					::ScreenToClient(hwnd, (LPPOINT)&rcVirtualParent);
					::ScreenToClient(hwnd, ((LPPOINT)&rcVirtualParent) + 1);

					rect.IntersectRect(rect, rcVirtualParent);
				}

				//
				if (rect.PtInRect(pt) && bPtInRegion)
				{
					// Only enabled and visible window can handle mouse message.
					if (pChild->IsWindowEnabled() && pChild->IsWindowVisible())
					{
						if (pThreadState->m_pWLMouseIn != pChild)
						{
							// If enter new control, send mouse leave message to the old control.
							GeneralWindowlessMouseLeaveMessage(hwnd, pt);

							pThreadState->m_pWLMouseIn = pChild;
						}
						LPARAM lParam2 = MAKELPARAM(pt.x - rcChild.left, pt.y - rcChild.top); // the coordinate relate to windowless control.
						LRESULT lr = pChild->WLWindowProc(uMsg, wParam, lParam2);	// If the child handle the message, refuse parent to handle
						return lr;
					}
				}
			}

			// If not enter new control, send mouse leave message to the old control
			GeneralWindowlessMouseLeaveMessage(hwnd, pt);
		}
	}
	else if (uMsg == WM_SETCURSOR)
	{
		HWND hSender = HWND(wParam);
		if (hwnd == hSender)
		{
			// Get current cursor position relate to hwnd.
			CPoint pt;
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);

			// Hit test child windowless control.
			WLMgr *pWLMgr = (WLMgr *)::GetProp(hwnd, GETPROP_WLMGR);
			int nCount = (int)pWLMgr->vpWLChildren.size();
			for (int i = nCount - 1; i >= 0; --i)		// Z-order
			{
				CWLWnd *pChild = (pWLMgr->vpWLChildren)[i];

				RECT rcChild;
				pChild->GetWindowRect(&rcChild);
				::ScreenToClient(hwnd, (LPPOINT)&rcChild);
				::ScreenToClient(hwnd, ((LPPOINT)&rcChild) + 1);

				BOOL bPtInRegion = TRUE;
				HRGN hRgnChild = (HRGN)pChild->WLWindowProc(WM_WL_GETWINDOWRGNREF, 0, 0);
				if (hRgnChild != NULL)
				{
					bPtInRegion = ::PtInRegion(hRgnChild, pt.x - rcChild.left, pt.y - rcChild.top);
				}

				CRect rect(rcChild);

				// If the windowless control scroll out of virtual parent
				if (pChild->GetVirtualParent() != NULL)
				{
					CRect rcVirtualParent;
					pChild->GetVirtualParent()->GetWindowRect(rcVirtualParent);
					::ScreenToClient(hwnd, (LPPOINT)&rcVirtualParent);
					::ScreenToClient(hwnd, ((LPPOINT)&rcVirtualParent) + 1);

					rect.IntersectRect(rect, rcVirtualParent);
				}

				if (rect.PtInRect(pt) && bPtInRegion)
				{
					// Only enabled and visible window can handle mouse message.
					if (pChild->IsWindowEnabled() && pChild->IsWindowVisible())
					{
						// If the child windowless control's WM_SETCURSOR message return TRUE, its parent window shouldn't receive WM_SETCURSOR message.
						BOOL bRet = (BOOL)pChild->WLWindowProc(uMsg, wParam, lParam);	// If the child handle the message, refuse parent to handle
						if (bRet)
						{
							return TRUE;
						}
					}

					break;
				}
			}
		}
	}
	// If the message is a keyboard message, check the focus windowless control, then, pass the message to it.
	else if (uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST)
	{

	}
	// Windowless control has no window handle, so, can't pass other messages to it.

	if (uMsg == WM_DESTROY)
	{
		// Destroy child windowless controls
		WLMgr *pWLMgr = (WLMgr *)::GetProp(hwnd, GETPROP_WLMGR);
		ASSERT(pWLMgr != NULL);

		std::vector<CWLWnd *>::reverse_iterator itr = pWLMgr->vpWLChildren.rbegin();
		for (; itr != pWLMgr->vpWLChildren.rend();)
		{
			CWLWnd *pChildWLWnd = *itr;
			ASSERT(pChildWLWnd->IsCreated());
			pChildWLWnd->DestroyWindow();

			itr = pWLMgr->vpWLChildren.rbegin();
		}

		ASSERT(pWLMgr->vpWLChildren.empty());
	}
	else if (uMsg == WM_NCDESTROY)	// the last message for a window
	{
		WLMgr *pWLMgr = (WLMgr *)::GetProp(hwnd, GETPROP_WLMGR);
		ASSERT(pWLMgr != NULL);
		delete pWLMgr;
		::SetProp(hwnd, GETPROP_WLMGR, NULL);
	}
	else if (uMsg == WM_KILLFOCUS)
	{
		_IUI_THREAD_STATE *pThreadState = IUIGetThreadState();
		if (pThreadState->m_pWLCapture != NULL && pThreadState->m_pWLCapture->GetParent()->GetSafeHwnd() == hwnd)
		{
			pThreadState->m_pWLCapture = NULL;
		}
	}

	return CallWindowProc(wndprocOld, hwnd, uMsg, wParam, lParam);
}

BOOL CWLWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
	const RECT &rect, CWnd *pParentWnd, UINT nID, CWLWnd *pVirtualParent)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	// 创建出来的windowless控件，是否加入到父窗口windowless列表WLMgr::vpWLChildren中，并子类化父窗口
	WLMgr *pWLMgr = NULL;
	BOOL bNotCreateAsChild = (BOOL)pParentWnd->SendMessage(WLNM_NOTCREATEASCHILD);
	if (!bNotCreateAsChild)
	{
		pWLMgr = (WLMgr *)::GetProp(pParentWnd->GetSafeHwnd(), GETPROP_WLMGR);
		if (pWLMgr == NULL)
		{
			pWLMgr = new WLMgr;
			::SetProp(pParentWnd->GetSafeHwnd(), GETPROP_WLMGR, pWLMgr);

			// Hook the parent destroy, and to delete pWLChildren.
			WNDPROC procOld = (WNDPROC)IUISetWindowLong(pParentWnd->GetSafeHwnd(),
					GWLP_WNDPROC, (LONG_PTR)WLParentWndProc);
			CWndProcList::AddToMap(pParentWnd->GetSafeHwnd(), procOld, (WNDPROC)WLParentWndProc);
		}
	}

	if (lpszClassName != NULL && _tcslen(lpszClassName) > 0)
	{
		pMember->m_strClassName = lpszClassName;
	}
	pMember->m_strWindowName = lpszWindowName;
	pMember->m_dwStyle = dwStyle;
	pMember->m_rcOrig = rect;
	pMember->m_rcCurrent = rect;
	pMember->m_pParent = pParentWnd;
	pMember->m_pVirtualParent = pVirtualParent;
	pMember->m_nID = nID;
	pMember->m_bCreated = TRUE;

	if ((dwStyle & WS_VISIBLE) == WS_VISIBLE)
	{
		pMember->m_bVisible = TRUE;
	}
	else
	{
		pMember->m_bVisible = FALSE;
	}

	if ((dwStyle & WS_DISABLED) == WS_DISABLED)
	{
		pMember->m_bEnabled = FALSE;
	}
	else
	{
		pMember->m_bEnabled = TRUE;
	}

	int nRet = OnCreate();
	pParentWnd->SendMessage(WLNM_CREATE, 0, (LPARAM)this);

	// Save to children list
	if (!bNotCreateAsChild)
	{
		if (pWLMgr != NULL)
		{
			pWLMgr->vpWLChildren.push_back(this);
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	return TRUE;
}

BOOL CWLWnd::IsCreated() const
{
	if (this == NULL)
	{
		return FALSE;
	}

	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;
	return pMember->m_bCreated;
}

//////////////////////////////////////////////////////////////////////////
// protected

int CWLWnd::OnCreate()
{
	return 0;
}

int CWLWnd::OnDraw(CDC *pMemDCParent)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	if (!IsWindowVisible())
	{
		return 1;
	}

	if (pMemDCParent == NULL)
	{
		return -1;
	}

	CRect rcWin;
	GetWindowRect(rcWin);
	GetParent()->ScreenToClient(rcWin);
	rcWin.OffsetRect(pMember->m_ptDrawOffset);

	// Don't draw this control if it's out of the parent
	CRect rcParent;
	GetParent()->GetClientRect(rcParent);

	CRect rcIntersect;
	rcIntersect.IntersectRect(rcWin, rcParent);
	if (rcIntersect.IsRectEmpty())
	{
		return 0;
	}

	//
	CPoint ptOldOrg = pMemDCParent->GetViewportOrg();
	pMemDCParent->SetViewportOrg(pMember->m_ptDrawOffset);


	DRAWITEMSTRUCT dis;
	dis.CtlType = ODT_WLWND;
	dis.CtlID = GetDlgCtrlID();
	dis.itemID = -1;
	dis.hwndItem = (HWND)this;
	dis.hDC = pMemDCParent->GetSafeHdc();
	dis.rcItem = rcWin;
	dis.itemData = (ULONG_PTR)GetTag();

	BOOL bParentDraw = (BOOL)GetParent()->SendMessage(WM_DRAWITEMEX, dis.CtlID, LPARAM(&dis));
	if (bParentDraw)
	{
		return 0;
	}

	//
	if (IsDrawBkColor())
	{
		pMemDCParent->FillSolidRect(rcWin, GetBackgroundColor());
	}

	if (pMember->m_hBackground->GetSafeHBITMAP() == NULL)
	{
		pMember->m_hBackground->SafeLoadSavedImage();
	}

	if (pMember->m_hBackground->GetSafeHBITMAP() != NULL)
	{
		IUIDrawImage(pMemDCParent->GetSafeHdc(), rcWin, pMember->m_hBackground,
			m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
	}

	// Draw children

	//
	pMemDCParent->SetViewportOrg(ptOldOrg);

	return 0;
}

// The pMemDCParent may be not parent HWND's DC.
// Such as rich edit's child OLE object's DC is rich edit parent.
int CWLWnd::OnDraw(CDC *pMemDCParent, LPPOINT lpptOrg)
{
	if (lpptOrg == NULL)
	{
		return -1;
	}

	CPoint ptOldOrg = pMemDCParent->GetViewportOrg();
	pMemDCParent->SetViewportOrg(*lpptOrg);

	OnDraw(pMemDCParent);

	pMemDCParent->SetViewportOrg(ptOldOrg);

	return 0;
}

LRESULT CWLWnd::WLWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	if (!IsCreated())
	{
		return 0;
	}

	if (message == WM_WL_GETWINDOWRGNREF)
	{
		return (LRESULT)pMember->m_hRgn;
	}

	BOOL bMouseMsg = IsMouseMessage(message, wParam, lParam);
	if (bMouseMsg)
	{
		return 1;    // refuse parent handle
	}

	// Other messages	//? IsMouseMessage call WindowProc also.
	return DefWindowProc(message, wParam, lParam);
}

LRESULT CWLWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	// special case for notifies
	if (message == WM_NOTIFY)
	{
		NMHDR *pNMHDR = (NMHDR *)lParam;
		LRESULT lResult = 0;
		if (pNMHDR->hwndFrom != NULL && OnNotify(wParam, lParam, &lResult))
		{
			return TRUE;
		}

		return FALSE;
	}

	else if (message == WM_WL_CALLONDRAW)
	{
		CDC *pMemDCParent = (CDC *)wParam;
		OnDraw(pMemDCParent);
	}

	else if (message == WM_WL_GETPROPMAP)
	{
		return (LRESULT)&pMember->m_mapProp;
	}

	else if (message == WM_WL_GETLBTNDOWNHITTESTRESULT)
	{
		return pMember->m_uLButtonDownHitTestRet;
	}

	else if (message == WM_WL_MOVEWINDOW)
	{
		LPCRECT lprcNew = (LPCRECT)lParam;
		pMember->m_rcCurrent = *lprcNew;
	}

	return 0;
}

class CReflectWnd : public CWnd
{
public:
	BOOL OnRefChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT *pResult)
	{
		return OnChildNotify(message, wParam, lParam, pResult);
	}
};

BOOL CWLWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT *pResult)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	ASSERT(pResult != NULL);
	NMHDR *pNMHDR = (NMHDR *)lParam;
	HWND hWndCtrl = pNMHDR->hwndFrom;

	// get the child ID from the window itself
	UINT nID = ::GetDlgCtrlID(hWndCtrl);
	int nCode = pNMHDR->code;

	ASSERT(hWndCtrl != NULL);
	ASSERT(::IsWindow(hWndCtrl));

	// reflect notification to child window control
	CReflectWnd *pChild = (CReflectWnd *)CWnd::FromHandlePermanent(hWndCtrl);
	if (pChild == NULL)
	{
		return FALSE;
	}
	if (pChild->OnRefChildNotify(WM_NOTIFY, wParam, lParam, pResult))
	{
		return TRUE;
	}

	return TRUE;
}

int SendMouseNotify(HWND hWndParent, UINT uCtrlID, UINT message, WPARAM wParam, LPARAM lParam)
{
	NMHDR nmhdr;
	nmhdr.hwndFrom = NULL;
	nmhdr.idFrom = uCtrlID;
	nmhdr.code = WLN_MOUSE;

	NMMOUSE nmMouse;
	nmMouse.hdr = nmhdr;
	nmMouse.dwItemData = message;
	nmMouse.pt.x = (short)GET_X_LPARAM(lParam);
	nmMouse.pt.y = (short)GET_Y_LPARAM(lParam);

	ASSERT(::IsWindow(hWndParent));
	int nRet = (int)::SendMessage(hWndParent, WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmMouse));

	return nRet;
}

BOOL CWLWnd::IsMouseMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	if (message < WM_MOUSEFIRST || message > WM_MOUSELAST)
	{
		return FALSE;
	}

	CPoint point(lParam);	// Relate to control

	SendMouseNotify(GetParent()->GetSafeHwnd(), GetDlgCtrlID(), message, wParam, lParam);

	CRect rcClient;
	GetClientRect(rcClient);

	if (rcClient.PtInRect(point))
	{
		pMember->m_bMouseOver = TRUE;
	}
	else
	{
		if (pMember->m_bMouseOver)	// First leave the control
		{
			message = WM_MOUSELEAVE;	// Translate to mouse leave message.

			pMember->m_bMouseOver = FALSE;
		}
		else
		{
			_IUI_THREAD_STATE *pThreadState = IUIGetThreadState();
			if (pThreadState->m_pWLCapture != this)
			{
				return TRUE;
			}
		}
	}

	// Give parent a chance to handle mouse message, simulate PreTranslateMsg.
	WLMOUSE wlMouse;
	wlMouse.wParam = wParam;
	wlMouse.lParam = lParam;
	wlMouse.pWLWnd = this;
	LRESULT lr = GetParent()->SendMessage(WM_WLMOUSEFIRST + message - WM_MOUSEFIRST, pMember->m_nID, (LPARAM)&wlMouse);
	if (lr != 0)
	{
		return TRUE;
	}

	CPoint pt(lParam);
	pMember->m_uLButtonDownHitTestRet = HitTest(pt, TRUE);

	if (message == WM_LBUTTONDOWN)
	{
		pMember->m_ptLButtonDown = pt;
		GetWindowRect(pMember->m_rcLButtonDownRelateParent);
		GetParent()->ScreenToClient(pMember->m_rcLButtonDownRelateParent);

		// Press title bar to moving or sizing the window less control.
		if (pMember->m_uLButtonDownHitTestRet != HTCLIENT)
		{
			SetCapture();

			pMember->m_bEnterSizeMove = TRUE;
			SendMessage(WM_WL_ENTERSIZEMOVE);

			NMHDR nmhdr;
			nmhdr.hwndFrom = NULL;
			nmhdr.idFrom = GetDlgCtrlID();
			nmhdr.code = WLN_ENTERSIZEMOVE;

			NMRECT nmRect;
			nmRect.hdr = nmhdr;
			nmRect.lParam = (LPARAM)this;

			BOOL bRet = (BOOL)pMember->m_pParent->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmRect));
			if (bRet)
			{
				return 0;
			}
		}
	}
	else if (message == WM_LBUTTONUP)
	{
		pMember->m_ptLButtonDown = CPoint(-1, -1);
		pMember->m_rcLButtonDownRelateParent.SetRectEmpty();
		ReleaseCapture();
		pMember->m_uLButtonDownHitTestRet = HTCLIENT;

		if (pMember->m_bEnterSizeMove)
		{
			pMember->m_bEnterSizeMove = FALSE;

			SendMessage(WM_WL_EXITSIZEMOVE);

			NMHDR nmhdr;
			nmhdr.hwndFrom = NULL;
			nmhdr.idFrom = GetDlgCtrlID();
			nmhdr.code = WLN_EXITSIZEMOVE;

			NMRECT nmRect;
			nmRect.hdr = nmhdr;
			nmRect.lParam = (LPARAM)this;

			BOOL bRet = (BOOL)pMember->m_pParent->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmRect));
			if (bRet)
			{
				return 0;
			}
		}
	}
	else if (message == WM_MOUSEMOVE)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.hwndTrack = pMember->m_pParent->GetSafeHwnd();
		tme.dwHoverTime = pMember->m_dwHoverTime;
		::TrackMouseEvent(&tme);

		CPoint pt(lParam);
		if (pMember->m_uLButtonDownHitTestRet == HTCAPTION)	// Drag move the windowless window
		{
			BOOL bSelected = GetKeyState(VK_LBUTTON) < 0;
			if (bSelected)
			{
				CRect rcOld;
				GetWindowRect(rcOld);
				GetParent()->ScreenToClient(rcOld);

				rcOld.OffsetRect(pt.x - pMember->m_ptLButtonDown.x, pt.y - pMember->m_ptLButtonDown.y);
				MoveWindow(rcOld, TRUE);
			}
		}
		else	// Drag resize the windowless window.
		{
			BOOL bSelected = GetKeyState(VK_LBUTTON) < 0;
			if (bSelected)
			{
				CRect rcOld;
				GetWindowRect(rcOld);
				GetParent()->ScreenToClient(rcOld);

				if ((pMember->m_uLButtonDownHitTestRet == HTLEFT) && (pMember->m_uCanDragResize & GRF_LEFT))
				{
					rcOld.left += (pt.x - pMember->m_ptLButtonDown.x);

					if (rcOld.left > rcOld.right)
					{
						rcOld.left = rcOld.right;
					}
				}
				else if ((pMember->m_uLButtonDownHitTestRet == HTTOPLEFT) && (pMember->m_uCanDragResize & GRF_TOPLEFT))
				{
					rcOld.left += (pt.x - pMember->m_ptLButtonDown.x);
					rcOld.top += (pt.y - pMember->m_ptLButtonDown.y);

					if (rcOld.left > rcOld.right)
					{
						rcOld.left = rcOld.right;
					}
					if (rcOld.top > rcOld.bottom)
					{
						rcOld.top = rcOld.bottom;
					}
				}
				else if ((pMember->m_uLButtonDownHitTestRet == HTTOP) && (pMember->m_uCanDragResize & GRF_TOP))
				{
					rcOld.top += (pt.y - pMember->m_ptLButtonDown.y);

					if (rcOld.top > rcOld.bottom)
					{
						rcOld.top = rcOld.bottom;
					}
				}
				else if ((pMember->m_uLButtonDownHitTestRet == HTTOPRIGHT) && (pMember->m_uCanDragResize & GRF_TOPRIGHT))
				{
					rcOld.top += (pt.y - pMember->m_ptLButtonDown.y);
					rcOld.right = pMember->m_rcLButtonDownRelateParent.right + (pt.x - pMember->m_ptLButtonDown.x);

					if (rcOld.top > rcOld.bottom)
					{
						rcOld.top = rcOld.bottom;
					}
					if (rcOld.right < rcOld.left)
					{
						rcOld.right = rcOld.left;
					}
				}
				else if ((pMember->m_uLButtonDownHitTestRet == HTRIGHT) && (pMember->m_uCanDragResize & GRF_RIGHT))
				{
					rcOld.right = pMember->m_rcLButtonDownRelateParent.right + (pt.x - pMember->m_ptLButtonDown.x);

					if (rcOld.right < rcOld.left)
					{
						rcOld.right = rcOld.left;
					}
				}
				else if ((pMember->m_uLButtonDownHitTestRet == HTBOTTOMRIGHT) && (pMember->m_uCanDragResize & GRF_BOTTOMRIGHT))
				{
					rcOld.right = pMember->m_rcLButtonDownRelateParent.right + (pt.x - pMember->m_ptLButtonDown.x);
					rcOld.bottom = pMember->m_rcLButtonDownRelateParent.bottom + (pt.y - pMember->m_ptLButtonDown.y);

					if (rcOld.right < rcOld.left)
					{
						rcOld.right = rcOld.left;
					}
					if (rcOld.bottom < rcOld.top)
					{
						rcOld.bottom = rcOld.top;
					}
				}
				else if ((pMember->m_uLButtonDownHitTestRet == HTBOTTOM) && (pMember->m_uCanDragResize & GRF_BOTTOM))
				{
					rcOld.bottom = pMember->m_rcLButtonDownRelateParent.bottom + (pt.y - pMember->m_ptLButtonDown.y);

					if (rcOld.bottom < rcOld.top)
					{
						rcOld.bottom = rcOld.top;
					}
				}
				else if ((pMember->m_uLButtonDownHitTestRet == HTBOTTOMLEFT) && (pMember->m_uCanDragResize & GRF_BOTTOMLEFT))
				{
					rcOld.left += (pt.x - pMember->m_ptLButtonDown.x);
					rcOld.bottom = pMember->m_rcLButtonDownRelateParent.bottom + (pt.y - pMember->m_ptLButtonDown.y);

					if (rcOld.left > rcOld.right)
					{
						rcOld.left = rcOld.right;
					}
					if (rcOld.bottom < rcOld.top)
					{
						rcOld.bottom = rcOld.top;
					}
				}

				MoveWindow(rcOld, TRUE);
			}
		}
	}

	return TRUE;
}

CPoint CWLWnd::GetLButtonDownPoint() const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	return pMember->m_ptLButtonDown;
}

UINT CWLWnd::GetLButtonDownHitTestResult() const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	return pMember->m_uLButtonDownHitTestRet;
}

int CWLWnd::GetLButtonDownRect(LPRECT lpRect) const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	if (lpRect == NULL)
	{
		return -1;
	}

	*lpRect = pMember->m_rcLButtonDownRelateParent;

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

CString CWLWnd::GetClassName() const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	return pMember->m_strClassName;
}

int CWLWnd::ShowWindow(int nCmdShow)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	pMember->m_bVisible = nCmdShow;

	//
	BOOL bShow = FALSE;
	if (nCmdShow == SW_SHOW
		|| nCmdShow == SW_SHOWDEFAULT
		|| nCmdShow == SW_SHOWMAXIMIZED
		|| nCmdShow == SW_SHOWMINIMIZED)
	{
		bShow = TRUE;
	}

	SendMessage(WM_WL_SHOWWINDOW, bShow, 0);

	//
	pMember->m_pParent->InvalidateRect(pMember->m_rcCurrent);

	return 0;
}

BOOL CWLWnd::IsWindowVisible() const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	return pMember->m_bVisible;
}

int CWLWnd::EnableWindow(BOOL bEnable)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	pMember->m_bEnabled = bEnable;

	pMember->m_pParent->InvalidateRect(pMember->m_rcCurrent);

	return 0;
}

BOOL CWLWnd::IsWindowEnabled() const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	return pMember->m_bEnabled;
}

int CWLWnd::SetWindowText(LPCTSTR lpszString)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	if (!IsCreated())
	{
		return -1;
	}

	pMember->m_strWindowName = lpszString;

	pMember->m_pParent->InvalidateRect(pMember->m_rcCurrent);

	return 0;
}

int CWLWnd::GetWindowText(LPTSTR lpszStringBuf, int nMaxCount) const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	if (lpszStringBuf == NULL)
	{
		return -1;
	}

	TSTRCPY(lpszStringBuf, nMaxCount, pMember->m_strWindowName);

	int nLen = pMember->m_strWindowName.GetLength();
	if (nLen >= nMaxCount)
	{
		return nMaxCount;
	}

	return nLen;
}

int CWLWnd::GetWindowText(CString &rString) const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	rString = pMember->m_strWindowName;

	return 0;
}

int CWLWnd::SetBkImage(LPCTSTR lpszImageName, BOOL bRedraw/* = TRUE*/)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	IUISetControlImage(&pMember->m_hBackground, lpszImageName);

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CWLWnd::GetBkImage(CString *pstrImageBk)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	if (pstrImageBk != NULL)
	{
		GetImageFileName(pMember->m_hBackground, pstrImageBk);
	}

	return 0;
}

int CWLWnd::SetRepeatPixel(const SIZE *psizeRepeat)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	if (psizeRepeat == NULL)
	{
		return -1;
	}

	pMember->m_sizeRepeat = *psizeRepeat;

	return 0;
}

int CWLWnd::GetRepeatPixel(SIZE *psizeRepeat) const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	if (psizeRepeat == NULL)
	{
		return -1;
	}

	*psizeRepeat = pMember->m_sizeRepeat;

	return 0;
}

BYTE CWLWnd::SetBkImgTransparent(BYTE btTransparent)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	BYTE btOld = m_btBkImgTransparent;

	m_btBkImgTransparent = btTransparent;

	return btOld;
}

BYTE CWLWnd::GetBkImgTransparent() const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	return m_btBkImgTransparent;
}

HWND CWLWnd::GetHostWnd()
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	return pMember->m_pParent->GetSafeHwnd();
}

CWnd *CWLWnd::GetParent() const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	return pMember->m_pParent;
}

CWLWnd *CWLWnd::SetCapture()
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	::SetCapture(GetParent()->GetSafeHwnd());

	_IUI_THREAD_STATE *pThreadState = IUIGetThreadState();
	CWLWnd *pOld = pThreadState->m_pWLCapture;
	pThreadState->m_pWLCapture = this;

	return pOld;
}

CWLWnd *CWLWnd::GetCapture()	// static
{
	_IUI_THREAD_STATE *pThreadState = IUIGetThreadState();
	return pThreadState->m_pWLCapture;
}

BOOL CWLWnd::ReleaseCapture()	// static
{
	_IUI_THREAD_STATE *pThreadState = IUIGetThreadState();
	pThreadState->m_pWLCapture = NULL;

	::ReleaseCapture();

	return TRUE;
}

int CWLWnd::GetOrigWindowRect(LPRECT lpRect) const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	if (lpRect == NULL)
	{
		return -1;
	}

	*lpRect = pMember->m_rcOrig;

	return 0;
}

int CWLWnd::SetOrigWindowRect(LPCRECT lpRect)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	if (lpRect == NULL)
	{
		return -1;
	}

	pMember->m_rcOrig = *lpRect;

	return 0;
}

int CWLWnd::GetWindowRect(LPRECT lpRect) const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	ASSERT(pMember->m_bCreated);
	*lpRect = pMember->m_rcCurrent;
	pMember->m_pParent->ClientToScreen(lpRect);

	return 0;
}

int CWLWnd::GetClientRect(LPRECT lpRect) const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	ASSERT(pMember->m_bCreated);

	if (lpRect == NULL)
	{
		return -1;
	}

	lpRect->left = 0;
	lpRect->top = 0;
	lpRect->right = pMember->m_rcCurrent.Width();
	lpRect->bottom = pMember->m_rcCurrent.Height();

	return 0;
}

int CWLWnd::ScreenToClient(LPPOINT lpPoint) const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	CRect rcWnd;
	GetWindowRect(rcWnd);

	lpPoint->x -= rcWnd.left;
	lpPoint->y -= rcWnd.top;

	return 0;
}

int CWLWnd::ScreenToClient(LPRECT lpRect) const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	CRect rcWnd;
	GetWindowRect(rcWnd);

	int nWidth = lpRect->right - lpRect->left;
	int nHeight = lpRect->bottom - lpRect->top;

	lpRect->left -= rcWnd.left;
	lpRect->top -= rcWnd.top;
	lpRect->right = lpRect->left + nWidth;
	lpRect->bottom = lpRect->top + nHeight;

	return 0;
}

int CWLWnd::ClientToScreen(LPPOINT lpPoint) const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	if (lpPoint == NULL)
	{
		return -1;
	}

	CRect rcWnd;
	GetWindowRect(rcWnd);

	lpPoint->x += rcWnd.left;
	lpPoint->y += rcWnd.top;

	return 0;
}

int CWLWnd::ClientToScreen(LPRECT lpRect) const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	if (lpRect == NULL)
	{
		return -1;
	}

	CRect rcWnd;
	GetWindowRect(rcWnd);

	CRect rc(*lpRect);
	rc.OffsetRect(rcWnd.TopLeft());
	*lpRect = rc;

	return 0;
}

int CWLWnd::GetDlgCtrlID() const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	return pMember->m_nID;
}

// MSDN: When this function is called, the system sends the WM_WINDOWPOSCHANGING and WM_WINDOWPOSCHANGED messages to the window
// See SetWindowRgn remark section in MSDN
int CWLWnd::SetWindowRgn(HRGN hRgn, BOOL bRedraw)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	// MSDN: The system does not make a copy of the region. Thus, you should not make any further function calls with this region handle.
	// In particular, do not delete this region handle. The system deletes the region handle when it no longer needed.

	if (pMember->m_hRgn != NULL)
	{
		DeleteObject(pMember->m_hRgn);
		pMember->m_hRgn = NULL;
	}

	pMember->m_hRgn = hRgn;

	if (bRedraw)
	{
		Invalidate();
	}

	return 1;	// If the function succeeds, the return value is nonzero. If the function fails, the return value is zero.
}

int CWLWnd::GetWindowRgn(HRGN hRgn) const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	if (hRgn == NULL)
	{
		return ERROR;
	}

	return ::CombineRgn(hRgn, pMember->m_hRgn, NULL, RGN_COPY);
}

int CWLWnd::SetVirtualParent(CWLWnd *pVirtualParent)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	pMember->m_pVirtualParent = pVirtualParent;

	return 0;
}

CWLWnd *CWLWnd::GetVirtualParent()
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	return pMember->m_pVirtualParent;
}

int CWLWnd::HitTest(CPoint pt, BOOL bSetCursor)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	// Disabled control refuse hit test.
	if (!IsWindowEnabled() || !IsWindowVisible())
	{
		return -1;
	}

	int nRet = HTCLIENT;

	CRect rcClient;
	GetClientRect(&rcClient);

	BOOL bSizingBorder = (pMember->m_dwStyle & WS_THICKFRAME);
	CRect rc;

	// left top
	rc.left = rc.top = 0;
	rc.right = rc.bottom = pMember->m_nCXBorder;
	if (bSizingBorder && PtInRect(&rc, pt) && (pMember->m_uCanDragResize & GRF_TOPLEFT))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
		}
		nRet = HTTOPLEFT;
		return nRet;
	}

	// top
	rc.left = pMember->m_nCXBorder + 1;
	rc.right = rcClient.right - pMember->m_nCXBorder - 1;
	rc.bottom = pMember->m_nCYBorder;
	if (bSizingBorder && PtInRect(&rc, pt) && (pMember->m_uCanDragResize & GRF_TOP))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
		}
		nRet = HTTOP;
		return nRet;
	}

	// right top
	rc.left = rcClient.right - pMember->m_nCXBorder;
	rc.right = rcClient.right;
	rc.bottom = pMember->m_nCXBorder;
	if (bSizingBorder && PtInRect(&rc, pt) && (pMember->m_uCanDragResize & GRF_TOPRIGHT))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENESW));
		}
		nRet = HTTOPRIGHT;
		return nRet;
	}

	// right
	rc.top = pMember->m_nCXBorder + 1;
	rc.bottom = rcClient.bottom - pMember->m_sizeScroll.cy - 1;
	rc.left = rcClient.right - pMember->m_nCYBorder;
	if (bSizingBorder && PtInRect(&rc, pt) && (pMember->m_uCanDragResize & GRF_RIGHT))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		}
		nRet = HTRIGHT;
		return nRet;
	}

	// right bottom
	rc.top = rcClient.bottom - pMember->m_sizeScroll.cy;
	rc.bottom = rcClient.bottom;
	rc.left = rcClient.right - pMember->m_sizeScroll.cx;
	if (bSizingBorder && PtInRect(&rc, pt) && (pMember->m_uCanDragResize & GRF_BOTTOMRIGHT))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
		}
		nRet = HTBOTTOMRIGHT;
		return nRet;
	}

	// bottom
	rc.left = pMember->m_nCXBorder + 1;
	rc.right = rcClient.right - pMember->m_sizeScroll.cx - 1;
	rc.top = rcClient.bottom - pMember->m_nCYBorder;
	if (bSizingBorder && PtInRect(&rc, pt) && (pMember->m_uCanDragResize & GRF_BOTTOM))
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
	rc.right = pMember->m_nCXBorder;
	rc.top = rcClient.bottom - pMember->m_nCXBorder;
	if (bSizingBorder && PtInRect(&rc, pt) && (pMember->m_uCanDragResize & GRF_BOTTOMLEFT))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENESW));
		}
		nRet = HTBOTTOMLEFT;
		return nRet;
	}

	// left
	rc.top = pMember->m_nCXBorder + 1;
	rc.bottom = rcClient.bottom - pMember->m_nCXBorder - 1;
	rc.right = pMember->m_nCYBorder;
	if (bSizingBorder && PtInRect(&rc, pt) && (pMember->m_uCanDragResize & GRF_LEFT))
	{
		if (bSetCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		}
		nRet = HTLEFT;
		return nRet;
	}

	// Title bar
	rc.top = pMember->m_nCYBorder;
	rc.left = pMember->m_nCXBorder;
	rc.right = rcClient.right - pMember->m_nCXBorder;
	if (pMember->m_nTitlebarHeight == -1)	// All client as title bar
	{
		rc.bottom = rcClient.bottom - pMember->m_nCYBorder;
	}
	else
	{
		rc.bottom = pMember->m_nTitlebarHeight;
	}

	if (PtInRect(&rc, pt))
	{
		nRet = HTCAPTION;
		return nRet;
	}

	if (bSetCursor)
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}

	return nRet;
}

int CWLWnd::SetBorderWidth(int nCXBorder, int nCYBorder)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	pMember->m_nCXBorder = nCXBorder;
	pMember->m_nCYBorder = nCYBorder;

	return 0;
}

int CWLWnd::GetBorderWidth(int *pnCXBorder, int *pnCYBorder)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	if (pnCXBorder != NULL)
	{
		*pnCXBorder = pMember->m_nCXBorder;
	}
	if (pnCYBorder != NULL)
	{
		*pnCYBorder = pMember->m_nCYBorder;
	}

	return 0;
}

int CWLWnd::SetTitlebarHeight(int nHeight)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	pMember->m_nTitlebarHeight = nHeight;

	return 0;
}

int CWLWnd::SetResizeFlag(UINT uFlag)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	pMember->m_uCanDragResize = uFlag;

	return 0;
}

UINT CWLWnd::GetResizeFlag() const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	return pMember->m_uCanDragResize;
}

int CWLWnd::SetNotify(BOOL bNotify)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	pMember->m_bNotify = bNotify;

	return 0;
}

BOOL CWLWnd::IsNotify() const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	return pMember->m_bNotify;
}

int CWLWnd::SetTag(LPVOID lpParam)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	pMember->m_lpParam = lpParam;

	return 0;
}

LPVOID CWLWnd::GetTag() const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	return pMember->m_lpParam;
}

int CWLWnd::MoveWindow(LPCRECT lpRect, BOOL bRepaint/* = TRUE*/)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	ASSERT(pMember->m_bCreated);

	CRect rcNew = *lpRect;

	// simulate WM_MOVING message
	if (pMember->m_rcCurrent.TopLeft() != rcNew.TopLeft())
	{
		WLWindowProc(WM_MOVING, 0, (LPARAM)(LPRECT)rcNew);

		if (pMember->m_bNotify)
		{
			NMHDR nmhdr;
			nmhdr.hwndFrom = NULL;
			nmhdr.idFrom = GetDlgCtrlID();
			nmhdr.code = WLN_MOVING;

			NMRECT nmRect;
			nmRect.hdr = nmhdr;
			nmRect.rect = rcNew;
			nmRect.lParam = (LPARAM)this;

			BOOL bRet = (BOOL)pMember->m_pParent->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmRect));
			if (bRet)
			{
				return 0;
			}

			rcNew = nmRect.rect;
		}
	}

	// simulate WM_SIZING message
	if (pMember->m_rcCurrent.Width() != rcNew.Width() || pMember->m_rcCurrent.Height() != rcNew.Height())
	{
		WLWindowProc(WM_SIZING, 0, (LPARAM)(LPRECT)rcNew);

		if (pMember->m_bNotify)
		{
			NMHDR nmhdr;
			nmhdr.hwndFrom = NULL;
			nmhdr.idFrom = GetDlgCtrlID();
			nmhdr.code = WLN_SIZING;

			NMRECT nmRect;
			nmRect.hdr = nmhdr;
			nmRect.rect = rcNew;
			nmRect.lParam = (LPARAM)this;

			BOOL bRet = (BOOL)pMember->m_pParent->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmRect));
			if (bRet)
			{
				return 0;
			}

			rcNew = nmRect.rect;
		}
	}

	// Refresh the old region.
	if (bRepaint)
	{
		Invalidate();
	}

	CRect rcOld = pMember->m_rcCurrent;
	pMember->m_rcCurrent = rcNew;

	// Refresh the new region.
	if (bRepaint)
	{
		Invalidate();
	}

	// simulate WM_MOVE message
	if (rcOld.TopLeft() != pMember->m_rcCurrent.TopLeft())
	{
		WLWindowProc(WM_MOVE, 0, MAKELPARAM(pMember->m_rcCurrent.left, pMember->m_rcCurrent.top));

		if (pMember->m_bNotify)
		{
			NMHDR nmhdr;
			nmhdr.hwndFrom = NULL;
			nmhdr.idFrom = GetDlgCtrlID();
			nmhdr.code = WLN_MOVE;

			NMRECT nmRect;
			nmRect.hdr = nmhdr;
			nmRect.rect = rcNew;
			nmRect.lParam = (LPARAM)this;

			BOOL bRet = (BOOL)pMember->m_pParent->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmRect));
			if (bRet)
			{
				return 0;
			}
		}
	}

	// simulate WM_SIZE message
	if (rcOld.Width() != pMember->m_rcCurrent.Width() || rcOld.Height() != pMember->m_rcCurrent.Height())
	{
		WLWindowProc(WM_SIZE, 0, MAKELPARAM(pMember->m_rcCurrent.Width(), pMember->m_rcCurrent.Height()));

		if (pMember->m_bNotify)
		{
			NMHDR nmhdr;
			nmhdr.hwndFrom = NULL;
			nmhdr.idFrom = GetDlgCtrlID();
			nmhdr.code = WLN_SIZE;

			NMRECT nmRect;
			nmRect.hdr = nmhdr;
			nmRect.rect = rcNew;
			nmRect.lParam = (LPARAM)this;

			BOOL bRet = (BOOL)pMember->m_pParent->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmRect));
			if (bRet)
			{
				return 0;
			}
		}
	}

	return 0;
}

int CWLWnd::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint/* = TRUE*/)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	CRect rect(x, y, x + nWidth, y + nHeight);

	return MoveWindow(rect, bRepaint);
}

BOOL CWLWnd::DestroyWindow()
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	if (!IsCreated())
	{
		return FALSE;
	}

	SendMessage(WM_WLDESTROY, 0, 0);

	CRect rcOld = pMember->m_rcCurrent;
	pMember->m_rcCurrent.SetRectEmpty();
	pMember->m_strWindowName.Empty();
	pMember->m_dwStyle = 0;
	pMember->m_nID = 0;

	// The parent may be free.
	HWND hParent = pMember->m_pParent->GetSafeHwnd();
	if (hParent != NULL && ::IsWindow(hParent))
	{
		pMember->m_pParent->InvalidateRect(rcOld);
		WLMgr *pWLMgr = (WLMgr *)::GetProp(hParent, GETPROP_WLMGR);

		// remove from parent's windowless control manager. the tree item bind windowless not
		// add to parent windowless list, so, pWLMgr may be NULL.
		if (pWLMgr != NULL)
		{
			int nCount = (int)pWLMgr->vpWLChildren.size();
			for (int i = 0; i < nCount; ++i)
			{
				if (pWLMgr->vpWLChildren[i] == this)
				{
					pWLMgr->vpWLChildren.erase(pWLMgr->vpWLChildren.begin() + i);
					break;
				}
			}
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	_IUI_THREAD_STATE *pThreadState = IUIGetThreadState();
	if (pThreadState->m_pWLMouseIn == this)
	{
		pThreadState->m_pWLMouseIn = NULL;
	}

	// Set to NOT create state.
	pMember->m_pParent = NULL;
	pMember->m_bCreated = FALSE;

	return TRUE;
}

int CWLWnd::SetDrawOffset(POINT ptOffset)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	pMember->m_ptDrawOffset = ptOffset;

	return 0;
}

int CWLWnd::GetDrawOffset(LPPOINT lpptOffset)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	if (lpptOffset == NULL)
	{
		return -1;
	}

	*lpptOffset = pMember->m_ptDrawOffset;

	return 0;
}

void CWLWnd::Invalidate(BOOL bErase/* = TRUE*/)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	pMember->m_pParent->InvalidateRect(pMember->m_rcCurrent);

	if (bErase)
	{
		// pMember->m_pParent->UpdateWindow();
	}
}

BOOL CWLWnd::IsOleControl() const
{
	return FALSE;
}

DWORD CWLWnd::SetStyle(DWORD dwStyle)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	DWORD dwOld = pMember->m_dwStyle;
	pMember->m_dwStyle = dwStyle;
	return dwOld;
}

DWORD CWLWnd::GetStyle() const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	return pMember->m_dwStyle;
}

DWORD CWLWnd::AddStyle(DWORD dwAddStyle)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	DWORD dwOld = pMember->m_dwStyle;
	pMember->m_dwStyle |= dwAddStyle;
	return dwOld;

}

DWORD CWLWnd::RemoveStyle(DWORD dwRemoveStyle)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	DWORD dwOld = pMember->m_dwStyle;
	pMember->m_dwStyle &= ~dwRemoveStyle;
	return dwOld;
}

LRESULT CWLWnd::SendMessage(UINT message, WPARAM wParam/* = 0*/, LPARAM lParam/* = 0*/)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	return WLWindowProc(message, wParam, lParam);
}

LRESULT CWLWnd::SendMessage(UINT message, WPARAM wParam/* = 0*/, LPARAM lParam/* = 0*/) const
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	CWLWnd *pThis = (CWLWnd *)this;
	return pThis->WLWindowProc(message, wParam, lParam);
}

UINT_PTR CWLWnd::SetTimer(UINT_PTR nIDEvent, UINT nElapse, void (CALLBACK *lpfnTimer)(HWND, UINT, UINT_PTR, DWORD))
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	return ::SetTimer(pMember->m_pParent->GetSafeHwnd(), nIDEvent, nElapse, lpfnTimer);
}

BOOL CWLWnd::KillTimer(UINT_PTR uIDEvent)
{
	WLWNDMEMBER *pMember = (WLWNDMEMBER *)m_pMember;

	return ::KillTimer(pMember->m_pParent->GetSafeHwnd(), uIDEvent);
}
