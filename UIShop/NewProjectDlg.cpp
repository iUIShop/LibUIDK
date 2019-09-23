// NewProjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uishop.h"
#include "NewProjectDlg.h"
#include <shlwapi.h>
#include <strsafe.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewProjectDlg dialog


CNewProjectDlg::CNewProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewProjectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewProjectDlg)
	m_bActiveX = FALSE;
	m_bSockets = FALSE;
	m_strPrjName = _T("");
	m_strLocation = _T("");
	//}}AFX_DATA_INIT
	m_bDatabase = FALSE;
	m_uVSEdition = IDC_RAD_VC60;
	m_uProType = IDC_RAD_PROJECT_GENERAL;
}


void CNewProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewProjectDlg)
	DDX_Check(pDX, IDC_CHK_ACTIVEX, m_bActiveX);
	DDX_Check(pDX, IDC_CHK_SOCKET, m_bSockets);
	DDX_Text(pDX, IDC_EDT_PROJECTNAME, m_strPrjName);
	DDV_MaxChars(pDX, m_strPrjName, 64);
	DDX_Text(pDX, IDC_EDT_LOCATION, m_strLocation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewProjectDlg, CDialog)
	//{{AFX_MSG_MAP(CNewProjectDlg)
	ON_BN_CLICKED(IDC_BTN_BROWSER, OnBtnBrowser)
	ON_BN_CLICKED(IDC_BTN_GOTO, &CNewProjectDlg::OnBnClickedBtnGoto)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewProjectDlg message handlers

void CNewProjectDlg::OnOK()
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if (m_strPrjName.GetLength() == 0)
	{
		AfxMessageBox(_T("Please input project name"));
		return;
	}

	if (m_strLocation.GetLength() == 0)
	{
		AfxMessageBox(_T("Please select location path"));
		return;
	}

	if (m_strLocation.Right(1) != _T("\\") && m_strLocation.Right(1) != _T("/"))
		m_strLocation += _T("\\");

	if (PathFileExists(m_strLocation + m_strPrjName))
	{
		AfxMessageBox(_T("A workspace with the specified name already exists at the specified location."));
		return;
	}
	else
	{
		BOOL bRet = CreateDirectory(m_strLocation + m_strPrjName, 0);
		if (!bRet)
		{
			CString str;
			str.Format(_T("Failed to create folder:\"%s\"!"), m_strLocation+m_strPrjName);
			AfxMessageBox(str, MB_ICONERROR);
			return;
		}
	}

	m_uVSEdition = GetCheckedRadioButton(IDC_RAD_VC60, IDC_RAD_VC140);
	m_uProType = GetCheckedRadioButton(IDC_RAD_PROJECT_GENERAL, IDC_RAD_PROJECT_WEISHI);
	m_uDBID = GetCheckedRadioButton(IDC_RAD_NONE, IDC_RAD_HEADER);
	m_uCommentID = GetCheckedRadioButton(IDC_RAD_YES, IDC_RAD_NO);

	//
	// 保存 vs版本：m_uVSEdition和location：m_strLocation
	//
	CString strSettingsFile = GetExecutePath() + _T("Settings.ini");
	TCHAR szBuf[MAX_PATH] = {0};

	StringCchPrintf(szBuf, MAX_PATH, _T("%d"), m_uVSEdition);
	WritePrivateProfileString(_T("NewProject"), _T("VSEdition"), szBuf, strSettingsFile);
	
	WritePrivateProfileString(_T("NewProject"), _T("Location"), m_strLocation, strSettingsFile);

	CDialog::OnOK();
}

void CNewProjectDlg::OnBtnBrowser()
{
	// TODO: Add your control notification handler code here

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

	m_strLocation = bInfo.pszDisplayName;
	UpdateData(FALSE);
}

void CNewProjectDlg::OnBnClickedBtnGoto()
{
	BOOL bRet = UpdateData();
	if (!bRet)
		return;

	int nRet = (int)ShellExecute(NULL, _T("open"), m_strLocation, NULL, NULL, SW_SHOW);
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

BOOL CNewProjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	
	//
	// 读取 vs版本：m_uVSEdition和location：m_strLocation
	//
	CString strFile = GetExecutePath() + _T("Settings.ini");
	
	// Project type
	CButton *pBtn = NULL;
	int nID = GetPrivateProfileInt(_T("NewProject"), _T("VSEdition"), 0, strFile);
	if (0 == nID)
	{
		pBtn = (CButton *)GetDlgItem(IDC_RAD_VC60);
	}
	else
	{
		pBtn = (CButton *)GetDlgItem(nID);
	}
	if (NULL != pBtn)
	{
		pBtn->SetCheck(BST_CHECKED);
	}
	
	TCHAR szBuf[MAX_PATH] = {0};
	GetPrivateProfileString(_T("NewProject"), _T("Location"), _T(""), szBuf, MAX_PATH, strFile);
	m_strLocation = szBuf;

	pBtn = (CButton *)GetDlgItem(IDC_RAD_PROJECT_GENERAL);
	if (NULL != pBtn)
	{
		pBtn->SetCheck(BST_CHECKED);
	}

	pBtn = (CButton *)GetDlgItem(IDC_RAD_NONE);
	if (NULL != pBtn)
	{
		pBtn->SetCheck(BST_CHECKED);
	}

	pBtn = (CButton *)GetDlgItem(IDC_RAD_YES);
	if (NULL != pBtn)
	{
		pBtn->SetCheck(BST_CHECKED);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
