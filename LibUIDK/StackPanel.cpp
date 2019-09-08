// StackPanel.cpp: implementation of the CStackPanel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <map>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStackPanel::CStackPanel()
{
	m_eOrientation = SPO_LEFT;
	memset(&m_si, 0, sizeof(SCROLLINFO));
	m_si.cbSize = sizeof(SCROLLINFO);
	m_bLastChildFill = FALSE;
}

CStackPanel::~CStackPanel()
{

}

//////////////////////////////////////////////////////////////////////////
// protected

LRESULT CStackPanel::ElementWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC wndprocOld = CWndProcList::GetOldWndProc(hwnd, (WNDPROC)ElementWndProc);
	if (wndprocOld == NULL)
	{
		return 0;
	}

	if (uMsg == WM_DESTROY)
	{
		CStackPanel *pStackPanel = (CStackPanel *)GetProp(hwnd, GETPROP_PANEL);
		if (pStackPanel != NULL)
		{
			pStackPanel->RemoveElement(CWnd::FromHandle(hwnd));
		}
	}
	else if (uMsg == WM_NCDESTROY)	// the last message for a window
	{
		RemoveProp(hwnd, GETPROP_PANEL);
	}
	// Handle window show or hide, Don't handle WM_SHOWWINDOW, because the state not changed call IsWindowVisible in WM_SHOWWINDOW
	// After the element show or hide, re-layout the elements.
	// Hide or show a window has tree ways:
	// 1. Call ShowWindow
	// 2. Call SetWindowPos
	// 3. Call SetWindowLong to remove WS_VISIBLE style
	// ShowWindow and SetWindowPos can send WM_WINDOWPOSCHANGED message(spy++)
	// SetWindowLong can send WM_STYLECHANGED message(spy++)
	else if (uMsg == WM_WINDOWPOSCHANGED)
	{
		WINDOWPOS *pwp = (WINDOWPOS *)lParam;
		BOOL bShow = ((pwp->flags & SWP_SHOWWINDOW) == SWP_SHOWWINDOW) ? TRUE : FALSE;
		BOOL bHide = ((pwp->flags & SWP_HIDEWINDOW) == SWP_HIDEWINDOW) ? TRUE : FALSE;
		if (bShow != bHide)
		{
			CStackPanel *pStackPanel = (CStackPanel *)GetProp(hwnd, GETPROP_PANEL);
			if (pStackPanel != NULL)
			{
				pStackPanel->RelayoutElements();
			}
		}
	}
	else if (uMsg == WM_STYLECHANGED)
	{
		// Modify style
		if ((wParam & GWL_STYLE) == GWL_STYLE)
		{
			STYLESTRUCT *pss = (STYLESTRUCT *)lParam;
			BOOL bVisiblePre = ((pss->styleOld & WS_VISIBLE) == WS_VISIBLE) ? TRUE : FALSE;
			BOOL bVisibleNew = ((pss->styleNew & WS_VISIBLE) == WS_VISIBLE) ? TRUE : FALSE;

			if (bVisiblePre != bVisibleNew)
			{
				CStackPanel *pStackPanel = (CStackPanel *)GetProp(hwnd, GETPROP_PANEL);
				if (pStackPanel != NULL)
				{
					pStackPanel->RelayoutElements();
				}
			}
		}
	}

	return CallWindowProc(wndprocOld, hwnd, uMsg, wParam, lParam);
}

int CStackPanel::BindStyle(LPCTSTR lpszStyleID)
{
	if (!IsCreated())
	{
		ASSERT(FALSE);
		return -1;
	}

	if (lpszStyleID == NULL)
	{
		return -2;
	}

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_PANEL_STACK, lpszStyleID);
	return BindStyle(pCtrlProp);
}

int CStackPanel::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	if (!IsCreated())
	{
		ASSERT(FALSE);
		return -1;
	}

	if (pCtrlProp == NULL)
	{
		return -2;
	}

	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (ect != CT_STYLE_PANEL_STACK && ect != CT_PNL_STACK)
	{
		return -3;
	}

	STACKPNLPROPERTIES *pPnlStackProp = (STACKPNLPROPERTIES *)pCtrlProp;
	SetOrientation(STACK_PANEL_ORIENTATION(pPnlStackProp->m_eOrientation));

	return 0;
}

int CStackPanel::ReleaseObject()
{
	size_t nCount = m_vElements.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		STACK_ELEMENT *pElement = &m_vElements[i];

		if (pElement->m_ePet == PET_PANEL)
		{
			continue;
		}

		CWnd *pWnd = (CWnd *)pElement->m_pElement;

		RemoveProp(pWnd->GetSafeHwnd(), GETPROP_PANEL);
	}

	m_vElements.clear();

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// virtual

int CStackPanel::RelayoutElements(BOOL bRedraw/* = FALSE*/)
{
	ASSERT(GetParent() != NULL);

	CRect rect;
	GetWindowRect(rect);
	GetParent()->ScreenToClient(rect);

	int nOffset = -m_si.nPos;
	if (m_eOrientation == SPO_RIGHT)
	{
		nOffset += rect.right;
	}
	// update scroll info.
	if (m_eOrientation == SPO_LEFT || m_eOrientation == SPO_RIGHT)
	{
		if (m_si.nMax + 1 <= rect.Width())
		{
			m_si.nPage = m_si.nMax + 1;
		}
		else
		{
			m_si.nPage = rect.Width();
		}
	}
	else if (m_eOrientation == SPO_TOP || m_eOrientation == SPO_BOTTOM)
	{
		if (m_si.nMax + 1 <= rect.Height())
		{
			m_si.nPage = m_si.nMax + 1;
		}
		else
		{
			m_si.nPage = rect.Height();
		}
	}

	// Get the total element span.
	int nTotalElementsSpan = 0;
	size_t i = 0;

	size_t nCount = m_vElements.size();
	for (i = 0; i < nCount ; i++)
	{
		STACK_ELEMENT *pElement = &m_vElements[i];
		CRect rcWnd(0, 0, 0, 0);
		if (pElement->m_ePet == PET_WINDOW)
		{
			CWnd *pWnd = (CWnd *)pElement->m_pElement;
			if (!IsWindowVisibleEx(pWnd->GetSafeHwnd()))
			{
				continue;
			}

			pWnd->GetWindowRect(rcWnd);
			GetParent()->ScreenToClient(rcWnd);
		}
		else if (pElement->m_ePet == PET_PANEL)
		{
			CPanel *pPanel = (CPanel *)pElement->m_pElement;
			pPanel->GetWindowRect(rcWnd);
			GetParent()->ScreenToClient(rcWnd);
		}
		else
		{
			ASSERT(FALSE);
		}

		if (m_eOrientation == SPO_CENTER)
		{
			nTotalElementsSpan += rcWnd.Width();
		}
		else if (m_eOrientation == SPO_VCENTER)
		{
			nTotalElementsSpan += rcWnd.Height();
		}
	}

	// Get the space
	int nSpace = 0;
	if (m_eOrientation == SPO_CENTER)
	{
		nSpace = (rect.Width() - nTotalElementsSpan) / 2;
	}
	else if (m_eOrientation == SPO_VCENTER)
	{
		nSpace = (rect.Height() - nTotalElementsSpan) / 2;
	}

	// Layout the elements.
	for (i = 0; i < nCount ; i++)
	{
		STACK_ELEMENT *pElement = &m_vElements[i];

		CRect rcWnd;
		if (pElement->m_ePet == PET_WINDOW)
		{
			CWnd *pWnd = (CWnd *)pElement->m_pElement;
			if (!IsWindowVisibleEx(pWnd->GetSafeHwnd()))
			{
				continue;
			}

			pWnd->GetWindowRect(rcWnd);
			GetParent()->ScreenToClient(rcWnd);
		}
		else if (pElement->m_ePet == PET_PANEL)
		{
			CPanel *pPanel = (CPanel *)pElement->m_pElement;
			pPanel->GetWindowRect(rcWnd);
			GetParent()->ScreenToClient(rcWnd);
		}

		if (m_eOrientation == SPO_LEFT)
		{
			if (nOffset == 0)
			{
				nOffset += rect.left;
			}

			int nWidth = rcWnd.Width();
			rcWnd.left = nOffset;
			if (i == nCount - 1)
			{
				// The last one is fill mode
				if (m_bLastChildFill)
				{
					rcWnd.right = rect.right;
				}
				else
				{
					rcWnd.right = rcWnd.left + nWidth;
				}
			}
			else
			{
				rcWnd.right = rcWnd.left + nWidth;
			}
			rcWnd.top = rect.top;
			rcWnd.bottom = rect.bottom;

			nOffset += nWidth;

			// Move the element
			if (pElement->m_ePet == PET_WINDOW)
			{
				CWnd *pWnd = (CWnd *)pElement->m_pElement;
				pWnd->MoveWindow(rcWnd);

				// if the control outside of the stack panel, hide it.
				if (rcWnd.right > rect.right)
				{
					pWnd->ShowWindow(SW_HIDE);
				}
				else
				{
					pWnd->ShowWindow(SW_SHOW);
				}
			}
			else if (pElement->m_ePet == PET_PANEL)
			{
				CPanel *pPanel = (CPanel *)pElement->m_pElement;
				pPanel->MoveWindow(rcWnd);
				pPanel->RelayoutElements();
			}
		}
		else if (m_eOrientation == SPO_CENTER)
		{
			if (nOffset == 0)
			{
				nOffset = rect.left + nSpace;
			}

			int nWidth = rcWnd.Width();
			rcWnd.left = nOffset;
			rcWnd.right = rcWnd.left + nWidth;

			nOffset += nWidth;

			if (pElement->m_ePet == PET_WINDOW)
			{
				CWnd *pWnd = (CWnd *)pElement->m_pElement;
				pWnd->MoveWindow(rcWnd);

				// if the control outside of the stack panel, hide it.
				if (rcWnd.right > rect.right)
				{
					pWnd->ShowWindow(SW_HIDE);
				}
				else
				{
					pWnd->ShowWindow(SW_SHOW);
				}
			}
		}
		else if (m_eOrientation == SPO_RIGHT)
		{
			int nWidth = rcWnd.Width();

			nOffset -= nWidth;

			if (i == nCount - 1)
			{
				// The last one is fill mode
				if (m_bLastChildFill)
				{
					rcWnd.left = rect.left;
					rcWnd.right = nOffset + nWidth;
				}
				else
				{
					rcWnd.left = nOffset;
					rcWnd.right = rcWnd.left + nWidth;
				}
			}
			else
			{
				rcWnd.left = nOffset;
				rcWnd.right = rcWnd.left + nWidth;
			}
			rcWnd.top = rect.top;
			rcWnd.bottom = rect.bottom;

			if (pElement->m_ePet == PET_WINDOW)
			{
				CWnd *pWnd = (CWnd *)pElement->m_pElement;
				pWnd->MoveWindow(rcWnd);
				if (bRedraw)
				{
					pWnd->UpdateWindow();
				}

				// if the control outside of the stack panel, hide it.
				if (rcWnd.right > rect.right)
				{
					pWnd->ShowWindow(SW_HIDE);
				}
				else
				{
					pWnd->ShowWindow(SW_SHOW);
				}
			}
		}
		else if (m_eOrientation == SPO_TOP)
		{
			if (nOffset == 0)
			{
				nOffset += rect.top;
			}

			int nHeight = rcWnd.Height();
			rcWnd.top = nOffset;
			if (i == nCount - 1)
			{
				// The last one is fill mode
				if (m_bLastChildFill)
				{
					rcWnd.bottom = rect.bottom;
				}
				else
				{
					rcWnd.bottom = rcWnd.top + nHeight;
				}
			}
			else
			{
				rcWnd.bottom = rcWnd.top + nHeight;
			}
			rcWnd.left = rect.left;
			rcWnd.right = rect.right;

			nOffset += nHeight;

			// Move the element
			if (pElement->m_ePet == PET_WINDOW)
			{
				CWnd *pWnd = (CWnd *)pElement->m_pElement;
				pWnd->MoveWindow(rcWnd);
			}
			else if (pElement->m_ePet == PET_PANEL)
			{
				CPanel *pPanel = (CPanel *)pElement->m_pElement;
				pPanel->MoveWindow(rcWnd);
				pPanel->RelayoutElements();
			}
		}
	}

	return 0;
}

int CStackPanel::MoveWindow(LPCRECT lpRect, BOOL bRepaint/* = TRUE*/)
{
	CPanel::MoveWindow(lpRect, bRepaint);

	return RelayoutElements(bRepaint);
}

//////////////////////////////////////////////////////////////////////////
// public

int CStackPanel::SetOrientation(STACK_PANEL_ORIENTATION eOrientation)
{
	m_eOrientation = eOrientation;

	return 0;
}

int CStackPanel::SetLastChildFill(BOOL bFill)
{
	m_bLastChildFill = bFill;

	return 0;
}

BOOL CStackPanel::IsLastChildFill() const
{
	return m_bLastChildFill;
}

int CStackPanel::AddElement(CWnd *pWnd, BOOL bLayout/* = TRUE*/)
{
	ASSERT(pWnd->GetParent() == GetParent());

	// The pWnd can only add to one Panel.
	CStackPanel *pStackPanel = (CStackPanel *)GetProp(pWnd->GetSafeHwnd(), GETPROP_PANEL);
	if (pStackPanel != NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	WNDPROC procOld = (WNDPROC)IUISetWindowLong(pWnd->GetSafeHwnd(), GWLP_WNDPROC, (LONG_PTR)ElementWndProc);
	SetProp(pWnd->GetSafeHwnd(), GETPROP_PANEL, this);
	CWndProcList::AddToMap(pWnd->GetSafeHwnd(), procOld, (WNDPROC)ElementWndProc);

	STACK_ELEMENT se;
	se.m_ePet = PET_WINDOW;
	se.m_pElement = pWnd;
	m_vElements.push_back(se);

	// layout elements.
	if (bLayout)
	{
		RelayoutElements();
	}

	// update scroll info
	CRect rcWnd;
	pWnd->GetWindowRect(rcWnd);
	if (m_eOrientation == SPO_LEFT || m_eOrientation == SPO_RIGHT)
	{
		m_si.nMax += rcWnd.Width();
	}
	else if (m_eOrientation == SPO_TOP || m_eOrientation == SPO_BOTTOM)
	{
		m_si.nMax += rcWnd.Height();
	}

	size_t nSize = m_vElements.size();
	return int(nSize - 1);
}

int CStackPanel::AddElement(CPanel *pPanel)
{
	STACK_ELEMENT se;
	se.m_ePet = PET_PANEL;
	se.m_pElement = pPanel;
	m_vElements.push_back(se);

	// layout elements.
	RelayoutElements();

	// update scroll info
	CRect rcWnd;
	pPanel->GetWindowRect(rcWnd);
	GetParent()->ScreenToClient(rcWnd);
	if (m_eOrientation == SPO_LEFT || m_eOrientation == SPO_RIGHT)
	{
		m_si.nMax += rcWnd.Width();
	}
	else if (m_eOrientation == SPO_TOP || m_eOrientation == SPO_BOTTOM)
	{
		m_si.nMax += rcWnd.Height();
	}

	size_t nSize = m_vElements.size();
	return int(nSize - 1);
}

CUIWnd *CStackPanel::AddElement(UINT uWinID, DWORD dwStyle)
{
	// Create the window
	CUIWnd *pUIWnd = (CUIWnd *)GetParent();
	if (!pUIWnd->SendMessage(WM_ISUIWND))
	{
		return NULL;
	}

	CUIWnd *pWndElement = CUIWnd::CreateChildWindow(NULL, uWinID, dwStyle, pUIWnd, NULL, 0);
	if (pWndElement == NULL)
	{
		return NULL;
	}

	STACK_ELEMENT se;
	se.m_ePet = PET_WINDOW;
	se.m_pElement = pWndElement;
	m_vElements.push_back(se);

	// layout elements.
	RelayoutElements();

	//// update scroll info
	//CRect rcWnd;
	//pPanel->GetWindowRect(rcWnd);
	//GetParent()->ScreenToClient(rcWnd);
	//if (m_eOrientation == SPO_LEFT || m_eOrientation == SPO_RIGHT)
	//	m_si.nMax += rcWnd.Width();
	//else if (m_eOrientation == SPO_TOP || m_eOrientation == SPO_BOTTOM)
	//	m_si.nMax += rcWnd.Height();

	return pWndElement;
}

int CStackPanel::RemoveElement(int nIndex)
{
	int nCount = (int)m_vElements.size();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return -1;
	}

	m_vElements.erase(m_vElements.begin() + nIndex);

	RelayoutElements();

	return 0;
}

int CStackPanel::RemoveElement(CWnd *pWnd)
{
	size_t nRet = -1;
	size_t nCount = m_vElements.size();
	for (size_t i = 0; i < nCount ; i++)
	{
		STACK_ELEMENT *pElement = &m_vElements[i];

		if (pElement->m_ePet == PET_PANEL)
		{
			continue;
		}

		if (pElement->m_pElement == pWnd)
		{
			nRet = i;
			m_vElements.erase(m_vElements.begin() + i);
			break;
		}
	}

	RelayoutElements();

	return (int)nRet;
}

int CStackPanel::MoveElementTo(STACK_ELEMENT *pWndFrom, int nNewIndex)
{
	//
	STACK_ELEMENT seFrom;
	int nCount = (int)m_vElements.size();
	for (int i = 0; i < nCount ; i++)
	{
		STACK_ELEMENT *pElement = &m_vElements[i];

		if (pElement->m_pElement == pWndFrom->m_pElement)
		{
			seFrom = *pElement;
			m_vElements.erase(m_vElements.begin() + i);
			break;
		}
	}

	//
	nCount = (int)m_vElements.size();
	if (nNewIndex < nCount)
	{
		m_vElements.insert(m_vElements.begin() + nNewIndex, seFrom);
	}
	else
	{
		m_vElements.push_back(seFrom);
	}

	RelayoutElements();

	return 0;
}

int CStackPanel::MoveElementTo(STACK_ELEMENT *pWndFrom, STACK_ELEMENT *pWndLast)
{
	//
	STACK_ELEMENT seFrom;
	int nCount = (int)m_vElements.size();
	int i = 0;
	for (i = 0; i < nCount ; i++)
	{
		STACK_ELEMENT *pElement = &m_vElements[i];

		if (pElement->m_pElement == pWndFrom->m_pElement)
		{
			seFrom = *pElement;
			m_vElements.erase(m_vElements.begin() + i);
			break;
		}
	}

	//
	nCount = (int)m_vElements.size();
	for (i = 0; i < nCount ; i++)
	{
		STACK_ELEMENT *pElement = &m_vElements[i];

		if (pElement->m_pElement == pWndLast->m_pElement)
		{
			m_vElements.insert(m_vElements.begin() + i + 1, seFrom);
			break;
		}
	}

	RelayoutElements();

	return 0;
}

int CStackPanel::GetElementCount()
{
	size_t nCount = m_vElements.size();

	return (int)nCount;
}

int CStackPanel::RemoveAllElements()
{
	m_vElements.clear();

	RelayoutElements();

	return 0;
}

int CStackPanel::EnsureVisible(int nObjID, BOOL bPartialOK)
{
	CWnd *pWnd = GetParent()->GetDlgItem(nObjID);
	return EnsureVisible(pWnd, bPartialOK);
}

int CStackPanel::EnsureVisible(CWnd *pWndObj, BOOL bPartialOK)
{
	if (pWndObj == NULL)
	{
		return -1;
	}

	int nOffset = 0;
	int nWidth1 = 0;	// width of left part of pWndObj
	int nWidth2 = 0;	// width of pWndObj
	int nWidth3 = 0;	// width of right part of pWndObj
	BOOL bWidth2Init = FALSE;

	size_t nCount = m_vElements.size();
	for (size_t i = 0; i < nCount ; i++)
	{
		STACK_ELEMENT *pElement = &m_vElements[i];

		CRect rcWnd;
		if (pElement->m_ePet == PET_WINDOW)
		{
			CWnd *pWnd = (CWnd *)pElement->m_pElement;
			pWnd->GetWindowRect(rcWnd);

			if (pWnd->GetSafeHwnd() == pWndObj->GetSafeHwnd())
			{
				if (m_eOrientation == SPO_LEFT || m_eOrientation == SPO_RIGHT)
				{
					nWidth2 = rcWnd.Width();
				}
				else if (m_eOrientation == SPO_TOP || m_eOrientation == SPO_BOTTOM)
				{
					nWidth2 = rcWnd.Height();
				}
				bWidth2Init = TRUE;
				continue;
			}
		}
		else
		{
			ASSERT(FALSE);
		}

		if (bWidth2Init)
		{
			if (m_eOrientation == SPO_LEFT || m_eOrientation == SPO_RIGHT)
			{
				nWidth3 += rcWnd.Width();
			}
			else if (m_eOrientation == SPO_TOP || m_eOrientation == SPO_BOTTOM)
			{
				nWidth3 += rcWnd.Height();
			}
		}
		else
		{
			if (m_eOrientation == SPO_LEFT || m_eOrientation == SPO_RIGHT)
			{
				nWidth1 += rcWnd.Width();
			}
			else if (m_eOrientation == SPO_TOP || m_eOrientation == SPO_BOTTOM)
			{
				nWidth1 += rcWnd.Height();
			}
		}
	}

	ASSERT(GetParent() != NULL);

	CRect rect;
	GetWindowRect(rect);
	GetParent()->ScreenToClient(rect);

	int nWidth = 0;
	if (m_eOrientation == SPO_LEFT || m_eOrientation == SPO_RIGHT)
	{
		nWidth = rect.Width();
	}
	else if (m_eOrientation == SPO_TOP || m_eOrientation == SPO_BOTTOM)
	{
		nWidth = rect.Height();
	}

	if (nWidth3 + nWidth2 >= nWidth)
	{
		m_si.nPos = nWidth1;
	}
	else if (nWidth1 + nWidth2 + nWidth3 > nWidth)
	{
		m_si.nPos = nWidth1 + nWidth2 + nWidth3 - nWidth;
	}
	else if (nWidth1 + nWidth2 + nWidth3 <= nWidth)
	{
		m_si.nPos = 0;
	}

	RelayoutElements();

	return 0;
}
