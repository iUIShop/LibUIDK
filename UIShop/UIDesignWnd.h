// UIDesignWnd.h
//

#pragma once

#include <VECTOR>
using namespace std;
#include "ControlWnd.h"


enum MakeSameSize
{
	MSS_WIDTH,
	MSS_HEIGHT,
	MSS_SIZE,
};


/////////////////////////////////////////////////////////////////////////////
// CUIDesignWnd window

class CUIDesignWnd : public CWnd
{
// Construction
public:
	CUIDesignWnd();
	virtual ~CUIDesignWnd();

protected:
	int MoveSelectedControls(CControlWnd *pSender, int nXOffset, int nYOffset);
	int GetControlIndex(const CTRLPROPERTIES *pCtrlProp);

public:
	int GetSelectedControls(vector<CControlWnd *> *pvControls);

	void SetCurZoomPercent(int nPercent);
	int GetCurZoomPercent();

	int SetFocusCtrl(CTRLPROPERTIES *pFocusCtrl, BOOL bFocus);
	CTRLPROPERTIES *GetFocusCtrl();

	BOOL IsUIWindowSelected() const;

	int AlignSelectedControls(int nDirection);
	int CenterSelectedControls(BOOL bHorizontal);
	int SpaceSelectedControls(BOOL bHorizontal);
	int MakeSameSizeSelectedControls(MakeSameSize enumMask);

	// If pCtrlPropAttach is NULL, alloc the CTRLPROPERTIES memory to the new CControlWnd,
	// Otherwise, attach pCtrlPropAttach to the new CControlWnd.
	CControlWnd *AddNewControl(const RECT& rect, CONTROL_TYPE eControlType, CTRLPROPERTIES *pCtrlPropAttach = NULL);
	int PasteNewControl(CTRLPROPERTIES *pCtrlProp);
	int DeleteControl(CControlWnd *pCtrlWnd, BOOL bDeleteCTRLPROPERTIES);
	int GetSelectedControlsCount();

	int HighlightControl(const CTRLPROPERTIES *pCtrlProp);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUIDesignWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL


	// Generated message map functions
protected:
	//{{AFX_MSG(CUIDesignWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void UpdateControlCoordinate(CControlWnd *pwndCtrl);
	void UpdateFocusControlPosInStatusBar(CTRLPROPERTIES *pCtrlProp);
	void MoveControls(UINT nChar, BOOL bShift, BOOL bCtrl);
	void ShowTypeFlag(BOOL bShow);
	BOOL IsTypeFlagShow();
	void ShowIDFlag(BOOL bShow);
	BOOL IsIDFlagShow();
	void ShowIndexFlag(BOOL bShow);
	BOOL IsIndexFlagShow();
	void SelectAllControls(CControlWnd *pSender, BOOL bSelect);
	int SelectControlByRect(const CRect &rect);
	BOOL SetBkBitmap();

	int SetUIWndProperties(UIWNDPROPERTIES *pProperties);
	UIWNDPROPERTIES *GetUIWndProperties();

	virtual void DrawBkImage(CDC *pMemDC);
	virtual void DrawBackgroundShowControls(CDC *pMemDC);
	virtual void DrawResizingLine(CDC *pMemDC);
	virtual void DrawClientArea(CDC *pMemDC);
	virtual void DrawSelectedFlag(CDC *pMemDC);

	int SetCurStoryFrame(int nFrame);
	int GetCurStoryFrame() const;

public:
	HIUIIMAGE m_himgBk;
	IMAGE_RESIZE_MODE m_eBkImageResizeMode;
	POINT m_ptBkImageResize;
	LONG m_lBkImageRepeatX;
	LONG m_lBkImageRepeatY;

	CBitmap m_bmpMemBK;
	vector<CControlWnd *> m_listCtrl;
	BOOL m_bBackgroundShow;

private:
	UIWNDPROPERTIES *m_pUIWndProperties;	// point to the properties struct in CUIWnd::m_vpWindows.
	int m_nID;
	BOOL m_bSizeToContent;
	int m_nCurZoomPercent; // zoom the window for edit control
	CTRLPROPERTIES *m_pCtrlFocus;	// Focus control properties in UIWNDPROPERTIES::vControls. NOT in storyboard.
	BOOL m_bValidLButtonUp;

	BOOL m_bSelected;
	CPoint m_ptLButtonDown;
	CRect m_rcLButtonDown;
	BOOL m_bDragSelectChildren;
	CRect m_rcPreviousDragSelectRect;
	int m_nHitTestRet; // the result of Hit test the resizing window witch can be resizing by dragging its board. default to HTCLIENT;

	// Show flag.
	BOOL m_bShowControlType;
	BOOL m_bShowControlID;
	BOOL m_bShowControlIndex;

	int m_nCurStoryFrame;
};
