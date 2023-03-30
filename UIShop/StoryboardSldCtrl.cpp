// StoryboardSldCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "UIShop.h"
#include "StoryboardSldCtrl.h"
#include "UIDesignWnd.h"
using namespace Render;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CStoryboardSldCtrl

CStoryboardSldCtrl::CStoryboardSldCtrl()
{
	m_pUIDesignWnd = NULL;
}

CStoryboardSldCtrl::~CStoryboardSldCtrl()
{
}


BEGIN_MESSAGE_MAP(CStoryboardSldCtrl, CSliderCtrlEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CStoryboardSldCtrl message handlers


void CStoryboardSldCtrl::DrawHorzSlider(CDC *pMemDC)
{
	CSliderCtrlEx::DrawHorzSlider(pMemDC);

	// Set frame position.
	if (m_pUIDesignWnd != NULL)
	{
		size_t nStoryCount = m_pUIDesignWnd->GetUIWndProperties()->m_vStoryFrame.size();
		for (int i = 0; i < nStoryCount; ++i)
		{
			STORY_FRAME *pFrame = m_pUIDesignWnd->GetUIWndProperties()->m_vStoryFrame[i];
			int nFrame = pFrame->nFrameIndex;

			CRect rcPt;
			rcPt.left = 10 + (nFrame + 1) * 20;
			rcPt.top = 85;
			rcPt.right = rcPt.left + m_imgPoint.GetWidth();
			rcPt.bottom = rcPt.top + m_imgPoint.GetHeight();

			IUIBitBlt(pMemDC->GetSafeHdc(), rcPt, &m_imgPoint, 0, 0, SRCCOPY);

			// if the frame not empty, draw flag
			if (!pFrame->m_vStoryFrameCtrlItem.empty())
			{
				rcPt.DeflateRect(3, 3);
				pMemDC->FillSolidRect(rcPt, RGB(255, 0, 0));
			}
		}

		Invalidate();
	}
}

int CStoryboardSldCtrl::SetBindUIWnd(CUIDesignWnd *pUIDesignWnd)
{
	m_pUIDesignWnd = pUIDesignWnd;

	if (m_pUIDesignWnd == NULL)
	{
		EnableWindow(FALSE);
	}
	else
	{
		EnableWindow(TRUE);
	}

	Invalidate();

	return 0;
}

int CStoryboardSldCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSliderCtrlEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	m_imgPoint.LoadFromResource(MAKEINTRESOURCE(IDB_STORYPANELRAMEFLAG), _T("png"));

	return 0;
}
