// UIShop.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "UIShop.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "UIShopDoc.h"
#include "UIShopView.h"
#include "PromptDocManager.h"
#include "FullPathRecentFileList.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

AFX_STATIC_DATA const TCHAR _afxFileSection[] = _T("Recent File List");
AFX_STATIC_DATA const TCHAR _afxFileEntry[] = _T("File%d");
AFX_STATIC_DATA const TCHAR _afxPreviewSection[] = _T("Settings");
AFX_STATIC_DATA const TCHAR _afxPreviewEntry[] = _T("PreviewPages");

//////////////////////////////////////////////////////////////////////////
// CPromptDocManager2

AFX_STATIC_DATA const TCHAR _afxShellOpenFmt[] = _T("%s\\shell\\open\\%s");
AFX_STATIC_DATA const TCHAR _afxShellPrintFmt[] = _T("%s\\shell\\print\\%s");
AFX_STATIC_DATA const TCHAR _afxShellPrintToFmt[] = _T("%s\\shell\\printto\\%s");
AFX_STATIC_DATA const TCHAR _afxDefaultIconFmt[] = _T("%s\\DefaultIcon");
AFX_STATIC_DATA const TCHAR _afxShellNewFmt[] = _T("%s\\ShellNew");

#define DEFAULT_ICON_INDEX 0

AFX_STATIC_DATA const TCHAR _afxIconIndexFmt[] = _T(",%d");
AFX_STATIC_DATA const TCHAR _afxCommand[] = _T("command");
AFX_STATIC_DATA const TCHAR _afxOpenArg[] = _T(" \"%1\"");
AFX_STATIC_DATA const TCHAR _afxPrintArg[] = _T(" /p \"%1\"");
AFX_STATIC_DATA const TCHAR _afxPrintToArg[] = _T(" /pt \"%1\" \"%2\" \"%3\" \"%4\"");
AFX_STATIC_DATA const TCHAR _afxDDEArg[] = _T(" /dde");

AFX_STATIC_DATA const TCHAR _afxDDEExec[] = _T("ddeexec");
AFX_STATIC_DATA const TCHAR _afxDDEOpen[] = _T("[open(\"%1\")]");
AFX_STATIC_DATA const TCHAR _afxDDEPrint[] = _T("[print(\"%1\")]");
AFX_STATIC_DATA const TCHAR _afxDDEPrintTo[] = _T("[printto(\"%1\",\"%2\",\"%3\",\"%4\")]");

AFX_STATIC_DATA const TCHAR _afxShellNewValueName[] = _T("NullFile");
AFX_STATIC_DATA const TCHAR _afxShellNewValue[] = _T("");

class CPromptDocManager2 : public CPromptDocManager
{
public:
	CPromptDocManager2() {}
	~CPromptDocManager2() {}

public:
	virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate)
	{
		CFileDialog dlgFile(bOpenFileDialog);

		CString title;
		VERIFY(title.LoadString(nIDSTitle));

		dlgFile.m_ofn.Flags |= lFlags;

		CString strFilter;

		// append the "*.ui"
		strFilter = _T("Compatible UI File (*.ui)");
		strFilter += (TCHAR)'\0';   // next string please
		strFilter += _T("*.ui");
		strFilter += (TCHAR)'\0';   // last string
		dlgFile.m_ofn.nMaxCustFilter++;

		// append the "*.xui:
		CString strDefault;
		_AfxAppendFilterSuffix2(m_strProjectFilterExt, m_strProjectFilterName, strFilter, dlgFile.m_ofn, &strDefault);

		// append the "*.*" all files filter
		CString allFilter;
		VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
		strFilter += allFilter;
		strFilter += (TCHAR)'\0';   // next string please
		strFilter += _T("*.*");
		strFilter += (TCHAR)'\0';   // last string
		dlgFile.m_ofn.nMaxCustFilter++;

		CString strExt = GetFileExt(fileName, TRUE);
		if (strExt.IsEmpty())
			dlgFile.m_ofn.lpstrDefExt = _T("xui");
		else
		{
			if (strExt.Left(1) == '.')
				strExt.Delete(0, 1);
			dlgFile.m_ofn.lpstrDefExt = strExt;
		}
		dlgFile.m_ofn.nFilterIndex = 2;
		dlgFile.m_ofn.lpstrFilter = strFilter;
		dlgFile.m_ofn.lpstrTitle = title;
		dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

		INT_PTR nResult = dlgFile.DoModal();
		fileName.ReleaseBuffer();
		return nResult == IDOK;
	}

	void RegisterShellFileTypes(BOOL bCompat)
	{
		bCompat = FALSE;
		ASSERT(!m_templateList.IsEmpty());  // must have some doc templates

		CString strPathName, strTemp;

		AfxGetModuleShortFileName(AfxGetInstanceHandle(), strPathName);

		POSITION pos = m_templateList.GetHeadPosition();
		for (int nTemplateIndex = 1; pos != NULL; nTemplateIndex++)
		{
			CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);

			CString strOpenCommandLine = strPathName;
			CString strPrintCommandLine = strPathName;
			CString strPrintToCommandLine = strPathName;
			CString strDefaultIconCommandLine = strPathName;

			//		if (bCompat)
			{
				CString strIconIndex;
				HICON hIcon = ::ExtractIcon(AfxGetInstanceHandle(), strPathName, nTemplateIndex);
				if (hIcon != NULL)
				{
					strIconIndex.Format(_afxIconIndexFmt, nTemplateIndex);
					DestroyIcon(hIcon);
				}
				else
				{
					strIconIndex.Format(_afxIconIndexFmt, DEFAULT_ICON_INDEX);
				}
				strDefaultIconCommandLine += strIconIndex;
			}

			CString strFilterExt, strFileTypeId, strFileTypeName;
			if (pTemplate->GetDocString(strFileTypeId,
				CDocTemplate::regFileTypeId) && !strFileTypeId.IsEmpty())
			{
				// enough info to register it
				if (!pTemplate->GetDocString(strFileTypeName,
					CDocTemplate::regFileTypeName))
					strFileTypeName = strFileTypeId;    // use id name

				ASSERT(strFileTypeId.Find(' ') == -1);  // no spaces allowed

				// first register the type ID of our server
				if (!IUISetRegKey(strFileTypeId, strFileTypeName, NULL))
					continue;       // just skip it

				//			if (bCompat)
				{
					// path\DefaultIcon = path,1
					strTemp.Format(_afxDefaultIconFmt, (LPCTSTR)strFileTypeId);
					if (!IUISetRegKey(strTemp, strDefaultIconCommandLine, NULL))
						continue;       // just skip it
				}

				// If MDI Application
				if (!pTemplate->GetDocString(strTemp, CDocTemplate::windowTitle) ||
					strTemp.IsEmpty())
				{
					// 				// path\shell\open\ddeexec = [open("%1")]
					// 				strTemp.Format(_afxShellOpenFmt, (LPCTSTR)strFileTypeId,
					// 					(LPCTSTR)_afxDDEExec);
					// 				if (!IUISetRegKey(strTemp, _afxDDEOpen))
					// 					continue;       // just skip it

					if (bCompat)
					{
						// path\shell\print\ddeexec = [print("%1")]
						strTemp.Format(_afxShellPrintFmt, (LPCTSTR)strFileTypeId,
							(LPCTSTR)_afxDDEExec);
						if (!IUISetRegKey(strTemp, _afxDDEPrint, NULL))
							continue;       // just skip it

						// path\shell\printto\ddeexec = [printto("%1","%2","%3","%4")]
						strTemp.Format(_afxShellPrintToFmt, (LPCTSTR)strFileTypeId,
							(LPCTSTR)_afxDDEExec);
						if (!IUISetRegKey(strTemp, _afxDDEPrintTo, NULL))
							continue;       // just skip it

						// path\shell\open\command = path /dde
						// path\shell\print\command = path /dde
						// path\shell\printto\command = path /dde
						strOpenCommandLine += _afxDDEArg;
						strPrintCommandLine += _afxDDEArg;
						strPrintToCommandLine += _afxDDEArg;
					}
					else
					{
						strOpenCommandLine += _afxOpenArg;
					}
				}
				else
				{
					// path\shell\open\command = path filename
					// path\shell\print\command = path /p filename
					// path\shell\printto\command = path /pt filename printer driver port
					strOpenCommandLine += _afxOpenArg;
					if (bCompat)
					{
						strPrintCommandLine += _afxPrintArg;
						strPrintToCommandLine += _afxPrintToArg;
					}
				}

				// path\shell\open\command = path filename
				strTemp.Format(_afxShellOpenFmt, (LPCTSTR)strFileTypeId,
					(LPCTSTR)_afxCommand);
				if (!IUISetRegKey(strTemp, strOpenCommandLine, NULL))
					continue;       // just skip it

				if (bCompat)
				{
					// path\shell\print\command = path /p filename
					strTemp.Format(_afxShellPrintFmt, (LPCTSTR)strFileTypeId,
						(LPCTSTR)_afxCommand);
					if (!IUISetRegKey(strTemp, strPrintCommandLine, NULL))
						continue;       // just skip it

					// path\shell\printto\command = path /pt filename printer driver port
					strTemp.Format(_afxShellPrintToFmt, (LPCTSTR)strFileTypeId,
						(LPCTSTR)_afxCommand);
					if (!IUISetRegKey(strTemp, strPrintToCommandLine, NULL))
						continue;       // just skip it
				}

				pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt);
				if (!strFilterExt.IsEmpty())
				{
					ASSERT(strFilterExt[0] == '.');

					LONG lSize = _MAX_PATH * 2;
					LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
						strTemp.GetBuffer(lSize), &lSize);
					strTemp.ReleaseBuffer();

					if (lResult != ERROR_SUCCESS || strTemp.IsEmpty() ||
						strTemp == strFileTypeId)
					{
						// no association for that suffix
						if (!IUISetRegKey(strFilterExt, strFileTypeId, NULL))
							continue;

						if (bCompat)
						{
							strTemp.Format(_afxShellNewFmt, (LPCTSTR)strFilterExt);
							(void)IUISetRegKey(strTemp, _afxShellNewValue, _afxShellNewValueName);
						}
					}
				}
			}
		}
	}
};

//////////////////////////////////////////////////////////////////////////
// CUIShopApp

BEGIN_MESSAGE_MAP(CUIShopApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CUIShopApp::OnAppAbout)
	// Standard file based document commands
//	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// CUIShopApp construction

CUIShopApp::CUIShopApp()
{
	m_bHiColorIcons = TRUE;

	m_gdiplusToken = NULL;
	m_atom = 0;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CUIShopApp object

CUIShopApp theApp;
CMainFrame *g_pFrm = NULL;


// CUIShopApp initialization

extern BOOL g_bCheckChangeList;	// Define within UIMgr.cpp

BOOL CUIShopApp::InitInstance()
{
	AfxInitRichEdit2();
	AfxInitSkinMenu(&m_atom);

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	g_bCheckChangeList = FALSE;

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("iUIShop"));
	LoadStdProfileSettings2(4);  // Load standard INI file options (including MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_UIShopTYPE,
		RUNTIME_CLASS(CUIShopDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CUIShopView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate2(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CUIShopApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class

	// Release GDI+
	if (m_gdiplusToken)
		GdiplusShutdown(m_gdiplusToken);
	m_gdiplusToken = NULL;

	AfxUninitSkinMenu(m_atom);

	return CWinAppEx::ExitInstance();

	delete m_pRecentFileList;
	m_pRecentFileList = NULL;
}

// for replace the CDocManager
void CUIShopApp::AddDocTemplate2(CDocTemplate* pTemplate)
{
	if (m_pDocManager == NULL)
	{
		m_pDocManager = new CPromptDocManager2;
		CString strFilterExt;
		strFilterExt.LoadString(IDR_PROJECT_FILTEREXT);
		CString strFilterName;
		strFilterName.LoadString(IDR_PROJECT_FILTERNAME);
		CPromptDocManager2 *pDocManager = (CPromptDocManager2 *)m_pDocManager;
		pDocManager->SetProjectFilterExt(strFilterExt);
		pDocManager->SetProjectFilterName(strFilterName);
	}
	m_pDocManager->AddDocTemplate(pTemplate);
}

// for show full path of recent file list
void CUIShopApp::LoadStdProfileSettings2(UINT nMaxMRU/* = _AFX_MRU_COUNT*/)
{
	ASSERT_VALID(this);
	ASSERT(m_pRecentFileList == NULL);

	if (nMaxMRU != 0)
	{
		// create file MRU since nMaxMRU not zero
		m_pRecentFileList = new CFullPathRecentFileList(0, _afxFileSection, _afxFileEntry, nMaxMRU);
		m_pRecentFileList->ReadList();
	}
	// 0 by default means not set
	m_nNumPreviewPages = GetProfileInt(_afxPreviewSection, _afxPreviewEntry, 0);
}

CRecentFileList *CUIShopApp::GetRecentFileList()
{
	return m_pRecentFileList;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	// Get build value(from perforce change list)
	int nVer = CUIMgr::GetLibUIDKChangeList();
	CString strVer;
	strVer.Format(IDS_VERSION, nVer);

	CWnd *pWnd = GetDlgItem(IDC_STA_VERSION);
	ASSERT(pWnd != NULL);
	pWnd->SetWindowText(strVer);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CUIShopApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CUIShopApp customization load/save methods

void CUIShopApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;

	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);

	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_RESOURCE);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_STYLE);
}

void CUIShopApp::LoadCustomState()
{
}

void CUIShopApp::SaveCustomState()
{
}

// CUIShopApp message handlers

CDocument* CUIShopApp::OpenDocumentFile(LPCTSTR lpszImageName)
{
	// TODO: Add your specialized code here and/or call the base class

	CDocument *pDoc = CWinAppEx::OpenDocumentFile(lpszImageName);
	if (pDoc != NULL)
	{
		pDoc->SetPathName(CUIMgr::GetUIPath());	// while open *.ui, if convert to *.xui, need reset the path name of the document.
	}

	return pDoc;
}
