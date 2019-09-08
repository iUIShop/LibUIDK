// MenuBar.cpp : implementation file
//

#include "stdafx.h"
#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuBar

struct MENUBARMEMBER
{
	MENUBARMEMBER()
	{
		m_uCurrentButtonID = -1;
		m_nNextButtonID = -1;

		for (int i = 0; i < COMBINEIMAGESIZE8 ; ++i)
		{
			m_hImgButtonBK[i] = NULL;
		}

		m_ptButtonResize.x = 18;
		m_ptButtonResize.y = 2;

		m_crButtonN = RGB(0, 0, 0);
		m_crButtonH = RGB(0, 0, 0);
		m_crButtonS = RGB(0, 0, 0);
		m_crButtonD = RGB(0, 0, 0);

		m_nButtonTextHorMargin = 16;
		m_nButtonTextVerMargin = 4;
	}

	~MENUBARMEMBER()
	{
		for (int i = 0; i < COMBINEIMAGESIZE8 ; ++i)
		{
			ReleaseIUIImage(m_hImgButtonBK[i]);
			m_hImgButtonBK[i] = NULL;
		}
	}

	// For menu bar button
	UINT m_uCurrentButtonID;
	int m_nNextButtonID;
	vector<CSkinButton *> m_vMenuButton;
	HIUIIMAGE m_hImgButtonBK[COMBINEIMAGESIZE8];

	CPoint m_ptButtonResize;

	CString m_strFontResID[4];

	COLORREF m_crButtonN;
	COLORREF m_crButtonH;
	COLORREF m_crButtonS;
	COLORREF m_crButtonD;

	int m_nButtonTextHorMargin;
	int m_nButtonTextVerMargin;

	// For menu and menu item
	MENUPROPERTIES m_MenuStyleProp;
};

CMenuBar::CMenuBar()
{
	m_pMember = new MENUBARMEMBER;
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;
}

CMenuBar::~CMenuBar()
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ReleaseMenuButton();
	ReleaseMenuItemBKImage();

	if (m_pMember != NULL)
	{
		delete (MENUBARMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}


BEGIN_MESSAGE_MAP(CMenuBar, CWnd)
	//{{AFX_MSG_MAP(CMenuBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_ENTERMENULOOP()
	ON_WM_EXITMENULOOP()
	ON_MESSAGE(WM_MENUMOUSEMOVE, OnMenuMouseMove)
END_MESSAGE_MAP()


BOOL CMenuBar::Create(DWORD dwStyle, CRect rect, CWnd *pParentWnd, UINT nWinID)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	CString strMyClass;
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;
	strMyClass = WC_MENUBAR;
	wndcls.lpszClassName = strMyClass.GetBuffer(strMyClass.GetLength());

	// Register the new class and exit if it fails
	if (!AfxRegisterClass(&wndcls))
	{
		TRACE(_T("[LibUIDK]===Class Registration Failed\n"));
		return FALSE;
	}

	CWnd *pWnd = this;
	BOOL bRet = pWnd->Create(WC_MENUBAR, _T("Menu bar"), dwStyle, rect, pParentWnd, nWinID);
	if (!bRet)
	{
		return bRet;
	}

	return bRet;
}

BOOL CMenuBar::Create(DWORD dwStyle, CRect rect, CWnd *pParentWnd, UINT nWinID, UINT nMenuID, HIUIIMAGE hCombineImage)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	BOOL bRet = Create(dwStyle, rect, pParentWnd, nWinID);
	if (!bRet)
	{
		return bRet;
	}

	SetMenuButtonImages(hCombineImage->GetSafeImageName());
	SetMenuID(nMenuID);

	return bRet;
}

BOOL CMenuBar::Create(DWORD dwStyle, CRect rect, CWnd *pParentWnd, UINT nWinID, UINT nMenuID,
	HIUIIMAGE hBmpN, HIUIIMAGE hBmpH, HIUIIMAGE hBmpS, HIUIIMAGE hBmpD)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	BOOL bRet = Create(dwStyle, rect, pParentWnd, nWinID);
	if (!bRet)
	{
		return bRet;
	}

	SetMenuButtonImages(CONTROL_STATE_UNCHECKED_ALL, hBmpN->GetSafeImageName(), hBmpH->GetSafeImageName(),
		hBmpS->GetSafeImageName(), hBmpD->GetSafeImageName());
	SetMenuID(nMenuID);

	return bRet;
}

int CMenuBar::BindMenuStyle(LPCTSTR lpszStyleID)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	if (m_menuBar.m_hMenu != NULL)
	{
		ASSERT(FALSE);
		return -1;
	}
	if (lpszStyleID == NULL)
	{
		return -2;
	}

	MENUPROPERTIES *pMenuStyleProp = (MENUPROPERTIES *)CUIMgr::GetStyleItem(STYLET_MENU, lpszStyleID);
	if (pMenuStyleProp == NULL)
	{
		return -3;
	}

	pMember->m_MenuStyleProp = *pMenuStyleProp;

	return 0;
}

int CMenuBar::BindMenuStyle(const CTRLPROPERTIES *pCtrlProp)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	if (m_menuBar.m_hMenu != NULL)
	{
		ASSERT(FALSE);
		return -1;
	}
	if (pCtrlProp == NULL)
	{
		return -2;
	}
	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (ect != CT_STYLE_MENU && ect != CT_MENU)
	{
		return -3;
	}

	pMember->m_MenuStyleProp = *((MENUPROPERTIES *)pCtrlProp);

	return 0;
}

CTRLPROPERTIES *CMenuBar::GetBindMenuStyle() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return &pMember->m_MenuStyleProp;
}

int CMenuBar::ReleaseMenuButton()
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	size_t nSize = pMember->m_vMenuButton.size();
	for (size_t i = 0; i < nSize ; i++)
	{
		CSkinButton *pBtn = pMember->m_vMenuButton[i];

		pBtn->ReleaseObject();

		delete pBtn;
	}
	pMember->m_vMenuButton.clear();

	return 0;
}

int CMenuBar::ReleaseMenuItemBKImage()
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// protected

int CMenuBar::RebuildMenuBar()
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ReleaseMenuButton();

	int nLeft = 0;
	int nTotalWidth = 0;
	int nMenubarHeight = 0;

	UINT nSize = m_menuBar.GetMenuItemCount();
	for (UINT i = 0; i < nSize ; i++)
	{
		// Get menu item text
		MENUITEMINFO mii;
		memset(&mii, 0, sizeof(MENUITEMINFO));
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_TYPE | MIIM_DATA;

		if (!m_menuBar.GetMenuItemInfo(i, &mii, TRUE))
		{
			DWORD dwError = GetLastError();
		}

		// Get string from CSkinMenu
		CSkinMenuItem *pData = (CSkinMenuItem *)mii.dwItemData;

		CMenu *pSubMenu = m_menuBar.GetSubMenu(i);

		// Create menu button, the button size will be adjust in SetMenuButtonImages
		CSkinButton *pMenuButton = new CSkinButton;
		DWORD dwStyle = WS_CHILD | WS_VISIBLE;
		// If the sub item is empty, use push button instead of check box.
		if (pSubMenu != NULL)
		{
			dwStyle |= BS_AUTOCHECKBOX;
		}
		pMenuButton->Create((pData == NULL) ? _T("") : pData->m_strText, dwStyle, CRect(0, 0, 10, 10), this, i + 1);
		SetProp(pMenuButton->GetSafeHwnd(), GETPROP_MENUBAR_BUTTON_INDEX, UintToPtr(i));

		SetMenuButtonProperty(pMenuButton);

		pMember->m_vMenuButton.push_back(pMenuButton);

		// Adjust the size of menu button
		int nImageHeight = 0;
		if (m_bBkCombine)
		{
			nImageHeight = m_himgCombineBk->GetHeight();
		}
		else
		{
			nImageHeight = pMember->m_hImgButtonBK[0]->GetHeight();
		}

		nMenubarHeight = RelayoutMenuBarButton(pMenuButton, nImageHeight, nLeft, nTotalWidth);
	}

	// Adjust menu bar's height
	if (nSize > 0)
	{
		ASSERT(nTotalWidth > 0);

		CRect rcWnd;
		GetWindowRect(rcWnd);
		GetParent()->ScreenToClient(rcWnd);
		rcWnd.bottom = rcWnd.top + nMenubarHeight;
		rcWnd.right = rcWnd.left + nTotalWidth;
		MoveWindow(rcWnd);
	}

	return 0;
}

int CMenuBar::RelayoutMenuBarButton(CSkinButton *pMenuButton, int nImageHeight, int &nLeft, int &nTotalWidth)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	CString strText;
	pMenuButton->GetWindowText(strText);

	CRect rcClient;
	pMenuButton->GetClientRect(rcClient);

	CClientDC dc(pMenuButton);

	HFONT hOldFont = NULL;
	HIUIFONT hIUIFont = NULL;
	if (!pMember->m_strFontResID[0].IsEmpty())
	{
		hIUIFont = CreateIUIFontInternal(pMember->m_strFontResID[0]);

		hOldFont = (HFONT)SelectObject(dc.GetSafeHdc(), GetHFont(hIUIFont));
	}

	int nHeight = dc.DrawText(strText, rcClient, DT_CALCRECT);

	if (hOldFont != NULL)
	{
		ReleaseIUIFontInternal(hIUIFont);
		dc.SelectObject(hOldFont);
	}

	CRect rcWin;
	pMenuButton->GetWindowRect(rcWin);
	ScreenToClient(rcWin);

	int nWidth = rcWin.Width();
	rcWin.left = nLeft;
	rcWin.right = rcWin.left + rcClient.Width() + pMember->m_nButtonTextHorMargin;
	rcWin.bottom = rcWin.top + max(nHeight + pMember->m_nButtonTextVerMargin, nImageHeight);
	pMenuButton->MoveWindow(rcWin);

	nLeft += rcWin.Width();
	nTotalWidth += rcWin.Width();

	return rcWin.Height();
}

int CMenuBar::SetMenuButtonProperty(CSkinButton *pMenuButton)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	if (pMenuButton == NULL)
	{
		return -1;
	}

	// Set images
	if (m_bBkCombine)
	{
		pMenuButton->SetBkCombineImage(m_himgCombineBk->GetSafeImageName());
	}
	else
	{
		pMenuButton->CButtonBase::SetBitmap(CONTROL_STATE_UNCHECKED_ALL,
			pMember->m_hImgButtonBK[0]->GetSafeImageName(),
			pMember->m_hImgButtonBK[1]->GetSafeImageName(),
			pMember->m_hImgButtonBK[2]->GetSafeImageName(),
			pMember->m_hImgButtonBK[3]->GetSafeImageName());

		// The menu button may be push button if no sub menu attach to it.
		LONG lStyle = GetWindowLong(pMenuButton->GetSafeHwnd(), GWL_STYLE);
		if ((lStyle & BS_AUTOCHECKBOX) == BS_AUTOCHECKBOX)
		{
			pMenuButton->SetCheckedBitmap(CONTROL_STATE_CHECKED_ALL,
				pMember->m_hImgButtonBK[4]->GetSafeImageName(),
				pMember->m_hImgButtonBK[5]->GetSafeImageName(),
				pMember->m_hImgButtonBK[6]->GetSafeImageName(),
				pMember->m_hImgButtonBK[7]->GetSafeImageName());
		}
	}

	// Set text color
	pMenuButton->SetTextColor(pMember->m_crButtonN, pMember->m_crButtonH, pMember->m_crButtonS, pMember->m_crButtonD, FALSE);
	pMenuButton->SetCheckedTextColor(pMember->m_crButtonN, pMember->m_crButtonH, pMember->m_crButtonS, pMember->m_crButtonD, FALSE);

	// Set text alignment
	pMenuButton->SetTextAlignHor(TAH_CENTER, TAH_CENTER, TAH_CENTER, TAH_CENTER);
	pMenuButton->SetCheckedTextAlignHor(TAH_CENTER, TAH_CENTER, TAH_CENTER, TAH_CENTER);

	// Set font
	pMenuButton->SetTextFont(CONTROL_STATE_UNCHECKED_ALL, pMember->m_strFontResID[0], pMember->m_strFontResID[1], pMember->m_strFontResID[2], pMember->m_strFontResID[3], FALSE);
	pMenuButton->SetCheckedTextFont(CONTROL_STATE_CHECKED_ALL, pMember->m_strFontResID[0], pMember->m_strFontResID[1], pMember->m_strFontResID[2], pMember->m_strFontResID[3], TRUE);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

int CMenuBar::SetMenuID(UINT uMenuID)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_hWnd != NULL);

	m_menuBar.ReleaseObject();
	m_menuBar.DestroyMenu();

	// Set menu properties before load skin menu.
	m_menuBar.BindStyle(&pMember->m_MenuStyleProp);

	BOOL bRet = m_menuBar.LoadMenu(uMenuID);
	if (!bRet)
	{
		return -2;
	}

	return RebuildMenuBar();
}

int CMenuBar::SetHMENU(HMENU hMenu)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_hWnd != NULL);
	if (hMenu == NULL)
	{
		return -1;
	}

	m_menuBar.ReleaseObject();
	m_menuBar.DestroyMenu();

	m_menuBar.BindStyle(&pMember->m_MenuStyleProp);

	BOOL bRet = m_menuBar.CreatePopupMenu();
	if (!bRet)
	{
		return -2;
	}

	CSkinMenu *pSkinMenu = m_menuBar.CopyMenu(hMenu, &m_menuBar);
	if (pSkinMenu == NULL)
	{
		return -3;
	}

	return RebuildMenuBar();
}

CSkinMenu *CMenuBar::GetSkinMenu()
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return &m_menuBar;
}

int CMenuBar::PopupMenu(UINT uButtonID)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	CButton *pButton = (CButton *)GetDlgItem(uButtonID);
	if (pButton == NULL)
	{
		return -1;
	}

	pMember->m_uCurrentButtonID = uButtonID;

	HANDLE hData = GetProp(pButton->GetSafeHwnd(), GETPROP_MENUBAR_BUTTON_INDEX);
	UINT uData = PtrToUint(hData);

	CRect rect;
	pButton->GetWindowRect(rect);

	SetForegroundWindow();

	CSkinMenu *pMenu = (CSkinMenu *)m_menuBar.GetSubMenu(uData);
	if (pMenu == NULL)
	{
		return 1;
	}

	// Check the menu button
	pButton->SetCheck(BST_CHECKED);

	BOOL bRet = pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN, rect.left, rect.bottom, this);

	// Uncheck the menu button
	pButton->SetCheck(BST_UNCHECKED);

	if (pMember->m_nNextButtonID != 0 && pMember->m_nNextButtonID != pMember->m_uCurrentButtonID)
	{
		PopupMenu(pMember->m_nNextButtonID);
	}

	return 0;
}

// Set all check boxes of menu bar, and adjust the size of the menu bar.
int CMenuBar::SetMenuButtonImages(UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
	LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	m_bBkCombine = false;
	IUISetControlImage4(uMask, pMember->m_hImgButtonBK,
		lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);

	// If the menu bar has build, apply the images to menu check box.
	int nLeft = 0;
	int nTotalWidth = 0;
	int nMenubarHeight = 0;

	// Adjust the size of menu button
	int nImageHeight = 0;
	if (m_bBkCombine)
	{
		nMenubarHeight = m_himgCombineBk->GetHeight();
	}
	else
	{
		nMenubarHeight = pMember->m_hImgButtonBK[0]->GetHeight();
	}

	size_t nSize = pMember->m_vMenuButton.size();
	for (size_t i = 0; i < nSize ; ++i)
	{
		// Set the background of menu button.
		CSkinButton *pMenuButton = pMember->m_vMenuButton[i];

		SetMenuButtonProperty(pMenuButton);

		nMenubarHeight = RelayoutMenuBarButton(pMenuButton, nImageHeight, nLeft, nTotalWidth);
	}

	// Adjust menu bar's height
	if (nSize > 0)
	{
		ASSERT(nTotalWidth > 0);

		CRect rcWnd;
		GetWindowRect(rcWnd);
		GetParent()->ScreenToClient(rcWnd);
		rcWnd.bottom = rcWnd.top + nMenubarHeight;
		rcWnd.right = rcWnd.left + nTotalWidth;
		MoveWindow(rcWnd);
	}

	return 0;
}

int CMenuBar::SetMenuButtonCheckedImages(UINT uMask,
	LPCTSTR lpszImageNameCN, LPCTSTR lpszImageNameCH,
	LPCTSTR lpszImageNameCS, LPCTSTR lpszImageNameCD)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	m_bBkCombine = false;
	IUISetControlCheckedImage4(uMask, &pMember->m_hImgButtonBK[4],
		lpszImageNameCN, lpszImageNameCH, lpszImageNameCS, lpszImageNameCD);

	// If the menu bar has build, apply the images to menu check box.
	int nLeft = 0;
	int nTotalWidth = 0;
	int nMenubarHeight = 0;

	// Adjust the size of menu button
	int nImageHeight = 0;
	if (m_bBkCombine)
	{
		nMenubarHeight = m_himgCombineBk->GetHeight();
	}
	else
	{
		nMenubarHeight = pMember->m_hImgButtonBK[0]->GetHeight();
	}

	size_t nSize = pMember->m_vMenuButton.size();
	for (size_t i = 0; i < nSize ; ++i)
	{
		// Set the background of menu button.
		CSkinButton *pMenuButton = pMember->m_vMenuButton[i];

		SetMenuButtonProperty(pMenuButton);

		nMenubarHeight = RelayoutMenuBarButton(pMenuButton, nImageHeight, nLeft, nTotalWidth);
	}

	// Adjust menu bar's height
	if (nSize > 0)
	{
		ASSERT(nTotalWidth > 0);

		CRect rcWnd;
		GetWindowRect(rcWnd);
		GetParent()->ScreenToClient(rcWnd);
		rcWnd.bottom = rcWnd.top + nMenubarHeight;
		rcWnd.right = rcWnd.left + nTotalWidth;
		MoveWindow(rcWnd);
	}

	return 0;
}

int CMenuBar::SetMenuButtonImages(LPCTSTR lpszImageName)
{
	return SetBkCombineImage(lpszImageName);
}

int CMenuBar::GetMenuButtonImages(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameH,
	CString *pstrImageNameS, CString *pstrImageNameD) const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bBkCombine;
	return IUIGetControlImage4(uMask, m_himgCombineBk, pMember->m_hImgButtonBK, *pbCombineImage,
			pstrCombineImageName,
			pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);
}

int CMenuBar::SetMenuButtonTextColor(COLORREF crN, COLORREF crH, COLORREF crS, COLORREF crD, BOOL bRedraw/* = TRUE*/)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	pMember->m_crButtonN = crN;
	pMember->m_crButtonH = crH;
	pMember->m_crButtonS = crS;
	pMember->m_crButtonD = crD;

	size_t nSize = pMember->m_vMenuButton.size();
	for (size_t i = 0; i < nSize ; i++)
	{
		CSkinButton *pBtn = pMember->m_vMenuButton[i];
		pBtn->SetTextColor(crN, crH, crS, crD, FALSE);
		pBtn->SetCheckedTextColor(crN, crH, crS, crD, bRedraw);
	}

	return 0;
}

int CMenuBar::GetMenuButtonTextColor(COLORREF *pcrN, COLORREF *pcrH, COLORREF *pcrS, COLORREF *pcrD) const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	if (pcrN != NULL)
	{
		*pcrN = pMember->m_crButtonN;
	}

	if (pcrH != NULL)
	{
		*pcrH = pMember->m_crButtonN;
	}

	if (pcrS != NULL)
	{
		*pcrS = pMember->m_crButtonN;
	}

	if (pcrD != NULL)
	{
		*pcrD = pMember->m_crButtonN;
	}

	return 0;
}

int CMenuBar::SetMenuButtonTextFont(LPCTSTR lpszFontIDN, LPCTSTR lpszFontIDH,
	LPCTSTR lpszFontIDS, LPCTSTR lpszFontIDD, BOOL bRedraw/* = TRUE*/)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	if (lpszFontIDN != NULL)
	{
		pMember->m_strFontResID[0] = lpszFontIDN;
	}
	if (lpszFontIDH != NULL)
	{
		pMember->m_strFontResID[1] = lpszFontIDH;
	}
	if (lpszFontIDS != NULL)
	{
		pMember->m_strFontResID[2] = lpszFontIDS;
	}
	if (lpszFontIDD != NULL)
	{
		pMember->m_strFontResID[3] = lpszFontIDD;
	}

	int nMenubarHeight = 0;
	int nLeft = 0;
	int nTotalWidth = 0;

	// Set the font for each menu button. the font can cause the size of menu button changed.
	int nImageHeight = 0;
	if (m_bBkCombine)
	{
		nImageHeight = m_himgCombineBk->GetHeight();
	}
	else
	{
		nImageHeight = pMember->m_hImgButtonBK[0]->GetHeight();
	}

	size_t nSize = pMember->m_vMenuButton.size();
	for (size_t i = 0; i < nSize ; i++)
	{
		CSkinButton *pMenuButton = pMember->m_vMenuButton[i];

		nMenubarHeight = RelayoutMenuBarButton(pMenuButton, nImageHeight, nLeft, nTotalWidth);
	}

	// Adjust menu bar's height
	if (nSize > 0)
	{
		ASSERT(nTotalWidth > 0);

		CRect rcWnd;
		GetWindowRect(rcWnd);
		GetParent()->ScreenToClient(rcWnd);
		rcWnd.bottom = rcWnd.top + nMenubarHeight;
		rcWnd.right = rcWnd.left + nTotalWidth;
		MoveWindow(rcWnd);
	}

	return 0;
}

int CMenuBar::GetMenuButtonTextFont(CString *pstrFontIDN, CString *pstrFontIDH,
	CString *pstrFontIDS, CString *pstrFontIDD) const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	size_t nSize = pMember->m_vMenuButton.size();
	if (nSize <= 0)
	{
		return -1;
	}

	CSkinButton *pBtn = pMember->m_vMenuButton[0];
	pBtn->GetTextFont(CONTROL_STATE_UNCHECKED_ALL, pstrFontIDN, pstrFontIDN, pstrFontIDN, pstrFontIDN);

	return 0;
}

int CMenuBar::SetMenuBorderStyle(MENU_BORDER_STYLE eMbs)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	pMember->m_MenuStyleProp.m_eMenuBorderStyle = eMbs;

	return 0;
}

MENU_BORDER_STYLE CMenuBar::GetMenuBorderStyle() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return (MENU_BORDER_STYLE)pMember->m_MenuStyleProp.m_eMenuBorderStyle;
}

int CMenuBar::SetFlatMenuBorderColor(COLORREF crBorder)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	pMember->m_MenuStyleProp.m_crFlatMenuBorder = (LONG)crBorder;

	return 0;
}

COLORREF CMenuBar::GetFlatMenuBorderColor() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return (COLORREF)pMember->m_MenuStyleProp.m_crFlatMenuBorder;
}

int CMenuBar::SetMenuItemHeight(int nHeight)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	pMember->m_MenuStyleProp.m_nMenuItemHeight = (LONG)nHeight;

	return 0;
}

int CMenuBar::GetMenuItemHeight() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return (int)(LONG)pMember->m_MenuStyleProp.m_nMenuItemHeight;
}

int CMenuBar::SetNormalMenuItemTextColor(COLORREF crItemText)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	pMember->m_MenuStyleProp.m_crNormalItemText = (LONG)crItemText;

	return 0;
}

COLORREF CMenuBar::GetNormalMenuItemTextColor() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return pMember->m_MenuStyleProp.m_crNormalItemText;
}

int CMenuBar::SetNormalMenuItemFont(const LOGFONT *plf)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	if (plf == NULL)
	{
		return -1;
	}

	pMember->m_MenuStyleProp.m_lfNormalItemText = *plf;

	return 0;
}

int CMenuBar::GetNormalMenuItemFont(LOGFONT *plf) const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	if (plf == NULL)
	{
		return -1;
	}

	memcpy(plf, &((LOGFONT)pMember->m_MenuStyleProp.m_lfNormalItemText), sizeof(LOGFONT));

	return 0;
}

int CMenuBar::SetNormalMenuItemBkType(BACKGROUND_TYPE eBkType)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	pMember->m_MenuStyleProp.m_eNormalItemBkType = eBkType;

	return 0;
}

BACKGROUND_TYPE CMenuBar::GetNormalMenuItemBkType() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return pMember->m_MenuStyleProp.m_eNormalItemBkType;
}

int CMenuBar::SetNormalMenuItemBkColor(COLORREF crItemBk)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	pMember->m_MenuStyleProp.m_crNormalItemBkColor = (LONG)crItemBk;

	return 0;
}

COLORREF CMenuBar::GetNormalMenuItemBkColor() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return pMember->m_MenuStyleProp.m_crNormalItemBkColor;
}

int CMenuBar::SetHighlightMenuItemTextColor(COLORREF crItemText)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	pMember->m_MenuStyleProp.m_crHighlightItemText = (LONG)crItemText;

	return 0;
}

COLORREF CMenuBar::GetHighlightMenuItemTextColor() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return pMember->m_MenuStyleProp.m_crHighlightItemText;
}

int CMenuBar::SetHighlightMenuItemFont(const LOGFONT *plf)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	if (plf == NULL)
	{
		return -1;
	}

	pMember->m_MenuStyleProp.m_lfHighlightItemText = *plf;

	return 0;
}

int CMenuBar::GetHighlightMenuItemFont(LOGFONT *plf) const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	if (plf == NULL)
	{
		return -1;
	}

	memcpy(plf, &((LOGFONT)pMember->m_MenuStyleProp.m_lfHighlightItemText), sizeof(LOGFONT));

	return 0;
}

int CMenuBar::SetHighlightMenuItemBkType(BACKGROUND_TYPE eBkType)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	pMember->m_MenuStyleProp.m_eHighlightItemBkType = eBkType;

	return 0;
}

BACKGROUND_TYPE CMenuBar::GetHighlightMenuItemBkType() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return pMember->m_MenuStyleProp.m_eHighlightItemBkType;
}

int CMenuBar::SetHighlightMenuItemBkColor(COLORREF crItemBk) const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	pMember->m_MenuStyleProp.m_crHighlightItemBkColor = (LONG)crItemBk;

	return 0;
}

COLORREF CMenuBar::GetHighlightMenuItemBkColor() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return pMember->m_MenuStyleProp.m_crHighlightItemBkColor;
}

int CMenuBar::SetDisabledMenuItemTextColor(COLORREF crItemText)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	pMember->m_MenuStyleProp.m_crDisabledItemText = (LONG)crItemText;

	return 0;
}

COLORREF CMenuBar::GetDisabledMenuItemTextColor() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return pMember->m_MenuStyleProp.m_crDisabledItemText;
}

int CMenuBar::SetDisabledMenuItemFont(const LOGFONT *plf)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	if (plf == NULL)
	{
		return -1;
	}

	pMember->m_MenuStyleProp.m_lfDisabledItemText = *plf;

	return 0;
}

int CMenuBar::GetDisabledMenuItemFont(LOGFONT *plf) const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	if (plf == NULL)
	{
		return -1;
	}

	memcpy(plf, &((LOGFONT)pMember->m_MenuStyleProp.m_lfDisabledItemText), sizeof(LOGFONT));

	return 0;
}

int CMenuBar::SetDisabledMenuItemBkType(BACKGROUND_TYPE eBkType)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	pMember->m_MenuStyleProp.m_eDisabledItemBkType = eBkType;

	return 0;
}

BACKGROUND_TYPE CMenuBar::GetDisabledMenuItemBkType() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return pMember->m_MenuStyleProp.m_eDisabledItemBkType;
}

int CMenuBar::SetDisabledMenuItemBkColor(COLORREF crItemBk) const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	pMember->m_MenuStyleProp.m_crDisabledItemBkColor = (LONG)crItemBk;

	return 0;
}

COLORREF CMenuBar::GetDisabledMenuItemBkColor() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return pMember->m_MenuStyleProp.m_crDisabledItemBkColor;
}

int CMenuBar::SetSideBarStartColor(COLORREF crSideBarStart)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	ASSERT(FALSE);
	//	pMember->m_MenuStyleProp.m_m_crSideBarStart = (LONG)crSideBarStart;

	return -1;
}

COLORREF CMenuBar::GetSideBarStartColor() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(FALSE);
	return -1;	// pMember->m_MenuStyleProp.m_crSideBarStart;
}

int CMenuBar::SetSideBarEndColor(COLORREF crSideBarEnd)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	ASSERT(FALSE);
	//	pMember->m_MenuStyleProp.m_crSideBarEnd = (LONG)crSideBarEnd;

	return -1;
}

COLORREF CMenuBar::GetSideBarEndColor() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(FALSE);
	return -1; // pMember->m_MenuStyleProp.m_crSideBarEnd;
}

int CMenuBar::SetSeparatorBkType(BACKGROUND_TYPE eBkType)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	pMember->m_MenuStyleProp.m_eSeparatorBkType = eBkType;

	return 0;
}

BACKGROUND_TYPE CMenuBar::GetSeparatorBkType() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return pMember->m_MenuStyleProp.m_eSeparatorBkType;
}

int CMenuBar::SetSeparatorColor(COLORREF crSeparator)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	pMember->m_MenuStyleProp.m_crSeparator = (LONG)crSeparator;

	return 0;
}

COLORREF CMenuBar::GetSeparatorColor() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return pMember->m_MenuStyleProp.m_crSeparator;
}

int CMenuBar::SetSeparatorLeftMargin(int nLeftMargin)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	pMember->m_MenuStyleProp.m_nSeparatorLeftMargin = (LONG)nLeftMargin;

	return 0;
}

int CMenuBar::GetSeparatorLeftMargin() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return (LONG)pMember->m_MenuStyleProp.m_nSeparatorLeftMargin;
}

int CMenuBar::SetSeparatorRightMargin(int nRightMargin)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	pMember->m_MenuStyleProp.m_nSeparatorRightMargin = (LONG)nRightMargin;

	return 0;
}

int CMenuBar::GetSeparatorRightMargin() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return (LONG)pMember->m_MenuStyleProp.m_nSeparatorRightMargin;
}

int CMenuBar::SetIconAreaWidth(int nWidth)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	pMember->m_MenuStyleProp.m_nIconAreaWidth = (LONG)nWidth;

	return 0;
}

int CMenuBar::GetIconAreaWidth() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return (LONG)pMember->m_MenuStyleProp.m_nIconAreaWidth;
}

int CMenuBar::SetIconAreaStartColor(COLORREF crIconAreaStart)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	pMember->m_MenuStyleProp.m_crIconAreaStart = (LONG)crIconAreaStart;

	return 0;
}

COLORREF CMenuBar::GetIconAreaStartColor() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return pMember->m_MenuStyleProp.m_crIconAreaStart;
}

int CMenuBar::SetIconAreaEndColor(COLORREF crIconAreaEnd)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ASSERT(m_menuBar.m_hMenu == NULL);

	pMember->m_MenuStyleProp.m_crIconAreaEnd = (LONG)crIconAreaEnd;

	return 0;
}

COLORREF CMenuBar::GetIconAreaEndColor() const
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	return pMember->m_MenuStyleProp.m_crIconAreaEnd;
}

/////////////////////////////////////////////////////////////////////////////
// CMenuBar message handlers

int CMenuBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	if (CWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// TODO: Add your specialized creation code here

	return 0;
}

BOOL CMenuBar::PreTranslateMessage(MSG *pMsg)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		HWND hWnd = pMsg->hwnd;
		if (hWnd != m_hWnd)
		{
			UINT uID = ::GetDlgCtrlID(hWnd);
			int nRet = PopupMenu(uID);
			if (nRet == 0)
			{
				return TRUE;
			}
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

LRESULT CMenuBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class

	if (WM_COMMAND == message)
	{
		GetParent()->SendMessage(message, wParam, lParam);
	}
#if(WINVER >= 0x0500)
	else if (WM_MENUCOMMAND == message)
	{
		GetParent()->SendMessage(message, wParam, lParam);
	}
#endif //(WINVER >= 0x0500)

	else if (WM_DESTROY == message)
	{
		//////////////////////////////////////////////////////////////////////////
		// For menu bar button

		ReleaseMenuButton();
		ReleaseMenuItemBKImage();

		for (int i = 0; i < 8; ++i)
		{
			ReleaseIUIImage(pMember->m_hImgButtonBK[i]);
			pMember->m_hImgButtonBK[i] = NULL;
		}
	}
	else if (WM_PAINT == message)
	{
		CPaintDC dc(this);

		// Draw the parent background to control for alpha blend with background of the control.

		CRect rcWin;
		GetWindowRect(rcWin);
		GetParent()->ScreenToClient(rcWin);

		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);

		CBitmap bmpMem;
		bmpMem.CreateCompatibleBitmap(&dc, rcWin.Width(), rcWin.Height());

		CBitmap *pbmpOldMem = dcMem.SelectObject(&bmpMem);

		BOOL bUpdateLayeredWindow = FALSE;
		DrawParentPart(this, &dcMem, rcWin, &bUpdateLayeredWindow, NULL);

		dc.BitBlt(0, 0, rcWin.Width(), rcWin.Height(), &dcMem, 0, 0, SRCCOPY);

		dcMem.SelectObject(pbmpOldMem);
	}
	else if (WM_INITMENUPOPUP == message)
	{
		// The CWnd * parameter of TrackPopupMenu receive the WM_INITMENUPOPUP message.
		// So transmit the WM_INITMENUPOPUP message to the parent of this menu bar.
		// See the source code of BOOL CMenu::TrackPopupMenu
		_AFX_THREAD_STATE *pThreadState = AfxGetThreadState();

		if (pThreadState->m_hTrackingWindow == m_hWnd)
		{
			GetParent()->SendMessage(message, wParam, lParam);
		}
	}
	else if (WM_ISMENUBAR == message)
	{
		CWnd::WindowProc(message, wParam, lParam);
		return TRUE;
	}

	// This menu bar may be destroyed by parent by handle WM_COMMAND message with ID_APP_EXIT parameter.
	if (!::IsWindow(m_hWnd))
	{
		return 0;
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

void CMenuBar::OnEnterMenuLoop(BOOL bIsTrackPopupMenu)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

}

void CMenuBar::OnExitMenuLoop(BOOL bIsTrackPopupMenu)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	ReleaseCapture();
}

LRESULT CMenuBar::OnMenuMouseMove(WPARAM wParam, LPARAM lParam)
{
	MENUBARMEMBER *pMember = (MENUBARMEMBER *)m_pMember;

	HWND hMenu = (HWND)wParam;
	HWND hWnd = ::WindowFromPoint(CPoint(lParam));

	if (IsChildWindow(m_hWnd, hWnd) && hWnd != ::GetDlgItem(m_hWnd, pMember->m_uCurrentButtonID) && hMenu != hWnd)
	{
		// Send WM_CANCELMODE message for destroy previous pop-up menu.
		SendMessage(WM_CANCELMODE, 0, 0);

		// Record next track menu. Do NOT track pop-up menu at this, because the previous menu not free completely.
		// The next pop-up menu will be pop up after previous menu destroy in interface PopupMenu.
		pMember->m_nNextButtonID = ::GetDlgCtrlID(hWnd);

		return 1;
	}

	pMember->m_nNextButtonID = 0;

	return 0;
}
