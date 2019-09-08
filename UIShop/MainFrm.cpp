// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "UIShop.h"
#include "MainFrm.h"
#include "NewProjectDlg.h"
#include "ImageSymbolsDlg.h"
#include "TransformDialogDlg.h"
#include "GenerateResource.h"
#include "GenerateResourceDlg.h"
#include "GenClassInfoFileDlg.h"
#include "SyncSkinTabOrderDlg.h"
#include "RenameTemplateFileDlg.h"
#include <HtmlHelp.h>
#pragma comment (lib, "HtmlHelp.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define ID_VIEW_CUSTOMIZE               126
#define ID_VIEW_FILEVIEW                133
#define ID_VIEW_PROPERTIESWND           150
#define ID_VIEW_CONTACTWND              160
#define ID_VIEW_STORYBOARDWND			161

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_COMMAND(ID_HELP_UISHOP, &CMainFrame::OnHelpUIShop)
	ON_COMMAND(ID_HELP_LIBUIDK, &CMainFrame::OnHelpLibUIDK)
	ON_COMMAND(ID_HELP_VIDEO, &CMainFrame::OnHelpVideo)
	ON_COMMAND(ID_HELP_WEB_LINK, &CMainFrame::OnHelpWebLink)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString)
	ON_MESSAGE(WM_SETTEXT, OnSetWindowText)
	ON_COMMAND(ID_CONTROL_SELECT, OnControlSelect)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_SELECT, OnUpdateControlSelect)
	ON_COMMAND(ID_CONTROL_BUTTON, OnControlButton)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_BUTTON, OnUpdateControlButton)
	ON_COMMAND(ID_CONTROL_CHECKBOX, OnControlCheckBox)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_CHECKBOX, OnUpdateControlCheckBox)
	ON_COMMAND(ID_CONTROL_RADIOBUTTON, OnControlRadioButton)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_RADIOBUTTON, OnUpdateControlRadioButton)
	ON_COMMAND(ID_CONTROL_STATIC, OnControlStatic)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_STATIC, OnUpdateControlStatic)
	ON_COMMAND(ID_CONTROL_EDIT, OnControlEdit)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_EDIT, OnUpdateControlEdit)
	ON_COMMAND(ID_CONTROL_COMBOBOX, OnControlComboBox)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_COMBOBOX, OnUpdateControlComboBox)
	ON_COMMAND(ID_CONTROL_SLIDER, OnControlSlider)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_SLIDER, OnUpdateControlSlider)
	ON_COMMAND(ID_CONTROL_SPIN, OnControlSpin)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_SPIN, OnUpdateControlSpin)
	ON_COMMAND(ID_CONTROL_PROGRESS, OnControlProgress)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_PROGRESS, OnUpdateControlProgress)
	ON_COMMAND(ID_CONTROL_HOTKEY, OnControlHotKey)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_HOTKEY, OnUpdateControlHotKey)
	ON_COMMAND(ID_CONTROL_IPADDRESS, OnControlIPAddress)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_IPADDRESS, OnUpdateControlIPAddress)
	ON_COMMAND(ID_CONTROL_SLIDEREX, OnControlSliderEx)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_SLIDEREX, OnUpdateControlSliderEx)
	ON_COMMAND(ID_CONTROL_LISTCTRL, OnControlListCtrl)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_LISTCTRL, OnUpdateControlListCtrl)
	ON_COMMAND(ID_CONTROL_TREECTRL, OnControlTreeCtrl)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_TREECTRL, OnUpdateControlTreeCtrl)
	ON_COMMAND(ID_CONTROL_SCROLLBAREX, OnControlScrollBarEx)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_SCROLLBAREX, OnUpdateControlScrollBarEx)
	ON_COMMAND(ID_CONTROL_THUMBNAIL, OnControlThumbnail)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_THUMBNAIL, OnUpdateControlThumbnail)
	ON_COMMAND(ID_CONTROL_TREELIST, OnControlTreeList)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_TREELIST, OnUpdateControlTreeList)
	ON_COMMAND(ID_CONTROL_RICHEDIT, OnControlRichEdit)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_RICHEDIT, OnUpdateControlRichEdit)
	ON_COMMAND(ID_CONTROL_RICHEDIT_IM, OnControlRichEditIM)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_RICHEDIT_IM, OnUpdateControlRichEditIM)
	ON_COMMAND(ID_CONTROL_HTMLCTRL, OnControlHtmlCtrl)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_HTMLCTRL, OnUpdateControlHtmlCtrl)
	ON_COMMAND(ID_CONTROL_DATETIMECTRL, OnControlDateTimeCtrl)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_DATETIMECTRL, OnUpdateControlDateTimeCtrl)
	ON_COMMAND(ID_CONTROL_WAVECTRL, OnControlWaveCtrl)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_WAVECTRL, OnUpdateControlWaveCtrl)
	ON_COMMAND(ID_CONTROL_PICCTRL, OnControlPicCtrl)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_PICCTRL, OnUpdateControlPicCtrl)
	ON_COMMAND(ID_CONTROL_MENUBAR, OnControlMenuBar)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_MENUBAR, OnUpdateControlMenuBar)
	ON_COMMAND(ID_CONTROL_SPLITTER, OnControlSplitter)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_SPLITTER, OnUpdateControlSplitter)
	ON_COMMAND(ID_CONTROL_RECT, OnControlRect)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_RECT, OnUpdateControlRect)
	ON_COMMAND(ID_CONTROL_SPLITTERWL, OnControlSplitterWL)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_SPLITTERWL, OnUpdateControlSplitterWL)
	ON_COMMAND(ID_CONTROL_LINE, OnControlLine)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_LINE, OnUpdateControlLine)
	ON_COMMAND(ID_CONTROL_TEXT, OnControlText)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_TEXT, OnUpdateControlText)
	ON_COMMAND(ID_CONTROL_BUTTONWL, OnControlButtonWL)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_BUTTONWL, OnUpdateControlButtonWL)
	ON_COMMAND(ID_CONTROL_CHECKBOXWL, OnControlCheckBoxWL)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_CHECKBOXWL, OnUpdateControlCheckBoxWL)
	ON_COMMAND(ID_CONTROL_RADIOWL, OnControlRadioWL)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_RADIOWL, OnUpdateControlRadioWL)
	ON_COMMAND(ID_CONTROL_SLIDERWL, OnControlSliderWL)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_SLIDERWL, OnUpdateControlSliderWL)
	ON_COMMAND(ID_CONTROL_PIC, OnControlPic)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_PIC, OnUpdateControlPic)
	ON_COMMAND(ID_CONTROL_RICHEDITWL, OnControlRichEditWL)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_RICHEDITWL, OnUpdateControlRichEditWL)
	ON_COMMAND(ID_CONTROL_IMWL, OnControlIMWL)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_IMWL, OnUpdateControlIMWL)
	ON_COMMAND(ID_CONTROL_TASK_WND_MGR, OnControlTaskWndMgr)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_TASK_WND_MGR, OnUpdateControlTaskWndMgr)
	ON_COMMAND(ID_CONTROL_DOCK_PANEL, OnControlDockPnl)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_DOCK_PANEL, OnUpdateControlDockPnl)
	ON_COMMAND(ID_CONTROL_UNIFORM_GRID_PANEL, OnControlUniformGridPnl)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_UNIFORM_GRID_PANEL, OnUpdateControlUniformGridPnl)
	ON_COMMAND(ID_CONTROL_STACK_PANEL, OnControlStackPnl)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_STACK_PANEL, OnUpdateControlStackPnl)
//	ON_COMMAND(ID_VIEW_CONTROLBAR, OnViewControlBar)
//	ON_UPDATE_COMMAND_UI(ID_VIEW_CONTROLBAR, OnUpdateViewControlBar)
	ON_COMMAND(ID_FILE_NEW_LIBUIDK_PROJECT, &CMainFrame::OnFileNewLibuidkProject)
	ON_COMMAND(ID_DESIGN_SIZE_TO_CONTENT, &CMainFrame::OnDesignSizeToContent)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_SIZE_TO_CONTENT, &CMainFrame::OnUpdateDesignSizeToContent)
	ON_COMMAND(ID_DESIGN_SIZE_TO_FOREGROUND_CONTENT, &CMainFrame::OnDesignSizeToForegroundContent)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_SIZE_TO_FOREGROUND_CONTENT, &CMainFrame::OnUpdateDesignSizeToForegroundContent)
	ON_COMMAND(ID_DESIGN_FORMAT_BRUSH, &CMainFrame::OnDesignFormatBrush)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_FORMAT_BRUSH, &CMainFrame::OnUpdateDesignFormatBrush)
	ON_COMMAND(ID_DESIGN_BACKGROUND_SHOW, &CMainFrame::OnDesignBackgroundShow)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_BACKGROUND_SHOW, &CMainFrame::OnUpdateDesignBackgroundShow)
	ON_COMMAND(ID_EDIT_UNDO, &CMainFrame::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CMainFrame::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &OnUpdateEditRedo)
#ifdef _DEBUG
	ON_COMMAND(ID_EDIT_CUT, &CMainFrame::OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CMainFrame::OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CMainFrame::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CMainFrame::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CMainFrame::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CMainFrame::OnUpdateEditPaste)
#endif
	ON_COMMAND(ID_EDIT_UNUSED_IMAGE_SYMBOLS, &CMainFrame::OnEditUnusedImageSymbols)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNUSED_IMAGE_SYMBOLS, &CMainFrame::OnUpdateEditUnusedImageSymbols)
	ON_COMMAND(ID_EDIT_UNFOUND_IMAGE_SYMBOLS, &CMainFrame::OnEditUnfoundImageSymbols)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNFOUND_IMAGE_SYMBOLS, &CMainFrame::OnUpdateEditUnfoundImageSymbols)
	ON_COMMAND(ID_TOOLS_CREATE_LANGUAGE_FILE, &CMainFrame::OnToolsCreateLanguageFile)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CREATE_LANGUAGE_FILE, &CMainFrame::OnUpdateToolsCreateLanguageFile)
	ON_COMMAND(ID_TOOLS_CREATE_RESOURCE_FILE, &CMainFrame::OnToolsCreateResourceFile)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CREATE_RESOURCE_FILE, &CMainFrame::OnUpdateToolsCreateResourceFile)
	ON_COMMAND(ID_TOOLS_TRANSFORM_DIALOG, &CMainFrame::OnToolsTransformDialog)
	ON_COMMAND(ID_TOOLS_CREATE_CLASS_INFO_FILE, &CMainFrame::OnToolsCreateClassInfoFile)
	ON_COMMAND(ID_TOOLS_GENERATE_RESOURCE_H, &CMainFrame::OnToolsGenerateResourceH)
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_TOOLS_SYNC_SKIN_TAB_ORDER, &CMainFrame::OnToolsSyncSkinTabOrder)
	ON_COMMAND(ID_TOOLS_RENAME_TEMPLATE_FILE, &CMainFrame::OnToolsRenameTemplateFile)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_POS,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_eCurControl = CT_ARROW;
	m_pViewUIShop = NULL;

	m_nCtrlPropClipboardFormat = 0;

	m_bFormatBrush = FALSE;
	m_pFormatBrushProp = NULL;
}

CMainFrame::~CMainFrame()
{
}


/////////////////////////////////////////////////////////////////////
// public

int CMainFrame::CustomizeProject(CNewProjectDlg *pDlg)
{
	m_ProjectGen.SetInputFolder(pDlg->m_strLocation);
	m_ProjectGen.SetProjectName(pDlg->m_strPrjName);

	m_ProjectGen.m_Dictionary.RemoveAll();
	m_ProjectGen.m_Dictionary[_T("root")] = pDlg->m_strPrjName;
	m_ProjectGen.m_Dictionary[_T("Root")] = pDlg->m_strPrjName;
	CString strUpperPrjName = pDlg->m_strPrjName;
	strUpperPrjName.MakeUpper();
	m_ProjectGen.m_Dictionary[_T("ROOT")] = strUpperPrjName;

	// Must define
	m_ProjectGen.m_Dictionary[_T("INSERT_LOCATION_COMMENT")] = _T("Microsoft Visual C++ will insert additional declarations immediately before the previous line.");

	// APP_CLASS
	CString str = pDlg->m_strPrjName;
	CString strName = str.Left(1);
	str.Delete(0, 1);
	strName.MakeUpper();
	strName += str;
	CString strAppClass = _T("C");
	strAppClass += strName;
	strAppClass += _T("App");
	m_ProjectGen.m_Dictionary[_T("APP_CLASS")] = strAppClass;

	// Project type
	switch (pDlg->m_uVSEdition)
	{
	case IDC_RAD_VC60:
		m_ProjectGen.m_Dictionary[_T("PROJECT_VC60")] = _T("1");
		break;
	case IDC_RAD_VC71:
		m_ProjectGen.m_Dictionary[_T("PROJECT_VC71")] = _T("1");
		break;
	case IDC_RAD_VC80:
		m_ProjectGen.m_Dictionary[_T("PROJECT_VC80")] = _T("1");
		break;
	case IDC_RAD_VC90:
		m_ProjectGen.m_Dictionary[_T("PROJECT_VC90")] = _T("1");
		break;
	case IDC_RAD_VC100:
		m_ProjectGen.m_Dictionary[_T("PROJECT_VC100")] = _T("1");
		break;
	case IDC_RAD_VC110:
		m_ProjectGen.m_Dictionary[_T("PROJECT_VC110")] = _T("1");
		break;
	case IDC_RAD_VC120:
		m_ProjectGen.m_Dictionary[_T("PROJECT_VC120")] = _T("1");
		break;
	case IDC_RAD_VC140:
		m_ProjectGen.m_Dictionary[_T("PROJECT_VC140")] = _T("1");
		break;
	default:
		break;
	}

	// MDI, the key not same as it defined in MFC.
	switch (pDlg->m_uProType)
	{
	case IDC_RAD_PROJECT_GENERAL:
		m_ProjectGen.m_Dictionary[_T("DLG")] = _T("1");
		break;
	case IDC_RAD_PROJECT_MDI:
		m_ProjectGen.m_Dictionary[_T("MDI")] = _T("1");
		break;
	case IDC_RAD_PROJECT_IM:
		m_ProjectGen.m_Dictionary[_T("IM")] = _T("1");
		break;
	case IDC_RAD_PROJECT_WEISHI:
		m_ProjectGen.m_Dictionary[_T("WEISHI")] = _T("1");
		break;
	default:
		break;
	}

	// Database
	switch (pDlg->m_uDBID)
	{
	case IDC_RAD_NONE:
		m_ProjectGen.m_Dictionary[_T("DBEX")] = _T("0");
		break;
	case IDC_RAD_HEADER:
		m_ProjectGen.m_Dictionary[_T("DBEX")] = _T("1");
		break;
	default:
		break;
	}

	// ActiveX
	m_ProjectGen.m_Dictionary[_T("OLECTLEX")] = pDlg->m_bActiveX ? _T("1") : _T("0");

	// Sockets
	m_ProjectGen.m_Dictionary[_T("SOCKETSEX")] = pDlg->m_bSockets ? _T("1") : _T("0");

	// Comment
	switch (pDlg->m_uCommentID)
	{
	case IDC_RAD_YES:
		m_ProjectGen.m_Dictionary[_T("VERBOSE")] = _T("1");
		break;
	case IDC_RAD_NO:
		m_ProjectGen.m_Dictionary[_T("VERBOSE")] = _T("0");
		break;
	default:
		break;
	}

	return 0;
}

void CMainFrame::SetControlPosText(CString str)
{
	int iIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_POS);
	m_wndStatusBar.SetPaneText(iIndex, str);

	CWindowDC dc(&m_wndStatusBar);
	CSize st = dc.GetTextExtent(str);
	m_wndStatusBar.SetPaneInfo(iIndex, ID_INDICATOR_POS, SBPS_NORMAL, st.cx);
}

int CMainFrame::ShowTBControlBar(BOOL bShow)
{
	//	ShowControlBar(&m_wndControlBar, bShow, FALSE);
	return 0;
}

CONTROL_TYPE CMainFrame::GetCurSelControlType()
{
	return m_eCurControl;
}

void CMainFrame::SetCurSelControlType(CONTROL_TYPE eControlType)
{
	m_eCurControl = eControlType;
}


/////////////////////////////////////////////////////////////////////
// message
extern CMainFrame *g_pFrm;

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	BOOL bNameValid;

	// set the visual manager used to draw all user interface elements
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME_256))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	if (!m_wndControlBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndControlBar.LoadToolBar(IDR_CONTROLBAR))
	{
		TRACE0("Failed to create controlbar\n");
		return -1;      // fail to create
	}

	if (!m_wndLayoutBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndLayoutBar.LoadToolBar(IDR_LAYOUT))
	{
		TRACE0("Failed to create layoutbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndControlBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndLayoutBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	DockPane(&m_wndControlBar);
	DockPane(&m_wndLayoutBar);


	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);
	m_wndContact.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndContact);
	m_wndStoryboard.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndStoryboard);

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	g_pFrm = this;
	m_nCtrlPropClipboardFormat = RegisterClipboardFormat(_T("CF_CTRLPROPERTIES"));

	return 0;
}

void CMainFrame::OnDestroy()
{
	CMDIFrameWndEx::OnDestroy();

	// TODO: Add your message handler code here
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT &cs)
{
	if (!CMDIFrameWndEx::PreCreateWindow(cs))
	{
		return FALSE;
	}
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

BOOL CMainFrame::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	HICON hFileViewIcon = m_wndFileView.GetIcon(FALSE);
	DestroyIcon(hFileViewIcon);
	HICON hPropertiesBarIcon = m_wndProperties.GetIcon(FALSE);
	DestroyIcon(hPropertiesBarIcon);

	return CMDIFrameWndEx::DestroyWindow();
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// Create file view
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create File View window\n");
		return FALSE; // failed to create
	}

	// Create properties window
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}

	// Create contact window
	CString strContactWnd;
	bNameValid = strContactWnd.LoadString(IDS_CONTACT_WND);
	ASSERT(bNameValid);
	if (!m_wndContact.Create(strContactWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CONTACTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Contact window\n");
		return FALSE; // failed to create
	}

	// Create storyboard panel
	CString strStoryboardWnd;
	bNameValid = strStoryboardWnd.LoadString(IDS_STORYBOARD_WND);
	ASSERT(bNameValid);
	if (!m_wndStoryboard.Create(strStoryboardWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_STORYBOARDWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Storyboaed window\n");
		return FALSE; // failed to create
	}


	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext &dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnFileNewLibuidkProject()
{
	CNewProjectDlg dlg;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	CustomizeProject(&dlg);

	void *pData = NULL;
	DWORD dwSize = -1;
	int nRet = m_ProjectGen.LoadCustomResource("NewProj.inf", &pData, &dwSize);
	if (nRet != 0)
	{
		AfxMessageBox(_T("Can't load \"NewProj.inf\" file!"));
		return;
	}

	nRet = m_ProjectGen.LoadTemplate(pData, dwSize);
	if (nRet != 0)
	{
		AfxMessageBox(_T("Can't parse \"NewProj.inf\" file!"));
		return;
	}

	nRet = m_ProjectGen.ProcessTemplate();
	if (nRet != 0)
	{
		AfxMessageBox(_T("Failed to new prject!"));
		return;
	}

	// If create successfully, open it.
	CString strXUIPath = dlg.m_strLocation;
	if (strXUIPath.Right(1) != _T("\\") && strXUIPath.Right(1) != _T("/"))
	{
		strXUIPath += _T("\\");
	}

	strXUIPath += dlg.m_strPrjName + _T("\\Bin\\Skins\\");
	strXUIPath += dlg.m_strPrjName + _T("\\the.xui");

	theApp.OpenDocumentFile(strXUIPath);
	// Update changelist number of the.xui by save skin.
	SendMessage(WM_COMMAND, MAKEWPARAM(ID_FILE_SAVE, 0), 0);
}

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog *pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->Create();
}

void CMainFrame::OnHelpUIShop()
{
	TCHAR szFilePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	TCHAR szDisc[MAX_PATH] = {0};
	TCHAR szPath[MAX_PATH] = {0};
	TSPLITPATH(szFilePath, szDisc, MAX_PATH, szPath, MAX_PATH, NULL, 0, NULL, 0);
	TSTRCAT(szDisc, MAX_PATH, szPath);
	CString strPath = szDisc;
	if (strPath.Right(1) == "/" || strPath.Right(1) == "\\")
	{
		strPath += "UIShop_CHS.chm";
	}
	else
	{
		strPath += "\\UIShop_CHS.chm";
	}

	::HtmlHelp(m_hWnd, strPath, HH_DISPLAY_TOPIC, 0);
}

void CMainFrame::OnHelpLibUIDK()
{
	TCHAR szFilePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	TCHAR szDisc[MAX_PATH] = {0};
	TCHAR szPath[MAX_PATH] = {0};
	TSPLITPATH(szFilePath, szDisc, MAX_PATH, szPath, MAX_PATH, NULL, 0, NULL, 0);
	TSTRCAT(szDisc, MAX_PATH, szPath);
	CString strPath = szDisc;
	if (strPath.Right(1) == "/" || strPath.Right(1) == "\\")
	{
		strPath += "LibUIDK_CHS.chm";
	}
	else
	{
		strPath += "\\LibUIDK_CHS.chm";
	}

	::HtmlHelp(m_hWnd, strPath, HH_DISPLAY_TOPIC, 0);
}

void CMainFrame::OnHelpVideo()
{
	TCHAR szFilePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	TCHAR szDisc[MAX_PATH] = {0};
	TCHAR szPath[MAX_PATH] = {0};
	TSPLITPATH(szFilePath, szDisc, MAX_PATH, szPath, MAX_PATH, NULL, 0, NULL, 0);
	TSTRCAT(szDisc, MAX_PATH, szPath);
	CString strPath = szDisc;
	if (strPath.Right(1) == "/" || strPath.Right(1) == "\\")
	{
		strPath += "UIShop_Help.exe";
	}
	else
	{
		strPath += "\\UIShop_Help.exe";
	}

	ShellExecute(NULL, _T("Open"), strPath, NULL, NULL, SW_SHOW);
}

void CMainFrame::OnHelpWebLink()
{
	ShellExecute(NULL, _T("Open"), _T("http://www.iuishop.com"), NULL, NULL, SW_SHOW);
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp, LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp, lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar *pUserToolbar = (CMFCToolBar *)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

CUIShopDoc *CMainFrame::GetUIShopDoc()
{
	CUIShopView *pView = GetUIShopView();
	if (pView == NULL)
	{
		return NULL;
	}
	return pView->GetDocument();
}

CFileView *CMainFrame::GetFileView()
{
	return &m_wndFileView;
}

CPropertiesWnd *CMainFrame::GetPropertiesWnd()
{
	return &m_wndProperties;
}

CStoryboardPane *CMainFrame::GetStoryboardPanel()
{
	return &m_wndStoryboard;
}

int CMainFrame::GetFileMRUMessage(UINT nID, CString *pstrMsg)
{
	if (pstrMsg == NULL)
	{
		return -1;
	}

	CUIShopApp *pApp = (CUIShopApp *)AfxGetApp();
	CRecentFileList *pRecentFileList = pApp->GetRecentFileList();
	ASSERT(pRecentFileList != NULL);

	ASSERT(nID >= ID_FILE_MRU_FILE1);
	ASSERT(nID < ID_FILE_MRU_FILE1 + (UINT)pRecentFileList->GetSize());
	int nIndex = nID - ID_FILE_MRU_FILE1;

	*pstrMsg = _T("Open project:");
	*pstrMsg += (*pRecentFileList)[nIndex];
	return 0;
}

BOOL CMainFrame::IsFormatBrushSelected()
{
	return m_bFormatBrush;
}

const CTRLPROPERTIES *CMainFrame::GetFormatBrushProp() const
{
	return m_pFormatBrushProp;
}

int CMainFrame::SetFormatBrushProp(const CTRLPROPERTIES *pProp)
{
	m_pFormatBrushProp = pProp;

	m_vBrushPropIndex.clear();

	return 0;
}

int CMainFrame::AddFormatBrushPropIndex(int nIndex)
{
	m_vBrushPropIndex.push_back(nIndex);

	return 0;
}

const std::vector<int> *CMainFrame::GetFormatBrushPropIndex() const
{
	return &m_vBrushPropIndex;
}

STORY_FRAME *CMainFrame::InsertFrame()
{
	int nFrame = m_wndStoryboard.GetCurFrame();
	if (nFrame < 0)
	{
		return NULL;
	}

	CUIDesignWnd *pCurUIDesignWnd = m_pViewUIShop->GetCurUIDesignWnd();
	if (pCurUIDesignWnd == NULL)
	{
		return NULL;
	}

	UIWNDPROPERTIES *pCurUIWnd = m_pViewUIShop->GetCurUIDesignWnd()->GetUIWndProperties();
	if (pCurUIWnd == NULL)
	{
		return NULL;
	}

	// If the story item not exit, create a new item.
	STORY_FRAME *pItem = pCurUIWnd->FindStoryFrame(nFrame);
	if (pItem == NULL)
	{
		pItem = new STORY_FRAME;
		pItem->nFrameIndex = nFrame;

		// Insert control
		int nControlCount = (int)pCurUIWnd->m_vControls.size();
		for (int i = 0; i < nControlCount; ++i)
		{
			CTRLPROPERTIES *pBaseCtrlProp = pCurUIWnd->m_vControls[i];

			CTRLPROPERTIES *pCurFrameCtrlProp =
				pCurUIWnd->AllocCtrlPropertiesAtStoryFrame(nFrame, pBaseCtrlProp->m_strID);

			pItem->m_vStoryFrameCtrlItem.push_back(pCurFrameCtrlProp);
		}

		pCurUIWnd->InsertStoryFrame(pItem);
	}

	// Update frame flag
	g_pFrm->GetStoryboardPanel()->SetBindUIWnd(pCurUIDesignWnd);

	GetUIShopDoc()->SetModifiedFlag(TRUE);

	return pItem;
}

LRESULT CMainFrame::OnSetMessageString(WPARAM wParam, LPARAM lParam)
{
	LRESULT hr =  CMDIFrameWndEx::OnSetMessageString(wParam, lParam);

	CWnd *pMessageBar = GetMessageBar();
	if (pMessageBar != NULL && wParam != 0)
	{
		// map SC_CLOSE to PREVIEW_CLOSE when in print preview mode
		if (wParam == AFX_IDS_SCCLOSE && m_lpfnCloseProc != NULL)
		{
			wParam = AFX_IDS_PREVIEW_CLOSE;
		}

		// get message associated with the ID indicated by wParam
		if (wParam >= ID_FILE_MRU_FIRST && wParam <= ID_FILE_MRU_LAST)
		{
			CString strMessage;
			GetFileMRUMessage((UINT)wParam, &strMessage);
			pMessageBar->SetWindowText(strMessage);
		}
	}

	return hr;
}

LRESULT CMainFrame::OnSetWindowText(WPARAM wParam, LPARAM lParam)
{
	CString strText((TCHAR *)lParam);

	if (m_pViewUIShop != NULL)
	{
		CDocument *pDocument = m_pViewUIShop->GetDocument();
		ASSERT(pDocument != NULL);
		CString strPathName = pDocument->GetPathName();
		if (!strPathName.IsEmpty())
		{
			strText += _T(" - ");
			strText += strPathName;
		}
	}
	DefWindowProc(WM_SETTEXT, wParam, (LPARAM)strText.GetBuffer(0));

	return 0;
}

void CMainFrame::OnControlSelect()
{
	m_eCurControl = CT_ARROW;
}

void CMainFrame::OnUpdateControlSelect(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_ARROW) ? TRUE : FALSE);
}

void CMainFrame::OnControlButton()
{
	if (CT_PUSHBUTTON == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_PUSHBUTTON;
	}
}

void CMainFrame::OnUpdateControlButton(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_PUSHBUTTON) ? TRUE : FALSE);
}

void CMainFrame::OnControlCheckBox()
{
	if (CT_CHECK == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_CHECK;
	}
}

void CMainFrame::OnUpdateControlCheckBox(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_CHECK) ? TRUE : FALSE);
}

void CMainFrame::OnControlRadioButton()
{
	if (CT_RADIO == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_RADIO;
	}
}

void CMainFrame::OnUpdateControlRadioButton(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_RADIO) ? TRUE : FALSE);
}

void CMainFrame::OnControlStatic()
{
	if (CT_STATIC == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_STATIC;
	}
}

void CMainFrame::OnUpdateControlStatic(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_STATIC) ? TRUE : FALSE);
}

void CMainFrame::OnControlEdit()
{
	if (CT_EDIT == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_EDIT;
	}
}

void CMainFrame::OnUpdateControlEdit(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_EDIT) ? TRUE : FALSE);
}

void CMainFrame::OnControlComboBox()
{
	if (CT_COMBOBOX == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_COMBOBOX;
	}
}

void CMainFrame::OnUpdateControlComboBox(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_COMBOBOX) ? TRUE : FALSE);
}

void CMainFrame::OnControlSlider()
{
	if (CT_SLIDER == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_SLIDER;
	}
}

void CMainFrame::OnUpdateControlSlider(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_SLIDER) ? TRUE : FALSE);
}

void CMainFrame::OnControlSpin()
{
	if (CT_SPIN == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_SPIN;
	}
}

void CMainFrame::OnUpdateControlSpin(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_SPIN) ? TRUE : FALSE);
}

void CMainFrame::OnControlProgress()
{
	if (CT_PROGRESS == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_PROGRESS;
	}
}

void CMainFrame::OnUpdateControlProgress(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_PROGRESS) ? TRUE : FALSE);
}

void CMainFrame::OnControlHotKey()
{
	if (CT_HOTKEY == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_HOTKEY;
	}
}

void CMainFrame::OnUpdateControlHotKey(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_HOTKEY) ? TRUE : FALSE);
}

void CMainFrame::OnControlIPAddress()
{
	if (CT_IPADDRESS == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_IPADDRESS;
	}
}

void CMainFrame::OnUpdateControlIPAddress(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_IPADDRESS) ? TRUE : FALSE);
}

void CMainFrame::OnControlSliderEx()
{
	if (CT_SLIDEREX == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_SLIDEREX;
	}
}

void CMainFrame::OnUpdateControlSliderEx(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_SLIDEREX) ? TRUE : FALSE);
}

void CMainFrame::OnControlListCtrl()
{
	if (CT_LISTCTRL == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_LISTCTRL;
	}
}

void CMainFrame::OnUpdateControlListCtrl(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_LISTCTRL) ? TRUE : FALSE);
}

void CMainFrame::OnControlTreeCtrl()
{
	if (CT_TREECTRL == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_TREECTRL;
	}
}

void CMainFrame::OnUpdateControlTreeCtrl(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_TREECTRL) ? TRUE : FALSE);
}

void CMainFrame::OnControlScrollBarEx()
{
	if (CT_SCROLLBAREX == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_SCROLLBAREX;
	}
}

void CMainFrame::OnUpdateControlScrollBarEx(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_SCROLLBAREX) ? TRUE : FALSE);
}

void CMainFrame::OnControlThumbnail()
{
	if (CT_THUMBNAIL == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_THUMBNAIL;
	}
}

void CMainFrame::OnUpdateControlThumbnail(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_THUMBNAIL) ? TRUE : FALSE);
}

void CMainFrame::OnControlTreeList()
{
	if (CT_TREELIST == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_TREELIST;
	}
}

void CMainFrame::OnUpdateControlTreeList(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_TREELIST) ? TRUE : FALSE);
}

void CMainFrame::OnControlRichEdit()
{
	if (CT_RICHEDIT == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_RICHEDIT;
	}
}

void CMainFrame::OnUpdateControlRichEdit(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_RICHEDIT) ? TRUE : FALSE);
}

void CMainFrame::OnControlRichEditIM()
{
	if (CT_RICHEDIT_IM == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_RICHEDIT_IM;
	}
}

void CMainFrame::OnUpdateControlRichEditIM(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_RICHEDIT_IM) ? TRUE : FALSE);
}

void CMainFrame::OnControlHtmlCtrl()
{
	if (CT_HTMLCTRL == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_HTMLCTRL;
	}
}

void CMainFrame::OnUpdateControlHtmlCtrl(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_HTMLCTRL) ? TRUE : FALSE);
}

void CMainFrame::OnControlDateTimeCtrl()
{
	if (CT_DATETIMECTRL == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_DATETIMECTRL;
	}
}

void CMainFrame::OnUpdateControlDateTimeCtrl(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_DATETIMECTRL) ? TRUE : FALSE);
}

void CMainFrame::OnControlWaveCtrl()
{
	if (CT_WAVECTRL == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_WAVECTRL;
	}
}

void CMainFrame::OnUpdateControlWaveCtrl(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_WAVECTRL) ? TRUE : FALSE);
}

void CMainFrame::OnControlPicCtrl()
{
	if (CT_PICTURE == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_PICTURE;
	}
}

void CMainFrame::OnUpdateControlPicCtrl(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_PICTURE) ? TRUE : FALSE);
}

void CMainFrame::OnControlMenuBar()
{
	if (CT_MENUBAR == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_MENUBAR;
	}
}

void CMainFrame::OnUpdateControlMenuBar(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_MENUBAR) ? TRUE : FALSE);
}

void CMainFrame::OnControlSplitter()
{
	if (CT_SPLITTER == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_SPLITTER;
	}
}

void CMainFrame::OnUpdateControlSplitter(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_SPLITTER) ? TRUE : FALSE);
}

void CMainFrame::OnControlRect()
{
	if (CT_WL_RECTCTRL == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_WL_RECTCTRL;
	}
}

void CMainFrame::OnUpdateControlRect(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_WL_RECTCTRL) ? TRUE : FALSE);
}

void CMainFrame::OnControlSplitterWL()
{
	if (CT_WL_SPLITTER == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_WL_SPLITTER;
	}
}

void CMainFrame::OnUpdateControlSplitterWL(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_WL_SPLITTER) ? TRUE : FALSE);
}

void CMainFrame::OnControlLine()
{
	if (CT_WL_LINE == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_WL_LINE;
	}
}

void CMainFrame::OnUpdateControlLine(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_WL_LINE) ? TRUE : FALSE);
}

void CMainFrame::OnControlText()
{
	if (CT_WL_TEXT == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_WL_TEXT;
	}
}

void CMainFrame::OnUpdateControlText(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_WL_TEXT) ? TRUE : FALSE);
}

void CMainFrame::OnControlButtonWL()
{
	if (CT_WL_BUTTON == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_WL_BUTTON;
	}
}

void CMainFrame::OnUpdateControlButtonWL(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_WL_BUTTON) ? TRUE : FALSE);
}

void CMainFrame::OnControlCheckBoxWL()
{
	if (CT_WL_CHECK == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_WL_CHECK;
	}
}

void CMainFrame::OnUpdateControlCheckBoxWL(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_WL_CHECK) ? TRUE : FALSE);
}

void CMainFrame::OnControlRadioWL()
{
	if (CT_WL_RADIO == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_WL_RADIO;
	}
}

void CMainFrame::OnUpdateControlRadioWL(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_WL_RADIO) ? TRUE : FALSE);
}

void CMainFrame::OnControlSliderWL()
{
	if (CT_WL_SLIDER == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_WL_SLIDER;
	}
}

void CMainFrame::OnUpdateControlSliderWL(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_WL_SLIDER) ? TRUE : FALSE);
}

void CMainFrame::OnControlPic()
{
	if (CT_WL_PICTURE == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_WL_PICTURE;
	}
}

void CMainFrame::OnUpdateControlPic(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_WL_PICTURE) ? TRUE : FALSE);
}

void CMainFrame::OnControlRichEditWL()
{
	if (CT_WL_RICHEDIT == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_WL_RICHEDIT;
	}
}

void CMainFrame::OnUpdateControlRichEditWL(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_WL_RICHEDIT) ? TRUE : FALSE);
}

void CMainFrame::OnControlIMWL()
{
	if (CT_WL_RICHEDIT_IM == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_WL_RICHEDIT_IM;
	}
}

void CMainFrame::OnUpdateControlIMWL(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_WL_RICHEDIT_IM) ? TRUE : FALSE);
}

void CMainFrame::OnControlTaskWndMgr()
{
	if (CT_TASK_WND_MGR == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_TASK_WND_MGR;
	}
}

void CMainFrame::OnUpdateControlTaskWndMgr(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_TASK_WND_MGR) ? TRUE : FALSE);
}

void CMainFrame::OnControlDockPnl()
{
	if (CT_PNL_DOCK == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_PNL_DOCK;
	}
}

void CMainFrame::OnUpdateControlDockPnl(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_PNL_DOCK) ? TRUE : FALSE);
}

void CMainFrame::OnControlUniformGridPnl()
{
	if (CT_UNIFORM_GRID == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_UNIFORM_GRID;
	}
}

void CMainFrame::OnUpdateControlUniformGridPnl(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_UNIFORM_GRID) ? TRUE : FALSE);
}

void CMainFrame::OnControlStackPnl()
{
	if (CT_PNL_STACK == m_eCurControl)
	{
		m_eCurControl = CT_ARROW;
	}
	else
	{
		m_eCurControl = CT_PNL_STACK;
	}
}

void CMainFrame::OnUpdateControlStackPnl(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((m_eCurControl == CT_PNL_STACK) ? TRUE : FALSE);
}

void CMainFrame::OnEditUndo()
{
	CUIShopDoc *pDoc = GetUIShopDoc();
	pDoc->Undo();
}

void CMainFrame::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	CUIShopDoc *pDoc = GetUIShopDoc();
	pCmdUI->Enable(pDoc->CanUndo());
}

void CMainFrame::OnEditRedo()
{
	CUIShopDoc *pDoc = GetUIShopDoc();
	pDoc->Redo();
}

void CMainFrame::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	CUIShopDoc *pDoc = GetUIShopDoc();
	pCmdUI->Enable(pDoc->CanRedo());
}

void CMainFrame::OnEditCut()
{
	CUIShopView *pView = GetUIShopView();
	ASSERT(pView != NULL);
	CUIDesignWnd *pUIDesignWnd = pView->GetCurUIDesignWnd();
	ASSERT(pUIDesignWnd != NULL);

	vector<CControlWnd *> vControls;
	pUIDesignWnd->GetSelectedControls(&vControls);

	// Cut to clipboard
	if (!OpenClipboard())
	{
		return;
	}

	if (!EmptyClipboard())
	{
		CloseClipboard();
		return;
	}

	//	CVKeyLogic *pvkl = (CVKeyLogic *)pData->pp;

	LONG lSize = 0;
	size_t nCount = vControls.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		CControlWnd *pControlWnd = vControls[i];
		CTRLPROPERTIES *pCtrlProp = pControlWnd->GetControlProperties();
		lSize += (LONG)pCtrlProp->m_lStructSize;
	}

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, lSize);
	if (hGlobal == NULL)
	{
		CloseClipboard();
		return;
	}

	BYTE *pBuffer = (BYTE *)GlobalLock(hGlobal);
	BYTE *pOld = pBuffer;
	memset(pBuffer, 0, lSize);
	for (size_t i = 0; i < nCount; ++i)
	{
		CControlWnd *pControlWnd = vControls[i];
		CTRLPROPERTIES *pCtrlProp = pControlWnd->GetControlProperties();
		LONG lCtrlPropSize = (LONG)pCtrlProp->m_lStructSize;

		memcpy(pBuffer, pCtrlProp, lCtrlPropSize);
		pBuffer += (int)lCtrlPropSize;
	}

	HANDLE hData = SetClipboardData(m_nCtrlPropClipboardFormat, pOld);
	GlobalUnlock(hGlobal);
	CloseClipboard();
}

void CMainFrame::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	CUIShopView *pView = GetUIShopView();
	if (pView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	CUIDesignWnd *pUIDesignWnd = pView->GetCurUIDesignWnd();
	if (pUIDesignWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(pUIDesignWnd->GetSelectedControlsCount() > 0 ? TRUE : FALSE);
}

void CMainFrame::OnEditCopy()
{

}

void CMainFrame::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	CUIShopView *pView = GetUIShopView();
	if (pView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	CUIDesignWnd *pUIDesignWnd = pView->GetCurUIDesignWnd();
	if (pUIDesignWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(pUIDesignWnd->GetSelectedControlsCount() > 0 ? TRUE : FALSE);
}

void CMainFrame::OnEditPaste()
{
	if (!OpenClipboard())
	{
		return;
	}

	// Get clipboard data
	HANDLE hClipboardData = GetClipboardData(m_nCtrlPropClipboardFormat);
	if (hClipboardData == NULL)
	{
		CloseClipboard();
		return;
	}

	CTRLPROPERTIES *pCtrlProp = (CTRLPROPERTIES *)GlobalLock(hClipboardData);
	SIZE_T nSize = GlobalSize(hClipboardData);
	LONG lCtrlPropSize = (LONG)pCtrlProp->m_lStructSize;
	CloseClipboard();

	// Create new CControlWnd.
	CUIShopView *pView = GetUIShopView();
	if (pView == NULL)
	{
		return;
	}
	CUIDesignWnd *pUIDesignWnd = pView->GetCurUIDesignWnd();
	if (pUIDesignWnd == NULL)
	{
		return;
	}

	do
	{
		pUIDesignWnd->PasteNewControl(pCtrlProp);

		nSize -= lCtrlPropSize;
		pCtrlProp = (CTRLPROPERTIES *)((BYTE *)pCtrlProp + lCtrlPropSize);
		lCtrlPropSize = pCtrlProp->m_lStructSize;
	}
	while (nSize != 0);
}

void CMainFrame::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	if (!OpenClipboard())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	// Get clipboard data
	HANDLE hClipboardData = GetClipboardData(m_nCtrlPropClipboardFormat);
	if (hClipboardData == NULL)
	{
		CloseClipboard();
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnDesignSizeToContent()
{
	CUIShopView *pView = GetUIShopView();
	if (pView == NULL)
	{
		return;
	}
	CUIDesignWnd *pUIDesignWnd = pView->GetCurUIDesignWnd();
	if (pUIDesignWnd == NULL)
	{
		return;
	}
	CTRLPROPERTIES *pCtrlProp = pUIDesignWnd->GetFocusCtrl();
	BOOL bUIWndSelected = pUIDesignWnd->IsUIWindowSelected();
	if (pCtrlProp == NULL && !bUIWndSelected)
	{
		return;
	}

	if (bUIWndSelected)
	{
		CIUIImage himgBk;
		himgBk.AutoLoad((LPCTSTR)pUIDesignWnd->GetUIWndProperties()->m_strBKNo);
		if (himgBk == NULL)
		{
			return;
		}

		pUIDesignWnd->GetUIWndProperties()->m_rcControl = CRect(0, 0, himgBk.GetWidth(), himgBk.GetHeight());
		pUIDesignWnd->Invalidate();
		pUIDesignWnd->UpdateFocusControlPosInStatusBar(pUIDesignWnd->GetUIWndProperties());
	}
	else if (pCtrlProp != NULL)
	{
		// Find focus control.
		CControlWnd *pFocusWnd = NULL;
		CControlWnd *pWnd = (CControlWnd *)pUIDesignWnd->GetWindow(GW_CHILD);
		while (pWnd != NULL)
		{
			CTRLPROPERTIES *pcp = pWnd->GetControlProperties();
			if (pcp == pCtrlProp)
			{
				pFocusWnd = pWnd;
				break;
			}

			pWnd = (CControlWnd *)pWnd->GetWindow(GW_HWNDNEXT);
		}
		if (pFocusWnd == NULL)
		{
			ASSERT(FALSE);
			return;
		}

		CIUIImage himgBk;
		CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
		if (eControlType == CT_SCROLLBAREX)
		{
			SBEXPROPERTIES *pSbExProp = (SBEXPROPERTIES *)pCtrlProp;
			if (pSbExProp->m_bVertical)
			{
				VERSBPROPERTIES *pVerSbExProp = (VERSBPROPERTIES *)CUIMgr::GetStyleItem(STYLET_VERSCROLLBAR, (LPCTSTR)pSbExProp->m_strVerScrollbarStyleID);
				if (pVerSbExProp != NULL)
				{
					pCtrlProp = pVerSbExProp;
				}
			}
			else
			{
				HORSBPROPERTIES *pHorSbExProp = (HORSBPROPERTIES *)CUIMgr::GetStyleItem(STYLET_HORSCROLLBAR, (LPCTSTR)pSbExProp->m_strHorScrollbarStyleID);
				if (pHorSbExProp != NULL)
				{
					pCtrlProp = pHorSbExProp;
				}
			}
		}

		if (pCtrlProp->m_bCombineBackgroundImages)
		{
			himgBk.AutoLoad((LPCTSTR)pCtrlProp->m_strBKCombine);
		}
		else
		{
			himgBk.AutoLoad((LPCTSTR)pCtrlProp->m_strBKN);
		}

		int nPart = 1;
		if (eControlType == CT_PUSHBUTTON || eControlType == CT_COMBOBOX || eControlType == CT_EDIT || eControlType == CT_SPIN || eControlType == CT_HOTKEY
			|| eControlType == CT_IPADDRESS || eControlType == CT_SLIDEREX || eControlType == CT_SCROLLBAREX || eControlType == CT_DATETIMECTRL	|| eControlType == CT_SPLITTER)
		{
			nPart = pCtrlProp->m_bCombineBackgroundImages ? 4 : 1;
		}
		else if (eControlType == CT_STATIC || eControlType == CT_LISTCTRL || eControlType == CT_TREECTRL || eControlType == CT_TREELIST || eControlType == CT_THUMBNAIL
			|| eControlType == CT_PROGRESS)
		{
			nPart = pCtrlProp->m_bCombineBackgroundImages ? 2 : 1;
		}
		else if (eControlType == CT_CHECK || eControlType == CT_RADIO)
		{
			nPart = pCtrlProp->m_bCombineBackgroundImages ? 8 : 1;
		}

		if (himgBk.GetWidth() / nPart == 0 || himgBk.GetHeight() == 0)
		{
			AfxMessageBox(_T("Can't set the new size to zero!"));
			return;
		}

		// Add to undo redo list
		::CAction action;
		action.m_eAction = ::CAction::actionMoveControl;
		action.m_pUIWndProp = pUIDesignWnd->GetUIWndProperties();

		CControlMove cm;
		cm.m_pCtrlProp = pFocusWnd->GetControlProperties();
		pFocusWnd->GetWindowRect(cm.m_rectOld);
		pUIDesignWnd->ScreenToClient(cm.m_rectOld);
		cm.m_rectNew = CRect(cm.m_rectOld.left, cm.m_rectOld.top, cm.m_rectOld.left + himgBk.GetWidth() / nPart, cm.m_rectOld.top + himgBk.GetHeight());

		pFocusWnd->MoveWindow(cm.m_rectNew);

		CUIShopDoc *pDoc = GetUIShopDoc();
		ASSERT(pDoc != NULL);
		action.m_ControlMove.AddHead(cm);
		if (!action.m_ControlMove.IsEmpty())
		{
			pDoc->AddUndo(&action);
		}

		pDoc->SetModifiedFlag(TRUE);
	}
}

void CMainFrame::OnUpdateDesignSizeToContent(CCmdUI *pCmdUI)
{
	CUIShopView *pView = GetUIShopView();
	if (pView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	CUIDesignWnd *pUIDesignWnd = pView->GetCurUIDesignWnd();
	if (pUIDesignWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	CTRLPROPERTIES *pCtrlProp = pUIDesignWnd->GetFocusCtrl();
	BOOL bUIWndSelected = pUIDesignWnd->IsUIWindowSelected();
	if (pCtrlProp == NULL && !bUIWndSelected)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if (pCtrlProp != NULL)
	{
		CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
		if (eControlType == CT_SCROLLBAREX)
		{
			SBEXPROPERTIES *pSbExProp = (SBEXPROPERTIES *)pCtrlProp;
			if (pSbExProp->m_bVertical)
			{
				VERSBPROPERTIES *pVerSbExProp = (VERSBPROPERTIES *)CUIMgr::GetStyleItem(STYLET_VERSCROLLBAR, (LPCTSTR)pSbExProp->m_strVerScrollbarStyleID);
				if (pVerSbExProp == NULL)
				{
					pCmdUI->Enable(FALSE);
					return;
				}
			}
			else
			{
				HORSBPROPERTIES *pHorSbExProp = (HORSBPROPERTIES *)CUIMgr::GetStyleItem(STYLET_HORSCROLLBAR, (LPCTSTR)pSbExProp->m_strHorScrollbarStyleID);
				if (pHorSbExProp == NULL)
				{
					pCmdUI->Enable(FALSE);
					return;
				}
			}
		}
		else if (eControlType == CT_WL_PICTURE || eControlType == CT_PICTURE)
		{
			CString strImage = (LPCTSTR)pCtrlProp->m_strBKN;
			if (strImage.IsEmpty())
			{
				pCmdUI->Enable(FALSE);
			}
			else
			{
				pCmdUI->Enable(TRUE);
			}

			return;
		}
		else
		{
			if (!pCtrlProp->m_bSpecifyBackgroundImages)
			{
				pCmdUI->Enable(FALSE);
				return;
			}
		}
	}

	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnDesignSizeToForegroundContent()
{
	CUIShopView *pView = GetUIShopView();
	if (pView == NULL)
	{
		return;
	}
	CUIDesignWnd *pUIDesignWnd = pView->GetCurUIDesignWnd();
	if (pUIDesignWnd == NULL)
	{
		return;
	}
	CTRLPROPERTIES *pCtrlProp = pUIDesignWnd->GetFocusCtrl();
	if (pCtrlProp == NULL)
	{
		return;
	}

	// Find focus control.
	CControlWnd *pFocusWnd = NULL;
	CControlWnd *pWnd = (CControlWnd *)pUIDesignWnd->GetWindow(GW_CHILD);
	while (pWnd != NULL)
	{
		CTRLPROPERTIES *pcp = pWnd->GetControlProperties();
		if (pcp == pCtrlProp)
		{
			pFocusWnd = pWnd;
			break;
		}

		pWnd = (CControlWnd *)pWnd->GetWindow(GW_HWNDNEXT);
	}
	if (pFocusWnd == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	CIUIImage himgBk;

	// Only the following type has a foreground image
	CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	int nPart = 1;

	if (eControlType == CT_PUSHBUTTON || eControlType == CT_CHECK || CT_PUSHBUTTON == CT_RADIO)
	{
		BTNBASEPROPERTIES *pBtnBaseProp = (BTNBASEPROPERTIES *)pCtrlProp;

		if (pBtnBaseProp->m_bCombineForegroundImages)
		{
			himgBk.AutoLoad((LPCTSTR)pBtnBaseProp->m_strFGCombine);
		}
		else
		{
			himgBk.AutoLoad((LPCTSTR)pBtnBaseProp->m_strFGN);
		}

		if (eControlType == CT_PUSHBUTTON)
		{
			nPart = pBtnBaseProp->m_bCombineForegroundImages ? 4 : 1;
		}
		else if (eControlType == CT_CHECK || CT_PUSHBUTTON == CT_RADIO)
		{
			nPart = pBtnBaseProp->m_bCombineForegroundImages ? 8 : 1;
		}
	}
	else if (eControlType == CT_STATIC)
	{
		TXTPROPERTIES *pTxtProp = (TXTPROPERTIES *)pCtrlProp;

		if (pTxtProp->m_bCombineForegroundImages)
		{
			himgBk.AutoLoad((LPCTSTR)pTxtProp->m_strFGCombine);
		}
		else
		{
			himgBk.AutoLoad((LPCTSTR)pTxtProp->m_strFGN);
		}

		nPart = pTxtProp->m_bCombineForegroundImages ? 2 : 1;
	}

	if (nPart == -1 || himgBk == NULL)
	{
		AfxMessageBox(_T("Error"));
		return;
	}

	if (himgBk.GetWidth() / nPart == 0 || himgBk.GetHeight() == 0)
	{
		AfxMessageBox(_T("Can't set the new size to zero!"));
		return;
	}

	// Add to undo redo list
	::CAction action;
	action.m_eAction = ::CAction::actionMoveControl;
	action.m_pUIWndProp = pUIDesignWnd->GetUIWndProperties();

	CControlMove cm;
	cm.m_pCtrlProp = pFocusWnd->GetControlProperties();
	pFocusWnd->GetWindowRect(cm.m_rectOld);
	pUIDesignWnd->ScreenToClient(cm.m_rectOld);
	cm.m_rectNew = CRect(cm.m_rectOld.left, cm.m_rectOld.top, cm.m_rectOld.left + himgBk.GetWidth() / nPart, cm.m_rectOld.top + himgBk.GetHeight());

	pFocusWnd->MoveWindow(cm.m_rectNew);

	CUIShopDoc *pDoc = GetUIShopDoc();
	ASSERT(pDoc != NULL);
	action.m_ControlMove.AddHead(cm);
	if (!action.m_ControlMove.IsEmpty())
	{
		pDoc->AddUndo(&action);
	}

	pDoc->SetModifiedFlag(TRUE);
}

void CMainFrame::OnUpdateDesignSizeToForegroundContent(CCmdUI *pCmdUI)
{
	CUIShopView *pView = GetUIShopView();
	if (pView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	CUIDesignWnd *pUIDesignWnd = pView->GetCurUIDesignWnd();
	if (pUIDesignWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	CTRLPROPERTIES *pCtrlProp = pUIDesignWnd->GetFocusCtrl();
	if (pCtrlProp == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (eControlType == CT_PUSHBUTTON || eControlType == CT_CHECK || CT_PUSHBUTTON == CT_RADIO)
	{
		BTNBASEPROPERTIES *pBtnBaseProp = (BTNBASEPROPERTIES *)pCtrlProp;

		if (pBtnBaseProp->m_bSpecifyForegroundImages)
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	else if (eControlType == CT_STATIC)
	{
		TXTPROPERTIES *pTxtProp = (TXTPROPERTIES *)pCtrlProp;

		if (pTxtProp->m_bSpecifyForegroundImages)
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}

	pCmdUI->Enable(FALSE);
}

void CMainFrame::OnDesignFormatBrush()
{
	if (m_bFormatBrush)
	{
		m_bFormatBrush = FALSE;
	}
	else
	{
		m_bFormatBrush = TRUE;

		m_pFormatBrushProp = NULL;
		m_vBrushPropIndex.clear();
	}
}

void CMainFrame::OnUpdateDesignFormatBrush(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bFormatBrush);
}

void CMainFrame::OnDesignBackgroundShow()
{
	CUIDesignWnd *pUIDesignWnd  = GetUIShopView()->GetCurUIDesignWnd();
	if (pUIDesignWnd == NULL)
	{
		return;
	}

	int nCtrlCount = pUIDesignWnd->m_listCtrl.size();

	if (pUIDesignWnd->m_bBackgroundShow)
	{
		pUIDesignWnd->m_bBackgroundShow = FALSE;

		for (int i = 0; i < nCtrlCount; ++i)
		{
			CControlWnd *pCtrlWnd = pUIDesignWnd->m_listCtrl[i];
			ASSERT(pCtrlWnd->GetSafeHwnd() != NULL);

			CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlWnd->GetControlProperties()->m_eControlType;
			if (eControlType == CT_WL_RECTCTRL
				|| eControlType == CT_WL_PICTURE
				|| eControlType == CT_WL_LINE
				|| eControlType == CT_PNL_DOCK
				|| eControlType == CT_UNIFORM_GRID
				|| eControlType == CT_PNL_STACK)
			{
				pCtrlWnd->ShowWindow(SW_SHOW);
			}
		}
	}
	else
	{
		pUIDesignWnd->m_bBackgroundShow = TRUE;

		for (int i = 0; i < nCtrlCount; ++i)
		{
			CControlWnd *pCtrlWnd = pUIDesignWnd->m_listCtrl[i];
			ASSERT(pCtrlWnd->GetSafeHwnd() != NULL);

			CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlWnd->GetControlProperties()->m_eControlType;
			if (eControlType == CT_WL_RECTCTRL
				|| eControlType == CT_WL_PICTURE
				|| eControlType == CT_WL_LINE
				|| eControlType == CT_PNL_DOCK
				|| eControlType == CT_UNIFORM_GRID
				|| eControlType == CT_PNL_STACK)
			{
				pCtrlWnd->ShowWindow(SW_HIDE);
			}
		}
	}

	pUIDesignWnd->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
}

void CMainFrame::OnUpdateDesignBackgroundShow(CCmdUI *pCmdUI)
{
	CUIShopView *pView = GetUIShopView();
	if (pView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	CUIDesignWnd *pUIDesignWnd  = GetUIShopView()->GetCurUIDesignWnd();
	if (pUIDesignWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->SetCheck(pUIDesignWnd->m_bBackgroundShow);
}

void CMainFrame::OnEditUnusedImageSymbols()
{
	CImageSymbolsDlg dlg(ISDM_UNUSED);
	dlg.DoModal();
}

void CMainFrame::OnUpdateEditUnusedImageSymbols(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnEditUnfoundImageSymbols()
{
	CImageSymbolsDlg dlg(ISDM_UNFOUND);
	dlg.DoModal();
}

void CMainFrame::OnUpdateEditUnfoundImageSymbols(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnToolsCreateLanguageFile()
{
	CFileDialog dlg(FALSE);
	INT_PTR nRet = dlg.DoModal();
	if (nRet == IDCANCEL)
	{
		return;
	}

	CString strFile = dlg.GetPathName();

	CUIMgr::CreateIniLanguageFile(strFile);
}

void CMainFrame::OnUpdateToolsCreateLanguageFile(CCmdUI *pCmdUI)
{
	CUIShopDoc *pDoc = GetUIShopDoc();
	pCmdUI->Enable(pDoc == NULL ? FALSE : TRUE);
}

void CMainFrame::OnToolsCreateResourceFile()
{
	CGenerateResourceDlg dlg;
	INT_PTR nRet = dlg.DoModal();
	if (nRet == IDCANCEL)
	{
		return;
	}

	CGenerateResource gr;
	gr.SetSource(dlg.m_strSavePath);
	gr.SetLanguage(dlg.m_nLanguage);
	gr.RelateSavePath(dlg.m_bRelateSavePath);
	if (!dlg.m_bRelateSavePath)
	{
		gr.SetCustomRelatePath(dlg.m_strRelateCustom);
	}
	gr.GeneralResource();
}

void CMainFrame::OnUpdateToolsCreateResourceFile(CCmdUI *pCmdUI)
{
	CUIShopDoc *pDoc = GetUIShopDoc();
	pCmdUI->Enable(pDoc == NULL ? FALSE : TRUE);
}

void CMainFrame::OnToolsTransformDialog()
{
	static CTransformDialogDlg dlg;
	if (dlg.GetSafeHwnd() != NULL)
	{
		dlg.ShowWindow(SW_SHOW);
	}
	else
	{
		dlg.Create(IDD_TRANSFORM_DIALOG);
		dlg.ShowWindow(SW_SHOW);
	}
}

void CMainFrame::OnToolsCreateClassInfoFile()
{
	CGenClassInfoFileDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnToolsGenerateResourceH()
{
	// Load resource.h each time for new data.
	CUIMgr::LoadIDDefined();

	// Generate resource.h file
	int nRet = CUIMgr::SaveIDDefined();
	if (nRet == 0 || nRet == 1)
	{
		AfxMessageBox(_T("Successed to generate resource.h file!"));
	}
	else
	{
		AfxMessageBox(_T("Failed to generate resource.h file!"));
	}
}

void CMainFrame::OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CMDIFrameWndEx::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: Add your message handler code here
}

void CMainFrame::OnToolsSyncSkinTabOrder()
{
	CSyncSkinTabOrderDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnToolsRenameTemplateFile()
{
	CRenameTemplateFileDlg dlg;
	dlg.DoModal();
}
