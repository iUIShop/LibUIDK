#pragma once


// CGenClassInfoFileDlg dialog

class CGenClassInfoFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CGenClassInfoFileDlg)

protected:
	int ParseHFile(const CString &strHFile, CString &strWindowID, CString &strClassName, BOOL *pbCodeStyleVC6);

public:
	CGenClassInfoFileDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGenClassInfoFileDlg();
	CString m_strCodeFolder;
	CString m_strIniFile;
	TCHAR m_szBrowseRoot[MAX_PATH];

// Dialog Data
	enum { IDD = IDD_GENERATE_CLASS_INFO_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnBrowseCodeFolder();
	afx_msg void OnBnClickedBtnBrowseClassInfoFile();
};
