// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "$$root$$.h"
#include "MainFrm.h"
$$IF(MDI)
#include "$$root$$Doc.h"
$$ENDIF

#ifdef _DEBUG
#define new DEBUG_NEW
$$IF(PROJECT_VC60)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
$$ENDIF
#endif

$$IF(PROJECT_VC60)
/////////////////////////////////////////////////////////////////////////////
$$ENDIF
// CMainFrame

$$IF(MDI)
BEGIN_MESSAGE_MAP(CMainFrame, CIUIMDIFrameWnd)
$$ENDIF
$$IF(!MDI)
BEGIN_MESSAGE_MAP(CMainFrame, CUIWnd)
$$ENDIF
$$IF(PROJECT_VC60)
	//{{AFX_MSG_MAP(CMainFrame)
$$ENDIF
	ON_WM_CREATE()
$$IF(DLG)
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
$$ENDIF
$$IF(MDI)
	ON_BN_CLICKED(IDC_BTN_PROJECT, OnBtnProject)
	ON_BN_CLICKED(IDC_BTN_EDIT, OnBtnEdit)
	ON_BN_CLICKED(IDC_BTN_VIEW, OnBtnView)
	ON_BN_CLICKED(IDC_BTN_LEFT, OnBtnLeft)
	ON_BN_CLICKED(IDC_BTN_RIGHT, OnBtnRight)
	ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
$$ENDIF // MDI
$$IF(IM)
	ON_WM_GETMINMAXINFO()
	ON_MESSAGE(WS_MENUWND_DESTROY, OnStartMenuDestroy)
	ON_MESSAGE(WS_MENUWND_HIDDEN, OnStartMenuHidden)
	ON_BN_CLICKED(IDC_BTN_SKIN, OnBtnSkin)
	ON_BN_CLICKED(IDC_BTN_HEADER, OnBtnHeader)
	ON_BN_CLICKED(IDC_CHK_STATUS, OnChkStatus)
	ON_BN_CLICKED(IDC_BTN_MAIL, OnBtnMail)
	ON_BN_CLICKED(IDC_BTN_ZONE, OnBtnZone)
	ON_BN_CLICKED(IDC_BTN_MESSAGE, OnBtnMessage)
	ON_BN_CLICKED(IDC_BTN_SEARCH_WEB, OnBtnSearchWeb)
	ON_BN_CLICKED(IDC_CHK_START, OnChkStart)
	ON_BN_CLICKED(IDC_BTN_GAME, OnBtnGame)
	ON_BN_CLICKED(IDC_BTN_SETTING, OnBtnSetting)
	ON_BN_CLICKED(IDC_BTN_MSG, OnBtnMsg)
	ON_BN_CLICKED(IDC_BTN_SECURE, OnBtnSecure)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_APP_MGR, OnBtnAppMgr)
	ON_BN_CLICKED(IDC_BTN_WEATHER, OnBtnWeather)
	ON_BN_CLICKED(IDC_BTN_SIGN, OnBtnSign)
$$ENDIF // IM
$$IF(PROJECT_VC60)
	//}}AFX_MSG_MAP
$$ENDIF
END_MESSAGE_MAP()

$$IF(PROJECT_VC60)
/////////////////////////////////////////////////////////////////////////////
$$ENDIF
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
$$IF(DLG)
	m_pStaTitle = NULL;
	m_pBtnMin = NULL;
	m_pChkMax = NULL;
	m_pBtnClose = NULL;
	m_pStaMsg = NULL;
	m_pBtnOk = NULL;

	m_bMax = FALSE;
$$ENDIF
$$IF(MDI)
	m_pBtnMin = NULL;
	m_pChkMax = NULL;
	m_pBtnClose = NULL;
	m_pMenuBarMain = NULL;
	m_pBtnProject = NULL;
	m_pBtnEdit = NULL;
	m_pBtnView = NULL;
	m_pBtnLeft = NULL;
	m_pBtnRight = NULL;
	m_pBtnUp = NULL;
	m_pBtnDown = NULL;
	m_pRadClassView = NULL;
	m_pRadFileView = NULL;
	m_pTWMgrLeft = NULL;

	m_bMax = FALSE;

	memset(m_hMDITab, 0, sizeof(m_hMDITab));

$$ENDIF // MDI
$$IF(IM)
	m_pBtnSkin = NULL;
	m_pBtnMin = NULL;
	m_pChkMax = NULL;
	m_pBtnClose = NULL;
	m_pBtnHeader = NULL;
	m_pChkStatus = NULL;
	m_pBtnMail = NULL;
	m_pBtnZone = NULL;
	m_pBtnMessage = NULL;
	m_pEdtSearch = NULL;
	m_pBtnSearchWeb = NULL;
	m_pRadFriend = NULL;
	m_pRadGroup = NULL;
	m_pRadRecent = NULL;
	m_pChkStart = NULL;
	m_pBtnGame = NULL;
	m_pBtnSetting = NULL;
	m_pBtnMsg = NULL;
	m_pBtnSecure = NULL;
	m_pBtnSearch = NULL;
	m_pBtnAppMgr = NULL;
	m_pBtnWeather = NULL;
	m_pStaName = NULL;
	m_pBtnSign = NULL;
	m_pStaTitle = NULL;
	m_pTWMgrMain = NULL;
	m_pUGridPnlTab = NULL;

	m_nColorIndex = 0;

	m_bMax = FALSE;
	m_bStatus = FALSE;
	m_strSearch = _T("");
	m_bStart = FALSE;

$$ENDIF // IM
$$IF(VERBOSE)
	// TODO: add member initialization code here

$$ENDIF
}

CMainFrame::~CMainFrame()
{
$$IF(MDI)
	for (int i = 0; i < 8 ; ++i)
	{
		ReleaseIUIImage(m_hMDITab[i]);
		m_hMDITab[i] = NULL;
	}
$$ENDIF // MDI
}

void CMainFrame::DoDataExchange(CDataExchange* pDX)
{
	CUIWnd::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHK_MAX, m_bMax);
$$IF(IM)
	DDX_Check(pDX, IDC_CHK_STATUS, m_bStatus);
	DDX_Text(pDX, IDC_EDT_SEARCH, m_strSearch);
	DDX_Check(pDX, IDC_CHK_START, m_bStart);
$$ENDIF // IM
}


//////////////////////////////////////////////////////////////////////////
// protected

int CMainFrame::InitControls()
{
$$IF(DLG)
	m_pStaTitle = (CSkinStatic *)GetDlgItem(IDC_STA_TITLE);
	ASSERT(m_pStaTitle->GetSafeHwnd() != NULL);
	m_pBtnMin = (CSkinButton *)GetDlgItem(IDC_BTN_MIN);
	ASSERT(m_pBtnMin->GetSafeHwnd() != NULL);
	m_pChkMax = (CSkinButton *)GetDlgItem(IDC_CHK_MAX);
	ASSERT(m_pChkMax->GetSafeHwnd() != NULL);
	m_pBtnClose = (CSkinButton *)GetDlgItem(IDC_BTN_CLOSE);
	ASSERT(m_pBtnClose->GetSafeHwnd() != NULL);
	m_pStaMsg = (CSkinStatic *)GetDlgItem(IDC_STA_MSG);
	ASSERT(m_pStaMsg->GetSafeHwnd() != NULL);
	m_pBtnOk = (CSkinButton *)GetDlgItem(IDC_BTN_OK);
	ASSERT(m_pBtnOk->GetSafeHwnd() != NULL);

$$ENDIF // DLG
$$IF(MDI)
	m_pBtnMin = (CSkinButton *)GetDlgItem(IDC_BTN_MIN);
	ASSERT(m_pBtnMin->GetSafeHwnd() != NULL);
	m_pChkMax = (CSkinButton *)GetDlgItem(IDC_CHK_MAX);
	ASSERT(m_pChkMax->GetSafeHwnd() != NULL);
	m_pBtnClose = (CSkinButton *)GetDlgItem(IDC_BTN_CLOSE);
	ASSERT(m_pBtnClose->GetSafeHwnd() != NULL);
	m_pMenuBarMain = (CMenuBar *)GetDlgItem(IDC_MENUBAR_MAIN);
	ASSERT(m_pMenuBarMain->GetSafeHwnd() != NULL);
	m_pBtnProject = (CSkinButton *)GetDlgItem(IDC_BTN_PROJECT);
	ASSERT(m_pBtnProject->GetSafeHwnd() != NULL);
	m_pBtnEdit = (CSkinButton *)GetDlgItem(IDC_BTN_EDIT);
	ASSERT(m_pBtnEdit->GetSafeHwnd() != NULL);
	m_pBtnView = (CSkinButton *)GetDlgItem(IDC_BTN_VIEW);
	ASSERT(m_pBtnView->GetSafeHwnd() != NULL);
	m_pBtnLeft = (CSkinButton *)GetDlgItem(IDC_BTN_LEFT);
	ASSERT(m_pBtnLeft->GetSafeHwnd() != NULL);
	m_pBtnRight = (CSkinButton *)GetDlgItem(IDC_BTN_RIGHT);
	ASSERT(m_pBtnRight->GetSafeHwnd() != NULL);
	m_pBtnUp = (CSkinButton *)GetDlgItem(IDC_BTN_UP);
	ASSERT(m_pBtnUp->GetSafeHwnd() != NULL);
	m_pBtnDown = (CSkinButton *)GetDlgItem(IDC_BTN_DOWN);
	ASSERT(m_pBtnDown->GetSafeHwnd() != NULL);
	m_pRadClassView = (CSkinButton *)GetDlgItem(IDC_RAD_CLASS_VIEW);
	ASSERT(m_pRadClassView->GetSafeHwnd() != NULL);
	m_pRadFileView = (CSkinButton *)GetDlgItem(IDC_RAD_FILE_VIEW);
	ASSERT(m_pRadFileView->GetSafeHwnd() != NULL);
	m_pTWMgrLeft = (CTaskWndMgr *)GetDlgWLItem(IDC_TWMGR_LEFT);
	ASSERT(m_pTWMgrLeft != NULL);

$$ENDIF // MDI
$$IF(IM)
	m_pBtnSkin = (CSkinButton *)GetDlgItem(IDC_BTN_SKIN);
	ASSERT(m_pBtnSkin->GetSafeHwnd() != NULL);
	m_pBtnMin = (CSkinButton *)GetDlgItem(IDC_BTN_MIN);
	ASSERT(m_pBtnMin->GetSafeHwnd() != NULL);
	m_pChkMax = (CSkinButton *)GetDlgItem(IDC_CHK_MAX);
	ASSERT(m_pChkMax->GetSafeHwnd() != NULL);
	m_pBtnClose = (CSkinButton *)GetDlgItem(IDC_BTN_CLOSE);
	ASSERT(m_pBtnClose->GetSafeHwnd() != NULL);
	m_pBtnHeader = (CSkinButton *)GetDlgItem(IDC_BTN_HEADER);
	ASSERT(m_pBtnHeader->GetSafeHwnd() != NULL);
	m_pChkStatus = (CSkinButton *)GetDlgItem(IDC_CHK_STATUS);
	ASSERT(m_pChkStatus->GetSafeHwnd() != NULL);
	m_pBtnMail = (CSkinButton *)GetDlgItem(IDC_BTN_MAIL);
	ASSERT(m_pBtnMail->GetSafeHwnd() != NULL);
	m_pBtnZone = (CSkinButton *)GetDlgItem(IDC_BTN_ZONE);
	ASSERT(m_pBtnZone->GetSafeHwnd() != NULL);
	m_pBtnMessage = (CSkinButton *)GetDlgItem(IDC_BTN_MESSAGE);
	ASSERT(m_pBtnMessage->GetSafeHwnd() != NULL);
	m_pEdtSearch = (CSkinEdit *)GetDlgItem(IDC_EDT_SEARCH);
	ASSERT(m_pEdtSearch->GetSafeHwnd() != NULL);
	m_pBtnSearchWeb = (CSkinButton *)GetDlgItem(IDC_BTN_SEARCH_WEB);
	ASSERT(m_pBtnSearchWeb->GetSafeHwnd() != NULL);
	m_pRadFriend = (CSkinButton *)GetDlgItem(IDC_RAD_FRIEND);
	ASSERT(m_pRadFriend->GetSafeHwnd() != NULL);
	m_pRadGroup = (CSkinButton *)GetDlgItem(IDC_RAD_GROUP);
	ASSERT(m_pRadGroup->GetSafeHwnd() != NULL);
	m_pRadRecent = (CSkinButton *)GetDlgItem(IDC_RAD_RECENT);
	ASSERT(m_pRadRecent->GetSafeHwnd() != NULL);
	m_pChkStart = (CSkinButton *)GetDlgItem(IDC_CHK_START);
	ASSERT(m_pChkStart->GetSafeHwnd() != NULL);
	m_pBtnGame = (CSkinButton *)GetDlgItem(IDC_BTN_GAME);
	ASSERT(m_pBtnGame->GetSafeHwnd() != NULL);
	m_pBtnSetting = (CSkinButton *)GetDlgItem(IDC_BTN_SETTING);
	ASSERT(m_pBtnSetting->GetSafeHwnd() != NULL);
	m_pBtnMsg = (CSkinButton *)GetDlgItem(IDC_BTN_MSG);
	ASSERT(m_pBtnMsg->GetSafeHwnd() != NULL);
	m_pBtnSecure = (CSkinButton *)GetDlgItem(IDC_BTN_SECURE);
	ASSERT(m_pBtnSecure->GetSafeHwnd() != NULL);
	m_pBtnSearch = (CSkinButton *)GetDlgItem(IDC_BTN_SEARCH);
	ASSERT(m_pBtnSearch->GetSafeHwnd() != NULL);
	m_pBtnAppMgr = (CSkinButton *)GetDlgItem(IDC_BTN_APP_MGR);
	ASSERT(m_pBtnAppMgr->GetSafeHwnd() != NULL);
	m_pBtnWeather = (CSkinButton *)GetDlgItem(IDC_BTN_WEATHER);
	ASSERT(m_pBtnWeather->GetSafeHwnd() != NULL);
	m_pStaName = (CSkinStatic *)GetDlgItem(IDC_STA_NAME);
	ASSERT(m_pStaName->GetSafeHwnd() != NULL);
	m_pBtnSign = (CSkinButton *)GetDlgItem(IDC_BTN_SIGN);
	ASSERT(m_pBtnSign->GetSafeHwnd() != NULL);
	m_pStaTitle = (CSkinStatic *)GetDlgItem(IDC_STA_TITLE);
	ASSERT(m_pStaTitle->GetSafeHwnd() != NULL);
	m_pTWMgrMain = (CTaskWndMgr *)GetDlgWLItem(IDC_TWMGR_MAIN);
	ASSERT(m_pTWMgrMain != NULL);
	m_pUGridPnlTab = (CUniformGridPanel *)GetDlgWLItem(IDC_PNL_UGRID_TAB);
	ASSERT(m_pUGridPnlTab != NULL);

$$ENDIF // IM
	return 0;
}

$$IF(MDI)
int CMainFrame::InitMenuBar()
{
	m_pMenuBarMain->SetMenuID(IDR_MAINFRAME);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// virtual

CMenuBar *CMainFrame::GetDefaultMenuBar()
{
	return m_pMenuBarMain;
}

BOOL CMainFrame::GetMDIClientWindowID(UINT *puID)
{
	if (puID == NULL)
	{
		return FALSE;
	}

	*puID = IDW_MDI_CLIENT;

	return TRUE;
}

BOOL CMainFrame::GetMDITabImages(CString *pstrImageNameN, CString *pstrImageNameH,
	CString *pstrImageNameS, CString *pstrImageNameD,
	CString *pstrImageNameCN, CString *pstrImageNameCH,
	CString *pstrImageNameCS, CString *pstrImageNameCD)
{
	if (pstrImageNameN != NULL)
	{
		*pstrImageNameN = m_hMDITab[0]->GetSafeImageName();
	}
	if (pstrImageNameH != NULL)
	{
		*pstrImageNameH = m_hMDITab[1]->GetSafeImageName();
	}
	if (pstrImageNameS != NULL)
	{
		*pstrImageNameS = m_hMDITab[2]->GetSafeImageName();
	}
	if (pstrImageNameD != NULL)
	{
		*pstrImageNameD = m_hMDITab[3]->GetSafeImageName();
	}
	if (pstrImageNameCN != NULL)
	{
		*pstrImageNameCN = m_hMDITab[4]->GetSafeImageName();
	}
	if (pstrImageNameCH != NULL)
	{
		*pstrImageNameCH = m_hMDITab[5]->GetSafeImageName();
	}
	if (pstrImageNameCS != NULL)
	{
		*pstrImageNameCS = m_hMDITab[6]->GetSafeImageName();
	}
	if (pstrImageNameCD != NULL)
	{
		*pstrImageNameCD = m_hMDITab[7]->GetSafeImageName();
	}

	return TRUE;
}

BOOL CMainFrame::GetMDITabsRect(LPRECT lpRect)
{
	if (lpRect != NULL)
	{
		*lpRect = CRect(0, 0, 40, 25);
	}

	return TRUE;
}

BOOL CMainFrame::GetSwitchDocumentButtonID(UINT *puID)
{
	if (puID != NULL)
	{
		*puID = IDC_CHK_SWITCH_DOCUMENT;
	}

	return TRUE;
}

BOOL CMainFrame::GetCloseDocumentButtonID(UINT *puID)
{
	if (puID != NULL)
	{
		*puID = IDC_BTN_CLOSE_DOCUMENT;
	}

	return TRUE;
}

$$ENDIF // MDI
/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
$$IF(MDI)
	HIUIIMAGE hMDITabs = CreateIUIImage(_T("MDIBtn_Tab.bmp"));
	int nWidth = hMDITabs->GetWidth() / 8;
	for (int i = 0; i < 8 ; ++i)
	{
		IUISubBitmap(hMDITabs, &m_hMDITab[i], nWidth * i, 0, nWidth, hMDITabs->GetHeight());
	}
	ReleaseIUIImage(hMDITabs);
	hMDITabs = NULL;

	if (CIUIMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
$$ENDIF
$$IF(!MDI)
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
$$ENDIF
	{
		return -1;
	}

	if (InitControls() != 0)
	{
		return -1;
	}

$$IF(MDI)
	InitMenuBar();

	m_pTWMgrLeft->AddTaskWindow(IDC_RAD_CLASS_VIEW, TRUE, &m_wndClassView, IDW_CLASS_VIEW, _T("ClassView"));
	m_pTWMgrLeft->AddTaskWindow(IDC_RAD_FILE_VIEW, TRUE, &m_wndFileView, IDW_FILE_VIEW, _T("FileView"));
	m_pTWMgrLeft->SwitchToTaskWindow(IDC_RAD_CLASS_VIEW);

	CRect rect(0, 0, 1024, 700);
	GetDPIRect(rect);
	MoveWindow(rect);
$$ENDIF
	CenterWindow();

$$IF(IM)
	// Layout tab button
	for (int i = 0; i < 3; ++i)
	{
		m_pUGridPnlTab->InsertColumn(i);
	}
	m_pUGridPnlTab->InsertRow(0);
	m_pUGridPnlTab->AddElement(m_pRadFriend, 0, 0, GEAH_STRETCH, GEAV_CENTER);
	m_pUGridPnlTab->AddElement(m_pRadGroup, 0, 1, GEAH_STRETCH, GEAV_CENTER);
	m_pUGridPnlTab->AddElement(m_pRadRecent, 0, 2, GEAH_STRETCH, GEAV_CENTER);

	// Add tab
	m_pTWMgrMain->AddTaskWindow(IDC_RAD_FRIEND, TRUE, &m_wndFriend, IDW_FRIEND_PAGE, _T("Friend"));
	m_pTWMgrMain->AddTaskWindow(IDC_RAD_GROUP, TRUE, &m_wndGroup, IDW_GROUP_PAGE, _T("Group"));
	m_pTWMgrMain->AddTaskWindow(IDC_RAD_RECENT, TRUE, &m_wndRecent, IDW_RECENT_PAGE, _T("Recent"));
	m_pTWMgrMain->SwitchToTaskWindow(IDC_RAD_FRIEND);

$$ENDIF // IM
$$IF(VERBOSE)
	// TODO: Add your message handler code here

$$ENDIF
	return 0;
}
$$IF(MDI)

void CMainFrame::OnBtnProject()
{
	AfxMessageBox(_T("IDC_BTN_PROJECT"));
}

void CMainFrame::OnBtnEdit()
{
	AfxMessageBox(_T("IDC_BTN_EDIT"));
}

void CMainFrame::OnBtnView()
{
	AfxMessageBox(_T("IDC_BTN_VIEW"));
}

void CMainFrame::OnBtnLeft()
{
	AfxMessageBox(_T("IDC_BTN_LEFT"));
}

void CMainFrame::OnBtnRight()
{
	AfxMessageBox(_T("IDC_BTN_RIGHT"));
}

void CMainFrame::OnBtnUp()
{
	AfxMessageBox(_T("IDC_BTN_UP"));
}

void CMainFrame::OnBtnDown()
{
	AfxMessageBox(_T("IDC_BTN_DOWN"));
}
$$ENDIF // MDI
$$IF(DLG)

void CMainFrame::OnBtnOk()
{
	AfxMessageBox(_T("IDC_BTN_OK"));
}
$$ENDIF // DLG
$$IF(IM)

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	lpMMI->ptMaxSize.x = 600;

	CUIWnd::OnGetMinMaxInfo(lpMMI);
}

LRESULT CMainFrame::OnStartMenuDestroy(WPARAM wParam, LPARAM lParam)
{
	m_pChkStart->SetCheck(BST_UNCHECKED);

	return 0;
}

LRESULT CMainFrame::OnStartMenuHidden(WPARAM wParam, LPARAM lParam)
{
	m_pChkStart->SetCheck(BST_UNCHECKED);

	return 0;
}

void CMainFrame::OnBtnSkin()
{
	if (m_nColorIndex == 0)
	{
		SetBkColor(RGB(241, 158, 189));
	}
	else if (m_nColorIndex == 1)
	{
		SetBkColor(RGB(155, 114, 239));
	}
	else if (m_nColorIndex == 2)
	{
		SetBkColor(RGB(255, 228, 1));
	}

	++m_nColorIndex;
	if (m_nColorIndex > 2)
		m_nColorIndex = 0;
}

void CMainFrame::OnBtnHeader()
{
	AfxMessageBox(_T("IDC_BTN_HEADER"));
}

void CMainFrame::OnChkStatus()
{
	AfxMessageBox(_T("IDC_CHK_STATUS"));
}

void CMainFrame::OnBtnMail()
{
	AfxMessageBox(_T("IDC_BTN_MAIL"));
}

void CMainFrame::OnBtnZone()
{
	AfxMessageBox(_T("IDC_BTN_ZONE"));
}

void CMainFrame::OnBtnMessage()
{
	AfxMessageBox(_T("IDC_BTN_MESSAGE"));
}

void CMainFrame::OnBtnSearchWeb()
{
	AfxMessageBox(_T("IDC_BTN_SEARCH_WEB"));
}

void CMainFrame::OnChkStart()
{
	if (m_wndStartMenu.GetSafeHwnd() == NULL)
		m_wndStartMenu.Create(this);

	CRect rcStartButton;
	m_pChkStart->GetWindowRect(rcStartButton);

	CRect rcStartMenu;
	m_wndStartMenu.GetClientRect(rcStartMenu);

	m_wndStartMenu.Popup(rcStartButton.left + 1, rcStartButton.top - rcStartMenu.Height() + 34);
}

void CMainFrame::OnBtnGame()
{
	AfxMessageBox(_T("IDC_BTN_GAME"));
}

void CMainFrame::OnBtnSetting()
{
	AfxMessageBox(_T("IDC_BTN_SETTING"));
}

void CMainFrame::OnBtnMsg()
{
	AfxMessageBox(_T("IDC_BTN_MSG"));
}

void CMainFrame::OnBtnSecure()
{
	AfxMessageBox(_T("IDC_BTN_SECURE"));
}

void CMainFrame::OnBtnSearch()
{
	AfxMessageBox(_T("IDC_BTN_SEARCH"));
}

void CMainFrame::OnBtnAppMgr()
{
	AfxMessageBox(_T("IDC_BTN_APP_MGR"));
}

void CMainFrame::OnBtnWeather()
{
	AfxMessageBox(_T("IDC_BTN_WEATHER"));
}

void CMainFrame::OnBtnSign()
{
	AfxMessageBox(_T("IDC_BTN_SIGN"));
}
$$ENDIF // IM
