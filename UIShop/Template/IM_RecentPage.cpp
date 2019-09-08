// RecentPage.cpp : implementation of the CRecentPage class
//

#include "stdafx.h"
#include "$$root$$.h"
#include "RecentPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecentPage

BEGIN_MESSAGE_MAP(CRecentPage, CUIWnd)
	//{{AFX_MSG_MAP(CRecentPage)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecentPage construction/destruction

CRecentPage::CRecentPage()
{
	m_pStaRecent = NULL;

}

CRecentPage::~CRecentPage()
{
}

void CRecentPage::DoDataExchange(CDataExchange* pDX)
{
	CUIWnd::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CRecentPage)
	//}}AFX_DATA_MAP
}


//////////////////////////////////////////////////////////////////////////
// protected

int CRecentPage::InitControls()
{
	m_pStaRecent = (CSkinStatic *)GetDlgItem(IDC_STA_RECENT);
	ASSERT(m_pStaRecent->GetSafeHwnd() != NULL);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CRecentPage message handlers

int CRecentPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (InitControls() != 0)
		return -1;


	return 0;
}
