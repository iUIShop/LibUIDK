// RectCtrl.cpp : implementation file
//

#include "stdafx.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


struct RECTMEMBER
{
	std::vector<CWnd *> m_vBindWindow;
};

/////////////////////////////////////////////////////////////////////////////
// CRectCtrl

CRectCtrl::CRectCtrl()
{
	m_pMember = new RECTMEMBER;
	RECTMEMBER *pMember = (RECTMEMBER *)m_pMember;
}

CRectCtrl::~CRectCtrl()
{
	RECTMEMBER *pMember = (RECTMEMBER *)m_pMember;

	if (m_pMember != NULL)
	{
		delete (RECTMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

BOOL CRectCtrl::Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd,
	UINT nID, CWLWnd *pVirtualParent)
{
	RECTMEMBER *pMember = (RECTMEMBER *)m_pMember;

	return CWLWnd::Create(_T("IUIWL_Rect"), lpszWindowName, dwStyle,
			rect, pParentWnd, nID, pVirtualParent);
}

//////////////////////////////////////////////////////////////////////////
// public

int CRectCtrl::AddBindWindow(CWnd *pBindWindow)
{
	RECTMEMBER *pMember = (RECTMEMBER *)m_pMember;

	pMember->m_vBindWindow.push_back(pBindWindow);

	return 0;
}

int CRectCtrl::DeleteBindWindow(CWnd *pBindWindow)
{
	RECTMEMBER *pMember = (RECTMEMBER *)m_pMember;

	std::vector<CWnd *>::iterator it = std::find(pMember->m_vBindWindow.begin(), pMember->m_vBindWindow.end(), pBindWindow);
	if (it == pMember->m_vBindWindow.end())
	{
		return -1;
	}

	pMember->m_vBindWindow.erase(it);

	return 0;
}

int CRectCtrl::MoveWindow(LPCRECT lpRect, BOOL bRepaint/* = TRUE*/)
{
	RECTMEMBER *pMember = (RECTMEMBER *)m_pMember;

	// The bind window may be destroyed
	size_t nBindCount = pMember->m_vBindWindow.size();
	for (size_t i = 0; i < nBindCount; ++i)
	{
		CWnd *pBindWindow = pMember->m_vBindWindow[i];

		if (pBindWindow != NULL && pBindWindow->GetSafeHwnd() != NULL)
		{
			pBindWindow->MoveWindow(lpRect, bRepaint);
		}
	}

	return CWLWnd::MoveWindow(lpRect, bRepaint);
}

BOOL CRectCtrl::DestroyWindow()
{
	RECTMEMBER *pMember = (RECTMEMBER *)m_pMember;

	BOOL bRet = CWLWnd::DestroyWindow();

	pMember->m_vBindWindow.clear();

	return bRet;
}
