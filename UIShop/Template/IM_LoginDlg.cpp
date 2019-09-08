// LoginDlg.cpp : implementation of the CLoginDlg class
//

#include "stdafx.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLoginDlg

BEGIN_MESSAGE_MAP(CLoginDlg, CUIWnd)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_CHK_REMEMBER_PASSWORD, OnChkRememberPassword)
	ON_BN_CLICKED(IDC_CHK_AUTO_LOGIN, OnChkAutoLogin)
	ON_BN_CLICKED(IDC_BTN_LOGIN, OnBtnLogin)
	ON_BN_CLICKED(IDC_BTN_FORGET_PASSWORD, OnBtnForgetPassword)
	ON_BN_CLICKED(IDC_BTN_REGISTER, OnBtnRegister)
END_MESSAGE_MAP()

// CLoginDlg construction/destruction

CLoginDlg::CLoginDlg()
{
	m_pBtnMin = NULL;
	m_pBtnClose = NULL;
	m_pEdtName = NULL;
	m_pEdtPassword = NULL;
	m_pChkRememberPassword = NULL;
	m_pChkAutoLogin = NULL;
	m_pBtnLogin = NULL;
	m_pBtnForgetPassword = NULL;
	m_pBtnRegister = NULL;

	m_strName = _T("");
	m_strPassword = _T("");
	m_bRememberPassword = FALSE;
	m_bAutoLogin = FALSE;
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CUIWnd::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDT_PASSWORD, m_strPassword);
	DDX_Check(pDX, IDC_CHK_REMEMBER_PASSWORD, m_bRememberPassword);
	DDX_Check(pDX, IDC_CHK_AUTO_LOGIN, m_bAutoLogin);
}


//////////////////////////////////////////////////////////////////////////
// protected

int CLoginDlg::InitControls()
{
	m_pBtnMin = (CSkinButton *)GetDlgItem(IDC_BTN_MIN);
	ASSERT(m_pBtnMin->GetSafeHwnd() != NULL);
	m_pBtnClose = (CSkinButton *)GetDlgItem(IDC_BTN_CLOSE);
	ASSERT(m_pBtnClose->GetSafeHwnd() != NULL);
	m_pEdtName = (CSkinEdit *)GetDlgItem(IDC_EDT_NAME);
	ASSERT(m_pEdtName->GetSafeHwnd() != NULL);
	m_pEdtPassword = (CSkinEdit *)GetDlgItem(IDC_EDT_PASSWORD);
	ASSERT(m_pEdtPassword->GetSafeHwnd() != NULL);
	m_pChkRememberPassword = (CSkinButton *)GetDlgItem(IDC_CHK_REMEMBER_PASSWORD);
	ASSERT(m_pChkRememberPassword->GetSafeHwnd() != NULL);
	m_pChkAutoLogin = (CSkinButton *)GetDlgItem(IDC_CHK_AUTO_LOGIN);
	ASSERT(m_pChkAutoLogin->GetSafeHwnd() != NULL);
	m_pBtnLogin = (CSkinButton *)GetDlgItem(IDC_BTN_LOGIN);
	ASSERT(m_pBtnLogin->GetSafeHwnd() != NULL);
	m_pBtnForgetPassword = (CSkinButton *)GetDlgItem(IDC_BTN_FORGET_PASSWORD);
	ASSERT(m_pBtnForgetPassword->GetSafeHwnd() != NULL);
	m_pBtnRegister = (CSkinButton *)GetDlgItem(IDC_BTN_REGISTER);
	ASSERT(m_pBtnRegister->GetSafeHwnd() != NULL);

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

void CLoginDlg::OnBtnClose()
{
	EndDialog(IDC_BTN_CLOSE);
}

void CLoginDlg::OnChkRememberPassword()
{
	AfxMessageBox(_T("IDC_CHK_REMEMBER_PASSWORD"));
}

void CLoginDlg::OnChkAutoLogin()
{
	AfxMessageBox(_T("IDC_CHK_AUTO_LOGIN"));
}

void CLoginDlg::OnBtnLogin()
{
	EndDialog(IDC_BTN_LOGIN);
}

void CLoginDlg::OnBtnForgetPassword()
{
	AfxMessageBox(_T("IDC_BTN_FORGET_PASSWORD"));
}

void CLoginDlg::OnBtnRegister()
{
	AfxMessageBox(_T("IDC_BTN_REGISTER"));
}
