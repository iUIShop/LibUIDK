// LoginDlg.h : interface of the CLoginDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGINDLG_H__8B1F6E08_367F_4005_9EEF_D8BD7C9D0452__INCLUDED_)
#define AFX_LOGINDLG_H__8B1F6E08_367F_4005_9EEF_D8BD7C9D0452__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLoginDlg : public CUIWnd
{
public:
	CLoginDlg();
	virtual ~CLoginDlg();
	void WindowID() { IDD = IDW_LOGIN; }

protected:
	int InitControls();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CLoginDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnLogin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CSkinButton *m_pBtnMin;
	CSkinButton *m_pBtnClose;
	CWLText *m_pWLTxtPassword;
	CSkinEdit *m_pEdtName;
	CWLText *m_pWLTxtName;
	CSkinEdit *m_pEdtPassword;
	CSkinButton *m_pBtnLogin;

public:
	CString m_strName;
	CString m_strPassword;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// 

#endif // !defined(AFX_LOGINDLG_H__8B1F6E08_367F_4005_9EEF_D8BD7C9D0452__INCLUDED_)
