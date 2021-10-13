#include "StdAfx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int DoCNewWindowsUIPanel_MoveItem(CGrid *pGridData, int nColumnCount, int nIndexFrom, int nIndexTo)
{
	// Save from item.
	CGridPanelElement *pEleFrom = (CGridPanelElement *)pGridData->GetItemPointer(nIndexFrom / nColumnCount, nIndexFrom % nColumnCount);

	// Move one step for each item.
	if (nIndexFrom > nIndexTo)
	{
		for (int i = nIndexFrom - 1; i >= nIndexTo; --i)
		{
			CGridPanelElement *pEle = (CGridPanelElement *)pGridData->GetItemPointer(i / nColumnCount, i % nColumnCount);

			pGridData->SetItemPointer((i + 1) / nColumnCount, (i + 1) % nColumnCount, pEle);
		}

	}
	else
	{
		for (int i = nIndexFrom + 1; i <= nIndexTo; ++i)
		{
			CGridPanelElement *pEle = (CGridPanelElement *)pGridData->GetItemPointer(i / nColumnCount, i % nColumnCount);

			pGridData->SetItemPointer((i - 1) / nColumnCount, (i - 1) % nColumnCount, pEle);
		}
	}

	// Instead of
	pGridData->SetItemPointer(nIndexTo / nColumnCount, nIndexTo % nColumnCount, pEleFrom);

	return 0;
}

int DoCNewWindowsUIPanel_HitTestItem(CGrid *pGridData, int nEleCount, int nColumnCount, HWND hWndSender, const CRect &rcSender)
{
	int nIndex = -1;

	for (int i = 0; i < nEleCount; ++i)
	{
		CGridPanelElement *pItem = (CGridPanelElement *)pGridData->GetItemPointer(i / nColumnCount, i % nColumnCount);

		if (pItem == NULL)
		{
			break;
		}

		if (!::IsWindowVisible(pItem->m_pWnd->GetSafeHwnd()))
		{
			continue;
		}

		if (hWndSender == pItem->m_pWnd->GetSafeHwnd())
		{
			continue;
		}

		CRect rcItem;
		pItem->m_pWnd->GetWindowRect(rcItem);
		CWnd *pParent = CWnd::FromHandle(::GetParent(pItem->m_pWnd->GetSafeHwnd()));
		pParent->ScreenToClient(rcItem);

		CRect rcIns;
		rcIns.IntersectRect(rcSender, rcItem);

		// If the intersect rect's area is more than 70% of rcItem,
		if (rcIns.Width() * rcIns.Height() >= (rcItem.Width() * rcItem.Height() * 70 / 100))
		{
			nIndex = i;

			break;
		}
	}

	return nIndex;
}

//////////////////////////////////////////////////////////////////////////
// CTimerList

typedef CMap<UINT, UINT, CNewWindowsUIPanel *, CNewWindowsUIPanel *> TimerPanelList;

TimerPanelList LibUIDK_g_timer_panel_list;

class CTimerList
{
private:
	CTimerList();
	~CTimerList();

public:
	static int AddToMap(UINT nTimerID, CNewWindowsUIPanel *pPanel)
	{
		// If the nTimerID is already exist, return -1.
		CNewWindowsUIPanel *pRet = NULL;
		BOOL bExist = LibUIDK_g_timer_panel_list.Lookup(nTimerID, pRet);
		if (bExist)
		{
			return -1;
		}

		LibUIDK_g_timer_panel_list[nTimerID] = pPanel;

		return 0;
	}

	static CNewWindowsUIPanel *GetNewWindowUIPanel(UINT nTimerID)
	{
		CNewWindowsUIPanel *pRet = NULL;
		BOOL bExist = LibUIDK_g_timer_panel_list.Lookup(nTimerID, pRet);
		if (!bExist)
		{
			return NULL;
		}

		return pRet;
	}

	static int RemoveFromMap(UINT nTimerID)
	{
		// Remove from map
		BOOL bRet = LibUIDK_g_timer_panel_list.RemoveKey(nTimerID);
		if (!bRet)
		{
			return -1;
		}

		return 0;
	}
};

//////////////////////////////////////////////////////////////////////////
// CNWUIOverItemWnd

class LibUIDK::CNWUIOverItemWnd : public CUIWndBase
{
public:
	CNWUIOverItemWnd()
	{
		m_hWndBindItem = NULL;
	}

	~CNWUIOverItemWnd()
	{

	}

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT &cs)
	{
		// TODO: Add your specialized code here and/or call the base class

		cs.dwExStyle |= WS_EX_TOOLWINDOW;	// 1. Don't show the icon on task bar; 2. refuse 'Aero shake'(shake the title bar of the window to minimizing other desktop app).

		return CWnd::PreCreateWindow(cs);
	}

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_EXITSIZEMOVE)
		{
			if (m_pPnlBind->m_nIndexTo >= 0)
			{
				m_pPnlBind->MoveItem(m_pPnlBind->m_nBeginDragItemIndex, m_pPnlBind->m_nIndexTo);
				TRACE(_T("[LibUIDK]===CNewWindowsUIPanel move item from %d to %d\n"), m_pPnlBind->m_nBeginDragItemIndex, m_pPnlBind->m_nIndexTo);
			}
			m_pPnlBind->m_nIndexTo = -1;
			::ShowWindow(m_hWndBindItem, SW_SHOW);
			DestroyWindow();
		}

		// Hit test target position, the the target position has an item, move the target item.
		else if (message == WM_MOVE)
		{
			CRect rcThis;
			GetWindowRect(rcThis);

			HWND hWndItemParent = ::GetParent(m_hWndBindItem);
			ASSERT(hWndItemParent != NULL);
			CWnd *pParent = CWnd::FromHandle(hWndItemParent);
			pParent->ScreenToClient(rcThis);

			int nIndexTo = DoCNewWindowsUIPanel_HitTestItem(&m_pPnlBind->m_DataTemp, m_pPnlBind->GetElementCount(), m_pPnlBind->GetColumnCount(), m_hWndBindItem, rcThis);
			if (nIndexTo >= 0 && m_pPnlBind->m_nIndexTo != nIndexTo)
			{
				m_pPnlBind->m_nIndexTo = nIndexTo;
				TRACE(_T("[LibUIDK: %d]===CNewWindowsUIPanel will move item from %d to %d\n"), GetTickCount(), m_pPnlBind->m_nDragOverFromIndex, m_pPnlBind->m_nIndexTo);
				m_pPnlBind->AnimateAdjustItemOrder(m_pPnlBind->m_nDragOverFromIndex, m_pPnlBind->m_nIndexTo);
			}
		}

		return CUIWndBase::WindowProc(message, wParam, lParam);
	}

public:
	int SetBindItem(HWND hWnd)
	{
		m_hWndBindItem = hWnd;

		return 0;
	}

	int SetBindPanel(CNewWindowsUIPanel *pPanel)
	{
		m_pPnlBind = pPanel;

		return 0;
	}

protected:
	//{{AFX_MSG(CNWUIOverItemWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CNewWindowsUIPanel *m_pPnlBind;
	HWND m_hWndBindItem;
};

BEGIN_MESSAGE_MAP(CNWUIOverItemWnd, CUIWndBase)
	//{{AFX_MSG_MAP(CNWUIOverItemWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// CNewWindowsUIPanel

CNewWindowsUIPanel::CNewWindowsUIPanel()
{
	m_nElementCount = 0;
	m_bEnableDrag = FALSE;
	m_nBeginDragItemIndex = -1;
	m_nDragOverFromIndex = -1;
	m_nIndexTo = -1;
	m_nTimerTick = 0;
	m_nMaxTimerStep = 10;
	m_uClipFormat = 0;
	m_nAdjustItemOrderTimerID = -1;
}

CNewWindowsUIPanel::~CNewWindowsUIPanel()
{
}

//////////////////////////////////////////////////////////////////////////
// protected

int CNewWindowsUIPanel::OnDeletedItem(int nRow, int nColumn, CWnd *pWndDeleted)
{
	m_nElementCount--;

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

int CNewWindowsUIPanel::HitTestItem(HWND hWndSender, const CRect &rcSender)
{
	int nItemCount = GetElementCount();

	return DoCNewWindowsUIPanel_HitTestItem(&m_Data, nItemCount, GetColumnCount(), hWndSender, rcSender);
}

VOID CALLBACK CNewWindowsUIPanel::CNewWindowsUIPanel_OnTimer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	ASSERT(uMsg == WM_TIMER);

	// if there more than one panel in the hwnd, this method is wrong.
	CNewWindowsUIPanel *pThis = CTimerList::GetNewWindowUIPanel((UINT)idEvent);
	ASSERT(pThis != NULL);
	pThis->OnTimer(hwnd, uMsg, idEvent, dwTime);
}

void CNewWindowsUIPanel::OnTimer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if (idEvent == m_nAdjustItemOrderTimerID)
	{
		m_nTimerTick++;

		ASSERT(m_nDragOverFromIndex != m_nIndexTo && m_nDragOverFromIndex != -1);
		if (m_nDragOverFromIndex > m_nIndexTo)
		{
			// Move item [nIndexTo, nIndexFrom)

			int nColumnCount = GetColumnCount();

			for (int i = m_nDragOverFromIndex - 1; i >= m_nIndexTo; --i)
			{
				CGridPanelElement *pItem = (CGridPanelElement *)m_DataTemp.GetItemPointer(i / nColumnCount, i % nColumnCount);

				if (pItem == NULL)
				{
					break;
				}

				// Move i to i+1 position
				CRect rcFrom;
				int nRet = GetGridRect(i / nColumnCount, i % nColumnCount, NULL, rcFrom);
				CRect rcTo;
				nRet = GetGridRect((i + 1) / nColumnCount, (i + 1) % nColumnCount, NULL, rcTo);

				CPoint ptMoving;
				ptMoving.x = rcFrom.left + (rcTo.left - rcFrom.left) * m_nTimerTick / m_nMaxTimerStep;
				ptMoving.y = rcFrom.top + (rcTo.top - rcFrom.top) * m_nTimerTick / m_nMaxTimerStep;

				::SetWindowPos(pItem->m_pWnd->GetSafeHwnd(), NULL, ptMoving.x, ptMoving.y, 0, 0, SWP_NOSIZE);
			}
		}
		else // nIndexFrom < nIndexTo
		{
			// Move item (nIndexFrom, nIndexTo]

			int nColumnCount = GetColumnCount();

			for (int i = m_nDragOverFromIndex + 1; i <= m_nIndexTo; ++i)
			{
				CGridPanelElement *pItem = (CGridPanelElement *)m_DataTemp.GetItemPointer(i / nColumnCount, i % nColumnCount);

				if (pItem == NULL)
				{
					break;
				}

				// Move i to i-1 position
				CRect rcFrom;
				int nRet = GetGridRect(i / nColumnCount, i % nColumnCount, NULL, rcFrom);
				CRect rcTo;
				nRet = GetGridRect((i - 1) / nColumnCount, (i - 1) % nColumnCount, NULL, rcTo);

				CPoint ptMoving;
				ptMoving.x = rcTo.left + (rcFrom.left - rcTo.left) * (m_nMaxTimerStep - m_nTimerTick) / m_nMaxTimerStep;
				ptMoving.y = rcTo.top + (rcFrom.top - rcTo.top) * (m_nMaxTimerStep - m_nTimerTick) / m_nMaxTimerStep;

				::SetWindowPos(pItem->m_pWnd->GetSafeHwnd(), NULL, ptMoving.x, ptMoving.y, 0, 0, SWP_NOSIZE);
			}
		}

		if (m_nTimerTick >= m_nMaxTimerStep)
		{
			m_nTimerTick = 0;
			m_nAdjustItemOrderTimerID = -1;
			::KillTimer(NULL, m_nAdjustItemOrderTimerID);
			DoCNewWindowsUIPanel_MoveItem(&m_DataTemp, GetColumnCount(), m_nDragOverFromIndex, m_nIndexTo);
			m_nDragOverFromIndex = m_nIndexTo;
			GetParent()->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
		}
	}
}

int CNewWindowsUIPanel::AnimateAdjustItemOrder(int nIndexFrom, int nIndexTo)
{
	if (nIndexFrom < 0 || nIndexTo < 0)
	{
		return -1;
	}

	if (nIndexFrom == nIndexTo)
	{
		return 1;
	}

	m_nDragOverFromIndex = nIndexFrom;
	m_nIndexTo = nIndexTo;

	// Stop the old timer.
	if (m_nAdjustItemOrderTimerID != -1)
	{
		::KillTimer(NULL, m_nAdjustItemOrderTimerID);
		MoveItem(m_nBeginDragItemIndex, m_nIndexTo);
	}

	m_nAdjustItemOrderTimerID = (int)::SetTimer(NULL, 1, 20, CNewWindowsUIPanel_OnTimer);
	CTimerList::AddToMap(m_nAdjustItemOrderTimerID, this);

	return 0;
}

int CNewWindowsUIPanel::MoveItem(int nIndexFrom, int nIndexTo)
{
	if (nIndexFrom < 0 || nIndexTo < 0)
	{
		return -1;
	}

	if (nIndexFrom == nIndexTo)
	{
		return 1;
	}

	int nColumnCount = GetColumnCount();

	DoCNewWindowsUIPanel_MoveItem(&m_Data, nColumnCount, nIndexFrom, nIndexTo);

	if (m_bRedraw)
	{
		RelayoutElements(TRUE);
	}

	return 0;
}

UINT g_uShellGetDragImageMsg;

int CNewWindowsUIPanel::EnableDrag(BOOL bDrag)
{
	m_bEnableDrag = bDrag;

	if (m_bEnableDrag && m_drop.GetSafeHwnd() == NULL)
	{
		m_drop.Register(GetParent());

		// Get the ID of 'ShellGetDragImage' message. while called IDragSourceHelper::InitializeFromWindow, the "ShellGetDragImage" message will be send.
		g_uShellGetDragImageMsg = RegisterWindowMessage(DI_GETDRAGIMAGE);	// g_uShellGetDragImageMsg是一个类型为UINT的全局变量。
	}

	return 0;
}

BOOL CNewWindowsUIPanel::IsEnableDrag()
{
	return m_bEnableDrag;
}

// friend of CNewWindowsUIPanel, so can access the private data of CNewWindowsUIPanel.
LRESULT LibUIDK::NWUIElementWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC wndprocOld = CWndProcList::GetOldWndProc(hwnd, (WNDPROC)NWUIElementWndProc);
	if (wndprocOld == NULL)
	{
		return 0;
	}

	if (uMsg == WM_LBUTTONDOWN)
	{
		CNewWindowsUIPanel *pGridPanel = (CNewWindowsUIPanel *)GetProp(hwnd, GETPROP_PANEL);
		ASSERT(pGridPanel != NULL);

		if (pGridPanel->IsEnableDrag())
		{
			BOOL bRet = CheckForDragBegin(hwnd, LOWORD(lParam), HIWORD(lParam));
			if (bRet)
			{
				HWND hParent = GetParent(hwnd);
				ASSERT(hParent != NULL);

				// Use temp data to support animation.
				pGridPanel->m_DataTemp = pGridPanel->m_Data;

				// Get the index of dragging item.
				int nRow = 0;
				int nColumn = 0;
				int nRet = pGridPanel->FindElement(hwnd, NULL, &nRow, &nColumn);
				if (nRet != 0)
				{
					return -1;
				}

				NWUIITEM item;
				item.hdr.idFrom = GetDlgCtrlID(hwnd);
				item.hdr.hwndFrom = hwnd;
				item.hdr.code = NWUIN_DEBGINDRAG;
				item.nItem = nRow * pGridPanel->GetColumnCount() + nColumn;
				item.ptAction = CPoint(lParam);
				item.lParam = 0;
				pGridPanel->SendMessage(WM_NOTIFY, item.hdr.idFrom, (LPARAM)&item);
			}
		}
	}

	return CallWindowProc(wndprocOld, hwnd, uMsg, wParam, lParam);
}

int CNewWindowsUIPanel::AddElement(CWnd *pWnd, GRID_ELEMENT_ALIGN_HOR eHorAlign, GRID_ELEMENT_ALIGN_VER eVerAlign)
{
	int nCount = GetElementCount();
	int nColumnCount = GetColumnCount();

	int nRet = CUniformGridPanel::AddElement(pWnd, nCount / nColumnCount, nCount % nColumnCount, eHorAlign, eVerAlign);
	if (nRet != 0)
	{
		return nRet;
	}

	m_nElementCount++;

	WNDPROC procOld = (WNDPROC)IUISetWindowLong(pWnd->GetSafeHwnd(), GWLP_WNDPROC, (LONG_PTR)NWUIElementWndProc);
	CWndProcList::AddToMap(pWnd->GetSafeHwnd(), procOld, (WNDPROC)NWUIElementWndProc);

	return 0;
}

int CNewWindowsUIPanel::GetElementCount() const
{
	return m_nElementCount;
}

CGridPanelElement *CNewWindowsUIPanel::GetElement(int nIndex)
{
	int nColumnCount = GetColumnCount();

	CGridPanelElement *pEle = (CGridPanelElement *)m_Data.GetItemPointer(nIndex / nColumnCount, nIndex % nColumnCount);

	return pEle;
}

int CNewWindowsUIPanel::RemoveAllElements(BOOL bRelayout/* = TRUE*/)
{
	m_nElementCount = 0;

	return CUniformGridPanel::RemoveAllElements(bRelayout);
}

BOOL CNewWindowsUIPanel::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT *pResult)
{
	NMHDR *pnmhdr = (NMHDR *)lParam;

	if (pnmhdr->code == NWUIN_DEBGINDRAG)
	{
		NWUIITEM *pItem = (NWUIITEM *)lParam;
		ASSERT(pItem != NULL);

		m_nBeginDragItemIndex = m_nDragOverFromIndex = pItem->nItem;

		// 因为是popup窗口，所以快速移动的时候，在win7下可能导致其它桌面上的窗口最小化。甚至自己主窗口也会最小化。这个特性在win7中叫aero shake.
		// 另外，由于是popup窗口，所以会跑到主窗口之外。
		static CNWUIOverItemWnd m_wndDragIcon;
		m_wndDragIcon.SetBindItem(pItem->hdr.hwndFrom);
		m_wndDragIcon.SetBindPanel(this);
		BOOL bRet = m_wndDragIcon.LoadFrame(0, WS_VISIBLE | WS_POPUP | WS_CLIPCHILDREN, NULL);
		m_wndDragIcon.UseLayeredWindow(TRUE);
		CRect rcItem;
		::GetWindowRect(pItem->hdr.hwndFrom, rcItem);
		m_wndDragIcon.MoveWindow(rcItem);
		m_wndDragIcon.SetBkImage(_T("Button.png"));
		m_wndDragIcon.UpdateUIWindow(255);

		// Hide the item
		::ShowWindow(pItem->hdr.hwndFrom, SW_HIDE);

		// Must post message, Don't call SendMessage since CheckForDragBegin need return and call ReleaseCapture.
		PostMessage(m_wndDragIcon.GetSafeHwnd(), WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(0, 0));
	}

	return CUniformGridPanel::OnNotify(wParam, lParam, pResult);
}
