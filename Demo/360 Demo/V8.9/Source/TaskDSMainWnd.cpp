// TaskDSMainWnd.cpp : implementation of the CTaskDSMainWnd class
//

#include "stdafx.h"
#include "TaskDSMainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaskDSMainWnd

BEGIN_MESSAGE_MAP(CTaskDSMainWnd, CUIWnd)
	//{{AFX_MSG_MAP(CTaskDSMainWnd)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_1, OnBtn1)
	ON_BN_CLICKED(IDC_BTN_2, OnBtn2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskDSMainWnd construction/destruction

CTaskDSMainWnd::CTaskDSMainWnd()
{
	m_pBtn1 = NULL;
	m_pBtn2 = NULL;

}

CTaskDSMainWnd::~CTaskDSMainWnd()
{
}

void CTaskDSMainWnd::DoDataExchange(CDataExchange* pDX)
{
	CUIWnd::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CTaskDSMainWnd)
	//}}AFX_DATA_MAP
}


//////////////////////////////////////////////////////////////////////////
// protected

int CTaskDSMainWnd::InitControls()
{
	m_pBtn1 = (CSkinButton *)GetDlgItem(IDC_BTN_1);
	ASSERT(m_pBtn1->GetSafeHwnd() != NULL);
	m_pBtn2 = (CSkinButton *)GetDlgItem(IDC_BTN_2);
	ASSERT(m_pBtn2->GetSafeHwnd() != NULL);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CTaskDSMainWnd message handlers

int CTaskDSMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (InitControls() != 0)
		return -1;


	return 0;
}

void CTaskDSMainWnd::OnBtn1()
{
	AfxMessageBox(_T("IDC_BTN_1"));
}

void CTaskDSMainWnd::OnBtn2()
{
	AfxMessageBox(_T("IDC_BTN_2"));
}
