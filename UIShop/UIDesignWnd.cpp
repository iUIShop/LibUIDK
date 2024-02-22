// UIDesignWnd.cpp : implementation file
//

#include "stdafx.h"
#include "uishop.h"
#include "UIDesignWnd.h"
#include "UIShopView.h"
#include "MainFrm.h"
#include "IDMgr.h"
#include <atlconv.h>
#include <algorithm>
#include "Global.h"
using namespace Render;
using namespace ImageManager;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CMainFrame *g_pFrm;


/////////////////////////////////////////////////////////////////////////////
// CUIDesignWnd

CUIDesignWnd::CUIDesignWnd()
{
	m_nID = 1234;
	m_nCurZoomPercent = 1;
	m_bSizeToContent = TRUE;
	m_pCtrlFocus = NULL;
	m_pUIWndProperties = NULL;
	m_bValidLButtonUp = FALSE;
	m_bDragSelectChildren = FALSE;
	m_rcPreviousDragSelectRect.SetRect(0, 0, 0, 0);

	m_bSelected = FALSE;
	m_nHitTestRet = HTCLIENT;

	m_bShowControlType = FALSE;
	m_bShowControlID = FALSE;
	m_bShowControlIndex = FALSE;

	m_nCurStoryFrame = -1;		// -1 not in story board. The story frame from 0.
	m_bBackgroundShow = FALSE;
	m_himgBk = NULL;
	m_eBkImageResizeMode = IRM_9GRID;
	m_ptBkImageResize.x = m_ptBkImageResize.y = 2;
	m_lBkImageRepeatX = 1;
	m_lBkImageRepeatY = 1;
}

CUIDesignWnd::~CUIDesignWnd()
{
	size_t nChildCount = m_listCtrl.size();
	for (size_t i = 0; i < nChildCount; ++i)
	{
		CControlWnd *pCtrlWnd = m_listCtrl[i];
		delete pCtrlWnd;
	}

	m_listCtrl.clear();

	ReleaseIUIImage(m_himgBk);
}


BEGIN_MESSAGE_MAP(CUIDesignWnd, CWnd)
	//{{AFX_MSG_MAP(CUIDesignWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CUIDesignWnd message handlers

void CUIDesignWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	// get the background bitmap size
	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcWnd;
	GetWindowRect(rcWnd);
	GetParent()->ScreenToClient(rcWnd);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	if (m_bmpMemBK.GetSafeHandle() != NULL)
	{
		m_bmpMemBK.DeleteObject();
	}
	m_bmpMemBK.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap *pBmpMemOld = memDC.SelectObject(&m_bmpMemBK);

	memDC.FillSolidRect(rcClient, RGB(255, 255, 255));
	// Draw background color
	COLORREF crBk = GetSysColor(COLOR_3DFACE);
	CUIMgr::GetColorResource(m_pUIWndProperties->m_strBackgoundColorResID, &crBk);

	CRect rcUIWnd = m_pUIWndProperties->m_rcControl;
	memDC.FillSolidRect(rcUIWnd, crBk);

	// Draw background image
	DrawBkImage(&memDC);

	// Draw 'Background Show' windowless picture controls.
	DrawBackgroundShowControls(&memDC);

	// Draw resize line
	DrawResizingLine(&memDC);

	// Draw client area
	DrawClientArea(&memDC);

	// Draw selected flag
	if (m_bSelected)
	{
		DrawSelectedFlag(&memDC);
	}

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pBmpMemOld);

	// Do not call CWnd::OnPaint() for painting messages
}

int CUIDesignWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// TODO: Add your specialized creation code here

	return 0;
}

void CUIDesignWnd::DrawBkImage(CDC *pMemDC)
{
	if (m_himgBk == NULL)
	{
		return;
	}

	CRect rcUIWnd = m_pUIWndProperties->m_rcControl;

	int nDestWidth = rcUIWnd.Width();
	int nDestHeight = rcUIWnd.Height();
	if (rcUIWnd.Width() < m_himgBk->GetWidth())
	{
		nDestWidth = m_himgBk->GetWidth();
	}
	if (rcUIWnd.Height() < m_himgBk->GetHeight())
	{
		nDestHeight = m_himgBk->GetHeight();
	}

	CDC memDC;
	memDC.CreateCompatibleDC(pMemDC);

	IUINineGridBlt(pMemDC->GetSafeHdc(), 0, 0, nDestWidth, nDestHeight, m_himgBk,
		m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
}

void CUIDesignWnd::DrawBackgroundShowControls(CDC *pMemDC)
{
	// The hidden window less picture controls need 'Backgournd Show'.
	size_t nCtrlCount = m_listCtrl.size();
	for (int i = 0; i < nCtrlCount; ++i)
	{
		CControlWnd *pCtrlWnd = m_listCtrl[i];
		if ((CONTROL_TYPE)pCtrlWnd->GetControlProperties()->m_eControlType == CT_WL_PICTURE && !pCtrlWnd->IsWindowVisible())
		{
			CRect rcCtrl;
			pCtrlWnd->GetWindowRect(rcCtrl);
			ScreenToClient(rcCtrl);

			PICWLPROPERTIES *pWLPicProp = (PICWLPROPERTIES *)pCtrlWnd->GetControlProperties();
			POINT ptResize = (POINT)pWLPicProp->m_ptImageResize;
			HIUIIMAGE himg = CreateIUIImage(CUIMgr::GetUIPathWithoutTitle() + (LPCTSTR)pWLPicProp->m_strBKN);
			IUINineGridBlt(pMemDC->GetSafeHdc(), rcCtrl,
				himg, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
			ReleaseIUIImage(himg);
		}
	}
}

void CUIDesignWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	SetFocus();

	SelectAllControls(NULL, FALSE);

	m_bValidLButtonUp = TRUE;

	m_ptLButtonDown = point;	// Adjust the size of the ui window
	m_rcLButtonDown = (CRect)(m_pUIWndProperties->m_rcControl);

	CRect rcUIWnd = m_pUIWndProperties->m_rcControl;

	CRect rcRightBorder = rcUIWnd;
	rcRightBorder.left = rcRightBorder.right - 4;
	rcRightBorder.DeflateRect(0, 4, 0, 4);

	CRect rcBottomBorder = rcUIWnd;
	rcBottomBorder.top = rcBottomBorder.bottom - 4;
	rcBottomBorder.DeflateRect(4, 0, 4, 0);

	CRect rcRightBottom = rcUIWnd;
	rcRightBottom.left = rcRightBottom.right - 4;
	rcRightBottom.top = rcRightBottom.bottom - 4;

	if (rcRightBorder.PtInRect(point))
	{
		LONG_PTR lNew = (LONG_PTR)LoadCursor(NULL, IDC_SIZEWE);
		SetClassLongPtr(this->GetSafeHwnd(), GCLP_HCURSOR, lNew);

		m_nHitTestRet = HTRIGHT;
	}
	else if (rcBottomBorder.PtInRect(point))
	{
		LONG_PTR lNew = (LONG_PTR)LoadCursor(NULL, IDC_SIZENS);
		SetClassLongPtr(this->GetSafeHwnd(), GCLP_HCURSOR, lNew);

		m_nHitTestRet = HTBOTTOM;
	}
	else if (rcRightBottom.PtInRect(point))
	{
		LONG_PTR lNew = (LONG_PTR)LoadCursor(NULL, IDC_SIZENWSE);
		SetClassLongPtr(this->GetSafeHwnd(), GCLP_HCURSOR, lNew);

		m_nHitTestRet = HTBOTTOMRIGHT;
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CUIDesignWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (m_bValidLButtonUp)
	{
		CONTROL_TYPE eControlType = g_pFrm->GetCurSelControlType();
		if (eControlType == CT_ARROW)
		{
			if (m_bDragSelectChildren)
			{
				SelectControlByRect(m_rcPreviousDragSelectRect);
			}
			else
			{
				if (m_pCtrlFocus != NULL)
				{
					NMHDR nmhdr;
					nmhdr.hwndFrom = m_hWnd;
					nmhdr.idFrom = GetDlgCtrlID();
					nmhdr.code = UIVN_FOCUSCHANGED;
					NMUISHOPVIEW vhdr;
					vhdr.hdr = nmhdr;
					vhdr.pOldCtrlProp = m_pCtrlFocus;
					vhdr.pNewCtrlProp = GetUIWndProperties();
					CWnd *pParent = GetParent();
					pParent->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&vhdr));
				}
				m_pCtrlFocus = NULL;

				m_bSelected = TRUE;
				Invalidate();	// Draw the selected flag.
			}
		}
		else
		{
			CRect rcItem = CRect(point.x, point.y, point.x + 60, point.y + 24);
			CControlWnd *pCtrlNew = AddNewControl(rcItem, eControlType);

			// Add to undo list
			::CAction action;
			action.m_eAction = ::CAction::actionNewControl;
			action.m_pUIWndProp = m_pUIWndProperties;
			action.m_pCtrlProp = pCtrlNew->GetControlProperties();

			g_pFrm->GetUIShopDoc()->AddUndo(&action);
		}
	}

	ReleaseCapture();
	m_bDragSelectChildren = FALSE;
	m_rcPreviousDragSelectRect.SetRect(0, 0, 0, 0);
	m_bValidLButtonUp = FALSE;
	m_nHitTestRet = HTCLIENT;

	CWnd::OnLButtonUp(nFlags, point);
}

void CUIDesignWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	// If mouse move in the border of the ui window, resize the ui window.
	CONTROL_TYPE eControlType = g_pFrm->GetCurSelControlType();
	if (eControlType != CT_ARROW)
	{
		LONG_PTR lNew = (LONG_PTR)LoadCursor(NULL, IDC_CROSS);
		SetClassLongPtr(this->GetSafeHwnd(), GCLP_HCURSOR, lNew);
	}
	else
	{
		LONG_PTR lNew = (LONG_PTR)LoadCursor(NULL, IDC_ARROW);
		SetClassLongPtr(this->GetSafeHwnd(), GCLP_HCURSOR, lNew);
	}

	if ((nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		if (m_nHitTestRet == HTRIGHT)
		{
			int nXOffset = point.x - m_ptLButtonDown.x;

			CRect rc = m_rcLButtonDown;
			rc.right += nXOffset;
			m_pUIWndProperties->m_rcControl = rc;
			m_pUIWndProperties->m_rcControl.wReserved1 = 1;
			Invalidate();
			UpdateFocusControlPosInStatusBar(m_pUIWndProperties);
		}
		else if (m_nHitTestRet == HTBOTTOM)
		{
			int nYOffset = point.y - m_ptLButtonDown.y;

			CRect rc = m_rcLButtonDown;
			rc.bottom += nYOffset;
			m_pUIWndProperties->m_rcControl = rc;
			m_pUIWndProperties->m_rcControl.wReserved1 = 1;
			Invalidate();
			UpdateFocusControlPosInStatusBar(m_pUIWndProperties);
		}
		else if (m_nHitTestRet == HTBOTTOMRIGHT)
		{
			int nXOffset = point.x - m_ptLButtonDown.x;
			int nYOffset = point.y - m_ptLButtonDown.y;

			CRect rc = m_rcLButtonDown;
			rc.right += nXOffset;
			rc.bottom += nYOffset;
			m_pUIWndProperties->m_rcControl = rc;
			m_pUIWndProperties->m_rcControl.wReserved1 = 1;
			Invalidate();
			UpdateFocusControlPosInStatusBar(m_pUIWndProperties);
		}
		else if (m_nHitTestRet == HTCLIENT)	// Select controls.
		{
			SetCapture();
			m_bDragSelectChildren = TRUE;

			CRect rcSelect(m_ptLButtonDown, point);
			rcSelect.NormalizeRect();

			// Draw selection rectangle on top of it.
			int cx = rcSelect.Width();
			int cy = rcSelect.Height();
			if (cx != 0 && cy != 0)
			{
				CClientDC dc(this);
				dc.DrawFocusRect(m_rcPreviousDragSelectRect);
				dc.DrawFocusRect(rcSelect);
				m_rcPreviousDragSelectRect = rcSelect;
			}
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CUIDesignWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	InvalidateRect(NULL);
}

BOOL CUIDesignWnd::PreTranslateMessage(MSG *pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_KEYDOWN == pMsg->message)
	{
		UINT nChar = (UINT)pMsg->wParam;
		if (nChar == VK_LEFT || nChar == VK_UP || nChar == VK_RIGHT || nChar == VK_DOWN)
		{
			MoveControls(nChar, GetAsyncKeyState(VK_SHIFT) < 0, GetAsyncKeyState(VK_CONTROL) < 0);
		}
		// if the control is selected, delete it
		else if (nChar == VK_DELETE)
		{
			CUIShopDoc *pDoc = (CUIShopDoc *)g_pFrm->GetUIShopDoc();

			// Add delete control action to undo list
			::CAction action;
			action.m_eAction = ::CAction::actionDeleteControl;
			action.m_pUIWndProp = m_pUIWndProperties;

			size_t nCount = m_listCtrl.size();
			size_t i = nCount - 1;
			vector<CControlWnd *>::reverse_iterator rit = m_listCtrl.rbegin();
			for (; rit != m_listCtrl.rend(); --i)
			{
				CControlWnd *pCtrlWnd = *rit;
				if (pCtrlWnd->m_bSelected)
				{
					// If the deleted control is focus control.
					if (m_pCtrlFocus == pCtrlWnd->GetControlProperties())
					{
						SetFocusCtrl(NULL, TRUE);
					}

					CTRLPROPERTIES *pCtrlProp = m_pUIWndProperties->m_vControls[i];
					CTRLPROPERTIES *p = pCtrlWnd->GetControlProperties();
					ASSERT(p == pCtrlProp);

					CControlDelete cd;
					cd.m_pCtrlProp = pCtrlProp;

					// delete control window
					rit = std::vector<CControlWnd *>::reverse_iterator(m_listCtrl.erase((++rit).base()));
					delete pCtrlWnd;

					vector<CTRLPROPERTIES *>::iterator it = GetUIWndProperties()->m_vControls.begin() + i;
					GetUIWndProperties()->m_vControls.erase(it);

					pDoc->SetModifiedFlag(TRUE);

					action.m_ControlDelete.AddHead(cd);
				}
				else
				{
					++rit;
				}
			}

			pDoc->AddUndo(&action);
		}
		// if the control is selected, show the properties dialog of the control
		else if (nChar == VK_RETURN)
		{
			size_t nCount = m_listCtrl.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				if (m_listCtrl[i]->m_bSelected)
				{
					m_listCtrl[i]->ShowPropertiesDlg();
				}
			}
		}
		// Handle Ctrl+A
		else if (nChar == 'A')
		{
			SHORT nCtrlKeyState = GetKeyState(VK_CONTROL);
			if (nCtrlKeyState < 0) // CTRL key is down
			{
				size_t nCount = m_listCtrl.size();
				for (size_t i = 0; i < nCount; ++i)
				{
					CControlWnd *pCtrlWnd = m_listCtrl[i];
					pCtrlWnd->Selected(TRUE);
				}
			}
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

LRESULT CUIDesignWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	if (WM_GETFOCUSCTRL == message)
	{
		return LRESULT(m_pCtrlFocus);
	}
	if (WM_SETFOCUSCTRL == message)
	{
		CTRLPROPERTIES *pNewFocus = (CTRLPROPERTIES *)wParam;	// pNewFocus为不在动画中的控件。
		BOOL bFocus = BOOL(lParam);
		if (bFocus)
		{
			if (m_pCtrlFocus != pNewFocus)
			{
				CTRLPROPERTIES *pNewFocusPropInStroyBoard = pNewFocus;
				// If current in storyboard mode, use properties in storyboard.
				// If the properties not in storyboard, create a new properties.
				// If the new properties be modified, add it to storyboard. otherwise, release it.
				if (m_nCurStoryFrame != -1)
				{
					// Find properties in storyboard
					STORY_FRAME *pStoryFrame = m_pUIWndProperties->FindStoryFrame(m_nCurStoryFrame);
					ASSERT(pStoryFrame != NULL);

					pNewFocusPropInStroyBoard = pStoryFrame->FindControl(pNewFocus->m_strID);
					ASSERT(pNewFocusPropInStroyBoard != NULL);
				}

				m_pCtrlFocus = pNewFocus;

				NMHDR nmhdr;
				nmhdr.hwndFrom = m_hWnd;
				nmhdr.idFrom = GetDlgCtrlID();
				nmhdr.code = UIVN_FOCUSCHANGED;
				NMUISHOPVIEW vhdr;
				vhdr.hdr = nmhdr;
				vhdr.pOldCtrlProp = m_pCtrlFocus;
				vhdr.pNewCtrlProp = pNewFocusPropInStroyBoard;
				CWnd *pParent = GetParent();
				pParent->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&vhdr));

				m_bSelected = FALSE;
				RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
			}
		}
		else
		{
			// move focus to last selected control.
			vector<CControlWnd *> vSels;
			GetSelectedControls(&vSels);
			vector<CControlWnd *>::reverse_iterator rit = vSels.rbegin();
			for (; rit != vSels.rend(); ++rit)
			{
				CControlWnd *pWnd = *rit;
				if (pWnd->GetControlProperties() == m_pCtrlFocus)
				{
					if (rit != vSels.rend() - 1) // not first element in vSels.
					{
						SendMessage(WM_SETFOCUSCTRL, WPARAM((*(rit + 1))->GetControlProperties()), TRUE);
					}
					else
					{
						SendMessage(WM_SETFOCUSCTRL, NULL, TRUE);
					}
				}
			}
		}
	}
	if (WM_GETSELCOUNT == message)
	{
		int nCount = 0;
		for (UINT i = 0; i < m_listCtrl.size(); ++i)
		{
			if (m_listCtrl[i]->m_bSelected)
			{
				nCount++;
			}
		}

		return nCount;
	}

	if (WM_CHILDMOVING == message)
	{
		CControlWnd *pWnd = (CControlWnd *)wParam;
		int nXMove = 0;
		int nYMove = 0;
		pWnd->GetMoveOffset(&nXMove, &nYMove);
		MoveSelectedControls(pWnd, nXMove, nYMove);
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

void CUIDesignWnd::SelectAllControls(CControlWnd *pSender, BOOL bSelect)
{
	size_t i = 0;

	//
	size_t nCtrlCount = m_listCtrl.size();
	for (i = 0; i < nCtrlCount; ++i)
	{
		if (m_listCtrl[i] != pSender)
		{
			m_listCtrl[i]->Selected(bSelect);
		}
	}

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
}

int CUIDesignWnd::SelectControlByRect(const CRect &rect)
{
	SelectAllControls(NULL, FALSE);

	size_t i = 0;
	//
	size_t nCtrlCount = m_listCtrl.size();
	for (i = 0; i < nCtrlCount; ++i)
	{
		CControlWnd *pChildCtrl = m_listCtrl[i];

		CRect rcChild;
		pChildCtrl->GetWindowRect(rcChild);
		ScreenToClient(rcChild);

		if (rect.left <= rcChild.left && rect.top <= rcChild.top && rect.right >= rcChild.right && rect.bottom >= rcChild.bottom)
		{
			pChildCtrl->Selected(TRUE);
		}
	}

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

	return 0;
}

void CUIDesignWnd::ShowTypeFlag(BOOL bShow)
{
	m_bShowControlType = bShow;

	for (UINT i = 0; i < m_listCtrl.size(); ++i)
	{
		m_listCtrl[i]->SetTypeFlag(bShow);
	}
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
}

BOOL CUIDesignWnd::IsTypeFlagShow()
{
	return m_bShowControlType;
}

void CUIDesignWnd::ShowIDFlag(BOOL bShow)
{
	m_bShowControlID = bShow;

	for (UINT i = 0; i < m_listCtrl.size(); ++i)
	{
		m_listCtrl[i]->ShowIDFlag(bShow);
	}
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
}

BOOL CUIDesignWnd::IsIDFlagShow()
{
	return m_bShowControlID;
}

void CUIDesignWnd::ShowIndexFlag(BOOL bShow)
{
	m_bShowControlIndex = bShow;

	for (UINT i = 0; i < m_listCtrl.size(); ++i)
	{
		CControlWnd *pControlWnd = m_listCtrl[i];

		pControlWnd->ShowIndexValue(bShow);

		if (bShow)
		{
			// Update the index
			CTRLPROPERTIES *pCtrlProp = pControlWnd->GetControlProperties();
			int nIndex = GetControlIndex(pCtrlProp);

			pControlWnd->SetIndexValue(nIndex + 1);
		}
		else
		{
			pControlWnd->SetIndexValue(0);
		}
	}

	if (bShow)
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
	}
}

BOOL CUIDesignWnd::IsIndexFlagShow()
{
	return m_bShowControlIndex;
}

void CUIDesignWnd::MoveControls(UINT nChar, BOOL bShift, BOOL bCtrl)
{
	// Add to undo list
	::CAction action;
	action.m_eAction = ::CAction::actionMoveControl;
	action.m_pUIWndProp = m_pUIWndProperties;

	// Move control(s)
	if (nChar == VK_LEFT)
	{
		if (m_bSelected)
		{
			if (bShift)
			{
				CRect rcWnd = m_pUIWndProperties->m_rcControl;
				rcWnd.right -= 1;
				m_pUIWndProperties->m_rcControl = rcWnd;
				m_pUIWndProperties->m_rcControl.wReserved1 = 1;
				Invalidate();
				UpdateFocusControlPosInStatusBar(m_pUIWndProperties);
			}
		}
		else
		{
			for (UINT i = 0; i < m_listCtrl.size(); ++i)
			{
				CControlWnd *pCtrlWnd = m_listCtrl[i];
				if (pCtrlWnd->m_bSelected)
				{
					CRect rcWin;
					pCtrlWnd->GetWindowRect(rcWin);
					ScreenToClient(rcWin);

					CControlMove cm;
					cm.m_pCtrlProp = pCtrlWnd->GetControlProperties();
					cm.m_rectOld = rcWin;
					cm.m_rectNew = rcWin;

					if (bCtrl && bShift)
					{
						pCtrlWnd->SetWindowPos(NULL, 0, 0, rcWin.Width() - 10, rcWin.Height(), SWP_NOMOVE);
						cm.m_rectNew.DeflateRect(0, 0, 10, 0);
					}
					else if (bShift)
					{
						pCtrlWnd->SetWindowPos(NULL, 0, 0, rcWin.Width() - 1, rcWin.Height(), SWP_NOMOVE);
						cm.m_rectNew.DeflateRect(0, 0, 1, 0);
					}
					else if (bCtrl)
					{
						pCtrlWnd->SetWindowPos(NULL, rcWin.left - 10, rcWin.top, 0, 0, SWP_NOSIZE);
						cm.m_rectNew.DeflateRect(-10, 0, 10, 0);
					}
					else
					{
						pCtrlWnd->SetWindowPos(NULL, rcWin.left - 1, rcWin.top, 0, 0, SWP_NOSIZE);
						cm.m_rectNew.DeflateRect(-1, 0, 1, 0);
					}

					action.m_ControlMove.AddHead(cm);
				}
			}
		}
	}
	else if (nChar == VK_UP)
	{
		if (m_bSelected)
		{
			if (bShift)
			{
				CRect rcWnd = m_pUIWndProperties->m_rcControl;
				rcWnd.bottom -= 1;
				m_pUIWndProperties->m_rcControl = rcWnd;
				m_pUIWndProperties->m_rcControl.wReserved1 = 1;
				Invalidate();
				UpdateFocusControlPosInStatusBar(m_pUIWndProperties);
			}
		}
		else
		{
			for (UINT i = 0; i < m_listCtrl.size(); ++i)
			{
				CControlWnd *pCtrlWnd = m_listCtrl[i];
				if (pCtrlWnd->m_bSelected)
				{
					CRect rcWin;
					pCtrlWnd->GetWindowRect(rcWin);
					ScreenToClient(rcWin);

					CControlMove cm;
					cm.m_pCtrlProp = pCtrlWnd->GetControlProperties();
					cm.m_rectOld = rcWin;
					cm.m_rectNew = rcWin;

					if (bCtrl && bShift)
					{
						BOOL bIsComboBox = (pCtrlWnd->m_eControlType == CT_COMBOBOX);
						if (bIsComboBox)
						{
							if (rcWin.Height() <= 6)
							{
								continue;
							}
							if (rcWin.Height() >= 255 + 6)
							{
								continue;
							}
						}
						pCtrlWnd->SetWindowPos(NULL, 0, 0, rcWin.Width(), rcWin.Height() - 10, SWP_NOMOVE);

						cm.m_rectNew.DeflateRect(0, 0, 0, 10);
					}
					else if (bShift)
					{
						BOOL bIsComboBox = (pCtrlWnd->m_eControlType == CT_COMBOBOX);
						if (bIsComboBox)
						{
							if (rcWin.Height() <= 6)
							{
								continue;
							}
							if (rcWin.Height() >= 255 + 6)
							{
								continue;
							}
						}
						pCtrlWnd->SetWindowPos(NULL, 0, 0, rcWin.Width(), rcWin.Height() - 1, SWP_NOMOVE);

						cm.m_rectNew.DeflateRect(0, 0, 0, 1);
					}
					else if (bCtrl)
					{
						pCtrlWnd->SetWindowPos(NULL, rcWin.left, rcWin.top - 10, 0, 0, SWP_NOSIZE);
						cm.m_rectNew.DeflateRect(0, -10, 0, 10);
					}
					else
					{
						pCtrlWnd->SetWindowPos(NULL, rcWin.left, rcWin.top - 1, 0, 0, SWP_NOSIZE);
						cm.m_rectNew.DeflateRect(0, -1, 0, 1);
					}

					action.m_ControlMove.AddHead(cm);
				}
			}
		}
	}
	else if (nChar == VK_RIGHT)
	{
		if (m_bSelected)
		{
			if (bShift)
			{
				CRect rcWnd = m_pUIWndProperties->m_rcControl;
				rcWnd.right += 1;
				m_pUIWndProperties->m_rcControl = rcWnd;
				m_pUIWndProperties->m_rcControl.wReserved1 = 1;
				Invalidate();
				UpdateFocusControlPosInStatusBar(m_pUIWndProperties);
			}
		}
		else
		{
			for (UINT i = 0; i < m_listCtrl.size(); ++i)
			{
				CControlWnd *pCtrlWnd = m_listCtrl[i];
				if (pCtrlWnd->m_bSelected)
				{
					CRect rcWin;
					pCtrlWnd->GetWindowRect(rcWin);
					ScreenToClient(rcWin);

					CControlMove cm;
					cm.m_pCtrlProp = pCtrlWnd->GetControlProperties();
					cm.m_rectOld = rcWin;
					cm.m_rectNew = rcWin;

					if (bCtrl && bShift)
					{
						pCtrlWnd->SetWindowPos(NULL, 0, 0, rcWin.Width() + 10, rcWin.Height(), SWP_NOMOVE);
						cm.m_rectNew.DeflateRect(0, 0, -10, 0);
					}
					else if (bShift)
					{
						pCtrlWnd->SetWindowPos(NULL, 0, 0, rcWin.Width() + 1, rcWin.Height(), SWP_NOMOVE);
						cm.m_rectNew.DeflateRect(0, 0, -1, 0);
					}
					else if (bCtrl)
					{
						pCtrlWnd->SetWindowPos(NULL, rcWin.left + 10, rcWin.top, 0, 0, SWP_NOSIZE);
						cm.m_rectNew.DeflateRect(10, 0, -10, 0);
					}
					else
					{
						pCtrlWnd->SetWindowPos(NULL, rcWin.left + 1, rcWin.top, 0, 0, SWP_NOSIZE);
						cm.m_rectNew.DeflateRect(1, 0, -1, 0);
					}

					action.m_ControlMove.AddHead(cm);
				}
			}
		}
	}
	else if (nChar == VK_DOWN)
	{
		if (m_bSelected)
		{
			if (bShift)
			{
				CRect rcWnd = m_pUIWndProperties->m_rcControl;
				rcWnd.bottom += 1;
				m_pUIWndProperties->m_rcControl = rcWnd;
				m_pUIWndProperties->m_rcControl.wReserved1 = 1;
				Invalidate();
				UpdateFocusControlPosInStatusBar(m_pUIWndProperties);
			}
		}
		else
		{
			for (UINT i = 0; i < m_listCtrl.size(); ++i)
			{
				CControlWnd *pCtrlWnd = m_listCtrl[i];
				if (pCtrlWnd->m_bSelected)
				{
					CRect rcWin;
					pCtrlWnd->GetWindowRect(rcWin);
					ScreenToClient(rcWin);

					CControlMove cm;
					cm.m_pCtrlProp = pCtrlWnd->GetControlProperties();
					cm.m_rectOld = rcWin;
					cm.m_rectNew = rcWin;

					if (bCtrl && bShift)
					{
						BOOL bIsComboBox = (pCtrlWnd->m_eControlType == CT_COMBOBOX);
						if (bIsComboBox)
						{
							if (rcWin.Height() <= 6)
							{
								continue;
							}
							if (rcWin.Height() >= 255 + 6)
							{
								continue;
							}
						}
						pCtrlWnd->SetWindowPos(NULL, 0, 0, rcWin.Width(), rcWin.Height() + 10, SWP_NOMOVE);

						cm.m_rectNew.DeflateRect(0, 0, 0, -10);
					}
					else if (bShift)
					{
						BOOL bIsComboBox = (pCtrlWnd->m_eControlType == CT_COMBOBOX);
						if (bIsComboBox)
						{
							if (rcWin.Height() <= 6)
							{
								continue;
							}
							if (rcWin.Height() >= 255 + 6)
							{
								continue;
							}
						}
						pCtrlWnd->SetWindowPos(NULL, 0, 0, rcWin.Width(), rcWin.Height() + 1, SWP_NOMOVE);

						cm.m_rectNew.DeflateRect(0, 0, 0, -1);
					}
					else if (bCtrl)
					{
						pCtrlWnd->SetWindowPos(NULL, rcWin.left, rcWin.top + 10, 0, 0, SWP_NOSIZE);
						cm.m_rectNew.DeflateRect(0, 10, 0, -10);
					}
					else
					{
						pCtrlWnd->SetWindowPos(NULL, rcWin.left, rcWin.top + 1, 0, 0, SWP_NOSIZE);
						cm.m_rectNew.DeflateRect(0, 1, 0, -1);
					}

					action.m_ControlMove.AddHead(cm);
				}
			}
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	// Add to undo list
	if (!action.m_ControlMove.IsEmpty())
	{
		CUIShopDoc *pDoc = g_pFrm->GetUIShopDoc();
		ASSERT(pDoc != NULL);

		pDoc->AddUndo(&action);
	}

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

	CUIShopDoc *pDoc = (CUIShopDoc *)g_pFrm->GetUIShopDoc();
	pDoc->SetModifiedFlag(TRUE);
}

int CUIDesignWnd::SetUIWndProperties(UIWNDPROPERTIES *pProperties)
{
	m_pUIWndProperties = pProperties;

	return 0;
}

UIWNDPROPERTIES *CUIDesignWnd::GetUIWndProperties()
{
	return m_pUIWndProperties;
}

void CUIDesignWnd::UpdateControlCoordinate(CControlWnd *pwndCtrl)
{
	ASSERT(pwndCtrl != NULL);

	CRect rect;
	pwndCtrl->GetWindowRect(rect);
	ScreenToClient(rect);
	rect.left /= m_nCurZoomPercent;
	rect.top /= m_nCurZoomPercent;
	rect.right /= m_nCurZoomPercent;
	rect.bottom /= m_nCurZoomPercent;

	CTRLPROPERTIES *pCtrlProp = pwndCtrl->GetControlProperties();

	if (m_nCurStoryFrame == -1)
	{
		pCtrlProp->m_rcControl = rect;
		pCtrlProp->m_rcControl.wReserved1 = 1;
	}
	else
	{
		// Update control rect in storyboard
		STORY_FRAME *pStoryItem = m_pUIWndProperties->FindStoryFrame(m_nCurStoryFrame);
		ASSERT(pStoryItem != NULL);

		CTRLPROPERTIES *pStoryCtrl = pStoryItem->FindControl(pCtrlProp->m_strID);
		ASSERT(pStoryCtrl != NULL);

		pStoryCtrl->m_rcControl = rect;
		pStoryCtrl->m_rcControl.wReserved1 = 1;
	}

	UpdateFocusControlPosInStatusBar(pCtrlProp);
}

void CUIDesignWnd::UpdateFocusControlPosInStatusBar(CTRLPROPERTIES *pCtrlProp)
{
	if (pCtrlProp == NULL)
	{
		g_pFrm->SetControlPosText(_T(""));
		return;
	}

	CRect rect;
	if (m_nCurStoryFrame == -1)
	{
		CRect rect2 = pCtrlProp->m_rcControl;
		rect = rect2;
	}
	else
	{
		STORY_FRAME *pStoryItem = m_pUIWndProperties->FindStoryFrame(m_nCurStoryFrame);
		if (pStoryItem != NULL)
		{
			CTRLPROPERTIES *pStoryCtrl = pStoryItem->FindControl(pCtrlProp->m_strID);
			if (pStoryCtrl != NULL)
			{
				CRect rect2 = pStoryCtrl->m_rcControl;
				rect = rect2;
			}
			else
			{
				g_pFrm->SetControlPosText(_T(""));
				return;
			}
		}
		else
		{
			g_pFrm->SetControlPosText(_T(""));
			return;
		}
	}

	CString strText;
	strText.Format(_T("{%d, %d, %d, %d} %d*%d"), rect.left, rect.top, rect.right, rect.bottom, rect.Width(), rect.Height());
	g_pFrm->SetControlPosText(strText);
}

int CUIDesignWnd::GetSelectedControls(vector<CControlWnd *> *pvControls)
{
	// The CUIDesignWnd may be destroyed.
	if (!IsWindow(m_hWnd))
	{
		return -1;
	}

	CControlWnd *pWnd = (CControlWnd *)GetWindow(GW_CHILD);
	while (pWnd != NULL)
	{
		if (pWnd->IsSelected())
		{
			pvControls->push_back(pWnd);
		}

		pWnd = (CControlWnd *)pWnd->GetWindow(GW_HWNDNEXT);
	}

	return 0;
}

void CUIDesignWnd::SetCurZoomPercent(int nPercent)
{
	int nOldZoom = m_nCurZoomPercent;
	m_nCurZoomPercent = nPercent;
	if (m_bSizeToContent)
	{
		int nWidth = m_himgBk->GetWidth();
		int nHeight = m_himgBk->GetHeight();

		CRect rcClient;
		rcClient.SetRect(0, 0, nWidth * m_nCurZoomPercent, nHeight * m_nCurZoomPercent);
		MoveWindow(rcClient);

		// move children control
		size_t nChildrenCount = m_listCtrl.size();
		for (size_t i = 0; i < nChildrenCount; ++i)
		{
			CRect rcWin;
			m_listCtrl[i]->GetWindowRect(rcWin);
			ScreenToClient(rcWin);

			int nOldWidth = rcWin.Width();
			int nOldHeight = rcWin.Height();

			rcWin.left = rcWin.left * m_nCurZoomPercent / nOldZoom;
			rcWin.top = rcWin.top * m_nCurZoomPercent / nOldZoom;
			rcWin.right = rcWin.left + nOldWidth * m_nCurZoomPercent / nOldZoom;
			rcWin.bottom = rcWin.top + nOldHeight * m_nCurZoomPercent / nOldZoom;

			m_listCtrl[i]->MoveWindow(rcWin);
		}

		((CScrollView *)GetParent())->SetScrollSizes(MM_TEXT, CSize(VIEW_WIDTH_MAX * m_nCurZoomPercent, VIEW_HEIGHT_MAX * m_nCurZoomPercent));
	}
}

int CUIDesignWnd::GetCurZoomPercent()
{
	return m_nCurZoomPercent;
}

int CUIDesignWnd::SetFocusCtrl(CTRLPROPERTIES *pFocusCtrl, BOOL bFocus)
{
	return (int)SendMessage(WM_SETFOCUSCTRL, WPARAM(pFocusCtrl), LPARAM(bFocus));
}

CTRLPROPERTIES *CUIDesignWnd::GetFocusCtrl()
{
	return m_pCtrlFocus;
}

BOOL CUIDesignWnd::IsUIWindowSelected() const
{
	return m_bSelected;
}

int CUIDesignWnd::AlignSelectedControls(int nDirection)
{
	if (m_pCtrlFocus == NULL)
	{
		AfxMessageBox(_T("Please select the focus control!"), MB_ICONERROR);
		return -2;
	}

	// Collect selected controls.
	vector<CControlWnd *> vSelCtrls;
	CControlWnd *pWnd = (CControlWnd *)GetWindow(GW_CHILD);
	while (pWnd != NULL)
	{
		if (pWnd->IsSelected())
		{
			vSelCtrls.push_back(pWnd);
		}

		pWnd = (CControlWnd *)pWnd->GetWindow(GW_HWNDNEXT);
	}

	size_t nSelCount = vSelCtrls.size();

	if (nSelCount < 2)
	{
		return -1;
	}

	// Add to undo list
	::CAction action;
	action.m_eAction = ::CAction::actionMoveControl;
	action.m_pUIWndProp = m_pUIWndProperties;

	CRect rcFocus = m_pCtrlFocus->m_rcControl;

	for (size_t i = 0; i < nSelCount; ++i)
	{
		CControlWnd *pCtrl = vSelCtrls[i];
		CRect rcCtrl;
		pCtrl->GetWindowRect(rcCtrl);
		ScreenToClient(rcCtrl);

		//
		CControlMove cm;
		cm.m_pCtrlProp = pCtrl->GetControlProperties();
		cm.m_rectOld = rcCtrl;

		if (nDirection == 1)		// left
		{
			int nXOffset = rcCtrl.left - rcFocus.left;
			rcCtrl.DeflateRect(-nXOffset, 0, nXOffset, 0);
		}
		else if (nDirection == 2)	// top
		{
			int nYOffset = rcCtrl.top - rcFocus.top;
			rcCtrl.DeflateRect(0, -nYOffset, 0, nYOffset);
		}
		else if (nDirection == 3)
		{
			int nXOffset = rcCtrl.right - rcFocus.right;
			rcCtrl.DeflateRect(-nXOffset, 0, nXOffset, 0);
		}
		else if (nDirection == 4)
		{
			int nYOffset = rcCtrl.bottom - rcFocus.bottom;
			rcCtrl.DeflateRect(0, -nYOffset, 0, nYOffset);
		}
		pCtrl->MoveWindow(rcCtrl);

		//
		cm.m_rectNew = rcCtrl;
		action.m_ControlMove.AddHead(cm);
	}

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

	// Add to undo list
	g_pFrm->GetUIShopDoc()->AddUndo(&action);

	return 0;
}

int CUIDesignWnd::CenterSelectedControls(BOOL bHorizontal)
{
	// Collect selected controls.
	vector<CControlWnd *> vSelCtrls;
	CControlWnd *pWnd = (CControlWnd *)GetWindow(GW_CHILD);
	while (pWnd != NULL)
	{
		if (pWnd->IsSelected())
		{
			vSelCtrls.push_back(pWnd);
		}

		pWnd = (CControlWnd *)pWnd->GetWindow(GW_HWNDNEXT);
	}

	size_t nSelCount = vSelCtrls.size();

	if (nSelCount < 1)
	{
		return -1;
	}

	// Add to undo list
	::CAction action;
	action.m_eAction = ::CAction::actionMoveControl;
	action.m_pUIWndProp = m_pUIWndProperties;

	CRect rcClient = (CRect)m_pUIWndProperties->m_rcControl;
	CRect rcOutside(rcClient.right, rcClient.bottom, rcClient.left, rcClient.top);

	size_t i = 0;
	for (i = 0; i < nSelCount; ++i)
	{
		CControlWnd *pCtrl = vSelCtrls[i];
		CRect rcCtrl;
		pCtrl->GetWindowRect(rcCtrl);
		ScreenToClient(rcCtrl);

		rcOutside.left = min(rcOutside.left, rcCtrl.left);
		rcOutside.top = min(rcOutside.top, rcCtrl.top);
		rcOutside.right = max(rcOutside.right, rcCtrl.right);
		rcOutside.bottom = max(rcOutside.bottom, rcCtrl.bottom);
	}

	int nXOffset = rcClient.CenterPoint().x - rcOutside.CenterPoint().x;
	int nYOffset = rcClient.CenterPoint().y - rcOutside.CenterPoint().y;

	for (i = 0; i < nSelCount; ++i)
	{
		CControlWnd *pCtrl = vSelCtrls[i];
		CRect rcCtrl;
		pCtrl->GetWindowRect(rcCtrl);
		ScreenToClient(rcCtrl);

		CControlMove cm;
		cm.m_pCtrlProp = pCtrl->GetControlProperties();
		cm.m_rectOld = rcCtrl;

		if (bHorizontal)
		{
			rcCtrl.DeflateRect(nXOffset, 0, -nXOffset, 0);
		}
		else
		{
			rcCtrl.DeflateRect(0, nYOffset, 0, -nYOffset);
		}
		pCtrl->MoveWindow(rcCtrl);

		//
		cm.m_rectNew = rcCtrl;
		action.m_ControlMove.AddHead(cm);
	}

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

	// Add to undo list
	g_pFrm->GetUIShopDoc()->AddUndo(&action);

	return 0;
}

struct SORTCONTROL
{
	CControlWnd *pWnd;
	CRect rect;
};

bool mycompareX(SORTCONTROL sc1, SORTCONTROL sc2)
{
	return sc1.rect.CenterPoint().x < sc2.rect.CenterPoint().x;
}

bool mycompareY(SORTCONTROL sc1, SORTCONTROL sc2)
{
	return sc1.rect.CenterPoint().y < sc2.rect.CenterPoint().y;
}

int CUIDesignWnd::SpaceSelectedControls(BOOL bHorizontal)
{
	// Collect selected controls.
	vector<SORTCONTROL> vSelCtrls;
	CControlWnd *pWnd = (CControlWnd *)GetWindow(GW_CHILD);
	while (pWnd != NULL)
	{
		if (pWnd->IsSelected())
		{
			CRect rect;
			pWnd->GetWindowRect(rect);
			ScreenToClient(rect);

			SORTCONTROL sc;
			sc.pWnd = pWnd;
			sc.rect = rect;

			vSelCtrls.push_back(sc);
		}

		pWnd = (CControlWnd *)pWnd->GetWindow(GW_HWNDNEXT);
	}

	size_t nSelCount = vSelCtrls.size();

	if (nSelCount < 3)
	{
		return -1;
	}

	// Add to undo list
	::CAction action;
	action.m_eAction = ::CAction::actionMoveControl;
	action.m_pUIWndProp = m_pUIWndProperties;

	if (bHorizontal)
	{
		std::sort(vSelCtrls.begin(), vSelCtrls.end(), mycompareX);
		size_t nSpace = (vSelCtrls[nSelCount - 1].rect.CenterPoint().x - vSelCtrls[0].rect.CenterPoint().x) / (nSelCount - 1);

		for (size_t i = 1; i < nSelCount - 1; ++i)
		{
			SORTCONTROL sc = vSelCtrls[i];

			CControlMove cm;
			cm.m_pCtrlProp = sc.pWnd->GetControlProperties();
			cm.m_rectOld = sc.rect;

			int nXOffset = int(sc.rect.CenterPoint().x - (vSelCtrls[0].rect.CenterPoint().x + i * nSpace));
			sc.rect.DeflateRect(-nXOffset, 0, nXOffset, 0);
			sc.pWnd->MoveWindow(sc.rect);

			cm.m_rectNew = sc.rect;
			action.m_ControlMove.AddHead(cm);
		}
	}
	else
	{
		std::sort(vSelCtrls.begin(), vSelCtrls.end(), mycompareY);
		size_t nSpace = (vSelCtrls[nSelCount - 1].rect.CenterPoint().y - vSelCtrls[0].rect.CenterPoint().y) / (nSelCount - 1);

		for (size_t i = 1; i < nSelCount - 1; ++i)
		{
			SORTCONTROL sc = vSelCtrls[i];

			CControlMove cm;
			cm.m_pCtrlProp = sc.pWnd->GetControlProperties();
			cm.m_rectOld = sc.rect;

			int nYOffset = int(sc.rect.CenterPoint().y - (vSelCtrls[0].rect.CenterPoint().y + i * nSpace));
			sc.rect.DeflateRect(0, -nYOffset, 0, nYOffset);
			sc.pWnd->MoveWindow(sc.rect);

			cm.m_rectNew = sc.rect;
			action.m_ControlMove.AddHead(cm);
		}
	}

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

	// Add to undo list
	g_pFrm->GetUIShopDoc()->AddUndo(&action);

	return 0;
}

int CUIDesignWnd::MakeSameSizeSelectedControls(MakeSameSize enumMask)
{
	if (m_pCtrlFocus == NULL)
	{
		AfxMessageBox(_T("Please select the focus control!"), MB_ICONERROR);
		return -2;
	}

	// Collect selected controls.
	vector<CControlWnd *> vSelCtrls;
	CControlWnd *pWnd = (CControlWnd *)GetWindow(GW_CHILD);
	while (pWnd != NULL)
	{
		if (pWnd->IsSelected())
		{
			vSelCtrls.push_back(pWnd);
		}

		pWnd = (CControlWnd *)pWnd->GetWindow(GW_HWNDNEXT);
	}

	size_t nSelCount = vSelCtrls.size();
	if (nSelCount < 1)
	{
		return -1;
	}

	// Add to undo list
	::CAction action;
	action.m_eAction = ::CAction::actionMoveControl;
	action.m_pUIWndProp = m_pUIWndProperties;

	// Base the rcFocus
	ASSERT(m_pCtrlFocus != NULL);
	CRect rcFocus = m_pCtrlFocus->m_rcControl;

	for (size_t i = 0; i < nSelCount; ++i)
	{
		CControlWnd *pCtrl = vSelCtrls[i];
		if (pCtrl->GetControlProperties() != m_pCtrlFocus)
		{
			CRect rcCtrl;
			pCtrl->GetWindowRect(rcCtrl);
			ScreenToClient(rcCtrl);

			CControlMove cm;
			cm.m_pCtrlProp = pCtrl->GetControlProperties();
			cm.m_rectOld = rcCtrl;

			if (enumMask == MSS_WIDTH)
			{
				rcCtrl.right = rcCtrl.left + rcFocus.Width();
			}
			else if (enumMask == MSS_HEIGHT)
			{
				rcCtrl.bottom = rcCtrl.top + rcFocus.Height();
			}
			else if (enumMask == MSS_SIZE)
			{
				rcCtrl.right = rcCtrl.left + rcFocus.Width();
				rcCtrl.bottom = rcCtrl.top + rcFocus.Height();
			}

			pCtrl->MoveWindow(rcCtrl);

			//
			cm.m_rectNew = rcCtrl;
			action.m_ControlMove.AddHead(cm);
		}
	}

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

	// Add to undo list
	g_pFrm->GetUIShopDoc()->AddUndo(&action);

	return 0;
}

CControlWnd *CUIDesignWnd::AddNewControl(const RECT &rect, CONTROL_TYPE eControlType, CTRLPROPERTIES *pCtrlPropAttach/* = NULL*/)
{
	CControlWnd *pCtrl = new CControlWnd;

	ASSERT(pCtrl != NULL);
	pCtrl->SetControlType(eControlType);
	BOOL bRet = pCtrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rect, this, m_nID);
	if (!bRet)
	{
		delete pCtrl;
		pCtrl = NULL;
	}

	NMHDR nmhdr;
	nmhdr.hwndFrom = m_hWnd;
	nmhdr.idFrom = GetDlgCtrlID();
	nmhdr.code = UIVN_ADDNEWCONTROL;
	NMUISHOPVIEW vhdr;
	vhdr.hdr = nmhdr;
	vhdr.pOldCtrlProp = pCtrlPropAttach;
	vhdr.pNewCtrl = pCtrl;
	GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&vhdr));

	m_nID++;

	return pCtrl;
}

int CUIDesignWnd::PasteNewControl(CTRLPROPERTIES *pCtrlProp)
{
	CControlWnd *pCtrl = new CControlWnd;

	ASSERT(pCtrl != NULL);
	pCtrl->SetControlType((CONTROL_TYPE)pCtrlProp->m_eControlType);
	pCtrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, pCtrlProp->m_rcControl, this, m_nID);

	NMHDR nmhdr;
	nmhdr.hwndFrom = m_hWnd;
	nmhdr.idFrom = GetDlgCtrlID();
	nmhdr.code = UIVN_PASTENEWCONTROL;
	NMUISHOPVIEW vhdr;
	memset(&vhdr, 0, sizeof(vhdr));
	vhdr.hdr = nmhdr;
	vhdr.pNewCtrl = pCtrl;
	vhdr.pOldCtrlProp = pCtrlProp;
	GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&vhdr));

	// Init the properties
	LONG lCount = pCtrl->GetControlProperties()->GetPropertyCount();
	LONG lCount2 = pCtrlProp->GetPropertyCount();
	if (lCount != lCount2)
	{
		return -1;
	}

	for (LONG i = 0; i < lCount; ++i)
	{
		IUI_VARIANT *pVar = pCtrl->GetControlProperties()->GetProperty(i);
		IUI_VARIANT *pVar2 = pCtrlProp->GetProperty(i);

		*pVar = *pVar2;
	}

	m_nID++;

	return 0;
}

int CUIDesignWnd::DeleteControl(CControlWnd *pCtrlWnd, BOOL bDeleteCTRLPROPERTIES)
{
	size_t nIndex = -1;
	size_t nCount = m_listCtrl.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		if (pCtrlWnd == m_listCtrl[i])
		{
			nIndex = i;
			break;
		}
	}

	if (nIndex == -1)
	{
		ASSERT(FALSE);
		return -1;
	}

	delete pCtrlWnd;
	vector<CControlWnd *>::iterator ite = m_listCtrl.begin() + nIndex;
	m_listCtrl.erase(ite);

	// Don't delete CTRLPROPERTIES for Undo operator
	if (bDeleteCTRLPROPERTIES)
	{
		CTRLPROPERTIES *pCtrlProp = m_pUIWndProperties->m_vControls[nIndex];
		delete pCtrlProp;
	}
	vector<CTRLPROPERTIES *>::iterator it = GetUIWndProperties()->m_vControls.begin() + nIndex;
	GetUIWndProperties()->m_vControls.erase(it);

	return 0;
}

int CUIDesignWnd::GetSelectedControlsCount()
{
	vector<CControlWnd *> vControls;
	GetSelectedControls(&vControls);

	return (int)vControls.size();
}

int CUIDesignWnd::HighlightControl(const CTRLPROPERTIES *pCtrlProp)
{
	size_t nCount = m_listCtrl.size();
	for (int i = 0; i < nCount; ++i)
	{
		CControlWnd *pWnd = m_listCtrl[i];

		if (pWnd->GetControlProperties() == pCtrlProp)
		{
			pWnd->HighlightControl();

			break;
		}
	}

	return 0;
}

void CUIDesignWnd::DrawResizingLine(CDC *pMemDC)
{
	CRect rcUIWnd = m_pUIWndProperties->m_rcControl;
	int nWidth = rcUIWnd.Width();
	int nHeight = rcUIWnd.Height();

	POINT ptResize = m_pUIWndProperties->m_bWindowResizePointSameAsImageResizePoint ? m_pUIWndProperties->m_ptImageResize : m_pUIWndProperties->m_ptWindowResize;
	int nX = ptResize.x;
	int nY = ptResize.y;

	CPen pen;
	pen.CreatePen(PS_DOT, 1, RGB(255, 0, 255));
	CPen *pPenOld = pMemDC->SelectObject(&pen);
	switch (m_nCurZoomPercent)
	{
	case 1:
	case 2:
	case 4:
	case 6:
	case 8:
		pMemDC->MoveTo(0, nY);
		pMemDC->LineTo(nWidth, nY);

		pMemDC->MoveTo(nX, 0);
		pMemDC->LineTo(nX, nHeight);
		break;
	default:
		break;
	}
	pMemDC->SelectObject(pPenOld);
	pen.DeleteObject();
}

void CUIDesignWnd::DrawClientArea(CDC *pMemDC)
{
	int nWidth = 0;
	int nHeight = 0;

	if (m_himgBk->GetSafeHBITMAP() == NULL)
	{
		CRect rcUIWnd = m_pUIWndProperties->m_rcControl;
		nWidth = rcUIWnd.Width();
		nHeight = rcUIWnd.Height();
	}
	else
	{
		nWidth = m_himgBk->GetWidth();
		nHeight = m_himgBk->GetHeight();
	}

	CRect rcClientArea = m_pUIWndProperties->m_rcClientArea;
	CPen pen;
	pen.CreatePen(PS_DOT, 1, RGB(0, 0, 255));
	CPen *pPenOld = pMemDC->SelectObject(&pen);
	switch (m_nCurZoomPercent)
	{
	case 1:
	case 2:
	case 4:
	case 6:
	case 8:
	{
		pMemDC->MoveTo(rcClientArea.left, rcClientArea.top);
		pMemDC->LineTo(rcClientArea.right, rcClientArea.top);
		pMemDC->LineTo(rcClientArea.right, rcClientArea.bottom);
		pMemDC->LineTo(rcClientArea.left, rcClientArea.bottom);
		pMemDC->LineTo(rcClientArea.left, rcClientArea.top);
	}
	break;
	default:
		break;
	}
	pMemDC->SelectObject(pPenOld);
	pen.DeleteObject();
}

void CUIDesignWnd::DrawSelectedFlag(CDC *pMemDC)
{
	CRect rcUIWnd = m_pUIWndProperties->m_rcControl;

	const int nSelectedFlagSize = 5;

	CRect rcLeft(rcUIWnd);
	rcLeft.right = rcLeft.left + nSelectedFlagSize;
	rcLeft.top = rcLeft.top + rcLeft.Height() / 2 - nSelectedFlagSize / 2;
	rcLeft.bottom = rcLeft.top + nSelectedFlagSize;
	pMemDC->Draw3dRect(rcLeft, RGB(0, 0, 0), RGB(0, 0, 0));

	CRect rcTop(rcUIWnd);
	rcTop.left = rcTop.left + rcTop.Width() / 2 - nSelectedFlagSize / 2;
	rcTop.right = rcTop.left + nSelectedFlagSize;
	rcTop.bottom = rcTop.top + nSelectedFlagSize;
	pMemDC->Draw3dRect(rcTop, RGB(0, 0, 0), RGB(0, 0, 0));

	CRect rcRight(rcUIWnd);
	rcRight.top = rcRight.top + rcRight.Height() / 2 - nSelectedFlagSize / 2;
	rcRight.bottom = rcRight.top + nSelectedFlagSize;
	rcRight.left = rcRight.right - nSelectedFlagSize;
	pMemDC->FillSolidRect(rcRight, RGB(0, 0, 0));

	CRect rcBottom(rcUIWnd);
	rcBottom.left = rcBottom.left + rcBottom.Width() / 2 - nSelectedFlagSize / 2;
	rcBottom.right = rcBottom.left + nSelectedFlagSize;
	rcBottom.top = rcBottom.bottom - nSelectedFlagSize;
	pMemDC->FillSolidRect(rcBottom, RGB(0, 0, 0));

	CRect rcLeftTop(rcUIWnd);
	rcLeftTop.right = rcLeftTop.left + nSelectedFlagSize;
	rcLeftTop.bottom = rcLeftTop.top + nSelectedFlagSize;
	pMemDC->Draw3dRect(rcLeftTop, RGB(0, 0, 0), RGB(0, 0, 0));

	CRect rcRightTop(rcUIWnd);
	rcRightTop.left = rcRightTop.right - nSelectedFlagSize;
	rcRightTop.bottom = rcRightTop.top + nSelectedFlagSize;
	pMemDC->Draw3dRect(rcRightTop, RGB(0, 0, 0), RGB(0, 0, 0));

	CRect rcLeftDown(rcUIWnd);
	rcLeftDown.right = rcLeftDown.left + nSelectedFlagSize;
	rcLeftDown.top = rcLeftDown.bottom - nSelectedFlagSize;
	pMemDC->Draw3dRect(rcLeftDown, RGB(0, 0, 0), RGB(0, 0, 0));

	CRect rcRightDown(rcUIWnd);
	rcRightDown.left = rcRightDown.right - nSelectedFlagSize;
	rcRightDown.top = rcRightDown.bottom - nSelectedFlagSize;
	pMemDC->FillSolidRect(rcRightDown, RGB(0, 0, 0));
}

int CUIDesignWnd::SetCurStoryFrame(int nFrame)
{
	m_nCurStoryFrame = nFrame;

	CTRLPROPERTIES *pNewFocusProp = m_pCtrlFocus;

	// If current frame not exist, auto create it.
	STORY_FRAME *pCurStoryFrame = NULL;
	if (nFrame > -1)
	{
		pCurStoryFrame = GetUIWndProperties()->FindStoryFrame(nFrame);
		if (pCurStoryFrame == NULL)
		{
			pCurStoryFrame = g_pFrm->InsertFrame();
		}

		if (pCurStoryFrame == NULL)
		{
			ASSERT(FALSE);
			return -1;
		}
	}

	// Update view to story frame
	size_t nCtrlCount = m_listCtrl.size();
	for (int i = 0; i < nCtrlCount; ++i)
	{
		CControlWnd *pWnd = m_listCtrl[i];
		LPCTSTR lpszCtrlID = pWnd->GetControlProperties()->m_strID;
		CTRLPROPERTIES *pCtrlProp = NULL;
		if (nFrame > -1)
		{
			pCtrlProp = pCurStoryFrame->FindControl(lpszCtrlID);

			// If current frame not include the control, create it.
			if (pCtrlProp == NULL)
			{
				pCtrlProp = GetUIWndProperties()->AllocCtrlPropertiesAtStoryFrame(
						nFrame, lpszCtrlID);
				ASSERT(pCtrlProp != NULL);
				pCurStoryFrame->m_vStoryFrameCtrlItem.push_back(pCtrlProp);
			}
		}
		else
		{
			pCtrlProp = GetUIWndProperties()->GetDlgItem(lpszCtrlID);
		}
		ASSERT(pCtrlProp != NULL);

		::ApplyStoryboardItem(pWnd, TRUE, pCtrlProp);

		if (pWnd->GetControlProperties() == m_pCtrlFocus)
		{
			pNewFocusProp = pCtrlProp;
		}
	}

	// 发送焦点改变通知
	NMHDR nmhdr;
	nmhdr.hwndFrom = m_hWnd;
	nmhdr.idFrom = GetDlgCtrlID();
	nmhdr.code = UIVN_FOCUSCHANGED;	// 在这个消息处理函数中，会更新Properties窗口中的属性。
	NMUISHOPVIEW vhdr;
	vhdr.hdr = nmhdr;
	vhdr.pOldCtrlProp = m_pCtrlFocus;
	vhdr.pNewCtrlProp = pNewFocusProp;
	CWnd *pParent = GetParent();
	pParent->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&vhdr));

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

	return 0;
}

int CUIDesignWnd::GetCurStoryFrame() const
{
	return m_nCurStoryFrame;
}

BOOL CUIDesignWnd::SetBkBitmap()
{
	if (m_himgBk->GetSafeHBITMAP() != NULL)
	{
		ReleaseIUIImage(m_himgBk);
	}

	m_himgBk = CreateIUIImage((LPCTSTR)m_pUIWndProperties->m_strBKNo);
	if (m_himgBk->GetSafeHBITMAP() == NULL)
	{
		TRACE(_T("[LibUIDK]===Failed to load background no controls image file \"%s\" of window \"%s\"\n"),
			(LPCTSTR)m_pUIWndProperties->m_strBKNo, (LPCTSTR)m_pUIWndProperties->m_strID);
		return FALSE;
	}

	m_eBkImageResizeMode = m_pUIWndProperties->m_eBkImageResizeMode;
	m_ptBkImageResize = m_pUIWndProperties->m_ptImageResize;

	InvalidateRect(NULL);

	return TRUE;
}

int CUIDesignWnd::MoveSelectedControls(CControlWnd *pSender, int nXOffset, int nYOffset)
{
	vector<CControlWnd *> vSelectedControls;
	GetSelectedControls(&vSelectedControls);
	size_t nSelCount = vSelectedControls.size();
#ifdef _WIN64
	ASSERT(nSelCount >> 32 == 0);
#endif
	HDWP hdwp = BeginDeferWindowPos(int(nSelCount));
	if (hdwp == NULL)
	{
		return -1;
	}

	for (size_t i = 0; i < nSelCount; ++i)
	{
		CControlWnd *pCtrlWnd = vSelectedControls[i];
		if (pCtrlWnd != pSender)
		{
			CRect rcOld;
			pCtrlWnd->GetWindowRect(rcOld);
			ScreenToClient(rcOld);

			if (NULL == DeferWindowPos(hdwp, pCtrlWnd->m_hWnd, NULL, rcOld.left + nXOffset, rcOld.top + nYOffset, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER))
			{
				return -1;
			}
		}
	}

	EndDeferWindowPos(hdwp);

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

	return 0;
}

int CUIDesignWnd::GetControlIndex(const CTRLPROPERTIES *pCtrlProp)
{
	size_t nChildCount = m_pUIWndProperties->m_vControls.size();

	int nIndex = -1;
	for (int i = 0; i < nChildCount; ++i)
	{
		CTRLPROPERTIES *p = m_pUIWndProperties->m_vControls[i];
		if (p == pCtrlProp)
		{
			nIndex = i;
			break;
		}
	}

	return nIndex;
}
