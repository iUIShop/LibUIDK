#include "StdAfx.h"
#include "MainWndHandler.h"
#include "ChildWndHandler.h"

#ifdef _DEBUG
#define new	IUI_DEBUG_NEW
#endif // _DEBUG


IUI_BEGIN_MESSAGE_MAP(CMainWndHandler)
	IUI_ON_MESSAGE(WM_CREATE, &CMainWndHandler::OnCreate)
	IUI_ON_MESSAGE(WM_DESTROY, &CMainWndHandler::OnDestroy)
	IUI_ON_MESSAGE(WM_GETCHILDMSGHANDLER, &CMainWndHandler::OnGetChildMsgHandler)
	IUI_ON_MESSAGE(WM_RELEASECHILDMSGHANDLER, &CMainWndHandler::OnReleaseChildMsgHandler)
	IUI_ON_BN_CLICKED(IDC_BTN_OK, &CMainWndHandler::OnBtnOk)
IUI_END_MESSAGE_MAP()


CMainWndHandler::CMainWndHandler(UINT uWinID)
	: CUIWndHandler(uWinID)
{

}

CMainWndHandler::~CMainWndHandler()
{

}

//////////////////////////////////////////////////////////////////////////
// Messages

LRESULT CMainWndHandler::OnCreate(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CMainWndHandler::OnDestroy(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

// 为控件绑定的窗口指定消息映射类
LRESULT CMainWndHandler::OnGetChildMsgHandler(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam)
{
	UINT uWinID = (UINT)wParam;
	CHILDMSGHANDLER *pChildMsgHandler = (CHILDMSGHANDLER *)lParam;
	if (pChildMsgHandler == NULL)
	{
		return 0;
	}

	if (uWinID == IDW_CHILD)
	{
		pChildMsgHandler->pUIWndHandler = new CChildWndHandler(uWinID);
	}

	return 0;
}

// 释放控件绑定的窗口对应的消息映射类, 释放后返回TRUE，否则返回FALSE
LRESULT CMainWndHandler::OnReleaseChildMsgHandler(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam)
{
	UINT uWinID = (UINT)wParam;
	CHILDMSGHANDLER *pChildMsgHandler = (CHILDMSGHANDLER *)lParam;
	if (pChildMsgHandler == NULL)
	{
		return (LRESULT)FALSE;
	}

	if (uWinID == IDW_CHILD)
	{
		ASSERT(pChildMsgHandler->pUIWndHandler != NULL);
		SafeDelete(pChildMsgHandler->pUIWndHandler);

		return (LRESULT)TRUE;
	}

	return (LRESULT)FALSE;
}

LRESULT CMainWndHandler::OnBtnOk(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam)
{
	CWnd *pWnd = pUIWnd->GetDlgItem(IDC_STA_MSG);

	AfxMessageBox(_T("IDC_BTN_OK"));

	pWnd->SetWindowText(_T("点击按钮后，文本就成这样了。"));

	return 0;
}
