#pragma once


#include "IUIMFCPropertyGridCtrl.h"


class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CPropertiesWnd : public CDockablePane
{
	friend class CUIShopDoc;

// Construction
public:
	CPropertiesWnd();
	virtual ~CPropertiesWnd();

protected:
	void SetPropListFont();

	int AddSubItem(CMFCPropertyGridProperty *pParent, const IUI_VARIANT *pVariant);
	int UpdatePropertyAndUI(IUI_VARIANT *pVariant, const IUI_VARIANT *pVarSetting);	// For undo and Redo
	int UpdatePropertyAndUI(CMFCPropertyGridProperty *pProp);
	CMFCPropertyGridProperty *SuperadditionProperties(CMFCPropertyGridProperty *pParentGridProp, CTRLPROPERTIES *pCtrlProp);

	int UpdateDisabledStatus(const IUI_VARIANT *pVariant, CMFCPropertyGridProperty *pItem);
	int UpdateSubPropertyDisabledStatus(CMFCPropertyGridProperty *pParentProp);
	int UpdateAllPropertiesDisabledStatus();

public:
	CMFCPropertyGridProperty *FindItemByString(CMFCPropertyGridProperty *pParentProp, const CString &strItemName);
	int ShowProperties(CTRLPROPERTIES *pCtrlProp);

	void AdjustLayout();
	void SetVSDotNetLook(BOOL bSet);
	int DeleteAllItems();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	CFont m_fntPropList;
	CIUIMFCPropertyGridCtrl m_wndPropList;
	const CTRLPROPERTIES *m_pCtrlProp;
};
