#pragma once

#include "ViewTree.h"
#include "TestWnd.h"
#include "UIDesignWnd.h"
#include "ProjectGen.h"

enum TREE_ITEM_TYPE
{
	WINDOWS_ROOT = 1,
		WINDOWS_NODE,
			CONTROL_NODE,
	RESOURCE_ROOT = 10,
		RES_DEFAULT_FONT_BRANCH,
		RES_FONT_BRANCH,
			RES_GENERAL_FONT_NODE,
			RES_CONDITION_FONT_NODE,
			RES_CONDITION_CHILD_FONT_NODE,
		RES_DEFAULT_COLOR_BRANCH,
		RES_COLOR_BRANCH,
			RES_COLOR_NODE,
	STYLE_ROOT = 100,
		STYLE_PUSHBUTTON_ROOT,
		STYLE_CHECK_ROOT,
		STYLE_RADIO_ROOT,
		STYLE_COMBOBOX_ROOT,
		STYLE_STATIC_ROOT,
		STYLE_EDIT_ROOT,
		STYLE_SLIDER_ROOT,
		STYLE_PROGRESS_ROOT,
		STYLE_SPIN_ROOT,
		STYLE_HOTKEY_ROOT,
		STYLE_IPADDRESS_ROOT,
		STYLE_SLIDEREX_ROOT,
		STYLE_LISTCTRL_ROOT,
		STYLE_TREECTRL_ROOT,
		STYLE_TREELIST_ROOT,
		STYLE_RICHEDIT_ROOT,
		STYLE_IMRICHEDIT_ROOT,
		STYLE_HTMLCTRL_ROOT,
		STYLE_DATETIMECTRL_ROOT,
		STYLE_WAVECTRL_ROOT,
		STYLE_MENU_ROOT,
		STYLE_MENUBAR_ROOT,
		STYLE_HEADERCTRL_ROOT,
		STYLE_HORSCROLLBAR_ROOT,
		STYLE_VERSCROLLBAR_ROOT,
		STYLE_SPLITTER_ROOT,
		STYLE_WL_LINE_ROOT,
		STYLE_WL_PIC_ROOT,
			STYLE_PUSHBUTTON_NODE,
			STYLE_CHECK_NODE,
			STYLE_RADIO_NODE,
			STYLE_COMBOBOX_NODE,
			STYLE_STATIC_NODE,
			STYLE_EDIT_NODE,
			STYLE_SLIDER_NODE,
			STYLE_PROGRESS_NODE,
			STYLE_SPIN_NODE,
			STYLE_HOTKEY_NODE,
			STYLE_IPADDRESS_NODE,
			STYLE_SLIDEREX_NODE,
			STYLE_LISTCTRL_NODE,
			STYLE_TREECTRL_NODE,
			STYLE_TREELIST_NODE,
			STYLE_RICHEDIT_NODE,
			STYLE_IMRICHEDIT_NODE,
			STYLE_HTMLCTRL_NODE,
			STYLE_DATETIMECTRL_NODE,
			STYLE_WAVECTRL_NODE,
			STYLE_MENU_NODE,
			STYLE_MENUBAR_NODE,
			STYLE_HEADERCTRL_NODE,
			STYLE_HORSCROLLBAR_NODE,
			STYLE_VERSCROLLBAR_NODE,
			STYLE_SPLITTER_NODE,
			STYLE_WL_LINE_NODE,
			STYLE_WL_PIC_NODE
};

struct TREE_ITEMDATA
{
	TREE_ITEMDATA()
	{
		eTreeItemType = WINDOWS_ROOT;
		pResourceOrStyle = NULL;
		pResFont = NULL;
	}
	virtual ~TREE_ITEMDATA()
	{

	}

	TREE_ITEM_TYPE eTreeItemType;
	CUIDesignWnd wndUIDesign;
	CTestWnd wndTest;
	CTRLPROPERTIES *pResourceOrStyle;
	FONTRESOURCEITEM *pResFont;
};


class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd * /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd *)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const
	{
		return FALSE;
	}
};

class CFileView : public CDockablePane
{
public:
	CFileView();
	virtual ~CFileView();

protected:
	int InsertWindowsGroup(HTREEITEM hRoot);

	int InitFontResourceBranch();
	HTREEITEM InsertConditionFont(HTREEITEM hParentItem, FONTRESOURCEITEM *pConditionFont);
	int InsertConditionFontChildren(HTREEITEM hParentItem, FONTRESOURCEITEM *pConditionFont);
	HTREEITEM InsertGeneralFont(HTREEITEM hParentItem, FONTRESOURCEITEM *pGeneralFont);

	int InitColorResourceBranch();

	HTREEITEM InsertEachStyle(LPCTSTR lpszNode, TREE_ITEM_TYPE eTreeItemTypeRoot, TREE_ITEM_TYPE eTreeItemTypeNode, STYLE_TYPE eStyleType, CONTROL_TYPE ectStyleType);
	int InitStyleBrach();

	int UpdateUIWindowsCount();

public:
	void AdjustLayout();
	void OnChangeVisualStyle();

	BOOL OnOpenFile();
	CViewTree *GetWindowTree()
	{
		return &m_treeWindow;
	}

	HTREEITEM FindItem(LPCTSTR lpszWindowID, HTREEITEM hItem);
	HTREEITEM FindItem(const CTRLPROPERTIES *pProp, HTREEITEM hItem);

	int InsertResource(TREE_ITEM_TYPE eTreeItemType);
	int InsertStyle(TREE_ITEM_TYPE eTreeItemType);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint point);
	afx_msg void OnInsertWindow();
	afx_msg void OnWindowProperties();
	afx_msg void OnTestWindow();
	afx_msg void OnTestHighDpi();
	afx_msg void OnTestCurFrame();
	afx_msg void OnDeleteWindow();
	afx_msg void OnTabOrder();
	afx_msg void OnFindInHD();
	afx_msg void OnUpdateDeletewindow(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTaborder(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTest(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTestHighDpi(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTestCurFrame(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProperties(CCmdUI *pCmdUI);
	afx_msg void OnAddClass();
	afx_msg void OnUpdateAddClass(CCmdUI *pCmdUI);
	afx_msg void OnAddAllClasses();
	afx_msg void OnUpdateAddAllClasses(CCmdUI *pCmdUI);
	afx_msg void OnAddEventHandlerClass();
	afx_msg void OnUpdateAddEventHandlerClass(CCmdUI *pCmdUI);
	afx_msg void OnAddAllEventHandlerClasses();
	afx_msg void OnUpdateAddAllEventHandlerClasses(CCmdUI *pCmdUI);
	afx_msg void OnInsertGeneralFont();
	afx_msg void OnInsertConditionFontChild();
	afx_msg void OnUpdateInsertConditionFont(CCmdUI *pCmdUI);
	afx_msg void OnInsertResColor();
	afx_msg void OnDeleteResource();
	afx_msg void OnConvertToConditionFont();
	afx_msg void OnUpdateConvertToConditionFont(CCmdUI *pCmdUI);
	// Style
	afx_msg void OnUpdateDeleteResource(CCmdUI *pCmdUI);
	afx_msg void OnInsertStyleItem();
	afx_msg void OnUpdateInsertStyleItem(CCmdUI *pCmdUI);
	afx_msg void OnDeleteStyle();
	afx_msg void OnUpdateDeleteStyle(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()

protected:
	CViewTree m_treeWindow;
	HTREEITEM m_hWindowsRoot;
	HTREEITEM m_hResourceRoot;
		HTREEITEM m_hResDefaultFontBranch;
			HTREEITEM m_hResDefaultFont;
		HTREEITEM m_hResFontBranch;
		HTREEITEM m_hResDefaultColorBranch;
			HTREEITEM m_hResDefaultColor;
		HTREEITEM m_hResColorBranch;
	HTREEITEM m_hStyleRoot;
		HTREEITEM m_hStylePushButtonRoot;
		HTREEITEM m_hStyleCheckRoot;
		HTREEITEM m_hStyleRadioRoot;
		HTREEITEM m_hStyleComboBoxRoot;
		HTREEITEM m_hStyleStaticRoot;
		HTREEITEM m_hStyleEditRoot;
		HTREEITEM m_hStyleSliderRoot;
		HTREEITEM m_hStyleProgressRoot;
		HTREEITEM m_hStyleSpinRoot;
		HTREEITEM m_hStyleHotKeyRoot;
		HTREEITEM m_hStyleIPAddressRoot;
		HTREEITEM m_hStyleSliderExRoot;
		HTREEITEM m_hStyleListCtrlRoot;
		HTREEITEM m_hStyleTreeCtrlRoot;
		HTREEITEM m_hStyleTreeListRoot;
		HTREEITEM m_hStyleRichEditRoot;
		HTREEITEM m_hStyleIMRichEditRoot;
		HTREEITEM m_hStyleHtmlCtrlRoot;
		HTREEITEM m_hStyleDateTimeCtrlRoot;
		HTREEITEM m_hStyleWaveCtrlRoot;
		HTREEITEM m_hStyleMenuRoot;
		HTREEITEM m_hStyleMenuBarRoot;
		HTREEITEM m_hStyleHeaderRoot;
		HTREEITEM m_hStyleHorScrollbarRoot;
		HTREEITEM m_hStyleVerScrollbarRoot;
		HTREEITEM m_hStyleSplitterRoot;
		HTREEITEM m_hStyleWLLineRoot;
		HTREEITEM m_hStyleWLPicRoot;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;
};
