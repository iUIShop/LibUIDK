// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "FileView.h"
#include "PropertiesWnd.h"
#include "StoryboardPane.h"
#include "UIShopView.h"
#include "ProjectGen.h"
#include "IUIMFCToolBar.h"


class CNewProjectDlg;

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	virtual ~CMainFrame();

public:
	int CustomizeProject(CNewProjectDlg *pDlg);
	void SetControlPosText(CString str);
	CONTROL_TYPE GetCurSelControlType();
	void SetCurSelControlType(CONTROL_TYPE eControlType);
	CUIShopView *GetUIShopView() { return m_pViewUIShop; }
	void SetUIShopView(CUIShopView *pView) { m_pViewUIShop = pView; }
	CUIShopDoc *GetUIShopDoc();
	CFileView *GetFileView();
	CPropertiesWnd *GetPropertiesWnd();
#ifdef ADV
	CStoryboardPane *GetStoryboardPanel();
#endif
	int ShowTBControlBar(BOOL bShow);
	int GetFileMRUMessage(UINT nID, CString *pstrMsg);

	BOOL IsFormatBrushSelected();
	const CTRLPROPERTIES *GetFormatBrushProp() const;
	int SetFormatBrushProp(const CTRLPROPERTIES *pProp);
	int AddFormatBrushPropIndex(int nIndex);
	const std::vector<int> *GetFormatBrushPropIndex() const;

	STORY_FRAME *InsertFrame();

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnFileNewLibuidkProject();
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg void OnHelpUIShop();
	afx_msg void OnHelpLibUIDK();
	afx_msg void OnHelpVideo();
	afx_msg void OnHelpWebLink();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSetMessageString(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetWindowText(WPARAM wParam, LPARAM lParam);
	afx_msg void OnControlSelect();
	afx_msg void OnUpdateControlSelect(CCmdUI* pCmdUI);
	afx_msg void OnControlButton();
	afx_msg void OnUpdateControlButton(CCmdUI* pCmdUI);
	afx_msg void OnControlCheckBox();
	afx_msg void OnUpdateControlCheckBox(CCmdUI* pCmdUI);
	afx_msg void OnControlRadioButton();
	afx_msg void OnUpdateControlRadioButton(CCmdUI* pCmdUI);
	afx_msg void OnControlStatic();
	afx_msg void OnUpdateControlStatic(CCmdUI* pCmdUI);
	afx_msg void OnControlEdit();
	afx_msg void OnUpdateControlEdit(CCmdUI* pCmdUI);
	afx_msg void OnControlComboBox();
	afx_msg void OnUpdateControlComboBox(CCmdUI* pCmdUI);
	afx_msg void OnControlSlider();
	afx_msg void OnUpdateControlSlider(CCmdUI* pCmdUI);
	afx_msg void OnControlSpin();
	afx_msg void OnUpdateControlSpin(CCmdUI* pCmdUI);
	afx_msg void OnControlProgress();
	afx_msg void OnUpdateControlProgress(CCmdUI* pCmdUI);
	afx_msg void OnControlHotKey();
	afx_msg void OnUpdateControlHotKey(CCmdUI* pCmdUI);
	afx_msg void OnControlIPAddress();
	afx_msg void OnUpdateControlIPAddress(CCmdUI* pCmdUI);
	afx_msg void OnControlSliderEx();
	afx_msg void OnUpdateControlSliderEx(CCmdUI* pCmdUI);
	afx_msg void OnControlListCtrl();
	afx_msg void OnUpdateControlListCtrl(CCmdUI* pCmdUI);
	afx_msg void OnControlTreeCtrl();
	afx_msg void OnUpdateControlTreeCtrl(CCmdUI* pCmdUI);
	afx_msg void OnControlScrollBarEx();
	afx_msg void OnUpdateControlScrollBarEx(CCmdUI* pCmdUI);
	afx_msg void OnControlThumbnail();
	afx_msg void OnUpdateControlThumbnail(CCmdUI* pCmdUI);
	afx_msg void OnControlTreeList();
	afx_msg void OnUpdateControlTreeList(CCmdUI* pCmdUI);
	afx_msg void OnControlRichEdit();
	afx_msg void OnUpdateControlRichEdit(CCmdUI* pCmdUI);
	afx_msg void OnControlRichEditIM();
	afx_msg void OnUpdateControlRichEditIM(CCmdUI* pCmdUI);
	afx_msg void OnControlHtmlCtrl();
	afx_msg void OnUpdateControlHtmlCtrl(CCmdUI* pCmdUI);
	afx_msg void OnControlDateTimeCtrl();
	afx_msg void OnUpdateControlDateTimeCtrl(CCmdUI* pCmdUI);
	afx_msg void OnControlWaveCtrl();
	afx_msg void OnUpdateControlWaveCtrl(CCmdUI* pCmdUI);
	afx_msg void OnControlPicCtrl();
	afx_msg void OnUpdateControlPicCtrl(CCmdUI* pCmdUI);
	afx_msg void OnControlMenuBar();
	afx_msg void OnUpdateControlMenuBar(CCmdUI* pCmdUI);
	afx_msg void OnControlSplitter();
	afx_msg void OnUpdateControlSplitter(CCmdUI* pCmdUI);
	afx_msg void OnControlRect();
	afx_msg void OnUpdateControlRect(CCmdUI* pCmdUI);
	afx_msg void OnControlSplitterWL();
	afx_msg void OnUpdateControlSplitterWL(CCmdUI* pCmdUI);
	afx_msg void OnControlLine();
	afx_msg void OnUpdateControlLine(CCmdUI* pCmdUI);
	afx_msg void OnControlText();
	afx_msg void OnUpdateControlText(CCmdUI* pCmdUI);
	afx_msg void OnControlButtonWL();
	afx_msg void OnUpdateControlButtonWL(CCmdUI* pCmdUI);
	afx_msg void OnControlCheckBoxWL();
	afx_msg void OnUpdateControlCheckBoxWL(CCmdUI* pCmdUI);
	afx_msg void OnControlRadioWL();
	afx_msg void OnUpdateControlRadioWL(CCmdUI* pCmdUI);
	afx_msg void OnControlSliderWL();
	afx_msg void OnUpdateControlSliderWL(CCmdUI* pCmdUI);
	afx_msg void OnControlPic();
	afx_msg void OnUpdateControlPic(CCmdUI* pCmdUI);
	afx_msg void OnControlRichEditWL();
	afx_msg void OnUpdateControlRichEditWL(CCmdUI* pCmdUI);
	afx_msg void OnControlIMWL();
	afx_msg void OnUpdateControlIMWL(CCmdUI* pCmdUI);
	afx_msg void OnControlTaskWndMgr();
	afx_msg void OnUpdateControlTaskWndMgr(CCmdUI *pCmdUI);
	afx_msg void OnControlDockPnl();
	afx_msg void OnUpdateControlDockPnl(CCmdUI *pCmdUI);
	afx_msg void OnControlUniformGridPnl();
	afx_msg void OnUpdateControlUniformGridPnl(CCmdUI *pCmdUI);
	afx_msg void OnControlStackPnl();
	afx_msg void OnUpdateControlStackPnl(CCmdUI *pCmdUI);
	afx_msg void OnViewControlBar();
	afx_msg void OnDesignSizeToContent();
	afx_msg void OnUpdateDesignSizeToContent(CCmdUI *pCmdUI);
	afx_msg void OnDesignSizeToForegroundContent();
	afx_msg void OnUpdateDesignSizeToForegroundContent(CCmdUI *pCmdUI);
	afx_msg void OnDesignFormatBrush();
	afx_msg void OnUpdateDesignFormatBrush(CCmdUI *pCmdUI);
	afx_msg void OnDesignBackgroundShow();
	afx_msg void OnUpdateDesignBackgroundShow(CCmdUI *pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditUnusedImageSymbols();
	afx_msg void OnUpdateEditUnusedImageSymbols(CCmdUI *pCmdUI);
	afx_msg void OnEditUnfoundImageSymbols();
	afx_msg void OnUpdateEditUnfoundImageSymbols(CCmdUI *pCmdUI);
	afx_msg void OnToolsCreateLanguageFile();
	afx_msg void OnUpdateToolsCreateLanguageFile(CCmdUI *pCmdUI);
	afx_msg void OnToolsCreateResourceFile();
	afx_msg void OnUpdateToolsCreateResourceFile(CCmdUI *pCmdUI);
	afx_msg void OnToolsTransformDialog();
	afx_msg void OnToolsCreateClassInfoFile();
	afx_msg void OnToolsGenerateResourceH();
	afx_msg void OnToolsSyncSkinTabOrder();
	afx_msg void OnToolsRenameTemplateFile();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

private:
	CONTROL_TYPE m_eCurControl;

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CIUIMFCToolBar    m_wndToolBar;
	CIUIMFCToolBar    m_wndControlBar;
	CIUIMFCToolBar    m_wndLayoutBar;
	CMFCStatusBar     m_wndStatusBar;
	CFileView         m_wndFileView;
	CPropertiesWnd    m_wndProperties;
#ifdef ADV
	CStoryboardPane	  m_wndStoryboard;
#endif
	CUIShopView		 *m_pViewUIShop;
	CProjectGen       m_ProjectGen;

	UINT m_nCtrlPropClipboardFormat;

	BOOL m_bFormatBrush;
	const CTRLPROPERTIES *m_pFormatBrushProp;
	std::vector<int> m_vBrushPropIndex;
};
