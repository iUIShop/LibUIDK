#pragma once

class CPropertiesWnd;


class CControlMove
{
public:
	CControlMove();
	CControlMove(const CControlMove &ctrlMove);
	~CControlMove();

	CControlMove &operator = (const CControlMove &ctrlMove);

public:
	CTRLPROPERTIES *m_pCtrlProp;
	CRect m_rectOld;
	CRect m_rectNew;
};

class CControlDelete
{
public:
	CControlDelete();
	CControlDelete(const CControlDelete &ctrlDelete);
	~CControlDelete();

	CControlDelete &operator = (const CControlDelete &ctrlDelete);

public:
	CTRLPROPERTIES *m_pCtrlProp;
};

#define MAX_UNDO					100000
//////////////////////////////////////////////////////////////////////////
// An action for Undo and Redo.

class CAction
{
public:
	CAction();
	CAction(const CAction &action);
	~CAction();

	CAction &operator = (const CAction &action);

public:
	enum Actions
	{
		actionUnknown,
		actionNewControl,
		actionDeleteControl,
		actionMoveControl,

		actionNewWindow,
		actionDeleteWindow,

		actionNewResource,
		actionDeleteResource,

		actionNewStyle,
		actionDeleteStyle,

		actionModifyProperty
	};

public:
	Actions m_eAction;

	UIWNDPROPERTIES *m_pUIWndProp;		// A pointer point to CUIWnd property attach to CUIDesignWnd.
	CTRLPROPERTIES *m_pCtrlProp;		// A pointer point to control, UI window, resource or style property.

	// Record move control(s)
	CList<CControlMove> m_ControlMove;	// Undo or Redo together move.

	// Record delete control(s)
	CList<CControlDelete> m_ControlDelete;	// Undo or Redo together delete

	// Record property modify, Don't record the IUI_VARIANT pointer of CTRLPROPERTIES, 
	// Since control CControlWnd may be recreated while redo, the CTRLPROPERTIES will be re-alloc.
	// While recreate CControlWnd, the m_pCtrlProp will be re-assign.
	CPropertiesWnd *m_pPropWnd;
	int m_nPropIndexInCTRLPROPERTIES;
	IUI_VARIANT m_varPropertyValueOld;
	IUI_VARIANT m_varPropertyValueNew;
};
