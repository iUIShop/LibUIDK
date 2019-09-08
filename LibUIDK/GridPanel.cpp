// GridPanel.cpp: implementation of the CGridPanel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <map>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


CGridPanelElement::CGridPanelElement()
{
	m_eGet = GET_WINDOW;
	m_pWnd = NULL;
	m_pChildPanel = NULL;
	m_eHorAlign = GEAH_STRETCH;
	m_eVerAlign = GEAV_STRETCH;
	m_nRowSpan = 1;
	m_nColumnSpan = 1;
}

CGridPanelElement::~CGridPanelElement()
{

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGridPanel::CGridPanel()
{
	m_eHoriGpu = GPU_PERCENT;
	m_eVertGpu = GPU_PERCENT;
}

CGridPanel::~CGridPanel()
{
	int nRows = GetRowCount();
	int nColumns = GetColumnCount();
	for (int nRow = 0; nRow < nRows; ++nRow)
	{
		for (int nColumn = 0; nColumn < nColumns; ++nColumn)
		{
			CGridPanelElement *pEle = (CGridPanelElement *)m_Data.GetItemPointer(nRow, nColumn);
			if (pEle != NULL)
			{
				HWND hWnd = pEle->m_pWnd->GetSafeHwnd();
				if (hWnd != NULL)
				{
					WNDPROC wndprocCur = (WNDPROC)IUIGetWindowLong(hWnd, GWLP_WNDPROC);
					WNDPROC wndprocOld = CWndProcList::GetOldWndProc(hWnd, wndprocCur);
					IUISetWindowLong(hWnd, GWLP_WNDPROC, (LONG_PTR)wndprocOld);

					RemoveProp(hWnd, GETPROP_PANEL);
				}

				SafeDelete(pEle);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// protected

LRESULT CGridPanel::ElementWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC wndprocOld = CWndProcList::GetOldWndProc(hwnd, (WNDPROC)ElementWndProc);
	if (wndprocOld == NULL)
	{
		return 0;
	}

	if (uMsg == WM_DESTROY)
	{
		CGridPanel *pGridPanel = (CGridPanel *)GetProp(hwnd, GETPROP_PANEL);
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

int RelayoutChildWindow(CGridPanelElement *pEle, CWnd *pParent, const CRect &rcCell, const CRect &rcPanel)
{
	CWnd *pWnd = pEle->m_pWnd;
	if (pWnd->GetSafeHwnd() == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}
	CRect rcWnd;
	pWnd->GetWindowRect(rcWnd);
	pParent->ScreenToClient(rcWnd);
	int nWidth = rcWnd.Width();
	int nHeight = rcWnd.Height();
	int nCellWidth = rcCell.Width();
	int nCellHeight = rcCell.Height();

	// X coordinate
	if (pEle->m_eHorAlign == GEAH_LEFT || pEle->m_eHorAlign == GEAH_STRETCH)
	{
		rcWnd.left = rcCell.left;
	}
	else if (pEle->m_eHorAlign == GEAH_CENTER)
	{
		rcWnd.left = rcCell.left + ((nCellWidth - nWidth) / 2);
	}
	else if (pEle->m_eHorAlign == GEAH_RIGHT)
	{
		rcWnd.left = rcCell.left + (nCellWidth - nWidth);
	}

	if (pEle->m_eHorAlign == GEAH_STRETCH)
	{
		rcWnd.right = rcWnd.left + nCellWidth;
	}
	else
	{
		rcWnd.right = rcWnd.left + nWidth;
	}

	// Y coordinate
	if (pEle->m_eVerAlign == GEAV_TOP || pEle->m_eVerAlign == GEAV_STRETCH)
	{
		rcWnd.top = rcCell.top;
	}
	else if (pEle->m_eVerAlign == GEAV_CENTER)
	{
		rcWnd.top = rcCell.top + ((nCellHeight - nHeight) / 2);
	}
	else if (pEle->m_eVerAlign == GEAV_BOTTOM)
	{
		rcWnd.top = rcCell.top + (nCellHeight - nHeight);
	}

	if (pEle->m_eVerAlign == GEAV_STRETCH)
	{
		rcWnd.bottom = rcWnd.top + nCellHeight;
	}
	else
	{
		rcWnd.bottom = rcWnd.top + nHeight;
	}

	pWnd->MoveWindow(rcWnd);

	// if the control outside of the grid panel, hide it.
	if (rcWnd.right > rcPanel.right)
	{
		pWnd->ShowWindow(SW_HIDE);
	}
	else
	{
		pWnd->ShowWindow(SW_SHOW);
	}

	return 0;
}

int RelayoutChildPanel(CGridPanelElement *pEle, CWnd *pParent, const CRect &rcCell)
{
	CPanel *pChildPanel = pEle->m_pChildPanel;
	if (pChildPanel->IsCreated() == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	// The child panel must be stretch mode.
	pChildPanel->MoveWindow(rcCell);
	pChildPanel->RelayoutElements();

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// virtual

int CGridPanel::RelayoutElements(BOOL bRedraw/* = FALSE*/)
{
	CRect rcPanel;
	GetWindowRect(rcPanel);
	GetParent()->ScreenToClient(rcPanel);

	int nCellHorOffset = 0;
	int nCellVerOffset = rcPanel.top;

	CRect rcCell;

	int nRows = GetRowCount();
	int nColumns = GetColumnCount();
	for (int nRow = 0; nRow < nRows; ++nRow)
	{
		rcCell.top = nCellVerOffset;
		// The Last element fill the last space.
		if (nRow == nRows - 1)
		{
			rcCell.bottom = rcPanel.bottom;
		}
		else
		{
			int nCellHeight = GetRowHeight(nRow);
			if (m_eVertGpu == GPU_PERCENT)
			{
				nCellHeight = rcPanel.Height() * nCellHeight / 100;
			}

			rcCell.bottom = rcCell.top + nCellHeight;
			nCellVerOffset += nCellHeight;
		}

		for (int nColumn = 0; nColumn < nColumns; ++nColumn)
		{
			int nCellWidth = GetColumnWidth(nColumn);
			if (m_eHoriGpu == GPU_PERCENT)
			{
				nCellWidth = rcPanel.Width() * nCellWidth / 100;
			}

			if (nColumn == 0)
			{
				nCellHorOffset = rcPanel.left;
			}

			rcCell.left = nCellHorOffset;
			rcCell.right = rcCell.left + nCellWidth;

			nCellHorOffset += nCellWidth;

			// The rcCell is the rect of cell of row 'nRow' and column 'nColumn'.
			CGridPanelElement *pEle = (CGridPanelElement *)m_Data.GetItemPointer(nRow, nColumn);
			if (pEle == NULL)
			{
				continue;
			}

			if (pEle->m_eGet == GET_WINDOW)
			{
				RelayoutChildWindow(pEle, GetParent(), rcCell, rcPanel);
			}
			else if (pEle->m_eGet == GET_PANEL)
			{
				RelayoutChildPanel(pEle, GetParent(), rcCell);
			}
		}
	}

	return 0;
}

int CGridPanel::InsertPanel(CPanel *pChildPanel, UINT nPos)
{
	int nRow = LOWORD(nPos);
	int nCol = HIWORD(nPos);

	CGridPanelElement *pEle = new CGridPanelElement;
	pEle->m_eGet = GET_PANEL;
	pEle->m_pChildPanel = pChildPanel;
	m_Data.SetItemPointer(nRow, nCol, pEle);

	pChildPanel->SetParentPanel(this);

	return 0;
}

int CGridPanel::GetCellRect(int nRow, int nCol, LPRECT lpRect) const
{
	int nRows = GetRowCount();
	if (nRow < 0 || nRow >= nRows)
	{
		return -1;
	}

	int nColumns = GetColumnCount();
	if (nCol < 0 || nCol >= nColumns)
	{
		return -1;
	}

	CRect rect;
	GetWindowRect(rect);
	CWnd *pParent = const_cast<CGridPanel *>(this)->GetParent();
	pParent->ScreenToClient(rect);

	int nCellHorOffset = rect.left;
	int nCellVerOffset = rect.top;

	for (int r = 0; r <= nRow; ++r)
	{
		int nCellHeight = GetRowHeight(r);
		if (m_eVertGpu == GPU_PERCENT)
		{
			nCellHeight = rect.Height() * nCellHeight / 100;
		}

		if (r == nRow)
		{
			lpRect->top = nCellVerOffset;
			lpRect->bottom = lpRect->top + nCellHeight;
			break;
		}

		nCellVerOffset += nCellHeight;

		for (int c = 0; c <= nCol; ++c)
		{
			int nCellWidth = GetColumnWidth(c);
			if (m_eHoriGpu == GPU_PERCENT)
			{
				nCellWidth = rect.Width() * nCellWidth / 100;
			}

			if (c == nCol)
			{
				lpRect->left = nCellHorOffset;
				lpRect->right = lpRect->left + nCellWidth;
				break;
			}

			nCellHorOffset += nCellWidth;
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

BOOL CGridPanel::Create(LPRECT lpRect, CUIWnd *pParent, CWLWnd *pVirtualParent,
	CPanel *pParentGridPanel/* = NULL*/, UINT nParentRowColID/* = MAKELPARAM(0, 0)*/)
{
	CPanel::Create(NULL, 0, *lpRect, pParent, 1, pVirtualParent);

	if (pParentGridPanel != NULL)
	{
		pParentGridPanel->InsertPanel(this, nParentRowColID);
	}

	ASSERT(GetParent() == NULL);

	return TRUE;
}

BOOL CGridPanel::CreateStatic(LPRECT lpRect, int nRows, int nCols, CUIWnd *pParent, CWLWnd *pVirtualParent, CPanel *pParentGridPanel/* = NULL*/, UINT nParentRowColID/* = MAKELPARAM(0, 0)*/)
{
	if (nRows <= 0 || nCols <= 0)
	{
		return FALSE;
	}

	if (pParentGridPanel != NULL)
	{
		pParentGridPanel->InsertPanel(this, nParentRowColID);
	}

	ASSERT(GetParent() == NULL);

	for (int i = 0; i < nCols; ++i)
	{
		InsertColumn(i, 100 / nCols);
	}
	for (int j = 0; j < nRows; ++j)
	{
		InsertRow(j, 100 / nRows);
	}

	return TRUE;
}

int CGridPanel::SetGridHorizontalUnit(GRID_PANEL_UINT eGpu)
{
	m_eHoriGpu = eGpu;

	return 0;
}

GRID_PANEL_UINT CGridPanel::GetGridHorizontalUnit() const
{
	return m_eHoriGpu;
}

int CGridPanel::SetGridVerticalUnit(GRID_PANEL_UINT eGpu)
{
	m_eVertGpu = eGpu;

	return 0;
}

GRID_PANEL_UINT CGridPanel::GetGridVerticalUnit() const
{
	return m_eVertGpu;
}

int CGridPanel::InsertRow(int nIndex, int nHeight)
{
	int nRet = m_Data.InsertItem(nIndex, _T(""));
	m_Data.SetItemData(nRet, 0, nHeight);

	return nRet;
}

int CGridPanel::GetRowCount() const
{
	int nCount = m_Data.GetItemCount();
	return nCount;
}

int CGridPanel::SetRowHeight(int nIndex, int nHeight)
{
	BOOL bRet = m_Data.SetItemData(nIndex, 0, nHeight);
	if (!bRet)
	{
		return -1;
	}

	return 0;
}

int CGridPanel::GetRowHeight(int nIndex) const
{
	int nCount = GetRowCount();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return -1;
	}

	INT_PTR nHeight = m_Data.GetItemData(nIndex, 0);
#ifdef _WIN64
	ASSERT(nHeight >> 32 == 0);
#endif
	return (int)nHeight;
}

int CGridPanel::DeleteRow(int nIndex)
{
	BOOL bRet = m_Data.DeleteItem(nIndex);
	return bRet ? 0 : -1;
}

int CGridPanel::InsertColumn(int nIndex, int nWidth)
{
	int nRet = m_Data.InsertColumn(nIndex, StringFromInt(nWidth));

	return nRet;
}

int CGridPanel::GetColumnCount() const
{
	int nCount = m_Data.GetColumnCount();
	return nCount;
}

int CGridPanel::SetColumnWidth(int nIndex, int nWidth)
{
	CGridItem item;
	item.m_uMask = GIF_TEXT;
	BOOL bRet = m_Data.GetColumn(nIndex, &item);
	if (!bRet)
	{
		return -1;
	}

	CString strWidth;
	strWidth.Format(_T("%d"), nWidth);
	item.SetText(strWidth);
	m_Data.SetColumn(nIndex, &item);

	return 0;
}

int CGridPanel::GetColumnWidth(int nIndex) const
{
	CGridItem item;
	item.m_uMask = GIF_TEXT;
	BOOL bRet = m_Data.GetColumn(nIndex, &item);
	if (!bRet)
	{
		return -1;
	}

	int nWidth = IntFromString(item.GetText());
	return nWidth;
}

int CGridPanel::DeleteColumn(int nIndex)
{
	BOOL bRet = m_Data.DeleteColumn(nIndex);
	if (!bRet)
	{
		return -1;
	}

	return 0;
}

int CGridPanel::AddElement(CWnd *pWnd, int nRow, int nColumn, GRID_ELEMENT_ALIGN_HOR eHorAlign, GRID_ELEMENT_ALIGN_VER eVerAlign)
{
	ASSERT(pWnd->GetParent() == GetParent());

	// The pWnd can only add to one Panel.
	CGridPanel *pGridPanel = (CGridPanel *)GetProp(pWnd->GetSafeHwnd(), GETPROP_PANEL);
	if (pGridPanel != NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	WNDPROC procOld = (WNDPROC)IUISetWindowLong(pWnd->GetSafeHwnd(), GWLP_WNDPROC, (LONG_PTR)ElementWndProc);
	SetProp(pWnd->GetSafeHwnd(), GETPROP_PANEL, this);
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

int CGridPanel::RemoveElement(int nRow, int nColumn)
{
	CGridPanelElement *pEle = (CGridPanelElement *)m_Data.GetItemPointer(nRow, nColumn);
	SafeDelete(pEle);

	BOOL bRet = m_Data.SetItemPointer(nRow, nColumn, NULL);
	if (!bRet)
	{
		return -1;
	}

	RelayoutElements();

	return 0;
}

int CGridPanel::RemoveElement(CWnd *pWnd)
{
	int nRows = GetRowCount();
	int nColumns = GetColumnCount();

	for (int nRow = 0; nRow < nRows; ++nRow)
	{
		for (int nColumn = 0; nColumn < nColumns; ++nColumn)
		{
			CGridPanelElement *pEle = (CGridPanelElement *)m_Data.GetItemPointer(nRow, nColumn);
			if (pEle != NULL && pEle->m_pWnd == pWnd)
			{
				SafeDelete(pEle);
				m_Data.SetItemPointer(nRow, nColumn, NULL);
				break;
			}
		}
	}

	RelayoutElements();

	return 0;
}

int CGridPanel::RemoveAllElements()
{
	m_Data.DeleteAllItems();

	RelayoutElements();

	return 0;
}
