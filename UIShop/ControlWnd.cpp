// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "UiShop.h"
#include "ControlWnd.h"
#include "UIDesignWnd.h"
#include "UIShopView.h"
#include "MainFrm.h"
#include "MsgWizardDlg.h"
#include "FormatBrushDlg.h"
using namespace Render;
using namespace ImageManager;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define ID_TIMER_HIGHLIGHT				1

extern CMainFrame *g_pFrm;


/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd()
{
	m_eControlType = CT_ARROW;

	m_bSelected = FALSE;
	m_bShowType = FALSE;
	m_bShowID = FALSE;
	m_bShowIndex = FALSE;
	m_nIndex = 0;

	m_pCtrlProperties = NULL;
	m_hImgCombine = NULL;
	m_hImgNormal = NULL;
	m_hForeground = NULL;
	m_pMoveSender = NULL;
	m_nXMove = 0;
	m_nYMove = 0;

	m_nHighlightCount = 0;

	m_bPressedLeftButton = FALSE;
	m_rcPressed.SetRect(0, 0, 0, 0);
}

CControlWnd::~CControlWnd()
{
	// TODO: Add your specialized creation code here
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_GETMINMAXINFO()
	ON_WM_RBUTTONUP()
	ON_COMMAND(IDSM_CTRL_PROPERTIES, OnCtrlProperties)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_MOVING()
	ON_MESSAGE(WM_EXITSIZEMOVE, OnExitSizeMove)
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCRBUTTONUP()
	ON_COMMAND(IDSM_CTRL_ADD_HANDLER, &CControlWnd::OnCtrlAddHandler)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// TODO: Add your specialized creation code here

	if (m_eControlType == CT_WL_SLIDER)
	{
		SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	else
	{
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	return 0;
}

void CControlWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: Add your message handler code here
	if (NULL != m_hImgCombine)
	{
		ReleaseIUIImage(m_hImgCombine);
		m_hImgCombine = NULL;
	}
	if (NULL != m_hImgNormal)
	{
		ReleaseIUIImage(m_hImgNormal);
		m_hImgNormal = NULL;
	}
	if (NULL != m_hForeground)
	{
		ReleaseIUIImage(m_hForeground);
		m_hForeground = NULL;
	}
}

int GetControlPreviewFont(HFONT *phFont, const CTRLPROPERTIES *pCtrlProp)
{
	if (phFont == NULL)
	{
		return -1;
	}

	// Default font
	CString strCurrentFont = CUIMgr::GetDefaultFont();
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	RESFONTPROPERTIES *pResDefaultFont = CUIMgr::GetDefaultFontResource(strCurrentFont);
	if (pResDefaultFont != NULL)
	{
		lf = pResDefaultFont->m_logFont;
	}

	FONTRESOURCEITEM *pResFontN = CUIMgr::GetFontResourceItem((LPCTSTR)pCtrlProp->m_strResFontIDN);
	CreateControlFont(phFont, pResFontN, &lf, strCurrentFont);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// virtual

int CControlWnd::DrawWindowlessColorBlock(CDC *pDC, const CRect &rcWnd)
{
	int nGridWidht = 8;
	int nGridHeight = 8;
	int X = rcWnd.Width() / nGridWidht;
	int Y = rcWnd.Height() / nGridHeight;
	for (int x = 0; x < X + 1; ++x)
	{
		for (int y = 0; y < Y + 1; ++y)
		{
			COLORREF cr = ((x + y) % 2 == 1) ? RGB(255, 255, 255) : RGB(204, 204, 204);
			pDC->FillSolidRect(nGridWidht * x, nGridHeight * y, nGridWidht, nGridHeight, cr);
		}
	}

	return 0;
}

int CControlWnd::DrawWindowlessLine(CDC *pDC, const LINEWLPROPERTIES *pLineProp)
{
	POINT pt1 = (POINT)pLineProp->m_ptStart;
	POINT pt2 = (POINT)pLineProp->m_ptEnd;
	CPoint pt1Now = pt1;
	CPoint pt2Now = pt2;

	CRect rcOrg = pLineProp->m_rcControl;
	rcOrg.OffsetRect(-rcOrg.left, -rcOrg.top);
	CRect rcNow;
	GetClientRect(rcNow);

	// pt1Now.x always equal to m_pt1.x

	// prepare pt1Now.y
	if (pt1.y > pt2.y)
	{
		pt1Now.y = rcNow.bottom - (rcOrg.bottom - pt1.y);
	}

	// prepare pt2Now.x
	pt2Now.x = rcNow.right - (rcOrg.right - pt2.x);

	// prepare pt2Now.y
	if (pt1.y < pt2.y)
	{
		pt2Now.y = rcNow.bottom - (rcOrg.bottom - pt2.y);
	}

	pDC->MoveTo(pt1Now);
	pDC->LineTo(pt2Now);

	return 0;
}

int CControlWnd::DrawControlImage(
	CDC *pDC,
	const CTRLPROPERTIES *pCtrlProp,
	const CRect &rcWnd,
	const CRect &rcClient)
{
	if (m_eControlType == CT_SCROLLBAREX)
	{
		const SBEXPROPERTIES *pSbExProp = (const SBEXPROPERTIES *)pCtrlProp;
		if (pSbExProp->m_bVertical)
		{
			VERSBPROPERTIES *pVerSbExProp = (VERSBPROPERTIES *)CUIMgr::GetStyleItem(
					STYLET_VERSCROLLBAR, (LPCTSTR)pSbExProp->m_strVerScrollbarStyleID);

			if (pVerSbExProp != NULL)
			{
				pCtrlProp = pVerSbExProp;
			}
		}
		else
		{
			HORSBPROPERTIES *pHorSbExProp = (HORSBPROPERTIES *)CUIMgr::GetStyleItem(
					STYLET_HORSCROLLBAR, (LPCTSTR)pSbExProp->m_strHorScrollbarStyleID);

			if (pHorSbExProp != NULL)
			{
				pCtrlProp = pHorSbExProp;
			}
		}
	}

	if (pCtrlProp != NULL)
	{
		// If the control specify background image, draw control's self image.
		if ((CONTROL_TYPE)pCtrlProp->m_eControlType == CT_STYLE_VERSCROLLBAR
			|| (CONTROL_TYPE)pCtrlProp->m_eControlType == CT_STYLE_HORSCROLLBAR
			|| (CONTROL_TYPE)pCtrlProp->m_eControlType == CT_WL_PICTURE
			|| (CONTROL_TYPE)pCtrlProp->m_eControlType == CT_PICTURE
			|| (CONTROL_TYPE)pCtrlProp->m_eControlType == CT_STYLE_SPLITTER
			|| (CONTROL_TYPE)pCtrlProp->m_eControlType == CT_SPLITTER
			|| pCtrlProp->m_bSpecifyBackgroundImages)
		{
			IMAGE_RESIZE_MODE eBkImgResizeMode = pCtrlProp->m_eBkImageResizeMode;

			POINT ptImageResize = pCtrlProp->m_ptImageResize;
			SIZE sizeRepeat;
			sizeRepeat.cx = pCtrlProp->m_uXRepeatPixel;
			sizeRepeat.cy = pCtrlProp->m_uYRepeatPixel;

			CRect rcDest(0, 0, rcWnd.Width(), rcWnd.Height());

			CString strBmpNormal[9];
			BOOL bCombineBk = TRUE;
			int nPart = 4;
			if (/* 4 State */ m_eControlType == CT_PUSHBUTTON
				|| m_eControlType == CT_COMBOBOX
				|| m_eControlType == CT_EDIT
				|| m_eControlType == CT_SPIN
				|| m_eControlType == CT_HOTKEY
				|| m_eControlType == CT_IPADDRESS
				|| m_eControlType == CT_SLIDEREX
				|| m_eControlType == CT_SCROLLBAREX
				|| m_eControlType == CT_DATETIMECTRL
				|| m_eControlType == CT_SPLITTER
				|| m_eControlType == CT_WL_BUTTON
				|| m_eControlType == CT_WL_CHECK
				|| m_eControlType == CT_WL_RADIO
				|| m_eControlType == CT_WL_SLIDER)
			{
				nPart = 4;
				CTRLPROPERTIES::IUIGetBackground4(pCtrlProp, &bCombineBk, strBmpNormal);
			}

			else if (/* 2 State */ m_eControlType == CT_STATIC
				|| m_eControlType == CT_PROGRESS
				|| m_eControlType == CT_LISTCTRL
				|| m_eControlType == CT_TREECTRL
				|| m_eControlType == CT_TREELIST
				|| m_eControlType == CT_THUMBNAIL
				|| m_eControlType == CT_WL_TEXT)
			{
				nPart = 2;
				CTRLPROPERTIES::IUIGetBackground2(pCtrlProp, &bCombineBk, strBmpNormal);
			}

			else if (/* 8 State */ m_eControlType == CT_CHECK || m_eControlType == CT_RADIO)
			{
				nPart = 8;
				TOGGLEBTNPROPERTIES *pBtnProp = (TOGGLEBTNPROPERTIES *)pCtrlProp;
				CTRLPROPERTIES::IUIGetToggleButtonBackground8(pBtnProp, &bCombineBk, strBmpNormal);
			}

			else if (/* 1 State */ m_eControlType == CT_WL_PICTURE || m_eControlType == CT_PICTURE)
			{
				nPart = 1;
				strBmpNormal[0] = pCtrlProp->m_strBKN;
			}

			if (bCombineBk)
			{
				if (NULL == m_hImgCombine)
				{
					m_hImgCombine = CreateIUIImage(strBmpNormal[0]);
				}

				if (m_hImgCombine->GetSafeHBITMAP() != NULL)
				{
					if (eBkImgResizeMode == IRM_9GRID)
					{
						IUIPartNineGridBlt(pDC->GetSafeHdc(), rcDest, m_hImgCombine,
							pCtrlProp->m_ptImageResize, pCtrlProp->m_uXRepeatPixel, pCtrlProp->m_uYRepeatPixel,
							nPart, 0);
					}
					else if (eBkImgResizeMode == IRM_STRETCH
						|| eBkImgResizeMode == IRM_STRETCH_HIGH_QUALITY)
					{
						IUIPartStretchBlt(pDC->GetSafeHdc(), rcDest, m_hImgCombine, nPart, 0,
							eBkImgResizeMode);
					}
				}
			}
			else
			{
				if (NULL == m_hImgNormal)
				{
					m_hImgNormal = CreateIUIImage(strBmpNormal[1]);
				}

				if (m_hImgNormal->GetSafeHBITMAP() != NULL)
				{
					if (eBkImgResizeMode == IRM_9GRID)
					{
						IUINineGridBltEx(pDC->GetSafeHdc(), rcDest,
							m_hImgNormal,
							pCtrlProp->m_ptImageResize, pCtrlProp->m_uXRepeatPixel, pCtrlProp->m_uYRepeatPixel,
							-1, (BYTE)(LONG)pCtrlProp->m_lBkImgTransparent);
					}
					else if (eBkImgResizeMode == IRM_STRETCH
						|| eBkImgResizeMode == IRM_STRETCH_HIGH_QUALITY)
					{
						IUIAlphaStretchBlt(pDC->GetSafeHdc(), rcDest,
							m_hImgNormal, (BYTE)(LONG)pCtrlProp->m_lBkImgTransparent,
							eBkImgResizeMode);
					}
					else if (eBkImgResizeMode == IRM_CENTER)
					{
						IUIBitBlt(
							pDC->GetSafeHdc(),
							(rcWnd.Width() - m_hImgNormal->GetWidth()) / 2,
							(rcWnd.Height() - m_hImgNormal->GetHeight()) / 2,
							m_hImgNormal->GetWidth(),
							m_hImgNormal->GetHeight(),
							m_hImgNormal,
							0,
							0,
							SRCCOPY);
					}
				}
			}
		}

		// Draw foreground image of the control
		BOOL bCombineFg = TRUE;
		CString strFgImageName[9];
		FOREGROUND_ALIGN_HOR eFah = FAH_CENTER;
		FOREGROUND_ALIGN_VER eFav = FAV_CENTER;
		CRect rcForegroundMargin(0, 0, 0, 0);
		int nPartCount = 1;
		if (m_eControlType == CT_STATIC || m_eControlType == CT_WL_TEXT)
		{
			TXTPROPERTIES *pTxtProp = (TXTPROPERTIES *)pCtrlProp;

			CTRLPROPERTIES::IUIGetStaticForeground2(pTxtProp, &bCombineFg, strFgImageName);
			eFah = pTxtProp->m_eForegroundHorAlignMode;
			eFav = pTxtProp->m_eForegroundVerAlignMode;
			rcForegroundMargin = (RECT)pTxtProp->m_rcPadding4Foreground;
			nPartCount = 2;
		}
		else if (m_eControlType == CT_PUSHBUTTON || m_eControlType == CT_WL_BUTTON)
		{
			BTNPROPERTIES *pBtnProp = (BTNPROPERTIES *)pCtrlProp;

			CTRLPROPERTIES::IUIGetButtonForeground4(pBtnProp, &bCombineFg, strFgImageName);
			eFah = pBtnProp->m_eForegroundHorAlignMode;
			eFav = pBtnProp->m_eForegroundVerAlignMode;
			rcForegroundMargin = (RECT)pBtnProp->m_rcPadding4Foreground;
			nPartCount = 4;
		}
		else if (m_eControlType == CT_CHECK || m_eControlType == CT_RADIO
			|| m_eControlType == CT_WL_CHECK || m_eControlType == CT_WL_RADIO)
		{
			TOGGLEBTNPROPERTIES *pBtnProp = (TOGGLEBTNPROPERTIES *)pCtrlProp;

			CTRLPROPERTIES::IUIGetToggleButtonForeground8(pBtnProp, &bCombineFg, strFgImageName);
			eFah = pBtnProp->m_eForegroundHorAlignMode;
			eFav = pBtnProp->m_eForegroundVerAlignMode;
			rcForegroundMargin = (RECT)pBtnProp->m_rcPadding4Foreground;
			nPartCount = 8;
		}
		else if (m_eControlType == CT_WL_SLIDER)	// Draw thumb
		{
		}

		if (bCombineFg)
		{
			m_hForeground = CreateIUIImage(strFgImageName[0]);
			IUIPartDrawForeground(pDC->GetSafeHdc(), rcClient, rcForegroundMargin, m_hForeground,
				CRect(0, 0, m_hForeground->GetWidth() / nPartCount, m_hForeground->GetHeight()),
				eFah, eFav, RGB(255, 0, 255), 255);
		}
		else
		{
			m_hForeground = CreateIUIImage(strFgImageName[1]);
			IUIDrawForeground(pDC->GetSafeHdc(),
				rcClient, rcForegroundMargin, m_hForeground, eFah, eFav, RGB(255, 0, 255), 255);
		}
	}

	return 0;
}

int CControlWnd::DrawFocusBorder(CDC *pDC, const CRect &rcWnd)
{
	CRect rcFocus(rcWnd);
	rcFocus.left = 0;
	rcFocus.right = rcWnd.Width();
	rcFocus.top = 0;
	rcFocus.bottom = rcWnd.Height();

	int nOldMode = pDC->SetROP2(R2_COPYPEN);
	pDC->SetBkMode(OPAQUE);
	CPen pen;
	if (m_eControlType == CT_TASK_WND_MGR)
	{
		if (m_nHighlightCount == 0)
		{
			pen.CreatePen(PS_DOT, 1, RGB(0, 255, 0));
		}
		else
		{
			if (m_nHighlightCount % 2)
			{
				pen.CreatePen(PS_DOT, 1, RGB(255, 255, 255));
			}
			else
			{
				pen.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
			}
		}
	}
	else if (m_eControlType == CT_PNL_DOCK
		|| m_eControlType == CT_UNIFORM_GRID
		|| m_eControlType == CT_PNL_STACK)
	{
		if (m_nHighlightCount == 0)
		{
			pen.CreatePen(PS_DOT, 1, RGB(255, 0, 0));
		}
		else
		{
			if (m_nHighlightCount % 2)
			{
				pen.CreatePen(PS_DOT, 1, RGB(255, 255, 255));
			}
			else
			{
				pen.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
			}
		}
	}
	else
	{
		if (m_nHighlightCount == 0)
		{
			pen.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
		}
		else
		{
			if (m_nHighlightCount % 2)
			{
				pen.CreatePen(PS_DOT, 1, RGB(255, 255, 255));
			}
			else
			{
				pen.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
			}
		}
	}

	CPen *pPenOld = pDC->SelectObject(&pen);
	pDC->MoveTo(rcFocus.left, rcFocus.top);
	pDC->LineTo(rcFocus.left, rcFocus.bottom - 1);
	pDC->LineTo(rcFocus.right - 1, rcFocus.bottom - 1);
	pDC->LineTo(rcFocus.right - 1, rcFocus.top);
	pDC->LineTo(rcFocus.left, rcFocus.top);
	pDC->SelectObject(pPenOld);
	pen.DeleteObject();
	pDC->SetROP2(nOldMode);

	return 0;
}

int CControlWnd::DrawSelectedFlag(CDC *pDC, const CRect &rcWnd)
{
	if (!m_bSelected)
	{
		return 1;
	}

	CRect rcFocus = rcWnd;
	rcFocus.OffsetRect(-rcWnd.TopLeft());

	CTRLPROPERTIES *pFocusCtrl = (CTRLPROPERTIES *)GetParent()->SendMessage(WM_GETFOCUSCTRL);
	BOOL bFocus = FALSE;
	if (pFocusCtrl == GetControlProperties())
	{
		bFocus = TRUE;
	}

	const int nSelectedFlagSize = 5;
	CRect rcLeft(rcFocus);
	rcLeft.right = rcLeft.left + nSelectedFlagSize;
	rcLeft.top = rcLeft.top + rcLeft.Height() / 2 - nSelectedFlagSize / 2;
	rcLeft.bottom = rcLeft.top + nSelectedFlagSize;
	pDC->Draw3dRect(rcLeft, RGB(0, 0, 0), RGB(0, 0, 0));
	if (bFocus)
	{
		pDC->FillSolidRect(rcLeft, RGB(0, 0, 0));
	}

	CRect rcTop(rcFocus);
	rcTop.left = rcTop.left + rcTop.Width() / 2 - nSelectedFlagSize / 2;
	rcTop.right = rcTop.left + nSelectedFlagSize;
	rcTop.bottom = rcTop.top + nSelectedFlagSize;
	pDC->Draw3dRect(rcTop, RGB(0, 0, 0), RGB(0, 0, 0));
	if (bFocus)
	{
		pDC->FillSolidRect(rcTop, RGB(0, 0, 0));
	}

	CRect rcRight(rcFocus);
	rcRight.top = rcRight.top + rcRight.Height() / 2 - nSelectedFlagSize / 2;
	rcRight.bottom = rcRight.top + nSelectedFlagSize;
	rcRight.left = rcRight.right - nSelectedFlagSize;
	pDC->Draw3dRect(rcRight, RGB(0, 0, 0), RGB(0, 0, 0));
	if (bFocus)
	{
		pDC->FillSolidRect(rcRight, RGB(0, 0, 0));
	}

	CRect rcBottom(rcFocus);
	rcBottom.left = rcBottom.left + rcBottom.Width() / 2 - nSelectedFlagSize / 2;
	rcBottom.right = rcBottom.left + nSelectedFlagSize;
	rcBottom.top = rcBottom.bottom - nSelectedFlagSize;
	pDC->Draw3dRect(rcBottom, RGB(0, 0, 0), RGB(0, 0, 0));
	if (bFocus)
	{
		pDC->FillSolidRect(rcBottom, RGB(0, 0, 0));
	}

	return 0;
}

int CControlWnd::DrawTypeFlag(CDC *pDC, const CRect &rcClient)
{
	if (!m_bShowType)
	{
		return 1;
	}

	CString strType;
	if (m_eControlType == CT_PUSHBUTTON)
	{
		strType = "BTN";
	}
	else if (m_eControlType == CT_CHECK)
	{
		strType = "CHK";
	}
	else if (m_eControlType == CT_COMBOBOX)
	{
		strType = "CMB";
	}
	else if (m_eControlType == CT_EDIT)
	{
		strType = "EDT";
	}
	else if (m_eControlType == CT_PROGRESS)
	{
		strType = "PRG";
	}
	else if (m_eControlType == CT_RADIO)
	{
		strType = "RAD";
	}
	else if (m_eControlType == CT_SLIDER)
	{
		strType = "SLD";
	}
	else if (m_eControlType == CT_SPIN)
	{
		strType = "SPN";
	}
	else if (m_eControlType == CT_STATIC)
	{
		strType = "STA";
	}
	else if (m_eControlType == CT_HOTKEY)
	{
		strType = "HOT";
	}
	else if (m_eControlType == CT_IPADDRESS)
	{
		strType = "IPA";
	}
	else if (m_eControlType == CT_SLIDEREX)
	{
		strType = "SLDEX";
	}
	else if (m_eControlType == CT_LISTCTRL)
	{
		strType = "LISTCTRL";
	}
	else if (m_eControlType == CT_TREECTRL)
	{
		strType = "TREECTRL";
	}
	else if (m_eControlType == CT_SCROLLBAREX)
	{
		strType = "SBEX";
	}
	else if (m_eControlType == CT_THUMBNAIL)
	{
		strType = "THM";
	}
	else if (m_eControlType == CT_TREELIST)
	{
		strType = "TL";
	}
	else if (m_eControlType == CT_RICHEDIT)
	{
		strType = "RE";
	}
	else if (m_eControlType == CT_RICHEDIT_IM)
	{
		strType = "IMRE";
	}
	else if (m_eControlType == CT_HTMLCTRL)
	{
		strType = "HTMLCTRL";
	}
	else if (m_eControlType == CT_DATETIMECTRL)
	{
		strType = "DATETIMECTRL";
	}
	else if (m_eControlType == CT_MENUBAR)
	{
		strType = _T("MENUBAR");
	}
	else if (m_eControlType == CT_WAVECTRL)
	{
		strType = "WAVECTRL";
	}
	else if (m_eControlType == CT_PICTURE)
	{
		strType = "PIC";
	}
	else if (m_eControlType == CT_SPLITTER)
	{
		strType = "SPL";
	}
	else if (m_eControlType == CT_WL_RECTCTRL)
	{
		strType = "WLRECT";
	}
	else if (m_eControlType == CT_WL_SPLITTER)
	{
		strType = "WLSPL";
	}
	else if (m_eControlType == CT_WL_LINE)
	{
		strType = "WLLINE";
	}
	else if (m_eControlType == CT_WL_TEXT)
	{
		strType = "WLTXT";
	}
	else if (m_eControlType == CT_WL_BUTTON)
	{
		strType = "WLBTN";
	}
	else if (m_eControlType == CT_WL_CHECK)
	{
		strType = "WLCHK";
	}
	else if (m_eControlType == CT_WL_RADIO)
	{
		strType = "WLRAD";
	}
	else if (m_eControlType == CT_WL_SLIDER)
	{
		strType = "WLSLD";
	}
	else if (m_eControlType == CT_WL_PICTURE)
	{
		strType = "WLPIC";
	}
	else if (m_eControlType == CT_WL_RICHEDIT)
	{
		strType = "WLRE";
	}
	else if (m_eControlType == CT_WL_RICHEDIT_IM)
	{
		strType = "WLIM";
	}
	else if (m_eControlType == CT_TASK_WND_MGR)
	{
		strType = "TWMGR";
	}
	else if (m_eControlType == CT_PNL_DOCK)
	{
		strType = "PNL_DOCK";
	}
	else if (m_eControlType == CT_UNIFORM_GRID)
	{
		strType = "PNL_UGRID";
	}
	else if (m_eControlType == CT_PNL_STACK)
	{
		strType = "PNL_STACK";
	}

	COLORREF crOld = pDC->GetBkColor();
	pDC->SetBkColor(RGB(255, 255, 0));
	CRect rect = rcClient;
	pDC->DrawText(strType, rect, DT_SINGLELINE | DT_RIGHT | DT_BOTTOM);
	pDC->SetBkColor(crOld);

	return 0;
}

int CControlWnd::DrawControlID(CDC *pDC, const CRect rcClient)
{
	COLORREF crOld = pDC->GetBkColor();
	pDC->SetBkColor(RGB(255, 0, 255));
	CString strID = m_pCtrlProperties->m_strID;

	CRect rect = rcClient;
	pDC->DrawText(strID, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->SetBkColor(crOld);

	return 0;
}

int CControlWnd::DrawControlIndex(CDC *pDC)
{
	ASSERT(m_nIndex > 0);
	CString strIndex;
	strIndex.Format(_T("%d"), m_nIndex);
	COLORREF crOld = pDC->GetBkColor();
	pDC->SetBkColor(RGB(255, 0, 0));
	pDC->TextOut(0, 0, strIndex);
	pDC->SetBkColor(crOld);

	return 0;
}

// Draw the control.
int CControlWnd::DrawControlText(
	CDC *pDC,
	const CTRLPROPERTIES *pCtrlPropDraw,
	const CRect &rcClient)
{
	pDC->SetBkMode(TRANSPARENT);

	CString strText;
	TEXT_ALIGN_HOR eTextHorAlignMode = TAH_DEFAULT;
	TEXT_ALIGN_VER eTextVerAlignMode = TAV_DEFAULT;
	CRect rcPadding4Text(0, 0, 0, 0);
	HFONT hFont = NULL;

	// Default color
	RESCOLORPROPERTIES *pResDefaultColor = CUIMgr::GetDefaultColorResource();
	COLORREF cr = pResDefaultColor->m_crColor;

	UINT uTextFormat = 0;

	if (m_eControlType == CT_PUSHBUTTON || m_eControlType == CT_CHECK || m_eControlType == CT_RADIO)
	{
		BTNBASEPROPERTIES *p = (BTNBASEPROPERTIES *)pCtrlPropDraw;

		strText = p->m_strCaption;
		eTextHorAlignMode = p->m_eTextHorAlignMode;
		eTextVerAlignMode = p->m_eTextVerAlignMode;
		GetControlPreviewFont(&hFont, pCtrlPropDraw);
		CUIMgr::GetColorResource(pCtrlPropDraw->m_strResColorIDN, &cr);
		rcPadding4Text = (RECT)p->m_rcPadding4Text;

		if (p->m_bMultiline)
		{
			uTextFormat |= DT_WORDBREAK;
		}
		else
		{
			uTextFormat |= DT_SINGLELINE;
		}
	}
	else if (m_eControlType == CT_PROGRESS
		|| m_eControlType == CT_HOTKEY
		|| m_eControlType == CT_IPADDRESS)
	{
	}
	else if (m_eControlType == CT_EDIT)
	{
		EDTPROPERTIES *p = (EDTPROPERTIES *)pCtrlPropDraw;
		GetControlPreviewFont(&hFont, pCtrlPropDraw);
	}
	else if (m_eControlType == CT_STATIC || m_eControlType == CT_WL_TEXT)
	{
		TXTPROPERTIES *p = (TXTPROPERTIES *)pCtrlPropDraw;

		strText = p->m_strCaption;
		eTextHorAlignMode = p->m_eTextHorAlignMode;
		eTextVerAlignMode = p->m_eTextVerAlignMode;
		GetControlPreviewFont(&hFont, pCtrlPropDraw);
		CUIMgr::GetColorResource(pCtrlPropDraw->m_strResColorIDN, &cr);
		RECT rcPadding4Text = p->m_rcPadding4Text;

		if (p->m_bNoWrap)
		{
			uTextFormat |= DT_SINGLELINE;
			if (p->m_bPathEllipsis)
			{
				uTextFormat |= DT_PATH_ELLIPSIS;
			}
			if (p->m_bEndEllipsis)
			{
				uTextFormat |= DT_END_ELLIPSIS;
			}
		}
		else
		{
			uTextFormat |= DT_WORDBREAK;
		}
	}
	else if (m_eControlType == CT_COMBOBOX)
	{
		CMBPROPERTIES *p = (CMBPROPERTIES *)pCtrlPropDraw;
		GetControlPreviewFont(&hFont, pCtrlPropDraw);
	}
	else if (m_eControlType == CT_SLIDER
		|| m_eControlType == CT_SPIN
		|| m_eControlType == CT_SLIDEREX
		|| m_eControlType == CT_LISTCTRL
		|| m_eControlType == CT_TREECTRL
		|| m_eControlType == CT_SCROLLBAREX
		|| m_eControlType == CT_THUMBNAIL
		|| m_eControlType == CT_TREELIST
		|| m_eControlType == CT_RICHEDIT
		|| m_eControlType == CT_RICHEDIT_IM
		|| m_eControlType == CT_HTMLCTRL
		|| m_eControlType == CT_DATETIMECTRL
		|| m_eControlType == CT_WAVECTRL
		|| m_eControlType == CT_PICTURE
		|| m_eControlType == CT_MENUBAR
		|| m_eControlType == CT_SPLITTER)
	{
	}

	// draw window text
	DrawFormatText(pDC->GetSafeHdc(),
		strText, strText.GetLength(),
		rcClient, &rcPadding4Text, eTextHorAlignMode, eTextVerAlignMode, uTextFormat,
		hFont, cr, m_eControlType);

	DeleteObject(hFont);

	return 0;
}

void CControlWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CUIDesignWnd *pParentWnd = (CUIDesignWnd *)GetParent();
	ASSERT(pParentWnd->GetSafeHwnd() != NULL);
	if (pParentWnd->m_bmpMemBK.GetSafeHandle() == NULL)
	{
		return;
	}

	// If the current story frame is -1, use m_pCtrlProperties to draw;
	// If current in story frame, use properties at current frame;
	// If current frame not include this control, use nearly properties in current frame.
	CTRLPROPERTIES *pCtrlPropDraw = m_pCtrlProperties;
	UIWNDPROPERTIES *pBindUIWndProp = pParentWnd->GetUIWndProperties();
	if (pParentWnd->GetCurStoryFrame() > -1)
	{
		pCtrlPropDraw = pBindUIWndProp->FindLeftNearControlInStoryBoard(m_pCtrlProperties->m_strID,
				pParentWnd->GetCurStoryFrame(), TRUE);
	}
	ASSERT(pCtrlPropDraw != NULL);

	// draw background
	CRect rcWnd;
	GetWindowRect(&rcWnd);
	GetParent()->ScreenToClient(rcWnd);

	CRect rcClient;
	GetClientRect(rcClient);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, rcWnd.Width(), rcWnd.Height());
	CBitmap *pbmpOld = memDC.SelectObject(&bmp);

	// Draw parent background to control, if background can't be changed.
	BitBltG(memDC.GetSafeHdc(), 0, 0, rcWnd.Width(), rcWnd.Height(),
		(HBITMAP)pParentWnd->m_bmpMemBK.GetSafeHandle(), rcWnd.left, rcWnd.top, SRCCOPY);

	// Draw CT_WL_RECTCTRL and CT_WL_SPLITTER type windowless color block.
	if (m_eControlType == CT_WL_RECTCTRL || m_eControlType == CT_WL_SPLITTER)
	{
		DrawWindowlessColorBlock(&memDC, rcWnd);
	}
	// Draw line
	else if (m_eControlType == CT_WL_LINE)
	{
		DrawWindowlessLine(&memDC, (const LINEWLPROPERTIES *)pCtrlPropDraw);
	}
	// Draw image
	else
	{
		DrawControlImage(&memDC, pCtrlPropDraw, rcWnd, rcClient);
	}

	// draw focus rectangle
	DrawFocusBorder(&memDC, rcWnd);

	// draw selected flag
	if (m_bSelected)
	{
		DrawSelectedFlag(&memDC, rcWnd);
	}

	// draw type flag
	if (m_bShowType)
	{
		DrawTypeFlag(&memDC, rcClient);
	}

	if (m_bShowID)
	{
		DrawControlID(&memDC, rcClient);
	}

	if (m_bShowIndex)
	{
		DrawControlIndex(&memDC);
	}

	if (!m_bShowIndex && !m_bShowType && !m_bShowID)
	{
		DrawControlText(&memDC, pCtrlPropDraw, rcClient);
	}

	dc.BitBlt(0, 0, rcWnd.Width(), rcWnd.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pbmpOld);

	// Do not call CWnd::OnPaint() for painting messages
}

#if WINVER >= 0x0502
LRESULT CControlWnd::OnNcHitTest(CPoint point)
#else
UINT CControlWnd::OnNcHitTest(CPoint point)
#endif
{
	// TODO: Add your message handler code here and/or call default
	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcLeft(rcClient);
	rcLeft.right = rcLeft.left + 2;
	rcLeft.top += 2;
	rcLeft.bottom -= 2;

	CRect rcTop(rcClient);
	rcTop.left += 2;
	rcTop.right -= 2;
	rcTop.bottom = rcTop.top + 2;

	CRect rcRight(rcClient);
	rcRight.left = rcRight.right - 2;
	rcRight.top += 2;
	rcRight.bottom -= 2;

	CRect rcBottom(rcClient);
	rcBottom.left += 2;
	rcBottom.top = rcBottom.bottom - 2;
	rcBottom.right -= 2;

	CRect rcLT(rcClient);
	rcLT.right = rcLT.left + 2;
	rcLT.bottom = rcLT.top + 2;

	CRect rcRT(rcClient);
	rcRT.left = rcRT.right - 2;
	rcRT.bottom = rcRT.top + 2;

	CRect rcLB(rcClient);
	rcLB.right = rcLB.top + 2;
	rcLB.top = rcLB.bottom - 2;

	CRect rcRB(rcClient);
	rcRB.left = rcRB.right - 2;
	rcRB.top = rcRB.bottom - 2;

	POINT pt(point);
	ScreenToClient(&pt);
	point = pt;
	rcClient.DeflateRect(2, 2, 2, 2);

	if (rcLeft.PtInRect(point))
	{
		return HTLEFT;
	}
	else if (rcTop.PtInRect(point))
	{
		return HTTOP;
	}
	else if (rcRight.PtInRect(point))
	{
		return HTRIGHT;
	}
	else if (rcBottom.PtInRect(point))
	{
		return HTBOTTOM;
	}
	else if (rcLT.PtInRect(point))
	{
		return HTTOPLEFT;
	}
	else if (rcRT.PtInRect(point))
	{
		return HTTOPRIGHT;
	}
	else if (rcRB.PtInRect(point))
	{
		return HTBOTTOMRIGHT;
	}
	else if (rcLB.PtInRect(point))
	{
		return HTBOTTOMLEFT;
	}
	else if (rcClient.PtInRect(point))
	{
		return HTCAPTION;
	}

	return -2;

	//	return CWnd::OnNcHitTest(point);
}

void CControlWnd::OnGetMinMaxInfo(MINMAXINFO *lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	BOOL bIsComboBox = (m_eControlType == CT_COMBOBOX);
	if (bIsComboBox)
	{
		lpMMI->ptMinTrackSize.y = 6;	// The border height of top add bottom
		lpMMI->ptMaxTrackSize.y = 255 + 6;
	}

	CWnd::OnGetMinMaxInfo(lpMMI);
}

void CControlWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CMenu menu;
	menu.LoadMenu(IDR_CONTROL_PROPERTIES);
	CMenu *pMenu;
	pMenu = menu.GetSubMenu(0);
	ASSERT(pMenu->GetSafeHmenu() != NULL);
	POINT pt;
	GetCursorPos(&pt);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);

	CWnd::OnRButtonUp(nFlags, point);
}

void CControlWnd::OnMoving(UINT fwSide, LPRECT pRect)
{
	CWnd::OnMoving(fwSide, pRect);

	// TODO: Add your message handler code here

	if (this == m_pMoveSender)
	{
		CRect rc;
		GetWindowRect(rc);

		// m_nXMove和m_nYMove记录的是本次移动与上次的偏移，不是相对原始位置的偏移。
		m_nXMove = pRect->left - rc.left;
		m_nYMove = pRect->top - rc.top;

		GetParent()->SendMessage(WM_CHILDMOVING, WPARAM(this), 0);
	}
	Invalidate();
}

// Only handle mouse move or resize. not handle keyboard move or resize.
LRESULT CControlWnd::OnExitSizeMove(WPARAM wParam, LPARAM lParam)
{
	m_pMoveSender = NULL;
	Invalidate();

	// If the control moved or resized, record to undo list
	if (m_bPressedLeftButton)	// The WM_NCLBUTTONDOWN message has happened.
	{
		CRect rcThisNow;
		GetWindowRect(rcThisNow);
		GetParent()->ScreenToClient(rcThisNow);

		// Add to undo list
		if ((rcThisNow != m_rcPressed))	// Has moved or resized.
		{
			::CAction action;
			action.m_eAction = ::CAction::actionMoveControl;
			CUIDesignWnd *pParentWnd = (CUIDesignWnd *)GetParent();
			ASSERT(pParentWnd->GetSafeHwnd() != NULL);
			action.m_pUIWndProp = pParentWnd->GetUIWndProperties();

			int nXOffset = rcThisNow.left - m_rcPressed.left;
			int nYOffset = rcThisNow.top - m_rcPressed.top;

			CUIDesignWnd *pUIDesignWnd = (CUIDesignWnd *)GetParent();
			vector<CControlWnd *> vSelectedControls;
			pUIDesignWnd->GetSelectedControls(&vSelectedControls);

			size_t nSelCount = vSelectedControls.size();
			for (size_t i = 0; i < nSelCount; ++i)
			{
				CControlWnd *pCtrlWnd = vSelectedControls[i];

				CRect rcNow;
				pCtrlWnd->GetWindowRect(rcNow);
				GetParent()->ScreenToClient(rcNow);

				CRect rcOld(rcNow);

				if ((rcThisNow.Width() == m_rcPressed.Width())
					&& (rcThisNow.Height() == m_rcPressed.Height()))	// Has moved, not resized.
				{
					rcOld.DeflateRect(-nXOffset, -nYOffset, nXOffset, nYOffset);
				}
				else	// Has resized
				{
					// Only can resize one control at the same time
					// by using mouse even selected more than one control.
					if (pCtrlWnd != this)
					{
						continue;
					}

					rcOld = m_rcPressed;
				}

				CControlMove ctrlMove;
				ctrlMove.m_pCtrlProp = pCtrlWnd->GetControlProperties();
				ctrlMove.m_rectOld = rcOld;
				ctrlMove.m_rectNew = rcNow;
				action.m_ControlMove.AddHead(ctrlMove);
			}

			CUIShopDoc *pDoc = g_pFrm->GetUIShopDoc();
			ASSERT(pDoc != NULL);
			pDoc->AddUndo(&action);
		}
	}

	m_bPressedLeftButton = FALSE;	// reset

	return 0;
}

void CControlWnd::OnWindowPosChanged(WINDOWPOS FAR *lpwndpos)
{
	CWnd::OnWindowPosChanged(lpwndpos);

	// TODO: Add your message handler code here

	// Update control coordinate
	if (lpwndpos->flags & SWP_NOSIZE && lpwndpos->flags & SWP_NOMOVE)
	{

	}
	else
	{
		CUIDesignWnd *pWnd = (CUIDesignWnd *)GetParent();
		ASSERT(pWnd->GetSafeHwnd() != NULL);
		pWnd->UpdateControlCoordinate(this);

		// Set modified flag
		CUIShopDoc *pDoc = g_pFrm->GetUIShopDoc();
		ASSERT(pDoc != NULL);
		pDoc->SetModifiedFlag(TRUE);
	}
}

void CControlWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CUIDesignWnd *pParent = (CUIDesignWnd *)GetParent();
	pParent->SetFocus();	// Set focus to parent to handle WM_KEYDOWN message.

	SHORT nCtrlKeyState = GetKeyState(VK_CONTROL);
	if (!m_bSelected)
	{
		if (nCtrlKeyState >= 0) // CTRL key is not down
		{
			pParent->SelectAllControls(this, FALSE);
		}

		Selected(TRUE);
	}

	// Is the control already be selected, un-selected
	// other controls and select itself after left button release.
	else
	{
		if (nCtrlKeyState < 0) // CTRL key is down
		{
			pParent->SetFocusCtrl(GetControlProperties(), FALSE);
			Selected(FALSE);
			return; //return to refuse to drag control, otherwise,
			// user can drag control before release left button.
		}
	}

	pParent->SetFocusCtrl(GetControlProperties(), TRUE);
	m_pMoveSender = this;

	// Record move control, must call before SendMessage(WM_NCLBUTTONUP, HTCAPTION, lParam)
	// and CWnd::OnNcLButtonDown if need, since the SendMessage is sync.
	m_bPressedLeftButton = TRUE;
	GetWindowRect(m_rcPressed);
	GetParent()->ScreenToClient(m_rcPressed);

	CWnd::OnNcLButtonDown(nHitTest, point);

	POINT pt;
	GetCursorPos(&pt);
	LPARAM lParam = MAKELPARAM(pt.x, pt.y);
	SendMessage(WM_NCLBUTTONUP, HTCAPTION, lParam);

	// For format brush
	BOOL bFormatBrush = g_pFrm->IsFormatBrushSelected();
	if (bFormatBrush)
	{
		const CTRLPROPERTIES *pFormatBrush = g_pFrm->GetFormatBrushProp();

		if (pFormatBrush == NULL)	// The control is template
		{
			g_pFrm->SetFormatBrushProp(m_pCtrlProperties);

			CFormatBrushDlg dlg;
			dlg.SetCtrlProp(m_pCtrlProperties);
			dlg.DoModal();
		}
		else	// The control need be set
		{
			if ((CONTROL_TYPE)pFormatBrush->m_eControlType
				!= (CONTROL_TYPE)m_pCtrlProperties->m_eControlType)
			{
				AfxMessageBox(_T("The control type not match!"));
			}
			else
			{
				const std::vector<INT_PTR> *pPropIndex = g_pFrm->GetFormatBrushPropIndex();

				size_t nIndexCount = pPropIndex->size();
				for (int i = 0; i < nIndexCount; ++i)
				{
					INT nIndex = (INT)(INT_PTR)(*pPropIndex)[i];		// The nIndex prop need be format.
					const IUI_VARIANT *pVar = pFormatBrush->GetProperty(nIndex);
					m_pCtrlProperties->SetProperty(nIndex, pVar);
				}
			}
		}
	}
}

void CControlWnd::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	// OnExitSizeMove set m_pMoveSender to NULL, if m_pMoveSender
	// isn't NULL at this step, it means the control not be move or resized.
	// this control will be selected and others be unselected while left button released.

	SHORT nCtrlKeyState = GetKeyState(VK_CONTROL);
	CUIDesignWnd *pParent = (CUIDesignWnd *)GetParent();

	if (m_pMoveSender != NULL)
	{
		if (m_bSelected)
		{
			if (nCtrlKeyState < 0) // CTRL key is down
			{

			}
			else
			{
				pParent->SelectAllControls(this, FALSE);
			}
		}
	}

	m_pMoveSender = NULL;

	CWnd::OnNcLButtonUp(nHitTest, point);
}

void CControlWnd::OnCtrlProperties()
{
	// TODO: Add your command handler code here
	ShowPropertiesDlg();
}

//////////////////////////////////////////////////////////////////////////

void CControlWnd::SetControlType(CONTROL_TYPE eControlType)
{
	m_eControlType = eControlType;
}

void CControlWnd::SetControlProperties(CTRLPROPERTIES *pCtrlProperties)
{
	ASSERT(pCtrlProperties != NULL);

	m_pCtrlProperties = pCtrlProperties;
}

CTRLPROPERTIES *CControlWnd::GetControlProperties()
{
	return m_pCtrlProperties;
}

void CControlWnd::Selected(BOOL bSelected)
{
	if (m_bSelected != bSelected)
	{
		m_bSelected = bSelected;
	}

	Invalidate();
	UpdateWindow();
}

BOOL CControlWnd::IsSelected()
{
	return m_bSelected;
}

void CControlWnd::SetTypeFlag(BOOL bShow)
{
	m_bShowType = bShow;
}

void CControlWnd::ShowIDFlag(BOOL bShow)
{
	m_bShowID = bShow;
}

void CControlWnd::ShowIndexValue(BOOL bShow)
{
	m_bShowIndex = bShow;
}

void CControlWnd::SetIndexValue(int nIndex)
{
	m_nIndex = nIndex;
}

//////////////////////////////////////////////////////////////////////////

void CControlWnd::ShowPropertiesDlg()
{
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	ASSERT(pFrm != NULL);
	CPropertiesWnd *pPropWnd = pFrm->GetPropertiesWnd();
	pPropWnd->ShowWindow(SW_SHOW);
}

int CControlWnd::GetMoveOffset(int *pnXMove, int *pnYMove)
{
	*pnXMove = m_nXMove;
	*pnYMove = m_nYMove;

	return 0;
}

int CControlWnd::HighlightControl()
{
	m_nHighlightCount = 0;
	SetTimer(ID_TIMER_HIGHLIGHT, 100, NULL);

	return 0;
}

void CControlWnd::OnNcRButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CMenu menu;
	menu.LoadMenu(IDR_CONTROL_PROPERTIES);
	CMenu *pMenu;
	pMenu = menu.GetSubMenu(0);
	ASSERT(pMenu->GetSafeHmenu() != NULL);
	POINT pt;
	GetCursorPos(&pt);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);

	CWnd::OnNcRButtonUp(nHitTest, point);
}

void CControlWnd::OnCtrlAddHandler()
{
	CMsgWizardDlg dlg;
	dlg.DoModal();
}

void CControlWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == ID_TIMER_HIGHLIGHT)
	{
		++m_nHighlightCount;

		if (m_nHighlightCount > 5)
		{
			KillTimer(nIDEvent);

			m_nHighlightCount = 0;
		}

		Invalidate();
	}

	CWnd::OnTimer(nIDEvent);
}
