#pragma once


#include "ResourceMgr.h"
using namespace ImageManager;

namespace LibUIDK
{
	struct BUTTONMEMBER
	{
		BUTTONMEMBER()
		{
			m_bWLButton = FALSE;
			m_eButtonControlType = CT_ARROW;

			int i = 0;
			for (i = 0; i < 4; ++i)
			{
				m_hIUIFont[i] = NULL;
				m_hIUIFontC[i] = NULL;
				m_cr[i] = RGB(0, 0, 0);
				m_crC[i] = RGB(0, 0, 0);
				m_rcTextMargin[i].SetRect(0, 0, 0, 0);
				m_rcTextMarginC[i].SetRect(0, 0, 0, 0);
				m_eTextHorAlignMode[i] = TAH_DEFAULT;
				m_eTextHorAlignModeC[i] = TAH_DEFAULT;
				m_eTextVerAlignMode[i] = TAV_DEFAULT;
				m_eTextVerAlignModeC[i] = TAV_DEFAULT;
			}

			m_bMultiLine = FALSE;
			m_bAnimateText = FALSE;
			m_eAnimateDirection = LEFT;
			m_nAnimateDelay = 0;
			m_nLeftAlign = 0;
			m_nTopAlign = 0;
			m_nAnimatePixel = 1;
			m_bAnimateReset = FALSE;
			m_sizeText.cx = m_sizeText.cy = 0;

			for (i = 0; i < 4; ++i)
			{
				m_himgBk[i] = NULL;
				m_himgBkC[i] = NULL;
			}

			m_bMaskCombine = true;
			m_himgMaskCombine = NULL;
			for (i = 0; i < 4; ++i)
			{
				m_hbmpMask[i] = NULL;
				m_hbmpCheckedMask[i] = NULL;
				m_hBmpForeground[i] = NULL;
			}
			m_eForegroundAlignHor = FAH_LEFT;
			m_eForegroundAlignVer = FAV_CENTER;
			m_crMask = RGB(255, 0, 255);

			for (i = 0; i < 4; ++i)
			{
				m_hBmpForegroundC[i] = NULL;
			}
			m_eForegroundAlignHorC = FAH_LEFT;
			m_eForegroundAlignVerC = FAV_CENTER;
			m_rcForegroundMargin.SetRect(0, 0, 0, 0);
			m_crMaskC = RGB(255, 0, 255);

			m_hIcon = NULL;
			m_hCheckedIcon = NULL;

			m_bEndEllipsis = FALSE;
			m_bTabStop = FALSE;

			m_nWidth = 0;
			m_nHeight = 0;
			m_bMouseOver = FALSE;

			m_eButtonType = BT_UNKNOWN;
			m_eCheckBoxType = CT_UNKNOWN;

			m_bUseActiveArea = FALSE;
			m_rcActive.SetRect(0, 0, 0, 0);
			m_bActiveAreaMouseOver = FALSE;

			m_dwHoverTime = HOVER_DEFAULT;
			m_bHoverRepeat = FALSE;

			m_uBindModalDialogID = 0;
			m_uBindModelessWndID = 0;
			m_uBindTaskWndMgrID = 0;
			m_uBindTaskWindowID = 0;

			m_bRecordLButtonDown = FALSE;
			m_nCheck = BST_UNCHECKED;
			m_bDraw = TRUE;
		}

		int Release()
		{
			m_bWLButton = FALSE;
			m_eButtonControlType = CT_ARROW;

			int i = 0;
			for (i = 0; i < 4; ++i)
			{
				ReleaseIUIFontInternal(m_hIUIFont[i]);
				m_hIUIFont[i] = NULL;
				ReleaseIUIFontInternal(m_hIUIFontC[i]);
				m_hIUIFontC[i] = NULL;
				m_cr[i] = RGB(0, 0, 0);
				m_crC[i] = RGB(0, 0, 0);
				m_rcTextMargin[i].SetRect(0, 0, 0, 0);
				m_rcTextMarginC[i].SetRect(0, 0, 0, 0);
				m_eTextHorAlignMode[i] = TAH_DEFAULT;
				m_eTextHorAlignModeC[i] = TAH_DEFAULT;
				m_eTextVerAlignMode[i] = TAV_DEFAULT;
				m_eTextVerAlignModeC[i] = TAV_DEFAULT;
			}

			m_bMultiLine = FALSE;
			m_bAnimateText = FALSE;
			m_eAnimateDirection = LEFT;
			m_nAnimateDelay = 0;
			m_nLeftAlign = 0;
			m_nTopAlign = 0;
			m_nAnimatePixel = 1;
			m_bAnimateReset = FALSE;
			m_sizeText.cx = m_sizeText.cy = 0;

			for (i = 0; i < 4; ++i)
			{
				ReleaseIUIImage(m_himgBk[i]);
				m_himgBk[i] = NULL;
				ReleaseIUIImage(m_himgBkC[i]);
				m_himgBkC[i] = NULL;
			}

			m_bMaskCombine = true;
			ReleaseIUIImage(m_himgMaskCombine);
			m_himgMaskCombine = NULL;
			for (i = 0; i < 4; ++i)
			{
				ReleaseIUIImage(m_hbmpMask[i]);
				m_hbmpMask[i] = NULL;
				ReleaseIUIImage(m_hbmpCheckedMask[i]);
				m_hbmpCheckedMask[i] = NULL;
				ReleaseIUIImage(m_hBmpForeground[i]);
				m_hBmpForeground[i] = NULL;
			}
			m_eForegroundAlignHor = FAH_LEFT;
			m_eForegroundAlignVer = FAV_CENTER;
			m_crMask = RGB(255, 0, 255);

			for (i = 0; i < 4; ++i)
			{
				ReleaseIUIImage(m_hBmpForegroundC[i]);
				m_hBmpForegroundC[i] = NULL;
			}
			m_eForegroundAlignHorC = FAH_LEFT;
			m_eForegroundAlignVerC = FAV_CENTER;
			m_rcForegroundMargin.SetRect(0, 0, 0, 0);
			m_crMaskC = RGB(255, 0, 255);

			// see DestroyIcon in MSDN:
			// It is only necessary to call DestroyIcon for icons and cursors created
			// with the following functions:
			// CreateIconFromResourceEx (if called without the LR_SHARED flag),
			// CreateIconIndirect, and CopyIcon.
			// Do not use this function to destroy a shared icon. A shared icon is
			// valid as long as the module from which it was loaded remains in memory.
			// The following functions obtain a shared icon:
			//	LoadIcon
			//	LoadImage (if you use the LR_SHARED flag)
			//	CopyImage (if you use the LR_COPYRETURNORG
			//		flag and the hImage parameter is a shared icon)
			//	CreateIconFromResource
			//	CreateIconFromResourceEx (if you use the LR_SHARED flag)
			m_hIcon = NULL;
			m_hCheckedIcon = NULL;

			m_bEndEllipsis = FALSE;
			m_bTabStop = FALSE;

			m_nWidth = 0;
			m_nHeight = 0;
			m_bMouseOver = FALSE;

			m_eButtonType = BT_UNKNOWN;
			m_eCheckBoxType = CT_UNKNOWN;

			m_bUseActiveArea = FALSE;
			m_rcActive.SetRect(0, 0, 0, 0);
			m_bActiveAreaMouseOver = FALSE;

			m_dwHoverTime = HOVER_DEFAULT;
			m_bHoverRepeat = FALSE;

			m_uBindModalDialogID = 0;
			m_uBindModelessWndID = 0;
			m_uBindTaskWndMgrID = 0;
			m_uBindTaskWindowID = 0;

			m_bRecordLButtonDown = FALSE;
			m_nCheck = BST_UNCHECKED;
			m_bDraw = TRUE;

			return 0;
		}

		BOOL m_bWLButton;
		CONTROL_TYPE m_eButtonControlType;

		HIUIFONT m_hIUIFont[4];
		HIUIFONT m_hIUIFontC[4];

		COLORREF m_cr[4];
		COLORREF m_crC[4];

		CRect m_rcTextMargin[4];
		CRect m_rcTextMarginC[4];

		TEXT_ALIGN_HOR m_eTextHorAlignMode[4];
		TEXT_ALIGN_HOR m_eTextHorAlignModeC[4];
		TEXT_ALIGN_VER m_eTextVerAlignMode[4];
		TEXT_ALIGN_VER m_eTextVerAlignModeC[4];

		BOOL m_bMultiLine;
		BOOL m_bAnimateText;
		Direction m_eAnimateDirection;
		int m_nAnimateDelay;
		int m_nLeftAlign;
		int m_nTopAlign;
		int m_nAnimatePixel;
		BOOL m_bAnimateReset;
		CSize m_sizeText;

		HIUIIMAGE m_himgBk[4];
		// the checked bitmap of normal, highlight,
		// selected and disable state for the check box and radio box
		HIUIIMAGE m_himgBkC[4];

		// the mask bitmap for checked and unchecked status
		bool m_bMaskCombine;
		HIUIIMAGE m_himgMaskCombine;
		HIUIIMAGE m_hbmpMask[4];
		HIUIIMAGE m_hbmpCheckedMask[4];

		// foreground bitmap for unchecked status
		HIUIIMAGE m_hBmpForeground[4];
		FOREGROUND_ALIGN_HOR m_eForegroundAlignHor;
		FOREGROUND_ALIGN_VER m_eForegroundAlignVer;
		COLORREF m_crMask;
		// foreground bitmap for checked status
		HIUIIMAGE m_hBmpForegroundC[4];
		FOREGROUND_ALIGN_HOR m_eForegroundAlignHorC;
		FOREGROUND_ALIGN_VER m_eForegroundAlignVerC;
		CRect m_rcForegroundMargin;
		COLORREF m_crMaskC;

		//
		HICON m_hIcon;
		HICON m_hCheckedIcon;

		BOOL m_bEndEllipsis;
		// the radio button will auto get WS_TABSTOP style when click
		// it or move focus to it. even create it with no WS_TABSTOP style.
		BOOL m_bTabStop;

		int m_nWidth;	// the width of the background bitmap
		int m_nHeight;	// the height of the background bitmap
		BOOL m_bMouseOver;
		BOOL m_bRecordLButtonDown;

		// for tool tip
		CToolTipCtrl m_wndToolTip;

		CString m_strTag;

		BTN_TYPE m_eButtonType;
		CHK_TYPE m_eCheckBoxType;

		// for Active area
		BOOL m_bUseActiveArea;
		CRect m_rcActive;
		BOOL m_bActiveAreaMouseOver;

		// for mouse hover
		DWORD m_dwHoverTime;
		BOOL m_bHoverRepeat;

		// for popup bind window
		UINT m_uBindModalDialogID;
		UINT m_uBindModelessWndID;

		// for Task window while the control is radio button
		UINT m_uBindTaskWndMgrID;
		UINT m_uBindTaskWindowID;

		// for CWLButton
		int m_nCheck;		// retore the checked state of check box or radio button

		BOOL m_bDraw;
	};

	//
	struct SLIDEREXMEMBER
	{
		SLIDEREXMEMBER()
		{
			m_nMin = 0;
			m_nMax = 100;
			m_nPos = 0;
			m_nDragPos = 0;

			m_nProgressPos = 0;
			m_nDeflateLTUP = 0;
			m_nDeflateRTDN = 0;

			m_sizeThumb.cx = m_sizeThumb.cy = 0;
			m_ptDragThumb.x = m_ptDragThumb.y = 0;
			m_rcTrackingThumb.SetRect(0, 0, 0, 0);

			m_dwStyle = 0;
			m_dwStyleEx = TBS_EX_NORMAL;
			m_bCaptureThumb = FALSE;
			m_bIsMouseOverThumb = FALSE;
			m_bIsMouseOver = FALSE;
			m_bUseMark = FALSE;
			m_bMarkToThumb = FALSE;

			int i = 0;
			for (i = 0; i < 4; ++i)
			{
				m_himgBk[i] = NULL;
			}

			m_bThumbCombine = true;
			m_himgCombineThumb = NULL;
			for (i = 0; i < 4; ++i)
			{
				m_himgThumb[i] = NULL;
			}

			m_bEnableTooltips = FALSE;
			m_bMouseMoveTooltips = TRUE;

			m_bEnableTipWindow = FALSE;
			m_pTipWnd = NULL;
			m_sizeTipWindowOffset.cx = m_sizeTipWindowOffset.cy = 0;

			m_nSplitCount = 10;
			m_pnSplitList = NULL;
		}

		int Release()
		{
			m_nMin = 0;
			m_nMax = 100;
			m_nPos = 0;
			m_nDragPos = 0;

			m_nProgressPos = 0;
			m_nDeflateLTUP = 0;
			m_nDeflateRTDN = 0;

			m_sizeThumb.cx = m_sizeThumb.cy = 0;
			m_ptDragThumb.x = m_ptDragThumb.y = 0;
			m_rcTrackingThumb.SetRect(0, 0, 0, 0);

			m_dwStyle = 0;
			m_dwStyleEx = TBS_EX_NORMAL;
			m_bCaptureThumb = FALSE;
			m_bIsMouseOverThumb = FALSE;
			m_bIsMouseOver = FALSE;
			m_bUseMark = FALSE;
			m_bMarkToThumb = FALSE;

			int i = 0;
			for (i = 0; i < 4; ++i)
			{
				ReleaseIUIImage(m_himgBk[i]);
				m_himgBk[i] = NULL;
			}

			m_bThumbCombine = true;
			ReleaseIUIImage(m_himgCombineThumb);
			m_himgCombineThumb = NULL;
			for (i = 0; i < 4; ++i)
			{
				ReleaseIUIImage(m_himgThumb[i]);
				m_himgThumb[i] = NULL;
			}

			m_bEnableTooltips = FALSE;
			m_bMouseMoveTooltips = TRUE;

			m_bEnableTipWindow = FALSE;
			if (m_pTipWnd != NULL)
			{
				m_pTipWnd->DestroyWindow();
				m_pTipWnd = NULL;
			}
			m_sizeTipWindowOffset.cx = m_sizeTipWindowOffset.cy = 0;

			m_ColorMap.clear();
			m_nSplitCount = 10;
			SafeDelete(m_pnSplitList);

			return 0;
		}

		int m_nMin;
		int m_nMax;
		int m_nPos;		// current position
		int m_nDragPos; // the position while begin dragging the thumb

		int m_nProgressPos; // only for TBS_EX_PROGRESS style

		int m_nDeflateLTUP;
		int m_nDeflateRTDN;

		CSize m_sizeThumb;
		CPoint m_ptDragThumb;
		CRect m_rcTrackingThumb;

		DWORD m_dwStyle;
		DWORD m_dwStyleEx;
		BOOL m_bCaptureThumb;
		BOOL m_bIsMouseOverThumb;
		BOOL m_bIsMouseOver;
		BOOL m_bUseMark;
		BOOL m_bMarkToThumb;

		// the bitmap of Highlight state used to fill left area with Horizontal style
		// or bottom area with Vertical style when m_bUseMark is TRUE
		HIUIIMAGE m_himgBk[4];

		bool m_bThumbCombine;
		HIUIIMAGE m_himgCombineThumb;
		HIUIIMAGE m_himgThumb[4];

		// General tooltip
		CToolTipCtrl m_tooltip;
		BOOL m_bEnableTooltips;
		BOOL m_bMouseMoveTooltips;

		// For custom tip window
		BOOL m_bEnableTipWindow;
		CWnd *m_pTipWnd;
		CSize m_sizeTipWindowOffset;

		// only support TBS_EX_SPLITPROGRESS
		std::map<int, HBITMAP> m_ColorMap;
		int m_nSplitCount;
		int *m_pnSplitList;

		CBitmap m_bmpCompatible;	// the compatible bitmap to CPaintDC in WM_PAINT message.
	};

	struct WLWNDMEMBER
	{
		WLWNDMEMBER()
		{
#ifdef _DEBUG
			m_nAllocIndex = 0;
#endif // _DEBUG
			m_bVisible = TRUE;
			m_bEnabled = TRUE;
			m_bNotify = TRUE;

			m_strClassName = _T("IUIWL_Wnd");

			m_hBackground = NULL;
			m_sizeRepeat.cx = m_sizeRepeat.cy = 1;

			m_dwStyle = 0;
			m_nID = 0;
			m_pParent = NULL;
			m_rcOrig.SetRect(0, 0, 0, 0);
			m_rcCurrent.SetRect(0, 0, 0, 0);
			m_bCreated = FALSE;

			m_bEnterSizeMove = FALSE;
			m_nTitlebarHeight = 0;
			m_hRgn = NULL;
			m_pVirtualParent = NULL;
			m_ptDrawOffset.x = m_ptDrawOffset.y = 0;
			m_bMouseOver = FALSE;
			m_ptLButtonDown = CPoint(-1, -1);
			m_rcLButtonDownRelateParent.SetRectEmpty();
			m_uLButtonDownHitTestRet = HTCLIENT;

			m_uCanDragResize = 0;
			m_nCXBorder = m_nCYBorder = 4;
			m_sizeScroll = CSize(16, 16);
			m_dwHoverTime = HOVER_DEFAULT;

			m_lpParam = 0;
		}

		int Release()
		{
#ifdef _DEBUG
			m_nAllocIndex = 0;
#endif // _DEBUG

			m_bVisible = TRUE;
			m_bEnabled = TRUE;
			m_bNotify = TRUE;

			m_strClassName = _T("IUIWL_Wnd");

			ReleaseIUIImage(m_hBackground);
			m_hBackground = NULL;
			m_sizeRepeat.cx = m_sizeRepeat.cy = 1;

			m_dwStyle = 0;
			m_nID = 0;
			m_pParent = NULL;
			m_rcOrig.SetRect(0, 0, 0, 0);
			m_rcCurrent.SetRect(0, 0, 0, 0);
			m_bCreated = FALSE;

			m_bEnterSizeMove = FALSE;
			m_nTitlebarHeight = 0;
			DeleteObject(m_hRgn);
			m_hRgn = NULL;
			m_pVirtualParent = NULL;
			m_ptDrawOffset.x = m_ptDrawOffset.y = 0;
			m_bMouseOver = FALSE;
			m_ptLButtonDown = CPoint(-1, -1);
			m_rcLButtonDownRelateParent.SetRectEmpty();
			m_uLButtonDownHitTestRet = HTCLIENT;

			m_uCanDragResize = 0;
			m_nCXBorder = m_nCYBorder = 4;
			m_sizeScroll = CSize(16, 16);
			m_dwHoverTime = HOVER_DEFAULT;

			m_lpParam = 0;

			return 0;
		}

#ifdef _DEBUG
		int m_nAllocIndex;
#endif // _DEBUG

		BOOL m_bVisible;
		BOOL m_bEnabled;
		BOOL m_bNotify;		// Send message or not

		CString m_strClassName;

		CString m_strWindowName;
		HIUIIMAGE m_hBackground;
		SIZE m_sizeRepeat;

		DWORD m_dwStyle;
		UINT m_nID;
		CWnd *m_pParent;
		CRect m_rcOrig;
		CRect m_rcCurrent;
		BOOL m_bCreated;

		BOOL m_bEnterSizeMove;
		int m_nTitlebarHeight;	// the area for drag-move.
		HRGN m_hRgn;			// The operating system does not make a copy of the region, so do not make any further function calls with this region handle, and do not close this region handle.
		CWLWnd *m_pVirtualParent;
		POINT m_ptDrawOffset;	// Offset while drawing. The control in tree or list item template need offset while drawing, since the coordinate of the control is releate to item not to tree or list.

		BOOL m_bMouseOver;
		CPoint m_ptLButtonDown;	// record the point relate to windowless control while left mouse button down.
		CRect m_rcLButtonDownRelateParent;	// record the rect while left mouse button down.
		UINT m_uLButtonDownHitTestRet;	// record the hit test result while left mouse button down.

		UINT m_uCanDragResize;	// if can drag resize the control or not. This parameter can be a combination of the following values: HTLEFT, HTRIGHT etc.
		int m_nCXBorder;
		int m_nCYBorder;
		CSize m_sizeScroll;

		// for SetWLProp
		std::map<CString, HANDLE> m_mapProp;

		// for mouse hover
		DWORD m_dwHoverTime;

		LPVOID m_lpParam;
	};

}
