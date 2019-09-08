#pragma once


// CStoryboardSldCtrl

class CUIDesignWnd;
class CStoryboardSldCtrl : public CSliderCtrlEx
{
public:
	CStoryboardSldCtrl();
	virtual ~CStoryboardSldCtrl();

	virtual void DrawHorzSlider(CDC *pMemDC);

	int SetBindUIWnd(CUIDesignWnd *pUIDesignWnd);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	CIUIImage m_imgPoint;
	CUIDesignWnd *m_pUIDesignWnd;
};
