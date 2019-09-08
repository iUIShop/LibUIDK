// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
#define $$FILE_NAME_SYMBOL$$_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

// Check memory leaks alloc by C.
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define IUI_DEBUG_NEW	new(__FILE__, __LINE__)

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
$$IF(OLECTLEX)
#include <afxdisp.h>        // MFC Automation classes
$$ENDIF //OLECTLEX
$$// Include database headers.  For a DLL or app which just includes database
$$//  headers (no database view), conditionally include DAO and ODBC.
$$//  If an actual view is used, always & only include the corresponding header.
$$// First, actual view is used:
$$IF(DBEX)
$$// Here, minimal DB support is requested, or we're a DLL.  No view is chosen.

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

$$ENDIF // DBEX
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

$$IF(SOCKETSEX)
#include <afxsock.h>		// MFC socket extensions
$$ENDIF //SOCKETSEX
$$IF(ATL_SUPPORT)
#include <atlbase.h>
extern CComModule _Module;
$$ENDIF
$$IF(OLEDB)
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include <atldbcli.h>
#include <afxoledb.h>
$$ENDIF


// #ifdef _UNICODE
// 	#if defined _M_IX86
// 		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
// 	#elif defined _M_IA64
// 		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
// 	#elif defined _M_X64
// 		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
// 	#else
// 		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
// 	#endif
// #endif


#include "Bin/Skins/$$root$$/Resource.h"
#include <LibUIDK.h>
using namespace LibUIDK;
using namespace ImageManager;
using namespace Render;

#if(_MSC_VER == 1200) // VC6.0
	#ifdef _DEBUG
		#ifdef _UNICODE
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK60ud.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs60ud.lib")
			#endif
		#else // ANSI
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK60d.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs60d.lib")
			#endif
		#endif // end _UNICODE
	#else // Release
		#ifdef _UNICODE
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK60u.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs60u.lib")
			#endif
		#else // ANSI
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK60.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs60.lib")
			#endif
		#endif // end _UNICODE
	#endif
#endif
#if (_MSC_VER == 1310) // VC2003
	#ifdef _DEBUG
		#ifdef _UNICODE
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK71ud.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs71ud.lib")
			#endif
		#else // ANSI
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK71d.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs71d.lib")
			#endif
		#endif // end _UNICODE
	#else // Release
		#ifdef _UNICODE
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK71u.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs71u.lib")
			#endif
		#else // ANSI
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK71.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs71.lib")
			#endif
		#endif // end _UNICODE
	#endif
#endif
#if (_MSC_VER == 1400)	// VC2005
	#ifdef _DEBUG
		#ifdef _UNICODE
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK80ud.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs80ud.lib")
			#endif
		#else // ANSI
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK80d.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs80d.lib")
			#endif
		#endif // end _UNICODE
	#else // Release
		#ifdef _UNICODE
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK80u.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs80u.lib")
			#endif
		#else // ANSI
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK80.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs80.lib")
			#endif
		#endif // end _UNICODE
	#endif
#endif
#if (_MSC_VER == 1500)	// VC2008
	#ifdef _DEBUG
		#ifdef _UNICODE
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK90ud.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs90ud.lib")
			#endif
		#else // ANSI
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK90d.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs90d.lib")
			#endif
		#endif // end _UNICODE
	#else // Release
		#ifdef _UNICODE
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK90u.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs90u.lib")
			#endif
		#else // ANSI
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK90.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs90.lib")
			#endif
		#endif // end _UNICODE
	#endif
#endif
#if (_MSC_VER == 1600)	// VC2010
	#ifdef _DEBUG
		#ifdef _UNICODE
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK100ud.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs100ud.lib")
			#endif
		#else // ANSI
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK100d.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs100d.lib")
			#endif
		#endif // end _UNICODE
	#else // Release
		#ifdef _UNICODE
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK100u.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs100u.lib")
			#endif
		#else // ANSI
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK100.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs100.lib")
			#endif
		#endif // end _UNICODE
	#endif
#endif
#if (_MSC_VER == 1700)	// VC2012
	#ifdef _DEBUG
		#ifdef _UNICODE
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK110ud.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs110ud.lib")
			#endif
		#else // ANSI
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK110d.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs110d.lib")
			#endif
		#endif // end _UNICODE
	#else // Release
		#ifdef _UNICODE
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK110u.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs110u.lib")
			#endif
		#else // ANSI
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK110.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs110.lib")
			#endif
		#endif // end _UNICODE
	#endif
#endif
#if (_MSC_VER == 1800)	// VC2013
	#ifdef _DEBUG
		#ifdef _UNICODE
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK120ud.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs120ud.lib")
			#endif
		#else // ANSI
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK120d.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs120d.lib")
			#endif
		#endif // end _UNICODE
	#else // Release
		#ifdef _UNICODE
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK120u.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs120u.lib")
			#endif
		#else // ANSI
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK120.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs120.lib")
			#endif
		#endif // end _UNICODE
	#endif
#endif
#if (_MSC_VER == 1900)	// VC2015
	#ifdef _DEBUG
		#ifdef _UNICODE
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK140ud.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs140ud.lib")
			#endif
		#else // ANSI
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK140d.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs140d.lib")
			#endif
		#endif // end _UNICODE
	#else // Release
		#ifdef _UNICODE
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK140u.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs140u.lib")
			#endif
		#else // ANSI
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK140.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs140.lib")
			#endif
		#endif // end _UNICODE
	#endif
#endif
#if (_MSC_VER >= 1910)	// VC2017
	#ifdef _DEBUG
		#ifdef _UNICODE
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDKud_vs2017.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKsud_vs2017.lib")
			#endif
		#else // ANSI
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDKd_vs2017.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKsd_vs2017.lib")
			#endif
		#endif // end _UNICODE
	#else // Release
		#ifdef _UNICODE
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDKu_vs2017.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKsu_vs2017.lib")
			#endif
		#else // ANSI
			#ifdef _AFXDLL
				#pragma comment (lib, "LibUIDK_vs2017.lib")
			#else // Static
				#pragma comment (lib, "LibUIDKs_vs2017.lib")
			#endif
		#endif // end _UNICODE
	#endif
#endif


//{{AFX_INSERT_LOCATION}}
// $$INSERT_LOCATION_COMMENT$$

#endif // !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
