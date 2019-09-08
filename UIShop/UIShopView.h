// UIShopView.h : interface of the CUIShopView class
//


#pragma once


#include "UIDesignWnd.h"
#include "UIShopDoc.h"

class CUIShopView : public CScrollView
{
protected: // create from serialization only
	CUIShopView();
	DECLARE_DYNCREATE(CUIShopView)
	virtual ~CUIShopView();

protected:
	int AllocCtrlProperties(CControlWnd *pCtrlWnd);
	BOOL CreateChildControls(const UIWNDPROPERTIES *pUIProp, CWnd *pParent);

public:
	CUIShopDoc* GetDocument() const;
	void SetCurZoomPercent(int nPercent) { ASSERT(m_pWndUIDesign->GetSafeHwnd() != NULL); m_pWndUIDesign->SetCurZoomPercent(nPercent); }
	int GetCurZoomPercent() { ASSERT(m_pWndUIDesign->GetSafeHwnd() != NULL); return m_pWndUIDesign->GetCurZoomPercent(); }

	CUIDesignWnd *GetCurUIDesignWnd() const;
	CString GetCurPreviewText();
	void SwitchWindow(CUIDesignWnd *pwndUIDesign);
	void SwitchCtrlProperties(CTRLPROPERTIES *pCtrlResource);

public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnViewShowControlType();
	afx_msg void OnUpdateViewShowControlType(CCmdUI* pCmdUI);
	afx_msg void OnViewShowControlID();
	afx_msg void OnUpdateViewShowControlID(CCmdUI* pCmdUI);
	afx_msg void OnViewShowControlIndex();
	afx_msg void OnUpdateViewShowControlIndex(CCmdUI* pCmdUI);
	afx_msg void OnLayoutAlignLeft();
	afx_msg void OnUpdateLayoutAlignLeft(CCmdUI* pCmdUI);
	afx_msg void OnLayoutAlignTop();
	afx_msg void OnUpdateLayoutAlignTop(CCmdUI* pCmdUI);
	afx_msg void OnLayoutAlignRight();
	afx_msg void OnUpdateLayoutAlignRight(CCmdUI* pCmdUI);
	afx_msg void OnLayoutAlignBottom();
	afx_msg void OnUpdateLayoutAlignBottom(CCmdUI* pCmdUI);
	afx_msg void OnLayoutCenterHorizontal();
	afx_msg void OnUpdateLayoutCenterHorizontal(CCmdUI* pCmdUI);
	afx_msg void OnLayoutCenterVertical();
	afx_msg void OnUpdateLayoutCenterVertical(CCmdUI* pCmdUI);
	afx_msg void OnLayoutSpaceAcross();
	afx_msg void OnUpdateLayoutSpaceAcross(CCmdUI* pCmdUI);
	afx_msg void OnLayoutSpaceDown();
	afx_msg void OnUpdateLayoutSpaceDown(CCmdUI* pCmdUI);
	afx_msg void OnLayoutSamesizeWidth();
	afx_msg void OnUpdateLayoutSamesizeWidth(CCmdUI* pCmdUI);
	afx_msg void OnLayoutSamesizeHeight();
	afx_msg void OnUpdateLayoutSamesizeHeight(CCmdUI* pCmdUI);
	afx_msg void OnLayoutSamesizeSize();
	afx_msg void OnUpdateLayoutSamesizeSize(CCmdUI* pCmdUI);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	CUIDesignWnd *m_pWndUIDesign;
	CTRLPROPERTIES *m_pFocusCtrlProp;	// include children's child
};

#ifndef _DEBUG  // debug version in UIShopView.cpp
inline CUIShopDoc* CUIShopView::GetDocument() const
   { return reinterpret_cast<CUIShopDoc*>(m_pDocument); }
#endif
