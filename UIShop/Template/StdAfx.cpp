// stdafx.cpp : source file that includes just the standard includes
//	$$root$$.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

$$IF(ATL_SUPPORT)
#include <atlimpl.cpp>
$$ENDIF

$$IF(OLEDB)
CComModule _Module;
#include <atlimpl.cpp>
$$ENDIF

