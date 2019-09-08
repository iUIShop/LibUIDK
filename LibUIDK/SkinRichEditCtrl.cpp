// SkinRichEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#if(_MSC_VER <= 1200) // VC6.0
#include <ObjIdl.h>
#endif
#include <RichOle.h>
#include "TextHost.h"
#include <atlbase.h>
#include <atlwin.h>
#pragma comment (lib, "Imm32.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////

CSkinRichEditCtrl::CSkinRichEditCtrl()
{
	m_pBindWnd = this;
}

CSkinRichEditCtrl::~CSkinRichEditCtrl()
{
}

BEGIN_MESSAGE_MAP(CSkinRichEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CSkinRichEditCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSkinRichEditCtrl message handlers

int CSkinRichEditCtrl::OnDrawBackground(CDC *pMemDCParent)
{
	CRect rcWnd;
	GetWindowRect(rcWnd);
	GetParent()->ScreenToClient(rcWnd);

	if (m_bBkCombine)
	{
		if (m_himgCombineBk != NULL)
		{
			DrawCombineBackground(pMemDCParent, rcWnd, COMBINEIMAGESIZE4, 0);
		}
	}
	else
	{
		IUIDrawImage(pMemDCParent->GetSafeHdc(), rcWnd, m_himgBk[0],
			m_eBkImageResizeMode, m_ptBkImageResize, m_lBkImageRepeatX, m_lBkImageRepeatY);
	}

	return 0;
}

int CSkinRichEditCtrl::OnDraw(CDC *pMemDCParent)
{
	return OnDrawRichText(pMemDCParent);
}

LRESULT CSkinRichEditCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	// When use layered window, the window not redraw when its child controls hide or display
	// to solve it, handle the WM_WINDOWPOSCHANGED message.
	CWnd *pParent = GetParent();
	BOOL bLayered = IsLayeredWindow(pParent->GetSafeHwnd());
	if (message == WM_WINDOWPOSCHANGED && bLayered)
	{
		WINDOWPOS *pos = (WINDOWPOS *)lParam;
		if (pos->hwnd == m_hWnd)
		{
			// When show the control, notify itself to redraw
			if ((pos->flags & SWP_SHOWWINDOW) == SWP_SHOWWINDOW)
			{
				Redraw(this);
			}
			// When hide the control, notify its parent to redraw
			else if ((pos->flags & SWP_HIDEWINDOW) == SWP_HIDEWINDOW)
			{
				CRect rcThis;
				GetWindowRect(rcThis);
				pParent->ScreenToClient(rcThis);
				pParent->InvalidateRect(rcThis);
			}
		}
	}

	if (message == WM_CREATE)
	{
	}
	else if (message == WM_WL_CALLONDRAW)
	{
		// 有句柄的RichEdit在控件缩放时，会有闪烁，很难解决。 所以，有句柄的RichEdit的内容，也通过
		// 调用ITextServices::TxDraw绘制到父窗口上，同时把控件自己的WM_PAINT消息，直接返回0，从而
		// 拒绝控件自己的显示。 所以这样一来，RichEdit按windowless控件那样，由父窗口向RichEdit控件
		// 发送WM_WL_CALLONDRAW消息来绘制。
		CDC *pParentMemDC = (CDC *)wParam;

		OnDrawBackground(pParentMemDC);

		OnDraw(pParentMemDC);
	}
	// 当有句柄的RichEdit包含WS_EX_TRANSPARENT风格时，RichEdit需要刷新的时候，就会触发父窗口的
	// WM_PAINT消息，我们无法主动去触发父窗口的WM_PAINT。 当RichEdit不包含WS_EX_TRANSPARENT风格时。
	// RichEdit需要刷新时，就会触发RichEdit自己的WM_PAINT，不会触发父窗口的WM_PAINT。
	else if (message == WM_PAINT)
	{
		// CPaintDC dc(this);不能删除，否则会导致不停的收到WM_PAINT消息
		CPaintDC dc(this);
		return 0;
	}
	else if (message == WM_ERASEBKGND)
	{
		return TRUE;
	}
	else if (message == WM_VSCROLL)
	{
		LRESULT lr = CRichEditCtrl::WindowProc(message, wParam, lParam);

		GetParent()->Invalidate();
		m_bNeedUpdate = TRUE;

		return lr;
	}
	else if (message == WM_ISNEEDUPDATA)
	{
		return (LRESULT)m_bNeedUpdate;
	}
	else if (message == WM_SETNEEDUPDATE)
	{
		m_bNeedUpdate = (BOOL)lParam;
	}
	else if (message == EM_ADD_GIF_OLE)
	{
		OnAddGifOle((CImageOleCtrl *)lParam);
	}
	else if (message == EM_GET_GIF_OLE_LIST)
	{
		return (LPARAM)&m_mapGifOles;
	}
	// RichEdit内的OLE控件发来的
	else if (message >= WM_WLMOUSEFIRST && message <= WM_WLMOUSELAST)
	{
		NMHDR nmhdr;
		nmhdr.hwndFrom = m_hWnd;
		nmhdr.idFrom = GetDlgCtrlID();
		nmhdr.code = EN_WLMOUSE;
		NMREWLMOUSE rehdr;
		rehdr.hdr = nmhdr;
		rehdr.uWLMsg = message;
		rehdr.uSenderID = (UINT)wParam;
		rehdr.pWLMouse = (WLMOUSE *)lParam;

		BOOL bRet = (BOOL)GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&rehdr));
		if (bRet)
		{
			return 0;
		}
	}

	return CRichEditCtrl::WindowProc(message, wParam, lParam);
}
