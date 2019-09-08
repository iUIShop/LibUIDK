// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "UIShop.h"
#include "ChildFrm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_NOTIFY(UIVN_FOCUSCHANGED, AFX_IDW_PANE_FIRST, OnFocusChangedPropertyControl)	// AFX_IDW_PANE_FIRST is ID of CUIShopView
END_MESSAGE_MAP()

// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_CHILD|WS_VISIBLE|WS_OVERLAPPED|FWS_ADDTOTITLE|WS_THICKFRAME|WS_MAXIMIZE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return TRUE;
}

// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame message handlers

void CChildFrame::OnFocusChangedPropertyControl(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMUISHOPVIEW * pNMUIShopView = (NMUISHOPVIEW *)pNMHDR;
	// TODO: Add your control notification handler code here

	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CPropertiesWnd *pPropWnd = pFrm->GetPropertiesWnd();

	// Update content in property window.
	pPropWnd->ShowProperties(pNMUIShopView->pNewCtrlProp);

	// Update content in status bar.
	if (pNMUIShopView->pNewCtrlProp != NULL)
	{
		CONTROL_TYPE eControlType =  pNMUIShopView->pNewCtrlProp->m_eControlType;

		if (eControlType == CT_PUSHBUTTON
			|| eControlType == CT_CHECK
			|| eControlType == CT_RADIO
			|| eControlType == CT_COMBOBOX
			|| eControlType == CT_STATIC
			|| eControlType == CT_EDIT
			|| eControlType == CT_SLIDER
			|| eControlType == CT_PROGRESS
			|| eControlType == CT_SPIN
			|| eControlType == CT_HOTKEY
			|| eControlType == CT_IPADDRESS
			|| eControlType == CT_SLIDEREX
			|| eControlType == CT_LISTCTRL
			|| eControlType == CT_TREECTRL
			|| eControlType == CT_SCROLLBAR
			|| eControlType == CT_SCROLLBAREX
			|| eControlType == CT_TREELIST
			|| eControlType == CT_THUMBNAIL
			|| eControlType == CT_RICHEDIT
			|| eControlType == CT_RICHEDIT_IM
			|| eControlType == CT_HTMLCTRL
			|| eControlType == CT_DATETIMECTRL
			|| eControlType == CT_WAVECTRL
			|| eControlType == CT_PICTURE
			|| eControlType == CT_MENUBAR
			|| eControlType == CT_SPLITTER
			|| eControlType == CT_WL_RECTCTRL
			|| eControlType == CT_WL_SPLITTER
			|| eControlType == CT_WL_LINE
			|| eControlType == CT_WL_TEXT
			|| eControlType == CT_WL_BUTTON
			|| eControlType == CT_WL_CHECK
			|| eControlType == CT_WL_RADIO
			|| eControlType == CT_WL_SLIDER
			|| eControlType == CT_WL_PICTURE
			|| eControlType == CT_WL_RICHEDIT
			|| eControlType == CT_WL_RICHEDIT_IM
			|| eControlType == CT_TASK_WND_MGR
			|| eControlType == CT_PNL_DOCK
			|| eControlType == CT_UNIFORM_GRID
			|| eControlType == CT_PNL_STACK
			|| eControlType == CT_UIWND)
		{
			ASSERT(m_pViewActive != NULL);
			CUIShopView *pUIShopView = (CUIShopView *)m_pViewActive;
			CUIDesignWnd *pUIDesignWnd = pUIShopView->GetCurUIDesignWnd();
			pUIDesignWnd->UpdateFocusControlPosInStatusBar(pNMUIShopView->pNewCtrlProp);
		}
	}

	*pResult = 0;
}
