// LoginDlg.cpp : implementation of the CLoginDlg class
//

#include "stdafx.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg

BEGIN_MESSAGE_MAP(CLoginDlg, CUIWnd)
	//{{AFX_MSG_MAP(CLoginDlg)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_LOGIN, OnBtnLogin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg construction/destruction

CLoginDlg::CLoginDlg()
{
	m_pBtnMin = NULL;
	m_pBtnClose = NULL;
	m_pWLTxtPassword = NULL;
	m_pEdtName = NULL;
	m_pWLTxtName = NULL;
	m_pEdtPassword = NULL;
	m_pBtnLogin = NULL;

	m_strName = _T("");
	m_strPassword = _T("");
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CUIWnd::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_Text(pDX, IDC_EDT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDT_PASSWORD, m_strPassword);
	//}}AFX_DATA_MAP
}


//////////////////////////////////////////////////////////////////////////
// protected

int CLoginDlg::InitControls()
{
	m_pBtnMin = (CSkinButton *)GetDlgItem(IDC_BTN_MIN);
	ASSERT(m_pBtnMin->GetSafeHwnd() != NULL);
	m_pBtnClose = (CSkinButton *)GetDlgItem(IDC_BTN_CLOSE);
	ASSERT(m_pBtnClose->GetSafeHwnd() != NULL);
	m_pWLTxtPassword = (CWLText *)GetDlgWLItem(IDC_WL_TXT_PASSWORD);
	ASSERT(m_pWLTxtPassword != NULL);
	m_pEdtName = (CSkinEdit *)GetDlgItem(IDC_EDT_NAME);
	ASSERT(m_pEdtName->GetSafeHwnd() != NULL);
	m_pWLTxtName = (CWLText *)GetDlgWLItem(IDC_WL_TXT_NAME);
	ASSERT(m_pWLTxtName != NULL);
	m_pEdtPassword = (CSkinEdit *)GetDlgItem(IDC_EDT_PASSWORD);
	ASSERT(m_pEdtPassword->GetSafeHwnd() != NULL);
	m_pBtnLogin = (CSkinButton *)GetDlgItem(IDC_BTN_LOGIN);
	ASSERT(m_pBtnLogin->GetSafeHwnd() != NULL);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

int CLoginDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (InitControls() != 0)
		return -1;


	return 0;
}

void CLoginDlg::OnBtnLogin()
{
	EndDialog(IDC_BTN_LOGIN);
}
