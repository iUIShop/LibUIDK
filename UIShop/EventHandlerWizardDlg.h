#pragma once


// CEventHandlerWizardDlg dialog

class CEventHandlerWizardDlg : public CDialog
{
	DECLARE_DYNAMIC(CEventHandlerWizardDlg)

public:
	CEventHandlerWizardDlg(CWnd *pParent = NULL);   // standard constructor
	virtual ~CEventHandlerWizardDlg();

	// Dialog Data
	enum { IDD = IDD_EVENT_HANDLER_WIZARD };
	CEdit m_edtClassName;
	CComboBox m_cmbMergeToolPath;
	CString m_strClassName;
	CString m_strWindowID;
	CString m_strHFile;
	CString m_strCppFile;
	CString m_strLocation;
	CString m_strMergeTool;

	UINT m_uCodeStyle;
	TCHAR m_szBrowseRoot[MAX_PATH];

protected:
	int SaveSettings();
	int Init3thPartyMergeTools();

protected:
	virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedBtnGoto();
	afx_msg void OnEnUpdateEdtClassName();
	afx_msg void OnBnClickedBtnBrowseMergeTool();
	afx_msg void OnBnClickedBtnMerge();
	DECLARE_MESSAGE_MAP()
};
