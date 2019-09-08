// WLButton.cpp : implementation file
//

#include "stdafx.h"
#include "ResourceMgr.h"
#include "ControlMember.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CWLButton

CWLButton::CWLButton()
{
	CButtonBase::m_bWLWnd = true;
	CButtonBase::m_pBindWLWnd = this;

	BUTTONMEMBER *pMember = (BUTTONMEMBER *)CButtonBase::m_pMember;

	pMember->m_bWLButton = TRUE;
}

CWLButton::~CWLButton()
{
}


BOOL CWLButton::Create(LPCTSTR lpszControlName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CWLWnd *pVirtualParent)
{
	BOOL bRet = CWLWnd::Create(IUIWL_Button, lpszControlName, dwStyle,
		rect, pParentWnd, nID, pVirtualParent);

	if (!bRet)
	{
		return FALSE;
	}

	BUTTONMEMBER *pMember = (BUTTONMEMBER *)CButtonBase::m_pMember;
	if ((dwStyle & BS_AUTOCHECKBOX) == BS_AUTOCHECKBOX)	// Check Box
	{
		pMember->m_eButtonControlType = CT_CHECK;
	}
	else if ((dwStyle & BS_AUTORADIOBUTTON) == BS_AUTORADIOBUTTON)	// Radio button
	{
		pMember->m_eButtonControlType = CT_RADIO;
	}
	else	// Push button
	{
		pMember->m_eButtonControlType = CT_PUSHBUTTON;
	}

	return TRUE;
}

int CWLButton::ReleaseObject()
{
	return CButtonBase::ReleaseObject();
}

BOOL CWLButton::IsCreated() const
{
	return CWLWnd::IsCreated();
}

//////////////////////////////////////////////////////////////////////////
// virtual

int CWLButton::OnDraw(CDC *pMemDCParent)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)CButtonBase::m_pMember;

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

	// Don't draw this control if it's out of the parent
	CRect rcParent;
	GetParent()->GetClientRect(rcParent);

	CRect rcIntersect;
	rcIntersect.IntersectRect(rcWin, rcParent);
	if (rcIntersect.IsRectEmpty())
	{
		return 0;
	}

	GetParent()->SendMessage(WM_BEFOREWLDRAW, (WPARAM)this, (LPARAM)pMemDCParent);

	if (CButtonBase::IsDrawBkColor())
	{
		pMemDCParent->FillSolidRect(rcWin, CButtonBase::GetBackgroundColor());
	}

	CFont *pFontOld = NULL;

	CString strText;
	GetWindowText(strText);

	// load background bitmap, and set font,color and offset
	CRect rcText(rcWin);

	BOOL bEnable = IsWindowEnabled();
	BOOL bSelected = GetKeyState(VK_LBUTTON) < 0;

	int nStateIndex = 0;
	if (!bEnable)
	{
		nStateIndex = 3;
	}
	else
	{
		if (bSelected && pMember->m_bRecordLButtonDown)
		{
			if (pMember->m_bMouseOver)
			{
				nStateIndex = 2;
			}
			else
			{
				nStateIndex = 1;
			}
		}
		else
		{
			if (pMember->m_bMouseOver)
			{
				nStateIndex = 1;
			}
			else
			{
				nStateIndex = 0;
			}
		}
	}

	if (pMember->m_nCheck == BST_CHECKED)
	{
		DrawButtonState(rcWin,
			pMemDCParent,
			nStateIndex,
			pMember->m_hbmpCheckedMask[nStateIndex],
			IRM_STRETCH,
			pMember->m_rcForegroundMargin,
			pMember->m_eForegroundAlignHorC,
			pMember->m_eForegroundAlignVerC,
			pMember->m_crMaskC,
			TRUE);

		::DrawIconEx(pMemDCParent->GetSafeHdc(), 0, 0, pMember->m_hCheckedIcon, 0, 0, 1, NULL, DI_NORMAL);

		// draw check box or radio button's text
		DrawFormatText(pMemDCParent->GetSafeHdc(),
			strText,
			strText.GetLength(),
			rcWin,
			pMember->m_rcTextMarginC[nStateIndex],
			pMember->m_eTextHorAlignModeC[nStateIndex],
			pMember->m_eTextVerAlignModeC[nStateIndex],
			0,
			GetHFont(pMember->m_hIUIFontC[nStateIndex]),
			pMember->m_crC[nStateIndex],
			CT_WL_BUTTON);
	}
	else // Unchecked
	{
		DrawButtonState(
			rcWin,
			pMemDCParent,
			nStateIndex,
			pMember->m_hbmpMask[nStateIndex],
			IRM_STRETCH,
			pMember->m_rcForegroundMargin,
			pMember->m_eForegroundAlignHor,
			pMember->m_eForegroundAlignVer,
			pMember->m_crMask,
			FALSE);

		::DrawIconEx(pMemDCParent->GetSafeHdc(), 0, 0, pMember->m_hIcon, 0, 0, 1, NULL, DI_NORMAL);

		// draw push button's text
		DrawFormatText(
			pMemDCParent->GetSafeHdc(),
			strText,
			strText.GetLength(),
			rcWin,
			pMember->m_rcTextMargin[nStateIndex],
			pMember->m_eTextHorAlignMode[nStateIndex],
			pMember->m_eTextVerAlignMode[nStateIndex],
			0,
			GetHFont(pMember->m_hIUIFont[nStateIndex]),
			pMember->m_cr[nStateIndex],
			CT_WL_BUTTON);
	}

	GetParent()->SendMessage(WM_AFTERWLDRAW, (WPARAM)this, (LPARAM)pMemDCParent);

	// 	// draw focus rect
	// 	if (m_bTabStop && (GetFocus() == this))
	// 	{
	// 		pMemDCParent->DrawFocusRect(rcWin);
	// 	}

	return 0;
}

// 收集本组所有Radio Button
int GetThisGroupRadioButton(CWLButton *pRadChecked, std::vector<CWLButton *> *pvRadios)
{
	if (pRadChecked == NULL || pvRadios == NULL)
	{
		return -1;
	}

	pvRadios->clear();

	pvRadios->push_back(pRadChecked);

	//
	// 1. 向前查询
	//

	// 先确认自己是不是Group风格
	DWORD dwStyle = pRadChecked->GetStyle();
	BOOL bGroup = IsIncludeFlag(dwStyle, WS_GROUP);
	if (!bGroup)
	{
		// 如果自己不是Group风格，向前查询，直到找到第一个Group风格的Radio Button为止。
		// 如果自己之前没有Radio Button，则自己当成同组第一个Radio Button
		// 如果自己之前有Radio Button，但是没有任何一个有Group风格，则第一个当成同组第一个Radio.

		CWLWnd *pPrevWnd = GetNextWLWindow(pRadChecked, GW_HWNDPREV);
		while (pPrevWnd != NULL)
		{
			if (pPrevWnd->GetClassName() == IUIWL_Button)
			{
				if (IsIncludeFlag(pPrevWnd->GetStyle(), BS_AUTORADIOBUTTON))
				{
					pvRadios->insert(pvRadios->begin(), (CWLButton *)pPrevWnd);

					if (IsIncludeFlag(pPrevWnd->GetStyle(), WS_GROUP))
					{
						break;
					}
				}
			}

			pPrevWnd = GetNextWLWindow(pPrevWnd, GW_HWNDPREV);
		}
	}

	//
	// 2. 向后查找
	//
	CWLWnd *pNextWnd = GetNextWLWindow(pRadChecked, GW_HWNDNEXT);
	while (pNextWnd != NULL)
	{
		if (pNextWnd->GetClassName() == IUIWL_Button)
		{
			if (IsIncludeFlag(pNextWnd->GetStyle(), BS_AUTORADIOBUTTON))
			{
				if (IsIncludeFlag(pNextWnd->GetStyle(), WS_GROUP))
				{
					break;
				}

				pvRadios->push_back((CWLButton *)pNextWnd);
			}
		}

		pNextWnd = GetNextWLWindow(pNextWnd, GW_HWNDNEXT);
	}

	return 0;
}

// 仅选中pRadChecked控件，并且把同组RadioButton反选
int OnlyCheck(CWLButton *pRadChecked)
{
	std::vector<CWLButton *> vRadios;
	GetThisGroupRadioButton(pRadChecked, &vRadios);

	std::vector<CWLButton *>::iterator it = vRadios.begin();
	for (; it != vRadios.end(); ++it)
	{
		if (*it == pRadChecked)
		{
			(*it)->SetCheck(BST_CHECKED);
		}
		else
		{
			(*it)->SetCheck(BST_UNCHECKED);
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CWLButton::WLWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	BUTTONMEMBER *pMember = (BUTTONMEMBER *)CButtonBase::m_pMember;

	if (message == WM_LBUTTONDOWN)
	{
		pMember->m_bRecordLButtonDown = TRUE;
		Invalidate();
		GetParent()->UpdateWindow();
	}

	else if (message == WM_LBUTTONUP)
	{
		if (pMember->m_bRecordLButtonDown)
		{
			pMember->m_bMouseOver = FALSE;	// before send BN_CLICKED, reset the mouse over state.
		}

		if (pMember->m_bRecordLButtonDown)
		{
			if (pMember->m_eButtonControlType == CT_CHECK)
			{
				if (pMember->m_nCheck == BST_UNCHECKED)
				{
					pMember->m_nCheck = BST_CHECKED;
				}
				else if (pMember->m_nCheck == BST_CHECKED)
				{
					pMember->m_nCheck = BST_UNCHECKED;
				}
			}
			else if (pMember->m_eButtonControlType == CT_RADIO)
			{
				OnlyCheck(this);
			}
		}

		Invalidate();
		GetParent()->UpdateWindow();

		if (pMember->m_bRecordLButtonDown)
		{
			BOOL bRet = (BOOL)GetParent()->SendMessage(WM_WL_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), LPARAM(this));
			if (bRet)
			{
				pMember->m_bRecordLButtonDown = FALSE;
				return bRet;
			}
		}

		pMember->m_bRecordLButtonDown = FALSE;
	}

	else if (message == WM_MOUSEMOVE)
	{
		if (pMember->m_bRecordLButtonDown)
		{
			CRect rcClient;
			GetClientRect(rcClient);

			CPoint point(lParam);

			if (!rcClient.PtInRect(point))
			{
				if (pMember->m_bMouseOver)
				{
					pMember->m_bMouseOver = FALSE;
					Invalidate();
				}
			}
			else
			{
				if (!pMember->m_bMouseOver)
				{
					pMember->m_bMouseOver = TRUE;
					Invalidate();
				}
			}
		}
		else
		{
			if (!pMember->m_bMouseOver)
			{
				pMember->m_bMouseOver = TRUE;

				// when set RGN for highlight stats, show the tool tip when mouse over the button(the tool tip and highlight stats at the same time work).
				// don't call Invalidate() after this "if" section
				Invalidate();
			}
		}
	}

	else if (message == WM_MOUSELEAVE)
	{
		pMember->m_bMouseOver = FALSE;
		Invalidate();
	}

	else if (message == WM_MOUSEACTIVATE)
	{
		Invalidate();
	}

	else if (message == BM_GETCHECK)
	{
		return pMember->m_nCheck;
	}
	else if (message == BM_SETCHECK)
	{
		pMember->m_nCheck = (int)wParam;
		return 0;
	}

	return CRectCtrl::WLWindowProc(message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
// public

int CWLButton::GetCheck() const
{
	ASSERT(IsCreated());
	return (int)((CWLButton *)this)->WLWindowProc(BM_GETCHECK, 0, 0);
}

void CWLButton::SetCheck(int nCheck)
{
	ASSERT(IsCreated());
	WLWindowProc(BM_SETCHECK, nCheck, 0);
	Invalidate();
}
