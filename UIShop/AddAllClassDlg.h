#pragma once


// CAddAllClassDlg dialog

class CAddAllClassDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddAllClassDlg)

public:
	CString m_strLocation;
	TCHAR m_szBrowseRoot[MAX_PATH];

public:
	CAddAllClassDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddAllClassDlg();

// Dialog Data
	enum { IDD = IDD_ADD_ALL_CLASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedOk();
};
