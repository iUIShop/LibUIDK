// FileDialogEx.cpp : implementation file
//

#include "stdafx.h"
#include "uishop.h"
#include "FileDialogEx.h"
#include <dlgs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileDialogEx

IMPLEMENT_DYNAMIC(CFileDialogEx, CFileDialog)

CFileDialogEx::CFileDialogEx(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszImageName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszImageName, dwFlags, lpszFilter, pParentWnd)
{
}


BEGIN_MESSAGE_MAP(CFileDialogEx, CFileDialog)
	//{{AFX_MSG_MAP(CFileDialogEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CFileDialogEx::OnInitDialog()
{
	CFileDialog::OnInitDialog();

	// TODO: Add extra initialization here

	// Hide the combo box and toolbar
	CWnd *pPWnd = GetParent(); // all controls are the children of CFileDialog's parent
	CWnd *pWnd = NULL;
	CRect rcWin;

	pWnd = pPWnd->GetDlgItem(stc4); // the "Look In:" static
	if (pWnd->GetSafeHwnd() != NULL)
		pWnd->ShowWindow(SW_HIDE);

	pWnd = pPWnd->GetDlgItem(cmb2); // dropdown box
	if (pWnd->GetSafeHwnd() != NULL)
	{
		pWnd->GetWindowRect(rcWin);
		pPWnd->ScreenToClient(rcWin);
		pWnd->ShowWindow(SW_HIDE);
	}

	pWnd = pPWnd->GetDlgItem(edt1); // Edit, to input file name
	if (pWnd->GetSafeHwnd() != NULL) // build with VC6, the handle is valid, but with VC2003, the control is a combo box
		pWnd->EnableWindow(FALSE);

	pWnd = pPWnd->GetDlgItem(cmb13); // the File name combobox build with VC2003
	if (pWnd->GetSafeHwnd() != NULL)
		pWnd->EnableWindow(FALSE);

	pWnd = pPWnd->GetDlgItem(0x4A0); // the left tool bar build with VC2003
	if (pWnd->GetSafeHwnd() != NULL)
		pWnd->EnableWindow(FALSE);

	CRect rcToolbar;
	rcToolbar.SetRect(0, 0, 0, 0);
	pWnd = pPWnd->GetWindow(GW_CHILD);
	while(pWnd != NULL)
	{
		TCHAR szName[MAX_PATH] = {0};
		GetClassName(pWnd->GetSafeHwnd(), szName, MAX_PATH-1);
		if (_tcscmp(szName, _T("ToolbarWindow32")) == 0)
		{
			pWnd->GetWindowRect(rcToolbar);
			pPWnd->ScreenToClient(rcToolbar);
			pWnd->ShowWindow(SW_HIDE);
			break;
		}

		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
	if (rcToolbar.right != 0)
	{
		rcWin.right = rcToolbar.right;
	}

	// create an edit replace the combo box and toolbar
	CFont *pFont = GetFont();
	m_edit.Create(ES_AUTOHSCROLL|ES_READONLY|WS_VISIBLE, rcWin, pPWnd, 1234);
	m_edit.SetFont(pFont);
	m_edit.SetWindowText(_T("The bitmaps and the ui file must in the same folder"));

	// modify the title of the CFileDialog
	CString strTitle;
	pPWnd->GetWindowText(strTitle);
	strTitle += " - ";
	TCHAR szDisc[_MAX_DRIVE] = {0};
	TCHAR szFolder[_MAX_PATH] = {0};
	TSPLITPATH(m_ofn.lpstrInitialDir, szDisc, _MAX_DRIVE, szFolder, _MAX_PATH, NULL, 0, NULL, 0);
	strTitle += szDisc;
	strTitle += szFolder;
	pPWnd->SetWindowText(strTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
