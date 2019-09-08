// Panel.cpp: implementation of the CPanel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPanel::CPanel()
{
	m_pParentPanel = NULL;
	m_bRedraw = TRUE;
}

CPanel::~CPanel()
{
}

int CPanel::InsertPanel(CPanel *pChildPanel, UINT nPos)
{
	ASSERT(FALSE);
	return -1;
}

int CPanel::RelayoutElements(BOOL bRedraw/* = FALSE*/)
{
	ASSERT(FALSE);
	return -1;
}

int CPanel::RemoveAllElements()
{
	ASSERT(FALSE);
	return -1;
}

BOOL CPanel::IsCreated()
{
	return (GetParent() != NULL);
}

int CPanel::SetParentPanel(CPanel *pParentPanel)
{
	m_pParentPanel = pParentPanel;

	return 0;
}

CPanel *CPanel::GetParentPanel()
{
	return m_pParentPanel;
}

int CPanel::SetRedraw(BOOL bRedraw)
{
	m_bRedraw = bRedraw;

	return 0;
}

int CPanel::Redraw()
{
	ASSERT(GetParent() != NULL);
	GetParent()->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

	return 0;
}
