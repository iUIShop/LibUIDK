// IUIMFCPropertyGridCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "UIShop.h"
#include "IUIMFCPropertyGridCtrl.h"
#include "FileDialogEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
//

IMPLEMENT_DYNAMIC(CIUIMFCPropertyGridFileProperty, CMFCPropertyGridFileProperty)

CIUIMFCPropertyGridFileProperty::CIUIMFCPropertyGridFileProperty(const CString& strName, const CString& strFolderName, DWORD_PTR dwData/* = 0*/, LPCTSTR lpszDescr/* = NULL*/)
	: CMFCPropertyGridFileProperty(strName, strFolderName, dwData, lpszDescr)
{

}

CIUIMFCPropertyGridFileProperty::CIUIMFCPropertyGridFileProperty(const CString& strName, BOOL bOpenFileDialog, const CString& strFileName, LPCTSTR lpszDefExt/* = NULL*/,
								DWORD dwFlags/* = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT*/, LPCTSTR lpszFilter/* = NULL*/, LPCTSTR lpszDescr/* = NULL*/, DWORD_PTR dwData/* = 0*/)
								: CMFCPropertyGridFileProperty(strName, bOpenFileDialog, strFileName, lpszDefExt, dwFlags, lpszFilter, lpszDescr, dwData)
{

}

CIUIMFCPropertyGridFileProperty::~CIUIMFCPropertyGridFileProperty()
{

}

void CIUIMFCPropertyGridFileProperty::OnClickButton(CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	m_bButtonIsDown = TRUE;
	Redraw();

	CString strPath = m_varValue.bstrVal;
	BOOL bUpdate = FALSE;

// 	if (m_bIsFolder)
// 	{
// 		if (afxShellManager == NULL)
// 		{
// 			CWinAppEx* pApp = DYNAMIC_DOWNCAST(CWinAppEx, AfxGetApp());
// 			if (pApp != NULL)
// 			{
// 				pApp->InitShellManager();
// 			}
// 		}
//
// 		if (afxShellManager == NULL)
// 		{
// 			ASSERT(FALSE);
// 		}
// 		else
// 		{
// 			bUpdate = afxShellManager->BrowseForFolder(strPath, m_pWndList, strPath);
// 		}
// 	}
// 	else
	{
		CFileDialogEx dlg(m_bOpenFileDialog, m_strDefExt, strPath, m_dwFileOpenFlags, m_strFilter, m_pWndList);
		dlg.m_ofn.lpstrInitialDir = CUIMgr::GetUIPathWithoutTitle();
		if (dlg.DoModal() == IDOK)
		{
			bUpdate = TRUE;
			strPath = dlg.GetFileName();
		}
	}

	if (bUpdate)
	{
		if (m_pWndInPlace != NULL)
		{
			m_pWndInPlace->SetWindowText(strPath);
		}

		m_varValue = (LPCTSTR) strPath;
	}

	m_bButtonIsDown = FALSE;
	Redraw();

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetFocus();
	}
	else
	{
		m_pWndList->SetFocus();
	}
}

//////////////////////////////////////////////////////////////////////////
// CIUIMFCPropertyGridCtrl

IMPLEMENT_DYNAMIC(CIUIMFCPropertyGridCtrl, CMFCPropertyGridCtrl)

CIUIMFCPropertyGridCtrl::CIUIMFCPropertyGridCtrl()
{
	m_bHandleEditChanged = TRUE;
}

CIUIMFCPropertyGridCtrl::~CIUIMFCPropertyGridCtrl()
{
}


BEGIN_MESSAGE_MAP(CIUIMFCPropertyGridCtrl, CMFCPropertyGridCtrl)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// public

BOOL CIUIMFCPropertyGridCtrl::EditItem(CMFCPropertyGridProperty* pProp, LPPOINT lptClick)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pProp);

	if (!EndEditItem())
	{
		return FALSE;
	}

	if (pProp->IsGroup() && pProp->GetOptionCount() <= 0)
	{
		return FALSE;
	}

	m_bHandleEditChanged = FALSE;
	if (pProp->OnEdit(lptClick))
	{
		pProp->Redraw();
		SetCurSel(pProp);
		SetCapture();
	}
	m_bHandleEditChanged = TRUE;

	return TRUE;
}

BOOL CIUIMFCPropertyGridCtrl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	WORD wNotify = HIWORD(wParam);
	WORD nID = LOWORD(wParam);
	HWND hEdit = HWND(lParam);

	if (wNotify == EN_CHANGE && m_bHandleEditChanged)
	{
		BOOL bRet = CMFCPropertyGridCtrl::OnCommand(wParam, lParam);
		m_pSel->OnUpdateValue();
		return bRet;
	}

	return CMFCPropertyGridCtrl::OnCommand(wParam, lParam);
}
