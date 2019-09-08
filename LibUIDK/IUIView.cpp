// IUIView.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIUIView

IMPLEMENT_DYNCREATE(CIUIView, CUIWnd)

CIUIView::CIUIView()
{
	m_pDocument = NULL;
}

CIUIView::~CIUIView()
{
}


BEGIN_MESSAGE_MAP(CIUIView, CUIWnd)
	//{{AFX_MSG_MAP(CIUIView)
	ON_WM_CREATE()
	ON_WM_MDIACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// virtual

void CIUIView::PostNcDestroy()
{
	delete this;
}

void CIUIView::OnActivateView(BOOL bActivate, CIUIView *pActivateView, CIUIView *pDeactiveView)
{
	UNUSED(pActivateView);   // unused in release builds

	if (bActivate)
	{
		// take the focus if this frame/view/pane is now active
		if (IsTopParentActive())
		{
			SetFocus();
		}
	}
}

void CIUIView::OnActivateFrame(UINT /*nState*/, CIUIFrameWnd * /*pFrameWnd*/)
{
}

//////////////////////////////////////////////////////////////////////////
// public

CIUIDocument *CIUIView::GetDocument()
{
	return m_pDocument;
}

/////////////////////////////////////////////////////////////////////////////
// CIUIView message handlers

int CIUIView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// TODO: Add your specialized creation code here

	// if ok, wire in the current document
	ASSERT(m_pDocument == NULL);
	CCreateContext *pContext = (CCreateContext *)lpCreateStruct->lpCreateParams;

	// A view should be created in a given context!
	if (pContext != NULL && pContext->m_pCurrentDoc != NULL)
	{
		((CIUIDocument *)pContext->m_pCurrentDoc)->AddView(this);
		ASSERT(m_pDocument != NULL);
	}
	else
	{
		TRACE0("[LibUIDK]===Warning: Creating a pane with no CDocument.\n");
	}

	return 0;
}

void CIUIView::OnMDIActivate(BOOL bActivate, CWnd *pActivateWnd, CWnd *)
{
}
