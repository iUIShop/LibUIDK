#if !defined(AFX_NEWPROJECTDLG_H__69F912DD_8319_407B_AE72_DD21B6A8BAF4__INCLUDED_)
#define AFX_NEWPROJECTDLG_H__69F912DD_8319_407B_AE72_DD21B6A8BAF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewProjectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewProjectDlg dialog

class CNewProjectDlg : public CDialog
{
// Construction
public:
	CNewProjectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewProjectDlg)
	enum { IDD = IDD_NEWPROJECT };
	BOOL	m_bActiveX;
	BOOL	m_bSockets;
	CString	m_strPrjName;
	CString	m_strLocation;
	//}}AFX_DATA
	BOOL	m_bDatabase;
	UINT	m_uProType;
	UINT	m_uDBID;
	UINT	m_uCommentID;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewProjectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewProjectDlg)
	virtual void OnOK();
	afx_msg void OnBtnBrowser();
	afx_msg void OnBnClickedBtnGoto();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPROJECTDLG_H__69F912DD_8319_407B_AE72_DD21B6A8BAF4__INCLUDED_)
