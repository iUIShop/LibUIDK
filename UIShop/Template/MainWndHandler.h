#pragma once


class CMainWndHandler : public CUIWndHandler
{
public:
	CMainWndHandler(UINT uWinID);
	virtual ~CMainWndHandler();

protected:
	afx_msg LRESULT OnCreate(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDestroy(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetChildMsgHandler(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReleaseChildMsgHandler(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBtnOk(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam);
	IUI_DECLARE_MESSAGE_MAP()
};
