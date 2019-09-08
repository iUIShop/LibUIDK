// $$CppFile$$ : implementation of the $$Class$$ class
//

#include "stdafx.h"
#include "$$HFile$$"

#ifdef _DEBUG
#define new DEBUG_NEW
$$IF(CODESTYLE_VC60)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
$$ENDIF
#endif

$$IF(CODESTYLE_VC60)
/////////////////////////////////////////////////////////////////////////////
$$ENDIF
// $$Class$$

BEGIN_MESSAGE_MAP($$Class$$, CUIWnd)
$$IF(CODESTYLE_VC60)
	//{{AFX_MSG_MAP($$Class$$)
$$ENDIF
	ON_WM_CREATE()
$$IF(GEN_HANDLER_MSG_MAP)
$$ENDIF
$$IF(CODESTYLE_VC60)
	//}}AFX_MSG_MAP
$$ENDIF
END_MESSAGE_MAP()

$$IF(CODESTYLE_VC60)
/////////////////////////////////////////////////////////////////////////////
$$ENDIF
// $$Class$$ construction/destruction

$$Class$$::$$Class$$()
{
$$IF(GEN_VARIABLE_INIT)
$$ENDIF
$$IF(GEN_DO_DATA_EXCHANGE_VARIABLE_INIT)
$$ENDIF
}

$$Class$$::~$$Class$$()
{
}

$$IF(GEN_DO_DATA_EXCHANGE)
void $$Class$$::DoDataExchange(CDataExchange* pDX)
{
	CUIWnd::DoDataExchange(pDX);

$$IF(CODESTYLE_VC60)
	//{{AFX_DATA_MAP($$Class$$)
$$ENDIF
$$IF(GEN_DO_DATA_EXCHANGE_VARIABLE_BIND)
$$ENDIF
$$IF(CODESTYLE_VC60)
	//}}AFX_DATA_MAP
$$ENDIF
}

$$ENDIF
$$IF(GEN_HANDLER)

//////////////////////////////////////////////////////////////////////////
// protected

int $$Class$$::InitControls()
{
$$IF(GEN_VARIABLE_ASSIGN)
$$ENDIF
	return 0;
}
$$ENDIF
$$IF(GEN_HANDLER_INIT_CONTROLS_IMPLE)
$$ENDIF

/////////////////////////////////////////////////////////////////////////////
// $$Class$$ message handlers

int $$Class$$::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CUIWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

$$IF(GEN_HANDLER)
	if (InitControls() != 0)
		return -1;

$$IF(GEN_HANDLER_CALL_INIT)
$$ENDIF

$$ENDIF
	return 0;
}
$$IF(GEN_HANDLER_IMPLE)
$$ENDIF
