// GroupPage.cpp : implementation of the CGroupPage class
//

#include "stdafx.h"
#include "$$root$$.h"
#include "GroupPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroupPage

BEGIN_MESSAGE_MAP(CGroupPage, CUIWnd)
	//{{AFX_MSG_MAP(CGroupPage)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupPage construction/destruction

CGroupPage::CGroupPage()
{
	m_pStaGroup = NULL;

}

CGroupPage::~CGroupPage()
{
}

void CGroupPage::DoDataExchange(CDataExchange* pDX)
{
	CUIWnd::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CGroupPage)
	//}}AFX_DATA_MAP
}


//////////////////////////////////////////////////////////////////////////
// protected

int CGroupPage::InitControls()
{
	m_pStaGroup = (CSkinStatic *)GetDlgItem(IDC_STA_GROUP);
	ASSERT(m_pStaGroup->GetSafeHwnd() != NULL);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CGroupPage message handlers

int CGroupPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (InitControls() != 0)
		return -1;


	return 0;
}
