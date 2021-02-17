// TaskExaminingWnd.h : interface of the CTaskExaminingWnd class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKEXAMININGWND_H__39894983_41BF_47F0_8C88_AFBE1E6278F9__INCLUDED_)
#define AFX_TASKEXAMININGWND_H__39894983_41BF_47F0_8C88_AFBE1E6278F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTaskExaminingWnd : public CUIWnd
{
public:
	CTaskExaminingWnd();
	virtual ~CTaskExaminingWnd();
	void WindowID() { IDD = IDW_EXAMINING; }

protected:
	int InitControls();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskExaminingWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CTaskExaminingWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CSkinButton *m_pBtnCancel;

public:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// 

#endif // !defined(AFX_TASKEXAMININGWND_H__39894983_41BF_47F0_8C88_AFBE1E6278F9__INCLUDED_)
