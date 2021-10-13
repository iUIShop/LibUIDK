// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__69BBC78E_4403_4464_90C1_0F728E1441D4__INCLUDED_)
#define AFX_MAINFRM_H__69BBC78E_4403_4464_90C1_0F728E1441D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ExamineFrame.h"
#include "TaskDSMainWnd.h"
#include "TaskWebWnd.h"


class CMainFrame : public CUIWnd
{
public:
	CMainFrame();
	virtual ~CMainFrame();
	void WindowID() { IDD = IDW_MAIN; }

protected:
	int InitControls();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnSkin();
	afx_msg void OnBtnMenu();
	afx_msg void OnBtnCheckUpdate();
	afx_msg void OnBtnLogo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CWLText *m_pWLTxtTitle;
	CSkinButton *m_pBtnSkin;
	CSkinButton *m_pBtnMenu;
	CSkinButton *m_pBtnMin;
	CSkinButton *m_pChkMax;
	CSkinButton *m_pBtnClose;
	CSkinButton *m_pRadExamine;
	CSkinButton *m_pRadDsmain;
	CSkinButton *m_pRadVulRepair;
	CSkinButton *m_pRadSysRepair;
	CSkinButton *m_pRadTraceCleaner;
	CSkinButton *m_pRadSpeedup;
	CSkinButton *m_pRadDnmz;
	CSkinButton *m_pRadSoftMgr;
	CSkinButton *m_pRadAdvTools;
	CWLText *m_pWLTxtVer;
	CSkinButton *m_pBtnCheckUpdate;
	CWLText *m_pWLTxtCloud;
	CSkinButton *m_pBtnLogo;
	CTaskWndMgr *m_pTWMgrMain;

public:
	BOOL m_bMax;

protected:
	CTaskWndMgr m_TaskWndMgr;
	CExamineFrame m_wndExamineFrame;
	CTaskDSMainWnd m_wndDSMain;
	CTaskWebWnd m_wndWeb;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__69BBC78E_4403_4464_90C1_0F728E1441D4__INCLUDED_)
