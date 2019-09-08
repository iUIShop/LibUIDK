#pragma once


// CSyncSkinTabOrderDlg dialog

class CSyncSkinTabOrderDlg : public CDialog
{
	DECLARE_DYNAMIC(CSyncSkinTabOrderDlg)

protected:
	int SyncSkinTabOrder(const CString &strSkinSource, const CString &strSkinTarget);

public:
	CSyncSkinTabOrderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSyncSkinTabOrderDlg();

// Dialog Data
	enum { IDD = IDD_SYNC_SKIN_TAB_ORDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnBrowseSource();
	afx_msg void OnBnClickedBtnBrowseTarget();
	afx_msg void OnBnClickedOk();

public:
	CString m_strSkinSource;
	CString m_strSkinTarget;
};
