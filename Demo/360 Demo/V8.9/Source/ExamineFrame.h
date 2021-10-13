// ExamineFrame.h : interface of the CExamineFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXAMINEFRAME_H__72DCAC07_34D6_48EA_A26F_E2B3B0E87446__INCLUDED_)
#define AFX_EXAMINEFRAME_H__72DCAC07_34D6_48EA_A26F_E2B3B0E87446__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "TaskExamineWnd.h"
#include "TaskExaminingWnd.h"


class CExamineFrame : public CUIWnd
{
public:
	CExamineFrame();
	virtual ~CExamineFrame();
	void WindowID() { IDD = IDW_EXAMINE_FRM; }

protected:
	int InitControls();

public:
	int SwitchTaskWnd(int nWndID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExamineFrame)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CExamineFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CTaskWndMgr *m_pTWMgrExamine;

protected:
	CTaskWndMgr m_TaskWndMgr;
	CTaskExamineWnd m_wndExamine;
	CTaskExaminingWnd m_wndExamining;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// 

#endif // !defined(AFX_EXAMINEFRAME_H__72DCAC07_34D6_48EA_A26F_E2B3B0E87446__INCLUDED_)
