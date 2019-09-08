// $$Root$$View.cpp : implementation file
//

#include "stdafx.h"
#include "$$Root$$.h"
#include "$$Root$$View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C$$Root$$View

IMPLEMENT_DYNCREATE(C$$Root$$View, CIUIView)

C$$Root$$View::C$$Root$$View()
{
}

C$$Root$$View::~C$$Root$$View()
{
}


BEGIN_MESSAGE_MAP(C$$Root$$View, CIUIView)
	//{{AFX_MSG_MAP(C$$Root$$View)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// C$$Root$$View message handlers

int C$$Root$$View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CIUIView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here

	return 0;
}
