// RenameTemplateFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UIShop.h"
#include "RenameTemplateFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CRenameTemplateFileDlg dialog

IMPLEMENT_DYNAMIC(CRenameTemplateFileDlg, CDialog)

CRenameTemplateFileDlg::CRenameTemplateFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRenameTemplateFileDlg::IDD, pParent)
	, m_strTemplatePath(_T(""))
	, m_strPrefix(_T(""))
	, m_strTargetPath(_T(""))
{

}

CRenameTemplateFileDlg::~CRenameTemplateFileDlg()
{
}

void CRenameTemplateFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_TEMPLATE_PATH, m_strTemplatePath);
	DDX_Text(pDX, IDC_EDT_PREFIX, m_strPrefix);
	DDX_Text(pDX, IDC_EDT_TARGET, m_strTargetPath);
}


BEGIN_MESSAGE_MAP(CRenameTemplateFileDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CRenameTemplateFileDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_BROWSE_TEMPLATE, &CRenameTemplateFileDlg::OnBnClickedBtnBrowseTemplate)
	ON_BN_CLICKED(IDC_BTN_BROWSE_TARGET, &CRenameTemplateFileDlg::OnBnClickedBtnBrowseTarget)
END_MESSAGE_MAP()


// CRenameTemplateFileDlg message handlers

void CRenameTemplateFileDlg::OnBnClickedBtnBrowseTemplate()
{
	BOOL bRet = UpdateData();
	if (!bRet)
		return;

	BROWSEINFO bInfo;
	LPITEMIDLIST pidl;
	CString strPath;
	ZeroMemory ((PVOID)&bInfo, sizeof(BROWSEINFO));

	bInfo.hwndOwner = this->GetSafeHwnd();
	bInfo.pszDisplayName = strPath.GetBuffer(MAX_PATH);
	bInfo.lpszTitle = _T("Browse");
	bInfo.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
	bInfo.lpfn = NULL;
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

	m_strTemplatePath = bInfo.pszDisplayName;
	UpdateData(FALSE);
}

void CRenameTemplateFileDlg::OnBnClickedBtnBrowseTarget()
{
	BOOL bRet = UpdateData();
	if (!bRet)
		return;

	BROWSEINFO bInfo;
	LPITEMIDLIST pidl;
	CString strPath;
	ZeroMemory ((PVOID)&bInfo, sizeof(BROWSEINFO));

	bInfo.hwndOwner = this->GetSafeHwnd();
	bInfo.pszDisplayName = strPath.GetBuffer(MAX_PATH);
	bInfo.lpszTitle = _T("Browse");
	bInfo.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
	bInfo.lpfn = NULL;
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

	m_strTargetPath = bInfo.pszDisplayName;
	UpdateData(FALSE);
}

void CRenameTemplateFileDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	if (!UpdateData())
		return;

	if (m_strTemplatePath.IsEmpty())
	{
		AfxMessageBox(_T("Please input template path!"));
		return;
	}
	if (m_strTargetPath.IsEmpty())
	{
		AfxMessageBox(_T("Please input target path!"));
		return;
	}
	if (m_strTemplatePath.CompareNoCase(m_strTargetPath) == 0)
	{
		AfxMessageBox(_T("The template path is same as target path!"));
		return;
	}

	//
	FILE *fpResource = NULL;
	fopen_s(&fpResource, _bstr_t(m_strTargetPath + _T("\\LibUIDK.rc")), "wb");
	if (fpResource == NULL)
		return;

	FILE *fpNewProject = NULL;
	fopen_s(&fpNewProject, _bstr_t(m_strTargetPath + _T("\\IUINewProj.inf")), "wb");
	if (fpNewProject == NULL)
		return;

	CFileFind finder;
	BOOL bWorking = finder.FindFile(m_strTemplatePath + _T("\\") + _T("*.*"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!
		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
			continue;

		CString strFile = finder.GetFileName();

		CString strExt = GetFileExt(strFile, TRUE);

		if (strExt.Compare(_T(".png")) != 0 && strExt.Compare(_T(".bmp")) != 0)
			continue;

		CString strTargetFile = m_strPrefix;
		strTargetFile += strFile;

		CopyFile(m_strTemplatePath + _T("\\") + strFile, m_strTargetPath + _T("\\") + strTargetFile, TRUE);

		// Write to rc file
		CStringA strRc;
		strRc = (LPCSTR)_bstr_t((LPCTSTR)strTargetFile);
		strRc.MakeUpper();

		int nCount = strRc.GetLength();
		if (nCount < 24)
		{
			for (int i = 0; i < 24 - nCount; ++i)
			{
				strRc += " ";
			}
		}
		else
		{
			strRc += " ";
		}

		strRc += "TEMPLATE                \"Template\\\\";
		strRc += (LPCSTR)_bstr_t(strTargetFile);
		strRc += "\"\r\n";
		fputs(strRc, fpResource);

		CStringA strNewProj;
		strNewProj = "=";
		strNewProj += (LPCSTR)_bstr_t(strTargetFile);
		strNewProj += "	Bin\\skins\\$$root$$\\";
		strNewProj += (LPCSTR)_bstr_t(strFile);
		strNewProj += "\r\n";
		fputs(strNewProj, fpNewProject);
	}

	fclose(fpResource);
	fclose(fpNewProject);

	OnOK();
}
