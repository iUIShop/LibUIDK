#pragma once
#include "afxcmn.h"


enum IMAGE_SYMBOLS_DLG_MODE
{
	ISDM_UNUSED,
	ISDM_UNFOUND
};

// CImageSymbolsDlg dialog

class CImageSymbolsDlg : public CDialog
{
	DECLARE_DYNAMIC(CImageSymbolsDlg)

public:
	CImageSymbolsDlg(IMAGE_SYMBOLS_DLG_MODE eMode, CWnd* pParent = NULL);   // standard constructor
	virtual ~CImageSymbolsDlg();

// Dialog Data
	enum { IDD = IDD_IMAGE_SYMBOLS };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedBtnMoveToUnused();
	DECLARE_MESSAGE_MAP()

public:
	IMAGE_SYMBOLS_DLG_MODE m_eMode;
	CTreeCtrl m_treeImageSymbols;
	CButton m_btnMoveToUnused;

protected:
	std::vector<CString> m_vFiles;
};
