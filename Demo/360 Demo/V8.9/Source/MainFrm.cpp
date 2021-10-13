// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SafeMgr.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

BEGIN_MESSAGE_MAP(CMainFrame, CUIWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_SKIN, OnBtnSkin)
	ON_BN_CLICKED(IDC_BTN_MENU, OnBtnMenu)
	ON_BN_CLICKED(IDC_BTN_CHECK_UPDATE, OnBtnCheckUpdate)
	ON_BN_CLICKED(IDC_BTN_LOGO, OnBtnLogo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_pWLTxtTitle = NULL;
	m_pBtnSkin = NULL;
	m_pBtnMenu = NULL;
	m_pBtnMin = NULL;
	m_pChkMax = NULL;
	m_pBtnClose = NULL;
	m_pRadExamine = NULL;
	m_pRadDsmain = NULL;
	m_pRadVulRepair = NULL;
	m_pRadSysRepair = NULL;
	m_pRadTraceCleaner = NULL;
	m_pRadSpeedup = NULL;
	m_pRadDnmz = NULL;
	m_pRadSoftMgr = NULL;
	m_pRadAdvTools = NULL;
	m_pWLTxtVer = NULL;
	m_pBtnCheckUpdate = NULL;
	m_pWLTxtCloud = NULL;
	m_pBtnLogo = NULL;
	m_pTWMgrMain = NULL;

	m_bMax = FALSE;
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::DoDataExchange(CDataExchange* pDX)
{
	CUIWnd::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CMainFrame)
	DDX_Check(pDX, IDC_CHK_MAX, m_bMax);
	//}}AFX_DATA_MAP
}


//////////////////////////////////////////////////////////////////////////
// protected

int CMainFrame::InitControls()
{
	m_pWLTxtTitle = (CWLText *)GetDlgWLItem(IDC_WL_TXT_TITLE);
	ASSERT(m_pWLTxtTitle != NULL);
	m_pBtnSkin = (CSkinButton *)GetDlgItem(IDC_BTN_SKIN);
	ASSERT(m_pBtnSkin->GetSafeHwnd() != NULL);
	m_pBtnMenu = (CSkinButton *)GetDlgItem(IDC_BTN_MENU);
	ASSERT(m_pBtnMenu->GetSafeHwnd() != NULL);
	m_pBtnMin = (CSkinButton *)GetDlgItem(IDC_BTN_MIN);
	ASSERT(m_pBtnMin->GetSafeHwnd() != NULL);
	m_pChkMax = (CSkinButton *)GetDlgItem(IDC_CHK_MAX);
	ASSERT(m_pChkMax->GetSafeHwnd() != NULL);
	m_pBtnClose = (CSkinButton *)GetDlgItem(IDC_BTN_CLOSE);
	ASSERT(m_pBtnClose->GetSafeHwnd() != NULL);
	m_pRadExamine = (CSkinButton *)GetDlgItem(IDC_RAD_EXAMINE);
	ASSERT(m_pRadExamine->GetSafeHwnd() != NULL);
	m_pRadDsmain = (CSkinButton *)GetDlgItem(IDC_RAD_DSMAIN);
	ASSERT(m_pRadDsmain->GetSafeHwnd() != NULL);
	m_pRadVulRepair = (CSkinButton *)GetDlgItem(IDC_RAD_VUL_REPAIR);
	ASSERT(m_pRadVulRepair->GetSafeHwnd() != NULL);
	m_pRadSysRepair = (CSkinButton *)GetDlgItem(IDC_RAD_SYS_REPAIR);
	ASSERT(m_pRadSysRepair->GetSafeHwnd() != NULL);
	m_pRadTraceCleaner = (CSkinButton *)GetDlgItem(IDC_RAD_TRACE_CLEANER);
	ASSERT(m_pRadTraceCleaner->GetSafeHwnd() != NULL);
	m_pRadSpeedup = (CSkinButton *)GetDlgItem(IDC_RAD_SPEEDUP);
	ASSERT(m_pRadSpeedup->GetSafeHwnd() != NULL);
	m_pRadDnmz = (CSkinButton *)GetDlgItem(IDC_RAD_DNMZ);
	ASSERT(m_pRadDnmz->GetSafeHwnd() != NULL);
	m_pRadSoftMgr = (CSkinButton *)GetDlgItem(IDC_RAD_SOFT_MGR);
	ASSERT(m_pRadSoftMgr->GetSafeHwnd() != NULL);
	m_pRadAdvTools = (CSkinButton *)GetDlgItem(IDC_RAD_ADV_TOOLS);
	ASSERT(m_pRadAdvTools->GetSafeHwnd() != NULL);
	m_pWLTxtVer = (CWLText *)GetDlgWLItem(IDC_WL_TXT_VER);
	ASSERT(m_pWLTxtVer != NULL);
	m_pBtnCheckUpdate = (CSkinButton *)GetDlgItem(IDC_BTN_CHECK_UPDATE);
	ASSERT(m_pBtnCheckUpdate->GetSafeHwnd() != NULL);
	m_pWLTxtCloud = (CWLText *)GetDlgWLItem(IDC_WL_TXT_CLOUD);
	ASSERT(m_pWLTxtCloud != NULL);
	m_pBtnLogo = (CSkinButton *)GetDlgItem(IDC_BTN_LOGO);
	ASSERT(m_pBtnLogo->GetSafeHwnd() != NULL);
	m_pTWMgrMain = (CTaskWndMgr *)GetDlgWLItem(IDC_TWMGR_MAIN);
	ASSERT(m_pTWMgrMain != NULL);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (InitControls() != 0)
		return -1;

	CenterWindow();

	// TODO: Add your message handler code here

	m_pTWMgrMain->AddTaskWindow(IDC_RAD_EXAMINE, TRUE, &m_wndExamineFrame, IDW_EXAMINE_FRM);
	m_pTWMgrMain->AddTaskWindow(IDC_RAD_DSMAIN, TRUE, &m_wndDSMain, IDW_DSMAIN);
	m_pTWMgrMain->AddTaskWindow(IDC_RAD_VUL_REPAIR, TRUE, &m_wndWeb, IDW_WEB);
	m_pTWMgrMain->SwitchToTaskWindow(IDC_RAD_EXAMINE);
	
	return 0;
}

void CMainFrame::OnBtnSkin()
{
	static BOOL bFlag = TRUE;
	if (bFlag)
	{
		SetBkColor(RGB(18, 94, 22));
		bFlag = FALSE;
	}
	else
	{
		SetBkColor(RGB(15, 86, 150));
		bFlag = TRUE;
	}
}

void CMainFrame::OnBtnMenu()
{
	AfxMessageBox(_T("IDC_BTN_MENU"));
}

void CMainFrame::OnBtnCheckUpdate()
{
	ShellExecute(NULL, _T("Open"), _T("http://www.iuishop.com"), NULL, NULL, SW_SHOW);
}

void CMainFrame::OnBtnLogo()
{
	ShellExecute(NULL, _T("Open"), _T("http://www.iuishop.com"), NULL, NULL, SW_SHOW);
}
