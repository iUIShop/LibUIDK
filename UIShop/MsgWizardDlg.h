#pragma once
#include "afxcmn.h"


// CMsgWizardDlg dialog

class CMsgWizardDlg : public CDialog
{
	DECLARE_DYNAMIC(CMsgWizardDlg)

public:
	CMsgWizardDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMsgWizardDlg();

// Dialog Data
	enum { IDD = IDD_ADD_HANDLER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnTvnItemChangedTreeControls(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

public:
	CTreeCtrl m_treeMessages;
	CRichEditCtrl m_reCode;
};
