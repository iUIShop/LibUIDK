// WLLine.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


struct WLLINEMEMBER
{
	CPoint m_pt1;		// The point of line start
	CPoint m_pt2;		// The point of line end

	int m_nPenStyle;
	int m_nLineWidth;
	COLORREF m_crLineColorPt1;
	COLORREF m_crLineColorPt2;
};

#define m_pt1 (((WLLINEMEMBER *)m_pMember)->m_pt1)
#define m_pt2 (((WLLINEMEMBER *)m_pMember)->m_pt2)

#define m_nPenStyle (((WLLINEMEMBER *)m_pMember)->m_nPenStyle)
#define m_nLineWidth (((WLLINEMEMBER *)m_pMember)->m_nLineWidth)
#define m_crLineColorPt1 (((WLLINEMEMBER *)m_pMember)->m_crLineColorPt1)
#define m_crLineColorPt2 (((WLLINEMEMBER *)m_pMember)->m_crLineColorPt2)

//////////////////////////////////////////////////////////////////////////
// CWLLine

CWLLine::CWLLine()
{
	m_pMember = NULL;
	m_pMember = new WLLINEMEMBER;

	m_pt1.x = m_pt1.y = 0;
	m_pt2.x = m_pt2.y = 0;

	m_nPenStyle = 0;
	m_nLineWidth = 1;
	m_crLineColorPt1 = RGB(0, 0, 0);
	m_crLineColorPt2 = RGB(0, 0, 0);
}

CWLLine::~CWLLine()
{
	if (m_pMember != NULL)
	{
		delete (WLLINEMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}


BOOL CWLLine::Create(LPCTSTR lpszControlName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CWLWnd *pVirtualParent)
{
	BOOL bRet = CRectCtrl::Create(lpszControlName, dwStyle, rect, pParentWnd, nID, pVirtualParent);
	if (!bRet)
		return FALSE;

	return TRUE;
}

int CWLLine::BindStyle(LPCTSTR lpszStyleID)
{
	if (!IsCreated())
	{
		ASSERT(FALSE);
		return -1;
	}

	if (lpszStyleID == NULL)
		return -2;

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_WL_LINE, lpszStyleID);
	return BindStyle(pCtrlProp);
}

int CWLLine::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	if (!IsCreated())
	{
		ASSERT(FALSE);
		return -1;
	}

	if (pCtrlProp == NULL)
		return -2;
	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (ect != CT_STYLE_WL_LINE && ect != CT_WL_LINE)
		return -3;

	LINEWLPROPERTIES *pWLLineProp = (LINEWLPROPERTIES *)pCtrlProp;

	SetLinePenStyle(pWLLineProp->m_ePenStyle);
	SetLineWidth((LONG)pWLLineProp->m_lLineWidth);

	RESCOLORPROPERTIES *pResColor = (RESCOLORPROPERTIES *)CUIMgr::GetColorResourceItem((LPCTSTR)pWLLineProp->m_strResColorIDStart);
	if (pResColor != NULL)
		SetPoint1Color(pResColor->m_crColor);
	pResColor = (RESCOLORPROPERTIES *)CUIMgr::GetColorResourceItem((LPCTSTR)pWLLineProp->m_strResColorIDEnd);
	if (pResColor != NULL)
		SetPoint2Color(pResColor->m_crColor);

	POINT pt1 = pWLLineProp->m_ptStart;
	SetPoint1Pos(&pt1);
	POINT pt2 = pWLLineProp->m_ptEnd;
	SetPoint2Pos(&pt2);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// protected

int CWLLine::OnDraw(CDC *pMemDCParent)
{
	if (!IsWindowVisible())
		return 1;

	CRect rcOrg;
	GetOrigWindowRect(rcOrg);

	CRect rcNow;
	GetWindowRect(rcNow);
	GetParent()->ScreenToClient(rcNow);

	// Don't draw this control if it's out of the parent
	CRect rcParent;
	GetParent()->GetClientRect(rcParent);

	CRect rcIntersect;
	rcIntersect.IntersectRect(rcNow, rcParent);
	if (rcIntersect.IsRectEmpty())
		return 0;

	CPoint pt1Now;
	CPoint pt2Now;

	// prepare pt.x
	if (m_pt1.x < m_pt2.x)
	{
		pt1Now.x = rcNow.left + m_pt1.x;

		int nRightMargin = rcOrg.Width() - m_pt2.x;
		pt2Now.x = rcNow.right - nRightMargin;
	}
	else if (m_pt1.x == m_pt2.x)
	{
		pt1Now.x = pt2Now.x = rcNow.left + m_pt1.x;
	}
	else
	{
		int nRightMargin = rcOrg.Width() - m_pt1.x;
		pt1Now.x = rcNow.right - nRightMargin;

		pt2Now.x = rcNow.left + m_pt2.x;
	}

	// prepare pt.y
	if (m_pt1.y < m_pt2.y)
	{
		pt1Now.y = rcNow.top + m_pt1.y;

		int nBottomMargin = rcOrg.Height() - m_pt2.y;
		pt2Now.y = rcNow.bottom - nBottomMargin;
	}
	else if (m_pt1.y == m_pt2.y)
	{
		pt1Now.y = pt2Now.y = rcNow.top + m_pt1.y;
	}
	else
	{
		int nBottomMargin = rcOrg.Height() - m_pt1.y;
		pt1Now.y = rcNow.bottom - nBottomMargin;

		pt2Now.y = rcNow.top + m_pt2.y;
	}

	// Get old clip region
	HRGN hOldClipRgn = ::CreateRectRgnIndirect(rcParent);
	int nRet = GetClipRgn(pMemDCParent->GetSafeHdc(), hOldClipRgn);

	// Set new clip region
	CRgn rgnClip;
	rgnClip.CreateRectRgnIndirect(rcNow);
	ExtSelectClipRgn(pMemDCParent->GetSafeHdc(), (HRGN)rgnClip.GetSafeHandle(), RGN_AND);

	// Draw line
	LOGPEN lp;
	lp.lopnColor = m_crLineColorPt1;
	lp.lopnStyle = m_nPenStyle;
	lp.lopnWidth = CPoint(m_nLineWidth, m_nLineWidth);

	CPen pen;
	pen.CreatePenIndirect(&lp);

	CPen *pOldPen = pMemDCParent->SelectObject(&pen);

	pMemDCParent->MoveTo(pt1Now);
	pMemDCParent->LineTo(pt2Now);

	pMemDCParent->SelectObject(pOldPen);

	// Restore old clip region
	if (nRet == 0)	// There is no clipping region for the given device context.
	{
		ExtSelectClipRgn(pMemDCParent->GetSafeHdc(), NULL, RGN_COPY);
	}
	else if (nRet == 1)
	{
		ExtSelectClipRgn(pMemDCParent->GetSafeHdc(), hOldClipRgn, RGN_COPY);
	}
	else
	{
		ASSERT(FALSE);
	}

	::DeleteObject(hOldClipRgn);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

int CWLLine::SetLinePenStyle(int nStyle)
{
	m_nPenStyle = nStyle;

	return 0;
}

int CWLLine::GetLinePenStyle() const
{
	return m_nPenStyle;
}

int CWLLine::SetLineWidth(int nWidth)
{
	if (nWidth < 1)
		return -1;

	m_nLineWidth = nWidth;

	return 0;
}

int CWLLine::GetLineWidth() const
{
	return m_nLineWidth;
}

int CWLLine::SetPoint1Color(COLORREF cr)
{
	m_crLineColorPt1 = cr;

	return 0;
}

COLORREF CWLLine::GetPoint1Color() const
{
	return m_crLineColorPt1;
}

int CWLLine::SetPoint2Color(COLORREF cr)
{
	m_crLineColorPt2 = cr;

	return 0;
}

COLORREF CWLLine::GetPoint2Color() const
{
	return m_crLineColorPt2;
}

int CWLLine::SetPoint1Pos(const LPPOINT lpPt)
{
	if (lpPt == NULL)
		return -1;

	m_pt1 = *lpPt;

	if (CUIMgr::IsEnableDPI())
	{
		GetDPIPoint(&m_pt1);
	}

	return 0;
}

int CWLLine::GetPoint1Pos(LPPOINT lpPt) const
{
	if (lpPt == NULL)
		return -1;

	*lpPt = m_pt1;

	return 0;
}

int CWLLine::SetPoint2Pos(const LPPOINT lpPt)
{
	if (lpPt == NULL)
		return -1;

	m_pt2 = *lpPt;

	if (CUIMgr::IsEnableDPI())
	{
		GetDPIPoint(&m_pt2);
	}

	return 0;
}

int CWLLine::GetPoint2Pos(LPPOINT lpPt) const
{
	if (lpPt == NULL)
		return -1;

	*lpPt = m_pt2;

	return 0;
}
