#pragma once


class CChildWndHandler : public CUIWndHandler
{
public:
	CChildWndHandler(UINT uWinID);
	~CChildWndHandler();

protected:
	afx_msg LRESULT OnCreate(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDestroy(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetChildMsgHandler(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReleaseChildMsgHandler(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBtnChild(CUIWnd *pUIWnd, WPARAM wParam, LPARAM lParam);
	IUI_DECLARE_MESSAGE_MAP()
};
