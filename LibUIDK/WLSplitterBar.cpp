// WLSplitterBar.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


struct WLSPLITTERMEMBER
{
	SPLITTER_ORIENTATION m_eOrientation;
	CWnd *m_pPanel;					// Another panel not the splitter bar in.
	SPLITTER_PANEL_POS m_ePanelPos;
	BOOL m_bPressedSplitterBar;
	BOOL m_bDraggingSplitterBar;
	CRect m_rcOldBarPos;
	CPoint m_ptRressedBar;		// The point of cursor while first pressed the bar.
};

#define m_eThisOrientation (((WLSPLITTERMEMBER *)m_pMember)->m_eOrientation)
#define m_pPanel (((WLSPLITTERMEMBER *)m_pMember)->m_pPanel)
#define m_ePanelPos (((WLSPLITTERMEMBER *)m_pMember)->m_ePanelPos)
#define m_bPressedSplitterBar (((WLSPLITTERMEMBER *)m_pMember)->m_bPressedSplitterBar)
#define m_bDraggingSplitterBar (((WLSPLITTERMEMBER *)m_pMember)->m_bDraggingSplitterBar)
#define m_rcOldBarPos (((WLSPLITTERMEMBER *)m_pMember)->m_rcOldBarPos)
#define m_ptRressedBar (((WLSPLITTERMEMBER *)m_pMember)->m_ptRressedBar)


#define GLP_PRESSED_BAR							1
#define GLP_DRAGGING							2
#define GLP_OLD_BAR_RECT_POINTER				3	// Only can get, can't set
#define GLP_PRESSED_CURSOR_PT_POINTER			4	// Only can get, can't set
#define GLP_PANEL_POS							5

//////////////////////////////////////////////////////////////////////////
// CWLSplitterBar

CWLSplitterBar::CWLSplitterBar()
{
	m_pMember = NULL;
	m_pMember = new WLSPLITTERMEMBER;

	m_eThisOrientation = SO_WE;
	m_pPanel = NULL;
	m_ePanelPos = SPP_RIGHT;
	m_bPressedSplitterBar = FALSE;
	m_bDraggingSplitterBar = FALSE;
	m_rcOldBarPos.SetRect(0, 0, 0, 0);
	m_ptRressedBar.x = m_ptRressedBar.y = 0;
}

CWLSplitterBar::~CWLSplitterBar()
{
	if (m_pMember != NULL)
	{
		delete (WLSPLITTERMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

void _LibUIDK_CSplitterBar_DrawDragBar(CRect rect, CRect &rcPre)
{
	CWindowDC dcScreen(NULL);
	CBrush *pBrush = CDC::GetHalftoneBrush();

	CBrush *pOldBrush = dcScreen.SelectObject(pBrush);
	dcScreen.PatBlt(rcPre.left, rcPre.top, rcPre.Width(), rcPre.Height(), PATINVERT);
	dcScreen.PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
	dcScreen.SelectObject(pOldBrush);

	rcPre = rect;
}

LRESULT CWLSplitterBar::SplitterParentWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC wndprocOld = CWndProcList::GetOldWndProc(hwnd, WNDPROC(SplitterParentWndProc));
	if (wndprocOld == NULL)
		return 0;

	CWLSplitterBar *pSplitterBar = (CWLSplitterBar *)GetProp(hwnd, GETPROP_SPLITTER_POINTER);
	if (pSplitterBar == NULL)
		return CallWindowProc(wndprocOld, hwnd, uMsg, wParam, lParam);

	CWnd *pParent = pSplitterBar->GetParent();
	ASSERT(pParent->GetSafeHwnd() == hwnd);

	if (uMsg == WM_NCDESTROY)	// the last message for a window
	{
		RemoveProp(hwnd, GETPROP_SPLITTER_POINTER);
	}

	// handle WM_MOUSEMOVE, WM_LBUTTONDOWN, WM_LBUTTONUP and WM_CAPTURECHANGED message to move the splitter
	else if (uMsg == WM_MOUSEMOVE)
	{
		// Change the cursor
		CRect rcVSplitter;		// Relative to the splitter bar's parent.
		pSplitterBar->GetWindowRect(rcVSplitter);
		pSplitterBar->GetParent()->ScreenToClient(rcVSplitter);

		CPoint pt(lParam);		// Relative to the splitter bar's parent.
		if (rcVSplitter.PtInRect(pt))
		{
			if (pSplitterBar->GetOrientation() == SO_WE)
				SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			else if (pSplitterBar->GetOrientation() == SO_NS)
				SetCursor(LoadCursor(NULL, IDC_SIZENS));
		}

		// If the splitter bar is dragging.
		LONG_PTR lData = pSplitterBar->GetLongPtr(GLP_DRAGGING);
#ifdef _WIN64
		ASSERT(lData >> 32 == 0);
#endif
		BOOL bDraggingSplitterBar = PtrToLong(reinterpret_cast<const void *>(lData));
		if (bDraggingSplitterBar)
		{
			// Draw the splitter bar. the limit position control by the minimize size of the two side of panel.
			CRect rcBarNew = rcVSplitter;			// Relative to screen.
			pParent->ClientToScreen(rcBarNew);

			// prcBarOld relative to screen. the rect is position of last WM_MOUSEMOVE, not the first WM_MOUSEMOVE, so is is changed every WM_MOUSEMOVE occur.
			CRect *prcBarOld = (CRect *)pSplitterBar->GetLongPtr(GLP_OLD_BAR_RECT_POINTER);
			CPoint *pPtCursorOld = (CPoint *)pSplitterBar->GetLongPtr(GLP_PRESSED_CURSOR_PT_POINTER);

			if (pSplitterBar->GetOrientation() == SO_WE)
				rcBarNew.OffsetRect(pt.x - pPtCursorOld->x, 0);
			else if (pSplitterBar->GetOrientation() == SO_NS)
				rcBarNew.OffsetRect(0, pt.y - pPtCursorOld->y);

			// Whether another panel allow drag to the new position or not.
			CWnd *pPanel = pSplitterBar->GetPanel();
			if (pPanel->GetSafeHwnd() != NULL)
			{
				SplitterStruct ss;
				ss.m_rcNew = rcBarNew;
				ss.m_rcOld = *prcBarOld;
				SPLITTER_PANEL_POS ePanelPos = (SPLITTER_PANEL_POS)pSplitterBar->GetLongPtr(GLP_PANEL_POS);
				if (ePanelPos == SPP_LEFT)
					ss.m_eDragOrientation = SDO_LEFT;
				else if (ePanelPos == SPP_RIGHT)
					ss.m_eDragOrientation = SDO_RIGHT;
				else if (ePanelPos == SPP_TOP)
					ss.m_eDragOrientation = SDO_UP;
				else if (ePanelPos == SPP_BOTTOM)
					ss.m_eDragOrientation = SDO_DOWN;
				else
					ASSERT(FALSE);
				int nRet = (int)pPanel->SendMessage(WM_DRAGGING_SPLITTERBAR, 0, (LPARAM)(&ss));
				rcBarNew = ss.m_rcNew;		// Since _LibUIDK_CSplitterBar_DrawDragBar can modify the old rect, so backup it.
				*prcBarOld = ss.m_rcOld;
				if (nRet == 0)
				{
					if (ePanelPos == SPP_LEFT)
						ss.m_eDragOrientation = SDO_RIGHT;
					else if (ePanelPos == SPP_RIGHT)
						ss.m_eDragOrientation = SDO_LEFT;
					else if (ePanelPos == SPP_TOP)
						ss.m_eDragOrientation = SDO_DOWN;
					else if (ePanelPos == SPP_BOTTOM)
						ss.m_eDragOrientation = SDO_UP;
					else
						ASSERT(FALSE);
					nRet = (int)pParent->SendMessage(WM_DRAGGING_SPLITTERBAR, 0, (LPARAM)(&ss));
					rcBarNew = ss.m_rcNew;
					*prcBarOld = ss.m_rcOld;
					_LibUIDK_CSplitterBar_DrawDragBar(rcBarNew, *prcBarOld);
				}
				else
				{
					_LibUIDK_CSplitterBar_DrawDragBar(rcBarNew, *prcBarOld);
				}
			}
			else
			{
				SplitterStruct ss;
				ss.m_rcNew = rcBarNew;
				ss.m_rcOld = *prcBarOld;
				if (pSplitterBar->GetOrientation() == SO_WE)
					ss.m_eDragOrientation = SDO_RIGHT;
				else if (pSplitterBar->GetOrientation() == SO_NS)
					ss.m_eDragOrientation = SDO_DOWN;
				int nRet = (int)pParent->SendMessage(WM_DRAGGING_SPLITTERBAR, 0, (LPARAM)(&ss));
				rcBarNew = ss.m_rcNew;
				*prcBarOld = ss.m_rcOld;
				_LibUIDK_CSplitterBar_DrawDragBar(rcBarNew, *prcBarOld);
			}
		}
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		// point and rcSplitter both relative to parent.
		CPoint point((DWORD)lParam);
		CRect rcSplitter;
		pSplitterBar->GetWindowRect(rcSplitter);
		pSplitterBar->GetParent()->ScreenToClient(rcSplitter);

		// If pressed the splitter bar.
		if (rcSplitter.PtInRect(point))
		{
			pSplitterBar->SetLongPtr(GLP_PRESSED_BAR, TRUE);

			pParent->SetCapture();

			// Save the current splitter bar position that relative screen.
			CRect *prcOld = (CRect *)pSplitterBar->GetLongPtr(GLP_OLD_BAR_RECT_POINTER);
			*prcOld = rcSplitter;
			pParent->ClientToScreen(prcOld);

			// Save the current point of cursor relative parent.
			CPoint *pPtCursorOld = (CPoint *)pSplitterBar->GetLongPtr(GLP_PRESSED_CURSOR_PT_POINTER);
			*pPtCursorOld = point;

			if (pSplitterBar->GetOrientation() == SO_WE)
				SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			else if (pSplitterBar->GetOrientation() == SO_NS)
				SetCursor(LoadCursor(NULL, IDC_SIZENS));

			pSplitterBar->SetLongPtr(GLP_DRAGGING, TRUE);
		}
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		// Release capture, and set dragging flag to FALSE.
		BOOL bPressedBar = (BOOL)pSplitterBar->GetLongPtr(GLP_PRESSED_BAR);
		if (bPressedBar)
		{
			CPoint pt(lParam);

			ReleaseCapture();
			pSplitterBar->SetLongPtr(GLP_DRAGGING, FALSE);
			pSplitterBar->SetLongPtr(GLP_PRESSED_BAR, FALSE);

			CRect rcVSplitter;
			pSplitterBar->GetWindowRect(rcVSplitter);
			pSplitterBar->GetParent()->ScreenToClient(rcVSplitter);

			CRect rcBarNew = rcVSplitter;
			pParent->ClientToScreen(rcBarNew);
			CPoint *pPtCursorOld = (CPoint *)pSplitterBar->GetLongPtr(GLP_PRESSED_CURSOR_PT_POINTER);
			if (pSplitterBar->GetOrientation() == SO_WE)
				rcBarNew.OffsetRect(pt.x - pPtCursorOld->x, 0);
			else if (pSplitterBar->GetOrientation() == SO_NS)
				rcBarNew.OffsetRect(0, pt.y - pPtCursorOld->y);

			// Erase the bar.
			CRect *prcOld = (CRect *)pSplitterBar->GetLongPtr(GLP_OLD_BAR_RECT_POINTER);
			CRect rcOld = *prcOld;
			_LibUIDK_CSplitterBar_DrawDragBar(CRect(0, 0, 0, 0), *prcOld);	// After draw, the prcOld equal to CRect(0, 0, 0, 0)

			// Clear the cursor point of last mouse move message
			pPtCursorOld->x = pPtCursorOld->y = 0;

			// Send end drag message to two panels to move the panel.
			SPLITTER_PANEL_POS ePanelPos = (SPLITTER_PANEL_POS)pSplitterBar->GetLongPtr(GLP_PANEL_POS);
			SplitterStruct ss;
			ss.m_rcNew = rcBarNew;
			ss.m_rcOld = rcOld;
			if (ePanelPos == SPP_LEFT)	// If another panel is left panel, and the splitter bar in right panel.
				ss.m_eDragOrientation = SDO_LEFT;
			else if (ePanelPos == SPP_RIGHT)
				ss.m_eDragOrientation = SDO_RIGHT;
			else if (ePanelPos == SPP_TOP)
				ss.m_eDragOrientation = SDO_UP;
			else if (ePanelPos == SPP_BOTTOM)
				ss.m_eDragOrientation = SDO_DOWN;
			else
			{
				ASSERT(FALSE);
			}
			CWnd *pPanel = pSplitterBar->GetPanel();
			if (pPanel->GetSafeHwnd() != NULL)
			{
				pPanel->SendMessage(WM_ENDDRAG_SPLITTERBAR, 0, (LPARAM)(&ss));
			}

			if (ePanelPos == SPP_LEFT)
				ss.m_eDragOrientation = SDO_RIGHT;
			else if (ePanelPos == SPP_RIGHT)
				ss.m_eDragOrientation = SDO_LEFT;
			else if (ePanelPos == SPP_TOP)
				ss.m_eDragOrientation = SDO_DOWN;
			else if (ePanelPos == SPP_BOTTOM)
				ss.m_eDragOrientation = SDO_UP;
			else
			{
				ASSERT(FALSE);
			}
			pParent->SendMessage(WM_ENDDRAG_SPLITTERBAR, 0, (LPARAM)(&ss));

			// Redraw all of the window.
			AfxGetMainWnd()->RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ALLCHILDREN);
		}
	}
	else if (uMsg == WM_CAPTURECHANGED)
	{
		LONG_PTR lData = pSplitterBar->GetLongPtr(GLP_DRAGGING);
#ifdef _WIN64
		ASSERT(lData >> 32 == 0);
#endif
		BOOL bDraggingSplitterBar = PtrToLong(reinterpret_cast<const void *>(lData));
		if (bDraggingSplitterBar)
		{
			ReleaseCapture();

			pSplitterBar->SetLongPtr(GLP_DRAGGING, FALSE);
			pSplitterBar->SetLongPtr(GLP_PRESSED_BAR, FALSE);

			CRect *prcOld = (CRect *)pSplitterBar->GetLongPtr(GLP_OLD_BAR_RECT_POINTER);
			CRect rcOld = *prcOld;
			_LibUIDK_CSplitterBar_DrawDragBar(CRect(0, 0, 0, 0), rcOld);

			// Clear the cursor point of last mouse move message
			CPoint *pPtCursorOld = (CPoint *)pSplitterBar->GetLongPtr(GLP_PRESSED_CURSOR_PT_POINTER);
			pPtCursorOld->x = pPtCursorOld->y = 0;

			AfxGetMainWnd()->RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ALLCHILDREN);
		}
	}

	return CallWindowProc(wndprocOld, hwnd, uMsg, wParam, lParam);
}

BOOL CWLSplitterBar::Create(LPCTSTR lpszControlName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CWLWnd *pVirtualParent)
{
	BOOL bRet = CWLWnd::Create(_T("IUIWL_SplitterBar"), lpszControlName, dwStyle, rect, pParentWnd, nID, pVirtualParent);
	if (!bRet)
		return FALSE;

	WNDPROC procOld = (WNDPROC)IUISetWindowLong(pParentWnd->GetSafeHwnd(), GWLP_WNDPROC, (LONG_PTR)SplitterParentWndProc);
	CWndProcList::AddToMap(pParentWnd->GetSafeHwnd(), procOld, (WNDPROC)SplitterParentWndProc);
	SetProp(pParentWnd->GetSafeHwnd(), GETPROP_SPLITTER_POINTER, (HANDLE)this);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// public

int CWLSplitterBar::SetOrientation(SPLITTER_ORIENTATION eOrientation)
{
	m_eThisOrientation = eOrientation;

	return 0;
}

SPLITTER_ORIENTATION CWLSplitterBar::GetOrientation() const
{
	return m_eThisOrientation;
}

int CWLSplitterBar::AddPanel(CWnd *pPanel, SPLITTER_PANEL_POS ePanelPos)
{
	m_pPanel = pPanel;
	m_ePanelPos = ePanelPos;

	return 0;
}

CWnd *CWLSplitterBar::GetPanel()
{
	return m_pPanel;
}

// Same as SetWindowLong, This interface should be used only LibUIDK. the return value is the previous value of the specified 32-bit integer
LONG_PTR CWLSplitterBar::SetLongPtr(__in int nIndex, __in LONG_PTR dwNewLong)
{
	if (nIndex == GLP_PRESSED_BAR)
	{
		BOOL bOld = m_bPressedSplitterBar;
		m_bPressedSplitterBar = (BOOL)dwNewLong;
		return (LONG_PTR)bOld;
	}
	else if (nIndex == GLP_DRAGGING)
	{
		BOOL bOld = m_bDraggingSplitterBar;
		m_bDraggingSplitterBar = (BOOL)dwNewLong;
		return (LONG_PTR)bOld;
	}

	ASSERT(FALSE);
	return -1;
}

// Same as GetWindowLong, This interface should be used only LibUIDK.
LONG_PTR CWLSplitterBar::GetLongPtr(__in int nIndex) const
{
	if (nIndex == GLP_PRESSED_BAR)
	{
		return (LONG_PTR)m_bPressedSplitterBar;
	}
	else if (nIndex == GLP_DRAGGING)
	{
		return (LONG_PTR)m_bDraggingSplitterBar;
	}
	else if (nIndex == GLP_OLD_BAR_RECT_POINTER)
	{
		return (LONG_PTR)(&m_rcOldBarPos);
	}
	else if (nIndex == GLP_PRESSED_CURSOR_PT_POINTER)
	{
		return (LONG_PTR)(&m_ptRressedBar);
	}
	else if (nIndex == GLP_PANEL_POS)
	{
		return (LONG_PTR)m_ePanelPos;
	}

	ASSERT(FALSE);
	return -1;
}
