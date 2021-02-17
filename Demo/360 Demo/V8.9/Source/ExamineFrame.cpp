// ExamineFrame.cpp : implementation of the CExamineFrame class
//

#include "stdafx.h"
#include "ExamineFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExamineFrame

BEGIN_MESSAGE_MAP(CExamineFrame, CUIWnd)
	//{{AFX_MSG_MAP(CExamineFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExamineFrame construction/destruction

CExamineFrame::CExamineFrame()
{
	m_pTWMgrExamine = NULL;

}

CExamineFrame::~CExamineFrame()
{
}

void CExamineFrame::DoDataExchange(CDataExchange* pDX)
{
	CUIWnd::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CExamineFrame)
	//}}AFX_DATA_MAP
}


//////////////////////////////////////////////////////////////////////////
// protected

int CExamineFrame::InitControls()
{
	m_pTWMgrExamine = (CTaskWndMgr *)GetDlgWLItem(IDC_TWMGR_EXAMINE);
	ASSERT(m_pTWMgrExamine != NULL);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

int CExamineFrame::SwitchTaskWnd(int nWndID)
{
	return m_TaskWndMgr.SwitchToTaskWindow(nWndID);
}

/////////////////////////////////////////////////////////////////////////////
// CExamineFrame message handlers

int CExamineFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (InitControls() != 0)
		return -1;

	m_pTWMgrExamine->AddTaskWindow(IDW_EXAMINE, FALSE, &m_wndExamine, IDW_EXAMINE, _T("Examine"));
	m_pTWMgrExamine->AddTaskWindow(IDW_EXAMINING, FALSE, &m_wndExamining, IDW_EXAMINING, _T("Examining"));
	m_pTWMgrExamine->SwitchToTaskWindow(IDW_EXAMINE);

	return 0;
}
