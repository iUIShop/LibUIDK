#pragma once


// CRenameTemplateFileDlg dialog

class CRenameTemplateFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CRenameTemplateFileDlg)

public:
	CRenameTemplateFileDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRenameTemplateFileDlg();

// Dialog Data
	enum { IDD = IDD_TEMPLATE_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTemplatePath;
	CString m_strPrefix;
	CString m_strTargetPath;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnBrowseTemplate();
	afx_msg void OnBnClickedBtnBrowseTarget();
};
