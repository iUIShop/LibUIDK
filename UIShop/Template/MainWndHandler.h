// MainWnd.h : interface of the CMainWnd class
//
#pragma once

#include "ChildWnd.h"


class CMainWnd : public CUIWnd
{
public:
	CMainWnd();
	virtual ~CMainWnd();
	void WindowID() { IDD = IDW_MAIN; }

protected:
	int InitControls();

public:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnMin();
	afx_msg void OnChkMax();
	afx_msg void OnBtnClose();
	afx_msg void OnBtnOk();
	DECLARE_MESSAGE_MAP()

protected:
	CSkinStatic *m_pStaTitle;
	CSkinButton *m_pBtnMin;
	CSkinButton *m_pChkMax;
	CSkinButton *m_pBtnClose;
	CSkinStatic *m_pStaMsg;
	CSkinButton *m_pBtnOk;
	CRectCtrl *m_pRcChild;

public:
	BOOL m_bMax;

protected:
	CChildWnd m_wndChild;
};
