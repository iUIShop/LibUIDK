#include "afxwin.h"
#if !defined(AFX_GENERATERESOURCEDLG_H__3E46741D_4C4F_44DE_AB3A_9B23FB6FADB9__INCLUDED_)
#define AFX_GENERATERESOURCEDLG_H__3E46741D_4C4F_44DE_AB3A_9B23FB6FADB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenerateResourceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGenerateResourceDlg dialog

class CGenerateResourceDlg : public CDialog
{
// Construction
public:
	CGenerateResourceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGenerateResourceDlg)
	enum { IDD = IDD_GENERATE_RESOURCE };
	CEdit m_edtSkinPath;
	CEdit m_edtSavePath;
	CString m_strSavePath;
	CComboBox m_cmbLanguage;
	CButton m_radRelateSavePath;
	CButton m_radRelateCustom;
	CString m_strRelateBaseOnSavePath;
	CEdit m_edtRelateCustom;
	CString m_strRelateCustom;
	//}}AFX_DATA

protected:
	int UpdateRelativePath();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenerateResourceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGenerateResourceDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtnBrowse();
	afx_msg void OnRadRelateSavePath();
	afx_msg void OnRadRelateCustom();
	afx_msg void OnEnChangeEdtSavePath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nLanguage;	// 0: Englist (United States); 1: Chinese (PRC).
	BOOL m_bRelateSavePath;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERATERESOURCEDLG_H__3E46741D_4C4F_44DE_AB3A_9B23FB6FADB9__INCLUDED_)
