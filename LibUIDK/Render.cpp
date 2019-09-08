// 基础绘制引擎
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// Render function

void LibUIDK::Render::TransparentBlt2(HDC hdcDest,
	int nXOriginDest, int nYOriginDest, int nDestWidth, int nDestHeight,
	HDC hdcSrc,
	int nXOriginSrc, int nYOriginSrc, int nSrcWidth, int nSrcHeight,
	UINT crTransparent)
{
	if (nDestWidth <= 0 || nDestHeight <= 0 || nSrcWidth <= 0 || nSrcHeight <= 0)
	{
		return;
	}

	HBITMAP hOldImageBMP;
	HBITMAP hImageBMP = ::CreateCompatibleBitmap(hdcDest, nDestWidth, nDestHeight);
	ASSERT(hImageBMP != NULL);
	HBITMAP hOldMaskBMP;
	HBITMAP hMaskBMP = ::CreateBitmap(nDestWidth, nDestHeight, 1, 1, NULL);
	HDC     hImageDC = ::CreateCompatibleDC(hdcDest);
	HDC     hMaskDC = ::CreateCompatibleDC(hdcDest);
	hOldImageBMP = (HBITMAP)::SelectObject(hImageDC, hImageBMP);
	hOldMaskBMP = (HBITMAP)::SelectObject(hMaskDC, hMaskBMP);


	// copy source DC to temp DC
	if (nDestWidth == nSrcWidth && nDestHeight == nSrcHeight)
	{
		BitBlt(hImageDC, 0, 0, nDestWidth, nDestHeight, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
	}
	else
	{
		StretchBlt(hImageDC,
			0, 0, nDestWidth, nDestHeight,
			hdcSrc,
			nXOriginSrc, nYOriginSrc, nSrcWidth, nSrcHeight,
			SRCCOPY);
	}


	// Set transparent color
	SetBkColor(hImageDC, crTransparent);

	// Create a bitmap with white as transparent area, and black as other area
	BitBlt(hMaskDC, 0, 0, nDestWidth, nDestHeight, hImageDC, 0, 0, SRCCOPY);

	// Create a bitmap with black as transparent area, and keep up other area
	SetBkColor(hImageDC, RGB(0, 0, 0));
	SetTextColor(hImageDC, RGB(255, 255, 255));
	BitBlt(hImageDC, 0, 0, nDestWidth, nDestHeight, hMaskDC, 0, 0, SRCAND);


	// keep up screen within transparent area, and change other area to black
	SetBkColor(hdcDest, RGB(255, 255, 255));
	SetTextColor(hdcDest, RGB(0, 0, 0));
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nDestWidth, nDestHeight, hMaskDC, 0, 0, SRCAND);

	// "or" operate to final
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nDestWidth, nDestHeight, hImageDC, 0, 0, SRCPAINT);

	// free object
	SelectObject(hImageDC, hOldImageBMP);
	DeleteDC(hImageDC);
	SelectObject(hMaskDC, hOldMaskBMP);
	DeleteDC(hMaskDC);
	DeleteObject(hImageBMP);
	DeleteObject(hMaskBMP);
}

void LibUIDK::Render::TransparentBltHBITMAP(HDC hdcDest,
	int nXOriginDest, int nYOriginDest, int nDestWidth, int nDestHeight,
	HBITMAP hBmpSrc,
	int nXOriginSrc, int nYOriginSrc, int nSrcWidth, int nSrcHeight,
	UINT crTransparent)
{
	HDC hDC = ::GetDC(::GetDesktopWindow());
	HDC memDC = ::CreateCompatibleDC(hDC);
	HBITMAP pSrcBmpOld = (HBITMAP)::SelectObject(memDC, hBmpSrc);
	TransparentBlt(hdcDest,
		nXOriginDest, nYOriginDest, nDestWidth, nDestHeight,
		memDC,
		nXOriginSrc, nYOriginSrc, nSrcWidth, nSrcHeight,
		crTransparent);
	::SelectObject(memDC, pSrcBmpOld);
	::DeleteDC(memDC);
	::ReleaseDC(::GetDesktopWindow(), hDC);
}

int LibUIDK::Render::TileBlt(HDC hdcDest,
	int nXOriginDest, int nYOriginDest, int nDestWidth, int nDestHeight,
	HDC hdcSrc,
	int nXOriginSrc, int nYOriginSrc, int nSrcWidth, int nSrcHeight,
	BYTE btSourceConstantAlpha/* = 255*/)
{
	if (hdcDest == NULL || hdcSrc == NULL)
	{
		return -1;
	}
	if (nDestWidth <= 0 || nDestHeight <= 0)
	{
		return -2;
	}
	if (nXOriginSrc < 0 || nYOriginSrc < 0)
	{
		return -3;
	}
	if (nSrcWidth <= 0 || nSrcHeight <= 0)
	{
		return -4;
	}

	HDC hMidDC = ::CreateCompatibleDC(hdcDest);
	HBITMAP hMidBmp = ::CreateCompatibleBitmap(hdcDest, nSrcWidth, nSrcHeight);
	ASSERT(GetObjectType(hMidBmp) == OBJ_BITMAP);
	if (hMidBmp == NULL)
	{
		::DeleteDC(hMidDC);
		return -5;
	}
	HBITMAP hOldMidBmp = (HBITMAP)::SelectObject(hMidDC, hMidBmp);
	ASSERT(hOldMidBmp != hMidBmp);
	::BitBlt(hMidDC, 0, 0, nSrcWidth, nSrcHeight, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
	::SelectObject(hMidDC, hOldMidBmp);
	HBRUSH brush = ::CreatePatternBrush(hMidBmp);

	// Save hdcDest's old state
	POINT pt;
	::GetBrushOrgEx(hdcDest, &pt);

	// Set hdcDest's new state
	::SetBrushOrgEx(hdcDest, nXOriginDest, nYOriginDest, NULL);

	::FillRect(hdcDest,
		CRect(nXOriginDest, nYOriginDest, nXOriginDest + nDestWidth, nYOriginDest + nDestHeight),
		brush);

	// Restored hdcDest's original state
	::SetBrushOrgEx(hdcDest, pt.x, pt.y, NULL);

	::DeleteObject(brush);
	::DeleteObject(hMidBmp);
	::DeleteDC(hMidDC);

	return 0;
}

int LibUIDK::Render::AlphaTileBlt(HDC hdcDest,
	int nXOriginDest, int nYOriginDest, int nDestWidth, int nDestHeight,
	HDC hdcSrc,
	int nXOriginSrc, int nYOriginSrc, int nSrcWidth, int nSrcHeight,
	BYTE btSourceConstantAlpha/* = 255*/)
{
	if (nSrcWidth == 1 && nSrcHeight == 1)
	{
		return StretchAlphaBlend(hdcDest,
				nXOriginDest, nYOriginDest, nDestWidth, nDestHeight,
				hdcSrc,
				nXOriginSrc, nYOriginSrc, nSrcWidth, nSrcHeight,
				btSourceConstantAlpha);
	}
	else
	{
		if (hdcDest == NULL || hdcSrc == NULL)
		{
			return -1;
		}
		if (nDestWidth <= 0 || nDestHeight <= 0)
		{
			return -2;
		}
		if (nXOriginSrc < 0 || nYOriginSrc < 0)
		{
			return -3;
		}
		if (nSrcWidth <= 0 || nSrcHeight <= 0)
		{
			return -4;
		}

		int nXCount = nDestWidth / nSrcWidth;
		int nYCount = nDestHeight / nSrcHeight;

		if (nSrcWidth == 1)
		{
			for (int y = 0; y < nYCount; ++y)
			{
				StretchAlphaBlend(hdcDest,
					nXOriginDest, nYOriginDest + nSrcHeight * y, nDestWidth, nSrcHeight,
					hdcSrc,
					nXOriginSrc, nYOriginSrc, nSrcWidth, nSrcHeight,
					btSourceConstantAlpha);
			}

			// Alpha tile the last block at bottom.
			int nYLast = nDestHeight % nSrcHeight;
			if (nYLast > 0)
			{
				StretchAlphaBlend(hdcDest,
					nXOriginDest, nYOriginDest + nSrcHeight * nYCount, nDestWidth, nYLast,
					hdcSrc,
					nXOriginSrc, nYOriginSrc, nSrcWidth, nYLast,
					btSourceConstantAlpha);
			}
		}
		else if (nSrcHeight == 1)
		{
			for (int x = 0; x < nXCount; ++x)
			{
				StretchAlphaBlend(hdcDest,
					nXOriginDest + nSrcWidth * x, nYOriginDest, nSrcWidth, nDestHeight,
					hdcSrc,
					nXOriginSrc, nYOriginSrc, nSrcWidth, nSrcHeight,
					btSourceConstantAlpha);
			}

			// Alpha tile the last block at right.
			int nXLast = nDestWidth % nSrcWidth;
			if (nXLast > 0)
			{
				StretchAlphaBlend(hdcDest,
					nXOriginDest + nSrcWidth * nXCount, nYOriginDest, nXLast, nDestHeight,
					hdcSrc,
					nXOriginSrc, nYOriginSrc, nXLast, nSrcHeight,
					btSourceConstantAlpha);
			}
		}
		else
		{
			// 这段效率极低，需要优化
			// Alpha tile in vertical
			for (int y = 0; y < nYCount; ++y)
			{
				// Alpha tile in horizontal
				for (int x = 0; x < nXCount; ++x)
				{
					StretchAlphaBlend(hdcDest,
						nXOriginDest + nSrcWidth * x,
						nYOriginDest + nSrcHeight * y,
						nSrcWidth, nSrcHeight,
						hdcSrc,
						nXOriginSrc, nYOriginSrc, nSrcWidth, nSrcHeight, btSourceConstantAlpha);
				}

				// Alpha tile the last block in horizontal
				int nXLastDestWidth = nDestWidth % nSrcWidth;
				if (nXLastDestWidth > 0)
				{
					StretchAlphaBlend(hdcDest,
						nXOriginDest + nSrcWidth * nXCount,
						nYOriginDest + nSrcHeight * y,
						nXLastDestWidth,
						nSrcHeight,
						hdcSrc,
						nXOriginSrc,
						nYOriginSrc,
						nXLastDestWidth,
						nSrcHeight,
						btSourceConstantAlpha);
				}
			}

			// Alpha tile the last block in vertical
			int nYLastDestHeight = nDestHeight % nSrcHeight;
			if (nYLastDestHeight > 0)
			{
				// Alpha tile in horizontal
				for (int x = 0; x < nXCount; ++x)
				{
					StretchAlphaBlend(hdcDest,
						nXOriginDest + nSrcWidth * x,
						nYOriginDest + nSrcHeight * nYCount,
						nSrcWidth,
						nYLastDestHeight,
						hdcSrc,
						nXOriginSrc,
						nYOriginSrc,
						nSrcWidth,
						nYLastDestHeight,
						btSourceConstantAlpha);
				}

				// Alpha tile the last block in right-bottom corner.
				int nXLastDestWidth = nDestWidth % nSrcWidth;
				if (nXLastDestWidth > 0)
				{
					StretchAlphaBlend(hdcDest,
						nXOriginDest + nSrcWidth * nXCount,
						nYOriginDest + nSrcHeight * nYCount,
						nXLastDestWidth,
						nYLastDestHeight,
						hdcSrc,
						nXOriginSrc,
						nYOriginSrc,
						nXLastDestWidth,
						nYLastDestHeight,
						btSourceConstantAlpha);
				}
			}
		}
	}

	return 0;
}

int LibUIDK::Render::DrawControlText(
	HDC hDstDC,
	LPCRECT lprcControl, LPCRECT lprcTextMargin,
	LPCTSTR lpszText, TEXT_ALIGN_HOR eTextAlignHor, TEXT_ALIGN_VER eTextAlignVer,
	BOOL bMultiLine, BOOL bEndEllipsis, COLORREF crText, HFONT hFont)
{
	if (hDstDC == NULL || lprcControl == NULL || lpszText == NULL)
	{
		return -1;
	}

	HFONT hOldFont = NULL;
	if (hFont != NULL)
	{
		hOldFont = (HFONT)SelectObject(hDstDC, hFont);
	}

	::SetTextColor(hDstDC, crText);

	CRect rcText(lprcControl);
	if (lprcTextMargin != NULL)
	{
		rcText.DeflateRect(lprcTextMargin);
	}

	UINT uFormat = 0;
	if (bMultiLine)
	{
		uFormat = DT_WORDBREAK;
	}
	else
	{
		uFormat = DT_SINGLELINE;
	}

	if (bEndEllipsis)
	{
		uFormat |= DT_END_ELLIPSIS;
	}

	// adjust horizontal alignment
	if (eTextAlignHor == TAH_DEFAULT || eTextAlignHor == TAH_CENTER)
	{
		uFormat |= DT_CENTER;
	}
	else if (eTextAlignHor == TAH_LEFT)
	{
		uFormat |= DT_LEFT;
	}
	else if (eTextAlignHor == TAH_RIGHT)
	{
		uFormat |= DT_RIGHT;
	}
	// adjust vertical alignment
	if (eTextAlignVer == TAV_DEFAULT || eTextAlignVer == TAV_CENTER)
	{
		CRect rcTest(rcText);
		if (bMultiLine)
		{
			int nHeight = ::DrawText(hDstDC, lpszText, TSTRLEN(lpszText),
					rcTest, uFormat | DT_CALCRECT);
			int nAdjustHeight = (rcText.Height() - nHeight) / 2;
			rcText.DeflateRect(0, nAdjustHeight, 0, 0);
		}
		else
		{
			uFormat |= DT_VCENTER;
		}
	}
	else if (eTextAlignVer == TAV_TOP)
	{
		uFormat |= DT_TOP;
	}
	else if (eTextAlignVer == TAV_BOTTOM)
	{
		CRect rcTest(rcText);
		if (bMultiLine)
		{
			int nHeight = ::DrawText(hDstDC, lpszText, TSTRLEN(lpszText),
					rcTest, uFormat | DT_CALCRECT);
			rcText.top += (rcText.Height() - nHeight);
		}
		else
		{
			uFormat |= DT_BOTTOM;
		}
	}

	::SetBkMode(hDstDC, TRANSPARENT);

	::DrawText(hDstDC, lpszText, TSTRLEN(lpszText), rcText, uFormat);

	if (hOldFont != NULL)
	{
		::SelectObject(hDstDC, hOldFont);
	}

	return 0;
}

int LibUIDK::Render::DrawFormatText(
	HDC hdcDest,
	LPCTSTR lpszText,
	int nTextLength,
	LPCRECT lprcDest,
	LPCRECT lprcPadding4Text,
	TEXT_ALIGN_HOR eTextHorAlign,
	TEXT_ALIGN_VER eTextVerAlign,
	UINT uTextFormat,
	HFONT hTextFont,
	COLORREF crText,
	CONTROL_TYPE eControlType)
{
	if (hdcDest == NULL || lprcDest == NULL)
	{
		return -1;
	}

	if (lpszText == NULL || nTextLength <= 0)
	{
		return 1;
	}

	SetBkMode(hdcDest, TRANSPARENT);
	HFONT hFontOld = NULL;

	// If hTextFont is NULL, use default font
	CString strDefaultFont;
	if (hTextFont == NULL)
	{
		// Default font
		strDefaultFont = CUIMgr::GetDefaultFont();
		hTextFont = CreateIUIFont(strDefaultFont);
	}

	hFontOld = (HFONT)SelectObject(hdcDest, hTextFont);
	SetTextColor(hdcDest, crText);

	CRect rcDraw = *lprcDest;
	if (lprcPadding4Text != NULL)
	{
		rcDraw.DeflateRect(lprcPadding4Text);
	}

	switch (eTextHorAlign)
	{
	case TAH_DEFAULT:
		if (eControlType == CT_PUSHBUTTON
			|| eControlType == CT_CHECK
			|| eControlType == CT_RADIO
			|| eControlType == CT_WL_BUTTON
			|| eControlType == CT_WL_CHECK
			|| eControlType == CT_WL_RADIO)
		{
			uTextFormat |= DT_CENTER;
		}
		else if (eControlType == CT_STATIC
			|| eControlType == CT_WL_TEXT)
		{
			uTextFormat |= DT_LEFT;
		}
		else
		{
			ASSERT(FALSE);
		}
		break;
	case TAH_LEFT:
	default: // left
		uTextFormat |= DT_LEFT;
		break;
	case TAH_RIGHT:
		uTextFormat |= DT_RIGHT;
		break;
	case TAH_CENTER:
		uTextFormat |= DT_CENTER;
		break;
	}
	switch (eTextVerAlign)
	{
	case TAV_DEFAULT:
		if (eControlType == CT_PUSHBUTTON
			|| eControlType == CT_CHECK
			|| eControlType == CT_RADIO
			|| eControlType == CT_WL_BUTTON
			|| eControlType == CT_WL_CHECK
			|| eControlType == CT_WL_RADIO)
		{
			uTextFormat |= DT_VCENTER;
		}
		else if (eControlType == CT_STATIC
			|| eControlType == CT_WL_TEXT)
		{
			uTextFormat |= DT_TOP;
		}
		else
		{
			ASSERT(FALSE);
		}
	case TAV_TOP:
	default: // top
		uTextFormat |= DT_TOP;
		break;
	case TAV_BOTTOM:
	{
		CRect rcTest(rcDraw);
		if (IsIncludeFlag(uTextFormat, DT_WORDBREAK))
		{
			int nHeight = DrawText(hdcDest, lpszText, nTextLength, rcTest,
					uTextFormat | DT_CALCRECT);

			rcDraw.top += (rcDraw.Height() - nHeight);
		}
		else
		{
			uTextFormat |= DT_BOTTOM;
		}
		break;
	}
	case TAV_CENTER:
	{
		CRect rcTest(*lprcDest);
		if (IsIncludeFlag(uTextFormat, DT_WORDBREAK))
		{
			int nHeight = DrawText(hdcDest, lpszText, nTextLength, rcTest,
					uTextFormat | DT_CALCRECT);

			int nAdjustHeight = (lprcDest->bottom - lprcDest->top - nHeight) / 2;
			rcDraw.DeflateRect(0, nAdjustHeight, 0, 0);
		}
		else
		{
			uTextFormat |= DT_VCENTER;
		}
		break;
	}
	}

	// #if(_MSC_VER > 1200) // VC6.0
	// 	if (m_bShadowText)
	// 	{
	// #ifdef _UNICODE
	// 		WCHAR *pwText = _bstr_t(strText);
	// 		DrawShadowText(pMemDC->GetSafeHdc(), pwText, strText.GetLength(), rcDraw, uFormat,
	//          bEnabled ? m_crN : m_crD, m_crTextShadow, m_ptTextShadowOffset.x,
	//          m_ptTextShadowOffset.y);
	// #else
	// 		pMemDC->DrawText(strText, rcDraw, uFormat);
	// #endif
	// 	}
	// 	else
	// #endif
	{
		DrawText(hdcDest, lpszText, nTextLength, rcDraw, uTextFormat);
	}

	SelectObject(hdcDest, hFontOld);

	if (!strDefaultFont.IsEmpty())
	{
		ReleaseIUIFont(strDefaultFont);
	}

	return 0;
}

int LibUIDK::Render::DrawTextGp(
	Graphics *pGraphicsDst,
	LPCTSTR lpszString,
	int nLength,
	HFONT hFont,
	COLORREF crText,
	LPRECT lprc,
	UINT format)
{
	Font *pFont = NULL;

#ifdef _DEBUG
#ifdef new
#undef new
	if (hFont != NULL)
	{
		HDC hDC = pGraphicsDst->GetHDC();
		pFont = new Font(hDC, hFont);
		pGraphicsDst->ReleaseHDC(hDC);
	}

#define new DEBUG_NEW
#endif // new
#endif // _DEBUG

	RectF layoutRect(REAL(lprc->left), REAL(lprc->top),
		REAL(lprc->right - lprc->left), REAL(lprc->bottom - lprc->top));
	StringFormat eAlign;
	eAlign.SetAlignment(StringAlignmentNear);
	eAlign.SetLineAlignment(StringAlignmentNear);

	if ((format & DT_SINGLELINE) == DT_SINGLELINE)
	{
		// Horizontal
		if ((format & DT_CENTER) == DT_CENTER)
		{
			eAlign.SetAlignment(StringAlignmentCenter);
		}
		else if ((format & DT_RIGHT) == DT_RIGHT)
		{
			eAlign.SetAlignment(StringAlignmentFar);
		}

		// Vertical
		if ((format & DT_VCENTER) == DT_VCENTER)
		{
			eAlign.SetLineAlignment(StringAlignmentCenter);
		}
		else if ((format & DT_BOTTOM) == DT_BOTTOM)
		{
			eAlign.SetLineAlignment(StringAlignmentFar);
		}
	}
	SolidBrush brush(Color(255, GetRValue(crText), GetGValue(crText), GetBValue(crText)));

	// Draw string.
	//CWindowDC dc(NULL);
	//Graphics g(dc.GetSafeHdc());
	pGraphicsDst->DrawString(
		(LPWSTR)_bstr_t(lpszString),
		nLength,
		pFont,
		layoutRect,
		&eAlign,
		&brush);

	SafeDelete(pFont);

	return 0;
}

int LibUIDK::Render::DrawLine(HDC hDstDC, COLORREF crLine, int nLineWidht, POINT ptBegin, POINT ptEnd)
{
	return DrawLine(hDstDC,
			crLine, nLineWidht, ptBegin.x, ptBegin.y, ptEnd.x, ptEnd.y);
}

int LibUIDK::Render::DrawLine(HDC hDstDC, COLORREF crLine,
	int nLineWidht, int nX1, int nY1, int nX2, int nY2)
{
	LOGPEN lpLine;
	lpLine.lopnColor = crLine;
	lpLine.lopnStyle = PS_SOLID;
	lpLine.lopnWidth = CPoint(nLineWidht, nLineWidht);
	HPEN hPenLine = ::CreatePenIndirect(&lpLine);
	HPEN hOldPen = (HPEN)::SelectObject(hDstDC, hPenLine);

	CPoint point;
	VERIFY(::MoveToEx(hDstDC, nX1, nY1, &point));
	::LineTo(hDstDC, nX2, nY2);

	::SelectObject(hDstDC, hOldPen);
	DeleteObject(hPenLine);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// CIUIImage

int LibUIDK::Render::StretchAlphaBlend(
	HDC hdcDest,
	int nXOriginDest, int nYOriginDest, int nDestWidth, int nDestHeight,
	HDC hdcSrc,
	int nXOriginSrc, int nYOriginSrc, int nSrcWidth, int nSrcHeight,
	BYTE btSourceConstantAlpha/* = 255*/)
{
	BLENDFUNCTION blend;
	blend.SourceConstantAlpha = btSourceConstantAlpha;
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.AlphaFormat = AC_SRC_ALPHA;

	// If the nDestWidth or nDestHeight not equal the image size, may return FALSE.
	BOOL bResult = ::AlphaBlend(hdcDest, nXOriginDest, nYOriginDest, nDestWidth, nDestHeight,
			hdcSrc, nXOriginSrc, nYOriginSrc, nSrcWidth, nSrcHeight, blend);
	if (!bResult)
	{
		return -1;
	}

	return 0;
}

// the resize point in bitmap, // best
int LibUIDK::Render::NineGridBltG(
	HDC hdcDest,
	int nDestX, int nDestY, int nDestWidth, int nDestHeight,
	HBITMAP hbmpSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY)
{
	if (hbmpSrc == NULL)
	{
		return -1;
	}

	HDC memDC = ::CreateCompatibleDC(hdcDest);
	HBITMAP pSrcBmpOld = (HBITMAP)::SelectObject(memDC, hbmpSrc);

	BITMAP bmp;
	int nCount = ::GetObject(hbmpSrc, sizeof(bmp), &bmp);
	ASSERT(nCount != 0);

	int nDestCol1Width = nDestWidth - bmp.bmWidth + 1;
	int nDestLine1Height = nDestHeight - bmp.bmHeight + 1;

	int nDestCol2Width = bmp.bmWidth - pt9GridResize.x - 1;
	int nDestCol2Left = nDestWidth - nDestCol2Width;
	if (nDestCol2Left < pt9GridResize.x)
	{
		nDestCol2Left = pt9GridResize.x;
	}

	int nDestLine2Height = bmp.bmHeight - pt9GridResize.y - 1;
	int nDestLine2Top = nDestHeight - nDestLine2Height;
	if (nDestLine2Top < pt9GridResize.y)
	{
		nDestLine2Top = pt9GridResize.y;
	}

	// if the dest size less than source size, not stretch.
	if (nDestWidth <= bmp.bmWidth && nDestHeight <= bmp.bmHeight)
	{
		::BitBlt(hdcDest, nDestX, nDestY, nDestWidth, nDestHeight, memDC, 0, 0, SRCCOPY);
	}
	else if (nDestWidth <= bmp.bmWidth && nDestHeight > bmp.bmHeight)
	{
		// top
		::BitBlt(hdcDest, nDestX, nDestY, nDestWidth, pt9GridResize.y, memDC, 0, 0, SRCCOPY);
		// center, not stretch at horizontal(ignore pt9GridResize.x and l9GridRepeatX), tile at vertical.
		if (pt9GridResize.y < bmp.bmHeight)	// in picture
		{
			TileBlt(hdcDest, nDestX, nDestY + pt9GridResize.y, nDestWidth, nDestLine1Height,
				memDC, 0, pt9GridResize.y, nDestWidth, l9GridRepeatY, 255);
		}

		// bottom
		::BitBlt(hdcDest, nDestX, nDestY + nDestLine2Top, nDestWidth, nDestLine2Height,
			memDC, 0, pt9GridResize.y + 1, SRCCOPY);
	}
	else if (nDestWidth > bmp.bmWidth && nDestHeight <= bmp.bmHeight)
	{
		// left
		::BitBlt(hdcDest, nDestX, nDestY, pt9GridResize.x, nDestHeight, memDC, 0, 0, SRCCOPY);
		// center, not stretch at vertical, tile at horizontal.
		if (pt9GridResize.x < bmp.bmWidth)	// in picture
		{
			TileBlt(hdcDest, nDestX + pt9GridResize.x, nDestY, nDestCol1Width, nDestHeight,
				memDC, pt9GridResize.x, 0, l9GridRepeatX, nDestHeight, 255);
		}
		// right
		::BitBlt(hdcDest, nDestX + nDestCol2Left, nDestY, nDestCol2Width, nDestHeight,
			memDC, pt9GridResize.x + 1, 0, SRCCOPY);
	}
	else
	{
		// area 0
		if (pt9GridResize.x > 0 && pt9GridResize.y > 0)
		{
			::BitBlt(hdcDest, nDestX, nDestY, pt9GridResize.x, pt9GridResize.y, memDC, 0, 0, SRCCOPY);
		}
		// area 1
		if (pt9GridResize.y > 0)
		{
			TileBlt(hdcDest, nDestX + pt9GridResize.x, nDestY, nDestCol1Width, pt9GridResize.y,
				memDC, pt9GridResize.x, 0, l9GridRepeatX, pt9GridResize.y, 255);
		}
		// area 2
		if (pt9GridResize.x < bmp.bmWidth - 1 && pt9GridResize.y > 0)
		{
			::BitBlt(hdcDest, nDestX + nDestCol2Left, nDestY, nDestCol2Width, pt9GridResize.y,
				memDC, pt9GridResize.x + 1, 0, SRCCOPY);
		}

		// area 3
		if (pt9GridResize.x > 0)
		{
			TileBlt(hdcDest, nDestX, nDestY + pt9GridResize.y, pt9GridResize.x, nDestLine1Height,
				memDC, 0, pt9GridResize.y, pt9GridResize.x, l9GridRepeatY, 255);
		}
		// area 4
		ASSERT(nDestCol1Width > 0 && nDestLine1Height > 0);
		TileBlt(hdcDest,
			nDestX + pt9GridResize.x, nDestY + pt9GridResize.y, nDestCol1Width, nDestLine1Height,
			memDC, pt9GridResize.x, pt9GridResize.y, l9GridRepeatX, l9GridRepeatY, 255);
		// area 5
		if (nDestCol2Width > 0)
		{
			TileBlt(hdcDest,
				nDestX + nDestCol2Left, nDestY + pt9GridResize.y, nDestCol2Width, nDestLine1Height,
				memDC, pt9GridResize.x + 1, pt9GridResize.y, nDestCol2Width, l9GridRepeatY, 255);
		}

		// area 6
		if (pt9GridResize.x > 0 && nDestLine2Height > 0)
		{
			::BitBlt(hdcDest, nDestX, nDestY + nDestLine2Top, pt9GridResize.x, nDestLine2Height,
				memDC, 0, pt9GridResize.y + 1, SRCCOPY);
		}
		// area 7
		if (nDestLine2Height > 0)
		{
			TileBlt(hdcDest,
				nDestX + pt9GridResize.x, nDestY + nDestLine2Top, nDestCol1Width, nDestLine2Height,
				memDC, pt9GridResize.x, pt9GridResize.y + 1, l9GridRepeatX, nDestLine2Height, 255);
		}
		// area 8
		if (nDestCol2Width > 0 && nDestLine2Height > 0)
		{
			::BitBlt(hdcDest,
				nDestX + nDestCol2Left, nDestY + nDestLine2Top, nDestCol2Width, nDestLine2Height,
				memDC, pt9GridResize.x + 1, pt9GridResize.y + 1, SRCCOPY);
		}
	}

	::SelectObject(memDC, pSrcBmpOld);
	::DeleteDC(memDC);

	return 0;
}

int LibUIDK::Render::NineGridBltG(HDC hdcDest, LPCRECT lprcDest,
	HBITMAP hbmpSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY)
{
	if (hdcDest == NULL || lprcDest == NULL)
	{
		return -1;
	}

	return NineGridBltG(hdcDest,
			lprcDest->left, lprcDest->top, RECTWIDTH(*lprcDest), RECTHEIGHT(*lprcDest),
			hbmpSrc, pt9GridResize, l9GridRepeatX, l9GridRepeatY);
}

int LibUIDK::Render::PartNineGridBltG(HDC hdcDest,
	int nDestX, int nDestY, int nDestWidth, int nDestHeight,
	HBITMAP hbmpSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	int nPart, int nPartIndex)
{
	if (hbmpSrc == NULL)
	{
		return -1;
	}
	if (nPart <= 0)
	{
		return -2;
	}
	if (nPartIndex < 0)
	{
		return -3;
	}
	if (nPartIndex >= nPart)
	{
		return -4;
	}

	SIZE sizeSrc;
	CIUIImage::GetBitmapSizeG(hbmpSrc, &sizeSrc);

	CRect rcSrc;
	rcSrc.left = sizeSrc.cx * nPartIndex / nPart;
	rcSrc.top = 0;
	rcSrc.right = rcSrc.left + sizeSrc.cx / nPart;
	rcSrc.bottom = rcSrc.top + sizeSrc.cy;

	return PartNineGridBltG(hdcDest,
			CRect(nDestX, nDestY, nDestX + nDestWidth, nDestY + nDestHeight),
			hbmpSrc, pt9GridResize, l9GridRepeatX, l9GridRepeatY, rcSrc);
}

int LibUIDK::Render::PartNineGridBltG(HDC hdcDest, LPCRECT lprcDest,
	HBITMAP hbmpSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	int nPart, int nPartIndex)
{
	if (lprcDest == NULL)
	{
		return -1;
	}

	return PartNineGridBltG(hdcDest,
			lprcDest->left, lprcDest->top, RECTWIDTH(*lprcDest), RECTHEIGHT(*lprcDest),
			hbmpSrc, pt9GridResize, l9GridRepeatX, l9GridRepeatY, nPart, nPartIndex);
}

int LibUIDK::Render::PartNineGridBltG(HDC hdcDest, LPCRECT lprcDest,
	HBITMAP hbmpSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	LPCRECT lprcSrc)
{
	if (hbmpSrc == NULL || lprcDest == NULL)
	{
		return -1;
	}

	CRect rcDest(lprcDest);

	CRect rcSrc(lprcSrc);
	if (rcSrc.IsRectEmpty())
	{
		SIZE size;
		CIUIImage::GetBitmapSizeG(hbmpSrc, &size);
		rcSrc.left = 0;
		rcSrc.top = 0;
		rcSrc.right = size.cx;
		rcSrc.bottom = size.cy;
	}

	HDC memDC = ::CreateCompatibleDC(hdcDest);
	HBITMAP pSrcBmpOld = (HBITMAP)::SelectObject(memDC, hbmpSrc);

	BITMAP bmpSrc;
	::GetObject(hbmpSrc, sizeof(bmpSrc), &bmpSrc);

	int nDestCol1Width = rcDest.Width() - rcSrc.Width() + 1;
	int nDestLine1Height = rcDest.Height() - rcSrc.Height() + 1;

	int nDestCol2Width = rcSrc.Width() - pt9GridResize.x - 1;
	int nDestCol2Left = rcDest.Width() - nDestCol2Width;
	if (nDestCol2Left < pt9GridResize.x)
	{
		nDestCol2Left = pt9GridResize.x;
	}

	int nDestLine2Height = rcSrc.Height() - pt9GridResize.y - 1;
	int nDestLine2Top = rcDest.Height() - nDestLine2Height;
	if (nDestLine2Top < pt9GridResize.y)
	{
		nDestLine2Top = pt9GridResize.y;
	}

	// if the dest size less than source size, not stretch.
	if (rcDest.Width() <= rcSrc.Width() && rcDest.Height() <= rcSrc.Height())
	{
		::BitBlt(hdcDest, rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(),
			memDC, rcSrc.left, rcSrc.top, SRCCOPY);
	}
	else if (rcDest.Width() <= rcSrc.Width() && rcDest.Height() > rcSrc.Height())
	{
		// top
		::BitBlt(hdcDest, rcDest.left, rcDest.top, rcDest.Width(), pt9GridResize.y,
			memDC, rcSrc.left, rcSrc.top, SRCCOPY);

		// center, not stretch at horizontal(ignore pt9GridResize.x and l9GridRepeatX), tile at vertical.
		if (pt9GridResize.y < rcSrc.Height())	// in picture
		{
			TileBlt(hdcDest,
				rcDest.left, rcDest.top + pt9GridResize.y, rcDest.Width(), nDestLine1Height,
				memDC, rcSrc.left, rcSrc.top + pt9GridResize.y, rcDest.Width(), l9GridRepeatY, 255);
		}

		// bottom
		::BitBlt(hdcDest, rcDest.left, rcDest.top + nDestLine2Top, rcDest.Width(), nDestLine2Height,
			memDC, rcSrc.left, rcSrc.top + pt9GridResize.y + 1, SRCCOPY);
	}
	else if (rcDest.Width() > rcSrc.Width() && rcDest.Height() <= rcSrc.Height())
	{
		// left
		::BitBlt(hdcDest, rcDest.left, rcDest.top, pt9GridResize.x, rcDest.Height(),
			memDC, rcSrc.left, rcSrc.top, SRCCOPY);

		// center, not stretch at vertical, tile at horizontal.
		if (pt9GridResize.x < rcSrc.Width())	// in picture
		{
			TileBlt(hdcDest, rcDest.left + pt9GridResize.x, rcDest.top, nDestCol1Width, rcDest.Height(),
				memDC, rcSrc.left + pt9GridResize.x, rcSrc.top, l9GridRepeatX, rcDest.Height(), 255);
		}

		// right
		::BitBlt(hdcDest, rcDest.left + nDestCol2Left, rcDest.top, nDestCol2Width, rcDest.Height(),
			memDC, rcSrc.left + pt9GridResize.x + 1, rcSrc.top, SRCCOPY);
	}
	else
	{
		// area 0
		if (pt9GridResize.x > 0 && pt9GridResize.y > 0)
		{
			::BitBlt(hdcDest, rcDest.left, rcDest.top, pt9GridResize.x, pt9GridResize.y,
				memDC, rcSrc.left, rcSrc.top, SRCCOPY);
		}

		// area 1
		if (pt9GridResize.y > 0)
		{
			TileBlt(hdcDest, rcDest.left + pt9GridResize.x, rcDest.top, nDestCol1Width, pt9GridResize.y,
				memDC, rcSrc.left + pt9GridResize.x, rcSrc.top, l9GridRepeatX, pt9GridResize.y, 255);
		}

		// area 2
		if (pt9GridResize.x < rcSrc.Width() - 1 && pt9GridResize.y > 0)
		{
			::BitBlt(hdcDest,
				rcDest.left + nDestCol2Left, rcDest.top, nDestCol2Width, pt9GridResize.y,
				memDC, rcSrc.left + pt9GridResize.x + 1, rcSrc.top, SRCCOPY);
		}

		// area 3
		if (pt9GridResize.x > 0)
		{
			TileBlt(hdcDest,
				rcDest.left, rcDest.top + pt9GridResize.y, pt9GridResize.x, nDestLine1Height,
				memDC, rcSrc.left, rcSrc.top + pt9GridResize.y, pt9GridResize.x, l9GridRepeatY, 255);
		}

		// area 4
		ASSERT(nDestCol1Width > 0 && nDestLine1Height > 0);
		TileBlt(hdcDest,
			rcDest.left + pt9GridResize.x,
			rcDest.top + pt9GridResize.y, nDestCol1Width, nDestLine1Height,
			memDC, rcSrc.left + pt9GridResize.x, rcSrc.top + pt9GridResize.y, l9GridRepeatX, l9GridRepeatY, 255);

		// area 5
		if (nDestCol2Width > 0)
		{
			TileBlt(hdcDest,
				rcDest.left + nDestCol2Left, rcDest.top + pt9GridResize.y,
				nDestCol2Width, nDestLine1Height,
				memDC,
				rcSrc.left + pt9GridResize.x + 1, rcSrc.top + pt9GridResize.y,
				nDestCol2Width, l9GridRepeatY, 255);
		}

		// area 6
		if (pt9GridResize.x > 0 && nDestLine2Height > 0)
		{
			::BitBlt(hdcDest,
				rcDest.left, rcDest.top + nDestLine2Top, pt9GridResize.x, nDestLine2Height,
				memDC,
				rcSrc.left, rcSrc.top + pt9GridResize.y + 1, SRCCOPY);
		}

		// area 7
		if (nDestLine2Height > 0)
		{
			TileBlt(hdcDest,
				rcDest.left + pt9GridResize.x, rcDest.top + nDestLine2Top,
				nDestCol1Width, nDestLine2Height,
				memDC,
				rcSrc.left + pt9GridResize.x, rcSrc.top + pt9GridResize.y + 1,
				l9GridRepeatX, nDestLine2Height, 255);
		}

		// area 8
		if (nDestCol2Width > 0 && nDestLine2Height > 0)
		{
			::BitBlt(hdcDest,
				rcDest.left + nDestCol2Left, rcDest.top + nDestLine2Top,
				nDestCol2Width, nDestLine2Height,
				memDC,
				rcSrc.left + pt9GridResize.x + 1, rcSrc.top + pt9GridResize.y + 1, SRCCOPY);
		}
	}

	::SelectObject(memDC, pSrcBmpOld);
	::DeleteDC(memDC);

	return 0;
}

int LibUIDK::Render::StretchBltG(HDC hdcDest, LPCRECT lprcDest, HBITMAP hbmpSrc)
{
	if (hbmpSrc == NULL)
	{
		return -1;
	}

	HDC memDC = ::CreateCompatibleDC(hdcDest);
	HBITMAP pSrcBmpOld = (HBITMAP)::SelectObject(memDC, hbmpSrc);

	CSize sizeSrc;
	CIUIImage::GetBitmapSizeG(hbmpSrc, &sizeSrc);
	::StretchBlt(hdcDest, lprcDest->left, lprcDest->top, RECTWIDTH(*lprcDest), RECTHEIGHT(*lprcDest),
		memDC, 0, 0, sizeSrc.cx, sizeSrc.cy, SRCCOPY);

	::SelectObject(memDC, pSrcBmpOld);
	::DeleteDC(memDC);

	return 0;
}

int LibUIDK::Render::BitBltG(HDC hdcDest, int nXDest, int nYDest, int nDestWidth, int nDestHeight,
	HBITMAP hSrcBmp, int nXSrc, int nYSrc, DWORD dwRop)
{
	if (hSrcBmp == NULL)
	{
		return -1;
	}

	HDC memDC = ::CreateCompatibleDC(hdcDest);

	// store the Font object before call DefWindowProc
	HFONT hFontOld = (HFONT)::GetCurrentObject(memDC, OBJ_FONT);

	HBITMAP pSrcBmpOld = (HBITMAP)::SelectObject(memDC, hSrcBmp);

	::BitBlt(hdcDest, nXDest, nYDest, nDestWidth, nDestHeight, memDC, nXSrc, nYSrc, dwRop);

	::SelectObject(memDC, pSrcBmpOld);

	if (hFontOld != NULL)
	{
		::SelectObject(memDC, hFontOld);
	}

	::DeleteDC(memDC);

	return 0;
}

int LibUIDK::Render::BitBltG(HDC hdcDest, LPCRECT lprcDest,
	HBITMAP hSrcBmp, int nXSrc, int nYSrc, DWORD dwRop)
{
	return BitBltG(hdcDest, lprcDest->left, lprcDest->top, RECTWIDTH(*lprcDest), RECTHEIGHT(*lprcDest),
			hSrcBmp, nXSrc, nYSrc, dwRop);
}

int LibUIDK::Render::CenterBitBltG(HDC hDC, LPCRECT lprcDest, HBITMAP hBmp, DWORD dwRop)
{
	if (lprcDest == NULL)
	{
		return -1;
	}

	CSize size;
	int nRet = CIUIImage::GetBitmapSizeG(hBmp, &size);
	if (nRet != 0)
	{
		return nRet;
	}

	CRect rcNewDest;
	rcNewDest.left = lprcDest->left + (RECTWIDTH(*lprcDest) - size.cx) / 2;
	rcNewDest.right = rcNewDest.left + size.cx;
	rcNewDest.top = lprcDest->top + (RECTHEIGHT(*lprcDest) - size.cy) / 2;
	rcNewDest.bottom = rcNewDest.top + size.cy;

	return BitBltG(hDC, rcNewDest.left, rcNewDest.top, rcNewDest.Width(), rcNewDest.Height(),
			hBmp, 0, 0, dwRop);
}

int LibUIDK::Render::DrawForegroundG(HDC hDstDC, LPCRECT lprcClient,
	LPCRECT lprcForegroundMargin, HBITMAP hbmpFg,
	FOREGROUND_ALIGN_HOR eForegroundAlignHor, FOREGROUND_ALIGN_VER eForegroundAlignVer,
	COLORREF crMask)
{
	if (hDstDC == NULL || hbmpFg == NULL || lprcClient == NULL)
	{
		return -1;
	}

	CSize sizeImg;
	CIUIImage::GetBitmapSizeG(hbmpFg, &sizeImg);

	CRect rcControl(lprcClient);
	if (lprcForegroundMargin != NULL)
	{
		rcControl.DeflateRect(lprcForegroundMargin);
	}

	int nX = rcControl.left;
	int nY = rcControl.top;

	if (eForegroundAlignHor == FAH_CENTER) // center
	{
		nX += (rcControl.Width() - sizeImg.cx) / 2;
	}
	else if (eForegroundAlignHor == FAH_LEFT || eForegroundAlignHor == FAH_STRETCH) // left or Stretch
	{
		// Keep.
	}
	else if (eForegroundAlignHor == FAH_RIGHT) // right
	{
		nX += (rcControl.Width() - sizeImg.cx);
	}
	else
	{
		ASSERT(FALSE);
	}

	if (eForegroundAlignVer == FAV_CENTER) // center
	{
		nY += (rcControl.Height() - sizeImg.cy) / 2;
	}
	else if (eForegroundAlignVer == FAV_TOP || eForegroundAlignVer == FAV_STRETCH) // top or Stretch
	{
		// Keep.
	}
	else if (eForegroundAlignVer == FAV_BOTTOM) // bottom
	{
		nY += (rcControl.Height() - sizeImg.cy);
	}
	else
	{
		ASSERT(FALSE);
	}

	HDC hMemDC = ::CreateCompatibleDC(hDstDC);
	HBITMAP hbmpOld = (HBITMAP)::SelectObject(hMemDC, hbmpFg);

	TransparentBlt2(hDstDC, nX, nY, sizeImg.cx, sizeImg.cy,
		hMemDC, 0, 0, sizeImg.cx, sizeImg.cy, crMask);

	::SelectObject(hMemDC, hbmpOld);
	::DeleteDC(hMemDC);

	return 0;
}

int LibUIDK::Render::DrawImageG(HDC hDstDC, LPCRECT lprcImage, HBITMAP hImage,
	IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY)
{
	if (hDstDC == NULL || lprcImage == NULL)
	{
		return -1;
	}
	if (hImage == NULL)
	{
		return -2;
	}

	if (eIrm == IRM_9GRID)
	{
		NineGridBltG(hDstDC, lprcImage,	hImage, pt9GridResize, l9GridRepeatX, l9GridRepeatY);
	}
	else if (eIrm == IRM_STRETCH
		|| eIrm == IRM_STRETCH_HIGH_QUALITY)
	{
		StretchBltG(hDstDC, lprcImage, hImage);
	}

	return 0;
}

// 早期的IUINineGridBlt函数，声明如下：
// int LibUIDK::IUINineGridBlt(HDC hdcDest, int nXDest, int nYDest, int nDestWidth, int nDestHeight,
//	   HIUIIMAGE himgSrc, int nResizeX, int nResizeY, COLORREF crColorKey = -1, BYTE btSourceConstantAlpha = 255)
// himgSrc参数的下一个参数是int nResizeX, 表示himgSrc的水平缩放点，
// 现在，缩放点已移植到了HIUIIMAGE内部，故新的IUINineGridBlt把int nResizeX参数删除。声明如下
// int LibUIDK::IUINineGridBlt(HDC hdcDest, int nXDest, int nYDest, int nDestWidth, int nDestHeight,
//	   HIUIIMAGE himgSrc, COLORREF crColorKey = -1, BYTE btSourceConstantAlpha = 255)
// 但是删除后，会导致一个问题。 假如原来的代码如下：
// IUINineGridBlt(hDC, 0, 0, 5, 5, hImg, 3, 3);
// 上面的3, 3,表示缩放点。 但在修改声明后，上面的代码仍然可以编译通过，编译时并不会
// 导致编译错误或警告。 但这里的第一个3，表示的是crColorKey，第二个3表示btSourceConstantAlpha。
// 意义已发生了变化，这将导致潜在的bug。 所以，解决方案有两个：
// 1. 在crColorKey之前，增加一个double参数，使上面的代码
// 在编译期，就被编译器识别出来。 如果当时IUINineGridBlt没有默认参数，删除缩放点参数后，
// 也会在编译期识别。 所以，以后定义函数，禁止使用默认参数
// 2. 修改IUINineGridBlt的参数。把原来的默认参数都删除掉，重新加一个带原来默认参数的IUINineGridBltEx。
// LibUIDK采用第二种方法。
int LibUIDK::Render::IUINineGridBlt(HDC hdcDest, int nXDest, int nYDest, int nDestWidth, int nDestHeight,
	HIUIIMAGE himgSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY)
{
	return IUINineGridBltEx(hdcDest, nXDest, nYDest, nDestWidth, nDestHeight,
			himgSrc, pt9GridResize, l9GridRepeatX, l9GridRepeatY, -1, 255);
}

int LibUIDK::Render::IUINineGridBlt(HDC hdcDest, LPCRECT lprcDest,
	HIUIIMAGE hbmpSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY)
{
	if (lprcDest == NULL)
	{
		return -1;
	}

	return IUINineGridBltEx(hdcDest,
			lprcDest->left, lprcDest->top, RECTWIDTH(*lprcDest), RECTHEIGHT(*lprcDest),
			hbmpSrc, pt9GridResize, l9GridRepeatX, l9GridRepeatY, -1, 255);
}

int LibUIDK::Render::IUINineGridBltEx(HDC hdcDest,
	int nXDest, int nYDest, int nDestWidth, int nDestHeight,
	HIUIIMAGE himgSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	COLORREF crColorKey, BYTE btSourceConstantAlpha)
{
	if (himgSrc == NULL)
	{
		return -1;
	}

	himgSrc->SafeLoadSavedImage();

	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -2;
	}

	if (himgSrc->IsHasAlphaChannel())
	{
		HDC memDC = ::CreateCompatibleDC(hdcDest);
		HBITMAP pSrcBmpOld = (HBITMAP)::SelectObject(memDC, himgSrc->GetSafeHBITMAP());

		int nDestCol1Width = nDestWidth - himgSrc->GetWidth() + 1;
		int nDestLine1Height = nDestHeight - himgSrc->GetHeight() + 1;

		int nDestCol2Width = himgSrc->GetWidth() - pt9GridResize.x - 1;
		int nDestCol2Left = nDestWidth - nDestCol2Width;
		if (nDestCol2Left < pt9GridResize.x)
		{
			nDestCol2Left = pt9GridResize.x;
		}

		int nDestLine2Height = himgSrc->GetHeight() - pt9GridResize.y - 1;
		int nDestLine2Top = nDestHeight - nDestLine2Height;
		if (nDestLine2Top < pt9GridResize.y)
		{
			nDestLine2Top = pt9GridResize.y;
		}

		int nRet = -1;
		while (true)
		{
			// if the dest size less than source size, not stretch.
			if (nDestWidth <= himgSrc->GetWidth() && nDestHeight <= himgSrc->GetHeight())
			{
				nRet = StretchAlphaBlend(hdcDest, nXDest, nYDest, nDestWidth, nDestHeight,
						memDC, 0, 0, nDestWidth, nDestHeight, btSourceConstantAlpha);
				if (nRet != 0)
				{
					break;
				}
			}
			else if (nDestWidth <= himgSrc->GetWidth() && nDestHeight > himgSrc->GetHeight())
			{
				// top
				nRet = StretchAlphaBlend(hdcDest, nXDest, nYDest, nDestWidth, pt9GridResize.y,
						memDC, 0, 0, nDestWidth, pt9GridResize.y, btSourceConstantAlpha);
				if (nRet != 0)
				{
					break;
				}

				// center, not stretch at horizontal(ignore pt9GridResize.x and l9GridRepeatX),
				// tile at vertical.
				if (pt9GridResize.y < himgSrc->GetWidth())	// in picture
				{
					nRet = AlphaTileBlt(hdcDest,
							nXDest, nYDest + pt9GridResize.y, nDestWidth, nDestLine1Height,
							memDC,
							0, pt9GridResize.y, nDestWidth, l9GridRepeatY, btSourceConstantAlpha);
					if (nRet != 0)
					{
						break;
					}
				}

				// bottom
				nRet = StretchAlphaBlend(hdcDest,
						nXDest, nYDest + nDestLine2Top, nDestWidth, nDestLine2Height,
						memDC,
						0, pt9GridResize.y + 1, nDestWidth, nDestLine2Height, btSourceConstantAlpha);
				if (nRet != 0)
				{
					break;
				}
			}
			else if (nDestWidth > himgSrc->GetWidth() && nDestHeight <= himgSrc->GetHeight())
			{
				// left
				nRet = StretchAlphaBlend(hdcDest,
						nXDest, nYDest, pt9GridResize.x, nDestHeight,
						memDC,
						0, 0, pt9GridResize.x, nDestHeight, btSourceConstantAlpha);
				if (nRet != 0)
				{
					break;
				}

				// center, not stretch at vertical, tile at horizontal.
				if (pt9GridResize.x < himgSrc->GetWidth())	// in picture
				{
					nRet = AlphaTileBlt(hdcDest,
							nXDest + pt9GridResize.x, nYDest, nDestCol1Width, nDestHeight,
							memDC,
							pt9GridResize.x, 0, l9GridRepeatX, nDestHeight, btSourceConstantAlpha);
					if (nRet != 0)
					{
						break;
					}
				}

				// right
				nRet = StretchAlphaBlend(hdcDest,
						nXDest + nDestCol2Left, nYDest, nDestCol2Width, nDestHeight,
						memDC,
						pt9GridResize.x + 1, 0, nDestCol2Width, nDestHeight, btSourceConstantAlpha);
				if (nRet != 0)
				{
					break;
				}
			}
			else
			{
				// area 0
				if (pt9GridResize.x > 0 && pt9GridResize.y > 0)
				{
					nRet = StretchAlphaBlend(hdcDest,
							nXDest, nYDest, pt9GridResize.x, pt9GridResize.y,
							memDC,
							0, 0, pt9GridResize.x, pt9GridResize.y, btSourceConstantAlpha);
					if (nRet != 0)
					{
						break;
					}
				}
				// area 1
				if (pt9GridResize.y > 0)
				{
					nRet = AlphaTileBlt(hdcDest,
							nXDest + pt9GridResize.x, nYDest, nDestCol1Width, pt9GridResize.y,
							memDC,
							pt9GridResize.x, 0, l9GridRepeatX, pt9GridResize.y, btSourceConstantAlpha);
					if (nRet != 0)
					{
						break;
					}
				}
				// area 2
				if (pt9GridResize.x < himgSrc->GetWidth() - 1 && pt9GridResize.y > 0)
				{
					nRet = StretchAlphaBlend(hdcDest,
							nXDest + nDestCol2Left, nYDest, nDestCol2Width, pt9GridResize.y,
							memDC,
							pt9GridResize.x + 1, 0, nDestCol2Width, pt9GridResize.y, btSourceConstantAlpha);
					if (nRet != 0)
					{
						break;
					}
				}

				// area 3
				if (pt9GridResize.x > 0)
				{
					nRet = AlphaTileBlt(hdcDest,
							nXDest, nYDest + pt9GridResize.y, pt9GridResize.x, nDestLine1Height,
							memDC,
							0, pt9GridResize.y, pt9GridResize.x, l9GridRepeatY, btSourceConstantAlpha);
					if (nRet != 0)
					{
						break;
					}
				}
				// area 4
				ASSERT(nDestCol1Width > 0 && nDestLine1Height > 0);
				nRet = AlphaTileBlt(hdcDest,
						nXDest + pt9GridResize.x, nYDest + pt9GridResize.y, nDestCol1Width, nDestLine1Height,
						memDC,
						pt9GridResize.x, pt9GridResize.y, l9GridRepeatX, l9GridRepeatY, btSourceConstantAlpha);
				if (nRet != 0)
				{
					break;
				}
				// area 5
				if (nDestCol2Width > 0)
				{
					nRet = AlphaTileBlt(hdcDest,
							nXDest + nDestCol2Left, nYDest + pt9GridResize.y,
							nDestCol2Width, nDestLine1Height,
							memDC,
							pt9GridResize.x + 1, pt9GridResize.y,
							nDestCol2Width, l9GridRepeatY, btSourceConstantAlpha);
					if (nRet != 0)
					{
						break;
					}
				}

				// area 6
				if (pt9GridResize.x > 0 && nDestLine2Height > 0)
				{
					nRet = StretchAlphaBlend(hdcDest,
							nXDest, nYDest + nDestLine2Top, pt9GridResize.x, nDestLine2Height,
							memDC,
							0, pt9GridResize.y + 1, pt9GridResize.x, nDestLine2Height,
							btSourceConstantAlpha);
					if (nRet != 0)
					{
						break;
					}
				}
				// area 7
				if (nDestLine2Height > 0)
				{
					nRet = AlphaTileBlt(hdcDest,
							nXDest + pt9GridResize.x, nYDest + nDestLine2Top,
							nDestCol1Width, nDestLine2Height,
							memDC,
							pt9GridResize.x, pt9GridResize.y + 1, l9GridRepeatX, nDestLine2Height,
							btSourceConstantAlpha);
					if (nRet != 0)
					{
						break;
					}
				}
				// area 8
				if (nDestCol2Width > 0 && nDestLine2Height > 0)
				{
					nRet = StretchAlphaBlend(hdcDest,
							nXDest + nDestCol2Left, nYDest + nDestLine2Top,
							nDestCol2Width, nDestLine2Height,
							memDC,
							pt9GridResize.x + 1, pt9GridResize.y + 1, nDestCol2Width, nDestLine2Height,
							btSourceConstantAlpha);
					if (nRet != 0)
					{
						break;
					}
				}
			}

			break;
		}

		::SelectObject(memDC, pSrcBmpOld);
		::DeleteDC(memDC);

		return nRet;
	}
	else
	{
		return NineGridBltG(hdcDest, nXDest, nYDest, nDestWidth, nDestHeight,
				himgSrc->GetSafeHBITMAP(), pt9GridResize, l9GridRepeatX, l9GridRepeatY);
	}
}

int LibUIDK::Render::IUINineGridBltEx(HDC hdcDest, LPCRECT lprcDest,
	HIUIIMAGE hbmpSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	COLORREF crColorKey, BYTE btSourceConstantAlpha)
{
	if (lprcDest == NULL)
	{
		return -1;
	}

	return IUINineGridBltEx(hdcDest,
			lprcDest->left, lprcDest->top, RECTWIDTH(*lprcDest), RECTHEIGHT(*lprcDest),
			hbmpSrc, pt9GridResize, l9GridRepeatX, l9GridRepeatY,
			crColorKey, btSourceConstantAlpha);
}

int LibUIDK::Render::IUIPartNineGridBlt(HDC hdcDest,
	int nXDest, int nYDest, int nDestWidth, int nDestHeight,
	HIUIIMAGE himgSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	int nPart, int nPartIndex)
{
	return IUIPartNineGridBltEx(hdcDest, nXDest, nYDest, nDestWidth, nDestHeight,
			himgSrc, pt9GridResize, l9GridRepeatX, l9GridRepeatY,
			nPart, nPartIndex, -1, 255, FALSE);
}

int LibUIDK::Render::IUIPartNineGridBlt(HDC hdcDest, LPCRECT lprcDest,
	HIUIIMAGE himgSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	int nPart, int nPartIndex)
{
	return IUIPartNineGridBltEx(hdcDest, lprcDest,
			himgSrc, pt9GridResize, l9GridRepeatX, l9GridRepeatY,
			nPart, nPartIndex, -1, 255, FALSE);
}

int LibUIDK::Render::IUIPartNineGridBlt(HDC hdcDest, LPCRECT lprcDest,
	HIUIIMAGE himgSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	LPCRECT lprcSrc)
{
	return IUIPartNineGridBltEx(hdcDest, lprcDest,
			himgSrc, pt9GridResize, l9GridRepeatX, l9GridRepeatY, lprcSrc, -1, 255, FALSE);
}

// 与IUIPartNineGridBlt作用一样，仅仅需要重设一下resize point，设置成相对于截取部分
int LibUIDK::Render::IUIPartNineGridBltResetResizePoint(HDC hdcDest, LPCRECT lprcDest,
	HIUIIMAGE himgSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	LPCRECT lprcSrc)
{
	return IUIPartNineGridBltEx(hdcDest, lprcDest,
			himgSrc, pt9GridResize, l9GridRepeatX, l9GridRepeatY, lprcSrc, -1, 255, TRUE);
}

int LibUIDK::Render::IUIPartNineGridBltEx(HDC hdcDest, int nXDest, int nYDest,
	int nDestWidth, int nDestHeight,
	HIUIIMAGE himgSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	int nPart, int nPartIndex,
	COLORREF crColorKey/* = -1*/, BYTE btSourceConstantAlpha/* = 255*/,
	BOOL bResetResizePoint/* = FALSE*/)
{
	himgSrc->SafeLoadSavedImage();
	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -1;
	}
	if (nPart <= 0)
	{
		return -2;
	}
	if (nPartIndex < 0)
	{
		return -3;
	}
	if (nPartIndex >= nPart)
	{
		return -4;
	}

	CRect rcSrc;
	rcSrc.left = himgSrc->GetWidth() * nPartIndex / nPart;
	rcSrc.top = 0;
	rcSrc.right = rcSrc.left + himgSrc->GetWidth() / nPart;
	rcSrc.bottom = rcSrc.top + himgSrc->GetHeight();

	return IUIPartNineGridBltEx(hdcDest,
			CRect(nXDest, nYDest, nXDest + nDestWidth, nYDest + nDestHeight),
			himgSrc, pt9GridResize, l9GridRepeatX, l9GridRepeatY, rcSrc,
			crColorKey, btSourceConstantAlpha, bResetResizePoint);
}

int LibUIDK::Render::IUIPartNineGridBltEx(HDC hdcDest, LPCRECT lprcDest,
	HIUIIMAGE hbmpSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	int nPart, int nPartIndex,
	COLORREF crColorKey/* = -1*/, BYTE btSourceConstantAlpha/* = 255*/,
	BOOL bResetResizePoint/* = FALSE*/)
{
	if (lprcDest == NULL)
	{
		return -1;
	}

	return IUIPartNineGridBltEx(hdcDest,
			lprcDest->left, lprcDest->top, RECTWIDTH(*lprcDest), RECTHEIGHT(*lprcDest),
			hbmpSrc, pt9GridResize, l9GridRepeatX, l9GridRepeatY, nPart, nPartIndex,
			crColorKey, btSourceConstantAlpha, bResetResizePoint);
}

int LibUIDK::Render::IUIPartNineGridBltEx(HDC hdcDest, LPCRECT lprcDest,
	HIUIIMAGE himgSrc, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	LPCRECT lprcSrc, COLORREF crColorKey/* = -1*/, BYTE btSourceConstantAlpha/* = 255*/,
	BOOL bResetResizePoint/* = FALSE*/)
{
	if (himgSrc == NULL || lprcDest == NULL)
	{
		return -1;
	}

	himgSrc->SafeLoadSavedImage();

	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -1;
	}

	CRect rcDest(lprcDest);

	CRect rcSrc(lprcSrc);
	if (rcSrc.IsRectEmpty())
	{
		rcSrc.left = 0;
		rcSrc.top = 0;
		rcSrc.right = himgSrc->GetWidth();
		rcSrc.bottom = himgSrc->GetHeight();
	}

	if (bResetResizePoint)
	{
		// 传入的缩放点，是相对于原图的。 换算成相对于part部分的图。
		pt9GridResize.x = (pt9GridResize.x - rcSrc.left);
		pt9GridResize.y = (pt9GridResize.y - rcSrc.top);
	}

	if (himgSrc->IsHasAlphaChannel())
	{
		HDC memDC = ::CreateCompatibleDC(hdcDest);
		HBITMAP pSrcBmpOld = (HBITMAP)::SelectObject(memDC, himgSrc->GetSafeHBITMAP());

		int nDestCol1Width = rcDest.Width() - rcSrc.Width() + 1;
		int nDestLine1Height = rcDest.Height() - rcSrc.Height() + 1;

		int nDestCol2Width = rcSrc.Width() - pt9GridResize.x - 1;
		int nDestCol2Left = rcDest.Width() - nDestCol2Width;
		if (nDestCol2Left < pt9GridResize.x)
		{
			nDestCol2Left = pt9GridResize.x;
		}

		int nDestLine2Height = rcSrc.Height() - pt9GridResize.y - 1;
		int nDestLine2Top = rcDest.Height() - nDestLine2Height;
		if (nDestLine2Top < pt9GridResize.y)
		{
			nDestLine2Top = pt9GridResize.y;
		}

		int nRet = -1;
		while (true)
		{
			// if the dest size less than source size, not stretch.
			if (rcDest.Width() <= rcSrc.Width() && rcDest.Height() <= rcSrc.Height())
			{
				nRet = StretchAlphaBlend(hdcDest,
						rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(),
						memDC,
						rcSrc.left, rcSrc.top, rcDest.Width(), rcDest.Height(), btSourceConstantAlpha);
				if (nRet != 0)
				{
					break;
				}
			}
			else if (rcDest.Width() <= rcSrc.Width() && rcDest.Height() > rcSrc.Height())
			{
				// top
				nRet = StretchAlphaBlend(hdcDest,
						rcDest.left, rcDest.top, rcDest.Width(), pt9GridResize.y,
						memDC,
						rcSrc.left, rcSrc.top, rcDest.Width(), pt9GridResize.y, btSourceConstantAlpha);
				if (nRet != 0)
				{
					break;
				}

				// center, not stretch at horizontal(ignore pt9GridResize.x and l9GridRepeatX),
				// tile at vertical.
				if (pt9GridResize.y < rcSrc.Width())	// in picture
				{
					nRet = AlphaTileBlt(hdcDest,
							rcDest.left, rcDest.top + pt9GridResize.y, rcDest.Width(), nDestLine1Height,
							memDC,
							rcSrc.left, rcSrc.top + pt9GridResize.y, rcDest.Width(), l9GridRepeatY,
							btSourceConstantAlpha);
					if (nRet != 0)
					{
						break;
					}
				}

				// bottom
				nRet = StretchAlphaBlend(hdcDest,
						rcDest.left, rcDest.top + nDestLine2Top, rcDest.Width(), nDestLine2Height,
						memDC,
						rcSrc.left, rcSrc.top + pt9GridResize.y + 1, rcDest.Width(), nDestLine2Height,
						btSourceConstantAlpha);

				if (nRet != 0)
				{
					break;
				}
			}
			else if (rcDest.Width() > rcSrc.Width() && rcDest.Height() <= rcSrc.Height())
			{
				// left
				nRet = StretchAlphaBlend(hdcDest,
						rcDest.left, rcDest.top, pt9GridResize.x, rcDest.Height(),
						memDC,
						rcSrc.left, rcSrc.top, pt9GridResize.x, rcDest.Height(),
						btSourceConstantAlpha);
				if (nRet != 0)
				{
					break;
				}

				// center, not stretch at vertical, tile at horizontal.
				if (pt9GridResize.x < rcSrc.Width())	// in picture
				{
					nRet = AlphaTileBlt(hdcDest,
							rcDest.left + pt9GridResize.x, rcDest.top, nDestCol1Width, rcDest.Height(),
							memDC,
							rcSrc.left + pt9GridResize.x, rcSrc.top, l9GridRepeatX, rcDest.Height(),
							btSourceConstantAlpha);

					if (nRet != 0)
					{
						break;
					}
				}

				// right
				nRet = StretchAlphaBlend(hdcDest,
						rcDest.left + nDestCol2Left, rcDest.top, nDestCol2Width, rcDest.Height(),
						memDC,
						rcSrc.left + pt9GridResize.x + 1, rcSrc.top, nDestCol2Width, rcDest.Height(),
						btSourceConstantAlpha);

				if (nRet != 0)
				{
					break;
				}
			}
			else
			{
				// area 0
				if (pt9GridResize.x > 0 && pt9GridResize.y > 0)
				{
					nRet = StretchAlphaBlend(hdcDest,
							rcDest.left, rcDest.top, pt9GridResize.x, pt9GridResize.y,
							memDC, rcSrc.left, rcSrc.top, pt9GridResize.x, pt9GridResize.y,
							btSourceConstantAlpha);
					if (nRet != 0)
					{
						break;
					}
				}
				// area 1
				if (pt9GridResize.y > 0)
				{
					nRet = AlphaTileBlt(hdcDest,
							rcDest.left + pt9GridResize.x, rcDest.top, nDestCol1Width, pt9GridResize.y,
							memDC,
							rcSrc.left + pt9GridResize.x, rcSrc.top, l9GridRepeatX, pt9GridResize.y,
							btSourceConstantAlpha);

					if (nRet != 0)
					{
						break;
					}
				}
				// area 2
				if (pt9GridResize.x < rcSrc.Width() - 1 && pt9GridResize.y > 0)
				{
					nRet = StretchAlphaBlend(hdcDest,
							rcDest.left + nDestCol2Left, rcDest.top, nDestCol2Width, pt9GridResize.y,
							memDC,
							rcSrc.left + pt9GridResize.x + 1, rcSrc.top, nDestCol2Width, pt9GridResize.y,
							btSourceConstantAlpha);

					if (nRet != 0)
					{
						break;
					}
				}

				// area 3
				if (pt9GridResize.x > 0)
				{
					nRet = AlphaTileBlt(hdcDest,
							rcDest.left, rcDest.top + pt9GridResize.y, pt9GridResize.x, nDestLine1Height,
							memDC, rcSrc.left, rcSrc.top + pt9GridResize.y, pt9GridResize.x, l9GridRepeatY,
							btSourceConstantAlpha);

					if (nRet != 0)
					{
						break;
					}
				}
				// area 4
				ASSERT(nDestCol1Width > 0 && nDestLine1Height > 0);
				nRet = AlphaTileBlt(hdcDest,
						rcDest.left + pt9GridResize.x, rcDest.top + pt9GridResize.y,
						nDestCol1Width, nDestLine1Height,
						memDC,
						rcSrc.left + pt9GridResize.x, rcSrc.top + pt9GridResize.y, l9GridRepeatX, l9GridRepeatY,
						btSourceConstantAlpha);

				if (nRet != 0)
				{
					break;
				}
				// area 5
				if (nDestCol2Width > 0)
				{
					nRet = AlphaTileBlt(hdcDest,
							rcDest.left + nDestCol2Left, rcDest.top + pt9GridResize.y,
							nDestCol2Width, nDestLine1Height,
							memDC,
							rcSrc.left + pt9GridResize.x + 1, rcSrc.top + pt9GridResize.y,
							nDestCol2Width, l9GridRepeatY, btSourceConstantAlpha);

					if (nRet != 0)
					{
						break;
					}
				}

				// area 6
				if (pt9GridResize.x > 0 && nDestLine2Height > 0)
				{
					nRet = StretchAlphaBlend(hdcDest,
							rcDest.left, rcDest.top + nDestLine2Top, pt9GridResize.x, nDestLine2Height,
							memDC,
							rcSrc.left, rcSrc.top + pt9GridResize.y + 1, pt9GridResize.x, nDestLine2Height,
							btSourceConstantAlpha);

					if (nRet != 0)
					{
						break;
					}
				}
				// area 7
				if (nDestLine2Height > 0)
				{
					nRet = AlphaTileBlt(hdcDest,
							rcDest.left + pt9GridResize.x, rcDest.top + nDestLine2Top,
							nDestCol1Width, nDestLine2Height,
							memDC,
							rcSrc.left + pt9GridResize.x, rcSrc.top + pt9GridResize.y + 1,
							l9GridRepeatX, nDestLine2Height, btSourceConstantAlpha);

					if (nRet != 0)
					{
						break;
					}
				}
				// area 8
				if (nDestCol2Width > 0 && nDestLine2Height > 0)
				{
					nRet = StretchAlphaBlend(hdcDest,
							rcDest.left + nDestCol2Left, rcDest.top + nDestLine2Top,
							nDestCol2Width, nDestLine2Height,
							memDC,
							rcSrc.left + pt9GridResize.x + 1, rcSrc.top + pt9GridResize.y + 1,
							nDestCol2Width, nDestLine2Height, btSourceConstantAlpha);

					if (nRet != 0)
					{
						break;
					}
				}
			}

			break;
		}

		::SelectObject(memDC, pSrcBmpOld);
		::DeleteDC(memDC);

		return nRet;
	}
	else
	{
		return PartNineGridBltG(hdcDest, rcDest,
				himgSrc->GetSafeHBITMAP(), pt9GridResize, l9GridRepeatX, l9GridRepeatY, rcSrc);
	}
}

int LibUIDK::Render::IUIStretchBlt(HDC hdcDest,
	int nXDest,
	int nYDest, int nDestWidth, int nDestHeight,
	HIUIIMAGE himgSrc,
	int nXSrc, int nYSrc, int nSrcWidth, int nSrcHeight)
{
	if (hdcDest == NULL)
	{
		return -1;
	}

	himgSrc->SafeLoadSavedImage();
	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	::SetStretchBltMode(hdcDest, COLORONCOLOR);

	HDC memDC = ::CreateCompatibleDC(hdcDest);
	HBITMAP pSrcBmpOld = (HBITMAP)::SelectObject(memDC, (HBITMAP)(*himgSrc));

	if (himgSrc->IsHasAlphaChannel())
	{
		StretchAlphaBlend(hdcDest,
			nXDest, nYDest, nDestWidth, nDestHeight,
			memDC,
			nXSrc, nYSrc, nSrcWidth, nSrcHeight, 255);
	}
	else
	{
		::StretchBlt(hdcDest,
			nXDest, nYDest, nDestWidth, nDestHeight,
			memDC,
			nXSrc, nYSrc, nSrcWidth, nSrcHeight, SRCCOPY);
	}

	::SelectObject(memDC, pSrcBmpOld);
	::DeleteDC(memDC);

	return 0;
}

int LibUIDK::Render::IUIStretchBlt(HDC hdcDest,
	int nXDest, int nYDest, int nDestWidth, int nDestHeight,
	HIUIIMAGE himgSrc)
{
	if (hdcDest == NULL)
	{
		return -1;
	}

	if (himgSrc == NULL)
	{
		return -2;
	}

	himgSrc->SafeLoadSavedImage();
	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	if (nDestWidth <= 0 || nDestHeight <= 0)
	{
		return -4;
	}

	return IUIStretchBlt(hdcDest,
			nXDest, nYDest, nDestWidth, nDestHeight,
			himgSrc,
			0, 0, himgSrc->GetWidth(), himgSrc->GetHeight());
}

int LibUIDK::Render::IUIStretchBlt(HDC hdcDest, LPCRECT lprcDest, HIUIIMAGE himgSrc)
{
	if (hdcDest == NULL)
	{
		return -1;
	}

	if (himgSrc == NULL)
	{
		return -2;
	}

	himgSrc->SafeLoadSavedImage();
	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	CRect rcDest(lprcDest);
	if (rcDest.IsRectEmpty() || rcDest.IsRectNull())
	{
		return -4;
	}

	return IUIStretchBlt(hdcDest,
			rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(), himgSrc);
}

Bitmap *CreateBitmapFromHBITMAP(IN HBITMAP hBitmap)
{
	BITMAP bmp = { 0 };
	if (0 == GetObject(hBitmap, sizeof(BITMAP), (LPVOID)&bmp))
	{
		return FALSE;
	}

	// Although we can get bitmap data address by bmp.bmBits member of BITMAP
	// which is got by GetObject function sometime,
	// we can determine the bitmap data in the HBITMAP is arranged bottom-up
	// or top-down, so we should always use GetDIBits to get bitmap data.
	BYTE *piexlsSrc = NULL;
	Bitmap *pBitmap = NULL;

	BOOL bOK = TRUE;
	do
	{
		LONG cbSize = bmp.bmWidthBytes * bmp.bmHeight;
		piexlsSrc = new BYTE[cbSize];

		BITMAPINFO bmpInfo = { 0 };
		// We should initialize the first six members of BITMAPINFOHEADER structure.
		// A bottom-up DIB is specified by setting the height to a positive number,
		// while a top-down DIB is specified by setting the height to a negative number.
		bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpInfo.bmiHeader.biWidth = bmp.bmWidth;
		bmpInfo.bmiHeader.biHeight = bmp.bmHeight; // 正数，说明数据从下到上，如未负数，则从上到下
		bmpInfo.bmiHeader.biPlanes = bmp.bmPlanes;
		bmpInfo.bmiHeader.biBitCount = bmp.bmBitsPixel;
		bmpInfo.bmiHeader.biCompression = BI_RGB;

		HDC hdcScreen = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
		LONG cbCopied = GetDIBits(hdcScreen, hBitmap, 0, bmp.bmHeight,
				piexlsSrc, &bmpInfo, DIB_RGB_COLORS);
		DeleteDC(hdcScreen);
		if (0 == cbCopied)
		{
			bOK = FALSE;
			break;
		}

		// Create an GDI+ Bitmap has the same dimensions with hbitmap
		pBitmap = ::new Bitmap(bmp.bmWidth, bmp.bmHeight, PixelFormat32bppPARGB);

		// Access to the Gdiplus::Bitmap's pixel data
		BitmapData bitmapData;
		Rect rect(0, 0, bmp.bmWidth, bmp.bmHeight);
		if (Ok != pBitmap->LockBits(&rect, ImageLockModeRead,
				PixelFormat32bppPARGB, &bitmapData))
		{
			bOK = FALSE;
			break;
		}

		BYTE *pixelsDest = (BYTE *)bitmapData.Scan0;
		int nLinesize = bmp.bmWidth * sizeof(UINT);
		int nHeight = bmp.bmHeight;

		// Copy pixel data from HBITMAP by bottom-up.
		for (int y = 0; y < nHeight; y++)
		{
			// 从下到上复制数据，因为前面设置高度时是正数。
#if (_MSC_VER > 1310)
			memcpy_s(
				(pixelsDest + y * nLinesize),
				nLinesize,
				(piexlsSrc + (nHeight - y - 1) * nLinesize),
				nLinesize);
#else
			memcpy(
				(pixelsDest + y * nLinesize),
				(piexlsSrc + (nHeight - y - 1) * nLinesize),
				nLinesize);
#endif
		}

		// Copy the data in temporary buffer to pBitmap
		if (Ok != pBitmap->UnlockBits(&bitmapData))
		{
			bOK = FALSE;
			break;
		}
	}
	while (0);
	SafeArrayDelete(piexlsSrc);

	if (!bOK)
	{
		::delete pBitmap;
		pBitmap = NULL;
	}

	return pBitmap;
}

int LibUIDK::Render::IUIAlphaStretchBlt(HDC hdcDest,
	int nXDest, int nYDest, int nDestWidth, int nDestHeight,
	HIUIIMAGE himgSrc, BYTE btSourceConstantAlpha, IMAGE_RESIZE_MODE eStretchQuality)
{
	if (hdcDest == NULL || himgSrc == NULL)
	{
		return -1;
	}

	himgSrc->SafeLoadSavedImage();
	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	if (eStretchQuality == IRM_STRETCH_HIGH_QUALITY)
	{
		Graphics g(hdcDest);
		g.SetInterpolationMode(InterpolationModeHighQuality);
		g.SetPixelOffsetMode(PixelOffsetModeHighQuality);
		Bitmap *p = CreateBitmapFromHBITMAP(himgSrc->GetSafeHBITMAP());//Bitmap::FromHBITMAP(himgSrc->GetSafeHBITMAP(), NULL);
		Rect rc(nXDest, nYDest, nDestWidth, nDestHeight);
		g.DrawImage(p, rc);

		::delete p;
		p = NULL;
	}
	else
	{
		::SetStretchBltMode(hdcDest, COLORONCOLOR);

		HDC memDC = ::CreateCompatibleDC(hdcDest);
		HBITMAP pSrcBmpOld = (HBITMAP)::SelectObject(memDC, (HBITMAP)(*himgSrc));

		if (himgSrc->IsHasAlphaChannel())
		{
			StretchAlphaBlend(hdcDest, nXDest, nYDest, nDestWidth, nDestHeight,
				memDC, 0, 0, himgSrc->GetWidth(), himgSrc->GetHeight(), btSourceConstantAlpha);
		}
		else
		{
			::StretchBlt(hdcDest, nXDest, nYDest, nDestWidth, nDestHeight,
				memDC, 0, 0, himgSrc->GetWidth(), himgSrc->GetHeight(), SRCCOPY);
		}

		::SelectObject(memDC, pSrcBmpOld);
		::DeleteDC(memDC);
	}

	return 0;
}

int LibUIDK::Render::IUIAlphaStretchBlt(HDC hdcDest,
	LPCRECT lprcDest, HIUIIMAGE himgSrc, BYTE btSourceConstantAlpha,
	IMAGE_RESIZE_MODE eStretchQuality)
{
	if (lprcDest == NULL)
	{
		return -1;
	}

	return IUIAlphaStretchBlt(hdcDest,
			lprcDest->left, lprcDest->top, RECTWIDTH(*lprcDest), RECTHEIGHT(*lprcDest),
			himgSrc, btSourceConstantAlpha, eStretchQuality);
}

int LibUIDK::Render::IUIPartStretchBlt(HDC hdcDest, int nXDest, int nYDest, int nDestWidth, int nDestHeight,
	HIUIIMAGE himgSrc, int nXSrc, int nYSrc, int nSrcWidth, int nSrcHeight,
	IMAGE_RESIZE_MODE eStretchQuality)
{
	if (hdcDest == NULL || himgSrc == NULL)
	{
		return -1;
	}

	himgSrc->SafeLoadSavedImage();
	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	::SetStretchBltMode(hdcDest, COLORONCOLOR);

	HDC memDC = ::CreateCompatibleDC(hdcDest);
	HBITMAP pSrcBmpOld = (HBITMAP)::SelectObject(memDC, himgSrc->GetSafeHBITMAP());

	if (himgSrc->IsHasAlphaChannel())
	{
		StretchAlphaBlend(hdcDest, nXDest, nYDest, nDestWidth, nDestHeight,
			memDC, nXSrc, nYSrc, nSrcWidth, nSrcHeight, 255);
	}
	else
	{
		::StretchBlt(hdcDest, nXDest, nYDest, nDestWidth, nDestHeight,
			memDC, nXSrc, nYSrc, nSrcWidth, nSrcHeight, SRCCOPY);
	}

	::SelectObject(memDC, pSrcBmpOld);
	::DeleteDC(memDC);

	return 0;
}

int LibUIDK::Render::IUIPartStretchBlt(HDC hdcDest, LPCRECT lprcDest, HIUIIMAGE himgSrc, LPCRECT lprcSrc,
	IMAGE_RESIZE_MODE eStretchQuality)
{
	if (hdcDest == NULL || lprcDest == NULL || himgSrc == NULL || lprcSrc == NULL)
	{
		return -1;
	}

	himgSrc->SafeLoadSavedImage();
	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	return IUIPartStretchBlt(hdcDest,
			lprcDest->left,
			lprcDest->top,
			lprcDest->right - lprcDest->left,
			lprcDest->bottom - lprcDest->top,
			himgSrc,
			lprcSrc->left,
			lprcSrc->top,
			lprcSrc->right - lprcSrc->left,
			lprcSrc->bottom - lprcSrc->top,
			eStretchQuality);
}

int LibUIDK::Render::IUIPartStretchBlt(HDC hdcDest,
	int nXDest, int nYDest, int nDestWidth, int nDestHeight,
	HIUIIMAGE himgSrc,
	int nPart, int nPartIndex, IMAGE_RESIZE_MODE eStretchQuality)
{
	if (hdcDest == NULL || himgSrc == NULL)
	{
		return -1;
	}

	himgSrc->SafeLoadSavedImage();
	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -2;
	}

	if (nPart <= 0)
	{
		return -3;
	}
	if (nPartIndex < 0)
	{
		return -4;
	}
	if (nPartIndex >= nPart)
	{
		return -5;
	}

	int nSrcWidth = himgSrc->GetWidth() / nPart;

	return IUIPartStretchBlt(hdcDest, nXDest, nYDest, nDestWidth, nDestHeight,
			himgSrc, nSrcWidth * nPartIndex, 0, nSrcWidth, himgSrc->GetHeight(),
			eStretchQuality);
}

int LibUIDK::Render::IUIPartStretchBlt(HDC hdcDest, LPCRECT lprcDest,
	HIUIIMAGE himgSrc, int nPart, int nPartIndex, IMAGE_RESIZE_MODE eStretchQuality)
{
	if (lprcDest == NULL)
	{
		return -1;
	}

	return IUIPartStretchBlt(hdcDest,
			lprcDest->left, lprcDest->top, RECTWIDTH(*lprcDest), RECTHEIGHT(*lprcDest),
			himgSrc,
			nPart, nPartIndex, eStretchQuality);
}

int LibUIDK::Render::IUITileBlt(HDC hdcDest, LPCRECT lprcDest,
	HIUIIMAGE himgSrc,
	int nXOriginSrc/* = 0*/, int nYOriginSrc/* = 0*/,
	int nSrcWidth/* = -1*/, int nSrcHeight/* = -1*/)
{
	if (himgSrc == NULL || lprcDest == NULL)
	{
		return -1;
	}

	himgSrc->SafeLoadSavedImage();
	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	HDC hdcMem = CreateCompatibleDC(hdcDest);
	HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcMem, (HBITMAP)(*himgSrc));

	if (nSrcWidth == -1)
	{
		nSrcWidth = himgSrc->GetWidth();
	}
	if (nSrcHeight == -1)
	{
		nSrcHeight = himgSrc->GetHeight();
	}

	if (himgSrc->IsHasAlphaChannel())
	{
		AlphaTileBlt(hdcDest,
			lprcDest->left, lprcDest->top, RECTWIDTH(*lprcDest), RECTHEIGHT(*lprcDest),
			hdcMem, nXOriginSrc, nYOriginSrc, nSrcWidth, nSrcHeight, 255);
	}
	else
	{
		TileBlt(hdcDest,
			lprcDest->left, lprcDest->top, RECTWIDTH(*lprcDest), RECTHEIGHT(*lprcDest),
			hdcMem, nXOriginSrc, nYOriginSrc, nSrcWidth, nSrcHeight, 255);
	}

	SelectObject(hdcMem, hbmpOld);
	DeleteDC(hdcMem);

	return 0;
}

int LibUIDK::Render::IUICenterBitBlt(HDC hDC, LPCRECT lprcDest, HIUIIMAGE himgSrc, DWORD dwRop)
{
	if (lprcDest == NULL || himgSrc == NULL)
	{
		return -1;
	}

	himgSrc->SafeLoadSavedImage();
	if (himgSrc->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	CRect rcNewDest;
	rcNewDest.left = lprcDest->left + (RECTWIDTH(*lprcDest) - himgSrc->GetWidth()) / 2;
	rcNewDest.right = rcNewDest.left + himgSrc->GetWidth();
	rcNewDest.top = lprcDest->top + (RECTHEIGHT(*lprcDest) - himgSrc->GetHeight()) / 2;
	rcNewDest.bottom = rcNewDest.top + himgSrc->GetHeight();

	return IUIBitBlt(hDC,
			rcNewDest.left, rcNewDest.top, rcNewDest.Width(), rcNewDest.Height(),
			himgSrc, 0, 0, dwRop);
}

int LibUIDK::Render::IUIBitBlt(HDC hdcDest,
	int nXDest, int nYDest, int nDestWidth, int nDestHeight,
	HIUIIMAGE hbmpSrc,
	int nXSrc, int nYSrc, DWORD dwRop)
{
	if (hbmpSrc == NULL)
	{
		return -1;
	}

	hbmpSrc->SafeLoadSavedImage();
	if (hbmpSrc->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	ASSERT(hdcDest != NULL);

	if (hbmpSrc->IsHasAlphaChannel())
	{
		HDC hdcMem = CreateCompatibleDC(hdcDest);

		HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcMem, (HBITMAP)(*hbmpSrc));
		int nRet = StretchAlphaBlend(hdcDest,
				nXDest, nYDest, nDestWidth, nDestHeight,
				hdcMem,
				nXSrc, nYSrc, nDestWidth, nDestHeight, 255);
		SelectObject(hdcMem, hbmpOld);

		DeleteDC(hdcMem);

		return nRet;
	}
	else
	{
		return BitBltG(hdcDest, nXDest, nYDest, nDestWidth, nDestHeight,
				*hbmpSrc, nXSrc, nYSrc, dwRop);
	}
}

int LibUIDK::Render::IUIBitBlt(HDC hdcDest, LPCRECT lprcDest,
	HIUIIMAGE hSrcBmp, int nXSrc, int nYSrc, DWORD dwRop)
{
	if (lprcDest == NULL)
	{
		return -1;
	}

	return IUIBitBlt(hdcDest,
			lprcDest->left, lprcDest->top, RECTWIDTH(*lprcDest), RECTHEIGHT(*lprcDest),
			hSrcBmp, nXSrc, nYSrc, dwRop);
}

int LibUIDK::Render::IUITileClipImage(Graphics *pGraphics, GraphicsPath *pDestClipPath,
	Image *pSrcImage, LPCRECT lprcSrc)
{
	if (pGraphics == NULL
		|| pDestClipPath == NULL
		|| pSrcImage == NULL)
	{
		return -1;
	}

	pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);

	// constructe a TextureBrush object need long time.
	// If you need tile the same TextureBrush many times,
	// you can call Graphics::FillPath directly, instead of IUITileClipImage.
	TextureBrush *pTextureBrush = NULL;
	if (lprcSrc != NULL)
	{
		Rect rect(
			lprcSrc->left,
			lprcSrc->top,
			lprcSrc->right - lprcSrc->left,
			lprcSrc->bottom - lprcSrc->top);

#ifdef _DEBUG
#ifdef new
#undef new
#endif
#endif

		pTextureBrush = new TextureBrush(pSrcImage, rect);
	}
	else
	{
		pTextureBrush = new TextureBrush(pSrcImage);
	}

	pGraphics->FillPath(pTextureBrush, pDestClipPath);

	SafeDelete(pTextureBrush);

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

	return 0;
}

int LibUIDK::Render::IUIDrawForeground(HDC hDstDC, LPCRECT lprcClient, LPCRECT lprcForegroundMargin,
	HIUIIMAGE himgFg,
	FOREGROUND_ALIGN_HOR eForegroundAlignHor, FOREGROUND_ALIGN_VER eForegroundAlignVer,
	COLORREF crMask, BYTE btFgImgTransparent/* = 255*/)
{
	if (hDstDC == NULL
		|| lprcClient == NULL
		|| himgFg == NULL)
	{
		return -1;
	}

	himgFg->SafeLoadSavedImage();
	if (himgFg->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	CRect rcControl(lprcClient);
	if (lprcForegroundMargin != NULL)
	{
		rcControl.DeflateRect(lprcForegroundMargin);
	}

	int nX = rcControl.left;
	int nY = rcControl.top;
	int nDestWidth = himgFg->GetWidth();
	int nDestHeight = himgFg->GetHeight();

	if (eForegroundAlignHor == FAH_CENTER) // center
	{
		// If the dest rect less then image, stretch it.
		int nOffset = (rcControl.Width() - himgFg->GetWidth()) / 2;
		if (nOffset < 0)
		{
			nOffset = 0;

			nDestWidth = rcControl.Width();
		}
		nX += nOffset;
	}
	else if (eForegroundAlignHor == FAH_LEFT)
	{
		// Keep.
	}
	else if (eForegroundAlignHor == FAH_STRETCH)
	{
		nDestWidth = rcControl.Width();
	}
	else if (eForegroundAlignHor == FAH_RIGHT) // right
	{
		nX += (rcControl.Width() - himgFg->GetWidth());
	}
	else
	{
		ASSERT(FALSE);
	}

	if (eForegroundAlignVer == FAV_CENTER) // center
	{
		// If the dest rect less then image, stretch it.
		int nOffset = (rcControl.Height() - himgFg->GetHeight()) / 2;
		if (nOffset < 0)
		{
			nOffset = 0;

			nDestHeight = rcControl.Height();
		}
		nY += nOffset;
	}
	else if (eForegroundAlignVer == FAV_TOP)
	{
		// Keep.
	}
	else if (eForegroundAlignVer == FAV_STRETCH)
	{
		nDestHeight = rcControl.Height();
	}
	else if (eForegroundAlignVer == FAV_BOTTOM) // bottom
	{
		nY += (rcControl.Height() - himgFg->GetHeight());
	}
	else
	{
		ASSERT(FALSE);
	}

	HDC hMemDC = ::CreateCompatibleDC(hDstDC);
	HBITMAP hbmpOld = (HBITMAP)::SelectObject(hMemDC, himgFg->GetSafeHBITMAP());

	if (himgFg->IsHasAlphaChannel())
	{
		StretchAlphaBlend(hDstDC, nX, nY, nDestWidth, nDestHeight,
			hMemDC,	0, 0, himgFg->GetWidth(), himgFg->GetHeight(), btFgImgTransparent);
	}
	else
	{
		TransparentBlt2(hDstDC, nX, nY, himgFg->GetWidth(), himgFg->GetHeight(),
			hMemDC, 0, 0, himgFg->GetWidth(), himgFg->GetHeight(), crMask);
	}

	::SelectObject(hMemDC, hbmpOld);
	DeleteDC(hMemDC);

	return 0;
}

int LibUIDK::Render::IUIPartDrawForeground(HDC hDstDC,
	LPCRECT lprcDest, LPCRECT lprcForegroundMargin,
	HIUIIMAGE himgFg, LPCRECT lprcFgPart,
	FOREGROUND_ALIGN_HOR eForegroundAlignHor, FOREGROUND_ALIGN_VER eForegroundAlignVer,
	COLORREF crMask, BYTE btSourceConstantAlpha/* = 255*/)
{
	if (hDstDC == NULL || lprcDest == NULL)
	{
		return -1;
	}

	if (himgFg->GetSafeHBITMAP() == NULL)
	{
		himgFg->SafeLoadSavedImage();
	}

	if (himgFg->GetSafeHBITMAP() == NULL)
	{
		return -1;
	}

	CRect rcControl(lprcDest);
	if (lprcForegroundMargin != NULL)
	{
		rcControl.DeflateRect(lprcForegroundMargin);
	}

	CRect rcFgPart(lprcFgPart);
	if (rcFgPart.IsRectEmpty())
	{
		rcFgPart.left = 0;
		rcFgPart.top = 0;
		rcFgPart.right = himgFg->GetWidth();
		rcFgPart.bottom = himgFg->GetHeight();
	}

	int nX = rcControl.left;
	int nY = rcControl.top;
	int nDestWidth = GetDPIWidth(rcFgPart.Width());
	int nDestHeight = GetDPIHeight(rcFgPart.Height());

	if (eForegroundAlignHor == FAH_CENTER) // center
	{
		// If the dest rect less then image, stretch it.
		int nOffset = (rcControl.Width() - nDestWidth) / 2;
		if (nOffset < 0)
		{
			nOffset = 0;

			nDestWidth = rcControl.Width();
		}
		nX += nOffset;
	}
	else if (eForegroundAlignHor == FAH_LEFT)
	{
		// Keep.
	}
	else if (eForegroundAlignHor == FAH_STRETCH)
	{
		nDestWidth = rcControl.Width();
	}
	else if (eForegroundAlignHor == FAH_RIGHT) // right
	{
		nX += (rcControl.Width() - nDestWidth);
	}
	else
	{
		ASSERT(FALSE);
	}

	if (eForegroundAlignVer == FAV_CENTER) // center
	{
		// If the dest rect less then image, stretch it.
		int nOffset = (rcControl.Height() - nDestHeight) / 2;
		if (nOffset < 0)
		{
			nOffset = 0;

			nDestHeight = rcControl.Height();
		}
		nY += nOffset;
	}
	else if (eForegroundAlignVer == FAV_TOP)
	{
		// Keep.
	}
	else if (eForegroundAlignVer == FAV_STRETCH)
	{
		nDestHeight = rcControl.Height();
	}
	else if (eForegroundAlignVer == FAV_BOTTOM) // bottom
	{
		nY += (rcControl.Height() - nDestHeight);
	}
	else
	{
		ASSERT(FALSE);
	}

	IUIPartStretchBlt(hDstDC,
		CRect(nX, nY, nX + nDestWidth, nY + nDestHeight),
		himgFg, rcFgPart, IRM_STRETCH);

	return 0;
}

int LibUIDK::Render::IUIDrawImage(HDC hDstDC, LPCRECT lprcDest, HIUIIMAGE hImage,
	IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY)
{
	if (hDstDC == NULL || lprcDest == NULL)
	{
		return -1;
	}
	if (hImage == NULL)
	{
		return -2;
	}

	hImage->SafeLoadSavedImage();
	if (hImage->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	if (eIrm == IRM_9GRID)
	{
		IUINineGridBlt(hDstDC, lprcDest, hImage, pt9GridResize, l9GridRepeatX, l9GridRepeatY);
	}
	else if (eIrm == IRM_STRETCH
		|| eIrm == IRM_STRETCH_HIGH_QUALITY)
	{
		IUIAlphaStretchBlt(hDstDC, lprcDest, hImage, 255, eIrm);
	}
	else if (eIrm == IRM_CENTER)
	{
		int nImageWidth = hImage->GetWidth();
		int nImageHeight = hImage->GetHeight();

		CRect rcNewDest;
		rcNewDest.left = lprcDest->left + (RECTWIDTH(*lprcDest) - nImageWidth) / 2;
		rcNewDest.top = lprcDest->top + (RECTHEIGHT(*lprcDest) - nImageHeight) / 2;
		rcNewDest.right = rcNewDest.left + nImageWidth;
		rcNewDest.bottom = rcNewDest.top + nImageHeight;

		IUIAlphaStretchBlt(hDstDC, rcNewDest, hImage, 255, eIrm);
	}
	else if (eIrm == IRM_TILE)
	{
		IUITileBlt(hDstDC, lprcDest, hImage, 0, 0, hImage->GetWidth(), hImage->GetHeight());
	}

	return 0;
}

int LibUIDK::Render::IUIPartDrawImage(HDC hDstDC, LPCRECT lprcDest,
	HIUIIMAGE hImage,
	IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	LPCRECT lprcSrc)
{
	if (hDstDC == NULL || lprcDest == NULL)
	{
		return -1;
	}
	if (hImage == NULL)
	{
		return -2;
	}

	hImage->SafeLoadSavedImage();
	if (hImage->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	CRect rcSrc(lprcSrc);
	if (rcSrc.IsRectEmpty())
	{
		rcSrc.left = 0;
		rcSrc.top = 0;
		rcSrc.right = hImage->GetWidth();
		rcSrc.bottom = hImage->GetHeight();
	}

	if (eIrm == IRM_9GRID)
	{
		IUIPartNineGridBlt(hDstDC, lprcDest,
			hImage, pt9GridResize, l9GridRepeatX, l9GridRepeatY, lprcSrc);
	}
	else if (eIrm == IRM_STRETCH
		|| eIrm == IRM_STRETCH_HIGH_QUALITY)
	{
		IUIStretchBlt(hDstDC, lprcDest->left, lprcDest->top, RECTWIDTH(*lprcDest), RECTHEIGHT(*lprcDest),
			hImage, rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height());
	}
	else if (eIrm == IRM_CENTER)
	{
		int nImageWidth = rcSrc.Width();
		int nImageHeight = rcSrc.Height();

		CRect rcNewDest;
		rcNewDest.left = lprcDest->left + (RECTWIDTH(*lprcDest) - nImageWidth) / 2;
		rcNewDest.top = lprcDest->top + (RECTHEIGHT(*lprcDest) - nImageHeight) / 2;
		rcNewDest.right = rcNewDest.left + nImageWidth;
		rcNewDest.bottom = rcNewDest.top + nImageHeight;

		IUIStretchBlt(hDstDC, rcNewDest.left, rcNewDest.top, rcNewDest.Width(), rcNewDest.Height(),
			hImage, rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height());
	}

	return 0;
}

int LibUIDK::Render::IUIPartDrawImage(HDC hDstDC, LPCRECT lprcDest,
	HIUIIMAGE hImage,
	IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	int nPartCount, int nPartIndex)
{
	if (hDstDC == NULL || lprcDest == NULL || hImage == NULL || nPartCount <= 0)
	{
		return -1;
	}

	hImage->SafeLoadSavedImage();

	if (hImage->GetSafeHBITMAP() == NULL)
	{
		return -2;
	}

	CRect rcSrc;
	rcSrc.left = hImage->GetWidth() * nPartIndex / nPartCount;
	rcSrc.right = rcSrc.left + hImage->GetWidth() / nPartCount;
	rcSrc.bottom = hImage->GetHeight();

	return IUIPartDrawImage(hDstDC, lprcDest,
			hImage, eIrm, pt9GridResize, l9GridRepeatX, l9GridRepeatY, rcSrc);
}

