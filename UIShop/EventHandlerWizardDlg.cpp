// EventHandlerWizardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UIShop.h"
#include "EventHandlerWizardDlg.h"
#include "ProjectGen.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CEventHandlerWizardDlg dialog

IMPLEMENT_DYNAMIC(CEventHandlerWizardDlg, CDialog)

CEventHandlerWizardDlg::CEventHandlerWizardDlg(CWnd *pParent /*=NULL*/)
	: CDialog(CEventHandlerWizardDlg::IDD, pParent)
	, m_strClassName(_T(""))
	, m_strWindowID(_T(""))
	, m_strHFile(_T(""))
	, m_strCppFile(_T(""))
	, m_strLocation(_T(""))
	, m_strMergeTool(_T(""))
{
	memset(m_szBrowseRoot, 0, sizeof(TCHAR) * MAX_PATH);
}

CEventHandlerWizardDlg::~CEventHandlerWizardDlg()
{
}

void CEventHandlerWizardDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDT_CLASS_NAME, m_edtClassName);
	DDX_Text(pDX, IDC_EDT_CLASS_NAME, m_strClassName);
	DDX_Text(pDX, IDC_EDT_WINDOW_ID, m_strWindowID);
	DDX_Text(pDX, IDC_EDT_H_FILE, m_strHFile);
	DDX_Text(pDX, IDC_EDT_CPP_FILE, m_strCppFile);
	DDX_Text(pDX, IDC_EDT_LOCATION, m_strLocation);
	DDX_Text(pDX, IDC_CMB_MERGE_TOOL_PATH, m_strMergeTool);
	DDX_Control(pDX, IDC_CMB_MERGE_TOOL_PATH, m_cmbMergeToolPath);
}


BEGIN_MESSAGE_MAP(CEventHandlerWizardDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CEventHandlerWizardDlg::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_GOTO, &CEventHandlerWizardDlg::OnBnClickedBtnGoto)
	ON_EN_UPDATE(IDC_EDT_CLASS_NAME, &CEventHandlerWizardDlg::OnEnUpdateEdtClassName)
	ON_BN_CLICKED(IDC_BTN_BROWSE_MERGE_TOOL, &CEventHandlerWizardDlg::OnBnClickedBtnBrowseMergeTool)
	ON_BN_CLICKED(IDC_BTN_MERGE, &CEventHandlerWizardDlg::OnBnClickedBtnMerge)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// protected

int CEventHandlerWizardDlg::SaveSettings()
{
	UpdateData(TRUE);

	CString strFile = CUIMgr::GetUIPathWithoutTitle() + _T("Settings.ini");
	WritePrivateProfileString(g_lpszEventHandlerWizardSection,
		g_lpszLocationKey, m_strLocation, strFile);
	WritePrivateProfileString(g_lpszEventHandlerWizardSection,
		g_lpszMergeToolPath, m_strMergeTool, strFile);

	return 0;
}

int CEventHandlerWizardDlg::Init3thPartyMergeTools()
{
	HKEY hKey = NULL;

	do
	{
		// 解析Araxis Merge的地址
		LPCTSTR lpszRegPath = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\merge.exe");
		long lRet = (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpszRegPath, 0, KEY_ALL_ACCESS, &hKey));
		if (lRet != ERROR_SUCCESS)
		{
			break;
		}

		TCHAR szByte[MAX_PATH] = {0};
		DWORD dwType = REG_SZ;
		DWORD dwDataSize = MAX_PATH * sizeof(TCHAR);

		lRet = RegQueryValueEx(hKey, _T(""), NULL, &dwType, (LPBYTE)szByte, &dwDataSize);

		if (lRet != ERROR_SUCCESS)
		{
			break;
		}

		// 把解析到的地址，插入combo box.
		m_cmbMergeToolPath.InsertString(0, szByte);
	}
	while (false);

	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CClassWizardDlg message handlers

BOOL CEventHandlerWizardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	OnEnUpdateEdtClassName();

	CString strFile = CUIMgr::GetUIPathWithoutTitle() + _T("Settings.ini");
	// Init location
	TCHAR szLocation[MAX_PATH] = {0};
	GetPrivateProfileString(g_lpszEventHandlerWizardSection,
		g_lpszLocationKey, _T(""), szLocation, MAX_PATH, strFile);
	m_strLocation = szLocation;

	GetPrivateProfileString(g_lpszEventHandlerWizardSection,
		g_lpszMergeToolPath, _T(""), m_strMergeTool.GetBufferSetLength(MAX_PATH), MAX_PATH, strFile);
	m_strMergeTool.ReleaseBuffer();

	// 搜索硬盘上已安装的第三方文件对比工具，作为备选
	Init3thPartyMergeTools();

	UpdateData(FALSE);

	m_edtClassName.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEventHandlerWizardDlg::OnOK()
{
	// TODO: Add extra validation here
	BOOL bRet = UpdateData();
	if (!bRet)
	{
		return;
	}

	if (m_strClassName.GetLength() == 0)
	{
		AfxMessageBox(_T("Please input class name."));
		return;
	}

	if (m_strWindowID.GetLength() == 0)
	{
		AfxMessageBox(_T("Please input window ID."));
		return;
	}

	if (m_strHFile.GetLength() == 0)
	{
		AfxMessageBox(_T("Please input .h file name."));
		return;
	}
	if (m_strCppFile.GetLength() == 0)
	{
		AfxMessageBox(_T("Please input .cpp file name."));
		return;
	}

	if (m_strLocation.GetLength() == 0)
	{
		AfxMessageBox(_T("Please input location name."));
		return;
	}
	if (m_strLocation.Right(1) != _T("\\") && m_strLocation.Right(1) != _T("/"))
	{
		m_strLocation += _T("\\");
	}

	if (PathFileExists(m_strLocation + m_strHFile))
	{
		AfxMessageBox(_T("A .h file with the specified name already exists at the specified location."));
		return;
	}

	if (PathFileExists(m_strLocation + m_strCppFile))
	{
		AfxMessageBox(_T("A .cpp file with the specified name already exists at the specified location."));
		return;
	}

	// Save to configuration file
	SaveSettings();

	CDialog::OnOK();
}

int CALLBACK BrowseEventCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	CEventHandlerWizardDlg *pDlg = (CEventHandlerWizardDlg *)lpData;

	if (uMsg == BFFM_INITIALIZED)
	{
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)pDlg->m_szBrowseRoot);
	}

	return 0;
}

void CEventHandlerWizardDlg::OnBnClickedBtnBrowse()
{
	BOOL bRet = UpdateData();
	if (!bRet)
	{
		return;
	}

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
	ZeroMemory((PVOID)&bInfo, sizeof(BROWSEINFO));

	bInfo.hwndOwner = this->GetSafeHwnd();
	bInfo.pszDisplayName = strPath.GetBuffer(MAX_PATH);
	bInfo.lpszTitle = _T("Browse");
	bInfo.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
	bInfo.lpfn = BrowseEventCallbackProc;
	bInfo.lParam = (LPARAM)this;

	if ((pidl = ::SHBrowseForFolder(&bInfo)) == NULL)
	{
		return;
	}

	if (::SHGetPathFromIDList(pidl, strPath.GetBuffer(MAX_PATH)) == FALSE)
	{
		return;
	}

	// to use bInfo.pszDisplayName

	m_strLocation = bInfo.pszDisplayName;
	UpdateData(FALSE);
}

void CEventHandlerWizardDlg::OnBnClickedBtnGoto()
{
	BOOL bRet = UpdateData();
	if (!bRet)
	{
		return;
	}

	int nRet = (int)ShellExecute(NULL, _T("open"), m_strLocation, NULL, NULL, SW_SHOW);
	if (nRet > 32)
	{
		return;    // Successful.
	}

	if (nRet == ERROR_FILE_NOT_FOUND || nRet == ERROR_PATH_NOT_FOUND)
	{
		AfxMessageBox(_T("The specified file or path was not found!"), MB_ICONERROR);
	}
	else
	{
		AfxMessageBox(_T("Failed to open the specified path!"), MB_ICONERROR);
	}
}

void CEventHandlerWizardDlg::OnEnUpdateEdtClassName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.

	// TODO:  Add your control notification handler code here

	BOOL bRet = UpdateData();
	if (!bRet)
	{
		return;
	}

	m_strHFile = m_strClassName;
	if (m_strHFile.Left(1) == _T("C"))
	{
		m_strHFile.Delete(0, 1);
	}

	if (m_strHFile.GetLength() > 0)
	{
		m_strHFile += _T(".h");
	}


	m_strCppFile = m_strClassName;
	if (m_strCppFile.Left(1) == _T("C"))
	{
		m_strCppFile.Delete(0, 1);
	}

	if (m_strCppFile.GetLength() > 0)
	{
		m_strCppFile += _T(".cpp");
	}

	UpdateData(FALSE);
}

void CEventHandlerWizardDlg::OnBnClickedBtnBrowseMergeTool()
{
	CFileDialog dlg(TRUE);
	INT_PTR nRet = dlg.DoModal();
	if (nRet != IDOK)
	{
		return;
	}

	CString strMergeTool = dlg.GetPathName();
	m_cmbMergeToolPath.SetWindowText(strMergeTool);
}

void CEventHandlerWizardDlg::OnBnClickedBtnMerge()
{
	UpdateData(TRUE);

	// Generate temp file to merge
	CString strTempPath;
	GetTempPath(MAX_PATH, strTempPath.GetBufferSetLength(MAX_PATH));
	strTempPath.ReleaseBuffer();

	// Macro Define
	CEventHandlerGen eventHandlerGen;
	eventHandlerGen.SetInputFolder(strTempPath);
	eventHandlerGen.SetClassName(m_strClassName);
	UIWNDPROPERTIES *pWndProperties = CUIMgr::GetUIWNDPROPERTIES(m_strWindowID);
	ASSERT(pWndProperties != NULL);
	eventHandlerGen.SetUIWndProperties(pWndProperties);

	eventHandlerGen.m_Dictionary.RemoveAll();

	// Class Name
	CString strLowerClassName = m_strClassName;
	strLowerClassName.MakeLower();
	eventHandlerGen.m_Dictionary[_T("class")] = strLowerClassName;

	eventHandlerGen.m_Dictionary[_T("Class")] = m_strClassName;

	CString strUpperClassName = m_strClassName;
	strUpperClassName.MakeUpper();
	eventHandlerGen.m_Dictionary[_T("CLASS")] = strUpperClassName;

	// Window ID
	eventHandlerGen.m_Dictionary[_T("WindowID")] = m_strWindowID;

	// .h File
	eventHandlerGen.m_Dictionary[_T("HFile")] = m_strHFile;

	// .cpp File
	eventHandlerGen.m_Dictionary[_T("CppFile")] = m_strCppFile;

	// Code Style
	eventHandlerGen.m_Dictionary[_T("CODESTYLE_VCNET")] = _T("1");

	eventHandlerGen.GenerateEventHandlerFile();

	// 使用第三方Merge工具比较
	if (m_strLocation.Right(1) != _T("\\") && m_strLocation.Right(1) != _T("/"))
	{
		m_strLocation += _T("\\");
	}

	CString strParam = strTempPath + m_strHFile;
	strParam += _T(" ");
	strParam += m_strLocation + m_strHFile;
	ShellExecute(NULL, _T("Open"), m_strMergeTool, strParam, NULL, SW_SHOW);

	strParam = strTempPath + m_strCppFile;
	strParam += _T(" ");
	strParam += m_strLocation + m_strCppFile;
	ShellExecute(NULL, _T("Open"), m_strMergeTool, strParam, NULL, SW_SHOW);

	SaveSettings();
}
