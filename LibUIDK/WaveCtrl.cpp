// WaveCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ResourceMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CWave

CWave::CWave()
{
	m_crWave = RGB(255, 255, 255);
	m_nLineWidth = 3;
}

CWave::~CWave()
{
	DeleteAllWaveItems();
}

int CWave::AddWaveItem(const POINT *pItem)
{
	m_vWaveItem.push_back(*pItem);

	int nCount = GetWaveItemCount();
	return nCount - 1;
}

int CWave::GetWaveItem(int nIndex, POINT *pItem) const
{
	if (pItem == NULL)
	{
		return -1;
	}

	int nCount = GetWaveItemCount();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return -2;
	}

	const POINT *pWaveItem = &m_vWaveItem[nIndex];
	*pItem = *pWaveItem;

	return 0;
}

int CWave::GetWaveItemCount() const
{
	int nCount = (int)m_vWaveItem.size();
	return nCount;
}

int CWave::DeleteAllWaveItems()
{
	m_vWaveItem.clear();
	return 0;
}

COLORREF CWave::SetWaveColor(COLORREF crWave)
{
	COLORREF crOld = m_crWave;
	m_crWave = crWave;
	return crOld;
}

COLORREF CWave::GetWaveColor() const
{
	return m_crWave;
}

UINT CWave::SetLineWidth(UINT nLineWidth)
{
	UINT nOldLineWidth = m_nLineWidth;
	m_nLineWidth = nLineWidth;
	return nOldLineWidth;
}

UINT CWave::GetLineWidth() const
{
	return m_nLineWidth;
}

/////////////////////////////////////////////////////////////////////////////
// CWaveCtrl

CWaveCtrl::CWaveCtrl()
{
	m_dwStyle = 0;
	m_bUseGdiplus = TRUE;

	m_bDrawWaveHorizontalLine = TRUE;

	m_crBackground = RGB(0, 0, 0);
	m_crGridLine = RGB(0, 128, 64);
	m_eWaveCtrlOrigin = BOTTOMLEFT;
	m_nGridWidth = 30;
	m_nGridHeight = 30;
	m_nInfoWidth = 60;
	m_nInfoHeight = 20;
	m_nGridUnitX = 1;
	m_nGridUnitY = 1;
	m_nScrollPos = 0;
	m_bEnableDragMove = TRUE;
	m_bEnableAntiAliasing = TRUE;

	for (int i = 0; i < 2; ++i)
	{
		m_himgBk[i] = NULL;
	}
}

CWaveCtrl::~CWaveCtrl()
{
	DeleteAllWaves();
}

int CWaveCtrl::BindStyle(LPCTSTR lpszStyleID)
{
	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (lpszStyleID == NULL)
	{
		return -2;
	}

	CTRLPROPERTIES *pCtrlProp = CUIMgr::GetStyleItem(STYLET_WAVECTRL, lpszStyleID);
	return BindStyle(pCtrlProp);

	return 0;
}

int CWaveCtrl::BindStyle(const CTRLPROPERTIES *pCtrlProp)
{
	if (m_hWnd == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (pCtrlProp == NULL)
	{
		return -2;
	}
	CONTROL_TYPE ect = (CONTROL_TYPE)pCtrlProp->m_eControlType;
	if (ect != CT_STYLE_WAVECTRL && ect != CT_WAVECTRL)
	{
		return -3;
	}

	WAVECTRLPROPERTIES *pWaveCtrlProp = (WAVECTRLPROPERTIES *)pCtrlProp;

	// Set bitmaps for background
	if (pWaveCtrlProp->m_bSpecifyBackgroundImages)
	{
		CString strImageName[3];
		BOOL bCombineImage = TRUE;
		CTRLPROPERTIES::IUIGetBackground2(pWaveCtrlProp, &bCombineImage, strImageName);
		if (bCombineImage)
		{
			SetBkCombineImage(strImageName[0]);
		}
		else
		{
			SetBkImage(CONTROL_STATE_NORMAL | CONTROL_STATE_DISABLED, strImageName[1], strImageName[2]);
		}

		SetBkImageResizeMode(pCtrlProp->m_eBkImageResizeMode);
		SetBkImageResizePoint(pCtrlProp->m_ptImageResize);
	}

	// Enable drag move
	EnableDragMove(pWaveCtrlProp->m_bEnableDragMove);

	//
	SetTag(pWaveCtrlProp->m_strTag);

	// Grid
	SetGridWidht(pWaveCtrlProp->m_lGridWidth);
	SetGridHeight(pWaveCtrlProp->m_lGridHeight);
	COLORREF cr = RGB(0, 128, 64);
	CUIMgr::GetColorResource(pWaveCtrlProp->m_strGridLineColorResID, &cr);
	SetGridLineColor(cr);
	SetGridUnitX(pWaveCtrlProp->m_lGridUnitX);
	SetGridUnitY(pWaveCtrlProp->m_lGridUnitY);

	// Information area
	SetInfoAreaWidth(pWaveCtrlProp->m_lInfoAreaWidth);
	SetInfoAreaHeight(pWaveCtrlProp->m_lInfoAreaHeight);

	// Background
	cr = RGB(0, 0, 0);
	CUIMgr::GetColorResource(pWaveCtrlProp->m_strBackgroundColorResID, &cr);
	SetBkColor(cr);

	return 0;
}

BOOL CWaveCtrl::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	m_dwStyle = dwStyle;

	CString strMyClass;
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;
	strMyClass = WC_WAVECTRL;
	wndcls.lpszClassName = strMyClass.GetBuffer(strMyClass.GetLength());

	// Register the new class and exit if it fails
	if (!AfxRegisterClass(&wndcls))
	{
		TRACE(_T("[LibUIDK]===Class Registration Failed\n"));
		return FALSE;
	}

	CWnd *pWnd = this;
	return pWnd->Create(WC_WAVECTRL, NULL, dwStyle, rect, pParentWnd, nID);
}

BEGIN_MESSAGE_MAP(CWaveCtrl, CWnd)
	//{{AFX_MSG_MAP(CWaveCtrl)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWaveCtrl message handlers

int CWaveCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// TODO: Add your specialized creation code here

	return 0;
}

void CWaveCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CRect rcClient;
	GetClientRect(rcClient);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap *pOldMemBmp = memDC.SelectObject(&bmpMem);

	OnDraw(&memDC);

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldMemBmp);

	// Do not call CWnd::OnPaint() for painting messages
}

BOOL CWaveCtrl::OnEraseBkgnd(CDC *pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//	return CWnd::OnEraseBkgnd(pDC);
}

LRESULT CWaveCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	if (m_bEnableDragMove)
	{
		if (message == WM_LBUTTONDOWN)
		{
			SetCapture();
			m_bLButtonDown = TRUE;
			m_ptLButtonDown = CPoint(lParam);
			m_nScrollPosOnLButtonDown = GetScrollPos();
		}
		if (message == WM_LBUTTONUP)
		{
			m_bLButtonDown = FALSE;
			ReleaseCapture();
		}
		if (message == WM_CAPTURECHANGED)
		{
			m_bLButtonDown = FALSE;
			ReleaseCapture();
		}

		// Drag the wave
		if (message == WM_MOUSEMOVE && wParam == MK_LBUTTON && m_bLButtonDown)
		{
			CPoint point(lParam);
			int nXOffset = point.x - m_ptLButtonDown.x;

			SetScrollPos(m_nScrollPosOnLButtonDown - nXOffset);

			NMHDR nmhdr;
			nmhdr.hwndFrom = m_hWnd;
			nmhdr.idFrom = GetDlgCtrlID();
			nmhdr.code = WVN_SCROLLED;
			BOOL bRet = (BOOL)GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmhdr));
			if (bRet)
			{
				return 0;
			}
		}
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
// protected

int CWaveCtrl::OnDraw(CDC *pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	// Draw background color
	OnDrawBackground(pDC);

	NMHDR nmhdr;
	nmhdr.hwndFrom = m_hWnd;
	nmhdr.idFrom = GetDlgCtrlID();
	nmhdr.code = WVN_DRAWINFOAREA;
	NMWAVECTRL nmwc;
	nmwc.hDC = pDC->GetSafeHdc();
	nmwc.hdr = nmhdr;
	nmwc.rc = rcClient;
	int nRet = (int)GetParent()->SendMessage(WM_NOTIFY, nmhdr.idFrom, LPARAM(&nmwc));
	if (nRet != 0)
	{
	}

	CDC dcWave;
	dcWave.CreateCompatibleDC(pDC);
	CBitmap bmpWave;
	bmpWave.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	CBitmap *pbmpOldWave = dcWave.SelectObject(&bmpWave);

	OnDrawBackground(&dcWave);
	OnDrawHorizontalGrid(&dcWave);
	if (GetWaveCount() != 1)
	{
		OnDrawVerticalGrid(&dcWave);
	}
	OnDrawWaves(&dcWave);

	pDC->BitBlt(m_nInfoWidth, 0, rcClient.Width() - m_nInfoWidth, rcClient.Height(), &dcWave, m_nInfoWidth, 0, SRCCOPY);
	dcWave.SelectObject(pbmpOldWave);

	return 0;
}

int CWaveCtrl::GenerateWaveRgn(const CWave *pWave, CRgn *pRgn)
{
	if (pWave == NULL)
	{
		return -1;
	}
	if (pRgn == NULL)
	{
		return -2;
	}

	if (pRgn->GetSafeHandle() != NULL)
	{
		return -3;
	}

	CRect rcClient;
	GetClientRect(rcClient);

	int nWaveItemCount = pWave->GetWaveItemCount();
	LONGLONG llXPrevious = 0;	// previous x value
	LONGLONG llYPrevious = 0;	// previous y value
	BOOL bPreviousValid = FALSE;

	LPPOINT lpPoints = new POINT[nWaveItemCount + 2];

	for (int nWaveItem = 0; nWaveItem < nWaveItemCount; ++nWaveItem)
	{
		POINT ptWaveItem;
		int nRet = pWave->GetWaveItem(nWaveItem, &ptWaveItem);
		if (nRet != 0)
		{
			continue;
		}

		if (m_eWaveCtrlOrigin == BOTTOMLEFT)
		{
			LONGLONG llX = m_nInfoWidth + m_nGridWidth * ptWaveItem.x / m_nGridUnitX - m_nScrollPos;
			LONGLONG llY = rcClient.bottom - m_nInfoHeight - m_nGridHeight * ptWaveItem.y / m_nGridUnitY;

			int nOffsetY = m_nInfoHeight;
			// First point
			if (nWaveItem == 0)
			{
				lpPoints->x = (LONG)llX;
				lpPoints->y = rcClient.bottom - nOffsetY;
			}

			(lpPoints + nWaveItem + 1)->x = (LONG)llX;
			(lpPoints + nWaveItem + 1)->y = (LONG)llY;

			// Last point
			if (nWaveItem == nWaveItemCount - 1)
			{
				(lpPoints + nWaveItemCount + 1)->x = (LONG)llX;
				(lpPoints + nWaveItemCount + 1)->y = rcClient.bottom - nOffsetY;
			}

			//
			llXPrevious = llX;
			llYPrevious = llY;
			bPreviousValid = TRUE;
		}
	}

	pRgn->CreatePolygonRgn(lpPoints, nWaveItemCount + 2, WINDING);
	delete [] lpPoints;

	return 0;
}

int CWaveCtrl::OnDrawBackground(CDC *pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	// Draw background
	IUIStretchBlt(pDC->GetSafeHdc(), rcClient, m_himgBk[0]);

	return 0;
}

int CWaveCtrl::OnDrawHorizontalGrid(CDC *pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	int nWaveCount = GetWaveCount();
	if (nWaveCount == 1 && m_bDrawWaveHorizontalLine)
	{
		GenerateWaveRgn(GetWave(0), &m_rgnOnlyOneWave);
		::ExtSelectClipRgn(pDC->GetSafeHdc(), (HRGN)m_rgnOnlyOneWave.GetSafeHandle(), RGN_XOR);
	}
	// Draw grid lines
	int nYLines = (rcClient.Height() - m_nInfoHeight) / m_nGridHeight;

	if (m_eWaveCtrlOrigin == BOTTOMLEFT)
	{
		// Draw horizontal lines
		LOGBRUSH lb;
		lb.lbColor = m_crGridLine;
		lb.lbStyle = BS_SOLID;
		CPen penGrid;
		penGrid.CreatePen(PS_COSMETIC | PS_ALTERNATE, 1, &lb, 0, NULL);

		CGdiObject *pOldGrid = pDC->SelectObject(&penGrid);
		for (int y = 0; y < nYLines + 1; ++y)
		{
			int nY = rcClient.bottom - m_nInfoHeight - m_nGridHeight * y;

			pDC->MoveTo(m_nInfoWidth, nY);
			pDC->LineTo(rcClient.right, nY);
		}
		pDC->SelectObject(pOldGrid);
		penGrid.DeleteObject();
	}

	if (nWaveCount == 1 && m_bDrawWaveHorizontalLine)
	{
		pDC->SelectClipRgn(NULL);
	}

	return 0;
}

int CWaveCtrl::OnDrawVerticalGrid(CDC *pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	// Draw grid lines
	int nXLines = (rcClient.Width() - m_nInfoWidth) / m_nGridWidth;

	if (m_eWaveCtrlOrigin == BOTTOMLEFT)
	{
		// Draw horizontal lines
		LOGPEN lpGrid;
		lpGrid.lopnColor = m_crGridLine;

		// Draw vertical lines
		lpGrid.lopnStyle = PS_SOLID;
		lpGrid.lopnWidth = CPoint(2, 2);
		CPen penGridVer;
		penGridVer.CreatePenIndirect(&lpGrid);
		CGdiObject *pOldVerGrid = pDC->SelectObject(&penGridVer);
		for (int x = 0; x < nXLines + 1; ++x)
		{
			int nX = m_nInfoWidth + m_nGridWidth * x - m_nScrollPos;
			if (nX < m_nInfoWidth)
			{
				nXLines++;
				continue;
			}

			if (m_bUseGdiplus)
			{
				Graphics g(pDC->GetSafeHdc());
				Color cr(170, 85, 100, 135);
				Pen p(cr, 2);
				g.DrawLine(&p, (REAL)nX, (REAL)0, (REAL)nX, (REAL)rcClient.bottom - m_nInfoHeight);
			}
			else
			{
				pDC->MoveTo(nX, 0);
				pDC->LineTo(nX, rcClient.bottom - m_nInfoHeight);
			}
		}
		pDC->SelectObject(pOldVerGrid);
		penGridVer.DeleteObject();
	}

	return 0;
}

int CWaveCtrl::OnDrawWaves(CDC *pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	// Draw waves
	int nWaveCount = GetWaveCount();
	for (int nWave = 0; nWave < nWaveCount; ++nWave)
	{
		CWave *pWave = GetWave(nWave);
		LOGPEN lpWave;
		lpWave.lopnColor = pWave->GetWaveColor();
		lpWave.lopnStyle = PS_SOLID;
		lpWave.lopnWidth = CPoint(1, 1);
		CPen penWave;
		penWave.CreatePenIndirect(&lpWave);
		CGdiObject *pOldWave = pDC->SelectObject(&penWave);

		int nWaveItemCount = pWave->GetWaveItemCount();
		LONGLONG llXPrevious = 0;	// previous x value
		LONGLONG llYPrevious = 0;	// previous y value
		BOOL bPreviousValid = FALSE;

		// Draw wave horizontal lines
		if (m_bDrawWaveHorizontalLine && nWaveCount == 1)
		{
			pDC->SelectClipRgn(&m_rgnOnlyOneWave);

			int nR = 4;
			int nG = 4;
			int nB = 4;

			for (int i = 0; i < rcClient.bottom;)
			{
				int nY = rcClient.top + 2 * i;

				if (m_bUseGdiplus)
				{
					Graphics g(pDC->GetSafeHdc());
					Color cr(170, 100 - 1 * (i / 2), 114 - 1 * (i / 2), 146 - 1 * (i / 2));
					Pen p(cr, 2);
					g.DrawLine(&p, (REAL)0, (REAL)nY, (REAL)rcClient.right, (REAL)nY);
				}
				else
				{
					LOGPEN lpGrid;
					lpGrid.lopnColor = RGB(100 - 1 * (i / 2), 114 - 1 * (i / 2), 146 - 1 * (i / 2));
					lpGrid.lopnStyle = PS_SOLID;
					lpGrid.lopnWidth = CPoint(2, 2);
					CPen penGrid;
					penGrid.CreatePenIndirect(&lpGrid);
					CGdiObject *pOldGrid = pDC->SelectObject(&penGrid);

					pDC->MoveTo(0, nY);
					pDC->LineTo(rcClient.right, nY);

					pDC->SelectObject(pOldGrid);
				}
				i += 2;
			}
			pDC->SelectClipRgn(NULL);
		}

		if (nWaveCount == 1)
		{
			OnDrawVerticalGrid(pDC);
		}

		bPreviousValid = FALSE;
		//////////////////////////////////////////////////////////////////////////

		int nMinValue = INT_MIN;
		for (int nWaveItem = 0; nWaveItem < nWaveItemCount; ++nWaveItem)
		{
			POINT ptWaveItem;
			int nRet = pWave->GetWaveItem(nWaveItem, &ptWaveItem);
			if (nRet != 0)
			{
				continue;
			}

			if (m_eWaveCtrlOrigin == BOTTOMLEFT)
			{
				LONGLONG llX = m_nInfoWidth + m_nGridWidth * ptWaveItem.x / m_nGridUnitX - m_nScrollPos;
				LONGLONG llY = rcClient.bottom - m_nInfoHeight - m_nGridHeight * ptWaveItem.y / m_nGridUnitY;

				if (llY > nMinValue)
				{
					nMinValue = (int)llY;
				}
				if (bPreviousValid && llX >= m_nInfoWidth)
				{
					if (m_bUseGdiplus)
					{
						if (IsEnableAntiAliasing())
						{
							Graphics g(pDC->GetSafeHdc());
							COLORREF crWave = pWave->GetWaveColor();
							Color cr(GetRValue(crWave), GetGValue(crWave), GetBValue(crWave));
							Pen p(cr, (REAL)pWave->GetLineWidth());
							g.SetSmoothingMode(SmoothingModeAntiAlias);
							g.DrawLine(&p, (REAL)llXPrevious, (REAL)llYPrevious, (REAL)llX, (REAL)llY);
						}
					}
					else
					{
						pDC->MoveTo((int)llXPrevious, (int)llYPrevious);
						pDC->LineTo((int)llX, (int)llY);
					}
				}

				//
				llXPrevious = llX;
				llYPrevious = llY;
				bPreviousValid = TRUE;
			}
		}

		// Draw min value horizontal line
		if (nWaveCount == 1)
		{
			OnDrawWaveMinValueHorizontalLine(pDC, nMinValue);
		}

		pDC->SelectObject(pOldWave);
		penWave.DeleteObject();
	}

	return 0;
}

int CWaveCtrl::OnDrawWaveMinValueHorizontalLine(CDC *pDC, int nMinValue)
{
	CRect rcClient;
	GetClientRect(rcClient);

	if (m_bUseGdiplus)
	{
		Graphics g(pDC->GetSafeHdc());
		Color cr(170, 100, 114, 146);
		Pen p(cr, 2);
		g.DrawLine(&p, (REAL)m_nInfoWidth, (REAL)nMinValue, (REAL)rcClient.right, (REAL)nMinValue);
	}
	else
	{
		LOGPEN lpGrid;
		lpGrid.lopnColor = RGB(100, 114, 146);
		lpGrid.lopnStyle = PS_SOLID;
		lpGrid.lopnWidth = CPoint(2, 2);
		CPen penGrid;
		penGrid.CreatePenIndirect(&lpGrid);
		CGdiObject *pOldGrid = pDC->SelectObject(&penGrid);

		pDC->MoveTo(m_nInfoWidth, nMinValue);
		pDC->LineTo(rcClient.right, nMinValue);

		pDC->SelectObject(pOldGrid);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// public

int CWaveCtrl::SetBkImage(UINT uMask, LPCTSTR lpszImageNameBkN, LPCTSTR lpszImageNameBkD)
{
	IUISetControlImage2(uMask, m_himgBk, lpszImageNameBkN, lpszImageNameBkD);

	return 0;
}

int CWaveCtrl::GetBkImage(UINT uMask,
	BOOL *pbCombineImage,
	CString *pstrCombineImageName,
	CString *pstrImageNameN, CString *pstrImageNameD)
{
	if (pstrImageNameN != NULL)
	{
		GetImageFileName(m_himgBk[0], pstrImageNameN);
	}

	if (pstrImageNameD != NULL)
	{
		GetImageFileName(m_himgBk[1], pstrImageNameD);
	}

	return 0;
}

WaveCtrl_Origin CWaveCtrl::SetOrigin(WaveCtrl_Origin ewo, BOOL bRedraw/* = TRUE*/)
{
	WaveCtrl_Origin eOld = m_eWaveCtrlOrigin;
	m_eWaveCtrlOrigin = ewo;

	if (bRedraw)
	{
		Invalidate();
	}

	return eOld;
}

WaveCtrl_Origin CWaveCtrl::GetOrigin() const
{
	return m_eWaveCtrlOrigin;
}

int CWaveCtrl::SetGridWidht(int nGridWidht, BOOL bRedraw/* = TRUE*/)
{
	ASSERT(m_hWnd != NULL);

	m_nGridWidth = nGridWidht;

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CWaveCtrl::GetGridWidth() const
{
	return m_nGridWidth;
}

int CWaveCtrl::SetGridHeight(int nGridHeight, BOOL bRedraw/* = TRUE*/)
{
	ASSERT(m_hWnd != NULL);

	m_nGridHeight = nGridHeight;

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CWaveCtrl::GetGridHeight() const
{
	return m_nGridHeight;
}

int CWaveCtrl::SetInfoAreaWidth(int nInfoAreaWidth, BOOL bRedraw/* = TRUE*/)
{
	ASSERT(m_hWnd != NULL);

	m_nInfoWidth = nInfoAreaWidth;

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CWaveCtrl::GetInfoAreaWidth() const
{
	return m_nInfoWidth;
}

int CWaveCtrl::SetInfoAreaHeight(int nInfoAreaHeight, BOOL bRedraw/* = TRUE*/)
{
	ASSERT(m_hWnd != NULL);

	m_nInfoHeight = nInfoAreaHeight;

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CWaveCtrl::GetInfoAreaHeight() const
{
	return m_nInfoHeight;
}

COLORREF CWaveCtrl::SetBkColor(COLORREF cr, BOOL bRedraw/* = TRUE*/)
{
	COLORREF crOld = m_crBackground;
	m_crBackground = cr;

	if (bRedraw)
	{
		Invalidate();
	}

	return crOld;
}

COLORREF CWaveCtrl::GetBkColor() const
{
	return m_crBackground;
}

COLORREF CWaveCtrl::SetGridLineColor(COLORREF cr, BOOL bRedraw/* = TRUE*/)
{
	COLORREF crOld = m_crGridLine;
	m_crGridLine = cr;

	if (bRedraw)
	{
		Invalidate();
	}

	return crOld;
}

COLORREF CWaveCtrl::GetGridLineColor() const
{
	return m_crGridLine;
}

int CWaveCtrl::SetGridUnitX(int nX, BOOL bRedraw/* = TRUE*/)
{
	ASSERT(m_hWnd != NULL);

	m_nGridUnitX = nX;

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CWaveCtrl::GetGridUnitX() const
{
	return m_nGridUnitX;
}

int CWaveCtrl::SetGridUnitY(int nY, BOOL bRedraw/* = TRUE*/)
{
	ASSERT(m_hWnd != NULL);

	m_nGridUnitY = nY;

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CWaveCtrl::GetGridUnitY() const
{
	return m_nGridUnitY;
}

int CWaveCtrl::SetTag(LPCTSTR lpszTag)
{
	if (lpszTag == NULL)
	{
		return -1;
	}

	m_strTag = lpszTag;

	return 0;
}

int CWaveCtrl::GetTag(CString &strTag) const
{
	strTag = m_strTag;

	return 0;
}

int CWaveCtrl::EnableAntiAliasing(BOOL bEnableAntiAliasing)
{
	m_bEnableAntiAliasing = bEnableAntiAliasing;
	return 0;
}

BOOL CWaveCtrl::IsEnableAntiAliasing() const
{
	return m_bEnableAntiAliasing;
}

int CWaveCtrl::SetScrollPos(int nPixel, BOOL bRedraw/* = TRUE*/)
{
	ASSERT(m_hWnd != NULL);

	if (nPixel == m_nScrollPos)
	{
		return 0;
	}

	if (nPixel < 0)
	{
		nPixel = 0;
	}

	m_nScrollPos = nPixel;

	if (bRedraw)
	{
		Invalidate();
	}

	return 0;
}

int CWaveCtrl::GetScrollPos() const
{
	return m_nScrollPos;
}

int CWaveCtrl::EnableDragMove(BOOL bEnableDragMove)
{
	m_bEnableDragMove = bEnableDragMove;
	return 0;
}

BOOL CWaveCtrl::IsEnableDragMove() const
{
	return m_bEnableDragMove;
}

int CWaveCtrl::AddWave(CWave *pWave)
{
	m_vWaves.push_back(pWave);
	int nCount = GetWaveCount();
	return nCount - 1;
}

CWave *CWaveCtrl::GetWave(int nIndex)
{
	int nCount = GetWaveCount();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return NULL;
	}

	CWave *pWave = m_vWaves[nIndex];
	return pWave;
}

int CWaveCtrl::DeleteWave(int nIndex)
{
	int nCount = GetWaveCount();
	if (nIndex < 0 || nIndex >= nCount)
	{
		return -1;
	}

	m_vWaves.erase(m_vWaves.begin() + nIndex);

	return 0;
}

int CWaveCtrl::DeleteWave(CWave *pWave)
{
	BOOL bFind = FALSE;
	int nCount = GetWaveCount();
	for (int i = 0; i < nCount; ++i)
	{
		CWave *p = m_vWaves[i];
		if (pWave == p)
		{
			bFind = TRUE;
			m_vWaves.erase(m_vWaves.begin() + i);
			break;
		}
	}

	if (!bFind)
	{
		return -1;
	}

	return 0;
}

int CWaveCtrl::DeleteAllWaves()
{
	m_vWaves.clear();
	return 0;
}

int CWaveCtrl::GetWaveCount() const
{
	int nCount = (int)m_vWaves.size();
	return nCount;
}
