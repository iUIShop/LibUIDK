#include "StdAfx.h"
#include "Action.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CControlMove

CControlMove::CControlMove()
{
	m_pCtrlProp = NULL;
}

CControlMove::CControlMove(const CControlMove &ctrlMove)
{
	m_pCtrlProp = ctrlMove.m_pCtrlProp;
	m_rectOld = ctrlMove.m_rectOld;
	m_rectNew = ctrlMove.m_rectNew;
}

CControlMove::~CControlMove()
{
}

CControlMove &CControlMove::operator = (const CControlMove &ctrlMove)
{
	if (this == &ctrlMove)
		return *this;

	m_pCtrlProp = ctrlMove.m_pCtrlProp;
	m_rectOld = ctrlMove.m_rectOld;
	m_rectNew = ctrlMove.m_rectNew;

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// CControlDelete

CControlDelete::CControlDelete()
{
	m_pCtrlProp = NULL;
}

CControlDelete::CControlDelete(const CControlDelete &ctrlDelete)
{
	m_pCtrlProp = ctrlDelete.m_pCtrlProp;
}

CControlDelete::~CControlDelete()
{

}

CControlDelete &CControlDelete::operator =(const CControlDelete &ctrlDelete)
{
	if (this == &ctrlDelete)
		return *this;

	m_pCtrlProp = ctrlDelete.m_pCtrlProp;

	return *this;
}

//////////////////////////////////////////////////////////////////////////
//

::CAction::CAction()
{
	m_eAction = actionUnknown;
	m_pUIWndProp = NULL;
	m_pCtrlProp = NULL;
	m_pPropWnd = NULL;
	m_nPropIndexInCTRLPROPERTIES = -1;
}

::CAction::CAction(const CAction &action)
{
	m_eAction = action.m_eAction;
	m_pUIWndProp = action.m_pUIWndProp;
	m_pCtrlProp = action.m_pCtrlProp;

	POSITION pos = action.m_ControlMove.GetHeadPosition();
	while (pos != NULL)
	{
		CControlMove cm = action.m_ControlMove.GetNext(pos);

		m_ControlMove.AddTail(cm);
	}

	pos = action.m_ControlDelete.GetHeadPosition();
	while (pos != NULL)
	{
		CControlDelete cd = action.m_ControlDelete.GetNext(pos);

		m_ControlDelete.AddTail(cd);
	}

	m_pPropWnd = action.m_pPropWnd;
	m_nPropIndexInCTRLPROPERTIES = action.m_nPropIndexInCTRLPROPERTIES;
	m_varPropertyValueOld = action.m_varPropertyValueOld;
	m_varPropertyValueNew = action.m_varPropertyValueNew;
}

::CAction::~CAction()
{
}

::CAction &::CAction::operator = (const CAction &action)
{
	if (this == &action)
		return *this;

	m_eAction = action.m_eAction;
	m_pUIWndProp = action.m_pUIWndProp;
	m_pCtrlProp = action.m_pCtrlProp;

	POSITION pos = action.m_ControlMove.GetHeadPosition();
	while (pos != NULL)
	{
		CControlMove cm = action.m_ControlMove.GetNext(pos);

		m_ControlMove.AddTail(cm);
	}

	pos = action.m_ControlDelete.GetHeadPosition();
	while (pos != NULL)
	{
		CControlDelete cd = action.m_ControlDelete.GetNext(pos);

		m_ControlDelete.AddTail(cd);
	}

	m_pPropWnd = action.m_pPropWnd;
	m_nPropIndexInCTRLPROPERTIES = action.m_nPropIndexInCTRLPROPERTIES;
	m_varPropertyValueOld = action.m_varPropertyValueOld;
	m_varPropertyValueNew = action.m_varPropertyValueNew;

	return *this;
}
