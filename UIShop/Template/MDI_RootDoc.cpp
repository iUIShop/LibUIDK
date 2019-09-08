// $$Root$$Doc.cpp : implementation file
//

#include "stdafx.h"
#include "$$Root$$.h"
#include "$$Root$$Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C$$Root$$Doc

IMPLEMENT_DYNCREATE(C$$Root$$Doc, CIUIDocument)

C$$Root$$Doc::C$$Root$$Doc()
{
}

C$$Root$$Doc::~C$$Root$$Doc()
{
}


BEGIN_MESSAGE_MAP(C$$Root$$Doc, CIUIDocument)
	//{{AFX_MSG_MAP(C$$Root$$Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C$$Root$$Doc diagnostics

#ifdef _DEBUG
void C$$Root$$Doc::AssertValid() const
{
	CIUIDocument::AssertValid();
}

void C$$Root$$Doc::Dump(CDumpContext& dc) const
{
	CIUIDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// virtual

void C$$Root$$Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

BOOL C$$Root$$Doc::OnNewDocument()
{
	if (!CIUIDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

BOOL C$$Root$$Doc::CanCloseUIView(CIUIView* pUIView)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// C$$Root$$Doc commands
