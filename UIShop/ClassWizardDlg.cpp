// ClassWizardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uishop.h"
#include "ClassWizardDlg.h"
#include <shlwapi.h>
#include "Global.h"
#include "ProjectGen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CClassWizardDlg dialog


CClassWizardDlg::CClassWizardDlg(CWnd *pParent /*=NULL*/)
	: CDialog(CClassWizardDlg::IDD, pParent)
	, m_strClassName(_T(""))
	, m_strWindowID(_T(""))
	, m_strHFile(_T(""))
	, m_strCppFile(_T(""))
	, m_strLocation(_T(""))
	, m_strMergeTool(_T(""))
	, m_bGenEventHandler(TRUE)
	, m_bGenVariable(TRUE)
	, m_bGenDoDataExchange(TRUE)
{
	//{{AFX_DATA_INIT(CClassWizardDlg)
	//}}AFX_DATA_INIT
	memset(m_szBrowseRoot, 0, sizeof(TCHAR) * MAX_PATH);
}


void CClassWizardDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClassWizardDlg)
	DDX_Control(pDX, IDC_EDT_CLASS_NAME, m_edtClassName);
	DDX_Control(pDX, IDC_CMB_MERGE_TOOL_PATH, m_cmbMergeToolPath);
	DDX_Text(pDX, IDC_EDT_CLASS_NAME, m_strClassName);
	DDX_Text(pDX, IDC_EDT_WINDOW_ID, m_strWindowID);
	DDX_Text(pDX, IDC_EDT_H_FILE, m_strHFile);
	DDX_Text(pDX, IDC_EDT_CPP_FILE, m_strCppFile);
	DDX_Text(pDX, IDC_EDT_LOCATION, m_strLocation);
	DDX_Text(pDX, IDC_CMB_MERGE_TOOL_PATH, m_strMergeTool);
	DDX_Check(pDX, IDC_CHK_GEN_HANDLER, m_bGenEventHandler);
	DDX_Check(pDX, IDC_CHK_GEN_VARIABLE, m_bGenVariable);
	DDX_Check(pDX, IDC_CHK_GEN_DO_DATA_EXCHANGE, m_bGenDoDataExchange);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClassWizardDlg, CDialog)
	//{{AFX_MSG_MAP(CClassWizardDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CClassWizardDlg::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_GOTO, &CClassWizardDlg::OnBnClickedBtnGoto)
	ON_EN_UPDATE(IDC_EDT_CLASS_NAME, &CClassWizardDlg::OnEnUpdateEdtClassName)
	ON_BN_CLICKED(IDC_CHK_GEN_HANDLER, &CClassWizardDlg::OnChkGenerateHandler)
	ON_BN_CLICKED(IDC_BTN_BROWSE_MERGE_TOOL, &CClassWizardDlg::OnBnClickedBtnBrowseMergeTool)
	ON_BN_CLICKED(IDC_BTN_MERGE, &CClassWizardDlg::OnBnClickedBtnMerge)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// protected

int CClassWizardDlg::SaveSettings()
{
	CString strFile = CUIMgr::GetUIPathWithoutTitle() + _T("Settings.ini");
	WritePrivateProfileString(g_lpszClassWizardSection, g_lpszLocationKey, m_strLocation, strFile);

	WritePrivateProfileString(g_lpszClassWizardSection, g_lpszMergeToolPath, m_strMergeTool, strFile);

	// Class Name
	WritePrivateProfileString(m_strWindowID, g_lpszClassName, m_strClassName, strFile);

	// .H file
	WritePrivateProfileString(m_strWindowID, g_lpszClassHFile, m_strHFile, strFile);

	// .Cpp file
	WritePrivateProfileString(m_strWindowID, g_lpszClassCppFile, m_strCppFile, strFile);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CClassWizardDlg message handlers

BOOL CClassWizardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	CString strFile = CUIMgr::GetUIPathWithoutTitle() + _T("Settings.ini");

	// Init class name
	TCHAR szClassName[MAX_PATH] = {0};
	GetPrivateProfileString(m_strWindowID, g_lpszClassName, _T(""), szClassName, MAX_PATH, strFile);
	m_strClassName = szClassName;

	// Update .h and .cpp file name
	UpdateData(FALSE); // OnEnUpdateEdtClassName need m_strClassName.
	OnEnUpdateEdtClassName();

	// .H file
	TCHAR szClassHFile[MAX_PATH] = {0};
	GetPrivateProfileString(m_strWindowID, g_lpszClassHFile, _T(""), szClassHFile, MAX_PATH, strFile);
	if (_tcslen(szClassHFile) > 0)
	{
		m_strHFile = szClassHFile;
	}

	// .Cpp file
	TCHAR szClassCppFile[MAX_PATH] = {0};
	GetPrivateProfileString(m_strWindowID, g_lpszClassCppFile, _T(""), szClassCppFile, MAX_PATH, strFile);
	if (_tcslen(szClassCppFile) > 0)
	{
		m_strCppFile = szClassCppFile;
	}

	// Init location
	TCHAR szLocation[MAX_PATH] = {0};
	GetPrivateProfileString(g_lpszClassWizardSection, g_lpszLocationKey, _T(""), szLocation, MAX_PATH, strFile);
	m_strLocation = szLocation;

	GetPrivateProfileString(g_lpszClassWizardSection,
		g_lpszMergeToolPath, _T(""), m_strMergeTool.GetBufferSetLength(MAX_PATH), MAX_PATH, strFile);
	m_strMergeTool.ReleaseBuffer();

	UpdateData(FALSE);

	OnChkGenerateHandler();

	m_edtClassName.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CClassWizardDlg::OnOK()
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

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	CClassWizardDlg *pDlg = (CClassWizardDlg *)lpData;

	if (uMsg == BFFM_INITIALIZED)
	{
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)pDlg->m_szBrowseRoot);
	}

	return 0;
}

void CClassWizardDlg::OnBnClickedBtnBrowse()
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
	bInfo.lpfn = BrowseCallbackProc;
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

void CClassWizardDlg::OnBnClickedBtnGoto()
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

void CClassWizardDlg::OnEnUpdateEdtClassName()
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

	m_strHFile = GenerateHFile(m_strClassName);
	m_strCppFile = GenerateCppFile(m_strClassName);

	UpdateData(FALSE);
}

void CClassWizardDlg::OnChkGenerateHandler()
{
	CButton *pChkGenHandler = (CButton *)GetDlgItem(IDC_CHK_GEN_HANDLER);
	ASSERT(pChkGenHandler != NULL);
	CButton *pChkGenVariable = (CButton *)GetDlgItem(IDC_CHK_GEN_VARIABLE);
	ASSERT(pChkGenVariable != NULL);

	int nCheck = pChkGenHandler->GetCheck();
	if (nCheck == BST_CHECKED)
	{
		pChkGenVariable->SetCheck(BST_CHECKED);
		pChkGenVariable->EnableWindow(FALSE);
	}
	else
	{
		pChkGenVariable->EnableWindow(TRUE);
	}
}

void CClassWizardDlg::OnBnClickedBtnBrowseMergeTool()
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


void CClassWizardDlg::OnBnClickedBtnMerge()
{
	UpdateData(TRUE);

	// Generate temp file to merge
	CString strTempPath;
	GetTempPath(MAX_PATH, strTempPath.GetBufferSetLength(MAX_PATH));
	strTempPath.ReleaseBuffer();

	// Macro Define
	CClassGen classGen;
	classGen.SetInputFolder(strTempPath);
	classGen.SetClassName(m_strClassName);
	UIWNDPROPERTIES *pWndProperties = CUIMgr::GetUIWNDPROPERTIES(m_strWindowID);
	ASSERT(pWndProperties != NULL);
	classGen.SetUIWndProperties(pWndProperties);

	classGen.m_Dictionary.RemoveAll();

	// Class Name
	CString strLowerClassName = m_strClassName;
	strLowerClassName.MakeLower();
	classGen.m_Dictionary[_T("class")] = strLowerClassName;

	classGen.m_Dictionary[_T("Class")] = m_strClassName;

	CString strUpperClassName = m_strClassName;
	strUpperClassName.MakeUpper();
	classGen.m_Dictionary[_T("CLASS")] = strUpperClassName;

	// Window ID
	classGen.m_Dictionary[_T("WindowID")] = m_strWindowID;

	// .h File
	classGen.m_Dictionary[_T("HFile")] = m_strHFile;

	// .cpp File
	classGen.m_Dictionary[_T("CppFile")] = m_strCppFile;

	// Generate event handler
	if (m_bGenEventHandler)
	{
		classGen.m_Dictionary[_T("GEN_HANDLER")] = _T("1");
	}

	// Generate control variable
	if (m_bGenVariable)
	{
		classGen.m_Dictionary[_T("GEN_VARIABLE")] = _T("1");
	}

	// Generate DoDataExchange
	if (m_bGenDoDataExchange)
	{
		classGen.m_Dictionary[_T("GEN_DO_DATA_EXCHANGE")] = _T("1");
	}

	//
	void *pData = NULL;
	DWORD dwSize = -1;
	int nRet = classGen.LoadCustomResource("NewClass.inf", &pData, &dwSize);
	if (nRet != 0)
	{
		AfxMessageBox(_T("Can't load \"NewClass.inf\" file!"));
		return;
	}

	nRet = classGen.LoadTemplate(pData, dwSize);
	if (nRet != 0)
	{
		AfxMessageBox(_T("Can't parse \"NewClass.inf\" file!"));
		return;
	}

	nRet = classGen.ProcessTemplate();
	if (nRet != 0)
	{
		AfxMessageBox(_T("Failed to new class!"));
		return;
	}

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
