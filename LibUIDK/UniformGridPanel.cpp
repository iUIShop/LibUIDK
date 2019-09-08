// UniformGridPanel.cpp: implementation of the CUniformGridPanel class.
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

CUniformGridPanel::CUniformGridPanel()
{
}

CUniformGridPanel::~CUniformGridPanel()
{
}

int CUniformGridPanel::ReleaseObject()
{
	RemoveAllElements(FALSE);

	return CControlBase::ReleaseObject();
}

LRESULT CUniformGridPanel::ElementWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC wndprocOld = CWndProcList::GetOldWndProc(hwnd, (WNDPROC)ElementWndProc);
	if (wndprocOld == NULL)
	{
		return 0;
	}

	if (uMsg == WM_DESTROY)
	{
		CUniformGridPanel *pGridPanel = (CUniformGridPanel *)GetProp(hwnd, GETPROP_PANEL);
		if (pGridPanel != NULL)
		{
			pGridPanel->RemoveElement(CWnd::FromHandle(hwnd));
		}
	}
	else if (uMsg == WM_NCDESTROY)	// the last message for a window
	{
		LRESULT lr = CallWindowProc(wndprocOld, hwnd, uMsg, wParam, lParam);

		RemoveProp(hwnd, GETPROP_PANEL);

		return lr;
	}

	return CallWindowProc(wndprocOld, hwnd, uMsg, wParam, lParam);
}

int CUniformGridPanel::OnShowWindow(BOOL bShow)
{
	int nColumnCount = m_Data.GetColumnCount();
	int nRowCount = m_Data.GetItemCount();

	for (int nRow = 0; nRow < nRowCount; ++nRow)
	{
		for (int nColumn = 0; nColumn < nColumnCount; ++nColumn)
		{
			CGridPanelElement *pEle = (CGridPanelElement *)m_Data.GetItemPointer(nRow, nColumn);
			if (pEle == NULL)
			{
				continue;
			}

			// If the element is a window.
			if (pEle->m_eGet == GET_WINDOW)
			{
				CWnd *pWnd = pEle->m_pWnd;
				ASSERT(pWnd != NULL);

				// Move element
				pWnd->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
			}
			// If the element is a panel.
			else if (pEle->m_eGet == GET_PANEL)
			{
			}
			else
			{
				ASSERT(FALSE);
			}
		}
	}

	return 0;
}

int CUniformGridPanel::OnDeletingItem(int nRow, int nColumn, CWnd *pWndDeleting)
{
	return 0;
}

int CUniformGridPanel::OnDeletedItem(int nRow, int nColumn, CWnd *pWndDeleted)
{
	RemoveProp(pWndDeleted->GetSafeHwnd(), GETPROP_PANEL);

	return 0;
}

int CUniformGridPanel::RelayoutElements(BOOL bRedraw/* = FALSE*/)
{
	if (!IsCreated())
	{
		return -1;
	}

	int nColumnCount = GetColumnCount();
	if (nColumnCount <= 0)
	{
		return 0;
	}
	int nRowCount = GetRowCount();
	if (nRowCount <= 0)
	{
		return 0;
	}

	CRect rcPanel;
	GetWindowRect(rcPanel);
	GetParent()->ScreenToClient(rcPanel);

	for (int nRow = 0; nRow < nRowCount; ++nRow)
	{
		for (int nColumn = 0; nColumn < nColumnCount; ++nColumn)
		{
			CGridPanelElement *pEle = (CGridPanelElement *)m_Data.GetItemPointer(nRow, nColumn);
			if (pEle == NULL)
			{
				continue;
			}

			CRect rcElement;
			int nRet = GetGridRect(nRow, nColumn, NULL, rcElement);
			if (nRet != 0)
			{
				continue;
			}

			// If the element is a window.
			if (pEle->m_eGet == GET_WINDOW)
			{
				CWnd *pWnd = pEle->m_pWnd;
				ASSERT(pWnd != NULL);

				// Move element
				pWnd->MoveWindow(rcElement);
			}
			// If the element is a panel.
			else if (pEle->m_eGet == GET_PANEL)
			{
				pEle->m_pChildPanel->MoveWindow(rcElement);
			}
			else
			{
				ASSERT(FALSE);
			}
		}
	}

	return 0;
}

int CUniformGridPanel::MoveWindow(LPCRECT lpRect, BOOL bRepaint/* = TRUE */)
{
	CPanel::MoveWindow(lpRect, bRepaint);

	return RelayoutElements(bRepaint);
}

int CUniformGridPanel::InsertRow(int nIndex)
{
	int nRet = m_Data.InsertItem(nIndex, _T(""));

	return nRet;
}

int CUniformGridPanel::GetRowHeight()
{
	int nCount = GetRowCount();
	if (nCount <= 0)
	{
		return -1;
	}

	CRect rect;
	GetWindowRect(rect);
	GetParent()->ScreenToClient(rect);

	int nHeight = rect.Height() / nCount;
	return nHeight;
}

int CUniformGridPanel::InsertColumn(int nIndex)
{
	int nRet = m_Data.InsertColumn(nIndex, _T(""));

	return nRet;
}

int CUniformGridPanel::GetColumnWidth()
{
	int nCount = GetColumnCount();
	if (nCount <= 0)
	{
		return -1;
	}

	CRect rect;
	GetWindowRect(rect);
	GetParent()->ScreenToClient(rect);

	int nWidth = rect.Width() / nCount;
	return nWidth;
}

int CUniformGridPanel::GetRowCount() const
{
	return m_Data.GetItemCount();
}

int CUniformGridPanel::GetColumnCount() const
{
	return m_Data.GetColumnCount();
}


int CUniformGridPanel::RemoveAllGrid()
{
	RemoveAllElements(FALSE);

	// m_Data same as report style's CListCtrl, after delete all columns, all rows will be deleted together.
	// But if delete all rows, the column will not be deleted.
	int nColumnCount = GetColumnCount();

	for (int i = nColumnCount - 1; i >= 0; --i)
	{
		m_Data.DeleteColumn(i);
	}

	return 0;
}

int CUniformGridPanel::GetGridRect(int nRow, int nColumn, LPRECT lprcCell, LPRECT lprcElement)
{
	int nColumnCount = GetColumnCount();
	if (nColumnCount <= 0)
	{
		return -1;
	}
	int nRowCount = GetRowCount();
	if (nRowCount <= 0)
	{
		return -2;
	}

	if (nRow < 0 || nRow >= nRowCount || nColumn < 0 || nColumn >= nColumnCount)
	{
		return -3;
	}

	CRect rcPanel;
	GetWindowRect(rcPanel);
	GetParent()->ScreenToClient(rcPanel);

	int nCellHorOffset = rcPanel.left + rcPanel.Width() * nColumn / nColumnCount;
	int nCellVerOffset = rcPanel.top + rcPanel.Height() * nRow / nRowCount;
	int nCellWidth = rcPanel.Width() * (nColumn + 1) / nColumnCount - rcPanel.Width() * nColumn / nColumnCount;
	int nCellHeight = rcPanel.Height() * (nRow + 1) / nRowCount - rcPanel.Height() * nRow / nRowCount;

	if (lprcCell != NULL)
	{
		lprcCell->left = nCellHorOffset;
		lprcCell->right = lprcCell->left + nCellWidth;
		lprcCell->top = nCellVerOffset;
		lprcCell->bottom = lprcCell->top + nCellHeight;
	}

	if (lprcElement != NULL)
	{
		int nEleHorOffset = 0;
		int nEleVerOffset = 0;

		CGridPanelElement *pEle = (CGridPanelElement *)m_Data.GetItemPointer(nRow, nColumn);
		if (pEle == NULL)
		{
			return -5;
		}

		// Prepare the rect of element.
		CRect rcElement;
		// If the element is a window.
		if (pEle->m_eGet == GET_WINDOW)
		{
			CWnd *pWnd = pEle->m_pWnd;
			ASSERT(pWnd != NULL);
			pWnd->GetWindowRect(rcElement);
			GetParent()->ScreenToClient(rcElement);
		}
		// If the element is a panel.
		else if (pEle->m_eGet == GET_PANEL)
		{
			pEle->m_pChildPanel->GetWindowRect(rcElement);
			GetParent()->ScreenToClient(rcElement);
		}
		else
		{
			ASSERT(FALSE);
		}

		int nOldElementWidth = rcElement.Width();
		int nOldElementHeight = rcElement.Height();

		// Left
		if (pEle->m_eHorAlign == GEAH_LEFT || pEle->m_eHorAlign == GEAH_STRETCH)
		{
			nEleHorOffset = nCellHorOffset;
		}
		else if (pEle->m_eHorAlign == GEAH_CENTER)
		{
			nEleHorOffset = nCellHorOffset + ((nCellWidth - nOldElementWidth) / 2);
		}
		else if (pEle->m_eHorAlign == GEAH_RIGHT)
		{
			nEleHorOffset = nCellHorOffset + (nCellWidth - nOldElementWidth);
		}
		rcElement.left = nEleHorOffset;

		// Right
		if (pEle->m_eHorAlign == GEAH_STRETCH)
		{
			rcElement.right = rcElement.left + rcPanel.Width() * (nColumn + pEle->m_nColumnSpan) / nColumnCount - rcPanel.Width() * nColumn / nColumnCount;
		}
		else
		{
			rcElement.right = rcElement.left + nOldElementWidth;
		}

		// Top
		if (pEle->m_eVerAlign == GEAV_TOP || pEle->m_eVerAlign == GEAV_STRETCH)
		{
			nEleVerOffset = nCellVerOffset;
		}
		else if (pEle->m_eVerAlign == GEAV_CENTER)
		{
			nEleVerOffset = nCellVerOffset + ((nCellHeight - nOldElementHeight) / 2);
		}
		else if (pEle->m_eVerAlign == GEAV_BOTTOM)
		{
			nEleVerOffset = nCellVerOffset + (nCellHeight - nOldElementHeight);
		}
		rcElement.top = nEleVerOffset;

		// Bottom
		if (pEle->m_eVerAlign == GEAV_STRETCH)
		{
			rcElement.bottom = rcElement.top + rcPanel.Height() * (nRow + pEle->m_nRowSpan) / nRowCount - rcPanel.Height() * nRow / nRowCount;
		}
		else
		{
			rcElement.bottom = rcElement.top + nOldElementHeight;
		}

		*lprcElement = rcElement;
	}

	return 0;
}

int CUniformGridPanel::AddElement(CWnd *pWnd, int nRow, int nColumn, GRID_ELEMENT_ALIGN_HOR eHorAlign, GRID_ELEMENT_ALIGN_VER eVerAlign)
{
	ASSERT(pWnd->GetParent() == GetParent());

	// The pWnd can only add to one Panel.
	CUniformGridPanel *pGridPanel = (CUniformGridPanel *)GetProp(pWnd->GetSafeHwnd(), GETPROP_PANEL);
	if (pGridPanel != NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	SetProp(pWnd->GetSafeHwnd(), GETPROP_PANEL, this);
	WNDPROC procOld = (WNDPROC)IUISetWindowLong(pWnd->GetSafeHwnd(), GWLP_WNDPROC, (LONG_PTR)ElementWndProc);
	CWndProcList::AddToMap(pWnd->GetSafeHwnd(), procOld, (WNDPROC)ElementWndProc);

	CGridPanelElement *pEle = new CGridPanelElement;
	pEle->m_pWnd = pWnd;
	pEle->m_eHorAlign = eHorAlign;
	pEle->m_eVerAlign = eVerAlign;
	m_Data.SetItemPointer(nRow, nColumn, pEle);

	// layout elements.
	if (m_bRedraw)
	{
		RelayoutElements();
	}

	return 0;
}

int CUniformGridPanel::RemoveElement(int nRow, int nColumn, BOOL bRelayout/* = TRUE*/)
{
	CGridPanelElement *pEle = (CGridPanelElement *)m_Data.GetItemPointer(nRow, nColumn);
	if (pEle == NULL)
	{
		return -1;
	}

	BOOL bItemWindow = (pEle->m_eGet == GET_WINDOW);
	CWnd *pItemWnd = pEle->m_pWnd;
	if (bItemWindow)
	{
		OnDeletingItem(nRow, nColumn, pItemWnd);
	}

	BOOL bRet = m_Data.SetItemPointer(nRow, nColumn, NULL);
	if (!bRet)
	{
		return -1;
	}

	if (bItemWindow)
	{
		SetProp(pItemWnd->GetSafeHwnd(), GETPROP_PANEL, NULL);

		OnDeletedItem(nRow, nColumn, pItemWnd);
	}

	SafeDelete(pEle);

	if (bRelayout)
	{
		RelayoutElements();
	}

	return 0;
}

int CUniformGridPanel::RemoveElement(CWnd *pWnd, BOOL bRelayout/* = TRUE*/)
{
	CGridPanelElement *pEle = NULL;
	int nRow = -1;
	int nColumn = -1;
	int nRet = FindElement(pWnd, &pEle, &nRow, &nColumn);
	if (nRet != 0)
	{
		return nRet;
	}

	OnDeletingItem(nRow, nColumn, pWnd);

	SafeDelete(pEle);
	m_Data.SetItemPointer(nRow, nColumn, NULL);

	SetProp(pWnd->GetSafeHwnd(), GETPROP_PANEL, NULL);

	OnDeletedItem(nRow, nColumn, pWnd);

	if (bRelayout)
	{
		RelayoutElements();
	}

	return 0;
}

// The row and column count not change.
int CUniformGridPanel::RemoveAllElements(BOOL bRelayout/* = TRUE*/)
{
	int nRows = GetRowCount();
	int nColumns = GetColumnCount();

	for (int nRow = 0; nRow < nRows; ++nRow)
	{
		for (int nColumn = 0; nColumn < nColumns; ++nColumn)
		{
			RemoveElement(nRow, nColumn, FALSE);
		}
	}

	if (bRelayout)
	{
		RelayoutElements();
	}

	return 0;
}

int CUniformGridPanel::FindElement(const CObject *pElement, CGridPanelElement **ppgpe, int *pnRow, int *pnColumn) const
{
	BOOL bFind = FALSE;

	int nRows = GetRowCount();
	int nColumns = GetColumnCount();

	for (int nRow = 0; nRow < nRows; ++nRow)
	{
		for (int nColumn = 0; nColumn < nColumns; ++nColumn)
		{
			CGridPanelElement *pEle = (CGridPanelElement *)m_Data.GetItemPointer(nRow, nColumn);
			if (pEle == NULL)
			{
				continue;
			}

			if (pEle->m_eGet == GET_PANEL)
			{
				if (pEle->m_pChildPanel == pElement)
				{
					bFind = TRUE;

					if (ppgpe != NULL)
					{
						*ppgpe = pEle;
					}
					if (pnRow != NULL)
					{
						*pnRow = nRow;
					}
					if (pnColumn != NULL)
					{
						*pnColumn = nColumn;
					}

					break;
				}
			}
			else if (pEle->m_eGet == GET_WINDOW)
			{
				if (pEle->m_pWnd == pElement)
				{
					bFind = TRUE;

					if (ppgpe != NULL)
					{
						*ppgpe = pEle;
					}
					if (pnRow != NULL)
					{
						*pnRow = nRow;
					}
					if (pnColumn != NULL)
					{
						*pnColumn = nColumn;
					}

					break;
				}
			}
		}
	}

	return bFind ? 0 : -1;
}

int CUniformGridPanel::FindElement(HWND hElement, CGridPanelElement **ppgpe, int *pnRow, int *pnColumn) const
{
	BOOL bFind = FALSE;

	int nRows = GetRowCount();
	int nColumns = GetColumnCount();

	for (int nRow = 0; nRow < nRows; ++nRow)
	{
		for (int nColumn = 0; nColumn < nColumns; ++nColumn)
		{
			CGridPanelElement *pEle = (CGridPanelElement *)m_Data.GetItemPointer(nRow, nColumn);
			if (pEle == NULL)
			{
				continue;
			}

			if (pEle->m_eGet == GET_PANEL)
			{
			}
			else if (pEle->m_eGet == GET_WINDOW)
			{
				if (pEle->m_pWnd->GetSafeHwnd() == hElement)
				{
					bFind = TRUE;

					if (ppgpe != NULL)
					{
						*ppgpe = pEle;
					}
					if (pnRow != NULL)
					{
						*pnRow = nRow;
					}
					if (pnColumn != NULL)
					{
						*pnColumn = nColumn;
					}

					break;
				}
			}
		}
	}

	return bFind ? 0 : -1;
}

int CUniformGridPanel::SetElementSpan(const CObject *pElement, int nRowSpan, int nColumnSpan)
{
	CGridPanelElement *pEle = NULL;
	int nRow = -1;
	int nColumn = -1;
	int nRet = FindElement(pElement, &pEle, &nRow, &nColumn);
	if (nRet != 0)
	{
		return nRet;
	}

	pEle->m_nRowSpan = nRowSpan;
	pEle->m_nColumnSpan = nColumnSpan;

	return 0;
}

CGridPanelElement *CUniformGridPanel::GetElement(int nRow, int nColumn)
{
	CGridPanelElement *pEle = (CGridPanelElement *)m_Data.GetItemPointer(nRow, nColumn);

	return pEle;
}

LRESULT CUniformGridPanel::WLWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// Show or hide all HWND children.
	if (message == WM_WL_SHOWWINDOW)
	{
		BOOL bShow = (BOOL)wParam;
		OnShowWindow(bShow);
	}

	return CPanel::WLWindowProc(message, wParam, lParam);
}
