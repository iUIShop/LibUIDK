// StartMenu.cpp : implementation of the CStartMenu class
//

#include "stdafx.h"
#include "$$root$$.h"
#include "StartMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStartMenu

BEGIN_MESSAGE_MAP(CStartMenu, CMenuWnd)
	//{{AFX_MSG_MAP(CStartMenu)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStartMenu construction/destruction

CStartMenu::CStartMenu()
{
}

CStartMenu::~CStartMenu()
{
}

void CStartMenu::DoDataExchange(CDataExchange* pDX)
{
	CMenuWnd::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CStartMenu)
	//}}AFX_DATA_MAP
}


//////////////////////////////////////////////////////////////////////////
// protected

int CStartMenu::InitControls()
{

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CStartMenu message handlers

int CStartMenu::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMenuWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (InitControls() != 0)
		return -1;


	return 0;
}
