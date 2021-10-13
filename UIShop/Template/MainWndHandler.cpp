// MainWnd.cpp : implementation of the CMainWnd class
//

#include "stdafx.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new	IUI_DEBUG_NEW
#endif // _DEBUG

// CMainWnd

BEGIN_MESSAGE_MAP(CMainWnd, CUIWnd)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_MIN, OnBtnMin)
	ON_BN_CLICKED(IDC_CHK_MAX, OnChkMax)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
END_MESSAGE_MAP()

// CMainWnd construction/destruction

CMainWnd::CMainWnd()
{
	m_pStaTitle = NULL;
	m_pBtnMin = NULL;
	m_pChkMax = NULL;
	m_pBtnClose = NULL;
	m_pStaMsg = NULL;
	m_pBtnOk = NULL;
	m_pRcChild = NULL;

	m_bMax = FALSE;
}

CMainWnd::~CMainWnd()
{
}

void CMainWnd::DoDataExchange(CDataExchange* pDX)
{
	CUIWnd::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHK_MAX, m_bMax);
}


//////////////////////////////////////////////////////////////////////////
// protected

int CMainWnd::InitControls()
{
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
	m_pRcChild = GetRectChild(IDC_WL_RECT_CHILD);
	ASSERT(m_pRcChild->IsCreated());

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (InitControls() != 0)
		return -1;

	m_wndChild.Create(NULL, WS_VISIBLE, m_pRcChild, this, IDW_CHILD);

	return 0;
}

void CMainWnd::OnBtnMin()
{
}

void CMainWnd::OnChkMax()
{
}

void CMainWnd::OnBtnClose()
{
}

void CMainWnd::OnBtnOk()
{
	AfxMessageBox(_T("IDC_BTN_OK"));
}
