// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B6AB0D5D_8102_464F_8B25_12DB1345C9DA__INCLUDED_)
#define AFX_STDAFX_H__B6AB0D5D_8102_464F_8B25_12DB1345C9DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#ifdef _UNICODE
	#if defined _M_IX86
		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#elif defined _M_IA64
		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#elif defined _M_X64
		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#else
		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#endif
#endif


#include "Bin/Skins/SafeMgr/Resource.h"
#include <LibUIDK.h>
using namespace LibUIDK;

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


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B6AB0D5D_8102_464F_8B25_12DB1345C9DA__INCLUDED_)
