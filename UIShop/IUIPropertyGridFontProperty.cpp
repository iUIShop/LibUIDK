// IUIPropertyGridFontProperty.cpp : implementation file
//

#include "stdafx.h"
#include "UIShop.h"
#include "IUIPropertyGridFontProperty.h"
#include "FontDialogEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CIUIPropertyGridFontProperty

CIUIPropertyGridFontProperty::CIUIPropertyGridFontProperty(const CString& strName, LOGFONT& lf, DWORD dwFontDialogFlags, LPCTSTR lpszDescr, DWORD_PTR dwData, COLORREF color)
: CMFCPropertyGridFontProperty(strName, lf, dwFontDialogFlags, lpszDescr, dwData, color)
{
}

CIUIPropertyGridFontProperty::~CIUIPropertyGridFontProperty()
{
}


void CIUIPropertyGridFontProperty::OnClickButton(CPoint /*point*/)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	LOGFONT lfPrev = m_lf;
	COLORREF nColorPrev = m_Color;

	m_bButtonIsDown = TRUE;
	Redraw();

#pragma warning(disable : 4244)

	CFontDialogEx dlg(&m_lf, m_dwFontDialogFlags, NULL, m_pWndList);

#pragma warning(default : 4244)

	if (m_Color != (COLORREF)-1)
	{
		dlg.m_cf.rgbColors = m_Color;
	}

	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCurrentFont(&m_lf);
		m_Color = dlg.GetColor();

		if (memcmp(&lfPrev, &m_lf, sizeof(lfPrev) - sizeof(lfPrev.lfFaceName)) || _tcscmp( lfPrev.lfFaceName, m_lf.lfFaceName) || nColorPrev != m_Color)
		{
			m_pWndList->OnPropertyChanged(this);
		}

		if (m_pWndInPlace != NULL)
		{
			m_pWndInPlace->SetWindowText(FormatProperty());
		}
		else
		{
			m_varValue = (LPCTSTR) FormatProperty();
		}
	}

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetFocus();
	}
	else
	{
		m_pWndList->SetFocus();
	}

	m_bButtonIsDown = FALSE;
	Redraw();
}
