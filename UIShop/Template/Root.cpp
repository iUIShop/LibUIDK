// $$root$$.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "$$root$$.h"
$$IF(IM)
#include "LoginDlg.h"
$$ENDIF // IM
$$IF(MDI)
#include "MainFrm.h"
#include "$$root$$Doc.h"
#include "ChildFrm.h"
#include "$$root$$View.h"
$$ENDIF
$$IF(DLG)
#include "MainWnd.h"
$$ENDIF


#ifdef _DEBUG
#define new DEBUG_NEW
$$IF(PROJECT_VC60)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
$$ENDIF
#endif

/////////////////////////////////////////////////////////////////////////////
// $$APP_CLASS$$

$$IF(!MDI)
BEGIN_MESSAGE_MAP($$APP_CLASS$$, CWinApp)
$$ENDIF
$$IF(MDI)
BEGIN_MESSAGE_MAP($$APP_CLASS$$, CIUIWinApp)
$$ENDIF
$$IF(PROJECT_VC60)
	//{{AFX_MSG_MAP($$APP_CLASS$$)
$$IF(VERBOSE)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
$$ENDIF
	//}}AFX_MSG_MAP
$$ENDIF //PROJECT_VC60
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// $$APP_CLASS$$ construction

$$APP_CLASS$$::$$APP_CLASS$$()
{
$$IF(VERBOSE)
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
$$ENDIF
$$IF(MDI)
	m_pFrame = NULL;
$$ENDIF
	m_bCallLibUIDKMsgMap = TRUE;
	m_pMainFrame = NULL;
}

$$IF(ATL_SUPPORT)
// ATL Module object
CComModule _Module;

$$ENDIF
/////////////////////////////////////////////////////////////////////////////
// The one and only $$APP_CLASS$$ object

$$APP_CLASS$$ theApp;

//////////////////////////////////////////////////////////////////////////
// public

BOOL $$APP_CLASS$$::IsCallLibUIDKMsgMap()
{
	return m_bCallLibUIDKMsgMap;
}

/////////////////////////////////////////////////////////////////////////////
// $$APP_CLASS$$ initialization

BOOL $$APP_CLASS$$::InitInstance()
{
$$IF(SOCKETSEX)
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

$$ENDIF //SOCKETSEX
$$IF(OLECTLEX)
	AfxEnableControlContainer();

$$ENDIF //OLECTLEX
	// Standard initialization
$$IF(VERBOSE)
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
$$ENDIF

	// Change the registry key under which our settings are stored.
$$IF(VERBOSE)
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
$$ENDIF
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

$$IF(MDI)
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

$$ENDIF
$$IF(VERBOSE)
	// Change the path under which your skin files are stored.
$$ENDIF
	int nRet = CUIMgr::SetUIPath(_T("Skins\\$$root$$\\the.xui"));
	if (nRet != 0)
	{
		AfxMessageBox(_T("Failed to set the ui path or the ui path isn't exist!"));
		return FALSE;
	}

$$IF(IM)
	// To create login dialog
	CLoginDlg dlg;
	INT_PTR nRet = dlg.DoModal();
	if (nRet != IDC_BTN_LOGIN)
		return FALSE;

$$ENDIF // IM
$$IF(MDI)
	AfxInitSkinMenu();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CIUIMultiDocTemplate(
		IDR_$$ROOT$$TYPE,
		RUNTIME_CLASS(C$$root$$Doc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(C$$root$$View));
	AddDocTemplate(pDocTemplate);

$$ENDIF
$$IF(VERBOSE)
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.
$$ENDIF
$$IF(MDI)
	m_pFrame = new CMainFrame;
	m_pMainWnd = m_pFrame;

	// create and load the frame with its resources

	BOOL bRet = m_pFrame->LoadFrame(IDR_MAINFRAME,
		WS_POPUP|WS_CLIPCHILDREN|WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX, NULL);

	if (!bRet)
	{
		DWORD dwLastError = m_pFrame->GetLastCreateError();
		switch (dwLastError)
		{
		case E_LOADBMPB: // 1 failed to load the base image
		case E_LOADBMPN: // 2 failed to load the background image of normal status(end by _N.bmp)
		case E_LOADBMPH: // 3 failed to laod the background image of highlight status(end by _H.bmp)
		case E_LOADBMPD: // 4 failed to load the background image of disable status(end by _D.bmp)
		case E_LOADBMPS: // 5 failed to load the background image of selected status(end by _S.bmp)
		case E_LOADBMPC: // 6 failed to load the background image of checked status(end by _C.bmp)
			TRACE(_T("Failed to load background image"));
			break;
		case E_ATTACHBMPB: // 50 failed to attach the HBITMAP of base image to CBitmap object
		case E_ATTACHBMPN: // 51 failed to attach the HBITMAP of normal image to CBitmap object
		case E_ATTACHBMPH: // 52 failed to attach the HBITMAP of highlight image to CBitmap object
		case E_ATTACHBMPD: // 53 failed to attach the HBITMAP of disable image to CBitmap object
		case E_ATTACHBMPS: // 54 failed to attach the HBITMAP of selected image to CBitmap object
		case E_ATTACHBMPC: // 55 failed to attach the HBITMAP of checked image to CBitmap object
		case E_ADJUSTCHILD:// 56 failed to adjust the child controls while change the skin
			TRACE(_T("Failed to attach bitmap to CBitmao object"));
			break;
		case E_CREATECHILD:// 110 failed to create the child window
			TRACE(_T("Failed to create child windows"));
			break;
		default:
			break;
		}
		return FALSE;
	}

	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(FALSE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pFrame->ShowWindow(SW_SHOW);
	m_pFrame->Update();
$$ENDIF
$$IF(!MDI)
	m_pMainFrame = new CMainWnd;
	m_pMainFrame->LoadFrame(IDR_MAINFRAME, WS_VISIBLE | WS_POPUP, NULL);
	if (m_pMainFrame->GetSafeHwnd() == NULL)
	{
		// Search the error code in LibUIDK.h, the first error code is E_LOADBMPB.
		DWORD dwLastError = m_pMainFrame->GetLastCreateError();
		return FALSE;
	}

	m_pMainFrame->CenterWindow();

	// The one and only window has been initialized, so show and update it.
	m_pMainFrame->ShowWindow(SW_SHOW);
	m_pMainFrame->UpdateWindow();

	m_pMainWnd = m_pMainFrame;
$$ENDIF

	return TRUE;
}

int $$APP_CLASS$$::ExitInstance()
{
$$IF(MDI)
	if (m_pFrame != NULL)
	{
		delete m_pFrame;
		m_pFrame = NULL;
	}
$$ENDIF
$$IF(!MDI)
	if (m_pMainFrame != NULL)
	{
		SafeDelete(m_pMainFrame);
	}
$$ENDIF

	CUIMgr::ReleaseSkin();

$$IF(VERBOSE)
	// TODO: Add your specialized code here and/or call the base class
$$ENDIF

$$IF(MDI)
	return CIUIWinApp::ExitInstance();
$$ENDIF
$$IF(!MDI)
	return CWinApp::ExitInstance();
$$ENDIF
}

/////////////////////////////////////////////////////////////////////////////
// $$APP_CLASS$$ message handlers

