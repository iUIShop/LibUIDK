// ControlWnd.h
//

#pragma once
/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

// store the properties of control
class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd();
	virtual ~CControlWnd();


protected:
	virtual int DrawWindowlessColorBlock(CDC *pDC, const CRect &rcWnd);
	virtual int DrawWindowlessLine(CDC *pDC, const LINEWLPROPERTIES *pLineProp);
	virtual int DrawControlImage(
		CDC *pDC,
		const CTRLPROPERTIES *pCtrlProp,
		const CRect &rcWnd,
		const CRect &rcClient);
	virtual int DrawFocusBorder(CDC *pDC, const CRect &rcWnd);
	virtual int DrawSelectedFlag(CDC *pDC, const CRect &rcWnd);
	virtual int DrawTypeFlag(CDC *pDC, const CRect &rcClient);
	virtual int DrawControlID(CDC *pDC, const CRect rcClient);
	virtual int DrawControlIndex(CDC *pDC);
	virtual int DrawControlText(
		CDC *pDC,
		const CTRLPROPERTIES *pCtrlPropDraw,
		const CRect &rcClient);

	// Generated message map functions
protected:
#if WINVER >= 0x0502
	afx_msg LRESULT OnNcHitTest(CPoint point);
#else
	afx_msg UINT OnNcHitTest(CPoint point);
#endif
	//{{AFX_MSG(CControlWnd)
	afx_msg void OnPaint();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCtrlProperties();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg LRESULT OnExitSizeMove(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnCtrlAddHandler();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void ShowPropertiesDlg();

	void SetTypeFlag(BOOL bShow);
	void ShowIDFlag(BOOL bShow);
	void ShowIndexValue(BOOL bShow);
	void SetIndexValue(int nIndex);
	void Selected(BOOL bSelected);
	BOOL IsSelected();
	void SetControlProperties(CTRLPROPERTIES *pCtrlProperties);
	CTRLPROPERTIES *GetControlProperties();
	void SetControlType(CONTROL_TYPE eControlType);

	int GetMoveOffset(int *pnXMove, int *pnYMove);

	int HighlightControl();

protected:
	// control's properties
	CTRLPROPERTIES *m_pCtrlProperties;

public:
	// General properties
	CONTROL_TYPE m_eControlType;

	BOOL m_bSelected;// when choose CTRL+A in CUIDesignWnd, set m_bSelected to TRUE, and show control type
	BOOL m_bShowType;
	BOOL m_bShowID;
	BOOL m_bShowIndex;
	int m_nIndex;	// Temp value

public:
	CControlWnd *m_pMoveSender;
	int m_nXMove;
	int m_nYMove;

	// For highlight
	int m_nHighlightCount;

	// For redo & undo
	BOOL m_bPressedLeftButton;
	CRect m_rcPressed;
};
