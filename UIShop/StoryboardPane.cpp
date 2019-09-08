// StoryboardPane.cpp : implementation file
//

#include "stdafx.h"
#include "UIShop.h"
#include "StoryboardPane.h"
using namespace ImageManager;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CStoryboardPane

IMPLEMENT_DYNAMIC(CStoryboardPane, CDockablePane)

CStoryboardPane::CStoryboardPane()
{
	m_pUIDesignWnd = NULL;
}

CStoryboardPane::~CStoryboardPane()
{
}


BEGIN_MESSAGE_MAP(CStoryboardPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_PAINT()
END_MESSAGE_MAP()


int CStoryboardPane::SetBindUIWnd(CUIDesignWnd *pUIDesignWnd)
{
	m_pUIDesignWnd = pUIDesignWnd;

	m_sldTimeline.SetBindUIWnd(pUIDesignWnd);

	// Set text
	CString strText = _T("Storyboard");
	if (pUIDesignWnd != NULL)
	{
		CString strUIDesignWndText = LPCTSTR(m_pUIDesignWnd->GetUIWndProperties()->m_strID);

		strText += _T(" - [ ");
		strText += strUIDesignWndText;
		strText += _T(" ]");
	}

	SetWindowText(strText);

	return 0;
}

int CStoryboardPane::GetCurFrame()
{
	int nPos = m_sldTimeline.GetPos();

	return nPos - 1;
}

// CStoryboardPane message handlers

int CStoryboardPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_sldTimeline.Create(WS_VISIBLE | TBS_HORZ | WS_DISABLED, CRect(10, 0, 1041, 120), this, 100);
	m_sldTimeline.SetRange(0, 50);
	CIUIImage img;
	LPCTSTR lpszKey = MAKEINTRESOURCE(IDB_STORYPANELSLIDERBK);
	img.LoadFromResource(lpszKey, _T("png"));
	AddCustomImage(lpszKey, img.Detach());
	m_sldTimeline.SetImages(CONTROL_STATE_UNCHECKED_ALL, lpszKey, lpszKey, lpszKey, lpszKey);

	CIUIImage imgThm;
	LPCTSTR lpszKeyThumb = MAKEINTRESOURCE(IDB_STORYPANELSLIDERTHUMB);
	imgThm.LoadFromResource(MAKEINTRESOURCE(IDB_STORYPANELSLIDERTHUMB), _T("png"));
	AddCustomImage(lpszKeyThumb, imgThm.Detach());
	m_sldTimeline.SetThumbImages(CONTROL_STATE_UNCHECKED_ALL,
		lpszKeyThumb, lpszKeyThumb, lpszKeyThumb, lpszKeyThumb);

	m_sldTimeline.SetDeflate(10, 10);

	return 0;
}

void CStoryboardPane::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDockablePane::OnPaint() for painting messages

	CRect rcClient;
	GetClientRect(rcClient);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	CBitmap *pbmpMemOld = dcMem.SelectObject(&bmpMem);

	// Draw content to dcMem...
	dcMem.FillSolidRect(rcClient, RGB(255, 255, 255));

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(pbmpMemOld);
}

void CStoryboardPane::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	if (pScrollBar->GetSafeHwnd() == m_sldTimeline.GetSafeHwnd())
	{
		int nPos = m_sldTimeline.GetPos();

		if (m_pUIDesignWnd != NULL)
			m_pUIDesignWnd->SetCurStoryFrame(nPos - 1);
	}

	CDockablePane::OnHScroll(nSBCode, nPos, pScrollBar);
}
