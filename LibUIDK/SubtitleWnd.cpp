// SubtitleWnd.cpp : implementation file
//

#include "stdafx.h"
#include <atlconv.h>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct SUBTITLEMEMBER
{
	// Window background
	BOOL m_bShowWindowBackground;
	int m_nWindowTransparent;
	Color m_crBackground;

	// Line
	int m_nLineCount;
	int m_nLineSpacing;

	// Stroke
	BOOL m_bStroke;
	Color m_crStroke;
	int m_nStrokeCount;

	// Shadow
	BOOL m_bShadow;

	// Text
	HIUIFONT m_hIUIFont;
	BOOL m_bKaraoke;
	BACKGROUND_TYPE m_ePlayedTextBkMode;
	std::vector<Color> m_vPlayedTextBkColor;
	HIUIIMAGE m_hPlayedTextBkImage;
	BACKGROUND_TYPE m_ePlayingTextBkMode;
	std::vector<Color> m_vPlayingTextBkColor;
	HIUIIMAGE m_hPlayingTextBkImage;

	std::vector<CString> m_vSubtitle;

	// Scroll
	int m_nMin;
	int m_nMax;
	int m_nPos;
};

#define m_bShowWindowBackground (((SUBTITLEMEMBER *)m_pMember)->m_bShowWindowBackground)
#define m_nWindowTransparent (((SUBTITLEMEMBER *)m_pMember)->m_nWindowTransparent)
#define m_crBackground (((SUBTITLEMEMBER *)m_pMember)->m_crBackground)

#define m_nLineCount (((SUBTITLEMEMBER *)m_pMember)->m_nLineCount)
#define m_nLineSpacing (((SUBTITLEMEMBER *)m_pMember)->m_nLineSpacing)

#define m_bStroke (((SUBTITLEMEMBER *)m_pMember)->m_bStroke)
#define m_crStroke (((SUBTITLEMEMBER *)m_pMember)->m_crStroke)
#define m_nStrokeCount (((SUBTITLEMEMBER *)m_pMember)->m_nStrokeCount)

#define m_bShadow (((SUBTITLEMEMBER *)m_pMember)->m_bShadow)

#define m_hIUIFont (((SUBTITLEMEMBER *)m_pMember)->m_hIUIFont)
#define m_bKaraoke (((SUBTITLEMEMBER *)m_pMember)->m_bKaraoke)
#define m_ePlayedTextBkMode (((SUBTITLEMEMBER *)m_pMember)->m_ePlayedTextBkMode)
#define m_vPlayedTextBkColor (((SUBTITLEMEMBER *)m_pMember)->m_vPlayedTextBkColor)
#define m_hPlayedTextBkImage (((SUBTITLEMEMBER *)m_pMember)->m_hPlayedTextBkImage)
#define m_ePlayingTextBkMode (((SUBTITLEMEMBER *)m_pMember)->m_ePlayingTextBkMode)
#define m_vPlayingTextBkColor (((SUBTITLEMEMBER *)m_pMember)->m_vPlayingTextBkColor)
#define m_hPlayingTextBkImage (((SUBTITLEMEMBER *)m_pMember)->m_hPlayingTextBkImage)

#define m_vSubtitle (((SUBTITLEMEMBER *)m_pMember)->m_vSubtitle)

#define m_nMin (((SUBTITLEMEMBER *)m_pMember)->m_nMin)
#define m_nMax (((SUBTITLEMEMBER *)m_pMember)->m_nMax)
#define m_nPos (((SUBTITLEMEMBER *)m_pMember)->m_nPos)


/////////////////////////////////////////////////////////////////////////////
// CSubtitleWnd

CSubtitleWnd::CSubtitleWnd()
{
	m_pMember = NULL;
	m_pMember = new SUBTITLEMEMBER;

	m_bShowWindowBackground = FALSE;
	m_nWindowTransparent = 255;
	m_crBackground = Color(25, 228, 228, 228);

	m_nLineCount = 1;
	m_nLineSpacing = 0;

	// Stroke
	m_bStroke = TRUE;
	m_crStroke = Color(62, 2, 62, 87);
	m_nStrokeCount = 5;

	// Shadow
	m_bShadow = FALSE;

	// Text
	m_hIUIFont = NULL;

	m_bKaraoke = TRUE;
	m_ePlayedTextBkMode = BKT_COLOR;
	m_vPlayedTextBkColor.push_back(Color(255, 250, 250, 30));
	m_vPlayedTextBkColor.push_back(Color(255, 250, 110, 0));
	m_vPlayedTextBkColor.push_back(Color(255, 250, 200, 20));
	m_ePlayingTextBkMode = BKT_COLOR;
	m_vPlayingTextBkColor.push_back(Color(255, 135, 210, 255));
	m_vPlayingTextBkColor.push_back(Color(255, 20, 100, 180));
	m_vPlayingTextBkColor.push_back(Color(255, 105, 180, 255));

	m_vSubtitle.resize(m_nLineCount);

	m_nMin = 0;
	m_nMax = 100;
	m_nPos = 0;
}

CSubtitleWnd::~CSubtitleWnd()
{
	if (m_pMember != NULL)
	{
		delete (SUBTITLEMEMBER *)m_pMember;
		m_pMember = NULL;
	}
}


BEGIN_MESSAGE_MAP(CSubtitleWnd, CUIWndBase)
	//{{AFX_MSG_MAP(CSubtitleWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CSubtitleWnd::Create(DWORD dwExStyle, DWORD dwStyle, LPCTSTR lpszWindowName, const RECT &rect, CWnd *pParentWnd, CCreateContext *pContext/* = NULL*/)
{
	// attempt to create the window
	CString strMyClass;
	strMyClass = _T("UIWND");
	WNDCLASS wndcls;
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hIcon = NULL;
	wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = (LPCTSTR) strMyClass;

	// Register the new class and exit if it fails
	if (!AfxRegisterClass(&wndcls))
	{
		TRACE(_T("[LibUIDK]===Class Registration Failed\n"));
		return FALSE;
	}

	if (!CreateEx(WS_EX_LAYERED | WS_EX_TOPMOST | dwExStyle, strMyClass, lpszWindowName, WS_POPUP | WS_CLIPCHILDREN | dwStyle, rect, pParentWnd, 0))
	{
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// public

int CSubtitleWnd::ShowWindowBackground(BOOL bShow)
{
	m_bShowWindowBackground = bShow;

	return 0;
}

BOOL CSubtitleWnd::IsWindowBackgroundVisible() const
{
	return m_bShowWindowBackground;
}

int CSubtitleWnd::SetWindowTransparent(int nTransparent)
{
	m_nWindowTransparent = nTransparent;

	return 0;
}

int CSubtitleWnd::GetWindowTransparent() const
{
	return m_nWindowTransparent;
}

int CSubtitleWnd::SetWindowBkColor(Color crBk)
{
	m_crBackground = crBk;

	return 0;
}

Color CSubtitleWnd::GetWindowBkColor() const
{
	return m_crBackground;
}

int CSubtitleWnd::SetLineCount(int nCount)
{
	m_nLineCount = nCount;

	m_vSubtitle.resize(m_nLineCount);

	return 0;
}

int CSubtitleWnd::GetLineCount() const
{
	return m_nLineCount;
}

int CSubtitleWnd::SetLineSpacing(int nLineSpacing)
{
	m_nLineSpacing = nLineSpacing;

	return 0;
}

int CSubtitleWnd::GetLineSpacing() const
{
	return m_nLineSpacing;
}

int CSubtitleWnd::EnableStroke(BOOL bEnable)
{
	m_bStroke = bEnable;

	return 0;
}

BOOL CSubtitleWnd::IsStrokeEnabled() const
{
	return m_bStroke;
}

int CSubtitleWnd::SetStrokeColor(Color crStroke)
{
	m_crStroke = crStroke;

	return 0;
}

Color CSubtitleWnd::GetStrokeColor() const
{
	return m_crStroke;
}

int CSubtitleWnd::SetStrokeCount(int nCount)
{
	m_nStrokeCount = nCount;

	return 0;
}

int CSubtitleWnd::GetStrokeCount() const
{
	return m_nStrokeCount;
}

int CSubtitleWnd::EnableShadow(BOOL bEnable)
{
	m_bShadow = bEnable;

	return 0;
}

BOOL CSubtitleWnd::IsShadowEnabled() const
{
	return m_bShadow;
}

int CSubtitleWnd::SetSubtitle(int nLine, LPCTSTR lpszSubtitle, BOOL bRedraw/* = TRUE*/)
{
	if (nLine < 0 || nLine >= m_nLineCount)
	{
		return -1;
	}

	m_vSubtitle[nLine] = lpszSubtitle;

	if (bRedraw && m_hWnd != NULL)
	{
		Refresh();
	}

	return 0;
}

int CSubtitleWnd::GetSubtitle(int nLine, CString &strSubtitle) const
{
	if (nLine < 0 || nLine >= m_nLineCount)
	{
		return -1;
	}

	strSubtitle = m_vSubtitle[nLine];

	return 0;
}

int CSubtitleWnd::SetFont(LPCTSTR lpszFontID)
{
	if (lpszFontID == NULL)
	{
		return -1;
	}

	ReleaseIUIFontInternal(m_hIUIFont);
	m_hIUIFont = CreateIUIFontInternal(lpszFontID);

	return 0;
}

int CSubtitleWnd::GetFont(CString *pstrFontID) const
{
	if (pstrFontID == NULL)
	{
		return -1;
	}

	GetFontResID(m_hIUIFont, pstrFontID);

	return 0;
}

int CSubtitleWnd::EnableKaraokeMode(BOOL bEnable)
{
	m_bKaraoke = bEnable;

	return 0;
}

BOOL CSubtitleWnd::IsKaraokeModeEnabled() const
{
	return m_bKaraoke;
}

int CSubtitleWnd::SetPlayedTextBackgroundMode(BACKGROUND_TYPE eBkType)
{
	m_ePlayedTextBkMode = eBkType;

	return 0;
}

BACKGROUND_TYPE CSubtitleWnd::GetPlayedTextBackgroundMode() const
{
	return m_ePlayedTextBkMode;
}

int CSubtitleWnd::SetPlayedTextBkColor(const std::vector<Color> *pvColors)
{
	std::copy(pvColors->begin(), pvColors->end(), m_vPlayedTextBkColor.begin());

	return 0;
}

int CSubtitleWnd::GetPlayedTextBkColor(std::vector<Color> *pvColors) const
{
	if (pvColors == NULL)
	{
		return -1;
	}

	std::copy(m_vPlayedTextBkColor.begin(), m_vPlayedTextBkColor.end(), pvColors->begin());

	return 0;
}

int CSubtitleWnd::SetPlayedTextBkImage(LPCTSTR lpszImageName)
{
	IUISetControlImage(&m_hPlayedTextBkImage, lpszImageName);

	return 0;
}

int CSubtitleWnd::GetPlayedTextBkImage(CString *pstrImageName)
{
	IUIGetControlImage(m_hPlayedTextBkImage, pstrImageName);

	return 0;
}

int CSubtitleWnd::SetPlayingTextBackgroundMode(BACKGROUND_TYPE eBkType)
{
	m_ePlayingTextBkMode = eBkType;

	return 0;
}

BACKGROUND_TYPE CSubtitleWnd::GetPlayingTextBackgroundMode() const
{
	return m_ePlayingTextBkMode;
}

int CSubtitleWnd::SetPlayingTextBkColor(const std::vector<Color> *pvColors)
{
	std::copy(pvColors->begin(), pvColors->end(), m_vPlayingTextBkColor.begin());

	return 0;
}

int CSubtitleWnd::GetPlayingTextBkColor(std::vector<Color> *pvColors) const
{
	if (pvColors == NULL)
	{
		return -1;
	}

	std::copy(m_vPlayingTextBkColor.begin(), m_vPlayingTextBkColor.end(), pvColors->begin());

	return 0;
}

int CSubtitleWnd::SetPlayingTextBkImage(LPCTSTR lpszImageName)
{
	IUISetControlImage(&m_hPlayingTextBkImage, lpszImageName);

	return 0;
}

int CSubtitleWnd::GetPlayingTextBkImage(CString *pstrImageName)
{
	IUIGetControlImage(m_hPlayingTextBkImage, pstrImageName);

	return 0;
}

int CSubtitleWnd::SetRange(int nMix, int nMax)
{
	m_nMin = nMix;
	m_nMax = nMax;

	return 0;
}

int CSubtitleWnd::GetRange(int *pnMix, int *pnMax) const
{
	if (pnMix != NULL)
	{
		*pnMix = m_nMin;
	}
	if (pnMax != NULL)
	{
		*pnMax = m_nMax;
	}

	return 0;
}

int CSubtitleWnd::SetPos(int nPos)
{
	m_nPos = nPos;
	Invalidate();
	return 0;
}

int CSubtitleWnd::GetPos() const
{
	return m_nPos;
}

//////////////////////////////////////////////////////////////////////////
// public

int CSubtitleWnd::Refresh()
{
	ASSERT(m_hWnd != NULL);
	CRect rcWnd;
	GetWindowRect(&rcWnd);

	// Prepare dcMem
	CDC *pDC = GetDC();

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(pDC, rcWnd.Width(), rcWnd.Height());

	CBitmap *pOldMemBmp = dcMem.SelectObject(&bmpMem);

	Graphics graphics(dcMem.GetSafeHdc());

	// Draw the string path.
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	USES_CONVERSION;
	LOGFONT lf;
	HFONT hFont = GetHFont(m_hIUIFont);
	::GetObject(hFont, sizeof(LOGFONT), &lf);
	FontFamily fontFamily(T2W(lf.lfFaceName));

	int nTop = 0;
	int nTextHeight = abs(lf.lfHeight);
	for (int i = 0; i < m_nLineCount; ++i)
	{
		CString strSubtitle = m_vSubtitle[i];

		GraphicsPath path;
		StringFormat StrFormat;
		path.AddString(T2W(strSubtitle.GetBuffer(0)), -1, &fontFamily, FontStyleRegular, (REAL)nTextHeight, Point(0, nTop), &StrFormat);
		nTop += (nTextHeight + m_nLineSpacing);

		// Draw path many times by different width and color.
		for (REAL i = 1; i < m_nStrokeCount; ++i)
		{
			Pen pen(m_crStroke, i);
			pen.SetLineJoin(LineJoinRound);
			graphics.DrawPath(&pen, &path);
		}

		// Show the window background and border
		if (m_bShowWindowBackground)
		{
			SolidBrush brush(m_crBackground);
			graphics.FillRectangle(&brush, 0, 0, rcWnd.Width(), rcWnd.Height());

			Pen pen1(Color(155, 223, 223, 223));
			graphics.DrawRectangle(&pen1, 0, 0, rcWnd.Width() - 1, rcWnd.Height() - 1);

			Pen pen2(Color(55, 223, 223, 223));
			graphics.DrawRectangle(&pen2, 1, 1, rcWnd.Width() - 3, rcWnd.Height() - 3);
		}

		// Fill string
		size_t nColorCount = m_vPlayingTextBkColor.size();
		size_t nColorHeight = nTextHeight / (nColorCount - 1);
		for (size_t nIndex = 0; nIndex < nColorCount - 1; ++nIndex)
		{
			Color crStart = m_vPlayingTextBkColor[nIndex];
			Color crEnd = m_vPlayingTextBkColor[nIndex + 1];

			LinearGradientBrush lgBrush(Point(0, INT(nColorHeight * nIndex)), Point(0, INT(nColorHeight * (nIndex + 1))), crStart, crEnd);
			graphics.FillPath(&lgBrush, &path);
		}
	}

	// Call UpdateLayeredWindow
	POINT ptWinPos = {rcWnd.left, rcWnd.top};
	SIZE sizeWindow = {rcWnd.Width(), rcWnd.Height()};
	POINT ptSrc = {0, 0};

	BLENDFUNCTION blend;
	blend.BlendOp = 0;
	blend.BlendFlags = 0;
	blend.AlphaFormat = 1;
	blend.SourceConstantAlpha = m_nWindowTransparent;

	typedef BOOL (WINAPI * UPDATE_LAYERED_WINDOW)(HWND, HDC, POINT *, SIZE *, HDC, POINT *, COLORREF, BLENDFUNCTION *, DWORD);
	static UPDATE_LAYERED_WINDOW UpdateLayeredWindow = NULL;

	if (UpdateLayeredWindow == NULL)
	{
		HINSTANCE hInst = LoadLibrary(_T("User32.DLL"));
		if (hInst != NULL)
		{
			UpdateLayeredWindow = (UPDATE_LAYERED_WINDOW)GetProcAddress(hInst, "UpdateLayeredWindow");
			FreeLibrary(hInst);
		}
	}

	if (UpdateLayeredWindow == NULL)
	{
		return -1;
	}

	BOOL bRet = UpdateLayeredWindow(GetSafeHwnd(), pDC->GetSafeHdc(), &ptWinPos, &sizeWindow, dcMem.GetSafeHdc(), &ptSrc, 0, &blend, 2);
	dcMem.SelectObject(pOldMemBmp);

	ReleaseDC(pDC);

	if (!bRet)
	{
		return -1;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// message

LRESULT CSubtitleWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	if (message == WM_CREATE)
	{
		Refresh();
	}

	else if (message == WM_SYSCOMMAND)
	{
		UINT nID = (UINT)wParam;

		if (nID == SC_MINIMIZE)
		{
			return 0;	// If the window has WS_EX_TOPMOST style, shouldn't call this step.
		}
	}

	else if (message == WM_MOUSEMOVE)
	{
		if (!m_bShowWindowBackground)
		{
			m_bShowWindowBackground = TRUE;
			Refresh();
		}
	}

	else if (message == WM_SIZE)
	{
		Refresh();
	}

	return CUIWndBase::WindowProc(message, wParam, lParam);
}
