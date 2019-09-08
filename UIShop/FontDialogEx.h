#if !defined(AFX_FONTDIALOGEX_H__3748A277_70D6_40D1_A5E5_C6CC23C93546__INCLUDED_)
#define AFX_FONTDIALOGEX_H__3748A277_70D6_40D1_A5E5_C6CC23C93546__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FontDialogEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFontDialogEx dialog

class CFontDialogEx : public CFontDialog
{
	DECLARE_DYNAMIC(CFontDialogEx)

public:
	CFontDialogEx(LPLOGFONT lplfInitial = NULL,
		DWORD dwFlags = CF_EFFECTS | CF_SCREENFONTS,
		CDC* pdcPrinter = NULL,
		CWnd* pParentWnd = NULL);
#ifndef _AFX_NO_RICHEDIT_SUPPORT
	CFontDialogEx(const CHARFORMAT& charformat,
		DWORD dwFlags = CF_SCREENFONTS,
		CDC* pdcPrinter = NULL,
		CWnd* pParentWnd = NULL);
#endif

protected:
	//{{AFX_MSG(CFontDialogEx)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal();
	virtual void OnOK();

protected:
	int InitOutPrecision(const CRect &rect1, const CRect &rect2);
	int InitClipPrecision(const CRect &rect1, const CRect &rect2);
	int InitQuality(const CRect &rect1, const CRect &rect2);
	int InitPitchAndFamily(const CRect &rect1, const CRect &rect2);

protected:
	CStatic m_staOutPrecision;
	CComboBox m_cmbOutPrecision;
	BYTE m_btOutPrecision;

	CStatic m_staClipPrecision;
	CComboBox m_cmbClipPrecision;
	BYTE m_btClipPrecision;

	CStatic m_staQuality;
	CComboBox m_cmbQuality;
	BYTE m_btQuality;

	CStatic m_staPitchAndFamily;
	CComboBox m_cmbPitchAndFamily;
	BYTE m_btPitchAndFamily;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTDIALOGEX_H__3748A277_70D6_40D1_A5E5_C6CC23C93546__INCLUDED_)
