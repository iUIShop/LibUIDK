// TaskExamineWnd.cpp : implementation of the CTaskExamineWnd class
//

#include "stdafx.h"
#include "TaskExamineWnd.h"
#include "ExamineFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaskExamineWnd

BEGIN_MESSAGE_MAP(CTaskExamineWnd, CUIWnd)
	//{{AFX_MSG_MAP(CTaskExamineWnd)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_SCAN, OnBtnScan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskExamineWnd construction/destruction

CTaskExamineWnd::CTaskExamineWnd()
{
	m_pBtnScan = NULL;

}

CTaskExamineWnd::~CTaskExamineWnd()
{
}

void CTaskExamineWnd::DoDataExchange(CDataExchange* pDX)
{
	CUIWnd::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CTaskExamineWnd)
	//}}AFX_DATA_MAP
}


//////////////////////////////////////////////////////////////////////////
// protected

int CTaskExamineWnd::InitControls()
{
	m_pBtnScan = (CSkinButton *)GetDlgItem(IDC_BTN_SCAN);
	ASSERT(m_pBtnScan->GetSafeHwnd() != NULL);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CTaskExamineWnd message handlers

int CTaskExamineWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (InitControls() != 0)
		return -1;


	return 0;
}

void CTaskExamineWnd::OnBtnScan()
{
	CExamineFrame *pParent = (CExamineFrame *)GetParent();
	pParent->SwitchTaskWnd(IDW_EXAMINING);
}
