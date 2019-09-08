// ScrollWnd.cpp : implementation file
// Copy from \Microsoft Visual Studio 9.0\VC\atlmfc\src\mfc\viewscrl.cpp
//

#include "stdafx.h"
#include <afxpriv.h>
#if(_MSC_VER <= 1200) // VC6.0
#include <../src/afximpl.h>
#else
#include <../src/mfc/afximpl.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CPreviewView2 : public CPreviewView
{
	DECLARE_DYNCREATE(CPreviewView2)

	friend class CScrollWnd;
public:
	CPreviewView2() {}
	~CPreviewView2() {}

	virtual void OnDisplayPageNumber(UINT nPage, UINT nPagesDisplayed);

	afx_msg void OnPreviewPrint();
	DECLARE_MESSAGE_MAP()
};

IMPLEMENT_DYNCREATE(CPreviewView2, CPreviewView)

BEGIN_MESSAGE_MAP(CPreviewView2, CPreviewView)
	//{{AFX_MSG_MAP(CPreviewView)
	ON_COMMAND(AFX_ID_PREVIEW_PRINT, OnPreviewPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPreviewView2::OnDisplayPageNumber(UINT nPage, UINT nPagesDisplayed)
{
	UINT nEndPage = nPage + nPagesDisplayed - 1;

	CWinThread *pThread = AfxGetThread();
	ASSERT(pThread);
	CFrameWnd* pParent = (CFrameWnd*)pThread->m_pMainWnd;

	int nSubString = (nPagesDisplayed == 1) ? 0 : 1;

	CString s;
	BOOL bOK = AfxExtractSubString(s, m_pPreviewInfo->m_strPageDesc, nSubString);
	if (bOK)
	{
		TCHAR szBuf[80];
		int nResult;

		CString strBuf;
		if (nSubString == 0)
		{
			strBuf.Format(s, nPage);
			nResult = strBuf.GetLength();
//			nResult = _stprintf_s(szBuf, _countof(szBuf), s, nPage);
		}
		else
		{
			strBuf.Format(s, nPage, nEndPage);
			nResult = strBuf.GetLength();
//			nResult = _stprintf_s(szBuf, _countof(szBuf), s, nPage, nEndPage);
		}

		if( nResult > 0 )
		{
			pParent->SendMessage(WM_SETMESSAGESTRING, 0, (LPARAM)(LPVOID)szBuf);
		}
		else
		{
			bOK = FALSE;
		}
	}

	if(!bOK)
	{
		TRACE(_T("Malformed Page Description string. Could not get string %d.\n"), nSubString);
	}
}



void CPreviewView2::OnPreviewPrint()
{
	OnPreviewClose();               // force close of Preview

	// cause print (can be overridden by catching the command)
	CWnd* pMainWnd = AfxGetThread()->m_pMainWnd;
	ASSERT_VALID(pMainWnd);
	pMainWnd->SendMessage(WM_COMMAND, ID_FILE_PRINT);
}

//////////////////////////////////////////////////////////////////////////

CScrollWnd::CScrollWnd()
{
	m_crBk = RGB(192, 192, 192);
}

CScrollWnd::~CScrollWnd()
{
}


BEGIN_MESSAGE_MAP(CScrollWnd, CScrollView)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CScrollWnd drawing

void CScrollWnd::OnDraw(CDC* pDC)
{
	// TODO: add draw code here
}

int CScrollWnd::OnDrawBackground(CDC *pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	pDC->FillSolidRect(rcClient, m_crBk);

	return 0;
}

// OnEndPrintPreview is here for swap tuning reasons
//  (see viewprev.cpp for complete preview mode implementation)
void CScrollWnd::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo,
							  POINT, CPreviewView* pView)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pView);

// 	if (pView2->m_pPrintView != NULL)
// 		pView2->m_pPrintView->OnEndPrinting(pDC, pInfo);

// 	CWnd* pMainWnd = GetParentFrame();
// 	if (DYNAMIC_DOWNCAST(CFrameWnd, pMainWnd) == NULL)
// 	{
// 		// if it's not a frame, we'll try the main window
// 		pMainWnd = AfxGetMainWnd();
// 	}
//
// 	CFrameWnd* pParent = STATIC_DOWNCAST(CFrameWnd, pMainWnd);
// 	ASSERT_VALID(pParent);
//
// 	// restore the old main window
// 	pParent->OnSetPreviewMode(FALSE, pView->m_pPreviewState);
//
// 	// Force active view back to old one
// 	pParent->SetActiveView(pView->m_pPreviewState->pViewActiveOld);
// 	if (pParent != GetParentFrame())
// 		OnActivateView(TRUE, this, this);   // re-activate view in real frame
	pView->DestroyWindow();     // destroy preview view
	// C++ object will be deleted in PostNcDestroy

	// restore main frame layout and idle message
// 	pParent->RecalcLayout();
// 	pParent->SendMessage(WM_SETMESSAGESTRING, (WPARAM)AFX_IDS_IDLEMESSAGE, 0L);
// 	pParent->UpdateWindow();
}



// CScrollWnd diagnostics

#ifdef _DEBUG
void CScrollWnd::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CScrollWnd::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// public

BOOL CScrollWnd::Create(DWORD dwStyle, CRectCtrl *pRectCtrl, CWnd* pParentWnd, UINT nID)
{
	ASSERT(pRectCtrl != NULL && pRectCtrl->IsCreated());

	CRect rect;
	pRectCtrl->GetWindowRect(rect);

	BOOL bRet = CScrollView::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID);
	if (!bRet)
		return FALSE;

	pRectCtrl->AddBindWindow(this);

	return TRUE;
}

int CScrollWnd::SetBkColor(COLORREF crBk)
{
	m_crBk = crBk;

	return 0;
}

COLORREF CScrollWnd::GetBkColor() const
{
	return m_crBk;
}

BOOL CScrollWnd::DoPrintPreview2(UINT nIDResource, CView* pPrintView,
						   CRuntimeClass* pPreviewViewClass, CPrintPreviewState* pState)
{
	// Copy from VC6.0
	ASSERT_VALID_IDR(nIDResource);
	ASSERT_VALID(pPrintView);
	ASSERT(pPreviewViewClass != NULL);
	ASSERT(pPreviewViewClass->IsDerivedFrom(RUNTIME_CLASS(CPreviewView)));
	ASSERT(pState != NULL);

	// Hide main window
//	AfxGetMainWnd()->ShowWindow(SW_HIDE);

	// Show print preview window.
	CFrameWnd* pParent = new CFrameWnd;
	pParent->Create(NULL, NULL);
	pParent->ShowWindow(SW_SHOW);
	ASSERT_VALID(pParent);

	CCreateContext context;
	context.m_pCurrentFrame = pParent;
	context.m_pCurrentDoc = GetDocument();
	context.m_pLastView = this;

	// Create the preview view object
	CPreviewView2* pView = (CPreviewView2*)pPreviewViewClass->CreateObject();
	if (pView == NULL)
	{
		TRACE0("Error: Failed to create preview view.\n");
		return FALSE;
	}
	ASSERT_KINDOF(CPreviewView, pView);
	pView->m_pPreviewState = pState;        // save pointer

//	pParent->OnSetPreviewMode(TRUE, pState);    // Take over Frame Window

	// Create the toolbar from the dialog resource
	pView->m_pToolBar = new CDialogBar;
	if (!pView->m_pToolBar->Create(pParent, MAKEINTRESOURCE(nIDResource),
		CBRS_TOP, AFX_IDW_PREVIEW_BAR))
	{
		TRACE0("Error: Preview could not create toolbar dialog.\n");
		pParent->OnSetPreviewMode(FALSE, pState);   // restore Frame Window
		delete pView->m_pToolBar;       // not autodestruct yet
		pView->m_pToolBar = NULL;
		pView->m_pPreviewState = NULL;  // do not delete state structure
		delete pView;
		return FALSE;
	}
	pView->m_pToolBar->m_bAutoDelete = TRUE;    // automatic cleanup

	// Create the preview view as a child of the App Main Window.  This
	// is a sibling of this view if this is an SDI app.  This is NOT a sibling
	// if this is an MDI app.

	if (!pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0,0,0,0), pParent, AFX_IDW_PANE_FIRST, &context))
	{
		TRACE0("Error: couldn't create preview view for frame.\n");
		pParent->OnSetPreviewMode(FALSE, pState);   // restore Frame Window
		pView->m_pPreviewState = NULL;  // do not delete state structure
		delete pView;
		return FALSE;
	}

	// Preview window shown now

	pState->pViewActiveOld = pParent->GetActiveView();
	CView* pActiveView = pParent->GetActiveFrame()->GetActiveView();
// 	if (pActiveView != NULL)
// 		pActiveView->OnActivateView(FALSE, pActiveView, pActiveView);

	if (!pView->SetPrintView(pPrintView))
	{
		pView->OnPreviewClose();
		return TRUE;            // signal that OnEndPrintPreview was called
	}

	pParent->SetActiveView(pView);  // set active view - even for MDI

	// update toolbar and redraw everything
	pView->m_pToolBar->SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);
	pParent->RecalcLayout();            // position and size everything
	pParent->UpdateWindow();

	return TRUE;

/*	Copy from VS2008
	ASSERT_VALID_IDR(nIDResource);
	ASSERT_VALID(pPrintView);
	ASSERT(pPreviewViewClass != NULL);
	ASSERT(pPreviewViewClass->IsDerivedFrom(RUNTIME_CLASS(CPreviewView)));
	ASSERT(pState != NULL);

	CWnd* pMainWnd = GetParentFrame();
	if (DYNAMIC_DOWNCAST(CFrameWnd, pMainWnd) == NULL)
	{
		// if it's not a frame, we'll try the main window
		pMainWnd = AfxGetMainWnd();
	}

	CFrameWnd* pParent = (CFrameWnd *)pMainWnd;//STATIC_DOWNCAST(CFrameWnd, pMainWnd);
	ASSERT_VALID(pParent);

	CCreateContext context;
	context.m_pCurrentFrame = pParent;
	context.m_pCurrentDoc = GetDocument();
	context.m_pLastView = this;

	// Create the preview view object
	CPreviewView2 *pView = (CPreviewView2 *)pPreviewViewClass->CreateObject();
	if (pView == NULL)
	{
		TRACE(traceAppMsg, 0, "Error: Failed to create preview view.\n");
		return FALSE;
	}
	ASSERT_KINDOF(CPreviewView, pView);
	pView->m_pPreviewState = pState;        // save pointer

//	pParent->OnSetPreviewMode(TRUE, pState);    // Take over Frame Window

	// Create the toolbar from the dialog resource
	pView->m_pToolBar = new CDialogBar;

	CFrameWnd *pParentFrame = pParent->GetActiveFrame();
	ASSERT(pParentFrame);

	COleIPFrameWnd *pInPlaceFrame = DYNAMIC_DOWNCAST(COleIPFrameWnd, pParentFrame);
	if (pInPlaceFrame)
	{
		CDocument *pViewDoc = GetDocument();
		COleServerDoc *pDoc = DYNAMIC_DOWNCAST(COleServerDoc, pViewDoc);
		if (!pDoc)
		{
			pParent->OnSetPreviewMode(FALSE, pState);   // restore Frame Window
			delete pView->m_pToolBar;       // not autodestruct yet
			pView->m_pToolBar = NULL;
			pView->m_pPreviewState = NULL;  // do not delete state structure
			delete pView;
			return FALSE;
		}

		CFrameWnd *pFrame = (CFrameWnd*)pInPlaceFrame->GetDocFrame();
		if (!pFrame)
			pFrame = pInPlaceFrame->GetMainFrame();
		ASSERT(pFrame != NULL);

		// hide existing toolbars.
		pDoc->OnDocWindowActivate(FALSE);

		if ( pFrame && pView->m_pToolBar->Create(pFrame,
			ATL_MAKEINTRESOURCE(nIDResource),CBRS_TOP, AFX_IDW_PREVIEW_BAR) )
		{
			// automatic cleanup
			pView->m_pToolBar->m_bAutoDelete = TRUE;

			// Tell the toolbar where to route command messages
			pView->m_pToolBar->SetInPlaceOwner(pInPlaceFrame);

			// now, merge the print preview toolbar into the
			// appropriate frame which would be the document
			// frame for an mdi container or the app frame for
			// an sdi container.
			CRect rcBorder;
			BOOL bFrame = FALSE;
			CComPtr<IOleInPlaceUIWindow> spIPUIWindow;

			if (FAILED(pInPlaceFrame->GetInPlaceDocFrame(&spIPUIWindow)))
			{
				pInPlaceFrame->GetInPlaceFrame(&spIPUIWindow);
				bFrame = TRUE;
			}
			ASSERT(spIPUIWindow);
			if (spIPUIWindow)
			{
				spIPUIWindow->GetBorder(rcBorder);
				pDoc->OnResizeBorder(rcBorder, spIPUIWindow, bFrame);
			}
			pInPlaceFrame->SetPreviewMode(TRUE);
		}
		else
		{
			TRACE(traceAppMsg, 0, "Error: Preview could not create toolbar dialog.\n");
			pParent->OnSetPreviewMode(FALSE, pState);   // restore Frame Window
			delete pView->m_pToolBar;       // not autodestruct yet
			pView->m_pToolBar = NULL;
			pView->m_pPreviewState = NULL;  // do not delete state structure
			delete pView;
			return FALSE;
		}
	}
	else
	{
		if (!pView->m_pToolBar->Create(pParent, ATL_MAKEINTRESOURCE(nIDResource),
			CBRS_TOP, AFX_IDW_PREVIEW_BAR))
		{
			TRACE(traceAppMsg, 0, "Error: Preview could not create toolbar dialog.\n");
			pParent->OnSetPreviewMode(FALSE, pState);   // restore Frame Window
			delete pView->m_pToolBar;       // not autodestruct yet
			pView->m_pToolBar = NULL;
			pView->m_pPreviewState = NULL;  // do not delete state structure
			delete pView;
			return FALSE;
		}
		pView->m_pToolBar->m_bAutoDelete = TRUE;    // automatic cleanup
	}

	// Create the preview view as a child of the App Main Window.  This
	// is a sibling of this view if this is an SDI app.  This is NOT a sibling
	// if this is an MDI app.

	if (!pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0,0,0,0), pParent, AFX_IDW_PANE_FIRST, &context))
	{
		TRACE(traceAppMsg, 0, "Error: couldn't create preview view for frame.\n");
		pParent->OnSetPreviewMode(FALSE, pState);   // restore Frame Window
		pView->m_pPreviewState = NULL;  // do not delete state structure
		delete pView;
		return FALSE;
	}

	// Preview window shown now
	pState->pViewActiveOld = pParent->GetActiveView();
	CView* pActiveView = pParent->GetActiveFrame()->GetActiveView();

// 	if (pActiveView != NULL)
// 		pActiveView->OnActivateView(FALSE, pActiveView, pActiveView);

	if (!pView->SetPrintView(pPrintView))
	{
		pView->OnPreviewClose();
		return TRUE;            // signal that OnEndPrintPreview was called
	}

	pParent->SetActiveView(pView);  // set active view - even for MDI

	pView->m_pToolBar->SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);
	pParent->RecalcLayout();            // position and size everything
	pParent->UpdateWindow();

	return TRUE;*/
}


// CScrollWnd message handlers

int CScrollWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	return 0;
}

void CScrollWnd::OnPaint()
{
	// standard paint routine
	CPaintDC dc(this);
	OnPrepareDC(&dc);

	CRect rcClient;
	GetClientRect(rcClient);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	CBitmap *pbmpMemOld = dcMem.SelectObject(&bmpMem);

	// Draw content to dcMem...
	OnDraw(&dcMem);

	dcMem.SetBkColor(RGB(255, 0, 0));
	CPoint pt = dc.GetViewportOrg();
	dcMem.DrawText(_T("adfsf"), CRect(0, 100 - pt.y, 100, 200 - pt.y), DT_SINGLELINE | DT_TOP);

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(pbmpMemOld);
}

void CScrollWnd::OnFilePrintPreview2()
{
	// In derived classes, implement special window handling here
	// Be sure to Unhook Frame Window close if hooked.

	// must not create this on the frame.  Must outlive this function
	CPrintPreviewState* pState = new CPrintPreviewState;

	TRY
	{
		// DoPrintPreview's return value does not necessarily indicate that
		// Print preview succeeded or failed, but rather what actions are necessary
		// at this point.  If DoPrintPreview returns TRUE, it means that
		// OnEndPrintPreview will be (or has already been) called and the
		// pState structure will be/has been deleted.
		// If DoPrintPreview returns FALSE, it means that OnEndPrintPreview
		// WILL NOT be called and that cleanup, including deleting pState
		// must be done here.

		if (!DoPrintPreview2(AFX_IDD_PREVIEW_TOOLBAR, this,
			RUNTIME_CLASS(CPreviewView2), pState))
		{
			// In derived classes, reverse special window handling here for
			// Preview failure case

			TRACE(_T("Error: DoPrintPreview failed.\n"));
			AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
			delete pState;      // preview failed to initialize, delete State now
		}
	}
	CATCH_ALL(e)
	{
		delete pState;
		THROW_LAST();
	}
	END_CATCH_ALL
}

LRESULT CScrollWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	// When use layered window, the window not redraw when its child controls hide or display
	// to solve it, handle the WM_WINDOWPOSCHANGED message.
	BOOL bLayered = IsLayeredWindow(GetParent()->GetSafeHwnd());
	if (message == WM_WINDOWPOSCHANGED && bLayered)
	{
		WINDOWPOS *pos = (WINDOWPOS *)lParam;
		if (pos->hwnd == m_hWnd)
		{
			// When show the control, notify itself to redraw
			if ((pos->flags&SWP_SHOWWINDOW) == SWP_SHOWWINDOW)
			{
				Invalidate();
			}
			// When hide the control, notify its parent to redraw
			else if ((pos->flags&SWP_HIDEWINDOW) == SWP_HIDEWINDOW)
			{
				CRect rcThis;
				GetWindowRect(rcThis);
				GetParent()->ScreenToClient(rcThis);
				GetParent()->InvalidateRect(rcThis);
			}
		}
	}

	if (message == WM_ERASEBKGND)
	{
		return TRUE;
	}

	return CScrollView::WindowProc(message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////

// Special mapping modes just for CFormWnd implementation
#define MM_NONE             0
#define MM_SCALETOFIT       (-1)

#define AFX_CX_ANCHOR_BITMAP	32
#define AFX_CY_ANCHOR_BITMAP	32

#ifndef SM_MOUSEWHEELPRESENT
#define SM_MOUSEWHEELPRESENT 75
#endif

#ifndef SPI_GETWHEELSCROLLLINES
#define SPI_GETWHEELSCROLLLINES  104
#endif

UINT PASCAL _AfxIUIGetMouseScrollLines()
{
	static UINT uCachedScrollLines;

	static UINT msgGetScrollLines;
	static WORD nRegisteredMessage;

	// couldn't use the window -- try system settings
	uCachedScrollLines = 3; // reasonable default
	::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &uCachedScrollLines, 0);

	return uCachedScrollLines;
}

/////////////////////////////////////////////////////////////////////////////
//

#if(_MSC_VER > 1200) // VC6.0

#define ID_TIMER_TRACKING	0xE000

BEGIN_MESSAGE_MAP(_AFX_MOUSEANCHORWND2, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

_AFX_MOUSEANCHORWND2::_AFX_MOUSEANCHORWND2(CPoint& ptAnchor)
: m_ptAnchor(ptAnchor), m_bQuitTracking(FALSE)
{
}

_AFX_MOUSEANCHORWND2::~_AFX_MOUSEANCHORWND2()
{
}

BOOL _AFX_MOUSEANCHORWND2::PreTranslateMessage(MSG* pMsg)
{
#if(_MSC_VER > 1310) // VC2003
	ENSURE_ARG(pMsg != NULL);
#endif
	BOOL bRetVal = FALSE;

	switch (pMsg->message)
	{
		// any of these messages cause us to quit scrolling
	case WM_MOUSEWHEEL:
	case WM_KEYDOWN:
	case WM_CHAR:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
		m_bQuitTracking = TRUE;
		bRetVal = TRUE;
		break;

		// Button up message depend on the position of cursor
		// This enables the user to click and drag for a quick pan.
	case WM_MBUTTONUP:
		{
			CPoint pt(pMsg->lParam);
			ClientToScreen(&pt);
			if (!PtInRect(&m_rectDrag, pt))
				m_bQuitTracking = TRUE;
			bRetVal = TRUE;
		}
		break;
	}

	return bRetVal;
}

void _AFX_MOUSEANCHORWND2::OnTimer(UINT_PTR nIDEvent)
{
	ASSERT(nIDEvent == ID_TIMER_TRACKING);
	UNUSED(nIDEvent);

	int nCursor = -1;

	CPoint ptNow;
	GetCursorPos(&ptNow);

	CRect rectClient;
	GetWindowRect(&rectClient);

	// decide where the relative position of the cursor is
	// pick a cursor that points where we're going
	nCursor = 0;

	// if vertical scrolling allowed, consider vertical
	// directions for the cursor we'll show
	if (m_nAnchorID == AFX_IDC_MOUSE_ORG_HV || m_nAnchorID == AFX_IDC_MOUSE_ORG_VERT)
	{
		if (ptNow.y < rectClient.top)
			nCursor = AFX_IDC_MOUSE_PAN_N;
		else if (ptNow.y > rectClient.bottom)
			nCursor = AFX_IDC_MOUSE_PAN_S;
	}

	// if horizontal scrolling allowed, cosider horizontal
	// directions for the cursor, too. Only consider diagonal
	// if we can scroll both ways.
	if (m_nAnchorID == AFX_IDC_MOUSE_ORG_HV || m_nAnchorID == AFX_IDC_MOUSE_ORG_HORZ)
	{
		if (ptNow.x < rectClient.left)
		{
			if (nCursor == 0)
				nCursor = AFX_IDC_MOUSE_PAN_W;
			else if (m_nAnchorID == AFX_IDC_MOUSE_ORG_HV)
				nCursor--;
		}
		else if (ptNow.x > rectClient.right)
		{
			if (nCursor == 0)
				nCursor = AFX_IDC_MOUSE_PAN_E;
			else if (m_nAnchorID == AFX_IDC_MOUSE_ORG_HV)
				nCursor++;
		}
	}

	if (m_bQuitTracking)
	{
		// Someone knows we should stop playing with the mouse
		// kill the timer, quit capturing the mouse, clear the cursor,
		// destroy the window, and make sure that CFormWnd knows the
		// window isn't valid anymore.

		KillTimer(ID_TIMER_TRACKING);
		ReleaseCapture();
		SetCursor(NULL);
		CFormWnd* pView = (CFormWnd*) GetOwner();
		DestroyWindow();
		delete pView->m_pAnchorWindow;
		pView->m_pAnchorWindow = NULL;
	}
	else if (nCursor == 0)
	{
		// The cursor is over the anchor window; use a cursor that
		// looks like the anchor bitmap.
		SetCursor(m_hAnchorCursor);
	}
	else
	{
		// We're still actively tracking, so we need to find a cursor and
		// set it up.

#if(_MSC_VER == 1310) // VC2003
		HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(nCursor),
			RT_GROUP_CURSOR);
		HICON hCursor = ::LoadCursor(hInst, MAKEINTRESOURCE(nCursor));
#else
		HINSTANCE hInst = AfxFindResourceHandle(ATL_MAKEINTRESOURCE(nCursor),
			ATL_RT_GROUP_CURSOR);
		HICON hCursor = ::LoadCursor(hInst, ATL_MAKEINTRESOURCE(nCursor));
#endif
		ASSERT(hCursor != NULL);
		SetCursor(hCursor);

		// ask the view how much to scroll this time
		CSize sizeDistance; // = ptNow - rectClient.CenterPoint();

		if (ptNow.x > rectClient.right)
			sizeDistance.cx = ptNow.x - rectClient.right;
		else if (ptNow.x < rectClient.left)
			sizeDistance.cx = ptNow.x - rectClient.left;
		else
			sizeDistance.cx = 0;

		if (ptNow.y > rectClient.bottom)
			sizeDistance.cy = ptNow.y - rectClient.bottom;
		else if (ptNow.y < rectClient.top)
			sizeDistance.cy = ptNow.y - rectClient.top;
		else
			sizeDistance.cy = 0;

		CFormWnd* pView = (CFormWnd*) GetOwner();

		CSize sizeToScroll = pView->GetWheelScrollDistance(sizeDistance,
			m_nAnchorID == AFX_IDC_MOUSE_ORG_HV || m_nAnchorID == AFX_IDC_MOUSE_ORG_HORZ,
			m_nAnchorID == AFX_IDC_MOUSE_ORG_HV || m_nAnchorID == AFX_IDC_MOUSE_ORG_VERT );

		// hide ourselves to minimize flicker
		ShowWindow(SW_HIDE);

		CWnd* pViewParent = pView->GetParent();
		CSplitterWnd* pSplitter = DYNAMIC_DOWNCAST(CSplitterWnd, pViewParent);

		// if it is in a splitter, then we need to handle it accordingly
		if (pSplitter == NULL)
		{
			// scroll the view
			pView->OnScrollBy(sizeToScroll, TRUE);
		}
		else
		{
			// ask the splitter to scroll
//IUI			pSplitter->DoScrollBy(pView, sizeToScroll, TRUE);
		}

		// restore ourselves and repaint
		// SetFocus();
		UpdateWindow();

		// move ourselves back (since we're a child, we scroll too!)
		SetWindowPos(&CWnd::wndTop, m_ptAnchor.x - AFX_CX_ANCHOR_BITMAP/2, m_ptAnchor.y - AFX_CY_ANCHOR_BITMAP/2, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_SHOWWINDOW);
	}
}

void _AFX_MOUSEANCHORWND2::SetBitmap(UINT nID)
{
#if(_MSC_VER == 1310) // VC2003
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(nID), RT_GROUP_CURSOR);
	ASSERT(hInst != NULL);
	m_hAnchorCursor = ::LoadCursor(hInst, MAKEINTRESOURCE(nID));
#else
	HINSTANCE hInst = AfxFindResourceHandle(ATL_MAKEINTRESOURCE(nID), ATL_RT_GROUP_CURSOR);
	ASSERT(hInst != NULL);
	m_hAnchorCursor = ::LoadCursor(hInst, ATL_MAKEINTRESOURCE(nID));
#endif
	m_nAnchorID = nID;
}

BOOL _AFX_MOUSEANCHORWND2::Create(CFormWnd* pParent)
{
#if(_MSC_VER == 1310) // VC2003
	ASSERT(pParent != NULL);
#else
	ENSURE_VALID(pParent);
#endif
//	ASSERT_KINDOF(CFormWnd, pParent);

	pParent->ClientToScreen(&m_ptAnchor);

	m_rectDrag.top = m_ptAnchor.y - GetSystemMetrics(SM_CYDOUBLECLK);
	m_rectDrag.bottom = m_ptAnchor.y + GetSystemMetrics(SM_CYDOUBLECLK);
	m_rectDrag.left = m_ptAnchor.x - GetSystemMetrics(SM_CXDOUBLECLK);
	m_rectDrag.right = m_ptAnchor.x + GetSystemMetrics(SM_CXDOUBLECLK);

	BOOL bRetVal =
		CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
		AfxRegisterWndClass(CS_SAVEBITS),
		NULL,
		WS_POPUP,
		m_ptAnchor.x - AFX_CX_ANCHOR_BITMAP/2,
		m_ptAnchor.y - AFX_CY_ANCHOR_BITMAP/2,
		AFX_CX_ANCHOR_BITMAP, AFX_CY_ANCHOR_BITMAP,
		NULL, NULL);
	SetOwner(pParent);

	if (bRetVal)
	{
		// set window's region for round effect
		CRgn rgn;
		rgn.CreateEllipticRgn(0, 0, AFX_CX_ANCHOR_BITMAP, AFX_CY_ANCHOR_BITMAP);
		SetWindowRgn(rgn, TRUE);

		// fire timer ever 50ms
		SetCapture();
		SetTimer(ID_TIMER_TRACKING, 50, NULL);
	}
#ifdef _DEBUG
	else
	{
		DWORD dwLastError = GetLastError();
		TRACE(traceAppMsg, 0, "Failed to create mouse anchor window! Last error is 0x%8.8X\n",
			dwLastError);
	}
#endif

	return bRetVal;
}

void _AFX_MOUSEANCHORWND2::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);

	// shrink a pixel in every dimension for border
	rect.DeflateRect(1, 1, 1, 1);
	dc.Ellipse(rect);

	// draw anchor shape
	dc.DrawIcon(0, 0, m_hAnchorCursor);
}

#endif // VC6.0

// CFormWnd

CFormWnd::CFormWnd()
{
	// Init everything to zero
	m_totalLog.cx = m_totalLog.cy = 0;
	m_totalDev.cx = m_totalDev.cy = 0;
	m_pageDev.cx  = m_pageDev.cy  = 0;
	m_lineDev.cx  = m_lineDev.cy  = 0;

	m_bCenter = FALSE;
	m_bInsideUpdate = FALSE;

#if(_MSC_VER > 1200) // VC6.0
	m_pAnchorWindow = NULL;
#endif
	m_nMapMode = MM_NONE;
	m_bUseCustomScrollBar = FALSE;
	m_nHScrollBarHeight = 17;
	m_nVScrollBarWidth = 17;
}

CFormWnd::~CFormWnd()
{
#if(_MSC_VER > 1200) // VC6.0
	if (m_pAnchorWindow != NULL)
		m_pAnchorWindow->DestroyWindow();
	delete m_pAnchorWindow;
#endif
}


BEGIN_MESSAGE_MAP(CFormWnd, CUIWnd)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MBUTTONDOWN, HandleMButtonDown)
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CFormWnd painting

void CFormWnd::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(pDC);

#ifdef _DEBUG
	if (m_nMapMode == MM_NONE)
	{
#if(_MSC_VER <= 1200) // VC6.0
		TRACE0("[LibUIDK]===Error: must call SetScrollSizes() or SetScaleToFitSize()");
		TRACE0("\t[LibUIDK]===before painting scroll view.\n");
#else
		TRACE(traceAppMsg, 0, "Error: must call SetScrollSizes() or SetScaleToFitSize()");
		TRACE(traceAppMsg, 0, "\tbefore painting scroll view.\n");
#endif
		ASSERT(FALSE);
		return;
	}
#endif //_DEBUG
	ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);
	switch (m_nMapMode)
	{
	case MM_SCALETOFIT:
		pDC->SetMapMode(MM_ANISOTROPIC);
		pDC->SetWindowExt(m_totalLog);  // window is in logical coordinates
		pDC->SetViewportExt(m_totalDev);
		if (m_totalDev.cx == 0 || m_totalDev.cy == 0)
#if(_MSC_VER <= 1200) // VC6.0
			TRACE0("[LibUIDK]===Warning: CFormWnd scaled to nothing.\n");
#else
			TRACE(traceAppMsg, 0, "[LibUIDK]===Warning: CFormWnd scaled to nothing.\n");
#endif
		break;

	default:
		ASSERT(m_nMapMode > 0);
		pDC->SetMapMode(m_nMapMode);
		break;
	}

	CPoint ptVpOrg(0, 0);       // assume no shift for printing
	if (!pDC->IsPrinting())
	{
		ASSERT(pDC->GetWindowOrg() == CPoint(0,0));

		// by default shift viewport origin in negative direction of scroll
		ptVpOrg = -GetDeviceScrollPosition();

		if (m_bCenter)
		{
			CRect rect;
			GetClientRect(&rect);

			// if client area is larger than total device size,
			// override scroll positions to place origin such that
			// output is centered in the window
			if (m_totalDev.cx < rect.Width())
				ptVpOrg.x = (rect.Width() - m_totalDev.cx) / 2;
			if (m_totalDev.cy < rect.Height())
				ptVpOrg.y = (rect.Height() - m_totalDev.cy) / 2;
		}
	}
	pDC->SetViewportOrg(ptVpOrg);

//IUI	CUIWnd::OnPrepareDC(pDC, pInfo);     // For default Printing behavior
}

/////////////////////////////////////////////////////////////////////////////
// Set mode and scaling/scrolling sizes

void CFormWnd::SetScaleToFitSize(SIZE sizeTotal)
{
	// Note: It is possible to set sizeTotal members to negative values to
	//  effectively invert either the X or Y axis.

	ASSERT(m_hWnd != NULL);
	m_nMapMode = MM_SCALETOFIT;     // special internal value
	m_totalLog = sizeTotal;

	// reset and turn any scroll bars off
	if (m_hWnd != NULL && (GetStyle() & (WS_HSCROLL|WS_VSCROLL)))
	{
		SetScrollPos(SB_HORZ, 0);
		SetScrollPos(SB_VERT, 0);
		EnableScrollBarCtrl(SB_BOTH, FALSE);
		ASSERT((GetStyle() & (WS_HSCROLL|WS_VSCROLL)) == 0);
	}

	CRect rectT;
	GetClientRect(rectT);
	m_totalDev = rectT.Size();

	if (m_hWnd != NULL)
	{
		// window has been created, invalidate
		UpdateBars();
		Invalidate(TRUE);
	}
}

const AFX_DATADEF SIZE CFormWnd::sizeDefault = {0,0};

void CFormWnd::SetScrollSizes(int nMapMode, SIZE sizeTotal,
								 const SIZE& sizePage, const SIZE& sizeLine)
{
	ASSERT(sizeTotal.cx >= 0 && sizeTotal.cy >= 0);
	ASSERT(nMapMode > 0);
	ASSERT(nMapMode != MM_ISOTROPIC && nMapMode != MM_ANISOTROPIC);

	int nOldMapMode = m_nMapMode;
	m_nMapMode = nMapMode;
	m_totalLog = sizeTotal;

	//BLOCK: convert logical coordinate space to device coordinates
	{
		CWindowDC dc(NULL);
		ASSERT(m_nMapMode > 0);
		dc.SetMapMode(m_nMapMode);

		// total size
		m_totalDev = m_totalLog;
		dc.LPtoDP((LPPOINT)&m_totalDev);
		m_pageDev = sizePage;
		dc.LPtoDP((LPPOINT)&m_pageDev);
		m_lineDev = sizeLine;
		dc.LPtoDP((LPPOINT)&m_lineDev);
		if (m_totalDev.cy < 0)
			m_totalDev.cy = -m_totalDev.cy;
		if (m_pageDev.cy < 0)
			m_pageDev.cy = -m_pageDev.cy;
		if (m_lineDev.cy < 0)
			m_lineDev.cy = -m_lineDev.cy;
	} // release DC here

	// now adjust device specific sizes
	ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);
	if (m_pageDev.cx == 0)
		m_pageDev.cx = m_totalDev.cx / 10;
	if (m_pageDev.cy == 0)
		m_pageDev.cy = m_totalDev.cy / 10;
	if (m_lineDev.cx == 0)
		m_lineDev.cx = m_pageDev.cx / 10;
	if (m_lineDev.cy == 0)
		m_lineDev.cy = m_pageDev.cy / 10;

	if (m_hWnd != NULL)
	{
		// window has been created, invalidate now
		UpdateBars();
		if (nOldMapMode != m_nMapMode)
			Invalidate(TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Getting information

CPoint CFormWnd::GetScrollPosition() const   // logical coordinates
{
	if (m_nMapMode == MM_SCALETOFIT)
	{
		return CPoint(0, 0);    // must be 0,0
	}

	CPoint pt = GetDeviceScrollPosition();

	if (m_bCenter)
	{
		CRect rect;
		GetClientRect(&rect);

		// if client area is larger than total device size,
		// the scroll positions are overridden to place origin such that
		// output is centered in the window
		// GetDeviceScrollPosition() must reflect this

		if (m_totalDev.cx < rect.Width())
			pt.x = -((rect.Width() - m_totalDev.cx) / 2);
		if (m_totalDev.cy < rect.Height())
			pt.y = -((rect.Height() - m_totalDev.cy) / 2);

	}
	// pt may be negative if m_bCenter is set

	if (m_nMapMode != MM_TEXT)
	{
		ASSERT(m_nMapMode > 0); // must be set
		CWindowDC dc(NULL);
		dc.SetMapMode(m_nMapMode);
		dc.DPtoLP((LPPOINT)&pt);
	}
	return pt;
}

void CFormWnd::ScrollToPosition(POINT pt)    // logical coordinates
{
	ASSERT(m_nMapMode > 0);     // not allowed for shrink to fit
	if (m_nMapMode != MM_TEXT)
	{
		CWindowDC dc(NULL);
		dc.SetMapMode(m_nMapMode);
		dc.LPtoDP((LPPOINT)&pt);
	}

	// now in device coordinates - limit if out of range
	int xMax = GetScrollLimit(SB_HORZ);
	int yMax = GetScrollLimit(SB_VERT);
	if (pt.x < 0)
		pt.x = 0;
	else if (pt.x > xMax)
		pt.x = xMax;
	if (pt.y < 0)
		pt.y = 0;
	else if (pt.y > yMax)
		pt.y = yMax;

	ScrollToDevicePosition(pt);
}

CPoint CFormWnd::GetDeviceScrollPosition() const
{
	CPoint pt(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
	ASSERT(pt.x >= 0 && pt.y >= 0);
	return pt;
}

void CFormWnd::GetDeviceScrollSizes(int& nMapMode, SIZE& sizeTotal,
									   SIZE& sizePage, SIZE& sizeLine) const
{
	if (m_nMapMode <= 0)
#if(_MSC_VER <= 1200) // VC6.0
		TRACE0("[LibUIDK]===Warning: CFormWnd::GetDeviceScrollSizes returning invalid mapping mode.\n");
#else
		TRACE(traceAppMsg, 0, "Warning: CFormWnd::GetDeviceScrollSizes returning invalid mapping mode.\n");
#endif
	nMapMode = m_nMapMode;
	sizeTotal = m_totalDev;
	sizePage = m_pageDev;
	sizeLine = m_lineDev;
}

void CFormWnd::ScrollToDevicePosition(POINT ptDev)
{
	ASSERT(ptDev.x >= 0);
	ASSERT(ptDev.y >= 0);

	// Note: ScrollToDevicePosition can and is used to scroll out-of-range
	//  areas as far as CFormWnd is concerned -- specifically in
	//  the print-preview code.  Since OnScrollBy makes sure the range is
	//  valid, ScrollToDevicePosition does not vector through OnScrollBy.

	int xOrig = GetScrollPos(SB_HORZ);
	SetScrollPos(SB_HORZ, ptDev.x);
	int yOrig = GetScrollPos(SB_VERT);
	SetScrollPos(SB_VERT, ptDev.y);
	ScrollWindow(xOrig - ptDev.x, yOrig - ptDev.y);
}

CSize CFormWnd::GetWheelScrollDistance(CSize sizeDistance, BOOL bHorz, BOOL bVert)
{
	CSize sizeRet;

	if (bHorz)
		sizeRet.cx = sizeDistance.cx / 10;
	else
		sizeRet.cx = 0;

	if (bVert)
		sizeRet.cy = sizeDistance.cy / 10;
	else
		sizeRet.cy = 0;

	return sizeRet;
}

/////////////////////////////////////////////////////////////////////////////
// Other helpers

void CFormWnd::FillOutsideRect(CDC* pDC, CBrush* pBrush)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pBrush);

	// fill rect outside the image
	CRect rect;
	GetClientRect(rect);
	ASSERT(rect.left == 0 && rect.top == 0);
	rect.left = m_totalDev.cx;
	if (!rect.IsRectEmpty())
		pDC->FillRect(rect, pBrush);    // vertical strip along the side
	rect.left = 0;
	rect.right = m_totalDev.cx;
	rect.top = m_totalDev.cy;
	if (!rect.IsRectEmpty())
		pDC->FillRect(rect, pBrush);    // horizontal strip along the bottom
}

void CFormWnd::ResizeParentToFit(BOOL bShrinkOnly)
{
	// adjust parent rect so client rect is appropriate size
	ASSERT(m_nMapMode != MM_NONE);  // mapping mode must be known

	// determine current size of the client area as if no scrollbars present
	CRect rectClient;
	GetWindowRect(rectClient);
	CRect rect = rectClient;
	CalcWindowRect(rect);
	rectClient.left += rectClient.left - rect.left;
	rectClient.top += rectClient.top - rect.top;
	rectClient.right -= rect.right - rectClient.right;
	rectClient.bottom -= rect.bottom - rectClient.bottom;
	rectClient.OffsetRect(-rectClient.left, -rectClient.top);
	ASSERT(rectClient.left == 0 && rectClient.top == 0);

	// determine desired size of the view
	CRect rectView(0, 0, m_totalDev.cx, m_totalDev.cy);
	if (bShrinkOnly)
	{
		if (rectClient.right <= m_totalDev.cx)
			rectView.right = rectClient.right;
		if (rectClient.bottom <= m_totalDev.cy)
			rectView.bottom = rectClient.bottom;
	}
	CalcWindowRect(rectView, CWnd::adjustOutside);
	rectView.OffsetRect(-rectView.left, -rectView.top);
	ASSERT(rectView.left == 0 && rectView.top == 0);
	if (bShrinkOnly)
	{
		if (rectClient.right <= m_totalDev.cx)
			rectView.right = rectClient.right;
		if (rectClient.bottom <= m_totalDev.cy)
			rectView.bottom = rectClient.bottom;
	}

	// dermine and set size of frame based on desired size of view
	CRect rectFrame;
#if(_MSC_VER <= 1310) // VC2003
	CIUIFrameWnd* pFrame = GetParentFrame();
#else
	CFrameWnd* pFrame = EnsureParentFrame();
	ASSERT_VALID(pFrame);
#endif
	pFrame->GetWindowRect(rectFrame);
	CSize size = rectFrame.Size();
	size.cx += rectView.right - rectClient.right;
	size.cy += rectView.bottom - rectClient.bottom;
	pFrame->SetWindowPos(NULL, 0, 0, size.cx, size.cy,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

/////////////////////////////////////////////////////////////////////////////

void CFormWnd::OnSize(UINT nType, int cx, int cy)
{
	CUIWnd::OnSize(nType, cx, cy);
	if (m_nMapMode == MM_SCALETOFIT)
	{
		// force recalculation of scale to fit parameters
		SetScaleToFitSize(m_totalLog);
	}
	else
	{
		// UpdateBars() handles locking out recursion
		UpdateBars();
	}
}

LRESULT CFormWnd::HandleMButtonDown(WPARAM wParam, LPARAM lParam)
{
	UINT nFlags = static_cast<UINT>(wParam);
	CPoint point(lParam);

	// if the user has CTRL or SHIFT down, we certainly
	// do not handle the message

	if (nFlags & (MK_SHIFT | MK_CONTROL))
	{
		CUIWnd::OnMButtonDown(nFlags, point);
		return FALSE;
	}

	// Otherwise, we actually have scrolling work to do.
	// See if we have a wheel mouse...

	BOOL bSupport = ::GetSystemMetrics(SM_MOUSEWHEELPRESENT);

	// If not a wheel mouse, the middle button is really the
	// middle button and not the wheel; the application should've
	//	handled it.

	if (!bSupport)
		CUIWnd::OnMButtonDown(nFlags, point);
	else
	{
#if(_MSC_VER > 1200) // VC6.0
		if (m_pAnchorWindow == NULL)
		{
			BOOL bVertBar;
			BOOL bHorzBar;
			CheckScrollBars(bHorzBar, bVertBar);

			UINT nBitmapID = 0;

			// based on which scrollbars we have, figure out which
			// anchor cursor to use for the anchor window
			if (bVertBar)
			{
				if (bHorzBar)
					nBitmapID = AFX_IDC_MOUSE_ORG_HV;
				else
					nBitmapID = AFX_IDC_MOUSE_ORG_VERT;
			}
			else if (bHorzBar)
				nBitmapID = AFX_IDC_MOUSE_ORG_HORZ;

			// if there's no scrollbars, we don't do anything!
			if (nBitmapID == 0)
			{
				CUIWnd::OnMButtonDown(nFlags, point);
				return FALSE;
			}
			else
			{
				m_pAnchorWindow = new _AFX_MOUSEANCHORWND2(point);
				m_pAnchorWindow->SetBitmap(nBitmapID);
				m_pAnchorWindow->Create(this);
				m_pAnchorWindow->ShowWindow(SW_SHOWNA);
			}
		}
		else
		{
			m_pAnchorWindow->DestroyWindow();
			delete m_pAnchorWindow;
			m_pAnchorWindow = NULL;
		}
#endif // VC6.0
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Scrolling Helpers

void CFormWnd::CenterOnPoint(CPoint ptCenter) // center in device coords
{
	CRect rect;
	GetClientRect(&rect);           // find size of client window

	int xDesired = ptCenter.x - rect.Width() / 2;
	int yDesired = ptCenter.y - rect.Height() / 2;

	DWORD dwStyle = GetStyle();

	if ((dwStyle & WS_HSCROLL) == 0 || xDesired < 0)
	{
		xDesired = 0;
	}
	else
	{
		int xMax = GetScrollLimit(SB_HORZ);
		if (xDesired > xMax)
			xDesired = xMax;
	}

	if ((dwStyle & WS_VSCROLL) == 0 || yDesired < 0)
	{
		yDesired = 0;
	}
	else
	{
		int yMax = GetScrollLimit(SB_VERT);
		if (yDesired > yMax)
			yDesired = yMax;
	}

	ASSERT(xDesired >= 0);
	ASSERT(yDesired >= 0);

	SetScrollPos(SB_HORZ, xDesired);
	SetScrollPos(SB_VERT, yDesired);
}

/////////////////////////////////////////////////////////////////////////////
// Tie to scrollbars and CWnd behaviour

void CFormWnd::GetScrollBarSizes(CSize& sizeSb)
{
	sizeSb.cx = sizeSb.cy = 0;
	DWORD dwStyle = GetStyle();

	if (GetScrollBarCtrl(SB_VERT) == NULL)
	{
		// vert scrollbars will impact client area of this window
		sizeSb.cx = afxData.cxVScroll;
		if (dwStyle & WS_BORDER)
#if(_MSC_VER <= 1400) // VC2005
			sizeSb.cx -= CX_BORDER;
#else
			sizeSb.cx -= AFX_CX_BORDER;
#endif
	}
	if (GetScrollBarCtrl(SB_HORZ) == NULL)
	{
		// horz scrollbars will impact client area of this window
		sizeSb.cy = afxData.cyHScroll;
		if (dwStyle & WS_BORDER)
#if(_MSC_VER <= 1400) // VC2005
			sizeSb.cy -= CY_BORDER;
#else
			sizeSb.cy -= AFX_CY_BORDER;
#endif
	}
}

BOOL CFormWnd::GetTrueClientSize(CSize& size, CSize& sizeSb)
// return TRUE if enough room to add scrollbars if needed
{
	CRect rect;
	GetClientRect(&rect);
	ASSERT(rect.top == 0 && rect.left == 0);
	size.cx = rect.right;
	size.cy = rect.bottom;
	DWORD dwStyle = GetStyle();

	// first get the size of the scrollbars for this window
	GetScrollBarSizes(sizeSb);

	// first calculate the size of a potential scrollbar
	// (scroll bar controls do not get turned on/off)
	if (sizeSb.cx != 0 && (dwStyle & WS_VSCROLL))
	{
		// vert scrollbars will impact client area of this window
		size.cx += sizeSb.cx;   // currently on - adjust now
	}
	if (sizeSb.cy != 0 && (dwStyle & WS_HSCROLL))
	{
		// horz scrollbars will impact client area of this window
		size.cy += sizeSb.cy;   // currently on - adjust now
	}

	// return TRUE if enough room
	return (size.cx > sizeSb.cx && size.cy > sizeSb.cy);
}

// helper to return the state of the scrollbars without actually changing
//  the state of the scrollbars
void CFormWnd::GetScrollBarState(CSize sizeClient, CSize& needSb,
									CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient)
{
	// get scroll bar sizes (the part that is in the client area)
	CSize sizeSb;
	GetScrollBarSizes(sizeSb);

	// enough room to add scrollbars
	sizeRange = m_totalDev - sizeClient;
	// > 0 => need to scroll
	ptMove = GetDeviceScrollPosition();
	// point to move to (start at current scroll pos)

	BOOL bNeedH = sizeRange.cx > 0;
	if (!bNeedH)
		ptMove.x = 0;                       // jump back to origin
	else if (bInsideClient)
		sizeRange.cy += sizeSb.cy;          // need room for a scroll bar

	BOOL bNeedV = sizeRange.cy > 0;
	if (!bNeedV)
		ptMove.y = 0;                       // jump back to origin
	else if (bInsideClient)
		sizeRange.cx += sizeSb.cx;          // need room for a scroll bar

	if (bNeedV && !bNeedH && sizeRange.cx > 0)
	{
		ASSERT(bInsideClient);
		// need a horizontal scrollbar after all
		bNeedH = TRUE;
		sizeRange.cy += sizeSb.cy;
	}

	// if current scroll position will be past the limit, scroll to limit
	if (sizeRange.cx > 0 && ptMove.x >= sizeRange.cx)
		ptMove.x = sizeRange.cx;
	if (sizeRange.cy > 0 && ptMove.y >= sizeRange.cy)
		ptMove.y = sizeRange.cy;

	// now update the bars as appropriate
	needSb.cx = bNeedH;
	needSb.cy = bNeedV;

	// needSb, sizeRange, and ptMove area now all updated
}

LRESULT CFormWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_LBUTTONDOWN
		|| message == WM_LBUTTONDBLCLK)
	{
		SetFocus();
	}

	if (message == WM_SHOWWINDOW)
	{
		BOOL bShow = BOOL(wParam);
		if (bShow)
		{
			UpdateCustomScrollBar();
		}
		else
		{
			if (IsWindow(m_wndHScroll.GetSafeHwnd()))
			{
				m_wndHScroll.ShowWindow(SW_HIDE);
			}

			if (IsWindow(m_wndVScroll.GetSafeHwnd()))
			{
				m_wndVScroll.ShowWindow(SW_HIDE);
			}
		}
	}
	if (message == WM_SIZE || message == WM_MOVE)
	{
		LRESULT lr = CUIWnd::WindowProc(message, wParam, lParam);
		UpdateCustomScrollBar();
		return lr;
	}

	if (message == WM_PAINT)
	{
		CPaintDC dc(this);
		OnDraw(&dc);

		UpdateCustomScrollBar();
	}

	return CUIWnd::WindowProc(message, wParam, lParam);
}

void CFormWnd::UpdateBars()
{
	// UpdateBars may cause window to be resized - ignore those resizings
	if (m_bInsideUpdate)
		return;         // Do not allow recursive calls

	// Lock out recursion
	m_bInsideUpdate = TRUE;

	// update the horizontal to reflect reality
	// NOTE: turning on/off the scrollbars will cause 'OnSize' callbacks
	ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);

	CRect rectClient;
	BOOL bCalcClient = TRUE;

	// allow parent to do inside-out layout first
	CWnd* pParentWnd = GetParent();
	if (pParentWnd != NULL)
	{
		// if parent window responds to this message, use just
		//  client area for scroll bar calc -- not "true" client area
		if ((BOOL)pParentWnd->SendMessage(WM_RECALCPARENT, 0, (LPARAM)(LPCRECT)&rectClient) != 0)
		{
			// use rectClient instead of GetTrueClientSize for
			//  client size calculation.
			bCalcClient = FALSE;
		}
	}

	CSize sizeClient;
	CSize sizeSb;

	if (bCalcClient)
	{
		// get client rect
		if (!GetTrueClientSize(sizeClient, sizeSb))
		{
			// no room for scroll bars (common for zero sized elements)
			CRect rect;
			GetClientRect(&rect);
			if (rect.right > 0 && rect.bottom > 0)
			{
				// if entire client area is not invisible, assume we have
				//  control over our scrollbars
				EnableScrollBarCtrl(SB_BOTH, FALSE);
			}
			m_bInsideUpdate = FALSE;
			return;
		}
	}
	else
	{
		// let parent window determine the "client" rect
		GetScrollBarSizes(sizeSb);
		sizeClient.cx = rectClient.right - rectClient.left;
		sizeClient.cy = rectClient.bottom - rectClient.top;
	}

	// enough room to add scrollbars
	CSize sizeRange;
	CPoint ptMove;
	CSize needSb;

	// get the current scroll bar state given the true client area
	GetScrollBarState(sizeClient, needSb, sizeRange, ptMove, bCalcClient);
	if (needSb.cx)
		sizeClient.cy -= sizeSb.cy;
	if (needSb.cy)
		sizeClient.cx -= sizeSb.cx;

	// first scroll the window as needed
	ScrollToDevicePosition(ptMove); // will set the scroll bar positions too

	// this structure needed to update the scrollbar page range
	SCROLLINFO info;
	info.fMask = SIF_PAGE|SIF_RANGE;
	info.nMin = 0;

	// now update the bars as appropriate
	EnableScrollBarCtrl(SB_HORZ, needSb.cx);
	if (needSb.cx)
	{
		info.nPage = sizeClient.cx;
		info.nMax = m_totalDev.cx-1;
		if (!SetScrollInfo(SB_HORZ, &info, TRUE))
			SetScrollRange(SB_HORZ, 0, sizeRange.cx, TRUE);
	}
	EnableScrollBarCtrl(SB_VERT, needSb.cy);
	if (needSb.cy)
	{
		info.nPage = sizeClient.cy;
		info.nMax = m_totalDev.cy-1;
		if (!SetScrollInfo(SB_VERT, &info, TRUE))
			SetScrollRange(SB_VERT, 0, sizeRange.cy, TRUE);
	}

	// remove recursion lockout
	m_bInsideUpdate = FALSE;
}

void CFormWnd::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
	if (nAdjustType == adjustOutside)
	{
		// allow for special client-edge style
		::AdjustWindowRectEx(lpClientRect, 0, FALSE, GetExStyle());

		if (m_nMapMode != MM_SCALETOFIT)
		{
			// if the view is being used in-place, add scrollbar sizes
			//  (scollbars should appear on the outside when in-place editing)
			CSize sizeClient(
				lpClientRect->right - lpClientRect->left,
				lpClientRect->bottom - lpClientRect->top);

			CSize sizeRange = m_totalDev - sizeClient;
			// > 0 => need to scroll

			// get scroll bar sizes (used to adjust the window)
			CSize sizeSb;
			GetScrollBarSizes(sizeSb);

			// adjust the window size based on the state
			if (sizeRange.cy > 0)
			{   // vertical scroll bars take up horizontal space
				lpClientRect->right += sizeSb.cx;
			}
			if (sizeRange.cx > 0)
			{   // horizontal scroll bars take up vertical space
				lpClientRect->bottom += sizeSb.cy;
			}
		}
	}
	else
	{
		// call default to handle other non-client areas
		::AdjustWindowRectEx(lpClientRect, GetStyle(), FALSE,
			GetExStyle() & ~(WS_EX_CLIENTEDGE));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFormWnd scrolling

void CFormWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;     // eat it

	// ignore scroll bar msgs from other controls
	if (m_bUseCustomScrollBar)
	{
		if (pScrollBar->GetSafeHwnd() != m_wndHScroll.GetSafeHwnd())
		{
			return;
		}
	}
	else
	{
		if (pScrollBar != GetScrollBarCtrl(SB_HORZ))
		{
			return;
		}
	}

	// Sine nPos only has 16 bit valid, use GetScrollInfo to get track pos.
	SCROLLINFO si;
	if (m_bUseCustomScrollBar)
	{
		m_wndHScroll.GetScrollInfo(&si, SIF_TRACKPOS);
	}
	else
	{
		GetScrollInfo(SB_HORZ, &si, SIF_TRACKPOS);
	}
	OnScroll(MAKEWORD(nSBCode, 0xff), si.nTrackPos);
}

void CFormWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;     // eat it

	// ignore scroll bar msgs from other controls
	if (m_bUseCustomScrollBar)
	{
		if (pScrollBar->GetSafeHwnd() != m_wndVScroll.GetSafeHwnd())
		{
			return;
		}
	}
	else
	{
		if (pScrollBar != GetScrollBarCtrl(SB_VERT))
		{
			return;
		}
	}

	// Sine nPos only has 16 bit valid, use GetScrollInfo to get track pos.
	SCROLLINFO si;
	if (m_bUseCustomScrollBar)
	{
		m_wndVScroll.GetScrollInfo(&si, SIF_TRACKPOS);
	}
	else
	{
		GetScrollInfo(SB_VERT, &si, SIF_TRACKPOS);
	}
	OnScroll(MAKEWORD(0xff, nSBCode), si.nTrackPos);
}

BOOL CFormWnd::OnMouseWheel(UINT fFlags, short zDelta, CPoint point)
{
	// we don't handle anything but scrolling
	if (fFlags & (MK_SHIFT | MK_CONTROL))
		return FALSE;

	//IUI
// 	// if the parent is a splitter, it will handle the message
// 	if (GetParentSplitter(this, TRUE))
// 		return FALSE;

	// we can't get out of it--perform the scroll ourselves
	return DoMouseWheel(fFlags, zDelta, point);
}

void CFormWnd::CheckScrollBars(BOOL& bHasHorzBar, BOOL& bHasVertBar) const
{
	DWORD dwStyle = GetStyle();
	CScrollBar* pBar = GetScrollBarCtrl(SB_VERT);
	bHasVertBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
		(dwStyle & WS_VSCROLL);

	pBar = GetScrollBarCtrl(SB_HORZ);
	bHasHorzBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
		(dwStyle & WS_HSCROLL);
}

// This function isn't virtual. If you need to override it,
// you really need to override OnMouseWheel() here or in
// CSplitterWnd.

BOOL CFormWnd::DoMouseWheel(UINT fFlags, short zDelta, CPoint point)
{
	UNUSED_ALWAYS(point);
	UNUSED_ALWAYS(fFlags);

	// if we have a vertical scroll bar, the wheel scrolls that
	// if we have _only_ a horizontal scroll bar, the wheel scrolls that
	// otherwise, don't do any work at all

	BOOL bHasHorzBar, bHasVertBar;
	CheckScrollBars(bHasHorzBar, bHasVertBar);
	if (!bHasVertBar && !bHasHorzBar)
		return FALSE;

	BOOL bResult = FALSE;
	UINT uWheelScrollLines = _AfxIUIGetMouseScrollLines();
	int nToScroll = ::MulDiv(-zDelta, uWheelScrollLines, WHEEL_DELTA);
	int nDisplacement;

	if (bHasVertBar)
	{
		if (uWheelScrollLines == WHEEL_PAGESCROLL)
		{
			nDisplacement = m_pageDev.cy;
			if (zDelta > 0)
				nDisplacement = -nDisplacement;
		}
		else
		{
			nDisplacement = nToScroll * m_lineDev.cy;
			nDisplacement = min(nDisplacement, m_pageDev.cy);
		}
		bResult = OnScrollBy(CSize(0, nDisplacement), TRUE);
	}
	else if (bHasHorzBar)
	{
		if (uWheelScrollLines == WHEEL_PAGESCROLL)
		{
			nDisplacement = m_pageDev.cx;
			if (zDelta > 0)
				nDisplacement = -nDisplacement;
		}
		else
		{
			nDisplacement = nToScroll * m_lineDev.cx;
			nDisplacement = min(nDisplacement, m_pageDev.cx);
		}
		bResult = OnScrollBy(CSize(nDisplacement, 0), TRUE);
	}

	if (bResult)
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
	}

	return bResult;
}

int CFormWnd::UseCustomScrollBar(BOOL bUseCustomScrollBar)
{
	m_bUseCustomScrollBar = bUseCustomScrollBar;

	if (m_bUseCustomScrollBar)
	{
		CWnd *pParent = GetParent();

		BOOL bRet = m_wndHScroll.Create(SBS_HORZ | WS_CHILD, CRect(0, 0, 1, 1), pParent, 5656);
		if (!bRet)
		{
			return -1;
		}
		m_wndHScroll.SetNotifyWnd(this);

		bRet = m_wndVScroll.Create(SBS_VERT | WS_CHILD, CRect(0, 0, 0, 0), pParent, 2);
		if (!bRet)
		{
			return -1;
		}
		m_wndVScroll.SetNotifyWnd(this);
	}

	return 0;
}

// update scroll information and position
int CFormWnd::UpdateCustomScrollBar()
{
	if (m_wndHScroll.GetSafeHwnd() == NULL
		|| m_wndVScroll.GetSafeHwnd() == NULL)
	{
		return -1;
	}

	BOOL bRet = FALSE;
	int nVScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
	int nHScrollBarHeight = GetSystemMetrics(SM_CYHSCROLL);
	// Ver
	SCROLLINFO siVert;
	memset(&siVert, 0, sizeof(SCROLLINFO));
	siVert.cbSize = sizeof(SCROLLINFO);
	siVert.fMask = SIF_ALL;
	bRet = GetScrollInfo(SB_VERT, &siVert);

	BOOL bShowVScroll = FALSE;
	if (int(siVert.nPage) < (siVert.nMax + 1) && bRet && siVert.nPage != 0)
	{
		bShowVScroll = TRUE;
	}

	// Horz
	SCROLLINFO siHorz;
	memset(&siHorz, 0, sizeof(SCROLLINFO));
	siHorz.cbSize = sizeof(SCROLLINFO);
	siHorz.fMask = SIF_ALL;
	bRet = GetScrollInfo(SB_HORZ, &siHorz);

	BOOL bShowHScroll = FALSE;
	if (int(siHorz.nPage) < (siHorz.nMax + 1) && bRet && siHorz.nPage != 0)
	{
		bShowHScroll = TRUE;
	}

	// Show scroll bar
	BOOL bNotifyWndShow = FALSE;
	LONG lStyle = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
	if ((lStyle & WS_VISIBLE) == WS_VISIBLE)
	{
		bNotifyWndShow = TRUE;
	}

	if (bShowVScroll && bShowHScroll)
	{
		CRect rect;
		GetWindowRect(rect);

		CRgn rgnV;
		rgnV.CreateRectRgn(rect.Width() - nVScrollBarWidth, 0, rect.Width(), rect.Height() - nHScrollBarHeight);
		CRgn rgnH;
		rgnH.CreateRectRgn(0, rect.Height() - nHScrollBarHeight, rect.Width() - nVScrollBarWidth, rect.Height());
		CRgn rgnAll;
		rgnAll.CreateRectRgn(0, 0, rect.Width(), rect.Height());
		rgnAll.CombineRgn(&rgnAll, &rgnV, RGN_DIFF);
		rgnAll.CombineRgn(&rgnAll, &rgnH, RGN_DIFF);
		SetWindowRgn(HRGN(rgnAll.GetSafeHandle()), TRUE);

		GetParent()->ScreenToClient(rect);
		m_wndVScroll.MoveWindow(rect.right - nVScrollBarWidth, rect.top, m_nVScrollBarWidth, rect.Height() - nHScrollBarHeight);
		if (bNotifyWndShow)
		{
			m_wndVScroll.ShowWindow(SW_SHOW);
		}

		m_wndHScroll.MoveWindow(rect.left, rect.bottom - nHScrollBarHeight, rect.Width() - nVScrollBarWidth, m_nHScrollBarHeight);
		if (bNotifyWndShow)
		{
			m_wndHScroll.ShowWindow(SW_SHOW);
		}
	}
	else if (bShowVScroll)
	{
		CRect rect;
		GetWindowRect(rect);

		CRgn rgnV;
		rgnV.CreateRectRgn(rect.Width() - nVScrollBarWidth, 0, rect.Width(), rect.Height());
		CRgn rgnAll;
		rgnAll.CreateRectRgn(0, 0, rect.Width(), rect.Height());
		rgnAll.CombineRgn(&rgnAll, &rgnV, RGN_DIFF);
		SetWindowRgn(HRGN(rgnAll.GetSafeHandle()), TRUE);

		GetParent()->ScreenToClient(rect);
		m_wndVScroll.MoveWindow(rect.right - nVScrollBarWidth, rect.top, m_nVScrollBarWidth, rect.Height());
		if (bNotifyWndShow)
		{
			m_wndVScroll.ShowWindow(SW_SHOW);
		}
		m_wndHScroll.ShowWindow(SW_HIDE);
	}
	else if (bShowHScroll)
	{
		CRect rect;
		GetWindowRect(rect);

		CRgn rgnH;
		rgnH.CreateRectRgn(0, rect.Height() - nHScrollBarHeight, rect.Width(), rect.Height());
		CRgn rgnAll;
		rgnAll.CreateRectRgn(0, 0, rect.Width(), rect.Height());
		rgnAll.CombineRgn(&rgnAll, &rgnH, RGN_DIFF);
		SetWindowRgn(HRGN(rgnAll.GetSafeHandle()), TRUE);

		GetParent()->ScreenToClient(rect);
		m_wndHScroll.MoveWindow(rect.left, rect.bottom - nHScrollBarHeight, rect.Width(), m_nHScrollBarHeight);
		if (bNotifyWndShow)
		{
			m_wndHScroll.ShowWindow(SW_SHOW);
		}
		m_wndVScroll.ShowWindow(SW_HIDE);
	}
	else
	{
		m_wndVScroll.ShowWindow(SW_HIDE);
		m_wndHScroll.ShowWindow(SW_HIDE);

		CRect rect;
		GetWindowRect(rect);

		CRgn rgnAll;
		rgnAll.CreateRectRgn(0, 0, rect.Width(), rect.Height());
		SetWindowRgn(HRGN(rgnAll.GetSafeHandle()), TRUE);
	}

	m_wndHScroll.SetScrollInfo(&siHorz);
	m_wndVScroll.SetScrollInfo(&siVert);

	return 0;
}

int CFormWnd::SetHScrollBarHeight(int nHeight)
{
	m_nHScrollBarHeight = nHeight;

	return 0;
}

int CFormWnd::GetHScrollBarHeight() const
{
	return m_nHScrollBarHeight;
}

int CFormWnd::SetVScrollBarWidth(int nWidth)
{
	m_nVScrollBarWidth = nWidth;

	return 0;
}

int CFormWnd::GetVScrollBarWidth() const
{
	return m_nVScrollBarWidth;
}

CScrollBarEx *CFormWnd::GetHorzScrollBarEx()
{
	return &m_wndHScroll;
}

CScrollBarEx *CFormWnd::GetVertScrollBarEx()
{
	return &m_wndVScroll;
}

BOOL CFormWnd::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	// calc new x position
	int x = GetScrollPos(SB_HORZ);
	int xOrig = x;

	switch (LOBYTE(nScrollCode))
	{
	case SB_TOP:
		x = 0;
		break;
	case SB_BOTTOM:
		x = INT_MAX;
		break;
	case SB_LINEUP:
		x -= m_lineDev.cx;
		break;
	case SB_LINEDOWN:
		x += m_lineDev.cx;
		break;
	case SB_PAGEUP:
		x -= m_pageDev.cx;
		break;
	case SB_PAGEDOWN:
		x += m_pageDev.cx;
		break;
	case SB_THUMBTRACK:
		x = nPos;
		break;
	}

	// calc new y position
	int y = GetScrollPos(SB_VERT);
	int yOrig = y;

	switch (HIBYTE(nScrollCode))
	{
	case SB_TOP:
		y = 0;
		break;
	case SB_BOTTOM:
		y = INT_MAX;
		break;
	case SB_LINEUP:
		y -= m_lineDev.cy;
		break;
	case SB_LINEDOWN:
		y += m_lineDev.cy;
		break;
	case SB_PAGEUP:
		y -= m_pageDev.cy;
		break;
	case SB_PAGEDOWN:
		y += m_pageDev.cy;
		break;
	case SB_THUMBTRACK:
		y = nPos;
		break;
	}

	BOOL bResult = OnScrollBy(CSize(x - xOrig, y - yOrig), bDoScroll);
	if (bResult && bDoScroll)
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
	}

	return bResult;
}

BOOL CFormWnd::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
	int xOrig, x;
	int yOrig, y;

	// don't scroll if there is no valid scroll range (ie. no scroll bar)
	CScrollBar* pBar;
	DWORD dwStyle = GetStyle();
	pBar = GetScrollBarCtrl(SB_VERT);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_VSCROLL)))
	{
		// vertical scroll bar not enabled
		sizeScroll.cy = 0;
	}
	pBar = GetScrollBarCtrl(SB_HORZ);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_HSCROLL)))
	{
		// horizontal scroll bar not enabled
		sizeScroll.cx = 0;
	}

	// adjust current x position
	xOrig = x = GetScrollPos(SB_HORZ);
	int xMax = GetScrollLimit(SB_HORZ);
	x += sizeScroll.cx;
	if (x < 0)
		x = 0;
	else if (x > xMax)
		x = xMax;

	// adjust current y position
	yOrig = y = GetScrollPos(SB_VERT);
	int yMax = GetScrollLimit(SB_VERT);
	y += sizeScroll.cy;
	if (y < 0)
		y = 0;
	else if (y > yMax)
		y = yMax;

	// did anything change?
	if (x == xOrig && y == yOrig)
		return FALSE;

	if (bDoScroll)
	{
		// do scroll and update scroll positions
		ScrollWindow(-(x-xOrig), -(y-yOrig));
		if (x != xOrig)
			SetScrollPos(SB_HORZ, x);
		if (y != yOrig)
			SetScrollPos(SB_VERT, y);
	}
	return TRUE;
}
