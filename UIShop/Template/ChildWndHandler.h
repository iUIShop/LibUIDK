// ChildWnd.h : interface of the CChildWnd class
//
#pragma once

class CChildWnd : public CUIWnd
{
public:
	CChildWnd();
	virtual ~CChildWnd();
	void WindowID() { IDD = IDW_CHILD; }

protected:
	int InitControls();

public:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnChild();
	DECLARE_MESSAGE_MAP()

protected:
	CSkinButton *m_pBtnChild;

public:
};
