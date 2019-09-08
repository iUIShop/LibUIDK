// ContactWnd.cpp : implementation file
//

#include "stdafx.h"
#include "UIShop.h"
#include "ContactWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CContactWnd

IMPLEMENT_DYNAMIC(CContactWnd, CDockablePane)

CContactWnd::CContactWnd()
{

}

CContactWnd::~CContactWnd()
{
}


BEGIN_MESSAGE_MAP(CContactWnd, CDockablePane)
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CContactWnd message handlers

int CContactWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

#ifndef _DEBUG
	m_ctrlHtml.Create(NULL, _T("ContactView"), WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, 1234);
	m_ctrlHtml.Navigate2(_T("http://www.iuishop.com/chat/chat.html"), NULL, NULL);
#endif

	return 0;
}

void CContactWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
}

void CContactWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if (m_ctrlHtml.GetSafeHwnd() != NULL)
	{
		m_ctrlHtml.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE);
	}
}
