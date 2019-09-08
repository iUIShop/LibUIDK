// PicCharacterCtrl.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct PICCHARACTERMEMBER
{
	PICCHARACTERMEMBER()
	{
		m_hBackground = NULL;
		m_hCharacterList = NULL;
	}

	HIUIIMAGE m_hBackground;
	HIUIIMAGE m_hCharacterList;

	std::map<TCHAR, CHARINDEX> m_mapCharIndex;	// record character index.
};

// CPicCharacterCtrl

CPicCharacterCtrl::CPicCharacterCtrl()
{
	m_pMember = new PICCHARACTERMEMBER;
	PICCHARACTERMEMBER *pMember = (PICCHARACTERMEMBER *)m_pMember;
}

CPicCharacterCtrl::~CPicCharacterCtrl()
{
	PICCHARACTERMEMBER *pMember = (PICCHARACTERMEMBER *)m_pMember;

	ReleaseIUIImage(pMember->m_hBackground);
	pMember->m_hBackground = NULL;
	ReleaseIUIImage(pMember->m_hCharacterList);
	pMember->m_hCharacterList = NULL;

	if (m_pMember != NULL)
	{
		delete (PICCHARACTERMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}


BEGIN_MESSAGE_MAP(CPicCharacterCtrl, CWnd)
END_MESSAGE_MAP()


BOOL CPicCharacterCtrl::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	PICCHARACTERMEMBER *pMember = (PICCHARACTERMEMBER *)m_pMember;

	CString strMyClass;
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;
	strMyClass = WC_PICCHARACTERVIEW;
	wndcls.lpszClassName = strMyClass.GetBuffer(strMyClass.GetLength());

	// Register the new class and exit if it fails
	if (!AfxRegisterClass(&wndcls))
	{
		TRACE(_T("[LibUIDK]===Class \"%s\" Registration Failed\n"), WC_PICCHARACTERVIEW);
		return FALSE;
	}

	CWnd *pWnd = this;
	return pWnd->Create(WC_PICCHARACTERVIEW, NULL, dwStyle, rect, pParentWnd, nID);
}

BOOL CPicCharacterCtrl::Create(DWORD dwStyle, CRectCtrl *pRectCtrl, CWnd *pParentWnd, UINT nID)
{
	PICCHARACTERMEMBER *pMember = (PICCHARACTERMEMBER *)m_pMember;

	ASSERT(pRectCtrl != NULL && pRectCtrl->IsCreated());

	CRect rect;
	pRectCtrl->GetWindowRect(rect);
	pParentWnd->ScreenToClient(rect);

	BOOL bRet = CPicCharacterCtrl::Create(dwStyle, rect, pParentWnd, nID);
	if (!bRet)
	{
		return FALSE;
	}

	pRectCtrl->AddBindWindow(this);

	return TRUE;
}

int CPicCharacterCtrl::BindStyle(LPCTSTR lpszStyleID)
{
	PICCHARACTERMEMBER *pMember = (PICCHARACTERMEMBER *)m_pMember;

	if (!IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
		return -1;
	}

	if (lpszStyleID == NULL)
	{
		return -2;
	}

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_WL_PICTURE, lpszStyleID);
	return BindStyle(pCtrlProp);
}

int CPicCharacterCtrl::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	PICCHARACTERMEMBER *pMember = (PICCHARACTERMEMBER *)m_pMember;

	if (!IsWindow(m_hWnd))
	{
		ASSERT(FALSE);
		return -1;
	}

	if (pCtrlProp == NULL)
	{
		return -2;
	}
	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (ect != CT_STYLE_WL_PICTURE && ect != CT_WL_PICTURE && ect != CT_PICTURE)
	{
		return -3;
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////
// virtual

int CPicCharacterCtrl::OnDraw(CDC *pdcMem)
{
	PICCHARACTERMEMBER *pMember = (PICCHARACTERMEMBER *)m_pMember;

	CRect rcClient;
	GetClientRect(rcClient);

	// Draw parent part
	CRect rcWin;
	GetWindowRect(rcWin);
	GetParent()->ScreenToClient(rcWin);

	BOOL bUpdateLayeredWindow = FALSE;
	DrawParentPart(this, pdcMem, rcWin, &bUpdateLayeredWindow, NULL);

	CString strText;
	GetWindowText(strText);

	int nImageWidth = pMember->m_hCharacterList->GetWidth();
	int nImageHeight = pMember->m_hCharacterList->GetHeight();

	int nLeftWidth = 0;				// The total width of character has draw.
	// Draw each character
	int nCount = strText.GetLength();
	for (int i = 0; i < nCount; ++i)
	{
		TCHAR ch = strText[i];
		std::map<TCHAR, CHARINDEX>::iterator it = pMember->m_mapCharIndex.find(ch);
		if (it == pMember->m_mapCharIndex.end())
		{
			continue;
		}

		CHARINDEX chIndex = it->second;

		IUIBitBlt(pdcMem->GetSafeHdc(), nLeftWidth, 0, chIndex.nWidth, nImageHeight, pMember->m_hCharacterList, chIndex.nXOffset, 0, SRCCOPY);

		nLeftWidth += chIndex.nWidth;
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////
// public

int CPicCharacterCtrl::SetCharacterListImage(LPCTSTR lpszImageName, BOOL bRedraw/* = TRUE*/)
{
	PICCHARACTERMEMBER *pMember = (PICCHARACTERMEMBER *)m_pMember;

	IUISetControlImage(&pMember->m_hCharacterList, lpszImageName);

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CPicCharacterCtrl::GetCharacterListImage(CString *pstrImageName)
{
	PICCHARACTERMEMBER *pMember = (PICCHARACTERMEMBER *)m_pMember;

	return IUIGetControlImage(pMember->m_hCharacterList, pstrImageName);
}


int CPicCharacterCtrl::SetCharacterIndex(CHARINDEX *pCharIndexList, int nCount)
{
	PICCHARACTERMEMBER *pMember = (PICCHARACTERMEMBER *)m_pMember;

	for (int i = 0; i < nCount; ++i)
	{
		CHARINDEX *pCharIndex = pCharIndexList + i;

		pMember->m_mapCharIndex[pCharIndex->ch] = *pCharIndex;
	}

	return 0;
}

int CPicCharacterCtrl::FindCharacter(TCHAR ch, __out CHARINDEX *pCharIndex)
{
	PICCHARACTERMEMBER *pMember = (PICCHARACTERMEMBER *)m_pMember;

	if (pCharIndex == NULL)
	{
		return -1;
	}

	std::map<TCHAR, CHARINDEX>::iterator it = pMember->m_mapCharIndex.find(ch);
	if (it == pMember->m_mapCharIndex.end())
	{
		return -1;
	}

	*pCharIndex = it->second;

	return 0;
}


// CPicCharacterCtrl message handlers

LRESULT CPicCharacterCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	PICCHARACTERMEMBER *pMember = (PICCHARACTERMEMBER *)m_pMember;

	// When use layered window, the window not redraw when its child controls hide or display
	// to solve it, handle the WM_WINDOWPOSCHANGED message.
	CWnd *pParent = GetParent();
	BOOL bLayered = IsLayeredWindow(pParent->GetSafeHwnd());
	if (message == WM_WINDOWPOSCHANGED && bLayered)
	{
		WINDOWPOS *pos = (WINDOWPOS *)lParam;
		if (pos->hwnd == m_hWnd)
		{
			// When show the control, notify itself to redraw
			if ((pos->flags & SWP_SHOWWINDOW) == SWP_SHOWWINDOW)
			{
				Redraw(this);
			}
			// When hide the control, notify its parent to redraw
			else if ((pos->flags & SWP_HIDEWINDOW) == SWP_HIDEWINDOW)
			{
				CRect rcThis;
				GetWindowRect(rcThis);
				pParent->ScreenToClient(rcThis);
				pParent->InvalidateRect(rcThis);
			}
		}
	}

	if (message == WM_PAINT)
	{
		CPaintDC dc(this);

		CRect rcClient;
		GetClientRect(rcClient);

		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);

		CBitmap bmpMem;
		bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

		CBitmap *pbmpMemOld = dcMem.SelectObject(&bmpMem);

		// Draw content to dcMem...
		OnDraw(&dcMem);

		dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);
		dcMem.SelectObject(pbmpMemOld);
	}
	else if (message == WM_ERASEBKGND)
	{
		return TRUE;
	}

	return CWnd::WindowProc(message, wParam, lParam);
}
