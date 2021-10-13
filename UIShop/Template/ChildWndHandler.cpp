// ChildWnd.cpp : implementation of the CChildWnd class
//

#include "stdafx.h"
#include "ChildWnd.h"

#ifdef _DEBUG
#define new IUI_DEBUG_NEW
#endif // _DEBUG

// CChildWnd

BEGIN_MESSAGE_MAP(CChildWnd, CUIWnd)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_CHILD, OnBtnChild)
END_MESSAGE_MAP()

// CChildWnd construction/destruction

CChildWnd::CChildWnd()
{
	m_pBtnChild = NULL;

}

CChildWnd::~CChildWnd()
{
}

void CChildWnd::DoDataExchange(CDataExchange* pDX)
{
	CUIWnd::DoDataExchange(pDX);

}


//////////////////////////////////////////////////////////////////////////
// protected

int CChildWnd::InitControls()
{
	m_pBtnChild = (CSkinButton *)GetDlgItem(IDC_BTN_CHILD);
	ASSERT(m_pBtnChild->GetSafeHwnd() != NULL);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CChildWnd message handlers

int CChildWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (InitControls() != 0)
		return -1;


	return 0;
}

void CChildWnd::OnBtnChild()
{
	AfxMessageBox(_T("IDC_BTN_CHILD"));
}
