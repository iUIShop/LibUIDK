// TaskWndMgr.cpp: implementation of the CTaskWndMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <map>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


int DestroyTaskWindowSendMessage(CTaskWndMgr *pThis, CTaskWndInfo *pTaskInfo)
{
	if (pThis == NULL || pTaskInfo == NULL)
	{
		return -1;
	}

	if (pTaskInfo->m_pTaskWnd == NULL)
	{
		return -2;
	}

	CWnd *pParentWnd = pThis->GetParent();
	ASSERT(::IsWindow(pParentWnd->GetSafeHwnd()));
	pParentWnd->SendMessage(WM_TASKWNDDESTROYING,
		pThis->GetDlgCtrlID(), (LPARAM)pTaskInfo);

	if (pTaskInfo->m_bRadioButton && pTaskInfo->m_pRadTab != NULL)
	{
		pTaskInfo->m_pRadTab->DestroyWindow();
		SafeDelete(pTaskInfo->m_pRadTab);
	}

	pTaskInfo->m_pTaskWnd->DestroyWindow();

	pParentWnd->SendMessage(WM_TASKWNDDESTROYED,
		pThis->GetDlgCtrlID(), (LPARAM)pTaskInfo);

	return 0;
}

int ReleaseNoUICodeTask(CTaskWndMgr *pThis, CTaskWndInfo *pTaskInfo)
{
	if (pTaskInfo == NULL)
	{
		return -1;
	}

	CUIWnd *pUITaskWnd = (CUIWnd *)pTaskInfo->m_pTaskWnd;
	if (pUITaskWnd == NULL || !pUITaskWnd->IsNoCodeUIMode())
	{
		return -3;
	}

	// Store the parent window before destroy.
	CWnd *pParent = NULL;
	if (pUITaskWnd->GetSafeHwnd() != NULL)
	{
		pParent = pUITaskWnd->GetParent();
	}

	// The handler may be handle some messages,
	// so, first destroy window, then release message handler.
	if (pUITaskWnd->GetSafeHwnd() != NULL)
	{
		DestroyTaskWindowSendMessage(pThis, pTaskInfo);
	}

	//
	// Give user a chance to release memory.
	//
	CHILDMSGHANDLER cmh;
	cmh.pUIWnd = pUITaskWnd;
	cmh.pUIWndHandler = pUITaskWnd->m_pUIWndHandler;

	if (IsWindow(pParent->GetSafeHwnd()))
	{
		BOOL bRet = (BOOL)pParent->SendMessage(WM_RELEASECHILDMSGHANDLER,
				pUITaskWnd->GetIDD(), LPARAM(&cmh));
		if (bRet)
		{
			pUITaskWnd->m_pUIWndHandler = NULL;
		}
	}

	delete pTaskInfo->m_pTaskWnd;
	pTaskInfo->m_pTaskWnd = NULL;

	return 0;
}

int RelayoutTaskWindow(LPCRECT lprcTaskMgr, const CRect &rcTaskWndOriginal,
	CONTROL_LAYOUT_HOR eHorLayoutMode, CONTROL_LAYOUT_VER eVerLayoutMode,
	LPCRECT lprcLayoutMargin,
	/*out*/ CRect &rcNew)
{
	if (lprcTaskMgr == NULL)
	{
		return -1;
	}

	rcNew = lprcTaskMgr;
	rcNew.DeflateRect(lprcLayoutMargin);

	CRect rcUIWndOrig = lprcTaskMgr;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rcUIWndOrig);
	}

	// Horizontal
	if (eHorLayoutMode == CLH_LEFT
		|| eHorLayoutMode == CLH_FREE)
	{
		rcNew.right = rcNew.left + rcTaskWndOriginal.Width();
	}
	else if (eHorLayoutMode == CLH_RIGHT)
	{
		rcNew.left = rcNew.right - rcTaskWndOriginal.Width();
	}
	else if (eHorLayoutMode == CLH_CENTER)
	{
		rcNew.left += (rcNew.Width() - rcTaskWndOriginal.Width()) / 2;
		rcNew.right = rcNew.left + rcTaskWndOriginal.Width();
	}

	// Vertical
	if (eVerLayoutMode == CLV_TOP
		|| eVerLayoutMode == CLV_FREE)
	{
		rcNew.bottom = rcNew.top + rcTaskWndOriginal.Height();
	}
	else if (eVerLayoutMode == CLV_BOTTOM)
	{
		rcNew.top = rcNew.bottom - rcTaskWndOriginal.Height();
	}
	else if (eVerLayoutMode == CLV_CENTER)
	{
		rcNew.top += (rcNew.Height() - rcTaskWndOriginal.Height()) / 2;
		rcNew.bottom = rcNew.top + rcTaskWndOriginal.Height();
	}

	return 0;
}


struct TASKWNDMGRMEMBER
{
	TASKWNDMGRMEMBER()
	{
		nCurrentTaskWndID = -1;

		bSmooth = FALSE;
		nSmoothSpeed =  10;
		nSmoothInterval = 100;
	}

	int Release()
	{
		nCurrentTaskWndID = -1;
		vTaskWnd.clear();

		bSmooth = FALSE;
		nSmoothSpeed =  10;
		nSmoothInterval = 100;
		return 0;
	}

	int nCurrentTaskWndID;
	std::vector<CTaskWndInfo> vTaskWnd;		// The task has order, so use vector instead of map.

	// For smooth move
	BOOL bSmooth;			// Smooth to switch the task window
	int nSmoothSpeed;		// Move pixels each step.
	int nSmoothInterval;	// The interval of SetTimer. The unit is ms.
};

//////////////////////////////////////////////////////////////////////////
// CTaskWndInfo

CTaskWndInfo::CTaskWndInfo()
{
	m_bRadioButton = FALSE;
	m_nRadioButtonID = -1;
	m_pRadTab = NULL;
	m_bUINoCodeMode = FALSE;
	m_pTaskWnd = NULL;
	m_nTaskWndID = -1;
	m_dwTaskWndCreateStyle = WS_TASKSTYLE;
}

CTaskWndInfo::~CTaskWndInfo()
{

}

CTaskParam::CTaskParam()
{
	m_pTaskWndMgr = NULL;
	m_pSwitchTo = NULL;
	m_pTobeHide = NULL;
}

CTaskParam::~CTaskParam()
{
}

//////////////////////////////////////////////////////////////////////
// CTaskWndMgr

struct TASK_TIMER_STRUCT
{
	CTaskWndInfo *pSwitchTo;
	CTaskWndInfo *pTobeHide;
	CRect rcTaskPos;
	CTaskWndMgr *pTaskWndMgr;
	TASKWNDMGRMEMBER *pTaskWndMgrMember;
};

VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if (hwnd == NULL)
	{
		return;
	}

	CUIWnd *pParentUIWnd = (CUIWnd *)CWnd::FromHandle(hwnd);
	BOOL bUIWnd = (BOOL)pParentUIWnd->SendMessage(WM_ISUIWND);
	if (!bUIWnd)
	{
		return;
	}

	if (idEvent != TIMER_ID_TASKWNDMGR)
	{
		return;
	}

	TASK_TIMER_STRUCT *ptts = (TASK_TIMER_STRUCT *)GetProp(hwnd, GETPROP_TASKWNDMGR_TIMERSTRUCT);
	if (ptts == NULL)
	{
		return;
	}

	CWnd *pWndSwitchTo = ptts->pSwitchTo->m_pTaskWnd;
	CWnd *pWndToBeHide = ptts->pTobeHide->m_pTaskWnd;
	CRect rcTaskPos = ptts->rcTaskPos;
	TASKWNDMGRMEMBER *pTaskWndMgrMember = ptts->pTaskWndMgrMember;

	// Get to be hide window rect.
	CRect rcTaskToBeHide;
	pWndToBeHide->GetWindowRect(rcTaskToBeHide);
	pParentUIWnd->ScreenToClient(rcTaskToBeHide);

	CRect rcTaskSwitchTo(rcTaskToBeHide);
	rcTaskSwitchTo.DeflateRect(rcTaskPos.Width(), 0, -rcTaskPos.Width(), 0);

	// Last move step
	int nSmoothSpeed = pTaskWndMgrMember->nSmoothSpeed;
	CRect rcWnd;
	ptts->pTaskWndMgr->GetWindowRect(rcWnd);
	pParentUIWnd->ScreenToClient(rcWnd);
	int nLastWidth = rcTaskToBeHide.right - rcWnd.left;
	if (nLastWidth <= pTaskWndMgrMember->nSmoothSpeed && nLastWidth > 0)
	{
		nSmoothSpeed = nLastWidth;
	}

	rcTaskToBeHide.DeflateRect(-nSmoothSpeed, 0, nSmoothSpeed, 0);
	rcTaskSwitchTo.DeflateRect(-nSmoothSpeed, 0, nSmoothSpeed, 0);

	pWndToBeHide->MoveWindow(rcTaskToBeHide);
	pWndSwitchTo->MoveWindow(rcTaskSwitchTo);

	// Release memory at last step
	if (rcTaskSwitchTo.right <= rcTaskPos.right)
	{
		KillTimer(hwnd, TIMER_ID_TASKWNDMGR);

#ifdef _DEBUG
		ASSERT(rcTaskSwitchTo == rcTaskPos);
#endif

		// Send message
		CTaskParam tp;
		tp.m_pTaskWndMgr = ptts->pTaskWndMgr;
		tp.m_pSwitchTo = ptts->pSwitchTo;
		tp.m_pTobeHide = ptts->pTobeHide;
		SendMessage(hwnd, WM_TASKWNDSWITCHED, ptts->pTaskWndMgr->GetDlgCtrlID(), (LPARAM)&tp);

		SafeDelete(ptts);
		SetProp(hwnd, GETPROP_TASKWNDMGR_TIMERSTRUCT, NULL);
	}
}


CTaskWndMgr::CTaskWndMgr()
{
	m_pMember = new TASKWNDMGRMEMBER;
}

CTaskWndMgr::~CTaskWndMgr()
{
	RemoveAllTaskWindows(FALSE);

	if (m_pMember != NULL)
	{
		delete (TASKWNDMGRMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

int CTaskWndMgr::ReleaseObject()
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	pMember->nCurrentTaskWndID = -1;

	return 0;
}

BOOL CTaskWndMgr::DestroyWindow()
{
	CWnd *pParent = GetParent();
	ASSERT(pParent != NULL);
	HWND hWndParent = pParent->GetSafeHwnd();
	ASSERT(::IsWindow(hWndParent));

	TASK_TIMER_STRUCT *ptts = (TASK_TIMER_STRUCT *)GetProp(hWndParent, GETPROP_TASKWNDMGR_TIMERSTRUCT);
	SafeDelete(ptts);

	RemoveAllTaskWindows(FALSE);

	BOOL bRet = CWLPicture::DestroyWindow();

	return bRet;
}

int CTaskWndMgr::AddTaskWindow(
	UINT nTaskRadioButtonID,
	BOOL bRadioButton,
	UINT uTaskWndID,
	LPCTSTR lpszWindowName/* = NULL*/,
	DWORD dwStyle/* = WS_TASKSTYLE*/)
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	CTaskWndInfo twi;
	twi.m_bRadioButton = bRadioButton;
	twi.m_nRadioButtonID = nTaskRadioButtonID;
	twi.m_bUINoCodeMode = TRUE;
	twi.m_pTaskWnd = NULL;
	twi.m_nTaskWndID = uTaskWndID;
	twi.m_dwTaskWndCreateStyle = dwStyle;
	if (lpszWindowName != NULL)
	{
		twi.m_strWindowName = lpszWindowName;
	}

	pMember->vTaskWnd.push_back(twi);

	int nCount = (int)pMember->vTaskWnd.size();

	return nCount - 1;
}

int CTaskWndMgr::AddTaskWindow(
	UINT nTaskRadioButtonID,
	BOOL bRadioButton,
	CWnd *pTaskPanel,
	UINT nTaskWndID,
	LPCTSTR lpszWindowName/* = NULL*/,
	DWORD dwStyle/* = WS_TASKSTYLE*/)
{
	if (NULL == pTaskPanel)
	{
		_ASSERT(FALSE);
		return -1;
	}

	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	CTaskWndInfo twi;
	twi.m_bRadioButton = bRadioButton;
	twi.m_nRadioButtonID = nTaskRadioButtonID;
	twi.m_pTaskWnd = pTaskPanel;
	twi.m_nTaskWndID = nTaskWndID;
	twi.m_dwTaskWndCreateStyle = dwStyle;
	if (lpszWindowName != NULL)
	{
		twi.m_strWindowName = lpszWindowName;
	}

	pMember->vTaskWnd.push_back(twi);

	int nCount = (int)pMember->vTaskWnd.size();

	return nCount - 1;
}

int CTaskWndMgr::AddTaskWindow(
	UINT nTaskRadioButtonID,
	const RECT &rcTab,
	LPCTSTR lpszBindTabStyle,
	LPCTSTR lpszTabName,
	UINT uTaskWndID,
	__out CSkinButton **ppRadTab/* = NULL*/,
	LPCTSTR lpszWindowName/* = NULL*/,
	DWORD dwStyle/* = WS_TASKSTYLE*/)
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	if (nTaskRadioButtonID <= 0
		|| lpszBindTabStyle == NULL
		|| uTaskWndID <= 0)
	{
		return -1;
	}

	CTaskWndInfo twi;
	twi.m_bRadioButton = TRUE;
	twi.m_nRadioButtonID = nTaskRadioButtonID;
	twi.m_pRadTab = new CSkinButton;
	twi.m_pRadTab->Create(lpszTabName, WS_VISIBLE | BS_AUTORADIOBUTTON, rcTab, GetParent(), nTaskRadioButtonID);

	if (lpszBindTabStyle != NULL)
	{
		twi.m_pRadTab->BindStyle(lpszBindTabStyle);
	}

	if (ppRadTab != NULL)
	{
		*ppRadTab = twi.m_pRadTab;
	}

	twi.m_bUINoCodeMode = TRUE;
	twi.m_pTaskWnd = NULL;
	twi.m_nTaskWndID = uTaskWndID;
	twi.m_dwTaskWndCreateStyle = dwStyle;
	if (lpszWindowName != NULL)
	{
		twi.m_strWindowName = lpszWindowName;
	}

	pMember->vTaskWnd.push_back(twi);

	int nCount = (int)pMember->vTaskWnd.size();

	return nCount - 1;
}

int CTaskWndMgr::RemoveTaskWindow(UINT nTaskRadioButton, BOOL bDestroyTaskWindow)
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	int nIndex = -1;

	size_t nCount = pMember->vTaskWnd.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		CTaskWndInfo *pTaskInfo = &pMember->vTaskWnd[i];
		if (pTaskInfo->m_nRadioButtonID == nTaskRadioButton)
		{
			if (bDestroyTaskWindow)
			{
				DestroyTaskWindowSendMessage(this, pTaskInfo);

				if (pTaskInfo->m_bRadioButton && pTaskInfo->m_pRadTab != NULL)
				{
					SafeDelete(pTaskInfo->m_pRadTab);
				}

				BOOL bUIWnd = (BOOL)pTaskInfo->m_pTaskWnd->SendMessage(WM_ISUIWND);
				if (bUIWnd)
				{
					CUIWnd *pUIWnd = (CUIWnd *)pTaskInfo->m_pTaskWnd;
					if (pUIWnd->IsNoCodeUIMode())
					{
						ReleaseNoUICodeTask(this, pTaskInfo);
					}
				}
			}

			// 如果不销毁窗口，并且窗口是无代码模式，要把handler从父窗口中移除

			pMember->vTaskWnd.erase(pMember->vTaskWnd.begin() + i);

			if (pMember->nCurrentTaskWndID == nTaskRadioButton)
			{
				pMember->nCurrentTaskWndID = -1;    // Reset
			}

			nIndex = (int)i;
			break;
		}
	}

	return nIndex;
}

int CTaskWndMgr::RemoveAllTaskWindows(BOOL bDestroyTaskWindow)
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	size_t nCount = pMember->vTaskWnd.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		CTaskWndInfo *pTaskInfo = &pMember->vTaskWnd[i];

		if (pTaskInfo->m_pTaskWnd == NULL)
		{
			continue;
		}

		if (bDestroyTaskWindow)
		{
			DestroyTaskWindowSendMessage(this, pTaskInfo);
		}

		// If with UI NO CODE MODE, the task window was created by LIBUIDK
		// So, the task window object still will be free.
		if (::IsWindow(pTaskInfo->m_pTaskWnd->GetSafeHwnd()))
		{
			BOOL bUIWnd = (BOOL)pTaskInfo->m_pTaskWnd->SendMessage(WM_ISUIWND);
			if (bUIWnd)
			{
				CUIWnd *pUIWnd = (CUIWnd *)pTaskInfo->m_pTaskWnd;
				if (pUIWnd->IsNoCodeUIMode())
				{
					ReleaseNoUICodeTask(this, pTaskInfo);
				}
			}
		}
	}

	pMember->vTaskWnd.clear();
	pMember->nCurrentTaskWndID = -1;	// Reset

	return 0;
}

int CTaskWndMgr::DestroyTaskWindow(UINT nTaskRadioButton)
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	int nIndex = -1;

	size_t nCount = pMember->vTaskWnd.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		CTaskWndInfo *pTaskInfo = &pMember->vTaskWnd[i];
		if (pTaskInfo->m_nRadioButtonID == nTaskRadioButton)
		{
			if (pTaskInfo->m_pTaskWnd != NULL)
			{
				// If with UI NO CODE MODE, the task window was created by LIBUIDK
				// So, the task window object still will be free.
				BOOL bNoCode = FALSE;
				BOOL bUIWnd = (BOOL)pTaskInfo->m_pTaskWnd->SendMessage(WM_ISUIWND);
				if (bUIWnd)
				{
					CUIWnd *pUIWnd = (CUIWnd *)pTaskInfo->m_pTaskWnd;
					if (pUIWnd->IsNoCodeUIMode())
					{
						bNoCode = TRUE;
						ReleaseNoUICodeTask(this, pTaskInfo);
					}
				}

				if (!bNoCode)
				{
					DestroyTaskWindowSendMessage(this, pTaskInfo);
				}
			}
			else
			{
				ASSERT(FALSE);
			}

			if (pMember->nCurrentTaskWndID == nTaskRadioButton)
			{
				pMember->nCurrentTaskWndID = -1;
			}

			nIndex = (int)i;
			break;
		}
	}

	return nIndex;
}

int CTaskWndMgr::GetTaskWindowCount()
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	size_t nCount = pMember->vTaskWnd.size();
	return (int)nCount;
}

int CTaskWndMgr::SwitchToTaskWindow(UINT nID, BOOL bSetFocus/* = FALSE*/, BOOL bNotFoundHideAll/* = FALSE*/)
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	if (nID == pMember->nCurrentTaskWndID)
	{
		return 1;
	}

	CTaskWndInfo *pSwitchTo = FindTaskWindow(nID);
	if (pSwitchTo == NULL)
	{
		if (!bNotFoundHideAll)
		{
			return -1;
		}
	}

	CTaskWndInfo *pTobeHide = FindTaskWindow(pMember->nCurrentTaskWndID);

	//
	// 1. Send message WM_TASKWNDSWITCHING to parent.
	//
	if (GetParent() != NULL)
	{
		CTaskParam tp;
		tp.m_pTaskWndMgr = this;
		tp.m_pSwitchTo = pSwitchTo;
		tp.m_pTobeHide = pTobeHide;
		GetParent()->SendMessage(WM_TASKWNDSWITCHING, GetDlgCtrlID(), (LPARAM)&tp);
	}

	//
	// Task position
	//
	CRect rcNewTaskPos;
	GetWindowRect(rcNewTaskPos);
	GetParent()->ScreenToClient(rcNewTaskPos);

	CRect rcTaskWnd = rcNewTaskPos;

	//
	// 2. Check task radio button
	//
	ASSERT(GetParent()->GetSafeHwnd() != NULL);

	size_t nCount = pMember->vTaskWnd.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		CTaskWndInfo *pTaskInfo = &pMember->vTaskWnd[i];
		if (!pTaskInfo->m_bRadioButton)
		{
			continue;
		}

		CSkinButton *pRadioButton = (CSkinButton *)GetParent()->GetDlgItem(pTaskInfo->m_nRadioButtonID);

		if (pRadioButton->GetSafeHwnd() == NULL)
		{
			ASSERT(FALSE);
		}
		else
		{
			if (pTaskInfo->m_nRadioButtonID == nID)
			{
				pRadioButton->SetCheck(BST_CHECKED);

				// If not set task window focus, set radio button focus
				if (!bSetFocus)
				{
					pRadioButton->SetFocus();
				}
			}
			else
			{
				pRadioButton->SetCheck(BST_UNCHECKED);
			}
		}
	}

	//
	// 3. To be created
	//
	if (pSwitchTo != NULL)
	{
		if (pSwitchTo->m_nTaskWndID != 0)
		{
			UIWNDPROPERTIES *pUIWndProp = CUIMgr::GetUIWNDPROPERTIES(pSwitchTo->m_nTaskWndID);
			if (pUIWndProp != NULL)
			{
				RECT rcMargin = (RECT)pUIWndProp->m_rcLayoutMargin;
				GetDPIPadding(&rcMargin);
				RECT rcChild = (RECT)pUIWndProp->m_rcControl;
				GetDPIRect(&rcChild);
				RelayoutTaskWindow(rcNewTaskPos, rcChild,
					pUIWndProp->m_eHorLayoutMode, pUIWndProp->m_eVerLayoutMode, &rcMargin,
					rcTaskWnd);
			}
			else
			{
				ASSERT(FALSE);
			}
		}

		if (pSwitchTo->m_pTaskWnd->GetSafeHwnd() == NULL)
		{
#ifdef _DEBUG
			DWORD dwStart = GetTickCount();
#endif
			if (pSwitchTo->m_bUINoCodeMode)
			{
				UIWNDPROPERTIES *pUIWndProp = CUIMgr::GetUIWNDPROPERTIES(pSwitchTo->m_nTaskWndID);

				CUIWnd *pTaskWnd = NULL;
				if ((WINDOW_TYPE)pUIWndProp->m_eWindowType == WT_FORMWND)
				{
					pTaskWnd = new CFormWnd;
				}
				else
				{
					pTaskWnd = new CUIWnd;
				}

				pTaskWnd->SetIDD(pSwitchTo->m_nTaskWndID);
				pTaskWnd->SetNoCodeUIMode(TRUE);
				pSwitchTo->m_pTaskWnd = pTaskWnd;

				CHILDMSGHANDLER cmh;
				cmh.pUIWnd = pTaskWnd;
				cmh.uChildMsgHandlerParamFlag = CMHF_TASKWNDMGR;
				cmh.dwChildMsgHandlerParam = (DWORD_PTR)pSwitchTo;

				GetParent()->SendMessage(WM_GETCHILDMSGHANDLER, pTaskWnd->GetIDD(), LPARAM(&cmh));

				pTaskWnd->m_pUIWndHandler = cmh.pUIWndHandler;

				if (pTaskWnd->m_pUIWndHandler != NULL)
				{
					// Since cmh.lParam is not set by User,
					// don't call SetUserData
					pTaskWnd->m_pUIWndHandler->m_pBindUIWnd = pTaskWnd;
				}
			}

			ASSERT(pSwitchTo->m_pTaskWnd != NULL && pSwitchTo->m_pTaskWnd->GetSafeHwnd() == NULL);
			BOOL bRet = pSwitchTo->m_pTaskWnd->Create(NULL, pSwitchTo->m_strWindowName,
					pSwitchTo->m_dwTaskWndCreateStyle,
					rcTaskWnd, GetParent(), pSwitchTo->m_nTaskWndID);
#ifdef _DEBUG
			DWORD dwLost = GetTickCount() - dwStart;
			TRACE(_T("[LibUIDK]===Task window with ID %d created lost %d(ms)\n"), pSwitchTo->m_nTaskWndID, dwLost);
#endif
		}
	}

	//
	// 4. Send a message after the new task window be created, and before it visible.
	//
	CTaskParam tp;
	tp.m_pTaskWndMgr = this;
	tp.m_pSwitchTo = pSwitchTo;
	tp.m_pTobeHide = pTobeHide;
	GetParent()->SendMessage(WM_TASKWNDCREATE, GetDlgCtrlID(), (LPARAM)&tp);


	// If not smooth move, show it immediately.
	if (!pMember->bSmooth)
	{
		if (pSwitchTo != NULL)
		{
			pSwitchTo->m_pTaskWnd->SetRedraw(FALSE);
			pSwitchTo->m_pTaskWnd->MoveWindow(rcTaskWnd);
			pSwitchTo->m_pTaskWnd->ShowWindow(SW_SHOW);
			pSwitchTo->m_pTaskWnd->SetRedraw(TRUE);
			pSwitchTo->m_pTaskWnd->Invalidate();
			pSwitchTo->m_pTaskWnd->UpdateWindow();

			if (bSetFocus)
			{
				pSwitchTo->m_pTaskWnd->SetFocus();
			}
		}
	}

	//
	// 5. If not smooth move, hide it immediately.
	//
	if (!pMember->bSmooth)
	{
		if (pTobeHide != NULL && pTobeHide->m_pTaskWnd->GetSafeHwnd() != NULL)
		{
			pTobeHide->m_pTaskWnd->ShowWindow(SW_HIDE);
		}
	}

	//
	// 6. Update
	//
	pMember->nCurrentTaskWndID = nID;

	//
	// 7. If smooth move, start a timer.
	// If smooth move, Can hide the old task window immediately, before hide, capture the old task window
	// snapshot, and create a new window with the snapshot, then do animation on the new window.
	//
	if (pMember->bSmooth)
	{
		// Kill the old timer, replace task window, and release the memory.
		TASK_TIMER_STRUCT *ptts = (TASK_TIMER_STRUCT *)GetProp(GetParent()->GetSafeHwnd(), GETPROP_TASKWNDMGR_TIMERSTRUCT);
		if (ptts != NULL)
		{
			SafeDelete(ptts);
		}
		::KillTimer(GetParent()->GetSafeHwnd(), TIMER_ID_TASKWNDMGR);

		if (!bNotFoundHideAll)
		{
			CRect rcCur;
			pSwitchTo->m_pTaskWnd->GetWindowRect(rcCur);
			GetParent()->ScreenToClient(rcCur);
			if (rcCur != rcNewTaskPos)
			{
				pSwitchTo->m_pTaskWnd->MoveWindow(rcNewTaskPos);
			}
		}

		// Start the new timer
		ptts = new TASK_TIMER_STRUCT;
		ptts->pTobeHide = pTobeHide;
		ptts->pSwitchTo = pSwitchTo;
		ptts->rcTaskPos = rcNewTaskPos;
		ptts->pTaskWndMgr = this;
		ptts->pTaskWndMgrMember = (TASKWNDMGRMEMBER *)m_pMember;

		SetProp(GetParent()->GetSafeHwnd(), GETPROP_TASKWNDMGR_TIMERSTRUCT, ptts);

		if (pTobeHide != NULL)
		{
			::SetTimer(GetParent()->GetSafeHwnd(), TIMER_ID_TASKWNDMGR, pMember->nSmoothInterval, (TIMERPROC)TimerProc);
		}
	}
	else
	{
		if (GetParent() != NULL)
		{
			CTaskParam tp;
			tp.m_pTaskWndMgr = this;
			tp.m_pSwitchTo = pSwitchTo;
			tp.m_pTobeHide = pTobeHide;

			GetParent()->SendMessage(WM_TASKWNDSWITCHED, GetDlgCtrlID(), (LPARAM)&tp);
		}
	}

	return 0;
}

int CTaskWndMgr::SwitchToTaskWindow(CSkinButton *pRadTask, BOOL bSetFocus/* = FALSE*/)
{
	int nID = pRadTask->GetDlgCtrlID();

	return SwitchToTaskWindow(nID, bSetFocus);
}

int CTaskWndMgr::SwitchToTaskWindow(const CWnd *pTaskWnd, BOOL bSetFocus/* = FALSE*/)
{
	CTaskWndInfo *pTaskInfo = FindTaskWindow((CWnd *)pTaskWnd);
	if (pTaskInfo == NULL)
	{
		return -1;
	}

	return SwitchToTaskWindow(pTaskInfo->m_nRadioButtonID, bSetFocus);
}

int CTaskWndMgr::SwitchToTaskWindowIndex(int nIndex, BOOL bSetFocus/* = FALSE*/)
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	if (nIndex < 0)
	{
		return -1;
	}

	int nCount = (int)pMember->vTaskWnd.size();
	if (nIndex >= nCount)
	{
		return -1;
	}

	return SwitchToTaskWindow(pMember->vTaskWnd[nIndex].m_nRadioButtonID, bSetFocus);
}

int CTaskWndMgr::SwitchToNextTaskWindow()
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	int nCurIndex = GetCurrentTaskWindowIndex();
	int nCount = (int)pMember->vTaskWnd.size();

	int nNextIndex = nCurIndex + 1;
	if (nNextIndex >= nCount)
	{
		nNextIndex = 0;
	}

	return SwitchToTaskWindow(pMember->vTaskWnd[nNextIndex].m_nRadioButtonID, FALSE);
}

int CTaskWndMgr::SwitchToPrevTaskWindow()
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	int nCurIndex = GetCurrentTaskWindowIndex();
	int nCount = (int)pMember->vTaskWnd.size();

	int nPrevIndex = nCurIndex - 1;
	if (nPrevIndex < 0)
	{
		nPrevIndex = nCount - 1;
	}

	return SwitchToTaskWindow(pMember->vTaskWnd[nPrevIndex].m_nRadioButtonID, FALSE);
}

const CTaskWndInfo *CTaskWndMgr::FindTaskWindow(UINT nTaskRadioButton) const
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	const CTaskWndInfo *pRet = NULL;

	size_t nCount = pMember->vTaskWnd.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		CTaskWndInfo *pTaskInfo = &pMember->vTaskWnd[i];
		if (pTaskInfo->m_nRadioButtonID == nTaskRadioButton)
		{
			pRet = pTaskInfo;
			break;
		}
	}

	return pRet;
}

CTaskWndInfo *CTaskWndMgr::FindTaskWindow(UINT nTaskRadioButton)
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	CTaskWndInfo *pRet = NULL;

	size_t nCount = pMember->vTaskWnd.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		CTaskWndInfo *pTaskInfo = &pMember->vTaskWnd[i];
		if (pTaskInfo->m_nRadioButtonID == nTaskRadioButton)
		{
			pRet = pTaskInfo;
			break;
		}
	}

	return pRet;
}

const CTaskWndInfo *CTaskWndMgr::FindTaskWindow(CWnd *pTaskPaenl) const
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	const CTaskWndInfo *pRet = NULL;

	size_t nCount = pMember->vTaskWnd.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		CTaskWndInfo *pTaskInfo = &pMember->vTaskWnd[i];
		if (pTaskInfo->m_pTaskWnd == pTaskPaenl)
		{
			pRet = pTaskInfo;
			break;
		}
	}

	return pRet;
}

CTaskWndInfo *CTaskWndMgr::FindTaskWindow(CWnd *pTaskPaenl)
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	CTaskWndInfo *pRet = NULL;

	size_t nCount = pMember->vTaskWnd.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		CTaskWndInfo *pTaskInfo = &pMember->vTaskWnd[i];
		if (pTaskInfo->m_pTaskWnd == pTaskPaenl)
		{
			pRet = pTaskInfo;
			break;
		}
	}

	return pRet;
}

const CTaskWndInfo *CTaskWndMgr::FindTaskWindowByTaskID(UINT nTaskID) const
{
	CWnd *pTaskPanel = GetParent()->GetDlgItem(nTaskID);
	return FindTaskWindow(pTaskPanel);
}

CTaskWndInfo *CTaskWndMgr::FindTaskWindowByTaskID(UINT nTaskID)
{
	CWnd *pTaskPanel = GetParent()->GetDlgItem(nTaskID);
	return FindTaskWindow(pTaskPanel);
}

const CTaskWndInfo *CTaskWndMgr::GetTaskWindow(int nIndex) const
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	size_t nCount = pMember->vTaskWnd.size();
	if (nIndex < 0 || nIndex >= (int)nCount)
	{
		return NULL;
	}

	CTaskWndInfo *pTaskInfo = &pMember->vTaskWnd[nIndex];

	return pTaskInfo;
}

CTaskWndInfo *CTaskWndMgr::GetTaskWindow(int nIndex)
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	size_t nCount = pMember->vTaskWnd.size();
	if (nIndex < 0 || nIndex >= (int)nCount)
	{
		return NULL;
	}

	CTaskWndInfo *pTaskInfo = &pMember->vTaskWnd[nIndex];

	return pTaskInfo;
}

UINT CTaskWndMgr::GetTaskRadioButtonID(CWnd *pTaskPaenl)
{
	const CTaskWndInfo *pTaskInfo = FindTaskWindow(pTaskPaenl);

	if (pTaskInfo == NULL)
	{
		return -1;
	}

	return pTaskInfo->m_nRadioButtonID;
}

UINT CTaskWndMgr::GetCurrentTaskWindow() const
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	return pMember->nCurrentTaskWndID;
}

int CTaskWndMgr::GetCurrentTaskWindowIndex() const
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	if (pMember->nCurrentTaskWndID == -1)
	{
		return -1;
	}

	int nRet = -1;
	size_t nCount = pMember->vTaskWnd.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		CTaskWndInfo *pTaskInfo = &pMember->vTaskWnd[i];
		if (pTaskInfo->m_nRadioButtonID == pMember->nCurrentTaskWndID)
		{
			nRet = (int)i;
			break;
		}
	}

	return nRet;
}

LRESULT CTaskWndMgr::BroadcastMessageToAllTaskWnds(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bSend)
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	size_t nCount = pMember->vTaskWnd.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		CTaskWndInfo *pTaskInfo = &pMember->vTaskWnd[i];

		CWnd *pTaskWnd = pTaskInfo->m_pTaskWnd;

		if (::IsWindow(pTaskWnd->GetSafeHwnd()))
		{
			if (bSend)
			{
				pTaskWnd->SendMessage(uMsg, wParam, lParam);
			}
			else
			{
				pTaskWnd->PostMessage(uMsg, wParam, lParam);
			}
		}
	}

	return 0;
}

LRESULT CTaskWndMgr::SendMessageToTaskWnd(UINT nTaskRadioButton, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CTaskWndInfo *pTaskInfo = FindTaskWindow(nTaskRadioButton);
	if (pTaskInfo == NULL)
	{
		return -1;
	}

	if (::IsWindow(pTaskInfo->m_pTaskWnd->GetSafeHwnd()))
	{
		return pTaskInfo->m_pTaskWnd->SendMessage(uMsg, wParam, lParam);
	}

	return 0;
}

int CTaskWndMgr::MoveWindow(LPCRECT lpRect, BOOL bRepaint/* = TRUE*/)
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	CWLPicture::MoveWindow(lpRect, bRepaint);

	if (pMember->nCurrentTaskWndID == -1)
	{
		return 1;
	}

	// Only adjust current task window, other hidden task windows will adjust position while switch to theme.
	CTaskWndInfo *pTaskInfo = FindTaskWindow(pMember->nCurrentTaskWndID);
	CWnd *pTaskWnd = pTaskInfo->m_pTaskWnd;

	// May be call CTaskWndMgr::MoveWindow at WM_TASKWNDDESTROYED message.
	if (pTaskWnd->GetSafeHwnd() != NULL)
	{
		CUIWnd *pUITaskWnd = (CUIWnd *)pTaskWnd;
		BOOL bUIWnd = (BOOL)pUITaskWnd->SendMessage(WM_ISUIWND);
		if (bUIWnd)
		{
			const UIWNDPROPERTIES *pUIWndProp = pUITaskWnd->GetUIWndProperties();

			// CRectCtrl's orig rect is get by call CRectCtrl::GetOrigWindowRect.
			CRect rcNew;

			CRect rcOld;
			pUITaskWnd->GetWindowRect(rcOld);
			GetParent()->ScreenToClient(rcOld);
			RECT rcUIWndOrig = pUIWndProp->m_rcControl;
			GetDPIRect(&rcUIWndOrig);
			RECT rcMargin = (RECT)pUIWndProp->m_rcLayoutMargin;
			GetDPIPadding(&rcMargin);
			RelayoutTaskWindow(lpRect, rcUIWndOrig,
				pUIWndProp->m_eHorLayoutMode, pUIWndProp->m_eVerLayoutMode, &rcMargin,
				rcNew);

			BOOL bMoveWindow = (BOOL)GetParent()->SendMessage(WM_ISCHILDMOVE,
					0, LPARAM(pTaskWnd->GetSafeHwnd()));

			if (bMoveWindow)
			{
				if (rcOld != rcNew)
				{
					pTaskWnd->MoveWindow(rcNew);
				}
			}
		}
		else
		{
			pTaskWnd->MoveWindow(lpRect);
		}
	}

	return 0;
}

int CTaskWndMgr::EnableSmooth(BOOL bSmooth)
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	pMember->bSmooth = bSmooth;

	return 0;
}

BOOL CTaskWndMgr::IsSmoothEnabled() const
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	return pMember->bSmooth;
}

int CTaskWndMgr::SetSmoothSpeed(int nSpeed)
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	pMember->nSmoothSpeed = nSpeed;

	return 0;
}

int CTaskWndMgr::GetSmoothSpeed() const
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	return pMember->nSmoothSpeed;
}

int CTaskWndMgr::SetSmoothInterval(int nInterval)
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	pMember->nSmoothInterval = nInterval;

	return 0;
}

int CTaskWndMgr::GetSmoothInterval() const
{
	TASKWNDMGRMEMBER *pMember = (TASKWNDMGRMEMBER *)m_pMember;

	return pMember->nSmoothInterval;
}
