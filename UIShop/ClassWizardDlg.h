#pragma once
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CClassWizardDlg dialog

class CClassWizardDlg : public CDialog
{
    // Construction
public:
    CClassWizardDlg(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CClassWizardDlg)
    enum { IDD = IDD_CLASS_WIZARD };
    CEdit m_edtClassName;
    CEdit m_cmbMergeToolPath;
    CString m_strClassName;
    CString m_strWindowID;
    CString m_strHFile;
    CString m_strCppFile;
    CString m_strLocation;
    CString m_strMergeTool;
    BOOL m_bGenEventHandler;
    BOOL m_bGenVariable;
    BOOL m_bGenDoDataExchange;
    //}}AFX_DATA
    UINT m_uCodeStyle;
    TCHAR m_szBrowseRoot[MAX_PATH];

protected:
    int SaveSettings();

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CClassWizardDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CClassWizardDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnBnClickedBtnBrowse();
    afx_msg void OnBnClickedBtnGoto();
    afx_msg void OnEnUpdateEdtClassName();
    afx_msg void OnChkGenerateHandler();
    afx_msg void OnBnClickedBtnBrowseMergeTool();
    afx_msg void OnBnClickedBtnMerge();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
