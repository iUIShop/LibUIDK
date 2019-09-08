// TransformDialogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UIShop.h"
#include "TransformDialogDlg.h"
#include "TransformDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CTransformDialogDlg dialog

IMPLEMENT_DYNAMIC(CTransformDialogDlg, CDialog)

CTransformDialogDlg::CTransformDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransformDialogDlg::IDD, pParent)
{

}

CTransformDialogDlg::~CTransformDialogDlg()
{
}

void CTransformDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDT_RES_SOURCE, m_strResSource);
	DDX_Text(pDX, IDC_EDT_XUI_TARGET, m_strXUITarget);
}


BEGIN_MESSAGE_MAP(CTransformDialogDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_BROWSE_RES, &CTransformDialogDlg::OnBnClickedBtnBrowseRes)
	ON_BN_CLICKED(IDC_BTN_GO_SOURCE, &CTransformDialogDlg::OnBnClickedBtnGoSource)
	ON_BN_CLICKED(IDC_BTN_BROWSE_XUI, &CTransformDialogDlg::OnBnClickedBtnBrowseXui)
	ON_BN_CLICKED(IDC_BTN_GO_TARGET, &CTransformDialogDlg::OnBnClickedBtnGoTarget)
	ON_BN_CLICKED(IDC_BTN_TRANSFORM, &CTransformDialogDlg::OnBnClickedBtnTransform)
END_MESSAGE_MAP()


// CTransformDialogDlg message handlers

void CTransformDialogDlg::OnBnClickedBtnBrowseRes()
{
	if (!UpdateData())
		return;

	TCHAR szFilters[] = _T("All Supported File Types(*.exe, *.rc)|*.exe; *.rc|Exe Files(*.exe)|*.exe|RC Files(*.rc)|*.rc||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, szFilters);
	INT_PTR nRet = dlg.DoModal();
	if (nRet == IDOK)
	{
		m_strResSource = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CTransformDialogDlg::OnBnClickedBtnGoSource()
{
	BOOL bRet = UpdateData();
	if (!bRet)
		return;

	int nRet = (int)ShellExecute(NULL, _T("open"), GetFilePath(m_strResSource), NULL, NULL, SW_SHOW);
	if (nRet > 32)
		return;		// Successful.

	if (nRet == ERROR_FILE_NOT_FOUND || nRet == ERROR_PATH_NOT_FOUND)
	{
		AfxMessageBox(_T("The specified file or path was not found!"), MB_ICONERROR);
	}
	else
	{
		AfxMessageBox(_T("Failed to open the specified path!"), MB_ICONERROR);
	}
}

void CTransformDialogDlg::OnBnClickedBtnBrowseXui()
{
	if (!UpdateData())
		return;

	TCHAR szFilters[] = _T("XUI Files(*.xui)|*.xui||");
	CFileDialog dlg(FALSE, _T("xui"), _T("Untitled.xui"), OFN_HIDEREADONLY, szFilters);
	INT_PTR nRet = dlg.DoModal();
	if (nRet == IDOK)
	{
		m_strXUITarget = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CTransformDialogDlg::OnBnClickedBtnGoTarget()
{
	BOOL bRet = UpdateData();
	if (!bRet)
		return;

	int nRet = (int)ShellExecute(NULL, _T("open"), GetFilePath(m_strXUITarget), NULL, NULL, SW_SHOW);
	if (nRet > 32)
		return;		// Successful.

	if (nRet == ERROR_FILE_NOT_FOUND || nRet == ERROR_PATH_NOT_FOUND)
	{
		AfxMessageBox(_T("The specified file or path was not found!"), MB_ICONERROR);
	}
	else
	{
		AfxMessageBox(_T("Failed to open the specified path!"), MB_ICONERROR);
	}
}

void CTransformDialogDlg::OnBnClickedBtnTransform()
{
	if (!UpdateData())
		return;

	if (m_strResSource.IsEmpty())
	{
		AfxMessageBox(_T("Please input source file"));
		return;
	}

	if (m_strXUITarget.IsEmpty())
	{
		AfxMessageBox(_T("Please input target file"));
		return;
	}

	CTransformDialog td;
	int nRet = td.Transform(m_strResSource, m_strXUITarget);
	if (nRet == -2)
	{
		CString strMsg;
		strMsg.Format(_T("Failded to open source file : %s"), m_strResSource);

		AfxMessageBox(strMsg);
	}

	EndDialog(IDC_BTN_TRANSFORM);
}
