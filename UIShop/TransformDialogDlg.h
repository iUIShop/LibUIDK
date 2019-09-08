#pragma once


// CTransformDialogDlg dialog

class CTransformDialogDlg : public CDialog
{
	DECLARE_DYNAMIC(CTransformDialogDlg)

public:
	CTransformDialogDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTransformDialogDlg();

// Dialog Data
	enum { IDD = IDD_TRANSFORM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnBnClickedBtnBrowseRes();
	afx_msg void OnBnClickedBtnGoSource();
	afx_msg void OnBnClickedBtnBrowseXui();
	afx_msg void OnBnClickedBtnGoTarget();
	afx_msg void OnBnClickedBtnTransform();
	DECLARE_MESSAGE_MAP()

public:
	CString m_strResSource;
	CString m_strXUITarget;
};
