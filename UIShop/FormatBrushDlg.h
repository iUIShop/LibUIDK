#pragma once
#include "afxcmn.h"


// CFormatBrushDlg dialog

class CFormatBrushDlg : public CDialog
{
	DECLARE_DYNAMIC(CFormatBrushDlg)

public:
	CFormatBrushDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFormatBrushDlg();

// Dialog Data
	enum { IDD = IDD_FORMAT_BRUSH };

public:
	int SetCtrlProp(const CTRLPROPERTIES *pProp);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_lstFormatBrush;
	const CTRLPROPERTIES *m_pProp;
	afx_msg void OnBnClickedOk();
};
