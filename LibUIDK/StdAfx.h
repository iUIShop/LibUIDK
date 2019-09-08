// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#if !defined(AFX_STDAFX_H__198FBB01_C38A_4DFD_B44A_17F676E4A0B0__INCLUDED_)
#define AFX_STDAFX_H__198FBB01_C38A_4DFD_B44A_17F676E4A0B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if _MSC_VER >= 1500
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#if(_MSC_VER <= 1200) // VC6.0
#define _WIN32_WINNT 0x0501		// Windows Server 2003, Windows XP
#define _WIN32_IE 0x0601
#define WINVER 0x0500			// for COMBOBOXINFO
#endif

#if(_MSC_VER > 1310) // VC7.1
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define IUI_DEBUG_NEW	new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // VC7.1

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

// If defined _AFXDLL (dynamic link to MFC) and not include the following two files,
// will cause error LNK2001: unresolved external symbol "protected: static struct AFX_INTERFACEMAP const COleControlSite::interfaceMap" (?interfaceMap@COleControlSite@@1UAFX_INTERFACEMAP@@B)
// If not define _AFXDLL(static link to MFC) and include the following two files,
// will cause error C2496: 'protected: static struct AFX_INTERFACEMAP const CCustomOleControlSite::interfaceMap' : 'selectany' can only be applied to statically initialized data items with external linkage
#ifdef _AFXDLL
#include <afxdisp.h>
#include <afxdtctl.h>
#endif

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#ifdef AFX_COMDAT
#undef AFX_COMDAT
#define AFX_COMDAT __declspec(selectany)
#endif

#define DELETE_EXCEPTION(e) do { e->Delete(); } while (0)

#include "LibUIDK.h"
#include <Shlwapi.h>
#pragma comment (lib, "shlwapi.lib")
using namespace LibUIDK;
using namespace Render;
using namespace ImageManager;
using namespace FontManager;
using namespace ControlResMgr;
#include "ResourceMgr.h"
#include "ControlMember.h"


// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__198FBB01_C38A_4DFD_B44A_17F676E4A0B0__INCLUDED_)
