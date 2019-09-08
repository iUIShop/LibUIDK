#include "StdAfx.h"
#include "ChildWndHandler.h"

#ifdef _DEBUG
#define new	IUI_DEBUG_NEW
#endif // _DEBUG


IUI_BEGIN_MESSAGE_MAP(CChildWndHandler)
	IUI_ON_MESSAGE(WM_CREATE, &CChildWndHandler::OnCreate)
	IUI_ON_MESSAGE(WM_DESTROY, &CChildWndHandler::OnDestroy)
	IUI_ON_MESSAGE(WM_GETCHILDMSGHANDLER, &CChildWndHandler::OnGetChildMsgHandler)
	IUI_ON_MESSAGE(WM_RELEASECHILDMSGHANDLER, &CChildWndHandler::OnReleaseChildMsgHandler)
	IUI_ON_BN_CLICKED(IDC_BTN_CHILD, &CChildWndHandler::OnBtnChild)
IUI_END_MESSAGE_MAP()


CChildWndHandler::CChildWndHandler(UINT uWinID)
	: CUIWndHandler(uWinID)
{
}

CChildWndHandler::~CChildWndHandler()
{
}


//////////////////////////////////////////////////////////////////////////
// Messages

LRESULT CChildWndHandler::OnCreate(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CChildWndHandler::OnDestroy(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

// 为控件绑定的窗口指定消息映射类
LRESULT CChildWndHandler::OnGetChildMsgHandler(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam)
{
	UINT uWinID = (UINT)wParam;
	CUIWndHandler **ppHandler = (CUIWndHandler **)lParam;

	return 0;
}

// 释放控件绑定的窗口对应的消息映射类, 释放后返回TRUE，否则返回FALSE
LRESULT CChildWndHandler::OnReleaseChildMsgHandler(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam)
{
	UINT uWinID = (UINT)wParam;
	CHILDMSGHANDLER *pChildMsgHandler = (CHILDMSGHANDLER *)lParam;
	if (pChildMsgHandler == NULL)
	{
		return (LRESULT)FALSE;
	}

	return (LRESULT)FALSE;
}

LRESULT CChildWndHandler::OnBtnChild(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox(_T("IDC_BTN_CHILD"));

	return 0;
}
