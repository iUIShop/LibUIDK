$$IF(PROJECT_VC60)
#if !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
#define $$FILE_NAME_SYMBOL$$_INCLUDED_

#if _MSC_VER > 1000
$$ENDIF
#pragma once
$$IF(PROJECT_VC60)
#endif // _MSC_VER > 1000
// FileViewWnd.h : header file
$$ENDIF
//

/////////////////////////////////////////////////////////////////////////////
// CFileViewWnd window

class CFileViewWnd : public CUIWnd
{
public:
	CFileViewWnd();
	virtual ~CFileViewWnd();
	void WindowID() { IDD = IDW_FILE_VIEW; }


// Overrides
$$IF(PROJECT_VC60)
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileViewWnd)
$$ENDIF
$$IF(PROJECT_VC60)
	//}}AFX_VIRTUAL
$$ENDIF

	// Generated message map functions
protected:
$$IF(PROJECT_VC60)
	//{{AFX_MSG(CFileViewWnd)
$$IF(VERBOSE)
		// NOTE - the ClassWizard will add and remove member functions here.
$$ENDIF
	//}}AFX_MSG
$$ENDIF
	DECLARE_MESSAGE_MAP()
};

$$IF(PROJECT_VC60)
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
$$ENDIF
