// TaskDSMainWnd.h : interface of the CTaskDSMainWnd class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKDSMAINWND_H__5E1A2AF6_0462_4874_98B6_D2574778D5EA__INCLUDED_)
#define AFX_TASKDSMAINWND_H__5E1A2AF6_0462_4874_98B6_D2574778D5EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTaskDSMainWnd : public CUIWnd
{
public:
	CTaskDSMainWnd();
	virtual ~CTaskDSMainWnd();
	void WindowID() { IDD = IDW_DSMAIN; }

protected:
	int InitControls();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskDSMainWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CTaskDSMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtn1();
	afx_msg void OnBtn2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CSkinButton *m_pBtn1;
	CSkinButton *m_pBtn2;

public:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// 

#endif // !defined(AFX_TASKDSMAINWND_H__5E1A2AF6_0462_4874_98B6_D2574778D5EA__INCLUDED_)
