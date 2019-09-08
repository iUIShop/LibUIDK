
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define IUI_DEBUG_NEW	new(_NORMAL_BLOCK, __FILE__, __LINE__)

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars


// #ifdef _UNICODE
// #if defined _M_IX86
// #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
// #elif defined _M_IA64
// #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
// #elif defined _M_X64
// #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
// #else
// #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
// #endif
// #endif


#include "../LibUIDK/LibUIDK.h"
using namespace LibUIDK;


// Custom define
#define WM_GETFOCUSCTRL										WM_USER+1000	// return: WM_GETFOCUSCTRL pointer
#define WM_SETFOCUSCTRL										WM_USER+1001	// WPARAM: new Focus CTRLPROPERTIES pointer; LPARAM: bFocus.
#define WM_GETSELCOUNT										WM_USER+1003
#define WM_CHILDMOVING										WM_USER+1004	// WPARAM: sender window pointer; LPARAM: not use.

#define UIVN_FOCUSCHANGED									(NM_FIRST-100)	// Parameter same as CSkinTreeCtrl::TVN_SELCHANGED. LPARAM: NMUISHOPVIEW *.
#define UIVN_ADDNEWCONTROL									(NM_FIRST-101)	// LPARAM: NMUISHOPVIEW *.
#define UIVN_PASTENEWCONTROL								(NM_FIRST-102)	// LPARAM: NMUISHOPVIEW *.


struct NMUISHOPVIEW
{
	NMHDR hdr;
	CTRLPROPERTIES *pOldCtrlProp;
	CTRLPROPERTIES *pNewCtrlProp;

	CObject *pNewCtrl;
};
