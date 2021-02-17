// TaskExaminingWnd.cpp : implementation of the CTaskExaminingWnd class
//

#include "stdafx.h"
#include "TaskExaminingWnd.h"
#include "ExamineFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaskExaminingWnd

BEGIN_MESSAGE_MAP(CTaskExaminingWnd, CUIWnd)
	//{{AFX_MSG_MAP(CTaskExaminingWnd)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskExaminingWnd construction/destruction

CTaskExaminingWnd::CTaskExaminingWnd()
{
	m_pBtnCancel = NULL;

}

CTaskExaminingWnd::~CTaskExaminingWnd()
{
}

void CTaskExaminingWnd::DoDataExchange(CDataExchange* pDX)
{
	CUIWnd::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CTaskExaminingWnd)
	//}}AFX_DATA_MAP
}


//////////////////////////////////////////////////////////////////////////
// protected

int CTaskExaminingWnd::InitControls()
{
	m_pBtnCancel = (CSkinButton *)GetDlgItem(IDC_BTN_CANCEL);
	ASSERT(m_pBtnCancel->GetSafeHwnd() != NULL);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CTaskExaminingWnd message handlers

int CTaskExaminingWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (InitControls() != 0)
		return -1;


	return 0;
}

void CTaskExaminingWnd::OnBtnCancel()
{
	CExamineFrame *pParent = (CExamineFrame *)GetParent();
	pParent->SwitchTaskWnd(IDW_EXAMINE);
}
