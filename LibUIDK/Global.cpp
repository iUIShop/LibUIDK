#include "stdafx.h"
#include <atlconv.h>
#include <map>
#include <comutil.h>
#include <ERRNO.H>
#include <afxpriv.h>
#if(_MSC_VER <= 1200) // VC6.0
#include <../src/afximpl.h>
#else
#include <../src/mfc/afximpl.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// global function

BOOL LibUIDK::IUIIsWindowsVersion(WORD wMajorVersion, WORD wMinorVersion,
	WORD wServicePackMajor, BYTE dwConditionMask)
{
#if(_MSC_VER <= 1200) // VC6.0
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);

	//
	// Current version
	//
	// Get Sp number
	CString strCSD = osvi.szCSDVersion;
	strCSD.Replace(_T("Service Pack"), _T(""));
	strCSD.TrimLeft();
	int nSPNumber = _ttoi(strCSD);

	DWORDLONG dlVersion = (DWORDLONG)osvi.dwMajorVersion << 32;
	dlVersion += osvi.dwMinorVersion << 16;
	dlVersion += WORD(nSPNumber);

	//
	// Compared version
	//
	DWORDLONG dlCompareVer = DWORDLONG(wMajorVersion) << 32;
	dlCompareVer += (DWORD)wMinorVersion << 16;
	dlCompareVer += wServicePackMajor;

	if (dwConditionMask == VER_EQUAL)
	{
		return dlVersion == dlCompareVer;
	}
	else if (dwConditionMask == VER_GREATER)
	{
		return dlVersion > dlCompareVer;
	}
	else if (dwConditionMask == VER_GREATER_EQUAL)
	{
		return dlVersion >= dlCompareVer;
	}
	else if (dwConditionMask == VER_LESS)
	{
		return dlVersion < dlCompareVer;
	}
	else if (dwConditionMask == VER_LESS_EQUAL)
	{
		return dlVersion <= dlCompareVer;
	}
	else
	{
		ASSERT(FALSE);
		return FALSE;
	}
#else
	OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0, {0}, 0, 0 };
	DWORDLONG const dwlConditionMask = VerSetConditionMask(
			VerSetConditionMask(
				VerSetConditionMask
				(0, VER_MAJORVERSION, dwConditionMask),
				VER_MINORVERSION, dwConditionMask),
			VER_SERVICEPACKMAJOR, dwConditionMask);

	osvi.dwMajorVersion = wMajorVersion;
	osvi.dwMinorVersion = wMinorVersion;
	osvi.wServicePackMajor = wServicePackMajor;

	return VerifyVersionInfoW(&osvi,
			VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask) != FALSE;
#endif
}

BOOL LibUIDK::IUIIsWindowsVersionEqual(
	WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor)
{
	return IUIIsWindowsVersion(wMajorVersion, wMinorVersion, wServicePackMajor, VER_EQUAL);
}

BOOL LibUIDK::IUIIsWindowsVersionOrGreater(
	WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor)
{
	return IUIIsWindowsVersion(wMajorVersion, wMinorVersion, wServicePackMajor, VER_GREATER_EQUAL);
}

BOOL LibUIDK::IUIIsWindowsVersionOrLess(
	WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor)
{
	return IUIIsWindowsVersion(wMajorVersion, wMinorVersion, wServicePackMajor, VER_LESS_EQUAL);
}

BOOL LibUIDK::IUIIsWindows2000()
{
	return IUIIsWindowsVersionEqual(HIBYTE(_WIN32_WINNT_WIN2K), LOBYTE(_WIN32_WINNT_WIN2K), 0);
}

BOOL LibUIDK::IUIIsWindowsXPOrGreater()
{
	return IUIIsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 0);
}

BOOL LibUIDK::IUIIsWindowsXPSP1OrGreater()
{
	return IUIIsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 1);
}

BOOL LibUIDK::IUIIsWindowsXPSP2OrGreater()
{
	return IUIIsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 2);
}

BOOL LibUIDK::IUIIsWindowsXPSP3OrGreater()
{
	return IUIIsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 3);
}

BOOL LibUIDK::IUIIsWindowsVistaOrGreater()
{
	return IUIIsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 0);
}

BOOL LibUIDK::IUIIsWindowsVistaSP1OrGreater()
{
	return IUIIsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 1);
}

BOOL LibUIDK::IUIIsWindowsVistaSP2OrGreater()
{
	return IUIIsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 2);
}

BOOL LibUIDK::IUIIsWindows7OrGreater()
{
	return IUIIsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN7), LOBYTE(_WIN32_WINNT_WIN7), 0);
}

BOOL LibUIDK::IUIIsWindows7SP1OrGreater()
{
	return IUIIsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN7), LOBYTE(_WIN32_WINNT_WIN7), 1);
}

BOOL LibUIDK::IUIIsWindows8OrGreater()
{
	return IUIIsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN8), LOBYTE(_WIN32_WINNT_WIN8), 0);
}

BOOL LibUIDK::IUIIsWindows8Point1OrGreater()
{
	return IUIIsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINBLUE),
			LOBYTE(_WIN32_WINNT_WINBLUE), 0);
}

BOOL LibUIDK::IUIIsWindowsServer()
{
#if(_MSC_VER <= 1200) // VC6.0
	ASSERT(FALSE);
	return FALSE;
#else
	OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0, {0}, 0, 0, 0, VER_NT_WORKSTATION };
	DWORDLONG const dwlConditionMask = VerSetConditionMask(0, VER_PRODUCT_TYPE, VER_EQUAL);

	return !VerifyVersionInfoW(&osvi, VER_PRODUCT_TYPE, dwlConditionMask);
#endif
}

int LibUIDK::RemoveAeroTheme(HWND hWnd)
{
	if (!IsWindow(hWnd))
	{
		return -1;
	}

	HINSTANCE hInst = LoadLibrary(_T("UxTheme.dll"));
	if (hInst)
	{
		typedef HRESULT(WINAPI * SET_WINDOW_THEME)(HWND, LPCWSTR, LPCWSTR);
		SET_WINDOW_THEME SetWindowTheme = (SET_WINDOW_THEME)GetProcAddress(hInst, "SetWindowTheme");
		if (SetWindowTheme != NULL)
		{
			SetWindowTheme(hWnd, L"", L"");    // 去掉xp主体
		}
		FreeLibrary(hInst);
	}
	hInst = LoadLibrary(_T("dwmapi.dll"));
	if (hInst)
	{
		typedef HRESULT(WINAPI * DWM_SET_WINDOW_ATTRIBUTE)(HWND, DWORD, LPCVOID, DWORD);
		DWM_SET_WINDOW_ATTRIBUTE DwmSetWindowAttribute = (DWM_SET_WINDOW_ATTRIBUTE)::GetProcAddress(
				hInst, "DwmSetWindowAttribute");
		if (DwmSetWindowAttribute != NULL)
		{
			DWORD ncrp = 1/*DWMNCRP_DISABLED*/;
			// 去掉vista特效
			DwmSetWindowAttribute(hWnd, 2/*DWMWA_NCRENDERING_POLICY*/, &ncrp, sizeof(ncrp));
		}
		FreeLibrary(hInst);
	}

	return 0;
}

BOOL LibUIDK::IsAeroTheme()
{
	BOOL bAero = FALSE;

	HINSTANCE hInst = LoadLibrary(_T("dwmapi.dll"));
	if (hInst != NULL)
	{
		typedef HRESULT(WINAPI * DWM_IS_COMPOSITION_ENABLED)(BOOL *);
		DWM_IS_COMPOSITION_ENABLED DwmIsCompositionEnabled =
			(DWM_IS_COMPOSITION_ENABLED)::GetProcAddress(hInst, "DwmIsCompositionEnabled");
		if (DwmIsCompositionEnabled != NULL)
		{
			BOOL bDwm = FALSE;
			HRESULT hr = DwmIsCompositionEnabled(&bDwm);
			if (hr == S_OK && bDwm)
			{
				bAero = TRUE;
			}
		}
		FreeLibrary(hInst);
	}

	return bAero;
}

//////////////////////////////////////////////////////////////////////////
//

// When use layered window, the window not redraw when its child controls hide or display
// to solve it, handle the WM_WINDOWPOSCHANGED message.
LRESULT LibUIDK::Ctrl_OnWindowPosChanged(CWnd *pCtrl, WPARAM wParam, LPARAM lParam)
{
	BOOL bLayered = IsLayeredWindow(pCtrl->GetParent()->GetSafeHwnd());
	if (bLayered)
	{
		WINDOWPOS *pos = (WINDOWPOS *)lParam;
		if (pos->hwnd == pCtrl->GetSafeHwnd())
		{
			// When show the control, notify itself to redraw
			if ((pos->flags & SWP_SHOWWINDOW) == SWP_SHOWWINDOW)
			{
				pCtrl->Invalidate();
			}
			// When hide the control, notify its parent to redraw
			else if ((pos->flags & SWP_HIDEWINDOW) == SWP_HIDEWINDOW)
			{
				CRect rcThis;
				pCtrl->GetWindowRect(rcThis);
				pCtrl->GetParent()->ScreenToClient(rcThis);
				pCtrl->GetParent()->InvalidateRect(rcThis);
			}
		}
	}

	return 0;
}

BOOL LibUIDK::IsParent(HWND hChild, HWND hParent)
{
	if (hChild == NULL || hParent == NULL)
	{
		return FALSE;
	}

	BOOL bParent = FALSE;
	HWND hWndParent = hChild;
	do
	{
		hWndParent = ::GetParent(hWndParent);
		if (hWndParent == hParent)
		{
			bParent = TRUE;
			break;
		}
	}
	while (hWndParent != NULL);

	return bParent;
}

BOOL LibUIDK::IsChildWindow(HWND hParent, HWND hChild)
{
	CWnd *pParent = CWnd::FromHandle(hParent);

	BOOL bRet = FALSE;
	CWnd *pChild = pParent->GetNextWindow(GW_CHILD);
	while (pChild != NULL)
	{
		if (pChild->GetSafeHwnd() == hChild)
		{
			bRet = TRUE;
			break;
		}

		pChild = pChild->GetNextWindow(GW_HWNDNEXT);
	}

	return bRet;
}

BOOL LibUIDK::IsWindowVisibleEx(HWND hWnd)
{
	BOOL bRet = FALSE;

	LONG lStyle = GetWindowLong(hWnd, GWL_STYLE);
	if ((lStyle & WS_VISIBLE) == WS_VISIBLE)
	{
		bRet = TRUE;
	}

	return bRet;
}

#if(_MSC_VER <= 1200) // VC6.0

typedef BOOL (WINAPI *UPDATE_LAYERED_WINDOW)(HWND, HDC, POINT *, SIZE *,
	HDC, POINT *, COLORREF, BLENDFUNCTION *, DWORD);
static UPDATE_LAYERED_WINDOW UpdateLayeredWindowFun = NULL;
BOOL LibUIDK::UpdateLayeredWindow(HWND hWnd, HDC hdcDst, POINT *pptDst, SIZE *psize,
	HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags)
{
	if (UpdateLayeredWindowFun == NULL)
	{
		HINSTANCE hInst = LoadLibrary(_T("User32.DLL"));
		if (hInst != NULL)
		{
			UpdateLayeredWindowFun = (UPDATE_LAYERED_WINDOW)GetProcAddress(hInst,
					"UpdateLayeredWindow");
			FreeLibrary(hInst);
		}
	}

	if (UpdateLayeredWindowFun == NULL)
	{
		return FALSE;
	}

	return UpdateLayeredWindowFun(hWnd, hdcDst, pptDst, psize,
			hdcSrc, pptSrc, crKey, pblend, dwFlags);
}

#endif

int LibUIDK::SetLayeredWindow(HWND hWnd, BOOL bSet)
{
	DWORD dwExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);

	if (bSet && (dwExStyle & WS_EX_LAYERED) != WS_EX_LAYERED)
	{
		LibUIDK::IUISetWindowLong(hWnd, GWL_EXSTYLE, dwExStyle | WS_EX_LAYERED);
	}
	else if (!bSet && (dwExStyle & WS_EX_LAYERED) == WS_EX_LAYERED)
	{
		LibUIDK::IUISetWindowLong(hWnd, GWL_EXSTYLE, dwExStyle & ~ WS_EX_LAYERED);
	}

	return 0;
}

BOOL LibUIDK::IsLayeredWindow(HWND hWnd)
{
	LONG lStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
	BOOL bLayered = ((lStyle & WS_EX_LAYERED) == WS_EX_LAYERED);
	return bLayered;
}

// Copy api: AfxInvariantStrICmp in vs2008
int LibUIDK::IUIInvariantStrICmp(const char *pszLeft, const char *pszRight)
{
	return ::CompareStringA(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT),
			NORM_IGNORECASE, pszLeft, -1, pszRight, -1) - CSTR_EQUAL;
}

// Copy api: AfxInvariantStrICmp in vs2008
int LibUIDK::IUIInvariantStrICmp(const wchar_t *pwszLeft, const wchar_t *pwszRight)
{
	return ::CompareStringW(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT),
			NORM_IGNORECASE, pwszLeft, -1, pwszRight, -1) - CSTR_EQUAL;
}

// See API: _AfxCompareClassName. AfxInvariantStrICmp can't be link in VS2013,
// so, use the code of AfxInvariantStrICmp directly.
BOOL LibUIDK::IUICompareClassName(HWND hWnd, LPCTSTR lpszClassName)
{
	ASSERT(::IsWindow(hWnd));
	TCHAR szTemp[32];
	::GetClassName(hWnd, szTemp, _countof(szTemp));
#if(_MSC_VER <= 1310) // VC7.1
	return lstrcmpi(szTemp, lpszClassName) == 0;
#else
	return IUIInvariantStrICmp(szTemp, lpszClassName) == 0;
#endif
}

int LibUIDK::TSPLITPATH(LPCTSTR path,
	LPTSTR drive, size_t driveSizeInCharacters,
	LPTSTR dir, size_t dirSizeInCharacters,
	LPTSTR fname, size_t nameSizeInCharacters,
	LPTSTR ext, size_t extSizeInBytes)
{
	if (path == NULL)
	{
		return -1;
	}
#if(_MSC_VER <= 1310) // VC6.0 or VC7.1
	_tsplitpath(path, drive, dir, fname, ext);
#else
	_tsplitpath_s(path, drive, driveSizeInCharacters, dir, dirSizeInCharacters,
		fname, nameSizeInCharacters, ext, extSizeInBytes);
#endif // (_MSC_VER <= 1310)

	return 0;
}

errno_t LibUIDK::TSTRCPY(LPTSTR strDestination, size_t numberOfElements, LPCTSTR strSource)
{
	errno_t erRet = 0;
#if(_MSC_VER <= 1310) // VC6.0 or VC7.1
	_tcscpy(strDestination, strSource);
#else
	erRet = _tcscpy_s(strDestination, numberOfElements, strSource);
#endif // (_MSC_VER <= 1310)

	return erRet;
}

errno_t LibUIDK::TSTRCAT(LPTSTR strDestination, size_t numberOfElements, LPCTSTR strSource)
{
	errno_t erRet = 0;
#if(_MSC_VER <= 1310) // VC6.0 or VC7.1
	_tcscat(strDestination, strSource);
#else
	erRet = _tcscat_s(strDestination, numberOfElements, strSource);
#endif // (_MSC_VER <= 1310)

	return erRet;
}

size_t LibUIDK::TSTRLEN(LPCTSTR lpszBuf)
{
	return _tcsclen(lpszBuf);
}

LPSTR LibUIDK::nstrchr(LPCSTR pbyte, int ncb, int c)
{
	for (int i = 0; i < ncb ; i++)
	{
		LPSTR pb = LPSTR(pbyte + i);
		if (*pb == c)
		{
			return pb;
		}
	}

	return NULL;
}

LPWSTR LibUIDK::wnstrchr(LPCWSTR pbyte, int ncb, int c)
{
	for (int i = 0; i < ncb ; i++)
	{
		LPWSTR pb = LPWSTR(pbyte + i);
		if (*pb == c)
		{
			return pb;
		}
	}

	return NULL;
}

int LibUIDK::FormatString(CString &str)
{
	CString s = str;
	int nLength = s.GetLength();

	// first, make tab to space
	s.Replace('\t', 0x20);

	// Second, remove more space key.
	for (int j = nLength - 1; j >= 0; --j)
	{
		if (s[j] == 0x20 && j - 1 >= 0 && s[j - 1] == 0x20)
		{
			s.Delete(j, 1);
		}
	}

	return 0;
}

int LibUIDK::RemoveQuotationMarks(CString &str, BOOL bDoubleQuotationMarks)
{
	TCHAR ch = bDoubleQuotationMarks ? '\"' : '\'';

	str.TrimLeft();
	str.TrimRight();

	int nLen = str.GetLength();
	if (nLen < 2)
	{
		return -1;
	}

	if (str[0] == ch && str[nLen - 1] == ch)
	{
		str = str.Mid(1, nLen - 2);
	}

	return 0;
}

BOOL LibUIDK::AfxExtractSubStringA(std::string &rstring,
	LPCSTR lpszFullString, int nFullStringCount, int iSubString, CHAR chSep)
{
	if (lpszFullString == NULL)
	{
		return FALSE;
	}

	while (iSubString--)
	{
		lpszFullString = nstrchr(lpszFullString, nFullStringCount, chSep);
		if (lpszFullString == NULL)
		{
			rstring.erase(rstring.begin(), rstring.end());	// return empty string as well
			return FALSE;
		}
		lpszFullString++;       // point past the separator
	}
	LPCSTR lpchEnd = nstrchr(lpszFullString, nFullStringCount, chSep);
	int nLen = (lpchEnd == NULL) ? nFullStringCount : (int)(lpchEnd - lpszFullString);
	ASSERT(nLen >= 0);
	CHAR *p = new CHAR [nLen + 1];
	memset(p, 0, sizeof(CHAR) * (nLen + 1));
	memcpy(p, lpszFullString, nLen * sizeof(CHAR));

	rstring = p;
	delete [] p;
	return TRUE;
}

BOOL LibUIDK::AfxExtractSubStringW(std::wstring &rstring,
	LPCWSTR lpszFullString, int nFullStringCount, int iSubString, WCHAR chSep)
{
	if (lpszFullString == NULL)
	{
		return FALSE;
	}

	while (iSubString--)
	{
		lpszFullString = wnstrchr(lpszFullString, nFullStringCount, chSep);
		if (lpszFullString == NULL)
		{
			rstring.erase(rstring.begin(), rstring.end());	// return empty string as well
			return FALSE;
		}
		lpszFullString++;       // point past the separator
	}
	LPCWSTR lpchEnd = wnstrchr(lpszFullString, nFullStringCount, chSep);
	int nLen = (lpchEnd == NULL) ? nFullStringCount : (int)(lpchEnd - lpszFullString);
	ASSERT(nLen >= 0);
	WCHAR *p = new WCHAR [nLen + 1];
	memset(p, 0, sizeof(WCHAR) * (nLen + 1));
	memcpy(p, lpszFullString, nLen * sizeof(WCHAR));

	rstring = p;
	delete [] p;
	return TRUE;
}

// UTF16 means Unicode or wide char.
std::wstring LibUIDK::MultiByteToUTF16(LPCSTR lpMultiByteStr, int cbMultiByte, UINT uCodePage)
{
	std::wstring wstrResult;

	// First call to get the required wide char length. not include the terminating null character.
	int cchWideChar = ::MultiByteToWideChar(uCodePage, 0, lpMultiByteStr, cbMultiByte, NULL, 0);
	if (cchWideChar > 0)
	{
		// must +1, MSDN is wrong, the return value of the required buffer size,
		// in WCHAR values not include the terminating null character.
		wchar_t *lpWideCharStr = new wchar_t[cchWideChar + 1];

		// Second call to transform the string to utf-16 format.
		cchWideChar = ::MultiByteToWideChar(uCodePage, 0, lpMultiByteStr, cbMultiByte,
				lpWideCharStr, cchWideChar);

		// Make the last wide char to L'\0'.
		if (cchWideChar > 0)
		{
			lpWideCharStr[cchWideChar] = L'\0';
			wstrResult = lpWideCharStr;
		}

		// Clear memory.
		delete []lpWideCharStr;
	}

	return wstrResult;
}

FILE *LibUIDK::TFOPEN(LPCTSTR filename, LPCTSTR mode)
{
#if(_MSC_VER <= 1310) // VC6.0 or VC7.1
	return _tfopen(filename, mode);
#else
	FILE *p = NULL;
	errno_t t = _tfopen_s(&p, filename, mode);
	if (t == 0)
	{
		return p;
	}
	else
	{
		return NULL;
	}
#endif // (_MSC_VER <= 1310)
}

errno_t LibUIDK::FOPENW(FILE **pFile, LPCWSTR filename, LPCWSTR mode)
{
#if(_MSC_VER <= 1310) // VC6.0 or VC7.1
	if (pFile == NULL)
	{
		return -1;
	}
	FILE *fp = _wfopen(filename, mode);
	if (fp == NULL)
	{
		return EINVAL;
	}

	*pFile = fp;

	return 0;
#else
	return _wfopen_s(pFile, filename, mode);
#endif // (_MSC_VER <= 1310)
}

// return drive name.
CString LibUIDK::GetFileDrive(const CString &strFile)
{
	CString strRet;

	TCHAR szDrive[MAX_PATH] = {0};
	TSPLITPATH(strFile, szDrive, MAX_PATH, NULL, 0, NULL, 0, NULL, 0);
	strRet = szDrive;

	return strRet;
}

// return only path, not include title and ext.
CString LibUIDK::GetFilePath(const CString &strFile)
{
	CString strRet;
	TCHAR szBuf[MAX_PATH] = {0};
	TSTRCPY(szBuf, MAX_PATH, strFile);
	PathRemoveFileSpec(szBuf);
	PathAddBackslash(szBuf);
	strRet = szBuf;

	return strRet;
}

// return title and ext.
CString LibUIDK::GetFileName(const CString &strFile)
{
	CString strRet;

	TCHAR szTitle[MAX_PATH] = {0};
	TCHAR szExt[MAX_PATH] = {0};
	TSPLITPATH(strFile, NULL, 0, NULL, 0, szTitle, MAX_PATH, szExt, MAX_PATH);
	strRet = szTitle;
	strRet += szExt;

	return strRet;
}

std::string LibUIDK::GetFileTitleExA(LPCSTR lpszFile)
{
	std::string strRet;
	if (lpszFile == NULL)
	{
		return strRet;
	}

	CHAR szName[MAX_PATH] = {0};
#if(_MSC_VER <= 1310) // VC6.0 or VC7.1
	_splitpath(lpszFile, NULL, NULL, szName, NULL);
#else
	_splitpath_s(lpszFile, NULL, 0, NULL, 0, szName, MAX_PATH, NULL, 0);
#endif // (_MSC_VER <= 1310)

	strRet = szName;
	return strRet;
}

std::wstring LibUIDK::GetFileTitleExW(LPCWSTR lpszFile)
{
	std::wstring wstrRet;
	if (lpszFile == NULL)
	{
		return wstrRet;
	}

	WCHAR szName[MAX_PATH] = {0};
#if(_MSC_VER <= 1310) // VC6.0 or VC7.1
	_wsplitpath(lpszFile, NULL, NULL, szName, NULL);
#else
	_wsplitpath_s(lpszFile, NULL, 0, NULL, 0, szName, MAX_PATH, NULL, 0);
#endif // (_MSC_VER <= 1310)

	wstrRet = szName;
	return wstrRet;
}

// return only title, not include ext.
CString LibUIDK::GetFileTitleEx(const CString &strFile)
{
	CString strRet;
	TCHAR szName[MAX_PATH] = {0};
	TSPLITPATH(strFile, NULL, 0, NULL, 0, szName, MAX_PATH, NULL, 0);
	strRet = szName;
	return strRet;
}

std::string LibUIDK::GetFileExtA(LPCSTR lpszFile, BOOL bHasDot/* = TRUE*/)
{
	std::string strRet;
	if (lpszFile == NULL)
	{
		return strRet;
	}

	CHAR szExt[MAX_PATH] = {0};
#if(_MSC_VER <= 1310) // VC6.0 or VC7.1
	_splitpath(lpszFile, NULL, NULL, NULL, szExt);
#else
	_splitpath_s(lpszFile, NULL, 0, NULL, 0, NULL, 0, szExt, MAX_PATH);
#endif // (_MSC_VER <= 1310)

	strRet = szExt;

	// Remove dot
	if (!bHasDot && !strRet.empty() && strRet[0] == '.')
	{
		strRet.erase(strRet.begin());
	}

	return strRet;
}

std::wstring LibUIDK::GetFileExtW(LPCWSTR lpszFile, BOOL bHasDot/* = TRUE*/)
{
	std::wstring wstrRet;
	if (lpszFile == NULL)
	{
		return wstrRet;
	}

	WCHAR szExt[MAX_PATH] = {0};
#if(_MSC_VER <= 1310) // VC6.0 or VC7.1
	_wsplitpath(lpszFile, NULL, NULL, NULL, szExt);
#else
	_wsplitpath_s(lpszFile, NULL, 0, NULL, 0, NULL, 0, szExt, MAX_PATH);
#endif // (_MSC_VER <= 1310)

	wstrRet = szExt;

	// Remove dot
	if (!bHasDot && !wstrRet.empty() && wstrRet[0] == '.')
	{
		wstrRet.erase(wstrRet.begin());
	}

	return wstrRet;
}

CString LibUIDK::GetFileExt(LPCTSTR lpszFile, BOOL bHasDot/* = TRUE*/)
{
	CString strRet;

	// 一定要把lpszFile赋值为CString，因为传入的lpszFile如果是数字强转成字符串的话
	// 用这样的lpszFile调用TSPLITPATH有可能会崩溃。
	CString strFile = lpszFile;
	TCHAR szExt[MAX_PATH] = {0};
	TSPLITPATH(strFile, NULL, 0, NULL, 0, NULL, 0, szExt, MAX_PATH);
	strRet = szExt;
	if (!bHasDot && !strRet.IsEmpty() && strRet.Left(1) == '.')
	{
		strRet.Delete(0, 1);
	}

	return strRet;
}

int LibUIDK::SplitPath(const CString &strFile, CString &strPath, CString &strTitle, CString &strExt)
{
	TCHAR szDrive[MAX_PATH] = {0};
	TCHAR szPath[MAX_PATH] = {0};
	TCHAR szTitle[MAX_PATH] = {0};
	TCHAR szExt[MAX_PATH] = {0};
	TSPLITPATH(strFile, szDrive, MAX_PATH, szPath, MAX_PATH, szTitle, MAX_PATH, szExt, MAX_PATH);

	strPath = CString(szDrive) + szPath;
	strTitle = szTitle;
	strExt = szExt;

	return 0;
}

int LibUIDK::CheckFileWritable(const CString &strFile, BOOL bWarning/* = TRUE*/)
{
	BOOL bRet = PathFileExists(strFile);
	if (!bRet)
	{
		// If file not exist, create it first.
		FILE *fp = TFOPEN(strFile, _T("wb"));
		if (fp == NULL)
		{
			return -1;
		}
		fclose(fp);
	}

	DWORD dwAttrib = GetFileAttributes(strFile);
	if (dwAttrib & FILE_ATTRIBUTE_READONLY)
	{
		if (bWarning)
		{
			CString strInfo;
			strInfo.Format(_T("The file \"%s\" is readonly!"), strFile);
			AfxMessageBox(strInfo);
		}
		return -2;
	}

	return 0;
}

int LibUIDK::CreateFileFolder(const CString &strFullPath)
{
	CString strDrive = GetFileDrive(strFullPath);
	if (!PathFileExists(strDrive))
	{
		return -1;
	}

	CString strFull = strFullPath;
	strFull.Replace('/', '\\');

	CString strFolder;
	int i = 0;

	CString strPath;
	while (AfxExtractSubString(strFolder, strFull, i++, '\\'))
	{
		if (strFull.IsEmpty())
		{
			break;
		}

		strPath += strFolder;

		if (!PathFileExists(strPath))
		{
			CreateDirectory(strPath, NULL);
		}
		strPath += '\\';
	}

	return 0;
}

CString LibUIDK::GetExecutePath()
{
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	PathRemoveFileSpec(szPath);
	PathAddBackslash(szPath);

	return szPath;
}

BOOL LibUIDK::PathRemoveFileSpecEx(CString &rstrPath, BOOL bHasEndBackslash)
{
	CString strTemp = rstrPath;
	strTemp.Replace('/', '\\');
	TCHAR szPath[MAX_PATH] = {0};
	TSTRCPY(szPath, MAX_PATH, strTemp);

	BOOL bRet = PathRemoveFileSpec(szPath);
	strTemp = szPath;
	if (bHasEndBackslash)
	{
		if (strTemp.Right(1) != '\\' && strTemp.Right(1) != '/')
		{
			strTemp += '\\';
		}
	}

	rstrPath = strTemp;
	return bRet;
}

BOOL LibUIDK::OpenForEditInPerforce(const CString &strFullPath)
{
	CString strCmd;
	strCmd.Format(_T("p4 edit \"%s\""), strFullPath);
	WinExec(_bstr_t(strCmd), SW_HIDE);

	// wait for open for edit finish or timeout
	DWORD dw = GetTickCount();
	while (true)
	{
		DWORD dwAttrib = GetFileAttributes(strFullPath);
		if ((dwAttrib & FILE_ATTRIBUTE_READONLY) != FILE_ATTRIBUTE_READONLY)
		{
			break;
		}
		if (GetTickCount() - dw > 500)
		{
			break;
		}
	}

	DWORD dwAttrib = GetFileAttributes(strFullPath);
	if (dwAttrib & FILE_ATTRIBUTE_READONLY)
	{
		return FALSE;
	}

	return TRUE; // success to open for edit
}

CString LibUIDK::StringFromInt(int nValue)
{
	TCHAR szValue[64] = {0};
#if(_MSC_VER <= 1310) // VC7.1
	_itot(nValue, szValue, 10);
#else
	_itot_s(nValue, szValue, 10);
#endif

	return szValue;
}

CString LibUIDK::StringFromUINT(UINT n)
{
	TCHAR szBuf[32] = {0};
#if(_MSC_VER <= 1310) // VC7.1
	_ultot(n, szBuf, 10);
#else
	_ultot_s(n, szBuf, 10);
#endif
	return szBuf;
}

CString LibUIDK::HexStringFromInt(int n, BOOL bHasPrefix, BOOL b0Fill, int nBits/* = 8*/)
{
	CString strRet;
	if (bHasPrefix)
	{
		if (b0Fill)
		{
			CString strFormat;
			strFormat.Format(_T("0x%%0%dX"), nBits);
			strRet.Format(strFormat, n);
		}
		else
		{
			strRet.Format(_T("0x%X"), n);
		}
	}
	else
	{
		if (b0Fill)
		{
			CString strFormat;
			strFormat.Format(_T("%%0%dX"), nBits);
			strRet.Format(strFormat, n);
		}
		else
		{
			strRet.Format(_T("%X"), n);
		}
	}
	return strRet;
}

CString LibUIDK::HexStringFromUINT(UINT n, BOOL bHasPrefix, BOOL b0Fill, int nBits/* = 8*/)
{
	CString strRet;
	if (bHasPrefix)
	{
		if (b0Fill)
		{
			CString strFormat;
			strFormat.Format(_T("0x%%0%dX"), nBits);
			strRet.Format(strFormat, n);
		}
		else
		{
			strRet.Format(_T("0x%X"), n);
		}
	}
	else
	{
		if (b0Fill)
		{
			CString strFormat;
			strFormat.Format(_T("%%0%dX"), nBits);
			strRet.Format(strFormat, n);
		}
		else
		{
			strRet.Format(_T("%X"), n);
		}
	}
	return strRet;
}

unsigned long LibUIDK::UINTFromString(LPCTSTR lpString)
{
	unsigned long ulValue = 0;
	CString strValue = lpString;
	if (strValue.Left(2) == _T("0x"))
	{
		ulValue = _tcstoul(strValue, NULL, 16);
	}
	else
	{
		ulValue = _tcstoul(strValue, NULL, 10);
	}

	return ulValue;
}

unsigned long LibUIDK::UINTFromHexString(LPCTSTR lpString)
{
	unsigned long ulValue = _tcstoul(lpString, NULL, 16);

	return ulValue;
}

int LibUIDK::IntFromString(LPCTSTR lpString)
{
	long lValue = 0;
	CString strValue(lpString);
	if (strValue.Left(2) == _T("0x"))
	{
		lValue = _tcstol(strValue, NULL, 16);
	}
	else
	{
		lValue = _tcstol(strValue, NULL, 10);
	}

	return lValue;
}

int LibUIDK::IntFromHexString(LPCTSTR lpString)
{
	long lValue = _tcstol(lpString, NULL, 16);

	return lValue;
}

long LibUIDK::LongFromString(LPCTSTR lpString)
{
	long lValue = 0;
	CString strValue(lpString);

	if (strValue.Left(2) == _T("0x"))
	{
		lValue = _tcstol(strValue, NULL, 16);
	}
	else
	{
		lValue = _tcstol(strValue, NULL, 10);
	}

	return lValue;
}

bool LibUIDK::boolFromString(LPCTSTR lpString)
{
	bool bRet = true;

	CString strValue(lpString);
	if (strValue.CompareNoCase(_T("False")) == 0 || strValue.CompareNoCase(_T("0")) == 0)
	{
		bRet = false;
	}

	return bRet;
}

CString LibUIDK::StringFromVariant(const _variant_t &rVariant)
{
	CString strRet;
	if (rVariant.vt == VT_NULL)
	{
		return strRet;
	}

#if(_MSC_VER <= 1200) // VC6.0
	strRet = (TCHAR *)_bstr_t(rVariant);
#else
	strRet = rVariant;
#endif

	return strRet;
}

int LibUIDK::IntFromVariant(const _variant_t &rVariant)
{
	int nRet;
#if(_MSC_VER <= 1200) // VC6.0
	nRet = (LONG)rVariant;
#else
	nRet = rVariant;
#endif

	return nRet;
}

_variant_t LibUIDK::VariantFromInt(int nValue)
{
	_variant_t varRet;
#if(_MSC_VER <= 1200) // VC6.0
	varRet = (LONG)nValue;
#else
	varRet = nValue;
#endif

	return varRet;
}

BOOL LibUIDK::IsIntegerValue(const CString &strValue)
{
	BOOL bRet = TRUE;

	int nLength = strValue.GetLength();
	for (int i = 0; i < nLength; ++i)
	{
		TCHAR chChar = strValue[i];
		if (chChar < '0' || chChar > '9')
		{
			bRet = FALSE;
			break;
		}
	}

	return bRet;
}

void LibUIDK::IUIScreenToClient(HWND hWnd, LPRECT lpRect)
{
	ASSERT(::IsWindow(hWnd));
	::ScreenToClient(hWnd, (LPPOINT)lpRect);
	::ScreenToClient(hWnd, ((LPPOINT)lpRect) + 1);

	LONG_PTR dwStyle = IUIGetWindowLong(hWnd, GWL_EXSTYLE);
	if (dwStyle & WS_EX_LAYOUTRTL)
	{
		CRect::SwapLeftRight(lpRect);
	}
}

int LibUIDK::GetRectWidth(LPCRECT lpRect)
{
	if (lpRect == NULL)
	{
		return 0;
	}

	return lpRect->right - lpRect->left;
}

int LibUIDK::GetRectHeight(LPCRECT lpRect)
{
	if (lpRect == NULL)
	{
		return 0;
	}

	return lpRect->bottom - lpRect->top;
}

// 1 twips equal to 1/1440 inch.
int LibUIDK::GetXTwipsFromPixel(int nPixel)
{
	HDC hDCN = ::GetDC(::GetDesktopWindow());
	int nXDPI = GetDeviceCaps(hDCN, LOGPIXELSX);
	::ReleaseDC(::GetDesktopWindow(), hDCN);

	if (nXDPI == 0)
	{
		nXDPI = 96;
	}

	int nRet = MulDiv(nPixel, 1440, nXDPI);

	return nRet;
}

int LibUIDK::GetYTwipsFromPixel(int nPixel)
{
	HDC hDCN = ::GetDC(::GetDesktopWindow());
	int nYDPI = GetDeviceCaps(hDCN, LOGPIXELSY);
	::ReleaseDC(::GetDesktopWindow(), hDCN);

	if (nYDPI == 0)
	{
		nYDPI = 96;
	}

	int nRet = MulDiv(nPixel, 1440, nYDPI);

	return nRet;
}

int LibUIDK::GetXPixelFromTwips(int nTwips)
{
	HDC hDCN = ::GetDC(::GetDesktopWindow());
	int nYDPI = GetDeviceCaps(hDCN, LOGPIXELSY);
	::ReleaseDC(::GetDesktopWindow(), hDCN);

	if (nYDPI == 0)
	{
		nYDPI = 96;
	}

	int nRet = MulDiv(nTwips, nYDPI, 1440);

	return nRet;
}

int LibUIDK::GetYPixelFromTwips(int nTwips)
{
	HDC hDCN = ::GetDC(::GetDesktopWindow());
	int nYDPI = GetDeviceCaps(hDCN, LOGPIXELSY);
	::ReleaseDC(::GetDesktopWindow(), hDCN);

	if (nYDPI == 0)
	{
		nYDPI = 96;
	}

	int nRet = MulDiv(nTwips, nYDPI, 1440);

	return nRet;
}

// 1 FPPTS equal to 1/20 twips
float LibUIDK::GetXFPPTSFromPixel(int nPixel)
{
	HDC hDCN = ::GetDC(::GetDesktopWindow());
	int nXDPI = GetDeviceCaps(hDCN, LOGPIXELSX);
	::ReleaseDC(::GetDesktopWindow(), hDCN);

	if (nXDPI == 0)
	{
		nXDPI = 96;
	}

	float nRet = (float)MulDiv(nPixel, 72, nXDPI); // 72 = 1440 * 0.05

	return nRet;
}

float LibUIDK::GetYFPPTSFromPixel(int nPixel)
{
	HDC hDCN = ::GetDC(::GetDesktopWindow());
	int nYDPI = GetDeviceCaps(hDCN, LOGPIXELSY);
	::ReleaseDC(::GetDesktopWindow(), hDCN);

	if (nYDPI == 0)
	{
		nYDPI = 96;
	}

	float nRet = (float)MulDiv(nPixel, 72, nYDPI); // 72 = 1440 * 0.05

	return nRet;
}

// 1 himetric equal to 1/100 mm.
int LibUIDK::GetXHimetricFromPixel(int nPixel)
{
	HDC hDCN = ::GetDC(::GetDesktopWindow());
	int nXDPI = GetDeviceCaps(hDCN, LOGPIXELSX);
	::ReleaseDC(::GetDesktopWindow(), hDCN);
	int nRet = MulDiv(nPixel, 2540, nXDPI);

	return nRet;
}

int LibUIDK::GetYHimetricFromPixel(int nPixel)
{
	HDC hDCN = ::GetDC(::GetDesktopWindow());
	int nYDPI = GetDeviceCaps(hDCN, LOGPIXELSY);
	::ReleaseDC(::GetDesktopWindow(), hDCN);
	int nRet = MulDiv(nPixel, 2540, nYDPI);

	return nRet;
}

CTime LibUIDK::VARIANT2CTime(_variant_t var)
{
	ASSERT(var.vt == VT_DATE);
	SYSTEMTIME st;
	VariantTimeToSystemTime(var, &st);
	CTime time(st);
	return time;
}

bool LibUIDK::IsGUIDZero(const GUID &guid)
{
	GUID g;
	memset(&g, 0, sizeof(GUID));

	bool bRet = ((memcmp(&guid, &g, sizeof(GUID)) == 0) ? true : false);
	return bRet;
}

int LibUIDK::SwitchWordEndian(const WORD *pszStream, WORD *pszTarget)
{
	ASSERT(pszStream != NULL && pszTarget != NULL);

	int i = 0;
	WORD w = pszStream[i];

	while (w != 0)
	{
		pszTarget[i] = MAKEWORD(HIBYTE(w), LOBYTE(w));
		i++;
		w = pszStream[i];
	}

	pszTarget[i] = 0;

	return 0;
}

std::string LibUIDK::GetHexCodeA(LPCSTR lpszSource)
{
	size_t nLen = strlen(lpszSource);

	CString strRet;
	for (size_t j = 0; j < nLen; ++j)
	{
		BYTE bt = lpszSource[j];

		BYTE nHi = bt >> 4;
		BYTE nLo = bt << 4;
		nLo = nLo >> 4;

		strRet += HexStringFromInt(nHi, FALSE, FALSE, 1);
		strRet += HexStringFromInt(nLo, FALSE, FALSE, 1);
	}

	return (LPCSTR)_bstr_t(strRet);
}

std::string LibUIDK::GetHexCodeW(LPCWSTR lpszSource)
{
	size_t nLen = wcslen(lpszSource);

	size_t nSize = nLen * sizeof(WCHAR);
	BYTE *pData = new BYTE[nSize];

	// Ignore 0.
	size_t nIndex = 0;
	BYTE *pSource = (BYTE *)lpszSource;
	for (size_t i = 0; i < nSize; ++i)
	{
		if (pSource[i] != 0)
		{
			pData[nIndex++] = pSource[i];
		}
	}

	CString strRet;
	for (size_t j = 0; j < nIndex; ++j)
	{
		BYTE bt = pData[j];

		BYTE nHi = bt >> 4;
		BYTE nLo = bt << 4;
		nLo = nLo >> 4;

		strRet += HexStringFromInt(nHi, FALSE, FALSE, 1);
		strRet += HexStringFromInt(nLo, FALSE, FALSE, 1);
	}

	delete [] pData;

	return (LPCSTR)_bstr_t(strRet);
}

std::string LibUIDK::GetHexCode(LPCTSTR lpszSource)
{
#ifdef _UNICODE
	return GetHexCodeW(lpszSource);
#else
	return GetHexCodeA(lpszSource);
#endif
}

BOOL LibUIDK::IsStringExist(const std::vector<CString> *pvStrings, const CString &rString,
	BOOL bNoCase)
{
	BOOL bFind = FALSE;

	size_t nCount = pvStrings->size();
	for (size_t i = 0; i < nCount; ++i)
	{
		CString strString = (*pvStrings)[i];

		if (bNoCase)
		{
			if (strString.CompareNoCase(rString) == 0)
			{
				bFind = TRUE;
				break;
			}
		}
		else
		{
			if (strString.Compare(rString) == 0)
			{
				bFind = TRUE;
				break;
			}
		}
	}

	return bFind;
}

BOOL LibUIDK::IsIncludeFlag(UINT_PTR uFlags, UINT_PTR uFlag)
{
	if (uFlag == 0)
	{
		if ((uFlags & 1) == 1)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	BOOL bInclude = ((uFlags & uFlag) == uFlag);

	return bInclude;
}

//////////////////////////////////////////////////////////////////////////
// For register

BOOL LibUIDK::IUISetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName/* = NULL*/)
{
	if (lpszValueName == NULL)
	{
		if (::RegSetValue(HKEY_CLASSES_ROOT, lpszKey, REG_SZ,
				lpszValue, lstrlen(lpszValue) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			TRACE1("[LibUIDK]===Warning: registration database update failed for key '%s'.\n",
				lpszKey);
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		HKEY hKey;

		if (::RegCreateKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) == ERROR_SUCCESS)
		{
			LONG lResult = ::RegSetValueEx(hKey, lpszValueName, 0, REG_SZ,
					(CONST BYTE *)lpszValue, (lstrlen(lpszValue) + 1) * sizeof(TCHAR));

			if (::RegCloseKey(hKey) == ERROR_SUCCESS && lResult == ERROR_SUCCESS)
			{
				return TRUE;
			}
		}
		TRACE1("[LibUIDK]===Warning: registration database update failed for key '%s'.\n", lpszKey);
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
// For LibUIDK

CString LibUIDK::GetControlTypeString(CONTROL_TYPE eControlType)
{
	CString strType;

	if (eControlType == CT_ARROW)
	{
		strType = _T("ARROW");
	}
	else if (eControlType == CT_PUSHBUTTON)
	{
		strType = _T("PUSHBUTTON");
	}
	else if (eControlType == CT_CHECK)
	{
		strType = _T("CHECK");
	}
	else if (eControlType == CT_RADIO)
	{
		strType = _T("RADIO");
	}
	else if (eControlType == CT_COMBOBOX)
	{
		strType = _T("COMBOBOX");
	}
	else if (eControlType == CT_STATIC)
	{
		strType = _T("STATIC");
	}
	else if (eControlType == CT_EDIT)
	{
		strType = _T("EDIT");
	}
	else if (eControlType == CT_SLIDER)
	{
		strType = _T("SLIDER");
	}
	else if (eControlType == CT_PROGRESS)
	{
		strType = _T("PROGRESS");
	}
	else if (eControlType == CT_SPIN)
	{
		strType = _T("SPIN");
	}
	else if (eControlType == CT_HOTKEY)
	{
		strType = _T("HOTKEY");
	}
	else if (eControlType == CT_IPADDRESS)
	{
		strType = _T("IPADDRESS");
	}
	else if (eControlType == CT_SLIDEREX)
	{
		strType = _T("SLIDEREX");
	}
	else if (eControlType == CT_LISTCTRL)
	{
		strType = _T("LISTCTRL");
	}
	else if (eControlType == CT_TREECTRL)
	{
		strType = _T("TREECTRL");
	}
	else if (eControlType == CT_RICHEDIT)
	{
		strType = _T("RICHEDIT");
	}
	else if (eControlType == CT_RICHEDIT_IM)
	{
		strType = _T("IMRE");
	}
	else if (eControlType == CT_SCROLLBAR)
	{
		strType = _T("SCROLLBAR");
	}
	else if (eControlType == CT_SCROLLBAREX)
	{
		strType = _T("SCROLLBAREX");
	}
	else if (eControlType == CT_TREELIST)
	{
		strType = _T("TREELIST");
	}
	else if (eControlType == CT_THUMBNAIL)
	{
		strType = _T("THUMBNAIL");
	}
	else if (eControlType == CT_HTMLCTRL)
	{
		strType = _T("HTMLCTRL");
	}
	else if (eControlType == CT_DATETIMECTRL)
	{
		strType = _T("DATETIMECTRL");
	}
	else if (eControlType == CT_WAVECTRL)
	{
		strType = _T("WAVECTRL");
	}
	else if (eControlType == CT_PICTURE)
	{
		strType = _T("PICCTRL");
	}
	else if (eControlType == CT_MENU)
	{
		strType = _T("MENU");
	}
	else if (eControlType == CT_MENUBAR)
	{
		strType = _T("MENUBAR");
	}
	else if (eControlType == CT_HEADERCTRL)
	{
		strType = _T("HEADERCTRL");
	}
	else if (eControlType == CT_HORSCROLLBAR)
	{
		strType = _T("HORSCROLLBAR");
	}
	else if (eControlType == CT_VERSCROLLBAR)
	{
		strType = _T("VERSCROLLBAR");
	}
	else if (eControlType == CT_SPLITTER)
	{
		strType = _T("SPLITTER");
	}
	// windowless control
	else if (eControlType == CT_WL_RECTCTRL)
	{
		strType = _T("WL_RECTCTRL");
	}
	else if (eControlType == CT_WL_SPLITTER)
	{
		strType = _T("WL_SPLITTER");
	}
	else if (eControlType == CT_WL_LINE)
	{
		strType = _T("WL_LINE");
	}
	else if (eControlType == CT_WL_TEXT)
	{
		strType = _T("WL_TEXT");
	}
	else if (eControlType == CT_WL_BUTTON)
	{
		strType = _T("WL_BUTTON");
	}
	else if (eControlType == CT_WL_CHECK)
	{
		strType = _T("WL_CHECK");
	}
	else if (eControlType == CT_WL_RADIO)
	{
		strType = _T("WL_RADIO");
	}
	else if (eControlType == CT_WL_SLIDER)
	{
		strType = _T("WL_SLIDER");
	}
	else if (eControlType == CT_WL_PICTURE)
	{
		strType = _T("WL_PICTURE");
	}
	else if (eControlType == CT_WL_RICHEDIT)
	{
		strType = _T("WL_RICHEDIT");
	}
	else if (eControlType == CT_WL_RICHEDIT_IM)
	{
		strType = _T("WL_IM");
	}
	// TaskWndMgr
	else if (eControlType == CT_TASK_WND_MGR)
	{
		strType = _T("TWMGR");
	}
	// Panel
	else if (eControlType == CT_PNL_DOCK)
	{
		strType = _T("DOCKPANEL");
	}
	else if (eControlType == CT_UNIFORM_GRID)
	{
		strType = _T("UGRIDPANEL");
	}
	else if (eControlType == CT_PNL_STACK)
	{
		strType = _T("STACKPANEL");
	}

	else if (eControlType == CT_UIWND)
	{
		strType = _T("UIWND");
	}
	// Resource
	else if (eControlType == CT_RESOURCE_DEFAULT_FONT)
	{
		strType = _T("DEFAULT_FONT");
	}
	else if (eControlType == CT_RESOURCE_GENERAL_FONT)
	{
		strType = _T("FONT");
	}
	else if (eControlType == CT_RESOURCE_CONDITION_FONT)
	{
		strType = _T("CONDITION_FONT");
	}
	else if (eControlType == CT_RESOURCE_DEFAULT_COLOR)
	{
		strType = _T("DEFAULT_COLOR");
	}
	else if (eControlType == CT_RESOURCE_COLOR)
	{
		strType = _T("COLOR");
	}

	else if (eControlType == CT_STYLE_PUSHBUTTON)
	{
		strType = _T("STYLE_PUSHBUTTON");
	}
	else if (eControlType == CT_STYLE_CHECK)
	{
		strType = _T("STYLE_CHECK");
	}
	else if (eControlType == CT_STYLE_RADIO)
	{
		strType = _T("STYLE_RADIO");
	}
	else if (eControlType == CT_STYLE_COMBOBOX)
	{
		strType = _T("STYLE_COMBOBOX");
	}
	else if (eControlType == CT_STYLE_STATIC)
	{
		strType = _T("STYLE_STATIC");
	}
	else if (eControlType == CT_STYLE_EDIT)
	{
		strType = _T("STYLE_EDIT");
	}
	else if (eControlType == CT_STYLE_SLIDER)
	{
		strType = _T("STYLE_SLIDER");
	}
	else if (eControlType == CT_STYLE_PROGRESS)
	{
		strType = _T("STYLE_PROGRESS");
	}
	else if (eControlType == CT_STYLE_SPIN)
	{
		strType = _T("STYLE_SPIN");
	}
	else if (eControlType == CT_STYLE_HOTKEY)
	{
		strType = _T("STYLE_HOTKEY");
	}
	else if (eControlType == CT_STYLE_IPADDRESS)
	{
		strType = _T("STYLE_IPADDRESS");
	}
	else if (eControlType == CT_STYLE_SLIDEREX)
	{
		strType = _T("STYLE_SLIDEREX");
	}
	else if (eControlType == CT_STYLE_LISTCTRL)
	{
		strType = _T("STYLE_LISTCTRL");
	}
	else if (eControlType == CT_STYLE_TREECTRL)
	{
		strType = _T("STYLE_TREECTRL");
	}
	else if (eControlType == CT_STYLE_TREELIST)
	{
		strType = _T("STYLE_TREELIST");
	}
	else if (eControlType == CT_STYLE_RICHEDIT)
	{
		strType = _T("STYLE_RICHEDIT");
	}
	else if (eControlType == CT_STYLE_IMRICHEDIT)
	{
		strType = _T("STYLE_IMRICHEDIT");
	}
	else if (eControlType == CT_STYLE_HTMLCTRL)
	{
		strType = _T("STYLE_HTMLCTRL");
	}
	else if (eControlType == CT_STYLE_DATETIMECTRL)
	{
		strType = _T("STYLE_DATETIMECTRL");
	}
	else if (eControlType == CT_STYLE_WAVECTRL)
	{
		strType = _T("STYLE_WAVECTRL");
	}
	else if (eControlType == CT_STYLE_MENU)
	{
		strType = _T("STYLE_MENU");
	}
	else if (eControlType == CT_STYLE_MENUBAR)
	{
		strType = _T("STYLE_MENUBAR");
	}
	else if (eControlType == CT_STYLE_HEADERCTRL)
	{
		strType = _T("STYLE_HEADERCTRL");
	}
	else if (eControlType == CT_STYLE_HORSCROLLBAR)
	{
		strType = _T("STYLE_HORSCROLLBAR");
	}
	else if (eControlType == CT_STYLE_VERSCROLLBAR)
	{
		strType = _T("STYLE_VERSCROLLBAR");
	}
	else if (eControlType == CT_STYLE_SPLITTER)
	{
		strType = _T("STYLE_SPLITTER");
	}
	else if (eControlType == CT_STYLE_WL_LINE)
	{
		strType = _T("STYLE_WL_LINE");
	}
	else if (eControlType == CT_STYLE_WL_PICTURE)
	{
		strType = _T("STYLE_WL_PICTURE");
	}

	return strType;
}

CONTROL_TYPE LibUIDK::GetControlTypeEnum(LPCTSTR lpszControlType)
{
	CONTROL_TYPE ect = CT_ARROW;

	for (int i = CT_ARROW; i < CT_LAST; ++i)
	{
		CString strTypeString = GetControlTypeString((CONTROL_TYPE)i);
		if (strTypeString == lpszControlType)
		{
			ect = (CONTROL_TYPE)i;
			break;
		}
	}

	return ect;
}

int LibUIDK::CreateControlFont(__out HFONT *phFont, __in FONTRESOURCEITEM *pResFontAs,
	const LOGFONT *plfDefault, const CString &strCurrentFont)
{
	if (phFont == NULL)
	{
		return -1;
	}

	HFONT hFont = CreateControlFont(pResFontAs, plfDefault, strCurrentFont);
	if (hFont == NULL)
	{
		return -2;
	}

	*phFont = hFont;

	return 0;
}

HFONT LibUIDK::CreateControlFont(__in FONTRESOURCEITEM *pResFontAs,
	const LOGFONT *plfDefault, const CString &strCurrentFont)
{
	HFONT hFont = NULL;

	if (pResFontAs == NULL)
	{
		if (CUIMgr::IsEnableDPI())
		{
			LOGFONT lfDPI;
			memcpy(&lfDPI, plfDefault, sizeof(LOGFONT));
			GetDPILogFont(&lfDPI);

			hFont = CreateFontIndirect(&lfDPI);
		}
		else
		{
			hFont = CreateFontIndirect(plfDefault);
		}
	}
	else
	{
		if (pResFontAs->m_bConditionFont)
		{
			RESFONTPROPERTIES *pConditionFont = pResFontAs->GetConditionFont(strCurrentFont);
			if (pConditionFont == NULL)
			{
				return NULL;
			}

			if (CUIMgr::IsEnableDPI())
			{
				LOGFONT lfDPI;
				memcpy(&lfDPI, &(LOGFONT)pConditionFont->m_logFont, sizeof(LOGFONT));
				GetDPILogFont(&lfDPI);

				hFont = CreateFontIndirect(&lfDPI);
			}
			else
			{
				hFont = CreateFontIndirect(&(LOGFONT)pConditionFont->m_logFont);
			}
		}
		else
		{
			ASSERT(pResFontAs->m_pFontResProp != NULL);

			if (CUIMgr::IsEnableDPI())
			{
				LOGFONT lfDPI;
				memcpy(&lfDPI, &(LOGFONT)pResFontAs->m_pFontResProp->m_logFont, sizeof(LOGFONT));
				GetDPILogFont(&lfDPI);

				hFont = CreateFontIndirect(&lfDPI);
			}
			else
			{
				hFont = CreateFontIndirect(&(LOGFONT)pResFontAs->m_pFontResProp->m_logFont);
			}
		}
	}

	return hFont;
}

//////////////////////////////////////////////////////////////////////////
// CIUIImage style

int LibUIDK::IUIGetBackgroundFromParent2(const CTRLPROPERTIES *pCtrlProp, const CRect &rcCtrl,
	HIUIIMAGE hParentBKN, HIUIIMAGE hParentBKD,
	__out HIUIIMAGE *phCtrlBKN, __out HIUIIMAGE *phCtrlBKD)
{
	if (hParentBKN->GetSafeHBITMAP() != NULL && phCtrlBKN != NULL)
	{
		CIUIImage::SubBitmap(hParentBKN, phCtrlBKN, rcCtrl);
	}

	if (hParentBKD->GetSafeHBITMAP() != NULL && phCtrlBKD != NULL)
	{
		CIUIImage::SubBitmap(hParentBKD, phCtrlBKD, rcCtrl);
	}

	return 0;
}

DWORD LibUIDK::GetControlBaseStyle(CTRLPROPERTIES *pCtrlProp)
{
	DWORD dwStyle = 0;

	if (pCtrlProp->m_bVisible)
	{
		dwStyle |= WS_VISIBLE;
	}
	if (pCtrlProp->m_bDisabled)
	{
		dwStyle |= WS_DISABLED;
	}
	if (pCtrlProp->m_bTabStop)
	{
		dwStyle |= WS_TABSTOP;
	}
	if (pCtrlProp->m_bClipChildren)
	{
		dwStyle |= WS_CLIPCHILDREN;
	}
	if (pCtrlProp->m_bClipSiblings)
	{
		dwStyle |= WS_CLIPSIBLINGS;
	}

	return dwStyle;
}

LONG_PTR LibUIDK::IUISetWindowLong(HWND hWnd, int nIndex, LONG_PTR dwNewLong)
{
#if(_MSC_VER <= 1200) // VC6.0
	return ::SetWindowLong(hWnd, nIndex, dwNewLong);
#else
	return ::SetWindowLongPtr(hWnd, nIndex, dwNewLong);
#endif
}

LONG_PTR LibUIDK::IUIGetWindowLong(HWND hWnd, int nIndex)
{
#if(_MSC_VER <= 1200) // VC6.0
	return ::GetWindowLong(hWnd, nIndex);
#else
	return ::GetWindowLongPtr(hWnd, nIndex);
#endif
}

int LibUIDK::RelayoutElement(const UIWNDPROPERTIES *pUIWndProp,
	const CRect &rcChildOriginal,
	const CPoint &ptResize,
	/*out*/ CRect &rcNew,
	int nWindowWidthLimitBkImage, int nWindowHeightLimitBkImage,
	int nWindowWidth, int nWindowHeight,
	CONTROL_LAYOUT_HOR eHorLayoutMode, CONTROL_LAYOUT_VER eVerLayoutMode,
	LPCRECT lprcLayoutMargin,
	BOOL bKeepAspectRatio,
	LONG lMaxHorRatio,
	LONG lMaxVerRatio)
{
	if (pUIWndProp == NULL)
	{
		return -1;
	}

	rcNew = rcChildOriginal;

	CRect rcUIWndOrig = pUIWndProp->m_rcControl;
	if (CUIMgr::IsEnableDPI())
	{
		GetDPIRect(rcUIWndOrig);
	}

	// Horizontal
	if (eHorLayoutMode == CLH_DEFAULT)
	{
		if (rcChildOriginal.right > ptResize.x)
		{
			rcNew.right = nWindowWidthLimitBkImage - (rcUIWndOrig.Width() - rcChildOriginal.right);
			if (rcChildOriginal.left > ptResize.x)
			{
				rcNew.left = rcNew.right - rcChildOriginal.Width();
			}
		}
	}
	else if (eHorLayoutMode == CLH_LEFT)
	{
		rcNew.left += lprcLayoutMargin->left;
		rcNew.right = rcNew.left + rcChildOriginal.Width();
	}
	else if (eHorLayoutMode == CLH_FREE)
	{
		// Keep
	}
	else if (eHorLayoutMode == CLH_RIGHT)
	{
		rcNew.right = nWindowWidthLimitBkImage - (rcUIWndOrig.Width()
				- rcChildOriginal.right) - lprcLayoutMargin->right;

		rcNew.left = rcNew.right - rcChildOriginal.Width();
	}
	else if (eHorLayoutMode == CLH_CENTER)
	{
		rcNew.left = (nWindowWidthLimitBkImage - lprcLayoutMargin->left
				- lprcLayoutMargin->right - rcChildOriginal.Width()) / 2
			+ lprcLayoutMargin->left;

		rcNew.right = rcNew.left + rcChildOriginal.Width();
	}
	else if (eHorLayoutMode == CLH_FILL)
	{
		rcNew.left += lprcLayoutMargin->left;
		rcNew.right = nWindowWidthLimitBkImage - (rcUIWndOrig.Width() - rcChildOriginal.right)
			- lprcLayoutMargin->right;
	}
	else if (eHorLayoutMode == CLH_FULL_FILL)
	{
		rcNew.left = nWindowWidthLimitBkImage + lprcLayoutMargin->left;
		rcNew.right = nWindowWidthLimitBkImage + nWindowWidth - lprcLayoutMargin->right;
	}
	else if (eHorLayoutMode == CLH_RIGHT_EVEN_ZOOM_OUT)
	{
		rcNew.right = nWindowWidth - (rcUIWndOrig.Width() - rcChildOriginal.right);
		rcNew.left = rcNew.right - rcChildOriginal.Width();
	}
	else if (eHorLayoutMode == CLH_KEEP_CHILDREN_ORIGIN_ASPECT_RATIO
		|| eHorLayoutMode == CLH_KEEP_CHILDREN_ASPECT_RATIO)
	{
		if (bKeepAspectRatio)
		{
			CRect rcUIWndOrig = pUIWndProp->m_rcControl;
			rcUIWndOrig.DeflateRect(lprcLayoutMargin);

			int nCalWindowWidth = nWindowWidth - lprcLayoutMargin->left - lprcLayoutMargin->right;
			if (lMaxHorRatio > 100)
			{
				// if m_lMaxHorRatio > 100, and the window width larger than
				// rcUIWndOrig.Width() * m_lMaxHorRatio / 100,
				// use rcUIWndOrig.Width() * m_lMaxHorRatio / 100 to calculate.
				if (nWindowWidth - lprcLayoutMargin->left - lprcLayoutMargin->right >
					rcUIWndOrig.Width() * lMaxHorRatio / 100)
				{
					nCalWindowWidth = rcUIWndOrig.Width() * lMaxHorRatio / 100;
				}
			}
			// If m_lMaxHorRatio < 100, the children keep the origin position.
			else if (lMaxHorRatio < 100)
			{
				nCalWindowWidth = rcUIWndOrig.Width();
			}

			if (rcUIWndOrig.Width() != 0)
			{
				rcNew.left = lprcLayoutMargin->left
					+ (rcChildOriginal.left - lprcLayoutMargin->left)
					* nCalWindowWidth
					/ rcUIWndOrig.Width();

				if (eHorLayoutMode == CLH_KEEP_CHILDREN_ORIGIN_ASPECT_RATIO)
				{
					rcNew.right = rcNew.left + rcChildOriginal.Width();
				}
				else if (eHorLayoutMode == CLH_KEEP_CHILDREN_ASPECT_RATIO)
				{
					rcNew.right = rcNew.left
						+ rcChildOriginal.Width() * nCalWindowWidth / rcUIWndOrig.Width();
				}
			}
			else
			{
				TRACE(_T("[LibUIDK]===The width of the window without margin is 0!\n"));
			}
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	// Vertical
	if (eVerLayoutMode == CLV_DEFAULT)
	{
		if (rcChildOriginal.bottom > ptResize.y)
		{
			rcNew.bottom = nWindowHeightLimitBkImage
				- (rcUIWndOrig.Height() - rcChildOriginal.bottom);

			if (rcChildOriginal.top > ptResize.y)
			{
				rcNew.top = rcNew.bottom - rcChildOriginal.Height();
			}
		}
	}
	else if (eVerLayoutMode == CLV_TOP)
	{
		rcNew.top += lprcLayoutMargin->top;
		rcNew.bottom = rcNew.top + rcChildOriginal.Height();
	}
	else if (eVerLayoutMode == CLV_FREE)
	{
		// Keep
	}
	else if (eVerLayoutMode == CLV_BOTTOM)
	{
		rcNew.bottom = nWindowHeightLimitBkImage
			- (rcUIWndOrig.Height() - rcChildOriginal.bottom) - lprcLayoutMargin->bottom;
		rcNew.top = rcNew.bottom - rcChildOriginal.Height();
	}
	else if (eVerLayoutMode == CLV_CENTER)
	{
		rcNew.top = (nWindowHeightLimitBkImage
				- lprcLayoutMargin->top
				- lprcLayoutMargin->bottom
				- rcChildOriginal.Height())
			/ 2
			+ lprcLayoutMargin->top;

		rcNew.bottom = rcNew.top + rcChildOriginal.Height();
	}
	else if (eVerLayoutMode == CLV_FILL)
	{
		rcNew.top += lprcLayoutMargin->top;
		rcNew.bottom = nWindowHeightLimitBkImage
			- (rcUIWndOrig.Height() - rcChildOriginal.bottom) - lprcLayoutMargin->bottom;
	}
	else if (eVerLayoutMode == CLV_FULL_FILL)
	{
		rcNew.top = nWindowHeightLimitBkImage + lprcLayoutMargin->top;
		rcNew.bottom = nWindowHeightLimitBkImage + nWindowHeight - lprcLayoutMargin->bottom;
	}
	else if (eVerLayoutMode == CLV_BOTTOM_EVEN_ZOOM_OUT)
	{
		rcNew.bottom = nWindowHeight - (rcUIWndOrig.Height() - rcChildOriginal.bottom);
		rcNew.top = rcNew.bottom - rcChildOriginal.Height();
	}
	else if (eVerLayoutMode == CLV_KEEP_CHILDREN_ORIGIN_ASPECT_RATIO
		|| eVerLayoutMode == CLV_KEEP_CHILDREN_ASPECT_RATIO)
	{
		if (bKeepAspectRatio)
		{
			CRect rcUIWndOrig = pUIWndProp->m_rcControl;
			rcUIWndOrig.DeflateRect(lprcLayoutMargin);

			int nCalWindowHeight = nWindowHeight - lprcLayoutMargin->top - lprcLayoutMargin->bottom;
			if (lMaxVerRatio > 100)
			{
				if (nWindowHeight - lprcLayoutMargin->top - lprcLayoutMargin->bottom >
					rcUIWndOrig.Height() * lMaxVerRatio / 100)
				{
					nCalWindowHeight = rcUIWndOrig.Height() * lMaxVerRatio / 100;
				}
			}
			else if (lMaxVerRatio < 100)
			{
				nCalWindowHeight = rcUIWndOrig.Height();
			}

			if (rcUIWndOrig.Height() != 0)
			{
				rcNew.top = lprcLayoutMargin->top
					+ (rcChildOriginal.top - lprcLayoutMargin->top)
					* nCalWindowHeight
					/ rcUIWndOrig.Height();

				if (eVerLayoutMode == CLV_KEEP_CHILDREN_ORIGIN_ASPECT_RATIO)
				{
					rcNew.bottom = rcNew.top + rcChildOriginal.Height();
				}
				else if (eVerLayoutMode == CLV_KEEP_CHILDREN_ASPECT_RATIO)
				{
					rcNew.bottom = rcNew.top
						+ rcChildOriginal.Height() * nCalWindowHeight / rcUIWndOrig.Height();
				}
			}
			else
			{
				TRACE(_T("[LibUIDK]===The height of the window without margin is 0!\n"));
			}
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	return 0;
}

int LibUIDK::RelayoutChildren(CWnd *pParentWnd,
	const UIWNDPROPERTIES *pUIWndProp,
	int cx, int cy,
	int nWindowCurWidth, int nWindowCurHeight,
	const std::vector<CChildInfo> *pvChildWndList,
	BOOL bKeepAspectRatio,
	LONG lMaxHorRatio,
	LONG lMaxVerRatio,
	POINT ptOffset,
	BOOL bChildInTemplate)
{
	if (pUIWndProp == NULL || pvChildWndList == NULL)
	{
		return -1;
	}

	POINT ptWindowResize = pUIWndProp->m_bWindowResizePointSameAsImageResizePoint
		? pUIWndProp->m_ptImageResize
		: pUIWndProp->m_ptWindowResize;

	GetDPIPoint(&ptWindowResize);

	// resize child control
	size_t i = 0;
	size_t nChildCount = pvChildWndList->size();
	HDWP hdwp = BeginDeferWindowPos(int(nChildCount));
	for (i = 0; i < nChildCount; ++i)
	{
		CONTROL_TYPE eControlType = (*pvChildWndList)[i].m_eControlType;

		CONTROL_LAYOUT_HOR eHorLayoutMode = (*pvChildWndList)[i].m_pCtrlProperties->m_eHorLayoutMode;
		CONTROL_LAYOUT_VER eVerLayoutMode = (*pvChildWndList)[i].m_pCtrlProperties->m_eVerLayoutMode;
		CRect rcParentMargin = (*pvChildWndList)[i].m_pCtrlProperties->m_rcLayoutMargin;
		if (CUIMgr::IsEnableDPI())
		{
			GetDPIRect(rcParentMargin);
		}

		if (eHorLayoutMode == CLH_FREE && eVerLayoutMode == CLV_FREE)
		{
			continue;
		}

		if (eControlType == CT_WL_RECTCTRL
			|| eControlType == CT_WL_SPLITTER
			|| eControlType == CT_WL_LINE
			|| eControlType == CT_WL_TEXT
			|| eControlType == CT_WL_BUTTON
			|| eControlType == CT_WL_CHECK
			|| eControlType == CT_WL_RADIO
			|| eControlType == CT_WL_SLIDER
			|| eControlType == CT_WL_PICTURE
			|| eControlType == CT_WL_RICHEDIT
			|| eControlType == CT_WL_RICHEDIT_IM
			|| eControlType == CT_TASK_WND_MGR
			|| eControlType == CT_PNL_DOCK
			|| eControlType == CT_UNIFORM_GRID
			|| eControlType == CT_PNL_STACK)
		{
			CRectCtrl *pRectCtrl = (CRectCtrl *)(*pvChildWndList)[i].m_pChildCtrl;

			CRect rcChildOrig;
			pRectCtrl->GetOrigWindowRect(rcChildOrig);
			CRect rcNew;
			RelayoutElement(pUIWndProp, rcChildOrig, ptWindowResize, rcNew,
				cx, cy, nWindowCurWidth, nWindowCurHeight,
				eHorLayoutMode, eVerLayoutMode, rcParentMargin,
				bKeepAspectRatio, lMaxHorRatio, lMaxVerRatio);

			rcNew.OffsetRect(ptOffset);

			if (bChildInTemplate)
			{
				pRectCtrl->SendMessage(WM_WL_MOVEWINDOW, 0, (LPARAM)(LPCRECT)rcNew);
			}
			else
			{
				pRectCtrl->MoveWindow(rcNew);
			}
		}
		else
		{
			// CRectCtrl's orig rect is get by call CRectCtrl::GetOrigWindowRect.
			CRect rcChildOriginal = (*pvChildWndList)[i].m_rcOriginal;
			CWnd *pChild = (*pvChildWndList)[i].m_pChildCtrl;
			if (pChild->GetSafeHwnd() == NULL)
			{
				continue;
			}

			CRect rcNew(rcChildOriginal);
			if (eControlType == CT_COMBOBOX)
			{
				CRect rcCombo;
				pChild->GetWindowRect(rcCombo);
				rcNew.bottom = rcNew.top + rcCombo.Height();
			}

			CRect rcOld;
			pChild->GetWindowRect(rcOld);
			pParentWnd->ScreenToClient(rcOld);
			RelayoutElement(pUIWndProp, rcChildOriginal, ptWindowResize, rcNew,
				cx, cy, nWindowCurWidth, nWindowCurHeight,
				eHorLayoutMode, eVerLayoutMode, rcParentMargin,
				bKeepAspectRatio, lMaxHorRatio, lMaxVerRatio);

			if (rcOld == rcNew)
			{
				continue;
			}

			if (eControlType == CT_COMBOBOX)
			{
				CRect rcCombo;
				pChild->GetWindowRect(rcCombo);
				rcNew.bottom = rcNew.top + rcCombo.Height();
			}

			BOOL bMoveWindow = (BOOL)pParentWnd->SendMessage(WM_ISCHILDMOVE,
					0, LPARAM(pChild->GetSafeHwnd()));

			if (bMoveWindow)
			{
				int nComboSelectFieldHeight = 0;
				BOOL bVisible = TRUE;
				if (eControlType == CT_COMBOBOX)
				{
					nComboSelectFieldHeight = (int)pChild->SendMessage(CB_GETITEMHEIGHT, -1);
					bVisible = pChild->IsWindowVisible();
					if (bVisible)
					{
						// Hide the combo box to avoid flicker while call MoveWindow
						pChild->ShowWindow(SW_HIDE);
					}
				}

				if (eControlType == CT_COMBOBOX)
				{
					// Don't move combo box together with other controls,
					// since the height of the combo box's selection field
					// will be restored to default value.
					pChild->MoveWindow(rcNew, FALSE);
				}
				else
				{
					if (NULL == DeferWindowPos(hdwp, pChild->m_hWnd, NULL,
							rcNew.left, rcNew.top, rcNew.Width(), rcNew.Height(),
							SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER))
					{

					}
				}

				if (eControlType == CT_COMBOBOX)
				{
					// After call MoveWindow, the height of the combo box's
					// selection field will be restored to default value.
					// Lost most time.
					pChild->SendMessage(CB_SETITEMHEIGHT, -1, nComboSelectFieldHeight);
					if (bVisible)
					{
						pChild->ShowWindow(SW_SHOW);
					}
				}
			}
		}
	}

	EndDeferWindowPos(hdwp);	// Move all controls without combo box.

	return 0;
}

BOOL LibUIDK::CreateChildControls(const UIWNDPROPERTIES *pUIWndProp, CWnd *pParent,
	HIUIIMAGE himgBkN, HIUIIMAGE himgBkH, HIUIIMAGE himgBkS, HIUIIMAGE himgBkD,
	std::vector<CChildInfo> *pvChildWndList)
{
	if (pUIWndProp == NULL || pvChildWndList == NULL)
	{
		return FALSE;
	}

	size_t nCtrlCount = pUIWndProp->m_vControls.size();
	for (size_t i = 0; i < nCtrlCount; ++i)
	{
		CTRLPROPERTIES *pCtrlProp = pUIWndProp->m_vControls[i];
		CONTROL_TYPE eControlType = (CONTROL_TYPE)pCtrlProp->m_eControlType;

		CChildInfo ci;
		ci.m_pCtrlProperties = pCtrlProp;
		// Create Button
		if (eControlType == CT_PUSHBUTTON)
		{
			CSkinButton *pChild = CreateButton((BTNPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkH, himgBkS, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// Create Check box
		else if (eControlType == CT_CHECK)
		{
			CSkinButton *pChild = CreateCheckBox((CHKPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkH, himgBkS, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// Create Radio button
		else if (eControlType == CT_RADIO)
		{
			CSkinButton *pChild = CreateRadioButton((RADPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkH, himgBkS, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// Create Combo box
		else if (eControlType == CT_COMBOBOX)
		{
			CSkinComboBox *pChild = CreateComboBox((CMBPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkH, himgBkS, himgBkD);

			ci.m_pChildCtrl = pChild;
			RECT rcOriginal = RECT(((CMBPROPERTIES *)pCtrlProp)->m_rcControl);
			GetDPIRect(&rcOriginal);
			ci.m_rcOriginal = rcOriginal;// combo box's rect include drop-down list
			ci.m_eControlType = eControlType;
		}
		// Create Edit
		else if (eControlType == CT_EDIT)
		{
			CSkinEdit *pChild = CreateEdit((EDTPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkH, himgBkS, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// Create Slider
		else if (eControlType == CT_SLIDER)
		{
			CSkinSliderCtrl *pChild = CreateSlider((SLDPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkH, himgBkS, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// Create Static
		else if (eControlType == CT_STATIC)
		{
			CSkinStatic *pChild = CreateStatic((TXTPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// Create Progress
		else if (eControlType == CT_PROGRESS)
		{
			CSkinProgressCtrl *pChild = CreateProgress((PRGPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkH, himgBkS, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// Create Spin button
		else if (eControlType == CT_SPIN)
		{
			CSkinSpinButtonCtrl *pChild = CreateSpinButton((SPNPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkH, himgBkS, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// Create Hotkey
		else if (eControlType == CT_HOTKEY)
		{
			CSkinHotKeyCtrl *pChild = CreateHotKey((HOTPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkH, himgBkS, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// Create IPAddress
		else if (eControlType == CT_IPADDRESS)
		{
			CSkinIPAddressCtrl *pChild = CreateIPAddress((IPAPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkH, himgBkS, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// Create SliderEx
		else if (eControlType == CT_SLIDEREX)
		{
			CSliderCtrlEx *pChild = CreateSliderEx((SLDEXPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkH, himgBkS, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// Create ListCtrl
		else if (eControlType == CT_LISTCTRL)
		{
			CSkinListCtrl *pChild = CreateListCtrl((LSTCTRLPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// Create tree ctrl
		else if (eControlType == CT_TREECTRL)
		{
			CSkinTreeCtrl *pChild = CreateTreeCtrl((TREECTRLPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkH, himgBkS, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// create rich edit control
		else if (eControlType == CT_RICHEDIT)
		{
			// Must call AfxInitRichEdit2()
			CSkinRichEditCtrl *pChild = CreateRichEdit((RICHEDITPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkH, himgBkS, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = (CWnd *)pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// create IM rich edit control
		else if (eControlType == CT_RICHEDIT_IM)
		{
			// Must call AfxInitRichEdit2()
			CIMRichEditCtrl *pChild = CreateIMRichEdit((IMREPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkH, himgBkS, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = (CWnd *)pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// create extended scrollbar
		else if (eControlType == CT_SCROLLBAREX)
		{
			CScrollBarEx *pChild = CreateScrollBarEx((SBEXPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkH, himgBkS, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// create thumbnail control
		else if (eControlType == CT_THUMBNAIL)
		{
			CThumbnailCtrl *pChild = CreateThumbnail((THMPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// create tree list control
		else if (eControlType == CT_TREELIST)
		{
			CTreeListCtrl *pChild = CreateTreeList((TREELISTPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// create html control
		else if (eControlType == CT_HTMLCTRL)
		{
			CHtmlCtrl *pChild = CreateHtmlCtrl((HTMLCTRLPROPERTIES *)pCtrlProp, pParent);
			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// create datetime control
		else if (eControlType == CT_DATETIMECTRL)
		{
			CSkinDateTimeCtrl *pChild = CreateDateTimeCtrl((DATETIMECTRLPROPERTIES *)pCtrlProp,
					pParent);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// create wave control
		else if (eControlType == CT_WAVECTRL)
		{
			CWaveCtrl *pChild = CreateWaveCtrl((WAVECTRLPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkH, himgBkS, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// create picture control
		else if (eControlType == CT_PICTURE)
		{
			CPictureCtrl *pChild = CreatePictureCtrl((PICWLPROPERTIES *)pCtrlProp, pParent,
					himgBkN, himgBkH, himgBkS, himgBkD);

			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// create menu bar
		else if (eControlType == CT_MENUBAR)
		{
			CMenuBar *pChild = CreateMenuBar((MENUBARPROPERTIES *)pCtrlProp, pParent);
			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// create Splitter bar
		else if (eControlType == CT_SPLITTER)
		{
			CSplitterBar *pChild = CreateSplitter((SPLITTERPROPERTIES *)pCtrlProp, pParent);
			CRect rect;
			pChild->GetWindowRect(rect);
			pParent->ScreenToClient(rect);

			ci.m_pChildCtrl = (CWnd *)pChild;
			ci.m_rcOriginal = rect;
			ci.m_eControlType = eControlType;
		}
		// create rect control
		else if (eControlType == CT_WL_RECTCTRL)
		{
			CRectCtrl *pChild = CreateWLRect((RECTWLPROPERTIES *)pCtrlProp, pParent);
			ci.m_pChildCtrl = (CWnd *)pChild;
			ci.m_rcOriginal = RECT(pCtrlProp->m_rcControl);
			ci.m_eControlType = eControlType;

			// If the rect control binds window, create the bind window as child
			CString strBindWindowID = (LPCTSTR)((RECTWLPROPERTIES *)pCtrlProp)->m_strBindWindowID;
			if (!strBindWindowID.IsEmpty())
			{
				UIWNDPROPERTIES *pBindWndProp = CUIMgr::GetUIWNDPROPERTIES(strBindWindowID);
				if (pBindWndProp != NULL)
				{
					CUIWnd *pBindWnd = CUIWnd::CreateChildWindow((LPCTSTR)pBindWndProp->m_strID,
							(LONG)pBindWndProp->m_nID, WS_VISIBLE, pChild, pParent, NULL, 0);

					if (pBindWnd != NULL)
					{
						pChild->AddBindWindow(pBindWnd);
					}
				}
			}
		}
		// create Splitter bar
		else if (eControlType == CT_WL_SPLITTER)
		{
			CWLSplitterBar *pChild = CreateWLSplitter((SPLITTERWLPROPERTIES *)pCtrlProp, pParent);
			ci.m_pChildCtrl = (CWnd *)pChild;
			ci.m_rcOriginal = RECT(pCtrlProp->m_rcControl);
			ci.m_eControlType = eControlType;
		}
		// create line
		else if (eControlType == CT_WL_LINE)
		{
			CWLLine *pChild = CreateWLLine((LINEWLPROPERTIES *)pCtrlProp, pParent);
			ci.m_pChildCtrl = (CWnd *)pChild;
			ci.m_rcOriginal = RECT(pCtrlProp->m_rcControl);
			ci.m_eControlType = eControlType;
		}
		// create text
		else if (eControlType == CT_WL_TEXT)
		{
			CWLText *pChild = CreateWLText((TXTPROPERTIES *)pCtrlProp, pParent);
			ci.m_pChildCtrl = (CWnd *)pChild;
			ci.m_rcOriginal = RECT(pCtrlProp->m_rcControl);
			ci.m_eControlType = eControlType;
		}
		// create windowless button
		else if (eControlType == CT_WL_BUTTON)
		{
			CWLButton *pChild = CreateWLButton((BTNPROPERTIES *)pCtrlProp, pParent);
			ci.m_pChildCtrl = (CWnd *)pChild;
			ci.m_rcOriginal = RECT(pCtrlProp->m_rcControl);
			ci.m_eControlType = eControlType;
		}
		else if (eControlType == CT_WL_CHECK)
		{
			CWLButton *pChild = CreateWLCheck((CHKPROPERTIES *)pCtrlProp, pParent);
			ci.m_pChildCtrl = (CWnd *)pChild;
			ci.m_rcOriginal = RECT(pCtrlProp->m_rcControl);
			ci.m_eControlType = eControlType;
		}
		else if (eControlType == CT_WL_RADIO)
		{
			CWLButton *pChild = CreateWLRadio((RADPROPERTIES *)pCtrlProp, pParent);
			ci.m_pChildCtrl = (CWnd *)pChild;
			ci.m_rcOriginal = RECT(pCtrlProp->m_rcControl);
			ci.m_eControlType = eControlType;
		}
		// create windowless slider
		else if (eControlType == CT_WL_SLIDER)
		{
			CWLSliderCtrl *pChild = CreateWLSlider((SLDEXPROPERTIES *)pCtrlProp, pParent);
			ci.m_pChildCtrl = (CWnd *)pChild;
			ci.m_rcOriginal = RECT(pCtrlProp->m_rcControl);
			ci.m_eControlType = eControlType;
		}
		// create windowless picture
		else if (eControlType == CT_WL_PICTURE)
		{
			CWLPicture *pChild = CreateWLPic((PICWLPROPERTIES *)pCtrlProp, pParent);
			ci.m_pChildCtrl = (CWnd *)pChild;
			ci.m_rcOriginal = RECT(pCtrlProp->m_rcControl);
			ci.m_eControlType = eControlType;
		}
		// create windowless richedit
		else if (eControlType == CT_WL_RICHEDIT)
		{
			CWLRichEditCtrl *pChild = CreateWLRichEdit((RICHEDITPROPERTIES *)pCtrlProp, pParent);
			ci.m_pChildCtrl = (CWnd *)pChild;
			ci.m_rcOriginal = RECT(pCtrlProp->m_rcControl);
			ci.m_eControlType = eControlType;
		}
		// create windowless IM
		else if (eControlType == CT_WL_RICHEDIT_IM)
		{
			CWLIMRichEditCtrl *pChild = CreateWLIMRichEdit((IMREPROPERTIES *)pCtrlProp, pParent);
			ci.m_pChildCtrl = (CWnd *)pChild;
			ci.m_rcOriginal = RECT(pCtrlProp->m_rcControl);
			ci.m_eControlType = eControlType;
		}
		// CTaskWndMgr
		else if (eControlType == CT_TASK_WND_MGR)
		{
			CTaskWndMgr *pChild = CreateTaskWndMgr((TWMGRPROPERTIES *)pCtrlProp, pParent);
			ci.m_pChildCtrl = (CWnd *)pChild;
			ci.m_rcOriginal = RECT(pCtrlProp->m_rcControl);
			ci.m_eControlType = eControlType;
		}
		// Panel
		else if (eControlType == CT_PNL_DOCK)
		{
			CDockPanel *pChild = CreatePanelDock((DOCKPNLPROPERTIES *)pCtrlProp, pParent);
			ci.m_pChildCtrl = (CWnd *)pChild;
			ci.m_rcOriginal = RECT(pCtrlProp->m_rcControl);
			ci.m_eControlType = eControlType;
		}
		else if (eControlType == CT_UNIFORM_GRID)
		{
			CUniformGridPanel *pChild = CreatePanelUniformGrid((UGRIDPNLPROPERTIES *)pCtrlProp,
					pParent);

			ci.m_pChildCtrl = (CWnd *)pChild;
			ci.m_rcOriginal = RECT(pCtrlProp->m_rcControl);
			ci.m_eControlType = eControlType;
		}
		else if (eControlType == CT_PNL_STACK)
		{
			CStackPanel *pChild = CreatePanelStack((STACKPNLPROPERTIES *)pCtrlProp, pParent);
			ci.m_pChildCtrl = (CWnd *)pChild;
			ci.m_rcOriginal = RECT(pCtrlProp->m_rcControl);
			ci.m_eControlType = eControlType;
		}
		else
		{
			ASSERT(FALSE);
		}

		pvChildWndList->push_back(ci);
	}

	return TRUE;
}

// 当工作窗口位置改变后，让阴影窗口的位置同步改变
// 本函数在工作窗口的WM_WINDOWPOSCHANGED消息中调用。
// lprcMargin: 工作窗口相对于阴影窗口的边距
// lpsizeRoundRect: 如果工作窗口有圆角，设置圆角大小
int LibUIDK::SyncHostWindow(CWnd *pWorkWnd, UINT uWorkWndFlags, LPCRECT lprcMargin, LPSIZE lpsizeRoundRect)
{
	if (pWorkWnd == NULL || lprcMargin == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	CWnd *pParent = pWorkWnd->GetParent();
	if (pParent == NULL)
	{
		return 1;
	}

	// 阴影窗口同步隐藏显示
	if (IsIncludeFlag(uWorkWndFlags, SWP_HIDEWINDOW))
	{
		// 因为工作窗口都是由阴影窗口弹出的模式对话框
		// 当工作窗口的CUIWnd::DoModal函数中的RunModalLoop返回时，
		// 会执行：
		// SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW |
		//	SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
		// 把模式对话框自己先隐藏掉。 就会执行到这里，如果这时，隐藏阴影窗口。
		// 会导致接下来弹出的窗口，未激活到最前端。 甚至调用BringWindowToTop、SetActiveWindow
		// 都无法激活。
		pParent->SetForegroundWindow();
		pParent->ShowWindow(SW_HIDE);
	}
	else if (IsIncludeFlag(uWorkWndFlags, SWP_SHOWWINDOW))
	{
		pParent->ShowWindow(SW_SHOW);
	}

	// 虽然通过把工作窗口当成阴影窗口的模式对话框，可以解决第三方窗口嵌入问题。
	// 但当工作窗口为topmost时，仍然会导致第三方窗口嵌入，解决方案就是让阴影窗口也变成topmost。
	// 如果为工作窗口设置了Topmost，那么同时也为阴影窗口设置Topmost。
	// 当阴影窗口为Topmost时，即使没有为工作窗口设置Topmost，系统也会自动为工作窗口加上的。
	// 所以在为阴影窗口设置Topmost时，没必要清除工作窗口的WS_EX_TOPMOST属性。
	LONG lExStyle = GetWindowLong(pWorkWnd->GetSafeHwnd(), GWL_EXSTYLE);
	if ((lExStyle & WS_EX_TOPMOST) == WS_EX_TOPMOST)
	{
		LONG lParentExStyle = GetWindowLong(pWorkWnd->GetParent()->GetSafeHwnd(), GWL_EXSTYLE);
		if ((lParentExStyle & WS_EX_TOPMOST) == 0)
		{
			pWorkWnd->GetParent()->SetWindowPos(&CWnd::wndTopMost,
				0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		}
	}

	// 阴影窗口同步跟随代码
	CRect rcWnd;
	pWorkWnd->GetWindowRect(rcWnd);
	rcWnd.InflateRect(lprcMargin);

	pParent->MoveWindow(rcWnd);

	// 如果有圆角，设置rgn
	if (lpsizeRoundRect != NULL && lpsizeRoundRect->cx != 0 && lpsizeRoundRect->cy != 0)
	{
		CRect rcClient;
		pWorkWnd->GetClientRect(rcClient);
		CRgn rgn;
		rgn.CreateRoundRectRgn(0, 0, rcClient.right + 1, rcClient.bottom + 1, 4, 4);
		pWorkWnd->SetWindowRgn((HRGN)rgn.GetSafeHandle(), TRUE);
		rgn.DeleteObject();
	}

	return 0;
}

int LibUIDK::SelectComboBoxString(CComboBox *pBox, DWORD dwItemData)
{
	int nCount = pBox->GetCount();
	for (int i = 0; i < nCount; ++i)
	{
#if(_MSC_VER <= 1200) // VC6.0
		DWORD dwData = pBox->GetItemData(i);
#else
		DWORD_PTR dwData = pBox->GetItemData(i);
#endif
		if (dwData == dwItemData)
		{
			pBox->SetCurSel(i);
			break;
		}
	}

	return 0;
}

int LibUIDK::SelectComboBoxString(CComboBox *pBox, const CString &str) // all same
{
	int nCount = pBox->GetCount();
	for (int i = 0; i < nCount; ++i)
	{
		CString strItem;
		pBox->GetLBText(i, strItem);
		if (strItem == str)
		{
			pBox->SetCurSel(i);
			break;
		}
	}

	return 0;
}

int LibUIDK::SelectComboBoxStringNoCase(CComboBox *pBox, const CString &str)
{
	int nCount = pBox->GetCount();
	for (int i = 0; i < nCount; ++i)
	{
		CString strItem;
		pBox->GetLBText(i, strItem);
		if (strItem.CompareNoCase(str) == 0)
		{
			pBox->SetCurSel(i);
			break;
		}
	}

	return 0;
}

UINT LibUIDK::GetDrawTextFormat(TEXT_ALIGN_HOR eHorAlign, CONTROL_TYPE eControlType)
{
	UINT uTextFormat = 0;

	switch (eHorAlign)
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
		else if (eControlType == CT_STATIC || eControlType == CT_WL_TEXT)
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

	return 0;
}

UINT LibUIDK::GetDrawTextFormat(TEXT_ALIGN_VER eVerAlign, CONTROL_TYPE eControlType)
{
	UINT uTextFormat = 0;

	switch (eVerAlign)
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
		else if (eControlType == CT_STATIC || eControlType == CT_WL_TEXT)
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
		uTextFormat |= DT_BOTTOM;
		break;
	case TAV_CENTER:
		uTextFormat |= DT_VCENTER;
		break;
	}

	return uTextFormat;
}

HTREEITEM LibUIDK::FindTreeItemByData(CTreeCtrl *pTreeCtrl, HTREEITEM hParentItem, DWORD dwItemData)
{
	if (pTreeCtrl == NULL)
	{
		return NULL;
	}

	HTREEITEM hRet = NULL;

	if (hParentItem == NULL)
	{
		hParentItem = pTreeCtrl->GetRootItem();
	}

	while (hParentItem != NULL)
	{
#if(_MSC_VER <= 1200) // VC6.0
		DWORD dwData = pTreeCtrl->GetItemData(hParentItem);
#else
		DWORD_PTR dwData = pTreeCtrl->GetItemData(hParentItem);
#endif
		if (dwData == dwItemData)
		{
			hRet = hParentItem;
			break;
		}

		// Check its children.
		HTREEITEM hChild = pTreeCtrl->GetChildItem(hParentItem);
		if (hChild != NULL)
		{
			HTREEITEM hChildRet = FindTreeItemByData(pTreeCtrl, hChild, dwItemData);
			if (hChildRet != NULL)
			{
				hRet = hChildRet;
				break;
			}
		}

		// Check it sibling.
		hParentItem = pTreeCtrl->GetNextSiblingItem(hParentItem);
	}

	return hRet;
}

WNDPROC g_wpOldMenuProc = NULL;

void RemoveMenuBorder(HWND hWnd, MENU_BORDER_STYLE eMbs)
{
	if (eMbs == MBS_NORMAL)
	{
		return;
	}

	// If menu style isn't MBS_NORMAL, remove the 3D border of menu
	DWORD dwStyleEx = GetWindowLong(hWnd, GWL_EXSTYLE);
	dwStyleEx &= ~WS_EX_WINDOWEDGE;
	dwStyleEx &= ~WS_EX_DLGMODALFRAME;
	IUISetWindowLong(hWnd, GWL_EXSTYLE, dwStyleEx);

	if (eMbs == MBS_NONE)
	{
		DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
		dwStyle &= ~WS_BORDER;
		IUISetWindowLong(hWnd, GWL_STYLE, dwStyle);
	}
}

LRESULT CALLBACK MenuWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	_AFX_THREAD_STATE *pThreadState = AfxGetThreadState();
	// the 4th 'CWnd *' parameter of CMenu::TrackPopupMenu
	HWND hMenuOwner = pThreadState->m_hTrackingWindow;
	HMENU hMenu = pThreadState->m_hTrackingMenu;

	// If the menu not pop-up by ourself, Do nothing.
	if (hMenuOwner == NULL)
	{
		return CallWindowProc(g_wpOldMenuProc, hWnd, uMsg, wParam, lParam);
	}

	if (hMenu == NULL)
	{
		return CallWindowProc(g_wpOldMenuProc, hWnd, uMsg, wParam, lParam);
	}

	// If the menu not CSkinMenu, Do nothing.
	CMenu *pMenu = (CMenu *)CMenu::FromHandle(hMenu);
	BOOL bSkinMenu = pMenu->IsKindOf(RUNTIME_CLASS(CSkinMenu));
	if (!bSkinMenu)
	{
		return CallWindowProc(g_wpOldMenuProc, hWnd, uMsg, wParam, lParam);
	}

	CSkinMenu *pSkinMenu = (CSkinMenu *)pMenu;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		_IUI_THREAD_STATE *pIUIThreadState = IUIGetThreadState();
		pIUIThreadState->m_hWndMenu = hWnd;

		// If use Aero theme in Win7, the menu border auto become flat style,
		// need not to remove menu border.
		BOOL bDwm = IsAeroTheme();
		if (!bDwm)
		{
			MENU_BORDER_STYLE eMbs = pSkinMenu->GetMenuBorderStyle();
			RemoveMenuBorder(hWnd, eMbs);
		}
	}
	break;

	case WM_WINDOWPOSCHANGING:
		if (pSkinMenu->GetMenuBorderStyle() == MBS_FLAT)
		{
			// Adjust the size of all menu.
			LPWINDOWPOS lpPos = (LPWINDOWPOS)lParam;

			BOOL bDwm = IsAeroTheme();
			if (!bDwm)
			{
				lpPos->cx -= 4;
				lpPos->cy -= 4;
			}

			DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
			if ((dwStyle & WS_BORDER) != WS_BORDER)
			{
				int nXBorder = GetSystemMetrics(SM_CXBORDER);
				int nYBorder = GetSystemMetrics(SM_CYBORDER);

				lpPos->cx -= 2 * nXBorder;
				lpPos->cy -= 2 * nYBorder;
			}
		}
		break;

	case 0x01E5: // same as WM_MOUSEMOVE
	{
		BOOL bMenuBar = (BOOL)::SendMessage(hMenuOwner, WM_ISMENUBAR, 0, 0);
		if (!bMenuBar)
		{
			return CallWindowProc(g_wpOldMenuProc, hWnd, uMsg, wParam, lParam);
		}

		CMenuBar *pMenuBar = (CMenuBar *)CWnd::FromHandle(hMenuOwner);

		CPoint pt;
		GetCursorPos(&pt);

		LRESULT lr = pMenuBar->SendMessage(WM_MENUMOUSEMOVE, (WPARAM)hWnd,
				MAKELPARAM(pt.x, pt.y));
		if (lr == 1)
		{
			return 0;
		}
	}
	break;

	// Draw non-client as client.
	case WM_PRINT:
	{
		HDC hDC = (HDC)wParam;
		LPARAM dwOptions = lParam;
		COLORREF crFlatMenuBorder = pSkinMenu->GetFlatMenuBorderColor();

		DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
		// Draw border of menu with flat style
		if ((dwStyle & WS_BORDER) == WS_BORDER
			&& (dwOptions & PRF_NONCLIENT) == PRF_NONCLIENT
			&& pSkinMenu->GetMenuBorderStyle() == MBS_FLAT)
		{
			CDC *pDC = CDC::FromHandle(hDC);

			LRESULT lr = CallWindowProc(g_wpOldMenuProc, hWnd, uMsg, wParam, lParam);

			CRect rcWnd;
			GetWindowRect(hWnd, rcWnd);

			BOOL bDwm = IsAeroTheme();

			// 5 / 2 + 1 = 3, The border is 3 pixels.
			CPen pen;
			pen.CreatePen(PS_SOLID, bDwm ? 5 : 1, crFlatMenuBorder);

			CPen *pOldPen = pDC->SelectObject(&pen);

			pDC->MoveTo(0, 0);
			pDC->LineTo(rcWnd.Width() - 1, 0);
			pDC->LineTo(rcWnd.Width() - 1, rcWnd.Height() - 1);
			pDC->LineTo(0, rcWnd.Height() - 1);
			pDC->LineTo(0, 0);

			pDC->SelectObject(pOldPen);

			return lr;
		}
	}
	break;

	default:
		break;
	}

	return CallWindowProc(g_wpOldMenuProc, hWnd, uMsg, wParam, lParam);
}

BOOL LibUIDK::AfxInitSkinMenu(__out ATOM *pAtomRet/* = NULL*/)
{
	// Superclass the menu.
	WNDCLASS wc;
	GetClassInfo(NULL, _T("#32768"), &wc);
	wc.hInstance = AfxGetInstanceHandle();
	g_wpOldMenuProc = wc.lpfnWndProc;
	wc.lpfnWndProc = (WNDPROC)MenuWndProc;
	SetLastError(0);
	ATOM atom = RegisterClass(&wc);
	if (atom == 0)
	{
		return FALSE;
	}

	if (pAtomRet != NULL)
	{
		*pAtomRet = atom;
	}

	return TRUE;
}

BOOL LibUIDK::AfxUninitSkinMenu(ATOM atomMenuClass)
{
	WPARAM dwClass = MAKEWPARAM(atomMenuClass, 0);
	return UnregisterClass((LPCTSTR)dwClass, AfxGetInstanceHandle());
}

_IUI_THREAD_STATE LibUIDK_g_iui_thread_state;
_IUI_THREAD_STATE *LibUIDK::IUIGetThreadState()
{
	return &LibUIDK_g_iui_thread_state;
}


//////////////////////////////////////////////////////////////////////////
//

AFX_STATIC_DATA const TCHAR _afxShellOpenFmt[] = _T("%s\\shell\\open\\%s");
AFX_STATIC_DATA const TCHAR _afxShellPrintFmt[] = _T("%s\\shell\\print\\%s");
AFX_STATIC_DATA const TCHAR _afxShellPrintToFmt[] = _T("%s\\shell\\printto\\%s");
AFX_STATIC_DATA const TCHAR _afxDefaultIconFmt[] = _T("%s\\DefaultIcon");
AFX_STATIC_DATA const TCHAR _afxShellNewFmt[] = _T("%s\\ShellNew");

#define DEFAULT_ICON_INDEX 0

AFX_STATIC_DATA const TCHAR _afxIconIndexFmt[] = _T(",%d");
AFX_STATIC_DATA const TCHAR _afxCommand[] = _T("command");
AFX_STATIC_DATA const TCHAR _afxOpenArg[] = _T(" \"%1\"");
AFX_STATIC_DATA const TCHAR _afxPrintArg[] = _T(" /p \"%1\"");
AFX_STATIC_DATA const TCHAR _afxPrintToArg[] = _T(" /pt \"%1\" \"%2\" \"%3\" \"%4\"");
AFX_STATIC_DATA const TCHAR _afxDDEArg[] = _T(" /dde");

AFX_STATIC_DATA const TCHAR _afxDDEExec[] = _T("ddeexec");
AFX_STATIC_DATA const TCHAR _afxDDEOpen[] = _T("[open(\"%1\")]");
AFX_STATIC_DATA const TCHAR _afxDDEPrint[] = _T("[print(\"%1\")]");
AFX_STATIC_DATA const TCHAR _afxDDEPrintTo[] = _T("[printto(\"%1\",\"%2\",\"%3\",\"%4\")]");

AFX_STATIC_DATA const TCHAR _afxShellNewValueName[] = _T("NullFile");
AFX_STATIC_DATA const TCHAR _afxShellNewValue[] = _T("");

BOOL AFXAPI _IUIAfxDeleteRegKey(LPCTSTR lpszKey)
{
	// copy the string
	LPTSTR lpszKeyCopy = _tcsdup(lpszKey);

	if (lpszKeyCopy == NULL)
	{
		return FALSE;
	}

	LPTSTR lpszLast = lpszKeyCopy + lstrlen(lpszKeyCopy);

	// work until the end of the string
	while (lpszLast != NULL)
	{
		*lpszLast = '\0';
		lpszLast = _tcsdec(lpszKeyCopy, lpszLast);

		// try to open that key
		HKEY hKey;
#if(_MSC_VER <= 1400) // VC2005
		if (::RegOpenKey(HKEY_CLASSES_ROOT, lpszKeyCopy, &hKey) != ERROR_SUCCESS)
#else
		if (AfxRegOpenKey(HKEY_CLASSES_ROOT, lpszKeyCopy, &hKey) != ERROR_SUCCESS)
#endif
			break;

		// enumerate the keys underneath
		TCHAR szScrap[_MAX_PATH + 1];
		DWORD dwLen = _countof(szScrap);
		BOOL bItExists = FALSE;

		if (::RegEnumKey(hKey, 0, szScrap, dwLen) == ERROR_SUCCESS)
		{
			bItExists = TRUE;
		}
		::RegCloseKey(hKey);

		// found one?  quit looping
		if (bItExists)
		{
			break;
		}

		// otherwise, delete and find the previous backwhack
#if(_MSC_VER <= 1400) // VC2005
		::RegDeleteKey(HKEY_CLASSES_ROOT, lpszKeyCopy);
#else
		AfxRegDeleteKey(HKEY_CLASSES_ROOT, lpszKeyCopy);
#endif
		lpszLast = _tcsrchr(lpszKeyCopy, '\\');
	}

	// release the string and return
	free(lpszKeyCopy);
	return TRUE;
}

// bCompat: print or print to.
void LibUIDK::IUIRegisterShellFileTypes(CString strDocStrings, BOOL bCompat)
{
	CString strPathName, strTemp;

	AfxGetModuleShortFileName(AfxGetInstanceHandle(), strPathName);

	CString strOpenCommandLine = strPathName;
	CString strPrintCommandLine = strPathName;
	CString strPrintToCommandLine = strPathName;
	CString strDefaultIconCommandLine = strPathName;

	if (bCompat)
	{
		CString strIconIndex;
		HICON hIcon = ::ExtractIcon(AfxGetInstanceHandle(), strPathName, 1);
		if (hIcon != NULL)
		{
			strIconIndex.Format(_afxIconIndexFmt, 1);
			DestroyIcon(hIcon);
		}
		else
		{
			strIconIndex.Format(_afxIconIndexFmt, DEFAULT_ICON_INDEX);
		}
		strDefaultIconCommandLine += strIconIndex;
	}

	CString strFilterExt, strFileTypeId, strFileTypeName;
	BOOL bRet = AfxExtractSubString(strFileTypeId, strDocStrings, (int)CDocTemplate::regFileTypeId);
	if (bRet && !strFileTypeId.IsEmpty())
	{
		// enough info to register it
		BOOL bRet2 = AfxExtractSubString(strFileTypeName,
				strDocStrings, (int)CDocTemplate::regFileTypeName);

		if (!bRet2)
		{
			strFileTypeName = strFileTypeId;    // use id name
		}

		ASSERT(strFileTypeId.Find(' ') == -1);  // no spaces allowed

		// first register the type ID of our server
		if (!IUISetRegKey(strFileTypeId, strFileTypeName, NULL))
		{
			return;    // just skip it
		}

		if (bCompat)
		{
			// path\DefaultIcon = path,1
			strTemp.Format(_afxDefaultIconFmt, (LPCTSTR)strFileTypeId);
			if (!IUISetRegKey(strTemp, strDefaultIconCommandLine, NULL))
			{
				return;    // just skip it
			}
		}

		// If MDI Application
		bRet2 = AfxExtractSubString(strTemp, strDocStrings, (int)CDocTemplate::windowTitle);
		if (!bRet2 || strTemp.IsEmpty())
		{
			// path\shell\open\ddeexec = [open("%1")]
			strTemp.Format(_afxShellOpenFmt, (LPCTSTR)strFileTypeId, (LPCTSTR)_afxDDEExec);
			if (!IUISetRegKey(strTemp, _afxDDEOpen, NULL))
			{
				return;    // just skip it
			}

			if (bCompat)
			{
				// path\shell\print\ddeexec = [print("%1")]
				strTemp.Format(_afxShellPrintFmt, (LPCTSTR)strFileTypeId, (LPCTSTR)_afxDDEExec);
				if (!IUISetRegKey(strTemp, _afxDDEPrint, NULL))
				{
					return;    // just skip it
				}

				// path\shell\printto\ddeexec = [printto("%1","%2","%3","%4")]
				strTemp.Format(_afxShellPrintToFmt, (LPCTSTR)strFileTypeId,
					(LPCTSTR)_afxDDEExec);
				if (!IUISetRegKey(strTemp, _afxDDEPrintTo, NULL))
				{
					return;    // just skip it
				}

				// path\shell\open\command = path /dde
				// path\shell\print\command = path /dde
				// path\shell\printto\command = path /dde
				strOpenCommandLine += _afxDDEArg;
				strPrintCommandLine += _afxDDEArg;
				strPrintToCommandLine += _afxDDEArg;
			}
			else
			{
				strOpenCommandLine += _afxOpenArg;
			}
		}
		else
		{
			// path\shell\open\command = path filename
			// path\shell\print\command = path /p filename
			// path\shell\printto\command = path /pt filename printer driver port
			strOpenCommandLine += _afxOpenArg;
			if (bCompat)
			{
				strPrintCommandLine += _afxPrintArg;
				strPrintToCommandLine += _afxPrintToArg;
			}
		}

		// path\shell\open\command = path filename
		strTemp.Format(_afxShellOpenFmt, (LPCTSTR)strFileTypeId, (LPCTSTR)_afxCommand);
		if (!IUISetRegKey(strTemp, strOpenCommandLine, NULL))
		{
			return;    // just skip it
		}

		if (bCompat)
		{
			// path\shell\print\command = path /p filename
			strTemp.Format(_afxShellPrintFmt, (LPCTSTR)strFileTypeId,
				(LPCTSTR)_afxCommand);
			if (!IUISetRegKey(strTemp, strPrintCommandLine, NULL))
			{
				return;    // just skip it
			}

			// path\shell\printto\command = path /pt filename printer driver port
			strTemp.Format(_afxShellPrintToFmt, (LPCTSTR)strFileTypeId,
				(LPCTSTR)_afxCommand);
			if (!IUISetRegKey(strTemp, strPrintToCommandLine, NULL))
			{
				return;    // just skip it
			}
		}

		AfxExtractSubString(strFilterExt, strDocStrings, (int)CDocTemplate::filterExt);
		if (!strFilterExt.IsEmpty())
		{
			ASSERT(strFilterExt[0] == '.');

			LONG lSize = _MAX_PATH * 2;
#if(_MSC_VER <= 1400) // VC2005
			LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
					strTemp.GetBuffer(lSize), &lSize);
#else
			LONG lResult = AfxRegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
					strTemp.GetBuffer(lSize), &lSize);
#endif
			strTemp.ReleaseBuffer();

			if (lResult != ERROR_SUCCESS || strTemp.IsEmpty() ||
				strTemp == strFileTypeId)
			{
				// no association for that suffix
				if (!IUISetRegKey(strFilterExt, strFileTypeId, NULL))
				{
					return;
				}

				if (bCompat)
				{
					strTemp.Format(_afxShellNewFmt, (LPCTSTR)strFilterExt);
					(void)IUISetRegKey(strTemp, _afxShellNewValue, _afxShellNewValueName);
				}
			}
		}
	}
}

void LibUIDK::IUIUnregisterShellFileTypes(CString &strDocStrings)
{
	CString strPathName, strTemp;

	AfxGetModuleShortFileName(AfxGetInstanceHandle(), strPathName);

	CString strFilterExt, strFileTypeId, strFileTypeName;

	BOOL bRet = AfxExtractSubString(strFileTypeId, strDocStrings, (int)CDocTemplate::regFileTypeId);
	if (bRet && !strFileTypeId.IsEmpty())
	{
		// enough info to register it
		BOOL bRet2 = AfxExtractSubString(strFileTypeName,
				strDocStrings, (int)CDocTemplate::regFileTypeName);

		if (!bRet2)
		{
			strFileTypeName = strFileTypeId;    // use id name
		}

		ASSERT(strFileTypeId.Find(' ') == -1);  // no spaces allowed

		strTemp.Format(_afxDefaultIconFmt, (LPCTSTR)strFileTypeId);
		_IUIAfxDeleteRegKey(strTemp);

		// If MDI Application
		bRet2 = AfxExtractSubString(strTemp, strDocStrings, (int)CDocTemplate::windowTitle);
		if (!bRet2 || strTemp.IsEmpty())
		{
			// path\shell\open\ddeexec = [open("%1")]
			strTemp.Format(_afxShellOpenFmt, (LPCTSTR)strFileTypeId,
				(LPCTSTR)_afxDDEExec);
			_IUIAfxDeleteRegKey(strTemp);

			// path\shell\print\ddeexec = [print("%1")]
			strTemp.Format(_afxShellPrintFmt, (LPCTSTR)strFileTypeId,
				(LPCTSTR)_afxDDEExec);
			_IUIAfxDeleteRegKey(strTemp);

			// path\shell\printto\ddeexec = [printto("%1","%2","%3","%4")]
			strTemp.Format(_afxShellPrintToFmt, (LPCTSTR)strFileTypeId,
				(LPCTSTR)_afxDDEExec);
			_IUIAfxDeleteRegKey(strTemp);
		}

		// path\shell\open\command = path filename
		strTemp.Format(_afxShellOpenFmt, (LPCTSTR)strFileTypeId,
			(LPCTSTR)_afxCommand);
		_IUIAfxDeleteRegKey(strTemp);

		// path\shell\print\command = path /p filename
		strTemp.Format(_afxShellPrintFmt, (LPCTSTR)strFileTypeId,
			(LPCTSTR)_afxCommand);
		_IUIAfxDeleteRegKey(strTemp);

		// path\shell\printto\command = path /pt filename printer driver port
		strTemp.Format(_afxShellPrintToFmt, (LPCTSTR)strFileTypeId,
			(LPCTSTR)_afxCommand);
		_IUIAfxDeleteRegKey(strTemp);

		bRet2 = AfxExtractSubString(strFilterExt, strDocStrings, (int)CDocTemplate::filterExt);
		if (!strFilterExt.IsEmpty())
		{
			ASSERT(strFilterExt[0] == '.');

			LONG lSize = _MAX_PATH * 2;
			LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
					strTemp.GetBuffer(lSize), &lSize);
			strTemp.ReleaseBuffer();

			if (lResult != ERROR_SUCCESS || strTemp.IsEmpty() ||
				strTemp == strFileTypeId)
			{
				strTemp.Format(_afxShellNewFmt, (LPCTSTR)strFilterExt);
				_IUIAfxDeleteRegKey(strTemp);

				// no association for that suffix
				_IUIAfxDeleteRegKey(strFilterExt);
			}
		}
	}
}

int LibUIDK::ApplyStoryboardItem(CObject *pApplyCtrl, BOOL bHWND, const CTRLPROPERTIES *pCtrlProp)
{
	if (pApplyCtrl == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	int nID = CUIMgr::StringIDToInt(pCtrlProp->m_strID);
	CONTROL_TYPE eControlType = pCtrlProp->m_eControlType;

	if (bHWND)	// control has HWND handle
	{
		CWnd *pCtrl = (CWnd *)pApplyCtrl;

		// Move control
		if (pCtrlProp->m_rcControl.wReserved1 == 1)
		{
			CRect rcNew = pCtrlProp->m_rcControl;
			pCtrl->MoveWindow(rcNew);
		}

		// Show or hide control
		if (pCtrlProp->m_bVisible.wReserved1 == 1)
		{
			pCtrl->ShowWindow(pCtrlProp->m_bVisible ? SW_SHOW : SW_HIDE);
		}

		// Background
		if (pCtrlProp->m_strBKN.wReserved1 == 1)
		{
			CString strBkN = (LPCTSTR)pCtrlProp->m_strBKN;
			if (!strBkN.IsEmpty())
			{
				CIUIImage imgBk;
				imgBk.AutoLoad(strBkN);
				imgBk.ConvertToDPIImage(GetDPIX(), GetDPIY());
				pCtrl->SendMessage(WM_SETBKIMAGEN, (WPARAM)imgBk.Detach(), TRUE);
			}
		}

		// Background Transparent
		if (pCtrlProp->m_lBkImgTransparent.wReserved1 == 1)
		{
			pCtrl->SendMessage(WM_SETBKIMGTRANSPARENT, (LONG)pCtrlProp->m_lBkImgTransparent, TRUE);
		}

		// Region
		if (pCtrlProp->m_rcRegion.wReserved1 == 1)
		{
			CRect rcControl;
			pCtrl->GetWindowRect(rcControl);
			rcControl.OffsetRect(-rcControl.TopLeft());
			RECT rcRegion = RECT(pCtrlProp->m_rcRegion);
			rcControl.DeflateRect(&rcRegion);

			CRgn rgn;
			rgn.CreateRectRgnIndirect(rcControl);
			pCtrl->SetWindowRgn((HRGN)rgn.GetSafeHandle(), TRUE);
			rgn.DeleteObject();
		}

		if (eControlType == CT_PUSHBUTTON || eControlType == CT_CHECK || eControlType == CT_RADIO)
		{
			BTNBASEPROPERTIES *pBtnBaseProp = (BTNBASEPROPERTIES *)pCtrlProp;

			// Caption
			if (pBtnBaseProp->m_strCaption.wReserved1 == 1)
			{
				pCtrl->SetWindowText(pBtnBaseProp->m_strCaption);
			}
		}

		// Foreground image
		if (eControlType == CT_PICTURE)
		{
			CPictureCtrl *pPicCtrl = (CPictureCtrl *)pCtrl;
			PICWLPROPERTIES *pPicCtrlProp = (PICWLPROPERTIES *)pCtrlProp;

			if (pPicCtrlProp->m_strFg.wReserved1 == 1)
			{
				CString strFg = (LPCTSTR)pPicCtrlProp->m_strFg;
				if (!strFg.IsEmpty())
				{
					pPicCtrl->SetFgImage(strFg);
				}
			}

			if (pPicCtrlProp->m_rcForegroundMargin.wReserved1 == 1)
			{
				CRect rcFgImgMargin = pPicCtrlProp->m_rcForegroundMargin;
				pPicCtrl->SetFgMargin(rcFgImgMargin);
			}

			if (pPicCtrlProp->m_btFgImgTransparent.wReserved1 == 1)
			{
				pPicCtrl->SetFgImageTransparent(pPicCtrlProp->m_btFgImgTransparent);
			}

			if (pPicCtrlProp->m_eForegroundHorAlignMode.wReserved1 == 1)
			{
				pPicCtrl->SetFgImageAlignModeHor(pPicCtrlProp->m_eForegroundHorAlignMode);
			}
			if (pPicCtrlProp->m_eForegroundVerAlignMode.wReserved1 == 1)
			{
				pPicCtrl->SetFgImageAlignModeVer(pPicCtrlProp->m_eForegroundVerAlignMode);
			}
		}
	}
	else
	{
		CWLWnd *pWLCtrl = (CWLWnd *)pApplyCtrl;

		// Move control
		if (pCtrlProp->m_rcControl.wReserved1 == 1)
		{
			CRect rcNew = pCtrlProp->m_rcControl;
			pWLCtrl->MoveWindow(rcNew);
		}

		// Show or hide control
		if (pCtrlProp->m_bVisible.wReserved1 == 1)
		{
			pWLCtrl->ShowWindow(pCtrlProp->m_bVisible ? SW_SHOW : SW_HIDE);
		}

		// Transparent
		if (pCtrlProp->m_lBkImgTransparent.wReserved1 == 1)
		{
			pWLCtrl->SetBkImgTransparent((BYTE)(LONG)pCtrlProp->m_lBkImgTransparent);
		}

		// background
		if (pCtrlProp->m_strBKN.wReserved1 == 1)
		{
			pWLCtrl->SetBkImage(pCtrlProp->m_strBKN);
		}

		pWLCtrl->GetParent()->RedrawWindow(NULL, NULL,
			RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
	}

	return 0;
}

CUIWndHandler *LibUIDK::FindUIWndHandler(UINT uWinID, LPARAM lData)
{
	if (uWinID <= 0)
	{
		return NULL;
	}

	_IUI_THREAD_STATE *pThreadState = IUIGetThreadState();
	if (pThreadState == NULL)
	{
		ASSERT(FALSE);
		return NULL;
	}

	std::map<UINT, std::vector<CUIWndHandler *> >::iterator it =
		pThreadState->m_mapUIWndHandler.find(uWinID);

	if (it == pThreadState->m_mapUIWndHandler.end())
	{
		return NULL;
	}

	int nWinIDBindUIWndHandlerCount = (int)it->second.size();
	if (nWinIDBindUIWndHandlerCount <= 0)
	{
		ASSERT(FALSE);
		return NULL;
	}

	CUIWndHandler *pHandlerRet = NULL;
	for (int i = 0; i < nWinIDBindUIWndHandlerCount; ++i)
	{
		CUIWndHandler *pHandler = it->second[i];

		if (pHandler->GetUserData() == lData)
		{
			pHandlerRet = pHandler;
			break;
		}
	}

	return pHandlerRet;
}

int LibUIDK::_InstantiateItemBindTemplate(SItemBindObject *pItemBindObject, CWnd *pHostWnd)
{
	if (pItemBindObject == NULL || pHostWnd == NULL)
	{
		return -1;
	}

	if (pItemBindObject->bAlreadyCreateChildren)
	{
		ASSERT(FALSE);
		return -2;
	}

	UIWNDPROPERTIES *pUIWndProp = CUIMgr::GetUIWNDPROPERTIES(pItemBindObject->uBindWndID);
	if (pUIWndProp == NULL)
	{
		return -3;
	}

	CreateChildControls(pUIWndProp, pHostWnd, NULL, NULL, NULL, NULL, &pItemBindObject->vSubWLWnd);

	pItemBindObject->bBindWLWndNeedRelease = TRUE;
	pItemBindObject->bAlreadyCreateChildren = TRUE;

	return 0;
}

// 当一个窗口内的某个windowless控件绑定了一个窗口，或者Tree、List控件的Item绑定了一个窗口时，
// 通过这个接口，可以为绑定的窗口内的控件进行布局。
// pCtrl：tree，list等控件或者绑定了窗口的windowless控件的父窗口
// rcDest: tree, list等控件绑定了窗口的Item相对tree, list的坐标，或绑定了窗口的windowless控件相对父窗口的坐标
// 包含绑定的窗口及其孩子信息的结构体。
int LibUIDK::_RelayoutItemBindObject(CWnd *pCtrl, const CRect &rcDest, SItemBindObject *pItemBindObject)
{
	if (pCtrl == NULL || pItemBindObject == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (pItemBindObject->rcLastParentLayout == rcDest)
	{
		return 1;
	}

	if (pItemBindObject->pBindUIWndProp == NULL)
	{
		pItemBindObject->pBindUIWndProp = CUIMgr::GetUIWNDPROPERTIES(pItemBindObject->uBindWndID);
	}

	UIWNDPROPERTIES *pUIWndProp = pItemBindObject->pBindUIWndProp;
	if (pUIWndProp == NULL)
	{
		ASSERT(FALSE);
		return -1;
	}

	RelayoutChildren(pCtrl, pUIWndProp,
		rcDest.Width(), rcDest.Height(), rcDest.Width(), rcDest.Height(),
		&pItemBindObject->vSubWLWnd, (bool)pUIWndProp->m_bKeepChildrenAspectRatio,
		(LONG)pUIWndProp->m_lMaxHorRatio, (LONG)pUIWndProp->m_lMaxVerRatio, rcDest.TopLeft(), TRUE);

	pItemBindObject->rcLastParentLayout = rcDest;

	return 0;
}

int SItemBindObject::ReleaseChildren(BOOL bDeleteMemory)
{
	std::vector<CChildInfo>::iterator it = vSubWLWnd.begin();

	for (;  it != vSubWLWnd.end(); ++it)
	{
		if (it->m_pChildCtrl != NULL)
		{
			((CWLWnd *)it->m_pChildCtrl)->DestroyWindow();
		}

		if (bDeleteMemory)
		{
			delete ((CWLWnd *)it->m_pChildCtrl);
		}
	}

	vSubWLWnd.clear();

	return 0;
}

// mask the background in Windows98
int LibUIDK::BackgroundMask(CWnd *pWnd, CString szMaskBitmap, COLORREF crMask)
{
	CIUIImage hbmp;
	hbmp.AutoLoad(szMaskBitmap);
	int nRet = BackgroundMaskG(pWnd, hbmp, crMask);
	return nRet;
}

// Alpha blend a color to dest dc.
int LibUIDK::BitAlphaBlendColor(HDC hdcDest, LPCRECT lpRect, COLORREF crAlphaBlend)
{
	CDC *pDC = CDC::FromHandle(hdcDest);
	CRect rcDest = *lpRect;

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(pDC, rcDest.Width(), rcDest.Height());

	CBitmap *pbmpMemOld = dcMem.SelectObject(&bmpMem);

	dcMem.FillSolidRect(rcDest, crAlphaBlend);

	// Alpha blend
	BLENDFUNCTION blendFunc;
	memset(&blendFunc, 0, sizeof(BLENDFUNCTION));
	blendFunc.AlphaFormat = 0 ;
	blendFunc.BlendFlags = 0;
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.SourceConstantAlpha = 99;

	::AlphaBlend(pDC->GetSafeHdc(),
		rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(),
		dcMem.GetSafeHdc(),
		rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(),
		blendFunc);

	dcMem.SelectObject(pbmpMemOld);

	return 0;
}

int LibUIDK::BackgroundMaskG(CWnd *pWnd, HBITMAP hBmpMask, COLORREF crMask)
{
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));
	if (hBmpMask == NULL)
	{
		return -1;
	}

	ASSERT(::GetObjectType(hBmpMask) == OBJ_BITMAP);

	CDC *pDC = pWnd->GetWindowDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	HBITMAP hOldMemBmp = (HBITMAP)::SelectObject(memDC.GetSafeHdc(), hBmpMask);

	CRect rectWnd;
	pWnd->GetWindowRect(&rectWnd);
	int w = rectWnd.Width();
	int h = rectWnd.Height();

	CRgn wndRgn;
	wndRgn.CreateRectRgn(0, 0, w, h);
	for (int x = 0; x <= w; ++x)
	{
		for (int y = 0; y <= h; ++y)
		{
			COLORREF crCur = memDC.GetPixel(x, y);
			if (crCur == crMask)
			{
				int y2 = y + 1;
				while (y <= h && memDC.GetPixel(x, y2) == crMask)
				{
					y2++;
				}

				CRgn rgnTemp;
				rgnTemp.CreateRectRgn(x, y, x + 1, y2);
				wndRgn.CombineRgn(&wndRgn, &rgnTemp, RGN_XOR);
				rgnTemp.DeleteObject();
				y = y2;
			}
		}
	}

	if (hOldMemBmp != NULL)
	{
		::SelectObject(memDC.GetSafeHdc(), hOldMemBmp);
	}

	if (pDC != NULL)
	{
		pWnd->ReleaseDC(pDC);
	}

	pWnd->SetWindowRgn((HRGN)wndRgn, TRUE);
	CWnd *pParent = pWnd->GetParent();
	if (pParent != NULL && ::IsWindow(pParent->GetSafeHwnd()))
	{
		pParent->UpdateWindow();
	}

	return 0;
}

int LibUIDK::DrawBackgroundG(CWnd *pCtrl, CDC *pDstDC,
	HBITMAP hbmp, IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY)
{
	if (pCtrl->GetSafeHwnd() == NULL)
	{
		return -1;
	}
	if (pDstDC == NULL)
	{
		return -2;
	}
	if (hbmp == NULL)
	{
		return -3;
	}
	CWnd *pParent = pCtrl->GetParent();
	if (pParent == NULL)
	{
		return -4;
	}

	CRect rcWin;
	pCtrl->GetWindowRect(rcWin);
	pParent->ScreenToClient(rcWin);

	LONG lStyle = GetWindowLong(pCtrl->GetSafeHwnd(), GWL_STYLE);
	BOOL bBorder = ((lStyle & WS_BORDER) == WS_BORDER);
	if (bBorder)
	{
		// Draw to dcMem witch size with non-client.
		CDC dcMem;
		dcMem.CreateCompatibleDC(pDstDC);

		CBitmap bmpMem;
		bmpMem.CreateCompatibleBitmap(pDstDC, rcWin.Width(), rcWin.Height());

		CBitmap *pbmpMemOld = dcMem.SelectObject(&bmpMem);

		// Draw content to DC, include non-client
		NineGridBltG(dcMem.GetSafeHdc(), 0, 0, rcWin.Width(), rcWin.Height(),
			hbmp, pt9GridResize, l9GridRepeatX, l9GridRepeatY);

		CRect rcClient;
		pCtrl->GetClientRect(rcClient);

		// Don't draw the border(1 pixel) and scroll bar.
		pDstDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 1, 1, SRCCOPY);
		dcMem.SelectObject(pbmpMemOld);
	}
	else
	{
		// Don't use Client size, since the list may have scroll bar.
		NineGridBltG(pDstDC->GetSafeHdc(), 0, 0, rcWin.Width(), rcWin.Height(),
			hbmp, pt9GridResize, l9GridRepeatX, l9GridRepeatY);
	}

	return 0;
}

int LibUIDK::DrawNCBackgroundG(CWnd *pCtrl, HBITMAP hbmpResized,
	BOOL bDrawResizeScrollBar, HBITMAP hbmpResizeScrollBar, LPCRECT lprcBorderWidth)
{
	if (pCtrl->GetSafeHwnd() == NULL)
	{
		return -1;
	}
	if (lprcBorderWidth == NULL)
	{
		return -2;
	}
	if (hbmpResized == NULL)
	{
		return -3;
	}

	//
	// Draw the new border using background image.
	//
	CWindowDC dc(pCtrl);

	// Don't use rcWnd.right, because rcWnd.right don't equal the value of rcWnd.Width().
	CRect rcWnd;
	pCtrl->GetWindowRect(rcWnd);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, rcWnd.Width(), rcWnd.Height());

	CBitmap *pbmpMemOld = dcMem.SelectObject(&bmpMem);

	// Draw content to dcMem...

	BitBltG(dcMem.GetSafeHdc(), 0, 0, rcWnd.Width(), rcWnd.Height(), hbmpResized, 0, 0, SRCCOPY);

	// Left border
	dc.BitBlt(0, 0, lprcBorderWidth->left, rcWnd.Height(), &dcMem, 0, 0, SRCCOPY);
	// Top border
	dc.BitBlt(0, 0, rcWnd.Width(), lprcBorderWidth->top, &dcMem, 0, 0, SRCCOPY);
	// Right border
	dc.BitBlt(rcWnd.Width() - lprcBorderWidth->right, 0, lprcBorderWidth->right, rcWnd.Height(),
		&dcMem, rcWnd.Width() - lprcBorderWidth->right, 0, SRCCOPY);
	// Bottom border
	dc.BitBlt(0, rcWnd.Height() - lprcBorderWidth->bottom, rcWnd.Width(), lprcBorderWidth->bottom,
		&dcMem, 0, rcWnd.Height() - lprcBorderWidth->bottom, SRCCOPY);

	dcMem.SelectObject(pbmpMemOld);

	//
	// Draw the resize scroll bar(right bottom corner)
	//

	// returns zero if it processes this message
	return 0;
}

int LibUIDK::DrawNCBackgroundG(CWnd *pCtrl, HBITMAP hbmp,
	IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	LPCRECT lprcBorderWidth)
{
	if (pCtrl->GetSafeHwnd() == NULL)
	{
		return -1;
	}
	if (lprcBorderWidth == NULL)
	{
		return -2;
	}
	if (hbmp == NULL)
	{
		return -3;
	}
	CWnd *pParent = pCtrl->GetParent();
	if (pParent == NULL)
	{
		return -4;
	}

	// Draw the new border using background image.
	CWindowDC dc(pCtrl);

	CRect rcWnd;
	pCtrl->GetWindowRect(rcWnd);
	pParent->ScreenToClient(rcWnd);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, rcWnd.Width(), rcWnd.Height());

	CBitmap *pbmpMemOld = dcMem.SelectObject(&bmpMem);

	// Draw content to dcMem...
	NineGridBltG(dcMem.GetSafeHdc(), 0, 0, rcWnd.Width(), rcWnd.Height(),
		hbmp, pt9GridResize, l9GridRepeatX, l9GridRepeatY);

	// Left border
	dc.BitBlt(0, 0, lprcBorderWidth->left, rcWnd.Height(), &dcMem, 0, 0, SRCCOPY);
	// Top border
	dc.BitBlt(0, 0, rcWnd.Width(), lprcBorderWidth->top, &dcMem, 0, 0, SRCCOPY);
	// Right border
	dc.BitBlt(rcWnd.Width() - lprcBorderWidth->right, 0, lprcBorderWidth->right, rcWnd.Height(),
		&dcMem, rcWnd.Width() - lprcBorderWidth->right, 0, SRCCOPY);
	// Bottom border
	dc.BitBlt(0, rcWnd.Height() - lprcBorderWidth->bottom, rcWnd.Width(), lprcBorderWidth->bottom,
		&dcMem, 0, rcWnd.Height() - lprcBorderWidth->bottom, SRCCOPY);

	dcMem.SelectObject(pbmpMemOld);

	// returns zero if it processes this message
	return 0;
}

int LibUIDK::GetDPIBitmapG(HBITMAP hSrcBmp, HBITMAP *phDstBmp)
{
	if (phDstBmp == NULL)
	{
		return -1;
	}

	SIZE size;
	CIUIImage::GetBitmapSizeG(hSrcBmp, &size);
	int nDPIX = CUIMgr::GetLogPixelsX();
	int nDPIY = CUIMgr::GetLogPixelsY();
	int nLogPixelsX = GetDPIX();
	int nLogPixelsY = GetDPIY();

	int nNewWidth = int((float)nLogPixelsX * size.cx / nDPIX);
	int nNewHeight = int((float)nLogPixelsY * size.cy / nDPIY);
	if (nDPIX == -1 || nDPIX == 0 || nDPIY == -1 || nDPIY == 0)
	{
		nNewWidth = size.cx;
		nNewHeight = size.cy;
	}

	CIUIImage::StretchBitmapG(hSrcBmp, phDstBmp, nNewWidth, nNewHeight);

	return 0;
}

HBITMAP LibUIDK::GetDPIBitmapG(HBITMAP hSrcBmp, BOOL bDeleteSrc/* = TRUE*/)
{
	HBITMAP bmpDPINew = NULL;

	if (!CUIMgr::IsEnableDPI() && bDeleteSrc)
	{
		return hSrcBmp;
	}

	GetDPIBitmapG(hSrcBmp, &bmpDPINew);
	if (bDeleteSrc)
	{
		SafeDeleteHBITMAP(hSrcBmp);
	}

	return bmpDPINew;
}

HBRUSH LibUIDK::CreatePatternBrushG(HBITMAP hImage, COLORREF crBackground/* = RGB(255, 255, 255)*/)
{
	if (hImage == NULL)
	{
		return NULL;
	}

	HBRUSH hBrush = ::CreatePatternBrush(hImage);

	return hBrush;
}

void LibUIDK::ASSERT_HBITMAP(HBITMAP obj)
{
	ASSERT(::GetObjectType(obj) == OBJ_BITMAP);
}

int LibUIDK::SafeDeleteHBITMAP(HBITMAP &hBmp)
{
	if (hBmp == NULL)
	{
		return -1;
	}

	::DeleteObject(hBmp);
	hBmp = NULL;

	return 0;
}

HBITMAP LibUIDK::CreateGradientBitmap(HDC hDC, COLORREF crStart,
	COLORREF crEnd, int nWidth, int nHeight, int nDir, int nNumColors)
{
	if (nNumColors > 256)
	{
		nNumColors = 256;
	}

	COLORREF crPalVal[256];
	memset(crPalVal, 0, sizeof(crPalVal));

	int r1 = GetRValue(crStart);
	int g1 = GetGValue(crStart);
	int b1 = GetBValue(crStart);

	int r2 = GetRValue(crEnd);
	int g2 = GetGValue(crEnd);
	int b2 = GetBValue(crEnd);

	for (int nIndex = 0; nIndex < nNumColors; ++nIndex)
	{
		BYTE btRed = (BYTE)(r1 + MulDiv((r2 - r1), nIndex, nNumColors - 1));
		BYTE btGreen = (BYTE)(g1 + MulDiv((g2 - g1), nIndex, nNumColors - 1));
		BYTE btBlue = (BYTE)(b1 + MulDiv((b2 - b1), nIndex, nNumColors - 1));

		crPalVal[nIndex] = RGB(btRed, btGreen, btBlue);
	}

	BITMAPINFO GradBitInfo;
	memset(&GradBitInfo, 0, sizeof(BITMAPINFO));
	GradBitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	GradBitInfo.bmiHeader.biWidth = nWidth;
	GradBitInfo.bmiHeader.biHeight = nHeight;
	GradBitInfo.bmiHeader.biPlanes = 1;
	GradBitInfo.bmiHeader.biBitCount = 32;
	GradBitInfo.bmiHeader.biCompression = BI_RGB;

	LPDWORD pGradBits = (DWORD *)malloc(nWidth * nHeight * sizeof(DWORD));
	if (nDir == 0)		// From bottom to top
	{
		for (int y = 0; y < nHeight; ++y)
		{
			for (int x = 0; x < nWidth; ++x)
			{
				*(pGradBits + (y * nWidth) + x) = crPalVal[MulDiv(nNumColors, y, nHeight)];
			}
		}
	}
	else if (nDir == 1)
	{
		for (int y = 0; y < nHeight; ++y)
		{
			int l = MulDiv((nNumColors / 2), y, nHeight);
			int r = l + (nNumColors / 2) - 1;
			for (int x = 0; x < nWidth; ++x)
			{
				*(pGradBits + (y * nWidth) + x) = crPalVal[l + MulDiv((r - l), x, nWidth)];
			}
		}
	}
	else if (nDir == 2)		// From left to right
	{
		for (int x = 0; x < nWidth; ++x)
		{
			for (int y = 0; y < nHeight; ++y)
			{
				*(pGradBits + (y * nWidth) + x) = crPalVal[MulDiv(nNumColors, x, nWidth)];
			}
		}
	}
	else if (nDir == 3)
	{
		for (int y = 0; y < nHeight; ++y)
		{
			int r = MulDiv((nNumColors / 2), y, nHeight);
			int l = r + (nNumColors / 2) - 1;
			for (int x = 0; x < nWidth; ++x)
			{
				*(pGradBits + (y * nWidth) + x) = crPalVal[l + MulDiv((r - l), x, nWidth)];
			}
		}
	}

	HBITMAP hBmp = CreateDIBitmap(hDC, &GradBitInfo.bmiHeader,
			CBM_INIT, pGradBits, &GradBitInfo, DIB_RGB_COLORS);

	free(pGradBits);

	return hBmp;
}

int LibUIDK::IUIBackgroundMask(CWnd *pWnd, HIUIIMAGE himgMask, COLORREF crMask)
{
	if (pWnd->GetSafeHwnd() == NULL)
	{
		TRACE(_T("LibUIDK===Parameter 1 of IUIBackgroundMask is invalid.\n"));
		return -1;
	}

	himgMask->SafeLoadSavedImage();

	if (himgMask->GetSafeHBITMAP() == NULL)
	{
		TRACE(_T("LibUIDK===Parameter 2 of IUIBackgroundMask is invalid.\n"));
		return -1;
	}

	return BackgroundMaskG(pWnd, *himgMask, crMask);
}

int LibUIDK::IUIDrawBackground(CWnd *pCtrl, CDC *pDstDC, HIUIIMAGE hbmp,
	IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY)
{
	if (pCtrl->GetSafeHwnd() == NULL)
	{
		return -1;
	}
	if (pDstDC == NULL)
	{
		return -2;
	}
	if (hbmp == NULL)
	{
		return -3;
	}

	hbmp->SafeLoadSavedImage();
	if (hbmp->GetSafeHBITMAP() == NULL)
	{
		return -4;
	}

	CWnd *pParent = pCtrl->GetParent();
	if (pParent == NULL)
	{
		return -5;
	}

	CRect rcWin;
	pCtrl->GetWindowRect(rcWin);
	pParent->ScreenToClient(rcWin);

	LONG lStyle = GetWindowLong(pCtrl->GetSafeHwnd(), GWL_STYLE);
	BOOL bBorder = ((lStyle & WS_BORDER) == WS_BORDER);
	if (bBorder)
	{
		// Draw to dcMem witch size with non-client.
		CDC dcMem;
		dcMem.CreateCompatibleDC(pDstDC);

		CBitmap bmpMem;
		bmpMem.CreateCompatibleBitmap(pDstDC, rcWin.Width(), rcWin.Height());

		CBitmap *pbmpMemOld = dcMem.SelectObject(&bmpMem);

		// Draw content to DC, include non-client
		NineGridBltG(dcMem.GetSafeHdc(), 0, 0, rcWin.Width(), rcWin.Height(),
			hbmp->GetSafeHBITMAP(), pt9GridResize, l9GridRepeatX, l9GridRepeatY);

		CRect rcClient;
		pCtrl->GetClientRect(rcClient);

		// Don't draw the border(1 pixel) and scroll bar.
		pDstDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 1, 1, SRCCOPY);
		dcMem.SelectObject(pbmpMemOld);
	}
	else
	{
		// Don't use Client size, since the list may have scroll bar.
		IUINineGridBlt(pDstDC->GetSafeHdc(), 0, 0, rcWin.Width(), rcWin.Height(),
			hbmp, pt9GridResize, l9GridRepeatX, l9GridRepeatY);
	}

	return 0;
}

int LibUIDK::IUIDrawNCBackground(CWnd *pCtrl, HIUIIMAGE himgResized, BOOL bDrawResizeScrollBar,
	HIUIIMAGE himgResizeScrollBar, LPCRECT lprcBorderWidth)
{
	if (pCtrl->GetSafeHwnd() == NULL)
	{
		return -1;
	}
	if (himgResized == NULL)
	{
		return -2;
	}
	himgResized->SafeLoadSavedImage();
	if (himgResized->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}
	if (lprcBorderWidth == NULL)
	{
		return -4;
	}

	//
	// Draw the new border using background image.
	//
	CWindowDC dc(pCtrl);

	// Don't use rcWnd.right, because rcWnd.right don't equal the value of rcWnd.Width().
	CRect rcWnd;
	pCtrl->GetWindowRect(rcWnd);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, rcWnd.Width(), rcWnd.Height());

	CBitmap *pbmpMemOld = dcMem.SelectObject(&bmpMem);

	// Clip client area.
	CRect rcWnd00(rcWnd);
	rcWnd00.OffsetRect(-rcWnd00.left, -rcWnd00.top);	// Set origin to (0, 0)
	CRect rcClip(rcWnd00);
	rcClip.DeflateRect(lprcBorderWidth);
	dcMem.ExcludeClipRect(rcClip);

	//
	// Draw parent part.
	//
	if (himgResized->IsHasAlphaChannel())
	{
		pCtrl->SendMessage(WM_DRAWPARENTPART, 0, (LPARAM)&dcMem);
	}

	//
	// Draw content to dcMem...
	//
	IUIBitBlt(dcMem.GetSafeHdc(), 0, 0, rcWnd.Width(), rcWnd.Height(), himgResized, 0, 0, SRCCOPY);

	// Left border
	dc.BitBlt(0, 0, lprcBorderWidth->left, rcWnd.Height(), &dcMem, 0, 0, SRCCOPY);
	// Top border
	dc.BitBlt(0, 0, rcWnd.Width(), lprcBorderWidth->top, &dcMem, 0, 0, SRCCOPY);
	// Right border
	dc.BitBlt(rcWnd.Width() - lprcBorderWidth->right, 0, lprcBorderWidth->right, rcWnd.Height(),
		&dcMem, rcWnd.Width() - lprcBorderWidth->right, 0, SRCCOPY);
	// Bottom border
	dc.BitBlt(0, rcWnd.Height() - lprcBorderWidth->bottom, rcWnd.Width(), lprcBorderWidth->bottom,
		&dcMem, 0, rcWnd.Height() - lprcBorderWidth->bottom, SRCCOPY);

	dcMem.SelectObject(pbmpMemOld);

	// returns zero if it processes this message
	return 0;
}

int LibUIDK::IUIDrawNCBackground(CWnd *pCtrl, HIUIIMAGE hbmp,
	IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	LPCRECT lprcBorderWidth)
{
	if (pCtrl->GetSafeHwnd() == NULL)
	{
		return -1;
	}
	if (hbmp == NULL)
	{
		return -2;
	}
	hbmp->SafeLoadSavedImage();
	if (hbmp->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	if (lprcBorderWidth == NULL)
	{
		return -4;
	}
	CWnd *pParent = pCtrl->GetParent();
	if (pParent == NULL)
	{
		return -5;
	}

	// Draw the new border using background image.
	CWindowDC dc(pCtrl);

	CRect rcWnd;
	pCtrl->GetWindowRect(rcWnd);
	pParent->ScreenToClient(rcWnd);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, rcWnd.Width(), rcWnd.Height());

	CBitmap *pbmpMemOld = dcMem.SelectObject(&bmpMem);

	// Clip client area.
	CRect rcWnd00(rcWnd);
	rcWnd00.OffsetRect(-rcWnd00.left, -rcWnd00.top);	// Set origin to (0, 0)
	CRect rcClip(rcWnd00);
	rcClip.DeflateRect(lprcBorderWidth);
	dcMem.ExcludeClipRect(rcClip);

	//
	// Draw parent part.
	//
	if (hbmp->IsHasAlphaChannel())
	{
		pCtrl->SendMessage(WM_DRAWPARENTPART, 0, (LPARAM)&dcMem);
	}

	//
	// Draw content to dcMem...
	//
	IUINineGridBlt(dcMem.GetSafeHdc(), 0, 0, rcWnd.Width(), rcWnd.Height(), hbmp,
		pt9GridResize, l9GridRepeatX, l9GridRepeatY);

	// Left border
	dc.BitBlt(0, 0, lprcBorderWidth->left, rcWnd.Height(), &dcMem, 0, 0, SRCCOPY);
	// Top border
	dc.BitBlt(0, 0, rcWnd.Width(), lprcBorderWidth->top, &dcMem, 0, 0, SRCCOPY);
	// Right border
	dc.BitBlt(rcWnd.Width() - lprcBorderWidth->right, 0, lprcBorderWidth->right, rcWnd.Height(),
		&dcMem, rcWnd.Width() - lprcBorderWidth->right, 0, SRCCOPY);
	// Bottom border
	dc.BitBlt(0, rcWnd.Height() - lprcBorderWidth->bottom, rcWnd.Width(), lprcBorderWidth->bottom,
		&dcMem, 0, rcWnd.Height() - lprcBorderWidth->bottom, SRCCOPY);

	dcMem.SelectObject(pbmpMemOld);

	// returns zero if it processes this message
	return 0;
}

int LibUIDK::IUIDrawCombineNCBackground(CWnd *pCtrl, HIUIIMAGE himgCombine,
	IMAGE_RESIZE_MODE eIrm, POINT pt9GridResize, LONG l9GridRepeatX, LONG l9GridRepeatY,
	int nPart, int nPartIndex, LPCRECT lprcBorderWidth)
{
	if (pCtrl->GetSafeHwnd() == NULL)
	{
		return -1;
	}
	if (himgCombine == NULL)
	{
		return -2;
	}
	himgCombine->SafeLoadSavedImage();
	if (himgCombine->GetSafeHBITMAP() == NULL)
	{
		return -3;
	}

	if (lprcBorderWidth == NULL)
	{
		return -4;
	}
	CWnd *pParent = pCtrl->GetParent();
	if (pParent == NULL)
	{
		return -5;
	}

	// Draw the new border using background image.
	CWindowDC dc(pCtrl);

	CRect rcWnd;
	pCtrl->GetWindowRect(rcWnd);
	pParent->ScreenToClient(rcWnd);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, rcWnd.Width(), rcWnd.Height());

	CBitmap *pbmpMemOld = dcMem.SelectObject(&bmpMem);

	// Clip client area.
	CRect rcWnd00(rcWnd);
	rcWnd00.OffsetRect(-rcWnd00.left, -rcWnd00.top);	// Set origin to (0, 0)
	CRect rcClip(rcWnd00);
	rcClip.DeflateRect(lprcBorderWidth);
	dcMem.ExcludeClipRect(rcClip);

	//
	// Draw parent part.
	//
	if (himgCombine->IsHasAlphaChannel())
	{
		pCtrl->SendMessage(WM_DRAWPARENTPART, 0, (LPARAM)&dcMem);
	}

	//
	// Draw content to dcMem...
	//
	if (eIrm == IRM_9GRID)
	{
		IUIPartNineGridBlt(dcMem.GetSafeHdc(), CRect(0, 0, rcWnd.Width(), rcWnd.Height()),
			himgCombine, pt9GridResize, l9GridRepeatX, l9GridRepeatY, nPart, nPartIndex);
	}
	else if (eIrm == IRM_STRETCH
		|| eIrm == IRM_STRETCH_HIGH_QUALITY)
	{
		IUIPartStretchBlt(dcMem.GetSafeHdc(), CRect(0, 0, rcWnd.Width(), rcWnd.Height()),
			himgCombine, nPart, nPartIndex, eIrm);
	}

	// Left border
	dc.BitBlt(0, 0, lprcBorderWidth->left, rcWnd.Height(), &dcMem, 0, 0, SRCCOPY);
	// Top border
	dc.BitBlt(0, 0, rcWnd.Width(), lprcBorderWidth->top, &dcMem, 0, 0, SRCCOPY);
	// Right border
	dc.BitBlt(rcWnd.Width() - lprcBorderWidth->right, 0, lprcBorderWidth->right, rcWnd.Height(),
		&dcMem, rcWnd.Width() - lprcBorderWidth->right, 0, SRCCOPY);
	// Bottom border
	dc.BitBlt(0, rcWnd.Height() - lprcBorderWidth->bottom, rcWnd.Width(), lprcBorderWidth->bottom,
		&dcMem, 0, rcWnd.Height() - lprcBorderWidth->bottom, SRCCOPY);

	dcMem.SelectObject(pbmpMemOld);

	// returns zero if it processes this message
	return 0;
}

BOOL LibUIDK::IsImageHasAlphaChanne(HIUIIMAGE hImage)
{
	if (hImage == NULL)
	{
		return FALSE;
	}

	if (hImage->GetSafeHBITMAP() == NULL)
	{
		hImage->SafeLoadSavedImage();
	}

	if (hImage->GetSafeHBITMAP() == NULL)
	{
		return FALSE;
	}

	return hImage->IsHasAlphaChannel();
}

int LibUIDK::GetDPIX()
{
	if (CUIMgr::IsRunAsDPIX())
	{
		int nDPIX = 96;
		CUIMgr::GetRunAsDPIX(&nDPIX);

		return nDPIX;
	}
	else
	{
		CWindowDC dc(NULL);
		int nDPIX = GetDeviceCaps(dc.GetSafeHdc(), LOGPIXELSX);
		if (nDPIX == -1 || nDPIX == 0)
		{
			return 0;
		}

		return nDPIX;
	}
}

int LibUIDK::GetDPIY()
{
	if (CUIMgr::IsRunAsDPIY())
	{
		int nDPIY = 96;
		CUIMgr::GetRunAsDPIY(&nDPIY);

		return nDPIY;
	}
	else
	{
		CWindowDC dc(NULL);
		int nDPIY = GetDeviceCaps(dc.GetSafeHdc(), LOGPIXELSY);
		if (nDPIY == -1 || nDPIY == 0)
		{
			return 0;
		}

		return nDPIY;
	}
}

int LibUIDK::GetDPIWidth(int nWidth)
{
	if (!CUIMgr::IsEnableDPI())
	{
		return nWidth;
	}

	int nLogPixelsX = GetDPIX();

	nWidth = MulDiv(nWidth, nLogPixelsX, 96);

	return nWidth;
}

int LibUIDK::GetDPIHeight(int nHeight)
{
	if (!CUIMgr::IsEnableDPI())
	{
		return nHeight;
	}

	int nLogPixelsY = GetDPIY();

	nHeight = MulDiv(nHeight, nLogPixelsY, 96);

	return nHeight;
}

int LibUIDK::GetDPIRect(LPRECT lpRect)
{
	if (lpRect == NULL)
	{
		return -1;
	}

	if (!CUIMgr::IsEnableDPI())
	{
		return 1;
	}

	int nLogPixelsX = GetDPIX();
	int nLogPixelsY = GetDPIY();

	int nOldWidth = lpRect->right - lpRect->left;
	int nOldHeight = lpRect->bottom - lpRect->top;
	lpRect->left = MulDiv(lpRect->left, nLogPixelsX, 96);
	lpRect->top = MulDiv(lpRect->top, nLogPixelsY, 96);
	lpRect->right = lpRect->left + MulDiv(nOldWidth, nLogPixelsX, 96);
	lpRect->bottom = lpRect->top + MulDiv(nOldHeight, nLogPixelsY, 96);

	return 0;
}

int LibUIDK::GetDPIPadding(LPRECT lprcPadding)
{
	if (lprcPadding == NULL)
	{
		return -1;
	}

	if (!CUIMgr::IsEnableDPI())
	{
		return 1;
	}

	int nLogPixelsX = GetDPIX();
	int nLogPixelsY = GetDPIY();

	lprcPadding->left = MulDiv(lprcPadding->left, nLogPixelsX, 96);
	lprcPadding->top = MulDiv(lprcPadding->top, nLogPixelsY, 96);
	lprcPadding->right = MulDiv(lprcPadding->right, nLogPixelsX, 96);
	lprcPadding->bottom = MulDiv(lprcPadding->bottom, nLogPixelsY, 96);

	return 0;
}

int LibUIDK::GetDPIPoint(LPPOINT lpPt)
{
	if (lpPt == NULL)
	{
		return -1;
	}

	if (!CUIMgr::IsEnableDPI())
	{
		return 1;
	}

	int nLogPixelsX = GetDPIX();
	int nLogPixelsY = GetDPIY();

	lpPt->x = MulDiv(lpPt->x, nLogPixelsX, 96);
	lpPt->y = MulDiv(lpPt->y, nLogPixelsY, 96);

	return 0;
}

int LibUIDK::GetDPIPointX(long &nX)
{
	int nDPIX = CUIMgr::GetLogPixelsX();
	if (nDPIX == -1 || nDPIX == 0)
	{
		return 0;
	}

	int nLogPixelsX = GetDPIX();
	nX = long(nX * nLogPixelsX / (float)nDPIX);

	return 0;
}

int LibUIDK::GetDPIPointY(long &nY)
{
	int nDPIY = CUIMgr::GetLogPixelsY();
	if (nDPIY == -1 || nDPIY == 0)
	{
		return 0;
	}

	int nLogPixelsY = GetDPIY();
	nY = long(nY * nLogPixelsY / (float)nDPIY);

	return 0;
}

int LibUIDK::GetDPILogFont(LPLOGFONT lpLogFont)
{
	if (lpLogFont == NULL)
	{
		return -1;
	}

	if (!CUIMgr::IsEnableDPI())
	{
		return 1;
	}

	int nLogPixelsY = GetDPIY();
	lpLogFont->lfHeight = MulDiv(lpLogFont->lfHeight, nLogPixelsY, 96);

	return 0;
}

int LibUIDK::GetDPILogFont(IUI_VARIANT *pvarLf)
{
	LOGFONT lf = *pvarLf;
	GetDPILogFont(&lf);
	*pvarLf = (lf);

	return 0;
}

int LibUIDK::DrawDefaultPictureCtrl(HDC hDC, LPCRECT lpcRect)
{
	::Rectangle(hDC, lpcRect->left, lpcRect->top, lpcRect->right, lpcRect->bottom);

	return 0;
}

//////////////////////////////////////////////////////////////////////////


int LibUIDK::LoadCustomResourceA(LPCSTR lpszResName, LPCSTR lpType, LPVOID *lpData, DWORD *pdwSize)
{
	HRSRC hRsrc = FindResourceA(NULL, lpszResName, lpType);
	if (NULL == hRsrc)
	{
		return -1;
	}

	DWORD dwSize = SizeofResource(NULL, hRsrc);
	if (0 == dwSize)
	{
		return -2;
	}

	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	if (NULL == hGlobal)
	{
		return -3;
	}

	LPVOID pBuffer = LockResource(hGlobal);
	if (NULL == pBuffer)
	{
		return -4;
	}

	*lpData = pBuffer;
	*pdwSize = dwSize;

	return 0;
}

void LibUIDK::ASSERT_CIUIImage(const CIUIImage &obj)
{
	ASSERT((HBITMAP)obj != NULL);
}

void LibUIDK::SafeDeleteHGDIOBJ(HGDIOBJ &obj)
{
	if (obj != NULL)
	{
		::DeleteObject(obj);
		obj = NULL;
	}
}
