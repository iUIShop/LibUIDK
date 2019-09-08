// SkinRichEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#if(_MSC_VER <= 1200) // VC6.0
#include <ObjIdl.h>
#endif
#include <RichOle.h>
#include <atlbase.h>
#include <atlwin.h>
#pragma comment (lib, "Imm32.lib")
#include "TextHost.h"
#include <strsafe.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


LRESULT RichEditWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

const LONG g_cInitTextMax = (32 * 1024) - 1;

EXTERN_C const IID IID_ITextServices =   // 8d33f740-cf58-11ce-a89d-00aa006cadc5
{
	0x8d33f740,
	0xcf58,
	0x11ce,
	{0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
};

EXTERN_C const IID IID_ITextHost =   /* c5bdd8d0-d26e-11ce-a89e-00aa006cadc5 */
{
	0xc5bdd8d0,
	0xd26e,
	0x11ce,
	{0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
};

HRESULT InitDefaultCharFormat(CWLRichEditCtrl *re, CHARFORMAT2W *pcf, HFONT hfont)
{
	memset(pcf, 0, sizeof(CHARFORMAT2W));
	LOGFONT lf = {0};

	if (hfont == NULL)
	{
		hfont = (HFONT)::SendMessage(GetDesktopWindow(), WM_GETFONT, 0, 0);
	}
	::GetObject(hfont, sizeof(LOGFONT), &lf);

	DWORD dwColor = 0xFFFF0000;
	pcf->cbSize = sizeof(CHARFORMAT2W);
	pcf->crTextColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
	LONG yPixPerInch = GetDeviceCaps(GetDC(GetDesktopWindow()), LOGPIXELSY);
	pcf->yHeight = -lf.lfHeight * 1440 / yPixPerInch;
	pcf->yOffset = 0;
	pcf->dwEffects = 0;
	pcf->dwMask = CFM_SIZE | CFM_OFFSET | CFM_FACE | CFM_CHARSET | CFM_COLOR | CFM_BOLD | CFM_ITALIC |
		CFM_UNDERLINE;

	if (lf.lfWeight >= FW_BOLD)
	{
		pcf->dwEffects |= CFE_BOLD;
	}

	if (lf.lfItalic)
	{
		pcf->dwEffects |= CFE_ITALIC;
	}

	if (lf.lfUnderline)
	{
		pcf->dwEffects |= CFE_UNDERLINE;
	}

	pcf->bCharSet = lf.lfCharSet;
	pcf->bPitchAndFamily = lf.lfPitchAndFamily;
	USES_CONVERSION;
	StringCchCopyW(pcf->szFaceName, LF_FACESIZE, T2W(lf.lfFaceName));

	return S_OK;
}

HRESULT InitDefaultParaFormat(CWLRichEditCtrl *re, PARAFORMAT2 *ppf)
{
	memset(ppf, 0, sizeof(PARAFORMAT2));
	ppf->cbSize = sizeof(PARAFORMAT2);
	ppf->dwMask = PFM_ALL;
	ppf->wAlignment = PFA_LEFT;
	ppf->cTabCount = 1;
	ppf->rgxTabs[0] = lDefaultTab;

	return S_OK;
}

CTextHost::CTextHost()
	: m_re(NULL)
{
	::ZeroMemory(&cRefs, sizeof(CTextHost) - offsetof(CTextHost, cRefs));
	m_bZoomEnable = true;
	m_cchTextMost = g_cInitTextMax;
	m_laccelpos = -1;
}

CTextHost::~CTextHost()
{
	m_pserv->OnTxInPlaceDeactivate();
	m_pserv->Release();
}

////////////////////// Create/Init/Destruct Commands ///////////////////////

int ConvertCHARFORMAT2(__in const CHARFORMAT2A *pSrc, __out CHARFORMAT2W *pDest)
{
	if (pSrc == NULL || pDest == NULL)
	{
		return -1;
	}

	// copy base class _charformat
	pDest->cbSize = sizeof(CHARFORMAT2W);
	pDest->dwMask = pDest->dwMask;
	pDest->dwEffects = pDest->dwEffects;
	pDest->yHeight = pDest->yHeight;
	pDest->yOffset = pDest->yOffset;
	pDest->crTextColor = pDest->crTextColor;
	pDest->bCharSet = pDest->bCharSet;
	pDest->bPitchAndFamily = pDest->bPitchAndFamily;

	USES_CONVERSION;
	StringCchCopyW(pDest->szFaceName, LF_FACESIZE, A2W(pSrc->szFaceName));

	// copy CHARFORMAT2
	pDest->wWeight = pSrc->wWeight;			// Font weight (LOGFONT value)
	pDest->sSpacing = pSrc->sSpacing;			// Amount to space between letters
	pDest->crBackColor = pSrc->crBackColor;		// Background color
	pDest->lcid = pSrc->lcid;				// Locale ID
#if (_RICHEDIT_VER >= 0x0500)
	pDest->dwReserved = pSrc->dwReserved;			// Name up to 5.0
	pSrc->dwCookie = pSrc->dwCoolie;	 		// Client cookie opaque to RichEdit
#else
	pDest->dwReserved = pSrc->dwReserved;			// Name up to 5.0
#endif
	pDest->sStyle = pSrc->sStyle; 			// Style handle
	pDest->wKerning = pSrc->wKerning;			// Twip size above which to kern char pair
	pDest->bUnderlineType = pSrc->bUnderlineType; 	// Underline type
	pDest->bAnimation = pSrc->bAnimation; 		// Animated text like marching ants
	pDest->bRevAuthor = pSrc->bRevAuthor; 		// Revision author index
#if (_RICHEDIT_VER >= 0x0800)
	pDest->bUnderlineColor = pSrc->bUnderlineColor;	// Underline color
#endif

	return 0;
}

BOOL CTextHost::Init(CWLRichEditCtrl *re, DWORD dwStyle)
{
	m_re = re;
	// Initialize Reference count
	cRefs = 1;

	// Create and cache CHARFORMAT for this control
	if (FAILED(InitDefaultCharFormat(re, &m_cf, NULL)))
	{
		return FALSE;
	}

	// Create and cache PARAFORMAT for this control
	if (FAILED(InitDefaultParaFormat(re, &m_pf)))
	{
		return FALSE;
	}

	// 下面设置一些属性。 部分属性，应该放到IM控件中。 而不是这里。
	// 不设置它们，会影响内容的显示。

	// edit controls are rich by default
	m_fRich = true;

	m_cchTextMost = re->GetLimitText();

	m_dwStyle = dwStyle;

	if (!(m_dwStyle & (ES_AUTOHSCROLL | WS_HSCROLL)))
	{
		m_fWordWrap = TRUE; // 不设置，不能设置自己消息的左缩进
	}

	if (!(m_dwStyle & ES_LEFT))
	{
		if (m_dwStyle & ES_CENTER)
		{
			m_pf.wAlignment = PFA_CENTER;
		}
		else if (m_dwStyle & ES_RIGHT)
		{
			m_pf.wAlignment = PFA_RIGHT;
		}
	}

	m_fInplaceActive = TRUE;

	PCreateTextServices CreateTextServices = NULL;
	//msftedit.dll是最新版本的richedit支持多国语言输入（如泰语等），而Riched20.dll是不支持的（比如泰语等）这个可以测试一下。
	//这里使用老的Riched20是因为在win8下面插入链接不显示
	//这个问题暂时先这样处理，以后看看能不能找到win8下为什么msftedit.dll不能插入链接的根本原因。
	//HMODULE hmod = LoadLibrary(_T("msftedit.dll"));
	HMODULE hmod = LoadLibrary(_T("Riched20.dll"));

	if (hmod)
	{
		CreateTextServices = (PCreateTextServices)GetProcAddress(hmod, "CreateTextServices");
	}

	CComPtr<IUnknown> pUnk = NULL;
	if (CreateTextServices != NULL)
	{
		HRESULT hr = CreateTextServices(NULL, this, &pUnk);
	}

	HRESULT hr = pUnk->QueryInterface(IID_ITextServices, (void **)&m_pserv);

	if (FAILED(hr))
	{
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////  IUnknown ////////////////////////////////


HRESULT CTextHost::QueryInterface(REFIID riid, void **ppvObject)
{
	HRESULT hr = E_NOINTERFACE;
	*ppvObject = NULL;

	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITextHost))
	{
		AddRef();
		*ppvObject = (ITextHost *) this;
		hr = S_OK;
	}

	return hr;
}

ULONG CTextHost::AddRef(void)
{
	return ++cRefs;
}

ULONG CTextHost::Release(void)
{
	ULONG c_Refs = --cRefs;

	if (c_Refs == 0)
	{
		CTextHost *p = this;
		delete p;
	}

	return c_Refs;
}

/////////////////////////////////  Far East Support  //////////////////////////////////////

HIMC CTextHost::TxImmGetContext(void)
{
	return NULL;
}

void CTextHost::TxImmReleaseContext(HIMC himc)
{
	//::ImmReleaseContext( hwnd, himc );
}

//////////////////////////// ITextHost Interface  ////////////////////////////

HDC CTextHost::TxGetDC()
{
	return m_re->GetDC();
}

int CTextHost::TxReleaseDC(HDC hdc)
{
	return 1;
}

BOOL CTextHost::TxShowScrollBar(INT fnBar, BOOL fShow)
{
	//     CScrollBarUI *pVerticalScrollBar = m_re->GetVerticalScrollBar();
	//     CScrollBarUI *pHorizontalScrollBar = m_re->GetHorizontalScrollBar();
	//
	//     if ( fnBar == SB_VERT && pVerticalScrollBar ) {
	//         pVerticalScrollBar->SetVisible(fShow == TRUE);
	//     } else if ( fnBar == SB_HORZ && pHorizontalScrollBar ) {
	//         pHorizontalScrollBar->SetVisible(fShow == TRUE);
	//     } else if ( fnBar == SB_BOTH ) {
	//         if ( pVerticalScrollBar ) {
	//             pVerticalScrollBar->SetVisible(fShow == TRUE);
	//         }
	//
	//         if ( pHorizontalScrollBar ) {
	//             pHorizontalScrollBar->SetVisible(fShow == TRUE);
	//         }
	//     }

	return TRUE;
}

BOOL CTextHost::TxEnableScrollBar(INT fuSBFlags, INT fuArrowflags)
{
	//     if ( fuSBFlags == SB_VERT ) {
	//         m_re->EnableScrollBar(true, m_re->GetHorizontalScrollBar() != NULL);
	//         m_re->GetVerticalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
	//     } else if ( fuSBFlags == SB_HORZ ) {
	//         m_re->EnableScrollBar(m_re->GetVerticalScrollBar() != NULL, true);
	//         m_re->GetHorizontalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
	//     } else if ( fuSBFlags == SB_BOTH ) {
	//         m_re->EnableScrollBar(true, true);
	//         m_re->GetVerticalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
	//         m_re->GetHorizontalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
	//     }

	return TRUE;
}

BOOL CTextHost::TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw)
{
	//     CScrollBarUI *pVerticalScrollBar = m_re->GetVerticalScrollBar();
	//     CScrollBarUI *pHorizontalScrollBar = m_re->GetHorizontalScrollBar();
	//
	//     if ( fnBar == SB_VERT && pVerticalScrollBar ) {
	//         if ( nMaxPos - nMinPos - rcClient.bottom + rcClient.top <= 0 ) {
	//             pVerticalScrollBar->SetVisible(false);
	//         } else {
	//             pVerticalScrollBar->SetVisible(true);
	//             pVerticalScrollBar->SetScrollRange(nMaxPos - nMinPos - rcClient.bottom + rcClient.top);
	//         }
	//     } else if ( fnBar == SB_HORZ && pHorizontalScrollBar ) {
	//         if ( nMaxPos - nMinPos - rcClient.right + rcClient.left <= 0 ) {
	//             pHorizontalScrollBar->SetVisible(false);
	//         } else {
	//             pHorizontalScrollBar->SetVisible(true);
	//             pHorizontalScrollBar->SetScrollRange(nMaxPos - nMinPos - rcClient.right + rcClient.left);
	//         }
	//     }

	return TRUE;
}

BOOL CTextHost::TxSetScrollPos(INT fnBar, INT nPos, BOOL fRedraw)
{
	//     CScrollBarUI *pVerticalScrollBar = m_re->GetVerticalScrollBar();
	//     CScrollBarUI *pHorizontalScrollBar = m_re->GetHorizontalScrollBar();
	//
	//     if ( fnBar == SB_VERT && pVerticalScrollBar ) {
	//         pVerticalScrollBar->SetScrollPos(nPos);
	//     } else if ( fnBar == SB_HORZ && pHorizontalScrollBar ) {
	//         pHorizontalScrollBar->SetScrollPos(nPos);
	//     }

	return TRUE;
}

void CTextHost::TxInvalidateRect(LPCRECT prc, BOOL fMode)
{
	if (prc == NULL)
	{
		m_re->GetParent()->InvalidateRect(&m_rcClient);
		return;
	}

	RECT rc = *prc;
	m_re->GetParent()->InvalidateRect(&rc);
}

void CTextHost::TxViewChange(BOOL fUpdate)
{
	if (fUpdate)
	{
		m_re->GetParent()->Invalidate();
	}
}

BOOL CTextHost::TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight)
{
	m_iCaretWidth = xWidth;
	m_iCaretHeight = yHeight;
	return ::CreateCaret(m_re->GetParent()->GetSafeHwnd(), hbmp, xWidth, yHeight);
}

BOOL CTextHost::TxShowCaret(BOOL fShow)
{
	m_fShowCaret = fShow;

	if (fShow/* && m_re->IsRedraw()*/)
	{
		return ::ShowCaret(m_re->GetParent()->GetSafeHwnd());
	}
	else
	{
		return ::HideCaret(m_re->GetParent()->GetSafeHwnd());
	}
}

BOOL CTextHost::TxSetCaretPos(INT x, INT y)
{
	POINT ptCaret = { 0 };
	::GetCaretPos(&ptCaret);
	RECT rcCaret = { ptCaret.x, ptCaret.y, ptCaret.x + m_iCaretLastWidth, ptCaret.y + m_iCaretLastHeight };

	//     if ( m_re->GetManager()->IsLayered() ) {
	//         m_re->GetManager()->Invalidate(rcCaret);
	//     } else if ( fNeedFreshCaret == TRUE ) {
	//         m_re->GetManager()->Invalidate(rcCaret);
	//         fNeedFreshCaret = FALSE;
	//     }

	rcCaret.left = x;
	rcCaret.top = y;
	rcCaret.right = x + m_iCaretWidth;
	rcCaret.bottom = y + m_iCaretHeight;

	//     if ( m_re->GetManager()->IsLayered() ) {
	//         m_re->GetManager()->Invalidate(rcCaret);
	//     }

	m_iCaretLastWidth = m_iCaretWidth;
	m_iCaretLastHeight = m_iCaretHeight;
	return ::SetCaretPos(x, y);
}

BOOL CTextHost::TxSetTimer(UINT idTimer, UINT uTimeout)
{
	m_fTimer = TRUE;
	return m_re->GetParent()->SetTimer(idTimer, uTimeout, NULL) == TRUE;
}

void CTextHost::TxKillTimer(UINT idTimer)
{
	m_re->GetParent()->KillTimer(idTimer);
	m_fTimer = FALSE;
}

void CTextHost::TxScrollWindowEx(INT dx, INT dy, LPCRECT lprcScroll,    LPCRECT lprcClip,
	HRGN hrgnUpdate, LPRECT lprcUpdate,    UINT fuScroll)
{
	return;
}

void CTextHost::TxSetCapture(BOOL fCapture)
{
	if (fCapture)
	{
		m_re->GetParent()->SetCapture();
	}
	else
	{
		::ReleaseCapture();
	}

	m_fCaptured = fCapture;
}

void CTextHost::TxSetFocus()
{
	//m_re->SetFocus();
}

void CTextHost::TxSetCursor(HCURSOR hcur,    BOOL fText)
{
	::SetCursor(hcur);
}

BOOL CTextHost::TxScreenToClient(LPPOINT lppt)
{
	return ::ScreenToClient(m_re->GetParent()->GetSafeHwnd(), lppt);
}

BOOL CTextHost::TxClientToScreen(LPPOINT lppt)
{
	return ::ClientToScreen(m_re->GetParent()->GetSafeHwnd(), lppt);
}

HRESULT CTextHost::TxActivate(LONG *plOldState)
{
	return S_OK;
}

HRESULT CTextHost::TxDeactivate(LONG lNewState)
{
	return S_OK;
}

HRESULT CTextHost::TxGetClientRect(LPRECT prc)
{
	*prc = m_rcClient;
	return NOERROR;
}

HRESULT CTextHost::TxGetViewInset(LPRECT prc)
{
	prc->left = prc->right = prc->top = prc->bottom = 0;
	return NOERROR;
}

HRESULT CTextHost::TxGetCharFormat(const CHARFORMATW **ppCF)
{
	*ppCF = &m_cf;
	return NOERROR;
}

HRESULT CTextHost::TxGetParaFormat(const PARAFORMAT **ppPF)
{
	*ppPF = &m_pf;
	return NOERROR;
}

// 调用pTextServices->TxDraw输出文本的时候，会调用CTextHost::TxGetSysColor，用来填充背景
COLORREF CTextHost::TxGetSysColor(int nIndex)
{
	return ::GetSysColor(nIndex);
}

HRESULT CTextHost::TxGetBackStyle(TXTBACKSTYLE *pstyle)
{
	*pstyle = !m_fTransparent ? TXTBACK_OPAQUE : TXTBACK_TRANSPARENT;
	return NOERROR;
}

HRESULT CTextHost::TxGetMaxLength(DWORD *pLength)
{
	*pLength = m_cchTextMost;
	return NOERROR;
}

HRESULT CTextHost::TxGetScrollBars(DWORD *pdwScrollBar)
{
	*pdwScrollBar =  m_dwStyle & (WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL |
			ES_AUTOHSCROLL | ES_DISABLENOSCROLL);

	return NOERROR;
}

HRESULT CTextHost::TxGetPasswordChar(TCHAR *pch)
{
#ifdef _UNICODE
	*pch = m_chPasswordChar;
#else
	::WideCharToMultiByte(CP_ACP, 0, &m_chPasswordChar, 1, pch, 1, NULL, NULL) ;
#endif
	return NOERROR;
}

HRESULT CTextHost::TxGetAcceleratorPos(LONG *pcp)
{
	*pcp = m_laccelpos;
	return S_OK;
}

HRESULT CTextHost::OnTxCharFormatChange(const CHARFORMATW *pcf)
{
	return S_OK;
}

HRESULT CTextHost::OnTxParaFormatChange(const PARAFORMAT *ppf)
{
	return S_OK;
}

HRESULT CTextHost::TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits)
{
	DWORD dwProperties = 0;

	if (m_fRich)
	{
		dwProperties = TXTBIT_RICHTEXT;
	}

	if (m_dwStyle & ES_MULTILINE)
	{
		dwProperties |= TXTBIT_MULTILINE;
	}

	if (m_dwStyle & ES_READONLY)
	{
		dwProperties |= TXTBIT_READONLY;
	}

	if (m_dwStyle & ES_PASSWORD)
	{
		dwProperties |= TXTBIT_USEPASSWORD;
	}

	if (!(m_dwStyle & ES_NOHIDESEL))
	{
		dwProperties |= TXTBIT_HIDESELECTION;
	}

	if (m_fEnableAutoWordSel)
	{
		dwProperties |= TXTBIT_AUTOWORDSEL;
	}

	if (m_fWordWrap)
	{
		dwProperties |= TXTBIT_WORDWRAP;
	}

	if (m_fAllowBeep)
	{
		dwProperties |= TXTBIT_ALLOWBEEP;
	}

	if (m_fSaveSelection)
	{
		dwProperties |= TXTBIT_SAVESELECTION;
	}

	*pdwBits = dwProperties & dwMask;
	return NOERROR;
}


HRESULT CTextHost::TxNotify(DWORD iNotify, void *pv)
{
	if (iNotify == WM_ISNEEDUPDATA)
	{
		return m_re->m_bNeedUpdate;
	}

	return S_OK;
}

HRESULT CTextHost::TxGetExtent(LPSIZEL lpExtent)
{
	*lpExtent = m_sizelExtent;
	return S_OK;
}

HRESULT CTextHost::TxGetSelectionBarWidth(LONG *plSelBarWidth)
{
	*plSelBarWidth = m_lSelBarWidth;
	return S_OK;
}

void CTextHost::SetWordWrap(BOOL fWordWrap)
{
	fWordWrap = fWordWrap;
	m_pserv->OnTxPropertyBitsChange(TXTBIT_WORDWRAP, fWordWrap ? TXTBIT_WORDWRAP : 0);
}

BOOL CTextHost::GetReadOnly()
{
	return (m_dwStyle & ES_READONLY) != 0;
}

void CTextHost::SetReadOnly(BOOL fReadOnly)
{
	if (fReadOnly)
	{
		m_dwStyle |= ES_READONLY;
	}
	else
	{
		m_dwStyle &= ~ES_READONLY;
	}

	m_pserv->OnTxPropertyBitsChange(TXTBIT_READONLY, fReadOnly ? TXTBIT_READONLY : 0);
}

void CTextHost::SetFont(HFONT hFont)
{
	if (hFont == NULL)
	{
		return;
	}

	LOGFONT lf;
	::GetObject(hFont, sizeof(LOGFONT), &lf);
	m_cf.yHeight = GetYTwipsFromPixel(-lf.lfHeight);

	if (lf.lfWeight >= FW_BOLD)
	{
		m_cf.dwEffects |= CFE_BOLD;
	}
	else
	{
		m_cf.dwEffects &= ~CFE_BOLD;
	}

	if (lf.lfItalic)
	{
		m_cf.dwEffects |= CFE_ITALIC;
	}
	else
	{
		m_cf.dwEffects &= ~CFE_ITALIC;
	}

	if (lf.lfUnderline)
	{
		m_cf.dwEffects |= CFE_UNDERLINE;
	}
	else
	{
		m_cf.dwEffects &= ~CFE_UNDERLINE;
	}

	m_cf.bCharSet = lf.lfCharSet;
	m_cf.bPitchAndFamily = lf.lfPitchAndFamily;

	USES_CONVERSION;
	StringCchCopyW(m_cf.szFaceName, LF_FACESIZE, T2W(lf.lfFaceName));

	m_pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, TXTBIT_CHARFORMATCHANGE);
}

void CTextHost::SetColor(DWORD dwColor)
{
	m_cf.crTextColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
	m_pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, TXTBIT_CHARFORMATCHANGE);
}

SIZEL *CTextHost::GetExtent()
{
	return &m_sizelExtent;
}

void CTextHost::SetExtent(SIZEL *psizelExtent)
{
	m_sizelExtent = *psizelExtent;
	m_pserv->OnTxPropertyBitsChange(TXTBIT_EXTENTCHANGE, TXTBIT_EXTENTCHANGE);
}

void CTextHost::LimitText(LONG nChars)
{
	m_cchTextMost = nChars;

	if (m_cchTextMost <= 0)
	{
		m_cchTextMost = g_cInitTextMax;
	}

	m_pserv->OnTxPropertyBitsChange(TXTBIT_MAXLENGTHCHANGE, TXTBIT_MAXLENGTHCHANGE);
}

BOOL CTextHost::IsCaptured()
{
	return m_fCaptured;
}

BOOL CTextHost::IsShowCaret()
{
	return m_fShowCaret;
}

void CTextHost::NeedFreshCaret()
{
	m_fNeedFreshCaret = TRUE;
}

INT CTextHost::GetCaretWidth()
{
	return m_iCaretWidth;
}

INT CTextHost::GetCaretHeight()
{
	return m_iCaretHeight;
}

BOOL CTextHost::GetAllowBeep()
{
	return m_fAllowBeep;
}

void CTextHost::SetAllowBeep(BOOL fAllowBeep)
{
	fAllowBeep = fAllowBeep;

	m_pserv->OnTxPropertyBitsChange(TXTBIT_ALLOWBEEP,
		fAllowBeep ? TXTBIT_ALLOWBEEP : 0);
}

WORD CTextHost::GetDefaultAlign()
{
	return m_pf.wAlignment;
}

void CTextHost::SetDefaultAlign(WORD wNewAlign)
{
	m_pf.wAlignment = wNewAlign;

	// Notify control of property change
	m_pserv->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
}

BOOL CTextHost::GetRichTextFlag()
{
	return m_fRich;
}

void CTextHost::SetRichTextFlag(BOOL fNew)
{
	m_fRich = fNew;

	m_pserv->OnTxPropertyBitsChange(TXTBIT_RICHTEXT,
		fNew ? TXTBIT_RICHTEXT : 0);
}

LONG CTextHost::GetDefaultLeftIndent()
{
	return m_pf.dxOffset;
}

void CTextHost::SetDefaultLeftIndent(LONG lNewIndent)
{
	m_pf.dxOffset = lNewIndent;

	m_pserv->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
}

void CTextHost::SetClientRect(RECT *prc)
{
	if (prc->left >= prc->right
		|| prc->top >= prc->bottom)
	{
		//解决最小化时，插入文字后，滚动条不在最低端问题
		return;
	}

	BOOL bScrollShowOrHide =
		FALSE; //解决当将文字插入到最后时，文字显示不全问题(插入表情也是一样的)和当插入文字到滚动条刚显示出来的时候没有在最底端的问题。（选择微软雅黑10.5号字体这个BUG很容易出现）

	if (m_bZoomEnable)
	{
		if (prc->left != m_rcClient.left || prc->top != m_rcClient.top || prc->bottom != m_rcClient.bottom
			|| prc->right != m_rcClient.right)
		{
			bScrollShowOrHide = TRUE;
		}
	}

	m_rcClient = *prc;

	m_sizelExtent.cx = GetXHimetricFromPixel(m_rcClient.right - m_rcClient.left);
	m_sizelExtent.cy = GetYHimetricFromPixel(m_rcClient.bottom - m_rcClient.top);

	if (bScrollShowOrHide)
	{
		m_pserv->OnTxPropertyBitsChange(TXTBIT_VIEWINSETCHANGE, TXTBIT_VIEWINSETCHANGE);
	}

	if (bScrollShowOrHide)
	{
		if (m_re/* && !m_re->IsFocused()*/)
		{
			m_pserv->OnTxInPlaceActivate(NULL);
			m_pserv->TxSendMessage(WM_SETFOCUS, 0, 0, 0);
			m_pserv->OnTxInPlaceActivate(NULL);
			m_pserv->TxSendMessage(WM_KILLFOCUS, 0, 0, 0);
		}
	}
}

BOOL CTextHost::SetSaveSelection(BOOL f_SaveSelection)
{
	BOOL fResult = f_SaveSelection;

	m_fSaveSelection = f_SaveSelection;

	// notify text services of property change
	m_pserv->OnTxPropertyBitsChange(TXTBIT_SAVESELECTION,
		m_fSaveSelection ? TXTBIT_SAVESELECTION : 0);

	return fResult;
}

HRESULT CTextHost::OnTxInPlaceDeactivate()
{
	HRESULT hr = m_pserv->OnTxInPlaceDeactivate();

	if (SUCCEEDED(hr))
	{
		m_fInplaceActive = FALSE;
	}

	return hr;
}

HRESULT CTextHost::OnTxInPlaceActivate(LPCRECT prcClient)
{
	m_fInplaceActive = TRUE;

	HRESULT hr = m_pserv->OnTxInPlaceActivate(prcClient);

	if (FAILED(hr))
	{
		m_fInplaceActive = FALSE;
	}

	return hr;
}

BOOL CTextHost::DoSetCursor(RECT *prc, POINT *pt)
{
	RECT rc = prc ? *prc : m_rcClient;

	// Is this in our rectangle?
	if (PtInRect(&rc, *pt))
	{
		RECT *prcClient = (!m_fInplaceActive || prc) ? &rc : NULL;
		m_pserv->OnTxSetCursor(DVASPECT_CONTENT, -1, NULL, NULL, m_re->GetDC(),
			NULL, prcClient, pt->x, pt->y);

		return TRUE;
	}

	return FALSE;
}

void CTextHost::SetTransparent(BOOL f_Transparent)
{
	m_fTransparent = f_Transparent;

	// notify text services of property change
	m_pserv->OnTxPropertyBitsChange(TXTBIT_BACKSTYLECHANGE, 0);
}

LONG CTextHost::SetAccelPos(LONG l_accelpos)
{
	LONG laccelposOld = l_accelpos;

	m_laccelpos = l_accelpos;

	// notify text services of property change
	m_pserv->OnTxPropertyBitsChange(TXTBIT_SHOWACCELERATOR, 0);

	return laccelposOld;
}

WCHAR CTextHost::SetPasswordChar(WCHAR ch_PasswordChar)
{
	WCHAR chOldPasswordChar = m_chPasswordChar;

	m_chPasswordChar = ch_PasswordChar;

	// notify text services of property change
	m_pserv->OnTxPropertyBitsChange(TXTBIT_USEPASSWORD,
		(m_chPasswordChar != 0) ? TXTBIT_USEPASSWORD : 0);

	return chOldPasswordChar;
}

void CTextHost::SetDisabled(BOOL fOn)
{
	m_cf.dwMask |= CFM_COLOR | CFM_DISABLED;
	m_cf.dwEffects |= CFE_AUTOCOLOR | CFE_DISABLED;

	if (!fOn)
	{
		m_cf.dwEffects &= ~CFE_DISABLED;
	}

	m_pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE,
		TXTBIT_CHARFORMATCHANGE);
}

LONG CTextHost::SetSelBarWidth(LONG l_SelBarWidth)
{
	LONG lOldSelBarWidth = m_lSelBarWidth;

	m_lSelBarWidth = l_SelBarWidth;

	if (m_lSelBarWidth)
	{
		m_dwStyle |= ES_SELECTIONBAR;
	}
	else
	{
		m_dwStyle &= (~ES_SELECTIONBAR);
	}

	m_pserv->OnTxPropertyBitsChange(TXTBIT_SELBARCHANGE, TXTBIT_SELBARCHANGE);

	return lOldSelBarWidth;
}

BOOL CTextHost::GetTimerState()
{
	return m_fTimer;
}

void CTextHost::SetCharFormat(CHARFORMAT2W &c)
{
	m_cf = c;
}

void CTextHost::SetParaFormat(PARAFORMAT2 &p)
{
	m_pf = p;
}

void CTextHost::SetZoomEnableInSetClientRect(bool bEnable)
{
	m_bZoomEnable = bEnable;
}

bool CTextHost::IsZoomEnableInSetClientRect() const
{
	return m_bZoomEnable;
}

//////////////////////////////////////////////////////////////////////////

CWLRichEditCtrl::CWLRichEditCtrl()
	: m_pTwh(NULL)
{
	CWLWnd::m_bWLWnd = true;
	CRichEditCtrlBase::m_bWLWnd = true;
	CRichEditCtrlBase::m_pBindWLWnd = this;

	m_hPaintDC = NULL;
}

CWLRichEditCtrl::~CWLRichEditCtrl()
{
	if (m_pTwh)
	{
		m_pTwh->Release();
	}
}

BOOL CWLRichEditCtrl::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CWLWnd *pVirtualParent)
{
	if (CWLWnd::IsCreated())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	BOOL bRet = CWLWnd::Create(_T("IUIWL_RichEdit"), NULL, dwStyle, rect, pParentWnd, nID, pVirtualParent);
	if (!bRet)
	{
		return FALSE;
	}

	CTextHost *phost = new CTextHost;

	if (phost != NULL)
	{
		if (phost->Init(this, ES_MULTILINE))
		{
			m_pTwh = phost;
		}
	}

	if (m_pTwh)
	{
		// 为基类的m_pTextServices赋值。
		m_pTextServices = m_pTwh->GetTextServices();

		m_pTwh->SetTransparent(TRUE);

		LRESULT lResult;
		m_pTwh->GetTextServices()->TxSendMessage(EM_SETLANGOPTIONS, 0, 0, &lResult);
		m_pTwh->OnTxInPlaceActivate(NULL);
		m_pTwh->SetZoomEnableInSetClientRect(m_pTwh->IsZoomEnableInSetClientRect());
	}

	return TRUE;
}

BOOL CWLRichEditCtrl::CreateEx(
	DWORD dwExStyle,
	DWORD dwStyle,
	const RECT &rect,
	CWnd *pParentWnd,
	UINT nID
)
{
	return Create(dwStyle, rect, pParentWnd, nID, NULL);
}

int CWLRichEditCtrl::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint/* = TRUE*/)
{
	if (m_pTwh->GetTextServices() == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	CRectCtrl::MoveWindow(CRect(x, y, x + nWidth, y + nHeight), bRepaint);

	CRect rcWorkArea = CRect(x, y, x + nWidth, y + nHeight);
	rcWorkArea.DeflateRect(m_rcInset);
	bool bVScrollBarVisiable = false;

	//     if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() && !m_bFloatVBar) {
	//         bVScrollBarVisiable = true;
	//         rc.right -= m_pVerticalScrollBar->GetFixedWidth();
	//     }
	//
	//     if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() && !m_bFloatHBar) {
	//         rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
	//     }

	if (m_pTwh != NULL)
	{
		m_pTwh->SetClientRect(rcWorkArea);
		//
		//         if ( bVScrollBarVisiable && (!m_pVerticalScrollBar->IsVisible() || m_bVScrollBarFixing) ) {
		//             LONG lWidth = rc.right - rc.left + m_pVerticalScrollBar->GetFixedWidth();
		//             LONG lHeight = 0;
		//             SIZEL szExtent = { -1, -1 };
		//             m_pTwh->GetTextServices()->TxGetNaturalSize(
		//                 DVASPECT_CONTENT,
		//                 GetManager()->GetPaintDC(),
		//                 NULL,
		//                 NULL,
		//                 TXTNS_FITTOCONTENT,
		//                 &szExtent,
		//                 &lWidth,
		//                 &lHeight);
		//
		//             if ( lHeight > rc.bottom - rc.top ) {
		//                 m_pVerticalScrollBar->SetVisible(true);
		//                 m_pVerticalScrollBar->SetScrollPos(0);
		//                 m_bVScrollBarFixing = true;
		//             } else {
		//                 if ( m_bVScrollBarFixing ) {
		//                     m_pVerticalScrollBar->SetVisible(false);
		//                     m_bVScrollBarFixing = false;
		//                 }
		//             }
		//         }
	}

	//     if ( m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible() ) {
	//         RECT rcScrollBarPos = { rc.right, rc.top, rc.right + m_pVerticalScrollBar->GetFixedWidth(), rc.bottom};
	//
	//         if (m_bFloatVBar) {
	//             rcScrollBarPos.left = rc.right - m_pVerticalScrollBar->GetFixedWidth();
	//             rcScrollBarPos.top = rc.top;
	//             rcScrollBarPos.right = rc.right;
	//             rcScrollBarPos.bottom = rc.bottom;
	//             //rcScrollBarPos = { rc.right - m_pVerticalScrollBar->GetFixedWidth(), rc.top, rc.right, rc.bottom };
	//         }
	//
	//         m_pVerticalScrollBar->SetPos(rcScrollBarPos);
	//     }

	//     if ( m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible() ) {
	//         RECT rcScrollBarPos = { rc.left, rc.bottom, rc.right, rc.bottom + m_pHorizontalScrollBar->GetFixedHeight()};
	//
	//         if (m_bFloatHBar) {
	//             rcScrollBarPos.left = rc.left;
	//             rcScrollBarPos.top = rc.bottom - m_pHorizontalScrollBar->GetFixedHeight();
	//             rcScrollBarPos.right = rc.right;
	//             rcScrollBarPos.bottom = rc.bottom;
	//             //rcScrollBarPos = { rc.left, rc.bottom - m_pHorizontalScrollBar->GetFixedHeight(), rc.right, rc.bottom };
	//         }
	//
	//         m_pHorizontalScrollBar->SetPos(rcScrollBarPos);
	//     }

	//     for ( int it = 0; it < m_items.GetSize(); it++ ) {
	//         CControlUI *pControl = static_cast<CControlUI *>(m_items[it]);
	//
	//         if ( !pControl->IsVisible() ) {
	//             continue;
	//         }
	//
	//         if ( pControl->IsFloat() ) {
	//             SetFloatPos(it);
	//         } else {
	//             //SIZE sz = { rc.right - rc.left, rc.bottom - rc.top };
	//             //if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
	//             //if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
	//             //if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
	//             //if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
	//             //RECT rcCtrl = { rc.left, rc.top, rc.left + sz.cx, rc.top + sz.cy };
	//             //pControl->SetPos(rcCtrl);
	//             pControl->SetPos(rc); // 所有非float子控件放大到整个客户区
	//         }
	//     }

	return 0;
}

int CWLRichEditCtrl::MoveWindow(LPCRECT lpRect, BOOL bRepaint/* = TRUE*/)
{
	if (lpRect == NULL)
	{
		return -1;
	}

	return MoveWindow(lpRect->left, lpRect->top,
			lpRect->right - lpRect->left, lpRect->bottom - lpRect->top,
			bRepaint);
}

LRESULT CWLRichEditCtrl::TxSendMessage(UINT message, WPARAM wParam/* = 0*/, LPARAM lParam/* = 0*/)
{
	if (GetTextServices() != NULL)
	{
		LRESULT lRet = 0;
		HRESULT hr = GetTextServices()->TxSendMessage(message, wParam, lParam, &lRet);
		return lRet;
	}
	else
	{
		return 0;
	}
}

LRESULT CWLRichEditCtrl::TxSendMessage(UINT message, WPARAM wParam/* = 0*/, LPARAM lParam/* = 0*/) const
{
	ASSERT(CWLWnd::IsCreated());

	CWLRichEditCtrl *pThis = (CWLRichEditCtrl *)this;

	if (pThis->GetTextServices() != NULL)
	{
		LRESULT lRet = 0;
		HRESULT hr = pThis->GetTextServices()->TxSendMessage(message, wParam, lParam, &lRet);
		return lRet;
	}
	else
	{
		return 0;
	}
}

BOOL CWLRichEditCtrl::CanUndo() const
{
	ASSERT(CWLWnd::IsCreated());
	return (BOOL)SendMessage(EM_CANUNDO, 0, 0);
}
BOOL CWLRichEditCtrl::CanRedo() const
{
	ASSERT(IsCreated());
	return (BOOL)SendMessage(EM_CANREDO, 0, 0);
}
UNDONAMEID CWLRichEditCtrl::GetUndoName() const
{
	ASSERT(IsCreated());
	return (UNDONAMEID) SendMessage(EM_GETUNDONAME, 0, 0);
}
UNDONAMEID CWLRichEditCtrl::GetRedoName() const
{
	ASSERT(IsCreated());
	return (UNDONAMEID) SendMessage(EM_GETREDONAME, 0, 0);
}
int CWLRichEditCtrl::GetLineCount() const
{
	ASSERT(IsCreated());
	return (int)SendMessage(EM_GETLINECOUNT, 0, 0);
}
BOOL CWLRichEditCtrl::GetModify() const
{
	ASSERT(IsCreated());
	return (BOOL)SendMessage(EM_GETMODIFY, 0, 0);
}
void CWLRichEditCtrl::SetModify(BOOL bModified /* = TRUE */)
{
	ASSERT(IsCreated());
	SendMessage(EM_SETMODIFY, bModified, 0);
}
BOOL CWLRichEditCtrl::SetTextMode(UINT fMode)
{
	ASSERT(IsCreated());
	return (BOOL) SendMessage(EM_SETTEXTMODE, (WPARAM) fMode, 0);
}
UINT CWLRichEditCtrl::GetTextMode() const
{
	ASSERT(IsCreated());
	return (UINT) SendMessage(EM_GETTEXTMODE, 0, 0);
}
void CWLRichEditCtrl::GetRect(LPRECT lpRect) const
{
	ASSERT(IsCreated());
	SendMessage(EM_GETRECT, 0, (LPARAM)lpRect);
}
CPoint CWLRichEditCtrl::GetCharPos(long lChar) const
{
	ASSERT(IsCreated());
	CPoint pt;
	SendMessage(EM_POSFROMCHAR, (WPARAM)&pt, (LPARAM)lChar);
	return pt;
}
UINT CWLRichEditCtrl::GetOptions() const
{
	ASSERT(IsCreated());
	return (UINT) SendMessage(EM_GETOPTIONS, 0, 0);
}
void CWLRichEditCtrl::SetOptions(WORD wOp, DWORD dwFlags)
{
	ASSERT(IsCreated());
	SendMessage(EM_SETOPTIONS, (WPARAM)wOp, (LPARAM)dwFlags);
}
BOOL CWLRichEditCtrl::SetAutoURLDetect(BOOL bEnable /* = TRUE */)
{
	ASSERT(IsCreated());
	return (BOOL) SendMessage(EM_AUTOURLDETECT, (WPARAM) bEnable, 0);
}
void CWLRichEditCtrl::EmptyUndoBuffer()
{
	ASSERT(IsCreated());
	SendMessage(EM_EMPTYUNDOBUFFER, 0, 0);
}
UINT CWLRichEditCtrl::SetUndoLimit(UINT nLimit)
{
	ASSERT(IsCreated());
	return (UINT) SendMessage(EM_SETUNDOLIMIT, (WPARAM) nLimit, 0);
}
void CWLRichEditCtrl::ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo)
{
	ASSERT(IsCreated());
	SendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpszNewText);
}
void CWLRichEditCtrl::SetRect(LPCRECT lpRect)
{
	ASSERT(IsCreated());
	SendMessage(EM_SETRECT, 0, (LPARAM)lpRect);
}
void CWLRichEditCtrl::StopGroupTyping()
{
	ASSERT(IsCreated());
	SendMessage(EM_STOPGROUPTYPING, 0, 0);
}
BOOL CWLRichEditCtrl::Redo()
{
	ASSERT(IsCreated());
	return (BOOL) SendMessage(EM_REDO, 0, 0);
}
BOOL CWLRichEditCtrl::Undo()
{
	ASSERT(IsCreated());
	return (BOOL)SendMessage(EM_UNDO, 0, 0);
}
void CWLRichEditCtrl::Clear()
{
	ASSERT(IsCreated());
	SendMessage(WM_CLEAR, 0, 0);
}
void CWLRichEditCtrl::Copy()
{
	ASSERT(IsCreated());
	SendMessage(WM_COPY, 0, 0);
}
void CWLRichEditCtrl::Cut()
{
	ASSERT(IsCreated());
	SendMessage(WM_CUT, 0, 0);
}
void CWLRichEditCtrl::Paste()
{
	ASSERT(IsCreated());
	SendMessage(WM_PASTE, 0, 0);
}
BOOL CWLRichEditCtrl::SetReadOnly(BOOL bReadOnly /* = TRUE */)
{
	ASSERT(IsCreated());
	return (BOOL)SendMessage(EM_SETREADONLY, bReadOnly, 0L);
}
int CWLRichEditCtrl::GetFirstVisibleLine() const
{
	ASSERT(IsCreated());
	return (int)SendMessage(EM_GETFIRSTVISIBLELINE, 0, 0L);
}
BOOL CWLRichEditCtrl::DisplayBand(LPRECT pDisplayRect)
{
	ASSERT(IsCreated());
	return (BOOL)SendMessage(EM_DISPLAYBAND, 0, (LPARAM)pDisplayRect);
}
void CWLRichEditCtrl::GetSel(CHARRANGE &cr) const
{
	ASSERT(IsCreated());
	SendMessage(EM_EXGETSEL, 0, (LPARAM)&cr);
}
BOOL CWLRichEditCtrl::GetPunctuation(UINT fType, PUNCTUATION *lpPunc) const
{
	ASSERT(IsCreated());
	return (BOOL) SendMessage(EM_GETPUNCTUATION, (WPARAM) fType, (LPARAM) lpPunc);
}
BOOL CWLRichEditCtrl::SetPunctuation(UINT fType, PUNCTUATION *lpPunc)
{
	ASSERT(IsCreated());
	return (BOOL) SendMessage(EM_SETPUNCTUATION, (WPARAM) fType, (LPARAM) lpPunc);
}
void CWLRichEditCtrl::LimitText(long nChars)
{
	ASSERT(IsCreated());
	SendMessage(EM_EXLIMITTEXT, 0, nChars);
}
long CWLRichEditCtrl::LineFromChar(long nIndex) const
{
	ASSERT(IsCreated());
	return (long)SendMessage(EM_EXLINEFROMCHAR, 0, nIndex);
}
CPoint CWLRichEditCtrl::PosFromChar(UINT nChar) const
{
	ASSERT(IsCreated());
	POINTL pt;
	SendMessage(EM_POSFROMCHAR, (WPARAM)&pt, nChar);
	return CPoint(pt.x, pt.y);
}
int CWLRichEditCtrl::CharFromPos(CPoint pt) const
{
	ASSERT(IsCreated());
	POINTL ptl = {pt.x, pt.y};
	return (int)SendMessage(EM_CHARFROMPOS, 0, (LPARAM)&ptl);
}
void CWLRichEditCtrl::SetSel(CHARRANGE &cr)
{
	ASSERT(IsCreated());
	SendMessage(EM_EXSETSEL, 0, (LPARAM)&cr);
}
DWORD CWLRichEditCtrl::FindWordBreak(UINT nCode, DWORD nStart) const
{
	ASSERT(IsCreated());
	return (DWORD)SendMessage(EM_FINDWORDBREAK, (WPARAM) nCode, (LPARAM) nStart);
}

long CWLRichEditCtrl::FindText(DWORD dwFlags, FINDTEXTEX *pFindText) const
{
	ASSERT(IsCreated());
	return (long)SendMessage(EM_FINDTEXTEX, dwFlags, (LPARAM)pFindText);
}

long CWLRichEditCtrl::FormatRange(FORMATRANGE *pfr, BOOL bDisplay)
{
	ASSERT(IsCreated());
	return (long)SendMessage(EM_FORMATRANGE, (WPARAM)bDisplay, (LPARAM)pfr);
}

long CWLRichEditCtrl::GetEventMask() const
{
	ASSERT(IsCreated());
	return (long)SendMessage(EM_GETEVENTMASK, 0, 0L);
}

long CWLRichEditCtrl::GetLimitText() const
{
	if (IsCreated())
	{
		return (long)SendMessage(EM_GETLIMITTEXT, 0, 0L);
	}
	else
	{
		return g_cInitTextMax;
	}
}

long CWLRichEditCtrl::GetSelText(__out LPSTR lpBuf) const
{
	ASSERT(IsCreated());
	return (long)SendMessage(EM_GETSELTEXT, 0, (LPARAM)lpBuf);
}

void CWLRichEditCtrl::HideSelection(BOOL bHide, BOOL bPerm)
{
	ASSERT(IsCreated());
	SendMessage(EM_HIDESELECTION, bHide, bPerm);
}

void CWLRichEditCtrl::RequestResize()
{
	ASSERT(IsCreated());
	SendMessage(EM_REQUESTRESIZE, 0, 0L);
}

WORD CWLRichEditCtrl::GetSelectionType() const
{
	ASSERT(IsCreated());
	return (WORD)SendMessage(EM_SELECTIONTYPE, 0, 0L);
}

UINT CWLRichEditCtrl::GetWordWrapMode() const
{
	ASSERT(IsCreated());
	return (UINT) SendMessage(EM_GETWORDWRAPMODE, 0, 0);
}

UINT CWLRichEditCtrl::SetWordWrapMode(UINT uFlags) const
{
	ASSERT(IsCreated());
	return (UINT) SendMessage(EM_SETWORDWRAPMODE, (WPARAM) uFlags, 0);
}

COLORREF CWLRichEditCtrl::SetBackgroundColor(BOOL bSysColor, COLORREF cr)
{
	ASSERT(IsCreated());
	return (COLORREF)SendMessage(EM_SETBKGNDCOLOR, bSysColor, cr);
}

DWORD CWLRichEditCtrl::SetEventMask(DWORD dwEventMask)
{
	ASSERT(IsCreated());
	return (DWORD)SendMessage(EM_SETEVENTMASK, 0, dwEventMask);
}

BOOL CWLRichEditCtrl::SetOLECallback(IRichEditOleCallback *pCallback)
{
	ASSERT(IsCreated());
	return (BOOL)SendMessage(EM_SETOLECALLBACK, 0, (LPARAM)pCallback);
}

BOOL CWLRichEditCtrl::SetTargetDevice(HDC hDC, long lLineWidth)
{
	ASSERT(IsCreated());
	return (BOOL)SendMessage(EM_SETTARGETDEVICE, (WPARAM)hDC, lLineWidth);
}

BOOL CWLRichEditCtrl::SetTargetDevice(CDC &dc, long lLineWidth)
{
	ASSERT(IsCreated());
	return (BOOL)SendMessage(EM_SETTARGETDEVICE, (WPARAM)dc.m_hDC, lLineWidth);
}

long CWLRichEditCtrl::StreamIn(int nFormat, EDITSTREAM &es)
{
	ASSERT(IsCreated());
	return (long)SendMessage(EM_STREAMIN, nFormat, (LPARAM)&es);
}

long CWLRichEditCtrl::StreamOut(int nFormat, EDITSTREAM &es)
{
	ASSERT(IsCreated());
	return (long)SendMessage(EM_STREAMOUT, nFormat, (LPARAM)&es);
}

long CWLRichEditCtrl::GetTextLength() const
{
	ASSERT(IsCreated());
	return (long)SendMessage(WM_GETTEXTLENGTH, NULL, NULL);
}

int CWLRichEditCtrl::GetLine(__in int nIndex, LPTSTR lpszBuffer) const
{
	ASSERT(IsCreated());
	return (int)SendMessage(EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
}

int CWLRichEditCtrl::LineIndex(int nLine /* = -1 */) const
{
	ASSERT(IsCreated());
	return (int)SendMessage(EM_LINEINDEX, nLine, 0);
}

int CWLRichEditCtrl::LineLength(int nLine /* = -1 */) const
{
	ASSERT(IsCreated());
	return (int)SendMessage(EM_LINELENGTH, nLine, 0);
}

void CWLRichEditCtrl::LineScroll(int nLines, int nChars /* = 0 */)
{
	ASSERT(IsCreated());
	SendMessage(EM_LINESCROLL, nChars, nLines);
}

void CWLRichEditCtrl::SetSel(long nStartChar, long nEndChar)
{
	ASSERT(IsCreated());
	CHARRANGE cr;
	cr.cpMin = nStartChar;
	cr.cpMax = nEndChar;
	SendMessage(EM_EXSETSEL, 0, (LPARAM)&cr);
}

BOOL CWLRichEditCtrl::CanPaste(UINT nFormat) const
{
	ASSERT(IsCreated());
	COleMessageFilter *pFilter = AfxOleGetMessageFilter();
	if (pFilter != NULL)
	{
		pFilter->BeginBusyState();
	}
	BOOL b = (BOOL)SendMessage(EM_CANPASTE, nFormat, 0L);
	if (pFilter != NULL)
	{
		pFilter->EndBusyState();
	}
	return b;
}

void CWLRichEditCtrl::PasteSpecial(UINT nClipFormat, DWORD dvAspect, HMETAFILE hMF)
{
	ASSERT(IsCreated());
	REPASTESPECIAL reps;
	reps.dwAspect = dvAspect;
	reps.dwParam = (DWORD_PTR)hMF;
	SendMessage(EM_PASTESPECIAL, nClipFormat, (LPARAM)&reps);
}

int CWLRichEditCtrl::GetLine(__in int nIndex, LPTSTR lpszBuffer, __in int nMaxLength) const
{
	ASSERT(IsCreated());
	ASSERT(sizeof(nMaxLength) <= nMaxLength * sizeof(TCHAR) && nMaxLength > 0);
	*(LPINT)lpszBuffer = nMaxLength;
	return (int)SendMessage(EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
}

void CWLRichEditCtrl::GetSel(long &nStartChar, long &nEndChar) const
{
	ASSERT(IsCreated());
	CHARRANGE cr;
	SendMessage(EM_EXGETSEL, 0, (LPARAM)&cr);
	nStartChar = cr.cpMin;
	nEndChar = cr.cpMax;
}

CString CWLRichEditCtrl::GetSelText() const
{
	ASSERT(IsCreated());
	CHARRANGE cr;
	cr.cpMin = cr.cpMax = 0;
	SendMessage(EM_EXGETSEL, 0, (LPARAM)&cr);
	CString strText;
	LPTSTR lpsz = strText.GetBufferSetLength((cr.cpMax - cr.cpMin + 1) * 2);
	lpsz[0] = NULL;
	SendMessage(EM_GETSELTEXT, 0, (LPARAM)lpsz);
	strText.ReleaseBuffer();
	return CString(strText);
}

IRichEditOle *CWLRichEditCtrl::GetIRichEditOle() const
{
	ASSERT(IsCreated());

	if (m_pTwh == NULL)
	{
		return NULL;
	}

	ITextServices *pTextServices = m_pTwh->GetTextServices();

	if (!pTextServices)
	{
		ASSERT(0);
		return NULL;
	}

	IRichEditOle *pRichEditOle = NULL;
	pTextServices->TxSendMessage(EM_GETOLEINTERFACE, 0, (LPARAM)&pRichEditOle, NULL);

	if (NULL == pRichEditOle)
	{
		ASSERT(0);
		return NULL;
	}

	return pRichEditOle;
}

long CWLRichEditCtrl::GetTextLengthEx(DWORD dwFlags, UINT nCodePage/* = -1*/) const
{
	ASSERT(IsCreated());
	GETTEXTLENGTHEX textLenEx;
	textLenEx.flags = dwFlags;

	if (nCodePage == -1)
	{
#ifdef _UNICODE
		// UNICODE code page
		textLenEx.codepage = 1200;
#else
		// default code page
		textLenEx.codepage = CP_ACP;
#endif
	}
	else
		// otherwise, use the code page specified
	{
		textLenEx.codepage = nCodePage;
	}

	return (long)SendMessage(EM_GETTEXTLENGTHEX, (WPARAM)&textLenEx, 0);
}

int CWLRichEditCtrl::GetTextRange(int nFirst, int nLast, CString &refString) const
{
	ASSERT(IsCreated());

#ifdef _UNICODE
	TEXTRANGEW textRange;
#else
	TEXTRANGE textRange;
#endif
	textRange.chrg.cpMin = nFirst;
	textRange.chrg.cpMax = nLast;

	// can't be backwards
	int nLength = int(nLast - nFirst + 1);
	ASSERT(nLength > 0);

	textRange.lpstrText = refString.GetBuffer(nLength);
	nLength = (int)SendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&textRange);
	refString.ReleaseBuffer(nLength);

	return nLength;
}

BOOL CWLRichEditCtrl::SetDefaultCharFormat(CHARFORMAT &cf)
{
	ASSERT(IsCreated());
	cf.cbSize = sizeof(CHARFORMAT);
	return (BOOL)SendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&cf);
}

BOOL CWLRichEditCtrl::SetDefaultCharFormat(CHARFORMAT2 &cf)
{
	ASSERT(IsCreated());
	cf.cbSize = sizeof(CHARFORMAT2);
	return (BOOL)SendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&cf);
}

BOOL CWLRichEditCtrl::SetSelectionCharFormat(CHARFORMAT &cf)
{
	ASSERT(IsCreated());
	cf.cbSize = sizeof(CHARFORMAT);
	return (BOOL)SendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

BOOL CWLRichEditCtrl::SetSelectionCharFormat(CHARFORMAT2 &cf)
{
	ASSERT(IsCreated());
	cf.cbSize = sizeof(CHARFORMAT2);
	return (BOOL)SendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

BOOL CWLRichEditCtrl::SetWordCharFormat(CHARFORMAT &cf)
{
	ASSERT(IsCreated());
	cf.cbSize = sizeof(CHARFORMAT);
	return (BOOL)SendMessage(EM_SETCHARFORMAT, SCF_SELECTION | SCF_WORD, (LPARAM)&cf);
}

BOOL CWLRichEditCtrl::SetWordCharFormat(CHARFORMAT2 &cf)
{
	ASSERT(IsCreated());
	cf.cbSize = sizeof(CHARFORMAT2);
	return (BOOL)SendMessage(EM_SETCHARFORMAT, SCF_SELECTION | SCF_WORD, (LPARAM)&cf);
}

DWORD CWLRichEditCtrl::GetDefaultCharFormat(CHARFORMAT &cf) const
{
	ASSERT(IsCreated());
	cf.cbSize = sizeof(CHARFORMAT);
	return (DWORD)SendMessage(EM_GETCHARFORMAT, 0, (LPARAM)&cf);
}

DWORD CWLRichEditCtrl::GetDefaultCharFormat(CHARFORMAT2 &cf) const
{
	ASSERT(IsCreated());
	cf.cbSize = sizeof(CHARFORMAT2);
	return (DWORD)SendMessage(EM_GETCHARFORMAT, 0, (LPARAM)&cf);
}

DWORD CWLRichEditCtrl::GetSelectionCharFormat(CHARFORMAT &cf) const
{
	ASSERT(IsCreated());
	cf.cbSize = sizeof(CHARFORMAT);
	return (DWORD)SendMessage(EM_GETCHARFORMAT, 1, (LPARAM)&cf);
}

DWORD CWLRichEditCtrl::GetSelectionCharFormat(CHARFORMAT2 &cf) const
{
	ASSERT(IsCreated());
	cf.cbSize = sizeof(CHARFORMAT2);
	return (DWORD)SendMessage(EM_GETCHARFORMAT, 1, (LPARAM)&cf);
}

DWORD CWLRichEditCtrl::GetParaFormat(PARAFORMAT &pf) const
{
	ASSERT(IsCreated());
	pf.cbSize = sizeof(PARAFORMAT);
	return (DWORD)SendMessage(EM_GETPARAFORMAT, 0, (LPARAM)&pf);
}

DWORD CWLRichEditCtrl::GetParaFormat(PARAFORMAT2 &pf) const
{
	ASSERT(IsCreated());
	pf.cbSize = sizeof(PARAFORMAT2);
	return (DWORD)SendMessage(EM_GETPARAFORMAT, 0, (LPARAM)&pf);
}

BOOL CWLRichEditCtrl::SetParaFormat(PARAFORMAT &pf)
{
	ASSERT(IsCreated());
	pf.cbSize = sizeof(PARAFORMAT);
	return (BOOL)SendMessage(EM_SETPARAFORMAT, 0, (LPARAM)&pf);
}

BOOL CWLRichEditCtrl::SetParaFormat(PARAFORMAT2 &pf)
{
	ASSERT(IsCreated());
	pf.cbSize = sizeof(PARAFORMAT2);
	return (BOOL)SendMessage(EM_SETPARAFORMAT, 0, (LPARAM)&pf);
}
