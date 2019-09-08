// SkinIPAddressCtrl.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


struct FIELD
{
	FIELD()
	{
		m_uFieldWidth = 0;
	}

	CSkinEdit m_ctrlEdit;
	UINT m_uFieldWidth;
};

struct IPADDRESSMEMBER
{
	IPADDRESSMEMBER()
	{
		m_bCombineFiller = TRUE;
		m_rcBorder.SetRect(2, 2, 2, 2);
		m_crBorder = RGB(128, 128, 128);
		m_crBk = GetSysColor(COLOR_WINDOW);
		m_uFieldHeight = 0; // 0: fill the client
		m_uFillerWidth = 0;
		m_eFieldAlignModeVer = CLV_FILL;

		for (int i = 0; i < 4; ++i)
		{
			m_himgBk[i] = NULL;
			m_himgFiller[i] = NULL;
		}
	}

	int Release()
	{
		m_bCombineFiller = TRUE;
		m_rcBorder.SetRect(2, 2, 2, 2);
		m_crBorder = RGB(128, 128, 128);
		m_crBk = GetSysColor(COLOR_WINDOW);
		m_uFieldHeight = 0;
		m_uFillerWidth = 0;
		m_eFieldAlignModeVer = CLV_FILL;

		for (int i = 0; i < 4; ++i)
		{
			ReleaseIUIImage(m_himgBk[i]);
			m_himgBk[i] = NULL;

			ReleaseIUIImage(m_himgFiller[i]);
			m_himgFiller[i] = NULL;
		}

		return 0;
	}

	HIUIIMAGE m_himgBk[4];
	BOOL m_bCombineFiller;
	// If m_bCombineFiller is TRUE, use m_himgFiller[0] store the combine filler image.
	HIUIIMAGE m_himgFiller[4];
	CRect m_rcBorder;
	COLORREF m_crBorder;
	COLORREF m_crBk;
	UINT m_uFieldHeight;
	CONTROL_LAYOUT_VER m_eFieldAlignModeVer;
	UINT m_uFillerWidth;

	FIELD m_ctrlEdit[4];
};

/////////////////////////////////////////////////////////////////////////////
// CSkinIPAddressCtrl

CSkinIPAddressCtrl::CSkinIPAddressCtrl()
{
	m_pBindWnd = this;

	m_pMember = new IPADDRESSMEMBER;
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;
}

CSkinIPAddressCtrl::~CSkinIPAddressCtrl()
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	if (m_pMember != NULL)
	{
		delete (IPADDRESSMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}

int CSkinIPAddressCtrl::BindStyle(LPCTSTR lpszStyleID)
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (lpszStyleID == NULL)
	{
		return -2;
	}

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_IPADDRESS, lpszStyleID);
	return BindStyle(pCtrlProp);

	return 0;
}

int CSkinIPAddressCtrl::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (pCtrlProp == NULL)
	{
		return -2;
	}
	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (ect != CT_STYLE_IPADDRESS && ect != CT_IPADDRESS)
	{
		return -3;
	}

	IPAPROPERTIES *pIPAProp = (IPAPROPERTIES *)pCtrlProp;

	// Background image
	if (pIPAProp->m_bSpecifyBackgroundImages)
	{
		CString strImageName[1 + COMBINEIMAGESIZE4];
		BOOL bCombineImage = TRUE;
		CTRLPROPERTIES::IUIGetBackground4(pCtrlProp, &bCombineImage, strImageName);
		if (bCombineImage)
		{
			SetBkCombineImage(strImageName[0]);
		}
		else
		{
			SetImages(CONTROL_STATE_UNCHECKED_ALL,
				strImageName[1], strImageName[2], strImageName[3], strImageName[4]);
		}
		for (int i = 0; i < 1 + COMBINEIMAGESIZE4; ++i)
		{
			ReleaseIUIImage(strImageName[i]);
		}
	}

	// Background color
	RESCOLORPROPERTIES *pColorPropBk = (RESCOLORPROPERTIES *)CUIMgr::GetColorResourceItem(
			(LPCTSTR)pCtrlProp->m_strResColorBk);
	if (pColorPropBk != NULL)
	{
		COLORREF crBk = pColorPropBk->m_crColor;
		SetBackgroundColor(crBk);
	}

	// Border size
	CRect rcBorder = pIPAProp->m_rcBorder;
	SetBorderRect(rcBorder);

	// Border color
	RESCOLORPROPERTIES *pColorPropBorder = (RESCOLORPROPERTIES *)CUIMgr::GetColorResourceItem(
			(LPCTSTR)pIPAProp->m_strResColorBorder);
	if (pColorPropBorder != NULL)
	{
		COLORREF cr = pColorPropBorder->m_crColor;
		SetBorderColor(cr);
	}

	// Filler
	SetFillerWidth((LONG)pIPAProp->m_lFillerWidth);

	// the SetFillerImage also set filler width, so must call SetFillerWidth before
	// SetCombineFillerImage or SetFillerImage
	CString strImageFillerName[1 + COMBINEIMAGESIZE4];
	bool bCombineFillerImage = (bool)pIPAProp->m_bCombineFillerImage;
	if (bCombineFillerImage)
	{
		strImageFillerName[0] = (LPCTSTR)pIPAProp->m_strCombineFillerImage;
		SetCombineFillerImage(strImageFillerName[0]);
	}
	else
	{
		int nIndex = 1;

		strImageFillerName[nIndex++] = (LPCTSTR)pIPAProp->m_strFillerImageN;
		strImageFillerName[nIndex++] = (LPCTSTR)pIPAProp->m_strFillerImageH;
		strImageFillerName[nIndex++] = (LPCTSTR)pIPAProp->m_strFillerImageS;
		strImageFillerName[nIndex++] = (LPCTSTR)pIPAProp->m_strFillerImageD;

		SetFillerImage(CONTROL_STATE_UNCHECKED_ALL,
			strImageFillerName[1], strImageFillerName[2],
			strImageFillerName[3], strImageFillerName[4]);
	}

	// Field
	CString strImageFieldName[1 + COMBINEIMAGESIZE4];
	bool bCombineFieldImage = (bool)pIPAProp->m_bCombineFieldImage;
	if (bCombineFieldImage)
	{
		strImageFieldName[0] = (LPCTSTR)pIPAProp->m_strCombineFieldImage;
		SetCombineFieldImage(strImageFieldName[0]);
	}
	else
	{
		int nIndex = 1;

		strImageFieldName[nIndex++] = (LPCTSTR)pIPAProp->m_strFieldImageN;
		strImageFieldName[nIndex++] = (LPCTSTR)pIPAProp->m_strFieldImageH;
		strImageFieldName[nIndex++] = (LPCTSTR)pIPAProp->m_strFieldImageS;
		strImageFieldName[nIndex++] = (LPCTSTR)pIPAProp->m_strFieldImageD;

		SetFieldImage(CONTROL_STATE_UNCHECKED_ALL,
			strImageFieldName[1], strImageFieldName[2],
			strImageFieldName[3], strImageFieldName[4]);
	}

	SetFieldHeight((LONG)pIPAProp->m_lFieldHeight);
	SetFieldAlignModeVer(pIPAProp->m_eFieldAlignModeVer);

	return 0;
}


BEGIN_MESSAGE_MAP(CSkinIPAddressCtrl, CIPAddressCtrl)
	//{{AFX_MSG_MAP(CSkinIPAddressCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

int CSkinIPAddressCtrl::SetImages(
	UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH, LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD)
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	m_bBkCombine = false;
	IUISetControlImage4(uMask, pMember->m_himgBk,
		lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);

	return 0;
}

int CSkinIPAddressCtrl::GetImages(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameH,
	CString *pstrImageNameS, CString *pstrImageNameD)
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = m_bBkCombine;
	return IUIGetControlImage4(uMask, m_himgCombineBk, pMember->m_himgBk, *pbCombineImage,
			pstrCombineImageName,
			pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);
}


// Set/Get child edit controls image
int CSkinIPAddressCtrl::SetCombineFieldImage(LPCTSTR lpszImageName)
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	for (int i = 0; i < 4 ; i++)
	{
		if (::IsWindow(pMember->m_ctrlEdit[i].m_ctrlEdit.GetSafeHwnd()))
		{
			pMember->m_ctrlEdit[i].m_ctrlEdit.SetBkCombineImage(lpszImageName);
		}
	}

	return 0;
}

int CSkinIPAddressCtrl::SetFieldImage(UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
	LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD)
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	for (int i = 0; i < 4 ; i++)
	{
		if (pMember->m_ctrlEdit[i].m_ctrlEdit.GetSafeHwnd() != NULL)
		{
			pMember->m_ctrlEdit[i].m_ctrlEdit.SetImages(uMask, lpszImageNameN, lpszImageNameH,
				lpszImageNameS, lpszImageNameD);
		}
	}

	return 0;
}

int CSkinIPAddressCtrl::GetFieldImage(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameH,
	CString *pstrImageNameS, CString *pstrImageNameD)
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	if (::IsWindow(pMember->m_ctrlEdit[0].m_ctrlEdit.GetSafeHwnd()))
	{
		return pMember->m_ctrlEdit[0].m_ctrlEdit.GetImages(uMask, pbCombineImage, pstrCombineImageName,
				pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);
	}

	return -1;
}

// Set/Get filler between two child edit control.
int CSkinIPAddressCtrl::SetCombineFillerImage(LPCTSTR lpszImageName)
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	pMember->m_bCombineFiller = TRUE;

	IUISetControlImage(&pMember->m_himgFiller[0], lpszImageName);
	pMember->m_uFillerWidth = pMember->m_himgFiller[0]->GetWidth() / 4;

	SendMessage(WM_SIZE, 0, 0);

	return 0;
}

int CSkinIPAddressCtrl::SetFillerImage(UINT uMask,
	LPCTSTR lpszImageNameN, LPCTSTR lpszImageNameH,
	LPCTSTR lpszImageNameS, LPCTSTR lpszImageNameD)
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	IUISetControlImage4(uMask, pMember->m_himgFiller,
		lpszImageNameN, lpszImageNameH, lpszImageNameS, lpszImageNameD);
	pMember->m_uFillerWidth = pMember->m_himgFiller[0]->GetWidth();

	SendMessage(WM_SIZE, 0, 0);

	return 0;
}

int CSkinIPAddressCtrl::GetFillerImage(
	UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameH,
	CString *pstrImageNameS, CString *pstrImageNameD)
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	if (pbCombineImage == NULL)
	{
		return -1;
	}

	*pbCombineImage = pMember->m_bCombineFiller;
	return IUIGetControlImage4(uMask, pMember->m_himgFiller[0], pMember->m_himgFiller, *pbCombineImage,
			pstrCombineImageName,
			pstrImageNameN, pstrImageNameH, pstrImageNameS, pstrImageNameD);
}

int CSkinIPAddressCtrl::SetBorderRect(LPCRECT lprcBorder)
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	if (lprcBorder == NULL)
	{
		return -1;
	}

	pMember->m_rcBorder = lprcBorder;

	SendMessage(WM_SIZE, 0, 0);

	return 0;
}

int CSkinIPAddressCtrl::GetBorderRect(LPRECT lprcBorder) const
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	if (lprcBorder == NULL)
	{
		return -1;
	}

	*lprcBorder = pMember->m_rcBorder;

	return 0;
}

int CSkinIPAddressCtrl::SetBorderColor(COLORREF crBorder)
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	pMember->m_crBorder = crBorder;

	return 0;
}

COLORREF CSkinIPAddressCtrl::GetBorderColor() const
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	return pMember->m_crBorder;
}

int CSkinIPAddressCtrl::SetBackgroundColor(COLORREF crBorder)
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	pMember->m_crBk = crBorder;

	return 0;
}

COLORREF CSkinIPAddressCtrl::GetBackgroundColor() const
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	return pMember->m_crBk;
}

int CSkinIPAddressCtrl::SetFieldHeight(UINT uHeight)
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	pMember->m_uFieldHeight = uHeight;

	SendMessage(WM_SIZE, 0, 0);

	return 0;
}

int CSkinIPAddressCtrl::GetFieldHeight() const
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	return pMember->m_uFieldHeight;
}

int CSkinIPAddressCtrl::SetFieldAlignModeVer(CONTROL_LAYOUT_VER eAlignMode)
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	pMember->m_eFieldAlignModeVer = eAlignMode;

	SendMessage(WM_SIZE, 0, 0);

	return 0;
}

CONTROL_LAYOUT_VER CSkinIPAddressCtrl::GetFieldAlignModeVer() const
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	return pMember->m_eFieldAlignModeVer;
}

int CSkinIPAddressCtrl::SetFillerWidth(UINT uWidth)
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	pMember->m_uFillerWidth = uWidth;

	SendMessage(WM_SIZE, 0, 0);

	return 0;
}

UINT CSkinIPAddressCtrl::GetFillerWidth() const
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	return pMember->m_uFillerWidth;
}

//////////////////////////////////////////////////////////////////////////
// CSkinIPAddressCtrl message handlers

LRESULT IPAddr_OnCreate(IPADDRESSMEMBER *pMember, HWND hWndIPAddr, WPARAM wParam, LPARAM lParam)
{
	LPCREATESTRUCT	lpCreateStruct = LPCREATESTRUCT(lParam);

	// the first child of IP Address control is the rightest edit control.
	HWND hEdit = ::GetWindow(hWndIPAddr, GW_CHILD);
	if (!::IsWindow(hEdit))
	{
		ASSERT(FALSE);
		return -1;
	}

	hEdit = ::GetWindow(hEdit, GW_HWNDLAST);

	int i = 0;
	while (::IsWindow(hEdit))
	{
		pMember->m_ctrlEdit[i].m_ctrlEdit.SubclassWindow(hEdit);

		i++;
		if (i >= 4)
		{
			break;;
		}

		hEdit = ::GetWindow(hEdit, GW_HWNDPREV);
	}

#define FILLER          TEXT('.')

	HFONT hfont = (HFONT)SendMessage(hWndIPAddr, WM_GETFONT, 0, 0);
	HDC hdc = GetDC(hWndIPAddr);
	HFONT hOldFont = (HFONT)::SelectObject(hdc, hfont);
	GetCharWidth(hdc, FILLER, FILLER, (int *)(&pMember->m_uFillerWidth));
	SelectObject(hdc, hOldFont);
	ReleaseDC(hWndIPAddr, hdc);

	return 0;
}

// 原生IP地址控件支持自定义尺寸，但里面的Edit控件只在创建时，根据IP地址控件的尺寸，
// 平均分成4份。 各占1/4，在创建好之后，为IP地址指定新尺寸后，里面的Edit控件位置不变。
// 所以，我们需要在IP地址控件尺寸改变后，让里面的Edit控件重新布局。
LRESULT IPAddr_OnSize(IPADDRESSMEMBER *pMember, HWND hWndIPAddr, WPARAM wParam, LPARAM lParam)
{
	CSize size((DWORD)lParam);

	CRect rcClient;
	::GetClientRect(hWndIPAddr, rcClient);
	rcClient.DeflateRect(pMember->m_rcBorder);

	UINT uiFieldStart = 0;

	//
	// 计算Edit控件垂直布局
	//
	int nTop = rcClient.top;
	int nBottom = rcClient.bottom;

	if (pMember->m_eFieldAlignModeVer == CLV_TOP)
	{
		nBottom = nTop + pMember->m_uFieldHeight;
	}
	else if (pMember->m_eFieldAlignModeVer == CLV_BOTTOM)
	{
		nTop = nBottom - pMember->m_uFieldHeight;
	}
	else if (pMember->m_eFieldAlignModeVer == CLV_CENTER)
	{
		nTop += (rcClient.Height() - pMember->m_uFieldHeight) / 2;
		nBottom = nTop + pMember->m_uFieldHeight;
	}
	else if (pMember->m_eFieldAlignModeVer == CLV_FILL)
	{
	}
	else
	{
		ASSERT(FALSE);
	}

	//
	// 为了使IP地址控件的边框永远是设置的值，并且Edit间距是相同的值，所以，每个Edit的宽度可能不相同。
	//
	for (int i = 0; i < 4; i++)
	{
		int uFieldWidth = (rcClient.Width() - uiFieldStart - pMember->m_uFillerWidth * (4 - i - 1)) / (4 - i);

		HWND hwnd = pMember->m_ctrlEdit[i].m_ctrlEdit.GetSafeHwnd();

		SetWindowPos(hwnd, NULL, pMember->m_rcBorder.left + uiFieldStart, nTop,
			uFieldWidth, nBottom - nTop, SWP_NOACTIVATE);

		pMember->m_ctrlEdit[i].m_uFieldWidth = uFieldWidth;
		uiFieldStart += (uFieldWidth + pMember->m_uFillerWidth);
	}

	return 0;
}

LRESULT IPAddr_OnPaint(IPADDRESSMEMBER *pMember, HWND hWndIPAddr, WPARAM wParam, LPARAM lParam)
{
	ASSERT(::IsWindow(hWndIPAddr));

	PAINTSTRUCT ps;
	HDC hDC = ::BeginPaint(hWndIPAddr, &ps);

	// TODO: Add your message handler code here
	CRect rcClient;
	::GetClientRect(hWndIPAddr, rcClient);

	if (pMember->m_himgBk[0] == NULL)
	{
		CRect rcClip = rcClient;
		rcClip.DeflateRect(pMember->m_rcBorder);

		// Draw client area
		HBRUSH hBrush = CreateSolidBrush(pMember->m_crBk);
		FillRect(hDC, rcClip, hBrush);
		::DeleteObject(hBrush);

		// Draw border
		ExcludeClipRect(hDC, rcClip.left, rcClip.top, rcClip.right, rcClip.bottom);

		hBrush = CreateSolidBrush(pMember->m_crBorder);
		FillRect(hDC, rcClient, hBrush);
		::DeleteObject(hBrush);

		SelectClipRgn(hDC, NULL);
	}
	else
	{
		// the IP address control can't be resized
		if (::IsWindowEnabled(hWndIPAddr))
		{
			IUIBitBlt(hDC, 0, 0, rcClient.Width(), rcClient.Height(),
				pMember->m_himgBk[0], 2, 2, SRCCOPY);
		}
		else
		{
			IUIBitBlt(hDC, 0, 0, rcClient.Width(), rcClient.Height(),
				pMember->m_himgBk[1], 2, 2, SRCCOPY);
		}
	}

	// Draw filler
	int nFillerStart = 0;
	for (int i = 0; i < 3; ++i)
	{
		CRect rcFiller = rcClient;
		rcFiller.DeflateRect(pMember->m_rcBorder);

		nFillerStart += pMember->m_ctrlEdit[i].m_uFieldWidth;

		rcFiller.left = pMember->m_rcBorder.left + nFillerStart;
		rcFiller.right = rcFiller.left + pMember->m_uFillerWidth;

		if (pMember->m_himgFiller[0] != NULL)
		{
			// Get filler width
			if (pMember->m_uFillerWidth != 0)
			{
				// center layout in vertical
				rcFiller.top += (rcFiller.Height() - pMember->m_himgFiller[0]->GetHeight()) / 2;
				rcFiller.bottom = rcFiller.top + pMember->m_himgFiller[0]->GetHeight();

				if (pMember->m_bCombineFiller)
				{
					IUIPartStretchBlt(hDC, rcFiller, pMember->m_himgFiller[0], 4, 0, IRM_STRETCH);
				}
				else
				{
					IUIStretchBlt(hDC, rcFiller, pMember->m_himgFiller[0]);
				}
			}
		}
		else
		{
			COLORREF crText = 0;
			if (IsWindowEnabled(hWndIPAddr))
			{
				crText = GetSysColor(COLOR_WINDOWTEXT);
			}
			else
			{
				crText = GetSysColor(COLOR_GRAYTEXT);
			}

			SetBkMode(hDC, TRANSPARENT);
			SetTextColor(hDC, crText);
			DrawText(hDC, CString(FILLER), 1, rcFiller, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		}

		nFillerStart += pMember->m_uFillerWidth;
	}

	::EndPaint(hWndIPAddr, &ps);

	return 0;
}

LRESULT IPAddr_OnDestroy(IPADDRESSMEMBER *pMember, HWND hWndIPAddr, WPARAM wParam, LPARAM lParam)
{
	// must destroy the edit control. otherwise, failed to
	// delete the EDITMEMBER member in the destructor of CSkinEdit control

	for (int i = 0; i < 4 ; i++)
	{
		pMember->m_ctrlEdit[i].m_ctrlEdit.ReleaseObject();
		pMember->m_ctrlEdit[i].m_ctrlEdit.DestroyWindow();
	}

	return 0;
}

LRESULT CSkinIPAddressCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	IPADDRESSMEMBER *pMember = (IPADDRESSMEMBER *)m_pMember;

	// TODO: Add your specialized code here and/or call the base class

	switch (message)
	{
	case WM_WINDOWPOSCHANGED:
		Ctrl_OnWindowPosChanged(this, wParam, lParam);
		break;

	case WM_NCCREATE:
	{
		CIPAddressCtrl::WindowProc(message, wParam, lParam);

		LONG_PTR lExStyle = IUIGetWindowLong(m_hWnd, GWL_EXSTYLE);
		lExStyle &= ~WS_EX_CLIENTEDGE;
		IUISetWindowLong(m_hWnd, GWL_EXSTYLE, lExStyle);
	}
	return 1;

	case WM_CREATE:
	{
		LPCREATESTRUCT	lpCreateStruct = LPCREATESTRUCT(lParam);

		ASSERT(!IsIncludeFlag(lpCreateStruct->dwExStyle, WS_EX_CLIENTEDGE));

		if (CIPAddressCtrl::OnCreate(lpCreateStruct) == -1)
		{
			return -1;
		}

		if (IPAddr_OnCreate(pMember, m_hWnd, wParam, lParam) != 0)
		{
			return -1;
		}
	}

		// must return TRUE, otherwise, the CSkinEdit::CtlColor will not be called.
	return TRUE;

	case WM_ERASEBKGND:
		return TRUE;

	case WM_PAINT:
		IPAddr_OnPaint(pMember, m_hWnd, wParam, lParam);
		break;

	case WM_SIZE:
		IPAddr_OnSize(pMember, m_hWnd, wParam, lParam);
		break;

	case WM_DESTROY:
		IPAddr_OnDestroy(pMember, m_hWnd, wParam, lParam);
		break;

	default:
		return CIPAddressCtrl::WindowProc(message, wParam, lParam);
	}

	return CIPAddressCtrl::WindowProc(message, wParam, lParam);
}
