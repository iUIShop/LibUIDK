#include "StdAfx.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL g_fAnimate;
BOOL g_fSmoothScroll;

int LibUIDK::g_cxEdge = GetSystemMetrics(SM_CXEDGE);
int LibUIDK::g_cyEdge = GetSystemMetrics(SM_CYEDGE);
int LibUIDK::g_cxBorder = GetSystemMetrics(SM_CXBORDER);
int LibUIDK::g_cyBorder = GetSystemMetrics(SM_CYBORDER);
int LibUIDK::g_cxScreen;
int LibUIDK::g_cyScreen;
int LibUIDK::g_cxFrame;
int LibUIDK::g_cyFrame;
int LibUIDK::g_cxVScroll;
int LibUIDK::g_cyHScroll;
int LibUIDK::g_cxIcon;
int LibUIDK::g_cyIcon;
int LibUIDK::g_cxSmIcon;
int LibUIDK::g_cySmIcon;
int LibUIDK::g_cxIconSpacing = GetSystemMetrics(SM_CXICONSPACING);
int LibUIDK::g_cyIconSpacing = GetSystemMetrics(SM_CYICONSPACING);
int LibUIDK::g_cxIconMargin, g_cyIconMargin;
int LibUIDK::g_cyLabelSpace;
int LibUIDK::g_cxLabelMargin = LibUIDK::g_cxEdge;
int LibUIDK::g_cxDoubleClk = GetSystemMetrics(SM_CXDOUBLECLK);
int LibUIDK::g_cyDoubleClk = GetSystemMetrics(SM_CYDOUBLECLK);
int LibUIDK::g_cxScrollbar;
int LibUIDK::g_cyScrollbar;
int LibUIDK::g_fDragFullWindows;

UINT LibUIDK::g_uDragImages = RegisterWindowMessage(DI_GETDRAGIMAGE);
UINT LibUIDK::g_msgMSWheel = RegisterWindowMessage(MSH_MOUSEWHEEL);
UINT LibUIDK::g_ucScrollLines = 3;                        /* default */

COLORREF LibUIDK::g_clrWindow;
COLORREF LibUIDK::g_clrWindowText;
COLORREF LibUIDK::g_clrWindowFrame;
COLORREF LibUIDK::g_clrGrayText;
COLORREF LibUIDK::g_clrBtnText;
COLORREF LibUIDK::g_clrBtnFace;
COLORREF LibUIDK::g_clrBtnShadow;
COLORREF LibUIDK::g_clrBtnHighlight;
COLORREF LibUIDK::g_clrHighlight;
COLORREF LibUIDK::g_clrHighlightText;
COLORREF LibUIDK::g_clrInfoText;
COLORREF LibUIDK::g_clrInfoBk;
COLORREF LibUIDK::g_clr3DDkShadow;
COLORREF LibUIDK::g_clr3DLight;

HBRUSH LibUIDK::g_hbrGrayText;
HBRUSH LibUIDK::g_hbrWindow;
HBRUSH LibUIDK::g_hbrWindowText;
HBRUSH LibUIDK::g_hbrWindowFrame;
HBRUSH LibUIDK::g_hbrBtnFace;
HBRUSH LibUIDK::g_hbrBtnHighlight;
HBRUSH LibUIDK::g_hbrBtnShadow;
HBRUSH LibUIDK::g_hbrHighlight;


DWORD  g_dwHoverSelectTimeout;

HFONT LibUIDK::g_hfontSystem;
int LibUIDK::gcWheelDelta;
const TCHAR c_szSpace[] = TEXT(" ");
const TCHAR LibUIDK::c_szSToolTipsClass[] = TOOLTIPS_CLASS;

#ifndef WINNT
HANDLE g_hSharedHeap = NULL;
#endif

LRESULT WINAPI LibUIDK::CCSendNotify(LibUIDK_CONTROLINFO *pci, int code, LPNMHDR pnmhdr)
{
	NMHDR nmhdr;
	LONG_PTR id;
#ifdef UNICODE
	THUNKSTATE ts = { 0 };
#define pvThunk1 ts.ts_pvThunk1
#define pvThunk2 ts.ts_pvThunk2
#define dwThunkSize ts.ts_dwThunkSize
	LRESULT lRet;
	BOOL  bSet = FALSE;
#endif
	HWND hwndParent = pci->hwndParent;
	DWORD dwParentPid;

	// -1 means Requery on each notify
	if (hwndParent == (HWND) - 1)
	{
		hwndParent = GetParent(pci->hwnd);
	}

	// unlikely but it can technically happen -- avoid the rips
	if (hwndParent == NULL)
	{
		return 0;
	}

	//
	// If pci->hwnd is -1, then a WM_NOTIFY is being forwared
	// from one control to a parent.  EG:  Tooltips sent
	// a WM_NOTIFY to toolbar, and toolbar is forwarding it
	// to the real parent window.
	//

	if (pci->hwnd != (HWND) - 1)
	{

		//
		// If this is a child then get its ID.  We need to go out of our way to
		// avoid calling GetDlgCtrlID on toplevel windows since it will return
		// a pseudo-random number (those of you who know what this number is
		// keep quiet).  Anyway it's kinda hard to figure this out in Windows
		// because of the following:
		//
		//  - a window can SetWindowLong(GWL_STYLE, WS_CHILD) but this only
		//    does about half the work - hence checking the style is out.
		//  - GetParent will return your OWNER if you are toplevel.
		//  - there is no GetWindow(...GW_HWNDPARENT) to save us.
		//
		// Hence we are stuck with calling GetParent and then checking to see
		// if it lied and gave us the owner instead.  Yuck.
		//
		id = 0;
		if (pci->hwnd)
		{
			HWND hwndParent = GetParent(pci->hwnd);

			if (hwndParent && (hwndParent != GetWindow(pci->hwnd, GW_OWNER)))
			{
				id = GetDlgCtrlID(pci->hwnd);
			}
		}

		if (!pnmhdr)
		{
			pnmhdr = &nmhdr;
		}

		pnmhdr->hwndFrom = pci->hwnd;
		pnmhdr->idFrom = id;
		pnmhdr->code = code;
	}
	else
	{

		id = pnmhdr->idFrom;
		code = pnmhdr->code;
	}


	// OLE in its massively componentized world sometimes creates
	// a control whose parent belongs to another process.  (For example,
	// when there is a local server embedding.)  WM_NOTIFY
	// messages can't cross process boundaries, so stop the message
	// from going there lest we fault the recipient.
	if (!GetWindowThreadProcessId(hwndParent, &dwParentPid) ||
		dwParentPid != GetCurrentProcessId())
	{
		//L TraceMsg(TF_WARNING, "nf: Not sending WM_NOTIFY %08x across processes", code);
		return 0;
	}

#ifdef NEED_WOWGETNOTIFYSIZE_HELPER
	ASSERT(code >= 0 || WOWGetNotifySize(code));
#endif // NEED_WOWGETNOTIFYSIZE_HELPER

#ifdef UNICODE
	/*
	 * All the thunking for Notify Messages happens here
	 */
	if (!pci->bUnicode)
	{
		BOOL fThunked = TRUE;
		switch (code)
		{
		case LVN_ODFINDITEMW:
			pnmhdr->code = LVN_ODFINDITEMA;
			goto ThunkLV_FINDINFO;

		case LVN_INCREMENTALSEARCHW:
			pnmhdr->code = LVN_INCREMENTALSEARCHA;
			goto ThunkLV_FINDINFO;

ThunkLV_FINDINFO:
			{
				LV_FINDINFO *plvfi;

				// Hack Alert!  This code assumes that all fields of LV_FINDINFOA and
				// LV_FINDINFOW are exactly the same except for the string pointers.
				COMPILETIME_ASSERT(sizeof(LV_FINDINFOA) == sizeof(LV_FINDINFOW));

				// Since WCHARs are bigger than char, we will just use the
				// wchar buffer to hold the chars, and not worry about the extra
				// room at the end.
				COMPILETIME_ASSERT(sizeof(WCHAR) >= sizeof(char));

				plvfi = &((PNM_FINDITEM)pnmhdr)->lvfi;
				if (plvfi->flags & (LVFI_STRING | LVFI_PARTIAL | LVFI_SUBSTRING))
				{
					pvThunk1 = (PVOID)plvfi->psz;
					dwThunkSize = lstrlen((LPCWSTR)pvThunk1) + 1;
					plvfi->psz = (LPWSTR)ProduceAFromW(pci->uiCodePage, plvfi->psz);
				}
			}
			break;

		case LVN_GETDISPINFOW:
		{
			LV_ITEMW *pitem;

			pnmhdr->code = LVN_GETDISPINFOA;

			// Hack Alert!  This code assumes that all fields of LV_DISPINFOA and
			// LV_DISPINFOW are exactly the same except for the string pointers.

			COMPILETIME_ASSERT(sizeof(LV_DISPINFOA) == sizeof(LV_DISPINFOW));

			// Since WCHARs are bigger than char, we will just use the
			// wchar buffer to hold the chars, and not worry about the extra
			// room at the end.
			COMPILETIME_ASSERT(sizeof(WCHAR) >= sizeof(char));

			//
			// Some sleazebag code (shell32.dll) just changes the pszText
			// pointer to point to the name, so capture the original pointer
			// so we can detect this and not smash their data.
			//
			pitem = &(((LV_DISPINFOW *)pnmhdr)->item);
			if (!IsFlagPtr(pitem) && (pitem->mask & LVIF_TEXT) && !IsFlagPtr(pitem->pszText))
			{
				pvThunk1 = pitem->pszText;
				dwThunkSize = pitem->cchTextMax;
			}
			break;
		}


		// LVN_ENDLABELEDIT uses an INOUT parameter, never explicitly
		// documented as such, but it just happened to be that way,
		// and I don't want to take the chance that somebody was relying
		// on it.

		case LVN_ENDLABELEDITW:
			pnmhdr->code = LVN_ENDLABELEDITA;
			goto ThunkLV_DISPINFO;

		case LVN_BEGINLABELEDITW:
			pnmhdr->code = LVN_BEGINLABELEDITA;
			goto ThunkLV_DISPINFO;

		case LVN_SETDISPINFOW:
			pnmhdr->code = LVN_SETDISPINFOA;
			goto ThunkLV_DISPINFO;

		case LVN_GETEMPTYTEXTW:
			pnmhdr->code = LVN_GETEMPTYTEXTA;
			goto ThunkLV_DISPINFO;

ThunkLV_DISPINFO:
			{
				LV_ITEMW *pitem;

				COMPILETIME_ASSERT(sizeof(LV_ITEMA) == sizeof(LV_ITEMW));
				pitem = &(((LV_DISPINFOW *)pnmhdr)->item);

				if (pitem->mask & LVIF_TEXT)
				{
					if (!InOutWtoA(pci, &ts, &pitem->pszText, pitem->cchTextMax))
					{
						return 0;
					}
				}
				break;
			}

		case LVN_GETINFOTIPW:
		{
			NMLVGETINFOTIPW *pgit = (NMLVGETINFOTIPW *)pnmhdr;

			COMPILETIME_ASSERT(sizeof(NMLVGETINFOTIPA) == sizeof(NMLVGETINFOTIPW));
			pnmhdr->code = LVN_GETINFOTIPA;

			if (!InOutWtoA(pci, &ts, &pgit->pszText, pgit->cchTextMax))
			{
				return 0;
			}
		}
		break;


		case TVN_GETINFOTIPW:
		{
			NMTVGETINFOTIPW *pgit = (NMTVGETINFOTIPW *)pnmhdr;

			pnmhdr->code = TVN_GETINFOTIPA;

			pvThunk1 = pgit->pszText;
			dwThunkSize = pgit->cchTextMax;
		}
		break;

		case TBN_GETINFOTIPW:
		{
			NMTBGETINFOTIPW *pgit = (NMTBGETINFOTIPW *)pnmhdr;

			pnmhdr->code = TBN_GETINFOTIPA;

			pvThunk1 = pgit->pszText;
			dwThunkSize = pgit->cchTextMax;
		}
		break;

		case TVN_SELCHANGINGW:
			pnmhdr->code = TVN_SELCHANGINGA;
			bSet = TRUE;
		// fall through

		case TVN_SELCHANGEDW:
			if (!bSet)
			{
				pnmhdr->code = TVN_SELCHANGEDA;
				bSet = TRUE;
			}

		/*
		 * These msgs have a NM_TREEVIEW with both TV_ITEMs filled in
		 *
		 * FALL THROUGH TO TVN_DELETEITEM to thunk itemOld then go on for
		 * the other structure.
		 */

		// fall through

		case TVN_DELETEITEMW:
		{
			/*
			 * This message has a NM_TREEVIEW in lParam with itemOld filled in
			 */
			LPTV_ITEMW pitem;

			if (!bSet)
			{
				pnmhdr->code = TVN_DELETEITEMA;
				bSet = TRUE;
			}

			pitem = &(((LPNM_TREEVIEWW)pnmhdr)->itemOld);

			// thunk itemOld
			if ((pitem->mask & TVIF_TEXT) && !IsFlagPtr(pitem->pszText))
			{
				pvThunk2 = pitem->pszText;
				pitem->pszText = (LPWSTR)ProduceAFromW(pci->uiCodePage, (LPCWSTR)pvThunk2);
			}

			// if this is deleteitem then we are done
			if (pnmhdr->code == TVN_DELETEITEMA)
			{
				break;
			}

			/* FALL THROUGH TO TVN_ITEMEXPANDING to thunk itemNew */
		}
		// fall through

		case TVN_ITEMEXPANDINGW:
			if (!bSet)
			{
				pnmhdr->code = TVN_ITEMEXPANDINGA;
				bSet = TRUE;
			}
		// fall through

		case TVN_ITEMEXPANDEDW:
			if (!bSet)
			{
				pnmhdr->code = TVN_ITEMEXPANDEDA;
				bSet = TRUE;
			}
		// fall through

		case TVN_BEGINDRAGW:
			if (!bSet)
			{
				pnmhdr->code = TVN_BEGINDRAGA;
				bSet = TRUE;
			}
		// fall through

		case TVN_BEGINRDRAGW:
		{
			/* these msgs have a NM_TREEVIEW with itemNew TV_ITEM filled in */
			LPTV_ITEMW pitem;

			if (!bSet)
			{
				pnmhdr->code = TVN_BEGINRDRAGA;
			}

			pitem = &(((LPNM_TREEVIEWW)pnmhdr)->itemNew);

			if ((pitem->mask & TVIF_TEXT) && !IsFlagPtr(pitem->pszText))
			{
				pvThunk1 = pitem->pszText;
				pitem->pszText = (LPWSTR)ProduceAFromW(pci->uiCodePage, (LPCWSTR)pvThunk1);
			}

			break;
		}

		case TVN_SETDISPINFOW:
			pnmhdr->code = TVN_SETDISPINFOA;
			goto ThunkTV_DISPINFO;

		case TVN_BEGINLABELEDITW:
			pnmhdr->code = TVN_BEGINLABELEDITA;
			goto ThunkTV_DISPINFO;


		// TVN_ENDLABELEDIT uses an INOUT parameter, never explicitly
		// documented as such, but it just happened to be that way,
		// and I don't want to take the chance that somebody was relying
		// on it.

		case TVN_ENDLABELEDITW:
			pnmhdr->code = TVN_ENDLABELEDITA;
			goto ThunkTV_DISPINFO;

ThunkTV_DISPINFO:
			{
				/*
				 * All these messages have a TV_DISPINFO in lParam.
				 */

				LPTV_ITEMW pitem;

				pitem = &(((TV_DISPINFOW *)pnmhdr)->item);

				if (pitem->mask & TVIF_TEXT)
				{
					if (!InOutWtoA(pci, &ts, &pitem->pszText, pitem->cchTextMax))
					{
						return 0;
					}
				}
				break;
			}

#if !defined(UNIX) || defined(ANSI_SHELL32_ON_UNIX)
		/* UNIX shell32 TVN_GETDISPINFOA TVN_GETDISPINFOW does the same thing */
		case TVN_GETDISPINFOW:
		{
			/*
			 * All these messages have a TV_DISPINFO in lParam.
			 */
			LPTV_ITEMW pitem;

			pnmhdr->code = TVN_GETDISPINFOA;

			pitem = &(((TV_DISPINFOW *)pnmhdr)->item);

			if ((pitem->mask & TVIF_TEXT) && !IsFlagPtr(pitem->pszText) && pitem->cchTextMax)
			{
				pvThunk1 = pitem->pszText;
				dwThunkSize = pitem->cchTextMax;
				pvThunk2 = LocalAlloc(LPTR, pitem->cchTextMax * sizeof(char));
				pitem->pszText = (LPWSTR)pvThunk2;
				pitem->pszText[0] = TEXT('\0');
			}

			break;
		}
#endif

		case HDN_ITEMCHANGINGW:
			pnmhdr->code = HDN_ITEMCHANGINGA;
			bSet = TRUE;
		// fall through

		case HDN_ITEMCHANGEDW:
			if (!bSet)
			{
				pnmhdr->code = HDN_ITEMCHANGEDA;
				bSet = TRUE;
			}
		// fall through

		case HDN_ITEMCLICKW:
			if (!bSet)
			{
				pnmhdr->code = HDN_ITEMCLICKA;
				bSet = TRUE;
			}
		// fall through

		case HDN_ITEMDBLCLICKW:
			if (!bSet)
			{
				pnmhdr->code = HDN_ITEMDBLCLICKA;
				bSet = TRUE;
			}
		// fall through

		case HDN_DIVIDERDBLCLICKW:
			if (!bSet)
			{
				pnmhdr->code = HDN_DIVIDERDBLCLICKA;
				bSet = TRUE;
			}
		// fall through

		case HDN_BEGINTRACKW:
			if (!bSet)
			{
				pnmhdr->code = HDN_BEGINTRACKA;
				bSet = TRUE;
			}
		// fall through

		case HDN_ENDTRACKW:
			if (!bSet)
			{
				pnmhdr->code = HDN_ENDTRACKA;
				bSet = TRUE;
			}
		// fall through

		case HDN_TRACKW:
		{
			HD_ITEMW *pitem;

			if (!bSet)
			{
				pnmhdr->code = HDN_TRACKA;
			}

			pitem = ((HD_NOTIFY *)pnmhdr)->pitem;

			if (!IsFlagPtr(pitem) && (pitem->mask & HDI_TEXT) && !IsFlagPtr(pitem->pszText))
			{
				pvThunk1 = pitem->pszText;
				dwThunkSize = pitem->cchTextMax;
				pitem->pszText = (LPWSTR)ProduceAFromW(pci->uiCodePage, (LPCWSTR)pvThunk1);
			}


			if (!IsFlagPtr(pitem) && (pitem->mask & HDI_FILTER) && pitem->pvFilter)
			{
				if (!(pitem->type & HDFT_HASNOVALUE) &&
					((pitem->type & HDFT_ISMASK) == HDFT_ISSTRING))
				{
					LPHD_TEXTFILTER ptextFilter = (LPHD_TEXTFILTER)pitem->pvFilter;
					pvThunk2 = ptextFilter->pszText;
					dwThunkSize = ptextFilter->cchTextMax;
					ptextFilter->pszText = (LPWSTR)ProduceAFromW(pci->uiCodePage, (LPCWSTR)pvThunk2);
				}
			}


			break;
		}

		case CBEN_ENDEDITW:
		{
			LPNMCBEENDEDITW peew = (LPNMCBEENDEDITW) pnmhdr;
			LPNMCBEENDEDITA peea = (LPNMCBEENDEDITA)LocalAlloc(LPTR, sizeof(NMCBEENDEDITA));

			if (!peea)
			{
				return 0;
			}

			peea->hdr  = peew->hdr;
			peea->hdr.code = CBEN_ENDEDITA;

			peea->fChanged = peew->fChanged;
			peea->iNewSelection = peew->iNewSelection;
			peea->iWhy = peew->iWhy;
			ConvertWToAN(pci->uiCodePage, peea->szText, ARRAYSIZE(peea->szText),
				peew->szText, -1);

			pvThunk1 = pnmhdr;
			pnmhdr = &peea->hdr;
			ASSERT((LPVOID)pnmhdr == (LPVOID)peea);
			break;
		}

		case CBEN_DRAGBEGINW:
		{
			LPNMCBEDRAGBEGINW pdbw = (LPNMCBEDRAGBEGINW) pnmhdr;
			LPNMCBEDRAGBEGINA pdba = (LPNMCBEDRAGBEGINA)LocalAlloc(LPTR, sizeof(NMCBEDRAGBEGINA));

			if (!pdba)
			{
				return 0;
			}

			pdba->hdr  = pdbw->hdr;
			pdba->hdr.code = CBEN_DRAGBEGINA;
			pdba->iItemid = pdbw->iItemid;
			ConvertWToAN(pci->uiCodePage, pdba->szText, ARRAYSIZE(pdba->szText),
				pdbw->szText, -1);

			pvThunk1 = pnmhdr;
			pnmhdr = &pdba->hdr;
			ASSERT((LPVOID)pnmhdr == (LPVOID)pdba);
			break;
		}


		case CBEN_GETDISPINFOW:
		{
			PNMCOMBOBOXEXW pnmcbe = (PNMCOMBOBOXEXW)pnmhdr;

			pnmhdr->code = CBEN_GETDISPINFOA;

			if (pnmcbe->ceItem.mask  & CBEIF_TEXT
				&& !IsFlagPtr(pnmcbe->ceItem.pszText) && pnmcbe->ceItem.cchTextMax)
			{
				pvThunk1 = pnmcbe->ceItem.pszText;
				dwThunkSize = pnmcbe->ceItem.cchTextMax;
				pvThunk2 = LocalAlloc(LPTR, pnmcbe->ceItem.cchTextMax * sizeof(char));
				pnmcbe->ceItem.pszText = (LPWSTR)pvThunk2;
				pnmcbe->ceItem.pszText[0] = TEXT('\0');
			}

			break;
		}

		case HDN_GETDISPINFOW:
		{
			LPNMHDDISPINFOW pHDDispInfoW;

			pnmhdr->code = HDN_GETDISPINFOA;

			pHDDispInfoW = (LPNMHDDISPINFOW) pnmhdr;

			pvThunk1 = pHDDispInfoW->pszText;
			dwThunkSize = pHDDispInfoW->cchTextMax;
			pHDDispInfoW->pszText = (LPWSTR)LocalAlloc(LPTR, pHDDispInfoW->cchTextMax * sizeof(char));

			if (!pHDDispInfoW->pszText)
			{
				pHDDispInfoW->pszText = (LPWSTR) pvThunk1;
				break;
			}

			WideCharToMultiByte(pci->uiCodePage, 0, (LPWSTR)pvThunk1, -1,
				(LPSTR)pHDDispInfoW->pszText, pHDDispInfoW->cchTextMax,
				NULL, NULL);
			break;
		}


		case TBN_GETBUTTONINFOW:
		{
			LPTBNOTIFYW pTBNW;

			pnmhdr->code = TBN_GETBUTTONINFOA;

			pTBNW = (LPTBNOTIFYW)pnmhdr;

			pvThunk1 = pTBNW->pszText;
			dwThunkSize = pTBNW->cchText;
			pvThunk2 = LocalAlloc(LPTR, pTBNW->cchText * sizeof(char));

			if (!pvThunk2)
			{
				break;
			}
			pTBNW->pszText = (LPWSTR)pvThunk2;

			WideCharToMultiByte(pci->uiCodePage, 0, (LPWSTR)pvThunk1, -1,
				(LPSTR)pTBNW->pszText, pTBNW->cchText,
				NULL, NULL);

		}
		break;

		case TTN_NEEDTEXTW:
		{
			LPTOOLTIPTEXTA lpTTTA;
			LPTOOLTIPTEXTW lpTTTW = (LPTOOLTIPTEXTW) pnmhdr;

			lpTTTA = (LPTOOLTIPTEXTA)LocalAlloc(LPTR, sizeof(TOOLTIPTEXTA));

			if (!lpTTTA)
			{
				return 0;
			}

			lpTTTA->hdr = lpTTTW->hdr;
			lpTTTA->hdr.code = TTN_NEEDTEXTA;

			lpTTTA->lpszText = lpTTTA->szText;
			lpTTTA->hinst    = lpTTTW->hinst;
			lpTTTA->uFlags   = lpTTTW->uFlags;
			lpTTTA->lParam   = lpTTTW->lParam;

			WideCharToMultiByte(pci->uiCodePage, 0, lpTTTW->szText, -1, lpTTTA->szText, ARRAYSIZE(lpTTTA->szText), NULL, NULL);
			pvThunk1 = pnmhdr;
			pnmhdr = (NMHDR FAR *)lpTTTA;
		}
		break;

		case DTN_USERSTRINGW:
		{
			LPNMDATETIMESTRINGW lpDateTimeString = (LPNMDATETIMESTRINGW) pnmhdr;

			pnmhdr->code = DTN_USERSTRINGA;

			pvThunk1 = ProduceAFromW(pci->uiCodePage, lpDateTimeString->pszUserString);
			lpDateTimeString->pszUserString = (LPWSTR) pvThunk1;
		}
		break;

		case DTN_WMKEYDOWNW:
		{
			LPNMDATETIMEWMKEYDOWNW lpDateTimeWMKeyDown =
				(LPNMDATETIMEWMKEYDOWNW) pnmhdr;

			pnmhdr->code = DTN_WMKEYDOWNA;

			pvThunk1 = ProduceAFromW(pci->uiCodePage, lpDateTimeWMKeyDown->pszFormat);
			lpDateTimeWMKeyDown->pszFormat = (LPWSTR) pvThunk1;
		}
		break;

		case DTN_FORMATQUERYW:
		{
			LPNMDATETIMEFORMATQUERYW lpDateTimeFormatQuery =
				(LPNMDATETIMEFORMATQUERYW) pnmhdr;

			pnmhdr->code = DTN_FORMATQUERYA;

			pvThunk1 = ProduceAFromW(pci->uiCodePage, lpDateTimeFormatQuery->pszFormat);
			lpDateTimeFormatQuery->pszFormat = (LPWSTR) pvThunk1;
		}
		break;

		case DTN_FORMATW:
		{
			LPNMDATETIMEFORMATW lpDateTimeFormat =
				(LPNMDATETIMEFORMATW) pnmhdr;

			pnmhdr->code = DTN_FORMATA;

			pvThunk1 = ProduceAFromW(pci->uiCodePage, lpDateTimeFormat->pszFormat);
			lpDateTimeFormat->pszFormat = (LPWSTR) pvThunk1;
		}
		break;

		default:
			fThunked = FALSE;
			break;
		}

#ifdef NEED_WOWGETNOTIFYSIZE_HELPER
		ASSERT(code >= 0 || WOWGetNotifySize(code));
#endif // NEED_WOWGETNOTIFYSIZE_HELPER

		lRet = SendMessage(hwndParent, WM_NOTIFY, (WPARAM)id, (LPARAM)pnmhdr);

		/*
		 * All the thunking for Notify Messages happens here
		 */
		if (fThunked)
		{
			switch (pnmhdr->code)
			{
			case LVN_ODFINDITEMA:
			case LVN_INCREMENTALSEARCHA:
			{
				LV_FINDINFO *plvfi = &((PNM_FINDITEM)pnmhdr)->lvfi;
				if (pvThunk1)
				{
					FreeProducedString((LPWSTR)plvfi->psz);
					plvfi->psz = (LPCWSTR)pvThunk1;
				}
			}
			break;

			case LVN_GETDISPINFOA:
			{
				LV_ITEMA *pitem = &(((LV_DISPINFOA *)pnmhdr)->item);

				// BUGBUG what if pointer is to large buffer?
				if (!IsFlagPtr(pitem) && (pitem->mask & LVIF_TEXT) && !IsFlagPtr(pitem->pszText))
				{
					StringBufferAtoW(pci->uiCodePage, pvThunk1, dwThunkSize, &pitem->pszText);
				}
			}
			break;

			case LVN_ENDLABELEDITA:
			case LVN_BEGINLABELEDITA:
			case LVN_SETDISPINFOA:
			case LVN_GETEMPTYTEXTA:
			{
				LV_ITEMA *pitem = &(((LV_DISPINFOA *)pnmhdr)->item);
				InOutAtoW(pci, &ts, &pitem->pszText);
			}
			break;

			case LVN_GETINFOTIPA:
			{
				NMLVGETINFOTIPA *pgit = (NMLVGETINFOTIPA *)pnmhdr;
				InOutAtoW(pci, &ts, &pgit->pszText);
			}
			break;

			case TVN_GETINFOTIPA:
			{
				NMTVGETINFOTIPA *pgit = (NMTVGETINFOTIPA *)pnmhdr;
				StringBufferAtoW(pci->uiCodePage, pvThunk1, dwThunkSize, &pgit->pszText);
			}
			break;

			case TBN_GETINFOTIPA:
			{
				NMTBGETINFOTIPA *pgit = (NMTBGETINFOTIPA *)pnmhdr;
				StringBufferAtoW(pci->uiCodePage, pvThunk1, dwThunkSize, &pgit->pszText);
			}
			break;
			case TVN_SELCHANGINGA:
			case TVN_SELCHANGEDA:
			case TVN_DELETEITEMA:
			{
				LPTV_ITEMW pitem;

				if (!IsFlagPtr(pvThunk2))
				{
					pitem = &(((LPNM_TREEVIEWW)pnmhdr)->itemOld);

					FreeProducedString(pitem->pszText);
					pitem->pszText = (LPWSTR)pvThunk2;
				}

				// if this is delitem, then we are done
				if (code == TVN_DELETEITEM)
				{
					break;
				}

				/* FALL THROUGH TO TVN_ITEMEXPANDING to unthunk itemNew */
			}
			// fall through

			case TVN_ITEMEXPANDINGA:
			case TVN_ITEMEXPANDEDA:
			case TVN_BEGINDRAGA:
			case TVN_BEGINRDRAGA:
			{
				/* these msgs have a NM_TREEVIEW with itemNew TV_ITEM filled in */
				LPTV_ITEMW pitem;

				if (!IsFlagPtr(pvThunk1))
				{
					pitem = &(((LPNM_TREEVIEWW)pnmhdr)->itemNew);

					FreeProducedString(pitem->pszText);
					pitem->pszText = (LPWSTR)pvThunk1;
				}

				break;
			}

			case TVN_SETDISPINFOA:
			case TVN_BEGINLABELEDITA:
			case TVN_ENDLABELEDITA:
			{
				LPTV_ITEMA pitem;
				pitem = &(((TV_DISPINFOA *)pnmhdr)->item);
				InOutAtoW(pci, &ts, &pitem->pszText);
			}
			break;

			case TVN_GETDISPINFOA:
			{
				/*
				 * This message has a TV_DISPINFO in lParam that wass filled in
				 * during the callback and needs to be unthunked.
				 */
				LPTV_ITEMW pitem;

				pitem = &(((TV_DISPINFOW *)pnmhdr)->item);

				if (!IsFlagPtr(pvThunk1) && (pitem->mask & TVIF_TEXT) && !IsFlagPtr(pitem->pszText))
				{
					ConvertAToWN(pci->uiCodePage, (LPWSTR)pvThunk1, dwThunkSize, (LPSTR)pitem->pszText, -1);
					pitem->pszText = (LPWSTR)pvThunk1;
					LocalFree(pvThunk2);
				}

				break;
			}

			case HDN_ITEMCHANGINGA:
			case HDN_ITEMCHANGEDA:
			case HDN_ITEMCLICKA:
			case HDN_ITEMDBLCLICKA:
			case HDN_DIVIDERDBLCLICKA:
			case HDN_BEGINTRACKA:
			case HDN_ENDTRACKA:
			case HDN_TRACKA:
			{
				HD_ITEMW *pitem;

				pitem = ((HD_NOTIFY *)pnmhdr)->pitem;

				if (!IsFlagPtr(pitem) && (pitem->mask & HDI_TEXT) && !IsFlagPtr(pvThunk1))
				{
					ConvertAToWN(pci->uiCodePage, (LPTSTR)pvThunk1, dwThunkSize, (LPSTR)(pitem->pszText), -1);

					FreeProducedString(pitem->pszText);
					pitem->pszText = (LPTSTR)pvThunk1;
				}

				if (!IsFlagPtr(pitem) && (pitem->mask & HDI_FILTER) && pitem->pvFilter && pvThunk2)
				{
					if (!(pitem->type & HDFT_HASNOVALUE) &&
						((pitem->type & HDFT_ISMASK) == HDFT_ISSTRING))
					{
						LPHD_TEXTFILTER ptextFilter = (LPHD_TEXTFILTER)pitem->pvFilter;
						ConvertAToWN(pci->uiCodePage, (LPTSTR)pvThunk2, dwThunkSize, (LPSTR)(ptextFilter->pszText), -1);
						FreeProducedString(ptextFilter->pszText);
						ptextFilter->pszText = (LPTSTR)pvThunk2;
					}
				}

				break;
			}

			case CBEN_ENDEDITA:
			{
				LPNMCBEENDEDITW peew = (LPNMCBEENDEDITW) pvThunk1;
				LPNMCBEENDEDITA peea = (LPNMCBEENDEDITA) pnmhdr;

				// Don't unthunk the string since that destroys unicode round-trip
				// and the client shouldn't be modifying it anyway.
				// ConvertAToWN(pci->uiCodePage, peew->szText, ARRAYSIZE(peew->szText),
				//              peea->szText, -1);
				LocalFree(peea);
			}
			break;

			case CBEN_DRAGBEGINA:
			{
				LPNMCBEDRAGBEGINW pdbw = (LPNMCBEDRAGBEGINW) pvThunk1;
				LPNMCBEDRAGBEGINA pdba = (LPNMCBEDRAGBEGINA) pnmhdr;

				// Don't unthunk the string since that destroys unicode round-trip
				// and the client shouldn't be modifying it anyway.
				// ConvertAToWN(pci->uiCodePage, pdbw->szText, ARRAYSIZE(pdbw->szText),
				//              pdba->szText, -1);
				LocalFree(pdba);
			}
			break;

			case CBEN_GETDISPINFOA:
			{
				PNMCOMBOBOXEXW pnmcbeW;

				pnmcbeW = (PNMCOMBOBOXEXW)pnmhdr;
				ConvertAToWN(pci->uiCodePage, (LPTSTR)pvThunk1, dwThunkSize, (LPSTR)(pnmcbeW->ceItem.pszText), -1);

				if (pvThunk2)
				{
					LocalFree(pvThunk2);
				}
				pnmcbeW->ceItem.pszText = (LPTSTR)pvThunk1;

			}
			break;


			case HDN_GETDISPINFOA:
			{
				LPNMHDDISPINFOW pHDDispInfoW;

				pHDDispInfoW = (LPNMHDDISPINFOW)pnmhdr;
				ConvertAToWN(pci->uiCodePage, (LPTSTR)pvThunk1, dwThunkSize, (LPSTR)(pHDDispInfoW->pszText), -1);

				LocalFree(pHDDispInfoW->pszText);
				pHDDispInfoW->pszText = (LPTSTR)pvThunk1;

			}
			break;

			case TBN_GETBUTTONINFOA:
			{
				LPTBNOTIFYW pTBNW;

				pTBNW = (LPTBNOTIFYW)pnmhdr;
				ConvertAToWN(pci->uiCodePage, (LPTSTR)pvThunk1, dwThunkSize, (LPSTR)(pTBNW->pszText), -1);

				pTBNW->pszText = (LPTSTR)pvThunk1;
				LocalFree(pvThunk2);

			}
			break;


			case TTN_NEEDTEXTA:
			{
				LPTOOLTIPTEXTA lpTTTA = (LPTOOLTIPTEXTA) pnmhdr;
				LPTOOLTIPTEXTW lpTTTW = (LPTOOLTIPTEXTW) pvThunk1;

				ThunkToolTipTextAtoW(lpTTTA, lpTTTW, pci->uiCodePage);
				LocalFree(lpTTTA);
			}
			break;

			case DTN_USERSTRINGA:
			case DTN_WMKEYDOWNA:
			case DTN_FORMATQUERYA:
			{
				FreeProducedString(pvThunk1);
			}
			break;

			case DTN_FORMATA:
			{
				LPNMDATETIMEFORMATA lpDateTimeFormat = (LPNMDATETIMEFORMATA) pnmhdr;

				FreeProducedString(pvThunk1);

				//
				// pszDisplay and szDisplay are special cases.
				//

				if (lpDateTimeFormat->pszDisplay && *lpDateTimeFormat->pszDisplay)
				{

					//
					// if pszDisplay still points at szDisplay then thunk
					// in place.  Otherwise allocate memory and copy the
					// display string.  This buffer will be freeded in monthcal.c
					//

					if (lpDateTimeFormat->pszDisplay == lpDateTimeFormat->szDisplay)
					{
						CHAR szDisplay[64];

						lstrcpynA(szDisplay, lpDateTimeFormat->szDisplay, 64);

						ConvertAToWN(pci->uiCodePage, (LPWSTR)lpDateTimeFormat->szDisplay, 64,
							szDisplay, -1);
					}
					else
					{
						lpDateTimeFormat->pszDisplay =
							(LPSTR) ProduceWFromA(pci->uiCodePage, lpDateTimeFormat->pszDisplay);
					}

				}

			}
			break;

			default:
				/* No thunking needed */
				break;
			}
		}
		return lRet;
	}
	else
#endif
		return (SendMessage(hwndParent, WM_NOTIFY, (WPARAM)id, (LPARAM)pnmhdr));

#undef pvThunk1
#undef pvThunk2
#undef dwThunkSize
}

//
//  New for v5.01:
//
//  Accessibility (and some other callers, sometimes even us) relies on
//  a XXM_GETITEM call filling the buffer and not just redirecting the
//  pointer.  Accessibility is particularly screwed by this because they
//  live outside the process, so the redirected pointer means nothing
//  to them.  Here, we copy the result back into the app buffer and return
//  the raw pointer.  The caller will return the raw pointer back to the
//  app, so the answer is in two places, either the app buffer, or in
//  the raw pointer.
//
//  Usage:
//
//      if (nm.item.mask & LVIF_TEXT)
//          pitem->pszText = CCReturnDispInfoText(nm.item.pszText,
//                              pitem->pszText, pitem->cchTextMax);
//
LPTSTR LibUIDK::CCReturnDispInfoText(LPTSTR pszSrc, LPTSTR pszDest, UINT cchDest)
{
	// Test pszSrc != pszDest first since the common case is that they
	// are equal.
	if (pszSrc != pszDest && !IsFlagPtr(pszSrc) && !IsFlagPtr(pszDest))
	{
		StrCpyN(pszDest, pszSrc, cchDest);
	}
	return pszSrc;
}

// MSDN上说，LocalAlloc速度比较慢，建议用heap function
void *CCLocalReAlloc(void *p, UINT uBytes)
{
	if (uBytes)
	{
		if (p)
		{
			return HeapReAlloc(GetProcessHeap(), 0, p, uBytes); //L LocalReAlloc(p, uBytes, LMEM_MOVEABLE | LMEM_ZEROINIT);
		}
		else
		{
			return HeapAlloc(GetProcessHeap(), 0, uBytes); //L LocalAlloc(LPTR, uBytes);
		}
	}
	else
	{
		if (p)
		{
			HeapFree(GetProcessHeap(), 0, p); //L LocalFree(p);
		}
		return NULL;
	}
}

BOOL LibUIDK::Str_Set(LPTSTR *ppsz, LPCTSTR psz)
{
	if (!psz || (psz == LPSTR_TEXTCALLBACK))
	{
		if (*ppsz)
		{
			if (*ppsz != (LPSTR_TEXTCALLBACK))
			{
				HeapFree(GetProcessHeap(), 0, *ppsz); //L LocalFree(*ppsz);
			}
		}
		*ppsz = (LPTSTR)psz;
	}
	else
	{
		LPTSTR pszNew = *ppsz;
		UINT cbSize = (lstrlen(psz) + 1) * sizeof(TCHAR);

		if (pszNew == LPSTR_TEXTCALLBACK)
		{
			pszNew = NULL;
		}

		pszNew = (LPTSTR)CCLocalReAlloc(pszNew, cbSize);

		if (!pszNew)
		{
			return FALSE;
		}

		lstrcpy(pszNew, psz);
		*ppsz = pszNew;
	}

	return TRUE;
}

HFONT LibUIDK::CCGetHotFont(HFONT hFont, HFONT *phFontHot)
{
	if (!*phFontHot)
	{
		LOGFONT lf;

		// create the underline font
		GetObject(hFont, sizeof(lf), &lf);
#ifndef DONT_UNDERLINE
		lf.lfUnderline = TRUE;
#endif
		*phFontHot = CreateFontIndirect(&lf);
	}
	return *phFontHot;
}

DWORD NEAR PASCAL LibUIDK::CICustomDrawNotify(LibUIDK_LPCONTROLINFO lpci, DWORD dwStage, LPNMCUSTOMDRAW lpnmcd)
{
	DWORD dwRet = CDRF_DODEFAULT;


	// bail if...


	// this is an item notification, but an item notification wasn't asked for
	if ((dwStage & CDDS_ITEM) && !(lpci->dwCustom & CDRF_NOTIFYITEMDRAW))
	{
		return dwRet;
	}

	lpnmcd->dwDrawStage = dwStage;
	dwRet = (DWORD) CCSendNotify(lpci, NM_CUSTOMDRAW, &lpnmcd->hdr);

	// validate the flags
	if (dwRet & ~CDRF_VALIDFLAGS)
	{
		return CDRF_DODEFAULT;
	}

	return dwRet;
}

HBITMAP FAR PASCAL LibUIDK::CreateColorBitmap(int cx, int cy)
{
	HBITMAP hbm;
	HDC hdc;

	hdc = GetDC(NULL);

	//
	// on a multimonitor system with mixed bitdepths
	// always use a 32bit bitmap for our work buffer
	// this will prevent us from losing colors when
	// blting to and from the screen.  this is mainly
	// important for the drag & drop offscreen buffers.
	//
	if (!(GetDeviceCaps(hdc, RASTERCAPS) & RC_PALETTE) &&
		GetSystemMetrics(SM_CMONITORS) > 1 &&
		GetSystemMetrics(SM_SAMEDISPLAYFORMAT) == 0)
	{
		LPVOID p;
#ifndef UNIX
		BITMAPINFO bi = {sizeof(BITMAPINFOHEADER), cx, cy, 1, 32};
#else
		BITMAPINFO bi;
		bi.bmiHeader.biSize     = sizeof(BITMAPINFOHEADER);
		bi.bmiHeader.biWidth    = cx;
		bi.bmiHeader.biHeight   = cy;
		bi.bmiHeader.biPlanes   = 1 ;
		bi.bmiHeader.biBitCount = 32;
#endif
		hbm = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, &p, NULL, 0);
	}
	else
	{
		hbm = CreateCompatibleBitmap(hdc, cx, cy);
	}

	ReleaseDC(NULL, hdc);
	return hbm;
}

HBITMAP FAR PASCAL LibUIDK::CreateMonoBitmap(int cx, int cy)
{
	return CreateBitmap(cx, cy, 1, 1, NULL);
}

void LibUIDK::FillRectClr(HDC hdc, LPRECT prc, COLORREF clr)
{
	COLORREF clrSave = SetBkColor(hdc, clr);
	ExtTextOut(hdc, 0, 0, ETO_OPAQUE, prc, NULL, 0, NULL);
	SetBkColor(hdc, clrSave);
}

/*
 * Creates a buffer for a unicode string, and then copies the ANSI text
 * into it (converting it to unicode in the process)
 *
 * The returned pointer should be freed with LocalFree after use.
 */
LPSTR LibUIDK::ProduceAFromW(UINT uiCodePage, LPCWSTR psz)
{
	LPSTR pszA;
	int cch;

	if (psz == NULL || psz == LPSTR_TEXTCALLBACKW)
	{
		return (LPSTR)psz;
	}

	cch = WideCharToMultiByte(uiCodePage, 0, psz, -1, NULL, 0, NULL, NULL);

	if (cch == 0)
	{
		cch = 1;
	}

	pszA = (LPSTR)LocalAlloc(LMEM_FIXED, cch * sizeof(char));

	if (pszA != NULL)
	{
		if (WideCharToMultiByte(uiCodePage, 0, psz, -1, pszA, cch, NULL, NULL) ==
			FALSE)
		{
			LocalFree(pszA);
			pszA = NULL;
		}
	}

	return pszA;

}

//
//  InOutWtoA/InOutAtoW is for thunking INOUT string parameters.
//
//  INOUT parameters suck.
//
// We need to save both the original ANSI and the
// original UNICODE strings, so that if the app doesn't
// change the ANSI string, we leave the original UNICODE
// string alone.  That way, UNICODE item names don't get
// obliterated by the thunk.
//
// The original buffer is saved in pvThunk1.
// We allocate two ANSI buffers.
// pvThunk2 contains the original ANSIfied string.
// pvThunk2+cchTextMax is the buffer we pass to the app.
// On the way back, we compare pvThunk2 with pvThunk2+cchTextMax.
// If they are different, then we unthunk the string; otherwise,
// we leave the original UNICODE buffer alone.

BOOL LibUIDK::InOutWtoA(LibUIDK_CONTROLINFO *pci, THUNKSTATE *pts, LPWSTR *ppsz, DWORD cchTextMax)
{
	pts->ts_pvThunk1 = *ppsz;               // Save original buffer
	pts->ts_dwThunkSize = cchTextMax;

	if (!IsFlagPtr(pts->ts_pvThunk1))
	{
		pts->ts_pvThunk2 = LocalAlloc(LPTR, cchTextMax * 2 * sizeof(char));
		if (!ConvertWToAN(pci->uiCodePage, (LPSTR)pts->ts_pvThunk2, pts->ts_dwThunkSize, (LPWSTR)pts->ts_pvThunk1, -1))
		{
			LocalFree(pts->ts_pvThunk2);
			return 0;
		}
		*ppsz = (LPWSTR)((LPSTR)pts->ts_pvThunk2 + cchTextMax);
		lstrcpyA((LPSTR)*ppsz, (LPCSTR)pts->ts_pvThunk2);
	}
	return TRUE;
}

/*
 * Creates a buffer for a unicode string, and then copies the ANSI text
 * into it (converting it to unicode in the process)
 *
 * The returned pointer should be freed with LocalFree after use.
 */
LPWSTR LibUIDK::ProduceWFromA(UINT uiCodePage, LPCSTR psz)
{
	LPWSTR pszW;
	int cch;

	if (psz == NULL || psz == LPSTR_TEXTCALLBACKA)
	{
		return (LPWSTR)psz;
	}

	// The old code would call lstrlen and lstrcpy which would fault internal to the
	// api, this should do about the same...
	if (IsBadReadPtr(psz, 1))
	{
		return NULL;    // For now lets try not setting a string...
	}

	cch = MultiByteToWideChar(uiCodePage, 0, psz, -1, NULL, 0);

	if (cch == 0)
	{
		cch = 1;
	}

	pszW = (LPWSTR)LocalAlloc(LMEM_FIXED, cch * sizeof(WCHAR));

	if (pszW != NULL)
	{
		if (MultiByteToWideChar(uiCodePage, MB_PRECOMPOSED, psz, -1, pszW,
				cch) == FALSE)
		{
			LocalFree(pszW);
			pszW = NULL;
		}
	}

	return pszW;

}

void LibUIDK::StringBufferAtoW(UINT uiCodePage, LPVOID pvOrgPtr, DWORD dwOrgSize, CHAR **ppszText)
{
	if (pvOrgPtr == *ppszText)
	{
		// the pointer has not been changed by the callback...
		// must convert from A to W in-place

		if (dwOrgSize)
		{
			LPWSTR pszW = ProduceWFromA(uiCodePage, *ppszText);
			if (pszW)
			{
				lstrcpynW((WCHAR *)(*ppszText), pszW, dwOrgSize);   // this becomes a W buffer
				FreeProducedString(pszW);
			}
		}
	}
	else
	{
		// the pointer has been changed out from underneath us, copy
		// unicode back into the original buffer.

		ConvertAToWN(uiCodePage, (LPWSTR)pvOrgPtr, dwOrgSize, *ppszText, -1);
		*ppszText = (CHAR *)pvOrgPtr;
	}
}

void LibUIDK::InOutAtoW(LibUIDK_CONTROLINFO *pci, THUNKSTATE *pts, LPSTR *ppsz)
{
	if (!IsFlagPtr(pts->ts_pvThunk1))
	{
		if (!IsFlagPtr(*ppsz) &&
			lstrcmpA((LPCSTR)pts->ts_pvThunk2, (LPSTR)*ppsz) != 0)
		{
			StringBufferAtoW(pci->uiCodePage, pts->ts_pvThunk1, pts->ts_dwThunkSize, ppsz);
		}
		LocalFree(pts->ts_pvThunk2);
	}
	*ppsz = (LPSTR)pts->ts_pvThunk1;
}

#define TTF_MEMALLOCED          0x0200

//*************************************************************
//
//  ThunkToolTipTextAtoW()
//
//  Purpose:    Thunks a TOOLTIPTEXTA structure to a TOOLTIPTEXTW
//              structure.
//
//  Return:     (BOOL) TRUE if successful
//                     FALSE if an error occurs
//
//*************************************************************

BOOL LibUIDK::ThunkToolTipTextAtoW(LPTOOLTIPTEXTA lpTttA, LPTOOLTIPTEXTW lpTttW, UINT uiCodePage)
{
	int iResult;


	if (!lpTttA || !lpTttW)
	{
		return FALSE;
	}

	//
	// Thunk the NMHDR structure.
	//
	lpTttW->hdr.hwndFrom = lpTttA->hdr.hwndFrom;
	lpTttW->hdr.idFrom   = lpTttA->hdr.idFrom;
	lpTttW->hdr.code     = TTN_NEEDTEXTW;

	lpTttW->hinst  = lpTttA->hinst;
	lpTttW->uFlags = lpTttA->uFlags;
	lpTttW->lParam = lpTttA->lParam;

	//
	// Thunk the string to the new structure.
	// Special case LPSTR_TEXTCALLBACK.
	//

	if (lpTttA->lpszText == LPSTR_TEXTCALLBACKA)
	{
		lpTttW->lpszText = LPSTR_TEXTCALLBACKW;

	}
	else if (!IS_INTRESOURCE(lpTttA->lpszText))
	{

		//
		//  Transfer the lpszText into the lpTttW...
		//
		//  First see if it fits into the buffer, and optimistically assume
		//  it will.
		//
		lpTttW->lpszText = lpTttW->szText;
		iResult = MultiByteToWideChar(uiCodePage, 0, lpTttA->lpszText, -1,
				lpTttW->szText, ARRAYSIZE(lpTttW->szText));
		if (!iResult)
		{
			//
			//  Didn't fit into the small buffer; must alloc our own.
			//
			lpTttW->lpszText = ProduceWFromA(uiCodePage, lpTttA->lpszText);
			lpTttW->uFlags |= TTF_MEMALLOCED;
		}

	}
	else
	{
		lpTttW->lpszText = (LPWSTR)lpTttA->lpszText;
	}

	return TRUE;
}

LONG LibUIDK::GetMessagePosClient(HWND hwnd, LPPOINT ppt)
{
	LPARAM lParam;
	POINT pt;
	if (!ppt)
	{
		ppt = &pt;
	}

	lParam = GetMessagePos();
	ppt->x = GET_X_LPARAM(lParam);
	ppt->y = GET_Y_LPARAM(lParam);
	ScreenToClient(hwnd, ppt);

	return MAKELONG(ppt->x, ppt->y);
}


// --------------------------------------------------------------------------
//
//  MyNotifyWinEvent()
//
//  This tries to get the proc address of NotifyWinEvent().  If it fails, we
//  remember that and do nothing.
//
//  NOTE TO NT FOLKS:
//  Don't freak out about this code.  It will do nothing on NT, nothing yet
//  that is.  Active Accessibility will be ported to NT for Service Pack #1
//  or at worst #2 after NT SUR ships, this code will work magically when
//  that is done/
//
// --------------------------------------------------------------------------
void LibUIDK::MyNotifyWinEvent(UINT event, HWND hwnd, LONG idContainer, LONG_PTR idChild)
{
	static NOTIFYWINEVENTPROC s_pfnNotifyWinEvent = NULL;

	if (!s_pfnNotifyWinEvent)
	{
		HMODULE hmod;

		if (hmod = GetModuleHandle(TEXT("USER32")))
			s_pfnNotifyWinEvent = (NOTIFYWINEVENTPROC)GetProcAddress(hmod,
					"NotifyWinEvent");

		if (!s_pfnNotifyWinEvent)
		{
			s_pfnNotifyWinEvent = DONOTHING_NOTIFYWINEVENT;
		}
	}

	if (s_pfnNotifyWinEvent != DONOTHING_NOTIFYWINEVENT)
	{
		(* s_pfnNotifyWinEvent)(event, hwnd, idContainer, idChild);
	}
}

#ifndef WINNT
HANDLE LibUIDK::InitSharedHeap(void)
{
	//L ENTERCRITICAL;
	if (g_hSharedHeap == NULL)
	{
		g_hSharedHeap = HeapCreate(HEAP_SHARED, 1, MAXHEAPSIZE);
	}
	//L LEAVECRITICAL;
	return g_hSharedHeap;
}
#endif

__inline HANDLE LibUIDK::GetSharedHeapHandle(void)
{
	if (g_hSharedHeap)
	{
		return g_hSharedHeap;
	}
	else
	{
		return InitSharedHeap();
	}
}

void *WINAPI LibUIDK::Alloc(long cb)
{
	// I will assume that this is the only one that needs the checks to
	// see if the heap has been previously created or not
#if defined(WINNT) || defined(MAINWIN)
	return (void *)LocalAlloc(LPTR, cb);
#else
	HANDLE hHeap = GetSharedHeapHandle();

	// If still NULL we have problems!
	if (hHeap == NULL)
	{
		return (NULL);
	}

	return HeapAlloc(hHeap, HEAP_ZERO_MEMORY, cb);
#endif
}

void *WINAPI LibUIDK::ReAlloc(void *pb, long cb)
{
	if (pb == NULL)
	{
		return Alloc(cb);
	}
#if defined(WINNT) || defined(MAINWIN)
	return (void *)LocalReAlloc((HLOCAL)pb, cb, LMEM_ZEROINIT | LMEM_MOVEABLE);
#else
	return HeapReAlloc(g_hSharedHeap, HEAP_ZERO_MEMORY, pb, cb);
#endif
}

//
//  IncrementSearchString - Add or clear the search string
//
//      ch == 0:  Reset the search string.  Return value meaningless.
//
//      ch != 0:  Append the character to the search string, starting
//                a new search string if we timed out the last one.
//                lplpstr receives the string so far.
//                Return value is TRUE if a new search string was
//                created, or FALSE if we appended to an existing one.
//

BOOL FAR PASCAL LibUIDK::IncrementSearchString(PISEARCHINFO pis, UINT ch, LPTSTR FAR *lplpstr)
{
	BOOL fRestart = FALSE;

	if (!ch)
	{
		pis->ichCharBuf = 0;
		pis->iIncrSearchFailed = 0;
		return FALSE;
	}

	if (IsISearchTimedOut(pis))
	{
		pis->iIncrSearchFailed = 0;
		pis->ichCharBuf = 0;
	}

	if (pis->ichCharBuf == 0)
	{
		fRestart = TRUE;
	}

	pis->timeLast = GetMessageTime();

	// Is there room for new character plus zero terminator?
	//
	if (pis->ichCharBuf + 1 + 1 > pis->cbCharBuf)
	{
		LPTSTR psz = (LPTSTR)ReAlloc(pis->pszCharBuf, ((pis->cbCharBuf + 16) * sizeof(TCHAR)));
		if (!psz)
		{
			return fRestart;
		}

		pis->cbCharBuf += 16;
		pis->pszCharBuf = psz;
	}

	pis->pszCharBuf[pis->ichCharBuf++] = (TCHAR)ch;
	pis->pszCharBuf[pis->ichCharBuf] = 0;

	*lplpstr = pis->pszCharBuf;

	return fRestart;
}

BOOL FAR PASCAL LibUIDK::SameChars(LPTSTR lpsz, TCHAR c)
{
	while (*lpsz)
	{
		if (*lpsz++ != c)
		{
			return FALSE;
		}
	}
	return TRUE;
}

// Beep only on the first failure.

void FAR PASCAL LibUIDK::IncrementSearchBeep(PISEARCHINFO pis)
{
	if (!pis->iIncrSearchFailed)
	{
		pis->iIncrSearchFailed = TRUE;
		MessageBeep(0);
	}
}

UINT LibUIDK::CCSwapKeys(WPARAM wParam, UINT vk1, UINT vk2)
{
	if (wParam == vk1)
	{
		return vk2;
	}
	if (wParam == vk2)
	{
		return vk1;
	}
	return (UINT)wParam;
}

UINT LibUIDK::RTLSwapLeftRightArrows(LibUIDK_CONTROLINFO *pci, WPARAM wParam)
{
	if (pci->dwExStyle & RTL_MIRRORED_WINDOW)
	{
		return CCSwapKeys(wParam, VK_LEFT, VK_RIGHT);
	}
	return (UINT)wParam;
}

//
// Translate the given font to a code page used for thunking text
//
UINT LibUIDK::GetCodePageForFont(HFONT hFont)
{
#ifdef WINNT
	LOGFONT lf;
	TCHAR szFontName[MAX_PATH];
	CHARSETINFO csi;
	DWORD dwSize, dwType;
	HKEY hKey;


	if (!GetObject(hFont, sizeof(lf), &lf))
	{
		return CP_ACP;
	}


	//
	// Check for font substitutes
	//

	lstrcpy(szFontName, lf.lfFaceName);

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\FontSubstitutes"),
			0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{

		dwSize = MAX_PATH * sizeof(TCHAR);
		RegQueryValueEx(hKey, lf.lfFaceName, NULL, &dwType,
			(LPBYTE) szFontName, &dwSize);

		RegCloseKey(hKey);
	}


	//
	//  This is to fix office for locales that use non 1252 versions
	//  of Ms Sans Serif and Ms Serif.  These fonts incorrectly identify
	//  themselves as having an Ansi charset, so TranslateCharsetInfo will
	//  return the wrong value.
	//
	//  NT bug 260697: Office 2000 uses Tahoma.
	//
	if ((lf.lfCharSet == ANSI_CHARSET) &&
		(!lstrcmpi(L"Helv", szFontName) ||
			!lstrcmpi(L"Ms Sans Serif", szFontName) ||
			!lstrcmpi(L"Ms Serif", szFontName) ||
			!lstrcmpi(L"Tahoma", szFontName)))
	{
		return CP_ACP;
	}
	//
	//  This is to fix FE office95a and Pro. msofe95.dll sets wrong charset when create
	//  listview control. so TranslateCharsetInfo will return the wrong value.
	//  Korea  : DotumChe.
	//  Taiwan : New MingLight
	//  China  : SongTi

	if ((lf.lfCharSet == SHIFTJIS_CHARSET) &&
		(!lstrcmpi(L"\xb3cb\xc6c0\xccb4", lf.lfFaceName))        || // Korea
		(!lstrcmpi(L"\x65b0\x7d30\x660e\x9ad4", lf.lfFaceName))  || // Taiwan
		(!lstrcmpi(L"\x5b8b\x4f53", lf.lfFaceName)))                // PRC
	{
		return CP_ACP;
	}

	if (!TranslateCharsetInfo((DWORD FAR *) lf.lfCharSet, &csi, TCI_SRCCHARSET))
	{
		return CP_ACP;
	}

	return csi.ciACP;
#else

	return CP_ACP;

#endif
}

//
//  When we want to turn a tooltip into an infotip, we set its
//  width to 300 "small pixels", where there are 72 small pixels
//  per inch when you are in small fonts mode.
//
//  Scale this value based on the magnification in effect
//  on the owner's monitor.  But never let the tooltip get
//  bigger than 3/4 of the screen.
//
void LibUIDK::CCSetInfoTipWidth(HWND hwndOwner, HWND hwndToolTips)
{
	HDC hdc = GetDC(hwndOwner);
	int iWidth = MulDiv(GetDeviceCaps(hdc, LOGPIXELSX), 300, 72);
	int iMaxWidth = GetDeviceCaps(hdc, HORZRES) * 3 / 4;
	SendMessage(hwndToolTips, TTM_SETMAXTIPWIDTH, 0, min(iWidth, iMaxWidth));
	ReleaseDC(hwndOwner, hdc);
}

void FAR PASCAL LibUIDK::RelayToToolTips(HWND hwndToolTips, HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	if (hwndToolTips)
	{
		MSG msg;
		msg.lParam = lParam;
		msg.wParam = wParam;
		msg.message = wMsg;
		msg.hwnd = hWnd;
		SendMessage(hwndToolTips, TTM_RELAYEVENT, 0, (LPARAM)(LPMSG)&msg);
	}
}

#ifdef UNICODE
/*
 * Thunk for LVM_GETISEARCHSTRINGA
 */
int FAR PASCAL LibUIDK::GetIncrementSearchStringA(PISEARCHINFO pis, UINT uiCodePage, LPSTR lpsz)
{
	if (IsISearchTimedOut(pis))
	{
		pis->iIncrSearchFailed = 0;
		pis->ichCharBuf = 0;
	}

	if (pis->ichCharBuf && lpsz)
	{
		ConvertWToAN(uiCodePage, lpsz, pis->ichCharBuf, pis->pszCharBuf, pis->ichCharBuf);
		lpsz[pis->ichCharBuf] = '\0';
	}
	return pis->ichCharBuf;
}
#endif

int FAR PASCAL LibUIDK::GetIncrementSearchString(PISEARCHINFO pis, LPTSTR lpsz)
{
	if (IsISearchTimedOut(pis))
	{
		pis->iIncrSearchFailed = 0;
		pis->ichCharBuf = 0;
	}

	if (pis->ichCharBuf && lpsz)
	{
		lstrcpyn(lpsz, pis->pszCharBuf, pis->ichCharBuf + 1);
		lpsz[pis->ichCharBuf] = TEXT('\0');
	}
	return pis->ichCharBuf;
}

STDAPI_(HCURSOR) LibUIDK::LoadHandCursor(DWORD dwRes)
{
	//if (g_bRunOnNT5 || g_bRunOnMemphis)
	//{
	//	HCURSOR hcur = LoadCursor(NULL, IDC_HAND);  // from USER, system supplied
	//	if (hcur)
	//	{
	//		return hcur;
	//	}
	//}

	//L
	return LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND_INTERNAL));
}

LRESULT FAR PASCAL LibUIDK::CIHandleNotifyFormat(LibUIDK_LPCONTROLINFO lpci, LPARAM lParam)
{
	if (lParam == NF_QUERY)
	{
#ifdef UNICODE
		return NFR_UNICODE;
#else
		return NFR_ANSI;
#endif
	}
	else if (lParam == NF_REQUERY)
	{
		LRESULT uiResult;

		uiResult = SendMessage(lpci->hwndParent, WM_NOTIFYFORMAT,
				(WPARAM)lpci->hwnd, NF_QUERY);

		lpci->bUnicode = BOOLIFY(uiResult == NFR_UNICODE);

		return uiResult;
	}
	return 0;
}

// returns TRUE if handled
BOOL LibUIDK::CCWndProc(LibUIDK_CONTROLINFO *pci, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *plres)
{
	if (uMsg >= CCM_FIRST && uMsg < CCM_LAST)
	{
		LRESULT lres = 0;
		switch (uMsg)
		{
		case CCM_SETUNICODEFORMAT:
			lres = pci->bUnicode;
			pci->bUnicode = BOOLFROMPTR(wParam);
			break;

		case CCM_GETUNICODEFORMAT:
			lres = pci->bUnicode;
			break;

		case CCM_SETVERSION:
			if (wParam <= COMCTL32_VERSION)
			{
				lres = pci->iVersion;
				pci->iVersion = (int)wParam;
			}
			else
			{
				lres = -1;
			}
			break;

		case CCM_GETVERSION:
			lres = pci->iVersion;
			break;

		}

		ASSERT(plres);
		*plres = lres;

		return TRUE;
	}

	return FALSE;
}

// BUGBUG: very similar routine in treeview

// in:
//      hwndEdit        edit control to position in client coords of parent window
//      prc             bonding rect of the text, used to position everthing
//      hFont           font being used
//      flags
//          SEIPS_WRAP      if this is a wrapped type (multiline) edit
//          SEIPS_NOSCROLL  if the parent control does not have scrollbars
//
//      The SEIPS_NOSCROLL flag is used only in DEBUG.  Normally, the item
//      being edited should have been scrolled into view, but if the parent
//      doesn't have scrollbars, then clearly that's not possible, so we
//      shouldn't ASSERT in that case.
//
// Notes:
//       The top-left corner of the bouding rectangle must be the position
//      the client uses to draw text. We adjust the edit field rectangle
//      appropriately.
//

void FAR PASCAL LibUIDK::SetEditInPlaceSize(HWND hwndEdit, RECT FAR *prc, HFONT hFont, UINT seips)
{
	RECT rc, rcClient, rcFormat;
	TCHAR szLabel[CCHLABELMAX + 1];
	int cchLabel, cxIconTextWidth;
	HDC hdc;
	HWND hwndParent = GetParent(hwndEdit);
	UINT flags;

	// was #ifdef DBCS
	// short wRightMgn;
	// #endif

	cchLabel = Edit_GetText(hwndEdit, szLabel, ARRAYSIZE(szLabel));
	if (szLabel[0] == 0)
	{
		lstrcpy(szLabel, c_szSpace);
		cchLabel = 1;
	}

	hdc = GetDC(hwndParent);

#ifdef DEBUG
	//DrawFocusRect(hdc, prc);       // this is the rect they are passing in
#endif

	SelectFont(hdc, hFont);

	cxIconTextWidth = g_cxIconSpacing - g_cxLabelMargin * 2;
	rc.left = rc.top = rc.bottom = 0;
	rc.right = cxIconTextWidth;      // for DT_LVWRAP

	// REVIEW: we might want to include DT_EDITCONTROL in our DT_LVWRAP

	if (seips & SEIPS_WRAP)
	{
		flags = DT_LVWRAP | DT_CALCRECT;
		// We only use DT_NOFULLWIDTHCHARBREAK on Korean(949) Memphis and NT5
		//L if (949 == g_uiACP && (g_bRunOnNT5 || g_bRunOnMemphis))
		//{
		//	flags |= DT_NOFULLWIDTHCHARBREAK;
		//}
	}
	else
	{
		flags = DT_LV | DT_CALCRECT;
	}
	// If the string is NULL display a rectangle that is visible.
	DrawText(hdc, szLabel, cchLabel, &rc, flags);

	// Minimum text box size is 1/4 icon spacing size
	if (rc.right < g_cxIconSpacing / 4)
	{
		rc.right = g_cxIconSpacing / 4;
	}

	// position the text rect based on the text rect passed in
	// if wrapping, center the edit control around the text mid point

	OffsetRect(&rc,
		(seips & SEIPS_WRAP) ? prc->left + ((prc->right - prc->left) - (rc.right - rc.left)) / 2 : prc->left,
		(seips & SEIPS_WRAP) ? prc->top : prc->top + ((prc->bottom - prc->top) - (rc.bottom - rc.top)) / 2);

	// give a little space to ease the editing of this thing
	if (!(seips & SEIPS_WRAP))
	{
		rc.right += g_cxLabelMargin * 4;
	}
	rc.right += g_cyEdge;   // try to leave a little more for dual blanks

#ifdef DEBUG
	//DrawFocusRect(hdc, &rc);
#endif

	ReleaseDC(hwndParent, hdc);

	//
	// #5688: We need to make it sure that the whole edit window is
	//  always visible. We should not extend it to the outside of
	//  the parent window.
	//
	{
		BOOL fSuccess;
		GetClientRect(hwndParent, &rcClient);
		fSuccess = IntersectRect(&rc, &rc, &rcClient);
		ASSERT(fSuccess || IsRectEmpty(&rcClient) || (seips & SEIPS_NOSCROLL));
	}

	//
	// Inflate it after the clipping, because it's ok to hide border.
	//
	// EM_GETRECT already takes EM_GETMARGINS into account, so don't use both.

	SendMessage(hwndEdit, EM_GETRECT, 0, (LPARAM)(LPRECT)&rcFormat);

	// Turn the margins inside-out so we can AdjustWindowRect on them.
	rcFormat.top = -rcFormat.top;
	rcFormat.left = -rcFormat.left;
	AdjustWindowRectEx(&rcFormat, GetWindowStyle(hwndEdit), FALSE,
		GetWindowExStyle(hwndEdit));

	InflateRect(&rc, -rcFormat.left, -rcFormat.top);

	HideCaret(hwndEdit);

	SetWindowPos(hwndEdit, NULL, rc.left, rc.top,
		rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS);

	CopyRect(prc, (CONST RECT *)&rc);

#ifndef UNIX
	InvalidateRect(hwndEdit, NULL, TRUE);
#endif

	ShowCaret(hwndEdit);
}

HWND FAR PASCAL LibUIDK::CreateEditInPlaceWindow(HWND hwnd, LPCTSTR lpText, int cbText, LONG style, HFONT hFont)
{
	HWND hwndEdit;

	// Create the window with some nonzero size so margins work properly
	// The caller will do a SetEditInPlaceSize to set the real size
	// But make sure the width is huge so when an app calls SetWindowText,
	// USER won't try to scroll the window.
	hwndEdit = CreateWindowEx(GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_RTLREADING,
			TEXT("EDIT"), lpText, style,
			0, 0, 16384, 20, hwnd, NULL, NULL, NULL);

	if (hwndEdit)
	{

		Edit_LimitText(hwndEdit, cbText);

		Edit_SetSel(hwndEdit, 0, 0);    // move to the beginning

		FORWARD_WM_SETFONT(hwndEdit, hFont, FALSE, SendMessage);

	}

	return hwndEdit;
}

void FAR PASCAL LibUIDK::RescrollEditWindow(HWND hwndEdit)
{
	Edit_SetSel(hwndEdit, -1, -1);      // move to the end
	Edit_SetSel(hwndEdit, 0, -1);       // select all text
}

void ScrollShrinkRect(int x, int y, LPRECT lprc)
{
	if (lprc)
	{
		if (x > 0)
		{
			lprc->left += x;
		}
		else
		{
			lprc->right += x;
		}

		if (y > 0)
		{
			lprc->top += y;
		}
		else
		{
			lprc->bottom += y;
		}

	}
}

#ifndef SSW_EX_IGNORESETTINGS
#define SSW_EX_IGNORESETTINGS   0x00040000  // ignore system settings to turn on/off smooth scroll
#endif

#define SUBSCROLLS 100

#define DEFAULT_MAXSCROLLTIME ((GetDoubleClickTime() / 2) + 1)  // Ensure >= 1
#define DEFAULT_MINSCROLL 8
int LibUIDK::SmoothScrollWindow(PSMOOTHSCROLLINFO psi)
{
	int dx = psi->dx;
	int dy = psi->dy;
	LPCRECT lprcSrc = psi->lprcSrc;
	LPCRECT lprcClip = psi->lprcClip;
	HRGN hrgnUpdate = psi->hrgnUpdate;
	LPRECT lprcUpdate = psi->lprcUpdate;
	UINT fuScroll = psi->fuScroll;
	int iRet = SIMPLEREGION;
	RECT rcUpdate;
	RECT rcSrc;
	RECT rcClip;
	int xStep;
	int yStep;
	int iSlicesDone = 0;
	int iSlices;
	DWORD dwTimeStart, dwTimeNow;
	HRGN hrgnLocalUpdate;
	UINT cxMinScroll = psi->cxMinScroll;
	UINT cyMinScroll = psi->cyMinScroll;
	UINT uMaxScrollTime = psi->uMaxScrollTime;
	int iSubScrolls;
	PFNSMOOTHSCROLLPROC pfnScrollProc;

	if (!lprcUpdate)
	{
		lprcUpdate = &rcUpdate;
	}
	SetRectEmpty(lprcUpdate);

	if (psi->cbSize != sizeof(SMOOTHSCROLLINFO))
	{
		return 0;
	}

	// check the defaults
	if (!(psi->fMask & SSIF_MINSCROLL)
		|| cxMinScroll == SSI_DEFAULT)
	{
		cxMinScroll = DEFAULT_MINSCROLL;
	}

	if (!(psi->fMask & SSIF_MINSCROLL)
		|| cyMinScroll == SSI_DEFAULT)
	{
		cyMinScroll = DEFAULT_MINSCROLL;
	}

	if (!(psi->fMask & SSIF_MAXSCROLLTIME)
		|| uMaxScrollTime == SSI_DEFAULT)
	{
		uMaxScrollTime = DEFAULT_MAXSCROLLTIME;
	}

	if (uMaxScrollTime < SUBSCROLLS)
	{
		uMaxScrollTime = SUBSCROLLS;
	}


	if ((!(fuScroll & SSW_EX_IGNORESETTINGS)) &&
		(!g_fSmoothScroll))
	{
		fuScroll |= SSW_EX_IMMEDIATE;
	}

	if ((psi->fMask & SSIF_SCROLLPROC) && psi->pfnScrollProc)
	{
		pfnScrollProc = psi->pfnScrollProc;
	}
	else
	{
		pfnScrollProc = ScrollWindowEx;
	}

#ifdef ScrollWindowEx
#undef ScrollWindowEx
#endif

	if (fuScroll & SSW_EX_IMMEDIATE)
	{
		return pfnScrollProc(psi->hwnd, dx, dy, lprcSrc, lprcClip, hrgnUpdate,
				lprcUpdate, LOWORD(fuScroll));
	}

	// copy input rects locally
	if (lprcSrc)
	{
		rcSrc = *lprcSrc;
		lprcSrc = &rcSrc;
	}
	if (lprcClip)
	{
		rcClip = *lprcClip;
		lprcClip = &rcClip;
	}

	if (!hrgnUpdate)
	{
		hrgnLocalUpdate = CreateRectRgn(0, 0, 0, 0);
	}
	else
	{
		hrgnLocalUpdate = hrgnUpdate;
	}

	//set up initial vars
	dwTimeStart = GetTickCount();

	if (fuScroll & SSW_EX_NOTIMELIMIT)
	{
		xStep = cxMinScroll * (dx < 0 ? -1 : 1);
		yStep = cyMinScroll * (dy < 0 ? -1 : 1);
	}
	else
	{
		iSubScrolls = (uMaxScrollTime / DEFAULT_MAXSCROLLTIME) * SUBSCROLLS;
		if (!iSubScrolls)
		{
			iSubScrolls = SUBSCROLLS;
		}
		xStep = dx / iSubScrolls;
		yStep = dy / iSubScrolls;
	}

	if (xStep == 0 && dx)
	{
		xStep = dx < 0 ? -1 : 1;
	}

	if (yStep == 0 && dy)
	{
		yStep = dy < 0 ? -1 : 1;
	}

	while (dx || dy)
	{
		int x, y;
		RECT rcTempUpdate;

		if (fuScroll & SSW_EX_NOTIMELIMIT)
		{
			x = xStep;
			y = yStep;
			if (abs(x) > abs(dx))
			{
				x = dx;
			}

			if (abs(y) > abs(dy))
			{
				y = dy;
			}

		}
		else
		{
			int iTimePerScroll = uMaxScrollTime / iSubScrolls;
			if (!iTimePerScroll)
			{
				iTimePerScroll = 1;
			}

			dwTimeNow = GetTickCount();

			iSlices = ((dwTimeNow - dwTimeStart) / iTimePerScroll) - iSlicesDone;
			if (iSlices < 0)
			{
				iSlices = 0;
			}


			do
			{

				int iRet = 0;

				iSlices++;
				if ((iSlicesDone + iSlices) <= iSubScrolls)
				{
					x = xStep * iSlices;
					y = yStep * iSlices;

					// this could go over if we rounded ?Step up to 1(-1) above
					if (abs(x) > abs(dx))
					{
						x = dx;
					}

					if (abs(y) > abs(dy))
					{
						y = dy;
					}

				}
				else
				{
					x = dx;
					y = dy;
				}

				//DebugMsg(DM_TRACE, "SmoothScrollWindowCallback %d", iRet);

				if (x == dx && y == dy)
				{
					break;
				}

				if ((((UINT)(abs(x)) >= cxMinScroll) || !x) &&
					(((UINT)(abs(y)) >= cyMinScroll) || !y))
				{
					break;
				}

			}
			while (1);
		}

		if (pfnScrollProc(psi->hwnd, x, y, lprcSrc, lprcClip, hrgnLocalUpdate, &rcTempUpdate, LOWORD(fuScroll)) == ERROR)
		{
			iRet = ERROR;
			goto Bail;
		}

		// we don't need to do this always because if iSlices >= iSlicesDone, we'll have scrolled blanks
		//if (iSlices < iSlicesDone)
		RedrawWindow(psi->hwnd, NULL, hrgnLocalUpdate, RDW_ERASE | RDW_ERASENOW | RDW_INVALIDATE);

		UnionRect(lprcUpdate, &rcTempUpdate, lprcUpdate);

		ScrollShrinkRect(x, y, (LPRECT)lprcSrc);
		ScrollShrinkRect(x, y, (LPRECT)lprcClip);

		dx -= x;
		dy -= y;
		iSlicesDone += iSlices;
	}

Bail:

	if (fuScroll & SW_SCROLLCHILDREN)
	{
		RedrawWindow(psi->hwnd, lprcUpdate, NULL, RDW_INVALIDATE);
	}

	if (hrgnLocalUpdate != hrgnUpdate)
	{
		DeleteObject(hrgnLocalUpdate);
	}

	return iRet;
}

//
//  Too many apps encounter strange behavior when we send out
//  NM_CUSTOMDRAW messages at times unrelated to painting.
//  E.g., NetMeeting and MFC recurse back into ListView_RecomputeLabelSize.
//  CryptUI will fault if it's asked to NM_CUSTOMDRAW before it gets
//  WM_INITDIALOG.  So all this fake customdraw stuff is v5 only.
//
//  And since it is very popular to call back into the control during
//  the handling of NM_CUSTOMDRAW, we protect against recursing ourselves
//  to death by blowing off nested fake customdraw messages.


DWORD LibUIDK::CIFakeCustomDrawNotify(LibUIDK_LPCONTROLINFO lpci, DWORD dwStage, LPNMCUSTOMDRAW lpnmcd)
{
	DWORD dwRet = CDRF_DODEFAULT;

	if (lpci->iVersion >= 5 && !lpci->bInFakeCustomDraw)
	{
		lpci->bInFakeCustomDraw = TRUE;
		dwRet = CICustomDrawNotify(lpci, dwStage, lpnmcd);
		ASSERT(lpci->bInFakeCustomDraw);
		lpci->bInFakeCustomDraw = FALSE;
	}

	return dwRet;
}

// common control info helpers
void FAR PASCAL LibUIDK::CIInitialize(LibUIDK_LPCONTROLINFO lpci, HWND hwnd, LPCREATESTRUCT lpcs)
{
	lpci->hwnd = hwnd;
	lpci->hwndParent = lpcs->hwndParent;
	lpci->style = lpcs->style;
	lpci->uiCodePage = CP_ACP;
	lpci->dwExStyle = lpcs->dwExStyle;

	lpci->bUnicode = lpci->hwndParent &&
		SendMessage(lpci->hwndParent, WM_NOTIFYFORMAT,
			(WPARAM)lpci->hwnd, NF_QUERY) == NFR_UNICODE;

#ifdef KEYBOARDCUES
	if (lpci->hwndParent)
	{
		LRESULT lRes = SendMessage(lpci->hwndParent, WM_QUERYUISTATE, 0, 0);
		lpci->wUIState = LOWORD(lRes);
	}
#endif
}

//////////////////////////////////////////////////////////////////////////
// mirror.c

/***************************************************************************\
* MirrorIcon
*
* Mirror an Icon , given an Icon handle so that when these icons are displayed
* on a Mirrored DC, they end would be displayed normal.
*
* History:
* 04-Feb-1998 samera    Created
\***************************************************************************/
BOOL WINAPI LibUIDK::MirrorIcon(HICON *phiconSmall, HICON *phiconLarge)
{
	//L	HDC      hdcScreen;
	//	HBITMAP  hbm, hbmMask, hbmOld, hbmOldMask;
	//	BITMAP   bm;
	//	HICON    hicon[2] = {NULL, NULL};
	//	HICON    hiconNew[2] = {NULL, NULL};
	//	ICONINFO ii ;
	//	int      i;
	//#ifdef WINNT
	//#define      IPIXELOFFSET 0
	//#else // !WINNT
	//#define      IPIXELOFFSET 2
	//#endif WINNT
	//
	//	//
	//	// Synchronize access to global DCs now!
	//	// Allocate DCs if we didn't so far.
	//	//
	//	ENTERCRITICAL;
	//	HDC g_hdc = NULL;
	//	HDC g_hdcMask = NULL;
	//	if (!g_hdc && !g_hdcMask)
	//	{
	//		g_hdc = CreateCompatibleDC(NULL);
	//		if (g_hdc)
	//		{
	//#ifndef UNIX
	//#ifndef WINNT
	//			SetObjectOwner(g_hdc, NULL);
	//#endif
	//#endif /* !UNIX */
	//			g_hdcMask = CreateCompatibleDC(NULL);
	//
	//			if (g_hdcMask)
	//			{
	//#ifndef UNIX
	//#ifndef WINNT
	//				SetObjectOwner(g_hdcMask, HINST_THISDLL);
	//#endif
	//#endif /* !UNIX */
	//
	//				// L				SET_DC_RTL_MIRRORED(g_hdc);
	//				// 				SET_DC_RTL_MIRRORED(g_hdcMask);
	//			}
	//			else
	//			{
	//				DeleteDC(g_hdc);
	//				g_hdc = NULL;
	//			}
	//		}
	//	}
	//
	//	if (phiconSmall)
	//	{
	//		hicon[0] = *phiconSmall;
	//	}
	//
	//	if (phiconLarge)
	//	{
	//		hicon[1] = *phiconLarge;
	//	}
	//
	//	//
	//	// Acquire the screen DC
	//	//
	//	hdcScreen = GetDC(NULL);
	//
	//	if (g_hdc && g_hdcMask && hdcScreen)
	//	{
	//		for (i = 0 ; i < (sizeof(hicon) / sizeof(HICON)) ; i++)
	//		{
	//			if (hicon[i])
	//			{
	//				if (GetIconInfo(hicon[i], &ii) &&
	//					GetObject(ii.hbmColor, sizeof(BITMAP), &bm))
	//				{
	//					//
	//					// I don't want these.
	//					//
	//					DeleteObject(ii.hbmMask);
	//					DeleteObject(ii.hbmColor);
	//					ii.hbmMask = ii.hbmColor = NULL;
	//
	//					hbm = CreateCompatibleBitmap(hdcScreen, bm.bmWidth, bm.bmHeight);
	//					hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
	//					hbmOld = (HBITMAP)SelectObject(g_hdc, hbm);
	//					hbmOldMask = (HBITMAP)SelectObject(g_hdcMask, hbmMask);
	//
	//					DrawIconEx(g_hdc, IPIXELOFFSET, 0, hicon[i], bm.bmWidth, bm.bmHeight, 0,
	//						NULL, DI_IMAGE);
	//
	//					DrawIconEx(g_hdcMask, IPIXELOFFSET, 0, hicon[i], bm.bmWidth, bm.bmHeight, 0,
	//						NULL, DI_MASK);
	//
	//					SelectObject(g_hdc, hbmOld);
	//					SelectObject(g_hdcMask, hbmOldMask);
	//
	//					//
	//					// create the new mirrored icon, and delete bmps
	//					//
	//					ii.hbmMask  = hbmMask;
	//					ii.hbmColor = hbm;
	//					hiconNew[i] = CreateIconIndirect(&ii);
	//
	//					DeleteObject(hbm);
	//					DeleteObject(hbmMask);
	//				}
	//			}
	//		}
	//	}
	//
	//	ReleaseDC(NULL, hdcScreen);
	//
	//	//
	//	// Now we can reuse the global DCs
	//	//
	//	LEAVECRITICAL;
	//
	//	//
	//	// Update icons if needed, and destroy old ones!
	//	//
	//	if (hicon[0] && hiconNew[0])
	//	{
	//		*phiconSmall = hiconNew[0];
	//		DestroyIcon(hicon[0]);
	//	}
	//
	//	if (hicon[1] && hiconNew[1])
	//	{
	//		*phiconLarge = hiconNew[1];
	//
	//		//
	//		// Don't delete twice
	//		//
	//		if (hicon[1] != hicon[0])
	//		{
	//			DestroyIcon(hicon[1]);
	//		}
	//	}

	return TRUE;
}

HIMAGELIST LibUIDK::CreateCheckBoxImagelist(HIMAGELIST himl, BOOL fTree, BOOL fUseColorKey, BOOL fMirror)
{
	int cxImage, cyImage;
	HBITMAP hbm;
	HBITMAP hbmTemp;
	COLORREF clrMask;
	HDC hdcDesk = GetDC(NULL);
	HDC hdc;
	RECT rc;
	int nImages = fTree ? 3 : 2;

	if (!hdcDesk)
	{
		return NULL;
	}

	hdc = CreateCompatibleDC(hdcDesk);
	ReleaseDC(NULL, hdcDesk);

	if (!hdc)
	{
		return NULL;
	}

	// Must protect against ImageList_GetIconSize failing in case app
	// gave us a bad himl
	if (himl && ImageList_GetIconSize(himl, &cxImage, &cyImage))
	{
		// cxImage and cyImage are okay
	}
	else
	{
		cxImage = g_cxSmIcon;
		cyImage = g_cySmIcon;
	}

	himl = ImageList_Create(cxImage, cyImage, ILC_MASK, 0, nImages);
	hbm = CreateColorBitmap(cxImage * nImages, cyImage);

	if (fUseColorKey)
	{
		clrMask = RGB(255, 000, 255); // magenta
		if (clrMask == g_clrWindow)
		{
			clrMask = RGB(000, 000, 255);    // blue
		}
	}
	else
	{
		clrMask = g_clrWindow;
	}

	// fill
	hbmTemp = (HBITMAP)SelectObject(hdc, hbm);

	rc.left = rc.top = 0;
	rc.bottom = cyImage;
	rc.right = cxImage * nImages;
	FillRectClr(hdc, &rc, clrMask);

	rc.right = cxImage;
	// now draw the real controls on
	InflateRect(&rc, -g_cxEdge, -g_cyEdge);
	rc.right++;
	rc.bottom++;

	if (fTree)
	{
		OffsetRect(&rc, cxImage, 0);
	}

	DrawFrameControl(hdc, &rc, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_FLAT |
		(fUseColorKey ? 0 : DFCS_TRANSPARENT));
	OffsetRect(&rc, cxImage, 0);
	// [msadek]; For the mirrored case, there is an off-by-one somewhere in MirrorIcon() or System API.
	// Since I will not be touching MirrorIcon() by any mean and no chance to fix a system API,
	// let's compensate for it here.
	if (fMirror)
	{
		OffsetRect(&rc, -1, 0);
	}

	DrawFrameControl(hdc, &rc, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_FLAT | DFCS_CHECKED |
		(fUseColorKey ? 0 : DFCS_TRANSPARENT));

	SelectObject(hdc, hbmTemp);

	if (fUseColorKey)
	{
		ImageList_AddMasked(himl, hbm, clrMask);
	}
	else
	{
		ImageList_Add(himl, hbm, NULL);
	}

	if (fMirror)
	{
		HICON hIcon = ImageList_ExtractIcon(0, himl, nImages - 1);
		MirrorIcon(&hIcon, NULL);
		ImageList_ReplaceIcon(himl, nImages - 1, hIcon);
	}

	DeleteDC(hdc);
	DeleteObject(hbm);
	return himl;
}

//  this is implemented in toolbar.c, but we should be able to use
//  as well as long as we always set fHorizMode to FALSE
void PASCAL LibUIDK::DrawInsertMark(HDC hdc, LPRECT prc, BOOL fHorizMode, COLORREF clr)
{
	HPEN hPnMark = CreatePen(PS_SOLID, 1, clr);
	HPEN hOldPn;
	POINT rgPoint[4];
	if (!hPnMark)
	{
		hPnMark = (HPEN)GetStockObject(BLACK_PEN);    // fallback to draw with black pen
	}
	hOldPn = (HPEN)SelectObject(hdc, (HGDIOBJ)hPnMark);

	if (fHorizMode)
	{
		int iXCentre = (prc->left + prc->right) / 2;

		rgPoint[0].x = iXCentre + 1;
		rgPoint[0].y = prc->top + 2;
		rgPoint[1].x = iXCentre + 3;
		rgPoint[1].y = prc->top;
		rgPoint[2].x = iXCentre - 2;
		rgPoint[2].y = prc->top;
		rgPoint[3].x = iXCentre;
		rgPoint[3].y = prc->top + 2;

		// draw the top bit...
		Polyline(hdc, rgPoint, 4);

		rgPoint[0].x = iXCentre;
		rgPoint[0].y = prc->top;
		rgPoint[1].x = iXCentre;
		rgPoint[1].y = prc->bottom - 1;
		rgPoint[2].x = iXCentre + 1;
		rgPoint[2].y = prc->bottom - 1;
		rgPoint[3].x = iXCentre + 1;
		rgPoint[3].y = prc->top;

		// draw the middle...
		Polyline(hdc, rgPoint, 4);

		rgPoint[0].x = iXCentre + 1;
		rgPoint[0].y = prc->bottom - 3;
		rgPoint[1].x = iXCentre + 3;
		rgPoint[1].y = prc->bottom - 1;
		rgPoint[2].x = iXCentre - 2;
		rgPoint[2].y = prc->bottom - 1;
		rgPoint[3].x = iXCentre;
		rgPoint[3].y = prc->bottom - 3;

		// draw the bottom bit...
		Polyline(hdc, rgPoint, 4);
	}
	else
	{
		int iYCentre = (prc->top + prc->bottom) / 2;

		rgPoint[0].x = prc->left + 2;
		rgPoint[0].y = iYCentre;
		rgPoint[1].x = prc->left;
		rgPoint[1].y = iYCentre - 2;
		rgPoint[2].x = prc->left;
		rgPoint[2].y = iYCentre + 3;
		rgPoint[3].x = prc->left + 2;
		rgPoint[3].y = iYCentre + 1;

		// draw the top bit...
		Polyline(hdc, rgPoint, 4);

		rgPoint[0].x = prc->left;
		rgPoint[0].y = iYCentre;
		rgPoint[1].x = prc->right - 1;
		rgPoint[1].y = iYCentre;
		rgPoint[2].x = prc->right - 1;
		rgPoint[2].y = iYCentre + 1;
		rgPoint[3].x = prc->left;
		rgPoint[3].y = iYCentre + 1;

		// draw the middle...
		Polyline(hdc, rgPoint, 4);

		rgPoint[0].x = prc->right - 3;
		rgPoint[0].y = iYCentre;
		rgPoint[1].x = prc->right - 1;
		rgPoint[1].y = iYCentre - 2;
		rgPoint[2].x = prc->right - 1;
		rgPoint[2].y = iYCentre + 3;
		rgPoint[3].x = prc->right - 3;
		rgPoint[3].y = iYCentre + 1;

		// draw the bottom bit...
		Polyline(hdc, rgPoint, 4);
	}

	SelectObject(hdc, hOldPn);
	DeleteObject((HGDIOBJ)hPnMark);
}
