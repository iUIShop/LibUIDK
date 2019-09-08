// GenClassInfoFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UIShop.h"
#include "GenClassInfoFileDlg.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CGenClassInfoFileDlg dialog

IMPLEMENT_DYNAMIC(CGenClassInfoFileDlg, CDialog)

CGenClassInfoFileDlg::CGenClassInfoFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGenClassInfoFileDlg::IDD, pParent)
{
	memset(m_szBrowseRoot, 0, sizeof(TCHAR) * MAX_PATH);
}

CGenClassInfoFileDlg::~CGenClassInfoFileDlg()
{
}

void CGenClassInfoFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_CODE_FOLDER, m_strCodeFolder);
	DDX_Text(pDX, IDC_EDT_CLASS_INFO_FILE, m_strIniFile);
}


BEGIN_MESSAGE_MAP(CGenClassInfoFileDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CGenClassInfoFileDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_BROWSE_CODE_FOLDER, &CGenClassInfoFileDlg::OnBnClickedBtnBrowseCodeFolder)
	ON_BN_CLICKED(IDC_BTN_BROWSE_CLASS_INFO_FILE, &CGenClassInfoFileDlg::OnBnClickedBtnBrowseClassInfoFile)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// protected

int CGenClassInfoFileDlg::ParseHFile(const CString &strHFile, CString &strWindowID, CString &strClassName, BOOL *pbCodeStyleVC6)
{
	FILE *fp = NULL;
	fopen_s(&fp, _bstr_t((LPCTSTR)strHFile), "rb");
	if (fp == NULL)
		return -1;

	CStringA strWindowIDA;
	CStringA strClassNameA;
	BOOL bCodeStyleVC6 = FALSE;

	int nLine = 0;
	while (!feof(fp))
	{
		CHAR szBuf[MAX_PATH * 2] = {0};
		fgets(szBuf, MAX_PATH * 2, fp);

		++nLine;
		CStringA strBufA = szBuf;

		// Code style
		int nMacorPos = strBufA.Find("!defined(AFX_");
		if (nMacorPos >= 0)
		{
			bCodeStyleVC6 = TRUE;
			continue;
		}

		// Class name
		int nClassPos = strBufA.Find("class");
		if (nClassPos >= 0)
		{
			strBufA.Delete(0, nClassPos + 5);
			strBufA.TrimLeft();

			int nMaoHaoPos = strBufA.Find(":");
			if (nMaoHaoPos > 0)
			{
				strBufA.Delete(nMaoHaoPos, strBufA.GetLength() - nMaoHaoPos);
				strBufA.TrimRight();

				strClassNameA = strBufA;

				continue;
			}
		}

		// Window ID
		int nWindowIDPos = strBufA.Find("WindowID");
		if (nWindowIDPos > 0)
		{
			int nEqualPos = strBufA.Find("=");
			if (nEqualPos > 0)
			{
				strBufA.Delete(0, nEqualPos + 1);

				int nFenHaoPos = strBufA.Find(";");
				if (nFenHaoPos > 0)
				{
					strBufA.Delete(nFenHaoPos, strBufA.GetLength() - nFenHaoPos);

					strBufA.TrimLeft();
					strBufA.TrimRight();
					strWindowIDA = strBufA;

					break;
				}
			}
		}
	}

	fclose(fp);

	//
	if (strWindowIDA.IsEmpty())
		return -2;
	if (strClassNameA.IsEmpty())
		return -3;

	strWindowID = (LPCTSTR)_bstr_t((LPCSTR)strWindowIDA);
	strClassName = (LPCTSTR)_bstr_t((LPCSTR)strClassNameA);
	if (pbCodeStyleVC6 != NULL)
	{
		*pbCodeStyleVC6 = bCodeStyleVC6;
	}

	return 0;
}

// CGenClassInfoFileDlg message handlers

void CGenClassInfoFileDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	if (!UpdateData())
		return;

	if (m_strCodeFolder.IsEmpty())
	{
		AfxMessageBox(_T("Please input code folder!"));
		return;
	}

	if (m_strIniFile.IsEmpty())
	{
		AfxMessageBox(_T("Please input ini file path!"));
		return;
	}

	CFileFind finder;
	BOOL bWorking = finder.FindFile(m_strCodeFolder + _T("\\") + _T("*.*"));
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

		if (strExt.Compare(_T(".h")) != 0)
			continue;

		CString strWindowID;
		CString strClassName;
		BOOL bCodeStyleVC6 = FALSE;
		int nRet = ParseHFile(m_strCodeFolder + _T("\\") + strFile, strWindowID, strClassName, &bCodeStyleVC6);
		if (nRet != 0)
			continue;

		// Class Name
		WritePrivateProfileString(strWindowID, g_lpszClassName, strClassName, m_strIniFile);

		// .H file
		WritePrivateProfileString(strWindowID, g_lpszClassHFile, strFile, m_strIniFile);

		// .Cpp file
		CString strCppFile = GetFileTitleEx(strFile);
		strCppFile += _T(".cpp");
		WritePrivateProfileString(strWindowID, g_lpszClassCppFile, strCppFile, m_strIniFile);

		// Code style
		if (bCodeStyleVC6)
			WritePrivateProfileString(strWindowID, g_lpszCodeStyleVC6, _T("1"), m_strIniFile);
		else
			WritePrivateProfileString(strWindowID, g_lpszCodeStyleVC6, _T("0"), m_strIniFile);
	}

	OnOK();
}

int CALLBACK BrowseCodeFolderCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	CGenClassInfoFileDlg *pDlg = (CGenClassInfoFileDlg *)lpData;

	if (uMsg == BFFM_INITIALIZED)
	{
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)pDlg->m_szBrowseRoot);
	}

	return 0;
}

void CGenClassInfoFileDlg::OnBnClickedBtnBrowseCodeFolder()
{
	BOOL bRet = UpdateData();
	if (!bRet)
		return;

	// init browse root
	if (m_strCodeFolder.IsEmpty())
	{
		TSTRCPY(m_szBrowseRoot, MAX_PATH, (LPCTSTR)_bstr_t(CUIMgr::GetUIPathWithoutTitle()));
	}
	else
	{
		TSTRCPY(m_szBrowseRoot, MAX_PATH, (LPCTSTR)_bstr_t(m_strCodeFolder));
	}

	BROWSEINFO bInfo;
	LPITEMIDLIST pidl;
	CString strPath;
	ZeroMemory ((PVOID)&bInfo, sizeof(BROWSEINFO));

	bInfo.hwndOwner = this->GetSafeHwnd();
	bInfo.pszDisplayName = strPath.GetBuffer(MAX_PATH);
	bInfo.lpszTitle = _T("Browse");
	bInfo.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
	bInfo.lpfn = BrowseCodeFolderCallbackProc;
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

	m_strCodeFolder = bInfo.pszDisplayName;
	UpdateData(FALSE);
}

void CGenClassInfoFileDlg::OnBnClickedBtnBrowseClassInfoFile()
{
	BOOL bRet = UpdateData(TRUE);
	if (!bRet)
		return;

	TCHAR szFilters[] = _T("LibUIDK Settings File(*.ini)|*.ini||");
	CFileDialog dlg(FALSE, _T("ini"), _T("Settings2.ini"), OFN_OVERWRITEPROMPT, szFilters);
	dlg.m_ofn.lpstrInitialDir = CUIMgr::GetUIPathWithoutTitle();
	INT_PTR nRet = dlg.DoModal();
	if (nRet == IDCANCEL)
		return;

	m_strIniFile = dlg.GetPathName();

	UpdateData(FALSE);
}
