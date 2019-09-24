// FontDialogEx.cpp : implementation file
//

#include "stdafx.h"
#include "UIShop.h"
#include "FontDialogEx.h"
#include <dlgs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFontDialogEx

IMPLEMENT_DYNAMIC(CFontDialogEx, CFontDialog)

CFontDialogEx::CFontDialogEx(LPLOGFONT lplfInitial, DWORD dwFlags, CDC* pdcPrinter, CWnd* pParentWnd) :
	CFontDialog(lplfInitial, dwFlags, pdcPrinter, pParentWnd)
{
	m_btOutPrecision = 3;
	m_btClipPrecision = 2;
	m_btQuality = 1;
	m_btPitchAndFamily = 34;
}


BEGIN_MESSAGE_MAP(CFontDialogEx, CFontDialog)
	//{{AFX_MSG_MAP(CFontDialogEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// protected

int CFontDialogEx::InitOutPrecision(const CRect &rect1, const CRect &rect2)
{
	m_staOutPrecision.Create(_T("OutPrecision:"), WS_VISIBLE, rect1, this, 0x0500);
	m_staOutPrecision.SetFont(GetFont());

	m_cmbOutPrecision.Create(WS_VISIBLE|WS_TABSTOP|CBS_SIMPLE|CBS_DISABLENOSCROLL|CBS_AUTOHSCROLL|CBS_HASSTRINGS, rect2, this, 0x0501);
	m_cmbOutPrecision.SetFont(GetFont());

	int nIndex = m_cmbOutPrecision.AddString(_T("0: OUT_DEFAULT_PRECIS"));
	m_cmbOutPrecision.SetItemData(nIndex, OUT_DEFAULT_PRECIS);

	nIndex = m_cmbOutPrecision.AddString(_T("1: OUT_STRING_PRECIS"));
	m_cmbOutPrecision.SetItemData(nIndex, OUT_STRING_PRECIS);

	nIndex = m_cmbOutPrecision.AddString(_T("2: OUT_CHARACTER_PRECIS"));
	m_cmbOutPrecision.SetItemData(nIndex, OUT_CHARACTER_PRECIS);

	nIndex = m_cmbOutPrecision.AddString(_T("3: OUT_STROKE_PRECIS"));
	m_cmbOutPrecision.SetItemData(nIndex, OUT_STROKE_PRECIS);

	nIndex = m_cmbOutPrecision.AddString(_T("4: OUT_TT_PRECIS"));
	m_cmbOutPrecision.SetItemData(nIndex, OUT_TT_PRECIS);

	nIndex = m_cmbOutPrecision.AddString(_T("5: OUT_DEVICE_PRECIS"));
	m_cmbOutPrecision.SetItemData(nIndex, OUT_DEVICE_PRECIS);

	nIndex = m_cmbOutPrecision.AddString(_T("6: OUT_RASTER_PRECIS"));
	m_cmbOutPrecision.SetItemData(nIndex, OUT_RASTER_PRECIS);

	nIndex = m_cmbOutPrecision.AddString(_T("7: OUT_TT_ONLY_PRECIS"));
	m_cmbOutPrecision.SetItemData(nIndex, OUT_TT_ONLY_PRECIS);

	nIndex = m_cmbOutPrecision.AddString(_T("8: OUT_OUTLINE_PRECIS"));
	m_cmbOutPrecision.SetItemData(nIndex, OUT_OUTLINE_PRECIS);

	nIndex = m_cmbOutPrecision.AddString(_T("10: OUT_PS_ONLY_PRECIS"));
	m_cmbOutPrecision.SetItemData(nIndex, OUT_PS_ONLY_PRECIS);

	SelectComboBoxString(&m_cmbOutPrecision, m_lf.lfOutPrecision);

	return 0;
}

int CFontDialogEx::InitClipPrecision(const CRect &rect1, const CRect &rect2)
{
	m_staClipPrecision.Create(_T("ClipPrecision:"), WS_VISIBLE, rect1, this, 0x0502);
	m_staClipPrecision.SetFont(GetFont());

	m_cmbClipPrecision.Create(WS_VISIBLE|CBS_SIMPLE, rect2, this, 0x0503);
	m_cmbClipPrecision.SetFont(GetFont());

	int nIndex = m_cmbClipPrecision.AddString(_T("0: CLIP_DEFAULT_PRECIS"));
	m_cmbClipPrecision.SetItemData(nIndex, CLIP_DEFAULT_PRECIS);

	nIndex = m_cmbClipPrecision.AddString(_T("1: CLIP_CHARACTER_PRECIS"));
	m_cmbClipPrecision.SetItemData(nIndex, CLIP_CHARACTER_PRECIS);

	nIndex = m_cmbClipPrecision.AddString(_T("2: CLIP_STROKE_PRECIS"));
	m_cmbClipPrecision.SetItemData(nIndex, CLIP_STROKE_PRECIS);

	nIndex = m_cmbClipPrecision.AddString(_T("0f: CLIP_MASK"));
	m_cmbClipPrecision.SetItemData(nIndex, CLIP_MASK);

	nIndex = m_cmbClipPrecision.AddString(_T("1<<4: CLIP_LH_ANGLES"));
	m_cmbClipPrecision.SetItemData(nIndex, CLIP_LH_ANGLES);

#if (_WIN32_WINNT >= _WIN32_WINNT_LONGHORN)
	nIndex = m_cmbClipPrecision.AddString(_T("4<<4: CLIP_DFA_DISABLE"));
	m_cmbClipPrecision.SetItemData(nIndex, CLIP_DFA_DISABLE);
#endif

	nIndex = m_cmbClipPrecision.AddString(_T("8<<4: CLIP_EMBEDDED"));
	m_cmbClipPrecision.SetItemData(nIndex, CLIP_EMBEDDED);

// 	nIndex = m_cmbClipPrecision.AddString(_T("CLIP_DFA_OVERRIDE"));
// 	m_cmbClipPrecision.SetItemData(nIndex, CLIP_DFA_OVERRIDE);

	SelectComboBoxString(&m_cmbClipPrecision, m_lf.lfClipPrecision);

	return 0;
}

int CFontDialogEx::InitQuality(const CRect &rect1, const CRect &rect2)
{
	m_staQuality.Create(_T("Quality:"), WS_VISIBLE, rect1, this, 0x0504);
	m_staQuality.SetFont(GetFont());

	m_cmbQuality.Create(WS_VISIBLE|CBS_SIMPLE, rect2, this, 0x0505);
	m_cmbQuality.SetFont(GetFont());

	int nIndex = 0;

	nIndex = m_cmbQuality.AddString(_T("0: DEFAULT_QUALITY"));
	m_cmbQuality.SetItemData(nIndex, DEFAULT_QUALITY);

	nIndex = m_cmbQuality.AddString(_T("1: DRAFT_QUALITY"));
	m_cmbQuality.SetItemData(nIndex, DRAFT_QUALITY);

	nIndex = m_cmbQuality.AddString(_T("2: PROOF_QUALITY"));
	m_cmbQuality.SetItemData(nIndex, PROOF_QUALITY);

	nIndex = m_cmbQuality.AddString(_T("3: NONANTIALIASED_QUALITY"));
	m_cmbQuality.SetItemData(nIndex, NONANTIALIASED_QUALITY);

	nIndex = m_cmbQuality.AddString(_T("4: ANTIALIASED_QUALITY"));
	m_cmbQuality.SetItemData(nIndex, ANTIALIASED_QUALITY);

	nIndex = m_cmbQuality.AddString(_T("5: CLEARTYPE_QUALITY"));
	m_cmbQuality.SetItemData(nIndex, CLEARTYPE_QUALITY);

	SelectComboBoxString(&m_cmbQuality, m_lf.lfQuality);

	return 0;
}

int CFontDialogEx::InitPitchAndFamily(const CRect &rect1, const CRect &rect2)
{
	m_staPitchAndFamily.Create(_T("PitchAndFamily:"), WS_VISIBLE, rect1, this, 0x0506);
	m_staPitchAndFamily.SetFont(GetFont());

	m_cmbPitchAndFamily.Create(WS_VISIBLE|CBS_SIMPLE, rect2, this, 0x0507);
	m_cmbPitchAndFamily.SetFont(GetFont());

	int nIndex = 0;

// 	nIndex = m_cmbPitchAndFamily.AddString(_T(""));
// 	m_cmbPitchAndFamily.SetItemData(nIndex, );

	SelectComboBoxString(&m_cmbPitchAndFamily, m_lf.lfPitchAndFamily);

	return 0;
}

BOOL CFontDialogEx::OnInitDialog()
{
	CFontDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	// Hide some control.
	CWnd *pWnd = NULL;
	CRect rcWin;
	GetWindowRect(rcWin);
	rcWin.DeflateRect(0, 0, -100, -120);
	MoveWindow(rcWin);

	// Hide color property.
	pWnd = GetDlgItem(stc4); // the "Look In:" static
	if (pWnd->GetSafeHwnd() != NULL)
		pWnd->ShowWindow(SW_HIDE);

	pWnd = GetDlgItem(cmb4); // the "Look In:" static
	if (pWnd->GetSafeHwnd() != NULL)
	{
		pWnd->GetWindowRect(rcWin);
		ScreenToClient(rcWin);
		pWnd->ShowWindow(SW_HIDE);
	}

	// Position
	pWnd = GetDlgItem(cmb1);
	if (pWnd->GetSafeHwnd() != NULL)
	{
		pWnd->GetWindowRect(rcWin);
		ScreenToClient(rcWin);
	}

	int nTop = 290;
	int nTop2 = 310;
	int nWidht = 72;
	CRect rect;
	CRect rect2;
	CFont *pFont = GetFont();

	//////////////////////////////////////////////////////////////////////////
	// Create OutPrecision properties
	rect.top = nTop;
	rect.bottom = rect.top + 20;
	rect.left = rcWin.left;
	rect.right = rect.left + nWidht *2 ;

	rect2.top = nTop2;
	rect2.bottom = rect2.top + 100;
	rect2.left = rcWin.left;
	rect2.right = rect2.left + nWidht * 2;

	InitOutPrecision(rect, rect2);

	// Create ClipPrecision properties
	rect.left = rect.right + 10;
	rect.right = rect.left + nWidht * 2;

	rect2.left = rect2.right + 10;
	rect2.right = rect2.left + nWidht * 2;

	InitClipPrecision(rect, rect2);

	// Create Quality properties
	rect.left = rect.right + 10;
	rect.right = rect.left + nWidht * 2;

	rect2.left = rect2.right + 10;
	rect2.right = rect2.left + nWidht * 2;

	InitQuality(rect, rect2);

	// Create PitchAndFamily properties
	rect.left = rect.right + 10;
	rect.right = rect.left + nWidht;

	rect2.left = rect2.right + 10;
	rect2.right = rect2.left + nWidht;

//	InitPitchAndFamily(rect, rect2);

	nTop = rect2.bottom + 10;
	//

	pWnd = GetDlgItem(stc6);
	if (pWnd->GetSafeHwnd() != NULL)
	{
		pWnd->GetWindowRect(rcWin);
		ScreenToClient(rcWin);
		rect = rcWin;
		rect.top = nTop;
		rect.bottom = rect.top + rcWin.Height();
		pWnd->MoveWindow(rect);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

INT_PTR CFontDialogEx::DoModal()
{
	ASSERT_VALID(this);
	ASSERT(m_cf.Flags & CF_ENABLEHOOK);
	ASSERT(m_cf.lpfnHook != NULL); // can still be a user hook

	m_cf.hwndOwner = PreModal();

#if (_MSC_VER >= 1910)	// VC2017
	INT_PTR nResult = ChooseFont(&m_cf);
#else
	INT_PTR nResult = ::AfxCtxChooseFont(&m_cf);
#endif
	PostModal();

	if (nResult == IDOK)
	{
		// copy logical font from user's initialization buffer (if needed)
		m_cf.lpLogFont->lfOutPrecision = m_btOutPrecision;
		m_cf.lpLogFont->lfClipPrecision = m_btClipPrecision;
		m_cf.lpLogFont->lfQuality = m_btQuality;
		m_cf.lpLogFont->lfPitchAndFamily = m_btPitchAndFamily;
		Checked::memcpy_s(&m_lf, sizeof(LOGFONT), m_cf.lpLogFont, sizeof(LOGFONT));
		return IDOK;
	}
	return nResult ? nResult : IDCANCEL;
}

void CFontDialogEx::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	int nSel = m_cmbOutPrecision.GetCurSel();
	if (nSel >= 0)
		m_btOutPrecision = (BYTE)m_cmbOutPrecision.GetItemData(nSel);

	nSel = m_cmbClipPrecision.GetCurSel();
	if (nSel >= 0)
		m_btClipPrecision = (BYTE)m_cmbClipPrecision.GetItemData(nSel);

	nSel = m_cmbQuality.GetCurSel();
	if (nSel >= 0)
		m_btQuality = (BYTE)m_cmbQuality.GetItemData(nSel);

	CFontDialog::OnOK();
}
