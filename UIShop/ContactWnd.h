#pragma once


// CContactWnd

class CContactWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CContactWnd)

public:
	CContactWnd();
	virtual ~CContactWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
	CHtmlCtrl m_ctrlHtml;
};
