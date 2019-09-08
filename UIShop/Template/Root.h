// $$root$$.h : main header file for the $$ROOT$$ application
//
$$IF(PROJECT_VC60)

#if !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
#define $$FILE_NAME_SYMBOL$$_INCLUDED_

#if _MSC_VER > 1000
$$ENDIF
#pragma once
$$IF(PROJECT_VC60)
#endif // _MSC_VER > 1000
$$ENDIF

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class CMainFrame;
$$IF(!MDI)
class CMainWndHandler;
$$ENDIF

/////////////////////////////////////////////////////////////////////////////
// $$APP_CLASS$$:
// See $$root$$.cpp for the implementation of this class
//

$$IF(MDI)
class $$APP_CLASS$$ : public CIUIWinApp
$$ENDIF
$$IF(!MDI)
class $$APP_CLASS$$ : public CWinApp
$$ENDIF
{
public:
	$$APP_CLASS$$();

public:
	BOOL IsCallLibUIDKMsgMap();

// Overrides
$$IF(PROJECT_VC60)
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL($$APP_CLASS$$)
$$ENDIF
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
$$IF(PROJECT_VC60)
	//}}AFX_VIRTUAL
$$ENDIF

// Implementation

public:
$$IF(PROJECT_VC60)
	//{{AFX_MSG($$APP_CLASS$$)
$$IF(VERBOSE)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
$$ENDIF
	//}}AFX_MSG
$$ENDIF
	DECLARE_MESSAGE_MAP()

private:
$$IF(MDI)
	CMainFrame *m_pFrame;
$$ENDIF
	BOOL m_bCallLibUIDKMsgMap;
	CUIWnd *m_pMainFrame;
$$IF(!MDI)
	CMainWndHandler *m_pMainWndHandler;
$$ENDIF
};

$$IF(PROJECT_VC60)

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// $$INSERT_LOCATION_COMMENT$$

#endif // !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
$$ENDIF
