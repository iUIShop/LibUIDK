// AddAllClassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UIShop.h"
#include "AddAllClassDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CAddAllClassDlg dialog

IMPLEMENT_DYNAMIC(CAddAllClassDlg, CDialog)

CAddAllClassDlg::CAddAllClassDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddAllClassDlg::IDD, pParent)
{
	memset(m_szBrowseRoot, 0, sizeof(TCHAR) * MAX_PATH);
}

CAddAllClassDlg::~CAddAllClassDlg()
{
}

void CAddAllClassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_ALL_CLASSED_SAVE_PATH, m_strLocation);
}


BEGIN_MESSAGE_MAP(CAddAllClassDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CAddAllClassDlg::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDOK, &CAddAllClassDlg::OnBnClickedOk)
END_MESSAGE_MAP()


int CALLBACK BrowseAllClassCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	CAddAllClassDlg *pDlg = (CAddAllClassDlg *)lpData;

	if (uMsg == BFFM_INITIALIZED)
	{
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)pDlg->m_szBrowseRoot);
	}

	return 0;
}

// CAddAllClassDlg message handlers

void CAddAllClassDlg::OnBnClickedBtnBrowse()
{
	BOOL bRet = UpdateData();
	if (!bRet)
		return;

	// init browse root
	if (m_strLocation.IsEmpty())
	{
		TSTRCPY(m_szBrowseRoot, MAX_PATH, (LPCTSTR)_bstr_t(CUIMgr::GetUIPathWithoutTitle()));
	}
	else
	{
		TSTRCPY(m_szBrowseRoot, MAX_PATH, (LPCTSTR)_bstr_t(m_strLocation));
	}

	BROWSEINFO bInfo;
	LPITEMIDLIST pidl;
	CString strPath;
	ZeroMemory ((PVOID)&bInfo, sizeof(BROWSEINFO));

	bInfo.hwndOwner = this->GetSafeHwnd();
	bInfo.pszDisplayName = strPath.GetBuffer(MAX_PATH);
	bInfo.lpszTitle = _T("Browse");
	bInfo.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
	bInfo.lpfn = BrowseAllClassCallbackProc;
	bInfo.lParam = (LPARAM)this;

	if ((pidl = ::SHBrowseForFolder(&bInfo) ) == NULL)
	{
		return;
	}

	if (::SHGetPathFromIDList(pidl, strPath.GetBuffer(MAX_PATH) ) == FALSE)
	{
		return;
	}

	// to use bInfo.pszDisplayName

	m_strLocation = bInfo.pszDisplayName;
	UpdateData(FALSE);
}

void CAddAllClassDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	BOOL bRet = UpdateData();
	if (!bRet)
		return;

	if (m_strLocation.GetLength() == 0)
	{
		AfxMessageBox(_T("Please input save path."));
		return;
	}

	OnOK();
}
