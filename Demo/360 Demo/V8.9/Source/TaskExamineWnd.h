// TaskExamineWnd.h : interface of the CTaskExamineWnd class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKEXAMINEWND_H__6326E908_827B_4AC3_B184_FFF0A9996D6A__INCLUDED_)
#define AFX_TASKEXAMINEWND_H__6326E908_827B_4AC3_B184_FFF0A9996D6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTaskExamineWnd : public CUIWnd
{
public:
	CTaskExamineWnd();
	virtual ~CTaskExamineWnd();
	void WindowID() { IDD = IDW_EXAMINE; }

protected:
	int InitControls();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskExamineWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CTaskExamineWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnScan();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CSkinButton *m_pBtnScan;

public:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// 

#endif // !defined(AFX_TASKEXAMINEWND_H__6326E908_827B_4AC3_B184_FFF0A9996D6A__INCLUDED_)
