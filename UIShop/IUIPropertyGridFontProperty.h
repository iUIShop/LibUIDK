#pragma once

// CIUIPropertyGridFontProperty command target

class CIUIPropertyGridFontProperty : public CMFCPropertyGridFontProperty
{
public:
	CIUIPropertyGridFontProperty(const CString& strName, LOGFONT& lf, DWORD dwFontDialogFlags = CF_EFFECTS | CF_SCREENFONTS, 
		LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0, COLORREF color = (COLORREF)-1);
	virtual ~CIUIPropertyGridFontProperty();


	// Overrides
public:
	virtual void OnClickButton(CPoint point);
};
