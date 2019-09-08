// $$HFile$$ : interface of the $$Class$$ class
//
$$IF(CODESTYLE_VC60)
/////////////////////////////////////////////////////////////////////////////

#if !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
#define $$FILE_NAME_SYMBOL$$_INCLUDED_

#if _MSC_VER > 1000
$$ENDIF
#pragma once
$$IF(CODESTYLE_VC60)
#endif // _MSC_VER > 1000
$$ENDIF

class $$Class$$ : public CUIWnd
{
public:
	$$Class$$();
	virtual ~$$Class$$();
	void WindowID() { IDD = $$WindowID$$; }
$$IF(GEN_HANDLER)

protected:
	int InitControls();
$$ENDIF
$$IF(GEN_HANDLER_INIT_CONTROLS_DECLARE)
$$ENDIF

public:

$$IF(CODESTYLE_VC60)
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL($$Class$$)
$$ENDIF
$$IF(GEN_DO_DATA_EXCHANGE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
$$ENDIF
$$IF(CODESTYLE_VC60)
	//}}AFX_VIRTUAL

$$ENDIF
// Generated message map functions
protected:
$$IF(CODESTYLE_VC60)
	//{{AFX_MSG($$Class$$)
$$ENDIF
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
$$IF(GEN_HANDLER_DECLARE)
$$ENDIF
$$IF(CODESTYLE_VC60)
	//}}AFX_MSG
$$ENDIF
	DECLARE_MESSAGE_MAP()
$$IF(GEN_VARIABLE_DECLARE)
$$ENDIF
$$IF(GEN_DO_DATA_EXCHANGE_VARIABLE_DECLARE)
$$ENDIF
};
$$IF(CODESTYLE_VC60)

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// $$INSERT_LOCATION_COMMENT$$

#endif // !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
$$ENDIF
