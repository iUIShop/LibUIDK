// UIShopView.cpp : implementation of the CUIShopView class
//

#include "stdafx.h"
#include "UIShop.h"

#include "UIShopDoc.h"
#include "UIShopView.h"
#include "ViewTree.h"
#include "MainFrm.h"
#include <algorithm>
#include "IDMgr.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern CMainFrame *g_pFrm;

/////////////////////////////////////////////////////////////////////////////
// CUIShopView

IMPLEMENT_DYNCREATE(CUIShopView, CScrollView)

BEGIN_MESSAGE_MAP(CUIShopView, CScrollView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_WM_PARENTNOTIFY()
	ON_COMMAND(ID_VIEW_SHOW_CONTROL_TYPE, OnViewShowControlType)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_CONTROL_TYPE, OnUpdateViewShowControlType)
	ON_COMMAND(ID_VIEW_SHOW_CONTROL_ID, OnViewShowControlID)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_CONTROL_ID, OnUpdateViewShowControlID)
	ON_COMMAND(ID_VIEW_SHOW_CONTROL_INDEX, OnViewShowControlIndex)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_CONTROL_INDEX, OnUpdateViewShowControlIndex)
	ON_COMMAND(ID_LAYOUT_ALIGN_LEFT, OnLayoutAlignLeft)
	ON_UPDATE_COMMAND_UI(ID_LAYOUT_ALIGN_LEFT, OnUpdateLayoutAlignLeft)
	ON_COMMAND(ID_LAYOUT_ALIGN_TOP, OnLayoutAlignTop)
	ON_UPDATE_COMMAND_UI(ID_LAYOUT_ALIGN_TOP, OnUpdateLayoutAlignTop)
	ON_COMMAND(ID_LAYOUT_ALIGN_RIGHT, OnLayoutAlignRight)
	ON_UPDATE_COMMAND_UI(ID_LAYOUT_ALIGN_RIGHT, OnUpdateLayoutAlignRight)
	ON_COMMAND(ID_LAYOUT_ALIGN_BOTTOM, OnLayoutAlignBottom)
	ON_UPDATE_COMMAND_UI(ID_LAYOUT_ALIGN_BOTTOM, OnUpdateLayoutAlignBottom)
	ON_COMMAND(ID_LAYOUT_CENTER_HORIZONTAL, OnLayoutCenterHorizontal)
	ON_UPDATE_COMMAND_UI(ID_LAYOUT_CENTER_HORIZONTAL, OnUpdateLayoutCenterHorizontal)
	ON_COMMAND(ID_LAYOUT_CENTER_VERTICAL, OnLayoutCenterVertical)
	ON_UPDATE_COMMAND_UI(ID_LAYOUT_CENTER_VERTICAL, OnUpdateLayoutCenterVertical)
	ON_COMMAND(ID_LAYOUT_SPACE_ACROSS, OnLayoutSpaceAcross)
	ON_UPDATE_COMMAND_UI(ID_LAYOUT_SPACE_ACROSS, OnUpdateLayoutSpaceAcross)
	ON_COMMAND(ID_LAYOUT_SPACE_DOWN, OnLayoutSpaceDown)
	ON_UPDATE_COMMAND_UI(ID_LAYOUT_SPACE_DOWN, OnUpdateLayoutSpaceDown)
	ON_COMMAND(ID_LAYOUT_SAMESIZE_WIDTH, OnLayoutSamesizeWidth)
	ON_UPDATE_COMMAND_UI(ID_LAYOUT_SAMESIZE_WIDTH, OnUpdateLayoutSamesizeWidth)
	ON_COMMAND(ID_LAYOUT_SAMESIZE_HEIGHT, OnLayoutSamesizeHeight)
	ON_UPDATE_COMMAND_UI(ID_LAYOUT_SAMESIZE_HEIGHT, OnUpdateLayoutSamesizeHeight)
	ON_COMMAND(ID_LAYOUT_SAMESIZE_SIZE, OnLayoutSamesizeSize)
	ON_UPDATE_COMMAND_UI(ID_LAYOUT_SAMESIZE_SIZE, OnUpdateLayoutSamesizeSize)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//

int CUIShopView::AllocCtrlProperties(CControlWnd *pCtrlWnd)
{
	CTRLPROPERTIES *pCtrlProperties = NULL;
	if (pCtrlWnd->m_eControlType == CT_PUSHBUTTON)
	{
		pCtrlProperties = new BTNPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextButtonID();
	}
	else if (pCtrlWnd->m_eControlType == CT_CHECK)
	{
		pCtrlProperties = new CHKPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextCheckBoxID();
	}
	else if (pCtrlWnd->m_eControlType == CT_RADIO)
	{
		pCtrlProperties = new RADPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextRadioButtonID();
	}
	else if (pCtrlWnd->m_eControlType == CT_STATIC)
	{
		pCtrlProperties = new TXTPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextStaticID();
	}
	else if (pCtrlWnd->m_eControlType == CT_EDIT)
	{
		pCtrlProperties = new EDTPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextEditID();
	}
	else if (pCtrlWnd->m_eControlType == CT_COMBOBOX)
	{
		pCtrlProperties = new CMBPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextComboBoxID();
	}
	else if (pCtrlWnd->m_eControlType == CT_PROGRESS)
	{
		pCtrlProperties = new PRGPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextProgressID();
	}
	else if (pCtrlWnd->m_eControlType == CT_SLIDER)
	{
		pCtrlProperties = new SLDPROPERTIES;
		((SLDPROPERTIES *)pCtrlProperties)->m_eExtendedStyle.bShowInPropertiesWnd = false;
		((SLDPROPERTIES *)pCtrlProperties)->m_eExtendedStyle.bSaveToSkinFile = false;
		((SLDPROPERTIES *)pCtrlProperties)->m_bEnableTipWindow.bShowInPropertiesWnd = false;
		((SLDPROPERTIES *)pCtrlProperties)->m_bEnableTipWindow.bSaveToSkinFile = false;
		((SLDPROPERTIES *)pCtrlProperties)->m_sizeTipWindowOffset.bShowInPropertiesWnd = false;
		((SLDPROPERTIES *)pCtrlProperties)->m_sizeTipWindowOffset.bSaveToSkinFile = false;
		((SLDPROPERTIES *)pCtrlProperties)->m_bVertical.bGroup = true;
		((SLDPROPERTIES *)pCtrlProperties)->m_bVertical.strGroupName = _T("General");
		pCtrlProperties->m_strID = CIDMgr::NextSliderID();
	}
	else if (pCtrlWnd->m_eControlType == CT_SPIN)
	{
		pCtrlProperties = new SPNPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextSpinID();
	}
	else if (pCtrlWnd->m_eControlType == CT_HOTKEY)
	{
		pCtrlProperties = new HOTPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextHotKeyID();
	}
	else if (pCtrlWnd->m_eControlType == CT_IPADDRESS)
	{
		pCtrlProperties = new IPAPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextIPAddressID();
	}
	else if (pCtrlWnd->m_eControlType == CT_SLIDEREX)
	{
		pCtrlProperties = new SLDEXPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextSliderExID();
	}
	else if (pCtrlWnd->m_eControlType == CT_LISTCTRL)
	{
		pCtrlProperties = new LSTCTRLPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextListCtrlID();
	}
	else if (pCtrlWnd->m_eControlType == CT_TREECTRL)
	{
		pCtrlProperties = new TREECTRLPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextTreeCtrlID();
	}
	else if (pCtrlWnd->m_eControlType == CT_SCROLLBAREX)
	{
		pCtrlProperties = new SBEXPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextScrollBarExID();
	}
	else if (pCtrlWnd->m_eControlType == CT_THUMBNAIL)
	{
		pCtrlProperties = new THMPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextThumbnailID();
	}
	else if (pCtrlWnd->m_eControlType == CT_TREELIST)
	{
		pCtrlProperties = new TREELISTPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextTreeLstID();
	}
	else if (pCtrlWnd->m_eControlType == CT_RICHEDIT)
	{
		pCtrlProperties = new RICHEDITPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextRichEditID();
	}
	else if (pCtrlWnd->m_eControlType == CT_RICHEDIT_IM)
	{
		pCtrlProperties = new IMREPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextIMRichEditID();
	}
	else if (pCtrlWnd->m_eControlType == CT_HTMLCTRL)
	{
		pCtrlProperties = new HTMLCTRLPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextHtmlCtrlID();
	}
	else if (pCtrlWnd->m_eControlType == CT_DATETIMECTRL)
	{
		pCtrlProperties = new DATETIMECTRLPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextDateTimeCtrlID();
	}
	else if (pCtrlWnd->m_eControlType == CT_WAVECTRL)
	{
		pCtrlProperties = new WAVECTRLPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextWaveCtrlID();
	}
	else if (pCtrlWnd->m_eControlType == CT_PICTURE)
	{
		pCtrlProperties = new PICWLPROPERTIES;
		pCtrlProperties->m_eControlType = CT_PICTURE;
		pCtrlProperties->m_strID = CIDMgr::NextPicCtrlID();
	}
	else if (pCtrlWnd->m_eControlType == CT_MENUBAR)
	{
		pCtrlProperties = new MENUBARPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextMenuBarID();
	}
	else if (pCtrlWnd->m_eControlType == CT_SPLITTER)
	{
		pCtrlProperties = new SPLITTERPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextSplitterID();
	}
	else if (pCtrlWnd->m_eControlType == CT_WL_RECTCTRL)
	{
		pCtrlProperties = new RECTWLPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextWLRectID();
	}
	else if (pCtrlWnd->m_eControlType == CT_WL_SPLITTER)
	{
		pCtrlProperties = new SPLITTERWLPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextWLSplitterID();
	}
	else if (pCtrlWnd->m_eControlType == CT_WL_LINE)
	{
		pCtrlProperties = new LINEWLPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextWLLineID();
	}
	else if (pCtrlWnd->m_eControlType == CT_WL_TEXT)
	{
		pCtrlProperties = new TXTPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_TEXT;
		pCtrlProperties->m_strID = CIDMgr::NextWLTextID();
	}
	else if (pCtrlWnd->m_eControlType == CT_WL_BUTTON)
	{
		pCtrlProperties = new BTNPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_BUTTON;
		pCtrlProperties->m_strID = CIDMgr::NextWLBtnID();
	}
	else if (pCtrlWnd->m_eControlType == CT_WL_CHECK)
	{
		pCtrlProperties = new BTNPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_CHECK;
		pCtrlProperties->m_strID = CIDMgr::NextWLChkID();
	}
	else if (pCtrlWnd->m_eControlType == CT_WL_RADIO)
	{
		pCtrlProperties = new BTNPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_RADIO;
		pCtrlProperties->m_strID = CIDMgr::NextWLRadID();
	}
	else if (pCtrlWnd->m_eControlType == CT_WL_SLIDER)
	{
		pCtrlProperties = new SLDEXPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_SLIDER;
		pCtrlProperties->m_strID = CIDMgr::NextWLSliderID();
	}
	else if (pCtrlWnd->m_eControlType == CT_WL_PICTURE)
	{
		pCtrlProperties = new PICWLPROPERTIES;
		pCtrlProperties->m_strID = CIDMgr::NextWLPicID();
	}
	else if (pCtrlWnd->m_eControlType == CT_WL_RICHEDIT)
	{
		pCtrlProperties = new RICHEDITPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_RICHEDIT;
		pCtrlProperties->m_strID = CIDMgr::NextWLRichEditID();
	}
	else if (pCtrlWnd->m_eControlType == CT_WL_RICHEDIT_IM)
	{
		pCtrlProperties = new IMREPROPERTIES;
		pCtrlProperties->m_eControlType = CT_WL_RICHEDIT_IM;
		pCtrlProperties->m_strID = CIDMgr::NextWLIMID();
	}
	else if (pCtrlWnd->m_eControlType == CT_TASK_WND_MGR)
	{
		pCtrlProperties = new TWMGRPROPERTIES;
		pCtrlProperties->m_eControlType = CT_TASK_WND_MGR;
		pCtrlProperties->m_strID = CIDMgr::NextTaskWndMgrID();
	}
	else if (pCtrlWnd->m_eControlType == CT_PNL_DOCK)
	{
		pCtrlProperties = new DOCKPNLPROPERTIES;
		pCtrlProperties->m_eControlType = CT_PNL_DOCK;
		pCtrlProperties->m_strID = CIDMgr::NextPnlDockID();
	}
	else if (pCtrlWnd->m_eControlType == CT_UNIFORM_GRID)
	{
		pCtrlProperties = new UGRIDPNLPROPERTIES;
		pCtrlProperties->m_eControlType = CT_UNIFORM_GRID;
		pCtrlProperties->m_strID = CIDMgr::NextPnlUniformGridID();
	}
	else if (pCtrlWnd->m_eControlType == CT_PNL_STACK)
	{
		pCtrlProperties = new STACKPNLPROPERTIES;
		pCtrlProperties->m_eControlType = CT_PNL_STACK;
		pCtrlProperties->m_strID = CIDMgr::NextPnlStackID();
	}

	// Assign ID value
	if (pCtrlProperties != NULL)
	{
		pCtrlProperties->m_nID = (LONG)CIDMgr::NextControlIDValue();		// A temporary ID value for test UIWnd.
	}

	pCtrlWnd->SetControlProperties(pCtrlProperties);

	return 0;
}

BOOL CUIShopView::CreateChildControls(const UIWNDPROPERTIES *pUIProp, CWnd *pParent)
{
	ASSERT(pUIProp != NULL);
	size_t nCtrlCount = pUIProp->m_vControls.size();
	for (size_t i=0; i<nCtrlCount; ++i)
	{
		CTRLPROPERTIES *pCtrlProp = pUIProp->m_vControls[i];

		CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;
		ASSERT(eControlType != CT_ARROW);

		// Rect
		CRect rect = pCtrlProp->m_rcControl;

		// ID number
		int nID = (LONG)pCtrlProp->m_nID;
		ASSERT(nID != -1);

		// Caption
		CString strName;

		// Create control
		CControlWnd *pChild = new CControlWnd;
		ASSERT(pChild != NULL);
		// Push button
		if (eControlType == CT_PUSHBUTTON || eControlType == CT_WL_BUTTON)
		{
			// Caption
			strName = ((BTNPROPERTIES *)pCtrlProp)->m_strCaption;
		}
		// Check box
		if (eControlType == CT_CHECK || eControlType == CT_WL_CHECK)
		{
			// Caption
			strName = ((CHKPROPERTIES *)pCtrlProp)->m_strCaption;
		}
		// Radio button
		if (eControlType == CT_RADIO || eControlType == CT_WL_RADIO)
		{
			// Caption
			strName = ((RADPROPERTIES *)pCtrlProp)->m_strCaption;
		}
		// Static
		if (eControlType == CT_STATIC || eControlType == CT_WL_TEXT)
		{
			// caption
			strName = ((TXTPROPERTIES *)pCtrlProp)->m_strCaption;
		}
		pChild->SetControlType(eControlType);

		if (rect.right <= rect.left || rect.bottom <= rect.top)
		{
			CString str;
			str.Format(_T("The control's RECT is invalid(%d, %d, %d, %d), its IDs is %s"),rect.left, rect.top, rect.right, rect.bottom, (LPCTSTR)pCtrlProp->m_strID);
			AfxMessageBox(str);
		}

		if (!pChild->Create(NULL, strName, WS_CHILD|WS_VISIBLE, rect, pParent, nID))
			return FALSE;
		pChild->SetControlProperties(pCtrlProp);
		((CUIDesignWnd *)pParent)->m_listCtrl.push_back(pChild);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CUIShopView construction/destruction

CUIShopView::CUIShopView()
{
	// TODO: add construction code here

	m_pWndUIDesign = NULL;
	m_pFocusCtrlProp = NULL;
	g_pFrm->SetUIShopView(this);
}

CUIShopView::~CUIShopView()
{
	g_pFrm->SetUIShopView(NULL);
}

BOOL CUIShopView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= WS_CLIPCHILDREN;

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CUIShopView drawing

void CUIShopView::OnDraw(CDC* pDC)
{
	CUIShopDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CRect rcClient;
	GetClientRect(rcClient);

	rcClient.right = VIEW_WIDTH_MAX;
	rcClient.bottom = VIEW_HEIGHT_MAX;

	pDC->FillSolidRect(rcClient, RGB(255, 255, 255));

	// When set the Background color in CToolsOptionsDlg,
	// refresh the current Preview window, and fill it use color CMainFrame::m_cr.
	CUIDesignWnd *pWnd = GetCurUIDesignWnd();
	if (::IsWindow(pWnd->GetSafeHwnd()))
		pWnd->InvalidateRect(NULL);
}

void CUIShopView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: calculate the total size of this view
	CSize sizeTotal;
	sizeTotal.cx = VIEW_WIDTH_MAX;
	sizeTotal.cy = VIEW_HEIGHT_MAX;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CUIShopView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CUIShopView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CUIShopView diagnostics

#ifdef _DEBUG
void CUIShopView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CUIShopView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CUIShopDoc* CUIShopView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUIShopDoc)));
	return (CUIShopDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUIShopView message handlers

int CUIShopView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CSize sizeTotal;
	sizeTotal.cx = VIEW_WIDTH_MAX;
	sizeTotal.cy = VIEW_HEIGHT_MAX;
	SetScrollSizes(MM_TEXT, sizeTotal);

	SetWindowText(_T("CUIShopView"));

	return 0;
}

BOOL CUIShopView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CScrollView::OnEraseBkgnd(pDC);
}

BOOL CUIShopView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if (zDelta > 0)
	{
		SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEUP, 0), (LPARAM)NULL);
	}
	else
	{
		SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEDOWN, 0), (LPARAM)NULL);
	}

	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CUIShopView::OnParentNotify(UINT message, LPARAM lParam)
{
	CScrollView::OnParentNotify(message, lParam);

	// TODO: Add your message handler code here

	// If the CUIDesignWnd is destroyed, assign m_pWndUIDesign to NULL
	UINT uEvent = LOWORD(message);
	if (uEvent == WM_DESTROY)
	{
		HWND hChild = (HWND)lParam;
		if (m_pWndUIDesign->GetSafeHwnd() == hChild)
		{
			m_pWndUIDesign = NULL;
		}
	}
}

int SendFocusChangedNotify(HWND hwndFrom, CTRLPROPERTIES *pOldCtrlProp, CTRLPROPERTIES *pNewCtrlProp)
{
	// Send UIWND changed notify to parent
	NMHDR nmhdr;
	nmhdr.hwndFrom = hwndFrom;
	nmhdr.idFrom = ::GetDlgCtrlID(hwndFrom);
	nmhdr.code = UIVN_FOCUSCHANGED;
	NMUISHOPVIEW vhdr;
	vhdr.hdr = nmhdr;
	vhdr.pOldCtrlProp = pOldCtrlProp;
	vhdr.pNewCtrlProp = pNewCtrlProp;
	::SendMessage(::GetParent(hwndFrom), WM_NOTIFY, nmhdr.idFrom, LPARAM(&vhdr));

	return 0;
}

void CUIShopView::SwitchWindow(CUIDesignWnd *pwndUIDesign)
{
	if (pwndUIDesign == m_pWndUIDesign)
	{
		if (pwndUIDesign != NULL)
		{
			pwndUIDesign->SelectAllControls(NULL, FALSE);
			SendFocusChangedNotify(m_hWnd, m_pWndUIDesign->GetUIWndProperties(), pwndUIDesign->GetUIWndProperties());
		}
		return;
	}

	// Hide old UI design window
	HWND hWnd = m_pWndUIDesign->GetSafeHwnd();
	if (hWnd != NULL && IsWindow(hWnd))
	{
		m_pWndUIDesign->ShowWindow(SW_HIDE);

		// Clear the flag.
		m_pWndUIDesign->ShowTypeFlag(FALSE);
		m_pWndUIDesign->ShowIDFlag(FALSE);
		m_pWndUIDesign->ShowIndexFlag(FALSE);
	}

	CTRLPROPERTIES *pOldCtrlProp = NULL;
	if (m_pWndUIDesign != NULL)
		pOldCtrlProp = m_pWndUIDesign->GetUIWndProperties();

	// Show new UI design window
	m_pWndUIDesign = pwndUIDesign;

	if (m_pWndUIDesign != NULL)
	{
		m_pFocusCtrlProp = m_pWndUIDesign->GetUIWndProperties();

		if (pwndUIDesign->GetSafeHwnd() == NULL)
		{
			// May be delay loaded.
			if (!BOOL(m_pWndUIDesign->GetUIWndProperties()->m_bLoaded))
			{
				CUIMgr::DelayLoadWindow(m_pWndUIDesign->GetUIWndProperties());
			}

			CString strTitle = _T("CUIDesignWnd [");
			strTitle += (LPCTSTR)m_pFocusCtrlProp->m_strID;
			strTitle += _T("]");
			BOOL bRet = pwndUIDesign->Create(NULL, strTitle, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, VIEW_WIDTH_MAX, VIEW_HEIGHT_MAX), this, 1111);
			if (!bRet)
			{
				ASSERT(FALSE);
				return;
			}
			pwndUIDesign->SetBkBitmap();

			// Delay create it's children
			if (!CreateChildControls(m_pWndUIDesign->GetUIWndProperties(), pwndUIDesign))
				return;
		}

		if (m_pWndUIDesign->GetSafeHwnd() != NULL)
		{
			m_pWndUIDesign->ShowWindow(SW_SHOW);
			POINT pt = {0, 0};
			ScrollToPosition(pt);

			CUIShopDoc *pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->SetTitle(m_pWndUIDesign->GetUIWndProperties()->m_strID);

			UIWNDPROPERTIES *pUIWndProp = (UIWNDPROPERTIES *)m_pFocusCtrlProp;
			if (!pUIWndProp->m_bLoaded)
			{
				CUIMgr::DelayLoadWindow(pUIWndProp);
			}
			SendFocusChangedNotify(m_hWnd, pOldCtrlProp, m_pFocusCtrlProp);
		}
	}
	else
	{
		m_pFocusCtrlProp = NULL;
	}

	g_pFrm->GetStoryboardPanel()->SetBindUIWnd(m_pWndUIDesign);
}

void CUIShopView::SwitchCtrlProperties(CTRLPROPERTIES *pCtrlResource)
{
	// Hide old UI design window
	if (m_pWndUIDesign != NULL)
	{
		HWND hWnd = m_pWndUIDesign->GetSafeHwnd();
		if (hWnd != NULL && IsWindow(hWnd))
			m_pWndUIDesign->ShowWindow(SW_HIDE);

		m_pWndUIDesign = NULL;
	}

	// Show new resource properties.
	CTRLPROPERTIES *pOldCtrlProp = NULL;
	m_pFocusCtrlProp = pCtrlResource;

	if (m_pFocusCtrlProp != NULL)
	{
		CUIShopDoc *pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pDoc->SetTitle(pCtrlResource->m_strID);

		SendFocusChangedNotify(m_hWnd, pOldCtrlProp, m_pFocusCtrlProp);
	}
}

void CUIShopView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: Add your specialized code here and/or call the base class

	SendMessage(WM_SIZE);

	CUIShopDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_pTree->GetSafeHwnd() == NULL)
		return;

	HTREEITEM hTI = pDoc->m_pTree->GetSelectedItem();
	if (hTI == NULL)
		return;

#ifdef _DEBUG
	CString strText = pDoc->m_pTree->GetItemText(hTI);
#endif

	HTREEITEM hRoot = pDoc->m_pTree->GetRootItem();
	if (hTI == hRoot)
	{
		SwitchWindow(NULL);
		return;
	}

	TREE_ITEMDATA *pData = (TREE_ITEMDATA *)pDoc->m_pTree->GetItemData(hTI);
	ASSERT(pData != NULL);
	if (pData == NULL)
		return;

	if (pData->eTreeItemType == WINDOWS_NODE)
		SwitchWindow(&(pData->wndUIDesign));
	else if (pData->eTreeItemType == RES_CONDITION_FONT_NODE)
		SwitchCtrlProperties(pData->pResFont->m_pFontResProp);
	else if (pData->eTreeItemType == RES_CONDITION_CHILD_FONT_NODE)
		SwitchCtrlProperties(pData->pResourceOrStyle);
	else if (pData->eTreeItemType == RES_GENERAL_FONT_NODE)
		SwitchCtrlProperties(pData->pResFont->m_pFontResProp);
	else if (pData->eTreeItemType == RES_COLOR_NODE)
		SwitchCtrlProperties(pData->pResourceOrStyle);
	else if (pData->eTreeItemType >= STYLE_PUSHBUTTON_NODE && pData->eTreeItemType <= STYLE_SPLITTER_NODE)
		SwitchCtrlProperties(pData->pResourceOrStyle);
}

BOOL CUIShopView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class

	NMHDR *pNMHdr = (NMHDR *)lParam;
	if (pNMHdr->code == UIVN_FOCUSCHANGED && ::GetParent(pNMHdr->hwndFrom) == m_hWnd)
	{
		NMUISHOPVIEW *pHdr = (NMUISHOPVIEW *)lParam;
		if (pHdr->pNewCtrlProp != m_pFocusCtrlProp)
		{
			NMHDR nmhdr;
			nmhdr.hwndFrom = m_hWnd;
			nmhdr.idFrom = GetDlgCtrlID();
			nmhdr.code = UIVN_FOCUSCHANGED;
			NMUISHOPVIEW vhdr;
			vhdr.hdr = nmhdr;
			vhdr.pOldCtrlProp = m_pFocusCtrlProp;
			vhdr.pNewCtrlProp = pHdr->pNewCtrlProp;
			m_pFocusCtrlProp = vhdr.pNewCtrlProp;
			CWnd *pParent = GetParent();
			pParent->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&vhdr));
		}
	}
	else if ((pNMHdr->code == UIVN_ADDNEWCONTROL || pNMHdr->code == UIVN_PASTENEWCONTROL)
		&& ::GetParent(pNMHdr->hwndFrom) == m_hWnd)
	{
		NMUISHOPVIEW *pHdr = (NMUISHOPVIEW *)lParam;
		CControlWnd *pCtrlWnd = (CControlWnd *)pHdr->pNewCtrl;
		CTRLPROPERTIES *pCtrlPropAttach = pHdr->pOldCtrlProp;

		// Alloc CTRLPROPERTIES for new control.
		if (pCtrlPropAttach == NULL)
		{
			AllocCtrlProperties(pCtrlWnd);
		}
		else
		{
			ASSERT(pCtrlWnd->m_eControlType == (CONTROL_TYPE)pCtrlPropAttach->m_eControlType);
			pCtrlWnd->SetControlProperties(pCtrlPropAttach);
		}

		// Add new control to control list
		ASSERT(m_pWndUIDesign != NULL);
		m_pWndUIDesign->m_listCtrl.push_back(pCtrlWnd);

		// Add new CTRLPROPERTIES to properties list
		CTRLPROPERTIES *pCtrlProp = pCtrlWnd->GetControlProperties();
		CRect rcCtrl;
		pCtrlWnd->GetWindowRect(rcCtrl);
		m_pWndUIDesign->ScreenToClient(rcCtrl);
		pCtrlProp->m_rcControl = rcCtrl;
		UIWNDPROPERTIES *pUIProp = m_pWndUIDesign->GetUIWndProperties();
		pUIProp->m_vControls.push_back(pCtrlProp);

		// Set the new control to focus control.
		pCtrlWnd->Selected(TRUE);
		m_pWndUIDesign->SetFocusCtrl(pCtrlWnd->GetControlProperties(), TRUE);
		m_pWndUIDesign->SelectAllControls(pCtrlWnd, FALSE);

		// Set modified flag.
		GetDocument()->SetModifiedFlag(TRUE);

		// Un-select the new control icon in toolbar.
		g_pFrm->SetCurSelControlType(CT_ARROW);
	}

	return CScrollView::OnNotify(wParam, lParam, pResult);
}

void CUIShopView::OnViewShowControlType()
{
	if (m_pWndUIDesign->GetSafeHwnd() != NULL)
	{
		m_pWndUIDesign->ShowTypeFlag(!m_pWndUIDesign->IsTypeFlagShow());
	}

	RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ALLCHILDREN);
}

void CUIShopView::OnUpdateViewShowControlType(CCmdUI* pCmdUI)
{
	CUIShopDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->Enable(pDoc->GetPathName().IsEmpty() ? FALSE : TRUE);
	if (m_pWndUIDesign->GetSafeHwnd() == NULL)
		pCmdUI->SetCheck(0);
	else
		pCmdUI->SetCheck(m_pWndUIDesign->IsTypeFlagShow() ? 1 : 0);
}

void CUIShopView::OnViewShowControlID()
{
	if (m_pWndUIDesign->GetSafeHwnd() != NULL)
	{
		m_pWndUIDesign->ShowIDFlag(!m_pWndUIDesign->IsIDFlagShow());
	}

	RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ALLCHILDREN);
}

void CUIShopView::OnUpdateViewShowControlID(CCmdUI* pCmdUI)
{
	CUIShopDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->Enable(pDoc->GetPathName().IsEmpty() ? FALSE : TRUE);
	if (m_pWndUIDesign->GetSafeHwnd() == NULL)
		pCmdUI->SetCheck(0);
	else
		pCmdUI->SetCheck(m_pWndUIDesign->IsIDFlagShow() ? 1 : 0);
}

void CUIShopView::OnViewShowControlIndex()
{
	if (m_pWndUIDesign->GetSafeHwnd() != NULL)
	{
		m_pWndUIDesign->ShowIndexFlag(!m_pWndUIDesign->IsIndexFlagShow());
	}

	RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ALLCHILDREN);
}

void CUIShopView::OnUpdateViewShowControlIndex(CCmdUI* pCmdUI)
{
	CUIShopDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pCmdUI->Enable(pDoc->GetPathName().IsEmpty() ? FALSE : TRUE);
	if (m_pWndUIDesign->GetSafeHwnd() == NULL)
		pCmdUI->SetCheck(0);
	else
		pCmdUI->SetCheck(m_pWndUIDesign->IsIndexFlagShow() ? 1 : 0);
}

void CUIShopView::OnLayoutAlignLeft()
{
	m_pWndUIDesign->AlignSelectedControls(1);
}

void CUIShopView::OnUpdateLayoutAlignLeft(CCmdUI* pCmdUI)
{
	if (m_pWndUIDesign == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	int nSels = m_pWndUIDesign->GetSelectedControlsCount();
	if (nSels > 1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CUIShopView::OnLayoutAlignTop()
{
	m_pWndUIDesign->AlignSelectedControls(2);
}

void CUIShopView::OnUpdateLayoutAlignTop(CCmdUI* pCmdUI)
{
	if (m_pWndUIDesign == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	int nSels = m_pWndUIDesign->GetSelectedControlsCount();
	if (nSels > 1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CUIShopView::OnLayoutAlignRight()
{
	m_pWndUIDesign->AlignSelectedControls(3);
}

void CUIShopView::OnUpdateLayoutAlignRight(CCmdUI* pCmdUI)
{
	if (m_pWndUIDesign == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	int nSels = m_pWndUIDesign->GetSelectedControlsCount();
	if (nSels > 1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CUIShopView::OnLayoutAlignBottom()
{
	m_pWndUIDesign->AlignSelectedControls(4);
}

void CUIShopView::OnUpdateLayoutAlignBottom(CCmdUI* pCmdUI)
{
	if (m_pWndUIDesign == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	int nSels = m_pWndUIDesign->GetSelectedControlsCount();
	if (nSels > 1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CUIShopView::OnLayoutCenterHorizontal()
{
	m_pWndUIDesign->CenterSelectedControls(TRUE);
}

void CUIShopView::OnUpdateLayoutCenterHorizontal(CCmdUI* pCmdUI)
{
	if (m_pWndUIDesign == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	int nSels = m_pWndUIDesign->GetSelectedControlsCount();
	if (nSels > 0)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CUIShopView::OnLayoutCenterVertical()
{
	m_pWndUIDesign->CenterSelectedControls(FALSE);
}

void CUIShopView::OnUpdateLayoutCenterVertical(CCmdUI* pCmdUI)
{
	if (m_pWndUIDesign == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	int nSels = m_pWndUIDesign->GetSelectedControlsCount();
	if (nSels > 0)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CUIShopView::OnLayoutSpaceAcross()
{
	m_pWndUIDesign->SpaceSelectedControls(TRUE);
}

void CUIShopView::OnUpdateLayoutSpaceAcross(CCmdUI* pCmdUI)
{
	if (m_pWndUIDesign == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	int nSels = m_pWndUIDesign->GetSelectedControlsCount();
	if (nSels > 2)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CUIShopView::OnLayoutSpaceDown()
{
	m_pWndUIDesign->SpaceSelectedControls(FALSE);
}

void CUIShopView::OnUpdateLayoutSpaceDown(CCmdUI* pCmdUI)
{
	if (m_pWndUIDesign == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	int nSels = m_pWndUIDesign->GetSelectedControlsCount();
	if (nSels > 2)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CUIShopView::OnLayoutSamesizeWidth()
{
	m_pWndUIDesign->MakeSameSizeSelectedControls(MSS_WIDTH);
}

void CUIShopView::OnUpdateLayoutSamesizeWidth(CCmdUI* pCmdUI)
{
	if (m_pWndUIDesign == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	int nSels = m_pWndUIDesign->GetSelectedControlsCount();
	if (nSels > 1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CUIShopView::OnLayoutSamesizeHeight()
{
	m_pWndUIDesign->MakeSameSizeSelectedControls(MSS_HEIGHT);
}

void CUIShopView::OnUpdateLayoutSamesizeHeight(CCmdUI* pCmdUI)
{
	if (m_pWndUIDesign == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	int nSels = m_pWndUIDesign->GetSelectedControlsCount();
	if (nSels > 1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CUIShopView::OnLayoutSamesizeSize()
{
	m_pWndUIDesign->MakeSameSizeSelectedControls(MSS_SIZE);
}

void CUIShopView::OnUpdateLayoutSamesizeSize(CCmdUI* pCmdUI)
{
	if (m_pWndUIDesign == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	int nSels = m_pWndUIDesign->GetSelectedControlsCount();
	if (nSels > 1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

//////////////////////////////////////////////////////////////////////////
//

CString CUIShopView::GetCurPreviewText()
{
	if (m_pWndUIDesign->GetSafeHwnd() != NULL)
	{
		return m_pWndUIDesign->GetUIWndProperties()->m_strID;
	}
	else
	{
		return _T("");
	}
}

CUIDesignWnd *CUIShopView::GetCurUIDesignWnd() const
{
	return m_pWndUIDesign;
}
