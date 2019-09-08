// LoginDlg.h : interface of the CLoginDlg class
//
#pragma once

class CLoginDlg : public CUIWnd
{
public:
	CLoginDlg();
	virtual ~CLoginDlg();
	void WindowID() { IDD = IDW_LOGIN; }

protected:
	int InitControls();

public:

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnClose();
	afx_msg void OnChkRememberPassword();
	afx_msg void OnChkAutoLogin();
	afx_msg void OnBtnLogin();
	afx_msg void OnBtnForgetPassword();
	afx_msg void OnBtnRegister();
	DECLARE_MESSAGE_MAP()

protected:
	CSkinButton *m_pBtnMin;
	CSkinButton *m_pBtnClose;
	CSkinEdit *m_pEdtName;
	CSkinEdit *m_pEdtPassword;
	CSkinButton *m_pChkRememberPassword;
	CSkinButton *m_pChkAutoLogin;
	CSkinButton *m_pBtnLogin;
	CSkinButton *m_pBtnForgetPassword;
	CSkinButton *m_pBtnRegister;

public:
	CString m_strName;
	CString m_strPassword;
	BOOL m_bRememberPassword;
	BOOL m_bAutoLogin;
};
