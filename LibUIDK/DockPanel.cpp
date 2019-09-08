// DockPanel.cpp: implementation of the CDockPanel class.
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

CDockPanel::CDockPanel()
{
	m_bLastChildFill = FALSE;
}

CDockPanel::~CDockPanel()
{

}

//////////////////////////////////////////////////////////////////////////
// protected

LRESULT CDockPanel::ElementWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC wndprocOld = CWndProcList::GetOldWndProc(hwnd, (WNDPROC)ElementWndProc);
	if (wndprocOld == NULL)
	{
		return 0;
	}

	if (uMsg == WM_DESTROY)
	{
		CDockPanel *pDockPanel = (CDockPanel *)GetProp(hwnd, GETPROP_PANEL);
		if (pDockPanel != NULL)
		{
			pDockPanel->RemoveElement(CWnd::FromHandle(hwnd));
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

		// If hook windows size change message, the pDockPanel->RelayoutElements() can send WM_WINDOWPOSCHANGED also.
		// So, add an interface CDockPanel::SetElementSize.
		// BOOL bSizeChanged = (((pwp->flags & SWP_NOZORDER) == SWP_NOZORDER) && ((pwp->flags & SWP_NOACTIVATE) == SWP_NOACTIVATE)) ? TRUE : FALSE;	// Handle the window's size changed.

		if ((bShow != bHide)/* || bSizeChanged*/)
		{
			CDockPanel *pDockPanel = (CDockPanel *)GetProp(hwnd, GETPROP_PANEL);
			if (pDockPanel != NULL)
			{
				pDockPanel->RelayoutElements();
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
				CDockPanel *pDockPanel = (CDockPanel *)GetProp(hwnd, GETPROP_PANEL);
				if (pDockPanel != NULL)
				{
					pDockPanel->RelayoutElements();
				}
			}
		}
	}

	return CallWindowProc(wndprocOld, hwnd, uMsg, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
// virtual

int CDockPanel::ReleaseObject()
{
	size_t nCount = m_vElements.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		DOCK_ELEMENT *pElement = &m_vElements[i];

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

int CDockPanel::RelayoutElements(BOOL bRedraw/* = FALSE*/)
{
	ASSERT(GetParent() != NULL);

	CRect rect;
	GetWindowRect(rect);
	GetParent()->ScreenToClient(rect);

	CRect rcRemain = rect;	// The remain area after dock the previous element.

	size_t nElementCount = m_vElements.size();

	for (size_t i = 0; i < nElementCount; ++i)
	{
		DOCK_ELEMENT *pItem = &m_vElements[i];

		DOCK_STYLE eDockStyle = pItem->m_eDockStyle;

		if (i == nElementCount - 1 && m_bLastChildFill) // Ignore the dock style
		{
			if (pItem->m_ePet == PET_WINDOW)
			{
				CWnd *pWnd = (CWnd *)pItem->m_pElement;

				// Skip the invisible element. The IsWindowVisible can't get the visible state at WM_CREATE message of parent, so use IsWindowVisibleEx(GetWindowLong) instead.
				if (!IsWindowVisibleEx(pWnd->GetSafeHwnd()))
				{
					continue;
				}

				pWnd->MoveWindow(rcRemain);
				rcRemain.SetRectEmpty();
			}
			else if (pItem->m_ePet == PET_PANEL)
			{
				CPanel *pPanel = (CPanel *)pItem->m_pElement;

				if (!pPanel->IsWindowVisible())
				{
					continue;
				}

				pPanel->MoveWindow(rcRemain);
				rcRemain.SetRectEmpty();
			}
			else if (pItem->m_ePet == PET_WINDOWLESS)
			{
				CWLWnd *pWLWnd = (CWLWnd *)pItem->m_pElement;

				if (!pWLWnd->IsWindowVisible())
				{
					continue;
				}

				pWLWnd->MoveWindow(rcRemain);
				rcRemain.SetRectEmpty();
			}
			else
			{
				ASSERT(FALSE);
			}
		}
		else
		{
			if (pItem->m_ePet == PET_WINDOW)
			{
				CWnd *pWnd = (CWnd *)pItem->m_pElement;

				// Skip the invisible element.
				if (!IsWindowVisibleEx(pWnd->GetSafeHwnd()))
				{
					continue;
				}

				CRect rect;
				pWnd->GetWindowRect(rect);
				GetParent()->ScreenToClient(rect);

				// Dock left keep the width of the element
				if (pItem->m_eDockStyle == DS_LEFT)
				{
					int nWidth = rect.Width();
					CRect rcNew = rcRemain;

					rcNew.right = rcNew.left + nWidth;
					rcRemain.left += nWidth;

					pWnd->MoveWindow(rcNew);
				}
				// Dock top keep the height of the element
				else if (pItem->m_eDockStyle == DS_TOP)
				{
					int nHeight = rect.Height();
					CRect rcNew = rcRemain;
					rcNew.bottom = rcNew.top + nHeight;
					rcRemain.top += nHeight;

					pWnd->MoveWindow(rcNew);
				}
				// Dock right keep the width of the element
				else if (pItem->m_eDockStyle == DS_RIGHT)
				{
					int nWidth = rect.Width();
					CRect rcNew = rcRemain;
					rcNew.left = rcNew.right - nWidth;
					rcRemain.right -= nWidth;

					pWnd->MoveWindow(rcNew);
				}
				// Dock bottom keep the height of the element
				else if (pItem->m_eDockStyle == DS_BOTTOM)
				{
					int nHeight = rect.Height();
					CRect rcNew = rcRemain;
					rcNew.top = rcNew.bottom - nHeight;
					rcRemain.bottom -= nHeight;

					pWnd->MoveWindow(rcNew);
				}
				// Dock fill will fill all of the area, the lost elements will be hide.
				else if (pItem->m_eDockStyle == DS_FILL)
				{
					pWnd->MoveWindow(rcRemain);
					rcRemain.SetRectEmpty();

					// Hide all of the lost elements.
					for (i++; i < nElementCount; ++i)
					{
						DOCK_ELEMENT *pItem = &m_vElements[i];
						if (pItem->m_ePet == PET_WINDOW)
						{
							CWnd *pWnd = (CWnd *)pItem->m_pElement;

							pWnd->ShowWindow(SW_HIDE);
						}
						else if (pItem->m_ePet == PET_PANEL)
						{
							CPanel *pPanel = (CPanel *)pItem->m_pElement;

							pPanel->ShowWindow(SW_HIDE);
						}
						else
						{
							ASSERT(FALSE);
						}
					}

					break;
				}
				else
				{
					ASSERT(FALSE);
				}
			}
			else if (pItem->m_ePet == PET_PANEL)
			{
				CPanel *pPanel = (CPanel *)pItem->m_pElement;

				// Skip the invisible element.
				if (!pPanel->IsWindowVisible())
				{
					continue;
				}

				CRect rect;
				pPanel->GetWindowRect(rect);
				GetParent()->ScreenToClient(rect);

				// Dock left keep the width of the element
				if (pItem->m_eDockStyle == DS_LEFT)
				{
					int nWidth = rect.Width();
					CRect rcNew = rcRemain;

					rcNew.right = rcNew.left + nWidth;
					rcRemain.left += nWidth;

					pPanel->MoveWindow(rcNew);
				}
				// Dock top keep the height of the element
				else if (pItem->m_eDockStyle == DS_TOP)
				{
					int nHeight = rect.Height();
					CRect rcNew = rcRemain;
					rcNew.bottom = rcNew.top + nHeight;
					rcRemain.top += nHeight;

					pPanel->MoveWindow(rcNew);
				}
				// Dock right keep the width of the element
				else if (pItem->m_eDockStyle == DS_RIGHT)
				{
					int nWidth = rect.Width();
					CRect rcNew = rcRemain;
					rcNew.left = rcNew.right - nWidth;
					rcRemain.right -= nWidth;

					pPanel->MoveWindow(rcNew);
				}
				// Dock bottom keep the height of the element
				else if (pItem->m_eDockStyle == DS_BOTTOM)
				{
					int nHeight = rect.Height();
					CRect rcNew = rcRemain;
					rcNew.top = rcNew.bottom - nHeight;
					rcRemain.bottom -= nHeight;

					pPanel->MoveWindow(rcNew);
				}
				// Dock fill will fill all of the area, the lost elements will be hide.
				else if (pItem->m_eDockStyle == DS_FILL)
				{
					pPanel->MoveWindow(rcRemain);
					rcRemain.SetRectEmpty();

					// Hide all of the lost elements.
					for (i++; i < nElementCount; ++i)
					{
						DOCK_ELEMENT *pItem = &m_vElements[i];
						if (pItem->m_ePet == PET_WINDOW)
						{
							CWnd *pWnd = (CWnd *)pItem->m_pElement;

							pWnd->ShowWindow(SW_HIDE);
						}
						else if (pItem->m_ePet == PET_PANEL)
						{
							CPanel *pPanel = (CPanel *)pItem->m_pElement;

							pPanel->ShowWindow(SW_HIDE);
						}
						else
						{
							ASSERT(FALSE);
						}
					}

					break;
				}
				else
				{
					ASSERT(FALSE);
				}
			}
			else
			{
				ASSERT(FALSE);
			}
		}
	}

	return 0;
}

int CDockPanel::MoveWindow(LPCRECT lpRect, BOOL bRepaint/* = TRUE*/)
{
	CPanel::MoveWindow(lpRect, bRepaint);

	return RelayoutElements(bRepaint);
}

//////////////////////////////////////////////////////////////////////////
// public

int CDockPanel::SetLastChildFill(BOOL bFill)
{
	m_bLastChildFill = bFill;

	RelayoutElements(TRUE);

	return 0;
}

BOOL CDockPanel::IsLastChildFill() const
{
	return m_bLastChildFill;
}

int CDockPanel::AddElement(CWnd *pWnd, DOCK_STYLE eDockStyle, BOOL bLayout/* = TRUE*/)
{
	ASSERT(pWnd->GetParent() == GetParent());

	// If the object already exist.
	int nRet = FindElement(pWnd, NULL);
	if (nRet == 0)	// Found
	{
		return 1;
	}

	// The pWnd can only add to one Panel.
	CDockPanel *pDockPanel = (CDockPanel *)GetProp(pWnd->GetSafeHwnd(), GETPROP_PANEL);
	if (pDockPanel != NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	// Hook the element show/hide etc.
	WNDPROC procOld = (WNDPROC)IUISetWindowLong(pWnd->GetSafeHwnd(), GWLP_WNDPROC, (LONG_PTR)ElementWndProc);
	SetProp(pWnd->GetSafeHwnd(), GETPROP_PANEL, this);
	CWndProcList::AddToMap(pWnd->GetSafeHwnd(), procOld, (WNDPROC)ElementWndProc);

	DOCK_ELEMENT de;
	de.m_ePet = PET_WINDOW;
	de.m_pElement = pWnd;
	de.m_eDockStyle = eDockStyle;
	m_vElements.push_back(de);

	// layout elements.
	if (bLayout)
	{
		RelayoutElements();
	}

	size_t nSize = m_vElements.size();
	return int(nSize - 1);
}

int CDockPanel::AddElement(CWLWnd *pWnd, DOCK_STYLE eDockStyle, BOOL bLayout/* = TRUE*/)
{
	ASSERT(pWnd->GetParent() == GetParent());

	// If the object already exist.
	int nRet = FindElement(pWnd, NULL);
	if (nRet == 0)	// Found
	{
		return 1;
	}

	// The pWnd can only add to one Panel.
	CDockPanel *pDockPanel = (CDockPanel *)GetWLProp(pWnd, GETPROP_PANEL);
	if (pDockPanel != NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	// Hook the element show/hide etc.

	DOCK_ELEMENT de;
	de.m_ePet = PET_WINDOWLESS;
	de.m_pElement = pWnd;
	de.m_eDockStyle = eDockStyle;
	m_vElements.push_back(de);

	// layout elements.
	if (bLayout)
	{
		RelayoutElements();
	}

	size_t nSize = m_vElements.size();
	return int(nSize - 1);
}

int CDockPanel::AddElement(UINT uCtrlID, DOCK_STYLE eDockStyle, BOOL bLayout/* = TRUE*/)
{
	// If the object already exist.
	int nRet = FindElement(uCtrlID, NULL);
	if (nRet == 0)	// Found
	{
		return 1;
	}

	CUIWnd *pUIWnd = (CUIWnd *)GetParent();
	if (pUIWnd != NULL)
	{
		CWnd *pChildWnd = pUIWnd->GetDlgItem(uCtrlID);

		// may be a windowless control
		if (pChildWnd != NULL)
		{
			return AddElement(pChildWnd, eDockStyle, bLayout);
		}
		else
		{
			CWLWnd *pChildWLWnd = pUIWnd->GetDlgWLItem(uCtrlID);
			return AddElement(pChildWLWnd, eDockStyle, bLayout);
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	return -1;
}

int CDockPanel::AddElement(CPanel *pPanel, DOCK_STYLE eDockStyle, BOOL bLayout/* = TRUE*/)
{
	DOCK_ELEMENT de;
	de.m_ePet = PET_PANEL;
	de.m_pElement = pPanel;
	de.m_eDockStyle = eDockStyle;
	m_vElements.push_back(de);

	// layout elements.
	if (bLayout)
	{
		RelayoutElements();
	}

	size_t nSize = m_vElements.size();
	return int(nSize - 1);
}

CUIWnd *CDockPanel::AddChildWindowElement(UINT uWinID, DOCK_STYLE eDockStyle, BOOL bLayout/* = TRUE*/)
{
	// If the object already exist.
	int nRet = FindElement(uWinID, NULL);
	if (nRet == 0)	// Found
	{
		return NULL;
	}

	CUIWnd *pWnd = CUIWnd::CreateChildWindow(NULL, uWinID, WS_VISIBLE, GetParent(), NULL, 0);
	if (pWnd != NULL)
	{
		AddElement(pWnd, eDockStyle, bLayout);
	}
	else
	{
		ASSERT(FALSE);
	}

	return pWnd;
}

int CDockPanel::RemoveElement(int nIndex, BOOL bLayout/* = TRUE*/)
{
	int nCount = (int)m_vElements.size();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return -1;
	}

	// Remove GETPROP_PANEL prop
	if (m_vElements[nIndex].m_ePet == PET_WINDOW)
	{
		CWnd *pWnd = (CWnd *)m_vElements[nIndex].m_pElement;
		RemoveProp(pWnd->GetSafeHwnd(), GETPROP_PANEL);
	}

	m_vElements.erase(m_vElements.begin() + nIndex);

	if (bLayout)
	{
		RelayoutElements();
	}

	return 0;
}

int CDockPanel::RemoveElement(CWnd *pWnd, BOOL bLayout/* = TRUE*/)
{
	size_t nRet = -1;
	size_t nCount = m_vElements.size();
	for (size_t i = 0; i < nCount ; i++)
	{
		DOCK_ELEMENT *pElement = &m_vElements[i];

		if (pElement->m_ePet == PET_PANEL)
		{
			continue;
		}

		if (pElement->m_pElement == pWnd)
		{
			nRet = i;

			// Remove GETPROP_PANEL prop
			RemoveProp(pWnd->GetSafeHwnd(), GETPROP_PANEL);

			m_vElements.erase(m_vElements.begin() + i);
			break;
		}
	}

	if (bLayout)
	{
		RelayoutElements();
	}

	return (int)nRet;
}

int CDockPanel::GetElementCount()
{
	size_t nCount = m_vElements.size();

	return (int)nCount;
}

int CDockPanel::RemoveAllElements()
{
	m_vElements.clear();

	return 0;
}

int CDockPanel::ChangeElementOrder(CObject *pElement, int nNewIndex)
{
	size_t nOldIndex = -1;
	size_t nCount = m_vElements.size();
	for (size_t i = 0; i < nCount ; i++)
	{
		DOCK_ELEMENT *pItem = &m_vElements[i];

		if (pItem->m_pElement == (void *)pElement)
		{
			nOldIndex = i;

			break;
		}
	}

	if (nOldIndex == -1)
	{
		return -1;    // Not find the element.
	}

	if (nNewIndex >= (int)nCount)
	{
		nNewIndex = (int)nCount - 1;
	}
	if (nNewIndex < 0)
	{
		nNewIndex = 0;
	}

	if (nNewIndex == nOldIndex)
	{
		return 0;
	}

	DOCK_ELEMENT de = m_vElements[nOldIndex];
	m_vElements.erase(m_vElements.begin() + nOldIndex);
	m_vElements.insert(m_vElements.begin() + nNewIndex, de);

	return RelayoutElements(TRUE);
}

int CDockPanel::FindElement(const CObject *pElement, DOCK_ELEMENT *pDockElement)
{
	BOOL bFind = FALSE;
	size_t nCount = m_vElements.size();
	for (size_t i = 0; i < nCount ; i++)
	{
		DOCK_ELEMENT *pItem = &m_vElements[i];

		if (pItem->m_pElement == (void *)pElement)
		{
			bFind = TRUE;

			if (pDockElement != NULL)
			{
				*pDockElement = *pItem;
			}

			break;
		}
	}

	return bFind ? 0 : -1;
}

int CDockPanel::FindElement(UINT uWinID, DOCK_ELEMENT *pDockElement)
{
	BOOL bFind = FALSE;
	size_t nCount = m_vElements.size();
	for (size_t i = 0; i < nCount ; i++)
	{
		DOCK_ELEMENT *pItem = &m_vElements[i];

		if (pItem->m_ePet == PET_WINDOW)
		{
			CWnd *pWnd = (CWnd *)pItem->m_pElement;
			if (pWnd->GetDlgCtrlID() == uWinID)
			{
				bFind = TRUE;

				if (pDockElement != NULL)
				{
					*pDockElement = *pItem;
				}

				break;
			}
		}
	}

	return bFind ? 0 : -1;
}

int CDockPanel::SetDock(const CObject *pElement, DOCK_STYLE eDockStyle)
{
	return 0;
}

DOCK_STYLE CDockPanel::GetDock(const CObject *pElement) const
{
	return DS_NONE;
}

// Ignore the nX if dock style is DT_TOP or DT_BOTTOM, and nY if DT_LEFT or DT_RIGHT.
int CDockPanel::SetElementSize(const CObject *pElement, int nX, int nY)
{
	DOCK_ELEMENT deItem;
	int nFind = FindElement(pElement, &deItem);
	if (nFind != 0)
	{
		return nFind;
	}

	if (deItem.m_ePet == PET_WINDOW)
	{
		CWnd *pWndItem = (CWnd *)deItem.m_pElement;
		CRect rcItem;
		pWndItem->GetWindowRect(rcItem);
		GetParent()->ScreenToClient(rcItem);

		if (deItem.m_eDockStyle == DS_LEFT)
		{
			rcItem.right = rcItem.left + nX;
		}
		else if (deItem.m_eDockStyle == DS_TOP)
		{
			rcItem.bottom = rcItem.top + nY;
		}
		else if (deItem.m_eDockStyle == DS_RIGHT)
		{
			rcItem.left = rcItem.right - nX;
		}
		else if (deItem.m_eDockStyle == DS_BOTTOM)
		{
			rcItem.top = rcItem.bottom - nY;
		}
		else if (deItem.m_eDockStyle == DS_FILL)
		{
			// Ignore
		}
		else
		{
			ASSERT(FALSE);
		}

		pWndItem->MoveWindow(rcItem);
	}
	else if (deItem.m_ePet == PET_PANEL)
	{
		ASSERT(FALSE);	// Not support now
	}
	else
	{
		ASSERT(FALSE);
	}

	RelayoutElements();

	return 0;
}
