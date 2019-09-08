#pragma once


#include "StoryboardSldCtrl.h"
#include "UIDesignWnd.h"

// CStoryboardPane

class CStoryboardPane : public CDockablePane
{
	DECLARE_DYNAMIC(CStoryboardPane)

public:
	CStoryboardPane();
	virtual ~CStoryboardPane();

public:
	int SetBindUIWnd(CUIDesignWnd *pUIDesignWnd);
	int GetCurFrame();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()

protected:
	CStoryboardSldCtrl m_sldTimeline;
	CUIDesignWnd *m_pUIDesignWnd;
};
