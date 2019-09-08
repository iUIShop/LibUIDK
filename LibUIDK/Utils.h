#pragma once

namespace LibUIDK
{
	extern int g_cxEdge;
	extern int g_cyEdge;
	extern int g_cxBorder;
	extern int g_cyBorder;
	extern int g_cxScreen;
	extern int g_cyScreen;
	extern int g_cxDoubleClk;
	extern int g_cyDoubleClk;

	extern int g_cxSmIcon;
	extern int g_cySmIcon;
	extern int g_cxIcon;
	extern int g_cyIcon;
	extern int g_cxFrame;
	extern int g_cyFrame;
	extern int g_cxIconSpacing, g_cyIconSpacing;
	extern int g_cxScrollbar, g_cyScrollbar;
	extern int g_cxIconMargin, g_cyIconMargin;
	extern int g_cyLabelSpace;
	extern int g_cxLabelMargin;
	//extern int g_cxIconOffset, g_cyIconOffset;
	extern int g_cxVScroll;
	extern int g_cyHScroll;
	extern int g_cxHScroll;
	extern int g_cyVScroll;
	extern int g_fDragFullWindows;
	extern int g_fDBCSEnabled;
	extern int g_fMEEnabled;
	extern int g_fThaiEnabled;
	extern int g_fDBCSInputEnabled;

	extern COLORREF g_clrWindow;
	extern COLORREF g_clrWindowText;
	extern COLORREF g_clrWindowFrame;
	extern COLORREF g_clrGrayText;
	extern COLORREF g_clrBtnText;
	extern COLORREF g_clrBtnFace;
	extern COLORREF g_clrBtnShadow;
	extern COLORREF g_clrBtnHighlight;
	extern COLORREF g_clrHighlight;
	extern COLORREF g_clrHighlightText;
	extern COLORREF g_clrInfoText;
	extern COLORREF g_clrInfoBk;
	extern COLORREF g_clr3DDkShadow;
	extern COLORREF g_clr3DLight;

	extern HBRUSH g_hbrGrayText;
	extern HBRUSH g_hbrWindow;
	extern HBRUSH g_hbrWindowText;
	extern HBRUSH g_hbrWindowFrame;
	extern HBRUSH g_hbrBtnFace;
	extern HBRUSH g_hbrBtnHighlight;
	extern HBRUSH g_hbrBtnShadow;
	extern HBRUSH g_hbrHighlight;

	extern HFONT g_hfontSystem;
#define WHEEL_DELTA     120
	extern UINT g_msgMSWheel;
	extern UINT g_ucScrollLines;
	extern int  gcWheelDelta;
	extern UINT g_uDragImages;

	extern const TCHAR c_szSToolTipsClass[];
	extern const TCHAR c_szTreeViewClass[];

#ifdef USE_MIRRORING
#define RTL_MIRRORED_WINDOW             dwExStyleRTLMirrorWnd
#define IS_WINDOW_RTL_MIRRORED(hwnd)    (g_bMirroredOS && Mirror_IsWindowMirroredRTL(hwnd))
#else
#define RTL_MIRRORED_WINDOW             0L
#define IS_WINDOW_RTL_MIRRORED(hwnd)    FALSE
#endif


#ifdef WIN32
#define CODESEG_INIT        ".text"
#else
#define CODESEG_INIT 	    "_INIT"
#endif

#define TVE_ACTIONMASK          0x0007      //  (TVE_COLLAPSE | TVE_EXPAND | TVE_TOGGLE)

#ifdef WIN32
	typedef MSG MSG32;
	typedef MSG32 FAR      *LPMSG32;
#define GetMessage32(lpmsg, hwnd, min, max, f32)        GetMessage(lpmsg, hwnd, min, max)
#define PeekMessage32(lpmsg, hwnd, min, max, flags, f32)       PeekMessage(lpmsg, hwnd, min, max, flags)
#define TranslateMessage32(lpmsg, f32)  TranslateMessage(lpmsg)
#define DispatchMessage32(lpmsg, f32)   DispatchMessage(lpmsg)
#define CallMsgFilter32(lpmsg, u, f32)  CallMsgFilter(lpmsg, u)
#define IsDialogMessage32(hwnd, lpmsg, f32)   IsDialogMessage(hwnd, lpmsg)
#endif

#define SSW_EX_NOTIMELIMIT      0x00010000
#define SSW_EX_IMMEDIATE        0x00020000
#define SSW_EX_IGNORESETTINGS   0x00040000  // ignore system settings to turn on/off smooth scroll

#define SSI_DEFAULT ((UINT)-1)

#define SSIF_SCROLLPROC    0x0001
#define SSIF_MAXSCROLLTIME 0x0002
#define SSIF_MINSCROLL     0x0004

	typedef int (CALLBACK *PFNSMOOTHSCROLLPROC)(HWND hWnd,
		int dx,
		int dy,
		CONST RECT *prcScroll,
		CONST RECT *prcClip,
		HRGN hrgnUpdate,
		LPRECT prcUpdate,
		UINT flags);

	typedef struct tagSSWInfo
	{
		UINT cbSize;
		DWORD fMask;
		HWND hwnd;
		int dx;
		int dy;
		LPCRECT lprcSrc;
		LPCRECT lprcClip;
		HRGN hrgnUpdate;
		LPRECT lprcUpdate;
		UINT fuScroll;

		UINT uMaxScrollTime;
		UINT cxMinScroll;
		UINT cyMinScroll;

		PFNSMOOTHSCROLLPROC pfnScrollProc;  // we'll call this back instead
	} SMOOTHSCROLLINFO, *PSMOOTHSCROLLINFO;

	typedef DWORD   BITBOOL;

	// Incremental search
	typedef struct ISEARCHINFO
	{
		int iIncrSearchFailed;
		LPTSTR pszCharBuf;                  // isearch string lives here
		int cbCharBuf;                      // allocated size of pszCharBuf
		int ichCharBuf;                     // number of live chars in pszCharBuf
		DWORD timeLast;                     // time of last input event
#if defined(FE_IME) || !defined(WINNT)
		BOOL fReplaceCompChar;
#endif

	} ISEARCHINFO, *PISEARCHINFO;

	typedef struct tagControlInfo
	{
		HWND        hwnd;
		HWND        hwndParent;
		DWORD       style;
		DWORD       dwCustom;
		BITBOOL     bUnicode : 1;
		BITBOOL     bInFakeCustomDraw: 1;
		UINT        uiCodePage;
		DWORD       dwExStyle;
		LRESULT     iVersion;
#ifdef KEYBOARDCUES
		WORD        wUIState;
#endif
	} LibUIDK_CONTROLINFO, FAR *LibUIDK_LPCONTROLINFO;


	typedef struct tagTHUNKSTATE
	{
		LPVOID ts_pvThunk1;
		LPVOID ts_pvThunk2;
		DWORD ts_dwThunkSize;
	} THUNKSTATE;

	typedef struct tagNMTTSHOWINFO
	{
		NMHDR hdr;
		DWORD dwStyle;
	} NMTTSHOWINFO, FAR *LPNMTTSHOWINFO;

	//
	// Bitfields don't get along too well with bools,
	// so here's an easy way to convert them:
	//
#define BOOLIFY(expr)           (!!(expr))

	// COMPILETIME_ASSERT(f)
	//
	//  Generates a build break at compile time if the constant expression
	//  is not true.  Unlike the "#if" compile-time directive, the expression
	//  in COMPILETIME_ASSERT() is allowed to use "sizeof".
	//
	//  Compiler magic!  If the expression "f" is FALSE, then you get the
	//  compiler error "Duplicate case expression in switch statement".
	//
#define COMPILETIME_ASSERT(f) switch (0) case 0: case f:

#ifdef _WIN64
#define HIWORD64(p)     ((ULONG_PTR)(p) >> 16)
#define BOOLFROMPTR(p)  ((p) != 0)
#define SPRINTF_PTR		"%016I64x"
#else
#define HIWORD64        HIWORD
#define BOOLFROMPTR(p)  ((BOOL)(p))
#define SPRINTF_PTR		"%08x"
#endif

	/*
	* Converts an ANSI string to UNICODE
	*/
#define ConvertAToWN( uiCodePage, pszWBuf, cchW, pszA, cchA )         \
	MultiByteToWideChar( uiCodePage, MB_PRECOMPOSED, pszA, cchA, pszWBuf, cchW )

	/*
	 * FreeProducedString
	 *
	 * Takes a pointer returned from Produce?From?() and frees it.  No
	 * validity checking is needed before calling this function.  (ie, any
	 * value returned by Produce?From?() can be safely sent to this function)
	 */
#define FreeProducedString( psz )   \
	if((psz) != NULL && ((LPSTR)psz) != LPSTR_TEXTCALLBACKA) {LocalFree(psz);} else

#define HDFT_ISMASK         0x000f
	/*
	* IsFlagPtr
	*  Returns TRUE if the pointer == NULL or -1
	*/
#define IsFlagPtr( p )  ((p) == NULL || (LPSTR)(p) == LPSTR_TEXTCALLBACKA)

#define LVN_GETEMPTYTEXTA          (LVN_FIRST-60)
#define LVN_GETEMPTYTEXTW          (LVN_FIRST-61)

#ifdef UNICODE
#define LVN_GETEMPTYTEXT           LVN_GETEMPTYTEXTW
#else
#define LVN_GETEMPTYTEXT           LVN_GETEMPTYTEXTA
#endif

#define ConvertWToAN( uiCodePage, pszABuf, cchA, pszW, cchW )         \
	WideCharToMultiByte(uiCodePage, 0, pszW, cchW, pszABuf, cchA, NULL, NULL)

#define CDRF_VALIDFLAGS         0xFFFF00F6
	typedef void (CALLBACK *NOTIFYWINEVENTPROC)(UINT, HWND, LONG, LONG_PTR);
#define DONOTHING_NOTIFYWINEVENT ((NOTIFYWINEVENTPROC)1)
#define DT_SEARCHTIMEOUT    1000L       // 1 seconds
#define g_dwPrototype   0
#define PTF_NOISEARCHTO 0x00000002  // No incremental search timeout
#define IsFlagSet(obj, f)           (BOOL)(((obj) & (f)) == (f))
	__inline BOOL IsISearchTimedOut(PISEARCHINFO pis)
	{
		return GetMessageTime() - pis->timeLast > DT_SEARCHTIMEOUT &&
			!IsFlagSet(g_dwPrototype, PTF_NOISEARCHTO);

	}


	//----------------------------------------------------------------------------
	// Define a Global Shared Heap that we use allocate memory out of that we
	// Need to share between multiple instances.
#ifndef WINNT
#define GROWABLE        0
#define MAXHEAPSIZE     GROWABLE
#define HEAP_SHARED	0x04000000		/* put heap in shared memory */
#endif

#define MAKELRESULTFROMUINT(i)  ((LRESULT)i)
#define STATEIMAGEMASKTOINDEX(i) ((i & LVIS_STATEIMAGEMASK) >> 12)

#define IDT_NAMEEDIT    42
#define CCResetInfoTipWidth(hwndOwner, hwndToolTips) \
	SendMessage(hwndToolTips, TTM_SETMAXTIPWIDTH, 0, -1)
#define FORWARD_WM_COMMAND(hwnd, id, hwndCtl, codeNotify, fn) \
	(void)(fn)((hwnd), WM_COMMAND, MAKEWPARAM((UINT)(id),(UINT)(codeNotify)), (LPARAM)(HWND)(hwndCtl))

#define TVM_SETBORDER         (TV_FIRST + 35)
#define TVM_GETBORDER         (TV_FIRST + 36)
#define TVSBF_YBORDER   0x00000002
#define TVSBF_XBORDER   0x00000001
#define CCM_TRANSLATEACCELERATOR (CCM_FIRST + 0xa)  // lParam == lpMsg
#define TVM_TRANSLATEACCELERATOR    CCM_TRANSLATEACCELERATOR
#define HANDLE_WM_CHAR(hwnd, wParam, lParam, fn) \
	((fn)((hwnd), (TCHAR)(wParam), (int)(short)LOWORD(lParam)), 0L)
#define IDC_HAND_INTERNAL       108
#define GET_WM_HSCROLL_CODE(wp, lp)                 LOWORD(wp)
#define GET_WM_HSCROLL_POS(wp, lp)                  HIWORD(wp)
#define GET_WM_HSCROLL_HWND(wp, lp)                 (HWND)(lp)
#define GET_WM_HSCROLL_MPS(code, pos, hwnd)    \
	(WPARAM)MAKELONG(code, pos), (LPARAM)(hwnd)

#define GET_WM_VSCROLL_CODE(wp, lp)                 LOWORD(wp)
#define GET_WM_VSCROLL_POS(wp, lp)                  HIWORD(wp)
#define GET_WM_VSCROLL_HWND(wp, lp)                 (HWND)(lp)
#define GET_WM_VSCROLL_MPS(code, pos, hwnd)    \
	(WPARAM)MAKELONG(code, pos), (LPARAM)(hwnd)

#define GET_WM_COMMAND_ID(wp, lp)               LOWORD(wp)
#define GET_WM_COMMAND_HWND(wp, lp)             (HWND)(lp)
#define GET_WM_COMMAND_CMD(wp, lp)              HIWORD(wp)
#define GET_WM_COMMAND_MPS(id, hwnd, cmd)    \
	(WPARAM)MAKELONG(id, cmd), (LPARAM)(hwnd)

#define MSAA_CLASSNAMEIDX_BASE 65536L
#define MSAA_CLASSNAMEIDX_TREEVIEW   (MSAA_CLASSNAMEIDX_BASE+25)
#define     GetWindowStyle(hwnd)    ((DWORD)GetWindowLong(hwnd, GWL_STYLE))
#define     GetWindowExStyle(hwnd)  ((DWORD)GetWindowLong(hwnd, GWL_EXSTYLE))

#define GetWindowInt        GetWindowLongPtr
#define SetWindowInt        SetWindowLongPtr
#define SetWindowID(hwnd,id)    SetWindowLongPtr(hwnd, GWLP_ID, id)
#define GetClassCursor(hwnd)    ((HCURSOR)GetClassLongPtr(hwnd, GCLP_HCURSOR))
#define GetClassIcon(hwnd)      ((HICON)GetClassLongPtr(hwnd, GCLP_HICON))
#define BOOL_PTR                INT_PTR

#define SEIPS_WRAP          0x0001
#ifdef DEBUG
#define SEIPS_NOSCROLL      0x0002      // Flag is used only in DEBUG
#endif

#define DT_LV       (DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_EDITCONTROL)
#define DT_LVWRAP   (DT_CENTER | DT_WORDBREAK | DT_NOPREFIX | DT_EDITCONTROL)
#define CCHLABELMAX MAX_PATH  // BUGBUG dangerous???
#define Edit_GetText(hwndCtl, lpch, cchMax)     GetWindowText((hwndCtl), (lpch), (cchMax))
#define SelectFont(hdc, hfont)  ((HFONT)SelectObject((hdc), (HGDIOBJ)(HFONT)(hfont)))
#define Edit_LimitText(hwndCtl, cchMax)         ((void)SNDMSG((hwndCtl), EM_LIMITTEXT, (WPARAM)(cchMax), 0L))
#define Edit_SetSel(hwndCtl, ichStart, ichEnd)  ((void)SNDMSG((hwndCtl), EM_SETSEL, (ichStart), (ichEnd)))
#define FORWARD_WM_SETFONT(hwnd, hfont, fRedraw, fn) \
	(void)(fn)((hwnd), WM_SETFONT, (WPARAM)(HFONT)(hfont), (LPARAM)(BOOL)(fRedraw))
#define     SubclassWindow(hwnd, lpfn)       \
	((WNDPROC)SetWindowLongPtr((hwnd), GWLP_WNDPROC, (LPARAM)(WNDPROC)(lpfn)))

#define     GetWindowID(hwnd)            GetDlgCtrlID(hwnd)
#define SSW_EX_IMMEDIATE        0x00020000

#ifdef NONAMELESSUNION
#define DUMMYUNION_MEMBER(member)   DUMMYUNIONNAME.member
#define DUMMYUNION2_MEMBER(member)  DUMMYUNIONNAME2.member
#define DUMMYUNION3_MEMBER(member)  DUMMYUNIONNAME3.member
#define DUMMYUNION4_MEMBER(member)  DUMMYUNIONNAME4.member
#define DUMMYUNION5_MEMBER(member)  DUMMYUNIONNAME5.member
#else
#define DUMMYUNION_MEMBER(member)    member
#define DUMMYUNION2_MEMBER(member)   member
#define DUMMYUNION3_MEMBER(member)   member
#define DUMMYUNION4_MEMBER(member)   member
#define DUMMYUNION5_MEMBER(member)   member
#endif

#define TVIF_WIN95              0x007F
#define TVIF_ALL                0x00FF

#define NearAlloc(cb)       ((void NEAR*)LocalAlloc(LPTR, (cb)))
#define NearReAlloc(pb, cb) ((void NEAR*)LocalReAlloc((HLOCAL)(pb), (cb), LMEM_MOVEABLE | LMEM_ZEROINIT))
#define NearFree(pb)        (LocalFree((HLOCAL)(pb)) ? FALSE : TRUE)
#define NearSize(pb)        LocalSize(pb)

	//L
#define IncrementSearchFree(pis) ((pis)->pszCharBuf ? free((pis)->pszCharBuf) : 0)

	//////////////////////////////////////////////////////////////////////////
	// mem.h

#define ControlAlloc(hheap, cb)       HeapAlloc((hheap), HEAP_ZERO_MEMORY, (cb))
#define ControlReAlloc(hheap, pb, cb) HeapReAlloc((hheap), HEAP_ZERO_MEMORY, (pb),(cb))
#define ControlFree(hheap, pb)        HeapFree((hheap), 0, (pb))
#define ControlSize(hheap, pb)        HeapSize((hheap), 0, (LPCVOID)(pb))

	BOOL Str_Set(LPTSTR *ppsz, LPCTSTR psz);


	//////////////////////////////////////////////////////////////////////////

	LRESULT WINAPI CCSendNotify(LibUIDK_CONTROLINFO *pci, int code, LPNMHDR pnmhdr);
	LPTSTR CCReturnDispInfoText(LPTSTR pszSrc, LPTSTR pszDest, UINT cchDest);
	HFONT CCGetHotFont(HFONT hFont, HFONT *phFontHot);
	DWORD NEAR PASCAL CICustomDrawNotify(LibUIDK_LPCONTROLINFO lpci, DWORD dwStage, LPNMCUSTOMDRAW lpnmcd);
	HBITMAP FAR PASCAL CreateColorBitmap(int cx, int cy);
	HBITMAP FAR PASCAL CreateMonoBitmap(int cx, int cy);
	void FillRectClr(HDC hdc, LPRECT prc, COLORREF clr);

	LPSTR ProduceAFromW(UINT uiCodePage, LPCWSTR psz);
	BOOL InOutWtoA(LibUIDK_CONTROLINFO *pci, THUNKSTATE *pts, LPWSTR *ppsz, DWORD cchTextMax);
	LPWSTR ProduceWFromA(UINT uiCodePage, LPCSTR psz);
	void StringBufferAtoW(UINT uiCodePage, LPVOID pvOrgPtr, DWORD dwOrgSize, CHAR **ppszText);
	void InOutAtoW(LibUIDK_CONTROLINFO *pci, THUNKSTATE *pts, LPSTR *ppsz);
	BOOL ThunkToolTipTextAtoW(LPTOOLTIPTEXTA lpTttA, LPTOOLTIPTEXTW lpTttW, UINT uiCodePage);
	LONG GetMessagePosClient(HWND hwnd, LPPOINT ppt);
	void MyNotifyWinEvent(UINT event, HWND hwnd, LONG idContainer, LONG_PTR idChild);
#ifndef WINNT
	HANDLE InitSharedHeap(void);
#endif
	__inline HANDLE GetSharedHeapHandle(void);
	void *WINAPI Alloc(long cb);
	void *WINAPI ReAlloc(void *pb, long cb);
	BOOL FAR PASCAL IncrementSearchString(PISEARCHINFO pis, UINT ch, LPTSTR FAR *lplpstr);
	BOOL FAR PASCAL SameChars(LPTSTR lpsz, TCHAR c);
	void FAR PASCAL IncrementSearchBeep(PISEARCHINFO pis);
	UINT CCSwapKeys(WPARAM wParam, UINT vk1, UINT vk2);
	UINT RTLSwapLeftRightArrows(LibUIDK_CONTROLINFO *pci, WPARAM wParam);
	UINT GetCodePageForFont(HFONT hFont);
	void CCSetInfoTipWidth(HWND hwndOwner, HWND hwndToolTips);
	void FAR PASCAL RelayToToolTips(HWND hwndToolTips, HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
	//
	//
	//  WindowLong accessor macros and other Win64 niceness
	//

	__inline void *GetWindowPtr(HWND hWnd, int nIndex)
	{
		return (void *)GetWindowLongPtr(hWnd, nIndex);
	}

	__inline void *SetWindowPtr(HWND hWnd, int nIndex, void *p)
	{
		return (void *)SetWindowLongPtr(hWnd, nIndex, (LONG_PTR)p);
	}
#ifdef UNICODE
	int FAR PASCAL GetIncrementSearchStringA(PISEARCHINFO pis, UINT uiCodePage, LPSTR lpsz);
#endif
	int FAR PASCAL GetIncrementSearchString(PISEARCHINFO pis, LPTSTR lpsz);
	STDAPI_(HCURSOR) LoadHandCursor(DWORD dwRes);
	LRESULT FAR PASCAL CIHandleNotifyFormat(LibUIDK_LPCONTROLINFO lpci, LPARAM lParam);
	BOOL CCWndProc(LibUIDK_CONTROLINFO *pci, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *plres);
	void FAR PASCAL SetEditInPlaceSize(HWND hwndEdit, RECT FAR *prc, HFONT hFont, UINT seips);
	HWND FAR PASCAL CreateEditInPlaceWindow(HWND hwnd, LPCTSTR lpText, int cbText, LONG style, HFONT hFont);
	void FAR PASCAL RescrollEditWindow(HWND hwndEdit);
	int SmoothScrollWindow(PSMOOTHSCROLLINFO psi);
	DWORD CIFakeCustomDrawNotify(LibUIDK_LPCONTROLINFO lpci, DWORD dwStage, LPNMCUSTOMDRAW lpnmcd);
	HIMAGELIST CreateCheckBoxImagelist(HIMAGELIST himl, BOOL fTree, BOOL fUseColorKey, BOOL fMirror);
	void PASCAL DrawInsertMark(HDC hdc, LPRECT prc, BOOL fHorizMode, COLORREF clr);

	//////////////////////////////////////////////////////////////////////////
	// ctlspriv.h
	void FAR PASCAL CIInitialize(LibUIDK_LPCONTROLINFO lpci, HWND hwnd, LPCREATESTRUCT lpcs);

	//////////////////////////////////////////////////////////////////////////
	// commctrl.w
	BOOL WINAPI MirrorIcon(HICON *phiconSmall, HICON *phiconLarge);


}
