// TaskWebWnd.cpp : implementation of the CTaskWebWnd class
//

#include "stdafx.h"
#include "TaskWebWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaskWebWnd

BEGIN_MESSAGE_MAP(CTaskWebWnd, CUIWnd)
	//{{AFX_MSG_MAP(CTaskWebWnd)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskWebWnd construction/destruction

CTaskWebWnd::CTaskWebWnd()
{
	m_pHtml1 = NULL;

}

CTaskWebWnd::~CTaskWebWnd()
{
}

void CTaskWebWnd::DoDataExchange(CDataExchange* pDX)
{
	CUIWnd::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CTaskWebWnd)
	//}}AFX_DATA_MAP
}


//////////////////////////////////////////////////////////////////////////
// protected

int CTaskWebWnd::InitControls()
{
	m_pHtml1 = (CHtmlCtrl *)GetDlgItem(IDC_HTMLCTRL_1);
	ASSERT(m_pHtml1->GetSafeHwnd() != NULL);

	return 0;
}

int CTaskWebWnd::InitHtml1()
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CTaskWebWnd message handlers

int CTaskWebWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (InitControls() != 0)
		return -1;

	InitHtml1();

	return 0;
}
