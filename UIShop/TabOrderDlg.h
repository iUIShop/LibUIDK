#if !defined(AFX_TABORDERDLG_H__3E46741D_4C4F_44DE_AB3A_9B23FB6FADB9__INCLUDED_)
#define AFX_TABORDERDLG_H__3E46741D_4C4F_44DE_AB3A_9B23FB6FADB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabOrderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabOrderDlg dialog

class CTabOrderDlg : public CDialog
{
// Construction
public:
	void UpdateControlsList();
	CWnd * SetBuddy(CWnd *pWndBuddy);
	CTabOrderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabOrderDlg)
	enum { IDD = IDD_TABORDER };
	CListCtrl	m_ctrlControlList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabOrderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabOrderDlg)
	afx_msg void OnBtnMoveUp();
	afx_msg void OnBtnMoveDown();
	afx_msg void OnBnClickedBtnAutoAdjust();
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedLstTabOrder(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnBnClickedBtnHighlight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CWnd *m_pWndBuddy;

	BOOL m_bHandleNotify;		// FALSE: don't handle LVN_ITEMCHANGED message
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABORDERDLG_H__3E46741D_4C4F_44DE_AB3A_9B23FB6FADB9__INCLUDED_)
